// The game's allocators routed to the native allocation layer
// (src/memory/heap.h): the process heap's HeapAlloc, HeapFree, HeapReAlloc
// and HeapSize, VirtualAlloc and VirtualFree, and XPhysicalAlloc,
// XPhysicalAllocEx and XPhysicalFree. Below them, every allocation that runs
// in play (new, malloc, calloc, realloc, LocalAlloc, XMemAlloc, the shader
// compiler, the game's ~150 VirtualAlloc callers, physical memory). See
// docs/native-memory.md.

#include "hooks/heap.h"

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "memory/heap.h"

#include <atomic>
#include <cstdint>

ITC_FLAG_BOOL(native_heap, true, "ITC",
              "Route the game's allocations to the native allocation layer: the process "
              "heap's HeapAlloc, HeapFree, HeapReAlloc and HeapSize (sub_82299D60, "
              "sub_8229A648, sub_8229A930, sub_82298FF8), VirtualAlloc and VirtualFree "
              "(sub_82295398, sub_82295330), XPhysicalAlloc, XPhysicalAllocEx and "
              "XPhysicalFree (sub_822964C8, sub_82296550, sub_822965C8).");

namespace hooks {

namespace {

// The process heap's handle (GetProcessHeap, sub_8229B670, reads it).
constexpr uint32_t kProcessHeapHandle = 0x8297149C;
// Read by XPhysicalAlloc and XPhysicalAllocEx: when not 0, they clear
// X_MEM_LARGE_PAGES from the protection (not read further).
constexpr uint32_t kNoLargePhysicalPages = 0x829537B0;

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
  if (heap == guest::LoadU32(base, kProcessHeapHandle)) {
    return true;
  }
  LogOnce(0, heap, "a heap other than the process heap's", caller);
  return false;
}

uint32_t PhysicalProtection(uint8_t* base, uint32_t protection) {
  if (guest::LoadU32(base, kNoLargePhysicalPages) != 0) {
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
extern "C" REX_FUNC(sub_82299D60) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    __imp__sub_82299D60(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::Alloc(ctx.r5.u32, ctx.r4.u32);
}

// HeapFree: r3 = heap, r4 = flags, r5 = block; returns whether it was freed.
extern "C" REX_FUNC(sub_8229A648) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    __imp__sub_8229A648(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::Free(ctx.r5.u32) ? 1 : 0;
}

// HeapReAlloc: r3 = heap, r4 = flags, r5 = block, r6 = size; returns the
// block.
extern "C" REX_FUNC(sub_8229A930) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    __imp__sub_8229A930(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::Realloc(ctx.r5.u32, ctx.r6.u32, ctx.r4.u32);
}

// HeapSize: r3 = heap, r4 = flags (unread), r5 = block; returns its size.
extern "C" REX_FUNC(sub_82298FF8) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native() || !hooks::IsProcessHeap(base, ctx.r3.u32, uint32_t(ctx.lr))) {
    __imp__sub_82298FF8(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::Size(ctx.r5.u32);
}

// VirtualAlloc: r3 = address, r4 = size, r5 = type, r6 = protection;
// returns the address.
extern "C" REX_FUNC(sub_82295398) {
  REX_FUNC_PROLOGUE();
  const uint32_t address = ctx.r3.u32, size = ctx.r4.u32, type = ctx.r5.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  if (!hooks::Native()) {
    __imp__sub_82295398(ctx, base);
    return;
  }
  if (address != 0 || !(type & hooks::kMemCommit) || (type & ~hooks::kTypesTaken)) {
    hooks::LogOnce(1, type, address != 0 ? "VirtualAlloc at an address, type" : "VirtualAlloc type",
                   caller);
    __imp__sub_82295398(ctx, base);
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

extern "C" REX_FUNC(sub_82295330) {
  REX_FUNC_PROLOGUE();
  auto r3 = hooks::VirtualFree(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, uint32_t(ctx.lr));
  if (!r3) {
    __imp__sub_82295330(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// XPhysicalAlloc: r3 = size, r4 = physical address (-1 for any), r5 =
// alignment, r6 = protection; returns the address. A physical address asks
// for exactly [address, address + size) with no alignment.
extern "C" REX_FUNC(sub_822964C8) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native()) {
    __imp__sub_822964C8(ctx, base);
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
extern "C" REX_FUNC(sub_82296550) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native()) {
    __imp__sub_82296550(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::AllocPhysical(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                     hooks::PhysicalProtection(base, ctx.r7.u32));
}

// XPhysicalFree: r3 = the address.
extern "C" REX_FUNC(sub_822965C8) {
  REX_FUNC_PROLOGUE();
  if (!hooks::Native()) {
    __imp__sub_822965C8(ctx, base);
    return;
  }
  memory::FreePhysical(ctx.r3.u32);
}
