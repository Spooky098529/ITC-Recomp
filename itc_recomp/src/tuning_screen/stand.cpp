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
#include "type67_modes/type67_modes.h"
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
void Stfs(uint8_t* base, uint32_t address, double value) {
  Store(base, address, std::bit_cast<uint32_t>(float(value)));
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
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

uint64_t ApplyAreaDecals(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7B90 TuningScreen::ApplyAreaDecals");
  const uint32_t frame = Frame(base, sp, 4160);
  const uint32_t s = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, s + 10012));
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, 0x82000A30);
  // GarageStage::SetSticker for slot `k` with the size memcpy'd from `size`
  // to frame + 96 (the caller's argument word SetSticker reads), the
  // position the doublewords at frame + 112 and + 120 (r8, r9).
  auto sticker = [&](uint32_t size, uint32_t lr, uint32_t area, uint32_t k, uint32_t word,
                     uint32_t flag, double f1) {
    front_end_car_draw::Memcpy(base, stand.calls, frame + 96, size, 16, frame, lr);
    r3 = stand_models::SetSticker(base, Holder(base), area, k, word, flag,
                                  Load64(base, frame + 112), Load64(base, frame + 120), f1, frame);
  };
  if (kind == 1) {
    // The area's five slots cleared: (0, 0, 0, 1.0) as the position and
    // the size, the angle 0.0.
    Stfs(base, frame + 112, zero);
    Stfs(base, frame + 116, zero);
    Stfs(base, frame + 120, zero);
    Stfs(base, frame + 144, zero);
    Stfs(base, frame + 124, one);
    Stfs(base, frame + 148, zero);
    Stfs(base, frame + 152, zero);
    Stfs(base, frame + 156, one);
    for (uint32_t k = 0; k < 5; k++) {
      sticker(frame + 144, 0x821F7C00, Load(base, s + 30724), k, 0xFFFFFFFFu, 0, zero);
    }
    r3 = stand_models::ReloadDecals(base, stand.call, Holder(base), frame);
    double f1 = 0.0;
    r3 = FitSelectedCar(base, stand.calls, Holder(base), 0, 0, frame, &f1);
  } else if (kind == 2) {
    const uint32_t g = GarageCarOf(base, Load(base, s + kSaveData)) + 212;
    const uint32_t car = frame + 176;
    MakeCar(base, stand.calls, car, g, 1, frame, 0x821F7CBC, 0x821F7CD0);
    const uint64_t pick = uint64_t(int64_t(int16_t(guest::LoadU16(base, g))));
    double f1 = 0.0;
    r3 = car_makers::GarageStageSetCar(base, stand.calls, stand.call, Holder(base), car, pick,
                                       frame, &f1);
  } else if (kind == 3) {
    front_end_car_draw::GarageStageCommand(base, stand.calls, Holder(base), 58, frame);
    const double offset = Lfs(base, 0x82002244);  // the mirrored angle's offset
    const double mirror = Lfs(base, 0x82001BE0);  // probably -1.0
    const double degree = Lfs(base, kDegree);
    Stfs(base, frame + 120, zero);
    Stfs(base, frame + 136, zero);
    Stfs(base, frame + 124, one);
    Stfs(base, frame + 140, one);
    for (uint32_t k = 0; k < 5; k++) {
      // The edited entry (+30832), else the area's saved one.
      const uint32_t e =
          int32_t(k) == int32_t(Load(base, s + 30732))
              ? s + 30832
              : uint32_t((uint64_t(Load(base, s + 30724)) * 5 + k) * 24 + s + 32360);
      const int64_t x = int32_t(Load(base, e + 0));
      // The scale's lfs then two stfs: its bits.
      const uint32_t scale = Load(base, e + 12);
      const int64_t y = int32_t(Load(base, e + 4));
      Store(base, frame + 128, scale);
      Store(base, frame + 132, scale);
      const uint32_t a = Load(base, s + 30724);
      double angle = Lfs(base, e + 8);
      uint32_t flag = Load(base, e + 16);
      double f1 = double(float(angle * degree));
      Store64(base, frame + 160, uint64_t(x));
      Store64(base, frame + 144, uint64_t(y));
      Stfs(base, frame + 116, ToSingle(y));
      const double fx = ToSingle(x);
      Stfs(base, frame + 112, fx);
      if (int32_t(a) == 2 || int32_t(a) == 7) {
        // The mirrored sides: x and the angle mirrored, the flag inverted.
        angle = double(float(angle - offset));
        Stfs(base, frame + 112, double(float(fx * mirror)));
        flag = flag == 0 ? 1 : 0;
        angle = double(float(angle * mirror));
        f1 = double(float(angle * degree));
      }
      const uint32_t word = Load(base, uint32_t(((uint64_t(a) + 1666) * 5 + k) << 2) + s);
      sticker(frame + 128, 0x821F7DF8, a, k, word, flag, f1);
    }
    r3 = stand_models::ReloadDecals(base, stand.call, Holder(base), frame);
  } else if (kind == 4) {
    front_end_car_draw::GarageStageCommand(base, stand.calls, Holder(base), 1, frame);
    type67_modes::GarageStageCameraCommand(base, Holder(base), 20, frame);
    front_end_car_draw::GarageStageCommand(base, stand.calls, Holder(base),
                                           Load(base, s + 30724) + 42, frame);
    Stfs(base, frame + 112, zero);
    Stfs(base, frame + 116, zero);
    Stfs(base, frame + 120, zero);
    Stfs(base, frame + 128, zero);
    Stfs(base, frame + 124, one);
    Stfs(base, frame + 132, zero);
    Stfs(base, frame + 136, zero);
    Stfs(base, frame + 140, one);
    for (uint32_t k = 0; k < 5; k++) {
      sticker(frame + 128, 0x821F7EC8, Load(base, s + 30724), k, 0xFFFFFFFFu, 0, zero);
    }
    r3 = stand_models::ReloadDecals(base, stand.call, Holder(base), frame);
  }
  Store(base, s + 10012, 0xFFFFFFFFu);
  return r3;
}

