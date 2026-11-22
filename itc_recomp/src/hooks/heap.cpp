// The game's allocators routed to the native allocation layer
// (src/memory/heap.h): the process heap's HeapAlloc, HeapFree, HeapReAlloc
// and HeapSize, VirtualAlloc and VirtualFree, and XPhysicalAlloc,
// XPhysicalAllocEx and XPhysicalFree (XPhysicalSize, sub_822965C0, has no
// adapter: its callers are native and call memory::PhysicalSize, the user's
// call of 2026-11-06). Below them, every allocation that runs
// in play (new, malloc, calloc, realloc, LocalAlloc, XMemAlloc, the shader
// compiler, the game's ~150 VirtualAlloc callers, physical memory). See
// docs/native-memory.md.

#include "hooks/heap.h"
#include "hooks/last_error.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "memory/heap.h"
#include "schema/heap.h"

#include <atomic>
#include <cstdint>

ITC_FLAG_BOOL(native_heap, true, "ITC",
              "Route the game's allocations to the native allocation layer: the process "
              "heap's HeapAlloc, HeapFree, HeapReAlloc and HeapSize (sub_82299D60, "
              "sub_8229A648, sub_8229A930, sub_82298FF8), VirtualAlloc and VirtualFree "
              "(sub_82295398, sub_82295330), XPhysicalAlloc, XPhysicalAllocEx and "
              "XPhysicalFree (sub_822964C8, sub_82296550, sub_822965C8). XPhysicalSize "
              "(sub_822965C0) is answered from the layer for its native callers "
              "(memory::PhysicalSize; no adapter).");

namespace hooks {

namespace {

constexpr uint32_t kMemCommit = 0x1000;
constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kMemLargePages = 0x20000000;
// VirtualAlloc's types the layer takes: MEM_COMMIT with MEM_RESERVE,
// MEM_TOP_DOWN, MEM_NOZERO (cleared anyway) or MEM_LARGE_PAGES.
constexpr uint32_t kTypesTaken = kMemCommit | 0x2000 | 0x100000 | 0x800000 | kMemLargePages;

thread_local bool use_recompiled = false;

bool Native() {
  return ITC_FLAG(native_heap) && !use_recompiled;
}

// Logs the first value of each kind (0-31) seen.
void LogOnce(uint32_t kind, uint32_t value, const char* what, uint32_t caller) {
  static std::atomic<uint32_t> logged_kinds{0};
  if (!(logged_kinds.fetch_or(1u << kind) & (1u << kind))) {
    ITC_LOG_WARN("native heap: {} {:#x} from {:#x}, left to the game's own function", what, value,
                 caller);
  }
}

bool IsProcessHeap(uint8_t* base, uint32_t heap, uint32_t caller) {
  if (heap == schema::HeapGlobals::process_heap(base).address) {
    return true;
  }
  LogOnce(0, heap, "a heap other than the process heap's", caller);
  return false;
}

uint32_t PhysicalProtection(uint8_t* base, uint32_t protection) {
  if (schema::HeapGlobals::no_large_physical_pages(base) != 0) {
    protection &= ~kMemLargePages;
  }
  return protection;
}

}  // namespace

bool NativeHeapActive() {
  return Native();
}

RecompiledHeapScope::RecompiledHeapScope() {
  use_recompiled = true;
}

RecompiledHeapScope::~RecompiledHeapScope() {
  use_recompiled = false;
}

}  // namespace hooks

// HeapAlloc: r3 = heap, r4 = flags, r5 = size; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(82299D60) {
  
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    lifted::NoOriginal(0x82299D60);
    return;
  }
  ctx.r3.u64 = memory::Alloc(ctx.r5.u32, ctx.r4.u32);
}

// HeapFree: r3 = heap, r4 = flags, r5 = block; returns whether it was freed.
ITC_NATIVE_ENTRY_ALWAYS(8229A648) {
  
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    lifted::NoOriginal(0x8229A648);
    return;
  }
  ctx.r3.u64 = memory::Free(ctx.r5.u32) ? 1 : 0;
}

// HeapReAlloc: r3 = heap, r4 = flags, r5 = block, r6 = size; returns the
// block.
ITC_NATIVE_ENTRY_ALWAYS(8229A930) {
  
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    lifted::NoOriginal(0x8229A930);
    return;
  }
  ctx.r3.u64 = memory::Realloc(ctx.r5.u32, ctx.r6.u32, ctx.r4.u32);
}

