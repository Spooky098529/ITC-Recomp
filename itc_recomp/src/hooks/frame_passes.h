// The frame passes' register steps that make guest calls, shared by the
// adapters (src/hooks/frame_passes.cpp) and the harness (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// Whether the frame passes run natively: --native_frame_passes, without the
// command list's diagnostics (they name who allocates slots by the return
// address).
bool NativeFramePasses();

// FramePasses::RecordPrimitiveReset (sub_82589990): no arguments. The chunk
// natively, handed to the render queue at [0x8296B91C] (its slot 4, bucket
// 0, f1 = 0.0) from a 128-byte frame like the original's; r3 as that call
// leaves it. The original where the plain C++ declines.
void FramePassesRecordPrimitiveReset(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
