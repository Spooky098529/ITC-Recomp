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
#include "debug/idle_timer_scan.h"
#include "debug/image_dump.h"
#include "debug/replay_stats.h"
#include "debug/state_capture.h"
#include "frame_pacer.h"
#include "game_messages.h"
#include "guest.h"
#include "hooks/title_screen.h"
#include "interpolation/blend_state.h"
#include "native/renderer.h"
#include <algorithm>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <rex/logging.h>
#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(log_frame_times, false, "ITC",
                    "Log where frame time goes (main thread work, waits, render thread "
                    "drawing and presenting, blending), every 2 s");

namespace {

using frame_pacing::GetFramePacer;

// Diagnostics (--log_frame_times): durations of each part of a frame, as
// average and worst over 2 s.
class FrameTimes {
 public:
  using Clock = std::chrono::steady_clock;
  enum Part {
    kLogicWork,       // main thread: a logic pass's work (tick handlers)
    kRenderOnlyWork,  // main thread: a render-only pass's work
    kWaitForRender,   // main thread: the game's wait for the frame in flight
    kPacing,          // main thread: held back by the frame cap
    kDraw,            // render thread: drawing a frame, present included
    kDrawCpu,         // render thread: CPU time used while drawing (kDraw less waits)
    kPresent,         // render thread: present (Renderer::Present)
    kParts
  };

  void Add(Part part, Clock::duration d) {
    std::lock_guard lock(mutex_);
    Stat& s = stats_[part];
    double ms = std::chrono::duration<double, std::milli>(d).count();
    s.total += ms;
    s.worst = std::max(s.worst, ms);
    s.count++;
  }

  // Main thread, once per pass.
  void MaybeLog() {
    auto now = Clock::now();
    if (now - last_log_ < std::chrono::seconds(2)) {
      return;
    }
    last_log_ = now;
    int64_t blend_ns, upload_ns;
    interpolation::BlendState_TakeReplayTimes(blend_ns, upload_ns);
    static const char* kNames[kParts] = {"logic pass work", "render-only pass work",
                                         "wait for render thread", "frame cap",
                                         "render thread draw", "render thread draw CPU",
                                         "present"};
    std::lock_guard lock(mutex_);
    std::string line;
    int frames = std::max(stats_[kDraw].count, 1);
    for (int i = 0; i < kParts; i++) {
      const Stat& s = stats_[i];
      line += fmt::format(" | {} {:.2f} avg {:.2f} worst ({})", kNames[i],
                          s.count ? s.total / s.count : 0.0, s.worst, s.count);
      stats_[i] = {};
    }
    REXLOG_INFO("frame times (ms):{} | per frame: blending {:.2f}, constant uploads {:.2f}", line,
                blend_ns / 1e6 / frames, upload_ns / 1e6 / frames);
  }

  Clock::time_point pass_start;
  bool pass_render_only = false;
  bool pass_started = false;

 private:
  struct Stat {
    double total = 0, worst = 0;
    int count = 0;
  };
  std::mutex mutex_;
  Stat stats_[kParts];
  Clock::time_point last_log_ = Clock::now();
};

// CPU time used by the calling thread; time it spends blocked doesn't count.
FrameTimes::Clock::duration ThreadCpuTime() {
#ifdef _WIN32
  // Not supported on Windows (a diagnostic): reads as zero.
  return FrameTimes::Clock::duration::zero();
#else
  timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
  return std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec);
#endif
}

FrameTimes& GetFrameTimes() {
  static FrameTimes times;
  return times;
}

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
  bool timing = REXCVAR_GET(log_frame_times);
  auto start = FrameTimes::Clock::now();
  auto cpu_start = timing ? ThreadCpuTime() : FrameTimes::Clock::duration{};
  interpolation::BlendState_OnReplayFrameStart();
  native::Renderer_OnReplayStart();
  if (native::Renderer_ReplayingStoredFrame()) {
    // A pass that didn't draw: the tick's stored list again, in place of
    // the game's frame (its replay, front buffer resolve and present).
    native::Renderer_DrawStoredFrame(base);
  } else {
    ctx.r3.u64 = renderer;
    sub_82091E58(ctx, base);
  }
  debug::ReplayStats_OnFrame();
  native::Renderer_EndFrame();
  if (timing) {
    GetFrameTimes().Add(FrameTimes::kDraw, FrameTimes::Clock::now() - start);
    GetFrameTimes().Add(FrameTimes::kDrawCpu, ThreadCpuTime() - cpu_start);
  }
}

