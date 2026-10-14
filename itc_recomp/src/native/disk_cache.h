// The native renderer's caches on disk (translated shaders, the Vulkan
// pipeline cache), in cache/ next to the executable, so shaders seen in an
// earlier session aren't translated and compiled again when they first
// appear (each was a hitch of ~1 ms, many at once when new scenery came
// into view). --native_renderer_disk_cache=false turns them off.

#pragma once

#include <filesystem>

namespace native {

// The cache directory (created if needed), or empty if the caches are off
// or it can't be made.
std::filesystem::path DiskCacheDirectory();

}  // namespace native
