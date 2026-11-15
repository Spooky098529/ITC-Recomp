// Our register view for the hand ports (docs/rexglue-removal.md, "Before
// E1: the rewiring", stage 3; the user's call, 2026-11-15): the registers a
// call passes and returns (lifted::Args, lifted::Result) under the names the
// SDK's context gives them, so a port's adapter body and its `Calls` run on
// it unchanged, without the SDK's context. It holds only what a call
// carries: the stack pointer, r3 to r10, r13, f1 to f13, the return
// address and ctr (a call's target); code that reads anything else (a
// caller's nonvolatile registers, another call's leftovers) doesn't build
// against it, and is ported by hand.

#pragma once

#include "lifted/lifted.h"

#include <cstdint>

namespace lifted {

struct Regs {
  Temp r1{}, r3{}, r4{}, r5{}, r6{}, r7{}, r8{}, r9{}, r10{}, r13{};
  Temp f1{}, f2{}, f3{}, f4{}, f5{}, f6{}, f7{}, f8{}, f9{}, f10{}, f11{}, f12{}, f13{};
  Temp ctr{};
  uint64_t lr = 0;

  // The float modes the SDK's context switches (its fpscr): the host's.
  struct FloatMode {
    void disableFlushMode() { ScalarFloatMode(); }
    void enableFlushMode() { VectorFloatMode(); }
  } fpscr;

  Regs() = default;
  explicit Regs(const Args& args) {
    r1.u64 = args.r1;
    r13.u64 = args.r13;
    Temp* r = &r3;
    for (int i = 0; i < 8; i++) {
      r[i].u64 = args.r[i];
    }
    Temp* f = &f1;
    for (int i = 0; i < 13; i++) {
      f[i].f64 = args.f[i];
    }
    lr = args.lr;
  }

  // A call's arguments from these registers.
  Args ToArgs() const {
    Args args;
    args.r1 = r1.u64;
    args.r13 = r13.u64;
    const Temp* r = &r3;
    for (int i = 0; i < 8; i++) {
      args.r[i] = r[i].u64;
    }
    const Temp* f = &f1;
    for (int i = 0; i < 13; i++) {
      args.f[i] = f[i].f64;
    }
    args.lr = uint32_t(lr);
    return args;
  }

  // What a call left: r3 and f1.
  void Apply(const Result& result) {
    r3.u64 = result.r3;
    f1.f64 = result.f1;
  }

  Result result() const { return {r3.u64, f1.f64}; }
};

// A call by address with these registers through the dispatch table; its r3
// and f1 back in them. The counterpart of CallFromContext (sdk.h).
inline void CallFromContext(Regs& regs, uint8_t* base, uint32_t address) {
  regs.ctr.u64 = address;
  regs.Apply(Call(base, address, regs.ToArgs()));
}

// The flag of entries registered always on.
inline constexpr bool kAlwaysOn = true;

// Registers one hand port's entry under its address, with the flag that
// turns it on (false when the address already has one: logged).
bool RegisterEntry(uint32_t address, Entry entry, const bool* enabled);

}  // namespace lifted
