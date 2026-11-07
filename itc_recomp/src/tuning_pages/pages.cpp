// The tuning screen's input pages and the list menu's step
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <functional>
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

namespace {

constexpr uint32_t kResetPose = 0x8211ABA8;         // UiMovie::ResetPose(movie)
constexpr uint32_t kRecordCopy = 0x820DB290;        // SceneModelRecord::Copy(to, from)
constexpr uint32_t kResetStickerEditor = 0x821F8440;  // not seen in play: recompiled
constexpr uint32_t kLoadStickers = 0x821F86C0;        // not seen in play: recompiled
constexpr uint32_t kCarScene = 0x826C2E44;          // the car scene (its second set's pools)
constexpr uint32_t kStandHolder = 0x826D83E0;       // [[[it]]] + 8: the stand car's model

// The item list's bar position: the int stored as a doubleword at frame + 80
// (std), loaded, converted (fcfid, frsp) and multiplied by the 40.0 at
// 0x82001FB0 (fmuls); the float's bits for the stfs.
uint32_t ItemPosition(uint8_t* base, uint32_t frame, int64_t x) {
  namespace steps = camera_modes::steps;
  Store(base, frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(base, frame + 84, uint32_t(x));
  return steps::Bits(steps::Fmuls(steps::Single(double(x)), steps::Lfs(base, 0x82001FB0)));
}

// The queue's tail after Go's first half: q+8 = q+0 when the state +9652
// differs from it, q+12 = c, q+4 = the state, q+16 = q+0 = -1.
void GoTail(uint8_t* base, uint32_t screen, uint32_t command) {
  uint32_t state = 0;
  const uint32_t queue = QueueCurrent(base, screen, &state);
  Store(base, queue + 12, command);
  Store(base, queue + 4, state);
  Store(base, queue + 16, 0xFFFFFFFFu);
  Store(base, queue + 0, 0xFFFFFFFFu);
}

// Push(4)'s word at the depth `depth` read before, then the depth stepped
// (read again), +9652 = s and Go's first half to s.
void PushFourGo(uint8_t* base, uint32_t screen, uint32_t depth, uint32_t state) {
  Store(base, ((depth + 2439) << 2) + screen, 4);
  const uint8_t next = uint8_t(Byte(base, screen + 10008) + 1);
  const uint32_t queue = Load(base, screen + 10036);
  Store(base, screen + 9652, state);
  guest::StoreU8(base, screen + 10008, next);
  GoFirstHalf(base, queue, state);
}

// The menu's fields stored in the original's order.
void StoreMenu(uint8_t* base, uint32_t menu,
               std::initializer_list<std::pair<uint32_t, uint32_t>> fields) {
  for (const auto& [field, value] : fields) {
    Store(base, menu + field, value);
  }
}

}  // namespace

uint64_t MoveItem(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                  uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821D0CC0 TuningScreen::MoveItem");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t menu = Load(base, s + 10040);
  const uint32_t movie = Load(base, found);
  const int32_t step = int32_t(Load(base, menu + 8));
  auto item = [&] { return uint64_t(Byte(base, s + 9660)); };
  auto word = [&](uint32_t offset) { return uint64_t(Load(base, s + offset)); };
  auto finish = [&](uint64_t index, uint32_t lr) {
    return call(tuning_screen::kFinish, {movie, 27, index}, frame, lr);
  };
  auto play = [&](uint64_t index, uint32_t lr) {
    return call(tuning_screen::kPlay, {movie, 27, index, 1, 0, 0, 0}, frame, lr);
  };
  auto loop = [&](uint64_t index, uint32_t lr) {
    return call(tuning_screen::kLoop, {movie, 27, index}, frame, lr);
  };
  // The bar snapped: node (27, 0)'s +16 = float(x) x 40.0 (x read after the
  // lookup).
  auto snap = [&](auto x) {
    const uint32_t node = ui_movies::FindNode(base, movie, 27, 0);
    Store(base, node + 16, ItemPosition(base, frame, x()));
  };
  auto aero_row = [&] { return int64_t(int32_t(Load(base, s + 25100))); };
  // The aero item's availability (category 4), else 1.
  auto available = [&]() -> uint32_t {
    if (int32_t(Load(base, s + 9656)) != 4) {
      return 1;
    }
    return Load(base, ((Byte(base, s + 9660) + 6282) << 2) + s);
  };
  // The aero list's rows: the row before (+25104) finished, the row now
  // (+25100) restarted and looped, or with the item not available finished
  // and the row before's mark (node (27, +25104 + 5)'s +68) set.
  auto row = [&]() -> uint64_t {
    const bool on = available() != 0;
    const uint64_t before = word(25104) + 2;
    if (on) {
      finish(before, 0x821D0F0C);
      finish(word(25100) + 2, 0x821D0F20);
      play(word(25100) + 2, 0x821D0F44);
      return loop(word(25100) + 2, 0x821D0F58);
    }
    finish(before, 0x821D10AC);
    finish(word(25100) + 2, 0x821D10C0);
    const uint32_t node = ui_movies::FindNode(base, movie, 27, Load(base, s + 25104) + 5);
    Store(base, node + 68, camera_modes::steps::Bits(camera_modes::steps::Lfs(base, 0x82002A3C)));
    return node;
  };
  if (step == -1 || step == 1) {
    const bool up = step == -1;
    if (int32_t(Load(base, s + 9656)) != 4) {
      uint64_t first;
      if (Load(base, menu + 40) != 0) {
        // Wrapped: the bar snapped to the item.
        finish(12, up ? 0x821D0D3C : 0x821D0F8C);
        finish(13, up ? 0x821D0D4C : 0x821D0F9C);
        snap([&] { return int64_t(item()); });
        first = up ? uint64_t(Byte(base, s + 9676)) + 1 : 2;
      } else {
        // The bar slid one way: 12 (up) or 13 (down) restarted.
        const uint64_t slide = up ? 12 : 13;
        finish(slide, up ? 0x821D0D90 : 0x821D0FE4);
        play(slide, up ? 0x821D0DB0 : 0x821D1004);
        first = item() + (up ? 3 : 1);
      }
      finish(first, 0x821D0DC4);
      finish(item() + 2, 0x821D0DD8);
      play(item() + 2, 0x821D0DFC);
      return loop(item() + 2, 0x821D0F58);
    }
    // The aero list.
    const int32_t before = int32_t(Load(base, s + 25104));
    if (up ? (before == 7 && int32_t(Load(base, s + 25100)) == 0)
           : (before == 0 && int32_t(Load(base, s + 25100)) == 7)) {
      finish(12, up ? 0x821D0E2C : 0x821D1038);
      finish(13, up ? 0x821D0E3C : 0x821D1048);
      snap(aero_row);
    } else {
      if (Load(base, s + 9708) == 0) {
        return found;
      }
      if (up) {
        if (before > 0) {
          const int32_t now = int32_t(Load(base, s + 25100));
          if (now >= 0 && now < 7) {
            finish(12, 0x821D0EAC);
            play(12, 0x821D0ECC);
          }
        }
      } else if (before < 7) {
        const int32_t now = int32_t(Load(base, s + 25100));
        if (now <= 7 && now > 0) {
          finish(13, 0x821D10A0);
          play(13, 0x821D0ECC);
        }
      }
    }
    return row();
  }
  // No step: A (the rows finished, the category's pick restarted) or B
  // (back to the categories).
  uint64_t r3 = found;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, native_slots, &r3);
  };
  auto finish_all = [&](uint32_t lr, uint32_t lr12, uint32_t lr13) {
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 2, lr);
    }
    finish(12, lr12);
    return finish(13, lr13);
  };
  if (asked(159, 0x821D1108)) {
    if (available() == 0) {
      return r3;
    }
    r3 = finish_all(0x821D1150, 0x821D116C, 0x821D117C);
    if (int32_t(Load(base, s + 9656)) == 4) {
      r3 = tuning_screen::FinishNamed(call, frame, 0x821D1194, movie, 1013);
    }
    if (int32_t(Load(base, s + 9656)) != 3) {
      return r3;
    }
    tuning_screen::FinishNamed(call, frame, 0x821D11B0, movie, item() + 401);
    return tuning_screen::PlayNamed(call, frame, 0x821D12B4, movie, item() + 401, 1);
  }
  if (!asked(158, 0x821D11E0)) {
    return r3;
  }
  finish_all(0x821D11FC, 0x821D1218, 0x821D1228);
  if (int32_t(Load(base, s + 9656)) == 4) {
    tuning_screen::FinishNamed(call, frame, 0x821D1240, movie, 1013);
  }
  if (int32_t(Load(base, s + 9656)) != 4) {
    tuning_screen::FinishNamed(call, frame, 0x821D1258, movie, 966);
    return tuning_screen::PlayNamed(call, frame, 0x821D12B4, movie, 966, 1);
  }
  call(tuning_screen::kFinish, {movie, tuning_screen::kAllScenes, 15}, frame, 0x821D126C);
  call(tuning_screen::kPlay, {movie, tuning_screen::kAllScenes, 15, 1, 0, 0, 0}, frame,
       0x821D128C);
  tuning_screen::FinishNamed(call, frame, 0x821D1298, movie, 967);
  return tuning_screen::PlayNamed(call, frame, 0x821D12B4, movie, 967, 1);
}

