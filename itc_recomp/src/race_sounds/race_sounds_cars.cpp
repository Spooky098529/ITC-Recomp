// The race's sounds: the cars' sounds' set-up and update and the music's
// picks (docs/functions/race-sounds.md): see race_sounds.h.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "car_sounds/car_sounds.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_sounds {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// memset(address, 0, 24): the same bytes.
void Clear24(uint8_t* base, uint32_t address) {
  for (uint32_t i = 0; i < 24; i++) {
    guest::StoreU8(base, address + i, 0);
  }
}

// The car sound of vehicle `v` as the loops form it: 0 while the array is
// 0.
uint32_t CarSoundOf(uint8_t* base, uint32_t v) {
  const uint32_t array = Load(base, kCarSounds);
  return array == 0 ? 0 : array + kCarSoundBytes * v;
}

// One car's sound set up over its slot's buffers, as SetUpCars and
// UpdateCars do it (`effects` set: the effects' view set up again first):
// CarSound::Stop, the two views cleared and set up, CarSound::Start with
// the effects' view, CarSounds::SetBanks (its r4 and r5, the indexes, left
// for the next call, as the original's registers), the vehicle, +1252 =
// 1.0, the slot no longer asked. Returns r3 (Vehicle::Get's).
uint64_t SetUpCar(uint8_t* base, uint32_t machine, uint32_t v, uint32_t frame, bool effects,
                  uint32_t stop_lr, uint32_t start_lr, const Calls& calls) {
  const uint32_t sound = CarSoundOf(base, v);
  calls.Call(kCarSoundStop, frame, stop_lr, 1, sound);
  if (effects) {
    car_sounds::WaveBankViewInit(base, machine + kEffectsView, Load(base, machine + kEffectsBank));
  }
  const uint32_t views = kCarViews + 8 * v;
  Clear24(base, Load(base, views));
  Clear24(base, Load(base, views + 4));
  car_sounds::WaveBankViewInit(base, Load(base, views + 4), Load(base, kSlotBuffer1 + 4 * v));
  car_sounds::WaveBankViewInit(base, Load(base, views), Load(base, kSlotBuffer0 + 4 * v));
  const uint32_t j = Load(base, kBankIndexJ + 4 * v);
  const uint32_t i = Load(base, kBankIndexI + 4 * v);
  calls.Call(kCarSoundStart, frame, start_lr, 3, sound, views, machine + kEffectsView);
  car_sounds::SetBanks(base, sound, i, j);
  calls.Leave(i, j);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, v);
  guest::StoreFloat(base, sound + 1252, float(double(guest::LoadFloat(base, kOne))));
  Store(base, sound + 400, vehicle);
  Store(base, kSlotAsked + 4 * v, 0);
  // A compare of i - 18 with 26 follows, with no branch after it (dead).
  return vehicle;
}

// Forgets the calls' leftovers when the function ends.
struct ForgetAtEnd {
  const Calls& calls;
  ~ForgetAtEnd() { calls.Forget(); }
};

// The racer n's vehicle index.
uint32_t RacerVehicle(uint8_t* base, uint32_t n) {
  return Load(base, Load(base, Load(base, kRace) + 2308 + 4 * n) + 20);
}

}  // namespace

uint64_t SetUpCars(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176188 RaceSounds::SetUpCars");
  const ForgetAtEnd forget{calls};
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 224);
  Store(base, machine + kSetUpWait, 10);
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    if (Load(base, kEffectsBankArrived) == 0 || Load(base, kSlotArrived + 4 * i) == 0) {
      continue;
    }
    r3 = SetUpCar(base, machine, i, frame, true, 0x8217622C, 0x8217628C, calls);
    Store(base, machine + kCarsStep, 2);
    Store(base, machine + kFlags, Load(base, machine + kFlags) | 0x20000);
  }
  return r3;
}

