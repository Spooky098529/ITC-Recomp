// PanelSet's pages and steps (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "memory/heap.h"
#include "race_setup/race_setup.h"
#include "tuning_pages/tuning_pages.h"
#include "type67_modes/type67_modes.h"

#include <rex/platform/fpscr.h>

#include <climits>
#include <cmath>
#include <utility>

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// VirtualFree(block, 0, MEM_RELEASE): the allocation layer's pages freed
// (1), else a guest call to its override.
uint64_t FreePages(uint32_t block, uint32_t frame, uint32_t lr, const Calls& calls) {
  if (memory::IsPages(block)) {
    return memory::FreePages(block) ? 1 : 0;
  }
  return calls.Call(kVirtualFree, frame, lr, block, 0, 0x8000);
}

// The fade the steps start: [0x82815EB0] = 1, then ScreenFades::FadeOut or
// FadeIn([passes], 0, 0, 0, 0).
uint64_t Fade(uint8_t* base, uint32_t function, uint32_t passes, uint32_t frame, uint32_t lr,
              const Calls& calls) {
  const uint32_t count = LoadU32(base, passes);
  StoreU32(base, kFadeWait, 1);
  return calls.Call(function, frame, lr, count, 0, 0, 0);
}

}  // namespace

uint64_t ToTabs(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249558 PanelSetBase::ToTabs");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t page = LoadU32(base, b + 32);
  if (page != 0) {
    StoreU32(base, b + 40, page);
  }
  StoreU32(base, b + 44, 0);
  StoreU32(base, b + 48, 1);
  StoreU32(base, b + 36, 0);
  StoreU32(base, b + 32, 0xFFFFFFFFu);
  StoreU32(base, LoadU32(base, b + 1900) + 152, 0);
  if (LoadU32(base, b + 1936) == 0) {
    return b_r3;
  }
  StoreU32(base, frame + 84, 159);
  StoreU32(base, frame + 92, 158);
  // The menu's address as addi leaves it (r3's high half kept).
  const uint64_t args[8] = {b_r3 + 60, 3, 3, 1, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, 0x822495E8, args);
  return calls.Call(kMenuListSetSelection, frame, 0x822495F4, b_r3 + 60, LoadU32(base, b + 232));
}

uint64_t Open(uint8_t* base, uint64_t set, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249730 PanelSet::Open");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t b_r3 = set + 4;
  const uint32_t b = uint32_t(b_r3);
  for (const uint32_t at : {8u, 12u, 16u, 20u, 24u}) StoreU32(base, b + at, 0);
  StoreU32(base, b + 1940, 1);
  StoreU32(base, b + 1948, 1);
  for (const uint32_t at : {36u, 32u, 40u, 52u, 44u}) StoreU32(base, b + at, 0xFFFFFFFFu);
  StoreU32(base, b + 56, 0);
  StoreU32(base, b + 1820, 0);
  for (const uint32_t at : {228u, 232u, 236u, 240u, 244u, 248u}) StoreU32(base, b + at, 0);
  const uint32_t kind = LoadU32(base, uint32_t(set) + 1932);
  StoreU32(base, b + 52, 0xFFFFFFFFu);
  StoreU32(base, b + 44, 0xFFFFFFFFu);
  StoreU32(base, b + 56, 0);
  uint64_t r3 = b_r3;
  if (kind == 1) {
    StoreU32(base, b + 36, 0);
    StoreU32(base, b + 32, 0);
    StoreU32(base, b + 40, 0);
    const uint32_t tab = LoadU32(base, b + 1880);
    StoreU32(base, b + 232, tab);
    StoreU32(base, b + 228, tab);
    r3 = ToTabs(base, b_r3, frame, calls);
  } else {
    StoreU32(base, b + 36, 1);
    StoreU32(base, b + 32, 1);
    StoreU32(base, b + 40, 1);
    StoreU32(base, b + 228, 0);
    StoreU32(base, b + 232, 0);
    StoreU32(base, b + 1820, b + 272);
    const uint32_t page = LoadU32(base, b + 32);
    if (page != 1) {
      StoreU32(base, b + 40, page);
    }
    StoreU32(base, b + 36, 1);
    StoreU32(base, b + 32, 0xFFFFFFFFu);
    StoreU32(base, b + 44, 4);
    StoreU32(base, b + 48, 6);
    StoreU32(base, LoadU32(base, b + 1900) + 152, 1);
  }
  StoreU32(base, uint32_t(set) + 1944, 0);
  return r3;
}

uint64_t Free(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B538 PanelSetBase::Free");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, b + 1820, 0);
  // Each list (its entries from +0, the count at `count`) freed from the
  // last entry down.
  const auto free_list = [&](uint32_t list, uint32_t count, uint32_t whole, uint32_t lr) {
    while (LoadU32(base, b + whole) != 0) {
      const uint32_t n = LoadU32(base, list + count) - 1;
      StoreU32(base, list + count, n);
      FreePages(LoadU32(base, list + 4 * n), frame, lr, calls);
    }
  };
  free_list(b + 272, 512, 784, 0x8224B588);
  free_list(b + 788, 512, 1300, 0x8224B5C4);
  free_list(b + 1304, 512, 1816, 0x8224B600);
  free_list(b + 1824, 52, 1876, 0x8224B63C);
  calls.Call(kMenuListRelease, frame, 0x8224B650, b_r3 + 60);
  return calls.Call(kMenuListRelease, frame, 0x8224B658, b_r3 + 132);
}

