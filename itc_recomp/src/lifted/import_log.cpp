// --log_imports: which SDK imports the game calls, from where and on which
// thread (E1's first step, "E1: the plan" in docs/rexglue-removal.md). Every
// import goes through lifted::Import, which calls NoteImport while the flag
// is on. Logged: each new (import, caller's return address, thread) once,
// and every 30 s each import's call count so far. Threads are told apart by
// r13 (the kernel's per-thread block).

#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>
#include <set>
#include <tuple>
#include <vector>

ITC_FLAG_BOOL(log_imports, false, "ITC",
              "Log the SDK imports the game calls: each new import, caller (return "
              "address) and thread (r13) once, and every 30 s each import's call count "
              "(src/lifted/import_log.cpp; E1 in docs/rexglue-removal.md)");

namespace lifted {

namespace {

struct ImportLog {
  std::mutex mutex;
  std::map<ImportFunction*, uint64_t> counts;
  std::set<std::tuple<ImportFunction*, uint32_t, uint32_t>> seen;
  std::chrono::steady_clock::time_point next_summary =
      std::chrono::steady_clock::now() + std::chrono::seconds(30);
};

ImportLog& Log() {
  static ImportLog log;
  return log;
}

void Summary(ImportLog& log) {
  std::vector<std::pair<uint64_t, const char*>> rows;
  for (const auto& [function, count] : log.counts) {
    rows.emplace_back(count, ImportName(function));
  }
  std::sort(rows.rbegin(), rows.rend());
  ITC_LOG_INFO("imports: {} called so far", rows.size());
  for (const auto& [count, name] : rows) {
    ITC_LOG_INFO("imports:   {} {}", name, count);
  }
}

}  // namespace

bool LoggingImports() {
  return ITC_FLAG(log_imports);
}

void NoteImport(ImportFunction* function, const Args& args) {
  ImportLog& log = Log();
  std::lock_guard lock(log.mutex);
  log.counts[function]++;
  const uint32_t thread = uint32_t(args.r13);
  if (log.seen.emplace(function, args.lr, thread).second) {
    ITC_LOG_INFO("imports: {} from {:08X} on thread {:08X}", ImportName(function), args.lr,
                 thread);
  }
  const auto now = std::chrono::steady_clock::now();
  if (now >= log.next_summary) {
    log.next_summary = now + std::chrono::seconds(30);
    Summary(log);
  }
}

}  // namespace lifted
