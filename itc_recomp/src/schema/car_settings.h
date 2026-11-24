// The views of docs/functions/car-settings.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-settings.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CarSettings
struct CarSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 3 is loaded (WaitForMovie)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForMovie go to state 9 (open on its message)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // as the type 2 mode's other machines (delete the movie on unload)
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// embedded; measure: CarSettings::Close.r4, CarSettings::Leave.r4
#pragma pack(push, 1)
struct CarSettingsModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(CarSettingsModeData) == 812);
inline constexpr memory::LayoutField kCarSettingsModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kCarSettingsModeLayout = {kCarSettingsModeFields, 2};

struct CarSettingsMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  CarSettingsModeData* data() const { return static_cast<CarSettingsModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static CarSettingsMode New(uint8_t* base) {
    return CarSettingsMode(base, memory::MakeHandle(new CarSettingsModeData(), sizeof(CarSettingsModeData), "CarSettingsMode", &kCarSettingsModeLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSettingsMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarSettingsModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSettingsModeData*>(::operator new(sizeof(CarSettingsModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSettingsModeData();
    return CarSettingsMode(base, memory::MakeHandle(p, sizeof(CarSettingsModeData) * count, "CarSettingsMode", &kCarSettingsModeLayout, sizeof(CarSettingsModeData)));
  }
  CarSettingsMode at(uint32_t i) const { return CarSettingsMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarSettingsModeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarSettingsModeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 204 : guest::Pointer(base, address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count() const { return on_host() ? uint32_t(data()->message_count) : uint32_t(guest::LoadU32(base, address + 804)); }
  void set_message_count(uint32_t v) const { if (on_host()) data()->message_count = v; else guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_bits() const { return on_host() ? data()->message_count : guest::LoadU32(base, address + 804); }
  void set_message_count_bits(uint32_t v) const { if (on_host()) data()->message_count = v; else guest::StoreU32(base, address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start() const { return on_host() ? uint32_t(data()->message_start) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_message_start(uint32_t v) const { if (on_host()) data()->message_start = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t message_start_bits() const { return on_host() ? data()->message_start : guest::LoadU32(base, address + 808); }
  void set_message_start_bits(uint32_t v) const { if (on_host()) data()->message_start = v; else guest::StoreU32(base, address + 808, v); }
};

// allocated; measure: new<764>@CarSettings::Create
#pragma pack(push, 1)
struct CarSettingsDataData {
  float working_gear_1 = 0;  // +0
  float working_gear_2 = 0;  // +4
  float working_gear_3 = 0;  // +8
  float working_gear_4 = 0;  // +12
  float working_gear_5 = 0;  // +16
  float working_gear_6 = 0;  // +20
  float working_final_drive = 0;  // +24
  uint32_t working_setting_72 = 0;  // +28
  uint32_t working_setting_73 = 0;  // +32
  uint32_t working_setting_74 = 0;  // +36
  uint32_t working_setting_75 = 0;  // +40
  uint32_t working_setting_80 = 0;  // +44
  uint32_t working_setting_81 = 0;  // +48
  uint32_t working_setting_78 = 0;  // +52
  uint32_t working_setting_79 = 0;  // +56
  uint32_t working_setting_82 = 0;  // +60
  uint32_t working_setting_83 = 0;  // +64
  uint32_t working_setting_84 = 0;  // +68
  uint32_t working_setting_85 = 0;  // +72
  uint32_t working_setting_86 = 0;  // +76
  uint32_t working_setting_87 = 0;  // +80
  uint32_t working_setting_88 = 0;  // +84
  uint32_t working_setting_89 = 0;  // +88
  uint32_t working_setting_94 = 0;  // +92
  uint32_t working_torque_share = 0;  // +96
  uint32_t working_setting_90 = 0;  // +100
  uint32_t working_setting_91 = 0;  // +104
  uint32_t working_setting_76 = 0;  // +108
  uint32_t working_setting_77 = 0;  // +112
  float applied_gear_1 = 0;  // +116
  float applied_gear_2 = 0;  // +120
  float applied_gear_3 = 0;  // +124
  float applied_gear_4 = 0;  // +128
  float applied_gear_5 = 0;  // +132
  float applied_gear_6 = 0;  // +136
  float applied_final_drive = 0;  // +140
  uint32_t applied_setting_72 = 0;  // +144
  uint32_t applied_setting_73 = 0;  // +148
  uint32_t applied_setting_74 = 0;  // +152
  uint32_t applied_setting_75 = 0;  // +156
  uint32_t applied_setting_80 = 0;  // +160
  uint32_t applied_setting_81 = 0;  // +164
  uint32_t applied_setting_78 = 0;  // +168
  uint32_t applied_setting_79 = 0;  // +172
  uint32_t applied_setting_82 = 0;  // +176
  uint32_t applied_setting_83 = 0;  // +180
  uint32_t applied_setting_84 = 0;  // +184
  uint32_t applied_setting_85 = 0;  // +188
  uint32_t applied_setting_86 = 0;  // +192
  uint32_t applied_setting_87 = 0;  // +196
  uint32_t applied_setting_88 = 0;  // +200
  uint32_t applied_setting_89 = 0;  // +204
  uint32_t applied_setting_94 = 0;  // +208
  uint32_t applied_torque_share = 0;  // +212
  uint32_t applied_setting_90 = 0;  // +216
  uint32_t applied_setting_91 = 0;  // +220
  uint32_t applied_setting_76 = 0;  // +224
  uint32_t applied_setting_77 = 0;  // +228
  float stock_gear_1 = 0;  // +232
  float stock_gear_2 = 0;  // +236
  float stock_gear_3 = 0;  // +240
  float stock_gear_4 = 0;  // +244
  float stock_gear_5 = 0;  // +248
  float stock_gear_6 = 0;  // +252
  float stock_final_drive = 0;  // +256
  uint32_t stock_setting_72 = 0;  // +260
  uint32_t stock_setting_73 = 0;  // +264
  uint32_t stock_setting_74 = 0;  // +268
  uint32_t stock_setting_75 = 0;  // +272
  uint32_t stock_setting_80 = 0;  // +276
  uint32_t stock_setting_81 = 0;  // +280
  uint32_t stock_setting_78 = 0;  // +284
  uint32_t stock_setting_79 = 0;  // +288
  uint32_t stock_setting_82 = 0;  // +292
  uint32_t stock_setting_83 = 0;  // +296
  uint32_t stock_setting_84 = 0;  // +300
  uint32_t stock_setting_85 = 0;  // +304
  uint32_t stock_setting_86 = 0;  // +308
  uint32_t stock_setting_87 = 0;  // +312
  uint32_t stock_setting_88 = 0;  // +316
  uint32_t stock_setting_89 = 0;  // +320
  uint32_t stock_setting_94 = 0;  // +324
  uint32_t stock_torque_share = 0;  // +328
  uint32_t stock_setting_90 = 0;  // +332
  uint32_t stock_setting_91 = 0;  // +336
  uint32_t stock_setting_76 = 0;  // +340
  uint32_t stock_setting_77 = 0;  // +344
  uint32_t route8_time = 0;  // +348
  uint32_t route8_entry_0 = 0;  // +352
  uint32_t route8_present = 0;  // +356
  uint8_t route8_rows[48] = {};  // +360, guest order
  uint32_t route9_time = 0;  // +408
  uint32_t route9_entry_0 = 0;  // +412
  uint32_t route9_present = 0;  // +416
  uint8_t route9_rows[48] = {};  // +420, guest order
  uint32_t route10_time = 0;  // +468
  uint32_t route10_entry_0 = 0;  // +472
  uint32_t route10_present = 0;  // +476
  uint8_t route10_rows[48] = {};  // +480, guest order
  uint8_t player_name[20] = {};  // +528, guest order
  uint32_t money = 0;  // +548
  uint32_t weekday = 0;  // +552
  uint32_t day = 0;  // +556
  uint32_t type5_mode = 0;  // +560
  uint32_t top_items = 0;  // +564
  uint32_t course = 0;  // +568
  uint32_t dialog_answer = 0;  // +572
  uint32_t test_run_set_up = 0;  // +576
  uint32_t start_at_courses = 0;  // +580
  uint32_t allows_gears = 0;  // +584
  uint32_t allows_pages_9_10 = 0;  // +588
  uint32_t allows_pages_11_16 = 0;  // +592
  uint32_t allows_page_17 = 0;  // +596
  uint32_t spec_block2_100 = 0;  // +600
  uint8_t gear_bounds[84] = {};  // +604, guest order
  uint32_t drive_type = 0;  // +688
  uint32_t tuning_52 = 0;  // +692
  float car_data_388 = 0;  // +696
  float car_data_1280 = 0;  // +700
  uint32_t unknown_704 = 0;  // +704
  uint8_t stack[20] = {};  // +708, guest order
  uint32_t stack_count = 0;  // +728
  uint32_t apply_choice = 0;  // +732
  uint32_t previous_apply_choice = 0;  // +736
  uint32_t unknown_740 = 0;  // +740
  uint32_t unknown_744 = 0;  // +744
  uint32_t unknown_748 = 0;  // +748
  uint32_t location = 0;  // +752
  uint32_t unknown_756 = 0;  // +756
  uint32_t unknown_760 = 0;  // +760
};
#pragma pack(pop)
static_assert(sizeof(CarSettingsDataData) == 764);
inline constexpr memory::LayoutField kCarSettingsDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {176, 4}, {180, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {220, 4}, {224, 4}, {228, 4}, {232, 4}, {236, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}, {256, 4}, {260, 4}, {264, 4}, {268, 4}, {272, 4}, {276, 4}, {280, 4}, {284, 4}, {288, 4}, {292, 4}, {296, 4}, {300, 4}, {304, 4}, {308, 4}, {312, 4}, {316, 4}, {320, 4}, {324, 4}, {328, 4}, {332, 4}, {336, 4}, {340, 4}, {344, 4}, {348, 4}, {352, 4}, {356, 4}, {408, 4}, {412, 4}, {416, 4}, {468, 4}, {472, 4}, {476, 4}, {548, 4}, {552, 4}, {556, 4}, {560, 4}, {564, 4}, {568, 4}, {572, 4}, {576, 4}, {580, 4}, {584, 4}, {588, 4}, {592, 4}, {596, 4}, {600, 4}, {688, 4}, {692, 4}, {696, 4}, {700, 4}, {704, 4}, {728, 4}, {732, 4}, {736, 4}, {740, 4}, {744, 4}, {748, 4}, {752, 4}, {756, 4}, {760, 4}};
inline constexpr memory::Layout kCarSettingsDataLayout = {kCarSettingsDataFields, 124};

struct CarSettingsData : View {
  using View::View;
  static constexpr uint32_t kSize = 764;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSettingsDataData* data() const { return static_cast<CarSettingsDataData*>(memory::HandleObject(address)); }
  static CarSettingsData New(uint8_t* base) {
    return CarSettingsData(base, memory::MakeHandle(new CarSettingsDataData(), sizeof(CarSettingsDataData), "CarSettingsData", &kCarSettingsDataLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSettingsData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarSettingsDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSettingsDataData*>(::operator new(sizeof(CarSettingsDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSettingsDataData();
    return CarSettingsData(base, memory::MakeHandle(p, sizeof(CarSettingsDataData) * count, "CarSettingsData", &kCarSettingsDataLayout, sizeof(CarSettingsDataData)));
  }
  CarSettingsData at(uint32_t i) const { return CarSettingsData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarSettingsDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarSettingsDataLayout, offset, width, v); }
  // car +40
  uint32_t working_gear_1_address() const { return address + 0; }
  float working_gear_1() const { return data()->working_gear_1; }
  void set_working_gear_1(float v) const { data()->working_gear_1 = v; }
  uint32_t working_gear_1_bits() const { return std::bit_cast<uint32_t>(data()->working_gear_1); }
  void set_working_gear_1_bits(uint32_t v) const { data()->working_gear_1 = std::bit_cast<float>(v); }
  // car +44
  uint32_t working_gear_2_address() const { return address + 4; }
  float working_gear_2() const { return data()->working_gear_2; }
  void set_working_gear_2(float v) const { data()->working_gear_2 = v; }
  uint32_t working_gear_2_bits() const { return std::bit_cast<uint32_t>(data()->working_gear_2); }
  void set_working_gear_2_bits(uint32_t v) const { data()->working_gear_2 = std::bit_cast<float>(v); }
  // car +48
  uint32_t working_gear_3_address() const { return address + 8; }
  float working_gear_3() const { return data()->working_gear_3; }
  void set_working_gear_3(float v) const { data()->working_gear_3 = v; }
  uint32_t working_gear_3_bits() const { return std::bit_cast<uint32_t>(data()->working_gear_3); }
  void set_working_gear_3_bits(uint32_t v) const { data()->working_gear_3 = std::bit_cast<float>(v); }
  // car +52
  uint32_t working_gear_4_address() const { return address + 12; }
  float working_gear_4() const { return data()->working_gear_4; }
  void set_working_gear_4(float v) const { data()->working_gear_4 = v; }
  uint32_t working_gear_4_bits() const { return std::bit_cast<uint32_t>(data()->working_gear_4); }
  void set_working_gear_4_bits(uint32_t v) const { data()->working_gear_4 = std::bit_cast<float>(v); }
  // car +56
  uint32_t working_gear_5_address() const { return address + 16; }
  float working_gear_5() const { return data()->working_gear_5; }
  void set_working_gear_5(float v) const { data()->working_gear_5 = v; }
  uint32_t working_gear_5_bits() const { return std::bit_cast<uint32_t>(data()->working_gear_5); }
  void set_working_gear_5_bits(uint32_t v) const { data()->working_gear_5 = std::bit_cast<float>(v); }
  // car +60
  uint32_t working_gear_6_address() const { return address + 20; }
  float working_gear_6() const { return data()->working_gear_6; }
  void set_working_gear_6(float v) const { data()->working_gear_6 = v; }
  uint32_t working_gear_6_bits() const { return std::bit_cast<uint32_t>(data()->working_gear_6); }
  void set_working_gear_6_bits(uint32_t v) const { data()->working_gear_6 = std::bit_cast<float>(v); }
  // car +36
  uint32_t working_final_drive_address() const { return address + 24; }
  float working_final_drive() const { return data()->working_final_drive; }
  void set_working_final_drive(float v) const { data()->working_final_drive = v; }
  uint32_t working_final_drive_bits() const { return std::bit_cast<uint32_t>(data()->working_final_drive); }
  void set_working_final_drive_bits(uint32_t v) const { data()->working_final_drive = std::bit_cast<float>(v); }
  // car byte +72
  uint32_t working_setting_72_address() const { return address + 28; }
  uint32_t working_setting_72() const { return data()->working_setting_72; }
  void set_working_setting_72(uint32_t v) const { data()->working_setting_72 = v; }
  uint32_t working_setting_72_bits() const { return data()->working_setting_72; }
  void set_working_setting_72_bits(uint32_t v) const { data()->working_setting_72 = v; }
  // car byte +73
  uint32_t working_setting_73_address() const { return address + 32; }
  uint32_t working_setting_73() const { return data()->working_setting_73; }
  void set_working_setting_73(uint32_t v) const { data()->working_setting_73 = v; }
  uint32_t working_setting_73_bits() const { return data()->working_setting_73; }
  void set_working_setting_73_bits(uint32_t v) const { data()->working_setting_73 = v; }
  // car byte +74
  uint32_t working_setting_74_address() const { return address + 36; }
  uint32_t working_setting_74() const { return data()->working_setting_74; }
  void set_working_setting_74(uint32_t v) const { data()->working_setting_74 = v; }
  uint32_t working_setting_74_bits() const { return data()->working_setting_74; }
  void set_working_setting_74_bits(uint32_t v) const { data()->working_setting_74 = v; }
  // car byte +75
  uint32_t working_setting_75_address() const { return address + 40; }
  uint32_t working_setting_75() const { return data()->working_setting_75; }
  void set_working_setting_75(uint32_t v) const { data()->working_setting_75 = v; }
  uint32_t working_setting_75_bits() const { return data()->working_setting_75; }
  void set_working_setting_75_bits(uint32_t v) const { data()->working_setting_75 = v; }
  // car byte +80 (probably the front camber)
  uint32_t working_setting_80_address() const { return address + 44; }
  uint32_t working_setting_80() const { return data()->working_setting_80; }
  void set_working_setting_80(uint32_t v) const { data()->working_setting_80 = v; }
  uint32_t working_setting_80_bits() const { return data()->working_setting_80; }
  void set_working_setting_80_bits(uint32_t v) const { data()->working_setting_80 = v; }
  // car byte +81 (probably the rear camber)
  uint32_t working_setting_81_address() const { return address + 48; }
  uint32_t working_setting_81() const { return data()->working_setting_81; }
  void set_working_setting_81(uint32_t v) const { data()->working_setting_81 = v; }
  uint32_t working_setting_81_bits() const { return data()->working_setting_81; }
  void set_working_setting_81_bits(uint32_t v) const { data()->working_setting_81 = v; }
  // car byte +78
  uint32_t working_setting_78_address() const { return address + 52; }
  uint32_t working_setting_78() const { return data()->working_setting_78; }
  void set_working_setting_78(uint32_t v) const { data()->working_setting_78 = v; }
  uint32_t working_setting_78_bits() const { return data()->working_setting_78; }
  void set_working_setting_78_bits(uint32_t v) const { data()->working_setting_78 = v; }
  // car byte +79
  uint32_t working_setting_79_address() const { return address + 56; }
  uint32_t working_setting_79() const { return data()->working_setting_79; }
  void set_working_setting_79(uint32_t v) const { data()->working_setting_79 = v; }
  uint32_t working_setting_79_bits() const { return data()->working_setting_79; }
  void set_working_setting_79_bits(uint32_t v) const { data()->working_setting_79 = v; }
  // car byte +82
  uint32_t working_setting_82_address() const { return address + 60; }
  uint32_t working_setting_82() const { return data()->working_setting_82; }
  void set_working_setting_82(uint32_t v) const { data()->working_setting_82 = v; }
  uint32_t working_setting_82_bits() const { return data()->working_setting_82; }
  void set_working_setting_82_bits(uint32_t v) const { data()->working_setting_82 = v; }
  // car byte +83
  uint32_t working_setting_83_address() const { return address + 64; }
  uint32_t working_setting_83() const { return data()->working_setting_83; }
  void set_working_setting_83(uint32_t v) const { data()->working_setting_83 = v; }
  uint32_t working_setting_83_bits() const { return data()->working_setting_83; }
  void set_working_setting_83_bits(uint32_t v) const { data()->working_setting_83 = v; }
  // car byte +84
  uint32_t working_setting_84_address() const { return address + 68; }
  uint32_t working_setting_84() const { return data()->working_setting_84; }
  void set_working_setting_84(uint32_t v) const { data()->working_setting_84 = v; }
  uint32_t working_setting_84_bits() const { return data()->working_setting_84; }
  void set_working_setting_84_bits(uint32_t v) const { data()->working_setting_84 = v; }
  // car byte +85
  uint32_t working_setting_85_address() const { return address + 72; }
  uint32_t working_setting_85() const { return data()->working_setting_85; }
  void set_working_setting_85(uint32_t v) const { data()->working_setting_85 = v; }
  uint32_t working_setting_85_bits() const { return data()->working_setting_85; }
  void set_working_setting_85_bits(uint32_t v) const { data()->working_setting_85 = v; }
  // car byte +86
  uint32_t working_setting_86_address() const { return address + 76; }
  uint32_t working_setting_86() const { return data()->working_setting_86; }
  void set_working_setting_86(uint32_t v) const { data()->working_setting_86 = v; }
  uint32_t working_setting_86_bits() const { return data()->working_setting_86; }
  void set_working_setting_86_bits(uint32_t v) const { data()->working_setting_86 = v; }
  // car byte +87
  uint32_t working_setting_87_address() const { return address + 80; }
  uint32_t working_setting_87() const { return data()->working_setting_87; }
  void set_working_setting_87(uint32_t v) const { data()->working_setting_87 = v; }
  uint32_t working_setting_87_bits() const { return data()->working_setting_87; }
  void set_working_setting_87_bits(uint32_t v) const { data()->working_setting_87 = v; }
  // car byte +88
  uint32_t working_setting_88_address() const { return address + 84; }
  uint32_t working_setting_88() const { return data()->working_setting_88; }
  void set_working_setting_88(uint32_t v) const { data()->working_setting_88 = v; }
  uint32_t working_setting_88_bits() const { return data()->working_setting_88; }
  void set_working_setting_88_bits(uint32_t v) const { data()->working_setting_88 = v; }
  // car byte +89
  uint32_t working_setting_89_address() const { return address + 88; }
  uint32_t working_setting_89() const { return data()->working_setting_89; }
  void set_working_setting_89(uint32_t v) const { data()->working_setting_89 = v; }
  uint32_t working_setting_89_bits() const { return data()->working_setting_89; }
  void set_working_setting_89_bits(uint32_t v) const { data()->working_setting_89 = v; }
  // car byte +94
  uint32_t working_setting_94_address() const { return address + 92; }
  uint32_t working_setting_94() const { return data()->working_setting_94; }
  void set_working_setting_94(uint32_t v) const { data()->working_setting_94 = v; }
  uint32_t working_setting_94_bits() const { return data()->working_setting_94; }
  void set_working_setting_94_bits(uint32_t v) const { data()->working_setting_94 = v; }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t working_torque_share_address() const { return address + 96; }
  uint32_t working_torque_share() const { return data()->working_torque_share; }
  void set_working_torque_share(uint32_t v) const { data()->working_torque_share = v; }
  uint32_t working_torque_share_bits() const { return data()->working_torque_share; }
  void set_working_torque_share_bits(uint32_t v) const { data()->working_torque_share = v; }
  // car byte +90
  uint32_t working_setting_90_address() const { return address + 100; }
  uint32_t working_setting_90() const { return data()->working_setting_90; }
  void set_working_setting_90(uint32_t v) const { data()->working_setting_90 = v; }
  uint32_t working_setting_90_bits() const { return data()->working_setting_90; }
  void set_working_setting_90_bits(uint32_t v) const { data()->working_setting_90 = v; }
  // car byte +91
  uint32_t working_setting_91_address() const { return address + 104; }
  uint32_t working_setting_91() const { return data()->working_setting_91; }
  void set_working_setting_91(uint32_t v) const { data()->working_setting_91 = v; }
  uint32_t working_setting_91_bits() const { return data()->working_setting_91; }
  void set_working_setting_91_bits(uint32_t v) const { data()->working_setting_91 = v; }
  // car byte +76
  uint32_t working_setting_76_address() const { return address + 108; }
  uint32_t working_setting_76() const { return data()->working_setting_76; }
  void set_working_setting_76(uint32_t v) const { data()->working_setting_76 = v; }
  uint32_t working_setting_76_bits() const { return data()->working_setting_76; }
  void set_working_setting_76_bits(uint32_t v) const { data()->working_setting_76 = v; }
  // car byte +77
  uint32_t working_setting_77_address() const { return address + 112; }
  uint32_t working_setting_77() const { return data()->working_setting_77; }
  void set_working_setting_77(uint32_t v) const { data()->working_setting_77 = v; }
  uint32_t working_setting_77_bits() const { return data()->working_setting_77; }
  void set_working_setting_77_bits(uint32_t v) const { data()->working_setting_77 = v; }
  // car +40
  uint32_t applied_gear_1_address() const { return address + 116; }
  float applied_gear_1() const { return data()->applied_gear_1; }
  void set_applied_gear_1(float v) const { data()->applied_gear_1 = v; }
  uint32_t applied_gear_1_bits() const { return std::bit_cast<uint32_t>(data()->applied_gear_1); }
  void set_applied_gear_1_bits(uint32_t v) const { data()->applied_gear_1 = std::bit_cast<float>(v); }
  // car +44
  uint32_t applied_gear_2_address() const { return address + 120; }
  float applied_gear_2() const { return data()->applied_gear_2; }
  void set_applied_gear_2(float v) const { data()->applied_gear_2 = v; }
  uint32_t applied_gear_2_bits() const { return std::bit_cast<uint32_t>(data()->applied_gear_2); }
  void set_applied_gear_2_bits(uint32_t v) const { data()->applied_gear_2 = std::bit_cast<float>(v); }
  // car +48
  uint32_t applied_gear_3_address() const { return address + 124; }
  float applied_gear_3() const { return data()->applied_gear_3; }
  void set_applied_gear_3(float v) const { data()->applied_gear_3 = v; }
  uint32_t applied_gear_3_bits() const { return std::bit_cast<uint32_t>(data()->applied_gear_3); }
  void set_applied_gear_3_bits(uint32_t v) const { data()->applied_gear_3 = std::bit_cast<float>(v); }
  // car +52
  uint32_t applied_gear_4_address() const { return address + 128; }
  float applied_gear_4() const { return data()->applied_gear_4; }
  void set_applied_gear_4(float v) const { data()->applied_gear_4 = v; }
  uint32_t applied_gear_4_bits() const { return std::bit_cast<uint32_t>(data()->applied_gear_4); }
  void set_applied_gear_4_bits(uint32_t v) const { data()->applied_gear_4 = std::bit_cast<float>(v); }
  // car +56
  uint32_t applied_gear_5_address() const { return address + 132; }
  float applied_gear_5() const { return data()->applied_gear_5; }
  void set_applied_gear_5(float v) const { data()->applied_gear_5 = v; }
  uint32_t applied_gear_5_bits() const { return std::bit_cast<uint32_t>(data()->applied_gear_5); }
  void set_applied_gear_5_bits(uint32_t v) const { data()->applied_gear_5 = std::bit_cast<float>(v); }
  // car +60
  uint32_t applied_gear_6_address() const { return address + 136; }
  float applied_gear_6() const { return data()->applied_gear_6; }
  void set_applied_gear_6(float v) const { data()->applied_gear_6 = v; }
  uint32_t applied_gear_6_bits() const { return std::bit_cast<uint32_t>(data()->applied_gear_6); }
  void set_applied_gear_6_bits(uint32_t v) const { data()->applied_gear_6 = std::bit_cast<float>(v); }
  // car +36
  uint32_t applied_final_drive_address() const { return address + 140; }
  float applied_final_drive() const { return data()->applied_final_drive; }
  void set_applied_final_drive(float v) const { data()->applied_final_drive = v; }
  uint32_t applied_final_drive_bits() const { return std::bit_cast<uint32_t>(data()->applied_final_drive); }
  void set_applied_final_drive_bits(uint32_t v) const { data()->applied_final_drive = std::bit_cast<float>(v); }
  // car byte +72
  uint32_t applied_setting_72_address() const { return address + 144; }
  uint32_t applied_setting_72() const { return data()->applied_setting_72; }
  void set_applied_setting_72(uint32_t v) const { data()->applied_setting_72 = v; }
  uint32_t applied_setting_72_bits() const { return data()->applied_setting_72; }
  void set_applied_setting_72_bits(uint32_t v) const { data()->applied_setting_72 = v; }
  // car byte +73
  uint32_t applied_setting_73_address() const { return address + 148; }
  uint32_t applied_setting_73() const { return data()->applied_setting_73; }
  void set_applied_setting_73(uint32_t v) const { data()->applied_setting_73 = v; }
  uint32_t applied_setting_73_bits() const { return data()->applied_setting_73; }
  void set_applied_setting_73_bits(uint32_t v) const { data()->applied_setting_73 = v; }
  // car byte +74
  uint32_t applied_setting_74_address() const { return address + 152; }
  uint32_t applied_setting_74() const { return data()->applied_setting_74; }
  void set_applied_setting_74(uint32_t v) const { data()->applied_setting_74 = v; }
  uint32_t applied_setting_74_bits() const { return data()->applied_setting_74; }
  void set_applied_setting_74_bits(uint32_t v) const { data()->applied_setting_74 = v; }
  // car byte +75
  uint32_t applied_setting_75_address() const { return address + 156; }
  uint32_t applied_setting_75() const { return data()->applied_setting_75; }
  void set_applied_setting_75(uint32_t v) const { data()->applied_setting_75 = v; }
  uint32_t applied_setting_75_bits() const { return data()->applied_setting_75; }
  void set_applied_setting_75_bits(uint32_t v) const { data()->applied_setting_75 = v; }
  // car byte +80 (probably the front camber)
  uint32_t applied_setting_80_address() const { return address + 160; }
  uint32_t applied_setting_80() const { return data()->applied_setting_80; }
  void set_applied_setting_80(uint32_t v) const { data()->applied_setting_80 = v; }
  uint32_t applied_setting_80_bits() const { return data()->applied_setting_80; }
  void set_applied_setting_80_bits(uint32_t v) const { data()->applied_setting_80 = v; }
  // car byte +81 (probably the rear camber)
  uint32_t applied_setting_81_address() const { return address + 164; }
  uint32_t applied_setting_81() const { return data()->applied_setting_81; }
  void set_applied_setting_81(uint32_t v) const { data()->applied_setting_81 = v; }
  uint32_t applied_setting_81_bits() const { return data()->applied_setting_81; }
  void set_applied_setting_81_bits(uint32_t v) const { data()->applied_setting_81 = v; }
  // car byte +78
  uint32_t applied_setting_78_address() const { return address + 168; }
  uint32_t applied_setting_78() const { return data()->applied_setting_78; }
  void set_applied_setting_78(uint32_t v) const { data()->applied_setting_78 = v; }
  uint32_t applied_setting_78_bits() const { return data()->applied_setting_78; }
  void set_applied_setting_78_bits(uint32_t v) const { data()->applied_setting_78 = v; }
  // car byte +79
  uint32_t applied_setting_79_address() const { return address + 172; }
  uint32_t applied_setting_79() const { return data()->applied_setting_79; }
  void set_applied_setting_79(uint32_t v) const { data()->applied_setting_79 = v; }
  uint32_t applied_setting_79_bits() const { return data()->applied_setting_79; }
  void set_applied_setting_79_bits(uint32_t v) const { data()->applied_setting_79 = v; }
  // car byte +82
  uint32_t applied_setting_82_address() const { return address + 176; }
  uint32_t applied_setting_82() const { return data()->applied_setting_82; }
  void set_applied_setting_82(uint32_t v) const { data()->applied_setting_82 = v; }
  uint32_t applied_setting_82_bits() const { return data()->applied_setting_82; }
  void set_applied_setting_82_bits(uint32_t v) const { data()->applied_setting_82 = v; }
  // car byte +83
  uint32_t applied_setting_83_address() const { return address + 180; }
  uint32_t applied_setting_83() const { return data()->applied_setting_83; }
  void set_applied_setting_83(uint32_t v) const { data()->applied_setting_83 = v; }
  uint32_t applied_setting_83_bits() const { return data()->applied_setting_83; }
  void set_applied_setting_83_bits(uint32_t v) const { data()->applied_setting_83 = v; }
  // car byte +84
  uint32_t applied_setting_84_address() const { return address + 184; }
  uint32_t applied_setting_84() const { return data()->applied_setting_84; }
  void set_applied_setting_84(uint32_t v) const { data()->applied_setting_84 = v; }
  uint32_t applied_setting_84_bits() const { return data()->applied_setting_84; }
  void set_applied_setting_84_bits(uint32_t v) const { data()->applied_setting_84 = v; }
  // car byte +85
  uint32_t applied_setting_85_address() const { return address + 188; }
  uint32_t applied_setting_85() const { return data()->applied_setting_85; }
  void set_applied_setting_85(uint32_t v) const { data()->applied_setting_85 = v; }
  uint32_t applied_setting_85_bits() const { return data()->applied_setting_85; }
  void set_applied_setting_85_bits(uint32_t v) const { data()->applied_setting_85 = v; }
  // car byte +86
  uint32_t applied_setting_86_address() const { return address + 192; }
  uint32_t applied_setting_86() const { return data()->applied_setting_86; }
  void set_applied_setting_86(uint32_t v) const { data()->applied_setting_86 = v; }
  uint32_t applied_setting_86_bits() const { return data()->applied_setting_86; }
  void set_applied_setting_86_bits(uint32_t v) const { data()->applied_setting_86 = v; }
  // car byte +87
  uint32_t applied_setting_87_address() const { return address + 196; }
  uint32_t applied_setting_87() const { return data()->applied_setting_87; }
  void set_applied_setting_87(uint32_t v) const { data()->applied_setting_87 = v; }
  uint32_t applied_setting_87_bits() const { return data()->applied_setting_87; }
  void set_applied_setting_87_bits(uint32_t v) const { data()->applied_setting_87 = v; }
  // car byte +88
  uint32_t applied_setting_88_address() const { return address + 200; }
  uint32_t applied_setting_88() const { return data()->applied_setting_88; }
  void set_applied_setting_88(uint32_t v) const { data()->applied_setting_88 = v; }
  uint32_t applied_setting_88_bits() const { return data()->applied_setting_88; }
  void set_applied_setting_88_bits(uint32_t v) const { data()->applied_setting_88 = v; }
  // car byte +89
  uint32_t applied_setting_89_address() const { return address + 204; }
  uint32_t applied_setting_89() const { return data()->applied_setting_89; }
  void set_applied_setting_89(uint32_t v) const { data()->applied_setting_89 = v; }
  uint32_t applied_setting_89_bits() const { return data()->applied_setting_89; }
  void set_applied_setting_89_bits(uint32_t v) const { data()->applied_setting_89 = v; }
  // car byte +94
  uint32_t applied_setting_94_address() const { return address + 208; }
  uint32_t applied_setting_94() const { return data()->applied_setting_94; }
  void set_applied_setting_94(uint32_t v) const { data()->applied_setting_94 = v; }
  uint32_t applied_setting_94_bits() const { return data()->applied_setting_94; }
  void set_applied_setting_94_bits(uint32_t v) const { data()->applied_setting_94 = v; }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t applied_torque_share_address() const { return address + 212; }
  uint32_t applied_torque_share() const { return data()->applied_torque_share; }
  void set_applied_torque_share(uint32_t v) const { data()->applied_torque_share = v; }
  uint32_t applied_torque_share_bits() const { return data()->applied_torque_share; }
  void set_applied_torque_share_bits(uint32_t v) const { data()->applied_torque_share = v; }
  // car byte +90
  uint32_t applied_setting_90_address() const { return address + 216; }
  uint32_t applied_setting_90() const { return data()->applied_setting_90; }
  void set_applied_setting_90(uint32_t v) const { data()->applied_setting_90 = v; }
  uint32_t applied_setting_90_bits() const { return data()->applied_setting_90; }
  void set_applied_setting_90_bits(uint32_t v) const { data()->applied_setting_90 = v; }
  // car byte +91
  uint32_t applied_setting_91_address() const { return address + 220; }
  uint32_t applied_setting_91() const { return data()->applied_setting_91; }
  void set_applied_setting_91(uint32_t v) const { data()->applied_setting_91 = v; }
  uint32_t applied_setting_91_bits() const { return data()->applied_setting_91; }
  void set_applied_setting_91_bits(uint32_t v) const { data()->applied_setting_91 = v; }
  // car byte +76
  uint32_t applied_setting_76_address() const { return address + 224; }
  uint32_t applied_setting_76() const { return data()->applied_setting_76; }
  void set_applied_setting_76(uint32_t v) const { data()->applied_setting_76 = v; }
  uint32_t applied_setting_76_bits() const { return data()->applied_setting_76; }
  void set_applied_setting_76_bits(uint32_t v) const { data()->applied_setting_76 = v; }
  // car byte +77
  uint32_t applied_setting_77_address() const { return address + 228; }
  uint32_t applied_setting_77() const { return data()->applied_setting_77; }
  void set_applied_setting_77(uint32_t v) const { data()->applied_setting_77 = v; }
  uint32_t applied_setting_77_bits() const { return data()->applied_setting_77; }
  void set_applied_setting_77_bits(uint32_t v) const { data()->applied_setting_77 = v; }
  // car +40
  uint32_t stock_gear_1_address() const { return address + 232; }
  float stock_gear_1() const { return data()->stock_gear_1; }
  void set_stock_gear_1(float v) const { data()->stock_gear_1 = v; }
  uint32_t stock_gear_1_bits() const { return std::bit_cast<uint32_t>(data()->stock_gear_1); }
  void set_stock_gear_1_bits(uint32_t v) const { data()->stock_gear_1 = std::bit_cast<float>(v); }
  // car +44
  uint32_t stock_gear_2_address() const { return address + 236; }
  float stock_gear_2() const { return data()->stock_gear_2; }
  void set_stock_gear_2(float v) const { data()->stock_gear_2 = v; }
  uint32_t stock_gear_2_bits() const { return std::bit_cast<uint32_t>(data()->stock_gear_2); }
  void set_stock_gear_2_bits(uint32_t v) const { data()->stock_gear_2 = std::bit_cast<float>(v); }
  // car +48
  uint32_t stock_gear_3_address() const { return address + 240; }
  float stock_gear_3() const { return data()->stock_gear_3; }
  void set_stock_gear_3(float v) const { data()->stock_gear_3 = v; }
  uint32_t stock_gear_3_bits() const { return std::bit_cast<uint32_t>(data()->stock_gear_3); }
  void set_stock_gear_3_bits(uint32_t v) const { data()->stock_gear_3 = std::bit_cast<float>(v); }
  // car +52
  uint32_t stock_gear_4_address() const { return address + 244; }
  float stock_gear_4() const { return data()->stock_gear_4; }
  void set_stock_gear_4(float v) const { data()->stock_gear_4 = v; }
  uint32_t stock_gear_4_bits() const { return std::bit_cast<uint32_t>(data()->stock_gear_4); }
  void set_stock_gear_4_bits(uint32_t v) const { data()->stock_gear_4 = std::bit_cast<float>(v); }
  // car +56
  uint32_t stock_gear_5_address() const { return address + 248; }
  float stock_gear_5() const { return data()->stock_gear_5; }
  void set_stock_gear_5(float v) const { data()->stock_gear_5 = v; }
  uint32_t stock_gear_5_bits() const { return std::bit_cast<uint32_t>(data()->stock_gear_5); }
  void set_stock_gear_5_bits(uint32_t v) const { data()->stock_gear_5 = std::bit_cast<float>(v); }
  // car +60
  uint32_t stock_gear_6_address() const { return address + 252; }
  float stock_gear_6() const { return data()->stock_gear_6; }
  void set_stock_gear_6(float v) const { data()->stock_gear_6 = v; }
  uint32_t stock_gear_6_bits() const { return std::bit_cast<uint32_t>(data()->stock_gear_6); }
  void set_stock_gear_6_bits(uint32_t v) const { data()->stock_gear_6 = std::bit_cast<float>(v); }
  // car +36
  uint32_t stock_final_drive_address() const { return address + 256; }
  float stock_final_drive() const { return data()->stock_final_drive; }
  void set_stock_final_drive(float v) const { data()->stock_final_drive = v; }
  uint32_t stock_final_drive_bits() const { return std::bit_cast<uint32_t>(data()->stock_final_drive); }
  void set_stock_final_drive_bits(uint32_t v) const { data()->stock_final_drive = std::bit_cast<float>(v); }
  // car byte +72
  uint32_t stock_setting_72_address() const { return address + 260; }
  uint32_t stock_setting_72() const { return data()->stock_setting_72; }
  void set_stock_setting_72(uint32_t v) const { data()->stock_setting_72 = v; }
  uint32_t stock_setting_72_bits() const { return data()->stock_setting_72; }
  void set_stock_setting_72_bits(uint32_t v) const { data()->stock_setting_72 = v; }
  // car byte +73
  uint32_t stock_setting_73_address() const { return address + 264; }
  uint32_t stock_setting_73() const { return data()->stock_setting_73; }
  void set_stock_setting_73(uint32_t v) const { data()->stock_setting_73 = v; }
  uint32_t stock_setting_73_bits() const { return data()->stock_setting_73; }
  void set_stock_setting_73_bits(uint32_t v) const { data()->stock_setting_73 = v; }
  // car byte +74
  uint32_t stock_setting_74_address() const { return address + 268; }
  uint32_t stock_setting_74() const { return data()->stock_setting_74; }
  void set_stock_setting_74(uint32_t v) const { data()->stock_setting_74 = v; }
  uint32_t stock_setting_74_bits() const { return data()->stock_setting_74; }
  void set_stock_setting_74_bits(uint32_t v) const { data()->stock_setting_74 = v; }
  // car byte +75
  uint32_t stock_setting_75_address() const { return address + 272; }
  uint32_t stock_setting_75() const { return data()->stock_setting_75; }
  void set_stock_setting_75(uint32_t v) const { data()->stock_setting_75 = v; }
  uint32_t stock_setting_75_bits() const { return data()->stock_setting_75; }
  void set_stock_setting_75_bits(uint32_t v) const { data()->stock_setting_75 = v; }
  // car byte +80 (probably the front camber)
  uint32_t stock_setting_80_address() const { return address + 276; }
  uint32_t stock_setting_80() const { return data()->stock_setting_80; }
  void set_stock_setting_80(uint32_t v) const { data()->stock_setting_80 = v; }
  uint32_t stock_setting_80_bits() const { return data()->stock_setting_80; }
  void set_stock_setting_80_bits(uint32_t v) const { data()->stock_setting_80 = v; }
  // car byte +81 (probably the rear camber)
  uint32_t stock_setting_81_address() const { return address + 280; }
  uint32_t stock_setting_81() const { return data()->stock_setting_81; }
  void set_stock_setting_81(uint32_t v) const { data()->stock_setting_81 = v; }
  uint32_t stock_setting_81_bits() const { return data()->stock_setting_81; }
  void set_stock_setting_81_bits(uint32_t v) const { data()->stock_setting_81 = v; }
  // car byte +78
  uint32_t stock_setting_78_address() const { return address + 284; }
  uint32_t stock_setting_78() const { return data()->stock_setting_78; }
  void set_stock_setting_78(uint32_t v) const { data()->stock_setting_78 = v; }
  uint32_t stock_setting_78_bits() const { return data()->stock_setting_78; }
  void set_stock_setting_78_bits(uint32_t v) const { data()->stock_setting_78 = v; }
  // car byte +79
  uint32_t stock_setting_79_address() const { return address + 288; }
  uint32_t stock_setting_79() const { return data()->stock_setting_79; }
  void set_stock_setting_79(uint32_t v) const { data()->stock_setting_79 = v; }
  uint32_t stock_setting_79_bits() const { return data()->stock_setting_79; }
  void set_stock_setting_79_bits(uint32_t v) const { data()->stock_setting_79 = v; }
  // car byte +82
  uint32_t stock_setting_82_address() const { return address + 292; }
  uint32_t stock_setting_82() const { return data()->stock_setting_82; }
  void set_stock_setting_82(uint32_t v) const { data()->stock_setting_82 = v; }
  uint32_t stock_setting_82_bits() const { return data()->stock_setting_82; }
  void set_stock_setting_82_bits(uint32_t v) const { data()->stock_setting_82 = v; }
  // car byte +83
  uint32_t stock_setting_83_address() const { return address + 296; }
  uint32_t stock_setting_83() const { return data()->stock_setting_83; }
  void set_stock_setting_83(uint32_t v) const { data()->stock_setting_83 = v; }
  uint32_t stock_setting_83_bits() const { return data()->stock_setting_83; }
  void set_stock_setting_83_bits(uint32_t v) const { data()->stock_setting_83 = v; }
  // car byte +84
  uint32_t stock_setting_84_address() const { return address + 300; }
  uint32_t stock_setting_84() const { return data()->stock_setting_84; }
  void set_stock_setting_84(uint32_t v) const { data()->stock_setting_84 = v; }
  uint32_t stock_setting_84_bits() const { return data()->stock_setting_84; }
  void set_stock_setting_84_bits(uint32_t v) const { data()->stock_setting_84 = v; }
  // car byte +85
  uint32_t stock_setting_85_address() const { return address + 304; }
  uint32_t stock_setting_85() const { return data()->stock_setting_85; }
  void set_stock_setting_85(uint32_t v) const { data()->stock_setting_85 = v; }
  uint32_t stock_setting_85_bits() const { return data()->stock_setting_85; }
  void set_stock_setting_85_bits(uint32_t v) const { data()->stock_setting_85 = v; }
  // car byte +86
  uint32_t stock_setting_86_address() const { return address + 308; }
  uint32_t stock_setting_86() const { return data()->stock_setting_86; }
  void set_stock_setting_86(uint32_t v) const { data()->stock_setting_86 = v; }
  uint32_t stock_setting_86_bits() const { return data()->stock_setting_86; }
  void set_stock_setting_86_bits(uint32_t v) const { data()->stock_setting_86 = v; }
  // car byte +87
  uint32_t stock_setting_87_address() const { return address + 312; }
  uint32_t stock_setting_87() const { return data()->stock_setting_87; }
  void set_stock_setting_87(uint32_t v) const { data()->stock_setting_87 = v; }
  uint32_t stock_setting_87_bits() const { return data()->stock_setting_87; }
  void set_stock_setting_87_bits(uint32_t v) const { data()->stock_setting_87 = v; }
  // car byte +88
  uint32_t stock_setting_88_address() const { return address + 316; }
  uint32_t stock_setting_88() const { return data()->stock_setting_88; }
  void set_stock_setting_88(uint32_t v) const { data()->stock_setting_88 = v; }
  uint32_t stock_setting_88_bits() const { return data()->stock_setting_88; }
  void set_stock_setting_88_bits(uint32_t v) const { data()->stock_setting_88 = v; }
  // car byte +89
  uint32_t stock_setting_89_address() const { return address + 320; }
  uint32_t stock_setting_89() const { return data()->stock_setting_89; }
  void set_stock_setting_89(uint32_t v) const { data()->stock_setting_89 = v; }
  uint32_t stock_setting_89_bits() const { return data()->stock_setting_89; }
  void set_stock_setting_89_bits(uint32_t v) const { data()->stock_setting_89 = v; }
  // car byte +94
  uint32_t stock_setting_94_address() const { return address + 324; }
  uint32_t stock_setting_94() const { return data()->stock_setting_94; }
  void set_stock_setting_94(uint32_t v) const { data()->stock_setting_94 = v; }
  uint32_t stock_setting_94_bits() const { return data()->stock_setting_94; }
  void set_stock_setting_94_bits(uint32_t v) const { data()->stock_setting_94 = v; }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t stock_torque_share_address() const { return address + 328; }
  uint32_t stock_torque_share() const { return data()->stock_torque_share; }
  void set_stock_torque_share(uint32_t v) const { data()->stock_torque_share = v; }
  uint32_t stock_torque_share_bits() const { return data()->stock_torque_share; }
  void set_stock_torque_share_bits(uint32_t v) const { data()->stock_torque_share = v; }
  // car byte +90
  uint32_t stock_setting_90_address() const { return address + 332; }
  uint32_t stock_setting_90() const { return data()->stock_setting_90; }
  void set_stock_setting_90(uint32_t v) const { data()->stock_setting_90 = v; }
  uint32_t stock_setting_90_bits() const { return data()->stock_setting_90; }
  void set_stock_setting_90_bits(uint32_t v) const { data()->stock_setting_90 = v; }
  // car byte +91
  uint32_t stock_setting_91_address() const { return address + 336; }
  uint32_t stock_setting_91() const { return data()->stock_setting_91; }
  void set_stock_setting_91(uint32_t v) const { data()->stock_setting_91 = v; }
  uint32_t stock_setting_91_bits() const { return data()->stock_setting_91; }
  void set_stock_setting_91_bits(uint32_t v) const { data()->stock_setting_91 = v; }
  // car byte +76
  uint32_t stock_setting_76_address() const { return address + 340; }
  uint32_t stock_setting_76() const { return data()->stock_setting_76; }
  void set_stock_setting_76(uint32_t v) const { data()->stock_setting_76 = v; }
  uint32_t stock_setting_76_bits() const { return data()->stock_setting_76; }
  void set_stock_setting_76_bits(uint32_t v) const { data()->stock_setting_76 = v; }
  // car byte +77
  uint32_t stock_setting_77_address() const { return address + 344; }
  uint32_t stock_setting_77() const { return data()->stock_setting_77; }
  void set_stock_setting_77(uint32_t v) const { data()->stock_setting_77 = v; }
  uint32_t stock_setting_77_bits() const { return data()->stock_setting_77; }
  void set_stock_setting_77_bits(uint32_t v) const { data()->stock_setting_77 = v; }
  // route 8's best times: five 12-byte rows {the course record entry's +20, its +0, 1 when there}; row 0's first word
  uint32_t route8_time_address() const { return address + 348; }
  uint32_t route8_time() const { return data()->route8_time; }
  void set_route8_time(uint32_t v) const { data()->route8_time = v; }
  uint32_t route8_time_bits() const { return data()->route8_time; }
  void set_route8_time_bits(uint32_t v) const { data()->route8_time = v; }
  // the entry's +0
  uint32_t route8_entry_0_address() const { return address + 352; }
  uint32_t route8_entry_0() const { return data()->route8_entry_0; }
  void set_route8_entry_0(uint32_t v) const { data()->route8_entry_0 = v; }
  uint32_t route8_entry_0_bits() const { return data()->route8_entry_0; }
  void set_route8_entry_0_bits(uint32_t v) const { data()->route8_entry_0 = v; }
  // 1 when there, 0 past the record's count
  uint32_t route8_present_address() const { return address + 356; }
  uint32_t route8_present() const { return data()->route8_present; }
  void set_route8_present(uint32_t v) const { data()->route8_present = v; }
  uint32_t route8_present_bits() const { return data()->route8_present; }
  void set_route8_present_bits(uint32_t v) const { data()->route8_present = v; }
  // rows 1 to 4
  uint32_t route8_rows_address() const { return address + 360; }
  uint8_t* route8_rows() const { return reinterpret_cast<uint8_t*>(data()) + 360; }
  // route 9's rows, as route 8's
  uint32_t route9_time_address() const { return address + 408; }
  uint32_t route9_time() const { return data()->route9_time; }
  void set_route9_time(uint32_t v) const { data()->route9_time = v; }
  uint32_t route9_time_bits() const { return data()->route9_time; }
  void set_route9_time_bits(uint32_t v) const { data()->route9_time = v; }
  uint32_t route9_entry_0_address() const { return address + 412; }
  uint32_t route9_entry_0() const { return data()->route9_entry_0; }
  void set_route9_entry_0(uint32_t v) const { data()->route9_entry_0 = v; }
  uint32_t route9_entry_0_bits() const { return data()->route9_entry_0; }
  void set_route9_entry_0_bits(uint32_t v) const { data()->route9_entry_0 = v; }
  uint32_t route9_present_address() const { return address + 416; }
  uint32_t route9_present() const { return data()->route9_present; }
  void set_route9_present(uint32_t v) const { data()->route9_present = v; }
  uint32_t route9_present_bits() const { return data()->route9_present; }
  void set_route9_present_bits(uint32_t v) const { data()->route9_present = v; }
  uint32_t route9_rows_address() const { return address + 420; }
  uint8_t* route9_rows() const { return reinterpret_cast<uint8_t*>(data()) + 420; }
  // route 10's rows, as route 8's
  uint32_t route10_time_address() const { return address + 468; }
  uint32_t route10_time() const { return data()->route10_time; }
  void set_route10_time(uint32_t v) const { data()->route10_time = v; }
  uint32_t route10_time_bits() const { return data()->route10_time; }
  void set_route10_time_bits(uint32_t v) const { data()->route10_time = v; }
  uint32_t route10_entry_0_address() const { return address + 472; }
  uint32_t route10_entry_0() const { return data()->route10_entry_0; }
  void set_route10_entry_0(uint32_t v) const { data()->route10_entry_0 = v; }
  uint32_t route10_entry_0_bits() const { return data()->route10_entry_0; }
  void set_route10_entry_0_bits(uint32_t v) const { data()->route10_entry_0 = v; }
  uint32_t route10_present_address() const { return address + 476; }
  uint32_t route10_present() const { return data()->route10_present; }
  void set_route10_present(uint32_t v) const { data()->route10_present = v; }
  uint32_t route10_present_bits() const { return data()->route10_present; }
  void set_route10_present_bits(uint32_t v) const { data()->route10_present = v; }
  uint32_t route10_rows_address() const { return address + 480; }
  uint8_t* route10_rows() const { return reinterpret_cast<uint8_t*>(data()) + 480; }
  // the save's +8 to +27
  uint32_t player_name_address() const { return address + 528; }
  uint8_t* player_name() const { return reinterpret_cast<uint8_t*>(data()) + 528; }
  // the save's +6116
  uint32_t money_address() const { return address + 548; }
  uint32_t money() const { return data()->money; }
  void set_money(uint32_t v) const { data()->money = v; }
  uint32_t money_bits() const { return data()->money; }
  void set_money_bits(uint32_t v) const { data()->money = v; }
  // the day mod 7
  uint32_t weekday_address() const { return address + 552; }
  uint32_t weekday() const { return data()->weekday; }
  void set_weekday(uint32_t v) const { data()->weekday = v; }
  uint32_t weekday_bits() const { return data()->weekday; }
  void set_weekday_bits(uint32_t v) const { data()->weekday = v; }
  // the save's +6100, at most 999
  uint32_t day_address() const { return address + 556; }
  uint32_t day() const { return data()->day; }
  void set_day(uint32_t v) const { data()->day = v; }
  uint32_t day_bits() const { return data()->day; }
  void set_day_bits(uint32_t v) const { data()->day = v; }
  // 1 in the type 5 mode (no test run, no stand car)
  uint32_t type5_mode_address() const { return address + 560; }
  uint32_t type5_mode() const { return data()->type5_mode; }
  void set_type5_mode(uint32_t v) const { data()->type5_mode = v; }
  uint32_t type5_mode_bits() const { return data()->type5_mode; }
  void set_type5_mode_bits(uint32_t v) const { data()->type5_mode = v; }
  // 5, or 4 in the type 5 mode (probably the top menu's item count)
  uint32_t top_items_address() const { return address + 564; }
  uint32_t top_items() const { return data()->top_items; }
  void set_top_items(uint32_t v) const { data()->top_items = v; }
  uint32_t top_items_bits() const { return data()->top_items; }
  void set_top_items_bits(uint32_t v) const { data()->top_items = v; }
  // the test run's course picked, 0 to 3
  uint32_t course_address() const { return address + 568; }
  uint32_t course() const { return data()->course; }
  void set_course(uint32_t v) const { data()->course = v; }
  uint32_t course_bits() const { return data()->course; }
  void set_course_bits(uint32_t v) const { data()->course = v; }
  // 0 yes, 1 no, 2 back
  uint32_t dialog_answer_address() const { return address + 572; }
  uint32_t dialog_answer() const { return data()->dialog_answer; }
  void set_dialog_answer(uint32_t v) const { data()->dialog_answer = v; }
  uint32_t dialog_answer_bits() const { return data()->dialog_answer; }
  void set_dialog_answer_bits(uint32_t v) const { data()->dialog_answer = v; }
  // 1 once a test run is set up (Destroy then leaves the state at 11)
  uint32_t test_run_set_up_address() const { return address + 576; }
  uint32_t test_run_set_up() const { return data()->test_run_set_up; }
  void set_test_run_set_up(uint32_t v) const { data()->test_run_set_up = v; }
  uint32_t test_run_set_up_bits() const { return data()->test_run_set_up; }
  void set_test_run_set_up_bits(uint32_t v) const { data()->test_run_set_up = v; }
  // 1 to start at the courses (ReenterToCourses)
  uint32_t start_at_courses_address() const { return address + 580; }
  uint32_t start_at_courses() const { return data()->start_at_courses; }
  void set_start_at_courses(uint32_t v) const { data()->start_at_courses = v; }
  uint32_t start_at_courses_bits() const { return data()->start_at_courses; }
  void set_start_at_courses_bits(uint32_t v) const { data()->start_at_courses = v; }
  // the tuning's +32 nonzero
  uint32_t allows_gears_address() const { return address + 584; }
  uint32_t allows_gears() const { return data()->allows_gears; }
  void set_allows_gears(uint32_t v) const { data()->allows_gears = v; }
  uint32_t allows_gears_bits() const { return data()->allows_gears; }
  void set_allows_gears_bits(uint32_t v) const { data()->allows_gears = v; }
  // the tuning's +56 nonzero
  uint32_t allows_pages_9_10_address() const { return address + 588; }
  uint32_t allows_pages_9_10() const { return data()->allows_pages_9_10; }
  void set_allows_pages_9_10(uint32_t v) const { data()->allows_pages_9_10 = v; }
  uint32_t allows_pages_9_10_bits() const { return data()->allows_pages_9_10; }
  void set_allows_pages_9_10_bits(uint32_t v) const { data()->allows_pages_9_10 = v; }
  // the tuning's +40 nonzero
  uint32_t allows_pages_11_16_address() const { return address + 592; }
  uint32_t allows_pages_11_16() const { return data()->allows_pages_11_16; }
  void set_allows_pages_11_16(uint32_t v) const { data()->allows_pages_11_16 = v; }
  uint32_t allows_pages_11_16_bits() const { return data()->allows_pages_11_16; }
  void set_allows_pages_11_16_bits(uint32_t v) const { data()->allows_pages_11_16 = v; }
  // the aero part's bit
  uint32_t allows_page_17_address() const { return address + 596; }
  uint32_t allows_page_17() const { return data()->allows_page_17; }
  void set_allows_page_17(uint32_t v) const { data()->allows_page_17 = v; }
  uint32_t allows_page_17_bits() const { return data()->allows_page_17; }
  void set_allows_page_17_bits(uint32_t v) const { data()->allows_page_17 = v; }
  // spec block 2's +100
  uint32_t spec_block2_100_address() const { return address + 600; }
  uint32_t spec_block2_100() const { return data()->spec_block2_100; }
  void set_spec_block2_100(uint32_t v) const { data()->spec_block2_100 = v; }
  uint32_t spec_block2_100_bits() const { return data()->spec_block2_100; }
  void set_spec_block2_100_bits(uint32_t v) const { data()->spec_block2_100 = v; }
  // seven 12-byte bounds of the gear ratios {min, max, ...}
  uint32_t gear_bounds_address() const { return address + 604; }
  uint8_t* gear_bounds() const { return reinterpret_cast<uint8_t*>(data()) + 604; }
  // block 2's byte +130 (probably the drive type; 4 both ends)
  uint32_t drive_type_address() const { return address + 688; }
  uint32_t drive_type() const { return data()->drive_type; }
  void set_drive_type(uint32_t v) const { data()->drive_type = v; }
  uint32_t drive_type_bits() const { return data()->drive_type; }
  void set_drive_type_bits(uint32_t v) const { data()->drive_type = v; }
  // the tuning's +52
  uint32_t tuning_52_address() const { return address + 692; }
  uint32_t tuning_52() const { return data()->tuning_52; }
  void set_tuning_52(uint32_t v) const { data()->tuning_52 = v; }
  uint32_t tuning_52_bits() const { return data()->tuning_52; }
  void set_tuning_52_bits(uint32_t v) const { data()->tuning_52 = v; }
  // the car data's +388 (the gear page's speeds)
  uint32_t car_data_388_address() const { return address + 696; }
  float car_data_388() const { return data()->car_data_388; }
  void set_car_data_388(float v) const { data()->car_data_388 = v; }
  uint32_t car_data_388_bits() const { return std::bit_cast<uint32_t>(data()->car_data_388); }
  void set_car_data_388_bits(uint32_t v) const { data()->car_data_388 = std::bit_cast<float>(v); }
  // the car data's +1280 (the gear page's speeds)
  uint32_t car_data_1280_address() const { return address + 700; }
  float car_data_1280() const { return data()->car_data_1280; }
  void set_car_data_1280(float v) const { data()->car_data_1280 = v; }
  uint32_t car_data_1280_bits() const { return std::bit_cast<uint32_t>(data()->car_data_1280); }
  void set_car_data_1280_bits(uint32_t v) const { data()->car_data_1280 = std::bit_cast<float>(v); }
  uint32_t unknown_704_address() const { return address + 704; }
  uint32_t unknown_704() const { return data()->unknown_704; }
  void set_unknown_704(uint32_t v) const { data()->unknown_704 = v; }
  uint32_t unknown_704_bits() const { return data()->unknown_704; }
  void set_unknown_704_bits(uint32_t v) const { data()->unknown_704 = v; }
  // a word stack
  uint32_t stack_address() const { return address + 708; }
  uint8_t* stack() const { return reinterpret_cast<uint8_t*>(data()) + 708; }
  uint32_t stack_count_address() const { return address + 728; }
  uint32_t stack_count() const { return data()->stack_count; }
  void set_stack_count(uint32_t v) const { data()->stack_count = v; }
  uint32_t stack_count_bits() const { return data()->stack_count; }
  void set_stack_count_bits(uint32_t v) const { data()->stack_count = v; }
  // the apply item's choice: 0 apply, 1 the car's, 2 stock
  uint32_t apply_choice_address() const { return address + 732; }
  uint32_t apply_choice() const { return data()->apply_choice; }
  void set_apply_choice(uint32_t v) const { data()->apply_choice = v; }
  uint32_t apply_choice_bits() const { return data()->apply_choice; }
  void set_apply_choice_bits(uint32_t v) const { data()->apply_choice = v; }
  uint32_t previous_apply_choice_address() const { return address + 736; }
  uint32_t previous_apply_choice() const { return data()->previous_apply_choice; }
  void set_previous_apply_choice(uint32_t v) const { data()->previous_apply_choice = v; }
  uint32_t previous_apply_choice_bits() const { return data()->previous_apply_choice; }
  void set_previous_apply_choice_bits(uint32_t v) const { data()->previous_apply_choice = v; }
  // 0 at Fill
  uint32_t unknown_740_address() const { return address + 740; }
  uint32_t unknown_740() const { return data()->unknown_740; }
  void set_unknown_740(uint32_t v) const { data()->unknown_740 = v; }
  uint32_t unknown_740_bits() const { return data()->unknown_740; }
  void set_unknown_740_bits(uint32_t v) const { data()->unknown_740 = v; }
  // Start: 0 when starting at the courses, else 1
  uint32_t unknown_744_address() const { return address + 744; }
  uint32_t unknown_744() const { return data()->unknown_744; }
  void set_unknown_744(uint32_t v) const { data()->unknown_744 = v; }
  uint32_t unknown_744_bits() const { return data()->unknown_744; }
  void set_unknown_744_bits(uint32_t v) const { data()->unknown_744 = v; }
  // 0 at Create, 1 at Fill
  uint32_t unknown_748_address() const { return address + 748; }
  uint32_t unknown_748() const { return data()->unknown_748; }
  void set_unknown_748(uint32_t v) const { data()->unknown_748 = v; }
  uint32_t unknown_748_bits() const { return data()->unknown_748; }
  void set_unknown_748_bits(uint32_t v) const { data()->unknown_748 = v; }
  // the type 5 mode's location - 14, else 3 (Fill's SetText: string 369 + it)
  uint32_t location_address() const { return address + 752; }
  uint32_t location() const { return data()->location; }
  void set_location(uint32_t v) const { data()->location = v; }
  uint32_t location_bits() const { return data()->location; }
  void set_location_bits(uint32_t v) const { data()->location = v; }
  // 0 at Fill
  uint32_t unknown_756_address() const { return address + 756; }
  uint32_t unknown_756() const { return data()->unknown_756; }
  void set_unknown_756(uint32_t v) const { data()->unknown_756 = v; }
  uint32_t unknown_756_bits() const { return data()->unknown_756; }
  void set_unknown_756_bits(uint32_t v) const { data()->unknown_756 = v; }
  // 21 at Create, 10 at a test run
  uint32_t unknown_760_address() const { return address + 760; }
  uint32_t unknown_760() const { return data()->unknown_760; }
  void set_unknown_760(uint32_t v) const { data()->unknown_760 = v; }
  uint32_t unknown_760_bits() const { return data()->unknown_760; }
  void set_unknown_760_bits(uint32_t v) const { data()->unknown_760 = v; }
};

// embedded; measure: CourseRecord::Entry.r4, CourseRecords::Of.ret
struct CourseRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 408;
  // ten halfword pairs: the entries' links (+0 the next, +2 the entry's index)
  uint32_t links_address() const { return address + 0; }
  uint16_t unknown_40() const { return guest::LoadU16(base, address + 40); }
  void set_unknown_40(uint16_t v) const { guest::StoreU16(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint16_t unknown_42() const { return guest::LoadU16(base, address + 42); }
  void set_unknown_42(uint16_t v) const { guest::StoreU16(base, address + 42, v); }
  uint32_t unknown_42_address() const { return address + 42; }
  // the first link (the ranks followed from it)
  uint16_t first() const { return guest::LoadU16(base, address + 44); }
  void set_first(uint16_t v) const { guest::StoreU16(base, address + 44, v); }
  uint32_t first_address() const { return address + 44; }
  uint16_t unknown_46() const { return guest::LoadU16(base, address + 46); }
  void set_unknown_46(uint16_t v) const { guest::StoreU16(base, address + 46, v); }
  uint32_t unknown_46_address() const { return address + 46; }
  // probably the entries' count
  uint16_t count() const { return guest::LoadU16(base, address + 48); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 48, v); }
  uint32_t count_address() const { return address + 48; }
  // 28 bytes each (CourseRecordEntry)
  uint32_t entries_address() const { return address + 52; }
  // a 76-byte record (cleared at the reset)
  uint32_t unknown_332_address() const { return address + 332; }
};

// embedded; measure: CourseRecord::Entry.ret
struct CourseRecordEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the car settings' best time rows' second word
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // probably the time: the best time rows' first word
  uint32_t time() const { return guest::LoadU32(base, address + 20); }
  void set_time(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t time_address() const { return address + 20; }
};

// The image words docs/functions/car-settings.md's code uses.
struct CarSettingsGlobals {
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8458u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8458u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8458u, v); }
  static constexpr uint32_t pages_address() { return 0x826D8458u; }
  // a front-end menu (+16 = 4 at Create)
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D845Cu)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D845Cu, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D845Cu, v); }
  static constexpr uint32_t menu_address() { return 0x826D845Cu; }
  template <typename T = CarSettingsData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8460u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8460u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8460u, v); }
  static constexpr uint32_t data_address() { return 0x826D8460u; }
};

}  // namespace schema
