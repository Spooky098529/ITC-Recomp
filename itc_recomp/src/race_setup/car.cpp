// The car object's parts as plain C++ (docs/functions/race-setup.md); see
// race_setup.h.

#include "race_setup/race_setup.h"

#include "base/log.h"
#include "guest.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_setup {

namespace {

// The originals' lfs then stfs of one float.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}

// `count` decal entries from `decals`, as the originals' loops store them.
void ResetDecals(uint8_t* base, uint32_t decals, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t entry = decals + 32 * i;
    CopyFloat(base, entry + 0, kOne);
    guest::StoreU16(base, entry + 20, 0);
    CopyFloat(base, entry + 4, kOne);
    guest::StoreU8(base, entry + 22, 0);
    CopyFloat(base, entry + 8, kZero);
    guest::StoreU8(base, entry + 23, 0);
    CopyFloat(base, entry + 12, kZero);
    guest::StoreU32(base, entry + 28, 0);
    CopyFloat(base, entry + 16, kZero);
    guest::StoreU32(base, entry + 24, 0xFFFFFFFFu);
  }
}

}  // namespace

void ResetDecals40(uint8_t* base, uint32_t decals) {
  ITC_NATIVE_RUNNING("sub_820CC4A0 Car::ResetDecals40");
  ResetDecals(base, decals, 40);
}

void ResetDecals6(uint8_t* base, uint32_t decals) {
  ITC_NATIVE_RUNNING("sub_820CCBF0 Car::ResetDecals6");
  ResetDecals(base, decals, 6);
}

void ResetSettings(uint8_t* base, uint32_t settings) {
  ITC_NATIVE_RUNNING("sub_820CD760 Car::ResetSettings");
  guest::StoreU8(base, settings + 61, 7);
  for (uint32_t i = 56; i <= 59; i++) {
    guest::StoreU8(base, settings + i, 2);
  }
  guest::StoreU8(base, settings + 60, 3);
  for (uint32_t i = 62; i <= 76; i++) {
    guest::StoreU8(base, settings + i, 0);
  }
  guest::StoreU8(base, settings + 77, 10);
  guest::StoreU8(base, settings + 78, 0);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU16(base, settings + 2 * i, 0);
  }
}

void Reset1568(uint8_t* base, uint32_t bytes) {
  ITC_NATIVE_RUNNING("sub_820CCC70 Car::Reset1568");
  guest::StoreU32(base, bytes + 16, 0);
  static constexpr uint8_t kBytes[12] = {0, 0, 5, 0, 0, 1, 2, 1, 2, 2, 2, 2};
  // The original's order: +0, +1, +12, then +2 to +11.
  guest::StoreU8(base, bytes + 0, 0);
  guest::StoreU8(base, bytes + 1, 0);
  guest::StoreU8(base, bytes + 12, 0);
  for (uint32_t i = 2; i < 12; i++) {
    guest::StoreU8(base, bytes + i, kBytes[i]);
  }
}

void TuningReset(uint8_t* base, uint32_t tuning, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_820CD618 CarTuning::Reset");
  guest::StoreU32(base, tuning + 0, 0);
  guest::StoreU32(base, tuning + 4, flag != 0 ? 1 : 0);
  // (Seven zero words go through the red zone first: not written.)
  const uint32_t word4 = guest::LoadU32(base, tuning + 4);
  guest::StoreU32(base, tuning + 8, 0);
  guest::StoreU32(base, tuning + 16, 0);
  guest::StoreU32(base, tuning + 12, word4 == 1 ? 1 : 0);
  for (uint32_t offset = 20; offset <= 164; offset += 4) {
    guest::StoreU32(base, tuning + offset, 0);
  }
  guest::StoreU32(base, tuning + 168, 3);
  guest::StoreU32(base, tuning + 172, 0);
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t entry = tuning + 176 + 12 * i;
    guest::StoreU32(base, entry + 8, 0);
    guest::StoreU32(base, entry + 4, 0);
    guest::StoreU32(base, entry + 0, 1);
  }
}

void DecalFromBuild(uint8_t* base, uint32_t decal, uint32_t build) {
  ITC_NATIVE_RUNNING("sub_820CDC18 CarDecal::FromBuild");
  const uint32_t w20 = guest::LoadU32(base, build + 20);
  const uint32_t w24 = guest::LoadU32(base, build + 24);
  const uint32_t w28 = guest::LoadU32(base, build + 28);
  // sub_820CC470: +20.
  guest::StoreU32(base, decal + 20, (w20 & 0xFFFF) << 16 | (w24 & 0xFF) << 8 | (w28 & 0xFF));
  // sub_820CC460, sub_820CC468: +8, +12.
  CopyFloat(base, decal + 8, build + 8);
  CopyFloat(base, decal + 12, build + 12);
  const double scale = double(guest::LoadFloat(base, build + 0));
  uint32_t mirrored = 0;
  if (scale < double(guest::LoadFloat(base, kZero))) {
    // sub_820CC450 (never seen in play): +0 = -(build +0).
    guest::StoreFloat(base, decal + 0, float(-scale));
    mirrored = 1;
  }
  // sub_820CC478: +28.
  guest::StoreU32(base, decal + 28, mirrored);
  // sub_820CC458: +4.
  CopyFloat(base, decal + 4, build + 4);
  vehicle_accessors::Object344SetFloat16(base, decal, double(guest::LoadFloat(base, build + 16)));
  scene_models::SetCarModel(base, decal, guest::LoadU32(base, build + 32));
}

uint32_t CarClear(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820CD7F0 Car::Clear");
  guest::StoreU32(base, car + 12, 0);
  TuningReset(base, car + 1648, 0);
  guest::StoreU16(base, car + 2, 0);
  guest::StoreU16(base, car + 0, 0);
  guest::StoreU16(base, car + 6, 0);
  guest::StoreU16(base, car + 8, 0);
  guest::StoreU8(base, car + 10, 0);
  guest::StoreU16(base, car + 4, 0xFFFF);
  guest::StoreU8(base, car + 11, 0);
  ResetSettings(base, car + 16);
  ResetDecals40(base, car + 288);
  ResetDecals6(base, car + 96);
  Reset1568(base, car + 1568);
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = car + 1588 + 20 * i;
    CopyFloat(base, entry + 8, kZero);
    guest::StoreU32(base, entry + 0, 0);
    CopyFloat(base, entry + 12, kZero);
    guest::StoreU32(base, entry + 4, 0);
    CopyFloat(base, entry + 16, kZero);
  }
  return car + 1568;
}

uint32_t CarInit(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820CD5D8 Car::Init");
  const uint32_t r3 = CarClear(base, car);
  guest::StoreU32(base, car + 3888, 0);
  guest::StoreU32(base, car + 3892, 1);
  return r3;
}

}  // namespace race_setup
