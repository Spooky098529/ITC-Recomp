// The model animation natively: adapters in front of
// src/model_animation/model_animation.cpp (docs/functions/model-animation.md).
// The track sets' accessors, sampling and stepping (sub_825A6670,
// sub_825A6690, sub_825A66D8, sub_825A1F08, sub_825A1F50, sub_825A2000,
// sub_825A2030, sub_825A1F98, sub_825A66F8, sub_825A1C88, sub_825A1D68, and
// the +364 set's sub_825A0FE0 and sub_825A10A8) have no adapters: only the
// plain C++ below calls them in play (the flag switches their callers). The
// ones that free or allocate run natively only while --native_heap routes
// the game's heap to the allocation layer, and hand the paths left to the
// original (free's and operator new's errors, Setup208 with tracks) back to
// it from the start, nothing having been done. SkeletonCursor::View
// (sub_825A2090), the node matrices' update (sub_8259C3A8, models.md) and
// the node channels' sampler (sub_825A20E8) have no adapters either: their
// only callers in play are native.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "model_animation/model_animation.h"

ITC_FLAG_BOOL(native_model_animation, true, "ITC",
              "Run the model animation (docs/functions/model-animation.md: Model::Animate, "
              "sub_82594B60; the track sets' constructor, release and set-ups, sub_825A1B70, "
              "sub_825A11D0, sub_825A1BA8, sub_825A1E08, sub_825A1BE8, sub_825A1218 and "
              "sub_825A1A38; their accessors, sampling and stepping, from sub_825A6670 to "
              "sub_825A10A8; the skeleton's constructors, destructors and frees, sub_82599758, "
              "sub_8259A140, sub_825999C8, sub_8259A9D0, sub_8259A228 and sub_825997A0; its "
              "cursor, node values, matrices and animation, sub_825A2068, sub_825A2090, "
              "sub_825997E0, sub_82599880, sub_825998A0, sub_8259C3A8, sub_825998B8 and "
              "sub_825A20E8; the primitive drawer's constructor, sub_825A1378; sub_825A18D8) "
              "natively instead of through the translated code (same results; "
              "checked by --run_harnesses=model_animation)");

using namespace model_animation;

namespace {

// The functions that free or allocate: only over the allocation layer.
bool NativeLifetime() {
  return ITC_FLAG(native_model_animation) && hooks::NativeHeapActive();
}

}  // namespace

#define ITC_MODEL_ANIMATION_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                        \
  if (!ITC_FLAG(native_model_animation)) {    \
    __imp__sub_##address(ctx, base);          \
    return;                                   \
  }

// Model::Animate: r3 = the model; r3 comes back as the original leaves it.
extern "C" REX_FUNC(sub_82594B60) {
  ITC_MODEL_ANIMATION_FALLBACK(82594B60)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ModelAnimate(base, ctx.r3.u32);
}

// TrackSet::TrackSet: r3 = the set, returned.
extern "C" REX_FUNC(sub_825A1B70) {
  ITC_MODEL_ANIMATION_FALLBACK(825A1B70)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = TrackSetConstruct(base, ctx.r3.u32);
}

// The release and set-ups: r3 = the set (r4 = the section).
#define ITC_MODEL_ANIMATION_LIFETIME(address, call)  \
  extern "C" REX_FUNC(sub_##address) {               \
    REX_FUNC_PROLOGUE();                             \
    if (NativeLifetime()) {                          \
      ctx.fpscr.disableFlushMode();                  \
      if (call) {                                    \
        return;                                      \
      }                                              \
    }                                                \
    __imp__sub_##address(ctx, base);                 \
  }

ITC_MODEL_ANIMATION_LIFETIME(825A11D0, TrackSetReleaseArray(base, ctx.r3.u32))
ITC_MODEL_ANIMATION_LIFETIME(825A1BA8, TrackSetRelease(base, ctx.r3.u32))
ITC_MODEL_ANIMATION_LIFETIME(825A1E08, TrackSetSetupFloat4(base, ctx.r3.u32, ctx.r4.u32))
ITC_MODEL_ANIMATION_LIFETIME(825A1BE8, TrackSetSetupFloat(base, ctx.r3.u32, ctx.r4.u32))
ITC_MODEL_ANIMATION_LIFETIME(825A1218, TrackSetSetup32(base, ctx.r3.u32, ctx.r4.u32))
ITC_MODEL_ANIMATION_LIFETIME(825A1A38, TrackSetSetup208(base, ctx.r3.u32, ctx.r4.u32))

