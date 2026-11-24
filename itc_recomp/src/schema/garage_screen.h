// The views of docs/functions/garage-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/garage-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: GarageScreen
struct GarageScreen : View {
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
  // 1 once movie 5 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForLoad go to state 9 (ScreenMachine::Reenter); the mode's Init sets 1
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 5 (UiMovies::Delete) and clears `loaded`
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// embedded; measure: GarageScreen::Close.r4, GarageScreen::OnNo.r4, GarageScreen::OnPlateCancelled.r4, GarageScreen::OnPlateDone.r4
#pragma pack(push, 1)
struct GarageScreenModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(GarageScreenModeData) == 812);
inline constexpr memory::LayoutField kGarageScreenModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kGarageScreenModeLayout = {kGarageScreenModeFields, 2};

struct GarageScreenMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  GarageScreenModeData* data() const { return static_cast<GarageScreenModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static GarageScreenMode New(uint8_t* base) {
    return GarageScreenMode(base, memory::MakeHandle(new GarageScreenModeData(), sizeof(GarageScreenModeData), "GarageScreenMode", &kGarageScreenModeLayout));
  }
  template <uint32_t Stride = kSize>
  static GarageScreenMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(GarageScreenModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<GarageScreenModeData*>(::operator new(sizeof(GarageScreenModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) GarageScreenModeData();
    return GarageScreenMode(base, memory::MakeHandle(p, sizeof(GarageScreenModeData) * count, "GarageScreenMode", &kGarageScreenModeLayout, sizeof(GarageScreenModeData)));
  }
  GarageScreenMode at(uint32_t i) const { return GarageScreenMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kGarageScreenModeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kGarageScreenModeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
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

// allocated; measure: new<104>@GarageScreen::Create
#pragma pack(push, 1)
struct GarageScreenDataData {
  uint32_t set_up = 0;  // +0
  uint8_t pick[20] = {};  // +4, guest order
  uint32_t money = 0;  // +24
  uint32_t day = 0;  // +28
  uint32_t weekday = 0;  // +32
  uint32_t save = 0;  // +36
  uint8_t pad_40[4] = {};
  uint32_t selection = 0;  // +44
  uint32_t scroll = 0;  // +48
  uint32_t action = 0;  // +52
  uint32_t unknown_56 = 0;  // +56
  uint32_t plate_pending = 0;  // +60
  uint32_t answered_yes = 0;  // +64
  uint32_t question = 0;  // +68
  uint32_t stand_shown = 0;  // +72
  uint32_t stand_model = 0;  // +76
  uint32_t toggles_shown = 0;  // +80
  uint32_t busy_shown = 0;  // +84
  uint32_t turning = 0;  // +88
  uint32_t doors = 0;  // +92
  uint32_t doors2 = 0;  // +96
  uint32_t sale_price = 0;  // +100
};
#pragma pack(pop)
static_assert(sizeof(GarageScreenDataData) == 104);
inline constexpr memory::LayoutField kGarageScreenDataFields[] = {{0, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}};
inline constexpr memory::Layout kGarageScreenDataLayout = {kGarageScreenDataFields, 20};

struct GarageScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 104;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  GarageScreenDataData* data() const { return static_cast<GarageScreenDataData*>(memory::HandleObject(address)); }
  static GarageScreenData New(uint8_t* base) {
    return GarageScreenData(base, memory::MakeHandle(new GarageScreenDataData(), sizeof(GarageScreenDataData), "GarageScreenData", &kGarageScreenDataLayout));
  }
  template <uint32_t Stride = kSize>
  static GarageScreenData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(GarageScreenDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<GarageScreenDataData*>(::operator new(sizeof(GarageScreenDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) GarageScreenDataData();
    return GarageScreenData(base, memory::MakeHandle(p, sizeof(GarageScreenDataData) * count, "GarageScreenData", &kGarageScreenDataLayout, sizeof(GarageScreenDataData)));
  }
  GarageScreenData at(uint32_t i) const { return GarageScreenData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kGarageScreenDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kGarageScreenDataLayout, offset, width, v); }
  // 1 once the header is set (SetHeader)
  uint32_t set_up_address() const { return address + 0; }
  uint32_t set_up() const { return data()->set_up; }
  void set_set_up(uint32_t v) const { data()->set_up = v; }
  uint32_t set_up_bits() const { return data()->set_up; }
  void set_set_up_bits(uint32_t v) const { data()->set_up = v; }
  // the career's pick (player 0's save data +8 to +27, shown as a name)
  uint32_t pick_address() const { return address + 4; }
  uint8_t* pick() const { return reinterpret_cast<uint8_t*>(data()) + 4; }
  // save data +6116; the sale adds its price
  uint32_t money_address() const { return address + 24; }
  uint32_t money() const { return data()->money; }
  void set_money(uint32_t v) const { data()->money = v; }
  uint32_t money_bits() const { return data()->money; }
  void set_money_bits(uint32_t v) const { data()->money = v; }
  // save data +6100, at most 999
  uint32_t day_address() const { return address + 28; }
  uint32_t day() const { return data()->day; }
  void set_day(uint32_t v) const { data()->day = v; }
  uint32_t day_bits() const { return data()->day; }
  void set_day_bits(uint32_t v) const { data()->day = v; }
  // the day mod 7
  uint32_t weekday_address() const { return address + 32; }
  uint32_t weekday() const { return data()->weekday; }
  void set_weekday(uint32_t v) const { data()->weekday = v; }
  uint32_t weekday_bits() const { return data()->weekday; }
  void set_weekday_bits(uint32_t v) const { data()->weekday = v; }
  // player 0's save data (SaveData::Of)
  uint32_t save_address() const { return address + 36; }
  template <typename T = SaveData>
  T save() const { return T(base, data()->save); }
  void set_save(const View& w) const { const uint32_t v = w.address; data()->save = v; }
  void set_save(uint32_t v) const { data()->save = v; }
  uint32_t save_bits() const { return data()->save; }
  void set_save_bits(uint32_t v) const { data()->save = v; }
  // the selected row (+ scroll: the car's index in the garage)
  uint32_t selection_address() const { return address + 44; }
  uint32_t selection() const { return data()->selection; }
  void set_selection(uint32_t v) const { data()->selection = v; }
  uint32_t selection_bits() const { return data()->selection; }
  void set_selection_bits(uint32_t v) const { data()->selection = v; }
  // the list's scroll
  uint32_t scroll_address() const { return address + 48; }
  uint32_t scroll() const { return data()->scroll; }
  void set_scroll(uint32_t v) const { data()->scroll = v; }
  uint32_t scroll_bits() const { return data()->scroll; }
  void set_scroll_bits(uint32_t v) const { data()->scroll = v; }
  // the action to select when the actions are set up again
  uint32_t action_address() const { return address + 52; }
  uint32_t action() const { return data()->action; }
  void set_action(uint32_t v) const { data()->action = v; }
  uint32_t action_bits() const { return data()->action; }
  void set_action_bits(uint32_t v) const { data()->action = v; }
  // 0 (Transition4)
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_56() const { return data()->unknown_56; }
  void set_unknown_56(uint32_t v) const { data()->unknown_56 = v; }
  uint32_t unknown_56_bits() const { return data()->unknown_56; }
  void set_unknown_56_bits(uint32_t v) const { data()->unknown_56 = v; }
  // 1 while the plate editor's question is pending
  uint32_t plate_pending_address() const { return address + 60; }
  uint32_t plate_pending() const { return data()->plate_pending; }
  void set_plate_pending(uint32_t v) const { data()->plate_pending = v; }
  uint32_t plate_pending_bits() const { return data()->plate_pending; }
  void set_plate_pending_bits(uint32_t v) const { data()->plate_pending = v; }
  // the dialog answered yes
  uint32_t answered_yes_address() const { return address + 64; }
  uint32_t answered_yes() const { return data()->answered_yes; }
  void set_answered_yes(uint32_t v) const { data()->answered_yes = v; }
  uint32_t answered_yes_bits() const { return data()->answered_yes; }
  void set_answered_yes_bits(uint32_t v) const { data()->answered_yes = v; }
  // the question asked: 0 the new plate, 1 make the car current, 2 sell it
  uint32_t question_address() const { return address + 68; }
  uint32_t question() const { return data()->question; }
  void set_question(uint32_t v) const { data()->question = v; }
  uint32_t question_bits() const { return data()->question; }
  void set_question_bits(uint32_t v) const { data()->question = v; }
  // the stand shown (Update loads and draws the car)
  uint32_t stand_shown_address() const { return address + 72; }
  uint32_t stand_shown() const { return data()->stand_shown; }
  void set_stand_shown(uint32_t v) const { data()->stand_shown = v; }
  uint32_t stand_shown_bits() const { return data()->stand_shown; }
  void set_stand_shown_bits(uint32_t v) const { data()->stand_shown = v; }
  // GarageStage::ReloadModel's answer for the shown car (0 until loaded)
  uint32_t stand_model_address() const { return address + 76; }
  uint32_t stand_model() const { return data()->stand_model; }
  void set_stand_model(uint32_t v) const { data()->stand_model = v; }
  uint32_t stand_model_bits() const { return data()->stand_model; }
  void set_stand_model_bits(uint32_t v) const { data()->stand_model = v; }
  // the view's options 1 and 2 (the doors) offered: the car spec's byte +771 is 1 (SetUpOptions)
  uint32_t toggles_shown_address() const { return address + 80; }
  uint32_t toggles_shown() const { return data()->toggles_shown; }
  void set_toggles_shown(uint32_t v) const { data()->toggles_shown = v; }
  uint32_t toggles_shown_bits() const { return data()->toggles_shown; }
  void set_toggles_shown_bits(uint32_t v) const { data()->toggles_shown = v; }
  // the stand's busy marker shown
  uint32_t busy_shown_address() const { return address + 84; }
  uint32_t busy_shown() const { return data()->busy_shown; }
  void set_busy_shown(uint32_t v) const { data()->busy_shown = v; }
  uint32_t busy_shown_bits() const { return data()->busy_shown; }
  void set_busy_shown_bits(uint32_t v) const { data()->busy_shown = v; }
  // the car turning on the stand (ToggleTurn)
  uint32_t turning_address() const { return address + 88; }
  uint32_t turning() const { return data()->turning; }
  void set_turning(uint32_t v) const { data()->turning = v; }
  uint32_t turning_bits() const { return data()->turning; }
  void set_turning_bits(uint32_t v) const { data()->turning = v; }
  // the first door animation open (ToggleDoors)
  uint32_t doors_address() const { return address + 92; }
  uint32_t doors() const { return data()->doors; }
  void set_doors(uint32_t v) const { data()->doors = v; }
  uint32_t doors_bits() const { return data()->doors; }
  void set_doors_bits(uint32_t v) const { data()->doors = v; }
  // the second door animation open (ToggleDoors2)
  uint32_t doors2_address() const { return address + 96; }
  uint32_t doors2() const { return data()->doors2; }
  void set_doors2(uint32_t v) const { data()->doors2 = v; }
  uint32_t doors2_bits() const { return data()->doors2; }
  void set_doors2_bits(uint32_t v) const { data()->doors2 = v; }
  // SalePrice's answer (SetSaleQuestion)
  uint32_t sale_price_address() const { return address + 100; }
  uint32_t sale_price() const { return data()->sale_price; }
  void set_sale_price(uint32_t v) const { data()->sale_price = v; }
  uint32_t sale_price_bits() const { return data()->sale_price; }
  void set_sale_price_bits(uint32_t v) const { data()->sale_price = v; }
};

// file; measure: Params::GetCarSpec.ret
struct CarSpec : View {
  using View::View;
  static constexpr uint32_t kSize = 8432;
  // the car id (block 1's head)
  uint32_t id_address() const { return address + 0; }
  // block 1 +356
  uint32_t drive_type() const { return guest::LoadU32(base, address + 356); }
  void set_drive_type(uint32_t v) const { guest::StoreU32(base, address + 356, v); }
  uint32_t drive_type_address() const { return address + 356; }
  // an index into 0x8260181C
  uint32_t drive_record() const { return guest::LoadU32(base, address + 360); }
  void set_drive_record(uint32_t v) const { guest::StoreU32(base, address + 360, v); }
  uint32_t drive_record_address() const { return address + 360; }
  float front_torque_share() const { return guest::LoadFloat(base, address + 364); }
  void set_front_torque_share(float v) const { guest::StoreFloat(base, address + 364, v); }
  uint32_t front_torque_share_address() const { return address + 364; }
  float unknown_368() const { return guest::LoadFloat(base, address + 368); }
  void set_unknown_368(float v) const { guest::StoreFloat(base, address + 368, v); }
  uint32_t unknown_368_address() const { return address + 368; }
  float final_drive() const { return guest::LoadFloat(base, address + 372); }
  void set_final_drive(float v) const { guest::StoreFloat(base, address + 372, v); }
  uint32_t final_drive_address() const { return address + 372; }
  // 9 floats: reverse, then up to 8 forward ratios
  uint32_t gear_ratios_address() const { return address + 384; }
  // block 1's floats +528 to +636 (read as words)
  uint32_t unknown_632() const { return guest::LoadU32(base, address + 632); }
  void set_unknown_632(uint32_t v) const { guest::StoreU32(base, address + 632, v); }
  uint32_t unknown_632_address() const { return address + 632; }
  uint32_t unknown_636() const { return guest::LoadU32(base, address + 636); }
  void set_unknown_636(uint32_t v) const { guest::StoreU32(base, address + 636, v); }
  uint32_t unknown_636_address() const { return address + 636; }
  // block 2's car id
  uint32_t block2_id_address() const { return address + 640; }
  // block 2 +8 (cc)
  uint32_t displacement() const { return guest::LoadU32(base, address + 648); }
  void set_displacement(uint32_t v) const { guest::StoreU32(base, address + 648, v); }
  uint32_t displacement_address() const { return address + 648; }
  // block 2 +68 (cylinders for the tuning)
  uint8_t engine_type() const { return guest::LoadU8(base, address + 708); }
  void set_engine_type(uint8_t v) const { guest::StoreU8(base, address + 708, v); }
  uint32_t engine_type_address() const { return address + 708; }
  // block 2 +69: 1 for turbo
  uint8_t turbo() const { return guest::LoadU8(base, address + 709); }
  void set_turbo(uint8_t v) const { guest::StoreU8(base, address + 709, v); }
  uint32_t turbo_address() const { return address + 709; }
  // CheckLimits: at most 4700
  uint32_t unknown_716() const { return guest::LoadU32(base, address + 716); }
  void set_unknown_716(uint32_t v) const { guest::StoreU32(base, address + 716, v); }
  uint32_t unknown_716_address() const { return address + 716; }
  // CheckLimits: at most 2000
  uint32_t unknown_720() const { return guest::LoadU32(base, address + 720); }
  void set_unknown_720(uint32_t v) const { guest::StoreU32(base, address + 720, v); }
  uint32_t unknown_720_address() const { return address + 720; }
  // CheckLimits: at most 1700
  uint32_t unknown_724() const { return guest::LoadU32(base, address + 724); }
  void set_unknown_724(uint32_t v) const { guest::StoreU32(base, address + 724, v); }
  uint32_t unknown_724_address() const { return address + 724; }
  // the car's price (Car::SetModelAndClass; the garage car's +1900)
  uint32_t price() const { return guest::LoadU32(base, address + 744); }
  void set_price(uint32_t v) const { guest::StoreU32(base, address + 744, v); }
  uint32_t price_address() const { return address + 744; }
  // Car::SetModelAndClass (the garage car's +1884)
  uint32_t class_word() const { return guest::LoadU32(base, address + 748); }
  void set_class_word(uint32_t v) const { guest::StoreU32(base, address + 748, v); }
  uint32_t class_word_address() const { return address + 748; }
  // string 1934 + it: the maker's name
  uint8_t maker() const { return guest::LoadU8(base, address + 769); }
  void set_maker(uint8_t v) const { guest::StoreU8(base, address + 769, v); }
  uint32_t maker_address() const { return address + 769; }
  // string 421 + it (CarDealer::SetSpecTexts)
  uint8_t unknown_770() const { return guest::LoadU8(base, address + 770); }
  void set_unknown_770(uint8_t v) const { guest::StoreU8(base, address + 770, v); }
  uint32_t unknown_770_address() const { return address + 770; }
  // 0 the first car list, 1 the second (the garage screen's door toggles with 1), others not sold
  uint8_t list_kind() const { return guest::LoadU8(base, address + 771); }
  void set_list_kind(uint8_t v) const { guest::StoreU8(base, address + 771, v); }
  uint32_t list_kind_address() const { return address + 771; }
  uint32_t unknown_772() const { return guest::LoadU32(base, address + 772); }
  void set_unknown_772(uint32_t v) const { guest::StoreU32(base, address + 772, v); }
  uint32_t unknown_772_address() const { return address + 772; }
  uint32_t unknown_776() const { return guest::LoadU32(base, address + 776); }
  void set_unknown_776(uint32_t v) const { guest::StoreU32(base, address + 776, v); }
  uint32_t unknown_776_address() const { return address + 776; }
  uint32_t unknown_780() const { return guest::LoadU32(base, address + 780); }
  void set_unknown_780(uint32_t v) const { guest::StoreU32(base, address + 780, v); }
  uint32_t unknown_780_address() const { return address + 780; }
  uint32_t unknown_784() const { return guest::LoadU32(base, address + 784); }
  void set_unknown_784(uint32_t v) const { guest::StoreU32(base, address + 784, v); }
  uint32_t unknown_784_address() const { return address + 784; }
  uint32_t unknown_788() const { return guest::LoadU32(base, address + 788); }
  void set_unknown_788(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t unknown_788_address() const { return address + 788; }
  uint32_t unknown_792() const { return guest::LoadU32(base, address + 792); }
  void set_unknown_792(uint32_t v) const { guest::StoreU32(base, address + 792, v); }
  uint32_t unknown_792_address() const { return address + 792; }
  uint32_t unknown_796() const { return guest::LoadU32(base, address + 796); }
  void set_unknown_796(uint32_t v) const { guest::StoreU32(base, address + 796, v); }
  uint32_t unknown_796_address() const { return address + 796; }
  uint32_t unknown_800() const { return guest::LoadU32(base, address + 800); }
  void set_unknown_800(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t unknown_800_address() const { return address + 800; }
  uint32_t unknown_804() const { return guest::LoadU32(base, address + 804); }
  void set_unknown_804(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t unknown_804_address() const { return address + 804; }
  uint32_t unknown_808() const { return guest::LoadU32(base, address + 808); }
  void set_unknown_808(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t unknown_808_address() const { return address + 808; }
  uint32_t unknown_812() const { return guest::LoadU32(base, address + 812); }
  void set_unknown_812(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t unknown_812_address() const { return address + 812; }
  uint32_t unknown_816() const { return guest::LoadU32(base, address + 816); }
  void set_unknown_816(uint32_t v) const { guest::StoreU32(base, address + 816, v); }
  uint32_t unknown_816_address() const { return address + 816; }
  uint32_t unknown_820() const { return guest::LoadU32(base, address + 820); }
  void set_unknown_820(uint32_t v) const { guest::StoreU32(base, address + 820, v); }
  uint32_t unknown_820_address() const { return address + 820; }
  uint32_t unknown_824() const { return guest::LoadU32(base, address + 824); }
  void set_unknown_824(uint32_t v) const { guest::StoreU32(base, address + 824, v); }
  uint32_t unknown_824_address() const { return address + 824; }
  uint32_t unknown_828() const { return guest::LoadU32(base, address + 828); }
  void set_unknown_828(uint32_t v) const { guest::StoreU32(base, address + 828, v); }
  uint32_t unknown_828_address() const { return address + 828; }
  uint32_t unknown_884() const { return guest::LoadU32(base, address + 884); }
  void set_unknown_884(uint32_t v) const { guest::StoreU32(base, address + 884, v); }
  uint32_t unknown_884_address() const { return address + 884; }
  uint32_t unknown_888() const { return guest::LoadU32(base, address + 888); }
  void set_unknown_888(uint32_t v) const { guest::StoreU32(base, address + 888, v); }
  uint32_t unknown_888_address() const { return address + 888; }
  uint32_t unknown_892() const { return guest::LoadU32(base, address + 892); }
  void set_unknown_892(uint32_t v) const { guest::StoreU32(base, address + 892, v); }
  uint32_t unknown_892_address() const { return address + 892; }
  uint32_t unknown_896() const { return guest::LoadU32(base, address + 896); }
  void set_unknown_896(uint32_t v) const { guest::StoreU32(base, address + 896, v); }
  uint32_t unknown_896_address() const { return address + 896; }
  uint32_t unknown_900() const { return guest::LoadU32(base, address + 900); }
  void set_unknown_900(uint32_t v) const { guest::StoreU32(base, address + 900, v); }
  uint32_t unknown_900_address() const { return address + 900; }
  uint32_t unknown_904() const { return guest::LoadU32(base, address + 904); }
  void set_unknown_904(uint32_t v) const { guest::StoreU32(base, address + 904, v); }
  uint32_t unknown_904_address() const { return address + 904; }
  // 16 engine stages of 204 bytes (indexed by the car's +1656)
  uint32_t engine_stages_address() const { return address + 980; }
  // 21 tires of 92 bytes (CarSpec::GetTire)
  uint32_t front_tires_address() const { return address + 4244; }
  // 21 tires of 92 bytes
  uint32_t rear_tires_address() const { return address + 6176; }
  // 13 20-byte paint slots from +8, the count at +268
  uint32_t paints_address() const { return address + 8108; }
  // a wheel record (car-specs.toml's CarWheelRecord)
  uint32_t stock_wheel_address() const { return address + 8380; }
};

// embedded; measure: GarageScreen::SetScrollArrows.ret
struct GarageScreenArrowNode : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  uint32_t visible() const { return guest::LoadU32(base, address + 80); }
  void set_visible(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t visible_address() const { return address + 80; }
};

// The image words docs/functions/garage-screen.md's code uses.
struct GarageScreenGlobals {
  // pages 0 the car list, 1 the car's five actions, 2 the view on the stand, 3 the view's options, 4 leaving; 18 transitions
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8420u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8420u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8420u, v); }
  static constexpr uint32_t pages_address() { return 0x826D8420u; }
  // a MenuList (72 bytes): the car list (page 0), the actions (page 1) or the view's options (page 3)
  template <typename T = MenuList>
  static T list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8424u)); }
  static void set_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8424u, v.address); }
  static void set_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8424u, v); }
  static constexpr uint32_t list_address() { return 0x826D8424u; }
  // a MenuList (72 bytes): the car list beside the actions; Destroy doesn't free it
  template <typename T = MenuList>
  static T list2(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8428u)); }
  static void set_list2(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8428u, v.address); }
  static void set_list2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8428u, v); }
  static constexpr uint32_t list2_address() { return 0x826D8428u; }
  template <typename T = GarageScreenData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D842Cu)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D842Cu, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D842Cu, v); }
  static constexpr uint32_t data_address() { return 0x826D842Cu; }
  // the stand scene's holder (StandScene::Create): +0 the garage stage's holder, whose +0 is the stage
  template <typename T = StandHolder>
  static T stand_holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83E0u, v.address); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // the license plate editor's data (76 bytes, type2-screens.md)
  template <typename T = PlateEditorData>
  static T plate_editor(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8438u)); }
  static void set_plate_editor(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8438u, v.address); }
  static void set_plate_editor(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8438u, v); }
  static constexpr uint32_t plate_editor_address() { return 0x826D8438u; }
  // the power graph screen's data (1,968 bytes, type2-screens.md)
  template <typename T = SpecSheetData>
  static T spec_sheet(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D846Cu)); }
  static void set_spec_sheet(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D846Cu, v.address); }
  static void set_spec_sheet(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D846Cu, v); }
  static constexpr uint32_t spec_sheet_address() { return 0x826D846Cu; }
  // Start's ScreenFades::FadeIn r3
  static uint32_t fade_in_argument(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_argument(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_argument_address() { return 0x82001BECu; }
  // Transition3's ScreenFades::FadeOut r3
  static uint32_t fade_out_argument(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_argument(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_argument_address() { return 0x82001BF0u; }
  // a screen fade's count (Transition3 waits for both at 0)
  static uint32_t fade_count_a(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_count_a(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_count_a_address() { return 0x826A50DCu; }
  // a screen fade's count
  static uint32_t fade_count_b(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_count_b(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_count_b_address() { return 0x826A50E4u; }
  // 0.0 (CarValue's start)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // around CarValue's part weights (from 0x8200A260 - 64)
  static float value_weights(uint8_t* base) { return guest::LoadFloat(base, 0x8200A260u); }
  static void set_value_weights(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200A260u, v); }
  static constexpr uint32_t value_weights_address() { return 0x8200A260u; }
  // SalePrice's: +0 the price's factor, +52 nine mileage thresholds, +84 their factors
  static float price_constants(uint8_t* base) { return guest::LoadFloat(base, 0x8200A348u); }
  static void set_price_constants(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200A348u, v); }
  static constexpr uint32_t price_constants_address() { return 0x8200A348u; }
  // SalePrice: CarValue's factor
  static float value_factor(uint8_t* base) { return guest::LoadFloat(base, 0x82002208u); }
  static void set_value_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002208u, v); }
  static constexpr uint32_t value_factor_address() { return 0x82002208u; }
  // ShowSpecTexts: the weight's scale
  static double weight_scale(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82009F00u)); }
  static void set_weight_scale(uint8_t* base, double v) { guest::StoreU64(base, 0x82009F00u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t weight_scale_address() { return 0x82009F00u; }
  // ShowSpecTexts: the +20 float's scale
  static double spec_scale(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82009F08u)); }
  static void set_spec_scale(uint8_t* base, double v) { guest::StoreU64(base, 0x82009F08u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t spec_scale_address() { return 0x82009F08u; }
  // the power curve's peak scale (ShowCarSpecs, SetUpSpecSheet)
  static float curve_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82009EC0u); }
  static void set_curve_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009EC0u, v); }
  static constexpr uint32_t curve_scale_address() { return 0x82009EC0u; }
  // ShowCarSpecs, SetUpSpecSheet
  static float power_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_power_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t power_scale_address() { return 0x820021E8u; }
};

}  // namespace schema
