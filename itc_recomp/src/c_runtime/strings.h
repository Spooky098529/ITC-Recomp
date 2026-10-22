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

}  // namespace c_runtime
