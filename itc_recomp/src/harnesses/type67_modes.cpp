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
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/heap.h"
#include "hooks/type67_modes.h"
#include "memory/heap.h"
#include "type67_modes/type67_modes.h"

#include <fmt/format.h>

#include <functional>
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
      guest::StoreU32(h.base, t67::kControllerData, 0);
      guest::StoreU32(h.base, t67::kControllerPage, 0);
      guest::StoreU32(h.base, t67::kModeType, r.Below(10));
      guest::StoreU32(h.base, Globals::kMissingMovie, g_movie);
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
}

}  // namespace harness
