// The impacts: a corner of one box crossing a face of the other between
// the two placements (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "schema/collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

using G = schema::CollisionGlobals;
using Box = schema::BoxShape;
using Block = schema::CollisionBlock;
using Query = schema::CollisionQuery;
using Record = schema::CollisionShape;
using Contact = schema::QueryContact;
// The impact and push-out tests' box-box entries' functions (the tables'
// entry 8, G::impact_tests and G::push_out_tests).
constexpr uint32_t kBoxBoxFindImpact = 0x820BF038;
constexpr uint32_t kBoxBoxFindPushOut = 0x820BFB28;

void SwapWords(uint32_t a, uint32_t b) {
  const uint32_t at_b = guest::LoadU32(b);
  const uint32_t at_a = guest::LoadU32(a);
  guest::StoreU32(a, at_b);
  guest::StoreU32(b, at_a);
}

// Both records' templates and current copies are boxes.
bool CurrentBoxes(uint32_t query) {
  const Query q(query);
  for (uint32_t at : {q.shape_a_address(), q.shape_b_address()}) {
    const Record record(guest::LoadU32(at));
    if (!IsBox(record.template_shape().address) ||
        !IsBox(record.current().address)) {
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
bool CrossesFace(uint32_t p, uint32_t c, uint32_t axis, uint32_t u, uint32_t v,
                 bool positive, double one, double* best) {
  const double pa = Lfs(p + axis);
  double move, t0;
  if (positive) {
    move = Fsubs(pa, Lfs(c + axis));
    t0 = Fsubs(pa, one);
  } else {
    move = Fsubs(Lfs(c + axis), pa);
    t0 = Fneg(Fadds(pa, one));
  }
  const double pu = Lfs(p + u);
  const double pv = Lfs(p + v);
  if (!(t0 < Fmuls(move, *best))) {
    return false;
  }
  const double t = Fdivs(t0, move);
  if (!(Fabs(Fmadds(Fsubs(Lfs(c + u), pu), t, pu)) < one)) {
    return false;
  }
  if (!(Fabs(Fmadds(Fsubs(Lfs(c + v), pv), t, pv)) < one)) {
    return false;
  }
  *best = t;
  return true;
}

}  // namespace

uint32_t BoxBoxSwapForImpact(uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BE490 BoxBox::SwapForImpact");
  const Query q(query);
  ContactFlip(q.contact_point_x_address());
  SwapWords(q.shape_a_address(), q.shape_b_address());
  SwapWords(q.b_in_a_current_address(), q.a_in_b_current_address());
  SwapWords(q.b_in_a_previous_address(), q.a_in_b_previous_address());
  return q.contact_push_x_address();
}

uint32_t BoxBoxSwapForPushOut(uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BEBF0 BoxBox::SwapForPushOut");
  const Query q(query);
  vl::Vec4ScaleBy(q.direction_x_address(), double(G::minus_one()));
  ContactFlip(q.contact_point_x_address());
  SwapWords(q.shape_a_address(), q.shape_b_address());
  SwapWords(q.b_in_a_current_address(), q.a_in_b_current_address());
  return q.contact_push_x_address();
}

bool BoxBoxCornerImpacts(uint32_t query, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BECA0 BoxBox::CornerImpacts");
  if (!CurrentBoxes(query)) {
    return false;
  }
  const Query q(query);
  *r3 = query;
  double best = double(q.contact_time());
  const double one = Lfs(kOne);
  for (uint32_t k = 0; k < 8; k++) {
    // A's corner k in B's unit box, at the previous and current placements.
    const Block previous = q.a_in_b_previous();
    const Block current = q.a_in_b_current();
    const uint32_t previous_code = guest::LoadU32(previous.corner_outcode_address() + 32 * k);
    const uint32_t current_code = guest::LoadU32(current.corner_outcode_address() + 32 * k);
    if ((previous_code & current_code) != 0) {
      continue;
    }
    const uint32_t left = previous_code & ~current_code;
    if (left == 0) {
      continue;
    }
    const uint32_t p = previous.corner_x_address() + 32 * k;
    const uint32_t c = current.corner_x_address() + 32 * k;
    uint32_t face = 0;
    if (left & 1) {
      CrossesFace(p, c, 0, 4, 8, true, one, &best);
    } else if (left & 2) {
      if (CrossesFace(p, c, 0, 4, 8, false, one, &best)) {
        face = 3;
      }
    }
    if (left & 4) {
      if (CrossesFace(p, c, 4, 0, 8, true, one, &best)) {
        face = 1;
      }
    } else if (left & 8) {
      if (CrossesFace(p, c, 4, 0, 8, false, one, &best)) {
        face = 4;
      }
    }
    if (left & 16) {
      if (CrossesFace(p, c, 8, 0, 4, true, one, &best)) {
        face = 2;
      }
    } else if (left & 32) {
      if (CrossesFace(p, c, 8, 0, 4, false, one, &best)) {
        face = 5;
      }
    }
    if (!(best < double(q.contact_time()))) {
      continue;
    }
    // The contact: A's current corner, B's face's normal, the depth the
    // corner is in by, the time.
    const uint32_t copy_a = q.shape_a().current().address;
    const uint32_t copy_b = q.shape_b().current().address;
    uint32_t normal = 0;
    const double distance =
        BoxFaceDistance(copy_b, face, BoxCorner(copy_a, k), &normal);
    const uint32_t face_at = BoxFace(copy_b, face);
    ContactSet(q.contact_point_x_address(), BoxCorner(copy_a, k),
               schema::BoxShapeFace(face_at).normal_x_address(), Fneg(distance), best);
    *r3 = q.contact_push_x_address();
  }
  return true;
}

bool BoxBoxFindImpact(uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BF038 BoxBox::FindImpact");
  if (!CurrentBoxes(query)) {
    return false;
  }
  uint32_t r3 = 0;
  BoxBoxCornerImpacts(query, &r3);
  BoxBoxSwapForImpact(query);
  BoxBoxCornerImpacts(query, &r3);
  BoxBoxSwapForImpact(query);
  return true;
}

bool QueryFindImpact(uint32_t query, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BCE80 Query::FindImpact");
  // Both boxes (type 2): entry 8 of the table.
  if (!CurrentBoxes(query) ||
      guest::LoadU32(G::impact_tests_address() + 4 * (2 * 3 + 2)) != kBoxBoxFindImpact) {
    return false;
  }
  const Query q(query);
  CopyWord(kZero, q.contact_depth_address());
  CopyWord(G::two_address(), q.contact_time_address());
  CopyWord(G::two_address(), q.unknown_88_address());
  BoxBoxFindImpact(query);
  *result = double(q.contact_time()) < double(G::impact_time_limit()) ? 1 : 0;
  return true;
}

// --- The push-outs ---

namespace {

// sub_820BE0D8 (never ran in play; done here as PushOutCorners' own work):
// along one axis of the other box's unit box, the distance `scale` x
// `direction` past the corner's way out (1.0 - c, or c + 1.0 going the
// other way); when larger, `best` = the exit's fraction and `face` = the
// face (`plus` or `minus`).
void AxisExit(uint32_t best, uint32_t face, double direction, double c,
              uint32_t plus, uint32_t minus) {
  const double zero = Lfs(kZero);
  if (!(direction > zero)) {
    if (!(direction < zero)) {
      return;
    }
    const double v = Fmuls(Lfs(best), direction);
    const double limit = Fadds(c, Lfs(kOne));
    if (!(Fneg(v) > limit)) {
      return;
    }
    Stfs(best, Fneg(Fdivs(limit, direction)));
    guest::StoreU32(face, minus);
    return;
  }
  const double v = Fmuls(Lfs(best), direction);
  const double limit = Fsubs(Lfs(kOne), c);
  if (!(v > limit)) {
    return;
  }
  Stfs(best, Fdivs(limit, direction));
  guest::StoreU32(face, plus);
}

// The crossing test of one of the 12 edges' ends (the corners `from` and
// `to`, their coordinates at `u`, `v`, `w`, a corner 32 bytes): the edge
// crosses the unit box's face where its coordinate `u` reaches `limit`
// (fsubs u - 1.0, or fadds u + 1.0), with the other two within 1.0.
bool CrossesFace(uint32_t from, uint32_t to, uint32_t u, uint32_t v,
                 uint32_t w, bool plus_side) {
  const double one = Lfs(kOne);
  const double u0 = Lfs(from + u);
  const double reach = plus_side ? Fsubs(u0, one) : Fadds(u0, one);
  const double t = Fdivs(reach, Fsubs(u0, Lfs(to + u)));
  const double v0 = Lfs(from + v);
  const double at_v = Fmadds(Fsubs(Lfs(to + v), v0), t, v0);
  if (Fabs(at_v) > one) {
    return false;
  }
  const double w0 = Lfs(from + w);
  const double at_w = Fmadds(Fsubs(Lfs(to + w), w0), t, w0);
  return !(Fabs(at_w) > one);
}

}  // namespace

uint32_t BoxBoxEdgesCrossing(uint32_t out, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820BF080 BoxBox::EdgesCrossing");
  uint32_t any = 0;
  for (uint32_t edge = 0; edge < 12; edge++, out += 4) {
    // The edge's ends: two of the block's corners (32 bytes a corner).
    const Block corners(block);
    const uint32_t start = BoxEdgesStart(edge) << 5;
    const uint32_t end = BoxEdgesEnd(edge) << 5;
    const uint32_t from = corners.corner_x_address() + start;
    const uint32_t to = corners.corner_x_address() + end;
    guest::StoreU32(out, 0);
    const uint32_t a = guest::LoadU32(corners.corner_outcode_address() + start);
    const uint32_t b = guest::LoadU32(corners.corner_outcode_address() + end);
    if ((a & b) != 0 || (a | b) == 0) {
      continue;
    }
    const uint32_t bits = a ^ b;
    if (bits == 0) {
      continue;
    }
    guest::StoreU32(out, 1);
    // x, y, z at +0, +4, +8 of the corner.
    if (((bits & 1) != 0 && CrossesFace(from, to, 0, 4, 8, true)) ||
        ((bits & 2) != 0 && CrossesFace(from, to, 0, 4, 8, false)) ||
        ((bits & 4) != 0 && CrossesFace(from, to, 4, 0, 8, true)) ||
        ((bits & 8) != 0 && CrossesFace(from, to, 4, 0, 8, false)) ||
        ((bits & 16) != 0 && CrossesFace(from, to, 8, 4, 0, true)) ||
        ((bits & 32) != 0 && CrossesFace(from, to, 8, 4, 0, false))) {
      any = 1;
      continue;
    }
    guest::StoreU32(out, 0);
  }
  return any;
}

bool BoxBoxPushOutEdges(uint32_t query, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BF320 BoxBox::PushOutEdges");
  if (!CurrentBoxes(query)) {
    return false;
  }
  const Query q(query);
  const uint32_t f = frame;
  uint32_t a = 0, b = 0;
  CollisionShapeCurrent(q.shape_a().address, &a);
  CollisionShapeCurrent(q.shape_b().address, &b);
  // A's edges crossing B's unit box (block +20), B's crossing A's (+12).
  *r3 = 0;
  if (BoxBoxEdgesCrossing(f + 272, q.a_in_b_current().address) == 0) {
    return true;
  }
  if (BoxBoxEdgesCrossing(f + 320, q.b_in_a_current().address) == 0) {
    return true;
  }
  *r3 = 1;
  const double limit = double(G::edge_depth_limit());
  const double half = double(G::half());
  double best = limit;
  const double floor = std::bit_cast<double>(
      uint64_t(guest::LoadU32(G::edge_separation_floor_address())) << 32 |
      guest::LoadU32(G::edge_separation_floor_address() + 4));
  const double zero = Lfs(kZero);
  CopyWord(kZero, f + 156);
  CopyWord(kZero, f + 172);
  const double minus_one = double(G::minus_one());
  for (uint32_t i = 0; i < 12; i++) {
    if (guest::LoadU32(f + 272 + 4 * i) == 0) {
      continue;
    }
    for (uint32_t j = 0; j < 12; j++) {
      if (guest::LoadU32(f + 320 + 4 * j) == 0) {
        continue;
      }
      const uint32_t a0 = BoxCorner(a, BoxEdgeStart(i));
      const uint32_t a1 = BoxCorner(a, BoxEdgeEnd(i));
      const uint32_t b0 = BoxCorner(b, BoxEdgeStart(j));
      const uint32_t b1 = BoxCorner(b, BoxEdgeEnd(j));
      *r3 = b1;
      // The normal: the edges' cross product, normalized.
      vl::Vec3Difference(f + 112, a1, a0);
      vl::Vec3Difference(f + 128, b1, b0);
      vl::Vec3Cross(f + 80, f + 112, f + 128);
      *r3 = f + 80;
      if (double(vl::Vec3Dot(f + 80, f + 80)) == zero) {
        continue;
      }
      vl::Vec3NormalizeSelf(f + 80);
      // Turned away from B's edge's outward direction.
      const uint32_t outward = BoxFaceDirection(b, j, f - 208);
      if (double(vl::Vec3DotOf(f + 80, outward)) > zero) {
        vl::Vec4Scaled(f + 176, f + 80, minus_one);
        vl::Vec4Copy(f + 80, f + 176);
      }
      // The depth along it: (B's start - A's start) . normal.
      vl::Vec3Difference(f + 208, b0, a0);
      const double depth = double(vl::Vec3DotOf(f + 208, f + 80));
      *r3 = f + 208;
      if (depth < floor) {
        continue;
      }
      // The edges' middles no further apart than either edge's length.
      vl::Vec3Sum(f + 256, b0, b1);
      vl::Vec3Sum(f + 240, a0, a1);
      vl::Vec3Difference(f + 192, f + 240, f + 256);
      vl::Vec4Scaled(f + 96, f + 192, half);
      double apart = double(vl::Vec3Dot(f + 96, f + 96));
      double length = double(vl::Vec3Dot(f + 112, f + 112));
      *r3 = f + 112;
      if (apart > length) {
        apart = double(vl::Vec3Dot(f + 96, f + 96));
        length = double(vl::Vec3Dot(f + 128, f + 128));
        *r3 = f + 128;
        if (apart > length) {
          continue;
        }
      }
      // Along the push direction, and shallower than the best so far.
      *r3 = q.direction_x_address();
      if (!(double(vl::Vec3DotOf(q.direction_x_address(), f + 80)) > zero)) {
        continue;
      }
      if (!(depth < best)) {
        continue;
      }
      best = depth;
      vl::Vec4Copy(f + 144, f + 80);
      vl::Vec3Lerp(f + 224, b0, b1, half);
      vl::Vec4Copy(f + 160, f + 224);
      *r3 = f + 160;
    }
  }
  if (!(best < limit) || !(best > double(q.contact_depth()))) {
    return true;
  }
  ContactSet(q.contact_point_x_address(), f + 160, f + 144, best, zero);
  *r3 = q.contact_push_x_address();
  return true;
}

bool BoxBoxPushOutCorners(uint32_t query, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BE150 BoxBox::PushOutCorners");
  if (!CurrentBoxes(query)) {
    return false;
  }
  const Query q(query);
  const uint32_t f = frame;
  uint32_t a = 0, b = 0;
  CollisionShapeCurrent(q.shape_a().address, &a);
  CollisionShapeCurrent(q.shape_b().address, &b);
  const double zero = Lfs(kZero);
  CopyWord(kZero, f + 108);
  const uint32_t direction = q.direction_x_address();
  // The direction in B's unit box: per face normal, its dot product x B's
  // inverse half extent.
  for (uint32_t i = 0; i < 3; i++) {
    const double d = double(vl::Vec3DotOf(schema::BoxShapeFace(BoxFace(b, i)).normal_x_address(), direction));
    const schema::CollisionVector inverse(vehicle_accessors::RigidBodyField144(b));
    Stfs(f + 96 + 4 * i, Fmuls(Lfs(inverse.x_address() + 4 * i), d));
  }
  *r3 = f + 96;
  if (double(vl::Vec3Dot(f + 96, f + 96)) == zero) {
    return true;
  }
  // A's corners inside B (outcode 0 in block +20): the farthest way out.
  const Block block = q.a_in_b_current();
  double best = zero;
  uint32_t best_face = 0xFFFFFFFFu, best_corner = 0xFFFFFFFFu;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t corner = block.corner_x_address() + 32 * k;
    if (guest::LoadU32(block.corner_outcode_address() + 32 * k) != 0) {
      continue;
    }
    CopyWord(G::corner_start_address(), f + 80);
    guest::StoreU32(f + 84, 0xFFFFFFFFu);
    for (uint32_t axis = 0; axis < 3; axis++) {
      AxisExit(f + 80, f + 84, Lfs(f + 96 + 4 * axis),
               Lfs(corner + 4 * axis), axis, axis + 3);
    }
    *r3 = f + 80;
    const double exit = Lfs(f + 80);
    if (!(best < exit)) {
      continue;
    }
    best_face = guest::LoadU32(f + 84);
    best = exit;
    best_corner = k;
  }
  if (!(best > zero)) {
    return true;
  }
  // Its length back in world units, along B's normals by B's half extents.
  vl::Vec4Scaled(f + 112, f + 96, best);
  const schema::CollisionVector extents_z(vehicle_accessors::RigidBodyTorque(b));
  vl::Vec4Scaled(f + 144, schema::BoxShapeFace(BoxFace(b, 2)).normal_x_address(),
                 Fmuls(double(extents_z.z()), Lfs(f + 120)));
  const schema::CollisionVector extents_y(vehicle_accessors::RigidBodyTorque(b));
  vl::Vec4Scaled(f + 160, schema::BoxShapeFace(BoxFace(b, 1)).normal_x_address(),
                 Fmuls(double(extents_y.y()), Lfs(f + 116)));
  const schema::CollisionVector extents_x(vehicle_accessors::RigidBodyTorque(b));
  vl::Vec4Scaled(f + 176, schema::BoxShapeFace(BoxFace(b, 0)).normal_x_address(),
                 Fmuls(double(extents_x.x()), Lfs(f + 112)));
  vl::Vec3Sum(f + 192, f + 176, f + 160);
  vl::Vec3Sum(f + 128, f + 192, f + 144);
  const double depth = double(vl::Vec3Length(f + 128));
  *r3 = f + 128;
  if (!(depth > double(q.contact_depth()))) {
    return true;
  }
  // sub_820BC8A8 (never ran in play; done here): the contact = A's corner,
  // B's face's normal, the push direction, the depth and time 0.0.
  const schema::BoxShapeFace face(BoxFace(b, best_face));
  const uint32_t point = BoxCorner(a, best_corner);
  const Contact contact(q.contact_point_x_address());
  vl::Vec4Copy(contact.point_x_address(), point);
  vl::Vec4Copy(contact.normal_x_address(), face.normal_x_address());
  vl::Vec4Copy(contact.push_x_address(), direction);
  contact.set_depth(float(depth));
  contact.set_time(float(zero));
  *r3 = contact.push_x_address();
  return true;
}

bool BoxBoxFindPushOut(uint32_t query, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BFB28 BoxBox::FindPushOut");
  if (!CurrentBoxes(query)) {
    return false;
  }
  const Query q(query);
  *r3 = query;
  if (q.b_in_a_current().outcode() != 0 || q.a_in_b_current().outcode() != 0) {
    return true;
  }
  BoxBoxPushOutCorners(query, frame - 304, r3);
  BoxBoxSwapForPushOut(query);
  BoxBoxPushOutCorners(query, frame - 304, r3);
  *r3 = BoxBoxSwapForPushOut(query);
  if (double(q.contact_depth()) > Lfs(kZero)) {
    return true;
  }
  BoxBoxPushOutEdges(query, frame - 544, r3);
  return true;
}

bool QueryFindPushOut(uint32_t query, uint32_t direction, uint32_t frame,
                      uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BCDD0 Query::FindPushOut");
  // Both boxes (type 2): entry 8 of the table.
  if (!CurrentBoxes(query) ||
      guest::LoadU32(G::push_out_tests_address() + 4 * (2 * 3 + 2)) !=
          kBoxBoxFindPushOut) {
    return false;
  }
  const Query q(query);
  vl::Vec4Copy(q.direction_x_address(), direction);
  CopyWord(kZero, q.contact_depth_address());
  CopyWord(G::two_address(), q.contact_time_address());
  CopyWord(G::two_address(), q.unknown_88_address());
  uint32_t r3 = 0;
  BoxBoxFindPushOut(query, frame - 96, &r3);
  *result = double(q.contact_depth()) > Lfs(kZero) ? 1 : 0;
  return true;
}

}  // namespace collision
