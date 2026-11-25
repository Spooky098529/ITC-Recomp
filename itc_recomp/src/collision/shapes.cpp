// The box shape's methods and accessors, the shape record's and the
// query's small functions (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "schema/collision.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

using G = schema::CollisionGlobals;
using Box = schema::BoxShape;
using Record = schema::CollisionShape;
using Query = schema::CollisionQuery;
using Contact = schema::QueryContact;

bool IsBox(uint32_t shape) {
  return Box(shape).vtable() == kBoxVtable;
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
  return Box(nullptr, box).corner_x_address() + (i << 4);
}
uint32_t BoxFace(uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820B9EE8 BoxShape::Face");
  return Box(nullptr, box).face_px_distance_address() + (i << 5);
}
uint32_t BoxEdgeStart(uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9F98 BoxShape::EdgeStart");
  return guest::LoadU32(G::edge_starts_address() + (edge << 2));
}
uint32_t BoxEdgeEnd(uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9FB0 BoxShape::EdgeEnd");
  return guest::LoadU32(G::edge_ends_address() + (edge << 2));
}
uint32_t BoxEdgesStart(uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9FC8 BoxEdges::Start");
  return guest::LoadU32(G::edge_starts_address() + (edge << 2));
}
uint32_t BoxEdgesEnd(uint32_t edge) {
  ITC_NATIVE_RUNNING("sub_820B9FE0 BoxEdges::End");
  return guest::LoadU32(G::edge_ends_address() + (edge << 2));
}

void BoxSetCacheWord(uint32_t box, uint32_t i, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820BA668 BoxShape::SetCacheWord");
  guest::StoreU32(Box(box).lower_corner_position_0_address() + (i << 2), value);
}
uint32_t BoxCacheWord(uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820BA678 BoxShape::CacheWord");
  return guest::LoadU32(Box(box).lower_corner_position_0_address() + (i << 2));
}
void BoxCopyCache(uint32_t box, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820BAA50 BoxShape::CopyCache");
  const Box to(box), source(from);
  to.set_lower_corner_position_0(source.lower_corner_position_0());
  to.set_lower_corner_position_1(source.lower_corner_position_1());
  to.set_lower_corner_position_2(source.lower_corner_position_2());
  to.set_lower_corner_position_3(source.lower_corner_position_3());
}

double ShapeRadius(uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_820BAB40 Shape::Radius");
  return double(Box(shape).radius());
}

uint32_t BoxLowerCorner(uint32_t box, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820BA368 BoxShape::LowerCorner");
  const Box b(box);
  const uint32_t k = guest::LoadU32(G::lower_corners_address() + (i << 2));
  const uint32_t partner = k ^ 2;
  const double y = Lfs(b.corner_y_address() + (k << 4));
  const double partner_y = Lfs(b.corner_y_address() + (partner << 4));
  // blt: k only when its y is less (equal or NaN takes the partner).
  const uint32_t pick = y < partner_y ? k : partner;
  return b.corner_x_address() + (pick << 4);
}

double BoxFaceDistance(uint32_t box, uint32_t i, uint32_t point, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BA3B0 BoxShape::FaceDistance");
  const Box b(box);
  const uint32_t normal = b.face_px_normal_x_address() + (i << 5);
  const double dot = double(vl::Vec3DotOf(normal, point));
  *r3 = normal;
  return Fsubs(dot, Lfs(b.face_px_distance_address() + (i << 5)));
}

uint32_t BoxFaceDirection(uint32_t box, uint32_t i, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B9F00 BoxShape::FaceDirection");
  const Box b(box);
  const uint32_t factors = (i << 4) + G::face_direction_factors_address();
  vl::Vec4Scaled(frame + 80, b.face_pz_normal_x_address(), Lfs(factors + 8));
  vl::Vec4Scaled(frame + 96, b.face_py_normal_x_address(), Lfs(factors + 4));
  vl::Vec4Scaled(frame + 112, b.face_px_normal_x_address(), Lfs(factors + 0));
  vl::Vec3Sum(frame + 128, frame + 112, frame + 96);
  vl::Vec3Sum(frame + 144, frame + 128, frame + 80);
  vl::Vec4Copy(G::face_direction_address(), frame + 144);
  return G::face_direction_address();
}

