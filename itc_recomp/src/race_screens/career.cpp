// The career's results' pages and transitions
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The screen and its movie (+180), read again at each use.
uint32_t Screen(uint8_t* base) { return Get(base, kCareerResults); }
uint32_t Movie(uint8_t* base) { return guest::LoadU32(base, Screen(base) + 180); }

// UiMovie::Play(movie, -1, index, 0, 0, 0, 0) through its override.
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t index) {
  const uint64_t args[8] = {movie, kNone, index, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

}  // namespace

uint64_t CareerResultsIntro(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82219210 CareerResults::Intro");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t kind = guest::LoadU32(base, Screen(base) + 0);
    if (kind < 3) {
      Play(calls, frame, 0x82219284, Movie(base), kind);
    }
  }
  return uint32_t(Run(calls, frame, 0x82219294, Movie(base))) == 0 ? 1 : 0;
}

uint64_t CareerResultsShow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822192B0 CareerResults::Show");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    Play(calls, frame, 0x822192FC, Movie(base), 3);
    calls.Call(kLoop, frame, 0x82219310, Movie(base), kNone, 3);
    const uint32_t kind = guest::LoadU32(base, Screen(base) + 0);
    if (kind < 3) {
      const uint32_t lrs[3] = {0x8221939C, 0x82219374, 0x8221934C};
      Play(calls, frame, lrs[kind], Movie(base), 11 + kind);
      calls.Call(kLoop, frame, 0x822193B0, Movie(base), kNone, 11 + kind);
    }
  }
  Run(calls, frame, 0x822193C0, Movie(base));
  return 1;
}

uint64_t CareerResultsClose1(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822193D8 CareerResults::Close1");
  const uint32_t frame = Frame(base, sp, 96);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    calls.Call(kFinish, frame, 0x82219414, Movie(base), kNone, 3);
    const uint32_t kind = guest::LoadU32(base, Screen(base) + 0);
    if (kind < 3) {
      calls.Call(kFinish, frame, 0x82219450, Movie(base), kNone, 11 + kind);
    }
    Play(calls, frame, 0x82219474, Movie(base), 4);
  }
  return uint32_t(Run(calls, frame, 0x82219484, Movie(base))) == 0 ? 1 : 0;
}

uint64_t CareerResultsPage1(uint8_t* base, uint32_t sp, bool native_slots, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82219A78 CareerResults::Page1");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t entry = guest::LoadU32(base, kControllerEntries) * 8 + kMapperEntries;
  const uint32_t screen = Screen(base);
  // Accept, then back: each 0 when the entry's +4 is 1, else the slot's low
  // byte.
  auto pressed = [&](uint32_t action, uint32_t lr) {
    if (int32_t(guest::LoadU32(base, entry + 4)) == 1) {
      return false;
    }
    return (MapperPressed(base, guest::LoadU32(base, entry), action, frame, lr, native_slots,
                          calls) &
            0xFF) != 0;
  };
  if (!pressed(159, 0x82219AC8) && !pressed(158, 0x82219AF4)) {
    return 0;
  }
  calls.Call(kMenuSoundPlay, frame, 0x82219B04, 0);
  uint64_t page = 4, transition = 6;
  if (int32_t(guest::LoadU32(base, screen + 20)) > 0) {
    page = 2;
    transition = 2;
  } else if (double(guest::LoadFloat(base, screen + 24)) >
             double(guest::LoadFloat(base, race_hud_object::kZero))) {
    page = 3;
    transition = 3;
  }
  calls.Call(kPagesGo, frame, 0x82219B54, uint64_t(Screen(base)) + 184, page, transition, kNone);
  return 0;
}

}  // namespace race_screens
