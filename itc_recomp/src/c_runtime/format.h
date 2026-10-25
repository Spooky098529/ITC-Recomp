// Pieces of the game's C runtime number formatting and parsing (Microsoft's,
// for the Xbox 360), natively: the 12-byte long double conversions under
// atof and printf's %f, and printf's character output. See
// docs/functions/c-runtime.md.

#pragma once

#include <cstdint>
#include <vector>

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

// A guest buffer worked on in host memory, for the %f text's steps
// (_fptostr, _cftof2_l): bytes are read from the guest the first time
// they're needed, and the writes kept until Commit, so the adapters can
// decide whether to run the original after the work is done. Offsets are
// from the buffer's address; past 1 MB (or the end of guest memory) the
// text is no longer ok and nothing is committed.
class GuestText {
 public:
  GuestText(uint8_t* base, uint32_t address) : base_(base), address_(address) {}
  bool ok() const { return ok_; }
  uint8_t Get(uint64_t at);
  void Set(uint64_t at, uint8_t value);
  // strlen at `at`.
  uint32_t Length(uint64_t at);
  // memmove (sub_825AA840)'s result: `count` bytes from `from` to `to`.
  void Move(uint64_t to, uint64_t from, uint32_t count);
  // memset (sub_825A71D0).
  void Fill(uint64_t at, uint8_t value, uint32_t count);
  // One past the last byte written (0 when none was).
  uint64_t WrittenEnd() const { return end_; }
  // Writes the bytes set into guest memory. False, with nothing written,
  // when the text isn't ok, or the bytes written cross from one guest view
  // into another or reach the physical views (0xA0000000 and up, where
  // memmove's copies through memcpy, sub_825A7410, are recorded for the
  // renderer's mirrors): the original runs there.
  bool Commit();

 private:
  bool Reach(uint64_t at);

  uint8_t* base_;
  uint32_t address_;
  bool ok_ = true;
  uint64_t end_ = 0;
  std::vector<uint8_t> bytes_, known_, written_;
};

// _fltout2's result (STRFLT) as the %f steps use it: +0 the sign character
// (a word: '-' or ' '), +4 the decimal exponent, +12 the digits.
constexpr int32_t kMinus = '-';

// _fptostr (sub_825B61E8): `digits` (as many as asked, '0' past the end of
// `mantissa`, which holds the digits without their terminator) after a
// leading '0' at `at` in `text`, rounded on the next digit (nines carried);
// a carry into the leading '0' adds one to `*exponent`, else the leading
// '0' is taken out. -1, with `text` unchanged, where the original goes
// through _errno (a size of 0, or no room for the digits and the
// terminator: EINVAL, ERANGE); else 0.
int32_t FpToStr(GuestText& text, uint64_t at, uint32_t size, int32_t digits, int32_t* exponent,
                const std::vector<uint8_t>& mantissa);

// _cftof2_l (sub_825AE708): _fptostr's digits in `text` laid out as %f: the
// sign, a '0' before the point when the exponent is 0 or less, the point
// (`point`, the locale's) when the precision is above 0, zeros after it for
// a negative exponent (as many as the precision allows; all of them for
// %g, `g_format`). For %g with the exponent one above the precision, the
// digit after it is set to '0' and ended first. False where an offset would
// fall before the buffer, or the text isn't ok.
bool Cftof2(GuestText& text, int32_t precision, int32_t sign, int32_t exponent, bool g_format,
            uint8_t point);

// The locale's decimal point: the byte at **0x8266C024, read from guest
// memory on every call (the locale is the game's to change). `words`, when
// given, gets the two pointers' addresses and the byte's.
uint32_t DecimalPointAddress(uint8_t* base, uint32_t* words = nullptr);

// _cftof_l (sub_825AE8E0, %f): the double's digits by _fltout2 (___dtold,
// $I10_OUTPUT with 17 digits), then _fptostr (as many digits as the
// exponent and the precision give, after the sign) and _cftof2_l, into the
// guest buffer. False, with nothing written, where the original goes
// through _errno (a null buffer, a size of 0, no room), where $I10_OUTPUT
// or the commit isn't native, or where the decimal point lies in what's
// written: the original runs there.
bool Cftof(uint8_t* base, uint64_t bits, uint32_t buffer, uint32_t size, int32_t precision);

// _vswprintf_l (sub_825A9980, from vswprintf sub_825A9A80) with _woutput_l
// (sub_825B13D0, wide_format.cpp): `format` (UTF-16) with the arguments at
// `args` (a va_list: 8-byte slots) into `buffer` through a string stream of
// INT_MAX bytes, then the two-byte terminator; `*count` the characters
// written (or -1). The work in `frame` (the original _woutput_l's r1, all
// below the caller's stack pointer): its text buffer at +128, as the
// original. A dry run first: false, with nothing written to `buffer`, where
// the original must run: a null buffer or format, %n, a counted string
// (%Z), a float conversion other than %f or one needing the heap (a
// precision above 163) or a forced point, %f's _cftof_l not native for the
// value, or the unit 0xFFFF written (the original's error paths read errno).
bool Vswprintf(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
               int32_t* count);

// __iob_func (sub_825AD4C0): the C runtime's stream table (_iob: 32 bytes
// a stream, stdin, stdout and stderr first).
constexpr uint32_t kStreams = 0x8266B7D0;

}  // namespace c_runtime
