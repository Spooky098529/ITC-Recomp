// The node screen (NodeScreen, Type6NodeScreen's base;
// docs/functions/type67-screens.md): its show handler, the page and step
// tables its Draw and Update build on first use with their pages and steps,
// the item loader, the course records' texts, the arrows, the toggle, and
// the destructors. See type67_screens.h. UiMovies::Find, UiMovie::FindNode,
// Child, SetNodeTexture and IsNamedActive, MenuList::Selection,
// ScreenFades::FadeIn and FadeOut, MenuSound::Play, SaveData::Of and
// CourseRecords::Of through their plain C++ (MenuList::GetPreviousSelection,
// a one-load leaf, inline); UiMovie::FinishNamed, PlayNamed, LoopNamed,
// UnloopNamed, ResetPose, Run, FinishAll and ClearText, MenuList::Update,
// Accepted and Cancelled guest calls to their overrides; UiMovie::SetText
// through Calls::set_text; the node table's Refresh and destructor by their
// plain C++; the screen's slots through Slot and Slot5 (Type6NodeScreen's
// own, in type6_node.cpp, as plain C++), the tables' entries through
// SlotNative where they hold one of ours.

#include "type67_screens/list_pages.h"
#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "schema/car_settings.h"
#include "schema/race_screens.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"
#include "save_data/save_data.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace type67_screens {

