// What our code (the lifted functions, the hand ports' entries, the lift
// runtime) needs for guest code: the image's layout, guest memory access,
// the function prologue's hint and the global lock. Our own copy of what
// generated/'s itc_pch.h gave (docs/rexglue-removal.md, "Before E1: the
// rewiring", stage 5: generated/ isn't built), kept as it was there; no
// SDK header since E4 ("E4: the plan", step 3).

#pragma once

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

// The short integer names the generated code (and so the lifted code) uses.
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

// A function's prologue: `base` is 32-byte aligned (lets the compiler fold
// guest addresses).
#if defined(__clang__)
#define REX_FUNC_PROLOGUE() __builtin_assume(((size_t)base & 0x1F) == 0)
#else
#define REX_FUNC_PROLOGUE() ((void)0)
#endif

// Guest memory is read and written through guest.h's guest::Load*,
// guest::Store* and guest::Pointer (docs/e5-native.md, step 0: one spelling
// for lifted code and hand ports).

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
