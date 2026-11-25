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
#include "schema/car_makers.h"
#include "schema/car_scene_draw.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/tuning_screen.h"

namespace car_makers {

namespace {

using namespace car_scene_draw::ops;

using G = schema::CarMakersGlobals;

constexpr uint32_t kRefresh = 0x820C4998;
constexpr uint32_t kAddRacer = 0x821017C0;
constexpr uint32_t kSetupVehicle = 0x820CD5C8;
constexpr uint32_t kSetupFromCar = 0x820C27E8;
constexpr uint32_t kFromGarageFrame = 4016;
constexpr uint32_t kSetupFromCarFrame = 368;

uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
  return frame;
}

}  // namespace

uint64_t GarageCarReset(const front_end_car_draw::Calls& calls, uint32_t car,
                        uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82110448 GarageCar::Reset");
  const uint32_t frame = Frame(stack, 96);
  const schema::GarageCar g(car);
  race_setup::CarClear(car);
  // The 0.0's bits, copied as words (the original's lwz and stw).
  const uint32_t zero = Load(schema::CarSceneDrawGlobals::zero_address());
  Store(g.unknown_1888_address(), zero);
  g.set_spec_748(0);
  Store(g.racer_float_address(), zero);
  g.set_unknown_1896(0);
  g.set_spec_744(0);
  g.set_unknown_1964(1);
  return front_end_car_draw::Memcpy(calls, g.owned_parts_address(),
                                    save_data::DefaultsTable3044(), 60, frame, 0x8211049C);
}

uint64_t GarageCarsStaticInit(const front_end_car_draw::Calls& calls,
                              uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_825D9EF0 GarageCars::StaticInit");
  // Its frame's address only (E5 step 6: nothing in it, no guest call made
  // with it; GarageCar::Reset's frame below it).
  const uint32_t frame = stack - 112;
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 2; i++) {
    const schema::GarageCarSlot slot(G::garage_cars_address() + 1972 * i);
    r3 = GarageCarReset(calls, slot.car_address(), frame);
    slot.set_unknown_1968(0);
  }
  return r3;
}

uint64_t FromGarageCar(const front_end_car_draw::Calls& calls, uint32_t garage,
                       uint32_t data, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82179A60 CarData::FromGarageCar");
  const uint32_t frame = Frame(stack, kFromGarageFrame);
  const uint32_t car = frame + 80;
  race_setup::CarInit(car);
  front_end_car_draw::Memcpy(calls, car, garage, 1884, frame, 0x82179A94);
  uint32_t r3 = 0;
  if (!car_specs::CarRefresh(car, &r3)) {
    const uint64_t regs[1] = {car};
    calls.call(kRefresh, regs, 1, nullptr, 0, frame, 0x82179A9C, nullptr);
  }
  const double zero = Lfs(schema::CarSceneDrawGlobals::zero_address());
  if (race_setup::AddRacerNative(car)) {
    race_setup::AddRacer(car, 0, zero, 0);
  } else {
    const uint64_t regs[4] = {car, garage, 0, 0};
    calls.call(kAddRacer, regs, 4, &zero, 1, frame, 0x82179AB4, nullptr);
  }
  const uint32_t setup_frame = frame - kSetupFromCarFrame;
  double left = 0.0;
  if (!race_setup::CarSetupVehicle(car, data, setup_frame, &left)) {
    const uint64_t regs[2] = {car, data};
    calls.call(kSetupVehicle, regs, 2, nullptr, 0, frame, 0x82179AC0, nullptr);
  }
  uint64_t result;
  if (car_physics::VehicleSetupFromCar(data, car, setup_frame, &left)) {
    result = setup_frame - 1008 + 352;
  } else {
    const uint64_t regs[2] = {data, car};
    result = calls.call(kSetupFromCar, regs, 2, nullptr, 0, frame, 0x82179ACC, &left);
  }
  *f1 = left;
  return result;
}

uint64_t TuningScreenSetUpCarData(const front_end_car_draw::Calls& calls,
                                  uint32_t screen, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_821DF2F8 TuningScreen::SetUpCarData");
  const schema::TuningScreen tuning(screen);
  const schema::SaveData save(tuning.save_data().address);
  // The garage list (career.toml's): 4-byte nodes {+0 next, +2 slot}.
  const schema::CareerGarageList garage(save.garage_address());
  const uint32_t nodes = garage.nodes_address();
  uint32_t n = save.selected_car();
  uint32_t node = uint32_t(int32_t(int16_t(garage.head()))) * 4 + nodes;
  while (n != 0) {
    node = uint32_t(int32_t(int16_t(guest::LoadU16(node)))) * 4 + nodes;
    n--;
  }
  const int64_t slot = int16_t(guest::LoadU16(node + 2));
  const uint32_t car = uint32_t(uint64_t(slot) * 1968) + garage.cars_address();
  const uint32_t holder =
      schema::StandHolder(G::stand_holder().address).stage_block_0();
  (void)holder;  // r3, not read by FromGarageCar
  return FromGarageCar(calls, car, tuning.car_data_address(), stack, f1);
}

}  // namespace car_makers
