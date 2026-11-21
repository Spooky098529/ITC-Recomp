// The color list screen (ColorListScreen, the type 7 list screen's base;
// docs/functions/type67-screens.md): its page block's reset, the page and
// step tables its Draw and Update build on first use, its pages and steps,
// the cursors and the list part's nodes, and the destructors. See
// type67_screens.h. UiMovies::Find, UiMovie::FindNode, Child, ShowNode and
// HideNode, MenuList::Selection and SetSelection, ScreenFades::FadeIn and
// FadeOut and MenuSound::Play through their plain C++
// (MenuList::GetPreviousSelection, a one-load leaf, inline);
// UiMovie::FinishNamed, PlayNamed, LoopNamed, UnloopNamed, Run and FinishAll,
// MenuList::Update, Accepted and Cancelled guest calls to their overrides;
// the list helpers, the node tables and ColorListScreen::LoadItem by their
// plain C++; the screen's and the list part's slots (and the node table's
// slot 1) through Slot and Slot5, the tables' entries through SlotNative
// where they hold one of ours.

#include "type67_screens/list_pages.h"
#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "schema/race_screens.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace type67_screens {

namespace {

using guest::LoadU32;
using G = schema::Type67ScreensGlobals;
using guest::StoreU32;
using CLS = schema::ColorListScreen;
using Part = schema::ListPart;
using namespace pages;

constexpr uint32_t kFinishNamed = 0x8211DF20, kLoopNamed = 0x8211DDE0,
                   kUnloopNamed = 0x8211DE30;
constexpr uint32_t kNone = 0xFFFFFFFFu;

// The screen's word [screen + 4 × (index + i)] (a node table's entry, the
// index formed as the originals do).
uint32_t Entry(uint8_t* base, uint32_t screen, uint32_t index, uint32_t i) {
  return LoadU32(base, ((i + index) << 2) + screen);
}

// UiMovie::FinishNamed(movie, named) as a guest call.
void Finish(const Calls& calls, uint32_t movie, uint32_t named, uint32_t frame, uint32_t lr) {
  calls.call(kFinishNamed, frame, lr, movie, named);
}

// FinishNamed of `named` when it's >= 0.
void FinishIf(const Calls& calls, uint32_t movie, uint32_t named, uint32_t frame, uint32_t lr) {
  if (int32_t(named) >= 0) Finish(calls, movie, named, frame, lr);
}

// PlayNamed (chained 1) and LoopNamed of the screen's word at `field` when
// it's >= 0, preceded by FinishNamed when `lr_finish` isn't 0 (each call
// reading the word again).
void Replay(uint8_t* base, const Calls& calls, uint32_t screen, uint32_t movie, uint32_t field,
            uint32_t frame, uint32_t lr_finish, uint32_t lr_play, uint32_t lr_loop) {
  const uint32_t named = LoadU32(base, screen + field);
  if (int32_t(named) < 0) return;
  if (lr_finish != 0) Finish(calls, movie, named, frame, lr_finish);
  Play(calls, movie, LoadU32(base, screen + field), 1, frame, lr_play);
  if (lr_loop != 0) calls.call(kLoopNamed, frame, lr_loop, movie, LoadU32(base, screen + field));
}

// UiMovie::ShowNode or HideNode(movie, sub_scene, child): the node's +80.
void SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
              uint32_t shown) {
  schema::UiMovieNode(base, ui_movies::FindNode(base, movie, sub_scene, child))
      .set_visible(shown);
}

// UiMovie::Run(movie, 0) as a guest call: 1 when it answers 0, else 0.
uint64_t RunDone(const Calls& calls, uint32_t movie, uint32_t frame, uint32_t lr) {
  return uint32_t(calls.call(kRun, frame, lr, movie, 0)) == 0 ? 1 : 0;
}

// Steps 8 and 10 (a list moved): the return addresses of their calls.
struct MovedLrs {
  uint32_t finish_old, finish_new, play_new, loop_new, finish_other, finish_arrow, play_arrow,
      finish_last, run;
};

