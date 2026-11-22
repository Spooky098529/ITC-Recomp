// The views of docs/functions/xact-tracks.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/xact-tracks.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: -
struct XACTTrack : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // 0x820765F8; the base's 0x82075344 after the destructor
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the node in the sound's track list (sound +36)
  uint32_t node_next() const { return guest::LoadU32(base, address + 4); }
  void set_node_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_next_address() const { return address + 4; }
  uint32_t node_prev() const { return guest::LoadU32(base, address + 8); }
  void set_node_prev(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_prev_address() const { return address + 8; }
  // the XACT engine
  uint32_t engine() const { return guest::LoadU32(base, address + 12); }
  void set_engine(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t engine_address() const { return address + 12; }
  // 0 for a simple sound
  template <typename T = XACTTrackRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t record_address() const { return address + 16; }
  // the XACTSound
  uint32_t sound() const { return guest::LoadU32(base, address + 20); }
  void set_sound(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t sound_address() const { return address + 20; }
  template <typename T = XACTTrackInstance>
  T instance() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_instance(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_instance(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t instance_address() const { return address + 24; }
  // a transition; never in play
  template <typename T = XACTTrackInstance>
  T next_instance() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_next_instance(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_next_instance(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t next_instance_address() const { return address + 28; }
  // 0x80 switch to the next instance when it's prepared
  uint8_t flags() const { return guest::LoadU8(base, address + 32); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // a pending transition event
  uint32_t pending_transition() const { return guest::LoadU32(base, address + 36); }
  void set_pending_transition(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t pending_transition_address() const { return address + 36; }
  // a gain, at least 0
  float volume() const { return guest::LoadFloat(base, address + 40); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t volume_address() const { return address + 40; }
  // the pitch offset (octaves)
  float pitch() const { return guest::LoadFloat(base, address + 44); }
  void set_pitch(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t pitch_address() const { return address + 44; }
  // the track index
  uint8_t index() const { return guest::LoadU8(base, address + 48); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 48, v); }
  uint32_t index_address() const { return address + 48; }
  // 44 bytes each, the count at -4; none in play
  uint32_t rpc_entries() const { return guest::LoadU32(base, address + 52); }
  void set_rpc_entries(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t rpc_entries_address() const { return address + 52; }
  // an RPC volume factor (1.0)
  float rpc_volume() const { return guest::LoadFloat(base, address + 56); }
  void set_rpc_volume(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t rpc_volume_address() const { return address + 56; }
  // an RPC pitch offset (0.0)
  float rpc_pitch() const { return guest::LoadFloat(base, address + 60); }
  void set_rpc_pitch(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t rpc_pitch_address() const { return address + 60; }
};

// file; measure: -
struct XACTTrackRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // an index into the gain table 0x820761F8
  uint8_t volume() const { return guest::LoadU8(base, address + 0); }
  void set_volume(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t volume_address() const { return address + 0; }
};

// allocated; measure: -
struct XACTTrackInstance : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // 0x82076AD8; the base's 0x82075344 after the destructor
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a node, linked to itself
  uint32_t node_next() const { return guest::LoadU32(base, address + 4); }
  void set_node_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_next_address() const { return address + 4; }
  uint32_t node_prev() const { return guest::LoadU32(base, address + 8); }
  void set_node_prev(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_prev_address() const { return address + 8; }
  // the XACT engine (+144 its critical section)
  uint32_t engine() const { return guest::LoadU32(base, address + 12); }
  void set_engine(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t engine_address() const { return address + 12; }
  template <typename T = XACTTrack>
  T track() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_track(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_track(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t track_address() const { return address + 16; }
  // an array of XACTEvent pointers
  uint32_t events() const { return guest::LoadU32(base, address + 20); }
  void set_events(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t events_address() const { return address + 20; }
  uint8_t event_count() const { return guest::LoadU8(base, address + 24); }
  void set_event_count(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t event_count_address() const { return address + 24; }
  // the play start time (ms)
  uint32_t start_time() const { return guest::LoadU32(base, address + 28); }
  void set_start_time(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t start_time_address() const { return address + 28; }
  // 0x40 play when prepared
  uint8_t flags() const { return guest::LoadU8(base, address + 32); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // the event of types 1 to 6 every volume, pitch, output and pause call goes to
  template <typename T = XACTPlayWaveEvent>
  T play_wave_event() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_play_wave_event(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_play_wave_event(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t play_wave_event_address() const { return address + 36; }
  // 1 created, 2 preparing, 3 prepared, 4 playing, 5 stopping, 6 done
  uint32_t state() const { return guest::LoadU32(base, address + 40); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t state_address() const { return address + 40; }
  // the events not yet fired
  uint8_t unfired_count() const { return guest::LoadU8(base, address + 44); }
  void set_unfired_count(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t unfired_count_address() const { return address + 44; }
  // the events fired and not done
  uint8_t unfinished_count() const { return guest::LoadU8(base, address + 45); }
  void set_unfinished_count(uint8_t v) const { guest::StoreU8(base, address + 45, v); }
  uint32_t unfinished_count_address() const { return address + 45; }
  // the uncounted helper events' list head: the base vtable 0x82075344
  uint32_t helpers_vtable() const { return guest::LoadU32(base, address + 48); }
  void set_helpers_vtable(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t helpers_vtable_address() const { return address + 48; }
  // a node's +12 is its event
  uint32_t helpers_next() const { return guest::LoadU32(base, address + 52); }
  void set_helpers_next(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t helpers_next_address() const { return address + 52; }
  uint32_t helpers_prev() const { return guest::LoadU32(base, address + 56); }
  void set_helpers_prev(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t helpers_prev_address() const { return address + 56; }
  uint32_t stopped_at_once() const { return guest::LoadU32(base, address + 60); }
  void set_stopped_at_once(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t stopped_at_once_address() const { return address + 60; }
};

// allocated; measure: -
struct XACTEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // 0x82076A10; the base's 0x82075344 after the destructor
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the node in the sound's event queue (sound +56) or pending list (+68)
  uint32_t node_next() const { return guest::LoadU32(base, address + 4); }
  void set_node_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_next_address() const { return address + 4; }
  uint32_t node_prev() const { return guest::LoadU32(base, address + 8); }
  void set_node_prev(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_prev_address() const { return address + 8; }
  uint32_t engine() const { return guest::LoadU32(base, address + 12); }
  void set_engine(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t engine_address() const { return address + 12; }
  template <typename T = XACTEventRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t record_address() const { return address + 16; }
  // ms
  uint32_t fire_time() const { return guest::LoadU32(base, address + 20); }
  void set_fire_time(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t fire_time_address() const { return address + 20; }
  uint32_t done_time() const { return guest::LoadU32(base, address + 24); }
  void set_done_time(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t done_time_address() const { return address + 24; }
  // 0x7FFFFFFF without one
  uint32_t end_time() const { return guest::LoadU32(base, address + 28); }
  void set_end_time(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t end_time_address() const { return address + 28; }
  // the time already elapsed at the prepare (taken off when scheduled)
  uint32_t elapsed() const { return guest::LoadU32(base, address + 32); }
  void set_elapsed(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t elapsed_address() const { return address + 32; }
  // the previous event (timing)
  template <typename T = XACTEvent>
  T previous() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t previous_address() const { return address + 36; }
  // 0x08 uncounted, 0x10 varied, 0x20 queued, 0x40 done, 0x80 fired
  uint8_t flags() const { return guest::LoadU8(base, address + 40); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t flags_address() const { return address + 40; }
  template <typename T = XACTTrackInstance>
  T instance() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_instance(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_instance(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t instance_address() const { return address + 44; }
};

// allocated; measure: -
struct XACTPlayWaveEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // 0x82076CE8; the wave event base's 0x82076B58, then the event's, in the destructor
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t node_next() const { return guest::LoadU32(base, address + 4); }
  void set_node_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_next_address() const { return address + 4; }
  uint32_t node_prev() const { return guest::LoadU32(base, address + 8); }
  void set_node_prev(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_prev_address() const { return address + 8; }
  uint32_t engine() const { return guest::LoadU32(base, address + 12); }
  void set_engine(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t engine_address() const { return address + 12; }
  template <typename T = XACTEventRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t record_address() const { return address + 16; }
  uint32_t fire_time() const { return guest::LoadU32(base, address + 20); }
  void set_fire_time(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t fire_time_address() const { return address + 20; }
  uint32_t done_time() const { return guest::LoadU32(base, address + 24); }
  void set_done_time(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t done_time_address() const { return address + 24; }
  uint32_t end_time() const { return guest::LoadU32(base, address + 28); }
  void set_end_time(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t end_time_address() const { return address + 28; }
  uint32_t elapsed() const { return guest::LoadU32(base, address + 32); }
  void set_elapsed(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t elapsed_address() const { return address + 32; }
  template <typename T = XACTEvent>
  T previous() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t previous_address() const { return address + 36; }
  uint8_t flags() const { return guest::LoadU8(base, address + 40); }
  void set_flags(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t flags_address() const { return address + 40; }
  template <typename T = XACTTrackInstance>
  T instance() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_instance(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_instance(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t instance_address() const { return address + 44; }
  // a flag (its slot +36)
  uint32_t slot36_flag() const { return guest::LoadU32(base, address + 48); }
  void set_slot36_flag(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot36_flag_address() const { return address + 48; }
  // 255 forever
  uint8_t loop_count() const { return guest::LoadU8(base, address + 52); }
  void set_loop_count(uint8_t v) const { guest::StoreU8(base, address + 52, v); }
  uint32_t loop_count_address() const { return address + 52; }
  // the record + 12 for types 2 and 5
  uint32_t variation() const { return guest::LoadU32(base, address + 56); }
  void set_variation(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t variation_address() const { return address + 56; }
  // the XACTWave, whose +48 is this event
  uint32_t wave() const { return guest::LoadU32(base, address + 60); }
  void set_wave(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t wave_address() const { return address + 60; }
  uint32_t play_time() const { return guest::LoadU32(base, address + 64); }
  void set_play_time(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t play_time_address() const { return address + 64; }
  uint32_t stop_time() const { return guest::LoadU32(base, address + 68); }
  void set_stop_time(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t stop_time_address() const { return address + 68; }
};

// allocated; measure: -
struct XACTWaveVariationEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 188;
  // 0x82076C60
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t variation_table() const { return guest::LoadU32(base, address + 52); }
  void set_variation_table(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t variation_table_address() const { return address + 52; }
  uint32_t variation_record() const { return guest::LoadU32(base, address + 60); }
  void set_variation_record(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t variation_record_address() const { return address + 60; }
  uint8_t loop_count() const { return guest::LoadU8(base, address + 68); }
  void set_loop_count(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t loop_count_address() const { return address + 68; }
  // the XACTWave
  uint32_t wave() const { return guest::LoadU32(base, address + 72); }
  void set_wave(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t wave_address() const { return address + 72; }
  // an inner marker's 12-byte record
  uint32_t marker_record_address() const { return address + 76; }
  // the inner marker event
  uint32_t marker_event() const { return guest::LoadU32(base, address + 100); }
  void set_marker_event(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t marker_event_address() const { return address + 100; }
  // a node for the instance's helper list
  uint32_t helper_node_address() const { return address + 172; }
};

// file; measure: -
struct XACTEventRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // bits 0-4 the type (1 play wave), 5-28 the time (ms), 0x20000000 timed from the play start, 0x40000000 from the previous event's fire time
  uint32_t word() const { return guest::LoadU32(base, address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_address() const { return address + 0; }
  // a random 0 to this added to the fire time
  uint16_t random_spread() const { return guest::LoadU16(base, address + 4); }
  void set_random_spread(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t random_spread_address() const { return address + 4; }
  // the previous event's index in the instance's events
  uint8_t previous_index() const { return guest::LoadU8(base, address + 6); }
  void set_previous_index(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t previous_index_address() const { return address + 6; }
  // bit 0 a release
  uint8_t wave_flags() const { return guest::LoadU8(base, address + 7); }
  void set_wave_flags(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t wave_flags_address() const { return address + 7; }
  // the wave in its bank
  uint16_t wave_index() const { return guest::LoadU16(base, address + 8); }
  void set_wave_index(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t wave_index_address() const { return address + 8; }
  uint8_t wave_bank_index() const { return guest::LoadU8(base, address + 10); }
  void set_wave_bank_index(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t wave_bank_index_address() const { return address + 10; }
  // 255 forever
  uint8_t loop_count() const { return guest::LoadU8(base, address + 11); }
  void set_loop_count(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t loop_count_address() const { return address + 11; }
  // types 2 and 5; the variation record at +12 (type 4) or +16 (type 5)
  uint32_t variation() const { return guest::LoadU32(base, address + 12); }
  void set_variation(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t variation_address() const { return address + 12; }
};

// The image words docs/functions/xact-tracks.md's code uses.
struct XactTracksGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 256 floats: a track record's volume byte to a gain
  static constexpr uint32_t gain_table_address() { return 0x820761F8u; }
};

}  // namespace schema
