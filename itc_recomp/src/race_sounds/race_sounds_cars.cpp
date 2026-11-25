// The race's sounds: the cars' sounds' set-up and update and the music's
// picks (docs/functions/race-sounds.md): see race_sounds.h.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "car_sounds/car_sounds.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "schema/race_event.h"  // OpponentItem (DataTables::Item's answer)
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_sounds {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// memset(address, 0, 24): the same bytes.
void Clear24(uint32_t address) {
  for (uint32_t i = 0; i < 24; i++) {
    guest::StoreU8(address + i, 0);
  }
}

// One car's sound set up over its slot's buffers, as SetUpCars and
// UpdateCars do it (`effects` set: the effects' view set up again first):
// CarSound::Stop, the two views cleared and set up, CarSound::Start with
// the effects' view, CarSounds::SetBanks (its r4 and r5, the indexes, left
// for the next call, as the original's registers), the vehicle, +1252 =
// 1.0, the slot no longer asked. Returns r3 (Vehicle::Get's).
uint64_t SetUpCar(uint32_t machine, uint32_t v, uint32_t frame, bool effects,
                  uint32_t stop_lr, uint32_t start_lr, const Calls& calls) {
  const uint32_t sound = CarSoundAt(v);
  calls.Call(kCarSoundStop, frame, stop_lr, 1, sound);
  if (effects) {
    car_sounds::WaveBankViewInit(Sounds(machine).effects_view_address(),
                                 Sounds(machine).effects_bank().address);
  }
  const uint32_t views = Globals::car_views_address() + 8 * v;
  Clear24(Load(views));
  Clear24(Load(views + 4));
  car_sounds::WaveBankViewInit(Load(views + 4),
                               Load(Globals::slot_buffers_1_address() + 4 * v));
  car_sounds::WaveBankViewInit(Load(views),
                               Load(Globals::slot_buffers_0_address() + 4 * v));
  const uint32_t j = Load(CarSounds::sound_picks_b_address() + 4 * v);
  const uint32_t i = Load(CarSounds::sound_picks_a_address() + 4 * v);
  calls.Call(kCarSoundStart, frame, start_lr, 3, sound, views,
             Sounds(machine).effects_view_address());
  car_sounds::SetBanks(sound, i, j);
  calls.Leave(i, j);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(v);
  schema::CarSound(sound).set_engine_gain(float(double(RaceStates::one())));
  schema::CarSound(sound).set_vehicle(vehicle);
  Store(Globals::slot_asked_address() + 4 * v, 0);
  // A compare of i - 18 with 26 follows, with no branch after it (dead).
  return vehicle;
}

// Forgets the calls' leftovers when the function ends.
struct ForgetAtEnd {
  const Calls& calls;
  ~ForgetAtEnd() { calls.Forget(); }
};

// The racer n's vehicle index.
uint32_t RacerVehicle(uint32_t n) {
  const uint32_t racers = schema::RaceWorldGlobals::racers_manager().racers_address();
  return schema::MinimapRacerEntry(Load(racers + 4 * n)).vehicle_index();
}

}  // namespace

uint64_t SetUpCars(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82176188 RaceSounds::SetUpCars");
  const ForgetAtEnd forget{calls};
  const uint32_t machine = uint32_t(r3);
  // No slots (E5 step 6): the car sounds' calls made from `sp`.
  const uint32_t frame = sp;
  Sounds(machine).set_set_up_wait(10);
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    if (Globals::effects_bank_arrived() == 0 ||
        Load(Globals::slot_arrived_address() + 4 * i) == 0) {
      continue;
    }
    r3 = SetUpCar(machine, i, frame, true, 0x8217622C, 0x8217628C, calls);
    Sounds(machine).set_cars_step(2);
    Sounds(machine).set_flags(Sounds(machine).flags() | 0x20000);
  }
  return r3;
}

