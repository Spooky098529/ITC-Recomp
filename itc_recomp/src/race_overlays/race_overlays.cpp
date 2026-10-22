#include "race_overlays/race_overlays.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "hooks/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <algorithm>
#include <bit>
#include <climits>
#include <cmath>
#include <initializer_list>

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

float SpeedKmh(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_821268C0 OverlayView::SpeedKmh");
  return Vehicle_GetSpeedKmh(base, Vehicle(base, view));
}

float WheelSpin(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821268F0 OverlayView::WheelSpin");
  return guest::LoadFloat(base, Vehicle(base, view) + kWheelBytes * wheel + 664);
}

float WheelValue512(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126908 OverlayView::WheelValue512");
  return guest::LoadFloat(base, Vehicle(base, view) + kWheelBytes * wheel + 512);
}

float AxleValue3764(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126920 OverlayView::AxleValue3764");
  if (wheel < 2) {
    return guest::LoadFloat(base, Vehicle(base, view) + 3764);
  }
  if (wheel < 4) {
    return guest::LoadFloat(base, Vehicle(base, view) + 4036);
  }
  return guest::LoadFloat(base, kZero);
}

float AxleValue3384(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126958 OverlayView::AxleValue3384");
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
  ITC_NATIVE_RUNNING("sub_82126A80 OverlayView::VehicleByte2504");
  return guest::LoadU8(base, Vehicle(base, view) + 2504);
}

uint32_t VehicleWord2252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AA0 OverlayView::VehicleWord2252");
  return guest::LoadU32(base, Vehicle(base, view) + 2252);
}

uint32_t VehicleWord340(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AB0 OverlayView::VehicleWord340");
  return guest::LoadU32(base, Vehicle(base, view) + 340);
}

float VehicleFloat6256(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AC0 OverlayView::VehicleFloat6256");
  return guest::LoadFloat(base, Vehicle(base, view) + 6256);
}

float VehicleFloat6252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AD0 OverlayView::VehicleFloat6252");
  return guest::LoadFloat(base, Vehicle(base, view) + 6252);
}

uint32_t RaceMode(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82126D18 RaceOverlays::RaceMode");
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

namespace race_overlays {

namespace {

constexpr uint32_t kOne = 0x82000A30;            // the game's float 1.0
constexpr uint32_t kMarkerSlope = 0x82002FB0;
constexpr uint32_t kMarkerMaxSize = 0x82002C3C;
constexpr uint32_t kMarkerMinSize = 0x82002568;

// fctiwz as the translated code does it: NaN gives 0x80000000, values from
// INT_MAX up INT_MAX, the rest truncated (0x80000000 below INT_MIN).
int32_t Fctiwz(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_set_sd(value));
}

}  // namespace

uint32_t MarkerSet(uint8_t* base, uint32_t marker, uint32_t position, double distance,
                   uint32_t mode, uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_8212CCE0 Marker::Set");
  const uint32_t sprite = marker + 128;
  const uint32_t table = guest::LoadU32(base, marker + 120);
  CopyVector(base, position, sprite + 48);
  const float zero = guest::LoadFloat(base, kZero);
  guest::StoreU32(base, marker + 256, mode);
  guest::StoreU32(base, marker + 288, distance == double(zero) ? 1 : 0);
  if (guest::LoadU32(base, marker + 288) == 1) {
    CopyFloat(base, table + 4, sprite + 100);
  } else {
    const double slope = guest::LoadFloat(base, kMarkerSlope);
    const double most = guest::LoadFloat(base, kMarkerMaxSize);
    const double one = guest::LoadFloat(base, kOne);
    const double least = guest::LoadFloat(base, kMarkerMinSize);
    // fnmsubs, then fmadds: one - distance x slope, times the most, plus
    // the least, each rounded to single.
    double size = double(float(-std::fma(distance, slope, -one)));
    size = double(float(std::fma(size, most, least)));
    guest::StoreFloat(base, sprite + 100, float(size));
    if (size < least) {
      guest::StoreFloat(base, sprite + 100, zero);
    }
    if (double(guest::LoadFloat(base, sprite + 100)) > most) {
      guest::StoreFloat(base, sprite + 100, float(most));
    }
  }
  guest::StoreU32(base, sprite + 112, kind <= 3 ? kind : 0);
  if (guest::LoadU32(base, sprite + 96) != 0) {
    return 1;
  }
  guest::StoreU32(base, sprite + 96, 1);
  const float alpha = guest::LoadFloat(base, table + 8);
  guest::StoreFloat(base, sprite + 104, alpha);
  const int32_t alpha_int = Fctiwz(double(alpha));
  guest::StoreU32(base, sprite + 108, guest::LoadU32(base, table + 0));
  guest::StoreU32(base, sprite + 80, 255);
  guest::StoreU32(base, sprite + 84, 255);
  guest::StoreU32(base, sprite + 88, 255);
  guest::StoreU32(base, sprite + 92, uint32_t(alpha_int));
  return 1;
}

void MarkerUpdate(uint8_t* base, uint32_t marker) {
  ITC_NATIVE_RUNNING("sub_8212CBE0 Marker::Update");
  const uint32_t sprite = marker + 128;
  if (guest::LoadU32(base, sprite + 96) == 0) {
    return;
  }
  const uint32_t age = guest::LoadU32(base, sprite + 108) + 1;
  guest::StoreU32(base, sprite + 108, age);
  if (int32_t(age) < 0) {
    guest::StoreU32(base, sprite + 108, 0);
  }
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kTrailStart = 64128;
constexpr uint32_t kTrailCount = 64136;

uint32_t TrailPoint(uint32_t trail, uint32_t index) {
  return trail + ((index + 1) << 7);
}

// The index after `index`, modulo the capacity (divw then the remainder, as
// the original).
uint32_t TrailNext(int32_t index, int32_t capacity) {
  return uint32_t(index - (index / capacity) * capacity);
}

}  // namespace

bool TrailUpdate(uint8_t* base, uint32_t trail) {
  ITC_NATIVE_RUNNING("sub_821292E8 Trail::Update");
  const int32_t capacity = int32_t(guest::LoadU32(base, trail + 104));
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const uint32_t table = guest::LoadU32(base, trail + 112);
  int32_t left = int32_t(guest::LoadU32(base, trail + kTrailCount));
  if (left == 0) {
    return true;
  }
  uint32_t index = guest::LoadU32(base, trail + kTrailStart);
  uint32_t point = TrailPoint(trail, index);
  if (int32_t(guest::LoadU32(base, point + 60)) <= 0) {
    guest::StoreU32(base, point + 112, 0);
    guest::StoreU32(base, point + 48, 0);
    const int32_t start = int32_t(guest::LoadU32(base, trail + kTrailStart) + 1);
    const uint32_t count = guest::LoadU32(base, trail + kTrailCount) - 1;
    // The capacity again, as the original reloads it (no point's fields
    // reach +104, so it's the one checked above).
    const int32_t size = int32_t(guest::LoadU32(base, trail + 104));
    guest::StoreU32(base, trail + kTrailCount, count);
    index = TrailNext(start, size);
    guest::StoreU32(base, trail + kTrailStart, index);
    left = int32_t(count);
    if (left != 0) {
      point = TrailPoint(trail, index);
      guest::StoreU32(base, point + 112, 1);
    }
  }
  if (left <= 0) {
    return true;
  }
  const double zero = guest::LoadFloat(base, kZero);
  const double one = guest::LoadFloat(base, kOne);
  while (true) {
    int32_t age = int32_t(guest::LoadU32(base, point + 60) - 1);
    guest::StoreU32(base, point + 60, uint32_t(age));
    if (age < 0) {
      guest::StoreU32(base, point + 60, 0);
    }
    const uint32_t now = guest::LoadU32(base, point + 60);
    if (guest::LoadU32(base, table + 0) - now > 1) {
      guest::StoreU32(base, point + 48, 1);
    }
    const uint32_t fade = guest::LoadU32(base, table + 12);
    const float full = guest::LoadFloat(base, point + 124);
    if (fade > now) {
      const double ticks = double(float(double(int64_t(fade))));
      const double aged = double(float(double(int64_t(int32_t(now)))));
      double width = double(float(one / ticks));
      width = double(float(width * aged));
      width = double(float(width * double(full)));
      guest::StoreFloat(base, point + 56, float(width));
    } else {
      guest::StoreFloat(base, point + 56, full);
    }
    if (double(guest::LoadFloat(base, point + 56)) < zero) {
      guest::StoreFloat(base, point + 56, float(zero));
    }
    if (double(full) < double(guest::LoadFloat(base, point + 56))) {
      guest::StoreFloat(base, point + 56, full);
    }
    guest::StoreU32(base, point + 44,
                    uint32_t(Fctiwz(double(guest::LoadFloat(base, point + 56)))));
    left--;
    const int32_t next = int32_t(index + 1);
    const int32_t size = int32_t(guest::LoadU32(base, trail + 104));
    index = TrailNext(next, size);
    point = TrailPoint(trail, index);
    if (left <= 0) {
      return true;
    }
  }
}

}  // namespace race_overlays

namespace race_overlays {

uint32_t ModelSpriteKind(int32_t racer) {
  ITC_NATIVE_RUNNING("sub_82123188 CarOverlays::ModelSpriteKind");
  switch (racer) {
    case 382:
    case 392:
    case 393:
    case 394:
    case 397:
      return 6;
    case 391:
      return 4;
    case 357:
    case 395:
    case 396:
    case 399:
      return 1;
    case 398:
      return 5;
    default:
      return 0;
  }
}

uint32_t ModelSpriteEntry(uint8_t* base, uint32_t kind, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211F300 ModelSprite::Entry");
  uint32_t table;
  switch (kind) {
    case 1:
      table = 0x82616CC4;
      break;
    case 2:
      table = 0x82616CE8;
      break;
    case 3:
    case 4:
    case 5:
      table = 0x82616D0C;
      break;
    case 6:
      table = 0x82616D30;
      break;
    case 7:
      table = 0x82616D54;
      break;
    case 8:
      table = 0x82616D78;
      break;
    default:
      table = 0x826C5CF0;
      break;
  }
  if (index > 8) {
    return 0;
  }
  return guest::LoadU32(base, table + 4 * index);
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return REX_LOAD_U64(address);
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  REX_STORE_U64(address, value);
}

// fsubs on two singles from guest memory: the difference of the two as
// doubles, rounded to single.
float Difference(double a, double b) {
  return float(a - b);
}

}  // namespace

uint32_t ParticlesCEmit(uint8_t* base, uint32_t effect, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212C1C0 ParticlesC::Emit");
  const uint32_t capacity = guest::LoadU32(base, effect + 2700);
  const uint32_t count = guest::LoadU32(base, effect + 2696);
  const uint32_t table = guest::LoadU32(base, effect + 108);
  if (!(capacity > count)) {
    return 0;
  }
  const uint32_t one = guest::LoadU32(base, kOne);
  const uint32_t entry = effect + 112 + (guest::LoadU32(base, effect + 2692) << 7);
  // The doublewords copied as the original's ld and std, in its order.
  StoreU64(base, entry + 48, LoadU64(base, position + 0));
  StoreU64(base, entry + 56, LoadU64(base, position + 8));
  StoreU64(base, entry + 64, LoadU64(base, effect + 2672));
  StoreU64(base, entry + 72, LoadU64(base, effect + 2680));
  const float dx = Difference(guest::LoadFloat(base, position + 0),
                              guest::LoadFloat(base, effect + 2672));
  const float dy = Difference(guest::LoadFloat(base, position + 4),
                              guest::LoadFloat(base, effect + 2676));
  const float dz = Difference(guest::LoadFloat(base, position + 8),
                              guest::LoadFloat(base, effect + 2680));
  guest::StoreFloat(base, entry + 32, dx);
  guest::StoreFloat(base, entry + 36, dy);
  guest::StoreFloat(base, entry + 40, dz);
  guest::StoreU32(base, entry + 44, one);
  guest::StoreU32(base, entry + 108, guest::LoadU32(base, table + 0));
  CopyFloat(base, table + 4, entry + 100);
  const uint32_t alpha = guest::LoadU32(base, table + 8);
  guest::StoreU32(base, entry + 112, 0);
  guest::StoreU32(base, entry + 104, alpha);
  const int32_t alpha_int = Fctiwz(double(std::bit_cast<float>(alpha)));
  guest::StoreU32(base, entry + 96, 1);
  guest::StoreU32(base, entry + 84, 0);
  guest::StoreU32(base, entry + 88, 0);
  guest::StoreU32(base, entry + 80, 255);
  guest::StoreU32(base, entry + 92, uint32_t(alpha_int));
  // The head moves on (divwu, then the remainder) and the count goes up.
  const uint32_t next = guest::LoadU32(base, effect + 2692) + 1;
  const uint32_t size = guest::LoadU32(base, effect + 2700);
  const uint32_t added = guest::LoadU32(base, effect + 2696) + 1;
  const uint32_t quotient = size ? next / size : 0;
  guest::StoreU32(base, effect + 2696, added);
  guest::StoreU32(base, effect + 2692, next - quotient * size);
  return 1;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kTrailTail = 64132;
constexpr uint32_t kTrailLast = 64144;
constexpr uint32_t kTrailSpeedScale = 0x82001FBC;
constexpr uint32_t kTrailMostWidth = 0x82000C44;
constexpr uint32_t kTrailSideScale = 0x82001BE4;

// A guest vector as the translated code holds it (word 0 in the top lane),
// from four words.
simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}

// lvx: the 16 bytes at `address` rounded down to 16, as the translated code
// loads them.
simde__m128 LoadVector(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(guest::LoadU32(base, at), guest::LoadU32(base, at + 4),
                  guest::LoadU32(base, at + 8), guest::LoadU32(base, at + 12));
}

