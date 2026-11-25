// The views of docs/functions/xaudio.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/xaudio.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: XAudioVoice, sub_8245FB00.r3, XAudioVoice.+0, XAudioVoice.+24.+0
struct XAudioVoice : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  // 0x8204A6CC then 0x8204AA00 (the base), 0x8204A998 (with outputs), 0x8204A720 source, 0x8204A870 submix; the root's 0x8204A6B8 once destroyed
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: XAudioVoice::BaseSize's r3 is the voice description (+1 its channels)
  uint8_t desc_channels() const { return guest::LoadU8(address + 1); }
  void set_desc_channels(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t desc_channels_address() const { return address + 1; }
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  template <typename T = XAudioBumpAllocator>
  T allocator() const { return T(guest::LoadU32(address + 8)); }
  void set_allocator(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_allocator(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t allocator_address() const { return address + 8; }
  // 0 source, 1 submix, 2 mastering
  uint8_t type() const { return guest::LoadU8(address + 12); }
  void set_type(uint8_t v) const { guest::StoreU8(address + 12, v); }
  uint32_t type_address() const { return address + 12; }
  // measure fault: the vtable's slot +12 (XAudioObject::Free), read through the voice; also BaseSize's description +12 (the buffer)
  uint32_t vt_free() const { return guest::LoadU32(address + 12); }
  void set_vt_free(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t vt_free_address() const { return address + 12; }
  // the voice's node in its stage's list (the stage's +8 says 16)
  uint32_t node_next() const { return guest::LoadU32(address + 16); }
  void set_node_next(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t node_next_address() const { return address + 16; }
  uint32_t node_prev() const { return guest::LoadU32(address + 20); }
  void set_node_prev(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t node_prev_address() const { return address + 20; }
  // the voice's node in the active list (+40 of its stage)
  uint32_t active_next() const { return guest::LoadU32(address + 24); }
  void set_active_next(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t active_next_address() const { return address + 24; }
  // measure fault: OutputsSize's r3 is the voice description (+24 the outputs)
  uint8_t desc_output_count() const { return guest::LoadU8(address + 24); }
  void set_desc_output_count(uint8_t v) const { guest::StoreU8(address + 24, v); }
  uint32_t desc_output_count_address() const { return address + 24; }
  // measure fault: OutputsSize's description (+25 the outputs' channels)
  uint8_t desc_output_channels() const { return guest::LoadU8(address + 25); }
  void set_desc_output_channels(uint8_t v) const { guest::StoreU8(address + 25, v); }
  uint32_t desc_output_channels_address() const { return address + 25; }
  uint32_t active_prev() const { return guest::LoadU32(address + 28); }
  void set_active_prev(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t active_prev_address() const { return address + 28; }
  // the output buffer's interface (the buffer + 8)
  uint32_t buffer() const { return guest::LoadU32(address + 32); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t buffer_address() const { return address + 32; }
  // the effect entries (XAudioEffectEntry, 8 bytes each)
  uint32_t effects() const { return guest::LoadU32(address + 36); }
  void set_effects(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t effects_address() const { return address + 36; }
  // how many voices output to this one while playing
  uint32_t output_uses() const { return guest::LoadU32(address + 40); }
  void set_output_uses(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t output_uses_address() const { return address + 40; }
  // called every pass with a pointer to its context
  uint32_t callback() const { return guest::LoadU32(address + 44); }
  void set_callback(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t callback_address() const { return address + 44; }
  uint32_t callback_context() const { return guest::LoadU32(address + 48); }
  void set_callback_context(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t callback_context_address() const { return address + 48; }
  // the voice's format (8 bytes, XAudioVoiceFormat): its first word
  uint32_t format() const { return guest::LoadU32(address + 52); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t format_address() const { return address + 52; }
  uint8_t format_type() const { return guest::LoadU8(address + 52); }
  void set_format_type(uint8_t v) const { guest::StoreU8(address + 52, v); }
  uint32_t format_type_address() const { return address + 52; }
  uint8_t channels() const { return guest::LoadU8(address + 53); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 53, v); }
  uint32_t channels_address() const { return address + 53; }
  // 48,000
  uint32_t rate() const { return guest::LoadU32(address + 56); }
  void set_rate(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t rate_address() const { return address + 56; }
  uint8_t effect_count() const { return guest::LoadU8(address + 60); }
  void set_effect_count(uint8_t v) const { guest::StoreU8(address + 60, v); }
  uint32_t effect_count_address() const { return address + 60; }
  // measure fault: the vtable's slot +60 (stop), read through the voice
  uint32_t vt_stop() const { return guest::LoadU32(address + 60); }
  void set_vt_stop(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t vt_stop_address() const { return address + 60; }
  // bit 0 playing, bit 2 stopping (the tail), bit 6 a start pending
  uint8_t flags() const { return guest::LoadU8(address + 61); }
  void set_flags(uint8_t v) const { guest::StoreU8(address + 61, v); }
  uint32_t flags_address() const { return address + 61; }
  // the effects' tail in passes (the largest effect's)
  uint16_t tail() const { return guest::LoadU16(address + 62); }
  void set_tail(uint16_t v) const { guest::StoreU16(address + 62, v); }
  uint32_t tail_address() const { return address + 62; }
  // passes left of the tail while stopping
  uint16_t tail_left() const { return guest::LoadU16(address + 64); }
  void set_tail_left(uint16_t v) const { guest::StoreU16(address + 64, v); }
  uint32_t tail_left_address() const { return address + 64; }
  // the outputs allowed (the description's +24)
  uint8_t output_slots() const { return guest::LoadU8(address + 68); }
  void set_output_slots(uint8_t v) const { guest::StoreU8(address + 68, v); }
  uint32_t output_slots_address() const { return address + 68; }
  // the outputs set
  uint8_t output_count() const { return guest::LoadU8(address + 69); }
  void set_output_count(uint8_t v) const { guest::StoreU8(address + 69, v); }
  uint32_t output_count_address() const { return address + 69; }
  // 12 bytes an output; also the vtable's slot +72 (the input buffer) read through the voice
  template <typename T = XAudioOutputSlot>
  T outputs() const { return T(guest::LoadU32(address + 72)); }
  void set_outputs(const View& v) const { guest::StoreU32(address + 72, v.address); }
  void set_outputs(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t outputs_address() const { return address + 72; }
  // a submix voice's stage (its list in the engine's +124)
  uint8_t stage() const { return guest::LoadU8(address + 76); }
  void set_stage(uint8_t v) const { guest::StoreU8(address + 76, v); }
  uint32_t stage_address() const { return address + 76; }
  // measure fault: the vtable's slot +76 (join the active list), read through the voice
  uint32_t vt_activate() const { return guest::LoadU32(address + 76); }
  void set_vt_activate(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t vt_activate_address() const { return address + 76; }
  // measure fault: the vtable's slot +80, read through the voice
  uint32_t vt_deactivate() const { return guest::LoadU32(address + 80); }
  void set_vt_deactivate(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t vt_deactivate_address() const { return address + 80; }
  // measure fault: the vtable's slot +84 (the voice's pass), read through the voice
  uint32_t vt_pass() const { return guest::LoadU32(address + 84); }
  void set_vt_pass(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t vt_pass_address() const { return address + 84; }
};

// image; measure: XAudioObject.+0
struct XAudioVoiceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
};

// allocated; measure: XAudioSourceVoice, XAudioSourceVoice::XAudioSourceVoice.ret
struct XAudioSourceVoice : View {
  using View::View;
  static constexpr uint32_t kSize = 148;
  static constexpr uint32_t kVtable = 0x8204A720;
  template <typename T = XAudioSourceVoiceVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: XAudioSourceVoice::Init's r4 is its description (+56 the category)
  uint8_t desc_category() const { return guest::LoadU8(address + 56); }
  void set_desc_category(uint8_t v) const { guest::StoreU8(address + 56, v); }
  uint32_t desc_category_address() const { return address + 56; }
  // measure fault: the description's +59 (the codec description's +68)
  uint8_t desc_codec_byte() const { return guest::LoadU8(address + 59); }
  void set_desc_codec_byte(uint8_t v) const { guest::StoreU8(address + 59, v); }
  uint32_t desc_codec_byte_address() const { return address + 59; }
  // measure fault: the description's +60 (the codec description's +64)
  float desc_codec_float() const { return guest::LoadFloat(address + 60); }
  void set_desc_codec_float(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t desc_codec_float_address() const { return address + 60; }
  // the voice's flags (XAudioVoice)
  uint8_t flags() const { return guest::LoadU8(address + 61); }
  void set_flags(uint8_t v) const { guest::StoreU8(address + 61, v); }
  uint32_t flags_address() const { return address + 61; }
  // measure fault: the description's +64 (the codec description's +69)
  uint8_t desc_codec_flag() const { return guest::LoadU8(address + 64); }
  void set_desc_codec_flag(uint8_t v) const { guest::StoreU8(address + 64, v); }
  uint32_t desc_codec_flag_address() const { return address + 64; }
  // the codec's effect entry (8 bytes): +76 the object, PCM or XMA by the format's id
  template <typename T = XAudioCodec>
  T codec() const { return T(guest::LoadU32(address + 76)); }
  void set_codec(const View& v) const { guest::StoreU32(address + 76, v.address); }
  void set_codec(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t codec_address() const { return address + 76; }
  // measure fault: the description's +80 (the codec description's +72)
  uint32_t desc_codec_word_a() const { return guest::LoadU32(address + 80); }
  void set_desc_codec_word_a(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t desc_codec_word_a_address() const { return address + 80; }
  // the 56-byte source format (also the description's +84 and +88 by the measure's join)
  uint32_t format_address() const { return address + 84; }
  uint32_t format_id() const { return guest::LoadU32(address + 88); }
  void set_format_id(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t format_id_address() const { return address + 88; }
  // 1.0 at first
  float volume() const { return guest::LoadFloat(address + 140); }
  void set_volume(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t volume_address() const { return address + 140; }
  // 0 or 1 (the engine's category volumes)
  uint8_t category() const { return guest::LoadU8(address + 144); }
  void set_category(uint8_t v) const { guest::StoreU8(address + 144, v); }
  uint32_t category_address() const { return address + 144; }
};

// image; measure: XAudioSourceVoice.+0, XAudioSourceVoice::XAudioSourceVoice.ret.+0
struct XAudioSourceVoiceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // XAudioObject::Free
  uint32_t free() const { return guest::LoadU32(address + 12); }
  void set_free(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t free_address() const { return address + 12; }
  uint32_t apply_volume() const { return guest::LoadU32(address + 92); }
  void set_apply_volume(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t apply_volume_address() const { return address + 92; }
};

// image; measure: XAudioSourceVoice.+76.+0
struct XAudioSourceCodecVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  uint32_t release() const { return guest::LoadU32(address + 4); }
  void set_release(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t release_address() const { return address + 4; }
  uint32_t submit_packet() const { return guest::LoadU32(address + 28); }
  void set_submit_packet(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t submit_packet_address() const { return address + 28; }
  uint32_t reset() const { return guest::LoadU32(address + 32); }
  void set_reset(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t reset_address() const { return address + 32; }
  // bit 0 playing, bit 2 done
  uint32_t get_state() const { return guest::LoadU32(address + 44); }
  void set_get_state(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t get_state_address() const { return address + 44; }
  uint32_t stop() const { return guest::LoadU32(address + 52); }
  void set_stop(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t stop_address() const { return address + 52; }
  uint32_t set_volume() const { return guest::LoadU32(address + 88); }
  void set_set_volume(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t set_volume_address() const { return address + 88; }
  uint32_t set_frequency_ratio() const { return guest::LoadU32(address + 96); }
  void set_set_frequency_ratio(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t set_frequency_ratio_address() const { return address + 96; }
};

// allocated; measure: XAudioSubmixVoice, XAudioSubmixVoice::XAudioSubmixVoice.ret
struct XAudioSubmixVoice : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  static constexpr uint32_t kVtable = 0x8204A870;
  template <typename T = XAudioSubmixVoiceVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: XAudioSubmixVoice::Describe's r3 is its description (+1 the channels)
  uint8_t desc_channels() const { return guest::LoadU8(address + 1); }
  void set_desc_channels(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t desc_channels_address() const { return address + 1; }
  // measure fault: the description's +8
  uint8_t desc_output_count() const { return guest::LoadU8(address + 8); }
  void set_desc_output_count(uint8_t v) const { guest::StoreU8(address + 8, v); }
  uint32_t desc_output_count_address() const { return address + 8; }
  // measure fault: the description's +9
  uint8_t desc_output_channels() const { return guest::LoadU8(address + 9); }
  void set_desc_output_channels(uint8_t v) const { guest::StoreU8(address + 9, v); }
  uint32_t desc_output_channels_address() const { return address + 9; }
  // measure fault: the description's +12
  uint32_t desc_effect_chain() const { return guest::LoadU32(address + 12); }
  void set_desc_effect_chain(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t desc_effect_chain_address() const { return address + 12; }
  // measure fault: the description's +16
  uint32_t desc_output_voices() const { return guest::LoadU32(address + 16); }
  void set_desc_output_voices(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t desc_output_voices_address() const { return address + 16; }
  // measure fault: the description's +20
  uint32_t desc_callback() const { return guest::LoadU32(address + 20); }
  void set_desc_callback(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t desc_callback_address() const { return address + 20; }
  // measure fault: the description's +24
  uint32_t desc_callback_context() const { return guest::LoadU32(address + 24); }
  void set_desc_callback_context(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t desc_callback_context_address() const { return address + 24; }
  // the output buffer's interface (the buffer + 8)
  uint32_t buffer() const { return guest::LoadU32(address + 32); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t buffer_address() const { return address + 32; }
  // the voice's flags (XAudioVoice)
  uint8_t flags() const { return guest::LoadU8(address + 61); }
  void set_flags(uint8_t v) const { guest::StoreU8(address + 61, v); }
  uint32_t flags_address() const { return address + 61; }
  // its stage: the list in the engine's +124
  uint8_t stage() const { return guest::LoadU8(address + 76); }
  void set_stage(uint8_t v) const { guest::StoreU8(address + 76, v); }
  uint32_t stage_address() const { return address + 76; }
};

// image; measure: XAudioSubmixVoice::XAudioSubmixVoice.ret.+0
struct XAudioSubmixVoiceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // XAudioObject::Free
  uint32_t free() const { return guest::LoadU32(address + 12); }
  void set_free(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t free_address() const { return address + 12; }
};

// embedded; measure: XAudio
struct XAudioSourceDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  // the source format (+0 its type: 0-3 PCM, 4 XMA; +12 + 8i an XMA stream's channels)
  uint32_t format_address() const { return address + 0; }
  uint8_t format_type() const { return guest::LoadU8(address + 0); }
  void set_format_type(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t format_type_address() const { return address + 0; }
  // the channels, or an XMA format's stream count
  uint8_t format_channels() const { return guest::LoadU8(address + 4); }
  void set_format_channels(uint8_t v) const { guest::StoreU8(address + 4, v); }
  uint32_t format_channels_address() const { return address + 4; }
  uint8_t category() const { return guest::LoadU8(address + 56); }
  void set_category(uint8_t v) const { guest::StoreU8(address + 56, v); }
  uint32_t category_address() const { return address + 56; }
  // 1 when 0
  uint8_t output_count() const { return guest::LoadU8(address + 57); }
  void set_output_count(uint8_t v) const { guest::StoreU8(address + 57, v); }
  uint32_t output_count_address() const { return address + 57; }
  // 6 when 0
  uint8_t output_channels() const { return guest::LoadU8(address + 58); }
  void set_output_channels(uint8_t v) const { guest::StoreU8(address + 58, v); }
  uint32_t output_channels_address() const { return address + 58; }
  // the codec description's +68
  uint8_t codec_byte() const { return guest::LoadU8(address + 59); }
  void set_codec_byte(uint8_t v) const { guest::StoreU8(address + 59, v); }
  uint32_t codec_byte_address() const { return address + 59; }
  // the codec description's +64
  float codec_float() const { return guest::LoadFloat(address + 60); }
  void set_codec_float(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t codec_float_address() const { return address + 60; }
  // the codec description's +69
  uint8_t codec_flag() const { return guest::LoadU8(address + 64); }
  void set_codec_flag(uint8_t v) const { guest::StoreU8(address + 64, v); }
  uint32_t codec_flag_address() const { return address + 64; }
  template <typename T = XAudioEffectChain>
  T effect_chain() const { return T(guest::LoadU32(address + 68)); }
  void set_effect_chain(const View& v) const { guest::StoreU32(address + 68, v.address); }
  void set_effect_chain(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t effect_chain_address() const { return address + 68; }
  template <typename T = XAudioOutputList>
  T output_voices() const { return T(guest::LoadU32(address + 72)); }
  void set_output_voices(const View& v) const { guest::StoreU32(address + 72, v.address); }
  void set_output_voices(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t output_voices_address() const { return address + 72; }
  uint32_t callback() const { return guest::LoadU32(address + 76); }
  void set_callback(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t callback_address() const { return address + 76; }
  // the codec description's +72
  uint32_t codec_word_a() const { return guest::LoadU32(address + 80); }
  void set_codec_word_a(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t codec_word_a_address() const { return address + 80; }
  // the codec description's +76
  uint32_t codec_word_b() const { return guest::LoadU32(address + 84); }
  void set_codec_word_b(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t codec_word_b_address() const { return address + 84; }
  // also the codec description's +4
  uint32_t callback_context() const { return guest::LoadU32(address + 88); }
  void set_callback_context(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t callback_context_address() const { return address + 88; }
  // measure fault: XAudio::WaitForVoice's engine (+144 the voices the mixer works on)
  uint32_t engine_working_voices() const { return guest::LoadU32(address + 144); }
  void set_engine_working_voices(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t engine_working_voices_address() const { return address + 144; }
  // measure fault: WaitForVoice's engine (+300 the mixer's thread)
  uint32_t engine_mixer_thread() const { return guest::LoadU32(address + 300); }
  void set_engine_mixer_thread(uint32_t v) const { guest::StoreU32(address + 300, v); }
  uint32_t engine_mixer_thread_address() const { return address + 300; }
};

// embedded; measure: XAudioSubmixVoice::XAudioSubmixVoice.r4
struct XAudioSubmixDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint8_t channels() const { return guest::LoadU8(address + 1); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t channels_address() const { return address + 1; }
  // 1 when 0
  uint8_t output_count() const { return guest::LoadU8(address + 8); }
  void set_output_count(uint8_t v) const { guest::StoreU8(address + 8, v); }
  uint32_t output_count_address() const { return address + 8; }
  // 6 when 0
  uint8_t output_channels() const { return guest::LoadU8(address + 9); }
  void set_output_channels(uint8_t v) const { guest::StoreU8(address + 9, v); }
  uint32_t output_channels_address() const { return address + 9; }
  uint8_t stage() const { return guest::LoadU8(address + 10); }
  void set_stage(uint8_t v) const { guest::StoreU8(address + 10, v); }
  uint32_t stage_address() const { return address + 10; }
  template <typename T = XAudioEffectChain>
  T effect_chain() const { return T(guest::LoadU32(address + 12)); }
  void set_effect_chain(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_effect_chain(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t effect_chain_address() const { return address + 12; }
  template <typename T = XAudioOutputList>
  T output_voices() const { return T(guest::LoadU32(address + 16)); }
  void set_output_voices(const View& v) const { guest::StoreU32(address + 16, v.address); }
  void set_output_voices(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t output_voices_address() const { return address + 16; }
  uint32_t callback() const { return guest::LoadU32(address + 20); }
  void set_callback(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t callback_address() const { return address + 20; }
  uint32_t callback_context() const { return guest::LoadU32(address + 24); }
  void set_callback_context(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t callback_context_address() const { return address + 24; }
};

// embedded; measure: XAudioSubmixVoice::Describe.r4, XAudioSourceVoice::Describe.r4, XAudioVoice::InitBase.r4, XAudioVoice::InitOutputs.r4
struct XAudioVoiceDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the voice format (8 bytes, XAudioVoiceFormat): its first word
  uint32_t format() const { return guest::LoadU32(address + 0); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t format_address() const { return address + 0; }
  // 0
  uint8_t format_type() const { return guest::LoadU8(address + 0); }
  void set_format_type(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t format_type_address() const { return address + 0; }
  uint8_t channels() const { return guest::LoadU8(address + 1); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t channels_address() const { return address + 1; }
  // 48,000
  uint32_t rate() const { return guest::LoadU32(address + 4); }
  void set_rate(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t rate_address() const { return address + 4; }
  template <typename T = XAudioEffectChain>
  T effect_chain() const { return T(guest::LoadU32(address + 8)); }
  void set_effect_chain(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_effect_chain(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t effect_chain_address() const { return address + 8; }
  // an output buffer's interface to share (the source's: the engine's first), 0 for the voice's own
  uint32_t buffer() const { return guest::LoadU32(address + 12); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t buffer_address() const { return address + 12; }
  uint32_t callback() const { return guest::LoadU32(address + 16); }
  void set_callback(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t callback_address() const { return address + 16; }
  uint32_t callback_context() const { return guest::LoadU32(address + 20); }
  void set_callback_context(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t callback_context_address() const { return address + 20; }
  uint8_t output_count() const { return guest::LoadU8(address + 24); }
  void set_output_count(uint8_t v) const { guest::StoreU8(address + 24, v); }
  uint32_t output_count_address() const { return address + 24; }
  uint8_t output_channels() const { return guest::LoadU8(address + 25); }
  void set_output_channels(uint8_t v) const { guest::StoreU8(address + 25, v); }
  uint32_t output_channels_address() const { return address + 25; }
  template <typename T = XAudioOutputList>
  T output_voices() const { return T(guest::LoadU32(address + 28)); }
  void set_output_voices(const View& v) const { guest::StoreU32(address + 28, v.address); }
  void set_output_voices(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t output_voices_address() const { return address + 28; }
};

// embedded; measure: XAudio::SourceOutputFormat.r4, XAudioVoice::GetFormat.r4, XAudioVoice::SetFormatWords.r4, XAudioSubmixVoice::SetFormat.r4
struct XAudioVoiceFormat : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the first word (copied as a word)
  uint32_t head() const { return guest::LoadU32(address + 0); }
  void set_head(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t head_address() const { return address + 0; }
  // 0
  uint8_t type() const { return guest::LoadU8(address + 0); }
  void set_type(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  uint8_t channels() const { return guest::LoadU8(address + 1); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t channels_address() const { return address + 1; }
  // 48,000
  uint32_t rate() const { return guest::LoadU32(address + 4); }
  void set_rate(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t rate_address() const { return address + 4; }
};

// embedded; measure: XAudioVoice::CreateEffects.r4, XAudioVoice::InitBase.r4.+8
struct XAudioEffectChain : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the class ids, a word each
  uint32_t ids() const { return guest::LoadU32(address + 4); }
  void set_ids(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ids_address() const { return address + 4; }
};

// embedded; measure: XAudioVoice::SetOutputVoices_82460BF0.r4
struct XAudioOutputList : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  uint32_t entries() const { return guest::LoadU32(address + 4); }
  void set_entries(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t entries_address() const { return address + 4; }
};

// embedded; measure: XAudioVoice::SetOutputMatrices.r4
struct XAudioOutputMatrices : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  uint32_t entries() const { return guest::LoadU32(address + 4); }
  void set_entries(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t entries_address() const { return address + 4; }
};

// embedded; measure: XAudioVoice::SetOutputMatrices.r4.+4
struct XAudioOutputMatrixEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t output() const { return guest::LoadU8(address + 0); }
  void set_output(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t output_address() const { return address + 0; }
  uint32_t levels() const { return guest::LoadU32(address + 4); }
  void set_levels(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t levels_address() const { return address + 4; }
};

// embedded; measure: XAudio::DefaultMatrix.r5
struct XAudioMatrixLevels : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint8_t count() const { return guest::LoadU8(address + 0); }
  void set_count(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // in the default matrices at 0x8204A7B8
  uint32_t table() const { return guest::LoadU32(address + 4); }
  void set_table(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t table_address() const { return address + 4; }
};

// embedded; measure: XAudioVoice::CreateObject.r4, XAudioSourceVoice@+76
struct XAudioEffectEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = XAudioEffectObject>
  T object() const { return T(guest::LoadU32(address + 0)); }
  void set_object(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_object(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t object_address() const { return address + 0; }
  // the effect's index (255 for a codec or matrix)
  uint8_t index() const { return guest::LoadU8(address + 4); }
  void set_index(uint8_t v) const { guest::StoreU8(address + 4, v); }
  uint32_t index_address() const { return address + 4; }
  // bit 0
  uint8_t enabled() const { return guest::LoadU8(address + 5); }
  void set_enabled(uint8_t v) const { guest::StoreU8(address + 5, v); }
  uint32_t enabled_address() const { return address + 5; }
  // the object's info byte (4 a codec, 6 a matrix or the reverb, 1 the meter): how the pass hands it the buffers
  uint8_t flags() const { return guest::LoadU8(address + 6); }
  void set_flags(uint8_t v) const { guest::StoreU8(address + 6, v); }
  uint32_t flags_address() const { return address + 6; }
};

// allocated; measure: XAudioVoice::CreateObject.r4.+0, XAudioSourceVoice@+76.+0
struct XAudioEffectObject : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the vtable's slot +8 (the info), read through the object
  uint32_t vt_info() const { return guest::LoadU32(address + 8); }
  void set_vt_info(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t vt_info_address() const { return address + 8; }
  // measure fault: the vtable's slot +24 (the pass), read through the object
  uint32_t vt_process() const { return guest::LoadU32(address + 24); }
  void set_vt_process(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t vt_process_address() const { return address + 24; }
};

// allocated; measure: XAudioVoice.+72
struct XAudioOutputSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the voice output to (AddRef'd)
  template <typename T = XAudioOutputTarget>
  T target() const { return T(guest::LoadU32(address + 0)); }
  void set_target(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_target(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t target_address() const { return address + 0; }
  // the matrix object (the factory's class 1)
  template <typename T = XAudioMatrix>
  T matrix() const { return T(guest::LoadU32(address + 4)); }
  void set_matrix(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_matrix(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t matrix_address() const { return address + 4; }
};

// allocated; measure: XAudioVoice.+72.+0
struct XAudioOutputTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the vtable's slot +64, read through the voice
  uint32_t vt_get_output_format() const { return guest::LoadU32(address + 64); }
  void set_vt_get_output_format(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t vt_get_output_format_address() const { return address + 64; }
};

// image; measure: XAudioVoice.+72.+4.+0
struct XAudioOutputMatrixVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // the pass: mixes the voice's buffer into the target's
  uint32_t process() const { return guest::LoadU32(address + 24); }
  void set_process(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t process_address() const { return address + 24; }
  uint32_t set_levels() const { return guest::LoadU32(address + 40); }
  void set_set_levels(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t set_levels_address() const { return address + 40; }
};

// image; measure: XAudioVoice.+8.+0, XAudioObject.+8.+0, XAudioBuffer::XAudioBuffer.r5.+0, XAudioBuffer::Create.r4.+0
struct XAudioVoiceAllocatorVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t add_ref() const { return guest::LoadU32(address + 0); }
  void set_add_ref(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t add_ref_address() const { return address + 0; }
  uint32_t release() const { return guest::LoadU32(address + 4); }
  void set_release(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t release_address() const { return address + 4; }
  uint32_t alloc() const { return guest::LoadU32(address + 20); }
  void set_alloc(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t alloc_address() const { return address + 20; }
};

// embedded; measure: XAudio::DefaultAllocator.r5, XAudio::GetAllocator.r5
struct XAudioAllocatorOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = XAudioBumpAllocator>
  T allocator() const { return T(guest::LoadU32(address + 0)); }
  void set_allocator(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_allocator(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t allocator_address() const { return address + 0; }
};

// allocated; measure: XAudioEngine, XAudioEngine.+120
struct XAudioEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 372;
  static constexpr uint32_t kVtable = 0x8204A6CC;
  // 0x8204A6CC then 0x8204A6E0
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: XAudioEngine::Barrier's r4 is a stage barrier (8 bytes, a byte a processor; the engine's +356 or +364)
  uint64_t barrier() const { return guest::LoadU64(address + 0); }
  void set_barrier(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t barrier_address() const { return address + 0; }
  // measure fault: XAudioEngine::Init's and Size's r4 are the init parameters (+0 the output channels)
  uint8_t params_channels() const { return guest::LoadU8(address + 0); }
  void set_params_channels(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t params_channels_address() const { return address + 0; }
  // measure fault: the parameters' +1 (the submix stage count)
  uint8_t params_stages() const { return guest::LoadU8(address + 1); }
  void set_params_stages(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t params_stages_address() const { return address + 1; }
  // measure fault: the parameters' +2 (the processors' mask)
  uint8_t params_processors() const { return guest::LoadU8(address + 2); }
  void set_params_processors(uint8_t v) const { guest::StoreU8(address + 2, v); }
  uint32_t params_processors_address() const { return address + 2; }
  // also the parameters' +4 (Init, Size)
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  template <typename T = XAudioBumpAllocator>
  T allocator() const { return T(guest::LoadU32(address + 8)); }
  void set_allocator(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_allocator(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t allocator_address() const { return address + 8; }
  // two output buffers' interfaces a processor (+12 + 8i)
  uint32_t mix_buffers_address() const { return address + 12; }
  // the object factory's interface (the factory + 8)
  uint32_t factory() const { return guest::LoadU32(address + 60); }
  void set_factory(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t factory_address() const { return address + 60; }
  // every voice's default output
  template <typename T = XAudioVoice>
  T mastering_voice() const { return T(guest::LoadU32(address + 64)); }
  void set_mastering_voice(const View& v) const { guest::StoreU32(address + 64, v.address); }
  void set_mastering_voice(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t mastering_voice_address() const { return address + 64; }
  // the voices whose start is pending: a list head
  uint32_t pending_next() const { return guest::LoadU32(address + 68); }
  void set_pending_next(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t pending_next_address() const { return address + 68; }
  uint32_t pending_prev() const { return guest::LoadU32(address + 72); }
  void set_pending_prev(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t pending_prev_address() const { return address + 72; }
  // the node's offset in the voice
  uint32_t pending_node_offset() const { return guest::LoadU32(address + 76); }
  void set_pending_node_offset(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t pending_node_offset_address() const { return address + 76; }
  // the source voices' stage (44 bytes, xaudio-engine.toml's XAudioStage)
  uint32_t source_stage_address() const { return address + 80; }
  uint32_t source_next() const { return guest::LoadU32(address + 80); }
  void set_source_next(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t source_next_address() const { return address + 80; }
  uint32_t source_prev() const { return guest::LoadU32(address + 84); }
  void set_source_prev(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t source_prev_address() const { return address + 84; }
  uint32_t source_node_offset() const { return guest::LoadU32(address + 88); }
  void set_source_node_offset(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t source_node_offset_address() const { return address + 88; }
  uint32_t source_list_a_next() const { return guest::LoadU32(address + 92); }
  void set_source_list_a_next(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t source_list_a_next_address() const { return address + 92; }
  uint32_t source_list_a_prev() const { return guest::LoadU32(address + 96); }
  void set_source_list_a_prev(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t source_list_a_prev_address() const { return address + 96; }
  uint32_t source_list_a_offset() const { return guest::LoadU32(address + 100); }
  void set_source_list_a_offset(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t source_list_a_offset_address() const { return address + 100; }
  uint32_t source_list_b_next() const { return guest::LoadU32(address + 104); }
  void set_source_list_b_next(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t source_list_b_next_address() const { return address + 104; }
  uint32_t source_list_b_prev() const { return guest::LoadU32(address + 108); }
  void set_source_list_b_prev(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t source_list_b_prev_address() const { return address + 108; }
  uint32_t source_list_b_offset() const { return guest::LoadU32(address + 112); }
  void set_source_list_b_offset(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t source_list_b_offset_address() const { return address + 112; }
  // the list to mix (+92 or +104)
  uint32_t source_to_mix() const { return guest::LoadU32(address + 116); }
  void set_source_to_mix(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t source_to_mix_address() const { return address + 116; }
  // the list mixed (the active list)
  uint32_t source_mixed() const { return guest::LoadU32(address + 120); }
  void set_source_mixed(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t source_mixed_address() const { return address + 120; }
  // the submix stages (44 bytes each, past a count word)
  uint32_t submix_stages() const { return guest::LoadU32(address + 124); }
  void set_submix_stages(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t submix_stages_address() const { return address + 124; }
  uint8_t submix_stage_count() const { return guest::LoadU8(address + 128); }
  void set_submix_stage_count(uint8_t v) const { guest::StoreU8(address + 128, v); }
  uint32_t submix_stage_count_address() const { return address + 128; }
  // the two categories' volumes (+132, +136)
  float category_volumes() const { return guest::LoadFloat(address + 132); }
  void set_category_volumes(float v) const { guest::StoreFloat(address + 132, v); }
  uint32_t category_volumes_address() const { return address + 132; }
  float category_volume_1() const { return guest::LoadFloat(address + 136); }
  void set_category_volume_1(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t category_volume_1_address() const { return address + 136; }
  // a bit a category whose volume changed
  uint32_t changed_categories() const { return guest::LoadU32(address + 140); }
  void set_changed_categories(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t changed_categories_address() const { return address + 140; }
  // the six voices the mixer is working on
  uint32_t working_voices_address() const { return address + 144; }
  // the processing callbacks: 2 stages x 8 slots of 8 bytes (the function, its context)
  uint32_t callbacks_address() const { return address + 168; }
  // the voice being processed
  uint32_t current_voice() const { return guest::LoadU32(address + 296); }
  void set_current_voice(uint32_t v) const { guest::StoreU32(address + 296, v); }
  uint32_t current_voice_address() const { return address + 296; }
  // the mixer's thread during a mix ([r13 + 256])
  uint32_t mixer_thread() const { return guest::LoadU32(address + 300); }
  void set_mixer_thread(uint32_t v) const { guest::StoreU32(address + 300, v); }
  uint32_t mixer_thread_address() const { return address + 300; }
  uint32_t processor_count() const { return guest::LoadU32(address + 304); }
  void set_processor_count(uint32_t v) const { guest::StoreU32(address + 304, v); }
  uint32_t processor_count_address() const { return address + 304; }
  // the processor threads' handles (+308 + 4i)
  uint32_t processor_threads_address() const { return address + 308; }
  // +332 + 4 x the processor: its worker thread
  uint32_t workers_address() const { return address + 332; }
  // two 8-byte barriers (a byte a processor)
  uint32_t stage_barriers_address() const { return address + 356; }
};

// allocated; measure: XAudioBuffer, XAudioBuffer::XAudioBuffer.ret
struct XAudioBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr uint32_t kVtable = 0x8204A8CC;
  template <typename T = XAudioBufferVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: XAudioBuffer::Size's r3 is the buffer description (+0 its kind)
  uint8_t desc_kind() const { return guest::LoadU8(address + 0); }
  void set_desc_kind(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t desc_kind_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  // measure fault: the description's +5
  uint8_t desc_channels() const { return guest::LoadU8(address + 5); }
  void set_desc_channels(uint8_t v) const { guest::StoreU8(address + 5, v); }
  uint32_t desc_channels_address() const { return address + 5; }
  // 0 samples of its own, else external samples
  uint8_t kind() const { return guest::LoadU8(address + 8); }
  void set_kind(uint8_t v) const { guest::StoreU8(address + 8, v); }
  uint32_t kind_address() const { return address + 8; }
  // measure fault: the description's +8
  uint32_t desc_rate() const { return guest::LoadU32(address + 8); }
  void set_desc_rate(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t desc_rate_address() const { return address + 8; }
  // the format it was made for (8 bytes): its first word
  uint32_t capacity_format() const { return guest::LoadU32(address + 12); }
  void set_capacity_format(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t capacity_format_address() const { return address + 12; }
  uint8_t capacity_channels() const { return guest::LoadU8(address + 13); }
  void set_capacity_channels(uint8_t v) const { guest::StoreU8(address + 13, v); }
  uint32_t capacity_channels_address() const { return address + 13; }
  uint32_t capacity_rate() const { return guest::LoadU32(address + 16); }
  void set_capacity_rate(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t capacity_rate_address() const { return address + 16; }
  // the format in use (12 bytes with the rate and the samples, GetFormat's): its first word
  uint32_t current() const { return guest::LoadU32(address + 20); }
  void set_current(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t current_address() const { return address + 20; }
  uint8_t current_type() const { return guest::LoadU8(address + 20); }
  void set_current_type(uint8_t v) const { guest::StoreU8(address + 20, v); }
  uint32_t current_type_address() const { return address + 20; }
  // the channels in use
  uint8_t current_channels() const { return guest::LoadU8(address + 21); }
  void set_current_channels(uint8_t v) const { guest::StoreU8(address + 21, v); }
  uint32_t current_channels_address() const { return address + 21; }
  // 48,000
  uint32_t current_rate() const { return guest::LoadU32(address + 24); }
  void set_current_rate(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t current_rate_address() const { return address + 24; }
  // 256 frames at the rate x the channels, floats, 128-byte aligned
  uint32_t samples() const { return guest::LoadU32(address + 28); }
  void set_samples(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t samples_address() const { return address + 28; }
  // the description's +16 (not read further)
  uint32_t extra() const { return guest::LoadU32(address + 32); }
  void set_extra(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t extra_address() const { return address + 32; }
};

// image; measure: XAudioBuffer::XAudioBuffer.ret.+0
struct XAudioBufferVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t get_interface() const { return guest::LoadU32(address + 20); }
  void set_get_interface(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t get_interface_address() const { return address + 20; }
};

// embedded; measure: XAudioBuffer::XAudioBuffer.r4
struct XAudioBufferDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 0 samples of its own
  uint8_t kind() const { return guest::LoadU8(address + 0); }
  void set_kind(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // the voice format (8 bytes): its first word
  uint32_t format() const { return guest::LoadU32(address + 4); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t format_address() const { return address + 4; }
  uint8_t channels() const { return guest::LoadU8(address + 5); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 5, v); }
  uint32_t channels_address() const { return address + 5; }
  uint32_t rate() const { return guest::LoadU32(address + 8); }
  void set_rate(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t rate_address() const { return address + 8; }
  // external samples (kind not 0)
  uint32_t samples() const { return guest::LoadU32(address + 12); }
  void set_samples(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t samples_address() const { return address + 12; }
  // to the buffer's +32
  uint32_t extra() const { return guest::LoadU32(address + 16); }
  void set_extra(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t extra_address() const { return address + 16; }
};

// embedded; measure: XAudioCreateSubmixVoice.r4, XAudioVoice::CreateSource.r5, XAudioVoice::CreateSubmix.r5, sub_82483770.r3@+56
struct XAudioVoiceOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t voice() const { return guest::LoadU32(address + 0); }
  void set_voice(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t voice_address() const { return address + 0; }
};

// embedded; measure: XAudioVoice::GetCallbackContext.r4
struct XAudioWordOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: XAudioVoice::GetType.r4, XAudioSourceVoice::GetState.r4, XAudioSubmixVoice::GetState.r4, XAudioVoice::GetEffectEnabled.r5
struct XAudioByteOut : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t value() const { return guest::LoadU8(address + 0); }
  void set_value(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: XAudioVoice::MixToOutputs.r4, XAudioVoice::ProcessEffect.r5
struct XAudioBufferInOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t buffer() const { return guest::LoadU32(address + 0); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t buffer_address() const { return address + 0; }
};

// embedded; measure: sub_825A5E28.r6
struct XAudioMeterLevelsOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = XAudioMeterLevels>
  T levels() const { return T(guest::LoadU32(address + 0)); }
  void set_levels(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_levels(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t levels_address() const { return address + 0; }
};

// embedded; measure: sub_825A5E28.r6.+0
struct XAudioMeterLevels : View {
  using View::View;
  static constexpr uint32_t kSize = 104;
  // probably (written last)
  uint32_t channels() const { return guest::LoadU32(address + 0); }
  void set_channels(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t channels_address() const { return address + 0; }
  // the meter's +24
  uint32_t history_slot() const { return guest::LoadU32(address + 4); }
  void set_history_slot(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t history_slot_address() const { return address + 4; }
  // 8 floats (probably, as the meter's +28)
  uint32_t threshold_counts_address() const { return address + 8; }
  // 8 floats (probably)
  uint32_t peaks_address() const { return address + 40; }
  // 8 floats (probably)
  uint32_t rms_address() const { return address + 72; }
};

// embedded; measure: XAudioEngine@+356
struct XAudioEngineAt356 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint64_t unknown_0() const { return guest::LoadU64(address + 0); }
  void set_unknown_0(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: XAudioEngine@+60
struct XAudioEngineAt60 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: XAudioEngine@+64
struct XAudioEngineAt64 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/xaudio.md's code uses.
struct XaudioGlobals {
  template <typename T = XAudioEngine>
  static T engine() { return T(guest::LoadU32(0x82954F08u)); }
  static void set_engine(const View& v) { guest::StoreU32(0x82954F08u, v.address); }
  static void set_engine(uint32_t v) { guest::StoreU32(0x82954F08u, v); }
  static constexpr uint32_t engine_address() { return 0x82954F08u; }
  // the voice lists' spin lock (the kernel's)
  static uint32_t spin_lock() { return guest::LoadU32(0x82954F10u); }
  static void set_spin_lock(uint32_t v) { guest::StoreU32(0x82954F10u, v); }
  static constexpr uint32_t spin_lock_address() { return 0x82954F10u; }
  // its depth (a recursive lock over the kernel's)
  static uint32_t spin_depth() { return guest::LoadU32(0x82954F14u); }
  static void set_spin_depth(uint32_t v) { guest::StoreU32(0x82954F14u, v); }
  static constexpr uint32_t spin_depth_address() { return 0x82954F14u; }
  // the owning thread (r13)
  static uint32_t spin_owner() { return guest::LoadU32(0x82954F18u); }
  static void set_spin_owner(uint32_t v) { guest::StoreU32(0x82954F18u, v); }
  static constexpr uint32_t spin_owner_address() { return 0x82954F18u; }
  // the IRQL KeRaiseIrqlToDpcLevel saved
  static uint8_t spin_irql() { return guest::LoadU8(0x82954F1Cu); }
  static void set_spin_irql(uint8_t v) { guest::StoreU8(0x82954F1Cu, v); }
  static constexpr uint32_t spin_irql_address() { return 0x82954F1Cu; }
  // +0 0x8204A868 (set at exit)
  static uint32_t api_lock_object() { return guest::LoadU32(0x8264D514u); }
  static void set_api_lock_object(uint32_t v) { guest::StoreU32(0x8264D514u, v); }
  static constexpr uint32_t api_lock_object_address() { return 0x8264D514u; }
  // the API lock: a kernel critical section
  static constexpr uint32_t api_lock_address() { return 0x8264D518u; }
  // the critical section's recursion count (the object's +24)
  static uint32_t api_lock_recursion() { return guest::LoadU32(0x8264D52Cu); }
  static void set_api_lock_recursion(uint32_t v) { guest::StoreU32(0x8264D52Cu, v); }
  static constexpr uint32_t api_lock_recursion_address() { return 0x8264D52Cu; }
  // the default output matrices' levels
  static constexpr uint32_t default_matrices_address() { return 0x8204A7B8u; }
  // {0, 0}: the mastering voice with its default matrix
  static constexpr uint32_t default_output_address() { return 0x8204A9F4u; }
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
};

}  // namespace schema
