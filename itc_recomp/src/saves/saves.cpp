#include "saves.h"

#include "helpers/executable.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <system_error>

namespace saves {

namespace {

// saves/ next to the executable; empty when the executable can't be found.
const std::filesystem::path& Folder() {
  static const std::filesystem::path folder = [] {
    std::error_code error;
    std::filesystem::path exe_folder = helpers::ExecutableFolder(error);
    return error ? std::filesystem::path() : exe_folder / "saves";
  }();
  return folder;
}

std::optional<std::filesystem::path> PathOf(const std::string& name) {
  if (Folder().empty() || !FileName(name)) {
    return std::nullopt;
  }
  return Folder() / name;
}

}  // namespace

std::optional<std::string> FileName(std::string_view path) {
  size_t start = path.find_last_of(":\\/");
  std::string_view name = start == std::string_view::npos ? path : path.substr(start + 1);
  if (name.empty() || name.front() == '.') {
    return std::nullopt;
  }
  for (char c : name) {
    bool plain = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
                 c == '_' || c == '-' || c == '.';
    if (!plain) {
      return std::nullopt;
    }
  }
  return std::string(name);
}

bool Exists(const std::string& name) {
  auto path = PathOf(name);
  std::error_code error;
  return path && std::filesystem::is_regular_file(*path, error);
}

std::optional<std::vector<uint8_t>> Read(const std::string& name, uint32_t max_bytes) {
  auto path = PathOf(name);
  if (!path) {
    return std::nullopt;
  }
  std::ifstream file(*path, std::ios::binary);
  if (!file) {
    return std::nullopt;
  }
  std::vector<uint8_t> bytes(max_bytes);
  file.read(reinterpret_cast<char*>(bytes.data()), max_bytes);
  if (file.bad()) {
    return std::nullopt;
  }
  bytes.resize(size_t(file.gcount()));
  return bytes;
}

bool Write(const std::string& name, const uint8_t* bytes, uint32_t size) {
  auto path = PathOf(name);
  if (!path) {
    return false;
  }
  std::error_code error;
  std::filesystem::create_directories(Folder(), error);
  if (error) {
    return false;
  }
  std::filesystem::path temporary = *path;
  temporary += ".tmp";
  {
    std::ofstream file(temporary, std::ios::binary | std::ios::trunc);
    file.write(reinterpret_cast<const char*>(bytes), size);
    file.close();
    if (!file) {
      std::filesystem::remove(temporary, error);
      return false;
    }
  }
  std::filesystem::rename(temporary, *path, error);
  if (error) {
    std::filesystem::remove(temporary, error);
    return false;
  }
  return true;
}

bool Delete(const std::string& name) {
  auto path = PathOf(name);
  if (!path) {
    return false;
  }
  std::error_code error;
  std::filesystem::remove(*path, error);
  return !error && !Exists(name);
}

std::optional<std::string> WrittenAt(const std::string& name) {
  auto path = PathOf(name);
  if (!path) {
    return std::nullopt;
  }
  std::error_code error;
  auto written = std::filesystem::last_write_time(*path, error);
  if (error) {
    return std::nullopt;
  }
  using namespace std::chrono;
  auto utc = time_point_cast<minutes>(file_clock::to_sys(written));
  auto day = floor<days>(utc);
  year_month_day date(day);
  hh_mm_ss time(utc - day);
  char text[32];
  std::snprintf(text, sizeof(text), "%04d/%02u/%02u %02d:%02d", int(date.year()),
                unsigned(date.month()), unsigned(date.day()), int(time.hours().count()),
                int(time.minutes().count()));
  return std::string(text);
}

}  // namespace saves
