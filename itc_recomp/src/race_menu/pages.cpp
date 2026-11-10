// The race menu's pages Draw runs (docs/functions/race-menu.md): see
// race_menu.h.

#include "race_menu/race_menu.h"

#include <bit>

#include "base/log.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "race_menu/internal.h"
#include "ui_movies/game.h"

namespace race_menu {

namespace {

constexpr uint64_t kSceneRoot = ~0ull;  // the sub-scene -1
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kResetScroll = 0x8211C5F8;
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

}  // namespace race_menu
