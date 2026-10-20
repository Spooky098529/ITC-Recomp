// The game's character set conversions (MultiByteToWideChar, sub_82296648;
// WideCharToMultiByte, sub_82296720) for code pages other than UTF-8, as
// plain C++. Their kernel calls (RtlMultiByteToUnicodeN,
// RtlUnicodeToMultiByteN) are answered the SDK's way: bytes widened as they
// are, characters above 255 narrowed to '?'. UTF-8 goes through the game's
// own converters. src/hooks/strings.cpp is the adapter. See B6 in
// docs/rexglue-removal.md.

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace helpers {

enum class Conversion {
  kMeasure,   // no destination: the length is the answer
  kTooSmall,  // ERROR_INSUFFICIENT_BUFFER (122), 0 returned
  kConvert,   // `length` characters converted, `length` returned
};

// `length` the source's characters (the caller gives a terminated string's
// with its terminator for -1), `destination_length` the destination's room
// (0 to measure).
Conversion PlanConversion(int32_t length, int32_t destination_length);

void Widen(const uint8_t* in, uint16_t* out, size_t count);
void Narrow(const uint16_t* in, uint8_t* out, size_t count);

// swprintf (sub_82296870) for its one call site, whose format (in the
// game's data at 0x82009218) has no conversions: the format with "%%" made
// "%". False, with `out` cut there, at any other conversion.
bool FormatWithoutArguments(const std::u16string& format, std::u16string* out);

}  // namespace helpers
