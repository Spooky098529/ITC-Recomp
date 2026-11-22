// The views of docs/functions/sound-cues.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/sound-cues.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Sound, Sound.+0
struct PlayingSound : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // the manager's +8 interface (its slot +4 GetSoundBank; +8 and +12 the 3D state and the listener)
  uint32_t owner() const { return guest::LoadU32(base, address + 0); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t owner_address() const { return address + 0; }
  // its emitter (300 bytes)
  template <typename T = SoundEmitter>
  T emitter() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_emitter(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_emitter(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t emitter_address() const { return address + 4; }
  // bytes +8 to +11 as one word (Sound::Sound's clear)
  uint32_t flags() const { return guest::LoadU32(base, address + 8); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t flags_address() const { return address + 8; }
  // 3D: X3DAudio's calculation each update (never set in play)
  uint8_t is_3d() const { return guest::LoadU8(base, address + 8); }
  void set_is_3d(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t is_3d_address() const { return address + 8; }
  // played again once stopped
  uint8_t loop() const { return guest::LoadU8(base, address + 9); }
  void set_loop(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t loop_address() const { return address + 9; }
  uint8_t was_playing() const { return guest::LoadU8(base, address + 10); }
  void set_was_playing(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t was_playing_address() const { return address + 10; }
  // Sound::Start; cleared when Sound::Update plays the cue
  uint8_t start_requested() const { return guest::LoadU8(base, address + 11); }
  void set_start_requested(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t start_requested_address() const { return address + 11; }
  // Sound::SetHandle (only the handle list's Sounds, menu-sounds.md)
  uint32_t handle() const { return guest::LoadU32(base, address + 12); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t handle_address() const { return address + 12; }
  uint32_t bank_index() const { return guest::LoadU32(base, address + 16); }
  void set_bank_index(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t bank_index_address() const { return address + 16; }
  uint32_t cue_index() const { return guest::LoadU32(base, address + 20); }
  void set_cue_index(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t cue_index_address() const { return address + 20; }
};

// allocated; measure: Emitter, Emitter.+8
struct SoundEmitter : View {
  using View::View;
  static constexpr uint32_t kSize = 300;
  // the XACT cue (0 none)
  template <typename T = EmitterCue>
  T cue() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_cue(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_cue(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t cue_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the X3DAudio emitter's cone pointer: this emitter's +100
  uint32_t cone() const { return guest::LoadU32(base, address + 8); }
  void set_cone(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t cone_address() const { return address + 8; }
  // front (0, 0, 1)
  float front_x() const { return guest::LoadFloat(base, address + 12); }
  void set_front_x(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t front_x_address() const { return address + 12; }
  float front_y() const { return guest::LoadFloat(base, address + 16); }
  void set_front_y(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t front_y_address() const { return address + 16; }
  float front_z() const { return guest::LoadFloat(base, address + 20); }
  void set_front_z(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t front_z_address() const { return address + 20; }
  // top (0, 1, 0)
  float top_x() const { return guest::LoadFloat(base, address + 24); }
  void set_top_x(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t top_x_address() const { return address + 24; }
  float top_y() const { return guest::LoadFloat(base, address + 28); }
  void set_top_y(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t top_y_address() const { return address + 28; }
  float top_z() const { return guest::LoadFloat(base, address + 32); }
  void set_top_z(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t top_z_address() const { return address + 32; }
  // Sound::SetPosition's (sub_8259CC80)
  float position_x() const { return guest::LoadFloat(base, address + 36); }
  void set_position_x(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t position_x_address() const { return address + 36; }
  float position_y() const { return guest::LoadFloat(base, address + 40); }
  void set_position_y(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t position_y_address() const { return address + 40; }
  float position_z() const { return guest::LoadFloat(base, address + 44); }
  void set_position_z(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t position_z_address() const { return address + 44; }
  float velocity_x() const { return guest::LoadFloat(base, address + 48); }
  void set_velocity_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t velocity_x_address() const { return address + 48; }
  float velocity_y() const { return guest::LoadFloat(base, address + 52); }
  void set_velocity_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t velocity_y_address() const { return address + 52; }
  float velocity_z() const { return guest::LoadFloat(base, address + 56); }
  void set_velocity_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t velocity_z_address() const { return address + 56; }
  // 1
  uint32_t channel_count() const { return guest::LoadU32(base, address + 60); }
  void set_channel_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t channel_count_address() const { return address + 60; }
  // 0
  float channel_radius() const { return guest::LoadFloat(base, address + 64); }
  void set_channel_radius(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t channel_radius_address() const { return address + 64; }
  // this emitter's +132
  uint32_t channel_azimuths() const { return guest::LoadU32(base, address + 68); }
  void set_channel_azimuths(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t channel_azimuths_address() const { return address + 68; }
  // 0: the library's default
  uint32_t volume_curve() const { return guest::LoadU32(base, address + 72); }
  void set_volume_curve(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t volume_curve_address() const { return address + 72; }
  uint32_t lfe_curve() const { return guest::LoadU32(base, address + 76); }
  void set_lfe_curve(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t lfe_curve_address() const { return address + 76; }
  uint32_t lpf_direct_curve() const { return guest::LoadU32(base, address + 80); }
  void set_lpf_direct_curve(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t lpf_direct_curve_address() const { return address + 80; }
  uint32_t lpf_reverb_curve() const { return guest::LoadU32(base, address + 84); }
  void set_lpf_reverb_curve(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t lpf_reverb_curve_address() const { return address + 84; }
  uint32_t reverb_curve() const { return guest::LoadU32(base, address + 88); }
  void set_reverb_curve(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t reverb_curve_address() const { return address + 88; }
  // 1
  float curve_distance_scaler() const { return guest::LoadFloat(base, address + 92); }
  void set_curve_distance_scaler(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t curve_distance_scaler_address() const { return address + 92; }
  // 1
  float doppler_scaler() const { return guest::LoadFloat(base, address + 96); }
  void set_doppler_scaler(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t doppler_scaler_address() const { return address + 96; }
  // the cone (0, 0, 0, 1, 0, 1, 0, 1): probably no cone
  float cone_inner_angle() const { return guest::LoadFloat(base, address + 100); }
  void set_cone_inner_angle(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t cone_inner_angle_address() const { return address + 100; }
  float cone_outer_angle() const { return guest::LoadFloat(base, address + 104); }
  void set_cone_outer_angle(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t cone_outer_angle_address() const { return address + 104; }
  float cone_inner_volume() const { return guest::LoadFloat(base, address + 108); }
  void set_cone_inner_volume(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t cone_inner_volume_address() const { return address + 108; }
  float cone_outer_volume() const { return guest::LoadFloat(base, address + 112); }
  void set_cone_outer_volume(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t cone_outer_volume_address() const { return address + 112; }
  float cone_inner_lpf() const { return guest::LoadFloat(base, address + 116); }
  void set_cone_inner_lpf(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t cone_inner_lpf_address() const { return address + 116; }
  float cone_outer_lpf() const { return guest::LoadFloat(base, address + 120); }
  void set_cone_outer_lpf(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t cone_outer_lpf_address() const { return address + 120; }
  float cone_inner_reverb() const { return guest::LoadFloat(base, address + 124); }
  void set_cone_inner_reverb(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t cone_inner_reverb_address() const { return address + 124; }
  float cone_outer_reverb() const { return guest::LoadFloat(base, address + 128); }
  void set_cone_outer_reverb(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t cone_outer_reverb_address() const { return address + 128; }
  // the one channel's azimuth (0)
  float azimuth() const { return guest::LoadFloat(base, address + 132); }
  void set_azimuth(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t azimuth_address() const { return address + 132; }
  // the cue's GetProperties (xact.md's sub_825C5790): category, track count, priority, the first wave's index, a duration, the channel count
  uint32_t cue_properties_address() const { return address + 136; }
  // 1.0 (not read)
  float unknown_152() const { return guest::LoadFloat(base, address + 152); }
  void set_unknown_152(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  // 1.0 (not read)
  float unknown_156() const { return guest::LoadFloat(base, address + 156); }
  void set_unknown_156(float v) const { guest::StoreFloat(base, address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  // six (index byte, 1.0) pairs from +164 (not read: probably an output map)
  uint8_t map_0_index() const { return guest::LoadU8(base, address + 164); }
  void set_map_0_index(uint8_t v) const { guest::StoreU8(base, address + 164, v); }
  uint32_t map_0_index_address() const { return address + 164; }
  float map_0_gain() const { return guest::LoadFloat(base, address + 168); }
  void set_map_0_gain(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t map_0_gain_address() const { return address + 168; }
  uint8_t map_1_index() const { return guest::LoadU8(base, address + 172); }
  void set_map_1_index(uint8_t v) const { guest::StoreU8(base, address + 172, v); }
  uint32_t map_1_index_address() const { return address + 172; }
  float map_1_gain() const { return guest::LoadFloat(base, address + 176); }
  void set_map_1_gain(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t map_1_gain_address() const { return address + 176; }
  uint8_t map_2_index() const { return guest::LoadU8(base, address + 180); }
  void set_map_2_index(uint8_t v) const { guest::StoreU8(base, address + 180, v); }
  uint32_t map_2_index_address() const { return address + 180; }
  float map_2_gain() const { return guest::LoadFloat(base, address + 184); }
  void set_map_2_gain(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t map_2_gain_address() const { return address + 184; }
  uint8_t map_3_index() const { return guest::LoadU8(base, address + 188); }
  void set_map_3_index(uint8_t v) const { guest::StoreU8(base, address + 188, v); }
  uint32_t map_3_index_address() const { return address + 188; }
  float map_3_gain() const { return guest::LoadFloat(base, address + 192); }
  void set_map_3_gain(float v) const { guest::StoreFloat(base, address + 192, v); }
  uint32_t map_3_gain_address() const { return address + 192; }
  uint8_t map_4_index() const { return guest::LoadU8(base, address + 196); }
  void set_map_4_index(uint8_t v) const { guest::StoreU8(base, address + 196, v); }
  uint32_t map_4_index_address() const { return address + 196; }
  float map_4_gain() const { return guest::LoadFloat(base, address + 200); }
  void set_map_4_gain(float v) const { guest::StoreFloat(base, address + 200, v); }
  uint32_t map_4_gain_address() const { return address + 200; }
  uint8_t map_5_index() const { return guest::LoadU8(base, address + 204); }
  void set_map_5_index(uint8_t v) const { guest::StoreU8(base, address + 204, v); }
  uint32_t map_5_index_address() const { return address + 204; }
  float map_5_gain() const { return guest::LoadFloat(base, address + 208); }
  void set_map_5_gain(float v) const { guest::StoreFloat(base, address + 208, v); }
  uint32_t map_5_gain_address() const { return address + 208; }
  // 6: the map's count
  uint8_t map_count() const { return guest::LoadU8(base, address + 212); }
  void set_map_count(uint8_t v) const { guest::StoreU8(base, address + 212, v); }
  uint32_t map_count_address() const { return address + 212; }
  // this emitter's +164: the map's pairs
  uint32_t map() const { return guest::LoadU32(base, address + 216); }
  void set_map(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t map_address() const { return address + 216; }
  // a one-pair map: index 0 (not read)
  uint8_t single_map_index() const { return guest::LoadU8(base, address + 220); }
  void set_single_map_index(uint8_t v) const { guest::StoreU8(base, address + 220, v); }
  uint32_t single_map_index_address() const { return address + 220; }
  // 1.0
  float single_map_gain() const { return guest::LoadFloat(base, address + 224); }
  void set_single_map_gain(float v) const { guest::StoreFloat(base, address + 224, v); }
  uint32_t single_map_gain_address() const { return address + 224; }
  // 1
  uint8_t single_map_count() const { return guest::LoadU8(base, address + 268); }
  void set_single_map_count(uint8_t v) const { guest::StoreU8(base, address + 268, v); }
  uint32_t single_map_count_address() const { return address + 268; }
  // this emitter's +220
  uint32_t single_map() const { return guest::LoadU32(base, address + 272); }
  void set_single_map(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t single_map_address() const { return address + 272; }
  // 0 (not read)
  uint8_t unknown_276() const { return guest::LoadU8(base, address + 276); }
  void set_unknown_276(uint8_t v) const { guest::StoreU8(base, address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  // this emitter's +212 (not read)
  uint32_t map_descriptor() const { return guest::LoadU32(base, address + 280); }
  void set_map_descriptor(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t map_descriptor_address() const { return address + 280; }
  // 1 (not read)
  uint8_t unknown_284() const { return guest::LoadU8(base, address + 284); }
  void set_unknown_284(uint8_t v) const { guest::StoreU8(base, address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
  // this emitter's +268 (not read)
  uint32_t single_map_descriptor() const { return guest::LoadU32(base, address + 288); }
  void set_single_map_descriptor(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t single_map_descriptor_address() const { return address + 288; }
  // 1 (not read)
  uint8_t unknown_292() const { return guest::LoadU8(base, address + 292); }
  void set_unknown_292(uint8_t v) const { guest::StoreU8(base, address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  // this emitter's +276 (not read)
  uint32_t unknown_296() const { return guest::LoadU32(base, address + 296); }
  void set_unknown_296(uint32_t v) const { guest::StoreU32(base, address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
};

// allocated; measure: Emitter.+0
struct EmitterCue : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the cue's vtable (and, through the fault, its slot 0: probably Play)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vtable's slot +4: probably Stop (Emitter::Stop, with 1: at once)
  uint32_t fault_stop() const { return guest::LoadU32(base, address + 4); }
  void set_fault_stop(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t fault_stop_address() const { return address + 4; }
  // the vtable's slot +8: probably GetState (Emitter::IsNotStopped, IsPrepared)
  uint32_t fault_get_state() const { return guest::LoadU32(base, address + 8); }
  void set_fault_get_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fault_get_state_address() const { return address + 8; }
  // the vtable's slot +12: probably Destroy (Emitter::DestroyCue)
  uint32_t fault_destroy() const { return guest::LoadU32(base, address + 12); }
  void set_fault_destroy(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fault_destroy_address() const { return address + 12; }
  // the vtable's slot +60: GetProperties (Emitter::Prepare, into the emitter's +136)
  uint32_t fault_get_properties() const { return guest::LoadU32(base, address + 60); }
  void set_fault_get_properties(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t fault_get_properties_address() const { return address + 60; }
};

// allocated; measure: Emitter::Prepare.r4.+8
struct EmitterSoundBank : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = EmitterSoundBankVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Emitter::Prepare.r4.+8.+0
struct EmitterSoundBankVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // probably Prepare (the cue index, flags 0, time 0, the cue into the emitter's +0)
  uint32_t prepare() const { return guest::LoadU32(base, address + 4); }
  void set_prepare(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t prepare_address() const { return address + 4; }
};

// embedded; measure: Emitter::Prepare.r5
struct EmitterCueIndex16 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint16_t index() const { return guest::LoadU16(base, address + 0); }
  void set_index(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// allocated; measure: BankPair
struct SoundBankPair : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  static constexpr uint32_t kVtable = 0x820066DC;
  // 0x820066DC (0x82000EB8, the base's, first)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82002908 (0x82000AB4, then 0x82000BA8, first)
  uint32_t vtable2() const { return guest::LoadU32(base, address + 4); }
  void set_vtable2(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable2_address() const { return address + 4; }
  // the command base's +8, 0
  uint32_t command_8() const { return guest::LoadU32(base, address + 8); }
  void set_command_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t command_8_address() const { return address + 8; }
  // the command base's +12, 0 (the manager's handler runs sub_82094B10 at 3)
  uint32_t command_12() const { return guest::LoadU32(base, address + 12); }
  void set_command_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t command_12_address() const { return address + 12; }
  // 0
  uint8_t command_flag_16() const { return guest::LoadU8(base, address + 16); }
  void set_command_flag_16(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t command_flag_16_address() const { return address + 16; }
  // 1
  uint8_t command_flag_17() const { return guest::LoadU8(base, address + 17); }
  void set_command_flag_17(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t command_flag_17_address() const { return address + 17; }
  // 0
  uint8_t command_flag_18() const { return guest::LoadU8(base, address + 18); }
  void set_command_flag_18(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t command_flag_18_address() const { return address + 18; }
  // 0x82002850 (0x82000ABC first)
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 24); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t delegate_vtable_address() const { return address + 24; }
  // 0
  uint32_t delegate_28() const { return guest::LoadU32(base, address + 28); }
  void set_delegate_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t delegate_28_address() const { return address + 28; }
  // 0
  uint32_t delegate_32() const { return guest::LoadU32(base, address + 32); }
  void set_delegate_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t delegate_32_address() const { return address + 32; }
  // the manager's +4 interface (its slot +12 GetEngine)
  uint32_t owner() const { return guest::LoadU32(base, address + 36); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t owner_address() const { return address + 36; }
  // a std::string
  uint32_t name_address() const { return address + 40; }
  // 1 skips the wave bank's load (probably one loaded elsewhere)
  uint8_t skip_wave_bank() const { return guest::LoadU8(base, address + 68); }
  void set_skip_wave_bank(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t skip_wave_bank_address() const { return address + 68; }
  // BankPair::Release
  uint8_t release_requested() const { return guest::LoadU8(base, address + 69); }
  void set_release_requested(uint8_t v) const { guest::StoreU8(base, address + 69, v); }
  uint32_t release_requested_address() const { return address + 69; }
  template <typename T = WaveBankHolder>
  T wave_bank_holder() const { return T(base, guest::LoadU32(base, address + 72)); }
  void set_wave_bank_holder(const View& v) const { guest::StoreU32(base, address + 72, v.address); }
  void set_wave_bank_holder(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t wave_bank_holder_address() const { return address + 72; }
  template <typename T = SoundBankHolder>
  T sound_bank_holder() const { return T(base, guest::LoadU32(base, address + 76)); }
  void set_sound_bank_holder(const View& v) const { guest::StoreU32(base, address + 76, v.address); }
  void set_sound_bank_holder(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t sound_bank_holder_address() const { return address + 76; }
  uint32_t bank_index() const { return guest::LoadU32(base, address + 80); }
  void set_bank_index(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t bank_index_address() const { return address + 80; }
  uint32_t wave_bank_file_id() const { return guest::LoadU32(base, address + 84); }
  void set_wave_bank_file_id(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t wave_bank_file_id_address() const { return address + 84; }
  uint32_t sound_bank_file_id() const { return guest::LoadU32(base, address + 88); }
  void set_sound_bank_file_id(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t sound_bank_file_id_address() const { return address + 88; }
  uint32_t wave_bank_size() const { return guest::LoadU32(base, address + 92); }
  void set_wave_bank_size(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t wave_bank_size_address() const { return address + 92; }
  uint32_t sound_bank_size() const { return guest::LoadU32(base, address + 96); }
  void set_sound_bank_size(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t sound_bank_size_address() const { return address + 96; }
  // XPhysicalAlloc's (bank 2: [0x826C5C88])
  uint32_t wave_bank_buffer() const { return guest::LoadU32(base, address + 100); }
  void set_wave_bank_buffer(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t wave_bank_buffer_address() const { return address + 100; }
  // operator new[]'s (bank 2: [0x826C5C8C])
  uint32_t sound_bank_buffer() const { return guest::LoadU32(base, address + 104); }
  void set_sound_bank_buffer(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t sound_bank_buffer_address() const { return address + 104; }
  uint8_t wave_bank_loaded() const { return guest::LoadU8(base, address + 108); }
  void set_wave_bank_loaded(uint8_t v) const { guest::StoreU8(base, address + 108, v); }
  uint32_t wave_bank_loaded_address() const { return address + 108; }
  uint8_t sound_bank_loaded() const { return guest::LoadU8(base, address + 109); }
  void set_sound_bank_loaded(uint8_t v) const { guest::StoreU8(base, address + 109, v); }
  uint32_t sound_bank_loaded_address() const { return address + 109; }
};

// allocated; measure: WaveBankHolder, BankPair.+72
struct WaveBankHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t buffer() const { return guest::LoadU32(base, address + 0); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t buffer_address() const { return address + 0; }
  uint32_t size() const { return guest::LoadU32(base, address + 4); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  // the XACT wave bank
  uint32_t wave_bank() const { return guest::LoadU32(base, address + 8); }
  void set_wave_bank(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t wave_bank_address() const { return address + 8; }
  // a file handle, -1 for none (always, in play)
  uint32_t handle() const { return guest::LoadU32(base, address + 12); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t handle_address() const { return address + 12; }
};

// allocated; measure: SoundBankHolder, Emitter::Prepare.r4
struct SoundBankHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t buffer() const { return guest::LoadU32(base, address + 0); }
  void set_buffer(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t buffer_address() const { return address + 0; }
  uint32_t size() const { return guest::LoadU32(base, address + 4); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  // the XACT sound bank
  template <typename T = EmitterSoundBank>
  T sound_bank() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_sound_bank(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_sound_bank(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sound_bank_address() const { return address + 8; }
};

// embedded; measure: sub_82118438.r3, sub_82118930.r3.+1516
struct BankPairSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = SoundBankPair>
  T pair() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_pair(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_pair(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pair_address() const { return address + 0; }
};

// embedded; measure: AudioManager@+1456, AudioManager::SetListenerOrientation.r3@+1456, AudioManager::SetListenerPosition.r3@+1456, AudioManager::SetListenerParams.r3@+1456
struct SoundListener : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // front (0, 0, 1)
  float front_x() const { return guest::LoadFloat(base, address + 0); }
  void set_front_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t front_x_address() const { return address + 0; }
  float front_y() const { return guest::LoadFloat(base, address + 4); }
  void set_front_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t front_y_address() const { return address + 4; }
  float front_z() const { return guest::LoadFloat(base, address + 8); }
  void set_front_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t front_z_address() const { return address + 8; }
  // top (0, 1, 0)
  float top_x() const { return guest::LoadFloat(base, address + 12); }
  void set_top_x(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t top_x_address() const { return address + 12; }
  float top_y() const { return guest::LoadFloat(base, address + 16); }
  void set_top_y(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t top_y_address() const { return address + 16; }
  float top_z() const { return guest::LoadFloat(base, address + 20); }
  void set_top_z(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t top_z_address() const { return address + 20; }
  // (0, 0, [0x82001FD8]) at first
  float position_x() const { return guest::LoadFloat(base, address + 24); }
  void set_position_x(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t position_x_address() const { return address + 24; }
  float position_y() const { return guest::LoadFloat(base, address + 28); }
  void set_position_y(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t position_y_address() const { return address + 28; }
  float position_z() const { return guest::LoadFloat(base, address + 32); }
  void set_position_z(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t position_z_address() const { return address + 32; }
  float velocity_x() const { return guest::LoadFloat(base, address + 36); }
  void set_velocity_x(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t velocity_x_address() const { return address + 36; }
  float velocity_y() const { return guest::LoadFloat(base, address + 40); }
  void set_velocity_y(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t velocity_y_address() const { return address + 40; }
  float velocity_z() const { return guest::LoadFloat(base, address + 44); }
  void set_velocity_z(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t velocity_z_address() const { return address + 44; }
  // [0x82002A0C] at first; AudioManager::SetListenerParams's (slot 28, not seen in play)
  float param_0() const { return guest::LoadFloat(base, address + 48); }
  void set_param_0(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t param_0_address() const { return address + 48; }
  // [0x820027A4] at first; Listener::SetFloat52's
  float param_1() const { return guest::LoadFloat(base, address + 52); }
  void set_param_1(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t param_1_address() const { return address + 52; }
};

// embedded; measure: sub_8259CC80.r3
struct SoundVector36Target : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  float vector_x() const { return guest::LoadFloat(base, address + 36); }
  void set_vector_x(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t vector_x_address() const { return address + 36; }
  float vector_y() const { return guest::LoadFloat(base, address + 40); }
  void set_vector_y(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t vector_y_address() const { return address + 40; }
  float vector_z() const { return guest::LoadFloat(base, address + 44); }
  void set_vector_z(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t vector_z_address() const { return address + 44; }
};

// embedded; measure: sub_8259CC80.r4, Listener::SetOrientation.r4, Listener::SetOrientation.r5, Listener::SetPosition.r4
struct SoundVector3 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// embedded; measure: sub_82118638.r3@+16, sub_82118638.r3@+16@+8, AudioManager@+16, AudioManager::GetCategory.r3@+16, AudioManager::SetCategoryVolume.r3@+16, AudioManager::PauseCategory.r3@+16, AudioManager::ResumeCategory.r3@+16, AudioEngine::DoWork.r3
struct AudioEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // the global settings (TXR_XACT.xgs) read whole (also the fault's handle: speaker count)
  uint32_t settings() const { return guest::LoadU32(base, address + 0); }
  void set_settings(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t settings_address() const { return address + 0; }
  // its size (also the fault's handle: LFE speaker)
  uint32_t settings_size() const { return guest::LoadU32(base, address + 4); }
  void set_settings_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t settings_size_address() const { return address + 4; }
  // XACT3CreateEngine's
  template <typename T = XactEngine>
  T xact_engine() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_xact_engine(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_xact_engine(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t xact_engine_address() const { return address + 8; }
  // the X3DAudio handle (16 bytes, car-sounds' X3DAudioHandle): 6 (5.1)
  uint32_t x3d_speaker_count() const { return guest::LoadU32(base, address + 12); }
  void set_x3d_speaker_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t x3d_speaker_count_address() const { return address + 12; }
  uint32_t x3d_lfe_speaker() const { return guest::LoadU32(base, address + 16); }
  void set_x3d_lfe_speaker(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t x3d_lfe_speaker_address() const { return address + 16; }
  float x3d_speed_of_sound() const { return guest::LoadFloat(base, address + 20); }
  void set_x3d_speed_of_sound(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t x3d_speed_of_sound_address() const { return address + 20; }
  float x3d_doppler_limit() const { return guest::LoadFloat(base, address + 24); }
  void set_x3d_doppler_limit(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t x3d_doppler_limit_address() const { return address + 24; }
  // the DSP settings' matrix pointer: this engine's +76
  uint32_t dsp_matrix() const { return guest::LoadU32(base, address + 28); }
  void set_dsp_matrix(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t dsp_matrix_address() const { return address + 28; }
  // 1
  uint32_t dsp_source_channels() const { return guest::LoadU32(base, address + 36); }
  void set_dsp_source_channels(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t dsp_source_channels_address() const { return address + 36; }
  // 6
  uint32_t dsp_destination_channels() const { return guest::LoadU32(base, address + 40); }
  void set_dsp_destination_channels(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t dsp_destination_channels_address() const { return address + 40; }
  // six 1.0 (written as words)
  uint32_t matrix_0() const { return guest::LoadU32(base, address + 76); }
  void set_matrix_0(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t matrix_0_address() const { return address + 76; }
  uint32_t matrix_1() const { return guest::LoadU32(base, address + 80); }
  void set_matrix_1(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t matrix_1_address() const { return address + 80; }
  uint32_t matrix_2() const { return guest::LoadU32(base, address + 84); }
  void set_matrix_2(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t matrix_2_address() const { return address + 84; }
  uint32_t matrix_3() const { return guest::LoadU32(base, address + 88); }
  void set_matrix_3(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t matrix_3_address() const { return address + 88; }
  uint32_t matrix_4() const { return guest::LoadU32(base, address + 92); }
  void set_matrix_4(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t matrix_4_address() const { return address + 92; }
  uint32_t matrix_5() const { return guest::LoadU32(base, address + 96); }
  void set_matrix_5(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t matrix_5_address() const { return address + 96; }
};

// allocated; measure: new<196>@XACT3CreateEngine, sub_82118638.r3@+16.+8, AudioManager::GetCategory.r3@+16.+8, AudioManager::SetCategoryVolume.r3@+16.+8, AudioManager::PauseCategory.r3@+16.+8, AudioManager::ResumeCategory.r3@+16.+8, AudioEngine::DoWork.r3.+8, AudioManager@+16.+8
struct XactEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 196;
  // 0x82075360
  template <typename T = XactEngineInterface>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82075348
  uint32_t vtable2() const { return guest::LoadU32(base, address + 4); }
  void set_vtable2(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable2_address() const { return address + 4; }
  // 1 at first
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  uint32_t renderer() const { return guest::LoadU32(base, address + 12); }
  void set_renderer(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t renderer_address() const { return address + 12; }
  // Initialize's parameters' +28
  uint32_t notification_callback() const { return guest::LoadU32(base, address + 16); }
  void set_notification_callback(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t notification_callback_address() const { return address + 16; }
  // the notification callback's thread id (-1)
  uint32_t notification_thread() const { return guest::LoadU32(base, address + 20); }
  void set_notification_thread(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t notification_thread_address() const { return address + 20; }
  // the audio callback's time (the clock + [0x8266CDF4])
  uint32_t callback_time() const { return guest::LoadU32(base, address + 24); }
  void set_callback_time(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t callback_time_address() const { return address + 24; }
  // the cues' list (nodes at cue + 12)
  uint32_t cues() const { return guest::LoadU32(base, address + 28); }
  void set_cues(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t cues_address() const { return address + 28; }
  uint32_t cues_next() const { return guest::LoadU32(base, address + 32); }
  void set_cues_next(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t cues_next_address() const { return address + 32; }
  uint32_t cues_previous() const { return guest::LoadU32(base, address + 36); }
  void set_cues_previous(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t cues_previous_address() const { return address + 36; }
  // the parameters' +0 (probably a look-ahead in ms)
  uint32_t look_ahead() const { return guest::LoadU32(base, address + 40); }
  void set_look_ahead(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t look_ahead_address() const { return address + 40; }
  // the sound banks' list (nodes at bank + 12)
  uint32_t sound_banks() const { return guest::LoadU32(base, address + 44); }
  void set_sound_banks(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t sound_banks_address() const { return address + 44; }
  uint32_t sound_banks_next() const { return guest::LoadU32(base, address + 48); }
  void set_sound_banks_next(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t sound_banks_next_address() const { return address + 48; }
  uint32_t sound_banks_previous() const { return guest::LoadU32(base, address + 52); }
  void set_sound_banks_previous(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t sound_banks_previous_address() const { return address + 52; }
  // the wave banks' list (nodes at bank + 8)
  uint32_t wave_banks() const { return guest::LoadU32(base, address + 56); }
  void set_wave_banks(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t wave_banks_address() const { return address + 56; }
  uint32_t wave_banks_next() const { return guest::LoadU32(base, address + 60); }
  void set_wave_banks_next(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t wave_banks_next_address() const { return address + 60; }
  uint32_t wave_banks_previous() const { return guest::LoadU32(base, address + 64); }
  void set_wave_banks_previous(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t wave_banks_previous_address() const { return address + 64; }
  // set by Initialize
  uint32_t initialized() const { return guest::LoadU32(base, address + 68); }
  void set_initialized(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t initialized_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  // objects to delete in DoWork
  uint32_t deletes() const { return guest::LoadU32(base, address + 76); }
  void set_deletes(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t deletes_address() const { return address + 76; }
  uint32_t deletes_next() const { return guest::LoadU32(base, address + 80); }
  void set_deletes_next(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t deletes_next_address() const { return address + 80; }
  uint32_t deletes_previous() const { return guest::LoadU32(base, address + 84); }
  void set_deletes_previous(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t deletes_previous_address() const { return address + 84; }
  // a list head (not read)
  uint32_t list_88() const { return guest::LoadU32(base, address + 88); }
  void set_list_88(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t list_88_address() const { return address + 88; }
  uint32_t list_88_next() const { return guest::LoadU32(base, address + 92); }
  void set_list_88_next(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t list_88_next_address() const { return address + 92; }
  uint32_t list_88_previous() const { return guest::LoadU32(base, address + 96); }
  void set_list_88_previous(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t list_88_previous_address() const { return address + 96; }
  // the notification registrations
  uint32_t notifications() const { return guest::LoadU32(base, address + 100); }
  void set_notifications(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t notifications_address() const { return address + 100; }
  uint32_t notifications_next() const { return guest::LoadU32(base, address + 104); }
  void set_notifications_next(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t notifications_next_address() const { return address + 104; }
  uint32_t notifications_previous() const { return guest::LoadU32(base, address + 108); }
  void set_notifications_previous(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t notifications_previous_address() const { return address + 108; }
  // objects to delete in DoWork
  uint32_t deletes_2() const { return guest::LoadU32(base, address + 112); }
  void set_deletes_2(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t deletes_2_address() const { return address + 112; }
  uint32_t deletes_2_next() const { return guest::LoadU32(base, address + 116); }
  void set_deletes_2_next(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t deletes_2_next_address() const { return address + 116; }
  uint32_t deletes_2_previous() const { return guest::LoadU32(base, address + 120); }
  void set_deletes_2_previous(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t deletes_2_previous_address() const { return address + 120; }
  uint32_t global_settings() const { return guest::LoadU32(base, address + 124); }
  void set_global_settings(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t global_settings_address() const { return address + 124; }
  // cues waiting on an instance limit (never filled in play)
  uint32_t waiting_cues() const { return guest::LoadU32(base, address + 128); }
  void set_waiting_cues(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t waiting_cues_address() const { return address + 128; }
  uint32_t waiting_cues_next() const { return guest::LoadU32(base, address + 132); }
  void set_waiting_cues_next(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t waiting_cues_next_address() const { return address + 132; }
  uint32_t waiting_cues_previous() const { return guest::LoadU32(base, address + 136); }
  void set_waiting_cues_previous(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t waiting_cues_previous_address() const { return address + 136; }
  uint32_t unknown_140() const { return guest::LoadU32(base, address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  // a critical section (RtlInitializeCriticalSection)
  uint32_t lock_address() const { return address + 144; }
  uint32_t start_time() const { return guest::LoadU32(base, address + 172); }
  void set_start_time(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t start_time_address() const { return address + 172; }
  uint32_t unknown_176() const { return guest::LoadU32(base, address + 176); }
  void set_unknown_176(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  // the audio callback does nothing when set
  uint32_t callback_disabled() const { return guest::LoadU32(base, address + 180); }
  void set_callback_disabled(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t callback_disabled_address() const { return address + 180; }
  // deferred voices to start
  uint32_t deferred_voices() const { return guest::LoadU32(base, address + 184); }
  void set_deferred_voices(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t deferred_voices_address() const { return address + 184; }
  // the parameters' +20 (probably a file I/O callback; 0x82295FC8)
  uint32_t file_callback_0() const { return guest::LoadU32(base, address + 188); }
  void set_file_callback_0(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t file_callback_0_address() const { return address + 188; }
  // the parameters' +24 (0x82295F20)
  uint32_t file_callback_1() const { return guest::LoadU32(base, address + 192); }
  void set_file_callback_1(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t file_callback_1_address() const { return address + 192; }
};

// image; measure: sub_82118638.r3@+16.+8.+0, AudioManager::GetCategory.r3@+16.+8.+0, AudioManager::SetCategoryVolume.r3@+16.+8.+0, AudioManager::PauseCategory.r3@+16.+8.+0, AudioManager::ResumeCategory.r3@+16.+8.+0, AudioEngine::DoWork.r3.+8.+0, AudioManager@+16.+8.+0
struct XactEngineInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // called by sub_8259D370; not read
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  // probably Initialize (sub_8259D1F0)
  uint32_t initialize() const { return guest::LoadU32(base, address + 8); }
  void set_initialize(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t initialize_address() const { return address + 8; }
  // probably DoWork (AudioEngine::DoWork)
  uint32_t do_work() const { return guest::LoadU32(base, address + 16); }
  void set_do_work(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t do_work_address() const { return address + 16; }
  // probably CreateSoundBank
  uint32_t create_sound_bank() const { return guest::LoadU32(base, address + 20); }
  void set_create_sound_bank(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t create_sound_bank_address() const { return address + 20; }
  // probably CreateInMemoryWaveBank
  uint32_t create_in_memory_wave_bank() const { return guest::LoadU32(base, address + 24); }
  void set_create_in_memory_wave_bank(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t create_in_memory_wave_bank_address() const { return address + 24; }
  // probably GetCategory
  uint32_t get_category() const { return guest::LoadU32(base, address + 40); }
  void set_get_category(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t get_category_address() const { return address + 40; }
  // probably SetVolume (a category's)
  uint32_t set_volume() const { return guest::LoadU32(base, address + 48); }
  void set_set_volume(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t set_volume_address() const { return address + 48; }
  // probably Pause (a category's: 1 pause, 0 resume)
  uint32_t pause() const { return guest::LoadU32(base, address + 52); }
  void set_pause(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t pause_address() const { return address + 52; }
};

// allocated; measure: AudioManager.+12
struct AudioManagerOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AudioManagerOwnerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: AudioManager.+12.+0
struct AudioManagerOwnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot 1: the object the manager's delegates are registered with
  uint32_t handler_registry() const { return guest::LoadU32(base, address + 4); }
  void set_handler_registry(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t handler_registry_address() const { return address + 4; }
  // slot 2: the object the manager's command (+1584) is queued on
  uint32_t command_queue() const { return guest::LoadU32(base, address + 8); }
  void set_command_queue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t command_queue_address() const { return address + 8; }
};

// allocated; measure: sub_82119188.r4
struct AudioManagerMessageSource : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AudioManagerMessageSourceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: sub_82119188.r4.+0
struct AudioManagerMessageSourceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // with 8 (probably a subscription)
  uint32_t slot_20() const { return guest::LoadU32(base, address + 20); }
  void set_slot_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot_20_address() const { return address + 20; }
  // with 6
  uint32_t slot_24() const { return guest::LoadU32(base, address + 24); }
  void set_slot_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot_24_address() const { return address + 24; }
  // with 1
  uint32_t slot_28() const { return guest::LoadU32(base, address + 28); }
  void set_slot_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t slot_28_address() const { return address + 28; }
  // with 7
  uint32_t slot_32() const { return guest::LoadU32(base, address + 32); }
  void set_slot_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t slot_32_address() const { return address + 32; }
};

// embedded; measure: sub_82119188.r5
struct AudioManagerMessage : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 5 or 6 handled
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
};

// allocated; measure: new<16>@sub_82118720, sub_82118720.r3
struct AudioManagerDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 0x8200833C
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the manager
  uint32_t object() const { return guest::LoadU32(base, address + 4); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_address() const { return address + 4; }
  // the member pointer's function
  uint32_t method() const { return guest::LoadU32(base, address + 8); }
  void set_method(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t method_address() const { return address + 8; }
  // the member pointer's this adjustment
  uint32_t method_adjust() const { return guest::LoadU32(base, address + 12); }
  void set_method_adjust(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t method_adjust_address() const { return address + 12; }
};

// embedded; measure: AudioManager::RemoveStoppedSounds.r3@+1528
struct SoundList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // probably the allocator word
  uint32_t allocator() const { return guest::LoadU32(base, address + 0); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t allocator_address() const { return address + 0; }
  // the sentinel node
  template <typename T = SoundListNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
};

// allocated; measure: AudioManager.+1532, AudioManager.+1532.+0, sub_82119018.r3.+1532, AudioManager::RemoveStoppedSounds.r3@+1528.+4, AudioManager::RemoveStoppedSounds.r3@+1528.+4.+0
struct SoundListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = SoundListNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = SoundListNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  template <typename T = PlayingSound>
  T sound() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_sound(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_sound(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sound_address() const { return address + 8; }
};

// embedded; measure: sub_82118F50.r3
struct SoundForEachResult : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t function() const { return guest::LoadU32(base, address + 0); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t function_address() const { return address + 0; }
};

// embedded; measure: WordVector
struct WordVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // probably the allocator word (never written here)
  uint32_t allocator() const { return guest::LoadU32(base, address + 0); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t allocator_address() const { return address + 0; }
  // read as a doubleword (probably a measure fault: an iterator pair joined)
  uint64_t fault_0() const { return guest::LoadU64(base, address + 0); }
  void set_fault_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_0_address() const { return address + 0; }
  // MessageVector::Allocate's block (cameras' NewAny_MessageVector_Allocate)
  uint32_t first() const { return guest::LoadU32(base, address + 4); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  uint32_t last() const { return guest::LoadU32(base, address + 8); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  // the capacity's end
  uint32_t end() const { return guest::LoadU32(base, address + 12); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// embedded; measure: AudioManager::LoadBank.r4, AudioManager::ReleaseBank.r4, AudioManager::IsBankLoading.r4, AudioManager::IsBankSlotEmpty.r4, AudioManager::GetSoundBank.r4, AudioManager::PlayCue.r4, Sound::Sound.r4, Sound::IsBank.r4, AudioManager::FindFreeBankSlot.r4, AudioManager::GetCueIndex.r4, AudioManager::IsBankLoaded.r4, AudioManager::PlayHandle.r4
struct SoundCuesBankIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: AudioManager::PlayCue.r5, Sound::Sound.r5, Sound::IsCue.r4, AudioManager::PlayHandle.r5
struct SoundCuesCueIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: AudioManager::LoadBank.r7, AudioManager::LoadBank.r8
struct BankFileId : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
};

// embedded; measure: BankPair::BankPair.r5
struct BankSkipWaveArg : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t value() const { return guest::LoadU8(base, address + 0); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: AudioManager::AudioManager.r6
struct AudioManagerBankCount : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
};

// embedded; measure: WordVector::Fill.r5
struct WordVectorFillValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: Sound::Set3D.r4
struct Sound3DArg : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t value() const { return guest::LoadU8(base, address + 0); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: AudioEngine::SetCategoryVolume.r4, AudioEngine::PauseCategory.r4, AudioEngine::ResumeCategory.r4
struct AudioCategoryArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t category() const { return guest::LoadU32(base, address + 0); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t category_address() const { return address + 0; }
};

// embedded; measure: AudioEngine::SetCategoryVolume.r5
struct AudioVolumeArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float volume() const { return guest::LoadFloat(base, address + 0); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t volume_address() const { return address + 0; }
};

// embedded; measure: AudioManager@+16@+12
struct AudioEngineAt12 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: AudioManager.+1544.+0@+8
struct PlayingSoundAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: BankPair@+72
struct SoundBankPairAt72 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: BankPair@+76
struct SoundBankPairAt76 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: Sound@+4
struct PlayingSoundAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: AudioManager@+1512
struct WordVectorAt1512 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_82090858.r3@-12
struct SoundBankPairOuter12 : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  uint32_t unknown_76() const { return guest::LoadU32(base, address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
};

// The image words docs/functions/sound-cues.md's code uses.
struct SoundCuesGlobals {
  // 0.0 (copied as a float)
  static uint32_t float_zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
  // 1.0 (copied as a float)
  static uint32_t float_one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_float_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t float_one_address() { return 0x82000A30u; }
  // the listener's first position's z
  static float listener_start_z(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_listener_start_z(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t listener_start_z_address() { return 0x82001FD8u; }
  // the listener's +48 at first
  static float listener_start_param_0(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_listener_start_param_0(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t listener_start_param_0_address() { return 0x82002A0Cu; }
  // the listener's +52 at first
  static float listener_start_param_1(uint8_t* base) { return guest::LoadFloat(base, 0x820027A4u); }
  static void set_listener_start_param_1(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A4u, v); }
  static constexpr uint32_t listener_start_param_1_address() { return 0x820027A4u; }
  // AudioEngine::Start3D's speed of sound
  static float speed_of_sound(uint8_t* base) { return guest::LoadFloat(base, 0x8207145Cu); }
  static void set_speed_of_sound(uint8_t* base, float v) { guest::StoreFloat(base, 0x8207145Cu, v); }
  static constexpr uint32_t speed_of_sound_address() { return 0x8207145Cu; }
  // bank pair 2's wave bank buffer (3 MiB, SoundGlobals::Init's)
  static uint32_t bank2_wave_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826C5C88u); }
  static void set_bank2_wave_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C88u, v); }
  static constexpr uint32_t bank2_wave_buffer_address() { return 0x826C5C88u; }
  // bank pair 2's sound bank buffer (512 bytes)
  static uint32_t bank2_sound_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826C5C8Cu); }
  static void set_bank2_sound_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C8Cu, v); }
  static constexpr uint32_t bank2_sound_buffer_address() { return 0x826C5C8Cu; }
  // XACT's engine, made once by XACT3CreateEngine
  template <typename T = XactEngine>
  static T xact_engine(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82971478u)); }
  static void set_xact_engine(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82971478u, v.address); }
  static void set_xact_engine(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82971478u, v); }
  static constexpr uint32_t xact_engine_address() { return 0x82971478u; }
};

}  // namespace schema
