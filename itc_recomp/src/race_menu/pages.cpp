// The race menu's pages Draw runs (docs/functions/race-menu.md): see
// race_menu.h.

#include "race_menu/race_menu.h"

#include <bit>

#include "base/log.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "race_menu/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_menu {

namespace {

constexpr uint64_t kSceneRoot = ~0ull;  // the sub-scene -1
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kResetScroll = 0x8211C5F8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kPlayNamed = 0x8211DAC8;

constexpr uint32_t kPressedOffset = menu_helpers::kPressedOffset;  // 10
constexpr uint32_t kFiredOffset = menu_helpers::kFiredOffset;      // 12

// Player 0's mapper entry and its slots asked as the originals do: nothing
// asked when the entry's +4 is 1; else the slot through the function table.
struct Mapper {
  uint32_t entry;
  uint32_t frame;
  const Calls& calls;
  uint64_t r3;

  bool Blocked() const { return int32_t(Entry(entry).unread()) == 1; }
  // The slot's answer (r3 kept); whether its low byte is set. False without
  // a call when blocked.
  bool Ask(uint32_t slot, uint64_t action, uint32_t lr) {
    if (Blocked()) {
      return false;
    }
    const uint32_t mapper = Entry(entry).mapper<schema::View>().address;
    r3 = calls.Call(Load(Load(mapper) + slot), frame, lr, mapper, action);
    return (r3 & 0xFF) != 0;
  }
};

uint32_t PlayerEntry() {
  return menu_helpers::kMapperEntries + (Load(menu_helpers::kControllerEntries) << 3);
}

}  // namespace

uint64_t AnimateKindPage(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821898C0 RaceMenu::AnimateKindPage");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t pair = FindPair(kMovie, frame + 80);
  const uint32_t m = Load(pair);
  Mapper mapper{entry, frame, calls, pair};
  auto finish = [&](uint32_t lr, uint64_t sub, uint64_t index) {
    return calls.Call(kFinish, frame, lr, m, sub, index);
  };
  auto play = [&](uint32_t lr, uint64_t sub, uint64_t index) {
    return calls.Call(kPlay, frame, lr, m, sub, index, 1);
  };
  // The wrap (mode 0 to 2 or 2 to 0): 12 and 13 finished, node (19, 0)'s
  // +16 the float of mode × 40 (its 64 bits stored at +80 for the convert).
  auto wrap = [&](const uint32_t (&lr)[4]) {
    calls.Call(kResetScroll, frame, lr[0], m, 1, 0);
    finish(lr[1], 19, 12);
    finish(lr[2], 19, 13);
    const uint32_t node = ui_movies::FindNode(m, 19, 0);
    const int64_t value = int32_t(Machine(machine).race_mode() * 40u);
    Store(frame + 80, uint32_t(uint64_t(value) >> 32));
    Store(frame + 84, uint32_t(value));
    Node(node).set_y(float(double(value)));
  };
  // The step's animation (12 or 13) finished and played.
  auto step = [&](const uint32_t (&lr)[2], uint64_t index) {
    calls.Call(kResetScroll, frame, lr[0], m, 1, 0);
    finish(lr[1], 19, index);
    play(0x82189BA0, 19, index);
  };
  const bool up = mapper.Ask(kFiredOffset, 153, 0x82189928) ||
                  mapper.Ask(kFiredOffset, 167, 0x82189954);
  if (up) {
    if (Machine(machine).race_mode() == 2 && Machine(machine).previous_race_mode() == 0) {
      wrap({0x82189B18, 0x82189B28, 0x82189B38, 0x82189B48});
    } else {
      step({0x82189B70, 0x82189B80}, 12);
    }
  } else if (mapper.Ask(kFiredOffset, 154, 0x82189980) ||
             mapper.Ask(kFiredOffset, 168, 0x821899AC)) {
    if (Machine(machine).race_mode() == 0 && Machine(machine).previous_race_mode() == 2) {
      wrap({0x82189A70, 0x82189A80, 0x82189A90, 0x82189AA0});
    } else {
      step({0x82189AD8, 0x82189AE8}, 13);
    }
  } else if (mapper.Ask(kPressedOffset, 159, 0x821899D8)) {
    calls.Call(kResetScroll, frame, 0x821899F0, m, 1, 0);
    finish(0x82189A04, 19, uint64_t(Machine(machine).race_mode()) + 2);
    return finish(0x82189A14, kSceneRoot, 0);
  } else {
    // B: Pressed(158)'s answer's low byte stored at +80, never read.
    if (!mapper.Blocked()) {
      mapper.Ask(kPressedOffset, 158, 0x82189A3C);
      Store(frame + 80, uint32_t(mapper.r3 & 0xFF));
    }
    return mapper.r3;
  }
  finish(0x82189BB0, 15, 1);
  play(0x82189BD0, 15, 1);
  finish(0x82189BE4, 19, uint64_t(Machine(machine).previous_race_mode()) + 2);
  play(0x82189C08, 19, uint64_t(Machine(machine).race_mode()) + 2);
  return calls.Call(kLoop, frame, 0x82189C1C, m, 19, uint64_t(Machine(machine).race_mode()) + 2);
}

