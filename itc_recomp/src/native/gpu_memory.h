// Running totals of what the native renderer holds on the GPU: device
// memory by what it's for, and the counts of other Vulkan objects that
// take driver memory. Counted always (these are made rarely), logged every
// 5 s with --log_gpu_memory (Renderer::LogStats), to find what grows from
// race to race.

#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <string>

namespace native::gpu_memory {

enum class Kind {
  kTexture,    // textures loaded from guest memory
  kResolved,   // resolve destinations (textures drawn into)
  kImage,      // Passes::State::CreateImage: targets, dummies, the shown frame
  kStaging,    // texture upload staging buffers
  kMirror,     // guest memory mirrors
  kConstants,  // constant blocks
  kOther,      // the gamma table
  kCount
};

enum class Object {
  kImageView,       // textures' swizzled views and attachment views
  kSampler,
  kDescriptorPool,
  kPipeline,
  // New textures starting where an older texture starts (a different key
  // for the same memory: likely the old one's memory reused).
  kTextureAtTakenAddress,
  kCount
};

struct Totals {
  std::array<std::atomic<int64_t>, size_t(Kind::kCount)> bytes{};
  std::array<std::atomic<int64_t>, size_t(Kind::kCount)> allocations{};
  std::array<std::atomic<int64_t>, size_t(Object::kCount)> objects{};
};

inline Totals& Get() {
  static Totals totals;
  return totals;
}

inline void Allocated(Kind kind, uint64_t bytes) {
  Get().bytes[size_t(kind)].fetch_add(int64_t(bytes), std::memory_order_relaxed);
  Get().allocations[size_t(kind)].fetch_add(1, std::memory_order_relaxed);
}

inline void Freed(Kind kind, uint64_t bytes) {
  Get().bytes[size_t(kind)].fetch_sub(int64_t(bytes), std::memory_order_relaxed);
  Get().allocations[size_t(kind)].fetch_sub(1, std::memory_order_relaxed);
}

inline void Created(Object object, int64_t count = 1) {
  Get().objects[size_t(object)].fetch_add(count, std::memory_order_relaxed);
}

// "texture 312.4 MB (1203); ..." for the log.
std::string Describe();

}  // namespace native::gpu_memory
