// The views of docs/functions/race-screens.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-screens.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<?>@MenuList::Init
struct MenuListEnabled : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // item 0's word (0 disabled); item i's at +4i
  uint32_t item_0() const { return guest::LoadU32(base, address + 0); }
  void set_item_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t item_0_address() const { return address + 0; }
};

// allocated; measure: MenuList, new<72>@PauseMenu::Create, new<72>@AfterRaceMenu::Create, new<72>@RaceDialog::Create, new<72>@PauseMenu4::Create, new<72>@SaveScreen::OnEnter, new<72>@TitleScreen::Create, new<72>@ReplayTheater::Create, new<72>@PlateEditor::Create, new<72>@BuildScreen::Create, new<72>@BuildScreen::Create#2, new<72>@GarageScreen::Create, new<72>@GarageScreen::Create#2, new<72>@RaceScreens::Make1004, new<72>@CarDealer::Create#2
struct MenuList : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // the item count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the first item shown
  uint32_t scroll() const { return guest::LoadU32(base, address + 4); }
  void set_scroll(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t scroll_address() const { return address + 4; }
  uint32_t scroll_before() const { return guest::LoadU32(base, address + 8); }
  void set_scroll_before(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t scroll_before_address() const { return address + 8; }
  // the items shown
  uint32_t shown() const { return guest::LoadU32(base, address + 12); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t shown_address() const { return address + 12; }
  // relative to the scroll
  uint32_t selection() const { return guest::LoadU32(base, address + 16); }
  void set_selection(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t selection_address() const { return address + 16; }
  uint32_t selection_before() const { return guest::LoadU32(base, address + 20); }
  void set_selection_before(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t selection_before_address() const { return address + 20; }
  // the last move: 1 down, -1 up, 0 none
  uint32_t move() const { return guest::LoadU32(base, address + 24); }
  void set_move(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t move_address() const { return address + 24; }
  // left or right asked: -1, 1, else 0
  uint32_t side() const { return guest::LoadU32(base, address + 28); }
  void set_side(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t side_address() const { return address + 28; }
  // set when the move wrapped or skipped
  uint32_t wrapped() const { return guest::LoadU32(base, address + 32); }
  void set_wrapped(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t wrapped_address() const { return address + 32; }
  uint32_t down_action() const { return guest::LoadU32(base, address + 36); }
  void set_down_action(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t down_action_address() const { return address + 36; }
  uint32_t up_action() const { return guest::LoadU32(base, address + 40); }
  void set_up_action(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t up_action_address() const { return address + 40; }
  uint32_t left_action() const { return guest::LoadU32(base, address + 44); }
  void set_left_action(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t left_action_address() const { return address + 44; }
  uint32_t right_action() const { return guest::LoadU32(base, address + 48); }
  void set_right_action(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t right_action_address() const { return address + 48; }
  uint32_t accept_action() const { return guest::LoadU32(base, address + 52); }
  void set_accept_action(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t accept_action_address() const { return address + 52; }
  uint32_t back_action() const { return guest::LoadU32(base, address + 56); }
  void set_back_action(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t back_action_address() const { return address + 56; }
  // an index into [0x826BABA4], then the mapper entry at 0x826BAB80
  uint32_t player() const { return guest::LoadU32(base, address + 60); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t player_address() const { return address + 60; }
  uint32_t wraps() const { return guest::LoadU32(base, address + 64); }
  void set_wraps(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t wraps_address() const { return address + 64; }
  // a word an item, 0 for a disabled item
  template <typename T = MenuListEnabled>
  T enabled() const { return T(base, guest::LoadU32(base, address + 68)); }
  void set_enabled(const View& v) const { guest::StoreU32(base, address + 68, v.address); }
  void set_enabled(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t enabled_address() const { return address + 68; }
  // measure fault: PanelSet::Open's PanelSetBase (car-list.toml), taken for a menu list; its +228
  uint32_t fault_panel_set_228() const { return guest::LoadU32(base, address + 228); }
  void set_fault_panel_set_228(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t fault_panel_set_228_address() const { return address + 228; }
  // measure fault: as fault_panel_set_228
  uint32_t fault_panel_set_232() const { return guest::LoadU32(base, address + 232); }
  void set_fault_panel_set_232(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t fault_panel_set_232_address() const { return address + 232; }
  // measure fault: as fault_panel_set_228 (b + 272, list 0)
  uint32_t fault_panel_set_1820() const { return guest::LoadU32(base, address + 1820); }
  void set_fault_panel_set_1820(uint32_t v) const { guest::StoreU32(base, address + 1820, v); }
  uint32_t fault_panel_set_1820_address() const { return address + 1820; }
  // measure fault: as fault_panel_set_228
  uint32_t fault_panel_set_1900() const { return guest::LoadU32(base, address + 1900); }
  void set_fault_panel_set_1900(uint32_t v) const { guest::StoreU32(base, address + 1900, v); }
  uint32_t fault_panel_set_1900_address() const { return address + 1900; }
};

// allocated; measure: new<4>@PauseMenu::Create
#pragma pack(push, 1)
struct PauseMenuScreenData {
  uint32_t movie = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(PauseMenuScreenData) == 4);
inline constexpr memory::LayoutField kPauseMenuScreenFields[] = {{0, 4}};
inline constexpr memory::Layout kPauseMenuScreenLayout = {kPauseMenuScreenFields, 1};

struct PauseMenuScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  PauseMenuScreenData* data() const { return static_cast<PauseMenuScreenData*>(memory::HandleObject(address)); }
  static PauseMenuScreen New(uint8_t* base) {
    return PauseMenuScreen(base, memory::MakeHandle(new PauseMenuScreenData(), sizeof(PauseMenuScreenData), "PauseMenuScreen", &kPauseMenuScreenLayout));
  }
  template <uint32_t Stride = kSize>
  static PauseMenuScreen NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(PauseMenuScreenData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PauseMenuScreenData*>(::operator new(sizeof(PauseMenuScreenData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PauseMenuScreenData();
    return PauseMenuScreen(base, memory::MakeHandle(p, sizeof(PauseMenuScreenData) * count, "PauseMenuScreen", &kPauseMenuScreenLayout, sizeof(PauseMenuScreenData)));
  }
  PauseMenuScreen at(uint32_t i) const { return PauseMenuScreen(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PauseMenuScreenData* direct() const { return static_cast<PauseMenuScreenData*>(memory::HandleOwn(address, &kPauseMenuScreenLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPauseMenuScreenLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPauseMenuScreenLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // UI movie 4 (UiMovies::Find)
  uint32_t movie_address() const { return address + 0; }
  uint32_t movie_bits() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  View movie() const { return View(base, movie_bits()); }
  void set_movie(uint32_t v) const { set_movie_bits(v); }
};

// embedded; measure: PauseMenu
struct PauseMenuStarted : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 on the step's first call; the transition sets 1
  uint32_t started() const { return guest::LoadU32(base, address + 0); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t started_address() const { return address + 0; }
};

// allocated; measure: new<160>@PauseMenu::Create
#pragma pack(push, 1)
struct PauseMenuDataData {
  uint32_t hud_option_0 = 0;  // +0
  uint32_t hud_option_1 = 0;  // +4
  uint32_t hud_option_2 = 0;  // +8
  uint32_t hud_option_3 = 0;  // +12
  uint32_t music_volume_saved = 0;  // +16
  uint32_t effects_volume_saved = 0;  // +20
  uint32_t adjust_saved_0 = 0;  // +24
  uint32_t adjust_saved_1 = 0;  // +28
  uint32_t adjust_saved_2 = 0;  // +32
  uint32_t hud_edit_0 = 0;  // +36
  uint32_t hud_edit_1 = 0;  // +40
  uint32_t hud_edit_2 = 0;  // +44
  uint32_t hud_edit_3 = 0;  // +48
  uint32_t music_volume = 0;  // +52
  uint32_t effects_volume = 0;  // +56
  uint32_t adjust_0 = 0;  // +60
  uint32_t adjust_1 = 0;  // +64
  uint32_t adjust_2 = 0;  // +68
  uint32_t hud_default_0 = 0;  // +72
  uint32_t hud_default_1 = 0;  // +76
  uint32_t hud_default_2 = 0;  // +80
  uint32_t hud_default_3 = 0;  // +84
  uint32_t music_volume_default = 0;  // +88
  uint32_t effects_volume_default = 0;  // +92
  uint32_t adjust_default_0 = 0;  // +96
  uint32_t adjust_default_1 = 0;  // +100
  uint32_t adjust_default_2 = 0;  // +104
  uint32_t money = 0;  // +108
  uint32_t weekday = 0;  // +112
  uint32_t day = 0;  // +116
  uint32_t win_streak = 0;  // +120
  uint32_t car_value_a = 0;  // +124
  uint32_t car_value_b = 0;  // +128
  uint32_t kind = 0;  // +132
  uint32_t item_count = 0;  // +136
  uint32_t answer = 0;  // +140
  uint32_t last_item_ok = 0;  // +144
  uint32_t main_selection = 0;  // +148
  uint32_t run_count = 0;  // +152
  uint32_t first_day = 0;  // +156
};
#pragma pack(pop)
static_assert(sizeof(PauseMenuDataData) == 160);
inline constexpr memory::LayoutField kPauseMenuDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}};
inline constexpr memory::Layout kPauseMenuDataLayout = {kPauseMenuDataFields, 40};

struct PauseMenuData : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  PauseMenuDataData* data() const { return static_cast<PauseMenuDataData*>(memory::HandleObject(address)); }
  static PauseMenuData New(uint8_t* base) {
    return PauseMenuData(base, memory::MakeHandle(new PauseMenuDataData(), sizeof(PauseMenuDataData), "PauseMenuData", &kPauseMenuDataLayout));
  }
  template <uint32_t Stride = kSize>
  static PauseMenuData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(PauseMenuDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PauseMenuDataData*>(::operator new(sizeof(PauseMenuDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PauseMenuDataData();
    return PauseMenuData(base, memory::MakeHandle(p, sizeof(PauseMenuDataData) * count, "PauseMenuData", &kPauseMenuDataLayout, sizeof(PauseMenuDataData)));
  }
  PauseMenuData at(uint32_t i) const { return PauseMenuData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PauseMenuDataData* direct() const { return static_cast<PauseMenuDataData*>(memory::HandleOwn(address, &kPauseMenuDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPauseMenuDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPauseMenuDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the four HUD options (the save's options words +0 to +12)
  uint32_t hud_option_0_address() const { return address + 0; }
  uint32_t hud_option_0_bits() const { if (auto* d = direct()) return d->hud_option_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_hud_option_0_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_option_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t hud_option_0() const { if (auto* d = direct()) return d->hud_option_0; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_hud_option_0(uint32_t v) const { if (auto* d = direct()) { d->hud_option_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t hud_option_1_address() const { return address + 4; }
  uint32_t hud_option_1_bits() const { if (auto* d = direct()) return d->hud_option_1; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_hud_option_1_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_option_1 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t hud_option_1() const { if (auto* d = direct()) return d->hud_option_1; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_hud_option_1(uint32_t v) const { if (auto* d = direct()) { d->hud_option_1 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t hud_option_2_address() const { return address + 8; }
  uint32_t hud_option_2_bits() const { if (auto* d = direct()) return d->hud_option_2; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_hud_option_2_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_option_2 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t hud_option_2() const { if (auto* d = direct()) return d->hud_option_2; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_hud_option_2(uint32_t v) const { if (auto* d = direct()) { d->hud_option_2 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t hud_option_3_address() const { return address + 12; }
  uint32_t hud_option_3_bits() const { if (auto* d = direct()) return d->hud_option_3; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_hud_option_3_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_option_3 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t hud_option_3() const { if (auto* d = direct()) return d->hud_option_3; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_hud_option_3(uint32_t v) const { if (auto* d = direct()) { d->hud_option_3 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the options' byte +28, sign-extended (the music's: VolumePage sets SetMusicVolume with it)
  uint32_t music_volume_saved_address() const { return address + 16; }
  uint32_t music_volume_saved_bits() const { if (auto* d = direct()) return d->music_volume_saved; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_music_volume_saved_bits(uint32_t v) const { if (auto* d = direct()) { d->music_volume_saved = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t music_volume_saved() const { if (auto* d = direct()) return d->music_volume_saved; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_music_volume_saved(uint32_t v) const { if (auto* d = direct()) { d->music_volume_saved = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // the options' byte +29
  uint32_t effects_volume_saved_address() const { return address + 20; }
  uint32_t effects_volume_saved_bits() const { if (auto* d = direct()) return d->effects_volume_saved; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_effects_volume_saved_bits(uint32_t v) const { if (auto* d = direct()) { d->effects_volume_saved = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t effects_volume_saved() const { if (auto* d = direct()) return d->effects_volume_saved; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_effects_volume_saved(uint32_t v) const { if (auto* d = direct()) { d->effects_volume_saved = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // three values -10 to 10 (the options' bytes +36 to +38)
  uint32_t adjust_saved_0_address() const { return address + 24; }
  uint32_t adjust_saved_0_bits() const { if (auto* d = direct()) return d->adjust_saved_0; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_adjust_saved_0_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_saved_0 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t adjust_saved_0() const { if (auto* d = direct()) return d->adjust_saved_0; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_adjust_saved_0(uint32_t v) const { if (auto* d = direct()) { d->adjust_saved_0 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t adjust_saved_1_address() const { return address + 28; }
  uint32_t adjust_saved_1_bits() const { if (auto* d = direct()) return d->adjust_saved_1; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_adjust_saved_1_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_saved_1 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t adjust_saved_1() const { if (auto* d = direct()) return d->adjust_saved_1; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_adjust_saved_1(uint32_t v) const { if (auto* d = direct()) { d->adjust_saved_1 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t adjust_saved_2_address() const { return address + 32; }
  uint32_t adjust_saved_2_bits() const { if (auto* d = direct()) return d->adjust_saved_2; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_adjust_saved_2_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_saved_2 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t adjust_saved_2() const { if (auto* d = direct()) return d->adjust_saved_2; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_adjust_saved_2(uint32_t v) const { if (auto* d = direct()) { d->adjust_saved_2 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the HUD options' edits (page 1)
  uint32_t hud_edit_0_address() const { return address + 36; }
  uint32_t hud_edit_0_bits() const { if (auto* d = direct()) return d->hud_edit_0; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_hud_edit_0_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_0 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t hud_edit_0() const { if (auto* d = direct()) return d->hud_edit_0; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_hud_edit_0(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_0 = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t hud_edit_1_address() const { return address + 40; }
  uint32_t hud_edit_1_bits() const { if (auto* d = direct()) return d->hud_edit_1; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_hud_edit_1_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_1 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t hud_edit_1() const { if (auto* d = direct()) return d->hud_edit_1; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_hud_edit_1(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_1 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t hud_edit_2_address() const { return address + 44; }
  uint32_t hud_edit_2_bits() const { if (auto* d = direct()) return d->hud_edit_2; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_hud_edit_2_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_2 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  uint32_t hud_edit_2() const { if (auto* d = direct()) return d->hud_edit_2; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_hud_edit_2(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_2 = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  uint32_t hud_edit_3_address() const { return address + 48; }
  uint32_t hud_edit_3_bits() const { if (auto* d = direct()) return d->hud_edit_3; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_hud_edit_3_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_3 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t hud_edit_3() const { if (auto* d = direct()) return d->hud_edit_3; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_hud_edit_3(uint32_t v) const { if (auto* d = direct()) { d->hud_edit_3 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  // the volumes' edits (page 2), 0 to 15: SoundOptions::SetMusicVolume's (+52) and SetEffectsVolume's (+56)
  uint32_t music_volume_address() const { return address + 52; }
  uint32_t music_volume_bits() const { if (auto* d = direct()) return d->music_volume; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_music_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->music_volume = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t music_volume() const { if (auto* d = direct()) return d->music_volume; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_music_volume(uint32_t v) const { if (auto* d = direct()) { d->music_volume = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t effects_volume_address() const { return address + 56; }
  uint32_t effects_volume_bits() const { if (auto* d = direct()) return d->effects_volume; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_effects_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->effects_volume = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  uint32_t effects_volume() const { if (auto* d = direct()) return d->effects_volume; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_effects_volume(uint32_t v) const { if (auto* d = direct()) { d->effects_volume = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  // the three values' edits (page 3), -10 to 10
  uint32_t adjust_0_address() const { return address + 60; }
  uint32_t adjust_0_bits() const { if (auto* d = direct()) return d->adjust_0; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_adjust_0_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_0 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t adjust_0() const { if (auto* d = direct()) return d->adjust_0; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_adjust_0(uint32_t v) const { if (auto* d = direct()) { d->adjust_0 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t adjust_1_address() const { return address + 64; }
  uint32_t adjust_1_bits() const { if (auto* d = direct()) return d->adjust_1; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_adjust_1_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_1 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  uint32_t adjust_1() const { if (auto* d = direct()) return d->adjust_1; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_adjust_1(uint32_t v) const { if (auto* d = direct()) { d->adjust_1 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  uint32_t adjust_2_address() const { return address + 68; }
  uint32_t adjust_2_bits() const { if (auto* d = direct()) return d->adjust_2; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(base, address + 68))); }
  void set_adjust_2_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_2 = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(base, address + 68, v); }
  uint32_t adjust_2() const { if (auto* d = direct()) return d->adjust_2; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(base, address + 68))); }
  void set_adjust_2(uint32_t v) const { if (auto* d = direct()) { d->adjust_2 = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(base, address + 68, v); }
  // the HUD options' defaults (the last item's Default), 1 at LoadData
  uint32_t hud_default_0_address() const { return address + 72; }
  uint32_t hud_default_0_bits() const { if (auto* d = direct()) return d->hud_default_0; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(base, address + 72))); }
  void set_hud_default_0_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_default_0 = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(base, address + 72, v); }
  uint32_t hud_default_0() const { if (auto* d = direct()) return d->hud_default_0; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(base, address + 72))); }
  void set_hud_default_0(uint32_t v) const { if (auto* d = direct()) { d->hud_default_0 = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(base, address + 72, v); }
  // 1 at LoadData
  uint32_t hud_default_1_address() const { return address + 76; }
  uint32_t hud_default_1_bits() const { if (auto* d = direct()) return d->hud_default_1; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_hud_default_1_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_default_1 = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  uint32_t hud_default_1() const { if (auto* d = direct()) return d->hud_default_1; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_hud_default_1(uint32_t v) const { if (auto* d = direct()) { d->hud_default_1 = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  // 1 at LoadData
  uint32_t hud_default_2_address() const { return address + 80; }
  uint32_t hud_default_2_bits() const { if (auto* d = direct()) return d->hud_default_2; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_hud_default_2_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_default_2 = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t hud_default_2() const { if (auto* d = direct()) return d->hud_default_2; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_hud_default_2(uint32_t v) const { if (auto* d = direct()) { d->hud_default_2 = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  // 1 at LoadData
  uint32_t hud_default_3_address() const { return address + 84; }
  uint32_t hud_default_3_bits() const { if (auto* d = direct()) return d->hud_default_3; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_hud_default_3_bits(uint32_t v) const { if (auto* d = direct()) { d->hud_default_3 = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t hud_default_3() const { if (auto* d = direct()) return d->hud_default_3; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_hud_default_3(uint32_t v) const { if (auto* d = direct()) { d->hud_default_3 = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  // the volumes' defaults, 15 at LoadData
  uint32_t music_volume_default_address() const { return address + 88; }
  uint32_t music_volume_default_bits() const { if (auto* d = direct()) return d->music_volume_default; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_music_volume_default_bits(uint32_t v) const { if (auto* d = direct()) { d->music_volume_default = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t music_volume_default() const { if (auto* d = direct()) return d->music_volume_default; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_music_volume_default(uint32_t v) const { if (auto* d = direct()) { d->music_volume_default = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  // 15 at LoadData (probably)
  uint32_t effects_volume_default_address() const { return address + 92; }
  uint32_t effects_volume_default_bits() const { if (auto* d = direct()) return d->effects_volume_default; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_effects_volume_default_bits(uint32_t v) const { if (auto* d = direct()) { d->effects_volume_default = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  uint32_t effects_volume_default() const { if (auto* d = direct()) return d->effects_volume_default; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_effects_volume_default(uint32_t v) const { if (auto* d = direct()) { d->effects_volume_default = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  // the three values' defaults, 0 at LoadData
  uint32_t adjust_default_0_address() const { return address + 96; }
  uint32_t adjust_default_0_bits() const { if (auto* d = direct()) return d->adjust_default_0; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_adjust_default_0_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_default_0 = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  uint32_t adjust_default_0() const { if (auto* d = direct()) return d->adjust_default_0; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_adjust_default_0(uint32_t v) const { if (auto* d = direct()) { d->adjust_default_0 = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  // 0 at LoadData
  uint32_t adjust_default_1_address() const { return address + 100; }
  uint32_t adjust_default_1_bits() const { if (auto* d = direct()) return d->adjust_default_1; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_adjust_default_1_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_default_1 = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  uint32_t adjust_default_1() const { if (auto* d = direct()) return d->adjust_default_1; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_adjust_default_1(uint32_t v) const { if (auto* d = direct()) { d->adjust_default_1 = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  // 0 at LoadData
  uint32_t adjust_default_2_address() const { return address + 104; }
  uint32_t adjust_default_2_bits() const { if (auto* d = direct()) return d->adjust_default_2; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(base, address + 104))); }
  void set_adjust_default_2_bits(uint32_t v) const { if (auto* d = direct()) { d->adjust_default_2 = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(base, address + 104, v); }
  uint32_t adjust_default_2() const { if (auto* d = direct()) return d->adjust_default_2; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(base, address + 104))); }
  void set_adjust_default_2(uint32_t v) const { if (auto* d = direct()) { d->adjust_default_2 = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(base, address + 104, v); }
  // the save data's +6116 (kinds 0 to 2)
  uint32_t money_address() const { return address + 108; }
  uint32_t money_bits() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(base, address + 108))); }
  void set_money_bits(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(base, address + 108, v); }
  uint32_t money() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(base, address + 108))); }
  void set_money(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(base, address + 108, v); }
  // the day (save +6100) mod 7
  uint32_t weekday_address() const { return address + 112; }
  uint32_t weekday_bits() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(base, address + 112))); }
  void set_weekday_bits(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(base, address + 112, v); }
  uint32_t weekday() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(base, address + 112))); }
  void set_weekday(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(base, address + 112, v); }
  // the day, at most 999
  uint32_t day_address() const { return address + 116; }
  uint32_t day_bits() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(base, address + 116))); }
  void set_day_bits(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(base, address + 116, v); }
  uint32_t day() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(base, address + 116))); }
  void set_day(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(base, address + 116, v); }
  // Stats::WinStreak, 0 to 999
  uint32_t win_streak_address() const { return address + 120; }
  uint32_t win_streak_bits() const { if (auto* d = direct()) return d->win_streak; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(base, address + 120))); }
  void set_win_streak_bits(uint32_t v) const { if (auto* d = direct()) { d->win_streak = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(base, address + 120, v); }
  uint32_t win_streak() const { if (auto* d = direct()) return d->win_streak; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(base, address + 120))); }
  void set_win_streak(uint32_t v) const { if (auto* d = direct()) { d->win_streak = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(base, address + 120, v); }
  // the car's float +6248, clamped and truncated
  uint32_t car_value_a_address() const { return address + 124; }
  uint32_t car_value_a_bits() const { if (auto* d = direct()) return d->car_value_a; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(base, address + 124))); }
  void set_car_value_a_bits(uint32_t v) const { if (auto* d = direct()) { d->car_value_a = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(base, address + 124, v); }
  uint32_t car_value_a() const { if (auto* d = direct()) return d->car_value_a; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(base, address + 124))); }
  void set_car_value_a(uint32_t v) const { if (auto* d = direct()) { d->car_value_a = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(base, address + 124, v); }
  // the car's float +6244, clamped and truncated
  uint32_t car_value_b_address() const { return address + 128; }
  uint32_t car_value_b_bits() const { if (auto* d = direct()) return d->car_value_b; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(base, address + 128))); }
  void set_car_value_b_bits(uint32_t v) const { if (auto* d = direct()) { d->car_value_b = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(base, address + 128, v); }
  uint32_t car_value_b() const { if (auto* d = direct()) return d->car_value_b; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(base, address + 128))); }
  void set_car_value_b(uint32_t v) const { if (auto* d = direct()) { d->car_value_b = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(base, address + 128, v); }
  // 0 to 8: which items (the table at 0x8200CBF0)
  uint32_t kind_address() const { return address + 132; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(base, address + 132))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(base, address + 132, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(base, address + 132))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(base, address + 132, v); }
  // the kind's items, at most 6
  uint32_t item_count_address() const { return address + 136; }
  uint32_t item_count_bits() const { if (auto* d = direct()) return d->item_count; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(base, address + 136))); }
  void set_item_count_bits(uint32_t v) const { if (auto* d = direct()) { d->item_count = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(base, address + 136, v); }
  uint32_t item_count() const { if (auto* d = direct()) return d->item_count; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(base, address + 136))); }
  void set_item_count(uint32_t v) const { if (auto* d = direct()) { d->item_count = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(base, address + 136, v); }
  // 1 the resume, 2 to 9 the items ending or restarting the race
  uint32_t answer_address() const { return address + 140; }
  uint32_t answer_bits() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(base, address + 140))); }
  void set_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(base, address + 140, v); }
  uint32_t answer() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(base, address + 140))); }
  void set_answer(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(base, address + 140, v); }
  // the sub-pages' last item: 1 OK (the edits kept), 0 Default (the defaults); the arrow's row
  uint32_t last_item_ok_address() const { return address + 144; }
  uint32_t last_item_ok_bits() const { if (auto* d = direct()) return d->last_item_ok; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_last_item_ok_bits(uint32_t v) const { if (auto* d = direct()) { d->last_item_ok = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  uint32_t last_item_ok() const { if (auto* d = direct()) return d->last_item_ok; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(base, address + 144))); }
  void set_last_item_ok(uint32_t v) const { if (auto* d = direct()) { d->last_item_ok = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(base, address + 144, v); }
  // the main list's selection kept while a sub-page shows
  uint32_t main_selection_address() const { return address + 148; }
  uint32_t main_selection_bits() const { if (auto* d = direct()) return d->main_selection; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(base, address + 148))); }
  void set_main_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->main_selection = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(base, address + 148, v); }
  uint32_t main_selection() const { if (auto* d = direct()) return d->main_selection; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(base, address + 148))); }
  void set_main_selection(uint32_t v) const { if (auto* d = direct()) { d->main_selection = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(base, address + 148, v); }
  // PauseMenu::RunTen's count, done at 10
  uint32_t run_count_address() const { return address + 152; }
  uint32_t run_count_bits() const { if (auto* d = direct()) return d->run_count; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(base, address + 152))); }
  void set_run_count_bits(uint32_t v) const { if (auto* d = direct()) { d->run_count = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(base, address + 152, v); }
  uint32_t run_count() const { if (auto* d = direct()) return d->run_count; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(base, address + 152))); }
  void set_run_count(uint32_t v) const { if (auto* d = direct()) { d->run_count = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(base, address + 152, v); }
  // Open's flag (RaceHudMachine::IsFirstDay): the items of id 4 disabled
  uint32_t first_day_address() const { return address + 156; }
  uint32_t first_day_bits() const { if (auto* d = direct()) return d->first_day; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(base, address + 156))); }
  void set_first_day_bits(uint32_t v) const { if (auto* d = direct()) { d->first_day = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(base, address + 156, v); }
  uint32_t first_day() const { if (auto* d = direct()) return d->first_day; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(base, address + 156))); }
  void set_first_day(uint32_t v) const { if (auto* d = direct()) { d->first_day = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(base, address + 156, v); }
};

// allocated; measure: new<40>@RaceDialog::Create
#pragma pack(push, 1)
struct RaceDialogDataData {
  uint32_t yes = 0;  // +0
  uint32_t slot = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
  uint32_t message = 0;  // +12
  uint32_t open_style = 0;  // +16
  uint32_t next_open_style = 0;  // +20
  uint32_t unknown_24 = 0;  // +24
  uint32_t selection = 0;  // +28
  uint32_t showing = 0;  // +32
  uint32_t close_asked = 0;  // +36
};
#pragma pack(pop)
static_assert(sizeof(RaceDialogDataData) == 40);
inline constexpr memory::LayoutField kRaceDialogDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}};
inline constexpr memory::Layout kRaceDialogDataLayout = {kRaceDialogDataFields, 10};

struct RaceDialogData : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceDialogDataData* data() const { return static_cast<RaceDialogDataData*>(memory::HandleObject(address)); }
  static RaceDialogData New(uint8_t* base) {
    return RaceDialogData(base, memory::MakeHandle(new RaceDialogDataData(), sizeof(RaceDialogDataData), "RaceDialogData", &kRaceDialogDataLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceDialogData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceDialogDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceDialogDataData*>(::operator new(sizeof(RaceDialogDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceDialogDataData();
    return RaceDialogData(base, memory::MakeHandle(p, sizeof(RaceDialogDataData) * count, "RaceDialogData", &kRaceDialogDataLayout, sizeof(RaceDialogDataData)));
  }
  RaceDialogData at(uint32_t i) const { return RaceDialogData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceDialogDataData* direct() const { return static_cast<RaceDialogDataData*>(memory::HandleOwn(address, &kRaceDialogDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceDialogDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceDialogDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 1 when the question was answered yes
  uint32_t yes_address() const { return address + 0; }
  uint32_t yes_bits() const { if (auto* d = direct()) return d->yes; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_yes_bits(uint32_t v) const { if (auto* d = direct()) { d->yes = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t yes() const { if (auto* d = direct()) return d->yes; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_yes(uint32_t v) const { if (auto* d = direct()) { d->yes = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // AskSlot's slot (0 a new one)
  uint32_t slot_address() const { return address + 4; }
  uint32_t slot_bits() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t slot() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_slot(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // 0 before Replays::Delete (Question's message 5)
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the message (ShowMessage's jump table, 0 to 11), 13 the question
  uint32_t message_address() const { return address + 12; }
  uint32_t message_bits() const { if (auto* d = direct()) return d->message; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_message_bits(uint32_t v) const { if (auto* d = direct()) { d->message = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t message() const { if (auto* d = direct()) return d->message; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_message(uint32_t v) const { if (auto* d = direct()) { d->message = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // how OpenAnim and ShowMessage open it: 0, 1 (with the list) or 2
  uint32_t open_style_address() const { return address + 16; }
  uint32_t open_style_bits() const { if (auto* d = direct()) return d->open_style; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_open_style_bits(uint32_t v) const { if (auto* d = direct()) { d->open_style = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t open_style() const { if (auto* d = direct()) return d->open_style; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_open_style(uint32_t v) const { if (auto* d = direct()) { d->open_style = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // open_style after OpenAnim
  uint32_t next_open_style_address() const { return address + 20; }
  uint32_t next_open_style_bits() const { if (auto* d = direct()) return d->next_open_style; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_next_open_style_bits(uint32_t v) const { if (auto* d = direct()) { d->next_open_style = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t next_open_style() const { if (auto* d = direct()) return d->next_open_style; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_next_open_style(uint32_t v) const { if (auto* d = direct()) { d->next_open_style = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_24_bits() const { if (auto* d = direct()) return d->unknown_24; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_unknown_24_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_24 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24() const { if (auto* d = direct()) return d->unknown_24; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_unknown_24(uint32_t v) const { if (auto* d = direct()) { d->unknown_24 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // the list's selection at opening
  uint32_t selection_address() const { return address + 28; }
  uint32_t selection_bits() const { if (auto* d = direct()) return d->selection; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->selection = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t selection() const { if (auto* d = direct()) return d->selection; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_selection(uint32_t v) const { if (auto* d = direct()) { d->selection = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // 1 while the dialog shows (RaceDialog::Answer clears it)
  uint32_t showing_address() const { return address + 32; }
  uint32_t showing_bits() const { if (auto* d = direct()) return d->showing; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_showing_bits(uint32_t v) const { if (auto* d = direct()) { d->showing = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t showing() const { if (auto* d = direct()) return d->showing; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_showing(uint32_t v) const { if (auto* d = direct()) { d->showing = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // 1: Update goes to page 5
  uint32_t close_asked_address() const { return address + 36; }
  uint32_t close_asked_bits() const { if (auto* d = direct()) return d->close_asked; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_close_asked_bits(uint32_t v) const { if (auto* d = direct()) { d->close_asked = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t close_asked() const { if (auto* d = direct()) return d->close_asked; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_close_asked(uint32_t v) const { if (auto* d = direct()) { d->close_asked = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
};

// allocated; measure: CareerResults
#pragma pack(push, 1)
struct CareerResultsScreenData {
  uint32_t kind = 0;  // +0
  uint32_t opponent_prize = 0;  // +4
  uint32_t win_prize = 0;  // +8
  float streak_factor = 0;  // +12
  uint32_t prize = 0;  // +16
  uint32_t item_bonus = 0;  // +20
  float entry_value = 0;  // +24
  uint32_t entry_points = 0;  // +28
  uint8_t pairs[128] = {};  // +32, guest order
  uint32_t pair_count = 0;  // +160
  uint32_t money = 0;  // +164
  uint32_t total = 0;  // +168
  uint32_t entry = 0;  // +172
  uint8_t pad_176[4] = {};
  uint32_t movie = 0;  // +180
  uint32_t pages_page = 0;  // +184
  uint32_t pages_next_page = 0;  // +188
  uint32_t pages_previous_page = 0;  // +192
  uint32_t pages_step = 0;  // +196
  uint32_t pages_queued_step = 0;  // +200
  uint32_t pages_unknown_20 = 0;  // +204
  uint32_t pages_step_started = 0;  // +208
};
#pragma pack(pop)
static_assert(sizeof(CareerResultsScreenData) == 212);
inline constexpr memory::LayoutField kCareerResultsScreenFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {180, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}};
inline constexpr memory::Layout kCareerResultsScreenLayout = {kCareerResultsScreenFields, 20};

struct CareerResultsScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 212;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CareerResultsScreenData* data() const { return static_cast<CareerResultsScreenData*>(memory::HandleObject(address)); }
  static CareerResultsScreen New(uint8_t* base) {
    return CareerResultsScreen(base, memory::MakeHandle(new CareerResultsScreenData(), sizeof(CareerResultsScreenData), "CareerResultsScreen", &kCareerResultsScreenLayout));
  }
  template <uint32_t Stride = kSize>
  static CareerResultsScreen NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CareerResultsScreenData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CareerResultsScreenData*>(::operator new(sizeof(CareerResultsScreenData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CareerResultsScreenData();
    return CareerResultsScreen(base, memory::MakeHandle(p, sizeof(CareerResultsScreenData) * count, "CareerResultsScreen", &kCareerResultsScreenLayout, sizeof(CareerResultsScreenData)));
  }
  CareerResultsScreen at(uint32_t i) const { return CareerResultsScreen(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CareerResultsScreenData* direct() const { return static_cast<CareerResultsScreenData*>(memory::HandleOwn(address, &kCareerResultsScreenLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCareerResultsScreenLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCareerResultsScreenLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0 to 2 by the race's result; the transitions' r3 (the step word, +208) is also measured here (a measure fault)
  uint32_t kind_address() const { return address + 0; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the opponents' prizes summed
  uint32_t opponent_prize_address() const { return address + 4; }
  uint32_t opponent_prize_bits() const { if (auto* d = direct()) return d->opponent_prize; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_opponent_prize_bits(uint32_t v) const { if (auto* d = direct()) { d->opponent_prize = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t opponent_prize() const { if (auto* d = direct()) return d->opponent_prize; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_opponent_prize(uint32_t v) const { if (auto* d = direct()) { d->opponent_prize = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the win's prize by the race's time
  uint32_t win_prize_address() const { return address + 8; }
  uint32_t win_prize_bits() const { if (auto* d = direct()) return d->win_prize; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_win_prize_bits(uint32_t v) const { if (auto* d = direct()) { d->win_prize = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t win_prize() const { if (auto* d = direct()) return d->win_prize; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_win_prize(uint32_t v) const { if (auto* d = direct()) { d->win_prize = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // by the day's win streak
  uint32_t streak_factor_address() const { return address + 12; }
  uint32_t streak_factor_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->streak_factor); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_streak_factor_bits(uint32_t v) const { if (auto* d = direct()) { d->streak_factor = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  float streak_factor() const { return std::bit_cast<float>(streak_factor_bits()); }
  void set_streak_factor(float v) const { set_streak_factor_bits(std::bit_cast<uint32_t>(v)); }
  // (opponent_prize + win_prize) x streak_factor
  uint32_t prize_address() const { return address + 16; }
  uint32_t prize_bits() const { if (auto* d = direct()) return d->prize; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_prize_bits(uint32_t v) const { if (auto* d = direct()) { d->prize = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t prize() const { if (auto* d = direct()) return d->prize; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_prize(uint32_t v) const { if (auto* d = direct()) { d->prize = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // the opponents' item bonuses summed
  uint32_t item_bonus_address() const { return address + 20; }
  uint32_t item_bonus_bits() const { if (auto* d = direct()) return d->item_bonus; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_item_bonus_bits(uint32_t v) const { if (auto* d = direct()) { d->item_bonus = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t item_bonus() const { if (auto* d = direct()) return d->item_bonus; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_item_bonus(uint32_t v) const { if (auto* d = direct()) { d->item_bonus = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // the result's float +844; the entries' first
  uint32_t entry_value_address() const { return address + 24; }
  uint32_t entry_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->entry_value); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_entry_value_bits(uint32_t v) const { if (auto* d = direct()) { d->entry_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  float entry_value() const { return std::bit_cast<float>(entry_value_bits()); }
  void set_entry_value(float v) const { set_entry_value_bits(std::bit_cast<uint32_t>(v)); }
  // entry_value x the scale, truncated
  uint32_t entry_points_address() const { return address + 28; }
  uint32_t entry_points_bits() const { if (auto* d = direct()) return d->entry_points; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_entry_points_bits(uint32_t v) const { if (auto* d = direct()) { d->entry_points = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t entry_points() const { if (auto* d = direct()) return d->entry_points; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_entry_points(uint32_t v) const { if (auto* d = direct()) { d->entry_points = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // (float, int) pairs, 8 bytes each, pair_count of them
  uint32_t pairs_address() const { return address + 32; }
  uint8_t* pairs() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 32 : guest::Pointer(base, address + 32); }
  uint32_t pair_count_address() const { return address + 160; }
  uint32_t pair_count_bits() const { if (auto* d = direct()) return d->pair_count; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(base, address + 160))); }
  void set_pair_count_bits(uint32_t v) const { if (auto* d = direct()) { d->pair_count = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(base, address + 160, v); }
  uint32_t pair_count() const { if (auto* d = direct()) return d->pair_count; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(base, address + 160))); }
  void set_pair_count(uint32_t v) const { if (auto* d = direct()) { d->pair_count = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(base, address + 160, v); }
  // the save data's +6116 before the prize
  uint32_t money_address() const { return address + 164; }
  uint32_t money_bits() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(base, address + 164))); }
  void set_money_bits(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(base, address + 164, v); }
  uint32_t money() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(base, address + 164))); }
  void set_money(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(base, address + 164, v); }
  // prize + item_bonus + entry_points + money, at least 0
  uint32_t total_address() const { return address + 168; }
  uint32_t total_bits() const { if (auto* d = direct()) return d->total; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(base, address + 168))); }
  void set_total_bits(uint32_t v) const { if (auto* d = direct()) { d->total = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(base, address + 168, v); }
  uint32_t total() const { if (auto* d = direct()) return d->total; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(base, address + 168))); }
  void set_total(uint32_t v) const { if (auto* d = direct()) { d->total = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(base, address + 168, v); }
  // the entry NextEntry shows
  uint32_t entry_address() const { return address + 172; }
  uint32_t entry_bits() const { if (auto* d = direct()) return d->entry; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(base, address + 172))); }
  void set_entry_bits(uint32_t v) const { if (auto* d = direct()) { d->entry = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(base, address + 172, v); }
  uint32_t entry() const { if (auto* d = direct()) return d->entry; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(base, address + 172))); }
  void set_entry(uint32_t v) const { if (auto* d = direct()) { d->entry = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(base, address + 172, v); }
  // UI movie 6
  uint32_t movie_address() const { return address + 180; }
  uint32_t movie_bits() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(base, address + 180))); }
  void set_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(base, address + 180, v); }
  View movie() const { return View(base, movie_bits()); }
  void set_movie(uint32_t v) const { set_movie_bits(v); }
  // its page object (PageObject) at +184: the page
  uint32_t pages_page_address() const { return address + 184; }
  uint32_t pages_page_bits() const { if (auto* d = direct()) return d->pages_page; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(base, address + 184))); }
  void set_pages_page_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_page = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(base, address + 184, v); }
  uint32_t pages_page() const { if (auto* d = direct()) return d->pages_page; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(base, address + 184))); }
  void set_pages_page(uint32_t v) const { if (auto* d = direct()) { d->pages_page = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(base, address + 184, v); }
  uint32_t pages_next_page_address() const { return address + 188; }
  uint32_t pages_next_page_bits() const { if (auto* d = direct()) return d->pages_next_page; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(base, address + 188))); }
  void set_pages_next_page_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_next_page = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(base, address + 188, v); }
  uint32_t pages_next_page() const { if (auto* d = direct()) return d->pages_next_page; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(base, address + 188))); }
  void set_pages_next_page(uint32_t v) const { if (auto* d = direct()) { d->pages_next_page = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(base, address + 188, v); }
  uint32_t pages_previous_page_address() const { return address + 192; }
  uint32_t pages_previous_page_bits() const { if (auto* d = direct()) return d->pages_previous_page; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_pages_previous_page_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_previous_page = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  uint32_t pages_previous_page() const { if (auto* d = direct()) return d->pages_previous_page; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_pages_previous_page(uint32_t v) const { if (auto* d = direct()) { d->pages_previous_page = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  // the running transition, -1 none
  uint32_t pages_step_address() const { return address + 196; }
  uint32_t pages_step_bits() const { if (auto* d = direct()) return d->pages_step; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_pages_step_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_step = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  uint32_t pages_step() const { if (auto* d = direct()) return d->pages_step; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(base, address + 196))); }
  void set_pages_step(uint32_t v) const { if (auto* d = direct()) { d->pages_step = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(base, address + 196, v); }
  uint32_t pages_queued_step_address() const { return address + 200; }
  uint32_t pages_queued_step_bits() const { if (auto* d = direct()) return d->pages_queued_step; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(base, address + 200))); }
  void set_pages_queued_step_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_queued_step = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(base, address + 200, v); }
  uint32_t pages_queued_step() const { if (auto* d = direct()) return d->pages_queued_step; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(base, address + 200))); }
  void set_pages_queued_step(uint32_t v) const { if (auto* d = direct()) { d->pages_queued_step = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(base, address + 200, v); }
  uint32_t pages_unknown_20_address() const { return address + 204; }
  uint32_t pages_unknown_20_bits() const { if (auto* d = direct()) return d->pages_unknown_20; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(base, address + 204))); }
  void set_pages_unknown_20_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_unknown_20 = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(base, address + 204, v); }
  uint32_t pages_unknown_20() const { if (auto* d = direct()) return d->pages_unknown_20; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(base, address + 204))); }
  void set_pages_unknown_20(uint32_t v) const { if (auto* d = direct()) { d->pages_unknown_20 = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(base, address + 204, v); }
  uint32_t pages_step_started_address() const { return address + 208; }
  uint32_t pages_step_started_bits() const { if (auto* d = direct()) return d->pages_step_started; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(base, address + 208))); }
  void set_pages_step_started_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_step_started = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(base, address + 208, v); }
  uint32_t pages_step_started() const { if (auto* d = direct()) return d->pages_step_started; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(base, address + 208))); }
  void set_pages_step_started(uint32_t v) const { if (auto* d = direct()) { d->pages_step_started = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(base, address + 208, v); }
};

// allocated; measure: new<4>@AfterRaceMenu::Create, AfterRaceMenu
#pragma pack(push, 1)
struct AfterRaceMenuScreenData {
  uint32_t movie = 0;  // +0
  uint8_t pad_4[2] = {};
  uint16_t fault_replay_day = 0;  // +6
  uint16_t fault_replay_hour = 0;  // +8
  uint16_t fault_replay_minute = 0;  // +10
  uint8_t pad_12[4] = {};
  uint32_t fault_replay_time = 0;  // +16
  uint32_t fault_replay_mode = 0;  // +20
  uint32_t fault_replay_course = 0;  // +24
  uint32_t fault_replay_number = 0;  // +28
  uint32_t fault_replay_result = 0;  // +32
};
#pragma pack(pop)
static_assert(sizeof(AfterRaceMenuScreenData) == 36);
inline constexpr memory::LayoutField kAfterRaceMenuScreenFields[] = {{0, 4}, {6, 2}, {8, 2}, {10, 2}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}};
inline constexpr memory::Layout kAfterRaceMenuScreenLayout = {kAfterRaceMenuScreenFields, 9};

struct AfterRaceMenuScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AfterRaceMenuScreenData* data() const { return static_cast<AfterRaceMenuScreenData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AfterRaceMenuScreen New(uint8_t* base) {
    return AfterRaceMenuScreen(base, memory::MakeHandle(new AfterRaceMenuScreenData(), sizeof(AfterRaceMenuScreenData), "AfterRaceMenuScreen", &kAfterRaceMenuScreenLayout));
  }
  template <uint32_t Stride = kSize>
  static AfterRaceMenuScreen NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AfterRaceMenuScreenData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AfterRaceMenuScreenData*>(::operator new(sizeof(AfterRaceMenuScreenData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AfterRaceMenuScreenData();
    return AfterRaceMenuScreen(base, memory::MakeHandle(p, sizeof(AfterRaceMenuScreenData) * count, "AfterRaceMenuScreen", &kAfterRaceMenuScreenLayout, sizeof(AfterRaceMenuScreenData)));
  }
  AfterRaceMenuScreen at(uint32_t i) const { return AfterRaceMenuScreen(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AfterRaceMenuScreenData* direct() const { return static_cast<AfterRaceMenuScreenData*>(memory::HandleOwn(address, &kAfterRaceMenuScreenLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAfterRaceMenuScreenLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAfterRaceMenuScreenLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // UI movie 9 (UiMovies::Find)
  uint32_t movie_address() const { return address + 0; }
  uint32_t movie_bits() const { if (auto* d = direct()) return d->movie; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->movie = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  View movie() const { return View(base, movie_bits()); }
  void set_movie(uint32_t v) const { set_movie_bits(v); }
  // measure fault: a replay record's halfwords +0 to +10, the date and time (probably a SYSTEMTIME's)
  uint32_t fault_replay_year_address() const { return address + 0; }
  uint16_t fault_replay_year_bits() const { if (auto* d = direct()) return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kAfterRaceMenuScreenLayout, 0, 2)); return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(base, address + 0))); }
  void set_fault_replay_year_bits(uint16_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAfterRaceMenuScreenLayout, 0, 2, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(base, address + 0, v); }
  uint16_t fault_replay_year() const { return uint16_t(fault_replay_year_bits()); }
  void set_fault_replay_year(uint16_t v) const { set_fault_replay_year_bits(uint16_t(v)); }
  // measure fault
  uint32_t fault_replay_month_address() const { return address + 2; }
  uint16_t fault_replay_month_bits() const { if (auto* d = direct()) return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kAfterRaceMenuScreenLayout, 2, 2)); return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(base, address + 2))); }
  void set_fault_replay_month_bits(uint16_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAfterRaceMenuScreenLayout, 2, 2, v); return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(base, address + 2, v); }
  uint16_t fault_replay_month() const { return uint16_t(fault_replay_month_bits()); }
  void set_fault_replay_month(uint16_t v) const { set_fault_replay_month_bits(uint16_t(v)); }
  // measure fault
  uint32_t fault_replay_day_address() const { return address + 6; }
  uint16_t fault_replay_day_bits() const { if (auto* d = direct()) return d->fault_replay_day; return uint16_t((memory::IsHandle(address + 6) ? uint16_t(memory::OwnerLoad(address + 6, 2)) : guest::LoadU16(base, address + 6))); }
  void set_fault_replay_day_bits(uint16_t v) const { if (auto* d = direct()) { d->fault_replay_day = v; return; } if (memory::IsHandle(address + 6)) memory::OwnerStore(address + 6, 2, v); else guest::StoreU16(base, address + 6, v); }
  uint16_t fault_replay_day() const { if (auto* d = direct()) return d->fault_replay_day; return uint16_t((memory::IsHandle(address + 6) ? uint16_t(memory::OwnerLoad(address + 6, 2)) : guest::LoadU16(base, address + 6))); }
  void set_fault_replay_day(uint16_t v) const { if (auto* d = direct()) { d->fault_replay_day = v; return; } if (memory::IsHandle(address + 6)) memory::OwnerStore(address + 6, 2, v); else guest::StoreU16(base, address + 6, v); }
  // measure fault
  uint32_t fault_replay_hour_address() const { return address + 8; }
  uint16_t fault_replay_hour_bits() const { if (auto* d = direct()) return d->fault_replay_hour; return uint16_t((memory::IsHandle(address + 8) ? uint16_t(memory::OwnerLoad(address + 8, 2)) : guest::LoadU16(base, address + 8))); }
  void set_fault_replay_hour_bits(uint16_t v) const { if (auto* d = direct()) { d->fault_replay_hour = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 2, v); else guest::StoreU16(base, address + 8, v); }
  uint16_t fault_replay_hour() const { if (auto* d = direct()) return d->fault_replay_hour; return uint16_t((memory::IsHandle(address + 8) ? uint16_t(memory::OwnerLoad(address + 8, 2)) : guest::LoadU16(base, address + 8))); }
  void set_fault_replay_hour(uint16_t v) const { if (auto* d = direct()) { d->fault_replay_hour = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 2, v); else guest::StoreU16(base, address + 8, v); }
  // measure fault
  uint32_t fault_replay_minute_address() const { return address + 10; }
  uint16_t fault_replay_minute_bits() const { if (auto* d = direct()) return d->fault_replay_minute; return uint16_t((memory::IsHandle(address + 10) ? uint16_t(memory::OwnerLoad(address + 10, 2)) : guest::LoadU16(base, address + 10))); }
  void set_fault_replay_minute_bits(uint16_t v) const { if (auto* d = direct()) { d->fault_replay_minute = v; return; } if (memory::IsHandle(address + 10)) memory::OwnerStore(address + 10, 2, v); else guest::StoreU16(base, address + 10, v); }
  uint16_t fault_replay_minute() const { if (auto* d = direct()) return d->fault_replay_minute; return uint16_t((memory::IsHandle(address + 10) ? uint16_t(memory::OwnerLoad(address + 10, 2)) : guest::LoadU16(base, address + 10))); }
  void set_fault_replay_minute(uint16_t v) const { if (auto* d = direct()) { d->fault_replay_minute = v; return; } if (memory::IsHandle(address + 10)) memory::OwnerStore(address + 10, 2, v); else guest::StoreU16(base, address + 10, v); }
  // measure fault: minutes, seconds and thousandths packed
  uint32_t fault_replay_time_address() const { return address + 16; }
  uint32_t fault_replay_time_bits() const { if (auto* d = direct()) return d->fault_replay_time; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_fault_replay_time_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_time = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t fault_replay_time() const { if (auto* d = direct()) return d->fault_replay_time; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_fault_replay_time(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_time = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // measure fault
  uint32_t fault_replay_mode_address() const { return address + 20; }
  uint32_t fault_replay_mode_bits() const { if (auto* d = direct()) return d->fault_replay_mode; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_fault_replay_mode_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_mode = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t fault_replay_mode() const { if (auto* d = direct()) return d->fault_replay_mode; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_fault_replay_mode(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_mode = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // measure fault
  uint32_t fault_replay_course_address() const { return address + 24; }
  uint32_t fault_replay_course_bits() const { if (auto* d = direct()) return d->fault_replay_course; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_fault_replay_course_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_course = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t fault_replay_course() const { if (auto* d = direct()) return d->fault_replay_course; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_fault_replay_course(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_course = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // measure fault: the replay's number, 0 for none
  uint32_t fault_replay_number_address() const { return address + 28; }
  uint32_t fault_replay_number_bits() const { if (auto* d = direct()) return d->fault_replay_number; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_fault_replay_number_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_number = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t fault_replay_number() const { if (auto* d = direct()) return d->fault_replay_number; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_fault_replay_number(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_number = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // measure fault
  uint32_t fault_replay_result_address() const { return address + 32; }
  uint32_t fault_replay_result_bits() const { if (auto* d = direct()) return d->fault_replay_result; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_fault_replay_result_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_result = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t fault_replay_result() const { if (auto* d = direct()) return d->fault_replay_result; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_fault_replay_result(uint32_t v) const { if (auto* d = direct()) { d->fault_replay_result = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
};

// allocated; measure: new<24>@AfterRaceMenu::Create
#pragma pack(push, 1)
struct AfterRaceMenuDataData {
  uint32_t kind = 0;  // +0
  uint32_t item_count = 0;  // +4
  uint32_t no_replay = 0;  // +8
  uint32_t answer = 0;  // +12
  uint32_t selection = 0;  // +16
  uint32_t unknown_20 = 0;  // +20
};
#pragma pack(pop)
static_assert(sizeof(AfterRaceMenuDataData) == 24);
inline constexpr memory::LayoutField kAfterRaceMenuDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}};
inline constexpr memory::Layout kAfterRaceMenuDataLayout = {kAfterRaceMenuDataFields, 6};

struct AfterRaceMenuData : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  AfterRaceMenuDataData* data() const { return static_cast<AfterRaceMenuDataData*>(memory::HandleObject(address)); }
  static AfterRaceMenuData New(uint8_t* base) {
    return AfterRaceMenuData(base, memory::MakeHandle(new AfterRaceMenuDataData(), sizeof(AfterRaceMenuDataData), "AfterRaceMenuData", &kAfterRaceMenuDataLayout));
  }
  template <uint32_t Stride = kSize>
  static AfterRaceMenuData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AfterRaceMenuDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AfterRaceMenuDataData*>(::operator new(sizeof(AfterRaceMenuDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AfterRaceMenuDataData();
    return AfterRaceMenuData(base, memory::MakeHandle(p, sizeof(AfterRaceMenuDataData) * count, "AfterRaceMenuData", &kAfterRaceMenuDataLayout, sizeof(AfterRaceMenuDataData)));
  }
  AfterRaceMenuData at(uint32_t i) const { return AfterRaceMenuData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AfterRaceMenuDataData* direct() const { return static_cast<AfterRaceMenuDataData*>(memory::HandleOwn(address, &kAfterRaceMenuDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAfterRaceMenuDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAfterRaceMenuDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0 to 6: which items (the table at 0x82010428)
  uint32_t kind_address() const { return address + 0; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the kind's items, at most 4
  uint32_t item_count_address() const { return address + 4; }
  uint32_t item_count_bits() const { if (auto* d = direct()) return d->item_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_item_count_bits(uint32_t v) const { if (auto* d = direct()) { d->item_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t item_count() const { if (auto* d = direct()) return d->item_count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_item_count(uint32_t v) const { if (auto* d = direct()) { d->item_count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // Open's flag: items 1 and 2 (the replay's) disabled
  uint32_t no_replay_address() const { return address + 8; }
  uint32_t no_replay_bits() const { if (auto* d = direct()) return d->no_replay; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_no_replay_bits(uint32_t v) const { if (auto* d = direct()) { d->no_replay = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t no_replay() const { if (auto* d = direct()) return d->no_replay; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_no_replay(uint32_t v) const { if (auto* d = direct()) { d->no_replay = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t answer_address() const { return address + 12; }
  uint32_t answer_bits() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t answer() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_answer(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the main list's selection kept
  uint32_t selection_address() const { return address + 16; }
  uint32_t selection_bits() const { if (auto* d = direct()) return d->selection; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->selection = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t selection() const { if (auto* d = direct()) return d->selection; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_selection(uint32_t v) const { if (auto* d = direct()) { d->selection = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // 0 at Open; 1 from sub_82211E90, 0 from sub_82211F18
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_20_bits() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_unknown_20_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_unknown_20(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
};

// The image words docs/functions/race-screens.md's code uses.
struct RaceScreensGlobals {
  template <typename T = PauseMenuScreen>
  static T pause_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84D0u)); }
  static void set_pause_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84D0u, v.address); }
  static void set_pause_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84D0u, v); }
  static constexpr uint32_t pause_screen_address() { return 0x826D84D0u; }
  template <typename T = PauseMenuData>
  static T pause_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84D4u)); }
  static void set_pause_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84D4u, v.address); }
  static void set_pause_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84D4u, v); }
  static constexpr uint32_t pause_data_address() { return 0x826D84D4u; }
  template <typename T = PageObject>
  static T pause_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84D8u)); }
  static void set_pause_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84D8u, v.address); }
  static void set_pause_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84D8u, v); }
  static constexpr uint32_t pause_pages_address() { return 0x826D84D8u; }
  template <typename T = MenuList>
  static T pause_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84DCu)); }
  static void set_pause_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84DCu, v.address); }
  static void set_pause_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84DCu, v); }
  static constexpr uint32_t pause_list_address() { return 0x826D84DCu; }
  template <typename T = MenuList>
  static T dialog_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F4u)); }
  static void set_dialog_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F4u, v.address); }
  static void set_dialog_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F4u, v); }
  static constexpr uint32_t dialog_list_address() { return 0x826D84F4u; }
  template <typename T = PageObject>
  static T dialog_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F8u)); }
  static void set_dialog_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F8u, v.address); }
  static void set_dialog_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F8u, v); }
  static constexpr uint32_t dialog_pages_address() { return 0x826D84F8u; }
  template <typename T = RaceDialogData>
  static T dialog_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84FCu)); }
  static void set_dialog_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84FCu, v.address); }
  static void set_dialog_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84FCu, v); }
  static constexpr uint32_t dialog_data_address() { return 0x826D84FCu; }
  template <typename T = CareerResultsScreen>
  static T career_results(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8500u)); }
  static void set_career_results(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8500u, v.address); }
  static void set_career_results(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8500u, v); }
  static constexpr uint32_t career_results_address() { return 0x826D8500u; }
  template <typename T = AfterRaceMenuScreen>
  static T after_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8510u)); }
  static void set_after_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8510u, v.address); }
  static void set_after_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8510u, v); }
  static constexpr uint32_t after_screen_address() { return 0x826D8510u; }
  template <typename T = AfterRaceMenuData>
  static T after_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8514u)); }
  static void set_after_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8514u, v.address); }
  static void set_after_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8514u, v); }
  static constexpr uint32_t after_data_address() { return 0x826D8514u; }
  template <typename T = PageObject>
  static T after_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8518u)); }
  static void set_after_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8518u, v.address); }
  static void set_after_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8518u, v); }
  static constexpr uint32_t after_pages_address() { return 0x826D8518u; }
  template <typename T = MenuList>
  static T after_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D851Cu)); }
  static void set_after_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D851Cu, v.address); }
  static void set_after_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D851Cu, v); }
  static constexpr uint32_t after_list_address() { return 0x826D851Cu; }
};

}  // namespace schema
