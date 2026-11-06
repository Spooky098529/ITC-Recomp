// The front end's menu helpers (docs/functions/menu-helpers.md): see
// menu_helpers.h.

#include "menu_helpers/menu_helpers.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"

namespace menu_helpers {

namespace {

int32_t S(uint32_t value) { return int32_t(value); }

uint32_t Word(uint8_t* base, uint32_t menu, uint32_t field) {
  return guest::LoadU32(base, menu + field);
}

void SetWord(uint8_t* base, uint32_t menu, uint32_t field, uint32_t value) {
  guest::StoreU32(base, menu + field, value);
}

// The mapper's read slot at `offset` in its vtable on `action`, as the
// original's bctrl from `frame` returning to `lr`: ActionMapper::Pressed's
// or Fired's plain C++ when the vtable holds it and `native_slots`, logged
// for --log_pause_menu as its adapter does, else the slot through `calls`.
uint64_t ReadSlot(uint8_t* base, uint32_t mapper, uint32_t offset, uint64_t action,
                  uint32_t frame, uint32_t lr, bool native_slots, const Calls& calls) {
  const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, mapper) + offset);
  if (native_slots && (function == kPressedSlot || function == kFiredSlot)) {
    const bool pressed = function == kPressedSlot;
    const uint32_t id = uint32_t(action);
    const uint32_t r3 = pressed ? action_mappers::Pressed(base, mapper, id)
                                : action_mappers::Fired(base, mapper, id);
    debug::PauseMenuLog_NoteSlotRead(pressed ? 10 : 12, mapper, id, lr, double(r3));
    return r3;
  }
  return calls.Call(function, frame, lr, mapper, action);
}

// One try on the player's entry: false when its +4 is 1 (the mappers
// blocked), else whether the slot's answer's low byte is nonzero. The entry's
// +4 and +0 are read again on every try, as the original does.
bool Try(uint8_t* base, uint32_t entry, uint32_t offset, uint64_t action, uint32_t frame,
         uint32_t lr, bool native_slots, const Calls& calls) {
  if (S(guest::LoadU32(base, entry + 4)) == 1) {
    return false;
  }
  const uint32_t mapper = guest::LoadU32(base, entry);
  return (ReadSlot(base, mapper, offset, action, frame, lr, native_slots, calls) & 0xFF) != 0;
}

// The cursor moved by `delta` (read again, as the original's lwz, addi, stw).
void MoveCursor(uint8_t* base, uint32_t menu, int32_t delta) {
  SetWord(base, menu, kCursor, Word(base, menu, kCursor) + uint32_t(delta));
}

struct Read {
  uint32_t offset;  // the slot
  uint32_t action;
  uint32_t lr;  // the original's return address
};

}  // namespace

