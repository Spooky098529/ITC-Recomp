// The box's and the base class's constructors and the box's placement
// (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/collision.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

using G = schema::CollisionGlobals;
using Box = schema::BoxShape;
using Matrix = schema::CollisionPlacementMatrix;

// Whether memcpy (sub_825A7410) copies these natively with nothing for its
// override to record: outside the physical views, each range in one view,
// the two not sharing memory (as CollisionWorld::RemovePair checks).
bool CopyFits(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && source < 0xA0000000u &&
         c_runtime::InOneView(destination, size) && c_runtime::InOneView(source, size) &&
         !c_runtime::MayShare(destination, size, source, size);
}

// Three floats of `from` (lfs then stfs, next to each other: their bits)
// and `w` into a temporary vector at `at`, in the original's order (x, y,
// w, z).
void RowInto(uint8_t* base, uint32_t at, uint32_t from, uint32_t w) {
  CopyWord(base, from + 0, at + 0);
  CopyWord(base, from + 4, at + 4);
  CopyWord(base, w, at + 12);
  CopyWord(base, from + 8, at + 8);
}

}  // namespace

void ShapeConstruct(uint8_t* base, uint32_t shape, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB020 Shape::Shape");
  const Box s(base, shape);
  s.set_vtable(kShapeVtable);
  CopyWord(base, kZero, s.center_w_address());
  for (uint32_t at : {s.axis0_w_address(), s.axis1_w_address(), s.axis2_w_address()}) {
    CopyWord(base, kZero, at);
  }
  vl::Vec4Copy(base, s.center_x_address(), kZeroVector);
  CopyWord(base, kZero, s.radius_address());
  for (uint32_t at : {84u, 88u, 92u}) {
    CopyWord(base, kZero, frame + at);
  }
  CopyWord(base, G::half_address(), s.car_float_16_address());
  CopyWord(base, G::half_address(), s.car_float_20_address());
  CopyWord(base, G::shape_float_24_address(), s.car_float_24_address());
  CopyWord(base, G::shape_float_28_address(), s.friction_address());
  CopyWord(base, kOne, frame + 80);
  vl::Vec4Copy(base, s.axis0_x_address(), frame + 80);
  CopyWord(base, kZero, frame + 96);
  CopyWord(base, kOne, frame + 100);
  CopyWord(base, kZero, frame + 104);
  CopyWord(base, kZero, frame + 108);
  vl::Vec4Copy(base, s.axis1_x_address(), frame + 96);
  CopyWord(base, kZero, frame + 112);
  CopyWord(base, kZero, frame + 116);
  CopyWord(base, kOne, frame + 120);
  CopyWord(base, kZero, frame + 124);
  vl::Vec4Copy(base, s.axis2_x_address(), frame + 112);
  s.set_unknown_112(0);
  s.set_unknown_114(0);
}

bool ShapeCopyConstruct(uint8_t* base, uint32_t shape, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820BAB48 Shape::Shape(const&)");
  const Box s(base, shape), source(base, from);
  if (!CopyFits(s.axis0_x_address(), source.axis0_x_address(), 48)) {
    return false;
  }
  s.set_vtable(kShapeVtable);
  CopyWord(base, source.car_float_16_address(), s.car_float_16_address());
  CopyWord(base, source.car_float_20_address(), s.car_float_20_address());
  CopyWord(base, source.car_float_24_address(), s.car_float_24_address());
  CopyWord(base, source.friction_address(), s.friction_address());
  CopyWord(base, source.radius_address(), s.radius_address());
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, s.center_x_address() + 4 * k,
                    guest::LoadU32(base, source.center_x_address() + 4 * k));
  }
  c_runtime::Memcpy(base, s.axis0_x_address(), source.axis0_x_address(), 48);
  s.set_unknown_112(source.unknown_112());
  s.set_unknown_114(source.unknown_114());
  return true;
}