namespace {

using guest::LoadU16;
using G = schema::Type67ScreensGlobals;
using guest::LoadU32;
using guest::StoreU32;
using NS = schema::NodeScreen;
using namespace pages;

constexpr uint32_t kFinishNamed = 0x8211DF20, kLoopNamed = 0x8211DDE0,
                   kUnloopNamed = 0x8211DE30, kClearText = 0x8211C470;
constexpr uint32_t kNone = 0xFFFFFFFFu;
// The node screen's page and step tables in .data and their guards.
constexpr uint32_t kNodePages = 0x82816078, kNodePageGuard = 0x828160A8;
constexpr uint32_t kNodeSteps = 0x828160AC, kNodeStepGuard = 0x828160C4;
// The course records in the save data.
constexpr uint32_t kCourseRecords = 0x1D52C;

// The screen's movie: [+10100] at the frame's +80, then UiMovies::Find.
uint32_t NodeMovieOf(uint8_t* base, uint32_t screen, uint32_t frame) {
  StoreU32(base, frame + 80, NS(base, screen).movie());
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

// FinishNamed(movie, named) as a guest call.
uint64_t Finish(const Calls& calls, uint32_t movie, uint32_t named, uint32_t frame,
                uint32_t lr) {
  return calls.call(kFinishNamed, frame, lr, movie, named);
}

// PlayNamed (chained 1) and LoopNamed of the screen's word at `field` when
// it's >= 0 (each call reading it again). LoopNamed's r3, else the word.
uint64_t PlayLoop(uint8_t* base, const Calls& calls, uint32_t screen, uint32_t movie,
                  uint32_t field, uint32_t frame, uint32_t lr_play, uint32_t lr_loop) {
  const uint32_t named = LoadU32(base, screen + field);
  if (int32_t(named) < 0) return named;
  Play(calls, movie, named, 1, frame, lr_play);
  return calls.call(kLoopNamed, frame, lr_loop, movie, LoadU32(base, screen + field));
}

// The record's i-th entry (CourseRecords::Of's record): the chain from the
// halfword +44 walked i times (each node's first halfword the next, words
// from the record), the node's second halfword the entry, 28 bytes each
// from the record's +52.
schema::CourseRecordEntry RecordEntry(uint8_t* base, uint32_t record, uint32_t i) {
  const schema::CourseRecord r(base, record);
  uint32_t node = record + (uint32_t(int32_t(int16_t(r.first()))) << 2);
  for (uint32_t k = 0; k < i; k++) {
    node = record + (uint32_t(int32_t(int16_t(LoadU16(base, node)))) << 2);
  }
  return schema::CourseRecordEntry(
      base, r.entries_address() + uint32_t(int32_t(int16_t(LoadU16(base, node + 2))) * 28));
}

}  // namespace

// --- The show handler and the tables ---

uint64_t NodeScreenShow(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259CA0 NodeScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  NodeScreenRefresh(base, r3, frame, calls);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  const uint32_t sub_scene = NS(base, screen).sub_scene();
  calls.set_r4(sub_scene);
  if (int32_t(sub_scene) >= 0) {
    const uint32_t title = NS(base, screen).title_child();
    if (int32_t(title) >= 0) calls.set_text(frame, 0x82259CF8, movie, sub_scene, title, 322);
    const uint32_t first = NS(base, screen).text_child_a();
    if (int32_t(first) >= 0) {
      const uint32_t at = NS(base, screen).sub_scene();
      calls.set_r4(at);
      calls.set_text(frame, 0x82259D14, movie, at, first, 0);
    }
    const uint32_t second = NS(base, screen).text_child_b();
    if (int32_t(second) >= 0) {
      const uint32_t at = NS(base, screen).sub_scene();
      calls.set_r4(at);
      calls.set_text(frame, 0x82259D30, movie, at, second, 1);
    }
  }
  // Page 2 after step 0.
  const uint32_t page = NS(base, screen).page();
  if (page != 2) NS(base, screen).set_previous_page(page);
  NS(base, screen).set_queued_step(kNone);
  NS(base, screen).set_page(kNone);
  NS(base, screen).set_step(0);
  NS(base, screen).set_next_page(2);
  // Slot 15 (Type6NodeScreen::SetUpMenus) with the handler's r4.
  return Slot(base, r3, 60, r4, frame, 0x82259D74, calls);
}

uint64_t NodeScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A1B8 NodeScreen::Draw");
  const uint32_t frame = Frame(base, sp, 160);
  static constexpr uint32_t kEntries[6] = {kEmpty,     kEmpty, 0x82259D80,
                                           kEmpty,     0x822590E8, 0x82259328};
  BuildTable(base, frame, kNodePageGuard, kNodePages, kEntries, 6);
  const uint32_t page = NS(base, uint32_t(r3)).page();
  if (page == kNone) return r3;
  const uint32_t entry = kNodePages + (page << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return CallEntry(base, target, self, r4, std::nullopt, frame, 0x8225A2A8, calls);
}

uint64_t NodeScreenUpdate(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A2C0 NodeScreen::Update");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  const uint32_t id = NS(base, screen).movie();
  const uint32_t step = NS(base, screen).step();
  if (int32_t(id) < 0) return r3;
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  if (step == kNone) return calls.call(kRun, frame, 0x8225A3D0, movie, 0);
  static constexpr uint32_t kEntries[3] = {0x82259EC0, 0x82259680, 0x82259750};
  BuildTable(base, frame, kNodeStepGuard, kNodeSteps, kEntries, 3);
  const uint32_t entry = kNodeSteps + (step << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  // r4 as UiMovies::Find leaves it (the id's word).
  const uint64_t done =
      CallEntry(base, target, self, frame + 80, std::nullopt, frame, 0x8225A38C, calls);
  if (int32_t(uint32_t(done)) == 0) return done;
  // The page block: the queued step or the page after.
  const uint32_t queued = NS(base, screen).queued_step();
  if (queued == kNone) {
    const uint32_t after = NS(base, screen).next_page();
    NS(base, screen).set_step(kNone);
    NS(base, screen).set_page(after);
  } else {
    NS(base, screen).set_unknown_10124(0);
    NS(base, screen).set_step(queued);
    NS(base, screen).set_queued_step(kNone);
  }
  NS(base, screen).set_step_started(0);
  return done;
}

// --- The pages ---

uint64_t NodeScreenPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259D80 NodeScreen::Page2");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint64_t list = r3 + 10132;
  Call1(calls, race_screens::kListUpdate, list, frame, 0x82259D9C);
  uint64_t result = 0;
  if (NS(base, screen).course_list_move() != 0 ||
      NS(base, screen).course_list_scroll() - NS(base, screen).course_list_scroll_before() != 0) {
    // The list moved: the pairs saved, the item loaded, page 2 after step 2.
    PlaySound(base, 4, frame, calls);
    const uint32_t a = NS(base, screen).table_item(), b = NS(base, screen).table_second(),
                   c = NS(base, screen).table_third();
    NS(base, screen).set_table_item_before(a);
    NS(base, screen).set_table_second_before(b);
    NS(base, screen).set_table_third_before(c);
    NodeScreenLoadItem(base, r3, frame, calls);
    const uint32_t page = NS(base, screen).page();
    if (page != 2) NS(base, screen).set_previous_page(page);
    NS(base, screen).set_step(2);
    NS(base, screen).set_queued_step(kNone);
    NS(base, screen).set_next_page(2);
    NS(base, screen).set_page(kNone);
  } else if (int32_t(uint32_t(Call1(calls, race_screens::kListAccepted, list, frame,
                                    0x82259DC0))) != 0 ||
             int32_t(uint32_t(Call1(calls, race_screens::kListCancelled, list, frame,
                                    0x82259DD0))) != 0) {
    // Accepted or cancelled: page 4 after step 1 (the leaving).
    result = PlaySound(base, 2, frame, calls);
    const uint32_t page = NS(base, screen).page();
    if (page != 4) NS(base, screen).set_previous_page(page);
    NS(base, screen).set_queued_step(kNone);
    NS(base, screen).set_step(1);
    NS(base, screen).set_next_page(4);
    NS(base, screen).set_page(kNone);
    return result;
  } else {
    result = Call1(calls, race_screens::kListUpdate, r3 + 10204, frame, 0x82259DE0);
    if (NS(base, screen).toggle_move() == 0) return result;
    PlaySound(base, 4, frame, calls);
    NodeScreenOnToggleMoved(base, r3, frame, calls);
  }
  const uint32_t traffic = race_screens::ListSelection(base, screen + 10204);
  const uint32_t selection = race_screens::ListSelection(base, screen + 10132);
  const uint64_t course = uint64_t(NS(base, screen).course_list_scroll()) + uint64_t(selection);
  return NodeScreenShowRecords(base, r3, course, traffic, frame, calls);
}

uint64_t NodeScreenPage4(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822590E8 NodeScreen::Page4");
  if ((G::fade_out_count(base) | G::fade_in_count(base)) != 0) return r3;
  const uint32_t screen = uint32_t(r3);
  const uint32_t page = NS(base, screen).page();
  if (page != 5) NS(base, screen).set_previous_page(page);
  NS(base, screen).set_step(kNone);
  NS(base, screen).set_page(5);
  NS(base, screen).set_queued_step(kNone);
  return r3;
}

uint64_t NodeScreenPage5(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259328 NodeScreen::Page5");
  const uint32_t frame = Frame(base, sp, 96);
  // Slot 6 (Type6NodeScreen::Back) with the page's r4 (the dispatcher).
  const uint64_t result = Slot(base, r3, 24, r4, frame, 0x8225934C, calls);
  NS(base, uint32_t(r3)).set_state(9);
  return result;
}

// --- The steps ---

uint64_t NodeScreenStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259EC0 NodeScreen::Step0");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  if (NS(base, screen).step_started() == 0) {
    uint32_t named = NS(base, screen).open_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x82259F1C);
    named = NS(base, screen).open_anim_b();
    if (int32_t(named) >= 0) Play(calls, movie, named, 1, frame, 0x82259F40);
    PlayLoop(base, calls, screen, movie, 10292, frame, 0x82259F64, 0x82259F70);
    PlayLoop(base, calls, screen, movie, 10296, frame, 0x82259F94, 0x82259FA0);
    named = NS(base, screen).open_anim_c();
    if (int32_t(named) >= 0) Play(calls, movie, named, 1, frame, 0x82259FC4);
    named = NS(base, screen).records_anim();
    if (int32_t(named) >= 0 && ui_movies::IsNamedActive(base, movie, named) == 0) {
      PlayLoop(base, calls, screen, movie, 10392, frame, 0x82259FFC, 0x8225A008);
    }
    // Slot 22 (Type6NodeScreen::ShowMarks) with r4 = +10136.
    Slot(base, r3, 88, NS(base, screen).course_list_scroll(), frame, 0x8225A020, calls);
    mode_tick::ScreenFadesFadeIn(base, 10, 0, 0, 0, 0);
    calls.set_r4(0);
    // Slot 21 (Type6NodeScreen::SetScrollText), r4 as FadeIn left it.
    Slot(base, r3, 84, std::nullopt, frame, 0x8225A04C, calls);
    NS(base, screen).set_step_started(1);
  }
  uint64_t done = uint32_t(calls.call(kRun, frame, 0x8225A060, movie, 0)) == 0 ? 1 : 0;
  if (int32_t(NS(base, screen).open_anim()) < 0 && int32_t(NS(base, screen).open_anim_b()) < 0) {
    done = 1;
  }
  if (done == 0) return 0;
  PlayLoop(base, calls, screen, movie, 10304, frame, 0x8225A0B0, 0x8225A0BC);
  PlayLoop(base, calls, screen, movie, 10308, frame, 0x8225A0E0, 0x8225A0EC);
  PlayLoop(base, calls, screen, movie, 10312, frame, 0x8225A110, 0x8225A11C);
  PlayLoop(base, calls, screen, movie, 10316, frame, 0x8225A140, 0x8225A14C);
  NodeScreenShowSelection(base, r3, frame, calls);
  PairedNodeTableRefresh(base, r3, frame, calls);
  return done;
}

