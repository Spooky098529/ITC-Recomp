// The box's and the base class's constructors and the box's placement
// (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

constexpr uint32_t kHalf = 0x82001BE4;      // the game's float 0.5
constexpr uint32_t kMinusOne = 0x82001BE0;  // -1.0
constexpr uint32_t kTwo = 0x820021E8;       // 2.0
constexpr uint32_t kShapeFloat24 = 0x82002968;
constexpr uint32_t kShapeFloat28 = 0x82002204;

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
  guest::StoreU32(base, shape + 0, kShapeVtable);
  CopyWord(base, kZero, shape + 60);
  for (uint32_t at : {76u, 92u, 108u}) {
    CopyWord(base, kZero, shape + at);
  }
  vl::Vec4Copy(base, shape + 48, kZeroVector);
  CopyWord(base, kZero, shape + 32);
  for (uint32_t at : {84u, 88u, 92u}) {
    CopyWord(base, kZero, frame + at);
  }
  CopyWord(base, kHalf, shape + 16);
  CopyWord(base, kHalf, shape + 20);
  CopyWord(base, kShapeFloat24, shape + 24);
  CopyWord(base, kShapeFloat28, shape + 28);
  CopyWord(base, kOne, frame + 80);
  vl::Vec4Copy(base, shape + 64, frame + 80);
  CopyWord(base, kZero, frame + 96);
  CopyWord(base, kOne, frame + 100);
  CopyWord(base, kZero, frame + 104);
  CopyWord(base, kZero, frame + 108);
  vl::Vec4Copy(base, shape + 80, frame + 96);
  CopyWord(base, kZero, frame + 112);
  CopyWord(base, kZero, frame + 116);
  CopyWord(base, kOne, frame + 120);
  CopyWord(base, kZero, frame + 124);
  vl::Vec4Copy(base, shape + 96, frame + 112);
  guest::StoreU16(base, shape + 112, 0);
  guest::StoreU16(base, shape + 114, 0);
}

bool ShapeCopyConstruct(uint8_t* base, uint32_t shape, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820BAB48 Shape::Shape(const&)");
  if (!CopyFits(shape + 64, from + 64, 48)) {
    return false;
  }
  guest::StoreU32(base, shape + 0, kShapeVtable);
  for (uint32_t at = 16; at <= 32; at += 4) {
    CopyWord(base, from + at, shape + at);
  }
  for (uint32_t at = 48; at < 64; at += 4) {
    guest::StoreU32(base, shape + at, guest::LoadU32(base, from + at));
  }
  c_runtime::Memcpy(base, shape + 64, from + 64, 48);
  guest::StoreU16(base, shape + 112, guest::LoadU16(base, from + 112));
  guest::StoreU16(base, shape + 114, guest::LoadU16(base, from + 114));
  return true;
}

bool BoxCopyFits(uint32_t box, uint32_t from) {
  return CopyFits(box + 64, from + 64, 48) && CopyFits(box + 176, from + 176, 128) &&
         CopyFits(box + 304, from + 304, 192);
}

bool BoxCopyConstruct(uint8_t* base, uint32_t box, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820BB688 BoxShape::BoxShape(const&)");
  if (!BoxCopyFits(box, from)) {
    return false;
  }
  ShapeCopyConstruct(base, box, from);
  guest::StoreU32(base, box + 0, kBoxVtable);
  for (uint32_t at = 128; at < 160; at += 4) {
    guest::StoreU32(base, box + at, guest::LoadU32(base, from + at));
  }
  for (uint32_t at = 160; at < 176; at += 4) {
    guest::StoreU32(base, box + at, guest::LoadU32(base, from + at));
  }
  c_runtime::Memcpy(base, box + 176, from + 176, 128);
  c_runtime::Memcpy(base, box + 304, from + 304, 192);
  return true;
}

