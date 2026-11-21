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
struct CarSettingsMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint32_t message_count() const { return guest::LoadU32(base, address + 804); }
  void set_message_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_address() const { return address + 804; }
  // the ring's first slot
  uint32_t message_start() const { return guest::LoadU32(base, address + 808); }
  void set_message_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t message_start_address() const { return address + 808; }
};

// allocated; measure: new<764>@CarSettings::Create
struct CarSettingsData : View {
  using View::View;
  static constexpr uint32_t kSize = 764;
  // car +40
  float working_gear_1() const { return guest::LoadFloat(base, address + 0); }
  void set_working_gear_1(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t working_gear_1_address() const { return address + 0; }
  // car +44
  float working_gear_2() const { return guest::LoadFloat(base, address + 4); }
  void set_working_gear_2(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t working_gear_2_address() const { return address + 4; }
  // car +48
  float working_gear_3() const { return guest::LoadFloat(base, address + 8); }
  void set_working_gear_3(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t working_gear_3_address() const { return address + 8; }
  // car +52
  float working_gear_4() const { return guest::LoadFloat(base, address + 12); }
  void set_working_gear_4(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t working_gear_4_address() const { return address + 12; }
  // car +56
  float working_gear_5() const { return guest::LoadFloat(base, address + 16); }
  void set_working_gear_5(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t working_gear_5_address() const { return address + 16; }
  // car +60
  float working_gear_6() const { return guest::LoadFloat(base, address + 20); }
  void set_working_gear_6(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t working_gear_6_address() const { return address + 20; }
  // car +36
  float working_final_drive() const { return guest::LoadFloat(base, address + 24); }
  void set_working_final_drive(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t working_final_drive_address() const { return address + 24; }
  // car byte +72
  uint32_t working_setting_72() const { return guest::LoadU32(base, address + 28); }
  void set_working_setting_72(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t working_setting_72_address() const { return address + 28; }
  // car byte +73
  uint32_t working_setting_73() const { return guest::LoadU32(base, address + 32); }
  void set_working_setting_73(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t working_setting_73_address() const { return address + 32; }
  // car byte +74
  uint32_t working_setting_74() const { return guest::LoadU32(base, address + 36); }
  void set_working_setting_74(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t working_setting_74_address() const { return address + 36; }
  // car byte +75
  uint32_t working_setting_75() const { return guest::LoadU32(base, address + 40); }
  void set_working_setting_75(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t working_setting_75_address() const { return address + 40; }
  // car byte +80 (probably the front camber)
  uint32_t working_setting_80() const { return guest::LoadU32(base, address + 44); }
  void set_working_setting_80(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t working_setting_80_address() const { return address + 44; }
  // car byte +81 (probably the rear camber)
  uint32_t working_setting_81() const { return guest::LoadU32(base, address + 48); }
  void set_working_setting_81(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t working_setting_81_address() const { return address + 48; }
  // car byte +78
  uint32_t working_setting_78() const { return guest::LoadU32(base, address + 52); }
  void set_working_setting_78(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t working_setting_78_address() const { return address + 52; }
  // car byte +79
  uint32_t working_setting_79() const { return guest::LoadU32(base, address + 56); }
  void set_working_setting_79(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t working_setting_79_address() const { return address + 56; }
  // car byte +82
  uint32_t working_setting_82() const { return guest::LoadU32(base, address + 60); }
  void set_working_setting_82(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t working_setting_82_address() const { return address + 60; }
  // car byte +83
  uint32_t working_setting_83() const { return guest::LoadU32(base, address + 64); }
  void set_working_setting_83(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t working_setting_83_address() const { return address + 64; }
  // car byte +84
  uint32_t working_setting_84() const { return guest::LoadU32(base, address + 68); }
  void set_working_setting_84(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t working_setting_84_address() const { return address + 68; }
  // car byte +85
  uint32_t working_setting_85() const { return guest::LoadU32(base, address + 72); }
  void set_working_setting_85(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t working_setting_85_address() const { return address + 72; }
  // car byte +86
  uint32_t working_setting_86() const { return guest::LoadU32(base, address + 76); }
  void set_working_setting_86(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t working_setting_86_address() const { return address + 76; }
  // car byte +87
  uint32_t working_setting_87() const { return guest::LoadU32(base, address + 80); }
  void set_working_setting_87(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t working_setting_87_address() const { return address + 80; }
  // car byte +88
  uint32_t working_setting_88() const { return guest::LoadU32(base, address + 84); }
  void set_working_setting_88(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t working_setting_88_address() const { return address + 84; }
  // car byte +89
  uint32_t working_setting_89() const { return guest::LoadU32(base, address + 88); }
  void set_working_setting_89(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t working_setting_89_address() const { return address + 88; }
  // car byte +94
  uint32_t working_setting_94() const { return guest::LoadU32(base, address + 92); }
  void set_working_setting_94(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t working_setting_94_address() const { return address + 92; }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t working_torque_share() const { return guest::LoadU32(base, address + 96); }
  void set_working_torque_share(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t working_torque_share_address() const { return address + 96; }
  // car byte +90
  uint32_t working_setting_90() const { return guest::LoadU32(base, address + 100); }
  void set_working_setting_90(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t working_setting_90_address() const { return address + 100; }
  // car byte +91
  uint32_t working_setting_91() const { return guest::LoadU32(base, address + 104); }
  void set_working_setting_91(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t working_setting_91_address() const { return address + 104; }
  // car byte +76
  uint32_t working_setting_76() const { return guest::LoadU32(base, address + 108); }
  void set_working_setting_76(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t working_setting_76_address() const { return address + 108; }
  // car byte +77
  uint32_t working_setting_77() const { return guest::LoadU32(base, address + 112); }
  void set_working_setting_77(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t working_setting_77_address() const { return address + 112; }
  // car +40
  float applied_gear_1() const { return guest::LoadFloat(base, address + 116); }
  void set_applied_gear_1(float v) const { guest::StoreFloat(base, address + 116, v); }
  uint32_t applied_gear_1_address() const { return address + 116; }
  // car +44
  float applied_gear_2() const { return guest::LoadFloat(base, address + 120); }
  void set_applied_gear_2(float v) const { guest::StoreFloat(base, address + 120, v); }
  uint32_t applied_gear_2_address() const { return address + 120; }
  // car +48
  float applied_gear_3() const { return guest::LoadFloat(base, address + 124); }
  void set_applied_gear_3(float v) const { guest::StoreFloat(base, address + 124, v); }
  uint32_t applied_gear_3_address() const { return address + 124; }
  // car +52
  float applied_gear_4() const { return guest::LoadFloat(base, address + 128); }
  void set_applied_gear_4(float v) const { guest::StoreFloat(base, address + 128, v); }
  uint32_t applied_gear_4_address() const { return address + 128; }
  // car +56
  float applied_gear_5() const { return guest::LoadFloat(base, address + 132); }
  void set_applied_gear_5(float v) const { guest::StoreFloat(base, address + 132, v); }
  uint32_t applied_gear_5_address() const { return address + 132; }
  // car +60
  float applied_gear_6() const { return guest::LoadFloat(base, address + 136); }
  void set_applied_gear_6(float v) const { guest::StoreFloat(base, address + 136, v); }
  uint32_t applied_gear_6_address() const { return address + 136; }
  // car +36
  float applied_final_drive() const { return guest::LoadFloat(base, address + 140); }
  void set_applied_final_drive(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t applied_final_drive_address() const { return address + 140; }
  // car byte +72
  uint32_t applied_setting_72() const { return guest::LoadU32(base, address + 144); }
  void set_applied_setting_72(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t applied_setting_72_address() const { return address + 144; }
  // car byte +73
  uint32_t applied_setting_73() const { return guest::LoadU32(base, address + 148); }
  void set_applied_setting_73(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t applied_setting_73_address() const { return address + 148; }
  // car byte +74
  uint32_t applied_setting_74() const { return guest::LoadU32(base, address + 152); }
  void set_applied_setting_74(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t applied_setting_74_address() const { return address + 152; }
  // car byte +75
  uint32_t applied_setting_75() const { return guest::LoadU32(base, address + 156); }
  void set_applied_setting_75(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t applied_setting_75_address() const { return address + 156; }
  // car byte +80 (probably the front camber)
  uint32_t applied_setting_80() const { return guest::LoadU32(base, address + 160); }
  void set_applied_setting_80(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t applied_setting_80_address() const { return address + 160; }
  // car byte +81 (probably the rear camber)
  uint32_t applied_setting_81() const { return guest::LoadU32(base, address + 164); }
  void set_applied_setting_81(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t applied_setting_81_address() const { return address + 164; }
  // car byte +78
  uint32_t applied_setting_78() const { return guest::LoadU32(base, address + 168); }
  void set_applied_setting_78(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t applied_setting_78_address() const { return address + 168; }
  // car byte +79
  uint32_t applied_setting_79() const { return guest::LoadU32(base, address + 172); }
  void set_applied_setting_79(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t applied_setting_79_address() const { return address + 172; }
  // car byte +82
  uint32_t applied_setting_82() const { return guest::LoadU32(base, address + 176); }
  void set_applied_setting_82(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t applied_setting_82_address() const { return address + 176; }
  // car byte +83
  uint32_t applied_setting_83() const { return guest::LoadU32(base, address + 180); }
  void set_applied_setting_83(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t applied_setting_83_address() const { return address + 180; }
  // car byte +84
  uint32_t applied_setting_84() const { return guest::LoadU32(base, address + 184); }
  void set_applied_setting_84(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t applied_setting_84_address() const { return address + 184; }
  // car byte +85
  uint32_t applied_setting_85() const { return guest::LoadU32(base, address + 188); }
  void set_applied_setting_85(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t applied_setting_85_address() const { return address + 188; }
  // car byte +86
  uint32_t applied_setting_86() const { return guest::LoadU32(base, address + 192); }
  void set_applied_setting_86(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t applied_setting_86_address() const { return address + 192; }
  // car byte +87
  uint32_t applied_setting_87() const { return guest::LoadU32(base, address + 196); }
  void set_applied_setting_87(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t applied_setting_87_address() const { return address + 196; }
  // car byte +88
  uint32_t applied_setting_88() const { return guest::LoadU32(base, address + 200); }
  void set_applied_setting_88(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t applied_setting_88_address() const { return address + 200; }
  // car byte +89
  uint32_t applied_setting_89() const { return guest::LoadU32(base, address + 204); }
  void set_applied_setting_89(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t applied_setting_89_address() const { return address + 204; }
  // car byte +94
  uint32_t applied_setting_94() const { return guest::LoadU32(base, address + 208); }
  void set_applied_setting_94(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t applied_setting_94_address() const { return address + 208; }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t applied_torque_share() const { return guest::LoadU32(base, address + 212); }
  void set_applied_torque_share(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t applied_torque_share_address() const { return address + 212; }
  // car byte +90
  uint32_t applied_setting_90() const { return guest::LoadU32(base, address + 216); }
  void set_applied_setting_90(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t applied_setting_90_address() const { return address + 216; }
  // car byte +91
  uint32_t applied_setting_91() const { return guest::LoadU32(base, address + 220); }
  void set_applied_setting_91(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t applied_setting_91_address() const { return address + 220; }
  // car byte +76
  uint32_t applied_setting_76() const { return guest::LoadU32(base, address + 224); }
  void set_applied_setting_76(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t applied_setting_76_address() const { return address + 224; }
  // car byte +77
  uint32_t applied_setting_77() const { return guest::LoadU32(base, address + 228); }
  void set_applied_setting_77(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t applied_setting_77_address() const { return address + 228; }
  // car +40
  float stock_gear_1() const { return guest::LoadFloat(base, address + 232); }
  void set_stock_gear_1(float v) const { guest::StoreFloat(base, address + 232, v); }
  uint32_t stock_gear_1_address() const { return address + 232; }
  // car +44
  float stock_gear_2() const { return guest::LoadFloat(base, address + 236); }
  void set_stock_gear_2(float v) const { guest::StoreFloat(base, address + 236, v); }
  uint32_t stock_gear_2_address() const { return address + 236; }
  // car +48
  float stock_gear_3() const { return guest::LoadFloat(base, address + 240); }
  void set_stock_gear_3(float v) const { guest::StoreFloat(base, address + 240, v); }
  uint32_t stock_gear_3_address() const { return address + 240; }
  // car +52
  float stock_gear_4() const { return guest::LoadFloat(base, address + 244); }
  void set_stock_gear_4(float v) const { guest::StoreFloat(base, address + 244, v); }
  uint32_t stock_gear_4_address() const { return address + 244; }
  // car +56
  float stock_gear_5() const { return guest::LoadFloat(base, address + 248); }
  void set_stock_gear_5(float v) const { guest::StoreFloat(base, address + 248, v); }
  uint32_t stock_gear_5_address() const { return address + 248; }
  // car +60
  float stock_gear_6() const { return guest::LoadFloat(base, address + 252); }
  void set_stock_gear_6(float v) const { guest::StoreFloat(base, address + 252, v); }
  uint32_t stock_gear_6_address() const { return address + 252; }
  // car +36
  float stock_final_drive() const { return guest::LoadFloat(base, address + 256); }
  void set_stock_final_drive(float v) const { guest::StoreFloat(base, address + 256, v); }
  uint32_t stock_final_drive_address() const { return address + 256; }
  // car byte +72
  uint32_t stock_setting_72() const { return guest::LoadU32(base, address + 260); }
  void set_stock_setting_72(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t stock_setting_72_address() const { return address + 260; }
  // car byte +73
  uint32_t stock_setting_73() const { return guest::LoadU32(base, address + 264); }
  void set_stock_setting_73(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t stock_setting_73_address() const { return address + 264; }
  // car byte +74
  uint32_t stock_setting_74() const { return guest::LoadU32(base, address + 268); }
  void set_stock_setting_74(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t stock_setting_74_address() const { return address + 268; }
  // car byte +75
  uint32_t stock_setting_75() const { return guest::LoadU32(base, address + 272); }
  void set_stock_setting_75(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t stock_setting_75_address() const { return address + 272; }
  // car byte +80 (probably the front camber)
  uint32_t stock_setting_80() const { return guest::LoadU32(base, address + 276); }
  void set_stock_setting_80(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t stock_setting_80_address() const { return address + 276; }
  // car byte +81 (probably the rear camber)
  uint32_t stock_setting_81() const { return guest::LoadU32(base, address + 280); }
  void set_stock_setting_81(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t stock_setting_81_address() const { return address + 280; }
  // car byte +78
  uint32_t stock_setting_78() const { return guest::LoadU32(base, address + 284); }
  void set_stock_setting_78(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t stock_setting_78_address() const { return address + 284; }
  // car byte +79
  uint32_t stock_setting_79() const { return guest::LoadU32(base, address + 288); }
  void set_stock_setting_79(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t stock_setting_79_address() const { return address + 288; }
  // car byte +82
  uint32_t stock_setting_82() const { return guest::LoadU32(base, address + 292); }
  void set_stock_setting_82(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t stock_setting_82_address() const { return address + 292; }
  // car byte +83
  uint32_t stock_setting_83() const { return guest::LoadU32(base, address + 296); }
  void set_stock_setting_83(uint32_t v) const { guest::StoreU32(base, address + 296, v); }
  uint32_t stock_setting_83_address() const { return address + 296; }
  // car byte +84
  uint32_t stock_setting_84() const { return guest::LoadU32(base, address + 300); }
  void set_stock_setting_84(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t stock_setting_84_address() const { return address + 300; }
  // car byte +85
  uint32_t stock_setting_85() const { return guest::LoadU32(base, address + 304); }
  void set_stock_setting_85(uint32_t v) const { guest::StoreU32(base, address + 304, v); }
  uint32_t stock_setting_85_address() const { return address + 304; }
  // car byte +86
  uint32_t stock_setting_86() const { return guest::LoadU32(base, address + 308); }
  void set_stock_setting_86(uint32_t v) const { guest::StoreU32(base, address + 308, v); }
  uint32_t stock_setting_86_address() const { return address + 308; }
  // car byte +87
  uint32_t stock_setting_87() const { return guest::LoadU32(base, address + 312); }
  void set_stock_setting_87(uint32_t v) const { guest::StoreU32(base, address + 312, v); }
  uint32_t stock_setting_87_address() const { return address + 312; }
  // car byte +88
  uint32_t stock_setting_88() const { return guest::LoadU32(base, address + 316); }
  void set_stock_setting_88(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t stock_setting_88_address() const { return address + 316; }
  // car byte +89
  uint32_t stock_setting_89() const { return guest::LoadU32(base, address + 320); }
  void set_stock_setting_89(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t stock_setting_89_address() const { return address + 320; }
  // car byte +94
  uint32_t stock_setting_94() const { return guest::LoadU32(base, address + 324); }
  void set_stock_setting_94(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t stock_setting_94_address() const { return address + 324; }
  // car byte +93 times 5 (probably the rear's torque share in percent)
  uint32_t stock_torque_share() const { return guest::LoadU32(base, address + 328); }
  void set_stock_torque_share(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t stock_torque_share_address() const { return address + 328; }
  // car byte +90
  uint32_t stock_setting_90() const { return guest::LoadU32(base, address + 332); }
  void set_stock_setting_90(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t stock_setting_90_address() const { return address + 332; }
  // car byte +91
  uint32_t stock_setting_91() const { return guest::LoadU32(base, address + 336); }
  void set_stock_setting_91(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t stock_setting_91_address() const { return address + 336; }
  // car byte +76
  uint32_t stock_setting_76() const { return guest::LoadU32(base, address + 340); }
  void set_stock_setting_76(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t stock_setting_76_address() const { return address + 340; }
  // car byte +77
  uint32_t stock_setting_77() const { return guest::LoadU32(base, address + 344); }
  void set_stock_setting_77(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t stock_setting_77_address() const { return address + 344; }
  // route 8's best times: five 12-byte rows {the course record entry's +20, its +0, 1 when there}; row 0's first word
  uint32_t route8_time() const { return guest::LoadU32(base, address + 348); }
  void set_route8_time(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t route8_time_address() const { return address + 348; }
  // the entry's +0
  uint32_t route8_entry_0() const { return guest::LoadU32(base, address + 352); }
  void set_route8_entry_0(uint32_t v) const { guest::StoreU32(base, address + 352, v); }
  uint32_t route8_entry_0_address() const { return address + 352; }
  // 1 when there, 0 past the record's count
  uint32_t route8_present() const { return guest::LoadU32(base, address + 356); }
  void set_route8_present(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t route8_present_address() const { return address + 356; }
  // rows 1 to 4
  uint32_t route8_rows_address() const { return address + 360; }
  // route 9's rows, as route 8's
  uint32_t route9_time() const { return guest::LoadU32(base, address + 408); }
  void set_route9_time(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t route9_time_address() const { return address + 408; }
  uint32_t route9_entry_0() const { return guest::LoadU32(base, address + 412); }
  void set_route9_entry_0(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t route9_entry_0_address() const { return address + 412; }
  uint32_t route9_present() const { return guest::LoadU32(base, address + 416); }
  void set_route9_present(uint32_t v) const { guest::StoreU32(base, address + 416, v); }
  uint32_t route9_present_address() const { return address + 416; }
  uint32_t route9_rows_address() const { return address + 420; }
  // route 10's rows, as route 8's
  uint32_t route10_time() const { return guest::LoadU32(base, address + 468); }
  void set_route10_time(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t route10_time_address() const { return address + 468; }
  uint32_t route10_entry_0() const { return guest::LoadU32(base, address + 472); }
  void set_route10_entry_0(uint32_t v) const { guest::StoreU32(base, address + 472, v); }
  uint32_t route10_entry_0_address() const { return address + 472; }
  uint32_t route10_present() const { return guest::LoadU32(base, address + 476); }
  void set_route10_present(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t route10_present_address() const { return address + 476; }
  uint32_t route10_rows_address() const { return address + 480; }
  // the save's +8 to +27
  uint32_t player_name_address() const { return address + 528; }
  // the save's +6116
  uint32_t money() const { return guest::LoadU32(base, address + 548); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 548, v); }
  uint32_t money_address() const { return address + 548; }
  // the day mod 7
  uint32_t weekday() const { return guest::LoadU32(base, address + 552); }
  void set_weekday(uint32_t v) const { guest::StoreU32(base, address + 552, v); }
  uint32_t weekday_address() const { return address + 552; }
  // the save's +6100, at most 999
  uint32_t day() const { return guest::LoadU32(base, address + 556); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 556, v); }
  uint32_t day_address() const { return address + 556; }
  // 1 in the type 5 mode (no test run, no stand car)
  uint32_t type5_mode() const { return guest::LoadU32(base, address + 560); }
  void set_type5_mode(uint32_t v) const { guest::StoreU32(base, address + 560, v); }
  uint32_t type5_mode_address() const { return address + 560; }
  // 5, or 4 in the type 5 mode (probably the top menu's item count)
  uint32_t top_items() const { return guest::LoadU32(base, address + 564); }
  void set_top_items(uint32_t v) const { guest::StoreU32(base, address + 564, v); }
  uint32_t top_items_address() const { return address + 564; }
  // the test run's course picked, 0 to 3
  uint32_t course() const { return guest::LoadU32(base, address + 568); }
  void set_course(uint32_t v) const { guest::StoreU32(base, address + 568, v); }
  uint32_t course_address() const { return address + 568; }
  // 0 yes, 1 no, 2 back
  uint32_t dialog_answer() const { return guest::LoadU32(base, address + 572); }
  void set_dialog_answer(uint32_t v) const { guest::StoreU32(base, address + 572, v); }
  uint32_t dialog_answer_address() const { return address + 572; }
  // 1 once a test run is set up (Destroy then leaves the state at 11)
  uint32_t test_run_set_up() const { return guest::LoadU32(base, address + 576); }
  void set_test_run_set_up(uint32_t v) const { guest::StoreU32(base, address + 576, v); }
  uint32_t test_run_set_up_address() const { return address + 576; }
  // 1 to start at the courses (ReenterToCourses)
  uint32_t start_at_courses() const { return guest::LoadU32(base, address + 580); }
  void set_start_at_courses(uint32_t v) const { guest::StoreU32(base, address + 580, v); }
  uint32_t start_at_courses_address() const { return address + 580; }
  // the tuning's +32 nonzero
  uint32_t allows_gears() const { return guest::LoadU32(base, address + 584); }
  void set_allows_gears(uint32_t v) const { guest::StoreU32(base, address + 584, v); }
  uint32_t allows_gears_address() const { return address + 584; }
  // the tuning's +56 nonzero
  uint32_t allows_pages_9_10() const { return guest::LoadU32(base, address + 588); }
  void set_allows_pages_9_10(uint32_t v) const { guest::StoreU32(base, address + 588, v); }
  uint32_t allows_pages_9_10_address() const { return address + 588; }
  // the tuning's +40 nonzero
  uint32_t allows_pages_11_16() const { return guest::LoadU32(base, address + 592); }
  void set_allows_pages_11_16(uint32_t v) const { guest::StoreU32(base, address + 592, v); }
  uint32_t allows_pages_11_16_address() const { return address + 592; }
  // the aero part's bit
  uint32_t allows_page_17() const { return guest::LoadU32(base, address + 596); }
  void set_allows_page_17(uint32_t v) const { guest::StoreU32(base, address + 596, v); }
  uint32_t allows_page_17_address() const { return address + 596; }
  // spec block 2's +100
  uint32_t spec_block2_100() const { return guest::LoadU32(base, address + 600); }
  void set_spec_block2_100(uint32_t v) const { guest::StoreU32(base, address + 600, v); }
  uint32_t spec_block2_100_address() const { return address + 600; }
  // seven 12-byte bounds of the gear ratios {min, max, ...}
  uint32_t gear_bounds_address() const { return address + 604; }
  // block 2's byte +130 (probably the drive type; 4 both ends)
  uint32_t drive_type() const { return guest::LoadU32(base, address + 688); }
  void set_drive_type(uint32_t v) const { guest::StoreU32(base, address + 688, v); }
  uint32_t drive_type_address() const { return address + 688; }
  // the tuning's +52
  uint32_t tuning_52() const { return guest::LoadU32(base, address + 692); }
  void set_tuning_52(uint32_t v) const { guest::StoreU32(base, address + 692, v); }
  uint32_t tuning_52_address() const { return address + 692; }
  // the car data's +388 (the gear page's speeds)
  float car_data_388() const { return guest::LoadFloat(base, address + 696); }
  void set_car_data_388(float v) const { guest::StoreFloat(base, address + 696, v); }
  uint32_t car_data_388_address() const { return address + 696; }
  // the car data's +1280 (the gear page's speeds)
  float car_data_1280() const { return guest::LoadFloat(base, address + 700); }
  void set_car_data_1280(float v) const { guest::StoreFloat(base, address + 700, v); }
  uint32_t car_data_1280_address() const { return address + 700; }
  uint32_t unknown_704() const { return guest::LoadU32(base, address + 704); }
  void set_unknown_704(uint32_t v) const { guest::StoreU32(base, address + 704, v); }
  uint32_t unknown_704_address() const { return address + 704; }
  // a word stack
  uint32_t stack_address() const { return address + 708; }
  uint32_t stack_count() const { return guest::LoadU32(base, address + 728); }
  void set_stack_count(uint32_t v) const { guest::StoreU32(base, address + 728, v); }
  uint32_t stack_count_address() const { return address + 728; }
  // the apply item's choice: 0 apply, 1 the car's, 2 stock
  uint32_t apply_choice() const { return guest::LoadU32(base, address + 732); }
  void set_apply_choice(uint32_t v) const { guest::StoreU32(base, address + 732, v); }
  uint32_t apply_choice_address() const { return address + 732; }
  uint32_t previous_apply_choice() const { return guest::LoadU32(base, address + 736); }
  void set_previous_apply_choice(uint32_t v) const { guest::StoreU32(base, address + 736, v); }
  uint32_t previous_apply_choice_address() const { return address + 736; }
  // 0 at Fill
  uint32_t unknown_740() const { return guest::LoadU32(base, address + 740); }
  void set_unknown_740(uint32_t v) const { guest::StoreU32(base, address + 740, v); }
  uint32_t unknown_740_address() const { return address + 740; }
  // Start: 0 when starting at the courses, else 1
  uint32_t unknown_744() const { return guest::LoadU32(base, address + 744); }
  void set_unknown_744(uint32_t v) const { guest::StoreU32(base, address + 744, v); }
  uint32_t unknown_744_address() const { return address + 744; }
  // 0 at Create, 1 at Fill
  uint32_t unknown_748() const { return guest::LoadU32(base, address + 748); }
  void set_unknown_748(uint32_t v) const { guest::StoreU32(base, address + 748, v); }
  uint32_t unknown_748_address() const { return address + 748; }
  // the type 5 mode's location - 14, else 3 (Fill's SetText: string 369 + it)
  uint32_t location() const { return guest::LoadU32(base, address + 752); }
  void set_location(uint32_t v) const { guest::StoreU32(base, address + 752, v); }
  uint32_t location_address() const { return address + 752; }
  // 0 at Fill
  uint32_t unknown_756() const { return guest::LoadU32(base, address + 756); }
  void set_unknown_756(uint32_t v) const { guest::StoreU32(base, address + 756, v); }
  uint32_t unknown_756_address() const { return address + 756; }
  // 21 at Create, 10 at a test run
  uint32_t unknown_760() const { return guest::LoadU32(base, address + 760); }
  void set_unknown_760(uint32_t v) const { guest::StoreU32(base, address + 760, v); }
  uint32_t unknown_760_address() const { return address + 760; }
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
