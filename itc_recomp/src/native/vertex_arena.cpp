// See vertex_arena.h.

#include "vertex_arena.h"
#include "gpu/vulkan/backend.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <mutex>
#include <sys/mman.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "base/log.h"
#include "buffers.h"
#include "gpu_memory.h"
#include "guest.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/model_load.h"
#include "schema/models.h"
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

// The GPU's address of the CPU's (as the graphics library's Physical).
uint32_t GpuAddress(uint32_t address) {
  return ((((address >> 20) & 0xFFF) + 512) & 0x1000) + (address & 0x1FFFFFFF);
}

// The held models' events, in order (VertexArena_GpuOnlyModel, a block
// freed), for the render thread's next frame start.
struct HeldBufferInput {
  uint32_t object, address, size, endian;
};
struct ModelEvent {
  bool freed = false;
  // The data's physical allocation (the CPU's address: the memory block's
  // header page, then the data) and the data's size.
  uint32_t block = 0, data_size = 0;
  std::vector<HeldBufferInput> buffers;
};
std::mutex g_model_mutex;
std::vector<ModelEvent> g_model_events;
// The blocks given to VertexArena_GpuOnlyModel and not freed since: only
// their frees are queued.
std::unordered_set<uint32_t> g_held_blocks;

void OnPhysicalFreed(uint32_t address, uint32_t size) {
  std::lock_guard lock(g_model_mutex);
  if (g_held_blocks.erase(address)) {
    g_model_events.push_back({true, address, size, {}});
  }
}

}  // namespace

void VertexArena_GpuOnlyModel(uint32_t model) {
  const schema::Model m(model);
  const uint32_t data = m.vertex_data();
  if (!data) {
    return;
  }
  // The memory block's header page is in front of its data (models'
  // memory_blocks.h).
  const uint32_t block = data - models::kBlockHeaderBytes;
  const uint32_t allocated = memory::PhysicalSize(block);
  if (allocated <= models::kBlockHeaderBytes) {
    return;
  }
  ModelEvent event;
  event.block = block;
  event.data_size = allocated - models::kBlockHeaderBytes;
  for (const models::ModelBuffer& b : models::ModelBuffers(model)) {
    HeldBufferInput in{b.object, 0, 0, 0};
    const uint32_t buffer = guest::LoadU32(b.object + 24);
    if (b.index) {
      // +0 bits 29-30 the endian, +28 the size in bytes.
      in.address = GpuAddress(buffer);
      in.size = guest::LoadU32(b.object + 28);
      in.endian = guest::LoadU32(b.object) >> 29 & 3;
    } else {
      // +24 the address with the type in its low bits, +28 the size in
      // words from bit 2 with the endian in the low bits (the fetch
      // constant's words).
      const uint32_t word1 = guest::LoadU32(b.object + 28);
      in.address = GpuAddress(buffer & ~3u);
      in.size = (word1 >> 2 & 0xFFFFFF) * 4;
      in.endian = word1 & 3;
    }
    if (in.size) {
      event.buffers.push_back(in);
    }
  }
  std::lock_guard lock(g_model_mutex);
  g_held_blocks.insert(block);
  g_model_events.push_back(std::move(event));
}

struct VertexArena::State {
  gpu::vulkan::Backend* backend = nullptr;
  DeviceContext context{};
  uint32_t frames_in_flight = 0;
  uint32_t frame = 0;
  uint64_t frame_number = 0;

  gpu::Buffer* object = nullptr;
  VkBuffer buffer = VK_NULL_HANDLE;  // the object's
  uint32_t inline_start = 0, range_start = 0, size = 0;
  uint32_t inline_used = 0;

  struct StagingChunk {
    gpu::Buffer* object = nullptr;
    VkBuffer buffer = VK_NULL_HANDLE;  // the object's
    uint8_t* mapped = nullptr;
    uint32_t size = 0;
  };
  struct Frame {
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

  // Buffers held by their objects (VertexArena_GpuOnlyModel), each its own
  // place, and the objects of each data block (the CPU's address).
  struct Held {
    uint32_t offset, address, size, endian, block;
  };
  std::unordered_map<uint32_t, Held> held;
  struct HeldBlock {
    std::vector<uint32_t> objects;
    // Its places (offset, size): buffers of the same range share one.
    std::vector<std::pair<uint32_t, uint32_t>> places;
  };
  std::unordered_map<uint32_t, HeldBlock> held_blocks;
  // The held blocks by the GPU's address: start -> end, to find writes into
  // them (none expected: their data is given back).
  std::map<uint32_t, uint32_t> held_ranges;
  bool logged_held_write = false;
  uint64_t held_bytes = 0, given_back_bytes = 0;
  void ProcessModels(const std::vector<ModelEvent>& events);
  void Retire(uint32_t block);

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
  }
  if (s.object) {
    s.backend->DestroyBuffer(s.object);
    gpu_memory::Freed(gpu_memory::Kind::kMirror, s.size);
  }
}

