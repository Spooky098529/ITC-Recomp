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

// write_multi_char (sub_825AF098): `length` characters of `string` through
// write_char, or only counted for a string stream with no buffer. False,
// with nothing written, where the original might reach _flsbuf (no room
// for them all) or _errno (the count reaching -1), or where the buffer's
// bytes would land on the stream's words or the count, or the count on
// the stream (its checks then can't be made first): the original runs.
// `*last` is the last character written (what the original leaves in r3),
// or -1 when none was.
bool WriteMultiChar(uint8_t* base, uint32_t string, int32_t length, uint32_t stream,
                    uint32_t count, int32_t* last);

// _putwc_nolock (sub_825B7A80) for a string stream (flag 0x40): stores the
// UTF-16 unit and moves the pointer on by 2. False, with nothing written,
// for stdout and stderr (the console), a file stream, or no room for two
// bytes (_flswbuf): the original runs.
bool PutwcString(uint8_t* base, uint16_t unit, uint32_t stream);

// The wide write_char (sub_825B1368): a string stream with no buffer only
// counts; else _putwc_nolock, the count set to -1 when the unit is 0xFFFF
// (its failure value) or moved on by one. False where PutwcString is.
bool WriteWideChar(uint8_t* base, uint16_t unit, uint32_t stream, uint32_t count);

// __iob_func (sub_825AD4C0): the C runtime's stream table (_iob: 32 bytes
// a stream, stdin, stdout and stderr first).
constexpr uint32_t kStreams = 0x8266B7D0;

}  // namespace c_runtime
