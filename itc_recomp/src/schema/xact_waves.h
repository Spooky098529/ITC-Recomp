// The views of docs/functions/xact-waves.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/xact-waves.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: -
struct XACTVoice : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // 0x82075B98
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the XAudio source voice
  uint32_t source_voice() const { return guest::LoadU32(address + 4); }
  void set_source_voice(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t source_voice_address() const { return address + 4; }
  // the reverb submix's XAudio voice (the holder's slot +4), 0 without a send; never set in play
  uint32_t reverb_voice() const { return guest::LoadU32(address + 8); }
  void set_reverb_voice(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t reverb_voice_address() const { return address + 8; }
  // the channel count (the description's byte +4)
  uint8_t channels() const { return guest::LoadU8(address + 12); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 12, v); }
  uint32_t channels_address() const { return address + 12; }
  // the reverb send level (1.0 when made)
  float reverb_level() const { return guest::LoadFloat(address + 16); }
  void set_reverb_level(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t reverb_level_address() const { return address + 16; }
  // the outputs not yet set by SetChannelMatrix (1 when made)
  uint32_t outputs_unset() const { return guest::LoadU32(address + 20); }
  void set_outputs_unset(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t outputs_unset_address() const { return address + 20; }
};

// allocated; measure: -
struct XACTReverb : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0x82075BD4
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the XAudio submix voice (2 channels, 48 kHz)
  uint32_t submix_voice() const { return guest::LoadU32(address + 4); }
  void set_submix_voice(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t submix_voice_address() const { return address + 4; }
};

// allocated; measure: -
struct XACTWave : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  // 0x82076A38 the base, 0x82076A74 the in-memory wave
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the node in the wave bank's wave list (bank +240): the next node's object (self when unlinked)
  uint32_t node_next() const { return guest::LoadU32(address + 4); }
  void set_node_next(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t node_next_address() const { return address + 4; }
  // the previous node's object (self when unlinked)
  uint32_t node_previous() const { return guest::LoadU32(address + 8); }
  void set_node_previous(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t node_previous_address() const { return address + 8; }
  // the listener interface on the wave bank (0x82076A70; 0x8203DF00 the base first)
  uint32_t listener_vtable() const { return guest::LoadU32(address + 12); }
  void set_listener_vtable(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t listener_vtable_address() const { return address + 12; }
  // set by Stop with mode 0
  uint32_t stopped_at_once() const { return guest::LoadU32(address + 16); }
  void set_stopped_at_once(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t stopped_at_once_address() const { return address + 16; }
  // XACT's engine
  uint32_t engine() const { return guest::LoadU32(address + 20); }
  void set_engine(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t engine_address() const { return address + 20; }
  // 255 forever
  uint8_t loop_count() const { return guest::LoadU8(address + 24); }
  void set_loop_count(uint8_t v) const { guest::StoreU8(address + 24, v); }
  uint32_t loop_count_address() const { return address + 24; }
  // 0x2 started, 0x4 released, 0x8 prepare the next wave, 0x10 finished, 0x20 loops, 0x40 flushed, 0x80 paused
  uint8_t flags() const { return guest::LoadU8(address + 25); }
  void set_flags(uint8_t v) const { guest::StoreU8(address + 25, v); }
  uint32_t flags_address() const { return address + 25; }
  // set atomically: the audio callback calls the owner's slot +32
  uint32_t request_owner() const { return guest::LoadU32(address + 28); }
  void set_request_owner(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t request_owner_address() const { return address + 28; }
  // set atomically (RequestNext): stop now and hand the next wave to the owner
  uint32_t request_stop() const { return guest::LoadU32(address + 32); }
  void set_request_stop(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t request_stop_address() const { return address + 32; }
  // the wave bank (its +4 the listener interface, +240 the wave list)
  uint32_t wave_bank() const { return guest::LoadU32(address + 36); }
  void set_wave_bank(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t wave_bank_address() const { return address + 36; }
  // 1 preparing, 2 prepared, 3 playing, 4 stopping, 5 stopped
  uint32_t state() const { return guest::LoadU32(address + 40); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t state_address() const { return address + 40; }
  template <typename T = XACTVoice>
  T voice() const { return T(guest::LoadU32(address + 44)); }
  void set_voice(const View& v) const { guest::StoreU32(address + 44, v.address); }
  void set_voice(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t voice_address() const { return address + 44; }
  // probably the play-wave event of a sound's track ([[[owner + 44] + 16] + 20] the sound, its +48 the cue)
  uint32_t owner() const { return guest::LoadU32(address + 48); }
  void set_owner(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t owner_address() const { return address + 48; }
  // the entry in the wave bank
  uint16_t wave_index() const { return guest::LoadU16(address + 52); }
  void set_wave_index(uint16_t v) const { guest::StoreU16(address + 52, v); }
  uint32_t wave_index_address() const { return address + 52; }
  // the next wave of a loop or variation
  template <typename T = XACTWave>
  T next_wave() const { return T(guest::LoadU32(address + 56)); }
  void set_next_wave(const View& v) const { guest::StoreU32(address + 56, v.address); }
  void set_next_wave(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t next_wave_address() const { return address + 56; }
  uint32_t prepared() const { return guest::LoadU32(address + 60); }
  void set_prepared(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t prepared_address() const { return address + 60; }
  // in-memory wave: the packets' context (16 bytes, the packet's +36 points here): the wave at first
  uint32_t packet_context() const { return guest::LoadU32(address + 64); }
  void set_packet_context(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t packet_context_address() const { return address + 64; }
  // in-memory wave: the context's +4, 0 when made
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  // in-memory wave: the context's +8, 1 with the loop region in the one packet, 0 with the loop packet
  uint32_t packet_context_looped() const { return guest::LoadU32(address + 72); }
  void set_packet_context_looped(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t packet_context_looped_address() const { return address + 72; }
  // in-memory wave: the context's +12, 0 when made
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  // in-memory wave: the loop packet's context when the start lies inside the loop (+64 to +76 copied here): the wave at first
  uint32_t loop_context() const { return guest::LoadU32(address + 80); }
  void set_loop_context(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t loop_context_address() const { return address + 80; }
  // in-memory wave: the loop context's +4
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  // in-memory wave: the loop context's +8 (1)
  uint32_t loop_context_looped() const { return guest::LoadU32(address + 88); }
  void set_loop_context_looped(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t loop_context_looped_address() const { return address + 88; }
  // in-memory wave: the loop context's +12
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
};

// The image words docs/functions/xact-waves.md's code uses.
struct XactWavesGlobals {
  // 1.0f (the voice's and the reverb's level without one)
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
};

}  // namespace schema
