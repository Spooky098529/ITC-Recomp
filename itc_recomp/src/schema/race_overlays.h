// The views of docs/functions/race-overlays.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-overlays.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceOverlays
struct RaceOverlays : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // overlay 0: loads the texture set the others share
  template <typename T = CarOverlays>
  T overlay_0() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_overlay_0(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_overlay_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t overlay_0_address() const { return address + 0; }
  // overlays 1 to 24 (pointers; in races 1 to 6 are CarOverlays, the rest 0)
  uint32_t others_address() const { return address + 4; }
};

// image; measure: CarOverlays.+0, RaceOverlays.+8.+0
struct OverlayVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 0 (r4 bit 0: free)
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // slot 3
  uint32_t reset() const { return guest::LoadU32(base, address + 12); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t reset_address() const { return address + 12; }
  // slot 4, once a tick
  uint32_t update() const { return guest::LoadU32(base, address + 16); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t update_address() const { return address + 16; }
  // slot 5 (the car overlays': each child's slot 7)
  uint32_t record() const { return guest::LoadU32(base, address + 20); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t record_address() const { return address + 20; }
  // slot 6: r4 a set-up (CarOverlays: 28 bytes; the children: 12), r5 a capacity
  uint32_t init() const { return guest::LoadU32(base, address + 24); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t init_address() const { return address + 24; }
  // slot 7: a child's recording, r4 the camera pass index (sprites.md)
  uint32_t record_child() const { return guest::LoadU32(base, address + 28); }
  void set_record_child(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t record_child_address() const { return address + 28; }
};

// image; measure: CarOverlays.+4.+0
struct OverlayViewVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
};

// allocated; measure: CarOverlays, RaceOverlays.+8
struct CarOverlays : View {
  using View::View;
  static constexpr uint32_t kSize = 848;
  static constexpr uint32_t kVtable = 0x82008524;
  // 0x82008524 (OverlayVtable)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 25 OverlayView pointers, one a view slot (0 none)
  uint32_t views_address() const { return address + 4; }
  // the texture table (42 textures, loaded by overlay 0 and shared)
  uint32_t textures() const { return guest::LoadU32(base, address + 104); }
  void set_textures(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t textures_address() const { return address + 104; }
  uint32_t texture_set() const { return guest::LoadU32(base, address + 108); }
  void set_texture_set(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t texture_set_address() const { return address + 108; }
  uint32_t loaded() const { return guest::LoadU32(base, address + 112); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t loaded_address() const { return address + 112; }
  // an index into the 12-byte entries at 0x82008558 (the set-up's +0)
  uint32_t viewport_entry() const { return guest::LoadU32(base, address + 116); }
  void set_viewport_entry(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t viewport_entry_address() const { return address + 116; }
  // an index into the pairs at 0x820085A0 (the set-up's +4)
  uint32_t second_index() const { return guest::LoadU32(base, address + 120); }
  void set_second_index(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t second_index_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(base, address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  // the slot of the car's own view
  uint32_t view_slot() const { return guest::LoadU32(base, address + 128); }
  void set_view_slot(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t view_slot_address() const { return address + 128; }
  // [0x826C5B2C] + 4 at the set-up
  uint32_t race_word_4() const { return guest::LoadU32(base, address + 132); }
  void set_race_word_4(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t race_word_4_address() const { return address + 132; }
  // [0x826C5B2C] + 28 at the set-up; the effects update with 0 or 3
  uint32_t race_word_28() const { return guest::LoadU32(base, address + 136); }
  void set_race_word_28(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t race_word_28_address() const { return address + 136; }
  // [0x826C5B2C] + 32 at the set-up
  uint32_t race_word_32() const { return guest::LoadU32(base, address + 140); }
  void set_race_word_32(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t race_word_32_address() const { return address + 140; }
  // the effects' parameter table 0x82616F50
  template <typename T = CarOverlaysParams>
  T params() const { return T(base, guest::LoadU32(base, address + 144)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 144, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t params_address() const { return address + 144; }
  // 4 Trail pointers, one a wheel
  uint32_t trails_address() const { return address + 148; }
  // 4 Smoke pointers, one a wheel
  uint32_t smokes_address() const { return address + 164; }
  // five words cleared by CreateChildren
  uint32_t unknown_180_address() const { return address + 180; }
  // 4 LightSprite pointers (model points 4-7)
  uint32_t light_sprites_address() const { return address + 200; }
  // 14 words cleared by CreateChildren
  uint32_t unknown_216_address() const { return address + 216; }
  // 14 words cleared by CreateChildren
  uint32_t unknown_272_address() const { return address + 272; }
  // 8 Sparks pointers
  uint32_t sparks_address() const { return address + 328; }
  // 4 ParticlesB pointers (model points 4-7)
  uint32_t particles_b_address() const { return address + 360; }
  // the marker above the car
  template <typename T = Marker>
  T marker() const { return T(base, guest::LoadU32(base, address + 376)); }
  void set_marker(const View& v) const { guest::StoreU32(base, address + 376, v.address); }
  void set_marker(uint32_t v) const { guest::StoreU32(base, address + 376, v); }
  uint32_t marker_address() const { return address + 376; }
  // 4 ParticlesC pointers (model points 14-17; probably the exhausts)
  uint32_t particles_c_address() const { return address + 380; }
  // 9 ModelSprite pointers
  uint32_t model_sprites_address() const { return address + 396; }
  // 4 CarOverlaysWheel (12 bytes a wheel)
  uint32_t wheels_address() const { return address + 432; }
  // cleared by the reset (with the rates after it)
  uint32_t unknown_480_address() const { return address + 480; }
  // +48 x wheel: the wheel's smoke rate accumulator (f32), the rest of each 48 bytes cleared by the reset
  uint32_t smoke_rates_address() const { return address + 528; }
  // the vehicle's word +2252
  uint32_t vehicle_word_2252() const { return guest::LoadU32(base, address + 720); }
  void set_vehicle_word_2252(uint32_t v) const { guest::StoreU32(base, address + 720, v); }
  uint32_t vehicle_word_2252_address() const { return address + 720; }
  // 1.0 at construction
  float unknown_748() const { return guest::LoadFloat(base, address + 748); }
  void set_unknown_748(float v) const { guest::StoreFloat(base, address + 748, v); }
  uint32_t unknown_748_address() const { return address + 748; }
  // probably a vector, 1.0 at construction
  uint32_t unknown_752_address() const { return address + 752; }
  // 1.0 at construction
  float unknown_764() const { return guest::LoadFloat(base, address + 764); }
  void set_unknown_764(float v) const { guest::StoreFloat(base, address + 764, v); }
  uint32_t unknown_764_address() const { return address + 764; }
  // 0.0 at construction and reset
  float color_r() const { return guest::LoadFloat(base, address + 768); }
  void set_color_r(float v) const { guest::StoreFloat(base, address + 768, v); }
  uint32_t color_r_address() const { return address + 768; }
  // 0.0 at construction and reset
  float color_g() const { return guest::LoadFloat(base, address + 772); }
  void set_color_g(float v) const { guest::StoreFloat(base, address + 772, v); }
  uint32_t color_g_address() const { return address + 772; }
  // 0.0 at construction and reset
  float color_b() const { return guest::LoadFloat(base, address + 776); }
  void set_color_b(float v) const { guest::StoreFloat(base, address + 776, v); }
  uint32_t color_b_address() const { return address + 776; }
  // 1.0 at construction and reset
  float color_a() const { return guest::LoadFloat(base, address + 780); }
  void set_color_a(float v) const { guest::StoreFloat(base, address + 780, v); }
  uint32_t color_a_address() const { return address + 780; }
  // the viewport's +1860: 1 and 2 turn some effects off
  uint32_t view_mode() const { return guest::LoadU32(base, address + 784); }
  void set_view_mode(uint32_t v) const { guest::StoreU32(base, address + 784, v); }
  uint32_t view_mode_address() const { return address + 784; }
  // 6 at construction and reset
  uint32_t unknown_788() const { return guest::LoadU32(base, address + 788); }
  void set_unknown_788(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t unknown_788_address() const { return address + 788; }
  // the bound viewport's camera entry (CarOverlays::BindViewport's out +0)
  uint32_t viewport_record() const { return guest::LoadU32(base, address + 796); }
  void set_viewport_record(uint32_t v) const { guest::StoreU32(base, address + 796, v); }
  uint32_t viewport_record_address() const { return address + 796; }
  // the bound camera pass (its eye at +400)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 800); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t camera_pass_address() const { return address + 800; }
  // the two words at 0x820085A0 + 8 x the second index
  uint32_t viewport_pair_address() const { return address + 804; }
  // 1 during UpdateSparks (0 again in view modes 1 and 2)
  uint32_t sparks_updating() const { return guest::LoadU32(base, address + 812); }
  void set_sparks_updating(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t sparks_updating_address() const { return address + 812; }
  uint32_t marker_shown() const { return guest::LoadU32(base, address + 816); }
  void set_marker_shown(uint32_t v) const { guest::StoreU32(base, address + 816, v); }
  uint32_t marker_shown_address() const { return address + 816; }
  // 0 at the set-up; read by the marker's and model sprites' kinds
  uint32_t unknown_820() const { return guest::LoadU32(base, address + 820); }
  void set_unknown_820(uint32_t v) const { guest::StoreU32(base, address + 820, v); }
  uint32_t unknown_820_address() const { return address + 820; }
  // 0 at the set-up; read by the marker's kind
  uint32_t unknown_824() const { return guest::LoadU32(base, address + 824); }
  void set_unknown_824(uint32_t v) const { guest::StoreU32(base, address + 824, v); }
  uint32_t unknown_824_address() const { return address + 824; }
  // 0 at the set-up; read by the marker's and model sprites' kinds
  uint32_t unknown_828() const { return guest::LoadU32(base, address + 828); }
  void set_unknown_828(uint32_t v) const { guest::StoreU32(base, address + 828, v); }
  uint32_t unknown_828_address() const { return address + 828; }
  uint32_t last_vehicle() const { return guest::LoadU32(base, address + 832); }
  void set_last_vehicle(uint32_t v) const { guest::StoreU32(base, address + 832, v); }
  uint32_t last_vehicle_address() const { return address + 832; }
  // -1 none
  int32_t racer_id() const { return int32_t(guest::LoadU32(base, address + 836)); }
  void set_racer_id(int32_t v) const { guest::StoreU32(base, address + 836, uint32_t(v)); }
  uint32_t racer_id_address() const { return address + 836; }
};

// embedded; measure: -
struct CarOverlaysWheel : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // counts the slipping ticks against the table's +156 and +160
  uint32_t trail_counter() const { return guest::LoadU32(base, address + 0); }
  void set_trail_counter(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t trail_counter_address() const { return address + 0; }
  // up to 25 on surface 8, then counts down while smoke is emitted
  uint32_t smoke_counter() const { return guest::LoadU32(base, address + 4); }
  void set_smoke_counter(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t smoke_counter_address() const { return address + 4; }
  // whether the last tick added a trail point
  uint32_t trail_added() const { return guest::LoadU32(base, address + 8); }
  void set_trail_added(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t trail_added_address() const { return address + 8; }
};

// image; measure: -
struct CarOverlaysParams : View {
  using View::View;
  static constexpr uint32_t kSize = 164;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // scales the smoke rates when DebugSwitchOff is set
  float debug_rate_scale() const { return guest::LoadFloat(base, address + 4); }
  void set_debug_rate_scale(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t debug_rate_scale_address() const { return address + 4; }
  // smoke on marking surfaces: this plus +12 x slip
  float marks_rate() const { return guest::LoadFloat(base, address + 8); }
  void set_marks_rate(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t marks_rate_address() const { return address + 8; }
  float marks_rate_slip() const { return guest::LoadFloat(base, address + 12); }
  void set_marks_rate_slip(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t marks_rate_slip_address() const { return address + 12; }
  // the size draw on marking surfaces
  float marks_size() const { return guest::LoadFloat(base, address + 16); }
  void set_marks_size(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t marks_size_address() const { return address + 16; }
  // WheelSlip's threshold for the smoke
  float smoke_slip_threshold() const { return guest::LoadFloat(base, address + 20); }
  void set_smoke_slip_threshold(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t smoke_slip_threshold_address() const { return address + 20; }
  // times the rate on surface 8
  float surface8_rate() const { return guest::LoadFloat(base, address + 24); }
  void set_surface8_rate(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t surface8_rate_address() const { return address + 24; }
  // times the rate while the wheel's smoke counter runs
  float counter_rate() const { return guest::LoadFloat(base, address + 28); }
  void set_counter_rate(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t counter_rate_address() const { return address + 28; }
  uint32_t unknown_32_address() const { return address + 32; }
  // the size draw on surface 8 and while the counter runs
  float surface8_size() const { return guest::LoadFloat(base, address + 44); }
  void set_surface8_size(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t surface8_size_address() const { return address + 44; }
  uint32_t unknown_48_address() const { return address + 48; }
  // times the rate on surfaces 1-2
  float surface12_rate() const { return guest::LoadFloat(base, address + 68); }
  void set_surface12_rate(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t surface12_rate_address() const { return address + 68; }
  uint32_t unknown_72_address() const { return address + 72; }
  // the size draw on surfaces 1-2
  float surface12_size() const { return guest::LoadFloat(base, address + 80); }
  void set_surface12_size(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t surface12_size_address() const { return address + 80; }
  uint32_t unknown_84_address() const { return address + 84; }
  // WheelSlip's threshold for the trails
  float trail_slip_threshold() const { return guest::LoadFloat(base, address + 152); }
  void set_trail_slip_threshold(float v) const { guest::StoreFloat(base, address + 152, v); }
  uint32_t trail_slip_threshold_address() const { return address + 152; }
  // a trail point is due from this many slipping ticks
  uint32_t trail_least_count() const { return guest::LoadU32(base, address + 156); }
  void set_trail_least_count(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t trail_least_count_address() const { return address + 156; }
  // up to this many (the counter capped at twice it)
  uint32_t trail_most_count() const { return guest::LoadU32(base, address + 160); }
  void set_trail_most_count(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t trail_most_count_address() const { return address + 160; }
};

// image; measure: -
struct OverlayParams : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // in ticks
  uint32_t life() const { return guest::LoadU32(base, address + 0); }
  void set_life(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t life_address() const { return address + 0; }
  float size() const { return guest::LoadFloat(base, address + 4); }
  void set_size(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  // the alpha or brightness (the trails: the base width)
  float alpha() const { return guest::LoadFloat(base, address + 8); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t alpha_address() const { return address + 8; }
  // fades over this many ticks of life left
  uint32_t fade_ticks() const { return guest::LoadU32(base, address + 12); }
  void set_fade_ticks(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fade_ticks_address() const { return address + 12; }
};

// allocated; measure: OverlayView, CarOverlays.+4
struct OverlayView : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82010B5C;
  // 0x82010B5C (OverlayViewVtable)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vehicle() const { return guest::LoadU32(base, address + 4); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vehicle_address() const { return address + 4; }
  // its scene model (0 none)
  uint32_t model() const { return guest::LoadU32(base, address + 8); }
  void set_model(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t model_address() const { return address + 8; }
  uint32_t viewport() const { return guest::LoadU32(base, address + 12); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t viewport_address() const { return address + 12; }
};

// embedded; measure: ViewDesc, OverlayView::Set.r4
struct ViewDesc : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t vehicle() const { return guest::LoadU32(base, address + 0); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vehicle_address() const { return address + 0; }
  uint32_t model() const { return guest::LoadU32(base, address + 4); }
  void set_model(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t model_address() const { return address + 4; }
  uint32_t viewport() const { return guest::LoadU32(base, address + 8); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t viewport_address() const { return address + 8; }
};

// embedded; measure: -
struct OverlayViewportBinding : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // [[0x826C2CE8] + 4 x entry.0] + 80 x entry.1
  uint32_t record() const { return guest::LoadU32(base, address + 0); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // 0x826BAC00 + 592 x entry.2
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 4); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t camera_pass_address() const { return address + 4; }
  uint32_t pair_0() const { return guest::LoadU32(base, address + 8); }
  void set_pair_0(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pair_0_address() const { return address + 8; }
  uint32_t pair_1() const { return guest::LoadU32(base, address + 12); }
  void set_pair_1(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pair_1_address() const { return address + 12; }
};

// image; measure: -
struct OverlayViewportEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // an index into [0x826C2CE8]
  uint32_t viewport() const { return guest::LoadU32(base, address + 0); }
  void set_viewport(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t viewport_address() const { return address + 0; }
  // an 80-byte record of the viewport
  uint32_t record() const { return guest::LoadU32(base, address + 4); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t record_address() const { return address + 4; }
  // an index of the 592-byte camera passes at 0x826BAC00
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 8); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t camera_pass_address() const { return address + 8; }
};

// embedded; measure: Overlay
struct Overlay : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  static constexpr uint32_t kVtable = 0x820085D8;
  // 0x820085D8 for the base class
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // SetTextures104 (the car overlays' texture table; particle effect C's texture)
  uint32_t textures() const { return guest::LoadU32(base, address + 104); }
  void set_textures(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t textures_address() const { return address + 104; }
  // SetTextures108 (the texture holder of the trails, smoke, sparks and particle effect B)
  uint32_t textures_108() const { return guest::LoadU32(base, address + 108); }
  void set_textures_108(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t textures_108_address() const { return address + 108; }
};

// allocated; measure: -
struct Trail : View {
  using View::View;
  static constexpr uint32_t kSize = 64160;
  static constexpr uint32_t kVtable = 0x820086C0;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(base, address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(base, address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // 500 TrailPoint
  uint32_t points_address() const { return address + 128; }
  // the oldest point's index
  uint32_t oldest() const { return guest::LoadU32(base, address + 64128); }
  void set_oldest(uint32_t v) const { guest::StoreU32(base, address + 64128, v); }
  uint32_t oldest_address() const { return address + 64128; }
  // where the next point goes
  uint32_t tail() const { return guest::LoadU32(base, address + 64132); }
  void set_tail(uint32_t v) const { guest::StoreU32(base, address + 64132, v); }
  uint32_t tail_address() const { return address + 64132; }
  uint32_t count() const { return guest::LoadU32(base, address + 64136); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 64136, v); }
  uint32_t count_address() const { return address + 64136; }
  // a vector
  uint32_t last_position_address() const { return address + 64144; }
};

// embedded; measure: -
struct TrailPoint : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // a vector
  uint32_t position_address() const { return address + 0; }
  // a vector: the trail's last position when added
  uint32_t last_position_address() const { return address + 16; }
  uint32_t red() const { return guest::LoadU32(base, address + 32); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t red_address() const { return address + 32; }
  uint32_t green() const { return guest::LoadU32(base, address + 36); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t green_address() const { return address + 36; }
  uint32_t blue() const { return guest::LoadU32(base, address + 40); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t blue_address() const { return address + 40; }
  // the width (+56) as an integer
  uint32_t width_int() const { return guest::LoadU32(base, address + 44); }
  void set_width_int(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t width_int_address() const { return address + 44; }
  // the value AddPoint is given; 1 once more than a tick old
  uint32_t shown() const { return guest::LoadU32(base, address + 48); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t shown_address() const { return address + 48; }
  // the width AddPoint is given (the edges' distance)
  float side_width() const { return guest::LoadFloat(base, address + 52); }
  void set_side_width(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t side_width_address() const { return address + 52; }
  // fades from +124
  float width() const { return guest::LoadFloat(base, address + 56); }
  void set_width(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t width_address() const { return address + 56; }
  // ticks left
  uint32_t age() const { return guest::LoadU32(base, address + 60); }
  void set_age(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t age_address() const { return address + 60; }
  // a vector: position less last position
  uint32_t difference_address() const { return address + 64; }
  // a vector: the ribbon's edge (position less the side)
  uint32_t edge_minus_address() const { return address + 80; }
  // a vector: the ribbon's other edge
  uint32_t edge_plus_address() const { return address + 96; }
  // 1 or 2 (2: joined to the one before)
  uint32_t state() const { return guest::LoadU32(base, address + 112); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t state_address() const { return address + 112; }
  // set to 1 where the trail breaks
  uint32_t broken() const { return guest::LoadU32(base, address + 116); }
  void set_broken(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t broken_address() const { return address + 116; }
  uint32_t texture_coordinate() const { return guest::LoadU32(base, address + 120); }
  void set_texture_coordinate(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t texture_coordinate_address() const { return address + 120; }
  float full_width() const { return guest::LoadFloat(base, address + 124); }
  void set_full_width(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t full_width_address() const { return address + 124; }
};

// allocated; measure: -
struct Smoke : View {
  using View::View;
  static constexpr uint32_t kSize = 32640;
  static constexpr uint32_t kVtable = 0x820086F4;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(base, address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(base, address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // 0 at construction; the set-up's +0 at Init
  uint32_t unknown_116() const { return guest::LoadU32(base, address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // 45 SmokePuff
  uint32_t puffs_address() const { return address + 128; }
  uint32_t oldest() const { return guest::LoadU32(base, address + 32528); }
  void set_oldest(uint32_t v) const { guest::StoreU32(base, address + 32528, v); }
  uint32_t oldest_address() const { return address + 32528; }
  // where the next puff goes
  uint32_t head() const { return guest::LoadU32(base, address + 32532); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 32532, v); }
  uint32_t head_address() const { return address + 32532; }
  uint32_t count() const { return guest::LoadU32(base, address + 32536); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 32536, v); }
  uint32_t count_address() const { return address + 32536; }
  // a vector (Smoke::SetVelocity)
  uint32_t velocity_address() const { return address + 32544; }
  // a vector: with +32,576, the span puffs drift along when the region is on
  uint32_t region_start_address() const { return address + 32560; }
  // a vector
  uint32_t region_end_address() const { return address + 32576; }
  // a vector: SetRegion's second point (the rear axle's), InRegion's a
  uint32_t region_rear_address() const { return address + 32592; }
  // a vector: SetRegion's first point (the front axle's), InRegion's b
  uint32_t region_front_address() const { return address + 32608; }
  // the view mode SetRegion is given; 1 turns the region on
  uint32_t region_on() const { return guest::LoadU32(base, address + 32624); }
  void set_region_on(uint32_t v) const { guest::StoreU32(base, address + 32624, v); }
  uint32_t region_on_address() const { return address + 32624; }
};

// embedded; measure: -
struct SmokePuff : View {
  using View::View;
  static constexpr uint32_t kSize = 720;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t unknown_32_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: the smoke's velocity (+32,544) at the emit
  uint32_t velocity_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(base, address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(base, address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(base, address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(base, address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(base, address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(base, address + 100); }
  void set_size(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(base, address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(base, address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  // the size Smoke::Emit is given
  float emit_size() const { return guest::LoadFloat(base, address + 112); }
  void set_emit_size(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t emit_size_address() const { return address + 112; }
  float growth() const { return guest::LoadFloat(base, address + 116); }
  void set_growth(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t growth_address() const { return address + 116; }
  float most_alpha() const { return guest::LoadFloat(base, address + 120); }
  void set_most_alpha(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t most_alpha_address() const { return address + 120; }
  float start_size() const { return guest::LoadFloat(base, address + 124); }
  void set_start_size(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t start_size_address() const { return address + 124; }
  // 2 to 10
  uint32_t particle_count() const { return guest::LoadU32(base, address + 128); }
  void set_particle_count(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t particle_count_address() const { return address + 128; }
  // a 4x4 matrix (Smoke::SetFrame)
  uint32_t frame_address() const { return address + 144; }
  // a vector: the frame's +64
  uint32_t frame_position_address() const { return address + 208; }
  // the frame's +80 (a float's bits)
  uint32_t frame_value() const { return guest::LoadU32(base, address + 224); }
  void set_frame_value(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t frame_value_address() const { return address + 224; }
  float unknown_228() const { return guest::LoadFloat(base, address + 228); }
  void set_unknown_228(float v) const { guest::StoreFloat(base, address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  // 10 OverlayParticle
  uint32_t particles_address() const { return address + 240; }
};

// embedded; measure: Particle
struct OverlayParticle : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // a vector (1.0 in its w at construction)
  uint32_t position_address() const { return address + 0; }
  // a vector (1.0 in its w at construction)
  uint32_t velocity_address() const { return address + 16; }
  // rand() % 3 (probably a texture coordinate choice)
  uint32_t pick_a() const { return guest::LoadU32(base, address + 32); }
  void set_pick_a(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t pick_a_address() const { return address + 32; }
  // rand() % 4 (probably a texture coordinate choice)
  uint32_t pick_b() const { return guest::LoadU32(base, address + 36); }
  void set_pick_b(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t pick_b_address() const { return address + 36; }
  uint32_t shown() const { return guest::LoadU32(base, address + 40); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shown_address() const { return address + 40; }
};

// allocated; measure: -
struct Sparks : View {
  using View::View;
  static constexpr uint32_t kSize = 29824;
  static constexpr uint32_t kVtable = 0x820087BC;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(base, address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(base, address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // 200 Spark
  uint32_t sparks_address() const { return address + 128; }
  uint32_t streak_counter() const { return guest::LoadU32(base, address + 28928); }
  void set_streak_counter(uint32_t v) const { guest::StoreU32(base, address + 28928, v); }
  uint32_t streak_counter_address() const { return address + 28928; }
  uint32_t oldest() const { return guest::LoadU32(base, address + 28932); }
  void set_oldest(uint32_t v) const { guest::StoreU32(base, address + 28932, v); }
  uint32_t oldest_address() const { return address + 28932; }
  // where the next spark goes
  uint32_t head() const { return guest::LoadU32(base, address + 28936); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 28936, v); }
  uint32_t head_address() const { return address + 28936; }
  uint32_t count() const { return guest::LoadU32(base, address + 28940); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 28940, v); }
  uint32_t count_address() const { return address + 28940; }
  float spread() const { return guest::LoadFloat(base, address + 28944); }
  void set_spread(float v) const { guest::StoreFloat(base, address + 28944, v); }
  uint32_t spread_address() const { return address + 28944; }
  // a vector
  uint32_t position_address() const { return address + 28960; }
  uint32_t streaking() const { return guest::LoadU32(base, address + 28976); }
  void set_streaking(uint32_t v) const { guest::StoreU32(base, address + 28976, v); }
  uint32_t streaking_address() const { return address + 28976; }
  // a vector
  uint32_t previous_position_address() const { return address + 28992; }
  // a vector
  uint32_t latest_position_address() const { return address + 29008; }
  // 50 vectors
  uint32_t random_vectors_address() const { return address + 29024; }
};

// embedded; measure: -
struct Spark : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: where it started (the emitter's position at the emit)
  uint32_t origin_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(base, address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(base, address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(base, address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(base, address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(base, address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(base, address + 100); }
  void set_size(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(base, address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(base, address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  // a vector: position less origin
  uint32_t offset_address() const { return address + 112; }
  // 0 to 2
  uint32_t state() const { return guest::LoadU32(base, address + 128); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t state_address() const { return address + 128; }
  uint32_t unknown_132() const { return guest::LoadU32(base, address + 132); }
  void set_unknown_132(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  // a streak's length limit
  float limit() const { return guest::LoadFloat(base, address + 136); }
  void set_limit(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t limit_address() const { return address + 136; }
};

// allocated; measure: -
struct ParticlesB : View {
  using View::View;
  static constexpr uint32_t kSize = 2816;
  static constexpr uint32_t kVtable = 0x82008720;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(base, address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(base, address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // the set-up's +0 at Init
  uint32_t unknown_116() const { return guest::LoadU32(base, address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // 15 ParticlesBEntry
  uint32_t entries_address() const { return address + 128; }
  uint32_t oldest() const { return guest::LoadU32(base, address + 2768); }
  void set_oldest(uint32_t v) const { guest::StoreU32(base, address + 2768, v); }
  uint32_t oldest_address() const { return address + 2768; }
  // where the next entry goes
  uint32_t head() const { return guest::LoadU32(base, address + 2772); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 2772, v); }
  uint32_t head_address() const { return address + 2772; }
  uint32_t count() const { return guest::LoadU32(base, address + 2776); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 2776, v); }
  uint32_t count_address() const { return address + 2776; }
  // a vector
  uint32_t position_address() const { return address + 2784; }
  // a vector: the last emit's position less the position before
  uint32_t moved_address() const { return address + 2800; }
};

// embedded; measure: -
struct ParticlesBEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: where it started (the emitter's position at the emit)
  uint32_t origin_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(base, address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(base, address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(base, address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(base, address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(base, address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(base, address + 100); }
  void set_size(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(base, address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(base, address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  float most_alpha() const { return guest::LoadFloat(base, address + 112); }
  void set_most_alpha(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t most_alpha_address() const { return address + 112; }
  uint32_t particle_count() const { return guest::LoadU32(base, address + 116); }
  void set_particle_count(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t particle_count_address() const { return address + 116; }
  // an OverlayParticle
  uint32_t particle_address() const { return address + 128; }
};

// allocated; measure: -
struct ParticlesC : View {
  using View::View;
  static constexpr uint32_t kSize = 2720;
  static constexpr uint32_t kVtable = 0x82008750;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // SetTextures104
  uint32_t texture() const { return guest::LoadU32(base, address + 104); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t texture_address() const { return address + 104; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 108)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 108, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t params_address() const { return address + 108; }
  // 20 ParticlesCEntry
  uint32_t entries_address() const { return address + 112; }
  // a vector
  uint32_t position_address() const { return address + 2672; }
  uint32_t oldest() const { return guest::LoadU32(base, address + 2688); }
  void set_oldest(uint32_t v) const { guest::StoreU32(base, address + 2688, v); }
  uint32_t oldest_address() const { return address + 2688; }
  // where the next entry goes
  uint32_t newest() const { return guest::LoadU32(base, address + 2692); }
  void set_newest(uint32_t v) const { guest::StoreU32(base, address + 2692, v); }
  uint32_t newest_address() const { return address + 2692; }
  uint32_t count() const { return guest::LoadU32(base, address + 2696); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 2696, v); }
  uint32_t count_address() const { return address + 2696; }
  uint32_t capacity() const { return guest::LoadU32(base, address + 2700); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 2700, v); }
  uint32_t capacity_address() const { return address + 2700; }
  float speed() const { return guest::LoadFloat(base, address + 2704); }
  void set_speed(float v) const { guest::StoreFloat(base, address + 2704, v); }
  uint32_t speed_address() const { return address + 2704; }
};

// embedded; measure: -
struct ParticlesCEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: where it started (the emitter's position at the emit)
  uint32_t origin_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(base, address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(base, address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(base, address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(base, address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(base, address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(base, address + 100); }
  void set_size(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(base, address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(base, address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(base, address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
};

// embedded; measure: -
struct OverlaySpriteEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(base, address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(base, address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(base, address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(base, address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(base, address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(base, address + 100); }
  void set_size(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(base, address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(base, address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  // the texture: the marker's kind 0-3; the car-model sprite's state (1-8, +336); 0 for the light sprite
  uint32_t kind() const { return guest::LoadU32(base, address + 112); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t kind_address() const { return address + 112; }
  // 4 at a reset; the light sprite's rand() % 4 when shown
  uint32_t unknown_116() const { return guest::LoadU32(base, address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // the car-model sprite's (9 at a reset)
  uint32_t unknown_120() const { return guest::LoadU32(base, address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  // the car-model sprite's (12 at a reset)
  uint32_t unknown_124() const { return guest::LoadU32(base, address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  // the car-model sprite's (0 at a reset)
  uint32_t unknown_128() const { return guest::LoadU32(base, address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
};

// allocated; measure: -
struct LightSprite : View {
  using View::View;
  static constexpr uint32_t kSize = 288;
  static constexpr uint32_t kVtable = 0x82008444;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // nine texture table entries' addresses
  uint32_t textures_address() const { return address + 104; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 140)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 140, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t params_address() const { return address + 140; }
  // an OverlaySpriteEntry
  uint32_t sprite_address() const { return address + 144; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_272_address() const { return address + 272; }
};

// allocated; measure: -
struct Marker : View {
  using View::View;
  static constexpr uint32_t kSize = 304;
  static constexpr uint32_t kVtable = 0x82008784;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // four texture table entries' addresses
  uint32_t textures_address() const { return address + 104; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 120)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 120, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t params_address() const { return address + 120; }
  // an OverlaySpriteEntry
  uint32_t sprite_address() const { return address + 128; }
  uint32_t view_mode() const { return guest::LoadU32(base, address + 256); }
  void set_view_mode(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t view_mode_address() const { return address + 256; }
  // a vector
  uint32_t camera_position_address() const { return address + 272; }
  // 1 for the camera-facing quad
  uint32_t facing() const { return guest::LoadU32(base, address + 288); }
  void set_facing(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t facing_address() const { return address + 288; }
};

// allocated; measure: -
struct ModelSprite : View {
  using View::View;
  static constexpr uint32_t kSize = 576;
  static constexpr uint32_t kVtable = 0x820084E8;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // Init's r5 as a float
  float unknown_104() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // 25 texture table entries' addresses
  uint32_t textures_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(base, guest::LoadU32(base, address + 208)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 208, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t params_address() const { return address + 208; }
  // an OverlaySpriteEntry
  uint32_t sprite_address() const { return address + 224; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_368_address() const { return address + 368; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_384_address() const { return address + 384; }
  // -1 at a reset
  int32_t unknown_404() const { return int32_t(guest::LoadU32(base, address + 404)); }
  void set_unknown_404(int32_t v) const { guest::StoreU32(base, address + 404, uint32_t(v)); }
  uint32_t unknown_404_address() const { return address + 404; }
  // -1 at a reset
  int32_t unknown_408() const { return int32_t(guest::LoadU32(base, address + 408)); }
  void set_unknown_408(int32_t v) const { guest::StoreU32(base, address + 408, uint32_t(v)); }
  uint32_t unknown_408_address() const { return address + 408; }
  // ten vectors, (0, 0, 0, 1) at a reset
  uint32_t unknown_416_address() const { return address + 416; }
};

// embedded; measure: Sprite, Line::RecordFlat.r4
struct OverlaySprite : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // a 4x4 matrix (identity at ResetStyle2)
  uint32_t matrix_address() const { return address + 0; }
  // drawn only when 0
  uint32_t skip() const { return guest::LoadU32(base, address + 64); }
  void set_skip(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t skip_address() const { return address + 64; }
  // a render state mode, 0-5
  uint32_t blend_mode() const { return guest::LoadU32(base, address + 68); }
  void set_blend_mode(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t blend_mode_address() const { return address + 68; }
  // a render state mode, 0-3 (2 at ResetStyle2)
  uint32_t address_mode() const { return guest::LoadU32(base, address + 72); }
  void set_address_mode(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t address_mode_address() const { return address + 72; }
  // four corners (x, y, z, w); 1.0 in the first two's w at ResetStyle2
  uint32_t corners_address() const { return address + 80; }
  // from LineSprite (power-graph)
  float end0_x() const { return guest::LoadFloat(base, address + 80); }
  void set_end0_x(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t end0_x_address() const { return address + 80; }
  // from LineSprite (power-graph)
  float end0_y() const { return guest::LoadFloat(base, address + 84); }
  void set_end0_y(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t end0_y_address() const { return address + 84; }
  // from LineSprite (power-graph)
  float end0_z() const { return guest::LoadFloat(base, address + 88); }
  void set_end0_z(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t end0_z_address() const { return address + 88; }
  // from LineSprite (power-graph)
  float end1_x() const { return guest::LoadFloat(base, address + 96); }
  void set_end1_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t end1_x_address() const { return address + 96; }
  // from LineSprite (power-graph)
  float end1_y() const { return guest::LoadFloat(base, address + 100); }
  void set_end1_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t end1_y_address() const { return address + 100; }
  // from LineSprite (power-graph)
  float end1_z() const { return guest::LoadFloat(base, address + 104); }
  void set_end1_z(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t end1_z_address() const { return address + 104; }
  // 0 to 255 (from LineSprite, power-graph)
  uint32_t end0_red() const { return guest::LoadU32(base, address + 112); }
  void set_end0_red(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t end0_red_address() const { return address + 112; }
  // end0_red's low byte, as Line::RecordFlat reads it (from LineSprite, power-graph)
  uint8_t end0_red_byte() const { return guest::LoadU8(base, address + 115); }
  void set_end0_red_byte(uint8_t v) const { guest::StoreU8(base, address + 115, v); }
  uint32_t end0_red_byte_address() const { return address + 115; }
  // from LineSprite (power-graph)
  uint32_t end0_green() const { return guest::LoadU32(base, address + 116); }
  void set_end0_green(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t end0_green_address() const { return address + 116; }
  // from LineSprite (power-graph)
  uint8_t end0_green_byte() const { return guest::LoadU8(base, address + 119); }
  void set_end0_green_byte(uint8_t v) const { guest::StoreU8(base, address + 119, v); }
  uint32_t end0_green_byte_address() const { return address + 119; }
  // from LineSprite (power-graph)
  uint32_t end0_blue() const { return guest::LoadU32(base, address + 120); }
  void set_end0_blue(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t end0_blue_address() const { return address + 120; }
  // from LineSprite (power-graph)
  uint8_t end0_blue_byte() const { return guest::LoadU8(base, address + 123); }
  void set_end0_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 123, v); }
  uint32_t end0_blue_byte_address() const { return address + 123; }
  // shifted into the vertex color's top byte (from LineSprite, power-graph)
  uint32_t end0_alpha() const { return guest::LoadU32(base, address + 124); }
  void set_end0_alpha(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t end0_alpha_address() const { return address + 124; }
  // from LineSprite (power-graph)
  uint32_t end1_red() const { return guest::LoadU32(base, address + 128); }
  void set_end1_red(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t end1_red_address() const { return address + 128; }
  // from LineSprite (power-graph)
  uint8_t end1_red_byte() const { return guest::LoadU8(base, address + 131); }
  void set_end1_red_byte(uint8_t v) const { guest::StoreU8(base, address + 131, v); }
  uint32_t end1_red_byte_address() const { return address + 131; }
  // from LineSprite (power-graph)
  uint32_t end1_green() const { return guest::LoadU32(base, address + 132); }
  void set_end1_green(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t end1_green_address() const { return address + 132; }
  // from LineSprite (power-graph)
  uint8_t end1_green_byte() const { return guest::LoadU8(base, address + 135); }
  void set_end1_green_byte(uint8_t v) const { guest::StoreU8(base, address + 135, v); }
  uint32_t end1_green_byte_address() const { return address + 135; }
  // from LineSprite (power-graph)
  uint32_t end1_blue() const { return guest::LoadU32(base, address + 136); }
  void set_end1_blue(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t end1_blue_address() const { return address + 136; }
  // from LineSprite (power-graph)
  uint8_t end1_blue_byte() const { return guest::LoadU8(base, address + 139); }
  void set_end1_blue_byte(uint8_t v) const { guest::StoreU8(base, address + 139, v); }
  uint32_t end1_blue_byte_address() const { return address + 139; }
  // from LineSprite (power-graph)
  uint32_t end1_alpha() const { return guest::LoadU32(base, address + 140); }
  void set_end1_alpha(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t end1_alpha_address() const { return address + 140; }
};

// The image words docs/functions/race-overlays.md's code uses.
struct RaceOverlaysGlobals {
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // the race data (racer records from +48, the race mode at +0xA3AF4)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // OverlayViewportEntry, 12 bytes each
  static uint32_t viewport_entries(uint8_t* base) { return guest::LoadU32(base, 0x82008558u); }
  static void set_viewport_entries(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82008558u, v); }
  static constexpr uint32_t viewport_entries_address() { return 0x82008558u; }
  // pairs of words, 8 bytes each
  static uint32_t viewport_pairs(uint8_t* base) { return guest::LoadU32(base, 0x820085A0u); }
  static void set_viewport_pairs(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820085A0u, v); }
  static constexpr uint32_t viewport_pairs_address() { return 0x820085A0u; }
  // the viewports (camera-modes.toml)
  static uint32_t viewport_owners(uint8_t* base) { return guest::LoadU32(base, 0x826C2CE8u); }
  static void set_viewport_owners(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewport_owners_address() { return 0x826C2CE8u; }
  // the camera passes (592 bytes each, cameras.toml)
  static uint32_t camera_passes(uint8_t* base) { return guest::LoadU32(base, 0x826BAC00u); }
  static void set_camera_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAC00u, v); }
  static constexpr uint32_t camera_passes_address() { return 0x826BAC00u; }
  // the sprite recorders' render queue (Overlays::SetRenderTargets)
  static uint32_t render_queue(uint8_t* base) { return guest::LoadU32(base, 0x826C5D5Cu); }
  static void set_render_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5D5Cu, v); }
  static constexpr uint32_t render_queue_address() { return 0x826C5D5Cu; }
  // the sprite recorders' command list
  static uint32_t command_list(uint8_t* base) { return guest::LoadU32(base, 0x826C5D60u); }
  static void set_command_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5D60u, v); }
  static constexpr uint32_t command_list_address() { return 0x826C5D60u; }
  // the game's own rand's seed
  static uint32_t rand_seed(uint8_t* base) { return guest::LoadU32(base, 0x82638188u); }
  static void set_rand_seed(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82638188u, v); }
  static constexpr uint32_t rand_seed_address() { return 0x82638188u; }
  // probably 1/32,767
  static float rand_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD4u); }
  static void set_rand_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD4u, v); }
  static constexpr uint32_t rand_scale_address() { return 0x82002BD4u; }
  static float axle_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_axle_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t axle_scale_address() { return 0x82001FBCu; }
  static float marker_slope(uint8_t* base) { return guest::LoadFloat(base, 0x82002FB0u); }
  static void set_marker_slope(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002FB0u, v); }
  static constexpr uint32_t marker_slope_address() { return 0x82002FB0u; }
  static float marker_most_size(uint8_t* base) { return guest::LoadFloat(base, 0x82002C3Cu); }
  static void set_marker_most_size(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C3Cu, v); }
  static constexpr uint32_t marker_most_size_address() { return 0x82002C3Cu; }
  static float marker_least_size(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_marker_least_size(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t marker_least_size_address() { return 0x82002568u; }
  static float trail_most_width(uint8_t* base) { return guest::LoadFloat(base, 0x82000C44u); }
  static void set_trail_most_width(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000C44u, v); }
  static constexpr uint32_t trail_most_width_address() { return 0x82000C44u; }
  // the trails' side scale, particle effect B's spread, the sparks' size scale
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  static float particles_b_size_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82008540u); }
  static void set_particles_b_size_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008540u, v); }
  static constexpr uint32_t particles_b_size_scale_address() { return 0x82008540u; }
  // a random angle's low end
  static float angle_low(uint8_t* base) { return guest::LoadFloat(base, 0x820029C4u); }
  static void set_angle_low(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029C4u, v); }
  static constexpr uint32_t angle_low_address() { return 0x820029C4u; }
  // a random angle's high end
  static float angle_high(uint8_t* base) { return guest::LoadFloat(base, 0x82002240u); }
  static void set_angle_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002240u, v); }
  static constexpr uint32_t angle_high_address() { return 0x82002240u; }
  static float spread_low(uint8_t* base) { return guest::LoadFloat(base, 0x82006908u); }
  static void set_spread_low(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006908u, v); }
  static constexpr uint32_t spread_low_address() { return 0x82006908u; }
  static float spread_high(uint8_t* base) { return guest::LoadFloat(base, 0x82002208u); }
  static void set_spread_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002208u, v); }
  static constexpr uint32_t spread_high_address() { return 0x82002208u; }
  // the sparks' spread and the smoke's growth from the speed
  static float speed_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029E4u); }
  static void set_speed_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029E4u, v); }
  static constexpr uint32_t speed_scale_address() { return 0x820029E4u; }
  static float sparks_most_spread(uint8_t* base) { return guest::LoadFloat(base, 0x82002C50u); }
  static void set_sparks_most_spread(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C50u, v); }
  static constexpr uint32_t sparks_most_spread_address() { return 0x82002C50u; }
  static float sparks_mode_factor(uint8_t* base) { return guest::LoadFloat(base, 0x820035B0u); }
  static void set_sparks_mode_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x820035B0u, v); }
  static constexpr uint32_t sparks_mode_factor_address() { return 0x820035B0u; }
  static float sparks_limit_high(uint8_t* base) { return guest::LoadFloat(base, 0x820021E4u); }
  static void set_sparks_limit_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E4u, v); }
  static constexpr uint32_t sparks_limit_high_address() { return 0x820021E4u; }
  // the sparks' limit low end, the smoke's frame scale, the trails' width scale
  static float frame_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_frame_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t frame_scale_address() { return 0x820021E8u; }
  static float sparks_step(uint8_t* base) { return guest::LoadFloat(base, 0x82002674u); }
  static void set_sparks_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002674u, v); }
  static constexpr uint32_t sparks_step_address() { return 0x82002674u; }
  static float sparks_speed_out(uint8_t* base) { return guest::LoadFloat(base, 0x82002204u); }
  static void set_sparks_speed_out(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002204u, v); }
  static constexpr uint32_t sparks_speed_out_address() { return 0x82002204u; }
  static float sparks_speed_in(uint8_t* base) { return guest::LoadFloat(base, 0x820087B8u); }
  static void set_sparks_speed_in(uint8_t* base, float v) { guest::StoreFloat(base, 0x820087B8u, v); }
  static constexpr uint32_t sparks_speed_in_address() { return 0x820087B8u; }
  static float sparks_rise_state0(uint8_t* base) { return guest::LoadFloat(base, 0x820087B0u); }
  static void set_sparks_rise_state0(uint8_t* base, float v) { guest::StoreFloat(base, 0x820087B0u, v); }
  static constexpr uint32_t sparks_rise_state0_address() { return 0x820087B0u; }
  static float sparks_rise_state2(uint8_t* base) { return guest::LoadFloat(base, 0x820087B4u); }
  static void set_sparks_rise_state2(uint8_t* base, float v) { guest::StoreFloat(base, 0x820087B4u, v); }
  static constexpr uint32_t sparks_rise_state2_address() { return 0x820087B4u; }
  static float smoke_growth_high(uint8_t* base) { return guest::LoadFloat(base, 0x82002C4Cu); }
  static void set_smoke_growth_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C4Cu, v); }
  static constexpr uint32_t smoke_growth_high_address() { return 0x82002C4Cu; }
  static float smoke_rise_high(uint8_t* base) { return guest::LoadFloat(base, 0x820029D4u); }
  static void set_smoke_rise_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D4u, v); }
  static constexpr uint32_t smoke_rise_high_address() { return 0x820029D4u; }
  static float smoke_drift_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820086F0u); }
  static void set_smoke_drift_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820086F0u, v); }
  static constexpr uint32_t smoke_drift_scale_address() { return 0x820086F0u; }
  static float smoke_drift_high(uint8_t* base) { return guest::LoadFloat(base, 0x82002C48u); }
  static void set_smoke_drift_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C48u, v); }
  static constexpr uint32_t smoke_drift_high_address() { return 0x82002C48u; }
  static float smoke_radius_base(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_smoke_radius_base(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t smoke_radius_base_address() { return 0x8200222Cu; }
  static float smoke_count_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029ECu); }
  static void set_smoke_count_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029ECu, v); }
  static constexpr uint32_t smoke_count_scale_address() { return 0x820029ECu; }
  static float smoke_radius_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820086ECu); }
  static void set_smoke_radius_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820086ECu, v); }
  static constexpr uint32_t smoke_radius_scale_address() { return 0x820086ECu; }
  // a puff's size at a reset
  static float smoke_puff_size(uint8_t* base) { return guest::LoadFloat(base, 0x82002A90u); }
  static void set_smoke_puff_size(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A90u, v); }
  static constexpr uint32_t smoke_puff_size_address() { return 0x82002A90u; }
  static float particles_b_most_speed(uint8_t* base) { return guest::LoadFloat(base, 0x82002A34u); }
  static void set_particles_b_most_speed(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A34u, v); }
  static constexpr uint32_t particles_b_most_speed_address() { return 0x82002A34u; }
  static float particles_b_least_speed(uint8_t* base) { return guest::LoadFloat(base, 0x82002A3Cu); }
  static void set_particles_b_least_speed(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A3Cu, v); }
  static constexpr uint32_t particles_b_least_speed_address() { return 0x82002A3Cu; }
  static float particles_c_speed_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BF0u); }
  static void set_particles_c_speed_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BF0u, v); }
  static constexpr uint32_t particles_c_speed_scale_address() { return 0x82002BF0u; }
  static float particles_c_pull_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200874Cu); }
  static void set_particles_c_pull_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200874Cu, v); }
  static constexpr uint32_t particles_c_pull_scale_address() { return 0x8200874Cu; }
  // a vector added to the wheels' contacts
  static float contact_offset(uint8_t* base) { return guest::LoadFloat(base, 0x826C5D40u); }
  static void set_contact_offset(uint8_t* base, float v) { guest::StoreFloat(base, 0x826C5D40u, v); }
  static constexpr uint32_t contact_offset_address() { return 0x826C5D40u; }
  // a vector added to the trails' points
  static float trail_lift(uint8_t* base) { return guest::LoadFloat(base, 0x82616F40u); }
  static void set_trail_lift(uint8_t* base, float v) { guest::StoreFloat(base, 0x82616F40u, v); }
  static constexpr uint32_t trail_lift_address() { return 0x82616F40u; }
  static float smoke_spin_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200805Cu); }
  static void set_smoke_spin_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200805Cu, v); }
  static constexpr uint32_t smoke_spin_scale_address() { return 0x8200805Cu; }
  static float smoke_slip_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82008060u); }
  static void set_smoke_slip_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008060u, v); }
  static constexpr uint32_t smoke_slip_scale_address() { return 0x82008060u; }
  static float smoke_spin_most(uint8_t* base) { return guest::LoadFloat(base, 0x82002C38u); }
  static void set_smoke_spin_most(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C38u, v); }
  static constexpr uint32_t smoke_spin_most_address() { return 0x82002C38u; }
  static float smoke_slip_most(uint8_t* base) { return guest::LoadFloat(base, 0x820029D0u); }
  static void set_smoke_slip_most(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D0u, v); }
  static constexpr uint32_t smoke_slip_most_address() { return 0x820029D0u; }
  // ModelSprite::Entry's tables (9 words each)
  static uint32_t model_sprite_kind1(uint8_t* base) { return guest::LoadU32(base, 0x82616CC4u); }
  static void set_model_sprite_kind1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616CC4u, v); }
  static constexpr uint32_t model_sprite_kind1_address() { return 0x82616CC4u; }
  static uint32_t model_sprite_kind2(uint8_t* base) { return guest::LoadU32(base, 0x82616CE8u); }
  static void set_model_sprite_kind2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616CE8u, v); }
  static constexpr uint32_t model_sprite_kind2_address() { return 0x82616CE8u; }
  static uint32_t model_sprite_kind345(uint8_t* base) { return guest::LoadU32(base, 0x82616D0Cu); }
  static void set_model_sprite_kind345(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616D0Cu, v); }
  static constexpr uint32_t model_sprite_kind345_address() { return 0x82616D0Cu; }
  static uint32_t model_sprite_kind6(uint8_t* base) { return guest::LoadU32(base, 0x82616D30u); }
  static void set_model_sprite_kind6(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616D30u, v); }
  static constexpr uint32_t model_sprite_kind6_address() { return 0x82616D30u; }
  static uint32_t model_sprite_kind7(uint8_t* base) { return guest::LoadU32(base, 0x82616D54u); }
  static void set_model_sprite_kind7(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616D54u, v); }
  static constexpr uint32_t model_sprite_kind7_address() { return 0x82616D54u; }
  static uint32_t model_sprite_kind8(uint8_t* base) { return guest::LoadU32(base, 0x82616D78u); }
  static void set_model_sprite_kind8(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616D78u, v); }
  static constexpr uint32_t model_sprite_kind8_address() { return 0x82616D78u; }
  static uint32_t model_sprite_other(uint8_t* base) { return guest::LoadU32(base, 0x826C5CF0u); }
  static void set_model_sprite_other(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5CF0u, v); }
  static constexpr uint32_t model_sprite_other_address() { return 0x826C5CF0u; }
};

}  // namespace schema
