// The tuning screen's input pages and the list menu's step
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <optional>
#include <utility>

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

// The players' action mapper entries (8 bytes each: the mapper, blocked
// when +4 is 1) and their indices (a word a player).
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed (slot 10, +40)
constexpr uint32_t kFiredSlot = 0x8210C010;    // ActionMapper::Fired (slot 12, +48)

constexpr uint32_t kMenuStep = 0x82186568;       // Menu::Step(menu)
constexpr uint32_t kMenuSoundPlay = 0x821103F0;  // MenuSound::Play(cue)
constexpr uint32_t kUnloadAll = 0x82208738;      // DecalImages::UnloadAll(images)

constexpr uint32_t kFadeIn = 0x826A50DC;   // layer 0's fade-in count
constexpr uint32_t kFadeOut = 0x826A50E4;  // and fade-out count
constexpr uint32_t kFadeInPasses = 0x82001BEC;
constexpr uint32_t kFadeOutPasses = 0x82001BF0;

// The mapper's read slot at `offset` (+40 slot 10, +48 slot 12) on
// `action`, as the original's bctrl from `frame` returning to `lr`:
// ActionMapper::Pressed's or Fired's plain C++ when the vtable holds it and
// `native_slots` (logged for --log_pause_menu as their adapters do), else
// the slot through `call`. Its r3.
uint64_t ReadSlot(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t mapper,
                  uint32_t offset, uint64_t action, uint32_t frame, uint32_t lr,
                  bool native_slots) {
  const uint32_t function = Load(base, Load(base, mapper) + offset);
  if (native_slots && (function == kPressedSlot || function == kFiredSlot)) {
    const bool pressed = function == kPressedSlot;
    const uint32_t id = uint32_t(action);
    const uint32_t r3 = pressed ? action_mappers::Pressed(base, mapper, id)
                                : action_mappers::Fired(base, mapper, id);
    debug::PauseMenuLog_NoteSlotRead(pressed ? 10 : 12, mapper, id, lr, double(r3));
    return r3;
  }
  return call(function, {mapper, action}, frame, lr);
}

// Push(s)'s first store: the state history's word at the byte +10008 = s
// (the callers step the byte where the original does).
void PushWord(uint8_t* base, uint32_t screen, uint32_t state) {
  Store(base, ((Byte(base, screen + 10008) + 2439) << 2) + screen, state);
}

// Go's first half to state s: q+4 = q+0 = q+8 = s, q+20 = q+12 = -1, q+24 = 0.
void GoFirstHalf(uint8_t* base, uint32_t queue, uint32_t state) {
  Store(base, queue + 4, state);
  Store(base, queue + 0, state);
  Store(base, queue + 8, state);
  Store(base, queue + 20, 0xFFFFFFFFu);
  Store(base, queue + 12, 0xFFFFFFFFu);
  Store(base, queue + 24, 0);
}

// The queue reloaded with the state +9652: q+8 = q+0 when they differ.
// Returns the queue; `state` the state read.
uint32_t QueueCurrent(uint8_t* base, uint32_t screen, uint32_t* state) {
  const uint32_t queue = Load(base, screen + 10036);
  *state = Load(base, screen + 9652);
  const uint32_t current = Load(base, queue + 0);
  if (int32_t(*state) != int32_t(current)) {
    Store(base, queue + 8, current);
  }
  return queue;
}

}  // namespace

