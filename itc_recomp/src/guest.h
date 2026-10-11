// Helpers for hand-written code that works with guest (Xbox 360) state.
//
// Guest memory is big-endian and addressed by 32-bit guest pointers relative
// to `base`. Guest functions are called through their generated sub_XXXXXXXX
// names, with arguments in ctx registers as the PowerPC ABI expects.

#pragma once

#include "generated/default/itc_funcs.h"

#include <bit>
#include <cstdint>

namespace guest {

inline uint8_t LoadU8(uint8_t* base, uint32_t address) {
  return REX_LOAD_U8(address);
}

inline uint32_t LoadU32(uint8_t* base, uint32_t address) {
  return REX_LOAD_U32(address);
}

inline void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  REX_STORE_U32(address, value);
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