bool Renderer_IsRunning(uint8_t* base, uint32_t renderer) {
  return guest::LoadU8(base, renderer + kRendererRunningOffset) != 0;
}

// Render thread: one frame per request from the main thread.
void Renderer_ThreadLoop(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  while (Renderer_IsRunning(base, renderer)) {
    Event_Wait(ctx, base, renderer + kRendererTickDoneEventOffset);
    debug::StateCapture_OnTickDone(base);
    Renderer_DrawFrame(ctx, base, renderer);
    Event_Signal(ctx, base, renderer + kRendererFrameDoneEventOffset);
  }
}

// Main thread, once per main-loop pass: the game's own wait for the frame in
// flight (and its deferred work), then pacing for the coming pass.
void Renderer_WaitForFrame(PPCContext& ctx, uint8_t* base, uint32_t renderer) {
  bool timing = REXCVAR_GET(log_frame_times);
  FrameTimes& times = GetFrameTimes();
  auto wait_start = FrameTimes::Clock::now();
  if (timing && times.pass_started) {
    times.Add(times.pass_render_only ? FrameTimes::kRenderOnlyWork : FrameTimes::kLogicWork,
              wait_start - times.pass_start);
  }
  debug::ImageDump_OnPass(base);
  native::Renderer_OnListHandedOver(frame_pacing::GameMessages_PassKind());
  ctx.r3.u64 = renderer;
  __imp__sub_820920D8(ctx, base);
  auto pacing_start = FrameTimes::Clock::now();

  auto& pacer = GetFramePacer();
  frame_pacing::GameMessages_OnPassFinished();
  bool render_only;
  if (frame_pacing::GameMessages_InRace()) {
    hooks::TitleScreen_SkipTickWait(true);
    render_only = pacer.WaitForNextFrame();  // false once the next tick is due
  } else {
    // Menus run at the game's 30 fps (see game_messages.h).
    if (hooks::TitleScreen_SkipTickWait(false)) {
      pacer.RestartTickGrid();
    } else {
      pacer.WaitForNextTick();
    }
    render_only = false;
  }
  if (!render_only) {
    interpolation::BlendState_OnLogicPassStart();
    interpolation::BlendState_OnReplayTick();
    pacer.OnTickReleased();
  }
  pacer.OnFrameStarted();
  if (timing) {
    auto now = FrameTimes::Clock::now();
    times.Add(FrameTimes::kWaitForRender, pacing_start - wait_start);
    times.Add(FrameTimes::kPacing, now - pacing_start);
    times.pass_start = now;
    times.pass_render_only = render_only && frame_pacing::GameMessages_InRace();
    times.pass_started = true;
    times.MaybeLog();
  }
  frame_pacing::GameMessages_SetRenderOnlyPass(render_only);
  debug::FrameStateDiff_OnPassStart(base, render_only);
  debug::IdleTimerScan_OnPassStart(base, render_only, frame_pacing::GameMessages_InRace());
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

// Renderer::Present: r3 = renderer. Wrapped only to time it
// (--log_frame_times).
extern "C" REX_FUNC(sub_82091A60) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(log_frame_times)) {
    __imp__sub_82091A60(ctx, base);
    return;
  }
  auto start = FrameTimes::Clock::now();
  __imp__sub_82091A60(ctx, base);
  GetFrameTimes().Add(FrameTimes::kPresent, FrameTimes::Clock::now() - start);
}

// VideoOutput_WaitForVblank: r3 = video output.
extern "C" REX_FUNC(sub_8258DAA8) {
  REX_FUNC_PROLOGUE();
  VideoOutput_WaitForVblank(ctx, base, ctx.r3.u32);
}
