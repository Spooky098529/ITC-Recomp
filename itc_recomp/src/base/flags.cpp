#include "base/flags.h"

#include "base/log.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <format>
#include <fstream>
#include <map>
#include <string>
#include <vector>

ITC_FLAG_BOOL(log_flags, false, "ITC",
              "Log every flag's value at start-up (A4 in docs/rexglue-removal.md: compares "
              "the flag parsers)");

namespace base::flags {

namespace {

struct Flag {
  Type type;
  void* value;
  const char* description;
  bool from_command_line = false;
};

std::map<std::string, Flag, std::less<>>& Registry() {
  static std::map<std::string, Flag, std::less<>> registry;
  return registry;
}

// What the command line and itc.toml held that wasn't a flag or a value.
std::vector<std::string>& Problems() {
  static std::vector<std::string> problems;
  return problems;
}

std::string Lower(std::string_view text) {
  std::string out(text);
  for (char& c : out) {
    c = char(std::tolower(static_cast<unsigned char>(c)));
  }
  return out;
}

// Sets a flag from its text; false when the text isn't a value of its type.
bool SetFromText(Flag& flag, std::string_view text) {
  switch (flag.type) {
    case Type::kBool: {
      const std::string word = Lower(text);
      if (word == "true" || word == "1" || word == "yes" || word == "on") {
        *static_cast<bool*>(flag.value) = true;
        return true;
      }
      if (word == "false" || word == "0" || word == "no" || word == "off") {
        *static_cast<bool*>(flag.value) = false;
        return true;
      }
      return false;
    }
    case Type::kInt32: {
      int32_t number = 0;
      std::string_view digits = text;
      int radix = 10;
      bool negative = false;
      if (digits.starts_with('-')) {
        negative = true;
        digits.remove_prefix(1);
      } else if (digits.starts_with('+')) {
        digits.remove_prefix(1);
      }
      if (digits.starts_with("0x") || digits.starts_with("0X")) {
        radix = 16;
        digits.remove_prefix(2);
      }
      int64_t wide = 0;
      const auto [end, error] =
          std::from_chars(digits.data(), digits.data() + digits.size(), wide, radix);
      if (digits.empty() || error != std::errc() || end != digits.data() + digits.size()) {
        return false;
      }
      wide = negative ? -wide : wide;
      if (wide < INT32_MIN || wide > INT32_MAX) {
        return false;
      }
      number = int32_t(wide);
      *static_cast<int32_t*>(flag.value) = number;
      return true;
    }
    case Type::kString:
      *static_cast<std::string*>(flag.value) = std::string(text);
      return true;
  }
  return false;
}

std::string ValueText(const Flag& flag) {
  switch (flag.type) {
    case Type::kBool: return *static_cast<const bool*>(flag.value) ? "true" : "false";
    case Type::kInt32: return std::to_string(*static_cast<const int32_t*>(flag.value));
    case Type::kString: return *static_cast<const std::string*>(flag.value);
  }
  return {};
}

// A TOML value of the subset itc.toml uses: a basic or literal string, or a
// bare word (a boolean or a number), with a comment after it dropped.
bool ParseTomlValue(std::string_view text, std::string& out) {
  while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
    text.remove_prefix(1);
  }
  if (text.starts_with('"')) {
    out.clear();
    for (size_t i = 1; i < text.size(); i++) {
      const char c = text[i];
      if (c == '"') {
        return true;
      }
      if (c == '\\' && i + 1 < text.size()) {
        const char e = text[++i];
        switch (e) {
          case 'n': out += '\n'; break;
          case 't': out += '\t'; break;
          case '\\': out += '\\'; break;
          case '"': out += '"'; break;
          default: out += '\\'; out += e; break;
        }
        continue;
      }
      out += c;
    }
    return false;
  }
  if (text.starts_with('\'')) {
    const size_t end = text.find('\'', 1);
    if (end == std::string_view::npos) {
      return false;
    }
    out = std::string(text.substr(1, end - 1));
    return true;
  }
  if (const size_t hash = text.find('#'); hash != std::string_view::npos) {
    text = text.substr(0, hash);
  }
  while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) {
    text.remove_suffix(1);
  }
  out = std::string(text);
  return !out.empty();
}

}  // namespace