uint64_t BackPage8(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A3F8 PanelSetBase::BackPage8");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t confirm = LoadU32(base, b + 1892);
  if (confirm != 0) {
    StoreU32(base, confirm + 4, LoadU32(base, confirm + 12));
  }
  const uint64_t r3 = Fade(base, kFadeIn, kFadeInPasses, frame, 0x8224A448, calls);
  const uint32_t page = LoadU32(base, b + 32);
  if (page != 2) {
    StoreU32(base, b + 40, page);
  }
  StoreU32(base, b + 44, 9);
  StoreU32(base, b + 48, 11);
  StoreU32(base, b + 36, 2);
  StoreU32(base, b + 32, 0xFFFFFFFFu);
  return r3;
}

uint64_t HandleInput(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224BF30 PanelSetBase::HandleInput");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 176);
  // The handlers' table on the frame (+80 to +112), two words 0 after it.
  static constexpr uint32_t kPages[9] = {0x82249EB8, 0x8224A178, 0x8224B8D8,
                                         0x8224AEA8, 0x82248E60, 0x82248EF0,
                                         0x8224BDC0, 0x8224A368, 0x8224A3F8};
  const uint32_t page = LoadU32(base, b + 32);
  for (uint32_t i = 0; i < 9; i++) {
    StoreU32(base, frame + 80 + 4 * i, kPages[i]);
  }
  StoreU32(base, frame + 116, 0);
  StoreU32(base, frame + 120, 0);
  StoreU32(base, b + 1936, page != LoadU32(base, b + 1932) ? 1 : 0);
  // r3 as the original's lis and addi leave it (sign-extended) when no
  // handler runs.
  uint64_t r3 = 0xFFFFFFFF8224B8D8ull;
  if (page != 0xFFFFFFFFu) {
    const uint32_t handler = LoadU32(base, frame + 80 + 4 * page);
    switch (handler) {
      case 0x82249EB8:
        r3 = TabsPage(base, b_r3, frame, calls);
        break;
      case 0x8224A178:
        r3 = RowsPage(base, b_r3, frame, calls);
        break;
      case 0x8224AEA8:
        r3 = ColorsPage(base, b_r3, frame, calls);
        break;
      case 0x8224BDC0:
        r3 = PowerGraphPage(base, b_r3, frame, calls);
        break;
      case 0x82248EF0:
        r3 = PickedPage(base, b_r3);
        break;
      case 0x8224A3F8:
        r3 = BackPage8(base, b_r3, frame, calls);
        break;
      default:
        r3 = calls.Call(handler, frame, 0x8224BFF8, b_r3);
        break;
    }
  }
  StoreU32(base, b + 1932, page);
  return r3;
}

uint64_t Step2(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248F00 PanelSetBase::Step2");
  const uint32_t frame = Frame(base, sp, 128);
  if (LoadU32(base, word) != 0) {
    return 1;
  }
  const uint32_t tabs = LoadU32(base, b + 1896);
  if (tabs != 0) {
    const uint32_t moves = LoadU32(base, b + 84);
    const uint64_t before = calls.Call(kMenuListPrevious, frame, 0x82248F3C, b + 60);
    const uint64_t tab = calls.Call(kMenuListSelection, frame, 0x82248F48, b + 60);
    TabMove(base, LoadU32(base, b + 1896), tab, before, moves, frame, calls);
  }
  StoreU32(base, word, 1);
  return 1;
}

uint64_t Step3(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A718 PanelSetBase::Step3");
  const uint32_t frame = Frame(base, sp, 112);
  if (LoadU32(base, word) == 0) {
    if (LoadU32(base, b + 1948) != 0) {
      Fade(base, kFadeOut, kFadeOutPasses, frame, 0x8224A76C, calls);
    }
    // The empty sub_82106540 on F (its override does nothing).
    StoreU32(base, word, 1);
  }
  uint32_t waited = 1;
  uint64_t r3 = 1;
  if (LoadU32(base, b + 1948) != 0) {
    waited = LoadU32(base, kFadeWait) == 0 ? 1 : 0;
  }
  const uint32_t tabs = LoadU32(base, b + 1896);
  if (tabs != 0) {
    r3 = calls.Call(kTabFinished, frame, 0x8224A7B8, tabs);
  }
  return waited != 0 && uint32_t(r3) != 0 ? 1 : 0;
}

uint64_t Step7(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249098 PanelSetBase::Step7");
  const uint32_t frame = Frame(base, sp, 144);
  if (LoadU32(base, word) != 0) {
    return 1;
  }
  if (LoadU32(base, b + 1900) != 0) {
    const uint32_t list = b + 60;
    const uint32_t step = LoadU32(base, b + 84);
    const uint32_t scroll = LoadU32(base, b + 64);
    const uint64_t moves = uint64_t(LoadU32(base, list + 4)) - uint64_t(LoadU32(base, list + 8));
    const uint64_t before = calls.Call(kMenuListPrevious, frame, 0x822490E4, list);
    const uint64_t row = calls.Call(kMenuListSelection, frame, 0x822490F0, list);
    RowMove(base, LoadU32(base, b + 1900), row, before, moves, scroll, LoadU32(base, b + 1820),
            frame, calls);
    (void)step;  // r8, which RowPanel::Move doesn't read
  }
  StoreU32(base, word, 1);
  return 1;
}

