#include "race_overlays/race_overlays.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "debug/frame_state_diff.h"
#include "hooks/vehicle.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/cameras.h"
#include "schema/car_models.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "schema/load_poll.h"
#include "schema/race_overlays.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/scene_models.h"
#include "schema/vehicle.h"
#include "schema/vehicle_accessors.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <algorithm>
#include <bit>
#include <climits>
#include <cmath>
#include <initializer_list>

namespace race_overlays {

using G = schema::RaceOverlaysGlobals;

namespace {

constexpr uint32_t kWheelBytes = schema::VehicleWheel::kSize;

// A word of the car overlays' (a host object since E5 step 4) at `at`, one
// of its fields' addresses (an interior handle): through the view, as the
// guest's bytes give it (its pointer arrays and rate accumulators are bytes
// fields in guest order).
uint32_t LoadWord(const schema::CarOverlays& o, uint32_t at) {
  return uint32_t(o.load_at(at - o.address, 4));
}
void StoreWord(const schema::CarOverlays& o, uint32_t at, uint32_t v) {
  o.store_at(at - o.address, 4, v);
}

uint32_t Vehicle(uint8_t* base, uint32_t view) {
  return schema::OverlayView(base, view).vehicle();
}

// The vehicle's car data (constructed in place at its +3376).
schema::CarData CarData(uint8_t* base, uint32_t view) {
  return schema::CarData(base, schema::Vehicle(base, Vehicle(base, view)).car_data_block_address());
}

// The vehicle's status (embedded at its +6160).
schema::VehicleStatus Status(uint8_t* base, uint32_t view) {
  return schema::VehicleStatus(base, schema::Vehicle(base, Vehicle(base, view)).status_address());
}

}  // namespace

uint32_t View(uint8_t* base, uint32_t overlays, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_82126550 CarOverlays::View");
  if (slot == 25) {
    return 0;
  }
  const schema::CarOverlays self(base, overlays);
  return LoadWord(self, self.views_address() + 4 * slot);
}

uint32_t ViewportEntry(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821264A8 CarOverlays::ViewportEntry");
  return G::viewport_entries_address() + 12 * index;
}

float SpeedKmh(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_821268C0 OverlayView::SpeedKmh");
  return Vehicle_GetSpeedKmh(base, Vehicle(base, view));
}

float WheelSpin(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821268F0 OverlayView::WheelSpin");
  return schema::VehicleWheel(base, schema::Vehicle(base, Vehicle(base, view)).wheels_address() + kWheelBytes * wheel).spin();
}

float WheelValue512(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126908 OverlayView::WheelValue512");
  return schema::VehicleWheel(base, schema::Vehicle(base, Vehicle(base, view)).wheels_address() + kWheelBytes * wheel)
      .ground_distance();
}

float AxleValue3764(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126920 OverlayView::AxleValue3764");
  if (wheel < 2) {
    return schema::CarDataAxle(base, CarData(base, view).front_axle_address()).radius();
  }
  if (wheel < 4) {
    return schema::CarDataAxle(base, CarData(base, view).rear_axle_address()).radius();
  }
  return G::zero(base);
}

float AxleValue3384(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126958 OverlayView::AxleValue3384");
  float value;
  if (wheel < 2) {
    value = CarData(base, view).front_track();
  } else if (wheel < 4) {
    value = CarData(base, view).rear_track();
  } else {
    value = G::zero(base);
  }
  // fmuls: the product of the two singles (as doubles), rounded to single.
  return float(double(value) * double(G::axle_scale(base)));
}

uint32_t WheelOnGround(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126998 OverlayView::WheelOnGround");
  return schema::VehicleWheel(base, schema::Vehicle(base, Vehicle(base, view)).wheels_address() + kWheelBytes * wheel)
      .on_ground();
}

uint32_t VehicleByte2504(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126A80 OverlayView::VehicleByte2504");
  return schema::Vehicle(base, Vehicle(base, view)).backfire();
}

uint32_t VehicleWord2252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AA0 OverlayView::VehicleWord2252");
  return uint32_t(schema::Vehicle(base, Vehicle(base, view)).gear());
}

uint32_t VehicleWord340(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AB0 OverlayView::VehicleWord340");
  return schema::Vehicle(base, Vehicle(base, view)).collision_object();
}

float VehicleFloat6256(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AC0 OverlayView::VehicleFloat6256");
  return Status(base, view).oil_temperature();
}

float VehicleFloat6252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AD0 OverlayView::VehicleFloat6252");
  return Status(base, view).water_temperature();
}

uint32_t RaceMode(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82126D18 RaceOverlays::RaceMode");
  return schema::RaceData(base, G::race_data(base)).race_mode();
}

uint32_t RacerFlags(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82127178 OverlayView::RacerFlags");
  const uint32_t racer = schema::Vehicle(base, Vehicle(base, view)).car();
  return schema::Car(base, racer).flags();
}

