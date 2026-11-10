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
constexpr uint32_t kFadeOutArgument = 0x82001BF0;

constexpr uint32_t kPressedOffset = menu_helpers::kPressedOffset;  // 10
constexpr uint32_t kFiredOffset = menu_helpers::kFiredOffset;      // 12

// Player 0's mapper entry and its slots asked as the originals do: nothing
// asked when the entry's +4 is 1; else the slot through the function table.
struct Mapper {
  uint8_t* base;
  uint32_t entry;
  uint32_t frame;
  const Calls& calls;
  uint64_t r3;

  bool Blocked() const { return int32_t(Load(base, entry + 4)) == 1; }
  // The slot's answer (r3 kept); whether its low byte is set. False without
  // a call when blocked.
  bool Ask(uint32_t slot, uint64_t action, uint32_t lr) {
    if (Blocked()) {
      return false;
    }
    const uint32_t mapper = Load(base, entry + 0);
    r3 = calls.Call(Load(base, Load(base, mapper) + slot), frame, lr, mapper, action);
    return (r3 & 0xFF) != 0;
  }
};

uint32_t PlayerEntry(uint8_t* base) {
  return menu_helpers::kMapperEntries + (Load(base, menu_helpers::kControllerEntries) << 3);
}

}  // namespace

uint64_t AnimateKindPage(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821898C0 RaceMenu::AnimateKindPage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t pair = FindPair(base, kMovie, frame + 80);
  const uint32_t m = Load(base, pair);
  Mapper mapper{base, entry, frame, calls, pair};
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
    const uint32_t node = ui_movies::FindNode(base, m, 19, 0);
    const int64_t value = int32_t(Load(base, machine + 9820) * 40u);
    Store(base, frame + 80, uint32_t(uint64_t(value) >> 32));
    Store(base, frame + 84, uint32_t(value));
    Store(base, node + 16, std::bit_cast<uint32_t>(float(double(value))));
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
    if (Load(base, machine + 9820) == 2 && Load(base, machine + 9824) == 0) {
      wrap({0x82189B18, 0x82189B28, 0x82189B38, 0x82189B48});
    } else {
      step({0x82189B70, 0x82189B80}, 12);
    }
  } else if (mapper.Ask(kFiredOffset, 154, 0x82189980) ||
             mapper.Ask(kFiredOffset, 168, 0x821899AC)) {
    if (Load(base, machine + 9820) == 0 && Load(base, machine + 9824) == 2) {
      wrap({0x82189A70, 0x82189A80, 0x82189A90, 0x82189AA0});
    } else {
      step({0x82189AD8, 0x82189AE8}, 13);
    }
  } else if (mapper.Ask(kPressedOffset, 159, 0x821899D8)) {
    calls.Call(kResetScroll, frame, 0x821899F0, m, 1, 0);
    finish(0x82189A04, 19, uint64_t(Load(base, machine + 9820)) + 2);
    return finish(0x82189A14, kSceneRoot, 0);
  } else {
    // B: Pressed(158)'s answer's low byte stored at +80, never read.
    if (!mapper.Blocked()) {
      mapper.Ask(kPressedOffset, 158, 0x82189A3C);
      Store(base, frame + 80, uint32_t(mapper.r3 & 0xFF));
    }
    return mapper.r3;
  }
  finish(0x82189BB0, 15, 1);
  play(0x82189BD0, 15, 1);
  finish(0x82189BE4, 19, uint64_t(Load(base, machine + 9824)) + 2);
  play(0x82189C08, 19, uint64_t(Load(base, machine + 9820)) + 2);
  return calls.Call(kLoop, frame, 0x82189C1C, m, 19, uint64_t(Load(base, machine + 9820)) + 2);
}