uint64_t Step11(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A7E0 PanelSetBase::Step11");
  const uint32_t frame = Frame(base, sp, 112);
  if (LoadU32(base, word) != 0) {
    return 1;
  }
  if (LoadU32(base, b + 1908) != 0) {
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224A818, b + 60);
    StoreU32(base, LoadU32(base, b + 1908) + 8, uint32_t(item));
    const uint32_t count = LoadU32(base, LoadU32(base, b + 1820) + 512);
    CarSelect(base, LoadU32(base, b + 1908), count > 1 ? 1 : 0, frame, calls);
  }
  StoreU32(base, word, 1);
  return 1;
}

uint64_t Step18(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249258 PanelSetBase::Step18");
  const uint32_t frame = Frame(base, sp, 112);
  if (LoadU32(base, word) != 0) {
    return 1;
  }
  if (LoadU32(base, b + 1904) != 0) {
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x82249294, b + 60);
    ColorSelect(base, LoadU32(base, b + 1904), item, frame, calls);
  }
  StoreU32(base, word, 1);
  return 1;
}

uint64_t Step26(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A8E0 PanelSetBase::Step26");
  const uint32_t frame = Frame(base, sp, 112);
  if (LoadU32(base, word) == 0) {
    if (LoadU32(base, b + 1948) != 0) {
      Fade(base, kFadeOut, kFadeOutPasses, frame, 0x8224A934, calls);
    }
    StoreU32(base, word, 1);
  }
  const uint32_t done = LoadU32(base, kFadeWait) == 0 ? 1 : 0;
  if (done == 0) {
    return 0;
  }
  if (LoadU32(base, b + 1904) != 0) {
    PartFinishAll(base, LoadU32(base, b + 1904), frame, calls);
  }
  if (LoadU32(base, b + 1896) != 0) {
    PartFinishAll(base, LoadU32(base, b + 1896), frame, calls);
  }
  return done;
}

namespace {

// MenuList::Init(list, count, shown, 1, a, b, c, d) with the two words the
// original stores at its frame's +84 (159) and +92 (158).
uint64_t MenuInit(uint8_t* base, uint64_t list, uint64_t count, uint64_t shown,
                  const uint32_t (&ids)[4], uint32_t frame, uint32_t lr, const Calls& calls) {
  StoreU32(base, frame + 92, 158);
  StoreU32(base, frame + 84, 159);
  const uint64_t args[8] = {list, count, shown, 1, ids[0], ids[1], ids[2], ids[3]};
  return calls.CallArgs(kMenuListInit, frame, lr, args);
}

}  // namespace

uint64_t SetUpRows(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A978 PanelSetBase::SetUpRows");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t save = uint32_t(SaveData(base, b, frame));
  // The count: the garage's (a halfword, sign-extended) for tab 2, else the
  // list's.
  const uint64_t count =
      LoadU32(base, b + 232) == 2
          ? uint64_t(int64_t(int16_t(guest::LoadU16(base, save + 6328))))
          : uint64_t(LoadU32(base, LoadU32(base, b + 1820) + 512));
  uint64_t shown = LoadU32(base, b + 1916);
  static constexpr uint32_t kRows[4] = {154, 153, 155, 156};
  static constexpr uint32_t kList[4] = {162, 161, 153, 154};
  if (LoadU32(base, kTwoPlayers) != 0) {
    if (!(int32_t(uint32_t(count)) > int32_t(uint32_t(shown)))) {
      shown = count;
    }
    MenuInit(base, b_r3 + 60, count, shown, kRows, frame, 0x8224A9FC, calls);
    MenuInit(base, b_r3 + 132, count, count, kList, frame, 0x8224AA28, calls);
    StoreU32(base, b + 192, LoadU32(base, b + 1952) != 0 ? 1 : 0);
  } else if (int32_t(uint32_t(count)) < int32_t(uint32_t(shown))) {
    MenuInit(base, b_r3 + 60, count, count, kRows, frame, 0x8224AA80, calls);
    MenuInit(base, b_r3 + 132, count, count, kList, frame, 0x8224AAAC, calls);
  } else {
    MenuInit(base, b_r3 + 60, count, shown, kRows, frame, 0x8224AABC, calls);
    MenuInit(base, b_r3 + 132, count, count, kList, frame, 0x8224AAE8, calls);
  }
  calls.Call(kMenuListSetSelection, frame, 0x8224AAF4, b_r3 + 60, LoadU32(base, b + 236));
  StoreU32(base, b + 64, LoadU32(base, b + 240));
  calls.Call(kMenuListFit, frame, 0x8224AB08, b_r3 + 60, 1);
  if (LoadU32(base, b + 1900) != 0) {
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224AB1C, b_r3 + 60);
    RowFill(base, LoadU32(base, b + 1900), LoadU32(base, b + 1820), LoadU32(base, b + 64), item,
            frame, calls);
  }
  const uint32_t rows = LoadU32(base, b + 1900);
  if (rows == 0) {
    return 0;
  }
  return RowRefresh(base, rows, LoadU32(base, b + 1820), LoadU32(base, b + 64), frame, calls);
}

