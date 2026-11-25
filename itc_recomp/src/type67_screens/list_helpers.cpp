// The type 6 and 7 list screens' shared helpers, item loaders and handlers
// (ListScreen, NodeListScreen, ColorListScreen; docs/functions/type67-screens.md):
// see type67_screens.h. UiMovies::Find, UiMovie::FindNode, ShowNode,
// HideNode, SetNodeTexture and MenuList::Selection through their plain C++
// (GetPreviousSelection, a two-instruction leaf, inline); UiMovie::
// FinishNamed, PlayNamed, LoopNamed, ResetPose, Run, ClearText, ScrollText
// and sub_8211C5F8 guest calls to their overrides; SetText through
// Calls::set_text; the screens' slots through Slot.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "schema/race_screens.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using CLS = schema::ColorListScreen;
using LS = schema::ListScreen;
using NLS = schema::NodeListScreen;

constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0;
constexpr uint32_t kClearText = 0x8211C470, kScrollText = 0x8211C570,
                   kResetTextScroll = 0x8211C5F8;
constexpr uint32_t kNone = 0xFFFFFFFFu;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// The screen's movie: [+9680] at the frame's +80, then UiMovies::Find.
// `found` the address Find answers (the r3 it leaves).
uint32_t MovieOf(uint32_t screen, uint32_t frame, uint32_t* found) {
  StoreU32(frame + 80, LS(screen).movie());
  *found = ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80);
  return LoadU32(*found);
}

// A node's +108 set (FindNode, then the store); the node.
uint32_t SetNodeWord(uint32_t movie, uint32_t sub_scene, uint32_t child,
                     uint32_t value) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  schema::UiMovieNode(node).set_texture_entry(value);
  return node;
}

// UiMovie::ShowNode (`shown` 1) and HideNode (0): the node's +80; the node.
uint32_t SetShown(uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  schema::UiMovieNode(node).set_visible(shown);
  return node;
}

// FinishNamed, then PlayNamed (chained 1), from `frame` returning to
// `lr[0]` and `lr[1]`. PlayNamed's r3.
uint64_t Restart(const Calls& calls, uint32_t frame, uint32_t lr0, uint32_t lr1, uint32_t movie,
                 uint32_t named) {
  calls.call(kFinishNamed, frame, lr0, movie, named);
  const uint64_t args[6] = {movie, named, 1, 0, 0, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr1);
}

}  // namespace

// --- The destructors and the item loaders ---

uint64_t ListScreenDestruct(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82251EB8 ListScreen::~ListScreen");
  LS(uint32_t(r3)).set_vtable(kListScreenVtable);
  return r3;
}

uint64_t ListScreenDeletingDestruct(uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252248 ListScreen::DeletingDestructor");
  const uint32_t frame = Frame(sp, 96);
  LS(uint32_t(r3)).set_vtable(kListScreenVtable);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82252274);
  }
  return r3;
}

uint64_t NodeListScreenItemInfo(uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82256FE8 NodeListScreen::ItemInfo");
  const schema::Type6ItemInfo item_info(info);
  item_info.set_third(kNone);
  item_info.set_item(kNone);
  item_info.set_second(kNone);
  return r3;
}

uint64_t NodeListScreenLoadItem(uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257060 NodeListScreen::LoadItem");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  const uint32_t info = frame + 80;
  uint64_t result = Slot5(r3, 100, r4, info, frame, 0x82257088, calls);
  if (NLS(screen).node_table().address != 0) {
    result = NodeTableSetPairs(r3 + 10360, info);
  }
  for (uint32_t i = 0; i < 2; i++) {
    // The original's (1 << i) != 0, ANDed with the info's +12 word.
    const uint32_t bit = (i & 0x20) != 0 ? 0 : 1u << (i & 0x1F);
    StoreU32(screen + 9700 + 4 * i,
             (bit != 0 ? 1u : 0u) & schema::Type6ItemInfo(info).fourth());
  }
  return result;
}

uint64_t ColorListScreenLoadItem(uint64_t r3, uint64_t r4, uint64_t r5,
                                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255238 ColorListScreen::LoadItem");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 144);
  const uint32_t info = frame + 80;
  Slot5(r3, 100, r4, info, frame, 0x82255268, calls);
  const uint64_t result = ColorNodeTableSet(r3 + 10432, info);
  const int32_t index = int32_t(uint32_t(r5));
  uint32_t value = kNone;
  if (index >= 0 && index < int32_t(schema::Type7ItemInfo(info).color_count())) {
    value = LoadU32((uint32_t(r5) << 2) + (frame + 100));
  }
  CLS(screen).set_table_color_child(value);
  return result;
}

