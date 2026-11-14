// The type 6 and 7 modes (src/type67_modes/,
// docs/functions/type6-type7-modes.md) against the recompiled functions. The
// originals run with --native_type67_modes off, the plain C++ through the
// register steps (hooks/type67_modes.h).
//
// The state machine Inits: an object block whose machine (at +0, +4 or
// +460) is empty or holds random transitions already (fewer than 200 less
// the Init's, so its fit) with keys of small numbers, some the Init adds (so
// some Adds replace); the original's frame filled with the same random words
// for both (the words it never writes go into the transitions); the object
// block, the frame (but its back chain and saved registers), the scratch
// area and r3 (both halves) compared. Compared with harness::Outputs, all
// exactly (no float math).
//
// The handlers without guest calls: a world block of random words holding
// the screen, dialog or machine (r3 at its start) and the blocks its
// pointers name; the globals they use (the controller pages' two, the mode
// type) set per case and put back after. The blocks they allocate (the
// layer's pages on both sides, at different addresses) are compared by
// content where their pointers were stored (each pointer word, and r3 when
// it's one, then replaced by its index), then freed; the blocks they free
// are the layer's pages made for the case, compared by whether they're
// still pages after. Floats the originals move through float registers are
// compared with Floats (two NaNs match).

#include "harness.h"
#include "exit_table.h"
#include "loading_world.h"
#include "scene_pools_take.h"

#include "base/flags.h"
#include "accessories/accessories.h"
#include "archive/archive.h"
#include "c_runtime/exit.h"
#include "car_models/scene_fill.h"
#include "base/log.h"
#include "cameras/cameras.h"
#include "car_specs/car_specs.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/archive.h"
#include "hooks/car_models.h"
#include "hooks/heap.h"
#include "hooks/network_mode.h"
#include "hooks/type67_modes.h"
#include "hooks/type2_mode.h"
#include "hooks/type8_mode.h"
#include "hooks/ui_movies.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "network_mode/network_mode.h"
#include "mode_tick/mode_tick.h"
#include "race_loading/race_loading.h"
#include "race_setup/race_setup.h"
#include "scene/scene.h"
#include "scene_loaders/pools.h"
#include "type67_modes/type67_modes.h"
#include "type2_mode/type2_mode.h"
#include "type8_mode/type8_mode.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <limits>
#include <mutex>
#include <optional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type67_modes);
ITC_FLAG_DECLARE(bool, native_network_mode);
ITC_FLAG_DECLARE(bool, native_type2_mode);
ITC_FLAG_DECLARE(bool, native_type8_mode);

namespace time_hooks {
void SetFakeNow(uint64_t now);
void ClearFakeNow();
}  // namespace time_hooks

namespace harness {

// The stand's car world (src/harnesses/car_makers.cpp): LayGarage's car
// (+0x4000), its spec (+0xA000, its id 5) and parameters (+0x9100) in a
// 0x10000-byte block. (The same definition is in car_makers.cpp.)
struct StandWorld {
  uint32_t params, holder_word, list;
};
StandWorld LayShowCarWorld(uint8_t* base, uint32_t block, Random& r, int c, uint32_t holder);

namespace {

namespace t67 = type67_modes;

constexpr uint32_t kObjectBytes = 0x2A00;  // the node screen's machine at +460

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

uint32_t MachineOffset(uint32_t address) {
  switch (address) {
    case 0x822454B8:  // Type7Screen0::Init
    case 0x8223F9A8:
    case 0x822436A0:
    case 0x82230F80:
    case 0x82240998:
    case 0x82237B78:
    case 0x822470A8:
      return 4;
    case 0x82237320:  // Type6NodeScreen::InitMachine
      return 460;
    default:
      return 0;
  }
}

GuestFunction Recompiled(uint32_t address) {
  switch (address) {
    case 0x82235138: return __imp__sub_82235138;
    case 0x822454B8: return __imp__sub_822454B8;
    case 0x822329A8: return __imp__sub_822329A8;
    case 0x82242420: return __imp__sub_82242420;
    case 0x821C9918: return __imp__sub_821C9918;
    case 0x821C9430: return __imp__sub_821C9430;
    case 0x821CB6E8: return __imp__sub_821CB6E8;
    case 0x821CB090: return __imp__sub_821CB090;
    case 0x821A8210: return __imp__sub_821A8210;
    case 0x8223F9A8: return __imp__sub_8223F9A8;
    case 0x822436A0: return __imp__sub_822436A0;
    case 0x82230F80: return __imp__sub_82230F80;
    case 0x82240998: return __imp__sub_82240998;
    case 0x82237320: return __imp__sub_82237320;
    case 0x82237B78: return __imp__sub_82237B78;
    case 0x822470A8: return __imp__sub_822470A8;
    default: return nullptr;
  }
}

void Inits(Context& h, const Block& object) {
  namespace gm = game_modes;
  for (const t67::InitInfo& info : t67::kInits) {
    Results results(fmt::format("{} (sub_{:08X})", info.name, info.address));
    const uint32_t saves = (32 - info.first_saved) * 8 + 8;
    const uint32_t frame = CallContext(h).r1.u32 - info.frame;
    const uint32_t machine = object.address + MachineOffset(info.address);
    for (uint64_t seed = 0; seed < 12; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 151 + info.address);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, object.address, object.bytes, r);
        WriteRandom(h.base, frame, info.frame, r);
        if (seed % 2 == 0) {
          gm::StateMachineConstruct(h.base, machine);
          guest::StoreU32(h.base, machine + gm::kTransitionCount, 0);
        } else {
          // Keys of small numbers: states below 12, messages below 36, so
          // some match the Init's (Add replaces those).
          const uint32_t count = 8 + r.Below(160);
          guest::StoreU32(h.base, machine + gm::kTransitionCount, count);
          for (uint32_t i = 0; i < gm::kTransitions; i++) {
            const uint32_t at = machine + gm::kTransitionBytes * i;
            guest::StoreU32(h.base, at, r.Below(12));
            guest::StoreU32(h.base, at + 4, r.Below(36));
            guest::StoreU32(h.base, at + 8, r.Below(8) == 0 ? 1 : 0);
            guest::StoreU32(h.base, at + 12, 0);
          }
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object.address;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          Recompiled(info.address)(t, h.base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          hooks::Type67Init(t, h.base, info.address);
        }
        out[v].Bytes(Read(h.base, object.address, object.bytes));
        out[v].Bytes(Read(h.base, frame + 80, info.frame - 80 - saves));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The handlers without guest calls ---

constexpr uint32_t kWorldBytes = 0x4000;

// What a handler case leaves to compare: the world, the globals, r3, the
// allocated blocks by content and the freed blocks' state.
struct Side {
  Outputs out;
  std::vector<uint32_t> allocated;  // blocks to free after the side
};

// The pointer word at `slot` names a block the side allocated: its first
// `bytes` compared (`floats_from` on as floats), the word replaced by the
// block's index in the side's list.
void Allocated(uint8_t* base, Side& side, uint32_t slot, uint32_t bytes,
               uint32_t floats_from = 0xFFFFFFFF) {
  const uint32_t block = guest::LoadU32(base, slot);
  if (block == 0 || !memory::IsPages(block)) {
    side.out.Word(0xDEAD0000 | (block != 0));
    return;
  }
  if (floats_from < bytes) {
    side.out.Bytes(Read(base, block, floats_from));
    side.out.Floats(base, block + floats_from, (bytes - floats_from) / 4);
  } else {
    side.out.Bytes(Read(base, block, bytes));
  }
  guest::StoreU32(base, slot, 0xA110C000 + uint32_t(side.allocated.size()));
  side.allocated.push_back(block);
}

struct Globals {
  explicit Globals(uint8_t* base) : base(base) {
    for (int i = 0; i < 3; i++) saved[i] = guest::LoadU32(base, kAddresses[i]);
    saved_missing = guest::LoadU32(base, kMissingMovie);
  }
  ~Globals() {
    for (int i = 0; i < 3; i++) guest::StoreU32(base, kAddresses[i], saved[i]);
    guest::StoreU32(base, kMissingMovie, saved_missing);
  }
  // The word UiMovies::IsLoaded reads as the movie for an id not in the
  // table (the movies 16 to 19 and 22 aren't when the harnesses run): a
  // movie block of the harness's, its loaded flag (+0x9FD0) set per case.
  static constexpr uint32_t kMissingMovie = 0x826C5CDC;
  uint32_t saved_missing;
  static constexpr uint32_t kAddresses[3] = {t67::kControllerData, t67::kControllerPage,
                                             t67::kModeType};
  uint8_t* base;
  uint32_t saved[3];
};

uint32_t g_movie = 0;  // the harness's movie block (0xA000 bytes)
// Whether the cases plant it (off for the UiMovies::Delete callers: Delete
// would unload a movie it finds there; with 0 it finds none, as in the
// game when the id isn't in the table).
bool g_plant_movie = true;
constexpr uint32_t kMovieLoaded = 0x9FD0;

using Native = std::function<void(PPCContext&, uint8_t*)>;
// setup(r, world): the inputs (the same random words on both sides); args(t,
// world): the registers; collect(side, t, world): what to compare besides
// the world, the globals and r3 (`r3_block`: r3 is an allocated block of
// that many bytes, compared by content).
using HandlerSetup = std::function<void(Random&, uint32_t)>;
using HandlerArgs = std::function<void(PPCContext&, uint32_t)>;
using HandlerCollect = std::function<void(Side&, PPCContext&, uint32_t)>;

// The flag a Handler's recompiled side turns off (type 6 and 7's when null).
bool* g_handler_flag = nullptr;

void Handler(Context& h, const Block& world, const char* name, uint32_t address, int cases,
             const HandlerSetup& setup, const HandlerArgs& args, GuestFunction recompiled,
             const Native& native, const HandlerCollect& collect = {}) {
  bool& flag = g_handler_flag != nullptr ? *g_handler_flag : ITC_FLAG(native_type67_modes);
  Results results(fmt::format("{} (sub_{:08X})", name, address));
  const Globals globals(h.base);
  for (int c = 0; c < cases; c++) {
    Side sides[2];
    for (int v = 0; v < 2; v++) {
      Side& side = sides[v];
      Random r(uint64_t(c) * 7919 + address);
      WriteRandom(h.base, world.address, world.bytes, r);
      // The stack the calls' frames go in (so a word only one side writes
      // shows).
      WriteRandom(h.base, CallContext(h).r1.u32 - 2048, 2048, r);
      guest::StoreU32(h.base, t67::kControllerData, 0);
      guest::StoreU32(h.base, t67::kControllerPage, 0);
      guest::StoreU32(h.base, t67::kModeType, r.Below(10));
      guest::StoreU32(h.base, Globals::kMissingMovie, g_plant_movie ? g_movie : 0);
      guest::StoreU32(h.base, g_movie + kMovieLoaded, r.Below(3) ? 1 : 0);
      setup(r, world.address);
      PPCContext t = CallContext(h);
      t.r3.u64 = world.address;
      args(t, world.address);
      if (v == 0) {
        flag = false;
        recompiled(t, h.base);
        flag = true;
      } else {
        native(t, h.base);
      }
      if (collect) {
        collect(side, t, world.address);
      }
      side.out.Bytes(Read(h.base, world.address, world.bytes));
      for (const uint32_t g : Globals::kAddresses) {
        side.out.Word(guest::LoadU32(h.base, g));
      }
      side.out.Word(t.r3.u32);
      side.out.Word(uint32_t(t.r3.u64 >> 32));
      for (const uint32_t block : side.allocated) {
        memory::FreePages(block);
      }
    }
    results.Check(sides[0].out, sides[1].out, fmt::format("case {}", c));
  }
  results.Report();
}

// A freeing step's adapter step: an error when it refused.
Native Frees(bool (*step)(PPCContext&, uint8_t*), const char* name) {
  return [step, name](PPCContext& t, uint8_t* base) {
    if (!step(t, base)) {
      ITC_LOG_ERROR("harness: type67_modes: {} refused", name);
    }
  };
}

void Handlers(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  const HandlerSetup none = [](Random&, uint32_t) {};
  const HandlerArgs plain = [](PPCContext&, uint32_t) {};
  // A word at +`offset` of the screen: 0 or a random small number.
  auto maybe = [base](Random& r, uint32_t address) {
    guest::StoreU32(base, address, r.Below(2) ? 0 : 1 + r.Below(5));
  };
  constexpr uint32_t kMenu = 0x3000, kPage = 0x3100;  // the blocks the pointers name

  Handler(h, world, "GarageStage::SetPlates", 0x82178DE8, 8,
          [base](Random&, uint32_t w) { guest::StoreU32(base, w, w + 0x400); },
          [](PPCContext& t, uint32_t) { t.r4.u64 = t.r5.u64 ^ 0x1234; }, __imp__sub_82178DE8,
          hooks::GarageStageSetPlates);
  Handler(h, world, "GarageStage::ResetPose", 0x8217A2D8, 4, none, plain, __imp__sub_8217A2D8,
          hooks::GarageStageResetPose);
  Handler(h, world, "CameraPreset::CameraPreset", 0x8217BE08, 4, none, plain,
          __imp__sub_8217BE08, hooks::CameraPresetConstruct);
  Handler(h, world, "PanelScreen::InitWords", 0x82230740, 4, none, plain, __imp__sub_82230740,
          hooks::PanelScreenInitWords);
  Handler(h, world, "ScreenMachine::Wait", 0x82277068, 4, none, plain, __imp__sub_82277068,
          hooks::ScreenMachineWait);
  Handler(h, world, "Screen::Wait", 0x822508F0, 4, none, plain, __imp__sub_822508F0,
          hooks::ScreenWait);
  Handler(h, world, "Type7Dialog::QueueReopen", 0x822419D0, 4, none, plain,
          __imp__sub_822419D0, hooks::DialogQueueReopen);
  Handler(h, world, "Screen::Reenter", 0x8223B740, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9612); }, plain, __imp__sub_8223B740,
          hooks::ScreenReenter);
  Handler(h, world, "ControllerOptions::Open", 0x821BC660, 4, none, plain, __imp__sub_821BC660,
          hooks::ControllerOptionsOpen);
  Handler(h, world, "ControllerOptions::Show", 0x821BB260, 8,
          [base](Random& r, uint32_t w) {
            guest::StoreU32(base, t67::kControllerPage, w + kPage);
            if (r.Below(2)) guest::StoreU32(base, w + kPage, 0xFFFFFFFF);
          },
          plain, __imp__sub_821BB260, hooks::ControllerOptionsShow);
  // The ring's count and start small, the mode type 0 to 9 (Handler's).
  Handler(h, world, "ControllerOptions::Leave", 0x821BFB48, 24,
          [base](Random& r, uint32_t w) {
            guest::StoreU32(base, w + 0x3200 + 204 + 600, r.Below(50));
            guest::StoreU32(base, w + 0x3200 + 204 + 604, r.Below(r.Below(4) ? 50 : 0x7FFFFFFF));
          },
          [](PPCContext& t, uint32_t w) { t.r4.u64 = w + 0x3200; }, __imp__sub_821BFB48,
          hooks::ControllerOptionsLeave);
  // The controller pages' steps: the menu (+0x3000) with its side -1, 1, 0
  // or any and its cursor 0 to 13 (now and then any), the settings
  // ([0x826D8494], +0x1000) with page 0's sliders around their bounds; the
  // settings' global put back after.
  {
    const uint32_t saved_settings = guest::LoadU32(base, t67::kControllerSettings);
    const HandlerSetup steps = [base](Random& r, uint32_t w) {
      guest::StoreU32(base, t67::kControllerData, w + kMenu);
      guest::StoreU32(base, t67::kControllerSettings, w + 0x1000);
      const uint32_t sides[4] = {0xFFFFFFFFu, 1, 0, uint32_t(r.Next())};
      guest::StoreU32(base, w + kMenu + 36, sides[r.Below(4)]);
      guest::StoreU32(base, w + kMenu + 0, r.Below(8) ? r.Below(14) : uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(base, w + 0x1000 + 272 + 4 * i, uint32_t(int32_t(r.Below(23)) - 11));
      }
    };
    Handler(h, world, "ControllerOptions::StepSliders", 0x821BB290, 32, steps, plain,
            __imp__sub_821BB290, hooks::ControllerOptionsStepSliders);
    Handler(h, world, "ControllerOptions::StepButtons", 0x821BB3B8, 16, steps, plain,
            __imp__sub_821BB3B8, hooks::ControllerOptionsStepButtons);
    Handler(h, world, "ControllerOptions::StepAnalog", 0x821BB590, 16, steps, plain,
            __imp__sub_821BB590, hooks::ControllerOptionsStepAnalog);
    guest::StoreU32(base, t67::kControllerSettings, saved_settings);
  }
  // SetMarkY: movie 7 (missing from the table, so UiMovies::Find answers
  // the word at 0x826C5CDC) a movie in the world (+0x1000) whose live scene
  // leads through children at small random indexes to the node it writes;
  // r3 an int (any, small or negative); the frame's words below the
  // original's register saves compared too.
  if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, 7) == Globals::kMissingMovie) {
    const HandlerSetup marks = [base](Random& r, uint32_t w) {
      const uint32_t m = w + 0x1000;
      guest::StoreU32(base, Globals::kMissingMovie, m);
      guest::StoreU32(base, m + 16, m + 0x100);
      // Each step: the node's index word (+108) i and its entry (+128 + 8 (i
      // + child)) the next node.
      const uint32_t path[4][2] = {{0x100, 8}, {0x300, 5}, {0x500, 0}, {0x700, 0}};
      for (int k = 0; k < 4; k++) {
        const uint32_t node = m + path[k][0], index = r.Below(3);
        guest::StoreU32(base, node + 108, index);
        guest::StoreU32(base, node + 128 + 8 * (index + path[k][1]),
                        k < 3 ? m + path[k + 1][0] : m + 0x900);
      }
    };
    // r3: 0, 80, a small negative or any word (the setup's at +0x1F00 and
    // +0x1F04, the high half).
    const HandlerArgs mark_y = [base](PPCContext& t, uint32_t w) {
      t.r3.u64 = (uint64_t(guest::LoadU32(base, w + 0x1F04)) << 32) |
                 guest::LoadU32(base, w + 0x1F00);
    };
    const HandlerSetup marks_y = [base, marks](Random& r, uint32_t w) {
      marks(r, w);
      const uint32_t picks[4] = {0, 80, uint32_t(-37 - int32_t(r.Below(100))),
                                 uint32_t(r.Next())};
      guest::StoreU32(base, w + 0x1F00, picks[r.Below(4)]);
      guest::StoreU32(base, w + 0x1F04, r.Below(2) ? 0 : uint32_t(r.Next()));
    };
    Handler(h, world, "ControllerOptions::SetMarkY", 0x821C0238, 16, marks_y, mark_y,
            __imp__sub_821C0238, hooks::ControllerOptionsSetMarkY,
            [base](Side& side, PPCContext& t, uint32_t) {
              side.out.Bytes(Read(base, t.r1.u32 - 112, 88));
            });
  } else {
    ITC_LOG_ERROR("harness: type67_modes: movie 7 is in the table; SetMarkY skipped");
  }
  Handler(h, world, "Type6Dialog::Shown", 0x82231F48, 4, none, plain, __imp__sub_82231F48,
          hooks::Type6DialogShown);
  Handler(h, world, "Type7Dialog::Shown", 0x822419C0, 4, none, plain, __imp__sub_822419C0,
          hooks::Type7DialogShown);
  Handler(h, world, "Type6Dialog::OnEnter", 0x82231F98, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9612); }, plain, __imp__sub_82231F98,
          hooks::Type6DialogOnEnter);
  Handler(h, world, "Type7Dialog::OnEnter", 0x82241A08, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9612); }, plain, __imp__sub_82241A08,
          hooks::Type7DialogOnEnter);
  Handler(h, world, "Type6Dialog::WaitForMovies", 0x82232910, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9608); }, plain, __imp__sub_82232910,
          hooks::Type6DialogWaitForMovies);
  Handler(h, world, "Type7Dialog::WaitForMovies", 0x82242388, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9608); }, plain, __imp__sub_82242388,
          hooks::Type7DialogWaitForMovies);
  Handler(h, world, "Type6Screen0::WaitForMovie", 0x82233B18, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9608); }, plain, __imp__sub_82233B18,
          hooks::Type6Screen0WaitForMovie);
  Handler(h, world, "Type7Screen0::WaitForMovie", 0x82244080, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9612); }, plain, __imp__sub_82244080,
          hooks::Type7Screen0WaitForMovie);
  const HandlerSetup menus = [base](Random&, uint32_t w) {
    guest::StoreU32(base, w + 9640, w + kMenu);
    guest::StoreU32(base, w + 9644, w + kPage);
  };
  Handler(h, world, "Type6Screen0::PickYes", 0x82233990, 4, menus, plain, __imp__sub_82233990,
          hooks::Type6Screen0PickYes);
  Handler(h, world, "Type6Screen0::PickNo", 0x822339A0, 4, menus, plain, __imp__sub_822339A0,
          hooks::Type6Screen0PickNo);
  Handler(h, world, "Type7Screen0::PickYes", 0x82243F28, 4, menus, plain, __imp__sub_82243F28,
          hooks::Type7Screen0PickYes);
  Handler(h, world, "Type6Screen0::GoOn", 0x822339C0, 8,
          [base, menus](Random& r, uint32_t w) {
            menus(r, w);
            if (r.Below(2)) guest::StoreU32(base, w + kPage, 3);
          },
          plain, __imp__sub_822339C0, hooks::Type6Screen0GoOn);

  // The allocating ones (over the layer, as the adapters run them).
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_ERROR("harness: type67_modes: the native heap isn't active; allocating cases skipped");
    return;
  }
  Handler(h, world, "ControllerOptions::OnEnter", 0x821BB120, 4, none, plain,
          __imp__sub_821BB120, hooks::ControllerOptionsOnEnter,
          [base](Side& side, PPCContext& t, uint32_t) {
            const bool r3_page = t.r3.u32 == guest::LoadU32(base, t67::kControllerPage);
            side.out.Word(r3_page);
            Allocated(base, side, t67::kControllerData, 68);
            Allocated(base, side, t67::kControllerPage, 28);
            t.r3.u64 = 0;
          });
  const HandlerCollect screen0_blocks = [base](Side& side, PPCContext& t, uint32_t w) {
    const uint32_t d = guest::LoadU32(base, w + 9648) == t.r3.u32 ? 0 : 4;
    side.out.Word(d);
    side.out.Word(t.r3.u32 == guest::LoadU32(base, w + d + 9648));
    Allocated(base, side, w + d + 9640, 76);
    Allocated(base, side, w + d + 9644, 28);
    Allocated(base, side, w + d + 9648, 68);
    t.r3.u64 = 0;
  };
  Handler(h, world, "Type6Screen0::OnEnter", 0x822339F0, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9612); }, plain, __imp__sub_822339F0,
          hooks::Type6Screen0OnEnter, screen0_blocks);
  Handler(h, world, "Type7Screen0::OnEnter", 0x82243F58, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9616); }, plain, __imp__sub_82243F58,
          hooks::Type7Screen0OnEnter, screen0_blocks);
  // CameraPreset::Copy: a source of random words (NaNs among them).
  Handler(h, world, "CameraPreset::Copy", 0x8217D7D0, 8, none, plain, __imp__sub_8217D7D0,
          [](PPCContext& t, uint8_t* b) { hooks::CameraPresetCopy(t, b); },
          [base](Side& side, PPCContext& t, uint32_t w) {
            guest::StoreU32(base, w + 0x3F00, t.r3.u32);
            Allocated(base, side, w + 0x3F00, 64, 4);
            t.r3.u64 = 0;
          });

  // The freeing ones: blocks made for the case.
  Handler(h, world, "ControllerOptions::OnLeft", 0x821BB1F8, 4,
          [base](Random&, uint32_t w) {
            guest::StoreU32(base, t67::kControllerData, memory::AllocPages(68, 4096));
            guest::StoreU32(base, t67::kControllerPage, memory::AllocPages(28, 4096));
            guest::StoreU32(base, w + 0x3F00, guest::LoadU32(base, t67::kControllerData));
            guest::StoreU32(base, w + 0x3F04, guest::LoadU32(base, t67::kControllerPage));
          },
          plain, __imp__sub_821BB1F8, Frees(hooks::ControllerOptionsOnLeft, "OnLeft"),
          [base](Side& side, PPCContext&, uint32_t w) {
            for (const uint32_t at : {w + 0x3F00, w + 0x3F04}) {
              side.out.Word(memory::IsPages(guest::LoadU32(base, at)));
              guest::StoreU32(base, at, 0);
            }
          });
  const HandlerCollect freed3 = [base](Side& side, PPCContext&, uint32_t w) {
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t block = guest::LoadU32(base, w + 0x3F00 + 4 * i);
      side.out.Word(memory::IsPages(block));
      if (memory::IsPages(block)) memory::FreePages(block);
      guest::StoreU32(base, w + 0x3F00 + 4 * i, 0);
    }
  };
  Handler(h, world, "Type6Screen0::OnLeft", 0x82233870, 4,
          [base](Random& r, uint32_t w) {
            for (uint32_t i = 0; i < 3; i++) {
              const uint32_t block = memory::AllocPages(64, 4096);
              guest::StoreU32(base, w + 9640 + 4 * i, block);
              guest::StoreU32(base, w + 0x3F00 + 4 * i, block);
            }
            (void)r;
          },
          plain, __imp__sub_82233870, Frees(hooks::Type6Screen0OnLeft, "Type6Screen0::OnLeft"),
          freed3);
  Handler(h, world, "Type7Screen0::OnLeft", 0x82243E08, 4,
          [base](Random& r, uint32_t w) {
            for (uint32_t i = 0; i < 3; i++) {
              const uint32_t block = memory::AllocPages(64, 4096);
              guest::StoreU32(base, w + 9644 + 4 * i, block);
              guest::StoreU32(base, w + 0x3F00 + 4 * i, block);
            }
            (void)r;
          },
          plain, __imp__sub_82243E08, Frees(hooks::Type7Screen0OnLeft, "Type7Screen0::OnLeft"),
          freed3);
  // The garage camera's presets: a camera at the world's +0 (its list at
  // +296, the count at +4392) with 0 to 4 presets of the layer's pages.
  constexpr uint32_t kMade = 0x3E00;  // the made blocks' addresses, for after
  auto presets = [base](Random& r, uint32_t camera, uint32_t made) {
    const uint32_t count = r.Below(5);
    guest::StoreU32(base, camera + 4392, count);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t block = memory::AllocPages(64, 4096);
      guest::StoreU32(base, camera + 296 + 4 * i, block);
      guest::StoreU32(base, made + 4 * i, block);
    }
    return count;
  };
  const HandlerCollect freed_made = [base](Side& side, PPCContext&, uint32_t w) {
    for (uint32_t i = 0; i < 8; i++) {
      const uint32_t block = guest::LoadU32(base, w + kMade + 4 * i);
      side.out.Word(block != 0 && memory::IsPages(block));
      if (block != 0 && memory::IsPages(block)) memory::FreePages(block);
      guest::StoreU32(base, w + kMade + 4 * i, 0);
    }
  };
  Handler(h, world, "GarageCamera::FreePresets", 0x8217D760, 8,
          [base, presets](Random& r, uint32_t w) {
            for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, w + kMade + 4 * i, 0);
            presets(r, w, w + kMade);
          },
          plain, __imp__sub_8217D760,
          Frees(hooks::GarageCameraFreePresets, "GarageCamera::FreePresets"),
          [base, freed_made](Side& side, PPCContext& t, uint32_t w) {
            freed_made(side, t, w);
            for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, w + 296 + 4 * i, 0);
          });
  // StandSlot::Free: the slot at the world's +0x3D00, its block (8 KiB of
  // the layer's pages, or none) with a camera at +112.
  Handler(h, world, "StandSlot::Free", 0x821800B0, 8,
          [base, presets](Random& r, uint32_t w) {
            for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, w + kMade + 4 * i, 0);
            if (r.Below(4) == 0) {
              guest::StoreU32(base, w + 0x3D00, 0);
              return;
            }
            const uint32_t block = memory::AllocPages(8192, 4096);
            guest::StoreU32(base, w + 0x3D00, block);
            guest::StoreU32(base, w + kMade + 28, block);
            presets(r, block + 112, w + kMade);
          },
          [](PPCContext& t, uint32_t w) { t.r3.u64 = w + 0x3D00; }, __imp__sub_821800B0,
          Frees(hooks::StandSlotFree, "StandSlot::Free"), freed_made);
  // OnPresetsLoaded: r4 the loaded data (the layer's pages: 0 to 4 records
  // of random words at +16), r6 the camera (the world's +0, 0 to 2 presets
  // already, random words not freed). The copies compared by content.
  Handler(h, world, "GarageCamera::OnPresetsLoaded", 0x8217FBE8, 12,
          [base](Random& r, uint32_t w) {
            for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, w + kMade + 4 * i, 0);
            const uint32_t data = memory::AllocPages(4096, 4096);
            const uint32_t count = r.Below(5);
            guest::StoreU32(base, data + 4, count);
            guest::StoreU32(base, data + 8, 16);
            WriteRandom(base, data + 16, 64 * count, r);
            guest::StoreU32(base, w + kMade, data);
            guest::StoreU32(base, w + 4392, r.Below(3));
          },
          [base](PPCContext& t, uint32_t w) {
            t.r4.u64 = guest::LoadU32(base, w + kMade);
            t.r6.u64 = w;
          },
          __imp__sub_8217FBE8,
          Frees(hooks::GarageCameraOnPresetsLoaded, "GarageCamera::OnPresetsLoaded"),
          [base, freed_made](Side& side, PPCContext& t, uint32_t w) {
            const uint32_t count = guest::LoadU32(base, w + 4392);
            for (uint32_t i = 0; i < count && i < 8; i++) {
              const uint32_t slot = w + 296 + 4 * i;
              if (memory::IsPages(guest::LoadU32(base, slot))) {
                Allocated(base, side, slot, 64, 4);
              }
            }
            freed_made(side, t, w);
          });
}

