// The graphics library's reference counts (src/native/refcount.cpp).

#pragma once

#include <cstdint>

namespace refcount {

// sub_822A6738 Resource::AddRef without the recompiler's global lock: adds 1
// to the count at +4 and returns it; at 1, a type 4 object with +0 bit
// 0x40000000 also AddRefs its parent (+24). The vertex shader
// SetVertexShader is binding is left as it is (its count returned).
uint32_t AddRef(uint8_t* base, uint32_t object);

}  // namespace refcount
