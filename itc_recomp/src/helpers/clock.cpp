#include "helpers/clock.h"

#include "base/log.h"

#include <chrono>

namespace helpers {

namespace {

const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

// The harnesses' stand-in answers, on their thread only (XAudio's thread
// reads the clocks meanwhile).
thread_local std::optional<uint64_t> harness_counter;
thread_local std::optional<uint32_t> harness_ticks;

}  // namespace

uint32_t TickCount() {
  ITC_NATIVE_RUNNING("sub_822956F0 GetTickCount");
  if (harness_ticks) {
    return *harness_ticks;
  }
  const auto elapsed = std::chrono::steady_clock::now() - start;
  return uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
}

uint64_t PerformanceCounter() {
  ITC_NATIVE_RUNNING("sub_825BA0C8 QueryPerformanceCounter");
  if (harness_counter) {
    return *harness_counter;
  }
  using Ticks = std::chrono::duration<uint64_t, std::ratio<1, kPerformanceFrequency>>;
  const auto now = [] {
    return std::chrono::duration_cast<Ticks>(std::chrono::steady_clock::now() - start).count();
  };
  // Read again when the low word is 0, as the original does (the 360's
  // timebase erratum).
  const uint64_t count = now();
  return uint32_t(count) != 0 ? count : now();
}

void SetClocksForHarness(std::optional<uint64_t> counter, std::optional<uint32_t> ticks) {
  harness_counter = counter;
  harness_ticks = ticks;
}

}  // namespace helpers
