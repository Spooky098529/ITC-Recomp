// What our code (the lifted functions, the hand ports' entries, the lift
// runtime) takes from the SDK for guest code: the image's layout, guest
// memory access, the function prologue's hint and the global lock (ours). Our own
// copy of what generated/'s itc_pch.h gave (docs/rexglue-removal.md,
// "Before E1: the rewiring", stage 5: generated/ isn't built), kept as it
// was there.

#pragma once

#include <rex/image_info.h>
#include <rex/logging.h>
#include <rex/ppc.h>

#include <atomic>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <mutex>
#include <string_view>

#include "helpers/clock.h"

// The image (default.xex): its base and size, and its code's.
#define REX_IMAGE_BASE 0x82000000ull
#define REX_IMAGE_SIZE 0x9B0000ull
#define REX_CODE_BASE 0x82090000ull
#define REX_CODE_SIZE 0x56C1B8ull

// The SDK's image description for its runtime (src/app/image.cpp): the
// layout above and the functions the kernel calls by address.
extern const rex::PPCImageInfo PPCImageConfig;

// A function's prologue: `base` is 32-byte aligned (lets the compiler fold
// guest addresses).
#if defined(__clang__)
#define REX_FUNC_PROLOGUE() __builtin_assume(((size_t)base & 0x1F) == 0)
#else
#define REX_FUNC_PROLOGUE() ((void)0)
#endif

// Guest memory: big-endian, `base` + the 32-bit guest address (the physical
// range above 0xE0000000 offset by a page where the host maps it so).
#if REX_PLATFORM_WIN32 || (REX_PLATFORM_MAC && REX_ARCH_ARM64)
#define REX_PHYS_HOST_OFFSET(addr) (((u32)(addr) >= 0xE0000000u) ? 0x1000u : 0u)
#else
#define REX_PHYS_HOST_OFFSET(addr) 0u
#endif

#define REX_RAW_ADDR(x) (base + (u32)(x) + REX_PHYS_HOST_OFFSET(x))

#define REX_LOAD_U8(x) (*(volatile u8*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)))
#define REX_LOAD_U16(x) __builtin_bswap16(*(volatile u16*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)))
#define REX_LOAD_U32(x) __builtin_bswap32(*(volatile u32*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)))
#define REX_LOAD_U64(x) __builtin_bswap64(*(volatile u64*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)))
#define REX_LOAD_STRING(x, len) \
  std::string_view(reinterpret_cast<const char*>(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)), (len))

#define REX_STORE_U8(x, y) (*(volatile u8*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)) = (y))
#define REX_STORE_U16(x, y) \
  (*(volatile u16*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)) = __builtin_bswap16(y))
#define REX_STORE_U32(x, y) \
  (*(volatile u32*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)) = __builtin_bswap32(y))
#define REX_STORE_U64(x, y) \
  (*(volatile u64*)(base + (u32)(x) + REX_PHYS_HOST_OFFSET(x)) = __builtin_bswap64(y))

// The guest's time base (mftb): our performance counter at the 360's 50 MHz
// (helpers/clock.h), the SDK's guest clock's rate when its runtime set it.
#define REX_QUERY_TIMEBASE() helpers::PerformanceCounter()

// The stand-in for the 360 turning interrupts off: one process-wide lock
// (ours since E4's step 2; the SDK's kernel, which shared it, no longer
// runs), and a count of how deep it's held (lifted::EnterGlobalLock and the
// like).
inline std::recursive_mutex& ppc_global_lock_() {
  static std::recursive_mutex lock;
  return lock;
}

inline std::atomic<i32>& ppc_global_lock_count_() {
  static std::atomic<i32> count{0};
  return count;
}

#define REX_CHECK_GLOBAL_LOCK()                                        \
  ([&]() -> u64 {                                                      \
    std::lock_guard<std::recursive_mutex> lock_(ppc_global_lock_());   \
    return ppc_global_lock_count_().load() ? 0 : 0x8000;               \
  }())

#define REX_ENTER_GLOBAL_LOCK()             \
  do {                                      \
    ppc_global_lock_().lock();              \
    ppc_global_lock_count_().fetch_add(1);  \
  } while (0)

#define REX_LEAVE_GLOBAL_LOCK()                                                           \
  do {                                                                                    \
    auto old_count_ = ppc_global_lock_count_().fetch_sub(1);                              \
    assert(old_count_ >= 1 && "LeaveGlobalLock called without matching EnterGlobalLock"); \
    ppc_global_lock_().unlock();                                                          \
  } while (0)
