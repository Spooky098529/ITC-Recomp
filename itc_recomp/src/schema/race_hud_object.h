// The views of docs/functions/race-hud-object.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-hud-object.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceHud, sub_822155C0.r3, sub_82213860.r3, sub_82214C88.r3, RaceHud::ApplyOptions.r3, RaceHud::StopCountdown.r3
#pragma pack(push, 1)
struct RaceHudData {
  uint32_t option_node3 = 0;  // +0
  uint32_t option_node1 = 0;  // +4
  uint32_t option_gauges = 0;  // +8
  uint32_t option_mirror = 0;  // +12
  uint32_t player = 0;  // +16
  uint8_t pad_20[100] = {};
  float tach_output = 0;  // +120
  uint8_t pad_124[108] = {};
  float boost_output = 0;  // +232
  uint8_t pad_236[8] = {};
  uint8_t bars[160] = {};  // +244, guest order
  float bar_value = 0;  // +404
  float bar_max = 0;  // +408
  float bar_min = 0;  // +412
  uint32_t sign_zone = 0;  // +416
  uint32_t sign_flags = 0;  // +420
  uint32_t signs_on = 0;  // +424
  uint32_t place_zone = 0;  // +428
  uint32_t view = 0;  // +432
  uint32_t countdown_running = 0;  // +436
  uint32_t vehicle = 0;  // +440
  uint32_t racers = 0;  // +444
  uint32_t mode = 0;  // +448
  uint32_t racer_count = 0;  // +452
  uint32_t any_bar_low = 0;  // +456
  uint32_t item_shown = 0;  // +460
  uint32_t timer_animation = 0;  // +464
  uint32_t warning_shown = 0;  // +468
  uint32_t view_dashboard = 0;  // +472
  uint32_t part_fitted = 0;  // +476
  uint32_t gauge_slot0 = 0;  // +480
  uint32_t gauge_slot1 = 0;  // +484
  uint32_t gauge_slot2 = 0;  // +488
  uint32_t recording_full = 0;  // +492
  uint32_t unknown_496 = 0;  // +496
  uint8_t pad_500[72] = {};
  uint32_t unknown_572 = 0;  // +572
  uint32_t unknown_576 = 0;  // +576
  uint32_t unknown_580 = 0;  // +580
  uint32_t unknown_584 = 0;  // +584
  uint32_t swapped_texture = 0;  // +588
  uint32_t mirror_texture = 0;  // +592
  uint32_t mirror_width = 0;  // +596
  uint32_t mirror_height = 0;  // +600
  uint32_t racers_movie = 0;  // +604
  uint32_t dashboard = 0;  // +608
  uint32_t countdown = 0;  // +612
  uint32_t warning = 0;  // +616
  uint32_t movie2 = 0;  // +620
  uint32_t timer = 0;  // +624
  uint32_t movie1 = 0;  // +628
  uint32_t splits = 0;  // +632
  uint32_t movie19 = 0;  // +636
};
#pragma pack(pop)
static_assert(sizeof(RaceHudData) == 640);
inline constexpr memory::LayoutField kRaceHudFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {120, 4}, {232, 4}, {404, 4}, {408, 4}, {412, 4}, {416, 4}, {420, 4}, {424, 4}, {428, 4}, {432, 4}, {436, 4}, {440, 4}, {444, 4}, {448, 4}, {452, 4}, {456, 4}, {460, 4}, {464, 4}, {468, 4}, {472, 4}, {476, 4}, {480, 4}, {484, 4}, {488, 4}, {492, 4}, {496, 4}, {572, 4}, {576, 4}, {580, 4}, {584, 4}, {588, 4}, {592, 4}, {596, 4}, {600, 4}, {604, 4}, {608, 4}, {612, 4}, {616, 4}, {620, 4}, {624, 4}, {628, 4}, {632, 4}, {636, 4}};
inline constexpr memory::Layout kRaceHudLayout = {kRaceHudFields, 48};

