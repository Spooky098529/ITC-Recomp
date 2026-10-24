// A random course's route for the harnesses (from the course's route's,
// src/harnesses/track_route.cpp, shared with the AI drivers'): in a block
// from the allocation layer, the route (+0: its file), the file (+0x40: the
// sections' count, 1 to 8, the slice length, the table), 8 sections (+0x100,
// 1,000 bytes apart), each 1 to 8 slices with random records, bounds, links,
// lane bytes and point tables (NaN, infinite and zero floats among them),
// up to +0x2040. The table is a block of its own with all 65,536 entries
// (entry i the section i modulo 8), so every section number a link or a
// position names is a section.

#pragma once

#include "harness.h"

#include <bit>
#include <cstdint>

namespace harness::test_route {

constexpr uint32_t kFile = 0x40, kSections = 0x100, kSectionStride = 1000;
constexpr uint32_t kTableEntries = 0x10000;
// The bytes the route takes in its block.
constexpr uint32_t kRouteBytes = kSections + 8 * kSectionStride;

// A float: now and then NaN (any sign, payload), an infinity or a zero.
inline uint32_t RandomFloat(Random& r, float range) {
  switch (r.Below(32)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x803FFFFFu) | 0x00400000u;
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    default: {
      const float unit = float(r.Below(1u << 24)) / float(1u << 24);
      return std::bit_cast<uint32_t>((unit * 2.0f - 1.0f) * range);
    }
  }
}

struct Route {
  uint32_t sections = 0;  // the file's count
  uint32_t slices[8] = {};
  uint32_t address[8] = {};
  uint32_t Slices(uint32_t section) const { return slices[section % 8]; }
  uint32_t Address(uint32_t section) const { return address[section % 8]; }
};

// The table (see the top), filled once.
inline void FillTable(uint8_t* base, uint32_t table, uint32_t area) {
  for (uint32_t i = 0; i < kTableEntries; i++) {
    guest::StoreU32(base, table + 4 * i, area + kSections + kSectionStride * (i % 8));
  }
}

// A random route in the block (see the top).
// `whole`: every section 8 slices and every link's slice within them
// (PathDistance's cases: its slice steps end only where they meet).
inline Route BuildRoute(uint8_t* base, uint32_t area, uint32_t table, Random& r, bool whole = false) {
  Route route;
  route.sections = 1 + r.Below(8);
  guest::StoreU32(base, area + 0, area + kFile);
  guest::StoreU32(base, area + kFile + 0, route.sections);
  guest::StoreU32(base, area + kFile + 4, uint32_t(r.Next()));
  guest::StoreU32(base, area + kFile + 8,
                  r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                   : std::bit_cast<uint32_t>(0.5f + float(r.Below(80)) / 8.0f));
  guest::StoreU32(base, area + kFile + 12, table);
  for (uint32_t s = 0; s < 8; s++) {
    const uint32_t at = area + kSections + kSectionStride * s;
    const uint32_t n = whole ? 8 : 1 + r.Below(8);
    route.slices[s] = n;
    route.address[s] = at;
    guest::StoreU16(base, at + 0, uint16_t(n));
    // The kind: 0 to 5 mostly (the low four bits), high bits random.
    guest::StoreU8(base, at + 2, uint8_t((r.Below(8) << 4) | (r.Below(4) ? r.Below(6) : r.Below(16))));
    // The branch number: 7, 8, 13 and 29 often, now and then above 31.
    const uint32_t branches[] = {7, 8, 13, 29};
    guest::StoreU8(base, at + 3, uint8_t(r.Below(3) == 0 ? branches[r.Below(4)]
                                         : r.Below(8) == 0 ? r.Below(256)
                                                           : r.Below(32)));
    for (uint32_t i = 0; i < 6; i++) {
      guest::StoreU32(base, at + 4 + 4 * i, RandomFloat(r, 200.0f));
    }
    // The links: -1 a third of the time, else a section (now and then
    // beyond the count, or above 0x8000) and a slice.
    for (uint32_t i = 0; i < 12; i++) {
      const bool none = r.Below(3) == 0;
      guest::StoreU16(base, at + 28 + 4 * i,
                      none ? 0xFFFF
                           : uint16_t(r.Below(4)    ? r.Below(route.sections)
                                      : r.Below(2) ? r.Below(16)
                                                   : r.Below(0xFFFF)));
      guest::StoreU16(base, at + 30 + 4 * i,
                      uint16_t(whole ? int32_t(r.Below(8)) : int32_t(r.Below(12)) - 2));
    }
    for (uint32_t i = 0; i < 48; i++) {
      guest::StoreU8(base, at + 76 + i, uint8_t(r.Below(4) == 0 ? 0xFF : r.Below(8)));
    }
    for (uint32_t k = 0; k < n; k++) {
      const uint32_t record = at + 124 + 84 * k;
      for (uint32_t i = 0; i < 84; i += 4) {
        guest::StoreU32(base, record + i, RandomFloat(r, 100.0f));
      }
      guest::StoreU32(base, record + 12,
                      r.Below(16) == 0 ? RandomFloat(r, 10.0f)
                                       : std::bit_cast<uint32_t>(1.0f + float(r.Below(40)) / 4.0f));
      guest::StoreU16(base, record + 32, uint16_t(r.Below(3)));
      guest::StoreU16(base, record + 34, uint16_t(r.Next()));
      guest::StoreU8(base, record + 56, uint8_t((r.Below(16) << 4) | (r.Below(4) ? r.Below(5)
                                                                                 : r.Below(16))));
      const uint8_t kinds[] = {0, 3, 4};
      guest::StoreU8(base, record + 57, uint8_t(r.Below(4) ? kinds[r.Below(3)] : r.Below(256)));
    }
    for (uint32_t i = 0; i < 24 * n; i += 4) {
      guest::StoreU32(base, at + 124 + 84 * n + i, RandomFloat(r, 100.0f));
    }
  }
  return route;
}

// A position: a section of the route (now and then one of the table's
// others) and a slice in it, now and then a few past either end.
inline void RandomPosition(uint8_t* base, uint32_t address, const Route& route, Random& r,
                    uint32_t sections_up_to = 0) {
  const uint32_t section = sections_up_to != 0 ? r.Below(sections_up_to)
                           : r.Below(8) == 0    ? r.Below(kTableEntries)
                                                : r.Below(route.sections);
  const int32_t n = int32_t(route.Slices(section));
  const int32_t slice = r.Below(4) == 0 ? int32_t(r.Below(uint32_t(n) + 6)) - 3
                                        : int32_t(r.Below(uint32_t(n)));
  guest::StoreU16(base, address + 0, uint16_t(section));
  guest::StoreU16(base, address + 2, uint16_t(slice));
}

}  // namespace harness::test_route
