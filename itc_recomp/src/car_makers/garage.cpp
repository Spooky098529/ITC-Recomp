// The garage cars' makers (docs/functions/car-makers.md), step for step as
// the translated code (see car_makers.h).

#include "car_makers/car_makers.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_scene_draw/ops.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "save_data/save_data.h"

namespace car_makers {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kGarageCars = 0x82814EB8;  // two 1,972-byte entries
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kRefresh = 0x820C4998;
constexpr uint32_t kAddRacer = 0x821017C0;
constexpr uint32_t kSetupVehicle = 0x820CD5C8;
constexpr uint32_t kSetupFromCar = 0x820C27E8;
constexpr uint32_t kFromGarageFrame = 4016;
constexpr uint32_t kSetupFromCarFrame = 368;

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

}  // namespace

uint64_t GarageCarReset(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t car,
                        uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82110448 GarageCar::Reset");
  const uint32_t frame = Frame(base, stack, 96);
  race_setup::CarClear(base, car);
  const uint32_t zero = Load(base, kZero);
  Store(base, car + 1888, zero);
  Store(base, car + 1884, 0);
  Store(base, car + 1892, zero);
  Store(base, car + 1896, 0);
  Store(base, car + 1900, 0);
  Store(base, car + 1964, 1);
  return front_end_car_draw::Memcpy(base, calls, car + 1904, save_data::DefaultsTable3044(), 60,
                                    frame, 0x8211049C);
}

uint64_t GarageCarsStaticInit(uint8_t* base, const front_end_car_draw::Calls& calls,
                              uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_825D9EF0 GarageCars::StaticInit");
  const uint32_t frame = Frame(base, stack, 112);
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t car = kGarageCars + 1972 * i;
    r3 = GarageCarReset(base, calls, car, frame);
    Store(base, car + 1968, 0);
  }
  return r3;
}

uint64_t FromGarageCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t garage,
                       uint32_t data, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82179A60 CarData::FromGarageCar");
  const uint32_t frame = Frame(base, stack, kFromGarageFrame);
  const uint32_t car = frame + 80;
  race_setup::CarInit(base, car);
  front_end_car_draw::Memcpy(base, calls, car, garage, 1884, frame, 0x82179A94);
  uint32_t r3 = 0;
  if (!car_specs::CarRefresh(base, car, &r3)) {
    const uint64_t regs[1] = {car};
    calls.call(kRefresh, regs, 1, nullptr, 0, frame, 0x82179A9C, nullptr);
  }
  const double zero = Lfs(base, kZero);
  if (race_setup::AddRacerNative(base, car)) {
    race_setup::AddRacer(base, car, 0, zero, 0);
  } else {
    const uint64_t regs[4] = {car, garage, 0, 0};
    calls.call(kAddRacer, regs, 4, &zero, 1, frame, 0x82179AB4, nullptr);
  }
  const uint32_t setup_frame = frame - kSetupFromCarFrame;
  double left = 0.0;
  if (!race_setup::CarSetupVehicle(base, car, data, setup_frame, &left)) {
    const uint64_t regs[2] = {car, data};
    calls.call(kSetupVehicle, regs, 2, nullptr, 0, frame, 0x82179AC0, nullptr);
  }
  uint64_t result;
  if (car_physics::VehicleSetupFromCar(base, data, car, setup_frame, &left)) {
    result = setup_frame - 1008 + 352;
  } else {
    const uint64_t regs[2] = {data, car};
    result = calls.call(kSetupFromCar, regs, 2, nullptr, 0, frame, 0x82179ACC, &left);
  }
  *f1 = left;
  return result;
}

uint64_t TuningScreenSetUpCarData(uint8_t* base, const front_end_car_draw::Calls& calls,
                                  uint32_t screen, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_821DF2F8 TuningScreen::SetUpCarData");
  const uint32_t save = Load(base, screen + 18904);
  const uint32_t nodes = save + 6120;
  uint32_t n = Load(base, save + 0x19920);
  uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, nodes + 204)))) * 4 + nodes;
  while (n != 0) {
    node = uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) * 4 + nodes;
    n--;
  }
  const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
  const uint32_t car = uint32_t(uint64_t(slot) * 1968) + nodes + 212;
  const uint32_t holder = Load(base, Load(base, kStandHolder));
  (void)holder;  // r3, not read by FromGarageCar
  return FromGarageCar(base, calls, car, screen + 18912, stack, f1);
}

}  // namespace car_makers
