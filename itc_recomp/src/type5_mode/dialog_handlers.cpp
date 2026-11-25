// The type 5 dialog's machine handlers (docs/functions/type5-mode.md): Draw
// and Update, which run the pages and steps from their tables (through the
// guest context, to the pages' and steps' overrides), and the transitions'
// handlers.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "hooks/heap.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/type5_mode.h"
#include "ui_movies/game.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type5ModeGlobals;
using Machine = schema::Type5Dialog;

constexpr uint32_t kNone = 0xFFFFFFFFu;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

// The pages' table in Draw's frame: ConfirmPage, NoticePage, ConfirmPage
// again (dialog 2), Notice2Page.
constexpr uint32_t kPageTable[4] = {0x82225D10, 0x82225F30, 0x82225D10, 0x82226088};
// The steps' table in Update's frame: OpenConfirmStep, CloseConfirmStep,
// the type 2 dialog's OpenNoticeStep, CloseNoticeStep, OpenConfirm2Step,
// CloseConfirm2Step, the type 2 dialog's OpenNotice2Step, CloseNotice2Step,
// MoveCursorStep, Step9.
constexpr uint32_t kStepTable[10] = {0x822261E0, 0x822262B8, 0x821878B0, 0x82226370,
                                     0x82226420, 0x822264B8, 0x82187B80, 0x82226568,
                                     0x82226618, 0x822266D0};

// Posts the message (id, 0, 0) on the dispatcher's ring (+204), the id
// stored first.
void Post(uint32_t dispatcher, uint32_t id) {
  const schema::ModeDispatcher d(dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + (end - (end / 50) * 50) * 12};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

}  // namespace

uint64_t Draw(uint64_t machine, uint64_t dispatcher, uint32_t sp,
              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822266F8 Type5Dialog::Draw");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t page = G::pages().page();
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(frame + 80 + 4 * i, kPageTable[i]);
  }
  StoreU32(frame + 96, 0);
  if (page == kNone) {
    return dispatcher;
  }
  // No bounds check, as the original: the word at the table's index.
  const uint32_t target = LoadU32(frame + 80 + (page << 2));
  return calls.CallArgs(target, frame, 0x82226774,
                        {dispatcher, machine, machine, kPageTable[0], kPageTable[1],
                         kPageTable[2], kPageTable[3], frame + 80});
}

uint64_t Update(uint32_t machine, uint32_t dispatcher, uint32_t sp,
                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82226788 Type5Dialog::Update");
  // The empty sub_82106540(100, 30, 0, "TXR_PaCmnWindow") the original
  // calls first is left out: it returns at once.
  const uint32_t frame = Frame(sp, 208);
  // The find's id (the original's +80, read only by the find) a value
  // since E5 step 6; the steps' table at +96 is handed to the step's call:
  // it stays on the stack.
  const uint32_t movie = LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, G::data().movie_shown()));
  const auto pages = G::pages();
  StoreU32(frame + 132, kStepTable[9]);
  const uint32_t step = pages.step();
  for (uint32_t i = 0; i < 9; i++) {
    StoreU32(frame + 96 + 4 * i, kStepTable[i]);
  }
  uint64_t answer = movie;
  if (step == kNone) {
    answer = calls.Call(kRun, frame, 0x822268F0, movie, 0);
  } else {
    bool done = true;
    schema::PageObject p = pages;
    if (int32_t(step) >= 0) {
      // No bounds check: the word at the table's index. r4 to r10 as the
      // original leaves them (the table's build), not read by the steps.
      const uint32_t target = LoadU32(frame + 96 + (step << 2));
      answer = calls.CallArgs(target, frame, 0x82226888,
                              {pages.step_started_address(), kStepTable[3], kStepTable[4], kStepTable[5],
                               kStepTable[6], kStepTable[7], frame + 96, step << 2});
      p = G::pages();
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t queued = p.queued_step();
      if (queued == kNone) {
        if (int32_t(step) >= 0) {
          p.set_page(p.next_page());
          p.set_step(queued);
        } else {
          const uint32_t next = p.next_page();
          p.set_step(kNone);
          p.set_page(next);
        }
      } else {
        p.set_queued_step(kNone);
        p.set_unknown_20(0);
        p.set_step(queued);
      }
      p.set_step_started(0);
    }
  }
  const uint32_t state = G::data().queued_state();
  if (state == 21) {
    return answer;
  }
  Machine(machine).set_state(state);
  const auto data = G::data();
  if (data.queued_state() == 9) {
    data.set_interrupted(0);
    Post(dispatcher, 18);
    G::data().set_queued_state(21);
  } else {
    data.set_queued_state(21);
  }
  return answer;
}