// Word `i` (0-3) of a vector held as above, as its bits.
uint32_t WordOf(simde__m128 v, int i) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return lanes[3 - i];
}

// vaddfp a, b with the first operand's NaN kept where both are NaNs (as
// the Xenon does): the sum, except that lanes where `a` is a NaN get `a`
// quieted. addps passes on its first operand's NaN, but the compiler may
// swap the operands of an addition: in this build the translated
// Smoke::Emit keeps vA first, while the translated Sparks::UpdateStreak and
// Sparks::Emit have theirs swapped (seen in the disassembly), so the sparks
// use a plain addps, which matches them as built.
simde__m128 AddKeepingFirstNaN(simde__m128 a, simde__m128 b) {
  const simde__m128 sum = simde_mm_add_ps(a, b);
  const simde__m128 a_nan = simde_mm_cmpunord_ps(a, a);
  const simde__m128 quieted =
      simde_mm_or_ps(a, simde_mm_castsi128_ps(simde_mm_set1_epi32(0x00400000)));
  return simde_mm_blendv_ps(sum, quieted, a_nan);
}

// The original's vector length, with flush-to-zero on (the caller's job):
// the squared length of x, y and z (vmsum3fp128, dpps with mask 0xEF), its
// reciprocal square root as 1/sqrt and one Newton step (vrsqrtefp,
// vnmsubfp, vmaddfp), times the squared length; the squared length itself
// where it's 0.
simde__m128 Length(simde__m128 v) {
  const simde__m128 half = simde_mm_set1_ps(0.5f);
  const simde__m128 sign = simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)));
  const simde__m128 squared = simde_mm_dp_ps(v, v, 0xEF);
  const simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(squared));
  const simde__m128 half_squared = simde_mm_mul_ps(squared, half);
  const simde__m128 zero = simde_mm_cmpeq_ps(squared, simde_mm_setzero_ps());
  const simde__m128 estimate_squared = simde_mm_mul_ps(estimate, estimate);
  const simde__m128 step = simde_mm_xor_ps(
      simde_mm_sub_ps(simde_mm_mul_ps(half_squared, estimate_squared), half), sign);
  const simde__m128 inverse =
      simde_mm_add_ps(simde_mm_mul_ps(estimate, step), estimate);
  const simde__m128 length = simde_mm_mul_ps(squared, inverse);
  return simde_mm_or_ps(simde_mm_andnot_ps(zero, length), simde_mm_and_ps(zero, squared));
}

}  // namespace

bool TrailAddPoint(uint8_t* base, uint32_t trail, uint32_t position, double speed, double width,
                   const uint32_t direction[4], uint32_t state, uint32_t value,
                   uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_82129598 Trail::AddPoint");
  const int32_t capacity = int32_t(guest::LoadU32(base, trail + 104));
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  *result = 0;
  const uint32_t table = guest::LoadU32(base, trail + 112);
  const int32_t count = int32_t(guest::LoadU32(base, trail + kTrailCount));
  if (!(capacity > count) || state == 0) {
    return true;
  }
  const uint32_t last = trail + kTrailLast;
  const uint32_t x = guest::LoadU32(base, position + 0);
  const uint32_t one = guest::LoadU32(base, kOne);
  const uint32_t zero = guest::LoadU32(base, kZero);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);

  // Away from the last position: the length of the difference isn't 0.0.
  {
    const float dx = Difference(std::bit_cast<float>(x), guest::LoadFloat(base, last + 0));
    const float dy = Difference(guest::LoadFloat(base, position + 4),
                                guest::LoadFloat(base, last + 4));
    const float dz = Difference(guest::LoadFloat(base, position + 8),
                                guest::LoadFloat(base, last + 8));
    const simde__m128 moved = VectorOf(std::bit_cast<uint32_t>(dx), std::bit_cast<uint32_t>(dy),
                                       std::bit_cast<uint32_t>(dz), one);
    Fpscr::setcsr(flushing);
    const simde__m128 length = Length(moved);
    Fpscr::setcsr(csr);
    if (double(std::bit_cast<float>(WordOf(length, 0))) == double(std::bit_cast<float>(zero))) {
      return true;
    }
  }
  if (count == 0 && state == 2) {
    state = 1;
  }

  const uint32_t point = trail + ((guest::LoadU32(base, trail + kTrailTail) + 1) << 7);
  const float speed_scale = guest::LoadFloat(base, kTrailSpeedScale);
  guest::StoreU32(base, point + 0, x);
  CopyFloat(base, position + 4, point + 4);
  CopyFloat(base, position + 8, point + 8);
  CopyFloat(base, position + 12, point + 12);
  CopyVector(base, last, point + 16);
  for (uint32_t i = 0; i < 12; i += 4) {
    const float d = Difference(guest::LoadFloat(base, position + i),
                               guest::LoadFloat(base, last + i));
    guest::StoreFloat(base, point + 64 + i, d);
  }
  guest::StoreFloat(base, point + 52, float(width));
  guest::StoreU32(base, point + 112, state);
  // fmadds: speed times the scale plus the table's +8, capped.
  double size = double(float(
      std::fma(speed, double(speed_scale), double(guest::LoadFloat(base, table + 8)))));
  const double most = guest::LoadFloat(base, kTrailMostWidth);
  if (size > most) {
    size = most;
  }
  guest::StoreFloat(base, point + 124, float(size));
  guest::StoreFloat(base, point + 56, float(size));
  const uint32_t life = guest::LoadU32(base, table + 0);
  guest::StoreU32(base, point + 48, value);
  const int32_t size_int = Fctiwz(size);
  guest::StoreU32(base, point + 60, life);
  guest::StoreU32(base, point + 120, 0);
  const simde__m128 along = VectorOf(direction[0], direction[1], direction[2], direction[3]);
  guest::StoreU32(base, point + 32, 255);
  guest::StoreU32(base, point + 36, 255);
  guest::StoreU32(base, point + 40, 255);
  guest::StoreU32(base, point + 44, uint32_t(size_int));

  // The side: `direction` crossed with the difference (vperm-made shuffles,
  // vmulfp128, vnmsubfp), scaled to the width over its length.
  const simde__m128 difference = LoadVector(base, point + 64);
  const float stored_width = guest::LoadFloat(base, point + 52);
  const float px = guest::LoadFloat(base, point + 0);
  const simde__m128 sign = simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)));
  Fpscr::setcsr(flushing);
  simde__m128 side = simde_mm_mul_ps(
      simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(difference), 0x9C)),
      simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(along), 0x78)));
  side = simde_mm_xor_ps(
      simde_mm_sub_ps(
          simde_mm_mul_ps(
              simde_mm_castsi128_ps(
                  simde_mm_shuffle_epi32(simde_mm_castps_si128(difference), 0x78)),
              simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(along), 0x9C))),
          side),
      sign);
  const simde__m128 side_length = Length(side);
  Fpscr::setcsr(csr);
  const float scale = float(double(stored_width) / double(std::bit_cast<float>(WordOf(side_length, 0))));
  const uint32_t s = std::bit_cast<uint32_t>(scale);
  Fpscr::setcsr(flushing);
  const simde__m128 scaled = simde_mm_mul_ps(side, VectorOf(s, s, s, zero));
  Fpscr::setcsr(csr);
  const double factor = guest::LoadFloat(base, kTrailSideScale);
  const double sx = float(double(std::bit_cast<float>(WordOf(scaled, 0))) * factor);
  const double sy = float(double(std::bit_cast<float>(WordOf(scaled, 1))) * factor);
  const double sz = float(double(std::bit_cast<float>(WordOf(scaled, 2))) * factor);
  guest::StoreFloat(base, point + 96, float(double(px) + sx));
  guest::StoreFloat(base, point + 100, float(sy + double(guest::LoadFloat(base, point + 4))));
  guest::StoreFloat(base, point + 104, float(sz + double(guest::LoadFloat(base, point + 8))));
  guest::StoreFloat(base, point + 80, float(double(guest::LoadFloat(base, point + 0)) - sx));
  guest::StoreFloat(base, point + 84, float(double(guest::LoadFloat(base, point + 4)) - sy));
  guest::StoreFloat(base, point + 88, float(double(guest::LoadFloat(base, point + 8)) - sz));

  // The tail moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(guest::LoadU32(base, trail + kTrailTail) + 1);
  const int32_t size_now = int32_t(guest::LoadU32(base, trail + 104));
  const uint32_t added = guest::LoadU32(base, trail + kTrailCount) + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  guest::StoreU32(base, trail + kTrailTail, uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  guest::StoreU32(base, trail + kTrailCount, added);
  *result = 1;
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kParticlesBSizeScale = 0x82008540;
constexpr uint32_t kParticlesBAngleLow = 0x820029C4;
constexpr uint32_t kParticlesBAngleHigh = 0x82002240;
constexpr uint32_t kParticlesBDistanceLow = 0x82006908;
constexpr uint32_t kParticlesBDistanceHigh = 0x82002208;
constexpr uint32_t kParticlesBSpread = 0x82001BE4;

}  // namespace

bool ParticlesBEmit(uint8_t* base, uint32_t effect, uint32_t position, double a,
                    uint32_t* result, double* sine) {
  ITC_NATIVE_RUNNING("sub_8212B4A8 ParticlesB::Emit");
  const int32_t capacity = int32_t(guest::LoadU32(base, effect + 104));
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const int32_t count = int32_t(guest::LoadU32(base, effect + 2776));
  const uint32_t table = guest::LoadU32(base, effect + 112);
  if (!(capacity > count)) {
    *result = 0;
    return true;
  }
  const uint32_t entry = effect + 128 + guest::LoadU32(base, effect + 2772) * 176;
  const uint32_t at = entry + 48;
  const uint32_t moved = effect + 2800;
  const double scaled =
      float(a * double(guest::LoadFloat(base, kParticlesBSizeScale)));
  const uint32_t one = guest::LoadU32(base, kOne);
  guest::StoreU32(base, entry + 96, 1);
  guest::StoreU32(base, entry + 108, guest::LoadU32(base, table + 0));
  CopyFloat(base, table + 4, entry + 100);
  CopyVector(base, position, at);
  CopyVector(base, effect + 2784, entry + 64);
  const float size = float(scaled * double(guest::LoadFloat(base, table + 8)));
  guest::StoreFloat(base, entry + 112, size);
  guest::StoreFloat(base, entry + 104, size);
  guest::StoreU32(base, entry + 80, 200);
  guest::StoreU32(base, entry + 84, 200);
  const double distance_high = guest::LoadFloat(base, kParticlesBDistanceHigh);
  guest::StoreU32(base, entry + 88, 200);
  const double distance_low = guest::LoadFloat(base, kParticlesBDistanceLow);
  guest::StoreU32(base, entry + 92, uint32_t(Fctiwz(double(size))));
  const float dx = Difference(guest::LoadFloat(base, at + 0), guest::LoadFloat(base, entry + 64));
  const float dy = Difference(guest::LoadFloat(base, at + 4), guest::LoadFloat(base, entry + 68));
  const float dz = Difference(guest::LoadFloat(base, at + 8), guest::LoadFloat(base, entry + 72));
  guest::StoreFloat(base, moved + 0, dx);
  guest::StoreFloat(base, moved + 4, dy);
  guest::StoreFloat(base, moved + 8, dz);
  guest::StoreU32(base, moved + 12, one);
  guest::StoreU32(base, entry + 116, 1);
  const double angle_low = guest::LoadFloat(base, kParticlesBAngleLow);
  const double angle_high = guest::LoadFloat(base, kParticlesBAngleHigh);
  const double spread = guest::LoadFloat(base, kParticlesBSpread);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);

  uint32_t particle = entry + 128;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, entry + 116)); i++, particle += 48) {
    const double angle = RandomRange(base, angle_low, angle_high);
    const double distance = RandomRange(base, distance_low, distance_high);
    double x = float(double(float(c_runtime::Cos(angle))) * distance);
    x = float(x * spread);
    *sine = c_runtime::Sin(angle);
    const double sine_single = float(*sine);
    guest::StoreFloat(base, particle + 0, float(x + double(guest::LoadFloat(base, at + 0))));
    CopyFloat(base, at + 4, particle + 4);
    const double z = guest::LoadFloat(base, at + 8);
    double offset = float(sine_single * distance);
    offset = float(offset * spread);
    guest::StoreFloat(base, particle + 8, float(z + offset));
    // The particle's vector less the effect's +2,800 (vsubfp, both loaded
    // as lvx does).
    const simde__m128 less = LoadVector(base, moved);
    const simde__m128 from = LoadVector(base, particle);
    Fpscr::setcsr(flushing);
    const simde__m128 difference = simde_mm_sub_ps(from, less);
    Fpscr::setcsr(csr);
    const uint32_t to = particle & ~0xFu;
    for (int k = 0; k < 4; k++) {
      guest::StoreU32(base, to + 4 * k, WordOf(difference, k));
    }
    CopyVector(base, moved, particle + 16);
    const int32_t first = int32_t(Rand(base));
    guest::StoreU32(base, particle + 32, uint32_t(first - (first / 3) * 3));
    const int32_t second = int32_t(Rand(base));
    guest::StoreU32(base, particle + 36, uint32_t(second - (second / 4) * 4));
  }

  // The head moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(guest::LoadU32(base, effect + 2772) + 1);
  const int32_t size_now = int32_t(guest::LoadU32(base, effect + 104));
  const uint32_t added = guest::LoadU32(base, effect + 2776) + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  guest::StoreU32(base, effect + 2772, uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  guest::StoreU32(base, effect + 2776, added);
  *result = 1;
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kModelPoints = 5048;
constexpr uint32_t kModelParts = 5060;

// Whether the model's +5048, +5052, +5060 and +5064 are all set (the
// original's checks, in its order).
bool ModelReady(uint8_t* base, uint32_t model) {
  return guest::LoadU32(base, model + kModelPoints) != 0 &&
         guest::LoadU32(base, model + 5052) != 0 &&
         guest::LoadU32(base, model + kModelParts) != 0 &&
         guest::LoadU32(base, model + 5064) != 0;
}

}  // namespace