bool VertexArena::State::CreateStaging(StagingChunk& chunk, uint32_t bytes) {
  chunk.object = backend->CreateBuffer({bytes, gpu::kBufferCopySource, true});
  if (!chunk.object) {
    return false;
  }
  chunk.buffer = gpu::vulkan::Backend::Handle(chunk.object);
  chunk.mapped = backend->Mapped(chunk.object);
  chunk.size = bytes;
  gpu_memory::Allocated(gpu_memory::Kind::kStaging, backend->MemorySize(chunk.object));
  return true;
}

void VertexArena::State::DestroyStaging(StagingChunk& chunk) {
  if (chunk.object) {
    backend->DestroyBuffer(chunk.object);
    gpu_memory::Freed(gpu_memory::Kind::kStaging, chunk.size);
  }
  chunk = {};
}

bool VertexArena::Init(gpu::vulkan::Backend& backend, uint32_t frames_in_flight) {
  State& s = *state_;
  s.backend = &backend;
  const DeviceContext& context = backend.context();
  s.context = context;
  s.frames_in_flight = frames_in_flight;
  VkDevice device = context.device;

  s.inline_start = AlignUp(kQuadIndices + 4 * 6 * kBuiltinPrimitives);
  s.range_start = s.inline_start + frames_in_flight * kInlineSlotBytes;
  s.size = s.range_start + kRangeBytes;
  s.object = backend.CreateBuffer(
      {s.size, gpu::kBufferIndex | gpu::kBufferVertex | gpu::kBufferCopyDest, false});
  if (!s.object) {
    ITC_LOG_ERROR("native renderer: the vertex arena ({} MB, device-local) couldn't be made",
                  s.size >> 20);
    return false;
  }
  s.buffer = gpu::vulkan::Backend::Handle(s.object);
  gpu_memory::Allocated(gpu_memory::Kind::kMirror, s.size);
  s.free_space.emplace(s.range_start, kRangeBytes);

  s.frames.resize(frames_in_flight);
  for (State::Frame& f : s.frames) {
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
  memory::SetPhysicalFreeListener(&OnPhysicalFreed);
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
  // The held models' loads and frees since then (a write into a block freed
  // since is a new owner's).
  std::vector<ModelEvent> events;
  {
    std::lock_guard lock(g_model_mutex);
    events.swap(g_model_events);
  }
  std::unordered_set<uint32_t> freed;
  for (const ModelEvent& e : events) {
    if (e.freed) {
      freed.insert(GpuAddress(e.block + models::kBlockHeaderBytes));
    }
  }
  for (auto [start, end] : s.writes) {
    // Into a held model's data (given back: the write would be lost).
    if (auto h = s.held_ranges.upper_bound(start);
        h != s.held_ranges.begin() && std::prev(h)->second > start &&
        !freed.count(std::prev(h)->first) && !s.logged_held_write) {
      s.logged_held_write = true;
      ITC_LOG_ERROR("native renderer: memcpy wrote {:08X}-{:08X}, into a model's vertex data "
                    "held by the GPU alone (its block {:08X})",
                    start, end, std::prev(h)->first);
    }
  }
  s.ProcessModels(events);
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

void VertexArena::State::Retire(uint32_t block) {
  auto it = held_blocks.find(block);
  if (it == held_blocks.end()) {
    return;
  }
  for (uint32_t object : it->second.objects) {
    auto h = held.find(object);
    if (h != held.end() && h->second.block == block) {
      held.erase(h);
    }
  }
  for (auto [offset, size] : it->second.places) {
    // Freed when this slot's fence has passed again (frames in flight).
    frames[frame].to_free.emplace_back(offset, size);
    held_bytes -= size;
  }
  held_blocks.erase(it);
  held_ranges.erase(GpuAddress(block + models::kBlockHeaderBytes));
}

void VertexArena::State::ProcessModels(const std::vector<ModelEvent>& events) {
  for (const ModelEvent& e : events) {
    Retire(e.block);
    if (e.freed) {
      continue;
    }
    // Each range copied into a place of its own (buffers of the same range
    // share it); if one doesn't fit, none is held (the draws place them by
    // address, the data kept).
    HeldBlock& block = held_blocks[e.block];
    std::map<uint64_t, uint32_t> placed;  // address << 32 | size, endian in bits 30-31
    bool all = true;
    for (const HeldBufferInput& b : e.buffers) {
      const uint64_t key = uint64_t(b.address | b.endian << 30) << 32 | b.size;
      uint32_t offset;
      if (auto p = placed.find(key); p != placed.end()) {
        offset = p->second;
      } else {
        offset = Allocate(b.size);
        if (offset == UINT32_MAX) {
          GiveBack(frames_in_flight + 1);
          offset = Allocate(b.size);
        }
        if (offset == UINT32_MAX) {
          all = false;
          break;
        }
        Copy(PhysicalHostAddress(b.address), b.size, offset, b.endian);
        placed.emplace(key, offset);
        block.places.emplace_back(offset, b.size);
        held_bytes += b.size;
      }
      if (auto old = held.find(b.object); old != held.end() && old->second.block != e.block) {
        // An object of another block (its memory reused): that block's
        // places stay until it's freed.
        held.erase(old);
      }
      held[b.object] = {offset, b.address, b.size, b.endian, e.block};
      block.objects.push_back(b.object);
    }
    if (!all) {
      Retire(e.block);
      if (!logged_full) {
        logged_full = true;
        ITC_LOG_ERROR("native renderer: the vertex arena is full ({} MB used): a model's "
                      "buffers left in physical memory",
                      used_bytes >> 20);
      }
      continue;
    }
    // Copied (into staging, read from here on by the GPU only): the data's
    // whole pages back to the system (not the block's header page).
    const uint32_t data = GpuAddress(e.block + models::kBlockHeaderBytes);
    held_ranges[data] = data + e.data_size;
    uint8_t* host = PhysicalHostAddress(data);
    uintptr_t first = (reinterpret_cast<uintptr_t>(host) + 0xFFF) & ~uintptr_t(0xFFF);
    uintptr_t end = (reinterpret_cast<uintptr_t>(host) + e.data_size) & ~uintptr_t(0xFFF);
    if (end > first) {
      madvise(reinterpret_cast<void*>(first), end - first, MADV_DONTNEED);
      given_back_bytes += end - first;
    }
  }
}

uint32_t VertexArena::HeldBuffer(uint32_t object, uint32_t address, uint32_t endian) const {
  const State& s = *state_;
  auto it = s.held.find(object);
  if (it == s.held.end()) {
    return UINT32_MAX;
  }
  const State::Held& h = it->second;
  if (address < h.address || address - h.address >= h.size || endian != h.endian) {
    return UINT32_MAX;
  }
  return h.offset + (address - h.address);
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
  // (The backend's upload commands hold the copies; this slot's staging
  // stays in use until they're submitted.)
  recording = true;
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
  backend->UploadBuffer(chunk->object, object, {at, arena_offset, bytes});
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

uint32_t VertexArena::PlaceInline(const void* data, uint32_t size, uint32_t endian) {
  State& s = *state_;
  uint32_t at = AlignUp(s.inline_used);
  if (at + size > kInlineSlotBytes) {
    s.stats.inline_full++;
    return UINT32_MAX;
  }
  s.inline_used = at + size;
  uint32_t offset = s.inline_start + s.frame * kInlineSlotBytes + at;
  s.Copy(data, size, offset, endian);
  s.stats.inline_bytes += size;
  return offset;
}

VkBuffer VertexArena::buffer() const {
  return state_->buffer;
}

void VertexArena::EndUploads() {
  state_->recording = false;
}

std::string VertexArena::TakeStats() {
  State& s = *state_;
  State::Stats st = s.stats;
  s.stats = {};
  double frames = std::max<uint32_t>(st.frames, 1);
  return std::format(
      "vertex arena: {:.1f} of {} MB in {} ranges; a frame {:.0f} placed, {:.1f} copied ({:.2f} "
      "MB, {:.1f} again after a write), {:.2f} MB inline; memcpy's writes {:.1f} ({:.1f} marking "
      "a range); {} given back; {} left out (full), {} inline left out; {} buffers held by "
      "object ({:.1f} MB, {} models), {:.1f} MB of their data given back so far",
      s.used_bytes / 1048576.0, kRangeBytes >> 20, s.ranges.size(), st.placed / frames,
      st.copied / frames, st.copied_bytes / frames / 1048576.0, st.recopied / frames,
      st.inline_bytes / frames / 1048576.0, st.writes / frames, st.writes_hit / frames, st.freed,
      st.full, st.inline_full, s.held.size(), s.held_bytes / 1048576.0, s.held_blocks.size(),
      s.given_back_bytes / 1048576.0);
}

}  // namespace native
