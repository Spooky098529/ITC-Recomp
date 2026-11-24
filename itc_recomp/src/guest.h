// Helpers for hand-written code that works with guest (Xbox 360) state.
//
// Guest memory is big-endian and addressed by 32-bit guest pointers relative
// to `base`. Guest functions are called through the dispatch table by
// address (lifted::Call; docs/rexglue-removal.md, "Before E1: the
// rewiring").

#pragma once

#include "guest_code.h"
#include "memory/handles.h"

#include <bit>
#include <cstdint>
#include <cstring>

#if defined(__SSSE3__)
#include <tmmintrin.h>
#endif

// Our register view (lifted/regs.h), declared here for the steps' and
// helpers' declarations that take one.
namespace lifted {
struct Regs;
}  // namespace lifted

namespace memory {
// A raw load or store reaching a host object through its handle
// (memory/host_copy.cpp): the bytes as the guest's would give them; logged
// once per type and offset (code still in guest shape, to convert).
uint64_t RawHandleLoad(uint32_t address, uint32_t width);
void RawHandleStore(uint32_t address, uint32_t width, uint64_t value);
// The same without the log line (memory/host_copy.h).
uint64_t OwnerLoad(uint32_t address, uint32_t width);
}  // namespace memory

namespace memory {
// The calling thread's own stack (E5 step 6): a blob (memory/handles.h), so
// its frames live in PC memory; set by our threads (threads/threads.cpp) when
// they start. Its bytes are found here first, without the handles' lookup:
// the stack is what the game's code touches most.
struct StackSpan {
  uint32_t start = 0;
  uint32_t size = 0;
  uint8_t* bytes = nullptr;
};
inline thread_local StackSpan t_stack;

// The physical memory (E5 step 7): the GPU's, mapped on its own outside
// guest memory (memory/mapping.cpp). The guest's addresses for it are
// translated: 0x7F000000 (physical 0-16 MB), 0xA0000000 and 0xC0000000 (the
// physical address), 0xE0000000 (the physical address + 4 KB).
inline uint8_t* g_physical = nullptr;
inline bool IsPhysical(uint32_t address) {
  return address >= 0xA0000000u || address - 0x7F000000u < 0x1000000u;
}
inline uint8_t* PhysicalPointer(uint32_t address) {
  if (address >= 0xE0000000u) {
    return g_physical + (address - 0xE0000000u) + 0x1000;
  }
  if (address >= 0xA0000000u) {
    return g_physical + (address & 0x1FFFFFFFu);
  }
  return g_physical + (address - 0x7F000000u);
}

// The host bytes of `width` bytes at `address` in the calling thread's
// stack, or nullptr.
inline uint8_t* StackBytes(uint32_t address, uint32_t width) {
  const uint32_t offset = address - t_stack.start;
  if (offset < t_stack.size && t_stack.size - offset >= width) [[likely]] {
    return t_stack.bytes + offset;
  }
  return nullptr;
}
}  // namespace memory

namespace guest {

// Guest memory's one spelling, for lifted code and hand ports alike
// (docs/e5-native.md, step 0): `base` + the 32-bit guest address,
// big-endian, through volatile accesses (as the generated code's macros
// did: the compiler neither merges nor drops them). The physical range
// above 0xE0000000 is mapped at physical + 4 KB by the views' layout
// (src/memory/mapping.cpp): no offset to add here.

// The host address of a guest address.
// A blob's handle (memory/handles.h: a file's data in PC memory, in the
// guest's byte order) gives the blob's bytes; a typed host object's handle
// gives base + handle, which faults (and names the object) when used.
inline uint8_t* Pointer(uint8_t* base, uint32_t address) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    return memory::PhysicalPointer(address);
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (uint8_t* bytes = memory::StackBytes(address, 1)) {
      return bytes;
    }
    if (uint8_t* bytes = memory::BlobPointer(address)) {
      return bytes;
    }
  }
  return base + address;
}