// Steps 8 and 10: on the first call (the node table's Refresh(0) and
// MarkSelected(s) first for step 8), when the list's selection s moved from
// p: [index + p] finished, [index + s] replayed, the arrow (`forward` when
// s - p > 0, else `back`) played |s - p| times and the other one finished;
// +9668 = 1. Then UiMovie::Run(m, 0). 1.
uint64_t ListMoved(uint8_t* base, uint64_t r3, uint32_t list_at, uint32_t index,
                   uint32_t forward, uint32_t back, bool table, uint32_t bytes,
                   const MovedLrs& lr, uint32_t sp, const Calls& calls) {
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, bytes);
  const uint32_t movie = MovieOf(base, screen, frame);
  const uint32_t list = screen + list_at;
  const uint32_t s = race_screens::ListSelection(base, list);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const uint32_t p = schema::MenuList(base, list).selection_before();
  const int32_t d = int32_t(s - p);
  if (CLS(base, screen).step_started() == 0) {
    if (table) {
      ColorNodeTableRefresh(base, r3 + 10432, 0, frame, calls);
      ColorNodeTableMarkSelected(base, r3 + 10432, s, frame);
    }
    if (d != 0) {
      if (int32_t(p) >= 0)
        FinishIf(calls, movie, Entry(base, screen, index, p), frame, lr.finish_old);
      if (int32_t(s) >= 0) {
        const uint32_t named = Entry(base, screen, index, s);
        if (int32_t(named) >= 0) {
          Finish(calls, movie, named, frame, lr.finish_new);
          Play(calls, movie, named, 1, frame, lr.play_new);
          calls.call(kLoopNamed, frame, lr.loop_new, movie, named);
        }
      }
      const uint32_t arrow = LoadU32(base, screen + (d > 0 ? forward : back));
      const uint32_t other = LoadU32(base, screen + (d > 0 ? back : forward));
      FinishIf(calls, movie, other, frame, lr.finish_other);
      if (int32_t(arrow) >= 0) {
        const uint32_t sign = uint32_t(d >> 31);
        const uint32_t times = (uint32_t(d) ^ sign) - sign;
        for (uint32_t i = times; i != 0; i--) {
          Finish(calls, movie, arrow, frame, lr.finish_arrow);
          Play(calls, movie, arrow, 0, frame, lr.play_arrow);
        }
        if (times > 1) Finish(calls, movie, arrow, frame, lr.finish_last);
      }
    }
    CLS(base, screen).set_step_started(1);
  }
  calls.call(kRun, frame, lr.run, movie, 0);
  return 1;
}

// The info slot 25 fills and the details slot 26 shows (the color list's
// pages 2 to 4): slot 25 with r4 = `item` into the frame's +80,
// ColorNodeTable::Set on +10432 with it.
uint64_t LoadInfo(uint8_t* base, uint64_t r3, uint64_t item, uint32_t frame, uint32_t lr,
                  const Calls& calls) {
  Slot5(base, r3, 100, item, frame + 80, frame, lr, calls);
  return ColorNodeTableSet(base, r3 + 10432, frame + 80);
}

// The toggle list's (+10288) update and move (ListScreen::OnToggleMoved), as
// pages 2 to 4 end. MenuList::Update's r3, else OnToggleMoved's.
uint64_t UpdateToggle(uint8_t* base, uint64_t r3, uint32_t frame, uint32_t lr_update,
                      const Calls& calls) {
  const uint64_t toggle = r3 + 10288;
  const uint64_t result = Call1(calls, race_screens::kListUpdate, toggle, frame, lr_update);
  if (CLS(base, uint32_t(r3)).toggle_move() == 0) return result;
  PlaySound(base, 4, frame, calls);
  return ListScreenOnToggleMoved(base, r3, toggle, frame, calls);
}

// Whether a list moved: its move word (+24) or its scroll (+4 against +8).
bool Moved(uint8_t* base, uint32_t list) {
  const schema::MenuList menu(base, list);
  return menu.move() != 0 || menu.scroll() - menu.scroll_before() != 0;
}

// The cursors' child +16 = `r4` × 40 as a float (the product left in the
// frame's +80 as a 64-bit integer, the conversion's spill).
uint64_t PlaceCursor(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t node_at,
                     uint32_t child_at, uint32_t sp) {
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t node_id = LoadU32(base, screen + node_at);
  if (int32_t(node_id) < 0 || int32_t(LoadU32(base, screen + child_at)) < 0) return r3;
  const uint32_t movie = MovieOf(base, screen, frame);
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, node_id);
  const int64_t product = int64_t(int32_t(uint32_t(r4) * 40u));
  StoreU32(base, frame + 80, uint32_t(uint64_t(product) >> 32));
  StoreU32(base, frame + 84, uint32_t(uint64_t(product)));
  const uint32_t child = ui_movies::Child(base, node, LoadU32(base, screen + child_at));
  schema::UiMovieNode(base, child).set_y(float(double(product)));
  return child;
}

}  // namespace

// --- The page block's reset and the tables ---

uint64_t ColorListScreenReset(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82255200 ColorListScreen::Reset");
  const uint32_t screen = uint32_t(r3);
  const uint32_t page = CLS(base, screen).page();
  if (page != 2) CLS(base, screen).set_previous_page(page);
  CLS(base, screen).set_queued_step(kNone);
  CLS(base, screen).set_step(1);
  CLS(base, screen).set_next_page(2);
  CLS(base, screen).set_page(kNone);
  // A tail call.
  return ColorNodeTableClear(base, r3 + 10432);
}