uint64_t InputItems(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821DFD48 TuningScreen::InputItems");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  {
    const uint32_t menu = Load(base, s + 10040);
    if (int32_t(Load(base, s + 9656)) == 4) {
      StepList(base, call, menu, frame, flags.native_slots);
    } else {
      call(kMenuStep, {menu}, frame, 0x821DFD84);
    }
  }
  const int32_t step = int32_t(Load(base, Load(base, s + 10040) + 8));
  uint64_t r3 = 0;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, flags.native_slots, &r3);
  };
  auto sound = [&](uint64_t cue, uint32_t lr) { call(kMenuSoundPlay, {cue}, frame, lr); };
  if (step == -1 || step == 1) {
    // (1) The cursor moved (the aero list's rows: the one before and now).
    sound(4, 0x821E076C);
    const uint32_t menu = Load(base, s + 10040);
    const int32_t category = int32_t(Load(base, s + 9656));
    Store(base, s + 9708, 1);
    const uint32_t cursor = Load(base, menu + 0);
    Store(base, s + 9740, cursor);
    guest::StoreU8(base, s + 9660, uint8_t(cursor));
    if (category == 4) {
      Store(base, s + 25104, Load(base, s + 25100));
      Store(base, s + 25100, Load(base, menu + 48));
    }
  } else if (asked(159, 0x821DFDC8)) {
    // (2) A: the stand car's record copied into the screen, then the item's
    // page when its bit in +9688 is set.
    const uint32_t holder = Load(base, Load(base, Load(base, kStandHolder)));
    const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, Load(base, kCarScene), 7,
                                                             Load(base, holder + 8));
    call(kRecordCopy, {screen + 20816, uint64_t(model) + 32}, frame, 0x821DFE04);
    const uint32_t cursor0 = Load(base, Load(base, s + 10040) + 0);
    const uint32_t bit = (cursor0 & 0x20) != 0 ? 0 : (1u << (cursor0 & 0x1F));
    if ((bit & Load(base, s + 9688)) != 0) {
      const int32_t category = int32_t(Load(base, s + 9656));
      const uint32_t item = Byte(base, s + 9660);
      enum class Page { kNone, kLevels, kTypes, kDoor, kWheels, kPaint, kLights, kEditor,
                        kAccessory };
      Page page = Page::kNone;
      if (category == 0 || category == 1 || category == 3) {
        page = Page::kLevels;
      } else if (category == 4) {
        page = Page::kTypes;
      } else if (category == 5) {
        switch (item) {
          case 0: page = Page::kWheels; break;
          case 1: page = Page::kDoor; break;
          case 2:
          case 3: page = Page::kTypes; break;
          case 4: page = Page::kPaint; break;
          case 5: page = Page::kLights; break;
          case 6:
          case 7: page = Page::kEditor; break;
          default: break;
        }
      } else if (category == 6) {
        page = item <= 2 ? Page::kTypes : item == 3 ? Page::kAccessory : Page::kNone;
      }
      switch (page) {
        case Page::kNone:
          break;
        case Page::kLevels: {
          // The level list (Push(4), Go(5, 3), ShowLevels).
          sound(0, 0x821E04F4);
          const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
          Store(base, s + 9740, 0);
          guest::StoreU8(base, s + 9661, 0);
          guest::StoreU8(base, s + 9660, uint8_t(cursor));
          const uint32_t count = LevelCount(base, s);
          const uint32_t depth = Byte(base, s + 10008);
          Store(base, s + 9708, 1);
          Store(base, s + 9716, 1);
          guest::StoreU8(base, s + 9677, uint8_t(count));
          PushFourGo(base, s, depth, 5);
          const int32_t now = int32_t(Load(base, s + 9656));
          if ((now == 0 && Byte(base, s + 9660) == 0) || (now == 1 && Byte(base, s + 9660) == 4)) {
            // The 10-level lists scroll.
            const uint32_t last = Byte(base, s + 9677) - 1;
            const uint32_t pick = Byte(base, s + 9661);
            const uint32_t menu = Load(base, s + 10040);
            StoreMenu(base, menu,
                      {{12, 0}, {20, 1}, {8, 0}, {40, 0}, {0, pick}, {16, last}, {4, pick},
                       {44, 0}, {48, 0}, {52, 0}, {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0},
                       {28, 0}, {32, 0}});
            Store(base, s + 25104, 0);
            Store(base, s + 25100, 0);
          } else {
            const uint32_t last = Byte(base, s + 9677) - 1;
            const uint32_t menu = Load(base, s + 10040);
            const uint32_t pick = Byte(base, s + 9661);
            StoreMenu(base, menu,
                      {{20, 1}, {12, pick}, {0, 0}, {4, 0}, {8, 0}, {40, 0}, {16, last},
                       {48, 0}, {52, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}});
          }
          GoTail(base, s, 3);
          break;
        }
        case Page::kTypes: {
          // The type list (Push(4), Go(6, 4), ShowTypes), the aero item
          // only when available; the bonnet's decal request.
          if (category == 4 &&
              Load(base, ((Byte(base, s + 9660) + 6282) << 2) + s) == 0) {
            sound(3, 0x821E04E8);
            break;
          }
          sound(0, 0x821E03D8);
          const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
          guest::StoreU8(base, s + 9662, 0);
          Store(base, s + 9740, cursor);
          guest::StoreU8(base, s + 9660, uint8_t(cursor));
          const uint32_t count = TypeCount(base, s);
          const uint32_t depth = Byte(base, s + 10008);
          guest::StoreU8(base, s + 9678, uint8_t(count));
          Store(base, s + 9708, 1);
          Store(base, s + 9716, 1);
          PushFourGo(base, s, depth, 6);
          const uint32_t last = Byte(base, s + 9678) - 1;
          const uint32_t pick = Byte(base, s + 9662);
          const uint32_t menu = Load(base, s + 10040);
          StoreMenu(base, menu,
                    {{12, 0}, {20, 1}, {8, 0}, {44, 0}, {0, pick}, {16, last}, {4, pick},
                     {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
          {
            uint32_t state = 0;
            const uint32_t queue = QueueCurrent(base, s, &state);
            Store(base, queue + 12, 4);
            Store(base, queue + 16, 0xFFFFFFFFu);
            Store(base, queue + 4, state);
            Store(base, queue + 0, 0xFFFFFFFFu);
          }
          if (int32_t(Load(base, s + 9656)) == 4 && Byte(base, s + 9660) == 4) {
            Store(base, s + 10012, 10);
          }
          break;
        }
        case Page::kDoor: {
          // The door's options (Push(4), Go(8, 6), ShowOptions).
          sound(0, 0x821DFEC4);
          const uint32_t menu = Load(base, s + 10040);
          const uint32_t depth = Byte(base, s + 10008);
          const uint32_t cursor = Load(base, menu + 0);
          guest::StoreU8(base, s + 9664, 0);
          guest::StoreU8(base, s + 9680, 2);
          Store(base, s + 9708, 1);
          Store(base, s + 9716, 1);
          Store(base, s + 9740, cursor);
          guest::StoreU8(base, s + 9660, uint8_t(cursor));
          PushFourGo(base, s, depth, 8);
          const uint32_t last = Byte(base, s + 9680) - 1;
          StoreMenu(base, Load(base, s + 10040),
                    {{12, 0}, {20, 1}, {0, 0}, {4, 0}, {8, 0}, {40, 0}, {16, last}, {48, 0},
                     {52, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}});
          GoTail(base, s, 6);
          break;
        }
        case Page::kWheels:
        case Page::kLights:
        case Page::kAccessory: {
          // The wheels' parts (Go(15, 7)), the light kits (Go(19, 11)) or the
          // accessory (Go(53, 54)), with Push(4) and no menu reset.
          sound(0, page == Page::kWheels   ? 0x821DFFBC
                   : page == Page::kLights ? 0x821E0134
                                           : 0x821E0318);
          const uint32_t menu = Load(base, s + 10040);
          const uint32_t depth = Byte(base, s + 10008);
          const uint32_t cursor = Load(base, menu + 0);
          if (page == Page::kWheels) {
            Store(base, s + 25196, 0);
          } else if (page == Page::kLights) {
            Store(base, s + 30492, 0);
          }
          Store(base, s + 9708, 1);
          Store(base, s + 9716, 1);
          if (page == Page::kAccessory) {
            Store(base, s + 33540, 0);
          }
          Store(base, s + 9740, cursor);
          guest::StoreU8(base, s + 9660, uint8_t(cursor));
          const uint32_t state = page == Page::kWheels ? 15 : page == Page::kLights ? 19 : 53;
          PushFourGo(base, s, depth, state);
          GoTail(base, s, page == Page::kWheels ? 7 : page == Page::kLights ? 11 : 54);
          break;
        }
        case Page::kPaint: {
          // The paint being picked from the saved one (Push(4), Go(18, 10)).
          sound(0, 0x821E0060);
          const uint32_t menu = Load(base, s + 10040);
          const uint32_t a = Load(base, s + 30480);
          const uint32_t b = Load(base, s + 30488);
          const uint32_t c = Load(base, s + 30476);
          const uint32_t d = Load(base, s + 30484);
          const uint32_t cursor = Load(base, menu + 0);
          Store(base, s + 30448, a);
          const uint32_t depth = Byte(base, s + 10008);
          Store(base, s + 30456, b);
          Store(base, s + 30468, b);
          Store(base, s + 9740, cursor);
          guest::StoreU8(base, s + 9660, uint8_t(cursor));
          Store(base, s + 30444, c);
          Store(base, s + 30440, 0);
          Store(base, s + 30460, 0);
          Store(base, s + 30452, d);
          Store(base, s + 30464, 0);
          Store(base, s + 30472, 0);
          Store(base, s + 9708, 1);
          Store(base, s + 9716, 1);
          PushFourGo(base, s, depth, 18);
          GoTail(base, s, 10);
          break;
        }
        case Page::kEditor: {
          // The vinyl or sticker editor: its records loaded, the decal
          // images requested, the queue to state 60 (no Push), faded out.
          sound(0, 0x821E01EC);
          const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
          Store(base, s + 30724, 0);
          Store(base, s + 9740, cursor);
          guest::StoreU8(base, s + 9660, uint8_t(cursor));
          SetEditorLimits(base, s);
          Store(base, s + 9708, 1);
          Store(base, s + 9716, 1);
          const uint32_t vinyls = tuning_screen::MovieEntry(base, frame, 13);
          call(kResetPose, {Load(base, vinyls)}, frame, 0x821E0238);
          const uint32_t stickers = tuning_screen::MovieEntry(base, frame, 14);
          call(kResetPose, {Load(base, stickers)}, frame, 0x821E0254);
          tuning_screen::StoreToGarageCar(base, screen, frame);
          if (Byte(base, s + 9660) == 6) {
            LoadVinyls(base, call, screen, frame);
            LoadPageLayouts(base, s);
          } else {
            call(kResetStickerEditor, {screen}, frame, 0x821E0280);
            call(kLoadStickers, {screen}, frame, 0x821E0288);
          }
          LoadDecalImages(base, call, screen, frame);
          uint32_t queue = Load(base, s + 10036);
          Store(base, s + 9652, 60);
          GoFirstHalf(base, queue, 60);
          uint32_t state = 0;
          queue = QueueCurrent(base, s, &state);
          Store(base, queue + 0, state);
          Store(base, queue + 12, 0xFFFFFFFFu);
          Store(base, queue + 16, 0xFFFFFFFFu);
          mode_tick::ScreenFadesFadeOut(base, Load(base, kFadeOutPasses), 0, 0, 0, 0);
          break;
        }
      }
    }
  } else if (asked(158, 0x821E0684)) {
    // (3) B: Pop, the menu reset to the page's count and cursor, PlayIntro.
    sound(2, 0x821E0694);
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    const uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9708, 1);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    Store(base, queue + 20, 0xFFFFFFFFu);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 24, 0);
    Store(base, queue + 4, popped);
    Store(base, queue + 0, popped);
    Store(base, queue + 8, popped);
    const uint32_t count = PageItemCount(base, s);
    const uint32_t cursor = PageCursor(base, s);
    StoreMenu(base, Load(base, s + 10040),
              {{12, 0}, {16, count - 1}, {20, 1}, {8, 0}, {0, cursor}, {4, cursor}, {44, 0},
               {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
    GoTail(base, s, 0);
  }
  return MoveItem(base, call, s, frame, flags.native_slots);
}

namespace {

// MoveOption's lists (its row): the bars (up, down), the rows' first named
// animations (the second set 0 for the bonnet), the list's node (FindNode
// of sub-scene `sub` and child `index`, then Child 0 when `child`), and
// the return addresses of each step's calls: [path] for the paths up
// wrapped, up, down wrapped, down; the bars' two calls, the first rows'
// four (F of the row before or after, F, P, L) and the second rows' four
// (the same on every path).
struct OptionList {
  uint64_t bar_up, bar_down, rows_a, rows_b;
  uint32_t sub, index;
  bool child;
  uint32_t bars[4][2];
  uint32_t first[4][4];
  uint32_t second[4];
};

constexpr OptionList kTireOptions = {
    854, 855, 844, 783, 33, 1, true,
    {{0x821D5688, 0x821D5694}, {0x821D5774, 0x821D5790}, {0x821D5A4C, 0x821D5A58},
     {0x821D5AF4, 0x821D5B10}},
    {{0x821D56E4, 0x821D56F4, 0x821D5714, 0x821D5724},
     {0x821D57A0, 0x821D57B0, 0x821D57D0, 0x821D57E0},
     {0x821D5AA4, 0x821D5AB4, 0x821D5AD4, 0x821D5AE4},
     {0x821D5B20, 0x821D5B30, 0x821D5B50, 0x821D5B60}},
    {0x821D5734, 0x821D5744, 0x821D5764, 0x821D5D9C}};
constexpr OptionList kBonnetOptions = {
    685, 686, 675, 0, 31, 0, false,
    {{0x821D5818, 0x821D5824}, {0x821D58AC, 0x821D58C8}, {0x821D5B94, 0x821D5BA0},
     {0x821D5BE8, 0x821D5C04}},
    {{0x821D586C, 0x821D587C, 0x821D589C, 0x821D5D9C},
     {0x821D586C, 0x821D587C, 0x821D589C, 0x821D5D9C},
     {0x821D586C, 0x821D587C, 0x821D589C, 0x821D5D9C},
     {0x821D586C, 0x821D587C, 0x821D589C, 0x821D5D9C}},
    {}};
constexpr OptionList kDoorOptions = {
    499, 500, 489, 428, 28, 1, true,
    {{0x821D5900, 0x821D590C}, {0x821D59A8, 0x821D59C4}, {0x821D5C38, 0x821D5C44},
     {0x821D5CE0, 0x821D5CFC}},
    {{0x821D595C, 0x821D596C, 0x821D598C, 0x821D599C},
     {0x821D59D4, 0x821D59E4, 0x821D5A04, 0x821D5A14},
     {0x821D5C90, 0x821D5CA0, 0x821D5CC0, 0x821D5CD0},
     {0x821D5D0C, 0x821D5D1C, 0x821D5D3C, 0x821D5D4C}},
    {0x821D5D5C, 0x821D5D6C, 0x821D5D8C, 0x821D5D9C}};

// The option page's list: the tire (category 3), the bonnet (category 4,
// item 4), the door (category 5, item 1), else none.
const OptionList* OptionListOf(uint8_t* base, uint32_t screen) {
  const int32_t category = int32_t(Load(base, screen + 9656));
  if (category == 3) {
    return &kTireOptions;
  }
  if (category == 4 && Byte(base, screen + 9660) == 4) {
    return &kBonnetOptions;
  }
  if (category == 5 && Byte(base, screen + 9660) == 1) {
    return &kDoorOptions;
  }
  return nullptr;
}

}  // namespace

uint64_t MoveOption(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                    uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821D5610 TuningScreen::MoveOption");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t menu = Load(base, s + 10040);
  const uint32_t movie = Load(base, found);
  const int32_t step = int32_t(Load(base, menu + 8));
  auto option = [&] { return uint64_t(Byte(base, s + 9664)); };
  auto count = [&] { return uint64_t(Byte(base, s + 9680)); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto loop = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::LoopNamed(call, frame, lr, movie, named);
  };
  if (step == -1 || step == 1) {
    const OptionList* list = OptionListOf(base, s);
    if (list == nullptr) {
      return found;
    }
    const bool up = step == -1;
    const bool wrapped = int32_t(Load(base, menu + 40)) != 0;
    const int path = (up ? 0 : 2) + (wrapped ? 0 : 1);
    const uint32_t* bars = list->bars[path];
    if (wrapped) {
      // The bars finished, the bar snapped to the option.
      finish(list->bar_up, bars[0]);
      finish(list->bar_down, bars[1]);
      uint32_t node = ui_movies::FindNode(base, movie, list->sub, list->index);
      if (list->child) {
        node = ui_movies::Child(base, node, 0);
      }
      Store(base, node + 16, ItemPosition(base, frame, int64_t(option())));
    } else {
      // The bar slid one way: up or down restarted.
      const uint64_t bar = up ? list->bar_up : list->bar_down;
      finish(bar, bars[0]);
      play(bar, bars[1]);
    }
    // The row before (wrapped up: the last; wrapped down: the first; else the
    // neighbor) finished, the option's restarted and looped.
    auto rows = [&](uint64_t first, const uint32_t* lr) {
      const uint64_t before = wrapped ? (up ? count() + first - 1 : first)
                                      : option() + (up ? first + 1 : first - 1);
      finish(before, lr[0]);
      finish(option() + first, lr[1]);
      play(option() + first, lr[2]);
      return loop(option() + first, lr[3]);
    };
    uint64_t r3 = rows(list->rows_a, list->first[path]);
    if (list->rows_b != 0) {
      r3 = rows(list->rows_b, list->second);
    }
    return r3;
  }
  // No step: A (the rows and bars finished) or B (the same, then the page
  // before played).
  uint64_t r3 = found;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, native_slots, &r3);
  };
  if (asked(159, 0x821D5DC4)) {
    const OptionList* list = OptionListOf(base, s);
    if (list == &kTireOptions) {
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 844, 0x821D5DEC);
        finish(k + 783, 0x821D5DF8);
      }
      finish(854, 0x821D5E10);
      return finish(855, 0x821D5EB4);
    }
    if (list == &kBonnetOptions) {
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 675, 0x821D5E3C);
      }
      finish(685, 0x821D5E54);
      return finish(686, 0x821D5EB4);
    }
    if (list == &kDoorOptions) {
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 489, 0x821D5E84);
        finish(k + 428, 0x821D5E90);
      }
      finish(499, 0x821D5EA8);
      return finish(500, 0x821D5EB4);
    }
    return r3;
  }
  if (!asked(158, 0x821D5EDC)) {
    return r3;
  }
  const OptionList* list = OptionListOf(base, s);
  if (list == &kTireOptions) {
    // Back to the tire's levels: the level's rows and its picture (the
    // current choice's, else 629) played, then sub-scene 31 restarted.
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 844, 0x821D5F04);
      finish(k + 783, 0x821D5F10);
    }
    finish(854, 0x821D5F28);
    finish(855, 0x821D5F34);
    auto level = [&] { return uint64_t(Byte(base, s + 9661)); };
    finish(level() + 658, 0x821D5F44);
    play(level() + 658, 0x821D5F64);
    finish(level() + 609, 0x821D5F74);
    play(level() + 609, 0x821D5F94);
    const int32_t choice = int32_t(tuning_screen::GetCurrentChoice(base, s));
    uint64_t picture;
    if (choice == int32_t(level())) {
      finish(level() + 619, 0x821D5FB4);
      picture = level() + 619;
    } else {
      finish(629, 0x821D5FC8);
      picture = 629;
    }
    play(picture, 0x821D5FE4);
    call(tuning_screen::kFinish, {movie, tuning_screen::kAllScenes, 31}, frame, 0x821D5FF4);
    return call(tuning_screen::kPlay, {movie, tuning_screen::kAllScenes, 31, 0, 0, 0, 0}, frame,
                0x821D6074);
  }
  if (list == &kBonnetOptions) {
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 675, 0x821D6020);
    }
    finish(685, 0x821D6038);
    finish(686, 0x821D6044);
    call(tuning_screen::kFinish, {movie, tuning_screen::kAllScenes, 35}, frame, 0x821D6054);
    return call(tuning_screen::kPlay, {movie, tuning_screen::kAllScenes, 35, 0, 0, 0, 0}, frame,
                0x821D6074);
  }
  if (list == &kDoorOptions) {
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 489, 0x821D60A0);
      finish(k + 428, 0x821D60AC);
    }
    finish(499, 0x821D60C4);
    finish(500, 0x821D60D0);
    finish(973, 0x821D60DC);
    return tuning_screen::PlayNamed(call, frame, 0x821D60F8, movie, 973, 0);
  }
  return r3;
}

