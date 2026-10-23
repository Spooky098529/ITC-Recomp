// The game's clock readers natively, from src/helpers/clock.h: the
// performance counter's rate and the tick count. (The audio library's hardware wait,
// sub_8245D288, which also read it, is in hooks/xma.cpp since B2: nothing to
// wait for.)

#include "guest.h"

#include "helpers/clock.h"

// QueryPerformanceFrequency: r3 = the rate (out, 64 bits); returns 1.
extern "C" REX_FUNC(sub_825B9A70) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r3.u32;
  guest::StoreU32(base, out, uint32_t(helpers::kPerformanceFrequency >> 32));
  guest::StoreU32(base, out + 4, uint32_t(helpers::kPerformanceFrequency));
  ctx.r3.u64 = 1;
}

// GetTickCount: returns the milliseconds since start. Every recompiled caller
// reaches the tick count through here, native callers through
// helpers::TickCount directly: one clock for both.
extern "C" REX_FUNC(sub_822956F0) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = helpers::TickCount();
}