double BoxSetCorners(uint8_t* base, uint32_t box, uint32_t min_corner, uint32_t max_corner,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B99D0 BoxShape::SetCorners");
  // The center and the radius.
  vl::Vec3Sum(base, frame + 128, min_corner, max_corner);
  const double half = Lfs(base, kHalf);
  vl::Vec4Scaled(base, frame + 144, frame + 128, half);
  vl::Vec4Copy(base, box + 48, frame + 144);
  vl::Vec3Difference(base, frame + 144, max_corner, min_corner);
  const double length = double(vl::Vec3Length(base, frame + 144));
  Stfs(base, box + 32, Fmuls(length, half));
  // The half extents and their inverses.
  vl::Vec3Difference(base, frame + 144, max_corner, min_corner);
  vl::Vec4Scaled(base, frame + 128, frame + 144, half);
  vl::Vec4Copy(base, box + 128, frame + 128);
  const double x = Lfs(base, box + 128);
  const double y = Lfs(base, box + 132);
  const double z = Lfs(base, box + 136);
  const double one = Lfs(base, kOne);
  Stfs(base, box + 144, Fdivs(one, x));
  Stfs(base, box + 148, Fdivs(one, y));
  Stfs(base, box + 152, Fdivs(one, z));
  // The corners: a coordinate from the min corner where its bit of i is
  // set, else from the max (their bits: the translated code keeps a
  // signaling NaN here, though branches join before the store; the
  // harness's "one NaN" group).
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t corner = box + 176 + 16 * i;
    CopyWord(base, ((i & 1) ? min_corner : max_corner) + 0, corner + 0);
    CopyWord(base, ((i & 2) ? min_corner : max_corner) + 4, corner + 4);
    CopyWord(base, ((i & 4) ? min_corner : max_corner) + 8, corner + 8);
  }
  // The faces: +x, +y, +z through the max corner, -x, -y, -z the min.
  const double zero = Lfs(base, kZero);
  Stfs(base, box + 324, zero);
  Stfs(base, box + 328, zero);
  Stfs(base, box + 320, one);
  double d = double(vl::Vec3DotOf(base, box + 320, max_corner));
  Stfs(base, box + 356, one);
  Stfs(base, box + 360, zero);
  Stfs(base, box + 304, d);
  Stfs(base, box + 352, zero);
  d = double(vl::Vec3DotOf(base, box + 352, max_corner));
  Stfs(base, box + 388, zero);
  Stfs(base, box + 392, one);
  Stfs(base, box + 336, d);
  Stfs(base, box + 384, zero);
  d = double(vl::Vec3DotOf(base, box + 384, max_corner));
  Stfs(base, box + 420, zero);
  Stfs(base, box + 424, zero);
  Stfs(base, box + 368, d);
  const double minus_one = Lfs(base, kMinusOne);
  Stfs(base, box + 416, minus_one);
  d = double(vl::Vec3DotOf(base, box + 416, min_corner));
  Stfs(base, box + 452, minus_one);
  Stfs(base, box + 456, zero);
  Stfs(base, box + 400, d);
  Stfs(base, box + 448, zero);
  d = double(vl::Vec3DotOf(base, box + 448, min_corner));
  Stfs(base, box + 484, zero);
  Stfs(base, box + 488, minus_one);
  Stfs(base, box + 432, d);
  Stfs(base, box + 480, zero);
  d = double(vl::Vec3DotOf(base, box + 480, min_corner));
  Stfs(base, box + 464, d);
  for (uint32_t at = 160; at < 176; at += 4) {
    guest::StoreU32(base, box + at, 0);
  }
  // The axes: the identity rows.
  Stfs(base, frame + 80, one);
  Stfs(base, frame + 84, zero);
  Stfs(base, frame + 88, zero);
  Stfs(base, frame + 92, zero);
  vl::Vec4Copy(base, box + 64, frame + 80);
  Stfs(base, frame + 96, zero);
  Stfs(base, frame + 100, one);
  Stfs(base, frame + 104, zero);
  Stfs(base, frame + 108, zero);
  vl::Vec4Copy(base, box + 80, frame + 96);
  Stfs(base, frame + 112, zero);
  Stfs(base, frame + 116, zero);
  Stfs(base, frame + 120, one);
  Stfs(base, frame + 124, zero);
  vl::Vec4Copy(base, box + 96, frame + 112);
  return d;
}

