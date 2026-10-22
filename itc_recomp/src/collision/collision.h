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

// --- The box's and the base class's constructors and placement ---

// Shape::Shape (sub_820BB020): the vtable 0x82002AA8; +60 and the w of +76,
// +92, +108 = 0.0; +48 = the zero vector 0x82953790; +32 = 0.0; +16, +20 =
// 0.5 (0x82001BE4), +24 = [0x82002968], +28 = [0x82002204]; +64, +80, +96
// the identity rows (through `frame` + 80 to + 127, the original's 176
// bytes); halfwords +112, +114 = 0.
void ShapeConstruct(uint8_t* base, uint32_t shape, uint32_t frame);
// Shape::Shape(const&) (sub_820BAB48) and BoxShape::BoxShape(const&)
// (sub_820BB688): the copies (the latter's 128 bytes of corners and 192 of
// faces, and the former's 48 at +64, by memcpy). False, with nothing done,
// where those memcpys can't be done natively (src/c_runtime/strings.h:
// ranges that may share memory, or in the physical views).
bool ShapeCopyConstruct(uint8_t* base, uint32_t shape, uint32_t from);
bool BoxCopyConstruct(uint8_t* base, uint32_t box, uint32_t from);
// Whether BoxCopyConstruct copies `from` into `box` natively.
bool BoxCopyFits(uint32_t box, uint32_t from);
// BoxShape::SetCorners (sub_820B99D0): the center, radius, half extents and
// their inverses, the 8 corners, the 6 faces, the cache words zeroed and
// the identity axes from the two corners. `frame`: the original's 224
// bytes. Returns what the original leaves in f1 (the last face's distance).
double BoxSetCorners(uint8_t* base, uint32_t box, uint32_t min_corner, uint32_t max_corner,
                     uint32_t frame);
// BoxShape::BoxShape (sub_820BB188): Shape::Shape (its frame below
// `frame`, the original's 112 bytes), the vtable 0x82002B30, the w of +140,
// +156, the corners and the faces' normals 0.0, then SetCorners.
void BoxConstruct(uint8_t* base, uint32_t box, uint32_t min_corner, uint32_t max_corner,
                  uint32_t frame);
// BoxShape::Place (sub_820B9C40): the box placed from the template box by
// the matrix (the center and corners through it, the faces' normals its
// rows, the axes its rows). False, with nothing done, for a template that
// isn't a box. `frame`: the original's 288 bytes.
bool BoxPlace(uint8_t* base, uint32_t box, uint32_t matrix, uint32_t source, uint32_t frame);

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

// CollisionShape::UpdateBounds (sub_820BAC00): +16..+24 = per axis the
// smaller of the two copies' centers minus the template's radius, +32..+40
// the larger plus it (`blt`: ties and NaNs take the current copy's).
void CollisionShapeUpdateBounds(uint8_t* base, uint32_t record);
// CollisionShape::UpdateMotion (sub_820BAE78): the motion from the previous
// copy to the current one into the matrix +48 (the axes' dot products, and
// the row +96). `frame`: the original's 240 bytes.
void CollisionShapeUpdateMotion(uint8_t* base, uint32_t record, uint32_t frame);
// CollisionShape::Place (sub_820BB758): the copies swapped, the new current
// one placed by the matrix (from the template, or from a copy of it on the
// stack with its bottom lowered by +112), its cache words from the
// previous one, then UpdateMotion and UpdateBounds. False, with nothing
// done, where a copy or the template isn't a box or the stack copy's
// memcpys can't be native. `frame`: the original's 608 bytes.
bool CollisionShapePlace(uint8_t* base, uint32_t record, uint32_t matrix, uint32_t frame);
// CollisionShape::Translate (sub_820BACD0): the current copy moved by the
// offset (BoxShape::Translate), the motion's row +96 worked out again, then
// UpdateBounds. False, with nothing done, where the current copy isn't a
// box. `frame`: the original's 192 bytes.
bool CollisionShapeTranslate(uint8_t* base, uint32_t record, uint32_t offset, uint32_t frame);

// --- The box-box separation tests (src/collision/box_box.cpp). A block
// (336 bytes, four of them in the query at +112, +448, +784, +1120, their
// addresses at the query's +8 to +20) is one box in the other's unit box:
// +0, +16, +32 its 3x3, +48 its center, +64 its outcode, +80 its 8 corners
// 32 bytes apart (each corner's outcode at its +16). ---