bool BoxCopyFits(uint32_t box, uint32_t from) {
  const Box b(nullptr, box), source(nullptr, from);
  return CopyFits(b.axis0_x_address(), source.axis0_x_address(), 48) &&
         CopyFits(b.corner_x_address(), source.corner_x_address(), 128) &&
         CopyFits(b.face_px_distance_address(), source.face_px_distance_address(), 192);
}

bool BoxCopyConstruct(uint8_t* base, uint32_t box, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820BB688 BoxShape::BoxShape(const&)");
  if (!BoxCopyFits(box, from)) {
    return false;
  }
  const Box b(base, box), source(base, from);
  ShapeCopyConstruct(base, box, from);
  b.set_vtable(kBoxVtable);
  for (uint32_t k = 0; k < 8; k++) {
    guest::StoreU32(base, b.half_extent_x_address() + 4 * k,
                    guest::LoadU32(base, source.half_extent_x_address() + 4 * k));
  }
  b.set_lower_corner_position_0(source.lower_corner_position_0());
  b.set_lower_corner_position_1(source.lower_corner_position_1());
  b.set_lower_corner_position_2(source.lower_corner_position_2());
  b.set_lower_corner_position_3(source.lower_corner_position_3());
  c_runtime::Memcpy(base, b.corner_x_address(), source.corner_x_address(), 128);
  c_runtime::Memcpy(base, b.face_px_distance_address(), source.face_px_distance_address(), 192);
  return true;
}

double BoxSetCorners(uint8_t* base, uint32_t box, uint32_t min_corner, uint32_t max_corner,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B99D0 BoxShape::SetCorners");
  const Box b(base, box);
  const schema::CollisionVector lo(base, min_corner), hi(base, max_corner);
  // The center and the radius.
  vl::Vec3Sum(base, frame + 128, min_corner, max_corner);
  const double half = double(G::half(base));
  vl::Vec4Scaled(base, frame + 144, frame + 128, half);
  vl::Vec4Copy(base, b.center_x_address(), frame + 144);
  vl::Vec3Difference(base, frame + 144, max_corner, min_corner);
  const double length = double(vl::Vec3Length(base, frame + 144));
  b.set_radius(float(Fmuls(length, half)));
  // The half extents and their inverses.
  vl::Vec3Difference(base, frame + 144, max_corner, min_corner);
  vl::Vec4Scaled(base, frame + 128, frame + 144, half);
  vl::Vec4Copy(base, b.half_extent_x_address(), frame + 128);
  const double x = double(b.half_extent_x());
  const double y = double(b.half_extent_y());
  const double z = double(b.half_extent_z());
  const double one = Lfs(base, kOne);
  b.set_inverse_half_extent_x(float(Fdivs(one, x)));
  b.set_inverse_half_extent_y(float(Fdivs(one, y)));
  b.set_inverse_half_extent_z(float(Fdivs(one, z)));
  // The corners: a coordinate from the min corner where its bit of i is
  // set, else from the max (their bits: the translated code keeps a
  // signaling NaN here, though branches join before the store; the
  // harness's "one NaN" group).
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t corner = b.corner_x_address() + 16 * i;
    CopyWord(base, ((i & 1) ? lo : hi).x_address(), corner + 0);
    CopyWord(base, ((i & 2) ? lo : hi).y_address(), corner + 4);
    CopyWord(base, ((i & 4) ? lo : hi).z_address(), corner + 8);
  }
  // The faces: +x, +y, +z through the max corner, -x, -y, -z the min.
  const double zero = Lfs(base, kZero);
  b.set_face_px_normal_y(float(zero));
  b.set_face_px_normal_z(float(zero));
  b.set_face_px_normal_x(float(one));
  double d = double(vl::Vec3DotOf(base, b.face_px_normal_x_address(), max_corner));
  b.set_face_py_normal_y(float(one));
  b.set_face_py_normal_z(float(zero));
  b.set_face_px_distance(float(d));
  b.set_face_py_normal_x(float(zero));
  d = double(vl::Vec3DotOf(base, b.face_py_normal_x_address(), max_corner));
  b.set_face_pz_normal_y(float(zero));
  b.set_face_pz_normal_z(float(one));
  b.set_face_py_distance(float(d));
  b.set_face_pz_normal_x(float(zero));
  d = double(vl::Vec3DotOf(base, b.face_pz_normal_x_address(), max_corner));
  b.set_face_nx_normal_y(float(zero));
  b.set_face_nx_normal_z(float(zero));
  b.set_face_pz_distance(float(d));
  const double minus_one = double(G::minus_one(base));
  b.set_face_nx_normal_x(float(minus_one));
  d = double(vl::Vec3DotOf(base, b.face_nx_normal_x_address(), min_corner));
  b.set_face_ny_normal_y(float(minus_one));
  b.set_face_ny_normal_z(float(zero));
  b.set_face_nx_distance(float(d));
  b.set_face_ny_normal_x(float(zero));
  d = double(vl::Vec3DotOf(base, b.face_ny_normal_x_address(), min_corner));
  b.set_face_nz_normal_y(float(zero));
  b.set_face_nz_normal_z(float(minus_one));
  b.set_face_ny_distance(float(d));
  b.set_face_nz_normal_x(float(zero));
  d = double(vl::Vec3DotOf(base, b.face_nz_normal_x_address(), min_corner));
  b.set_face_nz_distance(float(d));
  b.set_lower_corner_position_0(0);
  b.set_lower_corner_position_1(0);
  b.set_lower_corner_position_2(0);
  b.set_lower_corner_position_3(0);
  // The axes: the identity rows.
  Stfs(base, frame + 80, one);
  Stfs(base, frame + 84, zero);
  Stfs(base, frame + 88, zero);
  Stfs(base, frame + 92, zero);
  vl::Vec4Copy(base, b.axis0_x_address(), frame + 80);
  Stfs(base, frame + 96, zero);
  Stfs(base, frame + 100, one);
  Stfs(base, frame + 104, zero);
  Stfs(base, frame + 108, zero);
  vl::Vec4Copy(base, b.axis1_x_address(), frame + 96);
  Stfs(base, frame + 112, zero);
  Stfs(base, frame + 116, zero);
  Stfs(base, frame + 120, one);
  Stfs(base, frame + 124, zero);
  vl::Vec4Copy(base, b.axis2_x_address(), frame + 112);
  return d;
}

