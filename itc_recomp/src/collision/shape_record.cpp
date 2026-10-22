// The shape record's bounds, motion, placement and translation
// (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

// fneg between an lfs and an stfs: the word's sign bit flipped.
void NegateWord(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreU32(base, to, guest::LoadU32(base, from) ^ 0x80000000u);
}

// The motion's translation row (+96), as UpdateMotion and Translate work it
// out: first the change of the center (w 1.0), then the rotation +48 applied
// to minus the current center, plus the current center (w 1.0); through
// `frame` + 80 to + 159.
void MotionRow(uint8_t* base, uint32_t record, uint32_t frame) {
  const uint32_t previous = guest::LoadU32(base, record + 4);
  const uint32_t current = guest::LoadU32(base, record + 8);
  vl::Vec3Difference(base, frame + 144, current + 48, previous + 48);
  CopyWord(base, frame + 144 + 0, frame + 80);
  CopyWord(base, frame + 144 + 8, frame + 88);
  CopyWord(base, frame + 144 + 4, frame + 84);
  CopyWord(base, kOne, frame + 92);
  vl::Vec4Copy(base, record + 96, frame + 80);
  const uint32_t center = guest::LoadU32(base, record + 8) + 48;
  NegateWord(base, center + 0, frame + 96);
  NegateWord(base, center + 8, frame + 104);
  NegateWord(base, center + 4, frame + 100);
  CopyWord(base, kZero, frame + 108);
  vl::MatrixTransformPoint(base, frame + 128, record + 48, frame + 96);
  vl::Vec3Sum(base, frame + 144, frame + 128, guest::LoadU32(base, record + 8) + 48);
  CopyWord(base, frame + 144 + 0, frame + 112);
  CopyWord(base, kOne, frame + 124);
  CopyWord(base, frame + 144 + 8, frame + 120);
  CopyWord(base, frame + 144 + 4, frame + 116);
  vl::Vec4Copy(base, record + 96, frame + 112);
}

}  // namespace

void CollisionShapeUpdateBounds(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820BAC00 CollisionShape::UpdateBounds");
  const uint32_t previous = guest::LoadU32(base, record + 4) + 48;
  const uint32_t current = guest::LoadU32(base, record + 8) + 48;
  const double radius = Lfs(base, guest::LoadU32(base, record + 0) + 32);
  for (uint32_t axis = 0; axis < 12; axis += 4) {
    const double a = Lfs(base, previous + axis);
    const double b = Lfs(base, current + axis);
    double high;
    if (a < b) {
      Stfs(base, record + 16 + axis, Fsubs(a, radius));
      high = Lfs(base, current + axis);
    } else {
      Stfs(base, record + 16 + axis, Fsubs(b, radius));
      high = Lfs(base, previous + axis);
    }
    Stfs(base, record + 32 + axis, Fadds(high, radius));
  }
}

void CollisionShapeUpdateMotion(uint8_t* base, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BAE78 CollisionShape::UpdateMotion");
  const uint32_t previous = guest::LoadU32(base, record + 4) + 64;
  const uint32_t current = guest::LoadU32(base, record + 8) + 64;
  for (uint32_t row = 0; row < 3; row++) {
    for (uint32_t column = 0; column < 3; column++) {
      Stfs(base, record + 48 + 16 * row + 4 * column,
           double(vl::Vec3DotOf(base, current + 16 * row, previous + 16 * column)));
    }
  }
  MotionRow(base, record, frame);
}

