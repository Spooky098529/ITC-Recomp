// The type 9 mode, the career's end of the day (src/todays_result/,
// docs/functions/todays-result.md), against the recompiled functions, laid
// as the replay theater's harness lays its world (type10_mode.cpp). Each
// case fills a world of its own (one block from the allocation layer) with
// random words from the case's seed: the type 9 block (today's result's
// machine at +0, today's event's at +9640), the mode with its message ring
// and player 0's save data (a block of its own, [0x826C5B58] pointed at
// it), the fields each function tests shaped to their play ranges; points
// the globals into it (put back after); runs the original (with
// --native_todays_result off, so what it calls of the mode's is the
// original too) or the register steps (hooks/todays_result.h); and compares
// the world, the save data's words the functions read or write, the
// globals, the function's frame without its register saves, the caller's
// area above r1, r1 and r3.

#include "harness.h"

#include "loading_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/todays_result.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "race_loading/race_loading.h"
#include "todays_result/todays_result.h"

#include <fmt/format.h>

#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_todays_result);

namespace time_hooks {
void SetFakeNow(uint64_t now);
void ClearFakeNow();
}  // namespace time_hooks

namespace harness {

namespace {

namespace tr = todays_result;

// The block's layout.
constexpr uint32_t kTypeBlockAt = 0x0;  // the type 9 block (19,300 bytes)
constexpr uint32_t kModeAt = 0x4C00;    // the mode (852 bytes; its ring at +240)
constexpr uint32_t kAnswerAt = 0x4F80;  // the delegate functions' answer
constexpr uint32_t kBlockBytes = 0x5000;

// Player 0's save data (a block of its own).
constexpr uint32_t kSaveBytes = 0x1D600;
constexpr uint32_t kSaveTable = 0x826C5B58;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kFadesBytes = 64;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kSaveTable, 4},
    {kFades, kFadesBytes},
    {race_loading::kLoadingCurrent, 4},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), save(memory::Alloc(kSaveBytes, 0)) {}
  ~World() {
    memory::Free(allocation);
    memory::Free(save);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && save != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t result() const { return at(kTypeBlockAt); }
  uint32_t event() const { return at(kTypeBlockAt) + tr::kEventAt; }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t dispatcher() const { return mode() + tr::kDispatcher; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  uint32_t save;
};

// The states the machines take in play (and a random word now and then).
constexpr uint32_t kStates[] = {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 13, 20};

// The save data's words Kind reads: the unlocks just made's count (0, a
// few, or a random word) and the movie queued (-1, 0 to 8, or a random
// word).
void ShapeSave(uint8_t* base, const World& w, Random& r) {
  const uint32_t pick = r.Below(4);
  guest::StoreU32(base, w.save + tr::kUnlockCount,
                  pick == 0 ? 0 : pick == 3 ? uint32_t(r.Next()) : 1 + r.Below(20));
  const uint32_t movie = r.Below(5);
  guest::StoreU32(base, w.save + tr::kQueuedMovie,
                  movie == 0 ? 0xFFFFFFFFu : movie == 4 ? uint32_t(r.Next()) : r.Below(9));
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  // The mode's ring (the dispatcher's, at +240): count and start small; no
  // handlers.
  guest::StoreU32(base, w.mode() + tr::kRing + 600, r.Below(40));
  guest::StoreU32(base, w.mode() + tr::kRing + 604, r.Below(60));
  guest::StoreU32(base, w.dispatcher() + 200, 0);
  guest::StoreU32(base, w.mode() + tr::kBlock, w.result());
  // The machines' words: +9604 a state, +9608, +9612, +9616 0 or 1 or any;
  // the shown index 0 or 27 (the only ones the mode's functions store).
  for (const uint32_t machine : {w.result(), w.event()}) {
    guest::StoreU32(base, machine + tr::kMachineState,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kStates[r.Below(std::size(kStates))]);
    for (const uint32_t field : {tr::kMachineLoaded, tr::kMachineReenter, tr::kMachineDelete}) {
      guest::StoreU32(base, machine + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
    }
  }
  guest::StoreU32(base, w.result() + tr::kShown, r.Below(2) == 0 ? 0 : tr::kResultIndex);
  // Leaving's: now and then both machines in a leaving state.
  if (r.Below(3) == 0) {
    static constexpr uint32_t kLeft[] = {6, 13, 14, 12, 20};
    for (const uint32_t machine : {w.result(), w.event()}) {
      guest::StoreU32(base, machine + tr::kMachineState, kLeft[r.Below(5)]);
    }
  }
  ShapeSave(base, w, r);
  // The fades' words.
  WriteRandom(base, kFades, kFadesBytes, r);
}

void PointGlobals(uint8_t* base, const World& w) {
  guest::StoreU32(base, kSaveTable, w.save);
  // LoadingScreen::Run and Finish(-1): the current movie 3 (none: they do
  // nothing).
  guest::StoreU32(base, race_loading::kLoadingCurrent, 3);
}

// UiMovie::Run's update (sub_8211D2F8), stood in for: each call's return
// address, stack pointer (from the case's), r3 and r4 kept and answered 0
// or 1 from the case's generator (the update would draw the movie).
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

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read.
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
  AfterFn after = nullptr;
  // The loading screen's world shaped (loading_world.h) and compared:
  // LoadingScreen::Run and Finish(-1) on its current movie.
  bool loading = false;
};

// The loading screen's world, made once (Run's cases shape it when their
// spec asks).
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
void TakeLoading(uint8_t* base, Outputs& out) {
  out.Bytes(Read(base, g_loading->movies.address, g_loading->movies.bytes));
  out.Bytes(Read(base, g_loading->texts.address, g_loading->texts.bytes));
  out.Word(guest::LoadU32(base, race_loading::kLoadingShown));
  out.Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
}

// The save data's words compared: the two Kind reads.
void SaveWords(uint8_t* base, const World& w, Outputs& out) {
  out.Word(guest::LoadU32(base, w.save + tr::kUnlockCount));
  out.Word(guest::LoadU32(base, w.save + tr::kQueuedMovie));
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
      const loading_world::SavedGlobals loading(h.base);
      MakeWorld(h.base, w, r);
      PointGlobals(h.base, w);
      if (spec.loading) {
        ShapeLoading(h.base, r);
      }
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      if (v == 0) {
        const bool native = ITC_FLAG(native_todays_result);
        ITC_FLAG(native_todays_result) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_todays_result) = native;
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
      if (spec.after) {
        spec.after(t, w, out[v]);
      }
      SaveWords(h.base, w, out[v]);
      if (spec.loading) {
        TakeLoading(h.base, out[v]);
      }
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
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

// --- The mode ---

// r3 the answer, r4 the mode.
void Delegate(PPCContext& t, Random&, const World& w) {
  t.r3.u64 = w.at(kAnswerAt);
  t.r4.u64 = w.mode();
}

// The machine empty or holding random transitions with keys of small
// numbers, some the Init's (Add replaces those).
void InitMachine(PPCContext& t, Random& r, uint8_t* base, uint32_t machine) {
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
      guest::StoreU32(base, at + 4, r.Below(20));
      guest::StoreU32(base, at + 8, r.Below(8) == 0 ? 1 : 0);
      guest::StoreU32(base, at + 12, 0);
    }
  }
  t.r3.u64 = machine;
}

