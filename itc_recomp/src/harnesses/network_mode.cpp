// The type 12 mode, the game's Xbox Live mode (src/network_mode/,
// docs/functions/network-mode.md), against the recompiled functions. Each
// case fills a world of its own (one block from the allocation layer: the
// type 12 block with its fourteen machines at their offsets, the mode with
// its message ring, the delegate functions' answer and a fake sound
// manager) with random words from the case's seed; for the delegate
// functions shapes it as play has it and points the globals they read into
// it (put back after), now and then with a session of the layer's pages
// made for the side (Session::Free frees it); runs the original (with
// --native_network_mode off, so what it calls of the mode's is the original
// too) or the register steps (hooks/network_mode.h); and compares the
// world, the globals, whether the session and its buffers are still
// allocated, the function's frames without their register saves, the
// caller's area above r1, r1 and r3. Teardown's cases are in
// type67_modes.cpp (the stand's world), NextMode's time attack in
// race_setup.cpp (the race data's).

#include "harness.h"

#include "loading_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/heap.h"
#include "hooks/network_mode.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "network_mode/network_mode.h"
#include "race_loading/race_loading.h"

#include <fmt/format.h>

#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_network_mode);

namespace harness {

namespace {

namespace nm = network_mode;

constexpr uint32_t kBlockBytes = 0x21100;  // the type 12 block (135,208 bytes)
constexpr uint32_t kModeAt = 0x21100;      // the mode (852 bytes; its ring at +240)
constexpr uint32_t kAnswerAt = 0x21500;    // the delegate functions' answer
constexpr uint32_t kManagerAt = 0x21600;   // the sound manager (its vtable +0x100)
constexpr uint32_t kWorldBytes = 0x21800;
constexpr uint32_t kStackBytes = 8192;     // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;     // above it
// The session's pages (Session::Release reads its +0x13CD34).
constexpr uint32_t kSessionBytes = 0x13D000;

// The globals the delegate functions read or write (saved, compared, put
// back): the sound manager, the fades, the loading screen's current movie,
// the sign-in watch's word, the title's flag, the session and its buffer.
struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {nm::kSoundManager, 4},     {0x826A50AC, 64},           {race_loading::kLoadingCurrent, 4},
    {nm::kSignInWatch + 16, 4}, {nm::kTitleFlag, 4},        {nm::kSession, 4},
    {nm::kSessionBuffer, 4},
};

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kWorldBytes + 16, 0)),
        block((allocation + 15) & ~15u) {}
  ~World() { memory::Free(allocation); }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t mode() const { return at(kModeAt); }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

struct Spec {
  std::string name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
  bool loading = false;  // the loading screen's world shaped and compared
};

