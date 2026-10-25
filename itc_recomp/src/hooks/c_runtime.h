// The C runtime's adapters (hooks/c_runtime.cpp).

#pragma once

namespace hooks {

// Whether lock 8 (the exit lock) is the host mutex for this run
// (c_runtime::ExitMutex): --native_c_runtime at the first _lock call, fixed
// after, so a flag switched mid-run can't split its users.
bool CRuntimeExitHostLock();

// Whether atexit runs natively now: --native_c_runtime, the host exit lock,
// and the C runtime's heap over the allocation layer (--native_c_runtime_heap
// and hooks::NativeHeapActive), as _onexit calls its _msize and realloc.
bool NativeAtexit();

}  // namespace hooks
