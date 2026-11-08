// The car list's panels and panel groups (docs/functions/car-list.md): see
// car_list.h.

#include "car_list/car_list.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;
namespace steps = camera_modes::steps;

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
  const uint32_t node = LoadU32(base, group + 32);
  if (int32_t(node) < 0) {
    return m;
  }
  calls.Call(kSetText, frame, 0x8224EF4C, m, LoadU32(base, group + 12), node, id);
  calls.Call(kScrollText, frame, 0x8224EF60, m, LoadU32(base, group + 12),
             LoadU32(base, group + 32), 1);
  return calls.Call(kResetTextScroll, frame, 0x8224EF70, m, LoadU32(base, group + 12),
                    LoadU32(base, group + 32), 1);
}

uint64_t PartFinishAll(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249CF0 PanelPart::FinishAll");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  return calls.Call(kFinishAll, frame, 0x82249D08, m, frame - 96 + 80);
}

// --- The tabs F ---

uint64_t TabSetCursorY(uint8_t* base, uint32_t panel, uint64_t y, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224FD20 TabPanel::SetCursorY");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 24), LoadU32(base, panel + 28));
  const uint32_t child = ui_movies::Child(base, node, LoadU32(base, panel + 40));
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
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  static constexpr uint32_t kTextures[4] = {0, 0, 1, 1};
  static constexpr uint32_t kLr[6] = {0x8224FDC4, 0x8224FDDC, 0x8224FDF4,
                                      0x8224FE0C, 0x8224FE20, 0x8224FE34};
  for (uint32_t i = 0; i < 4; i++) {
    calls.Call(kSetNodeTexture, frame, kLr[i], m, LoadU32(base, panel + 36),
               LoadU32(base, panel + 84) + i, kTextures[i]);
  }
  // HideNode: r6 the last SetNodeTexture's 1 (unread).
  calls.Call(kHideNode, frame, kLr[4], m, LoadU32(base, panel + 36), LoadU32(base, panel + 84) + 4,
             1);
  return calls.Call(kHideNode, frame, kLr[5], m, LoadU32(base, panel + 36),
                    LoadU32(base, panel + 84) + 5, 1);
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
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  StoreU32(base, panel + 8, uint32_t(tab));
  TabSetCursorY(base, panel, tab * 40, frame);
  static constexpr uint32_t kLr[3] = {0x8225022C, 0x8225022C, 0x8225022C};
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t node =
        ui_movies::FindNode(base, m, LoadU32(base, panel + 24), LoadU32(base, panel + 28));
    const uint32_t child = ui_movies::Child(base, node, LoadU32(base, panel + 44 + 4 * i));
    calls.Call(kSetNodeText, frame, kLr[i], m, child, 124 + i);
  }
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 24), LoadU32(base, panel + 32));
  const uint32_t a = ui_movies::Child(base, node, LoadU32(base, panel + 88));
  const uint32_t b = ui_movies::Child(base, node, LoadU32(base, panel + 92));
  SetTabWords(base, a, b, tab);
  const uint64_t id = LoadU32(base, panel + 140) != 0 ? 929 : tab + 930;
  return SetCaption(base, LoadU32(base, panel + 4), id, frame, calls);
}

