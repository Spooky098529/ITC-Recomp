// The views of docs/functions/xaudio-objects.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/xaudio-objects.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: XAudioObject, sub_82465418.r3
struct XAudioObject : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the outer vtable (the class's methods: +0 AddRef, +4 Release, +8 the info, +24 the pass)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the inner object's vtable (first 0x8204AA58, the shared base)
  template <typename T = XAudioInnerVtable>
  T inner_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_inner_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  // the descriptor's +4 (probably the voice: the callbacks' first argument)
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
};

// image; measure: XAudioObject.+4, Reverb::Reverb.ret.+4, XmaCodec::XmaCodec.ret.+4
struct XAudioInnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t destructor() const { return guest::LoadU32(base, address + 0); }
  void set_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destructor_address() const { return address + 0; }
  // the deleting call (sub_8245AAB8, xaudio-engine.md)
  uint32_t delete_object() const { return guest::LoadU32(base, address + 12); }
  void set_delete_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t delete_object_address() const { return address + 12; }
  // sub_820911F8, shared
  uint32_t count() const { return guest::LoadU32(base, address + 16); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t count_address() const { return address + 16; }
};

// embedded; measure: XAudioCodec::Construct.r4, XAudioCodec::Init.r4, PcmCodec::PcmCodec.r4, XmaCodec::XmaCodec.r4, XAudioMatrix::Init.r4, VolumeMeter::Init.r4, Reverb::Reverb.r4, sub_825A50E0.r3, sub_825A5DB8.r3, sub_824657C8.r4
struct XAudioObjectDescriptor : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  uint32_t class_id() const { return guest::LoadU32(base, address + 0); }
  void set_class_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t class_id_address() const { return address + 0; }
  // copied to the object's +12
  uint32_t owner() const { return guest::LoadU32(base, address + 4); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t owner_address() const { return address + 4; }
  // a codec's format type: 0-3 PCM, 4 XMA (the format's +0)
  uint8_t format_type() const { return guest::LoadU8(base, address + 8); }
  void set_format_type(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t format_type_address() const { return address + 8; }
  // the matrix's entry count
  uint8_t matrix_entries() const { return guest::LoadU8(base, address + 8); }
  void set_matrix_entries(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t matrix_entries_address() const { return address + 8; }
  // the meter's channels (probably)
  uint32_t meter_channels() const { return guest::LoadU32(base, address + 8); }
  void set_meter_channels(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t meter_channels_address() const { return address + 8; }
  // the render driver's r4 to its Register (audio.md)
  uint32_t render_device() const { return guest::LoadU32(base, address + 8); }
  void set_render_device(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t render_device_address() const { return address + 8; }
  // the matrix's overwrite flag (SetOverwrite)
  uint8_t matrix_overwrite() const { return guest::LoadU8(base, address + 9); }
  void set_matrix_overwrite(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t matrix_overwrite_address() const { return address + 9; }
  // a codec's channels, or an XMA format's streams
  uint8_t format_channels() const { return guest::LoadU8(base, address + 12); }
  void set_format_channels(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t format_channels_address() const { return address + 12; }
  // the matrix's default levels (0: none)
  template <typename T = MatrixLevels>
  T matrix_default() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_matrix_default(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_matrix_default(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t matrix_default_address() const { return address + 12; }
  // the meter's history length (probably)
  uint32_t meter_history_length() const { return guest::LoadU32(base, address + 12); }
  void set_meter_history_length(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t meter_history_length_address() const { return address + 12; }
  // a PCM format's sample rate
  uint32_t format_rate() const { return guest::LoadU32(base, address + 16); }
  void set_format_rate(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t format_rate_address() const { return address + 16; }
  // the XMA codec's block size: 2^x x 16, rounded up to 16ths (XmaCodec::XmaCodec)
  float xma_block_scale() const { return guest::LoadFloat(base, address + 64); }
  void set_xma_block_scale(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t xma_block_scale_address() const { return address + 64; }
  // the codec's packet entries
  uint8_t packet_count() const { return guest::LoadU8(base, address + 68); }
  void set_packet_count(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t packet_count_address() const { return address + 68; }
  // copied to the codec's +268 (PCM, the converter's flags) or +252 (XMA)
  uint8_t converter_flags() const { return guest::LoadU8(base, address + 69); }
  void set_converter_flags(uint8_t v) const { guest::StoreU8(base, address + 69, v); }
  uint32_t converter_flags_address() const { return address + 69; }
  uint32_t packet_done_callback() const { return guest::LoadU32(base, address + 72); }
  void set_packet_done_callback(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t packet_done_callback_address() const { return address + 72; }
  uint32_t loop_end_callback() const { return guest::LoadU32(base, address + 76); }
  void set_loop_end_callback(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t loop_end_callback_address() const { return address + 76; }
};

// embedded; measure: AudioRenderDriver::Size.r4, PcmCodec::Size.r4, XmaCodec::Size.r4, Reverb::Size.r4, VolumeMeter::Size.r4, XAudioMatrix::Size.r4, sub_825A2360.r4, sub_825A5828.r4
struct FactorySizeOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t size() const { return guest::LoadU32(base, address + 0); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t size_address() const { return address + 0; }
};

// embedded; measure: AudioRenderDriver::Create.r5, PcmCodec::Create.r5, XmaCodec::Create.r5, Reverb::Create.r5, VolumeMeter::Create.r5, XAudioMatrix::Create.r5, sub_825A50E0.r5, sub_825A5DB8.r5, sub_82465868.r4, sub_82465418.r4
struct FactoryObjectOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t object() const { return guest::LoadU32(base, address + 0); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t object_address() const { return address + 0; }
};

// embedded; measure: XAudioCodec::GetInfo.r4, XAudioMatrix::GetInfo.r4, VolumeMeter::GetInfo.r4, sub_82461EF8.r4
struct FactoryInfoOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 4 a codec, 6 a matrix or the reverb, 1 the meter
  uint8_t info_byte() const { return guest::LoadU8(base, address + 0); }
  void set_info_byte(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t info_byte_address() const { return address + 0; }
  // 10 a codec, 0 a matrix, 3,750 the reverb, 1 the meter
  uint16_t info_halfword() const { return guest::LoadU16(base, address + 2); }
  void set_info_halfword(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t info_halfword_address() const { return address + 2; }
};

// embedded; measure: XAudioCodec::GetState.r4
struct CodecStateOut : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t state() const { return guest::LoadU8(base, address + 0); }
  void set_state(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t state_address() const { return address + 0; }
};

// allocated; measure: PacketList.+0, PacketLists.+24.+4
struct PacketNode : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t next() const { return guest::LoadU32(base, address + 0); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  uint32_t prev() const { return guest::LoadU32(base, address + 4); }
  void set_prev(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t prev_address() const { return address + 4; }
};

// embedded; measure: PacketList
struct PacketList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = PacketNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  uint32_t prev() const { return guest::LoadU32(base, address + 4); }
  void set_prev(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t prev_address() const { return address + 4; }
  uint32_t node_offset() const { return guest::LoadU32(base, address + 8); }
  void set_node_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_offset_address() const { return address + 8; }
};

// embedded; measure: PacketLists
struct PacketLists : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t queued_next() const { return guest::LoadU32(base, address + 0); }
  void set_queued_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t queued_next_address() const { return address + 0; }
  uint32_t queued_prev() const { return guest::LoadU32(base, address + 4); }
  void set_queued_prev(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t queued_prev_address() const { return address + 4; }
  uint32_t queued_node_offset() const { return guest::LoadU32(base, address + 8); }
  void set_queued_node_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t queued_node_offset_address() const { return address + 8; }
  uint32_t free_next() const { return guest::LoadU32(base, address + 12); }
  void set_free_next(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t free_next_address() const { return address + 12; }
  uint32_t free_prev() const { return guest::LoadU32(base, address + 16); }
  void set_free_prev(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t free_prev_address() const { return address + 16; }
  uint32_t free_node_offset() const { return guest::LoadU32(base, address + 20); }
  void set_free_node_offset(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t free_node_offset_address() const { return address + 20; }
  // the packet entries (120 bytes each, from the allocator)
  template <typename T = PacketEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t entries_address() const { return address + 24; }
};

// allocated; measure: CodecBase::OnSubmit.r4, CodecBase::RetirePacket.ret
struct PacketEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 120;
  // the list node (at the lists' node offset, 0)
  uint32_t node_next() const { return guest::LoadU32(base, address + 0); }
  void set_node_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t node_next_address() const { return address + 0; }
  uint32_t node_prev() const { return guest::LoadU32(base, address + 4); }
  void set_node_prev(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_prev_address() const { return address + 4; }
  uint32_t data() const { return guest::LoadU32(base, address + 8); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_address() const { return address + 8; }
  // in blocks
  uint32_t length() const { return guest::LoadU32(base, address + 12); }
  void set_length(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // -1 forever
  uint32_t loop_count() const { return guest::LoadU32(base, address + 16); }
  void set_loop_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t loop_count_address() const { return address + 16; }
  // in blocks
  uint32_t loop_start() const { return guest::LoadU32(base, address + 20); }
  void set_loop_start(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t loop_start_address() const { return address + 20; }
  // in blocks
  uint32_t loop_end() const { return guest::LoadU32(base, address + 24); }
  void set_loop_end(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t loop_end_address() const { return address + 24; }
  uint32_t play_begin() const { return guest::LoadU32(base, address + 28); }
  void set_play_begin(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t play_begin_address() const { return address + 28; }
  // 0: the length
  uint32_t play_end() const { return guest::LoadU32(base, address + 32); }
  void set_play_end(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t play_end_address() const { return address + 32; }
  // handed to the callbacks
  uint32_t context() const { return guest::LoadU32(base, address + 36); }
  void set_context(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t context_address() const { return address + 36; }
  // an XMA packet's region (72 bytes)
  uint32_t xma_region_address() const { return address + 40; }
  uint32_t sequence() const { return guest::LoadU32(base, address + 112); }
  void set_sequence(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t sequence_address() const { return address + 112; }
  // 0 when retired (RetirePacket)
  uint8_t retired() const { return guest::LoadU8(base, address + 116); }
  void set_retired(uint8_t v) const { guest::StoreU8(base, address + 116, v); }
  uint32_t retired_address() const { return address + 116; }
  // 1 once played from its start
  uint8_t begun() const { return guest::LoadU8(base, address + 117); }
  void set_begun(uint8_t v) const { guest::StoreU8(base, address + 117, v); }
  uint32_t begun_address() const { return address + 117; }
  uint8_t unknown_118() const { return guest::LoadU8(base, address + 118); }
  void set_unknown_118(uint8_t v) const { guest::StoreU8(base, address + 118, v); }
  uint32_t unknown_118_address() const { return address + 118; }
  uint8_t unknown_119() const { return guest::LoadU8(base, address + 119); }
  void set_unknown_119(uint8_t v) const { guest::StoreU8(base, address + 119, v); }
  uint32_t unknown_119_address() const { return address + 119; }
};

// embedded; measure: CodecBase::SubmitPacket.r4, XmaCodec::SubmitPacket.r4
struct XAudioPacket : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  uint32_t data() const { return guest::LoadU32(base, address + 0); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t data_address() const { return address + 0; }
  // in bytes
  uint32_t size() const { return guest::LoadU32(base, address + 4); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  uint32_t loop_count() const { return guest::LoadU32(base, address + 8); }
  void set_loop_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t loop_count_address() const { return address + 8; }
  // in bytes
  uint32_t loop_start() const { return guest::LoadU32(base, address + 12); }
  void set_loop_start(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t loop_start_address() const { return address + 12; }
  // in bytes (0: to the end)
  uint32_t loop_length() const { return guest::LoadU32(base, address + 16); }
  void set_loop_length(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t loop_length_address() const { return address + 16; }
  // with flags 0x20, 72 bytes copied to the entry's +40
  uint32_t xma_region_address() const { return address + 12; }
  uint32_t context() const { return guest::LoadU32(base, address + 84); }
  void set_context(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t context_address() const { return address + 84; }
};

// embedded; measure: CodecBase::SubmitRegion.r4, XmaCodec::SubmitRegion.r4
struct XAudioRegionPacket : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t data() const { return guest::LoadU32(base, address + 0); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t data_address() const { return address + 0; }
  // in bytes
  uint32_t size() const { return guest::LoadU32(base, address + 4); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  uint32_t loop_count() const { return guest::LoadU32(base, address + 8); }
  void set_loop_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t loop_count_address() const { return address + 8; }
  uint32_t loop_start() const { return guest::LoadU32(base, address + 12); }
  void set_loop_start(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t loop_start_address() const { return address + 12; }
  uint32_t loop_end() const { return guest::LoadU32(base, address + 16); }
  void set_loop_end(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t loop_end_address() const { return address + 16; }
  uint32_t play_begin() const { return guest::LoadU32(base, address + 20); }
  void set_play_begin(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t play_begin_address() const { return address + 20; }
  uint32_t play_end() const { return guest::LoadU32(base, address + 24); }
  void set_play_end(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t play_end_address() const { return address + 24; }
  uint32_t context() const { return guest::LoadU32(base, address + 28); }
  void set_context(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t context_address() const { return address + 28; }
};

// embedded; measure: CodecBase, sub_82469A88.r3
struct CodecBase : View {
  using View::View;
  static constexpr uint32_t kSize = 168;
  static constexpr uint32_t kVtable = 0x8204B958;
  // the codec's +12 (CompletePacket hands it to the callback)
  uint32_t codec_owner() const { return guest::LoadU32(base, address + -4); }
  void set_codec_owner(uint32_t v) const { guest::StoreU32(base, address + -4, v); }
  uint32_t codec_owner_address() const { return address + -4; }
  template <typename T = CodecBaseVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the queued packets' list head (PacketLists)
  uint32_t queued_next() const { return guest::LoadU32(base, address + 4); }
  void set_queued_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t queued_next_address() const { return address + 4; }
  uint32_t queued_prev() const { return guest::LoadU32(base, address + 8); }
  void set_queued_prev(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t queued_prev_address() const { return address + 8; }
  uint32_t queued_node_offset() const { return guest::LoadU32(base, address + 12); }
  void set_queued_node_offset(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t queued_node_offset_address() const { return address + 12; }
  // the free entries' list head
  uint32_t free_next() const { return guest::LoadU32(base, address + 16); }
  void set_free_next(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t free_next_address() const { return address + 16; }
  uint32_t free_prev() const { return guest::LoadU32(base, address + 20); }
  void set_free_prev(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t free_prev_address() const { return address + 20; }
  uint32_t free_node_offset() const { return guest::LoadU32(base, address + 24); }
  void set_free_node_offset(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t free_node_offset_address() const { return address + 24; }
  // the 120-byte packet entries
  template <typename T = PacketEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t entries_address() const { return address + 28; }
  // the last sequence number given (never 0)
  uint32_t next_sequence() const { return guest::LoadU32(base, address + 32); }
  void set_next_sequence(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t next_sequence_address() const { return address + 32; }
  // the 56-byte source format
  uint32_t format_address() const { return address + 36; }
  // 0-3 PCM with 4, 2, 1, 2 bytes a sample; 4 XMA
  uint8_t format_type() const { return guest::LoadU8(base, address + 36); }
  void set_format_type(uint8_t v) const { guest::StoreU8(base, address + 36, v); }
  uint32_t format_type_address() const { return address + 36; }
  // the channels, or an XMA format's streams
  uint8_t channels() const { return guest::LoadU8(base, address + 40); }
  void set_channels(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t channels_address() const { return address + 40; }
  // a PCM format's; an XMA format's first stream's (its streams 8 bytes each from here: the rate, then the channels)
  uint32_t sample_rate() const { return guest::LoadU32(base, address + 44); }
  void set_sample_rate(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t sample_rate_address() const { return address + 44; }
  // an XMA format's first stream's channels
  uint8_t stream_channels() const { return guest::LoadU8(base, address + 48); }
  void set_stream_channels(uint8_t v) const { guest::StoreU8(base, address + 48, v); }
  uint32_t stream_channels_address() const { return address + 48; }
  // bytes a block (a PCM frame)
  uint16_t block_size() const { return guest::LoadU16(base, address + 92); }
  void set_block_size(uint16_t v) const { guest::StoreU16(base, address + 92, v); }
  uint32_t block_size_address() const { return address + 92; }
  // 1 (probably the frames a block)
  uint16_t block_size_2() const { return guest::LoadU16(base, address + 94); }
  void set_block_size_2(uint16_t v) const { guest::StoreU16(base, address + 94, v); }
  uint32_t block_size_2_address() const { return address + 94; }
  // 1.0 at first
  float volume() const { return guest::LoadFloat(base, address + 96); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t volume_address() const { return address + 96; }
  // 1.0 at first
  float frequency_ratio() const { return guest::LoadFloat(base, address + 100); }
  void set_frequency_ratio(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t frequency_ratio_address() const { return address + 100; }
  // the current packet's state (cleared by RetirePacket)
  uint32_t packet_state_address() const { return address + 104; }
  // the position in the current packet (a PCM codec's)
  uint32_t position() const { return guest::LoadU32(base, address + 104); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t position_address() const { return address + 104; }
  uint32_t frames_played() const { return guest::LoadU32(base, address + 152); }
  void set_frames_played(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t frames_played_address() const { return address + 152; }
  // bit 0 playing, 1 just started, 2 stopping, 3 end of stream after the last packet, 4 starved, 5 queue empty
  uint8_t state() const { return guest::LoadU8(base, address + 156); }
  void set_state(uint8_t v) const { guest::StoreU8(base, address + 156, v); }
  uint32_t state_address() const { return address + 156; }
  // SetFormat's r5
  uint8_t format_extra() const { return guest::LoadU8(base, address + 157); }
  void set_format_extra(uint8_t v) const { guest::StoreU8(base, address + 157, v); }
  uint32_t format_extra_address() const { return address + 157; }
  // the fade-out's passes left (10 at a stop)
  uint16_t fade_passes() const { return guest::LoadU16(base, address + 158); }
  void set_fade_passes(uint16_t v) const { guest::StoreU16(base, address + 158, v); }
  uint32_t fade_passes_address() const { return address + 158; }
  uint32_t packet_done_callback() const { return guest::LoadU32(base, address + 160); }
  void set_packet_done_callback(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t packet_done_callback_address() const { return address + 160; }
  uint32_t loop_end_callback() const { return guest::LoadU32(base, address + 164); }
  void set_loop_end_callback(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t loop_end_callback_address() const { return address + 164; }
};

// image; measure: CodecBase.+0, PcmCodec.+16, XAudioCodec.+16
struct CodecBaseVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  uint32_t submit_packet() const { return guest::LoadU32(base, address + 8); }
  void set_submit_packet(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t submit_packet_address() const { return address + 8; }
  uint32_t submit_region() const { return guest::LoadU32(base, address + 12); }
  void set_submit_region(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t submit_region_address() const { return address + 12; }
  // PcmCodec::SetFrequencyRatioBase (SetFormat calls it with 1.0)
  uint32_t set_frequency_ratio() const { return guest::LoadU32(base, address + 80); }
  void set_set_frequency_ratio(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t set_frequency_ratio_address() const { return address + 80; }
  // CodecBase::SetFlags
  uint32_t set_flags() const { return guest::LoadU32(base, address + 84); }
  void set_set_flags(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t set_flags_address() const { return address + 84; }
  // CodecBase::CompletePacket
  uint32_t complete_packet() const { return guest::LoadU32(base, address + 92); }
  void set_complete_packet(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t complete_packet_address() const { return address + 92; }
};

// image; measure: PcmCodec.+0, XmaCodec::XmaCodec.ret.+0
struct CodecVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  uint32_t add_ref() const { return guest::LoadU32(base, address + 0); }
  void set_add_ref(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t add_ref_address() const { return address + 0; }
  uint32_t release() const { return guest::LoadU32(base, address + 4); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t release_address() const { return address + 4; }
  uint32_t get_info() const { return guest::LoadU32(base, address + 8); }
  void set_get_info(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t get_info_address() const { return address + 8; }
  uint32_t allocator() const { return guest::LoadU32(base, address + 20); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t allocator_address() const { return address + 20; }
  uint32_t process() const { return guest::LoadU32(base, address + 24); }
  void set_process(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t process_address() const { return address + 24; }
  uint32_t submit_packet() const { return guest::LoadU32(base, address + 28); }
  void set_submit_packet(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t submit_packet_address() const { return address + 28; }
  uint32_t flush() const { return guest::LoadU32(base, address + 32); }
  void set_flush(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flush_address() const { return address + 32; }
  uint32_t submit_region() const { return guest::LoadU32(base, address + 36); }
  void set_submit_region(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t submit_region_address() const { return address + 36; }
  uint32_t flush_again() const { return guest::LoadU32(base, address + 40); }
  void set_flush_again(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t flush_again_address() const { return address + 40; }
  uint32_t get_state() const { return guest::LoadU32(base, address + 44); }
  void set_get_state(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t get_state_address() const { return address + 44; }
  uint32_t start() const { return guest::LoadU32(base, address + 48); }
  void set_start(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t start_address() const { return address + 48; }
  uint32_t stop() const { return guest::LoadU32(base, address + 52); }
  void set_stop(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t stop_address() const { return address + 52; }
  uint32_t end_of_stream() const { return guest::LoadU32(base, address + 56); }
  void set_end_of_stream(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t end_of_stream_address() const { return address + 56; }
  uint32_t set_volume() const { return guest::LoadU32(base, address + 88); }
  void set_set_volume(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t set_volume_address() const { return address + 88; }
  uint32_t set_pitch() const { return guest::LoadU32(base, address + 96); }
  void set_set_pitch(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t set_pitch_address() const { return address + 96; }
  uint32_t set_frequency_ratio() const { return guest::LoadU32(base, address + 104); }
  void set_set_frequency_ratio(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t set_frequency_ratio_address() const { return address + 104; }
  uint32_t set_format() const { return guest::LoadU32(base, address + 112); }
  void set_set_format(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t set_format_address() const { return address + 112; }
  uint32_t query() const { return guest::LoadU32(base, address + 116); }
  void set_query(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t query_address() const { return address + 116; }
  // the XMA codec's only (sub_8246AEE0, audio.md)
  uint32_t xma_set_up() const { return guest::LoadU32(base, address + 120); }
  void set_xma_set_up(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t xma_set_up_address() const { return address + 120; }
};

// embedded; measure: Resampler, sub_82478E60.r3, sub_8247E188.r3, sub_8247E260.r3, sub_8247F558.r3, sub_82473E48.r3, sub_82474188.r3, sub_8246E208.r7, sub_8246E4C8.r7, XmaCodec.+188
struct ConverterState : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  // the input samples
  uint32_t input() const { return guest::LoadU32(base, address + 0); }
  void set_input(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t input_address() const { return address + 0; }
  // its end, in frames
  uint32_t input_end() const { return guest::LoadU32(base, address + 4); }
  void set_input_end(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t input_end_address() const { return address + 4; }
  // in frames
  uint32_t position() const { return guest::LoadU32(base, address + 8); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t position_address() const { return address + 8; }
  // the sample format (the codec's format type; 1 for XMA)
  uint8_t format() const { return guest::LoadU8(base, address + 12); }
  void set_format(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t format_address() const { return address + 12; }
  uint8_t channels() const { return guest::LoadU8(base, address + 13); }
  void set_channels(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t channels_address() const { return address + 13; }
  // the input rate x the frequency ratio
  uint32_t input_rate() const { return guest::LoadU32(base, address + 16); }
  void set_input_rate(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t input_rate_address() const { return address + 16; }
  // planar: a channel every 1,024 bytes
  uint32_t output() const { return guest::LoadU32(base, address + 20); }
  void set_output(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t output_address() const { return address + 20; }
  // 256
  uint32_t output_frames() const { return guest::LoadU32(base, address + 24); }
  void set_output_frames(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t output_frames_address() const { return address + 24; }
  uint32_t frames_written() const { return guest::LoadU32(base, address + 28); }
  void set_frames_written(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t frames_written_address() const { return address + 28; }
  // 48,000
  uint32_t output_rate() const { return guest::LoadU32(base, address + 32); }
  void set_output_rate(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t output_rate_address() const { return address + 32; }
  // the volume ramp's start (the last pass's end)
  float volume() const { return guest::LoadFloat(base, address + 36); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t volume_address() const { return address + 36; }
  // the volume ramp's end (an XMA stream's volume)
  float target_volume() const { return guest::LoadFloat(base, address + 40); }
  void set_target_volume(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t target_volume_address() const { return address + 40; }
  // the rates' quotient
  float ratio() const { return guest::LoadFloat(base, address + 44); }
  void set_ratio(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t ratio_address() const { return address + 44; }
  // the position's fraction
  float fraction() const { return guest::LoadFloat(base, address + 48); }
  void set_fraction(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t fraction_address() const { return address + 48; }
  // the last input frame's samples by channel (cleared at a start: six words)
  float last_sample_0() const { return guest::LoadFloat(base, address + 52); }
  void set_last_sample_0(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t last_sample_0_address() const { return address + 52; }
  float last_sample_1() const { return guest::LoadFloat(base, address + 56); }
  void set_last_sample_1(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t last_sample_1_address() const { return address + 56; }
  float last_sample_2() const { return guest::LoadFloat(base, address + 60); }
  void set_last_sample_2(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t last_sample_2_address() const { return address + 60; }
  float last_sample_3() const { return guest::LoadFloat(base, address + 64); }
  void set_last_sample_3(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t last_sample_3_address() const { return address + 64; }
  float last_sample_4() const { return guest::LoadFloat(base, address + 68); }
  void set_last_sample_4(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t last_sample_4_address() const { return address + 68; }
  float last_sample_5() const { return guest::LoadFloat(base, address + 72); }
  void set_last_sample_5(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t last_sample_5_address() const { return address + 72; }
  // from the table at 0x8204CF08
  uint32_t kernel() const { return guest::LoadU32(base, address + 76); }
  void set_kernel(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t kernel_address() const { return address + 76; }
  // bit 0 the format, bit 1 the rate: the kernel and ratio picked again
  uint32_t changed() const { return guest::LoadU32(base, address + 80); }
  void set_changed(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t changed_address() const { return address + 80; }
  // the descriptor's +69
  uint32_t flags() const { return guest::LoadU32(base, address + 84); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t flags_address() const { return address + 84; }
};

// allocated; measure: PcmCodec
struct PcmCodec : View {
  using View::View;
  static constexpr uint32_t kSize = 272;
  static constexpr uint32_t kVtable = 0x8204BB48;
  template <typename T = CodecVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = XAudioInnerVtable>
  T inner_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_inner_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  // the descriptor's +4
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
  // the codec base's (+16)
  template <typename T = CodecBaseVtable>
  T base_vtable() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_base_vtable(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_base_vtable(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t base_vtable_address() const { return address + 16; }
  // the queued packets' list head (PacketLists)
  uint32_t queued_next() const { return guest::LoadU32(base, address + 20); }
  void set_queued_next(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_next_address() const { return address + 20; }
  uint32_t queued_prev() const { return guest::LoadU32(base, address + 24); }
  void set_queued_prev(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t queued_prev_address() const { return address + 24; }
  uint32_t queued_node_offset() const { return guest::LoadU32(base, address + 28); }
  void set_queued_node_offset(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t queued_node_offset_address() const { return address + 28; }
  // the free entries' list head
  uint32_t free_next() const { return guest::LoadU32(base, address + 32); }
  void set_free_next(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t free_next_address() const { return address + 32; }
  uint32_t free_prev() const { return guest::LoadU32(base, address + 36); }
  void set_free_prev(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t free_prev_address() const { return address + 36; }
  uint32_t free_node_offset() const { return guest::LoadU32(base, address + 40); }
  void set_free_node_offset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t free_node_offset_address() const { return address + 40; }
  // the 120-byte packet entries
  template <typename T = PacketEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t entries_address() const { return address + 44; }
  // the last sequence number given (never 0)
  uint32_t next_sequence() const { return guest::LoadU32(base, address + 48); }
  void set_next_sequence(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t next_sequence_address() const { return address + 48; }
  // the 56-byte source format
  uint32_t format_address() const { return address + 52; }
  // 0-3 PCM with 4, 2, 1, 2 bytes a sample; 4 XMA
  uint8_t format_type() const { return guest::LoadU8(base, address + 52); }
  void set_format_type(uint8_t v) const { guest::StoreU8(base, address + 52, v); }
  uint32_t format_type_address() const { return address + 52; }
  // the channels, or an XMA format's streams
  uint8_t channels() const { return guest::LoadU8(base, address + 56); }
  void set_channels(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t channels_address() const { return address + 56; }
  // a PCM format's; an XMA format's first stream's (its streams 8 bytes each from here: the rate, then the channels)
  uint32_t sample_rate() const { return guest::LoadU32(base, address + 60); }
  void set_sample_rate(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t sample_rate_address() const { return address + 60; }
  // an XMA format's first stream's channels
  uint8_t stream_channels() const { return guest::LoadU8(base, address + 64); }
  void set_stream_channels(uint8_t v) const { guest::StoreU8(base, address + 64, v); }
  uint32_t stream_channels_address() const { return address + 64; }
  // bytes a block (a PCM frame)
  uint16_t block_size() const { return guest::LoadU16(base, address + 108); }
  void set_block_size(uint16_t v) const { guest::StoreU16(base, address + 108, v); }
  uint32_t block_size_address() const { return address + 108; }
  // 1 (probably the frames a block)
  uint16_t block_size_2() const { return guest::LoadU16(base, address + 110); }
  void set_block_size_2(uint16_t v) const { guest::StoreU16(base, address + 110, v); }
  uint32_t block_size_2_address() const { return address + 110; }
  // 1.0 at first
  float volume() const { return guest::LoadFloat(base, address + 112); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t volume_address() const { return address + 112; }
  // 1.0 at first
  float frequency_ratio() const { return guest::LoadFloat(base, address + 116); }
  void set_frequency_ratio(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t frequency_ratio_address() const { return address + 116; }
  // the current packet's state (cleared by RetirePacket)
  uint32_t packet_state_address() const { return address + 120; }
  // the position in the current packet (a PCM codec's)
  uint32_t position() const { return guest::LoadU32(base, address + 120); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t position_address() const { return address + 120; }
  uint32_t frames_played() const { return guest::LoadU32(base, address + 168); }
  void set_frames_played(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t frames_played_address() const { return address + 168; }
  // bit 0 playing, 1 just started, 2 stopping, 3 end of stream after the last packet, 4 starved, 5 queue empty
  uint8_t state() const { return guest::LoadU8(base, address + 172); }
  void set_state(uint8_t v) const { guest::StoreU8(base, address + 172, v); }
  uint32_t state_address() const { return address + 172; }
  // SetFormat's r5
  uint8_t format_extra() const { return guest::LoadU8(base, address + 173); }
  void set_format_extra(uint8_t v) const { guest::StoreU8(base, address + 173, v); }
  uint32_t format_extra_address() const { return address + 173; }
  // the fade-out's passes left (10 at a stop)
  uint16_t fade_passes() const { return guest::LoadU16(base, address + 174); }
  void set_fade_passes(uint16_t v) const { guest::StoreU16(base, address + 174, v); }
  uint32_t fade_passes_address() const { return address + 174; }
  uint32_t packet_done_callback() const { return guest::LoadU32(base, address + 176); }
  void set_packet_done_callback(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t packet_done_callback_address() const { return address + 176; }
  uint32_t loop_end_callback() const { return guest::LoadU32(base, address + 180); }
  void set_loop_end_callback(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t loop_end_callback_address() const { return address + 180; }
  // measure fault: PcmCodec::Size's r3 is the descriptor (+68 its packet count), not a codec
  uint8_t size_packet_count() const { return guest::LoadU8(base, address + 68); }
  void set_size_packet_count(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t size_packet_count_address() const { return address + 68; }
  // the input samples
  uint32_t converter_input() const { return guest::LoadU32(base, address + 184); }
  void set_converter_input(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t converter_input_address() const { return address + 184; }
  // its end, in frames
  uint32_t converter_input_end() const { return guest::LoadU32(base, address + 188); }
  void set_converter_input_end(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t converter_input_end_address() const { return address + 188; }
  // in frames
  uint32_t converter_position() const { return guest::LoadU32(base, address + 192); }
  void set_converter_position(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t converter_position_address() const { return address + 192; }
  // the sample format (the codec's format type; 1 for XMA)
  uint8_t converter_format() const { return guest::LoadU8(base, address + 196); }
  void set_converter_format(uint8_t v) const { guest::StoreU8(base, address + 196, v); }
  uint32_t converter_format_address() const { return address + 196; }
  uint8_t converter_channels() const { return guest::LoadU8(base, address + 197); }
  void set_converter_channels(uint8_t v) const { guest::StoreU8(base, address + 197, v); }
  uint32_t converter_channels_address() const { return address + 197; }
  // the input rate x the frequency ratio
  uint32_t converter_input_rate() const { return guest::LoadU32(base, address + 200); }
  void set_converter_input_rate(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t converter_input_rate_address() const { return address + 200; }
  // planar: a channel every 1,024 bytes
  uint32_t converter_output() const { return guest::LoadU32(base, address + 204); }
  void set_converter_output(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t converter_output_address() const { return address + 204; }
  // 256
  uint32_t converter_output_frames() const { return guest::LoadU32(base, address + 208); }
  void set_converter_output_frames(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t converter_output_frames_address() const { return address + 208; }
  uint32_t converter_frames_written() const { return guest::LoadU32(base, address + 212); }
  void set_converter_frames_written(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t converter_frames_written_address() const { return address + 212; }
  // 48,000
  uint32_t converter_output_rate() const { return guest::LoadU32(base, address + 216); }
  void set_converter_output_rate(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t converter_output_rate_address() const { return address + 216; }
  // the volume ramp's start (the last pass's end)
  float converter_volume() const { return guest::LoadFloat(base, address + 220); }
  void set_converter_volume(float v) const { guest::StoreFloat(base, address + 220, v); }
  uint32_t converter_volume_address() const { return address + 220; }
  // the volume ramp's end (an XMA stream's volume)
  float converter_target_volume() const { return guest::LoadFloat(base, address + 224); }
  void set_converter_target_volume(float v) const { guest::StoreFloat(base, address + 224, v); }
  uint32_t converter_target_volume_address() const { return address + 224; }
  // the rates' quotient
  float converter_ratio() const { return guest::LoadFloat(base, address + 228); }
  void set_converter_ratio(float v) const { guest::StoreFloat(base, address + 228, v); }
  uint32_t converter_ratio_address() const { return address + 228; }
  // the position's fraction
  float converter_fraction() const { return guest::LoadFloat(base, address + 232); }
  void set_converter_fraction(float v) const { guest::StoreFloat(base, address + 232, v); }
  uint32_t converter_fraction_address() const { return address + 232; }
  // the last input frame's samples by channel (cleared at a start: six words)
  float converter_last_sample_0() const { return guest::LoadFloat(base, address + 236); }
  void set_converter_last_sample_0(float v) const { guest::StoreFloat(base, address + 236, v); }
  uint32_t converter_last_sample_0_address() const { return address + 236; }
  float converter_last_sample_1() const { return guest::LoadFloat(base, address + 240); }
  void set_converter_last_sample_1(float v) const { guest::StoreFloat(base, address + 240, v); }
  uint32_t converter_last_sample_1_address() const { return address + 240; }
  float converter_last_sample_2() const { return guest::LoadFloat(base, address + 244); }
  void set_converter_last_sample_2(float v) const { guest::StoreFloat(base, address + 244, v); }
  uint32_t converter_last_sample_2_address() const { return address + 244; }
  float converter_last_sample_3() const { return guest::LoadFloat(base, address + 248); }
  void set_converter_last_sample_3(float v) const { guest::StoreFloat(base, address + 248, v); }
  uint32_t converter_last_sample_3_address() const { return address + 248; }
  float converter_last_sample_4() const { return guest::LoadFloat(base, address + 252); }
  void set_converter_last_sample_4(float v) const { guest::StoreFloat(base, address + 252, v); }
  uint32_t converter_last_sample_4_address() const { return address + 252; }
  float converter_last_sample_5() const { return guest::LoadFloat(base, address + 256); }
  void set_converter_last_sample_5(float v) const { guest::StoreFloat(base, address + 256, v); }
  uint32_t converter_last_sample_5_address() const { return address + 256; }
  // from the table at 0x8204CF08
  uint32_t converter_kernel() const { return guest::LoadU32(base, address + 260); }
  void set_converter_kernel(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t converter_kernel_address() const { return address + 260; }
  // bit 0 the format, bit 1 the rate: the kernel and ratio picked again
  uint32_t converter_changed() const { return guest::LoadU32(base, address + 264); }
  void set_converter_changed(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t converter_changed_address() const { return address + 264; }
  // the descriptor's +69
  uint32_t converter_flags() const { return guest::LoadU32(base, address + 268); }
  void set_converter_flags(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t converter_flags_address() const { return address + 268; }
};

// allocated; measure: XmaCodec, XmaCodec::XmaCodec.ret, sub_8246AB00.r3
struct XmaCodec : View {
  using View::View;
  static constexpr uint32_t kSize = 284;
  static constexpr uint32_t kVtable = 0x8204BA40;
  template <typename T = CodecVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = XAudioInnerVtable>
  T inner_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_inner_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  // the descriptor's +4
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
  // the codec base's (+16)
  template <typename T = CodecBaseVtable>
  T base_vtable() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_base_vtable(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_base_vtable(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t base_vtable_address() const { return address + 16; }
  // measure fault: XmaCodec::Size's r3 is the descriptor (+12 its stream count), not a codec
  uint8_t size_streams() const { return guest::LoadU8(base, address + 12); }
  void set_size_streams(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t size_streams_address() const { return address + 12; }
  // the queued packets' list head (PacketLists)
  uint32_t queued_next() const { return guest::LoadU32(base, address + 20); }
  void set_queued_next(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_next_address() const { return address + 20; }
  uint32_t queued_prev() const { return guest::LoadU32(base, address + 24); }
  void set_queued_prev(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t queued_prev_address() const { return address + 24; }
  uint32_t queued_node_offset() const { return guest::LoadU32(base, address + 28); }
  void set_queued_node_offset(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t queued_node_offset_address() const { return address + 28; }
  // the free entries' list head
  uint32_t free_next() const { return guest::LoadU32(base, address + 32); }
  void set_free_next(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t free_next_address() const { return address + 32; }
  uint32_t free_prev() const { return guest::LoadU32(base, address + 36); }
  void set_free_prev(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t free_prev_address() const { return address + 36; }
  uint32_t free_node_offset() const { return guest::LoadU32(base, address + 40); }
  void set_free_node_offset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t free_node_offset_address() const { return address + 40; }
  // the 120-byte packet entries
  template <typename T = PacketEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t entries_address() const { return address + 44; }
  // the last sequence number given (never 0)
  uint32_t next_sequence() const { return guest::LoadU32(base, address + 48); }
  void set_next_sequence(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t next_sequence_address() const { return address + 48; }
  // the 56-byte source format
  uint32_t format_address() const { return address + 52; }
  // 0-3 PCM with 4, 2, 1, 2 bytes a sample; 4 XMA
  uint8_t format_type() const { return guest::LoadU8(base, address + 52); }
  void set_format_type(uint8_t v) const { guest::StoreU8(base, address + 52, v); }
  uint32_t format_type_address() const { return address + 52; }
  // the channels, or an XMA format's streams
  uint8_t channels() const { return guest::LoadU8(base, address + 56); }
  void set_channels(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t channels_address() const { return address + 56; }
  // a PCM format's; an XMA format's first stream's (its streams 8 bytes each from here: the rate, then the channels)
  uint32_t sample_rate() const { return guest::LoadU32(base, address + 60); }
  void set_sample_rate(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t sample_rate_address() const { return address + 60; }
  // an XMA format's first stream's channels
  uint8_t stream_channels() const { return guest::LoadU8(base, address + 64); }
  void set_stream_channels(uint8_t v) const { guest::StoreU8(base, address + 64, v); }
  uint32_t stream_channels_address() const { return address + 64; }
  // bytes a block (a PCM frame)
  uint16_t block_size() const { return guest::LoadU16(base, address + 108); }
  void set_block_size(uint16_t v) const { guest::StoreU16(base, address + 108, v); }
  uint32_t block_size_address() const { return address + 108; }
  // 1 (probably the frames a block)
  uint16_t block_size_2() const { return guest::LoadU16(base, address + 110); }
  void set_block_size_2(uint16_t v) const { guest::StoreU16(base, address + 110, v); }
  uint32_t block_size_2_address() const { return address + 110; }
  // 1.0 at first
  float volume() const { return guest::LoadFloat(base, address + 112); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t volume_address() const { return address + 112; }
  // 1.0 at first
  float frequency_ratio() const { return guest::LoadFloat(base, address + 116); }
  void set_frequency_ratio(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t frequency_ratio_address() const { return address + 116; }
  // the current packet's state (cleared by RetirePacket)
  uint32_t packet_state_address() const { return address + 120; }
  // the position in the current packet (a PCM codec's)
  uint32_t position() const { return guest::LoadU32(base, address + 120); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t position_address() const { return address + 120; }
  uint32_t frames_played() const { return guest::LoadU32(base, address + 168); }
  void set_frames_played(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t frames_played_address() const { return address + 168; }
  // bit 0 playing, 1 just started, 2 stopping, 3 end of stream after the last packet, 4 starved, 5 queue empty
  uint8_t state() const { return guest::LoadU8(base, address + 172); }
  void set_state(uint8_t v) const { guest::StoreU8(base, address + 172, v); }
  uint32_t state_address() const { return address + 172; }
  // SetFormat's r5
  uint8_t format_extra() const { return guest::LoadU8(base, address + 173); }
  void set_format_extra(uint8_t v) const { guest::StoreU8(base, address + 173, v); }
  uint32_t format_extra_address() const { return address + 173; }
  // the fade-out's passes left (10 at a stop)
  uint16_t fade_passes() const { return guest::LoadU16(base, address + 174); }
  void set_fade_passes(uint16_t v) const { guest::StoreU16(base, address + 174, v); }
  uint32_t fade_passes_address() const { return address + 174; }
  uint32_t packet_done_callback() const { return guest::LoadU32(base, address + 176); }
  void set_packet_done_callback(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t packet_done_callback_address() const { return address + 176; }
  uint32_t loop_end_callback() const { return guest::LoadU32(base, address + 180); }
  void set_loop_end_callback(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t loop_end_callback_address() const { return address + 180; }
  // measure fault: XmaCodec::Size's r3 is the descriptor (+68 its packet count), not a codec
  uint8_t size_packet_count() const { return guest::LoadU8(base, address + 68); }
  void set_size_packet_count(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t size_packet_count_address() const { return address + 68; }
  // audio.md's XMA object (sub_8246AEE0 sets it up)
  uint32_t xma_object() const { return guest::LoadU32(base, address + 184); }
  void set_xma_object(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t xma_object_address() const { return address + 184; }
  // 88 bytes each
  template <typename T = ConverterState>
  T streams() const { return T(base, guest::LoadU32(base, address + 188)); }
  void set_streams(const View& v) const { guest::StoreU32(base, address + 188, v.address); }
  void set_streams(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t streams_address() const { return address + 188; }
  // from the descriptor's float +64
  uint32_t xma_block_size() const { return guest::LoadU32(base, address + 192); }
  void set_xma_block_size(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t xma_block_size_address() const { return address + 192; }
  // a copy of the format
  uint32_t format_copy_address() const { return address + 196; }
  // the descriptor's +69
  uint8_t converter_flags() const { return guest::LoadU8(base, address + 252); }
  void set_converter_flags(uint8_t v) const { guest::StoreU8(base, address + 252, v); }
  uint32_t converter_flags_address() const { return address + 252; }
  // 0 none yet, 1 plain packets, 2 sample regions
  uint8_t submit_mode() const { return guest::LoadU8(base, address + 266); }
  void set_submit_mode(uint8_t v) const { guest::StoreU8(base, address + 266, v); }
  uint32_t submit_mode_address() const { return address + 266; }
  // cleared
  uint32_t unknown_268() const { return guest::LoadU32(base, address + 268); }
  void set_unknown_268(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  // cleared
  uint32_t unknown_272() const { return guest::LoadU32(base, address + 272); }
  void set_unknown_272(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  // cleared
  uint32_t unknown_276() const { return guest::LoadU32(base, address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  // cleared
  uint8_t unknown_280() const { return guest::LoadU8(base, address + 280); }
  void set_unknown_280(uint8_t v) const { guest::StoreU8(base, address + 280, v); }
  uint32_t unknown_280_address() const { return address + 280; }
};

// embedded; measure: XAudioCodec, sub_82467450.ret, XAudioSourceVoice.+76
struct XAudioCodec : View {
  using View::View;
  static constexpr uint32_t kSize = 180;
  // 0x8204AA58, then 0x8204ABD0
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  uint32_t owner() const { return guest::LoadU32(base, address + 8); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t owner_address() const { return address + 8; }
  // measure fault: XAudioCodec::Size's and Create's r3 is the descriptor (+8 its format type)
  uint8_t size_format_type() const { return guest::LoadU8(base, address + 8); }
  void set_size_format_type(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t size_format_type_address() const { return address + 8; }
  // the codec base's (0x8204AB70)
  uint32_t base_vtable() const { return guest::LoadU32(base, address + 12); }
  void set_base_vtable(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t base_vtable_address() const { return address + 12; }
  // the codec base's +4 (EndOfStream reads it on the outer codec: measure fault: its r3 is the codec, +16 the base's vtable)
  uint32_t queued_next() const { return guest::LoadU32(base, address + 16); }
  void set_queued_next(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t queued_next_address() const { return address + 16; }
  // EndOfStream's +20: the outer codec's queued list head
  uint32_t queued_prev() const { return guest::LoadU32(base, address + 20); }
  void set_queued_prev(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t queued_prev_address() const { return address + 20; }
  // EndOfStream's +28: the outer codec's queued list's node offset
  uint32_t free_next() const { return guest::LoadU32(base, address + 28); }
  void set_free_next(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t free_next_address() const { return address + 28; }
  // the codec base's +160
  uint32_t packet_done_callback() const { return guest::LoadU32(base, address + 172); }
  void set_packet_done_callback(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t packet_done_callback_address() const { return address + 172; }
  // measure fault: XAudioCodec::GetState's r3 is the outer codec (+172 its state byte)
  uint8_t outer_state() const { return guest::LoadU8(base, address + 172); }
  void set_outer_state(uint8_t v) const { guest::StoreU8(base, address + 172, v); }
  uint32_t outer_state_address() const { return address + 172; }
  // the codec base's +164
  uint32_t loop_end_callback() const { return guest::LoadU32(base, address + 176); }
  void set_loop_end_callback(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t loop_end_callback_address() const { return address + 176; }
};

// allocated; measure: XAudioMatrix, XAudioVoice.+72.+4
struct XAudioMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr uint32_t kVtable = 0x8204AB30;
  template <typename T = MatrixVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = XAudioInnerVtable>
  T inner_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_inner_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  // measure fault: XAudioMatrix::Size's r3 is the descriptor (+8 its entry count)
  uint8_t size_entries() const { return guest::LoadU8(base, address + 8); }
  void set_size_entries(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t size_entries_address() const { return address + 8; }
  // (XAudioMatrix::Size reads the descriptor's +12 here: measure fault: its default matrix)
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
  uint8_t entries_set() const { return guest::LoadU8(base, address + 16); }
  void set_entries_set(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t entries_set_address() const { return address + 16; }
  template <typename T = MatrixEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t entries_address() const { return address + 20; }
  uint8_t entries_allowed() const { return guest::LoadU8(base, address + 24); }
  void set_entries_allowed(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t entries_allowed_address() const { return address + 24; }
  // the highest output channel used + 1
  uint8_t output_channels() const { return guest::LoadU8(base, address + 25); }
  void set_output_channels(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t output_channels_address() const { return address + 25; }
  // 1: the target's channels overwritten, else added
  uint8_t overwrite() const { return guest::LoadU8(base, address + 26); }
  void set_overwrite(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t overwrite_address() const { return address + 26; }
};

// image; measure: XAudioMatrix.+0
struct MatrixVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t set_overwrite() const { return guest::LoadU32(base, address + 32); }
  void set_set_overwrite(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t set_overwrite_address() const { return address + 32; }
  uint32_t set_matrix() const { return guest::LoadU32(base, address + 40); }
  void set_set_matrix(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t set_matrix_address() const { return address + 40; }
  uint32_t init() const { return guest::LoadU32(base, address + 52); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t init_address() const { return address + 52; }
  uint32_t query() const { return guest::LoadU32(base, address + 60); }
  void set_query(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t query_address() const { return address + 60; }
};

// allocated; measure: XAudioMatrix.+20
struct MatrixEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint8_t input_channel() const { return guest::LoadU8(base, address + 0); }
  void set_input_channel(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t input_channel_address() const { return address + 0; }
  uint8_t output_channel() const { return guest::LoadU8(base, address + 1); }
  void set_output_channel(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t output_channel_address() const { return address + 1; }
  // the current level
  float level() const { return guest::LoadFloat(base, address + 4); }
  void set_level(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t level_address() const { return address + 4; }
  float target_level() const { return guest::LoadFloat(base, address + 8); }
  void set_target_level(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t target_level_address() const { return address + 8; }
};

// embedded; measure: XAudioMatrix::SetMatrix.r4, XAudioMatrix::SetLevels.r4, XAudioMatrix::Init.r4.+12, XAudioMatrix.+12
struct MatrixLevels : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(base, address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  template <typename T = MatrixLevel>
  T levels() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_levels(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_levels(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t levels_address() const { return address + 4; }
};

// allocated; measure: XAudioMatrix::SetMatrix.r4.+4, XAudioMatrix::SetLevels.r4.+4
struct MatrixLevel : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // SetLevels: the entry's index
  uint8_t input_channel() const { return guest::LoadU8(base, address + 0); }
  void set_input_channel(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t input_channel_address() const { return address + 0; }
  uint8_t output_channel() const { return guest::LoadU8(base, address + 1); }
  void set_output_channel(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t output_channel_address() const { return address + 1; }
  float level() const { return guest::LoadFloat(base, address + 4); }
  void set_level(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t level_address() const { return address + 4; }
};

// allocated; measure: VolumeMeter, sub_825A5E28.r3
struct VolumeMeter : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr uint32_t kVtable = 0x8207229C;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  // the descriptor's 16 bytes from here (measure fault: VolumeMeter::Size reads its descriptor's +8 and +12 here)
  uint32_t class_id() const { return guest::LoadU32(base, address + 8); }
  void set_class_id(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t class_id_address() const { return address + 8; }
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
  // probably
  uint32_t channels() const { return guest::LoadU32(base, address + 16); }
  void set_channels(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t channels_address() const { return address + 16; }
  // probably
  uint32_t history_length() const { return guest::LoadU32(base, address + 20); }
  void set_history_length(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t history_length_address() const { return address + 20; }
  uint32_t history_slot() const { return guest::LoadU32(base, address + 24); }
  void set_history_slot(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t history_slot_address() const { return address + 24; }
  // channels x length floats
  template <typename T = MeterHistory>
  T threshold_sums() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_threshold_sums(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_threshold_sums(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t threshold_sums_address() const { return address + 28; }
  template <typename T = MeterHistory>
  T peaks() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_peaks(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_peaks(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t peaks_address() const { return address + 32; }
  // channels x length floats
  uint32_t rms() const { return guest::LoadU32(base, address + 36); }
  void set_rms(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t rms_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // 1 once the history has filled
  uint32_t filled() const { return guest::LoadU32(base, address + 44); }
  void set_filled(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t filled_address() const { return address + 44; }
  template <typename T = XAudioBumpAllocator>
  T allocator() const { return T(base, guest::LoadU32(base, address + 48)); }
  void set_allocator(const View& v) const { guest::StoreU32(base, address + 48, v.address); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t allocator_address() const { return address + 48; }
};

// allocated; measure: VolumeMeter.+28, VolumeMeter.+32, sub_825A5E28.r3.+28, sub_825A5E28.r3.+32, sub_825A5E28.r3.+36
struct MeterHistory : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the first slot's 8 channels (Process's fold-downs write 6 and 7)
  uint32_t values_address() const { return address + 0; }
  // channels x length floats (from XAudioMeterHistoryView, xaudio)
  float first() const { return guest::LoadFloat(base, address + 0); }
  void set_first(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// embedded; measure: I3dl2::ToNative.r5
struct NativeReverbParams : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  // probably (XAudio2's names, in its order but rear_delay last)
  uint32_t reflections_delay() const { return guest::LoadU32(base, address + 0); }
  void set_reflections_delay(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t reflections_delay_address() const { return address + 0; }
  uint32_t reverb_delay() const { return guest::LoadU32(base, address + 4); }
  void set_reverb_delay(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reverb_delay_address() const { return address + 4; }
  uint32_t position_left() const { return guest::LoadU32(base, address + 8); }
  void set_position_left(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t position_left_address() const { return address + 8; }
  uint32_t position_right() const { return guest::LoadU32(base, address + 12); }
  void set_position_right(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t position_right_address() const { return address + 12; }
  uint32_t position_matrix_left() const { return guest::LoadU32(base, address + 16); }
  void set_position_matrix_left(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t position_matrix_left_address() const { return address + 16; }
  uint32_t position_matrix_right() const { return guest::LoadU32(base, address + 20); }
  void set_position_matrix_right(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t position_matrix_right_address() const { return address + 20; }
  uint32_t early_diffusion() const { return guest::LoadU32(base, address + 24); }
  void set_early_diffusion(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t early_diffusion_address() const { return address + 24; }
  uint32_t late_diffusion() const { return guest::LoadU32(base, address + 28); }
  void set_late_diffusion(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t late_diffusion_address() const { return address + 28; }
  uint32_t low_eq_gain() const { return guest::LoadU32(base, address + 32); }
  void set_low_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t low_eq_gain_address() const { return address + 32; }
  uint32_t low_eq_cutoff() const { return guest::LoadU32(base, address + 36); }
  void set_low_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t low_eq_cutoff_address() const { return address + 36; }
  uint32_t high_eq_gain() const { return guest::LoadU32(base, address + 40); }
  void set_high_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t high_eq_gain_address() const { return address + 40; }
  uint32_t high_eq_cutoff() const { return guest::LoadU32(base, address + 44); }
  void set_high_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t high_eq_cutoff_address() const { return address + 44; }
  uint32_t rear_delay() const { return guest::LoadU32(base, address + 48); }
  void set_rear_delay(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t rear_delay_address() const { return address + 48; }
  float room_filter_freq() const { return guest::LoadFloat(base, address + 52); }
  void set_room_filter_freq(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t room_filter_freq_address() const { return address + 52; }
  float room_filter_main() const { return guest::LoadFloat(base, address + 56); }
  void set_room_filter_main(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t room_filter_main_address() const { return address + 56; }
  float room_filter_hf() const { return guest::LoadFloat(base, address + 60); }
  void set_room_filter_hf(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t room_filter_hf_address() const { return address + 60; }
  float reflections_gain() const { return guest::LoadFloat(base, address + 64); }
  void set_reflections_gain(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t reflections_gain_address() const { return address + 64; }
  float reverb_gain() const { return guest::LoadFloat(base, address + 68); }
  void set_reverb_gain(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t reverb_gain_address() const { return address + 68; }
  float decay_time() const { return guest::LoadFloat(base, address + 72); }
  void set_decay_time(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t decay_time_address() const { return address + 72; }
  float density() const { return guest::LoadFloat(base, address + 76); }
  void set_density(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t density_address() const { return address + 76; }
  // probably
  float room_size() const { return guest::LoadFloat(base, address + 80); }
  void set_room_size(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t room_size_address() const { return address + 80; }
};

// embedded; measure: I3dl2, I3dl2Params
struct I3dl2Reverb : View {
  using View::View;
  static constexpr uint32_t kSize = 196;
  static constexpr uint32_t kVtable = 0x8204AA8C;
  // (I3dl2::ToNative reads its input's room here: measure fault: its r3 is this object's +4)
  template <typename T = I3dl2ReverbVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // millibels
  int32_t room() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_room(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t room_address() const { return address + 4; }
  // millibels
  int32_t room_hf() const { return int32_t(guest::LoadU32(base, address + 8)); }
  void set_room_hf(int32_t v) const { guest::StoreU32(base, address + 8, uint32_t(v)); }
  uint32_t room_hf_address() const { return address + 8; }
  float room_rolloff_factor() const { return guest::LoadFloat(base, address + 12); }
  void set_room_rolloff_factor(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t room_rolloff_factor_address() const { return address + 12; }
  float decay_time() const { return guest::LoadFloat(base, address + 16); }
  void set_decay_time(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t decay_time_address() const { return address + 16; }
  float decay_hf_ratio() const { return guest::LoadFloat(base, address + 20); }
  void set_decay_hf_ratio(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t decay_hf_ratio_address() const { return address + 20; }
  // millibels
  int32_t reflections() const { return int32_t(guest::LoadU32(base, address + 24)); }
  void set_reflections(int32_t v) const { guest::StoreU32(base, address + 24, uint32_t(v)); }
  uint32_t reflections_address() const { return address + 24; }
  float reflections_delay() const { return guest::LoadFloat(base, address + 28); }
  void set_reflections_delay(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t reflections_delay_address() const { return address + 28; }
  // millibels
  int32_t reverb() const { return int32_t(guest::LoadU32(base, address + 32)); }
  void set_reverb(int32_t v) const { guest::StoreU32(base, address + 32, uint32_t(v)); }
  uint32_t reverb_address() const { return address + 32; }
  float reverb_delay() const { return guest::LoadFloat(base, address + 36); }
  void set_reverb_delay(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t reverb_delay_address() const { return address + 36; }
  float diffusion() const { return guest::LoadFloat(base, address + 40); }
  void set_diffusion(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t diffusion_address() const { return address + 40; }
  float density() const { return guest::LoadFloat(base, address + 44); }
  void set_density(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t density_address() const { return address + 44; }
  float hf_reference() const { return guest::LoadFloat(base, address + 48); }
  void set_hf_reference(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t hf_reference_address() const { return address + 48; }
  uint32_t pending_reflections_delay() const { return guest::LoadU32(base, address + 52); }
  void set_pending_reflections_delay(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t pending_reflections_delay_address() const { return address + 52; }
  uint32_t pending_reverb_delay() const { return guest::LoadU32(base, address + 56); }
  void set_pending_reverb_delay(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t pending_reverb_delay_address() const { return address + 56; }
  uint32_t pending_position_left() const { return guest::LoadU32(base, address + 60); }
  void set_pending_position_left(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t pending_position_left_address() const { return address + 60; }
  uint32_t pending_position_right() const { return guest::LoadU32(base, address + 64); }
  void set_pending_position_right(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t pending_position_right_address() const { return address + 64; }
  uint32_t pending_position_matrix_left() const { return guest::LoadU32(base, address + 68); }
  void set_pending_position_matrix_left(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t pending_position_matrix_left_address() const { return address + 68; }
  uint32_t pending_position_matrix_right() const { return guest::LoadU32(base, address + 72); }
  void set_pending_position_matrix_right(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t pending_position_matrix_right_address() const { return address + 72; }
  uint32_t pending_early_diffusion() const { return guest::LoadU32(base, address + 76); }
  void set_pending_early_diffusion(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t pending_early_diffusion_address() const { return address + 76; }
  uint32_t pending_late_diffusion() const { return guest::LoadU32(base, address + 80); }
  void set_pending_late_diffusion(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t pending_late_diffusion_address() const { return address + 80; }
  uint32_t pending_low_eq_gain() const { return guest::LoadU32(base, address + 84); }
  void set_pending_low_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t pending_low_eq_gain_address() const { return address + 84; }
  uint32_t pending_low_eq_cutoff() const { return guest::LoadU32(base, address + 88); }
  void set_pending_low_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t pending_low_eq_cutoff_address() const { return address + 88; }
  uint32_t pending_high_eq_gain() const { return guest::LoadU32(base, address + 92); }
  void set_pending_high_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t pending_high_eq_gain_address() const { return address + 92; }
  uint32_t pending_high_eq_cutoff() const { return guest::LoadU32(base, address + 96); }
  void set_pending_high_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t pending_high_eq_cutoff_address() const { return address + 96; }
  uint32_t pending_rear_delay() const { return guest::LoadU32(base, address + 100); }
  void set_pending_rear_delay(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t pending_rear_delay_address() const { return address + 100; }
  float pending_room_filter_freq() const { return guest::LoadFloat(base, address + 104); }
  void set_pending_room_filter_freq(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t pending_room_filter_freq_address() const { return address + 104; }
  float pending_room_filter_main() const { return guest::LoadFloat(base, address + 108); }
  void set_pending_room_filter_main(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t pending_room_filter_main_address() const { return address + 108; }
  float pending_room_filter_hf() const { return guest::LoadFloat(base, address + 112); }
  void set_pending_room_filter_hf(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t pending_room_filter_hf_address() const { return address + 112; }
  float pending_reflections_gain() const { return guest::LoadFloat(base, address + 116); }
  void set_pending_reflections_gain(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t pending_reflections_gain_address() const { return address + 116; }
  float pending_reverb_gain() const { return guest::LoadFloat(base, address + 120); }
  void set_pending_reverb_gain(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t pending_reverb_gain_address() const { return address + 120; }
  float pending_decay_time() const { return guest::LoadFloat(base, address + 124); }
  void set_pending_decay_time(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t pending_decay_time_address() const { return address + 124; }
  float pending_density() const { return guest::LoadFloat(base, address + 128); }
  void set_pending_density(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t pending_density_address() const { return address + 128; }
  float pending_room_size() const { return guest::LoadFloat(base, address + 132); }
  void set_pending_room_size(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t pending_room_size_address() const { return address + 132; }
  // 12 I3DL2 parameters pending (SetParameters kind 1)
  uint32_t pending_i3dl2_address() const { return address + 136; }
  float pending_gain() const { return guest::LoadFloat(base, address + 184); }
  void set_pending_gain(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t pending_gain_address() const { return address + 184; }
  uint8_t gain_changed() const { return guest::LoadU8(base, address + 188); }
  void set_gain_changed(uint8_t v) const { guest::StoreU8(base, address + 188, v); }
  uint32_t gain_changed_address() const { return address + 188; }
  uint8_t parameters_changed() const { return guest::LoadU8(base, address + 189); }
  void set_parameters_changed(uint8_t v) const { guest::StoreU8(base, address + 189, v); }
  uint32_t parameters_changed_address() const { return address + 189; }
  uint8_t i3dl2_changed() const { return guest::LoadU8(base, address + 190); }
  void set_i3dl2_changed(uint8_t v) const { guest::StoreU8(base, address + 190, v); }
  uint32_t i3dl2_changed_address() const { return address + 190; }
  // ReverbCore's (0x8204A868)
  uint32_t core_vtable() const { return guest::LoadU32(base, address + 192); }
  void set_core_vtable(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t core_vtable_address() const { return address + 192; }
};

// image; measure: I3dl2.+0, I3dl2Params.+0, Reverb.+16, Reverb.+0
struct I3dl2ReverbVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 192 in the reverb (measure fault: Reverb.+0 is Reverb::Run's core's vtable)
  uint32_t base_offset() const { return guest::LoadU32(base, address + 4); }
  void set_base_offset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t base_offset_address() const { return address + 4; }
};

// image; measure: Reverb::Reverb.ret.+0
struct ReverbVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t set_parameters() const { return guest::LoadU32(base, address + 16); }
  void set_set_parameters(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t set_parameters_address() const { return address + 16; }
  uint32_t process() const { return guest::LoadU32(base, address + 24); }
  void set_process(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t process_address() const { return address + 24; }
  uint32_t init() const { return guest::LoadU32(base, address + 28); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t init_address() const { return address + 28; }
};

// allocated; measure: Reverb, Reverb::Reverb.ret
struct Reverb : View {
  using View::View;
  static constexpr uint32_t kSize = 309936;
  static constexpr uint32_t kVtable = 0x8204AAB0;
  template <typename T = ReverbVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = XAudioInnerVtable>
  T inner_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_inner_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_inner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t inner_vtable_address() const { return address + 4; }
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
  // the I3DL2 side's
  template <typename T = I3dl2ReverbVtable>
  T core_vtable() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_core_vtable(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_core_vtable(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t core_vtable_address() const { return address + 16; }
  // millibels
  int32_t room() const { return int32_t(guest::LoadU32(base, address + 20)); }
  void set_room(int32_t v) const { guest::StoreU32(base, address + 20, uint32_t(v)); }
  uint32_t room_address() const { return address + 20; }
  // millibels
  int32_t room_hf() const { return int32_t(guest::LoadU32(base, address + 24)); }
  void set_room_hf(int32_t v) const { guest::StoreU32(base, address + 24, uint32_t(v)); }
  uint32_t room_hf_address() const { return address + 24; }
  float room_rolloff_factor() const { return guest::LoadFloat(base, address + 28); }
  void set_room_rolloff_factor(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t room_rolloff_factor_address() const { return address + 28; }
  float decay_time() const { return guest::LoadFloat(base, address + 32); }
  void set_decay_time(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t decay_time_address() const { return address + 32; }
  float decay_hf_ratio() const { return guest::LoadFloat(base, address + 36); }
  void set_decay_hf_ratio(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t decay_hf_ratio_address() const { return address + 36; }
  // millibels
  int32_t reflections() const { return int32_t(guest::LoadU32(base, address + 40)); }
  void set_reflections(int32_t v) const { guest::StoreU32(base, address + 40, uint32_t(v)); }
  uint32_t reflections_address() const { return address + 40; }
  float reflections_delay() const { return guest::LoadFloat(base, address + 44); }
  void set_reflections_delay(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t reflections_delay_address() const { return address + 44; }
  // millibels
  int32_t reverb() const { return int32_t(guest::LoadU32(base, address + 48)); }
  void set_reverb(int32_t v) const { guest::StoreU32(base, address + 48, uint32_t(v)); }
  uint32_t reverb_address() const { return address + 48; }
  float reverb_delay() const { return guest::LoadFloat(base, address + 52); }
  void set_reverb_delay(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t reverb_delay_address() const { return address + 52; }
  float diffusion() const { return guest::LoadFloat(base, address + 56); }
  void set_diffusion(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t diffusion_address() const { return address + 56; }
  float density() const { return guest::LoadFloat(base, address + 60); }
  void set_density(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t density_address() const { return address + 60; }
  float hf_reference() const { return guest::LoadFloat(base, address + 64); }
  void set_hf_reference(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t hf_reference_address() const { return address + 64; }
  uint32_t pending_reflections_delay() const { return guest::LoadU32(base, address + 68); }
  void set_pending_reflections_delay(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t pending_reflections_delay_address() const { return address + 68; }
  uint32_t pending_reverb_delay() const { return guest::LoadU32(base, address + 72); }
  void set_pending_reverb_delay(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t pending_reverb_delay_address() const { return address + 72; }
  uint32_t pending_position_left() const { return guest::LoadU32(base, address + 76); }
  void set_pending_position_left(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t pending_position_left_address() const { return address + 76; }
  uint32_t pending_position_right() const { return guest::LoadU32(base, address + 80); }
  void set_pending_position_right(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t pending_position_right_address() const { return address + 80; }
  uint32_t pending_position_matrix_left() const { return guest::LoadU32(base, address + 84); }
  void set_pending_position_matrix_left(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t pending_position_matrix_left_address() const { return address + 84; }
  uint32_t pending_position_matrix_right() const { return guest::LoadU32(base, address + 88); }
  void set_pending_position_matrix_right(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t pending_position_matrix_right_address() const { return address + 88; }
  uint32_t pending_early_diffusion() const { return guest::LoadU32(base, address + 92); }
  void set_pending_early_diffusion(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t pending_early_diffusion_address() const { return address + 92; }
  uint32_t pending_late_diffusion() const { return guest::LoadU32(base, address + 96); }
  void set_pending_late_diffusion(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t pending_late_diffusion_address() const { return address + 96; }
  uint32_t pending_low_eq_gain() const { return guest::LoadU32(base, address + 100); }
  void set_pending_low_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t pending_low_eq_gain_address() const { return address + 100; }
  uint32_t pending_low_eq_cutoff() const { return guest::LoadU32(base, address + 104); }
  void set_pending_low_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t pending_low_eq_cutoff_address() const { return address + 104; }
  uint32_t pending_high_eq_gain() const { return guest::LoadU32(base, address + 108); }
  void set_pending_high_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t pending_high_eq_gain_address() const { return address + 108; }
  uint32_t pending_high_eq_cutoff() const { return guest::LoadU32(base, address + 112); }
  void set_pending_high_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t pending_high_eq_cutoff_address() const { return address + 112; }
  uint32_t pending_rear_delay() const { return guest::LoadU32(base, address + 116); }
  void set_pending_rear_delay(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t pending_rear_delay_address() const { return address + 116; }
  float pending_room_filter_freq() const { return guest::LoadFloat(base, address + 120); }
  void set_pending_room_filter_freq(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t pending_room_filter_freq_address() const { return address + 120; }
  float pending_room_filter_main() const { return guest::LoadFloat(base, address + 124); }
  void set_pending_room_filter_main(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t pending_room_filter_main_address() const { return address + 124; }
  float pending_room_filter_hf() const { return guest::LoadFloat(base, address + 128); }
  void set_pending_room_filter_hf(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t pending_room_filter_hf_address() const { return address + 128; }
  float pending_reflections_gain() const { return guest::LoadFloat(base, address + 132); }
  void set_pending_reflections_gain(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t pending_reflections_gain_address() const { return address + 132; }
  float pending_reverb_gain() const { return guest::LoadFloat(base, address + 136); }
  void set_pending_reverb_gain(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t pending_reverb_gain_address() const { return address + 136; }
  float pending_decay_time() const { return guest::LoadFloat(base, address + 140); }
  void set_pending_decay_time(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t pending_decay_time_address() const { return address + 140; }
  float pending_density() const { return guest::LoadFloat(base, address + 144); }
  void set_pending_density(float v) const { guest::StoreFloat(base, address + 144, v); }
  uint32_t pending_density_address() const { return address + 144; }
  float pending_room_size() const { return guest::LoadFloat(base, address + 148); }
  void set_pending_room_size(float v) const { guest::StoreFloat(base, address + 148, v); }
  uint32_t pending_room_size_address() const { return address + 148; }
  // 12 I3DL2 parameters pending (SetParameters kind 1)
  uint32_t pending_i3dl2_address() const { return address + 152; }
  // measure fault: Reverb::Run's r3 is the I3DL2 side (+16): its +184, the reverb's +200
  float run_pending_gain() const { return guest::LoadFloat(base, address + 184); }
  void set_run_pending_gain(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t run_pending_gain_address() const { return address + 184; }
  // measure fault: Reverb::Run's: the I3DL2 side's +188 (the reverb's +204)
  uint8_t run_gain_changed() const { return guest::LoadU8(base, address + 188); }
  void set_run_gain_changed(uint8_t v) const { guest::StoreU8(base, address + 188, v); }
  uint32_t run_gain_changed_address() const { return address + 188; }
  // measure fault: Reverb::Run's: the I3DL2 side's +189
  uint8_t run_parameters_changed() const { return guest::LoadU8(base, address + 189); }
  void set_run_parameters_changed(uint8_t v) const { guest::StoreU8(base, address + 189, v); }
  uint32_t run_parameters_changed_address() const { return address + 189; }
  // measure fault: Reverb::Run's: the I3DL2 side's +190
  uint8_t run_i3dl2_changed() const { return guest::LoadU8(base, address + 190); }
  void set_run_i3dl2_changed(uint8_t v) const { guest::StoreU8(base, address + 190, v); }
  uint32_t run_i3dl2_changed_address() const { return address + 190; }
  // SetParameters kind 2: the gain x 0.01 (probably)
  float pending_gain() const { return guest::LoadFloat(base, address + 200); }
  void set_pending_gain(float v) const { guest::StoreFloat(base, address + 200, v); }
  uint32_t pending_gain_address() const { return address + 200; }
  uint8_t gain_changed() const { return guest::LoadU8(base, address + 204); }
  void set_gain_changed(uint8_t v) const { guest::StoreU8(base, address + 204, v); }
  uint32_t gain_changed_address() const { return address + 204; }
  uint8_t parameters_changed() const { return guest::LoadU8(base, address + 205); }
  void set_parameters_changed(uint8_t v) const { guest::StoreU8(base, address + 205, v); }
  uint32_t parameters_changed_address() const { return address + 205; }
  uint8_t i3dl2_changed() const { return guest::LoadU8(base, address + 206); }
  void set_i3dl2_changed(uint8_t v) const { guest::StoreU8(base, address + 206, v); }
  uint32_t i3dl2_changed_address() const { return address + 206; }
  // ReverbCore's (0x8204A868)
  uint32_t lines_vtable() const { return guest::LoadU32(base, address + 208); }
  void set_lines_vtable(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t lines_vtable_address() const { return address + 208; }
  // the passes' time (QueryPerformanceCounter); read by nothing found
  uint64_t time_spent() const { return guest::LoadU64(base, address + 309920); }
  void set_time_spent(uint64_t v) const { guest::StoreU64(base, address + 309920, v); }
  uint32_t time_spent_address() const { return address + 309920; }
  uint64_t passes() const { return guest::LoadU64(base, address + 309928); }
  void set_passes(uint64_t v) const { guest::StoreU64(base, address + 309928, v); }
  uint32_t passes_address() const { return address + 309928; }
};

// embedded; measure: Reverb::SetParameters.r6
struct ReverbParameterData : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // kinds 0 and 1: the parameters' address; kind 2: the gain (a float)
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: ReverbCore::Process.r4
struct ReverbInputs : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t left() const { return guest::LoadU32(base, address + 0); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  // 1,024 bytes on
  uint32_t right() const { return guest::LoadU32(base, address + 4); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t right_address() const { return address + 4; }
};

// embedded; measure: ReverbCore::Process.r5
struct ReverbOutputs : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // probably (channel 0)
  uint32_t front_left() const { return guest::LoadU32(base, address + 0); }
  void set_front_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t front_left_address() const { return address + 0; }
  // probably (channel 1)
  uint32_t front_right() const { return guest::LoadU32(base, address + 4); }
  void set_front_right(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t front_right_address() const { return address + 4; }
  // probably (channel 2; not read)
  uint32_t center() const { return guest::LoadU32(base, address + 8); }
  void set_center(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t center_address() const { return address + 8; }
  // probably (channel 4)
  uint32_t back_left() const { return guest::LoadU32(base, address + 12); }
  void set_back_left(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t back_left_address() const { return address + 12; }
  // probably (channel 5)
  uint32_t back_right() const { return guest::LoadU32(base, address + 16); }
  void set_back_right(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t back_right_address() const { return address + 16; }
};

// embedded; measure: QueryPerformanceCounter.r3
struct PerformanceCounterOut : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint64_t count() const { return guest::LoadU64(base, address + 0); }
  void set_count(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
};

// embedded; measure: sub_824624F8.r4, sub_824624F8.r5, sub_82462BC8.r4, sub_82462BC8.r5, sub_82462CF8.r4, sub_82462CF8.r5, sub_82462CF8.r6, sub_82462DC8.r4, sub_82463060.r4, sub_82463060.r5, sub_82463060.r6, sub_82463130.r4, sub_824632A0.r4, sub_82463410.r4, sub_82463410.r5, sub_82463578.r4, sub_82463578.r5, sub_82463578.r6, sub_82463620.r4, sub_82463620.r5
struct ReverbBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 1028;
  uint32_t samples_address() const { return address + 0; }
};

// embedded; measure: sub_82463620.r3
struct ReverbCrossfade : View {
  using View::View;
  static constexpr uint32_t kSize = 1056;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the crossfade's position (1.0 down to 0.0)
  float position() const { return guest::LoadFloat(base, address + 4); }
  void set_position(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t position_address() const { return address + 4; }
  // the tap faded to
  uint32_t tap_to() const { return guest::LoadU32(base, address + 8); }
  void set_tap_to(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tap_to_address() const { return address + 8; }
  // the tap faded from
  uint32_t tap_from() const { return guest::LoadU32(base, address + 12); }
  void set_tap_from(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t tap_from_address() const { return address + 12; }
  // the tap asked for
  uint32_t tap_asked() const { return guest::LoadU32(base, address + 16); }
  void set_tap_asked(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t tap_asked_address() const { return address + 16; }
  float last_output() const { return guest::LoadFloat(base, address + 20); }
  void set_last_output(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t last_output_address() const { return address + 20; }
  // (FillDelayLine's: +0 the index, +4 the fill, then the line)
  uint32_t write_index() const { return guest::LoadU32(base, address + 24); }
  void set_write_index(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t write_index_address() const { return address + 24; }
  float fill() const { return guest::LoadFloat(base, address + 28); }
  void set_fill(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t fill_address() const { return address + 28; }
  // the line's floats
  uint32_t line_address() const { return address + 32; }
};

// embedded; measure: -
struct ReverbLowPass : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  // probably the sample rate (FilterCoefficient: +4 x 2 pi / +0, a cos's angle)
  float rate() const { return guest::LoadFloat(base, address + 0); }
  void set_rate(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t rate_address() const { return address + 0; }
  // probably the cutoff frequency
  float frequency() const { return guest::LoadFloat(base, address + 4); }
  void set_frequency(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t frequency_address() const { return address + 4; }
  // a level FilterCoefficient sums with +12 (probably the room filter's main)
  float level_a() const { return guest::LoadFloat(base, address + 8); }
  void set_level_a(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t level_a_address() const { return address + 8; }
  // probably the room filter's HF level
  float level_b() const { return guest::LoadFloat(base, address + 12); }
  void set_level_b(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t level_b_address() const { return address + 12; }
  float state() const { return guest::LoadFloat(base, address + 16); }
  void set_state(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t state_address() const { return address + 16; }
  float last_output() const { return guest::LoadFloat(base, address + 20); }
  void set_last_output(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t last_output_address() const { return address + 20; }
  // FilterCoefficient's
  float gain() const { return guest::LoadFloat(base, address + 24); }
  void set_gain(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t gain_address() const { return address + 24; }
  // FilterCoefficient's (1 - it the input's weight)
  float coefficient() const { return guest::LoadFloat(base, address + 28); }
  void set_coefficient(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t coefficient_address() const { return address + 28; }
};

// embedded; measure: -
struct ReverbDelayLine : View {
  using View::View;
  static constexpr uint32_t kSize = 8236;
  // out_a = the line at the write index less tap_2, x gain_2, + the line less tap_1, x gain_1 (fmadds)
  uint32_t tap_1() const { return guest::LoadU32(base, address + 0); }
  void set_tap_1(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tap_1_address() const { return address + 0; }
  float gain_1() const { return guest::LoadFloat(base, address + 4); }
  void set_gain_1(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t gain_1_address() const { return address + 4; }
  // out_a's first frame (the last block's)
  float last_a() const { return guest::LoadFloat(base, address + 8); }
  void set_last_a(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t last_a_address() const { return address + 8; }
  uint32_t tap_2() const { return guest::LoadU32(base, address + 12); }
  void set_tap_2(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t tap_2_address() const { return address + 12; }
  float gain_2() const { return guest::LoadFloat(base, address + 16); }
  void set_gain_2(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t gain_2_address() const { return address + 16; }
  float unknown_20() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // out_b = tap 3 x gain_3
  uint32_t tap_3() const { return guest::LoadU32(base, address + 24); }
  void set_tap_3(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t tap_3_address() const { return address + 24; }
  float gain_3() const { return guest::LoadFloat(base, address + 28); }
  void set_gain_3(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t gain_3_address() const { return address + 28; }
  // out_b's first frame
  float last_b() const { return guest::LoadFloat(base, address + 32); }
  void set_last_b(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t last_b_address() const { return address + 32; }
  uint32_t write_index() const { return guest::LoadU32(base, address + 36); }
  void set_write_index(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t write_index_address() const { return address + 36; }
  float fill() const { return guest::LoadFloat(base, address + 40); }
  void set_fill(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t fill_address() const { return address + 40; }
  // the line's floats
  uint32_t line_address() const { return address + 44; }
};

// embedded; measure: sub_824632A0.r3
struct ReverbAllPass : View {
  using View::View;
  static constexpr uint32_t kSize = 2072;
  // the line read at the write index less it
  uint32_t tap() const { return guest::LoadU32(base, address + 0); }
  void set_tap(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tap_address() const { return address + 0; }
  // the output: gain_output x v + d
  float gain_output() const { return guest::LoadFloat(base, address + 4); }
  void set_gain_output(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t gain_output_address() const { return address + 4; }
  // v = gain_feedback x d + x, written to the line
  float gain_feedback() const { return guest::LoadFloat(base, address + 8); }
  void set_gain_feedback(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t gain_feedback_address() const { return address + 8; }
  // the output a frame late
  float last_output() const { return guest::LoadFloat(base, address + 12); }
  void set_last_output(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t last_output_address() const { return address + 12; }
  uint32_t write_index() const { return guest::LoadU32(base, address + 16); }
  void set_write_index(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t write_index_address() const { return address + 16; }
  float fill() const { return guest::LoadFloat(base, address + 20); }
  void set_fill(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t fill_address() const { return address + 20; }
  // the line's floats
  uint32_t line_address() const { return address + 24; }
};

// embedded; measure: -
struct ReverbDelay1024 : View {
  using View::View;
  static constexpr uint32_t kSize = 4128;
  // out_a = the line at the write index less tap_a x gain_a
  uint32_t tap_a() const { return guest::LoadU32(base, address + 0); }
  void set_tap_a(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tap_a_address() const { return address + 0; }
  float gain_a() const { return guest::LoadFloat(base, address + 4); }
  void set_gain_a(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t gain_a_address() const { return address + 4; }
  float last_a() const { return guest::LoadFloat(base, address + 8); }
  void set_last_a(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t last_a_address() const { return address + 8; }
  // out_b = ... tap_b x gain_b
  uint32_t tap_b() const { return guest::LoadU32(base, address + 12); }
  void set_tap_b(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t tap_b_address() const { return address + 12; }
  float gain_b() const { return guest::LoadFloat(base, address + 16); }
  void set_gain_b(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t gain_b_address() const { return address + 16; }
  float last_b() const { return guest::LoadFloat(base, address + 20); }
  void set_last_b(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t last_b_address() const { return address + 20; }
  uint32_t write_index() const { return guest::LoadU32(base, address + 24); }
  void set_write_index(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t write_index_address() const { return address + 24; }
  float fill() const { return guest::LoadFloat(base, address + 28); }
  void set_fill(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t fill_address() const { return address + 28; }
  // the line's floats
  uint32_t line_address() const { return address + 32; }
};

// embedded; measure: ReverbCore
struct ReverbCore : View {
  using View::View;
  static constexpr uint32_t kSize = 309712;
  // 0x8204A868 (ReverbCore::Init)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // probably (XAudio2's names, in its order but rear_delay last)
  uint32_t reflections_delay() const { return guest::LoadU32(base, address + 4); }
  void set_reflections_delay(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reflections_delay_address() const { return address + 4; }
  uint32_t reverb_delay() const { return guest::LoadU32(base, address + 8); }
  void set_reverb_delay(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t reverb_delay_address() const { return address + 8; }
  uint32_t position_left() const { return guest::LoadU32(base, address + 12); }
  void set_position_left(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t position_left_address() const { return address + 12; }
  uint32_t position_right() const { return guest::LoadU32(base, address + 16); }
  void set_position_right(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t position_right_address() const { return address + 16; }
  uint32_t position_matrix_left() const { return guest::LoadU32(base, address + 20); }
  void set_position_matrix_left(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t position_matrix_left_address() const { return address + 20; }
  uint32_t position_matrix_right() const { return guest::LoadU32(base, address + 24); }
  void set_position_matrix_right(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t position_matrix_right_address() const { return address + 24; }
  uint32_t early_diffusion() const { return guest::LoadU32(base, address + 28); }
  void set_early_diffusion(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t early_diffusion_address() const { return address + 28; }
  uint32_t late_diffusion() const { return guest::LoadU32(base, address + 32); }
  void set_late_diffusion(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t late_diffusion_address() const { return address + 32; }
  uint32_t low_eq_gain() const { return guest::LoadU32(base, address + 36); }
  void set_low_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t low_eq_gain_address() const { return address + 36; }
  uint32_t low_eq_cutoff() const { return guest::LoadU32(base, address + 40); }
  void set_low_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t low_eq_cutoff_address() const { return address + 40; }
  uint32_t high_eq_gain() const { return guest::LoadU32(base, address + 44); }
  void set_high_eq_gain(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t high_eq_gain_address() const { return address + 44; }
  uint32_t high_eq_cutoff() const { return guest::LoadU32(base, address + 48); }
  void set_high_eq_cutoff(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t high_eq_cutoff_address() const { return address + 48; }
  uint32_t rear_delay() const { return guest::LoadU32(base, address + 52); }
  void set_rear_delay(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t rear_delay_address() const { return address + 52; }
  float room_filter_freq() const { return guest::LoadFloat(base, address + 56); }
  void set_room_filter_freq(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t room_filter_freq_address() const { return address + 56; }
  float room_filter_main() const { return guest::LoadFloat(base, address + 60); }
  void set_room_filter_main(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t room_filter_main_address() const { return address + 60; }
  float room_filter_hf() const { return guest::LoadFloat(base, address + 64); }
  void set_room_filter_hf(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t room_filter_hf_address() const { return address + 64; }
  float reflections_gain() const { return guest::LoadFloat(base, address + 68); }
  void set_reflections_gain(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t reflections_gain_address() const { return address + 68; }
  float reverb_gain() const { return guest::LoadFloat(base, address + 72); }
  void set_reverb_gain(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t reverb_gain_address() const { return address + 72; }
  float decay_time() const { return guest::LoadFloat(base, address + 76); }
  void set_decay_time(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t decay_time_address() const { return address + 76; }
  float density() const { return guest::LoadFloat(base, address + 80); }
  void set_density(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t density_address() const { return address + 80; }
  // probably
  float room_size() const { return guest::LoadFloat(base, address + 84); }
  void set_room_size(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t room_size_address() const { return address + 84; }
  // the delays' lengths are the parameters times it (probably the rate)
  uint32_t sample_rate() const { return guest::LoadU32(base, address + 88); }
  void set_sample_rate(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t sample_rate_address() const { return address + 88; }
  // ReverbCore::SetGain's (the reverb's +200 x 0.01); weights the outputs
  float gain() const { return guest::LoadFloat(base, address + 92); }
  void set_gain(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t gain_address() const { return address + 92; }
  // the two chains' mix scaled by it
  float chain_mix_level() const { return guest::LoadFloat(base, address + 96); }
  void set_chain_mix_level(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t chain_mix_level_address() const { return address + 96; }
  // 1: the first input channel twice; 2: the second twice; else both (SetStereo)
  uint32_t stereo() const { return guest::LoadU32(base, address + 100); }
  void set_stereo(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t stereo_address() const { return address + 100; }
  float unknown_104() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  float early_a_crossfade_position() const { return guest::LoadFloat(base, address + 108); }
  void set_early_a_crossfade_position(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t early_a_crossfade_position_address() const { return address + 108; }
  uint32_t early_a_crossfade_tap_to() const { return guest::LoadU32(base, address + 112); }
  void set_early_a_crossfade_tap_to(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t early_a_crossfade_tap_to_address() const { return address + 112; }
  uint32_t early_a_crossfade_tap_from() const { return guest::LoadU32(base, address + 116); }
  void set_early_a_crossfade_tap_from(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t early_a_crossfade_tap_from_address() const { return address + 116; }
  uint32_t early_a_crossfade_tap_asked() const { return guest::LoadU32(base, address + 120); }
  void set_early_a_crossfade_tap_asked(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t early_a_crossfade_tap_asked_address() const { return address + 120; }
  float early_a_crossfade_last_output() const { return guest::LoadFloat(base, address + 124); }
  void set_early_a_crossfade_last_output(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t early_a_crossfade_last_output_address() const { return address + 124; }
  uint32_t early_a_crossfade_write_index() const { return guest::LoadU32(base, address + 128); }
  void set_early_a_crossfade_write_index(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t early_a_crossfade_write_index_address() const { return address + 128; }
  float early_a_crossfade_fill() const { return guest::LoadFloat(base, address + 132); }
  void set_early_a_crossfade_fill(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t early_a_crossfade_fill_address() const { return address + 132; }
  uint32_t early_a_crossfade_line_address() const { return address + 136; }
  float unknown_65672() const { return guest::LoadFloat(base, address + 65672); }
  void set_unknown_65672(float v) const { guest::StoreFloat(base, address + 65672, v); }
  uint32_t unknown_65672_address() const { return address + 65672; }
  float early_b_crossfade_position() const { return guest::LoadFloat(base, address + 65676); }
  void set_early_b_crossfade_position(float v) const { guest::StoreFloat(base, address + 65676, v); }
  uint32_t early_b_crossfade_position_address() const { return address + 65676; }
  uint32_t early_b_crossfade_tap_to() const { return guest::LoadU32(base, address + 65680); }
  void set_early_b_crossfade_tap_to(uint32_t v) const { guest::StoreU32(base, address + 65680, v); }
  uint32_t early_b_crossfade_tap_to_address() const { return address + 65680; }
  uint32_t early_b_crossfade_tap_from() const { return guest::LoadU32(base, address + 65684); }
  void set_early_b_crossfade_tap_from(uint32_t v) const { guest::StoreU32(base, address + 65684, v); }
  uint32_t early_b_crossfade_tap_from_address() const { return address + 65684; }
  uint32_t early_b_crossfade_tap_asked() const { return guest::LoadU32(base, address + 65688); }
  void set_early_b_crossfade_tap_asked(uint32_t v) const { guest::StoreU32(base, address + 65688, v); }
  uint32_t early_b_crossfade_tap_asked_address() const { return address + 65688; }
  float early_b_crossfade_last_output() const { return guest::LoadFloat(base, address + 65692); }
  void set_early_b_crossfade_last_output(float v) const { guest::StoreFloat(base, address + 65692, v); }
  uint32_t early_b_crossfade_last_output_address() const { return address + 65692; }
  uint32_t early_b_crossfade_write_index() const { return guest::LoadU32(base, address + 65696); }
  void set_early_b_crossfade_write_index(uint32_t v) const { guest::StoreU32(base, address + 65696, v); }
  uint32_t early_b_crossfade_write_index_address() const { return address + 65696; }
  float early_b_crossfade_fill() const { return guest::LoadFloat(base, address + 65700); }
  void set_early_b_crossfade_fill(float v) const { guest::StoreFloat(base, address + 65700, v); }
  uint32_t early_b_crossfade_fill_address() const { return address + 65700; }
  uint32_t early_b_crossfade_line_address() const { return address + 65704; }
  float early_a_low_pass_rate() const { return guest::LoadFloat(base, address + 131240); }
  void set_early_a_low_pass_rate(float v) const { guest::StoreFloat(base, address + 131240, v); }
  uint32_t early_a_low_pass_rate_address() const { return address + 131240; }
  float early_a_low_pass_frequency() const { return guest::LoadFloat(base, address + 131244); }
  void set_early_a_low_pass_frequency(float v) const { guest::StoreFloat(base, address + 131244, v); }
  uint32_t early_a_low_pass_frequency_address() const { return address + 131244; }
  float early_a_low_pass_level_a() const { return guest::LoadFloat(base, address + 131248); }
  void set_early_a_low_pass_level_a(float v) const { guest::StoreFloat(base, address + 131248, v); }
  uint32_t early_a_low_pass_level_a_address() const { return address + 131248; }
  float early_a_low_pass_level_b() const { return guest::LoadFloat(base, address + 131252); }
  void set_early_a_low_pass_level_b(float v) const { guest::StoreFloat(base, address + 131252, v); }
  uint32_t early_a_low_pass_level_b_address() const { return address + 131252; }
  float early_a_low_pass_state() const { return guest::LoadFloat(base, address + 131256); }
  void set_early_a_low_pass_state(float v) const { guest::StoreFloat(base, address + 131256, v); }
  uint32_t early_a_low_pass_state_address() const { return address + 131256; }
  float early_a_low_pass_last_output() const { return guest::LoadFloat(base, address + 131260); }
  void set_early_a_low_pass_last_output(float v) const { guest::StoreFloat(base, address + 131260, v); }
  uint32_t early_a_low_pass_last_output_address() const { return address + 131260; }
  float early_a_low_pass_gain() const { return guest::LoadFloat(base, address + 131264); }
  void set_early_a_low_pass_gain(float v) const { guest::StoreFloat(base, address + 131264, v); }
  uint32_t early_a_low_pass_gain_address() const { return address + 131264; }
  float early_a_low_pass_coefficient() const { return guest::LoadFloat(base, address + 131268); }
  void set_early_a_low_pass_coefficient(float v) const { guest::StoreFloat(base, address + 131268, v); }
  uint32_t early_a_low_pass_coefficient_address() const { return address + 131268; }
  float early_b_low_pass_rate() const { return guest::LoadFloat(base, address + 131272); }
  void set_early_b_low_pass_rate(float v) const { guest::StoreFloat(base, address + 131272, v); }
  uint32_t early_b_low_pass_rate_address() const { return address + 131272; }
  float early_b_low_pass_frequency() const { return guest::LoadFloat(base, address + 131276); }
  void set_early_b_low_pass_frequency(float v) const { guest::StoreFloat(base, address + 131276, v); }
  uint32_t early_b_low_pass_frequency_address() const { return address + 131276; }
  float early_b_low_pass_level_a() const { return guest::LoadFloat(base, address + 131280); }
  void set_early_b_low_pass_level_a(float v) const { guest::StoreFloat(base, address + 131280, v); }
  uint32_t early_b_low_pass_level_a_address() const { return address + 131280; }
  float early_b_low_pass_level_b() const { return guest::LoadFloat(base, address + 131284); }
  void set_early_b_low_pass_level_b(float v) const { guest::StoreFloat(base, address + 131284, v); }
  uint32_t early_b_low_pass_level_b_address() const { return address + 131284; }
  float early_b_low_pass_state() const { return guest::LoadFloat(base, address + 131288); }
  void set_early_b_low_pass_state(float v) const { guest::StoreFloat(base, address + 131288, v); }
  uint32_t early_b_low_pass_state_address() const { return address + 131288; }
  float early_b_low_pass_last_output() const { return guest::LoadFloat(base, address + 131292); }
  void set_early_b_low_pass_last_output(float v) const { guest::StoreFloat(base, address + 131292, v); }
  uint32_t early_b_low_pass_last_output_address() const { return address + 131292; }
  float early_b_low_pass_gain() const { return guest::LoadFloat(base, address + 131296); }
  void set_early_b_low_pass_gain(float v) const { guest::StoreFloat(base, address + 131296, v); }
  uint32_t early_b_low_pass_gain_address() const { return address + 131296; }
  float early_b_low_pass_coefficient() const { return guest::LoadFloat(base, address + 131300); }
  void set_early_b_low_pass_coefficient(float v) const { guest::StoreFloat(base, address + 131300, v); }
  uint32_t early_b_low_pass_coefficient_address() const { return address + 131300; }
  uint32_t early_a_delay_512_tap_1() const { return guest::LoadU32(base, address + 131312); }
  void set_early_a_delay_512_tap_1(uint32_t v) const { guest::StoreU32(base, address + 131312, v); }
  uint32_t early_a_delay_512_tap_1_address() const { return address + 131312; }
  float early_a_delay_512_gain_1() const { return guest::LoadFloat(base, address + 131316); }
  void set_early_a_delay_512_gain_1(float v) const { guest::StoreFloat(base, address + 131316, v); }
  uint32_t early_a_delay_512_gain_1_address() const { return address + 131316; }
  float early_a_delay_512_last_a() const { return guest::LoadFloat(base, address + 131320); }
  void set_early_a_delay_512_last_a(float v) const { guest::StoreFloat(base, address + 131320, v); }
  uint32_t early_a_delay_512_last_a_address() const { return address + 131320; }
  uint32_t early_a_delay_512_tap_2() const { return guest::LoadU32(base, address + 131324); }
  void set_early_a_delay_512_tap_2(uint32_t v) const { guest::StoreU32(base, address + 131324, v); }
  uint32_t early_a_delay_512_tap_2_address() const { return address + 131324; }
  float early_a_delay_512_gain_2() const { return guest::LoadFloat(base, address + 131328); }
  void set_early_a_delay_512_gain_2(float v) const { guest::StoreFloat(base, address + 131328, v); }
  uint32_t early_a_delay_512_gain_2_address() const { return address + 131328; }
  float unknown_131332() const { return guest::LoadFloat(base, address + 131332); }
  void set_unknown_131332(float v) const { guest::StoreFloat(base, address + 131332, v); }
  uint32_t unknown_131332_address() const { return address + 131332; }
  uint32_t early_a_delay_512_tap_3() const { return guest::LoadU32(base, address + 131336); }
  void set_early_a_delay_512_tap_3(uint32_t v) const { guest::StoreU32(base, address + 131336, v); }
  uint32_t early_a_delay_512_tap_3_address() const { return address + 131336; }
  float early_a_delay_512_gain_3() const { return guest::LoadFloat(base, address + 131340); }
  void set_early_a_delay_512_gain_3(float v) const { guest::StoreFloat(base, address + 131340, v); }
  uint32_t early_a_delay_512_gain_3_address() const { return address + 131340; }
  float early_a_delay_512_last_b() const { return guest::LoadFloat(base, address + 131344); }
  void set_early_a_delay_512_last_b(float v) const { guest::StoreFloat(base, address + 131344, v); }
  uint32_t early_a_delay_512_last_b_address() const { return address + 131344; }
  uint32_t early_a_delay_512_write_index() const { return guest::LoadU32(base, address + 131348); }
  void set_early_a_delay_512_write_index(uint32_t v) const { guest::StoreU32(base, address + 131348, v); }
  uint32_t early_a_delay_512_write_index_address() const { return address + 131348; }
  float early_a_delay_512_fill() const { return guest::LoadFloat(base, address + 131352); }
  void set_early_a_delay_512_fill(float v) const { guest::StoreFloat(base, address + 131352, v); }
  uint32_t early_a_delay_512_fill_address() const { return address + 131352; }
  uint32_t early_a_delay_512_line_address() const { return address + 131356; }
  uint32_t early_a_all_pass_128_tap() const { return guest::LoadU32(base, address + 133404); }
  void set_early_a_all_pass_128_tap(uint32_t v) const { guest::StoreU32(base, address + 133404, v); }
  uint32_t early_a_all_pass_128_tap_address() const { return address + 133404; }
  float early_a_all_pass_128_gain_output() const { return guest::LoadFloat(base, address + 133408); }
  void set_early_a_all_pass_128_gain_output(float v) const { guest::StoreFloat(base, address + 133408, v); }
  uint32_t early_a_all_pass_128_gain_output_address() const { return address + 133408; }
  float early_a_all_pass_128_gain_feedback() const { return guest::LoadFloat(base, address + 133412); }
  void set_early_a_all_pass_128_gain_feedback(float v) const { guest::StoreFloat(base, address + 133412, v); }
  uint32_t early_a_all_pass_128_gain_feedback_address() const { return address + 133412; }
  float early_a_all_pass_128_last_output() const { return guest::LoadFloat(base, address + 133416); }
  void set_early_a_all_pass_128_last_output(float v) const { guest::StoreFloat(base, address + 133416, v); }
  uint32_t early_a_all_pass_128_last_output_address() const { return address + 133416; }
  uint32_t early_a_all_pass_128_write_index() const { return guest::LoadU32(base, address + 133420); }
  void set_early_a_all_pass_128_write_index(uint32_t v) const { guest::StoreU32(base, address + 133420, v); }
  uint32_t early_a_all_pass_128_write_index_address() const { return address + 133420; }
  float early_a_all_pass_128_fill() const { return guest::LoadFloat(base, address + 133424); }
  void set_early_a_all_pass_128_fill(float v) const { guest::StoreFloat(base, address + 133424, v); }
  uint32_t early_a_all_pass_128_fill_address() const { return address + 133424; }
  uint32_t early_a_all_pass_128_line_address() const { return address + 133428; }
  uint32_t early_a_delay_2048_tap_1() const { return guest::LoadU32(base, address + 133940); }
  void set_early_a_delay_2048_tap_1(uint32_t v) const { guest::StoreU32(base, address + 133940, v); }
  uint32_t early_a_delay_2048_tap_1_address() const { return address + 133940; }
  float early_a_delay_2048_gain_1() const { return guest::LoadFloat(base, address + 133944); }
  void set_early_a_delay_2048_gain_1(float v) const { guest::StoreFloat(base, address + 133944, v); }
  uint32_t early_a_delay_2048_gain_1_address() const { return address + 133944; }
  float early_a_delay_2048_last_a() const { return guest::LoadFloat(base, address + 133948); }
  void set_early_a_delay_2048_last_a(float v) const { guest::StoreFloat(base, address + 133948, v); }
  uint32_t early_a_delay_2048_last_a_address() const { return address + 133948; }
  uint32_t early_a_delay_2048_tap_2() const { return guest::LoadU32(base, address + 133952); }
  void set_early_a_delay_2048_tap_2(uint32_t v) const { guest::StoreU32(base, address + 133952, v); }
  uint32_t early_a_delay_2048_tap_2_address() const { return address + 133952; }
  float early_a_delay_2048_gain_2() const { return guest::LoadFloat(base, address + 133956); }
  void set_early_a_delay_2048_gain_2(float v) const { guest::StoreFloat(base, address + 133956, v); }
  uint32_t early_a_delay_2048_gain_2_address() const { return address + 133956; }
  float unknown_133960() const { return guest::LoadFloat(base, address + 133960); }
  void set_unknown_133960(float v) const { guest::StoreFloat(base, address + 133960, v); }
  uint32_t unknown_133960_address() const { return address + 133960; }
  uint32_t early_a_delay_2048_tap_3() const { return guest::LoadU32(base, address + 133964); }
  void set_early_a_delay_2048_tap_3(uint32_t v) const { guest::StoreU32(base, address + 133964, v); }
  uint32_t early_a_delay_2048_tap_3_address() const { return address + 133964; }
  float early_a_delay_2048_gain_3() const { return guest::LoadFloat(base, address + 133968); }
  void set_early_a_delay_2048_gain_3(float v) const { guest::StoreFloat(base, address + 133968, v); }
  uint32_t early_a_delay_2048_gain_3_address() const { return address + 133968; }
  float early_a_delay_2048_last_b() const { return guest::LoadFloat(base, address + 133972); }
  void set_early_a_delay_2048_last_b(float v) const { guest::StoreFloat(base, address + 133972, v); }
  uint32_t early_a_delay_2048_last_b_address() const { return address + 133972; }
  uint32_t early_a_delay_2048_write_index() const { return guest::LoadU32(base, address + 133976); }
  void set_early_a_delay_2048_write_index(uint32_t v) const { guest::StoreU32(base, address + 133976, v); }
  uint32_t early_a_delay_2048_write_index_address() const { return address + 133976; }
  float early_a_delay_2048_fill() const { return guest::LoadFloat(base, address + 133980); }
  void set_early_a_delay_2048_fill(float v) const { guest::StoreFloat(base, address + 133980, v); }
  uint32_t early_a_delay_2048_fill_address() const { return address + 133980; }
  uint32_t early_a_delay_2048_line_address() const { return address + 133984; }
  uint32_t early_a_all_pass_256_tap() const { return guest::LoadU32(base, address + 142176); }
  void set_early_a_all_pass_256_tap(uint32_t v) const { guest::StoreU32(base, address + 142176, v); }
  uint32_t early_a_all_pass_256_tap_address() const { return address + 142176; }
  float early_a_all_pass_256_gain_output() const { return guest::LoadFloat(base, address + 142180); }
  void set_early_a_all_pass_256_gain_output(float v) const { guest::StoreFloat(base, address + 142180, v); }
  uint32_t early_a_all_pass_256_gain_output_address() const { return address + 142180; }
  float early_a_all_pass_256_gain_feedback() const { return guest::LoadFloat(base, address + 142184); }
  void set_early_a_all_pass_256_gain_feedback(float v) const { guest::StoreFloat(base, address + 142184, v); }
  uint32_t early_a_all_pass_256_gain_feedback_address() const { return address + 142184; }
  float early_a_all_pass_256_last_output() const { return guest::LoadFloat(base, address + 142188); }
  void set_early_a_all_pass_256_last_output(float v) const { guest::StoreFloat(base, address + 142188, v); }
  uint32_t early_a_all_pass_256_last_output_address() const { return address + 142188; }
  uint32_t early_a_all_pass_256_write_index() const { return guest::LoadU32(base, address + 142192); }
  void set_early_a_all_pass_256_write_index(uint32_t v) const { guest::StoreU32(base, address + 142192, v); }
  uint32_t early_a_all_pass_256_write_index_address() const { return address + 142192; }
  float early_a_all_pass_256_fill() const { return guest::LoadFloat(base, address + 142196); }
  void set_early_a_all_pass_256_fill(float v) const { guest::StoreFloat(base, address + 142196, v); }
  uint32_t early_a_all_pass_256_fill_address() const { return address + 142196; }
  uint32_t early_a_all_pass_256_line_address() const { return address + 142200; }
  uint32_t early_a_all_pass_512_tap() const { return guest::LoadU32(base, address + 143224); }
  void set_early_a_all_pass_512_tap(uint32_t v) const { guest::StoreU32(base, address + 143224, v); }
  uint32_t early_a_all_pass_512_tap_address() const { return address + 143224; }
  float early_a_all_pass_512_gain_output() const { return guest::LoadFloat(base, address + 143228); }
  void set_early_a_all_pass_512_gain_output(float v) const { guest::StoreFloat(base, address + 143228, v); }
  uint32_t early_a_all_pass_512_gain_output_address() const { return address + 143228; }
  float early_a_all_pass_512_gain_feedback() const { return guest::LoadFloat(base, address + 143232); }
  void set_early_a_all_pass_512_gain_feedback(float v) const { guest::StoreFloat(base, address + 143232, v); }
  uint32_t early_a_all_pass_512_gain_feedback_address() const { return address + 143232; }
  float early_a_all_pass_512_last_output() const { return guest::LoadFloat(base, address + 143236); }
  void set_early_a_all_pass_512_last_output(float v) const { guest::StoreFloat(base, address + 143236, v); }
  uint32_t early_a_all_pass_512_last_output_address() const { return address + 143236; }
  uint32_t early_a_all_pass_512_write_index() const { return guest::LoadU32(base, address + 143240); }
  void set_early_a_all_pass_512_write_index(uint32_t v) const { guest::StoreU32(base, address + 143240, v); }
  uint32_t early_a_all_pass_512_write_index_address() const { return address + 143240; }
  float early_a_all_pass_512_fill() const { return guest::LoadFloat(base, address + 143244); }
  void set_early_a_all_pass_512_fill(float v) const { guest::StoreFloat(base, address + 143244, v); }
  uint32_t early_a_all_pass_512_fill_address() const { return address + 143244; }
  uint32_t early_a_all_pass_512_line_address() const { return address + 143248; }
  uint32_t early_a_delay_1024_tap_a() const { return guest::LoadU32(base, address + 145296); }
  void set_early_a_delay_1024_tap_a(uint32_t v) const { guest::StoreU32(base, address + 145296, v); }
  uint32_t early_a_delay_1024_tap_a_address() const { return address + 145296; }
  float early_a_delay_1024_gain_a() const { return guest::LoadFloat(base, address + 145300); }
  void set_early_a_delay_1024_gain_a(float v) const { guest::StoreFloat(base, address + 145300, v); }
  uint32_t early_a_delay_1024_gain_a_address() const { return address + 145300; }
  float early_a_delay_1024_last_a() const { return guest::LoadFloat(base, address + 145304); }
  void set_early_a_delay_1024_last_a(float v) const { guest::StoreFloat(base, address + 145304, v); }
  uint32_t early_a_delay_1024_last_a_address() const { return address + 145304; }
  uint32_t early_a_delay_1024_tap_b() const { return guest::LoadU32(base, address + 145308); }
  void set_early_a_delay_1024_tap_b(uint32_t v) const { guest::StoreU32(base, address + 145308, v); }
  uint32_t early_a_delay_1024_tap_b_address() const { return address + 145308; }
  float early_a_delay_1024_gain_b() const { return guest::LoadFloat(base, address + 145312); }
  void set_early_a_delay_1024_gain_b(float v) const { guest::StoreFloat(base, address + 145312, v); }
  uint32_t early_a_delay_1024_gain_b_address() const { return address + 145312; }
  float early_a_delay_1024_last_b() const { return guest::LoadFloat(base, address + 145316); }
  void set_early_a_delay_1024_last_b(float v) const { guest::StoreFloat(base, address + 145316, v); }
  uint32_t early_a_delay_1024_last_b_address() const { return address + 145316; }
  uint32_t early_a_delay_1024_write_index() const { return guest::LoadU32(base, address + 145320); }
  void set_early_a_delay_1024_write_index(uint32_t v) const { guest::StoreU32(base, address + 145320, v); }
  uint32_t early_a_delay_1024_write_index_address() const { return address + 145320; }
  float early_a_delay_1024_fill() const { return guest::LoadFloat(base, address + 145324); }
  void set_early_a_delay_1024_fill(float v) const { guest::StoreFloat(base, address + 145324, v); }
  uint32_t early_a_delay_1024_fill_address() const { return address + 145324; }
  uint32_t early_a_delay_1024_line_address() const { return address + 145328; }
  uint32_t early_b_delay_512_tap_1() const { return guest::LoadU32(base, address + 149432); }
  void set_early_b_delay_512_tap_1(uint32_t v) const { guest::StoreU32(base, address + 149432, v); }
  uint32_t early_b_delay_512_tap_1_address() const { return address + 149432; }
  float early_b_delay_512_gain_1() const { return guest::LoadFloat(base, address + 149436); }
  void set_early_b_delay_512_gain_1(float v) const { guest::StoreFloat(base, address + 149436, v); }
  uint32_t early_b_delay_512_gain_1_address() const { return address + 149436; }
  float early_b_delay_512_last_a() const { return guest::LoadFloat(base, address + 149440); }
  void set_early_b_delay_512_last_a(float v) const { guest::StoreFloat(base, address + 149440, v); }
  uint32_t early_b_delay_512_last_a_address() const { return address + 149440; }
  uint32_t early_b_delay_512_tap_2() const { return guest::LoadU32(base, address + 149444); }
  void set_early_b_delay_512_tap_2(uint32_t v) const { guest::StoreU32(base, address + 149444, v); }
  uint32_t early_b_delay_512_tap_2_address() const { return address + 149444; }
  float early_b_delay_512_gain_2() const { return guest::LoadFloat(base, address + 149448); }
  void set_early_b_delay_512_gain_2(float v) const { guest::StoreFloat(base, address + 149448, v); }
  uint32_t early_b_delay_512_gain_2_address() const { return address + 149448; }
  float unknown_149452() const { return guest::LoadFloat(base, address + 149452); }
  void set_unknown_149452(float v) const { guest::StoreFloat(base, address + 149452, v); }
  uint32_t unknown_149452_address() const { return address + 149452; }
  uint32_t early_b_delay_512_tap_3() const { return guest::LoadU32(base, address + 149456); }
  void set_early_b_delay_512_tap_3(uint32_t v) const { guest::StoreU32(base, address + 149456, v); }
  uint32_t early_b_delay_512_tap_3_address() const { return address + 149456; }
  float early_b_delay_512_gain_3() const { return guest::LoadFloat(base, address + 149460); }
  void set_early_b_delay_512_gain_3(float v) const { guest::StoreFloat(base, address + 149460, v); }
  uint32_t early_b_delay_512_gain_3_address() const { return address + 149460; }
  float early_b_delay_512_last_b() const { return guest::LoadFloat(base, address + 149464); }
  void set_early_b_delay_512_last_b(float v) const { guest::StoreFloat(base, address + 149464, v); }
  uint32_t early_b_delay_512_last_b_address() const { return address + 149464; }
  uint32_t early_b_delay_512_write_index() const { return guest::LoadU32(base, address + 149468); }
  void set_early_b_delay_512_write_index(uint32_t v) const { guest::StoreU32(base, address + 149468, v); }
  uint32_t early_b_delay_512_write_index_address() const { return address + 149468; }
  float early_b_delay_512_fill() const { return guest::LoadFloat(base, address + 149472); }
  void set_early_b_delay_512_fill(float v) const { guest::StoreFloat(base, address + 149472, v); }
  uint32_t early_b_delay_512_fill_address() const { return address + 149472; }
  uint32_t early_b_delay_512_line_address() const { return address + 149476; }
  uint32_t early_b_all_pass_128_tap() const { return guest::LoadU32(base, address + 151524); }
  void set_early_b_all_pass_128_tap(uint32_t v) const { guest::StoreU32(base, address + 151524, v); }
  uint32_t early_b_all_pass_128_tap_address() const { return address + 151524; }
  float early_b_all_pass_128_gain_output() const { return guest::LoadFloat(base, address + 151528); }
  void set_early_b_all_pass_128_gain_output(float v) const { guest::StoreFloat(base, address + 151528, v); }
  uint32_t early_b_all_pass_128_gain_output_address() const { return address + 151528; }
  float early_b_all_pass_128_gain_feedback() const { return guest::LoadFloat(base, address + 151532); }
  void set_early_b_all_pass_128_gain_feedback(float v) const { guest::StoreFloat(base, address + 151532, v); }
  uint32_t early_b_all_pass_128_gain_feedback_address() const { return address + 151532; }
  float early_b_all_pass_128_last_output() const { return guest::LoadFloat(base, address + 151536); }
  void set_early_b_all_pass_128_last_output(float v) const { guest::StoreFloat(base, address + 151536, v); }
  uint32_t early_b_all_pass_128_last_output_address() const { return address + 151536; }
  uint32_t early_b_all_pass_128_write_index() const { return guest::LoadU32(base, address + 151540); }
  void set_early_b_all_pass_128_write_index(uint32_t v) const { guest::StoreU32(base, address + 151540, v); }
  uint32_t early_b_all_pass_128_write_index_address() const { return address + 151540; }
  float early_b_all_pass_128_fill() const { return guest::LoadFloat(base, address + 151544); }
  void set_early_b_all_pass_128_fill(float v) const { guest::StoreFloat(base, address + 151544, v); }
  uint32_t early_b_all_pass_128_fill_address() const { return address + 151544; }
  uint32_t early_b_all_pass_128_line_address() const { return address + 151548; }
  uint32_t early_b_delay_2048_tap_1() const { return guest::LoadU32(base, address + 152060); }
  void set_early_b_delay_2048_tap_1(uint32_t v) const { guest::StoreU32(base, address + 152060, v); }
  uint32_t early_b_delay_2048_tap_1_address() const { return address + 152060; }
  float early_b_delay_2048_gain_1() const { return guest::LoadFloat(base, address + 152064); }
  void set_early_b_delay_2048_gain_1(float v) const { guest::StoreFloat(base, address + 152064, v); }
  uint32_t early_b_delay_2048_gain_1_address() const { return address + 152064; }
  float early_b_delay_2048_last_a() const { return guest::LoadFloat(base, address + 152068); }
  void set_early_b_delay_2048_last_a(float v) const { guest::StoreFloat(base, address + 152068, v); }
  uint32_t early_b_delay_2048_last_a_address() const { return address + 152068; }
  uint32_t early_b_delay_2048_tap_2() const { return guest::LoadU32(base, address + 152072); }
  void set_early_b_delay_2048_tap_2(uint32_t v) const { guest::StoreU32(base, address + 152072, v); }
  uint32_t early_b_delay_2048_tap_2_address() const { return address + 152072; }
  float early_b_delay_2048_gain_2() const { return guest::LoadFloat(base, address + 152076); }
  void set_early_b_delay_2048_gain_2(float v) const { guest::StoreFloat(base, address + 152076, v); }
  uint32_t early_b_delay_2048_gain_2_address() const { return address + 152076; }
  float unknown_152080() const { return guest::LoadFloat(base, address + 152080); }
  void set_unknown_152080(float v) const { guest::StoreFloat(base, address + 152080, v); }
  uint32_t unknown_152080_address() const { return address + 152080; }
  uint32_t early_b_delay_2048_tap_3() const { return guest::LoadU32(base, address + 152084); }
  void set_early_b_delay_2048_tap_3(uint32_t v) const { guest::StoreU32(base, address + 152084, v); }
  uint32_t early_b_delay_2048_tap_3_address() const { return address + 152084; }
  float early_b_delay_2048_gain_3() const { return guest::LoadFloat(base, address + 152088); }
  void set_early_b_delay_2048_gain_3(float v) const { guest::StoreFloat(base, address + 152088, v); }
  uint32_t early_b_delay_2048_gain_3_address() const { return address + 152088; }
  float early_b_delay_2048_last_b() const { return guest::LoadFloat(base, address + 152092); }
  void set_early_b_delay_2048_last_b(float v) const { guest::StoreFloat(base, address + 152092, v); }
  uint32_t early_b_delay_2048_last_b_address() const { return address + 152092; }
  uint32_t early_b_delay_2048_write_index() const { return guest::LoadU32(base, address + 152096); }
  void set_early_b_delay_2048_write_index(uint32_t v) const { guest::StoreU32(base, address + 152096, v); }
  uint32_t early_b_delay_2048_write_index_address() const { return address + 152096; }
  float early_b_delay_2048_fill() const { return guest::LoadFloat(base, address + 152100); }
  void set_early_b_delay_2048_fill(float v) const { guest::StoreFloat(base, address + 152100, v); }
  uint32_t early_b_delay_2048_fill_address() const { return address + 152100; }
  uint32_t early_b_delay_2048_line_address() const { return address + 152104; }
  uint32_t early_b_all_pass_256_tap() const { return guest::LoadU32(base, address + 160296); }
  void set_early_b_all_pass_256_tap(uint32_t v) const { guest::StoreU32(base, address + 160296, v); }
  uint32_t early_b_all_pass_256_tap_address() const { return address + 160296; }
  float early_b_all_pass_256_gain_output() const { return guest::LoadFloat(base, address + 160300); }
  void set_early_b_all_pass_256_gain_output(float v) const { guest::StoreFloat(base, address + 160300, v); }
  uint32_t early_b_all_pass_256_gain_output_address() const { return address + 160300; }
  float early_b_all_pass_256_gain_feedback() const { return guest::LoadFloat(base, address + 160304); }
  void set_early_b_all_pass_256_gain_feedback(float v) const { guest::StoreFloat(base, address + 160304, v); }
  uint32_t early_b_all_pass_256_gain_feedback_address() const { return address + 160304; }
  float early_b_all_pass_256_last_output() const { return guest::LoadFloat(base, address + 160308); }
  void set_early_b_all_pass_256_last_output(float v) const { guest::StoreFloat(base, address + 160308, v); }
  uint32_t early_b_all_pass_256_last_output_address() const { return address + 160308; }
  uint32_t early_b_all_pass_256_write_index() const { return guest::LoadU32(base, address + 160312); }
  void set_early_b_all_pass_256_write_index(uint32_t v) const { guest::StoreU32(base, address + 160312, v); }
  uint32_t early_b_all_pass_256_write_index_address() const { return address + 160312; }
  float early_b_all_pass_256_fill() const { return guest::LoadFloat(base, address + 160316); }
  void set_early_b_all_pass_256_fill(float v) const { guest::StoreFloat(base, address + 160316, v); }
  uint32_t early_b_all_pass_256_fill_address() const { return address + 160316; }
  uint32_t early_b_all_pass_256_line_address() const { return address + 160320; }
  uint32_t early_b_all_pass_512_tap() const { return guest::LoadU32(base, address + 161344); }
  void set_early_b_all_pass_512_tap(uint32_t v) const { guest::StoreU32(base, address + 161344, v); }
  uint32_t early_b_all_pass_512_tap_address() const { return address + 161344; }
  float early_b_all_pass_512_gain_output() const { return guest::LoadFloat(base, address + 161348); }
  void set_early_b_all_pass_512_gain_output(float v) const { guest::StoreFloat(base, address + 161348, v); }
  uint32_t early_b_all_pass_512_gain_output_address() const { return address + 161348; }
  float early_b_all_pass_512_gain_feedback() const { return guest::LoadFloat(base, address + 161352); }
  void set_early_b_all_pass_512_gain_feedback(float v) const { guest::StoreFloat(base, address + 161352, v); }
  uint32_t early_b_all_pass_512_gain_feedback_address() const { return address + 161352; }
  float early_b_all_pass_512_last_output() const { return guest::LoadFloat(base, address + 161356); }
  void set_early_b_all_pass_512_last_output(float v) const { guest::StoreFloat(base, address + 161356, v); }
  uint32_t early_b_all_pass_512_last_output_address() const { return address + 161356; }
  uint32_t early_b_all_pass_512_write_index() const { return guest::LoadU32(base, address + 161360); }
  void set_early_b_all_pass_512_write_index(uint32_t v) const { guest::StoreU32(base, address + 161360, v); }
  uint32_t early_b_all_pass_512_write_index_address() const { return address + 161360; }
  float early_b_all_pass_512_fill() const { return guest::LoadFloat(base, address + 161364); }
  void set_early_b_all_pass_512_fill(float v) const { guest::StoreFloat(base, address + 161364, v); }
  uint32_t early_b_all_pass_512_fill_address() const { return address + 161364; }
  uint32_t early_b_all_pass_512_line_address() const { return address + 161368; }
  uint32_t early_b_delay_1024_tap_a() const { return guest::LoadU32(base, address + 163416); }
  void set_early_b_delay_1024_tap_a(uint32_t v) const { guest::StoreU32(base, address + 163416, v); }
  uint32_t early_b_delay_1024_tap_a_address() const { return address + 163416; }
  float early_b_delay_1024_gain_a() const { return guest::LoadFloat(base, address + 163420); }
  void set_early_b_delay_1024_gain_a(float v) const { guest::StoreFloat(base, address + 163420, v); }
  uint32_t early_b_delay_1024_gain_a_address() const { return address + 163420; }
  float early_b_delay_1024_last_a() const { return guest::LoadFloat(base, address + 163424); }
  void set_early_b_delay_1024_last_a(float v) const { guest::StoreFloat(base, address + 163424, v); }
  uint32_t early_b_delay_1024_last_a_address() const { return address + 163424; }
  uint32_t early_b_delay_1024_tap_b() const { return guest::LoadU32(base, address + 163428); }
  void set_early_b_delay_1024_tap_b(uint32_t v) const { guest::StoreU32(base, address + 163428, v); }
  uint32_t early_b_delay_1024_tap_b_address() const { return address + 163428; }
  float early_b_delay_1024_gain_b() const { return guest::LoadFloat(base, address + 163432); }
  void set_early_b_delay_1024_gain_b(float v) const { guest::StoreFloat(base, address + 163432, v); }
  uint32_t early_b_delay_1024_gain_b_address() const { return address + 163432; }
  float early_b_delay_1024_last_b() const { return guest::LoadFloat(base, address + 163436); }
  void set_early_b_delay_1024_last_b(float v) const { guest::StoreFloat(base, address + 163436, v); }
  uint32_t early_b_delay_1024_last_b_address() const { return address + 163436; }
  uint32_t early_b_delay_1024_write_index() const { return guest::LoadU32(base, address + 163440); }
  void set_early_b_delay_1024_write_index(uint32_t v) const { guest::StoreU32(base, address + 163440, v); }
  uint32_t early_b_delay_1024_write_index_address() const { return address + 163440; }
  float early_b_delay_1024_fill() const { return guest::LoadFloat(base, address + 163444); }
  void set_early_b_delay_1024_fill(float v) const { guest::StoreFloat(base, address + 163444, v); }
  uint32_t early_b_delay_1024_fill_address() const { return address + 163444; }
  uint32_t early_b_delay_1024_line_address() const { return address + 163448; }
  uint32_t late_all_pass_1_tap() const { return guest::LoadU32(base, address + 299312); }
  void set_late_all_pass_1_tap(uint32_t v) const { guest::StoreU32(base, address + 299312, v); }
  uint32_t late_all_pass_1_tap_address() const { return address + 299312; }
  float late_all_pass_1_gain_output() const { return guest::LoadFloat(base, address + 299316); }
  void set_late_all_pass_1_gain_output(float v) const { guest::StoreFloat(base, address + 299316, v); }
  uint32_t late_all_pass_1_gain_output_address() const { return address + 299316; }
  float late_all_pass_1_gain_feedback() const { return guest::LoadFloat(base, address + 299320); }
  void set_late_all_pass_1_gain_feedback(float v) const { guest::StoreFloat(base, address + 299320, v); }
  uint32_t late_all_pass_1_gain_feedback_address() const { return address + 299320; }
  float late_all_pass_1_last_output() const { return guest::LoadFloat(base, address + 299324); }
  void set_late_all_pass_1_last_output(float v) const { guest::StoreFloat(base, address + 299324, v); }
  uint32_t late_all_pass_1_last_output_address() const { return address + 299324; }
  uint32_t late_all_pass_1_write_index() const { return guest::LoadU32(base, address + 299328); }
  void set_late_all_pass_1_write_index(uint32_t v) const { guest::StoreU32(base, address + 299328, v); }
  uint32_t late_all_pass_1_write_index_address() const { return address + 299328; }
  float late_all_pass_1_fill() const { return guest::LoadFloat(base, address + 299332); }
  void set_late_all_pass_1_fill(float v) const { guest::StoreFloat(base, address + 299332, v); }
  uint32_t late_all_pass_1_fill_address() const { return address + 299332; }
  uint32_t late_all_pass_1_line_address() const { return address + 299336; }
  uint32_t late_all_pass_2_tap() const { return guest::LoadU32(base, address + 301384); }
  void set_late_all_pass_2_tap(uint32_t v) const { guest::StoreU32(base, address + 301384, v); }
  uint32_t late_all_pass_2_tap_address() const { return address + 301384; }
  float late_all_pass_2_gain_output() const { return guest::LoadFloat(base, address + 301388); }
  void set_late_all_pass_2_gain_output(float v) const { guest::StoreFloat(base, address + 301388, v); }
  uint32_t late_all_pass_2_gain_output_address() const { return address + 301388; }
  float late_all_pass_2_gain_feedback() const { return guest::LoadFloat(base, address + 301392); }
  void set_late_all_pass_2_gain_feedback(float v) const { guest::StoreFloat(base, address + 301392, v); }
  uint32_t late_all_pass_2_gain_feedback_address() const { return address + 301392; }
  float late_all_pass_2_last_output() const { return guest::LoadFloat(base, address + 301396); }
  void set_late_all_pass_2_last_output(float v) const { guest::StoreFloat(base, address + 301396, v); }
  uint32_t late_all_pass_2_last_output_address() const { return address + 301396; }
  uint32_t late_all_pass_2_write_index() const { return guest::LoadU32(base, address + 301400); }
  void set_late_all_pass_2_write_index(uint32_t v) const { guest::StoreU32(base, address + 301400, v); }
  uint32_t late_all_pass_2_write_index_address() const { return address + 301400; }
  float late_all_pass_2_fill() const { return guest::LoadFloat(base, address + 301404); }
  void set_late_all_pass_2_fill(float v) const { guest::StoreFloat(base, address + 301404, v); }
  uint32_t late_all_pass_2_fill_address() const { return address + 301404; }
  uint32_t late_all_pass_2_line_address() const { return address + 301408; }
  uint32_t late_all_pass_3_tap() const { return guest::LoadU32(base, address + 303456); }
  void set_late_all_pass_3_tap(uint32_t v) const { guest::StoreU32(base, address + 303456, v); }
  uint32_t late_all_pass_3_tap_address() const { return address + 303456; }
  float late_all_pass_3_gain_output() const { return guest::LoadFloat(base, address + 303460); }
  void set_late_all_pass_3_gain_output(float v) const { guest::StoreFloat(base, address + 303460, v); }
  uint32_t late_all_pass_3_gain_output_address() const { return address + 303460; }
  float late_all_pass_3_gain_feedback() const { return guest::LoadFloat(base, address + 303464); }
  void set_late_all_pass_3_gain_feedback(float v) const { guest::StoreFloat(base, address + 303464, v); }
  uint32_t late_all_pass_3_gain_feedback_address() const { return address + 303464; }
  float late_all_pass_3_last_output() const { return guest::LoadFloat(base, address + 303468); }
  void set_late_all_pass_3_last_output(float v) const { guest::StoreFloat(base, address + 303468, v); }
  uint32_t late_all_pass_3_last_output_address() const { return address + 303468; }
  uint32_t late_all_pass_3_write_index() const { return guest::LoadU32(base, address + 303472); }
  void set_late_all_pass_3_write_index(uint32_t v) const { guest::StoreU32(base, address + 303472, v); }
  uint32_t late_all_pass_3_write_index_address() const { return address + 303472; }
  float late_all_pass_3_fill() const { return guest::LoadFloat(base, address + 303476); }
  void set_late_all_pass_3_fill(float v) const { guest::StoreFloat(base, address + 303476, v); }
  uint32_t late_all_pass_3_fill_address() const { return address + 303476; }
  uint32_t late_all_pass_3_line_address() const { return address + 303480; }
  uint32_t late_all_pass_4_tap() const { return guest::LoadU32(base, address + 305528); }
  void set_late_all_pass_4_tap(uint32_t v) const { guest::StoreU32(base, address + 305528, v); }
  uint32_t late_all_pass_4_tap_address() const { return address + 305528; }
  float late_all_pass_4_gain_output() const { return guest::LoadFloat(base, address + 305532); }
  void set_late_all_pass_4_gain_output(float v) const { guest::StoreFloat(base, address + 305532, v); }
  uint32_t late_all_pass_4_gain_output_address() const { return address + 305532; }
  float late_all_pass_4_gain_feedback() const { return guest::LoadFloat(base, address + 305536); }
  void set_late_all_pass_4_gain_feedback(float v) const { guest::StoreFloat(base, address + 305536, v); }
  uint32_t late_all_pass_4_gain_feedback_address() const { return address + 305536; }
  float late_all_pass_4_last_output() const { return guest::LoadFloat(base, address + 305540); }
  void set_late_all_pass_4_last_output(float v) const { guest::StoreFloat(base, address + 305540, v); }
  uint32_t late_all_pass_4_last_output_address() const { return address + 305540; }
  uint32_t late_all_pass_4_write_index() const { return guest::LoadU32(base, address + 305544); }
  void set_late_all_pass_4_write_index(uint32_t v) const { guest::StoreU32(base, address + 305544, v); }
  uint32_t late_all_pass_4_write_index_address() const { return address + 305544; }
  float late_all_pass_4_fill() const { return guest::LoadFloat(base, address + 305548); }
  void set_late_all_pass_4_fill(float v) const { guest::StoreFloat(base, address + 305548, v); }
  uint32_t late_all_pass_4_fill_address() const { return address + 305548; }
  uint32_t late_all_pass_4_line_address() const { return address + 305552; }
  float unknown_307600() const { return guest::LoadFloat(base, address + 307600); }
  void set_unknown_307600(float v) const { guest::StoreFloat(base, address + 307600, v); }
  uint32_t unknown_307600_address() const { return address + 307600; }
  float late_crossfade_1_position() const { return guest::LoadFloat(base, address + 307604); }
  void set_late_crossfade_1_position(float v) const { guest::StoreFloat(base, address + 307604, v); }
  uint32_t late_crossfade_1_position_address() const { return address + 307604; }
  uint32_t late_crossfade_1_tap_to() const { return guest::LoadU32(base, address + 307608); }
  void set_late_crossfade_1_tap_to(uint32_t v) const { guest::StoreU32(base, address + 307608, v); }
  uint32_t late_crossfade_1_tap_to_address() const { return address + 307608; }
  uint32_t late_crossfade_1_tap_from() const { return guest::LoadU32(base, address + 307612); }
  void set_late_crossfade_1_tap_from(uint32_t v) const { guest::StoreU32(base, address + 307612, v); }
  uint32_t late_crossfade_1_tap_from_address() const { return address + 307612; }
  uint32_t late_crossfade_1_tap_asked() const { return guest::LoadU32(base, address + 307616); }
  void set_late_crossfade_1_tap_asked(uint32_t v) const { guest::StoreU32(base, address + 307616, v); }
  uint32_t late_crossfade_1_tap_asked_address() const { return address + 307616; }
  float late_crossfade_1_last_output() const { return guest::LoadFloat(base, address + 307620); }
  void set_late_crossfade_1_last_output(float v) const { guest::StoreFloat(base, address + 307620, v); }
  uint32_t late_crossfade_1_last_output_address() const { return address + 307620; }
  uint32_t late_crossfade_1_write_index() const { return guest::LoadU32(base, address + 307624); }
  void set_late_crossfade_1_write_index(uint32_t v) const { guest::StoreU32(base, address + 307624, v); }
  uint32_t late_crossfade_1_write_index_address() const { return address + 307624; }
  float late_crossfade_1_fill() const { return guest::LoadFloat(base, address + 307628); }
  void set_late_crossfade_1_fill(float v) const { guest::StoreFloat(base, address + 307628, v); }
  uint32_t late_crossfade_1_fill_address() const { return address + 307628; }
  uint32_t late_crossfade_1_line_address() const { return address + 307632; }
  float unknown_308656() const { return guest::LoadFloat(base, address + 308656); }
  void set_unknown_308656(float v) const { guest::StoreFloat(base, address + 308656, v); }
  uint32_t unknown_308656_address() const { return address + 308656; }
  float late_crossfade_2_position() const { return guest::LoadFloat(base, address + 308660); }
  void set_late_crossfade_2_position(float v) const { guest::StoreFloat(base, address + 308660, v); }
  uint32_t late_crossfade_2_position_address() const { return address + 308660; }
  uint32_t late_crossfade_2_tap_to() const { return guest::LoadU32(base, address + 308664); }
  void set_late_crossfade_2_tap_to(uint32_t v) const { guest::StoreU32(base, address + 308664, v); }
  uint32_t late_crossfade_2_tap_to_address() const { return address + 308664; }
  uint32_t late_crossfade_2_tap_from() const { return guest::LoadU32(base, address + 308668); }
  void set_late_crossfade_2_tap_from(uint32_t v) const { guest::StoreU32(base, address + 308668, v); }
  uint32_t late_crossfade_2_tap_from_address() const { return address + 308668; }
  uint32_t late_crossfade_2_tap_asked() const { return guest::LoadU32(base, address + 308672); }
  void set_late_crossfade_2_tap_asked(uint32_t v) const { guest::StoreU32(base, address + 308672, v); }
  uint32_t late_crossfade_2_tap_asked_address() const { return address + 308672; }
  float late_crossfade_2_last_output() const { return guest::LoadFloat(base, address + 308676); }
  void set_late_crossfade_2_last_output(float v) const { guest::StoreFloat(base, address + 308676, v); }
  uint32_t late_crossfade_2_last_output_address() const { return address + 308676; }
  uint32_t late_crossfade_2_write_index() const { return guest::LoadU32(base, address + 308680); }
  void set_late_crossfade_2_write_index(uint32_t v) const { guest::StoreU32(base, address + 308680, v); }
  uint32_t late_crossfade_2_write_index_address() const { return address + 308680; }
  float late_crossfade_2_fill() const { return guest::LoadFloat(base, address + 308684); }
  void set_late_crossfade_2_fill(float v) const { guest::StoreFloat(base, address + 308684, v); }
  uint32_t late_crossfade_2_fill_address() const { return address + 308684; }
  uint32_t late_crossfade_2_line_address() const { return address + 308688; }
  // the chains' mix gains (Process reads +0x247B0, +0x200EC, +0x247B4, +0x200E8)
  float chain_mix_gain_1() const { return guest::LoadFloat(base, address + 131304); }
  void set_chain_mix_gain_1(float v) const { guest::StoreFloat(base, address + 131304, v); }
  uint32_t chain_mix_gain_1_address() const { return address + 131304; }
  float chain_mix_gain_2() const { return guest::LoadFloat(base, address + 131308); }
  void set_chain_mix_gain_2(float v) const { guest::StoreFloat(base, address + 131308, v); }
  uint32_t chain_mix_gain_2_address() const { return address + 131308; }
  float chain_mix_gain_3() const { return guest::LoadFloat(base, address + 149424); }
  void set_chain_mix_gain_3(float v) const { guest::StoreFloat(base, address + 149424, v); }
  uint32_t chain_mix_gain_3_address() const { return address + 149424; }
  float chain_mix_gain_4() const { return guest::LoadFloat(base, address + 149428); }
  void set_chain_mix_gain_4(float v) const { guest::StoreFloat(base, address + 149428, v); }
  uint32_t chain_mix_gain_4_address() const { return address + 149428; }
  uint32_t unknown_167544() const { return guest::LoadU32(base, address + 167544); }
  void set_unknown_167544(uint32_t v) const { guest::StoreU32(base, address + 167544, v); }
  uint32_t unknown_167544_address() const { return address + 167544; }
  float unknown_167548() const { return guest::LoadFloat(base, address + 167548); }
  void set_unknown_167548(float v) const { guest::StoreFloat(base, address + 167548, v); }
  uint32_t unknown_167548_address() const { return address + 167548; }
  float unknown_167552() const { return guest::LoadFloat(base, address + 167552); }
  void set_unknown_167552(float v) const { guest::StoreFloat(base, address + 167552, v); }
  uint32_t unknown_167552_address() const { return address + 167552; }
  float unknown_167556() const { return guest::LoadFloat(base, address + 167556); }
  void set_unknown_167556(float v) const { guest::StoreFloat(base, address + 167556, v); }
  uint32_t unknown_167556_address() const { return address + 167556; }
  uint32_t unknown_167560() const { return guest::LoadU32(base, address + 167560); }
  void set_unknown_167560(uint32_t v) const { guest::StoreU32(base, address + 167560, v); }
  uint32_t unknown_167560_address() const { return address + 167560; }
  float unknown_167564() const { return guest::LoadFloat(base, address + 167564); }
  void set_unknown_167564(float v) const { guest::StoreFloat(base, address + 167564, v); }
  uint32_t unknown_167564_address() const { return address + 167564; }
  uint32_t unknown_169616() const { return guest::LoadU32(base, address + 169616); }
  void set_unknown_169616(uint32_t v) const { guest::StoreU32(base, address + 169616, v); }
  uint32_t unknown_169616_address() const { return address + 169616; }
  float unknown_169620() const { return guest::LoadFloat(base, address + 169620); }
  void set_unknown_169620(float v) const { guest::StoreFloat(base, address + 169620, v); }
  uint32_t unknown_169620_address() const { return address + 169620; }
  float unknown_169624() const { return guest::LoadFloat(base, address + 169624); }
  void set_unknown_169624(float v) const { guest::StoreFloat(base, address + 169624, v); }
  uint32_t unknown_169624_address() const { return address + 169624; }
  float unknown_169628() const { return guest::LoadFloat(base, address + 169628); }
  void set_unknown_169628(float v) const { guest::StoreFloat(base, address + 169628, v); }
  uint32_t unknown_169628_address() const { return address + 169628; }
  uint32_t unknown_169632() const { return guest::LoadU32(base, address + 169632); }
  void set_unknown_169632(uint32_t v) const { guest::StoreU32(base, address + 169632, v); }
  uint32_t unknown_169632_address() const { return address + 169632; }
  float unknown_169636() const { return guest::LoadFloat(base, address + 169636); }
  void set_unknown_169636(float v) const { guest::StoreFloat(base, address + 169636, v); }
  uint32_t unknown_169636_address() const { return address + 169636; }
  uint32_t unknown_171688() const { return guest::LoadU32(base, address + 171688); }
  void set_unknown_171688(uint32_t v) const { guest::StoreU32(base, address + 171688, v); }
  uint32_t unknown_171688_address() const { return address + 171688; }
  float unknown_171692() const { return guest::LoadFloat(base, address + 171692); }
  void set_unknown_171692(float v) const { guest::StoreFloat(base, address + 171692, v); }
  uint32_t unknown_171692_address() const { return address + 171692; }
  float unknown_171696() const { return guest::LoadFloat(base, address + 171696); }
  void set_unknown_171696(float v) const { guest::StoreFloat(base, address + 171696, v); }
  uint32_t unknown_171696_address() const { return address + 171696; }
  float unknown_171700() const { return guest::LoadFloat(base, address + 171700); }
  void set_unknown_171700(float v) const { guest::StoreFloat(base, address + 171700, v); }
  uint32_t unknown_171700_address() const { return address + 171700; }
  float unknown_171704() const { return guest::LoadFloat(base, address + 171704); }
  void set_unknown_171704(float v) const { guest::StoreFloat(base, address + 171704, v); }
  uint32_t unknown_171704_address() const { return address + 171704; }
  float unknown_171708() const { return guest::LoadFloat(base, address + 171708); }
  void set_unknown_171708(float v) const { guest::StoreFloat(base, address + 171708, v); }
  uint32_t unknown_171708_address() const { return address + 171708; }
  uint32_t unknown_171712() const { return guest::LoadU32(base, address + 171712); }
  void set_unknown_171712(uint32_t v) const { guest::StoreU32(base, address + 171712, v); }
  uint32_t unknown_171712_address() const { return address + 171712; }
  float unknown_171716() const { return guest::LoadFloat(base, address + 171716); }
  void set_unknown_171716(float v) const { guest::StoreFloat(base, address + 171716, v); }
  uint32_t unknown_171716_address() const { return address + 171716; }
  float unknown_171720() const { return guest::LoadFloat(base, address + 171720); }
  void set_unknown_171720(float v) const { guest::StoreFloat(base, address + 171720, v); }
  uint32_t unknown_171720_address() const { return address + 171720; }
  float unknown_171724() const { return guest::LoadFloat(base, address + 171724); }
  void set_unknown_171724(float v) const { guest::StoreFloat(base, address + 171724, v); }
  uint32_t unknown_171724_address() const { return address + 171724; }
  float unknown_171728() const { return guest::LoadFloat(base, address + 171728); }
  void set_unknown_171728(float v) const { guest::StoreFloat(base, address + 171728, v); }
  uint32_t unknown_171728_address() const { return address + 171728; }
  float unknown_171732() const { return guest::LoadFloat(base, address + 171732); }
  void set_unknown_171732(float v) const { guest::StoreFloat(base, address + 171732, v); }
  uint32_t unknown_171732_address() const { return address + 171732; }
  float unknown_171736() const { return guest::LoadFloat(base, address + 171736); }
  void set_unknown_171736(float v) const { guest::StoreFloat(base, address + 171736, v); }
  uint32_t unknown_171736_address() const { return address + 171736; }
  float unknown_171740() const { return guest::LoadFloat(base, address + 171740); }
  void set_unknown_171740(float v) const { guest::StoreFloat(base, address + 171740, v); }
  uint32_t unknown_171740_address() const { return address + 171740; }
  uint32_t unknown_171744() const { return guest::LoadU32(base, address + 171744); }
  void set_unknown_171744(uint32_t v) const { guest::StoreU32(base, address + 171744, v); }
  uint32_t unknown_171744_address() const { return address + 171744; }
  float unknown_171748() const { return guest::LoadFloat(base, address + 171748); }
  void set_unknown_171748(float v) const { guest::StoreFloat(base, address + 171748, v); }
  uint32_t unknown_171748_address() const { return address + 171748; }
  float unknown_171752() const { return guest::LoadFloat(base, address + 171752); }
  void set_unknown_171752(float v) const { guest::StoreFloat(base, address + 171752, v); }
  uint32_t unknown_171752_address() const { return address + 171752; }
  float unknown_171756() const { return guest::LoadFloat(base, address + 171756); }
  void set_unknown_171756(float v) const { guest::StoreFloat(base, address + 171756, v); }
  uint32_t unknown_171756_address() const { return address + 171756; }
  float unknown_171760() const { return guest::LoadFloat(base, address + 171760); }
  void set_unknown_171760(float v) const { guest::StoreFloat(base, address + 171760, v); }
  uint32_t unknown_171760_address() const { return address + 171760; }
  float unknown_171764() const { return guest::LoadFloat(base, address + 171764); }
  void set_unknown_171764(float v) const { guest::StoreFloat(base, address + 171764, v); }
  uint32_t unknown_171764_address() const { return address + 171764; }
  float unknown_171768() const { return guest::LoadFloat(base, address + 171768); }
  void set_unknown_171768(float v) const { guest::StoreFloat(base, address + 171768, v); }
  uint32_t unknown_171768_address() const { return address + 171768; }
  uint32_t unknown_171772() const { return guest::LoadU32(base, address + 171772); }
  void set_unknown_171772(uint32_t v) const { guest::StoreU32(base, address + 171772, v); }
  uint32_t unknown_171772_address() const { return address + 171772; }
  float unknown_171776() const { return guest::LoadFloat(base, address + 171776); }
  void set_unknown_171776(float v) const { guest::StoreFloat(base, address + 171776, v); }
  uint32_t unknown_171776_address() const { return address + 171776; }
  float unknown_171780() const { return guest::LoadFloat(base, address + 171780); }
  void set_unknown_171780(float v) const { guest::StoreFloat(base, address + 171780, v); }
  uint32_t unknown_171780_address() const { return address + 171780; }
  uint32_t unknown_171784() const { return guest::LoadU32(base, address + 171784); }
  void set_unknown_171784(uint32_t v) const { guest::StoreU32(base, address + 171784, v); }
  uint32_t unknown_171784_address() const { return address + 171784; }
  float unknown_171788() const { return guest::LoadFloat(base, address + 171788); }
  void set_unknown_171788(float v) const { guest::StoreFloat(base, address + 171788, v); }
  uint32_t unknown_171788_address() const { return address + 171788; }
  float unknown_171792() const { return guest::LoadFloat(base, address + 171792); }
  void set_unknown_171792(float v) const { guest::StoreFloat(base, address + 171792, v); }
  uint32_t unknown_171792_address() const { return address + 171792; }
  uint32_t unknown_171796() const { return guest::LoadU32(base, address + 171796); }
  void set_unknown_171796(uint32_t v) const { guest::StoreU32(base, address + 171796, v); }
  uint32_t unknown_171796_address() const { return address + 171796; }
  float unknown_171800() const { return guest::LoadFloat(base, address + 171800); }
  void set_unknown_171800(float v) const { guest::StoreFloat(base, address + 171800, v); }
  uint32_t unknown_171800_address() const { return address + 171800; }
  uint32_t unknown_188188() const { return guest::LoadU32(base, address + 188188); }
  void set_unknown_188188(uint32_t v) const { guest::StoreU32(base, address + 188188, v); }
  uint32_t unknown_188188_address() const { return address + 188188; }
  float unknown_188192() const { return guest::LoadFloat(base, address + 188192); }
  void set_unknown_188192(float v) const { guest::StoreFloat(base, address + 188192, v); }
  uint32_t unknown_188192_address() const { return address + 188192; }
  float unknown_188196() const { return guest::LoadFloat(base, address + 188196); }
  void set_unknown_188196(float v) const { guest::StoreFloat(base, address + 188196, v); }
  uint32_t unknown_188196_address() const { return address + 188196; }
  float unknown_188200() const { return guest::LoadFloat(base, address + 188200); }
  void set_unknown_188200(float v) const { guest::StoreFloat(base, address + 188200, v); }
  uint32_t unknown_188200_address() const { return address + 188200; }
  float unknown_188204() const { return guest::LoadFloat(base, address + 188204); }
  void set_unknown_188204(float v) const { guest::StoreFloat(base, address + 188204, v); }
  uint32_t unknown_188204_address() const { return address + 188204; }
  float unknown_188208() const { return guest::LoadFloat(base, address + 188208); }
  void set_unknown_188208(float v) const { guest::StoreFloat(base, address + 188208, v); }
  uint32_t unknown_188208_address() const { return address + 188208; }
  uint32_t unknown_188212() const { return guest::LoadU32(base, address + 188212); }
  void set_unknown_188212(uint32_t v) const { guest::StoreU32(base, address + 188212, v); }
  uint32_t unknown_188212_address() const { return address + 188212; }
  float unknown_188216() const { return guest::LoadFloat(base, address + 188216); }
  void set_unknown_188216(float v) const { guest::StoreFloat(base, address + 188216, v); }
  uint32_t unknown_188216_address() const { return address + 188216; }
  float unknown_188220() const { return guest::LoadFloat(base, address + 188220); }
  void set_unknown_188220(float v) const { guest::StoreFloat(base, address + 188220, v); }
  uint32_t unknown_188220_address() const { return address + 188220; }
  float unknown_188224() const { return guest::LoadFloat(base, address + 188224); }
  void set_unknown_188224(float v) const { guest::StoreFloat(base, address + 188224, v); }
  uint32_t unknown_188224_address() const { return address + 188224; }
  uint32_t unknown_188228() const { return guest::LoadU32(base, address + 188228); }
  void set_unknown_188228(uint32_t v) const { guest::StoreU32(base, address + 188228, v); }
  uint32_t unknown_188228_address() const { return address + 188228; }
  float unknown_188232() const { return guest::LoadFloat(base, address + 188232); }
  void set_unknown_188232(float v) const { guest::StoreFloat(base, address + 188232, v); }
  uint32_t unknown_188232_address() const { return address + 188232; }
  uint32_t unknown_190284() const { return guest::LoadU32(base, address + 190284); }
  void set_unknown_190284(uint32_t v) const { guest::StoreU32(base, address + 190284, v); }
  uint32_t unknown_190284_address() const { return address + 190284; }
  float unknown_190288() const { return guest::LoadFloat(base, address + 190288); }
  void set_unknown_190288(float v) const { guest::StoreFloat(base, address + 190288, v); }
  uint32_t unknown_190288_address() const { return address + 190288; }
  float unknown_190292() const { return guest::LoadFloat(base, address + 190292); }
  void set_unknown_190292(float v) const { guest::StoreFloat(base, address + 190292, v); }
  uint32_t unknown_190292_address() const { return address + 190292; }
  float unknown_190296() const { return guest::LoadFloat(base, address + 190296); }
  void set_unknown_190296(float v) const { guest::StoreFloat(base, address + 190296, v); }
  uint32_t unknown_190296_address() const { return address + 190296; }
  uint32_t unknown_190300() const { return guest::LoadU32(base, address + 190300); }
  void set_unknown_190300(uint32_t v) const { guest::StoreU32(base, address + 190300, v); }
  uint32_t unknown_190300_address() const { return address + 190300; }
  float unknown_190304() const { return guest::LoadFloat(base, address + 190304); }
  void set_unknown_190304(float v) const { guest::StoreFloat(base, address + 190304, v); }
  uint32_t unknown_190304_address() const { return address + 190304; }
  uint32_t unknown_192356() const { return guest::LoadU32(base, address + 192356); }
  void set_unknown_192356(uint32_t v) const { guest::StoreU32(base, address + 192356, v); }
  uint32_t unknown_192356_address() const { return address + 192356; }
  float unknown_192360() const { return guest::LoadFloat(base, address + 192360); }
  void set_unknown_192360(float v) const { guest::StoreFloat(base, address + 192360, v); }
  uint32_t unknown_192360_address() const { return address + 192360; }
  float unknown_192364() const { return guest::LoadFloat(base, address + 192364); }
  void set_unknown_192364(float v) const { guest::StoreFloat(base, address + 192364, v); }
  uint32_t unknown_192364_address() const { return address + 192364; }
  float unknown_192368() const { return guest::LoadFloat(base, address + 192368); }
  void set_unknown_192368(float v) const { guest::StoreFloat(base, address + 192368, v); }
  uint32_t unknown_192368_address() const { return address + 192368; }
  float unknown_192372() const { return guest::LoadFloat(base, address + 192372); }
  void set_unknown_192372(float v) const { guest::StoreFloat(base, address + 192372, v); }
  uint32_t unknown_192372_address() const { return address + 192372; }
  float unknown_192376() const { return guest::LoadFloat(base, address + 192376); }
  void set_unknown_192376(float v) const { guest::StoreFloat(base, address + 192376, v); }
  uint32_t unknown_192376_address() const { return address + 192376; }
  uint32_t unknown_192380() const { return guest::LoadU32(base, address + 192380); }
  void set_unknown_192380(uint32_t v) const { guest::StoreU32(base, address + 192380, v); }
  uint32_t unknown_192380_address() const { return address + 192380; }
  float unknown_192384() const { return guest::LoadFloat(base, address + 192384); }
  void set_unknown_192384(float v) const { guest::StoreFloat(base, address + 192384, v); }
  uint32_t unknown_192384_address() const { return address + 192384; }
  float unknown_192388() const { return guest::LoadFloat(base, address + 192388); }
  void set_unknown_192388(float v) const { guest::StoreFloat(base, address + 192388, v); }
  uint32_t unknown_192388_address() const { return address + 192388; }
  float unknown_192392() const { return guest::LoadFloat(base, address + 192392); }
  void set_unknown_192392(float v) const { guest::StoreFloat(base, address + 192392, v); }
  uint32_t unknown_192392_address() const { return address + 192392; }
  float unknown_192396() const { return guest::LoadFloat(base, address + 192396); }
  void set_unknown_192396(float v) const { guest::StoreFloat(base, address + 192396, v); }
  uint32_t unknown_192396_address() const { return address + 192396; }
  float unknown_192400() const { return guest::LoadFloat(base, address + 192400); }
  void set_unknown_192400(float v) const { guest::StoreFloat(base, address + 192400, v); }
  uint32_t unknown_192400_address() const { return address + 192400; }
  float unknown_192404() const { return guest::LoadFloat(base, address + 192404); }
  void set_unknown_192404(float v) const { guest::StoreFloat(base, address + 192404, v); }
  uint32_t unknown_192404_address() const { return address + 192404; }
  float unknown_192408() const { return guest::LoadFloat(base, address + 192408); }
  void set_unknown_192408(float v) const { guest::StoreFloat(base, address + 192408, v); }
  uint32_t unknown_192408_address() const { return address + 192408; }
  uint32_t unknown_192412() const { return guest::LoadU32(base, address + 192412); }
  void set_unknown_192412(uint32_t v) const { guest::StoreU32(base, address + 192412, v); }
  uint32_t unknown_192412_address() const { return address + 192412; }
  float unknown_192416() const { return guest::LoadFloat(base, address + 192416); }
  void set_unknown_192416(float v) const { guest::StoreFloat(base, address + 192416, v); }
  uint32_t unknown_192416_address() const { return address + 192416; }
  float unknown_192420() const { return guest::LoadFloat(base, address + 192420); }
  void set_unknown_192420(float v) const { guest::StoreFloat(base, address + 192420, v); }
  uint32_t unknown_192420_address() const { return address + 192420; }
  float unknown_192424() const { return guest::LoadFloat(base, address + 192424); }
  void set_unknown_192424(float v) const { guest::StoreFloat(base, address + 192424, v); }
  uint32_t unknown_192424_address() const { return address + 192424; }
  float unknown_192428() const { return guest::LoadFloat(base, address + 192428); }
  void set_unknown_192428(float v) const { guest::StoreFloat(base, address + 192428, v); }
  uint32_t unknown_192428_address() const { return address + 192428; }
  float unknown_192432() const { return guest::LoadFloat(base, address + 192432); }
  void set_unknown_192432(float v) const { guest::StoreFloat(base, address + 192432, v); }
  uint32_t unknown_192432_address() const { return address + 192432; }
  float unknown_192436() const { return guest::LoadFloat(base, address + 192436); }
  void set_unknown_192436(float v) const { guest::StoreFloat(base, address + 192436, v); }
  uint32_t unknown_192436_address() const { return address + 192436; }
  uint32_t unknown_192440() const { return guest::LoadU32(base, address + 192440); }
  void set_unknown_192440(uint32_t v) const { guest::StoreU32(base, address + 192440, v); }
  uint32_t unknown_192440_address() const { return address + 192440; }
  float unknown_192444() const { return guest::LoadFloat(base, address + 192444); }
  void set_unknown_192444(float v) const { guest::StoreFloat(base, address + 192444, v); }
  uint32_t unknown_192444_address() const { return address + 192444; }
  float unknown_192448() const { return guest::LoadFloat(base, address + 192448); }
  void set_unknown_192448(float v) const { guest::StoreFloat(base, address + 192448, v); }
  uint32_t unknown_192448_address() const { return address + 192448; }
  uint32_t unknown_192452() const { return guest::LoadU32(base, address + 192452); }
  void set_unknown_192452(uint32_t v) const { guest::StoreU32(base, address + 192452, v); }
  uint32_t unknown_192452_address() const { return address + 192452; }
  float unknown_192456() const { return guest::LoadFloat(base, address + 192456); }
  void set_unknown_192456(float v) const { guest::StoreFloat(base, address + 192456, v); }
  uint32_t unknown_192456_address() const { return address + 192456; }
  float unknown_192460() const { return guest::LoadFloat(base, address + 192460); }
  void set_unknown_192460(float v) const { guest::StoreFloat(base, address + 192460, v); }
  uint32_t unknown_192460_address() const { return address + 192460; }
  uint32_t unknown_192464() const { return guest::LoadU32(base, address + 192464); }
  void set_unknown_192464(uint32_t v) const { guest::StoreU32(base, address + 192464, v); }
  uint32_t unknown_192464_address() const { return address + 192464; }
  float unknown_192468() const { return guest::LoadFloat(base, address + 192468); }
  void set_unknown_192468(float v) const { guest::StoreFloat(base, address + 192468, v); }
  uint32_t unknown_192468_address() const { return address + 192468; }
  uint32_t unknown_208856() const { return guest::LoadU32(base, address + 208856); }
  void set_unknown_208856(uint32_t v) const { guest::StoreU32(base, address + 208856, v); }
  uint32_t unknown_208856_address() const { return address + 208856; }
  float unknown_208860() const { return guest::LoadFloat(base, address + 208860); }
  void set_unknown_208860(float v) const { guest::StoreFloat(base, address + 208860, v); }
  uint32_t unknown_208860_address() const { return address + 208860; }
  float unknown_208864() const { return guest::LoadFloat(base, address + 208864); }
  void set_unknown_208864(float v) const { guest::StoreFloat(base, address + 208864, v); }
  uint32_t unknown_208864_address() const { return address + 208864; }
  float unknown_208868() const { return guest::LoadFloat(base, address + 208868); }
  void set_unknown_208868(float v) const { guest::StoreFloat(base, address + 208868, v); }
  uint32_t unknown_208868_address() const { return address + 208868; }
  float unknown_208872() const { return guest::LoadFloat(base, address + 208872); }
  void set_unknown_208872(float v) const { guest::StoreFloat(base, address + 208872, v); }
  uint32_t unknown_208872_address() const { return address + 208872; }
  float unknown_208876() const { return guest::LoadFloat(base, address + 208876); }
  void set_unknown_208876(float v) const { guest::StoreFloat(base, address + 208876, v); }
  uint32_t unknown_208876_address() const { return address + 208876; }
  uint32_t unknown_208880() const { return guest::LoadU32(base, address + 208880); }
  void set_unknown_208880(uint32_t v) const { guest::StoreU32(base, address + 208880, v); }
  uint32_t unknown_208880_address() const { return address + 208880; }
  float unknown_208884() const { return guest::LoadFloat(base, address + 208884); }
  void set_unknown_208884(float v) const { guest::StoreFloat(base, address + 208884, v); }
  uint32_t unknown_208884_address() const { return address + 208884; }
  float unknown_208888() const { return guest::LoadFloat(base, address + 208888); }
  void set_unknown_208888(float v) const { guest::StoreFloat(base, address + 208888, v); }
  uint32_t unknown_208888_address() const { return address + 208888; }
  float unknown_208892() const { return guest::LoadFloat(base, address + 208892); }
  void set_unknown_208892(float v) const { guest::StoreFloat(base, address + 208892, v); }
  uint32_t unknown_208892_address() const { return address + 208892; }
  uint32_t unknown_208896() const { return guest::LoadU32(base, address + 208896); }
  void set_unknown_208896(uint32_t v) const { guest::StoreU32(base, address + 208896, v); }
  uint32_t unknown_208896_address() const { return address + 208896; }
  float unknown_208900() const { return guest::LoadFloat(base, address + 208900); }
  void set_unknown_208900(float v) const { guest::StoreFloat(base, address + 208900, v); }
  uint32_t unknown_208900_address() const { return address + 208900; }
  uint32_t unknown_217096() const { return guest::LoadU32(base, address + 217096); }
  void set_unknown_217096(uint32_t v) const { guest::StoreU32(base, address + 217096, v); }
  uint32_t unknown_217096_address() const { return address + 217096; }
  float unknown_217100() const { return guest::LoadFloat(base, address + 217100); }
  void set_unknown_217100(float v) const { guest::StoreFloat(base, address + 217100, v); }
  uint32_t unknown_217100_address() const { return address + 217100; }
  float unknown_217104() const { return guest::LoadFloat(base, address + 217104); }
  void set_unknown_217104(float v) const { guest::StoreFloat(base, address + 217104, v); }
  uint32_t unknown_217104_address() const { return address + 217104; }
  float unknown_217108() const { return guest::LoadFloat(base, address + 217108); }
  void set_unknown_217108(float v) const { guest::StoreFloat(base, address + 217108, v); }
  uint32_t unknown_217108_address() const { return address + 217108; }
  float unknown_217112() const { return guest::LoadFloat(base, address + 217112); }
  void set_unknown_217112(float v) const { guest::StoreFloat(base, address + 217112, v); }
  uint32_t unknown_217112_address() const { return address + 217112; }
  uint32_t unknown_217116() const { return guest::LoadU32(base, address + 217116); }
  void set_unknown_217116(uint32_t v) const { guest::StoreU32(base, address + 217116, v); }
  uint32_t unknown_217116_address() const { return address + 217116; }
  float unknown_217120() const { return guest::LoadFloat(base, address + 217120); }
  void set_unknown_217120(float v) const { guest::StoreFloat(base, address + 217120, v); }
  uint32_t unknown_217120_address() const { return address + 217120; }
  uint32_t unknown_225316() const { return guest::LoadU32(base, address + 225316); }
  void set_unknown_225316(uint32_t v) const { guest::StoreU32(base, address + 225316, v); }
  uint32_t unknown_225316_address() const { return address + 225316; }
  float unknown_225320() const { return guest::LoadFloat(base, address + 225320); }
  void set_unknown_225320(float v) const { guest::StoreFloat(base, address + 225320, v); }
  uint32_t unknown_225320_address() const { return address + 225320; }
  float unknown_225324() const { return guest::LoadFloat(base, address + 225324); }
  void set_unknown_225324(float v) const { guest::StoreFloat(base, address + 225324, v); }
  uint32_t unknown_225324_address() const { return address + 225324; }
  float unknown_225328() const { return guest::LoadFloat(base, address + 225328); }
  void set_unknown_225328(float v) const { guest::StoreFloat(base, address + 225328, v); }
  uint32_t unknown_225328_address() const { return address + 225328; }
  uint32_t unknown_225332() const { return guest::LoadU32(base, address + 225332); }
  void set_unknown_225332(uint32_t v) const { guest::StoreU32(base, address + 225332, v); }
  uint32_t unknown_225332_address() const { return address + 225332; }
  float unknown_225336() const { return guest::LoadFloat(base, address + 225336); }
  void set_unknown_225336(float v) const { guest::StoreFloat(base, address + 225336, v); }
  uint32_t unknown_225336_address() const { return address + 225336; }
  uint32_t unknown_233532() const { return guest::LoadU32(base, address + 233532); }
  void set_unknown_233532(uint32_t v) const { guest::StoreU32(base, address + 233532, v); }
  uint32_t unknown_233532_address() const { return address + 233532; }
  float unknown_233536() const { return guest::LoadFloat(base, address + 233536); }
  void set_unknown_233536(float v) const { guest::StoreFloat(base, address + 233536, v); }
  uint32_t unknown_233536_address() const { return address + 233536; }
  float unknown_233540() const { return guest::LoadFloat(base, address + 233540); }
  void set_unknown_233540(float v) const { guest::StoreFloat(base, address + 233540, v); }
  uint32_t unknown_233540_address() const { return address + 233540; }
  float unknown_233544() const { return guest::LoadFloat(base, address + 233544); }
  void set_unknown_233544(float v) const { guest::StoreFloat(base, address + 233544, v); }
  uint32_t unknown_233544_address() const { return address + 233544; }
  uint32_t unknown_233548() const { return guest::LoadU32(base, address + 233548); }
  void set_unknown_233548(uint32_t v) const { guest::StoreU32(base, address + 233548, v); }
  uint32_t unknown_233548_address() const { return address + 233548; }
  float unknown_233552() const { return guest::LoadFloat(base, address + 233552); }
  void set_unknown_233552(float v) const { guest::StoreFloat(base, address + 233552, v); }
  uint32_t unknown_233552_address() const { return address + 233552; }
  uint32_t unknown_237652() const { return guest::LoadU32(base, address + 237652); }
  void set_unknown_237652(uint32_t v) const { guest::StoreU32(base, address + 237652, v); }
  uint32_t unknown_237652_address() const { return address + 237652; }
  float unknown_237656() const { return guest::LoadFloat(base, address + 237656); }
  void set_unknown_237656(float v) const { guest::StoreFloat(base, address + 237656, v); }
  uint32_t unknown_237656_address() const { return address + 237656; }
  float unknown_237660() const { return guest::LoadFloat(base, address + 237660); }
  void set_unknown_237660(float v) const { guest::StoreFloat(base, address + 237660, v); }
  uint32_t unknown_237660_address() const { return address + 237660; }
  uint32_t unknown_237664() const { return guest::LoadU32(base, address + 237664); }
  void set_unknown_237664(uint32_t v) const { guest::StoreU32(base, address + 237664, v); }
  uint32_t unknown_237664_address() const { return address + 237664; }
  float unknown_237668() const { return guest::LoadFloat(base, address + 237668); }
  void set_unknown_237668(float v) const { guest::StoreFloat(base, address + 237668, v); }
  uint32_t unknown_237668_address() const { return address + 237668; }
  float unknown_237672() const { return guest::LoadFloat(base, address + 237672); }
  void set_unknown_237672(float v) const { guest::StoreFloat(base, address + 237672, v); }
  uint32_t unknown_237672_address() const { return address + 237672; }
  uint32_t unknown_237676() const { return guest::LoadU32(base, address + 237676); }
  void set_unknown_237676(uint32_t v) const { guest::StoreU32(base, address + 237676, v); }
  uint32_t unknown_237676_address() const { return address + 237676; }
  float unknown_237680() const { return guest::LoadFloat(base, address + 237680); }
  void set_unknown_237680(float v) const { guest::StoreFloat(base, address + 237680, v); }
  uint32_t unknown_237680_address() const { return address + 237680; }
  uint32_t unknown_254068() const { return guest::LoadU32(base, address + 254068); }
  void set_unknown_254068(uint32_t v) const { guest::StoreU32(base, address + 254068, v); }
  uint32_t unknown_254068_address() const { return address + 254068; }
  float unknown_254072() const { return guest::LoadFloat(base, address + 254072); }
  void set_unknown_254072(float v) const { guest::StoreFloat(base, address + 254072, v); }
  uint32_t unknown_254072_address() const { return address + 254072; }
  float unknown_254076() const { return guest::LoadFloat(base, address + 254076); }
  void set_unknown_254076(float v) const { guest::StoreFloat(base, address + 254076, v); }
  uint32_t unknown_254076_address() const { return address + 254076; }
  float unknown_254080() const { return guest::LoadFloat(base, address + 254080); }
  void set_unknown_254080(float v) const { guest::StoreFloat(base, address + 254080, v); }
  uint32_t unknown_254080_address() const { return address + 254080; }
  float unknown_254084() const { return guest::LoadFloat(base, address + 254084); }
  void set_unknown_254084(float v) const { guest::StoreFloat(base, address + 254084, v); }
  uint32_t unknown_254084_address() const { return address + 254084; }
  float unknown_254088() const { return guest::LoadFloat(base, address + 254088); }
  void set_unknown_254088(float v) const { guest::StoreFloat(base, address + 254088, v); }
  uint32_t unknown_254088_address() const { return address + 254088; }
  float unknown_254092() const { return guest::LoadFloat(base, address + 254092); }
  void set_unknown_254092(float v) const { guest::StoreFloat(base, address + 254092, v); }
  uint32_t unknown_254092_address() const { return address + 254092; }
  uint32_t unknown_254096() const { return guest::LoadU32(base, address + 254096); }
  void set_unknown_254096(uint32_t v) const { guest::StoreU32(base, address + 254096, v); }
  uint32_t unknown_254096_address() const { return address + 254096; }
  float unknown_254100() const { return guest::LoadFloat(base, address + 254100); }
  void set_unknown_254100(float v) const { guest::StoreFloat(base, address + 254100, v); }
  uint32_t unknown_254100_address() const { return address + 254100; }
  float unknown_254104() const { return guest::LoadFloat(base, address + 254104); }
  void set_unknown_254104(float v) const { guest::StoreFloat(base, address + 254104, v); }
  uint32_t unknown_254104_address() const { return address + 254104; }
  float unknown_254108() const { return guest::LoadFloat(base, address + 254108); }
  void set_unknown_254108(float v) const { guest::StoreFloat(base, address + 254108, v); }
  uint32_t unknown_254108_address() const { return address + 254108; }
  uint32_t unknown_254112() const { return guest::LoadU32(base, address + 254112); }
  void set_unknown_254112(uint32_t v) const { guest::StoreU32(base, address + 254112, v); }
  uint32_t unknown_254112_address() const { return address + 254112; }
  float unknown_254116() const { return guest::LoadFloat(base, address + 254116); }
  void set_unknown_254116(float v) const { guest::StoreFloat(base, address + 254116, v); }
  uint32_t unknown_254116_address() const { return address + 254116; }
  uint32_t unknown_262312() const { return guest::LoadU32(base, address + 262312); }
  void set_unknown_262312(uint32_t v) const { guest::StoreU32(base, address + 262312, v); }
  uint32_t unknown_262312_address() const { return address + 262312; }
  float unknown_262316() const { return guest::LoadFloat(base, address + 262316); }
  void set_unknown_262316(float v) const { guest::StoreFloat(base, address + 262316, v); }
  uint32_t unknown_262316_address() const { return address + 262316; }
  float unknown_262320() const { return guest::LoadFloat(base, address + 262320); }
  void set_unknown_262320(float v) const { guest::StoreFloat(base, address + 262320, v); }
  uint32_t unknown_262320_address() const { return address + 262320; }
  float unknown_262324() const { return guest::LoadFloat(base, address + 262324); }
  void set_unknown_262324(float v) const { guest::StoreFloat(base, address + 262324, v); }
  uint32_t unknown_262324_address() const { return address + 262324; }
  float unknown_262328() const { return guest::LoadFloat(base, address + 262328); }
  void set_unknown_262328(float v) const { guest::StoreFloat(base, address + 262328, v); }
  uint32_t unknown_262328_address() const { return address + 262328; }
  uint32_t unknown_262332() const { return guest::LoadU32(base, address + 262332); }
  void set_unknown_262332(uint32_t v) const { guest::StoreU32(base, address + 262332, v); }
  uint32_t unknown_262332_address() const { return address + 262332; }
  float unknown_262336() const { return guest::LoadFloat(base, address + 262336); }
  void set_unknown_262336(float v) const { guest::StoreFloat(base, address + 262336, v); }
  uint32_t unknown_262336_address() const { return address + 262336; }
  uint32_t unknown_270532() const { return guest::LoadU32(base, address + 270532); }
  void set_unknown_270532(uint32_t v) const { guest::StoreU32(base, address + 270532, v); }
  uint32_t unknown_270532_address() const { return address + 270532; }
  float unknown_270536() const { return guest::LoadFloat(base, address + 270536); }
  void set_unknown_270536(float v) const { guest::StoreFloat(base, address + 270536, v); }
  uint32_t unknown_270536_address() const { return address + 270536; }
  float unknown_270540() const { return guest::LoadFloat(base, address + 270540); }
  void set_unknown_270540(float v) const { guest::StoreFloat(base, address + 270540, v); }
  uint32_t unknown_270540_address() const { return address + 270540; }
  float unknown_270544() const { return guest::LoadFloat(base, address + 270544); }
  void set_unknown_270544(float v) const { guest::StoreFloat(base, address + 270544, v); }
  uint32_t unknown_270544_address() const { return address + 270544; }
  uint32_t unknown_270548() const { return guest::LoadU32(base, address + 270548); }
  void set_unknown_270548(uint32_t v) const { guest::StoreU32(base, address + 270548, v); }
  uint32_t unknown_270548_address() const { return address + 270548; }
  float unknown_270552() const { return guest::LoadFloat(base, address + 270552); }
  void set_unknown_270552(float v) const { guest::StoreFloat(base, address + 270552, v); }
  uint32_t unknown_270552_address() const { return address + 270552; }
  uint32_t unknown_278748() const { return guest::LoadU32(base, address + 278748); }
  void set_unknown_278748(uint32_t v) const { guest::StoreU32(base, address + 278748, v); }
  uint32_t unknown_278748_address() const { return address + 278748; }
  float unknown_278752() const { return guest::LoadFloat(base, address + 278752); }
  void set_unknown_278752(float v) const { guest::StoreFloat(base, address + 278752, v); }
  uint32_t unknown_278752_address() const { return address + 278752; }
  float unknown_278756() const { return guest::LoadFloat(base, address + 278756); }
  void set_unknown_278756(float v) const { guest::StoreFloat(base, address + 278756, v); }
  uint32_t unknown_278756_address() const { return address + 278756; }
  float unknown_278760() const { return guest::LoadFloat(base, address + 278760); }
  void set_unknown_278760(float v) const { guest::StoreFloat(base, address + 278760, v); }
  uint32_t unknown_278760_address() const { return address + 278760; }
  uint32_t unknown_278764() const { return guest::LoadU32(base, address + 278764); }
  void set_unknown_278764(uint32_t v) const { guest::StoreU32(base, address + 278764, v); }
  uint32_t unknown_278764_address() const { return address + 278764; }
  float unknown_278768() const { return guest::LoadFloat(base, address + 278768); }
  void set_unknown_278768(float v) const { guest::StoreFloat(base, address + 278768, v); }
  uint32_t unknown_278768_address() const { return address + 278768; }
  uint32_t unknown_282868() const { return guest::LoadU32(base, address + 282868); }
  void set_unknown_282868(uint32_t v) const { guest::StoreU32(base, address + 282868, v); }
  uint32_t unknown_282868_address() const { return address + 282868; }
  float unknown_282872() const { return guest::LoadFloat(base, address + 282872); }
  void set_unknown_282872(float v) const { guest::StoreFloat(base, address + 282872, v); }
  uint32_t unknown_282872_address() const { return address + 282872; }
  float unknown_282876() const { return guest::LoadFloat(base, address + 282876); }
  void set_unknown_282876(float v) const { guest::StoreFloat(base, address + 282876, v); }
  uint32_t unknown_282876_address() const { return address + 282876; }
  uint32_t unknown_282880() const { return guest::LoadU32(base, address + 282880); }
  void set_unknown_282880(uint32_t v) const { guest::StoreU32(base, address + 282880, v); }
  uint32_t unknown_282880_address() const { return address + 282880; }
  float unknown_282884() const { return guest::LoadFloat(base, address + 282884); }
  void set_unknown_282884(float v) const { guest::StoreFloat(base, address + 282884, v); }
  uint32_t unknown_282884_address() const { return address + 282884; }
  float unknown_282888() const { return guest::LoadFloat(base, address + 282888); }
  void set_unknown_282888(float v) const { guest::StoreFloat(base, address + 282888, v); }
  uint32_t unknown_282888_address() const { return address + 282888; }
  uint32_t unknown_282892() const { return guest::LoadU32(base, address + 282892); }
  void set_unknown_282892(uint32_t v) const { guest::StoreU32(base, address + 282892, v); }
  uint32_t unknown_282892_address() const { return address + 282892; }
  float unknown_282896() const { return guest::LoadFloat(base, address + 282896); }
  void set_unknown_282896(float v) const { guest::StoreFloat(base, address + 282896, v); }
  uint32_t unknown_282896_address() const { return address + 282896; }
  uint32_t unknown_299284() const { return guest::LoadU32(base, address + 299284); }
  void set_unknown_299284(uint32_t v) const { guest::StoreU32(base, address + 299284, v); }
  uint32_t unknown_299284_address() const { return address + 299284; }
  float unknown_299288() const { return guest::LoadFloat(base, address + 299288); }
  void set_unknown_299288(float v) const { guest::StoreFloat(base, address + 299288, v); }
  uint32_t unknown_299288_address() const { return address + 299288; }
  float unknown_299292() const { return guest::LoadFloat(base, address + 299292); }
  void set_unknown_299292(float v) const { guest::StoreFloat(base, address + 299292, v); }
  uint32_t unknown_299292_address() const { return address + 299292; }
  float unknown_299296() const { return guest::LoadFloat(base, address + 299296); }
  void set_unknown_299296(float v) const { guest::StoreFloat(base, address + 299296, v); }
  uint32_t unknown_299296_address() const { return address + 299296; }
  float unknown_299300() const { return guest::LoadFloat(base, address + 299300); }
  void set_unknown_299300(float v) const { guest::StoreFloat(base, address + 299300, v); }
  uint32_t unknown_299300_address() const { return address + 299300; }
  float unknown_299304() const { return guest::LoadFloat(base, address + 299304); }
  void set_unknown_299304(float v) const { guest::StoreFloat(base, address + 299304, v); }
  uint32_t unknown_299304_address() const { return address + 299304; }
  float unknown_299308() const { return guest::LoadFloat(base, address + 299308); }
  void set_unknown_299308(float v) const { guest::StoreFloat(base, address + 299308, v); }
  uint32_t unknown_299308_address() const { return address + 299308; }
};

// allocated; measure: sub_825A5BF0.r3, sub_825A5CC0.r3
struct Effect5 : View {
  using View::View;
  static constexpr uint32_t kSize = 332;
  // probably
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
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
  uint32_t unknown_292() const { return guest::LoadU32(base, address + 292); }
  void set_unknown_292(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
};

// allocated; measure: sub_825A5020.r3
struct Effect6 : View {
  using View::View;
  static constexpr uint32_t kSize = 2092;
  // probably
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_2060() const { return guest::LoadU32(base, address + 2060); }
  void set_unknown_2060(uint32_t v) const { guest::StoreU32(base, address + 2060, v); }
  uint32_t unknown_2060_address() const { return address + 2060; }
};

// The image words docs/functions/xaudio-objects.md's code uses.
struct XaudioObjectsGlobals {
  // I3dl2::ToNative clamps the reflections delay below it
  static uint32_t reverb_reflections_delay_limit(uint8_t* base) { return guest::LoadU32(base, 0x8204ABFCu); }
  static void set_reverb_reflections_delay_limit(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8204ABFCu, v); }
  static constexpr uint32_t reverb_reflections_delay_limit_address() { return 0x8204ABFCu; }
  // I3dl2::ToNative clamps the reverb delay below it
  static uint32_t reverb_reverb_delay_limit(uint8_t* base) { return guest::LoadU32(base, 0x8204AC00u); }
  static void set_reverb_reverb_delay_limit(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8204AC00u, v); }
  static constexpr uint32_t reverb_reverb_delay_limit_address() { return 0x8204AC00u; }
  // I3dl2::ToNative's position left and right
  static uint32_t reverb_default_position(uint8_t* base) { return guest::LoadU32(base, 0x8204AC04u); }
  static void set_reverb_default_position(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8204AC04u, v); }
  static constexpr uint32_t reverb_default_position_address() { return 0x8204AC04u; }
  // I3dl2::ToNative's position matrix left and right
  static uint32_t reverb_default_position_matrix(uint8_t* base) { return guest::LoadU32(base, 0x8204AC08u); }
  static void set_reverb_default_position_matrix(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8204AC08u, v); }
  static constexpr uint32_t reverb_default_position_matrix_address() { return 0x8204AC08u; }
  static uint32_t reverb_default_rear_delay(uint8_t* base) { return guest::LoadU32(base, 0x8204AC0Cu); }
  static void set_reverb_default_rear_delay(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8204AC0Cu, v); }
  static constexpr uint32_t reverb_default_rear_delay_address() { return 0x8204AC0Cu; }
  // ReverbCore::Defaults's first float
  static float reverb_default_room_filter_freq(uint8_t* base) { return guest::LoadFloat(base, 0x8204AC10u); }
  static void set_reverb_default_room_filter_freq(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204AC10u, v); }
  static constexpr uint32_t reverb_default_room_filter_freq_address() { return 0x8204AC10u; }
  // 0.0 (probably): the room filter's, reflections' and reverb's default gains; FilterCoefficient's zero
  static float reverb_default_gain(uint8_t* base) { return guest::LoadFloat(base, 0x8204AC14u); }
  static void set_reverb_default_gain(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204AC14u, v); }
  static constexpr uint32_t reverb_default_gain_address() { return 0x8204AC14u; }
  // 1.0 (probably; FilterCoefficient's one)
  static float reverb_default_decay_time(uint8_t* base) { return guest::LoadFloat(base, 0x8204AC18u); }
  static void set_reverb_default_decay_time(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204AC18u, v); }
  static constexpr uint32_t reverb_default_decay_time_address() { return 0x8204AC18u; }
  // also the room size
  static float reverb_default_density(uint8_t* base) { return guest::LoadFloat(base, 0x8204AC1Cu); }
  static void set_reverb_default_density(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204AC1Cu, v); }
  static constexpr uint32_t reverb_default_density_address() { return 0x8204AC1Cu; }
  // Reverb::SetParameters's room size scale
  static float reverb_room_size_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8204AA80u); }
  static void set_reverb_room_size_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204AA80u, v); }
  static constexpr uint32_t reverb_room_size_scale_address() { return 0x8204AA80u; }
  // FilterCoefficient: a gain at or above it skips the cos
  static float filter_limit(uint8_t* base) { return guest::LoadFloat(base, 0x8204B8B0u); }
  static void set_filter_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204B8B0u, v); }
  static constexpr uint32_t filter_limit_address() { return 0x8204B8B0u; }
  // probably (FilterCoefficient's angle)
  static double two_pi(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8204B8B8u)); }
  static void set_two_pi(uint8_t* base, double v) { guest::StoreU64(base, 0x8204B8B8u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t two_pi_address() { return 0x8204B8B8u; }
  // the decay HF ratio's step scale below 1
  static double decay_scale_low(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8204B938u)); }
  static void set_decay_scale_low(uint8_t* base, double v) { guest::StoreU64(base, 0x8204B938u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t decay_scale_low_address() { return 0x8204B938u; }
  // the decay HF ratio's step scale from 1
  static double decay_scale_high(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8204B940u)); }
  static void set_decay_scale_high(uint8_t* base, double v) { guest::StoreU64(base, 0x8204B940u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t decay_scale_high_address() { return 0x8204B940u; }
  // 10: the codecs' info half-word and the stop's fade passes
  static uint16_t codec_info_halfword(uint8_t* base) { return guest::LoadU16(base, 0x8204B950u); }
  static void set_codec_info_halfword(uint8_t* base, uint16_t v) { guest::StoreU16(base, 0x8204B950u, v); }
  static constexpr uint32_t codec_info_halfword_address() { return 0x8204B950u; }
  // 0.5: a stopping codec's volume each pass
  static float fade_step(uint8_t* base) { return guest::LoadFloat(base, 0x8204B954u); }
  static void set_fade_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x8204B954u, v); }
  static constexpr uint32_t fade_step_address() { return 0x8204B954u; }
  // bytes a sample by the format type (words: 4, 2, 1, 2)
  static constexpr uint32_t sample_bytes_address() { return 0x8204BBC0u; }
  // LargestPrimeBelow's 1,229 primes (words)
  static constexpr uint32_t primes_address() { return 0x8204BBD0u; }
  // Resampler::Process's 100 kernels (words)
  static constexpr uint32_t resampler_kernels_address() { return 0x8204CF08u; }
  // VolumeMeter::Process: samples past it counted
  static float meter_threshold(uint8_t* base) { return guest::LoadFloat(base, 0x82072298u); }
  static void set_meter_threshold(uint8_t* base, float v) { guest::StoreFloat(base, 0x82072298u, v); }
  static constexpr uint32_t meter_threshold_address() { return 0x82072298u; }
};

}  // namespace schema
