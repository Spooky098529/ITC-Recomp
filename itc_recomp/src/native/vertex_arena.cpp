// See vertex_arena.h.

#include "vertex_arena.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

#include "base/log.h"
#include "buffers.h"
#include "gpu_memory.h"
#include "state.h"

namespace native {

namespace {

// The arena: the built-in index lists, then each frame slot's inline
// vertices, then the ranges draws use.
constexpr uint32_t kInlineSlotBytes = 16u << 20;
// A range key's address bits (its top two: the endian mode, Place).
constexpr uint32_t kAddressBits = 0x3FFFFFFFu;
constexpr uint32_t kRangeBytes = 192u << 20;
// Staging: a chunk a frame slot, more while one frame needs more (a load),
// given back when the slot comes round again.
constexpr uint32_t kStagingChunk = 16u << 20;
constexpr uint32_t kAlignment = 256;
// Ranges not drawn for this many frames are given back (models of a race
// left, the work memory's moved morphs).
constexpr uint64_t kUnusedFrames = 900;

uint32_t FindMemoryType(const VkPhysicalDeviceMemoryProperties& properties, uint32_t type_bits,
                        VkMemoryPropertyFlags wanted) {
  for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
    if ((type_bits & (1u << i)) && (properties.memoryTypes[i].propertyFlags & wanted) == wanted) {
      return i;
    }
  }
  return UINT32_MAX;
}

uint32_t AlignUp(uint32_t value) {
  return (value + kAlignment - 1) & ~(kAlignment - 1);
}

}  // namespace

struct VertexArena::State {
  DeviceContext context{};
  VkPhysicalDeviceMemoryProperties memory_properties{};
  uint32_t frames_in_flight = 0;
  uint32_t frame = 0;
  uint64_t frame_number = 0;

  VkBuffer buffer = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  uint32_t inline_start = 0, range_start = 0, size = 0;
  uint32_t inline_used = 0;

  struct StagingChunk {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapped = nullptr;
    uint32_t size = 0;
  };
  struct Frame {
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer uploads = VK_NULL_HANDLE;
    std::vector<StagingChunk> staging;  // [0] kept; others freed at the slot's next frame
    uint32_t chunk = 0, chunk_used = 0;
    // Ranges given back while this slot's frame was recorded: free once its
    // fence has passed.
    std::vector<std::pair<uint32_t, uint32_t>> to_free;
  };
  std::vector<Frame> frames;
  bool recording = false;
  uint32_t recording_slot = 0;  // the slot whose command buffer and staging hold the copies

  // The ranges in the arena, by address << 32 | size; and by start, to find
  // those a write overlaps.
  struct Range {
    uint32_t offset = 0;
    uint64_t last_used = 0;
    bool written = false;
  };
  std::unordered_map<uint64_t, Range> ranges;
  std::multimap<uint32_t, uint64_t> by_start;
  uint32_t longest = 0;
  // Free space in the ranges' part: offset -> size, merged.
  std::map<uint32_t, uint32_t> free_space;
  uint64_t used_bytes = 0;

  std::vector<std::pair<uint32_t, uint32_t>> writes;

  struct Stats {
    uint64_t placed = 0, copied = 0, copied_bytes = 0, recopied = 0, inline_bytes = 0;
    uint64_t full = 0, inline_full = 0, freed = 0, writes = 0, writes_hit = 0;
    uint32_t frames = 0;
  } stats;
  bool logged_full = false;

  bool CreateStaging(StagingChunk& chunk, uint32_t bytes);
  void DestroyStaging(StagingChunk& chunk);
  // Space in this frame's staging for `bytes`: the chunk and its offset.
  std::pair<StagingChunk*, uint32_t> Stage(uint32_t bytes);
  void Copy(const void* data, uint32_t bytes, uint32_t arena_offset, uint32_t endian = 0);
  void BeginRecording();
  uint32_t Allocate(uint32_t bytes);
  void Free(uint32_t offset, uint32_t bytes);
  void GiveBack(uint64_t older_than);
};

VertexArena::VertexArena() : state_(std::make_unique<State>()) {}

VertexArena::~VertexArena() {
  State& s = *state_;
  if (!s.context.device) {
    return;
  }
  VkDevice device = s.context.device;
  for (State::Frame& f : s.frames) {
    for (State::StagingChunk& chunk : f.staging) {
      s.DestroyStaging(chunk);
    }
    if (f.pool) {
      vk::vkDestroyCommandPool(device, f.pool, nullptr);
    }
  }
  if (s.buffer) {
    vk::vkDestroyBuffer(device, s.buffer, nullptr);
  }
  if (s.memory) {
    vk::vkFreeMemory(device, s.memory, nullptr);
    gpu_memory::Freed(gpu_memory::Kind::kMirror, s.size);
  }
}

