#include "buffer_arena.h"

#include <algorithm>
#include <cstring>

#include "base/log.h"
#include "gpu_memory.h"

namespace native {

namespace {

uint32_t FindType(const VkPhysicalDeviceMemoryProperties& properties, uint32_t type_bits,
                  VkMemoryPropertyFlags wanted) {
  for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
    if ((type_bits & (1u << i)) && (properties.memoryTypes[i].propertyFlags & wanted) == wanted) {
      return i;
    }
  }
  return UINT32_MAX;
}

}  // namespace

bool BufferArena::Init(const DeviceContext& context, uint32_t frames_in_flight,
                       uint32_t fixed_bytes) {
  context_ = context;
  fixed_bytes_ = (fixed_bytes + kAlignment - 1) & ~(kAlignment - 1);
  VkDevice device = context.device;
  VkPhysicalDeviceMemoryProperties properties;
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &properties);

  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = VkDeviceSize(kSpaceSize) + fixed_bytes_ + kInlineSize;
  info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
               VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &info, nullptr, &buffer_) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, buffer_, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex =
      FindType(properties, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX) {
    alloc.memoryTypeIndex = FindType(properties, requirements.memoryTypeBits, 0);
  }
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &memory_) != VK_SUCCESS ||
      vk::vkBindBufferMemory(device, buffer_, memory_, 0) != VK_SUCCESS) {
    ITC_LOG_ERROR("native renderer: allocating the buffer arena failed");
    return false;
  }
  gpu_memory::Allocated(gpu_memory::Kind::kBuffers, alloc.allocationSize);
  free_[0] = kSpaceSize;

  frames_.resize(frames_in_flight);
  for (Frame& f : frames_) {
    VkBufferCreateInfo staging_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    staging_info.size = kStagingSize;
    staging_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    staging_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vk::vkCreateBuffer(device, &staging_info, nullptr, &f.staging) != VK_SUCCESS) {
      return false;
    }
    vk::vkGetBufferMemoryRequirements(device, f.staging, &requirements);
    VkMemoryAllocateInfo staging_alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    staging_alloc.allocationSize = requirements.size;
    staging_alloc.memoryTypeIndex =
        FindType(properties, requirements.memoryTypeBits,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (staging_alloc.memoryTypeIndex == UINT32_MAX ||
        vk::vkAllocateMemory(device, &staging_alloc, nullptr, &f.staging_memory) != VK_SUCCESS ||
        vk::vkBindBufferMemory(device, f.staging, f.staging_memory, 0) != VK_SUCCESS ||
        vk::vkMapMemory(device, f.staging_memory, 0, VK_WHOLE_SIZE, 0,
                        reinterpret_cast<void**>(&f.staging_mapped)) != VK_SUCCESS) {
      return false;
    }
    gpu_memory::Allocated(gpu_memory::Kind::kBuffers, staging_alloc.allocationSize);
    VkCommandPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    pool_info.queueFamilyIndex = context.queue_family;
    if (vk::vkCreateCommandPool(device, &pool_info, nullptr, &f.pool) != VK_SUCCESS) {
      return false;
    }
    VkCommandBufferAllocateInfo command_alloc{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    command_alloc.commandPool = f.pool;
    command_alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_alloc.commandBufferCount = 1;
    if (vk::vkAllocateCommandBuffers(device, &command_alloc, &f.uploads) != VK_SUCCESS) {
      return false;
    }
  }
  ITC_LOG_INFO("native renderer: buffer arena of {} MB ({} MB for buffers), {} MB staging a frame",
               info.size >> 20, kSpaceSize >> 20, kStagingSize >> 20);
  return true;
}

void BufferArena::BeginFrame(uint32_t frame_index) {
  frame_ = frame_index;
  frame_number_++;
  staging_used_ = 0;
  copies_.clear();
  vk::vkResetCommandPool(context_.device, frames_[frame_index].pool, 0);
  // Space no frame in flight can still read.
  size_t done = 0;
  while (done < retired_.size() &&
         retired_[done].frame_number + frames_.size() < frame_number_) {
    Release(retired_[done].offset, retired_[done].size);
    done++;
  }
  retired_.erase(retired_.begin(), retired_.begin() + done);
}

bool BufferArena::Allocate(uint32_t size, uint32_t& offset) {
  size = std::max((size + kAlignment - 1) & ~(kAlignment - 1), kAlignment);
  for (auto it = free_.begin(); it != free_.end(); ++it) {
    if (it->second < size) {
      continue;
    }
    offset = it->first;
    uint32_t left = it->second - size;
    free_.erase(it);
    if (left) {
      free_[offset + size] = left;
    }
    stats_.used += size;
    stats_.most_used = std::max(stats_.most_used, stats_.used);
    return true;
  }
  stats_.allocation_failed++;
  if (!full_logged_) {
    full_logged_ = true;
    ITC_LOG_WARN("native renderer: the buffer arena is full ({} MB used): a buffer of {} bytes "
                 "wasn't given space (logged once)",
                 stats_.used >> 20, size);
  }
  return false;
}

void BufferArena::Free(uint32_t offset, uint32_t size) {
  retired_.push_back({frame_number_, offset, size});
}

void BufferArena::Release(uint32_t offset, uint32_t size) {
  size = std::max((size + kAlignment - 1) & ~(kAlignment - 1), kAlignment);
  stats_.used -= size;
  auto next = free_.lower_bound(offset);
  if (next != free_.end() && offset + size == next->first) {
    size += next->second;
    next = free_.erase(next);
  }
  if (next != free_.begin()) {
    auto previous = std::prev(next);
    if (previous->first + previous->second == offset) {
      previous->second += size;
      return;
    }
  }
  free_[offset] = size;
}

bool BufferArena::Upload(uint32_t offset, const void* data, uint32_t size) {
  if (!size) {
    return true;
  }
  if (staging_used_ + size > kStagingSize) {
    stats_.staging_full++;
    return false;
  }
  std::memcpy(frames_[frame_].staging_mapped + staging_used_, data, size);
  copies_.push_back({staging_used_, offset, size});
  staging_used_ = (staging_used_ + size + 15) & ~VkDeviceSize(15);
  stats_.uploads++;
  stats_.upload_bytes += size;
  return true;
}

VkCommandBuffer BufferArena::EndUploads() {
  if (copies_.empty()) {
    return VK_NULL_HANDLE;
  }
  Frame& f = frames_[frame_];
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vk::vkBeginCommandBuffer(f.uploads, &begin);
  // Earlier submissions' reads (the frames in flight drawing from the space
  // written) before the copies.
  VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  vk::vkCmdPipelineBarrier(f.uploads, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &barrier, 0, nullptr, 0, nullptr);
  vk::vkCmdCopyBuffer(f.uploads, f.staging, buffer_, uint32_t(copies_.size()), copies_.data());
  // The copies before this frame's reads.
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_SHADER_READ_BIT;
  vk::vkCmdPipelineBarrier(f.uploads, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, 0, 1, &barrier, 0, nullptr, 0,
                           nullptr);
  vk::vkEndCommandBuffer(f.uploads);
  copies_.clear();
  return f.uploads;
}

BufferArena::Stats BufferArena::TakeStats() {
  Stats stats = stats_;
  stats_ = {};
  stats_.used = stats.used;
  stats_.most_used = stats.used;
  return stats;
}

}  // namespace native
