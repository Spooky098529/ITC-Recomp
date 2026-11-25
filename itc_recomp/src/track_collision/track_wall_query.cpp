// The wall query: the walls of the cells near a box, which ones the race
// skips, and the box's corners and edges moving through them
// (docs/functions/collision-world.md): see track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "collision/collision.h"
#include "schema/collision_world.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <bit>

namespace track_collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace co = collision;
namespace va = vehicle_accessors;

namespace {

using Fpscr = ::base::FloatMode;
using G = schema::CollisionWorldGlobals;
using Query = schema::WallQuery;

constexpr uint32_t kWallHit = 0x01000000;      // the hit's surface bits
constexpr uint32_t kWallLimit = 128;           // the walls kept in the tested list

// The shape record's template and copies are boxes (the wall query's
// functions are native only then).
bool Boxes(uint32_t record) {
  uint32_t previous = 0, current = 0;
  return co::CollisionShapePrevious(record, &previous) &&
         co::CollisionShapeCurrent(record, &current) && co::IsBox(previous) &&
         co::IsBox(current);
}

// out = the vector at `v` crossed with the one at `up` as the vector unit
// does it (vpermwi 0x9C and 0x78, vmulfp128, vnmsubfp), flush-to-zero on;
// the 16 bytes at each rounded down to 16. Its w is v.w * up.w - v.w *
// up.w's, as the original's.
void CrossVector(uint32_t v, uint32_t up, uint32_t to) {
  auto load = [](uint32_t at) {
    const uint32_t in = at & ~0xFu;
    return simde_mm_set_epi32(int(guest::LoadU32(in + 0)), int(guest::LoadU32(in + 4)),
                              int(guest::LoadU32(in + 8)), int(guest::LoadU32(in + 12)));
  };
  const simde__m128i u = load(up), d = load(v);
  simde__m128 u78 = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(u, 0x78));
  simde__m128 u9c = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(u, 0x9C));
  simde__m128 d9c = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(d, 0x9C));
  simde__m128 d78 = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(d, 0x78));
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vl::PinInMode(u78, u9c, d9c, d78);
  const simde__m128 product = simde_mm_mul_ps(d9c, u78);
  simde__m128 result =
      simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(d78, u9c), product),
                      simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
  vl::PinInMode(result);
  Fpscr::setcsr(csr);
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(result));
  const uint32_t out = to & ~0xFu;
  guest::StoreU32(out + 0, lanes[3]);
  guest::StoreU32(out + 4, lanes[2]);
  guest::StoreU32(out + 8, lanes[1]);
  guest::StoreU32(out + 12, lanes[0]);
}

// vmsum3fp128 of the 16 bytes at `a` and `b` (each rounded down to 16),
// flush-to-zero on, stored to all four words at `to` (rounded down).
// Returns it.
double DotTo(uint32_t a, uint32_t b, uint32_t to) {
  const float dot = vl::Dot(vl::LoadVector(a & ~0xFu), vl::LoadVector(b & ~0xFu));
  const uint32_t bits = std::bit_cast<uint32_t>(dot);
  const uint32_t out = to & ~0xFu;
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(out + i, bits);
  }
  return double(dot);
}

// The two ld/std pairs copying 16 bytes.
void Copy16(uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(to + i, guest::LoadU32(from + i));
  }
}

