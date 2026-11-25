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
  static AltHud New() {
    return AltHud(memory::MakeHandle(new AltHudData(), sizeof(AltHudData), "AltHud", &kAltHudLayout));
  }
  template <uint32_t Stride = kSize>
  static AltHud NewArray(uint32_t count) {
    static_assert(sizeof(AltHudData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AltHudData*>(::operator new(sizeof(AltHudData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AltHudData();
    return AltHud(memory::MakeHandle(p, sizeof(AltHudData) * count, "AltHud", &kAltHudLayout, sizeof(AltHudData)));
  }
  AltHud at(uint32_t i) const { return AltHud(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AltHudData* direct() const { return static_cast<AltHudData*>(memory::HandleOwn(address, &kAltHudLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAltHudLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAltHudLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // node 3's flag, the toggle AltHud::Tick flips on X
  uint32_t shown_address() const { return address + 0; }
  uint32_t shown_bits() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t shown() const { if (auto* d = direct()) return d->shown; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_shown(uint32_t v) const { if (auto* d = direct()) { d->shown = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the tachometer's HudCurve (one of three at 0x8200FD28)
  uint32_t tach_curve_address() const { return address + 4; }
  uint8_t* tach_curve() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4 : guest::Pointer(address + 4); }
  // the tachometer curve's output (its +100): the needle's rotation
  uint32_t tach_output_address() const { return address + 104; }
  uint32_t tach_output_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tach_output); return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_tach_output_bits(uint32_t v) const { if (auto* d = direct()) { d->tach_output = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  float tach_output() const { return std::bit_cast<float>(tach_output_bits()); }
  void set_tach_output(float v) const { set_tach_output_bits(std::bit_cast<uint32_t>(v)); }
  // the boost's HudCurve (copied from 0x8200FE78)
  uint32_t boost_curve_address() const { return address + 116; }
  uint8_t* boost_curve() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 116 : guest::Pointer(address + 116); }
  // the boost curve's output (its +100): the needle's rotation
  uint32_t boost_output_address() const { return address + 216; }
  uint32_t boost_output_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->boost_output); return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_boost_output_bits(uint32_t v) const { if (auto* d = direct()) { d->boost_output = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  float boost_output() const { return std::bit_cast<float>(boost_output_bits()); }
  void set_boost_output(float v) const { set_boost_output_bits(std::bit_cast<uint32_t>(v)); }
  // the bar's value (the car's +2420)
  uint32_t bar_address() const { return address + 228; }
  uint32_t bar_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->bar); return uint32_t((memory::IsHandle(address + 228) ? uint32_t(memory::OwnerLoad(address + 228, 4)) : guest::LoadU32(address + 228))); }
  void set_bar_bits(uint32_t v) const { if (auto* d = direct()) { d->bar = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 228)) memory::OwnerStore(address + 228, 4, v); else guest::StoreU32(address + 228, v); }
  float bar() const { return std::bit_cast<float>(bar_bits()); }
  void set_bar(float v) const { set_bar_bits(std::bit_cast<uint32_t>(v)); }
  // the bar's range top; nothing found writes it (0.0)
  uint32_t bar_high_address() const { return address + 232; }
  uint32_t bar_high_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->bar_high); return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(address + 232))); }
  void set_bar_high_bits(uint32_t v) const { if (auto* d = direct()) { d->bar_high = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(address + 232, v); }
  float bar_high() const { return std::bit_cast<float>(bar_high_bits()); }
  void set_bar_high(float v) const { set_bar_high_bits(std::bit_cast<uint32_t>(v)); }
  // the bar's range bottom; nothing found writes it (0.0)
  uint32_t bar_low_address() const { return address + 236; }
  uint32_t bar_low_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->bar_low); return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_bar_low_bits(uint32_t v) const { if (auto* d = direct()) { d->bar_low = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  float bar_low() const { return std::bit_cast<float>(bar_low_bits()); }
  void set_bar_low(float v) const { set_bar_low_bits(std::bit_cast<uint32_t>(v)); }
  // a Vehicle: Vehicle::Get(player)
  uint32_t car_address() const { return address + 240; }
  uint32_t car_bits() const { if (auto* d = direct()) return d->car; return uint32_t((memory::IsHandle(address + 240) ? uint32_t(memory::OwnerLoad(address + 240, 4)) : guest::LoadU32(address + 240))); }
  void set_car_bits(uint32_t v) const { if (auto* d = direct()) { d->car = v; return; } if (memory::IsHandle(address + 240)) memory::OwnerStore(address + 240, 4, v); else guest::StoreU32(address + 240, v); }
  View car() const { return View(car_bits()); }
  void set_car(uint32_t v) const { set_car_bits(v); }
  // the first extra gauge: 0 float, 1 int to 119, 2 int to 149, -1 none
  uint32_t slot0_address() const { return address + 244; }
  uint32_t slot0_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->slot0); return uint32_t((memory::IsHandle(address + 244) ? uint32_t(memory::OwnerLoad(address + 244, 4)) : guest::LoadU32(address + 244))); }
  void set_slot0_bits(uint32_t v) const { if (auto* d = direct()) { d->slot0 = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 244)) memory::OwnerStore(address + 244, 4, v); else guest::StoreU32(address + 244, v); }
  int32_t slot0() const { return int32_t(slot0_bits()); }
  void set_slot0(int32_t v) const { set_slot0_bits(uint32_t(v)); }
  // the second extra gauge, as slot0
  uint32_t slot1_address() const { return address + 248; }
  uint32_t slot1_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->slot1); return uint32_t((memory::IsHandle(address + 248) ? uint32_t(memory::OwnerLoad(address + 248, 4)) : guest::LoadU32(address + 248))); }
  void set_slot1_bits(uint32_t v) const { if (auto* d = direct()) { d->slot1 = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 248)) memory::OwnerStore(address + 248, 4, v); else guest::StoreU32(address + 248, v); }
  int32_t slot1() const { return int32_t(slot1_bits()); }
  void set_slot1(int32_t v) const { set_slot1_bits(uint32_t(v)); }
  // the third extra gauge, as slot0
  uint32_t slot2_address() const { return address + 252; }
  uint32_t slot2_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->slot2); return uint32_t((memory::IsHandle(address + 252) ? uint32_t(memory::OwnerLoad(address + 252, 4)) : guest::LoadU32(address + 252))); }
  void set_slot2_bits(uint32_t v) const { if (auto* d = direct()) { d->slot2 = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 252)) memory::OwnerStore(address + 252, 4, v); else guest::StoreU32(address + 252, v); }
  int32_t slot2() const { return int32_t(slot2_bits()); }
  void set_slot2(int32_t v) const { set_slot2_bits(uint32_t(v)); }
  // AltHud::ToggleView's flag: the big speed readout shown
  uint32_t big_view_address() const { return address + 256; }
  uint32_t big_view_bits() const { if (auto* d = direct()) return d->big_view; return uint32_t((memory::IsHandle(address + 256) ? uint32_t(memory::OwnerLoad(address + 256, 4)) : guest::LoadU32(address + 256))); }
  void set_big_view_bits(uint32_t v) const { if (auto* d = direct()) { d->big_view = v; return; } if (memory::IsHandle(address + 256)) memory::OwnerStore(address + 256, 4, v); else guest::StoreU32(address + 256, v); }
  uint32_t big_view() const { if (auto* d = direct()) return d->big_view; return uint32_t((memory::IsHandle(address + 256) ? uint32_t(memory::OwnerLoad(address + 256, 4)) : guest::LoadU32(address + 256))); }
  void set_big_view(uint32_t v) const { if (auto* d = direct()) { d->big_view = v; return; } if (memory::IsHandle(address + 256)) memory::OwnerStore(address + 256, 4, v); else guest::StoreU32(address + 256, v); }
  // the player (0 or 1)
  uint32_t player_address() const { return address + 260; }
  uint32_t player_bits() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_player_bits(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
  uint32_t player() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 260) ? uint32_t(memory::OwnerLoad(address + 260, 4)) : guest::LoadU32(address + 260))); }
  void set_player(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 260)) memory::OwnerStore(address + 260, 4, v); else guest::StoreU32(address + 260, v); }
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
  static AltHudHolder New() {
    return AltHudHolder(memory::MakeHandle(new AltHudHolderData(), sizeof(AltHudHolderData), "AltHudHolder", &kAltHudHolderLayout));
  }
  template <uint32_t Stride = kSize>
  static AltHudHolder NewArray(uint32_t count) {
    static_assert(sizeof(AltHudHolderData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AltHudHolderData*>(::operator new(sizeof(AltHudHolderData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AltHudHolderData();
    return AltHudHolder(memory::MakeHandle(p, sizeof(AltHudHolderData) * count, "AltHudHolder", &kAltHudHolderLayout, sizeof(AltHudHolderData)));
  }
  AltHudHolder at(uint32_t i) const { return AltHudHolder(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AltHudHolderData* direct() const { return static_cast<AltHudHolderData*>(memory::HandleOwn(address, &kAltHudHolderLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAltHudHolderLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAltHudHolderLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 264 bytes, cleared
  uint32_t object_address() const { return address + 0; }
  uint32_t object_bits() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_object_bits(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = AltHud>
  T object() const { return T(object_bits()); }
  void set_object(const View& w) const { set_object_bits(w.address); }
  void set_object(uint32_t v) const { set_object_bits(v); }
  // a MenuList (72 bytes, one item); its X press flips the toggle
  uint32_t menu_list_address() const { return address + 4; }
  uint32_t menu_list_bits() const { if (auto* d = direct()) return d->menu_list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_menu_list_bits(uint32_t v) const { if (auto* d = direct()) { d->menu_list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  View menu_list() const { return View(menu_list_bits()); }
  void set_menu_list(uint32_t v) const { set_menu_list_bits(v); }
};

// The image words docs/functions/alt-hud.md's code uses.
struct AltHudGlobals {
  // the car input records by player (racer-controllers.md): +4 the throttle, +6 the brake, +8 the steering, signed halfwords
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // AltHud::PowerClass: below the stock power times this, class 0
  static float power_low() { return guest::LoadFloat(0x820029F0u); }
  static void set_power_low(float v) { guest::StoreFloat(0x820029F0u, v); }
  static constexpr uint32_t power_low_address() { return 0x820029F0u; }
  // AltHud::PowerClass: below the stock power times this, class 1, else 2
  static float power_high() { return guest::LoadFloat(0x82002C40u); }
  static void set_power_high(float v) { guest::StoreFloat(0x82002C40u, v); }
  static constexpr uint32_t power_high_address() { return 0x82002C40u; }
  // the input halfwords' scale, probably 1 / 32,767
  static float input_scale() { return guest::LoadFloat(0x82002BD4u); }
  static void set_input_scale(float v) { guest::StoreFloat(0x82002BD4u, v); }
  static constexpr uint32_t input_scale_address() { return 0x82002BD4u; }
  // the km/h's factor for the dials
  static double kmh_factor() { return std::bit_cast<double>(guest::LoadU64(0x8200A510u)); }
  static void set_kmh_factor(double v) { guest::StoreU64(0x8200A510u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t kmh_factor_address() { return 0x8200A510u; }
  // 10.0, the digits' base for PowInt
  static double ten() { return std::bit_cast<double>(guest::LoadU64(0x8200F728u)); }
  static void set_ten(double v) { guest::StoreU64(0x8200F728u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t ten_address() { return 0x8200F728u; }
  // added to the tachometer curve's output
  static float tach_offset() { return guest::LoadFloat(0x8200F720u); }
  static void set_tach_offset(float v) { guest::StoreFloat(0x8200F720u, v); }
  static constexpr uint32_t tach_offset_address() { return 0x8200F720u; }
  // three 112-byte tachometer curves
  static constexpr uint32_t tach_curves_address() { return 0x8200FD28u; }
  // the boost's curve
  static constexpr uint32_t boost_curve_address() { return 0x8200FE78u; }
  // 16 bytes an index (the racer record's +60): word 0 the curve, words 1 to 3 the dial's faces by power class
  static constexpr uint32_t curve_table_address() { return 0x8200FEE8u; }
  // AltHud::SetSteering's scale
  static float steering_scale() { return guest::LoadFloat(0x82010368u); }
  static void set_steering_scale(float v) { guest::StoreFloat(0x82010368u, v); }
  static constexpr uint32_t steering_scale_address() { return 0x82010368u; }
};

}  // namespace schema
