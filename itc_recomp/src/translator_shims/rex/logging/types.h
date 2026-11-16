// Shim for the SDK's rex/logging/types.h (see ../../README.md): the spdlog
// types the SDK's logging macros use, as a minimal logger writing into our
// log.

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include "base/log.h"

namespace spdlog {

namespace level {
enum level_enum : int { trace, debug, info, warn, err, critical, off };
}  // namespace level

struct source_loc {
  const char* filename = nullptr;
  int line = 0;
  const char* funcname = nullptr;
};

// One category's logger: its lines go into our log under its name.
class logger {
 public:
  explicit logger(std::string name) : name_(std::move(name)) {}

  static ::base::LogLevel Ours(level::level_enum level) {
    switch (level) {
      case level::trace:
      case level::debug: return ::base::LogLevel::kDebug;
      case level::info: return ::base::LogLevel::kInfo;
      case level::warn: return ::base::LogLevel::kWarn;
      case level::off: return ::base::LogLevel::kOff;
      default: return ::base::LogLevel::kError;
    }
  }

  bool should_log(level::level_enum level) const {
    return level != level::off && ::base::LogEnabled(Ours(level));
  }

  template <typename... Args>
  void log(source_loc, level::level_enum level, fmt::format_string<Args...> format,
           Args&&... args) {
    ::base::WriteLogLine(Ours(level), name_, fmt::format(format, std::forward<Args>(args)...));
  }

  template <typename T>
  void log(source_loc, level::level_enum level, const T& message) {
    ::base::WriteLogLine(Ours(level), name_, fmt::format("{}", message));
  }

  void flush() {}

 private:
  std::string name_;
};

}  // namespace spdlog

namespace rex {

struct LogCategoryId {
  uint16_t id;
  constexpr explicit LogCategoryId(uint16_t id) : id(id) {}
  constexpr bool operator==(const LogCategoryId&) const = default;
};

}  // namespace rex
