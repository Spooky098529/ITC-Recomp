// The type 2 dialog's machine handlers (docs/functions/type2-dialog.md):
// Draw and Update, which run the pages and steps from their tables (through
// the guest context, to the pages' and steps' overrides).

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "guest.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/type2_dialog.h"
#include "ui_movies/game.h"

namespace type2_dialog {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type2DialogGlobals;
using Machine = schema::StateMachine;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The pages' table in Draw's frame: ConfirmPage, NoticePage, Confirm2Page,
// Notice2Page.
constexpr uint32_t kPageTable[4] = {0x82187160, 0x82187508, 0x82187330, 0x82187618};
// The steps' table in Update's frame: OpenConfirmStep, CloseConfirmStep,
// OpenNoticeStep, CloseNoticeStep, OpenConfirm2Step, CloseConfirm2Step,
// OpenNotice2Step, CloseNotice2Step, MoveCursorStep, MoveCursor2Step.
constexpr uint32_t kStepTable[10] = {0x82187728, 0x821877F8, 0x821878B0, 0x82187948,
                                     0x821879F8, 0x82187AC8, 0x82187B80, 0x82187C18,
                                     0x82187CC8, 0x82187D80};

}  // namespace

uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187E50 Type2Dialog::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t page = G::pages(base).page();
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, frame + 80 + 4 * i, kPageTable[i]);
  }
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  const uint32_t target = LoadU32(base, frame + 80 + (page << 2));
  return calls.CallArgs(target, frame, 0x82187EB8,
                        {mode, mode, 0, 0, kPageTable[0], kPageTable[1], kPageTable[2],
                         kPageTable[3]});
}

uint64_t Update(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187EC8 Type2Dialog::Update");
  // The empty sub_82106540(100, 30, 0, ..) the original calls first is left
  // out: it returns at once.
  const uint32_t frame = Frame(base, sp, 208);
  StoreU32(base, frame + 80, G::data(base).movie_shown());
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const auto pages = G::pages(base);
  for (uint32_t i = 0; i < 10; i++) {
    StoreU32(base, frame + 96 + 4 * i, kStepTable[i]);
  }
  const uint32_t step = pages.step();
  uint64_t answer = movie;
  if (step == 0xFFFFFFFFu) {
    answer = calls.Call(kRun, frame, 0x82188030, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      // No bounds check: the word at the table's index. r4 to r10 as the
      // original leaves them (the table's build), not read by the steps.
      const uint32_t target = LoadU32(base, frame + 96 + (step << 2));
      answer = calls.CallArgs(target, frame, 0x82187FC8,
                              {pages.step_started_address(), kStepTable[3], kStepTable[4], kStepTable[5],
                               kStepTable[6], kStepTable[7], frame + 96, step << 2});
      done = int32_t(answer) != 0;
    }
    if (done) {
      const auto p = G::pages(base);
      const uint32_t queued = p.queued_step();
      if (queued == 0xFFFFFFFFu) {
        if (int32_t(step) >= 0) {
          p.set_page(p.next_page());
          p.set_step(queued);
        } else {
          const uint32_t next = p.next_page();
          p.set_step(0xFFFFFFFFu);
          p.set_page(next);
        }
      } else {
        p.set_queued_step(0xFFFFFFFFu);
        p.set_unknown_20(0);
        p.set_step(queued);
      }
      p.set_step_started(0);
    }
  }
  const uint32_t state = G::data(base).queued_state();
  if (state == 21) {
    return answer;
  }
  if (state == 9) {
    Post(base, mode, 42);
  }
  Machine(base, machine).set_state(G::data(base).queued_state());
  G::data(base).set_queued_state(21);
  if (G::status(base) != 5) {
    G::set_status(base, 3);
  }
  return answer;
}

uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821869B8 Type2Dialog::OnEnter");
  const uint32_t frame = Frame(base, sp, 128);
  const Machine m(base, machine);
  m.set_loaded(0);
  const schema::Type2DialogPages pages(
      base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x821869E4, 0, 28, 0x1000, 4)));
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  pages.set_step_started(0);
  const schema::FrontEndMenu menu(
      base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x82186A20, 0, 68, 0x1000, 4)));
  G::set_menu(base, menu);
  menu.set_first_item(0);
  menu.set_last_item(1);
  menu.set_wraps(1);
  // Cleared in the original's order.
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_unknown_40(0);
  menu.set_unknown_48(0);
  menu.set_unknown_52(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82186A7C, 0, 68, 0x1000, 4);
  const schema::Type2DialogData data(base, uint32_t(r3));
  G::set_data(base, data);
  data.set_movies_loaded(0);
  data.set_movie_shown(0);
  data.set_queued_state(21);
  G::set_showing(base, 0);
  if (m.reenter() != 0) {
    m.set_state(1);
    G::set_status(base, 0xFFFFFFFFu);
  } else {
    m.set_state(9);
    G::set_status(base, 3);
  }
  return r3;
}

uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821880D0 Type2Dialog::WaitForMovies");
  Frame(base, sp, 96);
  const uint32_t machine = uint32_t(r3);
  const Machine m(base, machine);
  if (m.loaded() == 0) {
    uint32_t loaded = 0;
    for (uint32_t id = 16; id <= 19; id++) {
      loaded = ui_movies::IsLoaded(base, id);
      if (int32_t(loaded) == 0) {
        return loaded;
      }
    }
    m.set_loaded(1);
    return loaded;
  }
  G::data(base).set_movies_loaded(1);
  m.set_state(9);
  G::set_status(base, 3);
  return r3;
}

void Shown(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82186490 Type2Dialog::Shown");
  G::set_status(base, 0);
}

void Wait(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82186AD8 Type2Dialog::Wait");
  Machine(base, machine).set_state(9);
  G::set_status(base, 3);
}

uint64_t Cancel(uint8_t* base, uint64_t r3, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82186E40 Type2Dialog::Cancel");
  // Each dialog in turn: on its page, the page object to its close step
  // and its message posted; else, with it showing (its step running), the
  // message alone. (+8, the page before, is never written: the page is the
  // dialog's own.)
  static constexpr uint32_t kClose[4] = {1, 3, 5, 7};
  for (uint32_t k = 0; k < 4; k++) {
    const auto pages = G::pages(base);
    const uint32_t message = k == 3 ? 40 : 41;
    if (pages.page() == k) {
      switch (k) {
        case 0:
          pages.set_step(1);
          pages.set_queued_step(0xFFFFFFFFu);
          pages.set_next_page(0);
          pages.set_page(0xFFFFFFFFu);
          break;
        case 1:
          pages.set_next_page(1);
          pages.set_step(3);
          pages.set_queued_step(0xFFFFFFFFu);
          pages.set_page(0xFFFFFFFFu);
          break;
        case 2:
          pages.set_queued_step(0xFFFFFFFFu);
          pages.set_page(0xFFFFFFFFu);
          pages.set_step(5);
          pages.set_next_page(2);
          break;
        default:
          pages.set_queued_step(0xFFFFFFFFu);
          pages.set_page(0xFFFFFFFFu);
          pages.set_next_page(3);
          pages.set_step(kClose[k]);
          break;
      }
    } else if (G::showing(base) != k) {
      continue;
    }
    Post(base, mode, message, k == 3);
    if (k != 3) {
      r3 = 0;
    }
  }
  G::set_status(base, 2);
  return r3;
}

