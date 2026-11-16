// E1 (docs/rexglue-removal.md, "E1: the plan"): kernel imports the game
// calls answered by our own code instead of the SDK's. Each is registered
// under the import's function, and lifted::Import, which every call to an
// import goes through, runs ours instead (with --native_kernel_sync).

#pragma once

#include "lifted/sdk.h"

namespace kernel {

using NativeImport = lifted::Result (*)(uint8_t* base, const lifted::Args& args);

struct NativeImportEntry {
  lifted::ImportFunction* function;
  NativeImport native;
};

// Registers `count` entries at static initialization; returns true.
bool RegisterNativeImports(const NativeImportEntry* entries, size_t count);

// Ours for `function`, or null (the SDK's then runs).
NativeImport FindNativeImport(lifted::ImportFunction* function);

}  // namespace kernel