// The stage list's flags with 0x20 or 0x40: 0, else 1.
uint32_t StageFlagsSkip(const schema::RaceData& race) {
  const uint32_t flags = race.flags();
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
bool EndsInsideSides(uint32_t current, uint32_t query, uint32_t frame,
                     uint32_t at_a, uint32_t at_b, bool* a, bool* b) {
  for (uint32_t j = 0; j < co::BoxFaceCount(); j++) {
    if (j == 1 || j == 4) {
      continue;
    }
    const uint32_t face = co::BoxFace(current, j);
    if (*a) {
      const double distance = Lfs(face + 0);
      if (distance < DotTo(face + 16, Query(query).wall_a_x_address(), frame + at_a)) {
        *a = false;
      }
    }
    if (*b) {
      const double distance = Lfs(face + 0);
      if (distance < DotTo(face + 16, Query(query).wall_b_x_address(), frame + at_b)) {
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

uint32_t WallQueryModeSkips(uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820FC870 WallQuery::ModeSkips");
  if (guest::LoadU32(word) == 2) {
    return 1;
  }
  const schema::RaceData race = G::race_data();
  const int32_t mode = int32_t(race.race_mode());
  if (mode < 3) {
    return 1;
  }
  if (mode <= 4) {
    return StageFlagsSkip(race);
  }
  if (mode == 5) {
    const int32_t state = int32_t(G::race_progress().stage_type());
    return state == 1 || state == 3 ? StageFlagsSkip(race) : 1;
  }
  if (mode <= 6) {
    return 1;
  }
  if (mode <= 8) {
    return StageFlagsSkip(race);
  }
  return 1;
}

uint32_t WallQueryIgnoresWall(uint32_t kind, uint32_t a, uint32_t b,
                              uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FD6D8 WallQuery::IgnoresWall");
  const uint32_t k = guest::LoadU16(kind);
  if (k - 2 > 59) {
    return 1;
  }
  switch (k) {
    case 2:
      return guest::LoadU16(a) == 2 ? 1 : 0;
    case 3: {
      if (guest::LoadU16(a) != 0) {
        return 0;
      }
      const schema::RaceData race = G::race_data();
      if (int32_t(race.race_mode()) != 2) {
        return 0;
      }
      const uint32_t flags = race.flags();
      if ((flags & 0x20) != 0 || (flags & 0x40) != 0) {
        return 0;
      }
      return guest::LoadU16(b) == 0 ? 1 : 0;
    }
    case 8:
    case 9:
    case 10: {
      if (guest::LoadU16(a) == 2) {
        return 1;
      }
      const schema::RaceData race = G::race_data();
      if (int32_t(race.race_mode()) != 2) {
        return 1;
      }
      const uint32_t w = race.branch_flags();
      if (k == 8) {
        return (w & 0x10) != 0 ? 1 : 0;
      }
      if (k == 9) {
        return (w & 0x4) != 0 && (w & 0x8) != 0 ? 1 : 0;
      }
      return (w & 0x20) != 0 ? 1 : 0;
    }
    case 23: {
      guest::StoreU32(frame + 80, guest::LoadU16(a));
      if (WallQueryModeSkips(frame + 80) != 0) {
        return 1;
      }
      const int32_t v = int32_t(G::race_data().route_course());
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
    guest::StoreU32(frame + 80, guest::LoadU16(a));
    if (WallQueryModeSkips(frame + 80) != 0) {
      return 1;
    }
    const uint32_t v = G::race_data().route_course();
    return v - uint32_t(offset) != 0 ? 1 : 0;
  }
  if (k >= 50) {
    // The race's route bits (the race data's +24): the wall kept when its
    // bit is set.
    const uint32_t bit = k - 50;
    const uint32_t word =
        guest::LoadU32(G::race_data().drawn_entries_address() + (bit >> 5) * 4);
    return (word & (1u << (bit & 31))) != 0 ? 0 : 1;
  }
  return 1;
}

bool WallQueryCornersThroughWall(uint32_t query, uint32_t frame, uint32_t* hit) {
  ITC_NATIVE_RUNNING("sub_820FC3C8 WallQuery::CornersThroughWall");
  const Query wq(query);
  const uint32_t record = wq.record().address;
  uint32_t previous = 0, current = 0;
  if (!co::CollisionShapePrevious(record, &previous) ||
      !co::CollisionShapeCurrent(record, &current) || !co::IsBox(previous) ||
      !co::IsBox(current)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t normal = wq.normal_x_address();
  const double one = Lfs(kOne);
  *hit = 0;
  Stfs(f + 92, one);
  const uint32_t count = co::BoxCornerCount();
  if (count == 0) {
    return true;
  }
  Stfs(f + 108, one);
  const double reach = double(G::corner_reach());
  const double margin = double(G::corner_margin());
  const double zero = Lfs(kZero);
  for (uint32_t i = 0; i < co::BoxCornerCount(); i++) {
    const uint32_t index = i < 8 ? guest::LoadU8(G::corner_order_address() + i) : (i & 0xFF);
    const uint32_t from = co::BoxCorner(previous, index);
    const uint32_t to = co::BoxCorner(current, index);
    const double cx = Lfs(to + 0), cy = Lfs(to + 4), cz = Lfs(to + 8);
    const double dx = Fsubs(cx, Lfs(from + 0));
    const double dy = Fsubs(cy, Lfs(from + 4));
    Stfs(f + 100, dy);
    const double dz = Fsubs(cz, Lfs(from + 8));
    Stfs(f + 96, dx);
    Stfs(f + 104, dz);
    // Not moving in x and z.
    if (dx == zero && dz == zero) {
      continue;
    }
    // Moving into the wall: normal . motion below 0.
    const double into = DotTo(normal, f + 96, f + 128);
    if (!(into < zero)) {
      continue;
    }
    // Ending behind the wall.
    const double distance = double(wq.normal_distance());
    const double depth = Fsubs(Fsubs(DotTo(normal, to, f + 144), distance), margin);
    if (!(depth < zero)) {
      continue;
    }
    // Starting in front of it.
    const double before = Fmadds(into, reach, distance);
    const double start = DotTo(normal, from, f + 160);
    if (before > start) {
      continue;
    }
    // The corner pushed back onto the wall, between the wall's ends.
    const double push = Fneg(depth);
    const double nx = Lfs(normal + 0), ny = Lfs(normal + 4),
                 nz = Lfs(normal + 8);
    Stfs(f + 92, one);
    const double lo = double(wq.span_a()), hi = double(wq.span_b());
    Stfs(f + 80, Fadds(cx, Fmuls(nx, push)));
    Stfs(f + 84, Fadds(cy, Fmuls(ny, push)));
    Stfs(f + 88, Fadds(cz, Fmuls(nz, push)));
    const double along = DotTo(f + 80, wq.direction_x_address(), f + 112);
    if (lo < hi) {
      if (lo > along || !(hi >= along)) {
        continue;
      }
    } else if (lo < along || hi > along) {
      continue;
    }
    Copy16(f + 80, wq.hit_point_x_address());
    Copy16(normal, wq.hit_normal_x_address());
    Copy16(wq.hit_normal_x_address(), wq.hit_push_x_address());
    const uint32_t tested = wq.walls_tested();
    wq.set_hit_depth(float(push));
    wq.set_hit_depth_again(float(push));
    wq.set_hit_wall(tested);
    WallQueryAddHit(query, kWallHit);
    *hit = 1;
  }
  return true;
}

bool WallQueryEdgesAcrossWall(uint32_t query, uint32_t frame, uint32_t* hit) {
  ITC_NATIVE_RUNNING("sub_820FC900 WallQuery::EdgesAcrossWall");
  const Query wq(query);
  const uint32_t record = wq.record().address;
  uint32_t previous = 0, current = 0;
  if (!co::CollisionShapePrevious(record, &previous) ||
      !co::CollisionShapeCurrent(record, &current) || !co::IsBox(previous) ||
      !co::IsBox(current)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t q = query;
  const uint32_t end_a = wq.wall_a_x_address(), end_b = wq.wall_b_x_address();
  const uint32_t direction = wq.direction_x_address(), normal = wq.normal_x_address();
  const double one = Lfs(kOne);
  Stfs(f + 188, one);
  Stfs(f + 172, one);
  Stfs(f + 204, one);
  Stfs(f + 220, one);
  const double zero = Lfs(kZero);
  // The lengths of the ends' pushes (f18, f15), kept from edge to edge.
  double length_a = zero, length_b = zero;
  Stfs(f + 156, one);
  Stfs(f + 92, one);
  *hit = 0;
  if (co::BoxEdgeCount() == 0) {
    return true;
  }
  const double half = double(G::half());
  const double outward = double(G::outward());
  const double slope_floor = double(G::slope_floor());
  for (uint32_t e = 0; e < co::BoxEdgeCount(); e++) {
    if (e == 1 || e == 3 || e == 9 || e == 10) {
      continue;
    }
    const uint32_t s = co::BoxEdgeStart(e), t = co::BoxEdgeEnd(e);
    const uint32_t from_s = co::BoxCorner(previous, s), from_t = co::BoxCorner(previous, t);
    const uint32_t to_s = co::BoxCorner(current, s), to_t = co::BoxCorner(current, t);
    // The four places against the wall's plane.
    const double was_s = DotTo(normal, from_s, f + 368);
    const double was_t = DotTo(normal, from_t, f + 336);
    const double now_s = DotTo(normal, to_s, f + 496);
    const double distance = double(wq.normal_distance());
    const double now_t = DotTo(normal, to_t, f + 304);
    if (distance < was_s && distance < was_t && distance < now_s) {
      continue;
    }
    double move_s = Fsubs(now_s, was_s);
    double move_t = Fsubs(now_t, was_t);
    if (move_s > zero && move_t > zero) {
      continue;
    }
    // The ends' moves in x and z.
    const double move_sx = Fsubs(Lfs(to_s + 0), Lfs(from_s + 0));
    const double move_sz = Fsubs(Lfs(to_s + 8), Lfs(from_s + 8));
    const double move_tx = Fsubs(Lfs(to_t + 0), Lfs(from_t + 0));
    const double move_tz = Fsubs(Lfs(to_t + 8), Lfs(from_t + 8));
    guest::StoreU32(f + 224, to_s);
    guest::StoreU32(f + 228, to_t);
    guest::StoreU32(f + 232, end_a);
    guest::StoreU32(f + 236, end_b);
    // What's recorded: the point, the push direction and the depth.
    uint32_t point = 0, push = 0;
    double depth = zero;
    if (SegmentsIntersect2D(f + 224, f + 144) != 0) {
      // The edge across the wall in x and z: an end gone through it.
      const double distance2 = double(wq.normal_distance());
      const double depth_s = Fsubs(distance2, now_s);
      const double depth_t = Fsubs(distance2, now_t);
      if (move_s > slope_floor && move_s < zero) {
        move_s = slope_floor;
      }
      if (move_t > slope_floor && move_t < zero) {
        move_t = slope_floor;
      }
      uint32_t low = wq.span_a_address(), high = wq.span_b_address();
      if (Lfs(low) > Lfs(high)) {
        low = wq.span_b_address();
        high = wq.span_a_address();
      }
      DotTo(direction, to_s, f + 624);
      DotTo(direction, to_t, f + 400);
      bool through_s = false, through_t = false;
      if (depth_s > zero && !(Fadds(distance2, move_s) > was_s)) {
        const double along = Lfs(f + 624);
        through_s = !(Lfs(low) > along) && !(along > Lfs(high));
      }
      if (depth_t > zero && !(Fadds(distance2, move_t) > was_t)) {
        const double along = Lfs(f + 400);
        through_t = !(Lfs(low) > along) && !(along > Lfs(high));
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
        if (!(DotTo(normal, face_normal, f + 528) < zero)) {
          continue;
        }
        bool use_a = true, use_b = true;
        if (co::BoxFaceCount() != 0 &&
            !EndsInsideSides(current, q, f, 432, 592, &use_a, &use_b)) {
          continue;
        }
        if (!use_a && !use_b) {
          continue;
        }
        const double ox = Fmuls(Lfs(face_normal + 0), outward);
        const double oy = Fmuls(Lfs(face_normal + 4), outward);
        const double oz = Fmuls(Lfs(face_normal + 8), outward);
        if (use_a) {
          Stfs(f + 176, Fadds(Lfs(end_a + 0), ox));
          Stfs(f + 184, Fadds(Lfs(end_a + 8), oz));
          Stfs(f + 188, one);
          Stfs(f + 180, Fadds(oy, Lfs(end_a + 4)));
          guest::StoreU32(f + 272, to_s);
          guest::StoreU32(f + 276, to_t);
          guest::StoreU32(f + 280, end_a);
          guest::StoreU32(f + 284, f + 176);
          use_a = SegmentsIntersect2D(f + 272, f + 176) != 0;
          Stfs(f + 180, Lfs(end_a + 4));
        }
        if (use_b) {
          Stfs(f + 160, Fadds(Lfs(end_b + 0), ox));
          Stfs(f + 168, Fadds(Lfs(end_b + 8), oz));
          Stfs(f + 172, one);
          Stfs(f + 164, Fadds(oy, Lfs(end_b + 4)));
          guest::StoreU32(f + 240, to_s);
          guest::StoreU32(f + 244, to_t);
          guest::StoreU32(f + 248, end_b);
          guest::StoreU32(f + 252, f + 160);
          use_b = SegmentsIntersect2D(f + 240, f + 160) != 0;
          Stfs(f + 164, Lfs(end_b + 4));
        }
        if (!use_a && !use_b) {
          continue;
        }
        // Each end's push: from where the edge crosses the face's line to
        // the end, kept when it isn't back along the face or against the
        // wall's normal.
        if (use_a) {
          length_a = double(vl::Vec3Direction(f + 192, end_a, f + 176));
          Stfs(f + 204, one);
          if (!(length_a > zero)) {
            use_a = false;
          }
          if (DotTo(f + 192, face_normal, f + 464) > zero &&
              DotTo(f + 192, normal, f + 560) < zero) {
            use_a = false;
          }
        }
        if (use_b) {
          length_b = double(vl::Vec3Direction(f + 208, end_b, f + 160));
          Stfs(f + 220, one);
          if (!(length_b > zero)) {
            use_b = false;
          }
          if (DotTo(f + 208, face_normal, f + 320) > zero &&
              DotTo(f + 208, normal, f + 352) < zero) {
            use_b = false;
          }
        }
        if (use_a && use_b && length_a == length_b) {
          // Both alike: the mean of the two.
          Stfs(f + 80, Fadds(Lfs(f + 208), Lfs(f + 192)));
          Stfs(f + 84, Fadds(Lfs(f + 212), Lfs(f + 196)));
          Stfs(f + 88, Fadds(Lfs(f + 216), Lfs(f + 200)));
          Stfs(f + 92, one);
          const double mx = Fadds(Lfs(f + 160), Lfs(f + 176));
          const double my = Fadds(Lfs(f + 164), Lfs(f + 180));
          const double mz = Fadds(Lfs(f + 168), Lfs(f + 184));
          Stfs(f + 156, one);
          Stfs(f + 144, Fmuls(mx, half));
          Stfs(f + 148, Fmuls(my, half));
          Stfs(f + 152, Fmuls(mz, half));
          depth = length_a;
          NormalizeVector(f + 80, f + 80);
          Stfs(f + 92, one);
        } else if (use_a && !(use_b && !(length_a > length_b))) {
          depth = length_a;
          Copy16(f + 176, f + 144);
          Copy16(f + 192, f + 80);
        } else if (use_b) {
          depth = length_b;
          Copy16(f + 160, f + 144);
          Copy16(f + 208, f + 80);
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
          (!EndsInsideSides(current, q, f, 384, 416, &use_a, &use_b) || !use_a ||
           !use_b)) {
        continue;
      }
      Stfs(f + 140, one);
      Stfs(f + 124, one);
      Stfs(f + 108, one);
      const uint32_t face_normal = co::BoxFace(current, EdgeFace(e)) + 16;
      if (!(DotTo(normal, face_normal, f + 448) < zero)) {
        continue;
      }
      Stfs(f + 80, Fadds(move_tx, move_sx));
      Stfs(f + 88, Fadds(move_tz, move_sz));
      Stfs(f + 84, zero);
      Stfs(f + 92, one);
      if (DotTo(f + 80, f + 80, f + 480) == zero) {
        continue;
      }
      NormalizeVector(f + 80, f + 80);
      Stfs(f + 92, one);
      if (!(DotTo(f + 80, normal, f + 512) > zero)) {
        Copy16(normal, f + 80);
      }
      const double px = Fmuls(Lfs(f + 80), outward);
      const double py = Fmuls(Lfs(f + 84), outward);
      const double pz = Fmuls(Lfs(f + 88), outward);
      // End A pushed back (+128), end B (+112), each tested against the
      // box's edge in x and z.
      const double ax = Lfs(end_a + 0);
      Stfs(f + 112, px);
      Stfs(f + 116, py);
      Stfs(f + 120, pz);
      guest::StoreU32(f + 256, to_s);
      Stfs(f + 124, one);
      guest::StoreU32(f + 260, to_t);
      Stfs(f + 140, one);
      guest::StoreU32(f + 264, end_a);
      guest::StoreU32(f + 268, f + 128);
      Stfs(f + 128, Fsubs(ax, px));
      Stfs(f + 132, Fsubs(Lfs(end_a + 4), py));
      Stfs(f + 136, Fsubs(Lfs(end_a + 8), pz));
      const uint32_t crossed_a = SegmentsIntersect2D(f + 256, f + 128);
      Stfs(f + 132, Lfs(end_a + 4));
      Stfs(f + 112, Fsubs(Lfs(end_b + 0), Lfs(f + 112)));
      guest::StoreU32(f + 288, to_s);
      guest::StoreU32(f + 292, to_t);
      Stfs(f + 116, Fsubs(Lfs(end_b + 4), Lfs(f + 116)));
      guest::StoreU32(f + 296, end_b);
      guest::StoreU32(f + 300, f + 112);
      Stfs(f + 124, one);
      Stfs(f + 120, Fsubs(Lfs(end_b + 8), Lfs(f + 120)));
      const uint32_t crossed_b = SegmentsIntersect2D(f + 288, f + 112);
      const double by = Lfs(end_b + 4);
      Stfs(f + 116, by);
      if (crossed_a == 0) {
        if (crossed_b == 0) {
          continue;
        }
        Stfs(f + 100, Fsubs(Lfs(end_b + 4), by));
        Stfs(f + 96, Fsubs(Lfs(end_b + 0), Lfs(f + 112)));
        Stfs(f + 104, Fsubs(Lfs(end_b + 8), Lfs(f + 120)));
        Stfs(f + 108, one);
        depth = DotTo(f + 96, f + 80, f + 640);
        Copy16(f + 112, f + 144);
      } else {
        Stfs(f + 108, one);
        Stfs(f + 96, Fsubs(Lfs(end_a + 0), Lfs(f + 128)));
        Stfs(f + 100, Fsubs(Lfs(end_a + 4), Lfs(f + 132)));
        Stfs(f + 104, Fsubs(Lfs(end_a + 8), Lfs(f + 136)));
        if (crossed_b == 0) {
          depth = DotTo(f + 96, f + 80, f + 608);
          Copy16(f + 128, f + 144);
        } else {
          // Both: the deeper.
          const vl::Vector to_a = vl::LoadVector((f + 96) & ~0xFu);
          Stfs(f + 100, Fsubs(Lfs(end_b + 4), by));
          const float dot_a = vl::Dot(to_a, vl::LoadVector((f + 80) & ~0xFu));
          Stfs(f + 96, Fsubs(Lfs(end_b + 0), Lfs(f + 112)));
          Stfs(f + 104, Fsubs(Lfs(end_b + 8), Lfs(f + 120)));
          Stfs(f + 108, one);
          const double dot_b = DotTo(f + 96, f + 80, f + 576);
          for (uint32_t w = 0; w < 16; w += 4) {
            guest::StoreU32(f + 544 + w, std::bit_cast<uint32_t>(dot_a));
          }
          const double a = Lfs(f + 544);
          if (a < dot_b) {
            Copy16(f + 112, f + 144);
            depth = dot_b;
          } else {
            Copy16(f + 128, f + 144);
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
    Copy16(point, wq.hit_point_x_address());
    wq.set_hit_point_y(float(Fmuls(Fadds(Lfs(to_t + 4), Lfs(to_s + 4)), half)));
    Copy16(normal, wq.hit_normal_x_address());
    Copy16(push, wq.hit_push_x_address());
    const uint32_t tested = wq.walls_tested();
    wq.set_hit_depth(float(depth));
    wq.set_hit_depth_again(float(depth));
    wq.set_hit_wall(tested);
    WallQueryAddHit(q, kWallHit);
    *hit = 1;
  }
  return true;
}

bool WallQueryTestCell(uint32_t query, uint32_t mesh, uint32_t cell,
                       uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FDC18 WallQuery::TestCell");
  const uint32_t q = query;
  const Query wq(q);
  if (!Boxes(wq.record().address)) {
    return false;
  }
  const uint32_t f = frame;
  // The cell in the caller's frame (+36), as the original keeps it.
  guest::StoreU32(f + 292, cell);
  const uint32_t table = schema::TrackMesh(mesh).file().sections();
  const uint32_t section_number = guest::LoadU16(f + 292);
  const int32_t slice = int16_t(guest::LoadU16(f + 294));
  const uint32_t section =
      guest::LoadU32(uint32_t(int32_t(int16_t(section_number))) * 4 + table);
  const schema::TrackMeshSection sec(section);
  const schema::TrackMeshSlice slice_at(uint32_t(slice) * 28 + sec.slices() + section);
  const uint32_t vertices = sec.vertices() + section;
  const uint32_t first = slice_at.first_wall();
  uint32_t walls = slice_at.wall_count();
  uint32_t wall = (first << 3) + sec.walls() + section;
  if (walls == 0) {
    return true;
  }
  guest::StoreU16(f + 84, uint16_t(section_number));
  const double minus_one = double(G::minus_one());
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  for (; walls != 0; walls--, wall += 8) {
    const schema::TrackMeshWall w(wall);
    const uint32_t tested = wq.walls_tested();
    const uint32_t listed = tested > kWallLimit ? kWallLimit : tested;
    const uint32_t a = w.vertex_0(), b = w.vertex_1();
    guest::StoreU16(f + 86, uint16_t(a));
    guest::StoreU16(f + 88, uint16_t(b));
    // A wall with a kind the race skips.
    if (w.kind() != 0) {
      const uint32_t shape = wq.record().template_shape();
      guest::StoreU16(f + 80, uint16_t(va::ObjectHalf114(shape)));
      guest::StoreU16(f + 82, uint16_t(va::ObjectHalf112(shape)));
      if (WallQueryIgnoresWall(w.kind_address(), f + 82, f + 80, f - 96) != 0) {
        continue;
      }
    }
    // One already tested (another cell's), else listed.
    bool seen = false;
    for (uint32_t i = 0; i < listed && int32_t(listed) > 0; i++) {
      const schema::TrackTestedWall entry(wq.walls_address() + 6 * i);
      if (entry.section() == section_number && entry.vertex_0() == a && entry.vertex_1() == b) {
        seen = true;
        break;
      }
    }
    if (seen) {
      continue;
    }
    if (wq.walls_tested() < kWallLimit) {
      const schema::TrackTestedWall entry(wq.walls_address() + wq.walls_tested() * 6);
      entry.set_section(guest::LoadU16(f + 84));
      entry.set_vertex_0(guest::LoadU16(f + 86));
      entry.set_vertex_1(guest::LoadU16(f + 88));
      wq.set_walls_tested(wq.walls_tested() + 1);
    }
    // The wall: its ends, direction, normal and spans.
    for (uint32_t i = 0; i < 12; i += 4) {
      Stfs(wq.wall_a_x_address() + i, Lfs(w.vertex_0() * 12 + vertices + i));
    }
    for (uint32_t i = 0; i < 12; i += 4) {
      Stfs(wq.wall_b_x_address() + i, Lfs(w.vertex_1() * 12 + vertices + i));
    }
    if (double(vl::Vec3Direction(wq.direction_x_address(), wq.wall_a_x_address(),
                                 wq.wall_b_x_address())) == zero) {
      continue;
    }
    wq.set_direction_w(float(one));
    wq.set_negated_x(float(Fmuls(double(wq.direction_x()), minus_one)));
    wq.set_negated_y(float(Fmuls(double(wq.direction_y()), minus_one)));
    wq.set_negated_z(float(Fmuls(double(wq.direction_z()), minus_one)));
    CrossVector(wq.direction_x_address(), G::up_too_address(), wq.normal_x_address());
    wq.set_normal_w(float(one));
    wq.set_span_a(float(DotTo(wq.direction_x_address(), wq.wall_a_x_address(), f + 96)));
    wq.set_span_b(float(DotTo(wq.direction_x_address(), wq.wall_b_x_address(), f + 112)));
    wq.set_normal_distance(
        float(DotTo(wq.normal_x_address(), wq.wall_b_x_address(), f + 128)));
    uint32_t hit = 0;
    WallQueryCornersThroughWall(q, f - 304, &hit);
    if (hit == 0) {
      WallQueryEdgesAcrossWall(q, f - 944, &hit);
    }
  }
  return true;
}

bool TrackCollisionQueryWalls(uint32_t query, uint32_t mesh, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FDF38 TrackCollision::QueryWalls");
  const Query wq(query);
  if (!Boxes(wq.record().address)) {
    return false;
  }
  wq.set_walls_tested(0);
  uint32_t count = 0;
  WallQueryGatherCells(query, mesh, frame - 192, &count);
  for (int32_t i = 0; i < int32_t(wq.cell_count()); i++) {
    WallQueryTestCell(query, mesh, guest::LoadU32(wq.cells_address() + 4 * uint32_t(i)),
                      frame - 256);
  }
  return true;
}

bool TrackCollisionWallHits(uint32_t hits, uint32_t record, uint32_t frame,
                            uint32_t* count) {
  ITC_NATIVE_RUNNING("sub_820FF1B0 TrackCollision::WallHits");
  if (!Boxes(record)) {
    return false;
  }
  const uint32_t f = frame;
  const double zero = Lfs(kZero);
  Stfs(f + 108, zero);
  Stfs(f + 124, zero);
  const double one = Lfs(kOne);
  for (const uint32_t at : {140u, 156u, 172u, 1500u, 1516u, 1532u}) {
    Stfs(f + at, one);
  }
  co::CollisionShapeUpdateBounds(record);
  const Query wq(f + 80);
  wq.set_hits(hits);
  wq.set_record(record);
  const double two = double(G::two());
  Stfs(hits + 64, two);
  wq.set_time(float(two));
  wq.set_hit_count(0);
  TrackCollisionQueryWalls(wq.address, kMesh, f - 128);
  *count = wq.hit_count();
  return true;
}

}  // namespace track_collision
