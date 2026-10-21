#include "frame_pacer.h"

#include "base/flags.h"
#include "base/log.h"

#include <algorithm>
#include <thread>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

ITC_FLAG_INT32(max_fps, 0, "ITC", "Frame rate cap; 0 means unlimited");
ITC_FLAG_BOOL(log_fps, false, "ITC", "Log frames and game ticks per second");

namespace frame_pacing {

namespace {

#ifdef _WIN32
// Windows rounds sleeps up to the system timer (often 15.6 ms), far too
// coarse for a 144 fps cap. Sleep on a high-resolution waitable timer until
// a millisecond before, then spin the rest.
void SleepUntil(FramePacer::Clock::time_point until) {
  constexpr auto kSpin = std::chrono::milliseconds(1);
  thread_local HANDLE timer = CreateWaitableTimerExW(
      nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
  auto sleep = until - FramePacer::Clock::now() - kSpin;
  if (timer && sleep > FramePacer::Clock::duration::zero()) {
    LARGE_INTEGER due;
    // Relative, in 100 ns units.
    due.QuadPart = -std::chrono::duration_cast<std::chrono::nanoseconds>(sleep).count() / 100;
    if (SetWaitableTimerEx(timer, &due, 0, nullptr, nullptr, nullptr, 0)) {
      WaitForSingleObject(timer, INFINITE);
    }
  }
  while (FramePacer::Clock::now() < until) {
    YieldProcessor();
  }
}
#else
void SleepUntil(FramePacer::Clock::time_point until) {
  std::this_thread::sleep_until(until);
}
#endif

}  // namespace

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
    SleepUntil(next_tick_);
    return false;
  }
  SleepUntil(next_frame);
  return true;
}

void FramePacer::WaitForNextTick() {
  SleepUntil(next_tick_);
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
  int max_fps = ITC_FLAG(max_fps);
  if (max_fps <= 0) {
    return Clock::duration::zero();
  }
  return std::chrono::duration_cast<Clock::duration>(std::chrono::seconds(1)) / max_fps;
}

void FramePacer::LogRates(Clock::time_point now) {
  if (!ITC_FLAG(log_fps)) {
    return;
  }
  if (now - last_log_ < std::chrono::seconds(1)) {
    return;
  }
  ITC_LOG_INFO("fps {} ticks/s {} (late {}, grid restarts {})", frames_since_log_,
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