inline uint8_t LoadU8(uint8_t* base, uint32_t address) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    uint8_t v;
    std::memcpy(&v, memory::PhysicalPointer(address), 1);
    return v;
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (const uint8_t* p = memory::StackBytes(address, 1)) {
      uint8_t v;
      std::memcpy(&v, p, 1);
      return v;
    }
    return uint8_t(memory::RawHandleLoad(address, 1));
  }
  return *reinterpret_cast<volatile uint8_t*>(base + address);
}

inline void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    const uint8_t v = value;
    std::memcpy(memory::PhysicalPointer(address), &v, 1);
    return;
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (uint8_t* p = memory::StackBytes(address, 1)) {
      const uint8_t v = value;
      std::memcpy(p, &v, 1);
      return;
    }
    memory::RawHandleStore(address, 1, value);
    return;
  }
  *reinterpret_cast<volatile uint8_t*>(base + address) = value;
}

inline uint16_t LoadU16(uint8_t* base, uint32_t address) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    uint16_t v;
    std::memcpy(&v, memory::PhysicalPointer(address), 2);
    return __builtin_bswap16(v);
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (const uint8_t* p = memory::StackBytes(address, 2)) {
      uint16_t v;
      std::memcpy(&v, p, 2);
      return __builtin_bswap16(v);
    }
    return uint16_t(memory::RawHandleLoad(address, 2));
  }
  return __builtin_bswap16(*reinterpret_cast<volatile uint16_t*>(base + address));
}

inline void StoreU16(uint8_t* base, uint32_t address, uint16_t value) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    const uint16_t v = __builtin_bswap16(value);
    std::memcpy(memory::PhysicalPointer(address), &v, 2);
    return;
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (uint8_t* p = memory::StackBytes(address, 2)) {
      const uint16_t v = __builtin_bswap16(value);
      std::memcpy(p, &v, 2);
      return;
    }
    memory::RawHandleStore(address, 2, value);
    return;
  }
  *reinterpret_cast<volatile uint16_t*>(base + address) = __builtin_bswap16(value);
}

inline uint32_t LoadU32(uint8_t* base, uint32_t address) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    uint32_t v;
    std::memcpy(&v, memory::PhysicalPointer(address), 4);
    return __builtin_bswap32(v);
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (const uint8_t* p = memory::StackBytes(address, 4)) {
      uint32_t v;
      std::memcpy(&v, p, 4);
      return __builtin_bswap32(v);
    }
    return uint32_t(memory::RawHandleLoad(address, 4));
  }
  return __builtin_bswap32(*reinterpret_cast<volatile uint32_t*>(base + address));
}

inline void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    const uint32_t v = __builtin_bswap32(value);
    std::memcpy(memory::PhysicalPointer(address), &v, 4);
    return;
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (uint8_t* p = memory::StackBytes(address, 4)) {
      const uint32_t v = __builtin_bswap32(value);
      std::memcpy(p, &v, 4);
      return;
    }
    memory::RawHandleStore(address, 4, value);
    return;
  }
  *reinterpret_cast<volatile uint32_t*>(base + address) = __builtin_bswap32(value);
}

inline uint64_t LoadU64(uint8_t* base, uint32_t address) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    uint64_t v;
    std::memcpy(&v, memory::PhysicalPointer(address), 8);
    return __builtin_bswap64(v);
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (const uint8_t* p = memory::StackBytes(address, 8)) {
      uint64_t v;
      std::memcpy(&v, p, 8);
      return __builtin_bswap64(v);
    }
    return uint64_t(memory::RawHandleLoad(address, 8));
  }
  return __builtin_bswap64(*reinterpret_cast<volatile uint64_t*>(base + address));
}

inline void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  if (memory::IsPhysical(address)) [[unlikely]] {
    const uint64_t v = __builtin_bswap64(value);
    std::memcpy(memory::PhysicalPointer(address), &v, 8);
    return;
  }
  if (memory::IsHandle(address)) [[unlikely]] {
    if (uint8_t* p = memory::StackBytes(address, 8)) {
      const uint64_t v = __builtin_bswap64(value);
      std::memcpy(p, &v, 8);
      return;
    }
    memory::RawHandleStore(address, 8, value);
    return;
  }
  *reinterpret_cast<volatile uint64_t*>(base + address) = __builtin_bswap64(value);
}

