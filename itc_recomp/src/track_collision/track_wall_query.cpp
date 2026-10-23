// The wall query: the walls of the cells near a box, which ones the race
// skips, and the box's corners and edges moving through them
// (docs/functions/collision-world.md): see track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "collision/collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace track_collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace co = collision;

namespace {

constexpr uint32_t kRaceData = 0x826C5B2C;     // the race data's pointer
constexpr uint32_t kRaceMode = 0xA3AF4;        // its word: the race mode
constexpr uint32_t kStageFlags = 0x59620 + 1220;  // the stage list's flags
constexpr uint32_t kGameState = 0x826D82E8;    // a pointer: its +6100 read for mode 5
constexpr uint32_t kCornerOrder = 0x82006900;  // bytes: the corners' order (the first 8)
constexpr uint32_t kCornerReach = 0x82002CB0;
constexpr uint32_t kCornerMargin = 0x82001FBC;
constexpr uint32_t kWallHit = 0x01000000;      // the hit's surface bits

// vmsum3fp128 of the 16 bytes at `a` and `b` (each rounded down to 16),
// flush-to-zero on, stored to all four words at `to` (rounded down).
// Returns it.
double DotTo(uint8_t* base, uint32_t a, uint32_t b, uint32_t to) {
  const float dot = vl::Dot(vl::LoadVector(base, a & ~0xFu), vl::LoadVector(base, b & ~0xFu));
  const uint32_t bits = std::bit_cast<uint32_t>(dot);
  const uint32_t out = to & ~0xFu;
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, out + i, bits);
  }
  return double(dot);
}

// The two ld/std pairs copying 16 bytes.
void Copy16(uint8_t* base, uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, to + i, guest::LoadU32(base, from + i));
  }
}

// The stage list's flags with 0x20 or 0x40: 0, else 1.
uint32_t StageFlagsSkip(uint8_t* base, uint32_t race) {
  const uint32_t flags = guest::LoadU32(base, race + kStageFlags);
  return (flags & 0x20) != 0 || (flags & 0x40) != 0 ? 0 : 1;
}

// IgnoresWall's kinds 16 to 22 and 30 to 41: the race data's +4 less this.
int32_t KindOffset(uint32_t kind) {
  switch (kind) {
    case 16: return 0;
    case 17: return 1;
    case 18: return 2;
    case 19: return 3;
    case 20: return 4;
    case 21: return 5;
    case 22: return 6;
    case 30: return 11;
    case 31: return 12;
    case 32: return 13;
    case 33: return 14;
    case 34: return 15;
    case 35: return 16;
    case 36: return 17;
    case 37: return 18;
    case 38: return 21;
    case 39: return 22;
    case 40: return 19;
    case 41: return 20;
    default: return -1;
  }
}

}  // namespace

uint32_t WallQueryModeSkips(uint8_t* base, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820FC870 WallQuery::ModeSkips");
  if (guest::LoadU32(base, word) == 2) {
    return 1;
  }
  const uint32_t race = guest::LoadU32(base, kRaceData);
  const int32_t mode = int32_t(guest::LoadU32(base, race + kRaceMode));
  if (mode < 3) {
    return 1;
  }
  if (mode <= 4) {
    return StageFlagsSkip(base, race);
  }
  if (mode == 5) {
    const int32_t state =
        int32_t(guest::LoadU32(base, guest::LoadU32(base, kGameState) + 6100));
    return state == 1 || state == 3 ? StageFlagsSkip(base, race) : 1;
  }
  if (mode <= 6) {
    return 1;
  }
  if (mode <= 8) {
    return StageFlagsSkip(base, race);
  }
  return 1;
}