// --- The helpers ---

uint64_t ListScreenShowSelection(uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822518B0 ListScreen::ShowSelection");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 144);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  const uint32_t list = uint32_t(r4);
  const uint32_t selection = race_screens::ListSelection(list);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const uint32_t previous = schema::MenuList(list).selection_before();
  uint64_t result = previous;
  const uint32_t named = LS(screen).selection_anim();
  if (int32_t(named) >= 0) {
    result = Restart(calls, frame, 0x82251914, 0x82251930, movie, named);
  }
  if (int32_t(LS(screen).selection_sub_scene()) >= 0) {
    if (int32_t(previous) >= 0) {
      const uint32_t child = LS(screen).previous_child();
      if (int32_t(child) >= 0) {
        result = SetNodeWord(movie, LS(screen).selection_sub_scene(), child, previous);
      }
    }
    const uint32_t child = LS(screen).selection_child();
    if (int32_t(child) >= 0) {
      result = SetNodeWord(movie, LS(screen).selection_sub_scene(), child, selection);
    }
    for (int32_t i = 0; i < int32_t(LS(screen).children_count()); i++) {
      const uint32_t first = LS(screen).previous_children();
      if (first != 0 && int32_t(previous) >= 0) {
        const uint32_t node = LoadU32(uint32_t(4 * i) + first);
        if (int32_t(node) >= 0) {
          result = SetNodeWord(movie, LS(screen).selection_sub_scene(), node, previous);
        }
      }
      const uint32_t second = LS(screen).selection_children();
      if (second != 0) {
        const uint32_t node = LoadU32(uint32_t(4 * i) + second);
        if (int32_t(node) >= 0) {
          result =
              SetNodeWord(movie, LS(screen).selection_sub_scene(), node, selection);
        }
      }
    }
  }
  const uint32_t last = LS(screen).selection_node();
  if (int32_t(last) >= 0) {
    result = SetNodeWord(movie, kNone, last, selection);
  }
  return result;
}

uint64_t ListScreenLoopNode(uint64_t r3, uint64_t r4, uint32_t table, uint32_t sp,
                            const Calls& calls) {
  // The three functions' return addresses (FinishNamed, PlayNamed, LoopNamed).
  uint32_t lr[3];
  switch (table) {
    case 9936:
      ITC_NATIVE_RUNNING("sub_82251A20 ListScreen::LoopItemNode");
      lr[0] = 0x82251A8C, lr[1] = 0x82251AA8, lr[2] = 0x82251AB4;
      break;
    case 9976:
      ITC_NATIVE_RUNNING("sub_82251B80 ListScreen::LoopSecondNode");
      lr[0] = 0x82251BEC, lr[1] = 0x82251C08, lr[2] = 0x82251C14;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82251AD0 ListScreen::LoopThirdNode");
      lr[0] = 0x82251B3C, lr[1] = 0x82251B58, lr[2] = 0x82251B64;
      break;
  }
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  const uint32_t selection = race_screens::ListSelection(uint32_t(r4));
  if (int32_t(selection) < 0) return selection;
  const uint32_t named = LoadU32(((selection + table / 4) << 2) + screen);
  if (int32_t(named) < 0) return selection;
  Restart(calls, frame, lr[0], lr[1], movie, named);
  return calls.call(kLoopNamed, frame, lr[2], movie, named);
}

uint64_t ListScreenSetScrollText(uint64_t r3, uint64_t r4, uint64_t r5,
                                 uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251D00 ListScreen::SetScrollText");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  const uint32_t id = uint32_t(r4);
  if (id == LS(screen).scroll_text()) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  uint64_t result = found;
  const uint32_t sub_scene = LS(screen).scroll_sub_scene();
  if (int32_t(sub_scene) >= 0) {
    const uint32_t child = LS(screen).scroll_child();
    if (int32_t(child) >= 0 && int32_t(id) >= 0) {
      const uint64_t reset[6] = {movie, sub_scene, child, 0, 0, 0};
      calls.call_set(kResetTextScroll, reset, 3, frame, 0x82251D68);
      const uint32_t text_sub = LS(screen).scroll_sub_scene();
      calls.set_r4(text_sub);
      calls.set_text(frame, 0x82251D7C, movie, text_sub, LS(screen).scroll_child(), id);
      const uint64_t scroll[6] = {movie, LS(screen).scroll_sub_scene(),
                                  LS(screen).scroll_child(), uint32_t(r5), 0, 0};
      result = calls.call_set(kScrollText, scroll, 4, frame, 0x82251D90);
    }
  }
  LS(screen).set_scroll_text(id);
  return result;
}