uint64_t OpenConfirm(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186C30 Type2Dialog::OpenConfirm");
  const uint32_t frame = Frame(base, sp, 112);
  G::data(base).set_movie_shown(16);
  StoreU32(base, frame + 80, G::data(base).movie_shown());
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint64_t r3 = calls.Call(kResetPose, frame, 0x82186C80, movie);
  const uint32_t choice = LoadU32(base, movie + 40960);
  const auto menu = G::menu(base);
  const uint32_t cursor = menu.cursor();
  menu.set_cursor(choice);
  menu.set_previous(cursor);
  const auto pages = G::pages(base);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  pages.set_step_started(0);
  G::set_showing(base, 0);
  if (Machine(base, machine).loaded() != 0) {
    const auto p = G::pages(base);
    const uint32_t before = p.page();
    if (before != 0) {
      p.set_previous_page(before);
    }
    p.set_step(0);
    p.set_queued_step(0xFFFFFFFFu);
    p.set_next_page(0);
    p.set_page(0xFFFFFFFFu);
    Machine(base, machine).set_state(4);
  } else {
    Machine(base, machine).set_state(1);
  }
  G::set_status(base, 5);
  return r3;
}

uint64_t OpenConfirm2(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82186D40 Type2Dialog::OpenConfirm2");
  const uint32_t frame = Frame(base, sp, 112);
  G::data(base).set_movie_shown(18);
  StoreU32(base, frame + 80, G::data(base).movie_shown());
  const uint32_t pair = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = LoadU32(base, pair);
  const auto menu = G::menu(base);
  const uint32_t choice = LoadU32(base, movie + 40960);
  const uint32_t cursor = menu.cursor();
  menu.set_cursor(choice);
  menu.set_previous(cursor);
  const auto pages = G::pages(base);
  pages.set_next_page(2);
  pages.set_page(2);
  pages.set_previous_page(2);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  pages.set_step_started(0);
  G::set_showing(base, 2);
  if (Machine(base, machine).loaded() != 0) {
    const auto p = G::pages(base);
    const uint32_t before = p.page();
    if (before != 2) {
      p.set_previous_page(before);
    }
    p.set_queued_step(0xFFFFFFFFu);
    p.set_next_page(2);
    p.set_page(0xFFFFFFFFu);
    p.set_step(4);
    Machine(base, machine).set_state(4);
  } else {
    Machine(base, machine).set_state(1);
  }
  G::set_status(base, 5);
  return pair;
}

void OpenNotice(uint8_t* base, uint32_t machine, bool second) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82186B90 Type2Dialog::OpenNotice2");
  } else {
    ITC_NATIVE_RUNNING("sub_82186AF0 Type2Dialog::OpenNotice");
  }
  const uint32_t page = second ? 3 : 1;
  G::data(base).set_movie_shown(second ? 19 : 17);
  const auto pages = G::pages(base);
  pages.set_step_started(0);
  pages.set_next_page(page);
  pages.set_page(page);
  pages.set_previous_page(page);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  G::set_showing(base, page);
  if (Machine(base, machine).loaded() != 0) {
    const auto p = G::pages(base);
    const uint32_t before = p.page();
    if (before != page) {
      p.set_previous_page(before);
    }
    p.set_queued_step(0xFFFFFFFFu);
    p.set_next_page(page);
    p.set_page(0xFFFFFFFFu);
    p.set_step(second ? 6 : 2);
    Machine(base, machine).set_state(4);
  } else {
    Machine(base, machine).set_state(1);
  }
  G::set_status(base, 5);
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186428 Type2Dialog::OnLeft");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x82186450, G::pages(base).address, 0, 0x8000);
  calls.Call(kVirtualFree, frame, 0x82186464, G::menu(base).address, 0, 0x8000);
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82186478, G::data(base).address, 0, 0x8000);
  G::set_pages(base, 0u);
  G::set_menu(base, 0u);
  G::set_data(base, 0u);
  return r3;
}

uint32_t TakesInput(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821864A0 Type2Dialog::TakesInput");
  uint32_t status = G::status(base);
  if (status == 0) {
    status = 4;
    G::set_status(base, 4);
  }
  return status == 1 ? 1 : 0;
}

}  // namespace type2_dialog
