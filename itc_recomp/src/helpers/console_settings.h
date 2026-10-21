// The console's settings the game reads (the kernel's ExGetXConfigSetting),
// as plain C++: the answers the SDK gave
// (rexglue-sdk/src/kernel/xboxkrnl/xboxkrnl_xconfig.cpp), fixed. See B5 in
// docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace helpers {

// XCONFIG_USER_VIDEO_FLAGS (category 3, setting 10): Device::Init keeps it
// at the device's +15164.
constexpr uint32_t kVideoFlags = 0x00040000;

// XCONFIG_USER_RETAIL_FLAGS (3, 12).
constexpr uint32_t kRetailFlags = 0x40;

// The time zone settings (3, 1-7): the bias, the first four characters of
// each name, the two dates (month, day, day of week and hour, a byte each)
// and the two biases. All zero: the SDK has no time zone.
struct ConsoleTimeZone {
  int32_t bias = 0;
  uint8_t standard_name[4] = {};
  uint8_t daylight_name[4] = {};
  uint8_t standard_date[4] = {};
  uint8_t daylight_date[4] = {};
  int32_t standard_bias = 0;
  int32_t daylight_bias = 0;
};

constexpr ConsoleTimeZone kConsoleTimeZone{};

// Whether the zone uses daylight saving, as the time zone read
// (sub_8229B440) works it out from the retail flags: bit 1 clear.
constexpr bool UsesDaylight(uint32_t retail_flags) {
  return (retail_flags & 2) == 0;
}

}  // namespace helpers
