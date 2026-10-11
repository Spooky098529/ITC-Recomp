// Frame pacing, in the usual fixed-timestep shape: game logic ticks at a
// fixed 30 Hz, and between ticks the main thread produces as many frames as
// the frame cap allows.
//
// Main thread, each pass of the game's loop: the game tick runs logic and
// its draw step (which records the scene), then waits for the render thread
// in Renderer::WaitForFrame. That wait is replaced: after the game's own
// wait, it re-runs the draw step for each extra frame (see draw_step.h),
// makes that recording the displayed one, has the render thread draw it,
// and waits, until the next tick is due. Nothing else runs meanwhile, so no
// frame ever sees a half-updated world.
//
// Render thread: draws exactly one frame each time it's asked (as the game
// originally did), without the game's 2-vblank wait.

#include "debug/state_capture.h"
#include "draw_step.h"
#include "frame_pacer.h"
#include "guest.h"
#include "interpolation/interpolator.h"
#include "interpolation/transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <chrono>

REXCVAR_DEFINE_DOUBLE(debug_extra_frame_camera_lift, 0.0, "ITC",
                      "Debug: raise the camera by this many metres in re-recorded extra "
                      "frames only (flickers if the draw step reads the camera matrix)");

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

// How long an extra frame may take before the frame loop gives up on extra
// frames for the tick (the render thread may be busy elsewhere, as during
// loading).
constexpr uint32_t kExtraFrameTimeoutMs = 1000;

// Main thread: has the render thread draw one frame and waits for it.
// Returns false if it didn't finish in time.
bool Renderer_DrawFrameAndWait(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  Event_Signal(ctx, base, renderer + kRendererTickDoneEventOffset);
  if (Event_Wait(ctx, base, renderer + kRendererFrameDoneEventOffset, kExtraFrameTimeoutMs)) {
    return true;
  }
  static int logged = 0;
  if (logged++ < 10) {
    REXLOG_WARN("frame loop: render thread didn't finish an extra frame within {} ms",
                kExtraFrameTimeoutMs);
  }
  return false;
}

// See "Known data" in docs/functions.md.
constexpr uint32_t kCameraViewMatrixAddress = 0x826BAC10;

interpolation::Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  interpolation::Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

void StoreMatrix(uint8_t* base, uint32_t address, const interpolation::Matrix4& m) {
  for (int i = 0; i < 16; i++) {
    guest::StoreFloat(base, address + 4 * i, m[i]);
  }
}

// Logs how long game ticks take, since no new frame can appear meanwhile.
class TickTimer {
 public:
  void OnTickStarted() { started_ = Clock::now(); }
  void OnTickFinished() {
    if (started_ == Clock::time_point{}) {
      return;
    }
    double ms = std::chrono::duration<double, std::milli>(Clock::now() - started_).count();
    total_ms_ += ms;
    worst_ms_ = std::max(worst_ms_, ms);
    if (++ticks_ == 30) {
      REXLOG_INFO("frame loop: game tick took {:.1f} ms on average, {:.1f} ms at worst",
                  total_ms_ / ticks_, worst_ms_);
      ticks_ = 0;
      total_ms_ = worst_ms_ = 0;
    }
  }

 private:
  using Clock = std::chrono::steady_clock;
  Clock::time_point started_{};
  double total_ms_ = 0, worst_ms_ = 0;
  int ticks_ = 0;
};

TickTimer& GetTickTimer() {
  static TickTimer timer;
  return timer;
}

// Records one extra frame from the current game memory, with the debug
// camera lift applied, and has it drawn. Returns false if the render thread
// didn't respond.
bool DrawExtraFrame(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  float lift = float(REXCVAR_GET(debug_extra_frame_camera_lift));
  interpolation::Matrix4 camera = LoadMatrix(base, kCameraViewMatrixAddress);
  if (lift != 0) {
    if (auto transform = interpolation::TransformFromViewMatrix(camera)) {
      transform->position[1] += lift;
      StoreMatrix(base, kCameraViewMatrixAddress, interpolation::ViewMatrixFromTransform(*transform));
    }
  }
  frame_pacing::DrawStep_Rerecord(ctx, base);
  StoreMatrix(base, kCameraViewMatrixAddress, camera);

  frame_pacing::DrawStep_ShowRecording(base);
  bool drawn = Renderer_DrawFrameAndWait(ctx, base, renderer);
  frame_pacing::DrawStep_HideRecording(base);
  return drawn;
}

// Main thread, after the game tick: extra frames until the next tick is due,
// then the game's own wait.
//
// The game's wait (sub_820920D8) waits for the frame in flight and then runs
// deferred work, which frees objects that frame used. Extra frames must come
// before that, while those objects are still alive, so the frame in flight is
// waited for here first, and the frame-done event is set again afterwards so
// the game's own wait goes straight through to its deferred work.
void Renderer_WaitForFrame(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  GetTickTimer().OnTickFinished();
  Event_Wait(ctx, base, renderer + kRendererFrameDoneEventOffset);

  auto& pacer = GetFramePacer();
  while (Renderer_IsRunning(base, renderer) && pacer.WaitForNextFrame()) {
    pacer.OnFrameStarted();
    bool drawn;
    if (frame_pacing::DrawStep_CanRerecord()) {
      drawn = DrawExtraFrame(ctx, base, renderer);
    } else {
      // No draw step this tick (a menu): redraw what's displayed.
      drawn = Renderer_DrawFrameAndWait(ctx, base, renderer);
    }
    if (!drawn) {
      break;
    }
  }
  // Leave this tick's recording exactly as the game made it.
  frame_pacing::DrawStep_FinishExtraFrames(base);

  Event_Signal(ctx, base, renderer + kRendererFrameDoneEventOffset);
  ctx.r3.u64 = renderer;
  __imp__sub_820920D8(ctx, base);
  pacer.OnTickReleased();
  pacer.OnFrameStarted();  // the game's own frame for this tick follows
  GetTickTimer().OnTickStarted();
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
