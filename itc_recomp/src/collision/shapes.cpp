// The box shape's methods and accessors, the shape record's and the
// query's small functions (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

// The edges' corner indices (12 words each) and BoxShape::LowerCorner's
// corners (4 words).
constexpr uint32_t kEdgeStarts = 0x82600D54;
constexpr uint32_t kEdgeEnds = 0x82600D84;
constexpr uint32_t kLowerCorners = 0x82002B0C;
// BoxShape::FaceDirection's factors (16 bytes an entry) and its result.
constexpr uint32_t kFaceDirectionFactors = 0x82600DC0;
constexpr uint32_t kFaceDirection = 0x826C2DE0;
constexpr uint32_t kMinusOne = 0x82001BE0;  // the game's float -1.0

}  // namespace

bool IsBox(uint8_t* base, uint32_t shape) {
  return guest::LoadU32(base, shape) == kBoxVtable;
}

uint32_t BoxCornerCount() {
  ITC_NATIVE_RUNNING("sub_820B9EC8 BoxShape::CornerCount");
  return 8;
}
uint32_t BoxFaceCount() {
  ITC_NATIVE_RUNNING("sub_820B9EE0 BoxShape::FaceCount");
  return 6;
}
uint32_t BoxEdgeCount() {
  ITC_NATIVE_RUNNING("sub_820B9EF8 BoxShape::EdgeCount");
  return 12;
}
uint32_t BoxCorner(uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820B9ED0 BoxShape::Corner");
  return box + ((i + 11) << 4);
}
uint32_t BoxFace(uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820B9EE8 BoxShape::Face");
  return box + (i << 5) + 304;
}
uint32_t BoxEdgeStart(uint8_t* base, uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9F98 BoxShape::EdgeStart");
  return guest::LoadU32(base, kEdgeStarts + (edge << 2));
}
uint32_t BoxEdgeEnd(uint8_t* base, uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9FB0 BoxShape::EdgeEnd");
  return guest::LoadU32(base, kEdgeEnds + (edge << 2));
}
uint32_t BoxEdgesStart(uint8_t* base, uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9FC8 BoxEdges::Start");
  return guest::LoadU32(base, kEdgeStarts + (edge << 2));
}
uint32_t BoxEdgesEnd(uint8_t* base, uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9FE0 BoxEdges::End");
  return guest::LoadU32(base, kEdgeEnds + (edge << 2));
}

void BoxSetCacheWord(uint8_t* base, uint32_t box, uint32_t i, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820BA668 BoxShape::SetCacheWord");
  guest::StoreU32(base, box + ((i + 40) << 2), value);
}
uint32_t BoxCacheWord(uint8_t* base, uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820BA678 BoxShape::CacheWord");
  return guest::LoadU32(base, box + ((i + 40) << 2));
}
void BoxCopyCache(uint8_t* base, uint32_t box, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820BAA50 BoxShape::CopyCache");
  for (uint32_t at = 160; at < 176; at += 4) {
    guest::StoreU32(base, box + at, guest::LoadU32(base, from + at));
  }
}

double ShapeRadius(uint8_t* base, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_820BAB40 Shape::Radius");
  return Lfs(base, shape + 32);
}

uint32_t BoxLowerCorner(uint8_t* base, uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820BA368 BoxShape::LowerCorner");
  const uint32_t k = guest::LoadU32(base, kLowerCorners + (i << 2));
  const uint32_t partner = k ^ 2;
  const double y = Lfs(base, (k << 4) + box + 180);
  const double partner_y = Lfs(base, (partner << 4) + box + 180);
  // blt: k only when its y is less (equal or NaN takes the partner).
  const uint32_t pick = y < partner_y ? k : partner;
  return ((pick + 11) << 4) + box;
}

double BoxFaceDistance(uint8_t* base, uint32_t box, uint32_t i, uint32_t point, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BA3B0 BoxShape::FaceDistance");
  const uint32_t normal = ((i + 10) << 5) + box;
  const double dot = double(vl::Vec3DotOf(base, normal, point));
  *r3 = normal;
  return Fsubs(dot, Lfs(base, (i << 5) + box + 304));
}