uint32_t ModelPartsHasPoint(uint8_t* base, uint32_t parts, int32_t index) {
  ITC_NATIVE_RUNNING("sub_820DF2A0 ModelParts::HasPoint");
  if (index < 4 || index > 21) {
    return parts;
  }
  uint32_t offset;
  if (index <= 7) {
    offset = 34116;
  } else if (index <= 13) {
    offset = 22980;
  } else {
    offset = 24500;
  }
  return guest::LoadU32(base, parts + 240 * uint32_t(index) + offset) != 0 ? 1 : 0;
}

uint32_t ModelHasPoint(uint8_t* base, uint32_t model, int32_t index) {
  ITC_NATIVE_RUNNING("sub_820D9DB0 Model::HasPoint");
  if (!ModelReady(base, model)) {
    return 0;
  }
  if (index < 4) {
    return 1;
  }
  return ModelPartsHasPoint(base, guest::LoadU32(base, model + kModelParts), index);
}

uint32_t OverlayViewHasPoint(uint8_t* base, uint32_t view, int32_t index) {
  ITC_NATIVE_RUNNING("sub_82126A78 OverlayView::HasPoint");
  return ModelHasPoint(base, guest::LoadU32(base, view + 8), index);
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// Two doublewords from `from` to `to`, the first stored before the second
// is loaded (the original's ld, std, ld, std).
void CopyDoublewords(uint8_t* base, uint32_t from, uint32_t to) {
  StoreU64(base, to + 0, LoadU64(base, from + 0));
  StoreU64(base, to + 8, LoadU64(base, from + 8));
}

}  // namespace

uint32_t ModelPartsPointPosition(uint8_t* base, uint32_t out, uint32_t parts, int32_t index) {
  ITC_NATIVE_RUNNING("sub_820E0910 ModelParts::PointPosition");
  if (index < 4 || index > 21) {
    return out;
  }
  uint32_t offset;
  if (index <= 7) {
    offset = 34256;
  } else if (index <= 13) {
    offset = 23120;
  } else {
    offset = 24640;
  }
  CopyDoublewords(base, parts + 240 * uint32_t(index) + offset, out);
  return out;
}

uint32_t ModelPointPosition(uint8_t* base, uint32_t out, uint32_t model, int32_t index) {
  ITC_NATIVE_RUNNING("sub_820DA988 Model::PointPosition");
  if (!ModelReady(base, model)) {
    // lfs then four stfs: the word's bits.
    const uint32_t zero = guest::LoadU32(base, kZero);
    guest::StoreU32(base, out + 12, zero);
    guest::StoreU32(base, out + 8, zero);
    guest::StoreU32(base, out + 4, zero);
    guest::StoreU32(base, out + 0, zero);
    return out;
  }
  if (index < 4) {
    const uint32_t points = guest::LoadU32(base, guest::LoadU32(base, model + kModelPoints) + 24);
    CopyDoublewords(base, points + 6976 + (uint32_t(index) << 4), out);
    return out;
  }
  return ModelPartsPointPosition(base, out, guest::LoadU32(base, model + kModelParts), index);
}

uint32_t OverlayViewPointPosition(uint8_t* base, uint32_t out, uint32_t view, int32_t index) {
  ITC_NATIVE_RUNNING("sub_82127140 OverlayView::PointPosition");
  return ModelPointPosition(base, out, guest::LoadU32(base, view + 8), index);
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSparksSpreadScale = 0x820029E4;
constexpr uint32_t kSparksLeastSpread = 0x82001FBC;
constexpr uint32_t kSparksMostSpread = 0x82002C50;
constexpr uint32_t kSparksModeFactor = 0x820035B0;
constexpr uint32_t kSparksStreak = 28928;
constexpr uint32_t kSparksHead = 28932;
constexpr uint32_t kSparksCount = 28940;
constexpr uint32_t kSparksSpread = 28944;
constexpr uint32_t kSparksPosition = 28960;
constexpr uint32_t kSparksStreaking = 28976;
constexpr uint32_t kSparksBytes = 144;

// The 16 bytes at `to` (rounded down to 16) plus those at `from` (rounded
// down too), stored back at `to`: lvx, lvx, vaddfp with flush-to-zero on,
// stvx.
void AddVectorTo(uint8_t* base, uint32_t to, uint32_t from, uint32_t csr, uint32_t flushing) {
  const simde__m128 a = LoadVector(base, to);
  const simde__m128 b = LoadVector(base, from);
  Fpscr::setcsr(flushing);
  const simde__m128 sum = simde_mm_add_ps(a, b);
  Fpscr::setcsr(csr);
  const uint32_t at = to & ~0xFu;
  for (int k = 0; k < 4; k++) {
    guest::StoreU32(base, at + 4 * k, WordOf(sum, k));
  }
}

}  // namespace

void SparksUpdateStreak(uint8_t* base, uint32_t sparks, double speed,
                        const uint32_t position[4], uint32_t streaking, uint32_t mode,
                        uint32_t* drawn, double* value) {
  ITC_NATIVE_RUNNING("sub_8212E660 Sparks::UpdateStreak");
  const uint64_t position_high = (uint64_t(position[0]) << 32) | position[1];
  const uint64_t position_low = (uint64_t(position[2]) << 32) | position[3];
  const uint32_t head = guest::LoadU32(base, sparks + kSparksHead);
  const uint32_t one = guest::LoadU32(base, kOne);
  double factor = std::bit_cast<float>(one);
  // fmuls, then clamped (a NaN stays).
  double spread = float(speed * double(guest::LoadFloat(base, kSparksSpreadScale)));
  const double least = guest::LoadFloat(base, kSparksLeastSpread);
  if (spread < least) {
    spread = least;
  } else {
    const double most = guest::LoadFloat(base, kSparksMostSpread);
    if (spread > most) {
      spread = most;
    }
  }
  guest::StoreFloat(base, sparks + kSparksSpread, float(spread));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, sparks + kSparksPosition + 4 * i, position[i]);
  }
  const uint32_t counter = guest::LoadU32(base, sparks + kSparksStreak);
  guest::StoreU32(base, sparks + kSparksStreaking, streaking);
  guest::StoreU32(base, sparks + kSparksStreak, counter + 1);
  if (streaking != 0) {
    StoreU64(base, sparks + 28992, LoadU64(base, sparks + 29008));
    const uint64_t newer = LoadU64(base, sparks + 29016);
    StoreU64(base, sparks + 29000, newer);
    StoreU64(base, sparks + 29008, position_high);
    StoreU64(base, sparks + 29016, position_low);
  }
  if (int32_t(guest::LoadU32(base, sparks + kSparksStreak)) > 2) {
    CopyDoublewords(base, sparks + 29008, sparks + 28992);
  }
  if (int32_t(mode) == 1) {
    factor = guest::LoadFloat(base, kSparksModeFactor);
  }
  if (guest::LoadU32(base, sparks + kSparksStreaking) != 0) {
    guest::StoreU32(base, sparks + kSparksStreak, 0);
    for (uint32_t at = sparks + 29028, k = 0; k < 50; k++, at += 16) {
      for (const int32_t offset : {-4, 0, 4}) {
        const double range = guest::LoadFloat(base, sparks + kSparksSpread);
        *value = RandomRange(base, -range, range, drawn);
        guest::StoreFloat(base, at + uint32_t(offset), float(*value * factor));
      }
    }
  } else {
    const uint32_t zero = guest::LoadU32(base, kZero);
    for (uint32_t at = sparks + 29032, k = 0; k < 50; k++, at += 16) {
      guest::StoreU32(base, at - 8, zero);
      guest::StoreU32(base, at - 4, zero);
      guest::StoreU32(base, at + 0, zero);
      guest::StoreU32(base, at + 4, one);
    }
  }

  if (int32_t(guest::LoadU32(base, sparks + kSparksCount)) <= 0) {
    return;
  }
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  uint32_t spark = sparks + head * kSparksBytes + 128;
  int32_t i = 0;
  do {
    if (int32_t(guest::LoadU32(base, spark + 128)) == 2) {
      AddVectorTo(base, spark + 64, spark + 32, csr, flushing);
    }
    if (int32_t(guest::LoadU32(base, spark + 128)) == 1) {
      if (guest::LoadU32(base, sparks + kSparksStreaking) != 0) {
        const float dx = Difference(guest::LoadFloat(base, spark + 48),
                                    guest::LoadFloat(base, spark + 64));
        const float dy = Difference(guest::LoadFloat(base, spark + 52),
                                    guest::LoadFloat(base, spark + 68));
        const float dz = Difference(guest::LoadFloat(base, spark + 56),
                                    guest::LoadFloat(base, spark + 72));
        const float limit = guest::LoadFloat(base, spark + 136);
        const simde__m128 moved =
            VectorOf(std::bit_cast<uint32_t>(dx), std::bit_cast<uint32_t>(dy),
                     std::bit_cast<uint32_t>(dz), one);
        Fpscr::setcsr(flushing);
        const simde__m128 length = Length(moved);
        Fpscr::setcsr(csr);
        if (double(limit) < double(std::bit_cast<float>(WordOf(length, 0)))) {
          guest::StoreU32(base, spark + 128, 2);
        } else {
          StoreU64(base, spark + 64, position_high);
          StoreU64(base, spark + 72, position_low);
        }
        *drawn = Rand(base);
        const int32_t r = int32_t(*drawn);
        if (r - (r / 10) * 10 == 0) {
          guest::StoreU32(base, spark + 128, 2);
        }
        for (uint32_t k = 0; k < 12; k += 4) {
          guest::StoreFloat(base, spark + 112 + k,
                            Difference(guest::LoadFloat(base, spark + 64 + k),
                                       guest::LoadFloat(base, spark + 48 + k)));
        }
      } else {
        AddVectorTo(base, spark + 64, spark + 32, csr, flushing);
      }
    }
    const uint32_t last = guest::LoadU32(base, sparks + 104) * kSparksBytes + sparks - 16;
    spark = spark == last ? sparks + 128 : spark + kSparksBytes;
    i++;
  } while (i < int32_t(guest::LoadU32(base, sparks + kSparksCount)));
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSparksEmitHead = 28936;
constexpr uint32_t kSparksSizeScale = 0x82001BE4;
constexpr uint32_t kSparksLimitHigh = 0x820021E4;
constexpr uint32_t kSparksLimitLow = 0x820021E8;
constexpr uint32_t kSparksStep = 0x82002674;
constexpr uint32_t kSparksAngleLow = 0x820029C4;
constexpr uint32_t kSparksAngleHigh = 0x82002240;
constexpr uint32_t kSparksSpreadLow = 0x82006908;
constexpr uint32_t kSparksSpreadHigh = 0x82002208;
constexpr uint32_t kSparksSpeedOut = 0x82002204;
constexpr uint32_t kSparksSpeedIn = 0x820087B8;
constexpr uint32_t kSparksRiseState0 = 0x820087B0;
constexpr uint32_t kSparksRiseState2 = 0x820087B4;

// fctiwz of a float word, as the original's lfs, fctiwz, stfiwx.
uint32_t FloatToInt(uint8_t* base, uint32_t address) {
  return uint32_t(Fctiwz(double(guest::LoadFloat(base, address))));
}

}  // namespace

