// The C runtime's adapters (hooks/c_runtime.cpp).

#pragma once

#include "guest.h"

#include "c_runtime/exit.h"

namespace hooks {

// Whether lock 8 (the exit lock) is the host mutex for this run
// (c_runtime::ExitMutex): --native_c_runtime at the first _lock call, fixed
// after, so a flag switched mid-run can't split its users.
bool CRuntimeExitHostLock();

// Whether atexit runs natively now: --native_c_runtime, the host exit lock,
// and the C runtime's heap over the allocation layer (--native_c_runtime_heap
// and hooks::NativeHeapActive), as _onexit calls its _msize and realloc.
bool NativeAtexit();

// For adapters of native code that registers clean-ups: the game's atexit
// (__imp__sub_825A7068) through `ctx` when c_runtime::Atexit can't, the
// call made below ctx's stack pointer (the adapter's caller's frame). Lives
// as long as the adapter's call.
struct GuestAtexit {
  PPCContext& ctx;
  uint8_t* base;
  c_runtime::AtexitCall Call() { return {&GuestAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function);
};

}  // namespace hooks