uint32_t WallQueryIgnoresWall(uint8_t* base, uint32_t kind, uint32_t a, uint32_t b,
                              uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FD6D8 WallQuery::IgnoresWall");
  const uint32_t k = guest::LoadU16(base, kind);
  if (k - 2 > 59) {
    return 1;
  }
  switch (k) {
    case 2:
      return guest::LoadU16(base, a) == 2 ? 1 : 0;
    case 3: {
      if (guest::LoadU16(base, a) != 0) {
        return 0;
      }
      const uint32_t race = guest::LoadU32(base, kRaceData);
      if (int32_t(guest::LoadU32(base, race + kRaceMode)) != 2) {
        return 0;
      }
      const uint32_t flags = guest::LoadU32(base, race + kStageFlags);
      if ((flags & 0x20) != 0 || (flags & 0x40) != 0) {
        return 0;
      }
      return guest::LoadU16(base, b) == 0 ? 1 : 0;
    }
    case 8:
    case 9:
    case 10: {
      if (guest::LoadU16(base, a) == 2) {
        return 1;
      }
      const uint32_t race = guest::LoadU32(base, kRaceData);
      if (int32_t(guest::LoadU32(base, race + kRaceMode)) != 2) {
        return 1;
      }
      const uint32_t w = guest::LoadU32(base, race + 20);
      if (k == 8) {
        return (w & 0x10) != 0 ? 1 : 0;
      }
      if (k == 9) {
        return (w & 0x4) != 0 && (w & 0x8) != 0 ? 1 : 0;
      }
      return (w & 0x20) != 0 ? 1 : 0;
    }
    case 23: {
      guest::StoreU32(base, frame + 80, guest::LoadU16(base, a));
      if (WallQueryModeSkips(base, frame + 80) != 0) {
        return 1;
      }
      const int32_t v = int32_t(guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 4));
      if (v < 4) {
        return 1;
      }
      if (v <= 5) {
        return 0;
      }
      return v == 7 ? 0 : 1;
    }
    default:
      break;
  }
  if (const int32_t offset = KindOffset(k); offset >= 0) {
    guest::StoreU32(base, frame + 80, guest::LoadU16(base, a));
    if (WallQueryModeSkips(base, frame + 80) != 0) {
      return 1;
    }
    const uint32_t v = guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 4);
    return v - uint32_t(offset) != 0 ? 1 : 0;
  }
  if (k >= 50) {
    // The race's route bits (the race data's +24): the wall kept when its
    // bit is set.
    const uint32_t bit = k - 50;
    const uint32_t word =
        guest::LoadU32(base, guest::LoadU32(base, kRaceData) + ((bit >> 5) + 6) * 4);
    return (word & (1u << (bit & 31))) != 0 ? 0 : 1;
  }
  return 1;
}

bool WallQueryCornersThroughWall(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* hit) {
  ITC_NATIVE_RUNNING("sub_820FC3C8 WallQuery::CornersThroughWall");
  const uint32_t record = guest::LoadU32(base, query + 12);
  uint32_t previous = 0, current = 0;
  if (!co::CollisionShapePrevious(base, record, &previous) ||
      !co::CollisionShapeCurrent(base, record, &current) || !co::IsBox(base, previous) ||
      !co::IsBox(base, current)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t normal = query + 1440;
  const double one = Lfs(base, kOne);
  *hit = 0;
  Stfs(base, f + 92, one);
  const uint32_t count = co::BoxCornerCount();
  if (count == 0) {
    return true;
  }
  Stfs(base, f + 108, one);
  const double reach = Lfs(base, kCornerReach);
  const double margin = Lfs(base, kCornerMargin);
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < co::BoxCornerCount(); i++) {
    const uint32_t index = i < 8 ? guest::LoadU8(base, kCornerOrder + i) : (i & 0xFF);
    const uint32_t from = co::BoxCorner(previous, index);
    const uint32_t to = co::BoxCorner(current, index);
    const double cx = Lfs(base, to + 0), cy = Lfs(base, to + 4), cz = Lfs(base, to + 8);
    const double dx = Fsubs(cx, Lfs(base, from + 0));
    const double dy = Fsubs(cy, Lfs(base, from + 4));
    Stfs(base, f + 100, dy);
    const double dz = Fsubs(cz, Lfs(base, from + 8));
    Stfs(base, f + 96, dx);
    Stfs(base, f + 104, dz);
    // Not moving in x and z.
    if (dx == zero && dz == zero) {
      continue;
    }
    // Moving into the wall: normal . motion below 0.
    const double into = DotTo(base, normal, f + 96, f + 128);
    if (!(into < zero)) {
      continue;
    }
    // Ending behind the wall.
    const double distance = Lfs(base, query + 1464);
    const double depth = Fsubs(Fsubs(DotTo(base, normal, to, f + 144), distance), margin);
    if (!(depth < zero)) {
      continue;
    }
    // Starting in front of it.
    const double before = Fmadds(into, reach, distance);
    const double start = DotTo(base, normal, from, f + 160);
    if (before > start) {
      continue;
    }
    // The corner pushed back onto the wall, between the wall's ends.
    const double push = Fneg(depth);
    const double nx = Lfs(base, normal + 0), ny = Lfs(base, normal + 4),
                 nz = Lfs(base, normal + 8);
    Stfs(base, f + 92, one);
    const double lo = Lfs(base, query + 1456), hi = Lfs(base, query + 1460);
    Stfs(base, f + 80, Fadds(cx, Fmuls(nx, push)));
    Stfs(base, f + 84, Fadds(cy, Fmuls(ny, push)));
    Stfs(base, f + 88, Fadds(cz, Fmuls(nz, push)));
    const double along = DotTo(base, f + 80, query + 1408, f + 112);
    if (lo < hi) {
      if (lo > along || !(hi >= along)) {
        continue;
      }
    } else if (lo < along || hi > along) {
      continue;
    }
    Copy16(base, f + 80, query + 48);
    Copy16(base, normal, query + 64);
    Copy16(base, query + 64, query + 80);
    const uint32_t tested = guest::LoadU32(base, query + 624);
    Stfs(base, query + 96, push);
    Stfs(base, query + 100, push);
    guest::StoreU32(base, query + 104, tested);
    WallQueryAddHit(base, query, kWallHit);
    *hit = 1;
  }
  return true;
}

}  // namespace track_collision
