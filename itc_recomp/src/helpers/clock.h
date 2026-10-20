// The performance counter's rate (KeQueryPerformanceFrequency), as plain
// C++. The counter is the timebase recompiled code reads with mftb
// (REX_QUERY_TIMEBASE, the SDK's guest clock), whose rate the SDK's runtime
// sets to the 360's 50 MHz (rexglue-sdk/src/system/runtime.cpp): the two have
// to agree. See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace helpers {

constexpr uint64_t kPerformanceFrequency = 50000000;

// How many counter ticks a wait for the audio hardware (sub_8245D288) gives
// up after: an eighth of a second.
constexpr uint64_t AudioHardwareTimeout() {
  return kPerformanceFrequency >> 3;
}

}  // namespace helpers
