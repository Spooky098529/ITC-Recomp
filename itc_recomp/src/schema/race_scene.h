// The views of docs/functions/race-scene.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-scene.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceSceneMachine
struct RaceSceneMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  // 200 transitions of 48 bytes (StateMachine's; 11 used)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 at Init, 2 the scenery loading, 38 the race, 32 after message 44, 37 after 49, 35 the end
  uint32_t state() const { return guest::LoadU32(address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // the toggle of messages 29 and 58 (probably the pause; TogglePause, Resume); 0 from OnStart
  uint32_t paused() const { return guest::LoadU32(address + 9608); }
  void set_paused(uint32_t v) const { guest::StoreU32(address + 9608, v); }
  uint32_t paused_address() const { return address + 9608; }
};

// image; measure: Glows
struct RaceGlowParams : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // Glow::Set8436 (slot 2)
  float c33_x_second() const { return guest::LoadFloat(address + 0); }
  void set_c33_x_second(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t c33_x_second_address() const { return address + 0; }
  // Glow::Set8440 (slot 3)
  float c33_x_first() const { return guest::LoadFloat(address + 4); }
  void set_c33_x_first(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t c33_x_first_address() const { return address + 4; }
  // Glow::Set8444 (slot 4)
  float c34_x() const { return guest::LoadFloat(address + 8); }
  void set_c34_x(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t c34_x_address() const { return address + 8; }
  // Glow::Set8448 (slot 5)
  float c34_y() const { return guest::LoadFloat(address + 12); }
  void set_c34_y(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t c34_y_address() const { return address + 12; }
  // Glow::SetStrength (slot 6)
  float strength() const { return guest::LoadFloat(address + 16); }
  void set_strength(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t strength_address() const { return address + 16; }
  // Glow::Set8456 (slot 7)
  float c33_z() const { return guest::LoadFloat(address + 20); }
  void set_c33_z(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t c33_z_address() const { return address + 20; }
  // Glow::SetHistoryWeight (slot 8)
  float history_weight() const { return guest::LoadFloat(address + 24); }
  void set_history_weight(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t history_weight_address() const { return address + 24; }
  // Glow::Set8464 (slot 9)
  float param_8464() const { return guest::LoadFloat(address + 28); }
  void set_param_8464(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t param_8464_address() const { return address + 28; }
  // Glow::Set8468 (slot 10)
  float star_step() const { return guest::LoadFloat(address + 32); }
  void set_star_step(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t star_step_address() const { return address + 32; }
  // Glow::Set8472 (slot 11)
  float star_attenuation() const { return guest::LoadFloat(address + 36); }
  void set_star_attenuation(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t star_attenuation_address() const { return address + 36; }
  // Glow::Set8476 (slot 12)
  float c33_w() const { return guest::LoadFloat(address + 40); }
  void set_c33_w(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t c33_w_address() const { return address + 40; }
};

// embedded; measure: RaceMode@+87936
struct RaceSceneMachineAt87936 : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  uint32_t unknown_9604() const { return guest::LoadU32(address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
};

// The image words docs/functions/race-scene.md's code uses.
struct RaceSceneGlobals {
  // the race scene (31,168 bytes, RaceScene::Create; RaceScene::Destroy leaves the pointer)
  template <typename T = RaceScene>
  static T scene() { return T(guest::LoadU32(0x826D82D4u)); }
  static void set_scene(const View& v) { guest::StoreU32(0x826D82D4u, v.address); }
  static void set_scene(uint32_t v) { guest::StoreU32(0x826D82D4u, v); }
  static constexpr uint32_t scene_address() { return 0x826D82D4u; }
  // the viewports (Viewports::Init): +8 nonzero in split screen
  template <typename T = Viewports>
  static T viewports() { return T(guest::LoadU32(0x826C2CE8u)); }
  static void set_viewports(const View& v) { guest::StoreU32(0x826C2CE8u, v.address); }
  static void set_viewports(uint32_t v) { guest::StoreU32(0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // 24 RaceGlowParams: 3 a kind (0 to 7) and its index
  static constexpr uint32_t glow_params_address() { return 0x82637CC0u; }
  // the RaceGlowParams for kinds 8 and up (just before the table)
  static constexpr uint32_t glow_params_other_address() { return 0x82637C94u; }
  // the color filters' Set8120 value (0.05, probably)
  static float filter_value() { return guest::LoadFloat(0x82002204u); }
  static void set_filter_value(float v) { guest::StoreFloat(0x82002204u, v); }
  static constexpr uint32_t filter_value_address() { return 0x82002204u; }
  // 1.0: the light entries' vectors' w
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // kind 1 entries' +32 x, y and z
  static float light_kind1_value() { return guest::LoadFloat(0x82001070u); }
  static void set_light_kind1_value(float v) { guest::StoreFloat(0x82001070u, v); }
  static constexpr uint32_t light_kind1_value_address() { return 0x82001070u; }
  // kind 2 entries' +32 x (probably 0.5)
  static float light_kind2_x() { return guest::LoadFloat(0x82001BE4u); }
  static void set_light_kind2_x(float v) { guest::StoreFloat(0x82001BE4u, v); }
  static constexpr uint32_t light_kind2_x_address() { return 0x82001BE4u; }
  // kind 2 entries' +64 x
  static float light_kind2_64_x() { return guest::LoadFloat(0x82002578u); }
  static void set_light_kind2_64_x(float v) { guest::StoreFloat(0x82002578u, v); }
  static constexpr uint32_t light_kind2_64_x_address() { return 0x82002578u; }
  // kind 2 entries' +64 y
  static float light_kind2_64_y() { return guest::LoadFloat(0x8200222Cu); }
  static void set_light_kind2_64_y(float v) { guest::StoreFloat(0x8200222Cu, v); }
  static constexpr uint32_t light_kind2_64_y_address() { return 0x8200222Cu; }
  // the entries' +16 p (500.0)
  static float light_color_p() { return guest::LoadFloat(0x82001FC0u); }
  static void set_light_color_p(float v) { guest::StoreFloat(0x82001FC0u, v); }
  static constexpr uint32_t light_color_p_address() { return 0x82001FC0u; }
  // the entries' +16 q
  static float light_color_q() { return guest::LoadFloat(0x82009798u); }
  static void set_light_color_q(float v) { guest::StoreFloat(0x82009798u, v); }
  static constexpr uint32_t light_color_q_address() { return 0x82009798u; }
  // kind 2 entries' +84: cos of it (probably the cone's inner angle)
  static double light_cone_inner() { return std::bit_cast<double>(guest::LoadU64(0x82009790u)); }
  static void set_light_cone_inner(double v) { guest::StoreU64(0x82009790u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t light_cone_inner_address() { return 0x82009790u; }
  // kind 2 entries' +80: cos of it (probably the cone's outer angle)
  static double light_cone_outer() { return std::bit_cast<double>(guest::LoadU64(0x82002FE0u)); }
  static void set_light_cone_outer(double v) { guest::StoreU64(0x82002FE0u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t light_cone_outer_address() { return 0x82002FE0u; }
};

}  // namespace schema