// BoxShape::RelativePlacement (sub_820BA408): B in A's unit box into the
// block (its center +48 and its 3x3). BoxShape::RelativeCenter
// (sub_820BA5D8): the center only. They return what the original leaves in
// r3 (B + 384, B + 48).
uint32_t BoxRelativePlacement(uint8_t* base, uint32_t a, uint32_t block, uint32_t b);
uint32_t BoxRelativeCenter(uint8_t* base, uint32_t a, uint32_t block, uint32_t b);
// BoxBox::Outcode (sub_820BD0C8): the block's outcode (bits 1, 2 for x
// beyond +1 or -1 with its reach, 4, 8 for y, 16, 32 for z).
uint32_t BoxBoxOutcode(uint8_t* base, uint32_t block);
// BoxBox::BlockCorners (sub_820BC978): the block's 8 corners and their
// outcodes. `frame`: the original's 320 bytes.
void BoxBoxBlockCorners(uint8_t* base, uint32_t block, uint32_t frame);
// BoxBox::MoveCorners (sub_820BCCE8): the 8 corners from `corners` (a
// block's +80) moved by the offset, their outcodes again.
void BoxBoxMoveCorners(uint8_t* base, uint32_t corners, uint32_t offset);
// BoxBox::FaceSeparates (sub_820BB300): 1 when box B lies wholly outside
// the face at both placements (the faces of A's previous and current
// copies, B's previous and current copies), else 0.
uint32_t BoxBoxFaceSeparates(uint8_t* base, uint32_t previous_face, uint32_t current_face,
                             uint32_t previous_box, uint32_t current_box);
// BoxBox::Separated (sub_820BD280): the cached face (`cache`, the pair's
// +24) or a face of either box separating them at both placements: 1, the
// cache set; else 0, the cache 0 and the blocks' corners. False, with
// nothing done, where a record's template or a copy isn't a box. `frame`:
// the original's 160 bytes.
bool BoxBoxSeparated(uint8_t* base, uint32_t query, uint32_t cache, uint32_t frame,
                     uint32_t* result);
// BoxBox::SeparatedAfterMove (sub_820BD578): the current blocks' centers
// again; 1 when one's outcode shares a bit with the previous one's, else 0
// with the current blocks' corners moved. False as Separated. `frame`: the
// original's 240 bytes.
bool BoxBoxSeparatedAfterMove(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* result);
// Query::Separated (sub_820BD508) and Query::SeparatedAfterMove
// (sub_820BD6E0): the box-box tests for two boxes; false (the original
// runs) for any other shapes. `frame`: the original's 112 and 96 bytes.
bool QuerySeparated(uint8_t* base, uint32_t query, uint32_t cache, uint32_t frame,
                    uint32_t* result);
bool QuerySeparatedAfterMove(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* result);
// CollisionPair::NextCheckTime (sub_820BBB20): the clock (0x826C2D80) plus
// the time until the objects' bounding spheres could touch at the bodies'
// speeds, or plus the step (0x826C2D84). `r3`: what the original leaves
// there. `frame`: the original's 160 bytes.
double CollisionPairNextCheckTime(uint8_t* base, uint32_t pair, uint32_t frame, uint32_t* r3);

// --- The impacts (src/collision/impacts.cpp): the query's +32 its contact
// (+84 the impact time, +80 the depth), +96 the push direction. ---

// BoxBox::SwapForImpact (sub_820BE490): the contact flipped, A and B
// swapped (+0, +4) and the blocks +8 with +16, +12 with +20.
// BoxBox::SwapForPushOut (sub_820BEBF0): the direction +96 x -1.0, the
// contact flipped, A and B swapped and the blocks +12 with +20. Both return
// what the original leaves in r3 (the query + 64).
uint32_t BoxBoxSwapForImpact(uint8_t* base, uint32_t query);
uint32_t BoxBoxSwapForPushOut(uint8_t* base, uint32_t query);
// BoxBox::CornerImpacts (sub_820BECA0): A's corners crossing a face of B's
// unit box between the placements, the earliest under the time so far (+84)
// set as the contact. False, with nothing done, where a record's template or
// current copy isn't a box. `r3`: what the original leaves there.
bool BoxBoxCornerImpacts(uint8_t* base, uint32_t query, uint32_t* r3);
// BoxBox::FindImpact (sub_820BF038): CornerImpacts both ways round (swapped
// and back). False as CornerImpacts.
bool BoxBoxFindImpact(uint8_t* base, uint32_t query);
// Query::FindImpact (sub_820BCE80): +80 = 0.0, +84 = +88 = 2.0, the impact
// test for the shapes' types (box-box: BoxBox::FindImpact), then 1 when
// the time +84 is under [0x82002B90]. False, with nothing done, where the
// shapes aren't boxes or the table's box-box entry isn't
// BoxBox::FindImpact.
bool QueryFindImpact(uint8_t* base, uint32_t query, uint32_t* result);

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
