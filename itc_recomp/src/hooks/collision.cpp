// The collision world's narrow phase and its shapes natively: adapters in
// front of src/collision/ (docs/functions/collision.md). Each passes the
// original's stack frame (its r1 after the stwu) where the plain C++ keeps
// temporaries there, and leaves r3 (and f1 where the original leaves a
// value there) as the original does. Functions given a shape that isn't a
// box run the original.

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "collision/collision.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_collision, true, "ITC",
              "Run the collision world's narrow phase and its shapes "
              "(docs/functions/collision.md: the box's methods and accessors, sub_820B9EC8 to "
              "sub_820BAA50 and sub_822BE2C8; their constructors and placement, sub_820BB020, "
              "sub_820BAB48, sub_820BB688, sub_820B99D0, sub_820BB188, sub_820B9C40; the shape record's accessors, bounds, motion, "
              "placement and translation, sub_820BAB20, sub_820BAB30, sub_820BACB8, sub_820BADF8, "
              "sub_820BAE38, sub_820BAC00, sub_820BAE78, sub_820BB758, sub_820BACD0; the shapes' "
              "lifetime, sub_820BB140, sub_820BB218, sub_820BAAA0, sub_820BB580 (only while "
              "--native_heap routes the game's heap to the allocation layer); the box-box separation "
              "tests, sub_820BA408, sub_820BA5D8, sub_820BD0C8, sub_820BC978, sub_820BCCE8, "
              "sub_820BB300, sub_820BD280, sub_820BD578, sub_820BD508, sub_820BD6E0; the pair's "
              "next check time, sub_820BBB20; the impacts, sub_820BE490, sub_820BEBF0, "
              "sub_820BECA0, sub_820BF038, sub_820BCE80; the push-outs, sub_820BF080, "
              "sub_820BF320, sub_820BE150, sub_820BFB28, sub_820BCDD0; the pair's solving and "
              "response, sub_820BBE30, sub_820BB868; the query and its contact, sub_820BD1C8, sub_820BC848, sub_820BC908, sub_820BC858) natively "
              "instead of through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=collision). Those reached only from native callers have no "
              "adapters, and the flag switches their callers: sub_820BA3B0, sub_820BB020, "
              "sub_820BAB48, sub_820BB688, sub_820B99D0, sub_820BAB20, sub_820BAE78, "
              "sub_820BA408, sub_820BA5D8, sub_820BD0C8, sub_820BC978, sub_820BCCE8, "
              "sub_820BB300, sub_820BD280, sub_820BD578, sub_820BE490, sub_820BECA0, "
              "sub_820BC858, sub_820BACD0, sub_820BF080, sub_820BF320, sub_820BE150, sub_820BACB8, "
              "sub_820B9FC8, sub_820B9FE0, sub_820BC908, sub_820BEBF0, sub_820BB868");

// Defines an adapter: `body` uses `ctx` and `base`.
#define ITC_COLLISION(address, body)   \
  extern "C" REX_FUNC(sub_##address) { \
    REX_FUNC_PROLOGUE();               \
    if (!ITC_FLAG(native_collision)) { \
      __imp__sub_##address(ctx, base); \
      return;                          \
    }                                  \
    ctx.fpscr.disableFlushMode();      \
    body;                              \
  }
// The same for one handed a shape record: `call(&value)` false (not a box)
// runs the original; else r3 = the value.
#define ITC_COLLISION_RECORD(address, call) \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    uint32_t value = 0;                     \
    if (!ITC_FLAG(native_collision) || !call) { \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    ctx.r3.u64 = value;                     \
  }

using namespace collision;

// --- The box's methods (reached through its vtable) and accessors ---