uint64_t ColorListScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256CA0 ColorListScreen::Draw");
  const uint32_t frame = Frame(base, sp, 240);
  static constexpr uint32_t kEntries[11] = {kEmpty,     kEmpty,     0x822552B8, 0x82255490,
                                            0x82255658, 0x82256508, 0x822565A0, 0x822557F0,
                                            kEmpty,     0x822562E0, 0x82255818};
  BuildTable(base, frame, kColorPageGuard, kColorPages, kEntries, 11);
  const uint32_t page = CLS(base, uint32_t(r3)).page();
  if (page == kNone) return r3;
  const uint32_t entry = kColorPages + (page << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return CallEntry(base, target, self, r4, std::nullopt, frame, 0x82256E00, calls);
}

uint64_t ColorListScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256E08 ColorListScreen::Update");
  const uint32_t frame = Frame(base, sp, 240);
  const uint32_t step = CLS(base, uint32_t(r3)).step();
  static constexpr uint32_t kEntries[11] = {0x82258578, 0x82255890, 0x82256740, 0x82255B58,
                                            0x82256908, 0x82255C88, 0x82255CD0, 0x82256AC0,
                                            0x82256030, 0x82255C88, 0x82255EB0};
  // Building the table leaves its sixth entry in r4 (r5 to r9 the next
  // ones, which no step reads); else the caller's r4 is passed on.
  const bool built = BuildTable(base, frame, kColorStepGuard, kColorSteps, kEntries, 11);
  const uint32_t entry = kColorSteps + (step << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return CallEntry(base, target, self, built ? uint64_t(kEntries[5]) : r4, std::nullopt, frame,
                   0x82256F60, calls);
}

// --- The pages ---

uint64_t ColorListScreenPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822552B8 ColorListScreen::Page2");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint64_t first = r3 + 10360;
  Call1(calls, race_screens::kListUpdate, first, frame, 0x822552D4);
  if (CLS(base, screen).kind_list_move() != 0) {
    // The first list moved: step 10, then page 2.
    PlaySound(base, 4, frame, calls);
    ToStepThenPage(base, screen, 2, 10);
    ListScreenShowSelection(base, r3, first, frame, calls);
    const uint32_t selection = race_screens::ListSelection(base, uint32_t(first));
    return ListScreenSetScrollText(base, r3, LoadU32(base, screen + ((selection + 2754) << 2)), 1,
                                   frame, calls);
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListAccepted, first, frame, 0x82255350))) !=
      0) {
    // To the item list: its scroll and selection 0, the first item's info.
    PlaySound(base, 0, frame, calls);
    const uint32_t items = screen + 10144;
    CLS(base, screen).set_course_list_scroll(0);
    race_screens::ListSetSelection(base, items, 0);
    calls.set_r4(0);
    LoadInfo(base, r3, 0, frame, 0x82255394, calls);
    const uint32_t scroll = CLS(base, screen).course_list_scroll();
    CLS(base, screen).set_table_color_child(kNone);
    const uint32_t selection = race_screens::ListSelection(base, items);
    // Slot 26 (SetDetails) with r4 = the item.
    const uint64_t result =
        Slot(base, r3, 104, uint64_t(selection) + uint64_t(scroll), frame, 0x822553D0, calls);
    ToStepThenPage(base, screen, 3, 2);
    return result;
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListCancelled, first, frame, 0x82255404))) !=
      0) {
    PlaySound(base, 2, frame, calls);
    ToPage(base, screen, 9);
    return FadeOut(base, calls);
  }
  return UpdateToggle(base, r3, frame, 0x82255464, calls);
}

uint64_t ColorListScreenPage3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255490 ColorListScreen::Page3");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 160);
  const uint32_t colors = screen + 10216, items = screen + 10144;
  const uint32_t color = race_screens::ListSelection(base, colors);
  Call1(calls, race_screens::kListUpdate, items, frame, 0x822554BC);
  const uint32_t scroll = CLS(base, screen).course_list_scroll();
  const uint64_t item = uint64_t(scroll) + uint64_t(race_screens::ListSelection(base, items));
  if (Moved(base, items)) {
    // The item list moved: the item's info and details, step 7, page 3.
    PlaySound(base, 4, frame, calls);
    ColorNodeTableSavePairs(base, r3 + 10432);
    LoadInfo(base, r3, item, frame, 0x822555F8, calls);
    CLS(base, screen).set_table_color_child(kNone);
    const uint64_t result = Slot(base, r3, 104, item, frame, 0x82255624, calls);
    ToStepThenPage(base, screen, 3, 7);
    return result;
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListAccepted, items, frame, 0x822554F0))) !=
      0) {
    // To the color list: its scroll and selection 0, step 4, page 4.
    PlaySound(base, 0, frame, calls);
    CLS(base, screen).set_color_list_scroll(0);
    race_screens::ListSetSelection(base, colors, 0);
    calls.set_r4(0);
    ToStepThenPage(base, screen, 4, 4);
    ColorNodeTableSavePairs(base, r3 + 10432);
    return ColorListScreenLoadItem(base, r3, item, color, frame, calls);
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListCancelled, items, frame, 0x82255560))) !=
      0) {
    PlaySound(base, 2, frame, calls);
    const uint64_t result = ColorNodeTableSavePairs(base, r3 + 10432);
    ToStepThenPage(base, screen, 2, 3);
    return result;
  }
  return UpdateToggle(base, r3, frame, 0x822555A4, calls);
}

