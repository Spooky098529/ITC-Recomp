// A pair of collision objects solved and its contacts answered
// (docs/functions/collision.md): see collision.h. The response moves and
// pushes the objects through the car physics' plain C++
// (src/car_physics/car_physics.h).

#include "collision/collision.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;
namespace cp = car_physics;

namespace {

constexpr uint32_t kMinusOne = 0x82001BE0;  // the game's float -1.0
constexpr uint32_t kEpsilon = 0x8200222C;   // a small epsilon, probably
// The impulse's scale for the contact's force (read from guest memory).
constexpr uint32_t kImpulseScale = 0x82600E84;

// Whether the masks of one object's word and the other's share a bit.
bool Meet(uint8_t* base, uint32_t a, uint32_t a_at, uint32_t b, uint32_t b_at) {
  return (guest::LoadU32(base, a + a_at) & guest::LoadU32(base, b + b_at)) != 0;
}

// A contact recorded on `object` (CollisionObject::AddContact) for its
// meeting with `other`: +32 = the normal, +16 = the point, +64 = the
// impulse, +0 = the other object, +4 = its groups (+244).
void RecordContact(uint8_t* base, uint32_t contact, uint32_t normal, uint32_t point,
                   uint32_t other, double impulse) {
  vl::Vec4Copy(base, contact + 32, normal);
  vl::Vec4Copy(base, contact + 16, point);
  const uint32_t groups = guest::LoadU32(base, other + 244);
  Stfs(base, contact + 64, impulse);
  guest::StoreU32(base, contact + 0, other);
  guest::StoreU32(base, contact + 4, groups);
}

}  // namespace

bool CollisionPairRespondNative(uint8_t* base, uint32_t a, uint32_t b) {
  for (uint32_t object : {a, b}) {
    if (!IsBox(base, guest::LoadU32(base, object + 32 + 8))) {
      return false;
    }
  }
  // RigidBody::ContactImpulseWith runs the original for a fixed body.
  if (Meet(base, a, 256, b, 244) || Meet(base, b, 256, a, 244)) {
    for (uint32_t object : {a, b}) {
      if (guest::LoadU8(base, guest::LoadU32(base, object + 16)) == 1) {
        return false;
      }
    }
  }
  return true;
}

void CollisionPairRespond(uint8_t* base, uint32_t a, uint32_t b, uint32_t contact,
                          uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BB868 CollisionPair::Respond");
  const uint32_t f = frame;
  const uint32_t normal = contact + 16;
  const uint32_t direction = contact + 32;
  // The push-out: A when its +260 meets B's +244, B when its +260 meets
  // A's, by the depth plus the epsilon.
  bool push_a = Meet(base, a, 260, b, 244);
  bool push_b = Meet(base, b, 260, a, 244);
  const double depth = Fadds(Lfs(base, contact + 48), Lfs(base, kEpsilon));
  double impulse = Lfs(base, kZero);
  const double minus_one = Lfs(base, kMinusOne);
  if ((push_a || push_b) && depth > impulse) {
    // The mass shares: A moved by B's, B by A's (negated); a fixed body
    // isn't moved and its partner takes all.
    const double mass_a = double(va::RigidBodyMass(base, guest::LoadU32(base, a + 16)));
    const double mass_b = double(va::RigidBodyMass(base, guest::LoadU32(base, b + 16)));
    double share_a = Lfs(base, kOne);
    double share_b = Fdivs(share_a, Fadds(mass_b, mass_a));
    if ((va::RigidBodyIsFixed(base, guest::LoadU32(base, b + 16)) & 0xFF) != 0) {
      push_b = false;
    } else {
      share_a = Fmuls(double(va::RigidBodyMass(base, guest::LoadU32(base, b + 16))), share_b);
    }
    if ((va::RigidBodyIsFixed(base, guest::LoadU32(base, a + 16)) & 0xFF) != 0) {
      share_b = minus_one;
    } else {
      share_b = Fneg(
          Fmuls(double(va::RigidBodyMass(base, guest::LoadU32(base, a + 16))), share_b));
      if (push_a) {
        vl::Vec4Scaled(base, f + 96, direction, depth);
        vl::Vec4Scaled(base, f + 112, f + 96, share_a);
        cp::CollisionObjectTranslate(base, a, f + 112, f - 128);
      }
    }
    if (push_b) {
      vl::Vec4Scaled(base, f + 112, direction, depth);
      vl::Vec4Scaled(base, f + 96, f + 112, share_b);
      cp::CollisionObjectTranslate(base, b, f + 96, f - 128);
    }
  }
  // The impulse: when A's +256 meets B's +244 or B's +256 A's, the force
  // along the normal on each such object (B's negated).
  const bool hit_a = Meet(base, a, 256, b, 244);
  const bool hit_b = Meet(base, b, 256, a, 244);
  if (hit_a || hit_b) {
    CopyWord(base, kZero, f + 92);
    double f1 = 0.0;
    uint32_t ignored = 0;
    cp::RigidBodyContactImpulseWith(base, guest::LoadU32(base, a + 16), contact, normal,
                                    guest::LoadU32(base, b + 16), f - 272, &f1, &ignored);
    impulse = f1;
    vl::Vec4Scaled(base, f + 112, normal, Fmuls(Lfs(base, kImpulseScale), impulse));
    vl::Vec4Copy(base, f + 80, f + 112);
    if (hit_a) {
      cp::CollisionObjectAccumulateForce(base, a, contact, f + 80, f - 160);
    }
    if (hit_b) {
      vl::Vec4Scaled(base, f + 112, f + 80, minus_one);
      cp::CollisionObjectAccumulateForce(base, b, contact, f + 112, f - 160);
    }
  }
  // A contact on each object, B's with the normal negated.
  const uint32_t on_a = cp::CollisionObjectAddContact(base, a);
  if (on_a != 0) {
    RecordContact(base, on_a, normal, contact, b, impulse);
  }
  const uint32_t on_b = cp::CollisionObjectAddContact(base, b);
  *r3 = 0;
  if (on_b != 0) {
    vl::Vec4Scaled(base, f + 112, normal, minus_one);
    RecordContact(base, on_b, f + 112, contact, a, impulse);
    *r3 = on_b + 16;
  }
}

