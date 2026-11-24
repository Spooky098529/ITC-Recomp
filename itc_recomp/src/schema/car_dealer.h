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
  CarDealerModeData* direct() const { return static_cast<CarDealerModeData*>(memory::HandleOwn(address, &kCarDealerModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarDealerModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarDealerModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
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
  CarDealerListData* direct() const { return static_cast<CarDealerListData*>(memory::HandleOwn(address, &kCarDealerListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarDealerListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarDealerListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 128 pointers to CarDealerEntry
  uint32_t entries_address() const { return address + 0; }
  uint8_t* entries() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  uint32_t count_address() const { return address + 512; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 512) ? uint32_t(memory::OwnerLoad(address + 512, 4)) : guest::LoadU32(base, address + 512))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 512)) memory::OwnerStore(address + 512, 4, v); else guest::StoreU32(base, address + 512, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 512) ? uint32_t(memory::OwnerLoad(address + 512, 4)) : guest::LoadU32(base, address + 512))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 512)) memory::OwnerStore(address + 512, 4, v); else guest::StoreU32(base, address + 512, v); }
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
  CarDealerColorsData* direct() const { return static_cast<CarDealerColorsData*>(memory::HandleOwn(address, &kCarDealerColorsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarDealerColorsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarDealerColorsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 13 pointers to color records
  uint32_t records_address() const { return address + 0; }
  uint8_t* records() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  uint32_t count_address() const { return address + 52; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
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
  CarDealerDataData* direct() const { return static_cast<CarDealerDataData*>(memory::HandleOwn(address, &kCarDealerDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarDealerDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarDealerDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 1 once started (FillStatus)
  uint32_t started_address() const { return address + 0; }
  uint32_t started_bits() const { if (auto* d = direct()) return d->started; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_started_bits(uint32_t v) const { if (auto* d = direct()) { d->started = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t started() const { if (auto* d = direct()) return d->started; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_started(uint32_t v) const { if (auto* d = direct()) { d->started = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // 1 with an empty garage (the first car's purchase)
  uint32_t first_car_address() const { return address + 4; }
  uint32_t first_car_bits() const { if (auto* d = direct()) return d->first_car; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_first_car_bits(uint32_t v) const { if (auto* d = direct()) { d->first_car = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t first_car() const { if (auto* d = direct()) return d->first_car; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_first_car(uint32_t v) const { if (auto* d = direct()) { d->first_car = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // 1 when leaving after a purchase (Destroy: the machine to state 14)
  uint32_t leaving_bought_address() const { return address + 8; }
  uint32_t leaving_bought_bits() const { if (auto* d = direct()) return d->leaving_bought; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_leaving_bought_bits(uint32_t v) const { if (auto* d = direct()) { d->leaving_bought = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t leaving_bought() const { if (auto* d = direct()) return d->leaving_bought; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_leaving_bought(uint32_t v) const { if (auto* d = direct()) { d->leaving_bought = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the car detail open (CarDetail::Open 1, message 30 0)
  uint32_t detail_open_address() const { return address + 12; }
  uint32_t detail_open_bits() const { if (auto* d = direct()) return d->detail_open; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_detail_open_bits(uint32_t v) const { if (auto* d = direct()) { d->detail_open = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t detail_open() const { if (auto* d = direct()) return d->detail_open; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_detail_open(uint32_t v) const { if (auto* d = direct()) { d->detail_open = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // a dialog's answer (messages 38 1, 39 and 41 0)
  uint32_t answer_address() const { return address + 16; }
  uint32_t answer_bits() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t answer() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_answer(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // the stand's busy marker shown
  uint32_t busy_shown_address() const { return address + 20; }
  uint32_t busy_shown_bits() const { if (auto* d = direct()) return d->busy_shown; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_busy_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->busy_shown = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t busy_shown() const { if (auto* d = direct()) return d->busy_shown; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_busy_shown(uint32_t v) const { if (auto* d = direct()) { d->busy_shown = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // the second answer: the bought car made the selected one
  uint32_t select_bought_address() const { return address + 24; }
  uint32_t select_bought_bits() const { if (auto* d = direct()) return d->select_bought; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_select_bought_bits(uint32_t v) const { if (auto* d = direct()) { d->select_bought = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t select_bought() const { if (auto* d = direct()) return d->select_bought; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_select_bought(uint32_t v) const { if (auto* d = direct()) { d->select_bought = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // a car shown on the stand
  uint32_t car_shown_address() const { return address + 28; }
  uint32_t car_shown_bits() const { if (auto* d = direct()) return d->car_shown; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_car_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->car_shown = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t car_shown() const { if (auto* d = direct()) return d->car_shown; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_car_shown(uint32_t v) const { if (auto* d = direct()) { d->car_shown = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // GarageStage::ReloadModel's answer
  uint32_t stand_model_address() const { return address + 32; }
  uint32_t stand_model_bits() const { if (auto* d = direct()) return d->stand_model; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_stand_model_bits(uint32_t v) const { if (auto* d = direct()) { d->stand_model = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t stand_model() const { if (auto* d = direct()) return d->stand_model; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_stand_model(uint32_t v) const { if (auto* d = direct()) { d->stand_model = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the buy dialogs' step, 0 to 3 (CarDetail::Open 2)
  uint32_t buy_step_address() const { return address + 36; }
  uint32_t buy_step_bits() const { if (auto* d = direct()) return d->buy_step; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_buy_step_bits(uint32_t v) const { if (auto* d = direct()) { d->buy_step = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t buy_step() const { if (auto* d = direct()) return d->buy_step; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_buy_step(uint32_t v) const { if (auto* d = direct()) { d->buy_step = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  // the list menu's selection kept (which list)
  uint32_t kind_selection_address() const { return address + 40; }
  uint32_t kind_selection_bits() const { if (auto* d = direct()) return d->kind_selection; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_kind_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->kind_selection = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t kind_selection() const { if (auto* d = direct()) return d->kind_selection; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_kind_selection(uint32_t v) const { if (auto* d = direct()) { d->kind_selection = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  // the list shown, 0 or 1
  uint32_t list_shown_address() const { return address + 44; }
  uint32_t list_shown_bits() const { if (auto* d = direct()) return d->list_shown; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_list_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->list_shown = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  uint32_t list_shown() const { if (auto* d = direct()) return d->list_shown; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_list_shown(uint32_t v) const { if (auto* d = direct()) { d->list_shown = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  // the car list's selection (+ car_scroll: the index into shown_list)
  uint32_t car_selection_address() const { return address + 48; }
  uint32_t car_selection_bits() const { if (auto* d = direct()) return d->car_selection; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_car_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->car_selection = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t car_selection() const { if (auto* d = direct()) return d->car_selection; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_car_selection(uint32_t v) const { if (auto* d = direct()) { d->car_selection = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t car_scroll_address() const { return address + 52; }
  uint32_t car_scroll_bits() const { if (auto* d = direct()) return d->car_scroll; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_car_scroll_bits(uint32_t v) const { if (auto* d = direct()) { d->car_scroll = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t car_scroll() const { if (auto* d = direct()) return d->car_scroll; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_car_scroll(uint32_t v) const { if (auto* d = direct()) { d->car_scroll = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t color_selection_address() const { return address + 56; }
  uint32_t color_selection_bits() const { if (auto* d = direct()) return d->color_selection; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_color_selection_bits(uint32_t v) const { if (auto* d = direct()) { d->color_selection = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  uint32_t color_selection() const { if (auto* d = direct()) return d->color_selection; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_color_selection(uint32_t v) const { if (auto* d = direct()) { d->color_selection = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  uint32_t color_scroll_address() const { return address + 60; }
  uint32_t color_scroll_bits() const { if (auto* d = direct()) return d->color_scroll; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_color_scroll_bits(uint32_t v) const { if (auto* d = direct()) { d->color_scroll = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t color_scroll() const { if (auto* d = direct()) return d->color_scroll; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_color_scroll(uint32_t v) const { if (auto* d = direct()) { d->color_scroll = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  // the career's pick (save data +8, 20 bytes)
  uint32_t pick_address() const { return address + 64; }
  uint8_t* pick() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(base, address + 64); }
  // save data +6116
  uint32_t money_address() const { return address + 84; }
  uint32_t money_bits() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_money_bits(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t money() const { if (auto* d = direct()) return d->money; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_money(uint32_t v) const { if (auto* d = direct()) { d->money = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  // the day % 7
  uint32_t weekday_address() const { return address + 88; }
  uint32_t weekday_bits() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_weekday_bits(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t weekday() const { if (auto* d = direct()) return d->weekday; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_weekday(uint32_t v) const { if (auto* d = direct()) { d->weekday = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  // save data +6100, at most 999
  uint32_t day_address() const { return address + 92; }
  uint32_t day_bits() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_day_bits(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  uint32_t day() const { if (auto* d = direct()) return d->day; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_day(uint32_t v) const { if (auto* d = direct()) { d->day = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  // save data +6328
  uint32_t garage_count_address() const { return address + 96; }
  uint32_t garage_count_bits() const { if (auto* d = direct()) return d->garage_count; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_garage_count_bits(uint32_t v) const { if (auto* d = direct()) { d->garage_count = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  uint32_t garage_count() const { if (auto* d = direct()) return d->garage_count; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_garage_count(uint32_t v) const { if (auto* d = direct()) { d->garage_count = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  // save data +0x1991C (probably the garage's size)
  uint32_t garage_size_address() const { return address + 100; }
  uint32_t garage_size_bits() const { if (auto* d = direct()) return d->garage_size; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_garage_size_bits(uint32_t v) const { if (auto* d = direct()) { d->garage_size = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  uint32_t garage_size() const { if (auto* d = direct()) return d->garage_size; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_garage_size(uint32_t v) const { if (auto* d = direct()) { d->garage_size = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  // 128 entries, count at +616
  template <typename T = CarDealerList>
  T first_list() const { return T(base, address + 104); }
  // count at +1132
  template <typename T = CarDealerList>
  T second_list() const { return T(base, address + 620); }
  // first_list or second_list (an interior handle)
  uint32_t shown_list_address() const { return address + 1136; }
  uint32_t shown_list_bits() const { if (auto* d = direct()) return d->shown_list; return uint32_t((memory::IsHandle(address + 1136) ? uint32_t(memory::OwnerLoad(address + 1136, 4)) : guest::LoadU32(base, address + 1136))); }
  void set_shown_list_bits(uint32_t v) const { if (auto* d = direct()) { d->shown_list = v; return; } if (memory::IsHandle(address + 1136)) memory::OwnerStore(address + 1136, 4, v); else guest::StoreU32(base, address + 1136, v); }
  template <typename T = CarDealerList>
  T shown_list() const { return T(base, shown_list_bits()); }
  void set_shown_list(const View& w) const { set_shown_list_bits(w.address); }
  void set_shown_list(uint32_t v) const { set_shown_list_bits(v); }
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
  CarDealerEntryData* direct() const { return static_cast<CarDealerEntryData*>(memory::HandleOwn(address, &kCarDealerEntryLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarDealerEntryLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarDealerEntryLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // shown by SetSpecTexts; CheckLimits: at most 2000
  uint32_t unknown_648_address() const { return address + 648; }
  uint32_t unknown_648_bits() const { if (auto* d = direct()) return d->unknown_648; return uint32_t((memory::IsHandle(address + 648) ? uint32_t(memory::OwnerLoad(address + 648, 4)) : guest::LoadU32(base, address + 648))); }
  void set_unknown_648_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_648 = v; return; } if (memory::IsHandle(address + 648)) memory::OwnerStore(address + 648, 4, v); else guest::StoreU32(base, address + 648, v); }
  uint32_t unknown_648() const { if (auto* d = direct()) return d->unknown_648; return uint32_t((memory::IsHandle(address + 648) ? uint32_t(memory::OwnerLoad(address + 648, 4)) : guest::LoadU32(base, address + 648))); }
  void set_unknown_648(uint32_t v) const { if (auto* d = direct()) { d->unknown_648 = v; return; } if (memory::IsHandle(address + 648)) memory::OwnerStore(address + 648, 4, v); else guest::StoreU32(base, address + 648, v); }
  // shown by SetSpecTexts (truncated)
  uint32_t unknown_652_address() const { return address + 652; }
  uint32_t unknown_652_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_652); return uint32_t((memory::IsHandle(address + 652) ? uint32_t(memory::OwnerLoad(address + 652, 4)) : guest::LoadU32(base, address + 652))); }
  void set_unknown_652_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_652 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 652)) memory::OwnerStore(address + 652, 4, v); else guest::StoreU32(base, address + 652, v); }
  float unknown_652() const { return std::bit_cast<float>(unknown_652_bits()); }
  void set_unknown_652(float v) const { set_unknown_652_bits(std::bit_cast<uint32_t>(v)); }
  // shown by SetSpecTexts (a float)
  uint32_t unknown_660_address() const { return address + 660; }
  uint32_t unknown_660_bits() const { if (auto* d = direct()) return d->unknown_660; return uint32_t((memory::IsHandle(address + 660) ? uint32_t(memory::OwnerLoad(address + 660, 4)) : guest::LoadU32(base, address + 660))); }
  void set_unknown_660_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_660 = v; return; } if (memory::IsHandle(address + 660)) memory::OwnerStore(address + 660, 4, v); else guest::StoreU32(base, address + 660, v); }
  uint32_t unknown_660() const { if (auto* d = direct()) return d->unknown_660; return uint32_t((memory::IsHandle(address + 660) ? uint32_t(memory::OwnerLoad(address + 660, 4)) : guest::LoadU32(base, address + 660))); }
  void set_unknown_660(uint32_t v) const { if (auto* d = direct()) { d->unknown_660 = v; return; } if (memory::IsHandle(address + 660)) memory::OwnerStore(address + 660, 4, v); else guest::StoreU32(base, address + 660, v); }
  // shown by SetSpecTexts (nonzero or not)
  uint32_t unknown_709_address() const { return address + 709; }
  uint8_t unknown_709_bits() const { if (auto* d = direct()) return d->unknown_709; return uint8_t((memory::IsHandle(address + 709) ? uint8_t(memory::OwnerLoad(address + 709, 1)) : guest::LoadU8(base, address + 709))); }
  void set_unknown_709_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_709 = v; return; } if (memory::IsHandle(address + 709)) memory::OwnerStore(address + 709, 1, v); else guest::StoreU8(base, address + 709, v); }
  uint8_t unknown_709() const { if (auto* d = direct()) return d->unknown_709; return uint8_t((memory::IsHandle(address + 709) ? uint8_t(memory::OwnerLoad(address + 709, 1)) : guest::LoadU8(base, address + 709))); }
  void set_unknown_709(uint8_t v) const { if (auto* d = direct()) { d->unknown_709 = v; return; } if (memory::IsHandle(address + 709)) memory::OwnerStore(address + 709, 1, v); else guest::StoreU8(base, address + 709, v); }
  // shown by SetSpecTexts (scaled by the double at 0x82009F00)
  uint32_t weight_address() const { return address + 712; }
  uint32_t weight_bits() const { if (auto* d = direct()) return d->weight; return uint32_t((memory::IsHandle(address + 712) ? uint32_t(memory::OwnerLoad(address + 712, 4)) : guest::LoadU32(base, address + 712))); }
  void set_weight_bits(uint32_t v) const { if (auto* d = direct()) { d->weight = v; return; } if (memory::IsHandle(address + 712)) memory::OwnerStore(address + 712, 4, v); else guest::StoreU32(base, address + 712, v); }
  uint32_t weight() const { if (auto* d = direct()) return d->weight; return uint32_t((memory::IsHandle(address + 712) ? uint32_t(memory::OwnerLoad(address + 712, 4)) : guest::LoadU32(base, address + 712))); }
  void set_weight(uint32_t v) const { if (auto* d = direct()) { d->weight = v; return; } if (memory::IsHandle(address + 712)) memory::OwnerStore(address + 712, 4, v); else guest::StoreU32(base, address + 712, v); }
  // CheckLimits: at most 4700
  uint32_t unknown_716_address() const { return address + 716; }
  uint32_t unknown_716_bits() const { if (auto* d = direct()) return d->unknown_716; return uint32_t((memory::IsHandle(address + 716) ? uint32_t(memory::OwnerLoad(address + 716, 4)) : guest::LoadU32(base, address + 716))); }
  void set_unknown_716_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_716 = v; return; } if (memory::IsHandle(address + 716)) memory::OwnerStore(address + 716, 4, v); else guest::StoreU32(base, address + 716, v); }
  uint32_t unknown_716() const { if (auto* d = direct()) return d->unknown_716; return uint32_t((memory::IsHandle(address + 716) ? uint32_t(memory::OwnerLoad(address + 716, 4)) : guest::LoadU32(base, address + 716))); }
  void set_unknown_716(uint32_t v) const { if (auto* d = direct()) { d->unknown_716 = v; return; } if (memory::IsHandle(address + 716)) memory::OwnerStore(address + 716, 4, v); else guest::StoreU32(base, address + 716, v); }
  // CheckLimits: at most 2000
  uint32_t unknown_720_address() const { return address + 720; }
  uint32_t unknown_720_bits() const { if (auto* d = direct()) return d->unknown_720; return uint32_t((memory::IsHandle(address + 720) ? uint32_t(memory::OwnerLoad(address + 720, 4)) : guest::LoadU32(base, address + 720))); }
  void set_unknown_720_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_720 = v; return; } if (memory::IsHandle(address + 720)) memory::OwnerStore(address + 720, 4, v); else guest::StoreU32(base, address + 720, v); }
  uint32_t unknown_720() const { if (auto* d = direct()) return d->unknown_720; return uint32_t((memory::IsHandle(address + 720) ? uint32_t(memory::OwnerLoad(address + 720, 4)) : guest::LoadU32(base, address + 720))); }
  void set_unknown_720(uint32_t v) const { if (auto* d = direct()) { d->unknown_720 = v; return; } if (memory::IsHandle(address + 720)) memory::OwnerStore(address + 720, 4, v); else guest::StoreU32(base, address + 720, v); }
  // CheckLimits: at most 1700
  uint32_t unknown_724_address() const { return address + 724; }
  uint32_t unknown_724_bits() const { if (auto* d = direct()) return d->unknown_724; return uint32_t((memory::IsHandle(address + 724) ? uint32_t(memory::OwnerLoad(address + 724, 4)) : guest::LoadU32(base, address + 724))); }
  void set_unknown_724_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_724 = v; return; } if (memory::IsHandle(address + 724)) memory::OwnerStore(address + 724, 4, v); else guest::StoreU32(base, address + 724, v); }
  uint32_t unknown_724() const { if (auto* d = direct()) return d->unknown_724; return uint32_t((memory::IsHandle(address + 724) ? uint32_t(memory::OwnerLoad(address + 724, 4)) : guest::LoadU32(base, address + 724))); }
  void set_unknown_724(uint32_t v) const { if (auto* d = direct()) { d->unknown_724 = v; return; } if (memory::IsHandle(address + 724)) memory::OwnerStore(address + 724, 4, v); else guest::StoreU32(base, address + 724, v); }
  uint32_t price_address() const { return address + 744; }
  uint32_t price_bits() const { if (auto* d = direct()) return d->price; return uint32_t((memory::IsHandle(address + 744) ? uint32_t(memory::OwnerLoad(address + 744, 4)) : guest::LoadU32(base, address + 744))); }
  void set_price_bits(uint32_t v) const { if (auto* d = direct()) { d->price = v; return; } if (memory::IsHandle(address + 744)) memory::OwnerStore(address + 744, 4, v); else guest::StoreU32(base, address + 744, v); }
  uint32_t price() const { if (auto* d = direct()) return d->price; return uint32_t((memory::IsHandle(address + 744) ? uint32_t(memory::OwnerLoad(address + 744, 4)) : guest::LoadU32(base, address + 744))); }
  void set_price(uint32_t v) const { if (auto* d = direct()) { d->price = v; return; } if (memory::IsHandle(address + 744)) memory::OwnerStore(address + 744, 4, v); else guest::StoreU32(base, address + 744, v); }
  // string 1934 + it (the maker's name)
  uint32_t maker_address() const { return address + 769; }
  uint8_t maker_bits() const { if (auto* d = direct()) return d->maker; return uint8_t((memory::IsHandle(address + 769) ? uint8_t(memory::OwnerLoad(address + 769, 1)) : guest::LoadU8(base, address + 769))); }
  void set_maker_bits(uint8_t v) const { if (auto* d = direct()) { d->maker = v; return; } if (memory::IsHandle(address + 769)) memory::OwnerStore(address + 769, 1, v); else guest::StoreU8(base, address + 769, v); }
  uint8_t maker() const { if (auto* d = direct()) return d->maker; return uint8_t((memory::IsHandle(address + 769) ? uint8_t(memory::OwnerLoad(address + 769, 1)) : guest::LoadU8(base, address + 769))); }
  void set_maker(uint8_t v) const { if (auto* d = direct()) { d->maker = v; return; } if (memory::IsHandle(address + 769)) memory::OwnerStore(address + 769, 1, v); else guest::StoreU8(base, address + 769, v); }
  // shown by SetSpecTexts (string 421 + it)
  uint32_t unknown_770_address() const { return address + 770; }
  uint8_t unknown_770_bits() const { if (auto* d = direct()) return d->unknown_770; return uint8_t((memory::IsHandle(address + 770) ? uint8_t(memory::OwnerLoad(address + 770, 1)) : guest::LoadU8(base, address + 770))); }
  void set_unknown_770_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_770 = v; return; } if (memory::IsHandle(address + 770)) memory::OwnerStore(address + 770, 1, v); else guest::StoreU8(base, address + 770, v); }
  uint8_t unknown_770() const { if (auto* d = direct()) return d->unknown_770; return uint8_t((memory::IsHandle(address + 770) ? uint8_t(memory::OwnerLoad(address + 770, 1)) : guest::LoadU8(base, address + 770))); }
  void set_unknown_770(uint8_t v) const { if (auto* d = direct()) { d->unknown_770 = v; return; } if (memory::IsHandle(address + 770)) memory::OwnerStore(address + 770, 1, v); else guest::StoreU8(base, address + 770, v); }
  // 0 the first list, 1 the second, others not sold
  uint32_t list_kind_address() const { return address + 771; }
  uint8_t list_kind_bits() const { if (auto* d = direct()) return d->list_kind; return uint8_t((memory::IsHandle(address + 771) ? uint8_t(memory::OwnerLoad(address + 771, 1)) : guest::LoadU8(base, address + 771))); }
  void set_list_kind_bits(uint8_t v) const { if (auto* d = direct()) { d->list_kind = v; return; } if (memory::IsHandle(address + 771)) memory::OwnerStore(address + 771, 1, v); else guest::StoreU8(base, address + 771, v); }
  uint8_t list_kind() const { if (auto* d = direct()) return d->list_kind; return uint8_t((memory::IsHandle(address + 771) ? uint8_t(memory::OwnerLoad(address + 771, 1)) : guest::LoadU8(base, address + 771))); }
  void set_list_kind(uint8_t v) const { if (auto* d = direct()) { d->list_kind = v; return; } if (memory::IsHandle(address + 771)) memory::OwnerStore(address + 771, 1, v); else guest::StoreU8(base, address + 771, v); }
  template <typename T = CarDealerSpecColor>
  T color_records(uint32_t i) const { return T(base, address + 8116 + i * T::kSize); }
  uint32_t color_count_address() const { return address + 8376; }
  uint32_t color_count_bits() const { if (auto* d = direct()) return d->color_count; return uint32_t((memory::IsHandle(address + 8376) ? uint32_t(memory::OwnerLoad(address + 8376, 4)) : guest::LoadU32(base, address + 8376))); }
  void set_color_count_bits(uint32_t v) const { if (auto* d = direct()) { d->color_count = v; return; } if (memory::IsHandle(address + 8376)) memory::OwnerStore(address + 8376, 4, v); else guest::StoreU32(base, address + 8376, v); }
  uint32_t color_count() const { if (auto* d = direct()) return d->color_count; return uint32_t((memory::IsHandle(address + 8376) ? uint32_t(memory::OwnerLoad(address + 8376, 4)) : guest::LoadU32(base, address + 8376))); }
  void set_color_count(uint32_t v) const { if (auto* d = direct()) { d->color_count = v; return; } if (memory::IsHandle(address + 8376)) memory::OwnerStore(address + 8376, 4, v); else guest::StoreU32(base, address + 8376, v); }
  uint32_t car_id_address() const { return address + 8432; }
  uint32_t car_id_bits() const { if (auto* d = direct()) return d->car_id; return uint32_t((memory::IsHandle(address + 8432) ? uint32_t(memory::OwnerLoad(address + 8432, 4)) : guest::LoadU32(base, address + 8432))); }
  void set_car_id_bits(uint32_t v) const { if (auto* d = direct()) { d->car_id = v; return; } if (memory::IsHandle(address + 8432)) memory::OwnerStore(address + 8432, 4, v); else guest::StoreU32(base, address + 8432, v); }
  uint32_t car_id() const { if (auto* d = direct()) return d->car_id; return uint32_t((memory::IsHandle(address + 8432) ? uint32_t(memory::OwnerLoad(address + 8432, 4)) : guest::LoadU32(base, address + 8432))); }
  void set_car_id(uint32_t v) const { if (auto* d = direct()) { d->car_id = v; return; } if (memory::IsHandle(address + 8432)) memory::OwnerStore(address + 8432, 4, v); else guest::StoreU32(base, address + 8432, v); }
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
  CarDealerSpecColorData* direct() const { return static_cast<CarDealerSpecColorData*>(memory::HandleOwn(address, &kCarDealerSpecColorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarDealerSpecColorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarDealerSpecColorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 1 picks the value's offset at 0x82002FF4, else 0x82002FF8
  uint32_t kind_address() const { return address + 0; }
  uint16_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(base, address + 0))); }
  void set_kind_bits(uint16_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(base, address + 0, v); }
  uint16_t kind() const { if (auto* d = direct()) return d->kind; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(base, address + 0))); }
  void set_kind(uint16_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(base, address + 0, v); }
  uint32_t hue_address() const { return address + 4; }
  uint16_t hue_bits() const { if (auto* d = direct()) return d->hue; return uint16_t((memory::IsHandle(address + 4) ? uint16_t(memory::OwnerLoad(address + 4, 2)) : guest::LoadU16(base, address + 4))); }
  void set_hue_bits(uint16_t v) const { if (auto* d = direct()) { d->hue = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 2, v); else guest::StoreU16(base, address + 4, v); }
  uint16_t hue() const { if (auto* d = direct()) return d->hue; return uint16_t((memory::IsHandle(address + 4) ? uint16_t(memory::OwnerLoad(address + 4, 2)) : guest::LoadU16(base, address + 4))); }
  void set_hue(uint16_t v) const { if (auto* d = direct()) { d->hue = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 2, v); else guest::StoreU16(base, address + 4, v); }
  uint32_t saturation_address() const { return address + 6; }
  uint8_t saturation_bits() const { if (auto* d = direct()) return d->saturation; return uint8_t((memory::IsHandle(address + 6) ? uint8_t(memory::OwnerLoad(address + 6, 1)) : guest::LoadU8(base, address + 6))); }
  void set_saturation_bits(uint8_t v) const { if (auto* d = direct()) { d->saturation = v; return; } if (memory::IsHandle(address + 6)) memory::OwnerStore(address + 6, 1, v); else guest::StoreU8(base, address + 6, v); }
  uint8_t saturation() const { if (auto* d = direct()) return d->saturation; return uint8_t((memory::IsHandle(address + 6) ? uint8_t(memory::OwnerLoad(address + 6, 1)) : guest::LoadU8(base, address + 6))); }
  void set_saturation(uint8_t v) const { if (auto* d = direct()) { d->saturation = v; return; } if (memory::IsHandle(address + 6)) memory::OwnerStore(address + 6, 1, v); else guest::StoreU8(base, address + 6, v); }
  // plus the kind's offset, clamped to 0 to 255
  uint32_t value_address() const { return address + 7; }
  uint8_t value_bits() const { if (auto* d = direct()) return d->value; return uint8_t((memory::IsHandle(address + 7) ? uint8_t(memory::OwnerLoad(address + 7, 1)) : guest::LoadU8(base, address + 7))); }
  void set_value_bits(uint8_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 7)) memory::OwnerStore(address + 7, 1, v); else guest::StoreU8(base, address + 7, v); }
  uint8_t value() const { if (auto* d = direct()) return d->value; return uint8_t((memory::IsHandle(address + 7) ? uint8_t(memory::OwnerLoad(address + 7, 1)) : guest::LoadU8(base, address + 7))); }
  void set_value(uint8_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 7)) memory::OwnerStore(address + 7, 1, v); else guest::StoreU8(base, address + 7, v); }
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
