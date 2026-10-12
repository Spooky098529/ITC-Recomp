// Frame pacing, in the usual fixed-timestep shape: game logic ticks at a
// fixed 30 Hz, and every pass of the game's main loop draws a frame.
//
// Each pass, the game posts an Update and a Draw message (see
// game_messages.h), then waits for the render thread in
// Renderer::WaitForFrame, swaps buffers and has the render thread draw the
// new recording. That wait is replaced to pace the passes: when the next
// 30 Hz tick is due, the coming pass runs normally (a logic pass);
// otherwise it waits for the next frame slot (per --max_fps) and the coming
// pass is render-only: Update messages are skipped, so it redraws without
// advancing the game. Everything else in the pass, including the game's own
// buffer swaps and resource handling, runs exactly as the game does.
//
// Render thread: draws exactly one frame each time it's asked (as the game
// originally did), without the game's 2-vblank wait.

#include "debug/frame_state_diff.h"
#include "debug/state_capture.h"
#include "frame_pacer.h"
#include "game_messages.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "interpolation/interpolator.h"

namespace {

using frame_pacing::GetFramePacer;

// Renderer object (see Renderer::Init, sub_82092358).
constexpr uint32_t kRendererTickDoneEventOffset = 388;   // set by logic after a tick
constexpr uint32_t kRendererFrameDoneEventOffset = 392;  // set to let logic tick
constexpr uint32_t kRendererRunningOffset = 396;         // u8

// Video output object passed to the vblank wait (renderer+64).
constexpr uint32_t kVideoOutputDeviceOffset = 140;

constexpr uint32_t kInfiniteTimeout = 0xFFFFFFFF;

// Waits for a game event; the timeout is in milliseconds (sub_82297D38
// converts it for NtWaitForSingleObjectEx). Returns whether it was set.
bool Event_Wait(PPCContext& ctx, uint8_t* base, uint32_t event,
                uint32_t timeout_ms = kInfiniteTimeout) {
  guest::StackFrame frame(ctx, 96);
  uint32_t timeout = frame.Local(80);
  guest::StoreU32(base, timeout, timeout_ms);
  ctx.r3.u64 = event;
  ctx.r4.u64 = timeout;
  sub_8258DD90(ctx, base);
  return ctx.r3.u32 != 0;
}

void Event_Signal(PPCContext& ctx, uint8_t* base, uint32_t event) {
  ctx.r3.u64 = event;
  sub_8258DD78(ctx, base);
}

void Renderer_DrawFrame(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  interpolation::BlendState_OnReplayFrameStart();
  interpolation::Interpolation_BeginFrame(GetFramePacer().TickProgress());
  ctx.r3.u64 = renderer;
  sub_82091E58(ctx, base);
}

bool Renderer_IsRunning(uint8_t* base, uint32_t renderer) {
  return guest::LoadU8(base, renderer + kRendererRunningOffset) != 0;
}

// Render thread: one frame per request from the main thread.
void Renderer_ThreadLoop(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  while (Renderer_IsRunning(base, renderer)) {
    Event_Wait(ctx, base, renderer + kRendererTickDoneEventOffset);
    debug::StateCapture_OnTickDone(base);
    interpolation::Interpolation_OnTickDone(base);
    Renderer_DrawFrame(ctx, base, renderer);
    Event_Signal(ctx, base, renderer + kRendererFrameDoneEventOffset);
  }
}

// Main thread, once per main-loop pass: the game's own wait for the frame in
// flight (and its deferred work), then pacing for the coming pass.
void Renderer_WaitForFrame(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  ctx.r3.u64 = renderer;
  __imp__sub_820920D8(ctx, base);

  auto& pacer = GetFramePacer();
  frame_pacing::GameMessages_OnPassFinished();
  bool render_only;
  if (frame_pacing::GameMessages_InRace()) {
    render_only = pacer.WaitForNextFrame();  // false once the next tick is due
  } else {
    // Menus run at the game's 30 fps (see game_messages.h).
    pacer.WaitForNextTick();
    render_only = false;
  }
  if (!render_only) {
    interpolation::BlendState_OnLogicPassStart();
    interpolation::BlendState_OnReplayTick();
    pacer.OnTickReleased();
  }
  pacer.OnFrameStarted();
  frame_pacing::GameMessages_SetRenderOnlyPass(render_only);
  debug::FrameStateDiff_OnPassStart(base, render_only);
}

void VideoOutput_WaitForVblank(PPCContext& ctx, uint8_t* base, uint32_t video_output) {
  // Keep the graphics device call that precedes the wait; drop the wait
  // itself, which held each frame for the game's 2-vblank interval.
  ctx.r3.u64 = guest::LoadU32(base, video_output + kVideoOutputDeviceOffset);
  sub_822A0718(ctx, base);
}

}  // namespace

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Renderer_ThreadLoop: r3 = renderer.
extern "C" REX_FUNC(sub_82091EF8) {
  REX_FUNC_PROLOGUE();
  uint32_t r1 = ctx.r1.u32;
  Renderer_ThreadLoop(ctx, base, ctx.r3.u32);
  ctx.r1.u32 = r1;
  ctx.r3.u64 = 0;
}

// Renderer_WaitForFrame: r3 = renderer. A tick handler on the main thread.
extern "C" REX_FUNC(sub_820920D8) {
  REX_FUNC_PROLOGUE();
  uint32_t r1 = ctx.r1.u32;
  Renderer_WaitForFrame(ctx, base, ctx.r3.u32);
  ctx.r1.u32 = r1;
}

// VideoOutput_WaitForVblank: r3 = video output.
extern "C" REX_FUNC(sub_8258DAA8) {
  REX_FUNC_PROLOGUE();
  VideoOutput_WaitForVblank(ctx, base, ctx.r3.u32);
}
