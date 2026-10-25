// The game's clocks as plain C++: the performance counter's rate
// (KeQueryPerformanceFrequency) and the millisecond tick count
// (GetTickCount). The counter is the timebase recompiled code reads with mftb
// (REX_QUERY_TIMEBASE, the SDK's guest clock), whose rate the SDK's runtime
// sets to the 360's 50 MHz (rexglue-sdk/src/system/runtime.cpp): the two have
// to agree. See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace helpers {

constexpr uint64_t kPerformanceFrequency = 50000000;

// GetTickCount (sub_822956F0): milliseconds since the process started, from
// the host's steady clock, wrapping at 2^32 as the 360's does. The game's
// only reader of the kernel's tick count (KeTimeStampBundle +16, which the
// SDK sets to its uptime in milliseconds every 1 ms), so recompiled callers
// (through src/hooks/clock.cpp's adapter) and native ones share this clock.
uint32_t TickCount();

}  // namespace helpers
