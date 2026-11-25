// The type 2 dialog's machine handlers (docs/functions/type2-dialog.md):
// Draw and Update, which run the pages and steps from their tables (through
// the guest context, to the pages' and steps' overrides).

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
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

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
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

uint64_t Draw(uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187E50 Type2Dialog::Draw");
  // The frame kept for the page's call (its back chain); the pages' table
  // the original built at its +80 is kPageTable (E5 step 6: only this
  // function read it).
  const uint32_t frame = Frame(sp, 112);
  const uint32_t page = G::pages().page();
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index (past
  // the table, the frame's word there, as the original read it).
  const uint32_t target =
      page < 4 ? kPageTable[page] : LoadU32(frame + 80 + (page << 2));
  return calls.CallArgs(target, frame, 0x82187EB8,
                        {mode, mode, 0, 0, kPageTable[0], kPageTable[1], kPageTable[2],
                         kPageTable[3]});
}

uint64_t Update(uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187EC8 Type2Dialog::Update");
  // The empty sub_82106540(100, 30, 0, ..) the original calls first is left
  // out: it returns at once.
  // The frame kept for the steps' and Run's calls (its back chain); the
  // movie's key the original stored at its +80 is passed as a value, the
  // steps' table it built at +96 is kStepTable (E5 step 6: only this
  // function read them).
  const uint32_t frame = Frame(sp, 208);
  const uint32_t movie = LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, G::data().movie_shown()));
  const auto pages = G::pages();
  const uint32_t step = pages.step();
  uint64_t answer = movie;
  if (step == 0xFFFFFFFFu) {
    answer = calls.Call(kRun, frame, 0x82188030, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      // No bounds check: the word at the table's index (past the table, the
      // frame's word there, as the original read it). r4 to r10 as the
      // original leaves them (the table's build; r10 the table's address,
      // not read by the steps).
      const uint32_t target =
          step < 10 ? kStepTable[step] : LoadU32(frame + 96 + (step << 2));
      answer = calls.CallArgs(target, frame, 0x82187FC8,
                              {pages.step_started_address(), kStepTable[3], kStepTable[4], kStepTable[5],
                               kStepTable[6], kStepTable[7], frame + 96, step << 2});
      done = int32_t(answer) != 0;
    }
    if (done) {
      const auto p = G::pages();
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
  const uint32_t state = G::data().queued_state();
  if (state == 21) {
    return answer;
  }
  if (state == 9) {
    Post(mode, 42);
  }
  Machine(machine).set_state(G::data().queued_state());
  G::data().set_queued_state(21);
  if (G::status() != 5) {
    G::set_status(3);
  }
  return answer;
}

uint64_t OnEnter(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821869B8 Type2Dialog::OnEnter");
  // The original's 128-byte frame (only its back chain) left out: its
  // allocations are host objects now and no guest call is made from it (E5
  // step 6).
  (void)sp;
  (void)calls;
  const Machine m(machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x821869E4
  // and the VirtualAlloc(68) at 0x82186A20; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New();
  G::set_pages(pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  pages.set_step_started(0);
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
  // The data block lives in host memory (E5 step 3, the pilot): the
  // original's VirtualAlloc(68) at 0x82186A7C; the global holds its handle.
  const schema::Type2DialogData data = schema::Type2DialogData::New();
  const uint64_t r3 = data.address;
  G::set_data(data);
  data.set_movies_loaded(0);
  data.set_movie_shown(0);
  data.set_queued_state(21);
  G::set_showing(0);
  if (m.reenter() != 0) {
    m.set_state(1);
    G::set_status(0xFFFFFFFFu);
  } else {
    m.set_state(9);
    G::set_status(3);
  }
  return r3;
}

uint64_t WaitForMovies(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821880D0 Type2Dialog::WaitForMovies");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t machine = uint32_t(r3);
  const Machine m(machine);
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
  G::set_status(3);
  return r3;
}

void Shown() {
  ITC_NATIVE_RUNNING("sub_82186490 Type2Dialog::Shown");
  G::set_status(0);
}

void Wait(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82186AD8 Type2Dialog::Wait");
  Machine(machine).set_state(9);
  G::set_status(3);
}

uint64_t Cancel(uint64_t r3, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82186E40 Type2Dialog::Cancel");
  // Each dialog in turn: on its page, the page object to its close step
  // and its message posted; else, with it showing (its step running), the
  // message alone. (+8, the page before, is never written: the page is the
  // dialog's own.)
  static constexpr uint32_t kClose[4] = {1, 3, 5, 7};
  for (uint32_t k = 0; k < 4; k++) {
    const auto pages = G::pages();
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
    } else if (G::showing() != k) {
      continue;
    }
    Post(mode, message, k == 3);
    if (k != 3) {
      r3 = 0;
    }
  }
  G::set_status(2);
  return r3;
}

uint64_t OpenConfirm(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186C30 Type2Dialog::OpenConfirm");
  // The frame kept for ResetPose's call (its back chain); the movie's key
  // the original stored at its +80 passed as a value (E5 step 6).
  const uint32_t frame = Frame(sp, 112);
  G::data().set_movie_shown(16);
  const uint32_t movie = LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, G::data().movie_shown()));
  const uint64_t r3 = calls.Call(kResetPose, frame, 0x82186C80, movie);
  const uint32_t choice = LoadU32(movie + 40960);
  const auto menu = G::menu();
  const uint32_t cursor = menu.cursor();
  menu.set_cursor(choice);
  menu.set_previous(cursor);
  const auto pages = G::pages();
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  pages.set_step_started(0);
  G::set_showing(0);
  if (Machine(machine).loaded() != 0) {
    const auto p = G::pages();
    const uint32_t before = p.page();
    if (before != 0) {
      p.set_previous_page(before);
    }
    p.set_step(0);
    p.set_queued_step(0xFFFFFFFFu);
    p.set_next_page(0);
    p.set_page(0xFFFFFFFFu);
    Machine(machine).set_state(4);
  } else {
    Machine(machine).set_state(1);
  }
  G::set_status(5);
  return r3;
}

