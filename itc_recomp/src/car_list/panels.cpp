// The car list's panels and panel groups (docs/functions/car-list.md): see
// car_list.h.

#include "car_list/car_list.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/car_list.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;
namespace steps = camera_modes::steps;
using G = schema::CarListGlobals;
using Group = schema::PanelGroupBase;
using Part = schema::PanelPart;
using Tab = schema::TabPanel;
using Row = schema::RowPanel;
using ColorP = schema::ColorPanel;
using Car = schema::CarPanel;
using Entry = schema::CarListEntry;
using List = schema::CarListList;
using ColorList = schema::CarListColorList;
using Color = schema::CarListColor;
using Stats = schema::CarListStats;
using CarVtable = schema::CarPanelVtable;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// PanelGroup::Movie's plain C++ on the group `group` from `frame`.
uint32_t Movie(uint8_t* base, uint32_t group, uint32_t frame) {
  return uint32_t(type67_screens::PanelGroupMovie(base, group, frame));
}

// A 64-bit word (std) at `at`.
void StoreDouble(uint8_t* base, uint32_t at, uint64_t word) {
  StoreU32(base, at, uint32_t(word >> 32));
  StoreU32(base, at + 4, uint32_t(word));
}

// lfd.
double LoadDouble(uint8_t* base, uint32_t at) {
  return std::bit_cast<double>((uint64_t(LoadU32(base, at)) << 32) | LoadU32(base, at + 4));
}

// (float) of a signed word, as extsw, std at frame + 80, fcfid, frsp leave
// it.
double FloatOf(uint8_t* base, uint32_t frame, int64_t word) {
  StoreDouble(base, frame + 80, uint64_t(word));
  return steps::Single(double(word));
}

// The C panel's stats child index `i` (0 to 21: its +160 + 4 x i).
uint32_t StatsChild(uint8_t* base, uint32_t panel, uint32_t i) {
  return LoadU32(base, Car(base, panel).stats_children_address() + 4 * i);
}

// UiMovie::PlayNamed(m, named, r5, r6, r7, r8).
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t m, uint64_t named,
                   uint64_t chained, uint64_t reversed, uint64_t w32, uint64_t paused) {
  const uint64_t args[8] = {m, named, chained, reversed, w32, paused, 0, 0};
  return calls.CallArgs(kPlayNamed, frame, lr, args);
}

// The finish, play (once, not reversed) and loop of a named animation the
// panels' Select functions make: FinishNamed, PlayNamed(m, a, 1, 0, 0, 0),
// LoopNamed, from return addresses `lr`, `lr` + 0x28 and + 0x34 (the
// sequence's call sites in the originals vary: each given).
uint64_t Replay(const Calls& calls, uint32_t frame, uint32_t m, uint64_t named, uint32_t lr_finish,
                uint32_t lr_play, uint32_t lr_loop) {
  calls.Call(kFinishNamed, frame, lr_finish, m, named);
  PlayNamed(calls, frame, lr_play, m, named, 1, 0, 0, 0);
  return calls.Call(kLoopNamed, frame, lr_loop, m, named);
}

}  // namespace

// --- The panel groups ---

uint64_t SetCaption(uint8_t* base, uint32_t group, uint64_t id, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224EF18 PanelGroup::SetCaption");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, group, frame);
  const uint32_t node = Group(base, group).caption_node();
  if (int32_t(node) < 0) {
    return m;
  }
  calls.Call(kSetText, frame, 0x8224EF4C, m, Group(base, group).text_node_set(), node, id);
  calls.Call(kScrollText, frame, 0x8224EF60, m, Group(base, group).text_node_set(),
             Group(base, group).caption_node(), 1);
  return calls.Call(kResetTextScroll, frame, 0x8224EF70, m, Group(base, group).text_node_set(),
                    Group(base, group).caption_node(), 1);
}

uint64_t PartFinishAll(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249CF0 PanelPart::FinishAll");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, Part(base, panel).group().address, frame);
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  return calls.Call(kFinishAll, frame, 0x82249D08, m, frame - 96 + 80);
}

// --- The tabs F ---

uint64_t TabSetCursorY(uint8_t* base, uint32_t panel, uint64_t y, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224FD20 TabPanel::SetCursorY");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, Tab(base, panel).node_set(), Tab(base, panel).texts_node());
  const uint32_t child = ui_movies::Child(base, node, Tab(base, panel).cursor());
  if (child == 0) {
    return child;
  }
  const double value = FloatOf(base, frame, int64_t(int32_t(uint32_t(y))));
  steps::Stfs(base, child + 16, value);
  return child;
}

uint64_t TabSetButtons(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224FD90 TabPanel::SetButtons");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  static constexpr uint32_t kTextures[4] = {0, 0, 1, 1};
  static constexpr uint32_t kLr[6] = {0x8224FDC4, 0x8224FDDC, 0x8224FDF4,
                                      0x8224FE0C, 0x8224FE20, 0x8224FE34};
  for (uint32_t i = 0; i < 4; i++) {
    calls.Call(kSetNodeTexture, frame, kLr[i], m, Tab(base, panel).buttons_node(),
               Tab(base, panel).buttons() + i, kTextures[i]);
  }
  // HideNode: r6 the last SetNodeTexture's 1 (unread).
  calls.Call(kHideNode, frame, kLr[4], m, Tab(base, panel).buttons_node(),
             Tab(base, panel).buttons() + 4, 1);
  return calls.Call(kHideNode, frame, kLr[5], m, Tab(base, panel).buttons_node(),
                    Tab(base, panel).buttons() + 5, 1);
}


// The two nodes' +108 by the tab (0, 1, 2; nothing for 3 and above).
void SetTabWords(uint8_t* base, uint32_t a, uint32_t b, uint64_t tab) {
  const uint32_t t = uint32_t(tab);
  if (t >= 3) {
    return;
  }
  StoreU32(base, a + 108, t);
  StoreU32(base, b + 108, t);
}

uint64_t TabSetTab(uint8_t* base, uint32_t panel, uint64_t tab, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822501C8 TabPanel::SetTab");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  Tab(base, panel).set_tab(uint32_t(tab));
  TabSetCursorY(base, panel, tab * 40, frame);
  static constexpr uint32_t kLr[3] = {0x8225022C, 0x8225022C, 0x8225022C};
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t node =
        ui_movies::FindNode(base, m, Tab(base, panel).node_set(), Tab(base, panel).texts_node());
    const uint32_t child =
        ui_movies::Child(base, node, LoadU32(base, Tab(base, panel).tab0_text_address() + 4 * i));
    calls.Call(kSetNodeText, frame, kLr[i], m, child, 124 + i);
  }
  const uint32_t node =
      ui_movies::FindNode(base, m, Tab(base, panel).node_set(), Tab(base, panel).tabs_node());
  const uint32_t a = ui_movies::Child(base, node, Tab(base, panel).tab_word_a());
  const uint32_t b = ui_movies::Child(base, node, Tab(base, panel).tab_word_b());
  SetTabWords(base, a, b, tab);
  const uint64_t id = Tab(base, panel).two_players() != 0 ? 929 : tab + 930;
  return SetCaption(base, Tab(base, panel).group().address, id, frame, calls);
}

