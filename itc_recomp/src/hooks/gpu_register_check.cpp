// TEMPORARY (B3, the GPU registers at 0x7FC80000): counts the calls of the
// functions that read or write them, which nothing answers now that the GPU
// plugin is gone, logging the first calls and powers of two with their
// return address and what they touch. Removed when B3's register item is
// done. See B3 in docs/rexglue-removal.md.

#include "guest.h"

#include <rex/logging.h>

#include <atomic>

void GpuRegisterCheck_Interrupt(uint32_t source, uint32_t lr);

namespace {

struct Counter {
  const char* name;
  std::atomic<uint32_t> calls{0};

  bool Next() {
    const uint32_t n = ++calls;
    return n <= 4 || (n & (n - 1)) == 0;
  }
};

Counter deferred_kick_helper{"sub_8229DFF8"};
Counter register_read{"sub_822AC590"};
Counter register_write{"sub_822AC5A0"};
Counter hang_dump{"sub_822AC918"};
Counter hang{"sub_822ACAA8"};
Counter registers_read{"sub_822A0688"};
Counter engines_callback{"sub_822ACB48"};
Counter interrupt{"sub_8229DEA0"};

}  // namespace

void GpuRegisterCheck_Interrupt(uint32_t source, uint32_t lr) {
  if (interrupt.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}, source {}", interrupt.name,
                interrupt.calls.load(), lr, source);
  }
}

// Stores through the pointer at r3 + 8 (in the MMIO range or not).
extern "C" REX_FUNC(sub_8229DFF8) {
  REX_FUNC_PROLOGUE();
  if (deferred_kick_helper.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}, stores at {:08X}",
                deferred_kick_helper.name, deferred_kick_helper.calls.load(), ctx.lr,
                guest::LoadU32(base, ctx.r3.u32 + 8));
  }
  __imp__sub_8229DFF8(ctx, base);
}

// Reads register r3 (at 7FC80000 + 4 r3, a plain load).
extern "C" REX_FUNC(sub_822AC590) {
  REX_FUNC_PROLOGUE();
  const bool log = register_read.Next();
  const uint32_t index = ctx.r3.u32, lr = ctx.lr;
  __imp__sub_822AC590(ctx, base);
  if (log) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}, register {:04X} read {:08X}",
                register_read.name, register_read.calls.load(), lr, index, ctx.r3.u32);
  }
}

// Writes register r3 = r4.
extern "C" REX_FUNC(sub_822AC5A0) {
  REX_FUNC_PROLOGUE();
  if (register_write.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}, register {:04X} = {:08X}",
                register_write.name, register_write.calls.load(), ctx.lr, ctx.r3.u32,
                ctx.r4.u32);
  }
  __imp__sub_822AC5A0(ctx, base);
}

extern "C" REX_FUNC(sub_822AC918) {
  REX_FUNC_PROLOGUE();
  if (hang_dump.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}", hang_dump.name,
                hang_dump.calls.load(), ctx.lr);
  }
  __imp__sub_822AC918(ctx, base);
}

extern "C" REX_FUNC(sub_822ACAA8) {
  REX_FUNC_PROLOGUE();
  if (hang.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}", hang.name, hang.calls.load(), ctx.lr);
  }
  __imp__sub_822ACAA8(ctx, base);
}

extern "C" REX_FUNC(sub_822A0688) {
  REX_FUNC_PROLOGUE();
  if (registers_read.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}", registers_read.name,
                registers_read.calls.load(), ctx.lr);
  }
  __imp__sub_822A0688(ctx, base);
}

extern "C" REX_FUNC(sub_822ACB48) {
  REX_FUNC_PROLOGUE();
  if (engines_callback.Next()) {
    REXLOG_INFO("gpu register check: {} #{} from {:08X}", engines_callback.name,
                engines_callback.calls.load(), ctx.lr);
  }
  __imp__sub_822ACB48(ctx, base);
}
