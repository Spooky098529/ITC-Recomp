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

// Job::Wait (sub_82094B10; r3 = the job): natively with
// --native_frame_passes (the object's slot 1 a guest call from a 96-byte
// frame), else the original. --find_menu_selection's wrap
// (src/debug/menu_selection_scan.cpp) calls it.
void FramePassesJobWait(PPCContext& ctx, uint8_t* base);

// FramePasses::Boot (sub_820962B8; r3 = it): natively with
// --native_frame_passes and --native_video_output, else the original.
void FramePassesBoot(PPCContext& ctx, uint8_t* base);

// FramePasses::FramePasses (sub_82098630; r3 = it, r4 = the block):
// natively with --native_frame_passes, else the original.
void FramePassesConstruct(PPCContext& ctx, uint8_t* base);

// FramePasses::Record (sub_82096418; r3 = it): natively with
// --native_frame_passes (and without the command list's diagnostics), else
// the original (also where the plain C++ declines).
void FramePassesRecord(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