uint64_t OpenConfirm2(uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82186D40 Type2Dialog::OpenConfirm2");
  // The original's 112-byte frame left out: its one word (the movie's key
  // at +80) passed as a value, no guest call made from it (E5 step 6).
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
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  pages.set_step_started(0);
  G::set_showing(2);
  if (Machine(machine).loaded() != 0) {
    const auto p = G::pages();
    const uint32_t before = p.page();
    if (before != 2) {
      p.set_previous_page(before);
    }
    p.set_queued_step(0xFFFFFFFFu);
    p.set_next_page(2);
    p.set_page(0xFFFFFFFFu);
    p.set_step(4);
    Machine(machine).set_state(4);
  } else {
    Machine(machine).set_state(1);
  }
  G::set_status(5);
  return pair;
}

void OpenNotice(uint32_t machine, bool second) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82186B90 Type2Dialog::OpenNotice2");
  } else {
    ITC_NATIVE_RUNNING("sub_82186AF0 Type2Dialog::OpenNotice");
  }
  const uint32_t page = second ? 3 : 1;
  G::data().set_movie_shown(second ? 19 : 17);
  const auto pages = G::pages();
  pages.set_step_started(0);
  pages.set_next_page(page);
  pages.set_page(page);
  pages.set_previous_page(page);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  G::set_showing(page);
  if (Machine(machine).loaded() != 0) {
    const auto p = G::pages();
    const uint32_t before = p.page();
    if (before != page) {
      p.set_previous_page(before);
    }
    p.set_queued_step(0xFFFFFFFFu);
    p.set_next_page(page);
    p.set_page(0xFFFFFFFFu);
    p.set_step(second ? 6 : 2);
    Machine(machine).set_state(4);
  } else {
    Machine(machine).set_state(1);
  }
  G::set_status(5);
}

uint64_t OnLeft(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186428 Type2Dialog::OnLeft");
  // VirtualFree (sub_82295330) as its adapter answers it, called directly
  // (E5 step 6, as Session::Free's); through the dispatch, from the
  // original's frame (only its back chain), where the adapter would run the
  // original or the block is 0.
  auto free_block = [&](uint32_t block, uint32_t lr) {
    if (block != 0 && hooks::VirtualFree(block, 0, 0x8000, lr)) {
      return;
    }
    calls.Call(kVirtualFree, Frame(sp, 112), lr, block, 0, 0x8000);
  };
  free_block(G::pages().address, 0x82186450);
  free_block(G::menu().address, 0x82186464);
  // The host data block (OnEnter): the original's VirtualFree at
  // 0x82186478, which returned 1.
  G::data().Delete();
  const uint64_t r3 = 1;
  G::set_pages(0u);
  G::set_menu(0u);
  G::set_data(0u);
  return r3;
}

uint32_t TakesInput() {
  ITC_NATIVE_RUNNING("sub_821864A0 Type2Dialog::TakesInput");
  uint32_t status = G::status();
  if (status == 0) {
    status = 4;
    G::set_status(4);
  }
  return status == 1 ? 1 : 0;
}

}  // namespace type2_dialog
