// The collision world's objects, contacts and surface settings
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "memory/c_runtime_heap.h"
#include "schema/car_physics.h"
#include "schema/collision.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;

namespace {

// The surface settings: four values (groups A to D) for each of the
// default and the three flag bits.
struct SurfaceGroup {
  uint32_t fallback, bit24, bit25, bit26;
};
constexpr SurfaceGroup kGroupA = {0x826C2DFC, 0x826C2E20, 0x826C2E04, 0x826C2E18};
constexpr SurfaceGroup kGroupB = {0x826C2E08, 0x826C2DF4, 0x826C2E24, 0x826C2E1C};
constexpr SurfaceGroup kGroupC = {0x826C2DF0, 0x826C2E0C, 0x826C2E10, 0x826C2E00};
constexpr SurfaceGroup kGroupD = {0x826C2E14, 0x826C2DF8, 0x826C2E28, 0x826C2E2C};

double SurfaceGet(const SurfaceGroup& g, uint32_t flags, uint32_t fallback) {
  if ((flags & 0x01000000) != 0) {
    return Lfs(g.bit24);
  }
  if ((flags & 0x02000000) != 0) {
    return Lfs(g.bit25);
  }
  if ((flags & 0x04000000) != 0) {
    return Lfs(g.bit26);
  }
  return Lfs(fallback);
}

void SurfaceSet(const SurfaceGroup& g, uint32_t bits, double value) {
  if ((bits & 0x07000000) != 0) {
    Stfs(g.fallback, value);
  }
  if ((bits & 0x01000000) != 0) {
    Stfs(g.bit24, value);
  }
  if ((bits & 0x02000000) != 0) {
    Stfs(g.bit25, value);
  }
  if ((bits & 0x04000000) != 0) {
    Stfs(g.bit26, value);
  }
}

// An integer's float through the stack (extsw, std at `at`, lfd, fcfid,
// frsp).
double StackIntegerFloat(uint32_t at, int64_t value) {
  guest::StoreU32(at, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(at + 4, uint32_t(value));
  return double(float(double(value)));
}

// A record's push (+32) normalized, its length into +48 (0.0 for a zero
// vector, left as it is), as the merge does it.
void NormalizePush(uint32_t record) {
  const schema::CollisionHit hit(record);
  const double squared = double(vl::Vec3Dot(hit.push_address(), hit.push_address()));
  if (squared == Lfs(kZero)) {
    CopyWord(kZero, hit.depth_address());
    return;
  }
  const float length = vl::Sqrtf(squared);
  hit.set_depth(length);
  vl::Vec4DivideBy(hit.push_address(), double(length));
}

}  // namespace

double CollisionSurfaceA(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC1F8 CollisionSurface::A");
  return SurfaceGet(kGroupA, flags, kGroupA.fallback);
}
double CollisionSurfaceB(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC240 CollisionSurface::B");
  return SurfaceGet(kGroupB, flags, kGroupB.fallback);
}
double CollisionSurfaceC(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC288 CollisionSurface::C");
  return SurfaceGet(kGroupC, flags, kGroupC.fallback);
}
double CollisionSurfaceD(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC2D0 CollisionSurface::D");
  // Group B's default, not D's (the game's).
  return SurfaceGet(kGroupD, flags, kGroupB.fallback);
}
double CollisionSurfaceOfA(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC540 CollisionSurface::OfA");
  return CollisionSurfaceA(flags);
}
double CollisionSurfaceOfB(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC548 CollisionSurface::OfB");
  return CollisionSurfaceB(flags);
}
double CollisionSurfaceOfC(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC530 CollisionSurface::OfC");
  return CollisionSurfaceC(flags);
}
double CollisionSurfaceOfD(uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC538 CollisionSurface::OfD");
  return CollisionSurfaceD(flags);
}

void CollisionSurfaceSetA(uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC318 CollisionSurface::SetA");
  SurfaceSet(kGroupA, bits, value);
}
void CollisionSurfaceSetB(uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC360 CollisionSurface::SetB");
  SurfaceSet(kGroupB, bits, value);
}
void CollisionSurfaceSetC(uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC3A8 CollisionSurface::SetC");
  SurfaceSet(kGroupC, bits, value);
}
void CollisionSurfaceSetD(uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC3F0 CollisionSurface::SetD");
  SurfaceSet(kGroupD, bits, value);
}

void CollisionSurfacesReset() {
  ITC_NATIVE_RUNNING("sub_820BC438 CollisionSurfaces::Reset");
  // Groups A and C 0.5, B and D 1.0 (the floats' bits), in the original's
  // order.
  constexpr uint32_t kHalf = 0x82001BE4;
  constexpr struct {
    uint32_t at;
    bool one;
  } kStores[] = {{0x826C2DF0, false}, {0x826C2E14, true}, {0x826C2DFC, false},
                 {0x826C2E08, true},  {0x826C2E10, false}, {0x826C2E28, true},
                 {0x826C2E04, false}, {0x826C2E24, true},  {0x826C2E0C, false},
                 {0x826C2DF8, true},  {0x826C2E20, false}, {0x826C2DF4, true},
                 {0x826C2E00, false}, {0x826C2E2C, true},  {0x826C2E18, false},
                 {0x826C2E1C, true}};
  for (const auto& s : kStores) {
    CopyWord(s.one ? kOne : kHalf, s.at);
  }
}

void CollisionWorldResetSurfaces() {
  ITC_NATIVE_RUNNING("sub_820BC550 CollisionWorld::ResetSurfaces");
  CollisionSurfacesReset();
}

void CollisionShapeInit(uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_820BAA78 CollisionShape::Init");
  const schema::CollisionShape record(shape);
  CopyWord(kZero, record.bounds_min_w_address());
  record.set_previous(0u);
  CopyWord(kZero, record.bounds_max_w_address());
  record.set_current(0u);
  record.set_template_shape(0u);
}

void CollisionObjectConstruct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7AE8 CollisionObject::CollisionObject");
  schema::CollisionWorldObject(object).set_vtable(uint32_t(0x82002A94));
  CollisionShapeInit(schema::CollisionWorldObject(object).shape_record_address());
  const schema::CollisionWorldObject obj(object);
  CopyWord(kZero, obj.push_address() + 12);
  CopyWord(kZero, obj.pair_torque_address() + 12);
  CopyWord(kZero, obj.push_scale_address() + 12);
  CopyWord(kZero, obj.push_out_address() + 12);
}

uint32_t CollisionObjectAccumulateForce(uint32_t object, uint32_t point,
                                        uint32_t force, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7DF8 CollisionObject::AccumulateForce");
  const uint32_t f = frame;
  CopyWord(kZero, f + 92);
  const uint32_t center = va::ContactPosition(schema::CollisionWorldObject(object).body().address);
  vl::Vec3Difference(f + 96, point, center);
  vl::Vec3Cross(f + 112, f + 96, force);
  vl::Vec4Copy(f + 80, f + 112);
  vl::Vec3AddTo(schema::CollisionWorldObject(object).pair_torque_address(), f + 80);
  // The largest push per axis (+160 to +168).
  for (uint32_t i = 0; i < 12; i += 4) {
    if (Fabs(Lfs(force + i)) > Fabs(Lfs(schema::CollisionWorldObject(object).push_address() + i))) {
      CopyWord(force + i, schema::CollisionWorldObject(object).push_address() + i);
    }
  }
  schema::CollisionWorldObject(object).set_unmoved(uint32_t(0));
  schema::CollisionWorldObject(object).set_push_count(uint32_t(schema::CollisionWorldObject(object).push_count() + 1));
  return schema::CollisionWorldObject(object).pair_torque_address();
}