int32_t RacerId(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AE0 OverlayView::RacerId");
  const uint32_t records = schema::RaceData(base, G::race_data(base)).racer_records_address();
  const uint32_t racer = schema::Vehicle(base, Vehicle(base, view)).car();
  for (uint32_t i = 0; i < 93; i++) {
    const schema::Car record(base, records + schema::Car::kSize * i);
    if (record.address == racer) {
      return int32_t(record.item_id());
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
  CopyVector(base, vector, schema::Smoke(base, smoke).velocity_address());
}

void SmokeSetRegion(uint8_t* base, uint32_t smoke, uint32_t a, uint32_t b, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8212A998 Smoke::SetRegion");
  const schema::Smoke s(base, smoke);
  CopyVector(base, a, s.region_front_address());
  CopyVector(base, b, s.region_rear_address());
  s.set_region_on(mode);
}

void ParticlesBSetPosition(uint8_t* base, uint32_t effect, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212BA70 ParticlesB::SetPosition");
  CopyVector(base, position, schema::ParticlesB(base, effect).position_address());
}

void ParticlesCSetPosition(uint8_t* base, uint32_t effect, uint32_t position, double speed) {
  ITC_NATIVE_RUNNING("sub_8212C6A8 ParticlesC::SetPosition");
  const schema::ParticlesC c(base, effect);
  CopyVector(base, position, c.position_address());
  c.set_speed(float(speed));
}

void SparksSetPosition(uint8_t* base, uint32_t sparks, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212DF90 Sparks::SetPosition");
  CopyVector(base, position, schema::Sparks(base, sparks).position_address());
}

void TrailSetLastPosition(uint8_t* base, uint32_t trail, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_821299D8 Trail::SetLastPosition");
  CopyVector(base, position, schema::Trail(base, trail).last_position_address());
}

void MarkerSetCameraPosition(uint8_t* base, uint32_t marker, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212CE18 Marker::SetCameraPosition");
  CopyVector(base, position, schema::Marker(base, marker).camera_position_address());
}

void LightSpriteSetPosition(uint8_t* base, uint32_t sprite, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8211EBE0 LightSprite::SetPosition");
  const schema::OverlaySpriteEntry entry(base,
                                         schema::LightSprite(base, sprite).sprite_address());
  if (entry.shown() == 0) {
    return;
  }
  CopyVector(base, position, entry.position_address());
}

void ModelSpritePlaceHidden(uint8_t* base, uint32_t sprite, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82121560 ModelSprite::PlaceHidden");
  const schema::OverlaySpriteEntry entry(base,
                                         schema::ModelSprite(base, sprite).sprite_address());
  CopyVector(base, position, entry.position_address());
  entry.set_kind(0);
}

void SetTextures104(uint8_t* base, uint32_t overlay, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8212C0E0 Overlay::SetTextures104");
  schema::Overlay(base, overlay).set_textures(value);
}

void SetTextures108(uint8_t* base, uint32_t overlay, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8212D6D8 Overlay::SetTextures108");
  schema::Overlay(base, overlay).set_textures_108(value);
}

void MarkerSetTextures(uint8_t* base, uint32_t marker, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8212CC10 Marker::SetTextures");
  StoreEntries(base, schema::Marker(base, marker).textures_address(), table, 4);
}

void LightSpriteSetTextures(uint8_t* base, uint32_t sprite, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8211E9C8 LightSprite::SetTextures");
  StoreEntries(base, schema::LightSprite(base, sprite).textures_address(), table, 9);
}

void ModelSpriteSetTextures(uint8_t* base, uint32_t sprite, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8211F408 ModelSprite::SetTextures");
  StoreEntries(base, schema::ModelSprite(base, sprite).textures_address(), table, 25);

}

}  // namespace race_overlays

namespace race_overlays {

namespace {
}  // namespace

uint32_t Rand(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82293568 rand");
  const uint32_t seed =
      uint32_t(int64_t(int32_t(G::rand_seed(base))) * 1103515245 + 12345);
  G::set_rand_seed(base, seed);
  return (seed >> 16) & 0x7FFF;
}

float RandomRange(uint8_t* base, double low, double high, uint32_t* drawn) {
  ITC_NATIVE_RUNNING("sub_8211F118 RandomRange");
  const uint32_t r = Rand(base);
  if (drawn) {
    *drawn = r;
  }
  const double range = double(float(high - low));
  const double scale = double(G::rand_scale(base));
  const double value = double(float(double(int64_t(int32_t(r)))));
  const double scaled = double(float(range * value));
  return float(std::fma(scaled, scale, low));
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

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
  const schema::Marker m(base, marker);
  const schema::OverlaySpriteEntry sprite(base, m.sprite_address());
  const schema::OverlayParams table = m.params();
  CopyVector(base, position, sprite.position_address());
  const float zero = G::zero(base);
  m.set_view_mode(mode);
  m.set_facing(distance == double(zero) ? 1 : 0);
  if (m.facing() == 1) {
    CopyFloat(base, table.size_address(), sprite.size_address());
  } else {
    const double slope = G::marker_slope(base);
    const double most = G::marker_most_size(base);
    const double one = G::one(base);
    const double least = G::marker_least_size(base);
    // fnmsubs, then fmadds: one - distance x slope, times the most, plus
    // the least, each rounded to single.
    double size = double(float(-std::fma(distance, slope, -one)));
    size = double(float(std::fma(size, most, least)));
    sprite.set_size(float(size));
    if (size < least) {
      sprite.set_size(zero);
    }
    if (double(sprite.size()) > most) {
      sprite.set_size(float(most));
    }
  }
  sprite.set_kind(kind <= 3 ? kind : 0);
  if (sprite.shown() != 0) {
    return 1;
  }
  sprite.set_shown(1);
  const float alpha = table.alpha();
  sprite.set_alpha(alpha);
  const int32_t alpha_int = Fctiwz(double(alpha));
  sprite.set_life(table.life());
  sprite.set_red(255);
  sprite.set_green(255);
  sprite.set_blue(255);
  sprite.set_alpha_int(uint32_t(alpha_int));
  return 1;
}

void MarkerUpdate(uint8_t* base, uint32_t marker) {
  ITC_NATIVE_RUNNING("sub_8212CBE0 Marker::Update");
  const schema::Marker m(base, marker);
  const schema::OverlaySpriteEntry sprite(base, m.sprite_address());
  if (sprite.shown() == 0) {
    return;
  }
  const uint32_t age = sprite.life() + 1;
  sprite.set_life(age);
  if (int32_t(age) < 0) {
    sprite.set_life(0);
  }
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// The point `index` (128 bytes each, from +128).
schema::TrailPoint TrailPointAt(uint8_t* base, uint32_t trail, uint32_t index) {
  return schema::TrailPoint(base, schema::Trail(base, trail).points_address() + (index << 7));
}

// The index after `index`, modulo the capacity (divw then the remainder, as
// the original).
uint32_t TrailNext(int32_t index, int32_t capacity) {
  return uint32_t(index - (index / capacity) * capacity);
}

}  // namespace

bool TrailUpdate(uint8_t* base, uint32_t trail) {
  ITC_NATIVE_RUNNING("sub_821292E8 Trail::Update");
  const schema::Trail t(base, trail);
  const int32_t capacity = int32_t(t.capacity());
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const schema::OverlayParams table = t.params();
  int32_t left = int32_t(t.count());
  if (left == 0) {
    return true;
  }
  uint32_t index = t.oldest();
  schema::TrailPoint point = TrailPointAt(base, trail, index);
  if (int32_t(point.age()) <= 0) {
    point.set_state(0);
    point.set_shown(0);
    const int32_t start = int32_t(t.oldest() + 1);
    const uint32_t count = t.count() - 1;
    // The capacity again, as the original reloads it (no point's fields
    // reach +104, so it's the one checked above).
    const int32_t size = int32_t(t.capacity());
    t.set_count(count);
    index = TrailNext(start, size);
    t.set_oldest(index);
    left = int32_t(count);
    if (left != 0) {
      point = TrailPointAt(base, trail, index);
      point.set_state(1);
    }
  }
  if (left <= 0) {
    return true;
  }
  const double zero = G::zero(base);
  const double one = G::one(base);
  while (true) {
    int32_t age = int32_t(point.age() - 1);
    point.set_age(uint32_t(age));
    if (age < 0) {
      point.set_age(0);
    }
    const uint32_t now = point.age();
    if (table.life() - now > 1) {
      point.set_shown(1);
    }
    const uint32_t fade = table.fade_ticks();
    const float full = point.full_width();
    if (fade > now) {
      const double ticks = double(float(double(int64_t(fade))));
      const double aged = double(float(double(int64_t(int32_t(now)))));
      double width = double(float(one / ticks));
      width = double(float(width * aged));
      width = double(float(width * double(full)));
      point.set_width(float(width));
    } else {
      point.set_width(full);
    }
    if (double(point.width()) < zero) {
      point.set_width(float(zero));
    }
    if (double(full) < double(point.width())) {
      point.set_width(full);
    }
    point.set_width_int(uint32_t(Fctiwz(double(point.width()))));
    left--;
    const int32_t next = int32_t(index + 1);
    const int32_t size = int32_t(t.capacity());
    index = TrailNext(next, size);
    point = TrailPointAt(base, trail, index);
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
      table = G::model_sprite_kind1_address();
      break;
    case 2:
      table = G::model_sprite_kind2_address();
      break;
    case 3:
    case 4:
    case 5:
      table = G::model_sprite_kind345_address();
      break;
    case 6:
      table = G::model_sprite_kind6_address();
      break;
    case 7:
      table = G::model_sprite_kind7_address();
      break;
    case 8:
      table = G::model_sprite_kind8_address();
      break;
    default:
      table = G::model_sprite_other_address();
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
  return guest::LoadU64(base, address);
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU64(base, address, value);
}

// fsubs on two singles from guest memory: the difference of the two as
// doubles, rounded to single.
float Difference(double a, double b) {
  return float(a - b);
}

}  // namespace

uint32_t ParticlesCEmit(uint8_t* base, uint32_t effect, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8212C1C0 ParticlesC::Emit");
  const schema::ParticlesC c(base, effect);
  const uint32_t capacity = c.capacity();
  const uint32_t count = c.count();
  const schema::OverlayParams table = c.params();
  if (!(capacity > count)) {
    return 0;
  }
  const uint32_t one = guest::LoadU32(base, G::one_address());
  const schema::ParticlesCEntry entry(base, c.entries_address() + (c.newest() << 7));
  // The doublewords copied as the original's ld and std, in its order.
  StoreU64(base, entry.position_address(), LoadU64(base, position + 0));
  StoreU64(base, entry.position_address() + 8, LoadU64(base, position + 8));
  StoreU64(base, entry.origin_address(), LoadU64(base, c.position_address()));
  StoreU64(base, entry.origin_address() + 8, LoadU64(base, c.position_address() + 8));
  const float dx = Difference(guest::LoadFloat(base, position + 0),
                              guest::LoadFloat(base, c.position_address()));
  const float dy = Difference(guest::LoadFloat(base, position + 4),
                              guest::LoadFloat(base, c.position_address() + 4));
  const float dz = Difference(guest::LoadFloat(base, position + 8),
                              guest::LoadFloat(base, c.position_address() + 8));
  guest::StoreFloat(base, entry.velocity_address(), dx);
  guest::StoreFloat(base, entry.velocity_address() + 4, dy);
  guest::StoreFloat(base, entry.velocity_address() + 8, dz);
  guest::StoreU32(base, entry.velocity_address() + 12, one);
  entry.set_life(table.life());
  CopyFloat(base, table.size_address(), entry.size_address());
  const uint32_t alpha = guest::LoadU32(base, table.alpha_address());
  entry.set_unknown_112(0);
  guest::StoreU32(base, entry.alpha_address(), alpha);
  const int32_t alpha_int = Fctiwz(double(std::bit_cast<float>(alpha)));
  entry.set_shown(1);
  entry.set_green(0);
  entry.set_blue(0);
  entry.set_red(255);
  entry.set_alpha_int(uint32_t(alpha_int));
  // The head moves on (divwu, then the remainder) and the count goes up.
  const uint32_t next = c.newest() + 1;
  const uint32_t size = c.capacity();
  const uint32_t added = c.count() + 1;
  const uint32_t quotient = size ? next / size : 0;
  c.set_count(added);
  c.set_newest(next - quotient * size);
  return 1;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

using Fpscr = ::base::FloatMode;

// A guest vector as the translated code holds it (word 0 in the top lane),
// from four words.
simde__m128 VectorOf(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int32_t(w0), int32_t(w1), int32_t(w2), int32_t(w3)));
}

// The vector unit's flush-to-zero around vector work: the control word set
// to `flushing`, `work(in...)` run, `csr` put back. Compiler barriers
// (vector_library::PinInMode, as the vector library's since ee810a9) keep
// the inputs computed before the window and the work inside it: the
// compiler doesn't know the control word writes change how SSE rounds, and
// ThinLTO had moved Smoke::InRegion's second dpps past the restore
// (2026-10-24, found by its "tiny values" harness group).
template <typename Work, typename... In>
simde__m128 Flushing(uint32_t csr, uint32_t flushing, Work work, In... in) {
  vector_library::PinInMode(in...);
  Fpscr::setcsr(flushing);
  vector_library::PinInMode(in...);
  simde__m128 out = work(in...);
  vector_library::PinInMode(out);
  Fpscr::setcsr(csr);
  return out;
}

simde__m128 Mul(simde__m128 a, simde__m128 b) {
  return simde_mm_mul_ps(a, b);
}
simde__m128 Add(simde__m128 a, simde__m128 b) {
  return simde_mm_add_ps(a, b);
}
simde__m128 Sub(simde__m128 a, simde__m128 b) {
  return simde_mm_sub_ps(a, b);
}
simde__m128 Dot3(simde__m128 a, simde__m128 b) {
  return simde_mm_dp_ps(a, b, 0xEF);
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
// Sparks::Emit have theirs swapped (seen in the disassembly), so
// Sparks::UpdateStreak uses a plain addps, which matches it as built, and
// Sparks::Emit's step names the operands it keeps (MulKeepingFirstNaN, then
// this with the product first), since the flush-to-zero barriers changed
// the order the compiler picked for its plain mulps.
simde__m128 AddKeepingFirstNaN(simde__m128 a, simde__m128 b) {
  const simde__m128 sum = simde_mm_add_ps(a, b);
  const simde__m128 a_nan = simde_mm_cmpunord_ps(a, a);
  const simde__m128 quieted =
      simde_mm_or_ps(a, simde_mm_castsi128_ps(simde_mm_set1_epi32(0x00400000)));
  return simde_mm_blendv_ps(sum, quieted, a_nan);
}

// mulps with `a` as its first operand, whatever order the compiler picks:
// the product, except that lanes where `a` is a NaN get `a` quieted.
simde__m128 MulKeepingFirstNaN(simde__m128 a, simde__m128 b) {
  const simde__m128 product = simde_mm_mul_ps(a, b);
  const simde__m128 a_nan = simde_mm_cmpunord_ps(a, a);
  const simde__m128 quieted =
      simde_mm_or_ps(a, simde_mm_castsi128_ps(simde_mm_set1_epi32(0x00400000)));
  return simde_mm_blendv_ps(product, quieted, a_nan);
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
  const schema::Trail t(base, trail);
  const int32_t capacity = int32_t(t.capacity());
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  *result = 0;
  const schema::OverlayParams table = t.params();
  const int32_t count = int32_t(t.count());
  if (!(capacity > count) || state == 0) {
    return true;
  }
  const uint32_t last = t.last_position_address();
  const uint32_t x = guest::LoadU32(base, position + 0);
  const uint32_t one = guest::LoadU32(base, G::one_address());
  const uint32_t zero = guest::LoadU32(base, G::zero_address());
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
    const simde__m128 length = Flushing(csr, flushing, Length, moved);
    if (double(std::bit_cast<float>(WordOf(length, 0))) == double(std::bit_cast<float>(zero))) {
      return true;
    }
  }
  if (count == 0 && state == 2) {
    state = 1;
  }

  const schema::TrailPoint point = TrailPointAt(base, trail, t.tail());
  const float speed_scale = G::axle_scale(base);
  guest::StoreU32(base, point.position_address(), x);
  CopyFloat(base, position + 4, point.position_address() + 4);
  CopyFloat(base, position + 8, point.position_address() + 8);
  CopyFloat(base, position + 12, point.position_address() + 12);
  CopyVector(base, last, point.last_position_address());
  for (uint32_t i = 0; i < 12; i += 4) {
    const float d = Difference(guest::LoadFloat(base, position + i),
                               guest::LoadFloat(base, last + i));
    guest::StoreFloat(base, point.difference_address() + i, d);
  }
  point.set_side_width(float(width));
  point.set_state(state);
  // fmadds: speed times the scale plus the table's +8, capped.
  double size = double(float(
      std::fma(speed, double(speed_scale), double(table.alpha()))));
  const double most = G::trail_most_width(base);
  if (size > most) {
    size = most;
  }
  point.set_full_width(float(size));
  point.set_width(float(size));
  const uint32_t life = table.life();
  point.set_shown(value);
  const int32_t size_int = Fctiwz(size);
  point.set_age(life);
  point.set_texture_coordinate(0);
  const simde__m128 along = VectorOf(direction[0], direction[1], direction[2], direction[3]);
  point.set_red(255);
  point.set_green(255);
  point.set_blue(255);
  point.set_width_int(uint32_t(size_int));

  // The side: `direction` crossed with the difference (vperm-made shuffles,
  // vmulfp128, vnmsubfp), scaled to the width over its length.
  const simde__m128 difference = LoadVector(base, point.difference_address());
  const float stored_width = point.side_width();
  const float px = guest::LoadFloat(base, point.position_address());
  const simde__m128 sign = simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)));
  // In one window as Flushing's (two results): the barriers keep the
  // inputs before it and the work inside.
  simde__m128 side_in = difference, along_in = along;
  vector_library::PinInMode(side_in, along_in);
  Fpscr::setcsr(flushing);
  vector_library::PinInMode(side_in, along_in);
  simde__m128 side = simde_mm_mul_ps(
      simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(side_in), 0x9C)),
      simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(along_in), 0x78)));
  side = simde_mm_xor_ps(
      simde_mm_sub_ps(
          simde_mm_mul_ps(
              simde_mm_castsi128_ps(
                  simde_mm_shuffle_epi32(simde_mm_castps_si128(side_in), 0x78)),
              simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(along_in), 0x9C))),
          side),
      sign);
  simde__m128 side_length = Length(side);
  vector_library::PinInMode(side, side_length);
  Fpscr::setcsr(csr);
  const float scale = float(double(stored_width) / double(std::bit_cast<float>(WordOf(side_length, 0))));
  const uint32_t s = std::bit_cast<uint32_t>(scale);
  const simde__m128 scaled = Flushing(csr, flushing, Mul, side, VectorOf(s, s, s, zero));
  const double factor = G::half(base);
  const double sx = float(double(std::bit_cast<float>(WordOf(scaled, 0))) * factor);
  const double sy = float(double(std::bit_cast<float>(WordOf(scaled, 1))) * factor);
  const double sz = float(double(std::bit_cast<float>(WordOf(scaled, 2))) * factor);
  guest::StoreFloat(base, point.edge_plus_address(), float(double(px) + sx));
  guest::StoreFloat(base, point.edge_plus_address() + 4, float(sy + double(guest::LoadFloat(base, point.position_address() + 4))));
  guest::StoreFloat(base, point.edge_plus_address() + 8, float(sz + double(guest::LoadFloat(base, point.position_address() + 8))));
  guest::StoreFloat(base, point.edge_minus_address(), float(double(guest::LoadFloat(base, point.position_address())) - sx));
  guest::StoreFloat(base, point.edge_minus_address() + 4, float(double(guest::LoadFloat(base, point.position_address() + 4)) - sy));
  guest::StoreFloat(base, point.edge_minus_address() + 8, float(double(guest::LoadFloat(base, point.position_address() + 8)) - sz));

  // The tail moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(t.tail() + 1);
  const int32_t size_now = int32_t(t.capacity());
  const uint32_t added = t.count() + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  t.set_tail(uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  t.set_count(added);
  *result = 1;
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

bool ParticlesBEmit(uint8_t* base, uint32_t effect, uint32_t position, double a,
                    uint32_t* result, double* sine) {
  ITC_NATIVE_RUNNING("sub_8212B4A8 ParticlesB::Emit");
  const schema::ParticlesB b(base, effect);
  const int32_t capacity = int32_t(b.capacity());
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const int32_t count = int32_t(b.count());
  const schema::OverlayParams table = b.params();
  if (!(capacity > count)) {
    *result = 0;
    return true;
  }
  const schema::ParticlesBEntry entry(base, b.entries_address() + b.head() * 176);
  const uint32_t at = entry.position_address();
  const uint32_t moved = b.moved_address();
  const double scaled =
      float(a * double(G::particles_b_size_scale(base)));
  const uint32_t one = guest::LoadU32(base, G::one_address());
  entry.set_shown(1);
  entry.set_life(table.life());
  CopyFloat(base, table.size_address(), entry.size_address());
  CopyVector(base, position, at);
  CopyVector(base, b.position_address(), entry.origin_address());
  const float size = float(scaled * double(table.alpha()));
  entry.set_most_alpha(size);
  entry.set_alpha(size);
  entry.set_red(200);
  entry.set_green(200);
  const double distance_high = G::spread_high(base);
  entry.set_blue(200);
  const double distance_low = G::spread_low(base);
  entry.set_alpha_int(uint32_t(Fctiwz(double(size))));
  const float dx = Difference(guest::LoadFloat(base, at + 0), guest::LoadFloat(base, entry.origin_address()));
  const float dy = Difference(guest::LoadFloat(base, at + 4), guest::LoadFloat(base, entry.origin_address() + 4));
  const float dz = Difference(guest::LoadFloat(base, at + 8), guest::LoadFloat(base, entry.origin_address() + 8));
  guest::StoreFloat(base, moved + 0, dx);
  guest::StoreFloat(base, moved + 4, dy);
  guest::StoreFloat(base, moved + 8, dz);
  guest::StoreU32(base, moved + 12, one);
  entry.set_particle_count(1);
  const double angle_low = G::angle_low(base);
  const double angle_high = G::angle_high(base);
  const double spread = G::half(base);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);

  schema::OverlayParticle particle(base, entry.particle_address());
  for (int32_t i = 0; i < int32_t(entry.particle_count()); i++, particle.address += 48) {
    const double angle = RandomRange(base, angle_low, angle_high);
    const double distance = RandomRange(base, distance_low, distance_high);
    double x = float(double(float(c_runtime::Cos(angle))) * distance);
    x = float(x * spread);
    *sine = c_runtime::Sin(angle);
    const double sine_single = float(*sine);
    guest::StoreFloat(base, particle.position_address(), float(x + double(guest::LoadFloat(base, at + 0))));
    CopyFloat(base, at + 4, particle.position_address() + 4);
    const double z = guest::LoadFloat(base, at + 8);
    double offset = float(sine_single * distance);
    offset = float(offset * spread);
    guest::StoreFloat(base, particle.position_address() + 8, float(z + offset));
    // The particle's vector less the effect's +2,800 (vsubfp, both loaded
    // as lvx does).
    const simde__m128 less = LoadVector(base, moved);
    const simde__m128 from = LoadVector(base, particle.address);
    const simde__m128 difference = Flushing(csr, flushing, Sub, from, less);
    const uint32_t to = particle.address & ~0xFu;
    for (int k = 0; k < 4; k++) {
      guest::StoreU32(base, to + 4 * k, WordOf(difference, k));
    }
    CopyVector(base, moved, particle.velocity_address());
    const int32_t first = int32_t(Rand(base));
    particle.set_pick_a(uint32_t(first - (first / 3) * 3));
    const int32_t second = int32_t(Rand(base));
    particle.set_pick_b(uint32_t(second - (second / 4) * 4));
  }

  // The head moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(b.head() + 1);
  const int32_t size_now = int32_t(b.capacity());
  const uint32_t added = b.count() + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  b.set_head(uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  b.set_count(added);
  *result = 1;
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// Whether the scene model's car model, parts and their ready words are all
// set (+5048, +5052, +5060 and +5064: the original's checks, in its order).
bool ModelReady(uint8_t* base, uint32_t model) {
  const schema::SceneModel m(base, model);
  return m.car_model().address != 0 && m.car_model_ready() != 0 && m.parts().address != 0 &&
         m.parts_ready() != 0;
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
  return ModelPartsHasPoint(base, schema::SceneModel(base, model).parts().address, index);
}

uint32_t OverlayViewHasPoint(uint8_t* base, uint32_t view, int32_t index) {
  ITC_NATIVE_RUNNING("sub_82126A78 OverlayView::HasPoint");
  return ModelHasPoint(base, schema::OverlayView(base, view).model(), index);
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
    const uint32_t zero = guest::LoadU32(base, G::zero_address());
    guest::StoreU32(base, out + 12, zero);
    guest::StoreU32(base, out + 8, zero);
    guest::StoreU32(base, out + 4, zero);
    guest::StoreU32(base, out + 0, zero);
    return out;
  }
  if (index < 4) {
    const schema::CarModel points =
        schema::SceneModel(base, model).car_model().car_model();
    CopyDoublewords(base, points.tire_positions_address() + (uint32_t(index) << 4), out);
    return out;
  }
  return ModelPartsPointPosition(base, out, schema::SceneModel(base, model).parts().address, index);
}

uint32_t OverlayViewPointPosition(uint8_t* base, uint32_t out, uint32_t view, int32_t index) {
  ITC_NATIVE_RUNNING("sub_82127140 OverlayView::PointPosition");
  return ModelPointPosition(base, out, schema::OverlayView(base, view).model(), index);
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSparksBytes = 144;

// The 16 bytes at `to` (rounded down to 16) plus those at `from` (rounded
// down too), stored back at `to`: lvx, lvx, vaddfp with flush-to-zero on,
// stvx.
void AddVectorTo(uint8_t* base, uint32_t to, uint32_t from, uint32_t csr, uint32_t flushing) {
  const simde__m128 a = LoadVector(base, to);
  const simde__m128 b = LoadVector(base, from);
  const simde__m128 sum = Flushing(csr, flushing, Add, a, b);
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
  const schema::Sparks s(base, sparks);
  const uint64_t position_high = (uint64_t(position[0]) << 32) | position[1];
  const uint64_t position_low = (uint64_t(position[2]) << 32) | position[3];
  const uint32_t head = s.oldest();
  const uint32_t one = guest::LoadU32(base, G::one_address());
  double factor = std::bit_cast<float>(one);
  // fmuls, then clamped (a NaN stays).
  double spread = float(speed * double(G::speed_scale(base)));
  const double least = G::axle_scale(base);
  if (spread < least) {
    spread = least;
  } else {
    const double most = G::sparks_most_spread(base);
    if (spread > most) {
      spread = most;
    }
  }
  s.set_spread(float(spread));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, s.position_address() + 4 * i, position[i]);
  }
  const uint32_t counter = s.streak_counter();
  s.set_streaking(streaking);
  s.set_streak_counter(counter + 1);
  if (streaking != 0) {
    StoreU64(base, s.previous_position_address(), LoadU64(base, s.latest_position_address()));
    const uint64_t newer = LoadU64(base, s.latest_position_address() + 8);
    StoreU64(base, s.previous_position_address() + 8, newer);
    StoreU64(base, s.latest_position_address(), position_high);
    StoreU64(base, s.latest_position_address() + 8, position_low);
  }
  if (int32_t(s.streak_counter()) > 2) {
    CopyDoublewords(base, s.latest_position_address(), s.previous_position_address());
  }
  if (int32_t(mode) == 1) {
    factor = G::sparks_mode_factor(base);
  }
  if (s.streaking() != 0) {
    s.set_streak_counter(0);
    for (uint32_t at = s.random_vectors_address() + 4, k = 0; k < 50; k++, at += 16) {
      for (const int32_t offset : {-4, 0, 4}) {
        const double range = s.spread();
        *value = RandomRange(base, -range, range, drawn);
        guest::StoreFloat(base, at + uint32_t(offset), float(*value * factor));
      }
    }
  } else {
    const uint32_t zero = guest::LoadU32(base, G::zero_address());
    for (uint32_t at = s.random_vectors_address() + 8, k = 0; k < 50; k++, at += 16) {
      guest::StoreU32(base, at - 8, zero);
      guest::StoreU32(base, at - 4, zero);
      guest::StoreU32(base, at + 0, zero);
      guest::StoreU32(base, at + 4, one);
    }
  }

  if (int32_t(s.count()) <= 0) {
    return;
  }
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  schema::Spark spark(base, s.sparks_address() + head * kSparksBytes);
  int32_t i = 0;
  do {
    if (int32_t(spark.state()) == 2) {
      AddVectorTo(base, spark.origin_address(), spark.velocity_address(), csr, flushing);
    }
    if (int32_t(spark.state()) == 1) {
      if (s.streaking() != 0) {
        const float dx = Difference(guest::LoadFloat(base, spark.position_address()),
                                    guest::LoadFloat(base, spark.origin_address()));
        const float dy = Difference(guest::LoadFloat(base, spark.position_address() + 4),
                                    guest::LoadFloat(base, spark.origin_address() + 4));
        const float dz = Difference(guest::LoadFloat(base, spark.position_address() + 8),
                                    guest::LoadFloat(base, spark.origin_address() + 8));
        const float limit = spark.limit();
        const simde__m128 moved =
            VectorOf(std::bit_cast<uint32_t>(dx), std::bit_cast<uint32_t>(dy),
                     std::bit_cast<uint32_t>(dz), one);
        const simde__m128 length = Flushing(csr, flushing, Length, moved);
        if (double(limit) < double(std::bit_cast<float>(WordOf(length, 0)))) {
          spark.set_state(2);
        } else {
          StoreU64(base, spark.origin_address(), position_high);
          StoreU64(base, spark.origin_address() + 8, position_low);
        }
        *drawn = Rand(base);
        const int32_t r = int32_t(*drawn);
        if (r - (r / 10) * 10 == 0) {
          spark.set_state(2);
        }
        for (uint32_t k = 0; k < 12; k += 4) {
          guest::StoreFloat(base, spark.offset_address() + k,
                            Difference(guest::LoadFloat(base, spark.origin_address() + k),
                                       guest::LoadFloat(base, spark.position_address() + k)));
        }
      } else {
        AddVectorTo(base, spark.origin_address(), spark.velocity_address(), csr, flushing);
      }
    }
    const uint32_t last = s.capacity() * kSparksBytes + sparks - 16;
    spark = schema::Spark(base, spark.address == last ? s.sparks_address() : spark.address + kSparksBytes);
    i++;
  } while (i < int32_t(s.count()));
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// fctiwz of a float word, as the original's lfs, fctiwz, stfiwx.
uint32_t FloatToInt(uint8_t* base, uint32_t address) {
  return uint32_t(Fctiwz(double(guest::LoadFloat(base, address))));
}

}  // namespace

