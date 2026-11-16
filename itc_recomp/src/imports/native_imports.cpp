// See native_imports.h.

#include "imports/native_imports.h"

#include "base/log.h"

#include <unordered_map>
#include <vector>

namespace imports {

namespace {

struct Registration {
  const NativeImportEntry* entries;
  size_t count;
  bool (*enabled)();
};

std::vector<Registration>& Registrations() {
  static std::vector<Registration> registrations;
  return registrations;
}

// The table of the groups on, made at the first lookup (after start-up has
// parsed the flags).
const std::unordered_map<lifted::ImportFunction*, NativeImport>& Table() {
  static const auto table = [] {
    std::unordered_map<lifted::ImportFunction*, NativeImport> made;
    for (const Registration& registration : Registrations()) {
      if (!registration.enabled()) {
        continue;
      }
      for (size_t i = 0; i < registration.count; i++) {
        const NativeImportEntry& entry = registration.entries[i];
        if (!made.emplace(entry.function, entry.native).second) {
          ITC_LOG_ERROR("imports: {} registered twice", lifted::ImportName(entry.function));
        }
      }
    }
    return made;
  }();
  return table;
}

}  // namespace

bool RegisterNativeImports(const NativeImportEntry* entries, size_t count, bool (*enabled)()) {
  Registrations().push_back({entries, count, enabled});
  return true;
}

NativeImport FindNativeImport(lifted::ImportFunction* function) {
  const auto& table = Table();
  const auto it = table.find(function);
  return it == table.end() ? nullptr : it->second;
}

}  // namespace imports