uint64_t TabMove(uint8_t* base, uint32_t panel, uint64_t tab, uint64_t before, uint64_t moves,
                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224FF88 TabPanel::Move");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, Tab(base, panel).node_set(), Tab(base, panel).tabs_node());
  const uint32_t a = ui_movies::Child(base, node, Tab(base, panel).tab_word_a());
  const uint32_t b = ui_movies::Child(base, node, Tab(base, panel).tab_word_b());
  SetTabWords(base, a, b, tab);
  bool enabled = false;
  calls.Call(kFinishNamed, frame, 0x82250024, m, Tab(base, panel).move_anim());
  PlayNamed(calls, frame, 0x82250040, m, Tab(base, panel).move_anim(), 1, 0, 0, 0);
  calls.Call(kFinishNamed, frame, 0x82250050, m, Tab(base, panel).tab_anims() + before);
  // The tab's word (+12, +16, +20 for tabs 0 to 2): 1 its animation played
  // and looped, else the moves' two animations played and finished.
  const int32_t t = int32_t(uint32_t(tab));
  const bool known = t == 0 || t == 1 || t == 2;
  if (known && LoadU32(base, Tab(base, panel).tab0_enabled_address() + 4 * uint32_t(t)) == 1) {
    enabled = true;
    calls.Call(kFinishNamed, frame, 0x82250098, m, Tab(base, panel).tab_anims() + tab);
    PlayNamed(calls, frame, 0x822500B8, m, Tab(base, panel).tab_anims() + tab, 1, 0, 0, 0);
    calls.Call(kLoopNamed, frame, 0x822500C8, m, Tab(base, panel).tab_anims() + tab);
  } else {
    const uint32_t c = Tab(base, panel).unknown_108();
    if (int32_t(c) >= 0) {
      PlayNamed(calls, frame, 0x822500F0, m, c, 0, 0, 0, 0);
      calls.Call(kFinishNamed, frame, 0x822500FC, m, Tab(base, panel).unknown_108());
    }
    const uint32_t d = Tab(base, panel).unknown_112();
    if (int32_t(d) >= 0) {
      PlayNamed(calls, frame, 0x82250120, m, d, 1, 1, 0, 0);
      calls.Call(kFinishNamed, frame, 0x8225012C, m, Tab(base, panel).unknown_112());
    }
  }
  calls.Call(kFinishNamed, frame, 0x82250138, m, Tab(base, panel).scroll_down());
  uint64_t r3 = calls.Call(kFinishNamed, frame, 0x82250144, m, Tab(base, panel).scroll_up());
  const int32_t step = int32_t(uint32_t(tab - before));
  if (step > 1 || step < -1) {
    r3 = TabSetCursorY(base, panel, tab * 40, frame);
  } else if (int32_t(uint32_t(moves)) == 1) {
    r3 = PlayNamed(calls, frame, 0x8225017C, m, Tab(base, panel).scroll_up(), 1, 0, 0, 0);
  } else if (int32_t(uint32_t(moves)) == -1) {
    r3 = PlayNamed(calls, frame, 0x8225017C, m, Tab(base, panel).scroll_down(), 1, 0, 0, 0);
  }
  if (Tab(base, panel).two_players() != 0) {
    return r3;
  }
  return SetCaption(base, Tab(base, panel).group().address, enabled ? tab + 930 : 584, frame,
                    calls);
}

// --- The car rows E ---

uint64_t RowSetRow(uint8_t* base, uint32_t panel, uint64_t row, uint32_t entry, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F018 RowPanel::SetRow");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  uint64_t r3 = ui_movies::FindNode(base, m, Row(base, panel).node_set(), Row(base, panel).node());
  uint32_t node = 0;
  const uint32_t child =
      int32_t(uint32_t(row)) >= 0
          ? LoadU32(base, Row(base, panel).row_nodes_address() + 4 * uint32_t(row))
          : Row(base, panel).spare_row_node();
  if (int32_t(child) >= 0) {
    r3 = ui_movies::Child(base, uint32_t(r3), child);
    node = uint32_t(r3);
  }
  const uint32_t logo = Row(base, panel).logo();
  if (int32_t(logo) >= 0) {
    r3 = ui_movies::Child(base, node, logo);
    const int32_t id = int32_t(Entry(base, entry).car_id());
    int32_t frame_word;
    if (id < 35) {
      frame_word = id == 2 ? 35 : id;
    } else if (id == 43) {
      frame_word = 35;
    } else if (uint32_t(id - 47) <= 9) {
      frame_word = id - 11;
    } else {
      frame_word = id == 71 ? 44 : -1;
    }
    StoreU32(base, uint32_t(r3) + 108, uint32_t(frame_word));
  }
  // The texts: SetNodeText(m, the child, string) with r6 to r10 unset (0).
  const uint32_t text0 = Row(base, panel).text_maker();
  if (int32_t(text0) >= 0) {
    const uint32_t c = ui_movies::Child(base, node, text0);
    r3 = calls.Call(kSetNodeText, frame, 0x8224F100, m, c,
                    uint32_t(Entry(base, entry).maker()) + 1934);
  }
  const uint32_t text1 = Row(base, panel).text_name();
  if (int32_t(text1) >= 0) {
    const uint32_t c = ui_movies::Child(base, node, text1);
    r3 = calls.Call(kSetNodeText, frame, 0x8224F128, m, c, Entry(base, entry).car_id() + 1646);
  }
  const uint32_t text2 = Row(base, panel).text_third();
  if (int32_t(text2) >= 0) {
    const uint32_t c = ui_movies::Child(base, node, text2);
    r3 = calls.Call(kSetNodeText, frame, 0x8224F150, m, c, Entry(base, entry).car_id() + 1718);
  }
  return r3;
}

uint64_t RowFill(uint8_t* base, uint64_t panel_r3, uint32_t list, uint64_t scroll, uint64_t item,
                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F758 RowPanel::Fill");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 192);
  const int32_t count = int32_t(List(base, list).count());
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, Row(base, panel).node_set(), Row(base, panel).node());
  const uint32_t first = List(base, list).entries_address() + (uint32_t(scroll) << 2);
  for (int32_t i = 0; i < int32_t(Row(base, panel).row_count()); i++) {
    const uint32_t row = ui_movies::Child(
        base, node, LoadU32(base, Row(base, panel).row_nodes_address() + 4 * uint32_t(i)));
    if (i < count) {
      StoreU32(base, row + 80, 1);
      RowSetRow(base, panel, uint32_t(i), LoadU32(base, first + 4 * uint32_t(i)), frame, calls);
    } else {
      StoreU32(base, row + 80, 0);
    }
  }
  const uint32_t sub = Row(base, panel).node_set();
  const uint32_t cursor = int32_t(sub) >= 0
                              ? ui_movies::FindNode(base, m, sub, Row(base, panel).cursor())
                              : ui_movies::Child(base, node, Row(base, panel).cursor());
  StoreU32(base, cursor + 80, count > int32_t(Row(base, panel).row_count()) ? 1 : 0);
  if (Row(base, panel).two_players() == 0) {
    const uint32_t entry =
        LoadU32(base, List(base, list).entries_address() + (uint32_t(scroll + item) << 2));
    SetCaption(base, Row(base, panel).group().address, Entry(base, entry).car_id() + 1862, frame,
               calls);
  }
  return RowSetArrows(base, panel_r3, scroll, List(base, list).count(), frame);
}

