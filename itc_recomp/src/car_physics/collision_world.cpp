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

double SurfaceGet(uint8_t* base, const SurfaceGroup& g, uint32_t flags, uint32_t fallback) {
  if ((flags & 0x01000000) != 0) {
    return Lfs(base, g.bit24);
  }
  if ((flags & 0x02000000) != 0) {
    return Lfs(base, g.bit25);
  }
  if ((flags & 0x04000000) != 0) {
    return Lfs(base, g.bit26);
  }
  return Lfs(base, fallback);
}

void SurfaceSet(uint8_t* base, const SurfaceGroup& g, uint32_t bits, double value) {
  if ((bits & 0x07000000) != 0) {
    Stfs(base, g.fallback, value);
  }
  if ((bits & 0x01000000) != 0) {
    Stfs(base, g.bit24, value);
  }
  if ((bits & 0x02000000) != 0) {
    Stfs(base, g.bit25, value);
  }
  if ((bits & 0x04000000) != 0) {
    Stfs(base, g.bit26, value);
  }
}

// An integer's float through the stack (extsw, std at `at`, lfd, fcfid,
// frsp).
double StackIntegerFloat(uint8_t* base, uint32_t at, int64_t value) {
  guest::StoreU32(base, at, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(base, at + 4, uint32_t(value));
  return double(float(double(value)));
}

// A record's push (+32) normalized, its length into +48 (0.0 for a zero
// vector, left as it is), as the merge does it.
void NormalizePush(uint8_t* base, uint32_t record) {
  const schema::CollisionHit hit(base, record);
  const double squared = double(vl::Vec3Dot(base, hit.push_address(), hit.push_address()));
  if (squared == Lfs(base, kZero)) {
    CopyWord(base, kZero, hit.depth_address());
    return;
  }
  const float length = vl::Sqrtf(squared);
  hit.set_depth(length);
  vl::Vec4DivideBy(base, hit.push_address(), double(length));
}

}  // namespace

double CollisionSurfaceA(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC1F8 CollisionSurface::A");
  return SurfaceGet(base, kGroupA, flags, kGroupA.fallback);
}
double CollisionSurfaceB(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC240 CollisionSurface::B");
  return SurfaceGet(base, kGroupB, flags, kGroupB.fallback);
}
double CollisionSurfaceC(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC288 CollisionSurface::C");
  return SurfaceGet(base, kGroupC, flags, kGroupC.fallback);
}
double CollisionSurfaceD(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC2D0 CollisionSurface::D");
  // Group B's default, not D's (the game's).
  return SurfaceGet(base, kGroupD, flags, kGroupB.fallback);
}
double CollisionSurfaceOfA(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC540 CollisionSurface::OfA");
  return CollisionSurfaceA(base, flags);
}
double CollisionSurfaceOfB(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC548 CollisionSurface::OfB");
  return CollisionSurfaceB(base, flags);
}
double CollisionSurfaceOfC(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC530 CollisionSurface::OfC");
  return CollisionSurfaceC(base, flags);
}
double CollisionSurfaceOfD(uint8_t* base, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820BC538 CollisionSurface::OfD");
  return CollisionSurfaceD(base, flags);
}

void CollisionSurfaceSetA(uint8_t* base, uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC318 CollisionSurface::SetA");
  SurfaceSet(base, kGroupA, bits, value);
}
void CollisionSurfaceSetB(uint8_t* base, uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC360 CollisionSurface::SetB");
  SurfaceSet(base, kGroupB, bits, value);
}
void CollisionSurfaceSetC(uint8_t* base, uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC3A8 CollisionSurface::SetC");
  SurfaceSet(base, kGroupC, bits, value);
}
void CollisionSurfaceSetD(uint8_t* base, uint32_t bits, double value) {
  ITC_NATIVE_RUNNING("sub_820BC3F0 CollisionSurface::SetD");
  SurfaceSet(base, kGroupD, bits, value);
}

void CollisionSurfacesReset(uint8_t* base) {
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
    CopyWord(base, s.one ? kOne : kHalf, s.at);
  }
}

void CollisionWorldResetSurfaces(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820BC550 CollisionWorld::ResetSurfaces");
  CollisionSurfacesReset(base);
}

void CollisionShapeInit(uint8_t* base, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_820BAA78 CollisionShape::Init");
  const schema::CollisionShape record(base, shape);
  CopyWord(base, kZero, record.bounds_min_w_address());
  record.set_previous(0u);
  CopyWord(base, kZero, record.bounds_max_w_address());
  record.set_current(0u);
  record.set_template_shape(0u);
}

void CollisionObjectConstruct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7AE8 CollisionObject::CollisionObject");
  schema::CollisionWorldObject(base, object).set_vtable(uint32_t(0x82002A94));
  CollisionShapeInit(base, schema::CollisionWorldObject(base, object).shape_record_address());
  const schema::CollisionWorldObject obj(base, object);
  CopyWord(base, kZero, obj.push_address() + 12);
  CopyWord(base, kZero, obj.pair_torque_address() + 12);
  CopyWord(base, kZero, obj.push_scale_address() + 12);
  CopyWord(base, kZero, obj.push_out_address() + 12);
}

uint32_t CollisionObjectAccumulateForce(uint8_t* base, uint32_t object, uint32_t point,
                                        uint32_t force, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7DF8 CollisionObject::AccumulateForce");
  const uint32_t f = frame;
  CopyWord(base, kZero, f + 92);
  const uint32_t center = va::ContactPosition(schema::CollisionWorldObject(base, object).body().address);
  vl::Vec3Difference(base, f + 96, point, center);
  vl::Vec3Cross(base, f + 112, f + 96, force);
  vl::Vec4Copy(base, f + 80, f + 112);
  vl::Vec3AddTo(base, schema::CollisionWorldObject(base, object).pair_torque_address(), f + 80);
  // The largest push per axis (+160 to +168).
  for (uint32_t i = 0; i < 12; i += 4) {
    if (Fabs(Lfs(base, force + i)) > Fabs(Lfs(base, schema::CollisionWorldObject(base, object).push_address() + i))) {
      CopyWord(base, force + i, schema::CollisionWorldObject(base, object).push_address() + i);
    }
  }
  schema::CollisionWorldObject(base, object).set_unmoved(uint32_t(0));
  schema::CollisionWorldObject(base, object).set_push_count(uint32_t(schema::CollisionWorldObject(base, object).push_count() + 1));
  return schema::CollisionWorldObject(base, object).pair_torque_address();
}

uint32_t CollisionObjectAddContact(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7C50 CollisionObject::AddContact");
  const uint32_t contact = va::CollisionWorldNewContact(base);
  if (contact != 0) {
    schema::CollisionContact(base, contact).set_next(uint32_t(schema::CollisionWorldObject(base, object).first_contact().address));
    schema::CollisionWorldObject(base, object).set_first_contact(uint32_t(contact));
  }
  return contact;
}