// --- The modes' delegate functions (NextMode, Leaving, WaitForLoads: its
// world below): a mode block (its dispatcher without
// handlers, so GameMessages::Dispatch takes the queued messages and
// delivers none; its ring's count and start small) whose +848 is a screen
// block with the checked machines' states picked (10, 6 and 11 among
// them), the answer in the world, the original's frame random words; the
// mode, the block, the globals (the leaving flag, the cars' words), the
// answer, the frame's +80 to +87 and r3 compared. ---

constexpr uint32_t kModeBytes = 1024;

// WaitForLoads' stand-in for the UI movies' update and ApplyAnimation (the
// loading screen's UiMovie::Run and FinishNamed reach them): each call's
// function, return address, r1, r3 and r4 recorded, answered 0.
std::vector<uint32_t>* g_mode_calls = nullptr;
bool ModeStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_mode_calls == nullptr || (function != 0x8211D2F8 && function != 0x825A08E0)) {
    return false;
  }
  g_mode_calls->insert(g_mode_calls->end(), {function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u32,
                                             ctx.r4.u32});
  ctx.r3.u64 = 0;
  return true;
}
constexpr uint32_t kScreenBlockBytes = 0x17000;

void Modes(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  const Block mode(kModeBytes);
  const Block block(kScreenBlockBytes);
  if (!mode.address || !block.address) return;
  const uint32_t globals[3] = {t67::kLeavingFlag, t67::kType6Layout.car, t67::kType7Layout.car};
  uint32_t saved[3];
  for (int i = 0; i < 3; i++) saved[i] = guest::LoadU32(base, globals[i]);
  struct Function {
    const char* name;
    uint32_t address;
    const t67::ModeLayout* layout;
    bool next;
    GuestFunction recompiled;
    GuestFunction native;
    bool wait = false;  // WaitForLoads
  };
  const Function functions[] = {
      {"Type6Mode::NextMode", 0x822363B8, &t67::kType6Layout, true, __imp__sub_822363B8,
       hooks::Type6ModeNextMode},
      {"Type7Mode::NextMode", 0x82246800, &t67::kType7Layout, true, __imp__sub_82246800,
       hooks::Type7ModeNextMode},
      {"Type6Mode::Leaving", 0x82236100, &t67::kType6Layout, false, __imp__sub_82236100,
       hooks::Type6ModeLeaving},
      {"Type7Mode::Leaving", 0x82246590, &t67::kType7Layout, false, __imp__sub_82246590,
       hooks::Type7ModeLeaving},
      {"Type6Mode::WaitForLoads", 0x82235F28, &t67::kType6Layout, false, __imp__sub_82235F28,
       hooks::Type6ModeWaitForLoads, true},
      {"Type7Mode::WaitForLoads", 0x822463D8, &t67::kType7Layout, false, __imp__sub_822463D8,
       hooks::Type7ModeWaitForLoads, true},
  };
  // WaitForLoads' world: the sound manager a fake in `extra` (+0; its
  // vtable at +0x100: slot 6 the getter of its +60, the bank's loading
  // answer; slot 10 a recorder), the loading screen's current movie 0 to 2
  // (each a harness movie with empty animation lists; the update and
  // ApplyAnimation recorded through the UI movies' stand-in), the stand's holder
  // chain to a stage (`extra` +0x1000) whose garage camera isn't set up
  // (GarageStage::Draw does nothing) and whose scene model (index 0 of the
  // car scene's pool 7, the scene at `extra` +0x400) is in use.
  constexpr uint32_t kExtraBytes = 0x4000, kLoadingCurrent = 0x82815E64;
  constexpr uint32_t kLoadingMovies = 0x82815E68, kCarScene = 0x826C2E44;
  const Block extra(kExtraBytes);
  const Block loading_movie(0xA100);
  if (!extra.address || !loading_movie.address) return;
  const uint32_t wait_globals[7] = {menu_sounds::kSoundManager, kLoadingCurrent,
                                    t67::kStandHolder, kCarScene, kLoadingMovies,
                                    kLoadingMovies + 4, kLoadingMovies + 8};
  uint32_t wait_saved[7];
  for (int i = 0; i < 7; i++) wait_saved[i] = guest::LoadU32(base, wait_globals[i]);
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    const uint32_t frame_bytes =
        f.wait ? 160 : f.next ? t67::kNextModeFrame : t67::kLeavingFrame;
    const uint32_t frame = CallContext(h).r1.u32 - frame_bytes;
    const t67::ModeLayout& layout = *f.layout;
    for (int c = 0; c < 40; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 977 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, mode.address, mode.bytes, r);
        WriteRandom(base, block.address, block.bytes, r);
        WriteRandom(base, frame, frame_bytes, r);
        for (const uint32_t g : globals) guest::StoreU32(base, g, uint32_t(r.Next()));
        guest::StoreU32(base, mode.address + 36 + 200, 0);  // no handlers
        guest::StoreU32(base, mode.address + 840, r.Below(4));
        guest::StoreU32(base, mode.address + 844, r.Below(200));
        guest::StoreU32(base, mode.address + 848, block.address);
        auto put = [&](const t67::MachineAt& at, uint32_t state) {
          guest::StoreU32(base, block.address + at.machine + 9604, state);
        };
        const uint32_t kOther[] = {0, 1, 4, 5, 7, 9};
        for (uint32_t i = 0; i < layout.next_count; i++) {
          put(layout.next_checks[i], r.Below(10) == 0 ? 10 : kOther[r.Below(6)]);
        }
        for (uint32_t i = 0; i < layout.leave_count; i++) {
          if (!f.next) put(layout.leave_checks[i], r.Below(12) == 0 ? kOther[r.Below(6)] : 6);
        }
        if (!f.next && !f.wait) {
          const uint32_t kList[] = {6, 6, 11, 11, 10, 5};
          put(layout.list, kList[r.Below(6)]);
        }
        if (f.wait) {
          WriteRandom(base, extra.address, kExtraBytes, r);
          const uint32_t manager = extra.address, vtable = extra.address + 0x100;
          guest::StoreU32(base, manager, vtable);
          guest::StoreU32(base, vtable + 24, 0x82090770);  // r3 = [r3 + 60]
          guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
          guest::StoreU32(base, manager + 60, r.Below(3) == 0 ? 0x101 : r.Below(4) == 0 ? 1 : 0);
          guest::StoreU32(base, menu_sounds::kSoundManager, manager);
          guest::StoreU32(base, kLoadingCurrent, r.Below(3));
          WriteRandom(base, loading_movie.address, 0xA100, r);
          ui_movies::InitAnimationLists(base, loading_movie.address);
          for (uint32_t k = 0; k < 3; k++) {
            guest::StoreU32(base, kLoadingMovies + 4 * k, loading_movie.address);
          }
          const uint32_t scene = extra.address + 0x400, node = extra.address + 0x2000;
          const uint32_t stage = extra.address + 0x800, holder = extra.address + 0x300;
          guest::StoreU32(base, extra.address + 0x340, holder);
          guest::StoreU32(base, holder, stage);
          guest::StoreU32(base, t67::kStandHolder, extra.address + 0x340);
          guest::StoreU32(base, stage + 8, 0);
          guest::StoreU32(base, stage + 112, 0);
          guest::StoreU32(base, scene + 5028, node);
          guest::StoreU32(base, node + 8, 0);
          guest::StoreU32(base, node + 16, 3);
          guest::StoreU32(base, kCarScene, scene);
          // The members' loaded words: bit 0 set mostly.
          static constexpr uint32_t kLoaded[] = {29044, 62304, 91224, 19280, 50560, 9612,
                                                 40464, 71944, 61604, 51964, 29040, 19272,
                                                 40220, 9616, 81584};
          const bool all = r.Below(3) != 0;
          for (const uint32_t at : kLoaded) {
            const uint32_t word = uint32_t(r.Next());
            guest::StoreU32(base, block.address + at,
                            all || r.Below(4) != 0 ? word | 1 : word & ~1u);
          }
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = world.address + 0x100;
        t.r4.u64 = mode.address;
        std::vector<uint32_t> calls;
        g_mode_calls = &calls;
        const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
            ui_movies_hooks::g_update_callee_stand_in;
        if (f.wait) ui_movies_hooks::g_update_callee_stand_in = ModeStandIn;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          f.native(t, base);
        }
        ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
        g_mode_calls = nullptr;
        for (const uint32_t word : calls) out[v].Word(word);
        out[v].Bytes(Read(base, mode.address, mode.bytes));
        out[v].Bytes(Read(base, block.address, block.bytes));
        for (const uint32_t g : globals) out[v].Word(guest::LoadU32(base, g));
        out[v].Bytes(Read(base, world.address + 0x100, 8));
        out[v].Bytes(Read(base, frame + 80, f.wait ? 16 : 8));
        if (f.wait) {
          out[v].Bytes(Read(base, extra.address, kExtraBytes));
          out[v].Bytes(Read(base, loading_movie.address, 0xA100));
          for (const uint32_t g : wait_globals) out[v].Word(guest::LoadU32(base, g));
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  for (int i = 0; i < 3; i++) guest::StoreU32(base, globals[i], saved[i]);
  for (int i = 0; i < 7; i++) guest::StoreU32(base, wait_globals[i], wait_saved[i]);
}

// --- The handlers with guest calls. UiMovies::Open is recorded (its
// adapter's recorder on the original's side, the plain C++'s open on the
// native side), each call's id, asset, frame and return address compared,
// answered 1. UiMovies::Delete runs for real: the movies these delete (16
// to 19, 22) aren't in the movie table when the harnesses run (checked
// first), so it finds none on both sides. The Inits' vtables are in the
// world, their slots a state machine Init of ours, PanelScreen::InitWords
// or the empty handler sub_82106540 (picked). The Draw handlers' page
// handlers: Type6Screen0::Draw's table (0x826367B8) and Type7Screen0::Draw's
// (0x82815E20, its guard set) hold the empty handler for the case (put
// back after); the dialogs' and the controller pages' tables are built in
// their frames with the real pages, so their cases have page -1 (none
// called) and compare the frame's table. ---

constexpr uint32_t kEmpty = 0x82106540;

std::vector<uint32_t> g_opens;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != 0x8211E808) return false;
  g_opens.insert(g_opens.end(), {ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

void GuestCallHandlers(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u, 22u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != 0x826C5CDC) {
      ITC_LOG_ERROR("harness: type67_modes: movie {} is in the table; the guest call cases skipped",
                    id);
      return;
    }
  }
  const uint32_t sp = CallContext(h).r1.u32;
  const HandlerArgs plain = [](PPCContext&, uint32_t) {};
  g_plant_movie = false;
  // The dispatcher at the world's +0x3200 (its ring's count and start small).
  const HandlerSetup ring = [base](Random& r, uint32_t w) {
    guest::StoreU32(base, w + 0x3200 + 204 + 600, r.Below(50));
    guest::StoreU32(base, w + 0x3200 + 204 + 604, r.Below(60));
  };
  const HandlerArgs dispatcher = [](PPCContext& t, uint32_t w) { t.r4.u64 = w + 0x3200; };
  // The frame words a handler leaves, compared (its frame below `sp`).
  auto frame_out = [base, sp](uint32_t bytes, uint32_t words) {
    return [base, sp, bytes, words](Side& side, PPCContext&, uint32_t) {
      side.out.Bytes(Read(base, sp - bytes + 80, 4 * words));
    };
  };

  {
    // Dialog::OpenMovies: the opens recorded.
    Results results("Dialog::OpenMovies (sub_82225810)");
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      g_opens.clear();
      PPCContext t = CallContext(h);
      if (v == 0) {
        const hooks::StartUpRecorder saved = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = RecordOpen;
        ITC_FLAG(native_type67_modes) = false;
        __imp__sub_82225810(t, base);
        ITC_FLAG(native_type67_modes) = true;
        hooks::g_start_up_recorder = saved;
      } else {
        t67::Calls calls = hooks::Type67Calls(t, base);
        calls.open = [](uint32_t id, uint32_t asset, uint32_t frame, uint32_t lr) -> uint64_t {
          g_opens.insert(g_opens.end(), {id, asset, frame, lr});
          return 1;
        };
        t.r3.u64 = t67::DialogOpenMovies(base, t.r1.u32, calls);
      }
      for (const uint32_t word : g_opens) out[v].Word(word);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], "the four opens");
    results.Report();
  }
  Handler(h, world, "Dialog::Leave", 0x82225C70, 8, ring, dispatcher, __imp__sub_82225C70,
          hooks::Type6DialogLeave);
  Handler(h, world, "Type7Dialog::Leave", 0x82241E58, 8, ring, dispatcher, __imp__sub_82241E58,
          hooks::Type7DialogLeave);
  Handler(h, world, "Type6Screen0::Leave", 0x82233DF8, 8, ring, dispatcher, __imp__sub_82233DF8,
          hooks::Type6Screen0Leave);
  Handler(h, world, "Type7Screen0::Leave", 0x82244350, 8, ring, dispatcher, __imp__sub_82244350,
          hooks::Type7Screen0Leave);

  // The Inits: the vtable at the world's +0x3800.
  constexpr uint32_t kVtable = 0x3800;
  auto vtable = [base](uint32_t first, uint32_t second, std::vector<uint32_t> inits) {
    return [base, first, second, inits](Random& r, uint32_t w) {
      guest::StoreU32(base, w, w + kVtable);
      const uint32_t picks[3] = {0x82230740, kEmpty, inits[r.Below(uint32_t(inits.size()))]};
      guest::StoreU32(base, w + kVtable + first, picks[r.Below(3)]);
      guest::StoreU32(base, w + kVtable + second, inits[r.Below(uint32_t(inits.size()))]);
      // The machines empty (StateMachine::Add's count at +9600).
      for (const uint32_t m : {4u, 460u}) {
        guest::StoreU32(base, w + m + 9600, r.Below(4) == 0 ? r.Below(20) : 0);
      }
    };
  };
  Handler(h, world, "ListScreen::Init", 0x822515A0, 12,
          vtable(16, 20, {0x8223F9A8, 0x822436A0}), plain, __imp__sub_822515A0,
          hooks::ListScreenInit);
  Handler(h, world, "PanelScreen::Init", 0x82248E10, 12,
          vtable(0, 4, {0x82230F80, 0x82240998}), plain, __imp__sub_82248E10,
          hooks::PanelScreenInit);
  Handler(h, world, "NodeScreen::Init", 0x82259128, 12, vtable(28, 32, {0x82237320}), plain,
          __imp__sub_82259128, hooks::NodeScreenInit);

  // The Draws.
  constexpr uint32_t kType6Pages = 0x826367B8, kType7Table = 0x82815E20, kType7Guard = 0x82815E38;
  uint32_t saved6[8], saved7[7];
  for (uint32_t i = 0; i < 8; i++) saved6[i] = guest::LoadU32(base, kType6Pages + 4 * i);
  for (uint32_t i = 0; i < 7; i++) saved7[i] = guest::LoadU32(base, kType7Table + 4 * i);
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, kType6Pages + 4 * i, kEmpty);
  constexpr uint32_t kPageBlock = 0x3100;
  Handler(h, world, "Type6Screen0::Draw", 0x82234ED8, 8,
          [base](Random& r, uint32_t w) {
            guest::StoreU32(base, w + 9644, w + kPageBlock);
            guest::StoreU32(base, w + kPageBlock, r.Below(3) ? r.Below(8) : 0xFFFFFFFF);
          },
          [](PPCContext& t, uint32_t w) { t.r4.u64 = w + 0x3200; }, __imp__sub_82234ED8,
          hooks::Type6Screen0Draw);
  // Type7Screen0::Draw: the guard clear (the table built; page -1) or set
  // with the empty handler in the table.
  Handler(h, world, "Type7Screen0::Draw", 0x82245148, 12,
          [base](Random& r, uint32_t w) {
            guest::StoreU32(base, w + 9648, w + kPageBlock);
            if (r.Below(3) == 0) {
              guest::StoreU32(base, kType7Guard, 0);
              guest::StoreU32(base, w + kPageBlock, 0xFFFFFFFF);
            } else {
              guest::StoreU32(base, kType7Guard, 1);
              for (uint32_t i = 0; i < 3; i++) {
                guest::StoreU32(base, kType7Table + 8 * i, kEmpty);
                guest::StoreU32(base, kType7Table + 8 * i + 4, r.Below(64));
              }
              guest::StoreU32(base, w + kPageBlock, r.Below(4) ? r.Below(3) : 0xFFFFFFFF);
            }
          },
          [](PPCContext& t, uint32_t w) { t.r4.u64 = w + 0x3200; }, __imp__sub_82245148,
          hooks::Type7Screen0Draw, [base, frame_out](Side& side, PPCContext& t, uint32_t w) {
            frame_out(112, 6)(side, t, w);
            for (uint32_t i = 0; i < 7; i++) side.out.Word(guest::LoadU32(base, kType7Table + 4 * i));
          });
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, kType6Pages + 4 * i, saved6[i]);
  for (uint32_t i = 0; i < 7; i++) guest::StoreU32(base, kType7Table + 4 * i, saved7[i]);
  const HandlerSetup no_page = [base](Random&, uint32_t w) {
    guest::StoreU32(base, w + 9640, 0xFFFFFFFF);
    guest::StoreU32(base, t67::kControllerPage, w + kPageBlock);
    guest::StoreU32(base, w + kPageBlock, 0xFFFFFFFF);
  };
  Handler(h, world, "Type6Dialog::Draw", 0x82232740, 2, no_page, dispatcher, __imp__sub_82232740,
          hooks::Type6DialogDraw, frame_out(96, 2));
  Handler(h, world, "Type7Dialog::Draw", 0x822421C0, 2, no_page, dispatcher, __imp__sub_822421C0,
          hooks::Type7DialogDraw, frame_out(96, 2));
  Handler(h, world, "ControllerOptions::Draw", 0x821C7F28, 2, no_page, dispatcher,
          __imp__sub_821C7F28, hooks::ControllerOptionsDraw, frame_out(112, 3));
}

// --- GarageCamera::LoadPresets: Archive::Load recorded (ArchiveLoads),
// each call's words compared but the destination, which is compared by
// content (the presets' size from the TOC) and freed. StandScene::Release:
// the stand's holder, its slots (none, a slot without a block, or one with
// a block and 0 to 3 presets) and the viewports' block and two viewports
// made of the layer's pages for the case (the game's globals put back
// after); whether each made block is still pages after, the globals and r3
// compared. ---

void Garage(Context& h) {
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    ITC_LOG_ERROR("harness: type67_modes: the native heap or archive queue is off; garage skipped");
    return;
  }
  {
    Results results("GarageCamera::LoadPresets (sub_821802D8)");
    const uint32_t entry = archive::Toc(base, t67::kPresetsAsset) + 0x60000 - 31600;
    const uint32_t size = guest::LoadU32(base, entry + 8) != 0 ? guest::LoadU32(base, entry + 8)
                                                              : guest::LoadU32(base, entry + 4);
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        ArchiveLoads loads({c % 2 == 0 ? int64_t(c + 5) : -1});
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 64 * c;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          __imp__sub_821802D8(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          hooks::GarageCameraLoadPresets(t, base);
        }
        std::vector<uint32_t> calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        if (calls.size() >= 2) {
          const uint32_t destination = calls[1];
          out[v].Word(memory::IsPages(destination));
          out[v].Bytes(Read(base, destination, size < 0x10000 ? size : 0x10000));
          memory::FreePages(destination);
          calls[1] = 0;
        }
        for (const uint32_t w : calls) out[v].Word(w);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("StandScene::Release (sub_82180738)");
    const uint32_t saved_holder = guest::LoadU32(base, t67::kStandHolder);
    const uint32_t saved_viewports = guest::LoadU32(base, cameras::kViewports);
    for (int c = 0; c < 12; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 131 + 0x82180738);
        std::vector<uint32_t> made;
        auto pages = [&](uint32_t bytes) {
          const uint32_t block = memory::AllocPages(bytes, 4096);
          WriteRandom(base, block, bytes < 64 ? bytes : 64, r);
          made.push_back(block);
          return block;
        };
        const uint32_t viewports = pages(12);
        guest::StoreU32(base, viewports + 0, pages(1872));
        guest::StoreU32(base, viewports + 4, pages(1872));
        guest::StoreU32(base, cameras::kViewports, viewports);
        const uint32_t holder = pages(12);
        guest::StoreU32(base, t67::kStandHolder, holder);
        for (uint32_t i = 0; i < 3; i++) {
          uint32_t slot = 0;
          const uint32_t kind = r.Below(3);
          if (kind != 0) {
            slot = pages(64);
            guest::StoreU32(base, slot, 0);
            if (kind == 2) {
              const uint32_t block = pages(8192);
              guest::StoreU32(base, slot, block);
              const uint32_t count = r.Below(4);
              guest::StoreU32(base, block + 112 + 4392, count);
              for (uint32_t p = 0; p < count; p++) {
                guest::StoreU32(base, block + 112 + 296 + 4 * p, pages(64));
              }
            }
          }
          guest::StoreU32(base, holder + 4 * i, slot);
        }
        PPCContext t = CallContext(h);
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          __imp__sub_82180738(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else if (!hooks::StandSceneRelease(t, base)) {
          ITC_LOG_ERROR("harness: type67_modes: StandScene::Release refused");
        }
        out[v].Word(uint32_t(made.size()));
        for (const uint32_t block : made) {
          out[v].Word(memory::IsPages(block));
          if (memory::IsPages(block)) memory::FreePages(block);
        }
        out[v].Word(guest::LoadU32(base, t67::kStandHolder));
        out[v].Word(guest::LoadU32(base, cameras::kViewports));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    guest::StoreU32(base, t67::kStandHolder, saved_holder);
    guest::StoreU32(base, cameras::kViewports, saved_viewports);
    results.Report();
  }
}

// --- The garage camera: Quaternion::FromAngles on angles of every kind
// (zeros, small, large, NaN, infinities, denormals, random bits), the
// quaternion and the scratch below the stack pointer compared as floats;
// GarageCamera::Reset on a camera of random words, the camera, its frame's
// vectors (+80 to +255) and r3 compared. ---

double AngleOf(Random& r) {
  switch (r.Below(10)) {
    case 0: return 0.0;
    case 1: return -0.0;
    case 2: return double(float(int32_t(r.Below(2000)) - 1000) / 100.0f);
    case 3: return double(float(int32_t(r.Below(200000)) - 100000) * 1.7f);
    case 4: return std::numeric_limits<double>::quiet_NaN();
    case 5: return r.Below(2) ? std::numeric_limits<double>::infinity()
                              : -std::numeric_limits<double>::infinity();
    case 6: return double(std::bit_cast<float>(uint32_t(r.Below(0x800000)) | (r.Below(2) << 31)));
    case 7: return double(std::bit_cast<float>(uint32_t(r.Next())));
    default: return double(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 65536.0f);
  }
}