uint64_t SetUpCarPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224C560 PanelSetBase::SetUpCarPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 128);
  static constexpr uint32_t kActions[4] = {154, 153, 155, 156};
  MenuInit(base, b_r3 + 60, 2, 2, kActions, frame, 0x8224C5A8, calls);
  calls.Call(kMenuListSetSelection, frame, 0x8224C5B8, b_r3 + 132, LoadU32(base, b + 236));
  StoreU32(base, b + 136, LoadU32(base, b + 240));
  uint64_t r3 = calls.Call(kMenuListFit, frame, 0x8224C5CC, b_r3 + 132, 1);
  if (LoadU32(base, b + 1908) == 0) {
    return r3;
  }
  const uint64_t action = calls.Call(kMenuListSelection, frame, 0x8224C5E0, b_r3 + 60);
  CarPlaceCursor(base, LoadU32(base, b + 1908), action, frame);
  CarSetTexts(base, LoadU32(base, b + 1908), frame, calls);
  const auto entry = [&] {
    const uint32_t at = (LoadU32(base, b + 240) + LoadU32(base, b + 236)) << 2;
    return LoadU32(base, LoadU32(base, b + 1820) + at);
  };
  if (LoadU32(base, b + 232) == 2) {
    // The garage's car at the selection: its halfword links from +204 of
    // the garage (save data + 6120) walked selection + scroll times.
    const uint32_t garage = uint32_t(SaveData(base, b, frame)) + 6120;
    uint64_t steps = uint64_t(LoadU32(base, b + 240)) + uint64_t(LoadU32(base, b + 236));
    int16_t node = int16_t(guest::LoadU16(base, garage + 204));
    uint32_t at = garage + (uint32_t(int32_t(node)) << 2);
    while (steps != 0) {
      node = int16_t(guest::LoadU16(base, at));
      steps--;
      at = garage + (uint32_t(int32_t(node)) << 2);
    }
    const int32_t slot = int16_t(guest::LoadU16(base, at + 2));
    const uint32_t car = garage + uint32_t(slot * 1968) + 212;
    calls.Call(kShowCarStats, frame, 0x8224C64C, LoadU32(base, b + 1908), car);
  } else {
    CarShowStats(base, LoadU32(base, b + 1908), entry(), frame, calls);
  }
  return CarSetName(base, LoadU32(base, b + 1908), entry(), frame, calls);
}

namespace {

// The garage's car `steps` links from its head: the halfword links from
// +204 of the garage, each a node of the garage's (its +0 the next, its +2
// the car's slot) as the originals' lha and rlwinm leave them.
uint32_t GarageNode(uint8_t* base, uint32_t garage, uint32_t steps) {
  int16_t node = int16_t(guest::LoadU16(base, garage + 204));
  uint32_t at = garage + (uint32_t(int32_t(node)) << 2);
  while (steps != 0) {
    node = int16_t(guest::LoadU16(base, at));
    steps--;
    at = garage + (uint32_t(int32_t(node)) << 2);
  }
  return at;
}

// The garage's record (1,968 bytes from the garage) of the node's slot.
uint32_t GarageRecord(uint8_t* base, uint32_t garage, uint32_t node) {
  const int32_t slot = int16_t(guest::LoadU16(base, node + 2));
  return garage + uint32_t(slot * 1968);
}

// memcpy(destination, source, 8432): c_runtime::Memcpy's copy, else the
// original. Returns r3 as memcpy leaves it (the destination).
uint64_t CopySpec(uint8_t* base, uint32_t destination, uint32_t source, uint32_t frame,
                  uint32_t lr, const Calls& calls) {
  if (c_runtime::Memcpy(base, destination, source, 8432)) {
    return destination;
  }
  return calls.Call(kMemcpy, frame, lr, destination, source, 8432);
}

}  // namespace

uint64_t Build(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224C698 PanelSetBase::Build");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t built = LoadU32(base, b + 4);
  if ((built & LoadU32(base, kTwoPlayers)) != 0) {
    return b_r3;
  }
  StoreU32(base, b + 252, 20);
  StoreU32(base, b + 256, 6000);
  StoreU32(base, b + 260, 1);
  StoreU32(base, b + 264, 20);
  StoreU32(base, b + 268, 10);
  StoreU32(base, b + 208, 0);
  if (built != 0) {
    // Each list's entries freed from the last down.
    const auto free_list = [&](uint32_t list, uint32_t whole, uint32_t lr) {
      while (LoadU32(base, b + whole) != 0) {
        const uint32_t n = LoadU32(base, list + 512) - 1;
        StoreU32(base, list + 512, n);
        FreePages(LoadU32(base, list + 4 * n), frame, lr, calls);
      }
    };
    free_list(b + 272, 784, 0x8224C728);
    free_list(b + 788, 1300, 0x8224C764);
    free_list(b + 1304, 1816, 0x8224C7A0);
  }
  const uint32_t save = uint32_t(SaveData(base, b, frame));
  uint64_t r3 = save;
  StoreU32(base, b + 784, 0);
  StoreU32(base, b + 1300, 0);
  StoreU32(base, b + 1816, 0);
  // The cars whose bit is set in the save data's words at +104740.
  for (uint32_t car = 0; car < 72; car++) {
    const uint32_t word = LoadU32(base, save + ((((car >> 5) + 26185) << 2)));
    if ((word & (1u << (car & 31))) == 0) {
      continue;
    }
    const uint32_t entry = memory::AllocPages(8448, 4096);
    const uint32_t spec = car_specs::GetCarSpec(base, car);
    r3 = CopySpec(base, entry, spec, frame, 0x8224C838, calls);
    const int8_t kind = int8_t(guest::LoadU8(base, entry + 771));
    StoreU32(base, entry + 8432, car);
    StoreU32(base, entry + 8436, 0);
    uint32_t list = 0;
    if (kind == 0) {
      list = b + 272;
    } else if (kind == 1) {
      list = b + 788;
    } else {
      r3 = FreePages(entry, frame, 0x8224C888, calls);
      continue;
    }
    const uint32_t count = LoadU32(base, list + 512);
    StoreU32(base, list + 512, count + 1);
    StoreU32(base, list + 4 * count, entry);
  }
  // The garage's cars (the count a halfword, sign-extended, compared
  // unsigned) to list 2.
  const uint32_t garage = save + 6120;
  const uint32_t list = b + 1304;
  for (uint32_t i = 0; i < uint32_t(int32_t(int16_t(guest::LoadU16(base, save + 6328)))); i++) {
    const uint32_t entry = memory::AllocPages(8448, 4096);
    const uint32_t id = LoadU32(base, GarageRecord(base, garage, GarageNode(base, garage, i)) + 224);
    r3 = CopySpec(base, entry, car_specs::GetCarSpec(base, id), frame, 0x8224C914, calls);
    StoreU32(base, entry + 8432,
             LoadU32(base, GarageRecord(base, garage, GarageNode(base, garage, i)) + 224));
    const int16_t color =
        int16_t(guest::LoadU16(base, GarageRecord(base, garage, GarageNode(base, garage, i)) + 212));
    StoreU32(base, entry + 8436, uint32_t(int32_t(color)));
    const uint32_t count = LoadU32(base, list + 512);
    StoreU32(base, list + 512, count + 1);
    StoreU32(base, list + 4 * count, entry);
  }
  if (LoadU32(base, b + 4) == 0) {
    StoreU32(base, b + 1876, 0);
  }
  StoreU32(base, b + 4, 1);
  return r3;
}