bool SparksEmit(uint8_t* base, uint32_t sparks, uint32_t position, double strength,
                const uint32_t vector[4], uint32_t* result, double* last) {
  ITC_NATIVE_RUNNING("sub_8212D818 Sparks::Emit");
  const schema::Sparks group(base, sparks);
  const int32_t capacity = int32_t(group.capacity());
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const int32_t count = int32_t(group.count());
  const schema::OverlayParams table = group.params();
  if (!(capacity > count)) {
    *result = 0;
    return true;
  }
  const schema::Spark spark(base, group.sparks_address() +
      group.head() * kSparksBytes);
  const double vx = std::bit_cast<float>(vector[0]);
  const double vy = std::bit_cast<float>(vector[1]);
  const double vz = std::bit_cast<float>(vector[2]);

  // Size, life and the limit.
  const double scale = G::half(base);
  {
    const double high = table.size();
    *last = RandomRange(base, float(high * scale), high);
    spark.set_size(float(*last));
  }
  CopyFloat(base, table.alpha_address(), spark.alpha_address());
  const double limit_high = G::sparks_limit_high(base);
  const double limit_low = G::frame_scale(base);
  const uint32_t life = table.life();
  spark.set_shown(1);
  spark.set_unknown_132(0);
  spark.set_life(life);
  *last = RandomRange(base, limit_low, limit_high);
  spark.set_limit(float(*last));

  // The state.
  {
    const int32_t drawn = int32_t(Rand(base));
    const int32_t state = drawn - (drawn / 5) * 5;
    spark.set_state(uint32_t(state));
    if (state > 1) {
      spark.set_state(1);
    }
  }
  const double zero = G::zero(base);
  if (strength > zero) {
    spark.set_state(2);
  }

  // The colors.
  {
    const int32_t drawn = int32_t(Rand(base));
    const uint32_t pick = uint32_t(drawn - (drawn / 5) * 5);
    if (pick == 1 || pick == 3) {
      const uint32_t alpha = FloatToInt(base, spark.alpha_address());
      spark.set_green(200);
      spark.set_red(255);
      spark.set_blue(200);
      spark.set_alpha_int(alpha);
    } else if (pick < 5) {
      const uint32_t color = pick == 0 ? 200 : 255;
      spark.set_alpha_int(FloatToInt(base, spark.alpha_address()));
      spark.set_blue(color);
      spark.set_green(color);
      spark.set_red(color);
    }
  }

  // The position.
  const uint32_t at = spark.position_address();
  if (spark.state() != 0) {
    for (uint32_t i = 0; i < 16; i += 4) {
      CopyFloat(base, position + i, spark.origin_address() + i);
    }
    for (uint32_t i = 0; i < 16; i += 4) {
      CopyFloat(base, position + i, at + i);
    }
    if (strength > zero) {
      const uint32_t one = guest::LoadU32(base, G::one_address());
      // Toward the sparks' position less the vector, by a random step.
      const double step_high = limit_low;
      double radius = RandomRange(base, std::bit_cast<float>(one), step_high);
      *last = radius;
      const float dx = Difference(guest::LoadFloat(base, group.position_address() + 0), vx);
      const float dy = Difference(guest::LoadFloat(base, group.position_address() + 4), vy);
      const float dz = Difference(guest::LoadFloat(base, group.position_address() + 8), vz);
      const simde__m128 toward =
          VectorOf(std::bit_cast<uint32_t>(dx), std::bit_cast<uint32_t>(dy),
                   std::bit_cast<uint32_t>(dz), one);
      const uint32_t csr = Fpscr::getcsr();
      const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
      const simde__m128 length = Flushing(csr, flushing, Length, toward);
      const double length_x = std::bit_cast<float>(WordOf(length, 0));
      if (!(length_x == zero)) {
        const simde__m128 from = LoadVector(base, at);
        const float s = float(double(G::sparks_step(base)) / length_x);
        const uint32_t sw = std::bit_cast<uint32_t>(s);
        const simde__m128 factor = VectorOf(sw, sw, sw, guest::LoadU32(base, G::zero_address()));
        const simde__m128 moved = Flushing(
            csr, flushing,
            // The NaNs the translated code keeps where two meet, as built
            // (the harness showed it): the step's (toward) in the product,
            // then the product's in the sum. Made explicit since the
            // barriers changed the compiler's operand order.
            [](simde__m128 f, simde__m128 t, simde__m128 k) {
              return AddKeepingFirstNaN(MulKeepingFirstNaN(t, k), f);
            },
            from, toward, factor);
        const uint32_t to = at & ~0xFu;
        for (int k = 0; k < 4; k++) {
          guest::StoreU32(base, to + 4 * k, WordOf(moved, k));
        }
      }
      // And by a random angle and radius.
      const double angle = RandomRange(base, G::angle_low(base),
                                       G::angle_high(base));
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
    const double high_x = Difference(guest::LoadFloat(base, group.position_address() + 0), vx);
    const double high_z = Difference(guest::LoadFloat(base, group.position_address() + 8), vz);
    const double ox = RandomRange(base, low, high_x);
    const double oy = RandomRange(base, G::spread_low(base),
                                  G::spread_high(base));
    const double oz = RandomRange(base, low, high_z);
    *last = oz;
    for (uint32_t i = 0; i < 16; i += 4) {
      CopyFloat(base, position + i, spark.origin_address() + i);
    }
    guest::StoreFloat(base, at + 0, float(double(guest::LoadFloat(base, position + 0)) + ox));
    guest::StoreFloat(base, at + 4, float(double(guest::LoadFloat(base, position + 4)) + oy));
    guest::StoreFloat(base, at + 8, float(double(guest::LoadFloat(base, position + 8)) + oz));
  }

  // The offset from where it started, and the velocity.
  for (uint32_t i = 0; i < 12; i += 4) {
    guest::StoreFloat(base, spark.offset_address() + i,
                      Difference(guest::LoadFloat(base, at + i),
                                 guest::LoadFloat(base, spark.origin_address() + i)));
  }
  const double side_x = Difference(vx, guest::LoadFloat(base, position + 0));
  const double side_z = Difference(vz, guest::LoadFloat(base, position + 8));
  double rise = G::sparks_speed_out(base);
  const double inward = G::sparks_speed_in(base);
  const double toward_x = side_x < zero ? inward : rise;
  const double toward_z = side_z < zero ? inward : rise;
  const uint32_t state = spark.state();
  if (state == 1) {
    const double vz_drawn = RandomRange(base, zero, toward_z);
    const double vy_drawn = RandomRange(base, inward, rise);
    const double vx_drawn = RandomRange(base, zero, toward_x);
    *last = vx_drawn;
    guest::StoreFloat(base, spark.velocity_address(), float(vx_drawn));
    guest::StoreFloat(base, spark.velocity_address() + 4, float(vy_drawn));
    guest::StoreFloat(base, spark.velocity_address() + 8, float(vz_drawn));
    guest::StoreU32(base, spark.velocity_address() + 12, guest::LoadU32(base, G::zero_address()));
    const int32_t drawn = int32_t(Rand(base));
    if (drawn - (drawn / 20) * 20 == 0) {
      guest::StoreFloat(base, spark.velocity_address(), float(toward_x + double(guest::LoadFloat(base, spark.velocity_address()))));
      const double y = guest::LoadFloat(base, spark.velocity_address() + 4);
      const double z = guest::LoadFloat(base, spark.velocity_address() + 8);
      guest::StoreFloat(base, spark.velocity_address() + 4, float(y + rise));
      guest::StoreFloat(base, spark.velocity_address() + 8, float(z + toward_z));
    }
  } else if (state < 3) {
    if (double(guest::LoadFloat(base, spark.position_address() + 4)) < double(guest::LoadFloat(base, position + 4))) {
      rise = guest::LoadFloat(base, state == 0 ? G::sparks_rise_state0_address() : G::sparks_rise_state2_address());
    }
    const double vz_drawn = RandomRange(base, zero, toward_z);
    const double vy_drawn = RandomRange(base, zero, rise);
    const double vx_drawn = RandomRange(base, zero, toward_x);
    *last = vx_drawn;
    guest::StoreU32(base, spark.velocity_address() + 12, guest::LoadU32(base, G::zero_address()));
    guest::StoreFloat(base, spark.velocity_address() + 8, float(vz_drawn));
    guest::StoreFloat(base, spark.velocity_address() + 4, float(vy_drawn));
    guest::StoreFloat(base, spark.velocity_address(), float(vx_drawn));
  }

  // The head moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(group.head() + 1);
  const int32_t size_now = int32_t(group.capacity());
  const uint32_t added = group.count() + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  group.set_head(uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  group.set_count(added);
  *result = 1;
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSmokeBytes = 720;

}  // namespace

bool SmokeEmit(uint8_t* base, uint32_t smoke, double size, double speed, uint32_t frame,
               uint32_t region, uint32_t value, uint32_t* result, double* last) {
  ITC_NATIVE_RUNNING("sub_8212A350 Smoke::Emit");
  const schema::Smoke self(base, smoke);
  const int32_t capacity = int32_t(self.capacity());
  if (capacity == 0 || capacity == -1) {
    return false;
  }
  const int32_t count = int32_t(self.count());
  const schema::OverlayParams table = self.params();
  if (!(capacity > count)) {
    *result = 0;
    return true;
  }
  // The puff, and the one before it (the last when this is the first).
  const int32_t head = int32_t(self.head());
  const schema::SmokePuff puff(base, self.puffs_address() + uint32_t(head) * kSmokeBytes);
  schema::SmokePuff previous;
  if (puff.address == self.puffs_address()) {
    previous = schema::SmokePuff(base, smoke + uint32_t(capacity) * kSmokeBytes - 592);
  } else {
    const int32_t before = head - 1;
    const int32_t quotient = before == INT32_MIN && capacity == -1 ? 0 : before / capacity;
    previous = schema::SmokePuff(base, self.puffs_address() +
               (uint32_t(before) - uint32_t(quotient) * uint32_t(capacity)) * kSmokeBytes);
  }

  puff.set_emit_size(float(size));
  puff.set_shown(value);
  {
    const uint32_t alpha = guest::LoadU32(base, table.alpha_address());
    guest::StoreU32(base, puff.alpha_address(), alpha);
    guest::StoreU32(base, puff.most_alpha_address(), alpha);
  }
  const double zero = G::zero(base);
  const uint32_t zero_word = guest::LoadU32(base, G::zero_address());
  const double growth_high = G::smoke_growth_high(base);
  puff.set_life(table.life());
  {
    const double drawn = RandomRange(base, zero, growth_high);
    const double frame_value = guest::LoadFloat(base, frame + 84);
    const double scale = G::frame_scale(base);
    const float growth = float(std::fma(frame_value, scale, drawn));
    puff.set_size(growth);
    puff.set_start_size(growth);
  }
  const uint32_t one_word = guest::LoadU32(base, G::one_address());
  const double one = std::bit_cast<float>(one_word);
  {
    const double drawn = RandomRange(base, one, G::smoke_rise_high(base));
    const double scale = G::speed_scale(base);
    puff.set_growth(float(std::fma(speed, scale, drawn)));
  }
  const uint32_t velocity = puff.velocity_address();
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyFloat(base, frame + 64 + i, puff.position_address() + i);
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    CopyFloat(base, self.velocity_address() + i, velocity + i);
  }
  {
    const uint32_t alpha = FloatToInt(base, puff.alpha_address());
    puff.set_red(255);
    puff.set_green(255);
    puff.set_blue(255);
    puff.set_alpha_int(alpha);
  }

  // The drift.
  double span_x, span_y, span_z;
  if (region != 0) {
    span_x = Difference(guest::LoadFloat(base, self.region_end_address()), guest::LoadFloat(base, self.region_start_address()));
    span_y = Difference(guest::LoadFloat(base, self.region_end_address() + 4), guest::LoadFloat(base, self.region_start_address() + 4));
    span_z = Difference(guest::LoadFloat(base, self.region_end_address() + 8), guest::LoadFloat(base, self.region_start_address() + 8));
  } else {
    span_x = Difference(guest::LoadFloat(base, puff.position_address()), guest::LoadFloat(base, velocity + 0));
    span_y = Difference(guest::LoadFloat(base, puff.position_address() + 4), guest::LoadFloat(base, velocity + 4));
    span_z = Difference(guest::LoadFloat(base, puff.position_address() + 8), guest::LoadFloat(base, velocity + 8));
  }
  const double reach = RandomRange(
      base, zero, float(speed * double(G::smoke_drift_scale(base))));
  const double radius = float(reach + double(G::smoke_radius_base(base)));
  const double along = RandomRange(base, zero, G::smoke_drift_high(base));
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
  const simde__m128 drift = Flushing(csr, flushing, AddKeepingFirstNaN, drift_a, drift_b);
  int32_t particles = Fctiwz(double(float(speed * double(G::smoke_count_scale(base)))));
  if (particles < 2) {
    particles = 2;
  } else if (particles > 10) {
    particles = 10;
  }
  puff.set_particle_count(uint32_t(particles));

  // The particles.
  const double step_limit = float(speed * double(G::smoke_radius_scale(base)));
  const double angle_high = G::angle_high(base);
  const double angle_low = G::angle_low(base);
  schema::OverlayParticle particle(base, puff.particles_address());
  for (int32_t k = 0; k < int32_t(puff.particle_count()); k++, particle.address += 48) {
    const double limit = step_limit > one ? one : step_limit;
    const double share = float(double(int64_t(int32_t(puff.particle_count()))));
    const double dx = Difference(guest::LoadFloat(base, velocity + 0), guest::LoadFloat(base, puff.position_address()));
    const double dz = Difference(guest::LoadFloat(base, velocity + 8), guest::LoadFloat(base, puff.position_address() + 8));
    const double dy = Difference(guest::LoadFloat(base, velocity + 4), guest::LoadFloat(base, puff.position_address() + 4));
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
      guest::StoreU32(base, particle.velocity_address() + 4 * i, WordOf(drift, i));
    }
    {
      const int32_t drawn = int32_t(Rand(base));
      particle.set_pick_a(uint32_t(drawn - (drawn / 3) * 3));
    }
    {
      const int32_t drawn = int32_t(Rand(base));
      particle.set_shown(1);
      particle.set_pick_b(uint32_t(drawn - (drawn / 4) * 4));
    }
    const int32_t apart = int32_t(puff.life() - previous.life());
    if (apart <= 2) {
      const double index = float(double(int64_t(k)));
      guest::StoreFloat(base, particle.position_address(), float(double(float(step_x * index)) + double(guest::LoadFloat(base, puff.position_address()))));
      const double y = float(step_y * index);
      guest::StoreFloat(base, particle.position_address() + 4, float(double(guest::LoadFloat(base, puff.position_address() + 4)) + y));
      const double z = float(step_z * index);
      guest::StoreFloat(base, particle.position_address() + 8, float(double(guest::LoadFloat(base, puff.position_address() + 8)) + z));
    } else {
      // (The original first stores +48 plus the cosine offset and +56 plus
      // the sine offset, then overwrites them with these.)
      for (uint32_t i = 0; i < 16; i += 4) {
        CopyFloat(base, puff.position_address() + i, particle.address + i);
      }
      if (uint32_t(k) >= 5) {
        puff.set_particle_count(5);
        break;
      }
    }
  }

  // The head moves on (divw, then the remainder) and the count goes up.
  const int32_t next = int32_t(self.head() + 1);
  const int32_t size_now = int32_t(self.capacity());
  const uint32_t added = self.count() + 1;
  const int32_t quotient =
      (size_now && !(next == INT32_MIN && size_now == -1)) ? next / size_now : 0;
  self.set_head(uint32_t(next) - uint32_t(quotient) * uint32_t(size_now));
  self.set_count(added);
  *result = 1;
  return true;
}

