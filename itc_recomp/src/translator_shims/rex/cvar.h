// Shim for the SDK's rex/cvar.h (see ../README.md): the translator's flags
// as plain values at their defaults, with the SDK's spellings (no registry,
// no parser: our flags are base/flags.h's).

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <string>
#include <string_view>

namespace rex::cvar {

enum class Lifecycle { kInitOnly, kHotReload, kRequiresRestart };

// What a definition's chained modifiers (.allowed(...), .debug_only() and the
// like) are called on: they change nothing here.
struct FlagStub {
  FlagStub&& range(double, double) && { return std::move(*this); }
  FlagStub&& allowed(std::initializer_list<std::string>) && { return std::move(*this); }
  FlagStub&& lifecycle(Lifecycle) && { return std::move(*this); }
  FlagStub&& debug_only() && { return std::move(*this); }
  FlagStub&& validator(std::function<bool(std::string_view)>) && { return std::move(*this); }
};

}  // namespace rex::cvar

#define REXCVAR_DECLARE(type, name) type& FLAGS_##name##_storage_()
#define REXCVAR_GET(name) (FLAGS_##name##_storage_())
#define REXCVAR_SET(name, value) (FLAGS_##name##_storage_() = (value))

#define REXCVAR_SHIM_DEFINE_(type, name, default_val) \
  type& FLAGS_##name##_storage_() {                   \
    static type storage = (default_val);              \
    return storage;                                   \
  }                                                   \
  [[maybe_unused]] static auto _cvar_reg_##name = ::rex::cvar::FlagStub {}

#define REXCVAR_DEFINE_BOOL(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(bool, name, default_val)
#define REXCVAR_DEFINE_INT32(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(int32_t, name, default_val)
#define REXCVAR_DEFINE_INT64(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(int64_t, name, default_val)
#define REXCVAR_DEFINE_UINT32(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(uint32_t, name, default_val)
#define REXCVAR_DEFINE_UINT64(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(uint64_t, name, default_val)
#define REXCVAR_DEFINE_DOUBLE(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(double, name, default_val)
#define REXCVAR_DEFINE_STRING(name, default_val, category, desc) \
  REXCVAR_SHIM_DEFINE_(std::string, name, default_val)
