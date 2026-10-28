// The race's states' shared steps (src/race_states/): guest words, the
// route's section records and links, fctiwz's word, packed times. Float
// branches follow the translation, not the PowerPC: its bge is !lt and its
// ble !gt, so an unordered compare (a NaN) takes them.

#pragma once

#include "guest.h"
#include "race_world/race_world.h"

#include <bit>
#include <cmath>
#include <cstdint>

namespace race_states::steps {

constexpr uint32_t kEntries = 2188;  // the results entries, 972 bytes each
constexpr uint32_t kEntryBytes = 972;
constexpr uint32_t kSlotBytes = 536;

inline uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// A doubleword (big-endian, as std and ld).
inline uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
}
inline void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
inline int32_t S16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
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
inline uint32_t SectionRecord(uint8_t* base, uint32_t section) {
  const uint32_t file = Load(base, race_world::kRoute);
  return Load(base, Load(base, file + 12) + ((section << 2) & 0x3FFFC));
}
// A section's link `link` (a signed halfword at +28 + 4 x link).
inline int32_t Link(uint8_t* base, uint32_t record, uint32_t link) {
  return S16(base, record + ((link + 7) << 2));
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
inline bool FadesDone(uint8_t* base) {
  return (Load(base, 0x826A50E4) | Load(base, 0x826A50DC)) == 0;
}

// A message posted to every machine: appended to the dispatcher's ring
// (+204: 50 entries of 12 bytes, +0 the message, +4 and +8 0; +804 the
// count, +808 the start), as the handlers write it inline.
inline void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t at = Load(base, ring + 604) + count - 1;
  Store(base, ring + 600, count);
  const uint32_t entry = (at - at / 50 * 50) * 12 + ring;
  Store(base, entry + 0, message);
  Store(base, entry + 4, 0);
  Store(base, entry + 8, 0);
}

// The race data's flags (+0x59AE4 of [0x826C5B2C]).
inline uint32_t FlagsAddress(uint8_t* base) {
  return Load(base, race_world::kRaceData) + 0x59AE4;
}
// The race data's kind (+0xA3AF0).
inline uint32_t Kind(uint8_t* base) {
  return Load(base, Load(base, race_world::kRaceData) + 0xA3AF0);
}

}  // namespace race_states::steps
