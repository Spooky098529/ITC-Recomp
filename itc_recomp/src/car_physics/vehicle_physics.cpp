// The vehicle's physics objects (docs/functions/car-physics.md): its
// collision object in the world, the world's ground callback, its teardown.
// See car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "memory/c_runtime_heap.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;
namespace co = collision;

namespace {

constexpr uint32_t kWorldPointer = 0x826C2D88;  // the collision world
constexpr uint32_t kGroundCallback = 0x820FEAB0;  // TrackCollision::GroundCallback

uint32_t World(uint8_t* base) {
  return guest::LoadU32(base, kWorldPointer);
}

// Whether CollisionShape::Place runs natively on the record twice (the
// copies swapped between): the template and both copies boxes, and with
// the template lowered (+112 not 0.0) its stack copy native (the same
// place both times).
bool PlaceTwiceFits(uint8_t* base, uint32_t record, uint32_t place_frame) {
  const uint32_t source = guest::LoadU32(base, record + 0);
  if (!co::IsBox(base, source) || !co::IsBox(base, guest::LoadU32(base, record + 4)) ||
      !co::IsBox(base, guest::LoadU32(base, record + 8))) {
    return false;
  }
  const bool lowered = Lfs(base, record + 112) != Lfs(base, kZero);
  return !lowered || co::BoxCopyFits(place_frame + 80, source);
}

}  // namespace

void RigidBodySetFloat48(uint8_t* base, uint32_t body, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_8259D5A8 RigidBody::SetFloat48");
  CopyWord(base, from, body + 48);
}

bool CollisionWorldCallbackKnown(uint8_t* base) {
  return guest::LoadU32(base, World(base) + 8) == kGroundCallback;
}

bool CollisionWorldCallback(uint8_t* base, uint32_t result, uint32_t point, uint32_t position,
                            uint32_t r1, uint32_t* r3) {
  if (!CollisionWorldCallbackKnown(base)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B8798 CollisionWorld::Callback");
  // The tail call: the callback's 96-byte frame from the caller's r1.
  *r3 = track_collision::TrackCollisionGroundCallback(base, result, point, position, r1 - 96);
  return true;
}

bool CollisionWorldPairsMovable(uint8_t* base) {
  const uint32_t world = World(base);
  const uint32_t pairs = guest::LoadU32(base, world + 36);
  const uint32_t count = guest::LoadU32(base, world + 40);
  if (count < 2) {
    return true;
  }
  const uint64_t end = uint64_t(pairs) + 32ull * count;
  return end <= 0xA0000000ull && c_runtime::InOneView(pairs, 32 * count);
}

bool CollisionObjectResetTransform(uint8_t* base, uint32_t object, uint32_t frame) {
  const uint32_t record = object + 32;
  const uint32_t place_frame = frame - 608;
  if (!PlaceTwiceFits(base, record, place_frame)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B7998 CollisionObject::ResetTransform");
  guest::StoreU32(base, object + 288, 1);
  co::CollisionShapePlace(base, record,
                          va::RigidBodyMatrix(guest::LoadU32(base, object + 16)), place_frame);
  co::CollisionShapePlace(base, record,
                          va::RigidBodyMatrix(guest::LoadU32(base, object + 16)), place_frame);
  return true;
}

bool CollisionObjectInitFits(uint8_t* base, uint32_t object, uint32_t shape, uint32_t memory) {
  return co::IsBox(base, shape) && co::CollisionShapeDestroyable(base, object + 32) &&
         !(memory == 0 && memory::crt::GetProcessHeap(base) == 0);
}

void CollisionObjectInit(uint8_t* base, uint32_t object, uint32_t body, uint32_t shape,
                         uint32_t groups, uint32_t memory, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7A00 CollisionObject::Init");
  const uint32_t o = object;
  guest::StoreU32(base, o + 16, body);
  guest::StoreU32(base, o + 20, shape);
  guest::StoreU32(base, o + 244, groups);
  co::CollisionShapeSetUp(base, o + 32, shape, va::RigidBodyMatrix(body), memory, frame - 128);
  for (uint32_t at = 248; at <= 260; at += 4) {
    guest::StoreU32(base, o + at, 0xFFFFFFFFu);
  }
  vl::Vec4Copy(base, o + 160, kZeroVector);
  vl::Vec4Copy(base, o + 176, kZeroVector);
  guest::StoreU32(base, o + 192, 0);
  guest::StoreU32(base, o + 240, 0);
  guest::StoreU32(base, o + 236, 0);
  // +208 = (1, 1, 1) with its w loaded first and stored back (lfs then
  // stfs: its bits).
  const uint32_t w = guest::LoadU32(base, o + 220);
  CopyWord(base, kOne, o + 208);
  CopyWord(base, kOne, o + 212);
  CopyWord(base, kOne, o + 216);
  guest::StoreU32(base, o + 220, w);
  guest::StoreU32(base, o + 268, 0);
  CopyWord(base, kOne, o + 224);
  for (uint32_t at = 272; at <= 288; at += 4) {
    guest::StoreU32(base, o + at, 0);
  }
  guest::StoreU32(base, o + 264, 0);
  vl::Vec4Copy(base, o + 304, kZeroVector);
}

bool CollisionWorldAddObject(uint8_t* base, uint32_t body, uint32_t shape, uint32_t groups,
                             uint32_t frame, uint32_t* object) {
  const uint32_t free_object = guest::LoadU32(base, World(base) + 24);
  if (free_object == 0 ||
      !CollisionObjectInitFits(base, free_object, shape, guest::LoadU32(base, World(base) + 0))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B9570 CollisionWorld::AddObject");
  va::CollisionObjectUnlink(base, free_object, World(base) + 24);
  va::CollisionObjectLink(base, free_object, World(base) + 20);
  uint32_t world = World(base);
  guest::StoreU32(base, world + 28, guest::LoadU32(base, world + 28) + 1);
  world = World(base);
  CollisionObjectInit(base, free_object, body, shape, groups, guest::LoadU32(base, world + 0),
                      frame - 112);
  CollisionWorldAddPairs(base, free_object, guest::LoadU32(base, free_object + 248));
  *object = free_object;
  return true;
}

bool VehicleDestroyPhysics(uint8_t* base, uint32_t vehicle, uint32_t* r3) {
  const uint32_t object = guest::LoadU32(base, vehicle + 340);
  const uint32_t shape = guest::LoadU32(base, vehicle + 344);
  const uint32_t body = guest::LoadU32(base, vehicle + 336);
  // Everything checked first: the pairs' removal, the shape's slot 0 (a
  // box's: Shape::DeletingDestructor) and its block, the body's block.
  if ((object != 0 && !CollisionWorldPairsMovable(base)) ||
      (shape != 0 && (!co::IsBox(base, shape) || !co::Deletable(shape))) ||
      !co::Deletable(body)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B0E70 Vehicle::DestroyPhysics");
  if (object != 0) {
    uint32_t ignored = 0;
    CollisionWorldRemoveObject(base, object, &ignored);
    guest::StoreU32(base, vehicle + 340, 0);
  }
  // r3: the +344 loaded (slot 0 returns the shape), then the body's.
  *r3 = shape;
  if (shape != 0) {
    co::ShapeDeletingDestructor(base, shape, 1);
    guest::StoreU32(base, vehicle + 344, 0);
  }
  if (body != 0) {
    c_runtime::EmptyDestructor();
    memory::crt::OperatorDelete(body);
    guest::StoreU32(base, vehicle + 336, 0);
    *r3 = body;
  }
  return true;
}

}  // namespace car_physics
