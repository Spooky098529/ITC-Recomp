// The game's clocks as plain C++: the performance counter
// (QueryPerformanceCounter) and its rate (KeQueryPerformanceFrequency), and
// the millisecond tick count (GetTickCount). The counter is the timebase recompiled code reads with mftb
// (REX_QUERY_TIMEBASE, the SDK's guest clock), whose rate the SDK's runtime
// sets to the 360's 50 MHz (rexglue-sdk/src/system/runtime.cpp): the two have
// to agree. See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>
#include <optional>

namespace helpers {

constexpr uint64_t kPerformanceFrequency = 50000000;

// GetTickCount (sub_822956F0): milliseconds since the process started, from
// the host's steady clock, wrapping at 2^32 as the 360's does. The game's
// only reader of the kernel's tick count (KeTimeStampBundle +16, which the
// SDK sets to its uptime in milliseconds every 1 ms), so recompiled callers
// (through src/hooks/clock.cpp's adapter) and native ones share this clock.
uint32_t TickCount();

// QueryPerformanceCounter (sub_825BA0C8): the timebase at kPerformanceFrequency
// (50 MHz) since the process started, from the host's steady clock (the user's
// call, 2026-10-29). The recompiled one read mftb (the SDK's guest clock,
// counting from 0 at its start at the same rate); in play nothing else reads
// mftb (its other readers are Dropped or never run), so nothing compares the
// two. Recompiled callers reach it through src/hooks/clock.cpp's adapter,
// native ones call it directly: one clock for both.
uint64_t PerformanceCounter();

// For harnesses: while set, PerformanceCounter and TickCount answer these on
// the calling thread (recompiled callers through the adapters too), so both
// versions of a clock's user read the same time. nullopt for the clock.
void SetClocksForHarness(std::optional<uint64_t> counter, std::optional<uint32_t> ticks);

}  // namespace helpers
