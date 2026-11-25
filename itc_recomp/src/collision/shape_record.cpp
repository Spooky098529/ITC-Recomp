// The shape record's bounds, motion, placement and translation
// (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "schema/collision.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

using Record = schema::CollisionShape;

namespace {

// fneg between an lfs and an stfs: the word's sign bit flipped.
void NegateWord(uint32_t from, uint32_t to) {
  guest::StoreU32(to, guest::LoadU32(from) ^ 0x80000000u);
}

// The motion's translation row (+96), as UpdateMotion and Translate work it
// out: first the change of the center (w 1.0), then the rotation +48 applied
// to minus the current center, plus the current center (w 1.0); through
// `frame` + 80 to + 159.
void MotionRow(uint32_t record, uint32_t frame) {
  const Record r(record);
  const auto previous = r.previous();
  const auto current = r.current();
  vl::Vec3Difference(frame + 144, current.center_x_address(), previous.center_x_address());
  CopyWord(frame + 144 + 0, frame + 80);
  CopyWord(frame + 144 + 8, frame + 88);
  CopyWord(frame + 144 + 4, frame + 84);
  CopyWord(kOne, frame + 92);
  vl::Vec4Copy(r.motion_translation_x_address(), frame + 80);
  const uint32_t center = r.current().center_x_address();
  NegateWord(center + 0, frame + 96);
  NegateWord(center + 8, frame + 104);
  NegateWord(center + 4, frame + 100);
  CopyWord(kZero, frame + 108);
  vl::MatrixTransformPoint(frame + 128, r.motion_row0_x_address(), frame + 96);
  vl::Vec3Sum(frame + 144, frame + 128, r.current().center_x_address());
  CopyWord(frame + 144 + 0, frame + 112);
  CopyWord(kOne, frame + 124);
  CopyWord(frame + 144 + 8, frame + 120);
  CopyWord(frame + 144 + 4, frame + 116);
  vl::Vec4Copy(r.motion_translation_x_address(), frame + 112);
}

}  // namespace

void CollisionShapeUpdateBounds(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820BAC00 CollisionShape::UpdateBounds");
  const Record r(record);
  const uint32_t previous = r.previous().center_x_address();
  const uint32_t current = r.current().center_x_address();
  const double radius = double(r.template_shape().radius());
  for (uint32_t axis = 0; axis < 12; axis += 4) {
    const double a = Lfs(previous + axis);
    const double b = Lfs(current + axis);
    double high;
    if (a < b) {
      Stfs(r.bounds_min_x_address() + axis, Fsubs(a, radius));
      high = Lfs(current + axis);
    } else {
      Stfs(r.bounds_min_x_address() + axis, Fsubs(b, radius));
      high = Lfs(previous + axis);
    }
    Stfs(r.bounds_max_x_address() + axis, Fadds(high, radius));
  }
}

void CollisionShapeUpdateMotion(uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BAE78 CollisionShape::UpdateMotion");
  const Record r(record);
  const uint32_t previous = r.previous().axis0_x_address();
  const uint32_t current = r.current().axis0_x_address();
  for (uint32_t row = 0; row < 3; row++) {
    for (uint32_t column = 0; column < 3; column++) {
      Stfs(r.motion_row0_x_address() + 16 * row + 4 * column,
           double(vl::Vec3DotOf(current + 16 * row, previous + 16 * column)));
    }
  }
  MotionRow(record, frame);
}

