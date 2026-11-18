// The box-box separation tests and the pair's next check time
// (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "schema/collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;

namespace {

using G = schema::CollisionGlobals;
using Box = schema::BoxShape;
using Block = schema::CollisionBlock;
using Query = schema::CollisionQuery;
using Record = schema::CollisionShape;
using Object = schema::CollisionPairObject;

// A vector's x, y, z (their bits) into a temporary at `at`, its w 0.0, in
// the originals' order (x, w, z, y).
void CopyToTemporary(uint8_t* base, uint32_t at, uint32_t from) {
  CopyWord(base, from + 0, at + 0);
  CopyWord(base, kZero, at + 12);
  CopyWord(base, from + 8, at + 8);
  CopyWord(base, from + 4, at + 4);
}

// The outcode bits of one coordinate: `above` when > 1.0, else `below`
// when < -1.0 (`bgt`, `blt`: NaNs set neither).
uint32_t CoordinateBits(double value, double one, double minus_one, uint32_t above,
                        uint32_t below) {
  if (value > one) {
    return above;
  }
  if (value < minus_one) {
    return below;
  }
  return 0;
}

// A block's corner k's outcode, as the originals set it.
void CornerOutcode(uint8_t* base, const Block& block, uint32_t k, double one, double minus_one) {
  const uint32_t corner = block.corner_x_address() + 32 * k;
  const uint32_t outcode = block.corner_outcode_address() + 32 * k;
  guest::StoreU32(base, outcode, 0);
  for (uint32_t axis = 0; axis < 3; axis++) {
    const uint32_t bits =
        CoordinateBits(Lfs(base, corner + 4 * axis), one, minus_one, 1u << (2 * axis),
                       2u << (2 * axis));
    if (bits) {
      guest::StoreU32(base, outcode, guest::LoadU32(base, outcode) | bits);
    }
  }
}

// The face index of the lowest bit of six cache bits (+x, -x, +y, -y, +z,
// -z: faces 0, 3, 1, 4, 2, 5).
uint32_t CachedFace(uint32_t bits) {
  if (bits & 1) {
    return 0;
  }
  if (bits & 2) {
    return 3;
  }
  if (bits & 4) {
    return 1;
  }
  if (bits & 8) {
    return 4;
  }
  return (bits & 0x10) ? 2 : 5;
}

// The four copies a query's shape records hold (A's previous and current,
// B's), when both templates and all four copies are boxes.
struct Copies {
  uint32_t previous_a, current_a, previous_b, current_b;
};
bool BoxCopies(uint8_t* base, uint32_t query, Copies* copies) {
  const Query q(base, query);
  const Record record_a = q.shape_a();
  const Record record_b = q.shape_b();
  if (!IsBox(base, record_a.template_shape().address) ||
      !IsBox(base, record_b.template_shape().address)) {
    return false;
  }
  copies->previous_a = record_a.previous().address;
  copies->current_a = record_a.current().address;
  copies->previous_b = record_b.previous().address;
  copies->current_b = record_b.current().address;
  return true;
}

}  // namespace