uint64_t NodeScreenStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259680 NodeScreen::Step1");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  // The leaving step's quirk: +10128 is never set here, so this runs on
  // every call.
  if (NS(base, screen).step_started() == 0) {
    const uint32_t named = NS(base, screen).leave_anim();
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x822596E4);
  }
  uint64_t done = uint32_t(calls.call(kRun, frame, 0x822596F0, movie, 0)) == 0 ? 1 : 0;
  if (int32_t(NS(base, screen).leave_anim()) < 0) done = 1;
  if (done != 0) {
    Call1(calls, kFinishAll, movie, frame, 0x82259718);
    // ScreenFades::FadeOut(30, 0, 0, 0, 0): 30 given, not [0x82001BF0].
    mode_tick::ScreenFadesFadeOut(base, 30, 0, 0, 0, 0);
    calls.set_r4(0);
  }
  return done;
}

uint64_t NodeScreenStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259750 NodeScreen::Step2");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 176);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  const uint32_t list = screen + 10132;
  const uint32_t s = race_screens::ListSelection(base, list);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const schema::MenuList menu(base, list);
  const uint32_t p = menu.selection_before();
  const int32_t moved = int32_t(s - p);
  const uint32_t wrapped = NS(base, screen).course_list_wrapped();
  const int32_t scrolled = int32_t(menu.scroll() - menu.scroll_before());
  // The items' named animations, [+10352 + 4i].
  const uint32_t was = ((p + 2588) << 2) + screen;
  Finish(calls, movie, LoadU32(base, was), frame, 0x822597C8);
  calls.call(kUnloopNamed, frame, 0x822597D4, movie, LoadU32(base, was));
  const uint32_t is = ((s + 2588) << 2) + screen;
  Finish(calls, movie, LoadU32(base, is), frame, 0x822597E8);
  calls.call(kUnloopNamed, frame, 0x822597F4, movie, LoadU32(base, is));
  Finish(calls, movie, NS(base, screen).item_up_anim(), frame, 0x82259800);
  Finish(calls, movie, NS(base, screen).item_down_anim(), frame, 0x8225980C);
  PairedNodeTableRefresh(base, r3, frame, calls);
  // Slot 2 (Type6NodeScreen::SetText) with r4 = +10136.
  Slot(base, r3, 8, NS(base, screen).course_list_scroll(), frame, 0x8225982C, calls);
  Play(calls, movie, LoadU32(base, is), 1, frame, 0x82259848);
  calls.call(kLoopNamed, frame, 0x82259854, movie, LoadU32(base, is));
  if (wrapped == 0) {
    if (moved > 0) {
      Play(calls, movie, NS(base, screen).item_down_anim(), 1, frame, 0x8225988C);
    } else if (moved < 0) {
      Play(calls, movie, NS(base, screen).item_up_anim(), 1, frame, 0x8225988C);
    }
  } else {
    // The cursor's child +16 = the selection × 40 as a float (the product
    // left in the frame's +80 as a 64-bit integer, the conversion's spill).
    const uint32_t node = ui_movies::FindNode(base, movie, kNone, NS(base, screen).cursor_node());
    const int64_t product = int64_t(int32_t(s * 40));
    StoreU32(base, frame + 80, uint32_t(uint64_t(product) >> 32));
    StoreU32(base, frame + 84, uint32_t(uint64_t(product)));
    const uint32_t child = ui_movies::Child(base, node, NS(base, screen).cursor_child());
    schema::UiMovieNode(base, child).set_y(float(double(product)));
  }
  if (scrolled != 0) {
    Finish(calls, movie, NS(base, screen).scroll_down_anim(), frame, 0x822598D8);
    Finish(calls, movie, NS(base, screen).scroll_up_anim(), frame, 0x822598E4);
    Play(calls, movie, LoadU32(base, screen + (scrolled < 0 ? 10348 : 10344)), 1, frame,
         0x82259914);
  }
  // Slot 22 (Type6NodeScreen::ShowMarks) with r4 = +10136.
  Slot(base, r3, 88, NS(base, screen).course_list_scroll(), frame, 0x8225992C, calls);
  calls.call(kRun, frame, 0x82259938, movie, 0);
  return 1;
}

