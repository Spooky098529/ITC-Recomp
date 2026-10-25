// The video output's device lock natively: adapters in front of
// src/graphics/video_output.h (docs/functions/video-output.md). Each leaves
// r3 as the original does (the return value, or r3 as it came for
// ReleaseThreadOwnership), except UnlockDevice, which has none and leaves r3
// as it came where the original leaves Semaphore::Signal's leftover (its
// callers don't read it). The flag is read by each adapter: the constructor and the
// lock and unlock switch together (a lock made by one side is used only by
// it), so it's meant to be set at launch, not changed while the game runs.

#include "guest.h"

#include "base/flags.h"
#include "graphics/video_output.h"

ITC_FLAG_BOOL(native_video_output, true, "ITC",
              "Run the video output's device lock (docs/functions/video-output.md: "
              "VideoOutput::VideoOutput sub_8258DB18, VideoOutput::LockDevice sub_8258D8C8, "
              "VideoOutput::UnlockDevice sub_8258D908, over a host std::binary_semaphore in "
              "place of the game's semaphore at +224; Device::AcquireThreadOwnership "
              "sub_8229D9D0, Device::ReleaseThreadOwnership sub_8229DA10, the current "
              "thread's id sub_825B9A60) natively instead of through the translated code (the "
              "same results, the semaphore's bytes aside; checked by "
              "--run_harnesses=video_output). Set at launch: the constructor and the lock "
              "switch together.");

namespace vo = graphics::video_output;

#define ITC_VIDEO_OUTPUT(address, ...)       \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_video_output)) {    \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    __VA_ARGS__;                             \
  }

ITC_VIDEO_OUTPUT(825B9A60, ctx.r3.u64 = vo::CurrentThreadId())
ITC_VIDEO_OUTPUT(8229D9D0, ctx.r3.u64 = vo::AcquireThreadOwnership(base, ctx.r3.u32))
ITC_VIDEO_OUTPUT(8229DA10, vo::ReleaseThreadOwnership(base, ctx.r3.u32))
ITC_VIDEO_OUTPUT(8258DB18, ctx.r3.u64 = vo::Construct(base, ctx.r3.u32))
ITC_VIDEO_OUTPUT(8258D8C8, ctx.r3.u64 = vo::LockDevice(base, ctx.r3.u32))
ITC_VIDEO_OUTPUT(8258D908, vo::UnlockDevice(base, ctx.r3.u32, ctx.r4.u32))