// The loading screen's world (WaitForLoads runs and finishes the loading
// screen's current movie), as the type 9 mode's harness shapes it.
const loading_world::MovieWorld* g_loading = nullptr;
void ShapeLoading(uint8_t* base, Random& r) {
  namespace lw = loading_world;
  lw::ShapeMovieWorld(base, *g_loading, r);
  lw::BuildStringTable(base, *g_loading);
  guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
  guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                    r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
  }
}
// UiMovie::Run's update (sub_8211D2F8), stood in for as the type 9 mode's
// harness does: each call's return address, stack pointer (from the
// case's), r3 and r4 kept and answered 0 or 1 from the case's generator
// (the update would draw the movie).
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall> g_updates;
Random g_answers;
uint32_t g_case_sp = 0;
bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8) {
    return false;
  }
  g_updates.push_back({uint32_t(ctx.lr), g_case_sp - ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Below(2);
  return true;
}

void TakeLoading(uint8_t* base, Outputs& out) {
  out.Bytes(Read(base, g_loading->movies.address, g_loading->movies.bytes));
  out.Bytes(Read(base, g_loading->texts.address, g_loading->texts.bytes));
  out.Word(guest::LoadU32(base, race_loading::kLoadingShown));
  out.Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
}

// The session a side made (the layer's pages) and its two buffers (heap
// blocks), checked after the run: whether each is still allocated (then
// freed).
struct Session {
  uint32_t pages = 0, rows = 0, buffer = 0;
};
Session g_session;

void MakeSession(uint8_t* base, Random& r) {
  g_session = {};
  if (!hooks::NativeHeapActive() || r.Below(3) != 0) {
    guest::StoreU32(base, nm::kSession, 0);
    return;
  }
  g_session.pages = memory::AllocPages(kSessionBytes, 4096);
  g_session.rows = r.Below(2) == 0 ? 0 : memory::Alloc(32, 0);
  g_session.buffer = r.Below(2) == 0 ? 0 : memory::Alloc(32, 0);
  guest::StoreU32(base, g_session.pages + nm::kSessionRows, g_session.rows);
  guest::StoreU32(base, nm::kSessionBuffer, g_session.buffer);
  guest::StoreU32(base, nm::kSession, g_session.pages);
}

void TakeSession(uint8_t* base, Outputs& out) {
  // The global named by its role (the session made, or another word).
  const uint32_t session = guest::LoadU32(base, nm::kSession);
  out.Word(session != 0 && session == g_session.pages ? 1 : session);
  guest::StoreU32(base, nm::kSession, 0);
  if (g_session.pages != 0) {
    const bool pages = memory::IsPages(g_session.pages);
    out.Word(pages ? 1 : 0);
    if (pages) {
      out.Word(guest::LoadU32(base, g_session.pages + nm::kSessionRows) == g_session.rows);
      memory::FreePages(g_session.pages);
    }
  }
  for (const uint32_t block : {g_session.rows, g_session.buffer}) {
    if (block != 0) {
      const bool held = memory::IsHeapBlock(block);
      out.Word(held ? 1 : 0);
      if (held) {
        memory::Free(block);
      }
    }
  }
  const uint32_t buffer = guest::LoadU32(base, nm::kSessionBuffer);
  out.Word(buffer != 0 && buffer == g_session.buffer ? 1 : buffer);
  g_session = {};
}

void Run(Context& h, const World& w, const Spec& spec) {
  Results results(spec.name);
  for (int c = 0; c < spec.cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(h.base, g.address, g.bytes));
      }
      WriteRandom(h.base, w.block, kWorldBytes, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      g_session = {};
      const loading_world::SavedGlobals loading(h.base);
      spec.setup(t, r, w);
      const bool shaped = spec.loading && g_loading != nullptr;
      if (shaped) {
        ShapeLoading(h.base, r);
      }
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      if (v == 0) {
        const bool native = ITC_FLAG(native_network_mode);
        ITC_FLAG(native_network_mode) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_network_mode) = native;
      } else {
        spec.native(t, h.base);
      }
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      out[v].Word(uint32_t(g_updates.size()));
      for (const UpdateCall& call : g_updates) {
        out[v].Word(call.lr);
        out[v].Word(call.r1);
        out[v].Word(uint32_t(call.r3 >> 32));
        out[v].Word(uint32_t(call.r3));
        out[v].Word(uint32_t(call.r4 >> 32));
        out[v].Word(uint32_t(call.r4));
      }
      TakeSession(h.base, out[v]);
      if (shaped) {
        TakeLoading(h.base, out[v]);
      }
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kWorldBytes));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
      loading.Restore();
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The machine empty or holding random transitions with keys of small
// numbers, some the Init's (Add replaces those), as the type 9 harness's.
void InitMachine(Random& r, uint8_t* base, uint32_t machine) {
  namespace gm = game_modes;
  if (r.Below(2) == 0) {
    gm::StateMachineConstruct(base, machine);
    guest::StoreU32(base, machine + gm::kTransitionCount, 0);
  } else {
    const uint32_t count = 8 + r.Below(160);
    guest::StoreU32(base, machine + gm::kTransitionCount, count);
    for (uint32_t i = 0; i < gm::kTransitions; i++) {
      const uint32_t at = machine + gm::kTransitionBytes * i;
      guest::StoreU32(base, at, r.Below(12));
      guest::StoreU32(base, at + 4, r.Below(48));
      guest::StoreU32(base, at + 8, r.Below(8) == 0 ? 1 : 0);
      guest::StoreU32(base, at + 12, 0);
    }
  }
}

// The machines by their InitStates.
struct Machine {
  const char* name;
  uint32_t address;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t at;
  uint32_t frame;
  // Machines 6 to 13: their index and base Init.
  uint32_t index = 0;
  const char* base_name = nullptr;
  uint32_t base_address = 0;
  GuestFunction base_recompiled = nullptr;
  GuestFunction base_native = nullptr;
};