uint64_t KindPage(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218AFD8 RaceMenu::KindPage");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  calls.Call(kMenuStep, frame, 0x8218B014, G::menu().address);
  Mapper mapper{entry, frame, calls, 0};
  // Up (-1) or down (+1): the mode stepped, wrapping within 0 to 2.
  auto move = [&](uint32_t lr, bool down) {
    calls.Call(kMenuSoundPlay, frame, lr, 4);
    const uint32_t mode = Machine(machine).race_mode();
    Machine(machine).set_refresh_texts(1);
    uint32_t next = down ? mode + 1 : mode - 1;
    Machine(machine).set_previous_race_mode(mode);
    Machine(machine).set_race_mode(next);
    if (down ? int32_t(next) > 2 : int32_t(next) < 0) {
      next = down ? 0 : 2;
      Machine(machine).set_race_mode(next);
    }
    Machine(machine).set_menu_mode(next);
  };
  if (mapper.Ask(kFiredOffset, 153, 0x8218B038) || mapper.Ask(kFiredOffset, 167, 0x8218B064)) {
    move(0x8218B27C, false);
  } else if (mapper.Ask(kFiredOffset, 154, 0x8218B090) ||
             mapper.Ask(kFiredOffset, 168, 0x8218B0BC)) {
    move(0x8218B24C, true);
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218B0E8)) {
    // A: to page 4 through transition 3, the page pushed on the history.
    calls.Call(kMenuSoundPlay, frame, 0x8218B0F8, 0);
    const uint32_t mode = Machine(machine).race_mode();
    Machine(machine).set_previous_visible_row(0);
    Machine(machine).set_visible_row(0);
    Machine(machine).set_previous_event_cursor(0);
    Machine(machine).set_event_cursor(0);
    Machine(machine).set_transitions_set_up(0);
    Machine(machine).set_unknown_9816(0);
    Machine(machine).set_kept_event_cursor(0);
    const uint32_t depth = Machine(machine).history_depth();
    Machine(machine).set_refresh_texts(1);
    const uint32_t page = Machine(machine).page();
    Machine(machine).set_page(4);
    Machine(machine).set_previous_race_mode(mode);
    Machine(machine).set_menu_mode(mode);
    Machine(machine).set_previous_page(page);
    Store(Machine(machine).page_history_address() + 4 * depth, 3);
    Machine(machine).set_history_depth(uint8_t(Machine(machine).history_depth() + 1));
    const uint32_t now = Machine(machine).page();
    const Pages pages = G::pages();
    const uint32_t before = pages.page();
    if (now != before) {
      pages.set_previous_page(before);
    }
    pages.set_step(3);
    pages.set_next_page(now);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218B1B4)) {
    // B: page 9 (leave), the page object reset, the fade out.
    calls.Call(kMenuSoundPlay, frame, 0x8218B1C4, 2);
    const uint32_t mode = Machine(machine).race_mode();
    Machine(machine).set_previous_race_mode(mode);
    Machine(machine).set_page(9);
    Machine(machine).set_menu_mode(mode);
    Pages pages = G::pages();
    pages.set_next_page(9);
    pages.set_page(9);
    pages.set_previous_page(9);
    pages.set_unknown_20(kNone);
    pages.set_step(kNone);
    pages.set_step_started(0);
    pages = G::pages();
    const uint32_t now = Machine(machine).page();
    const uint32_t before = pages.page();
    if (now != before) {
      pages.set_previous_page(before);
    }
    pages.set_page(now);
    pages.set_step(kNone);
    pages.set_queued_step(kNone);
    calls.Call(kFadeOut, frame, 0x8218B240, G::fade_out_passes(), 0, 0, 0);
  }
  return AnimateKindPage(machine, frame, calls);
}