bool SparksEmit(uint8_t* base, uint32_t sparks, uint32_t position, double strength,
                const uint32_t vector[4], uint32_t* result, double* last) {
  ITC_NATIVE_RUNNING("sub_8212D818 Sparks::Emit");
  const int32_t capacity = int32_t(guest::LoadU32(base, sparks + 104));
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const int32_t count = int32_t(guest::LoadU32(base, sparks + kSparksCount));
  const uint32_t table = guest::LoadU32(base, sparks + 112);
  if (!(capacity > count)) {
    *result = 0;
    return true;
  }
  const uint32_t spark =
      sparks + guest::LoadU32(base, sparks + kSparksEmitHead) * kSparksBytes + 128;
  const double vx = std::bit_cast<float>(vector[0]);
  const double vy = std::bit_cast<float>(vector[1]);
  const double vz = std::bit_cast<float>(vector[2]);

  // Size, life and the limit.
  const double scale = guest::LoadFloat(base, kSparksSizeScale);
  {
    const double high = guest::LoadFloat(base, table + 4);
    *last = RandomRange(base, float(high * scale), high);
    guest::StoreFloat(base, spark + 100, float(*last));
  }
  CopyFloat(base, table + 8, spark + 104);
  const double limit_high = guest::LoadFloat(base, kSparksLimitHigh);
  const double limit_low = guest::LoadFloat(base, kSparksLimitLow);
  const uint32_t life = guest::LoadU32(base, table + 0);
  guest::StoreU32(base, spark + 96, 1);
  guest::StoreU32(base, spark + 132, 0);
  guest::StoreU32(base, spark + 108, life);
  *last = RandomRange(base, limit_low, limit_high);
  guest::StoreFloat(base, spark + 136, float(*last));

  // The state.
  {
    const int32_t drawn = int32_t(Rand(base));
    const int32_t state = drawn - (drawn / 5) * 5;
    guest::StoreU32(base, spark + 128, uint32_t(state));
    if (state > 1) {
      guest::StoreU32(base, spark + 128, 1);
    }
  }
  const double zero = guest::LoadFloat(base, kZero);
  if (strength > zero) {
    guest::StoreU32(base, spark + 128, 2);
  }

  // The colors.
  {
    const int32_t drawn = int32_t(Rand(base));
    const uint32_t pick = uint32_t(drawn - (drawn / 5) * 5);
    if (pick == 1 || pick == 3) {
      const uint32_t alpha = FloatToInt(base, spark + 104);
      guest::StoreU32(base, spark + 84, 200);
      guest::StoreU32(base, spark + 80, 255);
      guest::StoreU32(base, spark + 88, 200);
      guest::StoreU32(base, spark + 92, alpha);
    } else if (pick < 5) {
      const uint32_t color = pick == 0 ? 200 : 255;
      guest::StoreU32(base, spark + 92, FloatToInt(base, spark + 104));
      guest::StoreU32(base, spark + 88, color);
      guest::StoreU32(base, spark + 84, color);
      guest::StoreU32(base, spark + 80, color);
    }
  }

  // The position.
  const uint32_t at = spark + 48;
  if (guest::LoadU32(base, spark + 128) != 0) {
    for (uint32_t i = 0; i < 16; i += 4) {
      CopyFloat(base, position + i, spark + 64 + i);
    }
    for (uint32_t i = 0; i < 16; i += 4) {
      CopyFloat(base, position + i, at + i);
    }
    if (strength > zero) {
      const uint32_t one = guest::LoadU32(base, kOne);
      // Toward the sparks' position less the vector, by a random step.
      const double step_high = limit_low;
      double radius = RandomRange(base, std::bit_cast<float>(one), step_high);
      *last = radius;
      const float dx = Difference(guest::LoadFloat(base, sparks + kSparksPosition + 0), vx);
      const float dy = Difference(guest::LoadFloat(base, sparks + kSparksPosition + 4), vy);
      const float dz = Difference(guest::LoadFloat(base, sparks + kSparksPosition + 8), vz);
      const simde__m128 toward =
          VectorOf(std::bit_cast<uint32_t>(dx), std::bit_cast<uint32_t>(dy),
                   std::bit_cast<uint32_t>(dz), one);
      const uint32_t csr = Fpscr::getcsr();
      const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
      Fpscr::setcsr(flushing);
      const simde__m128 length = Length(toward);
      Fpscr::setcsr(csr);
      const double length_x = std::bit_cast<float>(WordOf(length, 0));
      if (!(length_x == zero)) {
        const simde__m128 from = LoadVector(base, at);
        const float s = float(double(guest::LoadFloat(base, kSparksStep)) / length_x);
        const uint32_t sw = std::bit_cast<uint32_t>(s);
        const simde__m128 factor = VectorOf(sw, sw, sw, guest::LoadU32(base, kZero));
        Fpscr::setcsr(flushing);
        const simde__m128 moved = simde_mm_add_ps(from, simde_mm_mul_ps(toward, factor));
        Fpscr::setcsr(csr);
        const uint32_t to = at & ~0xFu;
        for (int k = 0; k < 4; k++) {
          guest::StoreU32(base, to + 4 * k, WordOf(moved, k));
        }
      }
      // And by a random angle and radius.
      const double angle = RandomRange(base, guest::LoadFloat(base, kSparksAngleLow),
                                       guest::LoadFloat(base, kSparksAngleHigh));
      radius = RandomRange(base, -radius, radius);
      const double x = float(double(float(double(float(c_runtime::Cos(angle))) * radius)) * scale);
      const double y = float(double(float(double(float(c_runtime::Cos(angle))) * radius)) * scale);
      *last = c_runtime::Sin(angle);
      const double sine = float(*last);
      guest::StoreFloat(base, at + 0, float(x + double(guest::LoadFloat(base, at + 0))));
      guest::StoreFloat(base, at + 4, float(double(guest::LoadFloat(base, at + 4)) + y));
      const double z = float(double(float(sine * radius)) * scale);
      guest::StoreFloat(base, at + 8, float(double(guest::LoadFloat(base, at + 8)) + z));
    }
  } else {
    // Spread by random amounts.
    const double low = zero;
    const double high_x = Difference(guest::LoadFloat(base, sparks + kSparksPosition + 0), vx);
    const double high_z = Difference(guest::LoadFloat(base, sparks + kSparksPosition + 8), vz);
    const double ox = RandomRange(base, low, high_x);
    const double oy = RandomRange(base, guest::LoadFloat(base, kSparksSpreadLow),
                                  guest::LoadFloat(base, kSparksSpreadHigh));
    const double oz = RandomRange(base, low, high_z);
    *last = oz;
    for (uint32_t i = 0; i < 16; i += 4) {
      CopyFloat(base, position + i, spark + 64 + i);
    }
    guest::StoreFloat(base, at + 0, float(double(guest::LoadFloat(base, position + 0)) + ox));
    guest::StoreFloat(base, at + 4, float(double(guest::LoadFloat(base, position + 4)) + oy));
    guest::StoreFloat(base, at + 8, float(double(guest::LoadFloat(base, position + 8)) + oz));
  }

  // The offset from where it started, and the velocity.
  for (uint32_t i = 0; i < 12; i += 4) {
    guest::StoreFloat(base, spark + 112 + i,
                      Difference(guest::LoadFloat(base, at + i),
                                 guest::LoadFloat(base, spark + 64 + i)));
  }
  const double side_x = Difference(vx, guest::LoadFloat(base, position + 0));
  const double side_z = Difference(vz, guest::LoadFloat(base, position + 8));
  double rise = guest::LoadFloat(base, kSparksSpeedOut);
  const double inward = guest::LoadFloat(base, kSparksSpeedIn);
  const double toward_x = side_x < zero ? inward : rise;
  const double toward_z = side_z < zero ? inward : rise;
  const uint32_t state = guest::LoadU32(base, spark + 128);
  if (state == 1) {
    const double vz_drawn = RandomRange(base, zero, toward_z);
    const double vy_drawn = RandomRange(base, inward, rise);
    const double vx_drawn = RandomRange(base, zero, toward_x);
    *last = vx_drawn;
    guest::StoreFloat(base, spark + 32, float(vx_drawn));
    guest::StoreFloat(base, spark + 36, float(vy_drawn));
    guest::StoreFloat(base, spark + 40, float(vz_drawn));
    guest::StoreU32(base, spark + 44, guest::LoadU32(base, kZero));
    const int32_t drawn = int32_t(Rand(base));
    if (drawn - (drawn / 20) * 20 == 0) {
      guest::StoreFloat(base, spark + 32,
                        float(toward_x + double(guest::LoadFloat(base, spark + 32))));
      const double y = guest::LoadFloat(base, spark + 36);
      const double z = guest::LoadFloat(base, spark + 40);
      guest::StoreFloat(base, spark + 36, float(y + rise));
      guest::StoreFloat(base, spark + 40, float(z + toward_z));
    }
  } else if (state < 3) {
    if (double(guest::LoadFloat(base, spark + 52)) < double(guest::LoadFloat(base, position + 4))) {
      rise = guest::LoadFloat(base, state == 0 ? kSparksRiseState0 : kSparksRiseState2);
    }
    const double vz_drawn = RandomRange(base, zero, toward_z);
    const double vy_drawn = RandomRange(base, zero, rise);
    const double vx_drawn = RandomRange(base, zero, toward_x);
    *last = vx_drawn;
    guest::StoreU32(base, spark + 44, guest::LoadU32(base, kZero));
    guest::StoreFloat(base, spark + 40, float(vz_drawn));
    guest::StoreFloat(base, spark + 36, float(vy_drawn));
    guest::StoreFloat(base, spark + 32, float(vx_drawn));
  }

  // The head moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(guest::LoadU32(base, sparks + kSparksEmitHead) + 1);
  const int32_t size_now = int32_t(guest::LoadU32(base, sparks + 104));
  const uint32_t added = guest::LoadU32(base, sparks + kSparksCount) + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  guest::StoreU32(base, sparks + kSparksEmitHead,
                  uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  guest::StoreU32(base, sparks + kSparksCount, added);
  *result = 1;
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSmokeHead = 32532;
constexpr uint32_t kSmokeCount = 32536;
constexpr uint32_t kSmokeVelocity = 32544;
constexpr uint32_t kSmokeBytes = 720;
constexpr uint32_t kSmokeGrowthHigh = 0x82002C4C;
constexpr uint32_t kSmokeFrameScale = 0x820021E8;
constexpr uint32_t kSmokeRiseHigh = 0x820029D4;
constexpr uint32_t kSmokeSpeedScale = 0x820029E4;
constexpr uint32_t kSmokeDriftScale = 0x820086F0;
constexpr uint32_t kSmokeDriftHigh = 0x82002C48;
constexpr uint32_t kSmokeRadiusBase = 0x8200222C;
constexpr uint32_t kSmokeCountScale = 0x820029EC;
constexpr uint32_t kSmokeRadiusScale = 0x820086EC;
constexpr uint32_t kSmokeAngleLow = 0x820029C4;
constexpr uint32_t kSmokeAngleHigh = 0x82002240;

}  // namespace

bool SmokeEmit(uint8_t* base, uint32_t smoke, double size, double speed, uint32_t frame,
               uint32_t region, uint32_t value, uint32_t* result, double* last) {
  ITC_NATIVE_RUNNING("sub_8212A350 Smoke::Emit");
  const int32_t capacity = int32_t(guest::LoadU32(base, smoke + 104));
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const int32_t count = int32_t(guest::LoadU32(base, smoke + kSmokeCount));
  const uint32_t table = guest::LoadU32(base, smoke + 112);
  if (!(capacity > count)) {
    *result = 0;
    return true;
  }
  // The puff, and the one before it (the last when this is the first).
  const int32_t head = int32_t(guest::LoadU32(base, smoke + kSmokeHead));
  const uint32_t puff = smoke + uint32_t(head) * kSmokeBytes + 128;
  uint32_t previous;
  if (puff == smoke + 128) {
    previous = smoke + uint32_t(capacity) * kSmokeBytes - 592;
  } else {
    const int32_t before = head - 1;
    const int32_t quotient = before == INT32_MIN && capacity == -1 ? 0 : before / capacity;
    previous = smoke + (uint32_t(before) - uint32_t(quotient) * uint32_t(capacity)) * kSmokeBytes +
               128;
  }

  guest::StoreFloat(base, puff + 112, float(size));
  guest::StoreU32(base, puff + 96, value);
  {
    const uint32_t alpha = guest::LoadU32(base, table + 8);
    guest::StoreU32(base, puff + 104, alpha);
    guest::StoreU32(base, puff + 120, alpha);
  }
  const double zero = guest::LoadFloat(base, kZero);
  const uint32_t zero_word = guest::LoadU32(base, kZero);
  const double growth_high = guest::LoadFloat(base, kSmokeGrowthHigh);
  guest::StoreU32(base, puff + 108, guest::LoadU32(base, table + 0));
  {
    const double drawn = RandomRange(base, zero, growth_high);
    const double frame_value = guest::LoadFloat(base, frame + 84);
    const double scale = guest::LoadFloat(base, kSmokeFrameScale);
    const float growth = float(std::fma(frame_value, scale, drawn));
    guest::StoreFloat(base, puff + 100, growth);
    guest::StoreFloat(base, puff + 124, growth);
  }
  const uint32_t one_word = guest::LoadU32(base, kOne);
  const double one = std::bit_cast<float>(one_word);
  {
    const double drawn = RandomRange(base, one, guest::LoadFloat(base, kSmokeRiseHigh));
    const double scale = guest::LoadFloat(base, kSmokeSpeedScale);
    guest::StoreFloat(base, puff + 116, float(std::fma(speed, scale, drawn)));
  }
  const uint32_t velocity = puff + 64;
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyFloat(base, frame + 64 + i, puff + 48 + i);
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyFloat(base, smoke + kSmokeVelocity + i, velocity + i);
  }
  {
    const uint32_t alpha = FloatToInt(base, puff + 104);
    guest::StoreU32(base, puff + 80, 255);
    guest::StoreU32(base, puff + 84, 255);
    guest::StoreU32(base, puff + 88, 255);
    guest::StoreU32(base, puff + 92, alpha);
  }

  // The drift.
  double span_x, span_y, span_z;
  if (region != 0) {
    span_x = Difference(guest::LoadFloat(base, smoke + 32576), guest::LoadFloat(base, smoke + 32560));
    span_y = Difference(guest::LoadFloat(base, smoke + 32580), guest::LoadFloat(base, smoke + 32564));
    span_z = Difference(guest::LoadFloat(base, smoke + 32584), guest::LoadFloat(base, smoke + 32568));
  } else {
    span_x = Difference(guest::LoadFloat(base, puff + 48), guest::LoadFloat(base, velocity + 0));
    span_y = Difference(guest::LoadFloat(base, puff + 52), guest::LoadFloat(base, velocity + 4));
    span_z = Difference(guest::LoadFloat(base, puff + 56), guest::LoadFloat(base, velocity + 8));
  }
  const double reach = RandomRange(
      base, zero, float(speed * double(guest::LoadFloat(base, kSmokeDriftScale))));
  const double radius = float(reach + double(guest::LoadFloat(base, kSmokeRadiusBase)));
  const double along = RandomRange(base, zero, guest::LoadFloat(base, kSmokeDriftHigh));
  const float ax = float(span_x * along);
  const float ay = float(span_y * along);
  const float az = float(span_z * along);
  const double ox = RandomRange(base, -radius, radius);
  const double oz = RandomRange(base, -radius, radius);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  const simde__m128 drift_a = VectorOf(std::bit_cast<uint32_t>(ax), std::bit_cast<uint32_t>(ay),
                                       std::bit_cast<uint32_t>(az), one_word);
  const simde__m128 drift_b =
      VectorOf(std::bit_cast<uint32_t>(float(oz)), zero_word, std::bit_cast<uint32_t>(float(ox)),
               one_word);
  Fpscr::setcsr(flushing);
  const simde__m128 drift = AddKeepingFirstNaN(drift_a, drift_b);
  Fpscr::setcsr(csr);
  int32_t particles = Fctiwz(double(float(speed * double(guest::LoadFloat(base, kSmokeCountScale)))));
  if (particles < 2) {
    particles = 2;
  } else if (particles > 10) {
    particles = 10;
  }
  guest::StoreU32(base, puff + 128, uint32_t(particles));

  // The particles.
  const double step_limit = float(speed * double(guest::LoadFloat(base, kSmokeRadiusScale)));
  const double angle_high = guest::LoadFloat(base, kSmokeAngleHigh);
  const double angle_low = guest::LoadFloat(base, kSmokeAngleLow);
  uint32_t particle = puff + 240;
  for (int32_t k = 0; k < int32_t(guest::LoadU32(base, puff + 128)); k++, particle += 48) {
    const double limit = step_limit > one ? one : step_limit;
    const double share = float(double(int64_t(int32_t(guest::LoadU32(base, puff + 128)))));
    const double dx = Difference(guest::LoadFloat(base, velocity + 0), guest::LoadFloat(base, puff + 48));
    const double dz = Difference(guest::LoadFloat(base, velocity + 8), guest::LoadFloat(base, puff + 56));
    const double dy = Difference(guest::LoadFloat(base, velocity + 4), guest::LoadFloat(base, puff + 52));
    const double part = float(one / share);
    const double step_x = float(dx * part);
    const double step_y = float(dy * part);
    const double step_z = float(dz * part);
    // The angle and radius only place the particle in the branch whose
    // stores are overwritten (below), but they're drawn all the same.
    const double angle = RandomRange(base, angle_low, angle_high);
    RandomRange(base, -limit, limit);
    *last = c_runtime::Sin(angle);
    for (int i = 0; i < 4; i++) {
      guest::StoreU32(base, particle + 16 + 4 * i, WordOf(drift, i));
    }
    {
      const int32_t drawn = int32_t(Rand(base));
      guest::StoreU32(base, particle + 32, uint32_t(drawn - (drawn / 3) * 3));
    }
    {
      const int32_t drawn = int32_t(Rand(base));
      guest::StoreU32(base, particle + 40, 1);
      guest::StoreU32(base, particle + 36, uint32_t(drawn - (drawn / 4) * 4));
    }
    const int32_t apart = int32_t(guest::LoadU32(base, puff + 108) - guest::LoadU32(base, previous + 108));
    if (apart <= 2) {
      const double index = float(double(int64_t(k)));
      guest::StoreFloat(base, particle + 0,
                        float(double(float(step_x * index)) + double(guest::LoadFloat(base, puff + 48))));
      const double y = float(step_y * index);
      guest::StoreFloat(base, particle + 4, float(double(guest::LoadFloat(base, puff + 52)) + y));
      const double z = float(step_z * index);
      guest::StoreFloat(base, particle + 8, float(double(guest::LoadFloat(base, puff + 56)) + z));
    } else {
      // (The original first stores +48 plus the cosine offset and +56 plus
      // the sine offset, then overwrites them with these.)
      for (uint32_t i = 0; i < 16; i += 4) {
        CopyFloat(base, puff + 48 + i, particle + i);
      }
      if (uint32_t(k) >= 5) {
        guest::StoreU32(base, puff + 128, 5);
        break;
      }
    }
  }

  // The head moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(guest::LoadU32(base, smoke + kSmokeHead) + 1);
  const int32_t size_now = int32_t(guest::LoadU32(base, smoke + 104));
  const uint32_t added = guest::LoadU32(base, smoke + kSmokeCount) + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  guest::StoreU32(base, smoke + kSmokeHead,
                  uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  guest::StoreU32(base, smoke + kSmokeCount, added);
  *result = 1;
  return true;
}

bool EmitSmoke(uint8_t* base, uint32_t overlays, double rate, double speed, uint32_t frame,
               uint32_t wheel, uint32_t value, double* last) {
  ITC_NATIVE_RUNNING("sub_82123240 CarOverlays::EmitSmoke");
  const uint32_t accumulator = overlays + (wheel + 11) * 48;
  const uint32_t smoke = guest::LoadU32(base, overlays + ((wheel + 41) << 2));
  const uint32_t region = wheel >= 4 ? 1 : 0;
  if (rate == double(guest::LoadFloat(base, kZero))) {
    return true;
  }
  const int32_t capacity = int32_t(guest::LoadU32(base, smoke + 104));
  if (capacity == 0 || capacity == -1 ||
      (accumulator + 4 > smoke + 104 && accumulator < smoke + 108)) {
    return false;
  }
  const float sum = float(double(guest::LoadFloat(base, accumulator)) + rate);
  guest::StoreFloat(base, accumulator, sum);
  if (double(sum) < double(guest::LoadFloat(base, kOne))) {
    return true;
  }
  // fctidz, stfiwx, then the low word back as an unsigned number (lwz,
  // std, fcfid).
  int64_t whole_64;
  if (std::isnan(double(sum))) {
    whole_64 = INT64_MIN;
  } else if (double(sum) > double(INT64_MAX)) {
    whole_64 = INT64_MAX;
  } else {
    whole_64 = simde_mm_cvttsd_si64(simde_mm_set_sd(double(sum)));
  }
  const double whole = float(double(int64_t(uint32_t(whole_64))));
  *last = whole;
  uint32_t result = 0;
  SmokeEmit(base, smoke, whole, speed, frame, region, value, &result, last);
  guest::StoreFloat(base, accumulator,
                    float(double(guest::LoadFloat(base, accumulator)) - whole));
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kOverlayVtable = 0x820085D8;
constexpr uint32_t kOverlayViewVtable = 0x82010B5C;
constexpr uint32_t kViewportOwners = 0x826C2CE8;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kRenderTargets = 0x826C5D5C;

}  // namespace

void OverlayConstruct(uint8_t* base, uint32_t overlay) {
  ITC_NATIVE_RUNNING("sub_82126498 Overlay::Overlay");
  guest::StoreU32(base, overlay, kOverlayVtable);
}

void ClearViews(uint8_t* base, uint32_t overlays) {
  ITC_NATIVE_RUNNING("sub_82126530 CarOverlays::ClearViews");
  for (uint32_t i = 0; i < 25; i++) {
    guest::StoreU32(base, overlays + 4 + 4 * i, 0);
  }
}

void ViewDescClear(uint8_t* base, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_82126860 ViewDesc::Clear");
  guest::StoreU32(base, desc, 0);
  guest::StoreU32(base, desc + 4, 0);
}

void OverlayViewConstruct(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126870 OverlayView::OverlayView");
  guest::StoreU32(base, view, kOverlayViewVtable);
  guest::StoreU32(base, view + 4, 0);
  guest::StoreU32(base, view + 8, 0);
}

void OverlayViewSet(uint8_t* base, uint32_t view, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_82126890 OverlayView::Set");
  // Word by word, in the original's order (the two may overlap).
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, view + 4 + 4 * i, guest::LoadU32(base, desc + 4 * i));
  }
}

