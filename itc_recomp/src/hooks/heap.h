// The native heap's adapters (hooks/heap.cpp).

#pragma once

namespace hooks {

// While alive, the adapters on this thread call the game's own functions
// (__imp__), as with --native_heap=false: for the harness's recompiled side,
// whose HeapReAlloc calls HeapAlloc and HeapFree itself.
struct RecompiledHeapScope {
  RecompiledHeapScope();
  ~RecompiledHeapScope();
};

}  // namespace hooks