uint64_t RowRefresh(uint8_t* base, uint64_t panel_r3, uint32_t list, uint64_t scroll, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F888 RowPanel::Refresh");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = panel_r3;
  const uint32_t first = List(base, list).entries_address() + (uint32_t(scroll) << 2);
  for (int32_t i = 0; i < int32_t(Row(base, panel).row_count()); i++) {
    if (uint32_t(i) + uint32_t(scroll) < List(base, list).count()) {
      r3 = RowSetRow(base, panel, uint32_t(i), LoadU32(base, first + 4 * uint32_t(i)), frame,
                     calls);
    }
  }
  return r3;
}

uint64_t RowMove(uint8_t* base, uint64_t panel_r3, uint64_t row, uint64_t before, uint64_t moves,
                 uint64_t scroll, uint32_t list, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F8F8 RowPanel::Move");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  Row(base, panel).set_row(uint32_t(row));
  calls.Call(kFinishNamed, frame, 0x8224F938, m, before + Row(base, panel).row_anims());
  calls.Call(kFinishNamed, frame, 0x8224F948, m, row + Row(base, panel).row_anims());
  PlayNamed(calls, frame, 0x8224F968, m, row + Row(base, panel).row_anims(), 1, 0, 0, 0);
  calls.Call(kLoopNamed, frame, 0x8224F978, m, row + Row(base, panel).row_anims());
  if (uint32_t(moves) != 0) {
    const uint32_t first = List(base, list).entries_address() + (uint32_t(scroll) << 2);
    for (uint32_t i = 0;; i++) {
      uint32_t count = List(base, list).count();
      const uint32_t rows = Row(base, panel).row_count();
      if (count >= rows) count = rows;
      if (i >= count) break;
      RowSetRow(base, panel, i, LoadU32(base, first + 4 * i), frame, calls);
    }
    calls.Call(kFinishNamed, frame, 0x8224F9D4, m, Row(base, panel).scroll_up());
    calls.Call(kFinishNamed, frame, 0x8224F9E0, m, Row(base, panel).scroll_down());
    calls.Call(kFinishNamed, frame, 0x8224F9EC, m, Row(base, panel).scroll_up());
    calls.Call(kFinishNamed, frame, 0x8224F9F8, m, Row(base, panel).scroll_down());
    uint32_t named;
    if (int32_t(uint32_t(moves)) < 0) {
      const uint32_t at = uint32_t(Row(base, panel).row_count() + scroll) << 2;
      RowSetRow(base, panel, 0xFFFFFFFFFFFFFFFFull,
                LoadU32(base, List(base, list).entries_address() + at), frame, calls);
      named = Row(base, panel).scroll_down();
    } else {
      RowSetRow(base, panel, 0xFFFFFFFFFFFFFFFFull, LoadU32(base, first - 4), frame, calls);
      named = Row(base, panel).scroll_up();
    }
    PlayNamed(calls, frame, 0x8224FA48, m, named, 1, 0, 0, 0);
  }
  if (Row(base, panel).two_players() == 0) {
    const uint32_t entry =
        LoadU32(base, List(base, list).entries_address() + (uint32_t(row + scroll) << 2));
    SetCaption(base, Row(base, panel).group().address, Entry(base, entry).car_id() + 1862, frame,
               calls);
  }
  return RowSetArrows(base, panel_r3, scroll, List(base, list).count(), frame);
}

uint64_t RowSetArrows(uint8_t* base, uint64_t panel_r3, uint64_t scroll, uint64_t count,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224F680 RowPanel::SetArrows");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (int32_t(Row(base, panel).node()) < 0 || int32_t(Row(base, panel).arrows()) < 0) {
    return panel_r3;
  }
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, Row(base, panel).node_set(), Row(base, panel).node());
  const auto arrow = [&](uint32_t offset) {
    const uint32_t sub = Row(base, panel).node_set();
    const uint32_t index = Row(base, panel).arrows() + offset;
    return int32_t(sub) >= 0 ? ui_movies::FindNode(base, m, sub, index)
                             : ui_movies::Child(base, node, index);
  };
  const uint32_t up = arrow(0);
  StoreU32(base, up + 80, uint32_t(scroll) != 0 ? 1 : 0);
  const uint32_t down = arrow(1);
  StoreU32(base, down + 80,
           Row(base, panel).row_count() + uint32_t(scroll) - uint32_t(count) != 0 ? 1 : 0);
  return down;
}

uint64_t RowUnhighlight(uint8_t* base, uint64_t panel_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F558 RowPanel::Unhighlight");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Row(base, panel).two_players() == 0) {
    return panel_r3;
  }
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  calls.Call(kFinishNamed, frame, 0x8224F598, m,
             Row(base, panel).row() + Row(base, panel).row_anims());
  PlayNamed(calls, frame, 0x8224F5BC, m,
            Row(base, panel).row_anims() + Row(base, panel).row_count(), 1, 0, 0, 0);
  return calls.Call(kFinishNamed, frame, 0x8224F5D0, m,
                    Row(base, panel).row_anims() + Row(base, panel).row_count());
}

uint64_t RowHide(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F4C8 RowPanel::Hide");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  const uint32_t row = Row(base, panel).row();
  if (int32_t(row) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224F504, m, Row(base, panel).row_anims() + row);
    Row(base, panel).set_row(0xFFFFFFFFu);
  }
  calls.Call(kFinishNamed, frame, 0x8224F518, m, Row(base, panel).arrow_up());
  calls.Call(kFinishNamed, frame, 0x8224F524, m, Row(base, panel).arrow_down());
  return PlayNamed(calls, frame, 0x8224F540, m, Row(base, panel).hide_anim(), 1, 0, 1, 0);
}

// --- The colors D ---

uint64_t ColorShow(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D010 ColorPanel::Show");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  uint64_t r3 = PlayNamed(calls, frame, 0x8224D04C, m, ColorP(base, panel).show_anim(), 1, 0, 1, 0);
  const uint32_t up = ColorP(base, panel).arrow_a();
  if (int32_t(up) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224D060, m, up);
  }
  const uint32_t down = ColorP(base, panel).arrow_b();
  if (int32_t(down) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224D074, m, down);
  }
  return r3;
}

uint64_t ColorSelect(uint8_t* base, uint32_t panel, uint64_t item, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D118 ColorPanel::Select");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  calls.Call(kFinishNamed, frame, 0x8224D140, m, ColorP(base, panel).item_anims());
  PlayNamed(calls, frame, 0x8224D160, m, ColorP(base, panel).item_anims() + item, 1, 0, 0, 0);
  uint64_t r3 =
      calls.Call(kLoopNamed, frame, 0x8224D170, m, ColorP(base, panel).item_anims() + item);
  if (int32_t(ColorP(base, panel).arrow_a()) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224D184, m, ColorP(base, panel).arrow_a());
    PlayNamed(calls, frame, 0x8224D1A0, m, ColorP(base, panel).arrow_a(), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, 0x8224D1AC, m, ColorP(base, panel).arrow_a());
  }
  if (int32_t(ColorP(base, panel).arrow_b()) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224D1C0, m, ColorP(base, panel).arrow_b());
    PlayNamed(calls, frame, 0x8224D1DC, m, ColorP(base, panel).arrow_b(), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, 0x8224D1E8, m, ColorP(base, panel).arrow_b());
  }
  return r3;
}