const Machine kMachines[] = {
    {"NetModeSelect::InitStates", 0x8227D680, __imp__sub_8227D680,
     hooks::NetModeSelectInitStates, nm::kModeSelectAt, nm::kModeSelectInitFrame},
    {"Type12Screen1::InitStates", 0x82276510, __imp__sub_82276510,
     hooks::Type12Screen1InitStates, nm::kScreen1At, nm::kScreen1InitFrame},
    {"Type12Screen3::InitStates", 0x8225F330, __imp__sub_8225F330,
     hooks::Type12Screen3InitStates, nm::kScreen3At, nm::kScreen3InitFrame},
    {"NetRanking::InitStates", 0x822873A0, __imp__sub_822873A0, hooks::NetRankingInitStates,
     nm::kRankingAt, nm::kRankingInitFrame},
    {"NetTaCourse::InitStates", 0x82281858, __imp__sub_82281858, hooks::NetTaCourseInitStates,
     nm::kTaCourseAt, nm::kTaCourseInitFrame},
    {"PowerGraphScreen12::InitStates", 0x821B96B8, __imp__sub_821B96B8,
     hooks::PowerGraphScreen12InitStates, nm::kPowerGraphAt, nm::kPowerGraphInitFrame},
    {"Type12Screen6::InitStates", 0x82268410, __imp__sub_82268410,
     hooks::Type12Screen6InitStates, nm::kScreen6At, nm::kScreen6InitFrame, 6,
     "Type12Screen6::InitBase", 0x82278C20, __imp__sub_82278C20, hooks::Type12Screen6InitBase},
    {"Type12Screen7::InitStates", 0x8226EB40, __imp__sub_8226EB40,
     hooks::Type12Screen7InitStates, nm::kScreen7At, nm::kScreen7InitFrame, 7,
     "Type12Screen7::InitBase", 0x82279118, __imp__sub_82279118, hooks::Type12Screen7InitBase},
    {"Type12Screen8::InitStates", 0x82262A88, __imp__sub_82262A88,
     hooks::Type12Screen8InitStates, nm::kScreen8At, nm::kScreen8InitFrame, 8,
     "Type12Screen8::InitBase", 0x8227A000, __imp__sub_8227A000, hooks::Type12Screen8InitBase},
    {"Type12Screen9::InitStates", 0x822646B0, __imp__sub_822646B0,
     hooks::Type12Screen9InitStates, nm::kScreen9At, nm::kScreen9InitFrame, 9,
     "Type12Screen9::InitBase", 0x8227A4F8, __imp__sub_8227A4F8, hooks::Type12Screen9InitBase},
    {"Type12Dialog10::InitStates", 0x82288600, __imp__sub_82288600,
     hooks::Type12Dialog10InitStates, nm::kDialog10At, nm::kDialog10InitFrame, 10,
     "Type12Dialog10::InitBase", 0x82279610, __imp__sub_82279610,
     hooks::Type12Dialog10InitBase},
    {"Type12Dialog11::InitStates", 0x82277B18, __imp__sub_82277B18,
     hooks::Type12Dialog11InitStates, nm::kDialog11At, nm::kDialog11InitFrame, 11,
     "Type12Dialog11::InitBase", 0x82279B08, __imp__sub_82279B08,
     hooks::Type12Dialog11InitBase},
    {"Type12Dialog12::InitStates", 0x8227F6C8, __imp__sub_8227F6C8,
     hooks::Type12Dialog12InitStates, nm::kDialog12At, nm::kDialog12InitFrame, 12,
     "Type12Dialog12::InitBase", 0x8227A9F0, __imp__sub_8227A9F0,
     hooks::Type12Dialog12InitBase},
    {"Type12Dialog13::InitStates", 0x8227EA50, __imp__sub_8227EA50,
     hooks::Type12Dialog13InitStates, nm::kDialog13At, nm::kDialog13InitFrame, 13,
     "Type12Dialog13::InitBase", 0x8227AEE8, __imp__sub_8227AEE8,
     hooks::Type12Dialog13InitBase},
};

// Another machine of the block than `m`'s, for the base Inits' r4 now and
// then (a machine-sized block of the world).
uint32_t OtherMachine(Random& r, const World& w, const Machine& m) {
  const Machine& other = kMachines[r.Below(std::size(kMachines))];
  return w.at(other.at == m.at ? nm::kModeSelectAt : other.at);
}

// The base Init's frame (below the InitStates' or the dispatcher's caller's
// r1 by `above`) without its register saves.
Range BaseFrame(int32_t above) {
  return {-above - int32_t(nm::kInitBaseFrame), -above - 48};
}

