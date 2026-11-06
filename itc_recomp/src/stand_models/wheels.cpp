// The stand's wheel fitting (docs/functions/stand-models.md): the axles
// refitted from a car and the second set's wheel sizes, step for step as the
// translated code (see stand_models.h).

#include "stand_models/stand_models.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_scene_draw/ops.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "native/buffers.h"
#include "race_setup/race_setup.h"
#include "strings_and_font/steps.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace stand_models {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kSetupVehicle = 0x820CD5C8;
constexpr uint32_t kWheelsSetRecord = 0x820D8EC0;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kRefresh = 0x820C4998;
constexpr uint32_t kByteScale = 0x82002200;  // 1/255
constexpr uint32_t kTireScale = 0x82001BDC;
constexpr uint32_t kHundred = 0x82002A0C;    // 100.0

// GarageStage::SetWheel's table, built on its frame at + 96 each call: entry
// e (16 bytes) is {e, then these three words}: a brand (probably), the wheel
// model (the word +8, the only one read; -1 for entry 0, the stock wheel)
// and a variant (probably the color). The original's 716 stores, gathered.
constexpr int32_t kWheelTable[179][3] = {
    {0, -1, 0}, {1, 0, 0}, {1, 0, 1}, {1, 0, 2}, {1, 0, 3}, {1, 1, 0}, {1, 1, 1}, {1, 1, 2},
    {1, 1, 3}, {1, 2, 0}, {1, 2, 1}, {1, 2, 2}, {1, 2, 3}, {1, 3, 0}, {1, 3, 1}, {1, 3, 2},
    {1, 4, 0}, {1, 4, 1}, {1, 4, 2}, {1, 4, 3}, {1, 5, 0}, {1, 5, 1}, {1, 5, 2}, {1, 5, 3},
    {1, 5, 4}, {1, 5, 5}, {1, 6, 0}, {1, 6, 1}, {1, 6, 2}, {1, 6, 3}, {1, 7, 0}, {1, 7, 1},
    {1, 7, 2}, {1, 7, 3}, {1, 8, 0}, {1, 8, 1}, {1, 8, 2}, {1, 8, 3}, {1, 8, 4}, {2, 9, 0},
    {2, 9, 1}, {2, 10, 0}, {2, 11, 0}, {2, 12, 0}, {2, 12, 1}, {2, 13, 0}, {2, 14, 0}, {3, 15, 0},
    {3, 16, 0}, {3, 17, 0}, {3, 17, 1}, {3, 17, 2}, {3, 18, 0}, {3, 18, 1}, {3, 19, 0}, {3, 19, 1},
    {3, 19, 2}, {3, 19, 3}, {3, 20, 0}, {3, 20, 1}, {3, 20, 2}, {3, 21, 0}, {3, 21, 1}, {3, 22, 0},
    {3, 22, 1}, {3, 23, 0}, {3, 23, 1}, {3, 23, 2}, {3, 24, 0}, {3, 24, 1}, {3, 24, 2}, {3, 24, 3},
    {4, 25, 0}, {4, 25, 1}, {4, 26, 0}, {4, 26, 1}, {4, 26, 2}, {4, 26, 3}, {4, 27, 0}, {4, 27, 1},
    {4, 27, 2}, {4, 27, 3}, {4, 27, 4}, {4, 28, 0}, {4, 28, 1}, {4, 28, 2}, {4, 29, 0}, {4, 29, 1},
    {4, 29, 2}, {4, 29, 3}, {4, 30, 0}, {4, 30, 1}, {4, 30, 2}, {4, 30, 3}, {4, 31, 0}, {4, 32, 0},
    {4, 32, 1}, {4, 32, 2}, {4, 32, 3}, {5, 33, 0}, {5, 33, 1}, {5, 33, 2}, {5, 34, 0}, {5, 34, 1},
    {5, 35, 0}, {5, 35, 1}, {5, 35, 2}, {5, 36, 0}, {5, 36, 1}, {5, 36, 2}, {5, 37, 0}, {5, 37, 1},
    {5, 37, 2}, {5, 38, 0}, {5, 38, 1}, {5, 38, 2}, {6, 39, 0}, {6, 39, 1}, {6, 39, 2}, {6, 40, 0},
    {6, 40, 1}, {6, 40, 2}, {6, 40, 3}, {7, 41, 0}, {8, 42, 0}, {8, 43, 0}, {9, 44, 0}, {9, 44, 1},
    {9, 44, 2}, {9, 44, 3}, {9, 44, 4}, {9, 44, 5}, {9, 45, 0}, {9, 45, 1}, {9, 45, 2}, {10, 46, 0},
    {10, 46, 1}, {10, 46, 2}, {10, 47, 0}, {10, 47, 1}, {10, 47, 2}, {10, 47, 3}, {10, 48, 0},
    {10, 48, 1}, {11, 49, 0}, {11, 49, 1}, {11, 49, 2}, {12, 50, 0}, {12, 50, 1}, {12, 50, 2},
    {12, 50, 3}, {12, 51, 0}, {12, 51, 1}, {12, 52, 0}, {12, 52, 1}, {12, 52, 2}, {12, 52, 3},
    {12, 53, 0}, {12, 53, 1}, {12, 54, 0}, {12, 54, 1}, {12, 54, 2}, {13, 55, 0}, {14, 56, 0},
    {14, 56, 1}, {15, 57, 0}, {15, 58, 0}, {15, 59, 0}, {15, 59, 1}, {15, 60, 0}, {16, 61, 0},
    {16, 61, 1}, {16, 61, 2}, {16, 62, 0}, {16, 62, 1}, {16, 62, 2}, {16, 63, 0}, {16, 63, 1},
    {16, 63, 2},
};

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}

