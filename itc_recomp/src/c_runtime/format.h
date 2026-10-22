// Pieces of the game's C runtime number formatting and parsing (Microsoft's,
// for the Xbox 360), natively: the 12-byte long double conversions under
// atof and printf's %f, and printf's character output. See
// docs/functions/c-runtime.md.

#pragma once

#include <cstdint>

namespace c_runtime {

// The C runtime's 12-byte long double (_LDBL12), as the guest lays it out:
// +0 the sign and 15-bit exponent (bias 16383), +2 and +6 the 64-bit
// mantissa (its leading bit explicit), +10 16 more bits (ld12tod reads
// them; ___dtold leaves them as they are).
struct LongDouble12 {
  uint16_t sign_exponent;
  uint32_t mantissa_high;
  uint32_t mantissa_low;
};

// ___dtold (sub_825B6378): a double (its bits) to the 12-byte form; a
// denormal normalized, an infinity or NaN to exponent 32767, a zero to
// the sign alone.
LongDouble12 DoubleToLd12(uint64_t bits);

// _ld12tod (sub_825B69B8, _ld12cvt with the double's format at
// 0x8266C514): the 12-byte form (`extra` is the 16 bits at +10) rounded to
// a double's bits; `*status` 0, 1 (overflow: infinity) or 2 (underflow:
// zero or a denormal). Rounds up only past the halfway point (a tie is
// left as it is), as the original.
uint64_t Ld12ToDouble(const LongDouble12& value, uint16_t extra, int32_t* status);

// write_char (sub_825AF000): printf's output of one character to a string
// stream (+0 the pointer, +4 room left, +8 the buffer, +12 flags) with the
// count at `count`: a string stream with no buffer (flag 0x40, +8 = 0)
// only counts; else stores the byte while room is left. Returns 0 when it
// only counted, 1 when it stored, -1 with nothing written where the
// original calls _flsbuf (no room left; never seen in play).
int32_t WriteChar(uint8_t* base, uint32_t character, uint32_t stream, uint32_t count);

// __iob_func (sub_825AD4C0): the C runtime's stream table (_iob: 32 bytes
// a stream, stdin, stdout and stderr first).
constexpr uint32_t kStreams = 0x8266B7D0;

}  // namespace c_runtime
