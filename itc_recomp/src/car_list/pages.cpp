// PanelSet's pages and steps (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "memory/heap.h"
#include "power_graph/power_graph.h"
#include "race_setup/race_setup.h"
#include "schema/car_list.h"
#include "tuning_pages/tuning_pages.h"
#include "type67_modes/type67_modes.h"

#include "base/float_mode.h"

#include <climits>
#include <cmath>
#include <utility>

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::CarListGlobals;
using Base = schema::PanelSetBase;
using Set = schema::PanelSet;
using Tab = schema::TabPanel;
using Row = schema::RowPanel;
using Car = schema::CarPanel;
using Entry = schema::CarListEntry;
using List = schema::CarListList;
using ColorList = schema::CarListColorList;
using Color = schema::CarListColor;
using BackObject = schema::CarListBackObject;
using Word = schema::CarListStepWord;

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
  G::set_fade_wait(base, 1);
  return calls.Call(function, frame, lr, count, 0, 0, 0);
}

}  // namespace

uint64_t ToTabs(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249558 PanelSetBase::ToTabs");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t page = Base(base, b).page();
  if (page != 0) {
    Base(base, b).set_previous_page(page);
  }
  Base(base, b).set_step(0);
  Base(base, b).set_queued_step(1);
  Base(base, b).set_next_page(0);
  Base(base, b).set_page(0xFFFFFFFFu);
  Row(base, Base(base, b).rows().address).set_show_alt(0);
  if (Base(base, b).page_changed() == 0) {
    return b_r3;
  }
  StoreU32(base, frame + 84, 159);
  StoreU32(base, frame + 92, 158);
  // The menu's address as addi leaves it (r3's high half kept).
  const uint64_t args[8] = {b_r3 + 60, 3, 3, 1, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, 0x822495E8, args);
  return calls.Call(kMenuListSetSelection, frame, 0x822495F4, b_r3 + 60, Base(base, b).tab());
}

uint64_t Open(uint8_t* base, uint64_t set, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249730 PanelSet::Open");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t b_r3 = set + 4;
  const uint32_t b = uint32_t(b_r3);
  const Base set_base(base, b);
  set_base.set_picked(0);
  set_base.set_done(0);
  set_base.set_power_graph_asked(0);
  set_base.set_show_stand_car(0);
  set_base.set_stand_model_loaded(0);
  set_base.set_may_leave(1);
  set_base.set_fade_with_steps(1);
  set_base.set_next_page(0xFFFFFFFFu);
  set_base.set_page(0xFFFFFFFFu);
  set_base.set_previous_page(0xFFFFFFFFu);
  set_base.set_step_cleared(0xFFFFFFFFu);
  set_base.set_step(0xFFFFFFFFu);
  set_base.set_first_pass(0);
  set_base.set_current_list(0);
  set_base.set_tab_shown(0);
  set_base.set_tab(0);
  set_base.set_row(0);
  set_base.set_scroll(0);
  set_base.set_color(0);
  set_base.set_color_scroll(0);
  const uint32_t kind = Set(base, uint32_t(set)).open_kind();
  Base(base, b).set_step_cleared(0xFFFFFFFFu);
  Base(base, b).set_step(0xFFFFFFFFu);
  Base(base, b).set_first_pass(0);
  uint64_t r3 = b_r3;
  if (kind == 1) {
    Base(base, b).set_next_page(0);
    Base(base, b).set_page(0);
    Base(base, b).set_previous_page(0);
    const uint32_t tab = Base(base, b).previous_tab();
    Base(base, b).set_tab(tab);
    Base(base, b).set_tab_shown(tab);
    r3 = ToTabs(base, b_r3, frame, calls);
  } else {
    Base(base, b).set_next_page(1);
    Base(base, b).set_page(1);
    Base(base, b).set_previous_page(1);
    Base(base, b).set_tab_shown(0);
    Base(base, b).set_tab(0);
    Base(base, b).set_current_list(Base(base, b).list0_address());
    const uint32_t page = Base(base, b).page();
    if (page != 1) {
      Base(base, b).set_previous_page(page);
    }
    Base(base, b).set_next_page(1);
    Base(base, b).set_page(0xFFFFFFFFu);
    Base(base, b).set_step(4);
    Base(base, b).set_queued_step(6);
    Row(base, Base(base, b).rows().address).set_show_alt(1);
  }
  Set(base, uint32_t(set)).set_may_leave(0);
  return r3;
}

uint64_t Free(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B538 PanelSetBase::Free");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  Base(base, b).set_current_list(0);
  // Each list (its entries from +0, the count at `count`, the guest address
  // of its count word) freed from the last entry down.
  const auto free_list = [&](uint32_t list, uint32_t count, uint32_t lr) {
    while (LoadU32(base, count) != 0) {
      const uint32_t n = LoadU32(base, count) - 1;
      StoreU32(base, count, n);
      FreePages(LoadU32(base, list + 4 * n), frame, lr, calls);
    }
  };
  free_list(Base(base, b).list0_address(),
            List(base, Base(base, b).list0_address()).count_address(), 0x8224B588);
  free_list(Base(base, b).list1_address(),
            List(base, Base(base, b).list1_address()).count_address(), 0x8224B5C4);
  free_list(Base(base, b).list2_address(),
            List(base, Base(base, b).list2_address()).count_address(), 0x8224B600);
  free_list(Base(base, b).colors_address(),
            ColorList(base, Base(base, b).colors_address()).count_address(), 0x8224B63C);
  calls.Call(kMenuListRelease, frame, 0x8224B650, b_r3 + 60);
  return calls.Call(kMenuListRelease, frame, 0x8224B658, b_r3 + 132);
}

uint64_t BackPage8(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A3F8 PanelSetBase::BackPage8");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t confirm = Base(base, b).back_object().address;
  if (confirm != 0) {
    BackObject(base, confirm).set_value(BackObject(base, confirm).page8_value());
  }
  const uint64_t r3 = Fade(base, kFadeIn, G::fade_in_passes_address(), frame, 0x8224A448, calls);
  const uint32_t page = Base(base, b).page();
  if (page != 2) {
    Base(base, b).set_previous_page(page);
  }
  Base(base, b).set_step(9);
  Base(base, b).set_queued_step(11);
  Base(base, b).set_next_page(2);
  Base(base, b).set_page(0xFFFFFFFFu);
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
  const uint32_t page = Base(base, b).page();
  for (uint32_t i = 0; i < 9; i++) {
    StoreU32(base, frame + 80 + 4 * i, kPages[i]);
  }
  StoreU32(base, frame + 116, 0);
  StoreU32(base, frame + 120, 0);
  Base(base, b).set_page_changed(page != Base(base, b).page_drawn() ? 1 : 0);
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
      case 0x8224B8D8:
        r3 = CarPage(base, b_r3, frame, calls);
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
  Base(base, b).set_page_drawn(page);
  return r3;
}