#if defined(__SSSE3__)
// Reverses the bytes of each of a vector's four words (pshufb), written by
// hand: the compiler left the loops below scalar once inlined into their
// callers (seen in PixelConstantBlender::Apply's disassembly).
inline __m128i SwapVector(__m128i v) {
  const __m128i kReverse = _mm_setr_epi8(3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
  return _mm_shuffle_epi8(v, kReverse);
}
#endif

// Byte-swaps count words from in (host memory, contiguous) into out (four
// at a time, then the rest one by one): the words already swapped.
inline uint32_t SwapWords(const uint8_t* in, uint8_t* out, uint32_t count) {
  uint32_t i = 0;
#if defined(__SSSE3__)
  for (; i + 4 <= count; i += 4) {
    __m128i v = _mm_loadu_si128(reinterpret_cast<const __m128i*>(in + 4 * size_t(i)));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(out + 4 * size_t(i)), SwapVector(v));
  }
#endif
  return i;
}

// Byte-swaps count words from in (host memory, contiguous) into out;
// whether any differed from what out held. For CopyU32s.
inline bool SwapU32s(const uint8_t* in, uint32_t* out, uint32_t count) {
  uint32_t differ = 0;
  uint32_t i = 0;
#if defined(__SSSE3__)
  __m128i differ_vector = _mm_setzero_si128();
  for (; i + 4 <= count; i += 4) {
    __m128i v = SwapVector(_mm_loadu_si128(reinterpret_cast<const __m128i*>(in + 4 * size_t(i))));
    __m128i* o = reinterpret_cast<__m128i*>(out + i);
    differ_vector = _mm_or_si128(differ_vector, _mm_xor_si128(_mm_loadu_si128(o), v));
    _mm_storeu_si128(o, v);
  }
  differ = uint32_t(_mm_movemask_epi8(_mm_cmpeq_epi8(differ_vector, _mm_setzero_si128())) != 0xFFFF);
#endif
  for (; i < count; i++) {
    uint32_t value;
    std::memcpy(&value, in + 4 * i, 4);
    value = __builtin_bswap32(value);
    differ |= out[i] ^ value;
    out[i] = value;
  }
  return differ != 0;
}

// Copies count words from guest memory at address into out, in host byte
// order, and returns whether any differed from what out held. One vectorized
// loop, unlike LoadU32's volatile loads one by one. The host
// offset is taken once, so a range with words starting both below
// 0xE0000000 and at or above it (where it changes, by each word's start as
// in LoadU32) is copied in two parts.
inline bool CopyU32s(uint8_t* base, uint32_t address, uint32_t* out, uint32_t count) {
  constexpr uint32_t kOffsetChange = 0xE0000000u;
  if (count && address < kOffsetChange &&
      uint64_t(address) + 4 * uint64_t(count - 1) >= kOffsetChange) {
    uint32_t below = (kOffsetChange - address + 3) / 4;
    bool differ = SwapU32s(guest::Pointer(base, address), out, below);
    uint32_t rest = address + 4 * below;
    return SwapU32s(guest::Pointer(base, rest), out + below, count - below) || differ;
  }
  return SwapU32s(guest::Pointer(base, address), out, count);
}

// CopyU32s for floats (no comparison).
inline void SwapFloats(const uint8_t* in, float* out, uint32_t count) {
  for (uint32_t i = SwapWords(in, reinterpret_cast<uint8_t*>(out), count); i < count; i++) {
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
    SwapFloats(guest::Pointer(base, address), out, below);
    uint32_t rest = address + 4 * below;
    SwapFloats(guest::Pointer(base, rest), out + below, count - below);
    return;
  }
  SwapFloats(guest::Pointer(base, address), out, count);
}