struct RaceHud : View {
  using View::View;
  static constexpr uint32_t kSize = 640;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceHudData* data() const { return static_cast<RaceHudData*>(memory::HandleObject(address)); }
  static RaceHud New(uint8_t* base) {
    return RaceHud(base, memory::MakeHandle(new RaceHudData(), sizeof(RaceHudData), "RaceHud", &kRaceHudLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceHud NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceHudData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceHudData*>(::operator new(sizeof(RaceHudData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceHudData();
    return RaceHud(base, memory::MakeHandle(p, sizeof(RaceHudData) * count, "RaceHud", &kRaceHudLayout, sizeof(RaceHudData)));
  }
  RaceHud at(uint32_t i) const { return RaceHud(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceHudData* direct() const { return static_cast<RaceHudData*>(memory::HandleOwn(address, &kRaceHudLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceHudLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceHudLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // HUD option 0 (player 0's save data +0x1F85C): the dashboard's node 3 shown
  uint32_t option_node3_address() const { return address + 0; }
  uint32_t option_node3_bits() const { if (auto* d = direct()) return d->option_node3; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_option_node3_bits(uint32_t v) const { if (auto* d = direct()) { d->option_node3 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t option_node3() const { if (auto* d = direct()) return d->option_node3; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_option_node3(uint32_t v) const { if (auto* d = direct()) { d->option_node3 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // HUD option 1: the dashboard's node 1 shown
  uint32_t option_node1_address() const { return address + 4; }
  uint32_t option_node1_bits() const { if (auto* d = direct()) return d->option_node1; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_option_node1_bits(uint32_t v) const { if (auto* d = direct()) { d->option_node1 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t option_node1() const { if (auto* d = direct()) return d->option_node1; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_option_node1(uint32_t v) const { if (auto* d = direct()) { d->option_node1 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // HUD option 2: nodes (0, 3) and (0, 4) shown, node (0, 1) with +476
  uint32_t option_gauges_address() const { return address + 8; }
  uint32_t option_gauges_bits() const { if (auto* d = direct()) return d->option_gauges; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_option_gauges_bits(uint32_t v) const { if (auto* d = direct()) { d->option_gauges = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t option_gauges() const { if (auto* d = direct()) return d->option_gauges; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_option_gauges(uint32_t v) const { if (auto* d = direct()) { d->option_gauges = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // HUD option 3: the cockpit node 4 and the rear-view mirror by the view
  uint32_t option_mirror_address() const { return address + 12; }
  uint32_t option_mirror_bits() const { if (auto* d = direct()) return d->option_mirror; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_option_mirror_bits(uint32_t v) const { if (auto* d = direct()) { d->option_mirror = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t option_mirror() const { if (auto* d = direct()) return d->option_mirror; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_option_mirror(uint32_t v) const { if (auto* d = direct()) { d->option_mirror = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the player's index (RaceHud::SetPlayer)
  uint32_t player_address() const { return address + 16; }
  uint32_t player_bits() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_player_bits(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t player() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_player(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // the tachometer's HudCurve (one of three at 0x8200D1F8)
  uint32_t tach_curve_address() const { return address + 20; }
  uint8_t* tach_curve() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 20 : guest::Pointer(base, address + 20); }
  // the tachometer curve's output (its +100): the needle's rotation
  uint32_t tach_output_address() const { return address + 120; }
  uint32_t tach_output_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->tach_output); return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(base, address + 120))); }
  void set_tach_output_bits(uint32_t v) const { if (auto* d = direct()) { d->tach_output = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(base, address + 120, v); }
  float tach_output() const { return std::bit_cast<float>(tach_output_bits()); }
  void set_tach_output(float v) const { set_tach_output_bits(std::bit_cast<uint32_t>(v)); }
  // the boost's HudCurve (copied from 0x8200D348)
  uint32_t boost_curve_address() const { return address + 132; }
  uint8_t* boost_curve() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 132 : guest::Pointer(base, address + 132); }
  // the boost curve's output (its +100): the needle's rotation
  uint32_t boost_output_address() const { return address + 232; }
  uint32_t boost_output_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->boost_output); return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(base, address + 232))); }
  void set_boost_output_bits(uint32_t v) const { if (auto* d = direct()) { d->boost_output = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(base, address + 232, v); }
  float boost_output() const { return std::bit_cast<float>(boost_output_bits()); }
  void set_boost_output(float v) const { set_boost_output_bits(std::bit_cast<uint32_t>(v)); }
  // the racers' bars: four RaceHudBar, 40 bytes each
  uint32_t bars_address() const { return address + 244; }
  uint8_t* bars() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 244 : guest::Pointer(base, address + 244); }
  // the bar at node 35: its value (the car's +2420)
  uint32_t bar_value_address() const { return address + 404; }
  uint32_t bar_value_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->bar_value); return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_bar_value_bits(uint32_t v) const { if (auto* d = direct()) { d->bar_value = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  float bar_value() const { return std::bit_cast<float>(bar_value_bits()); }
  void set_bar_value(float v) const { set_bar_value_bits(std::bit_cast<uint32_t>(v)); }
  // the bar at node 35: its top
  uint32_t bar_max_address() const { return address + 408; }
  uint32_t bar_max_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->bar_max); return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(base, address + 408))); }
  void set_bar_max_bits(uint32_t v) const { if (auto* d = direct()) { d->bar_max = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(base, address + 408, v); }
  float bar_max() const { return std::bit_cast<float>(bar_max_bits()); }
  void set_bar_max(float v) const { set_bar_max_bits(std::bit_cast<uint32_t>(v)); }
  // the bar at node 35: its bottom (0.0)
  uint32_t bar_min_address() const { return address + 412; }
  uint32_t bar_min_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->bar_min); return uint32_t((memory::IsHandle(address + 412) ? uint32_t(memory::OwnerLoad(address + 412, 4)) : guest::LoadU32(base, address + 412))); }
  void set_bar_min_bits(uint32_t v) const { if (auto* d = direct()) { d->bar_min = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 412)) memory::OwnerStore(address + 412, 4, v); else guest::StoreU32(base, address + 412, v); }
  float bar_min() const { return std::bit_cast<float>(bar_min_bits()); }
  void set_bar_min(float v) const { set_bar_min_bits(std::bit_cast<uint32_t>(v)); }
  // the expressway sign's track zone (0-31, -1 none)
  uint32_t sign_zone_address() const { return address + 416; }
  uint32_t sign_zone_bits() const { if (auto* d = direct()) return d->sign_zone; return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(base, address + 416))); }
  void set_sign_zone_bits(uint32_t v) const { if (auto* d = direct()) { d->sign_zone = v; return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(base, address + 416, v); }
  uint32_t sign_zone() const { if (auto* d = direct()) return d->sign_zone; return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(base, address + 416))); }
  void set_sign_zone(uint32_t v) const { if (auto* d = direct()) { d->sign_zone = v; return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(base, address + 416, v); }
  // the course's sign flags (one of the 24 tables at 0x8200D9D0, by Race::CourseLayout)
  uint32_t sign_flags_address() const { return address + 420; }
  uint32_t sign_flags_bits() const { if (auto* d = direct()) return d->sign_flags; return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(base, address + 420))); }
  void set_sign_flags_bits(uint32_t v) const { if (auto* d = direct()) { d->sign_flags = v; return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(base, address + 420, v); }
  uint32_t sign_flags() const { if (auto* d = direct()) return d->sign_flags; return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(base, address + 420))); }
  void set_sign_flags(uint32_t v) const { if (auto* d = direct()) { d->sign_flags = v; return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(base, address + 420, v); }
  // 1 after RaceHud::SetUpSigns
  uint32_t signs_on_address() const { return address + 424; }
  uint32_t signs_on_bits() const { if (auto* d = direct()) return d->signs_on; return uint32_t((memory::IsHandle(address + 424) ? uint32_t(memory::OwnerLoad(address + 424, 4)) : guest::LoadU32(base, address + 424))); }
  void set_signs_on_bits(uint32_t v) const { if (auto* d = direct()) { d->signs_on = v; return; } if (memory::IsHandle(address + 424)) memory::OwnerStore(address + 424, 4, v); else guest::StoreU32(base, address + 424, v); }
  uint32_t signs_on() const { if (auto* d = direct()) return d->signs_on; return uint32_t((memory::IsHandle(address + 424) ? uint32_t(memory::OwnerLoad(address + 424, 4)) : guest::LoadU32(base, address + 424))); }
  void set_signs_on(uint32_t v) const { if (auto* d = direct()) { d->signs_on = v; return; } if (memory::IsHandle(address + 424)) memory::OwnerStore(address + 424, 4, v); else guest::StoreU32(base, address + 424, v); }
  // the place name's track zone (32-69, -1 none)
  uint32_t place_zone_address() const { return address + 428; }
  uint32_t place_zone_bits() const { if (auto* d = direct()) return d->place_zone; return uint32_t((memory::IsHandle(address + 428) ? uint32_t(memory::OwnerLoad(address + 428, 4)) : guest::LoadU32(base, address + 428))); }
  void set_place_zone_bits(uint32_t v) const { if (auto* d = direct()) { d->place_zone = v; return; } if (memory::IsHandle(address + 428)) memory::OwnerStore(address + 428, 4, v); else guest::StoreU32(base, address + 428, v); }
  uint32_t place_zone() const { if (auto* d = direct()) return d->place_zone; return uint32_t((memory::IsHandle(address + 428) ? uint32_t(memory::OwnerLoad(address + 428, 4)) : guest::LoadU32(base, address + 428))); }
  void set_place_zone(uint32_t v) const { if (auto* d = direct()) { d->place_zone = v; return; } if (memory::IsHandle(address + 428)) memory::OwnerStore(address + 428, 4, v); else guest::StoreU32(base, address + 428, v); }
  // the camera view (viewport 0's +1860)
  uint32_t view_address() const { return address + 432; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 432) ? uint32_t(memory::OwnerLoad(address + 432, 4)) : guest::LoadU32(base, address + 432))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 432)) memory::OwnerStore(address + 432, 4, v); else guest::StoreU32(base, address + 432, v); }
  uint32_t view() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 432) ? uint32_t(memory::OwnerLoad(address + 432, 4)) : guest::LoadU32(base, address + 432))); }
  void set_view(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 432)) memory::OwnerStore(address + 432, 4, v); else guest::StoreU32(base, address + 432, v); }
  // 1 from StartCountdown, then UiMovie::Run's answer on the countdown
  uint32_t countdown_running_address() const { return address + 436; }
  uint32_t countdown_running_bits() const { if (auto* d = direct()) return d->countdown_running; return uint32_t((memory::IsHandle(address + 436) ? uint32_t(memory::OwnerLoad(address + 436, 4)) : guest::LoadU32(base, address + 436))); }
  void set_countdown_running_bits(uint32_t v) const { if (auto* d = direct()) { d->countdown_running = v; return; } if (memory::IsHandle(address + 436)) memory::OwnerStore(address + 436, 4, v); else guest::StoreU32(base, address + 436, v); }
  uint32_t countdown_running() const { if (auto* d = direct()) return d->countdown_running; return uint32_t((memory::IsHandle(address + 436) ? uint32_t(memory::OwnerLoad(address + 436, 4)) : guest::LoadU32(base, address + 436))); }
  void set_countdown_running(uint32_t v) const { if (auto* d = direct()) { d->countdown_running = v; return; } if (memory::IsHandle(address + 436)) memory::OwnerStore(address + 436, 4, v); else guest::StoreU32(base, address + 436, v); }
  // Vehicle::Get(+16)
  uint32_t vehicle_address() const { return address + 440; }
  uint32_t vehicle_bits() const { if (auto* d = direct()) return d->vehicle; return uint32_t((memory::IsHandle(address + 440) ? uint32_t(memory::OwnerLoad(address + 440, 4)) : guest::LoadU32(base, address + 440))); }
  void set_vehicle_bits(uint32_t v) const { if (auto* d = direct()) { d->vehicle = v; return; } if (memory::IsHandle(address + 440)) memory::OwnerStore(address + 440, 4, v); else guest::StoreU32(base, address + 440, v); }
  template <typename T = Vehicle>
  T vehicle() const { return T(base, vehicle_bits()); }
  void set_vehicle(const View& w) const { set_vehicle_bits(w.address); }
  void set_vehicle(uint32_t v) const { set_vehicle_bits(v); }
  // the race progress [0x826D82E8] + 2184 (RaceHudMachine::StartCountdown)
  uint32_t racers_address() const { return address + 444; }
  uint32_t racers_bits() const { if (auto* d = direct()) return d->racers; return uint32_t((memory::IsHandle(address + 444) ? uint32_t(memory::OwnerLoad(address + 444, 4)) : guest::LoadU32(base, address + 444))); }
  void set_racers_bits(uint32_t v) const { if (auto* d = direct()) { d->racers = v; return; } if (memory::IsHandle(address + 444)) memory::OwnerStore(address + 444, 4, v); else guest::StoreU32(base, address + 444, v); }
  template <typename T = RaceHudRacers>
  T racers() const { return T(base, racers_bits()); }
  void set_racers(const View& w) const { set_racers_bits(w.address); }
  void set_racers(uint32_t v) const { set_racers_bits(v); }
  // the panel (0 the racers' bars, 1 the timer, 2 to 4 the time trial's and others'; RaceHud::SetMode)
  uint32_t mode_address() const { return address + 448; }
  uint32_t mode_bits() const { if (auto* d = direct()) return d->mode; return uint32_t((memory::IsHandle(address + 448) ? uint32_t(memory::OwnerLoad(address + 448, 4)) : guest::LoadU32(base, address + 448))); }
  void set_mode_bits(uint32_t v) const { if (auto* d = direct()) { d->mode = v; return; } if (memory::IsHandle(address + 448)) memory::OwnerStore(address + 448, 4, v); else guest::StoreU32(base, address + 448, v); }
  uint32_t mode() const { if (auto* d = direct()) return d->mode; return uint32_t((memory::IsHandle(address + 448) ? uint32_t(memory::OwnerLoad(address + 448, 4)) : guest::LoadU32(base, address + 448))); }
  void set_mode(uint32_t v) const { if (auto* d = direct()) { d->mode = v; return; } if (memory::IsHandle(address + 448)) memory::OwnerStore(address + 448, 4, v); else guest::StoreU32(base, address + 448, v); }
  uint32_t racer_count_address() const { return address + 452; }
  uint32_t racer_count_bits() const { if (auto* d = direct()) return d->racer_count; return uint32_t((memory::IsHandle(address + 452) ? uint32_t(memory::OwnerLoad(address + 452, 4)) : guest::LoadU32(base, address + 452))); }
  void set_racer_count_bits(uint32_t v) const { if (auto* d = direct()) { d->racer_count = v; return; } if (memory::IsHandle(address + 452)) memory::OwnerStore(address + 452, 4, v); else guest::StoreU32(base, address + 452, v); }
  uint32_t racer_count() const { if (auto* d = direct()) return d->racer_count; return uint32_t((memory::IsHandle(address + 452) ? uint32_t(memory::OwnerLoad(address + 452, 4)) : guest::LoadU32(base, address + 452))); }
  void set_racer_count(uint32_t v) const { if (auto* d = direct()) { d->racer_count = v; return; } if (memory::IsHandle(address + 452)) memory::OwnerStore(address + 452, 4, v); else guest::StoreU32(base, address + 452, v); }
  // 1 when a racer's bar fell below its warning level this tick
  uint32_t any_bar_low_address() const { return address + 456; }
  uint32_t any_bar_low_bits() const { if (auto* d = direct()) return d->any_bar_low; return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(base, address + 456))); }
  void set_any_bar_low_bits(uint32_t v) const { if (auto* d = direct()) { d->any_bar_low = v; return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(base, address + 456, v); }
  uint32_t any_bar_low() const { if (auto* d = direct()) return d->any_bar_low; return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(base, address + 456))); }
  void set_any_bar_low(uint32_t v) const { if (auto* d = direct()) { d->any_bar_low = v; return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(base, address + 456, v); }
  // the item shown on the racers' movie (-1 none; the race progress's +7240)
  uint32_t item_shown_address() const { return address + 460; }
  uint32_t item_shown_bits() const { if (auto* d = direct()) return d->item_shown; return uint32_t((memory::IsHandle(address + 460) ? uint32_t(memory::OwnerLoad(address + 460, 4)) : guest::LoadU32(base, address + 460))); }
  void set_item_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->item_shown = v; return; } if (memory::IsHandle(address + 460)) memory::OwnerStore(address + 460, 4, v); else guest::StoreU32(base, address + 460, v); }
  uint32_t item_shown() const { if (auto* d = direct()) return d->item_shown; return uint32_t((memory::IsHandle(address + 460) ? uint32_t(memory::OwnerLoad(address + 460, 4)) : guest::LoadU32(base, address + 460))); }
  void set_item_shown(uint32_t v) const { if (auto* d = direct()) { d->item_shown = v; return; } if (memory::IsHandle(address + 460)) memory::OwnerStore(address + 460, 4, v); else guest::StoreU32(base, address + 460, v); }
  // the timer movie's named animation playing (the racers' +12)
  uint32_t timer_animation_address() const { return address + 464; }
  uint32_t timer_animation_bits() const { if (auto* d = direct()) return d->timer_animation; return uint32_t((memory::IsHandle(address + 464) ? uint32_t(memory::OwnerLoad(address + 464, 4)) : guest::LoadU32(base, address + 464))); }
  void set_timer_animation_bits(uint32_t v) const { if (auto* d = direct()) { d->timer_animation = v; return; } if (memory::IsHandle(address + 464)) memory::OwnerStore(address + 464, 4, v); else guest::StoreU32(base, address + 464, v); }
  uint32_t timer_animation() const { if (auto* d = direct()) return d->timer_animation; return uint32_t((memory::IsHandle(address + 464) ? uint32_t(memory::OwnerLoad(address + 464, 4)) : guest::LoadU32(base, address + 464))); }
  void set_timer_animation(uint32_t v) const { if (auto* d = direct()) { d->timer_animation = v; return; } if (memory::IsHandle(address + 464)) memory::OwnerStore(address + 464, 4, v); else guest::StoreU32(base, address + 464, v); }
  // the warning movie (+616) shown (the car's +6168, probably wrong way)
  uint32_t warning_shown_address() const { return address + 468; }
  uint32_t warning_shown_bits() const { if (auto* d = direct()) return d->warning_shown; return uint32_t((memory::IsHandle(address + 468) ? uint32_t(memory::OwnerLoad(address + 468, 4)) : guest::LoadU32(base, address + 468))); }
  void set_warning_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->warning_shown = v; return; } if (memory::IsHandle(address + 468)) memory::OwnerStore(address + 468, 4, v); else guest::StoreU32(base, address + 468, v); }
  uint32_t warning_shown() const { if (auto* d = direct()) return d->warning_shown; return uint32_t((memory::IsHandle(address + 468) ? uint32_t(memory::OwnerLoad(address + 468, 4)) : guest::LoadU32(base, address + 468))); }
  void set_warning_shown(uint32_t v) const { if (auto* d = direct()) { d->warning_shown = v; return; } if (memory::IsHandle(address + 468)) memory::OwnerStore(address + 468, 4, v); else guest::StoreU32(base, address + 468, v); }
  // the cockpit node 4 shown by the view (RaceHud::UpdateView)
  uint32_t view_dashboard_address() const { return address + 472; }
  uint32_t view_dashboard_bits() const { if (auto* d = direct()) return d->view_dashboard; return uint32_t((memory::IsHandle(address + 472) ? uint32_t(memory::OwnerLoad(address + 472, 4)) : guest::LoadU32(base, address + 472))); }
  void set_view_dashboard_bits(uint32_t v) const { if (auto* d = direct()) { d->view_dashboard = v; return; } if (memory::IsHandle(address + 472)) memory::OwnerStore(address + 472, 4, v); else guest::StoreU32(base, address + 472, v); }
  uint32_t view_dashboard() const { if (auto* d = direct()) return d->view_dashboard; return uint32_t((memory::IsHandle(address + 472) ? uint32_t(memory::OwnerLoad(address + 472, 4)) : guest::LoadU32(base, address + 472))); }
  void set_view_dashboard(uint32_t v) const { if (auto* d = direct()) { d->view_dashboard = v; return; } if (memory::IsHandle(address + 472)) memory::OwnerStore(address + 472, 4, v); else guest::StoreU32(base, address + 472, v); }
  // a build entry of kind 1 (or the build's +12 10): node (0, 1) shown with +8
  uint32_t part_fitted_address() const { return address + 476; }
  uint32_t part_fitted_bits() const { if (auto* d = direct()) return d->part_fitted; return uint32_t((memory::IsHandle(address + 476) ? uint32_t(memory::OwnerLoad(address + 476, 4)) : guest::LoadU32(base, address + 476))); }
  void set_part_fitted_bits(uint32_t v) const { if (auto* d = direct()) { d->part_fitted = v; return; } if (memory::IsHandle(address + 476)) memory::OwnerStore(address + 476, 4, v); else guest::StoreU32(base, address + 476, v); }
  uint32_t part_fitted() const { if (auto* d = direct()) return d->part_fitted; return uint32_t((memory::IsHandle(address + 476) ? uint32_t(memory::OwnerLoad(address + 476, 4)) : guest::LoadU32(base, address + 476))); }
  void set_part_fitted(uint32_t v) const { if (auto* d = direct()) { d->part_fitted = v; return; } if (memory::IsHandle(address + 476)) memory::OwnerStore(address + 476, 4, v); else guest::StoreU32(base, address + 476, v); }
  // the extra gauges' slots: 0 a float, 1 and 2 integers, -1 none
  uint32_t gauge_slot0_address() const { return address + 480; }
  uint32_t gauge_slot0_bits() const { if (auto* d = direct()) return d->gauge_slot0; return uint32_t((memory::IsHandle(address + 480) ? uint32_t(memory::OwnerLoad(address + 480, 4)) : guest::LoadU32(base, address + 480))); }
  void set_gauge_slot0_bits(uint32_t v) const { if (auto* d = direct()) { d->gauge_slot0 = v; return; } if (memory::IsHandle(address + 480)) memory::OwnerStore(address + 480, 4, v); else guest::StoreU32(base, address + 480, v); }
  uint32_t gauge_slot0() const { if (auto* d = direct()) return d->gauge_slot0; return uint32_t((memory::IsHandle(address + 480) ? uint32_t(memory::OwnerLoad(address + 480, 4)) : guest::LoadU32(base, address + 480))); }
  void set_gauge_slot0(uint32_t v) const { if (auto* d = direct()) { d->gauge_slot0 = v; return; } if (memory::IsHandle(address + 480)) memory::OwnerStore(address + 480, 4, v); else guest::StoreU32(base, address + 480, v); }
  uint32_t gauge_slot1_address() const { return address + 484; }
  uint32_t gauge_slot1_bits() const { if (auto* d = direct()) return d->gauge_slot1; return uint32_t((memory::IsHandle(address + 484) ? uint32_t(memory::OwnerLoad(address + 484, 4)) : guest::LoadU32(base, address + 484))); }
  void set_gauge_slot1_bits(uint32_t v) const { if (auto* d = direct()) { d->gauge_slot1 = v; return; } if (memory::IsHandle(address + 484)) memory::OwnerStore(address + 484, 4, v); else guest::StoreU32(base, address + 484, v); }
  uint32_t gauge_slot1() const { if (auto* d = direct()) return d->gauge_slot1; return uint32_t((memory::IsHandle(address + 484) ? uint32_t(memory::OwnerLoad(address + 484, 4)) : guest::LoadU32(base, address + 484))); }
  void set_gauge_slot1(uint32_t v) const { if (auto* d = direct()) { d->gauge_slot1 = v; return; } if (memory::IsHandle(address + 484)) memory::OwnerStore(address + 484, 4, v); else guest::StoreU32(base, address + 484, v); }
  uint32_t gauge_slot2_address() const { return address + 488; }
  uint32_t gauge_slot2_bits() const { if (auto* d = direct()) return d->gauge_slot2; return uint32_t((memory::IsHandle(address + 488) ? uint32_t(memory::OwnerLoad(address + 488, 4)) : guest::LoadU32(base, address + 488))); }
  void set_gauge_slot2_bits(uint32_t v) const { if (auto* d = direct()) { d->gauge_slot2 = v; return; } if (memory::IsHandle(address + 488)) memory::OwnerStore(address + 488, 4, v); else guest::StoreU32(base, address + 488, v); }
  uint32_t gauge_slot2() const { if (auto* d = direct()) return d->gauge_slot2; return uint32_t((memory::IsHandle(address + 488) ? uint32_t(memory::OwnerLoad(address + 488, 4)) : guest::LoadU32(base, address + 488))); }
  void set_gauge_slot2(uint32_t v) const { if (auto* d = direct()) { d->gauge_slot2 = v; return; } if (memory::IsHandle(address + 488)) memory::OwnerStore(address + 488, 4, v); else guest::StoreU32(base, address + 488, v); }
  // the input recording's full notice shown
  uint32_t recording_full_address() const { return address + 492; }
  uint32_t recording_full_bits() const { if (auto* d = direct()) return d->recording_full; return uint32_t((memory::IsHandle(address + 492) ? uint32_t(memory::OwnerLoad(address + 492, 4)) : guest::LoadU32(base, address + 492))); }
  void set_recording_full_bits(uint32_t v) const { if (auto* d = direct()) { d->recording_full = v; return; } if (memory::IsHandle(address + 492)) memory::OwnerStore(address + 492, 4, v); else guest::StoreU32(base, address + 492, v); }
  uint32_t recording_full() const { if (auto* d = direct()) return d->recording_full; return uint32_t((memory::IsHandle(address + 492) ? uint32_t(memory::OwnerLoad(address + 492, 4)) : guest::LoadU32(base, address + 492))); }
  void set_recording_full(uint32_t v) const { if (auto* d = direct()) { d->recording_full = v; return; } if (memory::IsHandle(address + 492)) memory::OwnerStore(address + 492, 4, v); else guest::StoreU32(base, address + 492, v); }
  uint32_t unknown_496_address() const { return address + 496; }
  uint32_t unknown_496_bits() const { if (auto* d = direct()) return d->unknown_496; return uint32_t((memory::IsHandle(address + 496) ? uint32_t(memory::OwnerLoad(address + 496, 4)) : guest::LoadU32(base, address + 496))); }
  void set_unknown_496_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_496 = v; return; } if (memory::IsHandle(address + 496)) memory::OwnerStore(address + 496, 4, v); else guest::StoreU32(base, address + 496, v); }
  uint32_t unknown_496() const { if (auto* d = direct()) return d->unknown_496; return uint32_t((memory::IsHandle(address + 496) ? uint32_t(memory::OwnerLoad(address + 496, 4)) : guest::LoadU32(base, address + 496))); }
  void set_unknown_496(uint32_t v) const { if (auto* d = direct()) { d->unknown_496 = v; return; } if (memory::IsHandle(address + 496)) memory::OwnerStore(address + 496, 4, v); else guest::StoreU32(base, address + 496, v); }
  // the splits' (mode 2)
  uint32_t unknown_572_address() const { return address + 572; }
  uint32_t unknown_572_bits() const { if (auto* d = direct()) return d->unknown_572; return uint32_t((memory::IsHandle(address + 572) ? uint32_t(memory::OwnerLoad(address + 572, 4)) : guest::LoadU32(base, address + 572))); }
  void set_unknown_572_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_572 = v; return; } if (memory::IsHandle(address + 572)) memory::OwnerStore(address + 572, 4, v); else guest::StoreU32(base, address + 572, v); }
  uint32_t unknown_572() const { if (auto* d = direct()) return d->unknown_572; return uint32_t((memory::IsHandle(address + 572) ? uint32_t(memory::OwnerLoad(address + 572, 4)) : guest::LoadU32(base, address + 572))); }
  void set_unknown_572(uint32_t v) const { if (auto* d = direct()) { d->unknown_572 = v; return; } if (memory::IsHandle(address + 572)) memory::OwnerStore(address + 572, 4, v); else guest::StoreU32(base, address + 572, v); }
  // the splits' (mode 2)
  uint32_t unknown_576_address() const { return address + 576; }
  uint32_t unknown_576_bits() const { if (auto* d = direct()) return d->unknown_576; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_unknown_576_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_576 = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  uint32_t unknown_576() const { if (auto* d = direct()) return d->unknown_576; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_unknown_576(uint32_t v) const { if (auto* d = direct()) { d->unknown_576 = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  // the splits' (mode 2)
  uint32_t unknown_580_address() const { return address + 580; }
  uint32_t unknown_580_bits() const { if (auto* d = direct()) return d->unknown_580; return uint32_t((memory::IsHandle(address + 580) ? uint32_t(memory::OwnerLoad(address + 580, 4)) : guest::LoadU32(base, address + 580))); }
  void set_unknown_580_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_580 = v; return; } if (memory::IsHandle(address + 580)) memory::OwnerStore(address + 580, 4, v); else guest::StoreU32(base, address + 580, v); }
  uint32_t unknown_580() const { if (auto* d = direct()) return d->unknown_580; return uint32_t((memory::IsHandle(address + 580) ? uint32_t(memory::OwnerLoad(address + 580, 4)) : guest::LoadU32(base, address + 580))); }
  void set_unknown_580(uint32_t v) const { if (auto* d = direct()) { d->unknown_580 = v; return; } if (memory::IsHandle(address + 580)) memory::OwnerStore(address + 580, 4, v); else guest::StoreU32(base, address + 580, v); }
  // the splits' (mode 2)
  uint32_t unknown_584_address() const { return address + 584; }
  uint32_t unknown_584_bits() const { if (auto* d = direct()) return d->unknown_584; return uint32_t((memory::IsHandle(address + 584) ? uint32_t(memory::OwnerLoad(address + 584, 4)) : guest::LoadU32(base, address + 584))); }
  void set_unknown_584_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_584 = v; return; } if (memory::IsHandle(address + 584)) memory::OwnerStore(address + 584, 4, v); else guest::StoreU32(base, address + 584, v); }
  uint32_t unknown_584() const { if (auto* d = direct()) return d->unknown_584; return uint32_t((memory::IsHandle(address + 584) ? uint32_t(memory::OwnerLoad(address + 584, 4)) : guest::LoadU32(base, address + 584))); }
  void set_unknown_584(uint32_t v) const { if (auto* d = direct()) { d->unknown_584 = v; return; } if (memory::IsHandle(address + 584)) memory::OwnerStore(address + 584, 4, v); else guest::StoreU32(base, address + 584, v); }
  // the dashboard's texture 56 swapped out for the mirror (0 none)
  uint32_t swapped_texture_address() const { return address + 588; }
  uint32_t swapped_texture_bits() const { if (auto* d = direct()) return d->swapped_texture; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(base, address + 588))); }
  void set_swapped_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->swapped_texture = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(base, address + 588, v); }
  uint32_t swapped_texture() const { if (auto* d = direct()) return d->swapped_texture; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(base, address + 588))); }
  void set_swapped_texture(uint32_t v) const { if (auto* d = direct()) { d->swapped_texture = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(base, address + 588, v); }
  // the mirror's texture record: render target 11 (CameraPasses::RenderTarget), then its width and height
  uint32_t mirror_texture_address() const { return address + 592; }
  uint32_t mirror_texture_bits() const { if (auto* d = direct()) return d->mirror_texture; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(base, address + 592))); }
  void set_mirror_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->mirror_texture = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(base, address + 592, v); }
  View mirror_texture() const { return View(base, mirror_texture_bits()); }
  void set_mirror_texture(uint32_t v) const { set_mirror_texture_bits(v); }
  uint32_t mirror_width_address() const { return address + 596; }
  uint32_t mirror_width_bits() const { if (auto* d = direct()) return d->mirror_width; return uint32_t((memory::IsHandle(address + 596) ? uint32_t(memory::OwnerLoad(address + 596, 4)) : guest::LoadU32(base, address + 596))); }
  void set_mirror_width_bits(uint32_t v) const { if (auto* d = direct()) { d->mirror_width = v; return; } if (memory::IsHandle(address + 596)) memory::OwnerStore(address + 596, 4, v); else guest::StoreU32(base, address + 596, v); }
  uint32_t mirror_width() const { if (auto* d = direct()) return d->mirror_width; return uint32_t((memory::IsHandle(address + 596) ? uint32_t(memory::OwnerLoad(address + 596, 4)) : guest::LoadU32(base, address + 596))); }
  void set_mirror_width(uint32_t v) const { if (auto* d = direct()) { d->mirror_width = v; return; } if (memory::IsHandle(address + 596)) memory::OwnerStore(address + 596, 4, v); else guest::StoreU32(base, address + 596, v); }
  uint32_t mirror_height_address() const { return address + 600; }
  uint32_t mirror_height_bits() const { if (auto* d = direct()) return d->mirror_height; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(base, address + 600))); }
  void set_mirror_height_bits(uint32_t v) const { if (auto* d = direct()) { d->mirror_height = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(base, address + 600, v); }
  uint32_t mirror_height() const { if (auto* d = direct()) return d->mirror_height; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(base, address + 600))); }
  void set_mirror_height(uint32_t v) const { if (auto* d = direct()) { d->mirror_height = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(base, address + 600, v); }
  // UI movie id 10: the racers' gauges
  uint32_t racers_movie_address() const { return address + 604; }
  uint32_t racers_movie_bits() const { if (auto* d = direct()) return d->racers_movie; return uint32_t((memory::IsHandle(address + 604) ? uint32_t(memory::OwnerLoad(address + 604, 4)) : guest::LoadU32(base, address + 604))); }
  void set_racers_movie_bits(uint32_t v) const { if (auto* d = direct()) { d->racers_movie = v; return; } if (memory::IsHandle(address + 604)) memory::OwnerStore(address + 604, 4, v); else guest::StoreU32(base, address + 604, v); }
  View racers_movie() const { return View(base, racers_movie_bits()); }
  void set_racers_movie(uint32_t v) const { set_racers_movie_bits(v); }
  // UI movie id 0: the dashboard
  uint32_t dashboard_address() const { return address + 608; }
  uint32_t dashboard_bits() const { if (auto* d = direct()) return d->dashboard; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_dashboard_bits(uint32_t v) const { if (auto* d = direct()) { d->dashboard = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
  View dashboard() const { return View(base, dashboard_bits()); }
  void set_dashboard(uint32_t v) const { set_dashboard_bits(v); }
  // UI movie id 11: the start countdown
  uint32_t countdown_address() const { return address + 612; }
  uint32_t countdown_bits() const { if (auto* d = direct()) return d->countdown; return uint32_t((memory::IsHandle(address + 612) ? uint32_t(memory::OwnerLoad(address + 612, 4)) : guest::LoadU32(base, address + 612))); }
  void set_countdown_bits(uint32_t v) const { if (auto* d = direct()) { d->countdown = v; return; } if (memory::IsHandle(address + 612)) memory::OwnerStore(address + 612, 4, v); else guest::StoreU32(base, address + 612, v); }
  View countdown() const { return View(base, countdown_bits()); }
  void set_countdown(uint32_t v) const { set_countdown_bits(v); }
  // UI movie id 3: the warning
  uint32_t warning_address() const { return address + 616; }
  uint32_t warning_bits() const { if (auto* d = direct()) return d->warning; return uint32_t((memory::IsHandle(address + 616) ? uint32_t(memory::OwnerLoad(address + 616, 4)) : guest::LoadU32(base, address + 616))); }
  void set_warning_bits(uint32_t v) const { if (auto* d = direct()) { d->warning = v; return; } if (memory::IsHandle(address + 616)) memory::OwnerStore(address + 616, 4, v); else guest::StoreU32(base, address + 616, v); }
  View warning() const { return View(base, warning_bits()); }
  void set_warning(uint32_t v) const { set_warning_bits(v); }
  // UI movie id 2 (not read)
  uint32_t movie2_address() const { return address + 620; }
  uint32_t movie2_bits() const { if (auto* d = direct()) return d->movie2; return uint32_t((memory::IsHandle(address + 620) ? uint32_t(memory::OwnerLoad(address + 620, 4)) : guest::LoadU32(base, address + 620))); }
  void set_movie2_bits(uint32_t v) const { if (auto* d = direct()) { d->movie2 = v; return; } if (memory::IsHandle(address + 620)) memory::OwnerStore(address + 620, 4, v); else guest::StoreU32(base, address + 620, v); }
  View movie2() const { return View(base, movie2_bits()); }
  void set_movie2(uint32_t v) const { set_movie2_bits(v); }
  // UI movie id 13: the race timer
  uint32_t timer_address() const { return address + 624; }
  uint32_t timer_bits() const { if (auto* d = direct()) return d->timer; return uint32_t((memory::IsHandle(address + 624) ? uint32_t(memory::OwnerLoad(address + 624, 4)) : guest::LoadU32(base, address + 624))); }
  void set_timer_bits(uint32_t v) const { if (auto* d = direct()) { d->timer = v; return; } if (memory::IsHandle(address + 624)) memory::OwnerStore(address + 624, 4, v); else guest::StoreU32(base, address + 624, v); }
  View timer() const { return View(base, timer_bits()); }
  void set_timer(uint32_t v) const { set_timer_bits(v); }
  // UI movie id 1 (not read)
  uint32_t movie1_address() const { return address + 628; }
  uint32_t movie1_bits() const { if (auto* d = direct()) return d->movie1; return uint32_t((memory::IsHandle(address + 628) ? uint32_t(memory::OwnerLoad(address + 628, 4)) : guest::LoadU32(base, address + 628))); }
  void set_movie1_bits(uint32_t v) const { if (auto* d = direct()) { d->movie1 = v; return; } if (memory::IsHandle(address + 628)) memory::OwnerStore(address + 628, 4, v); else guest::StoreU32(base, address + 628, v); }
  View movie1() const { return View(base, movie1_bits()); }
  void set_movie1(uint32_t v) const { set_movie1_bits(v); }
  // UI movie id 12: the time trial's splits
  uint32_t splits_address() const { return address + 632; }
  uint32_t splits_bits() const { if (auto* d = direct()) return d->splits; return uint32_t((memory::IsHandle(address + 632) ? uint32_t(memory::OwnerLoad(address + 632, 4)) : guest::LoadU32(base, address + 632))); }
  void set_splits_bits(uint32_t v) const { if (auto* d = direct()) { d->splits = v; return; } if (memory::IsHandle(address + 632)) memory::OwnerStore(address + 632, 4, v); else guest::StoreU32(base, address + 632, v); }
  View splits() const { return View(base, splits_bits()); }
  void set_splits(uint32_t v) const { set_splits_bits(v); }
  // UI movie id 19 (never seen in play)
  uint32_t movie19_address() const { return address + 636; }
  uint32_t movie19_bits() const { if (auto* d = direct()) return d->movie19; return uint32_t((memory::IsHandle(address + 636) ? uint32_t(memory::OwnerLoad(address + 636, 4)) : guest::LoadU32(base, address + 636))); }
  void set_movie19_bits(uint32_t v) const { if (auto* d = direct()) { d->movie19 = v; return; } if (memory::IsHandle(address + 636)) memory::OwnerStore(address + 636, 4, v); else guest::StoreU32(base, address + 636, v); }
  View movie19() const { return View(base, movie19_bits()); }
  void set_movie19(uint32_t v) const { set_movie19_bits(v); }
};

// embedded; measure: RaceHud.+444, sub_822155C0.r3.+444, sub_82214BA8.r4
struct RaceHudRacers : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  // the racers' count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // record 0's item id (-1 none); record i's at +8 + 972 i
  uint32_t item0() const { return guest::LoadU32(base, address + 8); }
  void set_item0(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t item0_address() const { return address + 8; }
  // the timer movie's named animation (-1 the timer hidden)
  uint32_t timer_animation() const { return guest::LoadU32(base, address + 12); }
  void set_timer_animation(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t timer_animation_address() const { return address + 12; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  // record 0's bar top (an int); record i's at +788 + 972 i
  uint32_t bar_max0() const { return guest::LoadU32(base, address + 788); }
  void set_bar_max0(uint32_t v) const { guest::StoreU32(base, address + 788, v); }
  uint32_t bar_max0_address() const { return address + 788; }
  // taken from the race progress's +7156 for the timer's distance
  float start_distance() const { return guest::LoadFloat(base, address + 828); }
  void set_start_distance(float v) const { guest::StoreFloat(base, address + 828, v); }
  uint32_t start_distance_address() const { return address + 828; }
  // the split (mode 4's rows at +24 + 76 x it)
  uint32_t split() const { return guest::LoadU32(base, address + 836); }
  void set_split(uint32_t v) const { guest::StoreU32(base, address + 836, v); }
  uint32_t split_address() const { return address + 836; }
  // record 1's item id
  uint32_t item1() const { return guest::LoadU32(base, address + 980); }
  void set_item1(uint32_t v) const { guest::StoreU32(base, address + 980, v); }
  uint32_t item1_address() const { return address + 980; }
  // the race time in ticks (RaceTime::FromTicks)
  uint32_t ticks() const { return guest::LoadU32(base, address + 3932); }
  void set_ticks(uint32_t v) const { guest::StoreU32(base, address + 3932, v); }
  uint32_t ticks_address() const { return address + 3932; }
};

// embedded; measure: HudBar
struct RaceHudBar : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
  // the value's most a step (the top times the float at 0x82002A60)
  float value_step() const { return guest::LoadFloat(base, address + 4); }
  void set_value_step(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t value_step_address() const { return address + 4; }
  float trail() const { return guest::LoadFloat(base, address + 8); }
  void set_trail(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t trail_address() const { return address + 8; }
  // the trail's most a step (the top times the float at 0x8200F724)
  float trail_step() const { return guest::LoadFloat(base, address + 12); }
  void set_trail_step(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t trail_step_address() const { return address + 12; }
  float min() const { return guest::LoadFloat(base, address + 16); }
  void set_min(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t min_address() const { return address + 16; }
  float max() const { return guest::LoadFloat(base, address + 20); }
  void set_max(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t max_address() const { return address + 20; }
  float target() const { return guest::LoadFloat(base, address + 24); }
  void set_target(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t target_address() const { return address + 24; }
  // set by HudBar::Step: the value at or above the trail and the target, moving up
  uint32_t rising() const { return guest::LoadU32(base, address + 28); }
  void set_rising(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t rising_address() const { return address + 28; }
  // the low bar's animation played
  uint32_t animation_played() const { return guest::LoadU32(base, address + 32); }
  void set_animation_played(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t animation_played_address() const { return address + 32; }
  // set when the racer touched something (the trail held), cleared when the value meets the trail
  uint32_t touched() const { return guest::LoadU32(base, address + 36); }
  void set_touched(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t touched_address() const { return address + 36; }
};

// embedded; measure: HudCurve::Eval.r3
struct HudCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  // (x, y) floats, 8 bytes a point (HudCurvePoint)
  uint32_t points_address() const { return address + 0; }
  float max_x() const { return guest::LoadFloat(base, address + 88); }
  void set_max_x(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t max_x_address() const { return address + 88; }
  float min_x() const { return guest::LoadFloat(base, address + 92); }
  void set_min_x(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t min_x_address() const { return address + 92; }
  // the x evaluated (clamped to the bounds)
  float input() const { return guest::LoadFloat(base, address + 96); }
  void set_input(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t input_address() const { return address + 96; }
  // the y at it (left as it was when x was clamped)
  float output() const { return guest::LoadFloat(base, address + 100); }
  void set_output(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t output_address() const { return address + 100; }
  // the points' count
  uint32_t count() const { return guest::LoadU32(base, address + 108); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t count_address() const { return address + 108; }
};

// embedded; measure: -
struct HudCurvePoint : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
};

// embedded; measure: RaceHud::SetTime.r4, sub_820FF4D0.r3, sub_820FF628.r3, sub_820FF628.ret, sub_82213968.r4
struct RaceHudPackedTime : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t packed() const { return guest::LoadU32(base, address + 0); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t packed_address() const { return address + 0; }
  // the high half: bit 15 the sign, the minutes below
  uint16_t sign_minutes() const { return guest::LoadU16(base, address + 0); }
  void set_sign_minutes(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t sign_minutes_address() const { return address + 0; }
};

// embedded; measure: sub_820FF4D0.r4
struct RaceHudSplitTimes : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the first packed time; time i at +4 i
  uint32_t times() const { return guest::LoadU32(base, address + 0); }
  void set_times(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t times_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 60); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t count_address() const { return address + 60; }
};

// embedded; measure: UiMovie::FinishNamed.ret
struct RaceHudSignNode : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  // the node's picture
  uint32_t picture() const { return guest::LoadU32(base, address + 108); }
  void set_picture(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t picture_address() const { return address + 108; }
};

// embedded; measure: RaceScreens@+656
#pragma pack(push, 1)
struct RaceHudAt656Data {
  uint8_t pad_0[328] = {};
  uint32_t unknown_328 = 0;  // +328
  uint8_t pad_332[4] = {};
  uint32_t unknown_336 = 0;  // +336
  uint32_t unknown_340 = 0;  // +340
  uint32_t unknown_344 = 0;  // +344
};
#pragma pack(pop)
static_assert(sizeof(RaceHudAt656Data) == 348);
inline constexpr memory::LayoutField kRaceHudAt656Fields[] = {{328, 4}, {336, 4}, {340, 4}, {344, 4}};
inline constexpr memory::Layout kRaceHudAt656Layout = {kRaceHudAt656Fields, 4};

struct RaceHudAt656 : View {
  using View::View;
  static constexpr uint32_t kSize = 348;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceHudAt656Data* data() const { return static_cast<RaceHudAt656Data*>(memory::HandleObject(address)); }
  static RaceHudAt656 New(uint8_t* base) {
    return RaceHudAt656(base, memory::MakeHandle(new RaceHudAt656Data(), sizeof(RaceHudAt656Data), "RaceHudAt656", &kRaceHudAt656Layout));
  }
  template <uint32_t Stride = kSize>
  static RaceHudAt656 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceHudAt656Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceHudAt656Data*>(::operator new(sizeof(RaceHudAt656Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceHudAt656Data();
    return RaceHudAt656(base, memory::MakeHandle(p, sizeof(RaceHudAt656Data) * count, "RaceHudAt656", &kRaceHudAt656Layout, sizeof(RaceHudAt656Data)));
  }
  RaceHudAt656 at(uint32_t i) const { return RaceHudAt656(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceHudAt656Data* direct() const { return static_cast<RaceHudAt656Data*>(memory::HandleOwn(address, &kRaceHudAt656Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceHudAt656Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceHudAt656Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_328_address() const { return address + 328; }
  uint32_t unknown_328_bits() const { if (auto* d = direct()) return d->unknown_328; return uint32_t((memory::IsHandle(address + 328) ? uint32_t(memory::OwnerLoad(address + 328, 4)) : guest::LoadU32(base, address + 328))); }
  void set_unknown_328_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_328 = v; return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 4, v); else guest::StoreU32(base, address + 328, v); }
  uint32_t unknown_328() const { if (auto* d = direct()) return d->unknown_328; return uint32_t((memory::IsHandle(address + 328) ? uint32_t(memory::OwnerLoad(address + 328, 4)) : guest::LoadU32(base, address + 328))); }
  void set_unknown_328(uint32_t v) const { if (auto* d = direct()) { d->unknown_328 = v; return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 4, v); else guest::StoreU32(base, address + 328, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  uint32_t unknown_336_bits() const { if (auto* d = direct()) return d->unknown_336; return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(base, address + 336))); }
  void set_unknown_336_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_336 = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(base, address + 336, v); }
  uint32_t unknown_336() const { if (auto* d = direct()) return d->unknown_336; return uint32_t((memory::IsHandle(address + 336) ? uint32_t(memory::OwnerLoad(address + 336, 4)) : guest::LoadU32(base, address + 336))); }
  void set_unknown_336(uint32_t v) const { if (auto* d = direct()) { d->unknown_336 = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 4, v); else guest::StoreU32(base, address + 336, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  uint32_t unknown_340_bits() const { if (auto* d = direct()) return d->unknown_340; return uint32_t((memory::IsHandle(address + 340) ? uint32_t(memory::OwnerLoad(address + 340, 4)) : guest::LoadU32(base, address + 340))); }
  void set_unknown_340_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_340 = v; return; } if (memory::IsHandle(address + 340)) memory::OwnerStore(address + 340, 4, v); else guest::StoreU32(base, address + 340, v); }
  uint32_t unknown_340() const { if (auto* d = direct()) return d->unknown_340; return uint32_t((memory::IsHandle(address + 340) ? uint32_t(memory::OwnerLoad(address + 340, 4)) : guest::LoadU32(base, address + 340))); }
  void set_unknown_340(uint32_t v) const { if (auto* d = direct()) { d->unknown_340 = v; return; } if (memory::IsHandle(address + 340)) memory::OwnerStore(address + 340, 4, v); else guest::StoreU32(base, address + 340, v); }
  uint32_t unknown_344_address() const { return address + 344; }
  uint32_t unknown_344_bits() const { if (auto* d = direct()) return d->unknown_344; return uint32_t((memory::IsHandle(address + 344) ? uint32_t(memory::OwnerLoad(address + 344, 4)) : guest::LoadU32(base, address + 344))); }
  void set_unknown_344_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_344 = v; return; } if (memory::IsHandle(address + 344)) memory::OwnerStore(address + 344, 4, v); else guest::StoreU32(base, address + 344, v); }
  uint32_t unknown_344() const { if (auto* d = direct()) return d->unknown_344; return uint32_t((memory::IsHandle(address + 344) ? uint32_t(memory::OwnerLoad(address + 344, 4)) : guest::LoadU32(base, address + 344))); }
  void set_unknown_344(uint32_t v) const { if (auto* d = direct()) { d->unknown_344 = v; return; } if (memory::IsHandle(address + 344)) memory::OwnerStore(address + 344, 4, v); else guest::StoreU32(base, address + 344, v); }
};

// embedded; measure: RaceHudMachine.+9608@+16
#pragma pack(push, 1)
struct RaceHudAt16Data {
  uint8_t pad_0[444] = {};
  uint32_t unknown_444 = 0;  // +444
  uint8_t pad_448[8] = {};
  uint32_t unknown_456 = 0;  // +456
  uint8_t pad_460[144] = {};
  uint32_t unknown_604 = 0;  // +604
  uint32_t unknown_608 = 0;  // +608
  uint8_t pad_612[12] = {};
  uint32_t unknown_624 = 0;  // +624
  uint8_t pad_628[4] = {};
  uint32_t unknown_632 = 0;  // +632
};
#pragma pack(pop)
static_assert(sizeof(RaceHudAt16Data) == 636);
inline constexpr memory::LayoutField kRaceHudAt16Fields[] = {{444, 4}, {456, 4}, {604, 4}, {608, 4}, {624, 4}, {632, 4}};
inline constexpr memory::Layout kRaceHudAt16Layout = {kRaceHudAt16Fields, 6};

struct RaceHudAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 636;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceHudAt16Data* data() const { return static_cast<RaceHudAt16Data*>(memory::HandleObject(address)); }
  static RaceHudAt16 New(uint8_t* base) {
    return RaceHudAt16(base, memory::MakeHandle(new RaceHudAt16Data(), sizeof(RaceHudAt16Data), "RaceHudAt16", &kRaceHudAt16Layout));
  }
  template <uint32_t Stride = kSize>
  static RaceHudAt16 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceHudAt16Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceHudAt16Data*>(::operator new(sizeof(RaceHudAt16Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceHudAt16Data();
    return RaceHudAt16(base, memory::MakeHandle(p, sizeof(RaceHudAt16Data) * count, "RaceHudAt16", &kRaceHudAt16Layout, sizeof(RaceHudAt16Data)));
  }
  RaceHudAt16 at(uint32_t i) const { return RaceHudAt16(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceHudAt16Data* direct() const { return static_cast<RaceHudAt16Data*>(memory::HandleOwn(address, &kRaceHudAt16Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceHudAt16Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceHudAt16Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_444_address() const { return address + 444; }
  uint32_t unknown_444_bits() const { if (auto* d = direct()) return d->unknown_444; return uint32_t((memory::IsHandle(address + 444) ? uint32_t(memory::OwnerLoad(address + 444, 4)) : guest::LoadU32(base, address + 444))); }
  void set_unknown_444_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_444 = v; return; } if (memory::IsHandle(address + 444)) memory::OwnerStore(address + 444, 4, v); else guest::StoreU32(base, address + 444, v); }
  uint32_t unknown_444() const { if (auto* d = direct()) return d->unknown_444; return uint32_t((memory::IsHandle(address + 444) ? uint32_t(memory::OwnerLoad(address + 444, 4)) : guest::LoadU32(base, address + 444))); }
  void set_unknown_444(uint32_t v) const { if (auto* d = direct()) { d->unknown_444 = v; return; } if (memory::IsHandle(address + 444)) memory::OwnerStore(address + 444, 4, v); else guest::StoreU32(base, address + 444, v); }
  uint32_t unknown_456_address() const { return address + 456; }
  uint32_t unknown_456_bits() const { if (auto* d = direct()) return d->unknown_456; return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(base, address + 456))); }
  void set_unknown_456_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_456 = v; return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(base, address + 456, v); }
  uint32_t unknown_456() const { if (auto* d = direct()) return d->unknown_456; return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(base, address + 456))); }
  void set_unknown_456(uint32_t v) const { if (auto* d = direct()) { d->unknown_456 = v; return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(base, address + 456, v); }
  uint32_t unknown_604_address() const { return address + 604; }
  uint32_t unknown_604_bits() const { if (auto* d = direct()) return d->unknown_604; return uint32_t((memory::IsHandle(address + 604) ? uint32_t(memory::OwnerLoad(address + 604, 4)) : guest::LoadU32(base, address + 604))); }
  void set_unknown_604_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_604 = v; return; } if (memory::IsHandle(address + 604)) memory::OwnerStore(address + 604, 4, v); else guest::StoreU32(base, address + 604, v); }
  uint32_t unknown_604() const { if (auto* d = direct()) return d->unknown_604; return uint32_t((memory::IsHandle(address + 604) ? uint32_t(memory::OwnerLoad(address + 604, 4)) : guest::LoadU32(base, address + 604))); }
  void set_unknown_604(uint32_t v) const { if (auto* d = direct()) { d->unknown_604 = v; return; } if (memory::IsHandle(address + 604)) memory::OwnerStore(address + 604, 4, v); else guest::StoreU32(base, address + 604, v); }
  uint32_t unknown_608_address() const { return address + 608; }
  uint32_t unknown_608_bits() const { if (auto* d = direct()) return d->unknown_608; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_unknown_608_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_608 = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
  uint32_t unknown_608() const { if (auto* d = direct()) return d->unknown_608; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_unknown_608(uint32_t v) const { if (auto* d = direct()) { d->unknown_608 = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
  uint32_t unknown_624_address() const { return address + 624; }
  uint32_t unknown_624_bits() const { if (auto* d = direct()) return d->unknown_624; return uint32_t((memory::IsHandle(address + 624) ? uint32_t(memory::OwnerLoad(address + 624, 4)) : guest::LoadU32(base, address + 624))); }
  void set_unknown_624_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_624 = v; return; } if (memory::IsHandle(address + 624)) memory::OwnerStore(address + 624, 4, v); else guest::StoreU32(base, address + 624, v); }
  uint32_t unknown_624() const { if (auto* d = direct()) return d->unknown_624; return uint32_t((memory::IsHandle(address + 624) ? uint32_t(memory::OwnerLoad(address + 624, 4)) : guest::LoadU32(base, address + 624))); }
  void set_unknown_624(uint32_t v) const { if (auto* d = direct()) { d->unknown_624 = v; return; } if (memory::IsHandle(address + 624)) memory::OwnerStore(address + 624, 4, v); else guest::StoreU32(base, address + 624, v); }
  uint32_t unknown_632_address() const { return address + 632; }
  uint32_t unknown_632_bits() const { if (auto* d = direct()) return d->unknown_632; return uint32_t((memory::IsHandle(address + 632) ? uint32_t(memory::OwnerLoad(address + 632, 4)) : guest::LoadU32(base, address + 632))); }
  void set_unknown_632_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_632 = v; return; } if (memory::IsHandle(address + 632)) memory::OwnerStore(address + 632, 4, v); else guest::StoreU32(base, address + 632, v); }
  uint32_t unknown_632() const { if (auto* d = direct()) return d->unknown_632; return uint32_t((memory::IsHandle(address + 632) ? uint32_t(memory::OwnerLoad(address + 632, 4)) : guest::LoadU32(base, address + 632))); }
  void set_unknown_632(uint32_t v) const { if (auto* d = direct()) { d->unknown_632 = v; return; } if (memory::IsHandle(address + 632)) memory::OwnerStore(address + 632, 4, v); else guest::StoreU32(base, address + 632, v); }
};

// embedded; measure: RaceScreens::~RaceScreens.r3@+16
#pragma pack(push, 1)
struct RaceHudAt16_2Data {
  uint8_t pad_0[588] = {};
  uint32_t unknown_588 = 0;  // +588
  uint8_t pad_592[16] = {};
  uint32_t unknown_608 = 0;  // +608
};
#pragma pack(pop)
static_assert(sizeof(RaceHudAt16_2Data) == 612);
inline constexpr memory::LayoutField kRaceHudAt16_2Fields[] = {{588, 4}, {608, 4}};
inline constexpr memory::Layout kRaceHudAt16_2Layout = {kRaceHudAt16_2Fields, 2};

struct RaceHudAt16_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 612;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceHudAt16_2Data* data() const { return static_cast<RaceHudAt16_2Data*>(memory::HandleObject(address)); }
  static RaceHudAt16_2 New(uint8_t* base) {
    return RaceHudAt16_2(base, memory::MakeHandle(new RaceHudAt16_2Data(), sizeof(RaceHudAt16_2Data), "RaceHudAt16_2", &kRaceHudAt16_2Layout));
  }
  template <uint32_t Stride = kSize>
  static RaceHudAt16_2 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceHudAt16_2Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceHudAt16_2Data*>(::operator new(sizeof(RaceHudAt16_2Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceHudAt16_2Data();
    return RaceHudAt16_2(base, memory::MakeHandle(p, sizeof(RaceHudAt16_2Data) * count, "RaceHudAt16_2", &kRaceHudAt16_2Layout, sizeof(RaceHudAt16_2Data)));
  }
  RaceHudAt16_2 at(uint32_t i) const { return RaceHudAt16_2(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceHudAt16_2Data* direct() const { return static_cast<RaceHudAt16_2Data*>(memory::HandleOwn(address, &kRaceHudAt16_2Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceHudAt16_2Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceHudAt16_2Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_588_address() const { return address + 588; }
  uint32_t unknown_588_bits() const { if (auto* d = direct()) return d->unknown_588; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(base, address + 588))); }
  void set_unknown_588_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_588 = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(base, address + 588, v); }
  uint32_t unknown_588() const { if (auto* d = direct()) return d->unknown_588; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(base, address + 588))); }
  void set_unknown_588(uint32_t v) const { if (auto* d = direct()) { d->unknown_588 = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(base, address + 588, v); }
  uint32_t unknown_608_address() const { return address + 608; }
  uint32_t unknown_608_bits() const { if (auto* d = direct()) return d->unknown_608; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_unknown_608_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_608 = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
  uint32_t unknown_608() const { if (auto* d = direct()) return d->unknown_608; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_unknown_608(uint32_t v) const { if (auto* d = direct()) { d->unknown_608 = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
};

// embedded; measure: RaceScreens@+16
#pragma pack(push, 1)
struct RaceHudAt16_3Data {
  uint8_t pad_0[8] = {};
  uint32_t unknown_8 = 0;  // +8
  uint32_t unknown_12 = 0;  // +12
  uint32_t unknown_16 = 0;  // +16
  uint8_t pad_20[100] = {};
  float unknown_120 = 0;  // +120
  uint8_t pad_124[108] = {};
  float unknown_232 = 0;  // +232
  uint8_t pad_236[168] = {};
  float unknown_404 = 0;  // +404
  float unknown_408 = 0;  // +408
  float unknown_412 = 0;  // +412
  uint32_t unknown_416 = 0;  // +416
  uint32_t unknown_420 = 0;  // +420
  uint32_t unknown_424 = 0;  // +424
  uint32_t unknown_428 = 0;  // +428
  uint32_t unknown_432 = 0;  // +432
  uint32_t unknown_436 = 0;  // +436
  uint32_t unknown_440 = 0;  // +440
  uint32_t unknown_444 = 0;  // +444
  uint32_t unknown_448 = 0;  // +448
  uint32_t unknown_452 = 0;  // +452
  uint32_t unknown_456 = 0;  // +456
  uint32_t unknown_460 = 0;  // +460
  uint32_t unknown_464 = 0;  // +464
  uint32_t unknown_468 = 0;  // +468
  uint32_t unknown_472 = 0;  // +472
  uint32_t unknown_476 = 0;  // +476
  uint32_t unknown_480 = 0;  // +480
  uint8_t pad_484[4] = {};
  uint32_t unknown_488 = 0;  // +488
  uint32_t unknown_492 = 0;  // +492
  uint32_t unknown_496 = 0;  // +496
  uint8_t pad_500[72] = {};
  uint32_t unknown_572 = 0;  // +572
  uint32_t unknown_576 = 0;  // +576
  uint32_t unknown_580 = 0;  // +580
  uint32_t unknown_584 = 0;  // +584
  uint32_t unknown_588 = 0;  // +588
  uint32_t unknown_592 = 0;  // +592
  uint32_t unknown_596 = 0;  // +596
  uint32_t unknown_600 = 0;  // +600
  uint32_t unknown_604 = 0;  // +604
  uint32_t unknown_608 = 0;  // +608
  uint32_t unknown_612 = 0;  // +612
  uint32_t unknown_616 = 0;  // +616
  uint32_t unknown_620 = 0;  // +620
  uint32_t unknown_624 = 0;  // +624
  uint32_t unknown_628 = 0;  // +628
  uint32_t unknown_632 = 0;  // +632
  uint32_t unknown_636 = 0;  // +636
};
#pragma pack(pop)
static_assert(sizeof(RaceHudAt16_3Data) == 640);
inline constexpr memory::LayoutField kRaceHudAt16_3Fields[] = {{8, 4}, {12, 4}, {16, 4}, {120, 4}, {232, 4}, {404, 4}, {408, 4}, {412, 4}, {416, 4}, {420, 4}, {424, 4}, {428, 4}, {432, 4}, {436, 4}, {440, 4}, {444, 4}, {448, 4}, {452, 4}, {456, 4}, {460, 4}, {464, 4}, {468, 4}, {472, 4}, {476, 4}, {480, 4}, {488, 4}, {492, 4}, {496, 4}, {572, 4}, {576, 4}, {580, 4}, {584, 4}, {588, 4}, {592, 4}, {596, 4}, {600, 4}, {604, 4}, {608, 4}, {612, 4}, {616, 4}, {620, 4}, {624, 4}, {628, 4}, {632, 4}, {636, 4}};
inline constexpr memory::Layout kRaceHudAt16_3Layout = {kRaceHudAt16_3Fields, 45};

struct RaceHudAt16_3 : View {
  using View::View;
  static constexpr uint32_t kSize = 640;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceHudAt16_3Data* data() const { return static_cast<RaceHudAt16_3Data*>(memory::HandleObject(address)); }
  static RaceHudAt16_3 New(uint8_t* base) {
    return RaceHudAt16_3(base, memory::MakeHandle(new RaceHudAt16_3Data(), sizeof(RaceHudAt16_3Data), "RaceHudAt16_3", &kRaceHudAt16_3Layout));
  }
  template <uint32_t Stride = kSize>
  static RaceHudAt16_3 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceHudAt16_3Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceHudAt16_3Data*>(::operator new(sizeof(RaceHudAt16_3Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceHudAt16_3Data();
    return RaceHudAt16_3(base, memory::MakeHandle(p, sizeof(RaceHudAt16_3Data) * count, "RaceHudAt16_3", &kRaceHudAt16_3Layout, sizeof(RaceHudAt16_3Data)));
  }
  RaceHudAt16_3 at(uint32_t i) const { return RaceHudAt16_3(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceHudAt16_3Data* direct() const { return static_cast<RaceHudAt16_3Data*>(memory::HandleOwn(address, &kRaceHudAt16_3Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceHudAt16_3Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceHudAt16_3Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12_bits() const { if (auto* d = direct()) return d->unknown_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_unknown_12_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12() const { if (auto* d = direct()) return d->unknown_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_unknown_12(uint32_t v) const { if (auto* d = direct()) { d->unknown_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_16_bits() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_unknown_16_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16() const { if (auto* d = direct()) return d->unknown_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_unknown_16(uint32_t v) const { if (auto* d = direct()) { d->unknown_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_120_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_120); return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(base, address + 120))); }
  void set_unknown_120_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_120 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(base, address + 120, v); }
  float unknown_120() const { return std::bit_cast<float>(unknown_120_bits()); }
  void set_unknown_120(float v) const { set_unknown_120_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_232_address() const { return address + 232; }
  uint32_t unknown_232_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_232); return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(base, address + 232))); }
  void set_unknown_232_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_232 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(base, address + 232, v); }
  float unknown_232() const { return std::bit_cast<float>(unknown_232_bits()); }
  void set_unknown_232(float v) const { set_unknown_232_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_404_address() const { return address + 404; }
  uint32_t unknown_404_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_404); return uint32_t((memory::IsHandle(address + 404) ? uint32_t(memory::OwnerLoad(address + 404, 4)) : guest::LoadU32(base, address + 404))); }
  void set_unknown_404_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_404 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 4, v); else guest::StoreU32(base, address + 404, v); }
  float unknown_404() const { return std::bit_cast<float>(unknown_404_bits()); }
  void set_unknown_404(float v) const { set_unknown_404_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_408_address() const { return address + 408; }
  uint32_t unknown_408_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_408); return uint32_t((memory::IsHandle(address + 408) ? uint32_t(memory::OwnerLoad(address + 408, 4)) : guest::LoadU32(base, address + 408))); }
  void set_unknown_408_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_408 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 408)) memory::OwnerStore(address + 408, 4, v); else guest::StoreU32(base, address + 408, v); }
  float unknown_408() const { return std::bit_cast<float>(unknown_408_bits()); }
  void set_unknown_408(float v) const { set_unknown_408_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_412_address() const { return address + 412; }
  uint32_t unknown_412_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_412); return uint32_t((memory::IsHandle(address + 412) ? uint32_t(memory::OwnerLoad(address + 412, 4)) : guest::LoadU32(base, address + 412))); }
  void set_unknown_412_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_412 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 412)) memory::OwnerStore(address + 412, 4, v); else guest::StoreU32(base, address + 412, v); }
  float unknown_412() const { return std::bit_cast<float>(unknown_412_bits()); }
  void set_unknown_412(float v) const { set_unknown_412_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_416_address() const { return address + 416; }
  uint32_t unknown_416_bits() const { if (auto* d = direct()) return d->unknown_416; return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(base, address + 416))); }
  void set_unknown_416_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_416 = v; return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(base, address + 416, v); }
  uint32_t unknown_416() const { if (auto* d = direct()) return d->unknown_416; return uint32_t((memory::IsHandle(address + 416) ? uint32_t(memory::OwnerLoad(address + 416, 4)) : guest::LoadU32(base, address + 416))); }
  void set_unknown_416(uint32_t v) const { if (auto* d = direct()) { d->unknown_416 = v; return; } if (memory::IsHandle(address + 416)) memory::OwnerStore(address + 416, 4, v); else guest::StoreU32(base, address + 416, v); }
  uint32_t unknown_420_address() const { return address + 420; }
  uint32_t unknown_420_bits() const { if (auto* d = direct()) return d->unknown_420; return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(base, address + 420))); }
  void set_unknown_420_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_420 = v; return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(base, address + 420, v); }
  uint32_t unknown_420() const { if (auto* d = direct()) return d->unknown_420; return uint32_t((memory::IsHandle(address + 420) ? uint32_t(memory::OwnerLoad(address + 420, 4)) : guest::LoadU32(base, address + 420))); }
  void set_unknown_420(uint32_t v) const { if (auto* d = direct()) { d->unknown_420 = v; return; } if (memory::IsHandle(address + 420)) memory::OwnerStore(address + 420, 4, v); else guest::StoreU32(base, address + 420, v); }
  uint32_t unknown_424_address() const { return address + 424; }
  uint32_t unknown_424_bits() const { if (auto* d = direct()) return d->unknown_424; return uint32_t((memory::IsHandle(address + 424) ? uint32_t(memory::OwnerLoad(address + 424, 4)) : guest::LoadU32(base, address + 424))); }
  void set_unknown_424_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_424 = v; return; } if (memory::IsHandle(address + 424)) memory::OwnerStore(address + 424, 4, v); else guest::StoreU32(base, address + 424, v); }
  uint32_t unknown_424() const { if (auto* d = direct()) return d->unknown_424; return uint32_t((memory::IsHandle(address + 424) ? uint32_t(memory::OwnerLoad(address + 424, 4)) : guest::LoadU32(base, address + 424))); }
  void set_unknown_424(uint32_t v) const { if (auto* d = direct()) { d->unknown_424 = v; return; } if (memory::IsHandle(address + 424)) memory::OwnerStore(address + 424, 4, v); else guest::StoreU32(base, address + 424, v); }
  uint32_t unknown_428_address() const { return address + 428; }
  uint32_t unknown_428_bits() const { if (auto* d = direct()) return d->unknown_428; return uint32_t((memory::IsHandle(address + 428) ? uint32_t(memory::OwnerLoad(address + 428, 4)) : guest::LoadU32(base, address + 428))); }
  void set_unknown_428_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_428 = v; return; } if (memory::IsHandle(address + 428)) memory::OwnerStore(address + 428, 4, v); else guest::StoreU32(base, address + 428, v); }
  uint32_t unknown_428() const { if (auto* d = direct()) return d->unknown_428; return uint32_t((memory::IsHandle(address + 428) ? uint32_t(memory::OwnerLoad(address + 428, 4)) : guest::LoadU32(base, address + 428))); }
  void set_unknown_428(uint32_t v) const { if (auto* d = direct()) { d->unknown_428 = v; return; } if (memory::IsHandle(address + 428)) memory::OwnerStore(address + 428, 4, v); else guest::StoreU32(base, address + 428, v); }
  uint32_t unknown_432_address() const { return address + 432; }
  uint32_t unknown_432_bits() const { if (auto* d = direct()) return d->unknown_432; return uint32_t((memory::IsHandle(address + 432) ? uint32_t(memory::OwnerLoad(address + 432, 4)) : guest::LoadU32(base, address + 432))); }
  void set_unknown_432_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_432 = v; return; } if (memory::IsHandle(address + 432)) memory::OwnerStore(address + 432, 4, v); else guest::StoreU32(base, address + 432, v); }
  uint32_t unknown_432() const { if (auto* d = direct()) return d->unknown_432; return uint32_t((memory::IsHandle(address + 432) ? uint32_t(memory::OwnerLoad(address + 432, 4)) : guest::LoadU32(base, address + 432))); }
  void set_unknown_432(uint32_t v) const { if (auto* d = direct()) { d->unknown_432 = v; return; } if (memory::IsHandle(address + 432)) memory::OwnerStore(address + 432, 4, v); else guest::StoreU32(base, address + 432, v); }
  uint32_t unknown_436_address() const { return address + 436; }
  uint32_t unknown_436_bits() const { if (auto* d = direct()) return d->unknown_436; return uint32_t((memory::IsHandle(address + 436) ? uint32_t(memory::OwnerLoad(address + 436, 4)) : guest::LoadU32(base, address + 436))); }
  void set_unknown_436_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_436 = v; return; } if (memory::IsHandle(address + 436)) memory::OwnerStore(address + 436, 4, v); else guest::StoreU32(base, address + 436, v); }
  uint32_t unknown_436() const { if (auto* d = direct()) return d->unknown_436; return uint32_t((memory::IsHandle(address + 436) ? uint32_t(memory::OwnerLoad(address + 436, 4)) : guest::LoadU32(base, address + 436))); }
  void set_unknown_436(uint32_t v) const { if (auto* d = direct()) { d->unknown_436 = v; return; } if (memory::IsHandle(address + 436)) memory::OwnerStore(address + 436, 4, v); else guest::StoreU32(base, address + 436, v); }
  uint32_t unknown_440_address() const { return address + 440; }
  uint32_t unknown_440_bits() const { if (auto* d = direct()) return d->unknown_440; return uint32_t((memory::IsHandle(address + 440) ? uint32_t(memory::OwnerLoad(address + 440, 4)) : guest::LoadU32(base, address + 440))); }
  void set_unknown_440_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_440 = v; return; } if (memory::IsHandle(address + 440)) memory::OwnerStore(address + 440, 4, v); else guest::StoreU32(base, address + 440, v); }
  uint32_t unknown_440() const { if (auto* d = direct()) return d->unknown_440; return uint32_t((memory::IsHandle(address + 440) ? uint32_t(memory::OwnerLoad(address + 440, 4)) : guest::LoadU32(base, address + 440))); }
  void set_unknown_440(uint32_t v) const { if (auto* d = direct()) { d->unknown_440 = v; return; } if (memory::IsHandle(address + 440)) memory::OwnerStore(address + 440, 4, v); else guest::StoreU32(base, address + 440, v); }
  uint32_t unknown_444_address() const { return address + 444; }
  uint32_t unknown_444_bits() const { if (auto* d = direct()) return d->unknown_444; return uint32_t((memory::IsHandle(address + 444) ? uint32_t(memory::OwnerLoad(address + 444, 4)) : guest::LoadU32(base, address + 444))); }
  void set_unknown_444_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_444 = v; return; } if (memory::IsHandle(address + 444)) memory::OwnerStore(address + 444, 4, v); else guest::StoreU32(base, address + 444, v); }
  uint32_t unknown_444() const { if (auto* d = direct()) return d->unknown_444; return uint32_t((memory::IsHandle(address + 444) ? uint32_t(memory::OwnerLoad(address + 444, 4)) : guest::LoadU32(base, address + 444))); }
  void set_unknown_444(uint32_t v) const { if (auto* d = direct()) { d->unknown_444 = v; return; } if (memory::IsHandle(address + 444)) memory::OwnerStore(address + 444, 4, v); else guest::StoreU32(base, address + 444, v); }
  uint32_t unknown_448_address() const { return address + 448; }
  uint32_t unknown_448_bits() const { if (auto* d = direct()) return d->unknown_448; return uint32_t((memory::IsHandle(address + 448) ? uint32_t(memory::OwnerLoad(address + 448, 4)) : guest::LoadU32(base, address + 448))); }
  void set_unknown_448_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_448 = v; return; } if (memory::IsHandle(address + 448)) memory::OwnerStore(address + 448, 4, v); else guest::StoreU32(base, address + 448, v); }
  uint32_t unknown_448() const { if (auto* d = direct()) return d->unknown_448; return uint32_t((memory::IsHandle(address + 448) ? uint32_t(memory::OwnerLoad(address + 448, 4)) : guest::LoadU32(base, address + 448))); }
  void set_unknown_448(uint32_t v) const { if (auto* d = direct()) { d->unknown_448 = v; return; } if (memory::IsHandle(address + 448)) memory::OwnerStore(address + 448, 4, v); else guest::StoreU32(base, address + 448, v); }
  uint32_t unknown_452_address() const { return address + 452; }
  uint32_t unknown_452_bits() const { if (auto* d = direct()) return d->unknown_452; return uint32_t((memory::IsHandle(address + 452) ? uint32_t(memory::OwnerLoad(address + 452, 4)) : guest::LoadU32(base, address + 452))); }
  void set_unknown_452_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_452 = v; return; } if (memory::IsHandle(address + 452)) memory::OwnerStore(address + 452, 4, v); else guest::StoreU32(base, address + 452, v); }
  uint32_t unknown_452() const { if (auto* d = direct()) return d->unknown_452; return uint32_t((memory::IsHandle(address + 452) ? uint32_t(memory::OwnerLoad(address + 452, 4)) : guest::LoadU32(base, address + 452))); }
  void set_unknown_452(uint32_t v) const { if (auto* d = direct()) { d->unknown_452 = v; return; } if (memory::IsHandle(address + 452)) memory::OwnerStore(address + 452, 4, v); else guest::StoreU32(base, address + 452, v); }
  uint32_t unknown_456_address() const { return address + 456; }
  uint32_t unknown_456_bits() const { if (auto* d = direct()) return d->unknown_456; return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(base, address + 456))); }
  void set_unknown_456_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_456 = v; return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(base, address + 456, v); }
  uint32_t unknown_456() const { if (auto* d = direct()) return d->unknown_456; return uint32_t((memory::IsHandle(address + 456) ? uint32_t(memory::OwnerLoad(address + 456, 4)) : guest::LoadU32(base, address + 456))); }
  void set_unknown_456(uint32_t v) const { if (auto* d = direct()) { d->unknown_456 = v; return; } if (memory::IsHandle(address + 456)) memory::OwnerStore(address + 456, 4, v); else guest::StoreU32(base, address + 456, v); }
  uint32_t unknown_460_address() const { return address + 460; }
  uint32_t unknown_460_bits() const { if (auto* d = direct()) return d->unknown_460; return uint32_t((memory::IsHandle(address + 460) ? uint32_t(memory::OwnerLoad(address + 460, 4)) : guest::LoadU32(base, address + 460))); }
  void set_unknown_460_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_460 = v; return; } if (memory::IsHandle(address + 460)) memory::OwnerStore(address + 460, 4, v); else guest::StoreU32(base, address + 460, v); }
  uint32_t unknown_460() const { if (auto* d = direct()) return d->unknown_460; return uint32_t((memory::IsHandle(address + 460) ? uint32_t(memory::OwnerLoad(address + 460, 4)) : guest::LoadU32(base, address + 460))); }
  void set_unknown_460(uint32_t v) const { if (auto* d = direct()) { d->unknown_460 = v; return; } if (memory::IsHandle(address + 460)) memory::OwnerStore(address + 460, 4, v); else guest::StoreU32(base, address + 460, v); }
  uint32_t unknown_464_address() const { return address + 464; }
  uint32_t unknown_464_bits() const { if (auto* d = direct()) return d->unknown_464; return uint32_t((memory::IsHandle(address + 464) ? uint32_t(memory::OwnerLoad(address + 464, 4)) : guest::LoadU32(base, address + 464))); }
  void set_unknown_464_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_464 = v; return; } if (memory::IsHandle(address + 464)) memory::OwnerStore(address + 464, 4, v); else guest::StoreU32(base, address + 464, v); }
  uint32_t unknown_464() const { if (auto* d = direct()) return d->unknown_464; return uint32_t((memory::IsHandle(address + 464) ? uint32_t(memory::OwnerLoad(address + 464, 4)) : guest::LoadU32(base, address + 464))); }
  void set_unknown_464(uint32_t v) const { if (auto* d = direct()) { d->unknown_464 = v; return; } if (memory::IsHandle(address + 464)) memory::OwnerStore(address + 464, 4, v); else guest::StoreU32(base, address + 464, v); }
  uint32_t unknown_468_address() const { return address + 468; }
  uint32_t unknown_468_bits() const { if (auto* d = direct()) return d->unknown_468; return uint32_t((memory::IsHandle(address + 468) ? uint32_t(memory::OwnerLoad(address + 468, 4)) : guest::LoadU32(base, address + 468))); }
  void set_unknown_468_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_468 = v; return; } if (memory::IsHandle(address + 468)) memory::OwnerStore(address + 468, 4, v); else guest::StoreU32(base, address + 468, v); }
  uint32_t unknown_468() const { if (auto* d = direct()) return d->unknown_468; return uint32_t((memory::IsHandle(address + 468) ? uint32_t(memory::OwnerLoad(address + 468, 4)) : guest::LoadU32(base, address + 468))); }
  void set_unknown_468(uint32_t v) const { if (auto* d = direct()) { d->unknown_468 = v; return; } if (memory::IsHandle(address + 468)) memory::OwnerStore(address + 468, 4, v); else guest::StoreU32(base, address + 468, v); }
  uint32_t unknown_472_address() const { return address + 472; }
  uint32_t unknown_472_bits() const { if (auto* d = direct()) return d->unknown_472; return uint32_t((memory::IsHandle(address + 472) ? uint32_t(memory::OwnerLoad(address + 472, 4)) : guest::LoadU32(base, address + 472))); }
  void set_unknown_472_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_472 = v; return; } if (memory::IsHandle(address + 472)) memory::OwnerStore(address + 472, 4, v); else guest::StoreU32(base, address + 472, v); }
  uint32_t unknown_472() const { if (auto* d = direct()) return d->unknown_472; return uint32_t((memory::IsHandle(address + 472) ? uint32_t(memory::OwnerLoad(address + 472, 4)) : guest::LoadU32(base, address + 472))); }
  void set_unknown_472(uint32_t v) const { if (auto* d = direct()) { d->unknown_472 = v; return; } if (memory::IsHandle(address + 472)) memory::OwnerStore(address + 472, 4, v); else guest::StoreU32(base, address + 472, v); }
  uint32_t unknown_476_address() const { return address + 476; }
  uint32_t unknown_476_bits() const { if (auto* d = direct()) return d->unknown_476; return uint32_t((memory::IsHandle(address + 476) ? uint32_t(memory::OwnerLoad(address + 476, 4)) : guest::LoadU32(base, address + 476))); }
  void set_unknown_476_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_476 = v; return; } if (memory::IsHandle(address + 476)) memory::OwnerStore(address + 476, 4, v); else guest::StoreU32(base, address + 476, v); }
  uint32_t unknown_476() const { if (auto* d = direct()) return d->unknown_476; return uint32_t((memory::IsHandle(address + 476) ? uint32_t(memory::OwnerLoad(address + 476, 4)) : guest::LoadU32(base, address + 476))); }
  void set_unknown_476(uint32_t v) const { if (auto* d = direct()) { d->unknown_476 = v; return; } if (memory::IsHandle(address + 476)) memory::OwnerStore(address + 476, 4, v); else guest::StoreU32(base, address + 476, v); }
  uint32_t unknown_480_address() const { return address + 480; }
  uint32_t unknown_480_bits() const { if (auto* d = direct()) return d->unknown_480; return uint32_t((memory::IsHandle(address + 480) ? uint32_t(memory::OwnerLoad(address + 480, 4)) : guest::LoadU32(base, address + 480))); }
  void set_unknown_480_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_480 = v; return; } if (memory::IsHandle(address + 480)) memory::OwnerStore(address + 480, 4, v); else guest::StoreU32(base, address + 480, v); }
  uint32_t unknown_480() const { if (auto* d = direct()) return d->unknown_480; return uint32_t((memory::IsHandle(address + 480) ? uint32_t(memory::OwnerLoad(address + 480, 4)) : guest::LoadU32(base, address + 480))); }
  void set_unknown_480(uint32_t v) const { if (auto* d = direct()) { d->unknown_480 = v; return; } if (memory::IsHandle(address + 480)) memory::OwnerStore(address + 480, 4, v); else guest::StoreU32(base, address + 480, v); }
  uint32_t unknown_488_address() const { return address + 488; }
  uint32_t unknown_488_bits() const { if (auto* d = direct()) return d->unknown_488; return uint32_t((memory::IsHandle(address + 488) ? uint32_t(memory::OwnerLoad(address + 488, 4)) : guest::LoadU32(base, address + 488))); }
  void set_unknown_488_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_488 = v; return; } if (memory::IsHandle(address + 488)) memory::OwnerStore(address + 488, 4, v); else guest::StoreU32(base, address + 488, v); }
  uint32_t unknown_488() const { if (auto* d = direct()) return d->unknown_488; return uint32_t((memory::IsHandle(address + 488) ? uint32_t(memory::OwnerLoad(address + 488, 4)) : guest::LoadU32(base, address + 488))); }
  void set_unknown_488(uint32_t v) const { if (auto* d = direct()) { d->unknown_488 = v; return; } if (memory::IsHandle(address + 488)) memory::OwnerStore(address + 488, 4, v); else guest::StoreU32(base, address + 488, v); }
  uint32_t unknown_492_address() const { return address + 492; }
  uint32_t unknown_492_bits() const { if (auto* d = direct()) return d->unknown_492; return uint32_t((memory::IsHandle(address + 492) ? uint32_t(memory::OwnerLoad(address + 492, 4)) : guest::LoadU32(base, address + 492))); }
  void set_unknown_492_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_492 = v; return; } if (memory::IsHandle(address + 492)) memory::OwnerStore(address + 492, 4, v); else guest::StoreU32(base, address + 492, v); }
  uint32_t unknown_492() const { if (auto* d = direct()) return d->unknown_492; return uint32_t((memory::IsHandle(address + 492) ? uint32_t(memory::OwnerLoad(address + 492, 4)) : guest::LoadU32(base, address + 492))); }
  void set_unknown_492(uint32_t v) const { if (auto* d = direct()) { d->unknown_492 = v; return; } if (memory::IsHandle(address + 492)) memory::OwnerStore(address + 492, 4, v); else guest::StoreU32(base, address + 492, v); }
  uint32_t unknown_496_address() const { return address + 496; }
  uint32_t unknown_496_bits() const { if (auto* d = direct()) return d->unknown_496; return uint32_t((memory::IsHandle(address + 496) ? uint32_t(memory::OwnerLoad(address + 496, 4)) : guest::LoadU32(base, address + 496))); }
  void set_unknown_496_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_496 = v; return; } if (memory::IsHandle(address + 496)) memory::OwnerStore(address + 496, 4, v); else guest::StoreU32(base, address + 496, v); }
  uint32_t unknown_496() const { if (auto* d = direct()) return d->unknown_496; return uint32_t((memory::IsHandle(address + 496) ? uint32_t(memory::OwnerLoad(address + 496, 4)) : guest::LoadU32(base, address + 496))); }
  void set_unknown_496(uint32_t v) const { if (auto* d = direct()) { d->unknown_496 = v; return; } if (memory::IsHandle(address + 496)) memory::OwnerStore(address + 496, 4, v); else guest::StoreU32(base, address + 496, v); }
  uint32_t unknown_572_address() const { return address + 572; }
  uint32_t unknown_572_bits() const { if (auto* d = direct()) return d->unknown_572; return uint32_t((memory::IsHandle(address + 572) ? uint32_t(memory::OwnerLoad(address + 572, 4)) : guest::LoadU32(base, address + 572))); }
  void set_unknown_572_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_572 = v; return; } if (memory::IsHandle(address + 572)) memory::OwnerStore(address + 572, 4, v); else guest::StoreU32(base, address + 572, v); }
  uint32_t unknown_572() const { if (auto* d = direct()) return d->unknown_572; return uint32_t((memory::IsHandle(address + 572) ? uint32_t(memory::OwnerLoad(address + 572, 4)) : guest::LoadU32(base, address + 572))); }
  void set_unknown_572(uint32_t v) const { if (auto* d = direct()) { d->unknown_572 = v; return; } if (memory::IsHandle(address + 572)) memory::OwnerStore(address + 572, 4, v); else guest::StoreU32(base, address + 572, v); }
  uint32_t unknown_576_address() const { return address + 576; }
  uint32_t unknown_576_bits() const { if (auto* d = direct()) return d->unknown_576; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_unknown_576_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_576 = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  uint32_t unknown_576() const { if (auto* d = direct()) return d->unknown_576; return uint32_t((memory::IsHandle(address + 576) ? uint32_t(memory::OwnerLoad(address + 576, 4)) : guest::LoadU32(base, address + 576))); }
  void set_unknown_576(uint32_t v) const { if (auto* d = direct()) { d->unknown_576 = v; return; } if (memory::IsHandle(address + 576)) memory::OwnerStore(address + 576, 4, v); else guest::StoreU32(base, address + 576, v); }
  uint32_t unknown_580_address() const { return address + 580; }
  uint32_t unknown_580_bits() const { if (auto* d = direct()) return d->unknown_580; return uint32_t((memory::IsHandle(address + 580) ? uint32_t(memory::OwnerLoad(address + 580, 4)) : guest::LoadU32(base, address + 580))); }
  void set_unknown_580_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_580 = v; return; } if (memory::IsHandle(address + 580)) memory::OwnerStore(address + 580, 4, v); else guest::StoreU32(base, address + 580, v); }
  uint32_t unknown_580() const { if (auto* d = direct()) return d->unknown_580; return uint32_t((memory::IsHandle(address + 580) ? uint32_t(memory::OwnerLoad(address + 580, 4)) : guest::LoadU32(base, address + 580))); }
  void set_unknown_580(uint32_t v) const { if (auto* d = direct()) { d->unknown_580 = v; return; } if (memory::IsHandle(address + 580)) memory::OwnerStore(address + 580, 4, v); else guest::StoreU32(base, address + 580, v); }
  uint32_t unknown_584_address() const { return address + 584; }
  uint32_t unknown_584_bits() const { if (auto* d = direct()) return d->unknown_584; return uint32_t((memory::IsHandle(address + 584) ? uint32_t(memory::OwnerLoad(address + 584, 4)) : guest::LoadU32(base, address + 584))); }
  void set_unknown_584_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_584 = v; return; } if (memory::IsHandle(address + 584)) memory::OwnerStore(address + 584, 4, v); else guest::StoreU32(base, address + 584, v); }
  uint32_t unknown_584() const { if (auto* d = direct()) return d->unknown_584; return uint32_t((memory::IsHandle(address + 584) ? uint32_t(memory::OwnerLoad(address + 584, 4)) : guest::LoadU32(base, address + 584))); }
  void set_unknown_584(uint32_t v) const { if (auto* d = direct()) { d->unknown_584 = v; return; } if (memory::IsHandle(address + 584)) memory::OwnerStore(address + 584, 4, v); else guest::StoreU32(base, address + 584, v); }
  uint32_t unknown_588_address() const { return address + 588; }
  uint32_t unknown_588_bits() const { if (auto* d = direct()) return d->unknown_588; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(base, address + 588))); }
  void set_unknown_588_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_588 = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(base, address + 588, v); }
  uint32_t unknown_588() const { if (auto* d = direct()) return d->unknown_588; return uint32_t((memory::IsHandle(address + 588) ? uint32_t(memory::OwnerLoad(address + 588, 4)) : guest::LoadU32(base, address + 588))); }
  void set_unknown_588(uint32_t v) const { if (auto* d = direct()) { d->unknown_588 = v; return; } if (memory::IsHandle(address + 588)) memory::OwnerStore(address + 588, 4, v); else guest::StoreU32(base, address + 588, v); }
  uint32_t unknown_592_address() const { return address + 592; }
  uint32_t unknown_592_bits() const { if (auto* d = direct()) return d->unknown_592; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(base, address + 592))); }
  void set_unknown_592_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_592 = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(base, address + 592, v); }
  uint32_t unknown_592() const { if (auto* d = direct()) return d->unknown_592; return uint32_t((memory::IsHandle(address + 592) ? uint32_t(memory::OwnerLoad(address + 592, 4)) : guest::LoadU32(base, address + 592))); }
  void set_unknown_592(uint32_t v) const { if (auto* d = direct()) { d->unknown_592 = v; return; } if (memory::IsHandle(address + 592)) memory::OwnerStore(address + 592, 4, v); else guest::StoreU32(base, address + 592, v); }
  uint32_t unknown_596_address() const { return address + 596; }
  uint32_t unknown_596_bits() const { if (auto* d = direct()) return d->unknown_596; return uint32_t((memory::IsHandle(address + 596) ? uint32_t(memory::OwnerLoad(address + 596, 4)) : guest::LoadU32(base, address + 596))); }
  void set_unknown_596_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_596 = v; return; } if (memory::IsHandle(address + 596)) memory::OwnerStore(address + 596, 4, v); else guest::StoreU32(base, address + 596, v); }
  uint32_t unknown_596() const { if (auto* d = direct()) return d->unknown_596; return uint32_t((memory::IsHandle(address + 596) ? uint32_t(memory::OwnerLoad(address + 596, 4)) : guest::LoadU32(base, address + 596))); }
  void set_unknown_596(uint32_t v) const { if (auto* d = direct()) { d->unknown_596 = v; return; } if (memory::IsHandle(address + 596)) memory::OwnerStore(address + 596, 4, v); else guest::StoreU32(base, address + 596, v); }
  uint32_t unknown_600_address() const { return address + 600; }
  uint32_t unknown_600_bits() const { if (auto* d = direct()) return d->unknown_600; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(base, address + 600))); }
  void set_unknown_600_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_600 = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(base, address + 600, v); }
  uint32_t unknown_600() const { if (auto* d = direct()) return d->unknown_600; return uint32_t((memory::IsHandle(address + 600) ? uint32_t(memory::OwnerLoad(address + 600, 4)) : guest::LoadU32(base, address + 600))); }
  void set_unknown_600(uint32_t v) const { if (auto* d = direct()) { d->unknown_600 = v; return; } if (memory::IsHandle(address + 600)) memory::OwnerStore(address + 600, 4, v); else guest::StoreU32(base, address + 600, v); }
  uint32_t unknown_604_address() const { return address + 604; }
  uint32_t unknown_604_bits() const { if (auto* d = direct()) return d->unknown_604; return uint32_t((memory::IsHandle(address + 604) ? uint32_t(memory::OwnerLoad(address + 604, 4)) : guest::LoadU32(base, address + 604))); }
  void set_unknown_604_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_604 = v; return; } if (memory::IsHandle(address + 604)) memory::OwnerStore(address + 604, 4, v); else guest::StoreU32(base, address + 604, v); }
  uint32_t unknown_604() const { if (auto* d = direct()) return d->unknown_604; return uint32_t((memory::IsHandle(address + 604) ? uint32_t(memory::OwnerLoad(address + 604, 4)) : guest::LoadU32(base, address + 604))); }
  void set_unknown_604(uint32_t v) const { if (auto* d = direct()) { d->unknown_604 = v; return; } if (memory::IsHandle(address + 604)) memory::OwnerStore(address + 604, 4, v); else guest::StoreU32(base, address + 604, v); }
  uint32_t unknown_608_address() const { return address + 608; }
  uint32_t unknown_608_bits() const { if (auto* d = direct()) return d->unknown_608; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_unknown_608_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_608 = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
  uint32_t unknown_608() const { if (auto* d = direct()) return d->unknown_608; return uint32_t((memory::IsHandle(address + 608) ? uint32_t(memory::OwnerLoad(address + 608, 4)) : guest::LoadU32(base, address + 608))); }
  void set_unknown_608(uint32_t v) const { if (auto* d = direct()) { d->unknown_608 = v; return; } if (memory::IsHandle(address + 608)) memory::OwnerStore(address + 608, 4, v); else guest::StoreU32(base, address + 608, v); }
  uint32_t unknown_612_address() const { return address + 612; }
  uint32_t unknown_612_bits() const { if (auto* d = direct()) return d->unknown_612; return uint32_t((memory::IsHandle(address + 612) ? uint32_t(memory::OwnerLoad(address + 612, 4)) : guest::LoadU32(base, address + 612))); }
  void set_unknown_612_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_612 = v; return; } if (memory::IsHandle(address + 612)) memory::OwnerStore(address + 612, 4, v); else guest::StoreU32(base, address + 612, v); }
  uint32_t unknown_612() const { if (auto* d = direct()) return d->unknown_612; return uint32_t((memory::IsHandle(address + 612) ? uint32_t(memory::OwnerLoad(address + 612, 4)) : guest::LoadU32(base, address + 612))); }
  void set_unknown_612(uint32_t v) const { if (auto* d = direct()) { d->unknown_612 = v; return; } if (memory::IsHandle(address + 612)) memory::OwnerStore(address + 612, 4, v); else guest::StoreU32(base, address + 612, v); }
  uint32_t unknown_616_address() const { return address + 616; }
  uint32_t unknown_616_bits() const { if (auto* d = direct()) return d->unknown_616; return uint32_t((memory::IsHandle(address + 616) ? uint32_t(memory::OwnerLoad(address + 616, 4)) : guest::LoadU32(base, address + 616))); }
  void set_unknown_616_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_616 = v; return; } if (memory::IsHandle(address + 616)) memory::OwnerStore(address + 616, 4, v); else guest::StoreU32(base, address + 616, v); }
  uint32_t unknown_616() const { if (auto* d = direct()) return d->unknown_616; return uint32_t((memory::IsHandle(address + 616) ? uint32_t(memory::OwnerLoad(address + 616, 4)) : guest::LoadU32(base, address + 616))); }
  void set_unknown_616(uint32_t v) const { if (auto* d = direct()) { d->unknown_616 = v; return; } if (memory::IsHandle(address + 616)) memory::OwnerStore(address + 616, 4, v); else guest::StoreU32(base, address + 616, v); }
  uint32_t unknown_620_address() const { return address + 620; }
  uint32_t unknown_620_bits() const { if (auto* d = direct()) return d->unknown_620; return uint32_t((memory::IsHandle(address + 620) ? uint32_t(memory::OwnerLoad(address + 620, 4)) : guest::LoadU32(base, address + 620))); }
  void set_unknown_620_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_620 = v; return; } if (memory::IsHandle(address + 620)) memory::OwnerStore(address + 620, 4, v); else guest::StoreU32(base, address + 620, v); }
  uint32_t unknown_620() const { if (auto* d = direct()) return d->unknown_620; return uint32_t((memory::IsHandle(address + 620) ? uint32_t(memory::OwnerLoad(address + 620, 4)) : guest::LoadU32(base, address + 620))); }
  void set_unknown_620(uint32_t v) const { if (auto* d = direct()) { d->unknown_620 = v; return; } if (memory::IsHandle(address + 620)) memory::OwnerStore(address + 620, 4, v); else guest::StoreU32(base, address + 620, v); }
  uint32_t unknown_624_address() const { return address + 624; }
  uint32_t unknown_624_bits() const { if (auto* d = direct()) return d->unknown_624; return uint32_t((memory::IsHandle(address + 624) ? uint32_t(memory::OwnerLoad(address + 624, 4)) : guest::LoadU32(base, address + 624))); }
  void set_unknown_624_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_624 = v; return; } if (memory::IsHandle(address + 624)) memory::OwnerStore(address + 624, 4, v); else guest::StoreU32(base, address + 624, v); }
  uint32_t unknown_624() const { if (auto* d = direct()) return d->unknown_624; return uint32_t((memory::IsHandle(address + 624) ? uint32_t(memory::OwnerLoad(address + 624, 4)) : guest::LoadU32(base, address + 624))); }
  void set_unknown_624(uint32_t v) const { if (auto* d = direct()) { d->unknown_624 = v; return; } if (memory::IsHandle(address + 624)) memory::OwnerStore(address + 624, 4, v); else guest::StoreU32(base, address + 624, v); }
  uint32_t unknown_628_address() const { return address + 628; }
  uint32_t unknown_628_bits() const { if (auto* d = direct()) return d->unknown_628; return uint32_t((memory::IsHandle(address + 628) ? uint32_t(memory::OwnerLoad(address + 628, 4)) : guest::LoadU32(base, address + 628))); }
  void set_unknown_628_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_628 = v; return; } if (memory::IsHandle(address + 628)) memory::OwnerStore(address + 628, 4, v); else guest::StoreU32(base, address + 628, v); }
  uint32_t unknown_628() const { if (auto* d = direct()) return d->unknown_628; return uint32_t((memory::IsHandle(address + 628) ? uint32_t(memory::OwnerLoad(address + 628, 4)) : guest::LoadU32(base, address + 628))); }
  void set_unknown_628(uint32_t v) const { if (auto* d = direct()) { d->unknown_628 = v; return; } if (memory::IsHandle(address + 628)) memory::OwnerStore(address + 628, 4, v); else guest::StoreU32(base, address + 628, v); }
  uint32_t unknown_632_address() const { return address + 632; }
  uint32_t unknown_632_bits() const { if (auto* d = direct()) return d->unknown_632; return uint32_t((memory::IsHandle(address + 632) ? uint32_t(memory::OwnerLoad(address + 632, 4)) : guest::LoadU32(base, address + 632))); }
  void set_unknown_632_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_632 = v; return; } if (memory::IsHandle(address + 632)) memory::OwnerStore(address + 632, 4, v); else guest::StoreU32(base, address + 632, v); }
  uint32_t unknown_632() const { if (auto* d = direct()) return d->unknown_632; return uint32_t((memory::IsHandle(address + 632) ? uint32_t(memory::OwnerLoad(address + 632, 4)) : guest::LoadU32(base, address + 632))); }
  void set_unknown_632(uint32_t v) const { if (auto* d = direct()) { d->unknown_632 = v; return; } if (memory::IsHandle(address + 632)) memory::OwnerStore(address + 632, 4, v); else guest::StoreU32(base, address + 632, v); }
  uint32_t unknown_636_address() const { return address + 636; }
  uint32_t unknown_636_bits() const { if (auto* d = direct()) return d->unknown_636; return uint32_t((memory::IsHandle(address + 636) ? uint32_t(memory::OwnerLoad(address + 636, 4)) : guest::LoadU32(base, address + 636))); }
  void set_unknown_636_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_636 = v; return; } if (memory::IsHandle(address + 636)) memory::OwnerStore(address + 636, 4, v); else guest::StoreU32(base, address + 636, v); }
  uint32_t unknown_636() const { if (auto* d = direct()) return d->unknown_636; return uint32_t((memory::IsHandle(address + 636) ? uint32_t(memory::OwnerLoad(address + 636, 4)) : guest::LoadU32(base, address + 636))); }
  void set_unknown_636(uint32_t v) const { if (auto* d = direct()) { d->unknown_636 = v; return; } if (memory::IsHandle(address + 636)) memory::OwnerStore(address + 636, 4, v); else guest::StoreU32(base, address + 636, v); }
};

