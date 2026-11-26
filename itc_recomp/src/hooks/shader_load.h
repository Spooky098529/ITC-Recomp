// The shader load's adapter (src/graphics/shader_load.h): a guest frame for
// its scratch and the calls it makes, for the draw and the inline draws.

#pragma once

#include <cstdint>

namespace hooks {

// Device::LoadShaders (sub_822AED18) natively: r3 the device, r4 its shader
// dirty mask; `sp` and `r13` the caller's. Returns the original's r3.
uint64_t LoadShaders(uint32_t device, uint64_t mask, uint32_t sp, uint64_t r13);

}  // namespace hooks