uint64_t ColorSetArrows(uint8_t* base, uint32_t panel, uint64_t up, uint64_t down, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D2D0 ColorPanel::SetArrows");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  uint32_t node = ui_movies::FindNode(base, m, ColorP(base, panel).node_set(),
                                      ColorP(base, panel).arrows_node());
  // cmpw: the low words.
  StoreU32(base, node + 80, uint32_t(up) == uint32_t(down) ? uint32_t(up) : 1);
  const uint32_t a = ColorP(base, panel).arrow_up_node();
  if (int32_t(a) >= 0) {
    node = ui_movies::FindNode(base, m, ColorP(base, panel).node_set(), a);
    StoreU32(base, node + 80, uint32_t(up));
  }
  const uint32_t b = ColorP(base, panel).arrow_down_node();
  if (int32_t(b) >= 0) {
    node = ui_movies::FindNode(base, m, ColorP(base, panel).node_set(), b);
    StoreU32(base, node + 80, uint32_t(down));
  }
  return node;
}

uint64_t ColorShowSwatches(uint8_t* base, uint32_t panel, uint64_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D358 ColorPanel::ShowSwatches");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, ColorP(base, panel).node_set(), ColorP(base, panel).node());
  const uint32_t swatches = ui_movies::Child(base, node, ColorP(base, panel).swatches());
  uint32_t r3 = swatches;
  for (int32_t i = 0; i < int32_t(LoadU32(base, swatches + 104)); i++) {
    r3 = ui_movies::Child(base, node, uint32_t(i) + LoadU32(base, swatches + 100));
    StoreU32(base, r3 + 80, i < int32_t(uint32_t(count)) ? 1 : 0);
  }
  return r3;
}

uint64_t ColorPlaceCursor(uint8_t* base, uint32_t panel, uint64_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D3E0 ColorPanel::PlaceCursor");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, ColorP(base, panel).node_set(), ColorP(base, panel).node());
  const double y = FloatOf(base, frame, int64_t(int32_t((uint32_t(item) << 5) + 8)));
  const uint32_t child = ui_movies::Child(base, node, ColorP(base, panel).cursor());
  steps::Stfs(base, child + 16, y);
  return child;
}

uint64_t ColorMove(uint8_t* base, uint64_t panel_r3, uint64_t item, uint64_t before, uint64_t moves,
                   uint64_t scroll, uint64_t step, uint64_t placed, uint32_t list, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D580 ColorPanel::Move");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  calls.Call(kFinishNamed, frame, 0x8224D5C4, m, ColorP(base, panel).item_anims() + before);
  calls.Call(kFinishNamed, frame, 0x8224D5D4, m, ColorP(base, panel).item_anims() + item);
  PlayNamed(calls, frame, 0x8224D5F4, m, ColorP(base, panel).item_anims() + item, 1, 0, 0, 0);
  calls.Call(kLoopNamed, frame, 0x8224D604, m, ColorP(base, panel).item_anims() + item);
  calls.Call(kFinishNamed, frame, 0x8224D610, m, ColorP(base, panel).scroll_back());
  uint64_t r3 = calls.Call(kFinishNamed, frame, 0x8224D61C, m, ColorP(base, panel).scroll_on());
  if (uint32_t(placed) == 0) {
    const int32_t s = int32_t(uint32_t(step));
    if (s != 0) {
      r3 = PlayNamed(calls, frame, 0x8224D654, m,
                     s > 0 ? ColorP(base, panel).scroll_on() : ColorP(base, panel).scroll_back(), 1,
                     0, 0, 0);
    }
  } else {
    r3 = ColorPlaceCursor(base, panel, item, frame);
  }
  const int32_t moved = int32_t(uint32_t(moves));
  if (moved == 0) {
    return r3;
  }
  ColorSetSwatches(base, panel_r3, list, scroll, frame);
  calls.Call(kFinishNamed, frame, 0x8224D688, m, ColorP(base, panel).unknown_44());
  r3 = calls.Call(kFinishNamed, frame, 0x8224D694, m, ColorP(base, panel).unknown_48());
  return PlayNamed(
      calls, frame, 0x8224D6C4, m,
      moved < 0 ? ColorP(base, panel).swatches_back() : ColorP(base, panel).swatches_on(), 1, 0, 0,
      0);
}

uint64_t ColorSetCount(uint8_t* base, uint32_t panel, uint64_t count, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D458 ColorPanel::SetCount");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = ColorShowSwatches(base, panel, count, frame);
  if (ColorP(base, panel).two_players() != 0) {
    return r3;
  }
  return SetCaption(base, ColorP(base, panel).group().address, 552, frame, calls);
}

uint64_t ColorSetSwatch(uint8_t* base, uint32_t panel, uint64_t swatch, uint32_t color,
                        uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D4A0 ColorPanel::SetSwatch");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, ColorP(base, panel).node_set(), ColorP(base, panel).node());
  const uint32_t swatches = ui_movies::Child(base, node, ColorP(base, panel).swatches());
  const uint32_t at = ui_movies::Child(base, node, LoadU32(base, swatches + 100) + uint32_t(swatch));
  const uint32_t child = ui_movies::Child(base, at, ColorP(base, panel).swatch_color());
  StoreU32(base, child + 56, Color(base, color).red());
  StoreU32(base, child + 60, Color(base, color).green());
  StoreU32(base, child + 64, Color(base, color).blue());
  return child;
}

uint64_t ColorSetSwatches(uint8_t* base, uint64_t panel, uint32_t list, uint64_t scroll,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D518 ColorPanel::SetSwatches");
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = panel;
  for (uint32_t i = 0; i < ColorList(base, list).count(); i++) {
    const uint32_t count = ColorList(base, list).count();
    const uint32_t at = (i + uint32_t(scroll)) % count;
    r3 = ColorSetSwatch(base, uint32_t(panel), i,
                        LoadU32(base, ColorList(base, list).records_address() + 4 * at), frame);
  }
  return r3;
}

// --- The car's info C ---

uint64_t CarSetTexts(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D7F0 CarPanel::SetTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, Car(base, panel).node_set(), Car(base, panel).node());
  const uint32_t a = ui_movies::Child(base, node, Car(base, panel).texts());
  calls.Call(kSetNodeText, frame, 0x8224D834, m, a, Car(base, panel).text_a());
  const uint32_t b = ui_movies::Child(base, node, Car(base, panel).texts() + 1);
  return calls.Call(kSetNodeText, frame, 0x8224D854, m, b, Car(base, panel).text_b());
}

uint64_t CarSetButtons(uint8_t* base, uint32_t panel, uint64_t a, uint64_t b, uint64_t c,
                       uint64_t d, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D860 CarPanel::SetButtons");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  const uint64_t icons[4] = {a, b, c, d};
  // Each: hidden (-1) or shown with its texture; the calls' sites.
  static constexpr uint32_t kHideLr[4] = {0x8224D8A0, 0x8224D8D8, 0x8224D914, 0x8224D950};
  static constexpr uint32_t kShowLr[4] = {0x8224D8A8, 0x8224D8E0, 0x8224D91C, 0x8224D958};
  static constexpr uint32_t kTextureLr[4] = {0x8224D8BC, 0x8224D8F8, 0x8224D934, 0x8224D970};
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t node = Car(base, panel).buttons_node();
    const uint32_t child = Car(base, panel).buttons() + i;
    if (int32_t(uint32_t(icons[i])) == -1) {
      calls.Call(kHideNode, frame, kHideLr[i], m, node, child);
    } else {
      calls.Call(kShowNode, frame, kShowLr[i], m, node, child);
      calls.Call(kSetNodeTexture, frame, kTextureLr[i], m, Car(base, panel).buttons_node(),
                 Car(base, panel).buttons() + i, icons[i]);
    }
  }
  calls.Call(kHideNode, frame, 0x8224D984, m, Car(base, panel).buttons_node(),
             Car(base, panel).buttons() + 4);
  return calls.Call(kHideNode, frame, 0x8224D998, m, Car(base, panel).buttons_node(),
                    Car(base, panel).buttons() + 5);
}