// An unsigned value through the frame to a float (std, lfd, fcfid, frsp),
// its doubleword left at `scratch`.
double Convert(uint8_t* base, uint64_t value, uint32_t scratch) {
  Store64(base, scratch, value);
  return Frsp(double(int64_t(value)));
}

// An lfs then stfs of one word: its bits (as car_makers' record setters).
void CopyFloat(uint8_t* base, uint32_t from, uint32_t to) { Store(base, to, Load(base, from)); }

// A wheel's three colors (its +8 halfword, +10 and +11 bytes x 1/255; +20,
// +22, +23; +24, +26, +27) into `to`, + 16 and + 32 as float triples, each
// field from its own `wheel()` (a Params::GetWheel a field, or the stock
// record), through the frame's scratch doubleword.
template <typename Wheel>
void Colors(uint8_t* base, uint32_t to, const Wheel& wheel, double scale, uint32_t scratch) {
  for (const uint32_t from : {8u, 20u, 24u}) {
    const uint32_t at = to + (from == 8 ? 0 : from == 20 ? 16 : 32);
    Stfs(base, at, Convert(base, guest::LoadU16(base, wheel() + from), scratch));
    Stfs(base, at + 4, Fmuls(Convert(base, guest::LoadU8(base, wheel() + from + 2), scratch), scale));
    Stfs(base, at + 8, Fmuls(Convert(base, guest::LoadU8(base, wheel() + from + 3), scratch), scale));
  }
}

// The selected garage car of a save data: its list at +6120 walked
// [+0x19920] steps from the head halfword +204; the car's address.
uint32_t SelectedGarageCar(uint8_t* base, uint32_t save) {
  const uint32_t nodes = save + 6120;
  uint32_t n = Load(base, save + 0x19920);
  uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, nodes + 204)))) * 4 + nodes;
  while (n != 0) {
    node = uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) * 4 + nodes;
    n--;
  }
  const int64_t slot = int16_t(guest::LoadU16(base, node + 2));
  return uint32_t(uint64_t(slot) * 1968) + nodes + 212;
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