uint32_t CollisionObjectAddContact(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7C50 CollisionObject::AddContact");
  const uint32_t contact = va::CollisionWorldNewContact();
  if (contact != 0) {
    schema::CollisionContact(contact).set_next(uint32_t(schema::CollisionWorldObject(object).first_contact().address));
    schema::CollisionWorldObject(object).set_first_contact(uint32_t(contact));
  }
  return contact;
}

bool CollisionObjectTranslate(uint32_t object, uint32_t offset, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7B60 CollisionObject::Translate");
  // CollisionShape::Translate runs natively only on a box (its current
  // copy, the record's +8): else the original from the start.
  if (!collision::IsBox(schema::CollisionShape(schema::CollisionWorldObject(object).shape_record_address()).current().address)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t body = schema::CollisionWorldObject(object).body().address;
  // The body's center (+16, its 16 bytes by ld and std) moved by the offset.
  const uint32_t center = va::ContactPosition(body);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(f + 80 + i, guest::LoadU32(center + i));
  }
  vl::Vec3AddTo(f + 80, offset);
  RigidBodySetCenterOfMassPosition(schema::CollisionWorldObject(object).body().address, f + 80, f - 96);
  collision::CollisionShapeTranslate(schema::CollisionWorldObject(object).shape_record_address(), offset, f - 192);
  vl::Vec3AddTo(schema::CollisionWorldObject(object).push_out_address(), offset);
  schema::CollisionWorldObject(object).set_unmoved(uint32_t(0));
  return true;
}

uint32_t CollisionRecordsMerge(uint32_t out, uint32_t summary, uint32_t hits,
                               uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7ED0 CollisionRecords::Merge");
  const uint32_t f = frame;
  for (uint32_t i = 0; i < count; i++) {
    va::CollisionRecordCopy(out + 80 * i, hits + 80 * i);
  }
  uint32_t kept = 0;
  if (count == 0) {
    va::CollisionRecordCopy(summary, out);
    return kept;
  }
  const double one = Lfs(kOne);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t record = out + 80 * i;
    if (int32_t(schema::CollisionHit(record).key()) == -1) {
      continue;
    }
    // The later records with the same id (+60) merged into this one: their
    // points and normals summed, their pushes (+32 x +48) too.
    int32_t merged = 0;
    for (uint32_t j = i + 1; j < count; j++) {
      const uint32_t other = out + 80 * j;
      if (schema::CollisionHit(record).key() != schema::CollisionHit(other).key()) {
        continue;
      }
      vl::Vec3AddTo(record, other);
      vl::Vec3AddTo(schema::CollisionHit(record).normal_address(), schema::CollisionHit(other).normal_address());
      const double strength = double(schema::CollisionHit(other).depth());
      if (merged > 0) {
        vl::Vec4Scaled(f + 96, schema::CollisionHit(other).push_address(), strength);
        vl::Vec3AddTo(schema::CollisionHit(record).push_address(), f + 96);
      } else {
        vl::Vec4Scaled(f + 112, schema::CollisionHit(other).push_address(), strength);
        vl::Vec4Scaled(f + 128, schema::CollisionHit(record).push_address(), double(schema::CollisionHit(record).depth()));
        vl::Vec3Sum(f + 144, f + 128, f + 112);
        vl::Vec4Copy(schema::CollisionHit(record).push_address(), f + 144);
      }
      schema::CollisionHit(other).set_key(uint32_t(uint32_t(-1)));
      merged++;
    }
    if (merged != 0) {
      const double scale = Fdivs(one, StackIntegerFloat(f + 80, merged + 1));
      vl::Vec4ScaleBy(record, scale);
      vl::Vec4ScaleBy(schema::CollisionHit(record).normal_address(), scale);
      vl::Vec4ScaleBy(schema::CollisionHit(record).push_address(), scale);
      NormalizePush(record);
    }
    if (int32_t(i) != int32_t(kept)) {
      va::CollisionRecordCopy(out + 80 * kept, record);
    }
    kept++;
  }
  if (!(int32_t(kept) > 1)) {
    va::CollisionRecordCopy(summary, out);
    return kept;
  }
  // The summary: the average of the records with surface bits.
  for (uint32_t i = 0; i < 80; i += 4) {
    guest::StoreU32(summary + i, 0);
  }
  for (uint32_t k = 0; k < kept; k++) {
    const uint32_t record = out + 80 * k;
    if ((schema::CollisionHit(record).surface_bits() & 0x07000000) == 0) {
      continue;
    }
    vl::Vec3AddTo(summary, record);
    vl::Vec3AddTo(schema::CollisionHit(summary).normal_address(), schema::CollisionHit(record).normal_address());
    vl::Vec4Scaled(f + 144, schema::CollisionHit(record).push_address(), double(schema::CollisionHit(record).depth()));
    vl::Vec3AddTo(schema::CollisionHit(summary).push_address(), f + 144);
    schema::CollisionHit(summary).set_surface_bits(uint32_t(schema::CollisionHit(record).surface_bits()));
    schema::CollisionHit(summary).set_key(uint32_t(schema::CollisionHit(record).key()));
  }
  vl::Vec4ScaleBy(summary, Fdivs(one, StackIntegerFloat(f + 80, int32_t(kept))));
  vl::Vec3NormalizeSelf(schema::CollisionHit(summary).normal_address());
  NormalizePush(summary);
  return kept;
}


// --- The world's pairs and its reset ---

namespace {

uint32_t World() {
  return schema::CarPhysicsGlobals::collision_world().address;
}

// The pair's last in an object's list (+0/+8 on the side where the pair's
// +16 is the object, +4/+12 on the other), from the head +264; 0 for none.
uint32_t ListTail(uint32_t object) {
  uint32_t pair = va::CollisionObjectWord264(object);
  uint32_t last = 0;
  while (pair != 0) {
    last = pair;
    pair = schema::CollisionPair(pair).a().address == object ? schema::CollisionPair(pair).a_next().address
                                                     : schema::CollisionPair(pair).b_next().address;
  }
  return last;
}

// A neighbor's link toward the pair repointed: its next (+8, or +12 on its
// B side) when `next`, else its previous (+0, or +4), by whether its +16 is
// `object`.
void Repoint(uint32_t neighbor, uint32_t object, bool next, uint32_t to) {
  const bool a_side = schema::CollisionPair(neighbor).a().address == object;
  guest::StoreU32(schema::CollisionPair(neighbor).a_previous_address() + (next ? 8 : 0) + (a_side ? 0 : 4), to);
}

}  // namespace

bool CollisionWorldHasPair(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820B8C20 CollisionWorld::HasPair");
  const uint32_t world = World();
  const uint32_t count = schema::CollisionWorld(world).pair_count();
  if (count == 0) {
    return false;
  }
  uint32_t pair = schema::CollisionWorld(world).pairs().address;
  for (uint32_t i = 0; i < count; i++, pair += 32) {
    const uint32_t first = schema::CollisionPair(pair).a().address;
    if (first == a && schema::CollisionPair(pair).b().address == b) {
      return true;
    }
    if (first == b && schema::CollisionPair(pair).b().address == a) {
      return true;
    }
  }
  return false;
}