bool VertexArena::State::CreateStaging(StagingChunk& chunk, uint32_t bytes) {
  VkDevice device = context.device;
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = bytes;
  info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &info, nullptr, &chunk.buffer) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, chunk.buffer, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex =
      FindMemoryType(memory_properties, requirements.memoryTypeBits,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &chunk.memory) != VK_SUCCESS ||
      vk::vkBindBufferMemory(device, chunk.buffer, chunk.memory, 0) != VK_SUCCESS ||
      vk::vkMapMemory(device, chunk.memory, 0, VK_WHOLE_SIZE, 0,
                      reinterpret_cast<void**>(&chunk.mapped)) != VK_SUCCESS) {
    return false;
  }
  chunk.size = bytes;
  gpu_memory::Allocated(gpu_memory::Kind::kStaging, alloc.allocationSize);
  return true;
}

void VertexArena::State::DestroyStaging(StagingChunk& chunk) {
  VkDevice device = context.device;
  if (chunk.buffer) {
    vk::vkDestroyBuffer(device, chunk.buffer, nullptr);
  }
  if (chunk.memory) {
    vk::vkFreeMemory(device, chunk.memory, nullptr);
    gpu_memory::Freed(gpu_memory::Kind::kStaging, chunk.size);
  }
  chunk = {};
}

bool VertexArena::Init(const DeviceContext& context, uint32_t frames_in_flight) {
  State& s = *state_;
  s.context = context;
  s.frames_in_flight = frames_in_flight;
  VkDevice device = context.device;
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &s.memory_properties);

  s.inline_start = AlignUp(kQuadIndices + 4 * 6 * kBuiltinPrimitives);
  s.range_start = s.inline_start + frames_in_flight * kInlineSlotBytes;
  s.size = s.range_start + kRangeBytes;
  // The shaders read it as one storage buffer (xe_interface.glsl's
  // xe_memory).
  if (s.size > context.max_storage_buffer_range) {
    ITC_LOG_ERROR("native renderer: the vertex arena ({} MB) is larger than a storage buffer "
                  "can be on this GPU ({} MB)",
                  s.size >> 20, context.max_storage_buffer_range >> 20);
    return false;
  }
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = s.size;
  info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
               VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
               VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &info, nullptr, &s.buffer) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, s.buffer, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = FindMemoryType(s.memory_properties, requirements.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &s.memory) != VK_SUCCESS ||
      vk::vkBindBufferMemory(device, s.buffer, s.memory, 0) != VK_SUCCESS) {
    ITC_LOG_ERROR("native renderer: the vertex arena ({} MB, device-local) couldn't be made",
                  s.size >> 20);
    return false;
  }
  gpu_memory::Allocated(gpu_memory::Kind::kMirror, s.size);
  s.free_space.emplace(s.range_start, kRangeBytes);

  s.frames.resize(frames_in_flight);
  for (State::Frame& f : s.frames) {
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
    f.staging.emplace_back();
    if (!s.CreateStaging(f.staging.back(), kStagingChunk)) {
      return false;
    }
  }

  // The built-in index lists, through the first frame's staging.
  std::vector<uint32_t> indices;
  for (uint32_t i = 0; i < kBuiltinPrimitives; i++) {
    if (i) {
      indices.push_back(UINT32_MAX);
    }
    for (uint32_t j = 0; j < 4; j++) {
      indices.push_back(i << 2 | j);
    }
  }
  s.frame = 0;
  s.recording_slot = 0;
  s.Copy(indices.data(), uint32_t(indices.size() * 4), kRectangleIndices);
  indices.clear();
  for (uint32_t i = 0; i < kBuiltinPrimitives; i++) {
    uint32_t v = i << 2;
    for (uint32_t k : {v, v + 1, v + 2, v, v + 2, v + 3}) {
      indices.push_back(k);
    }
  }
  s.Copy(indices.data(), uint32_t(indices.size() * 4), kQuadIndices);
  ITC_LOG_INFO("native renderer: vertex arena of {} MB (device-local): {} MB of ranges, {} x "
               "{} MB of inline vertices",
               s.size >> 20, kRangeBytes >> 20, frames_in_flight, kInlineSlotBytes >> 20);
  // From here on memcpy keeps the writes into physical memory for us.
  Buffers_CollectMirrorWrites();
  return true;
}