void GarageCamera(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  {
    Results results("Quaternion::FromAngles (sub_8217C120)");
    for (int c = 0; c < 64; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 613 + 0x8217C120);
        PPCContext t = CallContext(h);
        // A stack pointer off 16-byte alignment in some cases (lvlx reads
        // from where the words land).
        t.r1.u64 = t.r1.u32 - 4 * r.Below(4) * (c % 4 == 3);
        WriteRandom(base, t.r1.u32 - 64, 64, r);
        WriteRandom(base, world.address, 64, r);
        t.f1.f64 = AngleOf(r);
        t.f2.f64 = AngleOf(r);
        t.f3.f64 = AngleOf(r);
        t.r3.u64 = world.address + 16 * r.Below(3) + (c % 5 == 4 ? 4 : 0);
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          __imp__sub_8217C120(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          hooks::QuaternionFromAngles(t, base);
        }
        out[v].Floats(base, world.address, 16);
        out[v].Floats(base, t.r1.u32 - 32, 4);
        out[v].Bytes(Read(base, t.r1.u32 - 64, 32));
        out[v].Bytes(Read(base, t.r1.u32 - 16, 16));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageCamera::Reset (sub_8217D888)");
    const uint32_t frame = CallContext(h).r1.u32 - t67::kGarageCameraResetFrame;
    for (int c = 0; c < 8; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 389 + 0x8217D888);
        WriteRandom(base, world.address, 4400, r);
        WriteRandom(base, frame - 64, t67::kGarageCameraResetFrame + 64, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = world.address + 16 * r.Below(4) + (uint64_t(r.Below(2)) << 32);
        const uint32_t camera = t.r3.u32;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          __imp__sub_8217D888(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          hooks::GarageCameraReset(t, base);
        }
        out[v].Bytes(Read(base, camera, 16));
        out[v].Floats(base, camera + 16, (272 - 16) / 4);
        out[v].Bytes(Read(base, camera + 272, 4400 - 272 - 64));
        out[v].Floats(base, frame + 80, (256 - 80) / 4);
        out[v].Floats(base, frame - 32, 4);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// GarageCamera::Command and GarageStage::CameraCommand: a camera of random
// words (the stage's +112: the stage at the world's start, its holder's
// word at +0x3F00), the fields the commands step (+100, +176, +180, +224,
// +240 to +264) angles of every kind, +272 (the stage's +384) 0, 1, 2 or
// any; the command 0 to 22 or any (above 20: only the matrices). The
// camera, the frame (+80 to +159: the angle and the identity) and r3
// compared; the frames below (Reset's, RotateX's) aren't.
void GarageCameraCommands(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  const uint32_t stage = world.address, camera = stage + 112, holder = world.address + 0x3F00;
  for (const bool through_stage : {false, true}) {
    Results results(through_stage ? "GarageStage::CameraCommand (sub_821808F0; sub_82180358)"
                                  : "GarageCamera::Command (sub_82180358; sub_821803A0)");
    for (int c = 0; c < 200; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 977 + (through_stage ? 0x821808F0 : 0x82180358));
        WriteRandom(base, world.address, 112 + 4400, r);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 192;
        WriteRandom(base, frame - 1024, 1024 + 192, r);
        for (const uint32_t field : {100u, 176u, 180u, 224u, 240u, 244u, 248u, 256u, 260u, 264u}) {
          guest::StoreFloat(base, camera + field, float(AngleOf(r)));
        }
        const uint32_t moved[4] = {0, 1, 2, uint32_t(r.Next())};
        guest::StoreU32(base, camera + 272, moved[r.Below(4)]);
        t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(23);
        guest::StoreU32(base, holder, stage);
        t.r3.u64 = through_stage ? holder : camera;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          (through_stage ? __imp__sub_821808F0 : __imp__sub_82180358)(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          (through_stage ? hooks::GarageStageCameraCommand : hooks::GarageCameraCommand)(t, base);
        }
        out[v].Bytes(Read(base, stage, 112 + 16));
        out[v].Floats(base, camera + 16, (272 - 16) / 4);
        out[v].Bytes(Read(base, camera + 272, 4400 - 272));
        out[v].Word(guest::LoadU32(base, frame));
        out[v].Floats(base, frame + 80, (160 - 80) / 4);
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// GarageLights::Init: the lights (random words) at the world's +0x1000, the
// globals it writes (0x8265ED00 and the scene light constants) random words
// first; the table it copies (0x82635950) the game's in some cases, else
// random floats with zero, NaN and infinite spot directions (put back
// after). The lights, the globals, the frame's +80 to +127 and r3 compared.
void GarageLights(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  Results results("GarageLights::Init (sub_8217D3C8)");
  constexpr uint32_t kTable = t67::kGarageLightsTable, kTableBytes = 11 * 96;
  constexpr uint32_t kConstants = 0x8296B950, kConstantBytes = 0x350;
  std::vector<uint8_t> table = Read(base, kTable, kTableBytes);
  std::vector<uint8_t> constants = Read(base, kConstants, kConstantBytes);
  std::vector<uint8_t> ambient = Read(base, 0x8265ED00, 16);
  const uint32_t frame = CallContext(h).r1.u32 - t67::kGarageLightsFrame;
  const uint32_t lights = world.address + 0x1000;
  for (int c = 0; c < 16; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(uint64_t(c) * 271 + 0x8217D3C8);
      WriteRandom(base, lights, 1088, r);
      WriteRandom(base, kConstants, kConstantBytes, r);
      WriteRandom(base, 0x8265ED00, 16, r);
      WriteRandom(base, frame, t67::kGarageLightsFrame, r);
      Write(base, kTable, table.data(), kTableBytes);
      if (c >= 2) {
        for (uint32_t i = 0; i < kTableBytes; i += 4) {
          guest::StoreFloat(base, kTable + i, float(AngleOf(r)));
        }
        // A spot's direction (q + 16, the table's +288 + 96i): zero, tiny,
        // NaN or infinite in some.
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t d = kTable + 288 + 16 + 96 * i;
          switch (r.Below(4)) {
            case 0:
              for (uint32_t k = 0; k < 3; k++) guest::StoreU32(base, d + 4 * k, 0);
              break;
            case 1:
              for (uint32_t k = 0; k < 3; k++) guest::StoreU32(base, d + 4 * k, r.Below(0x800000));
              break;
            default:
              break;
          }
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = lights;
      if (v == 0) {
        ITC_FLAG(native_type67_modes) = false;
        __imp__sub_8217D3C8(t, base);
        ITC_FLAG(native_type67_modes) = true;
      } else {
        hooks::GarageLightsInit(t, base);
      }
      out[v].Bytes(Read(base, lights, 16));
      out[v].Floats(base, lights + 16, (1088 - 16) / 4);
      out[v].Floats(base, kConstants, kConstantBytes / 4);
      out[v].Floats(base, 0x8265ED00, 4);
      out[v].Floats(base, frame + 80, 12);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  Write(base, kTable, table.data(), kTableBytes);
  Write(base, kConstants, constants.data(), kConstantBytes);
  Write(base, 0x8265ED00, ambient.data(), 16);
  results.Report();
}

// GarageCamera::Init, GarageStage::Init and StandSlot::Init: the object (a
// camera, a stage or a slot with a stage's room after it) random words at
// the world's +0x100, Archive::Load recorded (ArchiveLoads, a serial or -1),
// its destination (the presets' block) compared by content and freed;
// StandSlot::Init's stage (when the slot is 0) compared by content and
// freed; the lights' globals random first and compared; the frames' words
// the functions write (not their saved registers) and r3 compared.
void StandSlots(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    ITC_LOG_ERROR("harness: type67_modes: the native heap or archive queue is off; stand slots skipped");
    return;
  }
  constexpr uint32_t kConstants = 0x8296B950, kConstantBytes = 0x350;
  std::vector<uint8_t> constants = Read(base, kConstants, kConstantBytes);
  std::vector<uint8_t> ambient = Read(base, 0x8265ED00, 16);
  const uint32_t entry = archive::Toc(base, t67::kPresetsAsset) + 0x60000 - 31600;
  const uint32_t size = guest::LoadU32(base, entry + 8) != 0 ? guest::LoadU32(base, entry + 8)
                                                            : guest::LoadU32(base, entry + 4);
  const uint32_t sp = CallContext(h).r1.u32;
  const uint32_t object = world.address + 0x100;
  struct Function {
    const char* name;
    uint32_t address;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t stage_frame;  // GarageStage::Init's frame (0: none)
  };
  const Function functions[] = {
      {"GarageCamera::Init", 0x82180918, __imp__sub_82180918, hooks::GarageCameraInit, 0},
      {"GarageStage::Init", 0x82180968, __imp__sub_82180968, hooks::GarageStageInit,
       sp - t67::kGarageStageInitFrame},
      {"StandSlot::Init", 0x82180AA0, __imp__sub_82180AA0, hooks::StandSlotInit,
       sp - t67::kStandSlotInitFrame - t67::kGarageStageInitFrame},
  };
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    for (int c = 0; c < 8; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 523 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, sp - 2048, 2048, r);
        WriteRandom(base, kConstants, kConstantBytes, r);
        WriteRandom(base, 0x8265ED00, 16, r);
        const bool slot = f.address == 0x82180AA0;
        // StandSlot::Init: the slot empty (a stage made) or not (nothing).
        if (slot && c % 4 != 3) guest::StoreU32(base, object, 0);
        ArchiveLoads loads({c % 2 == 0 ? int64_t(c + 3) : -1});
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        t.r4.u64 = r.Below(3);
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          f.native(t, base);
        }
        std::vector<uint32_t> calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        if (calls.size() >= 2) {
          const uint32_t destination = calls[1];
          out[v].Word(memory::IsPages(destination));
          out[v].Bytes(Read(base, destination, size < 0x10000 ? size : 0x10000));
          memory::FreePages(destination);
          calls[1] = 0;
          // The camera's address as the load's context: compared by its
          // offset in the stage (the stage's address differs per side).
          if (calls.size() >= 4) calls[3] -= slot ? guest::LoadU32(base, object) : 0;
        }
        for (const uint32_t w : calls) out[v].Word(w);
        if (slot) {
          const uint32_t stage = guest::LoadU32(base, object);
          const bool made = c % 4 != 3;
          out[v].Word(made && memory::IsPages(stage));
          if (made && memory::IsPages(stage)) {
            out[v].Bytes(Read(base, stage, t67::kGarageStageBytes));
            memory::FreePages(stage);
            guest::StoreU32(base, object, 0);
          }
          out[v].Bytes(Read(base, object + 4, 0x1000));
        } else {
          out[v].Bytes(Read(base, object, 0x2000));
        }
        out[v].Bytes(Read(base, kConstants, kConstantBytes));
        out[v].Bytes(Read(base, 0x8265ED00, 16));
        // The frames: GarageCamera::Init's Reset frame and FromAngles'
        // scratch; GarageStage::Init's +80 to +111, the lights' and Reset's
        // frames below it (but the lights' saved registers).
        if (f.stage_frame == 0) {
          const uint32_t reset = sp - t67::kGarageCameraInitFrame - t67::kGarageCameraResetFrame;
          // Not +192: a word GarageCamera::LoadPresets' callees' frames
          // leave there on the recompiled side (the plain C++ replays no
          // frame for them, as in the first migration).
          out[v].Bytes(Read(base, reset + 80, 112));
          out[v].Bytes(Read(base, reset + 196, 60));
          out[v].Bytes(Read(base, reset - 32, 16));
        } else {
          const uint32_t g = f.stage_frame;
          out[v].Bytes(Read(base, g + 80, 32));
          out[v].Bytes(Read(base, g - 316, 124));  // not the lights' back chain
          out[v].Bytes(Read(base, g - 432, 16));
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Write(base, kConstants, constants.data(), kConstantBytes);
  Write(base, 0x8265ED00, ambient.data(), 16);
}

// --- The dialogs' confirm: the dialog at the world's +0 (random words, its
// step word +9664, cursors +9668 and +9672 0 or 1, the loaded word +9608,
// the reopen word +9748 picked); movie 16 a harness movie (0xA100 bytes, its
// default choice at +40960 0 or 1) in a harness movie table (with a few
// other ids), its live, saved and file scenes pool nodes whose children are
// pool nodes tagged TFM, PIC, TXT or nothing; its named animations (16) on
// the live scene with harness animations (no tracks, a length), now and then
// a slot in its queue or looping list; the sound manager a fake whose slot
// 10 records (as the race HUD object's harness has it). UiMovie::Update
// (UiMovie::Run's jump) and ApplyAnimation (FinishAll's and FinishNamed's)
// recorded through the UI movies' stand-in, answered from the case's
// generator. Movie 22 is the same movie (Type7Screen0::FadeOutStep and
// ChosenPage: the screen's page state and menu blocks at the world's +0x3800
// and +0x3900, the fake manager's slot 13 a recorder, the screen fades'
// words random with their counts mostly 0, compared and put back after).
// The pages (ConfirmPage) and Updates add the players' mappers
// (menu_helpers' harness's shape: the vtables' slots 10 and 12 Pressed and
// Fired), the dialog's menu at +9668, the dispatcher at the world's +0x3200
// and the step, queued step and queued state words; the Updates run the
// steps from their table recompiled on both sides. The world, the movie, the
// nodes, the fake manager, the mappers, the calls, the stack below the call
// (but the original's back chain and saved registers, and the step's saves
// below an Update's frame) and r3 compared. ---

struct DialogCall {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<DialogCall>* g_dialog_calls = nullptr;
Random g_dialog_answers;

bool DialogStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_dialog_calls == nullptr || (function != 0x8211D2F8 && function != 0x825A08E0)) {
    return false;
  }
  g_dialog_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = function == 0x8211D2F8 ? g_dialog_answers.Below(2) : g_dialog_answers.Next();
  return true;
}

// What a dialog case runs: its kind (kDraw: a Draw, its pages run
// recompiled on both sides as an Update's steps), whether the movie table
// holds ids 16, 19, 20 and 22 (kWide; else 16 and 22), and the flag its
// recompiled side turns off (type 8's also give the asking player
// [0x82814E6C] random words, compared).
enum DialogKind { kStep, kPage, kUpdate, kScreen7, kScreen6, kDraw };
struct DialogFunction {
  const char* name;
  uint32_t address, frame, saves;
  GuestFunction recompiled, native;
  DialogKind kind = kStep;
  bool wide = false;
  bool type8 = false;
};

void RunDialogs(Context& h, const Block& world, const DialogFunction* functions, size_t count);

void Dialogs(Context& h, const Block& world) {
  using Function = DialogFunction;
  const Function functions[] = {
      {"Type6Dialog::ConfirmPage", 0x82232328, 128, 48, __imp__sub_82232328,
       hooks::Type6DialogConfirmPage, kPage},
      {"Type7Dialog::ConfirmPage", 0x82241EF8, 128, 48, __imp__sub_82241EF8,
       hooks::Type7DialogConfirmPage, kPage},
      {"Type6Dialog::Update", 0x82232798, 160, 32, __imp__sub_82232798,
       hooks::Type6DialogUpdate, kUpdate},
      {"Type7Dialog::Update", 0x82242218, 160, 32, __imp__sub_82242218,
       hooks::Type7DialogUpdate, kUpdate},
      {"Type7Screen0::FadeOutStep", 0x82244760, 112, 24, __imp__sub_82244760,
       hooks::Type7Screen0FadeOutStep, kScreen7},
      {"Type7Screen0::ChosenPage", 0x82244610, 112, 16, __imp__sub_82244610,
       hooks::Type7Screen0ChosenPage, kScreen7},
      {"Type6Screen0::FadeOutStep", 0x822342F0, 112, 24, __imp__sub_822342F0,
       hooks::Type6Screen0FadeOutStep, kScreen6},
      {"Type6Screen0::ChosenPage", 0x82234190, 112, 16, __imp__sub_82234190,
       hooks::Type6Screen0ChosenPage, kScreen6},
      {"Type6Dialog::OpenConfirm", 0x822320D8, 112, 24, __imp__sub_822320D8,
       hooks::Type6DialogOpenConfirm},
      {"Type7Dialog::OpenConfirm", 0x82241B40, 112, 24, __imp__sub_82241B40,
       hooks::Type7DialogOpenConfirm},
      {"Dialog::DrawWaiting", 0x82241C18, 128, 32, __imp__sub_82241C18, hooks::DialogDrawWaiting},
      {"Dialog::OpenConfirmStep", 0x822325E0, 128, 40, __imp__sub_822325E0,
       hooks::DialogOpenConfirmStep},
      {"Dialog::CloseConfirmStep", 0x8223AA10, 128, 32, __imp__sub_8223AA10,
       hooks::DialogCloseConfirmStep},
      {"Dialog::MoveCursorStep", 0x8223AB50, 128, 32, __imp__sub_8223AB50,
       hooks::DialogMoveCursorStep},
      {"Dialog::OpenNoticeStep", 0x822326A8, 112, 24, __imp__sub_822326A8,
       hooks::DialogOpenNoticeStep, kStep, true},
      {"Dialog::CloseNoticeStep", 0x8223AAA8, 112, 24, __imp__sub_8223AAA8,
       hooks::DialogCloseNoticeStep, kStep, true},
  };
  RunDialogs(h, world, functions, std::size(functions));
}

void RunDialogs(Context& h, const Block& world, const DialogFunction* functions, size_t count) {
  using Function = DialogFunction;
  uint8_t* const base = h.base;
  constexpr uint32_t kMovieBlockBytes = 0xA100, kNodeBytes = 0x800, kNodes = 8;
  const Block movie_block(kMovieBlockBytes);
  const Block nodes(kNodeBytes * kNodes + 8 * 64);
  const Block misc(0x1000);  // the pairs (+0), the fake manager (+0x400), its vtable (+0x800)
  if (!movie_block.address || !nodes.address || !misc.address) return;
  const uint32_t movie = movie_block.address;
  auto node = [&](uint32_t i) { return nodes.address + kNodeBytes * i; };
  const uint32_t animations = nodes.address + kNodeBytes * kNodes;
  const uint32_t sp = CallContext(h).r1.u32;
  // The pages' and Updates' worlds: the players' mappers (their vtables at
  // +0x100, the mappers at +0x200) and the dispatcher at the world's +0x3200.
  const Block mappers(0x1000);
  if (!mappers.address) return;
  const uint32_t dispatcher = world.address + 0x3200;
  constexpr uint32_t kPlayer = 0x82814E6C;
  const uint32_t saved_player = guest::LoadU32(base, kPlayer);
  const std::vector<uint8_t> table = Read(base, ui_movies::kMovieTable, 12);
  const uint32_t saved_manager = guest::LoadU32(base, menu_sounds::kSoundManager);
  const std::vector<uint8_t> saved_mappers = Read(base, menu_helpers::kMapperEntries, 32);
  const std::vector<uint8_t> saved_controllers = Read(base, menu_helpers::kControllerEntries, 16);
  constexpr uint32_t kFades = mode_tick::kFadeOutDurations, kFadesBytes = 64;
  const std::vector<uint8_t> saved_fades = Read(base, kFades, kFadesBytes);
  for (size_t fi = 0; fi < count; fi++) {
    const Function& f = functions[fi];
    bool& flag = f.type8 ? ITC_FLAG(native_type8_mode) : ITC_FLAG(native_type67_modes);
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    const int cases = f.type8 || f.wide ? 12 : f.kind == kStep ? 24 : 48;
    for (int c = 0; c < cases; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 811 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, movie, kMovieBlockBytes, r);
        WriteRandom(base, nodes.address, nodes.bytes, r);
        WriteRandom(base, misc.address, misc.bytes, r);
        WriteRandom(base, sp - 1024, 1024 + 256, r);
        const uint32_t dialog = world.address;
        guest::StoreU32(base, dialog + 9664, r.Below(2));
        guest::StoreU32(base, dialog + 9668, r.Below(2));
        guest::StoreU32(base, dialog + 9672, r.Below(2));
        if (r.Below(2)) guest::StoreU32(base, dialog + 9608, 0);
        if (r.Below(2)) guest::StoreU32(base, dialog + 9640, 0);
        if (r.Below(2)) guest::StoreU32(base, dialog + 9748, 0);
        // The movie: its lists, scenes, named animations and choice.
        ui_movies::InitAnimationLists(base, movie);
        guest::StoreU32(base, movie + ui_movies::kMovieLive, node(0));
        guest::StoreU32(base, movie + ui_movies::kMovieSaved, node(1));
        guest::StoreU32(base, movie + ui_movies::kMovieFile, node(2));
        guest::StoreU32(base, movie + 40960, r.Below(2));
        static constexpr uint32_t kTags[] = {0x54464D00, 0x50494300, 0x54585400, 0x12345678};
        for (uint32_t i = 0; i < kNodes; i++) {
          guest::StoreU32(base, node(i), kTags[r.Below(4)]);
          guest::StoreU32(base, node(i) + 100, r.Below(4));
          guest::StoreU32(base, node(i) + 104, i < 3 ? r.Below(4) : 0);
          guest::StoreU32(base, node(i) + 108, r.Below(4));
          guest::StoreU32(base, node(i) + 116, 0);  // a PRM's vertex count
          for (uint32_t k = 128; k < 128 + 8 * 12; k += 8) {
            guest::StoreU32(base, node(i) + k, node(3 + r.Below(kNodes - 3)));
          }
        }
        for (uint32_t k = 0; k < 8; k++) {
          for (uint32_t i = 0; i < 64; i += 4) guest::StoreU32(base, animations + 64 * k + i, 0);
          guest::StoreFloat(base, animations + 64 * k + 12, float(r.Below(40)) / 4.0f);
        }
        for (uint32_t k = 0; k < 16; k++) {
          guest::StoreU32(base, movie + 176 + 36 * k + 28, node(0));
          guest::StoreU32(base, movie + 176 + 36 * k + 32, animations + 64 * r.Below(8));
        }
        guest::StoreU32(base, node(0) + 116, r.Below(4));  // the scene's animation index
        for (uint32_t k = r.Below(3); k > 0; k--) {
          ui_movies::Slot slot{};
          slot[0] = r.Below(2);
          slot[1] = 0xFFFFFFFFu;
          static constexpr uint32_t kNamed[] = {1, 2, 5, 6, 0xFFFFFFFFu};
          slot[2] = kNamed[r.Below(5)];
          slot[3] = node(0);
          slot[4] = animations + 64 * r.Below(8);
          slot[5] = std::bit_cast<uint32_t>(float(r.Below(40)) / 4.0f);
          slot[6] = r.Below(2);
          slot[7] = r.Below(2);
          ui_movies::Append(base, movie + (r.Below(2) ? ui_movies::kMovieQueue
                                                      : ui_movies::kMovieLooping),
                            slot);
        }
        // The movie table: movies 16 and 22 (the type 7 first screen's)
        // the movie, among a few others, in a random place.
        const uint32_t pairs = misc.address;
        uint32_t pair_count;
        if (f.wide) {
          // Movies 16, 19, 20 and 22 the movie (20 now and then missing).
          pair_count = 4 + r.Below(3);
          const uint32_t at = r.Below(pair_count - 3);
          const bool no20 = r.Below(4) == 0;
          static constexpr uint32_t kIds[] = {16, 19, 20, 22};
          for (uint32_t i = 0; i < pair_count; i++) {
            const bool ours = i >= at && i < at + 4 && !(no20 && i == at + 2);
            const uint32_t id = ours ? kIds[i - at] : 30 + i;
            guest::StoreU32(base, pairs + 8 * i, id);
            guest::StoreU32(base, pairs + 8 * i + 4, ours ? movie : uint32_t(r.Next()));
          }
        } else {
          pair_count = 2 + r.Below(4);
          const uint32_t at = r.Below(pair_count - 1);
          for (uint32_t i = 0; i < pair_count; i++) {
            const bool ours = i == at || i == at + 1;
            guest::StoreU32(base, pairs + 8 * i, i == at ? 16 : i == at + 1 ? 22 : 30 + i);
            guest::StoreU32(base, pairs + 8 * i + 4, ours ? movie : uint32_t(r.Next()));
          }
        }
        guest::StoreU32(base, ui_movies::kMovieTable, pairs);
        guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
        guest::StoreU32(base, ui_movies::kMovieTable + 8, pair_count);
        if (f.type8) {
          guest::StoreU32(base, kPlayer, uint32_t(r.Next()));
        }
        // The fake sound manager.
        const uint32_t manager = misc.address + 0x400, vtable = misc.address + 0x800;
        guest::StoreU32(base, manager, vtable);
        guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
        guest::StoreU32(base, vtable + 52, 0x820CE9F8);  // slot 13: r4, r5 to +444, +448
        guest::StoreU32(base, menu_sounds::kSoundManager, manager);
        // The pages: the menu at +9668 (as menu_helpers' harness shapes
        // it), the players' mappers, the moved word +9676 and the shown
        // word. The Updates: the step +9652 (none, the three seen, another
        // negative), the queued step +9656, the queued state +9740 and the
        // shown word; movie 16 at +9744.
        WriteRandom(base, mappers.address, mappers.bytes, r);
        for (uint32_t k = 0; k < 4; k++) {
          const uint32_t vt = mappers.address + 0x100 + 0x40 * k;
          const uint32_t mapper = mappers.address + 0x200 + 0x200 * k;
          guest::StoreU32(base, menu_helpers::kControllerEntries + 4 * k, r.Below(4));
          guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k, mapper);
          guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k + 4,
                          r.Below(5) == 0 ? 1 : 0);
          guest::StoreU32(base, mapper, vt);
          guest::StoreU32(base, vt + menu_helpers::kPressedOffset, menu_helpers::kPressedSlot);
          guest::StoreU32(base, vt + menu_helpers::kFiredOffset, menu_helpers::kFiredSlot);
          for (uint32_t i = 0; i < 6; i++) {
            const uint32_t bits = r.Below(3) == 0 ? 0 : uint32_t(r.Next() & r.Next());
            guest::StoreU32(base, mapper + 448 + 4 * i, bits);
          }
        }
        const uint32_t menu = dialog + 9668;
        guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
        guest::StoreU32(base, menu + menu_helpers::kFirst, 0);
        guest::StoreU32(base, menu + menu_helpers::kLast, 1);
        guest::StoreU32(base, menu + menu_helpers::kCursor, r.Below(2));
        for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                                 menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
          guest::StoreU32(base, menu + o, r.Below(2));
        }
        guest::StoreU32(base, menu + menu_helpers::kMoved, 0);
        guest::StoreU32(base, menu + menu_helpers::kSide, 0);
        guest::StoreU32(base, dialog + 9676, r.Below(2));
        static constexpr uint32_t kShown[] = {4, 1, 5, 3};
        guest::StoreU32(base, dialog + 9756, kShown[r.Below(4)]);
        guest::StoreU32(base, dialog + 9760, kShown[r.Below(4)]);
        if (f.kind == kDraw) {
          // The page: none, ConfirmPage or NoticePage.
          static constexpr uint32_t kPages[] = {0xFFFFFFFFu, 0, 1};
          guest::StoreU32(base, dialog + 9640, kPages[r.Below(3)]);
        }
        if (f.kind == kUpdate && f.type8) {
          // Type 8's: its five steps (the notice's too), none, another
          // negative; movie 16 or 19.
          static constexpr uint32_t kSteps8[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1, 2, 3, 4,
                                                 0xFFFFFFFEu};
          guest::StoreU32(base, dialog + 9652, kSteps8[r.Below(8)]);
          guest::StoreU32(base, dialog + 9656, r.Below(2) ? 0xFFFFFFFFu : r.Below(5));
          static constexpr uint32_t kStates[] = {21, 21, 9, 4, 5, 6};
          guest::StoreU32(base, dialog + 9740, kStates[r.Below(6)]);
          guest::StoreU32(base, dialog + 9744, r.Below(2) ? 16 : 19);
        } else if (f.kind == kUpdate) {
          static constexpr uint32_t kStepsSeen[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1, 4,
                                                    0xFFFFFFFEu};
          guest::StoreU32(base, dialog + 9652, kStepsSeen[r.Below(6)]);
          guest::StoreU32(base, dialog + 9656, r.Below(2) ? 0xFFFFFFFFu : r.Below(5));
          static constexpr uint32_t kStates[] = {21, 21, 9, 4, 5, 6};
          guest::StoreU32(base, dialog + 9740, kStates[r.Below(6)]);
          guest::StoreU32(base, dialog + 9744, 16);
        }
        // The type 7 first screen's: its page state (+9648) and menu
        // (+9652) blocks, the step's started word and the item; the screen
        // fades' words (their counts mostly 0).
        if (f.kind == kScreen7 || f.kind == kScreen6) {
          // Type 6's: the page state at +9644 and the menu at +9648, five
          // items.
          const uint32_t d = f.kind == kScreen7 ? 4 : 0;
          const uint32_t page = world.address + 0x3800, menu7 = world.address + 0x3900;
          guest::StoreU32(base, dialog + 9644 + d, page);
          guest::StoreU32(base, dialog + 9648 + d, menu7);
          guest::StoreU32(base, page + 24, r.Below(2));
          guest::StoreU32(base, menu7, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(d ? 5 : 6));
        }
        WriteRandom(base, kFades, kFadesBytes, r);
        guest::StoreU32(base, mode_tick::kFadeOutCounts, r.Below(3) == 0 ? r.Below(40) : 0);
        guest::StoreU32(base, mode_tick::kFadeInCounts, r.Below(3) == 0 ? r.Below(40) : 0);
        guest::StoreU32(base, dispatcher + 204 + 600, r.Below(50));
        guest::StoreU32(base, dispatcher + 204 + 604, r.Below(60));
        PPCContext t = CallContext(h);
        t.r3.u64 = dialog | (uint64_t(r.Below(2)) << 32);
        t.r4.u64 = dispatcher;
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        g_dialog_answers = Random(r.Next());
        std::vector<DialogCall> calls;
        g_dialog_calls = &calls;
        const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
            ui_movies_hooks::g_update_callee_stand_in;
        ui_movies_hooks::g_update_callee_stand_in = DialogStandIn;
        if (v == 0) {
          flag = false;
          if (f.kind == kUpdate || f.kind == kDraw) ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
          flag = true;
        } else if (f.kind == kUpdate || f.kind == kDraw) {
          // The steps (pages) the Update (Draw) calls through its table
          // recompiled on both sides (their own cases are above).
          ITC_FLAG(native_type67_modes) = false;
          ITC_FLAG(native_type8_mode) = false;
          f.native(t, base);
          ITC_FLAG(native_type8_mode) = true;
          ITC_FLAG(native_type67_modes) = true;
        } else {
          f.native(t, base);
        }
        ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
        g_dialog_calls = nullptr;
        out[v].Word(uint32_t(calls.size()));
        for (const DialogCall& call : calls) {
          for (const uint32_t w : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                   uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                   uint32_t(call.r4 >> 32)}) {
            out[v].Word(w);
          }
        }
        out[v].Bytes(Read(base, world.address, world.bytes));
        out[v].Bytes(Read(base, movie, kMovieBlockBytes));
        out[v].Bytes(Read(base, nodes.address, nodes.bytes));
        out[v].Bytes(Read(base, misc.address, misc.bytes));
        out[v].Bytes(Read(base, mappers.address, mappers.bytes));
        out[v].Bytes(Read(base, menu_helpers::kMapperEntries, 32));
        out[v].Bytes(Read(base, menu_helpers::kControllerEntries, 16));
        out[v].Bytes(Read(base, kFades, kFadesBytes));
        if (f.type8) out[v].Word(guest::LoadU32(base, kPlayer));
        if (f.kind == kUpdate || f.kind == kDraw) {
          // But the step's register saves (40 bytes below the Update's
          // frame): the Update's registers there, the native one's not.
          out[v].Bytes(Read(base, sp - 1024, 1024 - f.frame - 40));
        } else {
          out[v].Bytes(Read(base, sp - 1024, 1024 - f.frame));
        }
        out[v].Bytes(Read(base, sp - f.frame + 4, f.frame - 4 - f.saves));
        out[v].Bytes(Read(base, sp, 256));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Write(base, ui_movies::kMovieTable, table.data(), 12);
  guest::StoreU32(base, menu_sounds::kSoundManager, saved_manager);
  Write(base, menu_helpers::kMapperEntries, saved_mappers.data(), 32);
  Write(base, menu_helpers::kControllerEntries, saved_controllers.data(), 16);
  Write(base, kFades, saved_fades.data(), kFadesBytes);
  guest::StoreU32(base, kPlayer, saved_player);
}

// --- The first screens' confirms, Type6Screen0::ShowPage and the type 7
// first screen's ShowPage, PlaceCursor and ShowHelp: one harness movie as
// both 16 and 22 in a harness movie table; its live scene a node whose
// child 0 is a TXT node, child 1 a sub-scene with TXT children 0, 3 and 4,
// children 2 and 3 a sub-scene whose child 0 is a node and children 4 to 14
// are tagged PIC, PRM, TXT or nothing; the TXT nodes' text ids 0 to 14 (0: the movie's +0x9FCC plus
// one), ids 1 to 14 in the movie's texts' map with records of a texts
// block (and the word at 0x826C5CD8 another, so none is made); the string
// table (0x826C5C34) one container of strings of letters only (no argument
// codes: the user's call, 2026-10-30), as the location screen's harness
// builds it. The dispatcher at the world's +0x3200 (its ring's count and
// start small). The world, the movie, the nodes, the texts, the stack below
// the call (but the original's back chain, saved registers and SetText's
// unset varargs' words, its frame's +48 to +79) and r3 compared. ---

void BuildLetterStrings(uint8_t* base, uint32_t at, uint32_t bytes) {
  constexpr uint32_t kBounds = 0x82007E18;
  uint32_t span = 1;
  for (uint32_t i = 0; i < 17; i++) {
    const int32_t from = int32_t(guest::LoadU32(base, kBounds + 4 * i));
    const int32_t to = int32_t(guest::LoadU32(base, kBounds + 4 * i + 4));
    if (to > from && uint32_t(to - from) > span) span = uint32_t(to - from);
  }
  const uint32_t container = at + 0x100;
  if (container + 4 + 4 * span + 8 * 64 > at + bytes) {
    span = (bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) guest::StoreU32(base, at + 4 * i, container);
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x5479706536ull);
  const uint32_t text = container + 4 + 4 * span;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t length = r.Below(30);
    for (uint32_t c = 0; c < length; c++) {
      guest::StoreU16(base, text + 64 * k + 2 * c, uint16_t('A' + r.Below(26)));
    }
    guest::StoreU16(base, text + 64 * k + 2 * length, 0);
  }
  for (uint32_t i = 0; i < span; i++) {
    guest::StoreU32(base, container + 4 + 4 * i,
                    __builtin_bswap32(text + 64 * r.Below(8) - container));
  }
}

void Screen0Helpers(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  if (!hooks::Type67TextsNative(base)) {
    ITC_LOG_ERROR("harness: type67_modes: the UI movies' text setters aren't native; confirms skipped");
    return;
  }
  constexpr uint32_t kMovieBlockBytes = 0xA100, kNodeBytes = 0x200, kNodes = 16;
  constexpr uint32_t kStringTable = 0x826C5C34, kMissingText = 0x826C5CD8;
  constexpr uint32_t kStringsBytes = 0x10000, kTextsBytes = 0x11000;
  const Block movie_block(kMovieBlockBytes);
  const Block nodes(kNodeBytes * kNodes);
  const Block texts(kTextsBytes);
  const Block strings(kStringsBytes);
  const Block pairs(0x100);
  if (!movie_block.address || !nodes.address || !texts.address || !strings.address ||
      !pairs.address) {
    return;
  }
  const uint32_t movie = movie_block.address;
  auto node = [&](uint32_t i) { return nodes.address + kNodeBytes * i; };
  const uint32_t sp = CallContext(h).r1.u32;
  const std::vector<uint8_t> table = Read(base, ui_movies::kMovieTable, 12);
  const uint32_t saved_strings = guest::LoadU32(base, kStringTable);
  const uint32_t saved_missing = guest::LoadU32(base, kMissingText);
  struct Function {
    const char* name;
    uint32_t address;
    bool confirm;
    GuestFunction recompiled, native;
    uint32_t frame = 112, saves = 24;
    // The dialogs' closes: the asked word, the pick and page blocks shaped,
    // r4 the dispatcher; the original run with the flag on (its callees,
    // ShowPage and ConfirmAgain, native on both sides; their own cases are
    // above).
    int closed = 0;  // 6 or 7
    // Type7Screen0::MenuPage: the menu (+9652, menu_helpers' harness's
    // shape) and page (+9648) blocks, the players' mappers (action 163's
    // pressed bit clear: its branch's callee, the power graph's set-up, is
    // never seen in play and not laid here), the fake sound manager; the
    // original run with the flag on as the closes are.
    bool menu = false;
    // The first screens' Show (1) and ShowCarInfo (2): the car world
    // (LayShowCarWorld's, the mode's car its garage car, a save data's
    // garage list to it as Cars' cases lay it, the parameters' table at the
    // spec for the stock car 0 where an empty garage makes it), the live
    // scene's children 4 (the menu's sub-scene) and 10 to 14 (the car's
    // texts) laid, the TXT nodes' text ids small; the screen's loading
    // word, page and menu blocks. `below`: the callee's register saves under
    // the frame (ShowCarInfo's under Show's), left out; since 2026-11-13
    // also the recompiled callees' saves under a caller's frame where those
    // callees have no adapters (ShowPage, the confirms, PlaceCursor,
    // ShowHelp, PlayItem: the original runs them, the native their plain
    // C++).
    int show = 0;
    bool type7 = false;
    uint32_t below = 0;
    // The first screens' steps, PlayItem and the pages without a world of
    // their own above: kAnimated adds movie 22's animations (its lists,
    // named animations 0 to 48 over the live scene, slots queued and
    // looping, UiMovie::Run's update and ApplyAnimation recorded through the
    // dialogs' stand-in) and the screen fades' words (counts mostly 0), the
    // page state (+9644, type 7 +9648; its started word 0 or 1) and the menu
    // (+9648, +9652; the item and the previous one 0 to 5, a wrap now and
    // then); kQuit the save screen's data (+56 0, 1 or another); kGraph the
    // pick and page blocks as the closes' (the pick 2 or not) with the car
    // list's graph not shown ([0x82815EC0] 0: CarList::DrawCarPowerGraph
    // does nothing). The type 6 help's text node (sub-scene 2, child 0: node
    // 14) a TXT node for kAnimated and `help6`.
    enum Extra { kNone, kAnimated, kQuit, kGraph };
    Extra extra = kNone;
    bool help6 = false;
  };
  const Function functions[] = {
      {"Type6Screen0::Confirm", 0x82233FC8, true, __imp__sub_82233FC8,
       hooks::Type6Screen0Confirm},
      {"Type6Screen0::ConfirmAgain", 0x822340B0, true, __imp__sub_822340B0,
       hooks::Type6Screen0ConfirmAgain},
      {"Type7Screen0::Confirm", 0x82244528, true, __imp__sub_82244528,
       hooks::Type7Screen0Confirm},
      {"Type6Screen0::ShowPage", 0x822344A8, false, __imp__sub_822344A8,
       hooks::Type6Screen0ShowPage},
      {"Type7Screen0::ShowPage", 0x82244930, false, __imp__sub_82244930,
       hooks::Type7Screen0ShowPage},
      {"Type7Screen0::PlaceCursor", 0x822448B0, false, __imp__sub_822448B0,
       hooks::Type7Screen0PlaceCursor},
      {"Type7Screen0::ShowHelp", 0x82244AB8, false, __imp__sub_82244AB8,
       hooks::Type7Screen0ShowHelp, 128, 32},
      {"Type6Screen0::OnDialogClosed", 0x82234768, true, __imp__sub_82234768,
       hooks::Type6Screen0OnDialogClosed, 96, 16, 6, false, 0, false, 24},
      // A leaf; its tail call's (ShowPage's) frame compared.
      {"Type7Screen0::OnDialogClosed", 0x82244B60, false, __imp__sub_82244B60,
       hooks::Type7Screen0OnDialogClosed, 112, 24, 7},
      {"Type7Screen0::MenuPage", 0x82244BF8, true, __imp__sub_82244BF8,
       hooks::Type7Screen0MenuPage, 112, 24, 0, true, 0, true, 24},
      {"Type6Screen0::MenuPage", 0x82234958, true, __imp__sub_82234958,
       hooks::Type6Screen0MenuPage, 112, 24, 0, true, 0, false, 24},
      {"Type6Screen0::PlaceCursor", 0x82234428, false, __imp__sub_82234428,
       hooks::Type6Screen0PlaceCursor},
      {"Type6Screen0::ShowHelp", 0x822346A8, false, __imp__sub_822346A8,
       hooks::Type6Screen0ShowHelp, 128, 32, 0, false, 0, false, 0, Function::kNone, true},
      {"Type6Screen0::PlayItem", 0x822343A8, false, __imp__sub_822343A8,
       hooks::Type6Screen0PlayItem, 112, 24, 0, false, 0, false, 0, Function::kAnimated},
      {"Type7Screen0::PlayItem", 0x82244818, false, __imp__sub_82244818,
       hooks::Type7Screen0PlayItem, 112, 24, 0, false, 0, true, 0, Function::kAnimated},
      {"Type6Screen0::OpeningStep", 0x82234C20, false, __imp__sub_82234C20,
       hooks::Type6Screen0OpeningStep, 128, 32, 0, false, 0, false, 32, Function::kAnimated},
      {"Type7Screen0::OpeningStep", 0x82244E70, false, __imp__sub_82244E70,
       hooks::Type7Screen0OpeningStep, 128, 32, 0, false, 0, true, 32, Function::kAnimated},
      {"Type6Screen0::BackStep", 0x82234E38, false, __imp__sub_82234E38,
       hooks::Type6Screen0BackStep, 128, 32, 0, false, 0, false, 32, Function::kAnimated},
      {"Type7Screen0::BackStep", 0x822450A8, false, __imp__sub_822450A8,
       hooks::Type7Screen0BackStep, 128, 32, 0, false, 0, true, 32, Function::kAnimated},
      {"Type6Screen0::CursorStep", 0x82234D50, false, __imp__sub_82234D50,
       hooks::Type6Screen0CursorStep, 144, 48, 0, false, 0, false, 32, Function::kAnimated},
      {"Type7Screen0::CursorStep", 0x82244FC0, false, __imp__sub_82244FC0,
       hooks::Type7Screen0CursorStep, 144, 48, 0, false, 0, true, 32, Function::kAnimated},
      // A leaf with tail calls; ShowPage's frame compared.
      {"Type6Screen0::QuitPage", 0x82234BF0, false, __imp__sub_82234BF0,
       hooks::Type6Screen0QuitPage, 112, 24, 0, false, 0, false, 0, Function::kQuit},
      {"Type6Screen0::PowerGraphPage", 0x82234B80, false, __imp__sub_82234B80,
       hooks::Type6Screen0PowerGraphPage, 96, 16, 0, false, 0, false, 24, Function::kGraph},
      {"Type7Screen0::PowerGraphPage", 0x82244E00, false, __imp__sub_82244E00,
       hooks::Type7Screen0PowerGraphPage, 96, 16, 0, false, 0, true, 24, Function::kGraph},
      {"Type6Screen0::ShowCarInfo", 0x82233B70, false, __imp__sub_82233B70,
       hooks::Type6Screen0ShowCarInfo, 4064, 32, 0, false, 2, false},
      {"Type7Screen0::ShowCarInfo", 0x822440D8, false, __imp__sub_822440D8,
       hooks::Type7Screen0ShowCarInfo, 4048, 32, 0, false, 2, true},
      {"Type6Screen0::Show", 0x82233C80, false, __imp__sub_82233C80, hooks::Type6Screen0Show,
       128, 48, 0, false, 1, false, 32},
      {"Type7Screen0::Show", 0x822441D8, false, __imp__sub_822441D8, hooks::Type7Screen0Show,
       128, 48, 0, false, 1, true, 32},
  };
  // The show cases' car block and save data, and the globals they point.
  constexpr uint32_t kCarBlockBytes = 0x10000, kSaveBytes = 0x1A000, kCarBytes = 1984;
  constexpr uint32_t kSavePointer = 0x826C5B58;
  const Block car_block(kCarBlockBytes);
  const Block save_block(kSaveBytes);
  if (!car_block.address || !save_block.address) return;
  const uint32_t saved_save = guest::LoadU32(base, kSavePointer);
  const uint32_t saved_seed = guest::LoadU32(base, race_setup::kRandSeed);
  const uint32_t saved_params = guest::LoadU32(base, car_specs::kParams);
  const std::vector<uint8_t> saved_cars[2] = {Read(base, t67::kType6Car, kCarBytes),
                                              Read(base, t67::kType7Car, kCarBytes)};
  const Block mappers(0x1000);
  const Block manager_block(0x1000);  // the fake manager (+0), its vtable (+0x800)
  if (!mappers.address || !manager_block.address) return;
  const uint32_t saved_manager = guest::LoadU32(base, menu_sounds::kSoundManager);
  const std::vector<uint8_t> saved_mappers = Read(base, menu_helpers::kMapperEntries, 32);
  const std::vector<uint8_t> saved_controllers = Read(base, menu_helpers::kControllerEntries, 16);
  constexpr uint32_t kTagTxt = 0x54585400;
  static constexpr uint32_t kTags[] = {0x50494300, 0x50524D00, kTagTxt, 0x12345678};
  // The steps' animations (8 of 64 bytes), the screen fades' words, the save
  // screen's data's global and the car list graph's shown word.
  const Block anims(8 * 64);
  if (!anims.address) return;
  constexpr uint32_t kFades = mode_tick::kFadeOutDurations, kFadesBytes = 64;
  constexpr uint32_t kSaveScreenData = 0x826D8454, kGraphShown = 0x82815EC0;
  const std::vector<uint8_t> saved_fades = Read(base, kFades, kFadesBytes);
  const uint32_t saved_save_screen = guest::LoadU32(base, kSaveScreenData);
  const uint32_t saved_graph_shown = guest::LoadU32(base, kGraphShown);
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    const int cases = f.menu ? 96
                      : f.closed != 0 || f.show != 0 || f.extra == Function::kAnimated ? 48
                      : f.extra != Function::kNone                                    ? 24
                                                                                      : 16;
    for (int c = 0; c < cases; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 977 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, movie, kMovieBlockBytes, r);
        WriteRandom(base, nodes.address, nodes.bytes, r);
        WriteRandom(base, texts.address, 0x1000, r);
        WriteRandom(base, sp - 1024, 1024 + 256, r);
        BuildLetterStrings(base, strings.address, kStringsBytes);
        guest::StoreU32(base, kStringTable, strings.address);
        const bool animated = f.extra == Function::kAnimated;
        if (animated) {
          ui_movies::InitAnimationLists(base, movie);  // before the scenes: it clears +0 to +24
        }
        // The scene: node 0 live; its children 0 (TXT node 1), 1 (sub-scene
        // node 2: children 3, 4 TXT nodes 3, 4) and 3 (sub-scene node 5:
        // children 4 to 14 nodes 6 to 15 and 3, tagged at random).
        guest::StoreU32(base, movie + ui_movies::kMovieLive, node(0));
        for (const uint32_t scene : {node(0), node(2), node(5)}) {
          guest::StoreU32(base, scene + 108, 0);
        }
        auto child = [&](uint32_t scene, uint32_t i, uint32_t to) {
          guest::StoreU32(base, scene + 128 + 8 * i, to);
        };
        child(node(0), 0, node(1));
        child(node(0), 1, node(2));
        child(node(0), 2, node(5));
        child(node(0), 3, node(5));
        child(node(0), 4, node(5));  // Type6Screen0::PlaceCursor's (-1, 4)
        child(node(2), 0, node(1));
        child(node(2), 3, node(3));
        child(node(2), 4, node(4));
        child(node(5), 0, node(14));
        for (uint32_t i = 4; i <= 14; i++) child(node(5), i, i == 14 ? node(3) : node(2 + i));
        for (uint32_t i = 6; i < kNodes; i++) guest::StoreU32(base, node(i), kTags[r.Below(4)]);
        for (const uint32_t t : {1u, 3u, 4u}) {
          guest::StoreU32(base, node(t), kTagTxt);
          guest::StoreU32(base, node(t) + 108, r.Below(15));
        }
        guest::StoreU32(base, movie + 0x9FCC, r.Below(14));
        // The texts: ids 1 to 14 with records (+0 a 4 KB buffer), the
        // missing id's word another record.
        for (uint32_t k = 0; k < 15; k++) {
          const uint32_t record = texts.address + 32 * k;
          guest::StoreU32(base, record, texts.address + 0x1000 + 0x1000 * k);
          if (k < 14) {
            guest::StoreU32(base, movie + 0x9BCC + 8 * k, k + 1);
            guest::StoreU32(base, movie + 0x9BCC + 8 * k + 4, record);
          }
        }
        guest::StoreU32(base, movie + 0x9BCC + 1024, 14);
        guest::StoreU32(base, kMissingText, texts.address + 32 * 14);
        // The movie table: 16 and 22 the movie, among others.
        const uint32_t count = 2 + r.Below(4);
        const uint32_t first = r.Below(count - 1);
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t id = i == first ? 16 : i == first + 1 ? 22 : 30 + i;
          guest::StoreU32(base, pairs.address + 8 * i, id);
          guest::StoreU32(base, pairs.address + 8 * i + 4,
                          id == 16 || id == 22 ? movie : uint32_t(r.Next()));
        }
        guest::StoreU32(base, ui_movies::kMovieTable, pairs.address);
        guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
        guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
        guest::StoreU32(base, world.address + 0x3200 + 204 + 600, r.Below(50));
        guest::StoreU32(base, world.address + 0x3200 + 204 + 604, r.Below(60));
        if (f.closed != 0) {
          // The pick's block (+72) and the page block, at +9640 and +9644
          // (type 7: +9644 and +9648); type 6's asked word 4, 5 or another.
          const uint32_t d = f.closed == 7 ? 4 : 0;
          const uint32_t picks = world.address + 0x3900, page = world.address + 0x3A00;
          guest::StoreU32(base, world.address + 9640 + d, picks);
          guest::StoreU32(base, world.address + 9644 + d, page);
          static constexpr uint32_t kPicks[] = {0, 1, 2, 3, 0xFFFFFFFFu};
          guest::StoreU32(base, picks + 72, kPicks[r.Below(5)]);
          static constexpr uint32_t kPages[] = {0, 1, 2, 3, 0xFFFFFFFFu};
          guest::StoreU32(base, page, kPages[r.Below(5)]);
          static constexpr uint32_t kAsked[] = {4, 4, 5, 5, 0xFFFFFFFFu};
          guest::StoreU32(base, world.address + 9664, kAsked[r.Below(5)]);
        }
        if (f.show != 0) {
          // The callees' frames below (ShowCarInfo's car object).
          WriteRandom(base, sp - 9216, 9216 - 1024, r);
          // UiMovie::ResetPose: the file and saved scenes the live one, its
          // children 0 to 4 at most walked (a TXT node and untagged ones).
          guest::StoreU32(base, movie + ui_movies::kMovieFile, node(0));
          guest::StoreU32(base, movie + ui_movies::kMovieSaved, node(0));
          guest::StoreU32(base, node(0) + 100, 0);
          guest::StoreU32(base, node(0) + 104, r.Below(5));
          guest::StoreU32(base, race_setup::kRandSeed, uint32_t(r.Next()));
          guest::StoreU32(base, node(0) + 128 + 8 * 4, node(5));
          for (uint32_t k = 0; k < 5; k++) {
            guest::StoreU32(base, node(0) + 128 + 8 * (10 + k), node(6 + 2 * k));
          }
          // The texts' nodes TXT (SetText's original loops forever on another).
          for (uint32_t i = 6; i < kNodes; i++) {
            if (i <= 14) guest::StoreU32(base, node(i), kTagTxt);
            guest::StoreU32(base, node(i) + 108, r.Below(15));
          }
          const uint32_t cars = car_block.address, save = save_block.address;
          const StandWorld stand = LayShowCarWorld(base, cars, r, c, cars + 0xCE00);
          const uint32_t garage_car = cars + 0x4000, spec = cars + 0xA000;
          const uint32_t car = f.type7 ? t67::kType7Car : t67::kType6Car;
          WriteRandom(base, car, kCarBytes, r);
          Write(base, car, Read(base, garage_car, 1884).data(), 1884);
          static constexpr uint32_t kFrom[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 2};
          const uint32_t from = kFrom[r.Below(4)];
          guest::StoreU32(base, car + 1972, from);
          WriteRandom(base, save, 0x400, r);
          WriteRandom(base, save + 6120, 0x400, r);
          const uint32_t nodes_at = save + 6120;
          const uint32_t steps = r.Below(4);
          guest::StoreU32(base, save + 0x19920, steps);
          uint16_t at = uint16_t(r.Below(10));
          guest::StoreU16(base, nodes_at + 204, at);
          for (uint32_t i = 0; i < steps; i++) {
            const uint16_t next = uint16_t(10 * (i + 1) + r.Below(10));
            guest::StoreU16(base, nodes_at + 4 * at, next);
            at = next;
          }
          const uint32_t slot = r.Below(3);
          guest::StoreU16(base, nodes_at + 4 * at + 2, uint16_t(slot));
          WriteRandom(base, nodes_at + 212 + 1968 * slot, 1968, r);
          Write(base, nodes_at + 212 + 1968 * slot, Read(base, garage_car, 1884).data(), 1884);
          const bool empty = r.Below(3) == 0;
          guest::StoreU16(base, save + 6328, empty ? 0 : uint16_t(1 + r.Below(20)));
          guest::StoreU32(base, kSavePointer, save);
          guest::StoreU32(base, car_specs::kParams, stand.params);
          // The stock car 0 an empty garage makes: its spec the case's.
          if (f.show == 1 && from == 0xFFFFFFFFu && empty) {
            guest::StoreU32(base, stand.params + 12, spec);
          }
          const uint32_t d = f.type7 ? 4 : 0;
          guest::StoreU32(base, world.address + 9640 + d, world.address + 0x3800);
          guest::StoreU32(base, world.address + 9644 + d, world.address + 0x3A00);
          guest::StoreU32(base, world.address + 9648 + d, world.address + 0x3900);
          static constexpr uint32_t kPages[] = {0, 1, 2, 3, 0xFFFFFFFFu};
          guest::StoreU32(base, world.address + 0x3A00, kPages[r.Below(5)]);
        }
        if (f.menu) {
          // Type 6's: the page state at +9644, the menu at +9648, five items.
          const uint32_t menu = world.address + 0x3900, page = world.address + 0x3A00;
          const uint32_t md = f.type7 ? 4 : 0, quit = f.type7 ? 3 : 4;
          guest::StoreU32(base, world.address + 9644 + md, page);
          guest::StoreU32(base, world.address + 9648 + md, menu);
          static constexpr uint32_t kPages[] = {0, 1, 2, 3, 0xFFFFFFFFu};
          guest::StoreU32(base, page, kPages[r.Below(5)]);
          WriteRandom(base, mappers.address, mappers.bytes, r);
          // Random reads, or no move and accept (159), back (158) or neither.
          const uint32_t press = r.Below(4);
          for (uint32_t k = 0; k < 4; k++) {
            const uint32_t vt = mappers.address + 0x100 + 0x40 * k;
            const uint32_t mapper = mappers.address + 0x200 + 0x200 * k;
            guest::StoreU32(base, menu_helpers::kControllerEntries + 4 * k, r.Below(4));
            guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k, mapper);
            guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k + 4,
                            r.Below(5) == 0 ? 1 : 0);
            guest::StoreU32(base, mapper, vt);
            guest::StoreU32(base, vt + menu_helpers::kPressedOffset, menu_helpers::kPressedSlot);
            guest::StoreU32(base, vt + menu_helpers::kFiredOffset, menu_helpers::kFiredSlot);
            for (uint32_t i = 0; i < 6; i++) {
              const uint32_t bits = r.Below(3) == 0 ? 0 : uint32_t(r.Next() & r.Next());
              guest::StoreU32(base, mapper + 448 + 4 * i, bits);
            }
            // No move (Menu::Step's actions 153 to 156, 161, 162 and 167 to
            // 170 neither pressed nor fired), then 159 (index 8) and 158
            // (index 7) as picked.
            if (press != 0) {
              constexpr uint32_t kMoves = 0xF003C | (3u << 10);
              for (const uint32_t word : {456u, 464u}) {
                guest::StoreU32(base, mapper + word, guest::LoadU32(base, mapper + word) & ~kMoves);
              }
              const uint32_t pressed = guest::LoadU32(base, mapper + 456) & ~(3u << 7);
              guest::StoreU32(base, mapper + 456,
                              pressed | (press == 1 ? 1u << 8 : press == 2 ? 1u << 7 : 0));
            }
            // Action 163 (index 12) never pressed.
            guest::StoreU32(base, mapper + 456, guest::LoadU32(base, mapper + 456) & ~(1u << 12));
          }
          guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
          guest::StoreU32(base, menu + menu_helpers::kFirst, 0);
          guest::StoreU32(base, menu + menu_helpers::kLast, quit);
          guest::StoreU32(base, menu + menu_helpers::kCursor, r.Below(2) ? quit : r.Below(quit + 2));
          for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                                   menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
            guest::StoreU32(base, menu + o, r.Below(2));
          }
          guest::StoreU32(base, menu + menu_helpers::kMoved, 0);
          guest::StoreU32(base, menu + menu_helpers::kSide, 0);
          WriteRandom(base, manager_block.address, manager_block.bytes, r);
          const uint32_t manager = manager_block.address, vtable = manager + 0x800;
          guest::StoreU32(base, manager, vtable);
          guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
          guest::StoreU32(base, menu_sounds::kSoundManager, manager);
        }
        if (animated || f.help6) {
          guest::StoreU32(base, node(14), kTagTxt);
          guest::StoreU32(base, node(14) + 108, r.Below(15));
        }
        const uint32_t d = f.type7 ? 4 : 0;
        if (animated) {
          for (uint32_t k = 0; k < 8; k++) {
            for (uint32_t i = 0; i < 64; i += 4) guest::StoreU32(base, anims.address + 64 * k + i, 0);
            guest::StoreFloat(base, anims.address + 64 * k + 12, float(r.Below(40)) / 4.0f);
          }
          for (uint32_t k = 0; k < 49; k++) {
            guest::StoreU32(base, movie + 176 + 36 * k + 28, node(0));
            guest::StoreU32(base, movie + 176 + 36 * k + 32, anims.address + 64 * r.Below(8));
          }
          guest::StoreU32(base, node(0) + 116, r.Below(4));
          for (uint32_t k = r.Below(4); k > 0; k--) {
            ui_movies::Slot slot{};
            slot[0] = r.Below(2);
            slot[1] = 0xFFFFFFFFu;
            static constexpr uint32_t kNamed[] = {5, 6, 7, 9, 15, 16, 43, 48, 0, 0xFFFFFFFFu};
            slot[2] = kNamed[r.Below(10)];
            slot[3] = node(0);
            slot[4] = anims.address + 64 * r.Below(8);
            slot[5] = std::bit_cast<uint32_t>(float(r.Below(40)) / 4.0f);
            slot[6] = r.Below(2);
            slot[7] = r.Below(2);
            ui_movies::Append(base, movie + (r.Below(2) ? ui_movies::kMovieQueue
                                                        : ui_movies::kMovieLooping),
                              slot);
          }
          WriteRandom(base, kFades, kFadesBytes, r);
          guest::StoreU32(base, mode_tick::kFadeOutCounts, r.Below(3) == 0 ? r.Below(40) : 0);
          guest::StoreU32(base, mode_tick::kFadeInCounts, r.Below(3) == 0 ? r.Below(40) : 0);
          // The page state and the menu: the item and the previous one in
          // the menu's range (type 6: 0 to 4, type 7: 0 to 3) or one past.
          const uint32_t page = world.address + 0x3A00, menu = world.address + 0x3900;
          guest::StoreU32(base, world.address + 9644 + d, page);
          guest::StoreU32(base, world.address + 9648 + d, menu);
          guest::StoreU32(base, page + 24, r.Below(2));
          const uint32_t items = f.type7 ? 5 : 6;
          const uint32_t item = r.Below(items);
          uint32_t old = item;
          switch (r.Below(4)) {
            case 0: old = r.Below(items); break;  // a wrap, mostly
            case 1: old = item + 1 < items ? item + 1 : item; break;
            case 2: old = item > 0 ? item - 1 : item; break;
            default: break;
          }
          guest::StoreU32(base, menu + 0, item);
          guest::StoreU32(base, menu + 4, old);
        }
        if (f.extra == Function::kQuit) {
          const uint32_t data = world.address + 0x3B00;
          guest::StoreU32(base, kSaveScreenData, data);
          static constexpr uint32_t kWords[] = {0, 1, 2, 0xFFFFFFFFu};
          guest::StoreU32(base, data + 56, kWords[r.Below(4)]);
        }
        if (f.extra == Function::kGraph) {
          const uint32_t picks = world.address + 0x3900, page = world.address + 0x3A00;
          guest::StoreU32(base, world.address + 9640 + d, picks);
          guest::StoreU32(base, world.address + 9644 + d, page);
          static constexpr uint32_t kPicks[] = {2, 2, 0, 1, 0xFFFFFFFFu};
          guest::StoreU32(base, picks + 72, kPicks[r.Below(5)]);
          static constexpr uint32_t kPages[] = {0, 1, 2, 3, 0xFFFFFFFFu};
          guest::StoreU32(base, page, kPages[r.Below(5)]);
          guest::StoreU32(base, kGraphShown, 0);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = f.closed != 0 || f.menu || f.show == 1 || animated ||
                           f.extra == Function::kGraph
                       ? world.address | (uint64_t(r.Below(2)) << 32)
                       : uint32_t(r.Next());
        t.r4.u64 = f.confirm ? world.address + 0x3200
                             : (r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(5));
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        std::vector<DialogCall> calls;
        const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
            ui_movies_hooks::g_update_callee_stand_in;
        if (animated) {
          g_dialog_answers = Random(r.Next());
          g_dialog_calls = &calls;
          ui_movies_hooks::g_update_callee_stand_in = DialogStandIn;
        }
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = f.closed != 0 || f.menu || f.extra != Function::kNone;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          f.native(t, base);
        }
        if (animated) {
          ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
          g_dialog_calls = nullptr;
          out[v].Word(uint32_t(calls.size()));
          for (const DialogCall& call : calls) {
            for (const uint32_t w : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                     uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                     uint32_t(call.r4 >> 32)}) {
              out[v].Word(w);
            }
          }
          out[v].Bytes(Read(base, anims.address, anims.bytes));
          out[v].Bytes(Read(base, kFades, kFadesBytes));
        }
        if (f.extra == Function::kQuit || f.extra == Function::kGraph) {
          out[v].Word(guest::LoadU32(base, kSaveScreenData));
          out[v].Word(guest::LoadU32(base, kGraphShown));
        }
        out[v].Bytes(Read(base, world.address, world.bytes));
        out[v].Bytes(Read(base, movie, kMovieBlockBytes));
        out[v].Bytes(Read(base, nodes.address, nodes.bytes));
        out[v].Bytes(Read(base, texts.address, kTextsBytes));
        if (f.menu) {
          out[v].Bytes(Read(base, mappers.address, mappers.bytes));
          out[v].Bytes(Read(base, manager_block.address, manager_block.bytes));
          out[v].Bytes(Read(base, menu_helpers::kMapperEntries, 32));
          out[v].Bytes(Read(base, menu_helpers::kControllerEntries, 16));
        }
        if (f.show != 0) {
          out[v].Word(guest::LoadU32(base, race_setup::kRandSeed));
          out[v].Bytes(Read(base, car_block.address, kCarBlockBytes));
          out[v].Bytes(Read(base, save_block.address, kSaveBytes));
          out[v].Bytes(Read(base, t67::kType6Car, kCarBytes));
          out[v].Bytes(Read(base, t67::kType7Car, kCarBytes));
        }
        const uint32_t frame = sp - f.frame;
        if (f.frame <= 1024) {
          // Without the confirm's (MenuPage) or ShowHelp's (the steps)
          // frame's +48 to +79 under this one, when the original runs it:
          // UiMovie::SetText's unset varargs' words (r7 to r10 as the
          // original leaves them, 0 from the plain C++: the user's call,
          // 2026-10-30).
          const uint32_t callee = f.menu                         ? 112
                                  : f.extra == Function::kAnimated ? 128
                                                                   : 0;
          const uint32_t end = frame - f.below;
          if (callee != 0) {
            const uint32_t dead = frame - callee + 48;
            out[v].Bytes(Read(base, sp - 1024, dead - (sp - 1024)));
            out[v].Bytes(Read(base, dead + 32, end - (dead + 32)));
          } else {
            out[v].Bytes(Read(base, sp - 1024, end - (sp - 1024)));
          }
        }
        out[v].Bytes(Read(base, frame + 4, 44));
        out[v].Bytes(Read(base, frame + 80, f.frame - 80 - f.saves));
        out[v].Bytes(Read(base, sp, 256));
        out[v].Word(uint32_t(t.lr));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Write(base, ui_movies::kMovieTable, table.data(), 12);
  guest::StoreU32(base, kStringTable, saved_strings);
  guest::StoreU32(base, kMissingText, saved_missing);
  guest::StoreU32(base, menu_sounds::kSoundManager, saved_manager);
  Write(base, menu_helpers::kMapperEntries, saved_mappers.data(), 32);
  Write(base, menu_helpers::kControllerEntries, saved_controllers.data(), 16);
  guest::StoreU32(base, kSavePointer, saved_save);
  guest::StoreU32(base, race_setup::kRandSeed, saved_seed);
  guest::StoreU32(base, car_specs::kParams, saved_params);
  Write(base, t67::kType6Car, saved_cars[0].data(), kCarBytes);
  Write(base, t67::kType7Car, saved_cars[1].data(), kCarBytes);
  Write(base, kFades, saved_fades.data(), kFadesBytes);
  guest::StoreU32(base, kSaveScreenData, saved_save_screen);
  guest::StoreU32(base, kGraphShown, saved_graph_shown);
}