void BindViewport(uint8_t* base, uint32_t out, uint32_t index, uint32_t second) {
  ITC_NATIVE_RUNNING("sub_821264C0 CarOverlays::BindViewport");
  const uint32_t entry = kViewportTable + 12 * index;
  const uint32_t pair = kViewportTable + 72 + 8 * second;
  const uint32_t owners = guest::LoadU32(base, kViewportOwners);
  const uint32_t owner = guest::LoadU32(base, owners + 4 * guest::LoadU32(base, entry));
  guest::StoreU32(base, out, owner + 80 * guest::LoadU32(base, entry + 4));
  guest::StoreU32(base, out + 4, kCameraPasses + 592 * guest::LoadU32(base, entry + 8));
  guest::StoreU32(base, out + 8, guest::LoadU32(base, pair));
  guest::StoreU32(base, out + 12, guest::LoadU32(base, pair + 4));
}

void SetRenderTargets(uint8_t* base, uint32_t queue, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82128190 Overlays::SetRenderTargets");
  guest::StoreU32(base, kRenderTargets, queue);
  guest::StoreU32(base, kRenderTargets + 4, list);
}

void ParticleConstruct(uint8_t* base, uint32_t particle) {
  ITC_NATIVE_RUNNING("sub_8212B3D0 Particle::Particle");
  const uint32_t one = guest::LoadU32(base, kOne);
  guest::StoreU32(base, particle + 12, one);
  guest::StoreU32(base, particle + 28, one);
}

void RaceOverlaysClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8215DE80 RaceOverlays::Clear");
  for (uint32_t i = 0; i < 25; i++) {
    guest::StoreU32(base, list + 4 * i, 0);
  }
}

uint32_t ShareTextures(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8215E000 RaceOverlays::ShareTextures");
  uint32_t entry = 0;
  for (uint32_t i = 1; i < 25; i++) {
    entry = guest::LoadU32(base, list + 4 * i);
    if (entry != 0) {
      SetTextures104(base, entry, guest::LoadU32(base, guest::LoadU32(base, list) + 104));
    }
  }
  return entry;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSmokeEmitterSize = 0x82002A90;
constexpr uint32_t kFloatOne = 0x3F800000;  // the identity rows' 1.0 (vupkd3d128's)

// The stores of a reset, in the original's order: 'z' the game's 0.0 word,
// 'o' its 1.0 word, '0' and '1' the integers, 'v' a unit vector (four
// stores from the offset).
struct ResetStore {
  int32_t offset;
  char kind;
};

void ApplyStores(uint8_t* base, uint32_t at, std::initializer_list<ResetStore> stores) {
  const uint32_t zero = guest::LoadU32(base, kZero);
  const uint32_t one = guest::LoadU32(base, kOne);
  for (const ResetStore& s : stores) {
    const uint32_t to = at + uint32_t(s.offset);
    switch (s.kind) {
      case 'z':
        guest::StoreU32(base, to, zero);
        break;
      case 'o':
        guest::StoreU32(base, to, one);
        break;
      case '0':
        guest::StoreU32(base, to, 0);
        break;
      case '1':
        guest::StoreU32(base, to, 1);
        break;
      case 'v':
        guest::StoreU32(base, to, zero);
        guest::StoreU32(base, to + 4, zero);
        guest::StoreU32(base, to + 8, zero);
        guest::StoreU32(base, to + 12, one);
        break;
    }
  }
}

// One identity row as stvx stores it: at `to` rounded down to 16 bytes,
// 1.0 in word `column`, 0.0 elsewhere.
void StoreIdentityRow(uint8_t* base, uint32_t to, uint32_t column) {
  const uint32_t at = to & ~0xFu;
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, at + 4 * i, i == column ? kFloatOne : 0);
  }
}

// A sprite's state as LightSprite::Reset, ModelSprite::Reset and
// Marker::Reset clear it (the first part, the same in the three).
void ClearSpriteVectors(uint8_t* base, uint32_t sprite) {
  ApplyStores(base, sprite, {{48, 'v'}, {64, 'v'}, {32, 'v'}, {80, '0'}, {84, '0'}, {88, '0'},
                             {92, '1'}, {0, 'v'}, {16, 'v'}});
}

}  // namespace

void LightSpriteReset(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_8211E9F0 LightSprite::Reset");
  ApplyStores(base, sprite, {{272, 'v'}});
  const uint32_t inner = sprite + 144;
  ClearSpriteVectors(base, inner);
  ApplyStores(base, inner, {{96, '0'}, {100, 'z'}, {108, '0'}, {104, 'z'}});
  guest::StoreU32(base, inner + 116, 4);
  guest::StoreU32(base, inner + 112, 0);
}

void ModelSpriteReset(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_8211F430 ModelSprite::Reset");
  const uint32_t inner = sprite + 224;
  ClearSpriteVectors(base, inner);
  ApplyStores(base, inner, {{96, '0'}, {108, '0'}, {100, 'z'}, {128, '0'}, {104, 'z'}});
  guest::StoreU32(base, inner + 112, 9);
  guest::StoreU32(base, inner + 120, 9);
  guest::StoreU32(base, inner + 116, 4);
  guest::StoreU32(base, inner + 124, 12);
  ApplyStores(base, sprite, {{368, 'v'}, {384, 'v'}});
  guest::StoreU32(base, sprite + 404, 0xFFFFFFFF);
  guest::StoreU32(base, sprite + 408, 0xFFFFFFFF);
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, sprite + 416 + 16 * i, {{0, 'v'}});
  }
}

void SpriteResetStyle2(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_82123A00 Sprite::ResetStyle2");
  ApplyStores(base, sprite, {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {64, '0'}, {68, '0'}});
  guest::StoreU32(base, sprite + 72, 2);
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow(base, sprite + 16 * row, row);
  }
  ApplyStores(base, sprite, {{92, 'o'}, {108, 'o'}});
}

void TrailReset(uint8_t* base, uint32_t trail) {
  ITC_NATIVE_RUNNING("sub_82129498 Trail::Reset");
  ApplyStores(base, trail, {{64128, '0'}, {64132, '0'}, {64136, '0'}, {64144, 'v'}});
  for (uint32_t i = 0; i < guest::LoadU32(base, trail + 104); i++) {
    ApplyStores(base, trail + 128 + 128 * i,
                {{0, 'v'}, {16, 'v'}, {64, 'v'}, {96, 'v'}, {80, 'v'}, {32, '0'}, {36, '0'},
                 {40, '0'}, {44, '1'}, {52, 'z'}, {56, 'z'}, {112, '0'}, {124, 'z'}, {60, '0'},
                 {116, '0'}, {48, '0'}, {120, '0'}});
  }
}

