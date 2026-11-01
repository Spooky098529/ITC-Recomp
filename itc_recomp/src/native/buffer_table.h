// The renderer's buffers by handle (docs/conventional-renderer.md, "Step
// 2b", part 1): filled only by the interface's BufferCreated and
// BufferDestroyed (render/resources.h), on the render thread between lists.
// Draws still read the guest memory mirrors; this keeps the records part 2
// uploads from, and counts what's held. A buffer a draw binds that was
// never made through the interface is logged once with its header and
// taken as made (a creation path missed).

#pragma once

#include <cstdint>
#include <unordered_map>

#include "render/resources.h"

namespace native {

class BufferTable {
 public:
  void set_base(uint8_t* base) { base_ = base; }
  void OnFrame() { frame_++; }

  void Created(uint32_t handle, const render::BufferData& data);
  void Written(uint32_t handle);
  void Destroyed(uint32_t handle);
  // A draw binds `handle` (a stream's or the indices'). Cheap when it was
  // the last one checked in `slot`.
  void Bound(uint32_t slot, uint32_t handle);

  struct Stats {
    // Since the last TakeStats: made (again: the handle held already),
    // written, freed (of which never made), and the work memory's made and
    // their bytes. Now: held and their bytes, by kind; bound never made.
    uint64_t created = 0, remade = 0, written = 0, destroyed = 0, destroyed_unknown = 0;
    uint64_t work_created = 0, work_bytes = 0;
    uint64_t held = 0, held_vertex_bytes = 0, held_index_bytes = 0, held_other_bytes = 0;
    uint64_t never_created = 0;
  };
  Stats TakeStats();

 private:
  struct Record {
    render::BufferData data;
  };
  void Hold(const render::BufferData& data, int64_t sign);

  uint8_t* base_ = nullptr;
  uint64_t frame_ = 0;
  std::unordered_map<uint32_t, Record> records_;
  // The frame each handle was last freed in, for the never-made warning.
  std::unordered_map<uint32_t, uint64_t> freed_at_;
  static constexpr uint32_t kSlots = 17;  // 16 streams, the indices
  uint32_t last_bound_[kSlots] = {};
  Stats stats_;
};

}  // namespace native