uint64_t TabMove(uint8_t* base, uint32_t panel, uint64_t tab, uint64_t before, uint64_t moves,
                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224FF88 TabPanel::Move");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 24), LoadU32(base, panel + 32));
  const uint32_t a = ui_movies::Child(base, node, LoadU32(base, panel + 88));
  const uint32_t b = ui_movies::Child(base, node, LoadU32(base, panel + 92));
  SetTabWords(base, a, b, tab);
  bool enabled = false;
  calls.Call(kFinishNamed, frame, 0x82250024, m, LoadU32(base, panel + 120));
  PlayNamed(calls, frame, 0x82250040, m, LoadU32(base, panel + 120), 1, 0, 0, 0);
  calls.Call(kFinishNamed, frame, 0x82250050, m, LoadU32(base, panel + 96) + before);
  // The tab's word (+12, +16, +20 for tabs 0 to 2): 1 its animation played
  // and looped, else the moves' two animations played and finished.
  const int32_t t = int32_t(uint32_t(tab));
  const bool known = t == 0 || t == 1 || t == 2;
  if (known && LoadU32(base, panel + 12 + 4 * uint32_t(t)) == 1) {
    enabled = true;
    calls.Call(kFinishNamed, frame, 0x82250098, m, LoadU32(base, panel + 96) + tab);
    PlayNamed(calls, frame, 0x822500B8, m, LoadU32(base, panel + 96) + tab, 1, 0, 0, 0);
    calls.Call(kLoopNamed, frame, 0x822500C8, m, LoadU32(base, panel + 96) + tab);
  } else {
    const uint32_t c = LoadU32(base, panel + 108);
    if (int32_t(c) >= 0) {
      PlayNamed(calls, frame, 0x822500F0, m, c, 0, 0, 0, 0);
      calls.Call(kFinishNamed, frame, 0x822500FC, m, LoadU32(base, panel + 108));
    }
    const uint32_t d = LoadU32(base, panel + 112);
    if (int32_t(d) >= 0) {
      PlayNamed(calls, frame, 0x82250120, m, d, 1, 1, 0, 0);
      calls.Call(kFinishNamed, frame, 0x8225012C, m, LoadU32(base, panel + 112));
    }
  }
  calls.Call(kFinishNamed, frame, 0x82250138, m, LoadU32(base, panel + 100));
  uint64_t r3 = calls.Call(kFinishNamed, frame, 0x82250144, m, LoadU32(base, panel + 104));
  const int32_t step = int32_t(uint32_t(tab - before));
  if (step > 1 || step < -1) {
    r3 = TabSetCursorY(base, panel, tab * 40, frame);
  } else if (int32_t(uint32_t(moves)) == 1) {
    r3 = PlayNamed(calls, frame, 0x8225017C, m, LoadU32(base, panel + 104), 1, 0, 0, 0);
  } else if (int32_t(uint32_t(moves)) == -1) {
    r3 = PlayNamed(calls, frame, 0x8225017C, m, LoadU32(base, panel + 100), 1, 0, 0, 0);
  }
  if (LoadU32(base, panel + 140) != 0) {
    return r3;
  }
  return SetCaption(base, LoadU32(base, panel + 4), enabled ? tab + 930 : 584, frame, calls);
}

// --- The car rows E ---

uint64_t RowSetRow(uint8_t* base, uint32_t panel, uint64_t row, uint32_t entry, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F018 RowPanel::SetRow");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  uint64_t r3 =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 12), LoadU32(base, panel + 16));
  uint32_t node = 0;
  const uint32_t child =
      int32_t(uint32_t(row)) >= 0 ? LoadU32(base, panel + 4 * (uint32_t(row) + 5))
                                  : LoadU32(base, panel + 60);
  if (int32_t(child) >= 0) {
    r3 = ui_movies::Child(base, uint32_t(r3), child);
    node = uint32_t(r3);
  }
  const uint32_t logo = LoadU32(base, panel + 120);
  if (int32_t(logo) >= 0) {
    r3 = ui_movies::Child(base, node, logo);
    const int32_t id = int32_t(LoadU32(base, entry + 8432));
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
  const uint32_t text0 = LoadU32(base, panel + 124);
  if (int32_t(text0) >= 0) {
    const uint32_t c = ui_movies::Child(base, node, text0);
    r3 = calls.Call(kSetNodeText, frame, 0x8224F100, m, c,
                    uint32_t(guest::LoadU8(base, entry + 769)) + 1934);
  }
  const uint32_t text1 = LoadU32(base, panel + 128);
  if (int32_t(text1) >= 0) {
    const uint32_t c = ui_movies::Child(base, node, text1);
    r3 = calls.Call(kSetNodeText, frame, 0x8224F128, m, c, LoadU32(base, entry + 8432) + 1646);
  }
  const uint32_t text2 = LoadU32(base, panel + 132);
  if (int32_t(text2) >= 0) {
    const uint32_t c = ui_movies::Child(base, node, text2);
    r3 = calls.Call(kSetNodeText, frame, 0x8224F150, m, c, LoadU32(base, entry + 8432) + 1718);
  }
  return r3;
}

uint64_t RowFill(uint8_t* base, uint64_t panel_r3, uint32_t list, uint64_t scroll, uint64_t item,
                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F758 RowPanel::Fill");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 192);
  const int32_t count = int32_t(LoadU32(base, list + 512));
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 12), LoadU32(base, panel + 16));
  const uint32_t first = list + (uint32_t(scroll) << 2);
  for (int32_t i = 0; i < int32_t(LoadU32(base, panel + 4)); i++) {
    const uint32_t row = ui_movies::Child(base, node, LoadU32(base, panel + 20 + 4 * uint32_t(i)));
    if (i < count) {
      StoreU32(base, row + 80, 1);
      RowSetRow(base, panel, uint32_t(i), LoadU32(base, first + 4 * uint32_t(i)), frame, calls);
    } else {
      StoreU32(base, row + 80, 0);
    }
  }
  const uint32_t sub = LoadU32(base, panel + 12);
  const uint32_t cursor = int32_t(sub) >= 0
                              ? ui_movies::FindNode(base, m, sub, LoadU32(base, panel + 144))
                              : ui_movies::Child(base, node, LoadU32(base, panel + 144));
  StoreU32(base, cursor + 80, count > int32_t(LoadU32(base, panel + 4)) ? 1 : 0);
  if (LoadU32(base, panel + 156) == 0) {
    const uint32_t entry = LoadU32(base, list + (uint32_t(scroll + item) << 2));
    SetCaption(base, LoadU32(base, panel + 0), LoadU32(base, entry + 8432) + 1862, frame, calls);
  }
  return RowSetArrows(base, panel_r3, scroll, LoadU32(base, list + 512), frame);
}

