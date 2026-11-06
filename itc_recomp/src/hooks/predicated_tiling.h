// Predicated tiling's adapters' register steps (src/hooks/predicated_tiling.cpp),
// shared with the wraps in src/native/gpu_link.cpp and the harness
// (src/harnesses/predicated_tiling.cpp).

#pragma once

#include "guest.h"

#include "graphics/predicated_tiling.h"

namespace hooks {

// The tiling's guest calls through `ctx`: the segment's end (sub_8229EF48,
// its override) and the deferred kick (hooks::CommandBufferKick).
graphics::predicated_tiling::GuestCalls PredicatedTilingCalls(PPCContext& ctx, uint8_t* base);

// Device::EndTiledSegments (sub_822B6BF8): the plain C++ with
// --native_predicated_tiling (the original with it off, or with a recording
// command buffer). For its wrap in src/native/gpu_link.cpp, which logs.
void EndTiledSegments(PPCContext& ctx, uint8_t* base);

// The worker threads' guest calls through `ctx` (the kernel's imports).
graphics::predicated_tiling::WorkerCalls PredicatedTilingWorkerCalls(PPCContext& ctx,
                                                                     uint8_t* base);

// Device::CreateWorkers (sub_822B6D70) without the flag: r3 as the original's.
void NativeCreateWorkers(PPCContext& ctx, uint8_t* base);

// The worker thread's guest calls through `ctx`.
graphics::predicated_tiling::WorkerLoopCalls PredicatedTilingWorkerLoopCalls(PPCContext& ctx,
                                                                             uint8_t* base);

// Device::WorkerThread (sub_822B69A8) without the flag: r3 as the original's.
void NativeWorkerThread(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