uint64_t SetWheel(uint8_t* base, const GuestCall& call, uint32_t holder, uint64_t r4,
                  uint32_t which, uint32_t car, uint32_t stack, std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_8217DD38 GarageStage::SetWheel");
  const uint32_t frame = Frame(base, stack, 3120);
  // The arguments into the caller's parameter area.
  Store(base, stack + 20, holder);
  Store(base, stack + 36, which);
  Store(base, stack + 44, car);
  for (uint32_t e = 0; e < 179; e++) {
    const uint32_t at = frame + 96 + 16 * e;
    Store(base, at, e);
    for (uint32_t k = 0; k < 3; k++) {
      Store(base, at + 4 + 4 * k, uint32_t(kWheelTable[e][k]));
    }
  }
  Store64(base, frame + 80, r4);
  const uint32_t model = StageModel(base, holder);
  const uint32_t rec = model + 32;
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t spec =
      car_specs::GetCarSpec(base, Load(base, SelectedGarageCar(base, save) + 12));
  uint64_t number = r4;
  if (int32_t(number) > 178) {
    number = 178;
  }
  const int32_t side = int32_t(Load(base, stack + 36));
  const uint32_t word = Load(base, frame + 104 + (uint32_t(number) << 4));
  const uint32_t scratch = frame + 80;
  // The wheel: Params::GetWheel once a field, or the spec's stock record.
  const uint32_t stock = spec + 8380;
  const bool is_stock = uint32_t(number) == 0;
  const auto wheel = [&] {
    return is_stock ? stock : car_specs::GetWheel(base, int32_t(number));
  };
  const double scale = Lfs(base, kByteScale);
  const bool front = side == 0 || side == 1, rear = side != 1;
  if (front) {
    Store(base, rec + 4052, word);
  }
  if (rear) {
    Store(base, rec + 4068, word);
  }
  if (front) {
    Colors(base, rec + 80, wheel, scale, scratch);
  }
  if (rear) {
    Colors(base, rec + 128, wheel, scale, scratch);
  }
  if (front) {
    CopyFloat(base, wheel() + 16, rec + 4060);
    CopyFloat(base, wheel() + 28, rec + 4056);
  }
  if (rear) {
    CopyFloat(base, wheel() + 16, rec + 4076);
    CopyFloat(base, wheel() + 28, rec + 4072);
  }
  using strings_and_font::steps::Fctiwz;
  if (front) {
    Store(base, rec + 4080, uint32_t(Fctiwz(Lfs(base, car + 3164))));
    Stfs(base, rec + 4084, Fmuls(Lfs(base, car + 3132), Lfs(base, kTireScale)));
    Stfs(base, rec + 4088, Fmuls(Lfs(base, car + 3168), Lfs(base, kHundred)));
  }
  if (rear) {
    Store(base, rec + 4092, uint32_t(Fctiwz(Lfs(base, car + 3256))));
    Stfs(base, rec + 4096, Fmuls(Lfs(base, car + 3224), Lfs(base, kTireScale)));
    Stfs(base, rec + 4100, Fmuls(Lfs(base, car + 3260), Lfs(base, kHundred)));
  }
  FitAxles(base, call, holder, car, frame, f1);
  return SceneWheelsSetSizes(base, call, Load(base, model + 5100), Load(base, model + 4140),
                             Load(base, model + 4144), Load64(base, model + 4112),
                             uint64_t(Load(base, model + 4120)) << 32, Load64(base, model + 4124),
                             uint64_t(Load(base, model + 4132)) << 32, frame);
}

uint64_t PreviewWheel(uint8_t* base, const GuestCall& call, uint32_t screen, uint64_t r4,
                      uint32_t which, uint32_t stack, std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821DFA78 TuningScreen::PreviewWheel");
  const uint32_t frame = Frame(base, stack, 4016);
  const uint32_t garage = SelectedGarageCar(base, Load(base, screen + 18904));
  const uint32_t car = frame + 80;
  race_setup::CarInit(base, car);
  if (!native::Buffers_Memcpy(base, car, garage, 1884, 0x821DFAE8)) {
    call(kMemcpy, {car, garage, 1884}, frame, 0x821DFAE8);
  }
  Store(base, car + 3888, Load(base, car + 3888) | 1);
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, car, &refreshed)) {
    call(kRefresh, {car}, frame, 0x821DFAFC);
  }
  const uint32_t holder = Load(base, Load(base, kHolder));
  return SetWheel(base, call, holder, r4, which, car, frame, f1);
}

}  // namespace stand_models