uint64_t RowRefresh(uint8_t* base, uint64_t panel_r3, uint32_t list, uint64_t scroll, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F888 RowPanel::Refresh");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = panel_r3;
  const uint32_t first = list + (uint32_t(scroll) << 2);
  for (int32_t i = 0; i < int32_t(LoadU32(base, panel + 4)); i++) {
    if (uint32_t(i) + uint32_t(scroll) < LoadU32(base, list + 512)) {
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
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  StoreU32(base, panel + 8, uint32_t(row));
  calls.Call(kFinishNamed, frame, 0x8224F938, m, before + LoadU32(base, panel + 92));
  calls.Call(kFinishNamed, frame, 0x8224F948, m, row + LoadU32(base, panel + 92));
  PlayNamed(calls, frame, 0x8224F968, m, row + LoadU32(base, panel + 92), 1, 0, 0, 0);
  calls.Call(kLoopNamed, frame, 0x8224F978, m, row + LoadU32(base, panel + 92));
  if (uint32_t(moves) != 0) {
    const uint32_t first = list + (uint32_t(scroll) << 2);
    for (uint32_t i = 0;; i++) {
      uint32_t count = LoadU32(base, list + 512);
      const uint32_t rows = LoadU32(base, panel + 4);
      if (count >= rows) count = rows;
      if (i >= count) break;
      RowSetRow(base, panel, i, LoadU32(base, first + 4 * i), frame, calls);
    }
    calls.Call(kFinishNamed, frame, 0x8224F9D4, m, LoadU32(base, panel + 96));
    calls.Call(kFinishNamed, frame, 0x8224F9E0, m, LoadU32(base, panel + 100));
    calls.Call(kFinishNamed, frame, 0x8224F9EC, m, LoadU32(base, panel + 96));
    calls.Call(kFinishNamed, frame, 0x8224F9F8, m, LoadU32(base, panel + 100));
    uint32_t named;
    if (int32_t(uint32_t(moves)) < 0) {
      const uint32_t at = uint32_t(LoadU32(base, panel + 4) + scroll) << 2;
      RowSetRow(base, panel, 0xFFFFFFFFFFFFFFFFull, LoadU32(base, list + at), frame, calls);
      named = LoadU32(base, panel + 100);
    } else {
      RowSetRow(base, panel, 0xFFFFFFFFFFFFFFFFull, LoadU32(base, first - 4), frame, calls);
      named = LoadU32(base, panel + 96);
    }
    PlayNamed(calls, frame, 0x8224FA48, m, named, 1, 0, 0, 0);
  }
  if (LoadU32(base, panel + 156) == 0) {
    const uint32_t entry = LoadU32(base, list + (uint32_t(row + scroll) << 2));
    SetCaption(base, LoadU32(base, panel + 0), LoadU32(base, entry + 8432) + 1862, frame, calls);
  }
  return RowSetArrows(base, panel_r3, scroll, LoadU32(base, list + 512), frame);
}

uint64_t RowSetArrows(uint8_t* base, uint64_t panel_r3, uint64_t scroll, uint64_t count,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224F680 RowPanel::SetArrows");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (int32_t(LoadU32(base, panel + 16)) < 0 || int32_t(LoadU32(base, panel + 148)) < 0) {
    return panel_r3;
  }
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 12), LoadU32(base, panel + 16));
  const auto arrow = [&](uint32_t offset) {
    const uint32_t sub = LoadU32(base, panel + 12);
    const uint32_t index = LoadU32(base, panel + 148) + offset;
    return int32_t(sub) >= 0 ? ui_movies::FindNode(base, m, sub, index)
                             : ui_movies::Child(base, node, index);
  };
  const uint32_t up = arrow(0);
  StoreU32(base, up + 80, uint32_t(scroll) != 0 ? 1 : 0);
  const uint32_t down = arrow(1);
  StoreU32(base, down + 80,
           LoadU32(base, panel + 4) + uint32_t(scroll) - uint32_t(count) != 0 ? 1 : 0);
  return down;
}