uint64_t CarPlaceCursor(uint8_t* base, uint64_t panel_r3, uint64_t action, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D9A0 CarPanel::PlaceCursor");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (int32_t(Car(base, panel).cursor()) < 0) {
    return panel_r3;
  }
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, Car(base, panel).node_set(), Car(base, panel).node());
  const double y = FloatOf(base, frame, int64_t(int32_t(uint32_t(action) * 40)));
  const uint32_t child = ui_movies::Child(base, node, Car(base, panel).cursor());
  steps::Stfs(base, child + 16, y);
  return child;
}

uint64_t CarSelect(uint8_t* base, uint32_t panel, uint64_t arrows, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DCF0 CarPanel::Select");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  uint64_t r3 = m;
  const uint32_t action = Car(base, panel).action();
  if (int32_t(action) >= 0) {
    const auto named = [&] {
      return LoadU32(base, Car(base, panel).action_anims_address() + 4 * Car(base, panel).action());
    };
    calls.Call(kFinishNamed, frame, 0x8224DD2C, m, named());
    PlayNamed(calls, frame, 0x8224DD54, m, named(), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, 0x8224DD6C, m, named());
  }
  if (uint32_t(arrows) == 0) {
    return r3;
  }
  if (int32_t(Car(base, panel).arrow_a()) >= 0) {
    r3 = Replay(calls, frame, m, Car(base, panel).arrow_a(), 0x8224DD88, 0x8224DDA4, 0x8224DDB0);
  }
  if (int32_t(Car(base, panel).arrow_b()) >= 0) {
    r3 = Replay(calls, frame, m, Car(base, panel).arrow_b(), 0x8224DDC4, 0x8224DDE0, 0x8224DDEC);
  }
  return r3;
}

uint64_t CarStopArrows(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DF30 CarPanel::StopArrows");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  uint64_t r3 = m;
  if (int32_t(Car(base, panel).arrow_a()) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224DF64, m, Car(base, panel).arrow_a());
  }
  if (int32_t(Car(base, panel).arrow_b()) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224DF78, m, Car(base, panel).arrow_b());
  }
  return r3;
}

uint64_t CarSetName(uint8_t* base, uint32_t panel, uint32_t entry, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DED8 CarPanel::SetName");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  if (Car(base, panel).two_players() != 0) {
    return m;
  }
  return SetCaption(base, Car(base, panel).group().address, Entry(base, entry).car_id() + 1862,
                    frame, calls);
}

uint64_t CarSetStats(uint8_t* base, uint32_t panel, uint32_t record, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224E288 CarPanel::SetStats");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  const uint32_t node = ui_movies::FindNode(base, m, 0xFFFFFFFFu, Car(base, panel).stats_node());
  uint64_t r3 = node;
  // SetNodeText(m, the stats child `at`, r5, r6) when the child index isn't
  // negative; r6 to r10 unset where the original leaves them (0).
  const auto text = [&](uint32_t at, uint32_t lr, uint64_t r5, uint64_t r6) {
    const uint32_t index = StatsChild(base, panel, at);
    if (int32_t(index) < 0) {
      return false;
    }
    const uint32_t child = ui_movies::Child(base, node, index);
    r3 = calls.Call(kSetNodeText, frame, lr, m, child, r5, r6);
    return true;
  };
  {
    const uint32_t index = StatsChild(base, panel, 0);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E2E0, m, child,
                      Stats(base, record).unknown_0() + 421);
    }
  }
  text(1, 0x8224E308, Stats(base, record).turbo() + 426, 0);
  text(2, 0x8224E330, 8, Stats(base, record).displacement());
  {
    const uint32_t index = StatsChild(base, panel, 3);
    if (int32_t(index) >= 0) {
      const int64_t word = int64_t(int32_t(Stats(base, record).unknown_8()));
      StoreDouble(base, frame + 80, uint64_t(word));
      const double scaled =
          steps::Single(steps::Single(double(word)) * LoadDouble(base, G::stats_scale_a_address()));
      const uint32_t child = ui_movies::Child(base, node, index);
      const int32_t truncated = steps::Fctiwz(scaled);
      StoreU32(base, frame + 80, uint32_t(truncated));
      r3 = calls.Call(kSetNodeText, frame, 0x8224E388, m, child, 8, uint32_t(truncated));
    }
  }
  text(4, 0x8224E3B0, 8, Stats(base, record).unknown_16());
  {
    const uint32_t index = StatsChild(base, panel, 5);
    if (int32_t(index) >= 0) {
      const double scaled =
          steps::Single(steps::Lfs(base, Stats(base, record).unknown_24_address()) *
                        LoadDouble(base, G::stats_scale_b_address()));
      const uint32_t child = ui_movies::Child(base, node, index);
      const uint64_t bits = std::bit_cast<uint64_t>(scaled);
      StoreDouble(base, frame + 40, bits);
      const uint64_t args[8] = {m, child, 436, bits, 0, 0, 0, 0};
      r3 = calls.call(calls.user, kSetNodeText, args, &scaled, frame, 0x8224E3F4);
    }
  }
  text(14, 0x8224E418, 438, 0);
  text(15, 0x8224E43C, 440, 0);
  text(16, 0x8224E460, 437, 0);
  text(17, 0x8224E484, 439, 0);
  return r3;
}

uint64_t CarSetCarName(uint8_t* base, uint32_t panel, uint64_t id, uint64_t kind, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822417E8 CarPanel::SetCarName");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  uint64_t r3 = m;
  // UiMovie::SetText(m, -1, the node at `at`, string): r7 to r10 unset (0).
  const auto text = [&](uint32_t at, uint32_t lr, uint64_t string) {
    const uint32_t node = LoadU32(base, at);
    if (int32_t(node) >= 0) {
      r3 = calls.Call(kSetText, frame, lr, m, 0xFFFFFFFFFFFFFFFFull, node, string);
    }
  };
  text(Car(base, panel).text_maker_address(), 0x82241824, kind + 1934);
  text(Car(base, panel).text_name_address(), 0x82241840, id + 1646);
  text(Car(base, panel).text_third_address(), 0x8224185C, id + 1718);
  return r3;
}

uint64_t BaseSetCarName(uint8_t* base, uint32_t panel, uint64_t id, uint64_t kind, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224E878 CarPanelBase::SetCarName");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  const uint32_t node = ui_movies::FindNode(base, m, 0xFFFFFFFFu, Car(base, panel).name_node());
  uint64_t r3 = node;
  // SetNodeText(m, child [at], string): r6 to r10 unset (0).
  const auto text = [&](uint32_t at, uint32_t lr, uint64_t string) {
    const uint32_t index = LoadU32(base, at);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, lr, m, child, string);
    }
  };
  text(Car(base, panel).text_maker_address(), 0x8224E8CC, kind + 1934);
  text(Car(base, panel).text_name_address(), 0x8224E8F0, id + 1646);
  text(Car(base, panel).text_third_address(), 0x8224E914, id + 1718);
  return r3;
}

