// The list and node screens' node tables (NodeTable, PairedNodeTable,
// ColorNodeTable; docs/functions/type67-screens.md): see type67_screens.h.
// UiMovies::Find, UiMovie::FindNode, ShowNode, HideNode and Child through
// their plain C++; UiMovie::FinishNamed, PlayNamed and LoopNamed guest calls
// to their overrides; UiMovie::SetText through Calls::set_text.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using ColorTable = schema::ColorNodeTable;
using Info6 = schema::Type6ItemInfo;
using Info7 = schema::Type7ItemInfo;
using Paired = schema::PairedNodeTable;
using Table = schema::NodeTable;

constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The table's movie: its id (+4) at the frame's +80, then UiMovies::Find.
// `found` the address Find answers (the r3 it leaves).
uint32_t MovieOf(uint8_t* base, uint32_t table, uint32_t frame, uint32_t* found) {
  StoreU32(base, frame + 80, Table(base, table).movie());
  *found = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  return LoadU32(base, *found);
}

// UiMovie::ShowNode (sub_8211AC00, `shown` 1) and HideNode (sub_8211AC48, 0):
// the node's +80; the node.
uint32_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  schema::UiMovieNode(base, node).set_visible(shown);
  return node;
}

// 1 << n as slw does it (0 for n's bit 5 set).
uint32_t Bit(uint32_t n) { return (n & 0x20) != 0 ? 0 : 1u << (n & 0x1F); }

// The item's node (the table's +156 + 4 × item), the index as the originals
// form it.
uint32_t ItemNode(uint8_t* base, uint32_t table, uint32_t item) {
  return LoadU32(base, table + ((item + 39) << 2));
}

// FinishNamed, PlayNamed (chained 1) and LoopNamed of `named` from `frame`,
// returning to `lr[0]` to `lr[2]`. LoopNamed's r3.
uint64_t Replay(const Calls& calls, uint32_t frame, const uint32_t lr[3], uint32_t movie,
                uint32_t named) {
  calls.call(kFinishNamed, frame, lr[0], movie, named);
  const uint64_t args[6] = {movie, named, 1, 0, 0, 0};
  calls.call_set(kPlayNamed, args, 6, frame, lr[1]);
  return calls.call(kLoopNamed, frame, lr[2], movie, named);
}

// FinishNamed, PlayNamed (chained 1) and, when `finish`, FinishNamed again.
// The last call's r3.
uint64_t Restart(const Calls& calls, uint32_t frame, const uint32_t lr[3], uint32_t movie,
                 uint32_t named, bool finish) {
  calls.call(kFinishNamed, frame, lr[0], movie, named);
  const uint64_t args[6] = {movie, named, 1, 0, 0, 0};
  uint64_t r3 = calls.call_set(kPlayNamed, args, 6, frame, lr[1]);
  if (finish) {
    r3 = calls.call(kFinishNamed, frame, lr[2], movie, named);
  }
  return r3;
}

}  // namespace

// --- The leaves ---

uint64_t NodeTableDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247DC0 NodeTable::~NodeTable");
  Table(base, uint32_t(r3)).set_vtable(kNodeTableVtable);
  return r3;
}

uint64_t NodeTableSetPairs(uint8_t* base, uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82247CB8 NodeTable::SetPairs");
  const uint32_t t = uint32_t(r3);
  Table(base, t).set_item(Info6(base, info).item());
  Table(base, t).set_second(Info6(base, info).second());
  Table(base, t).set_third(Info6(base, info).third());
  Table(base, t).set_fourth(Info6(base, info).fourth());
  return r3;
}

uint64_t PairedNodeTableSavePairs(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247CE0 PairedNodeTable::SavePairs");
  const uint32_t t = uint32_t(r3);
  const uint32_t a = Paired(base, t).item(), b = Paired(base, t).second();
  const uint32_t c = Paired(base, t).third(), d = Paired(base, t).fourth();
  Paired(base, t).set_item_before(a);
  Paired(base, t).set_second_before(b);
  Paired(base, t).set_third_before(c);
  Paired(base, t).set_fourth_before(d);
  return r3;
}

