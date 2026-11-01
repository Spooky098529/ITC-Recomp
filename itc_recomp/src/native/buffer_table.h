// The renderer's buffers by handle (docs/conventional-renderer.md, "Step
// 2b"): filled only by the interface's BufferCreated and BufferDestroyed
// (render/resources.h), on the render thread between lists. Each buffer has
// its own space in the buffer arena (buffer_arena.h), and its data is
// copied from guest memory when a draw first uses it after it was made or
// written (BufferWritten, or memcpy's records of writes into its range:
// MarkWritten). A buffer a draw binds that was never made through the
// interface is logged once with its header and taken as made (a creation
// path missed).

#pragma once

#include <cstdint>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

#include "buffer_arena.h"
#include "render/resources.h"

namespace native {

class BufferTable {
 public:
  void set_base(uint8_t* base) { base_ = base; }
  // The arena buffers get space in.
  void set_arena(BufferArena* arena) { arena_ = arena; }
  void OnFrame() { frame_++; }

  void Created(uint32_t handle, const render::BufferData& data);
  void Written(uint32_t handle);
  void Destroyed(uint32_t handle);
  // A draw uses `handle` in `slot` (a stream, or kSlots - 1 the indices):
  // its offset in the arena, its data copied there first if made or written
  // since. False when it has no space or this frame's staging is full (the
  // draw is left out).
  bool Use(uint32_t slot, uint32_t handle, uint32_t& offset);
  // memcpy's writes into the physical views ([start, end) each): the
  // buffers they touch are written.
  void MarkWritten(const std::vector<std::pair<uint32_t, uint32_t>>& writes);

  struct Stats {
    // Since the last TakeStats: made (again: the handle held already),
    // written, freed (of which never made), and the work memory's made and
    // their bytes. Now: held and their bytes, by kind; bound never made.
    uint64_t created = 0, remade = 0, written = 0, destroyed = 0, destroyed_unknown = 0;
    uint64_t work_created = 0, work_bytes = 0;
    uint64_t held = 0, held_vertex_bytes = 0, held_index_bytes = 0, held_other_bytes = 0;
    uint64_t never_created = 0;
    // Buffers marked written by memcpy's records, and copies made at draws
    // (and their bytes).
    uint64_t marked = 0, uploads = 0, upload_bytes = 0;
  };
  Stats TakeStats();

 private:
  struct Record {
    render::BufferData data;
    uint32_t physical = 0;  // the data's physical address
    uint32_t offset = 0;    // in the arena, with space
    bool space = false, dirty = true;
  };
  void Hold(const render::BufferData& data, int64_t sign);
  Record& NeverMade(uint32_t slot, uint32_t handle);
  void Place(uint32_t handle, Record& record);
  void Unplace(uint32_t handle, Record& record);

  uint8_t* base_ = nullptr;
  BufferArena* arena_ = nullptr;
  // The records by their data's physical address, and the longest, to find
  // those a write touches.
  std::multimap<uint32_t, uint32_t> by_physical_;
  uint32_t longest_ = 0;
  uint64_t frame_ = 0;
  std::unordered_map<uint32_t, Record> records_;
  // The frame each handle was last freed in, for the never-made warning.
  std::unordered_map<uint32_t, uint64_t> freed_at_;
  static constexpr uint32_t kSlots = 17;  // 16 streams, the indices
  Stats stats_;
};

}  // namespace native