uint64_t ColorListScreenPage4(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255658 ColorListScreen::Page4");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint32_t colors = screen + 10216, items = screen + 10144;
  Call1(calls, race_screens::kListUpdate, colors, frame, 0x82255674);
  if (Moved(base, colors)) {
    // The color list moved: the item loaded with the color, step 8, page 4.
    PlaySound(base, 4, frame, calls);
    const uint32_t color = race_screens::ListSelection(base, colors);
    const uint32_t scroll = CLS(base, screen).course_list_scroll();
    const uint64_t item = uint64_t(scroll) + uint64_t(race_screens::ListSelection(base, items));
    const uint64_t result = ColorListScreenLoadItem(base, r3, item, color, frame, calls);
    ToStepThenPage(base, screen, 4, 8);
    return result;
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListAccepted, items, frame, 0x8225569C))) !=
      0) {
    const uint64_t result = PlaySound(base, 0, frame, calls);
    ToPage(base, screen, 5);
    return result;
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListCancelled, items, frame, 0x822556D8))) !=
      0) {
    // Back to the item list: step 6, page 3, the item's info again.
    PlaySound(base, 2, frame, calls);
    ToStepThenPage(base, screen, 3, 6);
    const uint32_t scroll = CLS(base, screen).course_list_scroll();
    const uint64_t item = uint64_t(race_screens::ListSelection(base, items)) + uint64_t(scroll);
    const uint64_t result = LoadInfo(base, r3, item, frame, 0x82255740, calls);
    CLS(base, screen).set_table_color_child(kNone);
    return result;
  }
  return UpdateToggle(base, r3, frame, 0x82255760, calls);
}

uint64_t ColorListScreenPage5(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256508 ColorListScreen::Page5");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t part = CLS(base, screen).list_part().address;
  if (part != 0) {
    // Slot 1 (ListPartBase::Open) with r4 as the page was given it.
    Slot(base, part, 4, std::nullopt, frame, 0x82256538, calls);
    CLS(base, screen).set_part_input(1);
  }
  uint64_t result =
      ListScreenSetScrollText(base, r3, CLS(base, screen).text_part(), 1, frame, calls);
  ToPage(base, screen, 6);
  if (CLS(base, screen).part_nodes() != 0) {
    result = ColorListScreenShowPartNodes(base, r3, 1, frame, calls);
  }
  return result;
}

uint64_t ColorListScreenPage6(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822565A0 ColorListScreen::Page6");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t part = CLS(base, screen).list_part().address;
  if (part == 0) {
    ToPage(base, screen, 10);
    return 0;
  }
  // Slot 2 (ListPartBase::Pick) with the page's r4.
  const uint64_t picked = Slot(base, part, 8, r4, frame, 0x822565D4, calls);
  if (int32_t(uint32_t(picked)) == 0) return picked;
  const uint32_t now = CLS(base, screen).list_part().address;
  if (Part(base, now).yes() != 0) {
    // Yes: page 7 with the part's nodes, else the leaving (page 9 after
    // step 0) with the screen fades' fade-out; the race set.
    if (CLS(base, screen).part_nodes() != 0) {
      ToPage(base, screen, 7);
    } else {
      ToStepThenPage(base, screen, 9, 0);
      FadeOut(base, calls);
    }
    CLS(base, screen).set_part_input(0);
    CLS(base, screen).set_race_set(1);
    // Slot 4 (ListPartBase::Close), r4 as the calls before left it.
    return Slot(base, CLS(base, screen).list_part().address, 16, std::nullopt, frame, 0x82256684,
                calls);
  }
  if (Part(base, now).back() == 0) return picked;
  // Back: page 4.
  ToPage(base, screen, 4);
  CLS(base, screen).set_part_input(0);
  Slot(base, CLS(base, screen).list_part().address, 16, std::nullopt, frame, 0x822566D4, calls);
  uint64_t result =
      ListScreenSetScrollText(base, r3, CLS(base, screen).text_colors(), 1, frame, calls);
  if (CLS(base, screen).part_nodes() != 0) {
    result = ColorListScreenShowPartNodes(base, r3, 0, frame, calls);
  }
  return result;
}

