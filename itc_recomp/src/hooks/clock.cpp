// The game's reader of the performance counter's rate natively, with the
// rate from src/helpers/clock.h. (The audio library's hardware wait,
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
