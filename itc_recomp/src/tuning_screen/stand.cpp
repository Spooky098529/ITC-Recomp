// The tuning screen's calls onto the stand (docs/functions/tuning-screen.md):
// see tuning_screen.h. The stand's setters, the car makers and the front end's
// car draw through their plain C++ (stand_models, car_makers,
// front_end_car_draw), each from the original's frame; the UI movie calls
// through their overrides. Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt): the decals' angles and the conversions are the translated
// code's, rounded to single.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "car_makers/car_makers.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "stand_models/stand_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cmath>
#include <climits>

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
double Lfs(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(Load(base, address)));
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kHolder = 0x826D83E0;   // [it] the stand's holder word
constexpr uint32_t kZero = 0x82000A34;     // 0.0f
constexpr uint32_t kDegree = 0x82002948;   // π/180
constexpr uint32_t kRefresh = 0x820C4998;  // Car::Refresh

uint32_t Holder(uint8_t* base) { return Load(base, Load(base, kHolder)); }

// A garage car's record of the save data `save` (its list at +6120 walked
// [+0x19920] steps from the head halfword +204): the list + 1,968 x slot (its
// fields at + 212 on).
uint32_t GarageCarOf(uint8_t* base, uint32_t save) {
  const uint32_t nodes = save + 6120;
  uint32_t n = Load(base, save + 0x19920);
  uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, nodes + 204)))) * 4 + nodes;
  while (n != 0) {
    node = uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) * 4 + nodes;
    n--;
  }
  const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
  return uint32_t(uint64_t(slot) * 1968) + nodes;
}

// fcfid then frsp: a word's signed value as a single.
double ToSingle(int64_t value) { return double(float(double(value))); }

// fctiwz as the translated code: NaN 0x80000000, saturated above.
int64_t Fctiwz(double value) {
  if (std::isnan(value)) {
    return int64_t(0x80000000u);
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value <= double(INT_MIN)) {
    return INT_MIN;
  }
  return int64_t(int32_t(value));
}

// The car object at `car` from the garage car `garage`: Car::Init, memcpy of
// its 1,884 bytes, the flags +3888 |= `flags`, Car::Refresh (its original
// through `calls` where its plain C++ declines), from `frame` with the
// original's return addresses.
void MakeCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t car,
             uint32_t garage, uint32_t flags, uint32_t frame, uint32_t memcpy_lr,
             uint32_t refresh_lr) {
  race_setup::CarInit(base, car);
  front_end_car_draw::Memcpy(base, calls, car, garage, 1884, frame, memcpy_lr);
  if (flags != 0) {
    Store(base, car + 3888, Load(base, car + 3888) | flags);
  }
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, car, &refreshed)) {
    const uint64_t regs[1] = {car};
    calls.call(kRefresh, regs, 1, nullptr, 0, frame, refresh_lr, nullptr);
  }
}

}  // namespace

uint64_t SetInteriorPair(uint8_t* base, const GuestCall& call, uint64_t r4, uint64_t r5,
                         uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CCC50 TuningScreen::SetInteriorPair");
  const uint32_t choice = uint32_t(r4), level = uint32_t(r5);
  const uint32_t k = choice < 5 ? choice : 0;
  uint32_t w = 0;
  switch (k) {
    case 1:
    case 2: {
      constexpr uint32_t kWords[] = {2, 1, 3, 0};
      w = level < 4 ? kWords[level] : 0;
      break;
    }
    case 3: {
      constexpr uint32_t kWords[] = {1, 2, 0};
      w = level < 3 ? kWords[level] : 0;
      break;
    }
    case 4: {
      constexpr uint32_t kWords[] = {1, 2, 0, 3};
      w = level < 4 ? kWords[level] : 0;
      break;
    }
    default:
      break;
  }
  // The tail call from the same stack pointer.
  return stand_models::SetInteriorPair(base, call, Holder(base), k, w, sp);
}

uint64_t FitSelectedCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t holder,
                        uint64_t r4, uint64_t r5, uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_8217DAC0 GarageStage::FitSelectedCar");
  const uint32_t frame = Frame(base, sp, 4048);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t g = GarageCarOf(base, save) + 212;
  const int64_t a = int8_t(guest::LoadU8(base, g + 88));
  const int64_t b = int8_t(guest::LoadU8(base, g + 89));
  guest::StoreU8(base, g + 88, uint8_t(r4));
  guest::StoreU8(base, g + 89, uint8_t(r5));
  Store64(base, frame + 88, uint64_t(a));
  Store64(base, frame + 80, uint64_t(b));
  const double old_a = ToSingle(a), old_b = ToSingle(b);
  const uint32_t car = frame + 96;
  MakeCar(base, calls, car, g, 0, frame, 0x8217DB7C, 0x8217DB84);
  const uint64_t r3 = car_makers::GarageStageFitCar(base, calls, holder, car, frame, f1);
  const int64_t back_a = Fctiwz(old_a), back_b = Fctiwz(old_b);
  Store64(base, frame + 80, uint64_t(back_a));
  Store64(base, frame + 88, uint64_t(back_b));
  guest::StoreU8(base, g + 88, guest::LoadU8(base, frame + 87));
  guest::StoreU8(base, g + 89, guest::LoadU8(base, frame + 95));
  return r3;
}