uint32_t Step(uint8_t* base, uint32_t menu, uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186568 Menu::Step");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  // The original's order: the player, the cursor and +24 loaded, the
  // player's entry index, then +4 and +36 stored.
  const uint32_t player = Word(base, menu, kPlayer);
  const uint32_t cursor = Word(base, menu, kCursor);
  const uint32_t locked = Word(base, menu, kUpDownLocked);
  const uint32_t index = guest::LoadU32(base, player * 4 + kControllerEntries);
  SetWord(base, menu, kPrevious, cursor);
  SetWord(base, menu, kSide, 0);
  const uint32_t entry = index * 8 + kMapperEntries;

  // (1) Up and down (and the side asked), the first fired wins.
  if (locked == 0) {
    struct Move {
      Read read;
      int kind;  // -1 / 1 the cursor, 2 the side -1, 3 the side 1
    };
    static constexpr Move kMoves[] = {
        {{kFiredOffset, 153, 0x821865E4}, -1}, {{kFiredOffset, 167, 0x82186610}, -1},
        {{kFiredOffset, 154, 0x8218663C}, 1},  {{kFiredOffset, 168, 0x82186668}, 1},
        {{kFiredOffset, 155, 0x82186694}, 2},  {{kFiredOffset, 169, 0x821866C0}, 2},
        {{kFiredOffset, 156, 0x821866EC}, 3},  {{kFiredOffset, 170, 0x82186718}, 3},
    };
    for (const Move& move : kMoves) {
      if (Try(base, entry, move.read.offset, move.read.action, frame, move.read.lr, native_slots,
              calls)) {
        if (move.kind == 2) {
          SetWord(base, menu, kSide, uint32_t(-1));
        } else if (move.kind == 3) {
          SetWord(base, menu, kSide, 1);
        } else {
          MoveCursor(base, menu, move.kind);
        }
        break;
      }
    }
  }

  // (2) Left and right step the cursor; (3) the shoulder buttons, pressed
  // or fired.
  struct Steps {
    uint32_t field;
    Read reads[4];
    int deltas[4];
  };
  static constexpr Steps kSteps[] = {
      {kSidesStep,
       {{kFiredOffset, 155, 0x82186778},
        {kFiredOffset, 169, 0x821867A4},
        {kFiredOffset, 156, 0x821867D0},
        {kFiredOffset, 170, 0x821867FC}},
       {-1, -1, 1, 1}},
      {kShouldersStep,
       {{kPressedOffset, 161, 0x8218684C},
        {kFiredOffset, 161, 0x82186878},
        {kPressedOffset, 162, 0x821868A4},
        {kFiredOffset, 162, 0x821868D0}},
       {-1, -1, 1, 1}},
  };
  for (const Steps& steps : kSteps) {
    if (Word(base, menu, steps.field) == 0) {
      continue;
    }
    for (int k = 0; k < 4; k++) {
      const Read& read = steps.reads[k];
      if (Try(base, entry, read.offset, read.action, frame, read.lr, native_slots, calls)) {
        MoveCursor(base, menu, steps.deltas[k]);
        break;
      }
    }
  }

  // Wrapped or clamped between the first and the last item.
  if (Word(base, menu, kWraps) != 0) {
    const int32_t now = S(Word(base, menu, kCursor));
    const int32_t first = S(Word(base, menu, kFirst));
    uint32_t wrapped;
    if (now < first) {
      wrapped = 1;
    } else {
      wrapped = now > S(Word(base, menu, kLast)) ? 1 : 0;
    }
    SetWord(base, menu, kWrapped, wrapped);
    int32_t result = now;
    if (now < first) {
      result = S(Word(base, menu, kLast));
    } else if (now > S(Word(base, menu, kLast))) {
      result = first;
    }
    SetWord(base, menu, kCursor, uint32_t(result));
  } else {
    const int32_t now = S(Word(base, menu, kCursor));
    const int32_t first = S(Word(base, menu, kFirst));
    SetWord(base, menu, kWrapped, 0);
    int32_t result = now;
    if (now < first) {
      result = first;
    } else {
      const int32_t last = S(Word(base, menu, kLast));
      if (now > last) {
        result = last;
      }
    }
    SetWord(base, menu, kCursor, uint32_t(result));
  }

  // Which way it moved since the start.
  const int32_t now = S(Word(base, menu, kCursor));
  const int32_t before = S(Word(base, menu, kPrevious));
  SetWord(base, menu, kMoved, now > before ? 1u : now < before ? uint32_t(-1) : 0u);
  return Word(base, menu, kMoved);
}

uint32_t Pressed(uint8_t* base, uint32_t menu, uint64_t action, uint32_t sp, bool native_slots,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821864C8 Menu::Pressed");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  if (Word(base, menu, kMoved) != 0 || Word(base, menu, kSide) != 0) {
    return 0;
  }
  const uint32_t player = Word(base, menu, kPlayer);
  const uint32_t index = guest::LoadU32(base, player * 4 + kControllerEntries);
  const uint32_t entry = index * 8 + kMapperEntries;
  return Try(base, entry, kPressedOffset, action, frame, 0x82186540, native_slots, calls) ? 1 : 0;
}

}  // namespace menu_helpers