bool EmitSmoke(uint8_t* base, uint32_t overlays, double rate, double speed, uint32_t frame,
               uint32_t wheel, uint32_t value, double* last) {
  ITC_NATIVE_RUNNING("sub_82123240 CarOverlays::EmitSmoke");
  const schema::CarOverlays self(base, overlays);
  // The wheel's rate accumulator (48 bytes a wheel) and smoke overlay.
  const uint32_t accumulator = self.smoke_rates_address() + wheel * 48;
  const uint32_t smoke = LoadWord(self, self.smokes_address() + (wheel << 2));
  const schema::Smoke s(base, smoke);
  const uint32_t region = wheel >= 4 ? 1 : 0;
  if (rate == double(G::zero(base))) {
    return true;
  }
  const int32_t capacity = int32_t(s.capacity());
  if (capacity == 0 || capacity == -1 ||
      (accumulator + 4 > s.capacity_address() && accumulator < s.texture_holder_address())) {
    return false;
  }
  const float sum = float(double(std::bit_cast<float>(LoadWord(self, accumulator))) + rate);
  StoreWord(self, accumulator, std::bit_cast<uint32_t>(sum));
  if (double(sum) < double(G::one(base))) {
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
  StoreWord(self, accumulator,
            std::bit_cast<uint32_t>(
                float(double(std::bit_cast<float>(LoadWord(self, accumulator))) - whole)));
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

void OverlayConstruct(uint8_t* base, uint32_t overlay) {
  ITC_NATIVE_RUNNING("sub_82126498 Overlay::Overlay");
  schema::Overlay(base, overlay).set_vtable(schema::Overlay::kVtable);
}

void ClearViews(uint8_t* base, uint32_t overlays) {
  ITC_NATIVE_RUNNING("sub_82126530 CarOverlays::ClearViews");
  const schema::CarOverlays self(base, overlays);
  for (uint32_t i = 0; i < 25; i++) {
    StoreWord(self, self.views_address() + 4 * i, 0);
  }
}

void ViewDescClear(uint8_t* base, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_82126860 ViewDesc::Clear");
  const schema::ViewDesc d(base, desc);
  d.set_vehicle(0);
  d.set_model(0);
}

void OverlayViewConstruct(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126870 OverlayView::OverlayView");
  const schema::OverlayView v(base, view);
  v.set_vtable(schema::OverlayView::kVtable);
  v.set_vehicle(0);
  v.set_model(0);
}

void OverlayViewSet(uint8_t* base, uint32_t view, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_82126890 OverlayView::Set");
  // Word by word, in the original's order (the two may overlap).
  const schema::OverlayView v(base, view);
  for (uint32_t i = 0; i < 3; i++) {
    v.store_at(v.vehicle_address() - v.address + 4 * i, 4,
               guest::LoadU32(base, schema::ViewDesc(base, desc).vehicle_address() + 4 * i));
  }
}

void BindViewport(uint8_t* base, uint32_t out, uint32_t index, uint32_t second) {
  ITC_NATIVE_RUNNING("sub_821264C0 CarOverlays::BindViewport");
  const schema::OverlayViewportEntry entry(base, G::viewport_entries_address() + 12 * index);
  const uint32_t pair = G::viewport_pairs_address() + 8 * second;
  const schema::OverlayViewportBinding binding(base, out);
  const uint32_t owners = G::viewport_owners(base);
  const uint32_t owner = guest::LoadU32(base, owners + 4 * entry.viewport());
  binding.set_record(owner + 80 * entry.record());
  binding.set_camera_pass(G::camera_passes_address() + 592 * entry.camera_pass());
  binding.set_pair_0(guest::LoadU32(base, pair));
  binding.set_pair_1(guest::LoadU32(base, pair + 4));
}

void SetRenderTargets(uint8_t* base, uint32_t queue, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82128190 Overlays::SetRenderTargets");
  G::set_render_queue(base, queue);
  G::set_command_list(base, list);
}

void ParticleConstruct(uint8_t* base, uint32_t particle) {
  ITC_NATIVE_RUNNING("sub_8212B3D0 Particle::Particle");
  const uint32_t one = guest::LoadU32(base, G::one_address());
  const schema::OverlayParticle p(base, particle);
  guest::StoreU32(base, p.position_address() + 12, one);
  guest::StoreU32(base, p.velocity_address() + 12, one);
}

void RaceOverlaysClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8215DE80 RaceOverlays::Clear");
  for (uint32_t i = 0; i < 25; i++) {
    guest::StoreU32(base, schema::RaceOverlays(base, list).overlay_0_address() + 4 * i, 0);
  }
}

uint32_t ShareTextures(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8215E000 RaceOverlays::ShareTextures");
  uint32_t entry = 0;
  for (uint32_t i = 1; i < 25; i++) {
    entry = guest::LoadU32(base, schema::RaceOverlays(base, list).overlay_0_address() + 4 * i);
    if (entry != 0) {
      SetTextures104(base, entry, schema::RaceOverlays(base, list).overlay_0().textures());
    }
  }
  return entry;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kFloatOne = 0x3F800000;  // the identity rows' 1.0 (vupkd3d128's)

// The stores of a reset, in the original's order: 'z' the game's 0.0 word,
// 'o' its 1.0 word, '0' and '1' the integers, 'v' a unit vector (four
// stores from the offset).
struct ResetStore {
  int32_t offset;
  char kind;
};

void ApplyStores(uint8_t* base, uint32_t at, std::initializer_list<ResetStore> stores) {
  const uint32_t zero = guest::LoadU32(base, G::zero_address());
  const uint32_t one = guest::LoadU32(base, G::one_address());
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
  const schema::LightSprite self(base, sprite);
  ApplyStores(base, sprite, {{272, 'v'}});
  const uint32_t inner = self.sprite_address();
  ClearSpriteVectors(base, inner);
  ApplyStores(base, inner, {{96, '0'}, {100, 'z'}, {108, '0'}, {104, 'z'}});
  schema::OverlaySpriteEntry(base, inner).set_unknown_116(4);
  schema::OverlaySpriteEntry(base, inner).set_kind(0);
}

void ModelSpriteReset(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_8211F430 ModelSprite::Reset");
  const schema::ModelSprite self(base, sprite);
  const uint32_t inner = self.sprite_address();
  ClearSpriteVectors(base, inner);
  ApplyStores(base, inner, {{96, '0'}, {108, '0'}, {100, 'z'}, {128, '0'}, {104, 'z'}});
  schema::OverlaySpriteEntry(base, inner).set_kind(9);
  schema::OverlaySpriteEntry(base, inner).set_unknown_120(9);
  schema::OverlaySpriteEntry(base, inner).set_unknown_116(4);
  schema::OverlaySpriteEntry(base, inner).set_unknown_124(12);
  ApplyStores(base, sprite, {{368, 'v'}, {384, 'v'}});
  self.set_unknown_404(-1);
  self.set_unknown_408(-1);
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, self.unknown_416_address() + 16 * i, {{0, 'v'}});
  }
}

void SpriteResetStyle2(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_82123A00 Sprite::ResetStyle2");
  ApplyStores(base, sprite, {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {64, '0'}, {68, '0'}});
  schema::OverlaySprite(base, sprite).set_address_mode(2);
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow(base, schema::OverlaySprite(base, sprite).matrix_address() + 16 * row, row);
  }
  ApplyStores(base, sprite, {{92, 'o'}, {108, 'o'}});
}