uint64_t InputOptions(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                      uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821D8160 TuningScreen::InputOptions");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  call(kMenuStep, {Load(base, s + 10040)}, frame, 0x821D8198);
  const int32_t step = int32_t(Load(base, Load(base, s + 10040) + 8));
  uint64_t r3 = 0;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, flags.native_slots, &r3);
  };
  if (step == -1 || step == 1) {
    // (1) The cursor moved: the option picked.
    call(kMenuSoundPlay, {4}, frame, 0x821D84C0);
    const uint32_t menu = Load(base, s + 10040);
    Store(base, s + 9708, 1);
    const uint32_t cursor = Load(base, menu + 0);
    Store(base, s + 9744, cursor);
    guest::StoreU8(base, s + 9664, uint8_t(cursor));
  } else if (asked(159, 0x821D81D4)) {
    // (2) A: the confirm dialog (command 63): equip when owned, else the
    // money short or buy; then Push(8).
    call(kMenuSoundPlay, {0}, frame, 0x821D81E4);
    uint32_t menu;
    if (int32_t(IsPickOwned(base, s)) != 0) {
      const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
      const uint32_t queue = Load(base, s + 10036);
      guest::StoreU8(base, s + 9665, 0);
      Store(base, s + 9652, 11);
      Store(base, s + 9744, cursor);
      guest::StoreU8(base, s + 9664, uint8_t(cursor));
      GoFirstHalf(base, queue, 11);
      menu = Load(base, s + 10040);
      StoreMenu(base, menu, {{0, 0}, {4, 0}, {40, 0}, {48, 0}, {52, 0}, {32, 0}, {12, 0}, {16, 1}});
    } else {
      const int32_t price = int32_t(tuning_screen::GetPrice(base, s));
      const int32_t money = int32_t(Load(base, s + 25116));
      const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
      const uint32_t queue = Load(base, s + 10036);
      Store(base, s + 9744, cursor);
      guest::StoreU8(base, s + 9664, uint8_t(cursor));
      if (money < price) {
        Store(base, s + 9652, 10);
        GoFirstHalf(base, queue, 10);
        menu = Load(base, s + 10040);
        StoreMenu(base, menu,
                  {{12, 0}, {16, 0}, {0, 0}, {4, 0}, {40, 0}, {48, 0}, {52, 0}, {32, 0}});
      } else {
        guest::StoreU8(base, s + 9665, 0);
        Store(base, s + 9652, 9);
        GoFirstHalf(base, queue, 9);
        const uint32_t pick = Byte(base, s + 9665);
        menu = Load(base, s + 10040);
        StoreMenu(base, menu,
                  {{0, pick}, {4, pick}, {44, 0}, {56, 0}, {60, 8}, {12, 0}, {16, 1}});
      }
    }
    StoreMenu(base, menu, {{20, 1}, {8, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
    uint32_t state = 0;
    const uint32_t queue = QueueCurrent(base, s, &state);
    Store(base, queue + 0, 0xFFFFFFFFu);
    Store(base, queue + 4, state);
    Store(base, queue + 16, 0xFFFFFFFFu);
    Store(base, queue + 12, 63);
    Store(base, s + 9708, 1);
    PushWord(base, s, 8);
    guest::StoreU8(base, s + 10008, uint8_t(Byte(base, s + 10008) + 1));
  } else if (asked(158, 0x821D83AC)) {
    // (3) B: Pop (the door marked at +9732), the menu reset to the page's
    // count and cursor; the tire's levels set up again (ShowLevels).
    call(kMenuSoundPlay, {2}, frame, 0x821D83BC);
    if (int32_t(Load(base, s + 9656)) == 5 && Byte(base, s + 9660) == 1) {
      Store(base, s + 9732, 1);
    }
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    Store(base, s + 9708, 1);
    const uint32_t queue = Load(base, s + 10036);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    Store(base, queue + 20, 0xFFFFFFFFu);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 24, 0);
    Store(base, queue + 4, popped);
    Store(base, queue + 0, popped);
    Store(base, queue + 8, popped);
    const uint32_t count = PageItemCount(base, s);
    const uint32_t cursor = PageCursor(base, s);
    StoreMenu(base, Load(base, s + 10040),
              {{12, 0}, {16, count - 1}, {20, 1}, {8, 0}, {0, cursor}, {4, cursor}, {44, 0},
               {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
    if (int32_t(Load(base, s + 9656)) == 3) {
      uint32_t state = 0;
      const uint32_t current = QueueCurrent(base, s, &state);
      Store(base, current + 16, 0xFFFFFFFFu);
      Store(base, current + 4, state);
      Store(base, current + 0, 0xFFFFFFFFu);
      Store(base, current + 12, 3);
    }
  }
  return MoveOption(base, call, s, frame, flags.native_slots);
}

namespace {

// MoveLevel's lists (its row): the short ones (category 0 items 1 to 3,
// category 1 items 0 to 3, category 2: sub-scene 28, bars 499 and 500, rows
// 489 + l and 428 + l), the long ones (category 0 item 0, category 1 item
// 4: the rows +25100 now and +25104 before) and the tire's (category 3:
// sub-scene 30, bars 642 and 643, rows 632 + l); else none.
enum class LevelList { kNone, kShort, kLong, kTire };

LevelList LevelListOf(uint8_t* base, uint32_t screen) {
  const int32_t category = int32_t(Load(base, screen + 9656));
  const uint32_t item = Byte(base, screen + 9660);
  if ((category == 0 && item >= 1 && item <= 3) || (category == 1 && item <= 3) ||
      category == 2) {
    return LevelList::kShort;
  }
  if ((category == 0 && item == 0) || (category == 1 && item == 4)) {
    return LevelList::kLong;
  }
  return category == 3 ? LevelList::kTire : LevelList::kNone;
}

}  // namespace

uint64_t MoveLevel(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                   uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821D1A18 TuningScreen::MoveLevel");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t menu = Load(base, s + 10040);
  const uint32_t movie = Load(base, found);
  const int32_t step = int32_t(Load(base, menu + 8));
  auto level = [&] { return uint64_t(Byte(base, s + 9661)); };
  auto count = [&] { return uint64_t(Byte(base, s + 9677)); };
  auto word = [&](uint32_t offset) { return uint64_t(Load(base, s + offset)); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto loop = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::LoopNamed(call, frame, lr, movie, named);
  };
  // The list's node: sub-scene `sub`'s child 1 (FindNode), its child i.
  auto list_child = [&](uint32_t sub, uint32_t i) {
    return ui_movies::Child(base, ui_movies::FindNode(base, movie, sub, 1), i);
  };
  // The bar snapped: child 0's +16 = float(x) x 40.0 (x read after the
  // lookups).
  auto snap = [&](uint32_t sub, auto x) {
    const uint32_t node = list_child(sub, 0);
    Store(base, node + 16, ItemPosition(base, frame, x()));
  };
  // A row not available dimmed: child i's +68 = the float at 0x82002A3C.
  auto dim = [&](uint32_t sub, uint32_t i) -> uint64_t {
    const uint32_t node = list_child(sub, i);
    Store(base, node + 68, camera_modes::steps::Bits(camera_modes::steps::Lfs(base, 0x82002A3C)));
    return node;
  };
  // IsItemAvailable(screen, l): a leaf that leaves r4 as the original loads
  // it, the level l, which the original reads back after the call (its row):
  // the level read before the call stands for it.
  auto available = [&](uint64_t l) { return tuning_screen::IsItemAvailable(base, s, l) != 0; };
  if (step == -1 || step == 1) {
    const bool up = step == -1;
    const LevelList list = LevelListOf(base, s);
    const bool wrapped = int32_t(Load(base, menu + 40)) != 0;
    auto by_level = [&] { return int64_t(level()); };
    if (list == LevelList::kShort) {
      if (up && wrapped) {
        finish(499, 0x821D1B1C);
        finish(500, 0x821D1B28);
        snap(28, by_level);
        finish(count() + 488, 0x821D1B78);
        finish(level() + 489, 0x821D1B88);
        play(level() + 489, 0x821D1BA8);
        loop(level() + 489, 0x821D1BB8);
        finish(count() + 427, 0x821D1BC8);
        finish(level() + 428, 0x821D1BD8);
        play(level() + 428, 0x821D1BF8);
        return loop(level() + 428, 0x821D1F9C);
      }
      if (!up && wrapped) {
        // The new row snapped to its end (not looped).
        finish(499, 0x821D2088);
        finish(500, 0x821D2094);
        snap(28, by_level);
        finish(489, 0x821D20E0);
        play(level() + 489, 0x821D2100);
        finish(level() + 489, 0x821D2110);
        dim(28, uint32_t(level() + 5));
        finish(428, 0x821D2148);
        play(level() + 428, 0x821D2168);
        return finish(level() + 428, 0x821D1E9C);
      }
      // The bar slid one way: 499 (up) or 500 (down) restarted; the row
      // before (l + 1 up, l - 1 down) finished, the level's restarted and
      // looped, or with the level not available finished and the row
      // before's mark dimmed.
      const uint64_t bar = up ? 499 : 500;
      finish(bar, up ? 0x821D1C08 : 0x821D2174);
      play(bar, up ? 0x821D1C24 : 0x821D2190);
      const uint64_t l = level();
      const bool on = available(l);
      const uint64_t before = up ? l + 490 : l + 488;
      if (on) {
        finish(before, up ? 0x821D1C44 : 0x821D21B0);
        finish(level() + 489, up ? 0x821D1C54 : 0x821D21C0);
        play(level() + 489, up ? 0x821D1C74 : 0x821D21E0);
        loop(level() + 489, up ? 0x821D1C84 : 0x821D21F0);
        finish(level() + (up ? 429 : 427), 0x821D1BC8);
        finish(level() + 428, 0x821D1BD8);
        play(level() + 428, 0x821D1BF8);
        return loop(level() + 428, 0x821D1F9C);
      }
      finish(before, up ? 0x821D1C94 : 0x821D21FC);
      finish(level() + 489, up ? 0x821D1CA4 : 0x821D220C);
      dim(28, uint32_t(level() + (up ? 6 : 4)));
      finish(level() + (up ? 429 : 427), 0x821D1CE0);
      return finish(level() + 428, 0x821D1E9C);
    }
    if (list == LevelList::kTire) {
      if (up && wrapped) {
        finish(642, 0x821D1EC0);
        finish(643, 0x821D1ECC);
        snap(30, by_level);
        finish(count() + 631, 0x821D1F5C);
        finish(level() + 632, 0x821D1F6C);
        play(level() + 632, 0x821D1F8C);
        return loop(level() + 632, 0x821D1F9C);
      }
      if (!up && wrapped) {
        finish(642, 0x821D2380);
        finish(643, 0x821D238C);
        snap(30, by_level);
        finish(632, 0x821D23D8);
        play(level() + 632, 0x821D23F8);
        finish(level() + 632, 0x821D2408);
        return dim(30, uint32_t(level() + 5));
      }
      const uint64_t bar = up ? 642 : 643;
      finish(bar, up ? 0x821D1F20 : 0x821D242C);
      play(bar, up ? 0x821D1F3C : 0x821D2448);
      const uint64_t l = level();
      const bool on = available(l);
      const uint64_t before = up ? l + 633 : l + 631;
      if (on) {
        finish(before, 0x821D1F5C);
        finish(level() + 632, 0x821D1F6C);
        play(level() + 632, 0x821D1F8C);
        return loop(level() + 632, 0x821D1F9C);
      }
      finish(before, up ? 0x821D1FA4 : 0x821D2468);
      finish(level() + 632, up ? 0x821D1FB4 : 0x821D2478);
      return dim(30, uint32_t(level() + (up ? 6 : 4)));
    }
    if (list != LevelList::kLong || count() <= 8) {
      return found;
    }
    // The long list: the bar snapped at the wraps (7 to 0 up, 0 to 7 down),
    // else (with +9708 set) slid inside the visible rows; then the rows.
    const int32_t before = int32_t(Load(base, s + 25104));
    const int32_t now = int32_t(Load(base, s + 25100));
    if (up ? (before == 7 && now == 0) : (before == 0 && now == 7)) {
      finish(499, up ? 0x821D1D40 : 0x821D229C);
      finish(500, up ? 0x821D1D4C : 0x821D22A8);
      snap(28, [&] { return int64_t(int32_t(Load(base, s + 25100))); });
    } else {
      if (Load(base, s + 9708) == 0) {
        return found;
      }
      if (up ? (before > 0 && now >= 0 && now < 7) : (before < 7 && now <= 7 && now > 0)) {
        finish(up ? 499 : 500, up ? 0x821D1E34 : 0x821D2340);
        play(up ? 499 : 500, 0x821D235C);
      }
    }
    if (available(level())) {
      finish(word(25104) + 489, 0x821D1D78);
      finish(word(25100) + 489, 0x821D1D88);
      play(word(25100) + 489, 0x821D1DA8);
      loop(word(25100) + 489, 0x821D1DB8);
      finish(word(25104) + 428, 0x821D1DC8);
      finish(word(25100) + 428, 0x821D1DD8);
      play(word(25100) + 428, 0x821D1DF8);
      return loop(word(25100) + 428, 0x821D1F9C);
    }
    finish(word(25104) + 489, 0x821D1E40);
    finish(word(25100) + 489, 0x821D1E50);
    dim(28, Load(base, s + 25104) + 5);
    finish(word(25104) + 428, 0x821D1E8C);
    return finish(word(25100) + 428, 0x821D1E9C);
  }
  // No step: A (with the level available: the rows and bars finished, then
  // the type or option list's level pictures played) or B (the same, then
  // the page before played) asked again.
  uint64_t r3 = found;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, native_slots, &r3);
  };
  // The list's rows and bars finished (k 0 to 9); the last call's r3, else
  // `r3`.
  struct Finishes {
    uint32_t short_rows[2], long_rows[2], short_bar, tire_rows[2], tire_bar, last;
  };
  auto finish_all = [&](const Finishes& lr, uint64_t r3_in) {
    const LevelList list = LevelListOf(base, s);
    if (list == LevelList::kShort || list == LevelList::kLong) {
      const uint32_t* rows = list == LevelList::kShort ? lr.short_rows : lr.long_rows;
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 489, rows[0]);
        finish(k + 428, rows[1]);
      }
      finish(499, lr.short_bar);
      return finish(500, lr.last);
    }
    if (list == LevelList::kTire) {
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 632, lr.tire_rows[0]);
        finish(k + 571, lr.tire_rows[1]);
      }
      finish(642, lr.tire_bar);
      return finish(643, lr.last);
    }
    return r3_in;
  };
  if (asked(159, 0x821D24C8)) {
    const uint32_t on = tuning_screen::IsItemAvailable(base, s, level());
    if (on == 0) {
      return on;
    }
    r3 = finish_all({{0x821D2590, 0x821D259C},
                     {0x821D25F8, 0x821D2604},
                     0x821D25B4,
                     {0x821D2630, 0x821D263C},
                     0x821D2654,
                     0x821D2660},
                    on);
    if (count() > 8) {
      r3 = finish(1013, 0x821D2678);
    }
    const int32_t category = int32_t(Load(base, s + 9656));
    const uint32_t item = Byte(base, s + 9660);
    // The level's pictures: the type list's (501, 444) for the levels that
    // open one, the option list's (644, 587) for the tire's; the current
    // choice's picture (455 or 598 + l), else 465 or 608.
    uint64_t first, second, chosen, other;
    const uint32_t* lrs;
    if ((category == 0 && item == 2) || (category == 1 && item == 3 && level() >= 1)) {
      first = 501, second = 444, chosen = 455, other = 465;
      static constexpr uint32_t kLrs[6] = {0x821D26C0, 0x821D26E0, 0x821D26F0,
                                           0x821D2710, 0x821D2730, 0x821D2744};
      lrs = kLrs;
    } else if (category == 3 && level() != 0) {
      first = 644, second = 587, chosen = 598, other = 608;
      static constexpr uint32_t kLrs[6] = {0x821D276C, 0x821D278C, 0x821D279C,
                                           0x821D27BC, 0x821D27DC, 0x821D27F0};
      lrs = kLrs;
    } else {
      return r3;
    }
    finish(level() + first, lrs[0]);
    play(level() + first, lrs[1]);
    finish(level() + second, lrs[2]);
    play(level() + second, lrs[3]);
    const int32_t choice = int32_t(tuning_screen::GetCurrentChoice(base, s));
    uint64_t picture;
    if (choice == int32_t(level())) {
      picture = level() + chosen;
      finish(picture, lrs[4]);
    } else {
      picture = other;
      finish(picture, lrs[5]);
    }
    return tuning_screen::PlayNamed(call, frame, 0x821D2A78, movie, picture, 0);
  }
  if (!asked(158, 0x821D281C)) {
    return r3;
  }
  r3 = finish_all({{0x821D28D0, 0x821D28DC},
                   {0x821D2938, 0x821D2944},
                   0x821D28F4,
                   {0x821D2970, 0x821D297C},
                   0x821D2994,
                   0x821D29A0},
                  r3);
  if (count() > 8) {
    r3 = finish(1013, 0x821D29B8);
  }
  if (count() > 8) {
    r3 = call(tuning_screen::kFinish, {movie, tuning_screen::kAllScenes, 15}, frame, 0x821D29D4);
  }
  if (int32_t(Load(base, s + 9656)) == 3) {
    const uint64_t item = Byte(base, s + 9660);
    finish(item + 415, 0x821D29F0);
    r3 = play(uint64_t(Byte(base, s + 9660)) + 415, 0x821D2A10);
  }
  // Back to the items: 972 (category 0), 973 (1 and 2) or 974 (3).
  const uint32_t category = Load(base, s + 9656);
  uint64_t back;
  uint32_t lr;
  if (category < 1) {
    back = 972, lr = 0x821D2A5C;
  } else if (category < 3) {
    back = 973, lr = 0x821D2A48;
  } else if (category == 3) {
    back = 974, lr = 0x821D2A34;
  } else {
    return r3;
  }
  finish(back, lr);
  return tuning_screen::PlayNamed(call, frame, 0x821D2A78, movie, back, 0);
}

