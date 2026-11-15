// The game's clock readers natively, from src/helpers/clock.h: the
// performance counter, its rate and the tick count. (The audio library's hardware wait,
// sub_8245D288, which also read it, is in hooks/xma.cpp since B2: nothing to
// wait for.)

#include "guest.h"
#include "hooks/native_entry.h"

#include "helpers/clock.h"

// QueryPerformanceFrequency: r3 = the rate (out, 64 bits); returns 1.
ITC_NATIVE_ENTRY_ALWAYS(825B9A70) {
  
  const uint32_t out = ctx.r3.u32;
  guest::StoreU32(base, out, uint32_t(helpers::kPerformanceFrequency >> 32));
  guest::StoreU32(base, out + 4, uint32_t(helpers::kPerformanceFrequency));
  ctx.r3.u64 = 1;
}

// GetTickCount: returns the milliseconds since start. Every recompiled caller
// reaches the tick count through here, native callers through
// helpers::TickCount directly: one clock for both.
ITC_NATIVE_ENTRY_ALWAYS(822956F0) {
  
  ctx.r3.u64 = helpers::TickCount();
}

// QueryPerformanceCounter: r3 = the count (out, 64 bits); returns 1. Every recompiled caller reaches
// the counter through here, native callers through
// helpers::PerformanceCounter directly: one clock for both.
ITC_NATIVE_ENTRY_ALWAYS(825BA0C8) {
  
  const uint32_t out = ctx.r3.u32;
  const uint64_t count = helpers::PerformanceCounter();
  guest::StoreU32(base, out, uint32_t(count >> 32));
  guest::StoreU32(base, out + 4, uint32_t(count));
  ctx.r3.u64 = 1;
}
