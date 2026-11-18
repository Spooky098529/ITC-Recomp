// The views of docs/functions/car-dealer.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-dealer.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: CarDealer, RaceMenu::StartPage.ret
struct CarDealer : View {
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
  // 1 once movie 4 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForLoad go to state 9 (ScreenMachine::Reenter)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 4 (UiMovies::Delete) and clears `loaded`
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// embedded; measure: CarDealer::Close.r4, CarDealer::OnDialogDone.r4
struct CarDealerMode : View {
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

// embedded; measure: -
struct CarDealerList : View {
  using View::View;
  static constexpr uint32_t kSize = 516;
  // 128 pointers to CarDealerEntry
  uint32_t entries_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 512); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 512, v); }
  uint32_t count_address() const { return address + 512; }
};

// embedded; measure: -
struct CarDealerColors : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // 13 pointers to color records
  uint32_t records_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 52); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t count_address() const { return address + 52; }
};

// allocated; measure: new<1196>@CarDealer::Create
struct CarDealerData : View {
  using View::View;
  static constexpr uint32_t kSize = 1196;
  // 1 once started (FillStatus)
  uint32_t started() const { return guest::LoadU32(base, address + 0); }
  void set_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t started_address() const { return address + 0; }
  // 1 with an empty garage (the first car's purchase)
  uint32_t first_car() const { return guest::LoadU32(base, address + 4); }
  void set_first_car(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_car_address() const { return address + 4; }
  // 1 when leaving after a purchase (Destroy: the machine to state 14)
  uint32_t leaving_bought() const { return guest::LoadU32(base, address + 8); }
  void set_leaving_bought(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t leaving_bought_address() const { return address + 8; }
  // the car detail open (CarDetail::Open 1, message 30 0)
  uint32_t detail_open() const { return guest::LoadU32(base, address + 12); }
  void set_detail_open(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t detail_open_address() const { return address + 12; }
  // a dialog's answer (messages 38 1, 39 and 41 0)
  uint32_t answer() const { return guest::LoadU32(base, address + 16); }
  void set_answer(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t answer_address() const { return address + 16; }
  // the stand's busy marker shown
  uint32_t busy_shown() const { return guest::LoadU32(base, address + 20); }
  void set_busy_shown(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t busy_shown_address() const { return address + 20; }
  // the second answer: the bought car made the selected one
  uint32_t select_bought() const { return guest::LoadU32(base, address + 24); }
  void set_select_bought(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t select_bought_address() const { return address + 24; }
  // a car shown on the stand
  uint32_t car_shown() const { return guest::LoadU32(base, address + 28); }
  void set_car_shown(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t car_shown_address() const { return address + 28; }
  // GarageStage::ReloadModel's answer
  uint32_t stand_model() const { return guest::LoadU32(base, address + 32); }
  void set_stand_model(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t stand_model_address() const { return address + 32; }
  // the buy dialogs' step, 0 to 3 (CarDetail::Open 2)
  uint32_t buy_step() const { return guest::LoadU32(base, address + 36); }
  void set_buy_step(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t buy_step_address() const { return address + 36; }
  // the list menu's selection kept (which list)
  uint32_t kind_selection() const { return guest::LoadU32(base, address + 40); }
  void set_kind_selection(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t kind_selection_address() const { return address + 40; }
  // the list shown, 0 or 1
  uint32_t list_shown() const { return guest::LoadU32(base, address + 44); }
  void set_list_shown(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t list_shown_address() const { return address + 44; }
  // the car list's selection (+ car_scroll: the index into shown_list)
  uint32_t car_selection() const { return guest::LoadU32(base, address + 48); }
  void set_car_selection(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t car_selection_address() const { return address + 48; }
  uint32_t car_scroll() const { return guest::LoadU32(base, address + 52); }
  void set_car_scroll(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t car_scroll_address() const { return address + 52; }
  uint32_t color_selection() const { return guest::LoadU32(base, address + 56); }
  void set_color_selection(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t color_selection_address() const { return address + 56; }
  uint32_t color_scroll() const { return guest::LoadU32(base, address + 60); }
  void set_color_scroll(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t color_scroll_address() const { return address + 60; }
  // the career's pick (save data +8, 20 bytes)
  uint32_t pick_address() const { return address + 64; }
  // save data +6116
  uint32_t money() const { return guest::LoadU32(base, address + 84); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t money_address() const { return address + 84; }
  // the day % 7
  uint32_t weekday() const { return guest::LoadU32(base, address + 88); }
  void set_weekday(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t weekday_address() const { return address + 88; }
  // save data +6100, at most 999
  uint32_t day() const { return guest::LoadU32(base, address + 92); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t day_address() const { return address + 92; }
  // save data +6328
  uint32_t garage_count() const { return guest::LoadU32(base, address + 96); }
  void set_garage_count(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t garage_count_address() const { return address + 96; }
  // save data +0x1991C (probably the garage's size)
  uint32_t garage_size() const { return guest::LoadU32(base, address + 100); }
  void set_garage_size(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t garage_size_address() const { return address + 100; }
  // a CarDealerList (128 entries, count at +616)
  uint32_t first_list_address() const { return address + 104; }
  // a CarDealerList (count at +1132)
  uint32_t second_list_address() const { return address + 620; }
  // first_list or second_list
  template <typename T = CarDealerList>
  T shown_list() const { return T(base, guest::LoadU32(base, address + 1136)); }
  void set_shown_list(const View& v) const { guest::StoreU32(base, address + 1136, v.address); }
  void set_shown_list(uint32_t v) const { guest::StoreU32(base, address + 1136, v); }
  uint32_t shown_list_address() const { return address + 1136; }
  // a CarDealerColors (13 records, count at +1192)
  uint32_t colors_address() const { return address + 1140; }
};

// allocated; measure: new<8448>@CarDealer::BuildLists
struct CarDealerEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8448;
  // shown by SetSpecTexts; CheckLimits: at most 2000
  uint32_t unknown_648() const { return guest::LoadU32(base, address + 648); }
  void set_unknown_648(uint32_t v) const { guest::StoreU32(base, address + 648, v); }
  uint32_t unknown_648_address() const { return address + 648; }
  // shown by SetSpecTexts (truncated)
  float unknown_652() const { return guest::LoadFloat(base, address + 652); }
  void set_unknown_652(float v) const { guest::StoreFloat(base, address + 652, v); }
  uint32_t unknown_652_address() const { return address + 652; }
  // shown by SetSpecTexts (a float)
  uint32_t unknown_660() const { return guest::LoadU32(base, address + 660); }
  void set_unknown_660(uint32_t v) const { guest::StoreU32(base, address + 660, v); }
  uint32_t unknown_660_address() const { return address + 660; }
  // shown by SetSpecTexts (nonzero or not)
  uint8_t unknown_709() const { return guest::LoadU8(base, address + 709); }
  void set_unknown_709(uint8_t v) const { guest::StoreU8(base, address + 709, v); }
  uint32_t unknown_709_address() const { return address + 709; }
  // shown by SetSpecTexts (scaled by the double at 0x82009F00)
  uint32_t weight() const { return guest::LoadU32(base, address + 712); }
  void set_weight(uint32_t v) const { guest::StoreU32(base, address + 712, v); }
  uint32_t weight_address() const { return address + 712; }
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
  uint32_t price() const { return guest::LoadU32(base, address + 744); }
  void set_price(uint32_t v) const { guest::StoreU32(base, address + 744, v); }
  uint32_t price_address() const { return address + 744; }
  // string 1934 + it (the maker's name)
  uint8_t maker() const { return guest::LoadU8(base, address + 769); }
  void set_maker(uint8_t v) const { guest::StoreU8(base, address + 769, v); }
  uint32_t maker_address() const { return address + 769; }
  // shown by SetSpecTexts (string 421 + it)
  uint8_t unknown_770() const { return guest::LoadU8(base, address + 770); }
  void set_unknown_770(uint8_t v) const { guest::StoreU8(base, address + 770, v); }
  uint32_t unknown_770_address() const { return address + 770; }
  // 0 the first list, 1 the second, others not sold
  uint8_t list_kind() const { return guest::LoadU8(base, address + 771); }
  void set_list_kind(uint8_t v) const { guest::StoreU8(base, address + 771, v); }
  uint32_t list_kind_address() const { return address + 771; }
  // 13 CarDealerSpecColor records of 20 bytes
  uint32_t color_records_address() const { return address + 8116; }
  uint32_t color_count() const { return guest::LoadU32(base, address + 8376); }
  void set_color_count(uint32_t v) const { guest::StoreU32(base, address + 8376, v); }
  uint32_t color_count_address() const { return address + 8376; }
  uint32_t car_id() const { return guest::LoadU32(base, address + 8432); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 8432, v); }
  uint32_t car_id_address() const { return address + 8432; }
};

// embedded; measure: -
struct CarDealerSpecColor : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 1 picks the value's offset at 0x82002FF4, else 0x82002FF8
  uint16_t kind() const { return guest::LoadU16(base, address + 0); }
  void set_kind(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  uint16_t hue() const { return guest::LoadU16(base, address + 4); }
  void set_hue(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t hue_address() const { return address + 4; }
  uint8_t saturation() const { return guest::LoadU8(base, address + 6); }
  void set_saturation(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t saturation_address() const { return address + 6; }
  // plus the kind's offset, clamped to 0 to 255
  uint8_t value() const { return guest::LoadU8(base, address + 7); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t value_address() const { return address + 7; }
};

// The image words docs/functions/car-dealer.md's code uses.
struct CarDealerGlobals {
  // the stand scene's holder (StandScene::Create): +0 the garage stage
  static View stand_holder(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // 6 pages (CarDealer::UpdatePage), 22 transitions (CarDealer::Update)
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83E8u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83E8u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E8u, v); }
  static constexpr uint32_t pages_address() { return 0x826D83E8u; }
  // a menu list (72 bytes): the page's menu
  static View list_menu(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83ECu)); }
  static void set_list_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83ECu, v); }
  static constexpr uint32_t list_menu_address() { return 0x826D83ECu; }
  // a menu list (72 bytes): the car list's selection on page 2, the colors' on page 3
  static View color_menu(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83F0u)); }
  static void set_color_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83F0u, v); }
  static constexpr uint32_t color_menu_address() { return 0x826D83F0u; }
  template <typename T = CarDealerData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83F4u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83F4u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83F4u, v); }
  static constexpr uint32_t data_address() { return 0x826D83F4u; }
};

}  // namespace schema
