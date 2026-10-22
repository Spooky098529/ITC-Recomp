// The impacts: a corner of one box crossing a face of the other between
// the two placements (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

constexpr uint32_t kMinusOne = 0x82001BE0;  // the game's float -1.0
constexpr uint32_t kTwo = 0x820021E8;       // 2.0
// The impact time's limit (read from guest memory; probably 1.0).
constexpr uint32_t kImpactLimit = 0x82002B90;
// The impact tests by the shapes' types (A's x 3 + B's), and the box-box
// entry's function.
constexpr uint32_t kImpactTests = 0x82600EB4;
constexpr uint32_t kBoxBoxFindImpact = 0x820BF038;
// The push-out tests by the shapes' types, and the box-box entry's.
constexpr uint32_t kPushOutTests = 0x82600E90;
constexpr uint32_t kBoxBoxFindPushOut = 0x820BFB28;
constexpr uint32_t kHalf = 0x82001BE4;  // 0.5
// PushOutCorners' starting distance (read from guest memory).
constexpr uint32_t kCornerStart = 0x82002A90;
// PushOutEdges' depth limit (a single) and the separation's floor (a
// double, lfd).
constexpr uint32_t kEdgeDepthLimit = 0x82002B94;
constexpr uint32_t kEdgeSeparationFloor = 0x82002B98;

void SwapWords(uint8_t* base, uint32_t a, uint32_t b) {
  const uint32_t at_b = guest::LoadU32(base, b);
  const uint32_t at_a = guest::LoadU32(base, a);
  guest::StoreU32(base, a, at_b);
  guest::StoreU32(base, b, at_a);
}

// Both records' templates and current copies are boxes.
bool CurrentBoxes(uint8_t* base, uint32_t query) {
  for (uint32_t at : {0u, 4u}) {
    const uint32_t record = guest::LoadU32(base, query + at);
    if (!IsBox(base, guest::LoadU32(base, record)) ||
        !IsBox(base, guest::LoadU32(base, record + 8))) {
      return false;
    }
  }
  return true;
}

// One axis of a corner's move against the unit box's faces on it: the
// coordinate `axis` (0, 4 or 8) of the previous corner `p` and the current
// one `c`, the other two `u` and `v`. `positive`: the + face (bit 1, 4 or
// 16), else the - face. The crossing's fraction t when it's under the best
// so far (`best` x the move) and the other two coordinates there are within
// 1.0; `best` becomes t.
bool CrossesFace(uint8_t* base, uint32_t p, uint32_t c, uint32_t axis, uint32_t u, uint32_t v,
                 bool positive, double one, double* best) {
  const double pa = Lfs(base, p + axis);
  double move, t0;
  if (positive) {
    move = Fsubs(pa, Lfs(base, c + axis));
    t0 = Fsubs(pa, one);
  } else {
    move = Fsubs(Lfs(base, c + axis), pa);
    t0 = Fneg(Fadds(pa, one));
  }
  const double pu = Lfs(base, p + u);
  const double pv = Lfs(base, p + v);
  if (!(t0 < Fmuls(move, *best))) {
    return false;
  }
  const double t = Fdivs(t0, move);
  if (!(Fabs(Fmadds(Fsubs(Lfs(base, c + u), pu), t, pu)) < one)) {
    return false;
  }
  if (!(Fabs(Fmadds(Fsubs(Lfs(base, c + v), pv), t, pv)) < one)) {
    return false;
  }
  *best = t;
  return true;
}

}  // namespace

uint32_t BoxBoxSwapForImpact(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BE490 BoxBox::SwapForImpact");
  ContactFlip(base, query + 32);
  SwapWords(base, query + 0, query + 4);
  SwapWords(base, query + 12, query + 20);
  SwapWords(base, query + 8, query + 16);
  return query + 64;
}

uint32_t BoxBoxSwapForPushOut(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BEBF0 BoxBox::SwapForPushOut");
  vl::Vec4ScaleBy(base, query + 96, Lfs(base, kMinusOne));
  ContactFlip(base, query + 32);
  SwapWords(base, query + 0, query + 4);
  SwapWords(base, query + 12, query + 20);
  return query + 64;
}