void TrailReset(uint8_t* base, uint32_t trail) {
  ITC_NATIVE_RUNNING("sub_82129498 Trail::Reset");
  const schema::Trail self(base, trail);
  ApplyStores(base, trail, {{64128, '0'}, {64132, '0'}, {64136, '0'}, {64144, 'v'}});
  for (uint32_t i = 0; i < self.capacity(); i++) {
    ApplyStores(base, self.points_address() + 128 * i,
                {{0, 'v'}, {16, 'v'}, {64, 'v'}, {96, 'v'}, {80, 'v'}, {32, '0'}, {36, '0'},
                 {40, '0'}, {44, '1'}, {52, 'z'}, {56, 'z'}, {112, '0'}, {124, 'z'}, {60, '0'},
                 {116, '0'}, {48, '0'}, {120, '0'}});
  }
}

void SmokeEmitterReset(uint8_t* base, uint32_t emitter) {
  ITC_NATIVE_RUNNING("sub_8212A218 SmokeEmitter::Reset");
  const schema::SmokePuff self(base, emitter);
  ApplyStores(base, emitter, {{48, 'v'}, {64, 'v'}, {32, 'v'}, {208, 'v'}});
  // The original's order: rows 1, 0, 3, 2.
  StoreIdentityRow(base, self.frame_address() + 16, 1);
  StoreIdentityRow(base, self.frame_address(), 0);
  StoreIdentityRow(base, self.frame_address() + 48, 3);
  StoreIdentityRow(base, self.frame_address() + 32, 2);
  ApplyStores(base, emitter,
              {{0, 'v'}, {16, 'v'}, {80, '0'}, {84, '0'}, {88, '0'}, {92, '1'}, {104, 'z'},
               {120, 'z'}, {108, '0'}, {124, 'z'}, {96, '0'}, {100, 'z'}});
  self.set_particle_count(10);
  ApplyStores(base, emitter, {{116, 'z'}});
  const uint32_t size = guest::LoadU32(base, G::smoke_puff_size_address());
  guest::StoreU32(base, self.emit_size_address(), size);
  self.set_frame_value(size);
  ApplyStores(base, emitter, {{228, 'z'}});
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, self.particles_address() + 48 * i, {{0, 'v'}, {16, 'v'}, {32, '0'}, {36, '0'}, {40, '0'}});
  }
}

void SmokeEmitterConstruct(uint8_t* base, uint32_t emitter) {
  ITC_NATIVE_RUNNING("sub_8212B230 SmokeEmitter::SmokeEmitter");
  const schema::SmokePuff self(base, emitter);
  ApplyStores(base, emitter, {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}, {156, 'o'},
                              {172, 'o'}, {188, 'o'}, {204, 'o'}, {220, 'o'}});
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, self.particles_address() + 48 * i, {{12, 'o'}, {28, 'o'}});
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
  const schema::ParticlesC self(base, particles);
  ApplyStores(base, particles, {{2704, 'z'}, {2688, '0'}, {2692, '0'}, {2696, '0'}, {2672, 'v'}});
  for (uint32_t i = 0; i < self.capacity(); i++) {
    ApplyStores(base, self.entries_address() + 128 * i,
                {{48, 'v'}, {64, 'v'}, {32, 'v'}, {0, 'v'}, {16, 'v'}, {80, '0'}, {84, '0'},
                 {88, '0'}, {92, '1'}, {100, 'z'}, {104, 'z'}, {108, '0'}, {96, '0'},
                 {112, '0'}});
  }
}

void MarkerReset(uint8_t* base, uint32_t marker) {
  ITC_NATIVE_RUNNING("sub_8212CC38 Marker::Reset");
  const schema::Marker self(base, marker);
  ApplyStores(base, marker, {{272, 'v'}, {288, '1'}});
  const uint32_t inner = self.sprite_address();
  ClearSpriteVectors(base, inner);
  ApplyStores(base, inner, {{96, '0'}, {100, 'z'}, {108, '0'}, {104, 'z'}, {112, '0'}});
}

void SparksReset(uint8_t* base, uint32_t sparks) {
  ITC_NATIVE_RUNNING("sub_8212D6E0 Sparks::Reset");
  const schema::Sparks self(base, sparks);
  ApplyStores(base, sparks,
              {{28932, '0'}, {28936, '0'}, {28940, '0'}, {28928, '0'}, {28960, 'v'},
               {28992, 'v'}, {29008, 'v'}, {28976, '0'}});
  for (uint32_t i = 0; i < 50; i++) {
    ApplyStores(base, self.random_vectors_address() + 16 * i, {{0, 'v'}});
  }
  for (uint32_t i = 0; i < self.capacity(); i++) {
    ApplyStores(base, self.sparks_address() + 144 * i,
                {{48, 'v'}, {64, 'v'}, {32, 'v'}, {0, 'v'}, {16, 'v'}, {112, 'v'}, {80, '0'},
                 {84, '0'}, {88, '0'}, {92, '1'}, {100, 'z'}, {104, 'z'}, {108, '0'}, {136, 'z'},
                 {128, '0'}, {96, '0'}, {132, '0'}});
  }
}

}  // namespace race_overlays

namespace race_overlays {

uint32_t SmokeInRegion(uint8_t* base, uint32_t smoke, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_8212A868 Smoke::InRegion");
  const schema::Smoke self(base, smoke);
  const double ax = guest::LoadFloat(base, self.region_rear_address());
  const double ay = guest::LoadFloat(base, self.region_rear_address() + 4);
  const double az = guest::LoadFloat(base, self.region_rear_address() + 8);
  const double bx = guest::LoadFloat(base, self.region_front_address());
  const double by = guest::LoadFloat(base, self.region_front_address() + 4);
  const double bz = guest::LoadFloat(base, self.region_front_address() + 8);
  const double px = guest::LoadFloat(base, point + 0);
  const double py = guest::LoadFloat(base, point + 4);
  const double pz = guest::LoadFloat(base, point + 8);
  const uint32_t one = guest::LoadU32(base, G::one_address());
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
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  // Each in its own window (the same results: the control word is the same
  // for both); the second had been moved past the restore before the
  // barriers.
  const simde__m128 past_a = Flushing(csr, flushing, Dot3, p_a, a_b);
  const simde__m128 past_b = Flushing(csr, flushing, Dot3, p_b, b_a);
  const double zero = G::zero(base);
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
  const schema::Sparks self(base, sparks);
  const int32_t count = int32_t(self.count());
  if (count <= 0) {
    return true;
  }
  const int32_t capacity = int32_t(self.capacity());
  const int32_t head = int32_t(self.oldest());
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
  const schema::OverlayParams table = self.params();
  const double zero = G::zero(base);
  const double one = G::one(base);
  const uint32_t zero_word = guest::LoadU32(base, G::zero_address());
  int32_t index = head;
  for (int32_t left = count; left > 0; left--) {
    const schema::Spark entry(base, self.sparks_address() + 144 * uint32_t(index));
    const int32_t life = int32_t(entry.life() - 1);
    entry.set_life(uint32_t(life));
    if (life < 0) {
      entry.set_life(0);
    }
    if (!(int32_t(entry.life()) > 0)) {
      entry.set_shown(0);
      const int32_t next = int32_t(self.oldest() + 1);
      const int32_t modulo = int32_t(self.capacity());
      const uint32_t remaining = self.count() - 1;
      self.set_oldest(uint32_t(next % modulo));
      self.set_count(remaining);
    }
    const uint32_t fade_ticks = table.fade_ticks();
    uint32_t brightness_word = guest::LoadU32(base, table.alpha_address());
    const uint32_t now = entry.life();
    if (fade_ticks > now) {
      const float ticks = float(double(fade_ticks));
      const float life_now = float(double(int32_t(now)));
      const float step = float(one / double(ticks));
      const float scaled = float(double(step) * double(life_now));
      brightness_word = std::bit_cast<uint32_t>(
          float(double(scaled) * double(table.alpha())));
    }
    // The original keeps this one as a double (it may be the product), so
    // a signaling NaN from the table comes back quieted.
    if (std::isnan(std::bit_cast<float>(brightness_word))) {
      brightness_word |= 0x00400000;
    }
    guest::StoreU32(base, entry.alpha_address(), brightness_word);
    if (double(std::bit_cast<float>(brightness_word)) < zero) {
      guest::StoreU32(base, entry.alpha_address(), zero_word);
    }
    const uint32_t most = guest::LoadU32(base, table.alpha_address());
    if (double(std::bit_cast<float>(most)) < double(entry.alpha())) {
      guest::StoreU32(base, entry.alpha_address(), most);
    }
    entry.set_alpha_int(uint32_t(Fctiwz(double(entry.alpha()))));
    // Where both operands are NaNs, the translated code (subss and addss
    // with the position as the destination, seen in the disassembly) keeps
    // the position's, quieted.
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(base, entry.offset_address() + 4 * i,
                      KeepFirstNaN(guest::LoadU32(base, entry.position_address() + 4 * i),
                                   guest::LoadU32(base, entry.origin_address() + 4 * i), false));
    }
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(base, entry.position_address() + 4 * i,
                      KeepFirstNaN(guest::LoadU32(base, entry.position_address() + 4 * i),
                                   guest::LoadU32(base, entry.velocity_address() + 4 * i), true));
    }
    index = (index + 1) % int32_t(self.capacity());
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// Overlay::Overlay, then the class's vtable.
void ConstructOverlay(uint8_t* base, uint32_t overlay, uint32_t vtable) {
  OverlayConstruct(base, overlay);
  schema::Overlay(base, overlay).set_vtable(vtable);
}

}  // namespace

void LightSpriteConstruct(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_8211F060 LightSprite::LightSprite");
  ConstructOverlay(base, sprite, schema::LightSprite::kVtable);
  ApplyStores(base, sprite, {{156, 'o'}, {172, 'o'}, {188, 'o'}, {204, 'o'}, {220, 'o'}, {284, 'o'}});
}

void ModelSpriteConstruct(uint8_t* base, uint32_t sprite) {
  ITC_NATIVE_RUNNING("sub_82122880 ModelSprite::ModelSprite");
  const schema::ModelSprite self(base, sprite);
  ConstructOverlay(base, sprite, schema::ModelSprite::kVtable);
  ApplyStores(base, sprite,
              {{236, 'o'}, {252, 'o'}, {268, 'o'}, {284, 'o'}, {300, 'o'}, {380, 'o'}, {396, 'o'}});
  for (uint32_t i = 0; i < 10; i++) {
    ApplyStores(base, self.unknown_416_address() + 12 + 16 * i, {{0, 'o'}});
  }
}

void TrailConstruct(uint8_t* base, uint32_t trail) {
  ITC_NATIVE_RUNNING("sub_8212A140 Trail::Trail");
  const schema::Trail self(base, trail);
  ConstructOverlay(base, trail, schema::Trail::kVtable);
  for (uint32_t i = 0; i < 500; i++) {
    ApplyStores(base, self.points_address() + 128 * i, {{12, 'o'}, {28, 'o'}, {76, 'o'}, {92, 'o'}, {108, 'o'}});
  }
  ApplyStores(base, trail, {{64156, 'o'}});
}

void SmokeConstruct(uint8_t* base, uint32_t smoke) {
  ITC_NATIVE_RUNNING("sub_8212B2E8 Smoke::Smoke");
  const schema::Smoke self(base, smoke);
  OverlayConstruct(base, smoke);
  self.set_unknown_116(0);
  self.set_vtable(schema::Smoke::kVtable);
  for (uint32_t i = 0; i < 45; i++) {
    SmokeEmitterConstruct(base, self.puffs_address() + 720 * i);
  }
  ApplyStores(base, smoke, {{32556, 'o'}, {32572, 'o'}, {32588, 'o'}, {32604, 'o'}, {32620, 'o'}});
}

void ParticlesCConstruct(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212CAF8 ParticlesC::ParticlesC");
  const schema::ParticlesC self(base, particles);
  ConstructOverlay(base, particles, schema::ParticlesC::kVtable);
  for (uint32_t i = 0; i < 20; i++) {
    ApplyStores(base, self.entries_address() + 128 * i,
                {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}});
  }
  ApplyStores(base, particles, {{2684, 'o'}});
}

void MarkerConstruct(uint8_t* base, uint32_t marker) {
  ITC_NATIVE_RUNNING("sub_8212D620 Marker::Marker");
  ConstructOverlay(base, marker, schema::Marker::kVtable);
  ApplyStores(base, marker, {{140, 'o'}, {156, 'o'}, {172, 'o'}, {188, 'o'}, {204, 'o'}, {284, 'o'}});
}

void SparksConstruct(uint8_t* base, uint32_t sparks) {
  ITC_NATIVE_RUNNING("sub_8212E9A0 Sparks::Sparks");
  const schema::Sparks self(base, sparks);
  ConstructOverlay(base, sparks, schema::Sparks::kVtable);
  for (uint32_t i = 0; i < 200; i++) {
    ApplyStores(base, self.sparks_address() + 144 * i,
                {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}, {124, 'o'}});
  }
  ApplyStores(base, sparks, {{28972, 'o'}, {29004, 'o'}, {29020, 'o'}});
  for (uint32_t i = 0; i < 50; i++) {
    ApplyStores(base, self.random_vectors_address() + 12 + 16 * i, {{0, 'o'}});
  }
}

namespace {

constexpr uint32_t kParticleConstructor = 0x8212B3D0;  // Particle::Particle
constexpr uint32_t kEmptyDestructor = 0x82106540;      // blr

// The deleting destructors' frees: 0 or a block the layer gave.
bool Deletable(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

}  // namespace

void ParticlesBConstruct(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212BFE8 ParticlesB::ParticlesB");
  const schema::ParticlesB self(base, particles);
  ConstructOverlay(base, particles, schema::ParticlesB::kVtable);
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t entry = self.entries_address() + 176 * i;
    ApplyStores(base, entry, {{12, 'o'}, {28, 'o'}, {44, 'o'}, {60, 'o'}, {76, 'o'}});
    // Particle::Particle is native, so the helper calls no guest code.
    c_runtime::VectorConstruct(base, schema::ParticlesBEntry(base, entry).particle_address(), 48, 1, kParticleConstructor, nullptr, nullptr);
  }
  ApplyStores(base, particles, {{2796, 'o'}, {2812, 'o'}});
}

void SmokeDestruct(uint8_t* base, uint32_t smoke) {
  ITC_NATIVE_RUNNING("sub_8212B288 Smoke::~Smoke");
  const schema::Smoke self(base, smoke);
  self.set_vtable(schema::Smoke::kVtable);
  for (int32_t i = 44; i >= 0; i--) {
    c_runtime::VectorDestruct(base,
                              schema::SmokePuff(base, self.puffs_address() + 720 * uint32_t(i))
                                  .particles_address(),
                              48, 10,

                              kEmptyDestructor, nullptr, nullptr);
  }
  OverlayConstruct(base, smoke);
}

bool SmokeDeletingDestructor(uint8_t* base, uint32_t smoke, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8212B358 Smoke::~Smoke (deleting)");
  if ((flags & 1) != 0 && !Deletable(smoke)) {
    return false;
  }
  SmokeDestruct(base, smoke);
  if ((flags & 1) != 0) {
    memory::crt::OperatorDelete(smoke);
  }
  return true;
}

