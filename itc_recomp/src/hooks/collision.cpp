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

ITC_FLAG_BOOL(native_collision, true, "ITC",
              "Run the collision world's narrow phase and its shapes "
              "(docs/functions/collision.md: the box's methods and accessors, sub_820B9EC8 to "
              "sub_820BAA50 and sub_822BE2C8; the shape record's accessors, sub_820BAB20, "
              "sub_820BAB30, sub_820BACB8, sub_820BADF8, sub_820BAE38; the query and its "
              "contact, sub_820BD1C8, sub_820BC848, sub_820BC908, sub_820BC858) natively "
              "instead of through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=collision)");

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
ITC_COLLISION(820B9FC8, ctx.r3.u64 = BoxEdgesStart(base, ctx.r3.u32))
ITC_COLLISION(820B9FE0, ctx.r3.u64 = BoxEdgesEnd(base, ctx.r3.u32))
ITC_COLLISION(820BA668, BoxSetCacheWord(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_COLLISION(820BA678, ctx.r3.u64 = BoxCacheWord(base, ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION(820BAA50, BoxCopyCache(base, ctx.r3.u32, ctx.r4.u32))
ITC_COLLISION(820BAB40, ctx.f1.f64 = ShapeRadius(base, ctx.r3.u32))
ITC_COLLISION(820BA368, ctx.r3.u64 = BoxLowerCorner(base, ctx.r3.u32, ctx.r4.u32))
// r5 = the point; r3 comes back as the face's normal.
ITC_COLLISION(820BA3B0, uint32_t r3 = 0;
              ctx.f1.f64 = BoxFaceDistance(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &r3);
              ctx.r3.u64 = r3)
ITC_COLLISION(820B9F00,
              ctx.r3.u64 = BoxFaceDirection(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 208))
// r4 = the offset; r3 comes back as the box + 48.
ITC_COLLISION(820BA2F0, ctx.f1.f64 = BoxTranslate(base, ctx.r3.u32, ctx.r4.u32);
              ctx.r3.u64 = ctx.r3.u32 + 48)
ITC_COLLISION(822BE2C8, ctx.r3.u64 = WordAt8(base, ctx.r3.u32))

// --- The shape record ---

ITC_COLLISION(820BAB20, ctx.r3.u64 = CollisionShapeCurrentCenter(base, ctx.r3.u32))
ITC_COLLISION(820BAB30, ctx.r3.u64 = CollisionShapePreviousCenter(base, ctx.r3.u32))
ITC_COLLISION_RECORD(820BACB8, CollisionShapeType(base, ctx.r3.u32, &value))
ITC_COLLISION_RECORD(820BADF8, CollisionShapePrevious(base, ctx.r3.u32, &value))
ITC_COLLISION_RECORD(820BAE38, CollisionShapeCurrent(base, ctx.r3.u32, &value))

// --- The query and its contact ---

ITC_COLLISION(820BD1C8, QueryConstruct(base, ctx.r3.u32))
ITC_COLLISION(820BC848, QuerySetShapes(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r4 = the point, r5 = the normal, f1 = the depth, f2 = the time; r3 comes
// back as the contact + 32.
ITC_COLLISION(820BC908,
              ContactSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.f2.f64);
              ctx.r3.u64 = ctx.r3.u32 + 32)
// r3 comes back as the contact + 32, f1 as -1.0.
ITC_COLLISION(820BC858, ctx.f1.f64 = ContactFlip(base, ctx.r3.u32);
              ctx.r3.u64 = ctx.r3.u32 + 32)
