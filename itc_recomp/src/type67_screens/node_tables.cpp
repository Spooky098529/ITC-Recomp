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

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// The table's movie: its id (+4) at the frame's +80, then UiMovies::Find.
// `found` the address Find answers (the r3 it leaves).
uint32_t MovieOf(uint32_t table, uint32_t frame, uint32_t* found) {
  StoreU32(frame + 80, Table(table).movie());
  *found = ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80);
  return LoadU32(*found);
}

// UiMovie::ShowNode (sub_8211AC00, `shown` 1) and HideNode (sub_8211AC48, 0):
// the node's +80; the node.
uint32_t SetShown(uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  schema::UiMovieNode(node).set_visible(shown);
  return node;
}

// 1 << n as slw does it (0 for n's bit 5 set).
uint32_t Bit(uint32_t n) { return (n & 0x20) != 0 ? 0 : 1u << (n & 0x1F); }

// The item's node (the table's +156 + 4 × item), the index as the originals
// form it.
uint32_t ItemNode(uint32_t table, uint32_t item) {
  return LoadU32(table + ((item + 39) << 2));
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

uint64_t NodeTableDestruct(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247DC0 NodeTable::~NodeTable");
  Table(uint32_t(r3)).set_vtable(kNodeTableVtable);
  return r3;
}

uint64_t NodeTableSetPairs(uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82247CB8 NodeTable::SetPairs");
  const uint32_t t = uint32_t(r3);
  Table(t).set_item(Info6(info).item());
  Table(t).set_second(Info6(info).second());
  Table(t).set_third(Info6(info).third());
  Table(t).set_fourth(Info6(info).fourth());
  return r3;
}

uint64_t PairedNodeTableSavePairs(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247CE0 PairedNodeTable::SavePairs");
  const uint32_t t = uint32_t(r3);
  const uint32_t a = Paired(t).item(), b = Paired(t).second();
  const uint32_t c = Paired(t).third(), d = Paired(t).fourth();
  Paired(t).set_item_before(a);
  Paired(t).set_second_before(b);
  Paired(t).set_third_before(c);
  Paired(t).set_fourth_before(d);
  return r3;
}

uint64_t ColorNodeTableClear(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247DD0 ColorNodeTable::Clear");
  const uint32_t t = uint32_t(r3);
  ColorTable(t).set_item_before(0xFFFFFFFFu);
  ColorTable(t).set_item(0);
  ColorTable(t).set_second(0);
  ColorTable(t).set_second_before(0xFFFFFFFFu);
  ColorTable(t).set_third(0);
  ColorTable(t).set_third_before(0xFFFFFFFFu);
  ColorTable(t).set_fourth(0);
  ColorTable(t).set_fourth_before(0xFFFFFFFFu);
  ColorTable(t).set_color_child(0xFFFFFFFFu);
  return r3;
}

uint64_t ColorNodeTableSet(uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82247E00 ColorNodeTable::Set");
  const uint32_t t = uint32_t(r3);
  ColorTable(t).set_item(Info7(info).item());
  ColorTable(t).set_fourth(Info7(info).grid_column());
  const bool one = int32_t(Info7(info).color_count()) == 1;
  ColorTable(t).set_second(Info7(info).detail_a());
  ColorTable(t).set_third(one ? 0xFFFFFFFFu : Info7(info).detail_b());
  ColorTable(t).set_mark_a(Info7(info).bit_a());
  ColorTable(t).set_mark_b(Info7(info).bit_b());
  return r3;
}

uint64_t ColorNodeTableSavePairs(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82247E50 ColorNodeTable::SavePairs");
  const uint32_t t = uint32_t(r3);
  const uint32_t a = ColorTable(t).item(), b = ColorTable(t).fourth();
  const uint32_t c = ColorTable(t).second(), d = ColorTable(t).third();
  const uint32_t e = ColorTable(t).mark_a(), f = ColorTable(t).mark_b();
  ColorTable(t).set_item_before(a);
  ColorTable(t).set_fourth_before(b);
  ColorTable(t).set_second_before(c);
  ColorTable(t).set_third_before(d);
  ColorTable(t).set_mark_a_before(e);
  ColorTable(t).set_mark_b_before(f);
  return r3;
}

uint64_t NodeTableDeletingDestruct(uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247E88 NodeTable::DeletingDestructor");
  const uint32_t frame = Frame(sp, 96);
  Table(uint32_t(r3)).set_vtable(kNodeTableVtable);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82247EB4);
  }
  return r3;
}

