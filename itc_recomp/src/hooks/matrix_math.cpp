// The game's matrix and quaternion math natively: adapters in front of
// src/matrix_math/matrix_math.cpp (docs/functions/matrix-math.md).
// Matrix::Multiply's and Matrix::Copy's adapters are the interpolation's
// wrappers in src/interpolation/blend_state.cpp, Vec4::Transform's the
// diagnostics' wrapper in src/debug/frame_state_diff.cpp (one override per
// function); they read this flag too.

#include "guest.h"

#include "base/flags.h"
#include "matrix_math/matrix_math.h"

ITC_FLAG_BOOL(native_matrix_math, true, "ITC",
              "Run the game's matrix and quaternion math (Matrix::Multiply, Copy and Inverse, "
              "sub_8209BD50, sub_820A0AC0 and sub_820A9988; Matrix::RotateX, Y and Z, "
              "sub_820A65F0, sub_820A67F8 and sub_820A2D30; Vec4::Transform, sub_820AB560; "
              "Vec3::SwapXZ, sub_820AB688; sub_8209B038; the physics' matrices and quaternions, "
              "sub_82293988 to sub_82293F10, sub_822B8F78; SinCos, sub_820B0E10) natively "
              "instead of through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=matrix_math)");

// Define an adapter: `call` uses `ctx` and `base`. Flush-to-zero is left
// as the translated code leaves it: off before scalar work (SCALAR), on
// after the vector unit's (VECTOR).
#define ITC_MATRIX_SCALAR(address, call)       \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_matrix_math)) {      \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    ctx.fpscr.disableFlushMode();             \
    call;                                     \
  }
#define ITC_MATRIX_VECTOR(address, call)       \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_matrix_math)) {      \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    call;                                     \
    ctx.fpscr.enableFlushMode();              \
  }

// Scalar work first, the vector unit's (Matrix::Multiply's) last.
#define ITC_MATRIX_MIXED(address, call)        \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_matrix_math)) {      \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    ctx.fpscr.disableFlushMode();             \
    call;                                     \
    ctx.fpscr.enableFlushMode();              \
  }

using namespace matrix_math;

// r5 = &angle; the turn on the original's stack (its r1 - 288 + 208).
ITC_MATRIX_MIXED(820A65F0, RotateX(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 80))
ITC_MATRIX_MIXED(820A67F8, RotateY(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 80))
ITC_MATRIX_MIXED(820A2D30, RotateZ(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 80))

ITC_MATRIX_VECTOR(820A9988, Inverse(base, ctx.r3.u32, ctx.r4.u32))
ITC_MATRIX_SCALAR(820AB688, Vec3SwapXZ(base, ctx.r3.u32))
ITC_MATRIX_SCALAR(8209B038, SetWsToOne(base, ctx.r3.u32))
// r3 comes back as out (Matrix::MultiplyUnaligned returns it).
ITC_MATRIX_VECTOR(82293988, MultiplyReversed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_MATRIX_SCALAR(82293998, InverseRotate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r3 comes back as out + 48 (the last Vec4::Copy's destination).
ITC_MATRIX_SCALAR(822936E0, SetIdentity(base, ctx.r3.u32); ctx.r3.u64 = ctx.r3.u32 + 48)
// r3 comes back as the quaternion (Quaternion::LengthSquared's argument).
ITC_MATRIX_SCALAR(822936F0, FromQuaternion(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r4.u32)
ITC_MATRIX_SCALAR(82293600, ProjectPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_MATRIX_SCALAR(82293AD8, QuaternionFromMatrix(base, ctx.r3.u32, ctx.r4.u32))
ITC_MATRIX_SCALAR(82293D78, QuaternionMultiply(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_MATRIX_SCALAR(82293F10, QuaternionNormalizeSelf(base, ctx.r3.u32))
// f1 = the angle, r4 = &sine, r5 = &cosine; f1 comes back as the cosine.
ITC_MATRIX_SCALAR(820B0E10, ctx.f1.f64 = SinCos(base, ctx.f1.f64, ctx.r4.u32, ctx.r5.u32))
