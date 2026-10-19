// Decides when the render thread draws frames and when game logic may tick.
//
// Game logic advances a fixed 1/30 s per tick (see docs/functions.md), so
// ticks are released on a steady 30 Hz clock. Between ticks the renderer
// draws as many frames as the frame cap allows. Pure host code: it knows
// nothing about guest state.

#pragma once

#include <chrono>

namespace frame_pacing {

class FramePacer {
 public:
  using Clock = std::chrono::steady_clock;

  static constexpr int kTicksPerSecond = 30;
  static constexpr Clock::duration kTickPeriod =
      std::chrono::duration_cast<Clock::duration>(std::chrono::seconds(1)) / kTicksPerSecond;

  // Call just before drawing a frame.
  void OnFrameStarted();

  // Sleeps until the next frame may start, honoring the frame cap.
  // Returns false instead if the next tick is due first.
  bool WaitForNextFrame();

  // Sleeps until the next tick is due.
  void WaitForNextTick();

  // Restarts the 30 Hz grid from now, for ticks released without waiting
  // (--skip_intro), so the grid doesn't run ahead of the clock.
  void RestartTickGrid() { next_tick_ = Clock::now(); }

  // Call when releasing game logic for its next tick. Schedules the tick
  // after it.
  void OnTickReleased();

  // How far the current moment is between the last tick and the next, from
  // 0 to 1. For interpolation.
  float TickProgress() const;

 private:
  Clock::duration FramePeriod() const;

  Clock::time_point next_tick_{};
  Clock::time_point last_tick_{};
  Clock::time_point last_frame_{};

  // For --log_fps.
  void LogRates(Clock::time_point now);
  Clock::time_point last_log_{};
  int frames_since_log_ = 0;
  int ticks_since_log_ = 0;
  int late_ticks_since_log_ = 0;
  int dropped_ticks_since_log_ = 0;
};

FramePacer& GetFramePacer();

}  // namespace frame_pacing