// --- The nodes ---

uint64_t NodeTableShow(uint64_t r3, uint32_t item, uint32_t on, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82248300 NodeTable::Show");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, 128);  // made before the checks
  if (item > 26) return r3;
  const uint32_t node = ItemNode(t, item);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  const uint32_t shown = on != 0 ? 1 : 0;
  uint64_t result = SetShown(movie, Table(t).node_set_b(), node, shown);
  const uint32_t extra = Table(t).extra_node_base();
  if (int32_t(extra) > -1) {
    result = SetShown(movie, Table(t).node_set_a(), extra + item, shown);
  }
  return result;
}

uint64_t NodeTableShowNode(uint64_t r3, uint32_t item, uint32_t on, uint32_t mask,
                           uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82247ED0 NodeTable::ShowNode");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, 160);
  if (item > 26) return r3;
  const uint32_t at = (item + 39) << 2;
  const uint32_t node = LoadU32(t + at);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  uint64_t result;
  if (on != 0) {
    result = SetShown(movie, Table(t).node_set_b(), node, 1);
    const uint32_t shown =
        ui_movies::FindNode(movie, Table(t).node_set_b(), LoadU32(t + at));
    result = shown;
    for (uint32_t j = 0; j < 4; j++) {
      const uint32_t child = LoadU32(Table(t).children_address() + 4 * j);
      if (int32_t(child) < 0) continue;
      const uint32_t value = ((1u << j) & mask) != 0 ? 1 : 0;
      const uint32_t at_child = ui_movies::Child(shown, child);
      schema::UiMovieNode(at_child).set_visible(value);
      result = at_child;
    }
  } else {
    result = SetShown(movie, Table(t).node_set_b(), node, 0);
  }
  const uint32_t extra = Table(t).extra_node_base();
  if (int32_t(extra) > -1) {
    result = SetShown(movie, Table(t).node_set_a(), extra + item, on != 0 ? 1 : 0);
  }
  return result;
}

uint64_t NodeTableSetMarks(uint64_t r3, uint32_t item, uint32_t mask, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822480F0 NodeTable::SetMarks");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, 144);
  if (item > 26) return r3;
  const uint32_t node = ItemNode(t, item);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  const uint32_t parent = ui_movies::FindNode(movie, Table(t).node_set_b(), node);
  uint64_t result = parent;
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t child = LoadU32(Table(t).children_address() + 4 * j);
    if (int32_t(child) < 0) continue;
    const uint32_t value = ((1u << j) & mask) != 0 ? 1 : 0;
    const uint32_t at_child = ui_movies::Child(parent, child);
    schema::UiMovieNode(at_child).set_visible(value);
    result = at_child;
  }
  return result;
}

// SetColors and ClearColors: the four children's colors (+56 to +64), the
// table's +552 colors for child k (`k` nullopt: none), +568 for the others.
static uint64_t Colors(uint64_t r3, uint32_t item, std::optional<uint32_t> k,
                       uint32_t sp) {
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, k ? 144 : 128);
  if (item > 26) return r3;
  const uint32_t node = ItemNode(t, item);
  if (int32_t(node) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  const uint32_t parent = ui_movies::FindNode(movie, ColorTable(t).node_set_b(), node);
  uint64_t result = parent;
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t child = LoadU32(Table(t).children_address() + 4 * j);
    if (int32_t(child) < 0) continue;
    const uint32_t at_child = ui_movies::Child(parent, child);
    const uint32_t color = k && int32_t(j) == int32_t(*k) ? t + 552 : t + 568;
    schema::UiMovieNode(at_child).set_red(LoadU32(color));
    schema::UiMovieNode(at_child).set_green(LoadU32(color + 4));
    schema::UiMovieNode(at_child).set_blue(LoadU32(color + 8));
    result = at_child;
  }
  return result;
}

uint64_t ColorNodeTableSetColors(uint64_t r3, uint32_t item, uint32_t k,
                                 uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82248198 ColorNodeTable::SetColors");
  return Colors(r3, item, k, sp);
}

uint64_t ColorNodeTableClearColors(uint64_t r3, uint32_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82248260 ColorNodeTable::ClearColors");
  return Colors(r3, item, std::nullopt, sp);
}