bool BoxBoxCornerImpacts(uint8_t* base, uint32_t query, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BECA0 BoxBox::CornerImpacts");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  *r3 = query;
  double best = Lfs(base, query + 84);
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    // A's corner k in B's unit box, at the previous and current placements.
    const uint32_t previous = guest::LoadU32(base, query + 16) + 32 * k;
    const uint32_t current = guest::LoadU32(base, query + 20) + 32 * k;
    const uint32_t previous_code = guest::LoadU32(base, previous + 96);
    const uint32_t current_code = guest::LoadU32(base, current + 96);
    if ((previous_code & current_code) != 0) {
      continue;
    }
    const uint32_t left = previous_code & ~current_code;
    if (left == 0) {
      continue;
    }
    const uint32_t p = previous + 80, c = current + 80;
    uint32_t face = 0;
    if (left & 1) {
      CrossesFace(base, p, c, 0, 4, 8, true, one, &best);
    } else if (left & 2) {
      if (CrossesFace(base, p, c, 0, 4, 8, false, one, &best)) {
        face = 3;
      }
    }
    if (left & 4) {
      if (CrossesFace(base, p, c, 4, 0, 8, true, one, &best)) {
        face = 1;
      }
    } else if (left & 8) {
      if (CrossesFace(base, p, c, 4, 0, 8, false, one, &best)) {
        face = 4;
      }
    }
    if (left & 16) {
      if (CrossesFace(base, p, c, 8, 0, 4, true, one, &best)) {
        face = 2;
      }
    } else if (left & 32) {
      if (CrossesFace(base, p, c, 8, 0, 4, false, one, &best)) {
        face = 5;
      }
    }
    if (!(best < Lfs(base, query + 84))) {
      continue;
    }
    // The contact: A's current corner, B's face's normal, the depth the
    // corner is in by, the time.
    const uint32_t copy_a = guest::LoadU32(base, guest::LoadU32(base, query + 0) + 8);
    const uint32_t copy_b = guest::LoadU32(base, guest::LoadU32(base, query + 4) + 8);
    uint32_t normal = 0;
    const double distance =
        BoxFaceDistance(base, copy_b, face, BoxCorner(copy_a, k), &normal);
    const uint32_t face_at = BoxFace(copy_b, face);
    ContactSet(base, query + 32, BoxCorner(copy_a, k), face_at + 16, Fneg(distance), best);
    *r3 = query + 64;
  }
  return true;
}

bool BoxBoxFindImpact(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BF038 BoxBox::FindImpact");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  uint32_t r3 = 0;
  BoxBoxCornerImpacts(base, query, &r3);
  BoxBoxSwapForImpact(base, query);
  BoxBoxCornerImpacts(base, query, &r3);
  BoxBoxSwapForImpact(base, query);
  return true;
}

bool QueryFindImpact(uint8_t* base, uint32_t query, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BCE80 Query::FindImpact");
  // Both boxes (type 2): entry 8 of the table.
  if (!CurrentBoxes(base, query) ||
      guest::LoadU32(base, kImpactTests + 4 * (2 * 3 + 2)) != kBoxBoxFindImpact) {
    return false;
  }
  CopyWord(base, kZero, query + 80);
  CopyWord(base, kTwo, query + 84);
  CopyWord(base, kTwo, query + 88);
  BoxBoxFindImpact(base, query);
  *result = Lfs(base, query + 84) < Lfs(base, kImpactLimit) ? 1 : 0;
  return true;
}

// --- The push-outs ---

