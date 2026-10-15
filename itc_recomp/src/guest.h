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

// Byte-swaps count words from in (host memory, contiguous) into out;
// whether any differed from what out held. For CopyU32s.
inline bool SwapU32s(const uint8_t* in, uint32_t* out, uint32_t count) {
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

// Copies count words from guest memory at address into out, in host byte
// order, and returns whether any differed from what out held. One loop the
// compiler vectorizes, unlike LoadU32's volatile loads one by one. The host
// offset is taken once, so a range with words starting both below
// 0xE0000000 and at or above it (where it changes, by each word's start as
// in LoadU32) is copied in two parts.
inline bool CopyU32s(uint8_t* base, uint32_t address, uint32_t* out, uint32_t count) {
  constexpr uint32_t kOffsetChange = 0xE0000000u;
  if (count && address < kOffsetChange &&
      uint64_t(address) + 4 * uint64_t(count - 1) >= kOffsetChange) {
    uint32_t below = (kOffsetChange - address + 3) / 4;
    bool differ = SwapU32s(REX_RAW_ADDR(address), out, below);
    uint32_t rest = address + 4 * below;
    return SwapU32s(REX_RAW_ADDR(rest), out + below, count - below) || differ;
  }
  return SwapU32s(REX_RAW_ADDR(address), out, count);
}

// CopyU32s for floats (no comparison).
inline void SwapFloats(const uint8_t* in, float* out, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    uint32_t value;
    std::memcpy(&value, in + 4 * i, 4);
    out[i] = std::bit_cast<float>(__builtin_bswap32(value));
  }
}

inline void CopyFloats(uint8_t* base, uint32_t address, float* out, uint32_t count) {
  constexpr uint32_t kOffsetChange = 0xE0000000u;
  if (count && address < kOffsetChange &&
      uint64_t(address) + 4 * uint64_t(count - 1) >= kOffsetChange) {
    uint32_t below = (kOffsetChange - address + 3) / 4;
    SwapFloats(REX_RAW_ADDR(address), out, below);
    uint32_t rest = address + 4 * below;
    SwapFloats(REX_RAW_ADDR(rest), out + below, count - below);
    return;
  }
  SwapFloats(REX_RAW_ADDR(address), out, count);
}

// Byte-swaps count words from in into guest memory at out (host memory,
// contiguous). For StoreU32s.
inline void SwapStoreU32s(uint8_t* out, const uint8_t* in, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    uint32_t value;
    std::memcpy(&value, in + 4 * i, 4);
    value = __builtin_bswap32(value);
    std::memcpy(out + 4 * i, &value, 4);
  }
}

// Stores count words (floats or integers, in host byte order) to guest
// memory at address, as StoreU32 would one by one, in one loop the compiler
// vectorizes. Split at 0xE0000000 as CopyU32s.
inline void StoreU32s(uint8_t* base, uint32_t address, const void* words, uint32_t count) {
  const uint8_t* in = static_cast<const uint8_t*>(words);
  constexpr uint32_t kOffsetChange = 0xE0000000u;
  if (count && address < kOffsetChange &&
      uint64_t(address) + 4 * uint64_t(count - 1) >= kOffsetChange) {
    uint32_t below = (kOffsetChange - address + 3) / 4;
    SwapStoreU32s(REX_RAW_ADDR(address), in, below);
    uint32_t rest = address + 4 * below;
    SwapStoreU32s(REX_RAW_ADDR(rest), in + 4 * below, count - below);
    return;
  }
  SwapStoreU32s(REX_RAW_ADDR(address), in, count);
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