uint64_t Step2(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248F00 PanelSetBase::Step2");
  const uint32_t frame = Frame(base, sp, 128);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  const uint32_t tabs = Base(base, b).tabs().address;
  if (tabs != 0) {
    const uint32_t moves = Base(base, b).page_menu_last_move();
    const uint64_t before =
        calls.Call(kMenuListPrevious, frame, 0x82248F3C, Base(base, b).page_menu_address());
    const uint64_t tab =
        calls.Call(kMenuListSelection, frame, 0x82248F48, Base(base, b).page_menu_address());
    TabMove(base, Base(base, b).tabs().address, tab, before, moves, frame, calls);
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step3(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A718 PanelSetBase::Step3");
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() == 0) {
    if (Base(base, b).fade_with_steps() != 0) {
      Fade(base, kFadeOut, G::fade_out_passes_address(), frame, 0x8224A76C, calls);
    }
    // The empty sub_82106540 on F (its override does nothing).
    Word(base, word).set_first_pass(1);
  }
  uint32_t waited = 1;
  uint64_t r3 = 1;
  if (Base(base, b).fade_with_steps() != 0) {
    waited = G::fade_wait(base) == 0 ? 1 : 0;
  }
  const uint32_t tabs = Base(base, b).tabs().address;
  if (tabs != 0) {
    r3 = calls.Call(kTabFinished, frame, 0x8224A7B8, tabs);
  }
  return waited != 0 && uint32_t(r3) != 0 ? 1 : 0;
}

uint64_t Step7(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249098 PanelSetBase::Step7");
  const uint32_t frame = Frame(base, sp, 144);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  if (Base(base, b).rows().address != 0) {
    const uint32_t list = Base(base, b).page_menu_address();
    const uint32_t step = Base(base, b).page_menu_last_move();
    const uint32_t scroll = Base(base, b).page_menu_scroll();
    const uint64_t moves =
        uint64_t(Base(base, b).page_menu_scroll()) - uint64_t(Base(base, b).page_menu_scroll_before());
    const uint64_t before = calls.Call(kMenuListPrevious, frame, 0x822490E4, list);
    const uint64_t row = calls.Call(kMenuListSelection, frame, 0x822490F0, list);
    RowMove(base, Base(base, b).rows().address, row, before, moves, scroll,
            Base(base, b).current_list().address, frame, calls);
    (void)step;  // r8, which RowPanel::Move doesn't read
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step11(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A7E0 PanelSetBase::Step11");
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  if (Base(base, b).car_panel().address != 0) {
    const uint64_t item =
        calls.Call(kMenuListSelection, frame, 0x8224A818, Base(base, b).page_menu_address());
    Car(base, Base(base, b).car_panel().address).set_action(uint32_t(item));
    const uint32_t count = Base(base, b).current_list().count();
    CarSelect(base, Base(base, b).car_panel().address, count > 1 ? 1 : 0, frame, calls);
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step18(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249258 PanelSetBase::Step18");
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  if (Base(base, b).color_panel().address != 0) {
    const uint64_t item =
        calls.Call(kMenuListSelection, frame, 0x82249294, Base(base, b).page_menu_address());
    ColorSelect(base, Base(base, b).color_panel().address, item, frame, calls);
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step26(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A8E0 PanelSetBase::Step26");
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() == 0) {
    if (Base(base, b).fade_with_steps() != 0) {
      Fade(base, kFadeOut, G::fade_out_passes_address(), frame, 0x8224A934, calls);
    }
    Word(base, word).set_first_pass(1);
  }
  const uint32_t done = G::fade_wait(base) == 0 ? 1 : 0;
  if (done == 0) {
    return 0;
  }
  if (Base(base, b).color_panel().address != 0) {
    PartFinishAll(base, Base(base, b).color_panel().address, frame, calls);
  }
  if (Base(base, b).tabs().address != 0) {
    PartFinishAll(base, Base(base, b).tabs().address, frame, calls);
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
  const uint64_t count = Base(base, b).tab() == 2
                             ? uint64_t(int64_t(int16_t(guest::LoadU16(base, save + 6328))))
                             : uint64_t(Base(base, b).current_list().count());
  uint64_t shown = Base(base, b).rows_shown();
  static constexpr uint32_t kRows[4] = {154, 153, 155, 156};
  static constexpr uint32_t kList[4] = {162, 161, 153, 154};
  if (G::two_players(base) != 0) {
    if (!(int32_t(uint32_t(count)) > int32_t(uint32_t(shown)))) {
      shown = count;
    }
    MenuInit(base, b_r3 + 60, count, shown, kRows, frame, 0x8224A9FC, calls);
    MenuInit(base, b_r3 + 132, count, count, kList, frame, 0x8224AA28, calls);
    Base(base, b).set_rows_menu_player(Base(base, b).player() != 0 ? 1 : 0);
  } else if (int32_t(uint32_t(count)) < int32_t(uint32_t(shown))) {
    MenuInit(base, b_r3 + 60, count, count, kRows, frame, 0x8224AA80, calls);
    MenuInit(base, b_r3 + 132, count, count, kList, frame, 0x8224AAAC, calls);
  } else {
    MenuInit(base, b_r3 + 60, count, shown, kRows, frame, 0x8224AABC, calls);
    MenuInit(base, b_r3 + 132, count, count, kList, frame, 0x8224AAE8, calls);
  }
  calls.Call(kMenuListSetSelection, frame, 0x8224AAF4, b_r3 + 60, Base(base, b).row());
  Base(base, b).set_page_menu_scroll(Base(base, b).scroll());
  calls.Call(kMenuListFit, frame, 0x8224AB08, b_r3 + 60, 1);
  if (Base(base, b).rows().address != 0) {
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224AB1C, b_r3 + 60);
    RowFill(base, Base(base, b).rows().address, Base(base, b).current_list().address,
            Base(base, b).page_menu_scroll(), item, frame, calls);
  }
  const uint32_t rows = Base(base, b).rows().address;
  if (rows == 0) {
    return 0;
  }
  return RowRefresh(base, rows, Base(base, b).current_list().address,
                    Base(base, b).page_menu_scroll(), frame, calls);
}

uint64_t SetUpCarPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224C560 PanelSetBase::SetUpCarPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 128);
  static constexpr uint32_t kActions[4] = {154, 153, 155, 156};
  MenuInit(base, b_r3 + 60, 2, 2, kActions, frame, 0x8224C5A8, calls);
  calls.Call(kMenuListSetSelection, frame, 0x8224C5B8, b_r3 + 132, Base(base, b).row());
  Base(base, b).set_rows_menu_scroll(Base(base, b).scroll());
  uint64_t r3 = calls.Call(kMenuListFit, frame, 0x8224C5CC, b_r3 + 132, 1);
  if (Base(base, b).car_panel().address == 0) {
    return r3;
  }
  const uint64_t action = calls.Call(kMenuListSelection, frame, 0x8224C5E0, b_r3 + 60);
  CarPlaceCursor(base, Base(base, b).car_panel().address, action, frame);
  CarSetTexts(base, Base(base, b).car_panel().address, frame, calls);
  const auto entry = [&] {
    const uint32_t at = (Base(base, b).scroll() + Base(base, b).row()) << 2;
    return LoadU32(base, Base(base, b).current_list().entries_address() + at);
  };
  if (Base(base, b).tab() == 2) {
    // The garage's car at the selection: its halfword links from +204 of
    // the garage (save data + 6120) walked selection + scroll times.
    const uint32_t garage = uint32_t(SaveData(base, b, frame)) + 6120;
    uint64_t steps = uint64_t(Base(base, b).scroll()) + uint64_t(Base(base, b).row());
    int16_t node = int16_t(guest::LoadU16(base, garage + 204));
    uint32_t at = garage + (uint32_t(int32_t(node)) << 2);
    while (steps != 0) {
      node = int16_t(guest::LoadU16(base, at));
      steps--;
      at = garage + (uint32_t(int32_t(node)) << 2);
    }
    const int32_t slot = int16_t(guest::LoadU16(base, at + 2));
    const uint32_t car = garage + uint32_t(slot * 1968) + 212;
    calls.Call(kShowCarStats, frame, 0x8224C64C, Base(base, b).car_panel().address, car);
  } else {
    CarShowStats(base, Base(base, b).car_panel().address, entry(), frame, calls);
  }
  return CarSetName(base, Base(base, b).car_panel().address, entry(), frame, calls);
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
  const uint32_t built = Base(base, b).built();
  if ((built & G::two_players(base)) != 0) {
    return b_r3;
  }
  Base(base, b).set_unknown_252(20);
  Base(base, b).set_unknown_256(6000);
  Base(base, b).set_unknown_260(1);
  Base(base, b).set_unknown_264(20);
  Base(base, b).set_unknown_268(10);
  Base(base, b).set_unknown_208(0);
  if (built != 0) {
    // Each list's entries freed from the last down.
    const auto free_list = [&](uint32_t list, uint32_t lr) {
      while (List(base, list).count() != 0) {
        const uint32_t n = List(base, list).count() - 1;
        List(base, list).set_count(n);
        FreePages(LoadU32(base, List(base, list).entries_address() + 4 * n), frame, lr, calls);
      }
    };
    free_list(Base(base, b).list0_address(), 0x8224C728);
    free_list(Base(base, b).list1_address(), 0x8224C764);
    free_list(Base(base, b).list2_address(), 0x8224C7A0);
  }
  const uint32_t save = uint32_t(SaveData(base, b, frame));
  uint64_t r3 = save;
  Base(base, b).set_list0_count(0);
  Base(base, b).set_list1_count(0);
  Base(base, b).set_list2_count(0);
  // The cars whose bit is set in the save data's words at +104740.
  for (uint32_t car = 0; car < 72; car++) {
    const uint32_t word = LoadU32(base, save + ((((car >> 5) + 26185) << 2)));
    if ((word & (1u << (car & 31))) == 0) {
      continue;
    }
    const uint32_t entry = memory::AllocPages(8448, 4096);
    const uint32_t spec = car_specs::GetCarSpec(base, car);
    r3 = CopySpec(base, entry, spec, frame, 0x8224C838, calls);
    const int8_t kind = int8_t(Entry(base, entry).list_kind());
    Entry(base, entry).set_car_id(car);
    Entry(base, entry).set_color(0);
    uint32_t list = 0;
    if (kind == 0) {
      list = Base(base, b).list0_address();
    } else if (kind == 1) {
      list = Base(base, b).list1_address();
    } else {
      r3 = FreePages(entry, frame, 0x8224C888, calls);
      continue;
    }
    const uint32_t count = List(base, list).count();
    List(base, list).set_count(count + 1);
    StoreU32(base, List(base, list).entries_address() + 4 * count, entry);
  }
  // The garage's cars (the count a halfword, sign-extended, compared
  // unsigned) to list 2.
  const uint32_t garage = save + 6120;
  const uint32_t list = Base(base, b).list2_address();
  for (uint32_t i = 0; i < uint32_t(int32_t(int16_t(guest::LoadU16(base, save + 6328)))); i++) {
    const uint32_t entry = memory::AllocPages(8448, 4096);
    const uint32_t id = LoadU32(base, GarageRecord(base, garage, GarageNode(base, garage, i)) + 224);
    r3 = CopySpec(base, entry, car_specs::GetCarSpec(base, id), frame, 0x8224C914, calls);
    Entry(base, entry)
        .set_car_id(LoadU32(base, GarageRecord(base, garage, GarageNode(base, garage, i)) + 224));
    const int16_t color =
        int16_t(guest::LoadU16(base, GarageRecord(base, garage, GarageNode(base, garage, i)) + 212));
    Entry(base, entry).set_color(uint32_t(int32_t(color)));
    const uint32_t count = List(base, list).count();
    List(base, list).set_count(count + 1);
    StoreU32(base, List(base, list).entries_address() + 4 * count, entry);
  }
  if (Base(base, b).built() == 0) {
    Base(base, b).set_color_count(0);
  }
  Base(base, b).set_built(1);
  return r3;
}

uint64_t ShowStandCar(uint8_t* base, uint64_t b_r3, uint64_t color, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A490 PanelSetBase::ShowStandCar");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 4032);
  const uint32_t at = (Base(base, b).scroll() + Base(base, b).row()) << 2;
  const uint32_t id =
      Entry(base, LoadU32(base, at + Base(base, b).current_list().entries_address())).car_id();
  const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224A4C8, b_r3 + 60);
  uint64_t pick = uint64_t(Base(base, b).page_menu_scroll()) + item;
  if (int32_t(uint32_t(color)) >= 0) {
    pick = color;
  }
  uint32_t slot = 0, flag = 1;
  if (G::two_players(base) != 0) {
    if (Base(base, b).player() == 0) {
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
  using Fpscr = ::base::FloatMode;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, car, &refreshed)) {
    calls.Call(kCarRefresh, frame, 0x8224A53C, car);
  }
  type67_modes::GarageStageSetPlates(base, LoadU32(base, G::stand_holders(base).address + 4 * slot),
                                     0);
  const uint32_t holder = LoadU32(base, G::stand_holders(base).address + 4 * slot);
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
  const uint32_t now = Base(base, b).page();
  if (now != page) {
    Base(base, b).set_previous_page(now);
  }
}

}  // namespace

uint64_t TabsPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249EB8 PanelSetBase::TabsPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 144);
  const uint64_t menu = b_r3 + 60;
  if (Base(base, b).page_changed() != 0) {
    StoreU32(base, frame + 84, 159);
    StoreU32(base, frame + 92, 158);
    const uint64_t args[8] = {menu, 3, 3, 1, 154, 153, 155, 156};
    calls.CallArgs(kMenuListInit, frame, 0x82249F14, args);
    calls.Call(kMenuListSetSelection, frame, 0x82249F20, menu, Base(base, b).tab());
    if (G::two_players(base) != 0) {
      calls.Call(kMenuListSetEnabled, frame, 0x82249F48, menu, 0,
                 Base(base, b).list0_count() != 0 ? 1 : 0);
      calls.Call(kMenuListSetEnabled, frame, 0x82249F64, menu, 1,
                 Base(base, b).list1_count() != 0 ? 1 : 0);
      calls.Call(kMenuListSetEnabled, frame, 0x82249F80, menu, 2,
                 Base(base, b).list2_count() != 0 ? 1 : 0);
      const uint64_t item = calls.Call(kMenuListSelection, frame, 0x82249F88, menu);
      const uint64_t on = calls.Call(kMenuListIsEnabled, frame, 0x82249F94, menu, item);
      if (uint32_t(on) == 0) {
        calls.Call(kMenuListSetSelection, frame, 0x82249FA8, menu, 0);
      }
    }
  }
  Base(base, b).set_cancelled(0);
  calls.Call(kMenuListUpdate, frame, 0x82249FB8, menu);
  uint32_t step = 0;
  uint64_t r3 = 0;
  if (Base(base, b).page_menu_last_move() != 0) {
    r3 = Sound(4, frame, 0x82249FCC, calls);
    const uint32_t now = Base(base, b).page();
    if (now != 0) {
      Base(base, b).set_previous_page(now);
    }
    step = 2;
    Base(base, b).set_next_page(0);
  } else {
    const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x82249FF0, menu);
    if (uint32_t(accepted) != 0) {
      const uint64_t tab = calls.Call(kMenuListSelection, frame, 0x8224A000, menu);
      const uint32_t t = uint32_t(tab);
      if (t < 1) {
        Base(base, b).set_current_list(Base(base, b).list0_address());
      } else if (t == 1) {
        Base(base, b).set_current_list(Base(base, b).list1_address());
      } else if (t < 3) {
        Base(base, b).set_current_list(Base(base, b).list2_address());
      }
      const uint32_t count = Base(base, b).current_list().count();
      if (int32_t(count) <= 0) {
        return Sound(3, frame, 0x8224A048, calls);
      }
      r3 = Sound(0, frame, 0x8224A054, calls);
      Base(base, b).set_tab(t);
      if (t == 0 || t == 1) {
        for (uint32_t i = 0, at = 0; int32_t(i) < int32_t(count); i++, at += 4) {
          uint32_t color = 0;
          if (t == Base(base, b).previous_tab() && i == Base(base, b).previous_entry()) {
            const uint32_t list = Base(base, b).current_list().address;
            color = Base(base, b).previous_color();
            Entry(base, LoadU32(base, at + list)).set_color(color);
          } else {
            Entry(base, LoadU32(base, at + Base(base, b).current_list().entries_address()))
                .set_color(color);
          }
        }
      }
      Leave(base, b, 1);
      Base(base, b).set_step(4);
      Base(base, b).set_queued_step(6);
      Base(base, b).set_next_page(1);
      Base(base, b).set_page(0xFFFFFFFFu);
      const uint32_t picked = Base(base, b).tab();
      const uint32_t before = Base(base, b).previous_tab();
      Base(base, b).set_scroll(0);
      Base(base, b).set_row(picked == before ? Base(base, b).previous_entry() : 0);
      return r3;
    }
    const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224A114, menu);
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    r3 = Sound(2, frame, 0x8224A124, calls);
    if (G::two_players(base) != 0) {
      Base(base, b).set_cancelled(1);
      if (Base(base, b).may_leave() == 0) {
        return r3;
      }
    }
    Leave(base, b, 5);
    Base(base, b).set_next_page(5);
    step = 3;
  }
  Base(base, b).set_page(0xFFFFFFFFu);
  Base(base, b).set_queued_step(0xFFFFFFFFu);
  Base(base, b).set_step(step);
  return r3;
}