uint64_t VariantPage(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218B750 RaceMenu::VariantPage");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  calls.Call(kMenuStep, frame, 0x8218B78C, G::menu().address);
  Mapper mapper{entry, frame, calls, 0};
  if (mapper.Ask(kFiredOffset, 153, 0x8218B7B0) || mapper.Ask(kFiredOffset, 167, 0x8218B7DC)) {
    // Up: back to the first variant.
    Machine(machine).set_refresh_texts(1);
    const uint32_t choice = Machine(machine).variant_cursor();
    Machine(machine).set_previous_variant_cursor(choice);
    if (choice != 0) {
      calls.Call(kMenuSoundPlay, frame, 0x8218B9C0, 4);
    }
    Machine(machine).set_variant_cursor(0);
    Machine(machine).set_variant(0);
  } else if (mapper.Ask(kFiredOffset, 154, 0x8218B808) ||
             mapper.Ask(kFiredOffset, 168, 0x8218B834)) {
    // Down: the second variant, when the event has one.
    const uint32_t cursor = Machine(machine).event_cursor();
    const uint32_t variant = EventKindAt(machine, cursor).variant_kind_address();
    const uint32_t choice = Machine(machine).variant_cursor();
    Machine(machine).set_refresh_texts(1);
    Machine(machine).set_previous_variant_cursor(choice);
    if (int32_t(Load(variant)) != -1) {
      if (int32_t(choice) != 1) {
        calls.Call(kMenuSoundPlay, frame, 0x8218B994, 4);
      }
      Machine(machine).set_variant_cursor(1);
    }
    Machine(machine).set_variant(Machine(machine).variant_cursor());
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218B860)) {
    // A: the question through transition 11, page 6.
    calls.Call(kMenuSoundPlay, frame, 0x8218B870, 0);
    const uint32_t choice = Machine(machine).variant_cursor();
    Machine(machine).set_unknown_9728(0);
    Machine(machine).set_unknown_9816(0);
    const uint32_t page = Machine(machine).page();
    Machine(machine).set_variant(choice);
    Machine(machine).set_refresh_texts(1);
    Machine(machine).set_unknown_9800(1);
    Machine(machine).set_page(6);
    Machine(machine).set_previous_page(page);
    const Pages pages = G::pages();
    const uint32_t before = pages.page();
    if (int32_t(before) != -1) {
      pages.set_previous_page(before);
    }
    pages.set_step(11);
    pages.set_next_page(kNone);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218B8F8)) {
    // B: back to page 4 through transition 3.
    calls.Call(kMenuSoundPlay, frame, 0x8218B908, 2);
    Machine(machine).set_variant(Machine(machine).variant_cursor());
    const uint32_t page = Machine(machine).page();
    Machine(machine).set_refresh_texts(1);
    Machine(machine).set_unknown_9816(0);
    Machine(machine).set_page(4);
    Machine(machine).set_previous_page(page);
    const Pages pages = G::pages();
    const uint32_t before = pages.page();
    if (int32_t(before) != 4) {
      pages.set_previous_page(before);
    }
    pages.set_next_page(4);
    pages.set_step(3);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
  }
  return AnimateVariantPage(machine, frame, calls);
}