void SmokeEmitterReset(uint8_t* base, uint32_t emitter) {
  ITC_NATIVE_RUNNING("sub_8212A218 SmokeEmitter::Reset");
  ApplyStores(base, emitter, {{48, 'v'}, {64, 'v'}, {32, 'v'}, {208, 'v'}});
  // The original's order: rows 1, 0, 3, 2.
  StoreIdentityRow(base, emitter + 160, 1);
  StoreIdentityRow(base, emitter + 144, 0);
  StoreIdentityRow(base, emitter + 192, 3);
  StoreIdentityRow(base, emitter + 176, 2);
  ApplyStores(base, emitter,
              {{0, 'v'}, {16, 'v'}, {80, '0'}, {84, '0'}, {88, '0'}, {92, '1'}, {104, 'z'},
               {120, 'z'}, {108, '0'}, {124, 'z'}, {96, '0'}, {100, 'z'}});
  guest::StoreU32(base, emitter + 128, 10);
  ApplyStores(base, emitter, {{116, 'z'}});
  const uint32_t size = guest::LoadU32(base, kSmokeEmitterSize);
  guest::StoreU32(base, emitter + 112, size);
  guest::StoreU32(base, emitter + 224, size);
  ApplyStores(base, emitter, {{228, 'z'}});
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, emitter + 240 + 48 * i, {{0, 'v'}, {16, 'v'}, {32, '0'}, {36, '0'}, {40, '0'}});
  }
}

void SmokeEmitterConstruct(uint8_t* base, uint32_t emitter) {
  ITC_NATIVE_RUNNING("sub_8212B230 SmokeEmitter::SmokeEmitter");
  ApplyStores(base, emitter, {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}, {156, 'o'},
                              {172, 'o'}, {188, 'o'}, {204, 'o'}, {220, 'o'}});
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, emitter + 240 + 48 * i, {{12, 'o'}, {28, 'o'}});
  }
}

void ParticlesBResetEntry(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8212B3E8 ParticlesB::ResetEntry");
  ApplyStores(base, entry,
              {{48, 'v'}, {64, 'v'}, {32, 'v'}, {0, 'v'}, {16, 'v'}, {80, '0'}, {84, '0'},
               {88, '0'}, {92, '1'}, {104, 'z'}, {100, 'z'}, {108, '0'}, {112, 'z'}, {96, '0'},
               {116, '1'}, {128, 'v'}, {144, 'v'}, {160, '0'}, {164, '0'}});
}

void ParticlesCReset(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212C0E8 ParticlesC::Reset");
  ApplyStores(base, particles, {{2704, 'z'}, {2688, '0'}, {2692, '0'}, {2696, '0'}, {2672, 'v'}});
  for (uint32_t i = 0; i < guest::LoadU32(base, particles + 2700); i++) {
    ApplyStores(base, particles + 112 + 128 * i,
                {{48, 'v'}, {64, 'v'}, {32, 'v'}, {0, 'v'}, {16, 'v'}, {80, '0'}, {84, '0'},
                 {88, '0'}, {92, '1'}, {100, 'z'}, {104, 'z'}, {108, '0'}, {96, '0'},
                 {112, '0'}});
  }
}

void MarkerReset(uint8_t* base, uint32_t marker) {
  ITC_NATIVE_RUNNING("sub_8212CC38 Marker::Reset");
  ApplyStores(base, marker, {{272, 'v'}, {288, '1'}});
  const uint32_t inner = marker + 128;
  ClearSpriteVectors(base, inner);
  ApplyStores(base, inner, {{96, '0'}, {100, 'z'}, {108, '0'}, {104, 'z'}, {112, '0'}});
}

void SparksReset(uint8_t* base, uint32_t sparks) {
  ITC_NATIVE_RUNNING("sub_8212D6E0 Sparks::Reset");
  ApplyStores(base, sparks,
              {{28932, '0'}, {28936, '0'}, {28940, '0'}, {28928, '0'}, {28960, 'v'},
               {28992, 'v'}, {29008, 'v'}, {28976, '0'}});
  for (uint32_t i = 0; i < 50; i++) {
    ApplyStores(base, sparks + 29024 + 16 * i, {{0, 'v'}});
  }
  for (uint32_t i = 0; i < guest::LoadU32(base, sparks + 104); i++) {
    ApplyStores(base, sparks + 128 + 144 * i,
                {{48, 'v'}, {64, 'v'}, {32, 'v'}, {0, 'v'}, {16, 'v'}, {112, 'v'}, {80, '0'},
                 {84, '0'}, {88, '0'}, {92, '1'}, {100, 'z'}, {104, 'z'}, {108, '0'}, {136, 'z'},
                 {128, '0'}, {96, '0'}, {132, '0'}});
  }
}

}  // namespace race_overlays

namespace race_overlays {

uint32_t SmokeInRegion(uint8_t* base, uint32_t smoke, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_8212A868 Smoke::InRegion");
  const double ax = guest::LoadFloat(base, smoke + 32592);
  const double ay = guest::LoadFloat(base, smoke + 32596);
  const double az = guest::LoadFloat(base, smoke + 32600);
  const double bx = guest::LoadFloat(base, smoke + 32608);
  const double by = guest::LoadFloat(base, smoke + 32612);
  const double bz = guest::LoadFloat(base, smoke + 32616);
  const double px = guest::LoadFloat(base, point + 0);
  const double py = guest::LoadFloat(base, point + 4);
  const double pz = guest::LoadFloat(base, point + 8);
  const uint32_t one = guest::LoadU32(base, kOne);
  auto bits = [](float f) { return std::bit_cast<uint32_t>(f); };
  const simde__m128 a_b = VectorOf(bits(Difference(ax, bx)), bits(Difference(ay, by)),
                                   bits(Difference(az, bz)), one);
  const simde__m128 p_a = VectorOf(bits(Difference(px, ax)), bits(Difference(py, ay)),
                                   bits(Difference(pz, az)), one);
  const simde__m128 b_a = VectorOf(bits(Difference(bx, ax)), bits(Difference(by, ay)),
                                   bits(Difference(bz, az)), one);
  const simde__m128 p_b = VectorOf(bits(Difference(px, bx)), bits(Difference(py, by)),
                                   bits(Difference(pz, bz)), one);
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  const simde__m128 past_a = simde_mm_dp_ps(p_a, a_b, 0xEF);
  const simde__m128 past_b = simde_mm_dp_ps(p_b, b_a, 0xEF);
  Fpscr::setcsr(csr);
  const double zero = guest::LoadFloat(base, kZero);
  if (!(double(std::bit_cast<float>(WordOf(past_a, 0))) < zero)) {
    return 0;
  }
  return double(std::bit_cast<float>(WordOf(past_b, 0))) < zero ? 1 : 0;
}

namespace {

// fadds or fsubs of two singles (`add` or not), rounded to single, with the
// first operand's NaN (quieted) where both are NaNs.
uint32_t KeepFirstNaN(uint32_t a, uint32_t b, bool add) {
  const float x = std::bit_cast<float>(a);
  const float y = std::bit_cast<float>(b);
  if (std::isnan(x)) {
    return a | 0x00400000;
  }
  if (std::isnan(y)) {
    return b | 0x00400000;
  }
  return std::bit_cast<uint32_t>(add ? float(double(x) + double(y)) : float(double(x) - double(y)));
}

}  // namespace

bool SparksUpdate(uint8_t* base, uint32_t sparks) {
  ITC_NATIVE_RUNNING("sub_8212DDC8 Sparks::Update");
  constexpr uint32_t kHead = 28932;
  constexpr uint32_t kCount = 28940;
  const int32_t count = int32_t(guest::LoadU32(base, sparks + kCount));
  if (count <= 0) {
    return true;
  }
  const int32_t capacity = int32_t(guest::LoadU32(base, sparks + 104));
  const int32_t head = int32_t(guest::LoadU32(base, sparks + kHead));
  if (capacity == 0 || capacity == -1 || head < 0 || head == INT32_MAX) {
    return false;
  }
  // The entries the loop reaches stay below 4 GB (no address wraps onto
  // the capacity, which the loop reads again).
  const uint64_t last_index =
      std::max<uint64_t>(uint64_t(head), uint64_t(std::abs(int64_t(capacity))));
  if (uint64_t(sparks) + 128 + 144 * (last_index + 1) > (uint64_t(1) << 32)) {
    return false;
  }
  const uint32_t table = guest::LoadU32(base, sparks + 112);
  const double zero = guest::LoadFloat(base, kZero);
  const double one = guest::LoadFloat(base, kOne);
  const uint32_t zero_word = guest::LoadU32(base, kZero);
  int32_t index = head;
  for (int32_t left = count; left > 0; left--) {
    const uint32_t entry = sparks + 128 + 144 * uint32_t(index);
    const int32_t life = int32_t(guest::LoadU32(base, entry + 108) - 1);
    guest::StoreU32(base, entry + 108, uint32_t(life));
    if (life < 0) {
      guest::StoreU32(base, entry + 108, 0);
    }
    if (!(int32_t(guest::LoadU32(base, entry + 108)) > 0)) {
      guest::StoreU32(base, entry + 96, 0);
      const int32_t next = int32_t(guest::LoadU32(base, sparks + kHead) + 1);
      const int32_t modulo = int32_t(guest::LoadU32(base, sparks + 104));
      const uint32_t remaining = guest::LoadU32(base, sparks + kCount) - 1;
      guest::StoreU32(base, sparks + kHead, uint32_t(next % modulo));
      guest::StoreU32(base, sparks + kCount, remaining);
    }
    const uint32_t fade_ticks = guest::LoadU32(base, table + 12);
    uint32_t brightness_word = guest::LoadU32(base, table + 8);
    const uint32_t now = guest::LoadU32(base, entry + 108);
    if (fade_ticks > now) {
      const float ticks = float(double(fade_ticks));
      const float life_now = float(double(int32_t(now)));
      const float step = float(one / double(ticks));
      const float scaled = float(double(step) * double(life_now));
      brightness_word = std::bit_cast<uint32_t>(
          float(double(scaled) * double(guest::LoadFloat(base, table + 8))));
    }
    // The original keeps this one as a double (it may be the product), so
    // a signaling NaN from the table comes back quieted.
    if (std::isnan(std::bit_cast<float>(brightness_word))) {
      brightness_word |= 0x00400000;
    }
    guest::StoreU32(base, entry + 104, brightness_word);
    if (double(std::bit_cast<float>(brightness_word)) < zero) {
      guest::StoreU32(base, entry + 104, zero_word);
    }
    const uint32_t most = guest::LoadU32(base, table + 8);
    if (double(std::bit_cast<float>(most)) < double(guest::LoadFloat(base, entry + 104))) {
      guest::StoreU32(base, entry + 104, most);
    }
    guest::StoreU32(base, entry + 92,
                    uint32_t(Fctiwz(double(guest::LoadFloat(base, entry + 104)))));
    // Where both operands are NaNs, the translated code (subss and addss
    // with the position as the destination, seen in the disassembly) keeps
    // the position's, quieted.
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(base, entry + 112 + 4 * i,
                      KeepFirstNaN(guest::LoadU32(base, entry + 48 + 4 * i),
                                   guest::LoadU32(base, entry + 64 + 4 * i), false));
    }
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(base, entry + 48 + 4 * i,
                      KeepFirstNaN(guest::LoadU32(base, entry + 48 + 4 * i),
                                   guest::LoadU32(base, entry + 32 + 4 * i), true));
    }
    index = (index + 1) % int32_t(guest::LoadU32(base, sparks + 104));
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// Overlay::Overlay, then the class's vtable.
void ConstructOverlay(uint8_t* base, uint32_t overlay, uint32_t vtable) {
  OverlayConstruct(base, overlay);
  guest::StoreU32(base, overlay, vtable);
}

}  // namespace

void LightSpriteConstruct(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_8211F060 LightSprite::LightSprite");
  ConstructOverlay(base, sprite, 0x82008444);
  ApplyStores(base, sprite, {{156, 'o'}, {172, 'o'}, {188, 'o'}, {204, 'o'}, {220, 'o'}, {284, 'o'}});
}

void ModelSpriteConstruct(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_82122880 ModelSprite::ModelSprite");
  ConstructOverlay(base, sprite, 0x820084E8);
  ApplyStores(base, sprite,
              {{236, 'o'}, {252, 'o'}, {268, 'o'}, {284, 'o'}, {300, 'o'}, {380, 'o'}, {396, 'o'}});
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, sprite + 428 + 16 * i, {{0, 'o'}});
  }
}

void TrailConstruct(uint8_t* base, uint32_t trail) {
  ITC_NATIVE_RUNNING("sub_8212A140 Trail::Trail");
  ConstructOverlay(base, trail, 0x820086C0);
  for (uint32_t i = 0; i < 500; i++) {
    ApplyStores(base, trail + 128 + 128 * i, {{12, 'o'}, {28, 'o'}, {76, 'o'}, {92, 'o'}, {108, 'o'}});
  }
  ApplyStores(base, trail, {{64156, 'o'}});
}

void SmokeConstruct(uint8_t* base, uint32_t smoke) {
  ITC_NATIVE_RUNNING("sub_8212B2E8 Smoke::Smoke");
  OverlayConstruct(base, smoke);
  guest::StoreU32(base, smoke + 116, 0);
  guest::StoreU32(base, smoke, 0x820086F4);
  for (uint32_t i = 0; i < 45; i++) {
    SmokeEmitterConstruct(base, smoke + 128 + 720 * i);
  }
  ApplyStores(base, smoke, {{32556, 'o'}, {32572, 'o'}, {32588, 'o'}, {32604, 'o'}, {32620, 'o'}});
}