bool CollisionShapePlace(uint32_t record, uint32_t matrix, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB758 CollisionShape::Place");
  const Record r(record);
  const uint32_t previous = r.previous().address;
  const uint32_t current = r.current().address;
  const uint32_t source = r.template_shape().address;
  // The copy to place (the previous one) answers 2 to its type when it's a
  // box; with +112 not 0.0 (fcmpu: NaN isn't equal) the template is copied
  // onto the stack (+80) and lowered.
  const uint32_t copy = frame + 80;
  if (!IsBox(previous) || !IsBox(source)) {
    return false;
  }
  const bool lowered = double(r.bottom_lowering()) != Lfs(kZero);
  if (lowered && !BoxCopyFits(copy, source)) {
    return false;
  }
  r.set_previous(current);
  r.set_current(previous);
  if (lowered) {
    // The stack copy (a box at `copy`): its min corner (corner 7)'s y
    // lowered, then its corners set again from the min and the max.
    const schema::BoxShape stack_copy(copy);
    BoxCopyConstruct(copy, r.template_shape().address);
    const uint32_t min_corner = stack_copy.corner_x_address() + 7 * 16;
    Stfs(min_corner + 4,
         Fsubs(Lfs(min_corner + 4), double(r.bottom_lowering())));
    BoxSetCorners(copy, min_corner, stack_copy.corner_x_address(), frame - 224);
    BoxPlace(r.current().address, matrix, copy, frame - 288);
    stack_copy.set_vtable(kShapeVtable);
  } else {
    BoxPlace(r.current().address, matrix, r.template_shape().address, frame - 288);
  }
  BoxCopyCache(r.current().address, r.previous().address);
  CollisionShapeUpdateMotion(record, frame - 240);
  CollisionShapeUpdateBounds(record);
  return true;
}

bool CollisionShapeTranslate(uint32_t record, uint32_t offset, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BACD0 CollisionShape::Translate");
  const Record r(record);
  if (!IsBox(r.current().address)) {
    return false;
  }
  BoxTranslate(r.current().address, offset);
  MotionRow(record, frame);
  CollisionShapeUpdateBounds(record);
  return true;
}

namespace {

// Whether the record's placed copies (+4, +8) can be deleted natively
// through their slot 0: each 0, or a box (slot 0 Shape::DeletingDestructor)
// the layer gave, the two not the same block.
bool CopiesDeletable(uint32_t record) {
  const Record r(record);
  const uint32_t previous = r.previous().address;
  const uint32_t current = r.current().address;
  for (uint32_t copy : {previous, current}) {
    if (copy != 0 && (!IsBox(copy) || !Deletable(copy))) {
      return false;
    }
  }
  return previous == 0 || previous != current;
}

// The copies deleted through slot 0 (r4 = 1) and zeroed, as the destructor
// and SetUp do.
void DeleteCopies(uint32_t record) {
  const Record r(record);
  for (uint32_t at : {r.previous_address(), r.current_address()}) {
    const uint32_t copy = guest::LoadU32(at);
    if (copy != 0) {
      ShapeDeletingDestructor(copy, 1);
      guest::StoreU32(at, 0);
    }
  }
}

}  // namespace

bool CollisionShapeDestroyable(uint32_t record) {
  return CopiesDeletable(record);
}

bool CollisionShapeDestroy(uint32_t record, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BAAA0 CollisionShape::~CollisionShape");
  if (!CopiesDeletable(record)) {
    return false;
  }
  // r3: the last copy loaded (+8), or what its slot 0 returned (the same).
  *r3 = Record(record).current().address;
  DeleteCopies(record);
  return true;
}

bool CollisionShapeSetUp(uint32_t record, uint32_t shape, uint32_t matrix,
                         uint32_t memory, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BB580 CollisionShape::SetUp");
  // The template's slots 5 (BoxShape::Clone) and 9 (BoxShape::Place) are
  // made natively only for a box; the copies' deletion and the clones'
  // allocation are checked before anything is written (malloc gives 0 with
  // no process heap, and the clones would take operator new's error path).
  if (!IsBox(shape) || !CopiesDeletable(record) ||
      (memory == 0 && memory::crt::GetProcessHeap() == 0)) {
    return false;
  }
  const uint32_t f = frame;
  DeleteCopies(record);
  uint32_t copy = 0;
  BoxClone(shape, memory, f - 96, &copy);
  const Record r(record);
  r.set_previous(copy);
  BoxClone(shape, memory, f - 96, &copy);
  r.set_current(copy);
  // Each placed by the matrix through its slot 9: a clone is a box (or 0,
  // which the original would call through too: not with a process heap).
  BoxPlace(r.previous().address, matrix, shape, f - 288);
  BoxPlace(r.current().address, matrix, shape, f - 288);
  r.set_template_shape(shape);
  matrix_math::SetIdentity(r.motion_row0_x_address());
  CopyWord(kZero, r.bottom_lowering_address());
  CollisionShapeUpdateBounds(record);
  return true;
}

}  // namespace collision