uint64_t ColorListScreenPage7(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822557F0 ColorListScreen::Page7");
  ToPage(base, uint32_t(r3), 10);
  return r3;
}

uint64_t ColorListScreenPage9(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822562E0 ColorListScreen::Page9");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  if ((G::fade_out_count(base) | G::fade_in_count(base)) != 0) return r3;
  ToPage(base, screen, 10);
  const uint32_t movie = MovieOf(base, screen, frame);
  return Call1(calls, kFinishAll, movie, frame, 0x8225634C);
}

// --- The cursors and the list part's nodes ---

uint64_t ColorListScreenPlaceItemCursor(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822561D0 ColorListScreen::PlaceItemCursor");
  return PlaceCursor(base, r3, r4, 9744, 9828, sp);
}

uint64_t ColorListScreenPlaceColorCursor(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82256258 ColorListScreen::PlaceColorCursor");
  return PlaceCursor(base, r3, r4, 9736, 9808, sp);
}

uint64_t ColorListScreenShowPartNodes(uint8_t* base, uint64_t r3, uint64_t hide, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256360 ColorListScreen::ShowPartNodes");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  const bool hiding = int32_t(uint32_t(hide)) != 0;
  // The toggle's node (+9892 for its selection 0, else +9896), read again
  // for each call as the original does.
  const uint32_t toggle = screen + 10288;
  auto toggle_node = [&] {
    return (race_screens::ListSelection(base, toggle) == 0 ? CLS(base, screen).toggle_anim_a()
                                                           : CLS(base, screen).toggle_anim_b());
  };
  uint64_t result = race_screens::ListSelection(base, toggle);
  if (int32_t((uint32_t(result) == 0 ? CLS(base, screen).toggle_anim_a()
                                     : CLS(base, screen).toggle_anim_b())) >= 0) {
    if (hiding) {
      result = calls.call(kFinishNamed, frame, 0x822563E8, movie, toggle_node());
    } else {
      Play(calls, movie, toggle_node(), 1, frame, 0x8225641C);
      result = calls.call(kLoopNamed, frame, 0x82256440, movie, toggle_node());
    }
  }
  // The color's node, [+10016 + 4 × the color list's selection].
  const uint32_t colors = screen + 10216;
  auto color_node = [&] {
    return LoadU32(base, screen + ((race_screens::ListSelection(base, colors) + 2504) << 2));
  };
  result = race_screens::ListSelection(base, colors);
  if (int32_t(LoadU32(base, screen + ((uint32_t(result) + 2504) << 2))) >= 0) {
    if (hiding) {
      result = calls.call(kFinishNamed, frame, 0x82256488, movie, color_node());
    } else {
      Play(calls, movie, color_node(), 1, frame, 0x822564B8);
      result = calls.call(kLoopNamed, frame, 0x822564D8, movie, color_node());
    }
  }
  // UiMovie::HideNode or ShowNode(m, +9728, +9824): the node's +80.
  const uint32_t child = CLS(base, screen).part_child();
  if (int32_t(child) < 0) return result;
  const uint32_t node = ui_movies::FindNode(base, movie, CLS(base, screen).text_sub_scene(), child);
  schema::UiMovieNode(base, node).set_visible(hiding ? 0 : 1);
  return node;
}

// --- The steps ---

uint64_t ColorListScreenStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258578 ColorListScreen::Step0");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = MovieOf(base, screen, frame);
  // The leaving step's quirk: +9668 is never set here, so this runs on
  // every call.
  if (CLS(base, screen).step_started() == 0) {
    const uint32_t named = CLS(base, screen).leave_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x822585DC);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x822585E8);
  if (int32_t(CLS(base, screen).leave_anim()) < 0) done = 1;
  if (done != 0) {
    Call1(calls, kFinishAll, movie, frame, 0x82258610);
    FadeOut(base, calls);
  }
  return done;
}