uint64_t RowsPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A178 PanelSetBase::RowsPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60;
  Base(base, b).set_cancelled(0);
  calls.Call(kMenuListUpdate, frame, 0x8224A19C, menu);
  uint32_t step = 0, page = 0;
  uint64_t r3 = 0;
  if (Base(base, b).page_menu_last_move() != 0 ||
      Base(base, b).page_menu_scroll() != Base(base, b).page_menu_scroll_before()) {
    r3 = Sound(4, frame, 0x8224A330, calls);
    Leave(base, b, 1);
    step = 7;
    page = 1;
  } else {
    const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x8224A1C0, menu);
    if (uint32_t(accepted) != 0) {
      Sound(0, frame, 0x8224A1D0, calls);
      r3 = calls.Call(kMenuListSelection, frame, 0x8224A1D8, menu);
      const uint32_t scroll = Base(base, b).page_menu_scroll();
      const uint32_t rows = Base(base, b).rows().address;
      Base(base, b).set_row(uint32_t(r3));
      Base(base, b).set_scroll(scroll);
      if (rows != 0) {
        r3 = RowUnhighlight(base, rows, frame, calls);
      }
      uint32_t next = 0;
      if (Base(base, b).back_object().address != 0) {
        Leave(base, b, 8);
        Base(base, b).set_queued_step(0xFFFFFFFFu);
        Base(base, b).set_step(26);
        Base(base, b).set_page(0xFFFFFFFFu);
        next = 8;
      } else {
        r3 = calls.Call(kMenuListSetSelection, frame, 0x8224A23C, menu, 0);
        Leave(base, b, 2);
        Base(base, b).set_step(9);
        Base(base, b).set_queued_step(11);
        Base(base, b).set_page(0xFFFFFFFFu);
        next = 2;
      }
      Base(base, b).set_next_page(next);
      const uint32_t at = (Base(base, b).scroll() + Base(base, b).row()) << 2;
      const uint32_t list = Base(base, b).current_list().address;
      Base(base, b).set_color_scroll(0);
      Base(base, b).set_color(Entry(base, LoadU32(base, at + list)).color());
      return r3;
    }
    const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224A29C, menu);
    if (uint32_t(cancelled) == 0) {
      return cancelled;
    }
    r3 = Sound(2, frame, 0x8224A2AC, calls);
    if (Base(base, b).open_kind() == 1) {
      Leave(base, b, 0);
      Base(base, b).set_next_page(0);
      Base(base, b).set_step(8);
      Base(base, b).set_queued_step(1);
      Base(base, b).set_page(0xFFFFFFFFu);
      return r3;
    }
    if (G::two_players(base) != 0) {
      return r3;
    }
    Base(base, b).set_cancelled(1);
    if (Base(base, b).may_leave() == 0) {
      return r3;
    }
    Leave(base, b, 5);
    step = 3;
    page = 5;
  }
  Base(base, b).set_page(0xFFFFFFFFu);
  Base(base, b).set_next_page(page);
  Base(base, b).set_queued_step(0xFFFFFFFFu);
  Base(base, b).set_step(step);
  return r3;
}