uint64_t ColorNodeTableMarkSelected(uint64_t r3, uint32_t which, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822487B8 ColorNodeTable::MarkSelected");
  const uint32_t t = uint32_t(r3);
  const uint32_t item = ColorTable(t).item();
  uint32_t mask;
  if (int32_t(which) == 0) {
    mask = Bit(ColorTable(t).mark_a());
  } else if (int32_t(which) == 1) {
    mask = Bit(ColorTable(t).mark_b());
  } else {
    mask = LoadU32(t + ((item + 78) << 2));
  }
  // A tail call: SetMarks entered with this function's r1.
  return NodeTableSetMarks(r3, item, mask, sp);
}

// --- The refreshes ---

uint64_t ColorNodeTableRefresh(uint64_t r3, uint32_t finish, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822483B0 ColorNodeTable::Refresh");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  uint64_t result = found;
  const bool again = finish != 0;
  // The old item.
  const uint32_t old = ColorTable(t).item_before();
  if (int32_t(old) >= 0 && ColorTable(t).item() != old) {
    calls.call(kFinishNamed, frame, 0x8224840C, movie, LoadU32(t + ((old + 4) << 2)));
    NodeTableShow(r3, ColorTable(t).item_before(), 0, frame);
    result = ColorNodeTableClearColors(r3, ColorTable(t).item_before(), frame);
  }
  // The new item.
  const uint32_t item = ColorTable(t).item();
  if (int32_t(item) >= 0) {
    const uint32_t named = LoadU32(t + ((item + 4) << 2));
    result = NodeTableShow(r3, item, 1, frame);
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x82248464, 0x82248480, 0x8224848C};
      result = Replay(calls, frame, kLr, movie, named);
      if (again) {
        result = calls.call(kFinishNamed, frame, 0x822484A0, movie, named);
      }
    }
    const uint32_t k = ColorTable(t).color_child();
    if (int32_t(k) >= 0) {
      result = ColorNodeTableSetColors(r3, ColorTable(t).item(), k, frame);
    } else {
      result = ColorNodeTableClearColors(r3, ColorTable(t).item_before(), frame);
    }
  }
  // The color pair.
  const uint32_t previous = ColorTable(t).fourth_before(), now = ColorTable(t).fourth();
  if (now != previous && int32_t(now) >= 0 && int32_t(previous) >= 0) {
    const uint32_t first = LoadU32(t + (((previous + 40) * 3 + now) << 2));
    if (int32_t(first) >= 0) {
      static constexpr uint32_t kLr[3] = {0x8224850C, 0x82248528, 0x8224853C};
      result = Restart(calls, frame, kLr, movie, first, again);
    }
    const uint32_t p = ColorTable(t).fourth_before(), n = ColorTable(t).fourth();
    const uint32_t second = LoadU32(t + (((p + 43) * 3 + n) << 2));
    if (int32_t(second) >= 0) {
      static constexpr uint32_t kLr[3] = {0x8224856C, 0x82248588, 0x8224859C};
      result = Restart(calls, frame, kLr, movie, second, again);
    }
  }
  // The four nodes from +424.
  if (int32_t(ColorTable(t).text_node_base()) > -1) {
    for (uint32_t i = 0; i < 4; i++) {
      result = SetShown(movie, ColorTable(t).node_set_a(),
                        ColorTable(t).text_node_base() + i, 0);
    }
    const int32_t now_item = int32_t(ColorTable(t).item());
    uint32_t shown;
    if (now_item == 0 || now_item == 1 || now_item == 5 || now_item == 6) {
      shown = ColorTable(t).text_node_base() + 2;
    } else if (now_item == 2 || now_item == 3 || now_item == 4) {
      shown = ColorTable(t).text_node_base() + 3;
    } else {
      SetShown(movie, ColorTable(t).node_set_a(), ColorTable(t).text_node_base(),
               1);
      shown = ColorTable(t).text_node_base() + 1;
    }
    result = SetShown(movie, ColorTable(t).node_set_a(), shown, 1);
  }
  return result;
}

