// The options screen's category pages (docs/functions/options-screen.md):
// see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Settings(uint8_t* base) { return LoadU32(base, kSettings); }
uint32_t Menu(uint8_t* base) { return LoadU32(base, kMenu); }

// The page block to `step` with `after` the page after and `queued` the
// step queued (the page before kept when the page changes).
void Go(uint8_t* base, uint32_t after, uint32_t step, uint32_t queued) {
  const uint32_t p = LoadU32(base, kPage);
  const uint32_t page = LoadU32(base, p + 0);
  if (page != after) {
    StoreU32(base, p + 8, page);
  }
  StoreU32(base, p + 16, queued);
  StoreU32(base, p + 4, after);
  StoreU32(base, p + 0, kMinusOne);
  StoreU32(base, p + 12, step);
}

// `count` words from `from` to `to` (the originals' inline copies).
void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    StoreU32(base, to + 4 * i, LoadU32(base, from + 4 * i));
  }
}

// memcpy(to, from, bytes) from `frame` returning to `lr`: its plain C++
// where it takes it (r3 the destination), else the guest's.
uint64_t Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t frame,
              uint32_t lr, const Calls& calls) {
  if (native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    return to;
  }
  return calls.Call(kMemcpy, frame, lr, to, from, bytes);
}

uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint64_t sound) {
  return calls.Call(kMenuSoundPlay, frame, lr, sound);
}

bool Pressed(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t action,
             uint64_t& r3) {
  r3 = calls.Call(kMenuPressed, frame, lr, Menu(base), action);
  return uint32_t(r3) != 0;
}

}  // namespace

uint64_t HudPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1270 Options::HudPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821C128C, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    r3 = Sound(calls, frame, 0x821C12A4, 4);
    Go(base, 1, 10, kMinusOne);
    return r3;
  }
  if (uint32_t(StepHud(base, frame)) != 0) {
    r3 = Sound(calls, frame, 0x821C12E8, 4);
    Go(base, 1, 11, kMinusOne);
    return r3;
  }
  if (Pressed(base, calls, frame, 0x821C1314, 159, r3)) {
    r3 = Sound(calls, frame, 0x821C1324, 0);
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 4 && choice == 1) {
      // OK: the edits applied.
      CopyWords(base, o + 44, o + 236, 4);
      r3 = ShowHudValues(base, frame);
      Go(base, kMinusOne, 0, 12);
      return r3;
    }
    if (cursor == 4 && choice == 0) {
      // Default: the defaults into the edits.
      CopyWords(base, o + 236, o + 428, 4);
      return ShowHudValues(base, frame);
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 4);
    StoreU32(base, Settings(base) + 660, 1);
    Go(base, 1, 10, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C1400, 158, r3)) {
    return r3;
  }
  // Back: the edits dropped.
  Sound(calls, frame, 0x821C1410, 2);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 236, o + 44, 4);
  r3 = ShowHudValues(base, frame);
  Go(base, kMinusOne, 0, 12);
  return r3;
}

uint64_t SlidersPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1888 Options::SlidersPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821C18A4, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    Go(base, 3, 18, kMinusOne);
    return Sound(calls, frame, 0x821C191C, 4);
  }
  if (uint32_t(StepSliders(base, frame)) != 0) {
    Go(base, 3, 19, kMinusOne);
    return Sound(calls, frame, 0x821C191C, 4);
  }
  if (Pressed(base, calls, frame, 0x821C192C, 159, r3)) {
    r3 = Sound(calls, frame, 0x821C193C, 0);
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 3 && choice == 0) {
      // OK: the edits applied.
      CopyWords(base, o + 68, o + 260, 3);
      r3 = ShowSliderMarks(base, frame);
      Go(base, kMinusOne, 0, 20);
      return r3;
    }
    if (cursor == 3 && choice == 1) {
      // Default: the defaults into the edits.
      CopyWords(base, o + 260, o + 452, 3);
      return ShowSliderNumbers(base, frame, calls);
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 3);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 3, 18, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C1A1C, 158, r3)) {
    return r3;
  }
  // Back: the edits dropped.
  Sound(calls, frame, 0x821C1A2C, 2);
  const uint32_t o = Settings(base);
  CopyWords(base, o + 260, o + 68, 3);
  r3 = ShowSliderNumbers(base, frame, calls);
  Go(base, kMinusOne, 0, 20);
  return r3;
}