uint64_t ColorsPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224AEA8 PanelSetBase::ColorsPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60;
  Base(base, b).set_show_stand_car(1);
  calls.Call(kMenuListUpdate, frame, 0x8224AECC, menu);
  if (Base(base, b).page_menu_last_move() != 0 ||
      Base(base, b).page_menu_scroll() != Base(base, b).page_menu_scroll_before()) {
    uint64_t r3 = Sound(4, frame, 0x8224AFFC, calls);
    Leave(base, b, 3);
    Base(base, b).set_queued_step(0xFFFFFFFFu);
    Base(base, b).set_step(19);
    Base(base, b).set_next_page(3);
    Base(base, b).set_page(0xFFFFFFFFu);
    if (Base(base, b).show_stand_car() == 1 && Base(base, b).stand_model_loaded() == 1) {
      r3 = ShowStandCar(base, b_r3, 0xFFFFFFFFFFFFFFFFull, frame, calls);
    }
    if (G::two_players(base) != 0) {
      return r3;
    }
    r3 = calls.Call(kMenuListSelection, frame, 0x8224B064, menu);
    const uint32_t scroll = Base(base, b).page_menu_scroll();
    Base(base, b).set_color(uint32_t(r3));
    Base(base, b).set_color_scroll(scroll);
    return r3;
  }
  const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x8224AEF0, menu);
  if (uint32_t(accepted) != 0) {
    Sound(0, frame, 0x8224AF00, calls);
    const uint64_t r3 = calls.Call(kMenuListSelection, frame, 0x8224AF08, menu);
    const uint32_t scroll = Base(base, b).page_menu_scroll();
    const uint32_t confirm = Base(base, b).confirm_panel().address;
    Base(base, b).set_color(uint32_t(r3));
    Base(base, b).set_color_scroll(scroll);
    uint32_t next = 0;
    if (confirm != 0) {
      Leave(base, b, 4);
      Base(base, b).set_step(21);
      Base(base, b).set_queued_step(22);
      Base(base, b).set_page(0xFFFFFFFFu);
      next = 4;
    } else {
      Leave(base, b, 5);
      Base(base, b).set_queued_step(0xFFFFFFFFu);
      Base(base, b).set_step(3);
      Base(base, b).set_page(0xFFFFFFFFu);
      next = 5;
    }
    Base(base, b).set_next_page(next);
    Base(base, b).set_picked(1);
    return r3;
  }
  const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224AF88, menu);
  if (uint32_t(cancelled) == 0) {
    return cancelled;
  }
  const uint64_t r3 = Sound(2, frame, 0x8224AF98, calls);
  const uint32_t at = (Base(base, b).scroll() + Base(base, b).row()) << 2;
  const uint32_t color =
      Entry(base, LoadU32(base, at + Base(base, b).current_list().entries_address())).color();
  Base(base, b).set_color_scroll(0);
  Base(base, b).set_color(color);
  Leave(base, b, 2);
  Base(base, b).set_step(20);
  Base(base, b).set_queued_step(11);
  Base(base, b).set_next_page(2);
  Base(base, b).set_page(0xFFFFFFFFu);
  return r3;
}

