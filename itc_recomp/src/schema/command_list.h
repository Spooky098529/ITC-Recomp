// The views of docs/functions/command-list.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/command-list.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CommandList, sub_8258CE00.r3, [82970BD8]
struct CommandList : View {
  using View::View;
  static constexpr uint32_t kSize = 53;
  static constexpr uint32_t kVtable = 0x8206C080;
  // 0x8206C080
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the buffer being recorded, 0 or 1 (flipped each tick)
  uint32_t buffer() const { return guest::LoadU32(base, address + 4); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t buffer_address() const { return address + 4; }
  // each buffer's capacity in slots
  uint32_t capacity() const { return guest::LoadU32(base, address + 8); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t capacity_address() const { return address + 8; }
  // the slots used this tick
  uint32_t used() const { return guest::LoadU32(base, address + 12); }
  void set_used(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t used_address() const { return address + 12; }
  // the slots the last tick used
  uint32_t last_used() const { return guest::LoadU32(base, address + 16); }
  void set_last_used(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t last_used_address() const { return address + 16; }
  // both buffers' block, freed with operator delete[] by the destructor
  uint32_t allocation() const { return guest::LoadU32(base, address + 20); }
  void set_allocation(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t allocation_address() const { return address + 20; }
  // the first buffer; +24 + 4 * buffer selects
  uint32_t buffer0() const { return guest::LoadU32(base, address + 24); }
  void set_buffer0(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t buffer0_address() const { return address + 24; }
  // the second buffer
  uint32_t buffer1() const { return guest::LoadU32(base, address + 28); }
  void set_buffer1(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t buffer1_address() const { return address + 28; }
  // where the next chunk starts (a slot address)
  uint32_t next_chunk() const { return guest::LoadU32(base, address + 32); }
  void set_next_chunk(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t next_chunk_address() const { return address + 32; }
  // the last chunk's start (a slot address)
  uint32_t chunk_start() const { return guest::LoadU32(base, address + 36); }
  void set_chunk_start(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t chunk_start_address() const { return address + 36; }
  // the last chunk's end (a slot address)
  uint32_t chunk_end() const { return guest::LoadU32(base, address + 40); }
  void set_chunk_end(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t chunk_end_address() const { return address + 40; }
  // the write pointer (a slot address)
  uint32_t write() const { return guest::LoadU32(base, address + 44); }
  void set_write(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t write_address() const { return address + 44; }
  // the open chunk's start again (no reader found)
  uint32_t open_start() const { return guest::LoadU32(base, address + 48); }
  void set_open_start(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t open_start_address() const { return address + 48; }
  // 1 while a chunk is open
  uint8_t chunk_open() const { return guest::LoadU8(base, address + 52); }
  void set_chunk_open(uint8_t v) const { guest::StoreU8(base, address + 52, v); }
  uint32_t chunk_open_address() const { return address + 52; }
};

// The image words docs/functions/command-list.md's code uses.
struct CommandListGlobals {
  // the count of chunks open (all lists); a debug message when one opens with another open
  static uint32_t open_chunks(uint8_t* base) { return guest::LoadU32(base, 0x8296BD28u); }
  static void set_open_chunks(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BD28u, v); }
  static constexpr uint32_t open_chunks_address() { return 0x8296BD28u; }
};

}  // namespace schema
