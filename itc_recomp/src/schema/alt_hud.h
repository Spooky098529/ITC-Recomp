// The views of docs/functions/alt-hud.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/alt-hud.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: RaceScreens@+1004.+0, AltHud::ToggleView.r3.+0
#pragma pack(push, 1)
struct AltHudData {
  uint32_t shown = 0;  // +0
  uint8_t pad_4[100] = {};
  float tach_output = 0;  // +104
  uint8_t pad_108[108] = {};
  float boost_output = 0;  // +216
  uint8_t pad_220[8] = {};
  float bar = 0;  // +228
  float bar_high = 0;  // +232
  float bar_low = 0;  // +236
  uint32_t car = 0;  // +240
  int32_t slot0 = 0;  // +244
  int32_t slot1 = 0;  // +248
  int32_t slot2 = 0;  // +252
  uint32_t big_view = 0;  // +256
  uint32_t player = 0;  // +260
};
#pragma pack(pop)
static_assert(sizeof(AltHudData) == 264);
inline constexpr memory::LayoutField kAltHudFields[] = {{0, 4}, {104, 4}, {216, 4}, {228, 4}, {232, 4}, {236, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}, {256, 4}, {260, 4}};
inline constexpr memory::Layout kAltHudLayout = {kAltHudFields, 12};