uint64_t StepList(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp,
                  bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821CE1D0 Menu::StepList");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = uint32_t(r3);
  const uint32_t player = Load(base, m + 64);
  const uint32_t cursor = Load(base, m + 0);
  const uint32_t index = Load(base, (player << 2) + kControllerEntries);
  Store(base, m + 4, cursor);
  Store(base, m + 44, 0);
  Store(base, m + 36, 0);
  const uint32_t entry = (index << 3) + kMapperEntries;
  // The first fired wins: up, down (the cursor and the row stepped, +52 the
  // row before the step), left, right (+36 the side).
  struct Move {
    uint32_t action;
    uint32_t lr;
    int kind;  // -1 / 1 the cursor, 2 the side -1, 3 the side 1
  };
  static constexpr Move kMoves[] = {
      {153, 0x821CE244, -1}, {167, 0x821CE270, -1}, {154, 0x821CE29C, 1}, {168, 0x821CE2C8, 1},
      {155, 0x821CE2F4, 2},  {169, 0x821CE320, 2},  {156, 0x821CE34C, 3}, {170, 0x821CE378, 3},
  };
  uint64_t result = r3;
  for (const Move& move : kMoves) {
    if (int32_t(Load(base, entry + 4)) == 1) {
      continue;
    }
    result = ReadSlot(base, call, Load(base, entry), 48, move.action, frame, move.lr,
                      native_slots);
    if ((result & 0xFF) == 0) {
      continue;
    }
    if (move.kind == 2) {
      Store(base, m + 36, 0xFFFFFFFFu);
    } else if (move.kind == 3) {
      Store(base, m + 36, 1);
    } else {
      Store(base, m + 0, Load(base, m + 0) + uint32_t(move.kind));
      const uint32_t row = Load(base, m + 48);
      Store(base, m + 48, row + uint32_t(move.kind));
      Store(base, m + 52, row);
    }
    break;
  }
  // The cursor wrapped (+44 = 1 when it left the range) or clamped.
  const uint32_t wraps = Load(base, m + 20);
  int32_t now = int32_t(Load(base, m + 0));
  const int32_t first = int32_t(Load(base, m + 12));
  if (wraps != 0) {
    if (now < first || now > int32_t(Load(base, m + 16))) {
      Store(base, m + 44, 1);
    }
    if (now < first) {
      now = int32_t(Load(base, m + 16));
    } else if (now > int32_t(Load(base, m + 16))) {
      now = first;
    }
  } else if (now < first) {
    now = first;
  } else {
    const int32_t last = int32_t(Load(base, m + 16));
    if (now > last) {
      now = last;
    }
  }
  Store(base, m + 0, uint32_t(now));
  // The visible row: wrapped to the other end after a wrap, else clamped to
  // [+56, +60 - 1].
  int32_t row = int32_t(Load(base, m + 48));
  bool wrapped = false;
  if (wraps != 0) {
    wrapped = int32_t(Load(base, m + 44)) == 1;
    row = int32_t(Load(base, m + 48));
  }
  if (wrapped) {
    const int32_t top = int32_t(Load(base, m + 56));
    if (row < top) {
      row = int32_t(Load(base, m + 60)) - 1;
    } else if (row > int32_t(Load(base, m + 60)) - 1) {
      row = top;
    }
  } else {
    const int32_t top = int32_t(Load(base, m + 56));
    if (row < top) {
      row = top;
    } else {
      const int32_t bottom = int32_t(Load(base, m + 60)) - 1;
      if (row > bottom) {
        row = bottom;
      }
    }
  }
  Store(base, m + 48, uint32_t(row));
  // Which way it moved.
  const int32_t after = int32_t(Load(base, m + 0));
  const int32_t before = int32_t(Load(base, m + 4));
  Store(base, m + 8, after > before ? 1u : after < before ? 0xFFFFFFFFu : 0u);
  return result;
}

