// The game's statically linked C runtime (docs/functions/c-runtime.md): the
// stack probe dropped and memset natively.

#include "guest.h"

#include "base/flags.h"

#include <cstring>

ITC_FLAG_BOOL(native_c_runtime, true, "ITC",
              "Run the C runtime's helpers natively instead of through the translated code: "
              "the stack probe (__chkstk, sub_825A9934) is skipped, as its reads of the guest "
              "stack have no effect; memset (sub_825A71D0) (checked by "
              "--run_harnesses=c_runtime)");

// __chkstk: r12 = minus the frame about to be made (4 KB or more). Reads one
// word every 4,096 bytes below r1 down to the new frame, touching the guard
// pages on the 360; the guest stack here is ordinary memory, so the reads do
// nothing and are left out. The callers only use r12 and r1 after it, both
// unchanged either way.
extern "C" REX_FUNC(sub_825A9934) {
  REX_FUNC_PROLOGUE();

  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A9934(ctx, base);
  }
}

// memset: r3 = destination, r4 = the value (its low byte), r5 = the count
// (32 bits); returns r3. The original stores bytes up to a word boundary,
// then words, then bytes: the same bytes in the end.
extern "C" REX_FUNC(sub_825A71D0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A71D0(ctx, base);
    return;
  }
  const uint8_t value = ctx.r4.u8;
  guest::ForEachHostPart(base, ctx.r3.u32, ctx.r5.u32,
                         [&](uint8_t* host, uint32_t, uint32_t bytes) {
                           std::memset(host, value, bytes);
                         });
}
