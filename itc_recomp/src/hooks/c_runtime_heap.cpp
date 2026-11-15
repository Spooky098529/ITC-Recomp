// Adapters in front of the C runtime's heap over the native allocation layer
// (src/memory/c_runtime_heap.h; docs/functions/heap.md). _calloc_impl,
// LocalAlloc and LocalFree have none: only calloc, XMemAlloc and XMemFree
// call them, and those call the plain C++. Each adapter falls back
// to the game's own function with --native_c_runtime_heap off, with
// --native_heap off (the game's own heap; the plain C++ calls the layer
// directly), and for the error paths the plain C++ leaves to it.

#include "guest.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"

ITC_FLAG_BOOL(native_c_runtime_heap, true, "ITC",
              "Run the C runtime's heap natively over the allocation layer (needs "
              "--native_heap): GetProcessHeap (sub_8229B670), malloc (sub_825A9A90), free "
              "(sub_825A9B58), _calloc_impl and calloc (sub_825AC248, sub_825AC320), realloc "
              "(sub_825AEBC8), _msize (sub_825AED30), LocalAlloc and LocalFree (sub_82297F48, "
              "sub_82297F90), XMemAlloc and XMemFree (sub_822976F8, sub_82297790), operator new "
              "and new[] (sub_825A6880, sub_825A6940), operator delete and delete[] "
              "(sub_825A70E8, sub_825A71C0); their error paths stay the game's (checked by "
              "--run_harnesses=c_runtime_heap). Those reached only from native callers have "
              "no adapters, and the flag switches their callers: _calloc_impl, LocalAlloc and "
              "LocalFree");

namespace {

bool Native() {
  return ITC_FLAG(native_c_runtime_heap) && hooks::NativeHeapActive();
}

}  // namespace

// GetProcessHeap: returns the process heap's handle.
ITC_NATIVE_ENTRY(8229B670, ITC_FLAG(native_c_runtime_heap)) {
  ctx.r3.u64 = memory::crt::GetProcessHeap(base);
}

// malloc: r3 = size; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(825A9A90) {
  
  if (Native()) {
    if (auto block = memory::crt::Malloc(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::Sdk(ctx, base, __imp__sub_825A9A90);
}

// free: r3 = block.
ITC_NATIVE_ENTRY_ALWAYS(825A9B58) {
  
  if (!Native() || !memory::crt::Free(ctx.r3.u32)) {
    lifted::Sdk(ctx, base, __imp__sub_825A9B58);
  }
}

// calloc: r3 = count, r4 = size; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(825AC320) {
  
  if (Native()) {
    if (auto block = memory::crt::Calloc(ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::Sdk(ctx, base, __imp__sub_825AC320);
}

// realloc: r3 = block, r4 = size; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(825AEBC8) {
  
  if (Native()) {
    if (auto block = memory::crt::Realloc(base, ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::Sdk(ctx, base, __imp__sub_825AEBC8);
}

// _msize: r3 = block; returns its size.
ITC_NATIVE_ENTRY_ALWAYS(825AED30) {
  
  if (Native()) {
    if (auto size = memory::crt::Msize(ctx.r3.u32)) {
      ctx.r3.u64 = *size;
      return;
    }
  }
  lifted::Sdk(ctx, base, __imp__sub_825AED30);
}

// XMemAlloc: r3 = size, r4 = attributes; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(822976F8) {
  
  if (!Native()) {
    lifted::Sdk(ctx, base, __imp__sub_822976F8);
    return;
  }
  ctx.r3.u64 = memory::crt::XMemAlloc(base, ctx.r3.u32, ctx.r4.u32);
}

// XMemFree: r3 = block, r4 = attributes.
ITC_NATIVE_ENTRY_ALWAYS(82297790) {
  
  if (!Native()) {
    lifted::Sdk(ctx, base, __imp__sub_82297790);
    return;
  }
  ctx.r3.u64 = memory::crt::XMemFree(ctx.r3.u32, ctx.r4.u32);
}

// operator new: r3 = size; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(825A6880) {
  
  if (hooks::StartUpRecorded(ctx, 0x825A6880)) {
    return;  // a harness's (the race HUD object's)
  }
  if (Native()) {
    if (auto block = memory::crt::OperatorNew(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::Sdk(ctx, base, __imp__sub_825A6880);
}

// operator new[]: r3 = size; returns the block.
ITC_NATIVE_ENTRY_ALWAYS(825A6940) {
  
  if (Native()) {
    if (auto block = memory::crt::OperatorNewArray(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::Sdk(ctx, base, __imp__sub_825A6940);
}

// operator delete: r3 = block.
ITC_NATIVE_ENTRY_ALWAYS(825A70E8) {
  
  if (!Native() || !memory::crt::OperatorDelete(ctx.r3.u32)) {
    lifted::original::c825A70E8(ctx, base);
  }
}

// operator delete[]: r3 = block.
ITC_NATIVE_ENTRY_ALWAYS(825A71C0) {
  
  if (!Native() || !memory::crt::OperatorDeleteArray(ctx.r3.u32)) {
    lifted::Sdk(ctx, base, __imp__sub_825A71C0);
  }
}