uint64_t LeaveEditor(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D8670 TuningScreen::LeaveEditor");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  if ((Load(base, kFadeOut) | Load(base, kFadeIn)) != 0) {
    return screen;
  }
  // Push(61), then the queue to state 62 (its page runs next).
  PushWord(base, s, 61);
  const uint8_t depth = uint8_t(Byte(base, s + 10008) + 1);
  uint32_t queue = Load(base, s + 10036);
  Store(base, s + 9652, 62);
  Store(base, s + 9708, 1);
  guest::StoreU8(base, s + 10008, depth);
  GoFirstHalf(base, queue, 62);
  uint32_t state = 0;
  queue = QueueCurrent(base, s, &state);
  Store(base, queue + 0, state);
  Store(base, queue + 12, 0xFFFFFFFFu);
  Store(base, queue + 16, 0xFFFFFFFFu);
  mode_tick::ScreenFadesFadeIn(base, Load(base, kFadeInPasses), 0, 0, 0, 0);
  // The item's animation in state 4's list restarted and looped.
  const uint32_t movie = Load(base, tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId));
  auto item = [&] { return uint64_t(Byte(base, s + 9660)) + 2; };
  call(tuning_screen::kFinish, {movie, 27, item()}, frame, 0x821D875C);
  call(tuning_screen::kPlay, {movie, 27, item(), 1, 0, 0, 0}, frame, 0x821D8780);
  const uint64_t r3 = call(tuning_screen::kLoop, {movie, 27, item()}, frame, 0x821D8794);
  // The busy icon.
  Store(base, s + 10016, 0);
  Store(base, s + 10020, 2);
  Store(base, s + 10024, 172);
  Store(base, s + 10028, 171);
  Store(base, s + 10032, 173);
  return r3;
}