bool CollisionObjectTranslate(uint8_t* base, uint32_t object, uint32_t offset, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7B60 CollisionObject::Translate");
  // CollisionShape::Translate runs natively only on a box (its current
  // copy, the record's +8): else the original from the start.
  if (!collision::IsBox(base, schema::CollisionShape(base, schema::CollisionWorldObject(base, object).shape_record_address()).current().address)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t body = schema::CollisionWorldObject(base, object).body().address;
  // The body's center (+16, its 16 bytes by ld and std) moved by the offset.
  const uint32_t center = va::ContactPosition(body);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 80 + i, guest::LoadU32(base, center + i));
  }
  vl::Vec3AddTo(base, f + 80, offset);
  RigidBodySetCenterOfMassPosition(base, schema::CollisionWorldObject(base, object).body().address, f + 80, f - 96);
  collision::CollisionShapeTranslate(base, schema::CollisionWorldObject(base, object).shape_record_address(), offset, f - 192);
  vl::Vec3AddTo(base, schema::CollisionWorldObject(base, object).push_out_address(), offset);
  schema::CollisionWorldObject(base, object).set_unmoved(uint32_t(0));
  return true;
}

uint32_t CollisionRecordsMerge(uint8_t* base, uint32_t out, uint32_t summary, uint32_t hits,
                               uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7ED0 CollisionRecords::Merge");
  const uint32_t f = frame;
  for (uint32_t i = 0; i < count; i++) {
    va::CollisionRecordCopy(base, out + 80 * i, hits + 80 * i);
  }
  uint32_t kept = 0;
  if (count == 0) {
    va::CollisionRecordCopy(base, summary, out);
    return kept;
  }
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t record = out + 80 * i;
    if (int32_t(schema::CollisionHit(base, record).key()) == -1) {
      continue;
    }
    // The later records with the same id (+60) merged into this one: their
    // points and normals summed, their pushes (+32 x +48) too.
    int32_t merged = 0;
    for (uint32_t j = i + 1; j < count; j++) {
      const uint32_t other = out + 80 * j;
      if (schema::CollisionHit(base, record).key() != schema::CollisionHit(base, other).key()) {
        continue;
      }
      vl::Vec3AddTo(base, record, other);
      vl::Vec3AddTo(base, schema::CollisionHit(base, record).normal_address(), schema::CollisionHit(base, other).normal_address());
      const double strength = double(schema::CollisionHit(base, other).depth());
      if (merged > 0) {
        vl::Vec4Scaled(base, f + 96, schema::CollisionHit(base, other).push_address(), strength);
        vl::Vec3AddTo(base, schema::CollisionHit(base, record).push_address(), f + 96);
      } else {
        vl::Vec4Scaled(base, f + 112, schema::CollisionHit(base, other).push_address(), strength);
        vl::Vec4Scaled(base, f + 128, schema::CollisionHit(base, record).push_address(), double(schema::CollisionHit(base, record).depth()));
        vl::Vec3Sum(base, f + 144, f + 128, f + 112);
        vl::Vec4Copy(base, schema::CollisionHit(base, record).push_address(), f + 144);
      }
      schema::CollisionHit(base, other).set_key(uint32_t(uint32_t(-1)));
      merged++;
    }
    if (merged != 0) {
      const double scale = Fdivs(one, StackIntegerFloat(base, f + 80, merged + 1));
      vl::Vec4ScaleBy(base, record, scale);
      vl::Vec4ScaleBy(base, schema::CollisionHit(base, record).normal_address(), scale);
      vl::Vec4ScaleBy(base, schema::CollisionHit(base, record).push_address(), scale);
      NormalizePush(base, record);
    }
    if (int32_t(i) != int32_t(kept)) {
      va::CollisionRecordCopy(base, out + 80 * kept, record);
    }
    kept++;
  }
  if (!(int32_t(kept) > 1)) {
    va::CollisionRecordCopy(base, summary, out);
    return kept;
  }
  // The summary: the average of the records with surface bits.
  for (uint32_t i = 0; i < 80; i += 4) {
    guest::StoreU32(base, summary + i, 0);
  }
  for (uint32_t k = 0; k < kept; k++) {
    const uint32_t record = out + 80 * k;
    if ((schema::CollisionHit(base, record).surface_bits() & 0x07000000) == 0) {
      continue;
    }
    vl::Vec3AddTo(base, summary, record);
    vl::Vec3AddTo(base, schema::CollisionHit(base, summary).normal_address(), schema::CollisionHit(base, record).normal_address());
    vl::Vec4Scaled(base, f + 144, schema::CollisionHit(base, record).push_address(), double(schema::CollisionHit(base, record).depth()));
    vl::Vec3AddTo(base, schema::CollisionHit(base, summary).push_address(), f + 144);
    schema::CollisionHit(base, summary).set_surface_bits(uint32_t(schema::CollisionHit(base, record).surface_bits()));
    schema::CollisionHit(base, summary).set_key(uint32_t(schema::CollisionHit(base, record).key()));
  }
  vl::Vec4ScaleBy(base, summary, Fdivs(one, StackIntegerFloat(base, f + 80, int32_t(kept))));
  vl::Vec3NormalizeSelf(base, schema::CollisionHit(base, summary).normal_address());
  NormalizePush(base, summary);
  return kept;
}


// --- The world's pairs and its reset ---

namespace {

uint32_t World(uint8_t* base) {
  return schema::CarPhysicsGlobals::collision_world(base).address;
}

// The pair's last in an object's list (+0/+8 on the side where the pair's
// +16 is the object, +4/+12 on the other), from the head +264; 0 for none.
uint32_t ListTail(uint8_t* base, uint32_t object) {
  uint32_t pair = va::CollisionObjectWord264(base, object);
  uint32_t last = 0;
  while (pair != 0) {
    last = pair;
    pair = schema::CollisionPair(base, pair).a().address == object ? schema::CollisionPair(base, pair).a_next().address
                                                     : schema::CollisionPair(base, pair).b_next().address;
  }
  return last;
}

// A neighbor's link toward the pair repointed: its next (+8, or +12 on its
// B side) when `next`, else its previous (+0, or +4), by whether its +16 is
// `object`.
void Repoint(uint8_t* base, uint32_t neighbor, uint32_t object, bool next, uint32_t to) {
  const bool a_side = schema::CollisionPair(base, neighbor).a().address == object;
  guest::StoreU32(base, schema::CollisionPair(base, neighbor).a_previous_address() + (next ? 8 : 0) + (a_side ? 0 : 4), to);
}

}  // namespace

bool CollisionWorldHasPair(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820B8C20 CollisionWorld::HasPair");
  const uint32_t world = World(base);
  const uint32_t count = schema::CollisionWorld(base, world).pair_count();
  if (count == 0) {
    return false;
  }
  uint32_t pair = schema::CollisionWorld(base, world).pairs().address;
  for (uint32_t i = 0; i < count; i++, pair += 32) {
    const uint32_t first = schema::CollisionPair(base, pair).a().address;
    if (first == a && schema::CollisionPair(base, pair).b().address == b) {
      return true;
    }
    if (first == b && schema::CollisionPair(base, pair).b().address == a) {
      return true;
    }
  }
  return false;
}