uint64_t AnimateEventPage(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82189FE8 RaceMenu::AnimateEventPage");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t pair = FindPair(kMovie, frame + 80);
  const uint32_t m = Load(pair);
  Mapper mapper{entry, frame, calls, pair};
  uint64_t r3 = pair;
  auto finish = [&](uint32_t lr, uint64_t sub, uint64_t index) {
    r3 = calls.Call(kFinish, frame, lr, m, sub, index);
  };
  auto play = [&](uint32_t lr, uint64_t sub, uint64_t index, uint64_t chained) {
    r3 = calls.Call(kPlay, frame, lr, m, sub, index, chained);
  };
  auto loop = [&](uint32_t lr, uint64_t sub, uint64_t index) {
    r3 = calls.Call(kLoop, frame, lr, m, sub, index);
  };
  auto reset = [&](uint32_t lr) { r3 = calls.Call(kResetScroll, frame, lr, m, 1, 0); };
  // A row word + 2 (its zero-extended load, as the original adds).
  auto row = [&](uint32_t value) { return uint64_t(value) + 2; };
  // The list's ends (first_event, last_event) by their accessors.
  using End = uint32_t (Machine::*)() const;
  // The row's animations after a step: node 10's, then scene 20's (the old
  // row's finished and the new one's played and looped, or the list's
  // `end_finish`/`end_play` past its end), then the list's ends (both met:
  // `ends` twice) or the rows' step (`step`) when the visible row moved.
  struct Step {
    uint32_t lr[8];
  };
  auto move = [&](const Step& s, bool past, uint64_t end_finish, uint64_t end_play,
                  uint64_t ends, uint64_t step, End ends_first, End ends_last) {
    finish(s.lr[0], 10, 0);
    play(s.lr[1], 10, 0, 0);
    loop(s.lr[2], 10, 0);
    uint64_t looped;
    if (past) {
      finish(s.lr[3], 20, end_finish);
      play(s.lr[4], 20, end_play, 1);
      looped = end_play;
    } else {
      finish(s.lr[5], 20, row(Machine(machine).previous_visible_row()));
      play(s.lr[6], 20, row(Machine(machine).visible_row()), 1);
      looped = row(Machine(machine).visible_row());
    }
    loop(s.lr[7], 20, looped);
    const Machine mm(machine);
    if ((mm.*ends_first)() == mm.event_cursor() &&
        (mm.*ends_last)() == mm.previous_event_cursor()) {
      // Both ends met: (20, ends) finished and played twice.
      const uint32_t first_lr = ends == 12 ? 0x8218A35C : 0x8218A648;
      const uint32_t play_lr = ends == 12 ? 0x8218A37C : 0x8218A668;
      finish(first_lr, 20, ends);
      play(play_lr, 20, ends, 1);
      finish(ends == 12 ? 0x8218A6A0 : 0x8218A678, 20, ends);
      play(0x8218A6C0, 20, ends, 1);
    } else if (Machine(machine).visible_row() != Machine(machine).previous_visible_row()) {
      finish(step == 12 ? 0x8218A6A0 : 0x8218A678, 20, step);
      play(0x8218A6C0, 20, step, 1);
    }
    // The badges: by the old and new rows' pair words (+9856 + 8 x row).
    const uint32_t now = Machine(machine).event_cursor();
    const uint32_t old = Machine(machine).previous_event_cursor();
    if (old == now) {
      return;
    }
    const int32_t a = int32_t(EventAt(machine, old).badge());
    const int32_t b = int32_t(EventAt(machine, now).badge());
    struct Badge {
      int32_t a, b;
      uint64_t scene11, scene10;
      uint32_t lr[3];
    };
    static constexpr Badge kBadges[] = {
        {0, 1, 10, 5, {0x8218A3A4, 0x8218A3C4, 0x8218A3D4}},
        {0, 2, 11, 6, {0x8218A408, 0x8218A428, 0x8218A438}},
        {1, 0, 6, 1, {0x8218A450, 0x8218A470, 0x8218A480}},
        {1, 2, 9, 4, {0x8218A4B4, 0x8218A4D4, 0x8218A4E4}},
        {2, 0, 8, 3, {0x8218A518, 0x8218A538, 0x8218A548}},
        {2, 1, 7, 2, {0x8218A748, 0x8218A768, 0x8218A778}},
    };
    for (const Badge& badge : kBadges) {
      if (badge.a == a && badge.b == b) {
        finish(badge.lr[0], 11, badge.scene11);
        play(badge.lr[1], 11, badge.scene11, 1);
        finish(badge.lr[2], 10, badge.scene10);
        play(0x8218A798, 10, badge.scene10, 1);
        return;
      }
    }
  };
  const bool up = mapper.Ask(kFiredOffset, 153, 0x8218A050) ||
                  mapper.Ask(kFiredOffset, 167, 0x8218A07C);
  bool down = false;
  if (!up) {
    down = mapper.Ask(kFiredOffset, 154, 0x8218A0A8) || mapper.Ask(kFiredOffset, 168, 0x8218A0D4);
  }
  if (up) {
    move({{0x8218A560, 0x8218A580, 0x8218A590, 0x8218A5AC, 0x8218A5CC, 0x8218A5E0, 0x8218A604,
           0x8218A618}},
         int32_t(Machine(machine).event_cursor()) < 0, 2, 4, 13, 12, &Machine::last_event, &Machine::first_event);
  } else if (down) {
    move({{0x8218A274, 0x8218A294, 0x8218A2A4, 0x8218A2C0, 0x8218A2E0, 0x8218A2F4, 0x8218A318,
           0x8218A32C}},
         int32_t(Machine(machine).event_cursor()) > 14, 4, 2, 12, 13, &Machine::first_event, &Machine::last_event);
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218A100)) {
    // A: the scroll reset and the row's, the scene's and the badge's
    // animations finished (node 10's when no variant follows).
    if (int32_t(Machine(machine).variant_events()) > int32_t(Machine(machine).kept_event_cursor())) {
      reset(0x8218A128);
      finish(0x8218A13C, 20, row(Machine(machine).visible_row()));
      finish(0x8218A14C, kSceneRoot, 1);
      finish(0x8218A15C, 11, 12);
      finish(0x8218A1B0, kSceneRoot, 6);
    } else {
      reset(0x8218A16C);
      finish(0x8218A180, 20, row(Machine(machine).visible_row()));
      finish(0x8218A190, 10, 0);
      finish(0x8218A1A0, kSceneRoot, 1);
      finish(0x8218A1B0, 11, 12);
    }
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218A1D8)) {
    // B: the same finished, then the mode page's (-1, 3) played.
    reset(0x8218A1F0);
    finish(0x8218A204, 20, row(Machine(machine).visible_row()));
    finish(0x8218A214, 10, 0);
    finish(0x8218A224, kSceneRoot, 1);
    finish(0x8218A234, 11, 12);
    finish(0x8218A244, kSceneRoot, 6);
    finish(0x8218A254, kSceneRoot, 3);
    play(0x8218A798, kSceneRoot, 3, 0);
  } else {
    r3 = mapper.r3;  // the last slot's answer, or Find's (the pair's address)
  }
  // The list's scroll (+9836: 1 up, 2 down).
  const int32_t scroll = int32_t(Machine(machine).scroll_flag());
  if (scroll == 1 || scroll == 2) {
    const uint64_t named = scroll == 1 ? 148 : 149;
    calls.Call(kFinishNamed, frame, scroll == 1 ? 0x8218A7B0 : 0x8218A7CC, m, named);
    r3 = calls.Call(kPlayNamed, frame, 0x8218A7E8, m, named, 1);
  }
  return r3;
}

