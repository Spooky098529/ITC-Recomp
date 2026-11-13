// The tuning screen's vinyl editor's pages (states 23 to 31 and 34 of
// TuningScreen::HandleInput) and their movie-side updates
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

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

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The players' action mapper entries (8 bytes each: the mapper, blocked
// when +4 is 1) and their indices (a word a player).
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kHeldSlot = 0x8210BE70;     // ActionMapper::Held (slot 8, +32)
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed (slot 10, +40)
constexpr uint32_t kFiredSlot = 0x8210C010;    // ActionMapper::Fired (slot 12, +48)

constexpr uint32_t kMenuSoundPlay = 0x821103F0;  // MenuSound::Play(cue)
constexpr uint32_t kStandHolder = 0x826D83E0;    // the stand scene's holder's word
constexpr uint32_t kStageCommand = 0x82180108;   // GarageStage::Command(holder, command)
constexpr uint32_t kCameraCommand = 0x821808F0;  // GarageStage::CameraCommand(holder, command)
constexpr uint32_t kFitSelectedCar = 0x8217DAC0;  // GarageStage::FitSelectedCar(holder, 0, 0)

// The player's mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
uint32_t PlayerEntry(uint8_t* base) {
  return (Load(base, kControllerEntries) << 3) + kMapperEntries;
}

// The mapper's read slot at `offset` (+32 slot 8, +40 slot 10, +44 slot 11,
// +48 slot 12) on `action`, as the original's bctrl from `frame` returning
// to `lr`: ActionMapper::Held's, Pressed's or Fired's plain C++ when the
// vtable holds it and `native_slots` (logged for --log_pause_menu as their
// adapters do), else the slot through `call`. Its r3.
uint64_t ReadSlot(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t mapper,
                  uint32_t offset, uint64_t action, uint32_t frame, uint32_t lr,
                  bool native_slots) {
  const uint32_t function = Load(base, Load(base, mapper) + offset);
  if (native_slots) {
    const uint32_t id = uint32_t(action);
    uint32_t r3;
    int slot;
    if (function == kHeldSlot) {
      r3 = action_mappers::Held(base, mapper, id);
      slot = 8;
    } else if (function == kPressedSlot) {
      r3 = action_mappers::Pressed(base, mapper, id);
      slot = 10;
    } else if (function == kFiredSlot) {
      r3 = action_mappers::Fired(base, mapper, id);
      slot = 12;
    } else {
      return call(function, {mapper, action}, frame, lr);
    }
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, id, lr, double(r3));
    return r3;
  }
  return call(function, {mapper, action}, frame, lr);
}

// The pages' reads of the player's mapper entry, each skipped while its +4
// is 1; the last read's r3 kept.
struct Reads {
  uint8_t* base;
  const tuning_screen::GuestCall& call;
  uint32_t entry;
  uint32_t frame;
  bool native_slots;
  uint64_t r3;
  // Slot `offset` on `action`: whether its low byte is set.
  bool operator()(uint32_t offset, uint64_t action, uint32_t lr) {
    if (int32_t(Load(base, entry + 4)) == 1) {
      return false;
    }
    r3 = ReadSlot(base, call, Load(base, entry), offset, action, frame, lr, native_slots);
    return (r3 & 0xFF) != 0;
  }
};

// The history popped once: byte +10008 - 1, +9652 = the word there. The
// popped state.
uint32_t Pop(uint8_t* base, uint32_t s) {
  const uint32_t depth = (Byte(base, s + 10008) + 255) & 0xFF;
  guest::StoreU8(base, s + 10008, uint8_t(depth));
  const uint32_t popped = Load(base, ((depth + 2439) << 2) + s);
  Store(base, s + 9652, popped);
  return popped;
}

// Push(s): the history's word at the byte +10008 = s, the byte + 1.
[[maybe_unused]] void Push(uint8_t* base, uint32_t s, uint32_t state) {
  const uint32_t depth = Byte(base, s + 10008);
  Store(base, ((depth + 2439) << 2) + s, state);
  guest::StoreU8(base, s + 10008, uint8_t(depth + 1));
}

// Go's first half to state `state`: q+4 = q+0 = q+8 = s, q+20 = q+12 = -1,
// q+24 = 0 (q = [+10036]).
void GoFirstHalf(uint8_t* base, uint32_t s, uint32_t state) {
  const uint32_t queue = Load(base, s + 10036);
  Store(base, queue + 20, kNone);
  Store(base, queue + 12, kNone);
  Store(base, queue + 24, 0);
  Store(base, queue + 4, state);
  Store(base, queue + 0, state);
  Store(base, queue + 8, state);
}