uint32_t CollisionWorldLinkPair(uint8_t* base, uint32_t pair, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BC068 CollisionWorld::LinkPair");
  CopyWord(base, kZero, schema::CollisionPair(base, pair).next_check_address());
  schema::CollisionPair(base, pair).set_b(uint32_t(b));
  schema::CollisionPair(base, pair).set_a(uint32_t(a));
  schema::CollisionPair(base, pair).set_a_next(uint32_t(0));
  schema::CollisionPair(base, pair).set_b_next(uint32_t(0));
  schema::CollisionPair(base, pair).set_separated_face(uint32_t(0));
  const uint32_t tail_a = ListTail(base, a);
  schema::CollisionPair(base, pair).set_a_previous(uint32_t(tail_a));
  if (tail_a == 0) {
    va::CollisionObjectSetWord264(base, schema::CollisionPair(base, pair).a().address, pair);
  } else {
    Repoint(base, tail_a, schema::CollisionPair(base, pair).a().address, true, pair);
  }
  const uint32_t second = schema::CollisionPair(base, pair).b().address;
  const uint32_t tail_b = ListTail(base, second);
  schema::CollisionPair(base, pair).set_b_previous(uint32_t(tail_b));
  if (tail_b == 0) {
    const uint32_t object = schema::CollisionPair(base, pair).b().address;
    va::CollisionObjectSetWord264(base, object, pair);
    return object;
  }
  Repoint(base, tail_b, schema::CollisionPair(base, pair).b().address, true, pair);
  return 0;
}

uint32_t CollisionWorldAddPairs(uint8_t* base, uint32_t object, uint32_t groups) {
  ITC_NATIVE_RUNNING("sub_820B94B0 CollisionWorld::AddPairs");
  uint32_t r3 = object;
  uint32_t world = World(base);
  for (uint32_t other = schema::CollisionWorld(base, world).active().address; other != 0;
       other = schema::CollisionWorldObject(base, other).next().address) {
    if (other == object || (schema::CollisionWorldObject(base, other).groups() & groups) == 0) {
      continue;
    }
    const uint32_t owner = schema::CollisionWorldObject(base, object).owner_id();
    if (owner == schema::CollisionWorldObject(base, other).owner_id() && owner != 0) {
      continue;
    }
    r3 = CollisionWorldHasPair(base, object, other) ? 1 : 0;
    if (r3 != 0) {
      continue;
    }
    const uint32_t pair =
        (schema::CollisionWorld(base, world).pair_count() << 5) + schema::CollisionWorld(base, world).pairs().address;
    CopyWord(base, kZero, schema::CollisionPair(base, pair).next_check_address());
    schema::CollisionWorld(base, world).set_pair_count(uint32_t(schema::CollisionWorld(base, world).pair_count() + 1));
    r3 = CollisionWorldLinkPair(base, pair, object, other);
    world = World(base);
  }
  return r3;
}

uint32_t CollisionWorldUnlinkPair(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_820BBC40 CollisionWorld::UnlinkPair");
  uint32_t r3 = pair;
  // The neighbors before it (+0 on A's side, +4 on B's) pointed past it.
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t previous = guest::LoadU32(base, schema::CollisionPair(base, pair).a_previous_address() + side);
    const uint32_t next = guest::LoadU32(base, schema::CollisionPair(base, pair).a_next_address() + side);
    if (previous == 0) {
      r3 = guest::LoadU32(base, schema::CollisionPair(base, pair).a_address() + side);
      va::CollisionObjectSetWord264(base, r3, next);
    } else {
      Repoint(base, previous, guest::LoadU32(base, schema::CollisionPair(base, pair).a_address() + side), true, next);
    }
  }
  // The neighbors after it pointed back past it.
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t next = guest::LoadU32(base, schema::CollisionPair(base, pair).a_next_address() + side);
    const uint32_t previous = guest::LoadU32(base, schema::CollisionPair(base, pair).a_previous_address() + side);
    if (next != 0) {
      Repoint(base, next, guest::LoadU32(base, schema::CollisionPair(base, pair).a_address() + side), false, previous);
    }
  }
  return r3;
}

void CollisionWorldMovePair(uint8_t* base, uint32_t pair, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820BBD30 CollisionWorld::MovePair");
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t previous = guest::LoadU32(base, schema::CollisionPair(base, pair).a_previous_address() + side);
    if (previous == 0) {
      va::CollisionObjectSetWord264(base, guest::LoadU32(base, schema::CollisionPair(base, pair).a_address() + side), slot);
    } else {
      Repoint(base, previous, guest::LoadU32(base, schema::CollisionPair(base, pair).a_address() + side), true, slot);
    }
  }
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t next = guest::LoadU32(base, schema::CollisionPair(base, pair).a_next_address() + side);
    if (next != 0) {
      Repoint(base, next, guest::LoadU32(base, schema::CollisionPair(base, pair).a_address() + side), false, slot);
    }
  }
  // memcpy (sub_825A7410), 32 bytes; its caller checked first that the
  // native copy does what the original would (CollisionWorldRemovePair).
  c_runtime::Memcpy(base, slot, pair, 32);
}

bool CollisionWorldRemovePair(uint8_t* base, uint32_t index, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8A88 CollisionWorld::RemovePair");
  uint32_t world = World(base);
  const uint32_t slot = (index << 5) + schema::CollisionWorld(base, world).pairs().address;
  const uint32_t count = schema::CollisionWorld(base, world).pair_count();
  const bool moves = count - 1 > index;
  const uint32_t last = (count << 5) + schema::CollisionWorld(base, world).pairs().address - 32;
  // The move's memcpy natively only where it's what the library's memcpy
  // does and its override has nothing to record (outside the physical
  // views, the two ranges not sharing memory); else the original, with
  // nothing done. (Unlinking writes only pairs and objects, so the world's
  // +36 and +40 are the same after it.)
  if (moves && !(slot < 0xA0000000u && last < 0xA0000000u && c_runtime::InOneView(slot, 32) &&
                 c_runtime::InOneView(last, 32) && !c_runtime::MayShare(slot, 32, last, 32))) {
    return false;
  }
  *r3 = CollisionWorldUnlinkPair(base, slot);
  world = World(base);
  const uint32_t count_after = schema::CollisionWorld(base, world).pair_count();
  if (count_after - 1 > index) {
    CollisionWorldMovePair(base, (count_after << 5) + schema::CollisionWorld(base, world).pairs().address - 32,
                           slot);
    *r3 = slot;
    world = World(base);
  }
  schema::CollisionWorld(base, world).set_pair_count(uint32_t(schema::CollisionWorld(base, world).pair_count() - 1));
  return true;
}