namespace {

// MoveType's lists (its row): G1 (category 0 item 2: sub-scene 29's node 0
// itself, bars 542 and 543, rows 532 + t), G2 (category 4, category 5 item
// 3, category 6 items 0 and 1: sub-scene 28, bars 499 and 500, rows 489 + t
// and 428 + t, with the type's availability) and G3 (category 5 item 2,
// category 6 item 2: as G2 with rows 489 + t only); else none.
enum class TypeList { kNone, kG1, kG2, kG3 };

constexpr uint32_t kMoveAnimation = 0x8211DBE0;  // UiMovie::MoveAnimation(movie, sub, index, to)

TypeList TypeListOf(uint8_t* base, uint32_t screen) {
  const int32_t category = int32_t(Load(base, screen + 9656));
  const uint32_t item = Byte(base, screen + 9660);
  if (category == 0 && item == 2) {
    return TypeList::kG1;
  }
  if (category == 4 || (category == 5 && item == 3) || (category == 6 && item <= 1)) {
    return TypeList::kG2;
  }
  if ((category == 5 || category == 6) && item == 2) {
    return TypeList::kG3;
  }
  return TypeList::kNone;
}

}  // namespace

uint64_t MoveType(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                  uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821D32C8 TuningScreen::MoveType");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t menu = Load(base, s + 10040);
  const uint32_t movie = Load(base, found);
  const int32_t step = int32_t(Load(base, menu + 8));
  auto type = [&] { return uint64_t(Byte(base, s + 9662)); };
  auto count = [&] { return uint64_t(Byte(base, s + 9678)); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto loop = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::LoopNamed(call, frame, lr, movie, named);
  };
  // The bar snapped: the list's node +16 = float(t) x 40.0 (t read after the
  // lookups): G1's sub-scene 29's node 0 itself, the others' sub-scene 28's
  // node 1's child 0.
  auto snap = [&](bool g1) {
    const uint32_t node =
        g1 ? ui_movies::FindNode(base, movie, 29, 0)
           : ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1), 0);
    Store(base, node + 16, ItemPosition(base, frame, int64_t(type())));
  };
  // A row not available dimmed: sub-scene 28's node 1's child i's +68 = the
  // float at 0x82002A3C.
  auto dim = [&](uint32_t i) {
    const uint32_t node = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1), i);
    Store(base, node + 68, camera_modes::steps::Bits(camera_modes::steps::Lfs(base, 0x82002A3C)));
  };
  if (step == -1 || step == 1) {
    const bool up = step == -1;
    const TypeList list = TypeListOf(base, s);
    if (list == TypeList::kNone) {
      return found;
    }
    const bool wrapped = int32_t(Load(base, menu + 40)) != 0;
    if (list == TypeList::kG1) {
      if (wrapped) {
        finish(542, up ? 0x821D334C : 0x821D3768);
        finish(543, up ? 0x821D3358 : 0x821D3774);
        snap(true);
        finish(up ? count() + 531 : 532, 0x821D33A0);
      } else {
        const uint64_t bar = up ? 542 : 543;
        finish(bar, up ? 0x821D33E0 : 0x821D37BC);
        play(bar, up ? 0x821D33FC : 0x821D37D8);
        finish(type() + (up ? 533 : 531), 0x821D33A0);
      }
      finish(type() + 532, 0x821D33B0);
      play(type() + 532, 0x821D33D0);
      return loop(type() + 532, 0x821D36E0);
    }
    if (list == TypeList::kG3) {
      if (wrapped) {
        finish(499, up ? 0x821D348C : 0x821D3864);
        finish(500, up ? 0x821D3498 : 0x821D3870);
        snap(false);
        finish(up ? count() + 488 : 489, 0x821D34E8);
      } else {
        const uint64_t bar = up ? 499 : 500;
        finish(bar, up ? 0x821D3528 : 0x821D38C0);
        play(bar, up ? 0x821D3544 : 0x821D38DC);
        finish(type() + (up ? 490 : 488), 0x821D34E8);
      }
      finish(type() + 489, 0x821D34F8);
      play(type() + 489, 0x821D3518);
      return loop(type() + 489, 0x821D36E0);
    }
    // G2: the 489 and 428 rows.
    if (wrapped) {
      finish(499, up ? 0x821D3568 : 0x821D38FC);
      finish(500, up ? 0x821D3574 : 0x821D3908);
      snap(false);
      finish(up ? count() + 488 : 489, up ? 0x821D35C4 : 0x821D3954);
      finish(type() + 489, up ? 0x821D35D4 : 0x821D3964);
      play(type() + 489, up ? 0x821D35F4 : 0x821D3984);
      loop(type() + 489, up ? 0x821D3604 : 0x821D3994);
      finish(up ? count() + 427 : 428, 0x821D36A0);
      finish(type() + 428, 0x821D36B0);
      play(type() + 428, 0x821D36D0);
      return loop(type() + 428, 0x821D36E0);
    }
    const uint64_t bar = up ? 499 : 500;
    finish(bar, up ? 0x821D3614 : 0x821D39A4);
    play(bar, up ? 0x821D3630 : 0x821D39C0);
    // IsItemAvailable(screen, t): a leaf that leaves r4 as the original
    // loads it, the type t, which the original reads back after the call
    // (its row): the type read before the call stands for it.
    const uint64_t t = type();
    const bool on = tuning_screen::IsItemAvailable(base, s, t) != 0;
    const uint64_t before = up ? t + 490 : t + 488;
    if (on) {
      finish(before, up ? 0x821D3650 : 0x821D39E0);
      finish(type() + 489, up ? 0x821D3660 : 0x821D39F0);
      play(type() + 489, up ? 0x821D3680 : 0x821D3A10);
      loop(type() + 489, up ? 0x821D3690 : 0x821D3A20);
      finish(type() + (up ? 429 : 427), 0x821D36A0);
      finish(type() + 428, 0x821D36B0);
      play(type() + 428, 0x821D36D0);
      return loop(type() + 428, 0x821D36E0);
    }
    finish(before, up ? 0x821D36E8 : 0x821D3A2C);
    finish(type() + 489, up ? 0x821D36F8 : 0x821D3A3C);
    dim(uint32_t(type() + (up ? 6 : 4)));
    finish(type() + (up ? 429 : 427), 0x821D3A78);
    return finish(type() + 428, 0x821D3A88);
  }
  // No step: A (with the level, byte +9661, available: the rows and bars
  // finished, then the option or type pictures played) or B (the same, then
  // the page before played) asked again.
  uint64_t r3 = found;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, native_slots, &r3);
  };
  // The 489 and 428 rows (k 0 to 9) and the bars finished.
  auto finish_rows = [&](uint32_t lr489, uint32_t lr428, uint32_t lr499, uint32_t lr500) {
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 489, lr489);
      finish(k + 428, lr428);
    }
    finish(499, lr499);
    return finish(500, lr500);
  };
  // The type's pictures (501 and 444 + t) restarted, then the type's or
  // 465 played unchained by whether `current` is the type.
  auto pictures = [&](uint32_t lr0, uint32_t lr1, uint32_t lr2, uint32_t lr3, auto current) {
    finish(type() + 501, lr0);
    play(type() + 501, lr1);
    finish(type() + 444, lr2);
    play(type() + 444, lr3);
    const int32_t chosen = int32_t(current());
    uint64_t picture;
    if (chosen == int32_t(type())) {
      finish(type() + 455, 0x821D3C1C);
      picture = type() + 455;
    } else {
      finish(465, 0x821D3C30);
      picture = 465;
    }
    return tuning_screen::PlayNamed(call, frame, 0x821D4178, movie, picture, 0);
  };
  auto category = [&] { return int32_t(Load(base, s + 9656)); };
  auto item = [&] { return Byte(base, s + 9660); };
  if (asked(159, 0x821D3AB0)) {
    const uint32_t on = tuning_screen::IsItemAvailable(base, s, Byte(base, s + 9661));
    if (on == 0) {
      return on;
    }
    const int32_t c = category();
    if ((c == 0 && item() == 2) || (c == 1 && item() == 3)) {
      // The loops of G1 and G3 count in r30, the screen's register; nothing
      // reads the screen after them.
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 532, 0x821D3B08);
      }
      finish(542, 0x821D3B20);
      return finish(543, 0x821D3A88);
    }
    if (c == 4) {
      r3 = finish_rows(0x821D3B44, 0x821D3B50, 0x821D3B68, 0x821D3B74);
      if (count() > 8) {
        r3 = finish(1013, 0x821D3B8C);
      }
      if (item() != 4) {
        return r3;
      }
      // The word +36368 (not GetCurrentChoice's +36372).
      return pictures(0x821D3BA8, 0x821D3BC8, 0x821D3BD8, 0x821D3BF8,
                      [&] { return Load(base, s + 36368); });
    }
    if ((c == 5 || c == 6) && item() == 2) {
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 489, 0x821D3C70);
      }
      finish(499, 0x821D3C88);
      return finish(500, 0x821D3A88);
    }
    if (!((c == 5 && item() == 3) || (c == 6 && item() <= 1))) {
      return on;
    }
    r3 = finish_rows(0x821D3CE0, 0x821D3CEC, 0x821D3D04, 0x821D3D10);
    if (category() != 6 || item() != 0) {
      return r3;
    }
    return pictures(0x821D3D38, 0x821D3D58, 0x821D3D68, 0x821D3D88,
                    [&] { return tuning_screen::GetCurrentChoice(base, s); });
  }
  if (!asked(158, 0x821D3DC0)) {
    return r3;
  }
  const int32_t c = category();
  if (c == 0 && item() == 2) {
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 532, 0x821D3DF0);
    }
    finish(542, 0x821D3E08);
    finish(543, 0x821D3E14);
    // The level's pictures (515 and 466 + l), then the level's or 486.
    auto level = [&] { return uint64_t(Byte(base, s + 9661)); };
    finish(level() + 515, 0x821D3E24);
    play(level() + 515, 0x821D3E44);
    finish(level() + 466, 0x821D3E54);
    play(level() + 466, 0x821D3E74);
    const int32_t chosen = int32_t(tuning_screen::GetCurrentChoice(base, s));
    uint64_t picture;
    if (chosen == int32_t(level())) {
      finish(level() + 476, 0x821D3E94);
      picture = level() + 476;
    } else {
      finish(486, 0x821D3EA8);
      picture = 486;
    }
    r3 = play(picture, 0x821D3EC4);
    if (category() != 0 || item() != 2) {
      return r3;
    }
    finish(990, 0x821D3EE8);
    return tuning_screen::PlayNamed(call, frame, 0x821D4178, movie, 990, 0);
  }
  if (c == 4) {
    finish_rows(0x821D3F0C, 0x821D3F18, 0x821D3F30, 0x821D3F3C);
    if (count() > 8) {
      call(tuning_screen::kFinish, {movie, tuning_screen::kAllScenes, 15}, frame, 0x821D3F58);
      call(tuning_screen::kPlay, {movie, tuning_screen::kAllScenes, 15, 1, 0, 0, 0}, frame,
           0x821D3F78);
      call(kMoveAnimation, {movie, tuning_screen::kAllScenes, 15, 1}, frame, 0x821D3F8C);
    }
    const uint64_t back = item() == 4 ? 975 : 973;
    finish(back, back == 975 ? 0x821D3FA4 : 0x821D3FB4);
    play(back, 0x821D3FD0);
    finish(1012, 0x821D3FDC);
    return tuning_screen::PlayNamed(call, frame, 0x821D4178, movie, 1012, 0);
  }
  if ((c == 5 || c == 6) && item() == 2) {
    for (uint64_t k = 0; k < 10; k++) {
      finish(k + 489, 0x821D401C);
    }
    finish(499, 0x821D4034);
    finish(500, 0x821D4040);
    finish(975, 0x821D404C);
    return tuning_screen::PlayNamed(call, frame, 0x821D4178, movie, 975, 0);
  }
  if (!((c == 5 && item() == 3) || (c == 6 && item() <= 1))) {
    return r3;
  }
  finish_rows(0x821D40A4, 0x821D40B0, 0x821D40C8, 0x821D40D4);
  if (category() == 6 && item() == 0) {
    finish(973, 0x821D40F8);
    play(973, 0x821D4114);
    finish(1012, 0x821D4120);
    tuning_screen::PlayNamed(call, frame, 0x821D413C, movie, 1012, 0);
    finish(1, 0x821D4148);
    return tuning_screen::PlayNamed(call, frame, 0x821D4178, movie, 1, 0);
  }
  finish(973, 0x821D415C);
  return tuning_screen::PlayNamed(call, frame, 0x821D4178, movie, 973, 0);
}