uint64_t ApplyDecalWord(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE4E0 TuningScreen::ApplyDecalWord");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t s = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, s + 10012));
  if (kind == 9 || kind == 10) {
    r3 = stand_models::SetDecalWord(base, call, Holder(base), kind == 9 ? 1 : 0, frame);
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

namespace {


// The choice tables both write: A = (3, 2, 1, 0) at `a`, B = (2, 1, 0, 4, 3)
// at `b`. Choice `k` from the word `v`: -1 GarageStage::ClearChoice, else
// GarageStage::SetChoice(holder, k, A[v / 10], B[v % 10]) (signed, the
// indexes unchecked: read as the original reads them), its plain C++ (`lr`
// the call's return address in the original). Returns its r3.
uint64_t ApplyChoice(uint8_t* base, const GuestCall& call, uint32_t frame, uint32_t a,
                     uint32_t b, uint32_t k, uint32_t v, uint32_t lr) {
  if (int32_t(v) == -1) {
    return stand_models::ClearChoice(base, call, Holder(base), k, frame);
  }
  const int32_t q = int32_t(v) / 10;
  const uint32_t d = v - uint32_t(q) * 10;
  const uint32_t first = Load(base, uint32_t(q) * 4 + a);
  const uint32_t second = Load(base, d * 4 + b);
  (void)lr;
  return stand_models::SetChoice(base, call, Holder(base), k, first, second, frame);
}

}  // namespace

uint64_t SetCaliper(uint8_t* base, const StandCalls& stand, uint32_t screen, uint32_t r4,
                    uint32_t choice, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821DA330 TuningScreen::SetCaliper");
  // The calipers' texture by the choice (unsigned), choice 0 by the
  // selected garage car's id (its jump table: 4 gives 6; 7, 12 and 14 give
  // 5; the others, and any above 22, 4).
  uint32_t w;
  switch (choice) {
    case 0: {
      const uint32_t id = Load(base, GarageCarOf(base, Load(base, screen + kSaveData)) + 224);
      w = id == 4 ? 6 : id == 7 || id == 12 || id == 14 ? 5 : 4;
      break;
    }
    case 1: w = 4; break;
    case 2: w = 6; break;
    case 3: w = 0; break;
    case 4: w = 5; break;
    default: w = 0; break;
  }
  return stand_models::SetWheelExtras(base, stand.call, stand.calls.poll, Holder(base), r4, w,
                                      sp);
}

uint64_t SetAccessory(uint8_t* base, const GuestCall& call, uint32_t kind, uint32_t choice,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CCBE8 TuningScreen::SetAccessory");
  // The kind turned around (unsigned): 0 becomes 3 with the choice 0, 1
  // becomes 2 with the choices 3 and 4 swapped, 2 becomes 1, 3 and up 0.
  uint32_t k;
  switch (kind) {
    case 0:
      k = 3;
      choice = 0;
      break;
    case 1:
      k = 2;
      if (int32_t(choice) == 3) {
        choice = 4;
      } else if (int32_t(choice) == 4) {
        choice = 3;
      }
      break;
    case 2: k = 1; break;
    default: k = 0; break;
  }
  return stand_models::SetAccessory(base, call, Holder(base), k, choice, sp);
}

uint64_t RefitAxles(uint8_t* base, const StandCalls& stand, uint32_t screen, uint32_t sp,
                    std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821DF9D0 TuningScreen::RefitAxles");
  const uint32_t frame = Frame(base, sp, 4000);
  const uint32_t garage = GarageCarOf(base, Load(base, screen + kSaveData)) + 212;
  const uint32_t car = frame + 80;
  MakeCar(base, stand.calls, car, garage, 1, frame, 0x821DFA3C, 0x821DFA50);
  return stand_models::FitAxles(base, stand.call, Holder(base), car, frame, f1);
}

uint64_t PreviewInteriorChoices(uint8_t* base, const GuestCall& call, uint32_t screen,
                                uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E7E48 TuningScreen::PreviewInteriorChoices");
  const uint32_t frame = Frame(base, sp, 208);
  // A = (3, 2, 1, 0) at +80, B = (2, 1, 0, 4, 3) at +96 (A[4] is B[0]).
  static constexpr uint32_t kTables[9] = {3, 2, 1, 0, 2, 1, 0, 4, 3};
  for (uint32_t i = 0; i < 9; i++) {
    Store(base, frame + 80 + 4 * i, kTables[i]);
  }
  const uint32_t a = frame + 80, b = frame + 96;
  const auto low = [&] { return int32_t(Load(base, screen + 35708)) < 3; };
  uint64_t r3 = 0;
  uint32_t c = 0;
  for (uint32_t i = 0; i < 5; i++) {
    // The stand's choice for the code: 6, 5, 4; then the pairs by the
    // interior level (+35708), the other of the pair cleared.
    switch (i) {
      case 0: c = 6; break;
      case 1: c = 5; break;
      case 2: c = 4; break;
      case 3: {
        const bool l = low();
        stand_models::ClearChoice(base, call, Holder(base), l ? 2 : 0, frame);
        c = l ? 0 : 2;
        break;
      }
      default: {
        const bool l = low();
        stand_models::ClearChoice(base, call, Holder(base), l ? 3 : 1, frame);
        c = l ? 1 : 3;
        break;
      }
    }
    const uint32_t w = Load(base, (i + 8450) * 4 + screen);
    if (int32_t(w) == -1) {
      r3 = stand_models::ClearChoice(base, call, Holder(base), c, frame);
      continue;
    }
    const int32_t q = int32_t(w) / 10;
    const uint32_t d = w - uint32_t(q) * 10;
    const uint32_t first = Load(base, (uint32_t(q) << 2) + a);
    const uint32_t second = Load(base, (d << 2) + b);
    r3 = stand_models::SetChoice(base, call, Holder(base), c, first, second, frame);
  }
  // The slot +33576 picks the choice the same way (5 and up: the loop's
  // last).
  switch (Load(base, screen + 33576)) {
    case 0: c = 6; break;
    case 1: c = 5; break;
    case 2: c = 4; break;
    case 3: c = low() ? 0 : 2; break;
    case 4: c = low() ? 1 : 3; break;
    default: break;
  }
  const uint32_t second = Load(base, (Load(base, screen + 33572) << 2) + b);
  const uint32_t first = Load(base, (Load(base, screen + 33564) << 2) + a);
  (void)r3;
  return stand_models::SetChoice(base, call, Holder(base), c, first, second, frame);
}

uint64_t PreviewLightKit(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp,
                         std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821CC930 TuningScreen::PreviewLightKit");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t s = uint32_t(r3);
  const int32_t state = int32_t(Load(base, s + kState));
  const auto word = [&](uint32_t offset) { return int64_t(int32_t(Load(base, s + offset))); };
  double f1v, f2, f3;
  uint32_t group, kit_word, index;
  if (state == 21) {
    const int64_t c = word(30520), b = word(30516), a = word(30512);
    index = Load(base, s + 30524);
    group = Load(base, s + 30492);
    Store64(base, frame + 80, uint64_t(c));
    Store64(base, frame + 88, uint64_t(b));
    Store64(base, frame + 96, uint64_t(a));
    const double scale = Lfs(base, 0x820021FC);
    f3 = double(float(ToSingle(c) * scale));
    f2 = double(float(ToSingle(b) * scale));
    f1v = double(float(ToSingle(a) * scale));
    kit_word = 1;
  } else if (state == 22) {
    const int64_t c = word(30552);
    group = Load(base, s + 30492);
    kit_word = Load(base, s + 30548) + 2;
    index = Load(base, s + 30556);
    Store64(base, frame + 96, uint64_t(c));
    const int64_t b = int64_t(int32_t(Load(base, (group + 7653) * 4 + s)));
    const int64_t a = int64_t(int32_t(Load(base, (group + 7649) * 4 + s)));
    Store64(base, frame + 88, uint64_t(a));
    Store64(base, frame + 96, uint64_t(b));
    const double scale = Lfs(base, 0x820021FC);
    f3 = double(float(ToSingle(c) * scale));
    f2 = double(float(ToSingle(b) * scale));
    f1v = double(float(ToSingle(a) * scale));
  } else {
    return r3;
  }
  const uint64_t left = front_end_car_draw::GarageStageSetLightKit(
      base, stand.calls, Holder(base), group, kit_word, index, f1v, f2, f3, frame);
  *f1 = f1v;  // SetLightKit leaves f1 as given
  return left;
}

uint64_t PreviewParts(uint8_t* base, const GuestCall& call, uint64_t r3, uint64_t pick,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CCAA8 TuningScreen::PreviewParts");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t s = uint32_t(r3);
  auto at = [&](uint32_t offset) { return Load(base, s + offset); };
  // The wing word: +36300, or with +25172 set 0 stays 0, 1 becomes 13 and
  // any other value n becomes n - 1.
  uint32_t w = at(36300);
  if (at(25172) != 0) {
    w = w == 0 ? 0 : w == 1 ? 13 : w - 1;
  }
  uint32_t kind;
  if (int32_t(pick) == 0) {
    r3 = stand_models::SetParts(base, call, Holder(base), at(36164), at(36252), at(36208),
                                at(36544), at(36368), w, frame);
    kind = 9;
  } else {
    r3 = stand_models::SetAeroSet(base, call, Holder(base), pick - 1, at(36368), w, frame);
    kind = 10;
  }
  if (int32_t(at(36372)) == 1 && at(36376) == 0) {
    Store(base, s + 10012, kind);
  }
  return r3;
}

uint64_t PreviewItem(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp,
                     std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E07B0 TuningScreen::PreviewItem");
  const uint32_t frame = Frame(base, sp, 160);
  const uint64_t screen = r3;
  const uint32_t s = uint32_t(r3);
  const GuestCall& call = stand.call;
  auto at = [&](uint32_t offset) { return Load(base, s + offset); };
  auto u8 = [&](uint32_t offset) { return uint32_t(guest::LoadU8(base, s + offset)); };
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, s + offset, value); };
  auto available = [&](uint32_t item) {
    r3 = IsItemAvailable(base, s, item);
    return int32_t(r3) != 0;
  };
  auto set_part = [&](uint32_t slot, uint32_t word) {
    r3 = stand_models::SetPart(base, call, Holder(base), slot, word, frame);
  };
  auto refit = [&] { r3 = RefitAxles(base, stand, s, frame, f1); };
  auto accessory = [&](uint32_t choice) {
    r3 = SetAccessory(base, call, at(33548), choice, frame);
  };
  const int32_t state = int32_t(at(kState));
  if (state == 3 || state == 4) {
    return r3;
  }
  if (int32_t(at(kCategory)) == 0 && state == 5) {
    const uint32_t p = u8(kItem);
    if (p == 0) {
      const uint32_t i = u8(kLevel);
      if (available(i)) {
        set(25076, 14);
        r3 = stand_models::SetInteriorClass(base, Holder(base), i, frame);
        set_part(at(25076), i);
      }
    } else if (p == 1) {
      const uint32_t i = u8(kLevel);
      if (available(i)) {
        set(25076, 1);
        set_part(1, i);
      }
    } else if (p == 2) {
      set(25076, 17);
    }
  }
  const int32_t m = int32_t(at(kCategory));
  if (m == 1) {
    const uint32_t p = u8(kItem);
    const uint32_t i = u8(kLevel);
    if (p != 3) {
      return r3;
    }
    const int32_t now = int32_t(at(kState));
    if (now == 5) {
      if (available(i)) {
        r3 = SetCaliper(base, stand, s, i, 0, frame);
      }
    } else if (now == 7) {
      if (available(i)) {
        r3 = SetCaliper(base, stand, s, i, u8(kType2), frame);
      }
    }
  } else if (m == 0) {
    if (int32_t(at(kState)) == 6) {
      set(25076, 15);
      set_part(15, u8(kType));
    }
  } else if (m == 2) {
    if (int32_t(at(kState)) != 5) {
      return r3;
    }
    const uint32_t i = u8(kLevel);
    if (!available(i)) {
      return r3;
    }
    const uint32_t a = frame + 80, b = frame + 96;
    Store(base, frame + 88, 1);
    Store(base, frame + 100, 1);
    Store(base, frame + 80, 3);
    Store(base, frame + 84, 2);
    Store(base, frame + 96, 2);
    Store(base, frame + 112, 3);
    Store(base, frame + 92, 0);
    Store(base, frame + 104, 0);
    Store(base, frame + 108, 4);
    r3 = stand_models::SetInteriorLevel(base, call, Holder(base), i, frame);
    uint32_t k;
    if (int32_t(i) < 3) {
      r3 = stand_models::ClearChoice(base, call, Holder(base), 2, frame);
      r3 = stand_models::ClearChoice(base, call, Holder(base), 3, frame);
      r3 = ApplyChoice(base, call, frame, a, b, 0, at(33812), 0x821E0A08);
      k = 1;
    } else {
      r3 = stand_models::ClearChoice(base, call, Holder(base), 0, frame);
      r3 = stand_models::ClearChoice(base, call, Holder(base), 1, frame);
      r3 = ApplyChoice(base, call, frame, a, b, 2, at(33812), 0x821E0A88);
      k = 3;
    }
    r3 = ApplyChoice(base, call, frame, a, b, k, at(33816), 0x821E0AE4);
  } else if (m == 3) {
    if (int32_t(at(kState)) != 5) {
      return r3;
    }
    const uint32_t i = u8(kLevel);
    if (available(i)) {
      r3 = stand_models::TuningScreenSetTireGrade(base, call, screen, u8(kItem), i, frame);
    }
  } else if (m == 4) {
    const int32_t now = int32_t(at(kState));
    if (now == 6) {
      const uint32_t p = u8(kItem);
      uint64_t j = u8(kType);
      if (p == 9) {
        r3 = PreviewParts(base, call, screen, j, frame);
        refit();
        return r3;
      }
      switch (p) {
        case 0: set(25076, 0); break;
        case 1: set(25076, 3); break;
        case 2: set(25076, 2); break;
        case 3: {
          const uint32_t swapped = at(25172);
          set(25076, 4);
          if (int32_t(swapped) != 0 && j != 0) {
            j = j == 1 ? 13 : j - 1;
          }
          break;
        }
        case 4: set(25076, 5); break;
        case 5:
          set(25076, 8);
          set_part(8, uint32_t(j));
          set(25076, 9);
          break;
        case 6: set(25076, 10); break;
        case 7: set(25076, 11); break;
        case 8:
          set(25076, 12);
          refit();
          break;
        default: break;
      }
      set_part(at(25076), uint32_t(j));
    } else if (now == 7 || now == 8) {
      if (int32_t(at(9712)) == 0 && u8(kItem) == 4) {
        const uint32_t pick = u8(now == 7 ? kType2 : kOption);
        if (pick == 0) {
          set(10012, now == 7 ? 10 : 9);
        } else if (pick == 1) {
          set(10012, now == 7 ? 9 : 10);
        }
      }
      set(9712, 0);
    }
  } else if (m == 5) {
    const int32_t p = int32_t(u8(kItem));
    if (p == 1) {
      r3 = stand_models::SetDoorKind(base, call, Holder(base), u8(kOption), frame);
    } else if (p == 2) {
      r3 = stand_models::SetAero2(base, call, Holder(base), u8(kType), frame);
    }
  } else if (m == 6) {
    const uint32_t p = u8(kItem);
    if (p == 3) {
      const int32_t now = int32_t(at(kState));
      if (now == 54) {
        accessory(0);
      } else if (now == 55) {
        accessory(at(33556));
      }
      return r3;
    }
    const int32_t now = int32_t(at(kState));
    if (now == 6) {
      if (p == 0) {
        r3 = SetInteriorPair(base, call, u8(kType), 0, frame);
      } else if (p == 1) {
        set(25080, 5);
        r3 = stand_models::SetInteriorWord(base, call, Holder(base), 5, u8(kType), frame);
      } else if (p == 2) {
        r3 = stand_models::SetAero(base, call, Holder(base), u8(kType), frame);
      }
    } else if (now == 7 && p == 0) {
      const uint32_t level = u8(kType2);
      r3 = SetInteriorPair(base, call, u8(kType), level, frame);
    }
  }
  return r3;
}