uint32_t BoxRelativePlacement(uint8_t* base, uint32_t a, uint32_t block, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BA408 BoxShape::RelativePlacement");
  const Box box_a(base, a), box_b(base, b);
  const Block out(base, block);
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    const double dot = double(
        vl::Vec3DotOf(base, box_b.center_x_address(), box_a.face_px_normal_x_address() + 32 * i));
    const double distance = Fsubs(dot, Lfs(base, box_a.face_px_distance_address() + 32 * i));
    Stfs(base, out.center_x_address() + 4 * i,
         Fmadds(distance, Lfs(base, box_a.inverse_half_extent_x_address() + 4 * i), one));
  }
  // Entry (j, i): dot(B's normal j, A's normal i) x A's inverse half extent
  // i x B's half extent j, the products in the original's orders.
  const uint32_t bx = box_b.face_px_normal_x_address(), by = box_b.face_py_normal_x_address(),
                 bz = box_b.face_pz_normal_x_address();
  const uint32_t ax = box_a.face_px_normal_x_address(), ay = box_a.face_py_normal_x_address(),
                 az = box_a.face_pz_normal_x_address();
  double d;
  d = double(vl::Vec3DotOf(base, bx, ax));
  out.set_row0_x(float(Fmuls(
      Fmuls(double(box_a.inverse_half_extent_x()), double(box_b.half_extent_x())), d)));
  d = double(vl::Vec3DotOf(base, bx, ay));
  out.set_row0_y(float(Fmuls(Fmuls(double(box_b.half_extent_x()), d),
                             double(box_a.inverse_half_extent_y()))));
  d = double(vl::Vec3DotOf(base, bx, az));
  out.set_row0_z(float(Fmuls(
      Fmuls(double(box_a.inverse_half_extent_z()), double(box_b.half_extent_x())), d)));
  d = double(vl::Vec3DotOf(base, by, ax));
  out.set_row1_x(float(Fmuls(Fmuls(double(box_a.inverse_half_extent_x()), d),
                             double(box_b.half_extent_y()))));
  d = double(vl::Vec3DotOf(base, by, ay));
  out.set_row1_y(float(Fmuls(Fmuls(d, double(box_b.half_extent_y())),
                             double(box_a.inverse_half_extent_y()))));
  d = double(vl::Vec3DotOf(base, by, az));
  out.set_row1_z(float(Fmuls(Fmuls(double(box_a.inverse_half_extent_z()), d),
                             double(box_b.half_extent_y()))));
  d = double(vl::Vec3DotOf(base, bz, ax));
  out.set_row2_x(float(Fmuls(Fmuls(double(box_a.inverse_half_extent_x()), d),
                             double(box_b.half_extent_z()))));
  d = double(vl::Vec3DotOf(base, bz, ay));
  out.set_row2_y(float(Fmuls(Fmuls(d, double(box_b.half_extent_z())),
                             double(box_a.inverse_half_extent_y()))));
  d = double(vl::Vec3DotOf(base, bz, az));
  out.set_row2_z(float(Fmuls(Fmuls(d, double(box_a.inverse_half_extent_z())),
                             double(box_b.half_extent_z()))));
  return bz;
}

uint32_t BoxRelativeCenter(uint8_t* base, uint32_t a, uint32_t block, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BA5D8 BoxShape::RelativeCenter");
  const Box box_a(base, a), box_b(base, b);
  const Block out(base, block);
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    const double dot = double(
        vl::Vec3DotOf(base, box_b.center_x_address(), box_a.face_px_normal_x_address() + 32 * i));
    const double distance = Fsubs(dot, Lfs(base, box_a.face_px_distance_address() + 32 * i));
    Stfs(base, out.center_x_address() + 4 * i,
         Fmadds(distance, Lfs(base, box_a.inverse_half_extent_x_address() + 4 * i), one));
  }
  return box_b.center_x_address();
}

uint32_t BoxBoxOutcode(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820BD0C8 BoxBox::Outcode");
  const Block b(base, block);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  uint32_t bits = 0;
  for (uint32_t axis = 0; axis < 3; axis++) {
    const double column1 = Fabs(Lfs(base, b.row1_x_address() + 4 * axis));
    const double column2 = Fabs(Lfs(base, b.row2_x_address() + 4 * axis));
    const double column0 = Fabs(Lfs(base, b.row0_x_address() + 4 * axis));
    const double center = Lfs(base, b.center_x_address() + 4 * axis);
    const double reach = Fadds(Fadds(Fadds(column2, column1), column0), one);
    if (Fsubs(center, reach) > zero) {
      bits |= 1u << (2 * axis);
    } else if (Fadds(center, reach) < zero) {
      bits |= 2u << (2 * axis);
    }
  }
  return bits;
}

