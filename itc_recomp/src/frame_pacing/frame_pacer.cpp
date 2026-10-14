#include "frame_pacer.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <thread>

REXCVAR_DEFINE_INT32(max_fps, 0, "ITC", "Frame rate cap; 0 means unlimited");
REXCVAR_DEFINE_BOOL(log_fps, false, "ITC", "Log frames and game ticks per second");

namespace frame_pacing {

void FramePacer::OnFrameStarted() {
  last_frame_ = Clock::now();
  frames_since_log_++;
  LogRates(last_frame_);
}

bool FramePacer::WaitForNextFrame() {
  // A tick that's already due goes first. The frame slot below counts from
  // the last frame's start, so after a frame longer than the cap's period it
  // lies in the past and could otherwise win over an overdue tick: on slow
  // machines (the Steam Deck) that held ticks back a frame at a time, and the
  // game ran at half speed.
  if (Clock::now() >= next_tick_) {
    return false;
  }
  Clock::time_point next_frame = last_frame_ + FramePeriod();
  if (next_frame >= next_tick_) {
    std::this_thread::sleep_until(next_tick_);
    return false;
  }
  std::this_thread::sleep_until(next_frame);
  return true;
}

void FramePacer::WaitForNextTick() {
  std::this_thread::sleep_until(next_tick_);
}

void FramePacer::OnTickReleased() {
  Clock::time_point now = Clock::now();
  // Stay on the 30 Hz grid, unless we've fallen a whole tick behind (a
  // loading screen, say), in which case restart the grid from now rather
  // than rushing ticks to catch up.
  // For --log_fps: ticks released more than a quarter of a tick late, and
  // grid restarts (time the game lost).
  if (now - next_tick_ > kTickPeriod / 4) {
    late_ticks_since_log_++;
  }
  Clock::time_point next = next_tick_ + kTickPeriod;
  if (next < now) {
    next = now + kTickPeriod;
    dropped_ticks_since_log_++;
  }
  last_tick_ = now;
  next_tick_ = next;
  ticks_since_log_++;
}

float FramePacer::TickProgress() const {
  auto elapsed = Clock::now() - last_tick_;
  return std::clamp(float(elapsed.count()) / float(kTickPeriod.count()), 0.0f, 1.0f);
}

FramePacer::Clock::duration FramePacer::FramePeriod() const {
  int max_fps = REXCVAR_GET(max_fps);
  if (max_fps <= 0) {
    return Clock::duration::zero();
  }
  return std::chrono::duration_cast<Clock::duration>(std::chrono::seconds(1)) / max_fps;
}

void FramePacer::LogRates(Clock::time_point now) {
  if (!REXCVAR_GET(log_fps)) {
    return;
  }
  if (now - last_log_ < std::chrono::seconds(1)) {
    return;
  }
  REXLOG_INFO("fps {} ticks/s {} (late {}, grid restarts {})", frames_since_log_,
              ticks_since_log_, late_ticks_since_log_, dropped_ticks_since_log_);
  last_log_ = now;
  frames_since_log_ = 0;
  ticks_since_log_ = 0;
  late_ticks_since_log_ = 0;
  dropped_ticks_since_log_ = 0;
}

FramePacer& GetFramePacer() {
  static FramePacer pacer;
  return pacer;
}

}  // namespace frame_pacing