uint64_t Panel6CSetStats(uint8_t* base, uint32_t panel, uint32_t record, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224E490 Panel6C::SetStats");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  const uint32_t node = ui_movies::FindNode(base, m, 0xFFFFFFFFu, Car(base, panel).stats_node());
  uint64_t r3 = node;
  // SetNodeText(m, the stats child `at`, r5, r6) when the child index isn't
  // negative; r6 to r10 unset where the original leaves them (0).
  const auto text = [&](uint32_t at, uint32_t lr, uint64_t r5, uint64_t r6) {
    const uint32_t index = StatsChild(base, panel, at);
    if (int32_t(index) < 0) {
      return;
    }
    const uint32_t child = ui_movies::Child(base, node, index);
    r3 = calls.Call(kSetNodeText, frame, lr, m, child, r5, r6);
  };
  text(0, 0x8224E4E4, 263, 0);
  text(3, 0x8224E508, 259, 0);
  text(2, 0x8224E52C, 266, 0);
  text(4, 0x8224E550, 267, 0);
  text(5, 0x8224E574, 268, 0);
  {
    const uint32_t index = StatsChild(base, panel, 6);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E59C, m, child,
                      Stats(base, record).unknown_0() + 421);
    }
  }
  {
    const uint32_t index = StatsChild(base, panel, 8);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E5C4, m, child, 8,
                      Stats(base, record).displacement());
    }
  }
  {
    const uint32_t index = StatsChild(base, panel, 9);
    if (int32_t(index) >= 0) {
      const int64_t word = int64_t(int32_t(Stats(base, record).unknown_8()));
      StoreDouble(base, frame + 80, uint64_t(word));
      const double scaled =
          steps::Single(steps::Single(double(word)) * LoadDouble(base, G::stats_scale_a_address()));
      const uint32_t child = ui_movies::Child(base, node, index);
      const int32_t truncated = steps::Fctiwz(scaled);
      StoreU32(base, frame + 80, uint32_t(truncated));
      r3 = calls.Call(kSetNodeText, frame, 0x8224E61C, m, child, 8, uint32_t(truncated));
    }
  }
  {
    const uint32_t index = StatsChild(base, panel, 10);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E644, m, child, 8,
                      Stats(base, record).unknown_16());
    }
  }
  {
    const uint32_t index = StatsChild(base, panel, 11);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E66C, m, child, 8,
                      Stats(base, record).unknown_20());
    }
  }
  {
    const uint32_t index = StatsChild(base, panel, 12);
    if (int32_t(index) >= 0) {
      const double scaled =
          steps::Single(steps::Lfs(base, Stats(base, record).unknown_24_address()) *
                        LoadDouble(base, G::stats_scale_b_address()));
      const uint32_t child = ui_movies::Child(base, node, index);
      const uint64_t bits = std::bit_cast<uint64_t>(scaled);
      StoreDouble(base, frame + 40, bits);
      const uint64_t args[8] = {m, child, 436, bits, 0, 0, 0, 0};
      r3 = calls.call(calls.user, kSetNodeText, args, &scaled, frame, 0x8224E6B0);
    }
  }
  {
    const uint32_t index = StatsChild(base, panel, 13);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E6D8, m, child, 8,
                      Stats(base, record).unknown_28());
    }
  }
  text(15, 0x8224E6FC, 440, 0);
  text(14, 0x8224E720, 438, 0);
  text(16, 0x8224E744, 437, 0);
  text(17, 0x8224E768, 439, 0);
  for (const uint32_t at : {18u, 19u}) text(at, 0x8224E794, 448, 0);
  for (const uint32_t at : {20u, 21u}) text(at, 0x8224E7CC, 449, 0);
  return r3;
}

uint64_t Panel7CShowArrows(uint8_t* base, uint32_t panel, uint64_t shown, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82241868 Panel7C::ShowArrows");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  if (uint32_t(shown) == 0) {
    return calls.Call(kHideNode, frame, 0x82241898, m, 0xFFFFFFFFFFFFFFFFull, 8);
  }
  return calls.Call(kShowNode, frame, 0x822418A0, m, 0xFFFFFFFFFFFFFFFFull, 8);
}

uint64_t CarShowStats(uint8_t* base, uint64_t panel_r3, uint32_t entry, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224E920 CarPanel::ShowStats");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 144);
  Movie(base, Car(base, panel).group().address, frame);
  // The stats record at frame + 80: +770, +709 set, +712, +648, then +652,
  // +656 as integers, +660, +664 as an integer.
  const uint32_t vtable = Car(base, panel).vtable().address;
  StoreU32(base, frame + 88, Entry(base, entry).unknown_712());
  StoreU32(base, frame + 80, Entry(base, entry).unknown_770());
  StoreU32(base, frame + 92, Entry(base, entry).displacement());
  StoreU32(base, frame + 84, Entry(base, entry).turbo() != 0 ? 1 : 0);
  StoreU32(base, frame + 96,
           uint32_t(steps::Fctiwz(steps::Lfs(base, Entry(base, entry).unknown_652_address()))));
  StoreU32(base, frame + 100,
           uint32_t(steps::Fctiwz(steps::Lfs(base, Entry(base, entry).unknown_656_address()))));
  steps::Stfs(base, frame + 104, steps::Lfs(base, Entry(base, entry).unknown_660_address()));
  StoreU32(base, frame + 108,
           uint32_t(steps::Fctiwz(steps::Lfs(base, Entry(base, entry).unknown_664_address()))));
  const uint32_t stats = CarVtable(base, vtable).set_stats();
  if (stats == kSetStats) {
    CarSetStats(base, panel, frame + 80, frame, calls);
  } else {
    calls.Call(stats, frame, 0x8224E9C4, panel_r3, frame + 80);
  }
  const uint32_t name = Car(base, panel).vtable().set_car_name();
  const uint64_t id = Entry(base, entry).car_id();
  const uint64_t kind = Entry(base, entry).maker();
  uint64_t r3;
  if (name == kSetCarName) {
    r3 = CarSetCarName(base, panel, id, kind, frame, calls);
  } else if (name == kBaseSetCarName) {
    r3 = BaseSetCarName(base, panel, id, kind, frame, calls);
  } else {
    r3 = calls.Call(name, frame, 0x8224E9E0, panel_r3, id, kind);
  }
  if (Car(base, panel).shows_price() != 0) {
    r3 = calls.Call(kSetPrice, frame, 0x8224E9F8, panel_r3, Entry(base, entry).price());
  }
  return r3;
}

// --- The panels' answers (UiMovie::IsNamedActive's plain C++, a leaf) ---

namespace {
// Whether named animation `named` of the movie is playing or looping.
bool Active(uint8_t* base, uint32_t m, uint32_t named) {
  return ui_movies::IsNamedActive(base, m, named) != 0;
}
}  // namespace

uint64_t TabIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224FCE0 TabPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  return Active(base, m, Tab(base, panel).show_anim()) ? 0 : 1;
}

uint64_t RowIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224F5E8 RowPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  const uint32_t named = Row(base, panel).show_alt() != 0 ? Row(base, panel).show_anim_alt()
                                                          : Row(base, panel).show_anim();
  return Active(base, m, named) ? 0 : 1;
}

