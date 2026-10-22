#include "race_overlays/race_overlays.h"

#include "guest.h"

#include "base/log.h"

#include <cmath>

namespace race_overlays {

namespace {

constexpr uint32_t kZero = 0x82000A34;        // the game's float 0.0
constexpr uint32_t kRaceData = 0x826C5B2C;    // the race's data (racer records, mode)
constexpr uint32_t kViewportTable = 0x82008558;
constexpr uint32_t kAxleScale = 0x82001FBC;
constexpr uint32_t kWheelBytes = 368;

uint32_t Vehicle(uint8_t* base, uint32_t view) {
  return guest::LoadU32(base, view + 4);
}

}  // namespace

uint32_t View(uint8_t* base, uint32_t overlays, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_82126550 CarOverlays::View");
  if (slot == 25) {
    return 0;
  }
  return guest::LoadU32(base, overlays + 4 * (slot + 1));
}

uint32_t ViewportEntry(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821264A8 CarOverlays::ViewportEntry");
  return kViewportTable + 12 * index;
}

float WheelSpin(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821268F0 OverlayView::WheelSpin");
  return guest::LoadFloat(base, Vehicle(base, view) + kWheelBytes * wheel + 664);
}

float WheelValue512(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126908");
  return guest::LoadFloat(base, Vehicle(base, view) + kWheelBytes * wheel + 512);
}

float AxleValue3764(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126920");
  if (wheel < 2) {
    return guest::LoadFloat(base, Vehicle(base, view) + 3764);
  }
  if (wheel < 4) {
    return guest::LoadFloat(base, Vehicle(base, view) + 4036);
  }
  return guest::LoadFloat(base, kZero);
}

float AxleValue3384(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126958");
  float value;
  if (wheel < 2) {
    value = guest::LoadFloat(base, Vehicle(base, view) + 3384);
  } else if (wheel < 4) {
    value = guest::LoadFloat(base, Vehicle(base, view) + 3388);
  } else {
    value = guest::LoadFloat(base, kZero);
  }
  // fmuls: the product of the two singles (as doubles), rounded to single.
  return float(double(value) * double(guest::LoadFloat(base, kAxleScale)));
}

uint32_t WheelOnGround(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126998 OverlayView::WheelOnGround");
  return guest::LoadU8(base, Vehicle(base, view) + kWheelBytes * wheel + 433);
}

uint32_t VehicleByte2504(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126A80");
  return guest::LoadU8(base, Vehicle(base, view) + 2504);
}

uint32_t VehicleWord2252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AA0");
  return guest::LoadU32(base, Vehicle(base, view) + 2252);
}

uint32_t VehicleWord340(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AB0");
  return guest::LoadU32(base, Vehicle(base, view) + 340);
}

float VehicleFloat6256(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AC0");
  return guest::LoadFloat(base, Vehicle(base, view) + 6256);
}

float VehicleFloat6252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AD0");
  return guest::LoadFloat(base, Vehicle(base, view) + 6252);
}

uint32_t RaceMode(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82126D18");
  return guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 0xA3AF4);
}

uint32_t RacerFlags(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82127178 OverlayView::RacerFlags");
  const uint32_t racer = guest::LoadU32(base, Vehicle(base, view) + 5712);
  return guest::LoadU32(base, racer + 3888);
}

int32_t RacerId(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AE0 OverlayView::RacerId");
  const uint32_t records = guest::LoadU32(base, kRaceData);
  const uint32_t racer = guest::LoadU32(base, Vehicle(base, view) + 5712);
  for (uint32_t i = 0; i < 93; i++) {
    if (records + 48 + 3936 * i == racer) {
      return int32_t(guest::LoadU32(base, records + 3936 * i + 3960));
    }
  }
  return -1;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// lfs then stfs of one word: the bits as they are (the translated code's
// float to double and back folds away, signaling NaNs included).
void CopyFloat(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreU32(base, to, guest::LoadU32(base, from));
}

void CopyVector(uint8_t* base, uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyFloat(base, from + i, to + i);
  }
}