uint64_t RowUnhighlight(uint8_t* base, uint64_t panel_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F558 RowPanel::Unhighlight");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (LoadU32(base, panel + 156) == 0) {
    return panel_r3;
  }
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  calls.Call(kFinishNamed, frame, 0x8224F598, m,
             LoadU32(base, panel + 8) + LoadU32(base, panel + 92));
  PlayNamed(calls, frame, 0x8224F5BC, m, LoadU32(base, panel + 92) + LoadU32(base, panel + 4), 1,
            0, 0, 0);
  return calls.Call(kFinishNamed, frame, 0x8224F5D0, m,
                    LoadU32(base, panel + 92) + LoadU32(base, panel + 4));
}

uint64_t RowHide(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224F4C8 RowPanel::Hide");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  const uint32_t row = LoadU32(base, panel + 8);
  if (int32_t(row) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224F504, m, LoadU32(base, panel + 92) + row);
    StoreU32(base, panel + 8, 0xFFFFFFFFu);
  }
  calls.Call(kFinishNamed, frame, 0x8224F518, m, LoadU32(base, panel + 104));
  calls.Call(kFinishNamed, frame, 0x8224F524, m, LoadU32(base, panel + 108));
  return PlayNamed(calls, frame, 0x8224F540, m, LoadU32(base, panel + 88), 1, 0, 1, 0);
}

// --- The colors D ---

uint64_t ColorShow(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D010 ColorPanel::Show");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  uint64_t r3 = PlayNamed(calls, frame, 0x8224D04C, m, LoadU32(base, panel + 16), 1, 0, 1, 0);
  const uint32_t up = LoadU32(base, panel + 52);
  if (int32_t(up) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224D060, m, up);
  }
  const uint32_t down = LoadU32(base, panel + 56);
  if (int32_t(down) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224D074, m, down);
  }
  return r3;
}

uint64_t ColorSelect(uint8_t* base, uint32_t panel, uint64_t item, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D118 ColorPanel::Select");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  calls.Call(kFinishNamed, frame, 0x8224D140, m, LoadU32(base, panel + 24));
  PlayNamed(calls, frame, 0x8224D160, m, LoadU32(base, panel + 24) + item, 1, 0, 0, 0);
  uint64_t r3 = calls.Call(kLoopNamed, frame, 0x8224D170, m, LoadU32(base, panel + 24) + item);
  if (int32_t(LoadU32(base, panel + 52)) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224D184, m, LoadU32(base, panel + 52));
    PlayNamed(calls, frame, 0x8224D1A0, m, LoadU32(base, panel + 52), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, 0x8224D1AC, m, LoadU32(base, panel + 52));
  }
  if (int32_t(LoadU32(base, panel + 56)) >= 0) {
    calls.Call(kFinishNamed, frame, 0x8224D1C0, m, LoadU32(base, panel + 56));
    PlayNamed(calls, frame, 0x8224D1DC, m, LoadU32(base, panel + 56), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, 0x8224D1E8, m, LoadU32(base, panel + 56));
  }
  return r3;
}

uint64_t ColorSetArrows(uint8_t* base, uint32_t panel, uint64_t up, uint64_t down, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D2D0 ColorPanel::SetArrows");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 8), LoadU32(base, panel + 60));
  // cmpw: the low words.
  StoreU32(base, node + 80, uint32_t(up) == uint32_t(down) ? uint32_t(up) : 1);
  const uint32_t a = LoadU32(base, panel + 72);
  if (int32_t(a) >= 0) {
    node = ui_movies::FindNode(base, m, LoadU32(base, panel + 8), a);
    StoreU32(base, node + 80, uint32_t(up));
  }
  const uint32_t b = LoadU32(base, panel + 76);
  if (int32_t(b) >= 0) {
    node = ui_movies::FindNode(base, m, LoadU32(base, panel + 8), b);
    StoreU32(base, node + 80, uint32_t(down));
  }
  return node;
}

uint64_t ColorShowSwatches(uint8_t* base, uint32_t panel, uint64_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D358 ColorPanel::ShowSwatches");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 8), LoadU32(base, panel + 12));
  const uint32_t swatches = ui_movies::Child(base, node, LoadU32(base, panel + 68));
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
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 8), LoadU32(base, panel + 12));
  const double y = FloatOf(base, frame, int64_t(int32_t((uint32_t(item) << 5) + 8)));
  const uint32_t child = ui_movies::Child(base, node, LoadU32(base, panel + 64));
  steps::Stfs(base, child + 16, y);
  return child;
}