uint64_t ListScreenClearScrollText(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251DA0 ListScreen::ClearScrollText");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  uint64_t result = found;
  const uint32_t sub_scene = LS(screen).scroll_sub_scene();
  if (int32_t(sub_scene) >= 0) {
    const uint32_t child = LS(screen).scroll_child();
    if (int32_t(child) >= 0) {
      const uint64_t args[6] = {movie, sub_scene, child, 0, 0, 0};
      result = calls.call_set(kClearText, args, 3, frame, 0x82251DEC);
    }
  }
  LS(screen).set_scroll_text_shown(0);
  LS(screen).set_scroll_text(kNone);
  return result;
}

uint64_t ListScreenSetArrows(uint64_t r3, uint32_t which, uint32_t on,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82251E10 ListScreen::SetArrows");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  const uint32_t child =
      (int32_t(which) == 0 ? LS(screen).arrow_child_a() : LS(screen).arrow_child_b());
  if (int32_t(child) < 0) return r3;
  const uint32_t sub_scene = LS(screen).text_sub_scene();
  if (int32_t(sub_scene) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  // The arrow's frame word, read for each use.
  auto frame_word = [&] {
    return int32_t(on) != 0 ? LS(screen).arrow_on_frame()
                            : LS(screen).arrow_off_frame();
  };
  if (int32_t(frame_word()) < 0) return movie;
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  schema::UiMovieNode(node).set_texture_entry(frame_word());
  return node;
}

uint64_t ListScreenOnToggleMoved(uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252068 ListScreen::OnToggleMoved");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  const uint32_t list = uint32_t(r4);
  const uint32_t selection = race_screens::ListSelection(list);
  // MenuList::GetPreviousSelection (sub_8220BC28), read and unused.
  (void)schema::MenuList(list).selection_before();
  const bool first = int32_t(selection) == 0;
  // The node finished, then the one played and looped.
  const uint32_t off = first ? 9896 : 9892, on = first ? 9892 : 9896;
  const uint32_t lr_finish_off = first ? 0x822520EC : 0x82252150;
  const uint32_t lr_finish_on = first ? 0x82252100 : 0x82252164;
  const uint32_t lr_play = first ? 0x8225211C : 0x82252180;
  ListScreenSetArrows(r3, 0, first ? 1 : 0, frame);
  uint64_t result = ListScreenSetArrows(r3, 1, first ? 0 : 1, frame);
  const uint32_t named_off = LoadU32(screen + off);
  if (int32_t(named_off) >= 0) {
    result = calls.call(kFinishNamed, frame, lr_finish_off, movie, named_off);
  }
  const uint32_t named_on = LoadU32(screen + on);
  if (int32_t(named_on) < 0) return result;
  calls.call(kFinishNamed, frame, lr_finish_on, movie, named_on);
  const uint64_t args[6] = {movie, LoadU32(screen + on), 1, 0, 0, 0};
  calls.call_set(kPlayNamed, args, 6, frame, lr_play);
  return calls.call(kLoopNamed, frame, 0x8225218C, movie, LoadU32(screen + on));
}

uint64_t ListScreenShowToggle(uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252198 ListScreen::ShowToggle");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  const uint32_t list = uint32_t(r4);
  ListScreenSetArrows(r3, race_screens::ListSelection(list), 1, frame);
  const uint32_t selection = race_screens::ListSelection(list);
  const uint32_t named = (int32_t(selection) == 0 ? LS(screen).toggle_anim_a()
                                                  : LS(screen).toggle_anim_b());
  if (int32_t(named) < 0) return selection;
  Restart(calls, frame, 0x82252214, 0x82252230, movie, named);
  return calls.call(kLoopNamed, frame, 0x8225223C, movie, named);
}

uint64_t ListScreenSetIcons(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82251C30 ListScreen::SetIcons");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  // UiMovie::SetNodeTexture (sub_8211A708): the lookup, then the texture.
  static constexpr uint32_t kTextures[4] = {0, 0, 1, 1};
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t node = ui_movies::FindNode(movie, LS(screen).icon_sub_scene(),
                                              LS(screen).icon_child() + k);
    ui_movies::SetNodeTexture(node, kTextures[k]);
  }
  SetShown(movie, LS(screen).icon_sub_scene(), LS(screen).icon_child() + 4, 0);
  return SetShown(movie, LS(screen).icon_sub_scene(), LS(screen).icon_child() + 5,
                  0);
}