uint32_t CollisionWorldLinkPair(uint32_t pair, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BC068 CollisionWorld::LinkPair");
  CopyWord(kZero, schema::CollisionPair(pair).next_check_address());
  schema::CollisionPair(pair).set_b(uint32_t(b));
  schema::CollisionPair(pair).set_a(uint32_t(a));
  schema::CollisionPair(pair).set_a_next(uint32_t(0));
  schema::CollisionPair(pair).set_b_next(uint32_t(0));
  schema::CollisionPair(pair).set_separated_face(uint32_t(0));
  const uint32_t tail_a = ListTail(a);
  schema::CollisionPair(pair).set_a_previous(uint32_t(tail_a));
  if (tail_a == 0) {
    va::CollisionObjectSetWord264(schema::CollisionPair(pair).a().address, pair);
  } else {
    Repoint(tail_a, schema::CollisionPair(pair).a().address, true, pair);
  }
  const uint32_t second = schema::CollisionPair(pair).b().address;
  const uint32_t tail_b = ListTail(second);
  schema::CollisionPair(pair).set_b_previous(uint32_t(tail_b));
  if (tail_b == 0) {
    const uint32_t object = schema::CollisionPair(pair).b().address;
    va::CollisionObjectSetWord264(object, pair);
    return object;
  }
  Repoint(tail_b, schema::CollisionPair(pair).b().address, true, pair);
  return 0;
}

uint32_t CollisionWorldAddPairs(uint32_t object, uint32_t groups) {
  ITC_NATIVE_RUNNING("sub_820B94B0 CollisionWorld::AddPairs");
  uint32_t r3 = object;
  uint32_t world = World();
  for (uint32_t other = schema::CollisionWorld(world).active().address; other != 0;
       other = schema::CollisionWorldObject(other).next().address) {
    if (other == object || (schema::CollisionWorldObject(other).groups() & groups) == 0) {
      continue;
    }
    const uint32_t owner = schema::CollisionWorldObject(object).owner_id();
    if (owner == schema::CollisionWorldObject(other).owner_id() && owner != 0) {
      continue;
    }
    r3 = CollisionWorldHasPair(object, other) ? 1 : 0;
    if (r3 != 0) {
      continue;
    }
    const uint32_t pair =
        (schema::CollisionWorld(world).pair_count() << 5) + schema::CollisionWorld(world).pairs().address;
    CopyWord(kZero, schema::CollisionPair(pair).next_check_address());
    schema::CollisionWorld(world).set_pair_count(uint32_t(schema::CollisionWorld(world).pair_count() + 1));
    r3 = CollisionWorldLinkPair(pair, object, other);
    world = World();
  }
  return r3;
}

uint32_t CollisionWorldUnlinkPair(uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_820BBC40 CollisionWorld::UnlinkPair");
  uint32_t r3 = pair;
  // The neighbors before it (+0 on A's side, +4 on B's) pointed past it.
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t previous = guest::LoadU32(schema::CollisionPair(pair).a_previous_address() + side);
    const uint32_t next = guest::LoadU32(schema::CollisionPair(pair).a_next_address() + side);
    if (previous == 0) {
      r3 = guest::LoadU32(schema::CollisionPair(pair).a_address() + side);
      va::CollisionObjectSetWord264(r3, next);
    } else {
      Repoint(previous, guest::LoadU32(schema::CollisionPair(pair).a_address() + side), true, next);
    }
  }
  // The neighbors after it pointed back past it.
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t next = guest::LoadU32(schema::CollisionPair(pair).a_next_address() + side);
    const uint32_t previous = guest::LoadU32(schema::CollisionPair(pair).a_previous_address() + side);
    if (next != 0) {
      Repoint(next, guest::LoadU32(schema::CollisionPair(pair).a_address() + side), false, previous);
    }
  }
  return r3;
}

void CollisionWorldMovePair(uint32_t pair, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820BBD30 CollisionWorld::MovePair");
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t previous = guest::LoadU32(schema::CollisionPair(pair).a_previous_address() + side);
    if (previous == 0) {
      va::CollisionObjectSetWord264(guest::LoadU32(schema::CollisionPair(pair).a_address() + side), slot);
    } else {
      Repoint(previous, guest::LoadU32(schema::CollisionPair(pair).a_address() + side), true, slot);
    }
  }
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t next = guest::LoadU32(schema::CollisionPair(pair).a_next_address() + side);
    if (next != 0) {
      Repoint(next, guest::LoadU32(schema::CollisionPair(pair).a_address() + side), false, slot);
    }
  }
  // memcpy (sub_825A7410), 32 bytes; its caller checked first that the
  // native copy does what the original would (CollisionWorldRemovePair).
  c_runtime::Memcpy(slot, pair, 32);
}

bool CollisionWorldRemovePair(uint32_t index, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8A88 CollisionWorld::RemovePair");
  uint32_t world = World();
  const uint32_t slot = (index << 5) + schema::CollisionWorld(world).pairs().address;
  const uint32_t count = schema::CollisionWorld(world).pair_count();
  const bool moves = count - 1 > index;
  const uint32_t last = (count << 5) + schema::CollisionWorld(world).pairs().address - 32;
  // The move's memcpy natively only where it's what the library's memcpy
  // does and its override has nothing to record (outside the physical
  // views, the two ranges not sharing memory); else the original, with
  // nothing done. (Unlinking writes only pairs and objects, so the world's
  // +36 and +40 are the same after it.)
  if (moves && !(slot < 0xA0000000u && last < 0xA0000000u && c_runtime::InOneView(slot, 32) &&
                 c_runtime::InOneView(last, 32) && !c_runtime::MayShare(slot, 32, last, 32))) {
    return false;
  }
  *r3 = CollisionWorldUnlinkPair(slot);
  world = World();
  const uint32_t count_after = schema::CollisionWorld(world).pair_count();
  if (count_after - 1 > index) {
    CollisionWorldMovePair((count_after << 5) + schema::CollisionWorld(world).pairs().address - 32,
                           slot);
    *r3 = slot;
    world = World();
  }
  schema::CollisionWorld(world).set_pair_count(uint32_t(schema::CollisionWorld(world).pair_count() - 1));
  return true;
}

uint32_t CollisionWorldReset() {
  ITC_NATIVE_RUNNING("sub_820B85E8 CollisionWorld::Reset");
  schema::CollisionWorld(World()).set_updating(uint32_t(0));
  schema::CollisionWorld(World()).set_query_count(uint32_t(32));
  schema::CollisionWorld(World()).set_active(uint32_t(0));
  uint32_t world = World();
  schema::CollisionWorld(world).set_free(uint32_t(schema::CollisionWorld(world).objects().address));
  world = World();
  // Every object on the free list: +228 the next, +232 the previous.
  uint32_t count = schema::CollisionWorld(world).object_count();
  for (uint32_t i = 0; i < count; i++) {
    if (i < count - 1) {
      const uint32_t object = schema::CollisionWorld(world).objects().address + 320 * i;
      schema::CollisionWorldObject(object).set_next(uint32_t(object + 320));
    } else {
      schema::CollisionWorldObject(count * 320 + schema::CollisionWorld(world).objects().address - 320).set_next(uint32_t(0));
    }
    const uint32_t object = schema::CollisionWorld(World()).objects().address + 320 * i;
    schema::CollisionWorldObject(object).set_previous(uint32_t(int32_t(i) > 0 ? object - 320 : 0));
    world = World();
    count = schema::CollisionWorld(world).object_count();
  }
  schema::CollisionWorld(world).set_active_count(uint32_t(0));
  schema::CollisionWorld(World()).set_pair_count(uint32_t(0));
  schema::CollisionWorld(World()).set_contacts_used(uint32_t(0));
  CollisionWorldResetSurfaces();
  return 0x826C2E30;
}