uint64_t KindPage(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218AFD8 RaceMenu::KindPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  calls.Call(kMenuStep, frame, 0x8218B014, Load(base, kMenu));
  Mapper mapper{base, entry, frame, calls, 0};
  // Up (-1) or down (+1): the mode stepped, wrapping within 0 to 2.
  auto move = [&](uint32_t lr, bool down) {
    calls.Call(kMenuSoundPlay, frame, lr, 4);
    const uint32_t mode = Load(base, machine + 9820);
    Store(base, machine + 9780, 1);
    uint32_t next = down ? mode + 1 : mode - 1;
    Store(base, machine + 9824, mode);
    Store(base, machine + 9820, next);
    if (down ? int32_t(next) > 2 : int32_t(next) < 0) {
      next = down ? 0 : 2;
      Store(base, machine + 9820, next);
    }
    Store(base, machine + 9648, next);
  };
  if (mapper.Ask(kFiredOffset, 153, 0x8218B038) || mapper.Ask(kFiredOffset, 167, 0x8218B064)) {
    move(0x8218B27C, false);
  } else if (mapper.Ask(kFiredOffset, 154, 0x8218B090) ||
             mapper.Ask(kFiredOffset, 168, 0x8218B0BC)) {
    move(0x8218B24C, true);
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218B0E8)) {
    // A: to page 4 through transition 3, the page pushed on the history.
    calls.Call(kMenuSoundPlay, frame, 0x8218B0F8, 0);
    const uint32_t mode = Load(base, machine + 9820);
    Store(base, machine + 9976, 0);
    Store(base, machine + 9972, 0);
    Store(base, machine + 9832, 0);
    Store(base, machine + 9828, 0);
    Store(base, machine + 9812, 0);
    guest::StoreU16(base, machine + 9816, 0);
    Store(base, machine + 9716, 0);
    const uint32_t depth = guest::LoadU8(base, machine + 9776);
    Store(base, machine + 9780, 1);
    const uint32_t page = Load(base, machine + kPage);
    Store(base, machine + kPage, 4);
    Store(base, machine + 9824, mode);
    Store(base, machine + 9648, mode);
    Store(base, machine + 9644, page);
    Store(base, machine + ((depth + 2433) << 2), 3);
    guest::StoreU8(base, machine + 9776, uint8_t(guest::LoadU8(base, machine + 9776) + 1));
    const uint32_t now = Load(base, machine + kPage);
    const uint32_t pages = Load(base, kPages);
    const uint32_t before = Load(base, pages + 0);
    if (now != before) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 12, 3);
    Store(base, pages + 4, now);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218B1B4)) {
    // B: page 9 (leave), the page object reset, the fade out.
    calls.Call(kMenuSoundPlay, frame, 0x8218B1C4, 2);
    const uint32_t mode = Load(base, machine + 9820);
    Store(base, machine + 9824, mode);
    Store(base, machine + kPage, 9);
    Store(base, machine + 9648, mode);
    uint32_t pages = Load(base, kPages);
    Store(base, pages + 4, 9);
    Store(base, pages + 0, 9);
    Store(base, pages + 8, 9);
    Store(base, pages + 20, kNone);
    Store(base, pages + 12, kNone);
    Store(base, pages + 24, 0);
    pages = Load(base, kPages);
    const uint32_t now = Load(base, machine + kPage);
    const uint32_t before = Load(base, pages + 0);
    if (now != before) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 0, now);
    Store(base, pages + 12, kNone);
    Store(base, pages + 16, kNone);
    calls.Call(kFadeOut, frame, 0x8218B240, Load(base, kFadeOutArgument), 0, 0, 0);
  }
  return AnimateKindPage(base, machine, frame, calls);
}