double BoxTranslate(uint32_t box, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820BA2F0 BoxShape::Translate");
  const Box b(box);
  for (uint32_t k = 0; k < 8; k++) {
    vl::Vec3AddTo(b.corner_x_address() + 16 * k, offset);
  }
  double dot = 0.0;
  for (uint32_t k = 0; k < 6; k++) {
    const schema::BoxShapeFace face(b.face_px_distance_address() + 32 * k);
    dot = double(vl::Vec3DotOf(face.normal_x_address(), offset));
    face.set_distance(float(Fadds(dot, double(face.distance()))));
  }
  vl::Vec3AddTo(b.center_x_address(), offset);
  return dot;
}

uint32_t WordAt8(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_822BE2C8 WordAt8");
  return schema::WordAt8Object(object).word_8();
}

uint32_t CollisionShapeCurrentCenter(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820BAB20 CollisionShape::CurrentCenter");
  return Record(record).current().center_x_address();
}
uint32_t CollisionShapePreviousCenter(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820BAB30 CollisionShape::PreviousCenter");
  return Record(record).previous().center_x_address();
}

// The box's slot 4 (its type) is a leaf answering 2.
bool CollisionShapeType(uint32_t record, uint32_t* type) {
  ITC_NATIVE_RUNNING("sub_820BACB8 CollisionShape::Type");
  if (!IsBox(Record(record).template_shape().address)) {
    return false;
  }
  *type = 2;
  return true;
}
bool CollisionShapePrevious(uint32_t record, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_820BADF8 CollisionShape::Previous");
  const Record r(record);
  if (!IsBox(r.template_shape().address)) {
    return false;
  }
  *copy = r.previous().address;
  return true;
}
bool CollisionShapeCurrent(uint32_t record, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_820BAE38 CollisionShape::Current");
  const Record r(record);
  if (!IsBox(r.template_shape().address)) {
    return false;
  }
  *copy = r.current().address;
  return true;
}

void QueryConstruct(uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BD1C8 Query::Query");
  const Query q(query);
  for (uint32_t at : {q.contact_point_w_address(), q.contact_normal_w_address(),
                      q.contact_push_w_address(), q.direction_w_address()}) {
    CopyWord(kZero, at);
  }
  for (uint32_t block : {q.b_in_a_previous_block_address(), q.b_in_a_current_block_address(),
                         q.a_in_b_previous_block_address(), q.a_in_b_current_block_address()}) {
    for (uint32_t k = 0; k < 8; k++) {
      CopyWord(kZero, schema::CollisionBlock(block).corner_w_address() + 32 * k);
    }
  }
  q.set_shape_a(0);
  q.set_shape_b(0);
  q.set_b_in_a_previous(q.b_in_a_previous_block_address());
  q.set_b_in_a_current(q.b_in_a_current_block_address());
  q.set_a_in_b_previous(q.a_in_b_previous_block_address());
  q.set_a_in_b_current(q.a_in_b_current_block_address());
}

void QuerySetShapes(uint32_t query, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BC848 Query::SetShapes");
  const Query q(query);
  q.set_shape_a(a);
  q.set_shape_b(b);
}

void ContactSet(uint32_t contact, uint32_t point, uint32_t normal, double depth,
                double time) {
  ITC_NATIVE_RUNNING("sub_820BC908 Contact::Set");
  const Contact c(contact);
  vl::Vec4Copy(c.point_x_address(), point);
  vl::Vec4Copy(c.normal_x_address(), normal);
  vl::Vec4Copy(c.push_x_address(), normal);
  c.set_depth(float(depth));
  c.set_time(float(time));
}

double ContactFlip(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820BC858 Contact::Flip");
  const Contact c(contact);
  const double minus_one = double(G::minus_one());
  vl::Vec4ScaleBy(c.normal_x_address(), minus_one);
  vl::Vec4ScaleBy(c.push_x_address(), minus_one);
  return minus_one;
}

}  // namespace collision