uint64_t InputLevels(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                     uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821D6E20 TuningScreen::InputLevels");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  {
    // The 10-level lists scroll.
    const int32_t category = int32_t(Load(base, s + 9656));
    const uint32_t menu = Load(base, s + 10040);
    if ((category == 0 && Byte(base, s + 9660) == 0) ||
        (category == 1 && Byte(base, s + 9660) == 4)) {
      StepList(base, call, menu, frame, flags.native_slots);
    } else {
      call(kMenuStep, {menu}, frame, 0x821D6E90);
    }
  }
  const int32_t step = int32_t(Load(base, Load(base, s + 10040) + 8));
  uint64_t r3 = 0;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, flags.native_slots, &r3);
  };
  if (step == -1 || step == 1) {
    // (1) The cursor moved: the level picked, the long list's rows (the one
    // before and now).
    call(kMenuSoundPlay, {4}, frame, 0x821D7454);
    const uint32_t menu = Load(base, s + 10040);
    const uint32_t now = Load(base, s + 25100);
    Store(base, s + 9708, 1);
    const uint32_t cursor = Load(base, menu + 0);
    Store(base, s + 25104, now);
    Store(base, s + 9744, cursor);
    guest::StoreU8(base, s + 9661, uint8_t(cursor));
    Store(base, s + 25100, Load(base, menu + 48));
  } else if (asked(159, 0x821D6ECC)) {
    // (2) A: nothing but the refusal when the level isn't available; else
    // the type list, the LSD's types, the tire's options or the confirm
    // dialog (equip when owned, else the money short or buy); then Push(5).
    if (int32_t(tuning_screen::IsItemAvailable(base, s, Byte(base, s + 9661))) == 0) {
      call(kMenuSoundPlay, {3}, frame, 0x821D731C);
      return MoveLevel(base, call, s, frame, flags.native_slots);
    }
    call(kMenuSoundPlay, {0}, frame, 0x821D6EF0);
    const int32_t category = int32_t(Load(base, s + 9656));
    if (category == 0 && Byte(base, s + 9660) == 2) {
      // The type list (Go(6, 4), ShowTypes).
      const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
      const uint32_t queue = Load(base, s + 10036);
      Store(base, s + 9652, 6);
      guest::StoreU8(base, s + 9662, 0);
      guest::StoreU8(base, s + 9678, 5);
      Store(base, s + 9744, cursor);
      guest::StoreU8(base, s + 9661, uint8_t(cursor));
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      GoFirstHalf(base, queue, 6);
      const uint32_t last = Byte(base, s + 9678) - 1;
      const uint32_t pick = Byte(base, s + 9662);
      StoreMenu(base, Load(base, s + 10040),
                {{12, 0}, {20, 1}, {8, 0}, {44, 0}, {0, pick}, {16, last}, {4, pick},
                 {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
      GoTail(base, s, 4);
    } else if ((category == 1 && Byte(base, s + 9660) == 3 && Byte(base, s + 9661) >= 1) ||
               (category == 3 && Byte(base, s + 9661) != 0)) {
      // The LSD's types (Go(7, 5)) or the tire's options (Go(8, 6),
      // ShowOptions).
      const bool lsd = category == 1;
      const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
      const uint32_t queue = Load(base, s + 10036);
      const uint32_t state = lsd ? 7 : 8;
      Store(base, s + 9652, state);
      if (lsd) {
        guest::StoreU8(base, s + 9663, 0);
        guest::StoreU8(base, s + 9679, 5);
      } else {
        guest::StoreU8(base, s + 9680, 3);
        guest::StoreU8(base, s + 9664, 0);
      }
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      Store(base, s + 9744, cursor);
      guest::StoreU8(base, s + 9661, uint8_t(cursor));
      GoFirstHalf(base, queue, state);
      const uint32_t last = Byte(base, s + (lsd ? 9679 : 9680)) - 1;
      StoreMenu(base, Load(base, s + 10040),
                {{12, 0}, {20, 1}, {0, 0}, {4, 0}, {8, 0}, {40, 0}, {16, last}, {48, 0},
                 {52, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}});
      GoTail(base, s, lsd ? 5 : 6);
    } else {
      // The confirm dialog (command 63).
      uint32_t menu;
      if (int32_t(IsPickOwned(base, s)) != 0) {
        const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
        const uint32_t queue = Load(base, s + 10036);
        guest::StoreU8(base, s + 9665, 0);
        Store(base, s + 9652, 11);
        Store(base, s + 9744, cursor);
        guest::StoreU8(base, s + 9661, uint8_t(cursor));
        GoFirstHalf(base, queue, 11);
        menu = Load(base, s + 10040);
        StoreMenu(base, menu,
                  {{0, 0}, {4, 0}, {40, 0}, {48, 0}, {52, 0}, {32, 0}, {12, 0}, {16, 1}});
      } else {
        const int32_t price = int32_t(tuning_screen::GetPrice(base, s));
        const int32_t money = int32_t(Load(base, s + 25116));
        const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
        const uint32_t queue = Load(base, s + 10036);
        Store(base, s + 9744, cursor);
        guest::StoreU8(base, s + 9661, uint8_t(cursor));
        if (money < price) {
          Store(base, s + 9652, 10);
          GoFirstHalf(base, queue, 10);
          menu = Load(base, s + 10040);
          StoreMenu(base, menu,
                    {{12, 0}, {16, 0}, {0, 0}, {4, 0}, {40, 0}, {48, 0}, {52, 0}, {32, 0}});
        } else {
          guest::StoreU8(base, s + 9665, 0);
          Store(base, s + 9652, 9);
          GoFirstHalf(base, queue, 9);
          const uint32_t pick = Byte(base, s + 9665);
          menu = Load(base, s + 10040);
          StoreMenu(base, menu,
                    {{0, pick}, {4, pick}, {44, 0}, {56, 0}, {60, 8}, {12, 0}, {16, 1}});
        }
      }
      StoreMenu(base, menu, {{20, 1}, {8, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
      GoTail(base, s, 63);
    }
    Store(base, s + 9708, 1);
    PushWord(base, s, 5);
    guest::StoreU8(base, s + 10008, uint8_t(Byte(base, s + 10008) + 1));
  } else if (asked(158, 0x821D7344)) {
    // (3) B: Pop (the stand and garage car rewritten: +9732), the menu reset
    // to the page's count and cursor, PlayIntro for category 2, else
    // OpenItems.
    call(kMenuSoundPlay, {2}, frame, 0x821D7354);
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    const uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9732, 1);
    Store(base, s + 9708, 1);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    Store(base, queue + 20, 0xFFFFFFFFu);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 24, 0);
    Store(base, queue + 4, popped);
    Store(base, queue + 0, popped);
    Store(base, queue + 8, popped);
    const uint32_t count = PageItemCount(base, s);
    const uint32_t cursor = PageCursor(base, s);
    StoreMenu(base, Load(base, s + 10040),
              {{12, 0}, {16, count - 1}, {20, 1}, {8, 0}, {0, cursor}, {4, cursor}, {44, 0},
               {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
    GoTail(base, s, int32_t(Load(base, s + 9656)) == 2 ? 0 : 2);
  }
  return MoveLevel(base, call, s, frame, flags.native_slots);
}

uint64_t InputTypes(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                    uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821D74A0 TuningScreen::InputTypes");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  {
    // Menu::Step only on the lists that step (else the menu is left as it
    // was).
    const int32_t category = int32_t(Load(base, s + 9656));
    const uint32_t item = Byte(base, s + 9660);
    if ((category == 0 && item == 2) || (category == 4 && item <= 9) ||
        (category == 5 && item >= 2 && item <= 3) || (category == 6 && item <= 2)) {
      call(kMenuStep, {Load(base, s + 10040)}, frame, 0x821D7550);
    }
  }
  const int32_t step = int32_t(Load(base, Load(base, s + 10040) + 8));
  uint64_t r3 = 0;
  auto asked = [&](uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, 40, action, frame, lr, flags.native_slots, &r3);
  };
  if (step == -1 || step == 1) {
    // (1) The cursor moved: the type picked, the long list's rows (the one
    // before and now).
    call(kMenuSoundPlay, {4}, frame, 0x821D7C48);
    const uint32_t menu = Load(base, s + 10040);
    const uint32_t before = Load(base, s + 25108);
    Store(base, s + 9708, 1);
    const uint32_t cursor = Load(base, menu + 0);
    Store(base, s + 25112, before);
    Store(base, s + 9744, cursor);
    guest::StoreU8(base, s + 9662, uint8_t(cursor));
    Store(base, s + 25108, Load(base, menu + 48));
  } else if (asked(157, 0x821D758C)) {
    // (2) Action 157: the horn's sound for types 0 to 3 (category 5, item
    // 3); anything else nothing (A and B aren't asked).
    const uint32_t type = Byte(base, s + 9662);
    if (int32_t(Load(base, s + 9656)) == 5 && Byte(base, s + 9660) == 3 && type <= 3) {
      call(kMenuSoundPlay, {16 + type}, frame, 0x821D79C4);
    }
  } else if (asked(159, 0x821D760C)) {
    // (3) A: nothing but the refusal when the type isn't available; else
    // the type's list (Go(7, 5)), the dialog of state 14, or the confirm
    // dialog (equip when owned, else the money short or buy); then Push(6).
    if (int32_t(tuning_screen::IsItemAvailable(base, s, Byte(base, s + 9662))) == 0) {
      call(kMenuSoundPlay, {3}, frame, 0x821D79C4);
     
  return MoveType(base, call, s, frame, flags.native_slots);
    }
    call(kMenuSoundPlay, {0}, frame, 0x821D7630);
    const int32_t category = int32_t(Load(base, s + 9656));
    const uint32_t item = Byte(base, s + 9660);
    const bool bonnet = category == 4 && item == 4 && Byte(base, s + 9662) != 0;
    const bool interior = category == 6 && item == 0 && Byte(base, s + 9662) >= 1 &&
                          Byte(base, s + 9662) <= 4;
    if (bonnet || interior) {
      // The type's list (Go(7, 5)): its length by the new type (bonnet 2;
      // interior types 1, 2 and 4: 4, 3: 3; any other new type leaves it).
      const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
      guest::StoreU8(base, s + 9663, 0);
      Store(base, s + 9744, cursor);
      guest::StoreU8(base, s + 9662, uint8_t(cursor));
      if (category == 4 && item == 4) {
        guest::StoreU8(base, s + 9679, 2);
      } else {
        const uint32_t type = Byte(base, s + 9662);
        if (type == 1 || type == 2 || type == 4) {
          guest::StoreU8(base, s + 9679, 4);
        } else if (type == 3) {
          guest::StoreU8(base, s + 9679, 3);
        }
      }
      const uint32_t queue = Load(base, s + 10036);
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      Store(base, s + 9652, 7);
      GoFirstHalf(base, queue, 7);
      const uint32_t last = Byte(base, s + 9679) - 1;
      StoreMenu(base, Load(base, s + 10040),
                {{12, 0}, {20, 1}, {0, 0}, {4, 0}, {8, 0}, {40, 0}, {16, last}, {48, 0},
                 {52, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}});
      GoTail(base, s, 5);
    } else if (category == 4 &&
               ((item == 3 && int32_t(Load(base, s + 25172)) == 1 &&
                 Byte(base, s + 9662) == 1) ||
                (item == 9 && Byte(base, s + 9662) == 0))) {
      // State 14 with the dialog (command 63): no menu reset, the type not
      // taken from the menu.
      const uint32_t queue = Load(base, s + 10036);
      Store(base, s + 9652, 14);
      Store(base, s + 9672, 0);
      GoFirstHalf(base, queue, 14);
      GoTail(base, s, 63);
    } else {
      // The confirm dialog (command 63).
      uint32_t menu;
      if (int32_t(IsPickOwned(base, s)) != 0) {
        const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
        const uint32_t queue = Load(base, s + 10036);
        guest::StoreU8(base, s + 9665, 0);
        Store(base, s + 9652, 11);
        Store(base, s + 9744, cursor);
        guest::StoreU8(base, s + 9662, uint8_t(cursor));
        GoFirstHalf(base, queue, 11);
        menu = Load(base, s + 10040);
        StoreMenu(base, menu, {{0, 0}, {4, 0}, {40, 0}, {48, 0}, {52, 0}, {32, 0}});
        StoreMenu(base, menu,
                  {{28, 0}, {24, 0}, {64, 0}, {36, 0}, {8, 0}, {20, 1}, {16, 1}, {12, 0}});
      } else {
        const int32_t price = int32_t(tuning_screen::GetPrice(base, s));
        const int32_t money = int32_t(Load(base, s + 25116));
        const uint32_t cursor = Load(base, Load(base, s + 10040) + 0);
        const uint32_t queue = Load(base, s + 10036);
        Store(base, s + 9744, cursor);
        guest::StoreU8(base, s + 9662, uint8_t(cursor));
        if (money < price) {
          Store(base, s + 9652, 10);
          GoFirstHalf(base, queue, 10);
          menu = Load(base, s + 10040);
          StoreMenu(base, menu,
                    {{16, 0}, {20, 1}, {0, 0}, {4, 0}, {8, 0}, {40, 0}, {48, 0}, {52, 0},
                     {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}, {12, 0}});
        } else {
          guest::StoreU8(base, s + 9665, 0);
          Store(base, s + 9652, 9);
          GoFirstHalf(base, queue, 9);
          const uint32_t pick = Byte(base, s + 9665);
          menu = Load(base, s + 10040);
          StoreMenu(base, menu, {{0, pick}, {4, pick}, {44, 0}, {56, 0}, {60, 8}});
          StoreMenu(base, menu,
                    {{28, 0}, {24, 0}, {64, 0}, {36, 0}, {8, 0}, {20, 1}, {16, 1}, {12, 0}});
        }
      }
      GoTail(base, s, 63);
    }
    Store(base, s + 9708, 1);
    PushWord(base, s, 6);
    guest::StoreU8(base, s + 10008, uint8_t(Byte(base, s + 10008) + 1));
  } else if (asked(158, 0x821D79EC)) {
    // (4) B: Pop (the stand and garage car rewritten: +9732), then by the
    // list: the menu reset to the page's count and cursor with ShowLevels
    // (category 0, item 2) or OpenItems (category 4, with the aero rows put
    // back for items 0 to 4; category 5 items 2, 3; category 6 items 0 to
    // 2); any other list leaves the popped state with no command.
    call(kMenuSoundPlay, {2}, frame, 0x821D79FC);
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    const uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9732, 1);
    Store(base, s + 9708, 1);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    Store(base, queue + 20, 0xFFFFFFFFu);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 24, 0);
    Store(base, queue + 4, popped);
    Store(base, queue + 0, popped);
    Store(base, queue + 8, popped);
    const int32_t category = int32_t(Load(base, s + 9656));
    uint32_t command = 0xFFFFFFFFu;
    if (category == 0 && Byte(base, s + 9660) == 2) {
      command = 3;
    } else if (category == 4) {
      command = 2;
    } else if ((category == 5 && Byte(base, s + 9660) >= 2 && Byte(base, s + 9660) <= 3) ||
               (category == 6 && Byte(base, s + 9660) <= 2)) {
      command = 2;
    }
    if (command != 0xFFFFFFFFu) {
      const uint32_t count = PageItemCount(base, s);
      const uint32_t cursor = PageCursor(base, s);
      StoreMenu(base, Load(base, s + 10040),
                {{12, 0}, {16, count - 1}, {20, 1}, {8, 0}, {0, cursor}, {4, cursor}, {44, 0},
                 {56, 0}, {60, 8}, {36, 0}, {64, 0}, {24, 0}, {28, 0}});
      GoTail(base, s, command);
      if (category == 4 && Byte(base, s + 9660) <= 4) {
        Store(base, Load(base, s + 10040) + 48, Load(base, s + 25100));
        Store(base, Load(base, s + 10040) + 52, Load(base, s + 25104));
      }
    }
  }
  return MoveType(base, call, s, frame, flags.native_slots);
}


namespace {

// Bit n of the bitset at `set` (word n >> 5's 1 << (n & 31)) set; the word
// as it was (the originals' r3).
uint32_t SetBit(uint8_t* base, uint32_t set, uint32_t n) {
  const uint32_t at = set + (n >> 5) * 4;
  const uint32_t old = Load(base, at);
  Store(base, at, old | (1u << (n & 31)));
  return old;
}

}  // namespace

uint64_t StoreOwnedParts(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DD408 TuningScreen::StoreOwnedParts");
  const uint32_t s = screen;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t car = tuning_screen::SelectedGarageCar(base, s) + 212 + 1904;
  // The save data's bitset, its pointer read again at each bit as the
  // original does.
  auto save_bit = [&](uint32_t n) { return SetBit(base, Load(base, s + 18904) + 0x1996C, n); };
  // `count` words from s + `from`, word k nonzero setting the car's bit
  // `first` + k (or the save's).
  auto words = [&](uint32_t from, uint32_t count, uint32_t first, bool save) {
    uint32_t old = 0;
    bool any = false;
    for (uint32_t k = 0; k < count; k++) {
      if (Load(base, s + from + 4 * k) != 0) {
        old = save ? save_bit(first + k) : SetBit(base, car, first + k);
        any = true;
      }
    }
    return std::pair<bool, uint32_t>(any, old);
  };
  // The power items 0 and 1 of the part set [+33900]: 1 and 3 one half of
  // the bits, 0, 2 and 4 the other, other sets none.
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t v = Load(base, s + 33900);
    if (Load(base, s + (v * 10 + i + 8581) * 4) != 0) {
      if (v == 1 || v == 3) {
        SetBit(base, car, i);
      } else if (v == 0 || v == 2 || v == 4) {
        SetBit(base, car, i + 10);
      }
    }
  }
  for (uint32_t j = 20; j < 27; j++) {
    const uint32_t v = Load(base, s + 33900);
    if (Load(base, s + (v * 7 + j + 8686) * 4) != 0) {
      if (v == 1 || v == 3) {
        SetBit(base, car, j);
      } else if (v == 0 || v == 2 || v == 4) {
        SetBit(base, car, j + 7);
      }
    }
  }
  words(35028, 7, 34, false);
  words(35144, 7, 41, false);
  words(35232, 7, 48, false);
  words(35320, 7, 55, false);
  words(35408, 7, 62, false);
  words(35500, 7, 69, false);
  for (uint32_t j = 0; j < 10; j++) {
    if (Load(base, s + 35612 + 4 * j) != 0) {
      SetBit(base, car, j < 4 ? 76 + j : j < 7 ? 77 + j : 78 + j);
    }
  }
  words(35712, 7, 88, false);
  // The tires: bit 95 always, then three words a tire compared with 1.
  for (uint32_t i = 0; i < 5; i++) {
    if (i == 0) {
      save_bit(95);
      continue;
    }
    if (Load(base, s + 35828 + 12 * i) == 1) save_bit(95 + i);
    if (Load(base, s + 35832 + 12 * i) == 1) save_bit(99 + i);
    if (Load(base, s + 35836 + 12 * i) == 1) save_bit(103 + i);
  }
  words(36168, 5, 108, false);
  words(36212, 5, 119, false);
  words(36256, 5, 130, false);
  if (Load(base, s + 25172) != 0) {
    for (uint32_t i = 0; i < 6; i++) {
      if (Load(base, s + 36304 + 4 * i) != 0 && i != 1) {
        SetBit(base, car, i == 0 ? 141 : 140 + i);
      }
    }
  } else {
    words(36304, 5, 141, false);
  }
  for (uint32_t k = 0; k < 5; k++) {
    if (Load(base, s + 36380 + 8 * k) != 0) SetBit(base, car, 152 + k);
    if (Load(base, s + 36384 + 8 * k) != 0) SetBit(base, car, 158 + k);
  }
  words(36440, 4, 170, false);
  words(36476, 4, 174, false);
  words(36512, 4, 178, false);
  words(36548, 4, 182, false);
  words(36592, 3, 186, false);
  // Every wheel's flag (set to 1 on the way) into the save's bits 189 on.
  uint64_t r3 = 0;
  for (uint32_t n = 0; n < 179; n++) {
    const uint32_t was = TestAndSetWheelFlag(base, s, n);
    r3 = was;
    if (was != 0) {
      save_bit(189 + n);
    }
  }
  auto last = [&](std::pair<bool, uint32_t> set) {
    if (set.first) r3 = set.second;
  };
  last(words(36616, 2, 389, false));
  last(words(36672, 4, 391, false));
  last(words(36716, 5, 395, true));
  last(words(36756, 4, 400, false));
  last(words(36800, 6, 404, false));
  last(words(33624, 4, 435, true));
  // The interior item 3's rows: five words each, their bits offset by the
  // table's {3, 2, 1, 0}, kept in the frame as the original does.
  static constexpr uint32_t kTable[4] = {3, 2, 1, 0};
  for (uint32_t r = 0; r < 4; r++) {
    Store(base, frame + 80 + 4 * r, kTable[r]);
  }
  static constexpr uint32_t kWords[5] = {4, 0, 8, 16, 12};
  static constexpr uint32_t kFirsts[5] = {411, 416, 421, 426, 431};
  for (uint32_t r = 0; r < 4; r++) {
    const uint32_t row = s + 33820 + 20 * r;
    for (uint32_t k = 0; k < 5; k++) {
      if (Load(base, row + kWords[k]) != 0) {
        r3 = save_bit(Load(base, frame + 80 + 4 * r) + kFirsts[k]);
      }
    }
  }
  return r3;
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E25B0 TuningScreen::Leave");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if ((Load(base, kFadeOut) | Load(base, kFadeIn)) != 0) {
    return r3;
  }
  Store(base, Load(base, s + 18904) + 6116, Load(base, s + 25116));
  tuning_screen::StoreToGarageCar(base, r3, frame);
  const uint64_t out = StoreOwnedParts(base, s, frame);
  // Message 11 posted to r4's ring (+204: 50 records of 12 bytes, +804 the
  // count, +808 the start).
  const uint32_t ring = uint32_t(r4) + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t record = ring + ((start + count - 1) % 50) * 12;
  Store(base, record + 0, 11);
  Store(base, record + 4, 0);
  Store(base, record + 8, 0);
  return out;
}


