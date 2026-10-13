// The Vulkan device as the native renderer's parts see it (created by
// renderer.cpp).

#pragma once

#include <cstdint>

#include "vulkan_functions.h"

namespace native {

struct DeviceContext {
  VkPhysicalDevice physical_device;
  VkDevice device;
  uint32_t queue_family;
  VkQueue queue;
  uint32_t max_storage_buffer_range;
  uint32_t min_uniform_buffer_offset_alignment;
  bool clip_distance, cull_distance, demote_to_helper_invocation;
  bool sampler_anisotropy, sampler_mirror_clamp_to_edge;
  float max_sampler_anisotropy;
};

}  // namespace native