uint64_t ColorListScreenStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255890 ColorListScreen::Step1");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (CLS(base, screen).step_started() == 0) {
    Replay(base, calls, screen, movie, 9864, frame, 0x822558DC, 0x822558F8, 0);
    Replay(base, calls, screen, movie, 9888, frame, 0x8225590C, 0x82255928, 0);
    Replay(base, calls, screen, movie, 9832, frame, 0x8225593C, 0x82255958, 0);
    Replay(base, calls, screen, movie, 9856, frame, 0x8225596C, 0x82255988, 0x82255994);
    Replay(base, calls, screen, movie, 9860, frame, 0x822559A8, 0x822559C4, 0x822559D0);
    mode_tick::ScreenFadesFadeIn(base, G::fade_in_passes(base), 0, 0, 0, 0);
    calls.set_r4(0);
    // The first list's ten nodes, +9976 on.
    for (uint32_t i = 0; i < 10; i++) {
      FinishIf(calls, movie, LoadU32(base, CLS(base, screen).first_anims_address() + 4 * i), frame,
               0x82255A08);
    }
    ListScreenClearScrollText(base, r3, frame, calls);
    const uint32_t selection = race_screens::ListSelection(base, screen + 10360);
    ListScreenSetScrollText(base, r3, Entry(base, screen, 2754, selection), 1, frame, calls);
    CLS(base, screen).set_step_started(1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82255A54);
  if (int32_t(CLS(base, screen).open_anim()) < 0 && int32_t(CLS(base, screen).open_anim_b()) < 0) {
    done = 1;
  }
  if (done == 0) return 0;
  Replay(base, calls, screen, movie, 9868, frame, 0x82255A94, 0x82255AB0, 0x82255ABC);
  Replay(base, calls, screen, movie, 9876, frame, 0x82255AD0, 0x82255AEC, 0x82255AF8);
  Replay(base, calls, screen, movie, 9880, frame, 0x82255B0C, 0x82255B28, 0x82255B34);
  ListScreenShowToggle(base, r3, r3 + 10288, frame, calls);
  ListScreenLoopNode(base, r3, r3 + 10360, 9976, frame, calls);
  return done;
}

uint64_t ColorListScreenStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256740 ColorListScreen::Step2");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (CLS(base, screen).step_started() == 0) {
    // The color node table's slot 1 (ColorNodeTable::Reset), r4 left.
    Slot(base, r3 + 10432, 4, std::nullopt, frame, 0x8225678C, calls);
    // Slot 2 (SetItemText) with r4 = +10148.
    Slot(base, r3, 8, CLS(base, screen).course_list_scroll(), frame, 0x822567A4, calls);
    Replay(base, calls, screen, movie, 9884, frame, 0, 0x822567C8, 0x822567D4);
    Replay(base, calls, screen, movie, 9872, frame, 0, 0x822567F8, 0x82256804);
    const uint32_t named = CLS(base, screen).first_to_items_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x82256828);
    ColorListScreenPlaceItemCursor(base, r3, race_screens::ListSelection(base, screen + 10144),
                                   frame);
    const uint32_t sub_scene = CLS(base, screen).color_cursor_node();
    if (int32_t(sub_scene) >= 0) {
      const uint32_t child = CLS(base, screen).color_cursor_child();
      if (int32_t(child) >= 0) SetShown(base, movie, sub_scene, child, 0);
    }
    // Slot 27 (ShowMarks) with r4 = +10148.
    Slot(base, r3, 108, CLS(base, screen).course_list_scroll(), frame, 0x82256874, calls);
    ListScreenSetScrollText(base, r3, CLS(base, screen).text_item_moved(), 1, frame, calls);
    CLS(base, screen).set_step_started(1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82256898);
  const uint32_t named = CLS(base, screen).first_to_items_anim();
  if (int32_t(named) < 0) done = 1;
  if (done == 0) return 0;
  FinishIf(calls, movie, named, frame, 0x822568C8);
  ListScreenLoopNode(base, r3, r3 + 10144, 9936, frame, calls);
  if (CLS(base, screen).node_table().address != 0) {
    ColorNodeTableRefresh(base, r3 + 10432, 0, frame, calls);
    ColorNodeTableMarkSelected(base, r3 + 10432, kNone, frame);
  }
  return done;
}

uint64_t ColorListScreenStep3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255B58 ColorListScreen::Step3");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (CLS(base, screen).step_started() == 0) {
    FinishIf(calls, movie, CLS(base, screen).items_loop(), frame, 0x82255BA4);
    const uint32_t item = race_screens::ListSelection(base, screen + 10144);
    FinishIf(calls, movie, Entry(base, screen, 2484, item), frame, 0x82255BC8);
    FinishIf(calls, movie, CLS(base, screen).shown_loop_d(), frame, 0x82255BDC);
    const uint32_t named = CLS(base, screen).items_to_first_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x82255C00);
    const uint32_t selection = race_screens::ListSelection(base, screen + 10360);
    ListScreenSetScrollText(base, r3, Entry(base, screen, 2754, selection), 1, frame, calls);
    CLS(base, screen).set_step_started(1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82255C38);
  if (int32_t(CLS(base, screen).items_to_first_anim()) < 0) done = 1;
  if (done == 0) return 0;
  ListScreenLoopNode(base, r3, r3 + 10360, 9976, frame, calls);
  FinishIf(calls, movie, CLS(base, screen).items_to_first_anim(), frame, 0x82255C78);
  return done;
}