uint64_t InputCategories(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821D6AD0 TuningScreen::InputCategories");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = (Load(base, kControllerEntries) << 3) + kMapperEntries;
  call(kMenuStep, {Load(base, s + 10040)}, frame, 0x821D6B08);
  const int32_t step = int32_t(Load(base, Load(base, s + 10040) + 8));
  auto pressed = [&](uint64_t action, uint32_t lr) {
    if (int32_t(Load(base, entry + 4)) == 1) {
      return false;
    }
    const uint64_t r3 =
        ReadSlot(base, call, Load(base, entry), 40, action, frame, lr, flags.native_slots);
    return (r3 & 0xFF) != 0;
  };
  if (step == -1 || step == 1) {
    // (1) The cursor moved.
    call(kMenuSoundPlay, {4}, frame, 0x821D6DE4);
    const uint32_t menu = Load(base, s + 10040);
    Store(base, s + 9708, 1);
    const uint32_t cursor = Load(base, menu + 0);
    Store(base, s + 9740, cursor);
    Store(base, s + 9656, cursor);
  } else if (pressed(159, 0x821D6B44)) {
    // (2) A: into the category.
    call(kMenuSoundPlay, {0}, frame, 0x821D6B54);
    const int32_t category = int32_t(Load(base, s + 9656));
    uint32_t menu = Load(base, s + 10040);
    if (category == 2) {
      // Body: one item, straight to its 7 levels (Reset(7, 0), Push(3),
      // the queue to state 5 with command 3, ShowLevels).
      const uint32_t cursor = Load(base, menu + 0);
      guest::StoreU8(base, s + 9661, 0);
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      Store(base, s + 9652, 5);
      guest::StoreU8(base, s + 9677, 7);
      Store(base, s + 9740, cursor);
      Store(base, s + 9656, cursor);
      for (const auto& [field, value] : {std::pair<uint32_t, uint32_t>{20, 1}, {12, 0}, {16, 6},
                                         {0, 0}, {4, 0}, {8, 0}, {40, 0}, {48, 0}, {52, 0},
                                         {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}}) {
        Store(base, menu + field, value);
      }
      PushWord(base, s, 3);
      const uint8_t depth = uint8_t(Byte(base, s + 10008) + 1);
      const uint32_t queue = Load(base, s + 10036);
      const uint32_t state = Load(base, s + 9652);
      guest::StoreU8(base, s + 10008, depth);
      const uint32_t current = Load(base, queue + 0);
      if (int32_t(state) != int32_t(current)) {
        Store(base, queue + 8, current);
      }
      Store(base, queue + 12, 3);
      Store(base, queue + 4, state);
      Store(base, queue + 16, 0xFFFFFFFFu);
      Store(base, queue + 0, 0xFFFFFFFFu);
    } else {
      // Any other: its items (Push(3), the queue to state 4 with command 2,
      // OpenItems; aero's scrolling list reset).
      const uint32_t cursor = Load(base, menu + 0);
      Store(base, s + 9740, 0);
      guest::StoreU8(base, s + 9660, 0);
      Store(base, s + 9656, cursor);
      const uint32_t count = tuning_screen::CategoryItemCount(base, s);
      const uint32_t at = ((Byte(base, s + 10008) + 2439) << 2) + s;
      guest::StoreU8(base, s + 9676, uint8_t(count));
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      Store(base, at, 3);
      const uint8_t depth = uint8_t(Byte(base, s + 10008) + 1);
      uint32_t queue = Load(base, s + 10036);
      Store(base, s + 9652, 4);
      guest::StoreU8(base, s + 10008, depth);
      GoFirstHalf(base, queue, 4);
      const int32_t now = int32_t(Load(base, s + 9656));
      const uint32_t last = Byte(base, s + 9676) - 1;
      menu = Load(base, s + 10040);
      if (now == 4) {
        Store(base, s + 25104, 0);
        Store(base, s + 25100, 0);
        Store(base, menu + 44, 0);
        Store(base, menu + 56, 0);
        Store(base, menu + 60, 8);
      }
      for (const auto& [field, value] :
           {std::pair<uint32_t, uint32_t>{32, 0}, {28, 0}, {24, 0}, {64, 0}, {36, 0}, {52, 0},
            {48, 0}, {16, last}, {40, 0}, {8, 0}, {4, 0}, {0, 0}, {20, 1}, {12, 0}}) {
        Store(base, menu + field, value);
      }
      uint32_t state = 0;
      queue = QueueCurrent(base, s, &state);
      Store(base, queue + 4, state);
      Store(base, queue + 12, 2);
      Store(base, queue + 16, 0xFFFFFFFFu);
      Store(base, queue + 0, 0xFFFFFFFFu);
    }
  } else if (pressed(158, 0x821D6D44)) {
    // (3) B: leaving the shop (state 59's page next), the decal images
    // unloaded, the screen faded out.
    call(kMenuSoundPlay, {2}, frame, 0x821D6D54);
    const uint32_t menu = Load(base, s + 10040);
    const uint32_t cursor = Load(base, menu + 0);
    uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9652, 59);
    Store(base, s + 9740, cursor);
    Store(base, s + 9656, cursor);
    GoFirstHalf(base, queue, 59);
    uint32_t state = 0;
    queue = QueueCurrent(base, s, &state);
    Store(base, queue + 0, state);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 16, 0xFFFFFFFFu);
    if (flags.native_heap) {
      UnloadAll(base, call, uint64_t(s) + 10048, frame, flags.native_free);
    } else {
      call(kUnloadAll, {uint64_t(s) + 10048}, frame, 0x821D6DBC);
    }
    mode_tick::ScreenFadesFadeOut(base, Load(base, kFadeOutPasses), 0, 0, 0, 0);
  }
  return tuning_screen::MoveCategory(base, call, s, frame, flags.native_slots);
}