// The skeleton's destructors and frees: r3 = the skeleton.
ITC_MODEL_ANIMATION_LIFETIME(8259A228, SkeletonBaseFree(base, ctx.r3.u32))
ITC_MODEL_ANIMATION_LIFETIME(8259A9D0, SkeletonBaseDestruct(base, ctx.r3.u32))
ITC_MODEL_ANIMATION_LIFETIME(825999C8, SkeletonDestruct(base, ctx.r3.u32))
ITC_MODEL_ANIMATION_LIFETIME(825997A0, SkeletonFreeNodeValues(base, ctx.r3.u32))

// The constructors: r3 = the object, returned.
extern "C" REX_FUNC(sub_8259A140) {
  ITC_MODEL_ANIMATION_FALLBACK(8259A140)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = SkeletonBaseConstruct(base, ctx.r3.u32);
}
extern "C" REX_FUNC(sub_82599758) {
  ITC_MODEL_ANIMATION_FALLBACK(82599758)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = SkeletonConstruct(base, ctx.r3.u32);
}
// PrimitiveDrawer::PrimitiveDrawer: operator new(4), so only over the
// allocation layer; its error path is the original's.
extern "C" REX_FUNC(sub_825A1378) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime() && PrimitiveDrawerConstruct(base, ctx.r3.u32)) {
    return;  // r3: the drawer, as given
  }
  __imp__sub_825A1378(ctx, base);
}

// SkeletonCursor::Clear and sub_825A18D8: r3 = the cursor or record.
extern "C" REX_FUNC(sub_825A2068) {
  ITC_MODEL_ANIMATION_FALLBACK(825A2068)
  ctx.fpscr.disableFlushMode();
  SkeletonCursorClear(base, ctx.r3.u32);
}
extern "C" REX_FUNC(sub_825A18D8) {
  ITC_MODEL_ANIMATION_FALLBACK(825A18D8)
  ctx.fpscr.disableFlushMode();
  RecordClear(base, ctx.r3.u32);
}

// Skeleton::LoadNodeValues: r3 = the skeleton, r4 = the section.
extern "C" REX_FUNC(sub_825997E0) {
  ITC_MODEL_ANIMATION_FALLBACK(825997E0)
  ctx.fpscr.disableFlushMode();
  SkeletonLoadNodeValues(base, ctx.r3.u32, ctx.r4.u32);
}
// Skeleton::NodeCount: r3 = the skeleton; returns the count.
extern "C" REX_FUNC(sub_82599880) {
  ITC_MODEL_ANIMATION_FALLBACK(82599880)
  ctx.r3.u64 = SkeletonNodeCount(base, ctx.r3.u32);
}
// Skeleton::UpdateMatrices: r3 = the skeleton. Flush-to-zero is left on
// after the multiplies, as the translated Matrix::Multiply leaves it.
extern "C" REX_FUNC(sub_825998A0) {
  ITC_MODEL_ANIMATION_FALLBACK(825998A0)
  switch (SkeletonUpdateMatrices(base, ctx.r3.u32)) {
    case MatricesResult::kNothing:
      return;
    case MatricesResult::kMultiplied:
      ctx.fpscr.enableFlushMode();
      return;
    case MatricesResult::kOriginal:
      __imp__sub_825998A0(ctx, base);
      return;
  }
}
// Skeleton::ApplyAnimation: r3 = the skeleton.
extern "C" REX_FUNC(sub_825998B8) {
  ITC_MODEL_ANIMATION_FALLBACK(825998B8)
  ctx.fpscr.disableFlushMode();
  SkeletonApplyAnimation(base, ctx.r3.u32);
}