uint32_t BoxFaceDirection(uint8_t* base, uint32_t box, uint32_t i, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B9F00 BoxShape::FaceDirection");
  const uint32_t factors = (i << 4) + kFaceDirectionFactors;
  vl::Vec4Scaled(base, frame + 80, box + 384, Lfs(base, factors + 8));
  vl::Vec4Scaled(base, frame + 96, box + 352, Lfs(base, factors + 4));
  vl::Vec4Scaled(base, frame + 112, box + 320, Lfs(base, factors + 0));
  vl::Vec3Sum(base, frame + 128, frame + 112, frame + 96);
  vl::Vec3Sum(base, frame + 144, frame + 128, frame + 80);
  vl::Vec4Copy(base, kFaceDirection, frame + 144);
  return kFaceDirection;
}

double BoxTranslate(uint8_t* base, uint32_t box, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820BA2F0 BoxShape::Translate");
  for (uint32_t k = 0; k < 8; k++) {
    vl::Vec3AddTo(base, box + 176 + 16 * k, offset);
  }
  double dot = 0.0;
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t face = box + 304 + 32 * k;
    dot = double(vl::Vec3DotOf(base, face + 16, offset));
    Stfs(base, face, Fadds(dot, Lfs(base, face)));
  }
  vl::Vec3AddTo(base, box + 48, offset);
  return dot;
}

uint32_t WordAt8(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_822BE2C8 WordAt8");
  return guest::LoadU32(base, object + 8);
}

uint32_t CollisionShapeCurrentCenter(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820BAB20 CollisionShape::CurrentCenter");
  return guest::LoadU32(base, record + 8) + 48;
}
uint32_t CollisionShapePreviousCenter(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820BAB30 CollisionShape::PreviousCenter");
  return guest::LoadU32(base, record + 4) + 48;
}

// The box's slot 4 (its type) is a leaf answering 2.
bool CollisionShapeType(uint8_t* base, uint32_t record, uint32_t* type) {
  ITC_NATIVE_RUNNING("sub_820BACB8 CollisionShape::Type");
  if (!IsBox(base, guest::LoadU32(base, record))) {
    return false;
  }
  *type = 2;
  return true;
}
bool CollisionShapePrevious(uint8_t* base, uint32_t record, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_820BADF8 CollisionShape::Previous");
  if (!IsBox(base, guest::LoadU32(base, record))) {
    return false;
  }
  *copy = guest::LoadU32(base, record + 4);
  return true;
}
bool CollisionShapeCurrent(uint8_t* base, uint32_t record, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_820BAE38 CollisionShape::Current");
  if (!IsBox(base, guest::LoadU32(base, record))) {
    return false;
  }
  *copy = guest::LoadU32(base, record + 8);
  return true;
}

void QueryConstruct(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BD1C8 Query::Query");
  for (uint32_t at : {44u, 60u, 76u, 108u}) {
    CopyWord(base, kZero, query + at);
  }
  for (uint32_t block : {112u, 448u, 784u, 1120u}) {
    for (uint32_t k = 0; k < 8; k++) {
      CopyWord(base, kZero, query + block + 80 + 12 + 32 * k);
    }
  }
  guest::StoreU32(base, query + 0, 0);
  guest::StoreU32(base, query + 4, 0);
  guest::StoreU32(base, query + 8, query + 112);
  guest::StoreU32(base, query + 12, query + 448);
  guest::StoreU32(base, query + 16, query + 784);
  guest::StoreU32(base, query + 20, query + 1120);
}

void QuerySetShapes(uint8_t* base, uint32_t query, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BC848 Query::SetShapes");
  guest::StoreU32(base, query + 0, a);
  guest::StoreU32(base, query + 4, b);
}

void ContactSet(uint8_t* base, uint32_t contact, uint32_t point, uint32_t normal, double depth,
                double time) {
  ITC_NATIVE_RUNNING("sub_820BC908 Contact::Set");
  vl::Vec4Copy(base, contact, point);
  vl::Vec4Copy(base, contact + 16, normal);
  vl::Vec4Copy(base, contact + 32, normal);
  Stfs(base, contact + 48, depth);
  Stfs(base, contact + 52, time);
}

double ContactFlip(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820BC858 Contact::Flip");
  const double minus_one = Lfs(base, kMinusOne);
  vl::Vec4ScaleBy(base, contact + 16, minus_one);
  vl::Vec4ScaleBy(base, contact + 32, minus_one);
  return minus_one;
}

}  // namespace collision