uint64_t ColorMove(uint8_t* base, uint64_t panel_r3, uint64_t item, uint64_t before, uint64_t moves,
                   uint64_t scroll, uint64_t step, uint64_t placed, uint32_t list, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D580 ColorPanel::Move");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  calls.Call(kFinishNamed, frame, 0x8224D5C4, m, LoadU32(base, panel + 24) + before);
  calls.Call(kFinishNamed, frame, 0x8224D5D4, m, LoadU32(base, panel + 24) + item);
  PlayNamed(calls, frame, 0x8224D5F4, m, LoadU32(base, panel + 24) + item, 1, 0, 0, 0);
  calls.Call(kLoopNamed, frame, 0x8224D604, m, LoadU32(base, panel + 24) + item);
  calls.Call(kFinishNamed, frame, 0x8224D610, m, LoadU32(base, panel + 28));
  uint64_t r3 = calls.Call(kFinishNamed, frame, 0x8224D61C, m, LoadU32(base, panel + 32));
  if (uint32_t(placed) == 0) {
    const int32_t s = int32_t(uint32_t(step));
    if (s != 0) {
      r3 = PlayNamed(calls, frame, 0x8224D654, m, LoadU32(base, panel + (s > 0 ? 32 : 28)), 1, 0,
                     0, 0);
    }
  } else {
    r3 = ColorPlaceCursor(base, panel, item, frame);
  }
  const int32_t moved = int32_t(uint32_t(moves));
  if (moved == 0) {
    return r3;
  }
  ColorSetSwatches(base, panel_r3, list, scroll, frame);
  calls.Call(kFinishNamed, frame, 0x8224D688, m, LoadU32(base, panel + 44));
  r3 = calls.Call(kFinishNamed, frame, 0x8224D694, m, LoadU32(base, panel + 48));
  return PlayNamed(calls, frame, 0x8224D6C4, m, LoadU32(base, panel + (moved < 0 ? 40 : 36)), 1,
                   0, 0, 0);
}

uint64_t ColorSetCount(uint8_t* base, uint32_t panel, uint64_t count, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D458 ColorPanel::SetCount");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = ColorShowSwatches(base, panel, count, frame);
  if (LoadU32(base, panel + 88) != 0) {
    return r3;
  }
  return SetCaption(base, LoadU32(base, panel + 4), 552, frame, calls);
}

uint64_t ColorSetSwatch(uint8_t* base, uint32_t panel, uint64_t swatch, uint32_t color,
                        uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D4A0 ColorPanel::SetSwatch");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 8), LoadU32(base, panel + 12));
  const uint32_t swatches = ui_movies::Child(base, node, LoadU32(base, panel + 68));
  const uint32_t at = ui_movies::Child(base, node, LoadU32(base, swatches + 100) + uint32_t(swatch));
  const uint32_t child = ui_movies::Child(base, at, LoadU32(base, panel + 80));
  StoreU32(base, child + 56, LoadU32(base, color + 0));
  StoreU32(base, child + 60, LoadU32(base, color + 4));
  StoreU32(base, child + 64, LoadU32(base, color + 8));
  return child;
}

uint64_t ColorSetSwatches(uint8_t* base, uint64_t panel, uint32_t list, uint64_t scroll,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D518 ColorPanel::SetSwatches");
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = panel;
  for (uint32_t i = 0; i < LoadU32(base, list + 52); i++) {
    const uint32_t count = LoadU32(base, list + 52);
    const uint32_t at = (i + uint32_t(scroll)) % count;
    r3 = ColorSetSwatch(base, uint32_t(panel), i, LoadU32(base, list + 4 * at), frame);
  }
  return r3;
}

// --- The car's info C ---

uint64_t CarSetTexts(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D7F0 CarPanel::SetTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 16), LoadU32(base, panel + 20));
  const uint32_t a = ui_movies::Child(base, node, LoadU32(base, panel + 144));
  calls.Call(kSetNodeText, frame, 0x8224D834, m, a, LoadU32(base, panel + 276));
  const uint32_t b = ui_movies::Child(base, node, LoadU32(base, panel + 144) + 1);
  return calls.Call(kSetNodeText, frame, 0x8224D854, m, b, LoadU32(base, panel + 280));
}