namespace {

// The wheel list's row index for a slide: j = the cursor +25220 + `step`
// (all 64 bits), wrapped once over m (3, or with `by_count` the list's
// length +25224 + 1, re-read): j - m from m up, j + m below 0 (the
// compares on the low words, signed).
std::function<uint64_t()> Wrapped(uint8_t* base, uint32_t screen, int64_t step, bool by_count) {
  return [base, screen, step, by_count] {
    const uint64_t c = Load(base, screen + 25220);
    const uint64_t m = by_count ? uint64_t(Load(base, screen + 25224)) + 1 : 3;
    const uint64_t j = c + uint64_t(step);
    if (int32_t(j) >= int32_t(m)) {
      return j - m;
    }
    if (int32_t(j) < 0) {
      return j + m;
    }
    return j;
  };
}

// The cursor +25220 + `step` (all 64 bits), not wrapped.
std::function<uint64_t()> Unwrapped(uint8_t* base, uint32_t screen, int64_t step) {
  return [base, screen, step] { return uint64_t(Load(base, screen + 25220)) + uint64_t(step); };
}

}  // namespace

uint64_t FillPartRowsRight(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F40B8 TuningScreen::FillPartRowsRight");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t entry = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t movie = Load(base, entry);
  const int32_t last = int32_t(Load(base, s + 25224));
  auto row = [&](uint32_t node, const std::function<uint64_t()>& index) {
    return tuning_screen::FillPartRow(base, r3, frame, movie, node, index);
  };
  if (last == 1) {
    // Nodes 14 and 15 for c - 1 and c (no wrap).
    tuning_screen::HidePartMarks(base, frame);
    row(14, Unwrapped(base, s, -1));
    return row(15, Unwrapped(base, s, 0));
  }
  if (last == 2) {
    // Nodes 14 to 16 for c - 1 to c + 1, then 19 for c + 1, wrapped over 3.
    tuning_screen::HidePartMarks(base, frame);
    for (int64_t k = 0; k < 3; k++) {
      row(uint32_t(14 + k), Wrapped(base, s, k - 1, false));
    }
    return row(19, Wrapped(base, s, 1, false));
  }
  if (last < 4) {
    return entry;
  }
  // Nodes 14 to 18 for c - 1 to c + 3, then 19 for c + 4, wrapped over n + 1.
  tuning_screen::HidePartMarks(base, frame);
  for (int64_t k = 0; k < 5; k++) {
    row(uint32_t(14 + k), Wrapped(base, s, k - 1, true));
  }
  return row(19, Wrapped(base, s, 4, true));
}