void BoxBoxBlockCorners(uint8_t* base, uint32_t block, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BC978 BoxBox::BlockCorners");
  const Block b(base, block);
  const uint32_t row0 = b.row0_x_address(), row1 = b.row1_x_address(), row2 = b.row2_x_address();
  const uint32_t corner = b.corner_x_address();  // corner k at + 32k
  // Corner 0: the center plus the three rows.
  vl::Vec3AddCopy(base, frame + 240, b.center_x_address(), row0);
  vl::Vec3AddCopy(base, frame + 96, frame + 240, row1);
  vl::Vec3AddCopy(base, frame + 80, frame + 96, row2);
  CopyToTemporary(base, frame + 176, frame + 80);
  vl::Vec4Copy(base, corner + 0, frame + 176);
  // The others, each from one before it and twice a row: (from, row,
  // subtract, temporary, to).
  const double two = double(G::two(base));
  const struct {
    uint32_t from, row;
    bool subtract;
    uint32_t temporary, to;
  } steps[] = {
      {0, row0, true, 224, 1},  {1, row1, true, 208, 3},  {3, row0, false, 192, 2},
      {2, row2, true, 160, 6},  {6, row1, false, 112, 4}, {4, row0, true, 128, 5},
      {5, row1, true, 144, 7},
  };
  for (const auto& s : steps) {
    vl::Vec3ScaledCopy(base, frame + 80, s.row, two);
    CopyToTemporary(base, frame + s.temporary, frame + 80);
    if (s.subtract) {
      vl::Vec3Difference(base, frame + 96, corner + 32 * s.from, frame + s.temporary);
    } else {
      vl::Vec3Sum(base, frame + 96, corner + 32 * s.from, frame + s.temporary);
    }
    vl::Vec4Copy(base, corner + 32 * s.to, frame + 96);
  }
  const double minus_one = double(G::minus_one(base));
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    CornerOutcode(base, b, k, one, minus_one);
  }
}

void BoxBoxMoveCorners(uint8_t* base, uint32_t block, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820BCCE8 BoxBox::MoveCorners");
  const Block b(base, block);
  const double minus_one = double(G::minus_one(base));
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t corner = b.corner_x_address() + 32 * k;
    const uint32_t outcode = b.corner_outcode_address() + 32 * k;
    guest::StoreU32(base, outcode, 0);
    vl::Vec3AddTo(base, corner, offset);
    for (uint32_t axis = 0; axis < 3; axis++) {
      const uint32_t bits =
          CoordinateBits(Lfs(base, corner + 4 * axis), one, minus_one, 1u << (2 * axis),
                         2u << (2 * axis));
      if (bits) {
        guest::StoreU32(base, outcode, guest::LoadU32(base, outcode) | bits);
      }
    }
  }
}

namespace {

// B's projected radius on the face's normal (|dot(axis i, n)| x half extent
// i, summed as the original does) and whether B's center lies at least
// that far outside the face; `strict` the second test's `bgt`, else the
// first's "not blt".
bool Outside(uint8_t* base, uint32_t face, uint32_t box, bool strict) {
  const schema::BoxShapeFace f(base, face);
  const Box b(base, box);
  const uint32_t normal = f.normal_x_address();
  const double d0 = Fabs(double(vl::Vec3DotOf(base, b.face_px_normal_x_address(), normal)));
  const double d1 = Fabs(double(vl::Vec3DotOf(base, b.face_py_normal_x_address(), normal)));
  const double d2 = double(vl::Vec3DotOf(base, b.face_pz_normal_x_address(), normal));
  const double part1 = Fmuls(double(b.half_extent_y()), d1);
  const double part2 = Fmadds(double(b.half_extent_z()), Fabs(d2), part1);
  const double radius = Fmadds(double(b.half_extent_x()), d0, part2);
  const double distance =
      Fsubs(double(vl::Vec3DotOf(base, b.center_x_address(), normal)), double(f.distance()));
  return strict ? distance > radius : !(distance < radius);
}

}  // namespace