void Inits(Context& h, const World& w) {
  for (const Machine& m : kMachines) {
    std::vector<Range> stack = {{-int32_t(m.frame), -48}};
    if (m.index != 0) {
      stack.push_back(BaseFrame(int32_t(m.frame)));
    }
    Run(h, w, {fmt::format("{} (sub_{:08X})", m.name, m.address), 24, m.recompiled, m.native,
               stack, [&m](PPCContext& t, Random& r, const World& w) {
                 InitMachine(r, w.base, w.at(m.at));
                 t.r3.u64 = w.at(m.at);
               }});
  }
  for (const Machine& m : kMachines) {
    if (m.index == 0) {
      continue;
    }
    // r4 the machine (play) or, one case in four, another machine.
    Run(h, w, {fmt::format("{} (sub_{:08X})", m.base_name, m.base_address), 24,
               m.base_recompiled, m.base_native, {BaseFrame(0)},
               [&m](PPCContext& t, Random& r, const World& w) {
                 InitMachine(r, w.base, w.at(m.at));
                 t.r3.u64 = w.at(m.at);
                 t.r4.u64 = r.Below(4) == 0 ? OtherMachine(r, w, m) : w.at(m.at);
               }});
  }
  // NetScreen::InitBase for each index of play (6 to 13), r5 the machine
  // or, one case in four, another.
  for (const Machine& m : kMachines) {
    if (m.index == 0) {
      continue;
    }
    Run(h, w, {fmt::format("NetScreen::InitBase (sub_8227B3E0) index {}", m.index), 12,
               __imp__sub_8227B3E0, hooks::NetScreenInitBase, {BaseFrame(0)},
               [&m](PPCContext& t, Random& r, const World& w) {
                 InitMachine(r, w.base, w.at(m.at));
                 t.r3.u64 = w.at(m.at);
                 t.r4.u64 = m.index;
                 t.r5.u64 = r.Below(4) == 0 ? OtherMachine(r, w, m) : w.at(m.at);
               }});
  }
}

// --- The mode's delegate functions and block helpers ---

// The shown indexes play gives (2 is none: no machine).
constexpr uint32_t kShownIndexes[] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

// The world as the delegate functions find it in play: the mode's block,
// its ring's count and start small, no handlers; the machines' states 0 to
// 20 (now and then any word), their movies loaded (all of them half the
// time), their words +9624 and +9632 0 or 1; the shown index one of play's,
// the session made here (+0x21024) 0 or 1; the time attack's screen (5)
// never in state 11 (its race set-up writes the game's race data: its
// cases are in race_setup.cpp); the sound manager a fake whose slots
// (ReleaseBank 5, IsBankLoading 6, PlayCue2D 10, StopCue 13, IsCuePlaying
// 14) all answer its +60 (0, 1, 0x100 or any word); the loading screen's
// current movie 3 (Run and Finish do nothing); a session made now and then.
void Shape(PPCContext& t, Random& r, const World& w) {
  uint8_t* const base = w.base;
  const uint32_t mode = w.mode(), block = w.block;
  guest::StoreU32(base, mode + nm::kBlock, block);
  guest::StoreU32(base, mode + nm::kRing + 600, r.Below(40));
  guest::StoreU32(base, mode + nm::kRing + 604, r.Below(60));
  guest::StoreU32(base, mode + nm::kDispatcher + 200, 0);
  const bool all_loaded = r.Below(2) == 0;
  for (const uint32_t index : kShownIndexes) {
    const uint32_t machine = nm::Machine(block, index);
    const uint32_t state = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(21);
    guest::StoreU32(base, machine + nm::kMachineState, state);
    guest::StoreU32(base, machine + nm::kMachineLoaded,
                    all_loaded ? 1 + r.Below(2) * uint32_t(r.Next() & 0xFF)
                               : (r.Below(4) == 0 ? 0 : 1));
    guest::StoreU32(base, machine + nm::kMachineBack, r.Below(2));
    guest::StoreU32(base, machine + nm::kMachineAgain, r.Below(2));
  }
  // Now and then few machines running (FindShown keeps the index).
  if (r.Below(3) == 0) {
    for (const uint32_t index : kShownIndexes) {
      const uint32_t at = nm::Machine(block, index) + nm::kMachineState;
      const uint32_t state = guest::LoadU32(base, at);
      if ((state == 4 || state == 5) && r.Below(4) != 0) {
        guest::StoreU32(base, at, 6 + r.Below(10));
      }
    }
  }
  if (guest::LoadU32(base, block + nm::kTaCourseAt + nm::kMachineState) == 11) {
    guest::StoreU32(base, block + nm::kTaCourseAt + nm::kMachineState, 12);
  }
  guest::StoreU32(base, block + nm::kShown, kShownIndexes[r.Below(std::size(kShownIndexes))]);
  guest::StoreU32(base, block + nm::kSessionMade, r.Below(2));
  const uint32_t manager = w.at(kManagerAt), vtable = manager + 0x100;
  guest::StoreU32(base, manager, vtable);
  static constexpr uint32_t kAnswers[] = {0, 1, 0x100};
  guest::StoreU32(base, manager + 60,
                  r.Below(4) == 0 ? uint32_t(r.Next()) : kAnswers[r.Below(3)]);
  for (const uint32_t slot : {20u, 24u, 40u, 52u, 56u}) {
    guest::StoreU32(base, vtable + slot, 0x82090770);  // r3 = [r3 + 60]
  }
  guest::StoreU32(base, nm::kSoundManager, manager);
  guest::StoreU32(base, race_loading::kLoadingCurrent, 3);
  MakeSession(base, r);
  t.r3.u64 = w.at(kAnswerAt);
  t.r4.u64 = mode;
}