// Byte-swaps count words from in into guest memory at out (host memory,
// contiguous). For StoreU32s.
inline void SwapStoreU32s(uint8_t* out, const uint8_t* in, uint32_t count) {
  for (uint32_t i = SwapWords(in, out, count); i < count; i++) {
    uint32_t value;
    std::memcpy(&value, in + 4 * i, 4);
    value = __builtin_bswap32(value);
    std::memcpy(out + 4 * i, &value, 4);
  }
}

// The host memory for `bytes` bytes of guest memory at `address`, as one or
// two contiguous parts (split at 0xE0000000, where the host offset changes, as
// in CopyU32s): calls part(host, offset into the range, part's bytes). For
// bulk copies such as file reads (no byte swap).
template <typename Part>
inline void ForEachHostPart(uint8_t* base, uint32_t address, uint32_t bytes, Part part) {
  constexpr uint32_t kOffsetChange = 0xE0000000u;
  if (bytes && address < kOffsetChange && uint64_t(address) + bytes > kOffsetChange) {
    uint32_t below = kOffsetChange - address;
    part(guest::Pointer(base, address), 0u, below);
    part(guest::Pointer(base, kOffsetChange), below, bytes - below);
    return;
  }
  part(guest::Pointer(base, address), 0u, bytes);
}

// Stores count words (floats or integers, in host byte order) to guest
// memory at address, as StoreU32 would one by one, in one vectorized loop.
// Split at 0xE0000000 as CopyU32s.
inline void StoreU32s(uint8_t* base, uint32_t address, const void* words, uint32_t count) {
  const uint8_t* in = static_cast<const uint8_t*>(words);
  constexpr uint32_t kOffsetChange = 0xE0000000u;
  if (count && address < kOffsetChange &&
      uint64_t(address) + 4 * uint64_t(count - 1) >= kOffsetChange) {
    uint32_t below = (kOffsetChange - address + 3) / 4;
    SwapStoreU32s(guest::Pointer(base, address), in, below);
    uint32_t rest = address + 4 * below;
    SwapStoreU32s(guest::Pointer(base, rest), in + 4 * below, count - below);
    return;
  }
  SwapStoreU32s(guest::Pointer(base, address), in, count);
}

inline float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(guest::LoadU32(base, address));
}

inline void StoreFloat(uint8_t* base, uint32_t address, float value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(value));
}

// An object's vtable word (its +0) for a virtual call (docs/loops/e5-loop.md,
// step 4): from guest memory, or from the host object a handle names (host
// structs keep the guest offsets and hold the word, still the image table's
// guest address, in host order). An interface's address inside an object
// (handle + offset) reads that interface's word.
inline uint32_t VtableWord(uint8_t* base, uint32_t object) {
  if (memory::IsHandle(object)) {
    // An object inside a blob (E5 step 5: the save slots in their operator
    // new[] block) keeps the guest's byte order.
    if (memory::BlobPointer(object)) [[unlikely]] {
      return guest::LoadU32(base, object);
    }
    // A host object, or one inside a host owner: the word as the owner's
    // layout orders it (memory::OwnerLoad).
    return uint32_t(memory::OwnerLoad(object, 4));
  }
  return guest::LoadU32(base, object);
}

// Reserves space on the guest stack for the lifetime of the object, for
// guest functions that take pointers to locals. Mirrors a guest prologue.
// `Context` is our register view (lifted::Regs).
template <typename Context>
class StackFrame {
 public:
  StackFrame(Context& ctx, uint32_t size) : ctx_(ctx), size_(size) {
    ctx_.r1.u32 -= size_;
  }
  ~StackFrame() { ctx_.r1.u32 += size_; }
  StackFrame(const StackFrame&) = delete;
  StackFrame& operator=(const StackFrame&) = delete;

  // Guest address of a local at `offset` within the frame. Offsets below 80
  // belong to the callee's linkage area, so locals start at 80.
  uint32_t Local(uint32_t offset) const { return ctx_.r1.u32 + offset; }

 private:
  Context& ctx_;
  uint32_t size_;
};

}  // namespace guest
