// The type 8 mode's panel screen (src/type8_screens/,
// docs/functions/type8-screens.md) against the recompiled functions. The
// originals run with --native_type8_screens off, the plain C++ through the
// register steps (hooks/type8_screens.h) with the calls
// hooks::Type67ScreensCalls makes. Panel8C's SetStats and ShowArrows run in
// the car_list harness, for its movie and text world.
//
// A world block of random words holds the object (r3 at +0x100, now and
// then with its high half set), the dispatcher (+0x6000) and the stand-ins
// a case needs. The screen fades' counts, the dialog's player, the mode
// before and the movie table's missing word are saved, shaped by the case,
// compared and put back. UiMovies::Open and Delete are recorded (their
// adapters hand the original's calls to hooks::g_start_up_recorder; the
// plain C++'s open and remove record the same words), each call's id, asset,
// frame and return address compared; Open answers 1, Delete 0x5A5A.
// Compared exactly (no float math): the world, the globals, the recorded
// calls, the function's own frame (its back chain and the words above its
// outgoing area, its register saves left out), the caller's area, r1 and r3
// (both halves). The deleting destructors with bit 0 set run on blocks of
// operator new's, compared by r3 and whether the block is handed out
// again. The list screen's list part runs in a world of its own (the part,
// its inner part, both players' blocks with their menus, the players'
// action mappers and a fake sound manager, as type67_list_part.cpp makes
// them; the color list part's the same with its three-menu blocks; the menus' item blocks the allocation layer's pages where the
// function makes or frees them), with the movie world.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/app_start_up.h"
#include "hooks/type67_screens.h"
#include "hooks/type8_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "game_modes/game_modes.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "type67_screens/type67_screens.h"
#include "type8_screens/type8_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type8_screens);

