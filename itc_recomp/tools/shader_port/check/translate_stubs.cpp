#include <rex/cvar.h>
#include <rex/string/utf8.h>
#include <optional>
#include <string>
namespace rex::cvar {
std::optional<size_t> RegisterFlag(FlagEntry) { return std::nullopt; }
void UnregisterFlag(std::string_view) {}
void RegisterChangeCallback(std::string_view, ChangeCallback) {}
}
namespace rex::string {
std::string to_utf8(const std::u16string_view s) { return std::string(s.begin(), s.end()); }
std::u16string to_utf16(const std::string_view s) { return std::u16string(s.begin(), s.end()); }
}
#include <rex/platform/env.h>
namespace rex::platform::env {
std::optional<std::string> get(std::string_view) { return std::nullopt; }
}
namespace rex::cvar {
void FlagRegistrar::apply_(std::function<void(FlagEntry&)>) {}
}