bool CollisionWorldRemovePairsOf(uint32_t object, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8BA8 CollisionWorld::RemovePairsOf");
  *r3 = object;
  uint32_t world = World();
  if (schema::CollisionWorld(world).pair_count() == 0) {
    return true;
  }
  // The pair at `pair` is index `i`'s; after a removal the last pair is in
  // its slot, tested next.
  uint32_t pair = schema::CollisionWorld(world).pairs().address;
  for (uint32_t i = 0; i < schema::CollisionWorld(world).pair_count(); i++) {
    if (schema::CollisionPair(pair).a().address == object || schema::CollisionPair(pair).b().address == object) {
      if (!CollisionWorldRemovePair(i, r3)) {
        return false;
      }
      world = World();
      i--;
    } else {
      pair += 32;
    }
  }
  return true;
}

bool CollisionWorldRemovePairsByGroups(uint32_t object, uint32_t groups,
                                       uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8AF8 CollisionWorld::RemovePairsByGroups");
  *r3 = object;
  uint32_t world = World();
  if (schema::CollisionWorld(world).pair_count() == 0) {
    return true;
  }
  uint32_t pair = schema::CollisionWorld(world).pairs().address;
  for (uint32_t i = 0; i < schema::CollisionWorld(world).pair_count(); i++) {
    const uint32_t a = schema::CollisionPair(pair).a().address;
    const uint32_t b = schema::CollisionPair(pair).b().address;
    // The other object out of the groups, unless the object's +244 still
    // meets its +248 (or, on B's side, its +244 the object's +248).
    bool remove = false;
    if (a == object) {
      remove = (schema::CollisionWorldObject(b).groups() & groups) != 0 &&
               (schema::CollisionWorldObject(a).groups() & schema::CollisionWorldObject(b).collides_with()) == 0;
    } else if (b == object) {
      remove = (schema::CollisionWorldObject(a).groups() & groups) != 0 &&
               (schema::CollisionWorldObject(a).collides_with() & schema::CollisionWorldObject(b).groups()) == 0;
    }
    if (remove) {
      if (!CollisionWorldRemovePair(i, r3)) {
        return false;
      }
      world = World();
      i--;
    } else {
      pair += 32;
    }
  }
  return true;
}

bool CollisionWorldRemoveObject(uint32_t object, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B9448 CollisionWorld::RemoveObject");
  va::CollisionObjectUnlink(object, schema::CollisionWorld(World()).address + 20);  // the active list's head
  va::CollisionObjectLink(object, schema::CollisionWorld(World()).address + 24);  // the free list's head
  const uint32_t world = World();
  schema::CollisionWorld(world).set_active_count(uint32_t(schema::CollisionWorld(world).active_count() - 1));
  return CollisionWorldRemovePairsOf(object, r3);
}

uint32_t CollisionObjectAddGroups(uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7CC8 CollisionObject::AddGroups");
  const uint32_t had = schema::CollisionWorldObject(object).collides_with();
  const uint32_t added = bits & ~had;
  if (added == 0) {
    return object;
  }
  schema::CollisionWorldObject(object).set_collides_with(uint32_t(had | bits));
  if ((added & 0x00FFFFFF) == 0) {
    return object;
  }
  return CollisionWorldAddPairs(object, added);
}

bool CollisionObjectRemoveGroups(uint32_t object, uint32_t bits,
                                 uint32_t* removed_groups, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B7CF0 CollisionObject::RemoveGroups");
  *r3 = object;
  *removed_groups = 0;
  const uint32_t had = schema::CollisionWorldObject(object).collides_with();
  const uint32_t removed = had & bits;
  if (removed == 0) {
    return true;
  }
  schema::CollisionWorldObject(object).set_collides_with(uint32_t(had & ~bits));
  if ((removed & 0x00FFFFFF) == 0) {
    return true;
  }
  *removed_groups = removed;
  return CollisionWorldRemovePairsByGroups(object, removed, r3);
}

namespace {

constexpr uint32_t kObjectVtable = 0x82002A94;
constexpr uint32_t kObjectDeletingDestructor = 0x820B9110;
constexpr uint32_t kObjectBytes = 320;

}  // namespace

void CollisionObjectDestruct(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7B40 CollisionObject::~CollisionObject");
  schema::CollisionWorldObject(object).set_vtable(uint32_t(kObjectVtable));
  schema::CollisionWorldObject(object).set_owner_id(uint32_t(0));
  uint32_t r3 = 0;
  collision::CollisionShapeDestroy(schema::CollisionWorldObject(object).shape_record_address(), &r3);
}

uint32_t CollisionObjectDeletingDestructor(uint32_t object, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820B9110 CollisionObject::DeletingDestructor");
  if ((flags & 0x2) == 0) {
    CollisionObjectDestruct(object);
    if ((flags & 0x1) != 0) {
      memory::crt::OperatorDelete(object);
    }
    return object;
  }
  const uint32_t block = object - 16;
  const uint32_t count = schema::CollisionObjectBlock(block).count();
  uint32_t at = count * kObjectBytes + object;
  for (int32_t i = int32_t(count) - 1; i >= 0; i--) {
    at -= kObjectBytes;
    CollisionObjectDestruct(at);
  }
  if ((flags & 0x1) != 0) {
    memory::crt::OperatorDeleteArray(block);
  }
  return block;
}

bool CollisionWorldDestroyable() {
  const uint32_t world = schema::CarPhysicsGlobals::collision_world().address;
  if (world == 0) {
    return false;
  }
  const uint32_t objects = schema::CollisionWorld(world).objects().address;
  if (objects != 0) {
    const uint32_t block = objects - 16;
    if (!collision::Deletable(block)) {
      return false;
    }
    const uint32_t count = schema::CollisionObjectBlock(block).count();
    if (count != 0) {
      const uint32_t vtable = guest::VtableWord(objects);
      if (vtable != kObjectVtable || guest::LoadU32(vtable) != kObjectDeletingDestructor) {
        return false;
      }
      for (int32_t i = int32_t(count) - 1; i >= 0; i--) {
        if (!collision::CollisionShapeDestroyable(objects + kObjectBytes * uint32_t(i) + 32)) {
          return false;
        }
      }
    }
  }
  return collision::Deletable(schema::CollisionWorld(world).pairs().address) &&
         collision::Deletable(schema::CollisionWorld(world).contacts().address) && collision::Deletable(world);
}