bool CollisionShapePlace(uint8_t* base, uint32_t record, uint32_t matrix, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB758 CollisionShape::Place");
  const uint32_t previous = guest::LoadU32(base, record + 4);
  const uint32_t current = guest::LoadU32(base, record + 8);
  const uint32_t source = guest::LoadU32(base, record + 0);
  // The copy to place (the previous one) answers 2 to its type when it's a
  // box; with +112 not 0.0 (fcmpu: NaN isn't equal) the template is copied
  // onto the stack (+80) and lowered.
  const uint32_t copy = frame + 80;
  if (!IsBox(base, previous) || !IsBox(base, source)) {
    return false;
  }
  const bool lowered = Lfs(base, record + 112) != Lfs(base, kZero);
  if (lowered && !BoxCopyFits(copy, source)) {
    return false;
  }
  guest::StoreU32(base, record + 4, current);
  guest::StoreU32(base, record + 8, previous);
  if (lowered) {
    BoxCopyConstruct(base, copy, guest::LoadU32(base, record + 0));
    Stfs(base, frame + 372, Fsubs(Lfs(base, frame + 372), Lfs(base, record + 112)));
    BoxSetCorners(base, copy, frame + 368, frame + 256, frame - 224);
    BoxPlace(base, guest::LoadU32(base, record + 8), matrix, copy, frame - 288);
    guest::StoreU32(base, copy, kShapeVtable);
  } else {
    BoxPlace(base, guest::LoadU32(base, record + 8), matrix, guest::LoadU32(base, record + 0),
             frame - 288);
  }
  BoxCopyCache(base, guest::LoadU32(base, record + 8), guest::LoadU32(base, record + 4));
  CollisionShapeUpdateMotion(base, record, frame - 240);
  CollisionShapeUpdateBounds(base, record);
  return true;
}

bool CollisionShapeTranslate(uint8_t* base, uint32_t record, uint32_t offset, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BACD0 CollisionShape::Translate");
  if (!IsBox(base, guest::LoadU32(base, record + 8))) {
    return false;
  }
  BoxTranslate(base, guest::LoadU32(base, record + 8), offset);
  MotionRow(base, record, frame);
  CollisionShapeUpdateBounds(base, record);
  return true;
}

namespace {

// Whether the record's placed copies (+4, +8) can be deleted natively
// through their slot 0: each 0, or a box (slot 0 Shape::DeletingDestructor)
// the layer gave, the two not the same block.
bool CopiesDeletable(uint8_t* base, uint32_t record) {
  const uint32_t previous = guest::LoadU32(base, record + 4);
  const uint32_t current = guest::LoadU32(base, record + 8);
  for (uint32_t copy : {previous, current}) {
    if (copy != 0 && (!IsBox(base, copy) || !Deletable(copy))) {
      return false;
    }
  }
  return previous == 0 || previous != current;
}

// The copies deleted through slot 0 (r4 = 1) and zeroed, as the destructor
// and SetUp do.
void DeleteCopies(uint8_t* base, uint32_t record) {
  for (uint32_t at : {4u, 8u}) {
    const uint32_t copy = guest::LoadU32(base, record + at);
    if (copy != 0) {
      ShapeDeletingDestructor(base, copy, 1);
      guest::StoreU32(base, record + at, 0);
    }
  }
}

}  // namespace

bool CollisionShapeDestroy(uint8_t* base, uint32_t record, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BAAA0 CollisionShape::~CollisionShape");
  if (!CopiesDeletable(base, record)) {
    return false;
  }
  // r3: the last copy loaded (+8), or what its slot 0 returned (the same).
  *r3 = guest::LoadU32(base, record + 8);
  DeleteCopies(base, record);
  return true;
}

bool CollisionShapeSetUp(uint8_t* base, uint32_t record, uint32_t shape, uint32_t matrix,
                         uint32_t memory, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB580 CollisionShape::SetUp");
  // The template's slots 5 (BoxShape::Clone) and 9 (BoxShape::Place) are
  // made natively only for a box; the copies' deletion and the clones'
  // allocation are checked before anything is written (malloc gives 0 with
  // no process heap, and the clones would take operator new's error path).
  if (!IsBox(base, shape) || !CopiesDeletable(base, record) ||
      (memory == 0 && memory::crt::GetProcessHeap(base) == 0)) {
    return false;
  }
  const uint32_t f = frame;
  DeleteCopies(base, record);
  uint32_t copy = 0;
  BoxClone(base, shape, memory, f - 96, &copy);
  guest::StoreU32(base, record + 4, copy);
  BoxClone(base, shape, memory, f - 96, &copy);
  guest::StoreU32(base, record + 8, copy);
  // Each placed by the matrix through its slot 9: a clone is a box (or 0,
  // which the original would call through too: not with a process heap).
  BoxPlace(base, guest::LoadU32(base, record + 4), matrix, shape, f - 288);
  BoxPlace(base, guest::LoadU32(base, record + 8), matrix, shape, f - 288);
  guest::StoreU32(base, record + 0, shape);
  matrix_math::SetIdentity(base, record + 48);
  CopyWord(base, kZero, record + 112);
  CollisionShapeUpdateBounds(base, record);
  return true;
}

}  // namespace collision
