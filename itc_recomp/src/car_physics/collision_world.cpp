// The collision world's objects, contacts and surface settings
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "memory/c_runtime_heap.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

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
  const double squared = double(vl::Vec3Dot(base, record + 32, record + 32));
  if (squared == Lfs(base, kZero)) {
    CopyWord(base, kZero, record + 48);
    return;
  }
  const float length = vl::Sqrtf(squared);
  guest::StoreFloat(base, record + 48, length);
  vl::Vec4DivideBy(base, record + 32, double(length));
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
  CopyWord(base, kZero, shape + 28);
  guest::StoreU32(base, shape + 4, 0);
  CopyWord(base, kZero, shape + 44);
  guest::StoreU32(base, shape + 8, 0);
  guest::StoreU32(base, shape + 0, 0);
}

void CollisionObjectConstruct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7AE8 CollisionObject::CollisionObject");
  guest::StoreU32(base, object, 0x82002A94);
  CollisionShapeInit(base, object + 32);
  for (uint32_t at : {172u, 188u, 220u, 316u}) {
    CopyWord(base, kZero, object + at);
  }
}

uint32_t CollisionObjectAccumulateForce(uint8_t* base, uint32_t object, uint32_t point,
                                        uint32_t force, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7DF8 CollisionObject::AccumulateForce");
  const uint32_t f = frame;
  CopyWord(base, kZero, f + 92);
  const uint32_t center = va::ContactPosition(guest::LoadU32(base, object + 16));
  vl::Vec3Difference(base, f + 96, point, center);
  vl::Vec3Cross(base, f + 112, f + 96, force);
  vl::Vec4Copy(base, f + 80, f + 112);
  vl::Vec3AddTo(base, object + 176, f + 80);
  // The largest push per axis (+160 to +168).
  for (uint32_t i = 0; i < 12; i += 4) {
    if (Fabs(Lfs(base, force + i)) > Fabs(Lfs(base, object + 160 + i))) {
      CopyWord(base, force + i, object + 160 + i);
    }
  }
  guest::StoreU32(base, object + 284, 0);
  guest::StoreU32(base, object + 192, guest::LoadU32(base, object + 192) + 1);
  return object + 176;
}

uint32_t CollisionObjectAddContact(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7C50 CollisionObject::AddContact");
  const uint32_t contact = va::CollisionWorldNewContact(base);
  if (contact != 0) {
    guest::StoreU32(base, contact + 68, guest::LoadU32(base, object + 240));
    guest::StoreU32(base, object + 240, contact);
  }
  return contact;
}

bool CollisionObjectTranslate(uint8_t* base, uint32_t object, uint32_t offset, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7B60 CollisionObject::Translate");
  // CollisionShape::Translate runs natively only on a box (its current
  // copy, the record's +8): else the original from the start.
  if (!collision::IsBox(base, guest::LoadU32(base, object + 32 + 8))) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t body = guest::LoadU32(base, object + 16);
  // The body's center (+16, its 16 bytes by ld and std) moved by the offset.
  const uint32_t center = va::ContactPosition(body);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 80 + i, guest::LoadU32(base, center + i));
  }
  vl::Vec3AddTo(base, f + 80, offset);
  RigidBodySetCenterOfMassPosition(base, guest::LoadU32(base, object + 16), f + 80, f - 96);
  collision::CollisionShapeTranslate(base, object + 32, offset, f - 192);
  vl::Vec3AddTo(base, object + 304, offset);
  guest::StoreU32(base, object + 284, 0);
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
    if (int32_t(guest::LoadU32(base, record + 60)) == -1) {
      continue;
    }
    // The later records with the same id (+60) merged into this one: their
    // points and normals summed, their pushes (+32 x +48) too.
    int32_t merged = 0;
    for (uint32_t j = i + 1; j < count; j++) {
      const uint32_t other = out + 80 * j;
      if (guest::LoadU32(base, record + 60) != guest::LoadU32(base, other + 60)) {
        continue;
      }
      vl::Vec3AddTo(base, record, other);
      vl::Vec3AddTo(base, record + 16, other + 16);
      const double strength = Lfs(base, other + 48);
      if (merged > 0) {
        vl::Vec4Scaled(base, f + 96, other + 32, strength);
        vl::Vec3AddTo(base, record + 32, f + 96);
      } else {
        vl::Vec4Scaled(base, f + 112, other + 32, strength);
        vl::Vec4Scaled(base, f + 128, record + 32, Lfs(base, record + 48));
        vl::Vec3Sum(base, f + 144, f + 128, f + 112);
        vl::Vec4Copy(base, record + 32, f + 144);
      }
      guest::StoreU32(base, other + 60, uint32_t(-1));
      merged++;
    }
    if (merged != 0) {
      const double scale = Fdivs(one, StackIntegerFloat(base, f + 80, merged + 1));
      vl::Vec4ScaleBy(base, record, scale);
      vl::Vec4ScaleBy(base, record + 16, scale);
      vl::Vec4ScaleBy(base, record + 32, scale);
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
    if ((guest::LoadU32(base, record + 56) & 0x07000000) == 0) {
      continue;
    }
    vl::Vec3AddTo(base, summary, record);
    vl::Vec3AddTo(base, summary + 16, record + 16);
    vl::Vec4Scaled(base, f + 144, record + 32, Lfs(base, record + 48));
    vl::Vec3AddTo(base, summary + 32, f + 144);
    guest::StoreU32(base, summary + 56, guest::LoadU32(base, record + 56));
    guest::StoreU32(base, summary + 60, guest::LoadU32(base, record + 60));
  }
  vl::Vec4ScaleBy(base, summary, Fdivs(one, StackIntegerFloat(base, f + 80, int32_t(kept))));
  vl::Vec3NormalizeSelf(base, summary + 16);
  NormalizePush(base, summary);
  return kept;
}