uint64_t ShowStandCar(uint8_t* base, uint64_t b_r3, uint64_t color, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A490 PanelSetBase::ShowStandCar");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 4032);
  const uint32_t at = (LoadU32(base, b + 240) + LoadU32(base, b + 236)) << 2;
  const uint32_t id = LoadU32(base, LoadU32(base, at + LoadU32(base, b + 1820)) + 8432);
  const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224A4C8, b_r3 + 60);
  uint64_t pick = uint64_t(LoadU32(base, b + 64)) + item;
  if (int32_t(uint32_t(color)) >= 0) {
    pick = color;
  }
  uint32_t slot = 0, flag = 1;
  if (LoadU32(base, kTwoPlayers) != 0) {
    if (LoadU32(base, b + 1952) == 0) {
      slot = 1;
    } else {
      slot = 2;
      flag = 128;
    }
  }
  const uint32_t car = frame + 80;
  race_setup::CarInit(base, car);
  race_setup::SetModel(base, car, id);
  StoreU32(base, car + 3888, LoadU32(base, car + 3888) | flag);
  // Flush-to-zero off, as Car::Refresh's plain C++ expects.
  using Fpscr = rex::platform::FPSCRPlatform;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, car, &refreshed)) {
    calls.Call(kCarRefresh, frame, 0x8224A53C, car);
  }
  type67_modes::GarageStageSetPlates(base, LoadU32(base, LoadU32(base, kStandHolders) + 4 * slot),
                                     0);
  const uint32_t holder = LoadU32(base, LoadU32(base, kStandHolders) + 4 * slot);
  return calls.Call(kSetCarClass, frame, 0x8224A568, holder, car, pick);
}

namespace {

// MenuSound::Play(cue) through its override.
uint64_t Sound(uint32_t cue, uint32_t frame, uint32_t lr, const Calls& calls) {
  return calls.Call(kMenuSoundPlay, frame, lr, cue);
}

// The step after a page: +40 = the page unless it's `page`, then the words
// the originals store (their order kept by the callers).
void Leave(uint8_t* base, uint32_t b, uint32_t page) {
  const uint32_t now = LoadU32(base, b + 32);
  if (now != page) {
    StoreU32(base, b + 40, now);
  }
}

}  // namespace