bool Register(const char* name, Type type, void* value, const char* description) {
  auto [it, added] = Registry().try_emplace(name, Flag{type, value, description});
  if (!added) {
    std::fprintf(stderr, "flags: %s defined twice\n", name);
  }
  return true;
}

std::vector<std::string> ParseCommandLine(int argc, char** argv) {
  auto& registry = Registry();
  std::vector<std::string> others;
  for (int i = 1; i < argc; i++) {
    std::string_view arg = argv[i];
    if (!arg.starts_with("--") || arg.size() == 2) {
      others.emplace_back(arg);
      continue;
    }
    arg.remove_prefix(2);
    std::string_view name = arg;
    std::string_view value;
    bool has_value = false;
    if (const size_t eq = arg.find('='); eq != std::string_view::npos) {
      name = arg.substr(0, eq);
      value = arg.substr(eq + 1);
      has_value = true;
    }
    auto it = registry.find(name);
    if (it == registry.end() && !has_value && name.starts_with("no-")) {
      auto negated = registry.find(name.substr(3));
      if (negated != registry.end() && negated->second.type == Type::kBool) {
        *static_cast<bool*>(negated->second.value) = false;
        negated->second.from_command_line = true;
        continue;
      }
    }
    if (it == registry.end()) {
      Problems().push_back("unknown flag on the command line: --" + std::string(arg));
      continue;
    }
    Flag& flag = it->second;
    if (!has_value) {
      if (flag.type == Type::kBool) {
        value = "true";
      } else if (i + 1 < argc) {
        value = argv[++i];
      } else {
        Problems().push_back("no value for --" + std::string(name));
        continue;
      }
    }
    if (!SetFromText(flag, value)) {
      Problems().push_back("bad value for --" + std::string(name) + ": " + std::string(value));
      continue;
    }
    flag.from_command_line = true;
  }
  return others;
}

void LoadConfig(const std::filesystem::path& path) {
  std::ifstream file(path);
  if (!file) {
    return;
  }
  auto& registry = Registry();
  std::string line;
  std::string table;  // the [table] the keys are in; ours are all at the top
  int number = 0;
  while (std::getline(file, line)) {
    number++;
    std::string_view text = line;
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
      text.remove_prefix(1);
    }
    if (text.empty() || text.starts_with('#')) {
      continue;
    }
    if (text.starts_with('[')) {
      const size_t end = text.find(']');
      table = std::string(text.substr(1, end == std::string_view::npos ? 0 : end - 1));
      continue;
    }
    const size_t eq = text.find('=');
    if (eq == std::string_view::npos) {
      Problems().push_back(std::format("{}:{}: not a setting", path.filename().string(), number));
      continue;
    }
    std::string_view key = text.substr(0, eq);
    while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back()))) {
      key.remove_suffix(1);
    }
    const std::string name = table.empty() ? std::string(key) : table + "." + std::string(key);
    std::string value;
    if (!ParseTomlValue(text.substr(eq + 1), value)) {
      Problems().push_back(std::format("{}:{}: bad value for {}", path.filename().string(), number, name));
      continue;
    }
    auto it = registry.find(name);
    if (it == registry.end()) {
      Problems().push_back(std::format("{}: unknown setting {}", path.filename().string(), name));
      continue;
    }
    if (it->second.from_command_line) {
      continue;  // the command line wins
    }
    if (!SetFromText(it->second, value)) {
      Problems().push_back(std::format("{}:{}: bad value for {}: {}", path.filename().string(),
                                       number, name, value));
    }
  }
}

void WarnAboutProblems() {
  for (const std::string& problem : Problems()) {
    ITC_LOG_WARN("flags: {}", problem);
  }
  Problems().clear();
}

}  // namespace base::flags

namespace base {

void LogFlags() {
  if (!ITC_FLAG(log_flags)) {
    return;
  }
  for (const auto& [name, flag] : flags::Registry()) {
    ITC_LOG_INFO("flag {} = {}", name, flags::ValueText(flag));
  }
}

}  // namespace base