ITC_COLLISION(820B9EC8, ctx.r3.u64 = BoxCornerCount())
ITC_COLLISION(820B9EE0, ctx.r3.u64 = BoxFaceCount())
ITC_COLLISION(820B9EF8, ctx.r3.u64 = BoxEdgeCount())
ITC_COLLISION(820B9ED0, ctx.r3.u64 = BoxCorner(ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION(820B9EE8, ctx.r3.u64 = BoxFace(ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION(820B9F98, ctx.r3.u64 = BoxEdgeStart(base, ctx.r4.u32))
ITC_COLLISION(820B9FB0, ctx.r3.u64 = BoxEdgeEnd(base, ctx.r4.u32))
// BoxEdges::Start and End (sub_820B9FC8, sub_820B9FE0) have no adapters:
// their caller in play, BoxBox::EdgesCrossing, is native.
ITC_COLLISION(820BA668, BoxSetCacheWord(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_COLLISION(820BA678, ctx.r3.u64 = BoxCacheWord(base, ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION(820BAA50, BoxCopyCache(base, ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION(820BAB40, ctx.f1.f64 = ShapeRadius(base, ctx.r3.u32))
ITC_COLLISION(820BA368, ctx.r3.u64 = BoxLowerCorner(base, ctx.r3.u32, ctx.r4.u32))
// BoxShape::FaceDistance (sub_820BA3B0) has no adapter: its caller in play
// is native.
ITC_COLLISION(820B9F00,
              ctx.r3.u64 = BoxFaceDirection(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 208))
// r4 = the offset; r3 comes back as the box + 48.
ITC_COLLISION(820BA2F0, ctx.f1.f64 = BoxTranslate(base, ctx.r3.u32, ctx.r4.u32);
              ctx.r3.u64 = ctx.r3.u32 + 48)
ITC_COLLISION(822BE2C8, ctx.r3.u64 = WordAt8(base, ctx.r3.u32))

// --- The constructors and the placement ---

// Shape::Shape, its copy, BoxShape's copy and SetCorners (sub_820BB020,
// sub_820BAB48, sub_820BB688, sub_820B99D0) have no adapters: their callers
// in play are native.
// r4 = the min corner, r5 = the max corner; r3 comes back as the box.
ITC_COLLISION(820BB188, BoxConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112))
// Slot 9: r4 = the matrix, r5 = the template; r3 comes back as the box +
// 96. A template that isn't a box: the original.
extern "C" REX_FUNC(sub_820B9C40) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_collision)) {
    __imp__sub_820B9C40(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!BoxPlace(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 288)) {
    __imp__sub_820B9C40(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 96;
}

// --- The shape record ---

// CollisionShape::CurrentCenter (sub_820BAB20) and UpdateMotion
// (sub_820BAE78) have no adapters: their callers in play are native.
ITC_COLLISION(820BAB30, ctx.r3.u64 = CollisionShapePreviousCenter(base, ctx.r3.u32))
// CollisionShape::Type (sub_820BACB8) has no adapter: its callers in play,
// the query's tests, are native.
ITC_COLLISION_RECORD(820BADF8, CollisionShapePrevious(base, ctx.r3.u32, &value))
ITC_COLLISION_RECORD(820BAE38, CollisionShapeCurrent(base, ctx.r3.u32, &value))

ITC_COLLISION(820BAC00, CollisionShapeUpdateBounds(base, ctx.r3.u32))
// r4 = the matrix. Copies or a template that aren't boxes: the original.
extern "C" REX_FUNC(sub_820BB758) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_collision)) {
    __imp__sub_820BB758(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!CollisionShapePlace(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 608)) {
    __imp__sub_820BB758(ctx, base);
  }
}
// CollisionShape::Translate (sub_820BACD0) has no adapter: its caller in
// play, CollisionObject::Translate, is native.

// --- The box-box separation tests and the pair's next check time ---

// BoxShape::RelativePlacement and RelativeCenter, BoxBox::Outcode,
// BlockCorners, MoveCorners, FaceSeparates, Separated and
// SeparatedAfterMove (sub_820BA408, sub_820BA5D8, sub_820BD0C8,
// sub_820BC978, sub_820BCCE8, sub_820BB300, sub_820BD280, sub_820BD578)
// have no adapters: their callers in play are native.
// The query's tests: shapes that aren't boxes run the original.
#define ITC_COLLISION_TEST(address, call)    \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    uint32_t result = 0;                    \
    if (!ITC_FLAG(native_collision)) {      \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    ctx.fpscr.disableFlushMode();           \
    if (!call) {                            \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    ctx.r3.u64 = result;                    \
  }
// r4 = the cache (the pair's +24).
ITC_COLLISION_TEST(820BD508, QuerySeparated(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112,
                                            &result))
ITC_COLLISION_TEST(820BD6E0, QuerySeparatedAfterMove(base, ctx.r3.u32, ctx.r1.u32 - 96, &result))
// f1 = the time.
ITC_COLLISION(820BBB20, uint32_t r3 = 0;
              ctx.f1.f64 = CollisionPairNextCheckTime(base, ctx.r3.u32, ctx.r1.u32 - 160, &r3);
              ctx.r3.u64 = r3)

// --- The impacts ---

// BoxBox::SwapForImpact, CornerImpacts and SwapForPushOut (sub_820BE490,
// sub_820BECA0, sub_820BEBF0) have no adapters: their callers in play are
// native.
// Reached through the table 0x82600EB4 (box-box); r3 comes back as the
// query + 64.
extern "C" REX_FUNC(sub_820BF038) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_collision)) {
    __imp__sub_820BF038(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!BoxBoxFindImpact(base, ctx.r3.u32)) {
    __imp__sub_820BF038(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 64;
}
ITC_COLLISION_TEST(820BCE80, QueryFindImpact(base, ctx.r3.u32, &result))

// --- The push-outs ---

// BoxBox::EdgesCrossing, PushOutEdges and PushOutCorners (sub_820BF080,
// sub_820BF320, sub_820BE150) have no adapters: their caller in play is
// native.
// Reached through the table 0x82600E90 (box-box).
extern "C" REX_FUNC(sub_820BFB28) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_collision)) {
    __imp__sub_820BFB28(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!BoxBoxFindPushOut(base, ctx.r3.u32, ctx.r1.u32 - 96, &r3)) {
    __imp__sub_820BFB28(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
// r4 = the direction.
ITC_COLLISION_TEST(820BCDD0, QueryFindPushOut(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112,
                                              &result))

// --- The pair ---

// CollisionPair::Respond (sub_820BB868) has no adapter: its caller in
// play, CollisionPair::Solve, is native.
ITC_COLLISION_TEST(820BBE30, CollisionPairSolve(base, ctx.r3.u32, ctx.r1.u32 - 1632, &result))

// --- The query and its contact ---

ITC_COLLISION(820BD1C8, QueryConstruct(base, ctx.r3.u32))
ITC_COLLISION(820BC848, QuerySetShapes(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// Contact::Set and Contact::Flip (sub_820BC908, sub_820BC858) have no
// adapters: their callers in play are native.

// --- The shapes' lifetime: only while the game's heap is the allocation
// layer's (operator new and delete called natively). ---

namespace {

bool NativeLifetime() {
  return ITC_FLAG(native_collision) && hooks::NativeHeapActive();
}

}  // namespace

// Slot 0: r4 = the flags; r3 comes back as the shape.
extern "C" REX_FUNC(sub_820BB140) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !ShapeDeletingDestructor(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820BB140(ctx, base);
  }
}
// Slot 5: r4 = memory or 0; r3 comes back as the copy (or 0).
extern "C" REX_FUNC(sub_820BB218) {
  REX_FUNC_PROLOGUE();
  uint32_t copy = 0;
  if (!NativeLifetime()) {
    __imp__sub_820BB218(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!BoxClone(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96, &copy)) {
    __imp__sub_820BB218(ctx, base);
    return;
  }
  ctx.r3.u64 = copy;
}
// r3 comes back as the old current copy (+8).
extern "C" REX_FUNC(sub_820BAAA0) {
  REX_FUNC_PROLOGUE();
  uint32_t r3 = 0;
  if (!NativeLifetime() || !CollisionShapeDestroy(base, ctx.r3.u32, &r3)) {
    __imp__sub_820BAAA0(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
// r4 = the template, r5 = the matrix, r6 = memory for the clones or 0; r3
// comes back as the record.
extern "C" REX_FUNC(sub_820BB580) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime()) {
    __imp__sub_820BB580(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!CollisionShapeSetUp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                           ctx.r1.u32 - 128)) {
    __imp__sub_820BB580(ctx, base);
  }
}