void BoxConstruct(uint8_t* base, uint32_t box, uint32_t min_corner, uint32_t max_corner,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB188 BoxShape::BoxShape");
  const Box b(base, box);
  ShapeConstruct(base, box, frame - 176);
  b.set_vtable(kBoxVtable);
  CopyWord(base, kZero, b.half_extent_w_address());
  CopyWord(base, kZero, b.inverse_half_extent_w_address());
  for (uint32_t k = 0; k < 8; k++) {
    CopyWord(base, kZero, b.corner_w_address() + 16 * k);
  }
  for (uint32_t k = 0; k < 6; k++) {
    CopyWord(base, kZero, b.face_px_normal_w_address() + 32 * k);
  }
  BoxSetCorners(base, box, min_corner, max_corner, frame - 224);
}

bool BoxPlace(uint8_t* base, uint32_t box, uint32_t matrix, uint32_t source, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B9C40 BoxShape::Place");
  // The template's type asked for first (its slot 4; a box's is a leaf).
  if (!IsBox(base, source)) {
    return false;
  }
  const Box b(base, box), from(base, source);
  const Matrix m(base, matrix);
  vl::Vec4Copy(base, b.half_extent_x_address(), from.half_extent_x_address());
  vl::Vec4Copy(base, b.inverse_half_extent_x_address(), from.inverse_half_extent_x_address());
  vl::MatrixTransformPoint(base, frame + 176, matrix, from.center_x_address());
  vl::Vec4Copy(base, b.center_x_address(), frame + 176);
  for (uint32_t k = 0; k < 8; k++) {
    vl::MatrixTransformPoint(base, frame + 176, matrix, from.corner_x_address() + 16 * k);
    vl::Vec4Copy(base, b.corner_x_address() + 16 * k, frame + 176);
  }
  // The positive faces: the matrix's rows, through corner 0.
  const uint32_t corner0 = b.corner_x_address();
  RowInto(base, frame + 80, m.row0_x_address(), kZero);
  vl::Vec4Copy(base, b.face_px_normal_x_address(), frame + 80);
  b.set_face_px_distance(vl::Vec3DotOf(base, b.face_px_normal_x_address(), corner0));
  RowInto(base, frame + 96, m.row1_x_address(), kZero);
  vl::Vec4Copy(base, b.face_py_normal_x_address(), frame + 96);
  b.set_face_py_distance(vl::Vec3DotOf(base, b.face_py_normal_x_address(), corner0));
  RowInto(base, frame + 112, m.row2_x_address(), kZero);
  vl::Vec4Copy(base, b.face_pz_normal_x_address(), frame + 112);
  b.set_face_pz_distance(vl::Vec3DotOf(base, b.face_pz_normal_x_address(), corner0));
  // The negative faces: the normals negated, the distance 2 x the half
  // extent - the positive face's.
  const double minus_one = double(G::minus_one(base));
  vl::Vec4Scaled(base, frame + 176, b.face_px_normal_x_address(), minus_one);
  vl::Vec4Copy(base, b.face_nx_normal_x_address(), frame + 176);
  const double two = double(G::two(base));
  b.set_face_nx_distance(
      float(Fmsubs(double(b.half_extent_x()), two, double(b.face_px_distance()))));
  vl::Vec4Scaled(base, frame + 176, b.face_py_normal_x_address(), minus_one);
  vl::Vec4Copy(base, b.face_ny_normal_x_address(), frame + 176);
  b.set_face_ny_distance(
      float(Fmsubs(double(b.half_extent_y()), two, double(b.face_py_distance()))));
  vl::Vec4Scaled(base, frame + 176, b.face_pz_normal_x_address(), minus_one);
  vl::Vec4Copy(base, b.face_nz_normal_x_address(), frame + 176);
  b.set_face_nz_distance(
      float(Fmsubs(double(b.half_extent_z()), two, double(b.face_pz_distance()))));
  // The axes: the rows.
  RowInto(base, frame + 128, m.row0_x_address(), kZero);
  vl::Vec4Copy(base, b.axis0_x_address(), frame + 128);
  RowInto(base, frame + 144, m.row1_x_address(), kZero);
  vl::Vec4Copy(base, b.axis1_x_address(), frame + 144);
  RowInto(base, frame + 160, m.row2_x_address(), kZero);
  vl::Vec4Copy(base, b.axis2_x_address(), frame + 160);
  return true;
}