// --- The world's pairs and its reset ---

namespace {

constexpr uint32_t kWorldPointer = 0x826C2D88;  // the collision world's pointer

uint32_t World(uint8_t* base) {
  return guest::LoadU32(base, kWorldPointer);
}

// The pair's last in an object's list (+0/+8 on the side where the pair's
// +16 is the object, +4/+12 on the other), from the head +264; 0 for none.
uint32_t ListTail(uint8_t* base, uint32_t object) {
  uint32_t pair = va::CollisionObjectWord264(base, object);
  uint32_t last = 0;
  while (pair != 0) {
    last = pair;
    pair = guest::LoadU32(base, pair + 16) == object ? guest::LoadU32(base, pair + 8)
                                                     : guest::LoadU32(base, pair + 12);
  }
  return last;
}

// A neighbor's link toward the pair repointed: its next (+8, or +12 on its
// B side) when `next`, else its previous (+0, or +4), by whether its +16 is
// `object`.
void Repoint(uint8_t* base, uint32_t neighbor, uint32_t object, bool next, uint32_t to) {
  const bool a_side = guest::LoadU32(base, neighbor + 16) == object;
  guest::StoreU32(base, neighbor + (next ? 8 : 0) + (a_side ? 0 : 4), to);
}

}  // namespace

bool CollisionWorldHasPair(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820B8C20 CollisionWorld::HasPair");
  const uint32_t world = World(base);
  const uint32_t count = guest::LoadU32(base, world + 40);
  if (count == 0) {
    return false;
  }
  uint32_t pair = guest::LoadU32(base, world + 36);
  for (uint32_t i = 0; i < count; i++, pair += 32) {
    const uint32_t first = guest::LoadU32(base, pair + 16);
    if (first == a && guest::LoadU32(base, pair + 20) == b) {
      return true;
    }
    if (first == b && guest::LoadU32(base, pair + 20) == a) {
      return true;
    }
  }
  return false;
}

uint32_t CollisionWorldLinkPair(uint8_t* base, uint32_t pair, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BC068 CollisionWorld::LinkPair");
  CopyWord(base, kZero, pair + 28);
  guest::StoreU32(base, pair + 20, b);
  guest::StoreU32(base, pair + 16, a);
  guest::StoreU32(base, pair + 8, 0);
  guest::StoreU32(base, pair + 12, 0);
  guest::StoreU32(base, pair + 24, 0);
  const uint32_t tail_a = ListTail(base, a);
  guest::StoreU32(base, pair + 0, tail_a);
  if (tail_a == 0) {
    va::CollisionObjectSetWord264(base, guest::LoadU32(base, pair + 16), pair);
  } else {
    Repoint(base, tail_a, guest::LoadU32(base, pair + 16), true, pair);
  }
  const uint32_t second = guest::LoadU32(base, pair + 20);
  const uint32_t tail_b = ListTail(base, second);
  guest::StoreU32(base, pair + 4, tail_b);
  if (tail_b == 0) {
    const uint32_t object = guest::LoadU32(base, pair + 20);
    va::CollisionObjectSetWord264(base, object, pair);
    return object;
  }
  Repoint(base, tail_b, guest::LoadU32(base, pair + 20), true, pair);
  return 0;
}

