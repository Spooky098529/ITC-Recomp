// The SDK's side of our log (A4 in docs/rexglue-removal.md): its logging set
// up with no file of its own, its lines (its shader translator's, the only
// SDK code left that logs) forwarded into ours. Until E4's step 5.

#include "base/log_sdk.h"

#include <memory>
#include <string>

#include <rex/logging.h>
#include <rex/logging/sink.h>
#include <spdlog/sinks/base_sink.h>

#include "base/log.h"

namespace base {

namespace {

LogLevel FromSpdlog(spdlog::level::level_enum level) {
  switch (level) {
    case spdlog::level::trace:
    case spdlog::level::debug: return LogLevel::kDebug;
    case spdlog::level::info: return LogLevel::kInfo;
    case spdlog::level::warn: return LogLevel::kWarn;
    default: return LogLevel::kError;
  }
}

// The SDK's loggers already filter by --log_level; every line that gets
// here is written.
class ForwardSink : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    WriteLogLine(FromSpdlog(msg.level),
                 std::string_view(msg.logger_name.data(), msg.logger_name.size()),
                 std::string_view(msg.payload.data(), msg.payload.size()));
  }
  void flush_() override {}
};

}  // namespace

void ForwardSdkLog(std::string_view level) {
  rex::LogConfig config;
  config.default_level = spdlog::level::from_str(std::string(level));
  config.extra_sinks.push_back(std::make_shared<ForwardSink>());
  rex::InitLogging(config);
}

}  // namespace base
