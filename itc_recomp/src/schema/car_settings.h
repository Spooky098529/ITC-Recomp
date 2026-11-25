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
  uint32_t transition_count() const { return guest::LoadU32(address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 3 is loaded (WaitForMovie)
  uint32_t loaded() const { return guest::LoadU32(address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForMovie go to state 9 (open on its message)
  uint32_t reenter() const { return guest::LoadU32(address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // as the type 2 mode's other machines (delete the movie on unload)
  uint32_t delete_movie() const { return guest::LoadU32(address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(address + 9616, v); }
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
  static CarSettingsMode New() {
    return CarSettingsMode(memory::MakeHandle(new CarSettingsModeData(), sizeof(CarSettingsModeData), "CarSettingsMode", &kCarSettingsModeLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSettingsMode NewArray(uint32_t count) {
    static_assert(sizeof(CarSettingsModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSettingsModeData*>(::operator new(sizeof(CarSettingsModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSettingsModeData();
    return CarSettingsMode(memory::MakeHandle(p, sizeof(CarSettingsModeData) * count, "CarSettingsMode", &kCarSettingsModeLayout, sizeof(CarSettingsModeData)));
  }
  CarSettingsMode at(uint32_t i) const { return CarSettingsMode(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSettingsModeData* direct() const { return static_cast<CarSettingsModeData*>(memory::HandleOwn(address, &kCarSettingsModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSettingsModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSettingsModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count_bits() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(address + 804))); }
  void set_message_count_bits(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(address + 804, v); }
  uint32_t message_count() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(address + 804))); }
  void set_message_count(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start_bits() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(address + 808))); }
  void set_message_start_bits(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(address + 808, v); }
  uint32_t message_start() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(address + 808))); }
  void set_message_start(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(address + 808, v); }
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
  static CarSettingsData New() {
    return CarSettingsData(memory::MakeHandle(new CarSettingsDataData(), sizeof(CarSettingsDataData), "CarSettingsData", &kCarSettingsDataLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSettingsData NewArray(uint32_t count) {
    static_assert(sizeof(CarSettingsDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSettingsDataData*>(::operator new(sizeof(CarSettingsDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSettingsDataData();
    return CarSettingsData(memory::MakeHandle(p, sizeof(CarSettingsDataData) * count, "CarSettingsData", &kCarSettingsDataLayout, sizeof(CarSettingsDataData)));
  }
  CarSettingsData at(uint32_t i) const { return CarSettingsData(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSettingsDataData* direct() const { return static_cast<CarSettingsDataData*>(memory::HandleOwn(address, &kCarSettingsDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSettingsDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSettingsDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // car +40
  uint32_t working_gear_1_address() const { return address + 0; }
  uint32_t working_gear_1_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_gear_1); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_working_gear_1_bits(uint32_t v) const { if (auto* d = direct()) { d->working_gear_1 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  float working_gear_1() const { return std::bit_cast<float>(working_gear_1_bits()); }
  void set_working_gear_1(float v) const { set_working_gear_1_bits(std::bit_cast<uint32_t>(v)); }
  // car +44
  uint32_t working_gear_2_address() const { return address + 4; }
  uint32_t working_gear_2_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_gear_2); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_working_gear_2_bits(uint32_t v) const { if (auto* d = direct()) { d->working_gear_2 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float working_gear_2() const { return std::bit_cast<float>(working_gear_2_bits()); }
  void set_working_gear_2(float v) const { set_working_gear_2_bits(std::bit_cast<uint32_t>(v)); }
  // car +48
  uint32_t working_gear_3_address() const { return address + 8; }
  uint32_t working_gear_3_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_gear_3); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_working_gear_3_bits(uint32_t v) const { if (auto* d = direct()) { d->working_gear_3 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float working_gear_3() const { return std::bit_cast<float>(working_gear_3_bits()); }
  void set_working_gear_3(float v) const { set_working_gear_3_bits(std::bit_cast<uint32_t>(v)); }
  // car +52
  uint32_t working_gear_4_address() const { return address + 12; }
  uint32_t working_gear_4_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_gear_4); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_working_gear_4_bits(uint32_t v) const { if (auto* d = direct()) { d->working_gear_4 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  float working_gear_4() const { return std::bit_cast<float>(working_gear_4_bits()); }
  void set_working_gear_4(float v) const { set_working_gear_4_bits(std::bit_cast<uint32_t>(v)); }
  // car +56
  uint32_t working_gear_5_address() const { return address + 16; }
  uint32_t working_gear_5_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_gear_5); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_working_gear_5_bits(uint32_t v) const { if (auto* d = direct()) { d->working_gear_5 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float working_gear_5() const { return std::bit_cast<float>(working_gear_5_bits()); }
  void set_working_gear_5(float v) const { set_working_gear_5_bits(std::bit_cast<uint32_t>(v)); }
  // car +60
  uint32_t working_gear_6_address() const { return address + 20; }
  uint32_t working_gear_6_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_gear_6); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_working_gear_6_bits(uint32_t v) const { if (auto* d = direct()) { d->working_gear_6 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float working_gear_6() const { return std::bit_cast<float>(working_gear_6_bits()); }
  void set_working_gear_6(float v) const { set_working_gear_6_bits(std::bit_cast<uint32_t>(v)); }
  // car +36
  uint32_t working_final_drive_address() const { return address + 24; }
  uint32_t working_final_drive_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->working_final_drive); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_working_final_drive_bits(uint32_t v) const { if (auto* d = direct()) { d->working_final_drive = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float working_final_drive() const { return std::bit_cast<float>(working_final_drive_bits()); }
  void set_working_final_drive(float v) const { set_working_final_drive_bits(std::bit_cast<uint32_t>(v)); }
  // car byte +72
  uint32_t working_setting_72_address() const { return address + 28; }
  uint32_t working_setting_72_bits() const { if (auto* d = direct()) return d->working_setting_72; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_working_setting_72_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_72 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t working_setting_72() const { if (auto* d = direct()) return d->working_setting_72; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_working_setting_72(uint32_t v) const { if (auto* d = direct()) { d->working_setting_72 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // car byte +73
  uint32_t working_setting_73_address() const { return address + 32; }
  uint32_t working_setting_73_bits() const { if (auto* d = direct()) return d->working_setting_73; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_working_setting_73_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_73 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t working_setting_73() const { if (auto* d = direct()) return d->working_setting_73; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_working_setting_73(uint32_t v) const { if (auto* d = direct()) { d->working_setting_73 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // car byte +74
  uint32_t working_setting_74_address() const { return address + 36; }
  uint32_t working_setting_74_bits() const { if (auto* d = direct()) return d->working_setting_74; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_working_setting_74_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_74 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  uint32_t working_setting_74() const { if (auto* d = direct()) return d->working_setting_74; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_working_setting_74(uint32_t v) const { if (auto* d = direct()) { d->working_setting_74 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  // car byte +75
  uint32_t working_setting_75_address() const { return address + 40; }
  uint32_t working_setting_75_bits() const { if (auto* d = direct()) return d->working_setting_75; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_working_setting_75_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_75 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t working_setting_75() const { if (auto* d = direct()) return d->working_setting_75; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_working_setting_75(uint32_t v) const { if (auto* d = direct()) { d->working_setting_75 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // car byte +80 (probably the front camber)
  uint32_t working_setting_80_address() const { return address + 44; }
  uint32_t working_setting_80_bits() const { if (auto* d = direct()) return d->working_setting_80; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_working_setting_80_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_80 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t working_setting_80() const { if (auto* d = direct()) return d->working_setting_80; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_working_setting_80(uint32_t v) const { if (auto* d = direct()) { d->working_setting_80 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // car byte +81 (probably the rear camber)
  uint32_t working_setting_81_address() const { return address + 48; }
  uint32_t working_setting_81_bits() const { if (auto* d = direct()) return d->working_setting_81; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_working_setting_81_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_81 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t working_setting_81() const { if (auto* d = direct()) return d->working_setting_81; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_working_setting_81(uint32_t v) const { if (auto* d = direct()) { d->working_setting_81 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // car byte +78
  uint32_t working_setting_78_address() const { return address + 52; }
  uint32_t working_setting_78_bits() const { if (auto* d = direct()) return d->working_setting_78; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_working_setting_78_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_78 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t working_setting_78() const { if (auto* d = direct()) return d->working_setting_78; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_working_setting_78(uint32_t v) const { if (auto* d = direct()) { d->working_setting_78 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // car byte +79
  uint32_t working_setting_79_address() const { return address + 56; }
  uint32_t working_setting_79_bits() const { if (auto* d = direct()) return d->working_setting_79; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_working_setting_79_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_79 = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t working_setting_79() const { if (auto* d = direct()) return d->working_setting_79; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_working_setting_79(uint32_t v) const { if (auto* d = direct()) { d->working_setting_79 = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // car byte +82
  uint32_t working_setting_82_address() const { return address + 60; }
  uint32_t working_setting_82_bits() const { if (auto* d = direct()) return d->working_setting_82; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_working_setting_82_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_82 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t working_setting_82() const { if (auto* d = direct()) return d->working_setting_82; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_working_setting_82(uint32_t v) const { if (auto* d = direct()) { d->working_setting_82 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // car byte +83
  uint32_t working_setting_83_address() const { return address + 64; }
  uint32_t working_setting_83_bits() const { if (auto* d = direct()) return d->working_setting_83; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_working_setting_83_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_83 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t working_setting_83() const { if (auto* d = direct()) return d->working_setting_83; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_working_setting_83(uint32_t v) const { if (auto* d = direct()) { d->working_setting_83 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  // car byte +84
  uint32_t working_setting_84_address() const { return address + 68; }
  uint32_t working_setting_84_bits() const { if (auto* d = direct()) return d->working_setting_84; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_working_setting_84_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_84 = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t working_setting_84() const { if (auto* d = direct()) return d->working_setting_84; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_working_setting_84(uint32_t v) const { if (auto* d = direct()) { d->working_setting_84 = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  // car byte +85
  uint32_t working_setting_85_address() const { return address + 72; }
  uint32_t working_setting_85_bits() const { if (auto* d = direct()) return d->working_setting_85; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_working_setting_85_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_85 = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t working_setting_85() const { if (auto* d = direct()) return d->working_setting_85; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_working_setting_85(uint32_t v) const { if (auto* d = direct()) { d->working_setting_85 = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  // car byte +86
  uint32_t working_setting_86_address() const { return address + 76; }
  uint32_t working_setting_86_bits() const { if (auto* d = direct()) return d->working_setting_86; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_working_setting_86_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_86 = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t working_setting_86() const { if (auto* d = direct()) return d->working_setting_86; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_working_setting_86(uint32_t v) const { if (auto* d = direct()) { d->working_setting_86 = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  // car byte +87
  uint32_t working_setting_87_address() const { return address + 80; }
  uint32_t working_setting_87_bits() const { if (auto* d = direct()) return d->working_setting_87; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_working_setting_87_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_87 = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t working_setting_87() const { if (auto* d = direct()) return d->working_setting_87; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_working_setting_87(uint32_t v) const { if (auto* d = direct()) { d->working_setting_87 = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  // car byte +88
  uint32_t working_setting_88_address() const { return address + 84; }
  uint32_t working_setting_88_bits() const { if (auto* d = direct()) return d->working_setting_88; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_working_setting_88_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_88 = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t working_setting_88() const { if (auto* d = direct()) return d->working_setting_88; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_working_setting_88(uint32_t v) const { if (auto* d = direct()) { d->working_setting_88 = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  // car byte +89
  uint32_t working_setting_89_address() const { return address + 88; }
  uint32_t working_setting_89_bits() const { if (auto* d = direct()) return d->working_setting_89; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_working_setting_89_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_89 = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  uint32_t working_setting_89() const { if (auto* d = direct()) return d->working_setting_89; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_working_setting_89(uint32_t v) const { if (auto* d = direct()) { d->working_setting_89 = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  // car byte +94
  uint32_t working_setting_94_address() const { return address + 92; }
  uint32_t working_setting_94_bits() const { if (auto* d = direct()) return d->working_setting_94; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_working_setting_94_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_94 = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  uint32_t working_setting_94() const { if (auto* d = direct()) return d->working_setting_94; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_working_setting_94(uint32_t v) const { if (auto* d = direct()) { d->working_setting_94 = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t working_torque_share_address() const { return address + 96; }
  uint32_t working_torque_share_bits() const { if (auto* d = direct()) return d->working_torque_share; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_working_torque_share_bits(uint32_t v) const { if (auto* d = direct()) { d->working_torque_share = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t working_torque_share() const { if (auto* d = direct()) return d->working_torque_share; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_working_torque_share(uint32_t v) const { if (auto* d = direct()) { d->working_torque_share = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  // car byte +90
  uint32_t working_setting_90_address() const { return address + 100; }
  uint32_t working_setting_90_bits() const { if (auto* d = direct()) return d->working_setting_90; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_working_setting_90_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_90 = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t working_setting_90() const { if (auto* d = direct()) return d->working_setting_90; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_working_setting_90(uint32_t v) const { if (auto* d = direct()) { d->working_setting_90 = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  // car byte +91
  uint32_t working_setting_91_address() const { return address + 104; }
  uint32_t working_setting_91_bits() const { if (auto* d = direct()) return d->working_setting_91; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_working_setting_91_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_91 = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t working_setting_91() const { if (auto* d = direct()) return d->working_setting_91; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_working_setting_91(uint32_t v) const { if (auto* d = direct()) { d->working_setting_91 = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  // car byte +76
  uint32_t working_setting_76_address() const { return address + 108; }
  uint32_t working_setting_76_bits() const { if (auto* d = direct()) return d->working_setting_76; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_working_setting_76_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_76 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t working_setting_76() const { if (auto* d = direct()) return d->working_setting_76; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_working_setting_76(uint32_t v) const { if (auto* d = direct()) { d->working_setting_76 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  // car byte +77
  uint32_t working_setting_77_address() const { return address + 112; }
  uint32_t working_setting_77_bits() const { if (auto* d = direct()) return d->working_setting_77; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_working_setting_77_bits(uint32_t v) const { if (auto* d = direct()) { d->working_setting_77 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t working_setting_77() const { if (auto* d = direct()) return d->working_setting_77; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_working_setting_77(uint32_t v) const { if (auto* d = direct()) { d->working_setting_77 = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  // car +40
  uint32_t applied_gear_1_address() const { return address + 116; }
  uint32_t applied_gear_1_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_gear_1); return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_applied_gear_1_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_gear_1 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  float applied_gear_1() const { return std::bit_cast<float>(applied_gear_1_bits()); }
  void set_applied_gear_1(float v) const { set_applied_gear_1_bits(std::bit_cast<uint32_t>(v)); }
  // car +44
  uint32_t applied_gear_2_address() const { return address + 120; }
  uint32_t applied_gear_2_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_gear_2); return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_applied_gear_2_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_gear_2 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  float applied_gear_2() const { return std::bit_cast<float>(applied_gear_2_bits()); }
  void set_applied_gear_2(float v) const { set_applied_gear_2_bits(std::bit_cast<uint32_t>(v)); }
  // car +48
  uint32_t applied_gear_3_address() const { return address + 124; }
  uint32_t applied_gear_3_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_gear_3); return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_applied_gear_3_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_gear_3 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  float applied_gear_3() const { return std::bit_cast<float>(applied_gear_3_bits()); }
  void set_applied_gear_3(float v) const { set_applied_gear_3_bits(std::bit_cast<uint32_t>(v)); }
  // car +52
  uint32_t applied_gear_4_address() const { return address + 128; }
  uint32_t applied_gear_4_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_gear_4); return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_applied_gear_4_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_gear_4 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  float applied_gear_4() const { return std::bit_cast<float>(applied_gear_4_bits()); }
  void set_applied_gear_4(float v) const { set_applied_gear_4_bits(std::bit_cast<uint32_t>(v)); }
  // car +56
  uint32_t applied_gear_5_address() const { return address + 132; }
  uint32_t applied_gear_5_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_gear_5); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_applied_gear_5_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_gear_5 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  float applied_gear_5() const { return std::bit_cast<float>(applied_gear_5_bits()); }
  void set_applied_gear_5(float v) const { set_applied_gear_5_bits(std::bit_cast<uint32_t>(v)); }
  // car +60
  uint32_t applied_gear_6_address() const { return address + 136; }
  uint32_t applied_gear_6_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_gear_6); return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_applied_gear_6_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_gear_6 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  float applied_gear_6() const { return std::bit_cast<float>(applied_gear_6_bits()); }
  void set_applied_gear_6(float v) const { set_applied_gear_6_bits(std::bit_cast<uint32_t>(v)); }
  // car +36
  uint32_t applied_final_drive_address() const { return address + 140; }
  uint32_t applied_final_drive_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->applied_final_drive); return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_applied_final_drive_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_final_drive = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  float applied_final_drive() const { return std::bit_cast<float>(applied_final_drive_bits()); }
  void set_applied_final_drive(float v) const { set_applied_final_drive_bits(std::bit_cast<uint32_t>(v)); }
  // car byte +72
  uint32_t applied_setting_72_address() const { return address + 144; }
  uint32_t applied_setting_72_bits() const { if (auto* d = direct()) return d->applied_setting_72; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_applied_setting_72_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_72 = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  uint32_t applied_setting_72() const { if (auto* d = direct()) return d->applied_setting_72; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_applied_setting_72(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_72 = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  // car byte +73
  uint32_t applied_setting_73_address() const { return address + 148; }
  uint32_t applied_setting_73_bits() const { if (auto* d = direct()) return d->applied_setting_73; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_applied_setting_73_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_73 = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t applied_setting_73() const { if (auto* d = direct()) return d->applied_setting_73; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_applied_setting_73(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_73 = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  // car byte +74
  uint32_t applied_setting_74_address() const { return address + 152; }
  uint32_t applied_setting_74_bits() const { if (auto* d = direct()) return d->applied_setting_74; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_applied_setting_74_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_74 = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t applied_setting_74() const { if (auto* d = direct()) return d->applied_setting_74; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_applied_setting_74(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_74 = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  // car byte +75
  uint32_t applied_setting_75_address() const { return address + 156; }
  uint32_t applied_setting_75_bits() const { if (auto* d = direct()) return d->applied_setting_75; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_applied_setting_75_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_75 = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  uint32_t applied_setting_75() const { if (auto* d = direct()) return d->applied_setting_75; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_applied_setting_75(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_75 = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  // car byte +80 (probably the front camber)
  uint32_t applied_setting_80_address() const { return address + 160; }
  uint32_t applied_setting_80_bits() const { if (auto* d = direct()) return d->applied_setting_80; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_applied_setting_80_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_80 = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  uint32_t applied_setting_80() const { if (auto* d = direct()) return d->applied_setting_80; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_applied_setting_80(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_80 = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  // car byte +81 (probably the rear camber)
  uint32_t applied_setting_81_address() const { return address + 164; }
  uint32_t applied_setting_81_bits() const { if (auto* d = direct()) return d->applied_setting_81; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_applied_setting_81_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_81 = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  uint32_t applied_setting_81() const { if (auto* d = direct()) return d->applied_setting_81; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_applied_setting_81(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_81 = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  // car byte +78
  uint32_t applied_setting_78_address() const { return address + 168; }
  uint32_t applied_setting_78_bits() const { if (auto* d = direct()) return d->applied_setting_78; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_applied_setting_78_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_78 = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  uint32_t applied_setting_78() const { if (auto* d = direct()) return d->applied_setting_78; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_applied_setting_78(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_78 = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  // car byte +79
  uint32_t applied_setting_79_address() const { return address + 172; }
  uint32_t applied_setting_79_bits() const { if (auto* d = direct()) return d->applied_setting_79; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_applied_setting_79_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_79 = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  uint32_t applied_setting_79() const { if (auto* d = direct()) return d->applied_setting_79; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_applied_setting_79(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_79 = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  // car byte +82
  uint32_t applied_setting_82_address() const { return address + 176; }
  uint32_t applied_setting_82_bits() const { if (auto* d = direct()) return d->applied_setting_82; return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(address + 176))); }
  void set_applied_setting_82_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_82 = v; return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(address + 176, v); }
  uint32_t applied_setting_82() const { if (auto* d = direct()) return d->applied_setting_82; return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(address + 176))); }
  void set_applied_setting_82(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_82 = v; return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(address + 176, v); }
  // car byte +83
  uint32_t applied_setting_83_address() const { return address + 180; }
  uint32_t applied_setting_83_bits() const { if (auto* d = direct()) return d->applied_setting_83; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(address + 180))); }
  void set_applied_setting_83_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_83 = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(address + 180, v); }
  uint32_t applied_setting_83() const { if (auto* d = direct()) return d->applied_setting_83; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(address + 180))); }
  void set_applied_setting_83(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_83 = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(address + 180, v); }
  // car byte +84
  uint32_t applied_setting_84_address() const { return address + 184; }
  uint32_t applied_setting_84_bits() const { if (auto* d = direct()) return d->applied_setting_84; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_applied_setting_84_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_84 = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  uint32_t applied_setting_84() const { if (auto* d = direct()) return d->applied_setting_84; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_applied_setting_84(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_84 = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  // car byte +85
  uint32_t applied_setting_85_address() const { return address + 188; }
  uint32_t applied_setting_85_bits() const { if (auto* d = direct()) return d->applied_setting_85; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_applied_setting_85_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_85 = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  uint32_t applied_setting_85() const { if (auto* d = direct()) return d->applied_setting_85; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_applied_setting_85(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_85 = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  // car byte +86
  uint32_t applied_setting_86_address() const { return address + 192; }
  uint32_t applied_setting_86_bits() const { if (auto* d = direct()) return d->applied_setting_86; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_applied_setting_86_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_86 = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  uint32_t applied_setting_86() const { if (auto* d = direct()) return d->applied_setting_86; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_applied_setting_86(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_86 = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  // car byte +87
  uint32_t applied_setting_87_address() const { return address + 196; }
  uint32_t applied_setting_87_bits() const { if (auto* d = direct()) return d->applied_setting_87; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_applied_setting_87_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_87 = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  uint32_t applied_setting_87() const { if (auto* d = direct()) return d->applied_setting_87; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_applied_setting_87(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_87 = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  // car byte +88
  uint32_t applied_setting_88_address() const { return address + 200; }
  uint32_t applied_setting_88_bits() const { if (auto* d = direct()) return d->applied_setting_88; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(address + 200))); }
  void set_applied_setting_88_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_88 = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(address + 200, v); }
  uint32_t applied_setting_88() const { if (auto* d = direct()) return d->applied_setting_88; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(address + 200))); }
  void set_applied_setting_88(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_88 = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(address + 200, v); }
  // car byte +89
  uint32_t applied_setting_89_address() const { return address + 204; }
  uint32_t applied_setting_89_bits() const { if (auto* d = direct()) return d->applied_setting_89; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_applied_setting_89_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_89 = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  uint32_t applied_setting_89() const { if (auto* d = direct()) return d->applied_setting_89; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_applied_setting_89(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_89 = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  // car byte +94
  uint32_t applied_setting_94_address() const { return address + 208; }
  uint32_t applied_setting_94_bits() const { if (auto* d = direct()) return d->applied_setting_94; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_applied_setting_94_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_94 = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  uint32_t applied_setting_94() const { if (auto* d = direct()) return d->applied_setting_94; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_applied_setting_94(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_94 = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t applied_torque_share_address() const { return address + 212; }
  uint32_t applied_torque_share_bits() const { if (auto* d = direct()) return d->applied_torque_share; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_applied_torque_share_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_torque_share = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  uint32_t applied_torque_share() const { if (auto* d = direct()) return d->applied_torque_share; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_applied_torque_share(uint32_t v) const { if (auto* d = direct()) { d->applied_torque_share = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  // car byte +90
  uint32_t applied_setting_90_address() const { return address + 216; }
  uint32_t applied_setting_90_bits() const { if (auto* d = direct()) return d->applied_setting_90; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_applied_setting_90_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_90 = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  uint32_t applied_setting_90() const { if (auto* d = direct()) return d->applied_setting_90; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_applied_setting_90(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_90 = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  // car byte +91
  uint32_t applied_setting_91_address() const { return address + 220; }
  uint32_t applied_setting_91_bits() const { if (auto* d = direct()) return d->applied_setting_91; return uint32_t((memory::IsHandle(address + 220) ? uint32_t(memory::OwnerLoad(address + 220, 4)) : guest::LoadU32(address + 220))); }
  void set_applied_setting_91_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_91 = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 4, v); else guest::StoreU32(address + 220, v); }
  uint32_t applied_setting_91() const { if (auto* d = direct()) return d->applied_setting_91; return uint32_t((memory::IsHandle(address + 220) ? uint32_t(memory::OwnerLoad(address + 220, 4)) : guest::LoadU32(address + 220))); }
  void set_applied_setting_91(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_91 = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 4, v); else guest::StoreU32(address + 220, v); }
  // car byte +76
  uint32_t applied_setting_76_address() const { return address + 224; }
  uint32_t applied_setting_76_bits() const { if (auto* d = direct()) return d->applied_setting_76; return uint32_t((memory::IsHandle(address + 224) ? uint32_t(memory::OwnerLoad(address + 224, 4)) : guest::LoadU32(address + 224))); }
  void set_applied_setting_76_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_76 = v; return; } if (memory::IsHandle(address + 224)) memory::OwnerStore(address + 224, 4, v); else guest::StoreU32(address + 224, v); }
  uint32_t applied_setting_76() const { if (auto* d = direct()) return d->applied_setting_76; return uint32_t((memory::IsHandle(address + 224) ? uint32_t(memory::OwnerLoad(address + 224, 4)) : guest::LoadU32(address + 224))); }
  void set_applied_setting_76(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_76 = v; return; } if (memory::IsHandle(address + 224)) memory::OwnerStore(address + 224, 4, v); else guest::StoreU32(address + 224, v); }
  // car byte +77
  uint32_t applied_setting_77_address() const { return address + 228; }
  uint32_t applied_setting_77_bits() const { if (auto* d = direct()) return d->applied_setting_77; return uint32_t((memory::IsHandle(address + 228) ? uint32_t(memory::OwnerLoad(address + 228, 4)) : guest::LoadU32(address + 228))); }
  void set_applied_setting_77_bits(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_77 = v; return; } if (memory::IsHandle(address + 228)) memory::OwnerStore(address + 228, 4, v); else guest::StoreU32(address + 228, v); }
  uint32_t applied_setting_77() const { if (auto* d = direct()) return d->applied_setting_77; return uint32_t((memory::IsHandle(address + 228) ? uint32_t(memory::OwnerLoad(address + 228, 4)) : guest::LoadU32(address + 228))); }
  void set_applied_setting_77(uint32_t v) const { if (auto* d = direct()) { d->applied_setting_77 = v; return; } if (memory::IsHandle(address + 228)) memory::OwnerStore(address + 228, 4, v); else guest::StoreU32(address + 228, v); }
  // car +40
  uint32_t stock_gear_1_address() const { return address + 232; }
  uint32_t stock_gear_1_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_gear_1); return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(address + 232))); }
  void set_stock_gear_1_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_gear_1 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(address + 232, v); }
  float stock_gear_1() const { return std::bit_cast<float>(stock_gear_1_bits()); }
  void set_stock_gear_1(float v) const { set_stock_gear_1_bits(std::bit_cast<uint32_t>(v)); }
  // car +44
  uint32_t stock_gear_2_address() const { return address + 236; }
  uint32_t stock_gear_2_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_gear_2); return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_stock_gear_2_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_gear_2 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  float stock_gear_2() const { return std::bit_cast<float>(stock_gear_2_bits()); }
  void set_stock_gear_2(float v) const { set_stock_gear_2_bits(std::bit_cast<uint32_t>(v)); }
  // car +48
  uint32_t stock_gear_3_address() const { return address + 240; }
  uint32_t stock_gear_3_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_gear_3); return uint32_t((memory::IsHandle(address + 240) ? uint32_t(memory::OwnerLoad(address + 240, 4)) : guest::LoadU32(address + 240))); }
  void set_stock_gear_3_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_gear_3 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 240)) memory::OwnerStore(address + 240, 4, v); else guest::StoreU32(address + 240, v); }
  float stock_gear_3() const { return std::bit_cast<float>(stock_gear_3_bits()); }
  void set_stock_gear_3(float v) const { set_stock_gear_3_bits(std::bit_cast<uint32_t>(v)); }
  // car +52
  uint32_t stock_gear_4_address() const { return address + 244; }
  uint32_t stock_gear_4_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_gear_4); return uint32_t((memory::IsHandle(address + 244) ? uint32_t(memory::OwnerLoad(address + 244, 4)) : guest::LoadU32(address + 244))); }
  void set_stock_gear_4_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_gear_4 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 244)) memory::OwnerStore(address + 244, 4, v); else guest::StoreU32(address + 244, v); }
  float stock_gear_4() const { return std::bit_cast<float>(stock_gear_4_bits()); }
  void set_stock_gear_4(float v) const { set_stock_gear_4_bits(std::bit_cast<uint32_t>(v)); }
  // car +56
  uint32_t stock_gear_5_address() const { return address + 248; }
  uint32_t stock_gear_5_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_gear_5); return uint32_t((memory::IsHandle(address + 248) ? uint32_t(memory::OwnerLoad(address + 248, 4)) : guest::LoadU32(address + 248))); }
  void set_stock_gear_5_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_gear_5 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 248)) memory::OwnerStore(address + 248, 4, v); else guest::StoreU32(address + 248, v); }
  float stock_gear_5() const { return std::bit_cast<float>(stock_gear_5_bits()); }
  void set_stock_gear_5(float v) const { set_stock_gear_5_bits(std::bit_cast<uint32_t>(v)); }
  // car +60
  uint32_t stock_gear_6_address() const { return address + 252; }
  uint32_t stock_gear_6_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_gear_6); return uint32_t((memory::IsHandle(address + 252) ? uint32_t(memory::OwnerLoad(address + 252, 4)) : guest::LoadU32(address + 252))); }
  void set_stock_gear_6_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_gear_6 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 252)) memory::OwnerStore(address + 252, 4, v); else guest::StoreU32(address + 252, v); }
  float stock_gear_6() const { return std::bit_cast<float>(stock_gear_6_bits()); }
  void set_stock_gear_6(float v) const { set_stock_gear_6_bits(std::bit_cast<uint32_t>(v)); }
  // car +36
  uint32_t stock_final_drive_address() const { return address + 256; }
  uint32_t stock_final_drive_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->stock_final_drive); return uint32_t((memory::IsHandle(address + 256) ? uint32_t(memory::OwnerLoad(address + 256, 4)) : guest::LoadU32(address + 256))); }
  void set_stock_final_drive_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_final_drive = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 256)) memory::OwnerStore(address + 256, 4, v); else guest::StoreU32(address + 256, v); }
  float stock_final_drive() const { return std::bit_cast<float>(stock_final_drive_bits()); }
  void set_stock_final_drive(float v) const { set_stock_final_drive_bits(std::bit_cast<uint32_t>(v)); }
  // car byte +72
  uint32_t stock_setting_72_address() const { return address + 260; }
  uint32_t stock_setting_72_bits() const { if (auto* d = direct()) return d->stock_setting_72; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_stock_setting_72_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_72 = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  uint32_t stock_setting_72() const { if (auto* d = direct()) return d->stock_setting_72; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_stock_setting_72(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_72 = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  // car byte +73
  uint32_t stock_setting_73_address() const { return address + 264; }
  uint32_t stock_setting_73_bits() const { if (auto* d = direct()) return d->stock_setting_73; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_stock_setting_73_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_73 = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  uint32_t stock_setting_73() const { if (auto* d = direct()) return d->stock_setting_73; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_stock_setting_73(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_73 = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  // car byte +74
  uint32_t stock_setting_74_address() const { return address + 268; }
  uint32_t stock_setting_74_bits() const { if (auto* d = direct()) return d->stock_setting_74; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_stock_setting_74_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_74 = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  uint32_t stock_setting_74() const { if (auto* d = direct()) return d->stock_setting_74; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_stock_setting_74(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_74 = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  // car byte +75
  uint32_t stock_setting_75_address() const { return address + 272; }
  uint32_t stock_setting_75_bits() const { if (auto* d = direct()) return d->stock_setting_75; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_stock_setting_75_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_75 = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  uint32_t stock_setting_75() const { if (auto* d = direct()) return d->stock_setting_75; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_stock_setting_75(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_75 = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  // car byte +80 (probably the front camber)
  uint32_t stock_setting_80_address() const { return address + 276; }
  uint32_t stock_setting_80_bits() const { if (auto* d = direct()) return d->stock_setting_80; return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_stock_setting_80_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_80 = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  uint32_t stock_setting_80() const { if (auto* d = direct()) return d->stock_setting_80; return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_stock_setting_80(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_80 = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  // car byte +81 (probably the rear camber)
  uint32_t stock_setting_81_address() const { return address + 280; }
  uint32_t stock_setting_81_bits() const { if (auto* d = direct()) return d->stock_setting_81; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_stock_setting_81_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_81 = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  uint32_t stock_setting_81() const { if (auto* d = direct()) return d->stock_setting_81; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_stock_setting_81(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_81 = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  // car byte +78
  uint32_t stock_setting_78_address() const { return address + 284; }
  uint32_t stock_setting_78_bits() const { if (auto* d = direct()) return d->stock_setting_78; return uint32_t((memory::IsHandle(address + 284) ? uint32_t(memory::OwnerLoad(address + 284, 4)) : guest::LoadU32(address + 284))); }
  void set_stock_setting_78_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_78 = v; return; } if (memory::IsHandle(address + 284)) memory::OwnerStore(address + 284, 4, v); else guest::StoreU32(address + 284, v); }
  uint32_t stock_setting_78() const { if (auto* d = direct()) return d->stock_setting_78; return uint32_t((memory::IsHandle(address + 284) ? uint32_t(memory::OwnerLoad(address + 284, 4)) : guest::LoadU32(address + 284))); }
  void set_stock_setting_78(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_78 = v; return; } if (memory::IsHandle(address + 284)) memory::OwnerStore(address + 284, 4, v); else guest::StoreU32(address + 284, v); }
  // car byte +79
  uint32_t stock_setting_79_address() const { return address + 288; }
  uint32_t stock_setting_79_bits() const { if (auto* d = direct()) return d->stock_setting_79; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(address + 288))); }
  void set_stock_setting_79_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_79 = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(address + 288, v); }
  uint32_t stock_setting_79() const { if (auto* d = direct()) return d->stock_setting_79; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(address + 288))); }
  void set_stock_setting_79(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_79 = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(address + 288, v); }
  // car byte +82
  uint32_t stock_setting_82_address() const { return address + 292; }
  uint32_t stock_setting_82_bits() const { if (auto* d = direct()) return d->stock_setting_82; return uint32_t((memory::IsHandle(address + 292) ? uint32_t(memory::OwnerLoad(address + 292, 4)) : guest::LoadU32(address + 292))); }
  void set_stock_setting_82_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_82 = v; return; } if (memory::IsHandle(address + 292)) memory::OwnerStore(address + 292, 4, v); else guest::StoreU32(address + 292, v); }
  uint32_t stock_setting_82() const { if (auto* d = direct()) return d->stock_setting_82; return uint32_t((memory::IsHandle(address + 292) ? uint32_t(memory::OwnerLoad(address + 292, 4)) : guest::LoadU32(address + 292))); }
  void set_stock_setting_82(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_82 = v; return; } if (memory::IsHandle(address + 292)) memory::OwnerStore(address + 292, 4, v); else guest::StoreU32(address + 292, v); }
  // car byte +83
  uint32_t stock_setting_83_address() const { return address + 296; }
  uint32_t stock_setting_83_bits() const { if (auto* d = direct()) return d->stock_setting_83; return uint32_t((memory::IsHandle(address + 296) ? uint32_t(memory::OwnerLoad(address + 296, 4)) : guest::LoadU32(address + 296))); }
  void set_stock_setting_83_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_83 = v; return; } if (memory::IsHandle(address + 296)) memory::OwnerStore(address + 296, 4, v); else guest::StoreU32(address + 296, v); }
  uint32_t stock_setting_83() const { if (auto* d = direct()) return d->stock_setting_83; return uint32_t((memory::IsHandle(address + 296) ? uint32_t(memory::OwnerLoad(address + 296, 4)) : guest::LoadU32(address + 296))); }
  void set_stock_setting_83(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_83 = v; return; } if (memory::IsHandle(address + 296)) memory::OwnerStore(address + 296, 4, v); else guest::StoreU32(address + 296, v); }
  // car byte +84
  uint32_t stock_setting_84_address() const { return address + 300; }
  uint32_t stock_setting_84_bits() const { if (auto* d = direct()) return d->stock_setting_84; return uint32_t((memory::IsHandle(address + 300) ? uint32_t(memory::OwnerLoad(address + 300, 4)) : guest::LoadU32(address + 300))); }
  void set_stock_setting_84_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_84 = v; return; } if (memory::IsHandle(address + 300)) memory::OwnerStore(address + 300, 4, v); else guest::StoreU32(address + 300, v); }
  uint32_t stock_setting_84() const { if (auto* d = direct()) return d->stock_setting_84; return uint32_t((memory::IsHandle(address + 300) ? uint32_t(memory::OwnerLoad(address + 300, 4)) : guest::LoadU32(address + 300))); }
  void set_stock_setting_84(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_84 = v; return; } if (memory::IsHandle(address + 300)) memory::OwnerStore(address + 300, 4, v); else guest::StoreU32(address + 300, v); }
  // car byte +85
  uint32_t stock_setting_85_address() const { return address + 304; }
  uint32_t stock_setting_85_bits() const { if (auto* d = direct()) return d->stock_setting_85; return uint32_t((memory::IsHandle(address + 304) ? uint32_t(memory::OwnerLoad(address + 304, 4)) : guest::LoadU32(address + 304))); }
  void set_stock_setting_85_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_85 = v; return; } if (memory::IsHandle(address + 304)) memory::OwnerStore(address + 304, 4, v); else guest::StoreU32(address + 304, v); }
  uint32_t stock_setting_85() const { if (auto* d = direct()) return d->stock_setting_85; return uint32_t((memory::IsHandle(address + 304) ? uint32_t(memory::OwnerLoad(address + 304, 4)) : guest::LoadU32(address + 304))); }
  void set_stock_setting_85(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_85 = v; return; } if (memory::IsHandle(address + 304)) memory::OwnerStore(address + 304, 4, v); else guest::StoreU32(address + 304, v); }
  // car byte +86
  uint32_t stock_setting_86_address() const { return address + 308; }
  uint32_t stock_setting_86_bits() const { if (auto* d = direct()) return d->stock_setting_86; return uint32_t((memory::IsHandle(address + 308) ? uint32_t(memory::OwnerLoad(address + 308, 4)) : guest::LoadU32(address + 308))); }
  void set_stock_setting_86_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_86 = v; return; } if (memory::IsHandle(address + 308)) memory::OwnerStore(address + 308, 4, v); else guest::StoreU32(address + 308, v); }
  uint32_t stock_setting_86() const { if (auto* d = direct()) return d->stock_setting_86; return uint32_t((memory::IsHandle(address + 308) ? uint32_t(memory::OwnerLoad(address + 308, 4)) : guest::LoadU32(address + 308))); }
  void set_stock_setting_86(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_86 = v; return; } if (memory::IsHandle(address + 308)) memory::OwnerStore(address + 308, 4, v); else guest::StoreU32(address + 308, v); }
  // car byte +87
  uint32_t stock_setting_87_address() const { return address + 312; }
  uint32_t stock_setting_87_bits() const { if (auto* d = direct()) return d->stock_setting_87; return uint32_t((memory::IsHandle(address + 312) ? uint32_t(memory::OwnerLoad(address + 312, 4)) : guest::LoadU32(address + 312))); }
  void set_stock_setting_87_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_87 = v; return; } if (memory::IsHandle(address + 312)) memory::OwnerStore(address + 312, 4, v); else guest::StoreU32(address + 312, v); }
  uint32_t stock_setting_87() const { if (auto* d = direct()) return d->stock_setting_87; return uint32_t((memory::IsHandle(address + 312) ? uint32_t(memory::OwnerLoad(address + 312, 4)) : guest::LoadU32(address + 312))); }
  void set_stock_setting_87(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_87 = v; return; } if (memory::IsHandle(address + 312)) memory::OwnerStore(address + 312, 4, v); else guest::StoreU32(address + 312, v); }
  // car byte +88
  uint32_t stock_setting_88_address() const { return address + 316; }
  uint32_t stock_setting_88_bits() const { if (auto* d = direct()) return d->stock_setting_88; return uint32_t((memory::IsHandle(address + 316) ? uint32_t(memory::OwnerLoad(address + 316, 4)) : guest::LoadU32(address + 316))); }
  void set_stock_setting_88_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_88 = v; return; } if (memory::IsHandle(address + 316)) memory::OwnerStore(address + 316, 4, v); else guest::StoreU32(address + 316, v); }
  uint32_t stock_setting_88() const { if (auto* d = direct()) return d->stock_setting_88; return uint32_t((memory::IsHandle(address + 316) ? uint32_t(memory::OwnerLoad(address + 316, 4)) : guest::LoadU32(address + 316))); }
  void set_stock_setting_88(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_88 = v; return; } if (memory::IsHandle(address + 316)) memory::OwnerStore(address + 316, 4, v); else guest::StoreU32(address + 316, v); }
  // car byte +89
  uint32_t stock_setting_89_address() const { return address + 320; }
  uint32_t stock_setting_89_bits() const { if (auto* d = direct()) return d->stock_setting_89; return uint32_t((memory::IsHandle(address + 320) ? uint32_t(memory::OwnerLoad(address + 320, 4)) : guest::LoadU32(address + 320))); }
  void set_stock_setting_89_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_89 = v; return; } if (memory::IsHandle(address + 320)) memory::OwnerStore(address + 320, 4, v); else guest::StoreU32(address + 320, v); }
  uint32_t stock_setting_89() const { if (auto* d = direct()) return d->stock_setting_89; return uint32_t((memory::IsHandle(address + 320) ? uint32_t(memory::OwnerLoad(address + 320, 4)) : guest::LoadU32(address + 320))); }
  void set_stock_setting_89(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_89 = v; return; } if (memory::IsHandle(address + 320)) memory::OwnerStore(address + 320, 4, v); else guest::StoreU32(address + 320, v); }
  // car byte +94
  uint32_t stock_setting_94_address() const { return address + 324; }
  uint32_t stock_setting_94_bits() const { if (auto* d = direct()) return d->stock_setting_94; return uint32_t((memory::IsHandle(address + 324) ? uint32_t(memory::OwnerLoad(address + 324, 4)) : guest::LoadU32(address + 324))); }
  void set_stock_setting_94_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_94 = v; return; } if (memory::IsHandle(address + 324)) memory::OwnerStore(address + 324, 4, v); else guest::StoreU32(address + 324, v); }
  uint32_t stock_setting_94() const { if (auto* d = direct()) return d->stock_setting_94; return uint32_t((memory::IsHandle(address + 324) ? uint32_t(memory::OwnerLoad(address + 324, 4)) : guest::LoadU32(address + 324))); }
  void set_stock_setting_94(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_94 = v; return; } if (memory::IsHandle(address + 324)) memory::OwnerStore(address + 324, 4, v); else guest::StoreU32(address + 324, v); }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t stock_torque_share_address() const { return address + 328; }
  uint32_t stock_torque_share_bits() const { if (auto* d = direct()) return d->stock_torque_share; return uint32_t((memory::IsHandle(address + 328) ? uint32_t(memory::OwnerLoad(address + 328, 4)) : guest::LoadU32(address + 328))); }
  void set_stock_torque_share_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_torque_share = v; return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 4, v); else guest::StoreU32(address + 328, v); }
  uint32_t stock_torque_share() const { if (auto* d = direct()) return d->stock_torque_share; return uint32_t((memory::IsHandle(address + 328) ? uint32_t(memory::OwnerLoad(address + 328, 4)) : guest::LoadU32(address + 328))); }
  void set_stock_torque_share(uint32_t v) const { if (auto* d = direct()) { d->stock_torque_share = v; return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 4, v); else guest::StoreU32(address + 328, v); }
  // car byte +90
  uint32_t stock_setting_90_address() const { return address + 332; }
  uint32_t stock_setting_90_bits() const { if (auto* d = direct()) return d->stock_setting_90; return uint32_t((memory::IsHandle(address + 332) ? uint32_t(memory::OwnerLoad(address + 332, 4)) : guest::LoadU32(address + 332))); }
  void set_stock_setting_90_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_90 = v; return; } if (memory::IsHandle(address + 332)) memory::OwnerStore(address + 332, 4, v); else guest::StoreU32(address + 332, v); }
  uint32_t stock_setting_90() const { if (auto* d = direct()) return d->stock_setting_90; return uint32_t((memory::IsHandle(address + 332) ? uint32_t(memory::OwnerLoad(address + 332, 4)) : guest::LoadU32(address + 332))); }
  void set_stock_setting_90(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_90 = v; return; } if (memory::IsHandle(address + 332)) memory::OwnerStore(address + 332, 4, v); else guest::StoreU32(address + 332, v); }
  // car byte +91
  uint32_t stock_setting_91_address() const { return address + 336; }
  uint32_t stock_setting_91_bits() const { if (auto* d = direct()) return d->stock_setting_91; return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(address + 336))); }
  void set_stock_setting_91_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_91 = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(address + 336, v); }
  uint32_t stock_setting_91() const { if (auto* d = direct()) return d->stock_setting_91; return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(address + 336))); }
  void set_stock_setting_91(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_91 = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(address + 336, v); }
  // car byte +76
  uint32_t stock_setting_76_address() const { return address + 340; }
  uint32_t stock_setting_76_bits() const { if (auto* d = direct()) return d->stock_setting_76; return uint32_t((memory::IsHandle(address + 340) ? uint32_t(memory::OwnerLoad(address + 340, 4)) : guest::LoadU32(address + 340))); }
  void set_stock_setting_76_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_76 = v; return; } if (memory::IsHandle(address + 340)) memory::OwnerStore(address + 340, 4, v); else guest::StoreU32(address + 340, v); }
  uint32_t stock_setting_76() const { if (auto* d = direct()) return d->stock_setting_76; return uint32_t((memory::IsHandle(address + 340) ? uint32_t(memory::OwnerLoad(address + 340, 4)) : guest::LoadU32(address + 340))); }
  void set_stock_setting_76(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_76 = v; return; } if (memory::IsHandle(address + 340)) memory::OwnerStore(address + 340, 4, v); else guest::StoreU32(address + 340, v); }
  // car byte +77
  uint32_t stock_setting_77_address() const { return address + 344; }
  uint32_t stock_setting_77_bits() const { if (auto* d = direct()) return d->stock_setting_77; return uint32_t((memory::IsHandle(address + 344) ? uint32_t(memory::OwnerLoad(address + 344, 4)) : guest::LoadU32(address + 344))); }
  void set_stock_setting_77_bits(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_77 = v; return; } if (memory::IsHandle(address + 344)) memory::OwnerStore(address + 344, 4, v); else guest::StoreU32(address + 344, v); }
  uint32_t stock_setting_77() const { if (auto* d = direct()) return d->stock_setting_77; return uint32_t((memory::IsHandle(address + 344) ? uint32_t(memory::OwnerLoad(address + 344, 4)) : guest::LoadU32(address + 344))); }
  void set_stock_setting_77(uint32_t v) const { if (auto* d = direct()) { d->stock_setting_77 = v; return; } if (memory::IsHandle(address + 344)) memory::OwnerStore(address + 344, 4, v); else guest::StoreU32(address + 344, v); }
  // route 8's best times: five 12-byte rows {the course record entry's +20, its +0, 1 when there}; row 0's first word
  uint32_t route8_time_address() const { return address + 348; }
  uint32_t route8_time_bits() const { if (auto* d = direct()) return d->route8_time; return uint32_t((memory::IsHandle(address + 348) ? uint32_t(memory::OwnerLoad(address + 348, 4)) : guest::LoadU32(address + 348))); }
  void set_route8_time_bits(uint32_t v) const { if (auto* d = direct()) { d->route8_time = v; return; } if (memory::IsHandle(address + 348)) memory::OwnerStore(address + 348, 4, v); else guest::StoreU32(address + 348, v); }
  uint32_t route8_time() const { if (auto* d = direct()) return d->route8_time; return uint32_t((memory::IsHandle(address + 348) ? uint32_t(memory::OwnerLoad(address + 348, 4)) : guest::LoadU32(address + 348))); }
  void set_route8_time(uint32_t v) const { if (auto* d = direct()) { d->route8_time = v; return; } if (memory::IsHandle(address + 348)) memory::OwnerStore(address + 348, 4, v); else guest::StoreU32(address + 348, v); }
  // the entry's +0
  uint32_t route8_entry_0_address() const { return address + 352; }
  uint32_t route8_entry_0_bits() const { if (auto* d = direct()) return d->route8_entry_0; return uint32_t((memory::IsHandle(address + 352) ? uint32_t(memory::OwnerLoad(address + 352, 4)) : guest::LoadU32(address + 352))); }
  void set_route8_entry_0_bits(uint32_t v) const { if (auto* d = direct()) { d->route8_entry_0 = v; return; } if (memory::IsHandle(address + 352)) memory::OwnerStore(address + 352, 4, v); else guest::StoreU32(address + 352, v); }
  uint32_t route8_entry_0() const { if (auto* d = direct()) return d->route8_entry_0; return uint32_t((memory::IsHandle(address + 352) ? uint32_t(memory::OwnerLoad(address + 352, 4)) : guest::LoadU32(address + 352))); }
  void set_route8_entry_0(uint32_t v) const { if (auto* d = direct()) { d->route8_entry_0 = v; return; } if (memory::IsHandle(address + 352)) memory::OwnerStore(address + 352, 4, v); else guest::StoreU32(address + 352, v); }
  // 1 when there, 0 past the record's count
  uint32_t route8_present_address() const { return address + 356; }
  uint32_t route8_present_bits() const { if (auto* d = direct()) return d->route8_present; return uint32_t((memory::IsHandle(address + 356) ? uint32_t(memory::OwnerLoad(address + 356, 4)) : guest::LoadU32(address + 356))); }
  void set_route8_present_bits(uint32_t v) const { if (auto* d = direct()) { d->route8_present = v; return; } if (memory::IsHandle(address + 356)) memory::OwnerStore(address + 356, 4, v); else guest::StoreU32(address + 356, v); }
  uint32_t route8_present() const { if (auto* d = direct()) return d->route8_present; return uint32_t((memory::IsHandle(address + 356) ? uint32_t(memory::OwnerLoad(address + 356, 4)) : guest::LoadU32(address + 356))); }
  void set_route8_present(uint32_t v) const { if (auto* d = direct()) { d->route8_present = v; return; } if (memory::IsHandle(address + 356)) memory::OwnerStore(address + 356, 4, v); else guest::StoreU32(address + 356, v); }
  // rows 1 to 4
  uint32_t route8_rows_address() const { return address + 360; }
  uint8_t* route8_rows() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 360 : guest::Pointer(address + 360); }
  // route 9's rows, as route 8's
  uint32_t route9_time_address() const { return address + 408; }
  uint32_t route9_time_bits() const { if (auto* d = direct()) return d->route9_time; return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(address + 408))); }
  void set_route9_time_bits(uint32_t v) const { if (auto* d = direct()) { d->route9_time = v; return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(address + 408, v); }
  uint32_t route9_time() const { if (auto* d = direct()) return d->route9_time; return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(address + 408))); }
  void set_route9_time(uint32_t v) const { if (auto* d = direct()) { d->route9_time = v; return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(address + 408, v); }
  uint32_t route9_entry_0_address() const { return address + 412; }
  uint32_t route9_entry_0_bits() const { if (auto* d = direct()) return d->route9_entry_0; return uint32_t((memory::IsHandle(address + 412) ? uint32_t(memory::OwnerLoad(address + 412, 4)) : guest::LoadU32(address + 412))); }
  void set_route9_entry_0_bits(uint32_t v) const { if (auto* d = direct()) { d->route9_entry_0 = v; return; } if (memory::IsHandle(address + 412)) memory::OwnerStore(address + 412, 4, v); else guest::StoreU32(address + 412, v); }
  uint32_t route9_entry_0() const { if (auto* d = direct()) return d->route9_entry_0; return uint32_t((memory::IsHandle(address + 412) ? uint32_t(memory::OwnerLoad(address + 412, 4)) : guest::LoadU32(address + 412))); }
  void set_route9_entry_0(uint32_t v) const { if (auto* d = direct()) { d->route9_entry_0 = v; return; } if (memory::IsHandle(address + 412)) memory::OwnerStore(address + 412, 4, v); else guest::StoreU32(address + 412, v); }
  uint32_t route9_present_address() const { return address + 416; }
  uint32_t route9_present_bits() const { if (auto* d = direct()) return d->route9_present; return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(address + 416))); }
  void set_route9_present_bits(uint32_t v) const { if (auto* d = direct()) { d->route9_present = v; return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(address + 416, v); }
  uint32_t route9_present() const { if (auto* d = direct()) return d->route9_present; return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(address + 416))); }
  void set_route9_present(uint32_t v) const { if (auto* d = direct()) { d->route9_present = v; return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(address + 416, v); }
  uint32_t route9_rows_address() const { return address + 420; }
  uint8_t* route9_rows() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 420 : guest::Pointer(address + 420); }
  // route 10's rows, as route 8's
  uint32_t route10_time_address() const { return address + 468; }
  uint32_t route10_time_bits() const { if (auto* d = direct()) return d->route10_time; return uint32_t((memory::IsHandle(address + 468) ? uint32_t(memory::OwnerLoad(address + 468, 4)) : guest::LoadU32(address + 468))); }
  void set_route10_time_bits(uint32_t v) const { if (auto* d = direct()) { d->route10_time = v; return; } if (memory::IsHandle(address + 468)) memory::OwnerStore(address + 468, 4, v); else guest::StoreU32(address + 468, v); }
  uint32_t route10_time() const { if (auto* d = direct()) return d->route10_time; return uint32_t((memory::IsHandle(address + 468) ? uint32_t(memory::OwnerLoad(address + 468, 4)) : guest::LoadU32(address + 468))); }
  void set_route10_time(uint32_t v) const { if (auto* d = direct()) { d->route10_time = v; return; } if (memory::IsHandle(address + 468)) memory::OwnerStore(address + 468, 4, v); else guest::StoreU32(address + 468, v); }
  uint32_t route10_entry_0_address() const { return address + 472; }
  uint32_t route10_entry_0_bits() const { if (auto* d = direct()) return d->route10_entry_0; return uint32_t((memory::IsHandle(address + 472) ? uint32_t(memory::OwnerLoad(address + 472, 4)) : guest::LoadU32(address + 472))); }
  void set_route10_entry_0_bits(uint32_t v) const { if (auto* d = direct()) { d->route10_entry_0 = v; return; } if (memory::IsHandle(address + 472)) memory::OwnerStore(address + 472, 4, v); else guest::StoreU32(address + 472, v); }
  uint32_t route10_entry_0() const { if (auto* d = direct()) return d->route10_entry_0; return uint32_t((memory::IsHandle(address + 472) ? uint32_t(memory::OwnerLoad(address + 472, 4)) : guest::LoadU32(address + 472))); }
  void set_route10_entry_0(uint32_t v) const { if (auto* d = direct()) { d->route10_entry_0 = v; return; } if (memory::IsHandle(address + 472)) memory::OwnerStore(address + 472, 4, v); else guest::StoreU32(address + 472, v); }
  uint32_t route10_present_address() const { return address + 476; }
  uint32_t route10_present_bits() const { if (auto* d = direct()) return d->route10_present; return uint32_t((memory::IsHandle(address + 476) ? uint32_t(memory::OwnerLoad(address + 476, 4)) : guest::LoadU32(address + 476))); }
  void set_route10_present_bits(uint32_t v) const { if (auto* d = direct()) { d->route10_present = v; return; } if (memory::IsHandle(address + 476)) memory::OwnerStore(address + 476, 4, v); else guest::StoreU32(address + 476, v); }
  uint32_t route10_present() const { if (auto* d = direct()) return d->route10_present; return uint32_t((memory::IsHandle(address + 476) ? uint32_t(memory::OwnerLoad(address + 476, 4)) : guest::LoadU32(address + 476))); }
  void set_route10_present(uint32_t v) const { if (auto* d = direct()) { d->route10_present = v; return; } if (memory::IsHandle(address + 476)) memory::OwnerStore(address + 476, 4, v); else guest::StoreU32(address + 476, v); }
  uint32_t route10_rows_address() const { return address + 480; }
  uint8_t* route10_rows() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 480 : guest::Pointer(address + 480); }
  // the save's +8 to +27
  uint32_t player_name_address() const { return address + 528; }
  uint8_t* player_name() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 528 : guest::Pointer(address + 528); }
  // the save's +6116
  uint32_t money_address() const { return address + 548; }
  uint32_t money_bits() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 548) ? uint32_t(memory::OwnerLoad(address + 548, 4)) : guest::LoadU32(address + 548))); }
  void set_money_bits(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 548)) memory::OwnerStore(address + 548, 4, v); else guest::StoreU32(address + 548, v); }
  uint32_t money() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 548) ? uint32_t(memory::OwnerLoad(address + 548, 4)) : guest::LoadU32(address + 548))); }
  void set_money(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 548)) memory::OwnerStore(address + 548, 4, v); else guest::StoreU32(address + 548, v); }
  // the day mod 7
  uint32_t weekday_address() const { return address + 552; }
  uint32_t weekday_bits() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 552) ? uint32_t(memory::OwnerLoad(address + 552, 4)) : guest::LoadU32(address + 552))); }
  void set_weekday_bits(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 552)) memory::OwnerStore(address + 552, 4, v); else guest::StoreU32(address + 552, v); }
  uint32_t weekday() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 552) ? uint32_t(memory::OwnerLoad(address + 552, 4)) : guest::LoadU32(address + 552))); }
  void set_weekday(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 552)) memory::OwnerStore(address + 552, 4, v); else guest::StoreU32(address + 552, v); }
  // the save's +6100, at most 999
  uint32_t day_address() const { return address + 556; }
  uint32_t day_bits() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(address + 556))); }
  void set_day_bits(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(address + 556, v); }
  uint32_t day() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 556) ? uint32_t(memory::OwnerLoad(address + 556, 4)) : guest::LoadU32(address + 556))); }
  void set_day(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 556)) memory::OwnerStore(address + 556, 4, v); else guest::StoreU32(address + 556, v); }
  // 1 in the type 5 mode (no test run, no stand car)
  uint32_t type5_mode_address() const { return address + 560; }
  uint32_t type5_mode_bits() const { if (auto* d = direct()) return d->type5_mode; return uint32_t((memory::IsHandle(address + 560) ? uint32_t(memory::OwnerLoad(address + 560, 4)) : guest::LoadU32(address + 560))); }
  void set_type5_mode_bits(uint32_t v) const { if (auto* d = direct()) { d->type5_mode = v; return; } if (memory::IsHandle(address + 560)) memory::OwnerStore(address + 560, 4, v); else guest::StoreU32(address + 560, v); }
  uint32_t type5_mode() const { if (auto* d = direct()) return d->type5_mode; return uint32_t((memory::IsHandle(address + 560) ? uint32_t(memory::OwnerLoad(address + 560, 4)) : guest::LoadU32(address + 560))); }
  void set_type5_mode(uint32_t v) const { if (auto* d = direct()) { d->type5_mode = v; return; } if (memory::IsHandle(address + 560)) memory::OwnerStore(address + 560, 4, v); else guest::StoreU32(address + 560, v); }
  // 5, or 4 in the type 5 mode (probably the top menu's item count)
  uint32_t top_items_address() const { return address + 564; }
  uint32_t top_items_bits() const { if (auto* d = direct()) return d->top_items; return uint32_t((memory::IsHandle(address + 564) ? uint32_t(memory::OwnerLoad(address + 564, 4)) : guest::LoadU32(address + 564))); }
  void set_top_items_bits(uint32_t v) const { if (auto* d = direct()) { d->top_items = v; return; } if (memory::IsHandle(address + 564)) memory::OwnerStore(address + 564, 4, v); else guest::StoreU32(address + 564, v); }
  uint32_t top_items() const { if (auto* d = direct()) return d->top_items; return uint32_t((memory::IsHandle(address + 564) ? uint32_t(memory::OwnerLoad(address + 564, 4)) : guest::LoadU32(address + 564))); }
  void set_top_items(uint32_t v) const { if (auto* d = direct()) { d->top_items = v; return; } if (memory::IsHandle(address + 564)) memory::OwnerStore(address + 564, 4, v); else guest::StoreU32(address + 564, v); }
  // the test run's course picked, 0 to 3
  uint32_t course_address() const { return address + 568; }
  uint32_t course_bits() const { if (auto* d = direct()) return d->course; return uint32_t((memory::IsHandle(address + 568) ? uint32_t(memory::OwnerLoad(address + 568, 4)) : guest::LoadU32(address + 568))); }
  void set_course_bits(uint32_t v) const { if (auto* d = direct()) { d->course = v; return; } if (memory::IsHandle(address + 568)) memory::OwnerStore(address + 568, 4, v); else guest::StoreU32(address + 568, v); }
  uint32_t course() const { if (auto* d = direct()) return d->course; return uint32_t((memory::IsHandle(address + 568) ? uint32_t(memory::OwnerLoad(address + 568, 4)) : guest::LoadU32(address + 568))); }
  void set_course(uint32_t v) const { if (auto* d = direct()) { d->course = v; return; } if (memory::IsHandle(address + 568)) memory::OwnerStore(address + 568, 4, v); else guest::StoreU32(address + 568, v); }
  // 0 yes, 1 no, 2 back
  uint32_t dialog_answer_address() const { return address + 572; }
  uint32_t dialog_answer_bits() const { if (auto* d = direct()) return d->dialog_answer; return uint32_t((memory::IsHandle(address + 572) ? uint32_t(memory::OwnerLoad(address + 572, 4)) : guest::LoadU32(address + 572))); }
  void set_dialog_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->dialog_answer = v; return; } if (memory::IsHandle(address + 572)) memory::OwnerStore(address + 572, 4, v); else guest::StoreU32(address + 572, v); }
  uint32_t dialog_answer() const { if (auto* d = direct()) return d->dialog_answer; return uint32_t((memory::IsHandle(address + 572) ? uint32_t(memory::OwnerLoad(address + 572, 4)) : guest::LoadU32(address + 572))); }
  void set_dialog_answer(uint32_t v) const { if (auto* d = direct()) { d->dialog_answer = v; return; } if (memory::IsHandle(address + 572)) memory::OwnerStore(address + 572, 4, v); else guest::StoreU32(address + 572, v); }
  // 1 once a test run is set up (Destroy then leaves the state at 11)
  uint32_t test_run_set_up_address() const { return address + 576; }
  uint32_t test_run_set_up_bits() const { if (auto* d = direct()) return d->test_run_set_up; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(address + 576))); }
  void set_test_run_set_up_bits(uint32_t v) const { if (auto* d = direct()) { d->test_run_set_up = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(address + 576, v); }
  uint32_t test_run_set_up() const { if (auto* d = direct()) return d->test_run_set_up; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(address + 576))); }
  void set_test_run_set_up(uint32_t v) const { if (auto* d = direct()) { d->test_run_set_up = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(address + 576, v); }
  // 1 to start at the courses (ReenterToCourses)
  uint32_t start_at_courses_address() const { return address + 580; }
  uint32_t start_at_courses_bits() const { if (auto* d = direct()) return d->start_at_courses; return uint32_t((memory::IsHandle(address + 580) ? uint32_t(memory::OwnerLoad(address + 580, 4)) : guest::LoadU32(address + 580))); }
  void set_start_at_courses_bits(uint32_t v) const { if (auto* d = direct()) { d->start_at_courses = v; return; } if (memory::IsHandle(address + 580)) memory::OwnerStore(address + 580, 4, v); else guest::StoreU32(address + 580, v); }
  uint32_t start_at_courses() const { if (auto* d = direct()) return d->start_at_courses; return uint32_t((memory::IsHandle(address + 580) ? uint32_t(memory::OwnerLoad(address + 580, 4)) : guest::LoadU32(address + 580))); }
  void set_start_at_courses(uint32_t v) const { if (auto* d = direct()) { d->start_at_courses = v; return; } if (memory::IsHandle(address + 580)) memory::OwnerStore(address + 580, 4, v); else guest::StoreU32(address + 580, v); }
  // the tuning's +32 nonzero
  uint32_t allows_gears_address() const { return address + 584; }
  uint32_t allows_gears_bits() const { if (auto* d = direct()) return d->allows_gears; return uint32_t((memory::IsHandle(address + 584) ? uint32_t(memory::OwnerLoad(address + 584, 4)) : guest::LoadU32(address + 584))); }
  void set_allows_gears_bits(uint32_t v) const { if (auto* d = direct()) { d->allows_gears = v; return; } if (memory::IsHandle(address + 584)) memory::OwnerStore(address + 584, 4, v); else guest::StoreU32(address + 584, v); }
  uint32_t allows_gears() const { if (auto* d = direct()) return d->allows_gears; return uint32_t((memory::IsHandle(address + 584) ? uint32_t(memory::OwnerLoad(address + 584, 4)) : guest::LoadU32(address + 584))); }
  void set_allows_gears(uint32_t v) const { if (auto* d = direct()) { d->allows_gears = v; return; } if (memory::IsHandle(address + 584)) memory::OwnerStore(address + 584, 4, v); else guest::StoreU32(address + 584, v); }
  // the tuning's +56 nonzero
  uint32_t allows_pages_9_10_address() const { return address + 588; }
  uint32_t allows_pages_9_10_bits() const { if (auto* d = direct()) return d->allows_pages_9_10; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(address + 588))); }
  void set_allows_pages_9_10_bits(uint32_t v) const { if (auto* d = direct()) { d->allows_pages_9_10 = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(address + 588, v); }
  uint32_t allows_pages_9_10() const { if (auto* d = direct()) return d->allows_pages_9_10; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(address + 588))); }
  void set_allows_pages_9_10(uint32_t v) const { if (auto* d = direct()) { d->allows_pages_9_10 = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(address + 588, v); }
  // the tuning's +40 nonzero
  uint32_t allows_pages_11_16_address() const { return address + 592; }
  uint32_t allows_pages_11_16_bits() const { if (auto* d = direct()) return d->allows_pages_11_16; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(address + 592))); }
  void set_allows_pages_11_16_bits(uint32_t v) const { if (auto* d = direct()) { d->allows_pages_11_16 = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(address + 592, v); }
  uint32_t allows_pages_11_16() const { if (auto* d = direct()) return d->allows_pages_11_16; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(address + 592))); }
  void set_allows_pages_11_16(uint32_t v) const { if (auto* d = direct()) { d->allows_pages_11_16 = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(address + 592, v); }
  // the aero part's bit
  uint32_t allows_page_17_address() const { return address + 596; }
  uint32_t allows_page_17_bits() const { if (auto* d = direct()) return d->allows_page_17; return uint32_t((memory::IsHandle(address + 596) ? uint32_t(memory::OwnerLoad(address + 596, 4)) : guest::LoadU32(address + 596))); }
  void set_allows_page_17_bits(uint32_t v) const { if (auto* d = direct()) { d->allows_page_17 = v; return; } if (memory::IsHandle(address + 596)) memory::OwnerStore(address + 596, 4, v); else guest::StoreU32(address + 596, v); }
  uint32_t allows_page_17() const { if (auto* d = direct()) return d->allows_page_17; return uint32_t((memory::IsHandle(address + 596) ? uint32_t(memory::OwnerLoad(address + 596, 4)) : guest::LoadU32(address + 596))); }
  void set_allows_page_17(uint32_t v) const { if (auto* d = direct()) { d->allows_page_17 = v; return; } if (memory::IsHandle(address + 596)) memory::OwnerStore(address + 596, 4, v); else guest::StoreU32(address + 596, v); }
  // spec block 2's +100
  uint32_t spec_block2_100_address() const { return address + 600; }
  uint32_t spec_block2_100_bits() const { if (auto* d = direct()) return d->spec_block2_100; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(address + 600))); }
  void set_spec_block2_100_bits(uint32_t v) const { if (auto* d = direct()) { d->spec_block2_100 = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(address + 600, v); }
  uint32_t spec_block2_100() const { if (auto* d = direct()) return d->spec_block2_100; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(address + 600))); }
  void set_spec_block2_100(uint32_t v) const { if (auto* d = direct()) { d->spec_block2_100 = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(address + 600, v); }
  // seven 12-byte bounds of the gear ratios {min, max, ...}
  uint32_t gear_bounds_address() const { return address + 604; }
  uint8_t* gear_bounds() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 604 : guest::Pointer(address + 604); }
  // block 2's byte +130 (probably the drive type; 4 both ends)
  uint32_t drive_type_address() const { return address + 688; }
  uint32_t drive_type_bits() const { if (auto* d = direct()) return d->drive_type; return uint32_t((memory::IsHandle(address + 688) ? uint32_t(memory::OwnerLoad(address + 688, 4)) : guest::LoadU32(address + 688))); }
  void set_drive_type_bits(uint32_t v) const { if (auto* d = direct()) { d->drive_type = v; return; } if (memory::IsHandle(address + 688)) memory::OwnerStore(address + 688, 4, v); else guest::StoreU32(address + 688, v); }
  uint32_t drive_type() const { if (auto* d = direct()) return d->drive_type; return uint32_t((memory::IsHandle(address + 688) ? uint32_t(memory::OwnerLoad(address + 688, 4)) : guest::LoadU32(address + 688))); }
  void set_drive_type(uint32_t v) const { if (auto* d = direct()) { d->drive_type = v; return; } if (memory::IsHandle(address + 688)) memory::OwnerStore(address + 688, 4, v); else guest::StoreU32(address + 688, v); }
  // the tuning's +52
  uint32_t tuning_52_address() const { return address + 692; }
  uint32_t tuning_52_bits() const { if (auto* d = direct()) return d->tuning_52; return uint32_t((memory::IsHandle(address + 692) ? uint32_t(memory::OwnerLoad(address + 692, 4)) : guest::LoadU32(address + 692))); }
  void set_tuning_52_bits(uint32_t v) const { if (auto* d = direct()) { d->tuning_52 = v; return; } if (memory::IsHandle(address + 692)) memory::OwnerStore(address + 692, 4, v); else guest::StoreU32(address + 692, v); }
  uint32_t tuning_52() const { if (auto* d = direct()) return d->tuning_52; return uint32_t((memory::IsHandle(address + 692) ? uint32_t(memory::OwnerLoad(address + 692, 4)) : guest::LoadU32(address + 692))); }
  void set_tuning_52(uint32_t v) const { if (auto* d = direct()) { d->tuning_52 = v; return; } if (memory::IsHandle(address + 692)) memory::OwnerStore(address + 692, 4, v); else guest::StoreU32(address + 692, v); }
  // the car data's +388 (the gear page's speeds)
  uint32_t car_data_388_address() const { return address + 696; }
  uint32_t car_data_388_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->car_data_388); return uint32_t((memory::IsHandle(address + 696) ? uint32_t(memory::OwnerLoad(address + 696, 4)) : guest::LoadU32(address + 696))); }
  void set_car_data_388_bits(uint32_t v) const { if (auto* d = direct()) { d->car_data_388 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 696)) memory::OwnerStore(address + 696, 4, v); else guest::StoreU32(address + 696, v); }
  float car_data_388() const { return std::bit_cast<float>(car_data_388_bits()); }
  void set_car_data_388(float v) const { set_car_data_388_bits(std::bit_cast<uint32_t>(v)); }
  // the car data's +1280 (the gear page's speeds)
  uint32_t car_data_1280_address() const { return address + 700; }
  uint32_t car_data_1280_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->car_data_1280); return uint32_t((memory::IsHandle(address + 700) ? uint32_t(memory::OwnerLoad(address + 700, 4)) : guest::LoadU32(address + 700))); }
  void set_car_data_1280_bits(uint32_t v) const { if (auto* d = direct()) { d->car_data_1280 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 700)) memory::OwnerStore(address + 700, 4, v); else guest::StoreU32(address + 700, v); }
  float car_data_1280() const { return std::bit_cast<float>(car_data_1280_bits()); }
  void set_car_data_1280(float v) const { set_car_data_1280_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_704_address() const { return address + 704; }
  uint32_t unknown_704_bits() const { if (auto* d = direct()) return d->unknown_704; return uint32_t((memory::IsHandle(address + 704) ? uint32_t(memory::OwnerLoad(address + 704, 4)) : guest::LoadU32(address + 704))); }
  void set_unknown_704_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_704 = v; return; } if (memory::IsHandle(address + 704)) memory::OwnerStore(address + 704, 4, v); else guest::StoreU32(address + 704, v); }
  uint32_t unknown_704() const { if (auto* d = direct()) return d->unknown_704; return uint32_t((memory::IsHandle(address + 704) ? uint32_t(memory::OwnerLoad(address + 704, 4)) : guest::LoadU32(address + 704))); }
  void set_unknown_704(uint32_t v) const { if (auto* d = direct()) { d->unknown_704 = v; return; } if (memory::IsHandle(address + 704)) memory::OwnerStore(address + 704, 4, v); else guest::StoreU32(address + 704, v); }
  // a word stack
  uint32_t stack_address() const { return address + 708; }
  uint8_t* stack() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 708 : guest::Pointer(address + 708); }
  uint32_t stack_count_address() const { return address + 728; }
  uint32_t stack_count_bits() const { if (auto* d = direct()) return d->stack_count; return uint32_t((memory::IsHandle(address + 728) ? uint32_t(memory::OwnerLoad(address + 728, 4)) : guest::LoadU32(address + 728))); }
  void set_stack_count_bits(uint32_t v) const { if (auto* d = direct()) { d->stack_count = v; return; } if (memory::IsHandle(address + 728)) memory::OwnerStore(address + 728, 4, v); else guest::StoreU32(address + 728, v); }
  uint32_t stack_count() const { if (auto* d = direct()) return d->stack_count; return uint32_t((memory::IsHandle(address + 728) ? uint32_t(memory::OwnerLoad(address + 728, 4)) : guest::LoadU32(address + 728))); }
  void set_stack_count(uint32_t v) const { if (auto* d = direct()) { d->stack_count = v; return; } if (memory::IsHandle(address + 728)) memory::OwnerStore(address + 728, 4, v); else guest::StoreU32(address + 728, v); }
  // the apply item's choice: 0 apply, 1 the car's, 2 stock
  uint32_t apply_choice_address() const { return address + 732; }
  uint32_t apply_choice_bits() const { if (auto* d = direct()) return d->apply_choice; return uint32_t((memory::IsHandle(address + 732) ? uint32_t(memory::OwnerLoad(address + 732, 4)) : guest::LoadU32(address + 732))); }
  void set_apply_choice_bits(uint32_t v) const { if (auto* d = direct()) { d->apply_choice = v; return; } if (memory::IsHandle(address + 732)) memory::OwnerStore(address + 732, 4, v); else guest::StoreU32(address + 732, v); }
  uint32_t apply_choice() const { if (auto* d = direct()) return d->apply_choice; return uint32_t((memory::IsHandle(address + 732) ? uint32_t(memory::OwnerLoad(address + 732, 4)) : guest::LoadU32(address + 732))); }
  void set_apply_choice(uint32_t v) const { if (auto* d = direct()) { d->apply_choice = v; return; } if (memory::IsHandle(address + 732)) memory::OwnerStore(address + 732, 4, v); else guest::StoreU32(address + 732, v); }
  uint32_t previous_apply_choice_address() const { return address + 736; }
  uint32_t previous_apply_choice_bits() const { if (auto* d = direct()) return d->previous_apply_choice; return uint32_t((memory::IsHandle(address + 736) ? uint32_t(memory::OwnerLoad(address + 736, 4)) : guest::LoadU32(address + 736))); }
  void set_previous_apply_choice_bits(uint32_t v) const { if (auto* d = direct()) { d->previous_apply_choice = v; return; } if (memory::IsHandle(address + 736)) memory::OwnerStore(address + 736, 4, v); else guest::StoreU32(address + 736, v); }
  uint32_t previous_apply_choice() const { if (auto* d = direct()) return d->previous_apply_choice; return uint32_t((memory::IsHandle(address + 736) ? uint32_t(memory::OwnerLoad(address + 736, 4)) : guest::LoadU32(address + 736))); }
  void set_previous_apply_choice(uint32_t v) const { if (auto* d = direct()) { d->previous_apply_choice = v; return; } if (memory::IsHandle(address + 736)) memory::OwnerStore(address + 736, 4, v); else guest::StoreU32(address + 736, v); }
  // 0 at Fill
  uint32_t unknown_740_address() const { return address + 740; }
  uint32_t unknown_740_bits() const { if (auto* d = direct()) return d->unknown_740; return uint32_t((memory::IsHandle(address + 740) ? uint32_t(memory::OwnerLoad(address + 740, 4)) : guest::LoadU32(address + 740))); }
  void set_unknown_740_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_740 = v; return; } if (memory::IsHandle(address + 740)) memory::OwnerStore(address + 740, 4, v); else guest::StoreU32(address + 740, v); }
  uint32_t unknown_740() const { if (auto* d = direct()) return d->unknown_740; return uint32_t((memory::IsHandle(address + 740) ? uint32_t(memory::OwnerLoad(address + 740, 4)) : guest::LoadU32(address + 740))); }
  void set_unknown_740(uint32_t v) const { if (auto* d = direct()) { d->unknown_740 = v; return; } if (memory::IsHandle(address + 740)) memory::OwnerStore(address + 740, 4, v); else guest::StoreU32(address + 740, v); }
  // Start: 0 when starting at the courses, else 1
  uint32_t unknown_744_address() const { return address + 744; }
  uint32_t unknown_744_bits() const { if (auto* d = direct()) return d->unknown_744; return uint32_t((memory::IsHandle(address + 744) ? uint32_t(memory::OwnerLoad(address + 744, 4)) : guest::LoadU32(address + 744))); }
  void set_unknown_744_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_744 = v; return; } if (memory::IsHandle(address + 744)) memory::OwnerStore(address + 744, 4, v); else guest::StoreU32(address + 744, v); }
  uint32_t unknown_744() const { if (auto* d = direct()) return d->unknown_744; return uint32_t((memory::IsHandle(address + 744) ? uint32_t(memory::OwnerLoad(address + 744, 4)) : guest::LoadU32(address + 744))); }
  void set_unknown_744(uint32_t v) const { if (auto* d = direct()) { d->unknown_744 = v; return; } if (memory::IsHandle(address + 744)) memory::OwnerStore(address + 744, 4, v); else guest::StoreU32(address + 744, v); }
  // 0 at Create, 1 at Fill
  uint32_t unknown_748_address() const { return address + 748; }
  uint32_t unknown_748_bits() const { if (auto* d = direct()) return d->unknown_748; return uint32_t((memory::IsHandle(address + 748) ? uint32_t(memory::OwnerLoad(address + 748, 4)) : guest::LoadU32(address + 748))); }
  void set_unknown_748_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_748 = v; return; } if (memory::IsHandle(address + 748)) memory::OwnerStore(address + 748, 4, v); else guest::StoreU32(address + 748, v); }
  uint32_t unknown_748() const { if (auto* d = direct()) return d->unknown_748; return uint32_t((memory::IsHandle(address + 748) ? uint32_t(memory::OwnerLoad(address + 748, 4)) : guest::LoadU32(address + 748))); }
  void set_unknown_748(uint32_t v) const { if (auto* d = direct()) { d->unknown_748 = v; return; } if (memory::IsHandle(address + 748)) memory::OwnerStore(address + 748, 4, v); else guest::StoreU32(address + 748, v); }
  // the type 5 mode's location - 14, else 3 (Fill's SetText: string 369 + it)
  uint32_t location_address() const { return address + 752; }
  uint32_t location_bits() const { if (auto* d = direct()) return d->location; return uint32_t((memory::IsHandle(address + 752) ? uint32_t(memory::OwnerLoad(address + 752, 4)) : guest::LoadU32(address + 752))); }
  void set_location_bits(uint32_t v) const { if (auto* d = direct()) { d->location = v; return; } if (memory::IsHandle(address + 752)) memory::OwnerStore(address + 752, 4, v); else guest::StoreU32(address + 752, v); }
  uint32_t location() const { if (auto* d = direct()) return d->location; return uint32_t((memory::IsHandle(address + 752) ? uint32_t(memory::OwnerLoad(address + 752, 4)) : guest::LoadU32(address + 752))); }
  void set_location(uint32_t v) const { if (auto* d = direct()) { d->location = v; return; } if (memory::IsHandle(address + 752)) memory::OwnerStore(address + 752, 4, v); else guest::StoreU32(address + 752, v); }
  // 0 at Fill
  uint32_t unknown_756_address() const { return address + 756; }
  uint32_t unknown_756_bits() const { if (auto* d = direct()) return d->unknown_756; return uint32_t((memory::IsHandle(address + 756) ? uint32_t(memory::OwnerLoad(address + 756, 4)) : guest::LoadU32(address + 756))); }
  void set_unknown_756_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_756 = v; return; } if (memory::IsHandle(address + 756)) memory::OwnerStore(address + 756, 4, v); else guest::StoreU32(address + 756, v); }
  uint32_t unknown_756() const { if (auto* d = direct()) return d->unknown_756; return uint32_t((memory::IsHandle(address + 756) ? uint32_t(memory::OwnerLoad(address + 756, 4)) : guest::LoadU32(address + 756))); }
  void set_unknown_756(uint32_t v) const { if (auto* d = direct()) { d->unknown_756 = v; return; } if (memory::IsHandle(address + 756)) memory::OwnerStore(address + 756, 4, v); else guest::StoreU32(address + 756, v); }
  // 21 at Create, 10 at a test run
  uint32_t unknown_760_address() const { return address + 760; }
  uint32_t unknown_760_bits() const { if (auto* d = direct()) return d->unknown_760; return uint32_t((memory::IsHandle(address + 760) ? uint32_t(memory::OwnerLoad(address + 760, 4)) : guest::LoadU32(address + 760))); }
  void set_unknown_760_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_760 = v; return; } if (memory::IsHandle(address + 760)) memory::OwnerStore(address + 760, 4, v); else guest::StoreU32(address + 760, v); }
  uint32_t unknown_760() const { if (auto* d = direct()) return d->unknown_760; return uint32_t((memory::IsHandle(address + 760) ? uint32_t(memory::OwnerLoad(address + 760, 4)) : guest::LoadU32(address + 760))); }
  void set_unknown_760(uint32_t v) const { if (auto* d = direct()) { d->unknown_760 = v; return; } if (memory::IsHandle(address + 760)) memory::OwnerStore(address + 760, 4, v); else guest::StoreU32(address + 760, v); }
};

// embedded; measure: CourseRecord::Entry.r4, CourseRecords::Of.ret
struct CourseRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 408;
  // ten halfword pairs: the entries' links (+0 the next, +2 the entry's index)
  uint32_t links_address() const { return address + 0; }
  uint16_t unknown_40() const { return guest::LoadU16(address + 40); }
  void set_unknown_40(uint16_t v) const { guest::StoreU16(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint16_t unknown_42() const { return guest::LoadU16(address + 42); }
  void set_unknown_42(uint16_t v) const { guest::StoreU16(address + 42, v); }
  uint32_t unknown_42_address() const { return address + 42; }
  // the first link (the ranks followed from it)
  uint16_t first() const { return guest::LoadU16(address + 44); }
  void set_first(uint16_t v) const { guest::StoreU16(address + 44, v); }
  uint32_t first_address() const { return address + 44; }
  uint16_t unknown_46() const { return guest::LoadU16(address + 46); }
  void set_unknown_46(uint16_t v) const { guest::StoreU16(address + 46, v); }
  uint32_t unknown_46_address() const { return address + 46; }
  // probably the entries' count
  uint16_t count() const { return guest::LoadU16(address + 48); }
  void set_count(uint16_t v) const { guest::StoreU16(address + 48, v); }
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
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // probably the time: the best time rows' first word
  uint32_t time() const { return guest::LoadU32(address + 20); }
  void set_time(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t time_address() const { return address + 20; }
};

// The image words docs/functions/car-settings.md's code uses.
struct CarSettingsGlobals {
  template <typename T = PageObject>
  static T pages() { return T(guest::LoadU32(0x826D8458u)); }
  static void set_pages(const View& v) { guest::StoreU32(0x826D8458u, v.address); }
  static void set_pages(uint32_t v) { guest::StoreU32(0x826D8458u, v); }
  static constexpr uint32_t pages_address() { return 0x826D8458u; }
  // a front-end menu (+16 = 4 at Create)
  template <typename T = FrontEndMenu>
  static T menu() { return T(guest::LoadU32(0x826D845Cu)); }
  static void set_menu(const View& v) { guest::StoreU32(0x826D845Cu, v.address); }
  static void set_menu(uint32_t v) { guest::StoreU32(0x826D845Cu, v); }
  static constexpr uint32_t menu_address() { return 0x826D845Cu; }
  template <typename T = CarSettingsData>
  static T data() { return T(guest::LoadU32(0x826D8460u)); }
  static void set_data(const View& v) { guest::StoreU32(0x826D8460u, v.address); }
  static void set_data(uint32_t v) { guest::StoreU32(0x826D8460u, v); }
  static constexpr uint32_t data_address() { return 0x826D8460u; }
};

}  // namespace schema
