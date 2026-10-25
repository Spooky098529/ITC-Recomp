#include "disk_cache.h"

#include "helpers/executable.h"

#include "base/flags.h"
#include "base/log.h"

#ifdef _WIN32
#include <cstring>
#include <string>
#endif

ITC_FLAG_BOOL(native_renderer_disk_cache, true, "ITC",
              "Keep translated shaders and compiled pipelines in cache/ next to the "
              "executable between sessions");

namespace native {

std::filesystem::path DiskCacheDirectory() {
  if (!ITC_FLAG(native_renderer_disk_cache)) {
    return {};
  }
  static const std::filesystem::path directory = [] {
    std::error_code error;
    std::filesystem::path folder = helpers::ExecutableFolder(error);
    if (error) {
      ITC_LOG_WARN("native renderer: no disk cache: can't find the executable ({})",
                   error.message());
      return std::filesystem::path();
    }
    std::filesystem::path path = folder / "cache";
    std::filesystem::create_directories(path, error);
    if (error) {
      ITC_LOG_WARN("native renderer: no disk cache: can't make {} ({})", path.string(),
                   error.message());
      return std::filesystem::path();
    }
    return path;
  }();
  return directory;
}

std::FILE* OpenFile(const std::filesystem::path& path, const char* mode) {
#ifdef _WIN32
  std::wstring wide_mode(mode, mode + std::strlen(mode));
  return _wfopen(path.c_str(), wide_mode.c_str());
#else
  return std::fopen(path.c_str(), mode);
#endif
}

}  // namespace native