uint64_t ColorListScreenStep4(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256908 ColorListScreen::Step4");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (CLS(base, screen).step_started() == 0) {
    ColorNodeTableRefresh(base, r3 + 10432, 0, frame, calls);
    ColorNodeTableMarkSelected(base, r3 + 10432,
                               race_screens::ListSelection(base, screen + 10216), frame);
    const uint32_t item = race_screens::ListSelection(base, screen + 10144);
    // Slot 3 (ClearItemText) with r4 = the item.
    Slot(base, r3, 12, item, frame, 0x8225698C, calls);
    FinishIf(calls, movie, CLS(base, screen).items_loop(), frame, 0x822569A0);
    FinishIf(calls, movie, Entry(base, screen, 2484, item), frame, 0x822569BC);
    FinishIf(calls, movie, Entry(base, screen, 2524, item), frame, 0x822569D8);
    uint32_t named = Entry(base, screen, 2514, item);
    if (int32_t(named) >= 0) Play(calls, movie, named, 1, frame, 0x82256A04);
    named = CLS(base, screen).items_to_colors_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x82256A28);
    ColorListScreenPlaceColorCursor(base, r3, race_screens::ListSelection(base, screen + 10216),
                                    frame);
    ListScreenSetScrollText(base, r3, CLS(base, screen).text_colors(), 1, frame, calls);
    CLS(base, screen).set_step_started(1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82256A60);
  const uint32_t named = CLS(base, screen).items_to_colors_anim();
  if (int32_t(named) < 0) done = 1;
  if (done == 0) return 0;
  FinishIf(calls, movie, named, frame, 0x82256A90);
  const uint32_t child = CLS(base, screen).color_cursor_child();
  if (int32_t(child) >= 0) SetShown(base, movie, CLS(base, screen).color_cursor_node(), child, 1);
  ListScreenLoopNode(base, r3, r3 + 10216, 10016, frame, calls);
  return done;
}

uint64_t ColorListScreenRunMovie(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255C88 ColorListScreen::RunMovie");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = MovieOf(base, uint32_t(r3), frame);
  return calls.call(kRun, frame, 0x82255CBC, movie, 0);
}

uint64_t ColorListScreenStep6(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255CD0 ColorListScreen::Step6");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (CLS(base, screen).step_started() == 0) {
    // Slot 2 (SetItemText) with r4 = +10148.
    Slot(base, r3, 8, CLS(base, screen).course_list_scroll(), frame, 0x82255D20, calls);
    ColorNodeTableRefresh(base, r3 + 10432, 0, frame, calls);
    ColorNodeTableMarkSelected(base, r3 + 10432, kNone, frame);
    const uint32_t color = race_screens::ListSelection(base, screen + 10216);
    FinishIf(calls, movie, Entry(base, screen, 2504, color), frame, 0x82255D60);
    Replay(base, calls, screen, movie, 9872, frame, 0, 0x82255D84, 0x82255D90);
    const uint32_t item = race_screens::ListSelection(base, screen + 10144);
    uint32_t named = Entry(base, screen, 2524, item);
    if (int32_t(named) >= 0) Play(calls, movie, named, 1, frame, 0x82255DC8);
    FinishIf(calls, movie, Entry(base, screen, 2514, item), frame, 0x82255DE4);
    named = CLS(base, screen).colors_to_items_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x82255E08);
    const uint32_t child = CLS(base, screen).color_cursor_child();
    if (int32_t(child) >= 0) SetShown(base, movie, CLS(base, screen).color_cursor_node(), child, 0);
    // Slot 27 (ShowMarks) with r4 = +10148.
    Slot(base, r3, 108, CLS(base, screen).course_list_scroll(), frame, 0x82255E38, calls);
    ListScreenSetScrollText(base, r3, CLS(base, screen).text_item_moved(), 1, frame, calls);
    CLS(base, screen).set_step_started(1);
  }
  // Done by +9844, not the +9852 played.
  uint64_t done = RunDone(calls, movie, frame, 0x82255E5C);
  if (int32_t(CLS(base, screen).items_to_first_anim()) < 0) done = 1;
  if (done == 0) return 0;
  ListScreenLoopNode(base, r3, r3 + 10144, 9936, frame, calls);
  if (int32_t(CLS(base, screen).items_to_first_anim()) >= 0) {
    Finish(calls, movie, CLS(base, screen).colors_to_items_anim(), frame, 0x82255EA0);
  }
  return done;
}