uint64_t ColorNodeTableClear(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247DD0 ColorNodeTable::Clear");
  const uint32_t t = uint32_t(r3);
  ColorTable(base, t).set_item_before(0xFFFFFFFFu);
  ColorTable(base, t).set_item(0);
  ColorTable(base, t).set_second(0);
  ColorTable(base, t).set_second_before(0xFFFFFFFFu);
  ColorTable(base, t).set_third(0);
  ColorTable(base, t).set_third_before(0xFFFFFFFFu);
  ColorTable(base, t).set_fourth(0);
  ColorTable(base, t).set_fourth_before(0xFFFFFFFFu);
  ColorTable(base, t).set_color_child(0xFFFFFFFFu);
  return r3;
}

uint64_t ColorNodeTableSet(uint8_t* base, uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82247E00 ColorNodeTable::Set");
  const uint32_t t = uint32_t(r3);
  ColorTable(base, t).set_item(Info7(base, info).item());
  ColorTable(base, t).set_fourth(Info7(base, info).grid_column());
  const bool one = int32_t(Info7(base, info).color_count()) == 1;
  ColorTable(base, t).set_second(Info7(base, info).detail_a());
  ColorTable(base, t).set_third(one ? 0xFFFFFFFFu : Info7(base, info).detail_b());
  ColorTable(base, t).set_mark_a(Info7(base, info).bit_a());
  ColorTable(base, t).set_mark_b(Info7(base, info).bit_b());
  return r3;
}

uint64_t ColorNodeTableSavePairs(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247E50 ColorNodeTable::SavePairs");
  const uint32_t t = uint32_t(r3);
  const uint32_t a = ColorTable(base, t).item(), b = ColorTable(base, t).fourth();
  const uint32_t c = ColorTable(base, t).second(), d = ColorTable(base, t).third();
  const uint32_t e = ColorTable(base, t).mark_a(), f = ColorTable(base, t).mark_b();
  ColorTable(base, t).set_item_before(a);
  ColorTable(base, t).set_fourth_before(b);
  ColorTable(base, t).set_second_before(c);
  ColorTable(base, t).set_third_before(d);
  ColorTable(base, t).set_mark_a_before(e);
  ColorTable(base, t).set_mark_b_before(f);
  return r3;
}

uint64_t NodeTableDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247E88 NodeTable::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 96);
  Table(base, uint32_t(r3)).set_vtable(kNodeTableVtable);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82247EB4);
  }
  return r3;
}

// --- The nodes ---

uint64_t NodeTableShow(uint8_t* base, uint64_t r3, uint32_t item, uint32_t on, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82248300 NodeTable::Show");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);  // made before the checks
  if (item > 26) return r3;
  const uint32_t node = ItemNode(base, t, item);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  const uint32_t shown = on != 0 ? 1 : 0;
  uint64_t result = SetShown(base, movie, Table(base, t).node_set_b(), node, shown);
  const uint32_t extra = Table(base, t).extra_node_base();
  if (int32_t(extra) > -1) {
    result = SetShown(base, movie, Table(base, t).node_set_a(), extra + item, shown);
  }
  return result;
}

uint64_t NodeTableShowNode(uint8_t* base, uint64_t r3, uint32_t item, uint32_t on, uint32_t mask,
                           uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82247ED0 NodeTable::ShowNode");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 160);
  if (item > 26) return r3;
  const uint32_t at = (item + 39) << 2;
  const uint32_t node = LoadU32(base, t + at);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  uint64_t result;
  if (on != 0) {
    result = SetShown(base, movie, Table(base, t).node_set_b(), node, 1);
    const uint32_t shown =
        ui_movies::FindNode(base, movie, Table(base, t).node_set_b(), LoadU32(base, t + at));
    result = shown;
    for (uint32_t j = 0; j < 4; j++) {
      const uint32_t child = LoadU32(base, Table(base, t).children_address() + 4 * j);
      if (int32_t(child) < 0) continue;
      const uint32_t value = ((1u << j) & mask) != 0 ? 1 : 0;
      const uint32_t at_child = ui_movies::Child(base, shown, child);
      schema::UiMovieNode(base, at_child).set_visible(value);
      result = at_child;
    }
  } else {
    result = SetShown(base, movie, Table(base, t).node_set_b(), node, 0);
  }
  const uint32_t extra = Table(base, t).extra_node_base();
  if (int32_t(extra) > -1) {
    result = SetShown(base, movie, Table(base, t).node_set_a(), extra + item, on != 0 ? 1 : 0);
  }
  return result;
}