namespace {

// sub_820BE0D8 (never ran in play; done here as PushOutCorners' own work):
// along one axis of the other box's unit box, the distance `scale` x
// `direction` past the corner's way out (1.0 - c, or c + 1.0 going the
// other way); when larger, `best` = the exit's fraction and `face` = the
// face (`plus` or `minus`).
void AxisExit(uint8_t* base, uint32_t best, uint32_t face, double direction, double c,
              uint32_t plus, uint32_t minus) {
  const double zero = Lfs(base, kZero);
  if (!(direction > zero)) {
    if (!(direction < zero)) {
      return;
    }
    const double v = Fmuls(Lfs(base, best), direction);
    const double limit = Fadds(c, Lfs(base, kOne));
    if (!(Fneg(v) > limit)) {
      return;
    }
    Stfs(base, best, Fneg(Fdivs(limit, direction)));
    guest::StoreU32(base, face, minus);
    return;
  }
  const double v = Fmuls(Lfs(base, best), direction);
  const double limit = Fsubs(Lfs(base, kOne), c);
  if (!(v > limit)) {
    return;
  }
  Stfs(base, best, Fdivs(limit, direction));
  guest::StoreU32(base, face, plus);
}

// The crossing test of one of the 12 edges' ends (the corners at +80 and
// +88's coordinates `u`, `v`, `w` of each, a corner 32 bytes): the edge
// crosses the unit box's face where its coordinate `u` reaches `limit`
// (fsubs u - 1.0, or fadds u + 1.0), with the other two within 1.0.
bool CrossesFace(uint8_t* base, uint32_t from, uint32_t to, uint32_t u, uint32_t v,
                 uint32_t w, bool plus_side) {
  const double one = Lfs(base, kOne);
  const double u0 = Lfs(base, from + u);
  const double reach = plus_side ? Fsubs(u0, one) : Fadds(u0, one);
  const double t = Fdivs(reach, Fsubs(u0, Lfs(base, to + u)));
  const double v0 = Lfs(base, from + v);
  const double at_v = Fmadds(Fsubs(Lfs(base, to + v), v0), t, v0);
  if (Fabs(at_v) > one) {
    return false;
  }
  const double w0 = Lfs(base, from + w);
  const double at_w = Fmadds(Fsubs(Lfs(base, to + w), w0), t, w0);
  return !(Fabs(at_w) > one);
}

}  // namespace

uint32_t BoxBoxEdgesCrossing(uint8_t* base, uint32_t out, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820BF080 BoxBox::EdgesCrossing");
  uint32_t any = 0;
  for (uint32_t edge = 0; edge < 12; edge++, out += 4) {
    const uint32_t from = (BoxEdgesStart(base, edge) << 5) + block;
    const uint32_t to = (BoxEdgesEnd(base, edge) << 5) + block;
    guest::StoreU32(base, out, 0);
    const uint32_t a = guest::LoadU32(base, from + 96), b = guest::LoadU32(base, to + 96);
    if ((a & b) != 0 || (a | b) == 0) {
      continue;
    }
    const uint32_t bits = a ^ b;
    if (bits == 0) {
      continue;
    }
    guest::StoreU32(base, out, 1);
    // x, y, z at +80, +84, +88 of the corner (from the block: +80 + 32k).
    if (((bits & 1) != 0 && CrossesFace(base, from, to, 80, 84, 88, true)) ||
        ((bits & 2) != 0 && CrossesFace(base, from, to, 80, 84, 88, false)) ||
        ((bits & 4) != 0 && CrossesFace(base, from, to, 84, 80, 88, true)) ||
        ((bits & 8) != 0 && CrossesFace(base, from, to, 84, 80, 88, false)) ||
        ((bits & 16) != 0 && CrossesFace(base, from, to, 88, 84, 80, true)) ||
        ((bits & 32) != 0 && CrossesFace(base, from, to, 88, 84, 80, false))) {
      any = 1;
      continue;
    }
    guest::StoreU32(base, out, 0);
  }
  return any;
}

