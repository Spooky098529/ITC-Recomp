// The graphics library's reference count on its objects (shaders and other
// resources; the count is at +4): AddRef sub_822A6738 and Release
// sub_822A67B0. Both are a lwarx/stwcx loop, which the recompiler wraps in
// the process-wide global lock (mfmsr/mtmsrd become REX_ENTER/LEAVE_GLOBAL_
// LOCK, and the mfmsr locks it once more), taken twice per call: ~6% of the
// render thread, mostly SetVertexShader's AddRef and Release. The stwcx is
// already a compare-and-swap on the guest word, and no code changes the
// count under the global lock by a plain store (see "The shader reference
// count" in docs/native-renderer.md), so the same loop without the lock is
// still atomic with every other thread's.
//
// The behavior is otherwise the library's: AddRef returns the new count
// and, at 1, also AddRefs the object at +24 for type 4 objects with +0 bit
// 0x40000000; Release returns the new count, and at 0 releases +24 the same
// way, calls sub_822A6210 on the object (its destroy) and returns 0.
//
// SetVertexShader (sub_822A8808) AddRefs the new shader and Releases the
// one bound (device +12436), also when they're the same object. Then the
// pair changes nothing (the device's own reference keeps the count above 0
// throughout), so both are skipped; the rest of the call runs as before.
// How many calls bind the same shader is logged every 5 s.
//
// --refcount_global_lock=true runs the library's own functions instead
// (for the recompiled callers; native callers, src/graphics/, call
// refcount::AddRef, lock-free, either way).

#include "native/refcount.h"

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"

#include <chrono>

ITC_FLAG_BOOL(refcount_global_lock, false, "ITC",
              "Let the graphics library's AddRef and Release take the recompiler's "
              "global lock as before (they're lock-free otherwise)");

namespace {

constexpr uint32_t kCount = 4;
constexpr uint32_t kChild = 24;
constexpr uint32_t kDeviceVertexShader = 12436;

// The object whose AddRef and Release SetVertexShader makes no-ops.
thread_local uint32_t g_unchanged_shader = 0;

// Counted per thread without atomics: a locked instruction costs a few
// hundred cycles on the render thread (it waits for the thread's stores).
thread_local uint64_t g_set_calls = 0, g_set_same = 0;
thread_local std::chrono::steady_clock::time_point g_last_log;

// Adds `delta` to the big-endian count at `address`; returns the new count.
uint32_t AddToCount(uint8_t* base, uint32_t address, int32_t delta) {
  uint32_t* word = reinterpret_cast<uint32_t*>(REX_RAW_ADDR(address));
  uint32_t old_raw = __atomic_load_n(word, __ATOMIC_RELAXED);
  uint32_t value;
  do {
    value = __builtin_bswap32(old_raw) + delta;
  } while (!__atomic_compare_exchange_n(word, &old_raw, __builtin_bswap32(value), false,
                                        __ATOMIC_SEQ_CST, __ATOMIC_RELAXED));
  return value;
}

// Type 4 with +0 bit 0x40000000: the object holds a reference on +24.
bool HoldsChild(uint8_t* base, uint32_t object) {
  uint32_t common = guest::LoadU32(base, object);
  return (common & 0xF) == 4 && (common & 0x40000000);
}

}  // namespace

namespace refcount {

uint32_t AddRef(uint8_t* base, uint32_t object) {
  if (object == g_unchanged_shader) {
    return guest::LoadU32(base, object + kCount);  // unused by the caller
  }
  uint32_t count = AddToCount(base, object + kCount, 1);
  if (count == 1 && HoldsChild(base, object)) {
    AddRef(base, guest::LoadU32(base, object + kChild));
  }
  return count;
}

}  // namespace refcount

extern "C" REX_FUNC(sub_822A6738) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(refcount_global_lock)) {
    __imp__sub_822A6738(ctx, base);
    return;
  }
  ctx.r3.u64 = refcount::AddRef(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_822A67B0) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(refcount_global_lock)) {
    __imp__sub_822A67B0(ctx, base);
    return;
  }
  uint32_t object = ctx.r3.u32;
  if (object == g_unchanged_shader) {
    ctx.r3.u64 = guest::LoadU32(base, object + kCount);  // unused by the caller
    return;
  }
  uint32_t count = AddToCount(base, object + kCount, -1);
  if (count != 0) {
    ctx.r3.u64 = count;
    return;
  }
  if (HoldsChild(base, object)) {
    ctx.r3.u64 = guest::LoadU32(base, object + kChild);
    ctx.lr = 0x822A6810;
    sub_822A67B0(ctx, base);
  }
  ctx.r3.u64 = object;
  ctx.lr = 0x822A6818;
  sub_822A6210(ctx, base);
  ctx.r3.u64 = 0;
}

extern "C" REX_FUNC(sub_822A8808) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(refcount_global_lock)) {
    __imp__sub_822A8808(ctx, base);
    return;
  }
  uint32_t shader = ctx.r4.u32;
  bool same = shader && shader == guest::LoadU32(base, ctx.r3.u32 + kDeviceVertexShader);
  g_set_calls++;
  g_set_same += same;
  if (same) g_unchanged_shader = shader;
  __imp__sub_822A8808(ctx, base);
  g_unchanged_shader = 0;

  if (g_set_calls % 65536 == 0) {
    auto now = std::chrono::steady_clock::now();
    if (now - g_last_log >= std::chrono::seconds(5)) {
      g_last_log = now;
      ITC_LOG_INFO("refcount: SetVertexShader: {} calls on this thread, {} binding the shader "
                   "already bound (AddRef and Release skipped)",
                   g_set_calls, g_set_same);
    }
  }
}