uint64_t FillPartRowsLeft(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F4768 TuningScreen::FillPartRowsLeft");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t entry = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t movie = Load(base, entry);
  const int32_t last = int32_t(Load(base, s + 25224));
  auto row = [&](uint32_t node, const std::function<uint64_t()>& index) {
    return tuning_screen::FillPartRow(base, r3, frame, movie, node, index);
  };
  if (last == 1) {
    // Nodes 15 and 16 for c and c + 1 (no wrap).
    tuning_screen::HidePartMarks(base, frame);
    row(15, Unwrapped(base, s, 0));
    return row(16, Unwrapped(base, s, 1));
  }
  uint64_t marks = entry;
  if (last == 2) {
    // Nodes 14 to 17 for c - 1 to c + 2, wrapped over 3.
    tuning_screen::HidePartMarks(base, frame);
    for (int64_t k = 0; k < 4; k++) {
      marks = row(uint32_t(14 + k), Wrapped(base, s, k - 1, false));
    }
    return marks;
  }
  if (last < 4) {
    return entry;
  }
  // Nodes 14 to 19 for c - 1 to c + 4, wrapped over n + 1.
  tuning_screen::HidePartMarks(base, frame);
  for (int64_t k = 0; k < 6; k++) {
    marks = row(uint32_t(14 + k), Wrapped(base, s, k - 1, true));
  }
  return marks;
}

