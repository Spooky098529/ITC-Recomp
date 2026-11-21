// The views of docs/functions/command-buffer.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/command-buffer.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: CommandChain
struct CommandChain : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the first block
  template <typename T = CommandChainBlock>
  T first() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
  // the current block's first word
  template <typename T = CommandChainBlock>
  T block() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_block(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t block_address() const { return address + 4; }
  // the write cursor (the block + 4 at its start)
  uint32_t cursor() const { return guest::LoadU32(base, address + 8); }
  void set_cursor(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t cursor_address() const { return address + 8; }
  // the block's end (block + 4,220); 0 when closed
  uint32_t end() const { return guest::LoadU32(base, address + 12); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
  // the device (device-state.md's Device)
  uint32_t device() const { return guest::LoadU32(base, address + 16); }
  void set_device(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t device_address() const { return address + 16; }
};

// gpu; measure: CommandChain.+0
struct CommandChainBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0xC0000000 + the physical address where the content ends; the cursor's word gets the end marker 0xC0000000 or the next block's link
  uint32_t content_end() const { return guest::LoadU32(base, address + 0); }
  void set_content_end(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t content_end_address() const { return address + 0; }
};

// gpu; measure: CommandChain.+16
struct CommandChainDevice : View {
  using View::View;
  static constexpr uint32_t kSize = 15180;
  // the 4,800-byte segment writers get when out of space (device +15176)
  uint32_t dummy_segment() const { return guest::LoadU32(base, address + 15176); }
  void set_dummy_segment(uint32_t v) const { guest::StoreU32(base, address + 15176, v); }
  uint32_t dummy_segment_address() const { return address + 15176; }
};

// allocated; measure: new<96>@Device::SetUpRing
struct CommandProgressBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // the last fence the GPU passed (against device +10780)
  uint32_t fence_done() const { return guest::LoadU32(base, address + 0); }
  void set_fence_done(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fence_done_address() const { return address + 0; }
  // the last kicked segment's end | its generation & 3
  uint32_t read_position() const { return guest::LoadU32(base, address + 4); }
  void set_read_position(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t read_position_address() const { return address + 4; }
  // the primary ring's read pointer (sub_8229DCC0)
  uint32_t ring_read_position() const { return guest::LoadU32(base, address + 60); }
  void set_ring_read_position(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t ring_read_position_address() const { return address + 60; }
};

// gpu; measure: Device.+13160, sub_822AD8F8.r3, sub_8229E100.ret
struct CommandRecorder : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  // probably: 0xC0000000 + the physical address of the first block sub_822AD790 reserves
  uint32_t first_block_link() const { return guest::LoadU32(base, address + 112); }
  void set_first_block_link(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t first_block_link_address() const { return address + 112; }
  // its own buffer's start (0: space from the callback +172)
  uint32_t buffer() const { return guest::LoadU32(base, address + 152); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t buffer_address() const { return address + 152; }
  // its buffer's size in bytes
  uint32_t buffer_size() const { return guest::LoadU32(base, address + 156); }
  void set_buffer_size(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t buffer_size_address() const { return address + 156; }
  // the callback's r3
  uint32_t callback_context() const { return guest::LoadU32(base, address + 164); }
  void set_callback_context(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t callback_context_address() const { return address + 164; }
  // the most a callback call gives (the size asked is clamped to it)
  uint32_t max_size() const { return guest::LoadU32(base, address + 168); }
  void set_max_size(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t max_size_address() const { return address + 168; }
  // gives space (context, kind, &size, alignment), under the device's lock +13576
  uint32_t callback() const { return guest::LoadU32(base, address + 172); }
  void set_callback(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t callback_address() const { return address + 172; }
};

// gpu; measure: sub_8229E100.r5, sub_822AD690.r5, sub_822AD8F8.r5
struct CommandSpaceSize : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t size() const { return guest::LoadU32(base, address + 0); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t size_address() const { return address + 0; }
};

// gpu; measure: Device.+19980
struct CommandTap : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = CommandTapVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// gpu; measure: Device.+19980.+0
struct CommandTapVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // (tap, words, count, 0) before the words are written
  uint32_t before_packets() const { return guest::LoadU32(base, address + 24); }
  void set_before_packets(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t before_packets_address() const { return address + 24; }
  // after them
  uint32_t after_packets() const { return guest::LoadU32(base, address + 28); }
  void set_after_packets(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t after_packets_address() const { return address + 28; }
};

// gpu; measure: Device::Kick.r4
struct CommandKickPairs : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t words_0() const { return guest::LoadU32(base, address + 0); }
  void set_words_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t words_0_address() const { return address + 0; }
  uint32_t address_0() const { return guest::LoadU32(base, address + 4); }
  void set_address_0(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t address_0_address() const { return address + 4; }
  uint32_t words_1() const { return guest::LoadU32(base, address + 8); }
  void set_words_1(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t words_1_address() const { return address + 8; }
  uint32_t address_1() const { return guest::LoadU32(base, address + 12); }
  void set_address_1(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t address_1_address() const { return address + 12; }
};

// gpu; measure: Device::WriteCallback.r4
struct CommandWritePosition : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t first_word() const { return guest::LoadU32(base, address + 4); }
  void set_first_word(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_word_address() const { return address + 4; }
};

// gpu; measure: Device::WriteFlushPackets.r4
struct CommandFlushAddress : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the block's physical address (`address` would hide the view's own)
  uint32_t block_address() const { return guest::LoadU32(base, address + 0); }
  void set_block_address(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t block_address_address() const { return address + 0; }
};

// gpu; measure: Device::WriteFlushPackets.r5
struct CommandFlushWords : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t words() const { return guest::LoadU32(base, address + 0); }
  void set_words(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t words_address() const { return address + 0; }
};

}  // namespace schema
