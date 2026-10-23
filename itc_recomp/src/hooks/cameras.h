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

// Camera::Update (sub_82593568) and Camera::Camera (sub_82593908)
// natively, without the flag: r3 = the camera; from frames like the
// originals' (320 and 128 bytes). False, with nothing done, where the
// original must run (a stack pointer not 16-byte aligned).
bool CameraUpdate(PPCContext& ctx, uint8_t* base);
bool CameraConstruct(PPCContext& ctx, uint8_t* base);

// CameraPass::RecordBegin (sub_820A0CC0) and RecordEnd (sub_820A1C30)
// natively, without the flag: r3 = the pass; the scene's getters and the
// render queue called as the originals do, from frames like theirs, and
// RecordEnd's CameraPasses::RenderTarget through its override (the
// diagnostics' wrapper). False where the original must run (the command
// list's diagnostics on, or a chunk its calls would print about), r3 put
// back.
bool CameraPassRecordBegin(PPCContext& ctx, uint8_t* base);
bool CameraPassRecordEnd(PPCContext& ctx, uint8_t* base);

// CameraPasses::RecordBegins (sub_8209F8A0) and RecordEnds (sub_8209F900)
// natively, for their wrappers in src/debug/frame_state_diff.cpp: each
// enabled pass recorded as above (the original RecordBegin or RecordEnd
// where it must run), from frames like the originals'.
void CameraPassesRecordBegins(PPCContext& ctx, uint8_t* base);
void CameraPassesRecordEnds(PPCContext& ctx, uint8_t* base);

// CameraPass::RecordViewport (sub_820A0978)'s adapter's work, without the
// flag: r3 = the pass, r4 = the sort key (for the harness).
void CameraPassRecordViewport(PPCContext& ctx, uint8_t* base);

// RecordClear (sub_8209FA38)'s adapter's work, without the flag: r4 = the
// key, r5 = the flags, r6 = the color, r7 = the depth (for the harness).
void RecordClear(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