uint32_t CollisionWorldAddPairs(uint8_t* base, uint32_t object, uint32_t groups) {
  ITC_NATIVE_RUNNING("sub_820B94B0 CollisionWorld::AddPairs");
  uint32_t r3 = object;
  uint32_t world = World(base);
  for (uint32_t other = guest::LoadU32(base, world + 20); other != 0;
       other = guest::LoadU32(base, other + 228)) {
    if (other == object || (guest::LoadU32(base, other + 244) & groups) == 0) {
      continue;
    }
    const uint32_t owner = guest::LoadU32(base, object + 268);
    if (owner == guest::LoadU32(base, other + 268) && owner != 0) {
      continue;
    }
    r3 = CollisionWorldHasPair(base, object, other) ? 1 : 0;
    if (r3 != 0) {
      continue;
    }
    const uint32_t pair =
        (guest::LoadU32(base, world + 40) << 5) + guest::LoadU32(base, world + 36);
    CopyWord(base, kZero, pair + 28);
    guest::StoreU32(base, world + 40, guest::LoadU32(base, world + 40) + 1);
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
    const uint32_t previous = guest::LoadU32(base, pair + side);
    const uint32_t next = guest::LoadU32(base, pair + 8 + side);
    if (previous == 0) {
      r3 = guest::LoadU32(base, pair + 16 + side);
      va::CollisionObjectSetWord264(base, r3, next);
    } else {
      Repoint(base, previous, guest::LoadU32(base, pair + 16 + side), true, next);
    }
  }
  // The neighbors after it pointed back past it.
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t next = guest::LoadU32(base, pair + 8 + side);
    const uint32_t previous = guest::LoadU32(base, pair + side);
    if (next != 0) {
      Repoint(base, next, guest::LoadU32(base, pair + 16 + side), false, previous);
    }
  }
  return r3;
}

void CollisionWorldMovePair(uint8_t* base, uint32_t pair, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820BBD30 CollisionWorld::MovePair");
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t previous = guest::LoadU32(base, pair + side);
    if (previous == 0) {
      va::CollisionObjectSetWord264(base, guest::LoadU32(base, pair + 16 + side), slot);
    } else {
      Repoint(base, previous, guest::LoadU32(base, pair + 16 + side), true, slot);
    }
  }
  for (uint32_t side = 0; side < 8; side += 4) {
    const uint32_t next = guest::LoadU32(base, pair + 8 + side);
    if (next != 0) {
      Repoint(base, next, guest::LoadU32(base, pair + 16 + side), false, slot);
    }
  }
  // memcpy (sub_825A7410), 32 bytes; its caller checked first that the
  // native copy does what the original would (CollisionWorldRemovePair).
  c_runtime::Memcpy(base, slot, pair, 32);
}

bool CollisionWorldRemovePair(uint8_t* base, uint32_t index, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8A88 CollisionWorld::RemovePair");
  uint32_t world = World(base);
  const uint32_t slot = (index << 5) + guest::LoadU32(base, world + 36);
  const uint32_t count = guest::LoadU32(base, world + 40);
  const bool moves = count - 1 > index;
  const uint32_t last = (count << 5) + guest::LoadU32(base, world + 36) - 32;
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
  const uint32_t count_after = guest::LoadU32(base, world + 40);
  if (count_after - 1 > index) {
    CollisionWorldMovePair(base, (count_after << 5) + guest::LoadU32(base, world + 36) - 32,
                           slot);
    *r3 = slot;
    world = World(base);
  }
  guest::StoreU32(base, world + 40, guest::LoadU32(base, world + 40) - 1);
  return true;
}

uint32_t CollisionWorldReset(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B85E8 CollisionWorld::Reset");
  guest::StoreU32(base, World(base) + 56, 0);
  guest::StoreU32(base, World(base) + 60, 32);
  guest::StoreU32(base, World(base) + 20, 0);
  uint32_t world = World(base);
  guest::StoreU32(base, world + 24, guest::LoadU32(base, world + 16));
  world = World(base);
  // Every object on the free list: +228 the next, +232 the previous.
  uint32_t count = guest::LoadU32(base, world + 12);
  for (uint32_t i = 0; i < count; i++) {
    if (i < count - 1) {
      const uint32_t object = guest::LoadU32(base, world + 16) + 320 * i;
      guest::StoreU32(base, object + 228, object + 320);
    } else {
      guest::StoreU32(base, count * 320 + guest::LoadU32(base, world + 16) - 92, 0);
    }
    const uint32_t object = guest::LoadU32(base, World(base) + 16) + 320 * i;
    guest::StoreU32(base, object + 232, int32_t(i) > 0 ? object - 320 : 0);
    world = World(base);
    count = guest::LoadU32(base, world + 12);
  }
  guest::StoreU32(base, world + 28, 0);
  guest::StoreU32(base, World(base) + 40, 0);
  guest::StoreU32(base, World(base) + 52, 0);
  CollisionWorldResetSurfaces(base);
  return 0x826C2E30;
}