uint64_t NodeTableSetMarks(uint8_t* base, uint64_t r3, uint32_t item, uint32_t mask, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822480F0 NodeTable::SetMarks");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  if (item > 26) return r3;
  const uint32_t node = ItemNode(base, t, item);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  const uint32_t parent = ui_movies::FindNode(base, movie, Table(base, t).node_set_b(), node);
  uint64_t result = parent;
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t child = LoadU32(base, Table(base, t).children_address() + 4 * j);
    if (int32_t(child) < 0) continue;
    const uint32_t value = ((1u << j) & mask) != 0 ? 1 : 0;
    const uint32_t at_child = ui_movies::Child(base, parent, child);
    schema::UiMovieNode(base, at_child).set_visible(value);
    result = at_child;
  }
  return result;
}

// SetColors and ClearColors: the four children's colors (+56 to +64), the
// table's +552 colors for child k (`k` nullopt: none), +568 for the others.
static uint64_t Colors(uint8_t* base, uint64_t r3, uint32_t item, std::optional<uint32_t> k,
                       uint32_t sp) {
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, k ? 144 : 128);
  if (item > 26) return r3;
  const uint32_t node = ItemNode(base, t, item);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  const uint32_t parent = ui_movies::FindNode(base, movie, ColorTable(base, t).node_set_b(), node);
  uint64_t result = parent;
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t child = LoadU32(base, Table(base, t).children_address() + 4 * j);
    if (int32_t(child) < 0) continue;
    const uint32_t at_child = ui_movies::Child(base, parent, child);
    const uint32_t color = k && int32_t(j) == int32_t(*k) ? t + 552 : t + 568;
    schema::UiMovieNode(base, at_child).set_red(LoadU32(base, color));
    schema::UiMovieNode(base, at_child).set_green(LoadU32(base, color + 4));
    schema::UiMovieNode(base, at_child).set_blue(LoadU32(base, color + 8));
    result = at_child;
  }
  return result;
}

uint64_t ColorNodeTableSetColors(uint8_t* base, uint64_t r3, uint32_t item, uint32_t k,
                                 uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82248198 ColorNodeTable::SetColors");
  return Colors(base, r3, item, k, sp);
}

uint64_t ColorNodeTableClearColors(uint8_t* base, uint64_t r3, uint32_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82248260 ColorNodeTable::ClearColors");
  return Colors(base, r3, item, std::nullopt, sp);
}

uint64_t ColorNodeTableMarkSelected(uint8_t* base, uint64_t r3, uint32_t which, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822487B8 ColorNodeTable::MarkSelected");
  const uint32_t t = uint32_t(r3);
  const uint32_t item = ColorTable(base, t).item();
  uint32_t mask;
  if (int32_t(which) == 0) {
    mask = Bit(ColorTable(base, t).mark_a());
  } else if (int32_t(which) == 1) {
    mask = Bit(ColorTable(base, t).mark_b());
  } else {
    mask = LoadU32(base, t + ((item + 78) << 2));
  }
  // A tail call: SetMarks entered with this function's r1.
  return NodeTableSetMarks(base, r3, item, mask, sp);
}

// --- The refreshes ---