// --- The first screens' OpenMovie and SetUpCar: the stand's holder
// [0x826D83E0] pointed at a harness word naming a holder whose +0 is a stage
// with a model claimed (+16 nonzero: GarageStage::MakeModel does nothing)
// and its garage camera not set up (+112's +0 0: GarageStage::Command 14's
// ApplyPreset only stores +6088), the stage's words random otherwise;
// UiMovies::Open recorded as in the guest call handlers' cases. SetUpCar:
// SaveData::Of(0) ([0x826C5B58]) a harness save data whose garage (+6120)
// holds the list walked [+0x19920] 0 to 3 steps through distinct nodes to a
// slot 0 to 2 (as car_makers' harness lays it), its count (+6328) 0 or
// not; the mode's car's +1972 -1, 0 or 2; GarageCar::Reset runs for real
// (its override on both sides). The dispatcher at the world's +0x3200. The
// world, the stage, the save data, both modes' cars, the screen fades'
// words, the opens, the stack below the call (but the original's back
// chain and saved registers) and r3 compared; the globals put back. ---

void Cars(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  constexpr uint32_t kSaveBytes = 0x1A000, kStageBytes = 0x1900;
  constexpr uint32_t kSavePointer = 0x826C5B58, kCarBytes = 1984;
  constexpr uint32_t kFades = mode_tick::kFadeOutDurations, kFadesBytes = 64;
  const Block save_block(kSaveBytes);
  const Block stage_block(kStageBytes + 0x100);
  if (!save_block.address || !stage_block.address) return;
  const uint32_t save = save_block.address, stage = stage_block.address;
  const uint32_t holder = stage + kStageBytes, holder_word = holder + 0x40;
  const uint32_t sp = CallContext(h).r1.u32;
  const uint32_t saved_holder = guest::LoadU32(base, t67::kStandHolder);
  const uint32_t saved_save = guest::LoadU32(base, kSavePointer);
  const std::vector<uint8_t> saved_cars[2] = {Read(base, t67::kType6Car, kCarBytes),
                                              Read(base, t67::kType7Car, kCarBytes)};
  const std::vector<uint8_t> saved_fades = Read(base, kFades, kFadesBytes);
  struct Function {
    const char* name;
    uint32_t address, frame, saves;
    GuestFunction recompiled;
    bool type7, set_up;
  };
  const Function functions[] = {
      {"Type6Screen0::OpenMovie", 0x822338E8, 96, 16, __imp__sub_822338E8, false, false},
      {"Type7Screen0::OpenMovie", 0x82243E80, 96, 16, __imp__sub_82243E80, true, false},
      {"Type6Screen0::SetUpCar", 0x82233E80, 128, 40, __imp__sub_82233E80, false, true},
      {"Type7Screen0::SetUpCar", 0x822443E0, 128, 40, __imp__sub_822443E0, true, true},
  };
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    for (int c = 0; c < (f.set_up ? 32 : 8); c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 1009 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, stage, kStageBytes, r);
        WriteRandom(base, save, 0x400, r);
        WriteRandom(base, save + 6120, 0x400, r);
        WriteRandom(base, sp - 1024, 1024 + 256, r);
        WriteRandom(base, kFades, kFadesBytes, r);
        guest::StoreU32(base, stage + 16, 1 + r.Below(3));
        guest::StoreU32(base, stage + 112, 0);
        guest::StoreU32(base, holder, stage);
        guest::StoreU32(base, holder_word, holder);
        guest::StoreU32(base, t67::kStandHolder, holder_word);
        // The save data's garage list (car_makers' LayGarageWalk's shape),
        // the slot's car random words.
        const uint32_t nodes = save + 6120;
        const uint32_t steps = r.Below(4);
        guest::StoreU32(base, save + 0x19920, steps);
        uint16_t at = uint16_t(r.Below(10));
        guest::StoreU16(base, nodes + 204, at);
        for (uint32_t i = 0; i < steps; i++) {
          const uint16_t next = uint16_t(10 * (i + 1) + r.Below(10));
          guest::StoreU16(base, nodes + 4 * at, next);
          at = next;
        }
        const uint32_t slot = r.Below(3);
        guest::StoreU16(base, nodes + 4 * at + 2, uint16_t(slot));
        WriteRandom(base, nodes + 212 + 1968 * slot, 1968, r);
        guest::StoreU16(base, save + 6328, r.Below(3) == 0 ? 0 : uint16_t(1 + r.Below(20)));
        guest::StoreU32(base, kSavePointer, save);
        const uint32_t car = f.type7 ? t67::kType7Car : t67::kType6Car;
        WriteRandom(base, car, kCarBytes, r);
        static constexpr uint32_t kFrom[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 2};
        guest::StoreU32(base, car + 1972, kFrom[r.Below(4)]);
        guest::StoreU32(base, world.address + 0x3200 + 204 + 600, r.Below(50));
        guest::StoreU32(base, world.address + 0x3200 + 204 + 604, r.Below(60));
        PPCContext t = CallContext(h);
        t.r3.u64 = world.address | (uint64_t(r.Below(2)) << 32);
        t.r4.u64 = world.address + 0x3200;
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        g_opens.clear();
        if (v == 0) {
          const hooks::StartUpRecorder saved = hooks::g_start_up_recorder;
          hooks::g_start_up_recorder = RecordOpen;
          ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
          hooks::g_start_up_recorder = saved;
        } else {
          t67::Calls calls = hooks::Type67Calls(t, base);
          calls.open = [](uint32_t id, uint32_t asset, uint32_t frame, uint32_t lr) -> uint64_t {
            g_opens.insert(g_opens.end(), {id, asset, frame, lr});
            return 1;
          };
          const uint64_t lr = t.lr;
          t.r3.u64 = f.set_up ? t67::Screen0SetUpCar(base, f.type7, t.r4.u64, t.r1.u32, calls)
                              : t67::Screen0OpenMovie(base, f.type7, t.r1.u32, calls);
          t.lr = lr;
        }
        // GarageStage::MakeModel has no adapter since OpenMovie's plain C++
        // calls its own (B7 loop, 2026-11-13): the recompiled side runs its
        // original, whose r28 to r31 and lr saves (40 bytes under
        // OpenMovie's frame, logged from the recompiled runs) the plain C++
        // doesn't write.
        if (!f.set_up) {
          Fill(base, sp - f.frame - 40, 40, 0);
        }
        for (const uint32_t word : g_opens) out[v].Word(word);
        out[v].Bytes(Read(base, world.address, world.bytes));
        out[v].Bytes(Read(base, stage, kStageBytes + 0x100));
        out[v].Bytes(Read(base, save, kSaveBytes));
        out[v].Bytes(Read(base, t67::kType6Car, kCarBytes));
        out[v].Bytes(Read(base, t67::kType7Car, kCarBytes));
        out[v].Bytes(Read(base, kFades, kFadesBytes));
        out[v].Bytes(Read(base, sp - 1024, 1024 - f.frame));
        out[v].Bytes(Read(base, sp - f.frame + 4, f.frame - 4 - f.saves));
        out[v].Bytes(Read(base, sp, 256));
        out[v].Word(uint32_t(t.lr));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  guest::StoreU32(base, t67::kStandHolder, saved_holder);
  guest::StoreU32(base, kSavePointer, saved_save);
  Write(base, t67::kType6Car, saved_cars[0].data(), kCarBytes);
  Write(base, t67::kType7Car, saved_cars[1].data(), kCarBytes);
  Write(base, kFades, saved_fades.data(), kFadesBytes);
}