void Mode(Context& h, const World& w) {
  Run(h, w, {"TodaysResult::InitStates (sub_8225B358)", 24, __imp__sub_8225B358,
             hooks::TodaysResultInitStates, {{-976, -48}},
             [](PPCContext& t, Random& r, const World& w) {
               InitMachine(t, r, w.base, w.result());
             }});
  Run(h, w, {"TodaysEvent::InitStates (sub_8225CF30)", 24, __imp__sub_8225CF30,
             hooks::TodaysEventInitStates, {{-976, -48}},
             [](PPCContext& t, Random& r, const World& w) {
               InitMachine(t, r, w.base, w.event());
             }});
  Run(h, w, {"ModeType9::FindShown (sub_8225D658)", 60, __imp__sub_8225D658,
             hooks::ModeType9FindShown, {},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.result() | (uint64_t(r.Below(2)) << 32);
             }});
  Run(h, w, {"TodaysEvent::Kind (sub_8225BA48)", 100, __imp__sub_8225BA48,
             hooks::TodaysEventKind, {{-96, -8}}, [](PPCContext&, Random&, const World&) {}});
  Run(h, w, {"ModeType9::WaitForLoads (sub_8225D918)", 150, __imp__sub_8225D918,
             hooks::ModeType9WaitForLoads, {{-128, -40}}, Delegate, nullptr, true});
  Run(h, w, {"ModeType9::NextMode (sub_8225DC38)", 300, __imp__sub_8225DC38,
             hooks::ModeType9NextMode, {{-144, -56}}, Delegate});
  Run(h, w, {"ModeType9::LeaveScreen (sub_8225DB70)", 60, __imp__sub_8225DB70,
             hooks::ModeType9LeaveScreen, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType9::WaitForShown (sub_8225DA68)", 100, __imp__sub_8225DA68,
             hooks::ModeType9WaitForShown, {{-112, -16}}, Delegate});
  Run(h, w, {"ModeType9::Leaving (sub_8225DE78)", 150, __imp__sub_8225DE78,
             hooks::ModeType9Leaving, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType9::IsLoaded (sub_8225D6A8)", 40, __imp__sub_8225D6A8,
             hooks::ModeType9IsLoaded, {},
             [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.mode() + 32; }});
}