void ParticlesCConstruct(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212CAF8 ParticlesC::ParticlesC");
  ConstructOverlay(base, particles, 0x82008750);
  for (uint32_t i = 0; i < 20; i++) {
    ApplyStores(base, particles + 112 + 128 * i,
                {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}});
  }
  ApplyStores(base, particles, {{2684, 'o'}});
}

void MarkerConstruct(uint8_t* base, uint32_t marker) {
  ITC_NATIVE_RUNNING("sub_8212D620 Marker::Marker");
  ConstructOverlay(base, marker, 0x82008784);
  ApplyStores(base, marker, {{140, 'o'}, {156, 'o'}, {172, 'o'}, {188, 'o'}, {204, 'o'}, {284, 'o'}});
}

void SparksConstruct(uint8_t* base, uint32_t sparks) {
  ITC_NATIVE_RUNNING("sub_8212E9A0 Sparks::Sparks");
  ConstructOverlay(base, sparks, 0x820087BC);
  for (uint32_t i = 0; i < 200; i++) {
    ApplyStores(base, sparks + 128 + 144 * i,
                {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}, {124, 'o'}});
  }
  ApplyStores(base, sparks, {{28972, 'o'}, {29004, 'o'}, {29020, 'o'}});
  for (uint32_t i = 0; i < 50; i++) {
    ApplyStores(base, sparks + 29036 + 16 * i, {{0, 'o'}});
  }
}

void SmokeReset(uint8_t* base, uint32_t smoke) {
  ITC_NATIVE_RUNNING("sub_8212A9E0 Smoke::Reset");
  ApplyStores(base, smoke, {{32528, '0'}, {32532, '0'}, {32536, '0'}, {32544, 'v'}, {32576, 'v'},
                            {32560, 'v'}});
  for (uint32_t i = 0; i < guest::LoadU32(base, smoke + 104); i++) {
    SmokeEmitterReset(base, smoke + 128 + 720 * i);
  }
}

void ParticlesBReset(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212BA98 ParticlesB::Reset");
  ApplyStores(base, particles, {{2768, '0'}, {2772, '0'}, {2776, '0'}, {2784, 'v'}, {2800, 'v'}});
  for (uint32_t i = 0; i < guest::LoadU32(base, particles + 104); i++) {
    ParticlesBResetEntry(base, particles + 128 + 176 * i);
  }
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSmokeOldest = 32528;  // the oldest emitter (Smoke::Emit adds at +32,532)
constexpr uint32_t kSmokeRegionOn = 32624;

// The index after `index` modulo `capacity` (divw, then the remainder, as
// the originals do it).
int32_t NextIndex(int32_t index, int32_t capacity) {
  const int32_t next = int32_t(uint32_t(index) + 1);
  return int32_t(uint32_t(next) - uint32_t(next / capacity) * uint32_t(capacity));
}

// Whether an update loop over 'capacity' entries of `bytes` from `first`,
// starting at `head`, stays below 4 GB, and the original can't trap (a
// capacity of 0 or -1, a head below 0 or INT_MAX).
bool EntriesInRange(uint32_t first, uint32_t bytes, int32_t capacity, int32_t head) {
  if (capacity == 0 || capacity == -1 || head < 0 || head == INT32_MAX) {
    return false;
  }
  const uint64_t last_index =
      std::max<uint64_t>(uint64_t(head), uint64_t(std::abs(int64_t(capacity))));
  return uint64_t(first) + uint64_t(bytes) * (last_index + 1) <= (uint64_t(1) << 32);
}

// int to float as the originals do it (stored as a doubleword, fcfid, frsp).
float FloatOf(int64_t value) {
  return float(double(value));
}

}  // namespace