uint64_t CarSetButtons(uint8_t* base, uint32_t panel, uint64_t a, uint64_t b, uint64_t c,
                       uint64_t d, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224D860 CarPanel::SetButtons");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  const uint64_t icons[4] = {a, b, c, d};
  // Each: hidden (-1) or shown with its texture; the calls' sites.
  static constexpr uint32_t kHideLr[4] = {0x8224D8A0, 0x8224D8D8, 0x8224D914, 0x8224D950};
  static constexpr uint32_t kShowLr[4] = {0x8224D8A8, 0x8224D8E0, 0x8224D91C, 0x8224D958};
  static constexpr uint32_t kTextureLr[4] = {0x8224D8BC, 0x8224D8F8, 0x8224D934, 0x8224D970};
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t node = LoadU32(base, panel + 32);
    const uint32_t child = LoadU32(base, panel + 272) + i;
    if (int32_t(uint32_t(icons[i])) == -1) {
      calls.Call(kHideNode, frame, kHideLr[i], m, node, child);
    } else {
      calls.Call(kShowNode, frame, kShowLr[i], m, node, child);
      calls.Call(kSetNodeTexture, frame, kTextureLr[i], m, LoadU32(base, panel + 32),
                 LoadU32(base, panel + 272) + i, icons[i]);
    }
  }
  calls.Call(kHideNode, frame, 0x8224D984, m, LoadU32(base, panel + 32),
             LoadU32(base, panel + 272) + 4);
  return calls.Call(kHideNode, frame, 0x8224D998, m, LoadU32(base, panel + 32),
                    LoadU32(base, panel + 272) + 5);
}

uint64_t CarPlaceCursor(uint8_t* base, uint64_t panel_r3, uint64_t action, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D9A0 CarPanel::PlaceCursor");
  const uint32_t panel = uint32_t(panel_r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (int32_t(LoadU32(base, panel + 152)) < 0) {
    return panel_r3;
  }
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  const uint32_t node =
      ui_movies::FindNode(base, m, LoadU32(base, panel + 16), LoadU32(base, panel + 20));
  const double y = FloatOf(base, frame, int64_t(int32_t(uint32_t(action) * 40)));
  const uint32_t child = ui_movies::Child(base, node, LoadU32(base, panel + 152));
  steps::Stfs(base, child + 16, y);
  return child;
}

uint64_t CarSelect(uint8_t* base, uint32_t panel, uint64_t arrows, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DCF0 CarPanel::Select");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  uint64_t r3 = m;
  const uint32_t action = LoadU32(base, panel + 8);
  if (int32_t(action) >= 0) {
    const auto named = [&] { return LoadU32(base, panel + 4 * (LoadU32(base, panel + 8) + 17)); };
    calls.Call(kFinishNamed, frame, 0x8224DD2C, m, named());
    PlayNamed(calls, frame, 0x8224DD54, m, named(), 1, 0, 0, 0);
    r3 = calls.Call(kLoopNamed, frame, 0x8224DD6C, m, named());
  }
  if (uint32_t(arrows) == 0) {
    return r3;
  }
  if (int32_t(LoadU32(base, panel + 60)) >= 0) {
    r3 = Replay(calls, frame, m, LoadU32(base, panel + 60), 0x8224DD88, 0x8224DDA4, 0x8224DDB0);
  }
  if (int32_t(LoadU32(base, panel + 64)) >= 0) {
    r3 = Replay(calls, frame, m, LoadU32(base, panel + 64), 0x8224DDC4, 0x8224DDE0, 0x8224DDEC);
  }
  return r3;
}

uint64_t CarStopArrows(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DF30 CarPanel::StopArrows");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  uint64_t r3 = m;
  if (int32_t(LoadU32(base, panel + 60)) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224DF64, m, LoadU32(base, panel + 60));
  }
  if (int32_t(LoadU32(base, panel + 64)) >= 0) {
    r3 = calls.Call(kFinishNamed, frame, 0x8224DF78, m, LoadU32(base, panel + 64));
  }
  return r3;
}

uint64_t CarSetName(uint8_t* base, uint32_t panel, uint32_t entry, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224DED8 CarPanel::SetName");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  if (LoadU32(base, panel + 316) != 0) {
    return m;
  }
  return SetCaption(base, LoadU32(base, panel + 12), LoadU32(base, entry + 8432) + 1862, frame,
                    calls);
}

