// The tuning screen machine's no answers and the notice's close, never seen
// in play (docs/functions/tuning-machine.md): see tuning_machine.h. The
// state history, the command queue and the category menu as
// TuningScreen::OnDialogYes's (tuning_pages/dialogs.cpp) leave them.

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/tuning_machine.h"
#include "tuning_pages/tuning_pages.h"
#include "ui_movies/game.h"

namespace tuning_machine {

namespace {

using Screen = schema::TuningMachineScreen;
using Queue_ = schema::PageObject;
using Menu = schema::FrontEndMenu;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kHideNode = 0x8211AC48;  // UiMovie::HideNode

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
int32_t S(uint32_t value) { return int32_t(value); }

// One state popped off the history (its depth byte down one, wrapping); the
// state it gives into +9652. Returns it.
uint32_t PopOne(uint8_t* base, uint32_t s) {
  const Screen sc(base, s);
  const uint32_t depth = (sc.history_depth() + 255) & 0xFF;
  sc.set_history_depth(uint8_t(depth));
  const uint32_t state = Load(base, sc.state_history_address() + depth * 4);
  sc.set_screen_state(state);
  return state;
}
// Popped until the state is `target` (no bound, as the original).
uint32_t PopTo(uint8_t* base, uint32_t s, uint32_t target) {
  for (;;) {
    if (S(PopOne(base, s)) == S(target)) {
      return target;
    }
  }
}

// The queue reset to `state` (+0, +4, +8 = it, +12 = +20 = -1, +24 = 0).
void Queue(uint8_t* base, uint32_t s, uint32_t state) {
  const Queue_ q = Screen(base, s).queue();
  q.set_unknown_20(kNone);
  q.set_step(kNone);
  q.set_step_started(0);
  q.set_next_page(state);
  q.set_page(state);
  q.set_previous_page(state);
}

// The queue's `command` then `next` for the state +9652: +8 = the head
// where it differs, +12, +16, +4 = the state, +0 = -1.
void Then(uint8_t* base, uint32_t s, uint32_t command, uint32_t next) {
  const Screen sc(base, s);
  const Queue_ q = sc.queue();
  const uint32_t state = sc.screen_state();
  const uint32_t head = q.page();
  if (S(state) != S(head)) {
    q.set_previous_page(head);
  }
  q.set_step(command);
  q.set_queued_step(next);
  q.set_next_page(state);
  q.set_page(kNone);
}

// The category menu reset from the page's item count and cursor
// (PageItemCount and PageCursor, their plain C++); with `clear_rows` its
// +32, +40, +48 and +52 cleared too (the notice's aero reset). Returns the
// cursor (PageCursor's r3).
uint32_t ResetMenu(uint8_t* base, uint32_t s, bool clear_rows = false) {
  const uint32_t count = tuning_pages::PageItemCount(base, s);
  const uint32_t cursor = tuning_pages::PageCursor(base, s);
  const Menu m = Screen(base, s).category_menu();
  m.set_first_item(0);
  m.set_moved(0);
  m.set_list_wrapped(0);
  m.set_first_row(0);
  m.set_side(0);
  m.set_player(0);
  m.set_up_down_locked(0);
  m.set_left_right_steps(0);
  m.set_last_item(count - 1);
  m.set_wraps(1);
  m.set_cursor(cursor);
  m.set_previous(cursor);
  m.set_rows_shown(8);
  if (clear_rows) {
    m.set_shoulders_step(0);
    m.set_wrapped(0);
    m.set_visible_row(0);
    m.set_previous_row(0);
  }
  return cursor;
}

// The menu's rows from the screen's +25100 and +25104 (the aero list).
void MenuRows(uint8_t* base, uint32_t s) {
  const Screen sc(base, s);
  sc.category_menu().set_visible_row(sc.list_row());
  sc.category_menu().set_previous_row(sc.list_previous_row());
}

uint32_t Category(uint8_t* base, uint32_t s) { return Screen(base, s).category(); }
uint32_t Item(uint8_t* base, uint32_t s) { return Screen(base, s).item(); }
// Category 5's item 0 and category 6's item 3 keep their own pages (no
// menu reset).
bool OwnPage(uint8_t* base, uint32_t s) {
  return (S(Category(base, s)) == 5 && Item(base, s) == 0) ||
         (S(Category(base, s)) == 6 && Item(base, s) == 3);
}

// The page the money short notice or the equip question goes back to: 3
// (category 2), 15 (category 5, item 0; with `mark_15` +25240 = 1), 53 or 56
// (category 6, item 3, by [+33540] 0 or 1), else 4.
uint32_t PopToPage(uint8_t* base, uint32_t s, bool mark_15) {
  const Screen sc(base, s);
  const int32_t category = S(Category(base, s));
  if (category == 2) {
    return PopTo(base, s, 3);
  }
  if (category == 5 && Item(base, s) == 0) {
    if (mark_15) {
      sc.set_wheel_back_pressed(1);
    }
    return PopTo(base, s, 15);
  }
  if (category == 6 && Item(base, s) == 3) {
    const int32_t sticker = S(sc.interior_mode());
    if (sticker == 0) {
      return PopTo(base, s, 53);
    }
    if (sticker == 1) {
      return PopTo(base, s, 56);
    }
  }
  return PopTo(base, s, 4);
}

// The page set-up command after 64 for the state reached; state 56 runs 57
// at once (+9716 and +9720 cleared).
void PageCommand(uint8_t* base, uint32_t s, bool has_57) {
  const Screen sc(base, s);
  switch (sc.screen_state()) {
    case 3: Then(base, s, 64, 0); break;
    case 4: Then(base, s, 64, 2); break;
    case 15: Then(base, s, 64, 7); break;
    case 53: Then(base, s, 64, 54); break;
    case 56:
      sc.set_command_first_run(0);
      sc.set_command_end_after_setup(0);
      Then(base, s, 57, kNone);
      break;
    case 57:
      if (has_57) {
        Then(base, s, 64, 58);
      }
      break;
    default: break;
  }
}

}  // namespace

uint64_t StickerDialogNo(uint8_t* base, uint64_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821F7988 TuningMachine::StickerDialogNo");
  const uint32_t s = uint32_t(screen);
  const Screen sc(base, s);
  const uint32_t frame = sp - 96;
  const int32_t state = S(sc.screen_state());
  if (state == 52) {
    sc.set_screen_state(40);
    Queue(base, s, 40);
    Then(base, s, 64, 36);
    return screen;
  }
  if (state != 50 && state != 51) {
    return screen;
  }
  // Back to the sticker editor's page (39): the queue's +0 = 39 (its head
  // already), +12 = +16 = -1; node (1, 1) of movie 14 hidden.
  sc.set_screen_state(39);
  Queue(base, s, 39);
  const Queue_ q = sc.queue();
  const uint32_t head = q.page();
  if (S(head) != 39) {
    q.set_previous_page(head);
  }
  q.set_page(39);
  q.set_step(kNone);
  q.set_queued_step(kNone);
  // UiMovies::Find by the id as a word (the original's key at the frame's
  // +80, read only by Find); the frame's back chain written for the guest
  // call alone (E5 step 6).
  const uint32_t movie = Load(base, ui_movies::PairsFindKey(base, ui_movies::kMovieTable, 14));
  Store(base, frame, sp);
  return calls.Call(kHideNode, frame, 0x821F7A88, movie, 1, 1);
}

uint64_t OnDialogNo(uint8_t* base, uint64_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821CEA88 TuningMachine::OnDialogNo");
  const uint32_t s = uint32_t(screen);
  const Screen sc(base, s);
  // The original's frame: nothing in it, no guest call made with it (E5
  // step 6: the back chain no longer written); StickerDialogNo's below it.
  const uint32_t frame = sp - 96;
  const int32_t state = S(sc.screen_state());
  auto pop_and_queue = [&] {
    const uint32_t reached = PopOne(base, s);
    Queue(base, s, reached);
  };
  if (state == 9) {
    // CONF BUY: one state back, the category menu again.
    const uint32_t first = sc.category_menu().cursor();
    sc.set_confirm_cursor(0);
    sc.set_command_end_now(1);
    sc.set_refresh(1);
    sc.set_unknown_9748(first);
    pop_and_queue();
    if (S(tuning_pages::PageItemCount(base, s)) > 8) {
      ResetMenu(base, s);
    } else if (!OwnPage(base, s)) {
      ResetMenu(base, s);
      if (S(Category(base, s)) == 4 && Item(base, s) == 4) {
        sc.set_bonnet_color_set(1);
      }
    }
    static constexpr struct {
      int32_t state;
      uint32_t command;
    } kCommands[] = {{5, 3}, {6, 4}, {7, 5}, {8, 6}, {17, 9}, {55, 56}, {56, 57}};
    for (const auto& entry : kCommands) {
      if (S(sc.screen_state()) == entry.state) {
        Then(base, s, entry.command, kNone);
        break;
      }
    }
  } else if (state == 11) {
    // CONF EQUIP: back to the category's page.
    sc.set_rewrite_car(1);
    PopToPage(base, s, true);
    sc.set_refresh(1);
    sc.set_command_first_run(1);
    Queue(base, s, sc.screen_state());
    if (S(Category(base, s)) == 4 || !OwnPage(base, s)) {
      ResetMenu(base, s);
    }
    if (S(Category(base, s)) == 4) {
      MenuRows(base, s);
    }
    PageCommand(base, s, true);
  } else if (state == 12 || state == 13) {
    // The light kits' questions: one state back, then command 10, 13 or 14
    // (state 12) or 64 with it after (state 13).
    if (S(Category(base, s)) == 5) {
      uint32_t next = 0;
      bool hit = true;
      const uint32_t item = Item(base, s);
      if (item == 4) {
        sc.set_rewrite_car(1);
        next = 10;
      } else if (item == 5 && S(sc.light_kit_kind()) == 0) {
        next = 13;
      } else if (item == 5 && S(sc.light_kit_kind()) == 1) {
        next = 14;
      } else {
        hit = false;
      }
      if (hit) {
        sc.set_refresh(1);
        sc.set_command_end_now(1);
        pop_and_queue();
        if (state == 12) {
          Then(base, s, next, kNone);
        } else {
          Then(base, s, 64, next);
        }
      }
    }
  } else if (state == 14) {
    sc.set_refresh(1);
    sc.set_command_first_run(1);
    const int32_t category = S(Category(base, s));
    const uint32_t item = Item(base, s);
    if (category == 4 && ((item == 3 && sc.type() == 1) ||
                          (item == 9 && sc.type() == 0))) {
      sc.set_rewrite_car(1);
      PopTo(base, s, 4);
      Queue(base, s, 4);
      ResetMenu(base, s);
      MenuRows(base, s);
      Then(base, s, 64, 2);
    } else if (category == 5 && item == 5) {
      PopTo(base, s, 19);
      Queue(base, s, 19);
      Then(base, s, 64, 11);
    } else if (category == 6 && item == 3) {
      const int32_t sticker = S(sc.interior_mode());
      if (sticker == 0 || sticker == 1) {
        sc.set_rewrite_car(1);
        const uint32_t target = sticker == 0 ? 53 : 56;
        PopTo(base, s, target);
        Queue(base, s, target);
        Then(base, s, 64, sticker == 0 ? 54 : 57);
      }
    }
  } else if (state == 35 || state == 36) {
    sc.set_command_first_run(0);
    sc.set_refresh(1);
    PopTo(base, s, 24);
    Queue(base, s, 24);
    if (S(Category(base, s)) == 5 && Item(base, s) == 6) {
      Then(base, s, 16, kNone);
    }
  } else if (state == 37) {
    if (S(Category(base, s)) == 5 && (Item(base, s) == 6 || Item(base, s) == 7)) {
      PopTo(base, s, 25);
      Queue(base, s, 25);
      Then(base, s, 64, 17);
    }
  }
  return StickerDialogNo(base, screen, frame, calls);
}

uint64_t OnNoticeClosed(uint8_t* base, uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CF8B8 TuningMachine::OnNoticeClosed");
  const uint32_t s = uint32_t(screen);
  const Screen sc(base, s);
  (void)sp;  // no frame (E5 step 6: the original's held nothing)
  if (S(sc.screen_state()) != 10) {
    return screen;
  }
  // The money short: back to the category's page.
  sc.set_rewrite_car(1);
  sc.set_refresh(1);
  sc.set_command_first_run(1);
  uint64_t r3 = screen;
  if (S(Category(base, s)) == 2) {
    PopTo(base, s, 3);
    Queue(base, s, 3);
    r3 = ResetMenu(base, s);
  } else {
    PopToPage(base, s, false);
    Queue(base, s, sc.screen_state());
    if (S(Category(base, s)) == 4) {
      r3 = ResetMenu(base, s, true);
      MenuRows(base, s);
    } else if (!OwnPage(base, s)) {
      r3 = ResetMenu(base, s);
    }
  }
  PageCommand(base, s, false);
  return r3;
}

}  // namespace tuning_machine