uint64_t ColorNodeTableReset(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248650 ColorNodeTable::Reset");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  for (uint32_t i = 0; i < 27; i++) {
    NodeTableShow(r3, i, 0, frame);
    NodeTableSetMarks(r3, i, LoadU32(Table(t).item_masks_address() + 4 * i),
                      frame);
    ColorNodeTableClearColors(r3, i, frame);
  }
  for (uint32_t j = 0; j < 8; j++) {
    const uint32_t node = LoadU32(Table(t).nodes_address() + 4 * j);
    if (int32_t(node) >= 0) {
      SetShown(movie, ColorTable(t).node_set_a(), node, 0);
    }
  }
  uint64_t result = ColorNodeTableRefresh(r3, 1, frame, calls);
  const uint32_t a = ColorTable(t).item(), b = ColorTable(t).fourth();
  const uint32_t c = ColorTable(t).second(), d = ColorTable(t).third();
  const uint32_t e = ColorTable(t).mark_a(), f = ColorTable(t).mark_b();
  const uint32_t texts = ColorTable(t).text_node_base();
  ColorTable(t).set_item_before(a);
  ColorTable(t).set_fourth_before(b);
  ColorTable(t).set_second_before(c);
  ColorTable(t).set_third_before(d);
  ColorTable(t).set_mark_a_before(e);
  ColorTable(t).set_mark_b_before(f);
  if (int32_t(texts) > -1) {
    calls.set_text(frame, 0x82248744, movie, ColorTable(t).node_set_a(), texts, 402);
    calls.set_text(frame, 0x8224875C, movie, ColorTable(t).node_set_a(),
                   ColorTable(t).text_node_base() + 1, 401);
    calls.set_text(frame, 0x82248774, movie, ColorTable(t).node_set_a(),
                   ColorTable(t).text_node_base() + 2, 407);
    calls.set_text(frame, 0x8224878C, movie, ColorTable(t).node_set_a(),
                   ColorTable(t).text_node_base() + 3, 408);
    for (uint32_t i = 0; i < 4; i++) {
      result = SetShown(movie, ColorTable(t).node_set_a(),
                        ColorTable(t).text_node_base() + i, 0);
    }
  }
  return result;
}

uint64_t PairedNodeTableRefresh(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248800 PairedNodeTable::Refresh");
  const uint32_t t = uint32_t(r3);
  const uint32_t frame = Frame(sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(t, frame, &found);
  uint64_t result = found;
  // The two pairs' old items.
  static constexpr uint32_t kOldLr[2][2] = {{0x82248858, 0x8224886C}, {0x82248898, 0x822488AC}};
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t now_at = t + 428 + 8 * p, old_at = now_at + 4;
    const uint32_t old = LoadU32(old_at);
    if (int32_t(old) >= 0 && LoadU32(now_at) != old) {
      calls.call(kFinishNamed, frame, kOldLr[p][0], movie, LoadU32(t + ((old + 4) << 2)));
      result = NodeTableShowNode(r3, LoadU32(old_at), 0, 0, frame);
    }
  }
  // The new item, its mask bit +452.
  const uint32_t item = Paired(t).item();
  if (int32_t(item) >= 0) {
    const uint32_t mask = Bit(Paired(t).fourth());
    const uint32_t named = LoadU32(t + ((item + 4) << 2));
    result = NodeTableShowNode(r3, item, 1, mask, frame);
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x822488F4, 0x82248910, 0x8224891C};
      result = Replay(calls, frame, kLr, movie, named);
    }
  }
  // The second pair's new item.
  const uint32_t second = Paired(t).second();
  if (second != Paired(t).item() && int32_t(second) >= 0) {
    const uint32_t named = LoadU32(t + ((second + 4) << 2));
    result = NodeTableShowNode(r3, second, 1, 0, frame);
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x82248968, 0x82248984, 0x82248990};
      result = Replay(calls, frame, kLr, movie, named);
    }
  }
  // The third pair (+124's named animations).
  const uint32_t third = Paired(t).third();
  if (int32_t(third) >= 0) {
    const uint32_t named = LoadU32(t + ((third + 31) << 2));
    if (int32_t(named) >= 0) {
      static constexpr uint32_t kLr[3] = {0x822489BC, 0x822489D8, 0x822489E4};
      result = Replay(calls, frame, kLr, movie, named);
    }
  }
  const uint32_t third_old = Paired(t).third_before();
  if (int32_t(third_old) >= 0 && Paired(t).third() != third_old) {
    const uint32_t named = LoadU32(t + ((third_old + 31) << 2));
    if (int32_t(named) >= 0) {
      result = calls.call(kFinishNamed, frame, 0x82248A18, movie, named);
    }
  }
  return result;
}

}  // namespace type67_screens