bool BoxBoxPushOutEdges(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BF320 BoxBox::PushOutEdges");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  const uint32_t f = frame;
  uint32_t a = 0, b = 0;
  CollisionShapeCurrent(base, guest::LoadU32(base, query + 0), &a);
  CollisionShapeCurrent(base, guest::LoadU32(base, query + 4), &b);
  // A's edges crossing B's unit box (block +20), B's crossing A's (+12).
  *r3 = 0;
  if (BoxBoxEdgesCrossing(base, f + 272, guest::LoadU32(base, query + 20)) == 0) {
    return true;
  }
  if (BoxBoxEdgesCrossing(base, f + 320, guest::LoadU32(base, query + 12)) == 0) {
    return true;
  }
  *r3 = 1;
  const double limit = Lfs(base, kEdgeDepthLimit);
  const double half = Lfs(base, kHalf);
  double best = limit;
  const double floor = std::bit_cast<double>(
      uint64_t(guest::LoadU32(base, kEdgeSeparationFloor)) << 32 |
      guest::LoadU32(base, kEdgeSeparationFloor + 4));
  const double zero = Lfs(base, kZero);
  CopyWord(base, kZero, f + 156);
  CopyWord(base, kZero, f + 172);
  const double minus_one = Lfs(base, kMinusOne);
  for (uint32_t i = 0; i < 12; i++) {
    if (guest::LoadU32(base, f + 272 + 4 * i) == 0) {
      continue;
    }
    for (uint32_t j = 0; j < 12; j++) {
      if (guest::LoadU32(base, f + 320 + 4 * j) == 0) {
        continue;
      }
      const uint32_t a0 = BoxCorner(a, BoxEdgeStart(base, i));
      const uint32_t a1 = BoxCorner(a, BoxEdgeEnd(base, i));
      const uint32_t b0 = BoxCorner(b, BoxEdgeStart(base, j));
      const uint32_t b1 = BoxCorner(b, BoxEdgeEnd(base, j));
      *r3 = b1;
      // The normal: the edges' cross product, normalized.
      vl::Vec3Difference(base, f + 112, a1, a0);
      vl::Vec3Difference(base, f + 128, b1, b0);
      vl::Vec3Cross(base, f + 80, f + 112, f + 128);
      *r3 = f + 80;
      if (double(vl::Vec3Dot(base, f + 80, f + 80)) == zero) {
        continue;
      }
      vl::Vec3NormalizeSelf(base, f + 80);
      // Turned away from B's edge's outward direction.
      const uint32_t outward = BoxFaceDirection(base, b, j, f - 208);
      if (double(vl::Vec3DotOf(base, f + 80, outward)) > zero) {
        vl::Vec4Scaled(base, f + 176, f + 80, minus_one);
        vl::Vec4Copy(base, f + 80, f + 176);
      }
      // The depth along it: (B's start - A's start) . normal.
      vl::Vec3Difference(base, f + 208, b0, a0);
      const double depth = double(vl::Vec3DotOf(base, f + 208, f + 80));
      *r3 = f + 208;
      if (depth < floor) {
        continue;
      }
      // The edges' middles no further apart than either edge's length.
      vl::Vec3Sum(base, f + 256, b0, b1);
      vl::Vec3Sum(base, f + 240, a0, a1);
      vl::Vec3Difference(base, f + 192, f + 240, f + 256);
      vl::Vec4Scaled(base, f + 96, f + 192, half);
      double apart = double(vl::Vec3Dot(base, f + 96, f + 96));
      double length = double(vl::Vec3Dot(base, f + 112, f + 112));
      *r3 = f + 112;
      if (apart > length) {
        apart = double(vl::Vec3Dot(base, f + 96, f + 96));
        length = double(vl::Vec3Dot(base, f + 128, f + 128));
        *r3 = f + 128;
        if (apart > length) {
          continue;
        }
      }
      // Along the push direction, and shallower than the best so far.
      *r3 = query + 96;
      if (!(double(vl::Vec3DotOf(base, query + 96, f + 80)) > zero)) {
        continue;
      }
      if (!(depth < best)) {
        continue;
      }
      best = depth;
      vl::Vec4Copy(base, f + 144, f + 80);
      vl::Vec3Lerp(base, f + 224, b0, b1, half);
      vl::Vec4Copy(base, f + 160, f + 224);
      *r3 = f + 160;
    }
  }
  if (!(best < limit) || !(best > Lfs(base, query + 80))) {
    return true;
  }
  ContactSet(base, query + 32, f + 160, f + 144, best, zero);
  *r3 = query + 64;
  return true;
}

