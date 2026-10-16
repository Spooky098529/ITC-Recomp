// Whether info logging is on (off in run.sh, --log_level=off): diagnostics
// that only feed the log (the 5 s stats, frame costs, description strings)
// skip their work when it isn't. Not for per-draw paths: the logger lookup
// is a call into the runtime.

#pragma once

#include <rex/logging.h>

namespace native {

inline bool InfoLogEnabled() {
  spdlog::logger* logger = rex::GetLoggerRaw(rex::log::core());
  return logger && logger->should_log(spdlog::level::info);
}

}  // namespace native