// embedded; measure: RaceScreens@+16.+444
struct RaceHudRacersPart : View {
  using View::View;
  static constexpr uint32_t kSize = 3936;
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_828() const { return guest::LoadFloat(base, address + 828); }
  void set_unknown_828(float v) const { guest::StoreFloat(base, address + 828, v); }
  uint32_t unknown_828_address() const { return address + 828; }
  uint32_t unknown_836() const { return guest::LoadU32(base, address + 836); }
  void set_unknown_836(uint32_t v) const { guest::StoreU32(base, address + 836, v); }
  uint32_t unknown_836_address() const { return address + 836; }
  uint32_t unknown_3932() const { return guest::LoadU32(base, address + 3932); }
  void set_unknown_3932(uint32_t v) const { guest::StoreU32(base, address + 3932, v); }
  uint32_t unknown_3932_address() const { return address + 3932; }
};

// The image words docs/functions/race-hud-object.md's code uses.
struct RaceHudObjectGlobals {
  // the race data (race-states.toml's RaceData; left untyped: alt_hud.cpp reads it without that header): +4 the course, the racers' records 3,936 bytes each from +0, +0xA3AF4 the race mode
  static View race_data(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's progress: +2184 the HUD's racers, +7156 a distance, +7240 the item shown
  template <typename T = RaceProgress>
  static T race_progress(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_race_progress(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82E8u, v.address); }
  static void set_race_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t race_progress_address() { return 0x826D82E8u; }
  // the viewports (camera-modes.toml's Viewports): +0 viewport 0 (+1860 its view)
  template <typename T = Viewports>
  static T viewports(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2CE8u)); }
  static void set_viewports(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2CE8u, v.address); }
  static void set_viewports(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2CE8u, v); }
  static constexpr uint32_t viewports_address() { return 0x826C2CE8u; }
  // the track zones' owner (track-zones.md)
  template <typename T = TrackZones>
  static T track_zones(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161ECu)); }
  static void set_track_zones(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161ECu, v.address); }
  static void set_track_zones(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161ECu, v); }
  static constexpr uint32_t track_zones_address() { return 0x828161ECu; }
  // the in-race screens' holder (race-hud-machine.toml's RaceScreens)
  template <typename T = RaceScreens>
  static T screens_holder(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D84F0u)); }
  static void set_screens_holder(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D84F0u, v.address); }
  static void set_screens_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84F0u, v); }
  static constexpr uint32_t screens_holder_address() { return 0x826D84F0u; }
  // the string table (Strings::Load and Release, 47)
  template <typename T = Strings>
  static T strings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C34u)); }
  static void set_strings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C34u, v.address); }
  static void set_strings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t strings_address() { return 0x826C5C34u; }
};

}  // namespace schema
