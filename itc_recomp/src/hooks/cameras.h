// The camera passes' adapters' shared work (src/hooks/cameras.cpp), for the
// wrappers elsewhere that keep their own diagnostics around it.

#pragma once

#include "guest.h"

namespace hooks {

// CameraPass::Update (sub_820A3978) natively (src/cameras/cameras.cpp) for
// its wrapper in src/frame_pacing/per_frame_logic.cpp: r3 = the pass. Its
// viewport recording's guest calls (the scene's getters and the render
// queue) made as the original's, from frames like its own. False, with
// nothing done, where the original must run instead (--native_cameras off,
// a stack pointer not 16-byte aligned, a camera whose vtable slot 0 isn't
// Camera::SetViewportSize).
bool CameraPassUpdate(PPCContext& ctx, uint8_t* base);

// CameraPass::RecordViewport (sub_820A0978)'s adapter's work, without the
// flag: r3 = the pass, r4 = the sort key (for the harness).
void CameraPassRecordViewport(PPCContext& ctx, uint8_t* base);

// RecordClear (sub_8209FA38)'s adapter's work, without the flag: r4 = the
// key, r5 = the flags, r6 = the color, r7 = the depth (for the harness).
void RecordClear(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