uint64_t RestoreStand(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp,
                      std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E0FB0 TuningScreen::RestoreStand");
  const uint32_t frame = Frame(base, sp, 224);
  const uint64_t screen = r3;
  const uint32_t s = uint32_t(r3);
  const GuestCall& call = stand.call;
  auto at = [&](uint32_t offset) { return Load(base, s + offset); };
  auto set_part = [&](uint32_t slot, uint32_t word) {
    r3 = stand_models::SetPart(base, call, Holder(base), slot, word, frame);
  };
  auto refit = [&](uint32_t) { r3 = RefitAxles(base, stand, s, frame, f1); };
  // GarageStage::ClearLightKit's plain C++ (f1 left as it is: neither it nor
  // SceneModel::ApplyLightKit changes it).
  auto clear_kit = [&](uint32_t group) {
    r3 = front_end_car_draw::GarageStageClearLightKit(base, stand.calls, Holder(base), group,
                                                      frame);
  };
  // GarageStage::SetColor's plain C++ (ApplyPaint's and ApplyWheelPaint's
  // with the load poll's calls); f1 left as given (neither paint changes
  // it).
  auto set_color = [&](uint32_t color, const double floats[3]) {
    r3 = stand_models::SetColor(base, call, stand.calls.poll, Holder(base), color, floats[0],
                                floats[1], floats[2], frame);
    *f1 = floats[0];
  };
  const int32_t m = int32_t(at(kCategory));
  const uint32_t p = guest::LoadU8(base, s + kItem);
  if (m == 0) {
    if (p == 0) {
      const uint32_t k = at(kPartSet);
      r3 = stand_models::SetInteriorClass(base, Holder(base), Load(base, (k + 8576) * 4 + s),
                                          frame);
      set_part(14, Load(base, (at(kPartSet) + 8576) * 4 + s));
    } else if (p == 1) {
      set_part(1, Load(base, (at(kPartSet) + 8701) * 4 + s));
    } else if (p == 2) {
      set_part(15, at(35024));
    }
  } else if (m == 1) {
    if (p == 3) {
      const uint32_t choice = at(35496);
      r3 = SetCaliper(base, stand, s, at(35492), choice, frame);
    }
  } else if (m == 2) {
    r3 = stand_models::SetInteriorLevel(base, call, Holder(base), at(35708), frame);
    const uint32_t level = at(35708);
    const uint32_t a = frame + 112, b = frame + 128;
    Store(base, frame + 112, 3);
    Store(base, frame + 144, 3);
    Store(base, frame + 116, 2);
    Store(base, frame + 120, 1);
    Store(base, frame + 124, 0);
    Store(base, frame + 128, 2);
    Store(base, frame + 132, 1);
    Store(base, frame + 136, 0);
    Store(base, frame + 140, 4);
    uint32_t k;
    if (int32_t(level) < 3) {
      r3 = stand_models::ClearChoice(base, call, Holder(base), 2, frame);
      r3 = stand_models::ClearChoice(base, call, Holder(base), 3, frame);
      r3 = ApplyChoice(base, call, frame, a, b, 0, at(33812), 0x821E119C);
      k = 1;
    } else {
      r3 = stand_models::ClearChoice(base, call, Holder(base), 0, frame);
      r3 = stand_models::ClearChoice(base, call, Holder(base), 1, frame);
      r3 = ApplyChoice(base, call, frame, a, b, 2, at(33812), 0x821E1244);
      k = 3;
    }
    const uint32_t v = at(33816);
    r3 = ApplyChoice(base, call, frame, a, b, k, v, 0x821E1ACC);
  } else if (m == 3) {
    if (p < 3) {
      // T = (0, 1, 1, 2, 2) at frame + 128, indexed unchecked.
      const uint32_t t = frame + 128;
      auto table = [&] {
        Store(base, t + 4, 1);
        Store(base, t + 8, 1);
        Store(base, t + 0, 0);
        Store(base, t + 12, 2);
        Store(base, t + 16, 2);
      };
      auto grade = [&](uint32_t which, uint32_t offset) {
        r3 = stand_models::SetTireGrade(base, call, Holder(base), which,
                                        Load(base, at(offset) * 4 + t), frame);
      };
      table();
      if (p == 0) {
        grade(1, 35948);
        table();
        grade(2, 36076);
      } else if (p == 1) {
        grade(1, 35948);
      } else {
        grade(2, 36076);
      }
    }
  } else if (m == 4) {
    switch (p) {
      case 0: set_part(0, at(36164)); break;
      case 1: set_part(3, at(36208)); break;
      case 2: set_part(2, at(36252)); break;
      case 3: {
        uint64_t w;
        if (int32_t(at(25172)) == 0) {
          w = at(36300);
        } else {
          const int32_t n = int32_t(at(36300));
          w = n == 0 ? 0 : n == 1 ? 13 : uint64_t(int64_t(n) - 1);
        }
        set_part(4, uint32_t(w));
        break;
      }
      case 4: {
        set_part(5, at(36368));
        const int32_t x = int32_t(at(36372));
        if (x == 0) {
          Store(base, s + 10012, 10);
        } else if (x == 1) {
          const int32_t y = int32_t(at(36376));
          if (y == 0) {
            Store(base, s + 10012, 9);
          } else if (y == 1) {
            Store(base, s + 10012, 10);
          }
        }
        break;
      }
      case 5: set_part(8, at(36436)); break;
      case 6: set_part(10, at(36472)); break;
      case 7: set_part(11, at(36508)); break;
      case 8:
        set_part(12, at(36544));
        refit(0x821E152C);
        break;
      case 9:
        r3 = PreviewParts(base, call, screen, at(36588), frame);
        refit(0x821E152C);
        break;
      default: break;
    }
  } else if (m == 5) {
    if (p == 0) {
      const uint32_t x = at(25196);
      auto wheel = [&](uint32_t offset, uint32_t which) {
        r3 = stand_models::PreviewWheel(base, call, s, at(offset), which, frame, f1);
      };
      if (x == 0) {
        if (int32_t(at(25240)) != 0) {
          wheel(27836, 1);
          wheel(27840, 2);
          Store(base, s + 25240, 0);
        } else {
          wheel(27832, 0);
        }
      } else if (x == 1) {
        wheel(27836, 1);
      } else if (x == 2) {
        wheel(27840, 2);
      }
      Store(base, s + 25244, 1);
    } else if (p == 1) {
      r3 = stand_models::SetDoorKind(base, call, Holder(base), at(36612), frame);
    } else if (p == 2) {
      r3 = stand_models::SetAero2(base, call, Holder(base), at(36636), frame);
    } else if (p == 4) {
      // The paint: colors 0 and 1 from the halfword-sized fields as ints,
      // their conversions' doublewords at +88, +96 and +112.
      const double scale = Lfs(base, 0x82002200);
      for (uint32_t color = 0; color < 2; color++) {
        const int64_t g = int32_t(at(30480)), red = int32_t(at(30476)),
                      blue = int32_t(at(30484));
        if (color == 0) {
          Store64(base, frame + 96, uint64_t(g));
          Store64(base, frame + 88, uint64_t(red));
          Store64(base, frame + 112, uint64_t(blue));
        } else {
          Store64(base, frame + 96, uint64_t(g));
          Store64(base, frame + 112, uint64_t(blue));
          Store64(base, frame + 88, uint64_t(red));
        }
        const double floats[3] = {ToSingle(red), double(float(ToSingle(g) * scale)),
                                  double(float(ToSingle(blue) * scale))};
        set_color(color, floats);
      }
      const int32_t x = int32_t(at(30488));
      if (x == 0) {
        const double zero = Lfs(base, kZero);
        const double floats[3] = {zero, zero, Lfs(base, 0x82001FBC)};
        set_color(2, floats);
      } else if (x == 1) {
        const int64_t red = int32_t(at(30476)), g = int32_t(at(30480));
        Store64(base, frame + 96, uint64_t(red));
        Store64(base, frame + 112, uint64_t(g));
        const double floats[3] = {ToSingle(red),
                                  double(float(ToSingle(g) * Lfs(base, 0x8200AD48))),
                                  Lfs(base, 0x82001070)};
        set_color(2, floats);
      }
    } else if (p == 5) {
      // The light kits: each group from the first kit (+30580 on) or the
      // second (+30660 on), the order by +30500; else cleared.
      const int32_t x = int32_t(at(30500));
      const double scale = Lfs(base, 0x820021FC);
      const double half = Lfs(base, 0x82001BE4);
      auto first = [&](uint32_t g, bool second_first) {
        const uint32_t e = s + 30580 + 4 * g;
        const int64_t a = int32_t(Load(base, e + 32)), b = int32_t(Load(base, e + 16)),
                      c = int32_t(Load(base, e + 48));
        const uint32_t index = Load(base, e + 64);
        Store64(base, frame + 88, uint64_t(a));
        if (second_first) {
          Store64(base, frame + 80, uint64_t(b));
          Store64(base, frame + 96, uint64_t(c));
        } else {
          Store64(base, frame + 96, uint64_t(b));
          Store64(base, frame + 80, uint64_t(c));
        }
        const double f2 = double(float(ToSingle(a) * scale));
        const double one = double(float(ToSingle(b) * scale));
        const double f3 = double(float(ToSingle(c) * scale));
        r3 = front_end_car_draw::GarageStageSetLightKit(base, stand.calls, Holder(base), g, 1,
                                                        index, one, f2, f3, frame);
        *f1 = one;
      };
      auto second = [&](uint32_t g) {
        const uint32_t e = s + 30660 + 4 * g;
        const int64_t d = int32_t(Load(base, e + 32));
        const uint32_t word = Load(base, e + 16) + 2;
        const uint32_t index = Load(base, e + 48);
        Store64(base, frame + 112, uint64_t(d));
        const double f3 = double(float(ToSingle(d) * scale));
        r3 = front_end_car_draw::GarageStageSetLightKit(base, stand.calls, Holder(base), g, word,
                                                        index, half, half, f3, frame);
        *f1 = half;
      };
      if (x == 0) {
        for (uint32_t g = 0; g < 3; g++) {
          if (int32_t(at(30580 + 4 * g)) != 0) {
            first(g, false);
          } else if (int32_t(at(30660 + 4 * g)) != 0) {
            second(g);
          } else {
            clear_kit(g);
          }
        }
      } else if (x == 1) {
        for (uint32_t g = 0; g < 3; g++) {
          if (int32_t(at(30660 + 4 * g)) != 0) {
            second(g);
          } else if (int32_t(at(30580 + 4 * g)) != 0) {
            first(g, true);
          } else {
            clear_kit(g);
          }
        }
      } else {
        clear_kit(at(30492));
      }
    }
  } else if (m == 6) {
    if (p == 0) {
      const uint32_t level = at(36712);
      r3 = SetInteriorPair(base, call, at(36708), level, frame);
    } else if (p == 1) {
      r3 = stand_models::SetInteriorWord(base, call, Holder(base), 5, at(36752), frame);
    } else if (p == 2) {
      r3 = stand_models::SetAero(base, call, Holder(base), at(36796), frame);
    } else if (p == 3) {
      const int32_t kind = int32_t(at(33540));
      if (kind == 0) {
        const uint32_t choice = at(33620);
        r3 = SetAccessory(base, call, at(33616), choice, frame);
      } else if (kind == 1) {
        const uint32_t c = at(33576);
        Store(base, frame + 124, 0);
        Store(base, frame + 112, 3);
        Store(base, frame + 116, 2);
        Store(base, frame + 120, 1);
        Store(base, frame + 128, 2);
        Store(base, frame + 132, 1);
        Store(base, frame + 136, 0);
        Store(base, frame + 140, 4);
        Store(base, frame + 144, 3);
        uint32_t k = 0;
        if (c == 0) {
          k = 6;
        } else if (c == 1) {
          k = 5;
        } else if (c == 2) {
          k = 4;
        } else if (c == 3) {
          k = int32_t(at(35708)) < 3 ? 0 : 2;
        } else if (c == 4) {
          k = int32_t(at(35708)) < 3 ? 1 : 3;
        }
        const uint32_t v = Load(base, (c + 8450) * 4 + s);
        r3 = ApplyChoice(base, call, frame, frame + 112, frame + 128, k, v, 0x821E1ACC);
      }
    }
  }
  return r3;
}

