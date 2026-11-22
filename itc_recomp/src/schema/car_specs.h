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

// file; measure: CarSpec, CarSpec::Swap5.r3
struct CarSpecSwapWords : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // the words (block 1 +16 to +80 float4s with w = 1.0, +96, +160 matrices, +224, +244, +264, +356, +368; block 2 and its +8, +132, +300)
  uint32_t words_address() const { return address + 0; }
};

// file; measure: CarSpec::Swap.r3@+4244, Tire::InchUp.r3
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

// file; measure: CarSpec::Swap.r3@+980
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

// file; measure: CarSpec::Swap.r3@+8108
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

// embedded; measure: CarSpec::Swap.r3@+640
struct CarSpecSwapWordsAt640 : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  uint8_t unknown_72() const { return guest::LoadU8(base, address + 72); }
  void set_unknown_72(uint8_t v) const { guest::StoreU8(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  float unknown_72_4f() const { return guest::LoadFloat(base, address + 72); }
  void set_unknown_72_4f(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t unknown_72_4f_address() const { return address + 72; }
  uint8_t unknown_73() const { return guest::LoadU8(base, address + 73); }
  void set_unknown_73(uint8_t v) const { guest::StoreU8(base, address + 73, v); }
  uint32_t unknown_73_address() const { return address + 73; }
  uint8_t unknown_74() const { return guest::LoadU8(base, address + 74); }
  void set_unknown_74(uint8_t v) const { guest::StoreU8(base, address + 74, v); }
  uint32_t unknown_74_address() const { return address + 74; }
  uint8_t unknown_75() const { return guest::LoadU8(base, address + 75); }
  void set_unknown_75(uint8_t v) const { guest::StoreU8(base, address + 75, v); }
  uint32_t unknown_75_address() const { return address + 75; }
  uint8_t unknown_76() const { return guest::LoadU8(base, address + 76); }
  void set_unknown_76(uint8_t v) const { guest::StoreU8(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  float unknown_76_4f() const { return guest::LoadFloat(base, address + 76); }
  void set_unknown_76_4f(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t unknown_76_4f_address() const { return address + 76; }
  uint8_t unknown_77() const { return guest::LoadU8(base, address + 77); }
  void set_unknown_77(uint8_t v) const { guest::StoreU8(base, address + 77, v); }
  uint32_t unknown_77_address() const { return address + 77; }
  uint8_t unknown_78() const { return guest::LoadU8(base, address + 78); }
  void set_unknown_78(uint8_t v) const { guest::StoreU8(base, address + 78, v); }
  uint32_t unknown_78_address() const { return address + 78; }
  uint8_t unknown_79() const { return guest::LoadU8(base, address + 79); }
  void set_unknown_79(uint8_t v) const { guest::StoreU8(base, address + 79, v); }
  uint32_t unknown_79_address() const { return address + 79; }
  uint8_t unknown_88() const { return guest::LoadU8(base, address + 88); }
  void set_unknown_88(uint8_t v) const { guest::StoreU8(base, address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  float unknown_88_4f() const { return guest::LoadFloat(base, address + 88); }
  void set_unknown_88_4f(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t unknown_88_4f_address() const { return address + 88; }
  uint8_t unknown_89() const { return guest::LoadU8(base, address + 89); }
  void set_unknown_89(uint8_t v) const { guest::StoreU8(base, address + 89, v); }
  uint32_t unknown_89_address() const { return address + 89; }
  uint8_t unknown_90() const { return guest::LoadU8(base, address + 90); }
  void set_unknown_90(uint8_t v) const { guest::StoreU8(base, address + 90, v); }
  uint32_t unknown_90_address() const { return address + 90; }
  uint8_t unknown_91() const { return guest::LoadU8(base, address + 91); }
  void set_unknown_91(uint8_t v) const { guest::StoreU8(base, address + 91, v); }
  uint32_t unknown_91_address() const { return address + 91; }
  uint8_t unknown_92() const { return guest::LoadU8(base, address + 92); }
  void set_unknown_92(uint8_t v) const { guest::StoreU8(base, address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  float unknown_92_4f() const { return guest::LoadFloat(base, address + 92); }
  void set_unknown_92_4f(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t unknown_92_4f_address() const { return address + 92; }
  uint8_t unknown_93() const { return guest::LoadU8(base, address + 93); }
  void set_unknown_93(uint8_t v) const { guest::StoreU8(base, address + 93, v); }
  uint32_t unknown_93_address() const { return address + 93; }
  uint8_t unknown_94() const { return guest::LoadU8(base, address + 94); }
  void set_unknown_94(uint8_t v) const { guest::StoreU8(base, address + 94, v); }
  uint32_t unknown_94_address() const { return address + 94; }
  uint8_t unknown_95() const { return guest::LoadU8(base, address + 95); }
  void set_unknown_95(uint8_t v) const { guest::StoreU8(base, address + 95, v); }
  uint32_t unknown_95_address() const { return address + 95; }
  uint8_t unknown_96() const { return guest::LoadU8(base, address + 96); }
  void set_unknown_96(uint8_t v) const { guest::StoreU8(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  float unknown_96_4f() const { return guest::LoadFloat(base, address + 96); }
  void set_unknown_96_4f(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t unknown_96_4f_address() const { return address + 96; }
  uint8_t unknown_97() const { return guest::LoadU8(base, address + 97); }
  void set_unknown_97(uint8_t v) const { guest::StoreU8(base, address + 97, v); }
  uint32_t unknown_97_address() const { return address + 97; }
  uint8_t unknown_98() const { return guest::LoadU8(base, address + 98); }
  void set_unknown_98(uint8_t v) const { guest::StoreU8(base, address + 98, v); }
  uint32_t unknown_98_address() const { return address + 98; }
  uint8_t unknown_99() const { return guest::LoadU8(base, address + 99); }
  void set_unknown_99(uint8_t v) const { guest::StoreU8(base, address + 99, v); }
  uint32_t unknown_99_address() const { return address + 99; }
  uint8_t unknown_100() const { return guest::LoadU8(base, address + 100); }
  void set_unknown_100(uint8_t v) const { guest::StoreU8(base, address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  float unknown_100_4f() const { return guest::LoadFloat(base, address + 100); }
  void set_unknown_100_4f(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t unknown_100_4f_address() const { return address + 100; }
  uint8_t unknown_101() const { return guest::LoadU8(base, address + 101); }
  void set_unknown_101(uint8_t v) const { guest::StoreU8(base, address + 101, v); }
  uint32_t unknown_101_address() const { return address + 101; }
  uint8_t unknown_102() const { return guest::LoadU8(base, address + 102); }
  void set_unknown_102(uint8_t v) const { guest::StoreU8(base, address + 102, v); }
  uint32_t unknown_102_address() const { return address + 102; }
  uint8_t unknown_103() const { return guest::LoadU8(base, address + 103); }
  void set_unknown_103(uint8_t v) const { guest::StoreU8(base, address + 103, v); }
  uint32_t unknown_103_address() const { return address + 103; }
  uint8_t unknown_104() const { return guest::LoadU8(base, address + 104); }
  void set_unknown_104(uint8_t v) const { guest::StoreU8(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  float unknown_104_4f() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104_4f(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_4f_address() const { return address + 104; }
  uint8_t unknown_105() const { return guest::LoadU8(base, address + 105); }
  void set_unknown_105(uint8_t v) const { guest::StoreU8(base, address + 105, v); }
  uint32_t unknown_105_address() const { return address + 105; }
  uint8_t unknown_106() const { return guest::LoadU8(base, address + 106); }
  void set_unknown_106(uint8_t v) const { guest::StoreU8(base, address + 106, v); }
  uint32_t unknown_106_address() const { return address + 106; }
  uint8_t unknown_107() const { return guest::LoadU8(base, address + 107); }
  void set_unknown_107(uint8_t v) const { guest::StoreU8(base, address + 107, v); }
  uint32_t unknown_107_address() const { return address + 107; }
  uint8_t unknown_108() const { return guest::LoadU8(base, address + 108); }
  void set_unknown_108(uint8_t v) const { guest::StoreU8(base, address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  float unknown_108_4f() const { return guest::LoadFloat(base, address + 108); }
  void set_unknown_108_4f(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t unknown_108_4f_address() const { return address + 108; }
  uint8_t unknown_109() const { return guest::LoadU8(base, address + 109); }
  void set_unknown_109(uint8_t v) const { guest::StoreU8(base, address + 109, v); }
  uint32_t unknown_109_address() const { return address + 109; }
  uint8_t unknown_110() const { return guest::LoadU8(base, address + 110); }
  void set_unknown_110(uint8_t v) const { guest::StoreU8(base, address + 110, v); }
  uint32_t unknown_110_address() const { return address + 110; }
  uint8_t unknown_111() const { return guest::LoadU8(base, address + 111); }
  void set_unknown_111(uint8_t v) const { guest::StoreU8(base, address + 111, v); }
  uint32_t unknown_111_address() const { return address + 111; }
  uint8_t unknown_112() const { return guest::LoadU8(base, address + 112); }
  void set_unknown_112(uint8_t v) const { guest::StoreU8(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  float unknown_112_4f() const { return guest::LoadFloat(base, address + 112); }
  void set_unknown_112_4f(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t unknown_112_4f_address() const { return address + 112; }
  uint8_t unknown_113() const { return guest::LoadU8(base, address + 113); }
  void set_unknown_113(uint8_t v) const { guest::StoreU8(base, address + 113, v); }
  uint32_t unknown_113_address() const { return address + 113; }
  uint8_t unknown_114() const { return guest::LoadU8(base, address + 114); }
  void set_unknown_114(uint8_t v) const { guest::StoreU8(base, address + 114, v); }
  uint32_t unknown_114_address() const { return address + 114; }
  uint8_t unknown_115() const { return guest::LoadU8(base, address + 115); }
  void set_unknown_115(uint8_t v) const { guest::StoreU8(base, address + 115, v); }
  uint32_t unknown_115_address() const { return address + 115; }
  uint8_t unknown_116() const { return guest::LoadU8(base, address + 116); }
  void set_unknown_116(uint8_t v) const { guest::StoreU8(base, address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  float unknown_116_4f() const { return guest::LoadFloat(base, address + 116); }
  void set_unknown_116_4f(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t unknown_116_4f_address() const { return address + 116; }
  uint8_t unknown_117() const { return guest::LoadU8(base, address + 117); }
  void set_unknown_117(uint8_t v) const { guest::StoreU8(base, address + 117, v); }
  uint32_t unknown_117_address() const { return address + 117; }
  uint8_t unknown_118() const { return guest::LoadU8(base, address + 118); }
  void set_unknown_118(uint8_t v) const { guest::StoreU8(base, address + 118, v); }
  uint32_t unknown_118_address() const { return address + 118; }
  uint8_t unknown_119() const { return guest::LoadU8(base, address + 119); }
  void set_unknown_119(uint8_t v) const { guest::StoreU8(base, address + 119, v); }
  uint32_t unknown_119_address() const { return address + 119; }
  uint8_t unknown_120() const { return guest::LoadU8(base, address + 120); }
  void set_unknown_120(uint8_t v) const { guest::StoreU8(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  float unknown_120_4f() const { return guest::LoadFloat(base, address + 120); }
  void set_unknown_120_4f(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t unknown_120_4f_address() const { return address + 120; }
  uint8_t unknown_121() const { return guest::LoadU8(base, address + 121); }
  void set_unknown_121(uint8_t v) const { guest::StoreU8(base, address + 121, v); }
  uint32_t unknown_121_address() const { return address + 121; }
  uint8_t unknown_122() const { return guest::LoadU8(base, address + 122); }
  void set_unknown_122(uint8_t v) const { guest::StoreU8(base, address + 122, v); }
  uint32_t unknown_122_address() const { return address + 122; }
  uint8_t unknown_123() const { return guest::LoadU8(base, address + 123); }
  void set_unknown_123(uint8_t v) const { guest::StoreU8(base, address + 123, v); }
  uint32_t unknown_123_address() const { return address + 123; }
  uint8_t unknown_124() const { return guest::LoadU8(base, address + 124); }
  void set_unknown_124(uint8_t v) const { guest::StoreU8(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  float unknown_124_4f() const { return guest::LoadFloat(base, address + 124); }
  void set_unknown_124_4f(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t unknown_124_4f_address() const { return address + 124; }
  uint8_t unknown_125() const { return guest::LoadU8(base, address + 125); }
  void set_unknown_125(uint8_t v) const { guest::StoreU8(base, address + 125, v); }
  uint32_t unknown_125_address() const { return address + 125; }
  uint8_t unknown_126() const { return guest::LoadU8(base, address + 126); }
  void set_unknown_126(uint8_t v) const { guest::StoreU8(base, address + 126, v); }
  uint32_t unknown_126_address() const { return address + 126; }
  uint8_t unknown_127() const { return guest::LoadU8(base, address + 127); }
  void set_unknown_127(uint8_t v) const { guest::StoreU8(base, address + 127, v); }
  uint32_t unknown_127_address() const { return address + 127; }
};

// embedded; measure: CarSpec::Swap.r3@+640@+132
struct CarSpecSwapWordsAt132 : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_0_4f() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0_4f(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_4f_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_3() const { return guest::LoadU8(base, address + 3); }
  void set_unknown_3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t unknown_3_address() const { return address + 3; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_4_4f() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4_4f(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_4f_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_8_4f() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8_4f(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_4f_address() const { return address + 8; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
  uint8_t unknown_10() const { return guest::LoadU8(base, address + 10); }
  void set_unknown_10(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_12_4f() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12_4f(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_4f_address() const { return address + 12; }
  uint8_t unknown_13() const { return guest::LoadU8(base, address + 13); }
  void set_unknown_13(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t unknown_13_address() const { return address + 13; }
  uint8_t unknown_14() const { return guest::LoadU8(base, address + 14); }
  void set_unknown_14(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t unknown_14_address() const { return address + 14; }
  uint8_t unknown_15() const { return guest::LoadU8(base, address + 15); }
  void set_unknown_15(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t unknown_15_address() const { return address + 15; }
  uint8_t unknown_16() const { return guest::LoadU8(base, address + 16); }
  void set_unknown_16(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float unknown_16_4f() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16_4f(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_4f_address() const { return address + 16; }
  uint8_t unknown_17() const { return guest::LoadU8(base, address + 17); }
  void set_unknown_17(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t unknown_17_address() const { return address + 17; }
  uint8_t unknown_18() const { return guest::LoadU8(base, address + 18); }
  void set_unknown_18(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t unknown_18_address() const { return address + 18; }
  uint8_t unknown_19() const { return guest::LoadU8(base, address + 19); }
  void set_unknown_19(uint8_t v) const { guest::StoreU8(base, address + 19, v); }
  uint32_t unknown_19_address() const { return address + 19; }
  uint8_t unknown_20() const { return guest::LoadU8(base, address + 20); }
  void set_unknown_20(uint8_t v) const { guest::StoreU8(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  float unknown_20_4f() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20_4f(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_4f_address() const { return address + 20; }
  uint8_t unknown_21() const { return guest::LoadU8(base, address + 21); }
  void set_unknown_21(uint8_t v) const { guest::StoreU8(base, address + 21, v); }
  uint32_t unknown_21_address() const { return address + 21; }
  uint8_t unknown_22() const { return guest::LoadU8(base, address + 22); }
  void set_unknown_22(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t unknown_22_address() const { return address + 22; }
  uint8_t unknown_23() const { return guest::LoadU8(base, address + 23); }
  void set_unknown_23(uint8_t v) const { guest::StoreU8(base, address + 23, v); }
  uint32_t unknown_23_address() const { return address + 23; }
  uint8_t unknown_24() const { return guest::LoadU8(base, address + 24); }
  void set_unknown_24(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  float unknown_24_4f() const { return guest::LoadFloat(base, address + 24); }
  void set_unknown_24_4f(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t unknown_24_4f_address() const { return address + 24; }
  uint8_t unknown_25() const { return guest::LoadU8(base, address + 25); }
  void set_unknown_25(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t unknown_25_address() const { return address + 25; }
  uint8_t unknown_26() const { return guest::LoadU8(base, address + 26); }
  void set_unknown_26(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t unknown_26_address() const { return address + 26; }
  uint8_t unknown_27() const { return guest::LoadU8(base, address + 27); }
  void set_unknown_27(uint8_t v) const { guest::StoreU8(base, address + 27, v); }
  uint32_t unknown_27_address() const { return address + 27; }
  uint8_t unknown_28() const { return guest::LoadU8(base, address + 28); }
  void set_unknown_28(uint8_t v) const { guest::StoreU8(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_28_4f() const { return guest::LoadFloat(base, address + 28); }
  void set_unknown_28_4f(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t unknown_28_4f_address() const { return address + 28; }
  uint8_t unknown_29() const { return guest::LoadU8(base, address + 29); }
  void set_unknown_29(uint8_t v) const { guest::StoreU8(base, address + 29, v); }
  uint32_t unknown_29_address() const { return address + 29; }
  uint8_t unknown_30() const { return guest::LoadU8(base, address + 30); }
  void set_unknown_30(uint8_t v) const { guest::StoreU8(base, address + 30, v); }
  uint32_t unknown_30_address() const { return address + 30; }
  uint8_t unknown_31() const { return guest::LoadU8(base, address + 31); }
  void set_unknown_31(uint8_t v) const { guest::StoreU8(base, address + 31, v); }
  uint32_t unknown_31_address() const { return address + 31; }
  uint8_t unknown_32() const { return guest::LoadU8(base, address + 32); }
  void set_unknown_32(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  float unknown_32_4f() const { return guest::LoadFloat(base, address + 32); }
  void set_unknown_32_4f(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t unknown_32_4f_address() const { return address + 32; }
  uint8_t unknown_33() const { return guest::LoadU8(base, address + 33); }
  void set_unknown_33(uint8_t v) const { guest::StoreU8(base, address + 33, v); }
  uint32_t unknown_33_address() const { return address + 33; }
  uint8_t unknown_34() const { return guest::LoadU8(base, address + 34); }
  void set_unknown_34(uint8_t v) const { guest::StoreU8(base, address + 34, v); }
  uint32_t unknown_34_address() const { return address + 34; }
  uint8_t unknown_35() const { return guest::LoadU8(base, address + 35); }
  void set_unknown_35(uint8_t v) const { guest::StoreU8(base, address + 35, v); }
  uint32_t unknown_35_address() const { return address + 35; }
  uint8_t unknown_36() const { return guest::LoadU8(base, address + 36); }
  void set_unknown_36(uint8_t v) const { guest::StoreU8(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  float unknown_36_4f() const { return guest::LoadFloat(base, address + 36); }
  void set_unknown_36_4f(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t unknown_36_4f_address() const { return address + 36; }
  uint8_t unknown_37() const { return guest::LoadU8(base, address + 37); }
  void set_unknown_37(uint8_t v) const { guest::StoreU8(base, address + 37, v); }
  uint32_t unknown_37_address() const { return address + 37; }
  uint8_t unknown_38() const { return guest::LoadU8(base, address + 38); }
  void set_unknown_38(uint8_t v) const { guest::StoreU8(base, address + 38, v); }
  uint32_t unknown_38_address() const { return address + 38; }
  uint8_t unknown_39() const { return guest::LoadU8(base, address + 39); }
  void set_unknown_39(uint8_t v) const { guest::StoreU8(base, address + 39, v); }
  uint32_t unknown_39_address() const { return address + 39; }
};

// embedded; measure: CarSpec::Swap.r3@+640@+300
struct CarSpecSwapWordsAt300 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_0_4f() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0_4f(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_4f_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_3() const { return guest::LoadU8(base, address + 3); }
  void set_unknown_3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t unknown_3_address() const { return address + 3; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_4_4f() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4_4f(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_4f_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_8_4f() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8_4f(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_4f_address() const { return address + 8; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
  uint8_t unknown_10() const { return guest::LoadU8(base, address + 10); }
  void set_unknown_10(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_12_4f() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12_4f(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_4f_address() const { return address + 12; }
  uint8_t unknown_13() const { return guest::LoadU8(base, address + 13); }
  void set_unknown_13(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t unknown_13_address() const { return address + 13; }
  uint8_t unknown_14() const { return guest::LoadU8(base, address + 14); }
  void set_unknown_14(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t unknown_14_address() const { return address + 14; }
  uint8_t unknown_15() const { return guest::LoadU8(base, address + 15); }
  void set_unknown_15(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t unknown_15_address() const { return address + 15; }
};

// embedded; measure: CarSpec::Swap.r3@+640@+8
struct CarSpecSwapWordsAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_0_4f() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0_4f(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_4f_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_3() const { return guest::LoadU8(base, address + 3); }
  void set_unknown_3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t unknown_3_address() const { return address + 3; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_4_4f() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4_4f(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_4f_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_12_4f() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12_4f(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_4f_address() const { return address + 12; }
  uint8_t unknown_13() const { return guest::LoadU8(base, address + 13); }
  void set_unknown_13(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t unknown_13_address() const { return address + 13; }
  uint8_t unknown_14() const { return guest::LoadU8(base, address + 14); }
  void set_unknown_14(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t unknown_14_address() const { return address + 14; }
  uint8_t unknown_15() const { return guest::LoadU8(base, address + 15); }
  void set_unknown_15(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t unknown_15_address() const { return address + 15; }
  float unknown_20() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint8_t unknown_56() const { return guest::LoadU8(base, address + 56); }
  void set_unknown_56(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  float unknown_56_4f() const { return guest::LoadFloat(base, address + 56); }
  void set_unknown_56_4f(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t unknown_56_4f_address() const { return address + 56; }
  uint8_t unknown_57() const { return guest::LoadU8(base, address + 57); }
  void set_unknown_57(uint8_t v) const { guest::StoreU8(base, address + 57, v); }
  uint32_t unknown_57_address() const { return address + 57; }
  uint8_t unknown_58() const { return guest::LoadU8(base, address + 58); }
  void set_unknown_58(uint8_t v) const { guest::StoreU8(base, address + 58, v); }
  uint32_t unknown_58_address() const { return address + 58; }
  uint8_t unknown_59() const { return guest::LoadU8(base, address + 59); }
  void set_unknown_59(uint8_t v) const { guest::StoreU8(base, address + 59, v); }
  uint32_t unknown_59_address() const { return address + 59; }
};

// embedded; measure: CarSpec::SwapBlock1.r3@+16
struct CarSpecSwapWordsAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_0_4f() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0_4f(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_4f_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_3() const { return guest::LoadU8(base, address + 3); }
  void set_unknown_3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t unknown_3_address() const { return address + 3; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_4_4f() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4_4f(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_4f_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_8_4f() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8_4f(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_4f_address() const { return address + 8; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
  uint8_t unknown_10() const { return guest::LoadU8(base, address + 10); }
  void set_unknown_10(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: CarSpec::SwapMatrix.ret@+64@+64@+64
struct CarSpecSwapWordsAt64 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_0_4f() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0_4f(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_4f_address() const { return address + 0; }
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
  uint8_t unknown_2() const { return guest::LoadU8(base, address + 2); }
  void set_unknown_2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint8_t unknown_3() const { return guest::LoadU8(base, address + 3); }
  void set_unknown_3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t unknown_3_address() const { return address + 3; }
  uint8_t unknown_4() const { return guest::LoadU8(base, address + 4); }
  void set_unknown_4(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_4_4f() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4_4f(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_4f_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_7() const { return guest::LoadU8(base, address + 7); }
  void set_unknown_7(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t unknown_7_address() const { return address + 7; }
  uint8_t unknown_8() const { return guest::LoadU8(base, address + 8); }
  void set_unknown_8(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_8_4f() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8_4f(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_4f_address() const { return address + 8; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
  uint8_t unknown_10() const { return guest::LoadU8(base, address + 10); }
  void set_unknown_10(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t unknown_10_address() const { return address + 10; }
  uint8_t unknown_11() const { return guest::LoadU8(base, address + 11); }
  void set_unknown_11(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t unknown_11_address() const { return address + 11; }
  uint8_t unknown_12() const { return guest::LoadU8(base, address + 12); }
  void set_unknown_12(uint8_t v) const { guest::StoreU8(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_12_4f() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12_4f(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_4f_address() const { return address + 12; }
  uint8_t unknown_13() const { return guest::LoadU8(base, address + 13); }
  void set_unknown_13(uint8_t v) const { guest::StoreU8(base, address + 13, v); }
  uint32_t unknown_13_address() const { return address + 13; }
  uint8_t unknown_14() const { return guest::LoadU8(base, address + 14); }
  void set_unknown_14(uint8_t v) const { guest::StoreU8(base, address + 14, v); }
  uint32_t unknown_14_address() const { return address + 14; }
  uint8_t unknown_15() const { return guest::LoadU8(base, address + 15); }
  void set_unknown_15(uint8_t v) const { guest::StoreU8(base, address + 15, v); }
  uint32_t unknown_15_address() const { return address + 15; }
  uint8_t unknown_16() const { return guest::LoadU8(base, address + 16); }
  void set_unknown_16(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  float unknown_16_4f() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16_4f(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_4f_address() const { return address + 16; }
  uint8_t unknown_17() const { return guest::LoadU8(base, address + 17); }
  void set_unknown_17(uint8_t v) const { guest::StoreU8(base, address + 17, v); }
  uint32_t unknown_17_address() const { return address + 17; }
  uint8_t unknown_18() const { return guest::LoadU8(base, address + 18); }
  void set_unknown_18(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t unknown_18_address() const { return address + 18; }
  uint8_t unknown_19() const { return guest::LoadU8(base, address + 19); }
  void set_unknown_19(uint8_t v) const { guest::StoreU8(base, address + 19, v); }
  uint32_t unknown_19_address() const { return address + 19; }
  uint8_t unknown_20() const { return guest::LoadU8(base, address + 20); }
  void set_unknown_20(uint8_t v) const { guest::StoreU8(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  float unknown_20_4f() const { return guest::LoadFloat(base, address + 20); }
  void set_unknown_20_4f(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t unknown_20_4f_address() const { return address + 20; }
  uint8_t unknown_21() const { return guest::LoadU8(base, address + 21); }
  void set_unknown_21(uint8_t v) const { guest::StoreU8(base, address + 21, v); }
  uint32_t unknown_21_address() const { return address + 21; }
  uint8_t unknown_22() const { return guest::LoadU8(base, address + 22); }
  void set_unknown_22(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t unknown_22_address() const { return address + 22; }
  uint8_t unknown_23() const { return guest::LoadU8(base, address + 23); }
  void set_unknown_23(uint8_t v) const { guest::StoreU8(base, address + 23, v); }
  uint32_t unknown_23_address() const { return address + 23; }
  uint8_t unknown_24() const { return guest::LoadU8(base, address + 24); }
  void set_unknown_24(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  float unknown_24_4f() const { return guest::LoadFloat(base, address + 24); }
  void set_unknown_24_4f(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t unknown_24_4f_address() const { return address + 24; }
  uint8_t unknown_25() const { return guest::LoadU8(base, address + 25); }
  void set_unknown_25(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t unknown_25_address() const { return address + 25; }
  uint8_t unknown_26() const { return guest::LoadU8(base, address + 26); }
  void set_unknown_26(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t unknown_26_address() const { return address + 26; }
  uint8_t unknown_27() const { return guest::LoadU8(base, address + 27); }
  void set_unknown_27(uint8_t v) const { guest::StoreU8(base, address + 27, v); }
  uint32_t unknown_27_address() const { return address + 27; }
  uint8_t unknown_28() const { return guest::LoadU8(base, address + 28); }
  void set_unknown_28(uint8_t v) const { guest::StoreU8(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  float unknown_28_4f() const { return guest::LoadFloat(base, address + 28); }
  void set_unknown_28_4f(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t unknown_28_4f_address() const { return address + 28; }
  uint8_t unknown_29() const { return guest::LoadU8(base, address + 29); }
  void set_unknown_29(uint8_t v) const { guest::StoreU8(base, address + 29, v); }
  uint32_t unknown_29_address() const { return address + 29; }
  uint8_t unknown_30() const { return guest::LoadU8(base, address + 30); }
  void set_unknown_30(uint8_t v) const { guest::StoreU8(base, address + 30, v); }
  uint32_t unknown_30_address() const { return address + 30; }
  uint8_t unknown_31() const { return guest::LoadU8(base, address + 31); }
  void set_unknown_31(uint8_t v) const { guest::StoreU8(base, address + 31, v); }
  uint32_t unknown_31_address() const { return address + 31; }
  uint8_t unknown_32() const { return guest::LoadU8(base, address + 32); }
  void set_unknown_32(uint8_t v) const { guest::StoreU8(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  float unknown_32_4f() const { return guest::LoadFloat(base, address + 32); }
  void set_unknown_32_4f(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t unknown_32_4f_address() const { return address + 32; }
  uint8_t unknown_33() const { return guest::LoadU8(base, address + 33); }
  void set_unknown_33(uint8_t v) const { guest::StoreU8(base, address + 33, v); }
  uint32_t unknown_33_address() const { return address + 33; }
  uint8_t unknown_34() const { return guest::LoadU8(base, address + 34); }
  void set_unknown_34(uint8_t v) const { guest::StoreU8(base, address + 34, v); }
  uint32_t unknown_34_address() const { return address + 34; }
  uint8_t unknown_35() const { return guest::LoadU8(base, address + 35); }
  void set_unknown_35(uint8_t v) const { guest::StoreU8(base, address + 35, v); }
  uint32_t unknown_35_address() const { return address + 35; }
  uint8_t unknown_36() const { return guest::LoadU8(base, address + 36); }
  void set_unknown_36(uint8_t v) const { guest::StoreU8(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  float unknown_36_4f() const { return guest::LoadFloat(base, address + 36); }
  void set_unknown_36_4f(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t unknown_36_4f_address() const { return address + 36; }
  uint8_t unknown_37() const { return guest::LoadU8(base, address + 37); }
  void set_unknown_37(uint8_t v) const { guest::StoreU8(base, address + 37, v); }
  uint32_t unknown_37_address() const { return address + 37; }
  uint8_t unknown_38() const { return guest::LoadU8(base, address + 38); }
  void set_unknown_38(uint8_t v) const { guest::StoreU8(base, address + 38, v); }
  uint32_t unknown_38_address() const { return address + 38; }
  uint8_t unknown_39() const { return guest::LoadU8(base, address + 39); }
  void set_unknown_39(uint8_t v) const { guest::StoreU8(base, address + 39, v); }
  uint32_t unknown_39_address() const { return address + 39; }
  uint8_t unknown_40() const { return guest::LoadU8(base, address + 40); }
  void set_unknown_40(uint8_t v) const { guest::StoreU8(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  float unknown_40_4f() const { return guest::LoadFloat(base, address + 40); }
  void set_unknown_40_4f(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t unknown_40_4f_address() const { return address + 40; }
  uint8_t unknown_41() const { return guest::LoadU8(base, address + 41); }
  void set_unknown_41(uint8_t v) const { guest::StoreU8(base, address + 41, v); }
  uint32_t unknown_41_address() const { return address + 41; }
  uint8_t unknown_42() const { return guest::LoadU8(base, address + 42); }
  void set_unknown_42(uint8_t v) const { guest::StoreU8(base, address + 42, v); }
  uint32_t unknown_42_address() const { return address + 42; }
  uint8_t unknown_43() const { return guest::LoadU8(base, address + 43); }
  void set_unknown_43(uint8_t v) const { guest::StoreU8(base, address + 43, v); }
  uint32_t unknown_43_address() const { return address + 43; }
  uint8_t unknown_44() const { return guest::LoadU8(base, address + 44); }
  void set_unknown_44(uint8_t v) const { guest::StoreU8(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  float unknown_44_4f() const { return guest::LoadFloat(base, address + 44); }
  void set_unknown_44_4f(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t unknown_44_4f_address() const { return address + 44; }
  uint8_t unknown_45() const { return guest::LoadU8(base, address + 45); }
  void set_unknown_45(uint8_t v) const { guest::StoreU8(base, address + 45, v); }
  uint32_t unknown_45_address() const { return address + 45; }
  uint8_t unknown_46() const { return guest::LoadU8(base, address + 46); }
  void set_unknown_46(uint8_t v) const { guest::StoreU8(base, address + 46, v); }
  uint32_t unknown_46_address() const { return address + 46; }
  uint8_t unknown_47() const { return guest::LoadU8(base, address + 47); }
  void set_unknown_47(uint8_t v) const { guest::StoreU8(base, address + 47, v); }
  uint32_t unknown_47_address() const { return address + 47; }
  uint8_t unknown_48() const { return guest::LoadU8(base, address + 48); }
  void set_unknown_48(uint8_t v) const { guest::StoreU8(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  float unknown_48_4f() const { return guest::LoadFloat(base, address + 48); }
  void set_unknown_48_4f(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t unknown_48_4f_address() const { return address + 48; }
  uint8_t unknown_49() const { return guest::LoadU8(base, address + 49); }
  void set_unknown_49(uint8_t v) const { guest::StoreU8(base, address + 49, v); }
  uint32_t unknown_49_address() const { return address + 49; }
  uint8_t unknown_50() const { return guest::LoadU8(base, address + 50); }
  void set_unknown_50(uint8_t v) const { guest::StoreU8(base, address + 50, v); }
  uint32_t unknown_50_address() const { return address + 50; }
  uint8_t unknown_51() const { return guest::LoadU8(base, address + 51); }
  void set_unknown_51(uint8_t v) const { guest::StoreU8(base, address + 51, v); }
  uint32_t unknown_51_address() const { return address + 51; }
  uint8_t unknown_52() const { return guest::LoadU8(base, address + 52); }
  void set_unknown_52(uint8_t v) const { guest::StoreU8(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  float unknown_52_4f() const { return guest::LoadFloat(base, address + 52); }
  void set_unknown_52_4f(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t unknown_52_4f_address() const { return address + 52; }
  uint8_t unknown_53() const { return guest::LoadU8(base, address + 53); }
  void set_unknown_53(uint8_t v) const { guest::StoreU8(base, address + 53, v); }
  uint32_t unknown_53_address() const { return address + 53; }
  uint8_t unknown_54() const { return guest::LoadU8(base, address + 54); }
  void set_unknown_54(uint8_t v) const { guest::StoreU8(base, address + 54, v); }
  uint32_t unknown_54_address() const { return address + 54; }
  uint8_t unknown_55() const { return guest::LoadU8(base, address + 55); }
  void set_unknown_55(uint8_t v) const { guest::StoreU8(base, address + 55, v); }
  uint32_t unknown_55_address() const { return address + 55; }
  uint8_t unknown_56() const { return guest::LoadU8(base, address + 56); }
  void set_unknown_56(uint8_t v) const { guest::StoreU8(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  float unknown_56_4f() const { return guest::LoadFloat(base, address + 56); }
  void set_unknown_56_4f(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t unknown_56_4f_address() const { return address + 56; }
  uint8_t unknown_57() const { return guest::LoadU8(base, address + 57); }
  void set_unknown_57(uint8_t v) const { guest::StoreU8(base, address + 57, v); }
  uint32_t unknown_57_address() const { return address + 57; }
  uint8_t unknown_58() const { return guest::LoadU8(base, address + 58); }
  void set_unknown_58(uint8_t v) const { guest::StoreU8(base, address + 58, v); }
  uint32_t unknown_58_address() const { return address + 58; }
  uint8_t unknown_59() const { return guest::LoadU8(base, address + 59); }
  void set_unknown_59(uint8_t v) const { guest::StoreU8(base, address + 59, v); }
  uint32_t unknown_59_address() const { return address + 59; }
  uint8_t unknown_60() const { return guest::LoadU8(base, address + 60); }
  void set_unknown_60(uint8_t v) const { guest::StoreU8(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  float unknown_60_4f() const { return guest::LoadFloat(base, address + 60); }
  void set_unknown_60_4f(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t unknown_60_4f_address() const { return address + 60; }
  uint8_t unknown_61() const { return guest::LoadU8(base, address + 61); }
  void set_unknown_61(uint8_t v) const { guest::StoreU8(base, address + 61, v); }
  uint32_t unknown_61_address() const { return address + 61; }
  uint8_t unknown_62() const { return guest::LoadU8(base, address + 62); }
  void set_unknown_62(uint8_t v) const { guest::StoreU8(base, address + 62, v); }
  uint32_t unknown_62_address() const { return address + 62; }
  uint8_t unknown_63() const { return guest::LoadU8(base, address + 63); }
  void set_unknown_63(uint8_t v) const { guest::StoreU8(base, address + 63, v); }
  uint32_t unknown_63_address() const { return address + 63; }
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
