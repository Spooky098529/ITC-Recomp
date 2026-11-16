// See native_imports.h.

#include "threads/native_imports.h"

#include "base/flags.h"
#include "base/log.h"

#include <unordered_map>

ITC_FLAG_BOOL(native_threads, true, "ITC",
              "Answer the kernel's thread and sync imports with our own code (E1 in "
              "docs/rexglue-removal.md: src/threads/) instead of the SDK's; read at start-up");

namespace threads {

namespace {

std::unordered_map<lifted::ImportFunction*, NativeImport>& Table() {
  static std::unordered_map<lifted::ImportFunction*, NativeImport> table;
  return table;
}

}  // namespace

bool RegisterNativeImports(const NativeImportEntry* entries, size_t count) {
  auto& table = Table();
  for (size_t i = 0; i < count; i++) {
    if (!table.emplace(entries[i].function, entries[i].native).second) {
      ITC_LOG_ERROR("threads: {} registered twice", lifted::ImportName(entries[i].function));
    }
  }
  return true;
}

NativeImport FindNativeImport(lifted::ImportFunction* function) {
  // The flag is read once: objects made by ours and the SDK's don't mix.
  static const bool on = ITC_FLAG(native_threads);
  if (!on) {
    return nullptr;
  }
  const auto& table = Table();
  const auto it = table.find(function);
  return it == table.end() ? nullptr : it->second;
}

}  // namespace threads