namespace {

// The page's read of the player's mapper entry `entry` (skipped while its
// +4 is 1): slot `offset` on `action`, its r3 into `*r3`. Whether its low
// byte is set.
bool Asked(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t entry, uint32_t offset,
           uint64_t action, uint32_t frame, uint32_t lr, bool native_slots, uint64_t* r3) {
  if (int32_t(Load(base, entry + 4)) == 1) {
    return false;
  }
  *r3 = ReadSlot(base, call, Load(base, entry), offset, action, frame, lr, native_slots);
  return (*r3 & 0xFF) != 0;
}

// The bar's position on the wheel lists: the word times 40 (mulli, the
// product's low word sign-extended) stored as a doubleword at frame + 80,
// loaded, converted (fcfid) and rounded to a single (frsp); its bits.
uint32_t BarPosition(uint8_t* base, uint32_t frame, uint32_t word) {
  const int64_t x = int32_t(word * 40u);
  Store(base, frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(base, frame + 84, uint32_t(x));
  return std::bit_cast<uint32_t>(float(double(x)));
}

// The player's mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
uint32_t PlayerEntry(uint8_t* base) {
  return (Load(base, kControllerEntries) << 3) + kMapperEntries;
}

}  // namespace

uint64_t BackToItems(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821D87C0 TuningScreen::BackToItems");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 96);
  if ((Load(base, kFadeOut) | Load(base, kFadeIn)) != 0) {
    return screen;
  }
  uint64_t r3;
  if (flags.native_heap) {
    r3 = UnloadAll(base, call, screen + 10048, frame, flags.native_free);
  } else {
    r3 = call(kUnloadAll, {screen + 10048}, frame, 0x821D87F4);
  }
  Store(base, s + 9708, 1);
  Store(base, s + 9716, 1);
  Store(base, s + 9720, 1);
  // Pop until state 4 (no bound: the byte wraps).
  uint32_t depth = Byte(base, s + 10008);
  uint32_t state;
  do {
    depth = (depth + 255) & 0xFF;
    const uint32_t at = ((depth + 2439) << 2) + s;
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    state = Load(base, at);
    Store(base, s + 9652, state);
  } while (int32_t(state) != 4);
  uint32_t queue = Load(base, s + 10036);
  GoFirstHalf(base, queue, state);
  queue = QueueCurrent(base, s, &state);
  Store(base, queue + 16, 0xFFFFFFFFu);
  Store(base, queue + 4, state);
  Store(base, queue + 0, 0xFFFFFFFFu);
  Store(base, queue + 12, 2);
  return r3;
}

uint64_t AnimateWheelParts(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821E8CE0 TuningScreen::AnimateWheelParts");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t movie = Load(base, found);
  auto row = [&] { return uint64_t(Load(base, s + 25196)); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  // The bar snapped to the row: node (27, 0)'s +16.
  auto snap = [&] {
    const uint32_t node = ui_movies::FindNode(base, movie, 27, 0);
    Store(base, node + 16, BarPosition(base, frame, Load(base, s + 25196)));
  };
  uint64_t r3 = found;
  auto asked = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, offset, action, frame, lr, native_slots, &r3);
  };
  uint64_t old_row;
  if (asked(48, 153, 0x821E8D48) || asked(48, 167, 0x821E8D74)) {
    // Up.
    if (Load(base, s + 9708) == 0) {
      return r3;
    }
    if (Load(base, s + 25200) != 0) {
      finish(399, 0x821E8F9C);
      finish(400, 0x821E8FA8);
      snap();
      old_row = 389;
    } else {
      finish(399, 0x821E8FD8);
      play(399, 0x821E8FF4);
      old_row = row() + 390;
    }
  } else if (asked(48, 154, 0x821E8DA0) || asked(48, 168, 0x821E8DCC)) {
    // Down.
    if (Load(base, s + 9708) == 0) {
      return r3;
    }
    if (Load(base, s + 25200) != 0) {
      finish(399, 0x821E8F00);
      finish(400, 0x821E8F0C);
      snap();
      old_row = 391;
    } else {
      finish(400, 0x821E8F50);
      play(400, 0x821E8F6C);
      old_row = row() + 388;
    }
  } else if (asked(40, 159, 0x821E8DF8)) {
    // A: the rows' highlights and the bar finished, the row's pick played.
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 389, 0x821E8E10);
    }
    finish(399, 0x821E8E28);
    finish(400, 0x821E8E34);
    finish(row() + 401, 0x821E8E44);
    return play(row() + 401, 0x821E8ED8);
  } else if (asked(40, 158, 0x821E8E74)) {
    // B: the same, then 966.
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 389, 0x821E8E8C);
    }
    finish(399, 0x821E8EA4);
    finish(400, 0x821E8EB0);
    finish(966, 0x821E8EBC);
    return play(966, 0x821E8ED8);
  } else {
    return r3;
  }
  // The old row finished, the new one restarted and looped.
  finish(old_row, 0x821E9004);
  finish(row() + 389, 0x821E9014);
  play(row() + 389, 0x821E9034);
  return tuning_screen::LoopNamed(call, frame, 0x821E9044, movie, row() + 389);
}