uint64_t TabsPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249EB8 PanelSetBase::TabsPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 144);
  const uint64_t menu = b_r3 + 60;
  if (LoadU32(base, b + 1936) != 0) {
    StoreU32(base, frame + 84, 159);
    StoreU32(base, frame + 92, 158);
    const uint64_t args[8] = {menu, 3, 3, 1, 154, 153, 155, 156};
    calls.CallArgs(kMenuListInit, frame, 0x82249F14, args);
    calls.Call(kMenuListSetSelection, frame, 0x82249F20, menu, LoadU32(base, b + 232));
    if (LoadU32(base, kTwoPlayers) != 0) {
      calls.Call(kMenuListSetEnabled, frame, 0x82249F48, menu, 0,
                 LoadU32(base, b + 784) != 0 ? 1 : 0);
      calls.Call(kMenuListSetEnabled, frame, 0x82249F64, menu, 1,
                 LoadU32(base, b + 1300) != 0 ? 1 : 0);
      calls.Call(kMenuListSetEnabled, frame, 0x82249F80, menu, 2,
                 LoadU32(base, b + 1816) != 0 ? 1 : 0);
      const uint64_t item = calls.Call(kMenuListSelection, frame, 0x82249F88, menu);
      const uint64_t on = calls.Call(kMenuListIsEnabled, frame, 0x82249F94, menu, item);
      if (uint32_t(on) == 0) {
        calls.Call(kMenuListSetSelection, frame, 0x82249FA8, menu, 0);
      }
    }
  }
  StoreU32(base, b + 1944, 0);
  calls.Call(kMenuListUpdate, frame, 0x82249FB8, menu);
  uint32_t step = 0;
  uint64_t r3 = 0;
  if (LoadU32(base, b + 84) != 0) {
    r3 = Sound(4, frame, 0x82249FCC, calls);
    const uint32_t now = LoadU32(base, b + 32);
    if (now != 0) {
      StoreU32(base, b + 40, now);
    }
    step = 2;
    StoreU32(base, b + 36, 0);
  } else {
    const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x82249FF0, menu);
    if (uint32_t(accepted) != 0) {
      const uint64_t tab = calls.Call(kMenuListSelection, frame, 0x8224A000, menu);
      const uint32_t t = uint32_t(tab);
      if (t < 1) {
        StoreU32(base, b + 1820, b + 272);
      } else if (t == 1) {
        StoreU32(base, b + 1820, b + 788);
      } else if (t < 3) {
        StoreU32(base, b + 1820, b + 1304);
      }
      const uint32_t count = LoadU32(base, LoadU32(base, b + 1820) + 512);
      if (int32_t(count) <= 0) {
        return Sound(3, frame, 0x8224A048, calls);
      }
      r3 = Sound(0, frame, 0x8224A054, calls);
      StoreU32(base, b + 232, t);
      if (t == 0 || t == 1) {
        for (uint32_t i = 0, at = 0; int32_t(i) < int32_t(count); i++, at += 4) {
          uint32_t color = 0;
          if (t == LoadU32(base, b + 1880) && i == LoadU32(base, b + 1884)) {
            const uint32_t list = LoadU32(base, b + 1820);
            color = LoadU32(base, b + 1888);
            StoreU32(base, LoadU32(base, at + list) + 8436, color);
          } else {
            StoreU32(base, LoadU32(base, at + LoadU32(base, b + 1820)) + 8436, color);
          }
        }
      }
      Leave(base, b, 1);
      StoreU32(base, b + 44, 4);
      StoreU32(base, b + 48, 6);
      StoreU32(base, b + 36, 1);
      StoreU32(base, b + 32, 0xFFFFFFFFu);
      const uint32_t picked = LoadU32(base, b + 232);
      const uint32_t before = LoadU32(base, b + 1880);
      StoreU32(base, b + 240, 0);
      StoreU32(base, b + 236, picked == before ? LoadU32(base, b + 1884) : 0);
      return r3;
    }
    const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224A114, menu);
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    r3 = Sound(2, frame, 0x8224A124, calls);
    if (LoadU32(base, kTwoPlayers) != 0) {
      StoreU32(base, b + 1944, 1);
      if (LoadU32(base, b + 1940) == 0) {
        return r3;
      }
    }
    Leave(base, b, 5);
    StoreU32(base, b + 36, 5);
    step = 3;
  }
  StoreU32(base, b + 32, 0xFFFFFFFFu);
  StoreU32(base, b + 48, 0xFFFFFFFFu);
  StoreU32(base, b + 44, step);
  return r3;
}

uint64_t RowsPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A178 PanelSetBase::RowsPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60;
  StoreU32(base, b + 1944, 0);
  calls.Call(kMenuListUpdate, frame, 0x8224A19C, menu);
  uint32_t step = 0, page = 0;
  uint64_t r3 = 0;
  if (LoadU32(base, b + 84) != 0 || LoadU32(base, b + 64) != LoadU32(base, b + 68)) {
    r3 = Sound(4, frame, 0x8224A330, calls);
    Leave(base, b, 1);
    step = 7;
    page = 1;
  } else {
    const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x8224A1C0, menu);
    if (uint32_t(accepted) != 0) {
      Sound(0, frame, 0x8224A1D0, calls);
      r3 = calls.Call(kMenuListSelection, frame, 0x8224A1D8, menu);
      const uint32_t scroll = LoadU32(base, b + 64);
      const uint32_t rows = LoadU32(base, b + 1900);
      StoreU32(base, b + 236, uint32_t(r3));
      StoreU32(base, b + 240, scroll);
      if (rows != 0) {
        r3 = RowUnhighlight(base, rows, frame, calls);
      }
      uint32_t next = 0;
      if (LoadU32(base, b + 1892) != 0) {
        Leave(base, b, 8);
        StoreU32(base, b + 48, 0xFFFFFFFFu);
        StoreU32(base, b + 44, 26);
        StoreU32(base, b + 32, 0xFFFFFFFFu);
        next = 8;
      } else {
        r3 = calls.Call(kMenuListSetSelection, frame, 0x8224A23C, menu, 0);
        Leave(base, b, 2);
        StoreU32(base, b + 44, 9);
        StoreU32(base, b + 48, 11);
        StoreU32(base, b + 32, 0xFFFFFFFFu);
        next = 2;
      }
      StoreU32(base, b + 36, next);
      const uint32_t at = (LoadU32(base, b + 240) + LoadU32(base, b + 236)) << 2;
      const uint32_t list = LoadU32(base, b + 1820);
      StoreU32(base, b + 248, 0);
      StoreU32(base, b + 244, LoadU32(base, LoadU32(base, at + list) + 8436));
      return r3;
    }
    const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224A29C, menu);
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    r3 = Sound(2, frame, 0x8224A2AC, calls);
    if (LoadU32(base, b + 1928) == 1) {
      Leave(base, b, 0);
      StoreU32(base, b + 36, 0);
      StoreU32(base, b + 44, 8);
      StoreU32(base, b + 48, 1);
      StoreU32(base, b + 32, 0xFFFFFFFFu);
      return r3;
    }
    if (LoadU32(base, kTwoPlayers) != 0) {
      return r3;
    }
    StoreU32(base, b + 1944, 1);
    if (LoadU32(base, b + 1940) == 0) {
      return r3;
    }
    Leave(base, b, 5);
    step = 3;
    page = 5;
  }
  StoreU32(base, b + 32, 0xFFFFFFFFu);
  StoreU32(base, b + 36, page);
  StoreU32(base, b + 48, 0xFFFFFFFFu);
  StoreU32(base, b + 44, step);
  return r3;
}

