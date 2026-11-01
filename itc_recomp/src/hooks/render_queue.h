// The render queue's adapters' register steps (src/hooks/render_queue.cpp),
// shared with its harness (src/harnesses/render_queue.cpp).

#pragma once

#include "guest.h"

#include "graphics/render_queue.h"

namespace hooks {

// Guest calls for the render queue's plain C++, made through `ctx`
// (r1 and the registers given set, then put back; the flush mode turned off
// again after each, as the original's float code expects).
graphics::render_queue::GuestCall RenderQueueCall(PPCContext& ctx, uint8_t* base);

// The adapters' native paths, without the flag: r3 and the other registers
// as the original's.
void NativeRenderQueueClearRecording(PPCContext& ctx, uint8_t* base);
void NativeRenderQueueInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