// --- The helpers ---

uint64_t NodeScreenShowSelection(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259A78 NodeScreen::ShowSelection");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  const uint32_t selection = race_screens::ListSelection(base, screen + 10132);
  if (int32_t(selection) >= 0) {
    const uint32_t named = LoadU32(base, ((selection + 2588) << 2) + screen);
    if (int32_t(named) >= 0) {
      Finish(calls, movie, named, frame, 0x82259AD8);
      Play(calls, movie, named, 1, frame, 0x82259AF4);
      calls.call(kLoopNamed, frame, 0x82259B00, movie, named);
    }
  }
  const uint32_t toggle = screen + 10204;
  NodeScreenSetArrows(base, r3, race_screens::ListSelection(base, toggle), 1, frame);
  const uint32_t which = race_screens::ListSelection(base, toggle);
  const uint32_t named =
      (which == 0 ? NS(base, screen).toggle_anim_a() : NS(base, screen).toggle_anim_b());
  if (int32_t(named) < 0) return which;
  Play(calls, movie, named, 1, frame, 0x82259B5C);
  return calls.call(kLoopNamed, frame, 0x82259B68, movie, named);
}

uint64_t NodeScreenOnToggleMoved(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259B70 NodeScreen::OnToggleMoved");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  const uint32_t toggle = screen + 10204;
  const uint32_t selection = race_screens::ListSelection(base, toggle);
  // MenuList::GetPreviousSelection (sub_8220BC28), read and unused.
  (void)schema::MenuList(base, toggle).selection_before();
  const bool first = selection == 0;
  // The node finished, then the one finished, played and looped.
  const uint32_t off = first ? 10328 : 10324, on = first ? 10324 : 10328;
  const uint32_t lr_finish_off = first ? 0x82259BF4 : 0x82259C58;
  const uint32_t lr_finish_on = first ? 0x82259C08 : 0x82259C6C;
  const uint32_t lr_play = first ? 0x82259C24 : 0x82259C88;
  NodeScreenSetArrows(base, r3, 0, first ? 1 : 0, frame);
  uint64_t result = NodeScreenSetArrows(base, r3, 1, first ? 0 : 1, frame);
  const uint32_t named_off = LoadU32(base, screen + off);
  if (int32_t(named_off) >= 0) result = Finish(calls, movie, named_off, frame, lr_finish_off);
  const uint32_t named_on = LoadU32(base, screen + on);
  if (int32_t(named_on) < 0) return result;
  Finish(calls, movie, named_on, frame, lr_finish_on);
  Play(calls, movie, LoadU32(base, screen + on), 1, frame, lr_play);
  return calls.call(kLoopNamed, frame, 0x82259C94, movie, LoadU32(base, screen + on));
}