uint64_t EventPage(uint32_t machine, uint32_t mode, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218B2C8 RaceMenu::EventPage");
  const uint32_t frame = Frame(sp, 160);
  const uint32_t entry = PlayerEntry();
  calls.Call(kMenuStep, frame, 0x8218B300, G::menu().address);
  Mapper mapper{entry, frame, calls, 0};
  if (mapper.Ask(kFiredOffset, 153, 0x8218B324) || mapper.Ask(kFiredOffset, 167, 0x8218B350)) {
    // Up: the visible row and the cursor one up (the row's scroll 1 at the
    // top; the cursor wraps to the list's last, row 2).
    calls.Call(kMenuSoundPlay, frame, 0x8218B6E0, 4);
    const uint32_t row = Machine(machine).visible_row();
    Machine(machine).set_previous_visible_row(row);
    Machine(machine).set_visible_row(row - 1);
    if (int32_t(row - 1) < 0) {
      Machine(machine).set_scroll_flag(1);
      Machine(machine).set_visible_row(0);
    }
    const uint32_t cursor = Machine(machine).event_cursor();
    const uint32_t first = Machine(machine).first_event();
    Machine(machine).set_refresh_texts(1);
    Machine(machine).set_previous_event_cursor(cursor);
    Machine(machine).set_event_cursor(cursor - 1);
    if (int32_t(cursor - 1) < int32_t(first)) {
      const uint32_t last = Machine(machine).last_event();
      Machine(machine).set_visible_row(2);
      Machine(machine).set_event_cursor(last);
      Machine(machine).set_kept_event_cursor(last);
    } else {
      Machine(machine).set_kept_event_cursor(Machine(machine).event_cursor());
    }
  } else if (mapper.Ask(kFiredOffset, 154, 0x8218B37C) ||
             mapper.Ask(kFiredOffset, 168, 0x8218B3A8)) {
    // Down: the same the other way (scroll 2 at the bottom; the cursor
    // wraps to the list's first, row 0).
    calls.Call(kMenuSoundPlay, frame, 0x8218B670, 4);
    const uint32_t row = Machine(machine).visible_row();
    Machine(machine).set_previous_visible_row(row);
    Machine(machine).set_visible_row(row + 1);
    if (int32_t(row + 1) > 2) {
      Machine(machine).set_scroll_flag(2);
      Machine(machine).set_visible_row(2);
    }
    const uint32_t cursor = Machine(machine).event_cursor();
    const uint32_t last = Machine(machine).last_event();
    Machine(machine).set_previous_event_cursor(cursor);
    Machine(machine).set_event_cursor(cursor + 1);
    if (int32_t(cursor + 1) > int32_t(last)) {
      const uint32_t first = Machine(machine).first_event();
      Machine(machine).set_refresh_texts(1);
      Machine(machine).set_visible_row(0);
      Machine(machine).set_event_cursor(first);
      Machine(machine).set_kept_event_cursor(first);
    } else {
      Machine(machine).set_refresh_texts(1);
      Machine(machine).set_kept_event_cursor(cursor + 1);
    }
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218B3D4)) {
    // A: page 5 through transition 5 when the event has variants (+9848
    // above +9716, read before +9716 is written), else page 6 and the
    // question asked at once (as AskConfirm does); the page pushed on the
    // history.
    calls.Call(kMenuSoundPlay, frame, 0x8218B3E4, 0);
    const uint32_t count = Machine(machine).variant_events();
    const uint32_t at = Machine(machine).kept_event_cursor();
    const uint32_t page = Machine(machine).page();
    const uint32_t cursor = Machine(machine).event_cursor();
    Machine(machine).set_scroll_flag(0);
    Machine(machine).set_previous_page(page);
    Machine(machine).set_kept_event_cursor(cursor);
    if (int32_t(count) > int32_t(at)) {
      Machine(machine).set_page(5);
      const Pages pages = G::pages();
      const uint32_t before = pages.page();
      if (int32_t(before) != 5) {
        pages.set_previous_page(before);
      }
      pages.set_step(5);
      pages.set_next_page(5);
      pages.set_queued_step(kNone);
      pages.set_page(kNone);
      Machine(machine).set_unknown_9816(0);
      Machine(machine).set_variant(0);
      Machine(machine).set_transitions_set_up(0);
      Machine(machine).set_refresh_texts(1);
    } else {
      Machine(machine).set_page(6);
      Pages pages = G::pages();
      uint32_t before = pages.page();
      if (int32_t(before) != 6) {
        pages.set_previous_page(before);
      }
      pages.set_page(6);
      pages.set_step(kNone);
      pages.set_queued_step(kNone);
      Machine(machine).set_unknown_9816(0);
      Machine(machine).set_unknown_9728(0);
      Machine(machine).set_refresh_texts(1);
      Machine(machine).set_takes_input(0);
      const uint32_t m = FindMovie(16, frame + 80);
      // The question and the two answers (SetText's varargs the call sites
      // leave unset passed 0).
      calls.CallArgs(kSetText, frame, 0x8218B4CC, {m, kSceneRoot, 0, 10, 0, 0, 0, 0});
      calls.CallArgs(kSetText, frame, 0x8218B4E0, {m, 1, 3, 0, 0, 0, 0, 0});
      calls.CallArgs(kSetText, frame, 0x8218B4F4, {m, 1, 4, 1, 0, 0, 0, 0});
      const uint32_t save = vehicle_accessors::SaveDataOf(0);
      Store(m + 40960, Save(save).profile_option() != 0 ? 0 : 1);
      pages = G::pages();
      before = pages.page();
      if (int32_t(before) != 8) {
        pages.set_previous_page(before);
      }
      pages.set_step(kNone);
      pages.set_queued_step(kNone);
      pages.set_page(8);
      Post(mode, 34);
    }
    const uint32_t depth = Machine(machine).history_depth();
    Store(Machine(machine).page_history_address() + 4 * depth, 4);
    Machine(machine).set_history_depth(uint8_t(Machine(machine).history_depth() + 1));
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218B5D8)) {
    // B: back to page 3 through transition 0 (the page object written
    // twice, as the original does).
    calls.Call(kMenuSoundPlay, frame, 0x8218B5E8, 2);
    Machine(machine).set_kept_event_cursor(Machine(machine).event_cursor());
    const uint32_t page = Machine(machine).page();
    Machine(machine).set_refresh_texts(1);
    Machine(machine).set_scroll_flag(0);
    Machine(machine).set_page(3);
    Machine(machine).set_previous_page(page);
    for (int k = 0; k < 2; k++) {
      const Pages pages = G::pages();
      const uint32_t before = pages.page();
      if (int32_t(before) != 3) {
        pages.set_previous_page(before);
      }
      if (k == 0) {
        pages.set_step(0);
        pages.set_next_page(3);
        pages.set_queued_step(kNone);
        pages.set_page(kNone);
        Machine(machine).set_unknown_9816(0);
      } else {
        pages.set_step(0);
        pages.set_queued_step(kNone);
        pages.set_next_page(3);
        pages.set_page(kNone);
      }
    }
  }
  return AnimateEventPage(machine, frame, calls);
}

}  // namespace race_menu
