// The tuning screen's vinyl editor's pages (states 23 to 31 and 34 of
// TuningScreen::HandleInput) and their movie-side updates
// (docs/functions/tuning-pages.md): see tuning_pages.h.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <cmath>
#include <optional>

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
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kFadeOutPasses = 0x82001BF0;
constexpr uint32_t kBarStep = 0x82001FB0;        // 40.0f, a bar's step
constexpr uint32_t kKeepCounter = 0x826D84A4;    // InputVinylPages' kept page's counter

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

// Push(s): the history's word at the byte +10008 = s, the byte read again
// + 1.
void Push(uint8_t* base, uint32_t s, uint32_t state) {
  Store(base, ((Byte(base, s + 10008) + 2439) << 2) + s, state);
  // The byte read again (a push at depth 63 writes over it).
  guest::StoreU8(base, s + 10008, uint8_t(Byte(base, s + 10008) + 1));
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

// memcpy(destination, source, n) from `frame` returning to `lr`: its plain
// C++ where Buffers_Memcpy takes it, else the guest's. Its r3.
uint64_t Memcpy(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t destination,
                uint64_t source, uint32_t n, uint32_t frame, uint32_t lr) {
  if (native::Buffers_Memcpy(base, uint32_t(destination), uint32_t(source), n, lr)) {
    return destination;
  }
  return call(kMemcpy, {destination, source, n}, frame, lr);
}

// A bar's position: the word sign-extended as a doubleword at frame + 80,
// converted (fcfid), rounded to a single, times 40.0f rounded to a single;
// its bits.
uint32_t BarPosition(uint8_t* base, uint32_t frame, uint32_t word) {
  const int64_t x = int32_t(word);
  Store(base, frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(base, frame + 84, uint32_t(x));
  const double f13 = double(float(double(x)));
  const double step = double(std::bit_cast<float>(Load(base, kBarStep)));
  return std::bit_cast<uint32_t>(float(f13 * step));
}

// Entry i's texture words (+28, +32) copied to the kept texture k (images
// + 7236 + 8k), both read first.
void KeepTexture(uint8_t* base, uint32_t images, uint32_t i, uint32_t k) {
  const uint32_t header = Load(base, images + 36 * i + 28);
  const uint32_t data = Load(base, images + 36 * i + 32);
  Store(base, images + 7236 + 8 * k, header);
  Store(base, images + 7240 + 8 * k, data);
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

void SetLastImage(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821FE498 TuningScreen::SetLastImage");
  uint32_t last;
  if (int32_t(Load(base, screen + 9656)) == 5 && Byte(base, screen + 9660) == 6) {
    const int32_t page = int32_t(Load(base, screen + 30724));
    if (page == 0) {
      last = 4;
    } else if (page == 1) {
      last = 193;
    } else {
      return;
    }
  } else {
    last = 198;
  }
  Store(base, screen + 30796, last);
}

uint64_t MoveVinylMenu(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821FF3A0 TuningScreen::MoveVinylMenu");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, 13);
  const uint32_t movie = Load(base, found);
  Reads asked{base, call, entry, frame, native_slots, found};
  auto page = [&] { return uint64_t(Load(base, s + 30724)); };
  auto finish = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::FinishNamed(call, frame, lr, movie, named);
  };
  auto play = [&](uint64_t named, uint32_t lr) {
    return tuning_screen::PlayNamed(call, frame, lr, movie, named, 1);
  };
  // The bar snapped to the page: node (3, 0)'s +16.
  auto snap = [&] {
    const uint32_t node = ui_movies::FindNode(base, movie, 3, 0);
    Store(base, node + 16, BarPosition(base, frame, Load(base, s + 30724)));
  };
  uint64_t old;
  if (asked(48, 153, 0x821FF408) || asked(48, 167, 0x821FF434)) {
    // Up.
    if (Load(base, s + 9708) == 0) {
      return asked.r3;
    }
    if (Load(base, s + 30728) != 0) {
      finish(16, 0x821FF62C);
      finish(17, 0x821FF638);
      snap();
      old = 6;
    } else {
      finish(16, 0x821FF67C);
      play(16, 0x821FF698);
      old = page() + 7;
    }
  } else if (asked(48, 154, 0x821FF460) || asked(48, 168, 0x821FF48C)) {
    // Down.
    if (Load(base, s + 9708) == 0) {
      return asked.r3;
    }
    if (Load(base, s + 30728) != 0) {
      finish(16, 0x821FF588);
      finish(17, 0x821FF594);
      snap();
      old = uint64_t(Load(base, s + 30784)) + 6;
    } else {
      finish(17, 0x821FF5E0);
      play(17, 0x821FF5FC);
      old = page() + 5;
    }
  } else if (asked(40, 159, 0x821FF4B8)) {
    // A: the bars and the page's highlight finished, the decals applied on
    // the stand.
    finish(16, 0x821FF4CC);
    finish(17, 0x821FF4D8);
    finish(page() + 6, 0x821FF4E8);
    Store(base, s + 10012, 5);
    return call(kFitSelectedCar, {Holder(base), 0, 0}, frame, 0x821FF508);
  } else if (asked(40, 158, 0x821FF530)) {
    // B: the same, nothing applied.
    finish(16, 0x821FF544);
    finish(17, 0x821FF550);
    return finish(page() + 6, 0x821FF560);
  } else {
    return asked.r3;
  }
  // The old page finished, the new one restarted and looped.
  finish(old, 0x821FF6A8);
  finish(page() + 6, 0x821FF6B8);
  play(page() + 6, 0x821FF6D8);
  return tuning_screen::LoopNamed(call, frame, 0x821FF6E8, movie, page() + 6);
}

uint64_t InputVinylPages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82204B70 TuningScreen::InputVinylPages");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint64_t images = screen + 10048;
  const uint32_t im = uint32_t(images);
  const bool native_free = flags.native_heap && flags.native_free;
  // The pages' kept textures: page 0's six (kept 0 to 5 = entries 0, 1, 2,
  // 6, 7, 8), page 1's (kept 15 to 20 = entries 21, 22, 23, 27, 28, 29).
  static constexpr uint32_t kPage0[6] = {0, 1, 2, 6, 7, 8};
  static constexpr uint32_t kPage1[6] = {21, 22, 23, 27, 28, 29};
  if (Load(base, s + 18900) != 0) {
    // A page kept: its textures freed and kept again on the third frame.
    const uint32_t count = Load(base, kKeepCounter) + 1;
    Store(base, kKeepCounter, count);
    if (int32_t(count) == 3) {
      const int32_t page = int32_t(Load(base, s + 30724));
      if (page == 0 || page == 1) {
        const uint32_t first = page == 0 ? 0 : 15;
        const uint32_t lrs0[6] = {0x82204BDC, 0x82204BE8, 0x82204BF4,
                                  0x82204C00, 0x82204C0C, 0x82204C18};
        const uint32_t lrs1[6] = {0x82204C94, 0x82204CA0, 0x82204CAC,
                                  0x82204CB8, 0x82204CC4, 0x82204CD0};
        for (uint32_t k = 0; k < 6; k++) {
          FreeKept(base, images, first + k,
                   GuestTextureFree(base, call, frame, page == 0 ? lrs0[k] : lrs1[k],
                                    native_free));
        }
        for (uint32_t k = 0; k < 6; k++) {
          KeepTexture(base, im, page == 0 ? kPage0[k] : kPage1[k], first + k);
        }
      }
      Store(base, s + 18900, 0);
      Store(base, kKeepCounter, 0);
    }
  }
  if (Load(base, s + 18900) == 0) {
    Reads asked{base, call, entry, frame, flags.native_slots, 0};
    if (asked(48, 153, 0x82204D6C) || asked(48, 167, 0x82204D98)) {
      // Up, wrapping to the last page +30784.
      call(kMenuSoundPlay, {4}, frame, 0x822050D4);
      const int32_t page = int32_t(Load(base, s + 30724) - 1);
      Store(base, s + 9708, 1);
      Store(base, s + 30724, uint32_t(page));
      if (page < 0) {
        Store(base, s + 30724, Load(base, s + 30784));
        Store(base, s + 30728, 1);
      } else {
        Store(base, s + 30728, 0);
      }
    } else if (asked(48, 154, 0x82204DC4) || asked(48, 168, 0x82204DF0)) {
      // Down, wrapping to 0 past +30784.
      call(kMenuSoundPlay, {4}, frame, 0x822050A4);
      const int32_t page = int32_t(Load(base, s + 30724) + 1);
      const int32_t last = int32_t(Load(base, s + 30784));
      Store(base, s + 9708, 1);
      Store(base, s + 30724, uint32_t(page));
      if (last < page) {
        Store(base, s + 30724, 0);
        Store(base, s + 30728, 1);
      } else {
        Store(base, s + 30728, 0);
      }
    } else if (asked(40, 159, 0x82204E1C)) {
      // A: the page's layers 0 and 1 (not 2: the loop stops at 2) copied to
      // their saved ids and records, then the layers' page.
      call(kMenuSoundPlay, {0}, frame, 0x82204E2C);
      SetLastImage(base, s);
      for (uint32_t j = 0; j < 2; j++) {
        const uint32_t n = Load(base, s + 30724) * 3 + j;
        Store(base, ((n + 7798) << 2) + s, Load(base, ((n + 7792) << 2) + s));
        const uint64_t m = uint64_t(Load(base, s + 30724)) * 3 + j;
        const uint64_t at = m * 24 + screen;
        Memcpy(base, call, at + 31024, at + 30880, 24, frame, 0x82204E80);
      }
      Store(base, s + 30732, 0);
      Store(base, s + 9708, 1);
      Store(base, s + 9716, 1);
      Store(base, s + 30736, 3);
      Push(base, s, 23);
      Store(base, s + 9652, 24);
      GoFirstHalf(base, s, 24);
      // The page's kept textures set again (no FreeKept this time).
      const int32_t page = int32_t(Load(base, s + 30724));
      if (page == 0 || page == 1) {
        for (uint32_t k = 0; k < 6; k++) {
          KeepTexture(base, im, page == 0 ? kPage0[k] : kPage1[k], (page == 0 ? 0 : 15) + k);
        }
      }
      GoSecondHalf(base, s, 16);
    } else if (asked(40, 158, 0x82205018)) {
      // B: state 61 (leaving the editor) with no command, the vinyls stored,
      // the fade out.
      call(kMenuSoundPlay, {2}, frame, 0x82205028);
      Store(base, s + 9652, 61);
      GoFirstHalf(base, s, 61);
      const uint32_t queue = Load(base, s + 10036);
      const uint32_t state = Load(base, s + 9652);
      const uint32_t current = Load(base, queue + 0);
      if (int32_t(state) != int32_t(current)) {
        Store(base, queue + 8, current);
      }
      Store(base, queue + 0, state);
      Store(base, queue + 12, kNone);
      Store(base, queue + 16, kNone);
      StoreVinyls(base, screen, frame);
      mode_tick::ScreenFadesFadeOut(base, Load(base, kFadeOutPasses), 0, 0, 0, 0);
    }
  }
  return MoveVinylMenu(base, call, screen, frame, flags.native_slots);
}

namespace {

namespace fs = strings_and_font::steps;

constexpr uint32_t kHighlightSelected = 0x82203570;  // TuningScreen::HighlightSelected(screen, on)
constexpr uint32_t kLimits = 0x8200C840;  // the decal's position limits, 16 bytes a page

// The decal edits: state 29 (position), 30 (angle), 31 (scale).
enum class Edit { kPosition, kAngle, kScale };

// The return addresses of an edit page's calls, in the order the original
// makes them.
struct EditInputCalls {
  uint32_t held, released, highlight;
  uint32_t moves[8];
  uint32_t a, a_held, a_released, a_sound;
  uint32_t b, b_held, b_released, b_sound, b_memcpy;
  uint32_t move;
};
// And of its movie side's.
struct EditMoveCalls {
  uint32_t moves[8];
  uint32_t a, b, held, released, b_again, finish, play;
};

constexpr EditInputCalls kPositionInput = {
    0x82206AA4, 0x82206AD8, 0x82206AEC,
    {0x82206B10, 0x82206B3C, 0x82206B68, 0x82206B94, 0x82206BC0, 0x82206BEC, 0x82206C18,
     0x82206C44},
    0x82206C70, 0x82206C9C, 0x82206CC8, 0x82206CD8,
    0x82206D50, 0x82206D7C, 0x82206DA8, 0x82206DB8, 0x82206E18,
    0x82206F30};
constexpr EditInputCalls kAngleInput = {
    0x82206F9C, 0x82206FD0, 0x82206FE4,
    {0x82207008, 0x82207034, 0x82207060, 0x8220708C},
    0x822070B8, 0x822070E4, 0x82207110, 0x82207120,
    0x82207198, 0x822071C4, 0x822071F0, 0x82207200, 0x82207260,
    0x82207308};
constexpr EditInputCalls kScaleInput = {
    0x82207374, 0x822073A8, 0x822073BC,
    {0x822073E0, 0x8220740C, 0x82207438, 0x82207464},
    0x82207490, 0x822074BC, 0x822074E8, 0x822074F8,
    0x82207570, 0x8220759C, 0x822075C8, 0x822075D8, 0x82207638,
    0x822076D0};
constexpr EditMoveCalls kPositionMove = {
    {0x82200E28, 0x82200E54, 0x82200E80, 0x82200EAC, 0x82200ED8, 0x82200F04, 0x82200F30,
     0x82200F5C},
    0x82200F88, 0x82200FB4, 0x82200FE0, 0x8220100C, 0x82201038, 0x82201184, 0x822011A0};
constexpr EditMoveCalls kAngleMove = {
    {0x82201350, 0x8220137C, 0x822013A8, 0x822013D4},
    0x82201400, 0x8220142C, 0x82201458, 0x82201484, 0x822014B0, 0x8220155C, 0x82201578};
constexpr EditMoveCalls kScaleMove = {
    {0x82201688, 0x822016B4, 0x822016E0, 0x8220170C},
    0x82201738, 0x82201764, 0x82201790, 0x822017BC, 0x822017E8, 0x822018D0, 0x822018EC};

// The edited decal's node: movie 13's (-1, 19 + layer) on page 0, (-1, 22 +
// layer) on page 1; 0 on other pages.
uint32_t DecalNode(uint8_t* base, uint32_t movie, uint32_t s, int32_t page) {
  const uint32_t layer = Load(base, s + 30732);
  return ui_movies::FindNode(base, movie, 0xFFFFFFFFu, layer + (page == 0 ? 19 : 22));
}

// A doubleword at `at` (an int sign-extended through the stack for fcfid).
void StoreDoubleword(uint8_t* base, uint32_t at, int64_t value) {
  Store(base, at, uint32_t(uint64_t(value) >> 32));
  Store(base, at + 4, uint32_t(value));
}

// Place: the node's +12 and +16 from the decal's x and y (+30832, +30836)
// times the page's scale plus its origin (single-precision multiply-adds)
// plus the page's offsets; the conversions' doublewords at frame + 80
// (the y origin) and + 88 (y) as they're left. The node, or nullopt on
// other pages.
std::optional<uint32_t> Place(uint8_t* base, uint32_t movie, uint32_t s, uint32_t frame) {
  const int32_t page = int32_t(Load(base, s + 30724));
  if (page != 0 && page != 1) {
    return std::nullopt;
  }
  const uint32_t node = DecalNode(base, movie, s, page);
  const uint32_t scale_at = page == 0 ? 30812 : 30828;
  const uint32_t origin_at = page == 0 ? 30800 : 30816;
  const uint32_t x_offset = page == 0 ? 0x8200CAF4 : 0x8200CAEC;
  const uint32_t y_offset = page == 0 ? 0x8200CAF0 : 0x8200CAE8;
  auto coordinate = [&](uint32_t at, uint32_t origin, uint32_t offset) {
    const double scale = fs::Lfs(base, s + scale_at);
    const int64_t v = int32_t(Load(base, s + at));
    const int64_t o = int32_t(Load(base, s + origin));
    const double f13 = fs::ToSingle(v), f12 = fs::ToSingle(o);
    const double sum = fs::Single(std::fma(f13, scale, f12));
    StoreDoubleword(base, frame + 80, o);
    StoreDoubleword(base, frame + 88, v);
    return fs::Single(sum + fs::Lfs(base, offset));
  };
  fs::Stfs(base, node + 12, coordinate(30832, origin_at, x_offset));
  fs::Stfs(base, node + 16, coordinate(30836, origin_at + 4, y_offset));
  return node;
}

// The node's pivot: +20 and +24 the first lookup's +100 and +104 times 0.5
// (fctiwz) into the second's.
void Pivot(uint8_t* base, uint32_t first, uint32_t second) {
  const double half = fs::Lfs(base, 0x82001BE4);
  Store(base, second + 20, uint32_t(fs::Fctiwz(fs::Single(fs::Lfs(base, first + 100) * half))));
  Store(base, second + 24, uint32_t(fs::Fctiwz(fs::Single(fs::Lfs(base, first + 104) * half))));
}

// Turn: the node (looked up twice) pivoted, its +36 the angle +30840.
std::optional<uint32_t> Turn(uint8_t* base, uint32_t movie, uint32_t s) {
  const int32_t page = int32_t(Load(base, s + 30724));
  if (page != 0 && page != 1) {
    return std::nullopt;
  }
  const uint32_t first = DecalNode(base, movie, s, page);
  const uint32_t second = DecalNode(base, movie, s, page);
  Pivot(base, first, second);
  fs::Stfs(base, second + 36, fs::Lfs(base, s + 30840));
  return second;
}

// Size: the node (looked up twice): +28 and +32 the scale +30844 times the
// page's (+30808 or +30824), then pivoted.
std::optional<uint32_t> Size(uint8_t* base, uint32_t movie, uint32_t s) {
  const int32_t page = int32_t(Load(base, s + 30724));
  if (page != 0 && page != 1) {
    return std::nullopt;
  }
  const uint32_t first = DecalNode(base, movie, s, page);
  const uint32_t second = DecalNode(base, movie, s, page);
  const uint32_t factor = s + (page == 0 ? 30808 : 30824);
  fs::Stfs(base, second + 28, fs::Single(fs::Lfs(base, s + 30844) * fs::Lfs(base, factor)));
  fs::Stfs(base, second + 32, fs::Single(fs::Lfs(base, s + 30844) * fs::Lfs(base, factor)));
  Pivot(base, first, second);
  return second;
}

uint64_t MoveDecal(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                   uint32_t sp, bool native_slots, Edit edit) {
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = PlayerEntry(base);
  const uint32_t found = tuning_screen::MovieEntry(base, frame, 13);
  const uint32_t movie = Load(base, found);
  Reads asked{base, call, entry, frame, native_slots, found};
  const EditMoveCalls& lr = edit == Edit::kPosition ? kPositionMove
                            : edit == Edit::kAngle  ? kAngleMove
                                                    : kScaleMove;
  auto apply = [&]() -> std::optional<uint32_t> {
    switch (edit) {
      case Edit::kPosition: return Place(base, movie, s, frame);
      case Edit::kAngle: return Turn(base, movie, s);
      default: return Size(base, movie, s);
    }
  };
  static constexpr uint32_t kPositionMoves[8] = {153, 167, 154, 168, 156, 170, 155, 169};
  static constexpr uint32_t kTurnMoves[4] = {156, 170, 155, 169};
  const uint32_t* moves = edit == Edit::kPosition ? kPositionMoves : kTurnMoves;
  const uint32_t count = edit == Edit::kPosition ? 8 : 4;
  for (uint32_t k = 0; k < count; k++) {
    if (asked(48, moves[k], lr.moves[k])) {
      const std::optional<uint32_t> node = apply();
      return node ? *node : asked.r3;
    }
  }
  if (!asked(40, 159, lr.a) && !asked(40, 158, lr.b)) {
    return asked.r3;
  }
  if (asked(32, 164, lr.held) || asked(44, 164, lr.released)) {
    return asked.r3;
  }
  if (asked(40, 158, lr.b_again)) {
    apply();
  }
  tuning_screen::FinishNamed(call, frame, lr.finish, movie, 228);
  return tuning_screen::PlayNamed(call, frame, lr.play, movie, 228, 1);
}

// An edit page's move: the field at `at` stepped and clamped (the
// position's ints) or the float stepped by [step] and wrapped or clamped
// (the angle's and scale's); +9708 = 1 first.
void Step(uint8_t* base, uint32_t s, Edit edit, uint32_t move) {
  Store(base, s + 9708, 1);
  if (edit == Edit::kPosition) {
    // 0 up (y - 1, y min), 1 down (y + 1, y max), 2 left (x - 1, x min), 3
    // right (x + 1, x max).
    const uint32_t at = move < 2 ? 30836 : 30832;
    const int32_t delta = (move % 2 == 0) ? -1 : 1;
    const uint32_t limits = (Load(base, s + 30724) << 4) + kLimits;
    const uint32_t limit_at = limits + (move < 2 ? 8 : 0) + (move % 2 == 0 ? 0 : 4);
    const int32_t value = int32_t(Load(base, s + at) + uint32_t(delta));
    Store(base, s + at, uint32_t(value));
    const int32_t limit = int32_t(Load(base, limit_at));
    if (delta < 0 ? value < limit : value > limit) {
      Store(base, s + at, uint32_t(limit));
    }
    return;
  }
  // 0 left (minus), 1 right (plus).
  const bool plus = move == 1;
  const uint32_t at = s + (edit == Edit::kAngle ? 30840 : 30844);
  const uint32_t step = edit == Edit::kAngle ? 0x82000A30 : 0x8200222C;
  const double old = fs::Lfs(base, at);
  const double value = fs::Single(plus ? old + fs::Lfs(base, step) : old - fs::Lfs(base, step));
  fs::Stfs(base, at, value);
  if (edit == Edit::kAngle) {
    // 0.0 to 359.0, wrapped.
    if (plus ? value > fs::Lfs(base, 0x8200C54C) : value < fs::Lfs(base, 0x82000A34)) {
      fs::Stfs(base, at, fs::Lfs(base, plus ? 0x82000A34 : 0x8200C54C));
    }
  } else {
    // 0.1 to 2.0, clamped.
    const double limit = fs::Lfs(base, plus ? 0x820021E8 : 0x82001FBC);
    if (plus ? value > limit : value < limit) {
      fs::Stfs(base, at, limit);
    }
  }
}

uint64_t InputDecal(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp, const PageFlags& flags, Edit edit) {
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = PlayerEntry(base);
  Reads asked{base, call, entry, frame, flags.native_slots, 0};
  const EditInputCalls& lr = edit == Edit::kPosition ? kPositionInput
                             : edit == Edit::kAngle  ? kAngleInput
                                                     : kScaleInput;
  // 164 held: the selected decal highlighted (0), released: back (1).
  if (asked(32, 164, lr.held)) {
    call(kHighlightSelected, {screen, 0}, frame, lr.highlight);
  } else if (asked(44, 164, lr.released)) {
    call(kHighlightSelected, {screen, 1}, frame, lr.highlight);
  }
  // The moves: the position's up, down, left, right; the angle's and
  // scale's left, right.
  static constexpr uint32_t kPositionMoves[8] = {153, 167, 154, 168, 155, 169, 156, 170};
  static constexpr uint32_t kTurnMoves[4] = {155, 169, 156, 170};
  const uint32_t* moves = edit == Edit::kPosition ? kPositionMoves : kTurnMoves;
  const uint32_t count = edit == Edit::kPosition ? 8 : 4;
  bool moved = false;
  for (uint32_t k = 0; k < count && !moved; k++) {
    if (asked(48, moves[k], lr.moves[k])) {
      Step(base, s, edit, k / 2);
      moved = true;
    }
  }
  if (!moved) {
    // A or B, with 164 neither held nor released: Pop, Go(the popped
    // state, 18); B first puts the decal back as it was (+30856).
    bool a = false, b = false;
    if (asked(40, 159, lr.a) && !asked(32, 164, lr.a_held) &&
        !asked(44, 164, lr.a_released)) {
      a = true;
    } else if (asked(40, 158, lr.b) && !asked(32, 164, lr.b_held) &&
               !asked(44, 164, lr.b_released)) {
      b = true;
    }
    if (a || b) {
      call(kMenuSoundPlay, {a ? 0u : 2u}, frame, a ? lr.a_sound : lr.b_sound);
      Store(base, s + 9708, 1);
      const uint32_t popped = Pop(base, s);
      GoFirstHalf(base, s, popped);
      if (b) {
        Memcpy(base, call, screen + 30832, screen + 30856, 24, frame, lr.b_memcpy);
      }
      GoSecondHalf(base, s, 18);
    }
  }
  return MoveDecal(base, call, screen, frame, flags.native_slots, edit);
}

}  // namespace

uint64_t MoveDecalPosition(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82200DC0 TuningScreen::MoveDecalPosition");
  return MoveDecal(base, call, screen, sp, native_slots, Edit::kPosition);
}
uint64_t MoveDecalAngle(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_822012E8 TuningScreen::MoveDecalAngle");
  return MoveDecal(base, call, screen, sp, native_slots, Edit::kAngle);
}
uint64_t MoveDecalScale(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_82201620 TuningScreen::MoveDecalScale");
  return MoveDecal(base, call, screen, sp, native_slots, Edit::kScale);
}
uint64_t InputDecalPosition(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82206A50 TuningScreen::InputDecalPosition");
  return InputDecal(base, call, screen, sp, flags, Edit::kPosition);
}
uint64_t InputDecalAngle(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82206F48 TuningScreen::InputDecalAngle");
  return InputDecal(base, call, screen, sp, flags, Edit::kAngle);
}
uint64_t InputDecalScale(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_82207320 TuningScreen::InputDecalScale");
  return InputDecal(base, call, screen, sp, flags, Edit::kScale);
}

}  // namespace tuning_pages