// --- The first screens' Update: one harness movie as 22 in a harness movie
// table (its lists, named animations 0 to 47 over a live scene, slots
// queued and looping, some named 44 to 46), the UI movies' update and
// ApplyAnimation recorded through the dialogs' stand-in (UiMovie::Run,
// FinishNamed, PlayNamed and LoopNamed run for real through their
// overrides on both sides); the page state (+9644, type 7 +9648) and the
// loading word's block (+9640, type 7 +9644) in the world, the step none or
// 0 to 3, the queued step none or small. The steps: type 6's table
// (0x826367C8) and type 7's (0x82815E3C, its guard bit set) pointed at the
// leaf 0x82090770 (r3 = [r3 + 60], type 7's adjustment 0, 4 or 8), so the
// step answers the screen's word there (0 or not); type 7's guard clear
// where the step is none (the table built from the frame, no step called).
// The stand (the mode's car's +1972 -1, 0, 2 or another): [0x826D83E0]
// pointing at the holder's word, a stage whose garage camera isn't set up
// (GarageStage::Draw does nothing), +4 -1 or not and +8 0 (the car scene's
// one pool 7 model, its +5028 and +5040 0 or not: GarageStage::IsBusy
// answers either way), the car scene as build_screen's StandWorld lays it
// for GarageStage::ReloadModel with the reload count 0 to 2, so it answers
// 0 (the reload's later branch, which answers 1 and leads to the car object
// and GarageStage::SetCar, isn't laid: SetCar's scene model world, the
// load_poll harness's, is too large to lay beside this one; its Car::Init,
// memcpy and Car::Refresh are ShowCarInfo's, covered above); the car's
// +1968 0 (the reload) or not (Draw), its +12 a car model id below 80.
// Compared: the world, the movie, its nodes and animations, the stand's
// block, both modes' cars, the step tables and guard, the globals, the
// recorded calls, the stack below the call but the original's register
// saves (and its back chain's word), and r3 (both halves). ---

void Screen0Updates(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  constexpr uint32_t kMovieBlockBytes = 0xA100, kNodeBytes = 0x800, kNodes = 8;
  constexpr uint32_t kNamedCount = 48, kCarBytes = 1984;
  constexpr uint32_t kType6Steps = 0x826367C8, kType7Steps = 0x82815E3C;
  constexpr uint32_t kType7Guard = 0x82815E5C, kLeaf = 0x82090770;
  constexpr uint32_t kScene = 0x826C2E44, kFirst = 0x826C2E40, kReloads = 0x826D83E4;
  constexpr uint32_t kStackBytes = 0x6000;
  // The stand's block: the holder's word, the holder, the stage, the car
  // scene and its pool 7 model.
  constexpr uint32_t kHolderWord = 0x0, kHolder = 0x40, kStage = 0x100, kSceneAt = 0x2000,
                     kModel = 0x3800, kStandBlockBytes = 0x5000;
  const Block movie_block(kMovieBlockBytes);
  const Block nodes(kNodeBytes * kNodes + 8 * 64);
  const Block misc(0x100);  // the movie table's pairs
  const Block stand(kStandBlockBytes);
  if (!movie_block.address || !nodes.address || !misc.address || !stand.address) return;
  const uint32_t movie = movie_block.address;
  auto node = [&](uint32_t i) { return nodes.address + kNodeBytes * i; };
  const uint32_t animations = nodes.address + kNodeBytes * kNodes;
  const uint32_t sp = CallContext(h).r1.u32;
  const std::vector<uint8_t> table = Read(base, ui_movies::kMovieTable, 12);
  const std::vector<uint8_t> saved_cars[2] = {Read(base, t67::kType6Car, kCarBytes),
                                              Read(base, t67::kType7Car, kCarBytes)};
  const std::vector<uint8_t> saved_steps6 = Read(base, kType6Steps, 16);
  const std::vector<uint8_t> saved_steps7 = Read(base, kType7Steps, 36);  // and the guard
  const uint32_t globals[4] = {t67::kStandHolder, kReloads, kScene, kFirst};
  uint32_t saved_globals[4];
  for (int i = 0; i < 4; i++) saved_globals[i] = guest::LoadU32(base, globals[i]);
  struct Function {
    const char* name;
    uint32_t address, frame;
    GuestFunction recompiled, native;
    bool type7;
  };
  const Function functions[] = {
      {"Type6Screen0::Update", 0x82234F08, t67::kType6UpdateFrame, __imp__sub_82234F08,
       hooks::Type6Screen0Update, false},
      {"Type7Screen0::Update", 0x82245200, t67::kType7UpdateFrame, __imp__sub_82245200,
       hooks::Type7Screen0Update, true},
  };
  constexpr uint32_t kSaves = 56;  // r26 to r31 and lr
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    const uint32_t d = f.type7 ? 4 : 0;
    for (int c = 0; c < 64; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 7919 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, movie, kMovieBlockBytes, r);
        WriteRandom(base, nodes.address, nodes.bytes, r);
        WriteRandom(base, misc.address, misc.bytes, r);
        WriteRandom(base, stand.address, kStandBlockBytes, r);
        WriteRandom(base, sp - kStackBytes, kStackBytes + 256, r);
        // The movie: its lists, scenes and named animations.
        ui_movies::InitAnimationLists(base, movie);
        guest::StoreU32(base, movie + ui_movies::kMovieLive, node(0));
        guest::StoreU32(base, movie + ui_movies::kMovieSaved, node(1));
        guest::StoreU32(base, movie + ui_movies::kMovieFile, node(2));
        static constexpr uint32_t kTags[] = {0x54464D00, 0x50494300, 0x54585400, 0x12345678};
        for (uint32_t i = 0; i < kNodes; i++) {
          guest::StoreU32(base, node(i), kTags[r.Below(4)]);
          guest::StoreU32(base, node(i) + 100, r.Below(4));
          guest::StoreU32(base, node(i) + 104, i < 3 ? r.Below(4) : 0);
          guest::StoreU32(base, node(i) + 108, r.Below(4));
          guest::StoreU32(base, node(i) + 116, 0);
          for (uint32_t k = 128; k < 128 + 8 * 12; k += 8) {
            guest::StoreU32(base, node(i) + k, node(3 + r.Below(kNodes - 3)));
          }
        }
        for (uint32_t k = 0; k < 8; k++) {
          for (uint32_t i = 0; i < 64; i += 4) guest::StoreU32(base, animations + 64 * k + i, 0);
          guest::StoreFloat(base, animations + 64 * k + 12, float(r.Below(40)) / 4.0f);
        }
        for (uint32_t k = 0; k < kNamedCount; k++) {
          guest::StoreU32(base, movie + 176 + 36 * k + 28, node(0));
          guest::StoreU32(base, movie + 176 + 36 * k + 32, animations + 64 * r.Below(8));
        }
        guest::StoreU32(base, node(0) + 116, r.Below(4));
        for (uint32_t k = r.Below(4); k > 0; k--) {
          ui_movies::Slot slot{};
          slot[0] = r.Below(2);
          slot[1] = 0xFFFFFFFFu;
          static constexpr uint32_t kNamed[] = {44, 45, 46, 2, 0xFFFFFFFFu};
          slot[2] = kNamed[r.Below(5)];
          slot[3] = node(0);
          slot[4] = animations + 64 * r.Below(8);
          slot[5] = std::bit_cast<uint32_t>(float(r.Below(40)) / 4.0f);
          slot[6] = r.Below(2);
          slot[7] = r.Below(2);
          ui_movies::Append(base, movie + (r.Below(2) ? ui_movies::kMovieQueue
                                                      : ui_movies::kMovieLooping),
                            slot);
        }
        // The movie table: 22 the movie, among a few others.
        const uint32_t pairs = misc.address;
        const uint32_t count = 1 + r.Below(4);
        const uint32_t at = r.Below(count);
        for (uint32_t i = 0; i < count; i++) {
          guest::StoreU32(base, pairs + 8 * i, i == at ? 22 : 30 + i);
          guest::StoreU32(base, pairs + 8 * i + 4, i == at ? movie : uint32_t(r.Next()));
        }
        guest::StoreU32(base, ui_movies::kMovieTable, pairs);
        guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
        guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
        // The screen: the loading word's block and the page state.
        const uint32_t screen = world.address;
        const uint32_t loading = world.address + 0x3800, page = world.address + 0x3A00;
        guest::StoreU32(base, screen + 9640 + d, loading);
        guest::StoreU32(base, screen + 9644 + d, page);
        guest::StoreU32(base, loading, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
        static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1, 2, 3};
        const uint32_t step = kSteps[r.Below(6)];
        guest::StoreU32(base, page + 12, step);
        guest::StoreU32(base, page + 16, r.Below(2) ? 0xFFFFFFFFu : r.Below(4));
        // The steps' leaf and its answers.
        for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, kType6Steps + 4 * i, kLeaf);
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(base, kType7Steps + 8 * i, kLeaf);
          guest::StoreU32(base, kType7Steps + 8 * i + 4, 4 * r.Below(3));
        }
        const uint32_t guard = uint32_t(r.Next());
        guest::StoreU32(base, kType7Guard,
                        step == 0xFFFFFFFFu && r.Below(2) == 0 ? guard & ~1u : guard | 1u);
        for (uint32_t k = 0; k < 3; k++) {
          guest::StoreU32(base, screen + 60 + 4 * k, r.Below(2) ? 0 : uint32_t(r.Next()));
        }
        // The mode's car.
        const uint32_t car = f.type7 ? t67::kType7Car : t67::kType6Car;
        WriteRandom(base, car, kCarBytes, r);
        static constexpr uint32_t kFrom[] = {0xFFFFFFFFu, 0, 2, 5};
        guest::StoreU32(base, car + 1972, kFrom[r.Below(4)]);
        guest::StoreU32(base, car + 1968, r.Below(2) ? 0 : 1 + r.Below(3));
        guest::StoreU32(base, car + 12, r.Below(80));
        // The stand.
        const uint32_t b = stand.address;
        const uint32_t stage = b + kStage, scene = b + kSceneAt, model = b + kModel;
        guest::StoreU32(base, b + kHolderWord, b + kHolder);
        guest::StoreU32(base, b + kHolder, stage);
        guest::StoreU32(base, t67::kStandHolder, b + kHolderWord);
        guest::StoreU32(base, stage + 4, r.Below(2) ? 0xFFFFFFFFu : uint32_t(r.Next()));
        guest::StoreU32(base, stage + 8, 0);
        guest::StoreU32(base, stage + 16, r.Below(2));
        guest::StoreU32(base, stage + 112, 0);
        guest::StoreU32(base, kScene, scene);
        guest::StoreU32(base, kFirst, 0);
        guest::StoreU32(base, kReloads, r.Below(3));
        guest::StoreU32(base, scene + 0, r.Below(2) == 0 ? 4 : r.Below(4));
        guest::StoreU32(base, scene + 48, 3 + r.Below(4));
        guest::StoreU32(base, scene + 4, 0);
        guest::StoreU32(base, scene + 5048, 1);
        guest::StoreU32(base, scene + 5028, model);
        guest::StoreU32(base, scene + 5036, 1);
        guest::StoreU32(base, model + 8, 0);
        guest::StoreU32(base, model + 16, 3);
        guest::StoreU32(base, model + 20, r.Below(6));
        guest::StoreU32(base, model + 32, 1);
        guest::StoreU32(base, model + 5028, r.Below(4) == 0 ? 1 : 0);
        guest::StoreU32(base, model + 5040, r.Below(4) == 0 ? 1 : 0);
        guest::StoreU32(base, model + 5044, 3 + r.Below(3));
        for (const uint32_t word : {5048u, 5060u, 5068u, 5076u, 5084u, 5100u}) {
          guest::StoreU32(base, model + word, 0);
        }
        guest::StoreU32(base, model + 5116, scene);
        PPCContext t = CallContext(h);
        t.r3.u64 = screen | (uint64_t(r.Below(2)) << 32);
        t.r4.u64 = r.Next();
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        g_dialog_answers = Random(r.Next());
        std::vector<DialogCall> calls;
        g_dialog_calls = &calls;
        const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
            ui_movies_hooks::g_update_callee_stand_in;
        ui_movies_hooks::g_update_callee_stand_in = DialogStandIn;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          f.native(t, base);
        }
        ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
        g_dialog_calls = nullptr;
        out[v].Word(uint32_t(calls.size()));
        for (const DialogCall& call : calls) {
          for (const uint32_t w : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                   uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                   uint32_t(call.r4 >> 32)}) {
            out[v].Word(w);
          }
        }
        out[v].Bytes(Read(base, world.address, world.bytes));
        out[v].Bytes(Read(base, movie, kMovieBlockBytes));
        out[v].Bytes(Read(base, nodes.address, nodes.bytes));
        out[v].Bytes(Read(base, misc.address, misc.bytes));
        out[v].Bytes(Read(base, stand.address, kStandBlockBytes));
        out[v].Bytes(Read(base, t67::kType6Car, kCarBytes));
        out[v].Bytes(Read(base, t67::kType7Car, kCarBytes));
        out[v].Bytes(Read(base, kType6Steps, 16));
        out[v].Bytes(Read(base, kType7Steps, 36));
        for (const uint32_t g : globals) out[v].Word(guest::LoadU32(base, g));
        const uint32_t frame = sp - f.frame;
        out[v].Bytes(Read(base, sp - kStackBytes, frame - (sp - kStackBytes)));
        out[v].Bytes(Read(base, frame, f.frame - kSaves));
        out[v].Bytes(Read(base, sp, 256));
        out[v].Word(uint32_t(t.lr));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Write(base, ui_movies::kMovieTable, table.data(), 12);
  Write(base, t67::kType6Car, saved_cars[0].data(), kCarBytes);
  Write(base, t67::kType7Car, saved_cars[1].data(), kCarBytes);
  Write(base, kType6Steps, saved_steps6.data(), 16);
  Write(base, kType7Steps, saved_steps7.data(), 36);
  for (int i = 0; i < 4; i++) guest::StoreU32(base, globals[i], saved_globals[i]);
}

