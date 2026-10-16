#include "disk_cache.h"

#include <rex/cvar.h>
#include <rex/logging.h>

REXCVAR_DEFINE_BOOL(native_renderer_disk_cache, true, "ITC",
                    "Keep translated shaders and compiled pipelines in cache/ next to the "
                    "executable between sessions");

namespace native {

std::filesystem::path DiskCacheDirectory() {
  if (!REXCVAR_GET(native_renderer_disk_cache)) {
    return {};
  }
  static const std::filesystem::path directory = [] {
    std::error_code error;
    std::filesystem::path exe = std::filesystem::read_symlink("/proc/self/exe", error);
    if (error) {
      REXLOG_WARN("native renderer: no disk cache: can't find the executable ({})",
                  error.message());
      return std::filesystem::path();
    }
    std::filesystem::path path = exe.parent_path() / "cache";
    std::filesystem::create_directories(path, error);
    if (error) {
      REXLOG_WARN("native renderer: no disk cache: can't make {} ({})", path.string(),
                  error.message());
      return std::filesystem::path();
    }
    return path;
  }();
  return directory;
}

}  // namespace native