void CollisionWorldDestroy() {
  ITC_NATIVE_RUNNING("sub_820B86B8 CollisionWorld::Destroy");
  const uint32_t objects = schema::CarPhysicsGlobals::collision_world().objects().address;
  if (objects != 0) {
    const uint32_t block = objects - 16;
    if (schema::CollisionObjectBlock(block).count() != 0) {
      // The first object's slot 0 (CollisionWorldDestroyable checked it).
      CollisionObjectDeletingDestructor(objects, 3);
    } else {
      schema::CollisionObjectBlock(block).Delete();
    }
  }
  schema::CarPhysicsGlobals::collision_world().set_objects(uint32_t(0));
  memory::crt::OperatorDeleteArray(schema::CarPhysicsGlobals::collision_world().pairs().address);
  schema::CarPhysicsGlobals::collision_world().set_pairs(uint32_t(0));
  schema::CarPhysicsGlobals::collision_world().contacts().Delete();
  schema::CarPhysicsGlobals::collision_world().set_contacts(uint32_t(0));
  memory::crt::OperatorDelete(schema::CarPhysicsGlobals::collision_world().address);
  schema::CarPhysicsGlobals::set_collision_world(uint32_t(0));
}

}  // namespace car_physics

// --- The collision world's set-up ---

namespace car_physics {

namespace {

constexpr uint32_t kRefused = 0xFFFFFFFFu;  // a size the originals make -1
// The objects in CollisionObjectBlock (its schema's embed count).
constexpr uint32_t kBlockObjects =
    (schema::CollisionObjectBlock::kSize - 16) / schema::CollisionWorldObject::kSize;

// operator new[]'s block, as its plain C++ gives it (0 where it gives none:
// checked before by the callers, never in play).
uint32_t NewArray(uint32_t bytes) {
  return memory::crt::OperatorNewArray(bytes).value_or(0);
}

// `count` records of `bytes` (80 here) with the words at +12, +28 and +44
// from `first` (the record's offset + 12) zeroed (stfs of 0.0).
void ZeroRecordWs(uint32_t block, uint32_t count, uint32_t first) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t at = block + first + 80 * i;
    CopyWord(kZero, at);
    CopyWord(kZero, at + 16);
    CopyWord(kZero, at + 32);
  }
}

// The sizes as the originals work them out (mulli, with their cmplw
// limits), -1 past them.
uint32_t ObjectsBytes(uint32_t n) {
  const uint32_t bytes = n > 0x00CCCCCCu ? kRefused : n * 320;
  return bytes > 0xFFFFFFEFu ? kRefused : bytes + 16;
}
uint32_t PairsBytes(uint32_t n) {
  return n > 0x07FFFFFFu ? kRefused : n << 5;
}
uint32_t RecordsBytes(uint32_t n) {
  return n > 0x03333333u ? kRefused : n * 80;
}
// Whether operator new[] gives a block for `bytes` (malloc's limit; the
// process heap is checked by the callers).
bool ArrayFits(uint32_t bytes) {
  return bytes <= 0xFFFFF000u;
}

}  // namespace