uint64_t InputWheelParts(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821EE390 TuningScreen::InputWheelParts");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  uint64_t r3 = 0;
  auto asked = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, offset, action, frame, lr, flags.native_slots, &r3);
  };
  if (asked(48, 153, 0x821EE3DC) || asked(48, 167, 0x821EE408)) {
    // Up (wrapping to 2).
    call(kMenuSoundPlay, {4}, frame, 0x821EE68C);
    const int32_t row = int32_t(Load(base, s + 25196) - 1);
    Store(base, s + 9708, 1);
    Store(base, s + 25196, uint32_t(row));
    if (row < 0) {
      Store(base, s + 25196, 2);
      Store(base, s + 25200, 1);
    } else {
      Store(base, s + 25200, 0);
    }
  } else if (asked(48, 154, 0x821EE434) || asked(48, 168, 0x821EE460)) {
    // Down (wrapping to 0).
    call(kMenuSoundPlay, {4}, frame, 0x821EE660);
    const int32_t row = int32_t(Load(base, s + 25196) + 1);
    Store(base, s + 9708, 1);
    Store(base, s + 25196, uint32_t(row));
    if (row > 2) {
      Store(base, s + 25196, 0);
      Store(base, s + 25200, 1);
    } else {
      Store(base, s + 25200, 0);
    }
  } else if (asked(40, 159, 0x821EE48C)) {
    // A: brand 0 first, Push(15), Go(16, 8), the wheels previewed.
    call(kMenuSoundPlay, {0}, frame, 0x821EE49C);
    Store(base, s + 25204, 0);
    Store(base, s + 9708, 1);
    Store(base, s + 9716, 1);
    Store(base, s + 25208, 0);
    Store(base, s + 25212, 0);
    Store(base, s + 25216, 0);
    Store(base, s + 25220, 0);
    SetWheelListEnd(base, s);
    PushWord(base, s, 15);
    const uint8_t depth = uint8_t(Byte(base, s + 10008) + 1);
    uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9652, 16);
    guest::StoreU8(base, s + 10008, depth);
    GoFirstHalf(base, queue, 16);
    uint32_t state = 0;
    queue = QueueCurrent(base, s, &state);
    Store(base, queue + 16, 0xFFFFFFFFu);
    Store(base, queue + 4, state);
    Store(base, queue + 0, 0xFFFFFFFFu);
    Store(base, queue + 12, 8);
    std::optional<double> f1;  // PreviewWheel's f1, not read after
    tuning_screen::PreviewWheels(base, call, s, frame, &f1);
    ShowPricePanel(base, s, frame);
  } else if (asked(40, 158, 0x821EE574)) {
    // B: Pop, the menu reset to the page's count and cursor, OpenItems.
    call(kMenuSoundPlay, {2}, frame, 0x821EE584);
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9708, 1);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    GoFirstHalf(base, queue, popped);
    const uint32_t count = PageItemCount(base, s);
    const uint32_t cursor = PageCursor(base, s);
    const uint32_t menu = Load(base, s + 10040);
    for (const auto& [field, value] :
         {std::pair<uint32_t, uint32_t>{12, 0}, {16, count - 1}, {20, 1}, {8, 0}, {0, cursor},
          {4, cursor}, {44, 0}, {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}}) {
      Store(base, menu + field, value);
    }
    uint32_t state = 0;
    queue = QueueCurrent(base, s, &state);
    Store(base, queue + 16, 0xFFFFFFFFu);
    Store(base, queue + 4, state);
    Store(base, queue + 0, 0xFFFFFFFFu);
    Store(base, queue + 12, 2);
  }
  return AnimateWheelParts(base, call, s, frame, flags.native_slots);
}

