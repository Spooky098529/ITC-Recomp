// Command-line flags for our code (A4 in docs/rexglue-removal.md): our own
// registry and parser, in place of the SDK's. Flags are defined at file scope,
// outside any namespace, and a flag defined in another file is declared
// before use:
//
//   ITC_FLAG_BOOL(my_flag, false, "ITC", "What it does");   // in one .cpp
//   ITC_FLAG_DECLARE(bool, my_flag);                         // elsewhere
//   if (ITC_FLAG(my_flag)) ...
//
// Values come from the command line (`--name=value`, `--name value`, and for
// a boolean `--name` or `--no-name`; the last one given wins), then from
// itc.toml next to the executable (a flat list of `name = value`) for flags
// the command line didn't set, else the default.

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace base::flags {

enum class Type { kBool, kInt32, kString };

// Adds a flag to the registry (at static initialization); returns true.
bool Register(const char* name, Type type, void* value, const char* description);

// Reads the command line: sets the flags it names and returns what wasn't a
// flag. Unknown flags and bad values are kept for WarnAboutProblems.
std::vector<std::string> ParseCommandLine(int argc, char** argv);

// Reads itc.toml (when it exists), for the flags the command line didn't set.
void LoadConfig(const std::filesystem::path& path);

// Logs what the command line and itc.toml held that wasn't a flag or a
// value (once the log is open).
void WarnAboutProblems();

}  // namespace base::flags

#define ITC_FLAG_DEFINE_(cpp_type, flag_type, name, default_val, description)              \
  cpp_type& itc_flag_##name() {                                                            \
    static cpp_type value = default_val;                                                   \
    return value;                                                                          \
  }                                                                                        \
  [[maybe_unused]] static const bool itc_flag_registered_##name =                          \
      ::base::flags::Register(#name, flag_type, &itc_flag_##name(), description);

#define ITC_FLAG_BOOL(name, default_val, category, desc) \
  ITC_FLAG_DEFINE_(bool, ::base::flags::Type::kBool, name, default_val, desc)
#define ITC_FLAG_INT32(name, default_val, category, desc) \
  ITC_FLAG_DEFINE_(int32_t, ::base::flags::Type::kInt32, name, default_val, desc)
#define ITC_FLAG_STRING(name, default_val, category, desc) \
  ITC_FLAG_DEFINE_(std::string, ::base::flags::Type::kString, name, default_val, desc)
#define ITC_FLAG_DECLARE(type, name) type& itc_flag_##name()

// A flag's value.
#define ITC_FLAG(name) (itc_flag_##name())

namespace base {

// --log_flags: every flag's value, one line each, sorted by name.
void LogFlags();

}  // namespace base