bool CollisionWorldCreate(uint32_t objects, uint32_t pairs, uint32_t contacts,
                          uint32_t* r3) {
  // The objects' block is a host object of 50 (E5 step 4): Setup's count,
  // the only one; another goes to the original.
  if (objects != kBlockObjects || memory::crt::GetProcessHeap() == 0 ||
      !ArrayFits(ObjectsBytes(objects)) ||
      !ArrayFits(PairsBytes(pairs)) || !ArrayFits(RecordsBytes(64)) ||
      !ArrayFits(RecordsBytes(contacts))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B8D30 CollisionWorld::Create");
  const uint32_t world_block = schema::CollisionWorld::New().address;  // host memory (E5 step 4)
  schema::CarPhysicsGlobals::set_collision_world(uint32_t(world_block));
  schema::CollisionWorld(world_block).set_buffer(uint32_t(0));
  schema::CollisionWorld(World()).set_updating(uint32_t(0));
  schema::CollisionWorld(World()).set_query_count(uint32_t(32));
  schema::CollisionWorld(World()).set_object_count(uint32_t(objects));
  // The objects, with operator new[]'s count header (host memory, E5 step 4).
  {
    const uint32_t n = schema::CollisionWorld(World()).object_count();
    const uint32_t block = schema::CollisionObjectBlock::New().address;
    uint32_t first = 0;
    if (block != 0) {
      first = block + 16;
      schema::CollisionObjectBlock(block).set_count(n);
      for (uint32_t i = 0; i < n; i++) {
        CollisionObjectConstruct(first + 320 * i);
      }
    }
    schema::CollisionWorld(World()).set_objects(uint32_t(first));
  }
  // The free list of all of them, as CollisionWorld::Reset makes it.
  uint32_t world = World();
  schema::CollisionWorld(world).set_active(uint32_t(0));
  schema::CollisionWorld(world).set_free(uint32_t(schema::CollisionWorld(world).objects().address));
  world = World();
  uint32_t count = schema::CollisionWorld(world).object_count();
  for (uint32_t i = 0; i < count; i++) {
    if (i < count - 1) {
      const uint32_t object = schema::CollisionWorld(world).objects().address + 320 * i;
      schema::CollisionWorldObject(object).set_next(uint32_t(object + 320));
    } else {
      schema::CollisionWorldObject(count * 320 + schema::CollisionWorld(world).objects().address - 320).set_next(uint32_t(0));
    }
    world = World();
    const uint32_t object = schema::CollisionWorld(world).objects().address + 320 * i;
    schema::CollisionWorldObject(object).set_previous(uint32_t(int32_t(i) > 0 ? object - 320 : 0));
    world = World();
    count = schema::CollisionWorld(world).object_count();
  }
  schema::CollisionWorld(world).set_active_count(uint32_t(0));
  schema::CollisionWorld(World()).set_pair_capacity(uint32_t(pairs));
  schema::CollisionWorld(World()).set_pairs(uint32_t(NewArray(PairsBytes(schema::CollisionWorld(World()).pair_capacity()))));
  schema::CollisionWorld(World()).set_pair_count(uint32_t(0));
  // The query records: 2 x +60 (host memory, E5 step 4).
  {
    const uint32_t n = schema::CollisionWorld(World()).query_count() << 1;
    const uint32_t block = schema::CollisionHit::NewArray(n).address;
    if (block != 0) {
      ZeroRecordWs(block, n, 12);
    }
    schema::CollisionWorld(World()).set_hits(uint32_t(block));
  }
  world = World();
  schema::CollisionWorld(world).set_merged_hits(uint32_t(schema::CollisionWorld(world).query_count() * 80 + schema::CollisionWorld(world).hits().address));
  schema::CollisionWorld(World()).set_contact_capacity(uint32_t(contacts));
  {
    const uint32_t n = schema::CollisionWorld(World()).contact_capacity();
    const uint32_t block = schema::CollisionContact::NewArray(n).address;  // host memory (E5 step 4)
    if (block != 0) {
      ZeroRecordWs(block, n, 28);
    }
    schema::CollisionWorld(World()).set_contacts(uint32_t(block));
  }
  schema::CollisionWorld(World()).set_contacts_used(uint32_t(0));
  schema::CollisionWorld(World()).set_default_step_bits(guest::LoadU32(0x820029EC));  // 1/30
  CollisionWorldResetSurfaces();
  schema::CarPhysicsGlobals::set_query_walls(uint32_t(0x825C8388));
  schema::CarPhysicsGlobals::set_query_ground(uint32_t(0x820BC828));
  schema::CollisionWorld(World()).set_ground_callback(uint32_t(0x820BC4D0));
  *r3 = 0x826C2E30;  // ResetSurfaces'
  return true;
}

bool CollisionWorldSetQueryCapacity(uint32_t n, uint32_t* r3) {
  const uint32_t world = World();
  if (schema::CollisionWorld(world).buffer() != 0 || !collision::Deletable(schema::CollisionWorld(world).hits().address) ||
      !ArrayFits(RecordsBytes(n << 1))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B9198 CollisionWorld::SetQueryCapacity");
  schema::CollisionWorld(World()).set_query_count(uint32_t(n));
  schema::CollisionWorld(World()).hits().Delete();  // host memory (E5 step 4)
  const uint32_t count = schema::CollisionWorld(World()).query_count() << 1;
  const uint32_t block = schema::CollisionHit::NewArray(count).address;
  if (block != 0) {
    ZeroRecordWs(block, count, 12);
  }
  schema::CollisionWorld(World()).set_hits(uint32_t(block));
  const uint32_t w = World();
  schema::CollisionWorld(w).set_merged_hits(uint32_t(schema::CollisionWorld(w).query_count() * 80 + schema::CollisionWorld(w).hits().address));
  *r3 = block;
  return true;
}

bool CollisionWorldSetup(uint32_t* r3) {
  uint32_t ignored = 0;
  if (!CollisionWorldCreate(50, 1000, 100, &ignored)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820FF250 CollisionWorld::Setup");
  va::CollisionSetGlobal78(0x820FF1B0);
  va::CollisionSetGlobal7C(0x820FEBD0);
  va::CollisionWorldSetCallback(0x820FEAB0);
  // Create's query block is the layer's: SetQueryCapacity can't refuse.
  CollisionWorldSetQueryCapacity(32, r3);
  const double zero = Lfs(kZero), one = Lfs(kOne);
  CollisionSurfaceSetC(0x02000000, zero);
  CollisionSurfaceSetC(0x01000000, zero);
  CollisionSurfaceSetC(0x04000000, zero);
  CollisionSurfaceSetD(0x02000000, zero);
  CollisionSurfaceSetD(0x01000000, one);
  CollisionSurfaceSetD(0x04000000, one);
  CollisionSurfaceSetA(0x02000000, zero);
  CollisionSurfaceSetA(0x01000000, Lfs(0x82001BE4));
  CollisionSurfaceSetA(0x04000000, Lfs(0x82002968));
  CollisionSurfaceSetB(0x02000000, zero);
  CollisionSurfaceSetB(0x01000000, zero);
  CollisionSurfaceSetB(0x04000000, zero);
  return true;
}

// --- The collision world's update ---

namespace {

constexpr uint32_t kWallHits = 0x820FF1B0;     // TrackCollision::WallHits
constexpr uint32_t kGroundHits = 0x820FEBD0;   // TrackCollision::GroundHits
constexpr uint32_t kRespond = 0x820B81C8;      // CollisionObject::Respond
constexpr uint32_t kSolve = 0x820BBE30;        // CollisionPair::Solve
constexpr uint32_t kPlace = 0x820BB758;        // CollisionShape::Place
constexpr uint32_t kClockWrap = 0x82002AA0;    // the clock's wrap
constexpr uint32_t kHalf = 0x82001BE4;         // 0.5
constexpr uint32_t kSlowSpin = 0x820021F8;     // Respond's skip case's bound
constexpr uint32_t kRestitutionBias = 0x82001FBC;
constexpr uint32_t kTorqueBase = 0x82002A98;   // a double: pow's base

// pow's own stores into its caller's linkage area (x at r1 + 16, y at
// + 24, stfd), which c_runtime::Pow doesn't make; then pow.
double PowFromFrame(uint32_t frame, double x, double y) {
  const uint64_t xb = std::bit_cast<uint64_t>(x), yb = std::bit_cast<uint64_t>(y);
  guest::StoreU32(frame + 24, uint32_t(yb >> 32));
  guest::StoreU32(frame + 28, uint32_t(yb));
  guest::StoreU32(frame + 16, uint32_t(xb >> 32));
  guest::StoreU32(frame + 20, uint32_t(xb));
  return c_runtime::Pow(x, y);
}

// A query through its pointer: WallHits and GroundHits natively, anything
// else (and their refusals) by the guest call, from Collide's frame.
uint32_t Query(uint32_t target, uint32_t hits, uint32_t shape, uint32_t groups,
               uint32_t frame, uint32_t lr, const CollisionGuestCall& guest) {
  uint32_t count = 0;
  if (target == kWallHits &&
      track_collision::TrackCollisionWallHits(hits, shape, frame - 1584, &count)) {
    return count;
  }
  if (target == kGroundHits &&
      track_collision::TrackCollisionGroundHits(hits, shape, frame - 384, &count)) {
    return count;
  }
  return guest(target, hits, shape, groups, 0, frame, lr);
}

}  // namespace

bool CollisionObjectRespond(uint32_t object, uint32_t hits, uint32_t count,
                            uint32_t flag, uint32_t frame, uint32_t* r3) {
  // CollisionObject::Translate runs natively only on a box (the record's
  // current copy): else the original from the start.
  if (!collision::IsBox(schema::CollisionShape(schema::CollisionWorldObject(object).shape_record_address()).current().address)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B81C8 CollisionObject::Respond");
  const uint32_t f = frame;
  const double zero = Lfs(kZero);  // f27
  Stfs(f + 140, zero);
  Stfs(f + 156, zero);
  Stfs(f + 172, zero);
  const uint32_t merged = va::CollisionWorldWord68();  // r27
  const uint32_t left = CollisionRecordsMerge(merged, f + 128, hits, count, f - 304);
  const double half = Lfs(kHalf);  // f23
  bool skip = false;                     // r25
  if (left != 0) {
    const uint32_t bits = guest::LoadU32(f + 184) & 0x07000000u;
    const uint32_t normal = va::RigidBodyField144(schema::CollisionWorldObject(object).body().address);
    if (guest::LoadU32(flag) != 0) {
      const double facing = Fabs(double(vl::Vec3DotOf(f + 144, normal)));  // f31
      if (guest::LoadU32(flag) != 0) {
        const uint32_t body = schema::CollisionWorldObject(object).body().address;
        const uint32_t word = schema::RigidBody(body).flags();
        if ((word & 1) != 0 && ((word >> 1) & 1) != 0) {
          const double spin = Fabs(Lfs(va::RigidBodyField160(body)));
          if (spin < Lfs(kSlowSpin) && facing > half) {
            skip = true;
          }
        }
      }
    }
    if ((schema::CollisionWorldObject(object).push_out_bits() & bits) != 0) {
      vl::Vec4Scaled(f + 112, f + 160, Lfs(f + 176));
      CollisionObjectTranslate(object, f + 112, f - 128);
    }
  }
  uint32_t r = va::RigidBodyField160(schema::CollisionWorldObject(object).body().address);
  const double spin_x = Lfs(r);  // f22
  if (left == 0) {
    *r3 = r;
    return true;
  }
  const uint64_t base_bits = (uint64_t(guest::LoadU32(kTorqueBase)) << 32) |
                             guest::LoadU32(kTorqueBase + 4);
  const double torque_base = std::bit_cast<double>(base_bits);  // f24
  const double one = Lfs(kOne);                           // f28
  const double bias = Lfs(kRestitutionBias);              // f25
  uint32_t n = left;
  uint32_t hit = merged;
  do {
    const uint32_t w = schema::CollisionHit(hit).surface_bits();
    Stfs(f + 80, zero);
    const uint32_t mask = schema::CollisionWorldObject(object).response_bits();
    Stfs(f + 84, zero);
    const uint32_t bits = w & 0x07000000u;  // r31
    Stfs(f + 88, zero);
    Stfs(f + 92, zero);
    Stfs(f + 108, zero);
    if ((mask & bits) == 0 || skip) {
      continue;
    }
    if ((bits & 0x01000000u) != 0) {
      r = va::ContactPosition(schema::CollisionWorldObject(object).body().address);
      CopyWord(r + 4, schema::CollisionHit(hit).point_address() + 4);
    }
    const uint32_t point = hit, normal = schema::CollisionHit(hit).push_address();  // r27, r28
    const double v = RigidBodyContactImpulse(schema::CollisionWorldObject(object).body().address, point,
                                             normal, f - 208, &r);  // f26
    if (!(v > zero)) {
      continue;
    }
    double e = Fmuls(Fadds(CollisionSurfaceOfC(bits), bias), half);
    if (e < zero) {
      e = zero;
    } else if (e > one) {
      e = one;
    }
    const double impulse = Fmuls(Fadds(e, one), v);  // f30
    vl::Vec4Scaled(f + 208, normal, impulse);
    vl::Vec4Copy(f + 96, f + 208);
    double friction = double(va::Object344Float28(schema::CollisionWorldObject(object).shape()));
    friction = Fadds(CollisionSurfaceOfA(bits), friction);  // f31
    if (schema::CollisionWorldObject(object).box_friction() != 0) {
      friction = double(va::Object344Float28(schema::CollisionWorldObject(object).shape()));
    }
    const double power = PowFromFrame(f, torque_base, spin_x);  // f29
    if ((bits & 0x04000000u) == 0) {
      const double d = CollisionSurfaceOfD(bits);
      vl::Vec4Scaled(f + 256, f + 96, Fmuls(d, double(float(power))));
      vl::Vec4Copy(f + 80, f + 256);
    }
    if (friction > zero) {
      RigidBodyFrictionImpulse(f + 112, schema::CollisionWorldObject(object).body().address, point, normal,
                               impulse, friction, f - 192);
      const double scale = CollisionSurfaceOfB(bits);
      if ((bits & 0x02000000u) != 0) {
        vl::Vec3Sub(f + 96, f + 96, f + 112);
        vl::Vec4Scaled(f + 240, f + 112, scale);
        vl::Vec3Sub(f + 80, f + 80, f + 240);
      } else {
        vl::Vec3AddTo(f + 96, f + 112);
        vl::Vec4Scaled(f + 224, f + 112, scale);
        vl::Vec3AddTo(f + 80, f + 224);
      }
    }
    va::RigidBodyAddForce(schema::CollisionWorldObject(object).body().address, f + 96);
    va::RigidBodyAddTorqueAt(schema::CollisionWorldObject(object).body().address, f + 80, point);
    RigidBodyApplyImpulses(schema::CollisionWorldObject(object).body().address, one, f - 144);
    const uint32_t contact = va::CollisionWorldNewContact();
    r = contact;
    if (contact == 0) {
      continue;
    }
    schema::CollisionContact(contact).set_next(uint32_t(schema::CollisionWorldObject(object).first_contact().address));
    schema::CollisionWorldObject(object).set_first_contact(uint32_t(contact));
    schema::CollisionContact(contact).set_other(uint32_t(schema::CollisionHit(hit).surface_bits() & 0x80FFFFFFu));
    const uint32_t flags = schema::CollisionHit(hit).surface_bits();
    schema::CollisionContact(contact).set_strength(float(v));
    schema::CollisionContact(contact).set_flags(uint32_t(flags));
    vl::Vec4Copy(schema::CollisionContact(contact).position_address(), point);
    vl::Vec4Copy(schema::CollisionContact(contact).normal_address(), schema::CollisionHit(hit).normal_address());
    vl::Vec4Copy(schema::CollisionContact(contact).push_direction_address(), normal);
    r = schema::CollisionContact(contact).push_direction_address();
  } while (hit += 80, --n != 0);
  *r3 = r;
  return true;
}

uint32_t CollisionObjectCollide(uint32_t object, uint32_t frame,
                                const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B8548 CollisionObject::Collide");
  const uint32_t f = frame;
  const uint32_t hits = va::CollisionWorldWord64();  // r30
  const uint32_t record = schema::CollisionWorldObject(object).shape_record_address();                  // r29
  auto respond = [&](uint32_t count, uint32_t lr) {
    uint32_t r3 = 0;
    if (!CollisionObjectRespond(object, hits, count, f + 80, f - 416, &r3)) {
      r3 = guest(kRespond, object, hits, count, f + 80, f, lr);
    }
    return r3;
  };
  uint32_t count = Query(schema::CarPhysicsGlobals::query_walls(), hits, record,
                         schema::CollisionWorldObject(object).collides_with(), f, 0x820B857C, guest);
  if (count != 0) {
    guest::StoreU32(f + 80, 1);
    respond(count, 0x820B859C);
  }
  const uint32_t copy = collision::WordAt8(record);
  count = Query(schema::CarPhysicsGlobals::query_ground(), hits, copy,
                schema::CollisionWorldObject(object).collides_with(), f, 0x820B85C0, guest);
  if (count == 0) {
    return 0;
  }
  guest::StoreU32(f + 80, 0);
  return respond(count, 0x820B85E0);
}

uint32_t CollisionWorldCollideAll(uint32_t frame, uint32_t r3,
                                  const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B89F0 CollisionWorld::CollideAll");
  for (uint32_t o = schema::CollisionWorld(World()).active().address; o != 0;
       o = schema::CollisionWorldObject(o).next().address) {
    if (schema::CollisionWorldObject(o).no_queries() == 0) {
      r3 = CollisionObjectCollide(o, frame - 128, guest);
    }
  }
  return r3;
}

uint32_t CollisionWorldSolvePairs(double dt, uint32_t frame, uint32_t r3,
                                  const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B87B0 CollisionWorld::SolvePairs");
  const uint32_t f = frame;
  Stfs(schema::CarPhysicsGlobals::pair_clock_address(), Fadds(Lfs(schema::CarPhysicsGlobals::pair_clock_address()), dt));
  const double wrap = Lfs(kClockWrap);
  const double clock = Lfs(schema::CarPhysicsGlobals::pair_clock_address());
  uint32_t world = World();
  Stfs(schema::CarPhysicsGlobals::last_step_address(), dt);
  if (clock > wrap) {
    Stfs(schema::CarPhysicsGlobals::pair_clock_address(), Fsubs(clock, wrap));
    uint32_t n = schema::CollisionWorld(world).pair_count();
    const uint32_t pairs = schema::CollisionWorld(world).pairs().address;
    if (n != 0) {
      uint32_t at = pairs + 28;
      do {
        Stfs(at, Fsubs(Lfs(at), wrap));
        at += 32;
      } while (--n != 0);
      world = World();
    }
  }
  const double zero = Lfs(kZero);  // f31
  for (uint32_t pass = 0; pass < 2; pass++) {
    uint32_t o = schema::CollisionWorld(world).active().address;
    if (o != 0) {
      for (; o != 0; o = schema::CollisionWorldObject(o).next().address) {
        vl::Vec4Copy(schema::CollisionWorldObject(o).push_out_address(), ops::kZeroVector);
        r3 = schema::CollisionWorldObject(o).push_out_address();
      }
      world = World();
    }
    uint32_t n = schema::CollisionWorld(world).pair_count();
    uint32_t moved = 0;  // r29: the pairs that hit
    uint32_t pair = schema::CollisionWorld(world).pairs().address;
    if (n == 0) {
      return r3;
    }
    do {
      const bool due = schema::CollisionPair(pair).a().moved() != 0 ||
                       schema::CollisionPair(pair).b().moved() != 0 ||
                       !(double(schema::CollisionPair(pair).next_check()) > Lfs(schema::CarPhysicsGlobals::pair_clock_address()));
      if (!due) {
        continue;
      }
      const double next = collision::CollisionPairNextCheckTime(pair, f - 160, &r3);
      schema::CollisionPair(pair).set_next_check(float(next));
      if (!(next < Fadds(Lfs(schema::CarPhysicsGlobals::pair_clock_address()), Lfs(schema::CarPhysicsGlobals::last_step_address())))) {
        continue;
      }
      Stfs(f + 92, zero);
      Stfs(f + 108, zero);
      vl::Vec4Copy(f + 80,
                   va::ContactPosition(schema::CollisionPair(pair).a().body().address));
      vl::Vec4Copy(f + 96,
                   va::ContactPosition(schema::CollisionPair(pair).b().body().address));
      uint32_t hit = 0;
      if (!collision::CollisionPairSolve(pair, f - 1632, &hit)) {
        hit = guest(kSolve, pair, 0, 0, 0, f, 0x820B890C);
      }
      r3 = hit;
      if (hit == 0) {
        continue;
      }
      moved++;
      r3 = CollisionObjectCollide(schema::CollisionPair(pair).a().address, f - 128, guest);
      r3 = CollisionObjectCollide(schema::CollisionPair(pair).b().address, f - 128, guest);
      RigidBodySetCenterOfMassPosition(schema::CollisionPair(pair).a().body().address, f + 80, f - 96);
      const uint32_t body_b = schema::CollisionPair(pair).b().body().address;
      RigidBodySetCenterOfMassPosition(body_b, f + 96, f - 96);
      r3 = schema::RigidBody(body_b).matrix_address() + 48;
    } while (pair += 32, --n != 0);
    if (moved == 0) {
      return r3;
    }
    world = World();
    o = schema::CollisionWorld(world).active().address;
    if (o != 0) {
      for (; o != 0; o = schema::CollisionWorldObject(o).next().address) {
        const uint32_t push = schema::CollisionWorldObject(o).push_out_address();  // r30
        const double sx = Lfs(schema::CollisionWorldObject(o).push_scale_address());
        const double py = Lfs(schema::CollisionWorldObject(o).push_out_address() + 4);
        const double sy = Lfs(schema::CollisionWorldObject(o).push_scale_address() + 4);
        const double sz = Lfs(schema::CollisionWorldObject(o).push_scale_address() + 8);
        Stfs(schema::CollisionWorldObject(o).push_out_address() + 4, Fmuls(sy, py));
        const double x = Fmuls(sx, Lfs(push));
        const double z = Fmuls(sz, Lfs(schema::CollisionWorldObject(o).push_out_address() + 8));
        Stfs(push, x);
        Stfs(schema::CollisionWorldObject(o).push_out_address() + 8, z);
        const uint32_t body = schema::CollisionWorldObject(o).body().address;
        vl::Vec3Sum(f + 112, va::ContactPosition(body), push);
        RigidBodySetCenterOfMassPosition(schema::CollisionWorldObject(o).body().address, f + 112, f - 96);
        r3 = schema::CollisionWorldObject(o).body().address + 336;
      }
      world = World();
    }
  }
  return r3;
}

uint32_t CollisionWorldUpdate(double dt, uint32_t frame, uint32_t r3,
                              const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B92C8 CollisionWorld::Update");
  const uint32_t f = frame;
  schema::CollisionWorld(World()).set_contacts_used(uint32_t(0));
  schema::CollisionWorld(World()).set_updating(uint32_t(1));
  for (uint32_t o = schema::CollisionWorld(World()).active().address; o != 0;
       o = schema::CollisionWorldObject(o).next().address) {
    if (schema::CollisionWorldObject(o).unmoved() == 0) {
      const uint32_t matrix = va::RigidBodyMatrix(schema::CollisionWorldObject(o).body().address);
      if (collision::CollisionShapePlace(schema::CollisionWorldObject(o).shape_record_address(), matrix, f - 608)) {
        r3 = schema::CollisionWorldObject(o).shape_record_address();
      } else {
        r3 = guest(kPlace, schema::CollisionWorldObject(o).shape_record_address(), matrix, 0, 0, f, 0x820B9324);
      }
    }
    schema::CollisionWorldObject(o).set_first_contact(uint32_t(0));
  }
  // fcmpu then beq: a NaN step isn't equal to 0.0.
  if (!(dt == Lfs(kZero))) {
    r3 = CollisionWorldSolvePairs(dt, f - 208, r3, guest);
  }
  r3 = CollisionWorldCollideAll(f - 96, r3, guest);
  uint32_t world = World();
  uint32_t o = schema::CollisionWorld(world).active().address;
  if (o != 0) {
    const double one = Lfs(kOne);  // f31
    for (; o != 0; o = schema::CollisionWorldObject(o).next().address) {
      if (schema::CollisionWorldObject(o).first_contact().address == 0 || schema::CollisionWorldObject(o).push_count() == 0) {
        continue;
      }
      va::RigidBodyAddForce(schema::CollisionWorldObject(o).body().address, schema::CollisionWorldObject(o).push_address());
      // The count through the stack (std, lfd, fcfid, frsp).
      const uint32_t pushes = schema::CollisionWorldObject(o).push_count();
      guest::StoreU32(f + 80, 0);
      guest::StoreU32(f + 84, pushes);
      const double share = Fdivs(one, double(float(double(int64_t(pushes)))));
      vl::Vec4Scaled(f + 96, schema::CollisionWorldObject(o).pair_torque_address(), share);
      vl::Vec4Scaled(f + 112, f + 96, Lfs(schema::CollisionWorldObject(o).torque_scale_address()));
      va::RigidBodyAddTorque(schema::CollisionWorldObject(o).body().address, f + 112);
      r3 = RigidBodyApplyImpulses(schema::CollisionWorldObject(o).body().address, one, f - 144);
    }
    world = World();
  }
  o = schema::CollisionWorld(world).active().address;
  if (o != 0) {
    for (; o != 0; o = schema::CollisionWorldObject(o).next().address) {
      const uint32_t pushes = schema::CollisionWorldObject(o).push_count();
      schema::CollisionWorldObject(o).set_moved(uint32_t(0));
      if (pushes != 0) {
        vl::Vec4Copy(schema::CollisionWorldObject(o).push_address(), ops::kZeroVector);
        vl::Vec4Copy(schema::CollisionWorldObject(o).pair_torque_address(), ops::kZeroVector);
        schema::CollisionWorldObject(o).set_push_count(uint32_t(0));
        r3 = schema::CollisionWorldObject(o).pair_torque_address();
      }
    }
    world = World();
  }
  schema::CollisionWorld(world).set_updating(uint32_t(0));
  return r3;
}

}  // namespace car_physics