void BoxConstruct(uint8_t* base, uint32_t box, uint32_t min_corner, uint32_t max_corner,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB188 BoxShape::BoxShape");
  ShapeConstruct(base, box, frame - 176);
  guest::StoreU32(base, box + 0, kBoxVtable);
  CopyWord(base, kZero, box + 140);
  CopyWord(base, kZero, box + 156);
  for (uint32_t k = 0; k < 8; k++) {
    CopyWord(base, kZero, box + 188 + 16 * k);
  }
  for (uint32_t k = 0; k < 6; k++) {
    CopyWord(base, kZero, box + 332 + 32 * k);
  }
  BoxSetCorners(base, box, min_corner, max_corner, frame - 224);
}

bool BoxPlace(uint8_t* base, uint32_t box, uint32_t matrix, uint32_t source, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B9C40 BoxShape::Place");
  // The template's type asked for first (its slot 4; a box's is a leaf).
  if (!IsBox(base, source)) {
    return false;
  }
  vl::Vec4Copy(base, box + 128, source + 128);
  vl::Vec4Copy(base, box + 144, source + 144);
  vl::MatrixTransformPoint(base, frame + 176, matrix, source + 48);
  vl::Vec4Copy(base, box + 48, frame + 176);
  for (uint32_t k = 0; k < 8; k++) {
    vl::MatrixTransformPoint(base, frame + 176, matrix, source + 176 + 16 * k);
    vl::Vec4Copy(base, box + 176 + 16 * k, frame + 176);
  }
  // The positive faces: the matrix's rows, through corner 0.
  const uint32_t corner0 = box + 176;
  RowInto(base, frame + 80, matrix + 0, kZero);
  vl::Vec4Copy(base, box + 320, frame + 80);
  Stfs(base, box + 304, double(vl::Vec3DotOf(base, box + 320, corner0)));
  RowInto(base, frame + 96, matrix + 16, kZero);
  vl::Vec4Copy(base, box + 352, frame + 96);
  Stfs(base, box + 336, double(vl::Vec3DotOf(base, box + 352, corner0)));
  RowInto(base, frame + 112, matrix + 32, kZero);
  vl::Vec4Copy(base, box + 384, frame + 112);
  Stfs(base, box + 368, double(vl::Vec3DotOf(base, box + 384, corner0)));
  // The negative faces: the normals negated, the distance 2 x the half
  // extent - the positive face's.
  const double minus_one = Lfs(base, kMinusOne);
  vl::Vec4Scaled(base, frame + 176, box + 320, minus_one);
  vl::Vec4Copy(base, box + 416, frame + 176);
  const double two = Lfs(base, kTwo);
  Stfs(base, box + 400, Fmsubs(Lfs(base, box + 128), two, Lfs(base, box + 304)));
  vl::Vec4Scaled(base, frame + 176, box + 352, minus_one);
  vl::Vec4Copy(base, box + 448, frame + 176);
  Stfs(base, box + 432, Fmsubs(Lfs(base, box + 132), two, Lfs(base, box + 336)));
  vl::Vec4Scaled(base, frame + 176, box + 384, minus_one);
  vl::Vec4Copy(base, box + 480, frame + 176);
  Stfs(base, box + 464, Fmsubs(Lfs(base, box + 136), two, Lfs(base, box + 368)));
  // The axes: the rows.
  RowInto(base, frame + 128, matrix + 0, kZero);
  vl::Vec4Copy(base, box + 64, frame + 128);
  RowInto(base, frame + 144, matrix + 16, kZero);
  vl::Vec4Copy(base, box + 80, frame + 144);
  RowInto(base, frame + 160, matrix + 32, kZero);
  vl::Vec4Copy(base, box + 96, frame + 160);
  return true;
}

bool Deletable(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

bool ShapeDeletingDestructor(uint8_t* base, uint32_t shape, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BB140 Shape::DeletingDestructor");
  // operator delete's error path (a block the layer didn't give) is the
  // original's: checked before anything is written.
  if ((flags & 1) != 0 && !Deletable(shape)) {
    return false;
  }
  guest::StoreU32(base, shape + 0, kShapeVtable);
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
    BoxConstruct(base, out, box + 288, box + 176, frame - 112);
  }
  *copy = out;
  return true;
}

}  // namespace collision
