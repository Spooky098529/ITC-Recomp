// The camera passes' adapters' shared work (src/hooks/cameras.cpp), for the
// wrappers elsewhere that keep their own diagnostics around it.

#pragma once

#include "guest.h"

#include <optional>

#include <array>
#include <vector>

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

// CameraPasses::MakeRenderTarget (sub_8209FBF0) natively, without the
// flag: r3 = the id; from a frame like the original's (112 bytes), the video
// output (`[0x8266D088]`'s slots), CameraPasses::RenderTarget (through its
// override, the diagnostics' wrapper) and Resource::Release as guest calls
// with its return addresses; r3 the video output, as the original's last
// call leaves it. False, with nothing done, where the original must run
// (--native_video_output or --native_graphics_resources off, or the
// allocation layer not routing the heaps).
bool CameraPassesMakeRenderTarget(PPCContext& ctx, uint8_t* base);

// CameraPass::SetUpSurfaces (sub_820A2F40) natively, without the flag:
// r3 = the pass; from a frame like the original's (544 bytes), the video
// output's slots, CameraPasses::RenderTarget, MakeRenderTarget and
// CameraPass::ReleaseSurfaces (through their overrides) and the cube map's
// surface (sub_822A5C50, recompiled) as guest calls. r3 is left as the last
// guest call leaves it (the video output; the original's last call is
// UnlockDevice, after the same slot calls; no caller reads it). False, with
// nothing done, as CameraPassesMakeRenderTarget.
bool CameraPassSetUpSurfaces(PPCContext& ctx, uint8_t* base);

// CameraPass::Init (sub_820A3DB0) natively, without the flag: r3 = the
// pass, r4 = the type; from a frame like the original's (144 bytes),
// Camera::Camera, the video output's slots (type 0), CameraPass::Update and
// SetUpSurfaces (through their overrides) as guest calls; r3 = 0.
// Flush-to-zero left off. Always runs (true).
bool CameraPassInit(PPCContext& ctx, uint8_t* base);

// For the cameras harness: while not null, CameraPass::Init's adapter only
// appends its r3, r4 and r5 here (and leaves r3 = 0; whatever the flag), so
// the original CameraPasses::InitPasses can be run without touching the
// game's passes.
extern thread_local std::vector<std::array<uint32_t, 3>>* camera_pass_init_calls;

// CameraPasses::Init (sub_8209F9B8, `all`) or InitPasses (sub_8209F960)
// natively, without the flag: from a frame like InitPasses' (128 bytes),
// CameraPass::Init through its override with InitPasses' return address;
// r3 and f1 as the original's last call (SetMode) leaves them.
void CameraPassesInit(PPCContext& ctx, uint8_t* base, bool all);

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

// Viewports::Init (sub_820AB358) natively, without the flag: from frames
// like the originals' (112 and Viewport::Init's 160 below it), the video
// output's slots as guest calls with Viewport::Init's return addresses, and
// atexit through the game's only where the native one can't. Returns the
// second viewport (r3 left for the caller); nullopt, with nothing done,
// where the original must run (cameras::ViewportsInit's).
std::optional<uint32_t> ViewportsInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
