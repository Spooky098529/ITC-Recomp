// The Vulkan device as the native renderer's parts see it (created by
// renderer.cpp).

#pragma once

#include <array>
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
  // For checking a saved pipeline cache is this device's.
  uint32_t vendor_id, device_id;
  std::array<uint8_t, VK_UUID_SIZE> pipeline_cache_uuid;
};

}  // namespace native
