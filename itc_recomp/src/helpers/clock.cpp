#include "helpers/clock.h"

#include "base/log.h"

#include <chrono>

namespace helpers {

namespace {

const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

}  // namespace

uint32_t TickCount() {
  ITC_NATIVE_RUNNING("sub_822956F0 GetTickCount");
  const auto elapsed = std::chrono::steady_clock::now() - start;
  return uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
}

}  // namespace helpers
