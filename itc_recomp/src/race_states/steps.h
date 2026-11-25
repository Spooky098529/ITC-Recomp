// The race's states' shared steps (src/race_states/): guest words, the
// route's section records and links, fctiwz's word, packed times. Float
// branches follow the translation, not the PowerPC: its bge is !lt and its
// ble !gt, so an unordered compare (a NaN) takes them.

#pragma once

#include "guest.h"
#include "race_world/race_world.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "schema/game_modes.h"
#include "schema/minimap.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/track_route.h"
#include "schema/vehicle.h"
#include "schema/vehicle_accessors.h"

#include <bit>
#include <cmath>
#include <cstdint>

namespace race_states::steps {

constexpr uint32_t kEntryBytes = 972;  // a results entry (the progress's +2188 + 972 i)
constexpr uint32_t kLapRecordBytes = 76;  // a results entry's lap record (+20 + 76 i)
constexpr uint32_t kRecordBytes = 3936;  // a racer record (the race data's +48 + 3936 i)

// Other areas' objects the steps reach.
using RacerEntry = schema::MinimapRacerEntry;  // a racers' manager entry (minimap.toml)
using Manager = schema::RaceManager;           // the racers' manager (race-world.toml)
using VehicleView = schema::Vehicle;           // a vehicle (car-physics.toml)
using CarStatus = schema::VehicleStatus;       // a vehicle's +6160 (vehicle.toml)
using TrackPos = schema::TrackPosition;        // Vehicle::Identity's (collision-world.toml)
using RouteFile = schema::TrackRouteFile;      // the route's file (track-route.toml)
using RouteSection = schema::TrackRouteSection;
using RouteLink = schema::TrackRouteLink;
using RouteSlice = schema::TrackRouteSlice;

inline uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// A doubleword (big-endian, as std and ld).
inline uint64_t Load64(uint32_t address) {
  return uint64_t(Load(address)) << 32 | Load(address + 4);
}
inline void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
inline int32_t S16(uint32_t address) {
  return int16_t(guest::LoadU16(address));
}

// fctiwz's low word, as the translation leaves it (stfiwx).
inline uint32_t FctiwzWord(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT32_MAX)) {
    return uint32_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}
// fctidz's low word, as the translation leaves it (stfiwx).
inline uint32_t FctidzWord(double value) {
  if (std::isnan(value)) {
    return 0;
  }
  if (value > 9223372036854775808.0) {
    return 0xFFFFFFFFu;  // LLONG_MAX
  }
  if (value >= 9223372036854775808.0 || value < -9223372036854775808.0) {  // indefinite
    return 0;
  }
  return uint32_t(uint64_t(int64_t(value)));
}

// A route section's record: the route file's table (+12) at the section's
// low 16 bits (the originals' rlwinm by 2 into bits 14-29).
inline uint32_t SectionRecord(uint32_t section) {
  const uint32_t file = schema::RaceStatesGlobals::route();
  return Load(RouteFile(file).sections() + ((section << 2) & 0x3FFFC));
}
// A section's link `link` (a signed halfword at +28 + 4 x link).
inline int32_t Link(uint32_t record, uint32_t link) {
  return S16(RouteLink(RouteSection(record).links_address() + (link << 2)).section_address());
}

// A packed time unpacked as the originals do (rotations, subfic and
// subfe for the sign, mullw): the signed milliseconds, sign-extended.
inline int64_t Unpack(uint32_t w) {
  const uint64_t seconds = std::rotl(w, 22) & 0x3F;
  const uint64_t minutes = std::rotl(w, 16) & 0x7FFF;
  const uint64_t ms = (minutes * 60 + seconds) * 1000 + (w & 0x3FF);
  const int32_t sign = (w & 0x80000000u) != 0 ? -1 : 1;
  return int64_t(int32_t(uint32_t(ms))) * int64_t(sign);
}

// The screen fades are done: their in and out counts (0x826A50DC,
// 0x826A50E4) both 0.
inline bool FadesDone() {
  using G = schema::RaceStatesGlobals;
  return (G::fade_out_count() | G::fade_in_count()) == 0;
}

// A message posted to every machine: appended to the dispatcher's ring
// (+204: 50 entries of 12 bytes, +0 the message, +4 and +8 0; +804 the
// count, +808 the start), as the handlers write it inline.
inline void Post(uint32_t dispatcher, uint32_t message) {
  const schema::RaceStatesDispatcher d(dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t at = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const schema::ModeMessage entry((at - at / 50 * 50) * 12 + d.ring_address());
  entry.set_id(message);
  entry.set_arg0(0);
  entry.set_arg1(0);
}

// The race data's kind (+0xA3AF0).
inline uint32_t Kind() {
  return schema::RaceStatesGlobals::race_data().kind();
}

}  // namespace race_states::steps
