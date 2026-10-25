#include "game_data.h"

#include <array>
#include <cctype>
#include <cstdio>
#include <memory>
#include <mutex>
#include <string>
#include <system_error>
#include <unordered_map>
#include <vector>

namespace game_data {

namespace {

std::filesystem::path g_root;

struct File {
  std::FILE* file = nullptr;
  uint64_t size = 0;
  uint64_t position = 0;
  std::mutex mutex;  // the position and the FILE's own
};

std::mutex g_table_mutex;
std::array<std::unique_ptr<File>, kHandleCount> g_files;

// Names found without regard to case, by the lowercased relative path.
std::mutex g_found_mutex;
std::unordered_map<std::string, std::filesystem::path> g_found;

std::string Lower(std::string_view s) {
  std::string out(s);
  for (char& c : out) {
    c = char(std::tolower(static_cast<unsigned char>(c)));
  }
  return out;
}

std::FILE* OpenFile(const std::filesystem::path& path) {
#ifdef _WIN32
  return _wfopen(path.c_str(), L"rb");
#else
  return std::fopen(path.c_str(), "rb");
#endif
}

bool SeekFile(std::FILE* file, uint64_t offset) {
#ifdef _WIN32
  return _fseeki64(file, int64_t(offset), SEEK_SET) == 0;
#else
  return fseeko(file, off_t(offset), SEEK_SET) == 0;
#endif
}

File* Lookup(uint32_t handle) {
  if (!IsHandle(handle)) {
    return nullptr;
  }
  std::lock_guard lock(g_table_mutex);
  return g_files[handle - kFirstHandle].get();
}

}  // namespace

void SetRoot(const std::filesystem::path& root) {
  g_root = root;
}

std::optional<std::string_view> RelativePath(std::string_view game_path) {
  size_t colon = game_path.find(':');
  if (colon == std::string_view::npos) {
    return game_path;
  }
  if (Lower(game_path.substr(0, colon)) != "game") {
    return std::nullopt;
  }
  std::string_view rest = game_path.substr(colon + 1);
  while (!rest.empty() && (rest.front() == '\\' || rest.front() == '/')) {
    rest.remove_prefix(1);
  }
  return rest;
}

std::optional<std::filesystem::path> Find(std::string_view relative_path) {
  if (g_root.empty() || relative_path.empty()) {
    return std::nullopt;
  }
  // The parts, without empty ones, "." or ".." (nothing outside the root).
  std::filesystem::path exact = g_root;
  std::string key;
  std::vector<std::string> parts;
  size_t start = 0;
  while (start <= relative_path.size()) {
    size_t end = relative_path.find_first_of("\\/", start);
    if (end == std::string_view::npos) {
      end = relative_path.size();
    }
    std::string_view part = relative_path.substr(start, end - start);
    if (part == "..") {
      return std::nullopt;
    }
    if (!part.empty() && part != ".") {
      parts.emplace_back(part);
      exact /= std::string(part);
      key += "/" + Lower(part);
    }
    start = end + 1;
  }
  if (parts.empty()) {
    return std::nullopt;
  }
  std::error_code error;
  if (std::filesystem::is_regular_file(exact, error)) {
    return exact;
  }
  {
    std::lock_guard lock(g_found_mutex);
    if (auto it = g_found.find(key); it != g_found.end()) {
      return it->second;
    }
  }
  // Each part matched against its folder's entries whatever their case.
  std::filesystem::path path = g_root;
  for (const std::string& part : parts) {
    std::filesystem::path next = path / part;
    if (!std::filesystem::exists(next, error)) {
      const std::string lower = Lower(part);
      next.clear();
      for (const auto& entry : std::filesystem::directory_iterator(path, error)) {
        if (Lower(entry.path().filename().string()) == lower) {
          next = entry.path();
          break;
        }
      }
      if (next.empty()) {
        return std::nullopt;
      }
    }
    path = next;
  }
  if (!std::filesystem::is_regular_file(path, error)) {
    return std::nullopt;
  }
  std::lock_guard lock(g_found_mutex);
  g_found[key] = path;
  return path;
}

std::optional<uint32_t> Open(std::string_view relative_path) {
  auto path = Find(relative_path);
  if (!path) {
    return std::nullopt;
  }
  std::error_code error;
  uint64_t size = std::filesystem::file_size(*path, error);
  if (error) {
    return std::nullopt;
  }
  std::FILE* file = OpenFile(*path);
  if (!file) {
    return std::nullopt;
  }
  std::lock_guard lock(g_table_mutex);
  for (uint32_t i = 0; i < kHandleCount; i++) {
    if (!g_files[i]) {
      g_files[i] = std::make_unique<File>();
      g_files[i]->file = file;
      g_files[i]->size = size;
      return kFirstHandle + i;
    }
  }
  std::fclose(file);
  return std::nullopt;
}

bool Close(uint32_t handle) {
  if (!IsHandle(handle)) {
    return false;
  }
  std::unique_ptr<File> file;
  {
    std::lock_guard lock(g_table_mutex);
    file = std::move(g_files[handle - kFirstHandle]);
  }
  if (!file) {
    return false;
  }
  std::lock_guard lock(file->mutex);
  std::fclose(file->file);
  return true;
}

std::optional<uint64_t> Size(uint32_t handle) {
  File* file = Lookup(handle);
  if (!file) {
    return std::nullopt;
  }
  return file->size;
}

std::optional<uint32_t> Read(uint32_t handle, uint8_t* out, uint32_t size) {
  File* file = Lookup(handle);
  if (!file) {
    return std::nullopt;
  }
  std::lock_guard lock(file->mutex);
  if (!SeekFile(file->file, file->position)) {
    return std::nullopt;
  }
  size_t read = std::fread(out, 1, size, file->file);
  if (read < size && std::ferror(file->file)) {
    std::clearerr(file->file);
    return std::nullopt;
  }
  file->position += read;
  return uint32_t(read);
}

std::optional<uint32_t> ReadAt(uint32_t handle, uint64_t offset, uint8_t* out, uint32_t size) {
  File* file = Lookup(handle);
  if (!file) {
    return std::nullopt;
  }
  std::lock_guard lock(file->mutex);
  if (!SeekFile(file->file, offset)) {
    return std::nullopt;
  }
  size_t read = std::fread(out, 1, size, file->file);
  if (read < size && std::ferror(file->file)) {
    std::clearerr(file->file);
    return std::nullopt;
  }
  return uint32_t(read);
}

std::optional<uint64_t> Seek(uint32_t handle, int64_t offset, uint32_t method) {
  File* file = Lookup(handle);
  if (!file || method > 2) {
    return std::nullopt;
  }
  std::lock_guard lock(file->mutex);
  int64_t from = method == 0 ? 0 : method == 1 ? int64_t(file->position) : int64_t(file->size);
  int64_t position = from + offset;
  if (position < 0) {
    return std::nullopt;
  }
  file->position = uint64_t(position);
  return file->position;
}

}  // namespace game_data