// r3 the mode + 32 (the +32 slots).
void ShapeObject(PPCContext& t, Random& r, const World& w) {
  Shape(t, r, w);
  t.r3.u64 = w.mode() + 32;
}

void Delegates(Context& h, const World& w) {
  Run(h, w, {"ModeType12::Machine (sub_82277E10)", 48, __imp__sub_82277E10,
             hooks::ModeType12Machine, {},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.block | (uint64_t(r.Below(2)) << 32);
               t.r4.u64 = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(17);
             }});
  Run(h, w, {"ModeType12::FindShown (sub_82277FF0)", 60, __imp__sub_82277FF0,
             hooks::ModeType12FindShown, {},
             [](PPCContext& t, Random& r, const World& w) {
               Shape(t, r, w);
               t.r3.u64 = w.block | (uint64_t(r.Below(2)) << 32);
             }});
  Run(h, w, {"ModeType12::AllLoaded (sub_82277EE0)", 60, __imp__sub_82277EE0,
             hooks::ModeType12AllLoaded, {},
             [](PPCContext& t, Random& r, const World& w) {
               Shape(t, r, w);
               t.r3.u64 = w.block;
             }});
  Run(h, w, {"ModeType12::WaitForLoads (sub_8227B440)", 150, __imp__sub_8227B440,
             hooks::ModeType12WaitForLoads, {{-176, -48}}, Shape, true});
  Run(h, w, {"ModeType12::NextMode (sub_8227B930)", 300, __imp__sub_8227B930,
             hooks::ModeType12NextMode, {{-4112, -104}}, Shape});
  Run(h, w, {"ModeType12::LeaveScreen (sub_8227B868)", 60, __imp__sub_8227B868,
             hooks::ModeType12LeaveScreen, {{-128, -32}}, Shape});
  Run(h, w, {"ModeType12::WaitForShown (sub_8227B770)", 100, __imp__sub_8227B770,
             hooks::ModeType12WaitForShown, {{-112, -16}}, Shape});
  Run(h, w, {"ModeType12::ForceExit (sub_822788D8)", 100, __imp__sub_822788D8,
             hooks::ModeType12ForceExit, {{-240, -64}, {-336, -256}, {-432, -352}},
             ShapeObject});
  Run(h, w, {"ModeType12::Resume (sub_82278B98)", 40, __imp__sub_82278B98,
             hooks::ModeType12Resume, {{-96, -8}}, ShapeObject});
  Run(h, w, {"Session::Free (sub_82292710)", 40, __imp__sub_82292710, hooks::SessionFree,
             {{-96, -16}, {-192, -112}}, Shape});
}

}  // namespace

// ModeType12::Init's cases, in src/harnesses/type67_modes.cpp (the stand's
// world: StandSaved, StandTake, StandRecorder), Teardown's there too (the
// stand as PollTeardown's cases lay it), NextMode's time attack in
// src/harnesses/race_setup.cpp (the race data's world).
void NetworkModeInits(Context& h);
void NetworkModeTeardowns(Context& h);
void NetworkModeTimeAttack(Context& h);

void NetworkMode(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: network_mode: no block from the allocation layer");
    return;
  }
  if (hooks::NativeHeapActive()) {
    Inits(h, w);
  }
  {
    const loading_world::MovieWorld loading;
    if (loading.ok()) {
      g_loading = &loading;
      Delegates(h, w);
      g_loading = nullptr;
    } else {
      ITC_LOG_ERROR("harness: network_mode: no blocks for the loading screen's world");
    }
  }
  NetworkModeInits(h);
  NetworkModeTeardowns(h);
  NetworkModeTimeAttack(h);
}

}  // namespace harness