uint64_t CarSetStats(uint8_t* base, uint32_t panel, uint32_t record, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224E288 CarPanel::SetStats");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  const uint32_t node = ui_movies::FindNode(base, m, 0xFFFFFFFFu, LoadU32(base, panel + 24));
  uint64_t r3 = node;
  // SetNodeText(m, child [panel + at], r5, r6) when the child index isn't
  // negative; r6 to r10 unset where the original leaves them (0).
  const auto text = [&](uint32_t at, uint32_t lr, uint64_t r5, uint64_t r6) {
    const uint32_t index = LoadU32(base, panel + at);
    if (int32_t(index) < 0) {
      return false;
    }
    const uint32_t child = ui_movies::Child(base, node, index);
    r3 = calls.Call(kSetNodeText, frame, lr, m, child, r5, r6);
    return true;
  };
  {
    const uint32_t index = LoadU32(base, panel + 160);
    if (int32_t(index) >= 0) {
      const uint32_t child = ui_movies::Child(base, node, index);
      r3 = calls.Call(kSetNodeText, frame, 0x8224E2E0, m, child, LoadU32(base, record + 0) + 421);
    }
  }
  text(164, 0x8224E308, LoadU32(base, record + 4) + 426, 0);
  text(168, 0x8224E330, 8, LoadU32(base, record + 12));
  {
    const uint32_t index = LoadU32(base, panel + 172);
    if (int32_t(index) >= 0) {
      const int64_t word = int64_t(int32_t(LoadU32(base, record + 8)));
      StoreDouble(base, frame + 80, uint64_t(word));
      const double scaled =
          steps::Single(steps::Single(double(word)) * LoadDouble(base, kStatsScaleA));
      const uint32_t child = ui_movies::Child(base, node, index);
      const int32_t truncated = steps::Fctiwz(scaled);
      StoreU32(base, frame + 80, uint32_t(truncated));
      r3 = calls.Call(kSetNodeText, frame, 0x8224E388, m, child, 8, uint32_t(truncated));
    }
  }
  text(176, 0x8224E3B0, 8, LoadU32(base, record + 16));
  {
    const uint32_t index = LoadU32(base, panel + 180);
    if (int32_t(index) >= 0) {
      const double scaled = steps::Single(steps::Lfs(base, record + 24) *
                                          LoadDouble(base, kStatsScaleB));
      const uint32_t child = ui_movies::Child(base, node, index);
      const uint64_t bits = std::bit_cast<uint64_t>(scaled);
      StoreDouble(base, frame + 40, bits);
      const uint64_t args[8] = {m, child, 436, bits, 0, 0, 0, 0};
      r3 = calls.call(calls.user, kSetNodeText, args, &scaled, frame, 0x8224E3F4);
    }
  }
  text(216, 0x8224E418, 438, 0);
  text(220, 0x8224E43C, 440, 0);
  text(224, 0x8224E460, 437, 0);
  text(228, 0x8224E484, 439, 0);
  return r3;
}

uint64_t CarSetCarName(uint8_t* base, uint32_t panel, uint64_t id, uint64_t kind, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822417E8 CarPanel::SetCarName");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  uint64_t r3 = m;
  // UiMovie::SetText(m, -1, the node, string): r7 to r10 unset (0).
  const auto text = [&](uint32_t at, uint32_t lr, uint64_t string) {
    const uint32_t node = LoadU32(base, panel + at);
    if (int32_t(node) >= 0) {
      r3 = calls.Call(kSetText, frame, lr, m, 0xFFFFFFFFFFFFFFFFull, node, string);
    }
  };
  text(132, 0x82241824, kind + 1934);
  text(136, 0x82241840, id + 1646);
  text(140, 0x8224185C, id + 1718);
  return r3;
}