uint64_t PickCameraPreset(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D99A0 TuningScreen::PickCameraPreset");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t s = uint32_t(r3);
  const uint32_t save = Load(base, s + kSaveData);
  const uint32_t c = GarageCarOf(base, save);
  const int32_t state = int32_t(Load(base, s + kState));
  auto pick = [&](uint32_t value) { Store(base, s + 25072, value); };
  auto item = [&] { return uint32_t(guest::LoadU8(base, s + kItem)); };
  auto fit = [&](uint64_t a, uint64_t b) {
    double f1 = 0.0;
    r3 = FitSelectedCar(base, stand.calls, Holder(base), a, b, frame, &f1);
  };
  auto word5016 = [&](uint32_t value) {
    r3 = stand_models::SetWord5016(base, Holder(base), value, frame);
  };
  // The wheel sets' 19, 20, 21 by `x` (0, 1, 2; none above).
  auto wheels = [&](uint32_t x) {
    if (x == 0) {
      pick(19);
    } else if (x == 1) {
      pick(20);
    } else if (x == 2) {
      pick(21);
    }
  };
  if (state == 3 || state == 4) {
    pick(15);
    const uint32_t again = GarageCarOf(base, save);
    const int64_t a = int8_t(guest::LoadU8(base, again + 300));
    const int64_t b = int8_t(guest::LoadU8(base, again + 301));
    fit(uint64_t(a), uint64_t(b));
    word5016(0);
  } else if (state == 62) {
    pick(15);
  } else {
    const int32_t m = int32_t(Load(base, s + kCategory));
    if (m == 0 && state == 5) {
      const uint32_t p = item();
      if (p == 0) {
        pick(16);
      } else if (p == 1) {
        const int32_t id = int32_t(Load(base, c + 224));
        pick(id == 14 || id == 15 ? 16 : 17);
      } else if (p == 2) {
        pick(18);
      }
    }
    if (m == 1) {
      if (item() == 3 && (state == 5 || state == 7)) {
        pick(32);
      }
    } else if (m == 0) {
      if (state == 6) {
        pick(18);
      }
    } else if (m == 2) {
      if (state == 5) {
        pick(31);
      }
    } else if (m == 3) {
      if (state == 5 || state == 8) {
        wheels(item());
      }
    } else if (m == 4) {
      if (state == 6) {
        const uint32_t p = item();
        if (p <= 8) {
          pick(22 + p);
        }
      }
    } else if (m == 5) {
      const uint32_t p = item();
      if (p == 4) {
      } else if (p == 5) {
        if (state == 20 || state == 21 || state == 22) {
          const uint32_t x = Load(base, s + 30492);
          if (x == 0) {
            pick(37);
          } else if (x == 1) {
            pick(39);
          } else if (x == 2) {
            pick(38);
          } else if (x == 3) {
            pick(15);
          }
        } else if (state == 19) {
          pick(15);
        }
      } else if (p == 6) {
        if (state == 23) {
          const uint32_t y = Load(base, s + 30724);
          if (y == 0) {
            pick(40);
          } else if (y == 1) {
            pick(41);
          }
        }
      } else if (p == 7) {
        pick(0xFFFFFFFFu);
      } else if (p == 0) {
        if (state == 16 || state == 17) {
          wheels(Load(base, s + 25196));
        } else if (state == 15) {
          pick(15);
        }
      } else if (p == 1) {
        pick(33);
      } else if (p == 2) {
        pick(34);
      } else if (p == 3) {
        pick(35);
      }
    } else if (m == 6) {
      const uint32_t p = item();
      if (p == 3) {
        if (state == 54 || state == 55) {
          pick(64);
          fit(0, 0);
        } else if (state == 53 || state == 56 || state == 57 || state == 58) {
          pick(53);
          fit(0, 0);
        }
      } else if (state == 6) {
        if (p == 0) {
          pick(50);
          fit(0, 0);
        } else if (p == 1 || p == 2) {
          pick(p == 1 ? 51 : 52);
          fit(0, 0);
          word5016(1);
        }
      } else if (state == 7 && p == 0) {
        pick(50);
        fit(0, 0);
      }
    }
  }
  const uint32_t command = Load(base, s + 25072);
  if (int32_t(command) >= 0) {
    r3 = front_end_car_draw::GarageStageCommand(base, stand.calls, Holder(base), command, frame);
  }
  return r3;
}