uint64_t UpdateCars(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82175F20 RaceSounds::UpdateCars");
  const ForgetAtEnd forget{calls};
  const uint32_t machine = uint32_t(r3);
  // No slots (E5 step 6): the car sounds' calls made from `sp`.
  const uint32_t frame = sp;
  const int32_t wait = int32_t(Sounds(machine).set_up_wait());
  if (wait > 0) {
    Sounds(machine).set_set_up_wait(uint32_t(wait - 1));
    return r3;
  }
  const uint32_t flags = Sounds(machine).flags();
  if ((flags & 0x4000) || !(flags & 0x20000)) {
    return r3;
  }
  // Each racer's car set up again when asked and both its files arrived
  // (Race::RacerCount asked again after each).
  uint32_t count = track_collision::RaceRacerCount();
  r3 = count;
  for (int32_t n = 0; n < int32_t(count);) {
    const uint32_t v = RacerVehicle(uint32_t(n));
    if (Load(Globals::slot_asked_address() + 4 * v) != 0 &&
        Load(Globals::slot_arrived_address() + 4 * v) != 0) {
      SetUpCar(machine, v, frame, false, 0x82176008, 0x82176064, calls);
    }
    n++;
    count = track_collision::RaceRacerCount();
    r3 = count;
  }
  // Each racer's car sound updated unless asked again.
  count = track_collision::RaceRacerCount();
  r3 = count;
  for (int32_t n = 0; n < int32_t(count);) {
    const uint32_t v = RacerVehicle(uint32_t(n));
    if (Load(Globals::slot_asked_address() + 4 * v) == 0) {
      calls.Call(kCarSoundUpdate, frame, 0x821760F8, 1, CarSoundAt(v));
    }
    n++;
    count = track_collision::RaceRacerCount();
    r3 = count;
  }
  // The system UI: the car sounds paused while either word is set, resumed
  // once both are clear.
  uint32_t a = schema::BootModeGlobals::pad_blocked0(), b = schema::BootModeGlobals::pad_blocked1();
  if (a != 0 || b != 0) {
    if (Sounds(machine).cars_paused() == 0) {
      r3 = PauseCars(frame, calls);
      Sounds(machine).set_cars_paused(1);
      a = schema::BootModeGlobals::pad_blocked0();
      b = schema::BootModeGlobals::pad_blocked1();
    }
    if (a != 0 || b != 0) {
      return r3;
    }
  }
  if (Sounds(machine).cars_paused() != 0) {
    r3 = ResumeCars(frame, calls);
    Sounds(machine).set_cars_paused(0);
  }
  return r3;
}

uint64_t PickRivalMusic(uint64_t r3, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821764F0 RaceSounds::PickRivalMusic");
  const uint32_t machine = uint32_t(r3);
  // No frame (E5 step 6): the original's held only its back chain.
  // The results record's last entry's item (+972 * count - 964: entry
  // count - 1's +4).
  const uint32_t results = RaceStates::progress().results_address();
  const uint32_t count = schema::RacerResultsRecord(results).count();
  const int32_t item =
      int32_t(schema::RacerResultsRecord(results + 972 * (count - 1)).e0_item());
  uint32_t flag = 0x40;
  if (int32_t(count) != 1 && item != -1) {
    r3 = data_tables::Item(uint32_t(item));
    // The class byte (race-event.toml's OpponentItem), sign-extended,
    // compared unsigned.
    const uint32_t kind =
        uint32_t(int32_t(int8_t(schema::OpponentItem(uint32_t(r3)).kind())));
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
  Sounds(machine).set_flags(Sounds(machine).flags() | flag);
  return r3;
}

uint64_t PlayRivalMusic(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821762F8 RaceSounds::PlayRivalMusic");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(sp, 144);
  // Music::Play with r3 = a word 2 at `word`, r4 = the table's entry at
  // `entry`, r5 = a byte 0 at `byte`.
  const auto play = [&](uint32_t entry, uint32_t byte, uint32_t word, uint32_t lr) {
    guest::StoreU8(frame + byte, 0);
    Store(frame + word, 2);
    r3 = calls.Call(kMusicPlay, frame, lr, 3, frame + word,
                    Load(Globals::music_names_address() + entry), frame + byte);
  };
  const auto clear = [&](uint32_t flag) {
    Sounds(machine).set_flags(Sounds(machine).flags() & ~flag);
  };
  if (Sounds(machine).flags() & 0x40) {
    const uint32_t course = RaceStates::race_data().mode();
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
    if (Sounds(machine).flags() & step.flag) {
      play(step.entry, 82, 92, step.lr);
      clear(step.flag);
    }
  }
  return r3;
}

}  // namespace race_sounds