void ParticlesBDestruct(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212BF88 ParticlesB::~ParticlesB");
  schema::ParticlesB(base, particles).set_vtable(schema::ParticlesB::kVtable);
  for (int32_t i = 14; i >= 0; i--) {
    c_runtime::VectorDestruct(base,
                              schema::ParticlesBEntry(base, schema::ParticlesB(base, particles)
                                                                    .entries_address() +
                                                                176 * uint32_t(i))
                                  .particle_address(),
                              48, 1,

                              kEmptyDestructor, nullptr, nullptr);
  }
  OverlayConstruct(base, particles);
}

bool ParticlesBDeletingDestructor(uint8_t* base, uint32_t particles, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8212C070 ParticlesB::~ParticlesB (deleting)");
  if ((flags & 1) != 0 && !Deletable(particles)) {
    return false;
  }
  ParticlesBDestruct(base, particles);
  if ((flags & 1) != 0) {
    memory::crt::OperatorDelete(particles);
  }
  return true;
}

void SmokeReset(uint8_t* base, uint32_t smoke) {
  ITC_NATIVE_RUNNING("sub_8212A9E0 Smoke::Reset");
  const schema::Smoke self(base, smoke);
  ApplyStores(base, smoke, {{32528, '0'}, {32532, '0'}, {32536, '0'}, {32544, 'v'}, {32576, 'v'},
                            {32560, 'v'}});
  for (uint32_t i = 0; i < self.capacity(); i++) {
    SmokeEmitterReset(base, self.puffs_address() + 720 * i);
  }
}

void ParticlesBReset(uint8_t* base, uint32_t particles) {
  ITC_NATIVE_RUNNING("sub_8212BA98 ParticlesB::Reset");
  const schema::ParticlesB self(base, particles);
  ApplyStores(base, particles, {{2768, '0'}, {2772, '0'}, {2776, '0'}, {2784, 'v'}, {2800, 'v'}});
  for (uint32_t i = 0; i < self.capacity(); i++) {
    ParticlesBResetEntry(base, self.entries_address() + 176 * i);
  }
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

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
  const schema::Smoke self(base, smoke);
  const schema::OverlayParams table = self.params();
  int32_t count = int32_t(self.count());
  if (count == 0) {
    return true;
  }
  int32_t index = int32_t(self.oldest());
  if (!EntriesInRange(self.puffs_address(), kSmokeBytes,
                      int32_t(self.capacity()), index)) {
    return false;
  }
  auto emitter_at = [&](int32_t i) { return self.puffs_address() + kSmokeBytes * uint32_t(i); };
  schema::SmokePuff emitter(base, emitter_at(index));
  // The oldest emitter, when its life is out, is reset and dropped (the
  // head and count read again after the reset).
  if (!(int32_t(emitter.life()) > 0)) {
    SmokeEmitterReset(base, emitter.address);
    const int32_t head = int32_t(self.oldest());
    const int32_t capacity = int32_t(self.capacity());
    count = int32_t(self.count() - 1);
    self.set_count(uint32_t(count));
    index = NextIndex(head, capacity);
    self.set_oldest(uint32_t(index));
    if (count != 0) {
      emitter = schema::SmokePuff(base, emitter_at(index));
    }
  }
  if (!(count > 0)) {
    return true;
  }
  const double zero = G::zero(base);
  const double one = G::one(base);
  const uint32_t zero_word = guest::LoadU32(base, G::zero_address());
  for (; count > 0; count--) {
    const int32_t life_left = int32_t(emitter.life() - 1);
    emitter.set_life(uint32_t(life_left));
    if (life_left < 0) {
      emitter.set_life(0);
    }
    const uint32_t life = emitter.life();
    if (table.life() - life > 1) {
      emitter.set_shown(1);
    }
    // The size: +124 plus +116 x +124 x (the table's life - life) / the
    // table's life.
    const uint32_t most = table.life();
    const double size_from = emitter.start_size();
    const double growth = emitter.growth();
    double size = float(growth * size_from);
    size = float(size * double(FloatOf(int64_t(uint32_t(most - life)))));
    size = float(size / double(FloatOf(int64_t(most))));
    emitter.set_size(float(size + size_from));
    // The fade: +120 times life / the table's +12 while life is below it.
    const uint32_t fade_ticks = table.fade_ticks();
    const uint32_t most_fade_word = guest::LoadU32(base, emitter.most_alpha_address());
    const double most_fade = std::bit_cast<float>(most_fade_word);
    if (fade_ticks > life) {
      double fade = float(one / double(FloatOf(int64_t(fade_ticks))));
      fade = float(fade * double(FloatOf(int64_t(int32_t(life)))));
      emitter.set_alpha(float(fade * most_fade));
    } else {
      guest::StoreU32(base, emitter.alpha_address(), most_fade_word);
    }
    if (double(emitter.alpha()) < zero) {
      guest::StoreU32(base, emitter.alpha_address(), zero_word);
    }
    if (most_fade < double(emitter.alpha())) {
      guest::StoreU32(base, emitter.alpha_address(), most_fade_word);
    }
    emitter.set_alpha_int(uint32_t(Fctiwz(double(emitter.alpha()))));
    // The particles drift by their velocity (+16) and show (+36 = 1),
    // unless the region is on and they're inside it.
    for (int32_t i = 0; i < int32_t(emitter.particle_count()); i++) {
      const uint32_t particle = emitter.particles_address() + 48 * uint32_t(i);
      // Where both are NaNs, the translated code (addss, seen in the
      // disassembly) keeps the position's for x and the velocity's for y
      // and z.
      for (uint32_t k = 0; k < 3; k++) {
        const uint32_t velocity = guest::LoadU32(base, schema::OverlayParticle(base, particle).velocity_address() + 4 * k);
        const uint32_t position = guest::LoadU32(base, schema::OverlayParticle(base, particle).position_address() + 4 * k);
        guest::StoreU32(base, schema::OverlayParticle(base, particle).position_address() + 4 * k,
                        k == 0 ? KeepFirstNaN(position, velocity, true)
                               : KeepFirstNaN(velocity, position, true));
      }
      schema::OverlayParticle(base, particle).set_shown(1);
      if (self.region_on() == 1) {
        *r3 = SmokeInRegion(base, smoke, particle);
        if (*r3 != 0) {
          schema::OverlayParticle(base, particle).set_shown(0);
        }
      }
    }
    index = NextIndex(index, int32_t(self.capacity()));
    emitter = schema::SmokePuff(base, emitter_at(index));
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kParticlesBBytes = 176;

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
  const schema::ParticlesB self(base, effect);
  const schema::OverlayParams table = self.params();
  int32_t count = int32_t(self.count());
  if (count == 0) {
    return true;
  }
  int32_t index = int32_t(self.oldest());
  if (!EntriesInRange(self.entries_address(), kParticlesBBytes,
                      int32_t(self.capacity()), index)) {
    return false;
  }
  auto entry_at = [&](int32_t i) { return self.entries_address() + kParticlesBBytes * uint32_t(i); };
  schema::ParticlesBEntry entry(base, entry_at(index));
  // The oldest entry, when its life is out, is reset and dropped (the count
  // and head read again after the reset).
  if (!(int32_t(entry.life()) > 0)) {
    ParticlesBResetEntry(base, entry.address);
    count = int32_t(self.count() - 1);
    const int32_t head = int32_t(self.oldest());
    const int32_t capacity = int32_t(self.capacity());
    self.set_count(uint32_t(count));
    index = NextIndex(head, capacity);
    self.set_oldest(uint32_t(index));
    if (count != 0) {
      entry = schema::ParticlesBEntry(base, entry_at(index));
    }
  }
  if (!(count > 0)) {
    return true;
  }
  const double most_speed = G::particles_b_most_speed(base);
  const double least_speed = G::particles_b_least_speed(base);
  const double zero = G::zero(base);
  const double one = G::one(base);
  const uint32_t zero_word = guest::LoadU32(base, G::zero_address());
  const uint32_t one_word = std::bit_cast<uint32_t>(float(one));
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  for (; count > 0; count--) {
    const int32_t life_left = int32_t(entry.life() - 1);
    entry.set_life(uint32_t(life_left));
    if (life_left < 0) {
      entry.set_life(0);
    }
    // The size: the table's +4 plus (the table's life - life) / (twice the
    // table's life).
    const uint32_t most = table.life();
    const double size_from = table.size();
    const uint32_t life = entry.life();
    double size = float(double(FloatOf(int64_t(uint32_t(most - life)))) /
                        double(FloatOf(int64_t(uint32_t(most << 1)))));
    entry.set_size(float(size + size_from));
    // The fade: +112 times life / the table's +12 while life is below it.
    const uint32_t fade_ticks = table.fade_ticks();
    const uint32_t most_fade_word = guest::LoadU32(base, entry.most_alpha_address());
    const double most_fade = std::bit_cast<float>(most_fade_word);
    if (fade_ticks > life) {
      double fade = float(one / double(FloatOf(int64_t(fade_ticks))));
      fade = float(fade * double(FloatOf(int64_t(int32_t(life)))));
      entry.set_alpha(float(fade * most_fade));
    } else {
      guest::StoreU32(base, entry.alpha_address(), most_fade_word);
    }
    if (double(entry.alpha()) < zero) {
      guest::StoreU32(base, entry.alpha_address(), zero_word);
    }
    if (most_fade < double(entry.alpha())) {
      guest::StoreU32(base, entry.alpha_address(), most_fade_word);
    }
    entry.set_alpha_int(uint32_t(Fctiwz(double(entry.alpha()))));
    // Each particle's velocity (+16) is the effect's +2,800 scaled by the
    // table's +4 times its length plus a least, at most a most; then it
    // moves by it.
    for (int32_t i = 0; i < int32_t(entry.particle_count()); i++) {
      const schema::OverlayParticle particle(base, entry.particle_address() + 48 * uint32_t(i));
      const simde__m128 moved = LoadVector(base, self.moved_address());
      const simde__m128 length = Flushing(csr, flushing, Length, moved);
      const double scale = table.size();
      double speed = float(std::fma(scale, double(std::bit_cast<float>(WordOf(length, 0))),
                                    least_speed));
      if (speed > most_speed) {
        speed = most_speed;
      }
      const uint32_t vx = ScaledWord(base, self.moved_address() + 0, speed);
      const uint32_t vy = ScaledWord(base, self.moved_address() + 4, speed);
      const uint32_t vz = ScaledWord(base, self.moved_address() + 8, speed);
      guest::StoreU32(base, particle.velocity_address(), vx);
      guest::StoreU32(base, particle.velocity_address() + 4, vy);
      guest::StoreU32(base, particle.velocity_address() + 8, vz);
      guest::StoreU32(base, particle.velocity_address() + 12, one_word);
      // Where both are NaNs, the translated code (addss, seen in the
      // disassembly) keeps the velocity's.
      for (uint32_t k = 0; k < 3; k++) {
        guest::StoreU32(base, particle.position_address() + 4 * k,
                        KeepFirstNaN(guest::LoadU32(base, particle.velocity_address() + 4 * k),
                                     guest::LoadU32(base, particle.position_address() + 4 * k), true));
      }
    }
    index = NextIndex(index, int32_t(self.capacity()));
    entry = schema::ParticlesBEntry(base, entry_at(index));
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

bool ParticlesCUpdate(uint8_t* base, uint32_t effect, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8212C2F0 ParticlesC::Update");
  const schema::ParticlesC self(base, effect);
  const int32_t count = int32_t(self.count());
  const schema::OverlayParams table = self.params();
  if (count == 0) {
    return true;
  }
  // The capacity is read again for each division, but nothing here writes
  // it (the entries' fields stay off the effect's own).
  if (self.capacity() == 0) {
    return false;
  }
  auto entry_at = [&](uint32_t i) { return self.entries_address() + (i << 7); };
  const double zero = G::zero(base);
  const double one = G::one(base);
  const uint32_t zero_word = guest::LoadU32(base, G::zero_address());
  const uint32_t one_word = std::bit_cast<uint32_t>(float(one));

  // Each entry from the oldest ages and fades, and its brightness gets a
  // random 0 to 2 added.
  uint32_t index = self.oldest();
  if (count > 0) {
    const double speed_scale = G::particles_c_speed_scale(base);
    for (int32_t left = count; left > 0; left--) {
      const schema::ParticlesCEntry entry(base, entry_at(index));
      const int32_t life_left = int32_t(entry.life() - 1);
      entry.set_life(uint32_t(life_left));
      if (life_left < 0) {
        entry.set_life(0);
      }
      if (!(int32_t(entry.life()) > 0)) {
        entry.set_shown(0);
        const uint32_t next = self.oldest() + 1;
        const uint32_t capacity = self.capacity();
        const uint32_t remaining = self.count() - 1;
        self.set_count(remaining);
        self.set_oldest(next % capacity);
      }
      const uint32_t fade_ticks = table.fade_ticks();
      const uint32_t brightest_word = guest::LoadU32(base, table.alpha_address());
      double brightness = std::bit_cast<float>(brightest_word);
      const uint32_t life = entry.life();
      uint32_t brightness_word = brightest_word;
      if (fade_ticks > life) {
        double fade = float(one / double(FloatOf(int64_t(fade_ticks))));
        fade = float(fade * double(FloatOf(int64_t(int32_t(life)))));
        brightness = float(fade * brightness);
        brightness_word = std::bit_cast<uint32_t>(float(brightness));
      }
      guest::StoreU32(base, entry.alpha_address(), brightness_word);
      if (brightness < zero) {
        guest::StoreU32(base, entry.alpha_address(), zero_word);
      }
      const uint32_t most_word = guest::LoadU32(base, table.alpha_address());
      if (double(std::bit_cast<float>(most_word)) < double(entry.alpha())) {
        guest::StoreU32(base, entry.alpha_address(), most_word);
      }
      double speed = float(double(self.speed()) * speed_scale);
      if (speed > one) {
        speed = one;
      }
      entry.set_alpha(float(double(entry.alpha()) * speed));
      const int32_t drawn = int32_t(Rand(base));
      *r3 = uint32_t(drawn);
      const double added = FloatOf(int64_t(drawn % 3));
      const float shown = float(added + double(entry.alpha()));
      entry.set_alpha(shown);
      entry.set_alpha_int(uint32_t(Fctiwz(double(shown))));
      index = (index + 1) % self.capacity();
    }
  }

  // The newest entry takes the effect's position (+2,672), and each entry
  // before it, from the newest back, is pulled to within the effect's speed
  // (+2,704) times the table's +4 times a scale of the one after it.
  const int32_t pulls = int32_t(self.count() - 1);
  uint32_t capacity = self.capacity();
  uint32_t after = (self.newest() + capacity - 1) % capacity;
  uint32_t before = (capacity + after - 1) % capacity;
  {
    const uint32_t to = schema::ParticlesCEntry(base, entry_at(after)).position_address();
    guest::StoreU32(base, to + 0, guest::LoadU32(base, self.position_address() + 0));
    guest::StoreU32(base, to + 4, guest::LoadU32(base, self.position_address() + 4));
    guest::StoreU32(base, to + 8, guest::LoadU32(base, self.position_address() + 8));
    guest::StoreU32(base, to + 12, guest::LoadU32(base, self.position_address() + 12));
  }
  if (!(pulls > 0)) {
    return true;
  }
  const double pull_scale = G::particles_c_pull_scale(base);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  for (int32_t left = pulls; left > 0; left--) {
    const uint32_t a = schema::ParticlesCEntry(base, entry_at(after)).position_address();
    const uint32_t b = schema::ParticlesCEntry(base, entry_at(before)).position_address();
    const double ay = guest::LoadFloat(base, a + 4);
    const float dx = Difference(guest::LoadFloat(base, a + 0), guest::LoadFloat(base, b + 0));
    const float dy = Difference(ay, guest::LoadFloat(base, b + 4));
    const float dz = Difference(guest::LoadFloat(base, a + 8), guest::LoadFloat(base, b + 8));
    auto bits = [](float f) { return std::bit_cast<uint32_t>(f); };
    const simde__m128 apart = VectorOf(bits(dx), bits(dy), bits(dz), one_word);
    const simde__m128 length = Flushing(csr, flushing, Length, apart);
    const double distance = std::bit_cast<float>(WordOf(length, 0));
    if (!(distance == zero)) {
      double most = float(double(self.speed()) * pull_scale);
      most = float(most * double(table.size()));
      const double ax = guest::LoadFloat(base, a + 0);
      const double az = guest::LoadFloat(base, a + 8);
      const uint32_t ratio = bits(float(most / distance));
      const simde__m128 ratios = VectorOf(ratio, ratio, ratio, zero_word);
      // vmulfp with the difference's NaN kept where both are NaNs (the
      // translated code's mulps keeps its first operand's, seen in the
      // disassembly; the compiler may swap a plain mulps's operands).
      const simde__m128 product = Flushing(csr, flushing, Mul, apart, ratios);
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
    capacity = self.capacity();
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
  return schema::Vehicle(base, schema::OverlayView(base, view).vehicle()).body().address;
}

}  // namespace

uint32_t BodyMatrix(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_821268B0 OverlayView::BodyMatrix");
  return va::RigidBodyMatrix(Body(base, view));
}

uint32_t WheelContact(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821268C8 OverlayView::WheelContact");
  return va::VehicleWheelContact(schema::OverlayView(base, view).vehicle(), wheel) + 16;
}

uint32_t WheelSurfaceMarks(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821269B0 OverlayView::WheelSurfaceMarks");
  const uint32_t record = va::VehicleWheelContact(schema::OverlayView(base, view).vehicle(), wheel);
  // lha, then cmplwi: a negative surface compares as a large one.
  const uint32_t surface =
      uint32_t(int32_t(int16_t(schema::TrackPosition(base, record).surface_kind())));
  if (surface == 0) {
    return 1;
  }
  return (surface <= 2 || surface == 8) ? 0 : 1;
}

float WheelSlip(uint8_t* base, uint32_t view, uint32_t wheel, double threshold) {
  ITC_NATIVE_RUNNING("sub_82126A70 OverlayView::WheelSlip");
  return va::VehicleWheelSlip(base, schema::OverlayView(base, view).vehicle(), wheel, threshold);
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
  const uint32_t one = guest::LoadU32(base, G::one_address());
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
  const schema::CarOverlays self(base, overlays);
  const uint32_t pass = self.camera_pass();
  const uint32_t view = View(base, overlays, self.view_slot());
  const uint32_t matrix = BodyMatrix(base, view);
  const uint32_t one = guest::LoadU32(base, G::one_address());
  auto bits = [](float f) { return std::bit_cast<uint32_t>(f); };
  const double px = guest::LoadFloat(base, point + 0);
  const double py = guest::LoadFloat(base, point + 4);
  const double pz = guest::LoadFloat(base, point + 8);
  const simde__m128 from_eye =
      VectorOf(bits(Difference(px, schema::CameraPass(base, pass).eye_x())),
               bits(Difference(py, schema::CameraPass(base, pass).eye_y())),
               bits(Difference(pz, schema::CameraPass(base, pass).eye_z())), one);
  const simde__m128 from_car =
      VectorOf(bits(Difference(px, schema::VectorLibMatrix(base, matrix).m30())),
               bits(Difference(py, schema::VectorLibMatrix(base, matrix).m31())),
               bits(Difference(pz, schema::VectorLibMatrix(base, matrix).m32())), one);
  const uint32_t csr = Fpscr::getcsr();
  const simde__m128 dot =
      Flushing(csr, csr | uint32_t(Fpscr::FlushMask), Dot3, from_car, from_eye);
  return double(std::bit_cast<float>(WordOf(dot, 0))) < double(G::zero(base)) ? 0
                                                                                              : 1;
}

namespace {

// A save flag: the halfword at player 0's save data + `offset` + 12 x id
// (lhax, then cntlzw: any nonzero halfword, negative ones too, is 1).
uint32_t SaveFlag(uint8_t* base, uint32_t id, uint32_t offset) {
  if (id > 399) {
    return 0;
  }
  const uint32_t save = va::SaveDataOf(base, 0);
  return guest::LoadU16(base, save + offset + 12 * id) != 0 ? 1 : 0;
}

}  // namespace

uint32_t SaveFlag199FA(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82126BB0 SaveFlag199FA");
  return SaveFlag(base, id, 0x199FA);
}

uint32_t SaveFlag199F8(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82126C08 SaveFlag199F8");
  return SaveFlag(base, id, 0x199F8);
}

uint32_t SaveFlag199FC(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82126C58 SaveFlag199FC");
  return SaveFlag(base, id, 0x199FC);
}

uint32_t SaveFlag199F8Or199FC(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82126CB0 SaveFlag199F8Or199FC");
  if (id > 399) {
    return 0;
  }
  return (SaveFlag199F8(base, id) != 0 || SaveFlag199FC(base, id) != 0) ? 1 : 0;
}

uint32_t RacerInCurrentStage(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82126B50 RacerInCurrentStage");
  if (id > 399) {
    return 0;
  }
  const uint32_t race = G::race_data(base);
  const int32_t index = va::RacersIndexOf(base, race + 48, id);
  return va::StagesHasRacer(base, race + 0x59620, uint32_t(index), 0xFFFFFFFFu);
}

bool UpdateTrails(uint8_t* base, uint32_t overlays, uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_82124EE0 CarOverlays::UpdateTrails");
  const schema::CarOverlays self(base, overlays);
  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const int32_t capacity =
        int32_t(schema::Trail(base, LoadWord(self, self.trails_address() + 4 * wheel)).capacity());
    if (capacity == 0 || capacity == -1) {
      return false;
    }
  }
  const schema::CarOverlaysParams table = self.params();
  const uint32_t view = View(base, overlays, self.view_slot());
  if (va::DebugSwitchOff() != 0) {
    return true;
  }
  const uint32_t zero = guest::LoadU32(base, G::zero_address());
  const uint32_t one = guest::LoadU32(base, G::one_address());
  const double zero_value = std::bit_cast<float>(zero);
  // The direction passed to Trail::AddPoint, (0, 1, 0, 0). The original
  // puts it back to the same when its length is 0.0, which it never is.
  const uint32_t direction[4] = {zero, one, zero, zero};
  const int32_t mode = int32_t(self.view_mode());
  uint32_t value = (mode == 1 || mode == 2) ? 0 : 1;
  const double width_scale = G::frame_scale(base);
  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const uint32_t trail = LoadWord(self, self.trails_address() + 4 * wheel);
    const schema::CarOverlaysWheel counter = self.wheels(wheel);
    uint32_t due = 0;
    const float axle = AxleValue3384(base, view, wheel);
    if (WheelOnGround(base, view, wheel) != 0 && WheelSurfaceMarks(base, view, wheel) != 0 &&
        double(WheelSlip(base, view, wheel, table.trail_slip_threshold())) != zero_value) {
      uint32_t count = counter.trail_counter();
      if (!(table.trail_least_count() > count) &&
          !(count > table.trail_most_count())) {
        due = 1;
      }
      const uint32_t most = table.trail_most_count() << 1;
      count = count + 1;
      counter.set_trail_counter(count > most ? most : count);
    } else {
      counter.set_trail_counter(0);
    }

    // The point: the contact position plus the offset, then the lift.
    const uint32_t contact = WheelContact(base, view, wheel);
    auto sum = [&](uint32_t k) {
      return std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, contact + 4 * k)) +
                                           double(guest::LoadFloat(base, G::contact_offset_address() + 4 * k))));
    };
    const simde__m128 at = VectorOf(sum(0), sum(1), sum(2), one);
    const uint32_t csr = Fpscr::getcsr();
    const simde__m128 point = Flushing(csr, csr | uint32_t(Fpscr::FlushMask), Add, at,
                                       LoadVector(base, G::trail_lift_address()));
    for (int k = 0; k < 4; k++) {
      guest::StoreU32(base, scratch + 4 * k, WordOf(point, k));
    }

    if (due != 0) {
      const uint32_t state = counter.trail_added() != 0 ? 2 : 1;
      if (wheel >= 2) {
        value = 1;
      }
      const double speed = SpeedKmh(base, view);
      const double width = float(double(axle) * width_scale);
      uint32_t added = 0;
      TrailAddPoint(base, trail, scratch, speed, width, direction, state, value, &added);
      if (added == 0) {
        due = 0;
      }
    }
    counter.set_trail_added(due);
    TrailSetLastPosition(base, trail, scratch);
  }
  return true;
}