bool Deletable(uint32_t block) {
  // A host object's handle (E5 step 4): operator delete frees it.
  memory::HandleInfo info;
  return block == 0 || memory::IsHeapBlock(block) ||
         (memory::DescribeHandle(block, &info) && info.offset == 0);
}

bool ShapeDeletingDestructor(uint8_t* base, uint32_t shape, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BB140 Shape::DeletingDestructor");
  // operator delete's error path (a block the layer didn't give) is the
  // original's: checked before anything is written.
  if ((flags & 1) != 0 && !Deletable(shape)) {
    return false;
  }
  Box(base, shape).set_vtable(kShapeVtable);
  if ((flags & 1) != 0) {
    memory::crt::OperatorDelete(shape);
  }
  return true;
}

bool BoxClone(uint8_t* base, uint32_t box, uint32_t memory, uint32_t frame, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_820BB218 BoxShape::Clone");
  uint32_t out = memory;
  if (out == 0) {
    // operator new's paths that aren't malloc's (the new handler,
    // std::bad_alloc) are the original's.
    const auto block = memory::crt::OperatorNew(base, 496);
    if (!block) {
      return false;
    }
    out = *block;
  }
  if (out != 0) {
    // The min corner (corner 7) and the max (corner 0).
    const Box b(base, box);
    BoxConstruct(base, out, b.corner_x_address() + 7 * 16, b.corner_x_address(), frame - 112);
  }
  *copy = out;
  return true;
}

}  // namespace collision
