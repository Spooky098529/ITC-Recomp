// The frame driver's and the app's scene's register steps
// (src/frame_driver/, docs/functions/frame-driver.md), shared by the
// adapters (src/hooks/frame_driver.cpp, behind
// --native_frame_driver) and the harness (without the flag). Each
// leaves r3 as the original does and runs the original where the plain C++
// refuses.

#pragma once

#include "guest.h"

namespace hooks {

// Whether --native_frame_driver is on.
bool NativeFrameDriver();

// FrameDriver::GetVideoOutput sub_824960E0: r3 += 44.
void FrameDriverGetVideoOutput(PPCContext& ctx, uint8_t* base);
// FrameDriver::AddListener sub_820922B8: r3 = the interface, r4 = the key's
// address, r5 = the delegate; from a 112-byte frame (the request at its
// +80); r3 as PushBack leaves it.
void FrameDriverAddListener(PPCContext& ctx, uint8_t* base);
// FrameDriver::ApplyRequest sub_82091FF0: r3 = the frame driver, r4 the type
// (high word) and key, r5 the delegate (high word); stored at the caller's
// r1 + 32 and + 40; r3 r1 - 32 after an insert, else left.
void FrameDriverApplyRequest(PPCContext& ctx, uint8_t* base);
// FrameDriver::CallListener sub_82091A98: r3 = the frame driver, r4 = the entry
// (key, delegate); the delegate's slot 1 from a 96-byte frame; its r3.
void FrameDriverCallListener(PPCContext& ctx, uint8_t* base);
// FrameDriver::DrawFrame sub_82091E58: r3 = the frame driver; from a 144-byte
// frame. No adapter: its one caller, FrameDriver::ThreadLoop's override
// (src/frame_pacing/renderer_hooks.cpp), calls this, which runs the
// original without the flags.
void FrameDriverDrawFrame(PPCContext& ctx, uint8_t* base);

// Scene::Update sub_820988C8: r3 = the scene; from a 112-byte frame. No
// adapter of its own: its wrap (--log_boot_modes, src/hooks/title_screen.cpp)
// calls this with the flag, else the original.
void AppSceneUpdate(PPCContext& ctx, uint8_t* base);
// Scene::Draw sub_82098940: r3 = the scene, r4 = the frame driver's interface,
// r5 = the device; from a 112-byte frame; r3 the video output.
void AppSceneDraw(PPCContext& ctx, uint8_t* base);

// Thread::Thread sub_8258DE88: r3 = the thread object, left.
void ThreadConstruct(PPCContext& ctx, uint8_t* base);
// Thread::Begin sub_8258DF18: r3 = the thread object, r4 = the object to
// run (0: the thread), r5 and r6 the processor's and priority index's
// addresses; r3 as the original leaves it.
void ThreadBegin(PPCContext& ctx, uint8_t* base);
// The wrappers' register steps, for the harness (no adapters: only
// Thread::Begin's plain C++ calls them in play): CreateThread sub_825BA380
// (r4 the stack size, r5 the entry, r6 its parameter, r7 the flags, r8 the
// processor, r9 the id's address), SetThreadPriority sub_822977B0 (r3 a
// handle, r4 a priority), XSetThreadProcessor sub_82297888 (r3 a handle, r4
// a processor) and ResumeThread sub_825BA118 (r3 a handle); r3 the answer.
void CreateThreadStep(PPCContext& ctx, uint8_t* base);
void SetThreadPriorityStep(PPCContext& ctx, uint8_t* base);
void XSetThreadProcessorStep(PPCContext& ctx, uint8_t* base);
void ResumeThreadStep(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
