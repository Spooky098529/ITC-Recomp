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
constexpr uint32_t kHalf = 0x82001BE4;         // 0.5
constexpr uint32_t kOutward = 0x8200690C;      // the faces' normals scaled by this
constexpr uint32_t kSlopeFloor = 0x82006908;   // the ends' moves toward the wall at least this

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

// The face EdgesAcrossWall pushes along for an edge (its switches: edges
// 1, 3, 9 and 10 aren't tested).
uint32_t EdgeFace(uint32_t edge) {
  switch (edge) {
    case 0:
    case 5:
      return 2;
    case 2:
    case 6:
      return 0;
    case 4:
    case 7:
      return 3;
    default:
      return 5;  // 8 and 11
  }
}

// Whether the wall's ends (+16, +32) lie inside the box's side faces (all
// but 1 and 4): `a` and `b` cleared for an end outside one (distance below
// normal . end). False when both are cleared, as soon as they are. The dot
// products go to `frame` + `at_a` and + `at_b`.
bool EndsInsideSides(uint8_t* base, uint32_t current, uint32_t query, uint32_t frame,
                     uint32_t at_a, uint32_t at_b, bool* a, bool* b) {
  for (uint32_t j = 0; j < co::BoxFaceCount(); j++) {
    if (j == 1 || j == 4) {
      continue;
    }
    const uint32_t face = co::BoxFace(current, j);
    if (*a) {
      const double distance = Lfs(base, face + 0);
      if (distance < DotTo(base, face + 16, query + 16, frame + at_a)) {
        *a = false;
      }
    }
    if (*b) {
      const double distance = Lfs(base, face + 0);
      if (distance < DotTo(base, face + 16, query + 32, frame + at_b)) {
        *b = false;
      }
    }
    if (!*a && !*b) {
      return false;
    }
  }
  return true;
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

bool WallQueryEdgesAcrossWall(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* hit) {
  ITC_NATIVE_RUNNING("sub_820FC900 WallQuery::EdgesAcrossWall");
  const uint32_t record = guest::LoadU32(base, query + 12);
  uint32_t previous = 0, current = 0;
  if (!co::CollisionShapePrevious(base, record, &previous) ||
      !co::CollisionShapeCurrent(base, record, &current) || !co::IsBox(base, previous) ||
      !co::IsBox(base, current)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t q = query;
  const uint32_t end_a = q + 16, end_b = q + 32, direction = q + 1408, normal = q + 1440;
  const double one = Lfs(base, kOne);
  Stfs(base, f + 188, one);
  Stfs(base, f + 172, one);
  Stfs(base, f + 204, one);
  Stfs(base, f + 220, one);
  const double zero = Lfs(base, kZero);
  // The lengths of the ends' pushes (f18, f15), kept from edge to edge.
  double length_a = zero, length_b = zero;
  Stfs(base, f + 156, one);
  Stfs(base, f + 92, one);
  *hit = 0;
  if (co::BoxEdgeCount() == 0) {
    return true;
  }
  const double half = Lfs(base, kHalf);
  const double outward = Lfs(base, kOutward);
  const double slope_floor = Lfs(base, kSlopeFloor);
  for (uint32_t e = 0; e < co::BoxEdgeCount(); e++) {
    if (e == 1 || e == 3 || e == 9 || e == 10) {
      continue;
    }
    const uint32_t s = co::BoxEdgeStart(base, e), t = co::BoxEdgeEnd(base, e);
    const uint32_t from_s = co::BoxCorner(previous, s), from_t = co::BoxCorner(previous, t);
    const uint32_t to_s = co::BoxCorner(current, s), to_t = co::BoxCorner(current, t);
    // The four places against the wall's plane.
    const double was_s = DotTo(base, normal, from_s, f + 368);
    const double was_t = DotTo(base, normal, from_t, f + 336);
    const double now_s = DotTo(base, normal, to_s, f + 496);
    const double distance = Lfs(base, q + 1464);
    const double now_t = DotTo(base, normal, to_t, f + 304);
    if (distance < was_s && distance < was_t && distance < now_s) {
      continue;
    }
    double move_s = Fsubs(now_s, was_s);
    double move_t = Fsubs(now_t, was_t);
    if (move_s > zero && move_t > zero) {
      continue;
    }
    // The ends' moves in x and z.
    const double move_sx = Fsubs(Lfs(base, to_s + 0), Lfs(base, from_s + 0));
    const double move_sz = Fsubs(Lfs(base, to_s + 8), Lfs(base, from_s + 8));
    const double move_tx = Fsubs(Lfs(base, to_t + 0), Lfs(base, from_t + 0));
    const double move_tz = Fsubs(Lfs(base, to_t + 8), Lfs(base, from_t + 8));
    guest::StoreU32(base, f + 224, to_s);
    guest::StoreU32(base, f + 228, to_t);
    guest::StoreU32(base, f + 232, end_a);
    guest::StoreU32(base, f + 236, end_b);
    // What's recorded: the point, the push direction and the depth.
    uint32_t point = 0, push = 0;
    double depth = zero;
    if (SegmentsIntersect2D(base, f + 224, f + 144) != 0) {
      // The edge across the wall in x and z: an end gone through it.
      const double distance2 = Lfs(base, q + 1464);
      const double depth_s = Fsubs(distance2, now_s);
      const double depth_t = Fsubs(distance2, now_t);
      if (move_s > slope_floor && move_s < zero) {
        move_s = slope_floor;
      }
      if (move_t > slope_floor && move_t < zero) {
        move_t = slope_floor;
      }
      uint32_t low = q + 1456, high = q + 1460;
      if (Lfs(base, low) > Lfs(base, high)) {
        low = q + 1460;
        high = q + 1456;
      }
      DotTo(base, direction, to_s, f + 624);
      DotTo(base, direction, to_t, f + 400);
      bool through_s = false, through_t = false;
      if (depth_s > zero && !(Fadds(distance2, move_s) > was_s)) {
        const double along = Lfs(base, f + 624);
        through_s = !(Lfs(base, low) > along) && !(along > Lfs(base, high));
      }
      if (depth_t > zero && !(Fadds(distance2, move_t) > was_t)) {
        const double along = Lfs(base, f + 400);
        through_t = !(Lfs(base, low) > along) && !(along > Lfs(base, high));
      }
      if (through_s && !(through_t && depth_s < depth_t)) {
        point = to_s;
        depth = depth_s;
        push = normal;
      } else if (through_t) {
        point = to_t;
        depth = depth_t;
        push = normal;
      } else {
        // Neither: the wall's ends pushed out of the box's face for the
        // edge, along its normal.
        const uint32_t face_normal = co::BoxFace(current, EdgeFace(e)) + 16;
        if (!(DotTo(base, normal, face_normal, f + 528) < zero)) {
          continue;
        }
        bool use_a = true, use_b = true;
        if (co::BoxFaceCount() != 0 &&
            !EndsInsideSides(base, current, q, f, 432, 592, &use_a, &use_b)) {
          continue;
        }
        if (!use_a && !use_b) {
          continue;
        }
        const double ox = Fmuls(Lfs(base, face_normal + 0), outward);
        const double oy = Fmuls(Lfs(base, face_normal + 4), outward);
        const double oz = Fmuls(Lfs(base, face_normal + 8), outward);
        if (use_a) {
          Stfs(base, f + 176, Fadds(Lfs(base, end_a + 0), ox));
          Stfs(base, f + 184, Fadds(Lfs(base, end_a + 8), oz));
          Stfs(base, f + 188, one);
          Stfs(base, f + 180, Fadds(oy, Lfs(base, end_a + 4)));
          guest::StoreU32(base, f + 272, to_s);
          guest::StoreU32(base, f + 276, to_t);
          guest::StoreU32(base, f + 280, end_a);
          guest::StoreU32(base, f + 284, f + 176);
          use_a = SegmentsIntersect2D(base, f + 272, f + 176) != 0;
          Stfs(base, f + 180, Lfs(base, end_a + 4));
        }
        if (use_b) {
          Stfs(base, f + 160, Fadds(Lfs(base, end_b + 0), ox));
          Stfs(base, f + 168, Fadds(Lfs(base, end_b + 8), oz));
          Stfs(base, f + 172, one);
          Stfs(base, f + 164, Fadds(oy, Lfs(base, end_b + 4)));
          guest::StoreU32(base, f + 240, to_s);
          guest::StoreU32(base, f + 244, to_t);
          guest::StoreU32(base, f + 248, end_b);
          guest::StoreU32(base, f + 252, f + 160);
          use_b = SegmentsIntersect2D(base, f + 240, f + 160) != 0;
          Stfs(base, f + 164, Lfs(base, end_b + 4));
        }
        if (!use_a && !use_b) {
          continue;
        }
        // Each end's push: from where the edge crosses the face's line to
        // the end, kept when it isn't back along the face or against the
        // wall's normal.
        if (use_a) {
          length_a = double(vl::Vec3Direction(base, f + 192, end_a, f + 176));
          Stfs(base, f + 204, one);
          if (!(length_a > zero)) {
            use_a = false;
          }
          if (DotTo(base, f + 192, face_normal, f + 464) > zero &&
              DotTo(base, f + 192, normal, f + 560) < zero) {
            use_a = false;
          }
        }
        if (use_b) {
          length_b = double(vl::Vec3Direction(base, f + 208, end_b, f + 160));
          Stfs(base, f + 220, one);
          if (!(length_b > zero)) {
            use_b = false;
          }
          if (DotTo(base, f + 208, face_normal, f + 320) > zero &&
              DotTo(base, f + 208, normal, f + 352) < zero) {
            use_b = false;
          }
        }
        if (use_a && use_b && length_a == length_b) {
          // Both alike: the mean of the two.
          Stfs(base, f + 80, Fadds(Lfs(base, f + 208), Lfs(base, f + 192)));
          Stfs(base, f + 84, Fadds(Lfs(base, f + 212), Lfs(base, f + 196)));
          Stfs(base, f + 88, Fadds(Lfs(base, f + 216), Lfs(base, f + 200)));
          Stfs(base, f + 92, one);
          const double mx = Fadds(Lfs(base, f + 160), Lfs(base, f + 176));
          const double my = Fadds(Lfs(base, f + 164), Lfs(base, f + 180));
          const double mz = Fadds(Lfs(base, f + 168), Lfs(base, f + 184));
          Stfs(base, f + 156, one);
          Stfs(base, f + 144, Fmuls(mx, half));
          Stfs(base, f + 148, Fmuls(my, half));
          Stfs(base, f + 152, Fmuls(mz, half));
          depth = length_a;
          NormalizeVector(base, f + 80, f + 80);
          Stfs(base, f + 92, one);
        } else if (use_a && !(use_b && !(length_a > length_b))) {
          depth = length_a;
          Copy16(base, f + 176, f + 144);
          Copy16(base, f + 192, f + 80);
        } else if (use_b) {
          depth = length_b;
          Copy16(base, f + 160, f + 144);
          Copy16(base, f + 208, f + 80);
        } else {
          continue;
        }
        point = f + 144;
        push = f + 80;
      }
    } else {
      // The edge beside the wall: the wall's ends inside the box's side
      // faces, pushed out along the edge's move.
      bool use_a = true, use_b = true;
      if (co::BoxFaceCount() != 0 &&
          (!EndsInsideSides(base, current, q, f, 384, 416, &use_a, &use_b) || !use_a ||
           !use_b)) {
        continue;
      }
      Stfs(base, f + 140, one);
      Stfs(base, f + 124, one);
      Stfs(base, f + 108, one);
      const uint32_t face_normal = co::BoxFace(current, EdgeFace(e)) + 16;
      if (!(DotTo(base, normal, face_normal, f + 448) < zero)) {
        continue;
      }
      Stfs(base, f + 80, Fadds(move_tx, move_sx));
      Stfs(base, f + 88, Fadds(move_tz, move_sz));
      Stfs(base, f + 84, zero);
      Stfs(base, f + 92, one);
      if (DotTo(base, f + 80, f + 80, f + 480) == zero) {
        continue;
      }
      NormalizeVector(base, f + 80, f + 80);
      Stfs(base, f + 92, one);
      if (!(DotTo(base, f + 80, normal, f + 512) > zero)) {
        Copy16(base, normal, f + 80);
      }
      const double px = Fmuls(Lfs(base, f + 80), outward);
      const double py = Fmuls(Lfs(base, f + 84), outward);
      const double pz = Fmuls(Lfs(base, f + 88), outward);
      // End A pushed back (+128), end B (+112), each tested against the
      // box's edge in x and z.
      const double ax = Lfs(base, end_a + 0);
      Stfs(base, f + 112, px);
      Stfs(base, f + 116, py);
      Stfs(base, f + 120, pz);
      guest::StoreU32(base, f + 256, to_s);
      Stfs(base, f + 124, one);
      guest::StoreU32(base, f + 260, to_t);
      Stfs(base, f + 140, one);
      guest::StoreU32(base, f + 264, end_a);
      guest::StoreU32(base, f + 268, f + 128);
      Stfs(base, f + 128, Fsubs(ax, px));
      Stfs(base, f + 132, Fsubs(Lfs(base, end_a + 4), py));
      Stfs(base, f + 136, Fsubs(Lfs(base, end_a + 8), pz));
      const uint32_t crossed_a = SegmentsIntersect2D(base, f + 256, f + 128);
      Stfs(base, f + 132, Lfs(base, end_a + 4));
      Stfs(base, f + 112, Fsubs(Lfs(base, end_b + 0), Lfs(base, f + 112)));
      guest::StoreU32(base, f + 288, to_s);
      guest::StoreU32(base, f + 292, to_t);
      Stfs(base, f + 116, Fsubs(Lfs(base, end_b + 4), Lfs(base, f + 116)));
      guest::StoreU32(base, f + 296, end_b);
      guest::StoreU32(base, f + 300, f + 112);
      Stfs(base, f + 124, one);
      Stfs(base, f + 120, Fsubs(Lfs(base, end_b + 8), Lfs(base, f + 120)));
      const uint32_t crossed_b = SegmentsIntersect2D(base, f + 288, f + 112);
      const double by = Lfs(base, end_b + 4);
      Stfs(base, f + 116, by);
      if (crossed_a == 0) {
        if (crossed_b == 0) {
          continue;
        }
        Stfs(base, f + 100, Fsubs(Lfs(base, end_b + 4), by));
        Stfs(base, f + 96, Fsubs(Lfs(base, end_b + 0), Lfs(base, f + 112)));
        Stfs(base, f + 104, Fsubs(Lfs(base, end_b + 8), Lfs(base, f + 120)));
        Stfs(base, f + 108, one);
        depth = DotTo(base, f + 96, f + 80, f + 640);
        Copy16(base, f + 112, f + 144);
      } else {
        Stfs(base, f + 108, one);
        Stfs(base, f + 96, Fsubs(Lfs(base, end_a + 0), Lfs(base, f + 128)));
        Stfs(base, f + 100, Fsubs(Lfs(base, end_a + 4), Lfs(base, f + 132)));
        Stfs(base, f + 104, Fsubs(Lfs(base, end_a + 8), Lfs(base, f + 136)));
        if (crossed_b == 0) {
          depth = DotTo(base, f + 96, f + 80, f + 608);
          Copy16(base, f + 128, f + 144);
        } else {
          // Both: the deeper.
          const vl::Vector to_a = vl::LoadVector(base, (f + 96) & ~0xFu);
          Stfs(base, f + 100, Fsubs(Lfs(base, end_b + 4), by));
          const float dot_a = vl::Dot(to_a, vl::LoadVector(base, (f + 80) & ~0xFu));
          Stfs(base, f + 96, Fsubs(Lfs(base, end_b + 0), Lfs(base, f + 112)));
          Stfs(base, f + 104, Fsubs(Lfs(base, end_b + 8), Lfs(base, f + 120)));
          Stfs(base, f + 108, one);
          const double dot_b = DotTo(base, f + 96, f + 80, f + 576);
          for (uint32_t w = 0; w < 16; w += 4) {
            guest::StoreU32(base, f + 544 + w, std::bit_cast<uint32_t>(dot_a));
          }
          const double a = Lfs(base, f + 544);
          if (a < dot_b) {
            Copy16(base, f + 112, f + 144);
            depth = dot_b;
          } else {
            Copy16(base, f + 128, f + 144);
            depth = a;
          }
        }
      }
      point = f + 144;
      push = f + 80;
    }
    if (!(depth > zero)) {
      continue;
    }
    // The hit: the point at the edge's mean height.
    Copy16(base, point, q + 48);
    Stfs(base, q + 52, Fmuls(Fadds(Lfs(base, to_t + 4), Lfs(base, to_s + 4)), half));
    Copy16(base, normal, q + 64);
    Copy16(base, push, q + 80);
    const uint32_t tested = guest::LoadU32(base, q + 624);
    Stfs(base, q + 96, depth);
    Stfs(base, q + 100, depth);
    guest::StoreU32(base, q + 104, tested);
    WallQueryAddHit(base, q, kWallHit);
    *hit = 1;
  }
  return true;
}

}  // namespace track_collision
