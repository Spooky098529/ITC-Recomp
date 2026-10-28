// The scene's loading objects' resets (see objects.h).

#include "scene_loaders/objects.h"

#include "base/log.h"
#include "guest.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/pools.h"
#include "scene_models/scene_models.h"

#include <cstdlib>

namespace scene_loaders {

namespace {

constexpr uint32_t kPhysical = 0xA0000000u;
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kOne = 0x82000A30;   // 1.0

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene loaders: {} refused after its checks", what);
    std::abort();
  }
}

// The identity matrix (the four rows the original builds on its frame,
// copied by memcpy).
void Identity(uint8_t* base, uint32_t matrix) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      Store(base, matrix + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
}

// A position (0, 0, 0, 1) at `at`, in the original's store order (+8, +4,
// +0, then w at +12).
void Position(uint8_t* base, uint32_t at, uint32_t zero, uint32_t one) {
  Store(base, at + 8, zero);
  Store(base, at + 4, zero);
  Store(base, at + 0, zero);
  Store(base, at + 12, one);
}

}  // namespace

bool CarModelResettable(uint8_t* base, uint32_t car_model) {
  if (car_model >= kPhysical || kPhysical - car_model < kCarModelBytes) {
    return false;
  }
  return CarModelLoadsResettable(base, car_model + kCarModelLoads);
}

std::optional<uint32_t> CarModelReset(uint8_t* base, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820D3E98 CarModel::Reset");
  if (!CarModelResettable(base, car_model)) {
    return std::nullopt;
  }
  Store(base, car_model + kCarModelSet, Load(base, kFirstSet));
  scene_models::CarRecordReset(base, car_model + kCarModelRecord);
  Require(CarModelLoadsReset(base, car_model + kCarModelLoads).has_value(),
          "CarModelLoads::Reset");
  for (uint32_t i = 0; i < 4; i++) {
    Identity(base, car_model + 6720 + 64 * i);
  }
  // The original loads both constants after the tires' matrices.
  const uint32_t one = Load(base, kOne);
  const uint32_t zero = Load(base, kZero);
  for (uint32_t i = 0; i < 4; i++) {
    Position(base, car_model + 6976 + 16 * i, zero, one);
  }
  for (uint32_t i = 0; i < 2; i++) {
    Identity(base, car_model + 7040 + 64 * i);
    Position(base, car_model + 7168 + 16 * i, zero, one);
  }
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, car_model + 7232 + 4 * i, 0xFFFFFFFFu);
  }
  Store(base, car_model + 7200, 0);
  Store(base, car_model + 7216, zero);
  Store(base, car_model + 7220, zero);
  Store(base, car_model + 7224, zero);
  Store(base, car_model + 7228, one);
  return car_model + 7040 + 64;
}

}  // namespace scene_loaders
