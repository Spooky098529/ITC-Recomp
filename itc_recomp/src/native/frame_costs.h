// Diagnostics: what a frame spent on work that comes and goes (texture
// loads, shader translations, pipeline creations), timed only when that
// work happens, so a slow frame can say what made it slow. Render thread
// only; the renderer logs and resets it each present.

#pragma once

#include <chrono>
#include <cstdint>

#include "log_enabled.h"

namespace native {

struct FrameCosts {
  int64_t texture_ns = 0;
  uint32_t textures = 0;
  uint64_t texture_bytes = 0;
  int64_t hash_ns = 0;  // hashing textures' memory (first loads and rechecks)
  int64_t shader_ns = 0;
  uint32_t shaders = 0;
  int64_t pipeline_ns = 0;
  uint32_t pipelines = 0;
};

inline FrameCosts g_frame_costs;

// Adds the time from construction to destruction to `ns`, when info
// logging is on (nothing else reads it).
class ScopedCost {
 public:
  explicit ScopedCost(int64_t& ns) : ns_(ns), timed_(InfoLogEnabled()) {
    if (timed_) {
      start_ = std::chrono::steady_clock::now();
    }
  }
  ~ScopedCost() {
    if (timed_) {
      ns_ += std::chrono::duration_cast<std::chrono::nanoseconds>(
                 std::chrono::steady_clock::now() - start_)
                 .count();
    }
  }

 private:
  int64_t& ns_;
  bool timed_;
  std::chrono::steady_clock::time_point start_;
};

}  // namespace native
