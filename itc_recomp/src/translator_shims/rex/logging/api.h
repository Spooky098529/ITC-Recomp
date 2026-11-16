// Shim for the SDK's rex/logging/api.h (see ../../README.md): categories by
// name, each a logger writing into our log.

#pragma once

#include <rex/cvar.h>
#include <rex/logging/types.h>

#include <deque>
#include <mutex>
#include <string>

REXCVAR_DECLARE(bool, log_noisy);

namespace rex {

namespace log_shim {
inline std::mutex& Mutex() {
  static std::mutex mutex;
  return mutex;
}
inline std::deque<spdlog::logger>& Loggers() {
  static std::deque<spdlog::logger> loggers;
  return loggers;
}
}  // namespace log_shim

inline LogCategoryId RegisterLogCategory(const char* name) {
  std::lock_guard lock(log_shim::Mutex());
  auto& loggers = log_shim::Loggers();
  loggers.emplace_back(name);
  return LogCategoryId(uint16_t(loggers.size() - 1));
}

inline LogCategoryId RegisterLogSubcategory(const char* name, LogCategoryId) {
  return RegisterLogCategory(name);
}

inline spdlog::logger* GetLoggerRaw(LogCategoryId category) {
  std::lock_guard lock(log_shim::Mutex());
  auto& loggers = log_shim::Loggers();
  return category.id < loggers.size() ? &loggers[category.id] : nullptr;
}

inline spdlog::logger* GetLogger(LogCategoryId category) { return GetLoggerRaw(category); }

// The default logger (only flushed before a fatal stop: ours flushes every
// line, so none).
inline spdlog::logger* GetLogger() { return nullptr; }

}  // namespace rex