bool SmokeUpdate(uint8_t* base, uint32_t smoke, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8212AA78 Smoke::Update");
  const uint32_t table = guest::LoadU32(base, smoke + 112);
  int32_t count = int32_t(guest::LoadU32(base, smoke + kSmokeCount));
  if (count == 0) {
    return true;
  }
  int32_t index = int32_t(guest::LoadU32(base, smoke + kSmokeOldest));
  if (!EntriesInRange(smoke + 128, kSmokeBytes,
                      int32_t(guest::LoadU32(base, smoke + 104)), index)) {
    return false;
  }
  auto emitter_at = [&](int32_t i) { return smoke + 128 + kSmokeBytes * uint32_t(i); };
  uint32_t emitter = emitter_at(index);
  // The oldest emitter, when its life is out, is reset and dropped (the
  // head and count read again after the reset).
  if (!(int32_t(guest::LoadU32(base, emitter + 108)) > 0)) {
    SmokeEmitterReset(base, emitter);
    const int32_t head = int32_t(guest::LoadU32(base, smoke + kSmokeOldest));
    const int32_t capacity = int32_t(guest::LoadU32(base, smoke + 104));
    count = int32_t(guest::LoadU32(base, smoke + kSmokeCount) - 1);
    guest::StoreU32(base, smoke + kSmokeCount, uint32_t(count));
    index = NextIndex(head, capacity);
    guest::StoreU32(base, smoke + kSmokeOldest, uint32_t(index));
    if (count != 0) {
      emitter = emitter_at(index);
    }
  }
  if (!(count > 0)) {
    return true;
  }
  const double zero = guest::LoadFloat(base, kZero);
  const double one = guest::LoadFloat(base, kOne);
  const uint32_t zero_word = guest::LoadU32(base, kZero);
  for (; count > 0; count--) {
    const int32_t life_left = int32_t(guest::LoadU32(base, emitter + 108) - 1);
    guest::StoreU32(base, emitter + 108, uint32_t(life_left));
    if (life_left < 0) {
      guest::StoreU32(base, emitter + 108, 0);
    }
    const uint32_t life = guest::LoadU32(base, emitter + 108);
    if (guest::LoadU32(base, table + 0) - life > 1) {
      guest::StoreU32(base, emitter + 96, 1);
    }
    // The size: +124 plus +116 x +124 x (the table's life - life) / the
    // table's life.
    const uint32_t most = guest::LoadU32(base, table + 0);
    const double size_from = guest::LoadFloat(base, emitter + 124);
    const double growth = guest::LoadFloat(base, emitter + 116);
    double size = float(growth * size_from);
    size = float(size * double(FloatOf(int64_t(uint32_t(most - life)))));
    size = float(size / double(FloatOf(int64_t(most))));
    guest::StoreFloat(base, emitter + 100, float(size + size_from));
    // The fade: +120 times life / the table's +12 while life is below it.
    const uint32_t fade_ticks = guest::LoadU32(base, table + 12);
    const uint32_t most_fade_word = guest::LoadU32(base, emitter + 120);
    const double most_fade = std::bit_cast<float>(most_fade_word);
    if (fade_ticks > life) {
      double fade = float(one / double(FloatOf(int64_t(fade_ticks))));
      fade = float(fade * double(FloatOf(int64_t(int32_t(life)))));
      guest::StoreFloat(base, emitter + 104, float(fade * most_fade));
    } else {
      guest::StoreU32(base, emitter + 104, most_fade_word);
    }
    if (double(guest::LoadFloat(base, emitter + 104)) < zero) {
      guest::StoreU32(base, emitter + 104, zero_word);
    }
    if (most_fade < double(guest::LoadFloat(base, emitter + 104))) {
      guest::StoreU32(base, emitter + 104, most_fade_word);
    }
    guest::StoreU32(base, emitter + 92,
                    uint32_t(Fctiwz(double(guest::LoadFloat(base, emitter + 104)))));
    // The particles drift by their velocity (+16) and show (+36 = 1),
    // unless the region is on and they're inside it.
    for (int32_t i = 0; i < int32_t(guest::LoadU32(base, emitter + 128)); i++) {
      const uint32_t particle = emitter + 240 + 48 * uint32_t(i);
      // Where both are NaNs, the translated code (addss, seen in the
      // disassembly) keeps the position's for x and the velocity's for y
      // and z.
      for (uint32_t k = 0; k < 3; k++) {
        const uint32_t velocity = guest::LoadU32(base, particle + 16 + 4 * k);
        const uint32_t position = guest::LoadU32(base, particle + 4 * k);
        guest::StoreU32(base, particle + 4 * k,
                        k == 0 ? KeepFirstNaN(position, velocity, true)
                               : KeepFirstNaN(velocity, position, true));
      }
      guest::StoreU32(base, particle + 40, 1);
      if (guest::LoadU32(base, smoke + kSmokeRegionOn) == 1) {
        *r3 = SmokeInRegion(base, smoke, particle);
        if (*r3 != 0) {
          guest::StoreU32(base, particle + 40, 0);
        }
      }
    }
    index = NextIndex(index, int32_t(guest::LoadU32(base, smoke + 104)));
    emitter = emitter_at(index);
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kParticlesBOldest = 2768;  // the oldest entry (ParticlesB::Emit adds at +2,772)
constexpr uint32_t kParticlesBCount = 2776;
constexpr uint32_t kParticlesBMoved = 2800;
constexpr uint32_t kParticlesBBytes = 176;
constexpr uint32_t kParticlesBMostSpeed = 0x82002A34;
constexpr uint32_t kParticlesBLeastSpeed = 0x82002A3C;

// A single from guest memory times `factor` (fmuls), the single's NaN kept,
// quieted, where both are NaNs (the translated code's operand order, seen in
// the disassembly).
uint32_t ScaledWord(uint8_t* base, uint32_t address, double factor) {
  const uint32_t word = guest::LoadU32(base, address);
  const double value = std::bit_cast<float>(word);
  if (std::isnan(value)) {
    return word | 0x00400000;
  }
  return std::bit_cast<uint32_t>(float(value * factor));
}

}  // namespace

bool ParticlesBUpdate(uint8_t* base, uint32_t effect) {
  ITC_NATIVE_RUNNING("sub_8212B748 ParticlesB::Update");
  const uint32_t table = guest::LoadU32(base, effect + 112);
  int32_t count = int32_t(guest::LoadU32(base, effect + kParticlesBCount));
  if (count == 0) {
    return true;
  }
  int32_t index = int32_t(guest::LoadU32(base, effect + kParticlesBOldest));
  if (!EntriesInRange(effect + 128, kParticlesBBytes,
                      int32_t(guest::LoadU32(base, effect + 104)), index)) {
    return false;
  }
  auto entry_at = [&](int32_t i) { return effect + 128 + kParticlesBBytes * uint32_t(i); };
  uint32_t entry = entry_at(index);
  // The oldest entry, when its life is out, is reset and dropped (the count
  // and head read again after the reset).
  if (!(int32_t(guest::LoadU32(base, entry + 108)) > 0)) {
    ParticlesBResetEntry(base, entry);
    count = int32_t(guest::LoadU32(base, effect + kParticlesBCount) - 1);
    const int32_t head = int32_t(guest::LoadU32(base, effect + kParticlesBOldest));
    const int32_t capacity = int32_t(guest::LoadU32(base, effect + 104));
    guest::StoreU32(base, effect + kParticlesBCount, uint32_t(count));
    index = NextIndex(head, capacity);
    guest::StoreU32(base, effect + kParticlesBOldest, uint32_t(index));
    if (count != 0) {
      entry = entry_at(index);
    }
  }
  if (!(count > 0)) {
    return true;
  }
  const double most_speed = guest::LoadFloat(base, kParticlesBMostSpeed);
  const double least_speed = guest::LoadFloat(base, kParticlesBLeastSpeed);
  const double zero = guest::LoadFloat(base, kZero);
  const double one = guest::LoadFloat(base, kOne);
  const uint32_t zero_word = guest::LoadU32(base, kZero);
  const uint32_t one_word = std::bit_cast<uint32_t>(float(one));
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  for (; count > 0; count--) {
    const int32_t life_left = int32_t(guest::LoadU32(base, entry + 108) - 1);
    guest::StoreU32(base, entry + 108, uint32_t(life_left));
    if (life_left < 0) {
      guest::StoreU32(base, entry + 108, 0);
    }
    // The size: the table's +4 plus (the table's life - life) / (twice the
    // table's life).
    const uint32_t most = guest::LoadU32(base, table + 0);
    const double size_from = guest::LoadFloat(base, table + 4);
    const uint32_t life = guest::LoadU32(base, entry + 108);
    double size = float(double(FloatOf(int64_t(uint32_t(most - life)))) /
                        double(FloatOf(int64_t(uint32_t(most << 1)))));
    guest::StoreFloat(base, entry + 100, float(size + size_from));
    // The fade: +112 times life / the table's +12 while life is below it.
    const uint32_t fade_ticks = guest::LoadU32(base, table + 12);
    const uint32_t most_fade_word = guest::LoadU32(base, entry + 112);
    const double most_fade = std::bit_cast<float>(most_fade_word);
    if (fade_ticks > life) {
      double fade = float(one / double(FloatOf(int64_t(fade_ticks))));
      fade = float(fade * double(FloatOf(int64_t(int32_t(life)))));
      guest::StoreFloat(base, entry + 104, float(fade * most_fade));
    } else {
      guest::StoreU32(base, entry + 104, most_fade_word);
    }
    if (double(guest::LoadFloat(base, entry + 104)) < zero) {
      guest::StoreU32(base, entry + 104, zero_word);
    }
    if (most_fade < double(guest::LoadFloat(base, entry + 104))) {
      guest::StoreU32(base, entry + 104, most_fade_word);
    }
    guest::StoreU32(base, entry + 92,
                    uint32_t(Fctiwz(double(guest::LoadFloat(base, entry + 104)))));
    // Each particle's velocity (+16) is the effect's +2,800 scaled by the
    // table's +4 times its length plus a least, at most a most; then it
    // moves by it.
    for (int32_t i = 0; i < int32_t(guest::LoadU32(base, entry + 116)); i++) {
      const uint32_t particle = entry + 128 + 48 * uint32_t(i);
      const simde__m128 moved = LoadVector(base, effect + kParticlesBMoved);
      Fpscr::setcsr(flushing);
      const simde__m128 length = Length(moved);
      Fpscr::setcsr(csr);
      const double scale = guest::LoadFloat(base, table + 4);
      double speed = float(std::fma(scale, double(std::bit_cast<float>(WordOf(length, 0))),
                                    least_speed));
      if (speed > most_speed) {
        speed = most_speed;
      }
      const uint32_t vx = ScaledWord(base, effect + kParticlesBMoved + 0, speed);
      const uint32_t vy = ScaledWord(base, effect + kParticlesBMoved + 4, speed);
      const uint32_t vz = ScaledWord(base, effect + kParticlesBMoved + 8, speed);
      guest::StoreU32(base, particle + 16, vx);
      guest::StoreU32(base, particle + 20, vy);
      guest::StoreU32(base, particle + 24, vz);
      guest::StoreU32(base, particle + 28, one_word);
      // Where both are NaNs, the translated code (addss, seen in the
      // disassembly) keeps the velocity's.
      for (uint32_t k = 0; k < 3; k++) {
        guest::StoreU32(base, particle + 4 * k,
                        KeepFirstNaN(guest::LoadU32(base, particle + 16 + 4 * k),
                                     guest::LoadU32(base, particle + 4 * k), true));
      }
    }
    index = NextIndex(index, int32_t(guest::LoadU32(base, effect + 104)));
    entry = entry_at(index);
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kParticlesCPosition = 2672;
constexpr uint32_t kParticlesCOldest = 2688;
constexpr uint32_t kParticlesCNewest = 2692;
constexpr uint32_t kParticlesCCount = 2696;
constexpr uint32_t kParticlesCCapacity = 2700;
constexpr uint32_t kParticlesCSpeed = 2704;
constexpr uint32_t kParticlesCSpeedScale = 0x82002BF0;
constexpr uint32_t kParticlesCPullScale = 0x8200874C;

}  // namespace

bool ParticlesCUpdate(uint8_t* base, uint32_t effect, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8212C2F0 ParticlesC::Update");
  const int32_t count = int32_t(guest::LoadU32(base, effect + kParticlesCCount));
  const uint32_t table = guest::LoadU32(base, effect + 108);
  if (count == 0) {
    return true;
  }
  // The capacity is read again for each division, but nothing here writes
  // it (the entries' fields stay off the effect's own).
  if (guest::LoadU32(base, effect + kParticlesCCapacity) == 0) {
    return false;
  }
  auto entry_at = [&](uint32_t i) { return effect + 112 + (i << 7); };
  const double zero = guest::LoadFloat(base, kZero);
  const double one = guest::LoadFloat(base, kOne);
  const uint32_t zero_word = guest::LoadU32(base, kZero);
  const uint32_t one_word = std::bit_cast<uint32_t>(float(one));

  // Each entry from the oldest ages and fades, and its brightness gets a
  // random 0 to 2 added.
  uint32_t index = guest::LoadU32(base, effect + kParticlesCOldest);
  if (count > 0) {
    const double speed_scale = guest::LoadFloat(base, kParticlesCSpeedScale);
    for (int32_t left = count; left > 0; left--) {
      const uint32_t entry = entry_at(index);
      const int32_t life_left = int32_t(guest::LoadU32(base, entry + 108) - 1);
      guest::StoreU32(base, entry + 108, uint32_t(life_left));
      if (life_left < 0) {
        guest::StoreU32(base, entry + 108, 0);
      }
      if (!(int32_t(guest::LoadU32(base, entry + 108)) > 0)) {
        guest::StoreU32(base, entry + 96, 0);
        const uint32_t next = guest::LoadU32(base, effect + kParticlesCOldest) + 1;
        const uint32_t capacity = guest::LoadU32(base, effect + kParticlesCCapacity);
        const uint32_t remaining = guest::LoadU32(base, effect + kParticlesCCount) - 1;
        guest::StoreU32(base, effect + kParticlesCCount, remaining);
        guest::StoreU32(base, effect + kParticlesCOldest, next % capacity);
      }
      const uint32_t fade_ticks = guest::LoadU32(base, table + 12);
      const uint32_t brightest_word = guest::LoadU32(base, table + 8);
      double brightness = std::bit_cast<float>(brightest_word);
      const uint32_t life = guest::LoadU32(base, entry + 108);
      uint32_t brightness_word = brightest_word;
      if (fade_ticks > life) {
        double fade = float(one / double(FloatOf(int64_t(fade_ticks))));
        fade = float(fade * double(FloatOf(int64_t(int32_t(life)))));
        brightness = float(fade * brightness);
        brightness_word = std::bit_cast<uint32_t>(float(brightness));
      }
      guest::StoreU32(base, entry + 104, brightness_word);
      if (brightness < zero) {
        guest::StoreU32(base, entry + 104, zero_word);
      }
      const uint32_t most_word = guest::LoadU32(base, table + 8);
      if (double(std::bit_cast<float>(most_word)) < double(guest::LoadFloat(base, entry + 104))) {
        guest::StoreU32(base, entry + 104, most_word);
      }
      double speed = float(double(guest::LoadFloat(base, effect + kParticlesCSpeed)) * speed_scale);
      if (speed > one) {
        speed = one;
      }
      guest::StoreFloat(base, entry + 104,
                        float(double(guest::LoadFloat(base, entry + 104)) * speed));
      const int32_t drawn = int32_t(Rand(base));
      *r3 = uint32_t(drawn);
      const double added = FloatOf(int64_t(drawn % 3));
      const float shown = float(added + double(guest::LoadFloat(base, entry + 104)));
      guest::StoreFloat(base, entry + 104, shown);
      guest::StoreU32(base, entry + 92, uint32_t(Fctiwz(double(shown))));
      index = (index + 1) % guest::LoadU32(base, effect + kParticlesCCapacity);
    }
  }

  // The newest entry takes the effect's position (+2,672), and each entry
  // before it, from the newest back, is pulled to within the effect's speed
  // (+2,704) times the table's +4 times a scale of the one after it.
  const int32_t pulls = int32_t(guest::LoadU32(base, effect + kParticlesCCount) - 1);
  uint32_t capacity = guest::LoadU32(base, effect + kParticlesCCapacity);
  uint32_t after = (guest::LoadU32(base, effect + kParticlesCNewest) + capacity - 1) % capacity;
  uint32_t before = (capacity + after - 1) % capacity;
  {
    const uint32_t to = entry_at(after) + 48;
    guest::StoreU32(base, to + 0, guest::LoadU32(base, effect + kParticlesCPosition + 0));
    guest::StoreU32(base, to + 4, guest::LoadU32(base, effect + kParticlesCPosition + 4));
    guest::StoreU32(base, to + 8, guest::LoadU32(base, effect + kParticlesCPosition + 8));
    guest::StoreU32(base, to + 12, guest::LoadU32(base, effect + kParticlesCPosition + 12));
  }
  if (!(pulls > 0)) {
    return true;
  }
  const double pull_scale = guest::LoadFloat(base, kParticlesCPullScale);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  for (int32_t left = pulls; left > 0; left--) {
    const uint32_t a = entry_at(after) + 48;
    const uint32_t b = entry_at(before) + 48;
    const double ay = guest::LoadFloat(base, a + 4);
    const float dx = Difference(guest::LoadFloat(base, a + 0), guest::LoadFloat(base, b + 0));
    const float dy = Difference(ay, guest::LoadFloat(base, b + 4));
    const float dz = Difference(guest::LoadFloat(base, a + 8), guest::LoadFloat(base, b + 8));
    auto bits = [](float f) { return std::bit_cast<uint32_t>(f); };
    const simde__m128 apart = VectorOf(bits(dx), bits(dy), bits(dz), one_word);
    Fpscr::setcsr(flushing);
    const simde__m128 length = Length(apart);
    Fpscr::setcsr(csr);
    const double distance = std::bit_cast<float>(WordOf(length, 0));
    if (!(distance == zero)) {
      double most = float(double(guest::LoadFloat(base, effect + kParticlesCSpeed)) * pull_scale);
      most = float(most * double(guest::LoadFloat(base, table + 4)));
      const double ax = guest::LoadFloat(base, a + 0);
      const double az = guest::LoadFloat(base, a + 8);
      const uint32_t ratio = bits(float(most / distance));
      const simde__m128 ratios = VectorOf(ratio, ratio, ratio, zero_word);
      // vmulfp with the difference's NaN kept where both are NaNs (the
      // translated code's mulps keeps its first operand's, seen in the
      // disassembly; the compiler may swap a plain mulps's operands).
      Fpscr::setcsr(flushing);
      const simde__m128 product = simde_mm_mul_ps(apart, ratios);
      Fpscr::setcsr(csr);
      const simde__m128 apart_nan = simde_mm_cmpunord_ps(apart, apart);
      const simde__m128 quieted =
          simde_mm_or_ps(apart, simde_mm_castsi128_ps(simde_mm_set1_epi32(0x00400000)));
      const simde__m128 back = simde_mm_blendv_ps(product, quieted, apart_nan);
      guest::StoreFloat(base, b + 0,
                        Difference(ax, double(std::bit_cast<float>(WordOf(back, 0)))));
      guest::StoreFloat(base, b + 4,
                        Difference(ay, double(std::bit_cast<float>(WordOf(back, 1)))));
      guest::StoreFloat(base, b + 8,
                        Difference(az, double(std::bit_cast<float>(WordOf(back, 2)))));
      guest::StoreU32(base, b + 12, one_word);
    }
    capacity = guest::LoadU32(base, effect + kParticlesCCapacity);
    after = (capacity + after - 1) % capacity;
    before = (capacity + after - 1) % capacity;
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

uint32_t SlotOf(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8215E048 RaceOverlays::SlotOf");
  return index <= 24 ? index : 0;
}

}  // namespace race_overlays

namespace race_overlays {

namespace va = vehicle_accessors;

namespace {

uint32_t Body(uint8_t* base, uint32_t view) {
  return guest::LoadU32(base, guest::LoadU32(base, view + 4) + 336);
}

}  // namespace

uint32_t BodyMatrix(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_821268B0 OverlayView::BodyMatrix");
  return va::RigidBodyMatrix(Body(base, view));
}

uint32_t WheelContact(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821268C8 OverlayView::WheelContact");
  return va::VehicleWheelContact(guest::LoadU32(base, view + 4), wheel) + 16;
}

uint32_t WheelSurfaceMarks(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821269B0 OverlayView::WheelSurfaceMarks");
  const uint32_t record = va::VehicleWheelContact(guest::LoadU32(base, view + 4), wheel);
  // lha, then cmplwi: a negative surface compares as a large one.
  const uint32_t surface = uint32_t(int32_t(int16_t(guest::LoadU16(base, record + 80))));
  if (surface == 0) {
    return 1;
  }
  return (surface <= 2 || surface == 8) ? 0 : 1;
}

float WheelSlip(uint8_t* base, uint32_t view, uint32_t wheel, double threshold) {
  ITC_NATIVE_RUNNING("sub_82126A70 OverlayView::WheelSlip");
  return va::VehicleWheelSlip(base, guest::LoadU32(base, view + 4), wheel, threshold);
}

uint32_t BodyPosition(uint8_t* base, uint32_t out, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126F80 OverlayView::BodyPosition");
  const uint32_t from = va::RigidBodyPosition(Body(base, view));
  uint32_t words[4];
  for (uint32_t i = 0; i < 4; i++) {
    words[i] = guest::LoadU32(base, from + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, out + 4 * i, words[i]);
  }
  return out;
}

uint32_t BodyMatrixCopy(uint8_t* base, uint32_t out, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126FF0 OverlayView::BodyMatrixCopy");
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t row = 0; row < 4; row++) {
    guest::StoreU32(base, out + 16 * row + 12, one);
  }
  // The original reads the matrix after those stores, into its stack, then
  // writes it out.
  const uint32_t from = va::RigidBodyMatrix(Body(base, view));
  uint32_t words[16];
  for (uint32_t i = 0; i < 16; i++) {
    words[i] = guest::LoadU32(base, from + 4 * i);
  }
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, out + 4 * i, words[i]);
  }
  return out;
}

uint32_t IsBehindCamera(uint8_t* base, uint32_t overlays, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_82124630 CarOverlays::IsBehindCamera");
  const uint32_t pass = guest::LoadU32(base, overlays + 800);
  const uint32_t view = View(base, overlays, guest::LoadU32(base, overlays + 128));
  const uint32_t matrix = BodyMatrix(base, view);
  const uint32_t one = guest::LoadU32(base, kOne);
  auto bits = [](float f) { return std::bit_cast<uint32_t>(f); };
  const double px = guest::LoadFloat(base, point + 0);
  const double py = guest::LoadFloat(base, point + 4);
  const double pz = guest::LoadFloat(base, point + 8);
  const simde__m128 from_eye =
      VectorOf(bits(Difference(px, guest::LoadFloat(base, pass + 400))),
               bits(Difference(py, guest::LoadFloat(base, pass + 404))),
               bits(Difference(pz, guest::LoadFloat(base, pass + 408))), one);
  const simde__m128 from_car =
      VectorOf(bits(Difference(px, guest::LoadFloat(base, matrix + 48))),
               bits(Difference(py, guest::LoadFloat(base, matrix + 52))),
               bits(Difference(pz, guest::LoadFloat(base, matrix + 56))), one);
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  const simde__m128 dot = simde_mm_dp_ps(from_car, from_eye, 0xEF);
  Fpscr::setcsr(csr);
  return double(std::bit_cast<float>(WordOf(dot, 0))) < double(guest::LoadFloat(base, kZero)) ? 0
                                                                                              : 1;
}

}  // namespace race_overlays
