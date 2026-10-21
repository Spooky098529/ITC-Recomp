// Voice chat (Xbox-only), skipped at its thread: the voice library's worker
// (sub_8257B5D8) called Voice::Poll (sub_8257AF30) every 10 ms, which in play
// read a notification into a flag only never-called code reads, set up an
// audio voice nothing else uses, and found no talkers. The engine itself is
// still made by Voice::Init (the game calls it without null checks). See B5
// in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

using guest_call::InFrame;

// The thread's entry: r3 = the engine. Signals the event Voice::Init waits on
// (+1304, "started") and ends; nothing waits on the thread until the engine's
// release, which finds it ended.
extern "C" REX_FUNC(sub_8257B5D8) {
  REX_FUNC_PROLOGUE();
  const uint32_t engine = ctx.r3.u32;
  InFrame(ctx, base, 112, 0x8257B650, [&](uint32_t) {
    ctx.r3.u64 = guest::LoadU32(base, engine + 1304);
    sub_825BA1E0(ctx, base);  // SetEvent
  });
  ctx.r3.u64 = 0;
}