void VertexArena::BeginFrame(uint32_t frame) {
  State& s = *state_;
  s.frame = frame;
  s.frame_number++;
  s.stats.frames++;
  s.inline_used = 0;
  State::Frame& f = s.frames[frame];
  // Its last frame has finished (the fence was waited), with its copies and
  // draws: what it gave back is free, and its staging too. (Copies recorded
  // and not yet submitted, the built-in lists' from Init, stay in their
  // slot until this frame's EndUploads.)
  for (auto [offset, bytes] : f.to_free) {
    s.Free(offset, bytes);
  }
  f.to_free.clear();
  if (!s.recording) {
    vk::vkResetCommandPool(s.context.device, f.pool, 0);
    while (f.staging.size() > 1) {
      s.DestroyStaging(f.staging.back());
      f.staging.pop_back();
    }
    f.chunk = 0;
    f.chunk_used = 0;
    s.recording_slot = frame;
  }
  // memcpy's writes since the last frame: the ranges they overlap are
  // copied again at their next draw.
  s.writes.clear();
  Buffers_TakeMirrorWrites(s.writes);
  s.stats.writes += s.writes.size();
  for (auto [start, end] : s.writes) {
    uint32_t from = start > s.longest ? start - s.longest : 0;
    for (auto it = s.by_start.lower_bound(from); it != s.by_start.end() && it->first < end; ++it) {
      uint32_t range_end = it->first + uint32_t(it->second);
      if (range_end > start) {
        auto found = s.ranges.find(it->second);
        if (found != s.ranges.end()) {
          s.stats.writes_hit += !found->second.written;
          found->second.written = true;
        }
      }
    }
  }
  if (s.frame_number % 300 == 0) {
    s.GiveBack(kUnusedFrames);
  }
}

uint32_t VertexArena::State::Allocate(uint32_t bytes) {
  bytes = AlignUp(bytes);
  for (auto it = free_space.begin(); it != free_space.end(); ++it) {
    if (it->second >= bytes) {
      uint32_t offset = it->first;
      uint32_t left = it->second - bytes;
      free_space.erase(it);
      if (left) {
        free_space.emplace(offset + bytes, left);
      }
      used_bytes += bytes;
      return offset;
    }
  }
  return UINT32_MAX;
}

void VertexArena::State::Free(uint32_t offset, uint32_t bytes) {
  bytes = AlignUp(bytes);
  used_bytes -= bytes;
  auto [it, added] = free_space.emplace(offset, bytes);
  // Merge with the next and the previous.
  auto next = std::next(it);
  if (next != free_space.end() && it->first + it->second == next->first) {
    it->second += next->second;
    free_space.erase(next);
  }
  if (it != free_space.begin()) {
    auto previous = std::prev(it);
    if (previous->first + previous->second == it->first) {
      previous->second += it->second;
      free_space.erase(it);
    }
  }
}

void VertexArena::State::GiveBack(uint64_t older_than) {
  std::vector<std::pair<uint32_t, uint32_t>>& to_free = frames[frame].to_free;
  for (auto it = ranges.begin(); it != ranges.end();) {
    if (frame_number - it->second.last_used <= older_than) {
      ++it;
      continue;
    }
    uint32_t start = uint32_t(it->first >> 32) & kAddressBits, bytes = uint32_t(it->first);
    // Freed when this slot's fence has passed again: frames in flight may
    // still draw from it.
    to_free.emplace_back(it->second.offset, bytes);
    auto [first, last] = by_start.equal_range(start);
    for (auto b = first; b != last; ++b) {
      if (b->second == it->first) {
        by_start.erase(b);
        break;
      }
    }
    stats.freed++;
    it = ranges.erase(it);
  }
}

void VertexArena::State::BeginRecording() {
  if (recording) {
    return;
  }
  recording = true;
  VkCommandBuffer commands = frames[recording_slot].uploads;
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vk::vkBeginCommandBuffer(commands, &begin);
  // Earlier frames' draws may still read ranges copied again in place:
  // the copies wait for them.
  VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &barrier, 0, nullptr, 0,
                           nullptr);
}

std::pair<VertexArena::State::StagingChunk*, uint32_t> VertexArena::State::Stage(
    uint32_t bytes) {
  Frame& f = frames[recording_slot];
  StagingChunk* chunk = &f.staging[f.chunk];
  if (f.chunk_used + bytes > chunk->size) {
    f.staging.emplace_back();
    if (!CreateStaging(f.staging.back(), std::max(bytes, kStagingChunk))) {
      DestroyStaging(f.staging.back());
      f.staging.pop_back();
      return {nullptr, 0};
    }
    f.chunk = uint32_t(f.staging.size() - 1);
    f.chunk_used = 0;
    chunk = &f.staging.back();
  }
  uint32_t at = f.chunk_used;
  f.chunk_used = (f.chunk_used + bytes + 15) & ~15u;
  return {chunk, at};
}