uint32_t BoxBoxFaceSeparates(uint8_t* base, uint32_t previous_face, uint32_t current_face,
                             uint32_t previous_box, uint32_t current_box) {
  ITC_NATIVE_RUNNING("sub_820BB300 BoxBox::FaceSeparates");
  if (!Outside(base, previous_face, previous_box, false)) {
    return 0;
  }
  return Outside(base, current_face, current_box, true) ? 1 : 0;
}

bool BoxBoxSeparated(uint8_t* base, uint32_t query, uint32_t cache, uint32_t frame,
                     uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD280 BoxBox::Separated");
  Copies c;
  if (!BoxCopies(base, query, &c) || !IsBox(base, c.previous_a) || !IsBox(base, c.current_a) ||
      !IsBox(base, c.previous_b) || !IsBox(base, c.current_b)) {
    return false;
  }
  // The cached face (one of A's faces in bits 0x3F, of B's in 0x3F00).
  const schema::CollisionPairCache pair_cache(base, cache);
  const uint32_t cached = pair_cache.separating_faces();
  if (cached != 0) {
    bool tested = false;
    uint32_t separates = 0;
    if ((cached & 0x3F) != 0) {
      const uint32_t face = CachedFace(cached);
      const uint32_t current_face = BoxFace(c.current_a, face);
      const uint32_t previous_face = BoxFace(c.previous_a, face);
      separates = BoxBoxFaceSeparates(base, previous_face, current_face, c.previous_b,
                                      c.current_b);
      tested = true;
    } else if ((cached & 0x3F00) != 0) {
      const uint32_t face = CachedFace(cached >> 8);
      const uint32_t current_face = BoxFace(c.current_b, face);
      const uint32_t previous_face = BoxFace(c.previous_b, face);
      separates = BoxBoxFaceSeparates(base, previous_face, current_face, c.previous_a,
                                      c.current_a);
      tested = true;
    }
    if (tested && separates != 0) {
      *result = 1;
      return true;
    }
  }
  const Query q(base, query);
  const Block block_previous_ba = q.b_in_a_previous();
  const Block block_current_ba = q.b_in_a_current();
  const Block block_previous_ab = q.a_in_b_previous();
  const Block block_current_ab = q.a_in_b_current();
  BoxRelativePlacement(base, c.previous_a, block_previous_ba.address, c.previous_b);
  block_previous_ba.set_outcode(BoxBoxOutcode(base, block_previous_ba.address));
  BoxRelativePlacement(base, c.current_a, block_current_ba.address, c.current_b);
  uint32_t outcode = BoxBoxOutcode(base, block_current_ba.address);
  block_current_ba.set_outcode(outcode);
  uint32_t shared = block_previous_ba.outcode() & outcode;
  if (shared != 0) {
    pair_cache.set_separating_faces(shared);
    *result = 1;
    return true;
  }
  BoxRelativePlacement(base, c.previous_b, block_previous_ab.address, c.previous_a);
  block_previous_ab.set_outcode(BoxBoxOutcode(base, block_previous_ab.address));
  BoxRelativePlacement(base, c.current_b, block_current_ab.address, c.current_a);
  outcode = BoxBoxOutcode(base, block_current_ab.address);
  block_current_ab.set_outcode(outcode);
  shared = block_previous_ab.outcode() & outcode;
  if (shared != 0) {
    pair_cache.set_separating_faces(shared << 8);
    *result = 1;
    return true;
  }
  pair_cache.set_separating_faces(0);
  for (const Block& block : {block_previous_ba, block_current_ba, block_previous_ab,
                             block_current_ab}) {
    BoxBoxBlockCorners(base, block.address, frame - 320);
  }
  *result = 0;
  return true;
}

