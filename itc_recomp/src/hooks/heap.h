// The native heap's adapters (hooks/heap.cpp).

#pragma once

namespace hooks {

// Whether the adapters on this thread route to the native allocation layer
// (--native_heap, outside a RecompiledHeapScope): native code over the layer
// (src/memory/c_runtime_heap.h) runs only then, or the two heaps would mix.
bool NativeHeapActive();

// While alive, the adapters on this thread call the game's own functions
// (__imp__), as with --native_heap=false: for the harness's recompiled side,
// whose HeapReAlloc calls HeapAlloc and HeapFree itself.
struct RecompiledHeapScope {
  RecompiledHeapScope();
  ~RecompiledHeapScope();
};

}  // namespace hooks
