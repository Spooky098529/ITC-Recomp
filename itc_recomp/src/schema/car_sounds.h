// The views of docs/functions/car-sounds.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-sounds.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: CarSound, CarSounds
struct CarSound : View {
  using View::View;
  static constexpr uint32_t kSize = 1264;
  // the first element only: operator new[]'s count (7) in the array's block
  uint32_t array_count() const { return guest::LoadU32(base, address + -16); }
  void set_array_count(uint32_t v) const { guest::StoreU32(base, address + -16, v); }
  uint32_t array_count_address() const { return address + -16; }
  // the submix's reverb state: 1 the block 0x826166F8 set (the vehicle's +5314 bit 1, probably a tunnel), 0 the block 0x82616638
  uint32_t tunnel_reverb() const { return guest::LoadU32(base, address + 0); }
  void set_tunnel_reverb(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tunnel_reverb_address() const { return address + 0; }
  // an X3DAudioListener; its cone pointer is the emitter's +0 (+52)
  uint32_t listener_address() const { return address + 4; }
  // an X3DAudioEmitter (its cone pointer +52 points at +144)
  uint32_t emitter_address() const { return address + 52; }
  // an X3DAudioCone
  uint32_t cone_address() const { return address + 144; }
  // an X3DAudioDspSettings: +176 the matrix (24 bytes from operator new[]), +184 1 source, +188 6 destination channels, +200 the reverb level, +204 the Doppler factor
  uint32_t dsp_address() const { return address + 176; }
  // six {channel byte, gain float} pairs: the reverb level times the volume
  uint32_t reverb_map_address() const { return address + 224; }
  // six {channel byte, gain float} pairs: the matrix times the volume
  uint32_t output_map_address() const { return address + 272; }
  // 6
  uint8_t reverb_map_count() const { return guest::LoadU8(base, address + 320); }
  void set_reverb_map_count(uint8_t v) const { guest::StoreU8(base, address + 320, v); }
  uint32_t reverb_map_count_address() const { return address + 320; }
  // +224
  uint32_t reverb_map_pointer() const { return guest::LoadU32(base, address + 324); }
  void set_reverb_map_pointer(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t reverb_map_pointer_address() const { return address + 324; }
  // 6
  uint8_t output_map_count() const { return guest::LoadU8(base, address + 328); }
  void set_output_map_count(uint8_t v) const { guest::StoreU8(base, address + 328, v); }
  uint32_t output_map_count_address() const { return address + 328; }
  // +272
  uint32_t output_map_pointer() const { return guest::LoadU32(base, address + 332); }
  void set_output_map_pointer(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t output_map_pointer_address() const { return address + 332; }
  // 0
  uint8_t sends_0_kind() const { return guest::LoadU8(base, address + 336); }
  void set_sends_0_kind(uint8_t v) const { guest::StoreU8(base, address + 336, v); }
  uint32_t sends_0_kind_address() const { return address + 336; }
  // +328
  uint32_t sends_0() const { return guest::LoadU32(base, address + 340); }
  void set_sends_0(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t sends_0_address() const { return address + 340; }
  // 1
  uint8_t sends_1_kind() const { return guest::LoadU8(base, address + 344); }
  void set_sends_1_kind(uint8_t v) const { guest::StoreU8(base, address + 344, v); }
  uint32_t sends_1_kind_address() const { return address + 344; }
  // +320
  uint32_t sends_1() const { return guest::LoadU32(base, address + 348); }
  void set_sends_1(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t sends_1_address() const { return address + 348; }
  // the DSP output: the 8 bytes the parts' updates are given as r5 (+352 2, +356 +336)
  uint64_t output() const { return guest::LoadU64(base, address + 352); }
  void set_output(uint64_t v) const { guest::StoreU64(base, address + 352, v); }
  uint32_t output_address() const { return address + 352; }
  // 2
  uint8_t output_count() const { return guest::LoadU8(base, address + 352); }
  void set_output_count(uint8_t v) const { guest::StoreU8(base, address + 352, v); }
  uint32_t output_count_address() const { return address + 352; }
  // +336
  uint32_t output_sends() const { return guest::LoadU32(base, address + 356); }
  void set_output_sends(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t output_sends_address() const { return address + 356; }
  // the emitter's position as a vector (x, y, z, w; stvx and lvx), last tick's for the velocity
  float emitter_position_x() const { return guest::LoadFloat(base, address + 368); }
  void set_emitter_position_x(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t emitter_position_x_address() const { return address + 368; }
  float emitter_position_y() const { return guest::LoadFloat(base, address + 372); }
  void set_emitter_position_y(float v) const { guest::StoreFloat(base, address + 372, v); }
  uint32_t emitter_position_y_address() const { return address + 372; }
  float emitter_position_z() const { return guest::LoadFloat(base, address + 376); }
  void set_emitter_position_z(float v) const { guest::StoreFloat(base, address + 376, v); }
  uint32_t emitter_position_z_address() const { return address + 376; }
  float emitter_position_w() const { return guest::LoadFloat(base, address + 380); }
  void set_emitter_position_w(float v) const { guest::StoreFloat(base, address + 380, v); }
  uint32_t emitter_position_w_address() const { return address + 380; }
  // the listener's position as a vector (x, y, z, w), last tick's for the velocity
  float listener_position_x() const { return guest::LoadFloat(base, address + 384); }
  void set_listener_position_x(float v) const { guest::StoreFloat(base, address + 384, v); }
  uint32_t listener_position_x_address() const { return address + 384; }
  float listener_position_y() const { return guest::LoadFloat(base, address + 388); }
  void set_listener_position_y(float v) const { guest::StoreFloat(base, address + 388, v); }
  uint32_t listener_position_y_address() const { return address + 388; }
  float listener_position_z() const { return guest::LoadFloat(base, address + 392); }
  void set_listener_position_z(float v) const { guest::StoreFloat(base, address + 392, v); }
  uint32_t listener_position_z_address() const { return address + 392; }
  float listener_position_w() const { return guest::LoadFloat(base, address + 396); }
  void set_listener_position_w(float v) const { guest::StoreFloat(base, address + 396, v); }
  uint32_t listener_position_w_address() const { return address + 396; }
  // 0 when the car sound has no racer
  template <typename T = Vehicle>
  T vehicle() const { return T(base, guest::LoadU32(base, address + 400)); }
  void set_vehicle(const View& v) const { guest::StoreU32(base, address + 400, v.address); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 400, v); }
  uint32_t vehicle_address() const { return address + 400; }
  // the car's index, 0 to 6
  uint8_t index() const { return guest::LoadU8(base, address + 404); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 404, v); }
  uint32_t index_address() const { return address + 404; }
  // a CarSoundEngine
  uint32_t engine_address() const { return address + 408; }
  // a CarSoundHits
  uint32_t hits_address() const { return address + 584; }
  // a CarSoundTires (its voice holder at +676)
  uint32_t tires_address() const { return address + 652; }
  // a CarSoundGear (sub_8210EBF0, probably the gear change)
  uint32_t gear_address() const { return address + 736; }
  // a CarSoundRoll
  uint32_t roll_address() const { return address + 792; }
  // a CarSoundEvents (sub_8210ECF0)
  uint32_t events_address() const { return address + 840; }
  // a CarSoundFade (sub_8210EEE0)
  uint32_t fade_address() const { return address + 888; }
  // a CarSoundTurbo (probably)
  uint32_t turbo_address() const { return address + 952; }
  // a CarSoundWind
  uint32_t wind_address() const { return address + 1016; }
  // a CarSoundHorn (probably)
  uint32_t horn_address() const { return address + 1072; }
  // a CarSoundPassBy
  uint32_t pass_by_address() const { return address + 1136; }
  // the emitter's volume curve: six {distance, value} float pairs
  uint32_t volume_curve_points_address() const { return address + 1192; }
  // an X3DAudioCurve over +1192 (six points), the emitter's +64
  uint32_t volume_curve_address() const { return address + 1240; }
  // 1 once CarSounds::SetBanks gave it its banks; 0 stopped
  uint32_t live() const { return guest::LoadU32(base, address + 1248); }
  void set_live(uint32_t v) const { guest::StoreU32(base, address + 1248, v); }
  uint32_t live_address() const { return address + 1248; }
  // 1.0; EngineSound::Update's f2
  float engine_gain() const { return guest::LoadFloat(base, address + 1252); }
  void set_engine_gain(float v) const { guest::StoreFloat(base, address + 1252, v); }
  uint32_t engine_gain_address() const { return address + 1252; }
  // the last tick's Doppler factor (+204)
  float last_doppler() const { return guest::LoadFloat(base, address + 1256); }
  void set_last_doppler(float v) const { guest::StoreFloat(base, address + 1256, v); }
  uint32_t last_doppler_address() const { return address + 1256; }
  // 1 the tick a car passed close (PassBySound::Update's r6)
  uint32_t passed_close() const { return guest::LoadU32(base, address + 1260); }
  void set_passed_close(uint32_t v) const { guest::StoreU32(base, address + 1260, v); }
  uint32_t passed_close_address() const { return address + 1260; }
};

// allocated; measure: new<8864>@CarSounds::Init
struct CarSoundArray : View {
  using View::View;
  static constexpr uint32_t kSize = 8864;
  // 7
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // seven CarSounds
  uint32_t sounds_address() const { return address + 16; }
};

// embedded; measure: -
struct CarSoundVoiceHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  static constexpr uint32_t kVtable = 0x82010B5C;
  // 0x82010B5C
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // four XAudio source voices by slot (0 a free slot)
  uint32_t voices_address() const { return address + 4; }
  uint32_t voice_count() const { return guest::LoadU32(base, address + 20); }
  void set_voice_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t voice_count_address() const { return address + 20; }
  // four words: which voices were playing when paused
  uint32_t paused_address() const { return address + 24; }
};

// embedded; measure: -
struct CarSoundPart : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  // the effects' bank
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  // the car's index
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
};

// embedded; measure: EngineSound
struct CarSoundEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  static constexpr uint32_t kVtable = 0x82008018;
  // a CarSoundEngineLayer (from the second wave bank view)
  uint32_t layer_address() const { return address + 0; }
  // a CarSoundEngineLayer (from the first wave bank view)
  uint32_t pitched_layer_address() const { return address + 84; }
  // the load, eased toward the throttle's target (probably)
  float load() const { return guest::LoadFloat(base, address + 168); }
  void set_load(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t load_address() const { return address + 168; }
  // the speed without a vehicle: up by 10 a tick, back to 800 past 7,000
  float idle_speed() const { return guest::LoadFloat(base, address + 172); }
  void set_idle_speed(float v) const { guest::StoreFloat(base, address + 172, v); }
  uint32_t idle_speed_address() const { return address + 172; }
};

// embedded; measure: EngineLayer
struct CarSoundEngineLayer : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  static constexpr uint32_t kVtable = 0x82008018;
  // 0x82008018
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the car's sound parameters (CarSounds::SetBanks; 0 none)
  template <typename T = CarSoundEngineTable>
  T table() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_table(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_table(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t table_address() const { return address + 4; }
  // the same pointer
  uint32_t table_copy() const { return guest::LoadU32(base, address + 8); }
  void set_table_copy(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t table_copy_address() const { return address + 8; }
  // the table's first word
  uint32_t table_volume() const { return guest::LoadU32(base, address + 12); }
  void set_table_volume(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t table_volume_address() const { return address + 12; }
  // the same word
  uint32_t table_volume_copy() const { return guest::LoadU32(base, address + 16); }
  void set_table_volume_copy(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t table_volume_copy_address() const { return address + 16; }
  // a float per table entry: the volume's amount from its first curve
  uint32_t amounts_address() const { return address + 20; }
  // a CarSoundVoiceHolder: one voice per table entry
  uint32_t holder_address() const { return address + 44; }
};

// image; measure: EngineLayer.+4
struct CarSoundEngineTable : View {
  using View::View;
  static constexpr uint32_t kSize = 548;
  // an integer: the volume's scale (times 1/128)
  uint32_t volume_scale() const { return guest::LoadU32(base, address + 0); }
  void set_volume_scale(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t volume_scale_address() const { return address + 0; }
  // CarSoundEngineEntry records of 136 bytes, while the entry's id is above -1
  uint32_t entries_address() const { return address + 4; }
};

// image; measure: -
struct CarSoundEngineEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 136;
  // -1 (or below) ends the table
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // an integer, SetPitchScaled's r9
  uint32_t pitch_scale() const { return guest::LoadU32(base, address + 4); }
  void set_pitch_scale(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t pitch_scale_address() const { return address + 4; }
  // eight (x, y) float pairs, ended by an x at most -1.0
  uint32_t volume_curve_address() const { return address + 8; }
  // eight (x, y) float pairs
  uint32_t pitch_curve_address() const { return address + 72; }
};

// embedded; measure: EngineSound::Start.r5
struct CarSoundEngineViews : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = CarSoundWaveBankView>
  T pitched_layer_view() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_pitched_layer_view(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_pitched_layer_view(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pitched_layer_view_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T layer_view() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_layer_view(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_layer_view(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t layer_view_address() const { return address + 4; }
};

// embedded; measure: HitSounds
struct CarSoundHits : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  // the effects' bank
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  // the car's index
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // ticks to wait before the next hit (unsigned)
  uint32_t wait_ticks() const { return guest::LoadU32(base, address + 48); }
  void set_wait_ticks(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t wait_ticks_address() const { return address + 48; }
  // ticks out of contact while the scrape plays
  uint32_t scrape_ticks() const { return guest::LoadU32(base, address + 52); }
  void set_scrape_ticks(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t scrape_ticks_address() const { return address + 52; }
  // the looping wave 10's slot, -1 none
  uint32_t scrape_slot() const { return guest::LoadU32(base, address + 56); }
  void set_scrape_slot(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t scrape_slot_address() const { return address + 56; }
  // the last impact's strength (fctiwz)
  uint32_t strength() const { return guest::LoadU32(base, address + 60); }
  void set_strength(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t strength_address() const { return address + 60; }
  // ticks since the last impact, up to 32 (then -1)
  uint32_t since_impact() const { return guest::LoadU32(base, address + 64); }
  void set_since_impact(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t since_impact_address() const { return address + 64; }
};

// embedded; measure: TireSound, TireLayer
struct CarSoundTires : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  // the layer record (TireLayer::Update's r4): the voice's slot
  uint32_t slot() const { return guest::LoadU32(base, address + 0); }
  void set_slot(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slot_address() const { return address + 0; }
  // the last tick's `active`
  uint32_t was_active() const { return guest::LoadU32(base, address + 4); }
  void set_was_active(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t was_active_address() const { return address + 4; }
  // the ticks slipped
  uint8_t slip_ticks() const { return guest::LoadU8(base, address + 8); }
  void set_slip_ticks(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t slip_ticks_address() const { return address + 8; }
  uint8_t ramp_step() const { return guest::LoadU8(base, address + 9); }
  void set_ramp_step(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t ramp_step_address() const { return address + 9; }
  uint8_t ramp_length() const { return guest::LoadU8(base, address + 10); }
  void set_ramp_length(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t ramp_length_address() const { return address + 10; }
  // the second layer record's (set 1 by TireSound::Start)
  uint32_t slot_2() const { return guest::LoadU32(base, address + 12); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_2_address() const { return address + 12; }
  uint32_t was_active_2() const { return guest::LoadU32(base, address + 16); }
  void set_was_active_2(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t was_active_2_address() const { return address + 16; }
  uint8_t slip_ticks_2() const { return guest::LoadU8(base, address + 20); }
  void set_slip_ticks_2(uint8_t v) const { guest::StoreU8(base, address + 20, v); }
  uint32_t slip_ticks_2_address() const { return address + 20; }
  uint8_t ramp_step_2() const { return guest::LoadU8(base, address + 21); }
  void set_ramp_step_2(uint8_t v) const { guest::StoreU8(base, address + 21, v); }
  uint32_t ramp_step_2_address() const { return address + 21; }
  uint8_t ramp_length_2() const { return guest::LoadU8(base, address + 22); }
  void set_ramp_length_2(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t ramp_length_2_address() const { return address + 22; }
  // a CarSoundVoiceHolder (+28 and +32 its voices)
  uint32_t holder_address() const { return address + 24; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 64)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 64, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t view_address() const { return address + 64; }
  uint8_t index() const { return guest::LoadU8(base, address + 68); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 68, v); }
  uint32_t index_address() const { return address + 68; }
  // up by one a slipping tick
  uint16_t slip_steps() const { return guest::LoadU16(base, address + 70); }
  void set_slip_steps(uint16_t v) const { guest::StoreU16(base, address + 70, v); }
  uint32_t slip_steps_address() const { return address + 70; }
  // up by 1.0 a slipping tick
  float slip_time() const { return guest::LoadFloat(base, address + 72); }
  void set_slip_time(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t slip_time_address() const { return address + 72; }
  // a signed half-word, stepped a third of the way to its target
  uint16_t pitch() const { return guest::LoadU16(base, address + 76); }
  void set_pitch(uint16_t v) const { guest::StoreU16(base, address + 76, v); }
  uint32_t pitch_address() const { return address + 76; }
  float volume() const { return guest::LoadFloat(base, address + 80); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t volume_address() const { return address + 80; }
};

// embedded; measure: -
struct CarSoundGear : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // 32 when played, down by one while the vehicle's byte +2948 is clear
  uint32_t wait_ticks() const { return guest::LoadU32(base, address + 48); }
  void set_wait_ticks(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t wait_ticks_address() const { return address + 48; }
  // -1 none
  uint32_t slot() const { return guest::LoadU32(base, address + 52); }
  void set_slot(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t slot_address() const { return address + 52; }
};

// embedded; measure: RollSound
struct CarSoundRoll : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // a CarSoundVoiceHolder (+4 the voice)
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
};

// embedded; measure: -
struct CarSoundEvents : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // 2 when wave 18 played, down by one a tick
  uint8_t wave18_wait() const { return guest::LoadU8(base, address + 45); }
  void set_wave18_wait(uint8_t v) const { guest::StoreU8(base, address + 45, v); }
  uint32_t wave18_wait_address() const { return address + 45; }
};

// embedded; measure: -
struct CarSoundFade : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  uint32_t playing() const { return guest::LoadU32(base, address + 48); }
  void set_playing(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t playing_address() const { return address + 48; }
  // -1 none (used without a check)
  uint32_t slot() const { return guest::LoadU32(base, address + 52); }
  void set_slot(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t slot_address() const { return address + 52; }
  // -1.0 none
  float fade_out() const { return guest::LoadFloat(base, address + 56); }
  void set_fade_out(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t fade_out_address() const { return address + 56; }
  // -1.0 none
  float fade_in() const { return guest::LoadFloat(base, address + 60); }
  void set_fade_in(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t fade_in_address() const { return address + 60; }
};

// embedded; measure: TurboSound
struct CarSoundTurbo : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // the looping wave 3's slot, -1 none
  uint32_t slot() const { return guest::LoadU32(base, address + 48); }
  void set_slot(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot_address() const { return address + 48; }
  float last_boost() const { return guest::LoadFloat(base, address + 52); }
  void set_last_boost(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t last_boost_address() const { return address + 52; }
  // 0 to 3072 (+ 8192 for SetPitch)
  uint16_t pitch() const { return guest::LoadU16(base, address + 56); }
  void set_pitch(uint16_t v) const { guest::StoreU16(base, address + 56, v); }
  uint32_t pitch_address() const { return address + 56; }
  // 16 when started, counted down
  uint8_t wait_ticks() const { return guest::LoadU8(base, address + 58); }
  void set_wait_ticks(uint8_t v) const { guest::StoreU8(base, address + 58, v); }
  uint32_t wait_ticks_address() const { return address + 58; }
  uint8_t volume_steps() const { return guest::LoadU8(base, address + 59); }
  void set_volume_steps(uint8_t v) const { guest::StoreU8(base, address + 59, v); }
  uint32_t volume_steps_address() const { return address + 59; }
  float volume() const { return guest::LoadFloat(base, address + 60); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t volume_address() const { return address + 60; }
};

// embedded; measure: WindSound
struct CarSoundWind : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // 1 once wave 5 played
  uint32_t played() const { return guest::LoadU32(base, address + 48); }
  void set_played(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t played_address() const { return address + 48; }
  // -1 from CarSound::Start; not read
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: HornSound
struct CarSoundHorn : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // -1 none
  uint32_t slot() const { return guest::LoadU32(base, address + 48); }
  void set_slot(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot_address() const { return address + 48; }
  // race mode 8's other record's
  uint32_t slot_2() const { return guest::LoadU32(base, address + 52); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t slot_2_address() const { return address + 52; }
  uint32_t playing() const { return guest::LoadU32(base, address + 56); }
  void set_playing(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t playing_address() const { return address + 56; }
  uint32_t playing_2() const { return guest::LoadU32(base, address + 60); }
  void set_playing_2(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t playing_2_address() const { return address + 60; }
};

// embedded; measure: PassBySound
struct CarSoundPassBy : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_view(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t view_address() const { return address + 40; }
  uint8_t index() const { return guest::LoadU8(base, address + 44); }
  void set_index(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t index_address() const { return address + 44; }
  // -1 from CarSound::Start; not read
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  // 0 from CarSound::Start; not read
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// allocated; measure: new<?>@RaceSounds::Begin
struct CarSoundWaveBank : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // the bank data's offset
  uint32_t data_offset() const { return guest::LoadU32(base, address + 8); }
  void set_data_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_offset_address() const { return address + 8; }
  // the entries' offset
  uint32_t entries_offset() const { return guest::LoadU32(base, address + 16); }
  void set_entries_offset(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t entries_offset_address() const { return address + 16; }
  // the wave data's offset
  uint32_t wave_data_offset() const { return guest::LoadU32(base, address + 40); }
  void set_wave_data_offset(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t wave_data_offset_address() const { return address + 40; }
};

// allocated; measure: WaveBankView
struct CarSoundWaveBankView : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  template <typename T = CarSoundWaveBank>
  T bank() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_bank(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_bank(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t bank_address() const { return address + 0; }
  // 0; not read
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  template <typename T = CarSoundWaveBank>
  T bank_copy() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_bank_copy(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_bank_copy(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t bank_copy_address() const { return address + 8; }
  template <typename T = CarSoundWaveBankData>
  T bank_data() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_bank_data(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_bank_data(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t bank_data_address() const { return address + 12; }
  // CarSoundWaveEntry records, 24 bytes a wave
  uint32_t entries() const { return guest::LoadU32(base, address + 16); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t entries_address() const { return address + 16; }
  // the wave data's start
  uint32_t wave_data() const { return guest::LoadU32(base, address + 20); }
  void set_wave_data(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t wave_data_address() const { return address + 20; }
};

// allocated; measure: WaveBankView.+12
struct CarSoundWaveBankData : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  // 0x20000 compact (one format for every wave, +84)
  uint32_t flags() const { return guest::LoadU32(base, address + 0); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t flags_address() const { return address + 0; }
  // the waves
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
  uint32_t compact_format() const { return guest::LoadU32(base, address + 84); }
  void set_compact_format(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t compact_format_address() const { return address + 84; }
};

// allocated; measure: WaveBankView.+16
struct CarSoundWaveEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // bits 0-1 the codec (0 PCM), 2-4 the channels, 5-22 the rate, 31 16-bit samples
  uint32_t format() const { return guest::LoadU32(base, address + 4); }
  void set_format(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t format_address() const { return address + 4; }
  // from the wave data's start
  uint32_t data_offset() const { return guest::LoadU32(base, address + 8); }
  void set_data_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_offset_address() const { return address + 8; }
  // probably the play region's length
  uint32_t length() const { return guest::LoadU32(base, address + 12); }
  void set_length(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // the loop region's start, in sample frames
  uint32_t loop_start() const { return guest::LoadU32(base, address + 16); }
  void set_loop_start(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t loop_start_address() const { return address + 16; }
  // the loop region's length, in sample frames
  uint32_t loop_length() const { return guest::LoadU32(base, address + 20); }
  void set_loop_length(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t loop_length_address() const { return address + 20; }
};

// embedded; measure: WaveBankView::Format.r5
struct CarSoundWaveFormat : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 1 16-bit, 2 8-bit
  uint8_t sample_type() const { return guest::LoadU8(base, address + 0); }
  void set_sample_type(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t sample_type_address() const { return address + 0; }
  uint8_t channels() const { return guest::LoadU8(base, address + 4); }
  void set_channels(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t channels_address() const { return address + 4; }
  uint32_t rate() const { return guest::LoadU32(base, address + 8); }
  void set_rate(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t rate_address() const { return address + 8; }
};

// embedded; measure: WaveBankView::Length.r5, WaveBankView::Data.r5
struct CarSoundWaveWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// image; measure: X3DAudio
struct X3DAudioHandle : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 6 (5.1)
  uint32_t speaker_count() const { return guest::LoadU32(base, address + 0); }
  void set_speaker_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t speaker_count_address() const { return address + 0; }
  // -1 none
  uint32_t lfe_speaker() const { return guest::LoadU32(base, address + 4); }
  void set_lfe_speaker(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t lfe_speaker_address() const { return address + 4; }
  float speed_of_sound() const { return guest::LoadFloat(base, address + 8); }
  void set_speed_of_sound(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t speed_of_sound_address() const { return address + 8; }
  // what the Doppler velocities are clamped to
  float doppler_limit() const { return guest::LoadFloat(base, address + 12); }
  void set_doppler_limit(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t doppler_limit_address() const { return address + 12; }
};

// embedded; measure: -
struct X3DAudioListener : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  float front_x() const { return guest::LoadFloat(base, address + 0); }
  void set_front_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t front_x_address() const { return address + 0; }
  float front_y() const { return guest::LoadFloat(base, address + 4); }
  void set_front_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t front_y_address() const { return address + 4; }
  float front_z() const { return guest::LoadFloat(base, address + 8); }
  void set_front_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t front_z_address() const { return address + 8; }
  float top_x() const { return guest::LoadFloat(base, address + 12); }
  void set_top_x(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t top_x_address() const { return address + 12; }
  float top_y() const { return guest::LoadFloat(base, address + 16); }
  void set_top_y(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t top_y_address() const { return address + 16; }
  float top_z() const { return guest::LoadFloat(base, address + 20); }
  void set_top_z(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t top_z_address() const { return address + 20; }
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
  template <typename T = X3DAudioCone>
  T cone() const { return T(base, guest::LoadU32(base, address + 48)); }
  void set_cone(const View& v) const { guest::StoreU32(base, address + 48, v.address); }
  void set_cone(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t cone_address() const { return address + 48; }
};

// embedded; measure: X3DAudio::Delay.r5, X3DAudioCalculate.r5
struct X3DAudioEmitter : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  // 0 none
  template <typename T = X3DAudioCone>
  T cone() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_cone(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_cone(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t cone_address() const { return address + 0; }
  float front_x() const { return guest::LoadFloat(base, address + 4); }
  void set_front_x(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t front_x_address() const { return address + 4; }
  float front_y() const { return guest::LoadFloat(base, address + 8); }
  void set_front_y(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t front_y_address() const { return address + 8; }
  float front_z() const { return guest::LoadFloat(base, address + 12); }
  void set_front_z(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t front_z_address() const { return address + 12; }
  float top_x() const { return guest::LoadFloat(base, address + 16); }
  void set_top_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t top_x_address() const { return address + 16; }
  float top_y() const { return guest::LoadFloat(base, address + 20); }
  void set_top_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t top_y_address() const { return address + 20; }
  float top_z() const { return guest::LoadFloat(base, address + 24); }
  void set_top_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t top_z_address() const { return address + 24; }
  float position_x() const { return guest::LoadFloat(base, address + 28); }
  void set_position_x(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t position_x_address() const { return address + 28; }
  float position_y() const { return guest::LoadFloat(base, address + 32); }
  void set_position_y(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t position_y_address() const { return address + 32; }
  float position_z() const { return guest::LoadFloat(base, address + 36); }
  void set_position_z(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t position_z_address() const { return address + 36; }
  float velocity_x() const { return guest::LoadFloat(base, address + 40); }
  void set_velocity_x(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t velocity_x_address() const { return address + 40; }
  float velocity_y() const { return guest::LoadFloat(base, address + 44); }
  void set_velocity_y(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t velocity_y_address() const { return address + 44; }
  float velocity_z() const { return guest::LoadFloat(base, address + 48); }
  void set_velocity_z(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t velocity_z_address() const { return address + 48; }
  uint32_t channel_count() const { return guest::LoadU32(base, address + 52); }
  void set_channel_count(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t channel_count_address() const { return address + 52; }
  float channel_radius() const { return guest::LoadFloat(base, address + 56); }
  void set_channel_radius(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t channel_radius_address() const { return address + 56; }
  // a float per channel (X3DAUDIO_2PI the LFE)
  uint32_t channel_azimuths() const { return guest::LoadU32(base, address + 60); }
  void set_channel_azimuths(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t channel_azimuths_address() const { return address + 60; }
  // 0: the library's default
  template <typename T = X3DAudioCurve>
  T volume_curve() const { return T(base, guest::LoadU32(base, address + 64)); }
  void set_volume_curve(const View& v) const { guest::StoreU32(base, address + 64, v.address); }
  void set_volume_curve(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t volume_curve_address() const { return address + 64; }
  template <typename T = X3DAudioCurve>
  T lfe_curve() const { return T(base, guest::LoadU32(base, address + 68)); }
  void set_lfe_curve(const View& v) const { guest::StoreU32(base, address + 68, v.address); }
  void set_lfe_curve(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t lfe_curve_address() const { return address + 68; }
  template <typename T = X3DAudioCurve>
  T lpf_direct_curve() const { return T(base, guest::LoadU32(base, address + 72)); }
  void set_lpf_direct_curve(const View& v) const { guest::StoreU32(base, address + 72, v.address); }
  void set_lpf_direct_curve(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t lpf_direct_curve_address() const { return address + 72; }
  template <typename T = X3DAudioCurve>
  T lpf_reverb_curve() const { return T(base, guest::LoadU32(base, address + 76)); }
  void set_lpf_reverb_curve(const View& v) const { guest::StoreU32(base, address + 76, v.address); }
  void set_lpf_reverb_curve(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t lpf_reverb_curve_address() const { return address + 76; }
  template <typename T = X3DAudioCurve>
  T reverb_curve() const { return T(base, guest::LoadU32(base, address + 80)); }
  void set_reverb_curve(const View& v) const { guest::StoreU32(base, address + 80, v.address); }
  void set_reverb_curve(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t reverb_curve_address() const { return address + 80; }
  float curve_distance_scaler() const { return guest::LoadFloat(base, address + 84); }
  void set_curve_distance_scaler(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t curve_distance_scaler_address() const { return address + 84; }
  float doppler_scaler() const { return guest::LoadFloat(base, address + 88); }
  void set_doppler_scaler(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t doppler_scaler_address() const { return address + 88; }
};

// embedded; measure: CarSound.+52, X3DAudio::Delay.r5.+0
struct X3DAudioCone : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  float inner_angle() const { return guest::LoadFloat(base, address + 0); }
  void set_inner_angle(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t inner_angle_address() const { return address + 0; }
  float outer_angle() const { return guest::LoadFloat(base, address + 4); }
  void set_outer_angle(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t outer_angle_address() const { return address + 4; }
  float inner_volume() const { return guest::LoadFloat(base, address + 8); }
  void set_inner_volume(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t inner_volume_address() const { return address + 8; }
  float outer_volume() const { return guest::LoadFloat(base, address + 12); }
  void set_outer_volume(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t outer_volume_address() const { return address + 12; }
  float inner_lpf() const { return guest::LoadFloat(base, address + 16); }
  void set_inner_lpf(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t inner_lpf_address() const { return address + 16; }
  float outer_lpf() const { return guest::LoadFloat(base, address + 20); }
  void set_outer_lpf(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t outer_lpf_address() const { return address + 20; }
  float inner_reverb() const { return guest::LoadFloat(base, address + 24); }
  void set_inner_reverb(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t inner_reverb_address() const { return address + 24; }
  float outer_reverb() const { return guest::LoadFloat(base, address + 28); }
  void set_outer_reverb(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t outer_reverb_address() const { return address + 28; }
};

// embedded; measure: -
struct X3DAudioCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // {distance, value} float pairs
  uint32_t points() const { return guest::LoadU32(base, address + 0); }
  void set_points(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t points_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t count_address() const { return address + 4; }
};

// embedded; measure: X3DAudio::Delay.r6, X3DAudioCalculate.r7
struct X3DAudioDspSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // destination rows of source-channel floats
  template <typename T = X3DAudioMatrix>
  T matrix() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_matrix(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_matrix(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t matrix_address() const { return address + 0; }
  template <typename T = X3DAudioDelays>
  T delays() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delays(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delays(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delays_address() const { return address + 4; }
  uint32_t source_channels() const { return guest::LoadU32(base, address + 8); }
  void set_source_channels(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t source_channels_address() const { return address + 8; }
  uint32_t destination_channels() const { return guest::LoadU32(base, address + 12); }
  void set_destination_channels(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t destination_channels_address() const { return address + 12; }
  float lpf_direct() const { return guest::LoadFloat(base, address + 16); }
  void set_lpf_direct(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t lpf_direct_address() const { return address + 16; }
  float lpf_reverb() const { return guest::LoadFloat(base, address + 20); }
  void set_lpf_reverb(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t lpf_reverb_address() const { return address + 20; }
  float reverb_level() const { return guest::LoadFloat(base, address + 24); }
  void set_reverb_level(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t reverb_level_address() const { return address + 24; }
  float doppler_factor() const { return guest::LoadFloat(base, address + 28); }
  void set_doppler_factor(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t doppler_factor_address() const { return address + 28; }
  float emitter_angle() const { return guest::LoadFloat(base, address + 32); }
  void set_emitter_angle(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t emitter_angle_address() const { return address + 32; }
  float distance() const { return guest::LoadFloat(base, address + 36); }
  void set_distance(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t distance_address() const { return address + 36; }
  // the emitter's velocity along the line to the listener
  float emitter_velocity() const { return guest::LoadFloat(base, address + 40); }
  void set_emitter_velocity(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t emitter_velocity_address() const { return address + 40; }
  // the listener's
  float listener_velocity() const { return guest::LoadFloat(base, address + 44); }
  void set_listener_velocity(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t listener_velocity_address() const { return address + 44; }
};

// allocated; measure: X3DAudio::Delay.r6.+0, X3DAudio.+0
struct X3DAudioMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  float coefficient_0() const { return guest::LoadFloat(base, address + 0); }
  void set_coefficient_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t coefficient_0_address() const { return address + 0; }
  float coefficient_1() const { return guest::LoadFloat(base, address + 4); }
  void set_coefficient_1(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t coefficient_1_address() const { return address + 4; }
  float coefficient_2() const { return guest::LoadFloat(base, address + 8); }
  void set_coefficient_2(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t coefficient_2_address() const { return address + 8; }
  float coefficient_3() const { return guest::LoadFloat(base, address + 12); }
  void set_coefficient_3(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t coefficient_3_address() const { return address + 12; }
  float coefficient_4() const { return guest::LoadFloat(base, address + 16); }
  void set_coefficient_4(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t coefficient_4_address() const { return address + 16; }
  float coefficient_5() const { return guest::LoadFloat(base, address + 20); }
  void set_coefficient_5(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t coefficient_5_address() const { return address + 20; }
  float coefficient_6() const { return guest::LoadFloat(base, address + 24); }
  void set_coefficient_6(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t coefficient_6_address() const { return address + 24; }
  float coefficient_7() const { return guest::LoadFloat(base, address + 28); }
  void set_coefficient_7(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t coefficient_7_address() const { return address + 28; }
  float coefficient_8() const { return guest::LoadFloat(base, address + 32); }
  void set_coefficient_8(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t coefficient_8_address() const { return address + 32; }
};

// allocated; measure: X3DAudio::Delay.r6.+4
struct X3DAudioDelays : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float delay_0() const { return guest::LoadFloat(base, address + 0); }
  void set_delay_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t delay_0_address() const { return address + 0; }
  float delay_1() const { return guest::LoadFloat(base, address + 4); }
  void set_delay_1(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t delay_1_address() const { return address + 4; }
};

// embedded; measure: X3DVec, X3DVec::Cross.r4, X3DVec::Cross.r5, X3DVec::Dot.r4, X3DVec::Sub.r4, X3DVec::Sub.r5
struct X3DAudioVector : View {
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

// The image words docs/functions/car-sounds.md's code uses.
struct CarSoundsGlobals {
  // seven pointers to CarInputRecords, by the car's index
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // the seven car sounds (an array, the count 16 bytes before)
  template <typename T = CarSound>
  static T sounds(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C3Cu)); }
  static void set_sounds(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C3Cu, v.address); }
  static void set_sounds(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C3Cu, v); }
  static constexpr uint32_t sounds_address() { return 0x826C5C3Cu; }
  // 0 by CarSounds::Init; not read further
  static uint32_t unknown_826C5C40(uint8_t* base) { return guest::LoadU32(base, 0x826C5C40u); }
  static void set_unknown_826C5C40(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C40u, v); }
  static constexpr uint32_t unknown_826C5C40_address() { return 0x826C5C40u; }
  // the six-channel 48 kHz XAudio submix voice every car's voices send to
  static uint32_t submix(uint8_t* base) { return guest::LoadU32(base, 0x826C5C44u); }
  static void set_submix(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C44u, v); }
  static constexpr uint32_t submix_address() { return 0x826C5C44u; }
  // seven words cleared by SoundGlobals::Init (the scene models' sound picks)
  static constexpr uint32_t sound_picks_a_address() { return 0x826C5C48u; }
  // seven words cleared by SoundGlobals::Init (the scene models' sound picks)
  static constexpr uint32_t sound_picks_b_address() { return 0x826C5C64u; }
  // the sound manager (AudioManager, app + 124)
  static uint32_t sound_manager(uint8_t* base) { return guest::LoadU32(base, 0x826C5C80u); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // 0 by SoundGlobals::Init and the race's music fades
  static uint8_t music_byte(uint8_t* base) { return guest::LoadU8(base, 0x826C5C84u); }
  static void set_music_byte(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x826C5C84u, v); }
  static constexpr uint32_t music_byte_address() { return 0x826C5C84u; }
  // 0 by SoundGlobals::Init
  static uint8_t unknown_826C5C85(uint8_t* base) { return guest::LoadU8(base, 0x826C5C85u); }
  static void set_unknown_826C5C85(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x826C5C85u, v); }
  static constexpr uint32_t unknown_826C5C85_address() { return 0x826C5C85u; }
  // bank pair 2's 3 MiB physical buffer
  static uint32_t bank_pair_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826C5C88u); }
  static void set_bank_pair_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C88u, v); }
  static constexpr uint32_t bank_pair_buffer_address() { return 0x826C5C88u; }
  // bank pair 2's 512-byte physical buffer
  static uint32_t bank_pair_small_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826C5C8Cu); }
  static void set_bank_pair_small_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C8Cu, v); }
  static constexpr uint32_t bank_pair_small_buffer_address() { return 0x826C5C8Cu; }
  // an X3DAudioHandle (X3DAudioInitialize's, 5.1)
  static constexpr uint32_t x3daudio_address() { return 0x826C5C90u; }
  // the emitter's one channel azimuth
  static float channel_azimuths(uint8_t* base) { return guest::LoadFloat(base, 0x826C5C38u); }
  static void set_channel_azimuths(uint8_t* base, float v) { guest::StoreFloat(base, 0x826C5C38u, v); }
  static constexpr uint32_t channel_azimuths_address() { return 0x826C5C38u; }
  // a CarInputRecord: race mode 8's other player's
  static constexpr uint32_t horn_record_address() { return 0x826C62CCu; }
  // the scene models' sound assets by block (16-byte entries): +0 a list of pointers to the cars' sound parameters
  static constexpr uint32_t sound_parameters_address() { return 0x82616318u; }
  // the music's volume (1.0 by SoundGlobals::Init)
  static float music_volume(uint8_t* base) { return guest::LoadFloat(base, 0x82616BECu); }
  static void set_music_volume(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616BECu, v); }
  static constexpr uint32_t music_volume_address() { return 0x82616BECu; }
  // a volume, 1.0 by SoundGlobals::Init
  static float volume_c34(uint8_t* base) { return guest::LoadFloat(base, 0x82616C34u); }
  static void set_volume_c34(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616C34u, v); }
  static constexpr uint32_t volume_c34_address() { return 0x82616C34u; }
  // the effects' volume option's gain (the car sounds' output maps times it)
  static float effects_volume(uint8_t* base) { return guest::LoadFloat(base, 0x82616C38u); }
  static void set_effects_volume(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616C38u, v); }
  static constexpr uint32_t effects_volume_address() { return 0x82616C38u; }
  // the effects' volume option's gain
  static float effects_volume_2(uint8_t* base) { return guest::LoadFloat(base, 0x82616C3Cu); }
  static void set_effects_volume_2(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616C3Cu, v); }
  static constexpr uint32_t effects_volume_2_address() { return 0x82616C3Cu; }
  // the submix's reverb parameters outside a tunnel (probably)
  static constexpr uint32_t reverb_off_address() { return 0x82616638u; }
  // the submix's reverb parameters in a tunnel (probably)
  static constexpr uint32_t reverb_tunnel_address() { return 0x826166F8u; }
};

}  // namespace schema