uint64_t ButtonsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C1D08 Options::ButtonsPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x821C1D28, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    r3 = Sound(calls, frame, 0x821C1D40, 4);
    Go(base, 5, 26, kMinusOne);
    return r3;
  }
  if (StepButtons(base) != 0) {
    r3 = Sound(calls, frame, 0x821C1D84, 4);
    Go(base, 5, 27, kMinusOne);
    return r3;
  }
  if (Pressed(base, calls, frame, 0x821C1DB0, 159, r3)) {
    const int32_t cursor = int32_t(LoadU32(base, Menu(base) + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 12 && choice == 0) {
      // OK: kept only without a button used twice (else sound 3).
      const uint32_t clash = HasButtonClash(base, frame);
      if (clash != 0) {
        return Sound(calls, frame, 0x821C1E40, 3);
      }
      Sound(calls, frame, 0x821C1DEC, clash);
      const uint32_t settings = Settings(base);
      r3 = Copy(base, settings + 92, settings + 284, 48, frame, 0x821C1E00, calls);
      Go(base, kMinusOne, 1, 28);
      return r3;
    }
    if (cursor == 12 && choice == 1) {
      // Default: the defaults into the edits, shown.
      Copy(base, o + 284, o + 476, 48, frame, 0x821C1E30, calls);
      ShowButtonList(base, frame, calls);
      ShowButtonClashes(base, frame, calls);
      return Sound(calls, frame, 0x821C1E40, 0);
    }
    // The cursor sent to OK.
    r3 = Sound(calls, frame, 0x821C1E4C, 0);
    const uint32_t menu = Menu(base);
    const uint32_t before = LoadU32(base, menu + 0);
    StoreU32(base, menu + 0, 12);
    StoreU32(base, menu + 4, before);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 5, 26, kMinusOne);
    return r3;
  }
  if (!Pressed(base, calls, frame, 0x821C1E7C, 158, r3)) {
    return r3;
  }
  // Back (the edits not put back: the page's set-up copies them again).
  r3 = Sound(calls, frame, 0x821C1E8C, 2);
  Go(base, kMinusOne, 1, 28);
  return r3;
}

uint64_t AnalogPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C21A8 Options::AnalogPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMenuStep, frame, 0x821C21C4, Menu(base));
  uint64_t r3;
  if (LoadU32(base, Menu(base) + 8) != 0) {
    Go(base, 7, 34, kMinusOne);
    return Sound(calls, frame, 0x821C2374, 4);
  }
  if (StepAnalog(base) != 0) {
    Go(base, 7, 35, kMinusOne);
    return Sound(calls, frame, 0x821C2374, 4);
  }
  if (Pressed(base, calls, frame, 0x821C2240, 159, r3)) {
    const uint32_t menu = Menu(base);
    const int32_t cursor = int32_t(LoadU32(base, menu + 0));
    const uint32_t o = Settings(base);
    const int32_t choice = int32_t(LoadU32(base, o + 660));
    if (cursor == 6 && choice == 0) {
      // OK: kept only without an input used twice (else sound 3).
      if (HasAnalogClash(base, frame) != 0) {
        return Sound(calls, frame, 0x821C2374, 3);
      }
      Copy(base, o + 188, o + 380, 24, frame, 0x821C2288, calls);
      Go(base, kMinusOne, 2, 36);
      return Sound(calls, frame, 0x821C2374, 0);
    }
    if (cursor == 6 && choice == 1) {
      // Default: the defaults into the edits, shown.
      Copy(base, o + 380, o + 572, 24, frame, 0x821C22D4, calls);
      ShowAnalogList(base, frame, calls);
      ShowAnalogClashes(base, frame, calls);
      return Sound(calls, frame, 0x821C2374, 0);
    }
    // The cursor sent to OK.
    StoreU32(base, menu + 4, LoadU32(base, menu + 0));
    StoreU32(base, menu + 0, 6);
    StoreU32(base, Settings(base) + 660, 0);
    Go(base, 7, 34, kMinusOne);
    return Sound(calls, frame, 0x821C2374, 0);
  }
  if (!Pressed(base, calls, frame, 0x821C2330, 158, r3)) {
    return r3;
  }
  // Back (the edits not put back).
  Go(base, kMinusOne, 2, 36);
  return Sound(calls, frame, 0x821C2374, 2);
}

}  // namespace options_screen
