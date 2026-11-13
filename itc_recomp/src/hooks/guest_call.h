// Calling recompiled code from an adapter (src/hooks/), from inside a frame
// made as the recompiled function's own was.

#pragma once

#include "guest.h"

namespace guest_call {

// General register `reg` of `ctx`. The context doesn't keep them in number
// order (r3 comes first, then r0 to r2, then r4 onward), so they can't be
// indexed from r0 or r3.
inline PPCRegister& Gpr(PPCContext& ctx, int reg) {
  switch (reg) {
    case 0: return ctx.r0;
    case 1: return ctx.r1;
    case 2: return ctx.r2;
    case 3: return ctx.r3;
    default: return (&ctx.r4)[reg - 4];
  }
}

// Makes a frame of `frame_bytes` below the stack pointer (with its back
// chain), sets the link register to `return_address` (the recompiled call's),
// runs `call(frame)` and pops the frame.
template <typename Call>
void InFrame(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, uint32_t return_address,
             Call call) {
  const uint64_t stack = ctx.r1.u64;
  const uint32_t frame = ctx.r1.u32 - frame_bytes;
  guest::StoreU32(base, frame, ctx.r1.u32);
  ctx.r1.u64 = frame;
  ctx.lr = return_address;
  call(frame);
  ctx.r1.u64 = stack;
}

}  // namespace guest_call