uint64_t AnimateInches(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                       uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821E9110 TuningScreen::AnimateInches");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t movie = Load(base, found);
  auto row = [&] { return uint64_t(Load(base, s + 25228)); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  // The bar snapped to the row: Child(node (1, 9), 0)'s +16.
  auto snap = [&] {
    const uint32_t node = ui_movies::Child(base, ui_movies::FindNode(base, movie, 1, 9), 0);
    Store(base, node + 16, BarPosition(base, frame, Load(base, s + 25228)));
  };
  uint64_t r3 = found;
  auto asked = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, offset, action, frame, lr, native_slots, &r3);
  };
  uint64_t old_row;
  if (asked(48, 153, 0x821E9178) || asked(48, 167, 0x821E91A4)) {
    // Up.
    if (Load(base, s + 9708) == 0) {
      return r3;
    }
    if (Load(base, s + 25236) != 0) {
      finish(30, 0x821E93C0);
      finish(31, 0x821E93CC);
      snap();
      old_row = 20;
    } else {
      finish(30, 0x821E9414);
      play(30, 0x821E9430);
      old_row = row() + 21;
    }
  } else if (asked(48, 154, 0x821E91D0) || asked(48, 168, 0x821E91FC)) {
    // Down.
    if (Load(base, s + 9708) == 0) {
      return r3;
    }
    if (Load(base, s + 25236) != 0) {
      finish(30, 0x821E9318);
      finish(31, 0x821E9324);
      snap();
      old_row = uint64_t(Load(base, s + 25232)) + 20;
    } else {
      finish(31, 0x821E9374);
      play(31, 0x821E9390);
      old_row = row() + 19;
    }
  } else if (asked(40, 159, 0x821E9228)) {
    // A: the rows and the bar finished.
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 20, 0x821E9240);
    }
    finish(30, 0x821E9258);
    return finish(31, 0x821E9264);
  } else if (asked(40, 158, 0x821E928C)) {
    // B: the same, then 62 restarted.
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 20, 0x821E92A4);
    }
    finish(30, 0x821E92BC);
    finish(31, 0x821E92C8);
    finish(62, 0x821E92D4);
    return play(62, 0x821E92F0);
  } else {
    return r3;
  }
  // The old row finished, the new one restarted and looped.
  finish(old_row, 0x821E9440);
  finish(row() + 20, 0x821E9450);
  play(row() + 20, 0x821E9470);
  return tuning_screen::LoopNamed(call, frame, 0x821E9480, movie, row() + 20);
}

