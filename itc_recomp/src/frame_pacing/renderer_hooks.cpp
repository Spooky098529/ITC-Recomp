// Replaces the game's render thread pacing so frames are no longer tied to
// game logic ticks.
//
// Originally the render thread drew exactly one frame per tick and present
// waited for 2 vblanks, which locked everything to 30 fps. Now, after each
// tick, the render thread draws frames until the next tick is due, then
// releases game logic. Logic stays paused while frames are drawn, as before,
// so drawing never sees a half-updated scene.

#include "frame_pacer.h"
#include "guest.h"

namespace {

using frame_pacing::GetFramePacer;

// Renderer object (see Renderer::Init, sub_82092358).
constexpr uint32_t kRendererTickDoneEventOffset = 388;   // set by logic after a tick
constexpr uint32_t kRendererFrameDoneEventOffset = 392;  // set to let logic tick
constexpr uint32_t kRendererRunningOffset = 396;         // u8

// Video output object passed to the vblank wait (renderer+64).
constexpr uint32_t kVideoOutputDeviceOffset = 140;

constexpr uint32_t kInfiniteTimeout = 0xFFFFFFFF;

void Event_Wait(PPCContext& ctx, uint8_t* base, uint32_t event) {
  guest::StackFrame frame(ctx, 96);
  uint32_t timeout = frame.Local(80);
  guest::StoreU32(base, timeout, kInfiniteTimeout);
  ctx.r3.u64 = event;
  ctx.r4.u64 = timeout;
  sub_8258DD90(ctx, base);
}

void Event_Signal(PPCContext& ctx, uint8_t* base, uint32_t event) {
  ctx.r3.u64 = event;
  sub_8258DD78(ctx, base);
}

void Renderer_DrawFrame(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  GetFramePacer().OnFrameStarted();
  ctx.r3.u64 = renderer;
  sub_82091E58(ctx, base);
}

bool Renderer_IsRunning(uint8_t* base, uint32_t renderer) {
  return guest::LoadU8(base, renderer + kRendererRunningOffset) != 0;
}

void Renderer_ThreadLoop(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  auto& pacer = GetFramePacer();
  while (Renderer_IsRunning(base, renderer)) {
    Event_Wait(ctx, base, renderer + kRendererTickDoneEventOffset);

    Renderer_DrawFrame(ctx, base, renderer);
    while (pacer.WaitForNextFrame()) {
      Renderer_DrawFrame(ctx, base, renderer);
    }

    pacer.OnTickReleased();
    Event_Signal(ctx, base, renderer + kRendererFrameDoneEventOffset);
  }
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

// VideoOutput_WaitForVblank: r3 = video output.
extern "C" REX_FUNC(sub_8258DAA8) {
  REX_FUNC_PROLOGUE();
  VideoOutput_WaitForVblank(ctx, base, ctx.r3.u32);
}