bool BoxBoxSeparatedAfterMove(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD578 BoxBox::SeparatedAfterMove");
  Copies c;
  if (!BoxCopies(base, query, &c)) {
    return false;
  }
  const Query q(base, query);
  const Block block_current_ba = q.b_in_a_current();
  const Block block_previous_ba = q.b_in_a_previous();
  const Block block_previous_ab = q.a_in_b_previous();
  const Block block_current_ab = q.a_in_b_current();
  // The current blocks' centers before (+80, +112 of the frame, w 0.0).
  CopyToTemporary(base, frame + 80, block_current_ba.center_x_address());
  BoxRelativeCenter(base, c.current_a, block_current_ba.address, c.current_b);
  uint32_t outcode = BoxBoxOutcode(base, block_current_ba.address);
  block_current_ba.set_outcode(outcode);
  if ((block_previous_ba.outcode() & outcode) != 0) {
    *result = 1;
    return true;
  }
  CopyToTemporary(base, frame + 112, block_current_ab.center_x_address());
  BoxRelativeCenter(base, c.current_b, block_current_ab.address, c.current_a);
  outcode = BoxBoxOutcode(base, block_current_ab.address);
  block_current_ab.set_outcode(outcode);
  if ((block_previous_ab.outcode() & outcode) != 0) {
    *result = 1;
    return true;
  }
  // The corners moved by the centers' change.
  vl::Vec3SubCopy(base, frame + 144, block_current_ba.center_x_address(), frame + 80);
  CopyToTemporary(base, frame + 96, frame + 144);
  BoxBoxMoveCorners(base, block_current_ba.address, frame + 96);
  vl::Vec3SubCopy(base, frame + 144, block_current_ab.center_x_address(), frame + 112);
  CopyToTemporary(base, frame + 128, frame + 144);
  BoxBoxMoveCorners(base, block_current_ab.address, frame + 128);
  *result = 0;
  return true;
}

bool QuerySeparated(uint8_t* base, uint32_t query, uint32_t cache, uint32_t frame,
                    uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD508 Query::Separated");
  return BoxBoxSeparated(base, query, cache, frame - 160, result);
}

bool QuerySeparatedAfterMove(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD6E0 Query::SeparatedAfterMove");
  return BoxBoxSeparatedAfterMove(base, query, frame - 240, result);
}

double CollisionPairNextCheckTime(uint8_t* base, uint32_t pair, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BBB20 CollisionPair::NextCheckTime");
  // The pair's objects A (+16) and B (+20).
  const auto object_a = [&] { return Object(base, guest::LoadU32(base, pair + 16)); };
  const auto object_b = [&] { return Object(base, guest::LoadU32(base, pair + 20)); };
  const double step = double(G::step(base));
  const double clock = double(G::clock(base));
  const Object a = object_a();
  const Object b = object_b();
  if (a.no_queries() == 0 && b.no_queries() == 0 && a.unknown_276() == 0 &&
      b.unknown_276() == 0) {
    const double speed_a = double(va::RigidBodyFloat48(base, a.body()));
    const double speed_b = double(va::RigidBodyFloat48(base, object_b().body()));
    const double speeds = Fadds(speed_b, speed_a);
    if (speeds != Lfs(base, kZero)) {
      const uint32_t center_b =
          CollisionShapeCurrentCenter(base, object_b().shape_record_address());
      const uint32_t center_a =
          CollisionShapeCurrentCenter(base, object_a().shape_record_address());
      vl::Vec3Difference(base, frame + 80, center_a, center_b);
      const double gap = double(vl::Vec3Length(base, frame + 80));
      const double radius_a = ShapeRadius(base, object_a().shape().address);
      const uint32_t shape_b = object_b().shape().address;
      const double radius_b = ShapeRadius(base, shape_b);
      *r3 = shape_b;
      return Fadds(Fdivs(Fsubs(gap, Fadds(radius_b, radius_a)), speeds), clock);
    }
    // r3 as RigidBodyFloat48's adapter leaves it: B's body.
    *r3 = object_b().body();
    return Fadds(clock, step);
  }
  *r3 = pair;
  return Fadds(clock, step);
}

}  // namespace collision
