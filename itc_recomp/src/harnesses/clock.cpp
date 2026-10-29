// The game's clock readers (src/hooks/clock.cpp) against the recompiled ones:
// QueryPerformanceFrequency, QueryPerformanceCounter and GetTickCount. The tick counts come from two
// clocks (the SDK's KeTimeStampBundle and our steady clock) started at
// different moments, so a case compares how far each advances over a sleep:
// the same within 3 ms (the SDK refreshes its count every 1 ms, on a timer),
// and ours never going back. The performance counters (the SDK's guest
// timebase and our steady clock, both at 50 MHz) likewise: the same advance
// within 1 ms, returning 1, and the count stored big-endian where r3 points
// (the 8 bytes after it left as they were).

#include "harness.h"

#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

namespace harness {

namespace {

uint32_t Read(Context& h, GuestFunction function) {
  PPCContext t = CallContext(h);
  function(t, h.base);
  return t.r3.u32;
}

struct Count {
  uint64_t count;
  uint32_t r3;
  std::vector<uint8_t> after;  // the 8 bytes past the count
};

Count ReadCount(Context& h, GuestFunction function) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(h.base, h.data + i, 0xCD);
  }
  PPCContext t = CallContext(h);
  t.r3.u64 = h.data;
  function(t, h.base);
  return {(uint64_t(guest::LoadU32(h.base, h.data)) << 32) | guest::LoadU32(h.base, h.data + 4),
          t.r3.u32, harness::Read(h.base, h.data + 8, 8)};
}

}  // namespace

void Clock(Context& h) {
  Results results("QueryPerformanceFrequency (sub_825B9A70)");
  Both(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = h.data; }, __imp__sub_825B9A70,
       sub_825B9A70, h.data, 8, "the rate");
  results.Report();

  Results ticks("GetTickCount (sub_822956F0)");
  for (int sleep_ms : {0, 1, 2, 5, 10, 16, 25, 33, 50, 100}) {
    const uint32_t recompiled_start = Read(h, __imp__sub_822956F0);
    const uint32_t native_start = Read(h, sub_822956F0);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    const uint32_t recompiled_end = Read(h, __imp__sub_822956F0);
    const uint32_t native_end = Read(h, sub_822956F0);
    const int32_t recompiled_advance = int32_t(recompiled_end - recompiled_start);
    const int32_t native_advance = int32_t(native_end - native_start);
    const bool agrees =
        native_advance >= 0 && std::abs(native_advance - recompiled_advance) <= 3;
    Outputs out[2];
    out[0].Word(1);
    out[1].Word(agrees ? 1 : 0);
    ticks.Check(out[0], out[1],
                "a " + std::to_string(sleep_ms) + " ms sleep: the SDK's count advanced " +
                    std::to_string(recompiled_advance) + " ms, ours " +
                    std::to_string(native_advance) + " ms");
  }
  ticks.Report();

  // 1 ms at 50 MHz: both clocks are read on demand (no timer), so the two
  // reads of a pair are microseconds apart.
  constexpr int64_t kCountSlack = 50000;
  Results counts("QueryPerformanceCounter (sub_825BA0C8)");
  for (int sleep_ms : {0, 1, 2, 5, 10, 16, 25, 33, 50, 100}) {
    const Count recompiled_start = ReadCount(h, __imp__sub_825BA0C8);
    const Count native_start = ReadCount(h, sub_825BA0C8);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    const Count recompiled_end = ReadCount(h, __imp__sub_825BA0C8);
    const Count native_end = ReadCount(h, sub_825BA0C8);
    const int64_t recompiled_advance = int64_t(recompiled_end.count - recompiled_start.count);
    const int64_t native_advance = int64_t(native_end.count - native_start.count);
    const bool agrees = native_advance >= 0 &&
                        std::abs(native_advance - recompiled_advance) <= kCountSlack;
    Outputs out[2];
    for (const Count* c : {&recompiled_start, &recompiled_end}) {
      out[0].Word(c->r3);
      out[0].Bytes(c->after);
    }
    for (const Count* c : {&native_start, &native_end}) {
      out[1].Word(c->r3);
      out[1].Bytes(c->after);
    }
    out[0].Word(1);
    out[1].Word(agrees ? 1 : 0);
    counts.Check(out[0], out[1],
                 "a " + std::to_string(sleep_ms) + " ms sleep: the SDK's timebase advanced " +
                     std::to_string(recompiled_advance) + ", ours " +
                     std::to_string(native_advance));
  }
  counts.Report();
}

}  // namespace harness
