// The views of docs/functions/options-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/options-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Options
struct OptionsMachine : View {
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
  // 1 once movie 7 is loaded (WaitForMovie)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: OnEnter and WaitForMovie go to state 9 (open on its message)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // set: Options::Close deletes movie 7
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// embedded; measure: Options::Close.r4, Options::Leave.r4, Options::OnClosed.r4, Options::Show.r4
#pragma pack(push, 1)
struct OptionsDispatcherData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(OptionsDispatcherData) == 812);
inline constexpr memory::LayoutField kOptionsDispatcherFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kOptionsDispatcherLayout = {kOptionsDispatcherFields, 2};

struct OptionsDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  OptionsDispatcherData* data() const { return static_cast<OptionsDispatcherData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static OptionsDispatcher New(uint8_t* base) {
    return OptionsDispatcher(base, memory::MakeHandle(new OptionsDispatcherData(), sizeof(OptionsDispatcherData), "OptionsDispatcher", &kOptionsDispatcherLayout));
  }
  template <uint32_t Stride = kSize>
  static OptionsDispatcher NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(OptionsDispatcherData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<OptionsDispatcherData*>(::operator new(sizeof(OptionsDispatcherData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) OptionsDispatcherData();
    return OptionsDispatcher(base, memory::MakeHandle(p, sizeof(OptionsDispatcherData) * count, "OptionsDispatcher", &kOptionsDispatcherLayout, sizeof(OptionsDispatcherData)));
  }
  OptionsDispatcher at(uint32_t i) const { return OptionsDispatcher(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  OptionsDispatcherData* direct() const { return static_cast<OptionsDispatcherData*>(memory::HandleOwn(address, &kOptionsDispatcherLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kOptionsDispatcherLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOptionsDispatcherLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(base, address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count_bits() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count_bits(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t message_count() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start_bits() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start_bits(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t message_start() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
};

// allocated; measure: Settings, Settings.+-31624, new<68>@Options::OnEnter.+4.+-31620, Settings.+-31596
struct OptionsSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 716;
  // 1 once Options::SetUpMovie ran
  uint32_t set_up() const { return guest::LoadU32(base, address + 0); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_up_address() const { return address + 0; }
  // from the mode type: 2 -> 1, 4 -> 0, 6 -> 2, 7 -> 3, 8 -> 4
  uint32_t context() const { return guest::LoadU32(base, address + 4); }
  void set_context(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t context_address() const { return address + 4; }
  // save +4, or bit 5 of save +6092
  uint32_t music_preview_shown() const { return guest::LoadU32(base, address + 8); }
  void set_music_preview_shown(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t music_preview_shown_address() const { return address + 8; }
  // save +8 + 4i (probably the player's name; type 2 only)
  uint32_t name_0() const { return guest::LoadU32(base, address + 12); }
  void set_name_0(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t name_0_address() const { return address + 12; }
  uint32_t name_1() const { return guest::LoadU32(base, address + 16); }
  void set_name_1(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t name_1_address() const { return address + 16; }
  uint32_t name_2() const { return guest::LoadU32(base, address + 20); }
  void set_name_2(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t name_2_address() const { return address + 20; }
  uint32_t name_3() const { return guest::LoadU32(base, address + 24); }
  void set_name_3(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t name_3_address() const { return address + 24; }
  uint32_t name_4() const { return guest::LoadU32(base, address + 28); }
  void set_name_4(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t name_4_address() const { return address + 28; }
  // save +6116 (type 2 only)
  uint32_t money() const { return guest::LoadU32(base, address + 32); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t money_address() const { return address + 32; }
  // save +6100 mod 7 (probably; type 2 only)
  uint32_t day_of_week() const { return guest::LoadU32(base, address + 36); }
  void set_day_of_week(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t day_of_week_address() const { return address + 36; }
  // save +6100 capped at 999 (probably; type 2 only)
  uint32_t day() const { return guest::LoadU32(base, address + 40); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t day_address() const { return address + 40; }
  // the current settings: the 192-byte settings block named word by word below
  uint32_t current_block_address() const { return address + 44; }
  // the HUD's four on/off values (save options +0 to +12)
  uint32_t current_hud_0() const { return guest::LoadU32(base, address + 44); }
  void set_current_hud_0(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t current_hud_0_address() const { return address + 44; }
  uint32_t current_hud_1() const { return guest::LoadU32(base, address + 48); }
  void set_current_hud_1(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t current_hud_1_address() const { return address + 48; }
  uint32_t current_hud_2() const { return guest::LoadU32(base, address + 52); }
  void set_current_hud_2(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t current_hud_2_address() const { return address + 52; }
  uint32_t current_hud_3() const { return guest::LoadU32(base, address + 56); }
  void set_current_hud_3(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t current_hud_3_address() const { return address + 56; }
  // 0 to 15 (save options byte +28)
  uint32_t current_music_volume() const { return guest::LoadU32(base, address + 60); }
  void set_current_music_volume(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t current_music_volume_address() const { return address + 60; }
  // 0 to 15 (save options byte +29)
  uint32_t current_effects_volume() const { return guest::LoadU32(base, address + 64); }
  void set_current_effects_volume(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t current_effects_volume_address() const { return address + 64; }
  // -10 to 10 (save options bytes +36 to +38)
  uint32_t current_slider_0() const { return guest::LoadU32(base, address + 68); }
  void set_current_slider_0(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t current_slider_0_address() const { return address + 68; }
  uint32_t current_slider_1() const { return guest::LoadU32(base, address + 72); }
  void set_current_slider_1(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t current_slider_1_address() const { return address + 72; }
  uint32_t current_slider_2() const { return guest::LoadU32(base, address + 76); }
  void set_current_slider_2(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t current_slider_2_address() const { return address + 76; }
  // player 1's (save options bytes +39 to +41)
  uint32_t current_p1_slider_0() const { return guest::LoadU32(base, address + 80); }
  void set_current_p1_slider_0(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t current_p1_slider_0_address() const { return address + 80; }
  uint32_t current_p1_slider_1() const { return guest::LoadU32(base, address + 84); }
  void set_current_p1_slider_1(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t current_p1_slider_1_address() const { return address + 84; }
  uint32_t current_p1_slider_2() const { return guest::LoadU32(base, address + 88); }
  void set_current_p1_slider_2(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t current_p1_slider_2_address() const { return address + 88; }
  // button indexes in menu order (save options record +44)
  uint32_t current_button_0() const { return guest::LoadU32(base, address + 92); }
  void set_current_button_0(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t current_button_0_address() const { return address + 92; }
  uint32_t current_button_1() const { return guest::LoadU32(base, address + 96); }
  void set_current_button_1(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t current_button_1_address() const { return address + 96; }
  uint32_t current_button_2() const { return guest::LoadU32(base, address + 100); }
  void set_current_button_2(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t current_button_2_address() const { return address + 100; }
  uint32_t current_button_3() const { return guest::LoadU32(base, address + 104); }
  void set_current_button_3(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t current_button_3_address() const { return address + 104; }
  uint32_t current_button_4() const { return guest::LoadU32(base, address + 108); }
  void set_current_button_4(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t current_button_4_address() const { return address + 108; }
  uint32_t current_button_5() const { return guest::LoadU32(base, address + 112); }
  void set_current_button_5(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t current_button_5_address() const { return address + 112; }
  uint32_t current_button_6() const { return guest::LoadU32(base, address + 116); }
  void set_current_button_6(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t current_button_6_address() const { return address + 116; }
  uint32_t current_button_7() const { return guest::LoadU32(base, address + 120); }
  void set_current_button_7(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t current_button_7_address() const { return address + 120; }
  uint32_t current_button_8() const { return guest::LoadU32(base, address + 124); }
  void set_current_button_8(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t current_button_8_address() const { return address + 124; }
  uint32_t current_button_9() const { return guest::LoadU32(base, address + 128); }
  void set_current_button_9(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t current_button_9_address() const { return address + 128; }
  uint32_t current_button_10() const { return guest::LoadU32(base, address + 132); }
  void set_current_button_10(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t current_button_10_address() const { return address + 132; }
  uint32_t current_button_11() const { return guest::LoadU32(base, address + 136); }
  void set_current_button_11(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t current_button_11_address() const { return address + 136; }
  // player 1's (save options record +164)
  uint32_t current_p1_button_0() const { return guest::LoadU32(base, address + 140); }
  void set_current_p1_button_0(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t current_p1_button_0_address() const { return address + 140; }
  uint32_t current_p1_button_1() const { return guest::LoadU32(base, address + 144); }
  void set_current_p1_button_1(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t current_p1_button_1_address() const { return address + 144; }
  uint32_t current_p1_button_2() const { return guest::LoadU32(base, address + 148); }
  void set_current_p1_button_2(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t current_p1_button_2_address() const { return address + 148; }
  uint32_t current_p1_button_3() const { return guest::LoadU32(base, address + 152); }
  void set_current_p1_button_3(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t current_p1_button_3_address() const { return address + 152; }
  uint32_t current_p1_button_4() const { return guest::LoadU32(base, address + 156); }
  void set_current_p1_button_4(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t current_p1_button_4_address() const { return address + 156; }
  uint32_t current_p1_button_5() const { return guest::LoadU32(base, address + 160); }
  void set_current_p1_button_5(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t current_p1_button_5_address() const { return address + 160; }
  uint32_t current_p1_button_6() const { return guest::LoadU32(base, address + 164); }
  void set_current_p1_button_6(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t current_p1_button_6_address() const { return address + 164; }
  uint32_t current_p1_button_7() const { return guest::LoadU32(base, address + 168); }
  void set_current_p1_button_7(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t current_p1_button_7_address() const { return address + 168; }
  uint32_t current_p1_button_8() const { return guest::LoadU32(base, address + 172); }
  void set_current_p1_button_8(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t current_p1_button_8_address() const { return address + 172; }
  uint32_t current_p1_button_9() const { return guest::LoadU32(base, address + 176); }
  void set_current_p1_button_9(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t current_p1_button_9_address() const { return address + 176; }
  uint32_t current_p1_button_10() const { return guest::LoadU32(base, address + 180); }
  void set_current_p1_button_10(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t current_p1_button_10_address() const { return address + 180; }
  uint32_t current_p1_button_11() const { return guest::LoadU32(base, address + 184); }
  void set_current_p1_button_11(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t current_p1_button_11_address() const { return address + 184; }
  // analog indexes (save options record +104)
  uint32_t current_analog_0() const { return guest::LoadU32(base, address + 188); }
  void set_current_analog_0(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t current_analog_0_address() const { return address + 188; }
  uint32_t current_analog_1() const { return guest::LoadU32(base, address + 192); }
  void set_current_analog_1(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t current_analog_1_address() const { return address + 192; }
  uint32_t current_analog_2() const { return guest::LoadU32(base, address + 196); }
  void set_current_analog_2(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t current_analog_2_address() const { return address + 196; }
  uint32_t current_analog_3() const { return guest::LoadU32(base, address + 200); }
  void set_current_analog_3(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t current_analog_3_address() const { return address + 200; }
  uint32_t current_analog_4() const { return guest::LoadU32(base, address + 204); }
  void set_current_analog_4(uint32_t v) const { guest::StoreU32(base, address + 204, v); }
  uint32_t current_analog_4_address() const { return address + 204; }
  uint32_t current_analog_5() const { return guest::LoadU32(base, address + 208); }
  void set_current_analog_5(uint32_t v) const { guest::StoreU32(base, address + 208, v); }
  uint32_t current_analog_5_address() const { return address + 208; }
  // player 1's (save options record +224; slot 4 written to word 8)
  uint32_t current_p1_analog_0() const { return guest::LoadU32(base, address + 212); }
  void set_current_p1_analog_0(uint32_t v) const { guest::StoreU32(base, address + 212, v); }
  uint32_t current_p1_analog_0_address() const { return address + 212; }
  uint32_t current_p1_analog_1() const { return guest::LoadU32(base, address + 216); }
  void set_current_p1_analog_1(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t current_p1_analog_1_address() const { return address + 216; }
  uint32_t current_p1_analog_2() const { return guest::LoadU32(base, address + 220); }
  void set_current_p1_analog_2(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t current_p1_analog_2_address() const { return address + 220; }
  uint32_t current_p1_analog_3() const { return guest::LoadU32(base, address + 224); }
  void set_current_p1_analog_3(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t current_p1_analog_3_address() const { return address + 224; }
  uint32_t current_p1_analog_4() const { return guest::LoadU32(base, address + 228); }
  void set_current_p1_analog_4(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t current_p1_analog_4_address() const { return address + 228; }
  uint32_t current_p1_analog_5() const { return guest::LoadU32(base, address + 232); }
  void set_current_p1_analog_5(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t current_p1_analog_5_address() const { return address + 232; }
  // the edits the pages change: the 192-byte settings block named word by word below
  uint32_t edit_block_address() const { return address + 236; }
  // the HUD's four on/off values (save options +0 to +12)
  uint32_t edit_hud_0() const { return guest::LoadU32(base, address + 236); }
  void set_edit_hud_0(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t edit_hud_0_address() const { return address + 236; }
  uint32_t edit_hud_1() const { return guest::LoadU32(base, address + 240); }
  void set_edit_hud_1(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t edit_hud_1_address() const { return address + 240; }
  uint32_t edit_hud_2() const { return guest::LoadU32(base, address + 244); }
  void set_edit_hud_2(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t edit_hud_2_address() const { return address + 244; }
  uint32_t edit_hud_3() const { return guest::LoadU32(base, address + 248); }
  void set_edit_hud_3(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t edit_hud_3_address() const { return address + 248; }
  // 0 to 15 (save options byte +28)
  uint32_t edit_music_volume() const { return guest::LoadU32(base, address + 252); }
  void set_edit_music_volume(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t edit_music_volume_address() const { return address + 252; }
  // 0 to 15 (save options byte +29)
  uint32_t edit_effects_volume() const { return guest::LoadU32(base, address + 256); }
  void set_edit_effects_volume(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t edit_effects_volume_address() const { return address + 256; }
  // -10 to 10 (save options bytes +36 to +38)
  uint32_t edit_slider_0() const { return guest::LoadU32(base, address + 260); }
  void set_edit_slider_0(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t edit_slider_0_address() const { return address + 260; }
  uint32_t edit_slider_1() const { return guest::LoadU32(base, address + 264); }
  void set_edit_slider_1(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t edit_slider_1_address() const { return address + 264; }
  uint32_t edit_slider_2() const { return guest::LoadU32(base, address + 268); }
  void set_edit_slider_2(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t edit_slider_2_address() const { return address + 268; }
  // player 1's (save options bytes +39 to +41)
  uint32_t edit_p1_slider_0() const { return guest::LoadU32(base, address + 272); }
  void set_edit_p1_slider_0(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t edit_p1_slider_0_address() const { return address + 272; }
  uint32_t edit_p1_slider_1() const { return guest::LoadU32(base, address + 276); }
  void set_edit_p1_slider_1(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t edit_p1_slider_1_address() const { return address + 276; }
  uint32_t edit_p1_slider_2() const { return guest::LoadU32(base, address + 280); }
  void set_edit_p1_slider_2(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t edit_p1_slider_2_address() const { return address + 280; }
  // button indexes in menu order (save options record +44)
  uint32_t edit_button_0() const { return guest::LoadU32(base, address + 284); }
  void set_edit_button_0(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t edit_button_0_address() const { return address + 284; }
  uint32_t edit_button_1() const { return guest::LoadU32(base, address + 288); }
  void set_edit_button_1(uint32_t v) const { guest::StoreU32(base, address + 288, v); }
  uint32_t edit_button_1_address() const { return address + 288; }
  uint32_t edit_button_2() const { return guest::LoadU32(base, address + 292); }
  void set_edit_button_2(uint32_t v) const { guest::StoreU32(base, address + 292, v); }
  uint32_t edit_button_2_address() const { return address + 292; }
  uint32_t edit_button_3() const { return guest::LoadU32(base, address + 296); }
  void set_edit_button_3(uint32_t v) const { guest::StoreU32(base, address + 296, v); }
  uint32_t edit_button_3_address() const { return address + 296; }
  uint32_t edit_button_4() const { return guest::LoadU32(base, address + 300); }
  void set_edit_button_4(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t edit_button_4_address() const { return address + 300; }
  uint32_t edit_button_5() const { return guest::LoadU32(base, address + 304); }
  void set_edit_button_5(uint32_t v) const { guest::StoreU32(base, address + 304, v); }
  uint32_t edit_button_5_address() const { return address + 304; }
  uint32_t edit_button_6() const { return guest::LoadU32(base, address + 308); }
  void set_edit_button_6(uint32_t v) const { guest::StoreU32(base, address + 308, v); }
  uint32_t edit_button_6_address() const { return address + 308; }
  uint32_t edit_button_7() const { return guest::LoadU32(base, address + 312); }
  void set_edit_button_7(uint32_t v) const { guest::StoreU32(base, address + 312, v); }
  uint32_t edit_button_7_address() const { return address + 312; }
  uint32_t edit_button_8() const { return guest::LoadU32(base, address + 316); }
  void set_edit_button_8(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t edit_button_8_address() const { return address + 316; }
  uint32_t edit_button_9() const { return guest::LoadU32(base, address + 320); }
  void set_edit_button_9(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t edit_button_9_address() const { return address + 320; }
  uint32_t edit_button_10() const { return guest::LoadU32(base, address + 324); }
  void set_edit_button_10(uint32_t v) const { guest::StoreU32(base, address + 324, v); }
  uint32_t edit_button_10_address() const { return address + 324; }
  uint32_t edit_button_11() const { return guest::LoadU32(base, address + 328); }
  void set_edit_button_11(uint32_t v) const { guest::StoreU32(base, address + 328, v); }
  uint32_t edit_button_11_address() const { return address + 328; }
  // player 1's (save options record +164)
  uint32_t edit_p1_button_0() const { return guest::LoadU32(base, address + 332); }
  void set_edit_p1_button_0(uint32_t v) const { guest::StoreU32(base, address + 332, v); }
  uint32_t edit_p1_button_0_address() const { return address + 332; }
  uint32_t edit_p1_button_1() const { return guest::LoadU32(base, address + 336); }
  void set_edit_p1_button_1(uint32_t v) const { guest::StoreU32(base, address + 336, v); }
  uint32_t edit_p1_button_1_address() const { return address + 336; }
  uint32_t edit_p1_button_2() const { return guest::LoadU32(base, address + 340); }
  void set_edit_p1_button_2(uint32_t v) const { guest::StoreU32(base, address + 340, v); }
  uint32_t edit_p1_button_2_address() const { return address + 340; }
  uint32_t edit_p1_button_3() const { return guest::LoadU32(base, address + 344); }
  void set_edit_p1_button_3(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t edit_p1_button_3_address() const { return address + 344; }
  uint32_t edit_p1_button_4() const { return guest::LoadU32(base, address + 348); }
  void set_edit_p1_button_4(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t edit_p1_button_4_address() const { return address + 348; }
  uint32_t edit_p1_button_5() const { return guest::LoadU32(base, address + 352); }
  void set_edit_p1_button_5(uint32_t v) const { guest::StoreU32(base, address + 352, v); }
  uint32_t edit_p1_button_5_address() const { return address + 352; }
  uint32_t edit_p1_button_6() const { return guest::LoadU32(base, address + 356); }
  void set_edit_p1_button_6(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t edit_p1_button_6_address() const { return address + 356; }
  uint32_t edit_p1_button_7() const { return guest::LoadU32(base, address + 360); }
  void set_edit_p1_button_7(uint32_t v) const { guest::StoreU32(base, address + 360, v); }
  uint32_t edit_p1_button_7_address() const { return address + 360; }
  uint32_t edit_p1_button_8() const { return guest::LoadU32(base, address + 364); }
  void set_edit_p1_button_8(uint32_t v) const { guest::StoreU32(base, address + 364, v); }
  uint32_t edit_p1_button_8_address() const { return address + 364; }
  uint32_t edit_p1_button_9() const { return guest::LoadU32(base, address + 368); }
  void set_edit_p1_button_9(uint32_t v) const { guest::StoreU32(base, address + 368, v); }
  uint32_t edit_p1_button_9_address() const { return address + 368; }
  uint32_t edit_p1_button_10() const { return guest::LoadU32(base, address + 372); }
  void set_edit_p1_button_10(uint32_t v) const { guest::StoreU32(base, address + 372, v); }
  uint32_t edit_p1_button_10_address() const { return address + 372; }
  uint32_t edit_p1_button_11() const { return guest::LoadU32(base, address + 376); }
  void set_edit_p1_button_11(uint32_t v) const { guest::StoreU32(base, address + 376, v); }
  uint32_t edit_p1_button_11_address() const { return address + 376; }
  // analog indexes (save options record +104)
  uint32_t edit_analog_0() const { return guest::LoadU32(base, address + 380); }
  void set_edit_analog_0(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t edit_analog_0_address() const { return address + 380; }
  uint32_t edit_analog_1() const { return guest::LoadU32(base, address + 384); }
  void set_edit_analog_1(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t edit_analog_1_address() const { return address + 384; }
  uint32_t edit_analog_2() const { return guest::LoadU32(base, address + 388); }
  void set_edit_analog_2(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t edit_analog_2_address() const { return address + 388; }
  uint32_t edit_analog_3() const { return guest::LoadU32(base, address + 392); }
  void set_edit_analog_3(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t edit_analog_3_address() const { return address + 392; }
  uint32_t edit_analog_4() const { return guest::LoadU32(base, address + 396); }
  void set_edit_analog_4(uint32_t v) const { guest::StoreU32(base, address + 396, v); }
  uint32_t edit_analog_4_address() const { return address + 396; }
  uint32_t edit_analog_5() const { return guest::LoadU32(base, address + 400); }
  void set_edit_analog_5(uint32_t v) const { guest::StoreU32(base, address + 400, v); }
  uint32_t edit_analog_5_address() const { return address + 400; }
  // player 1's (save options record +224; slot 4 written to word 8)
  uint32_t edit_p1_analog_0() const { return guest::LoadU32(base, address + 404); }
  void set_edit_p1_analog_0(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t edit_p1_analog_0_address() const { return address + 404; }
  uint32_t edit_p1_analog_1() const { return guest::LoadU32(base, address + 408); }
  void set_edit_p1_analog_1(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t edit_p1_analog_1_address() const { return address + 408; }
  uint32_t edit_p1_analog_2() const { return guest::LoadU32(base, address + 412); }
  void set_edit_p1_analog_2(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t edit_p1_analog_2_address() const { return address + 412; }
  uint32_t edit_p1_analog_3() const { return guest::LoadU32(base, address + 416); }
  void set_edit_p1_analog_3(uint32_t v) const { guest::StoreU32(base, address + 416, v); }
  uint32_t edit_p1_analog_3_address() const { return address + 416; }
  uint32_t edit_p1_analog_4() const { return guest::LoadU32(base, address + 420); }
  void set_edit_p1_analog_4(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t edit_p1_analog_4_address() const { return address + 420; }
  uint32_t edit_p1_analog_5() const { return guest::LoadU32(base, address + 424); }
  void set_edit_p1_analog_5(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t edit_p1_analog_5_address() const { return address + 424; }
  // the defaults: the 192-byte settings block named word by word below
  uint32_t default_block_address() const { return address + 428; }
  // the HUD's four on/off values (save options +0 to +12)
  uint32_t default_hud_0() const { return guest::LoadU32(base, address + 428); }
  void set_default_hud_0(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t default_hud_0_address() const { return address + 428; }
  uint32_t default_hud_1() const { return guest::LoadU32(base, address + 432); }
  void set_default_hud_1(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t default_hud_1_address() const { return address + 432; }
  uint32_t default_hud_2() const { return guest::LoadU32(base, address + 436); }
  void set_default_hud_2(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t default_hud_2_address() const { return address + 436; }
  uint32_t default_hud_3() const { return guest::LoadU32(base, address + 440); }
  void set_default_hud_3(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t default_hud_3_address() const { return address + 440; }
  // 0 to 15 (save options byte +28)
  uint32_t default_music_volume() const { return guest::LoadU32(base, address + 444); }
  void set_default_music_volume(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t default_music_volume_address() const { return address + 444; }
  // 0 to 15 (save options byte +29)
  uint32_t default_effects_volume() const { return guest::LoadU32(base, address + 448); }
  void set_default_effects_volume(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t default_effects_volume_address() const { return address + 448; }
  // -10 to 10 (save options bytes +36 to +38)
  uint32_t default_slider_0() const { return guest::LoadU32(base, address + 452); }
  void set_default_slider_0(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t default_slider_0_address() const { return address + 452; }
  uint32_t default_slider_1() const { return guest::LoadU32(base, address + 456); }
  void set_default_slider_1(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t default_slider_1_address() const { return address + 456; }
  uint32_t default_slider_2() const { return guest::LoadU32(base, address + 460); }
  void set_default_slider_2(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t default_slider_2_address() const { return address + 460; }
  // player 1's (save options bytes +39 to +41)
  uint32_t default_p1_slider_0() const { return guest::LoadU32(base, address + 464); }
  void set_default_p1_slider_0(uint32_t v) const { guest::StoreU32(base, address + 464, v); }
  uint32_t default_p1_slider_0_address() const { return address + 464; }
  uint32_t default_p1_slider_1() const { return guest::LoadU32(base, address + 468); }
  void set_default_p1_slider_1(uint32_t v) const { guest::StoreU32(base, address + 468, v); }
  uint32_t default_p1_slider_1_address() const { return address + 468; }
  uint32_t default_p1_slider_2() const { return guest::LoadU32(base, address + 472); }
  void set_default_p1_slider_2(uint32_t v) const { guest::StoreU32(base, address + 472, v); }
  uint32_t default_p1_slider_2_address() const { return address + 472; }
  // button indexes in menu order (save options record +44)
  uint32_t default_button_0() const { return guest::LoadU32(base, address + 476); }
  void set_default_button_0(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t default_button_0_address() const { return address + 476; }
  uint32_t default_button_1() const { return guest::LoadU32(base, address + 480); }
  void set_default_button_1(uint32_t v) const { guest::StoreU32(base, address + 480, v); }
  uint32_t default_button_1_address() const { return address + 480; }
  uint32_t default_button_2() const { return guest::LoadU32(base, address + 484); }
  void set_default_button_2(uint32_t v) const { guest::StoreU32(base, address + 484, v); }
  uint32_t default_button_2_address() const { return address + 484; }
  uint32_t default_button_3() const { return guest::LoadU32(base, address + 488); }
  void set_default_button_3(uint32_t v) const { guest::StoreU32(base, address + 488, v); }
  uint32_t default_button_3_address() const { return address + 488; }
  uint32_t default_button_4() const { return guest::LoadU32(base, address + 492); }
  void set_default_button_4(uint32_t v) const { guest::StoreU32(base, address + 492, v); }
  uint32_t default_button_4_address() const { return address + 492; }
  uint32_t default_button_5() const { return guest::LoadU32(base, address + 496); }
  void set_default_button_5(uint32_t v) const { guest::StoreU32(base, address + 496, v); }
  uint32_t default_button_5_address() const { return address + 496; }
  uint32_t default_button_6() const { return guest::LoadU32(base, address + 500); }
  void set_default_button_6(uint32_t v) const { guest::StoreU32(base, address + 500, v); }
  uint32_t default_button_6_address() const { return address + 500; }
  uint32_t default_button_7() const { return guest::LoadU32(base, address + 504); }
  void set_default_button_7(uint32_t v) const { guest::StoreU32(base, address + 504, v); }
  uint32_t default_button_7_address() const { return address + 504; }
  uint32_t default_button_8() const { return guest::LoadU32(base, address + 508); }
  void set_default_button_8(uint32_t v) const { guest::StoreU32(base, address + 508, v); }
  uint32_t default_button_8_address() const { return address + 508; }
  uint32_t default_button_9() const { return guest::LoadU32(base, address + 512); }
  void set_default_button_9(uint32_t v) const { guest::StoreU32(base, address + 512, v); }
  uint32_t default_button_9_address() const { return address + 512; }
  uint32_t default_button_10() const { return guest::LoadU32(base, address + 516); }
  void set_default_button_10(uint32_t v) const { guest::StoreU32(base, address + 516, v); }
  uint32_t default_button_10_address() const { return address + 516; }
  uint32_t default_button_11() const { return guest::LoadU32(base, address + 520); }
  void set_default_button_11(uint32_t v) const { guest::StoreU32(base, address + 520, v); }
  uint32_t default_button_11_address() const { return address + 520; }
  // player 1's (save options record +164)
  uint32_t default_p1_button_0() const { return guest::LoadU32(base, address + 524); }
  void set_default_p1_button_0(uint32_t v) const { guest::StoreU32(base, address + 524, v); }
  uint32_t default_p1_button_0_address() const { return address + 524; }
  uint32_t default_p1_button_1() const { return guest::LoadU32(base, address + 528); }
  void set_default_p1_button_1(uint32_t v) const { guest::StoreU32(base, address + 528, v); }
  uint32_t default_p1_button_1_address() const { return address + 528; }
  uint32_t default_p1_button_2() const { return guest::LoadU32(base, address + 532); }
  void set_default_p1_button_2(uint32_t v) const { guest::StoreU32(base, address + 532, v); }
  uint32_t default_p1_button_2_address() const { return address + 532; }
  uint32_t default_p1_button_3() const { return guest::LoadU32(base, address + 536); }
  void set_default_p1_button_3(uint32_t v) const { guest::StoreU32(base, address + 536, v); }
  uint32_t default_p1_button_3_address() const { return address + 536; }
  uint32_t default_p1_button_4() const { return guest::LoadU32(base, address + 540); }
  void set_default_p1_button_4(uint32_t v) const { guest::StoreU32(base, address + 540, v); }
  uint32_t default_p1_button_4_address() const { return address + 540; }
  uint32_t default_p1_button_5() const { return guest::LoadU32(base, address + 544); }
  void set_default_p1_button_5(uint32_t v) const { guest::StoreU32(base, address + 544, v); }
  uint32_t default_p1_button_5_address() const { return address + 544; }
  uint32_t default_p1_button_6() const { return guest::LoadU32(base, address + 548); }
  void set_default_p1_button_6(uint32_t v) const { guest::StoreU32(base, address + 548, v); }
  uint32_t default_p1_button_6_address() const { return address + 548; }
  uint32_t default_p1_button_7() const { return guest::LoadU32(base, address + 552); }
  void set_default_p1_button_7(uint32_t v) const { guest::StoreU32(base, address + 552, v); }
  uint32_t default_p1_button_7_address() const { return address + 552; }
  uint32_t default_p1_button_8() const { return guest::LoadU32(base, address + 556); }
  void set_default_p1_button_8(uint32_t v) const { guest::StoreU32(base, address + 556, v); }
  uint32_t default_p1_button_8_address() const { return address + 556; }
  uint32_t default_p1_button_9() const { return guest::LoadU32(base, address + 560); }
  void set_default_p1_button_9(uint32_t v) const { guest::StoreU32(base, address + 560, v); }
  uint32_t default_p1_button_9_address() const { return address + 560; }
  uint32_t default_p1_button_10() const { return guest::LoadU32(base, address + 564); }
  void set_default_p1_button_10(uint32_t v) const { guest::StoreU32(base, address + 564, v); }
  uint32_t default_p1_button_10_address() const { return address + 564; }
  uint32_t default_p1_button_11() const { return guest::LoadU32(base, address + 568); }
  void set_default_p1_button_11(uint32_t v) const { guest::StoreU32(base, address + 568, v); }
  uint32_t default_p1_button_11_address() const { return address + 568; }
  // analog indexes (save options record +104)
  uint32_t default_analog_0() const { return guest::LoadU32(base, address + 572); }
  void set_default_analog_0(uint32_t v) const { guest::StoreU32(base, address + 572, v); }
  uint32_t default_analog_0_address() const { return address + 572; }
  uint32_t default_analog_1() const { return guest::LoadU32(base, address + 576); }
  void set_default_analog_1(uint32_t v) const { guest::StoreU32(base, address + 576, v); }
  uint32_t default_analog_1_address() const { return address + 576; }
  uint32_t default_analog_2() const { return guest::LoadU32(base, address + 580); }
  void set_default_analog_2(uint32_t v) const { guest::StoreU32(base, address + 580, v); }
  uint32_t default_analog_2_address() const { return address + 580; }
  uint32_t default_analog_3() const { return guest::LoadU32(base, address + 584); }
  void set_default_analog_3(uint32_t v) const { guest::StoreU32(base, address + 584, v); }
  uint32_t default_analog_3_address() const { return address + 584; }
  uint32_t default_analog_4() const { return guest::LoadU32(base, address + 588); }
  void set_default_analog_4(uint32_t v) const { guest::StoreU32(base, address + 588, v); }
  uint32_t default_analog_4_address() const { return address + 588; }
  uint32_t default_analog_5() const { return guest::LoadU32(base, address + 592); }
  void set_default_analog_5(uint32_t v) const { guest::StoreU32(base, address + 592, v); }
  uint32_t default_analog_5_address() const { return address + 592; }
  // player 1's (save options record +224; slot 4 written to word 8)
  uint32_t default_p1_analog_0() const { return guest::LoadU32(base, address + 596); }
  void set_default_p1_analog_0(uint32_t v) const { guest::StoreU32(base, address + 596, v); }
  uint32_t default_p1_analog_0_address() const { return address + 596; }
  uint32_t default_p1_analog_1() const { return guest::LoadU32(base, address + 600); }
  void set_default_p1_analog_1(uint32_t v) const { guest::StoreU32(base, address + 600, v); }
  uint32_t default_p1_analog_1_address() const { return address + 600; }
  uint32_t default_p1_analog_2() const { return guest::LoadU32(base, address + 604); }
  void set_default_p1_analog_2(uint32_t v) const { guest::StoreU32(base, address + 604, v); }
  uint32_t default_p1_analog_2_address() const { return address + 604; }
  uint32_t default_p1_analog_3() const { return guest::LoadU32(base, address + 608); }
  void set_default_p1_analog_3(uint32_t v) const { guest::StoreU32(base, address + 608, v); }
  uint32_t default_p1_analog_3_address() const { return address + 608; }
  uint32_t default_p1_analog_4() const { return guest::LoadU32(base, address + 612); }
  void set_default_p1_analog_4(uint32_t v) const { guest::StoreU32(base, address + 612, v); }
  uint32_t default_p1_analog_4_address() const { return address + 612; }
  uint32_t default_p1_analog_5() const { return guest::LoadU32(base, address + 616); }
  void set_default_p1_analog_5(uint32_t v) const { guest::StoreU32(base, address + 616, v); }
  uint32_t default_p1_analog_5_address() const { return address + 616; }
  // page 9's dialog (category 5, the load): the state shown
  uint32_t load_dialog_state() const { return guest::LoadU32(base, address + 620); }
  void set_load_dialog_state(uint32_t v) const { guest::StoreU32(base, address + 620, v); }
  uint32_t load_dialog_state_address() const { return address + 620; }
  // the state next
  uint32_t load_dialog_next() const { return guest::LoadU32(base, address + 624); }
  void set_load_dialog_next(uint32_t v) const { guest::StoreU32(base, address + 624, v); }
  uint32_t load_dialog_next_address() const { return address + 624; }
  uint32_t load_dialog_kind() const { return guest::LoadU32(base, address + 628); }
  void set_load_dialog_kind(uint32_t v) const { guest::StoreU32(base, address + 628, v); }
  uint32_t load_dialog_kind_address() const { return address + 628; }
  // the passes its note and wait sub-pages count
  uint32_t load_dialog_passes() const { return guest::LoadU32(base, address + 632); }
  void set_load_dialog_passes(uint32_t v) const { guest::StoreU32(base, address + 632, v); }
  uint32_t load_dialog_passes_address() const { return address + 632; }
  // the default answer
  uint32_t load_dialog_default() const { return guest::LoadU32(base, address + 636); }
  void set_load_dialog_default(uint32_t v) const { guest::StoreU32(base, address + 636, v); }
  uint32_t load_dialog_default_address() const { return address + 636; }
  // page 10's dialog (category 6, the save): the state shown
  uint32_t save_dialog_state() const { return guest::LoadU32(base, address + 640); }
  void set_save_dialog_state(uint32_t v) const { guest::StoreU32(base, address + 640, v); }
  uint32_t save_dialog_state_address() const { return address + 640; }
  // the state next
  uint32_t save_dialog_next() const { return guest::LoadU32(base, address + 644); }
  void set_save_dialog_next(uint32_t v) const { guest::StoreU32(base, address + 644, v); }
  uint32_t save_dialog_next_address() const { return address + 644; }
  uint32_t save_dialog_kind() const { return guest::LoadU32(base, address + 648); }
  void set_save_dialog_kind(uint32_t v) const { guest::StoreU32(base, address + 648, v); }
  uint32_t save_dialog_kind_address() const { return address + 648; }
  // the passes its note and wait sub-pages count
  uint32_t save_dialog_passes() const { return guest::LoadU32(base, address + 652); }
  void set_save_dialog_passes(uint32_t v) const { guest::StoreU32(base, address + 652, v); }
  uint32_t save_dialog_passes_address() const { return address + 652; }
  // the default answer
  uint32_t save_dialog_default() const { return guest::LoadU32(base, address + 656); }
  void set_save_dialog_default(uint32_t v) const { guest::StoreU32(base, address + 656, v); }
  uint32_t save_dialog_default_address() const { return address + 656; }
  // the category page's last row: 1 OK, else Default
  uint32_t ok_default() const { return guest::LoadU32(base, address + 660); }
  void set_ok_default(uint32_t v) const { guest::StoreU32(base, address + 660, v); }
  uint32_t ok_default_address() const { return address + 660; }
  // the category picked
  uint32_t category_cursor() const { return guest::LoadU32(base, address + 664); }
  void set_category_cursor(uint32_t v) const { guest::StoreU32(base, address + 664, v); }
  uint32_t category_cursor_address() const { return address + 664; }
  // the buttons page's last left or right (-1, 1, else 0)
  uint32_t buttons_side() const { return guest::LoadU32(base, address + 668); }
  void set_buttons_side(uint32_t v) const { guest::StoreU32(base, address + 668, v); }
  uint32_t buttons_side_address() const { return address + 668; }
  // player 1's (the controller pages')
  uint32_t p1_buttons_side() const { return guest::LoadU32(base, address + 672); }
  void set_p1_buttons_side(uint32_t v) const { guest::StoreU32(base, address + 672, v); }
  uint32_t p1_buttons_side_address() const { return address + 672; }
  // the analog page's last left or right
  uint32_t analog_side() const { return guest::LoadU32(base, address + 676); }
  void set_analog_side(uint32_t v) const { guest::StoreU32(base, address + 676, v); }
  uint32_t analog_side_address() const { return address + 676; }
  // player 1's (the controller pages')
  uint32_t p1_analog_side() const { return guest::LoadU32(base, address + 680); }
  void set_p1_analog_side(uint32_t v) const { guest::StoreU32(base, address + 680, v); }
  uint32_t p1_analog_side_address() const { return address + 680; }
  // the wait steps' pass counter
  uint32_t wait_counter() const { return guest::LoadU32(base, address + 684); }
  void set_wait_counter(uint32_t v) const { guest::StoreU32(base, address + 684, v); }
  uint32_t wait_counter_address() const { return address + 684; }
  // the music track picked (0 to 16, bgm_tmp_01 to bgm_tmp_17)
  uint32_t music_track() const { return guest::LoadU32(base, address + 688); }
  void set_music_track(uint32_t v) const { guest::StoreU32(base, address + 688, v); }
  uint32_t music_track_address() const { return address + 688; }
  // -1 none
  uint32_t track_previewed() const { return guest::LoadU32(base, address + 692); }
  void set_track_previewed(uint32_t v) const { guest::StoreU32(base, address + 692, v); }
  uint32_t track_previewed_address() const { return address + 692; }
  // the track loaded in bank 2, -1 none
  uint32_t track_loaded() const { return guest::LoadU32(base, address + 696); }
  void set_track_loaded(uint32_t v) const { guest::StoreU32(base, address + 696, v); }
  uint32_t track_loaded_address() const { return address + 696; }
  // the music preview's state (0 to 3)
  uint32_t preview_state() const { return guest::LoadU32(base, address + 700); }
  void set_preview_state(uint32_t v) const { guest::StoreU32(base, address + 700, v); }
  uint32_t preview_state_address() const { return address + 700; }
  // the menu music's cue (bank 1) to restart
  uint32_t menu_music_cue() const { return guest::LoadU32(base, address + 704); }
  void set_menu_music_cue(uint32_t v) const { guest::StoreU32(base, address + 704, v); }
  uint32_t menu_music_cue_address() const { return address + 704; }
  // the dialog's cancel flag
  uint32_t dialog_cancel() const { return guest::LoadU32(base, address + 708); }
  void set_dialog_cancel(uint32_t v) const { guest::StoreU32(base, address + 708, v); }
  uint32_t dialog_cancel_address() const { return address + 708; }
  // 1 at reset, 0 at a load or save page's set-up, 1 when its close or cancel sub-page runs
  uint32_t save_page_closed() const { return guest::LoadU32(base, address + 712); }
  void set_save_page_closed(uint32_t v) const { guest::StoreU32(base, address + 712, v); }
  uint32_t save_page_closed_address() const { return address + 712; }
  // measure fault: the image word [0x826D8478] read through the globals' base 0x826E0000, not a field
  uint32_t fault_settings_global() const { return guest::LoadU32(base, address + -31624); }
  void set_fault_settings_global(uint32_t v) const { guest::StoreU32(base, address + -31624, v); }
  uint32_t fault_settings_global_address() const { return address + -31624; }
  // measure fault: the image word [0x826D8494] read through the globals' base 0x826E0000, not a field
  uint32_t fault_controller_settings_global() const { return guest::LoadU32(base, address + -31596); }
  void set_fault_controller_settings_global(uint32_t v) const { guest::StoreU32(base, address + -31596, v); }
  uint32_t fault_controller_settings_global_address() const { return address + -31596; }
};

// embedded; measure: new<68>@Options::OnEnter.+4
struct OptionsFaultBase : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // measure fault: the image word [0x826D8478], not a field
  uint32_t fault_settings_global() const { return guest::LoadU32(base, address + -31620); }
  void set_fault_settings_global(uint32_t v) const { guest::StoreU32(base, address + -31620, v); }
  uint32_t fault_settings_global_address() const { return address + -31620; }
};

// file; measure: UiMovies::Find.ret
struct UiMovieTableValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the value (in the movie table: the UI movie)
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: Music::Play.ret.+0
struct OptionsMusicPlayVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // probably the sound manager's IsBankLoading
  uint32_t slot_6() const { return guest::LoadU32(base, address + 24); }
  void set_slot_6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot_6_address() const { return address + 24; }
};

// embedded; measure: Music::Play.ret
struct OptionsMusicPlayResult : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = OptionsMusicPlayVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// The image words docs/functions/options-screen.md's code uses.
struct OptionsScreenGlobals {
  // the options' page block
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8470u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8470u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8470u, v); }
  static constexpr uint32_t pages_address() { return 0x826D8470u; }
  // the categories' and every category page's menu
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8474u)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8474u, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8474u, v); }
  static constexpr uint32_t menu_address() { return 0x826D8474u; }
  template <typename T = OptionsSettings>
  static T settings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8478u)); }
  static void set_settings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8478u, v.address); }
  static void set_settings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8478u, v); }
  static constexpr uint32_t settings_address() { return 0x826D8478u; }
  // page 9's sub-page block (28 bytes, options-load-save.toml's)
  static uint32_t load_pages(uint8_t* base) { return guest::LoadU32(base, 0x826D847Cu); }
  static void set_load_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D847Cu, v); }
  static constexpr uint32_t load_pages_address() { return 0x826D847Cu; }
  // a 28-byte block laid out as the page block, set up by OnEnter only
  template <typename T = PageObject>
  static T spare_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8480u)); }
  static void set_spare_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8480u, v.address); }
  static void set_spare_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8480u, v); }
  static constexpr uint32_t spare_pages_address() { return 0x826D8480u; }
  // the save pages' two-item menu
  template <typename T = FrontEndMenu>
  static T dialog_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8484u)); }
  static void set_dialog_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8484u, v.address); }
  static void set_dialog_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8484u, v); }
  static constexpr uint32_t dialog_menu_address() { return 0x826D8484u; }
  // page 10's sub-page block (28 bytes, options-load-save.toml's)
  static uint32_t save_pages(uint8_t* base) { return guest::LoadU32(base, 0x826D8488u); }
  static void set_save_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8488u, v); }
  static constexpr uint32_t save_pages_address() { return 0x826D8488u; }
  // the mode type the context and the posts follow
  static uint32_t mode_type(uint8_t* base) { return guest::LoadU32(base, 0x826032DCu); }
  static void set_mode_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032DCu, v); }
  static constexpr uint32_t mode_type_address() { return 0x826032DCu; }
  // which front-end menu to show on return (probably); 0 from the categories' back in type 4
  static uint32_t return_menu(uint8_t* base) { return guest::LoadU32(base, 0x826364ECu); }
  static void set_return_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826364ECu, v); }
  static constexpr uint32_t return_menu_address() { return 0x826364ECu; }
  // the category table: 5 contexts of 7 words, -1 past the last
  static constexpr uint32_t categories_address() { return 0x8200A578u; }
  // the sliders' x scale
  static float slider_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200A6ACu); }
  static void set_slider_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200A6ACu, v); }
  static constexpr uint32_t slider_scale_address() { return 0x8200A6ACu; }
  // the screen fades object FadeIn is given
  static uint32_t screen_fades(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_screen_fades(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t screen_fades_address() { return 0x82001BECu; }
};

}  // namespace schema