uint64_t ColorNodeTableRefresh(uint8_t* base, uint64_t r3, uint32_t finish, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822483B0 ColorNodeTable::Refresh");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  uint64_t result = found;
  const bool again = finish != 0;
  // The old item.
  const uint32_t old = ColorTable(base, t).item_before();
  if (int32_t(old) >= 0 && ColorTable(base, t).item() != old) {
    calls.call(kFinishNamed, frame, 0x8224840C, movie, LoadU32(base, t + ((old + 4) << 2)));
    NodeTableShow(base, r3, ColorTable(base, t).item_before(), 0, frame);
    result = ColorNodeTableClearColors(base, r3, ColorTable(base, t).item_before(), frame);
  }
  // The new item.
  const uint32_t item = ColorTable(base, t).item();
  if (int32_t(item) >= 0) {
    const uint32_t named = LoadU32(base, t + ((item + 4) << 2));
    result = NodeTableShow(base, r3, item, 1, frame);
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x82248464, 0x82248480, 0x8224848C};
      result = Replay(calls, frame, kLr, movie, named);
      if (again) {
        result = calls.call(kFinishNamed, frame, 0x822484A0, movie, named);
      }
    }
    const uint32_t k = ColorTable(base, t).color_child();
    if (int32_t(k) >= 0) {
      result = ColorNodeTableSetColors(base, r3, ColorTable(base, t).item(), k, frame);
    } else {
      result = ColorNodeTableClearColors(base, r3, ColorTable(base, t).item_before(), frame);
    }
  }
  // The color pair.
  const uint32_t previous = ColorTable(base, t).fourth_before(), now = ColorTable(base, t).fourth();
  if (now != previous && int32_t(now) >= 0 && int32_t(previous) >= 0) {
    const uint32_t first = LoadU32(base, t + (((previous + 40) * 3 + now) << 2));
    if (int32_t(first) >= 0) {
      static constexpr uint32_t kLr[3] = {0x8224850C, 0x82248528, 0x8224853C};
      result = Restart(calls, frame, kLr, movie, first, again);
    }
    const uint32_t p = ColorTable(base, t).fourth_before(), n = ColorTable(base, t).fourth();
    const uint32_t second = LoadU32(base, t + (((p + 43) * 3 + n) << 2));
    if (int32_t(second) >= 0) {
      static constexpr uint32_t kLr[3] = {0x8224856C, 0x82248588, 0x8224859C};
      result = Restart(calls, frame, kLr, movie, second, again);
    }
  }
  // The four nodes from +424.
  if (int32_t(ColorTable(base, t).text_node_base()) > -1) {
    for (uint32_t i = 0; i < 4; i++) {
      result = SetShown(base, movie, ColorTable(base, t).node_set_a(),
                        ColorTable(base, t).text_node_base() + i, 0);
    }
    const int32_t now_item = int32_t(ColorTable(base, t).item());
    uint32_t shown;
    if (now_item == 0 || now_item == 1 || now_item == 5 || now_item == 6) {
      shown = ColorTable(base, t).text_node_base() + 2;
    } else if (now_item == 2 || now_item == 3 || now_item == 4) {
      shown = ColorTable(base, t).text_node_base() + 3;
    } else {
      SetShown(base, movie, ColorTable(base, t).node_set_a(), ColorTable(base, t).text_node_base(),
               1);
      shown = ColorTable(base, t).text_node_base() + 1;
    }
    result = SetShown(base, movie, ColorTable(base, t).node_set_a(), shown, 1);
  }
  return result;
}

