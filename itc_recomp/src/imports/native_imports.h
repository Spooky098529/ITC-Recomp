// SDK imports the game calls answered by our own code instead of the SDK's
// (E1 and E2, docs/rexglue-removal.md). Each group registers its imports
// with a function telling whether its flag is on (read once, at the first
// lookup: objects made by ours and the SDK's don't mix); lifted::Import,
// which every call to an import goes through, runs ours for those on.

#pragma once

#include "lifted/sdk.h"

namespace imports {

using NativeImport = lifted::Result (*)(const lifted::Args& args);

struct NativeImportEntry {
  lifted::ImportFunction* function;
  NativeImport native;
};

// Registers `count` entries at static initialization, used while
// `enabled()` answers true; returns true.
bool RegisterNativeImports(const NativeImportEntry* entries, size_t count, bool (*enabled)());

// Ours for `function`, or null (the SDK's then runs).
NativeImport FindNativeImport(lifted::ImportFunction* function);

}  // namespace imports
