// The race's end: see race_end.h and docs/functions/race-end.md.

#include "race_end/race_end.h"

#include "base/log.h"
#include "career/career.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "network_mode/network_mode.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_end {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`: its back chain.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// An answer asking for mode `type`: built at the frame's +80 (word +84 the
// type, byte +80 1; +81 to +83 left as the stack has them), then its 8
// bytes stored at `answer` (one doubleword in the original).
void Answer(uint8_t* base, uint32_t answer, uint32_t frame, uint32_t type) {
  Store(base, frame + 84, type);
  guest::StoreU8(base, frame + 80, 1);
  const uint32_t w0 = Load(base, frame + 80), w1 = Load(base, frame + 84);
  Store(base, answer, w0);
  Store(base, answer + 4, w1);
}

// The race data's flags word.
uint32_t FlagsAt(uint8_t* base) { return Load(base, kRaceData) + kFlags; }

// The flag `bit` cleared (the word loaded again, as the original does).
void ClearFlag(uint8_t* base, uint32_t at, uint32_t bit) { Store(base, at, Load(base, at) & ~bit); }

// ScreenFades::FadeIn([0x82001BEC] passes, black, layer 0).
void FadeIn(uint8_t* base) {
  mode_tick::ScreenFadesFadeIn(base, Load(base, kFadeInPasses), 0, 0, 0, 0);
}

// Career::EventLocation(save) from the frame (its plain C++; the condition
// tests' depth below the adapter's stack pointer).
void EventLocation(uint8_t* base, uint32_t save, const Calls& calls) {
  career::EventLocation(base, save, calls.event.Down(kModeAfterRaceFrame));
}

// The flags' answer with unlocks to show (stages 2 to 8, but stage 6's and
// 8's without them): 0x8 → 9 (cleared), else 0x10 → cleared,
// Career::EventLocation, 5; else nothing asked.
void UnlocksAnswer(uint8_t* base, uint32_t answer, uint32_t frame, uint32_t save,
                   const Calls& calls) {
  const uint32_t at = FlagsAt(base);
  const uint32_t flags = Load(base, at);
  if (flags & 0x8) {
    Answer(base, answer, frame, 9);
    ClearFlag(base, at, 0x8);
  } else if (flags & 0x10) {
    ClearFlag(base, at, 0x10);
    EventLocation(base, save, calls);
    Answer(base, answer, frame, 5);
  }
}

// Stages 6 and 8 without unlocks to show: the boot movies' table {first,
// second, 11, 11, 11} then the career's result (9), skippable, from the
// first; the boot mode (3), the fade-in and Progress::CheckUnlocks.
void MoviesAnswer(uint8_t* base, uint32_t answer, uint32_t frame, uint32_t save,
                  uint32_t first, uint32_t second, const Calls& calls) {
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, kBootMovies + 4 * k, 11);
  }
  Store(base, kBootMovies + 28, 0);
  Store(base, kBootMovies + 24, 1);
  Store(base, kBootMovies + 0, first);
  Store(base, kBootMovies + 4, second);
  Store(base, frame + 84, 3);
  Store(base, kBootMovies + 20, 9);
  guest::StoreU8(base, frame + 80, 1);
  const uint32_t w0 = Load(base, frame + 80), w1 = Load(base, frame + 84);
  Store(base, answer, w0);
  Store(base, answer + 4, w1);
  FadeIn(base);
  if (career::CheckUnlocksNative(base, frame)) {
    career::ProgressCheckUnlocks(base, save, frame);
  } else {
    calls.guest.Call(kCheckUnlocks, frame, 0x82151024, save);
  }
}

}  // namespace

