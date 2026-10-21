// Logging for our code. Forwards to the SDK's logger for now, which owns the
// log file (logs/itc_<n>.log) until our own replaces it: A2 in
// docs/rexglue-removal.md.

#pragma once

#include <rex/logging.h>

#define ITC_LOG_DEBUG(...) REXLOG_DEBUG(__VA_ARGS__)
#define ITC_LOG_INFO(...) REXLOG_INFO(__VA_ARGS__)
#define ITC_LOG_WARN(...) REXLOG_WARN(__VA_ARGS__)
#define ITC_LOG_ERROR(...) REXLOG_ERROR(__VA_ARGS__)

namespace base {

// Whether info logging is on (off in run.sh, --log_level=off): diagnostics
// that only feed the log (the 5 s stats, frame costs, description strings)
// skip their work when it isn't. Not for per-draw paths: the logger lookup
// is a call into the runtime.
inline bool InfoLogEnabled() {
  spdlog::logger* logger = rex::GetLoggerRaw(rex::log::core());
  return logger && logger->should_log(spdlog::level::info);
}

}  // namespace base