uint64_t RowIsHidden(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224F640 RowPanel::IsHidden");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  return Active(base, m, Row(base, panel).hide_anim()) ? 0 : 1;
}

uint64_t CarIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224E108 CarPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  for (uint32_t i = 0; i < 2; i++) {
    if (Active(base, m, LoadU32(base, Car(base, panel).show_anim_a_address() + 4 * i))) {
      return 0;
    }
  }
  return 1;
}

uint64_t ColorIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D1F0 ColorPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  return Active(base, m, ColorP(base, panel).show_anim()) ? 0 : 1;
}

uint64_t ColorIsIdle(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D288 ColorPanel::IsIdle");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, ColorP(base, panel).group().address, frame);
  const bool a = Active(base, m, ColorP(base, panel).swatches_back());
  const bool b = Active(base, m, ColorP(base, panel).swatches_on());
  return a || b ? 0 : 1;
}

// --- The panels' shows, selections and moves ---

namespace {
// The loop the shows and selections start when it isn't running: for the
// word at `at` (nothing when negative), IsNamedActive (r3 1 when it's
// running), else PlayNamed(m, it, 1, 0, 0, 0) and LoopNamed from `lr_play`
// and `lr_loop`. The word read again for each call, as the originals do.
void LoopIfIdle(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t m, uint32_t at,
                uint32_t lr_play, uint32_t lr_loop, uint64_t& r3) {
  const uint32_t named = LoadU32(base, at);
  if (int32_t(named) < 0) {
    return;
  }
  if (Active(base, m, named)) {
    r3 = 1;
    return;
  }
  PlayNamed(calls, frame, lr_play, m, LoadU32(base, at), 1, 0, 0, 0);
  r3 = calls.Call(kLoopNamed, frame, lr_loop, m, LoadU32(base, at));
}

// FinishNamed on the word at `at` when it isn't negative.
void FinishIfSet(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t m, uint32_t at,
                 uint32_t lr, uint64_t& r3) {
  const uint32_t named = LoadU32(base, at);
  if (int32_t(named) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, lr, m, named);
  }
}

// The sum of two of the panel's words (their guest addresses), as the
// originals' 64-bit add of two zero-extended loads.
uint64_t Sum(uint8_t* base, uint32_t a, uint32_t b) {
  return uint64_t(LoadU32(base, a)) + uint64_t(LoadU32(base, b));
}

// The C panel's action word for `action` (its action_anims + 4 x action,
// 32-bit).
uint32_t ActionAt(uint8_t* base, uint32_t panel, uint64_t action) {
  return Car(base, panel).action_anims_address() + (uint32_t(action) << 2);
}
}  // namespace

uint64_t TabShow(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224FE50 TabPanel::Show");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  const uint32_t again = Movie(base, Tab(base, panel).group().address, frame);
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8224FE80, again, frame - 96 + 80);
  const uint32_t first = Tab(base, panel).show_anim();
  if (int32_t(first) >= 0) {
    r3 = PlayNamed(calls, frame, 0x8224FEA4, m, first, 1, 0, 0, 0);
  }
  LoopIfIdle(base, calls, frame, m, Tab(base, panel).loop_a_address(), 0x8224FEDC, 0x8224FEE8, r3);
  LoopIfIdle(base, calls, frame, m, Tab(base, panel).loop_c_address(), 0x8224FF20, 0x8224FF2C, r3);
  LoopIfIdle(base, calls, frame, m, Tab(base, panel).loop_d_address(), 0x8224FF64, 0x8224FF70, r3);
  return r3;
}

uint64_t TabSelect(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224FB20 TabPanel::Select");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Tab(base, panel).group().address, frame);
  uint64_t r3 = m;
  // The tab (+8, unsigned) and its word (+12, +16, +20): when set, its
  // animation (+96 + the tab) finished, played and looped.
  const uint32_t tab = Tab(base, panel).tab();
  if (tab < 3 && LoadU32(base, Tab(base, panel).tab0_enabled_address() + 4 * tab) != 0) {
    calls.Call(kFinishNamed, frame, 0x8224FB9C, m, uint64_t(Tab(base, panel).tab_anims()) + tab);
    PlayNamed(calls, frame, 0x8224FBC0, m,
              Sum(base, Tab(base, panel).tab_address(), Tab(base, panel).tab_anims_address()), 1, 0,
              0, 0);
    r3 =
        calls.Call(kLoopNamed, frame, 0x8224FBD4, m,
                   Sum(base, Tab(base, panel).tab_address(), Tab(base, panel).tab_anims_address()));
  }
  LoopIfIdle(base, calls, frame, m, Tab(base, panel).loop_b_address(), 0x8224FC0C, 0x8224FC18, r3);
  LoopIfIdle(base, calls, frame, m, Tab(base, panel).loop_c_address(), 0x8224FC50, 0x8224FC5C, r3);
  // The three tabs' nodes dimmed (color 0) or not (2) by their words.
  const uint32_t node =
      ui_movies::FindNode(base, m, Tab(base, panel).node_set(), Tab(base, panel).texts_node());
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t child =
        ui_movies::Child(base, node, LoadU32(base, Tab(base, panel).tab0_text_address() + 4 * i));
    ui_movies::SetColor(
        base, child, LoadU32(base, Tab(base, panel).tab0_enabled_address() + 4 * i) == 0 ? 2 : 0);
  }
  return m;
}

uint64_t RowShow(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F1D8 RowPanel::Show");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  const uint32_t first = Row(base, panel).show_alt() != 0 ? Row(base, panel).show_anim_alt()
                                                          : Row(base, panel).show_anim();
  uint64_t r3 = PlayNamed(calls, frame, 0x8224F228, m, first, 1, 0, 1, 0);
  LoopIfIdle(base, calls, frame, m, Row(base, panel).loop_a_address(), 0x8224F260, 0x8224F26C, r3);
  LoopIfIdle(base, calls, frame, m, Row(base, panel).loop_b_address(), 0x8224F2A4, 0x8224F2B0, r3);
  FinishIfSet(base, calls, frame, m, Row(base, panel).arrow_up_address(), 0x8224F2C4, r3);
  FinishIfSet(base, calls, frame, m, Row(base, panel).arrow_down_address(), 0x8224F2D8, r3);
  if (int32_t(Row(base, panel).row()) >= 0) {
    r3 =
        calls.Call(kFinishNamed, frame, 0x8224F2F4, m,
                   Sum(base, Row(base, panel).row_anims_address(), Row(base, panel).row_address()));
    Row(base, panel).set_row(0xFFFFFFFFu);
  }
  FinishIfSet(base, calls, frame, m, Row(base, panel).arrow_up_address(), 0x8224F310, r3);
  FinishIfSet(base, calls, frame, m, Row(base, panel).arrow_down_address(), 0x8224F324, r3);
  return r3;
}

