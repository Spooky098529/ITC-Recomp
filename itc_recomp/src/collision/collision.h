// The collision world's narrow phase and its shapes as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/collision.md):
// the box shape, the shape record each collision object carries at its +32,
// the query a pair is solved with and its contact. The objects are guest
// objects shared with the recompiled code, so these work on guest memory,
// with the originals' loads and stores in their order and their arithmetic
// step for step (src/car_physics/float_ops.h), so the results are the
// translated code's bit for bit. Temporaries the originals keep on their
// stack go at the same offsets of `frame` (the original's r1 after its
// stwu, guest memory). Flush-to-zero off on entry (the adapters' job); the
// vector library's plain C++ is called directly
// (src/vehicle_accessors/vectors.h). Built with -ffp-model=strict and
// -ffp-contract=off (CMakeLists.txt). Adapters: src/hooks/collision.cpp.
//
// Calls the originals make through a shape's vtable are made natively only
// for a box (vtable 0x82002B30, the only shape seen in play): the functions
// taking a shape return false for any other, with nothing done, and the
// adapter runs the original.

#pragma once

#include <cstdint>

namespace collision {

// The box's vtable (BoxShape, 496 bytes) and the base class's (Shape).
constexpr uint32_t kBoxVtable = 0x82002B30;
constexpr uint32_t kShapeVtable = 0x82002AA8;

// Whether `shape`'s vtable is the box's.
bool IsBox(uint8_t* base, uint32_t shape);

// --- The box's methods (vtable slots) and accessors ---

// BoxShape::CornerCount (sub_820B9EC8), FaceCount (sub_820B9EE0) and
// EdgeCount (sub_820B9EF8): 8, 6 and 12.
uint32_t BoxCornerCount();
uint32_t BoxFaceCount();
uint32_t BoxEdgeCount();
// BoxShape::Corner (sub_820B9ED0): box + 176 + 16i. BoxShape::Face
// (sub_820B9EE8): box + 304 + 32i (the distance, the normal at +16).
uint32_t BoxCorner(uint32_t box, uint32_t i);
uint32_t BoxFace(uint32_t box, uint32_t i);
// BoxShape::EdgeStart and EdgeEnd (sub_820B9F98, sub_820B9FB0) and
// BoxEdges::Start and End (sub_820B9FC8, sub_820B9FE0): the edge's corner
// indices, words of the tables 0x82600D54 and 0x82600D84.
uint32_t BoxEdgeStart(uint8_t* base, uint32_t edge);
uint32_t BoxEdgeEnd(uint8_t* base, uint32_t edge);
uint32_t BoxEdgesStart(uint8_t* base, uint32_t edge);
uint32_t BoxEdgesEnd(uint8_t* base, uint32_t edge);
// BoxShape::SetCacheWord (sub_820BA668) and CacheWord (sub_820BA678): the
// word +160 + 4i. BoxShape::CopyCache (sub_820BAA50): the four words +160
// copied from `from`.
void BoxSetCacheWord(uint8_t* base, uint32_t box, uint32_t i, uint32_t value);
uint32_t BoxCacheWord(uint8_t* base, uint32_t box, uint32_t i);
void BoxCopyCache(uint8_t* base, uint32_t box, uint32_t from);
// Shape::Radius (sub_820BAB40): the float +32.
double ShapeRadius(uint8_t* base, uint32_t shape);
// BoxShape::LowerCorner (sub_820BA368): corner k (word i of 0x82002B0C) or
// k ^ 2, whichever has the smaller y (k when equal or NaN); its address.
uint32_t BoxLowerCorner(uint8_t* base, uint32_t box, uint32_t i);
// BoxShape::FaceDistance (sub_820BA3B0): dot(face i's normal, the point) -
// its distance. `r3`: what the original leaves there (the normal).
double BoxFaceDistance(uint8_t* base, uint32_t box, uint32_t i, uint32_t point, uint32_t* r3);
// BoxShape::FaceDirection (sub_820B9F00): the factors (entry i of
// 0x82600DC0) applied to the face normals +320, +352, +384, copied into the
// static vector 0x826C2DE0; returns it. `frame`: the original's 208 bytes.
uint32_t BoxFaceDirection(uint8_t* base, uint32_t box, uint32_t i, uint32_t frame);
// BoxShape::Translate (sub_820BA2F0): the corners and the center += the
// offset, each face's distance += dot(normal, offset). Returns what the
// original leaves in f1 (the last dot product); r3 is the box + 48.
double BoxTranslate(uint8_t* base, uint32_t box, uint32_t offset);
// sub_822BE2C8 (an accessor shared between classes): the word +8.
uint32_t WordAt8(uint8_t* base, uint32_t object);

// --- The shape record (a collision object's +32: +0 the template shape,
// +4 the previous placed copy, +8 the current one) ---

// CollisionShape::CurrentCenter (sub_820BAB20) and PreviousCenter
// (sub_820BAB30): [+8] + 48 and [+4] + 48.
uint32_t CollisionShapeCurrentCenter(uint8_t* base, uint32_t record);
uint32_t CollisionShapePreviousCenter(uint8_t* base, uint32_t record);
// CollisionShape::Type (sub_820BACB8): the template's type (its slot 4: 2
// for a box). CollisionShape::Previous (sub_820BADF8) and Current
// (sub_820BAE38): the template's type asked for (the answer dropped), then
// +4 or +8. False for a template that isn't a box.
bool CollisionShapeType(uint8_t* base, uint32_t record, uint32_t* type);
bool CollisionShapePrevious(uint8_t* base, uint32_t record, uint32_t* copy);
bool CollisionShapeCurrent(uint8_t* base, uint32_t record, uint32_t* copy);

// --- The query (on CollisionPair::Solve's stack) and its contact (+32) ---

// Query::Query (sub_820BD1C8): the w of +32, +48, +64, +96 and of the 8
// corners of each of the four blocks 0.0; +0, +4 = 0; +8 to +20 the blocks
// at +112, +448, +784, +1120.
void QueryConstruct(uint8_t* base, uint32_t query);
// Query::SetShapes (sub_820BC848): +0 = a, +4 = b.
void QuerySetShapes(uint8_t* base, uint32_t query, uint32_t a, uint32_t b);
// Contact::Set (sub_820BC908): +0 = the point, +16 and +32 = the normal,
// +48 = depth, +52 = time.
void ContactSet(uint8_t* base, uint32_t contact, uint32_t point, uint32_t normal, double depth,
                double time);
// Contact::Flip (sub_820BC858): +16 and +32 x -1.0. Returns the -1.0 (the
// original's f1).
double ContactFlip(uint8_t* base, uint32_t contact);

}  // namespace collision
