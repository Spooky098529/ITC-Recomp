// The game's C runtime string and character helpers (Microsoft's, for the
// Xbox 360), natively: the same loops over guest memory as the translated
// code, element by element in the same order, so overlapping and unaligned
// strings come out the same. Strings are guest addresses (big-endian UTF-16
// for the wide ones); `base` is guest memory. See
// docs/functions/c-runtime.md.

#pragma once

#include <cstdint>

namespace c_runtime {

// wcslen (sub_825A9958): the UTF-16 units before the terminator.
uint32_t Wcslen(uint8_t* base, uint32_t string);
// wcscpy (sub_825A7F30): copies through the terminator, unit by unit.
void Wcscpy(uint8_t* base, uint32_t destination, uint32_t source);
// wcscat (sub_825A7EF8): copies the source over the destination's terminator.
void Wcscat(uint8_t* base, uint32_t destination, uint32_t source);
// wcscmp (sub_825A7EB0): -1, 0 or 1 by the first differing unit (unsigned).
int32_t Wcscmp(uint8_t* base, uint32_t a, uint32_t b);
// mbtowc's single-byte path (sub_825B7EC8): 0 for no bytes, a count of 0 or
// an empty string (storing 0 when `wide` isn't 0), else stores the byte
// zero-extended and returns 1.
int32_t Mbtowc(uint8_t* base, uint32_t wide, uint32_t bytes, uint32_t count);

// strncpy (sub_825A7F50): copies up to `count` bytes, stopping after the
// terminator, and pads the rest of `count` with zeros.
void Strncpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t count);
// strcpy_s (sub_825A9DD8)'s copy: false, with nothing written, where the
// original goes through _errno (a null destination or source, a size of 0,
// a source that doesn't fit) or where the destination overlaps the source
// above it (the original reads its own writes there); else copies with the
// terminator.
bool StrcpyS(uint8_t* base, uint32_t destination, uint32_t size, uint32_t source);
// strncmp (sub_825AACD0): the difference of the first differing bytes
// (unsigned) within `count`, stopping at the first string's terminator; 0
// for a count of 0 or less (as int32).
int32_t Strncmp(uint8_t* base, uint32_t a, uint32_t b, int32_t count);
// _stricmp (sub_825AA100): A-Z folded to lower case (ASCII only); the
// difference of the first differing bytes (unfolded when the second string
// ended).
int32_t Stricmp(uint8_t* base, uint32_t a, uint32_t b);
// __ascii_strnicmp (sub_825AC6B0): up to `count` bytes with A-Z folded,
// stopping at the first string's terminator; the difference.
int32_t AsciiStrnicmp(uint8_t* base, uint32_t a, uint32_t b, uint32_t count);
// strstr (sub_825A98A8): the first match's offset from `string` (0 for an
// empty `find`), or -1.
int64_t Strstr(uint8_t* base, uint32_t string, uint32_t find);
// The second memcpy (sub_825A7830): bytes until the destination is word
// aligned, then four bytes at a time (each read before it's written), then
// the rest: overlapping copies come out as the original's.
void CopyForward(uint8_t* base, uint32_t destination, uint32_t source, uint32_t count);

// toupper and tolower (sub_825A8600, sub_825A8618; ASCII only, no locale):
// what to add to the character (-32, 32 or 0). The originals add it to
// the whole register.
int32_t ToUpperStep(int32_t c);
int32_t ToLowerStep(int32_t c);

// The character type tables (docs/known-data.md): halfwords, 1 upper, 2
// lower, 4 digit, 8 space, 0x100 alpha, 0x8000 a multibyte lead byte. Read
// from guest memory on every call, as the originals do (the current
// locale's pointer is the game's to change).
constexpr uint32_t kCLocale = 0x8266BB48;       // the C locale's data
constexpr uint32_t kCurrentLocale = 0x8266BC28;  // the current locale's pointer
// isalpha, isdigit and isalnum (sub_825AB020, sub_825AB040, sub_825AB0A0):
// the C locale's entry for c (c as an unsigned index: -1 reads the halfword
// before the table) masked with 0x103, 4 or 0x107.
uint32_t CTypeC(uint8_t* base, uint32_t c, uint32_t mask);
// isleadbyte (sub_825B6898): the current locale's entry for c's low byte,
// & 0x8000.
uint32_t IsLeadByte(uint8_t* base, uint32_t c);

// strtoxl (sub_825A82C8, through strtoul sub_825A85E0): parses `string` in
// `radix` (0 or 2-36) with `flags` (1 unsigned): spaces by the current
// locale's table, a sign, a 0x prefix or the base guessed. False where the
// original goes through _errno (a null string, a base out of range, a
// number out of range) or tests spaces for a multibyte locale
// (sub_825AFD98): those stay on the recompiled path. Else `result` is r3
// as the original leaves it (negated in 64 bits for a minus sign) and `end`
// where it stopped (the string itself when no digits were read).
bool StrToXl(uint8_t* base, uint32_t string, int32_t radix, uint32_t flags, bool want_end,
             uint32_t* end, uint64_t* result);

// memcpy (sub_825A7410)'s copy: the bytes copied with the host's memcpy,
// the same bytes the original's byte, word and doubleword loops leave when
// the two ranges don't share memory. False, with nothing copied, where they
// might: ranges that overlap in the memory behind them (the guest views that
// share memory, the xex's two and the physical ones, counted as one), or a
// range that crosses from one view into another; the original runs there.
bool Memcpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size);

// memmove (sub_825AA840)'s backward copy, for a destination above the source
// (as signed words: the original's cmpw; within one view that's the address
// order): the host's memmove where both ranges are in one view, the host's
// memcpy where they don't share memory. False, with nothing copied, where a
// range crosses into another view or the two share memory through different
// views; the original runs there. (A destination below the source is the
// original's tail call to memcpy, sub_825A7410.)
bool MemmoveDown(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size);

// XAPI's vectorized memset (sub_822962B0, probably XMemSet): `bytes` bytes
// at `destination` set to `value`'s low byte. Its 16-byte store loops step
// by the word at 0x8263830C (kXMemSetStride, 16 in the xex): with 16 the
// bytes are a plain memset's (the 128-byte lines it clears with dcbzl lie
// wholly inside the range); with any other step the same stores, line
// clears and edge bytes as the translated code, in its order.
constexpr uint32_t kXMemSetStride = 0x8263830C;
void XMemSet(uint8_t* base, uint32_t destination, uint32_t value, uint32_t bytes);

// Whether `size` bytes at `address` lie in one guest view (not crossing
// into the next one).
bool InOneView(uint32_t address, uint32_t size);
// Whether two ranges might share memory behind the guest views: true where
// they do, or where either crosses into another view (then it can't be
// told).
bool MayShare(uint32_t a, uint32_t a_size, uint32_t b, uint32_t b_size);

}  // namespace c_runtime