uint64_t PowerGraphPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224BDC0 PanelSetBase::PowerGraphPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60;
  Base(base, b).set_picked(0);
  const uint32_t asked = Base(base, b).power_graph_asked();
  if (G::two_players(base) != 0) {
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
    Base(base, b).set_step(16);
    Base(base, b).set_page(0xFFFFFFFFu);
    Base(base, b).set_next_page(2);
  } else if (asked == 0) {
    Leave(base, b, 2);
    Base(base, b).set_page(2);
    Base(base, b).set_step(0xFFFFFFFFu);
  } else {
    const uint64_t steps = uint64_t(Base(base, b).scroll()) + uint64_t(Base(base, b).row());
    if (Base(base, b).tab() == 2) {
      // The record's address as the original's mulli and adds leave it (64
      // bits).
      const uint64_t garage = SaveData(base, b, frame) + 6120;
      const uint32_t node = GarageNode(base, uint32_t(garage), uint32_t(steps));
      const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
      return calls.Call(kGaragePowerGraph, frame, 0x8224BF00,
                        uint64_t(slot * 1968) + garage + 212);
    }
    const uint32_t entry =
        LoadU32(base, Base(base, b).current_list().entries_address() + (uint32_t(steps) << 2));
    return calls.Call(kEntryPowerGraph, frame, 0x8224BF14, entry);
  }
  Base(base, b).set_queued_step(0xFFFFFFFFu);
  return CarSetButtons(base, Base(base, b).car_panel().address, 0, 0, 1, 1, frame, calls);
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
  using Fpscr = ::base::FloatMode;
  const auto flush_off = [] { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); };
  flush_off();
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 224);
  const uint64_t menu = b_r3 + 60;
  const uint32_t at = (Base(base, b).scroll() + Base(base, b).row()) << 2;
  const uint32_t limit = Base(base, b).colors_shown();
  const uint32_t entry = LoadU32(base, at + Base(base, b).current_list().entries_address());
  const uint32_t count = Entry(base, entry).color_count();
  const bool over = int32_t(count) > int32_t(limit);
  StoreU32(base, frame + 92, 158);
  StoreU32(base, frame + 84, 159);
  const uint64_t args[8] = {menu, count, over ? limit : count, 1, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, over ? 0x8224B2BC : 0x8224B2D8, args);
  calls.Call(kMenuListSetSelection, frame, 0x8224B2E4, menu, Base(base, b).color());
  Base(base, b).set_page_menu_scroll(Base(base, b).color_scroll());
  calls.Call(kMenuListFit, frame, 0x8224B2F8, menu, 1);
  if (Base(base, b).color_panel().address != 0) {
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224B30C, menu);
    ColorPlaceCursor(base, Base(base, b).color_panel().address, item, frame);
  }
  const uint32_t colors = Base(base, b).color_panel().address;
  if (colors != 0) {
    const uint32_t items = Base(base, b).page_menu_count();
    const uint32_t shown = Base(base, b).colors_shown();
    uint64_t up = 0, down = 0;
    if (int32_t(items) > int32_t(shown)) {
      const uint32_t scroll = Base(base, b).page_menu_scroll();
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
  const uint32_t list = Base(base, b).colors_address();
  while (Base(base, b).color_count() != 0) {
    const uint32_t n = ColorList(base, list).count() - 1;
    ColorList(base, list).set_count(n);
    const uint32_t record = LoadU32(base, ColorList(base, list).records_address() + 4 * n);
    if (record != 0) {
      FreePages(record, frame, 0x8224B3B0, calls);
    }
  }
  if (int32_t(count) > 0) {
    const double scale = double(G::color_scale(base));
    const double unit = double(G::color_unit(base));
    uint32_t source = Entry(base, entry).color_records_address();
    for (uint32_t left = count; left != 0; left--, source += 20) {
      const uint32_t record = memory::AllocPages(32, 4096);
      const uint32_t word = LoadU32(base, source + 4);
      const uint32_t raised = guest::LoadU16(base, source + 0);
      StoreU32(base, frame + 96, word);
      const uint32_t add = LoadU32(
          base, raised == 0 ? G::color_value_add_address() : G::color_value_add_raised_address());
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
      Color(base, record).set_hue(float(h));
      Color(base, record).set_saturation(float(double(float(s * unit))));
      Color(base, record).set_value(float(double(float(vv * unit))));
      tuning_pages::HsvToRgb(base, frame + 128, Color(base, record).hue_address(), frame);
      flush_off();
      const double red = double(float(double(guest::LoadFloat(base, frame + 128)) * scale));
      const double green = double(float(double(guest::LoadFloat(base, frame + 132)) * scale));
      const double blue = double(float(double(guest::LoadFloat(base, frame + 136)) * scale));
      Color(base, record).set_red(Fctiwz(red));
      Color(base, record).set_green(Fctiwz(green));
      Color(base, record).set_blue(Fctiwz(blue));
      const uint32_t n = ColorList(base, list).count();
      ColorList(base, list).set_count(n + 1);
      StoreU32(base, ColorList(base, list).records_address() + 4 * n, record);
    }
  }
  if (Base(base, b).color_panel().address != 0) {
    ColorSetCount(base, Base(base, b).color_panel().address, count, frame, calls);
    ColorSetSwatches(base, Base(base, b).color_panel().address, list,
                     Base(base, b).page_menu_scroll(), frame);
  }
  return ShowStandCar(base, b_r3, 0xFFFFFFFFFFFFFFFFull, frame, calls);
}

// --- The steps over the panels' shows and answers ---

namespace {
// The tabs' menu set up as ToTabs and TabsPage set it up (MenuList::Init
// with the two words at the frame's +84 and +92, in the order `first84`
// says) and its selection put back (+228), from return addresses `lr` and
// `lr` + 0x0C.
void TabsMenu(uint8_t* base, uint64_t b_r3, uint32_t frame, uint32_t lr, bool first84,
              const Calls& calls) {
  const uint32_t b = uint32_t(b_r3);
  if (first84) {
    StoreU32(base, frame + 84, 159);
    StoreU32(base, frame + 92, 158);
  } else {
    StoreU32(base, frame + 92, 158);
    StoreU32(base, frame + 84, 159);
  }
  const uint64_t args[8] = {b_r3 + 60, 3, 3, 1, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, lr, args);
  calls.Call(kMenuListSetSelection, frame, lr + 0x0C, b_r3 + 60, Base(base, b).tab_shown());
}
}  // namespace

uint64_t Step0(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A570 PanelSetBase::Step0");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 144);
  if (Word(base, word).first_pass() == 0) {
    const uint32_t tabs = Base(base, b).tabs().address;
    if (tabs != 0) {
      // F's +12, +16, +20: lists 0 to 2 aren't empty (the menu's words at
      // +92 and +84 stored between, as the original's).
      const uint32_t first = Base(base, b).list0_count() != 0 ? 1 : 0;
      StoreU32(base, frame + 92, 158);
      StoreU32(base, frame + 84, 159);
      Tab(base, tabs).set_tab0_enabled(first);
      Tab(base, Base(base, b).tabs().address)
          .set_tab1_enabled(Base(base, b).list1_count() != 0 ? 1 : 0);
      Tab(base, Base(base, b).tabs().address)
          .set_tab2_enabled(Base(base, b).list2_count() != 0 ? 1 : 0);
      const uint64_t args[8] = {b_r3 + 60, 3, 3, 1, 154, 153, 155, 156};
      calls.CallArgs(kMenuListInit, frame, 0x8224A618, args);
      calls.Call(kMenuListSetSelection, frame, 0x8224A624, b_r3 + 60, Base(base, b).tab_shown());
      TabSetTab(base, Base(base, b).tabs().address, Base(base, b).tab_shown(), frame, calls);
      TabShow(base, Base(base, b).tabs().address, frame, calls);
      TabSetButtons(base, Base(base, b).tabs().address, frame, calls);
    }
    if (Base(base, b).fade_with_steps() != 0) {
      Fade(base, kFadeIn, G::fade_in_passes_address(), frame, 0x8224A674, calls);
    }
    Word(base, word).set_first_pass(1);
  }
  const uint32_t tabs = Base(base, b).tabs().address;
  return tabs != 0 ? TabIsShown(base, tabs, frame) : 1;
}

uint64_t Step1(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224A6A0 PanelSetBase::Step1");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  SaveData(base, b, frame);  // unused
  const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224A6D4, b_r3 + 60);
  const uint32_t tabs = Base(base, b).tabs().address;
  if (tabs != 0) {
    Tab(base, tabs).set_tab(uint32_t(item));
    TabSelect(base, Base(base, b).tabs().address, frame, calls);
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step4(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B078 PanelSetBase::Step4");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() == 0) {
    calls.Call(kMenuListSelection, frame, 0x8224B0A8, b_r3 + 60);  // unused
    SetUpRows(base, b_r3, frame, calls);
    // F's buttons with no check that F is there (the original's).
    TabSetButtons(base, Base(base, b).tabs().address, frame, calls);
    const uint32_t rows = Base(base, b).rows().address;
    if (rows != 0) {
      RowShow(base, rows, frame, calls);
    }
    if (Base(base, b).open_kind() == 0) {
      Fade(base, kFadeIn, G::fade_in_passes_address(), frame, 0x8224B0FC, calls);
    }
    Word(base, word).set_first_pass(1);
  }
  const uint32_t rows = Base(base, b).rows().address;
  return rows != 0 ? RowIsShown(base, rows, frame) : 1;
}

uint64_t Step6(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248F70 PanelSetBase::Step6");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  const uint64_t item = calls.Call(kMenuListSelection, frame, 0x82248FA0, b_r3 + 60);
  const uint32_t rows = Base(base, b).rows().address;
  if (rows != 0) {
    Row(base, rows).set_row(uint32_t(item));
    RowSelect(base, Base(base, b).rows().address, frame, calls);
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step8(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248FE0 PanelSetBase::Step8");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (Word(base, word).first_pass() == 0) {
    const uint32_t rows = Base(base, b).rows().address;
    if (rows != 0) {
      RowHide(base, rows, frame, calls);
    }
    Base(base, b).set_tab_shown(Base(base, b).tab());
    Word(base, word).set_first_pass(1);
  }
  uint64_t answer = 1;
  const uint32_t rows = Base(base, b).rows().address;
  if (rows != 0) {
    answer = RowIsHidden(base, rows, frame);
    if (int32_t(uint32_t(answer)) == 0) {
      return answer;
    }
  }
  TabsMenu(base, b_r3, frame, 0x82249074, true, calls);
  TabSetTab(base, Base(base, b).tabs().address, Base(base, b).tab_shown(), frame, calls);
  return answer;
}

uint64_t Step9(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CB28 PanelSetBase::Step9");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() == 0) {
    const uint32_t car = Base(base, b).car_panel().address;
    if (car != 0) {
      CarShow(base, car, frame, calls);
    }
    SetUpCarPage(base, b_r3, frame, calls);
    CarSetButtons(base, Base(base, b).car_panel().address, 0, 0, 1, 1, frame, calls);
    // C's slot 3 (ShowArrows: the list has more than one entry): the plain
    // C++ where it holds Panel7C::ShowArrows, else a guest call.
    const uint32_t list = Base(base, b).current_list().address;
    const uint32_t panel = Base(base, b).car_panel().address;
    const uint64_t shown = List(base, list).count() > 1 ? 1 : 0;
    const uint32_t slot = Car(base, panel).vtable().show_arrows();
    if (slot == kPanel7CShowArrows) {
      Panel7CShowArrows(base, panel, shown, frame, calls);
    } else {
      calls.Call(slot, frame, 0x8224CBA4, panel, shown);
    }
    Word(base, word).set_first_pass(1);
  }
  uint64_t answer = 1;
  const uint32_t car = Base(base, b).car_panel().address;
  if (car != 0) {
    answer = CarIsShown(base, car, frame);
    if (int32_t(uint32_t(answer)) == 0) {
      return answer;
    }
  }
  Base(base, b).set_show_stand_car(1);
  Base(base, b).set_stand_model_loaded(0);
  if (G::two_players(base) == 0) {
    // GarageStage::SetDrawWord(stage 0, 1): its plain C++.
    const uint32_t holder = LoadU32(base, G::stand_holders(base).address);
    front_end_car_draw::GarageStageSetDrawWord(base, holder, 1, frame);
  }
  return answer;
}

uint64_t Step12(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249128 PanelSetBase::Step12");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (Word(base, word).first_pass() != 0) {
    return 1;
  }
  if (Base(base, b).car_panel().address != 0) {
    const uint64_t menu = b_r3 + 60;
    const uint64_t moves = Base(base, b).page_menu_last_move();
    const uint64_t before = calls.Call(kMenuListPrevious, frame, 0x82249164, menu);
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x82249170, menu);
    CarMove(base, Base(base, b).car_panel().address, item, before, moves, frame, calls);
  }
  Word(base, word).set_first_pass(1);
  return 1;
}

