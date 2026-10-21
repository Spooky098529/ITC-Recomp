// The performance counter's rate (KeQueryPerformanceFrequency), as plain
// C++. The counter is the timebase recompiled code reads with mftb
// (REX_QUERY_TIMEBASE, the SDK's guest clock), whose rate the SDK's runtime
// sets to the 360's 50 MHz (rexglue-sdk/src/system/runtime.cpp): the two have
// to agree. See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace helpers {

constexpr uint64_t kPerformanceFrequency = 50000000;

}  // namespace helpers