uint32_t CollisionWorldReset(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B85E8 CollisionWorld::Reset");
  schema::CollisionWorld(base, World(base)).set_updating(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_query_count(uint32_t(32));
  schema::CollisionWorld(base, World(base)).set_active(uint32_t(0));
  uint32_t world = World(base);
  schema::CollisionWorld(base, world).set_free(uint32_t(schema::CollisionWorld(base, world).objects().address));
  world = World(base);
  // Every object on the free list: +228 the next, +232 the previous.
  uint32_t count = schema::CollisionWorld(base, world).object_count();
  for (uint32_t i = 0; i < count; i++) {
    if (i < count - 1) {
      const uint32_t object = schema::CollisionWorld(base, world).objects().address + 320 * i;
      schema::CollisionWorldObject(base, object).set_next(uint32_t(object + 320));
    } else {
      schema::CollisionWorldObject(base, count * 320 + schema::CollisionWorld(base, world).objects().address - 320).set_next(uint32_t(0));
    }
    const uint32_t object = schema::CollisionWorld(base, World(base)).objects().address + 320 * i;
    schema::CollisionWorldObject(base, object).set_previous(uint32_t(int32_t(i) > 0 ? object - 320 : 0));
    world = World(base);
    count = schema::CollisionWorld(base, world).object_count();
  }
  schema::CollisionWorld(base, world).set_active_count(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_pair_count(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_contacts_used(uint32_t(0));
  CollisionWorldResetSurfaces(base);
  return 0x826C2E30;
}


bool CollisionWorldRemovePairsOf(uint8_t* base, uint32_t object, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8BA8 CollisionWorld::RemovePairsOf");
  *r3 = object;
  uint32_t world = World(base);
  if (schema::CollisionWorld(base, world).pair_count() == 0) {
    return true;
  }
  // The pair at `pair` is index `i`'s; after a removal the last pair is in
  // its slot, tested next.
  uint32_t pair = schema::CollisionWorld(base, world).pairs().address;
  for (uint32_t i = 0; i < schema::CollisionWorld(base, world).pair_count(); i++) {
    if (schema::CollisionPair(base, pair).a().address == object || schema::CollisionPair(base, pair).b().address == object) {
      if (!CollisionWorldRemovePair(base, i, r3)) {
        return false;
      }
      world = World(base);
      i--;
    } else {
      pair += 32;
    }
  }
  return true;
}

bool CollisionWorldRemovePairsByGroups(uint8_t* base, uint32_t object, uint32_t groups,
                                       uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8AF8 CollisionWorld::RemovePairsByGroups");
  *r3 = object;
  uint32_t world = World(base);
  if (schema::CollisionWorld(base, world).pair_count() == 0) {
    return true;
  }
  uint32_t pair = schema::CollisionWorld(base, world).pairs().address;
  for (uint32_t i = 0; i < schema::CollisionWorld(base, world).pair_count(); i++) {
    const uint32_t a = schema::CollisionPair(base, pair).a().address;
    const uint32_t b = schema::CollisionPair(base, pair).b().address;
    // The other object out of the groups, unless the object's +244 still
    // meets its +248 (or, on B's side, its +244 the object's +248).
    bool remove = false;
    if (a == object) {
      remove = (schema::CollisionWorldObject(base, b).groups() & groups) != 0 &&
               (schema::CollisionWorldObject(base, a).groups() & schema::CollisionWorldObject(base, b).collides_with()) == 0;
    } else if (b == object) {
      remove = (schema::CollisionWorldObject(base, a).groups() & groups) != 0 &&
               (schema::CollisionWorldObject(base, a).collides_with() & schema::CollisionWorldObject(base, b).groups()) == 0;
    }
    if (remove) {
      if (!CollisionWorldRemovePair(base, i, r3)) {
        return false;
      }
      world = World(base);
      i--;
    } else {
      pair += 32;
    }
  }
  return true;
}

bool CollisionWorldRemoveObject(uint8_t* base, uint32_t object, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B9448 CollisionWorld::RemoveObject");
  va::CollisionObjectUnlink(base, object, schema::CollisionWorld(base, World(base)).address + 20);  // the active list's head
  va::CollisionObjectLink(base, object, schema::CollisionWorld(base, World(base)).address + 24);  // the free list's head
  const uint32_t world = World(base);
  schema::CollisionWorld(base, world).set_active_count(uint32_t(schema::CollisionWorld(base, world).active_count() - 1));
  return CollisionWorldRemovePairsOf(base, object, r3);
}

uint32_t CollisionObjectAddGroups(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7CC8 CollisionObject::AddGroups");
  const uint32_t had = schema::CollisionWorldObject(base, object).collides_with();
  const uint32_t added = bits & ~had;
  if (added == 0) {
    return object;
  }
  schema::CollisionWorldObject(base, object).set_collides_with(uint32_t(had | bits));
  if ((added & 0x00FFFFFF) == 0) {
    return object;
  }
  return CollisionWorldAddPairs(base, object, added);
}

bool CollisionObjectRemoveGroups(uint8_t* base, uint32_t object, uint32_t bits,
                                 uint32_t* removed_groups, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B7CF0 CollisionObject::RemoveGroups");
  *r3 = object;
  *removed_groups = 0;
  const uint32_t had = schema::CollisionWorldObject(base, object).collides_with();
  const uint32_t removed = had & bits;
  if (removed == 0) {
    return true;
  }
  schema::CollisionWorldObject(base, object).set_collides_with(uint32_t(had & ~bits));
  if ((removed & 0x00FFFFFF) == 0) {
    return true;
  }
  *removed_groups = removed;
  return CollisionWorldRemovePairsByGroups(base, object, removed, r3);
}

namespace {

constexpr uint32_t kObjectVtable = 0x82002A94;
constexpr uint32_t kObjectDeletingDestructor = 0x820B9110;
constexpr uint32_t kObjectBytes = 320;

}  // namespace

void CollisionObjectDestruct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7B40 CollisionObject::~CollisionObject");
  schema::CollisionWorldObject(base, object).set_vtable(uint32_t(kObjectVtable));
  schema::CollisionWorldObject(base, object).set_owner_id(uint32_t(0));
  uint32_t r3 = 0;
  collision::CollisionShapeDestroy(base, schema::CollisionWorldObject(base, object).shape_record_address(), &r3);
}

uint32_t CollisionObjectDeletingDestructor(uint8_t* base, uint32_t object, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820B9110 CollisionObject::DeletingDestructor");
  if ((flags & 0x2) == 0) {
    CollisionObjectDestruct(base, object);
    if ((flags & 0x1) != 0) {
      memory::crt::OperatorDelete(object);
    }
    return object;
  }
  const uint32_t block = object - 16;
  const uint32_t count = schema::CollisionObjectBlock(base, block).count();
  uint32_t at = count * kObjectBytes + object;
  for (int32_t i = int32_t(count) - 1; i >= 0; i--) {
    at -= kObjectBytes;
    CollisionObjectDestruct(base, at);
  }
  if ((flags & 0x1) != 0) {
    memory::crt::OperatorDeleteArray(block);
  }
  return block;
}

bool CollisionWorldDestroyable(uint8_t* base) {
  const uint32_t world = schema::CarPhysicsGlobals::collision_world(base).address;
  if (world == 0) {
    return false;
  }
  const uint32_t objects = schema::CollisionWorld(base, world).objects().address;
  if (objects != 0) {
    const uint32_t block = objects - 16;
    if (!collision::Deletable(block)) {
      return false;
    }
    const uint32_t count = schema::CollisionObjectBlock(base, block).count();
    if (count != 0) {
      const uint32_t vtable = guest::VtableWord(base, objects);
      if (vtable != kObjectVtable || guest::LoadU32(base, vtable) != kObjectDeletingDestructor) {
        return false;
      }
      for (int32_t i = int32_t(count) - 1; i >= 0; i--) {
        if (!collision::CollisionShapeDestroyable(base, objects + kObjectBytes * uint32_t(i) + 32)) {
          return false;
        }
      }
    }
  }
  return collision::Deletable(schema::CollisionWorld(base, world).pairs().address) &&
         collision::Deletable(schema::CollisionWorld(base, world).contacts().address) && collision::Deletable(world);
}

void CollisionWorldDestroy(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B86B8 CollisionWorld::Destroy");
  const uint32_t objects = schema::CarPhysicsGlobals::collision_world(base).objects().address;
  if (objects != 0) {
    const uint32_t block = objects - 16;
    if (schema::CollisionObjectBlock(base, block).count() != 0) {
      // The first object's slot 0 (CollisionWorldDestroyable checked it).
      CollisionObjectDeletingDestructor(base, objects, 3);
    } else {
      schema::CollisionObjectBlock(base, block).Delete();
    }
  }
  schema::CarPhysicsGlobals::collision_world(base).set_objects(uint32_t(0));
  memory::crt::OperatorDeleteArray(schema::CarPhysicsGlobals::collision_world(base).pairs().address);
  schema::CarPhysicsGlobals::collision_world(base).set_pairs(uint32_t(0));
  schema::CarPhysicsGlobals::collision_world(base).contacts().Delete();
  schema::CarPhysicsGlobals::collision_world(base).set_contacts(uint32_t(0));
  memory::crt::OperatorDelete(schema::CarPhysicsGlobals::collision_world(base).address);
  schema::CarPhysicsGlobals::set_collision_world(base, uint32_t(0));
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
uint32_t NewArray(uint8_t* base, uint32_t bytes) {
  return memory::crt::OperatorNewArray(base, bytes).value_or(0);
}

// `count` records of `bytes` (80 here) with the words at +12, +28 and +44
// from `first` (the record's offset + 12) zeroed (stfs of 0.0).
void ZeroRecordWs(uint8_t* base, uint32_t block, uint32_t count, uint32_t first) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t at = block + first + 80 * i;
    CopyWord(base, kZero, at);
    CopyWord(base, kZero, at + 16);
    CopyWord(base, kZero, at + 32);
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

bool CollisionWorldCreate(uint8_t* base, uint32_t objects, uint32_t pairs, uint32_t contacts,
                          uint32_t* r3) {
  // The objects' block is a host object of 50 (E5 step 4): Setup's count,
  // the only one; another goes to the original.
  if (objects != kBlockObjects || memory::crt::GetProcessHeap(base) == 0 ||
      !ArrayFits(ObjectsBytes(objects)) ||
      !ArrayFits(PairsBytes(pairs)) || !ArrayFits(RecordsBytes(64)) ||
      !ArrayFits(RecordsBytes(contacts))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B8D30 CollisionWorld::Create");
  const uint32_t world_block = schema::CollisionWorld::New(base).address;  // host memory (E5 step 4)
  schema::CarPhysicsGlobals::set_collision_world(base, uint32_t(world_block));
  schema::CollisionWorld(base, world_block).set_buffer(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_updating(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_query_count(uint32_t(32));
  schema::CollisionWorld(base, World(base)).set_object_count(uint32_t(objects));
  // The objects, with operator new[]'s count header (host memory, E5 step 4).
  {
    const uint32_t n = schema::CollisionWorld(base, World(base)).object_count();
    const uint32_t block = schema::CollisionObjectBlock::New(base).address;
    uint32_t first = 0;
    if (block != 0) {
      first = block + 16;
      schema::CollisionObjectBlock(base, block).set_count(n);
      for (uint32_t i = 0; i < n; i++) {
        CollisionObjectConstruct(base, first + 320 * i);
      }
    }
    schema::CollisionWorld(base, World(base)).set_objects(uint32_t(first));
  }
  // The free list of all of them, as CollisionWorld::Reset makes it.
  uint32_t world = World(base);
  schema::CollisionWorld(base, world).set_active(uint32_t(0));
  schema::CollisionWorld(base, world).set_free(uint32_t(schema::CollisionWorld(base, world).objects().address));
  world = World(base);
  uint32_t count = schema::CollisionWorld(base, world).object_count();
  for (uint32_t i = 0; i < count; i++) {
    if (i < count - 1) {
      const uint32_t object = schema::CollisionWorld(base, world).objects().address + 320 * i;
      schema::CollisionWorldObject(base, object).set_next(uint32_t(object + 320));
    } else {
      schema::CollisionWorldObject(base, count * 320 + schema::CollisionWorld(base, world).objects().address - 320).set_next(uint32_t(0));
    }
    world = World(base);
    const uint32_t object = schema::CollisionWorld(base, world).objects().address + 320 * i;
    schema::CollisionWorldObject(base, object).set_previous(uint32_t(int32_t(i) > 0 ? object - 320 : 0));
    world = World(base);
    count = schema::CollisionWorld(base, world).object_count();
  }
  schema::CollisionWorld(base, world).set_active_count(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_pair_capacity(uint32_t(pairs));
  schema::CollisionWorld(base, World(base)).set_pairs(uint32_t(NewArray(base, PairsBytes(schema::CollisionWorld(base, World(base)).pair_capacity()))));
  schema::CollisionWorld(base, World(base)).set_pair_count(uint32_t(0));
  // The query records: 2 x +60 (host memory, E5 step 4).
  {
    const uint32_t n = schema::CollisionWorld(base, World(base)).query_count() << 1;
    const uint32_t block = schema::CollisionHit::NewArray(base, n).address;
    if (block != 0) {
      ZeroRecordWs(base, block, n, 12);
    }
    schema::CollisionWorld(base, World(base)).set_hits(uint32_t(block));
  }
  world = World(base);
  schema::CollisionWorld(base, world).set_merged_hits(uint32_t(schema::CollisionWorld(base, world).query_count() * 80 + schema::CollisionWorld(base, world).hits().address));
  schema::CollisionWorld(base, World(base)).set_contact_capacity(uint32_t(contacts));
  {
    const uint32_t n = schema::CollisionWorld(base, World(base)).contact_capacity();
    const uint32_t block = schema::CollisionContact::NewArray(base, n).address;  // host memory (E5 step 4)
    if (block != 0) {
      ZeroRecordWs(base, block, n, 28);
    }
    schema::CollisionWorld(base, World(base)).set_contacts(uint32_t(block));
  }
  schema::CollisionWorld(base, World(base)).set_contacts_used(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_default_step_bits(guest::LoadU32(base, 0x820029EC));  // 1/30
  CollisionWorldResetSurfaces(base);
  schema::CarPhysicsGlobals::set_query_walls(base, uint32_t(0x825C8388));
  schema::CarPhysicsGlobals::set_query_ground(base, uint32_t(0x820BC828));
  schema::CollisionWorld(base, World(base)).set_ground_callback(uint32_t(0x820BC4D0));
  *r3 = 0x826C2E30;  // ResetSurfaces'
  return true;
}

bool CollisionWorldSetQueryCapacity(uint8_t* base, uint32_t n, uint32_t* r3) {
  const uint32_t world = World(base);
  if (schema::CollisionWorld(base, world).buffer() != 0 || !collision::Deletable(schema::CollisionWorld(base, world).hits().address) ||
      !ArrayFits(RecordsBytes(n << 1))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B9198 CollisionWorld::SetQueryCapacity");
  schema::CollisionWorld(base, World(base)).set_query_count(uint32_t(n));
  schema::CollisionWorld(base, World(base)).hits().Delete();  // host memory (E5 step 4)
  const uint32_t count = schema::CollisionWorld(base, World(base)).query_count() << 1;
  const uint32_t block = schema::CollisionHit::NewArray(base, count).address;
  if (block != 0) {
    ZeroRecordWs(base, block, count, 12);
  }
  schema::CollisionWorld(base, World(base)).set_hits(uint32_t(block));
  const uint32_t w = World(base);
  schema::CollisionWorld(base, w).set_merged_hits(uint32_t(schema::CollisionWorld(base, w).query_count() * 80 + schema::CollisionWorld(base, w).hits().address));
  *r3 = block;
  return true;
}

bool CollisionWorldSetup(uint8_t* base, uint32_t* r3) {
  uint32_t ignored = 0;
  if (!CollisionWorldCreate(base, 50, 1000, 100, &ignored)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820FF250 CollisionWorld::Setup");
  va::CollisionSetGlobal78(base, 0x820FF1B0);
  va::CollisionSetGlobal7C(base, 0x820FEBD0);
  va::CollisionWorldSetCallback(base, 0x820FEAB0);
  // Create's query block is the layer's: SetQueryCapacity can't refuse.
  CollisionWorldSetQueryCapacity(base, 32, r3);
  const double zero = Lfs(base, kZero), one = Lfs(base, kOne);
  CollisionSurfaceSetC(base, 0x02000000, zero);
  CollisionSurfaceSetC(base, 0x01000000, zero);
  CollisionSurfaceSetC(base, 0x04000000, zero);
  CollisionSurfaceSetD(base, 0x02000000, zero);
  CollisionSurfaceSetD(base, 0x01000000, one);
  CollisionSurfaceSetD(base, 0x04000000, one);
  CollisionSurfaceSetA(base, 0x02000000, zero);
  CollisionSurfaceSetA(base, 0x01000000, Lfs(base, 0x82001BE4));
  CollisionSurfaceSetA(base, 0x04000000, Lfs(base, 0x82002968));
  CollisionSurfaceSetB(base, 0x02000000, zero);
  CollisionSurfaceSetB(base, 0x01000000, zero);
  CollisionSurfaceSetB(base, 0x04000000, zero);
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
double PowFromFrame(uint8_t* base, uint32_t frame, double x, double y) {
  const uint64_t xb = std::bit_cast<uint64_t>(x), yb = std::bit_cast<uint64_t>(y);
  guest::StoreU32(base, frame + 24, uint32_t(yb >> 32));
  guest::StoreU32(base, frame + 28, uint32_t(yb));
  guest::StoreU32(base, frame + 16, uint32_t(xb >> 32));
  guest::StoreU32(base, frame + 20, uint32_t(xb));
  return c_runtime::Pow(x, y);
}

// A query through its pointer: WallHits and GroundHits natively, anything
// else (and their refusals) by the guest call, from Collide's frame.
uint32_t Query(uint8_t* base, uint32_t target, uint32_t hits, uint32_t shape, uint32_t groups,
               uint32_t frame, uint32_t lr, const CollisionGuestCall& guest) {
  uint32_t count = 0;
  if (target == kWallHits &&
      track_collision::TrackCollisionWallHits(base, hits, shape, frame - 1584, &count)) {
    return count;
  }
  if (target == kGroundHits &&
      track_collision::TrackCollisionGroundHits(base, hits, shape, frame - 384, &count)) {
    return count;
  }
  return guest(target, hits, shape, groups, 0, frame, lr);
}

}  // namespace

bool CollisionObjectRespond(uint8_t* base, uint32_t object, uint32_t hits, uint32_t count,
                            uint32_t flag, uint32_t frame, uint32_t* r3) {
  // CollisionObject::Translate runs natively only on a box (the record's
  // current copy): else the original from the start.
  if (!collision::IsBox(base, schema::CollisionShape(base, schema::CollisionWorldObject(base, object).shape_record_address()).current().address)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B81C8 CollisionObject::Respond");
  const uint32_t f = frame;
  const double zero = Lfs(base, kZero);  // f27
  Stfs(base, f + 140, zero);
  Stfs(base, f + 156, zero);
  Stfs(base, f + 172, zero);
  const uint32_t merged = va::CollisionWorldWord68(base);  // r27
  const uint32_t left = CollisionRecordsMerge(base, merged, f + 128, hits, count, f - 304);
  const double half = Lfs(base, kHalf);  // f23
  bool skip = false;                     // r25
  if (left != 0) {
    const uint32_t bits = guest::LoadU32(base, f + 184) & 0x07000000u;
    const uint32_t normal = va::RigidBodyField144(schema::CollisionWorldObject(base, object).body().address);
    if (guest::LoadU32(base, flag) != 0) {
      const double facing = Fabs(double(vl::Vec3DotOf(base, f + 144, normal)));  // f31
      if (guest::LoadU32(base, flag) != 0) {
        const uint32_t body = schema::CollisionWorldObject(base, object).body().address;
        const uint32_t word = schema::RigidBody(base, body).flags();
        if ((word & 1) != 0 && ((word >> 1) & 1) != 0) {
          const double spin = Fabs(Lfs(base, va::RigidBodyField160(body)));
          if (spin < Lfs(base, kSlowSpin) && facing > half) {
            skip = true;
          }
        }
      }
    }
    if ((schema::CollisionWorldObject(base, object).push_out_bits() & bits) != 0) {
      vl::Vec4Scaled(base, f + 112, f + 160, Lfs(base, f + 176));
      CollisionObjectTranslate(base, object, f + 112, f - 128);
    }
  }
  uint32_t r = va::RigidBodyField160(schema::CollisionWorldObject(base, object).body().address);
  const double spin_x = Lfs(base, r);  // f22
  if (left == 0) {
    *r3 = r;
    return true;
  }
  const uint64_t base_bits = (uint64_t(guest::LoadU32(base, kTorqueBase)) << 32) |
                             guest::LoadU32(base, kTorqueBase + 4);
  const double torque_base = std::bit_cast<double>(base_bits);  // f24
  const double one = Lfs(base, kOne);                           // f28
  const double bias = Lfs(base, kRestitutionBias);              // f25
  uint32_t n = left;
  uint32_t hit = merged;
  do {
    const uint32_t w = schema::CollisionHit(base, hit).surface_bits();
    Stfs(base, f + 80, zero);
    const uint32_t mask = schema::CollisionWorldObject(base, object).response_bits();
    Stfs(base, f + 84, zero);
    const uint32_t bits = w & 0x07000000u;  // r31
    Stfs(base, f + 88, zero);
    Stfs(base, f + 92, zero);
    Stfs(base, f + 108, zero);
    if ((mask & bits) == 0 || skip) {
      continue;
    }
    if ((bits & 0x01000000u) != 0) {
      r = va::ContactPosition(schema::CollisionWorldObject(base, object).body().address);
      CopyWord(base, r + 4, schema::CollisionHit(base, hit).point_address() + 4);
    }
    const uint32_t point = hit, normal = schema::CollisionHit(base, hit).push_address();  // r27, r28
    const double v = RigidBodyContactImpulse(base, schema::CollisionWorldObject(base, object).body().address, point,
                                             normal, f - 208, &r);  // f26
    if (!(v > zero)) {
      continue;
    }
    double e = Fmuls(Fadds(CollisionSurfaceOfC(base, bits), bias), half);
    if (e < zero) {
      e = zero;
    } else if (e > one) {
      e = one;
    }
    const double impulse = Fmuls(Fadds(e, one), v);  // f30
    vl::Vec4Scaled(base, f + 208, normal, impulse);
    vl::Vec4Copy(base, f + 96, f + 208);
    double friction = double(va::Object344Float28(base, schema::CollisionWorldObject(base, object).shape()));
    friction = Fadds(CollisionSurfaceOfA(base, bits), friction);  // f31
    if (schema::CollisionWorldObject(base, object).box_friction() != 0) {
      friction = double(va::Object344Float28(base, schema::CollisionWorldObject(base, object).shape()));
    }
    const double power = PowFromFrame(base, f, torque_base, spin_x);  // f29
    if ((bits & 0x04000000u) == 0) {
      const double d = CollisionSurfaceOfD(base, bits);
      vl::Vec4Scaled(base, f + 256, f + 96, Fmuls(d, double(float(power))));
      vl::Vec4Copy(base, f + 80, f + 256);
    }
    if (friction > zero) {
      RigidBodyFrictionImpulse(base, f + 112, schema::CollisionWorldObject(base, object).body().address, point, normal,
                               impulse, friction, f - 192);
      const double scale = CollisionSurfaceOfB(base, bits);
      if ((bits & 0x02000000u) != 0) {
        vl::Vec3Sub(base, f + 96, f + 96, f + 112);
        vl::Vec4Scaled(base, f + 240, f + 112, scale);
        vl::Vec3Sub(base, f + 80, f + 80, f + 240);
      } else {
        vl::Vec3AddTo(base, f + 96, f + 112);
        vl::Vec4Scaled(base, f + 224, f + 112, scale);
        vl::Vec3AddTo(base, f + 80, f + 224);
      }
    }
    va::RigidBodyAddForce(base, schema::CollisionWorldObject(base, object).body().address, f + 96);
    va::RigidBodyAddTorqueAt(base, schema::CollisionWorldObject(base, object).body().address, f + 80, point);
    RigidBodyApplyImpulses(base, schema::CollisionWorldObject(base, object).body().address, one, f - 144);
    const uint32_t contact = va::CollisionWorldNewContact(base);
    r = contact;
    if (contact == 0) {
      continue;
    }
    schema::CollisionContact(base, contact).set_next(uint32_t(schema::CollisionWorldObject(base, object).first_contact().address));
    schema::CollisionWorldObject(base, object).set_first_contact(uint32_t(contact));
    schema::CollisionContact(base, contact).set_other(uint32_t(schema::CollisionHit(base, hit).surface_bits() & 0x80FFFFFFu));
    const uint32_t flags = schema::CollisionHit(base, hit).surface_bits();
    schema::CollisionContact(base, contact).set_strength(float(v));
    schema::CollisionContact(base, contact).set_flags(uint32_t(flags));
    vl::Vec4Copy(base, schema::CollisionContact(base, contact).position_address(), point);
    vl::Vec4Copy(base, schema::CollisionContact(base, contact).normal_address(), schema::CollisionHit(base, hit).normal_address());
    vl::Vec4Copy(base, schema::CollisionContact(base, contact).push_direction_address(), normal);
    r = schema::CollisionContact(base, contact).push_direction_address();
  } while (hit += 80, --n != 0);
  *r3 = r;
  return true;
}

uint32_t CollisionObjectCollide(uint8_t* base, uint32_t object, uint32_t frame,
                                const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B8548 CollisionObject::Collide");
  const uint32_t f = frame;
  const uint32_t hits = va::CollisionWorldWord64(base);  // r30
  const uint32_t record = schema::CollisionWorldObject(base, object).shape_record_address();                  // r29
  auto respond = [&](uint32_t count, uint32_t lr) {
    uint32_t r3 = 0;
    if (!CollisionObjectRespond(base, object, hits, count, f + 80, f - 416, &r3)) {
      r3 = guest(kRespond, object, hits, count, f + 80, f, lr);
    }
    return r3;
  };
  uint32_t count = Query(base, schema::CarPhysicsGlobals::query_walls(base), hits, record,
                         schema::CollisionWorldObject(base, object).collides_with(), f, 0x820B857C, guest);
  if (count != 0) {
    guest::StoreU32(base, f + 80, 1);
    respond(count, 0x820B859C);
  }
  const uint32_t copy = collision::WordAt8(base, record);
  count = Query(base, schema::CarPhysicsGlobals::query_ground(base), hits, copy,
                schema::CollisionWorldObject(base, object).collides_with(), f, 0x820B85C0, guest);
  if (count == 0) {
    return 0;
  }
  guest::StoreU32(base, f + 80, 0);
  return respond(count, 0x820B85E0);
}

uint32_t CollisionWorldCollideAll(uint8_t* base, uint32_t frame, uint32_t r3,
                                  const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B89F0 CollisionWorld::CollideAll");
  for (uint32_t o = schema::CollisionWorld(base, World(base)).active().address; o != 0;
       o = schema::CollisionWorldObject(base, o).next().address) {
    if (schema::CollisionWorldObject(base, o).no_queries() == 0) {
      r3 = CollisionObjectCollide(base, o, frame - 128, guest);
    }
  }
  return r3;
}

uint32_t CollisionWorldSolvePairs(uint8_t* base, double dt, uint32_t frame, uint32_t r3,
                                  const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B87B0 CollisionWorld::SolvePairs");
  const uint32_t f = frame;
  Stfs(base, schema::CarPhysicsGlobals::pair_clock_address(), Fadds(Lfs(base, schema::CarPhysicsGlobals::pair_clock_address()), dt));
  const double wrap = Lfs(base, kClockWrap);
  const double clock = Lfs(base, schema::CarPhysicsGlobals::pair_clock_address());
  uint32_t world = World(base);
  Stfs(base, schema::CarPhysicsGlobals::last_step_address(), dt);
  if (clock > wrap) {
    Stfs(base, schema::CarPhysicsGlobals::pair_clock_address(), Fsubs(clock, wrap));
    uint32_t n = schema::CollisionWorld(base, world).pair_count();
    const uint32_t pairs = schema::CollisionWorld(base, world).pairs().address;
    if (n != 0) {
      uint32_t at = pairs + 28;
      do {
        Stfs(base, at, Fsubs(Lfs(base, at), wrap));
        at += 32;
      } while (--n != 0);
      world = World(base);
    }
  }
  const double zero = Lfs(base, kZero);  // f31
  for (uint32_t pass = 0; pass < 2; pass++) {
    uint32_t o = schema::CollisionWorld(base, world).active().address;
    if (o != 0) {
      for (; o != 0; o = schema::CollisionWorldObject(base, o).next().address) {
        vl::Vec4Copy(base, schema::CollisionWorldObject(base, o).push_out_address(), ops::kZeroVector);
        r3 = schema::CollisionWorldObject(base, o).push_out_address();
      }
      world = World(base);
    }
    uint32_t n = schema::CollisionWorld(base, world).pair_count();
    uint32_t moved = 0;  // r29: the pairs that hit
    uint32_t pair = schema::CollisionWorld(base, world).pairs().address;
    if (n == 0) {
      return r3;
    }
    do {
      const bool due = schema::CollisionPair(base, pair).a().moved() != 0 ||
                       schema::CollisionPair(base, pair).b().moved() != 0 ||
                       !(double(schema::CollisionPair(base, pair).next_check()) > Lfs(base, schema::CarPhysicsGlobals::pair_clock_address()));
      if (!due) {
        continue;
      }
      const double next = collision::CollisionPairNextCheckTime(base, pair, f - 160, &r3);
      schema::CollisionPair(base, pair).set_next_check(float(next));
      if (!(next < Fadds(Lfs(base, schema::CarPhysicsGlobals::pair_clock_address()), Lfs(base, schema::CarPhysicsGlobals::last_step_address())))) {
        continue;
      }
      Stfs(base, f + 92, zero);
      Stfs(base, f + 108, zero);
      vl::Vec4Copy(base, f + 80,
                   va::ContactPosition(schema::CollisionPair(base, pair).a().body().address));
      vl::Vec4Copy(base, f + 96,
                   va::ContactPosition(schema::CollisionPair(base, pair).b().body().address));
      uint32_t hit = 0;
      if (!collision::CollisionPairSolve(base, pair, f - 1632, &hit)) {
        hit = guest(kSolve, pair, 0, 0, 0, f, 0x820B890C);
      }
      r3 = hit;
      if (hit == 0) {
        continue;
      }
      moved++;
      r3 = CollisionObjectCollide(base, schema::CollisionPair(base, pair).a().address, f - 128, guest);
      r3 = CollisionObjectCollide(base, schema::CollisionPair(base, pair).b().address, f - 128, guest);
      RigidBodySetCenterOfMassPosition(
          base, schema::CollisionPair(base, pair).a().body().address, f + 80, f - 96);
      const uint32_t body_b = schema::CollisionPair(base, pair).b().body().address;
      RigidBodySetCenterOfMassPosition(base, body_b, f + 96, f - 96);
      r3 = schema::RigidBody(base, body_b).matrix_address() + 48;
    } while (pair += 32, --n != 0);
    if (moved == 0) {
      return r3;
    }
    world = World(base);
    o = schema::CollisionWorld(base, world).active().address;
    if (o != 0) {
      for (; o != 0; o = schema::CollisionWorldObject(base, o).next().address) {
        const uint32_t push = schema::CollisionWorldObject(base, o).push_out_address();  // r30
        const double sx = Lfs(base, schema::CollisionWorldObject(base, o).push_scale_address());
        const double py = Lfs(base, schema::CollisionWorldObject(base, o).push_out_address() + 4);
        const double sy = Lfs(base, schema::CollisionWorldObject(base, o).push_scale_address() + 4);
        const double sz = Lfs(base, schema::CollisionWorldObject(base, o).push_scale_address() + 8);
        Stfs(base, schema::CollisionWorldObject(base, o).push_out_address() + 4, Fmuls(sy, py));
        const double x = Fmuls(sx, Lfs(base, push));
        const double z = Fmuls(sz, Lfs(base, schema::CollisionWorldObject(base, o).push_out_address() + 8));
        Stfs(base, push, x);
        Stfs(base, schema::CollisionWorldObject(base, o).push_out_address() + 8, z);
        const uint32_t body = schema::CollisionWorldObject(base, o).body().address;
        vl::Vec3Sum(base, f + 112, va::ContactPosition(body), push);
        RigidBodySetCenterOfMassPosition(base, schema::CollisionWorldObject(base, o).body().address, f + 112, f - 96);
        r3 = schema::CollisionWorldObject(base, o).body().address + 336;
      }
      world = World(base);
    }
  }
  return r3;
}

uint32_t CollisionWorldUpdate(uint8_t* base, double dt, uint32_t frame, uint32_t r3,
                              const CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820B92C8 CollisionWorld::Update");
  const uint32_t f = frame;
  schema::CollisionWorld(base, World(base)).set_contacts_used(uint32_t(0));
  schema::CollisionWorld(base, World(base)).set_updating(uint32_t(1));
  for (uint32_t o = schema::CollisionWorld(base, World(base)).active().address; o != 0;
       o = schema::CollisionWorldObject(base, o).next().address) {
    if (schema::CollisionWorldObject(base, o).unmoved() == 0) {
      const uint32_t matrix = va::RigidBodyMatrix(schema::CollisionWorldObject(base, o).body().address);
      if (collision::CollisionShapePlace(base, schema::CollisionWorldObject(base, o).shape_record_address(), matrix, f - 608)) {
        r3 = schema::CollisionWorldObject(base, o).shape_record_address();
      } else {
        r3 = guest(kPlace, schema::CollisionWorldObject(base, o).shape_record_address(), matrix, 0, 0, f, 0x820B9324);
      }
    }
    schema::CollisionWorldObject(base, o).set_first_contact(uint32_t(0));
  }
  // fcmpu then beq: a NaN step isn't equal to 0.0.
  if (!(dt == Lfs(base, kZero))) {
    r3 = CollisionWorldSolvePairs(base, dt, f - 208, r3, guest);
  }
  r3 = CollisionWorldCollideAll(base, f - 96, r3, guest);
  uint32_t world = World(base);
  uint32_t o = schema::CollisionWorld(base, world).active().address;
  if (o != 0) {
    const double one = Lfs(base, kOne);  // f31
    for (; o != 0; o = schema::CollisionWorldObject(base, o).next().address) {
      if (schema::CollisionWorldObject(base, o).first_contact().address == 0 || schema::CollisionWorldObject(base, o).push_count() == 0) {
        continue;
      }
      va::RigidBodyAddForce(base, schema::CollisionWorldObject(base, o).body().address, schema::CollisionWorldObject(base, o).push_address());
      // The count through the stack (std, lfd, fcfid, frsp).
      const uint32_t pushes = schema::CollisionWorldObject(base, o).push_count();
      guest::StoreU32(base, f + 80, 0);
      guest::StoreU32(base, f + 84, pushes);
      const double share = Fdivs(one, double(float(double(int64_t(pushes)))));
      vl::Vec4Scaled(base, f + 96, schema::CollisionWorldObject(base, o).pair_torque_address(), share);
      vl::Vec4Scaled(base, f + 112, f + 96, Lfs(base, schema::CollisionWorldObject(base, o).torque_scale_address()));
      va::RigidBodyAddTorque(base, schema::CollisionWorldObject(base, o).body().address, f + 112);
      r3 = RigidBodyApplyImpulses(base, schema::CollisionWorldObject(base, o).body().address, one, f - 144);
    }
    world = World(base);
  }
  o = schema::CollisionWorld(base, world).active().address;
  if (o != 0) {
    for (; o != 0; o = schema::CollisionWorldObject(base, o).next().address) {
      const uint32_t pushes = schema::CollisionWorldObject(base, o).push_count();
      schema::CollisionWorldObject(base, o).set_moved(uint32_t(0));
      if (pushes != 0) {
        vl::Vec4Copy(base, schema::CollisionWorldObject(base, o).push_address(), ops::kZeroVector);
        vl::Vec4Copy(base, schema::CollisionWorldObject(base, o).pair_torque_address(), ops::kZeroVector);
        schema::CollisionWorldObject(base, o).set_push_count(uint32_t(0));
        r3 = schema::CollisionWorldObject(base, o).pair_torque_address();
      }
    }
    world = World(base);
  }
  schema::CollisionWorld(base, world).set_updating(uint32_t(0));
  return r3;
}

}  // namespace car_physics
