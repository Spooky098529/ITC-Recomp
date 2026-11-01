#include "native/buffer_table.h"

#include <string>

#include "base/log.h"
#include "guest.h"

namespace native {

namespace {

// The work memory's vertex buffer headers (WorkMem::AllocVertexBuffer,
// sub_8258A9E8; docs/functions/car-draw.md): 1,280 of 32 bytes for each of
// two buffers, set again every frame.
constexpr uint32_t kWorkHeaders = 0x82957780;
constexpr uint32_t kWorkHeadersEnd = kWorkHeaders + 2 * 1280 * 32;

}  // namespace

void BufferTable::Hold(const render::BufferData& data, int64_t sign) {
  stats_.held += sign;
  switch (data.kind) {
    case render::BufferKind::kVertex:
      stats_.held_vertex_bytes += sign * int64_t(data.size);
      break;
    case render::BufferKind::kIndex:
      stats_.held_index_bytes += sign * int64_t(data.size);
      break;
    case render::BufferKind::kOther:
      stats_.held_other_bytes += sign * int64_t(data.size);
      break;
  }
}

void BufferTable::Created(uint32_t handle, const render::BufferData& data) {
  stats_.created++;
  if (handle >= kWorkHeaders && handle < kWorkHeadersEnd) {
    stats_.work_created++;
    stats_.work_bytes += data.size;
  }
  auto [it, added] = records_.try_emplace(handle);
  if (!added) {
    stats_.remade++;
    Hold(it->second.data, -1);
  }
  it->second.data = data;
  Hold(data, 1);
}

void BufferTable::Written(uint32_t handle) {
  stats_.written++;
}

void BufferTable::Destroyed(uint32_t handle) {
  stats_.destroyed++;
  auto it = records_.find(handle);
  if (it == records_.end()) {
    stats_.destroyed_unknown++;
    return;
  }
  Hold(it->second.data, -1);
  records_.erase(it);
  freed_at_[handle] = frame_;
  for (uint32_t& last : last_bound_) {
    if (last == handle) {
      last = 0;
    }
  }
}

void BufferTable::Bound(uint32_t slot, uint32_t handle) {
  if (!handle || last_bound_[slot] == handle) {
    return;
  }
  last_bound_[slot] = handle;
  if (records_.count(handle)) {
    return;
  }
  stats_.never_created++;
  std::string words;
  for (uint32_t i = 0; base_ && i < 8; i++) {
    words += fmt::format(" {:08X}", guest::LoadU32(base_, handle + 4 * i));
  }
  auto freed = freed_at_.find(handle);
  ITC_LOG_WARN("native renderer: buffer {:08X} bound ({}) but never made through the "
               "interface{}; taken as made; header:{}",
               handle, slot == kSlots - 1 ? std::string("indices") : fmt::format("stream {}", slot),
               freed == freed_at_.end()
                   ? std::string()
                   : fmt::format(" (freed {} frames before)", frame_ - freed->second),
               words);
  render::BufferData data;
  if (base_) {
    const uint32_t type = guest::LoadU32(base_, handle) & 0xF;
    data.kind = type == 1   ? render::BufferKind::kVertex
                : type == 2 ? render::BufferKind::kIndex
                            : render::BufferKind::kOther;
  }
  records_[handle].data = data;
  Hold(data, 1);
}

BufferTable::Stats BufferTable::TakeStats() {
  Stats stats = stats_;
  stats_ = {};
  stats_.held = stats.held;
  stats_.held_vertex_bytes = stats.held_vertex_bytes;
  stats_.held_index_bytes = stats.held_index_bytes;
  stats_.held_other_bytes = stats.held_other_bytes;
  return stats;
}

}  // namespace native
