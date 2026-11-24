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
#pragma pack(push, 1)
struct CarDealerModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(CarDealerModeData) == 812);
inline constexpr memory::LayoutField kCarDealerModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kCarDealerModeLayout = {kCarDealerModeFields, 2};

struct CarDealerMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  CarDealerModeData* data() const { return static_cast<CarDealerModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static CarDealerMode New(uint8_t* base) {
    return CarDealerMode(base, memory::MakeHandle(new CarDealerModeData(), sizeof(CarDealerModeData), "CarDealerMode", &kCarDealerModeLayout));
  }
  template <uint32_t Stride = kSize>
  static CarDealerMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarDealerModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarDealerModeData*>(::operator new(sizeof(CarDealerModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarDealerModeData();
    return CarDealerMode(base, memory::MakeHandle(p, sizeof(CarDealerModeData) * count, "CarDealerMode", &kCarDealerModeLayout, sizeof(CarDealerModeData)));
  }
  CarDealerMode at(uint32_t i) const { return CarDealerMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarDealerModeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarDealerModeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
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

// embedded; measure: new<1196>@CarDealer::Create.+1136
#pragma pack(push, 1)
struct CarDealerListData {
  uint8_t entries[512] = {};  // +0, guest order
  uint32_t count = 0;  // +512
};
#pragma pack(pop)
static_assert(sizeof(CarDealerListData) == 516);
inline constexpr memory::LayoutField kCarDealerListFields[] = {{512, 4}};
inline constexpr memory::Layout kCarDealerListLayout = {kCarDealerListFields, 1};

struct CarDealerList : View {
  using View::View;
  static constexpr uint32_t kSize = 516;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarDealerListData* data() const { return static_cast<CarDealerListData*>(memory::HandleObject(address)); }
  static CarDealerList New(uint8_t* base) {
    return CarDealerList(base, memory::MakeHandle(new CarDealerListData(), sizeof(CarDealerListData), "CarDealerList", &kCarDealerListLayout));
  }
  template <uint32_t Stride = kSize>
  static CarDealerList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarDealerListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarDealerListData*>(::operator new(sizeof(CarDealerListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarDealerListData();
    return CarDealerList(base, memory::MakeHandle(p, sizeof(CarDealerListData) * count, "CarDealerList", &kCarDealerListLayout, sizeof(CarDealerListData)));
  }
  CarDealerList at(uint32_t i) const { return CarDealerList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarDealerListLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarDealerListLayout, offset, width, v); }
  // 128 pointers to CarDealerEntry
  uint32_t entries_address() const { return address + 0; }
  uint8_t* entries() const { return reinterpret_cast<uint8_t*>(data()) + 0; }
  uint32_t count_address() const { return address + 512; }
  uint32_t count() const { return data()->count; }
  void set_count(uint32_t v) const { data()->count = v; }
  uint32_t count_bits() const { return data()->count; }
  void set_count_bits(uint32_t v) const { data()->count = v; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarDealerColorsData {
  uint8_t records[52] = {};  // +0, guest order
  uint32_t count = 0;  // +52
};
#pragma pack(pop)
static_assert(sizeof(CarDealerColorsData) == 56);
inline constexpr memory::LayoutField kCarDealerColorsFields[] = {{52, 4}};
inline constexpr memory::Layout kCarDealerColorsLayout = {kCarDealerColorsFields, 1};

struct CarDealerColors : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarDealerColorsData* data() const { return static_cast<CarDealerColorsData*>(memory::HandleObject(address)); }
  static CarDealerColors New(uint8_t* base) {
    return CarDealerColors(base, memory::MakeHandle(new CarDealerColorsData(), sizeof(CarDealerColorsData), "CarDealerColors", &kCarDealerColorsLayout));
  }
  template <uint32_t Stride = kSize>
  static CarDealerColors NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarDealerColorsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarDealerColorsData*>(::operator new(sizeof(CarDealerColorsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarDealerColorsData();
    return CarDealerColors(base, memory::MakeHandle(p, sizeof(CarDealerColorsData) * count, "CarDealerColors", &kCarDealerColorsLayout, sizeof(CarDealerColorsData)));
  }
  CarDealerColors at(uint32_t i) const { return CarDealerColors(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarDealerColorsLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarDealerColorsLayout, offset, width, v); }
  // 13 pointers to color records
  uint32_t records_address() const { return address + 0; }
  uint8_t* records() const { return reinterpret_cast<uint8_t*>(data()) + 0; }
  uint32_t count_address() const { return address + 52; }
  uint32_t count() const { return data()->count; }
  void set_count(uint32_t v) const { data()->count = v; }
  uint32_t count_bits() const { return data()->count; }
  void set_count_bits(uint32_t v) const { data()->count = v; }
};

// allocated; measure: new<1196>@CarDealer::Create
#pragma pack(push, 1)
struct CarDealerDataData {
  uint32_t started = 0;  // +0
  uint32_t first_car = 0;  // +4
  uint32_t leaving_bought = 0;  // +8
  uint32_t detail_open = 0;  // +12
  uint32_t answer = 0;  // +16
  uint32_t busy_shown = 0;  // +20
  uint32_t select_bought = 0;  // +24
  uint32_t car_shown = 0;  // +28
  uint32_t stand_model = 0;  // +32
  uint32_t buy_step = 0;  // +36
  uint32_t kind_selection = 0;  // +40
  uint32_t list_shown = 0;  // +44
  uint32_t car_selection = 0;  // +48
  uint32_t car_scroll = 0;  // +52
  uint32_t color_selection = 0;  // +56
  uint32_t color_scroll = 0;  // +60
  uint8_t pick[20] = {};  // +64, guest order
  uint32_t money = 0;  // +84
  uint32_t weekday = 0;  // +88
  uint32_t day = 0;  // +92
  uint32_t garage_count = 0;  // +96
  uint32_t garage_size = 0;  // +100
  uint8_t first_list[516] = {};  // +104, 1 x CarDealerList
  uint8_t second_list[516] = {};  // +620, 1 x CarDealerList
  uint32_t shown_list = 0;  // +1136
  uint8_t colors[56] = {};  // +1140, 1 x CarDealerColors
};
#pragma pack(pop)
static_assert(sizeof(CarDealerDataData) == 1196);
inline constexpr memory::LayoutField kCarDealerDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {616, 4}, {1132, 4}, {1136, 4}, {1192, 4}};
inline constexpr memory::Layout kCarDealerDataLayout = {kCarDealerDataFields, 25};

struct CarDealerData : View {
  using View::View;
  static constexpr uint32_t kSize = 1196;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarDealerDataData* data() const { return static_cast<CarDealerDataData*>(memory::HandleObject(address)); }
  static CarDealerData New(uint8_t* base) {
    return CarDealerData(base, memory::MakeHandle(new CarDealerDataData(), sizeof(CarDealerDataData), "CarDealerData", &kCarDealerDataLayout));
  }
  template <uint32_t Stride = kSize>
  static CarDealerData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarDealerDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarDealerDataData*>(::operator new(sizeof(CarDealerDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarDealerDataData();
    return CarDealerData(base, memory::MakeHandle(p, sizeof(CarDealerDataData) * count, "CarDealerData", &kCarDealerDataLayout, sizeof(CarDealerDataData)));
  }
  CarDealerData at(uint32_t i) const { return CarDealerData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarDealerDataLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarDealerDataLayout, offset, width, v); }
  // 1 once started (FillStatus)
  uint32_t started_address() const { return address + 0; }
  uint32_t started() const { return data()->started; }
  void set_started(uint32_t v) const { data()->started = v; }
  uint32_t started_bits() const { return data()->started; }
  void set_started_bits(uint32_t v) const { data()->started = v; }
  // 1 with an empty garage (the first car's purchase)
  uint32_t first_car_address() const { return address + 4; }
  uint32_t first_car() const { return data()->first_car; }
  void set_first_car(uint32_t v) const { data()->first_car = v; }
  uint32_t first_car_bits() const { return data()->first_car; }
  void set_first_car_bits(uint32_t v) const { data()->first_car = v; }
  // 1 when leaving after a purchase (Destroy: the machine to state 14)
  uint32_t leaving_bought_address() const { return address + 8; }
  uint32_t leaving_bought() const { return data()->leaving_bought; }
  void set_leaving_bought(uint32_t v) const { data()->leaving_bought = v; }
  uint32_t leaving_bought_bits() const { return data()->leaving_bought; }
  void set_leaving_bought_bits(uint32_t v) const { data()->leaving_bought = v; }
  // the car detail open (CarDetail::Open 1, message 30 0)
  uint32_t detail_open_address() const { return address + 12; }
  uint32_t detail_open() const { return data()->detail_open; }
  void set_detail_open(uint32_t v) const { data()->detail_open = v; }
  uint32_t detail_open_bits() const { return data()->detail_open; }
  void set_detail_open_bits(uint32_t v) const { data()->detail_open = v; }
  // a dialog's answer (messages 38 1, 39 and 41 0)
  uint32_t answer_address() const { return address + 16; }
  uint32_t answer() const { return data()->answer; }
  void set_answer(uint32_t v) const { data()->answer = v; }
  uint32_t answer_bits() const { return data()->answer; }
  void set_answer_bits(uint32_t v) const { data()->answer = v; }
  // the stand's busy marker shown
  uint32_t busy_shown_address() const { return address + 20; }
  uint32_t busy_shown() const { return data()->busy_shown; }
  void set_busy_shown(uint32_t v) const { data()->busy_shown = v; }
  uint32_t busy_shown_bits() const { return data()->busy_shown; }
  void set_busy_shown_bits(uint32_t v) const { data()->busy_shown = v; }
  // the second answer: the bought car made the selected one
  uint32_t select_bought_address() const { return address + 24; }
  uint32_t select_bought() const { return data()->select_bought; }
  void set_select_bought(uint32_t v) const { data()->select_bought = v; }
  uint32_t select_bought_bits() const { return data()->select_bought; }
  void set_select_bought_bits(uint32_t v) const { data()->select_bought = v; }
  // a car shown on the stand
  uint32_t car_shown_address() const { return address + 28; }
  uint32_t car_shown() const { return data()->car_shown; }
  void set_car_shown(uint32_t v) const { data()->car_shown = v; }
  uint32_t car_shown_bits() const { return data()->car_shown; }
  void set_car_shown_bits(uint32_t v) const { data()->car_shown = v; }
  // GarageStage::ReloadModel's answer
  uint32_t stand_model_address() const { return address + 32; }
  uint32_t stand_model() const { return data()->stand_model; }
  void set_stand_model(uint32_t v) const { data()->stand_model = v; }
  uint32_t stand_model_bits() const { return data()->stand_model; }
  void set_stand_model_bits(uint32_t v) const { data()->stand_model = v; }
  // the buy dialogs' step, 0 to 3 (CarDetail::Open 2)
  uint32_t buy_step_address() const { return address + 36; }
  uint32_t buy_step() const { return data()->buy_step; }
  void set_buy_step(uint32_t v) const { data()->buy_step = v; }
  uint32_t buy_step_bits() const { return data()->buy_step; }
  void set_buy_step_bits(uint32_t v) const { data()->buy_step = v; }
  // the list menu's selection kept (which list)
  uint32_t kind_selection_address() const { return address + 40; }
  uint32_t kind_selection() const { return data()->kind_selection; }
  void set_kind_selection(uint32_t v) const { data()->kind_selection = v; }
  uint32_t kind_selection_bits() const { return data()->kind_selection; }
  void set_kind_selection_bits(uint32_t v) const { data()->kind_selection = v; }
  // the list shown, 0 or 1
  uint32_t list_shown_address() const { return address + 44; }
  uint32_t list_shown() const { return data()->list_shown; }
  void set_list_shown(uint32_t v) const { data()->list_shown = v; }
  uint32_t list_shown_bits() const { return data()->list_shown; }
  void set_list_shown_bits(uint32_t v) const { data()->list_shown = v; }
  // the car list's selection (+ car_scroll: the index into shown_list)
  uint32_t car_selection_address() const { return address + 48; }
  uint32_t car_selection() const { return data()->car_selection; }
  void set_car_selection(uint32_t v) const { data()->car_selection = v; }
  uint32_t car_selection_bits() const { return data()->car_selection; }
  void set_car_selection_bits(uint32_t v) const { data()->car_selection = v; }
  uint32_t car_scroll_address() const { return address + 52; }
  uint32_t car_scroll() const { return data()->car_scroll; }
  void set_car_scroll(uint32_t v) const { data()->car_scroll = v; }
  uint32_t car_scroll_bits() const { return data()->car_scroll; }
  void set_car_scroll_bits(uint32_t v) const { data()->car_scroll = v; }
  uint32_t color_selection_address() const { return address + 56; }
  uint32_t color_selection() const { return data()->color_selection; }
  void set_color_selection(uint32_t v) const { data()->color_selection = v; }
  uint32_t color_selection_bits() const { return data()->color_selection; }
  void set_color_selection_bits(uint32_t v) const { data()->color_selection = v; }
  uint32_t color_scroll_address() const { return address + 60; }
  uint32_t color_scroll() const { return data()->color_scroll; }
  void set_color_scroll(uint32_t v) const { data()->color_scroll = v; }
  uint32_t color_scroll_bits() const { return data()->color_scroll; }
  void set_color_scroll_bits(uint32_t v) const { data()->color_scroll = v; }
  // the career's pick (save data +8, 20 bytes)
  uint32_t pick_address() const { return address + 64; }
  uint8_t* pick() const { return reinterpret_cast<uint8_t*>(data()) + 64; }
  // save data +6116
  uint32_t money_address() const { return address + 84; }
  uint32_t money() const { return data()->money; }
  void set_money(uint32_t v) const { data()->money = v; }
  uint32_t money_bits() const { return data()->money; }
  void set_money_bits(uint32_t v) const { data()->money = v; }
  // the day % 7
  uint32_t weekday_address() const { return address + 88; }
  uint32_t weekday() const { return data()->weekday; }
  void set_weekday(uint32_t v) const { data()->weekday = v; }
  uint32_t weekday_bits() const { return data()->weekday; }
  void set_weekday_bits(uint32_t v) const { data()->weekday = v; }
  // save data +6100, at most 999
  uint32_t day_address() const { return address + 92; }
  uint32_t day() const { return data()->day; }
  void set_day(uint32_t v) const { data()->day = v; }
  uint32_t day_bits() const { return data()->day; }
  void set_day_bits(uint32_t v) const { data()->day = v; }
  // save data +6328
  uint32_t garage_count_address() const { return address + 96; }
  uint32_t garage_count() const { return data()->garage_count; }
  void set_garage_count(uint32_t v) const { data()->garage_count = v; }
  uint32_t garage_count_bits() const { return data()->garage_count; }
  void set_garage_count_bits(uint32_t v) const { data()->garage_count = v; }
  // save data +0x1991C (probably the garage's size)
  uint32_t garage_size_address() const { return address + 100; }
  uint32_t garage_size() const { return data()->garage_size; }
  void set_garage_size(uint32_t v) const { data()->garage_size = v; }
  uint32_t garage_size_bits() const { return data()->garage_size; }
  void set_garage_size_bits(uint32_t v) const { data()->garage_size = v; }
  // 128 entries, count at +616
  template <typename T = CarDealerList>
  T first_list() const { return T(base, address + 104); }
  // count at +1132
  template <typename T = CarDealerList>
  T second_list() const { return T(base, address + 620); }
  // first_list or second_list (an interior handle)
  uint32_t shown_list_address() const { return address + 1136; }
  template <typename T = CarDealerList>
  T shown_list() const { return T(base, data()->shown_list); }
  void set_shown_list(const View& w) const { const uint32_t v = w.address; data()->shown_list = v; }
  void set_shown_list(uint32_t v) const { data()->shown_list = v; }
  uint32_t shown_list_bits() const { return data()->shown_list; }
  void set_shown_list_bits(uint32_t v) const { data()->shown_list = v; }
  // 13 records, count at +1192
  template <typename T = CarDealerColors>
  T colors() const { return T(base, address + 1140); }
};

// allocated; measure: new<8448>@CarDealer::BuildLists
#pragma pack(push, 1)
struct CarDealerEntryData {
  uint8_t pad_0[648] = {};
  uint32_t unknown_648 = 0;  // +648
  float unknown_652 = 0;  // +652
  uint8_t pad_656[4] = {};
  uint32_t unknown_660 = 0;  // +660
  uint8_t pad_664[45] = {};
  uint8_t unknown_709 = 0;  // +709
  uint8_t pad_710[2] = {};
  uint32_t weight = 0;  // +712
  uint32_t unknown_716 = 0;  // +716
  uint32_t unknown_720 = 0;  // +720
  uint32_t unknown_724 = 0;  // +724
  uint8_t pad_728[16] = {};
  uint32_t price = 0;  // +744
  uint8_t pad_748[21] = {};
  uint8_t maker = 0;  // +769
  uint8_t unknown_770 = 0;  // +770
  uint8_t list_kind = 0;  // +771
  uint8_t pad_772[7344] = {};
  uint8_t color_records[260] = {};  // +8116, 13 x CarDealerSpecColor
  uint32_t color_count = 0;  // +8376
  uint8_t pad_8380[52] = {};
  uint32_t car_id = 0;  // +8432
  uint8_t pad_8436[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarDealerEntryData) == 8448);
inline constexpr memory::LayoutField kCarDealerEntryFields[] = {{648, 4}, {652, 4}, {660, 4}, {712, 4}, {716, 4}, {720, 4}, {724, 4}, {744, 4}, {8116, 2}, {8120, 2}, {8136, 2}, {8140, 2}, {8156, 2}, {8160, 2}, {8176, 2}, {8180, 2}, {8196, 2}, {8200, 2}, {8216, 2}, {8220, 2}, {8236, 2}, {8240, 2}, {8256, 2}, {8260, 2}, {8276, 2}, {8280, 2}, {8296, 2}, {8300, 2}, {8316, 2}, {8320, 2}, {8336, 2}, {8340, 2}, {8356, 2}, {8360, 2}, {8376, 4}, {8432, 4}};
inline constexpr memory::Layout kCarDealerEntryLayout = {kCarDealerEntryFields, 36};

struct CarDealerEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8448;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarDealerEntryData* data() const { return static_cast<CarDealerEntryData*>(memory::HandleObject(address)); }
  static CarDealerEntry New(uint8_t* base) {
    return CarDealerEntry(base, memory::MakeHandle(new CarDealerEntryData(), sizeof(CarDealerEntryData), "CarDealerEntry", &kCarDealerEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static CarDealerEntry NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarDealerEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarDealerEntryData*>(::operator new(sizeof(CarDealerEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarDealerEntryData();
    return CarDealerEntry(base, memory::MakeHandle(p, sizeof(CarDealerEntryData) * count, "CarDealerEntry", &kCarDealerEntryLayout, sizeof(CarDealerEntryData)));
  }
  CarDealerEntry at(uint32_t i) const { return CarDealerEntry(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarDealerEntryLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarDealerEntryLayout, offset, width, v); }
  // shown by SetSpecTexts; CheckLimits: at most 2000
  uint32_t unknown_648_address() const { return address + 648; }
  uint32_t unknown_648() const { return data()->unknown_648; }
  void set_unknown_648(uint32_t v) const { data()->unknown_648 = v; }
  uint32_t unknown_648_bits() const { return data()->unknown_648; }
  void set_unknown_648_bits(uint32_t v) const { data()->unknown_648 = v; }
  // shown by SetSpecTexts (truncated)
  uint32_t unknown_652_address() const { return address + 652; }
  float unknown_652() const { return data()->unknown_652; }
  void set_unknown_652(float v) const { data()->unknown_652 = v; }
  uint32_t unknown_652_bits() const { return std::bit_cast<uint32_t>(data()->unknown_652); }
  void set_unknown_652_bits(uint32_t v) const { data()->unknown_652 = std::bit_cast<float>(v); }
  // shown by SetSpecTexts (a float)
  uint32_t unknown_660_address() const { return address + 660; }
  uint32_t unknown_660() const { return data()->unknown_660; }
  void set_unknown_660(uint32_t v) const { data()->unknown_660 = v; }
  uint32_t unknown_660_bits() const { return data()->unknown_660; }
  void set_unknown_660_bits(uint32_t v) const { data()->unknown_660 = v; }
  // shown by SetSpecTexts (nonzero or not)
  uint32_t unknown_709_address() const { return address + 709; }
  uint8_t unknown_709() const { return data()->unknown_709; }
  void set_unknown_709(uint8_t v) const { data()->unknown_709 = v; }
  uint8_t unknown_709_bits() const { return data()->unknown_709; }
  void set_unknown_709_bits(uint8_t v) const { data()->unknown_709 = v; }
  // shown by SetSpecTexts (scaled by the double at 0x82009F00)
  uint32_t weight_address() const { return address + 712; }
  uint32_t weight() const { return data()->weight; }
  void set_weight(uint32_t v) const { data()->weight = v; }
  uint32_t weight_bits() const { return data()->weight; }
  void set_weight_bits(uint32_t v) const { data()->weight = v; }
  // CheckLimits: at most 4700
  uint32_t unknown_716_address() const { return address + 716; }
  uint32_t unknown_716() const { return data()->unknown_716; }
  void set_unknown_716(uint32_t v) const { data()->unknown_716 = v; }
  uint32_t unknown_716_bits() const { return data()->unknown_716; }
  void set_unknown_716_bits(uint32_t v) const { data()->unknown_716 = v; }
  // CheckLimits: at most 2000
  uint32_t unknown_720_address() const { return address + 720; }
  uint32_t unknown_720() const { return data()->unknown_720; }
  void set_unknown_720(uint32_t v) const { data()->unknown_720 = v; }
  uint32_t unknown_720_bits() const { return data()->unknown_720; }
  void set_unknown_720_bits(uint32_t v) const { data()->unknown_720 = v; }
  // CheckLimits: at most 1700
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_724() const { return data()->unknown_724; }
  void set_unknown_724(uint32_t v) const { data()->unknown_724 = v; }
  uint32_t unknown_724_bits() const { return data()->unknown_724; }
  void set_unknown_724_bits(uint32_t v) const { data()->unknown_724 = v; }
  uint32_t price_address() const { return address + 744; }
  uint32_t price() const { return data()->price; }
  void set_price(uint32_t v) const { data()->price = v; }
  uint32_t price_bits() const { return data()->price; }
  void set_price_bits(uint32_t v) const { data()->price = v; }
  // string 1934 + it (the maker's name)
  uint32_t maker_address() const { return address + 769; }
  uint8_t maker() const { return data()->maker; }
  void set_maker(uint8_t v) const { data()->maker = v; }
  uint8_t maker_bits() const { return data()->maker; }
  void set_maker_bits(uint8_t v) const { data()->maker = v; }
  // shown by SetSpecTexts (string 421 + it)
  uint32_t unknown_770_address() const { return address + 770; }
  uint8_t unknown_770() const { return data()->unknown_770; }
  void set_unknown_770(uint8_t v) const { data()->unknown_770 = v; }
  uint8_t unknown_770_bits() const { return data()->unknown_770; }
  void set_unknown_770_bits(uint8_t v) const { data()->unknown_770 = v; }
  // 0 the first list, 1 the second, others not sold
  uint32_t list_kind_address() const { return address + 771; }
  uint8_t list_kind() const { return data()->list_kind; }
  void set_list_kind(uint8_t v) const { data()->list_kind = v; }
  uint8_t list_kind_bits() const { return data()->list_kind; }
  void set_list_kind_bits(uint8_t v) const { data()->list_kind = v; }
  template <typename T = CarDealerSpecColor>
  T color_records(uint32_t i) const { return T(base, address + 8116 + i * T::kSize); }
  uint32_t color_count_address() const { return address + 8376; }
  uint32_t color_count() const { return data()->color_count; }
  void set_color_count(uint32_t v) const { data()->color_count = v; }
  uint32_t color_count_bits() const { return data()->color_count; }
  void set_color_count_bits(uint32_t v) const { data()->color_count = v; }
  uint32_t car_id_address() const { return address + 8432; }
  uint32_t car_id() const { return data()->car_id; }
  void set_car_id(uint32_t v) const { data()->car_id = v; }
  uint32_t car_id_bits() const { return data()->car_id; }
  void set_car_id_bits(uint32_t v) const { data()->car_id = v; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarDealerSpecColorData {
  uint16_t kind = 0;  // +0
  uint8_t pad_2[2] = {};
  uint16_t hue = 0;  // +4
  uint8_t saturation = 0;  // +6
  uint8_t value = 0;  // +7
  uint8_t pad_8[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarDealerSpecColorData) == 20);
inline constexpr memory::LayoutField kCarDealerSpecColorFields[] = {{0, 2}, {4, 2}};
inline constexpr memory::Layout kCarDealerSpecColorLayout = {kCarDealerSpecColorFields, 2};

struct CarDealerSpecColor : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarDealerSpecColorData* data() const { return static_cast<CarDealerSpecColorData*>(memory::HandleObject(address)); }
  static CarDealerSpecColor New(uint8_t* base) {
    return CarDealerSpecColor(base, memory::MakeHandle(new CarDealerSpecColorData(), sizeof(CarDealerSpecColorData), "CarDealerSpecColor", &kCarDealerSpecColorLayout));
  }
  template <uint32_t Stride = kSize>
  static CarDealerSpecColor NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(CarDealerSpecColorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarDealerSpecColorData*>(::operator new(sizeof(CarDealerSpecColorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarDealerSpecColorData();
    return CarDealerSpecColor(base, memory::MakeHandle(p, sizeof(CarDealerSpecColorData) * count, "CarDealerSpecColor", &kCarDealerSpecColorLayout, sizeof(CarDealerSpecColorData)));
  }
  CarDealerSpecColor at(uint32_t i) const { return CarDealerSpecColor(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kCarDealerSpecColorLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kCarDealerSpecColorLayout, offset, width, v); }
  // 1 picks the value's offset at 0x82002FF4, else 0x82002FF8
  uint32_t kind_address() const { return address + 0; }
  uint16_t kind() const { return data()->kind; }
  void set_kind(uint16_t v) const { data()->kind = v; }
  uint16_t kind_bits() const { return data()->kind; }
  void set_kind_bits(uint16_t v) const { data()->kind = v; }
  uint32_t hue_address() const { return address + 4; }
  uint16_t hue() const { return data()->hue; }
  void set_hue(uint16_t v) const { data()->hue = v; }
  uint16_t hue_bits() const { return data()->hue; }
  void set_hue_bits(uint16_t v) const { data()->hue = v; }
  uint32_t saturation_address() const { return address + 6; }
  uint8_t saturation() const { return data()->saturation; }
  void set_saturation(uint8_t v) const { data()->saturation = v; }
  uint8_t saturation_bits() const { return data()->saturation; }
  void set_saturation_bits(uint8_t v) const { data()->saturation = v; }
  // plus the kind's offset, clamped to 0 to 255
  uint32_t value_address() const { return address + 7; }
  uint8_t value() const { return data()->value; }
  void set_value(uint8_t v) const { data()->value = v; }
  uint8_t value_bits() const { return data()->value; }
  void set_value_bits(uint8_t v) const { data()->value = v; }
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
  template <typename T = MenuList>
  static T list_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83ECu)); }
  static void set_list_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83ECu, v.address); }
  static void set_list_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83ECu, v); }
  static constexpr uint32_t list_menu_address() { return 0x826D83ECu; }
  // a menu list (72 bytes): the car list's selection on page 2, the colors' on page 3
  template <typename T = MenuList>
  static T color_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83F0u)); }
  static void set_color_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83F0u, v.address); }
  static void set_color_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83F0u, v); }
  static constexpr uint32_t color_menu_address() { return 0x826D83F0u; }
  template <typename T = CarDealerData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D83F4u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D83F4u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83F4u, v); }
  static constexpr uint32_t data_address() { return 0x826D83F4u; }
};

}  // namespace schema
