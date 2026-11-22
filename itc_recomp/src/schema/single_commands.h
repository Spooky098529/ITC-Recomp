// The views of docs/functions/single-commands.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/single-commands.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: Single
struct Single : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // the command id (0-36), the walker's (ChunkChain::RunSingles)
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // the entry count (1-4, 8-12, 20, 22-24); the data's slot count (21, 25-27, returned); the vertex count (5) or index count (6); the primitive type (13, 17, 18, 19); the shader, 0 vertex 1 pixel (7); the object (28, a SingleCallbackObject)
  uint32_t argument0() const { return guest::LoadU32(base, address + 4); }
  void set_argument0(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t argument0_address() const { return address + 4; }
  // the vertex shader's index (13); the start vertex (17); the base vertex and start index as halfwords (18); the constants' type 0-4 (7); 32-bit indices when not 0 (6)
  uint32_t argument1() const { return guest::LoadU32(base, address + 8); }
  void set_argument1(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t argument1_address() const { return address + 8; }
  // DrawPrimitiveUP's (19) vertex count
  uint16_t vertex_count() const { return guest::LoadU16(base, address + 8); }
  void set_vertex_count(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t vertex_count_address() const { return address + 8; }
  // DrawPrimitiveUP's (19) stride in bytes
  uint16_t vertex_stride() const { return guest::LoadU16(base, address + 10); }
  void set_vertex_stride(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t vertex_stride_address() const { return address + 10; }
  // the pixel shader's index (13); the primitive count (17, 18); the vertices' address (19)
  uint32_t argument2() const { return guest::LoadU32(base, address + 12); }
  void set_argument2(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t argument2_address() const { return address + 12; }
  // SetShaderConstants' (7) first register
  uint8_t first_register() const { return guest::LoadU8(base, address + 12); }
  void set_first_register(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t first_register_address() const { return address + 12; }
  // SetShaderConstants' (7) register count (returned)
  uint8_t register_count() const { return guest::LoadU8(base, address + 13); }
  void set_register_count(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t register_count_address() const { return address + 13; }
  // the data's first word: entry 0's +0 (1-4, 8-12, 20); SetShaderConstants' bit mask (type 4); Resolve's bits picking its optional arguments (25); DrawIndexedPrimitiveUP's primitive type (27)
  uint32_t data() const { return guest::LoadU32(base, address + 16); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t data_address() const { return address + 16; }
  // entry 0's +4 (1-4, 11, 22-24: the object, index or value); SetShaderConstants' bit count (type 4); Resolve's and Clear's flags (25, 26); DrawIndexedPrimitiveUP's packet word (27)
  uint32_t data_4() const { return guest::LoadU32(base, address + 20); }
  void set_data_4(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t data_4_address() const { return address + 20; }
  // entry 0's +8 (2, 11: the value, the offset); entry 0's flag (22) and surface (23); Resolve's source rectangle, 16 bytes (25); DrawIndexedPrimitiveUP's vertex count (27)
  uint32_t data_8() const { return guest::LoadU32(base, address + 24); }
  void set_data_8(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t data_8_address() const { return address + 24; }
  // entry 0's +12 (11: the stride); DrawIndexedPrimitiveUP's index count (27)
  uint32_t data_12() const { return guest::LoadU32(base, address + 28); }
  void set_data_12(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t data_12_address() const { return address + 28; }
  // DrawIndexedPrimitiveUP's index format (27: 1 for 16-bit)
  uint32_t index_format() const { return guest::LoadU32(base, address + 32); }
  void set_index_format(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t index_format_address() const { return address + 32; }
  // DrawIndexedPrimitiveUP's stride (27)
  uint32_t stride() const { return guest::LoadU32(base, address + 36); }
  void set_stride(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t stride_address() const { return address + 36; }
  // Resolve's destination texture (25); Clear's color (26); DrawIndexedPrimitiveUP's index source (27)
  uint32_t texture_or_color() const { return guest::LoadU32(base, address + 40); }
  void set_texture_or_color(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t texture_or_color_address() const { return address + 40; }
  // Clear's Z, a float (26); Resolve's destination point, 8 bytes (25); DrawIndexedPrimitiveUP's vertex source (27)
  uint32_t z_or_point() const { return guest::LoadU32(base, address + 44); }
  void set_z_or_point(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t z_or_point_address() const { return address + 44; }
  // Clear's stencil (26)
  uint32_t clear_stencil() const { return guest::LoadU32(base, address + 48); }
  void set_clear_stencil(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t clear_stencil_address() const { return address + 48; }
  // Resolve's level (25)
  uint32_t resolve_level() const { return guest::LoadU32(base, address + 52); }
  void set_resolve_level(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t resolve_level_address() const { return address + 52; }
  // Resolve's slice (25)
  uint32_t resolve_slice() const { return guest::LoadU32(base, address + 56); }
  void set_resolve_slice(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t resolve_slice_address() const { return address + 56; }
  // Resolve's clear color: the bytes +60 to +63 as floats x 1/255 (25)
  uint8_t resolve_color_0() const { return guest::LoadU8(base, address + 60); }
  void set_resolve_color_0(uint8_t v) const { guest::StoreU8(base, address + 60, v); }
  uint32_t resolve_color_0_address() const { return address + 60; }
  uint8_t resolve_color_1() const { return guest::LoadU8(base, address + 61); }
  void set_resolve_color_1(uint8_t v) const { guest::StoreU8(base, address + 61, v); }
  uint32_t resolve_color_1_address() const { return address + 61; }
  uint8_t resolve_color_2() const { return guest::LoadU8(base, address + 62); }
  void set_resolve_color_2(uint8_t v) const { guest::StoreU8(base, address + 62, v); }
  uint32_t resolve_color_2_address() const { return address + 62; }
  uint8_t resolve_color_3() const { return guest::LoadU8(base, address + 63); }
  void set_resolve_color_3(uint8_t v) const { guest::StoreU8(base, address + 63, v); }
  uint32_t resolve_color_3_address() const { return address + 63; }
  // Resolve's clear Z (25)
  float resolve_z() const { return guest::LoadFloat(base, address + 64); }
  void set_resolve_z(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t resolve_z_address() const { return address + 64; }
  // Resolve's stencil (25); its parameters follow at +72 (with bit 4 of +16)
  uint32_t resolve_stencil() const { return guest::LoadU32(base, address + 68); }
  void set_resolve_stencil(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t resolve_stencil_address() const { return address + 68; }
};

// image; measure: Single.+4.+0
struct SingleCallbackVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0: r3 the object, r4 the device
  uint32_t call() const { return guest::LoadU32(base, address + 0); }
  void set_call(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t call_address() const { return address + 0; }
};

// gpu; measure: Single.+4
struct SingleCallbackObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = SingleCallbackVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Single.+20.+0
struct SingleReleaseVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // called first
  uint32_t slot1() const { return guest::LoadU32(base, address + 4); }
  void set_slot1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot1_address() const { return address + 4; }
  // called second; its low byte 0 (with the entry's flag 1) releases the entry
  uint32_t slot2() const { return guest::LoadU32(base, address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
  // called after the entry is pushed on the release list (read in generated/, 2026-11-21; the row leaves it out)
  uint32_t slot3() const { return guest::LoadU32(base, address + 12); }
  void set_slot3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot3_address() const { return address + 12; }
};

// gpu; measure: Single.+20
struct SingleReleaseObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = SingleReleaseVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// gpu; measure: sub_820911F8.r3
struct SingleCountObject : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the slot count (command 21) or a reference count (xaudio.md, xaudio-objects.md)
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
};

// gpu; measure: sub_822A8418.r5
struct SingleShaderBoolValues : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the first constant (0 or not)
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// gpu; measure: sub_822A84C8.r5, sub_822A8518.r5
struct SingleShaderLoopConstants : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // word 0's low byte (probably the loop count)
  uint8_t count_low() const { return guest::LoadU8(base, address + 3); }
  void set_count_low(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t count_low_address() const { return address + 3; }
  // word 1's low byte (probably the start)
  uint8_t start_low() const { return guest::LoadU8(base, address + 7); }
  void set_start_low(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t start_low_address() const { return address + 7; }
  // word 2's low byte (probably the step)
  uint8_t step_low() const { return guest::LoadU8(base, address + 11); }
  void set_step_low(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t step_low_address() const { return address + 11; }
};

// gpu; measure: sub_822AEAA0.r4, sub_822AEAA0.r7, sub_822AE918.r5, sub_822AE9E8.r5
struct SingleShaderHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // its low 3 bits: 7 skips the copy
  uint8_t kind_byte() const { return guest::LoadU8(base, address + 0); }
  void set_kind_byte(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t kind_byte_address() const { return address + 0; }
  // read whole by sub_822AE918 and sub_822AE9E8
  uint16_t kind() const { return guest::LoadU16(base, address + 0); }
  void set_kind(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // bit 18 (r4) or 17 (r7) skips; bits 5-9 a count, bits 0-4 another
  uint32_t flags() const { return guest::LoadU32(base, address + 16); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t flags_address() const { return address + 16; }
  // with +28 + 10, the words' index (x 4 from the header)
  uint32_t table_base() const { return guest::LoadU32(base, address + 24); }
  void set_table_base(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t table_base_address() const { return address + 24; }
  uint32_t table_skip() const { return guest::LoadU32(base, address + 28); }
  void set_table_skip(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t table_skip_address() const { return address + 28; }
};

// gpu; measure: sub_822AEAA0.r6
struct SingleShaderCountWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // bits 8-11 replaced (rlwimi)
  uint32_t word() const { return guest::LoadU32(base, address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_address() const { return address + 0; }
};

// gpu; measure: sub_822AE9E8.r6
struct SingleShaderPatchWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t word() const { return guest::LoadU32(base, address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_address() const { return address + 0; }
};

// gpu; measure: Device.+12960, Device::ReserveWords.ret, Device::WriteDefaultDrawRegisters.ret
struct CommandPacketWords : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // the last word written before (the write position itself)
  uint32_t word_0() const { return guest::LoadU32(base, address + 0); }
  void set_word_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_0_address() const { return address + 0; }
  // the first word written
  uint32_t word_1() const { return guest::LoadU32(base, address + 4); }
  void set_word_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t word_1_address() const { return address + 4; }
  uint32_t word_2() const { return guest::LoadU32(base, address + 8); }
  void set_word_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_2_address() const { return address + 8; }
  uint32_t word_3() const { return guest::LoadU32(base, address + 12); }
  void set_word_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t word_3_address() const { return address + 12; }
  uint32_t word_4() const { return guest::LoadU32(base, address + 16); }
  void set_word_4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t word_4_address() const { return address + 16; }
  uint32_t word_5() const { return guest::LoadU32(base, address + 20); }
  void set_word_5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t word_5_address() const { return address + 20; }
  uint32_t word_6() const { return guest::LoadU32(base, address + 24); }
  void set_word_6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t word_6_address() const { return address + 24; }
  uint32_t word_7() const { return guest::LoadU32(base, address + 28); }
  void set_word_7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t word_7_address() const { return address + 28; }
  uint32_t word_8() const { return guest::LoadU32(base, address + 32); }
  void set_word_8(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t word_8_address() const { return address + 32; }
  uint32_t word_9() const { return guest::LoadU32(base, address + 36); }
  void set_word_9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t word_9_address() const { return address + 36; }
};

// gpu; measure: CommandChain::NewBlock.ret
struct CommandNewBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 0xC0000000 + the physical address where the content ends
  uint32_t content_end() const { return guest::LoadU32(base, address + 0); }
  void set_content_end(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t content_end_address() const { return address + 0; }
  uint32_t word_1() const { return guest::LoadU32(base, address + 4); }
  void set_word_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t word_1_address() const { return address + 4; }
  uint32_t word_2() const { return guest::LoadU32(base, address + 8); }
  void set_word_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_2_address() const { return address + 8; }
  uint32_t word_3() const { return guest::LoadU32(base, address + 12); }
  void set_word_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t word_3_address() const { return address + 12; }
};

// gpu; measure: Device::AllocateFromCommandBuffer.ret
struct CommandDataSpace : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  // the vertices, 4-byte floats (Resolve 6 words, the Clear worker 21)
  uint32_t vertices_address() const { return address + 0; }
};

// gpu; measure: BeginIndexedVertices.r10
struct InlineIndexSpaceOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the index space in the command buffer's data space
  template <typename T = CommandDataSpace>
  T space() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_space(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_space(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t space_address() const { return address + 0; }
};

// embedded; measure: Device.+40@+4
struct CommandPacketWordsAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
};

}  // namespace schema
