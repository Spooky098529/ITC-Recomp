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
// it from the start, nothing having been done.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "model_animation/model_animation.h"

ITC_FLAG_BOOL(native_model_animation, true, "ITC",
              "Run the model animation (docs/functions/model-animation.md: Model::Animate, "
              "sub_82594B60; the track sets' constructor, release and set-ups, sub_825A1B70, "
              "sub_825A11D0, sub_825A1BA8, sub_825A1E08, sub_825A1BE8, sub_825A1218 and "
              "sub_825A1A38; their accessors, sampling and stepping, from sub_825A6670 to "
              "sub_825A10A8) natively instead of through the translated code (same results; "
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