uint64_t RowSelect(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F340 RowPanel::Select");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, Row(base, panel).group().address, frame);
  calls.Call(kFinishNamed, frame, 0x8224F374, m,
             Sum(base, Row(base, panel).row_address(), Row(base, panel).row_anims_address()));
  PlayNamed(calls, frame, 0x8224F398, m,
            Sum(base, Row(base, panel).row_address(), Row(base, panel).row_anims_address()), 1, 0,
            0, 0);
  uint64_t r3 =
      calls.Call(kLoopNamed, frame, 0x8224F3AC, m,
                 Sum(base, Row(base, panel).row_address(), Row(base, panel).row_anims_address()));
  LoopIfIdle(base, calls, frame, m, Row(base, panel).loop_a_address(), 0x8224F3E4, 0x8224F3F0, r3);
  LoopIfIdle(base, calls, frame, m, Row(base, panel).loop_b_address(), 0x8224F428, 0x8224F434, r3);
  // The arrows (+104, +108) finished, played and looped.
  static constexpr uint32_t kArrows[2][3] = {{0x8224F448, 0x8224F464, 0x8224F470},
                                             {0x8224F484, 0x8224F4A0, 0x8224F4AC}};
  const uint32_t arrows[2] = {Row(base, panel).arrow_up_address(),
                              Row(base, panel).arrow_down_address()};
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t* const a = kArrows[k];
    const uint32_t named = LoadU32(base, arrows[k]);
    if (int32_t(named) < 0) {
      continue;
    }
    calls.Call(kFinishNamed, frame, a[0], m, named);
    PlayNamed(calls, frame, a[1], m, LoadU32(base, arrows[k]), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, a[2], m, LoadU32(base, arrows[k]));
  }
  return r3;
}

uint64_t CarShow(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DA20 CarPanel::Show");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  uint64_t r3 = m;
  const uint32_t action = Car(base, panel).action();
  if (int32_t(action) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224DA58, m, LoadU32(base, ActionAt(base, panel, action)));
    Car(base, panel).set_action(0xFFFFFFFFu);
  }
  // The ten actions' animations (+68 on): played once when not running,
  // then finished.
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t named = LoadU32(base, Car(base, panel).action_anims_address() + 4 * i);
    if (!Active(base, m, named)) {
      PlayNamed(calls, frame, 0x8224DA9C, m, named, 1, 0, 0, 0);
    }
    r3 = calls.Call(kFinishNamed, frame, 0x8224DAA8, m, named);
  }
  for (uint32_t i = 0; i < 2; i++) {
    r3 = PlayNamed(calls, frame, 0x8224DAD8, m,
                   LoadU32(base, Car(base, panel).show_anim_a_address() + 4 * i), 1, 0, 1, 0);
  }
  LoopIfIdle(base, calls, frame, m, Car(base, panel).loop_a_address(), 0x8224DB1C, 0x8224DB28, r3);
  LoopIfIdle(base, calls, frame, m, Car(base, panel).loop_b_address(), 0x8224DB60, 0x8224DB6C, r3);
  FinishIfSet(base, calls, frame, m, Car(base, panel).arrow_a_address(), 0x8224DB80, r3);
  FinishIfSet(base, calls, frame, m, Car(base, panel).arrow_b_address(), 0x8224DB94, r3);
  return r3;
}

namespace {
// Move's and Pick's shared part: the action before's animation finished
// and unlooped, the new one's finished, unlooped, played and looped, and
// the scroll animations (+108, +112) finished. Its return addresses from
// `lr` (Move's 0x8224DE30, Pick's 0x8224E084), the same steps apart.
void CarSwap(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t m, uint32_t panel,
             uint64_t action, uint64_t before, uint32_t lr) {
  const uint32_t old_at = ActionAt(base, panel, before);
  calls.Call(kFinishNamed, frame, lr, m, LoadU32(base, old_at));
  calls.Call(kUnloopNamed, frame, lr + 0x0C, m, LoadU32(base, old_at));
  const uint32_t new_at = ActionAt(base, panel, action);
  calls.Call(kFinishNamed, frame, lr + 0x20, m, LoadU32(base, new_at));
  calls.Call(kUnloopNamed, frame, lr + 0x2C, m, LoadU32(base, new_at));
  PlayNamed(calls, frame, lr + 0x48, m, LoadU32(base, new_at), 1, 0, 0, 0);
  calls.Call(kLoopNamed, frame, lr + 0x54, m, LoadU32(base, new_at));
  calls.Call(kFinishNamed, frame, lr + 0x60, m, Car(base, panel).scroll_back());
}
}  // namespace

uint64_t CarMove(uint8_t* base, uint32_t panel, uint64_t action, uint64_t before,
                 uint64_t direction, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DDF8 CarPanel::Move");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  CarSwap(base, calls, frame, m, panel, action, before, 0x8224DE30);
  uint64_t r3 = calls.Call(kFinishNamed, frame, 0x8224DE9C, m, Car(base, panel).scroll_on());
  const int32_t d = int32_t(uint32_t(direction));
  if (d == 1) {
    r3 = PlayNamed(calls, frame, 0x8224DED0, m, Car(base, panel).scroll_on(), 1, 0, 0, 0);
  } else if (d == -1) {
    r3 = PlayNamed(calls, frame, 0x8224DED0, m, Car(base, panel).scroll_back(), 1, 0, 0, 0);
  }
  return r3;
}

uint64_t CarPick(uint8_t* base, uint64_t panel_r3, uint64_t action, uint64_t before, uint32_t sp,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224E050 CarPanel::Pick");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, Car(base, panel).group().address, frame);
  CarSwap(base, calls, frame, m, panel, action, before, 0x8224E084);
  calls.Call(kFinishNamed, frame, 0x8224E0F0, m, Car(base, panel).scroll_on());
  return CarPlaceCursor(base, panel_r3, action, frame);
}

// --- The power graph's record ---

uint64_t PowerGraphSetEntry(uint8_t* base, uint64_t entry_r3, uint64_t tab) {
  ITC_NATIVE_RUNNING("sub_82250368 PowerGraph::SetEntry");
  const uint32_t entry = uint32_t(entry_r3);
  const uint32_t record = G::power_graph(base).address + 8;
  StoreU32(base, record + 80, uint32_t(tab));
  StoreU32(base, record + 0, 0);
  StoreU32(base, record + 4, Entry(base, entry).unknown_712());
  StoreU32(base, record + 8, Entry(base, entry).unknown_716());
  StoreU32(base, record + 12, Entry(base, entry).unknown_724());
  StoreU32(base, record + 16, Entry(base, entry).unknown_720());
  StoreU32(base, record + 20, Entry(base, entry).car_id());
  StoreU32(base, record + 24, Entry(base, entry).maker());
  StoreU32(base, record + 28, Entry(base, entry).unknown_770());
  // The switch on the byte +708 (its cases 0 and 4 the split piece
  // sub_822503F0); nothing above 7.
  static constexpr uint32_t kKinds[8] = {6, 2, 3, 4, 6, 0, 1, 5};
  const uint8_t kind = Entry(base, entry).unknown_708();
  if (kind <= 7) {
    StoreU32(base, record + 32, kKinds[kind]);
  }
  StoreU32(base, record + 36, Entry(base, entry).turbo() != 0 ? 1 : 0);
  StoreU32(base, record + 40, Entry(base, entry).displacement());
  StoreU32(base, record + 44,
           uint32_t(steps::Fctiwz(steps::Lfs(base, Entry(base, entry).unknown_652_address()))));
  StoreU32(base, record + 48,
           uint32_t(steps::Fctiwz(steps::Lfs(base, Entry(base, entry).unknown_656_address()))));
  steps::Stfs(base, record + 52, steps::Lfs(base, Entry(base, entry).unknown_660_address()));
  StoreU32(base, record + 56,
           uint32_t(steps::Fctiwz(steps::Lfs(base, Entry(base, entry).unknown_664_address()))));
  return entry_r3;
}

}  // namespace car_list
