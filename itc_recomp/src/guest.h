// Helpers for hand-written code that works with guest (Xbox 360) state.
//
// Guest memory is big-endian and addressed by 32-bit guest pointers relative
// to `base`. Guest functions are called through their generated sub_XXXXXXXX
// names, with arguments in ctx registers as the PowerPC ABI expects.

#pragma once

#include "generated/default/itc_funcs.h"

#include <bit>
#include <cstdint>
#include <cstring>

namespace guest {

inline uint8_t LoadU8(uint8_t* base, uint32_t address) {
  return REX_LOAD_U8(address);
}

inline void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  REX_STORE_U8(address, value);
}

inline uint32_t LoadU32(uint8_t* base, uint32_t address) {
  return REX_LOAD_U32(address);
}

inline void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  REX_STORE_U32(address, value);
}

// Copies count words from guest memory at address into out, in host byte
// order, and returns whether any differed from what out held. One loop the
// compiler vectorizes, unlike LoadU32's volatile loads one by one (the
// range mustn't cross 0xE0000000, where the host offset changes).
inline bool CopyU32s(uint8_t* base, uint32_t address, uint32_t* out, uint32_t count) {
  const uint8_t* in = REX_RAW_ADDR(address);
  uint32_t differ = 0;
  for (uint32_t i = 0; i < count; i++) {
    uint32_t value;
    std::memcpy(&value, in + 4 * i, 4);
    value = __builtin_bswap32(value);
    differ |= out[i] ^ value;
    out[i] = value;
  }
  return differ != 0;
}

inline float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(REX_LOAD_U32(address));
}

inline void StoreFloat(uint8_t* base, uint32_t address, float value) {
  REX_STORE_U32(address, std::bit_cast<uint32_t>(value));
}

// Reserves space on the guest stack for the lifetime of the object, for
// guest functions that take pointers to locals. Mirrors a guest prologue.
class StackFrame {
 public:
  StackFrame(PPCContext& ctx, uint32_t size) : ctx_(ctx), size_(size) {
    ctx_.r1.u32 -= size_;
  }
  ~StackFrame() { ctx_.r1.u32 += size_; }
  StackFrame(const StackFrame&) = delete;
  StackFrame& operator=(const StackFrame&) = delete;

  // Guest address of a local at `offset` within the frame. Offsets below 80
  // belong to the callee's linkage area, so locals start at 80.
  uint32_t Local(uint32_t offset) const { return ctx_.r1.u32 + offset; }

 private:
  PPCContext& ctx_;
  uint32_t size_;
};

}  // namespace guest