uint64_t Step19(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822492C8 PanelSetBase::Step19");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 160);
  uint64_t answer = 1;
  if (Word(base, word).first_pass() == 0) {
    if (Base(base, b).color_panel().address != 0) {
      const uint64_t list = b_r3 + 60;
      const uint64_t placed = Base(base, b).page_menu_wrapped();
      const uint64_t step = Base(base, b).page_menu_last_move();
      const uint64_t scroll = Base(base, b).page_menu_scroll();
      const uint64_t moves = uint64_t(Base(base, b).page_menu_scroll()) -
                             uint64_t(Base(base, b).page_menu_scroll_before());
      const uint64_t before = calls.Call(kMenuListPrevious, frame, 0x8224931C, list);
      const uint64_t item = calls.Call(kMenuListSelection, frame, 0x82249328, list);
      ColorMove(base, Base(base, b).color_panel().address, item, before, moves, scroll, step,
                placed, Base(base, b).colors_address(), frame, calls);
    }
    Word(base, word).set_first_pass(1);
  }
  const uint32_t colors = Base(base, b).color_panel().address;
  if (colors != 0) {
    // D's arrows: up and down by the colors' count (+60), the rows shown
    // (+1924) and the scroll (+64).
    const int32_t count = int32_t(Base(base, b).page_menu_count());
    const int32_t shown = int32_t(Base(base, b).colors_shown());
    uint64_t up = 0, down = 0;
    if (count > shown) {
      const uint32_t scroll = Base(base, b).page_menu_scroll();
      if (scroll == 0) {
        down = 1;
      } else {
        up = 1;
        down = int32_t(uint32_t(shown) + scroll) == count ? 0 : 1;
      }
    }
    ColorSetArrows(base, colors, up, down, frame);
    answer = ColorIsIdle(base, Base(base, b).color_panel().address, frame);
    if (int32_t(uint32_t(answer)) == 0) {
      return answer;
    }
  }
  // D's swatches (with no check that D is there, the original's).
  ColorSetSwatches(base, Base(base, b).color_panel().address, Base(base, b).colors_address(),
                   Base(base, b).page_menu_scroll(), frame);
  return answer;
}

