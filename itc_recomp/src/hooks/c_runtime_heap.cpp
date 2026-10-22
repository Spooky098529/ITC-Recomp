// Adapters in front of the C runtime's heap over the native allocation layer
// (src/memory/c_runtime_heap.h; docs/functions/heap.md). Each falls back to
// the game's own function with --native_c_runtime_heap off, with
// --native_heap off (the game's own heap; the plain C++ calls the layer
// directly), and for the error paths the plain C++ leaves to it.

#include "guest.h"

#include "base/flags.h"
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
              "--run_harnesses=c_runtime_heap)");

namespace {

bool Native() {
  return ITC_FLAG(native_c_runtime_heap) && hooks::NativeHeapActive();
}

}  // namespace

// GetProcessHeap: returns the process heap's handle.
extern "C" REX_FUNC(sub_8229B670) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime_heap)) {
    __imp__sub_8229B670(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::crt::GetProcessHeap(base);
}

// malloc: r3 = size; returns the block.
extern "C" REX_FUNC(sub_825A9A90) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto block = memory::crt::Malloc(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_825A9A90(ctx, base);
}

// free: r3 = block.
extern "C" REX_FUNC(sub_825A9B58) {
  REX_FUNC_PROLOGUE();
  if (!Native() || !memory::crt::Free(ctx.r3.u32)) {
    __imp__sub_825A9B58(ctx, base);
  }
}

// _calloc_impl: r3 = count, r4 = size, r5 = where errno goes; returns the
// block.
extern "C" REX_FUNC(sub_825AC248) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto block = memory::crt::CallocImpl(ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_825AC248(ctx, base);
}

// calloc: r3 = count, r4 = size; returns the block.
extern "C" REX_FUNC(sub_825AC320) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto block = memory::crt::Calloc(ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_825AC320(ctx, base);
}

// realloc: r3 = block, r4 = size; returns the block.
extern "C" REX_FUNC(sub_825AEBC8) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto block = memory::crt::Realloc(base, ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_825AEBC8(ctx, base);
}

// _msize: r3 = block; returns its size.
extern "C" REX_FUNC(sub_825AED30) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto size = memory::crt::Msize(ctx.r3.u32)) {
      ctx.r3.u64 = *size;
      return;
    }
  }
  __imp__sub_825AED30(ctx, base);
}

// LocalAlloc: r3 = flags, r4 = size; returns the block.
extern "C" REX_FUNC(sub_82297F48) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_82297F48(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::crt::LocalAlloc(ctx.r3.u32, ctx.r4.u32);
}

// LocalFree: r3 = block; returns 0 when freed, else the block.
extern "C" REX_FUNC(sub_82297F90) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_82297F90(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::crt::LocalFree(ctx.r3.u32);
}

// XMemAlloc: r3 = size, r4 = attributes; returns the block.
extern "C" REX_FUNC(sub_822976F8) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_822976F8(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::crt::XMemAlloc(base, ctx.r3.u32, ctx.r4.u32);
}

// XMemFree: r3 = block, r4 = attributes.
extern "C" REX_FUNC(sub_82297790) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_82297790(ctx, base);
    return;
  }
  ctx.r3.u64 = memory::crt::XMemFree(ctx.r3.u32, ctx.r4.u32);
}

// operator new: r3 = size; returns the block.
extern "C" REX_FUNC(sub_825A6880) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto block = memory::crt::OperatorNew(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_825A6880(ctx, base);
}

// operator new[]: r3 = size; returns the block.
extern "C" REX_FUNC(sub_825A6940) {
  REX_FUNC_PROLOGUE();
  if (Native()) {
    if (auto block = memory::crt::OperatorNewArray(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_825A6940(ctx, base);
}

// operator delete: r3 = block.
extern "C" REX_FUNC(sub_825A70E8) {
  REX_FUNC_PROLOGUE();
  if (!Native() || !memory::crt::OperatorDelete(ctx.r3.u32)) {
    __imp__sub_825A70E8(ctx, base);
  }
}

// operator delete[]: r3 = block.
extern "C" REX_FUNC(sub_825A71C0) {
  REX_FUNC_PROLOGUE();
  if (!Native() || !memory::crt::OperatorDeleteArray(ctx.r3.u32)) {
    __imp__sub_825A71C0(ctx, base);
  }
}