struct AltHud : View {
  using View::View;
  static constexpr uint32_t kSize = 264;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  AltHudData* data() const { return static_cast<AltHudData*>(memory::HandleObject(address)); }
  static AltHud New(uint8_t* base) {
    return AltHud(base, memory::MakeHandle(new AltHudData(), sizeof(AltHudData), "AltHud", &kAltHudLayout));
  }
  template <uint32_t Stride = kSize>
  static AltHud NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AltHudData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AltHudData*>(::operator new(sizeof(AltHudData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AltHudData();
    return AltHud(base, memory::MakeHandle(p, sizeof(AltHudData) * count, "AltHud", &kAltHudLayout, sizeof(AltHudData)));
  }
  AltHud at(uint32_t i) const { return AltHud(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAltHudLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAltHudLayout, offset, width, v); }
  // node 3's flag, the toggle AltHud::Tick flips on X
  uint32_t shown_address() const { return address + 0; }
  uint32_t shown() const { return data()->shown; }
  void set_shown(uint32_t v) const { data()->shown = v; }
  uint32_t shown_bits() const { return data()->shown; }
  void set_shown_bits(uint32_t v) const { data()->shown = v; }
  // the tachometer's HudCurve (one of three at 0x8200FD28)
  uint32_t tach_curve_address() const { return address + 4; }
  uint8_t* tach_curve() const { return reinterpret_cast<uint8_t*>(data()) + 4; }
  // the tachometer curve's output (its +100): the needle's rotation
  uint32_t tach_output_address() const { return address + 104; }
  float tach_output() const { return data()->tach_output; }
  void set_tach_output(float v) const { data()->tach_output = v; }
  uint32_t tach_output_bits() const { return std::bit_cast<uint32_t>(data()->tach_output); }
  void set_tach_output_bits(uint32_t v) const { data()->tach_output = std::bit_cast<float>(v); }
  // the boost's HudCurve (copied from 0x8200FE78)
  uint32_t boost_curve_address() const { return address + 116; }
  uint8_t* boost_curve() const { return reinterpret_cast<uint8_t*>(data()) + 116; }
  // the boost curve's output (its +100): the needle's rotation
  uint32_t boost_output_address() const { return address + 216; }
  float boost_output() const { return data()->boost_output; }
  void set_boost_output(float v) const { data()->boost_output = v; }
  uint32_t boost_output_bits() const { return std::bit_cast<uint32_t>(data()->boost_output); }
  void set_boost_output_bits(uint32_t v) const { data()->boost_output = std::bit_cast<float>(v); }
  // the bar's value (the car's +2420)
  uint32_t bar_address() const { return address + 228; }
  float bar() const { return data()->bar; }
  void set_bar(float v) const { data()->bar = v; }
  uint32_t bar_bits() const { return std::bit_cast<uint32_t>(data()->bar); }
  void set_bar_bits(uint32_t v) const { data()->bar = std::bit_cast<float>(v); }
  // the bar's range top; nothing found writes it (0.0)
  uint32_t bar_high_address() const { return address + 232; }
  float bar_high() const { return data()->bar_high; }
  void set_bar_high(float v) const { data()->bar_high = v; }
  uint32_t bar_high_bits() const { return std::bit_cast<uint32_t>(data()->bar_high); }
  void set_bar_high_bits(uint32_t v) const { data()->bar_high = std::bit_cast<float>(v); }
  // the bar's range bottom; nothing found writes it (0.0)
  uint32_t bar_low_address() const { return address + 236; }
  float bar_low() const { return data()->bar_low; }
  void set_bar_low(float v) const { data()->bar_low = v; }
  uint32_t bar_low_bits() const { return std::bit_cast<uint32_t>(data()->bar_low); }
  void set_bar_low_bits(uint32_t v) const { data()->bar_low = std::bit_cast<float>(v); }
  // a Vehicle: Vehicle::Get(player)
  uint32_t car_address() const { return address + 240; }
  View car() const { return View(base, data()->car); }
  void set_car(uint32_t v) const { data()->car = v; }
  uint32_t car_bits() const { return data()->car; }
  void set_car_bits(uint32_t v) const { data()->car = v; }
  // the first extra gauge: 0 float, 1 int to 119, 2 int to 149, -1 none
  uint32_t slot0_address() const { return address + 244; }
  int32_t slot0() const { return data()->slot0; }
  void set_slot0(int32_t v) const { data()->slot0 = v; }
  uint32_t slot0_bits() const { return std::bit_cast<uint32_t>(data()->slot0); }
  void set_slot0_bits(uint32_t v) const { data()->slot0 = std::bit_cast<int32_t>(v); }
  // the second extra gauge, as slot0
  uint32_t slot1_address() const { return address + 248; }
  int32_t slot1() const { return data()->slot1; }
  void set_slot1(int32_t v) const { data()->slot1 = v; }
  uint32_t slot1_bits() const { return std::bit_cast<uint32_t>(data()->slot1); }
  void set_slot1_bits(uint32_t v) const { data()->slot1 = std::bit_cast<int32_t>(v); }
  // the third extra gauge, as slot0
  uint32_t slot2_address() const { return address + 252; }
  int32_t slot2() const { return data()->slot2; }
  void set_slot2(int32_t v) const { data()->slot2 = v; }
  uint32_t slot2_bits() const { return std::bit_cast<uint32_t>(data()->slot2); }
  void set_slot2_bits(uint32_t v) const { data()->slot2 = std::bit_cast<int32_t>(v); }
  // AltHud::ToggleView's flag: the big speed readout shown
  uint32_t big_view_address() const { return address + 256; }
  uint32_t big_view() const { return data()->big_view; }
  void set_big_view(uint32_t v) const { data()->big_view = v; }
  uint32_t big_view_bits() const { return data()->big_view; }
  void set_big_view_bits(uint32_t v) const { data()->big_view = v; }
  // the player (0 or 1)
  uint32_t player_address() const { return address + 260; }
  uint32_t player() const { return data()->player; }
  void set_player(uint32_t v) const { data()->player = v; }
  uint32_t player_bits() const { return data()->player; }
  void set_player_bits(uint32_t v) const { data()->player = v; }
};

// embedded; measure: RaceScreens@+1004, RaceScreens::~RaceScreens.r3@+1004, AltHud::ToggleView.r3
#pragma pack(push, 1)
struct AltHudHolderData {
  uint32_t object = 0;  // +0
  uint32_t menu_list = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(AltHudHolderData) == 8);
inline constexpr memory::LayoutField kAltHudHolderFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kAltHudHolderLayout = {kAltHudHolderFields, 2};

struct AltHudHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  AltHudHolderData* data() const { return static_cast<AltHudHolderData*>(memory::HandleObject(address)); }
  static AltHudHolder New(uint8_t* base) {
    return AltHudHolder(base, memory::MakeHandle(new AltHudHolderData(), sizeof(AltHudHolderData), "AltHudHolder", &kAltHudHolderLayout));
  }
  template <uint32_t Stride = kSize>
  static AltHudHolder NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AltHudHolderData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AltHudHolderData*>(::operator new(sizeof(AltHudHolderData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AltHudHolderData();
    return AltHudHolder(base, memory::MakeHandle(p, sizeof(AltHudHolderData) * count, "AltHudHolder", &kAltHudHolderLayout, sizeof(AltHudHolderData)));
  }
  AltHudHolder at(uint32_t i) const { return AltHudHolder(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAltHudHolderLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAltHudHolderLayout, offset, width, v); }
  // 264 bytes, cleared
  uint32_t object_address() const { return address + 0; }
  template <typename T = AltHud>
  T object() const { return T(base, data()->object); }
  void set_object(const View& w) const { const uint32_t v = w.address; data()->object = v; }
  void set_object(uint32_t v) const { data()->object = v; }
  uint32_t object_bits() const { return data()->object; }
  void set_object_bits(uint32_t v) const { data()->object = v; }
  // a MenuList (72 bytes, one item); its X press flips the toggle
  uint32_t menu_list_address() const { return address + 4; }
  View menu_list() const { return View(base, data()->menu_list); }
  void set_menu_list(uint32_t v) const { data()->menu_list = v; }
  uint32_t menu_list_bits() const { return data()->menu_list; }
  void set_menu_list_bits(uint32_t v) const { data()->menu_list = v; }
};

// The image words docs/functions/alt-hud.md's code uses.
struct AltHudGlobals {
  // the car input records by player (racer-controllers.md): +4 the throttle, +6 the brake, +8 the steering, signed halfwords
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // AltHud::PowerClass: below the stock power times this, class 0
  static float power_low(uint8_t* base) { return guest::LoadFloat(base, 0x820029F0u); }
  static void set_power_low(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029F0u, v); }
  static constexpr uint32_t power_low_address() { return 0x820029F0u; }
  // AltHud::PowerClass: below the stock power times this, class 1, else 2
  static float power_high(uint8_t* base) { return guest::LoadFloat(base, 0x82002C40u); }
  static void set_power_high(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C40u, v); }
  static constexpr uint32_t power_high_address() { return 0x82002C40u; }
  // the input halfwords' scale, probably 1 / 32,767
  static float input_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD4u); }
  static void set_input_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD4u, v); }
  static constexpr uint32_t input_scale_address() { return 0x82002BD4u; }
  // the km/h's factor for the dials
  static double kmh_factor(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8200A510u)); }
  static void set_kmh_factor(uint8_t* base, double v) { guest::StoreU64(base, 0x8200A510u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t kmh_factor_address() { return 0x8200A510u; }
  // 10.0, the digits' base for PowInt
  static double ten(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x8200F728u)); }
  static void set_ten(uint8_t* base, double v) { guest::StoreU64(base, 0x8200F728u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t ten_address() { return 0x8200F728u; }
  // added to the tachometer curve's output
  static float tach_offset(uint8_t* base) { return guest::LoadFloat(base, 0x8200F720u); }
  static void set_tach_offset(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200F720u, v); }
  static constexpr uint32_t tach_offset_address() { return 0x8200F720u; }
  // three 112-byte tachometer curves
  static constexpr uint32_t tach_curves_address() { return 0x8200FD28u; }
  // the boost's curve
  static constexpr uint32_t boost_curve_address() { return 0x8200FE78u; }
  // 16 bytes an index (the racer record's +60): word 0 the curve, words 1 to 3 the dial's faces by power class
  static constexpr uint32_t curve_table_address() { return 0x8200FEE8u; }
  // AltHud::SetSteering's scale
  static float steering_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82010368u); }
  static void set_steering_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82010368u, v); }
  static constexpr uint32_t steering_scale_address() { return 0x82010368u; }
};

}  // namespace schema