uint64_t VariantPage(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218B750 RaceMenu::VariantPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  calls.Call(kMenuStep, frame, 0x8218B78C, Load(base, kMenu));
  Mapper mapper{base, entry, frame, calls, 0};
  if (mapper.Ask(kFiredOffset, 153, 0x8218B7B0) || mapper.Ask(kFiredOffset, 167, 0x8218B7DC)) {
    // Up: back to the first variant.
    Store(base, machine + 9780, 1);
    const uint32_t choice = Load(base, machine + 10100);
    Store(base, machine + 10104, choice);
    if (choice != 0) {
      calls.Call(kMenuSoundPlay, frame, 0x8218B9C0, 4);
    }
    Store(base, machine + 10100, 0);
    Store(base, machine + 9724, 0);
  } else if (mapper.Ask(kFiredOffset, 154, 0x8218B808) ||
             mapper.Ask(kFiredOffset, 168, 0x8218B834)) {
    // Down: the second variant, when the event has one.
    const uint32_t cursor = Load(base, machine + 9828);
    const uint32_t variant = machine + ((cursor + 1248) << 3);
    const uint32_t choice = Load(base, machine + 10100);
    Store(base, machine + 9780, 1);
    Store(base, machine + 10104, choice);
    if (int32_t(Load(base, variant)) != -1) {
      if (int32_t(choice) != 1) {
        calls.Call(kMenuSoundPlay, frame, 0x8218B994, 4);
      }
      Store(base, machine + 10100, 1);
    }
    Store(base, machine + 9724, Load(base, machine + 10100));
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218B860)) {
    // A: the question through transition 11, page 6.
    calls.Call(kMenuSoundPlay, frame, 0x8218B870, 0);
    const uint32_t choice = Load(base, machine + 10100);
    Store(base, machine + 9728, 0);
    guest::StoreU16(base, machine + 9816, 0);
    const uint32_t page = Load(base, machine + kPage);
    Store(base, machine + 9724, choice);
    Store(base, machine + 9780, 1);
    Store(base, machine + 9800, 1);
    Store(base, machine + kPage, 6);
    Store(base, machine + 9644, page);
    const uint32_t pages = Load(base, kPages);
    const uint32_t before = Load(base, pages + 0);
    if (int32_t(before) != -1) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 12, 11);
    Store(base, pages + 4, kNone);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218B8F8)) {
    // B: back to page 4 through transition 3.
    calls.Call(kMenuSoundPlay, frame, 0x8218B908, 2);
    Store(base, machine + 9724, Load(base, machine + 10100));
    const uint32_t page = Load(base, machine + kPage);
    Store(base, machine + 9780, 1);
    guest::StoreU16(base, machine + 9816, 0);
    Store(base, machine + kPage, 4);
    Store(base, machine + 9644, page);
    const uint32_t pages = Load(base, kPages);
    const uint32_t before = Load(base, pages + 0);
    if (int32_t(before) != 4) {
      Store(base, pages + 8, before);
    }
    Store(base, pages + 4, 4);
    Store(base, pages + 12, 3);
    Store(base, pages + 16, kNone);
    Store(base, pages + 0, kNone);
  }
  return AnimateVariantPage(base, machine, frame, calls);
}

uint64_t AnimateEventPage(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82189FE8 RaceMenu::AnimateEventPage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t pair = FindPair(base, kMovie, frame + 80);
  const uint32_t m = Load(base, pair);
  Mapper mapper{base, entry, frame, calls, pair};
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
  auto row = [&](uint32_t offset) { return uint64_t(Load(base, machine + offset)) + 2; };
  // The row's animations after a step: node 10's, then scene 20's (the old
  // row's finished and the new one's played and looped, or the list's
  // `end_finish`/`end_play` past its end), then the list's ends (both met:
  // `ends` twice) or the rows' step (`step`) when the visible row moved.
  struct Step {
    uint32_t lr[8];
  };
  auto move = [&](const Step& s, bool past, uint64_t end_finish, uint64_t end_play,
                  uint64_t ends, uint64_t step, uint32_t ends_first, uint32_t ends_last) {
    finish(s.lr[0], 10, 0);
    play(s.lr[1], 10, 0, 0);
    loop(s.lr[2], 10, 0);
    uint64_t looped;
    if (past) {
      finish(s.lr[3], 20, end_finish);
      play(s.lr[4], 20, end_play, 1);
      looped = end_play;
    } else {
      finish(s.lr[5], 20, row(9976));
      play(s.lr[6], 20, row(9972), 1);
      looped = row(9972);
    }
    loop(s.lr[7], 20, looped);
    if (Load(base, machine + ends_first) == Load(base, machine + 9828) &&
        Load(base, machine + ends_last) == Load(base, machine + 9832)) {
      // Both ends met: (20, ends) finished and played twice.
      const uint32_t first_lr = ends == 12 ? 0x8218A35C : 0x8218A648;
      const uint32_t play_lr = ends == 12 ? 0x8218A37C : 0x8218A668;
      finish(first_lr, 20, ends);
      play(play_lr, 20, ends, 1);
      finish(ends == 12 ? 0x8218A6A0 : 0x8218A678, 20, ends);
      play(0x8218A6C0, 20, ends, 1);
    } else if (Load(base, machine + 9972) != Load(base, machine + 9976)) {
      finish(step == 12 ? 0x8218A6A0 : 0x8218A678, 20, step);
      play(0x8218A6C0, 20, step, 1);
    }
    // The badges: by the old and new rows' pair words (+9856 + 8 x row).
    const uint32_t now = Load(base, machine + 9828);
    const uint32_t old = Load(base, machine + 9832);
    if (old == now) {
      return;
    }
    const int32_t a = int32_t(Load(base, machine + ((old + 1232) << 3)));
    const int32_t b = int32_t(Load(base, machine + ((now + 1232) << 3)));
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
         int32_t(Load(base, machine + 9828)) < 0, 2, 4, 13, 12, 9844, 9840);
  } else if (down) {
    move({{0x8218A274, 0x8218A294, 0x8218A2A4, 0x8218A2C0, 0x8218A2E0, 0x8218A2F4, 0x8218A318,
           0x8218A32C}},
         int32_t(Load(base, machine + 9828)) > 14, 4, 2, 12, 13, 9840, 9844);
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218A100)) {
    // A: the scroll reset and the row's, the scene's and the badge's
    // animations finished (node 10's when no variant follows).
    if (int32_t(Load(base, machine + 9848)) > int32_t(Load(base, machine + 9716))) {
      reset(0x8218A128);
      finish(0x8218A13C, 20, row(9972));
      finish(0x8218A14C, kSceneRoot, 1);
      finish(0x8218A15C, 11, 12);
      finish(0x8218A1B0, kSceneRoot, 6);
    } else {
      reset(0x8218A16C);
      finish(0x8218A180, 20, row(9972));
      finish(0x8218A190, 10, 0);
      finish(0x8218A1A0, kSceneRoot, 1);
      finish(0x8218A1B0, 11, 12);
    }
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218A1D8)) {
    // B: the same finished, then the mode page's (-1, 3) played.
    reset(0x8218A1F0);
    finish(0x8218A204, 20, row(9972));
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
  const int32_t scroll = int32_t(Load(base, machine + 9836));
  if (scroll == 1 || scroll == 2) {
    const uint64_t named = scroll == 1 ? 148 : 149;
    calls.Call(kFinishNamed, frame, scroll == 1 ? 0x8218A7B0 : 0x8218A7CC, m, named);
    r3 = calls.Call(kPlayNamed, frame, 0x8218A7E8, m, named, 1);
  }
  return r3;
}

