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

#include "base/flags.h"
#include "archive/archive.h"
#include "base/log.h"
#include "cameras/cameras.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "hooks/type67_modes.h"
#include "memory/heap.h"
#include "type67_modes/type67_modes.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <limits>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type67_modes);

namespace harness {

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

void Handler(Context& h, const Block& world, const char* name, uint32_t address, int cases,
             const HandlerSetup& setup, const HandlerArgs& args, GuestFunction recompiled,
             const Native& native, const HandlerCollect& collect = {}) {
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
        ITC_FLAG(native_type67_modes) = false;
        recompiled(t, h.base);
        ITC_FLAG(native_type67_modes) = true;
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

// --- The modes' delegate functions: a mode block (its dispatcher without
// handlers, so GameMessages::Dispatch takes the queued messages and
// delivers none; its ring's count and start small) whose +848 is a screen
// block with the checked machines' states picked (10, 6 and 11 among
// them), the answer in the world, the original's frame random words; the
// mode, the block, the globals (the leaving flag, the cars' words), the
// answer, the frame's +80 to +87 and r3 compared. ---

constexpr uint32_t kModeBytes = 1024;
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
  };
  for (const Function& f : functions) {
    Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
    const uint32_t frame_bytes = f.next ? t67::kNextModeFrame : t67::kLeavingFrame;
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
        if (!f.next) {
          const uint32_t kList[] = {6, 6, 11, 11, 10, 5};
          put(layout.list, kList[r.Below(6)]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = world.address + 0x100;
        t.r4.u64 = mode.address;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          f.recompiled(t, base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          f.native(t, base);
        }
        out[v].Bytes(Read(base, mode.address, mode.bytes));
        out[v].Bytes(Read(base, block.address, block.bytes));
        for (const uint32_t g : globals) out[v].Word(guest::LoadU32(base, g));
        out[v].Bytes(Read(base, world.address + 0x100, 8));
        out[v].Bytes(Read(base, frame + 80, 8));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  for (int i = 0; i < 3; i++) guest::StoreU32(base, globals[i], saved[i]);
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

}  // namespace

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
  GarageLights(h, world);
  StandSlots(h, world);
}

}  // namespace harness
