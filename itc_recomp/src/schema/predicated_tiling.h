// The views of docs/functions/predicated-tiling.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/predicated-tiling.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: Device.+0
struct TilingWorkerState : View {
  using View::View;
  static constexpr uint32_t kSize = 400;
  // the job's spin lock (lwarx/stwcx.; 1 held)
  uint32_t job_lock() const { return guest::LoadU32(base, address + 0); }
  void set_job_lock(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t job_lock_address() const { return address + 0; }
  // the primary ring (device +13496); 0 tells the workers to exit
  uint32_t ring() const { return guest::LoadU32(base, address + 4); }
  void set_ring(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ring_address() const { return address + 4; }
  // the primary ring's write position
  uint32_t ring_write() const { return guest::LoadU32(base, address + 8); }
  void set_ring_write(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ring_write_address() const { return address + 8; }
  // the ring's device +13500
  uint32_t ring_mask() const { return guest::LoadU32(base, address + 12); }
  void set_ring_mask(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t ring_mask_address() const { return address + 12; }
  // the walk's function, called for each of a plain entry's words (set by a 0x80000000 entry)
  uint32_t entry_function() const { return guest::LoadU32(base, address + 16); }
  void set_entry_function(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t entry_function_address() const { return address + 16; }
  // its first argument (set with it)
  uint32_t entry_argument() const { return guest::LoadU32(base, address + 20); }
  void set_entry_argument(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t entry_argument_address() const { return address + 20; }
  // the plain entry's word being called (counts up to +28)
  uint32_t entry_index() const { return guest::LoadU32(base, address + 24); }
  void set_entry_index(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t entry_index_address() const { return address + 24; }
  // the plain entry's word count (its low half)
  uint32_t entry_count() const { return guest::LoadU32(base, address + 28); }
  void set_entry_count(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t entry_count_address() const { return address + 28; }
  // the plain entry's words (after its count)
  template <typename T = TilingChainWords>
  T entry_data() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_entry_data(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_entry_data(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t entry_data_address() const { return address + 32; }
  // the walk's position (the next entry); kept by WorkerNextBlock (sub_822B60C8) when it waits
  template <typename T = TilingChainWords>
  T position() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_position(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t position_address() const { return address + 36; }
  // probably the markers' nesting: its top bits pick each tile's select bits (1 or 2, WriteBinSelect sub_822B6370)
  uint32_t select_mode() const { return guest::LoadU32(base, address + 40); }
  void set_select_mode(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t select_mode_address() const { return address + 40; }
  // the current tile (WorkerMarkers sub_822B6528, WriteBinSelect); the job masks the draws (sub_822B6288) when it's 1
  uint32_t tile() const { return guest::LoadU32(base, address + 44); }
  void set_tile(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t tile_address() const { return address + 44; }
  // the job's depth: a wake while it runs (above 1) continues the saved walk
  uint32_t job_depth() const { return guest::LoadU32(base, address + 48); }
  void set_job_depth(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t job_depth_address() const { return address + 48; }
  // 1 when a job takes a header; WorkerNextBlock clears it at the chain's end with the depth at 0
  uint32_t job_running() const { return guest::LoadU32(base, address + 52); }
  void set_job_running(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t job_running_address() const { return address + 52; }
  // kicks deferred until the GPU's interrupts (device +10868; always 0 in play)
  uint32_t pending_kicks() const { return guest::LoadU32(base, address + 56); }
  void set_pending_kicks(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t pending_kicks_address() const { return address + 56; }
  // a spin lock (KfAcquireSpinLock, WorkerNextBlock)
  uint32_t next_block_lock() const { return guest::LoadU32(base, address + 60); }
  void set_next_block_lock(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t next_block_lock_address() const { return address + 60; }
  // an event (KEVENT) WorkerNextBlock waits on for the chain's next block
  uint32_t next_block_event_address() const { return address + 64; }
  // the callback's argument (Device::WakeWorker sub_822B6A78): the segment chain's first block, the tiling header at its +4; cleared when the job takes it
  template <typename T = TilingChainWords>
  T pending_header() const { return T(base, guest::LoadU32(base, address + 80)); }
  void set_pending_header(const View& v) const { guest::StoreU32(base, address + 80, v.address); }
  void set_pending_header(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t pending_header_address() const { return address + 80; }
  // the walk's block: the header's, then each next block (WorkerNextBlock's return)
  template <typename T = TilingChainWords>
  T walk_block() const { return T(base, guest::LoadU32(base, address + 84)); }
  void set_walk_block(const View& v) const { guest::StoreU32(base, address + 84, v.address); }
  void set_walk_block(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t walk_block_address() const { return address + 84; }
  // 0xC0000000
  uint32_t end_marker() const { return guest::LoadU32(base, address + 92); }
  void set_end_marker(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t end_marker_address() const { return address + 92; }
  // 1 while a nested wake hands its walk to the running job, which spins until the markers' walk clears it
  uint32_t walk_handoff() const { return guest::LoadU32(base, address + 104); }
  void set_walk_handoff(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t walk_handoff_address() const { return address + 104; }
  // the L2-locked area (0x7F100000)
  uint32_t l2_area() const { return guest::LoadU32(base, address + 108); }
  void set_l2_area(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t l2_area_address() const { return address + 108; }
  // the tiling header's copy (the draw chain's and third chain's first blocks, the flags, +124 the tile count, the tiles)
  uint32_t header_copy_address() const { return address + 112; }
  // the last bin select written (0x7FFFFFFF reset by the job)
  uint32_t bin_select() const { return guest::LoadU32(base, address + 368); }
  void set_bin_select(uint32_t v) const { guest::StoreU32(base, address + 368, v); }
  uint32_t bin_select_address() const { return address + 368; }
  // probably the 0x82-0x84 markers' saved positions (not read in detail)
  uint32_t nest_positions_address() const { return address + 372; }
  // the 0x86000000 marker's position (the tiles' replay goes back there)
  uint32_t tiling_start() const { return guest::LoadU32(base, address + 388); }
  void set_tiling_start(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t tiling_start_address() const { return address + 388; }
  // kept with it (not read in detail)
  uint32_t tiling_start_word() const { return guest::LoadU32(base, address + 392); }
  void set_tiling_start_word(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t tiling_start_word_address() const { return address + 392; }
  // the CPU of the last worker made (device +11208): its worker waits with a 30 ms timeout
  uint32_t last_worker_cpu() const { return guest::LoadU32(base, address + 396); }
  void set_last_worker_cpu(uint32_t v) const { guest::StoreU32(base, address + 396, v); }
  uint32_t last_worker_cpu_address() const { return address + 396; }
};

// gpu; measure: Device.+0.+36
struct TilingChainWords : View {
  using View::View;
  static constexpr uint32_t kSize = 264;
  // after a 0x80000000 entry: the walk's function; in a header block, the header's first word
  uint32_t next_function() const { return guest::LoadU32(base, address + 4); }
  void set_next_function(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_function_address() const { return address + 4; }
  // after a 0x80000000 entry: the function's argument
  uint32_t next_argument() const { return guest::LoadU32(base, address + 8); }
  void set_next_argument(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_argument_address() const { return address + 8; }
  // in a header block: the first entry after the header
  uint32_t first_entry() const { return guest::LoadU32(base, address + 260); }
  void set_first_entry(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t first_entry_address() const { return address + 260; }
};

// gpu; measure: Device.+12916
struct TilingTileRect : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t left() const { return guest::LoadU32(base, address + 0); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  uint32_t top() const { return guest::LoadU32(base, address + 4); }
  void set_top(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t top_address() const { return address + 4; }
  uint32_t right() const { return guest::LoadU32(base, address + 8); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t right_address() const { return address + 8; }
  uint32_t bottom() const { return guest::LoadU32(base, address + 12); }
  void set_bottom(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t bottom_address() const { return address + 12; }
};

// gpu; measure: sub_822B2598.r4, Device::Swap.r5
struct ScalerParameters : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // Device::Swap's byte read of +0 (its r5, never other than the device's own in play)
  uint8_t source_left_byte() const { return guest::LoadU8(base, address + 0); }
  void set_source_left_byte(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t source_left_byte_address() const { return address + 0; }
  // probably
  uint32_t source_left() const { return guest::LoadU32(base, address + 0); }
  void set_source_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t source_left_address() const { return address + 0; }
  // probably
  uint32_t source_top() const { return guest::LoadU32(base, address + 4); }
  void set_source_top(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t source_top_address() const { return address + 4; }
  // probably
  uint32_t source_right() const { return guest::LoadU32(base, address + 8); }
  void set_source_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t source_right_address() const { return address + 8; }
  // probably
  uint32_t source_bottom() const { return guest::LoadU32(base, address + 12); }
  void set_source_bottom(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t source_bottom_address() const { return address + 12; }
  // probably
  uint32_t scaled_width() const { return guest::LoadU32(base, address + 16); }
  void set_scaled_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t scaled_width_address() const { return address + 16; }
  // probably
  uint32_t scaled_height() const { return guest::LoadU32(base, address + 20); }
  void set_scaled_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t scaled_height_address() const { return address + 20; }
  // probably; not 0: the set-up's +24 and +40 are 3
  uint32_t filter_profile() const { return guest::LoadU32(base, address + 24); }
  void set_filter_profile(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t filter_profile_address() const { return address + 24; }
};

// gpu; measure: sub_822B2598.r5
struct ScalerSetUp : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // the parameters' +0
  uint32_t source_left() const { return guest::LoadU32(base, address + 0); }
  void set_source_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t source_left_address() const { return address + 0; }
  // the parameters' +4
  uint32_t source_top() const { return guest::LoadU32(base, address + 4); }
  void set_source_top(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t source_top_address() const { return address + 4; }
  // the parameters' +8
  uint32_t source_right() const { return guest::LoadU32(base, address + 8); }
  void set_source_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t source_right_address() const { return address + 8; }
  // the parameters' +12
  uint32_t source_bottom() const { return guest::LoadU32(base, address + 12); }
  void set_source_bottom(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t source_bottom_address() const { return address + 12; }
  // the parameters' +16
  uint32_t scaled_width() const { return guest::LoadU32(base, address + 16); }
  void set_scaled_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t scaled_width_address() const { return address + 16; }
  // the parameters' +20
  uint32_t scaled_height() const { return guest::LoadU32(base, address + 20); }
  void set_scaled_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t scaled_height_address() const { return address + 20; }
  // probably: 3 with a filter profile, else 0
  uint32_t horizontal_filter() const { return guest::LoadU32(base, address + 24); }
  void set_horizontal_filter(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t horizontal_filter_address() const { return address + 24; }
  // 0
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // 0
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  // 0
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  // probably: as +24
  uint32_t vertical_filter() const { return guest::LoadU32(base, address + 40); }
  void set_vertical_filter(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t vertical_filter_address() const { return address + 40; }
  // 0
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  // 0
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  // 0
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// gpu; measure: sub_822AF438.r4
struct ResolveClearTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the low 6 bits a GPU format (probably a surface's format word)
  uint32_t format() const { return guest::LoadU32(base, address + 8); }
  void set_format(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t format_address() const { return address + 8; }
};

}  // namespace schema