uint64_t UpdateStand(uint8_t* base, const StandCalls& stand,
                     const front_end_car_draw::DrawCallees& draw, load_poll::Calls* paint_poll,
                     uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D9F10 TuningScreen::UpdateStand");
  constexpr uint32_t kBonnetOpen = 0x826D84A0;   // the three stand animations' open states
  constexpr uint32_t kRoofOpen = 0x826D849C;
  constexpr uint32_t kDoorsOpen = 0x826D8498;
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t s = screen;
  auto at = [&](uint32_t offset) { return Load(base, s + offset); };
  const uint32_t c = GarageCarOf(base, at(kSaveData));
  // An animation's open state `flag`: opened with `open` while `wanted`,
  // closed with `open` + 1 otherwise (GarageStage::Command).
  auto toggle = [&](bool wanted, uint32_t flag, uint32_t open) {
    const uint32_t now = Load(base, flag);
    if (wanted ? int32_t(now) != 0 : int32_t(now) == 0) {
      return;
    }
    Store(base, flag, wanted ? 1 : 0);
    front_end_car_draw::GarageStageCommand(base, stand.calls, Holder(base), wanted ? open : open + 1,
                                           frame);
  };
  auto in = [&](std::initializer_list<int32_t> states) {
    const int32_t state = int32_t(at(kState));
    for (const int32_t x : states) {
      if (state == x) {
        return true;
      }
    }
    return false;
  };
  const auto item = [&]() { return guest::LoadU8(base, s + kItem); };
  // (1) The bonnet: power, item 0, states 5, 9 to 11.
  toggle(int32_t(at(kCategory)) == 0 && item() == 0 && in({5, 9, 10, 11}), kBonnetOpen, 2);
  // (2) The same for item 1 on cars 14 and 15 (the flag left alone on others).
  {
    const bool page = int32_t(at(kCategory)) == 0 && item() == 1 && in({5, 9, 10, 11});
    const int32_t id = int32_t(Load(base, c + 224));
    if (id == 14 || id == 15) {
      toggle(page, kRoofOpen, 2);
    }
  }
  // (3) The doors: exterior, item 1, states 8 to 11.
  toggle(int32_t(at(kCategory)) == 5 && item() == 1 && in({8, 9, 10, 11}), kDoorsOpen, 4);
  // (4) The paint: exterior, item 4, state 18.
  if (int32_t(at(kCategory)) == 5 && item() == 4 && int32_t(at(kState)) == 18) {
    auto set_color = [&](uint32_t color, const double floats[3], uint32_t) {
      stand_models::SetColor(base, stand.call, paint_poll, Holder(base), color, floats[0],
                             floats[1], floats[2], frame);
    };
    const double scale = Lfs(base, 0x82002200);  // 1/255
    {
      const int64_t g = int32_t(at(30448)), hue = int32_t(at(30444));
      Store64(base, frame + 88, uint64_t(g));
      Store64(base, frame + 96, uint64_t(hue));
      const int64_t b = int32_t(at(30452));
      Store64(base, frame + 80, uint64_t(b));
      const double floats[3] = {ToSingle(hue), double(float(ToSingle(g) * scale)),
                                double(float(ToSingle(b) * scale))};
      set_color(0, floats, 0x821DA1A8);
    }
    {
      const int64_t b = int32_t(at(30452)), g = int32_t(at(30448)), hue = int32_t(at(30444));
      Store64(base, frame + 96, uint64_t(b));
      Store64(base, frame + 88, uint64_t(g));
      Store64(base, frame + 80, uint64_t(hue));
      const double floats[3] = {ToSingle(hue), double(float(ToSingle(g) * scale)),
                                double(float(ToSingle(b) * scale))};
      set_color(1, floats, 0x821DA200);
    }
    const int32_t x = int32_t(at(30456));
    if (x == 0) {
      const double zero = Lfs(base, kZero);
      const double floats[3] = {zero, zero, Lfs(base, 0x82001FBC)};
      set_color(2, floats, 0x821DA280);
    } else if (x == 1) {
      const int64_t g = int32_t(at(30448)), hue = int32_t(at(30444));
      Store64(base, frame + 96, uint64_t(g));
      Store64(base, frame + 88, uint64_t(hue));
      const double floats[3] = {ToSingle(hue),
                                double(float(ToSingle(g) * Lfs(base, 0x8200AD48))),
                                Lfs(base, 0x82001070)};
      set_color(2, floats, 0x821DA280);
    }
  }
  // (5) The decals waiting (+10012), once the stand isn't busy.
  if (int32_t(at(10012)) != -1 &&
      int32_t(front_end_car_draw::GarageStageIsBusy(base, stand.calls, Holder(base), frame)) == 0) {
    const int32_t v = int32_t(at(10012));
    if (v >= 1 && v < 5) {
      ApplyAreaDecals(base, stand, s, frame);
    } else if (v >= 5 && v < 9) {
      ApplyDecals(base, stand, s, frame);
    } else if (v >= 9 && v < 11) {
      ApplyDecalWord(base, stand.call, s, frame);
    }
  }
  // (6) The second wheels, the wheel word, the stand drawn.
  stand_models::StepSecondWheels(base, stand.call, paint_poll, s, frame);
  stand_models::SetWheelWord(base, Holder(base), 1, frame);
  return front_end_car_draw::GarageStageDraw(base, stand.calls, Holder(base), 0, draw, frame);
}

}  // namespace tuning_screen
