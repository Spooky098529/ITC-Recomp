// The game's two readers of the performance counter's rate natively, with
// the rate from src/helpers/clock.h.

#include "guest.h"
#include "guest_call.h"

#include "helpers/clock.h"

using guest_call::InFrame;

// QueryPerformanceFrequency: r3 = the rate (out, 64 bits); returns 1.
extern "C" REX_FUNC(sub_825B9A70) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r3.u32;
  guest::StoreU32(base, out, uint32_t(helpers::kPerformanceFrequency >> 32));
  guest::StoreU32(base, out + 4, uint32_t(helpers::kPerformanceFrequency));
  ctx.r3.u64 = 1;
}

// The audio library's wait for its hardware (r3 = the object): polls
// sub_8245D350 until it answers, and returns 0. After an eighth of a second
// without an answer it resets the hardware (sub_82106540, Sleep(1000), two
// writes to the XMA registers at 0x7FEA1804, sub_8245CE98 for each of the
// object's [+0] contexts) and returns 1. Its callers are in the audio library
// (B2 in docs/rexglue-removal.md), so the steps stay guest calls.
extern "C" REX_FUNC(sub_8245D288) {
  REX_FUNC_PROLOGUE();
  const uint32_t object = ctx.r3.u32;
  bool reset = false;
  InFrame(ctx, base, 112, 0x8245D2A0, [&](uint32_t) {
    const uint64_t start = REX_QUERY_TIMEBASE();
    ctx.r3.u64 = object;
    sub_8245D350(ctx, base);
    while (ctx.r3.s32 == 0) {
      if (REX_QUERY_TIMEBASE() - start > helpers::AudioHardwareTimeout()) {
        reset = true;
        break;
      }
      ctx.r3.u64 = object;
      ctx.lr = 0x8245D2CC;
      sub_8245D350(ctx, base);
    }
    if (!reset) {
      return;
    }
    ctx.r3.u64 = object;
    ctx.lr = 0x8245D2E8;
    sub_82106540(ctx, base);
    ctx.r3.u64 = 1000;
    ctx.lr = 0x8245D2F0;
    sub_82295500(ctx, base);
    REX_MM_STORE_U32(0x7FEA1804u, 0u);
    REX_MM_STORE_U32(0x7FEA1804u, 0x03000000u);
    ctx.r3.u64 = object;
    ctx.lr = 0x8245D31C;
    sub_8245D350(ctx, base);
    for (uint32_t i = 0; i < guest::LoadU32(base, object); i++) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = i;
      ctx.lr = 0x8245D334;
      sub_8245CE98(ctx, base);
    }
  });
  ctx.r3.u64 = reset ? 1 : 0;
}
