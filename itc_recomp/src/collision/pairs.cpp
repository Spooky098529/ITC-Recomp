// A pair of collision objects solved and its contacts answered
// (docs/functions/collision.md): see collision.h. The response moves and
// pushes the objects through the car physics' plain C++
// (src/car_physics/car_physics.h).

#include "collision/collision.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_physics.h"
#include "schema/collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;
namespace cp = car_physics;

namespace {

using G = schema::CollisionGlobals;
using Object = schema::CollisionPairObject;
using Contact = schema::QueryContact;
using Pair = schema::CollisionPair;

// Whether the surface bits of one object that get a response (`response`)
// or push it out (else) meet the other's groups.
bool Meet(uint8_t* base, uint32_t a, bool response, uint32_t b) {
  const Object object_a(base, a), object_b(base, b);
  return ((response ? object_a.response_bits() : object_a.push_out_bits()) &
          object_b.groups()) != 0;
}

// A contact recorded on `object` (CollisionObject::AddContact) for its
// meeting with `other`: +32 = the normal, +16 = the point, +64 = the
// impulse, +0 = the other object, +4 = its groups (+244).
void RecordContact(uint8_t* base, uint32_t contact, uint32_t normal, uint32_t point,
                   uint32_t other, double impulse) {
  const schema::CollisionContact c(base, contact);
  vl::Vec4Copy(base, c.normal_address(), normal);
  vl::Vec4Copy(base, c.position_address(), point);
  const uint32_t groups = Object(base, other).groups();
  Stfs(base, c.strength_address(), impulse);
  c.set_other(other);
  c.set_flags(groups);
}

}  // namespace

bool CollisionPairRespondNative(uint8_t* base, uint32_t a, uint32_t b) {
  for (uint32_t object : {a, b}) {
    const schema::CollisionShape record(base, Object(base, object).shape_record_address());
    if (!IsBox(base, record.current().address)) {
      return false;
    }
  }
  // RigidBody::ContactImpulseWith runs the original for a fixed body.
  if (Meet(base, a, true, b) || Meet(base, b, true, a)) {
    for (uint32_t object : {a, b}) {
      if (schema::RigidBody(base, Object(base, object).body()).fixed() == 1) {
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
  const Object object_a(base, a), object_b(base, b);
  const Contact c(base, contact);
  const uint32_t normal = c.normal_x_address();
  const uint32_t direction = c.push_x_address();
  // The push-out: A when its +260 meets B's +244, B when its +260 meets
  // A's, by the depth plus the epsilon.
  bool push_a = Meet(base, a, false, b);
  bool push_b = Meet(base, b, false, a);
  const double depth = Fadds(double(c.depth()), double(G::epsilon(base)));
  double impulse = Lfs(base, kZero);
  const double minus_one = double(G::minus_one(base));
  if ((push_a || push_b) && depth > impulse) {
    // The mass shares: A moved by B's, B by A's (negated); a fixed body
    // isn't moved and its partner takes all.
    const double mass_a = double(va::RigidBodyMass(base, object_a.body()));
    const double mass_b = double(va::RigidBodyMass(base, object_b.body()));
    double share_a = Lfs(base, kOne);
    double share_b = Fdivs(share_a, Fadds(mass_b, mass_a));
    if ((va::RigidBodyIsFixed(base, object_b.body()) & 0xFF) != 0) {
      push_b = false;
    } else {
      share_a = Fmuls(double(va::RigidBodyMass(base, object_b.body())), share_b);
    }
    if ((va::RigidBodyIsFixed(base, object_a.body()) & 0xFF) != 0) {
      share_b = minus_one;
    } else {
      share_b = Fneg(
          Fmuls(double(va::RigidBodyMass(base, object_a.body())), share_b));
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
  const bool hit_a = Meet(base, a, true, b);
  const bool hit_b = Meet(base, b, true, a);
  if (hit_a || hit_b) {
    CopyWord(base, kZero, f + 92);
    double f1 = 0.0;
    uint32_t ignored = 0;
    cp::RigidBodyContactImpulseWith(base, object_a.body(), contact, normal,
                                    object_b.body(), f - 272, &f1, &ignored);
    impulse = f1;
    vl::Vec4Scaled(base, f + 112, normal, Fmuls(double(G::contact_impulse_scale(base)), impulse));
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
    *r3 = schema::CollisionContact(base, on_b).position_address();
  }
}

bool CollisionPairSolve(uint8_t* base, uint32_t pair, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BBE30 CollisionPair::Solve");
  const uint32_t a = Pair(base, pair).a().address;
  const uint32_t b = Pair(base, pair).b().address;
  // The query's tests are made natively for boxes only (the records'
  // templates and both copies), and the response for objects
  // CollisionPairRespondNative takes: else the original from the start.
  for (uint32_t object : {a, b}) {
    const schema::CollisionShape record(base, Object(base, object).shape_record_address());
    for (uint32_t at :
         {record.template_shape_address(), record.previous_address(), record.current_address()}) {
      if (!IsBox(base, guest::LoadU32(base, at))) {
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
  QuerySetShapes(base, query, Pair(base, pair).a<Object>().shape_record_address(),
                 Pair(base, pair).b<Object>().shape_record_address());
  uint32_t touched = 0;
  uint32_t answer = 0;
  QuerySeparated(base, query, Pair(base, pair).separated_face_address(), f - 112, &answer);
  if (answer != 0) {
    *result = 0;
    return true;
  }
  // After each response: done when the objects no longer push each other
  // out, or the moved copies are apart.
  auto respond = [&](uint32_t* r3) {
    const uint32_t contact = va::RigidBodyVelocity(query);  // the query's contact (+32)
    CollisionPairRespond(base, Pair(base, pair).a().address, Pair(base, pair).b().address,
                         contact, f - 240, r3);
    touched = 1;
    const Object oa = Pair(base, pair).a<Object>();
    const Object ob = Pair(base, pair).b<Object>();
    if ((oa.push_out_bits() & ob.groups()) == 0 || (oa.groups() & ob.push_out_bits()) == 0) {
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
  const uint32_t center_b = CollisionShapePreviousCenter(
      base, Pair(base, pair).b<Object>().shape_record_address());
  const uint32_t center_a = CollisionShapePreviousCenter(
      base, Pair(base, pair).a<Object>().shape_record_address());
  vl::Vec3Difference(base, f + 112, center_a, center_b);
  vl::Vec4Copy(base, f + 80, f + 112);
  const Object ob = Pair(base, pair).b<Object>();
  const Object oa = Pair(base, pair).a<Object>();
  for (uint32_t i = 0; i < 12; i += 4) {
    const double weight = Fmuls(Lfs(base, oa.push_out_scale_x_address() + i),
                                Lfs(base, ob.push_out_scale_x_address() + i));
    Stfs(base, f + 80 + i, Fmuls(weight, Lfs(base, f + 80 + i)));
  }
  if (double(vl::Vec3Dot(base, f + 80, f + 80)) < double(G::epsilon(base))) {
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