// --- ControllerOptions::Update: the page object (the global 0x826D848C)
// in the world at +0x3000, its step none (-1), another negative (-2), or
// steps 0 and 3 (never seen in play; recompiled on both sides) with their
// started word set (they find movie 7, do nothing else and answer 1), its
// queued step none or small. The world, the globals, r3 and the frame's
// table compared. ---

void ControllerUpdate(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  const uint32_t sp = CallContext(h).r1.u32;
  g_plant_movie = true;
  Handler(
      h, world, "ControllerOptions::Update", 0x821C7F98, 24,
      [base](Random& r, uint32_t w) {
        const uint32_t page = w + 0x3000;
        guest::StoreU32(base, t67::kControllerPage, page);
        static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0xFFFFFFFEu, 0, 3};
        const uint32_t step = kSteps[r.Below(4)];
        guest::StoreU32(base, page + 12, step);
        guest::StoreU32(base, page + 16, r.Below(2) ? 0xFFFFFFFFu : r.Below(12));
        if (int32_t(step) >= 0) guest::StoreU32(base, page + 24, 1);
      },
      [](PPCContext&, uint32_t) {}, __imp__sub_821C7F98, hooks::ControllerOptionsUpdate,
      [base, sp](Side& side, PPCContext&, uint32_t) {
        side.out.Bytes(Read(base, sp - 176 + 80, 48));
      });
}

// --- StandScene::PollTeardown: the stand's holder in a block (its three
// slots naming stages whose scene model index (+8) is -1 or 0 to 2), its
// teardown word (+12) 0 or 1, its count (+16) -1 to 3 and its model word
// (+20) 0 or not; the car scene in the block with +0 4 (CarScene::Update
// only counts its +48 down, 4 to 11, and CarScene::Draw returns at once),
// the shared assets' eight slots loading (2) now and then, the second
// set's pools 0 to 6 empty and pool 7 three scene models in use whose
// deferred build (+5040) is 0 or not (so LoadsDone answers either way);
// the first pool set none (ScenePools::UpdateA does nothing; the block's
// where StartTeardown runs, as its StartTeardownA writes it) and the
// accessories' set the block's with no entries (StartTeardown's ReleaseAll
// does nothing). Compared: the block, the frame (its back chain and body,
// not the saved registers), the globals and r3. ---

constexpr uint32_t kStandBytes = 0x8000;
constexpr uint32_t kSceneGlobal = 0x826C2E44, kFirstSet = 0x826C2E40, kAccessorySet = 0x826C5510;
constexpr uint32_t kStandGlobals[4] = {t67::kStandHolder, kSceneGlobal, kFirstSet, kAccessorySet};

// The stand's world in the block at `b` (kStandBytes), as above.
void LayStand(uint8_t* base, uint32_t b, Random& r) {
  constexpr uint32_t kScene = 0x1000, kModels = 0x3000, kModelBytes = 0x1400;
  WriteRandom(base, b, kStandBytes, r);
  const uint32_t holder = b, scene = b + kScene;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = b + 0x40 + 4 * i, stage = b + 0x100 + 0x100 * i;
    guest::StoreU32(base, holder + 4 * i, slot);
    guest::StoreU32(base, slot, stage);
    guest::StoreU32(base, stage + 8, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(3));
  }
  const uint32_t under_way = r.Below(2), made = r.Below(3) == 0 ? 0 : 1 + r.Below(5);
  guest::StoreU32(base, holder + 12, under_way);
  guest::StoreU32(base, holder + 16, uint32_t(int32_t(r.Below(5)) - 1));
  guest::StoreU32(base, holder + 20, made);
  bool loading = false;
  guest::StoreU32(base, scene + 0, 4);
  guest::StoreU32(base, scene + 48, 4 + r.Below(8));  // above the three Updates
  for (const uint32_t slot : {84u, 132u, 156u, 268u, 472u, 964u, 4156u, 4764u}) {
    const uint32_t state = r.Below(12) == 0 ? 2 : r.Below(2);
    loading |= state == 2;
    guest::StoreU32(base, scene + slot, state);
  }
  for (uint32_t pool = 0; pool < 7; pool++) {
    guest::StoreU32(base, scene + 4944 + 12 * pool + 8, 0);
  }
  guest::StoreU32(base, scene + 5028, b + kModels);
  guest::StoreU32(base, scene + 5036, 3);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t model = b + kModels + kModelBytes * k;
    guest::StoreU32(base, model + 8, k < 2 ? model + kModelBytes : 0);
    guest::StoreU32(base, model + 16, 3);
    const uint32_t deferred = r.Below(3) == 0 ? 1 : 0;
    loading |= deferred != 0;
    guest::StoreU32(base, model + 5040, deferred);
  }
  const uint32_t set = b + 0x7000;
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, set + 4 * i, 0);
  guest::StoreU32(base, t67::kStandHolder, holder);
  guest::StoreU32(base, kSceneGlobal, scene);
  // The first set only where StartTeardown runs (its StartTeardownA writes
  // it unchecked; Update's UpdateA then has nothing to do).
  const bool tear_down = under_way == 0 && (made == 0 || !loading);
  guest::StoreU32(base, kFirstSet, tear_down ? b + 0x7800 : 0);
  guest::StoreU32(base, kAccessorySet, set);
}

void PollTeardown(Context& h) {
  uint8_t* const base = h.base;
  const Block block(kStandBytes);
  if (!block.address) return;
  uint32_t saved[4];
  for (int i = 0; i < 4; i++) saved[i] = guest::LoadU32(base, kStandGlobals[i]);
  const uint32_t sp = CallContext(h).r1.u32;
  Results results("StandScene::PollTeardown (sub_82179AE8)");
  for (int c = 0; c < 48; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(uint64_t(c) * 4241 + 0x82179AE8);
      const uint32_t b = block.address;
      WriteRandom(base, sp - 1024, 1024, r);
      LayStand(base, b, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = r.Next();
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        ITC_FLAG(native_type67_modes) = false;
        __imp__sub_82179AE8(t, base);
        ITC_FLAG(native_type67_modes) = true;
      } else {
        hooks::StandScenePollTeardown(t, base);
      }
      out[v].Bytes(Read(base, b, kStandBytes));
      out[v].Bytes(Read(base, sp - t67::kPollTeardownFrame, t67::kPollTeardownFrame - 48));
      for (const uint32_t g : kStandGlobals) out[v].Word(guest::LoadU32(base, g));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  for (int i = 0; i < 4; i++) guest::StoreU32(base, kStandGlobals[i], saved[i]);
}

// --- The modes' TearDown: the mode (its ring's count and start small) and
// its screen block (the phase 4 to 7, mostly 5 or 6, the hold 0 or 1) in
// blocks of their own, the answer in the world, the stand as
// PollTeardown's cases lay it (the teardown under way with its count 1
// half the time, so it answers 1); the sound manager a fake (its slot 14
// the getter of its +60, 0, 1 or 0x100 (a nonzero word whose low byte is
// 0), slot 13 the recorder of r4 and r5 at +444). Compared: the mode, the
// block, the answer, the stand's block, the manager, the frame's +80 to
// +111, the globals and r3. ---

void TearDowns(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  const Block mode(kModeBytes);
  const Block block(kScreenBlockBytes);
  const Block stand(kStandBytes);
  const Block extra(0x400);
  if (!mode.address || !block.address || !stand.address || !extra.address) return;
  uint32_t saved[5];
  for (int i = 0; i < 4; i++) saved[i] = guest::LoadU32(base, kStandGlobals[i]);
  saved[4] = guest::LoadU32(base, menu_sounds::kSoundManager);
  struct Function {
    const char* name;
    uint32_t address;
    bool type7;
    GuestFunction recompiled;
  };
  const Function functions[] = {
      {"Type6Mode::TearDown", 0x82235C18, false, __imp__sub_82235C18},
      {"Type7Mode::TearDown", 0x82246098, true, __imp__sub_82246098},
  };
  const uint32_t sp = CallContext(h).r1.u32;
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    const t67::ModeLayout& layout = f.type7 ? t67::kType7Layout : t67::kType6Layout;
    for (int c = 0; c < 48; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 6151 + f.address);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, mode.address, mode.bytes, r);
        WriteRandom(base, block.address + layout.answer, 16, r);
        WriteRandom(base, sp - 1024, 1024, r);
        LayStand(base, stand.address, r);
        if (r.Below(2) == 0) {
          guest::StoreU32(base, stand.address + 12, 1);
          guest::StoreU32(base, stand.address + 16, 1);
        }
        guest::StoreU32(base, mode.address + 840, r.Below(4));
        guest::StoreU32(base, mode.address + 844, r.Below(200));
        guest::StoreU32(base, mode.address + 848, block.address);
        static constexpr uint32_t kPhases[] = {5, 5, 5, 6, 6, 4, 7};
        guest::StoreU32(base, block.address + layout.phase, kPhases[r.Below(7)]);
        guest::StoreU32(base, block.address + layout.phase + 4, r.Below(3) == 0 ? 1 : 0);
        const uint32_t manager = extra.address, vtable = extra.address + 0x100;
        WriteRandom(base, manager, 0x400, r);
        guest::StoreU32(base, manager, vtable);
        guest::StoreU32(base, vtable + 56, 0x82090770);  // r3 = [r3 + 60]
        guest::StoreU32(base, vtable + 52, 0x820CE9F8);  // r4, r5 to +444, +448
        static constexpr uint32_t kPlaying[] = {0, 1, 0x100};
        guest::StoreU32(base, manager + 60, kPlaying[r.Below(3)]);
        guest::StoreU32(base, menu_sounds::kSoundManager, manager);
        PPCContext t = CallContext(h);
        t.r3.u64 = (world.address + 0x100) | (uint64_t(r.Below(2)) << 32);
        t.r4.u64 = mode.address;
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          (f.type7 ? hooks::Type7ModeTearDown : hooks::Type6ModeTearDown)(t, base);
        }
        out[v].Bytes(Read(base, mode.address, mode.bytes));
        out[v].Bytes(Read(base, block.address + layout.answer, 16));
        out[v].Bytes(Read(base, world.address + 0x100, 8));
        out[v].Bytes(Read(base, stand.address, kStandBytes));
        out[v].Bytes(Read(base, extra.address, 0x400));
        out[v].Bytes(Read(base, sp - t67::kTearDownFrame + 80, 32));
        for (const uint32_t g : kStandGlobals) out[v].Word(guest::LoadU32(base, g));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  for (int i = 0; i < 4; i++) guest::StoreU32(base, kStandGlobals[i], saved[i]);
  guest::StoreU32(base, menu_sounds::kSoundManager, saved[4]);
}

}  // namespace

// --- StandScene::Create: the whole of it, its callees' worlds as the game
// has them: the viewports, the scene, the first set and the accessories'
// set none (made), the stand's holder none (made, with its slots, stages
// and the second set's fill) or, now and then, one in a block (only the
// viewports and the scene made), the cars 0 to 3. Archive::Load's calls
// recorded. Compared: the calls (the presets' blocks by content, then
// freed), every block made with the pointers into the case's blocks made
// offsets (the viewports, the scene, the sets, the second and first sets'
// objects and records, the holder, its slots and stages), the providers
// (scene_pools_take::TakeProvider), the part files' pages, the globals,
// the palette and its two words, the viewports' vectors and near plane,
// the stages' constants, the exit table's additions and r3; then all freed
// and the globals put back. The modes' slot 0 (below) take the stand the
// same way. ---

constexpr uint32_t kTakeMarker = 0x50414745;

// The globals and image words StandScene::Create's callees write, saved
// once and put back before each side and at the end.
struct StandSaved {
  explicit StandSaved(uint8_t* base) : base(base) {
    for (const uint32_t g : kGlobals) Keep(g, 4);
    Keep(kPalette, 1024);
    Keep(0x82601854, 8);       // CarScene::Create's two words
    Keep(0x826C2CF0, 0x34);    // the viewports' vectors and guard
    Keep(0x826BAC00 + 384, 4); // camera pass 0's near plane
    Keep(0x8296B950, 0x350);   // the stages' constants
    Keep(0x8265ED00, 16);      // the ambient
  }
  ~StandSaved() { Restore(); }
  void Keep(uint32_t at, uint32_t bytes) { saved.push_back({at, bytes, Read(base, at, bytes)}); }
  void Restore() const {
    for (const Saved& k : saved) Write(base, k.address, k.data.data(), k.bytes);
  }
  // The words compared after a side (the globals by `regions`).
  void Take(const std::vector<scene_pools_take::PageRegion>& regions, Outputs& out) const {
    for (const uint32_t g : kGlobals) {
      out.Bytes(scene_pools_take::Normalized(base, regions, g, 4));
    }
    for (size_t k = std::size(kGlobals); k < saved.size(); k++) {
      out.Bytes(Read(base, saved[k].address, saved[k].bytes));
    }
  }
  struct Saved {
    uint32_t address, bytes;
    std::vector<uint8_t> data;
  };
  static constexpr uint32_t kPalette = 0x826C5110;
  static constexpr uint32_t kGlobals[] = {t67::kStandHolder, cameras::kViewports, scene::kScene,
                                          scene::kFirstSet, accessories::kSet};
  uint8_t* base;
  std::vector<Saved> saved;
};

// Archive::Load's calls (and, for the modes' slot 0, the UI movies' start
// and Strings::Load's) recorded through hooks::g_start_up_recorder: each
// call's address, r3 to r6 (r1 and lr for the two others); Archive::Load
// answered 7.
std::vector<uint32_t>* g_stand_calls = nullptr;
bool g_record_mode_calls = false;
bool StandRecorder(PPCContext& ctx, uint32_t address) {
  if (g_stand_calls == nullptr) return false;
  if (address == 0x8209DE48) {
    g_stand_calls->insert(g_stand_calls->end(),
                          {address, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32});
    ctx.r3.u64 = 7;
    return true;
  }
  if (g_record_mode_calls && (address == 0x8211AD00 || address == 0x8210A8D0)) {
    g_stand_calls->insert(g_stand_calls->end(), {address, ctx.r3.u32, ctx.r4.u32,
                                                 ctx.r1.u32, uint32_t(ctx.lr)});
    return true;
  }
  return false;
}

// The blocks a side made, found from the globals: regions for the
// pointers' offsets, freed after.
struct StandTake {
  std::vector<scene_pools_take::PageRegion> regions;
  std::vector<std::pair<uint32_t, uint32_t>> made;  // pages, bytes
  std::vector<std::pair<int, uint32_t>> objects;    // first-set pool (-1 second), object
  std::vector<uint32_t> heap;                       // operator new's blocks
  uint32_t scene = 0;

  void Add(uint8_t*, uint32_t at, uint32_t bytes) {
    if (at != 0 && memory::IsPages(at)) {
      for (const auto& [m, n] : made) {
        if (m == at) return;
      }
      regions.push_back({at, bytes});
      made.push_back({at, bytes});
    }
  }
  // Heap blocks named by words of [at, at + bytes) (operator new's).
  void AddHeapFrom(uint8_t* base, uint32_t at, uint32_t bytes) {
    for (uint32_t o = 0; o + 4 <= bytes; o += 4) {
      const uint32_t w = guest::LoadU32(base, at + o);
      if (w != 0 && memory::IsHeapBlock(w) &&
          std::find(heap.begin(), heap.end(), w) == heap.end()) {
        heap.push_back(w);
        regions.push_back({w, memory::Size(w)});
      }
    }
  }
  void Collect(uint8_t* base, bool held) {
    namespace sl = scene_loaders;
    const uint32_t viewports = guest::LoadU32(base, cameras::kViewports);
    Add(base, viewports, 12);
    if (memory::IsPages(viewports)) {
      Add(base, guest::LoadU32(base, viewports + 0), 1872);
      Add(base, guest::LoadU32(base, viewports + 4), 1872);
    }
    scene = guest::LoadU32(base, scene::kScene);
    const uint32_t first = guest::LoadU32(base, scene::kFirstSet);
    Add(base, scene, scene::kSceneBytes);
    Add(base, first, 140);
    Add(base, guest::LoadU32(base, accessories::kSet), 32);
    const uint32_t holder = guest::LoadU32(base, t67::kStandHolder);
    if (!held) {
      Add(base, holder, 24);
      for (uint32_t i = 0; memory::IsPages(holder) && i < 3; i++) {
        const uint32_t slot = guest::LoadU32(base, holder + 4 * i);
        Add(base, slot, 4);
        if (memory::IsPages(slot)) Add(base, guest::LoadU32(base, slot), t67::kGarageStageBytes);
      }
    }
    if (memory::IsPages(scene)) {
      for (uint32_t pool = 0; pool < 8; pool++) {
        for (uint32_t node = guest::LoadU32(base, scene + 4944 + 12 * pool), n = 0;
             node != 0 && n < 16; node = guest::LoadU32(base, node + 8), n++) {
          if (memory::IsPages(node)) {
            Add(base, node, car_models::kSecondSetBytes[pool]);
            objects.push_back({-1, node});
          }
        }
      }
    }
    if (memory::IsPages(first)) {
      Add(base, guest::LoadU32(base, first + 52),
          80 * (guest::LoadU32(base, first + 12) + guest::LoadU32(base, first + 40)));
      for (uint32_t pool = 0; pool < 7; pool++) {
        for (uint32_t node = guest::LoadU32(base, first + 56 + 12 * pool), n = 0;
             node != 0 && n < 16; node = guest::LoadU32(base, node + 8), n++) {
          if (memory::IsPages(node)) {
            Add(base, node, sl::kPoolObjectBytes[pool]);
            objects.push_back({int(pool), node});
          }
        }
      }
    }
  }
  // The recorded calls: the presets' blocks by content (then freed), the
  // words into the regions as offsets, other pages as markers.
  void TakeCalls(uint8_t* base, std::vector<uint32_t> calls, Outputs& out) {
    out.Word(uint32_t(calls.size()));
    for (size_t k = 0; k + 5 <= calls.size(); k += 5) {
      if (calls[k] == 0x8209DE48 && calls[k + 3] == t67::kOnPresetsLoaded &&
          memory::IsPages(calls[k + 2])) {
        out.Bytes(Read(base, calls[k + 2], 0x400));
        presets.push_back(calls[k + 2]);
        calls[k + 2] = kTakeMarker;
      }
    }
    for (uint32_t& w : calls) {
      bool in_region = false;
      for (size_t k = 0; k < regions.size(); k++) {
        if (w >= regions[k].start && w - regions[k].start < regions[k].bytes + 16) {
          w = 0xB0000000u + (uint32_t(k) << 20) + (w - regions[k].start);
          in_region = true;
          break;
        }
      }
      if (!in_region && w != 0 && memory::IsPages(w)) w = kTakeMarker;
    }
    for (const uint32_t w : calls) out.Word(w);
  }
  // The scene's part files' pages and every provider, then each block.
  void TakeBlocks(uint8_t* base, Outputs& out) {
    namespace pt = scene_pools_take;
    constexpr uint32_t kOwn[] = {452, 4112, 1152, 1176, 1200, 1224};
    if (memory::IsPages(scene)) {
      for (const uint32_t at : kOwn) {
        const uint32_t pages = guest::LoadU32(base, scene + at + 16);
        if (pages != 0 && memory::IsPages(pages)) {
          out.Word(kTakeMarker);
          memory::FreePages(pages);
          guest::StoreU32(base, scene + at + 16, kTakeMarker);
        } else {
          out.Word(pages);
        }
      }
      for (const uint32_t provider : scene::kAssetsProviders) {
        pt::TakeProvider(base, scene + scene::kAssets + provider, out);
      }
      for (const uint32_t model : scene::kAssetsModels) {
        pt::TakeProvider(base, scene + scene::kAssets + model + models::kModelTextures, out);
      }
    }
    for (const auto& [pool, object] : objects) {
      if (pool >= 0) {
        for (const uint32_t offset : pt::PoolProviders(uint32_t(pool))) {
          pt::TakeProvider(base, object + offset, out);
        }
      }
    }
    out.Word(uint32_t(made.size()));
    for (const auto& [at, bytes] : made) {
      out.Bytes(pt::Normalized(base, regions, at, bytes));
    }
    out.Word(uint32_t(heap.size()));
    for (const uint32_t at : heap) {
      out.Word(memory::Size(at));
      out.Bytes(pt::Normalized(base, regions, at, memory::Size(at)));
    }
  }
  // r3 as a region and offset, else its offset from `sp`.
  uint32_t Word(uint32_t value, uint32_t sp) const {
    for (size_t k = 0; k < regions.size(); k++) {
      if (value >= regions[k].start && value - regions[k].start < regions[k].bytes) {
        return 0xB0000000u + (uint32_t(k) << 20) + (value - regions[k].start);
      }
    }
    return value - sp;
  }
  void Free() {
    for (const auto& [at, bytes] : made) memory::FreePages(at);
    for (const uint32_t p : presets) memory::FreePages(p);
    for (const uint32_t at : heap) memory::crt::OperatorDelete(at);
  }
  std::vector<uint32_t> presets;
};