uint64_t UpdateBusyIcon(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D6100 TuningScreen::UpdateBusyIcon");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t s = uint32_t(r3);
  const uint32_t id = Load(base, s + 10020);
  if (int32_t(id) <= -1) {
    return r3;
  }
  const uint32_t movie = Load(base, MovieEntry(base, frame, id));
  const uint32_t busy = front_end_car_draw::GarageStageIsBusy(base, stand.calls, Holder(base), frame);
  const uint32_t shown = Load(base, s + 10016);
  const GuestCall& call = stand.call;
  if (busy != 0) {
    if (shown != 0) {
      return busy;
    }
    Store(base, s + 10016, 1);
    FinishNamed(call, frame, 0x821D617C, movie, Load(base, s + 10024));
    PlayNamed(call, frame, 0x821D6198, movie, Load(base, s + 10024), 1);
    FinishNamed(call, frame, 0x821D61A4, movie, Load(base, s + 10028));
    PlayNamed(call, frame, 0x821D61C0, movie, Load(base, s + 10028), 1);
    return LoopNamed(call, frame, 0x821D61CC, movie, Load(base, s + 10028));
  }
  if (int32_t(shown) != 1) {
    return busy;
  }
  Store(base, s + 10016, 0);
  FinishNamed(call, frame, 0x821D61EC, movie, Load(base, s + 10028));
  FinishNamed(call, frame, 0x821D61F8, movie, Load(base, s + 10032));
  return PlayNamed(call, frame, 0x821D6214, movie, Load(base, s + 10032), 1);
}

uint64_t PreviewWheels(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp,
                       std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E0E00 TuningScreen::PreviewWheels");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t s = uint32_t(r3);
  const uint32_t c = GarageCarOf(base, Load(base, s + kSaveData));
  auto at = [&](uint32_t offset) { return Load(base, s + offset); };
  const int32_t state = int32_t(at(kState));
  if (state == 16) {
    const uint32_t x = at(25196);
    if (x == 0) {
      const uint32_t group = at(25204);
      if (group == 0) {
        Store(base, c + 1944, group);
        Store(base, c + 1952, group);
      } else {
        Store(base, c + 1944, at(27848));
        Store(base, c + 1952, at(27852));
      }
    } else if (x == 1) {
      const uint32_t group = at(25204);
      Store(base, c + 1944, group == 0 ? group : at(27848));
    } else if (x == 2) {
      const uint32_t group = at(25204);
      Store(base, c + 1952, group == 0 ? group : at(27852));
    }
  } else if (state == 17) {
    const uint32_t x = at(25196);
    if (x == 0) {
      Store(base, c + 1944, at(25228));
      Store(base, c + 1952, at(25228));
    } else if (x == 1) {
      Store(base, c + 1944, at(25228));
    } else if (x == 2) {
      Store(base, c + 1952, at(25228));
    }
  }
  const int32_t now = int32_t(at(kState));
  if (now == 15 || now == 16 || now == 17) {
    const uint32_t x = at(25196);
    if (x < 3) {
      const uint64_t number = WheelNumber(at(25204), at(25220));
      r3 = stand_models::PreviewWheel(base, call, s, number, x, frame, f1);
    }
  }
  if (int32_t(at(kState)) != 17) {
    Store(base, s + 25244, 1);
  }
  return r3;
}

