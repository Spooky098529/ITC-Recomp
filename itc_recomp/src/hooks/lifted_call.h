// Calls from plain C++ to other guest functions through the lift sweep's
// dispatch table (src/lifted/lifted.h, docs/loops/lift-sweep.md): a lifted
// function's entry when there is one, else the function the SDK has at the
// address (a port's override or the recompiled original). No `ctx`: the
// plain C++ gets the stack pointer and r13 it was called with, and its
// adapter enters it through lifted::Enter, which makes the thread's context
// the one calls through the boundary use.
#pragma once

#include "guest.h"
#include "lifted/lifted.h"
#include "lifted/sdk.h"

#include <initializer_list>

namespace lifted_call {

// A call's registers: r1, r13 and r3 onward from `r` (the rest 0), f1.
inline lifted::Args Args(uint32_t sp, uint64_t r13, std::initializer_list<uint64_t> r,
                         double f1 = 0.0) {
  lifted::Args args;
  args.r1 = sp;
  args.r13 = r13;
  int i = 0;
  for (const uint64_t value : r) {
    args.r[i++] = value;
  }
  args.f[0] = f1;
  return args;
}

// Calls the function at `address` (a direct call, a vtable slot or a
// function pointer) with the stack pointer `sp`.
inline lifted::Result Call(uint8_t* base, uint32_t address, uint32_t sp, uint64_t r13,
                           std::initializer_list<uint64_t> r, double f1 = 0.0) {
  return lifted::Call(base, address, Args(sp, r13, r, f1));
}

// Calls an SDK import (a kernel or XAM function) as the original does.
inline lifted::Result Import(uint8_t* base, lifted::ImportFunction* function, uint32_t sp,
                             uint64_t r13, std::initializer_list<uint64_t> r) {
  return lifted::Import(base, function, Args(sp, r13, r));
}

// Makes a frame of `bytes` below `sp` as the original's stwu does (its back
// chain stored); returns the new stack pointer.
inline uint32_t PushFrame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// An adapter's step for plain C++ `function(base, sp, r13, r3)` returning
// the original's r3: entered through lifted::Enter, f1 left as it was.
template <uint64_t (*function)(uint8_t*, uint32_t, uint64_t, uint32_t)>
void EnterR3(PPCContext& ctx, uint8_t* base) {
  lifted::Enter(ctx, base, [](uint8_t* base, const lifted::Args& a) -> lifted::Result {
    return {function(base, uint32_t(a.r1), a.r13, uint32_t(a.r[0])), a.f[0]};
  });
}

}  // namespace lifted_call
