// The views of docs/functions/alt-hud.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/alt-hud.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: AltHud.+0
struct AltHud : View {
  using View::View;
  static constexpr uint32_t kSize = 264;
  // node 3's flag, the toggle AltHud::Tick flips on X
  uint32_t shown() const { return guest::LoadU32(base, address + 0); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t shown_address() const { return address + 0; }
  // the tachometer's HudCurve (one of three at 0x8200FD28)
  uint32_t tach_curve_address() const { return address + 4; }
  // the tachometer curve's output (its +100): the needle's rotation
  float tach_output() const { return guest::LoadFloat(base, address + 104); }
  void set_tach_output(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t tach_output_address() const { return address + 104; }
  // the boost's HudCurve (copied from 0x8200FE78)
  uint32_t boost_curve_address() const { return address + 116; }
  // the boost curve's output (its +100): the needle's rotation
  float boost_output() const { return guest::LoadFloat(base, address + 216); }
  void set_boost_output(float v) const { guest::StoreFloat(base, address + 216, v); }
  uint32_t boost_output_address() const { return address + 216; }
  // the bar's value (the car's +2420)
  float bar() const { return guest::LoadFloat(base, address + 228); }
  void set_bar(float v) const { guest::StoreFloat(base, address + 228, v); }
  uint32_t bar_address() const { return address + 228; }
  // the bar's range top; nothing found writes it (0.0)
  float bar_high() const { return guest::LoadFloat(base, address + 232); }
  void set_bar_high(float v) const { guest::StoreFloat(base, address + 232, v); }
  uint32_t bar_high_address() const { return address + 232; }
  // the bar's range bottom; nothing found writes it (0.0)
  float bar_low() const { return guest::LoadFloat(base, address + 236); }
  void set_bar_low(float v) const { guest::StoreFloat(base, address + 236, v); }
  uint32_t bar_low_address() const { return address + 236; }
  // a Vehicle: Vehicle::Get(player)
  View car() const { return View(base, guest::LoadU32(base, address + 240)); }
  void set_car(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t car_address() const { return address + 240; }
  // the first extra gauge: 0 float, 1 int to 119, 2 int to 149, -1 none
  int32_t slot0() const { return int32_t(guest::LoadU32(base, address + 244)); }
  void set_slot0(int32_t v) const { guest::StoreU32(base, address + 244, uint32_t(v)); }
  uint32_t slot0_address() const { return address + 244; }
  // the second extra gauge, as slot0
  int32_t slot1() const { return int32_t(guest::LoadU32(base, address + 248)); }
  void set_slot1(int32_t v) const { guest::StoreU32(base, address + 248, uint32_t(v)); }
  uint32_t slot1_address() const { return address + 248; }
  // the third extra gauge, as slot0
  int32_t slot2() const { return int32_t(guest::LoadU32(base, address + 252)); }
  void set_slot2(int32_t v) const { guest::StoreU32(base, address + 252, uint32_t(v)); }
  uint32_t slot2_address() const { return address + 252; }
  // AltHud::ToggleView's flag: the big speed readout shown
  uint32_t big_view() const { return guest::LoadU32(base, address + 256); }
  void set_big_view(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t big_view_address() const { return address + 256; }
  // the player (0 or 1)
  uint32_t player() const { return guest::LoadU32(base, address + 260); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t player_address() const { return address + 260; }
};

// embedded; measure: AltHud
struct AltHudHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 264 bytes, cleared
  template <typename T = AltHud>
  T object() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_object(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t object_address() const { return address + 0; }
  // a MenuList (72 bytes, one item); its X press flips the toggle
  View menu_list() const { return View(base, guest::LoadU32(base, address + 4)); }
  void set_menu_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t menu_list_address() const { return address + 4; }
};

// The image words docs/functions/alt-hud.md's code uses.
struct AltHudGlobals {
  // the car input records by player (racer-controllers.md): +4 the throttle, +6 the brake, +8 the steering, signed halfwords
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // AltHud::PowerClass: below the stock power times this, class 0
  static float power_low(uint8_t* base) { return guest::LoadFloat(base, 0x820029F0u); }
  static void set_power_low(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029F0u, v); }
  static constexpr uint32_t power_low_address() { return 0x820029F0u; }
  // AltHud::PowerClass: below the stock power times this, class 1, else 2
  static float power_high(uint8_t* base) { return guest::LoadFloat(base, 0x82002C40u); }
  static void set_power_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C40u, v); }
  static constexpr uint32_t power_high_address() { return 0x82002C40u; }
  // the input halfwords' scale, probably 1 / 32,767
  static float input_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD4u); }
  static void set_input_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD4u, v); }
  static constexpr uint32_t input_scale_address() { return 0x82002BD4u; }
  // the km/h's factor for the dials
  static double kmh_factor(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8200A510u)); }
  static void set_kmh_factor(uint8_t* base, double v) { guest::StoreU64(base, 0x8200A510u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t kmh_factor_address() { return 0x8200A510u; }
  // 10.0, the digits' base for PowInt
  static double ten(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8200F728u)); }
  static void set_ten(uint8_t* base, double v) { guest::StoreU64(base, 0x8200F728u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t ten_address() { return 0x8200F728u; }
  // added to the tachometer curve's output
  static float tach_offset(uint8_t* base) { return guest::LoadFloat(base, 0x8200F720u); }
  static void set_tach_offset(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200F720u, v); }
  static constexpr uint32_t tach_offset_address() { return 0x8200F720u; }
  // three 112-byte tachometer curves
  static constexpr uint32_t tach_curves_address() { return 0x8200FD28u; }
  // the boost's curve
  static constexpr uint32_t boost_curve_address() { return 0x8200FE78u; }
  // 16 bytes an index (the racer record's +60): word 0 the curve, words 1 to 3 the dial's faces by power class
  static constexpr uint32_t curve_table_address() { return 0x8200FEE8u; }
  // AltHud::SetSteering's scale
  static float steering_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82010368u); }
  static void set_steering_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82010368u, v); }
  static constexpr uint32_t steering_scale_address() { return 0x82010368u; }
};

}  // namespace schema