void StandSceneCreates(Context& h) {
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue() ||
      !hooks::CarModelsSceneSetUpNative()) {
    ITC_LOG_WARN("harness: type67_modes: the allocation layer, the archive's queue, the models' "
                 "lifetime or memcpy isn't native: StandScene::Create's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> exits(c_runtime::ExitMutex());
  std::lock_guard<std::recursive_mutex> blocks(models::MemoryBlocksMutex());
  const StandSaved saved(base);
  const Block world(0x1000);
  if (!world.address) return;
  Random random(0x82180B00);
  Results results("StandScene::Create (sub_82180B00)");
  for (int c = 0; c < 12; c++) {
    const uint64_t seed = random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      saved.Restore();
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      WriteRandom(base, world.address, world.bytes, r);
      WriteRandom(base, t.r1.u32 - 0x1000, 0x1000, r);
      const bool held = c % 4 == 3;
      guest::StoreU32(base, t67::kStandHolder, held ? world.address : 0);
      guest::StoreU32(base, scene::kScene, 0);
      guest::StoreU32(base, scene::kFirstSet, 0);
      guest::StoreU32(base, accessories::kSet, 0);
      const uint32_t mark = exit_table::Mark(base);
      std::vector<uint32_t> calls;
      g_stand_calls = &calls;
      const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = StandRecorder;
      const uint32_t cars[] = {1, 3, 1, 0, 2, 3};
      t.r3.u64 = cars[r.Below(6)];
      if (v == 0) {
        ITC_FLAG(native_type67_modes) = false;
        __imp__sub_82180B00(t, base);
        ITC_FLAG(native_type67_modes) = true;
      } else {
        hooks::StandSceneCreate(t, base);
      }
      hooks::g_start_up_recorder = saved_recorder;
      g_stand_calls = nullptr;
      StandTake take;
      take.Collect(base, held);
      take.TakeCalls(base, calls, out[v]);
      take.TakeBlocks(base, out[v]);
      out[v].Bytes(scene_pools_take::Normalized(base, take.regions, world.address, world.bytes));
      saved.Take(take.regions, out[v]);
      out[v].Bytes(scene_pools_take::Normalized(base, take.regions, t.r1.u32 - 176 + 80, 48));
      exit_table::Take(base, mark, out[v]);
      out[v].Word(take.Word(t.r3.u32, t.r1.u32));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      take.Free();
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  saved.Restore();
  results.Report();
}

// --- The modes' slot 0 (Type6Mode::Init, Type7Mode::Init): the mode a
// block of random words (its dispatcher's count 0 to 7), the stand made as
// StandScene::Create's cases have it (its holder none or held), the UI
// movies' start and Strings::Load recorded (their adapters' recorder; the
// native side's guest calls reach the same adapters), Archive::Load
// recorded, LoadingScreen::Show over the loading screen's world
// (loading_world.h, a made-up time for the tip), the screen fades' words
// saved and put back. Compared: the recorded calls, the mode, the screen
// block, its panels (operator new's blocks, found by the words naming
// them) and every block the stand made, with the pointers into them made
// offsets; the loading world, the fades, the stand's globals, the answer's
// frame words and r3; then all freed. The originals run with the flag on
// (their callees' adapters native, as in the game). ---

void ModeInits(Context& h) {
  namespace lw = loading_world;
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue() ||
      !hooks::CarModelsSceneSetUpNative()) {
    ITC_LOG_WARN("harness: type67_modes: the allocation layer, the archive's queue, the models' "
                 "lifetime or memcpy isn't native: the modes' slot 0 cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> exits(c_runtime::ExitMutex());
  std::lock_guard<std::recursive_mutex> blocks(models::MemoryBlocksMutex());
  constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x40;
  const std::vector<uint8_t> fades = Read(base, kFades, kFadesBytes);
  const StandSaved saved(base);
  const lw::MovieWorld w;
  const Block mode(kModeBytes);
  const Block world(0x1000);
  if (!w.ok() || !mode.address || !world.address) return;
  struct Function {
    const char* name;
    uint32_t address;
    bool type7;
    GuestFunction recompiled, native;
    uint32_t frame, block_bytes;
  };
  const Function functions[] = {
      {"Type6Mode::Init", 0x82236880, false, __imp__sub_82236880, hooks::Type6ModeInit,
       t67::kType6ModeInitFrame, 91272},
      {"Type7Mode::Init", 0x82246C90, true, __imp__sub_82246C90, hooks::Type7ModeInit,
       t67::kType7ModeInitFrame, 71292},
  };
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    // The screen block's words that are stack leftovers: the members' Inits
    // copy three frame words they never write into each transition's
    // functor (its +32 to +40; the transitions 48 bytes from the machine's
    // +0, their count at +9600), dead words; compared on their own
    // (expected to differ: the original's callees that run recompiled, the
    // screen block's constructor since its adapter went, leave register
    // saves and back chains in the stack there that the plain C++ doesn't,
    // and the plain C++ back chains the originals don't).
    Results leftovers(fmt::format(
        "{} (sub_{:08X}), the transitions' stack leftovers (expected to differ)", f.name,
        f.address));
    for (int c = 0; c < 6; c++) {
      Outputs out[2];
      std::vector<uint8_t> blocks[2];
      std::vector<uint32_t> masked[2];  // the leftovers' offsets in the block
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 811 + f.address);
        saved.Restore();
        const lw::SavedGlobals loading(base);
        lw::ShapeMovieWorld(base, w, r);
        lw::BuildStringTable(base, w);
        PPCContext t = CallContext(h);
        t.r1.u64 &= ~0xFull;
        WriteRandom(base, mode.address, mode.bytes, r);
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, t.r1.u32 - 0x1000, 0x1000, r);
        const uint32_t handlers = r.Below(8);
        guest::StoreU32(base, mode.address + 36 + 200, handlers);
        const bool held = c % 3 == 2;
        guest::StoreU32(base, t67::kStandHolder, held ? world.address : 0);
        guest::StoreU32(base, scene::kScene, 0);
        guest::StoreU32(base, scene::kFirstSet, 0);
        guest::StoreU32(base, accessories::kSet, 0);
        guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
        guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
        for (uint32_t i = 0; i < 3; i++) {
          guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                          r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
        }
        const uint32_t mark = exit_table::Mark(base);
        time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
        std::vector<uint32_t> calls;
        g_stand_calls = &calls;
        g_record_mode_calls = true;
        const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = StandRecorder;
        t.r3.u64 = mode.address;
        // The original with the flag on: its callees' adapters native, as
        // the game runs it (with the flag off, the members' Inits would run
        // recompiled and leave register saves and back chains in their
        // frames that later Inits copy as leftovers; the plain C++ Inits
        // write neither, as in the game).
        if (v == 0) {
          f.recompiled(t, base);
        } else {
          f.native(t, base);
        }
        hooks::g_start_up_recorder = saved_recorder;
        g_record_mode_calls = false;
        g_stand_calls = nullptr;
        time_hooks::ClearFakeNow();
        StandTake take;
        take.Collect(base, held);
        const uint32_t block = guest::LoadU32(base, mode.address + 848);
        const bool block_made = memory::IsPages(block);
        if (block_made) {
          take.regions.push_back({block, f.block_bytes});
          take.AddHeapFrom(base, block, f.block_bytes);
        }
        // The recorded calls' frames and return addresses: r1 by its offset.
        for (size_t k = 0; k + 5 <= calls.size(); k += 5) {
          if (calls[k] != 0x8209DE48) calls[k + 3] -= t.r1.u32;
        }
        take.TakeCalls(base, calls, out[v]);
        take.TakeBlocks(base, out[v]);
        out[v].Word(block_made);
        if (block_made) {
          blocks[v] = scene_pools_take::Normalized(base, take.regions, block, f.block_bytes);
          // The machines the slot 0 added to the dispatcher.
          const uint32_t added = guest::LoadU32(base, mode.address + 36 + 200);
          for (uint32_t k = handlers; k < added && k < handlers + 16; k++) {
            const uint32_t machine = guest::LoadU32(base, mode.address + 36 + 4 * k);
            if (machine < block || machine - block >= f.block_bytes) continue;
            const uint32_t count = guest::LoadU32(base, machine + game_modes::kTransitionCount);
            for (uint32_t e = 0; e < count && e < 200; e++) {
              for (const uint32_t word : {32u, 36u, 40u}) {
                masked[v].push_back(machine - block + 48 * e + word);
              }
            }
          }
        }
        out[v].Bytes(scene_pools_take::Normalized(base, take.regions, mode.address, mode.bytes));
        out[v].Bytes(scene_pools_take::Normalized(base, take.regions, world.address,
                                                  world.bytes));
        saved.Take(take.regions, out[v]);
        out[v].Bytes(Read(base, t.r1.u32 - f.frame + 80, 8));
        out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
        out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
        out[v].Bytes(Read(base, w.strings.address, w.strings.bytes));
        out[v].Word(guest::LoadU32(base, race_loading::kLoadingShown));
        out[v].Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
        out[v].Bytes(Read(base, kFades, kFadesBytes));
        exit_table::Take(base, mark, out[v]);
        out[v].Word(take.Word(t.r3.u32, t.r1.u32));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        take.Free();
        if (block_made) memory::FreePages(block);
        loading.Restore();
        Write(base, kFades, fades.data(), kFadesBytes);
      }
      Outputs left[2];
      for (int v = 0; v < 2; v++) {
        std::vector<uint8_t> kept = blocks[v];
        out[v].Word(uint32_t(masked[v].size()));
        for (const uint32_t o : masked[v]) {
          if (o + 4 > kept.size()) continue;
          out[v].Word(o);
          left[v].Bytes(std::vector<uint8_t>(kept.begin() + o, kept.begin() + o + 4));
          std::fill(kept.begin() + o, kept.begin() + o + 4, 0);
        }
        out[v].Bytes(kept);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
      leftovers.Check(left[0], left[1], fmt::format("case {}", c));
    }
    results.Report();
    leftovers.Report();
  }
  saved.Restore();
  Write(base, kFades, fades.data(), kFadesBytes);
}

// StandMode::Release over a mode of the case's: its screen block (+848)
// the layer's pages, the stand holder and viewports as StandScene::
// Release's cases make them, the string table ([0x826C5C34]) a table of the
// case's (each file none, the layer's pages or a pointer never freed,
// counts 0 to 3) and the movie table 0 to 4 pairs of no movie (keys 100 +
// n, some repeated). Both sides reach the same overrides of
// StandScene::Release (the original's) or its plain C++ (ours, the same
// work), Strings::Release and UiMovies::DeleteAll's jump. Compared: what's
// freed, the mode, the globals, the string table, the movie table's pairs,
// the frame (its register saves left out), the caller's area, r1 and r3
// (the string table as 1, the movie table as 2, a pair by its offset).
void StandModeReleases(Context& h) {
  namespace sf = strings_and_font;
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("StandMode::Release (sub_82246038)");
  const Block mode(0x400), table(sf::kStringsBytes);
  if (!mode.address || !table.address) {
    ITC_LOG_ERROR("harness: type67_modes: no block from the allocation layer");
    return;
  }
  const uint32_t saved_holder = guest::LoadU32(base, t67::kStandHolder);
  const uint32_t saved_viewports = guest::LoadU32(base, cameras::kViewports);
  const uint32_t saved_strings = guest::LoadU32(base, 0x826C5C34);
  const std::vector<uint8_t> saved_movies = Read(base, ui_movies::kMovieTable, 12);
  for (int c = 0; c < 40; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<uint32_t> made;
      auto pages = [&](uint32_t bytes) {
        const uint32_t block = memory::AllocPages(bytes, 4096);
        WriteRandom(base, block, bytes < 64 ? bytes : 64, r);
        made.push_back(block);
        return block;
      };
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - 512, 512 + 256, r);
      WriteRandom(base, mode.address, mode.bytes, r);
      guest::StoreU32(base, mode.address + 848, pages(0x2000));
      const uint32_t viewports = pages(12);
      guest::StoreU32(base, viewports + 0, pages(1872));
      guest::StoreU32(base, viewports + 4, pages(1872));
      guest::StoreU32(base, cameras::kViewports, viewports);
      const uint32_t holder = pages(12);
      guest::StoreU32(base, t67::kStandHolder, holder);
      for (uint32_t i = 0; i < 3; i++) {
        uint32_t slot = 0;
        if (r.Below(2) != 0) {
          slot = pages(64);
          guest::StoreU32(base, slot, 0);
        }
        guest::StoreU32(base, holder + 4 * i, slot);
      }
      WriteRandom(base, table.address, table.bytes, r);
      uint32_t files[sf::kStringFiles];
      uint32_t kinds[sf::kStringFiles];
      for (uint32_t f = 0; f < sf::kStringFiles; f++) {
        kinds[f] = r.Below(3);
        uint32_t count = r.Below(4);
        files[f] = kinds[f] == 0   ? 0
                   : kinds[f] == 1 ? memory::AllocPages(64, 4096)
                                   : uint32_t(r.Next()) | 1;
        if (kinds[f] == 2 && count == 1) {
          count = 2;
        }
        guest::StoreU32(base, table.address + 4 * f, files[f]);
        guest::StoreU32(base, table.address + 68 + 4 * f, r.Below(2));
        guest::StoreU32(base, table.address + 4236 + 4 * f, count);
      }
      guest::StoreU32(base, 0x826C5C34, table.address);
      const uint32_t capacity = 1 + r.Below(4);
      const uint32_t count = r.Below(capacity + 1);
      const uint32_t array = memory::crt::OperatorNewArray(base, 8 * capacity).value_or(0);
      WriteRandom(base, array, 8 * capacity, r);
      for (uint32_t k = 0; k < count; k++) {
        const uint32_t key = k > 0 && r.Below(4) == 0 ? 100 + r.Below(k) : 100 + k;
        guest::StoreU32(base, array + 8 * k, key);
        guest::StoreU32(base, array + 8 * k + 4, 0);
      }
      guest::StoreU32(base, ui_movies::kMovieTable, array);
      guest::StoreU32(base, ui_movies::kMovieTable + 4, capacity);
      guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
      t.r3.u64 = mode.address;
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        ITC_FLAG(native_type67_modes) = false;
        __imp__sub_82246038(t, base);
        ITC_FLAG(native_type67_modes) = true;
      } else if (!hooks::StandModeRelease(t, base)) {
        ITC_LOG_ERROR("harness: type67_modes: StandMode::Release refused");
      }
      out[v].Word(uint32_t(made.size()));
      for (const uint32_t block : made) {
        out[v].Word(memory::IsPages(block));
        if (memory::IsPages(block)) memory::FreePages(block);
      }
      out[v].Bytes(Read(base, mode.address, mode.bytes));
      out[v].Word(guest::LoadU32(base, t67::kStandHolder));
      out[v].Word(guest::LoadU32(base, cameras::kViewports));
      for (uint32_t f = 0; f < sf::kStringFiles; f++) {
        const uint32_t now = guest::LoadU32(base, table.address + 4 * f);
        out[v].Word(now == files[f] ? 1 : now);
        if (kinds[f] == 1) {
          const bool left = memory::IsPages(files[f]);
          out[v].Word(left ? 1 : 0);
          if (left) {
            memory::FreePages(files[f]);
          }
        }
      }
      out[v].Bytes(Read(base, table.address + 68, table.bytes - 68));
      const uint32_t now = guest::LoadU32(base, ui_movies::kMovieTable);
      out[v].Word(now == array ? 1 : now);
      out[v].Bytes(Read(base, ui_movies::kMovieTable + 4, 8));
      out[v].Bytes(Read(base, array, 8 * capacity));
      out[v].Bytes(Read(base, sp - 96, 80));
      out[v].Bytes(Read(base, sp, 256));
      out[v].Word(t.r1.u32);
      const uint32_t r3 = t.r3.u32;
      out[v].Word(r3 == table.address              ? 1
                  : r3 == ui_movies::kMovieTable   ? 2
                  : r3 >= array && r3 < array + 64 ? 0x100 + (r3 - array)
                                                   : r3);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (array != 0 && memory::IsHeapBlock(array)) memory::crt::OperatorDeleteArray(array);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  guest::StoreU32(base, t67::kStandHolder, saved_holder);
  guest::StoreU32(base, cameras::kViewports, saved_viewports);
  guest::StoreU32(base, 0x826C5C34, saved_strings);
  Write(base, ui_movies::kMovieTable, saved_movies.data(), 12);
  results.Report();
}

void Type67Modes(Context& h) {
  const Block object(kObjectBytes);
  const Block world(kWorldBytes);
  const Block movie(0xA000);
  g_movie = movie.address;
  if (!object.address || !world.address || !movie.address) {
    ITC_LOG_ERROR("harness: type67_modes: no block from the allocation layer");
    return;
  }
  Inits(h, object);
  Handlers(h, world);
  Modes(h, world);
  GuestCallHandlers(h, world);
  Garage(h);
  GarageCamera(h, world);
  GarageCameraCommands(h, world);
  GarageLights(h, world);
  StandSlots(h, world);
  Dialogs(h, world);
  Screen0Helpers(h, world);
  Cars(h, world);
  Screen0Updates(h, world);
  ControllerUpdate(h, world);
  PollTeardown(h);
  TearDowns(h, world);
  StandSceneCreates(h);
  ModeInits(h);
  StandModeReleases(h);
}


// --- The type 2 mode (src/type2_mode/, docs/functions/type2-mode.md; the
// harness `type2_mode`, here for the stand's and the loading screen's worlds
// this file lays). Each case: the mode a block of random words (its
// dispatcher without handlers, so GameMessages::Dispatch takes the queued
// messages and delivers none; its ring's count and start small), its +848 a
// screen block of the type 2 mode's size (random words, the 14 machines'
// states and loaded words picked, the shown index 0 to 7), the answer in
// the world; the originals run with --native_type2_mode off (the functions
// they call of the mode's are the originals too) or the register steps
// (hooks/type2_mode.h). Compared: the mode, the block, the answer, the
// globals, the function's frame without its register saves and r3. ---

namespace {

namespace t2m = type2_mode;

constexpr uint32_t kType2BlockBytes = 0x27C00;  // past +0x27B7C
// The 14 machines' offsets (MachineOf's).
constexpr uint32_t kType2Machines[] = {0,       9640,    19792,   0x173D0, 0x19978,
                                       0x23018, 0x1BF20, 0x12880, 56624,   0x102D8,
                                       0x14E28, 0x1E4C8, 0x20A70, 0x255C0};

// The block's machines' states (from the ones the functions test, now and
// then any), loaded words (0 now and then), the shown index (0 to 7, now and
// then 3, past 7 never: FindShown writes 0 to 7 only), the save screen's
// state.
void ShapeType2Block(uint8_t* base, uint32_t block, Random& r) {
  static constexpr uint32_t kStates[] = {4, 5, 5, 6, 8, 9, 10, 11, 12, 14, 20, 1, 7};
  const bool all_loaded = r.Below(3) != 0;
  for (const uint32_t m : kType2Machines) {
    guest::StoreU32(base, block + m + 9604,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kStates[r.Below(std::size(kStates))]);
    guest::StoreU32(base, block + m + 9608,
                    all_loaded || r.Below(6) != 0 ? 1 + r.Below(3) : 0);
  }
  guest::StoreU32(base, block + t2m::kBlockLoaded, all_loaded || r.Below(6) != 0 ? 1 : 0);
  guest::StoreU32(base, block + t2m::kShown, r.Below(8));
  guest::StoreU32(base, block + t2m::kSaveScreenState, r.Below(3) == 0 ? 5 : r.Below(10));
}

// MachineOf, AllLoaded and FindShown.
void Type2Blocks(Context& h) {
  uint8_t* const base = h.base;
  const Block block(kType2BlockBytes);
  if (!block.address) return;
  {
    Results results("ModeType2::MachineOf (sub_82196FC0; sub_82197000)");
    for (int c = 0; c < 400; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 131 + 0x82196FC0);
        PPCContext t = CallContext(h);
        t.r3.u64 = r.Below(3) == 0 ? r.Next() : uint32_t(r.Next());
        const uint32_t pick = r.Below(4);
        t.r4.u64 = pick == 0   ? r.Next()
                   : pick == 1 ? uint64_t(int64_t(int32_t(r.Below(8)) - 4))
                               : r.Below(48);
        if (v == 0) {
          ITC_FLAG(native_type2_mode) = false;
          __imp__sub_82196FC0(t, base);
          ITC_FLAG(native_type2_mode) = true;
        } else {
          hooks::Type2ModeMachineOf(t, base);
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t frame;
  };
  const Function functions[] = {
      {"ModeType2::AllLoaded (sub_82197130)", __imp__sub_82197130, hooks::Type2ModeAllLoaded, 0},
      {"ModeType2::FindShown (sub_82197218; sub_82196FC0)", __imp__sub_82197218,
       hooks::Type2ModeFindShown, 96},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 200; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 137 + f.frame);
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(base, block.address, block.bytes, r);
        WriteRandom(base, sp - 256, 256, r);
        ShapeType2Block(base, block.address, r);
        t.r3.u64 = block.address | (uint64_t(r.Below(2)) << 32);
        if (v == 0) {
          ITC_FLAG(native_type2_mode) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type2_mode) = true;
        } else {
          f.native(t, base);
        }
        out[v].Bytes(Read(base, block.address, block.bytes));
        if (f.frame != 0) out[v].Bytes(Read(base, sp - f.frame, f.frame - 8));
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The delegate functions, the thunks and the +32 interface. WaitForLoads'
// world as the type 6 and 7 modes' (Modes above): the sound manager a fake
// (Sound::Play's PlayCue2D slot the recorder sub_820A8420), the loading
// screen's current movie 0 to 2 (a harness movie with empty animation lists;
// the update and ApplyAnimation recorded through the UI movies' stand-in);
// its shown index never 3 (the hub's sub_8218F620 there, never seen in play,
// not given a world). TearDown's stand as PollTeardown's cases lay it.
// Interrupt's options settings ([0x826D8478] +712) 0 or not and the
// dialog's status 0 to 5. The thunks' object a vtable in the world: each
// slot the type 2 mode's function or the getter sub_82090770 (another
// mode's). ForceExit's number 0 to 5 or any, the words below r1 compared.
void Type2Delegates(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  const Block mode(kModeBytes);
  const Block block(kType2BlockBytes);
  const Block stand(kStandBytes);
  const Block extra(0x400);
  const Block loading_movie(0xA100);
  if (!mode.address || !block.address || !stand.address || !extra.address ||
      !loading_movie.address) {
    return;
  }
  constexpr uint32_t kLoadingCurrent = 0x82815E64, kLoadingMovies = 0x82815E68;
  const uint32_t globals[] = {t2m::kPadBlocked0,     t2m::kPadBlocked1,  t2m::kLeavingFlag,
                              t2m::kOptionsSettings, t2m::kDialogStatus, menu_sounds::kSoundManager,
                              kLoadingCurrent,       kLoadingMovies,     kLoadingMovies + 4,
                              kLoadingMovies + 8,    kStandGlobals[0],   kStandGlobals[1],
                              kStandGlobals[2],      kStandGlobals[3]};
  uint32_t saved[std::size(globals)];
  for (size_t i = 0; i < std::size(globals); i++) saved[i] = guest::LoadU32(base, globals[i]);
  enum Kind { kDelegate, kWait, kTearDown, kThunk, kInterrupt, kForceExit };
  struct Function {
    const char* name;
    Kind kind;
    GuestFunction recompiled, native;
    int32_t frame, saves;  // the own frame compared: [-frame, -saves)
    uint32_t slot = 0;     // a thunk's
    uint32_t target = 0;   // its type 2 function
  };
  const Function functions[] = {
      {"ModeType2::NextMode (sub_82197D50; sub_82197218)", kDelegate, __imp__sub_82197D50,
       hooks::Type2ModeNextMode, 144, 56},
      {"ModeType2::LeaveScreen (sub_82197C88; sub_82197218)", kDelegate, __imp__sub_82197C88,
       hooks::Type2ModeLeaveScreen, 128, 32},
      {"ModeType2::AfterLeaveScreen (sub_821977E0; sub_82197218)", kDelegate,
       __imp__sub_821977E0, hooks::Type2ModeAfterLeaveScreen, 96, 8},
      {"ModeType2::Restart (sub_82197A30)", kDelegate, __imp__sub_82197A30,
       hooks::Type2ModeRestart, 112, 16},
      {"ModeType2::Leaving (sub_82198008)", kDelegate, __imp__sub_82198008,
       hooks::Type2ModeLeaving, 128, 32},
      {"ModeType2::WaitForLoads (sub_82197B00; sub_82197218, sub_82197130)", kWait,
       __imp__sub_82197B00, hooks::Type2ModeWaitForLoads, 144, 40},
      {"ModeType2::TearDown (sub_821970C0)", kTearDown, __imp__sub_821970C0,
       hooks::Type2ModeTearDown, 112, 24},
      {"Mode::Slot8Thunk (sub_8223D4B0)", kThunk, __imp__sub_8223D4B0, hooks::ModeSlot8Thunk, 0,
       0, 8, 0x82197A30},
      {"Mode::Slot9Thunk (sub_8223D4C0)", kThunk, __imp__sub_8223D4C0, hooks::ModeSlot9Thunk, 0,
       0, 9, 0x82197B00},
      {"Mode::Slot10Thunk (sub_8223D4A0)", kThunk, __imp__sub_8223D4A0, hooks::ModeSlot10Thunk,
       0, 0, 10, 0x82197C88},
      {"Mode::Slot11Thunk (sub_8222FC60)", kThunk, __imp__sub_8222FC60, hooks::ModeSlot11Thunk,
       0, 0, 11, 0x821970C0},
      {"Mode::Slot12Thunk (sub_8222FC70)", kThunk, __imp__sub_8222FC70, hooks::ModeSlot12Thunk,
       0, 0, 12, 0x821977E0},
      {"Mode::Slot13Thunk (sub_8222FC80)", kThunk, __imp__sub_8222FC80, hooks::ModeSlot13Thunk,
       0, 0, 13, 0x82198008},
      {"ModeType2::Interrupt (sub_82197858; sub_82197130)", kInterrupt, __imp__sub_82197858,
       hooks::Type2ModeInterrupt, 96, 16},
      {"ModeType2::ForceExit (sub_82197930)", kForceExit, __imp__sub_82197930,
       hooks::Type2ModeForceExit, 16, 0},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    const int cases = f.kind == kThunk ? 60 : 120;
    for (int c = 0; c < cases; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(uint64_t(c) * 1009 + uint64_t(f.frame) * 7 + f.slot + uint64_t(f.kind) * 31);
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(base, world.address, world.bytes, r);
        WriteRandom(base, mode.address, mode.bytes, r);
        WriteRandom(base, block.address, block.bytes, r);
        WriteRandom(base, sp - 1024, 1024 + 256, r);
        ShapeType2Block(base, block.address, r);
        guest::StoreU32(base, mode.address + 36 + 200, 0);  // no handlers
        guest::StoreU32(base, mode.address + 840, r.Below(4));
        guest::StoreU32(base, mode.address + 844, r.Below(200));
        guest::StoreU32(base, mode.address + t2m::kBlock, block.address);
        guest::StoreU32(base, t2m::kPadBlocked0, r.Below(4) == 0 ? 1 : 0);
        guest::StoreU32(base, t2m::kPadBlocked1, r.Below(4) == 0 ? 1 : 0);
        guest::StoreU32(base, t2m::kLeavingFlag, uint32_t(r.Next()));
        guest::StoreU32(base, t2m::kOptionsSettings, world.address + 0x800);
        guest::StoreU32(base, world.address + 0x800 + 712, r.Below(3) == 0 ? 0 : 1);
        guest::StoreU32(base, t2m::kDialogStatus, r.Below(6));
        const bool waits = f.kind == kWait || (f.kind == kThunk && f.slot == 9);
        if (waits) {
          // The shown index never 3 (FindShown's pick: the car settings,
          // machine 3, not running or starting).
          if (guest::LoadU32(base, block.address + t2m::kShown) == 3) {
            guest::StoreU32(base, block.address + t2m::kShown, 4);
          }
          const uint32_t settings = block.address + 0x173D0 + 9604;
          if (guest::LoadU32(base, settings) == 4 || guest::LoadU32(base, settings) == 5) {
            guest::StoreU32(base, settings, 6);
          }
          WriteRandom(base, extra.address, 0x400, r);
          const uint32_t manager = extra.address, vtable = extra.address + 0x100;
          guest::StoreU32(base, manager, vtable);
          guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
          guest::StoreU32(base, menu_sounds::kSoundManager, manager);
          guest::StoreU32(base, kLoadingCurrent, r.Below(3));
          WriteRandom(base, loading_movie.address, 0xA100, r);
          ui_movies::InitAnimationLists(base, loading_movie.address);
          for (uint32_t k = 0; k < 3; k++) {
            guest::StoreU32(base, kLoadingMovies + 4 * k, loading_movie.address);
          }
        }
        if (f.kind == kTearDown) {
          LayStand(base, stand.address, r);
          if (r.Below(2) == 0) {
            guest::StoreU32(base, stand.address + 12, 1);
            guest::StoreU32(base, stand.address + 16, 1);
          }
        }
        t.r3.u64 = (world.address + 0x100) | (uint64_t(r.Below(2)) << 32);
        t.r4.u64 = mode.address;
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        if (f.kind == kThunk) {
          // The object's vtable at the world's +0x400: the slot the type 2
          // mode's function, or the getter (another mode's).
          const uint32_t vtable = world.address + 0x400;
          guest::StoreU32(base, mode.address, vtable);
          guest::StoreU32(base, vtable + 4 * f.slot, r.Below(4) == 0 ? 0x82090770 : f.target);
          if (f.slot == 11) {
            LayStand(base, stand.address, r);
          }
        }
        if (f.kind == kInterrupt || f.kind == kForceExit) {
          t.r3.u64 = mode.address + 32;
          t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(6);
        }
        std::vector<uint32_t> calls;
        g_mode_calls = &calls;
        const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
            ui_movies_hooks::g_update_callee_stand_in;
        ui_movies_hooks::g_update_callee_stand_in = ModeStandIn;
        if (v == 0) {
          ITC_FLAG(native_type2_mode) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type2_mode) = true;
        } else {
          f.native(t, base);
        }
        ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
        g_mode_calls = nullptr;
        for (const uint32_t word : calls) out[v].Word(word);
        out[v].Bytes(Read(base, mode.address, mode.bytes));
        out[v].Bytes(Read(base, block.address, block.bytes));
        out[v].Bytes(Read(base, world.address, world.bytes));
        for (const uint32_t g : globals) out[v].Word(guest::LoadU32(base, g));
        if (waits) {
          out[v].Bytes(Read(base, extra.address, 0x400));
          out[v].Bytes(Read(base, loading_movie.address, 0xA100));
        }
        if (f.kind == kTearDown || (f.kind == kThunk && f.slot == 11)) {
          out[v].Bytes(Read(base, stand.address, kStandBytes));
        }
        if (f.frame != 0) {
          out[v].Bytes(Read(base, sp - uint32_t(f.frame), uint32_t(f.frame - f.saves)));
        }
        out[v].Bytes(Read(base, sp, 256));
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  for (size_t i = 0; i < std::size(globals); i++) guest::StoreU32(base, globals[i], saved[i]);
}

}  // namespace

// --- ModeType2::Init, laid as the type 6 and 7 modes' slot 0 (ModeInits
// above): the mode a block of random words (its dispatcher's count 0 to 7),
// the stand made as StandScene::Create's cases have it (its holder none or
// held), the UI movies' start and Strings::Load recorded, Archive::Load
// recorded, LoadingScreen::Show over the loading screen's world (a made-up
// time for the tip), the screen fades' words saved and put back; player 0's
// save data random words with its day (+6100: 0, 1, a few, 999 or past it)
// and its garage (+6120, 0 to 20 cars, shaped as the career's harness shapes
// it, its count the halfword +6328) and day sums (Career::NextDay's), the
// mode before 0 to 5 and the race data's race mode (+0xA3AF4) 0 to 3. The
// 14 machines' Inits run through their adapters (native, as in the game) on
// both sides. Compared: the recorded calls, the mode, the screen block
// (VirtualAlloc's pages) and every block the stand made, with the pointers
// into them made offsets; the save data, the loading world, the fades, the
// stand's globals, the frame's +80 to +111, the caller's +20 and r3. ---

namespace {

void ShapeType2Garage(uint8_t* base, uint32_t garage, Random& r, uint32_t count) {
  uint32_t order[50];
  for (uint32_t i = 0; i < 50; i++) order[i] = i;
  for (uint32_t i = 49; i > 0; i--) std::swap(order[i], order[r.Below(i + 1)]);
  const auto link = [&](uint32_t head_at, uint32_t from, uint32_t to) {
    guest::StoreU16(base, garage + head_at, uint16_t(from < to ? order[from] : 0xFFFF));
    for (uint32_t i = from; i < to; i++) {
      guest::StoreU16(base, garage + 4 * order[i], uint16_t(i + 1 < to ? order[i + 1] : 0xFFFF));
      guest::StoreU16(base, garage + 4 * order[i] + 2, uint16_t(r.Below(50)));
    }
  };
  link(204, 0, count);
  link(200, count, 50);
  guest::StoreU16(base, garage + 208, uint16_t(count));
}

// Career::NextDay's day sums (Stats::CloseDay's), as the career's harness
// shapes them: finite floats, small counts and divisors.
void ShapeType2DaySums(uint8_t* base, uint32_t stats, Random& r) {
  for (const uint32_t at : {5928u, 5936u, 5944u}) {
    guest::StoreFloat(base, stats + at, float(int32_t(r.Below(20000)) - 10000) / 8.0f);
    guest::StoreU32(base, stats + at + 4, r.Below(4) == 0 ? 0 : r.Below(100));
  }
  guest::StoreU32(base, stats + 5952, r.Below(500));
  const uint32_t divisors[] = {1, 2, 7, 30};
  guest::StoreU32(base, stats + 5956, divisors[r.Below(4)]);
  guest::StoreU32(base, stats + 5960, r.Below(99999));
  guest::StoreU32(base, stats + 5964, r.Below(99999));
  guest::StoreU16(base, stats + 6028, uint16_t(r.Below(100)));
}

}  // namespace

void Type2ModeInits(Context& h) {
  namespace lw = loading_world;
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue() ||
      !hooks::CarModelsSceneSetUpNative()) {
    ITC_LOG_WARN("harness: type2_mode: the allocation layer, the archive's queue, the models' "
                 "lifetime or memcpy isn't native: ModeType2::Init's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> exits(c_runtime::ExitMutex());
  std::lock_guard<std::recursive_mutex> blocks(models::MemoryBlocksMutex());
  constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x40;
  constexpr uint32_t kSaveSlots = 0x826C5B58, kSaveDataBytes = 0x1F978;
  const std::vector<uint8_t> fades = Read(base, kFades, kFadesBytes);
  const std::vector<uint8_t> slots = Read(base, kSaveSlots, 16);
  const uint32_t mode_before = guest::LoadU32(base, t2m::kModeBefore);
  const uint32_t race_data = guest::LoadU32(base, t2m::kRaceData);
  const StandSaved saved(base);
  const lw::MovieWorld w;
  const Block mode(kModeBytes);
  const Block world(0x1000);
  const Block save(kSaveDataBytes);
  if (!w.ok() || !mode.address || !world.address || !save.address) return;
  Results results("ModeType2::Init (sub_82197278)");
  for (int c = 0; c < 12; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(uint64_t(c) * 811 + 0x82197278);
      saved.Restore();
      const lw::SavedGlobals loading(base);
      lw::ShapeMovieWorld(base, w, r);
      lw::BuildStringTable(base, w);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, mode.address, mode.bytes, r);
      WriteRandom(base, world.address, world.bytes, r);
      WriteRandom(base, sp - 0x1000, 0x1000 + 0x100, r);
      guest::StoreU32(base, mode.address + 36 + 200, r.Below(8));
      const bool held = c % 3 == 2;
      guest::StoreU32(base, t67::kStandHolder, held ? world.address : 0);
      guest::StoreU32(base, scene::kScene, 0);
      guest::StoreU32(base, scene::kFirstSet, 0);
      guest::StoreU32(base, accessories::kSet, 0);
      guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
      guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                        r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
      }
      // Player 0's save data, the mode before and the race mode.
      WriteRandom(base, save.address, save.bytes, r);
      const uint32_t cars = r.Below(4) == 0 ? 0 : r.Below(21);
      ShapeType2Garage(base, save.address + 6120, r, cars);
      ShapeType2DaySums(base, save.address + 48, r);
      static constexpr uint32_t kDays[] = {0, 0, 1, 1, 2, 7, 998, 999, 1000, 5000};
      guest::StoreU32(base, save.address + 6100, kDays[r.Below(std::size(kDays))]);
      guest::StoreU16(base, save.address + 70, uint16_t(r.Below(15)));  // PickNext's gate
      guest::StoreU32(base, save.address + 48 + 5672, r.Below(12));
      for (uint32_t p = 0; p < 4; p++) guest::StoreU32(base, kSaveSlots + 4 * p, save.address);
      guest::StoreU32(base, t2m::kModeBefore, r.Below(6));
      guest::StoreU32(base, t2m::kRaceData, world.address + 0x200 - 0xA3AF4);
      guest::StoreU32(base, world.address + 0x200, r.Below(4));
      const uint32_t mark = exit_table::Mark(base);
      time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
      std::vector<uint32_t> calls;
      g_stand_calls = &calls;
      g_record_mode_calls = true;
      const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = StandRecorder;
      t.r3.u64 = mode.address;
      if (v == 0) {
        ITC_FLAG(native_type2_mode) = false;
        __imp__sub_82197278(t, base);
        ITC_FLAG(native_type2_mode) = true;
      } else {
        hooks::Type2ModeInit(t, base);
      }
      hooks::g_start_up_recorder = saved_recorder;
      g_record_mode_calls = false;
      g_stand_calls = nullptr;
      time_hooks::ClearFakeNow();
      StandTake take;
      take.Collect(base, held);
      const uint32_t block = guest::LoadU32(base, mode.address + t2m::kBlock);
      const bool block_made = memory::IsPages(block);
      if (block_made) {
        take.regions.push_back({block, t2m::kBlockBytes});
        take.AddHeapFrom(base, block, t2m::kBlockBytes);
      }
      // The recorded calls' frames and return addresses: r1 by its offset;
      // the screen start's r4 (it reads r3 only: the original's caller's
      // leftover, 0 in ours) left out.
      for (size_t k = 0; k + 5 <= calls.size(); k += 5) {
        if (calls[k] != 0x8209DE48) calls[k + 3] -= t.r1.u32;
        if (calls[k] == 0x8211AD00) calls[k + 2] = 0;
      }
      take.TakeCalls(base, calls, out[v]);
      take.TakeBlocks(base, out[v]);
      out[v].Word(block_made);
      if (block_made) {
        out[v].Bytes(scene_pools_take::Normalized(base, take.regions, block, t2m::kBlockBytes));
      }
      out[v].Bytes(scene_pools_take::Normalized(base, take.regions, mode.address, mode.bytes));
      out[v].Bytes(scene_pools_take::Normalized(base, take.regions, world.address, world.bytes));
      saved.Take(take.regions, out[v]);
      out[v].Bytes(Read(base, save.address, save.bytes));
      out[v].Bytes(Read(base, sp - t2m::kInitFrame + 80, 32));
      out[v].Word(guest::LoadU32(base, sp + 20));
      out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
      out[v].Bytes(Read(base, w.strings.address, w.strings.bytes));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingShown));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
      out[v].Bytes(Read(base, kFades, kFadesBytes));
      exit_table::Take(base, mark, out[v]);
      out[v].Word(take.Word(t.r3.u32, t.r1.u32));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      take.Free();
      if (block_made) memory::FreePages(block);
      loading.Restore();
      Write(base, kFades, fades.data(), kFadesBytes);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  saved.Restore();
  Write(base, kFades, fades.data(), kFadesBytes);
  Write(base, kSaveSlots, slots.data(), 16);
  guest::StoreU32(base, t2m::kModeBefore, mode_before);
  guest::StoreU32(base, t2m::kRaceData, race_data);
}

