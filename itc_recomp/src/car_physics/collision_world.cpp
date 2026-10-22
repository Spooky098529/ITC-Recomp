// The collision world's objects, contacts and surface settings
// (docs/functions/car-physics.md): see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
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

}  // namespace car_physics