uint64_t OnEnter(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225878 Type5Dialog::OnEnter");
  // The original's 112-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  (void)calls;
  const Machine m(machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x822258A4
  // and the VirtualAlloc(68) at 0x822258E0; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New();
  G::set_pages(pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  const schema::FrontEndMenu menu = schema::FrontEndMenu::New();
  G::set_menu(menu);
  menu.set_first_item(0);
  menu.set_last_item(1);
  menu.set_wraps(1);
  // Cleared in the original's order.
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  // The data block lives in host memory (E5 step 3): the original's
  // VirtualAlloc(68) at 0x8222593C; the global holds its handle.
  const schema::Type5DialogData data = schema::Type5DialogData::New();
  const uint64_t r3 = data.address;
  G::set_data(data);
  data.set_movies_loaded(0);
  data.set_movie_shown(0);
  data.set_interrupted(0);
  data.set_queued_state(21);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t WaitForMovies(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82226978 Type5Dialog::WaitForMovies");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const Machine m(uint32_t(r3));
  if (m.loaded() == 0) {
    uint32_t loaded = 0;
    for (uint32_t id = 16; id <= 19; id++) {
      loaded = ui_movies::IsLoaded(id);
      if (int32_t(loaded) == 0) {
        return loaded;
      }
    }
    m.set_loaded(1);
    return loaded;
  }
  G::data().set_movies_loaded(1);
  m.set_state(9);
  return r3;
}

void OnInterrupt() {
  ITC_NATIVE_RUNNING("sub_82225860 Type5Dialog::OnInterrupt");
  G::data().set_interrupted(1);
}

uint64_t OpenConfirm(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225A90 Type5Dialog::OpenConfirm");
  // The original's 112-byte frame held only the find's id (a value since E5
  // step 6): ResetPose runs from `sp`.
  G::data().set_movie_shown(16);
  const uint32_t movie = LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, G::data().movie_shown()));
  const uint64_t r3 = calls.Call(kResetPose, sp, 0x82225AE0, movie);
  // The movie's +40960: the choice it last showed (a UI movie's word).
  const uint32_t choice = LoadU32(movie + 40960);
  const auto menu = G::menu();
  const uint32_t cursor = menu.cursor();
  menu.set_cursor(choice);
  menu.set_previous(cursor);
  const auto pages = G::pages();
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const Machine m(machine);
  if (m.loaded() != 0) {
    const auto p = G::pages();
    const uint32_t before = p.page();
    if (before != 0) {
      p.set_previous_page(before);
    }
    p.set_step(0);
    p.set_queued_step(kNone);
    p.set_next_page(0);
    p.set_page(kNone);
    m.set_state(4);
  } else {
    m.set_state(1);
  }
  return r3;
}

uint64_t OpenConfirm2(uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82225B88 Type5Dialog::OpenConfirm2");
  // The original's 112-byte frame held only the find's id (a value since E5
  // step 6); no guest call is made.
  (void)sp;
  G::data().set_movie_shown(18);
  const uint32_t pair =
      ui_movies::PairsFindKey(ui_movies::kMovieTable, G::data().movie_shown());
  const uint32_t movie = LoadU32(pair);
  const auto menu = G::menu();
  const uint32_t choice = LoadU32(movie + 40960);
  const uint32_t cursor = menu.cursor();
  menu.set_cursor(choice);
  menu.set_previous(cursor);
  const auto pages = G::pages();
  pages.set_next_page(2);
  pages.set_page(2);
  pages.set_previous_page(2);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const Machine m(machine);
  if (m.loaded() != 0) {
    const auto p = G::pages();
    const uint32_t before = p.page();
    if (before != 2) {
      p.set_previous_page(before);
    }
    p.set_queued_step(kNone);
    p.set_next_page(2);
    p.set_page(kNone);
    p.set_step(4);
    m.set_state(4);
  } else {
    m.set_state(1);
  }
  return pair;
}

void OpenNotice(uint32_t machine, bool second) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82225A08 Type5Dialog::OpenNotice2");
  } else {
    ITC_NATIVE_RUNNING("sub_82225980 Type5Dialog::OpenNotice");
  }
  const uint32_t page = second ? 3 : 1;
  G::data().set_movie_shown(second ? 19 : 17);
  const auto pages = G::pages();
  pages.set_next_page(page);
  pages.set_page(page);
  pages.set_previous_page(page);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const Machine m(machine);
  if (m.loaded() != 0) {
    const auto p = G::pages();
    const uint32_t before = p.page();
    if (before != page) {
      p.set_previous_page(before);
    }
    p.set_queued_step(kNone);
    p.set_next_page(page);
    p.set_page(kNone);
    p.set_step(second ? 6 : 2);
    m.set_state(4);
  } else {
    m.set_state(1);
  }
}

uint64_t OnLeft(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822257A8 Type5Dialog::OnLeft");
  // VirtualFree (sub_82295330) as its adapter answers it, called directly
  // (E5 step 6); through the dispatch, from the original's 112-byte frame,
  // where the adapter runs the original (and for 0, which its adapter
  // answers with the last error set).
  auto free_pages = [&](uint32_t pages, uint32_t lr) {
    if (pages != 0 && hooks::VirtualFree(pages, 0, 0x8000, lr)) {
      return;
    }
    calls.Call(kVirtualFree, Frame(sp, 112), lr, pages, 0, 0x8000);
  };
  free_pages(G::pages().address, 0x822257D0);
  free_pages(G::menu().address, 0x822257E4);
  // The host data block (OnEnter): the original's VirtualFree at
  // 0x822257F8, which returned 1.
  G::data().Delete();
  const uint64_t r3 = 1;
  G::set_pages(0u);
  G::set_menu(0u);
  G::set_data(0u);
  return r3;
}

}  // namespace type5_mode