namespace harness {

// The stand's car world (src/harnesses/car_makers.cpp): a spec and
// parameters for one car id in a 0x10000-byte block. (The same definition
// is in car_makers.cpp.)
struct StandWorld {
  uint32_t params, holder_word, list;
};
StandWorld LayShowCarWorld(uint8_t* base, uint32_t block, Random& r, int c, uint32_t holder);
// The list screen's pages, steps and slots (type8_list_screen.cpp).
void Type8ListScreen(Context& h);

namespace {

namespace lw = loading_world;
namespace t8 = type8_screens;
namespace ts = type67_screens;

constexpr uint32_t kWorldBytes = 0x8000;
constexpr uint32_t kObjectAt = 0x100;
constexpr uint32_t kDispatcherAt = 0x6000;
constexpr uint32_t kStackBytes = 0x400;   // below the call's r1
constexpr uint32_t kCallerBytes = 0x100;  // above it
constexpr uint32_t kOpen = 0x8211E808, kDelete = 0x8211E258;

const std::vector<std::pair<uint32_t, uint32_t>> kGlobals = {
    {0x826A50AC, 0x40},  // the screen fades' words (the counts among them)
    {ts::kFadeWait, 4},       {t8::kDialogPlayer, 4}, {t8::kModeBefore, 4},
    {0x826C5CDC, 4},  // UiMovies::Find's answer for a missing id
    {0x82815EAC, 4},  // the car list's two-player word (PanelSetPair's constructor)
};

uint64_t Seed(uint64_t z) {
  z += 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
}

// The UI movie world of the handlers' cases (none for the others), shaped
// for each case as the node screen's harness shapes it, and UiMovie::Run's
// update (sub_8211D2F8) and ApplyAnimation (sub_825A08E0) stood in for,
// recorded and answered from the case's generator.
const lw::MovieWorld* g_movies = nullptr;
struct Recorded {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<Recorded> g_recorded;
Random g_answers;
bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8 && function != 0x825A08E0) {
    return false;
  }
  g_recorded.push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = function == 0x8211D2F8 ? g_answers.Below(2) : g_answers.Next();
  return true;
}

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// UiMovies::Open and Delete recorded: (address, id, asset for Open, r1, lr).
std::vector<uint32_t> g_records;
bool Record(PPCContext& ctx, uint32_t address) {
  if (address == kOpen) {
    g_records.insert(g_records.end(),
                     {kOpen, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 1;
    return true;
  }
  if (address == kDelete) {
    g_records.insert(g_records.end(), {kDelete, ctx.r3.u32, ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 0x5A5A;
    return true;
  }
  return false;
}

// The adapters' calls with UiMovies::Open and Delete recorded.
t8::Calls RecordingCalls(PPCContext& t, uint8_t* base) {
  t8::Calls calls = hooks::Type67ScreensCalls(t, base);
  calls.open = [](uint32_t id, uint32_t asset, uint32_t sp, uint32_t lr) -> uint64_t {
    g_records.insert(g_records.end(), {kOpen, id, asset, sp, lr});
    return 1;
  };
  calls.remove = [](uint32_t id, uint32_t sp, uint32_t lr) -> uint64_t {
    g_records.insert(g_records.end(), {kDelete, id, sp, lr});
    return 0x5A5A;
  };
  return calls;
}

// A function's frame: its size and the register saves at its top (0 for a
// leaf or a tail call: the stack below compared whole, nothing written).
struct Frame {
  uint32_t size, saves;
  // Whether the stack below a frameless call is compared (not for a tail
  // call into functions with register saves, or a leaf saving registers
  // below its r1).
  bool below = true;
};

using Setup = std::function<void(PPCContext&, Random&, uint32_t world)>;
using Native = std::function<void(PPCContext&, uint8_t*)>;
// Run after the function, before the outputs are read.
using After = std::function<void(Outputs&)>;

void Group(Context& h, uint32_t world, const char* name, uint32_t address, int cases,
           Frame frame, const Setup& setup, GuestFunction recompiled, const Native& native,
           const After& after = {}) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", name, address));
  for (int c = 0; c < cases; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed((uint64_t(address) << 16) ^ uint64_t(c)));
      std::vector<std::vector<uint8_t>> saved;
      for (const auto& [at, bytes] : kGlobals) saved.push_back(Read(base, at, bytes));
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(base, world, kWorldBytes, r);
      for (const auto& [at, bytes] : kGlobals) WriteRandom(base, at, bytes, r);
      if (g_movies != nullptr) {
        lw::ShapeMovieWorld(base, *g_movies, r);
        for (uint32_t k = 0; k < lw::kLeaves; k++) {
          guest::StoreU32(base, g_movies->leaf(k) + 108, 1 + r.Below(7));
        }
        guest::StoreU32(base, 0x826C5CDC, g_movies->movie(0));
      }
      // The fade counts 0 mostly (Leave waits on both).
      for (const uint32_t at : {ts::kFadeCountA, ts::kFadeCountB}) {
        guest::StoreU32(base, at, r.Below(2) == 0 ? 0 : r.Below(4));
      }
      // The dispatcher's ring: its count and start small.
      guest::StoreU32(base, world + kDispatcherAt + 204 + 600, r.Below(50));
      guest::StoreU32(base, world + kDispatcherAt + 204 + 604, r.Below(60));
      t.r3.u64 = (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0) | (world + kObjectAt);
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      setup(t, r, world);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_records.clear();
      g_recorded.clear();
      g_answers = Random(r.Next());
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = StandIn;
      if (v == 0) {
        const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = Record;
        ITC_FLAG(native_type8_screens) = false;
        recompiled(t, base);
        ITC_FLAG(native_type8_screens) = true;
        hooks::g_start_up_recorder = recorder;
      } else {
        native(t, base);
      }
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      if (after) after(out[v]);
      out[v].Word(uint32_t(g_records.size()));
      for (const uint32_t word : g_records) out[v].Word(word);
      out[v].Word(uint32_t(g_recorded.size()));
      for (const Recorded& call : g_recorded) {
        for (const uint32_t word : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                    uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                    uint32_t(call.r4 >> 32)}) {
          out[v].Word(word);
        }
      }
      if (g_movies != nullptr) {
        out[v].Bytes(Read(base, g_movies->movies.address, g_movies->movies.bytes));
        out[v].Bytes(Read(base, g_movies->texts.address, g_movies->texts.bytes));
      }
      out[v].Bytes(Read(base, world, kWorldBytes));
      for (const auto& [at, bytes] : kGlobals) out[v].Bytes(Read(base, at, bytes));
      if (frame.size != 0) {
        const uint32_t bottom = sp - frame.size;
        out[v].Bytes(Read(base, bottom, 4));
        out[v].Bytes(Read(base, bottom + 80, frame.size - 80 - frame.saves));
      } else if (frame.below) {
        out[v].Bytes(Read(base, sp - kStackBytes, kStackBytes));
      }
      out[v].Bytes(Read(base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].first, saved[i].data(), kGlobals[i].second);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// A deleting destructor with bit 0 of r4 set: a block of operator new's
// (`bytes`, shaped by `shape`) destroyed and deleted, compared by r3 and
// whether operator new hands the block out again.
void Deleting(Context& h, const char* name, uint32_t address, uint32_t bytes,
              const std::function<void(uint32_t, Random&)>& shape, GuestFunction recompiled,
              GuestFunction native) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{}, deleting (sub_{:08X})", name, address));
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed(uint64_t(address) << 20 ^ uint64_t(c)));
      const auto object = memory::crt::OperatorNew(base, bytes);
      if (!object) {
        ITC_LOG_ERROR("harness: type8_screens: operator new failed");
        return;
      }
      WriteRandom(base, *object, bytes, r);
      shape(*object, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = *object;
      t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type8_screens) = false;
        recompiled(t, base);
        ITC_FLAG(native_type8_screens) = true;
      } else {
        native(t, base);
      }
      out[v].Word(t.r3.u32 == *object ? 1 : 0);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const auto probe = memory::crt::OperatorNew(base, bytes);
      out[v].Word(probe && *probe == *object ? 1 : 0);
      if (probe) memory::crt::OperatorDelete(*probe);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The player: 0 or 1, now and then any word.
uint64_t Player(Random& r) {
  return r.Below(4) == 0 ? r.Next() : r.Below(2);
}

void NoSetup(PPCContext&, Random&, uint32_t) {}

// The step with the recording calls (UiMovies::Open and Delete).
Native Recording(uint64_t (*step)(uint32_t, uint32_t, const t8::Calls&)) {
  return [step](PPCContext& t, uint8_t* base) {
    const t8::Calls calls = RecordingCalls(t, base);
    t.r3.u64 = step(t.r1.u32, uint32_t(t.lr), calls);
  };
}

// --- The panels, the group and the screen's leaves ---

void Panels(Context& h, uint32_t world) {
  const auto ctor = [](PPCContext& t, Random& r, uint32_t w) {
    t.r4.u64 = Player(r);
    t.r5.u64 = r.Below(2) == 0 ? r.Next() : w + 0x2000;
  };
  Group(
      h, world, "PanelGroup8::PanelGroup8", 0x82239C10, 6, {112, 24},
      [](PPCContext& t, Random& r, uint32_t) { t.r4.u64 = r.Below(2) == 0 ? 4 : r.Next(); },
      __imp__sub_82239C10, hooks::Type8PanelGroupConstruct);
  Group(h, world, "Panel8F::Panel8F", 0x82239C70, 6, {112, 32}, ctor, __imp__sub_82239C70,
        hooks::Type8Panel8FConstruct);
  Group(h, world, "Panel8E::Panel8E", 0x82239D58, 6, {112, 32}, ctor, __imp__sub_82239D58,
        hooks::Type8Panel8EConstruct);
  Group(h, world, "Panel8C::Panel8C", 0x82239E78, 6, {128, 48}, ctor, __imp__sub_82239E78,
        hooks::Type8Panel8CConstruct);
  Group(h, world, "Panel8D::Panel8D", 0x8223A100, 6, {112, 32}, ctor, __imp__sub_8223A100,
        hooks::Type8Panel8DConstruct);
  Group(h, world, "ConfirmPanel8::ConfirmPanel8", 0x8223A208, 6, {112, 32}, ctor,
        __imp__sub_8223A208, hooks::Type8ConfirmPanelConstruct);
  // The deleting destructors with bit 0 of r4 clear, then set.
  const auto keep = [](PPCContext& t, Random& r, uint32_t) { t.r4.u64 = r.Next() & ~1ull; };
  Group(h, world, "PanelGroup8::DeletingDestructor", 0x8223A2A8, 4, {112, 24}, keep,
        __imp__sub_8223A2A8, hooks::Type8PanelGroupDeletingDestruct);
  Group(h, world, "Panel8C::DeletingDestructor", 0x8223A348, 4, {112, 24}, keep,
        __imp__sub_8223A348, hooks::Type8Panel8CDeletingDestruct);
  const auto random = [](uint32_t, Random&) {};
  Deleting(h, "PanelGroup8::DeletingDestructor", 0x8223A2A8, 44, random, __imp__sub_8223A2A8,
           hooks::Type8PanelGroupDeletingDestruct);
  Deleting(h, "Panel8C::DeletingDestructor", 0x8223A348, 324, random, __imp__sub_8223A348,
           hooks::Type8Panel8CDeletingDestruct);
}

void Leaves(Context& h, uint32_t world) {
  uint8_t* const base = h.base;
  // The list screen's leaves (list_screen.cpp): its movie id +9680 16 to
  // 19; ItemInfo's r4 an item (0 to 7 mostly, else 8 to 11 or any word), r5
  // an info in the world.
  const auto list_id = [base](PPCContext&, Random& r, uint32_t w) {
    guest::StoreU32(base, w + kObjectAt + 9680, 16 + r.Below(4));
  };
  Group(h, world, "Type8ListScreen::IsLoaded", 0x8223F590, 4, {0, 0}, list_id,
        __imp__sub_8223F590, hooks::Type8ListIsLoaded);
  Group(
      h, world, "Type8ListScreen::OpenMovie", 0x8223F3E0, 4, {0, 0}, list_id,
      __imp__sub_8223F3E0, [](PPCContext& t, uint8_t* b) {
        const t8::Calls c = RecordingCalls(t, b);
        t.r3.u64 = t8::ListScreen8OpenMovie(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
      });
  Group(
      h, world, "Type8ListScreen::Release", 0x8223F588, 4, {0, 0}, list_id, __imp__sub_8223F588,
      [](PPCContext& t, uint8_t* b) {
        const t8::Calls c = RecordingCalls(t, b);
        t.r3.u64 = t8::ListScreen8Release(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
      });
  Group(
      h, world, "Type8ListScreen::ItemInfo", 0x8223F598, 12, {0, 0},
      [](PPCContext& t, Random& r, uint32_t w) {
        t.r4.u64 = r.Below(4) == 0 ? (r.Below(2) == 0 ? 8 + r.Below(4) : r.Next()) : r.Below(8);
        t.r5.u64 = w + 0x7000 + 4 * r.Below(48);
      },
      __imp__sub_8223F598, hooks::Type8ListItemInfo);
  Group(h, world, "Type8PanelScreen::InitWords", 0x82238420, 4, {0, 0}, NoSetup,
        __imp__sub_82238420, hooks::Type8InitWords);
  Group(h, world, "Type8PanelScreen::IsLoaded", 0x82238418, 2, {0, 0}, NoSetup,
        __imp__sub_82238418, hooks::Type8IsLoaded);
  Group(h, world, "Type8PanelScreen::OpenMovies", 0x82238440, 4, {0, 0}, NoSetup,
        __imp__sub_82238440, Recording(t8::OpenMovies));
  Group(h, world, "Type8PanelScreen::DeleteMovies", 0x82238450, 4, {0, 0}, NoSetup,
        __imp__sub_82238450, Recording(t8::DeleteMovies));
  for (const auto& [name, address, recompiled, native] :
       {std::tuple{"Type8PanelScreen::OnMessage2", 0x82238458u, __imp__sub_82238458,
                   hooks::Type8OnMessage2},
        std::tuple{"Type8PanelScreen::OnSaveLoaded", 0x82238530u, __imp__sub_82238530,
                   hooks::Type8OnSaveLoaded},
        std::tuple{"Type8PanelScreen::OnAnswer0", 0x82238540u, __imp__sub_82238540,
                   hooks::Type8OnAnswer0},
        std::tuple{"Type8PanelScreen::OnAnswer1", 0x82238550u, __imp__sub_82238550,
                   hooks::Type8OnAnswer1},
        std::tuple{"Type8PanelScreen::OnAnswer2", 0x82238560u, __imp__sub_82238560,
                   hooks::Type8OnAnswer2}}) {
    Group(h, world, name, address, 3, {0, 0}, NoSetup, recompiled, native);
  }
  Group(
      h, world, "Type8PanelScreen::Leave", 0x82238B10, 10, {0, 0},
      [base = h.base](PPCContext& t, Random& r, uint32_t w) {
        guest::StoreU32(base, w + kObjectAt + 15240, r.Below(2));
        t.r4.u64 = w + kDispatcherAt;
      },
      __imp__sub_82238B10, hooks::Type8Leave);
}

// --- The handlers (the screen's world and the UI movie world) ---

constexpr uint32_t kListAt = 0x4000, kListVtableAt = 0x4400, kVtableAt = 0x4800,
                   kHolderVtableAt = 0x4C00;
constexpr uint32_t kEmpty = 0x82106540, kSetR4 = 0x820CC470, kGetter = 0x82090780;

uint32_t Pick(Random& r, std::initializer_list<uint32_t> targets) {
  return *(targets.begin() + r.Below(uint32_t(targets.size())));
}

// The screen at the world's +0x100: its vtable, the car list's (+9644,
// +0x4000) and the holder's (+9648 and +15220) vtables from stand-ins (the
// empty handler; sub_820CC470, which stores r4 at r3 + 20; sub_82090780,
// which answers r3's +80) and, for the holder, now and then the one-group
// holder's own (GroupPair::WaitForFade and Run: UiMovie::Run's update stood
// in for); the group's movie id 16 to 19 (not in the movie table: movie
// 0 of the world), the nodes +11256 (0 to 19) and +11260 (0 to 53), the
// named animations +11264 and +11268 (0 to 255), the flags +9652, +15228 to
// +15240 and the bases' +1944 0 or 1, +15244 0, 1 or -1, the dialog's state
// +9656 0 to 3 (mostly 0), the mode before 0 to 2 and the fade wait 0 or 1;
// r4 the dispatcher.
void LayScreen(uint8_t* base, Random& r, uint32_t w, PPCContext& t) {
  const uint32_t s = w + kObjectAt;
  guest::StoreU32(base, s, w + kVtableAt);
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, w + kVtableAt + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
    guest::StoreU32(base, w + kListVtableAt + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
    guest::StoreU32(base, w + kHolderVtableAt + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, s + 9644, w + kListAt);
  guest::StoreU32(base, w + kListAt, w + kListVtableAt);
  guest::StoreU32(base, w + kListAt + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  const uint32_t holder_vtable = r.Below(3) == 0 ? ts::kSingleGroupVtable : w + kHolderVtableAt;
  guest::StoreU32(base, s + t8::kHolder, holder_vtable);
  guest::StoreU32(base, s + t8::kHolder + 4, s + t8::kGroup);
  guest::StoreU32(base, s + 9648, s + t8::kHolder);
  guest::StoreU32(base, s + t8::kGroup + 4, 16 + r.Below(4));
  guest::StoreU32(base, s + 11256, r.Below(20));
  guest::StoreU32(base, s + 11260, r.Below(54));
  guest::StoreU32(base, s + 11264, r.Below(256));
  guest::StoreU32(base, s + 11268, r.Below(256));
  for (const uint32_t at : {9652u, 15228u, 15232u, 15236u, 15240u, 13220u, 15192u}) {
    guest::StoreU32(base, s + at, r.Below(2));
  }
  guest::StoreU32(base, s + 15244, Pick(r, {0, 1, 0xFFFFFFFFu}));
  guest::StoreU32(base, s + 9656, r.Below(2) == 0 ? 0 : r.Below(4));
  guest::StoreU32(base, t8::kModeBefore, r.Below(3));
  guest::StoreU32(base, ts::kFadeWait, r.Below(2));
  t.r4.u64 = w + kDispatcherAt;
}

// The functions without adapters: their plain C++ with the adapters' calls.
void NativeShowPlayerNodes(PPCContext& t, uint8_t* b) {
  t.r3.u64 = t8::ShowPlayerNodes(b, t.r3.u64, t.r4.u64, t.r5.u64, t.r1.u32);
}
void NativeAskQuit(PPCContext& t, uint8_t* b) {
  const t8::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = t8::AskQuit(b, t.r3.u64, t.r4.u64, t.r5.u32, t.r1.u32, c);
}
void NativeDrawList(PPCContext& t, uint8_t* b) {
  const t8::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = t8::DrawList(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
}

void Handlers(Context& h, uint32_t world) {
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(h.base, ui_movies::kMovieTable, id) != 0x826C5CDC) {
      ITC_LOG_ERROR("harness: type8_screens: movie {} is in the table; the handlers skipped",
                    id);
      return;
    }
  }
  const lw::MovieWorld w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type8_screens: no movie world");
    return;
  }
  const lw::SavedGlobals saved(h.base);
  lw::BuildStringTable(h.base, w);
  g_movies = &w;
  uint8_t* const base = h.base;
  const auto screen = [base](PPCContext& t, Random& r, uint32_t w) { LayScreen(base, r, w, t); };
  Group(
      h, world, "Type8PanelScreen::ShowPlayerNodes", 0x82238570, 8, {128, 40},
      [base](PPCContext& t, Random& r, uint32_t w) {
        LayScreen(base, r, w, t);
        t.r4.u64 = r.Below(4) == 0 ? (r.Next() & 0xFFFFFFFF00000000ull) : r.Below(2);
        t.r5.u64 = r.Below(2);
      },
      __imp__sub_82238570, NativeShowPlayerNodes);
  Group(h, world, "Type8PanelScreen::FadeInGroup", 0x82238468, 10, {112, 24}, screen,
        __imp__sub_82238468, hooks::Type8FadeInGroup);
  Group(h, world, "Type8PanelScreen::UpdateOpening", 0x82238A00, 12, {112, 32}, screen,
        __imp__sub_82238A00, hooks::Type8UpdateOpening);
  Group(h, world, "Type8PanelScreen::Show", 0x82238928, 8, {96, 16}, screen,
        __imp__sub_82238928, hooks::Type8Show);
  Group(
      h, world, "Type8PanelScreen::AskQuit", 0x82238BA0, 6, {128, 40},
      [base](PPCContext& t, Random& r, uint32_t w) {
        LayScreen(base, r, w, t);
        t.r5.u64 = r.Below(2);
      },
      __imp__sub_82238BA0, NativeAskQuit);
  Group(h, world, "Type8PanelScreen::DrawList", 0x82238DC8, 10, {112, 24}, screen,
        __imp__sub_82238DC8, NativeDrawList);
  Group(h, world, "Type8PanelScreen::Draw", 0x82238E78, 10, {112, 24}, screen,
        __imp__sub_82238E78, hooks::Type8Draw);
  Group(h, world, "Type8PanelScreen::OnDialogClosed", 0x82238990, 8, {96, 16}, screen,
        __imp__sub_82238990, hooks::Type8OnDialogClosed);
  g_movies = nullptr;
  saved.Restore();
}

// --- The constructor, the destructors, the machine and the pick ---

// The car list's menus MenuList::Release frees (each base's +60 and +132):
// their item blocks (+68) 0 or pages of the allocation layer's, compared by
// whether they're still pages after (and freed).
std::vector<uint32_t> g_made;
void Menus(uint8_t* base, uint32_t screen, Random& r) {
  for (const uint32_t b : {screen + 11276, screen + 11276 + 1972}) {
    for (const uint32_t list : {b + 60, b + 132}) {
      uint32_t block = 0;
      if (r.Below(2) == 0) {
        block = memory::AllocPages(64, 4096);
        g_made.push_back(block);
      }
      guest::StoreU32(base, list + 68, block);
    }
  }
}
void Released(Outputs& out) {
  for (const uint32_t block : g_made) {
    const bool still = memory::IsPages(block);
    out.Word(still ? 1 : 0);
    if (still) memory::FreePages(block);
  }
  g_made.clear();
}

void NativeInitMachine(PPCContext& t, uint8_t* b) { hooks::Type8InitMachine(t, b); }

void Screen(Context& h, uint32_t world) {
  uint8_t* const base = h.base;
  Group(h, world, "Type8PanelScreen::Type8PanelScreen", 0x822386F0, 6, {224, 112}, NoSetup,
        __imp__sub_822386F0, hooks::Type8Construct);
  // The list screen's constructors (list_screen.cpp) on a random screen.
  Group(h, world, "Type8ListBase::Type8ListBase", 0x822574D0, 4, {96, 16}, NoSetup,
        __imp__sub_822574D0, hooks::Type8ListBaseConstruct);
  Group(h, world, "Type8ListScreen::Type8ListScreen", 0x8223F140, 4, {112, 32}, NoSetup,
        __imp__sub_8223F140, hooks::Type8ListScreenConstruct);
  const auto menus = [base](PPCContext& t, Random& r, uint32_t w) {
    Menus(base, w + kObjectAt, r);
    t.r4.u64 = r.Next() & ~1ull;
  };
  Group(h, world, "Type8PanelScreen::~Type8PanelScreen", 0x82238368, 6, {112, 24}, menus,
        __imp__sub_82238368, hooks::Type8Destruct, Released);
  Group(h, world, "Type8PanelScreen::DeletingDestructor", 0x822388D8, 4, {112, 24}, menus,
        __imp__sub_822388D8, hooks::Type8DeletingDestruct, Released);
  Deleting(
      h, "Type8PanelScreen::DeletingDestructor", 0x822388D8, t8::kScreenBytes,
      [base](uint32_t object, Random&) {
        for (const uint32_t b : {object + 11276, object + 11276 + 1972}) {
          guest::StoreU32(base, b + 60 + 68, 0);
          guest::StoreU32(base, b + 132 + 68, 0);
        }
      },
      __imp__sub_822388D8, hooks::Type8DeletingDestruct);
  // The machine (+4) empty, or holding transitions whose keys are small
  // numbers (states below 12, messages below 36, so some match the
  // Init's: Add replaces those).
  Group(
      h, world, "Type8PanelScreen::InitMachine", 0x82238E98, 12, {1712, 80},
      [base](PPCContext&, Random& r, uint32_t w) {
        namespace gm = game_modes;
        const uint32_t machine = w + kObjectAt + 4;
        if (r.Below(2) == 0) {
          gm::StateMachineConstruct(base, machine);
          guest::StoreU32(base, machine + gm::kTransitionCount, 0);
          return;
        }
        guest::StoreU32(base, machine + gm::kTransitionCount, 8 + r.Below(160));
        for (uint32_t i = 0; i < gm::kTransitions; i++) {
          const uint32_t at = machine + gm::kTransitionBytes * i;
          guest::StoreU32(base, at, r.Below(12));
          guest::StoreU32(base, at + 4, r.Below(36));
          guest::StoreU32(base, at + 8, r.Below(8) == 0 ? 1 : 0);
          guest::StoreU32(base, at + 12, 0);
        }
      },
      __imp__sub_82238E98, NativeInitMachine);
}

// PickCar: both car list bases' words (+232 the tab 0 to 3, +236 to +248
// small steps, +1820 an entry table at the world's +0x5000 naming one entry
// whose car id (+8432) is LayShowCarWorld's: the case's spec and
// parameters, [0x826C2E48] pointed at them; rand's seed set per case for
// Car::SetModel's picks), the two-player save block ([0x826C5B5C]) a
// harness block whose halves' garage lists are walked the steps to a slot
// of random words, the garage cars random (+4 0xFFFF now and then).
// GarageCar::Reset runs through its adapter on both sides. Compared: the
// world, the save block's garages, the garage cars, the rand seed, the
// frame (but the register saves) and r3; the globals put back after.
void PickCar(Context& h, uint32_t world) {
  uint8_t* const base = h.base;
  constexpr uint32_t kBlockBytes = 0x3AB80, kParams = 0x826C2E48, kRandSeed = 0x82638188;
  constexpr uint32_t kHalf = 120108, kCarsBytes = 2 * 1972;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  const uint32_t stand = memory::Alloc(0x10000, 0);
  const uint32_t entry = memory::Alloc(0x2100, 0);
  if (block == 0 || stand == 0 || entry == 0) {
    ITC_LOG_ERROR("harness: type8_screens: no blocks for PickCar");
    for (const uint32_t b : {block, stand, entry}) {
      if (b != 0) memory::Free(b);
    }
    return;
  }
  const uint32_t saved_block = guest::LoadU32(base, car_list::kPairSaveData);
  const uint32_t saved_params = guest::LoadU32(base, kParams);
  const uint32_t saved_seed = guest::LoadU32(base, kRandSeed);
  const std::vector<uint8_t> saved_cars = Read(base, t8::kGarageCars, kCarsBytes);
  Group(
      h, world, "Type8PanelScreen::PickCar", 0x82238C88, 16, {160, 72},
      [base, block, stand, entry](PPCContext& t, Random& r, uint32_t w) {
        const StandWorld stand_world = LayShowCarWorld(base, stand, r, int(r.Below(64)), 0);
        const uint32_t array = guest::LoadU32(base, stand_world.list + 1136);
        const uint32_t at = guest::LoadU32(base, stand_world.list + 48) +
                            guest::LoadU32(base, stand_world.list + 52);
        const uint32_t car_id =
            guest::LoadU32(base, guest::LoadU32(base, array + 4 * at) + 8432);
        guest::StoreU32(base, 0x826C2E48, stand_world.params);
        WriteRandom(base, entry + 8400, 64, r);
        guest::StoreU32(base, entry + 8432, car_id);
        guest::StoreU32(base, car_list::kPairSaveData, block);
        const uint32_t s = w + kObjectAt;
        for (uint32_t player = 0; player < 2; player++) {
          const uint32_t words = s + 11512 + 1972 * player;
          guest::StoreU32(base, words - 4, r.Below(4));
          const uint32_t a = r.Below(3), b = r.Below(3);
          guest::StoreU32(base, words + 0, a);
          guest::StoreU32(base, words + 4, b);
          guest::StoreU32(base, words + 8, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(3));
          guest::StoreU32(base, words + 12, r.Below(3));
          guest::StoreU32(base, words + 1584, w + 0x5000 + 0x40 * player);
          guest::StoreU32(base, w + 0x5000 + 0x40 * player + 4 * (a + b), entry);
          // The half's garage list: from +204, a + b steps through distinct
          // nodes, to a slot of random words.
          const uint32_t nodes = block + kHalf * player + 6120;
          WriteRandom(base, nodes, 0x400, r);
          uint16_t node = uint16_t(r.Below(10));
          guest::StoreU16(base, nodes + 204, node);
          for (uint32_t i = 0; i < a + b; i++) {
            const uint16_t next = uint16_t(10 * (i + 1) + r.Below(10));
            guest::StoreU16(base, nodes + 4 * node, next);
            node = next;
          }
          const uint32_t slot = r.Below(3);
          guest::StoreU16(base, nodes + 4 * node + 2, uint16_t(slot));
          WriteRandom(base, nodes + 212 + 1968 * slot, 1968, r);
          const uint32_t car = t8::kGarageCars + 1972 * player;
          WriteRandom(base, car, 1972, r);
          if (r.Below(2) == 0) guest::StoreU16(base, car + 4, 0xFFFF);
        }
        guest::StoreU32(base, kRandSeed, uint32_t(r.Next()));
        t.r4.u64 = w + kDispatcherAt;
      },
      __imp__sub_82238C88, hooks::Type8PickCar,
      [base, block](Outputs& out) {
        for (uint32_t player = 0; player < 2; player++) {
          out.Bytes(Read(base, block + kHalf * player + 6120, 0x400 + 3 * 1968));
        }
        out.Bytes(Read(base, t8::kGarageCars, kCarsBytes));
        out.Word(guest::LoadU32(base, kRandSeed));
      });
  guest::StoreU32(base, car_list::kPairSaveData, saved_block);
  guest::StoreU32(base, kParams, saved_params);
  guest::StoreU32(base, kRandSeed, saved_seed);
  Write(base, t8::kGarageCars, saved_cars.data(), kCarsBytes);
  memory::Free(block);
  memory::Free(stand);
  memory::Free(entry);
}

// --- The list screen's list part ---

// The world: the part at +0x100 (its inner at +0x114, the blocks at +0x13C
// and +0x224), the menus' item blocks (+0x1000), the players' action
// mappers (+0x1400 their vtables, +0x1800 the mappers), a fake sound manager
// (+0x2400, as the list part's harness in type67_list_part.cpp makes them).
constexpr uint32_t kPartAt = kObjectAt, kInnerAt = kPartAt + 20;
constexpr uint32_t kItemsAt = 0x1000, kMapperVtablesAt = 0x1400, kMappersAt = 0x1800;
constexpr uint32_t kManagerAt = 0x2400, kManagerVtableAt = 0x2480;
constexpr uint32_t kNone = 0xFFFFFFFFu;

// The players' mappers, the controllers and the sound manager: saved, laid
// by the cases, compared and put back.
const std::vector<std::pair<uint32_t, uint32_t>> kMenuGlobals = {
    {race_screens::kMapperEntries, 32},
    {race_screens::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
};

// An action id: mostly the menus' (151 to 180), sometimes any word.
uint32_t AnyAction(Random& r) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30);
}

// The menus' item blocks: ours, or 0 or pages of the allocation layer's
// (the functions that make or free them), recorded in g_items.
struct ItemBlock {
  uint32_t list, old;
};
std::vector<ItemBlock> g_items;
bool g_pages = false;

void LayMenu(uint8_t* base, uint32_t list, uint32_t items, Random& r) {
  namespace rs = race_screens;
  guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
  guest::StoreU32(base, list + rs::kListCount, r.Below(4) == 0 ? 1 + r.Below(3) : 2);
  guest::StoreU32(base, list + rs::kListShown, r.Below(4) == 0 ? r.Below(4) : 2);
  guest::StoreU32(base, list + rs::kListScroll, r.Below(4) == 0 ? kNone : r.Below(2));
  guest::StoreU32(base, list + rs::kListSelection, r.Below(6) == 0 ? r.Below(3) : r.Below(2));
  guest::StoreU32(base, list + rs::kListSelectionBefore,
                  r.Below(6) == 0 ? r.Below(3) : r.Below(2));
  for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
    guest::StoreU32(base, list + field, AnyAction(r));
  }
  guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
  guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  uint32_t block = items;
  if (g_pages) {
    block = 0;
    if (r.Below(4) != 0) {
      block = memory::AllocPages(16, 4096);
      WriteRandom(base, block, 16, r);
    }
  }
  guest::StoreU32(base, list + rs::kListBlock, block);
  g_items.push_back({list, block});
}

// A player's block: the player, the ready and backed-out words, the two
// menus, its node and string ids small (sub-scene -1 now and then) and its
// named animations 0 to 255, as the movie world has them.
void LayPlayerBlock(uint8_t* base, uint32_t block, uint32_t items, Random& r) {
  guest::StoreU32(base, block + 0, r.Below(2));
  guest::StoreU32(base, block + 4, r.Below(5) == 0 ? 2 : r.Below(2));
  guest::StoreU32(base, block + 8, r.Below(2));
  LayMenu(base, block + 12, items, r);
  LayMenu(base, block + 84, items + 0x40, r);
  guest::StoreU32(base, block + 156, r.Below(3) == 0 ? kNone : r.Below(16));
  for (uint32_t at = 160; at <= 176; at += 4) guest::StoreU32(base, block + at, r.Below(16));
  for (uint32_t at = 180; at <= 220; at += 4) guest::StoreU32(base, block + at, r.Below(256));
  guest::StoreU32(base, block + 224, r.Below(16));
  guest::StoreU32(base, block + 228, r.Below(16));
}

// A color block: the player, the ready and backed-out words, the three
// menus (the +156 one of five items mostly), its node and string ids small
// (+228 a sub-scene and a child of the live scene, never -1) and its named
// animations 0 to 255.
void LayColorBlock(uint8_t* base, uint32_t block, uint32_t items, Random& r) {
  guest::StoreU32(base, block + 0, r.Below(2));
  guest::StoreU32(base, block + 4, r.Below(5) == 0 ? 2 : r.Below(2));
  guest::StoreU32(base, block + 8, r.Below(2));
  LayMenu(base, block + 12, items, r);
  guest::StoreU32(base, block + 12 + race_screens::kListSelection, r.Below(4));
  guest::StoreU32(base, block + 12 + race_screens::kListSelectionBefore, r.Below(4));
  LayMenu(base, block + 84, items + 0x40, r);
  LayMenu(base, block + 156, items + 0x80, r);
  guest::StoreU32(base, block + 156 + race_screens::kListSelection, r.Below(5));
  for (uint32_t at = 228; at <= 276; at += 4) guest::StoreU32(base, block + at, r.Below(16));
  for (uint32_t at = 280; at <= 348; at += 4) guest::StoreU32(base, block + at, r.Below(256));
  guest::StoreU32(base, block + 356, r.Below(16));
  guest::StoreU32(base, block + 360, r.Below(16));
}

// The list part's world (or the color list part's): the part, its inner
// part (the page -1 or 0 to 2, the step -1, 0 or 1, the queued step, the
// first-call word) and both blocks; the mappers, the controllers and the
// sound manager.
void LayListPart(uint8_t* base, uint32_t w, Random& r, bool color) {
  g_items.clear();
  const uint32_t part = w + kPartAt, inner = w + kInnerAt;
  guest::StoreU32(base, part, color ? t8::kColorPartVtable : t8::kListPartVtable);
  guest::StoreU32(base, part + 12, 16 + r.Below(4));
  guest::StoreU32(base, part + 16, inner);
  guest::StoreU32(base, inner, color ? t8::kColorInnerVtable : t8::kListInnerVtable);
  auto small = [&r](uint32_t n) { return r.Below(4) == 0 ? kNone : r.Below(n); };
  guest::StoreU32(base, inner + 4, small(3));
  guest::StoreU32(base, inner + 8, small(3));
  guest::StoreU32(base, inner + 12, small(3));
  guest::StoreU32(base, inner + 16, small(2));
  guest::StoreU32(base, inner + 20, small(2));
  guest::StoreU32(base, inner + 24, small(2));
  guest::StoreU32(base, inner + 28, r.Below(2));
  if (color) {
    LayColorBlock(base, inner + 40, w + kItemsAt, r);
    LayColorBlock(base, inner + 404, w + kItemsAt + 0x100, r);
  } else {
    LayPlayerBlock(base, inner + 40, w + kItemsAt, r);
    LayPlayerBlock(base, inner + 272, w + kItemsAt + 0x80, r);
  }
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t vtable = w + kMapperVtablesAt + 0x40 * c;
    const uint32_t mapper = w + kMappersAt + 0x200 * c;
    guest::StoreU32(base, race_screens::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, race_screens::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, race_screens::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, race_screens::kPressedSlot);
    guest::StoreU32(base, vtable + 48, race_screens::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  const uint32_t manager = w + kManagerAt, vtable = w + kManagerVtableAt;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, vtable + 52, 0x820CE9F8);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// After a case: the menu globals, and with pages the item blocks: a new
// one's words compared and freed (its word in the list 1), the old one
// whether still the layer's.
void ListPartOutputs(uint8_t* base, Outputs& out) {
  for (const auto& [at, bytes] : kMenuGlobals) out.Bytes(Read(base, at, bytes));
  if (!g_pages) return;
  for (const ItemBlock& item : g_items) {
    const uint32_t now = guest::LoadU32(base, item.list + race_screens::kListBlock);
    out.Word(now == item.old ? 1 : 0);
    out.Word(now == 0 ? 0 : 1);
    if (now != 0 && now != item.old && memory::IsPages(now)) {
      out.Bytes(Read(base, now, 4 * guest::LoadU32(base, item.list + race_screens::kListCount)));
      memory::FreePages(now);
      guest::StoreU32(base, item.list + race_screens::kListBlock, 1);
    }
  }
  for (const ItemBlock& item : g_items) {
    if (item.old == 0) continue;
    const bool still = memory::IsPages(item.old);
    out.Word(still ? 1 : 0);
    if (still) memory::FreePages(item.old);
  }
  g_items.clear();
}

// What r3 is: the part, the inner part, or a player's block (either).
enum class Object { kPart, kInner, kBlock };

void ListPart(Context& h, uint32_t world) {
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(h.base, ui_movies::kMovieTable, id) != 0x826C5CDC) {
      ITC_LOG_ERROR("harness: type8_screens: movie {} is in the table; the list part skipped",
                    id);
      return;
    }
  }
  const lw::MovieWorld w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type8_screens: no movie world");
    return;
  }
  uint8_t* const base = h.base;
  const lw::SavedGlobals saved(base);
  std::vector<std::vector<uint8_t>> menu_globals;
  for (const auto& [at, bytes] : kMenuGlobals) menu_globals.push_back(Read(base, at, bytes));
  lw::BuildStringTable(base, w);
  g_movies = &w;
  // r3 the object (its high half as the Group set it), r4 the movie id (or
  // the player for the block's constructor, flags with bit 0 clear for the
  // deleting destructors), r5 the setting for PlaceCursor.
  enum class R4 { kMovie, kPlayer, kNoDelete };
  const auto setup = [base](Object object, R4 r4, bool color) {
    return [base, object, r4, color](PPCContext& t, Random& r, uint32_t wb) {
      LayListPart(base, wb, r, color);
      uint32_t at = wb + kPartAt;
      if (object == Object::kInner) at = wb + kInnerAt;
      if (object == Object::kBlock) {
        at = wb + kInnerAt + (r.Below(2) == 0 ? 40 : (color ? 404 : 272));
      }
      t.r3.u64 = (t.r3.u64 & 0xFFFFFFFF00000000ull) | at;
      switch (r4) {
        case R4::kMovie:
          t.r4.u64 = 16 + r.Below(4);
          break;
        case R4::kPlayer:
          t.r4.u64 = r.Below(2);
          break;
        case R4::kNoDelete:
          t.r4.u64 = r.Next() & ~1ull;
          break;
      }
      t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
    };
  };
  const auto after = [base](Outputs& out) { ListPartOutputs(base, out); };
  struct Function {
    const char* name;
    uint32_t address;
    int cases;
    Frame frame;
    Object object;
    R4 r4;
    bool pages;
    GuestFunction recompiled;
    void (*native)(PPCContext&, uint8_t*);
    bool color = false;
  };
  const Object part = Object::kPart, inner = Object::kInner, block = Object::kBlock;
  const R4 movie = R4::kMovie;
  const Function functions[] = {
      {"Type8PlayerBlock::Type8PlayerBlock", 0x822542B8, 4, {0, 0, false}, block, R4::kPlayer,
       false, __imp__sub_822542B8, hooks::Type8BlockConstruct},
      {"Type8PlayerBlock::SetUp", 0x822545D0, 8, {160, 48}, block, movie, true,
       __imp__sub_822545D0, hooks::Type8BlockSetUp},
      {"Type8PlayerBlock::PlayOpening", 0x822546F8, 4, {112, 24}, block, movie, false,
       __imp__sub_822546F8, hooks::Type8BlockPlayOpening},
      {"Type8PlayerBlock::ShowMenuMove", 0x82254798, 10, {144, 48}, block, movie, false,
       __imp__sub_82254798, hooks::Type8BlockShowMenuMove},
      {"Type8PlayerBlock::PlayReady", 0x822548B8, 4, {112, 24}, block, movie, false,
       __imp__sub_822548B8, hooks::Type8BlockPlayReady},
      {"Type8PlayerBlock::PlaceCursor", 0x82254948, 6, {128, 32}, block, movie, false,
       __imp__sub_82254948, hooks::Type8BlockPlaceCursor},
      {"Type8PlayerBlock::ShowSelections", 0x82254AD8, 6, {128, 40}, block, movie, false,
       __imp__sub_82254AD8, hooks::Type8BlockShowSelections},
      {"Type8PlayerBlock::WhileReady", 0x82254A70, 8, {112, 24}, block, movie, false,
       __imp__sub_82254A70, hooks::Type8BlockWhileReady},
      {"Type8PlayerBlock::Input", 0x82254EC8, 16, {128, 40}, block, movie, false,
       __imp__sub_82254EC8, hooks::Type8BlockInput},
      {"Type8PlayerBlock::UpdateSetting", 0x82254DB8, 8, {112, 24}, block, movie, false,
       __imp__sub_82254DB8, hooks::Type8BlockUpdateSetting},
      {"Type8PlayerBlock::SettingMoved", 0x82254C50, 6, {144, 48}, block, movie, false,
       __imp__sub_82254C50, hooks::Type8BlockSettingMoved},
      {"Type8ListPartInner::Type8ListPartInner", 0x822543C0, 4, {96, 16}, inner, movie, false,
       __imp__sub_822543C0, hooks::Type8ListInnerConstruct},
      {"Type8ListPart::Type8ListPart", 0x82254488, 4, {112, 24}, part, movie, false,
       __imp__sub_82254488, hooks::Type8ListPartConstruct},
      {"Type8ListPartInner::~Type8ListPartInner", 0x82254418, 6, {112, 24}, inner, movie, true,
       __imp__sub_82254418, hooks::Type8ListInnerDestruct},
      {"Type8ListPartInner::DeletingDestructor", 0x82254530, 4, {112, 24}, inner, R4::kNoDelete,
       true, __imp__sub_82254530, hooks::Type8ListInnerDeletingDestruct},
      {"Type8ListPart::~Type8ListPart", 0x822544E8, 6, {96, 16}, part, movie, true,
       __imp__sub_822544E8, hooks::Type8ListPartDestruct},
      {"Type8ListPart::DeletingDestructor", 0x82254580, 4, {112, 24}, part, R4::kNoDelete, true,
       __imp__sub_82254580, hooks::Type8ListPartDeletingDestruct},
      {"Type8ListPartInner::Open", 0x822549D8, 8, {128, 32}, inner, movie, true,
       __imp__sub_822549D8, hooks::Type8ListInnerOpen},
      {"Type8ListPartInner::Step0", 0x82254D08, 8, {128, 32}, inner, movie, false,
       __imp__sub_82254D08, hooks::Type8ListInnerStep0},
      {"Type8PartInner::Step1", 0x822538A8, 6, {112, 24}, inner, movie, false,
       __imp__sub_822538A8, hooks::Type8PartInnerStep1},
      {"Type8PartInner::Page1", 0x82253108, 4, {0, 0}, inner, movie, false, __imp__sub_82253108,
       hooks::Type8PartInnerPage1},
      {"Type8PartInner::Page2", 0x82253838, 6, {112, 16}, inner, movie, false,
       __imp__sub_82253838, hooks::Type8PartInnerPage2},
      {"Type8ListPartInner::Page0", 0x82254FB8, 16, {128, 40}, inner, movie, false,
       __imp__sub_82254FB8, hooks::Type8ListInnerPage0},
      {"Type8ListPartInner::Pick", 0x822550C0, 10, {0, 0, false}, inner, movie, false,
       __imp__sub_822550C0, hooks::Type8ListInnerPick},
      {"Type8ListPartInner::Input", 0x82254E18, 12, {128, 32}, inner, movie, false,
       __imp__sub_82254E18, hooks::Type8ListInnerInput},
      // The color list part.
      {"Type8ColorBlock::Type8ColorBlock", 0x82252EA0, 4, {0, 0, false}, block, R4::kPlayer,
       false, __imp__sub_82252EA0, hooks::Type8ColorBlockConstruct, true},
      {"Type8ColorBlock::SetUp", 0x82253308, 8, {176, 72}, block, movie, true,
       __imp__sub_82253308, hooks::Type8ColorBlockSetUp, true},
      {"Type8ColorBlock::PlaceCursorA", 0x82253728, 6, {112, 24}, block, movie, false,
       __imp__sub_82253728, hooks::Type8ColorBlockPlaceCursorA, true},
      {"Type8ColorBlock::PlaceCursorB", 0x82253690, 4, {144, 48}, block, movie, false,
       __imp__sub_82253690, hooks::Type8ColorBlockPlaceCursorB, true},
      {"Type8ColorBlock::PlaceCursorC", 0x82253600, 6, {128, 32}, block, movie, false,
       __imp__sub_82253600, hooks::Type8ColorBlockPlaceCursorC, true},
      {"Type8ColorBlock::PlayOpening", 0x822539A8, 4, {128, 32}, block, movie, false,
       __imp__sub_822539A8, hooks::Type8ColorBlockPlayOpening, true},
      {"Type8ColorBlock::ShowSelections", 0x82253A50, 6, {128, 40}, block, movie, false,
       __imp__sub_82253A50, hooks::Type8ColorBlockShowSelections, true},
      {"Type8ColorBlock::ShowMenuMove", 0x82253BF8, 12, {144, 56}, block, movie, false,
       __imp__sub_82253BF8, hooks::Type8ColorBlockShowMenuMove, true},
      {"Type8ColorBlock::PlayReady", 0x82253570, 4, {112, 24}, block, movie, false,
       __imp__sub_82253570, hooks::Type8ColorBlockPlayReady, true},
      {"Type8ColorBlock::SecondMoved", 0x822534F0, 6, {128, 32}, block, movie, false,
       __imp__sub_822534F0, hooks::Type8ColorBlockSecondMoved, true},
      {"Type8ColorBlock::UpdateSecond", 0x82253948, 8, {112, 24}, block, movie, false,
       __imp__sub_82253948, hooks::Type8ColorBlockUpdateSecond, true},
      {"Type8ColorBlock::ThirdMoved", 0x82253D90, 6, {144, 48}, block, movie, false,
       __imp__sub_82253D90, hooks::Type8ColorBlockThirdMoved, true},
      {"Type8ColorBlock::UpdateThird", 0x82253F60, 8, {112, 24}, block, movie, false,
       __imp__sub_82253F60, hooks::Type8ColorBlockUpdateThird, true},
      {"Type8ColorBlock::WhileReady", 0x82253EF8, 8, {112, 24}, block, movie, false,
       __imp__sub_82253EF8, hooks::Type8ColorBlockWhileReady, true},
      {"Type8ColorBlock::Input", 0x82254070, 16, {128, 40}, block, movie, false,
       __imp__sub_82254070, hooks::Type8ColorBlockInput, true},
      {"Type8ColorPartInner::Type8ColorPartInner", 0x82253030, 4, {96, 16}, inner, movie, false,
       __imp__sub_82253030, hooks::Type8ColorInnerConstruct, true},
      {"Type8ColorListPart::Type8ColorListPart", 0x82253190, 4, {112, 24}, part, movie, false,
       __imp__sub_82253190, hooks::Type8ColorPartConstruct, true},
      {"Type8ColorPartInner::~Type8ColorPartInner", 0x82253088, 6, {112, 24}, inner, movie,
       true, __imp__sub_82253088, hooks::Type8ColorInnerDestruct, true},
      {"Type8ColorPartInner::DeletingDestructor", 0x82253268, 4, {112, 24}, inner,
       R4::kNoDelete, true, __imp__sub_82253268, hooks::Type8ColorInnerDeletingDestruct, true},
      {"Type8ColorListPart::~Type8ColorListPart", 0x822531F0, 6, {96, 16}, part, movie, true,
       __imp__sub_822531F0, hooks::Type8ColorPartDestruct, true},
      {"Type8ColorListPart::DeletingDestructor", 0x822532B8, 4, {112, 24}, part, R4::kNoDelete,
       true, __imp__sub_822532B8, hooks::Type8ColorPartDeletingDestruct, true},
      {"Type8ColorPartInner::IsFirstChoice", 0x82253148, 4, {96, 8}, inner, R4::kPlayer, false,
       __imp__sub_82253148, hooks::Type8ColorInnerIsFirstChoice, true},
      {"Type8ColorListPart::IsFirstChoice", 0x82253238, 4, {96, 8}, part, R4::kPlayer, false,
       __imp__sub_82253238, hooks::Type8ColorPartIsFirstChoice, true},
      {"Type8ColorListPart::ThirdChoice", 0x82253240, 4, {0, 0}, part, R4::kPlayer, false,
       __imp__sub_82253240, hooks::Type8ColorPartThirdChoice, true},
      {"Type8ColorPartInner::Open", 0x822537A0, 8, {128, 32}, inner, movie, true,
       __imp__sub_822537A0, hooks::Type8ColorInnerOpen, true},
      {"Type8ColorPartInner::Step0", 0x82253E48, 8, {128, 32}, inner, movie, false,
       __imp__sub_82253E48, hooks::Type8ColorInnerStep0, true},
      {"Type8ColorPartInner::Page0", 0x82254178, 16, {128, 40}, inner, movie, false,
       __imp__sub_82254178, hooks::Type8ColorInnerPage0, true},
      {"Type8ColorPartInner::Pick", 0x82254280, 10, {0, 0, false}, inner, movie, false,
       __imp__sub_82254280, hooks::Type8ColorInnerPick, true},
      {"Type8ColorPartInner::Input", 0x82253FC0, 12, {128, 32}, inner, movie, false,
       __imp__sub_82253FC0, hooks::Type8ColorInnerInput, true},
  };
  for (const Function& f : functions) {
    g_pages = f.pages;
    Group(h, world, f.name, f.address, f.cases, f.frame, setup(f.object, f.r4, f.color),
          f.recompiled, f.native, after);
  }
  g_pages = false;
  g_movies = nullptr;
  // The deleting destructors with bit 0 set (the menus' item blocks 0).
  const auto shape = [base](bool whole, bool color = false) {
    return [base, whole, color](uint32_t object, Random&) {
      const uint32_t in = whole ? object + 20 : object;
      if (whole) {
        guest::StoreU32(base, object, color ? t8::kColorPartVtable : t8::kListPartVtable);
      }
      guest::StoreU32(base, in, color ? t8::kColorInnerVtable : t8::kListInnerVtable);
      for (const uint32_t b : {in + 40, in + (color ? 404 : 272)}) {
        guest::StoreU32(base, b + 12 + race_screens::kListBlock, 0);
        guest::StoreU32(base, b + 84 + race_screens::kListBlock, 0);
        if (color) guest::StoreU32(base, b + 156 + race_screens::kListBlock, 0);
      }
    };
  };
  Deleting(h, "Type8ListPartInner::DeletingDestructor", 0x82254530, 504, shape(false),
           __imp__sub_82254530, hooks::Type8ListInnerDeletingDestruct);
  Deleting(h, "Type8ListPart::DeletingDestructor", 0x82254580, 524, shape(true),
           __imp__sub_82254580, hooks::Type8ListPartDeletingDestruct);
  Deleting(h, "Type8ColorPartInner::DeletingDestructor", 0x82253268, 768, shape(false, true),
           __imp__sub_82253268, hooks::Type8ColorInnerDeletingDestruct);
  Deleting(h, "Type8ColorListPart::DeletingDestructor", 0x822532B8, 788, shape(true, true),
           __imp__sub_822532B8, hooks::Type8ColorPartDeletingDestruct);
  for (size_t i = 0; i < kMenuGlobals.size(); i++) {
    Write(base, kMenuGlobals[i].first, menu_globals[i].data(), kMenuGlobals[i].second);
  }
  saved.Restore();
}

}  // namespace

void Type8Screens(Context& h) {
  const uint32_t world = memory::Alloc(kWorldBytes, 0);
  if (world == 0) {
    ITC_LOG_ERROR("harness: type8_screens: no world block");
    return;
  }
  Panels(h, world);
  Leaves(h, world);
  Handlers(h, world);
  Screen(h, world);
  PickCar(h, world);
  ListPart(h, world);
  memory::Free(world);
  Type8ListScreen(h);
}

}  // namespace harness