// Go's second half with command `command`: the queue reloaded, q+8 = q+0
// when +9652 isn't q+0, q+16 = -1, q+4 = +9652, q+0 = -1, q+12 = command.
void GoSecondHalf(uint8_t* base, uint32_t s, uint32_t command) {
  const uint32_t queue = Load(base, s + 10036);
  const uint32_t state = Load(base, s + 9652);
  const uint32_t current = Load(base, queue + 0);
  if (int32_t(state) != int32_t(current)) {
    Store(base, queue + 8, current);
  }
  Store(base, queue + 16, kNone);
  Store(base, queue + 4, state);
  Store(base, queue + 0, kNone);
  Store(base, queue + 12, command);
}

// Go(state, command): +9652 = state, both halves.
[[maybe_unused]] void Go(uint8_t* base, uint32_t s, uint32_t state, uint32_t command) {
  Store(base, s + 9652, state);
  GoFirstHalf(base, s, state);
  GoSecondHalf(base, s, command);
}

// The stand holder [[0x826D83E0]].
uint32_t Holder(uint8_t* base) { return Load(base, Load(base, kStandHolder)); }

}  // namespace

uint64_t LeaveVinylView(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82201B40 TuningScreen::LeaveVinylView");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, 13);
  const uint32_t movie = Load(base, found);
  Reads asked{base, call, entry, frame, native_slots, found};
  if (!asked(40, 159, 0x82201BA8) && !asked(40, 158, 0x82201BD4)) {
    return asked.r3;
  }
  Store(base, s + 10012, 8);
  call(kFitSelectedCar, {Holder(base), 0, 0}, frame, 0x82201BFC);
  tuning_screen::FinishNamed(call, frame, 0x82201C08, movie, 241);
  uint64_t r3 = tuning_screen::PlayNamed(call, frame, 0x82201C24, movie, 241, 1);
  const int32_t area = int32_t(Load(base, s + 30724));
  if (area == 0) {
    r3 = call(kStageCommand, {Holder(base), 40}, frame, 0x82201C50);
  } else if (area == 1) {
    r3 = call(kStageCommand, {Holder(base), 41}, frame, 0x82201C50);
  }
  return r3;
}

uint64_t InputVinylView(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82202928 TuningScreen::InputVinylView");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  Reads asked{base, call, entry, frame, native_slots, 0};
  if (asked(40, 159, 0x82202978) || asked(40, 158, 0x822029A4)) {
    // A or B: Pop, Go(the popped state, 18), the turns cancelled, the camera
    // reset.
    call(kMenuSoundPlay, {2}, frame, 0x822029B4);
    Store(base, s + 9708, 1);
    const uint32_t popped = Pop(base, s);
    GoFirstHalf(base, s, popped);
    GoSecondHalf(base, s, 18);
    call(kStageCommand, {Holder(base), 1}, frame, 0x82202A40);
    call(kCameraCommand, {Holder(base), 20}, frame, 0x82202A50);
  }
  // The held buttons: the distance, the turns and the tilt.
  if (asked(32, 163, 0x82202A74)) {
    call(kCameraCommand, {Holder(base), 4}, frame, 0x82202A8C);
  }
  if (asked(32, 164, 0x82202AB0)) {
    call(kCameraCommand, {Holder(base), 5}, frame, 0x82202AC8);
  }
  if (asked(32, 173, 0x82202AEC)) {
    call(kStageCommand, {Holder(base), 1}, frame, 0x82202B04);
    call(kStageCommand, {Holder(base), 9}, frame, 0x82202B14);
  }
  if (asked(32, 174, 0x82202B38)) {
    call(kStageCommand, {Holder(base), 1}, frame, 0x82202B50);
    call(kStageCommand, {Holder(base), 10}, frame, 0x82202B60);
  }
  if (asked(32, 171, 0x82202B84)) {
    call(kStageCommand, {Holder(base), 1}, frame, 0x82202B9C);
    call(kCameraCommand, {Holder(base), 0}, frame, 0x82202BAC);
  }
  if (asked(32, 172, 0x82202BD0)) {
    call(kStageCommand, {Holder(base), 1}, frame, 0x82202BE8);
    call(kCameraCommand, {Holder(base), 1}, frame, 0x82202BF8);
  }
  return LeaveVinylView(base, call, screen, frame, native_slots);
}

}  // namespace tuning_pages
