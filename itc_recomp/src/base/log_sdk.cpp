// The SDK's side of our log, until our own app shell (A4 in
// docs/rexglue-removal.md): the SDK picks the file's name (logs/itc_<n>.log,
// one more than the highest there) and opens it in its logging set-up; we
// take the file over from its sink and forward its own lines (the runtime's,
// the kernel's) into ours. At A4 our main names the file and sets up the
// SDK's logging with no file of its own.

#include "base/log_sdk.h"

#include <memory>
#include <string>

#include <rex/logging.h>
#include <rex/logging/sink.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "base/flags.h"
#include "base/log.h"

ITC_FLAG_DECLARE(std::string, log_level);

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

void TakeOverSdkLogFile() {
  std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> sdk_file;
  if (auto core = rex::GetLogger(rex::log::core())) {
    for (const auto& sink : core->sinks()) {
      if (auto file = std::dynamic_pointer_cast<spdlog::sinks::rotating_file_sink_mt>(sink)) {
        sdk_file = file;
      }
    }
  }
  if (!sdk_file) {
    // Without the SDK's file there's no name to take: our lines go nowhere.
    StartLogFile("", "off");
    return;
  }
  // Removed from every logger, and off: a category the SDK registers later
  // (on its first use; "sys" for the guest memory's lines) still gets the
  // SDK's file sink among its sinks. The SDK keeps the file open, unused.
  sdk_file->set_level(spdlog::level::off);
  rex::RemoveSink(sdk_file);
  rex::AddSink(std::make_shared<ForwardSink>());
  StartLogFile(sdk_file->filename(), ITC_FLAG(log_level));
}

}  // namespace base
