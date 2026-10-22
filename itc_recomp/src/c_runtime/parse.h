// The C runtime's 12-byte long double arithmetic (Microsoft's, for the Xbox
// 360), natively, step for step as the translated code: the parsing under
// atof (the decimal string to the 12-byte long double: __strgtold12_l,
// with __mtold12 and the multiplication by powers of ten it inlines,
// _fltin2 and _atof_l) and the decimal digits under printf's %f
// ($I10_OUTPUT and _fltout2). See docs/functions/c-runtime.md.

#pragma once

#include "c_runtime/format.h"

#include <cstdint>

namespace c_runtime {

// The 12-byte long double as the guest lays it out, all of it: `value`
// (+0 to +9) and the 16 bits at +10.
struct LongDouble12Full {
  LongDouble12 value;
  uint16_t extra;
};

// __mtold12 (sub_825B7F10): `count` digits (bytes, added as signed) made
// into a 96-bit integer (times 10 plus the digit, modulo 2^96), normalized
// so bit 79 is set, its exponent (16462 less the shifts) written over the
// top 16 bits. False for a mantissa of 0, where the original never ends.
bool Mtold12(const uint8_t* digits, uint32_t count, LongDouble12Full* out);

// __strgtold12_l (sub_825B36B8): parses the decimal number at `string`
// (spaces, tabs, line feeds and returns skipped; a sign; digits with the
// decimal point read through 0x8266C024; an exponent after d, D, e or E,
// or after a sign when `implicit_e` isn't 0) into the 12-byte long double.
// `mult12`, `scale` and `decpt` as the original's: `scale` added to the
// exponent when none was given, `decpt` taken off when there's no decimal
// point, the 16 bits at +10 cleared before scaling when `mult12` is 0.
// `end` is where parsing stopped (the string itself when nothing was
// parsed); the result's flags: 4 nothing parsed, 2 overflow, 1 underflow.
// False where a digit buffer would be read past its start (never: its
// first digit isn't 0).
struct Strgtold12Result {
  LongDouble12Full value;
  uint32_t end;
  uint32_t flags;
};
bool Strgtold12(uint8_t* base, uint32_t string, uint32_t mult12, uint32_t scale, uint32_t decpt,
                uint32_t implicit_e, Strgtold12Result* result);

// _fltin2 (sub_825AFDC8): __strgtold12_l with no scaling, then _ld12tod:
// `flags` 512 nothing parsed (the double 0), 128 overflow, 256 underflow;
// `used` the characters parsed; `bits` the double.
struct FltinResult {
  uint32_t flags;
  uint32_t used;
  uint64_t bits;
};
bool Fltin2(uint8_t* base, uint32_t string, FltinResult* result);

// _atof_l (sub_825AB0C0, and atof sub_825AB1C0): spaces skipped by the
// current locale's character type table (0x8266BC28), then _fltin2's
// double. False, with nothing written, where the original goes through
// _errno (a null string) or tests spaces for a multibyte locale
// (sub_825AFD98, never seen in play).
bool AtofL(uint8_t* base, uint32_t string, uint64_t* bits);

// $I10_OUTPUT (sub_825B8B48)'s FOS (+0 the decimal exponent, +2 the sign
// character, +3 the digits' count, +4 the digits and a terminator; 26
// bytes) as the original leaves it: only the bytes it writes are marked.
struct FloatOut {
  uint8_t bytes[26];
  bool written[26];
  int32_t result;  // r3: 1 a number, 0 an infinity or NaN
};

// $I10_OUTPUT: the long double's first `digits` decimal digits (at most 21;
// with `flags` & 1, that many after the point), rounded on one more: an
// estimate of the decimal exponent from the binary one, the number scaled
// by the powers of ten and corrected by a tenth, the digits by
// multiplications by ten; "1#INF", "1#IND", "1#QNAN" and "1#SNAN" (read
// from guest memory) for the special values. False where the original
// would read a byte it didn't write (no digits asked for) or strcpy_s
// fails (never: the strings fit).
bool I10Output(uint8_t* base, const LongDouble12& value, int32_t digits, uint32_t flags,
               FloatOut* out);

}  // namespace c_runtime
