// The performance counter's rate (src/hooks/clock.cpp) against the
// recompiled QueryPerformanceFrequency.

#include "harness.h"

namespace harness {

void Clock(Context& h) {
  Results results("QueryPerformanceFrequency (sub_825B9A70)");
  Both(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = h.data; }, __imp__sub_825B9A70,
       sub_825B9A70, h.data, 8, "the rate");
  results.Report();
}

}  // namespace harness