uint64_t ListScreenSetUpMovie(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251EC8 ListScreen::SetUpMovie");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  // r4 as Find leaves it (the id's word).
  calls.call(kResetPose, frame, 0x82251F04, movie, frame + 80);
  const uint32_t table = LS(screen).node_table().address;
  if (table != 0) {
    // Slot 1 with r4 as ResetPose left it.
    Slot(table, 4, std::nullopt, frame, 0x82251F24, calls);
  }
  const uint32_t sub_scene = LS(screen).selection_sub_scene();
  if (int32_t(sub_scene) >= 0) {
    const uint32_t child = LS(screen).shown_child();
    if (int32_t(child) >= 0) {
      SetShown(movie, sub_scene, child, LS(screen).shown_child_on() != 0 ? 1 : 0);
    }
  }
  ListScreenSetArrows(r3, 0, 0, frame);
  ListScreenSetArrows(r3, 1, 0, frame);
  return ListScreenSetIcons(r3, frame);
}

// --- The handlers ---

uint64_t ListScreenShow(uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251F98 ListScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  ListScreenSetUpMovie(r3, frame, calls);
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  // r4 the sub-scene as the original loads it (slot 22 is called with the
  // r4 the last of these leaves).
  const uint32_t sub_scene = LS(screen).text_sub_scene();
  calls.set_r4(sub_scene);
  if (int32_t(sub_scene) >= 0) {
    const uint32_t title = LS(screen).title_child();
    if (int32_t(title) >= 0) {
      calls.set_text(frame, 0x82251FF0, movie, sub_scene, title, 322);
    }
    const uint32_t first = LS(screen).text_child_a();
    if (first != 0) {
      const uint32_t at = LS(screen).text_sub_scene();
      calls.set_r4(at);
      calls.set_text(frame, 0x8225200C, movie, at, first, 0);
    }
    const uint32_t second = LS(screen).text_child_b();
    if (second != 0) {
      const uint32_t at = LS(screen).text_sub_scene();
      calls.set_r4(at);
      calls.set_text(frame, 0x82252028, movie, at, second, 1);
    }
  }
  LS(screen).set_part_input(0);
  Slot(r3, 88, std::nullopt, frame, 0x82252044, calls);
  return Slot(r3, 56, r4, frame, 0x8225205C, calls);
}

uint64_t ListScreenDraw(uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251520 ListScreen::Draw");
  const uint32_t frame = Frame(sp, 112);
  Slot(r3, 92, r4, frame, 0x8225154C, calls);
  return Slot(r3, 40, r4, frame, 0x82251564, calls);
}

uint64_t ListScreenUpdate(uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822517B8 ListScreen::Update");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  if (int32_t(LS(screen).movie()) < 0) return r3;
  uint32_t found = 0;
  const uint32_t movie = MovieOf(screen, frame, &found);
  if (LS(screen).step() != kNone) {
    // r4 as Find leaves it (the id's word).
    const uint64_t done = Slot(r3, 96, frame + 80, frame, 0x8225181C, calls);
    if (int32_t(uint32_t(done)) != 0) {
      // The page block: the queued step or the page after.
      const uint32_t queued = LS(screen).queued_step();
      if (queued == kNone) {
        const uint32_t after = LS(screen).next_page();
        LS(screen).set_step(kNone);
        LS(screen).set_page(after);
      } else {
        LS(screen).set_unknown_9664(0);
        LS(screen).set_step(queued);
        LS(screen).set_queued_step(kNone);
      }
      LS(screen).set_step_started(0);
    }
    return done;
  }
  uint64_t result = calls.call(kRun, frame, 0x82251864, movie, 0);
  if (LS(screen).part_input() != 0) {
    const uint32_t part = LS(screen).list_part().address;
    if (part != 0) {
      result = Slot(part, 12, r4, frame, 0x82251894, calls);
    }
  }
  return result;
}

uint64_t ListScreenTail(uint64_t r3, uint64_t r4, uint32_t slot, uint32_t sp,
                        uint32_t lr, const Calls& calls) {
  switch (slot) {
    case 64:
      ITC_NATIVE_RUNNING("sub_82251580 ListScreen::Suspend");
      break;
    case 68:
      ITC_NATIVE_RUNNING("sub_82251590 ListScreen::Resume");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_822517A0 ListScreen::Open");
      LS(uint32_t(r3)).set_state(4);
      break;
  }
  return Slot(r3, slot, r4, sp, lr, calls);
}

}  // namespace type67_screens