uint32_t NoSaveFlags(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82127198 OverlayView::NoSaveFlags");
  if (id > 399 || SaveFlag199FA(base, id) != 0) {
    return 0;
  }
  return SaveFlag199F8Or199FC(base, id) == 0 ? 1 : 0;
}

uint32_t FlagsKind(uint8_t* base, uint32_t overlays, uint32_t id, int32_t a, int32_t b,
                   int32_t c, int32_t d) {
  ITC_NATIVE_RUNNING("sub_82122DB8 CarOverlays::FlagsKind");
  const schema::CarOverlays self(base, overlays);
  View(base, overlays, self.view_slot());
  if (NoSaveFlags(base, id) != 0) {
    return 0;
  }
  int32_t ids[4] = {a, b, c, d};
  const int count = c == -1 ? 2 : d == -1 ? 3 : 4;
  for (int i = 0; i < count; i++) {
    if (SaveFlag199FA(base, uint32_t(ids[i])) == 0) {
      return 1;
    }
  }
  return 2;
}

uint32_t MarkerKind(uint8_t* base, uint32_t overlays, int32_t id, uint32_t fallback) {
  ITC_NATIVE_RUNNING("sub_82122EA8 CarOverlays::MarkerKind");
  const schema::CarOverlays self(base, overlays);
  int32_t first = 0, last = 0;
  if (id == 67 || id == 68 || id == 374 || id == 375) {
    first = id <= 68 ? 67 : 374;
    return FlagsKind(base, overlays, uint32_t(id), first, first + 1, -1, -1);
  }
  if (id >= 107 && id <= 109) {
    first = 107, last = 109;
  } else if (id >= 111 && id <= 114) {
    first = 111, last = 114;
  } else if (id >= 248 && id <= 250) {
    first = 248, last = 250;
  } else if (id >= 304 && id <= 306) {
    first = 304, last = 306;
  } else if (id >= 383 && id <= 385) {
    first = 383, last = 385;
  } else if (id >= 393 && id <= 395) {
    first = 393, last = 395;
  } else {
    return fallback;
  }
  View(base, overlays, self.view_slot());
  if (NoSaveFlags(base, uint32_t(id)) != 0) {
    return 0;
  }
  for (int32_t flag = first; flag <= last; flag++) {
    if (SaveFlag199FA(base, uint32_t(flag)) == 0) {
      return 1;
    }
  }
  return 2;
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

// The model point `index` in world space at frame + 80, as the effects'
// updates place it: the body matrix copied to frame + 112, the point's
// position to frame + 96, then copied to frame + 80 (ld and std), its x less
// `shift` (fsubs) when given, and transformed by the matrix in place
// (Vec4::Transform, with its override's checks around it).
void WorldPoint(uint8_t* base, uint32_t view, int32_t index, uint32_t frame,
                const double* shift) {
  BodyMatrixCopy(base, frame + 112, view);
  const uint32_t position = OverlayViewPointPosition(base, frame + 96, view, index);
  CopyDoublewords(base, position, frame + 80);
  if (shift) {
    guest::StoreFloat(base, frame + 80,
                      Difference(guest::LoadFloat(base, frame + 80), *shift));
  }
  debug::Vec4Transform(base, frame + 80, frame + 112, frame + 80);
}

}  // namespace

float VehicleFloat2508(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126A90 OverlayView::VehicleFloat2508");
  return schema::Vehicle(base, schema::OverlayView(base, view).vehicle()).backfire_strength();
}

void LightSpriteShow(uint8_t* base, uint32_t sprite, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8211EAA0 LightSprite::Show");
  const schema::OverlaySpriteEntry at(base, schema::LightSprite(base, sprite).sprite_address());
  const schema::OverlayParams table = schema::LightSprite(base, sprite).params();
  CopyVector(base, position, at.position_address());
  at.set_shown(1);
  CopyFloat(base, table.size_address(), at.size_address());
  CopyFloat(base, table.alpha_address(), at.alpha_address());
  const uint32_t life = table.life();
  at.set_kind(0);
  at.set_life(life);
  const int32_t r = int32_t(Rand(base));
  const int32_t alpha = Fctiwz(at.alpha());
  at.set_unknown_116(uint32_t(r - (r / 4) * 4));
  at.set_red(255);
  at.set_green(255);
  at.set_blue(255);
  at.set_alpha_int(uint32_t(alpha));
}

void UpdateLightSprites(uint8_t* base, uint32_t overlays, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821251C0 CarOverlays::UpdateLightSprites");
  const schema::CarOverlays self(base, overlays);
  const uint32_t view = View(base, overlays, self.view_slot());
  const uint32_t word = VehicleWord2252(base, view);
  CopyFloat(base, G::one_address(), frame + 92);
  if (va::DebugSwitchOff() != 0) {
    return;
  }
  const double shift = G::axle_scale(base);
  for (int32_t i = 0; i < 4; i++) {
    const uint32_t sprite = LoadWord(self, self.light_sprites_address() + 4 * uint32_t(i));
    if (OverlayViewHasPoint(base, view, 4 + i) == 0) {
      continue;
    }
    WorldPoint(base, view, 4 + i, frame, &shift);
    if (VehicleByte2504(base, view) != 0) {
      VehicleFloat2508(base, view);  // left in f1, which LightSprite::Show doesn't read
      LightSpriteShow(base, sprite, frame + 80);
    }
    LightSpriteSetPosition(base, sprite, frame + 80);
  }
  self.set_vehicle_word_2252(word);
}

