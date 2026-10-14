// Diagnostics: what a frame spent on work that comes and goes (texture
// loads, shader translations, pipeline creations), timed only when that
// work happens, so a slow frame can say what made it slow. Render thread
// only; the renderer logs and resets it each present.

#pragma once

#include <chrono>
#include <cstdint>

namespace native {

struct FrameCosts {
  int64_t texture_ns = 0;
  uint32_t textures = 0;
  uint64_t texture_bytes = 0;
  int64_t shader_ns = 0;
  uint32_t shaders = 0;
  int64_t pipeline_ns = 0;
  uint32_t pipelines = 0;
};

inline FrameCosts g_frame_costs;

// Adds the time from construction to destruction to `ns`.
class ScopedCost {
 public:
  explicit ScopedCost(int64_t& ns) : ns_(ns), start_(std::chrono::steady_clock::now()) {}
  ~ScopedCost() {
    ns_ += std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now() - start_)
               .count();
  }

 private:
  int64_t& ns_;
  std::chrono::steady_clock::time_point start_;
};

}  // namespace native