uint64_t NodeScreenSetArrows(uint8_t* base, uint64_t r3, uint32_t which, uint32_t on,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82259368 NodeScreen::SetArrows");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t child =
      (int32_t(which) == 0 ? NS(base, screen).arrow_child_a() : NS(base, screen).arrow_child_b());
  if (int32_t(child) < 0) return r3;
  const uint32_t sub_scene = NS(base, screen).sub_scene();
  if (int32_t(sub_scene) < 0) return r3;
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  // The arrow's frame word, read for each use.
  auto frame_word = [&] {
    return int32_t(on) != 0 ? NS(base, screen).arrow_on_frame()
                            : NS(base, screen).arrow_off_frame();
  };
  if (int32_t(frame_word()) < 0) return movie;
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  schema::UiMovieNode(base, node).set_texture_entry(frame_word());
  return node;
}

uint64_t NodeScreenRefresh(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822599B0 NodeScreen::Refresh");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  // r4 as Find leaves it (the id's word).
  calls.call(kResetPose, frame, 0x822599E8, movie, frame + 80);
  // Slot 1 (NodeScreen::SetUpTexts), r4 as ResetPose left it.
  Slot(base, r3, 4, std::nullopt, frame, 0x822599FC, calls);
  const uint32_t traffic = race_screens::ListSelection(base, screen + 10204);
  const uint32_t selection = race_screens::ListSelection(base, screen + 10132);
  const uint64_t course = uint64_t(NS(base, screen).course_list_scroll()) + uint64_t(selection);
  NodeScreenShowRecords(base, r3, course, traffic, frame, calls);
  NodeScreenSetArrows(base, r3, 0, 0, frame);
  NodeScreenSetArrows(base, r3, 1, 0, frame);
  // Slot 20 (Type6NodeScreen::SetIcons), r4 as SetArrows left it (which the
  // slot doesn't read: it sets r4 first).
  return Slot(base, r3, 80, std::nullopt, frame, 0x82259A5C, calls);
}