uint64_t ColorsPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AEA8 PanelSetBase::ColorsPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60;
  StoreU32(base, b + 20, 1);
  calls.Call(kMenuListUpdate, frame, 0x8224AECC, menu);
  if (LoadU32(base, b + 84) != 0 || LoadU32(base, b + 64) != LoadU32(base, b + 68)) {
    uint64_t r3 = Sound(4, frame, 0x8224AFFC, calls);
    Leave(base, b, 3);
    StoreU32(base, b + 48, 0xFFFFFFFFu);
    StoreU32(base, b + 44, 19);
    StoreU32(base, b + 36, 3);
    StoreU32(base, b + 32, 0xFFFFFFFFu);
    if (LoadU32(base, b + 20) == 1 && LoadU32(base, b + 24) == 1) {
      r3 = ShowStandCar(base, b_r3, 0xFFFFFFFFFFFFFFFFull, frame, calls);
    }
    if (LoadU32(base, kTwoPlayers) != 0) {
      return r3;
    }
    r3 = calls.Call(kMenuListSelection, frame, 0x8224B064, menu);
    const uint32_t scroll = LoadU32(base, b + 64);
    StoreU32(base, b + 244, uint32_t(r3));
    StoreU32(base, b + 248, scroll);
    return r3;
  }
  const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x8224AEF0, menu);
  if (uint32_t(accepted) != 0) {
    Sound(0, frame, 0x8224AF00, calls);
    const uint64_t r3 = calls.Call(kMenuListSelection, frame, 0x8224AF08, menu);
    const uint32_t scroll = LoadU32(base, b + 64);
    const uint32_t confirm = LoadU32(base, b + 1912);
    StoreU32(base, b + 244, uint32_t(r3));
    StoreU32(base, b + 248, scroll);
    uint32_t next = 0;
    if (confirm != 0) {
      Leave(base, b, 4);
      StoreU32(base, b + 44, 21);
      StoreU32(base, b + 48, 22);
      StoreU32(base, b + 32, 0xFFFFFFFFu);
      next = 4;
    } else {
      Leave(base, b, 5);
      StoreU32(base, b + 48, 0xFFFFFFFFu);
      StoreU32(base, b + 44, 3);
      StoreU32(base, b + 32, 0xFFFFFFFFu);
      next = 5;
    }
    StoreU32(base, b + 36, next);
    StoreU32(base, b + 8, 1);
    return r3;
  }
  const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224AF88, menu);
  if (uint32_t(cancelled) == 0) {
    return cancelled;
  }
  const uint64_t r3 = Sound(2, frame, 0x8224AF98, calls);
  const uint32_t at = (LoadU32(base, b + 240) + LoadU32(base, b + 236)) << 2;
  const uint32_t color = LoadU32(base, LoadU32(base, at + LoadU32(base, b + 1820)) + 8436);
  StoreU32(base, b + 248, 0);
  StoreU32(base, b + 244, color);
  Leave(base, b, 2);
  StoreU32(base, b + 44, 20);
  StoreU32(base, b + 48, 11);
  StoreU32(base, b + 36, 2);
  StoreU32(base, b + 32, 0xFFFFFFFFu);
  return r3;
}

uint64_t PowerGraphPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224BDC0 PanelSetBase::PowerGraphPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60;
  StoreU32(base, b + 8, 0);
  const uint32_t asked = LoadU32(base, b + 16);
  if (LoadU32(base, kTwoPlayers) != 0) {
    if (asked != 0) {
      const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x8224BE08, menu);
      if (uint32_t(accepted) == 0) {
        const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224BE18, menu);
        if (uint32_t(cancelled) == 0) {
          return cancelled;
        }
      }
    }
    Sound(2, frame, 0x8224BE28, calls);
    Leave(base, b, 2);
    StoreU32(base, b + 44, 16);
    StoreU32(base, b + 32, 0xFFFFFFFFu);
    StoreU32(base, b + 36, 2);
  } else if (asked == 0) {
    Leave(base, b, 2);
    StoreU32(base, b + 32, 2);
    StoreU32(base, b + 44, 0xFFFFFFFFu);
  } else {
    const uint64_t steps = uint64_t(LoadU32(base, b + 240)) + uint64_t(LoadU32(base, b + 236));
    if (LoadU32(base, b + 232) == 2) {
      // The record's address as the original's mulli and adds leave it (64
      // bits).
      const uint64_t garage = SaveData(base, b, frame) + 6120;
      const uint32_t node = GarageNode(base, uint32_t(garage), uint32_t(steps));
      const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
      return calls.Call(kGaragePowerGraph, frame, 0x8224BF00,
                        uint64_t(slot * 1968) + garage + 212);
    }
    const uint32_t entry = LoadU32(base, LoadU32(base, b + 1820) + (uint32_t(steps) << 2));
    return calls.Call(kEntryPowerGraph, frame, 0x8224BF14, entry);
  }
  StoreU32(base, b + 48, 0xFFFFFFFFu);
  return CarSetButtons(base, LoadU32(base, b + 1908), 0, 0, 1, 1, frame, calls);
}

