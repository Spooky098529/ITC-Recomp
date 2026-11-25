// The frame driver's and the app's scene's register steps
// (src/frame_driver/, docs/functions/frame-driver.md), shared by the
// adapters (src/hooks/frame_driver.cpp, behind
// --native_frame_driver) and the harness (without the flag). Each
// leaves r3 as the original does and runs the original where the plain C++
// refuses.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Whether --native_frame_driver is on.
bool NativeFrameDriver();

// FrameDriver::GetVideoOutput sub_824960E0: r3 += 44.
void FrameDriverGetVideoOutput(lifted::Regs& ctx);
// FrameDriver::AddListener sub_820922B8: r3 = the interface, r4 = the key's
// address, r5 = the delegate; from a 112-byte frame (the request at its
// +80); r3 as PushBack leaves it.
void FrameDriverAddListener(lifted::Regs& ctx);
// FrameDriver::ApplyRequest sub_82091FF0: r3 = the frame driver, r4 the type
// (high word) and key, r5 the delegate (high word); stored at the caller's
// r1 + 32 and + 40; r3 r1 - 32 after an insert, else left.
void FrameDriverApplyRequest(lifted::Regs& ctx);
// FrameDriver::CallListener sub_82091A98: r3 = the frame driver, r4 = the entry
// (key, delegate); the delegate's slot 1 from a 96-byte frame; its r3.
void FrameDriverCallListener(lifted::Regs& ctx);
// FrameDriver::DrawFrame sub_82091E58: r3 = the frame driver; from a 144-byte
// frame. No adapter: its one caller, FrameDriver::ThreadLoop's override
// (src/frame_pacing/renderer_hooks.cpp), calls this, which runs the
// original without the flags.
void FrameDriverDrawFrame(lifted::Regs& ctx);

// Scene::Update sub_820988C8: r3 = the scene; from a 112-byte frame. No
// adapter of its own: its wrap (--log_boot_modes, src/hooks/title_screen.cpp)
// calls this with the flag, else the original.
void AppSceneUpdate(lifted::Regs& ctx);
// Scene::Draw sub_82098940: r3 = the scene, r4 = the frame driver's interface,
// r5 = the device; from a 112-byte frame; r3 the video output.
void AppSceneDraw(lifted::Regs& ctx);

// Thread::Thread sub_8258DE88: r3 = the thread object, left.
void ThreadConstruct(lifted::Regs& ctx);
// Thread::Begin sub_8258DF18: r3 = the thread object, r4 = the object to
// run (0: the thread), r5 and r6 the processor's and priority index's
// addresses; r3 as the original leaves it.
void ThreadBegin(lifted::Regs& ctx);
// The wrappers' register steps, for the harness (no adapters: only
// Thread::Begin's plain C++ calls them in play): CreateThread sub_825BA380
// (r4 the stack size, r5 the entry, r6 its parameter, r7 the flags, r8 the
// processor, r9 the id's address), SetThreadPriority sub_822977B0 (r3 a
// handle, r4 a priority), XSetThreadProcessor sub_82297888 (r3 a handle, r4
// a processor) and ResumeThread sub_825BA118 (r3 a handle); r3 the answer.
void CreateThreadStep(lifted::Regs& ctx);
void SetThreadPriorityStep(lifted::Regs& ctx);
void XSetThreadProcessorStep(lifted::Regs& ctx);
void ResumeThreadStep(lifted::Regs& ctx);

// VideoOutput::UnbindAll sub_8258D9F8: r3 = the video output; from a
// 112-byte frame; r3 as the last setter leaves it.
void VideoOutputUnbindAll(lifted::Regs& ctx);

// Scene::Scene sub_82098A98: r3 = the scene, r4 = the block; from a
// 192-byte frame; r3 the scene. Its callees' register steps, for the
// harness (no adapters: only Scene::Scene's plain C++ calls them in play):
// CommandList::Init sub_8258CD28 (r3 the list, r4 the slots; r3 1),
// ColorRect::SetTargets sub_82178B28 (r3 the queue, r4 the list),
// DrawGlobals::Init sub_82589E78 (r4 the capacity, r5 the count) and
// PrimitiveDecl::Init sub_82591858 (r3 1).
void SceneConstruct(lifted::Regs& ctx);
void CommandListInitStep(lifted::Regs& ctx);
void ColorRectSetTargetsStep(lifted::Regs& ctx);
void DrawGlobalsInitStep(lifted::Regs& ctx);
void PrimitiveDeclInitStep(lifted::Regs& ctx);

}  // namespace hooks
