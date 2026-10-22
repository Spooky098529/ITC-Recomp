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

}  // namespace c_runtime
