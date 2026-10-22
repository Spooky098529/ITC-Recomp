// Logging for our code: lines formatted with std::format and written to the
// log file (logs/itc_<n>.log) with stdio, one write a line, flushed. A2 in
// docs/rexglue-removal.md. Until our own app shell (A4), the SDK names the
// file and its own lines are forwarded into ours (log_sdk.cpp).

#pragma once

#include <atomic>
#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace base {

enum class LogLevel : int { kDebug, kInfo, kWarn, kError, kOff };

namespace log_detail {
// Everything is kept until the file opens and the level is known.
inline std::atomic<int> min_level{int(LogLevel::kDebug)};
}  // namespace log_detail

inline bool LogEnabled(LogLevel level) {
  return int(level) >= log_detail::min_level.load(std::memory_order_relaxed);
}

// Whether info logging is on (off as released, log_level = "off"):
// diagnostics that only feed the log (the 5 s stats, frame costs,
// description strings) skip their work when it isn't.
inline bool InfoLogEnabled() { return LogEnabled(LogLevel::kInfo); }

// One line, as "[time] [level] [category] [t<thread>] text". Not filtered by
// level: callers check LogEnabled first.
void WriteLogLine(LogLevel level, std::string_view category, std::string_view text);

// Opens the log file (appending) and sets the level from log_level's
// spelling (trace and debug, info, warn, error, critical and off as the
// SDK's), then writes the lines kept until now that pass it. Nothing is
// opened with the level off.
void StartLogFile(const std::string& path, std::string_view level);

template <typename... Args>
void Log(LogLevel level, std::format_string<Args...> format, Args&&... args) {
  WriteLogLine(level, "itc", std::format(format, std::forward<Args>(args)...));
}

}  // namespace base

// The arguments are only evaluated when the level is on.
#define ITC_LOG_AT_(level, ...)                 \
  do {                                          \
    if (::base::LogEnabled(level))              \
      ::base::Log(level, __VA_ARGS__);          \
  } while (0)
#define ITC_LOG_DEBUG(...) ITC_LOG_AT_(::base::LogLevel::kDebug, __VA_ARGS__)
#define ITC_LOG_INFO(...) ITC_LOG_AT_(::base::LogLevel::kInfo, __VA_ARGS__)
#define ITC_LOG_WARN(...) ITC_LOG_AT_(::base::LogLevel::kWarn, __VA_ARGS__)
#define ITC_LOG_ERROR(...) ITC_LOG_AT_(::base::LogLevel::kError, __VA_ARGS__)

namespace base::log_detail {
// Set on the thread running --run_harnesses while it runs (they run on the
// main thread, between its passes), so ITC_NATIVE_RUNNING can tell a
// harness's call from the game's on any thread.
inline thread_local bool in_harness = false;
}  // namespace base::log_detail

namespace base {
// While alive, ITC_NATIVE_RUNNING on this thread counts as a harness's.
struct InHarnessScope {
  InHarnessScope() { log_detail::in_harness = true; }
  ~InHarnessScope() { log_detail::in_harness = false; }
};
}  // namespace base

// At the top of each native function's plain C++ (the B7 loop): logs
// "native running (game): <name>" the first time the game runs it and
// "native running (harness): <name>" the first time a harness does, so a
// log shows which native functions ran, and how. One relaxed load a call
// after that.
#define ITC_NATIVE_RUNNING(name)                                          \
  do {                                                                    \
    static std::atomic<bool> itc_native_ran_[2] = {false, false};         \
    const bool itc_harness_ = ::base::log_detail::in_harness;             \
    if (!itc_native_ran_[itc_harness_].load(std::memory_order_relaxed) && \
        !itc_native_ran_[itc_harness_].exchange(true,                     \
                                                std::memory_order_relaxed)) \
      ITC_LOG_INFO("native running ({}): {}",                             \
                   itc_harness_ ? "harness" : "game", name);              \
  } while (0)