bool BoxBoxPushOutCorners(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BE150 BoxBox::PushOutCorners");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  const uint32_t f = frame;
  uint32_t a = 0, b = 0;
  CollisionShapeCurrent(base, guest::LoadU32(base, query + 0), &a);
  CollisionShapeCurrent(base, guest::LoadU32(base, query + 4), &b);
  const double zero = Lfs(base, kZero);
  CopyWord(base, kZero, f + 108);
  const uint32_t direction = query + 96;
  // The direction in B's unit box: per face normal, its dot product x B's
  // inverse half extent.
  for (uint32_t i = 0; i < 3; i++) {
    const double d = double(vl::Vec3DotOf(base, BoxFace(b, i) + 16, direction));
    const uint32_t inverse = vehicle_accessors::RigidBodyField144(b);
    Stfs(base, f + 96 + 4 * i, Fmuls(Lfs(base, inverse + 4 * i), d));
  }
  *r3 = f + 96;
  if (double(vl::Vec3Dot(base, f + 96, f + 96)) == zero) {
    return true;
  }
  // A's corners inside B (outcode 0 in block +20): the farthest way out.
  const uint32_t block = guest::LoadU32(base, query + 20);
  double best = zero;
  uint32_t best_face = 0xFFFFFFFFu, best_corner = 0xFFFFFFFFu;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t corner = block + 80 + 32 * k;
    if (guest::LoadU32(base, corner + 16) != 0) {
      continue;
    }
    CopyWord(base, kCornerStart, f + 80);
    guest::StoreU32(base, f + 84, 0xFFFFFFFFu);
    for (uint32_t axis = 0; axis < 3; axis++) {
      AxisExit(base, f + 80, f + 84, Lfs(base, f + 96 + 4 * axis),
               Lfs(base, corner + 4 * axis), axis, axis + 3);
    }
    *r3 = f + 80;
    const double exit = Lfs(base, f + 80);
    if (!(best < exit)) {
      continue;
    }
    best_face = guest::LoadU32(base, f + 84);
    best = exit;
    best_corner = k;
  }
  if (!(best > zero)) {
    return true;
  }
  // Its length back in world units, along B's normals by B's half extents.
  vl::Vec4Scaled(base, f + 112, f + 96, best);
  const uint32_t extents_z = vehicle_accessors::RigidBodyTorque(b);
  vl::Vec4Scaled(base, f + 144, BoxFace(b, 2) + 16,
                 Fmuls(Lfs(base, extents_z + 8), Lfs(base, f + 120)));
  const uint32_t extents_y = vehicle_accessors::RigidBodyTorque(b);
  vl::Vec4Scaled(base, f + 160, BoxFace(b, 1) + 16,
                 Fmuls(Lfs(base, extents_y + 4), Lfs(base, f + 116)));
  const uint32_t extents_x = vehicle_accessors::RigidBodyTorque(b);
  vl::Vec4Scaled(base, f + 176, BoxFace(b, 0) + 16,
                 Fmuls(Lfs(base, extents_x + 0), Lfs(base, f + 112)));
  vl::Vec3Sum(base, f + 192, f + 176, f + 160);
  vl::Vec3Sum(base, f + 128, f + 192, f + 144);
  const double depth = double(vl::Vec3Length(base, f + 128));
  *r3 = f + 128;
  if (!(depth > Lfs(base, query + 80))) {
    return true;
  }
  // sub_820BC8A8 (never ran in play; done here): the contact = A's corner,
  // B's face's normal, the push direction, the depth and time 0.0.
  const uint32_t face = BoxFace(b, best_face);
  const uint32_t point = BoxCorner(a, best_corner);
  const uint32_t contact = query + 32;
  vl::Vec4Copy(base, contact + 0, point);
  vl::Vec4Copy(base, contact + 16, face + 16);
  vl::Vec4Copy(base, contact + 32, direction);
  Stfs(base, contact + 48, depth);
  Stfs(base, contact + 52, zero);
  *r3 = contact + 32;
  return true;
}

bool BoxBoxFindPushOut(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BFB28 BoxBox::FindPushOut");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  *r3 = query;
  if (guest::LoadU32(base, guest::LoadU32(base, query + 12) + 64) != 0 ||
      guest::LoadU32(base, guest::LoadU32(base, query + 20) + 64) != 0) {
    return true;
  }
  BoxBoxPushOutCorners(base, query, frame - 304, r3);
  BoxBoxSwapForPushOut(base, query);
  BoxBoxPushOutCorners(base, query, frame - 304, r3);
  *r3 = BoxBoxSwapForPushOut(base, query);
  if (Lfs(base, query + 80) > Lfs(base, kZero)) {
    return true;
  }
  BoxBoxPushOutEdges(base, query, frame - 544, r3);
  return true;
}

bool QueryFindPushOut(uint8_t* base, uint32_t query, uint32_t direction, uint32_t frame,
                      uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BCDD0 Query::FindPushOut");
  // Both boxes (type 2): entry 8 of the table.
  if (!CurrentBoxes(base, query) ||
      guest::LoadU32(base, kPushOutTests + 4 * (2 * 3 + 2)) != kBoxBoxFindPushOut) {
    return false;
  }
  vl::Vec4Copy(base, query + 96, direction);
  CopyWord(base, kZero, query + 80);
  CopyWord(base, kTwo, query + 84);
  CopyWord(base, kTwo, query + 88);
  uint32_t r3 = 0;
  BoxBoxFindPushOut(base, query, frame - 96, &r3);
  *result = Lfs(base, query + 80) > Lfs(base, kZero) ? 1 : 0;
  return true;
}

}  // namespace collision