uint64_t UpdateCars(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175F20 RaceSounds::UpdateCars");
  const ForgetAtEnd forget{calls};
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 240);
  const int32_t wait = int32_t(Load(base, machine + kSetUpWait));
  if (wait > 0) {
    Store(base, machine + kSetUpWait, uint32_t(wait - 1));
    return r3;
  }
  const uint32_t flags = Load(base, machine + kFlags);
  if ((flags & 0x4000) || !(flags & 0x20000)) {
    return r3;
  }
  // Each racer's car set up again when asked and both its files arrived
  // (Race::RacerCount asked again after each).
  uint32_t count = track_collision::RaceRacerCount(base);
  r3 = count;
  for (int32_t n = 0; n < int32_t(count);) {
    const uint32_t v = RacerVehicle(base, uint32_t(n));
    if (Load(base, kSlotAsked + 4 * v) != 0 && Load(base, kSlotArrived + 4 * v) != 0) {
      SetUpCar(base, machine, v, frame, false, 0x82176008, 0x82176064, calls);
    }
    n++;
    count = track_collision::RaceRacerCount(base);
    r3 = count;
  }
  // Each racer's car sound updated unless asked again.
  count = track_collision::RaceRacerCount(base);
  r3 = count;
  for (int32_t n = 0; n < int32_t(count);) {
    const uint32_t v = RacerVehicle(base, uint32_t(n));
    if (Load(base, kSlotAsked + 4 * v) == 0) {
      calls.Call(kCarSoundUpdate, frame, 0x821760F8, 1, CarSoundOf(base, v));
    }
    n++;
    count = track_collision::RaceRacerCount(base);
    r3 = count;
  }
  // The system UI: the car sounds paused while either word is set, resumed
  // once both are clear.
  uint32_t a = Load(base, kSystemUi0), b = Load(base, kSystemUi1);
  if (a != 0 || b != 0) {
    if (Load(base, machine + kCarsPaused) == 0) {
      r3 = PauseCars(base, frame, calls);
      Store(base, machine + kCarsPaused, 1);
      a = Load(base, kSystemUi0);
      b = Load(base, kSystemUi1);
    }
    if (a != 0 || b != 0) {
      return r3;
    }
  }
  if (Load(base, machine + kCarsPaused) != 0) {
    r3 = ResumeCars(base, frame, calls);
    Store(base, machine + kCarsPaused, 0);
  }
  return r3;
}

uint64_t PickRivalMusic(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821764F0 RaceSounds::PickRivalMusic");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 112);
  const uint32_t results = Load(base, kProgress) + 2184;
  const uint32_t count = Load(base, results);
  const int32_t item = int32_t(Load(base, results + 972 * count - 964));
  uint32_t flag = 0x40;
  if (int32_t(count) != 1 && item != -1) {
    r3 = data_tables::Item(base, uint32_t(item));
    // The class byte, sign-extended, compared unsigned.
    const uint32_t kind = uint32_t(int32_t(int8_t(guest::LoadU8(base, uint32_t(r3) + 35))));
    if (kind < 1) {
      flag = 0x40;
    } else if (kind == 1) {
      flag = 0x100;
    } else if (kind < 3) {
      flag = 0x80;
    } else if (kind == 3) {
      if (item < 383) {
        flag = 0x200;
      } else if (item <= 390) {
        flag = 0x400;
      } else if (item <= 391) {
        flag = 0x200;
      } else if (item <= 397) {
        flag = 0x800;
      } else {
        flag = 0x200;
      }
    } else if (kind < 5) {
      flag = item == 391 ? 0x800 : item == 398 ? 0x1000 : 0x400;
    } else if (kind == 5) {
      flag = 0x2000;
    }
  }
  Store(base, machine + kFlags, Load(base, machine + kFlags) | flag);
  return r3;
}

uint64_t PlayRivalMusic(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821762F8 RaceSounds::PlayRivalMusic");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  // Music::Play with r3 = a word 2 at `word`, r4 = the table's entry at
  // `entry`, r5 = a byte 0 at `byte`.
  const auto play = [&](uint32_t entry, uint32_t byte, uint32_t word, uint32_t lr) {
    guest::StoreU8(base, frame + byte, 0);
    Store(base, frame + word, 2);
    r3 = calls.Call(kMusicPlay, frame, lr, 3, frame + word, Load(base, kMusicNames + entry),
                    frame + byte);
  };
  const auto clear = [&](uint32_t flag) {
    Store(base, machine + kFlags, Load(base, machine + kFlags) & ~flag);
  };
  if (Load(base, machine + kFlags) & 0x40) {
    const uint32_t course = Load(base, Load(base, kRaceData) + 32);
    if (course < 1) {
      play(4, 82, 92, 0x8217638C);
    } else if (course == 1) {
      play(8, 81, 88, 0x8217638C);
    } else if (course < 3) {
      play(12, 80, 84, 0x8217638C);
    }
    clear(0x40);
  }
  struct Step {
    uint32_t flag, entry, lr;
  };
  static constexpr Step kSteps[] = {
      {0x100, 20, 0x821763BC},  {0x80, 16, 0x821763EC},   {0x200, 24, 0x8217641C},
      {0x400, 28, 0x8217644C},  {0x800, 32, 0x8217647C},  {0x1000, 36, 0x821764AC},
      {0x2000, 40, 0x821764DC},
  };
  for (const Step& step : kSteps) {
    if (Load(base, machine + kFlags) & step.flag) {
      play(step.entry, 82, 92, step.lr);
      clear(step.flag);
    }
  }
  return r3;
}

}  // namespace race_sounds
