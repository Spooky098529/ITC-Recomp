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
              "sub_825A8770; pow, sub_825A8ED0, with log, sub_825A9C00, copysign, sub_825AB1C8, "
              "_set_exp, _get_exp and _decomp, sub_825AFE80, sub_825AFEA8 and sub_825AFF80, and the "
              "integer test sub_825A8CE0) natively instead of through the translated code (the same "
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

// copysign: f1 = magnitude, f2 = sign. (The original also stores both in its
// caller's linkage area, r1 + 16 and + 24, which nothing reads back.)
extern "C" REX_FUNC(sub_825AB1C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825AB1C8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Copysign(ctx.f1.f64, ctx.f2.f64);
}

// _set_exp: f1 = x, r4 = the exponent.
extern "C" REX_FUNC(sub_825AFE80) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825AFE80(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::SetExp(ctx.f1.f64, ctx.r4.s32);
}

// _get_exp: f1 = x; returns r3.
extern "C" REX_FUNC(sub_825AFEA8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825AFEA8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.s64 = c_runtime::GetExp(ctx.f1.f64);
}

// _decomp: f1 = x, r4 = where the exponent goes (a 32-bit int).
extern "C" REX_FUNC(sub_825AFF80) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825AFF80(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  int32_t exponent;
  ctx.f1.f64 = c_runtime::Decomp(ctx.f1.f64, &exponent);
  guest::StoreU32(base, ctx.r4.u32, uint32_t(exponent));
}

// pow's integer test: f1 = y; returns r3.
extern "C" REX_FUNC(sub_825A8CE0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A8CE0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.s64 = c_runtime::IntType(ctx.f1.f64);
}

// log: f1 = x.
extern "C" REX_FUNC(sub_825A9C00) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A9C00(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Log(ctx.f1.f64);
}

// pow: f1 = x, f2 = y. (The original also stores both in its caller's
// linkage area, r1 + 16 and + 24, which nothing reads back.)
extern "C" REX_FUNC(sub_825A8ED0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_math)) {
    __imp__sub_825A8ED0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Pow(ctx.f1.f64, ctx.f2.f64);
}
