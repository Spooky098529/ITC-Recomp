// The render queue's adapters' register steps (src/hooks/render_queue.cpp),
// shared with its harness (src/harnesses/render_queue.cpp).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include "graphics/render_queue.h"

namespace hooks {

// Guest calls for the render queue's plain C++, made through `ctx`
// (r1 and the registers given set, then put back; the flush mode turned off
// again after each, as the original's float code expects).
graphics::render_queue::GuestCall RenderQueueCall(lifted::Regs& ctx);

// The adapters' native paths, without the flag: r3 and the other registers
// as the original's.
void NativeRenderQueueClearRecording(lifted::Regs& ctx);
void NativeRenderQueueInit(lifted::Regs& ctx);
// Add, with the original from the start for the warning's path.
void NativeRenderQueueAdd(lifted::Regs& ctx);
void NativeRenderQueueReplay(lifted::Regs& ctx);

}  // namespace hooks