// --- ModeType9::Init: the mode random words (its dispatcher's count 0 to
// 7), the UI movies' start and Strings::Load recorded (their adapters'
// recorder), LoadingScreen::Show over the loading screen's world
// (loading_world.h, a made-up time for the tip), the screen fades' words
// saved and put back. Compared: the recorded calls, the mode, the block
// VirtualAlloc made (compared by content and freed, its addresses in it and
// in the mode made offsets), the save data's words Kind reads, the loading
// world, the fades, the frame's +80 to +87 (the machines' Inits' frames
// below hold the machines' addresses) and r3. ---

std::vector<uint32_t> g_calls;
bool RecordStart(PPCContext& ctx, uint32_t address) {
  if (address != tr::kScreenStart && address != tr::kStringsLoad) {
    return false;
  }
  // The start reads r3 only (its r4 a leftover).
  g_calls.insert(g_calls.end(), {address, ctx.r3.u32,
                                 address == tr::kScreenStart ? 0u : ctx.r4.u32, ctx.r1.u32,
                                 uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

// The words of [at, at + bytes) that point into [lo, lo + span) made their
// offsets with a mark (the block VirtualAlloc made differs by side).
void NormalizePointers(uint8_t* base, uint32_t at, uint32_t bytes, uint32_t lo, uint32_t span) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, at + i);
    if (word >= lo && word < lo + span) {
      guest::StoreU32(base, at + i, 0xB10C0000u ^ (word - lo));
    }
  }
}

void Init(Context& h, const World& w) {
  namespace lw = loading_world;
  uint8_t* const base = h.base;
  const lw::MovieWorld& movies = *g_loading;
  Results results("ModeType9::Init (sub_8225D6D0)");
  for (int c = 0; c < 12; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(base, g.address, g.bytes));
      }
      const lw::SavedGlobals loading(base);
      lw::ShapeMovieWorld(base, movies, r);
      lw::BuildStringTable(base, movies);
      guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
      guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                        r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
      }
      WriteRandom(base, w.block, kBlockBytes, r);
      ShapeSave(base, w, r);
      WriteRandom(base, kFades, kFadesBytes, r);
      guest::StoreU32(base, kSaveTable, w.save);
      guest::StoreU32(base, w.dispatcher() + 200, r.Below(8));
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = w.mode();
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
      g_calls.clear();
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = RecordStart;
      if (v == 0) {
        const bool native = ITC_FLAG(native_todays_result);
        ITC_FLAG(native_todays_result) = false;
        __imp__sub_8225D6D0(t, base);
        ITC_FLAG(native_todays_result) = native;
      } else {
        hooks::ModeType9Init(t, base);
      }
      hooks::g_start_up_recorder = recorder;
      time_hooks::ClearFakeNow();
      out[v].Word(uint32_t(g_calls.size()));
      for (size_t k = 0; k < g_calls.size(); k++) {
        // The frames by their offset from r1.
        out[v].Word(k % 5 == 3 ? sp - g_calls[k] : g_calls[k]);
      }
      const uint32_t block = guest::LoadU32(base, w.mode() + tr::kBlock);
      const bool made = memory::IsPages(block);
      out[v].Word(made ? 1 : 0);
      if (made) {
        NormalizePointers(base, block, tr::kBlockBytes, block, tr::kBlockBytes);
        out[v].Bytes(Read(base, block, tr::kBlockBytes));
        NormalizePointers(base, w.mode(), 1024, block, tr::kBlockBytes);
        memory::FreePages(block);
        guest::StoreU32(base, w.mode() + tr::kBlock, 1);
      }
      SaveWords(base, w, out[v]);
      out[v].Bytes(Read(base, w.block, kBlockBytes));
      out[v].Bytes(Read(base, movies.movies.address, movies.movies.bytes));
      out[v].Bytes(Read(base, movies.texts.address, movies.texts.bytes));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingShown));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
      out[v].Bytes(Read(base, kFades, kFadesBytes));
      out[v].Bytes(Read(base, sp - 112 + 80, 8));
      out[v].Bytes(Read(base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      loading.Restore();
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void TodaysResult(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: todays_result: no block from the allocation layer");
    return;
  }
  const loading_world::MovieWorld loading;
  if (!loading.ok()) {
    ITC_LOG_ERROR("harness: todays_result: no loading world");
    return;
  }
  g_loading = &loading;
  if (hooks::NativeHeapActive()) {
    Mode(h, w);
    Init(h, w);
  }
  g_loading = nullptr;
}

}  // namespace harness
