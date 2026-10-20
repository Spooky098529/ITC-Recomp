// TEMPORARY (rexglue-removal B3): the Swap counter's calls, made by the
// native Swap (src/hooks/graphics_device.cpp). See graphics_boundary_log.cpp.

#pragma once

#include "guest.h"

#include <string>

namespace graphics_boundary {

// What SwapEntered keeps for SwapLeft.
struct SwapEntry {
  bool log = false;
  uint32_t lr = 0, device = 0, front_buffer = 0, parameters = 0;
  std::string fields;
};

// At Swap's entry (r3 device, r4 front buffer, r5 parameters): counts the
// branches and the caller with --log_graphics_boundary, and keeps the fields
// to log at its first calls and powers of two.
SwapEntry SwapEntered(uint8_t* base, uint32_t lr, uint32_t device, uint32_t front_buffer,
                      uint32_t parameters);

// At Swap's exit (r3 what it returns): logs the entry's and exit's fields.
void SwapLeft(uint8_t* base, const SwapEntry& entry, uint32_t result);

}  // namespace graphics_boundary
