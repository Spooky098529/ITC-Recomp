// The views of docs/functions/car-specs.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-specs.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<468>@Params::Init, Params::OnWheelsLoaded.r6, Params::OnTable83Loaded.r6, Params::OnCarSpecsLoaded.r6, Params::OnLoaded.r6
struct CarParams : View {
  using View::View;
  static constexpr uint32_t kSize = 468;
  // 12 at boot; each load's callback counts it down
  uint32_t loads_pending() const { return guest::LoadU32(base, address + 0); }
  void set_loads_pending(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t loads_pending_address() const { return address + 0; }
  // the block (operator new[]) tables 72-83 are loaded into
  uint32_t block() const { return guest::LoadU32(base, address + 4); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t block_address() const { return address + 4; }
  // table 73: the aftermarket wheels (48-byte records; Params::GetWheel)
  template <typename T = CarWheelRecord>
  T wheels() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_wheels(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_wheels(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t wheels_address() const { return address + 8; }
  // table 72: the car specs (8,432-byte records; Params::GetCarSpec)
  uint32_t specs() const { return guest::LoadU32(base, address + 12); }
  void set_specs(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t specs_address() const { return address + 12; }
  // table 74: aero part records (280 bytes a car)
  template <typename T = AeroPartRecord>
  T aero74() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_aero74(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_aero74(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t aero74_address() const { return address + 16; }
  // table 75: aero part records (280 bytes a car)
  template <typename T = AeroPartRecord>
  T aero75() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_aero75(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_aero75(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t aero75_address() const { return address + 20; }
  // table 76: aero part records (280 bytes a car)
  template <typename T = AeroPartRecord>
  T aero76() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_aero76(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_aero76(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t aero76_address() const { return address + 24; }
  // table 77: aero part records (280 bytes a car)
  template <typename T = AeroPartRecord>
  T aero77() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_aero77(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_aero77(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t aero77_address() const { return address + 28; }
  // table 78: aero part records (168 bytes a car)
  template <typename T = AeroPartRecord>
  T aero78() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_aero78(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_aero78(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t aero78_address() const { return address + 32; }
  // table 79: aero part records (168 bytes a car)
  template <typename T = AeroPartRecord>
  T aero79() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_aero79(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_aero79(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t aero79_address() const { return address + 36; }
  // table 80: aero part records (84 bytes a car)
  template <typename T = AeroPartRecord>
  T aero80() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_aero80(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_aero80(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t aero80_address() const { return address + 40; }
  // table 81: aero part records (108 bytes a car)
  template <typename T = AeroPartRecord>
  T aero81() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_aero81(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_aero81(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t aero81_address() const { return address + 44; }
  // table 82: aero part records (72 bytes a car)
  template <typename T = AeroPartRecord>
  T aero82() const { return T(base, guest::LoadU32(base, address + 48)); }
  void set_aero82(const View& v) const { guest::StoreU32(base, address + 48, v.address); }
  void set_aero82(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t aero82_address() const { return address + 48; }
  // table 83: 23 records of 10 floats (all 1.0 in the data; no reader found)
  uint32_t table83() const { return guest::LoadU32(base, address + 52); }
  void set_table83(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t table83_address() const { return address + 52; }
  // table 72's size / 8,432 (72)
  uint32_t spec_count() const { return guest::LoadU32(base, address + 56); }
  void set_spec_count(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t spec_count_address() const { return address + 56; }
  // table 73's size / 48 (178)
  uint32_t wheel_count() const { return guest::LoadU32(base, address + 60); }
  void set_wheel_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t wheel_count_address() const { return address + 60; }
  // +4m: the first wheel record of wheel model m (from 1; -1 before table 73's callback)
  uint32_t first_wheels_address() const { return address + 64; }
  // first_wheels' model 1 (0, written first by table 73's callback)
  uint32_t model1_first_wheel() const { return guest::LoadU32(base, address + 68); }
  void set_model1_first_wheel(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t model1_first_wheel_address() const { return address + 68; }
  // the last wheel model + 1
  uint32_t model_end() const { return guest::LoadU32(base, address + 464); }
  void set_model_end(uint32_t v) const { guest::StoreU32(base, address + 464, v); }
  uint32_t model_end_address() const { return address + 464; }
};

// file; measure: Params, AeroParts::Get82.r4, AeroParts::Get79.r4, CountBodyKits.r3
struct CarPartTables : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // the car's first record in table 74
  template <typename T = AeroPartRecord>
  T table74() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_table74(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_table74(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t table74_address() const { return address + 0; }
  // the car's first record in table 75
  template <typename T = AeroPartRecord>
  T table75() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_table75(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_table75(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t table75_address() const { return address + 4; }
  // the car's first record in table 76
  template <typename T = AeroPartRecord>
  T table76() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_table76(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_table76(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t table76_address() const { return address + 8; }
  // the car's first record in table 77
  template <typename T = AeroPartRecord>
  T table77() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_table77(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_table77(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t table77_address() const { return address + 12; }
  // the car's first record in table 78
  template <typename T = AeroPartRecord>
  T table78() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_table78(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_table78(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t table78_address() const { return address + 16; }
  // the car's first record in table 79
  template <typename T = AeroPartRecord>
  T table79() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_table79(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_table79(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t table79_address() const { return address + 20; }
  // the car's first record in table 80
  template <typename T = AeroPartRecord>
  T table80() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_table80(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_table80(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t table80_address() const { return address + 24; }
  // the car's first record in table 81
  template <typename T = AeroPartRecord>
  T table81() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_table81(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_table81(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t table81_address() const { return address + 28; }
  // the car's first record in table 82
  template <typename T = AeroPartRecord>
  T table82() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_table82(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_table82(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t table82_address() const { return address + 32; }
  // a measure fault: Params::SwapTable83Record's r3 (a 40-byte table 83 record being swapped) is filed here too
  uint32_t table83_raw_address() const { return address + 0; }
};

// file; measure: AeroParts, AeroPart
struct AeroPartRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // probably
  float weight_change() const { return guest::LoadFloat(base, address + 0); }
  void set_weight_change(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t weight_change_address() const { return address + 0; }
  // probably
  float weight_change_2() const { return guest::LoadFloat(base, address + 4); }
  void set_weight_change_2(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t weight_change_2_address() const { return address + 4; }
  // probably
  float lift() const { return guest::LoadFloat(base, address + 8); }
  void set_lift(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t lift_address() const { return address + 8; }
  // probably
  float lift_2() const { return guest::LoadFloat(base, address + 12); }
  void set_lift_2(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t lift_2_address() const { return address + 12; }
  // 1.0 for none
  float factor_16() const { return guest::LoadFloat(base, address + 16); }
  void set_factor_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t factor_16_address() const { return address + 16; }
  // 1.0 for none
  float factor_20() const { return guest::LoadFloat(base, address + 20); }
  void set_factor_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t factor_20_address() const { return address + 20; }
  // bits the tuning screens test (& 1, & 2, & 4: availability and types)
  uint32_t flags() const { return guest::LoadU32(base, address + 24); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t flags_address() const { return address + 24; }
  // tables 81 and 82 only
  float extra_28() const { return guest::LoadFloat(base, address + 28); }
  void set_extra_28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t extra_28_address() const { return address + 28; }
  // tables 81 and 82 only (CountBodyKits reads the first float only)
  float extra_32() const { return guest::LoadFloat(base, address + 32); }
  void set_extra_32(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t extra_32_address() const { return address + 32; }
  // the swap's byte stores (AeroPart::Swap, Swap36)
  uint32_t raw_address() const { return address + 0; }
};

// file; measure: CarSpec
struct CarSpecSwapWords : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // the words (block 1 +16 to +80 float4s with w = 1.0, +96, +160 matrices, +224, +244, +264, +356, +368; block 2 and its +8, +132, +300)
  uint32_t words_address() const { return address + 0; }
};

// file; measure: Tire
struct CarTire : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  // the car id (not swapped)
  uint32_t id_address() const { return address + 0; }
  // copied to the axle (CarData::FillAxle)
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // copied to the axle (CarData::FillAxle)
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // copied to the axle (CarData::FillAxle)
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // copied to the axle (CarData::FillAxle)
  float unknown_20() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // copied to the axle (CarData::FillAxle)
  float unknown_24() const { return guest::LoadFloat(base, address + 24); }
  void set_unknown_24(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // copied to the axle (CarData::FillAxle)
  float unknown_28() const { return guest::LoadFloat(base, address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // copied to the axle (CarData::FillAxle)
  float unknown_32() const { return guest::LoadFloat(base, address + 32); }
  void set_unknown_32(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  // copied to the axle (CarData::FillAxle)
  float unknown_36() const { return guest::LoadFloat(base, address + 36); }
  void set_unknown_36(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  // copied to the axle (CarData::FillAxle)
  float unknown_40() const { return guest::LoadFloat(base, address + 40); }
  void set_unknown_40(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // m: rim / 2 + width x aspect
  float radius() const { return guest::LoadFloat(base, address + 44); }
  void set_radius(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t radius_address() const { return address + 44; }
  // m
  float width() const { return guest::LoadFloat(base, address + 48); }
  void set_width(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t width_address() const { return address + 48; }
  // the wheel's, probably (about 26)
  float mass() const { return guest::LoadFloat(base, address + 52); }
  void set_mass(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t mass_address() const { return address + 52; }
  // copied to the axle (CarData::FillAxle)
  float unknown_56() const { return guest::LoadFloat(base, address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  // copied to the axle (CarData::FillAxle)
  float unknown_60() const { return guest::LoadFloat(base, address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  float grip() const { return guest::LoadFloat(base, address + 64); }
  void set_grip(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t grip_address() const { return address + 64; }
  // Tire::InchUp writes +64's value to both
  float grip_2() const { return guest::LoadFloat(base, address + 68); }
  void set_grip_2(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t grip_2_address() const { return address + 68; }
  // copied to the axle (CarData::FillAxle)
  float unknown_72() const { return guest::LoadFloat(base, address + 72); }
  void set_unknown_72(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  // copied to the axle (CarData::FillAxle)
  float unknown_76() const { return guest::LoadFloat(base, address + 76); }
  void set_unknown_76(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  // the rim diameter (in)
  float rim() const { return guest::LoadFloat(base, address + 80); }
  void set_rim(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t rim_address() const { return address + 80; }
  // the aspect ratio, a fraction (0.45)
  float aspect() const { return guest::LoadFloat(base, address + 84); }
  void set_aspect(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t aspect_address() const { return address + 84; }
  // in
  float rim_width() const { return guest::LoadFloat(base, address + 88); }
  void set_rim_width(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t rim_width_address() const { return address + 88; }
  // the swap's byte stores (Tire::Swap)
  uint32_t raw_address() const { return address + 0; }
};

// file; measure: EngineStage
struct CarEngineStage : View {
  using View::View;
  static constexpr uint32_t kSize = 204;
  // the id (not swapped)
  uint32_t id_address() const { return address + 0; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // probably
  float max_rpm() const { return guest::LoadFloat(base, address + 12); }
  void set_max_rpm(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t max_rpm_address() const { return address + 12; }
  // 12800
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float rev_limit() const { return guest::LoadFloat(base, address + 20); }
  void set_rev_limit(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t rev_limit_address() const { return address + 20; }
  // the torque x 10
  float torque_x10() const { return guest::LoadFloat(base, address + 24); }
  void set_torque_x10(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t torque_x10_address() const { return address + 24; }
  // copied to the car data, then overwritten (CarData::ConvertUnits)
  float unknown_156() const { return guest::LoadFloat(base, address + 156); }
  void set_unknown_156(float v) const { guest::StoreFloat(base, address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  float unknown_160() const { return guest::LoadFloat(base, address + 160); }
  void set_unknown_160(float v) const { guest::StoreFloat(base, address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  float unknown_164() const { return guest::LoadFloat(base, address + 164); }
  void set_unknown_164(float v) const { guest::StoreFloat(base, address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  float unknown_168() const { return guest::LoadFloat(base, address + 168); }
  void set_unknown_168(float v) const { guest::StoreFloat(base, address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  // the turbo flag
  uint32_t turbo() const { return guest::LoadU32(base, address + 172); }
  void set_turbo(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t turbo_address() const { return address + 172; }
  float boost() const { return guest::LoadFloat(base, address + 176); }
  void set_boost(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t boost_address() const { return address + 176; }
  float boost_2() const { return guest::LoadFloat(base, address + 180); }
  void set_boost_2(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t boost_2_address() const { return address + 180; }
  float boost_3() const { return guest::LoadFloat(base, address + 184); }
  void set_boost_3(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t boost_3_address() const { return address + 184; }
  float unknown_188() const { return guest::LoadFloat(base, address + 188); }
  void set_unknown_188(float v) const { guest::StoreFloat(base, address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  // probably the blow-off (the car data's +1605)
  uint32_t blow_off() const { return guest::LoadU32(base, address + 192); }
  void set_blow_off(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t blow_off_address() const { return address + 192; }
  float unknown_196() const { return guest::LoadFloat(base, address + 196); }
  void set_unknown_196(float v) const { guest::StoreFloat(base, address + 196, v); }
  uint32_t unknown_196_address() const { return address + 196; }
  // 32 floats
  uint32_t torque_curve_address() const { return address + 28; }
  // not swapped
  uint32_t tail_address() const { return address + 200; }
  // the swap's byte stores (EngineStage::Swap)
  uint32_t raw_address() const { return address + 0; }
};

// file; measure: Paint
struct CarPaints : View {
  using View::View;
  static constexpr uint32_t kSize = 272;
  // the car id
  uint32_t id_address() const { return address + 0; }
  // 13 CarPaintSlots of 20 bytes
  uint32_t slots_address() const { return address + 8; }
  // the slots used
  uint32_t count() const { return guest::LoadU32(base, address + 268); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t count_address() const { return address + 268; }
  // the swap's byte stores (Paint::Swap)
  uint32_t raw_address() const { return address + 0; }
};

// file; measure: -
struct CarPaintSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 0 solid, 1 metallic, 2 kept as given
  uint16_t type() const { return guest::LoadU16(base, address + 0); }
  void set_type(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  // color 0's hue
  uint16_t hue0() const { return guest::LoadU16(base, address + 4); }
  void set_hue0(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t hue0_address() const { return address + 4; }
  uint8_t saturation0() const { return guest::LoadU8(base, address + 6); }
  void set_saturation0(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t saturation0_address() const { return address + 6; }
  uint8_t value0() const { return guest::LoadU8(base, address + 7); }
  void set_value0(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t value0_address() const { return address + 7; }
  // color 1's hue
  uint16_t hue1() const { return guest::LoadU16(base, address + 8); }
  void set_hue1(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t hue1_address() const { return address + 8; }
  uint8_t saturation1() const { return guest::LoadU8(base, address + 10); }
  void set_saturation1(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t saturation1_address() const { return address + 10; }
  uint8_t value1() const { return guest::LoadU8(base, address + 11); }
  void set_value1(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t value1_address() const { return address + 11; }
  // color 2's hue (the second shade: type 0 gives 0, 0, 25; type 1 the hue, 60% of the saturation, 204)
  uint16_t hue2() const { return guest::LoadU16(base, address + 12); }
  void set_hue2(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t hue2_address() const { return address + 12; }
  uint8_t saturation2() const { return guest::LoadU8(base, address + 14); }
  void set_saturation2(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t saturation2_address() const { return address + 14; }
  uint8_t value2() const { return guest::LoadU8(base, address + 15); }
  void set_value2(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t value2_address() const { return address + 15; }
  // color 3's hue
  uint16_t hue3() const { return guest::LoadU16(base, address + 16); }
  void set_hue3(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t hue3_address() const { return address + 16; }
  uint8_t saturation3() const { return guest::LoadU8(base, address + 18); }
  void set_saturation3(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t saturation3_address() const { return address + 18; }
  uint8_t value3() const { return guest::LoadU8(base, address + 19); }
  void set_value3(uint8_t v) const { guest::StoreU8(base, address + 19, v); }
  uint32_t value3_address() const { return address + 19; }
};

// file; measure: Params::OnWheelsLoaded.r6.+8
struct CarWheelRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // not swapped
  uint32_t id_address() const { return address + 0; }
  // the first 12-byte color record (u16 +0, u16 +4, word +8)
  uint16_t color_a_half0() const { return guest::LoadU16(base, address + 8); }
  void set_color_a_half0(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t color_a_half0_address() const { return address + 8; }
  uint16_t color_a_half4() const { return guest::LoadU16(base, address + 12); }
  void set_color_a_half4(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t color_a_half4_address() const { return address + 12; }
  uint32_t color_a_word() const { return guest::LoadU32(base, address + 16); }
  void set_color_a_word(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t color_a_word_address() const { return address + 16; }
  // the second color record
  uint16_t color_b_half0() const { return guest::LoadU16(base, address + 20); }
  void set_color_b_half0(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t color_b_half0_address() const { return address + 20; }
  uint16_t color_b_half4() const { return guest::LoadU16(base, address + 24); }
  void set_color_b_half4(uint16_t v) const { guest::StoreU16(base, address + 24, v); }
  uint32_t color_b_half4_address() const { return address + 24; }
  uint32_t color_b_word() const { return guest::LoadU32(base, address + 28); }
  void set_color_b_word(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t color_b_word_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  // the wheel model (the id's index among the 64 ids; + 1 after load)
  uint32_t model() const { return guest::LoadU32(base, address + 40); }
  void set_model(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t model_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  // the swap's byte stores (Wheel::Swap, Wheel::SwapColor)
  uint32_t raw_address() const { return address + 0; }
};

// file; measure: Archive::Toc.ret
struct ArchiveTocBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 366252;
  uint32_t asset72_stored_size() const { return guest::LoadU32(base, address + 1172); }
  void set_asset72_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1172, v); }
  uint32_t asset72_stored_size_address() const { return address + 1172; }
  // 0 when stored as is
  uint32_t asset72_unpacked_size() const { return guest::LoadU32(base, address + 1176); }
  void set_asset72_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1176, v); }
  uint32_t asset72_unpacked_size_address() const { return address + 1176; }
  uint32_t asset73_stored_size() const { return guest::LoadU32(base, address + 1188); }
  void set_asset73_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1188, v); }
  uint32_t asset73_stored_size_address() const { return address + 1188; }
  // 0 when stored as is
  uint32_t asset73_unpacked_size() const { return guest::LoadU32(base, address + 1192); }
  void set_asset73_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1192, v); }
  uint32_t asset73_unpacked_size_address() const { return address + 1192; }
  uint32_t asset74_stored_size() const { return guest::LoadU32(base, address + 1204); }
  void set_asset74_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1204, v); }
  uint32_t asset74_stored_size_address() const { return address + 1204; }
  // 0 when stored as is
  uint32_t asset74_unpacked_size() const { return guest::LoadU32(base, address + 1208); }
  void set_asset74_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1208, v); }
  uint32_t asset74_unpacked_size_address() const { return address + 1208; }
  uint32_t asset75_stored_size() const { return guest::LoadU32(base, address + 1220); }
  void set_asset75_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1220, v); }
  uint32_t asset75_stored_size_address() const { return address + 1220; }
  // 0 when stored as is
  uint32_t asset75_unpacked_size() const { return guest::LoadU32(base, address + 1224); }
  void set_asset75_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1224, v); }
  uint32_t asset75_unpacked_size_address() const { return address + 1224; }
  uint32_t asset76_stored_size() const { return guest::LoadU32(base, address + 1236); }
  void set_asset76_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1236, v); }
  uint32_t asset76_stored_size_address() const { return address + 1236; }
  // 0 when stored as is
  uint32_t asset76_unpacked_size() const { return guest::LoadU32(base, address + 1240); }
  void set_asset76_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1240, v); }
  uint32_t asset76_unpacked_size_address() const { return address + 1240; }
  uint32_t asset77_stored_size() const { return guest::LoadU32(base, address + 1252); }
  void set_asset77_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1252, v); }
  uint32_t asset77_stored_size_address() const { return address + 1252; }
  // 0 when stored as is
  uint32_t asset77_unpacked_size() const { return guest::LoadU32(base, address + 1256); }
  void set_asset77_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1256, v); }
  uint32_t asset77_unpacked_size_address() const { return address + 1256; }
  uint32_t asset78_stored_size() const { return guest::LoadU32(base, address + 1268); }
  void set_asset78_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1268, v); }
  uint32_t asset78_stored_size_address() const { return address + 1268; }
  // 0 when stored as is
  uint32_t asset78_unpacked_size() const { return guest::LoadU32(base, address + 1272); }
  void set_asset78_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1272, v); }
  uint32_t asset78_unpacked_size_address() const { return address + 1272; }
  uint32_t asset79_stored_size() const { return guest::LoadU32(base, address + 1284); }
  void set_asset79_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1284, v); }
  uint32_t asset79_stored_size_address() const { return address + 1284; }
  // 0 when stored as is
  uint32_t asset79_unpacked_size() const { return guest::LoadU32(base, address + 1288); }
  void set_asset79_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1288, v); }
  uint32_t asset79_unpacked_size_address() const { return address + 1288; }
  uint32_t asset80_stored_size() const { return guest::LoadU32(base, address + 1300); }
  void set_asset80_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1300, v); }
  uint32_t asset80_stored_size_address() const { return address + 1300; }
  // 0 when stored as is
  uint32_t asset80_unpacked_size() const { return guest::LoadU32(base, address + 1304); }
  void set_asset80_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1304, v); }
  uint32_t asset80_unpacked_size_address() const { return address + 1304; }
  uint32_t asset81_stored_size() const { return guest::LoadU32(base, address + 1316); }
  void set_asset81_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1316, v); }
  uint32_t asset81_stored_size_address() const { return address + 1316; }
  // 0 when stored as is
  uint32_t asset81_unpacked_size() const { return guest::LoadU32(base, address + 1320); }
  void set_asset81_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1320, v); }
  uint32_t asset81_unpacked_size_address() const { return address + 1320; }
  uint32_t asset82_stored_size() const { return guest::LoadU32(base, address + 1332); }
  void set_asset82_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1332, v); }
  uint32_t asset82_stored_size_address() const { return address + 1332; }
  // 0 when stored as is
  uint32_t asset82_unpacked_size() const { return guest::LoadU32(base, address + 1336); }
  void set_asset82_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1336, v); }
  uint32_t asset82_unpacked_size_address() const { return address + 1336; }
  uint32_t asset83_stored_size() const { return guest::LoadU32(base, address + 1348); }
  void set_asset83_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1348, v); }
  uint32_t asset83_stored_size_address() const { return address + 1348; }
  // 0 when stored as is
  uint32_t asset83_unpacked_size() const { return guest::LoadU32(base, address + 1352); }
  void set_asset83_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1352, v); }
  uint32_t asset83_unpacked_size_address() const { return address + 1352; }
  // read by DataTables::Init
  uint32_t asset84_stored_size() const { return guest::LoadU32(base, address + 1364); }
  void set_asset84_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1364, v); }
  uint32_t asset84_stored_size_address() const { return address + 1364; }
  // 0 when stored as is
  uint32_t asset84_unpacked_size() const { return guest::LoadU32(base, address + 1368); }
  void set_asset84_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1368, v); }
  uint32_t asset84_unpacked_size_address() const { return address + 1368; }
  uint32_t asset94_stored_size() const { return guest::LoadU32(base, address + 1524); }
  void set_asset94_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1524, v); }
  uint32_t asset94_stored_size_address() const { return address + 1524; }
  // 0 when stored as is
  uint32_t asset94_unpacked_size() const { return guest::LoadU32(base, address + 1528); }
  void set_asset94_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1528, v); }
  uint32_t asset94_unpacked_size_address() const { return address + 1528; }
  uint32_t asset95_stored_size() const { return guest::LoadU32(base, address + 1540); }
  void set_asset95_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1540, v); }
  uint32_t asset95_stored_size_address() const { return address + 1540; }
  // 0 when stored as is
  uint32_t asset95_unpacked_size() const { return guest::LoadU32(base, address + 1544); }
  void set_asset95_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1544, v); }
  uint32_t asset95_unpacked_size_address() const { return address + 1544; }
  uint32_t asset96_stored_size() const { return guest::LoadU32(base, address + 1556); }
  void set_asset96_stored_size(uint32_t v) const { guest::StoreU32(base, address + 1556, v); }
  uint32_t asset96_stored_size_address() const { return address + 1556; }
  // 0 when stored as is
  uint32_t asset96_unpacked_size() const { return guest::LoadU32(base, address + 1560); }
  void set_asset96_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 1560, v); }
  uint32_t asset96_unpacked_size_address() const { return address + 1560; }
  // read by GarageCamera::LoadPresets
  uint32_t asset22600_stored_size() const { return guest::LoadU32(base, address + 361620); }
  void set_asset22600_stored_size(uint32_t v) const { guest::StoreU32(base, address + 361620, v); }
  uint32_t asset22600_stored_size_address() const { return address + 361620; }
  // 0 when stored as is
  uint32_t asset22600_unpacked_size() const { return guest::LoadU32(base, address + 361624); }
  void set_asset22600_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 361624, v); }
  uint32_t asset22600_unpacked_size_address() const { return address + 361624; }
  // read by RaceSounds::Begin
  uint32_t asset22889_stored_size() const { return guest::LoadU32(base, address + 366244); }
  void set_asset22889_stored_size(uint32_t v) const { guest::StoreU32(base, address + 366244, v); }
  uint32_t asset22889_stored_size_address() const { return address + 366244; }
  // 0 when stored as is
  uint32_t asset22889_unpacked_size() const { return guest::LoadU32(base, address + 366248); }
  void set_asset22889_unpacked_size(uint32_t v) const { guest::StoreU32(base, address + 366248, v); }
  uint32_t asset22889_unpacked_size_address() const { return address + 366248; }
};

// The image words docs/functions/car-specs.md's code uses.
struct CarSpecsGlobals {
  // 0 in the image; Params::Init makes it, Params::Free clears it
  template <typename T = CarParams>
  static T params(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E48u)); }
  static void set_params(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E48u, v.address); }
  static void set_params(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E48u, v); }
  static constexpr uint32_t params_address() { return 0x826C2E48u; }
  // 1.0f (copied as a word)
  static uint32_t float_one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_float_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t float_one_address() { return 0x82000A30u; }
  // 0.0f (copied as a word)
  static uint32_t float_zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
};

}  // namespace schema