bool CollisionPairSolve(uint8_t* base, uint32_t pair, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BBE30 CollisionPair::Solve");
  const uint32_t a = guest::LoadU32(base, pair + 16);
  const uint32_t b = guest::LoadU32(base, pair + 20);
  // The query's tests are made natively for boxes only (the records'
  // templates and both copies), and the response for objects
  // CollisionPairRespondNative takes: else the original from the start.
  for (uint32_t object : {a, b}) {
    for (uint32_t at : {0u, 4u, 8u}) {
      if (!IsBox(base, guest::LoadU32(base, object + 32 + at))) {
        return false;
      }
    }
  }
  if (!CollisionPairRespondNative(base, a, b)) {
    return false;
  }
  const uint32_t f = frame;
  const uint32_t query = f + 128;
  QueryConstruct(base, query);
  QuerySetShapes(base, query, guest::LoadU32(base, pair + 16) + 32,
                 guest::LoadU32(base, pair + 20) + 32);
  uint32_t touched = 0;
  uint32_t answer = 0;
  QuerySeparated(base, query, pair + 24, f - 112, &answer);
  if (answer != 0) {
    *result = 0;
    return true;
  }
  // After each response: done when the objects no longer push each other
  // out, or the moved copies are apart.
  auto respond = [&](uint32_t* r3) {
    const uint32_t contact = va::RigidBodyVelocity(query);  // query + 32
    CollisionPairRespond(base, guest::LoadU32(base, pair + 16), guest::LoadU32(base, pair + 20),
                         contact, f - 240, r3);
    touched = 1;
    const uint32_t oa = guest::LoadU32(base, pair + 16);
    const uint32_t ob = guest::LoadU32(base, pair + 20);
    if ((guest::LoadU32(base, oa + 260) & guest::LoadU32(base, ob + 244)) == 0 ||
        (guest::LoadU32(base, oa + 244) & guest::LoadU32(base, ob + 260)) == 0) {
      return true;
    }
    uint32_t apart = 0;
    QuerySeparatedAfterMove(base, query, f - 96, &apart);
    return apart != 0;
  };
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 10; i++) {
    uint32_t found = 0;
    QueryFindImpact(base, query, &found);
    if (found == 0) {
      break;
    }
    if (respond(&r3)) {
      *result = 1;
      return true;
    }
  }
  // The push-outs, along the direction from B's previous center to A's
  // weighted by the objects' +208 ((1, 0, 0) when too short), normalized.
  CopyWord(base, kZero, f + 92);
  const uint32_t center_b =
      CollisionShapePreviousCenter(base, guest::LoadU32(base, pair + 20) + 32);
  const uint32_t center_a =
      CollisionShapePreviousCenter(base, guest::LoadU32(base, pair + 16) + 32);
  vl::Vec3Difference(base, f + 112, center_a, center_b);
  vl::Vec4Copy(base, f + 80, f + 112);
  const uint32_t ob = guest::LoadU32(base, pair + 20);
  const uint32_t oa = guest::LoadU32(base, pair + 16);
  for (uint32_t i = 0; i < 12; i += 4) {
    const double weight = Fmuls(Lfs(base, oa + 208 + i), Lfs(base, ob + 208 + i));
    Stfs(base, f + 80 + i, Fmuls(weight, Lfs(base, f + 80 + i)));
  }
  if (double(vl::Vec3Dot(base, f + 80, f + 80)) < Lfs(base, kEpsilon)) {
    CopyWord(base, kZero, f + 100);
    CopyWord(base, kZero, f + 104);
    CopyWord(base, kZero, f + 108);
    CopyWord(base, kOne, f + 96);
    vl::Vec4Copy(base, f + 80, f + 96);
  }
  vl::Vec3NormalizeSelf(base, f + 80);
  for (uint32_t i = 0; i < 3; i++) {
    uint32_t found = 0;
    QueryFindPushOut(base, query, f + 80, f - 112, &found);
    if (found == 0) {
      break;
    }
    if (respond(&r3)) {
      *result = 1;
      return true;
    }
  }
  *result = touched;
  return true;
}

}  // namespace collision