uint64_t ColorNodeTableReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248650 ColorNodeTable::Reset");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  for (uint32_t i = 0; i < 27; i++) {
    NodeTableShow(base, r3, i, 0, frame);
    NodeTableSetMarks(base, r3, i, LoadU32(base, Table(base, t).item_masks_address() + 4 * i),
                      frame);
    ColorNodeTableClearColors(base, r3, i, frame);
  }
  for (uint32_t j = 0; j < 8; j++) {
    const uint32_t node = LoadU32(base, Table(base, t).nodes_address() + 4 * j);
    if (int32_t(node) >= 0) {
      SetShown(base, movie, ColorTable(base, t).node_set_a(), node, 0);
    }
  }
  uint64_t result = ColorNodeTableRefresh(base, r3, 1, frame, calls);
  const uint32_t a = ColorTable(base, t).item(), b = ColorTable(base, t).fourth();
  const uint32_t c = ColorTable(base, t).second(), d = ColorTable(base, t).third();
  const uint32_t e = ColorTable(base, t).mark_a(), f = ColorTable(base, t).mark_b();
  const uint32_t texts = ColorTable(base, t).text_node_base();
  ColorTable(base, t).set_item_before(a);
  ColorTable(base, t).set_fourth_before(b);
  ColorTable(base, t).set_second_before(c);
  ColorTable(base, t).set_third_before(d);
  ColorTable(base, t).set_mark_a_before(e);
  ColorTable(base, t).set_mark_b_before(f);
  if (int32_t(texts) > -1) {
    calls.set_text(frame, 0x82248744, movie, ColorTable(base, t).node_set_a(), texts, 402);
    calls.set_text(frame, 0x8224875C, movie, ColorTable(base, t).node_set_a(),
                   ColorTable(base, t).text_node_base() + 1, 401);
    calls.set_text(frame, 0x82248774, movie, ColorTable(base, t).node_set_a(),
                   ColorTable(base, t).text_node_base() + 2, 407);
    calls.set_text(frame, 0x8224878C, movie, ColorTable(base, t).node_set_a(),
                   ColorTable(base, t).text_node_base() + 3, 408);
    for (uint32_t i = 0; i < 4; i++) {
      result = SetShown(base, movie, ColorTable(base, t).node_set_a(),
                        ColorTable(base, t).text_node_base() + i, 0);
    }
  }
  return result;
}

uint64_t PairedNodeTableRefresh(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248800 PairedNodeTable::Refresh");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(base, t, frame, &found);
  uint64_t result = found;
  // The two pairs' old items.
  static constexpr uint32_t kOldLr[2][2] = {{0x82248858, 0x8224886C}, {0x82248898, 0x822488AC}};
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t now_at = t + 428 + 8 * p, old_at = now_at + 4;
    const uint32_t old = LoadU32(base, old_at);
    if (int32_t(old) >= 0 && LoadU32(base, now_at) != old) {
      calls.call(kFinishNamed, frame, kOldLr[p][0], movie, LoadU32(base, t + ((old + 4) << 2)));
      result = NodeTableShowNode(base, r3, LoadU32(base, old_at), 0, 0, frame);
    }
  }
  // The new item, its mask bit +452.
  const uint32_t item = Paired(base, t).item();
  if (int32_t(item) >= 0) {
    const uint32_t mask = Bit(Paired(base, t).fourth());
    const uint32_t named = LoadU32(base, t + ((item + 4) << 2));
    result = NodeTableShowNode(base, r3, item, 1, mask, frame);
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x822488F4, 0x82248910, 0x8224891C};
      result = Replay(calls, frame, kLr, movie, named);
    }
  }
  // The second pair's new item.
  const uint32_t second = Paired(base, t).second();
  if (second != Paired(base, t).item() && int32_t(second) >= 0) {
    const uint32_t named = LoadU32(base, t + ((second + 4) << 2));
    result = NodeTableShowNode(base, r3, second, 1, 0, frame);
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x82248968, 0x82248984, 0x82248990};
      result = Replay(calls, frame, kLr, movie, named);
    }
  }
  // The third pair (+124's named animations).
  const uint32_t third = Paired(base, t).third();
  if (int32_t(third) >= 0) {
    const uint32_t named = LoadU32(base, t + ((third + 31) << 2));
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x822489BC, 0x822489D8, 0x822489E4};
      result = Replay(calls, frame, kLr, movie, named);
    }
  }
  const uint32_t third_old = Paired(base, t).third_before();
  if (int32_t(third_old) >= 0 && Paired(base, t).third() != third_old) {
    const uint32_t named = LoadU32(base, t + ((third_old + 31) << 2));
    if (int32_t(named) >= 0) {
      result = calls.call(kFinishNamed, frame, 0x82248A18, movie, named);
    }
  }
  return result;
}

}  // namespace type67_screens