uint64_t NodeScreenShowRecords(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5,
                               uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259410 NodeScreen::ShowRecords");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 160);
  const uint32_t course = uint32_t(r4);
  const uint32_t movie = NodeMovieOf(base, screen, frame);
  const uint32_t records = vehicle_accessors::SaveDataOf(base, 0) + kCourseRecords;
  StoreU32(base, frame + 88, course);
  StoreU32(base, frame + 92, uint32_t(r5));
  const uint32_t record = uint32_t(save_data::CourseRecordsOf(base, records, frame + 88));
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t texts = screen + 10416 + 4 * i;
    const uint32_t count = uint32_t(int32_t(int16_t(schema::CourseRecord(base, record).count())));
    calls.set_r4(kNone);
    if (i >= count) {
      for (const uint32_t at : {0u, 40u, 80u}) {
        const uint64_t args[6] = {movie, kNone, LoadU32(base, texts + at), 0, 0, 0};
        calls.call_set(kClearText, args, 3, frame,
                       at == 0 ? 0x82259630 : at == 40 ? 0x82259640 : 0x82259650);
      }
      continue;
    }
    // The time: minutes (the first halfword's low 15 bits), seconds (6
    // bits) and thousandths (10 bits) of the entry's +72, or 59, 59, 999
    // past 59 minutes.
    const schema::CourseRecordEntry entry = RecordEntry(base, record, i);
    // The time's first halfword (no halfword field: the time is a word).
    const uint32_t minutes = LoadU16(base, entry.time_address()) & 0x7FFF;
    uint64_t time[4] = {59, 59, 999, 0};
    if (minutes <= 59) {
      const uint32_t word = entry.time();
      time[0] = minutes;
      time[1] = (word >> 10) & 0x3F;
      time[2] = word & 0x3FF;
    }
    calls.set_text_args(frame, 0x82259584, movie, kNone, LoadU32(base, texts), 528, time);
    calls.set_r4(kNone);
    calls.set_text(frame, 0x822595D0, movie, kNone, LoadU32(base, texts + 40),
                   RecordEntry(base, record, i).unknown_0() + 1646);
    calls.set_r4(kNone);
    calls.set_text(frame, 0x8225961C, movie, kNone, LoadU32(base, texts + 80),
                   RecordEntry(base, record, i).unknown_0() + 1718);
  }
  // UiMovie::SetNodeTexture(m, -1, +10544, the course): the lookup, then the
  // texture.
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, NS(base, screen).course_node());
  return ui_movies::SetNodeTexture(base, node, course);
}

uint64_t NodeScreenLoadItem(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259050 NodeScreen::LoadItem");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t scroll = NS(base, screen).course_list_scroll();
  const uint32_t selection = race_screens::ListSelection(base, screen + 10132);
  // Slot 4 (ItemInfo) with r4 = the item, r5 = the frame's +80.
  const uint64_t result = Slot5(base, r3, 16, uint64_t(selection) + uint64_t(scroll), frame + 80,
                                frame, 0x82259094, calls);
  const uint32_t a = LoadU32(base, frame + 84), b = LoadU32(base, frame + 88),
                 c = LoadU32(base, frame + 80), d = LoadU32(base, frame + 92);
  NS(base, screen).set_table_item(a);
  NS(base, screen).set_table_second(b);
  NS(base, screen).set_table_third(c);
  NS(base, screen).set_table_fourth(d);
  return result;
}

// --- The destructors ---

uint64_t NodeScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259948 NodeScreen::~NodeScreen");
  const uint32_t frame = Frame(base, sp, 96);
  NS(base, uint32_t(r3)).set_vtable(kNodeScreenVtable);
  ReleaseList(base, r3 + 10204, frame, 0x82259970, calls);
  ReleaseList(base, r3 + 10132, frame, 0x82259978, calls);
  return NodeTableDestruct(base, r3);
}

uint64_t NodeScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225A168 NodeScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  NodeScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x8225A198);
  }
  return r3;
}

}  // namespace type67_screens