uint64_t ApplyDecals(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822042D8 TuningScreen::ApplyDecals");
  const uint32_t frame = Frame(base, sp, 4112);
  const uint32_t s = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, s + 10012));
  auto decal = [&](uint32_t at, uint32_t entry, uint32_t a, uint32_t b, double f1, double f2,
                   double f3, double f4) {
    r3 = stand_models::SetDecal(base, Holder(base), entry, a, b, f1, f2, f3, f4, frame, at);
  };
  auto clear = [&] {
    const double zero = Lfs(base, kZero);
    for (uint32_t k = 0; k < 3; k++) {
      decal(stand_models::kDecalA, k, 0xFFFFFFFFu, 0, zero, zero, zero, zero);
      decal(stand_models::kDecalB, k, 0xFFFFFFFFu, 0, zero, zero, zero, zero);
    }
    r3 = stand_models::ReloadDecals(base, stand.call, Holder(base), frame);
  };
  if (kind == 5 || kind == 8) {
    clear();
  } else if (kind == 6) {
    const uint32_t g = GarageCarOf(base, Load(base, s + kSaveData)) + 212;
    const uint32_t car = frame + 144;
    MakeCar(base, stand.calls, car, g, 1, frame, 0x822043D0, 0x822043E4);
    const uint64_t pick = uint64_t(int64_t(int16_t(guest::LoadU16(base, g))));
    double f1 = 0.0;
    r3 = car_makers::GarageStageSetCar(base, stand.calls, stand.call, Holder(base), car, pick,
                                       frame, &f1);
  } else if (kind == 7) {
    const double degree = Lfs(base, kDegree);
    // The values the conversions leave: a set's (x, y) at the frame's +88 and
    // +120 (A), +104 and +136 (B), the edited entry's at +80 and +96 (A) or
    // +112 and +128 (B).
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t e = s + 31024 + 24 * k;
      {
        const int64_t y = int32_t(Load(base, e + 4)), x = int32_t(Load(base, e + 0));
        const double f3 = double(float(Lfs(base, e + 8) * degree));
        Store64(base, frame + 88, uint64_t(y));
        Store64(base, frame + 120, uint64_t(x));
        decal(stand_models::kDecalA, k, Load(base, s + 31192 + 4 * k), Load(base, e + 16),
              ToSingle(x), ToSingle(y), f3, Lfs(base, e + 12));
      }
      {
        const double f3 = double(float(Lfs(base, e + 80) * degree));
        const int64_t y = int32_t(Load(base, e + 76)), x = int32_t(Load(base, e + 72));
        Store64(base, frame + 104, uint64_t(y));
        Store64(base, frame + 136, uint64_t(x));
        decal(stand_models::kDecalB, k, Load(base, s + 31204 + 4 * k), Load(base, e + 88),
              ToSingle(x), ToSingle(y), f3, Lfs(base, e + 84));
      }
      if (int32_t(k) == int32_t(Load(base, s + 30732))) {
        const uint32_t set = Load(base, s + 30724);
        const double f0 = Lfs(base, s + 30840);
        const int64_t y = int32_t(Load(base, s + 30836));
        const double f4 = Lfs(base, s + 30844);
        const int64_t x = int32_t(Load(base, s + 30832));
        const uint32_t b = Load(base, s + 30848);
        const double f3 = double(float(f0 * degree));
        if (set == 0) {
          Store64(base, frame + 80, uint64_t(y));
          Store64(base, frame + 96, uint64_t(x));
          decal(stand_models::kDecalA, k, Load(base, s + 31192 + 4 * k), b, ToSingle(x),
                ToSingle(y), f3, f4);
        } else {
          Store64(base, frame + 112, uint64_t(y));
          Store64(base, frame + 128, uint64_t(x));
          decal(stand_models::kDecalB, k, Load(base, s + 31204 + 4 * k), b, ToSingle(x),
                ToSingle(y), f3, f4);
        }
      }
    }
    r3 = stand_models::ReloadDecals(base, stand.call, Holder(base), frame);
  }
  Store(base, s + 10012, 0xFFFFFFFFu);
  return r3;
}

uint64_t DecalCommand(uint8_t* base, const StandCalls& stand, uint32_t screen, uint32_t sp,
                      double* f1) {
  ITC_NATIVE_RUNNING("sub_822019D0 TuningScreen::DecalCommand");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t s = screen;
  const uint32_t movie = Load(base, MovieEntry(base, frame, 13));
  if (Load(base, s + 9716) != 0) {
    if (Load(base, s + 9720) != 0) {
      Advance(base, s);
      Store(base, s + 9720, 0);
    } else {
      FinishNamed(stand.call, frame, 0x82201A64, movie, 240);
      PlayNamed(stand.call, frame, 0x82201A80, movie, 240, 1);
      front_end_car_draw::GarageStageCommand(base, stand.calls, Holder(base), 58, frame);
    }
    Store(base, s + 9716, 0);
  }
  Store(base, s + 10012, 7);
  const uint32_t c = GarageCarOf(base, Load(base, s + kSaveData));
  const int64_t a = int8_t(guest::LoadU8(base, c + 300));
  const int64_t b = int8_t(guest::LoadU8(base, c + 301));
  double fit = 0.0;
  FitSelectedCar(base, stand.calls, Holder(base), uint64_t(a), uint64_t(b), frame, &fit);
  // The text calls with f1 as FitSelectedCar leaves it, their last f1 the
  // function's.
  const uint64_t reset[3] = {movie, 2, 0};
  stand.calls.call(kResetTextScroll, reset, 3, &fit, 1, frame, 0x82201B0C, nullptr);
  const uint64_t text[8] = {movie, 2, 0, 824, 0, 0, 0, 0};
  stand.calls.call(kSetText, text, 8, nullptr, 0, frame, 0x82201B20, nullptr);
  const uint64_t scroll[4] = {movie, 2, 0, 1};
  return stand.calls.call(kScrollText, scroll, 4, nullptr, 0, frame, 0x82201B34, f1);
}

}  // namespace tuning_screen