void ModeAfterRace(uint8_t* base, uint32_t answer, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82150E00 RaceMode::ModeAfterRace");
  const uint32_t frame = Frame(base, sp, kModeAfterRaceFrame);
  const uint32_t race = Load(base, kRaceData);
  Store(base, answer + 4, 0);
  const uint32_t flags = Load(base, race + kFlags);
  guest::StoreU8(base, answer, 0);
  const auto title = [&] {
    // The title (4), the title's flag set.
    Store(base, frame + 84, 4);
    guest::StoreU8(base, frame + 80, 1);
    const uint32_t w0 = Load(base, frame + 80), w1 = Load(base, frame + 84);
    Store(base, kTitleFlag, 1);
    Store(base, answer, w0);
    Store(base, answer + 4, w1);
  };
  if (flags & 0x8000) {
    title();
    return;
  }
  if (flags & 0x10000) {
    network_mode::SessionFree(base, frame, calls.guest);
    Answer(base, answer, frame, 12);
    return;
  }
  const int32_t kind = int32_t(Load(base, race + kKind));
  switch (kind) {
    case 0:
      FadeIn(base);
      title();
      return;
    case 2:
      Answer(base, answer, frame, 6);
      return;
    case 3:
      Answer(base, answer, frame, 7);
      return;
    case 4:
      Answer(base, answer, frame, 8);
      return;
    case 5: {
      const bool network = int32_t(Load(base, race + kRaceMode)) == 7 && Load(base, kSession) != 0;
      Answer(base, answer, frame, network ? 12 : 10);
      return;
    }
    case 6:
      Answer(base, answer, frame, 12);
      return;
    default:
      break;
  }
  // The career (kind 1, or any other).
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const int32_t race_mode = int32_t(Load(base, Load(base, kRaceData) + kRaceMode));
  if (race_mode == 0 || race_mode == 1) {
    Answer(base, answer, frame, 2);
    return;
  }
  const int32_t stage =
      career::AfterRaceNative(base, frame)
          ? int32_t(career::ProgressAfterRace(base, save, frame))
          : int32_t(uint32_t(calls.guest.Call(kAfterRace, frame, 0x82150EB8, save)));
  if (stage == 6) {
    if (Load(base, save + kUnlocksShown) != 0) {
      UnlocksAnswer(base, answer, frame, save, calls);
      Store(base, save + kStageShown, 6);
    } else {
      MoviesAnswer(base, answer, frame, save, 6, 7, calls);
    }
    return;
  }
  if (stage == 8) {
    MoviesAnswer(base, answer, frame, save, 8, 9, calls);
    return;
  }
  if (stage != -1) {
    Store(base, save + kStageShown, uint32_t(stage));
    if (Load(base, save + kUnlocksShown) != 0) {
      UnlocksAnswer(base, answer, frame, save, calls);
    } else {
      Answer(base, answer, frame, 9);
    }
    return;
  }
  // Nothing reached.
  const uint32_t at = FlagsAt(base);
  const uint32_t now = Load(base, at);
  if (now & 0x8) {
    Store(base, at, now & ~0x8u);
    uint32_t type = 9;
    if (int32_t(Load(base, Load(base, kRaceData) + kKind)) == 1) {
      const int32_t day = int32_t(Load(base, vehicle_accessors::SaveDataOf(base, 0) + 6100));
      if (day <= 999 && day == 1) {
        type = 2;
      }
    }
    Answer(base, answer, frame, type);
  } else if (now & 0x10) {
    ClearFlag(base, at, 0x10);
    EventLocation(base, save, calls);
    Answer(base, answer, frame, 5);
  } else {
    Answer(base, answer, frame, 9);
  }
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82151EF0 RaceMode::NextMode");
  const uint32_t frame = Frame(base, sp, kNextModeFrame);
  Store(base, answer + 4, 0);
  guest::StoreU8(base, answer, 0);
  // The messages, as the original inlines GameMessages::Post: the count + 1
  // stored, then the message and two zero words at (start + count - 1) mod
  // 50 (unsigned).
  const uint32_t ring = mode + kRing;
  const auto post = [&](uint32_t message) {
    const uint32_t count = Load(base, ring + 600) + 1;
    const uint32_t at = Load(base, ring + 604) + count - 1;
    Store(base, ring + 600, count);
    const uint32_t slot = ring + (at % 50) * 12;
    Store(base, slot, message);
    Store(base, slot + 4, 0);
    Store(base, slot + 8, 0);
  };
  if (Load(base, mode + kPostUpdate) != 0) {
    post(0);
  }
  post(1);
  const int32_t state = int32_t(Load(base, mode + kMachineState));
  if (state == 3 || state == 4) {
    race_loading::Run(base, ~0ull, frame, 0x82151FD4, calls.loading);
  }
  race_loading::CheckSystemRequest(base, frame, calls.loading);
  calls.guest.Call(kDispatch, frame, 0x82151FE0, mode + kDispatcher);
  if (int32_t(Load(base, mode + kMachineState)) != 35) {
    return;
  }
  Calls inner = calls;
  inner.event = calls.event.Down(kNextModeFrame);
  const uint32_t temporary = frame + 80;
  ModeAfterRace(base, temporary, frame, inner);
  const uint32_t w0 = Load(base, temporary), w1 = Load(base, temporary + 4);
  Store(base, answer, w0);
  Store(base, answer + 4, w1);
}

}  // namespace race_end
