#include "base/log.h"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <mutex>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace base {

namespace {

std::mutex mutex;
std::FILE* file = nullptr;
bool started = false;
// Lines from before StartLogFile, with their level.
std::vector<std::pair<LogLevel, std::string>> early;

uint32_t ThreadId() {
#ifdef _WIN32
  thread_local uint32_t id = GetCurrentThreadId();
#else
  thread_local uint32_t id = uint32_t(gettid());
#endif
  return id;
}

std::string_view LevelName(LogLevel level) {
  switch (level) {
    case LogLevel::kDebug: return "debug";
    case LogLevel::kInfo: return "info";
    case LogLevel::kWarn: return "warning";
    case LogLevel::kError: return "error";
    default: return "off";
  }
}

LogLevel ParseLevel(std::string_view name) {
  if (name == "trace" || name == "debug") return LogLevel::kDebug;
  if (name == "warn" || name == "warning") return LogLevel::kWarn;
  if (name == "error" || name == "err") return LogLevel::kError;
  // Nothing of ours logs at critical.
  if (name == "critical" || name == "off") return LogLevel::kOff;
  return LogLevel::kInfo;
}

}  // namespace

void WriteLogLine(LogLevel level, std::string_view category, std::string_view text) {
  auto now = std::chrono::system_clock::now();
  std::time_t seconds = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  std::tm local{};
#ifdef _WIN32
  localtime_s(&local, &seconds);
#else
  localtime_r(&seconds, &local);
#endif
  std::string line =
      std::format("[{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}] [{}] [{}] [t{}] {}\n",
                  local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour,
                  local.tm_min, local.tm_sec, ms.count(), LevelName(level), category,
                  ThreadId(), text);
  std::lock_guard lock(mutex);
  if (!started) {
    early.emplace_back(level, std::move(line));
  } else if (file) {
    std::fwrite(line.data(), 1, line.size(), file);
    std::fflush(file);
  }
}

void StartLogFile(const std::string& path, std::string_view level_name) {
  LogLevel level = ParseLevel(level_name);
  std::lock_guard lock(mutex);
  if (started) {
    return;
  }
  started = true;
  log_detail::min_level.store(int(level), std::memory_order_relaxed);
  if (level == LogLevel::kOff) {
    early.clear();
    return;
  }
  file = std::fopen(path.c_str(), "ab");
  if (!file) {
    std::fprintf(stderr, "Can't open the log file %s\n", path.c_str());
    early.clear();
    return;
  }
  for (const auto& [line_level, line] : early) {
    if (line_level >= level) {
      std::fwrite(line.data(), 1, line.size(), file);
    }
  }
  std::fflush(file);
  early.clear();
  early.shrink_to_fit();
}

}  // namespace base