// The addresses of `count` consecutive words from `table`, stored from `to`.
void StoreEntries(uint8_t* base, uint32_t to, uint32_t table, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, to + 4 * i, table + 4 * i);
  }
}

}  // namespace

void SmokeSetVelocity(uint8_t* base, uint32_t smoke, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_8212A840 Smoke::SetVelocity");
  CopyVector(base, vector, smoke + 32544);
}

void SmokeSetRegion(uint8_t* base, uint32_t smoke, uint32_t a, uint32_t b, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8212A998 Smoke::SetRegion");
  CopyVector(base, a, smoke + 32608);
  CopyVector(base, b, smoke + 32592);
  guest::StoreU32(base, smoke + 32624, mode);
}

void ParticlesBSetPosition(uint8_t* base, uint32_t effect, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212BA70 ParticlesB::SetPosition");
  CopyVector(base, position, effect + 2784);
}

void ParticlesCSetPosition(uint8_t* base, uint32_t effect, uint32_t position, double speed) {
  ITC_NATIVE_RUNNING("sub_8212C6A8 ParticlesC::SetPosition");
  CopyVector(base, position, effect + 2672);
  guest::StoreFloat(base, effect + 2704, float(speed));
}

void SparksSetPosition(uint8_t* base, uint32_t sparks, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212DF90 Sparks::SetPosition");
  CopyVector(base, position, sparks + 28960);
}

void TrailSetLastPosition(uint8_t* base, uint32_t trail, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_821299D8 Trail::SetLastPosition");
  CopyVector(base, position, trail + 64144);
}

void MarkerSetCameraPosition(uint8_t* base, uint32_t marker, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212CE18 Marker::SetCameraPosition");
  CopyVector(base, position, marker + 272);
}

void LightSpriteSetPosition(uint8_t* base, uint32_t sprite, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8211EBE0 LightSprite::SetPosition");
  if (guest::LoadU32(base, sprite + 240) == 0) {
    return;
  }
  CopyVector(base, position, sprite + 192);
}

void ModelSpritePlaceHidden(uint8_t* base, uint32_t sprite, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82121560 ModelSprite::PlaceHidden");
  CopyVector(base, position, sprite + 272);
  guest::StoreU32(base, sprite + 336, 0);
}

void SetTextures104(uint8_t* base, uint32_t overlay, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8212C0E0 Overlay::SetTextures104");
  guest::StoreU32(base, overlay + 104, value);
}

void SetTextures108(uint8_t* base, uint32_t overlay, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8212D6D8 Overlay::SetTextures108");
  guest::StoreU32(base, overlay + 108, value);
}

void MarkerSetTextures(uint8_t* base, uint32_t marker, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8212CC10 Marker::SetTextures");
  StoreEntries(base, marker + 104, table, 4);
}

void LightSpriteSetTextures(uint8_t* base, uint32_t sprite, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8211E9C8 LightSprite::SetTextures");
  StoreEntries(base, sprite + 104, table, 9);
}

void ModelSpriteSetTextures(uint8_t* base, uint32_t sprite, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8211F408 ModelSprite::SetTextures");
  StoreEntries(base, sprite + 108, table, 25);
}

}  // namespace race_overlays

namespace race_overlays {

namespace {
constexpr uint32_t kSeed = 0x82638188;
constexpr uint32_t kRandScale = 0x82002BD4;  // probably 1/32,767
}  // namespace

uint32_t Rand(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82293568 rand");
  const uint32_t seed =
      uint32_t(int64_t(int32_t(guest::LoadU32(base, kSeed))) * 1103515245 + 12345);
  guest::StoreU32(base, kSeed, seed);
  return (seed >> 16) & 0x7FFF;
}

float RandomRange(uint8_t* base, double low, double high, uint32_t* drawn) {
  ITC_NATIVE_RUNNING("sub_8211F118 RandomRange");
  const uint32_t r = Rand(base);
  if (drawn) {
    *drawn = r;
  }
  const double range = double(float(high - low));
  const double scale = double(guest::LoadFloat(base, kRandScale));
  const double value = double(float(double(int64_t(int32_t(r)))));
  const double scaled = double(float(range * value));
  return float(std::fma(scaled, scale, low));
}

}  // namespace race_overlays
