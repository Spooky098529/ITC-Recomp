// The C runtime's math functions natively: adapters in front of
// src/c_runtime/math.cpp (docs/functions/c-runtime.md). Each takes its
// argument in f1 and returns in f1, with flush-to-zero off as the translated
// code runs.

#include "guest.h"

#include "base/flags.h"
#include "c_runtime/math.h"

ITC_FLAG_BOOL(native_math, true, "ITC",
              "Run the C runtime's math functions (acos and asin, sub_825A8170, sub_825A7FB0 "
              "and sub_825A80A8; sin and cos, sub_825A88A0 and sub_825A8978; atan and atan2, sub_825A8688 and "
              "sub_825A8770) natively instead of through the translated code (the same "
              "doubles, bit for bit; checked by --run_harnesses=math)");

// acos: f1 = x.
extern "C" REX_FUNC(sub_825A8170) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A8170(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Acos(ctx.f1.f64);
}

// asin and acos's kernel: f1 = x, r4 = 1 for acos, 0 for asin.
extern "C" REX_FUNC(sub_825A7FB0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A7FB0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::AsinAcos(ctx.f1.f64, ctx.r4.s32 != 0);
}

// asin: f1 = x.
extern "C" REX_FUNC(sub_825A80A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A80A8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Asin(ctx.f1.f64);
}

// sin: f1 = x.
extern "C" REX_FUNC(sub_825A88A0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A88A0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Sin(ctx.f1.f64);
}

// cos: f1 = x.
extern "C" REX_FUNC(sub_825A8978) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A8978(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Cos(ctx.f1.f64);
}

// atan: f1 = x.
extern "C" REX_FUNC(sub_825A8688) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A8688(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Atan(ctx.f1.f64);
}

// atan2: f1 = y, f2 = x. (The original also stores both in its caller's
// linkage area, r1 + 16 and + 24, which nothing reads back.)
extern "C" REX_FUNC(sub_825A8770) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A8770(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Atan2(ctx.f1.f64, ctx.f2.f64);
}