uint64_t Panel7CShowArrows(uint8_t* base, uint32_t panel, uint64_t shown, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82241868 Panel7C::ShowArrows");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
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
  Movie(base, LoadU32(base, panel + 12), frame);
  // The stats record at frame + 80: +770, +709 set, +712, +648, then +652,
  // +656 as integers, +660, +664 as an integer.
  const uint32_t vtable = LoadU32(base, panel + 0);
  StoreU32(base, frame + 88, LoadU32(base, entry + 712));
  StoreU32(base, frame + 80, guest::LoadU8(base, entry + 770));
  StoreU32(base, frame + 92, LoadU32(base, entry + 648));
  StoreU32(base, frame + 84, guest::LoadU8(base, entry + 709) != 0 ? 1 : 0);
  StoreU32(base, frame + 96, uint32_t(steps::Fctiwz(steps::Lfs(base, entry + 652))));
  StoreU32(base, frame + 100, uint32_t(steps::Fctiwz(steps::Lfs(base, entry + 656))));
  steps::Stfs(base, frame + 104, steps::Lfs(base, entry + 660));
  StoreU32(base, frame + 108, uint32_t(steps::Fctiwz(steps::Lfs(base, entry + 664))));
  const uint32_t stats = LoadU32(base, vtable + 4);
  if (stats == kSetStats) {
    CarSetStats(base, panel, frame + 80, frame, calls);
  } else {
    calls.Call(stats, frame, 0x8224E9C4, panel_r3, frame + 80);
  }
  const uint32_t name = LoadU32(base, LoadU32(base, panel + 0) + 8);
  const uint64_t id = LoadU32(base, entry + 8432);
  const uint64_t kind = guest::LoadU8(base, entry + 769);
  uint64_t r3;
  if (name == kSetCarName) {
    r3 = CarSetCarName(base, panel, id, kind, frame, calls);
  } else {
    r3 = calls.Call(name, frame, 0x8224E9E0, panel_r3, id, kind);
  }
  if (LoadU32(base, panel + 4) != 0) {
    r3 = calls.Call(kSetPrice, frame, 0x8224E9F8, panel_r3, LoadU32(base, entry + 744));
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
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  return Active(base, m, LoadU32(base, panel + 116)) ? 0 : 1;
}

uint64_t RowIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224F5E8 RowPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  const uint32_t named = LoadU32(base, panel + (LoadU32(base, panel + 152) != 0 ? 80 : 76));
  return Active(base, m, named) ? 0 : 1;
}

uint64_t RowIsHidden(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224F640 RowPanel::IsHidden");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, LoadU32(base, panel + 0), frame);
  return Active(base, m, LoadU32(base, panel + 88)) ? 0 : 1;
}

uint64_t CarIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224E108 CarPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = Movie(base, LoadU32(base, panel + 12), frame);
  for (uint32_t i = 0; i < 2; i++) {
    if (Active(base, m, LoadU32(base, panel + 40 + 4 * i))) {
      return 0;
    }
  }
  return 1;
}

uint64_t ColorIsShown(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D1F0 ColorPanel::IsShown");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  return Active(base, m, LoadU32(base, panel + 16)) ? 0 : 1;
}

uint64_t ColorIsIdle(uint8_t* base, uint32_t panel, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224D288 ColorPanel::IsIdle");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = Movie(base, LoadU32(base, panel + 4), frame);
  const bool a = Active(base, m, LoadU32(base, panel + 40));
  const bool b = Active(base, m, LoadU32(base, panel + 36));
  return a || b ? 0 : 1;
}

// --- The power graph's record ---

uint64_t PowerGraphSetEntry(uint8_t* base, uint64_t entry_r3, uint64_t tab) {
  ITC_NATIVE_RUNNING("sub_82250368 PowerGraph::SetEntry");
  const uint32_t entry = uint32_t(entry_r3);
  const uint32_t record = LoadU32(base, kPowerGraph) + 8;
  StoreU32(base, record + 80, uint32_t(tab));
  StoreU32(base, record + 0, 0);
  StoreU32(base, record + 4, LoadU32(base, entry + 712));
  StoreU32(base, record + 8, LoadU32(base, entry + 716));
  StoreU32(base, record + 12, LoadU32(base, entry + 724));
  StoreU32(base, record + 16, LoadU32(base, entry + 720));
  StoreU32(base, record + 20, LoadU32(base, entry + 8432));
  StoreU32(base, record + 24, guest::LoadU8(base, entry + 769));
  StoreU32(base, record + 28, guest::LoadU8(base, entry + 770));
  // The switch on the byte +708 (its cases 0 and 4 the split piece
  // sub_822503F0); nothing above 7.
  static constexpr uint32_t kKinds[8] = {6, 2, 3, 4, 6, 0, 1, 5};
  const uint8_t kind = guest::LoadU8(base, entry + 708);
  if (kind <= 7) {
    StoreU32(base, record + 32, kKinds[kind]);
  }
  StoreU32(base, record + 36, guest::LoadU8(base, entry + 709) != 0 ? 1 : 0);
  StoreU32(base, record + 40, LoadU32(base, entry + 648));
  StoreU32(base, record + 44, uint32_t(steps::Fctiwz(steps::Lfs(base, entry + 652))));
  StoreU32(base, record + 48, uint32_t(steps::Fctiwz(steps::Lfs(base, entry + 656))));
  steps::Stfs(base, record + 52, steps::Lfs(base, entry + 660));
  StoreU32(base, record + 56, uint32_t(steps::Fctiwz(steps::Lfs(base, entry + 664))));
  return entry_r3;
}

}  // namespace car_list
