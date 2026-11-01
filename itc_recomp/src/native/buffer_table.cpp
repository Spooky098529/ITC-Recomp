#include "native/buffer_table.h"

#include <algorithm>
#include <string>

#include "base/hash.h"
#include "base/log.h"
#include "guest.h"
#include "state.h"

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

// Space in the arena; the data to copy at the next use.
void BufferTable::Place(Record& record) {
  record.physical = GpuPhysicalAddress(record.data.address);
  record.dirty = true;
  record.reported = false;
  record.space = arena_->Allocate(record.data.size, record.offset);
}

void BufferTable::Unplace(Record& record) {
  if (record.space) {
    arena_->Free(record.offset, record.data.size);
    record.space = false;
  }
}

uint64_t BufferTable::Hash(const Record& record) const {
  return base::Hash64(PhysicalHostAddress(base_, record.physical), record.data.size, 0);
}

// With verify, once a frame: the buffer's memory as when it was copied, or
// it changed with no BufferWritten (a writer missed; drawn as copied).
void BufferTable::Verify(uint32_t handle, Record& record) {
  if (record.checked_frame == frame_ || record.reported) {
    return;
  }
  record.checked_frame = frame_;
  stats_.verified++;
  if (Hash(record) == record.hash) {
    return;
  }
  record.reported = true;
  stats_.unannounced++;
  if (unannounced_lines_) {
    unannounced_lines_--;
    ITC_LOG_WARN("native renderer: buffer {:08X} ({} bytes at {:08X}) changed in memory with no "
                 "BufferWritten, {} frames after it was copied (logged once a buffer; a "
                 "debugging aid)",
                 handle, record.data.size, record.data.address, frame_ - record.uploaded_frame);
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
    Unplace(it->second);
  }
  it->second.data = data;
  Hold(data, 1);
  Place(it->second);
}

void BufferTable::Written(uint32_t handle) {
  stats_.written++;
  if (auto it = records_.find(handle); it != records_.end()) {
    it->second.dirty = true;
  }
}

void BufferTable::Destroyed(uint32_t handle) {
  stats_.destroyed++;
  auto it = records_.find(handle);
  if (it == records_.end()) {
    stats_.destroyed_unknown++;
    return;
  }
  Hold(it->second.data, -1);
  Unplace(it->second);
  records_.erase(it);
  freed_at_[handle] = frame_;
}

// A buffer bound that was never made through the interface: logged once
// and taken as made.
BufferTable::Record& BufferTable::NeverMade(uint32_t slot, uint32_t handle) {
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
    const uint32_t word = guest::LoadU32(base_, handle + 28);
    if (type == 1) {
      data = {render::BufferKind::kVertex, guest::LoadU32(base_, handle + 24) & ~3u,
              ((word >> 2) & 0xFFFFFF) * 4};
    } else {
      data = {type == 2 ? render::BufferKind::kIndex : render::BufferKind::kOther,
              guest::LoadU32(base_, handle + 24), word};
    }
  }
  Record& record = records_[handle];
  record.data = data;
  Hold(data, 1);
  Place(record);
  return record;
}

bool BufferTable::Use(uint32_t slot, uint32_t handle, uint32_t& offset) {
  auto it = records_.find(handle);
  Record& record = it != records_.end() ? it->second : NeverMade(slot, handle);
  if (!record.space) {
    return false;
  }
  if (record.dirty) {
    const uint32_t size = record.data.size;
    if (uint64_t(record.physical) + size > kPhysicalMemorySize ||
        !arena_->Upload(record.offset, PhysicalHostAddress(base_, record.physical), size)) {
      return false;
    }
    record.dirty = false;
    stats_.uploads++;
    stats_.upload_bytes += size;
    if (verify_) {
      record.hash = Hash(record);
      record.uploaded_frame = record.checked_frame = frame_;
    }
  } else if (verify_) {
    Verify(handle, record);
  }
  offset = record.offset;
  return true;
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
