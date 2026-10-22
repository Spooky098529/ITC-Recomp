#include "race_overlays/race_overlays.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
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