uint64_t AnimateWheels(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821F60E0 TuningScreen::AnimateWheels");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId);
  const uint32_t movie = Load(base, found);
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  auto tab = [&] { return uint64_t(Load(base, s + 25212)) + 20; };
  // The tabs' bar snapped to the row t: Child(node (1, 9), 0)'s +16.
  auto snap = [&] {
    const uint32_t node = ui_movies::Child(base, ui_movies::FindNode(base, movie, 1, 9), 0);
    Store(base, node + 16, BarPosition(base, frame, Load(base, s + 25212)));
  };
  uint64_t r3 = found;
  auto asked = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, offset, action, frame, lr, native_slots, &r3);
  };
  // A step left or right: the part name, then the list's slide (`one` with
  // a two-item list that moved, `many` with more items).
  auto slide = [&](uint64_t one, uint32_t one_lr, uint64_t many, uint32_t many_lr,
                   uint32_t play_lr) {
    tuning_screen::SetPartName(base, call, s, frame);
    const int32_t n = int32_t(Load(base, s + 25224));
    if (n == 1) {
      if (Load(base, s + 9708) != 0) {
        finish(one, one_lr);
        play(one, play_lr);
      }
    } else if (n != 0) {
      finish(many, many_lr);
      play(many, play_lr);
    }
  };
  if (asked(48, 155, 0x821F6148) || asked(48, 169, 0x821F6174)) {
    // Left.
    slide(64, 0x821F6644, 59, 0x821F6658, 0x821F6674);
    return FillPartRowsLeft(base, screen, frame);
  }
  if (asked(48, 156, 0x821F61A0) || asked(48, 170, 0x821F61CC)) {
    // Right.
    slide(63, 0x821F65D4, 58, 0x821F65E8, 0x821F6604);
    return FillPartRowsRight(base, screen, frame);
  }
  if (asked(48, 153, 0x821F61F8) || asked(48, 167, 0x821F6224)) {
    // Up: the bar snapped after the wrap from brand 0 to 16, else (with
    // +9708 0, a step inside the four tabs) 30 restarted.
    if (Load(base, s + 9708) != 0) {
      if (int32_t(Load(base, s + 25212)) == 3 && int32_t(Load(base, s + 25216)) == 0) {
        finish(30, 0x821F6550);
        finish(31, 0x821F655C);
        snap();
      }
    } else {
      finish(30, 0x821F6598);
      play(30, 0x821F64B4);
    }
  } else if (asked(48, 154, 0x821F6250) || asked(48, 168, 0x821F627C)) {
    // Down: the same with the wrap from 16 to 0, and 31.
    if (Load(base, s + 9708) != 0) {
      if (int32_t(Load(base, s + 25212)) == 0 && int32_t(Load(base, s + 25216)) == 3) {
        finish(30, 0x821F6444);
        finish(31, 0x821F6450);
        snap();
      }
    } else {
      finish(31, 0x821F6498);
      play(31, 0x821F64B4);
    }
  } else if (asked(40, 159, 0x821F62A8)) {
    // A: the tab's highlight, the bars and the slides finished.
    finish(tab(), 0x821F62C0);
    finish(30, 0x821F62CC);
    finish(31, 0x821F62D8);
    finish(58, 0x821F62E4);
    finish(59, 0x821F62F0);
    finish(70, 0x821F62FC);
    return finish(69, 0x821F6308);
  } else if (asked(40, 158, 0x821F6330)) {
    // B: the same, then the part select's row pick, 1008 and 1012 played.
    finish(tab(), 0x821F6348);
    finish(30, 0x821F6354);
    finish(31, 0x821F6360);
    finish(58, 0x821F636C);
    finish(59, 0x821F6378);
    finish(70, 0x821F6384);
    finish(69, 0x821F6390);
    finish(uint64_t(Load(base, s + 25196)) + 415, 0x821F63A0);
    play(uint64_t(Load(base, s + 25196)) + 415, 0x821F63C0);
    finish(1008, 0x821F63CC);
    play(1008, 0x821F63E8);
    finish(1012, 0x821F63F4);
    return tuning_screen::PlayNamed(call, frame, 0x821F6410, movie, 1012, 0);
  } else {
    return r3;
  }
  // After up or down: the part name, the group's animation, the rows, the
  // old tab's highlight finished, the new one restarted and looped.
  tuning_screen::SetPartName(base, call, s, frame);
  tuning_screen::PlayGroupAnimation(base, call, s, frame);
  tuning_screen::FillPartRows(base, screen, frame);
  finish(uint64_t(Load(base, s + 25216)) + 20, 0x821F64DC);
  finish(tab(), 0x821F64EC);
  play(tab(), 0x821F650C);
  return tuning_screen::LoopNamed(call, frame, 0x821F651C, movie, tab());
}

uint64_t InputWheels(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F6688 TuningScreen::InputWheels");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie =
      Load(base, tuning_screen::MovieEntry(base, frame, tuning_screen::kMovieId));
  const uint32_t entry = PlayerEntry(base);
  uint64_t r3 = 0;
  auto asked = [&](uint32_t offset, uint64_t action, uint32_t lr) {
    return Asked(base, call, entry, offset, action, frame, lr, flags.native_slots, &r3);
  };
  auto sound = [&](uint64_t cue, uint32_t lr) { call(kMenuSoundPlay, {cue}, frame, lr); };
  auto preview = [&] {
    std::optional<double> f1;  // PreviewWheel's f1, not read after
    tuning_screen::PreviewWheels(base, call, screen, frame, &f1);
  };
  // The queue's second half: the state +9652 current, command `command`.
  auto queue_command = [&](uint32_t command) {
    uint32_t state = 0;
    const uint32_t queue = QueueCurrent(base, s, &state);
    Store(base, queue + 0, 0xFFFFFFFFu);
    Store(base, queue + 4, state);
    Store(base, queue + 16, 0xFFFFFFFFu);
    Store(base, queue + 12, command);
  };
  // Push(16), then Go's first half to `state`.
  auto push_go = [&](uint32_t state) {
    PushWord(base, s, 16);
    const uint8_t depth = uint8_t(Byte(base, s + 10008) + 1);
    const uint32_t queue = Load(base, s + 10036);
    Store(base, s + 9652, state);
    guest::StoreU8(base, s + 10008, depth);
    GoFirstHalf(base, queue, state);
  };
  bool previewed = false;
  if (asked(48, 155, 0x821F66F0) || asked(48, 169, 0x821F671C)) {
    // Left: the model cursor back (wrapping to n past two items).
    const int32_t n = int32_t(Load(base, s + 25224));
    if (n == 1) {
      const int32_t c = int32_t(Load(base, s + 25220) - 1);
      Store(base, s + 25220, uint32_t(c));
      if (c < 0) {
        Store(base, s + 9708, 0);
        Store(base, s + 25220, 0);
      } else {
        sound(4, 0x821F6BD8);
        Store(base, s + 9708, 1);
      }
    } else if (n != 0) {
      sound(4, 0x821F6BEC);
      const int32_t c = int32_t(Load(base, s + 25220) - 1);
      Store(base, s + 25220, uint32_t(c));
      if (c < 0) {
        Store(base, s + 25220, Load(base, s + 25224));
      }
    }
    preview();
  } else if (asked(48, 156, 0x821F6748) || asked(48, 170, 0x821F6774)) {
    // Right: the model cursor on (wrapping to 0 past two items).
    const int32_t n = int32_t(Load(base, s + 25224));
    if (n == 1) {
      const int32_t c = int32_t(Load(base, s + 25220) + 1);
      Store(base, s + 25220, uint32_t(c));
      if (c > 1) {
        Store(base, s + 9708, 0);
        Store(base, s + 25220, 1);
      } else {
        sound(4, 0x821F6BD8);
        Store(base, s + 9708, 1);
      }
    } else if (n != 0) {
      sound(4, 0x821F6B80);
      const int32_t c = int32_t(Load(base, s + 25220) + 1);
      const int32_t last = int32_t(Load(base, s + 25224));
      Store(base, s + 25220, uint32_t(c));
      if (last < c) {
        Store(base, s + 25220, 0);
      }
    }
    preview();
  } else if (asked(48, 153, 0x821F67A0) || asked(48, 167, 0x821F67CC)) {
    // Up: the brand before (wrapping from 0 to 16 and the tabs to row 3),
    // the tabs' row back (scrolling at row 0).
    sound(4, 0x821F6AF4);
    const uint32_t t = Load(base, s + 25212), g = Load(base, s + 25204);
    Store(base, s + 25216, t);
    Store(base, s + 25220, 0);
    Store(base, s + 25208, g);
    Store(base, s + 25212, t - 1);
    Store(base, s + 25204, g - 1);
    if (int32_t(g - 1) < 0) {
      Store(base, s + 25204, 16);
      Store(base, s + 25212, 3);
      Store(base, s + 9708, 1);
    } else if (int32_t(t - 1) < 0) {
      Store(base, s + 25212, 0);
      Store(base, s + 9708, 1);
    }
    SetWheelListEnd(base, s);
    previewed = true;
  } else if (asked(48, 154, 0x821F67F8) || asked(48, 168, 0x821F6824)) {
    // Down: the brand after (wrapping from 16 to 0 and the tabs to row 0),
    // the tabs' row on (scrolling at row 3).
    sound(4, 0x821F6A84);
    const uint32_t t = Load(base, s + 25212), g = Load(base, s + 25204);
    Store(base, s + 25216, t);
    Store(base, s + 25220, 0);
    Store(base, s + 25208, g);
    Store(base, s + 25212, t + 1);
    Store(base, s + 25204, g + 1);
    if (int32_t(g + 1) > 16) {
      Store(base, s + 25204, 0);
      Store(base, s + 25212, 0);
      Store(base, s + 9708, 1);
    } else if (int32_t(t + 1) > 3) {
      Store(base, s + 25212, 3);
      Store(base, s + 9708, 1);
    }
    SetWheelListEnd(base, s);
    previewed = true;
  } else if (asked(40, 159, 0x821F6850)) {
    // A: the stock wheel (brand 0) to the confirm dialog (CONF EQUIP,
    // command 63), else the inch list (state 17, command 9); Push(16).
    sound(0, 0x821F6860);
    if (int32_t(Load(base, s + 25204)) == 0) {
      guest::StoreU8(base, s + 9665, 0);
      push_go(11);
      const uint32_t menu = Load(base, s + 10040);
      for (const auto& [field, value] :
           {std::pair<uint32_t, uint32_t>{12, 0}, {16, 1}, {20, 1}, {0, 0}, {4, 0}, {8, 0},
            {40, 0}, {48, 0}, {52, 0}, {36, 0}, {64, 0}, {24, 0}, {28, 0}, {32, 0}}) {
        Store(base, menu + field, value);
      }
      queue_command(63);
    } else {
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      const uint32_t end = InchListEnd(base, s);
      Store(base, s + 25232, end);
      Store(base, s + 25228, 0);
      push_go(17);
      queue_command(9);
    }
    previewed = true;
  } else if (asked(40, 158, 0x821F69E4)) {
    // B: Pop, the stand and garage car rewritten, the part select's set-up
    // again (command 7).
    sound(2, 0x821F69F4);
    const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
    const uint32_t queue = Load(base, s + 10036);
    Store(base, s + 25240, 1);
    Store(base, s + 9732, 1);
    Store(base, s + 9708, 1);
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
    Store(base, s + 9652, popped);
    Store(base, queue + 20, 0xFFFFFFFFu);
    Store(base, queue + 12, 0xFFFFFFFFu);
    Store(base, queue + 24, 0);
    Store(base, queue + 4, popped);
    Store(base, queue + 0, popped);
    Store(base, queue + 8, popped);
    uint32_t state = 0;
    const uint32_t current = QueueCurrent(base, s, &state);
    Store(base, current + 16, 0xFFFFFFFFu);
    Store(base, current + 4, state);
    Store(base, current + 0, 0xFFFFFFFFu);
    Store(base, current + 12, 7);
  }
  if (previewed) {
    preview();
    ShowPricePanel(base, s, frame);
  }
  // The price panel's texts (varargs: the arguments these calls leave unset
  // passed as 0, the user's call of 2026-10-30), the brand logo, then the
  // movie side.
  tuning_screen::SetText(call, frame, 0x821F6C20, movie, 19, 4, 527, 0);
  const uint64_t price = tuning_screen::GetPrice(base, s);
  tuning_screen::SetText(call, frame, 0x821F6C40, movie, 19, 2, 9, price);
  tuning_screen::SetText(call, frame, 0x821F6C54, movie, 19, 3, 435, 0);
  const uint32_t logo = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 17);
  if (int32_t(Load(base, s + 25204)) == 0) {
    Store(base, logo + 80, 0);
  } else {
    Store(base, logo + 80, 1);
    SetBrandLogo(base, s, frame);
  }
  return AnimateWheels(base, call, screen, frame, flags.native_slots);
}

}  // namespace tuning_pages