// HeapSize: r3 = heap, r4 = flags (unread), r5 = block; returns its size.
ITC_NATIVE_ENTRY_ALWAYS(82298FF8) {
  
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    lifted::original::c82298FF8(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::Size(ctx.r5.u32);
}

// VirtualAlloc: r3 = address, r4 = size, r5 = type, r6 = protection;
// returns the address.
ITC_NATIVE_ENTRY_ALWAYS(82295398) {
  
  const uint32_t address = ctx.r3.u32, size = ctx.r4.u32, type = ctx.r5.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  if (!hooks::Native()) {
    lifted::original::c82295398(ctx, base);
    return;
  }
  if (address != 0 || !(type & hooks::kMemCommit) || (type & ~hooks::kTypesTaken)) {
    hooks::LogOnce(1, type, address != 0 ? "VirtualAlloc at an address, type" : "VirtualAlloc type",
                   caller);
    lifted::original::c82295398(ctx, base);
    return;
  }
  if (ctx.r6.u32 != 4) {
    hooks::LogOnce(2, ctx.r6.u32, "VirtualAlloc protection (taken as read-write)", caller);
  }
  ctx.r3.u64 = memory::AllocPages(size, (type & hooks::kMemLargePages) ? 64 * 1024 : 4096);
}

// VirtualFree: r3 = address, r4 = size, r5 = type; returns whether it was
// freed.
std::optional<uint32_t> hooks::VirtualFree(uint32_t address, uint32_t size, uint32_t type,
                                           uint32_t caller) {
  if (!hooks::Native() || !memory::InRange(address)) {
    return std::nullopt;
  }
  if (size != 0 || type != hooks::kMemRelease) {
    // The layer's pages stay as they are (a decommit: the archive's request
    // finish, sub_8209D688, frees its data with MEM_DECOMMIT).
    hooks::LogOnce(3, type, "VirtualFree type (nothing done)", caller);
    return 1;
  }
  return memory::FreePages(address) ? 1 : 0;
}

ITC_NATIVE_ENTRY_ALWAYS(82295330) {
  // VirtualFree(0, ...), plugged in (E2's first step, docs/rexglue-removal.md):
  // the only frees in play outside the layer (the pad replay: 34, from
  // 0x820E85BC and 0x820F864C). The original asks the kernel, which answers
  // STATUS_MEMORY_NOT_ALLOCATED, sets the last error from it
  // (ERROR_INVALID_ADDRESS) and returns 0; a release with a size fails
  // before the kernel (the original's first branch, kept).
  if (ctx.r3.u32 == 0 && !((ctx.r5.u32 & hooks::kMemRelease) && ctx.r4.u32 != 0)) {
    constexpr uint32_t kErrorInvalidAddress = 487;
    last_error::Set(base, ctx.r13.u32, kErrorInvalidAddress);
    ctx.r3.u64 = 0;
    return;
  }
  auto r3 = hooks::VirtualFree(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, uint32_t(ctx.lr));
  if (!r3) {
    lifted::original::c82295330(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// XPhysicalAlloc: r3 = size, r4 = physical address (-1 for any), r5 =
// alignment, r6 = protection; returns the address. A physical address asks
// for exactly [address, address + size) with no alignment.
ITC_NATIVE_ENTRY_ALWAYS(822964C8) {
  
  if (!hooks::Native()) {
    lifted::original::c822964C8(ctx, base);
    return;
  }
  const uint32_t size = ctx.r3.u32, physical = ctx.r4.u32;
  const uint32_t protection = hooks::PhysicalProtection(base, ctx.r6.u32);
  ctx.r3.u64 = physical == 0xFFFFFFFF
                   ? memory::AllocPhysical(size, 0, 0xFFFFFFFF, ctx.r5.u32, protection)
                   : memory::AllocPhysical(size, physical, size + physical - 1, 0, protection);
}

// XPhysicalAllocEx: r3 = size, r4 = lowest and r5 = highest physical
// address, r6 = alignment, r7 = protection; returns the address.
ITC_NATIVE_ENTRY_ALWAYS(82296550) {
  
  if (!hooks::Native()) {
    lifted::original::c82296550(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::AllocPhysical(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                     hooks::PhysicalProtection(base, ctx.r7.u32));
}

// XPhysicalFree: r3 = the address.
ITC_NATIVE_ENTRY_ALWAYS(822965C8) {
  
  if (!hooks::Native()) {
    lifted::original::c822965C8(ctx, base);
    return;
  }
  memory::FreePhysical(ctx.r3.u32);
}