uint64_t EventPage(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218B2C8 RaceMenu::EventPage");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t entry = PlayerEntry(base);
  calls.Call(kMenuStep, frame, 0x8218B300, Load(base, kMenu));
  Mapper mapper{base, entry, frame, calls, 0};
  if (mapper.Ask(kFiredOffset, 153, 0x8218B324) || mapper.Ask(kFiredOffset, 167, 0x8218B350)) {
    // Up: the visible row and the cursor one up (the row's scroll 1 at the
    // top; the cursor wraps to the list's last, row 2).
    calls.Call(kMenuSoundPlay, frame, 0x8218B6E0, 4);
    const uint32_t row = Load(base, machine + 9972);
    Store(base, machine + 9976, row);
    Store(base, machine + 9972, row - 1);
    if (int32_t(row - 1) < 0) {
      Store(base, machine + 9836, 1);
      Store(base, machine + 9972, 0);
    }
    const uint32_t cursor = Load(base, machine + 9828);
    const uint32_t first = Load(base, machine + 9840);
    Store(base, machine + 9780, 1);
    Store(base, machine + 9832, cursor);
    Store(base, machine + 9828, cursor - 1);
    if (int32_t(cursor - 1) < int32_t(first)) {
      const uint32_t last = Load(base, machine + 9844);
      Store(base, machine + 9972, 2);
      Store(base, machine + 9828, last);
      Store(base, machine + 9716, last);
    } else {
      Store(base, machine + 9716, Load(base, machine + 9828));
    }
  } else if (mapper.Ask(kFiredOffset, 154, 0x8218B37C) ||
             mapper.Ask(kFiredOffset, 168, 0x8218B3A8)) {
    // Down: the same the other way (scroll 2 at the bottom; the cursor
    // wraps to the list's first, row 0).
    calls.Call(kMenuSoundPlay, frame, 0x8218B670, 4);
    const uint32_t row = Load(base, machine + 9972);
    Store(base, machine + 9976, row);
    Store(base, machine + 9972, row + 1);
    if (int32_t(row + 1) > 2) {
      Store(base, machine + 9836, 2);
      Store(base, machine + 9972, 2);
    }
    const uint32_t cursor = Load(base, machine + 9828);
    const uint32_t last = Load(base, machine + 9844);
    Store(base, machine + 9832, cursor);
    Store(base, machine + 9828, cursor + 1);
    if (int32_t(cursor + 1) > int32_t(last)) {
      const uint32_t first = Load(base, machine + 9840);
      Store(base, machine + 9780, 1);
      Store(base, machine + 9972, 0);
      Store(base, machine + 9828, first);
      Store(base, machine + 9716, first);
    } else {
      Store(base, machine + 9780, 1);
      Store(base, machine + 9716, cursor + 1);
    }
  } else if (mapper.Ask(kPressedOffset, 159, 0x8218B3D4)) {
    // A: page 5 through transition 5 when the event has variants (+9848
    // above +9716, read before +9716 is written), else page 6 and the
    // question asked at once (as AskConfirm does); the page pushed on the
    // history.
    calls.Call(kMenuSoundPlay, frame, 0x8218B3E4, 0);
    const uint32_t count = Load(base, machine + 9848);
    const uint32_t at = Load(base, machine + 9716);
    const uint32_t page = Load(base, machine + kPage);
    const uint32_t cursor = Load(base, machine + 9828);
    Store(base, machine + 9836, 0);
    Store(base, machine + 9644, page);
    Store(base, machine + 9716, cursor);
    if (int32_t(count) > int32_t(at)) {
      Store(base, machine + kPage, 5);
      const uint32_t pages = Load(base, kPages);
      const uint32_t before = Load(base, pages + 0);
      if (int32_t(before) != 5) {
        Store(base, pages + 8, before);
      }
      Store(base, pages + 12, 5);
      Store(base, pages + 4, 5);
      Store(base, pages + 16, kNone);
      Store(base, pages + 0, kNone);
      guest::StoreU16(base, machine + 9816, 0);
      Store(base, machine + 9724, 0);
      Store(base, machine + 9812, 0);
      Store(base, machine + 9780, 1);
    } else {
      Store(base, machine + kPage, 6);
      uint32_t pages = Load(base, kPages);
      uint32_t before = Load(base, pages + 0);
      if (int32_t(before) != 6) {
        Store(base, pages + 8, before);
      }
      Store(base, pages + 0, 6);
      Store(base, pages + 12, kNone);
      Store(base, pages + 16, kNone);
      guest::StoreU16(base, machine + 9816, 0);
      Store(base, machine + 9728, 0);
      Store(base, machine + 9780, 1);
      Store(base, machine + 9784, 0);
      const uint32_t m = FindMovie(base, 16, frame + 80);
      // The question and the two answers (SetText's varargs the call sites
      // leave unset passed 0).
      calls.CallArgs(kSetText, frame, 0x8218B4CC, {m, kSceneRoot, 0, 10, 0, 0, 0, 0});
      calls.CallArgs(kSetText, frame, 0x8218B4E0, {m, 1, 3, 0, 0, 0, 0, 0});
      calls.CallArgs(kSetText, frame, 0x8218B4F4, {m, 1, 4, 1, 0, 0, 0, 0});
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      Store(base, m + 40960, Load(base, save + 0x1F870) != 0 ? 0 : 1);
      pages = Load(base, kPages);
      before = Load(base, pages + 0);
      if (int32_t(before) != 8) {
        Store(base, pages + 8, before);
      }
      Store(base, pages + 12, kNone);
      Store(base, pages + 16, kNone);
      Store(base, pages + 0, 8);
      Post(base, mode, 34);
    }
    const uint32_t depth = guest::LoadU8(base, machine + 9776);
    Store(base, machine + ((depth + 2433) << 2), 4);
    guest::StoreU8(base, machine + 9776, uint8_t(guest::LoadU8(base, machine + 9776) + 1));
  } else if (mapper.Ask(kPressedOffset, 158, 0x8218B5D8)) {
    // B: back to page 3 through transition 0 (the page object written
    // twice, as the original does).
    calls.Call(kMenuSoundPlay, frame, 0x8218B5E8, 2);
    Store(base, machine + 9716, Load(base, machine + 9828));
    const uint32_t page = Load(base, machine + kPage);
    Store(base, machine + 9780, 1);
    Store(base, machine + 9836, 0);
    Store(base, machine + kPage, 3);
    Store(base, machine + 9644, page);
    for (int k = 0; k < 2; k++) {
      const uint32_t pages = Load(base, kPages);
      const uint32_t before = Load(base, pages + 0);
      if (int32_t(before) != 3) {
        Store(base, pages + 8, before);
      }
      if (k == 0) {
        Store(base, pages + 12, 0);
        Store(base, pages + 4, 3);
        Store(base, pages + 16, kNone);
        Store(base, pages + 0, kNone);
        guest::StoreU16(base, machine + 9816, 0);
      } else {
        Store(base, pages + 12, 0);
        Store(base, pages + 16, kNone);
        Store(base, pages + 4, 3);
        Store(base, pages + 0, kNone);
      }
    }
  }
  return AnimateEventPage(base, machine, frame, calls);
}

}  // namespace race_menu
