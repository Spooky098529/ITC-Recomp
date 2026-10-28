// The native heap's adapters (hooks/heap.cpp).

#pragma once

#include <cstdint>
#include <optional>

namespace hooks {

// VirtualFree (sub_82295330) as its adapter does it, for native callers:
// its r3, or nullopt where the adapter runs the original (the native heap
// off on this thread, an address the layer didn't give). `caller` is only
// logged.
std::optional<uint32_t> VirtualFree(uint32_t address, uint32_t size, uint32_t type,
                                    uint32_t caller);

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