// --- ModeType12::Init (the type 12 mode, docs/functions/network-mode.md;
// run by the network_mode harness), laid as ModeType2::Init's above: the
// mode a block of random words (its dispatcher's count 0 to 7), the stand
// made as StandScene::Create's cases have it (its holder none or held), the
// UI movies' start and Strings::Load recorded, Archive::Load recorded,
// LoadingScreen::Show over the loading screen's world (a made-up time for the
// tip), the screen fades' words saved and put back; the mode before 0 to 5
// (1, back from a race, often), the session [0x82816E3C] null (play: no
// Live) or a block of the case's with its +240 and +244 each 0 or not, and
// the network's state [0x8281611C] 0, 1 or another value. The original runs
// with --native_network_mode as the game has it (its InitStates then the
// adapters' or, once those are gone, the recompiled ones; the functors'
// stack leftovers in a group of their own). Compared: the
// recorded calls, the mode, the type 12 block (VirtualAlloc's pages) and
// every block the stand made, with the pointers into them made offsets; the
// session's block, the loading world, the fades, the stand's globals, the
// frame's +80 to +119, the caller's +20 and r3. ---

void NetworkModeInits(Context& h) {
  namespace lw = loading_world;
  namespace nm = network_mode;
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue() ||
      !hooks::CarModelsSceneSetUpNative()) {
    ITC_LOG_WARN("harness: network_mode: the allocation layer, the archive's queue, the models' "
                 "lifetime or memcpy isn't native: ModeType12::Init's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> exits(c_runtime::ExitMutex());
  std::lock_guard<std::recursive_mutex> blocks(models::MemoryBlocksMutex());
  constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x40;
  const std::vector<uint8_t> fades = Read(base, kFades, kFadesBytes);
  const uint32_t globals[] = {nm::kModeBefore, nm::kSession, nm::kNetworkState};
  uint32_t kept[std::size(globals)];
  for (size_t g = 0; g < std::size(globals); g++) kept[g] = guest::LoadU32(base, globals[g]);
  const StandSaved saved(base);
  const lw::MovieWorld w;
  const Block mode(kModeBytes);
  const Block world(0x1000);
  if (!w.ok() || !mode.address || !world.address) return;
  const uint32_t session = world.address + 0x400;
  Results results("ModeType12::Init (sub_82278058)");
  // The block's words that are stack leftovers: the InitStates copy three
  // frame words they never write into each transition's functor (its +32
  // to +40; the transitions 48 bytes from the machine's +0, their count at
  // +9600), dead words; compared on their own (expected to differ: the
  // original's InitStates run recompiled since their adapters went, and
  // leave register saves and return addresses in the stack there, from
  // NetScreen::InitBase's and the base Inits' calls, that later InitStates
  // copy; the plain C++ writes neither), as Type6Mode::Init's.
  Results leftovers(
      "ModeType12::Init (sub_82278058), the transitions' stack leftovers (expected to differ)");
  for (int c = 0; c < 16; c++) {
    Outputs out[2];
    std::vector<uint8_t> made[2];
    std::vector<uint32_t> masked[2];  // the leftovers' offsets in the block
    for (int v = 0; v < 2; v++) {
      Random r(uint64_t(c) * 811 + 0x82278058);
      saved.Restore();
      const lw::SavedGlobals loading(base);
      lw::ShapeMovieWorld(base, w, r);
      lw::BuildStringTable(base, w);
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, mode.address, mode.bytes, r);
      WriteRandom(base, world.address, world.bytes, r);
      WriteRandom(base, sp - 0x1000, 0x1000 + 0x100, r);
      const uint32_t handlers = r.Below(8);
      guest::StoreU32(base, mode.address + 36 + 200, handlers);
      const bool held = c % 3 == 2;
      guest::StoreU32(base, t67::kStandHolder, held ? world.address : 0);
      guest::StoreU32(base, scene::kScene, 0);
      guest::StoreU32(base, scene::kFirstSet, 0);
      guest::StoreU32(base, accessories::kSet, 0);
      guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
      guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                        r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
      }
      // The mode before, the session and the network's state.
      guest::StoreU32(base, nm::kModeBefore, r.Below(2) == 0 ? 1 : r.Below(6));
      const bool live = c % 2 == 1;
      guest::StoreU32(base, nm::kSession, live ? session : 0);
      for (const uint32_t at : {240u, 244u}) {
        guest::StoreU32(base, session + at, r.Below(2) == 0 ? 0 : 1 + r.Below(7));
      }
      const uint32_t states[] = {0, 1, 2, uint32_t(r.Next())};
      guest::StoreU32(base, nm::kNetworkState, states[r.Below(4)]);
      const uint32_t mark = exit_table::Mark(base);
      time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
      std::vector<uint32_t> calls;
      g_stand_calls = &calls;
      g_record_mode_calls = true;
      const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = StandRecorder;
      t.r3.u64 = mode.address;
      if (v == 0) {
        __imp__sub_82278058(t, base);
      } else {
        hooks::ModeType12Init(t, base);
      }
      hooks::g_start_up_recorder = saved_recorder;
      g_record_mode_calls = false;
      g_stand_calls = nullptr;
      time_hooks::ClearFakeNow();
      StandTake take;
      take.Collect(base, held);
      const uint32_t block = guest::LoadU32(base, mode.address + nm::kBlock);
      const bool block_made = memory::IsPages(block);
      if (block_made) {
        take.regions.push_back({block, nm::kBlockBytes});
        take.AddHeapFrom(base, block, nm::kBlockBytes);
      }
      // The recorded calls' frames and return addresses: r1 by its offset;
      // the screen start's r4 (it reads r3 only: the original's caller's
      // leftover, 0 in ours) left out.
      for (size_t k = 0; k + 5 <= calls.size(); k += 5) {
        if (calls[k] != 0x8209DE48) calls[k + 3] -= t.r1.u32;
        if (calls[k] == 0x8211AD00) calls[k + 2] = 0;
      }
      take.TakeCalls(base, calls, out[v]);
      take.TakeBlocks(base, out[v]);
      out[v].Word(block_made);
      if (block_made) {
        made[v] = scene_pools_take::Normalized(base, take.regions, block, nm::kBlockBytes);
        // The machines the Init added to the dispatcher.
        const uint32_t added = guest::LoadU32(base, mode.address + 36 + 200);
        for (uint32_t k = handlers; k < added && k < handlers + 16; k++) {
          const uint32_t machine = guest::LoadU32(base, mode.address + 36 + 4 * k);
          if (machine < block || machine - block >= nm::kBlockBytes) continue;
          const uint32_t count = guest::LoadU32(base, machine + game_modes::kTransitionCount);
          for (uint32_t e = 0; e < count && e < 200; e++) {
            for (const uint32_t word : {32u, 36u, 40u}) {
              masked[v].push_back(machine - block + 48 * e + word);
            }
          }
        }
      }
      out[v].Bytes(scene_pools_take::Normalized(base, take.regions, mode.address, mode.bytes));
      out[v].Bytes(scene_pools_take::Normalized(base, take.regions, world.address, world.bytes));
      saved.Take(take.regions, out[v]);
      for (const uint32_t g : globals) out[v].Word(guest::LoadU32(base, g));
      out[v].Bytes(Read(base, sp - nm::kInitFrame + 80, 40));
      out[v].Word(guest::LoadU32(base, sp + 20));
      out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
      out[v].Bytes(Read(base, w.strings.address, w.strings.bytes));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingShown));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
      out[v].Bytes(Read(base, kFades, kFadesBytes));
      exit_table::Take(base, mark, out[v]);
      out[v].Word(take.Word(t.r3.u32, t.r1.u32));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      take.Free();
      if (block_made) memory::FreePages(block);
      loading.Restore();
      Write(base, kFades, fades.data(), kFadesBytes);
    }
    Outputs left[2];
    for (int v = 0; v < 2; v++) {
      std::vector<uint8_t> kept = made[v];
      out[v].Word(uint32_t(masked[v].size()));
      for (const uint32_t o : masked[v]) {
        if (o + 4 > kept.size()) continue;
        out[v].Word(o);
        left[v].Bytes(std::vector<uint8_t>(kept.begin() + o, kept.begin() + o + 4));
        std::fill(kept.begin() + o, kept.begin() + o + 4, 0);
      }
      out[v].Bytes(kept);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
    leftovers.Check(left[0], left[1], fmt::format("case {}", c));
  }
  results.Report();
  leftovers.Report();
  saved.Restore();
  Write(base, kFades, fades.data(), kFadesBytes);
  for (size_t g = 0; g < std::size(globals); g++) guest::StoreU32(base, globals[g], kept[g]);
}

// --- ModeType12::Teardown (registered under network_mode): the mode (its
// ring's count and start small, no handlers) and the type 12 block in
// blocks of their own (the shown index one of play's, the next answer at
// +0x21014 random, the session made here (+0x21024) 0 or 1), the answer in
// the world, the stand as PollTeardown's cases lay it (the teardown under
// way with its count 1 half the time, so it answers 1), the sound manager
// a fake (its StopCue, slot 13, the recorder of r4 and r5 at +444), the
// session none or, now and then, the layer's pages made for the side
// (Session::Free frees them when the poll answers 1 and +0x21024 is set).
// Compared: the mode, the block, the answer, the stand's block, the
// manager, the frame without its register saves, whether the session is
// still allocated, the globals and r3. ---

void NetworkModeTeardowns(Context& h) {
  namespace nm = network_mode;
  uint8_t* const base = h.base;
  const Block world(0x1000);
  const Block mode(0x400);
  const Block block(nm::kBlockBytes);
  const Block stand(kStandBytes);
  if (!world.address || !mode.address || !block.address || !stand.address) return;
  const uint32_t globals[] = {kStandGlobals[0], kStandGlobals[1],    kStandGlobals[2],
                              kStandGlobals[3], menu_sounds::kSoundManager, nm::kSession,
                              nm::kSessionBuffer};
  uint32_t kept[std::size(globals)];
  for (size_t g = 0; g < std::size(globals); g++) kept[g] = guest::LoadU32(base, globals[g]);
  static constexpr uint32_t kShownIndexes[] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
  const uint32_t sp = CallContext(h).r1.u32;
  Results results("ModeType12::Teardown (sub_8227BE10)");
  for (int c = 0; c < 64; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(uint64_t(c) * 7919 + 0x8227BE10);
      WriteRandom(base, world.address, world.bytes, r);
      WriteRandom(base, mode.address, mode.bytes, r);
      WriteRandom(base, block.address, block.bytes, r);
      WriteRandom(base, sp - 1024, 1024 + 256, r);
      LayStand(base, stand.address, r);
      if (r.Below(2) == 0) {
        guest::StoreU32(base, stand.address + 12, 1);
        guest::StoreU32(base, stand.address + 16, 1);
      }
      guest::StoreU32(base, mode.address + nm::kRing + 600, r.Below(40));
      guest::StoreU32(base, mode.address + nm::kRing + 604, r.Below(60));
      guest::StoreU32(base, mode.address + nm::kDispatcher + 200, 0);
      guest::StoreU32(base, mode.address + nm::kBlock, block.address);
      guest::StoreU32(base, block.address + nm::kShown,
                      kShownIndexes[r.Below(std::size(kShownIndexes))]);
      guest::StoreU32(base, block.address + nm::kSessionMade, r.Below(2));
      const uint32_t manager = world.address + 0x800, vtable = manager + 0x100;
      guest::StoreU32(base, manager, vtable);
      guest::StoreU32(base, vtable + 52, 0x820CE9F8);  // r4, r5 to +444, +448
      guest::StoreU32(base, menu_sounds::kSoundManager, manager);
      uint32_t session = 0;
      if (hooks::NativeHeapActive() && r.Below(3) == 0) {
        session = memory::AllocPages(0x13D000, 4096);
        guest::StoreU32(base, session + nm::kSessionRows, 0);
      }
      guest::StoreU32(base, nm::kSession, session);
      guest::StoreU32(base, nm::kSessionBuffer, 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = (world.address + 0x100) | (uint64_t(r.Below(2)) << 32);
      t.r4.u64 = mode.address;
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        ITC_FLAG(native_network_mode) = false;
        __imp__sub_8227BE10(t, base);
        ITC_FLAG(native_network_mode) = true;
      } else {
        hooks::ModeType12Teardown(t, base);
      }
      const uint32_t now = guest::LoadU32(base, nm::kSession);
      out[v].Word(now != 0 && now == session ? 1 : now);
      guest::StoreU32(base, nm::kSession, 0);
      if (session != 0) {
        const bool pages = memory::IsPages(session);
        out[v].Word(pages ? 1 : 0);
        if (pages) memory::FreePages(session);
      }
      out[v].Bytes(Read(base, mode.address, mode.bytes));
      out[v].Bytes(Read(base, block.address, block.bytes));
      out[v].Bytes(Read(base, world.address, world.bytes));
      out[v].Bytes(Read(base, stand.address, kStandBytes));
      out[v].Bytes(Read(base, sp - 144, 144 - 48));
      out[v].Bytes(Read(base, sp, 256));
      for (const uint32_t g : globals) out[v].Word(guest::LoadU32(base, g));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  for (size_t g = 0; g < std::size(globals); g++) guest::StoreU32(base, globals[g], kept[g]);
}

void Type2Mode(Context& h) {
  const Block world(kWorldBytes);
  if (!world.address) {
    ITC_LOG_ERROR("harness: type2_mode: no block from the allocation layer");
    return;
  }
  Type2Blocks(h);
  Type2Delegates(h, world);
  Type2ModeInits(h);
}


// --- The type 8 mode's dialog (src/type8_mode/, docs/functions/type8-mode.md;
// called by the type8_mode harness, here for the dialogs' worlds this file
// lays): its leaves as the type 6 and 7 dialogs' Handler cases, OpenMovies
// with the opens recorded, and its pages, Update, Draw, opens and step 0 as
// the Dialogs cases (movies 16, 19, 20 and 22 in the table). ---

void Type8Dialogs(Context& h) {
  uint8_t* const base = h.base;
  const Block world(kWorldBytes);
  const Block movie(0xA000);
  if (!world.address || !movie.address) {
    ITC_LOG_ERROR("harness: type8_mode: no block from the allocation layer");
    return;
  }
  const uint32_t saved_movie = g_movie;
  g_movie = movie.address;
  g_handler_flag = &ITC_FLAG(native_type8_mode);
  const HandlerArgs plain = [](PPCContext&, uint32_t) {};
  auto maybe = [base](Random& r, uint32_t address) {
    guest::StoreU32(base, address, r.Below(2) ? 0 : 1 + r.Below(5));
  };
  Handler(h, world, "Type8Dialog::OnEnter", 0x8223A408, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9612); }, plain, __imp__sub_8223A408,
          hooks::Type8DialogOnEnter);
  Handler(h, world, "Type8Dialog::WaitForMovies", 0x8223ADC0, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9608); }, plain, __imp__sub_8223ADC0,
          hooks::Type8DialogWaitForMovies);
  Handler(h, world, "Type8Dialog::OpenNotice", 0x8223A4A0, 8,
          [maybe](Random& r, uint32_t w) { maybe(r, w + 9608); }, plain, __imp__sub_8223A4A0,
          hooks::Type8DialogOpenNotice);
  g_handler_flag = nullptr;
  g_movie = saved_movie;
  {
    // OpenMovies: the five opens recorded.
    Results results("Type8Dialog::OpenMovies (sub_8223A398)");
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      g_opens.clear();
      PPCContext t = CallContext(h);
      if (v == 0) {
        const hooks::StartUpRecorder saved = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = RecordOpen;
        ITC_FLAG(native_type8_mode) = false;
        __imp__sub_8223A398(t, base);
        ITC_FLAG(native_type8_mode) = true;
        hooks::g_start_up_recorder = saved;
      } else {
        t67::Calls calls = hooks::Type67Calls(t, base);
        calls.open = [](uint32_t id, uint32_t asset, uint32_t frame, uint32_t lr) -> uint64_t {
          g_opens.insert(g_opens.end(), {id, asset, frame, lr});
          return 1;
        };
        t.r3.u64 = type8_mode::DialogOpenMovies(base, t.r1.u32, calls);
      }
      for (const uint32_t word : g_opens) out[v].Word(word);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], "the five opens");
    results.Report();
  }
  const DialogFunction functions[] = {
      {"Type8Dialog::ConfirmPage", 0x8223A6B8, 112, 32, __imp__sub_8223A6B8,
       hooks::Type8DialogConfirmPage, kPage, true, true},
      {"Type8Dialog::NoticePage", 0x8223A858, 112, 32, __imp__sub_8223A858,
       hooks::Type8DialogNoticePage, kPage, true, true},
      {"Type8Dialog::Update", 0x8223AC58, 160, 32, __imp__sub_8223AC58,
       hooks::Type8DialogUpdate, kUpdate, true, true},
      {"Type8Dialog::Draw", 0x8223AC00, 96, 8, __imp__sub_8223AC00, hooks::Type8DialogDraw,
       kDraw, true, true},
      {"Type8Dialog::OpenConfirm", 0x8223A510, 112, 24, __imp__sub_8223A510,
       hooks::Type8DialogOpenConfirm, kStep, true, true},
      {"Type8Dialog::DrawWaiting", 0x8223A5F0, 128, 32, __imp__sub_8223A5F0,
       hooks::Type8DialogDrawWaiting, kStep, true, true},
      {"Type8Dialog::OpenConfirmStep", 0x8223A940, 128, 40, __imp__sub_8223A940,
       hooks::Type8DialogOpenConfirmStep, kStep, true, true},
  };
  RunDialogs(h, world, functions, std::size(functions));
}

}  // namespace harness