uint64_t ColorListScreenStep7(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256AC0 ColorListScreen::Step7");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 176);
  const uint32_t movie = MovieOf(base, screen, frame);
  const uint32_t items = screen + 10144;
  const uint32_t s = race_screens::ListSelection(base, items);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const schema::MenuList menu(base, items);
  const uint32_t p = menu.selection_before();
  const int32_t scrolled = int32_t(menu.scroll() - menu.scroll_before());
  const uint32_t cursor = CLS(base, screen).course_list_wrapped();
  const int32_t moved = int32_t(s - p);
  ColorNodeTableRefresh(base, r3 + 10432, 0, frame, calls);
  ColorNodeTableMarkSelected(base, r3 + 10432, kNone, frame);
  // Slot 2 (SetItemText) with r4 = +10148.
  Slot(base, r3, 8, CLS(base, screen).course_list_scroll(), frame, 0x82256B54, calls);
  const uint32_t was = ((p + 2484) << 2) + screen;
  Finish(calls, movie, LoadU32(base, was), frame, 0x82256B68);
  calls.call(kUnloopNamed, frame, 0x82256B74, movie, LoadU32(base, was));
  const uint32_t is = ((s + 2484) << 2) + screen;
  Finish(calls, movie, LoadU32(base, is), frame, 0x82256B88);
  calls.call(kUnloopNamed, frame, 0x82256B94, movie, LoadU32(base, is));
  Finish(calls, movie, CLS(base, screen).item_up_anim(), frame, 0x82256BA0);
  Finish(calls, movie, CLS(base, screen).item_down_anim(), frame, 0x82256BAC);
  Play(calls, movie, LoadU32(base, is), 1, frame, 0x82256BC8);
  calls.call(kLoopNamed, frame, 0x82256BD4, movie, LoadU32(base, is));
  if (cursor == 0) {
    if (moved > 0) {
      Play(calls, movie, CLS(base, screen).item_down_anim(), 1, frame, 0x82256C0C);
    } else if (moved < 0) {
      Play(calls, movie, CLS(base, screen).item_up_anim(), 1, frame, 0x82256C0C);
    }
  } else {
    ColorListScreenPlaceItemCursor(base, r3, s, frame);
  }
  if (scrolled != 0) {
    Finish(calls, movie, CLS(base, screen).scroll_down_anim(), frame, 0x82256C30);
    Finish(calls, movie, CLS(base, screen).scroll_up_anim(), frame, 0x82256C3C);
    if (scrolled < 0) {
      Play(calls, movie, CLS(base, screen).scroll_up_anim(), 1, frame, 0x82256C6C);
    } else {
      Play(calls, movie, CLS(base, screen).scroll_down_anim(), 1, frame, 0x82256C6C);
    }
  }
  // Slot 27 (ShowMarks) with r4 = +10148.
  Slot(base, r3, 108, CLS(base, screen).course_list_scroll(), frame, 0x82256C84, calls);
  calls.call(kRun, frame, 0x82256C90, movie, 0);
  return 1;
}

uint64_t ColorListScreenStep8(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82256030 ColorListScreen::Step8");
  static constexpr MovedLrs kLrs = {0x822560D4, 0x822560FC, 0x82256118, 0x82256124, 0x82256150,
                                    0x82256178, 0x82256194, 0x822561B0, 0x822561C4};
  return ListMoved(base, r3, 10216, 2504, 9924, 9920, true, 144, kLrs, sp, calls);
}

uint64_t ColorListScreenStep10(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255EB0 ColorListScreen::Step10");
  static constexpr MovedLrs kLrs = {0x82255F34, 0x82255F5C, 0x82255F78, 0x82255F84, 0x82255FB0,
                                    0x82255FD8, 0x82255FF4, 0x82256010, 0x82256024};
  return ListMoved(base, r3, 10360, 2494, 9916, 9912, false, 144, kLrs, sp, calls);
}

// --- The destructors ---

uint64_t ColorListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255198 ColorListScreen::~ColorListScreen");
  const uint32_t frame = Frame(base, sp, 96);
  CLS(base, uint32_t(r3)).set_vtable(kColorListScreenVtable);
  NodeTableDestruct(base, r3 + 10432);
  ReleaseList(base, r3 + 10360, frame, 0x822551C8, calls);
  ReleaseList(base, r3 + 10288, frame, 0x822551D0, calls);
  ReleaseList(base, r3 + 10216, frame, 0x822551D8, calls);
  ReleaseList(base, r3 + 10144, frame, 0x822551E0, calls);
  return ListScreenDestruct(base, r3);
}

uint64_t ColorListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255840 ColorListScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  ColorListScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x82255870);
  }
  return r3;
}

}  // namespace type67_screens