uint64_t Step17(uint8_t* base, uint64_t b_r3, uint32_t word, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224C450 PanelSetBase::Step17");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Word(base, word).first_pass() == 0) {
    const uint32_t car = Base(base, b).car_panel().address;
    if (car != 0) {
      CarStopArrows(base, car, frame, calls);
    }
    const uint32_t colors = Base(base, b).color_panel().address;
    if (colors != 0) {
      ColorShow(base, colors, frame, calls);
    }
    SetUpColors(base, b_r3, frame, calls);
    Word(base, word).set_first_pass(1);
  }
  const uint32_t colors = Base(base, b).color_panel().address;
  return colors != 0 ? ColorIsShown(base, colors, frame) : 1;
}

uint64_t CarPage(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224B8D8 PanelSetBase::CarPage");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t menu = b_r3 + 60, rows = b_r3 + 132;
  // The garage's record `steps` links on (SaveData's address + 6120, the
  // record's +212), as the original's 64-bit adds and mulli leave it.
  const auto garage_record = [&](uint32_t steps) {
    const uint64_t garage = SaveData(base, b, frame) + 6120;
    const uint32_t node = GarageNode(base, uint32_t(garage), steps);
    const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
    return uint64_t(slot * 1968) + garage + 212;
  };
  const auto entry_at = [&](uint32_t steps) {
    return LoadU32(base, Base(base, b).current_list().entries_address() + (steps << 2));
  };
  calls.Call(kMenuListUpdate, frame, 0x8224B8F4, menu);
  if (Base(base, b).page_menu_last_move() != 0 ||
      Base(base, b).page_menu_scroll() != Base(base, b).page_menu_scroll_before()) {
    const uint64_t r3 = Sound(4, frame, 0x8224BD88, calls);
    Leave(base, b, 2);
    Base(base, b).set_queued_step(0xFFFFFFFFu);
    Base(base, b).set_step(12);
    Base(base, b).set_page(0xFFFFFFFFu);
    Base(base, b).set_next_page(2);
    return r3;
  }
  const uint64_t accepted = calls.Call(kMenuListAccepted, frame, 0x8224B918, menu);
  if (uint32_t(accepted) != 0) {
    Sound(0, frame, 0x8224B928, calls);
    const uint64_t row = calls.Call(kMenuListSelection, frame, 0x8224B930, rows);
    Base(base, b).set_scroll(Base(base, b).rows_menu_scroll());
    Base(base, b).set_row(uint32_t(row));
    const uint64_t before = calls.Call(kMenuListPrevious, frame, 0x8224B948, menu);
    const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224B954, menu);
    CarPick(base, Base(base, b).car_panel().address, item, before, frame, calls);
    uint64_t r3 = calls.Call(kMenuListSelection, frame, 0x8224B96C, menu);
    const uint32_t action = uint32_t(r3);
    if (action > 1) {
      return r3;
    }
    if (action == 1) {
      // The power graph.
      const uint32_t steps = Base(base, b).row() + Base(base, b).scroll();
      if (G::two_players(base) != 0) {
        if (Base(base, b).tab() == 2) {
          const uint64_t record = garage_record(steps);
          r3 = calls.Call(kShowCarStats, frame, 0x8224B9F0, Base(base, b).car_panel().address,
                          record);
        } else {
          r3 = CarShowStats(base, Base(base, b).car_panel().address, entry_at(steps), frame, calls);
        }
        Leave(base, b, 6);
        Base(base, b).set_step(15);
        Base(base, b).set_page(0xFFFFFFFFu);
        Base(base, b).set_next_page(6);
      } else {
        const uint32_t tab = Base(base, b).tab();
        if (tab == 2) {
          const uint64_t record = garage_record(steps);
          power_graph::SetFromCar(
              base, uint32_t(record), Base(base, b).tab(), frame,
              [&calls](uint32_t function, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t at,
                       uint32_t lr) { calls.Call(function, at, lr, r3, r4, r5); });
        } else {
          PowerGraphSetEntry(base, entry_at(steps), tab);
        }
        r3 = CarSetButtons(base, Base(base, b).car_panel().address, 0, 1, 0xFFFFFFFFFFFFFFFFull,
                           0xFFFFFFFFFFFFFFFFull, frame, calls);
        Leave(base, b, 6);
        Base(base, b).set_page(6);
        Base(base, b).set_step(0xFFFFFFFFu);
      }
      Base(base, b).set_queued_step(0xFFFFFFFFu);
      Base(base, b).set_power_graph_asked(1);
      Base(base, b).set_picked(1);
      return r3;
    }
    // Action 0: the pick.
    const uint32_t tab = Base(base, b).tab();
    if (Base(base, b).confirm_panel().address != 0) {
      if (tab == 0) {
        Leave(base, b, 3);
        Base(base, b).set_step(17);
        Base(base, b).set_queued_step(18);
        Base(base, b).set_page(0xFFFFFFFFu);
        Base(base, b).set_next_page(3);
      } else {
        Leave(base, b, 4);
        Base(base, b).set_step(21);
        Base(base, b).set_queued_step(22);
        Base(base, b).set_next_page(4);
        Base(base, b).set_page(0xFFFFFFFFu);
        Base(base, b).set_picked(1);
      }
      return r3;
    }
    if (tab == 1 || tab == 2) {
      Leave(base, b, 5);
      Base(base, b).set_queued_step(0xFFFFFFFFu);
      Base(base, b).set_step(3);
      Base(base, b).set_next_page(5);
      Base(base, b).set_page(0xFFFFFFFFu);
      Base(base, b).set_picked(1);
      return r3;
    }
    Leave(base, b, 3);
    Base(base, b).set_step(17);
    Base(base, b).set_queued_step(18);
    Base(base, b).set_next_page(3);
    Base(base, b).set_page(0xFFFFFFFFu);
    return ShowStandCar(base, b_r3, Base(base, b).color(), frame, calls);
  }
  const uint64_t cancelled = calls.Call(kMenuListCancelled, frame, 0x8224BC00, menu);
  if (uint32_t(cancelled) != 0) {
    uint64_t r3 = Sound(2, frame, 0x8224BC10, calls);
    const uint32_t panel = Base(base, b).rows().address;
    if (panel != 0) {
      r3 = RowUnhighlight(base, panel, frame, calls);
    }
    const bool confirm = Base(base, b).back_object().address != 0;
    Leave(base, b, confirm ? 7 : 1);
    Base(base, b).set_step(14);
    Base(base, b).set_queued_step(confirm ? 26 : 6);
    Base(base, b).set_page(0xFFFFFFFFu);
    Base(base, b).set_next_page(confirm ? 7 : 1);
    return r3;
  }
  const uint64_t updated = calls.Call(kMenuListUpdate, frame, 0x8224BC74, rows);
  if (Base(base, b).rows_menu_last_move() == 0 &&
      Base(base, b).rows_menu_scroll() == Base(base, b).rows_menu_scroll_before()) {
    return updated;
  }
  Sound(4, frame, 0x8224BC98, calls);
  Leave(base, b, 2);
  Base(base, b).set_queued_step(0xFFFFFFFFu);
  Base(base, b).set_step(13);
  Base(base, b).set_next_page(2);
  Base(base, b).set_page(0xFFFFFFFFu);
  if (Base(base, b).tab() == 2) {
    const uint64_t data = SaveData(base, b, frame);
    const uint64_t row = calls.Call(kMenuListSelection, frame, 0x8224BCE4, rows);
    const uint64_t garage = data + 6120;
    const uint32_t node =
        GarageNode(base, uint32_t(garage), uint32_t(row) + Base(base, b).rows_menu_scroll());
    const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
    calls.Call(kShowCarStats, frame, 0x8224BD24, Base(base, b).car_panel().address,
               uint64_t(slot * 1968) + garage + 212);
  } else {
    const uint64_t row = calls.Call(kMenuListSelection, frame, 0x8224BD30, rows);
    CarShowStats(base, Base(base, b).car_panel().address,
                 entry_at(uint32_t(row) + Base(base, b).rows_menu_scroll()), frame, calls);
  }
  Base(base, b).set_color_scroll(0);
  const uint64_t row = calls.Call(kMenuListSelection, frame, 0x8224BD60, rows);
  Base(base, b).set_color(
      Entry(base, entry_at(uint32_t(row) + Base(base, b).rows_menu_scroll())).color());
  return row;
}

}  // namespace car_list