bool CollisionWorldRemovePairsOf(uint8_t* base, uint32_t object, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B8BA8 CollisionWorld::RemovePairsOf");
  *r3 = object;
  uint32_t world = World(base);
  if (guest::LoadU32(base, world + 40) == 0) {
    return true;
  }
  // The pair at `pair` is index `i`'s; after a removal the last pair is in
  // its slot, tested next.
  uint32_t pair = guest::LoadU32(base, world + 36);
  for (uint32_t i = 0; i < guest::LoadU32(base, world + 40); i++) {
    if (guest::LoadU32(base, pair + 16) == object || guest::LoadU32(base, pair + 20) == object) {
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
  if (guest::LoadU32(base, world + 40) == 0) {
    return true;
  }
  uint32_t pair = guest::LoadU32(base, world + 36);
  for (uint32_t i = 0; i < guest::LoadU32(base, world + 40); i++) {
    const uint32_t a = guest::LoadU32(base, pair + 16);
    const uint32_t b = guest::LoadU32(base, pair + 20);
    // The other object out of the groups, unless the object's +244 still
    // meets its +248 (or, on B's side, its +244 the object's +248).
    bool remove = false;
    if (a == object) {
      remove = (guest::LoadU32(base, b + 244) & groups) != 0 &&
               (guest::LoadU32(base, a + 244) & guest::LoadU32(base, b + 248)) == 0;
    } else if (b == object) {
      remove = (guest::LoadU32(base, a + 244) & groups) != 0 &&
               (guest::LoadU32(base, a + 248) & guest::LoadU32(base, b + 244)) == 0;
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
  va::CollisionObjectUnlink(base, object, World(base) + 20);
  va::CollisionObjectLink(base, object, World(base) + 24);
  const uint32_t world = World(base);
  guest::StoreU32(base, world + 28, guest::LoadU32(base, world + 28) - 1);
  return CollisionWorldRemovePairsOf(base, object, r3);
}

uint32_t CollisionObjectAddGroups(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7CC8 CollisionObject::AddGroups");
  const uint32_t had = guest::LoadU32(base, object + 248);
  const uint32_t added = bits & ~had;
  if (added == 0) {
    return object;
  }
  guest::StoreU32(base, object + 248, had | bits);
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
  const uint32_t had = guest::LoadU32(base, object + 248);
  const uint32_t removed = had & bits;
  if (removed == 0) {
    return true;
  }
  guest::StoreU32(base, object + 248, had & ~bits);
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
  guest::StoreU32(base, object + 0, kObjectVtable);
  guest::StoreU32(base, object + 268, 0);
  uint32_t r3 = 0;
  collision::CollisionShapeDestroy(base, object + 32, &r3);
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
  const uint32_t count = guest::LoadU32(base, block);
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
  const uint32_t world = guest::LoadU32(base, kWorldPointer);
  if (world == 0) {
    return false;
  }
  const uint32_t objects = guest::LoadU32(base, world + 16);
  if (objects != 0) {
    const uint32_t block = objects - 16;
    if (!collision::Deletable(block)) {
      return false;
    }
    const uint32_t count = guest::LoadU32(base, block);
    if (count != 0) {
      const uint32_t vtable = guest::LoadU32(base, objects);
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
  return collision::Deletable(guest::LoadU32(base, world + 36)) &&
         collision::Deletable(guest::LoadU32(base, world + 48)) && collision::Deletable(world);
}

void CollisionWorldDestroy(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B86B8 CollisionWorld::Destroy");
  const uint32_t objects = guest::LoadU32(base, guest::LoadU32(base, kWorldPointer) + 16);
  if (objects != 0) {
    const uint32_t block = objects - 16;
    if (guest::LoadU32(base, block) != 0) {
      // The first object's slot 0 (CollisionWorldDestroyable checked it).
      CollisionObjectDeletingDestructor(base, objects, 3);
    } else {
      memory::crt::OperatorDeleteArray(block);
    }
  }
  guest::StoreU32(base, guest::LoadU32(base, kWorldPointer) + 16, 0);
  memory::crt::OperatorDeleteArray(guest::LoadU32(base, guest::LoadU32(base, kWorldPointer) + 36));
  guest::StoreU32(base, guest::LoadU32(base, kWorldPointer) + 36, 0);
  memory::crt::OperatorDeleteArray(guest::LoadU32(base, guest::LoadU32(base, kWorldPointer) + 48));
  guest::StoreU32(base, guest::LoadU32(base, kWorldPointer) + 48, 0);
  memory::crt::OperatorDelete(guest::LoadU32(base, kWorldPointer));
  guest::StoreU32(base, kWorldPointer, 0);
}

}  // namespace car_physics
