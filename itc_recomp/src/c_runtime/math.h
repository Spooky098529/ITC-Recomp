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

// The floating-point helpers pow uses, on the double's bits:
// copysign (sub_825AB1C8): magnitude's bits with sign's sign bit.
double Copysign(double magnitude, double sign);
// _set_exp (sub_825AFE80): the biased exponent set to exponent + 1022 (as a
// 12-bit field: an overflow reaches the sign bit, as the original's).
double SetExp(double x, int32_t exponent);
// _get_exp (sub_825AFEA8): the biased exponent less 1022.
int32_t GetExp(double x);
// _decomp (sub_825AFF80): the mantissa in [0.5, 1) with x's sign, and the
// exponent (0 for a zero, which is returned as it is; denormals normalized).
double Decomp(double x, int32_t* exponent);
// pow's integer test (sub_825A8CE0): 2 for an even integer, 1 for an odd
// one, 0 otherwise (denormals included).
int32_t IntType(double y);

// log (sub_825A9C00) and pow (sub_825A8ED0; x to the y). pow's infinity
// cases (sub_825A8D50, never seen in play) are ported with it.
double Log(double x);
double Pow(double x, double y);

// floor (sub_825A8CA0) and ceil (sub_825AA0B8): by truncation (fctidz);
// |x| from 1e18 (the bound at 0x82072430, not 2^52) and NaNs returned as
// they are.
double Floor(double x);
double Ceil(double x);

// log10 (sub_825A9D80): log(x) times 1/ln 10 (the double at 0x820725A0).
double Log10(double x);

// __ultod (sub_825A9DB0): an unsigned 64-bit integer to a double, its bits
// built by hand: the low bits beyond the mantissa dropped (truncated, not
// rounded as double(uint64_t) would).
double U64ToDouble(uint64_t value);

// _fpclass (sub_825AB268): the class bits (512 +infinity, 4 -infinity, 2
// quiet NaN, 1 signaling NaN, 128 and 16 denormals, 64 and 32 zeros, 256 and
// 8 normals: positive, negative), with its helper sub_825AFEF8 (never seen in
// play) ported.
int32_t FpClass(double x);

}  // namespace c_runtime
