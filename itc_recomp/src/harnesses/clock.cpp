// The game's clock readers (src/hooks/clock.cpp) against the recompiled ones:
// QueryPerformanceFrequency, and GetTickCount. The tick counts come from two
// clocks (the SDK's KeTimeStampBundle and our steady clock) started at
// different moments, so a case compares how far each advances over a sleep:
// the same within 3 ms (the SDK refreshes its count every 1 ms, on a timer),
// and ours never going back.

#include "harness.h"

#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>

namespace harness {

namespace {

uint32_t Read(Context& h, GuestFunction function) {
  PPCContext t = CallContext(h);
  function(t, h.base);
  return t.r3.u32;
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
}

}  // namespace harness