namespace {

// fctiwz as the translated code does it: NaN and below INT_MIN 0x80000000,
// INT_MAX and above INT_MAX, else truncated; the low word.
uint32_t Fctiwz(double value) {
  if (std::isnan(value)) return 0x80000000u;
  if (value >= double(INT32_MAX)) return uint32_t(INT32_MAX);
  if (value < double(INT32_MIN)) return 0x80000000u;
  return uint32_t(int32_t(value));
}

}  // namespace

uint64_t SetUpColors(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B248 PanelSetBase::SetUpColors");
  using Fpscr = rex::platform::FPSCRPlatform;
  const auto flush_off = [] { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); };
  flush_off();
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 224);
  const uint64_t menu = b_r3 + 60;
  const uint32_t at = (LoadU32(base, b + 240) + LoadU32(base, b + 236)) << 2;
  const uint32_t limit = LoadU32(base, b + 1924);
  const uint32_t entry = LoadU32(base, at + LoadU32(base, b + 1820));
  const uint32_t count = LoadU32(base, entry + 8376);
  const bool over = int32_t(count) > int32_t(limit);
  StoreU32(base, frame + 92, 158);
  StoreU32(base, frame + 84, 159);
  const uint64_t args[8] = {menu, count, over ? limit : count, 1, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, over ? 0x8224B2BC : 0x8224B2D8, args);
  calls.Call(kMenuListSetSelection, frame, 0x8224B2E4, menu, LoadU32(base, b + 244));
  StoreU32(base, b + 64, LoadU32(base, b + 248));
  calls.Call(kMenuListFit, frame, 0x8224B2F8, menu, 1);
  if (LoadU32(base, b + 1904) != 0) {
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224B30C, menu);
    ColorPlaceCursor(base, LoadU32(base, b + 1904), item, frame);
  }
  const uint32_t colors = LoadU32(base, b + 1904);
  if (colors != 0) {
    const uint32_t items = LoadU32(base, b + 60);
    const uint32_t shown = LoadU32(base, b + 1924);
    uint64_t up = 0, down = 0;
    if (int32_t(items) > int32_t(shown)) {
      const uint32_t scroll = LoadU32(base, b + 64);
      if (scroll == 0) {
        down = 1;
      } else {
        up = 1;
        down = uint32_t(scroll + shown) == items ? 0 : 1;
      }
    }
    ColorSetArrows(base, colors, up, down, frame);
  }
  // The colors' list freed, from the last record down.
  const uint32_t list = b + 1824;
  while (LoadU32(base, b + 1876) != 0) {
    const uint32_t n = LoadU32(base, list + 52) - 1;
    StoreU32(base, list + 52, n);
    const uint32_t record = LoadU32(base, list + 4 * n);
    if (record != 0) {
      FreePages(record, frame, 0x8224B3B0, calls);
    }
  }
  if (int32_t(count) > 0) {
    const double scale = double(guest::LoadFloat(base, 0x82000C44));
    const double unit = double(guest::LoadFloat(base, 0x82002200));
    uint32_t source = entry + 8116;
    for (uint32_t left = count; left != 0; left--, source += 20) {
      const uint32_t record = memory::AllocPages(32, 4096);
      const uint32_t word = LoadU32(base, source + 4);
      const uint32_t raised = guest::LoadU16(base, source + 0);
      StoreU32(base, frame + 96, word);
      const uint32_t add = LoadU32(base, raised == 0 ? 0x82002FF8 : 0x82002FF4);
      int32_t value = int32_t(guest::LoadU8(base, frame + 99) + add);
      if (value < 0) {
        value = 0;
      } else if (value > 255) {
        value = 255;
      }
      const uint64_t saturation = guest::LoadU8(base, frame + 98);
      const uint64_t v = uint32_t(value) & 0xFF;
      const uint64_t hue = guest::LoadU16(base, frame + 96);
      // The three std's (big-endian doublewords: the high words 0).
      for (const auto& [to, word] : {std::pair{frame + 104, saturation}, std::pair{frame + 112, v},
                                     std::pair{frame + 120, hue}}) {
        StoreU32(base, to, uint32_t(word >> 32));
        StoreU32(base, to + 4, uint32_t(word));
      }
      const double s = double(float(double(int64_t(saturation))));
      const double vv = double(float(double(int64_t(v))));
      const double h = double(float(double(int64_t(hue))));
      guest::StoreFloat(base, record + 16, float(h));
      guest::StoreFloat(base, record + 20, float(double(float(s * unit))));
      guest::StoreFloat(base, record + 24, float(double(float(vv * unit))));
      tuning_pages::HsvToRgb(base, frame + 128, record + 16, frame);
      flush_off();
      const double red = double(float(double(guest::LoadFloat(base, frame + 128)) * scale));
      const double green = double(float(double(guest::LoadFloat(base, frame + 132)) * scale));
      const double blue = double(float(double(guest::LoadFloat(base, frame + 136)) * scale));
      StoreU32(base, record + 0, Fctiwz(red));
      StoreU32(base, record + 4, Fctiwz(green));
      StoreU32(base, record + 8, Fctiwz(blue));
      const uint32_t n = LoadU32(base, list + 52);
      StoreU32(base, list + 52, n + 1);
      StoreU32(base, list + 4 * n, record);
    }
  }
  if (LoadU32(base, b + 1904) != 0) {
    ColorSetCount(base, LoadU32(base, b + 1904), count, frame, calls);
    ColorSetSwatches(base, LoadU32(base, b + 1904), list, LoadU32(base, b + 64), frame);
  }
  return ShowStandCar(base, b_r3, 0xFFFFFFFFFFFFFFFFull, frame, calls);
}

}  // namespace car_list
