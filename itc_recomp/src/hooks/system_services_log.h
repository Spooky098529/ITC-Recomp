// TEMPORARY (rexglue-removal B5): logging for the two system service
// functions that already have overrides (see system_services_log.cpp).

#pragma once

#include <cstdint>

namespace system_services_log {

// The console's time zone read (sub_8229B440): the zone written, the
// "uses daylight saving" word and the result.
void TimeZoneRead(uint8_t* base, uint32_t lr, uint32_t zone, uint32_t uses_daylight,
                  uint32_t result);

// Device::Init's ExGetXConfigSetting call: what the SDK answers.
void ConfigSetting(uint8_t* base, uint32_t category, uint32_t setting, uint32_t buffer,
                   uint32_t size, uint32_t result);

}  // namespace system_services_log
