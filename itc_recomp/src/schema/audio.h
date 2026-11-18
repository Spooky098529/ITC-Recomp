// The views of docs/functions/audio.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/audio.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: XmaObject, sub_8245C948.r3, sub_8245CAE0.r3, sub_8245CB38.r3, sub_8245CBB0.r3, sub_8245CD68.r3, sub_8245CE10.r3, sub_8245D050.r3, sub_8245D288.r3
struct XmaObject : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t stream_count() const { return guest::LoadU32(base, address + 0); }
  void set_stream_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t stream_count_address() const { return address + 0; }
  // 0x10000 locked, 0x20000 the copies hold the contexts (written back at the unlock), 0x40000 contexts made; Create sets 0x30000
  uint32_t flags() const { return guest::LoadU32(base, address + 4); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t flags_address() const { return address + 4; }
  // 96 bytes each
  template <typename T = XmaStreamEntry>
  T streams() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_streams(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_streams(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t streams_address() const { return address + 8; }
};

// allocated; measure: XmaObject.+8, sub_8245CAE0.r3.+8, sub_8245CB38.r3.+8, sub_8245CBB0.r3.+8, sub_8245CD68.r3.+8, sub_8245CE10.r3.+8, sub_8245D050.r3.+8
struct XmaStreamEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // context word 0: input buffer 0's packet count, the loop count, the input buffers' valid bits, the output buffer's block count and write offset (probably)
  uint32_t context_buffers() const { return guest::LoadU32(base, address + 0); }
  void set_context_buffers(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t context_buffers_address() const { return address + 0; }
  // context word 1: input buffer 1's packet count, the loop subframes, the output padding (bits 24-26), sample rate, stereo bit, output valid bit
  uint32_t context_format() const { return guest::LoadU32(base, address + 4); }
  void set_context_format(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t context_format_address() const { return address + 4; }
  // context word 2: the input read offset (bits) and the error status (top bits)
  uint32_t context_input_read_offset() const { return guest::LoadU32(base, address + 8); }
  void set_context_input_read_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t context_input_read_offset_address() const { return address + 8; }
  // context word 3: the loop start (bit offset)
  uint32_t context_loop_start() const { return guest::LoadU32(base, address + 12); }
  void set_context_loop_start(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t context_loop_start_address() const { return address + 12; }
  // context word 4: the loop end (bit offset)
  uint32_t context_loop_end() const { return guest::LoadU32(base, address + 16); }
  void set_context_loop_end(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t context_loop_end_address() const { return address + 16; }
  // context word 5: input buffer 0's physical address
  uint32_t context_input_buffer_0() const { return guest::LoadU32(base, address + 20); }
  void set_context_input_buffer_0(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t context_input_buffer_0_address() const { return address + 20; }
  // context word 6: input buffer 1's physical address
  uint32_t context_input_buffer_1() const { return guest::LoadU32(base, address + 24); }
  void set_context_input_buffer_1(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t context_input_buffer_1_address() const { return address + 24; }
  // context word 7: the output buffer's physical address
  uint32_t context_output_buffer() const { return guest::LoadU32(base, address + 28); }
  void set_context_output_buffer(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t context_output_buffer_address() const { return address + 28; }
  // context word 8: the work buffer's physical address
  uint32_t context_work_buffer() const { return guest::LoadU32(base, address + 32); }
  void set_context_work_buffer(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t context_work_buffer_address() const { return address + 32; }
  // context word 9: the output buffer's read offset (blocks)
  uint32_t context_output_read_offset() const { return guest::LoadU32(base, address + 36); }
  void set_context_output_read_offset(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t context_output_read_offset_address() const { return address + 36; }
  // context word 10
  uint32_t context_word_10() const { return guest::LoadU32(base, address + 40); }
  void set_context_word_10(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t context_word_10_address() const { return address + 40; }
  // context word 11
  uint32_t context_word_11() const { return guest::LoadU32(base, address + 44); }
  void set_context_word_11(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t context_word_11_address() const { return address + 44; }
  // XMACreateContext's context (0 when released)
  uint32_t hardware_context() const { return guest::LoadU32(base, address + 64); }
  void set_hardware_context(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t hardware_context_address() const { return address + 64; }
  // the decoded samples' ring
  uint32_t output_buffer() const { return guest::LoadU32(base, address + 68); }
  void set_output_buffer(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t output_buffer_address() const { return address + 68; }
  uint32_t work_buffer() const { return guest::LoadU32(base, address + 72); }
  void set_work_buffer(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t work_buffer_address() const { return address + 72; }
  uint32_t samples_consumed() const { return guest::LoadU32(base, address + 76); }
  void set_samples_consumed(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t samples_consumed_address() const { return address + 76; }
  // (physical address - [0x82954F0C]) / 64
  uint16_t context_id() const { return guest::LoadU16(base, address + 80); }
  void set_context_id(uint16_t v) const { guest::StoreU16(base, address + 80, v); }
  uint32_t context_id_address() const { return address + 80; }
  // the position in the current 256-byte output block
  uint16_t block_position() const { return guest::LoadU16(base, address + 82); }
  void set_block_position(uint16_t v) const { guest::StoreU16(base, address + 82, v); }
  uint32_t block_position_address() const { return address + 82; }
  // input buffer 0's address (virtual)
  uint32_t input_address_0() const { return guest::LoadU32(base, address + 84); }
  void set_input_address_0(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t input_address_0_address() const { return address + 84; }
  // input buffer 1's address (virtual)
  uint32_t input_address_1() const { return guest::LoadU32(base, address + 88); }
  void set_input_address_1(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t input_address_1_address() const { return address + 88; }
};

// embedded; measure: XmaObject::Create.r4
struct XmaStreamDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t sample_rate() const { return guest::LoadU32(base, address + 0); }
  void set_sample_rate(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t sample_rate_address() const { return address + 0; }
  // the output ring's size, in samples
  uint32_t ring_samples() const { return guest::LoadU32(base, address + 4); }
  void set_ring_samples(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ring_samples_address() const { return address + 4; }
  uint8_t channels() const { return guest::LoadU8(base, address + 8); }
  void set_channels(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t channels_address() const { return address + 8; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
};

// embedded; measure: XmaObject::Create.r6
struct XmaObjectOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the XMA object (ours: an opaque handle)
  uint32_t object() const { return guest::LoadU32(base, address + 0); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t object_address() const { return address + 0; }
};

// embedded; measure: XmaObject::SetInputBuffer.r5, sub_8246A5F8.r3, sub_8246A698.r3
struct XmaData : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the first packet's header: frame count (top 6 bits), first frame's bit offset, stream (low 8 bits)
  uint32_t first_packet_header() const { return guest::LoadU32(base, address + 0); }
  void set_first_packet_header(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t first_packet_header_address() const { return address + 0; }
};

// embedded; measure: XmaObject::SetLoopData.r5
struct XmaLoopData : View {
  using View::View;
  static constexpr uint32_t kSize = 11;
  // the loop start's bit offset into the input (26 bits)
  uint32_t start_bit() const { return guest::LoadU32(base, address + 0); }
  void set_start_bit(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t start_bit_address() const { return address + 0; }
  // the loop end's bit offset (26 bits)
  uint32_t end_bit() const { return guest::LoadU32(base, address + 4); }
  void set_end_bit(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t end_bit_address() const { return address + 4; }
  // subframes played in the last frame - 1 (2 bits)
  uint8_t end_subframe() const { return guest::LoadU8(base, address + 8); }
  void set_end_subframe(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t end_subframe_address() const { return address + 8; }
  // subframes skipped at the start (3 bits)
  uint8_t skip_subframes() const { return guest::LoadU8(base, address + 9); }
  void set_skip_subframes(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t skip_subframes_address() const { return address + 9; }
  // 255 forever
  uint8_t count() const { return guest::LoadU8(base, address + 10); }
  void set_count(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t count_address() const { return address + 10; }
};

// embedded; measure: sub_8245CBB0.r6, sub_8245CD68.r5
struct XmaReadOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // where the samples are read from
  uint32_t read_address() const { return guest::LoadU32(base, address + 0); }
  void set_read_address(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t read_address_address() const { return address + 0; }
};

// embedded; measure: sub_8245D070.r5
struct XmaInputPosition : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the current input buffer's address (+84 or +88)
  uint32_t input_buffer() const { return guest::LoadU32(base, address + 0); }
  void set_input_buffer(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t input_buffer_address() const { return address + 0; }
  // the input read offset
  uint32_t read_offset() const { return guest::LoadU32(base, address + 4); }
  void set_read_offset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t read_offset_address() const { return address + 4; }
  // context word 1's low 3 bits
  uint32_t format_bits() const { return guest::LoadU32(base, address + 8); }
  void set_format_bits(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t format_bits_address() const { return address + 8; }
};

// allocated; measure: AudioRenderDriver, sub_824657C8.r3
struct AudioRenderDriver : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr uint32_t kVtable = 0x8204AAEC;
  // 0x8204AAEC
  template <typename T = AudioRenderDriverVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x8204AAD0 (0x8204AA58 at the start, the base's 0x8204A6B8 after the destructor)
  uint32_t inner_vtable() const { return guest::LoadU32(base, address + 4); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  // the mastering voice: the mixer callback's argument
  uint32_t callback_arg() const { return guest::LoadU32(base, address + 12); }
  void set_callback_arg(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t callback_arg_address() const { return address + 12; }
  // the render driver client's handle (ours from src/audio/native_audio.cpp), 0 when none
  uint32_t handle() const { return guest::LoadU32(base, address + 16); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t handle_address() const { return address + 16; }
};

// image; measure: sub_824657C8.r3.+0
struct AudioRenderDriverVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // AudioRenderDriver::Register (sub_824657E0)
  uint32_t register() const { return guest::LoadU32(base, address + 28); }
  void set_register(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t register_address() const { return address + 28; }
};

// The image words docs/functions/audio.md's code uses.
struct AudioGlobals {
  // the XMA registers' ContextArrayAddress (read once at boot by sub_8245C638, used by sub_8245D138 for context ids; neither runs now)
  static uint32_t xma_context_array(uint8_t* base) { return guest::LoadU32(base, 0x82954F0Cu); }
  static void set_xma_context_array(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82954F0Cu, v); }
  static constexpr uint32_t xma_context_array_address() { return 0x82954F0Cu; }
};

}  // namespace schema
