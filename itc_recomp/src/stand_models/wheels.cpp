// The stand's wheel fitting (docs/functions/stand-models.md): the axles
// refitted from a car and the second set's wheel sizes, step for step as the
// translated code (see stand_models.h).

#include "stand_models/stand_models.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

namespace stand_models {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kSetupVehicle = 0x820CD5C8;
constexpr uint32_t kWheelsSetRecord = 0x820D8EC0;

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

}  // namespace

uint64_t FitAxles(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t car,
                  uint32_t stack, std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_82178B98 GarageStage::FitAxles");
  const uint32_t frame = Frame(base, stack, 1968);
  const uint32_t data = frame + 80;
  car_physics::CarDataConstruct(base, data, frame - 96);
  // Car::SetupVehicle is a tail call to Vehicle::SetupFromCar(data, car):
  // its frame right below this one.
  double left = 0.0;
  uint64_t r3;
  if (car_physics::VehicleSetupFromCar(base, data, car, frame - 368, &left)) {
    r3 = frame - 368 - 1008 + 352;
    *f1 = left;
  } else {
    r3 = call(kSetupVehicle, {car, data}, frame, 0x82178BC8);
    *f1 = std::nullopt;  // as the guest call left it
  }
  const auto stage = [&] { return Load(base, holder); };
  Stfs(base, stage() + 6176, Lfs(base, frame + 416));
  const double one = Lfs(base, kOne);
  Stfs(base, stage() + 6180, Lfs(base, frame + 420));
  Stfs(base, stage() + 6184, Lfs(base, frame + 424));
  Stfs(base, stage() + 6188, one);
  Stfs(base, stage() + 6192, Lfs(base, frame + 688));
  Stfs(base, stage() + 6196, Lfs(base, frame + 692));
  Stfs(base, stage() + 6200, Lfs(base, frame + 696));
  Stfs(base, stage() + 6204, one);
  return r3;
}

uint64_t SceneWheelsSetSizes(uint8_t* base, const GuestCall& call, uint32_t wheels, uint32_t a,
                             uint32_t b, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9,
                             uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7380 SceneWheels::SetSizes");
  const uint32_t frame = Frame(base, stack, 112);
  // The four registers into the caller's parameter area (its r1 + 48 to +
  // 79), read back as words.
  Store64(base, stack + 48, r6);
  Store64(base, stack + 64, r8);
  Store64(base, stack + 56, r7);
  Store64(base, stack + 72, r9);
  const uint32_t w0 = Load(base, stack + 48);
  Store(base, wheels + 108, b);
  const uint32_t w1 = Load(base, stack + 52), w2 = Load(base, stack + 56);
  const uint32_t v2 = Load(base, stack + 72), v0 = Load(base, stack + 64);
  const uint32_t v1 = Load(base, stack + 68);
  Store(base, wheels + 104, a);
  const uint32_t first = Load(base, wheels + 28);
  Store(base, wheels + 76, w0);
  Store(base, wheels + 80, w1);
  Store(base, wheels + 84, w2);
  Store(base, wheels + 88, v0);
  Store(base, wheels + 92, v1);
  Store(base, wheels + 96, v2);
  return call(kWheelsSetRecord, {first, wheels + 32}, frame, 0x820E73F4);
}

}  // namespace stand_models
