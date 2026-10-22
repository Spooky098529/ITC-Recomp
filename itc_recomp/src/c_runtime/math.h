// The game's C runtime math functions (Microsoft's, for the Xbox 360),
// natively: the same algorithms and coefficient tables as the translated
// code, step for step, so the results are the same doubles bit for bit
// (NaNs, infinities, denormals and signed zeros included). See the intro of
// docs/functions/c-runtime.md for what that takes: std::fma where the
// PowerPC code has fmadd, fmsub or fnmsub, no other contraction and no
// rewrites that change a NaN's sign (math.cpp builds with -ffp-model=strict
// and -ffp-contract=off: CMakeLists.txt), the host's rounding mode for
// fctid, and flush-to-zero off (the adapters turn it off, as the translated
// code does).

#pragma once

#include <cstdint>

namespace c_runtime {

// asin and acos's shared kernel (sub_825A7FB0): acos when `acos`, else asin.
double AsinAcos(double x, bool acos);
// acos (sub_825A8170): the kernel for acos.
double Acos(double x);
// asin (sub_825A80A8): its own copy of the kernel's asin steps.
double Asin(double x);

// sin (sub_825A88A0) and cos (sub_825A8978): fctid rounds by the host's
// rounding mode (nearest in play), as the translated code's does.
double Sin(double x);
double Cos(double x);

// atan (sub_825A8688) and atan2 (sub_825A8770; y, then x).
double Atan(double x);
double Atan2(double y, double x);

}  // namespace c_runtime