bool UpdateParticlesB(uint8_t* base, uint32_t overlays, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821252D0 CarOverlays::UpdateParticlesB");
  const schema::CarOverlays self(base, overlays);
  for (uint32_t i = 0; i < 4; i++) {
    const int32_t capacity =
        int32_t(schema::ParticlesB(base, LoadWord(self, self.particles_b_address() + 4 * i)).capacity());
    if (capacity == 0 || capacity == -1) {
      return false;
    }
  }
  const uint32_t view = View(base, overlays, self.view_slot());
  CopyFloat(base, G::one_address(), frame + 92);
  if (va::DebugSwitchOff() != 0) {
    return true;
  }
  const double shift = G::axle_scale(base);
  for (int32_t i = 0; i < 4; i++) {
    const uint32_t effect = LoadWord(self, self.particles_b_address() + 4 * uint32_t(i));
    if (OverlayViewHasPoint(base, view, 4 + i) == 0) {
      continue;
    }
    WorldPoint(base, view, 4 + i, frame, &shift);
    // The speed and +6252 go in f3 and f2, which ParticlesB::Emit doesn't
    // read; +6256, the last read, is its f1.
    SpeedKmh(base, view);
    VehicleFloat6252(base, view);
    const double factor = VehicleFloat6256(base, view);
    uint32_t result = 0;
    double sine = factor;
    ParticlesBEmit(base, effect, frame + 80, factor, &result, &sine);
    ParticlesBSetPosition(base, effect, frame + 80);
  }
  return true;
}

void UpdateParticlesC(uint8_t* base, uint32_t overlays, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82125798 CarOverlays::UpdateParticlesC");
  const schema::CarOverlays self(base, overlays);
  const uint32_t view = View(base, overlays, self.view_slot());
  CopyFloat(base, G::one_address(), frame + 92);
  for (int32_t i = 0; i < 4; i++) {
    const uint32_t effect = LoadWord(self, self.particles_c_address() + 4 * uint32_t(i));
    if (OverlayViewHasPoint(base, view, 14 + i) == 0) {
      continue;
    }
    WorldPoint(base, view, 14 + i, frame, nullptr);
    if (IsBehindCamera(base, overlays, frame + 80) == 0) {
      ParticlesCEmit(base, effect, frame + 80);
    }
    const double speed = SpeedKmh(base, view);
    ParticlesCSetPosition(base, effect, frame + 80, speed);
  }
}

}  // namespace race_overlays

namespace race_overlays {

namespace {

constexpr uint32_t kSmokeEmitterBytes = 720;
constexpr uint32_t kSmokeFrameCopies = 0x10000;  // more emitters than this: the original runs

// Whether Smoke::SetFrame's memcpy of each of the smoke's emitters (+144,
// 64 bytes from `frame`) is the library's plain copy (CopyLikeMemcpy's
// cases: below the physical views, not sharing memory with the frame).
bool SmokeFrameCopiesPlain(uint8_t* base, uint32_t smoke, uint32_t frame) {
  const uint32_t count = schema::Smoke(base, smoke).capacity();
  if (count > kSmokeFrameCopies) {
    return false;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t to =
        schema::SmokePuff(base, schema::Smoke(base, smoke).puffs_address() + kSmokeEmitterBytes * i)
            .frame_address();
    if (to >= 0xA0000000u || c_runtime::MayShare(to, 64, frame, 64)) {
      return false;
    }
  }
  return true;
}

// Whether CarOverlays::EmitSmoke would decline for the wheel (its smoke's
// capacity 0 or -1, or its accumulator the capacity word).
bool EmitSmokeDeclines(uint8_t* base, uint32_t overlays, uint32_t wheel) {
  const schema::CarOverlays self(base, overlays);
  const uint32_t accumulator = self.smoke_rates_address() + wheel * 48;
  const uint32_t smoke = LoadWord(self, self.smokes_address() + (wheel << 2));
  const schema::Smoke s(base, smoke);
  const int32_t capacity = int32_t(s.capacity());
  return capacity == 0 || capacity == -1 ||
         (accumulator + 4 > s.capacity_address() && accumulator < s.texture_holder_address());
}

// The wheel's contact record's surface (the halfword +80, sign-extended).
int32_t WheelSurface(uint8_t* base, uint32_t view, uint32_t wheel) {
  const uint32_t record = va::VehicleWheelContact(schema::OverlayView(base, view).vehicle(), wheel);
  return int16_t(guest::LoadU16(base, record + 80));
}

// The rate draw UpdateSmoke makes three times: RandomRange(a x v, v), and
// `a` itself unless it's below that draw, where a second draw from a x v
// to v replaces it (the first draw is only compared). `a` is reloaded from
// the table after the draw, as the original does.
double SmokeDraw(uint8_t* base, uint32_t at, double v, bool value_first) {
  const double a = guest::LoadFloat(base, at);
  const double low = value_first ? double(float(v * a)) : double(float(a * v));
  const double drawn = RandomRange(base, low, v, nullptr);
  const double again = guest::LoadFloat(base, at);
  if (again < drawn) {
    return RandomRange(base, double(float(again * v)), v, nullptr);
  }
  return again;
}

// The size UpdateSmoke stores at the frame's +192: the translated code keeps
// it as a double across branches, so a signaling NaN from the table comes
// back quieted.
void StoreSmokeSize(uint8_t* base, uint32_t at, double size) {
  uint32_t word = std::bit_cast<uint32_t>(float(size));
  if (std::isnan(float(size))) {
    word |= 0x00400000;
  }
  guest::StoreU32(base, at, word);
}

// x clamped to `most` (NaNs as they are), times `scale`, and 0.0 (the
// game's) below 0.0.
double ClampedScaled(double x, double most, double scale, double zero) {
  const double a = std::fabs(x);
  const double scaled = float((a > most ? most : a) * scale);
  if (scaled < zero) {
    return zero;
  }
  return float((a > most ? most : a) * scale);
}

}  // namespace

bool SmokeSetFrame(uint8_t* base, uint32_t smoke, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8212B1B0 Smoke::SetFrame");
  if (!SmokeFrameCopiesPlain(base, smoke, frame)) {
    return false;
  }
  for (uint32_t i = 0; i < schema::Smoke(base, smoke).capacity(); i++) {
    const schema::SmokePuff emitter(base, schema::Smoke(base, smoke).puffs_address() + kSmokeEmitterBytes * i);
    for (uint32_t k = 0; k < 4; k++) {
      CopyFloat(base, frame + 64 + 4 * k, emitter.frame_position_address() + 4 * k);
    }
    c_runtime::Memcpy(base, emitter.frame_address(), frame, 64);
    CopyFloat(base, frame + 80, emitter.frame_value_address());
  }
  return true;
}

uint32_t WheelOnSurface8(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126A00 OverlayView::WheelOnSurface8");
  return WheelSurface(base, view, wheel) == 8 ? 1 : 0;
}

uint32_t WheelOnSurface1Or2(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126A38 OverlayView::WheelOnSurface1Or2");
  return uint32_t(WheelSurface(base, view, wheel) - 1) <= 1 ? 1 : 0;
}

bool UpdateSmoke(uint8_t* base, uint32_t overlays, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82125B60 CarOverlays::UpdateSmoke");
  const schema::CarOverlays self(base, overlays);
  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const uint32_t smoke = LoadWord(self, self.smokes_address() + 4 * wheel);
    if (EmitSmokeDeclines(base, overlays, wheel) ||
        !SmokeFrameCopiesPlain(base, smoke, frame + 112)) {
      return false;
    }
  }
  const schema::CarOverlaysParams table = self.params();
  const uint32_t view = View(base, overlays, self.view_slot());
  const uint32_t one = guest::LoadU32(base, G::one_address());
  const double scale_off =
      va::DebugSwitchOff() != 0 ? double(table.debug_rate_scale())
                                : double(G::one(base));
  for (uint32_t at : {124u, 140u, 156u, 172u, 188u}) {
    guest::StoreU32(base, frame + at, one);
  }
  const uint32_t zero_word = guest::LoadU32(base, G::zero_address());
  const double zero = std::bit_cast<float>(zero_word);

  // The car's two axes: half the front contacts' difference (wheels 2 and
  // 3) at +80 and the rear's (0 and 1) at +96, w 1.0; then each plus a
  // contact (3 and 1), vaddfp with flush-to-zero on.
  const double half = G::half(base);
  auto axis = [&](uint32_t to, uint32_t a_wheel, uint32_t b_wheel) {
    const uint32_t b = WheelContact(base, view, b_wheel);
    const uint32_t a = WheelContact(base, view, a_wheel);
    float d[3];
    for (uint32_t k = 0; k < 3; k++) {
      d[k] = Difference(guest::LoadFloat(base, a + 4 * k), guest::LoadFloat(base, b + 4 * k));
    }
    guest::StoreU32(base, to + 12, one);
    for (uint32_t k = 0; k < 3; k++) {
      guest::StoreFloat(base, to + 4 * k, float(double(d[k]) * half));
    }
  };
  axis(frame + 80, 2, 3);
  axis(frame + 96, 0, 1);
  const uint32_t csr = Fpscr::getcsr();
  const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
  AddVectorTo(base, frame + 80, WheelContact(base, view, 3), csr, flushing);
  AddVectorTo(base, frame + 96, WheelContact(base, view, 1), csr, flushing);
  if (va::DebugSwitchOff() != 0) {
    return true;
  }

  const int32_t mode = int32_t(self.view_mode());
  const uint32_t value = (mode == 1 || mode == 2) ? 0 : 1;
  for (uint32_t at : {176u, 180u, 184u}) {
    guest::StoreU32(base, frame + at, zero_word);
  }
  guest::StoreU32(base, frame + 188, one);
  guest::StoreU32(base, frame + 192, one);
  const double spin_scale = G::smoke_spin_scale(base);
  const double spin_most = G::smoke_spin_most(base);
  const double slip_scale = G::smoke_slip_scale(base);
  const double slip_most = G::smoke_slip_most(base);
  guest::StoreU32(base, frame + 220, one);
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow(base, frame + 112 + 16 * row, row);
  }

  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const schema::CarOverlaysWheel counter = self.wheels(wheel);
    const double slip = ClampedScaled(
        WheelSlip(base, view, wheel, table.smoke_slip_threshold()), slip_most, slip_scale,
        zero);
    double rate = ClampedScaled(WheelSpin(base, view, wheel), spin_most, spin_scale, zero);
    if (!(slip < rate)) {
      rate = slip;
    }
    const uint32_t smoke = LoadWord(self, self.smokes_address() + 4 * wheel);
    const float axle = AxleValue3764(base, view, wheel);
    const uint32_t contact = WheelContact(base, view, wheel);
    for (uint32_t k = 0; k < 3; k++) {
      guest::StoreFloat(base, frame + 208 + 4 * k,
                        float(double(guest::LoadFloat(base, contact + 4 * k)) +
                              double(guest::LoadFloat(base, G::contact_offset_address() + 4 * k))));
    }
    const uint64_t first = LoadU64(base, frame + 208);
    const uint64_t second = LoadU64(base, frame + 216);
    StoreU64(base, frame + 176, first);
    StoreU64(base, frame + 184, second);
    const double lift = WheelValue512(base, view, wheel);
    const double y = guest::LoadFloat(base, frame + 180);
    guest::StoreFloat(base, frame + 196, axle);
    guest::StoreFloat(base, frame + 180, float(lift + y));
    SmokeSetFrame(base, smoke, frame + 112);
    SmokeSetRegion(base, smoke, frame + 80, frame + 96, self.view_mode());

    double last = 0.0;
    if (WheelOnGround(base, view, wheel) != 0) {
      double size = zero;
      if (WheelSurfaceMarks(base, view, wheel) != 0) {
        const double base_rate = float(double(table.marks_rate_slip()) * slip);
        size = SmokeDraw(base, table.marks_size_address(), slip, false);
        if (slip != zero) {
          const double speed = SpeedKmh(base, view);
          const double emit = float(double(table.marks_rate()) + base_rate);
          EmitSmoke(base, overlays, emit, speed, frame + 112, wheel, value, &last);
        }
      } else if (WheelOnSurface8(base, view, wheel) != 0) {
        const double emit =
            float(double(float(double(table.surface8_rate()) * rate)) * scale_off);
        size = SmokeDraw(base, table.surface8_size_address(), rate, false);
        const double speed = SpeedKmh(base, view);
        EmitSmoke(base, overlays, emit, speed, frame + 112, wheel, value, &last);
        const int32_t count = int32_t(counter.smoke_counter());
        if (count < 25) {
          counter.set_smoke_counter(uint32_t(count + 1));
        }
      } else if (WheelOnSurface1Or2(base, view, wheel) != 0) {
        const double emit =
            float(double(float(double(table.surface12_rate()) * rate)) * scale_off);
        size = SmokeDraw(base, table.surface12_size_address(), rate, true);
        const double speed = SpeedKmh(base, view);
        EmitSmoke(base, overlays, emit, speed, frame + 112, wheel, value, &last);
      }
      StoreSmokeSize(base, frame + 192, size);
    }

    if (int32_t(counter.smoke_counter()) > 0 &&
        (WheelSurfaceMarks(base, view, wheel) != 0 || WheelOnGround(base, view, wheel) == 0)) {
      const double size = SmokeDraw(base, table.surface8_size_address(), rate, false);
      const double emit =
          float(double(float(double(table.counter_rate()) * rate)) * scale_off);
      const double speed = SpeedKmh(base, view);
      EmitSmoke(base, overlays, emit, speed, frame + 112, wheel, value, &last);
      const uint32_t count = counter.smoke_counter();
      StoreSmokeSize(base, frame + 192, size);
      // The count less EmitSmoke's r3 (0) and 1, 0 when that's negative.
      const uint32_t next = count - 1;
      counter.set_smoke_counter(int32_t(next) < 0 ? 0 : next);
    }
    SmokeSetVelocity(base, smoke, frame + 176);
  }
  return true;
}

}  // namespace race_overlays

namespace race_overlays {

bool ModelMatrix(uint8_t* base, uint32_t out, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_821270E8 OverlayView::ModelMatrix");
  const uint32_t from = schema::OverlayView(base, view).model() + 4288 + 16;
  if (out >= 0xA0000000u || c_runtime::MayShare(out, 64, from, 64)) {
    return false;
  }
  const uint32_t one = guest::LoadU32(base, G::one_address());
  for (uint32_t row = 0; row < 4; row++) {
    guest::StoreU32(base, out + 16 * row + 12, one);
  }
  c_runtime::Memcpy(base, out, from, 64);
  return true;
}

}  // namespace race_overlays