void VertexArena::State::Copy(const void* data, uint32_t bytes, uint32_t arena_offset,
                              uint32_t endian) {
  if (!bytes) {
    return;
  }
  auto [chunk, at] = Stage(bytes);
  if (!chunk) {
    return;
  }
  std::memcpy(chunk->mapped + at, data, bytes);
  if (endian == 1) {  // 8-in-16
    uint8_t* p = chunk->mapped + at;
    for (uint32_t i = 0; i + 1 < bytes; i += 2) {
      std::swap(p[i], p[i + 1]);
    }
  } else if (endian == 2) {  // 8-in-32
    uint8_t* p = chunk->mapped + at;
    for (uint32_t i = 0; i + 3 < bytes; i += 4) {
      uint32_t v;
      std::memcpy(&v, p + i, 4);
      v = __builtin_bswap32(v);
      std::memcpy(p + i, &v, 4);
    }
  } else if (endian == 3) {  // 16-in-32
    uint8_t* p = chunk->mapped + at;
    for (uint32_t i = 0; i + 3 < bytes; i += 4) {
      uint32_t v;
      std::memcpy(&v, p + i, 4);
      v = v << 16 | v >> 16;
      std::memcpy(p + i, &v, 4);
    }
  }
  BeginRecording();
  VkBufferCopy region{at, arena_offset, bytes};
  vk::vkCmdCopyBuffer(frames[recording_slot].uploads, chunk->buffer, buffer, 1, &region);
  stats.copied++;
  stats.copied_bytes += bytes;
}

uint32_t VertexArena::Place(uint32_t address, uint32_t size, uint32_t endian) {
  State& s = *state_;
  s.stats.placed++;
  if (!size) {
    return s.range_start;
  }
  // The endian mode in the key's top bits (physical addresses are 29 bits).
  uint64_t key = (uint64_t(address | (endian & 3) << 30) << 32) | size;
  auto it = s.ranges.find(key);
  if (it == s.ranges.end()) {
    uint32_t offset = s.Allocate(size);
    if (offset == UINT32_MAX) {
      // Full: give back what the last few frames didn't draw, and try again
      // (it's free at once only if no frame in flight drew it: those wait).
      s.GiveBack(s.frames_in_flight + 1);
      offset = s.Allocate(size);
    }
    if (offset == UINT32_MAX) {
      s.stats.full++;
      if (!s.logged_full) {
        s.logged_full = true;
        ITC_LOG_ERROR("native renderer: the vertex arena is full ({} MB used, {} ranges): a "
                      "range of {} bytes at {:08X} left out",
                      s.used_bytes >> 20, s.ranges.size(), size, address);
      }
      return UINT32_MAX;
    }
    it = s.ranges.emplace(key, State::Range{offset, 0, true}).first;
    s.by_start.emplace(address, key);
    s.longest = std::max(s.longest, size);
  } else if (it->second.written) {
    s.stats.recopied++;
  }
  State::Range& range = it->second;
  range.last_used = s.frame_number;
  if (range.written) {
    range.written = false;
    s.Copy(PhysicalHostAddress(address), size, range.offset, endian);
  }
  return range.offset;
}

uint32_t VertexArena::PlaceInline(const void* data, uint32_t size) {
  State& s = *state_;
  uint32_t at = AlignUp(s.inline_used);
  if (at + size > kInlineSlotBytes) {
    s.stats.inline_full++;
    return UINT32_MAX;
  }
  s.inline_used = at + size;
  uint32_t offset = s.inline_start + s.frame * kInlineSlotBytes + at;
  s.Copy(data, size, offset);
  s.stats.inline_bytes += size;
  return offset;
}

VkBuffer VertexArena::buffer() const {
  return state_->buffer;
}

VkCommandBuffer VertexArena::EndUploads() {
  State& s = *state_;
  if (!s.recording) {
    return VK_NULL_HANDLE;
  }
  s.recording = false;
  VkCommandBuffer commands = s.frames[s.recording_slot].uploads;
  VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INDEX_READ_BIT |
                          VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_VERTEX_INPUT_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                           0, 1, &barrier, 0, nullptr, 0, nullptr);
  vk::vkEndCommandBuffer(commands);
  return commands;
}

std::string VertexArena::TakeStats() {
  State& s = *state_;
  State::Stats st = s.stats;
  s.stats = {};
  double frames = std::max<uint32_t>(st.frames, 1);
  return std::format(
      "vertex arena: {:.1f} of {} MB in {} ranges; a frame {:.0f} placed, {:.1f} copied ({:.2f} "
      "MB, {:.1f} again after a write), {:.2f} MB inline; memcpy's writes {:.1f} ({:.1f} marking "
      "a range); {} given back; {} left out (full), {} inline left out",
      s.used_bytes / 1048576.0, kRangeBytes >> 20, s.ranges.size(), st.placed / frames,
      st.copied / frames, st.copied_bytes / frames / 1048576.0, st.recopied / frames,
      st.inline_bytes / frames / 1048576.0, st.writes / frames, st.writes_hit / frames, st.freed,
      st.full, st.inline_full);
}

}  // namespace native