uint64_t InputInches(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                     uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821EEEA8 TuningScreen::InputInches");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  uint64_t r3 = 0;
  auto asked = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, offset, action, frame, lr, flags.native_slots, &r3);
  };
  if (asked(48, 153, 0x821EEEFC) || asked(48, 167, 0x821EEF28)) {
    // Up (wrapping to the list's end +25232).
    call(kMenuSoundPlay, {4}, frame, 0x821EF230);
    const int32_t row = int32_t(Load(base, s + 25228) - 1);
    Store(base, s + 9708, 1);
    Store(base, s + 25228, uint32_t(row));
    if (row < 0) {
      Store(base, s + 25228, Load(base, s + 25232));
      Store(base, s + 25236, 1);
    } else {
      Store(base, s + 25236, 0);
    }
  } else if (asked(48, 154, 0x821EEF54) || asked(48, 168, 0x821EEF80)) {
    // Down (wrapping to 0).
    call(kMenuSoundPlay, {4}, frame, 0x821EF200);
    const int32_t row = int32_t(Load(base, s + 25228) + 1);
    const int32_t last = int32_t(Load(base, s + 25232));
    Store(base, s + 9708, 1);
    Store(base, s + 25228, uint32_t(row));
    if (last < row) {
      Store(base, s + 25228, 0);
      Store(base, s + 25236, 1);
    } else {
      Store(base, s + 25236, 0);
    }
  } else if (asked(40, 159, 0x821EEFAC)) {
    // A: the confirm dialog (command 63): equip when owned, else the money
    // short or buy; then Push(17).
    call(kMenuSoundPlay, {0}, frame, 0x821EEFBC);
    uint32_t menu;
    if (int32_t(IsPickOwned(base, s)) != 0) {
      const uint32_t queue = Load(base, s + 10036);
      guest::StoreU8(base, s + 9665, 0);
      Store(base, s + 9652, 11);
      GoFirstHalf(base, queue, 11);
      menu = Load(base, s + 10040);
      for (const uint32_t field : {0u, 4u, 40u, 48u, 52u, 32u}) {
        Store(base, menu + field, 0);
      }
      Store(base, menu + 12, 0);
      Store(base, menu + 16, 1);
    } else {
      const int32_t price = int32_t(tuning_screen::GetPrice(base, s));
      const int32_t money = int32_t(Load(base, s + 25116));
      const uint32_t queue = Load(base, s + 10036);
      if (money < price) {
        Store(base, s + 9652, 10);
        GoFirstHalf(base, queue, 10);
        menu = Load(base, s + 10040);
        for (const uint32_t field : {12u, 16u, 0u, 4u, 40u, 48u, 52u, 32u}) {
          Store(base, menu + field, 0);
        }
      } else {
        guest::StoreU8(base, s + 9665, 0);
        Store(base, s + 9652, 9);
        GoFirstHalf(base, queue, 9);
        const uint32_t pick = Byte(base, s + 9665);
        menu = Load(base, s + 10040);
        Store(base, menu + 0, pick);
        Store(base, menu + 4, pick);
        Store(base, menu + 44, 0);
        Store(base, menu + 56, 0);
        Store(base, menu + 60, 8);
        Store(base, menu + 12, 0);
        Store(base, menu + 16, 1);
      }
    }
    for (const auto& [field, value] : {std::pair<uint32_t, uint32_t>{20, 1}, {8, 0}, {36, 0},
                                       {64, 0}, {24, 0}, {28, 0}}) {
      Store(base, menu + field, value);
    }
    uint32_t state = 0;
    const uint32_t queue = QueueCurrent(base, s, &state);
    Store(base, queue + 0, 0xFFFFFFFFu);
    Store(base, queue + 4, state);
    Store(base, queue + 16, 0xFFFFFFFFu);
    Store(base, queue + 12, 63);
    Store(base, s + 9708, 1);
    PushWord(base, s, 17);
    guest::StoreU8(base, s + 10008, uint8_t(Byte(base, s + 10008) + 1));
  } else if (asked(40, 158, 0x821EF168)) {
    // B: Pop, the wheel list's set-up (command 8).
    call(kMenuSoundPlay, {2}, frame, 0x821EF178);
    Store(base, s + 9708, 1);
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    uint32_t queue = Load(base, s + 10036);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    GoFirstHalf(base, queue, popped);
    uint32_t state = 0;
    queue = QueueCurrent(base, s, &state);
    Store(base, queue + 16, 0xFFFFFFFFu);
    Store(base, queue + 4, state);
    Store(base, queue + 0, 0xFFFFFFFFu);
    Store(base, queue + 12, 8);
  } else {
    return AnimateInches(base, call, s, frame, flags.native_slots);
  }
  std::optional<double> f1;  // PreviewWheel's f1, not read after
  tuning_screen::PreviewWheels(base, call, s, frame, &f1);
  return AnimateInches(base, call, s, frame, flags.native_slots);
}

}  // namespace tuning_pages
