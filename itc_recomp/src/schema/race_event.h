// The views of docs/functions/race-event.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-event.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: RaceEvent
#pragma pack(push, 1)
struct RaceEventData {
  uint32_t section_rival_waiting = 0;  // +0
  uint32_t on_section_13 = 0;  // +4
  uint32_t on_section_14 = 0;  // +8
  uint32_t on_section_4 = 0;  // +12
  uint32_t on_section_5 = 0;  // +16
  uint16_t count = 0;  // +20
  uint16_t open_0_to_3 = 0;  // +22
  uint16_t locked_0_to_3 = 0;  // +24
  uint16_t open_4_to_9 = 0;  // +26
  uint16_t locked_4_to_9 = 0;  // +28
  uint16_t mode = 0;  // +30
  uint16_t mode_streak = 0;  // +32
  uint16_t location = 0;  // +34
  uint16_t group0_open = 0;  // +36
  uint16_t group0_locked = 0;  // +38
  uint32_t group0_entries = 0;  // +40
  uint32_t group0_locked_entries = 0;  // +44
  uint8_t pad_48[36] = {};
  uint16_t group4_open = 0;  // +84
  uint16_t group4_locked = 0;  // +86
  uint32_t group4_entries = 0;  // +88
  uint32_t group4_locked_entries = 0;  // +92
  uint8_t pad_96[60] = {};
  uint32_t list = 0;  // +156
};
#pragma pack(pop)
static_assert(sizeof(RaceEventData) == 160);
inline constexpr memory::LayoutField kRaceEventFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 2}, {22, 2}, {24, 2}, {26, 2}, {28, 2}, {30, 2}, {32, 2}, {34, 2}, {36, 2}, {38, 2}, {40, 4}, {44, 4}, {48, 2}, {50, 2}, {52, 4}, {56, 4}, {60, 2}, {62, 2}, {64, 4}, {68, 4}, {72, 2}, {74, 2}, {76, 4}, {80, 4}, {84, 2}, {86, 2}, {88, 4}, {92, 4}, {96, 2}, {98, 2}, {100, 4}, {104, 4}, {108, 2}, {110, 2}, {112, 4}, {116, 4}, {120, 2}, {122, 2}, {124, 4}, {128, 4}, {132, 2}, {134, 2}, {136, 4}, {140, 4}, {144, 2}, {146, 2}, {148, 4}, {152, 4}, {156, 4}};
inline constexpr memory::Layout kRaceEventLayout = {kRaceEventFields, 54};

struct RaceEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceEventData* data() const { return static_cast<RaceEventData*>(memory::HandleObject(address)); }
  static RaceEvent New() {
    return RaceEvent(memory::MakeHandle(new RaceEventData(), sizeof(RaceEventData), "RaceEvent", &kRaceEventLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceEvent NewArray(uint32_t count) {
    static_assert(sizeof(RaceEventData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceEventData*>(::operator new(sizeof(RaceEventData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceEventData();
    return RaceEvent(memory::MakeHandle(p, sizeof(RaceEventData) * count, "RaceEvent", &kRaceEventLayout, sizeof(RaceEventData)));
  }
  RaceEvent at(uint32_t i) const { return RaceEvent(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceEventData* direct() const { return static_cast<RaceEventData*>(memory::HandleOwn(address, &kRaceEventLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceEventLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the ten groups, one a location 0 to 9
  template <typename T = EventGroup>
  T group(uint32_t i) const { return T(address + 36 + i * T::kSize); }
  // 1 when a list entry's item has a kind 65 condition (RaceEvent::Init); also the list entry's item id through the measure's join
  uint32_t section_rival_waiting_address() const { return address + 0; }
  uint32_t section_rival_waiting_bits() const { if (auto* d = direct()) return d->section_rival_waiting; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_section_rival_waiting_bits(uint32_t v) const { if (auto* d = direct()) { d->section_rival_waiting = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t section_rival_waiting() const { if (auto* d = direct()) return d->section_rival_waiting; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_section_rival_waiting(uint32_t v) const { if (auto* d = direct()) { d->section_rival_waiting = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // 1 while the player's car is on section 13's slices 0 to 9 (AiPath::NoteFixedSections; +4 and +8, or +12 and +16, let the section rival in); also a group's entries through the measure's join
  uint32_t on_section_13_address() const { return address + 4; }
  uint32_t on_section_13_bits() const { if (auto* d = direct()) return d->on_section_13; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_on_section_13_bits(uint32_t v) const { if (auto* d = direct()) { d->on_section_13 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t on_section_13() const { if (auto* d = direct()) return d->on_section_13; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_on_section_13(uint32_t v) const { if (auto* d = direct()) { d->on_section_13 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // section 14's slices 0 to 4; also a group's locked entries through the measure's join
  uint32_t on_section_14_address() const { return address + 8; }
  uint32_t on_section_14_bits() const { if (auto* d = direct()) return d->on_section_14; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_on_section_14_bits(uint32_t v) const { if (auto* d = direct()) { d->on_section_14 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t on_section_14() const { if (auto* d = direct()) return d->on_section_14; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_on_section_14(uint32_t v) const { if (auto* d = direct()) { d->on_section_14 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // section 4's slices 0 to 9
  uint32_t on_section_4_address() const { return address + 12; }
  uint32_t on_section_4_bits() const { if (auto* d = direct()) return d->on_section_4; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_on_section_4_bits(uint32_t v) const { if (auto* d = direct()) { d->on_section_4 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t on_section_4() const { if (auto* d = direct()) return d->on_section_4; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_on_section_4(uint32_t v) const { if (auto* d = direct()) { d->on_section_4 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // section 5's slices 0 to 4
  uint32_t on_section_5_address() const { return address + 16; }
  uint32_t on_section_5_bits() const { if (auto* d = direct()) return d->on_section_5; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_on_section_5_bits(uint32_t v) const { if (auto* d = direct()) { d->on_section_5 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t on_section_5() const { if (auto* d = direct()) return d->on_section_5; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_on_section_5(uint32_t v) const { if (auto* d = direct()) { d->on_section_5 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // the list's count (+22 + +24)
  uint32_t count_address() const { return address + 20; }
  uint16_t count_bits() const { if (auto* d = direct()) return d->count; return uint16_t((memory::IsHandle(address + 20) ? uint16_t(memory::OwnerLoad(address + 20, 2)) : guest::LoadU16(address + 20))); }
  void set_count_bits(uint16_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 2, v); else guest::StoreU16(address + 20, v); }
  uint16_t count() const { if (auto* d = direct()) return d->count; return uint16_t((memory::IsHandle(address + 20) ? uint16_t(memory::OwnerLoad(address + 20, 2)) : guest::LoadU16(address + 20))); }
  void set_count(uint16_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 2, v); else guest::StoreU16(address + 20, v); }
  // groups 0 to 3's open counts summed
  uint32_t open_0_to_3_address() const { return address + 22; }
  uint16_t open_0_to_3_bits() const { if (auto* d = direct()) return d->open_0_to_3; return uint16_t((memory::IsHandle(address + 22) ? uint16_t(memory::OwnerLoad(address + 22, 2)) : guest::LoadU16(address + 22))); }
  void set_open_0_to_3_bits(uint16_t v) const { if (auto* d = direct()) { d->open_0_to_3 = v; return; } if (memory::IsHandle(address + 22)) memory::OwnerStore(address + 22, 2, v); else guest::StoreU16(address + 22, v); }
  uint16_t open_0_to_3() const { if (auto* d = direct()) return d->open_0_to_3; return uint16_t((memory::IsHandle(address + 22) ? uint16_t(memory::OwnerLoad(address + 22, 2)) : guest::LoadU16(address + 22))); }
  void set_open_0_to_3(uint16_t v) const { if (auto* d = direct()) { d->open_0_to_3 = v; return; } if (memory::IsHandle(address + 22)) memory::OwnerStore(address + 22, 2, v); else guest::StoreU16(address + 22, v); }
  // groups 0 to 3's locked counts summed
  uint32_t locked_0_to_3_address() const { return address + 24; }
  uint16_t locked_0_to_3_bits() const { if (auto* d = direct()) return d->locked_0_to_3; return uint16_t((memory::IsHandle(address + 24) ? uint16_t(memory::OwnerLoad(address + 24, 2)) : guest::LoadU16(address + 24))); }
  void set_locked_0_to_3_bits(uint16_t v) const { if (auto* d = direct()) { d->locked_0_to_3 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 2, v); else guest::StoreU16(address + 24, v); }
  uint16_t locked_0_to_3() const { if (auto* d = direct()) return d->locked_0_to_3; return uint16_t((memory::IsHandle(address + 24) ? uint16_t(memory::OwnerLoad(address + 24, 2)) : guest::LoadU16(address + 24))); }
  void set_locked_0_to_3(uint16_t v) const { if (auto* d = direct()) { d->locked_0_to_3 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 2, v); else guest::StoreU16(address + 24, v); }
  // groups 4 to 9's open counts summed
  uint32_t open_4_to_9_address() const { return address + 26; }
  uint16_t open_4_to_9_bits() const { if (auto* d = direct()) return d->open_4_to_9; return uint16_t((memory::IsHandle(address + 26) ? uint16_t(memory::OwnerLoad(address + 26, 2)) : guest::LoadU16(address + 26))); }
  void set_open_4_to_9_bits(uint16_t v) const { if (auto* d = direct()) { d->open_4_to_9 = v; return; } if (memory::IsHandle(address + 26)) memory::OwnerStore(address + 26, 2, v); else guest::StoreU16(address + 26, v); }
  uint16_t open_4_to_9() const { if (auto* d = direct()) return d->open_4_to_9; return uint16_t((memory::IsHandle(address + 26) ? uint16_t(memory::OwnerLoad(address + 26, 2)) : guest::LoadU16(address + 26))); }
  void set_open_4_to_9(uint16_t v) const { if (auto* d = direct()) { d->open_4_to_9 = v; return; } if (memory::IsHandle(address + 26)) memory::OwnerStore(address + 26, 2, v); else guest::StoreU16(address + 26, v); }
  // groups 4 to 9's locked counts summed
  uint32_t locked_4_to_9_address() const { return address + 28; }
  uint16_t locked_4_to_9_bits() const { if (auto* d = direct()) return d->locked_4_to_9; return uint16_t((memory::IsHandle(address + 28) ? uint16_t(memory::OwnerLoad(address + 28, 2)) : guest::LoadU16(address + 28))); }
  void set_locked_4_to_9_bits(uint16_t v) const { if (auto* d = direct()) { d->locked_4_to_9 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 2, v); else guest::StoreU16(address + 28, v); }
  uint16_t locked_4_to_9() const { if (auto* d = direct()) return d->locked_4_to_9; return uint16_t((memory::IsHandle(address + 28) ? uint16_t(memory::OwnerLoad(address + 28, 2)) : guest::LoadU16(address + 28))); }
  void set_locked_4_to_9(uint16_t v) const { if (auto* d = direct()) { d->locked_4_to_9 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 2, v); else guest::StoreU16(address + 28, v); }
  // RaceEvent::SetMode
  uint32_t mode_address() const { return address + 30; }
  uint16_t mode_bits() const { if (auto* d = direct()) return d->mode; return uint16_t((memory::IsHandle(address + 30) ? uint16_t(memory::OwnerLoad(address + 30, 2)) : guest::LoadU16(address + 30))); }
  void set_mode_bits(uint16_t v) const { if (auto* d = direct()) { d->mode = v; return; } if (memory::IsHandle(address + 30)) memory::OwnerStore(address + 30, 2, v); else guest::StoreU16(address + 30, v); }
  uint16_t mode() const { if (auto* d = direct()) return d->mode; return uint16_t((memory::IsHandle(address + 30) ? uint16_t(memory::OwnerLoad(address + 30, 2)) : guest::LoadU16(address + 30))); }
  void set_mode(uint16_t v) const { if (auto* d = direct()) { d->mode = v; return; } if (memory::IsHandle(address + 30)) memory::OwnerStore(address + 30, 2, v); else guest::StoreU16(address + 30, v); }
  // the save data's +6060 + 1 with the same mode as last, else 1
  uint32_t mode_streak_address() const { return address + 32; }
  uint16_t mode_streak_bits() const { if (auto* d = direct()) return d->mode_streak; return uint16_t((memory::IsHandle(address + 32) ? uint16_t(memory::OwnerLoad(address + 32, 2)) : guest::LoadU16(address + 32))); }
  void set_mode_streak_bits(uint16_t v) const { if (auto* d = direct()) { d->mode_streak = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 2, v); else guest::StoreU16(address + 32, v); }
  uint16_t mode_streak() const { if (auto* d = direct()) return d->mode_streak; return uint16_t((memory::IsHandle(address + 32) ? uint16_t(memory::OwnerLoad(address + 32, 2)) : guest::LoadU16(address + 32))); }
  void set_mode_streak(uint16_t v) const { if (auto* d = direct()) { d->mode_streak = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 2, v); else guest::StoreU16(address + 32, v); }
  uint32_t location_address() const { return address + 34; }
  uint16_t location_bits() const { if (auto* d = direct()) return d->location; return uint16_t((memory::IsHandle(address + 34) ? uint16_t(memory::OwnerLoad(address + 34, 2)) : guest::LoadU16(address + 34))); }
  void set_location_bits(uint16_t v) const { if (auto* d = direct()) { d->location = v; return; } if (memory::IsHandle(address + 34)) memory::OwnerStore(address + 34, 2, v); else guest::StoreU16(address + 34, v); }
  uint16_t location() const { if (auto* d = direct()) return d->location; return uint16_t((memory::IsHandle(address + 34) ? uint16_t(memory::OwnerLoad(address + 34, 2)) : guest::LoadU16(address + 34))); }
  void set_location(uint16_t v) const { if (auto* d = direct()) { d->location = v; return; } if (memory::IsHandle(address + 34)) memory::OwnerStore(address + 34, 2, v); else guest::StoreU16(address + 34, v); }
  // ten EventGroups, one a location 0 to 9
  uint32_t groups_address() const { return address + 36; }
  uint8_t* groups() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 36 : guest::Pointer(address + 36); }
  // group 0's (EventGroup.open)
  uint32_t group0_open_address() const { return address + 36; }
  uint16_t group0_open_bits() const { if (auto* d = direct()) return d->group0_open; return uint16_t((memory::IsHandle(address + 36) ? uint16_t(memory::OwnerLoad(address + 36, 2)) : guest::LoadU16(address + 36))); }
  void set_group0_open_bits(uint16_t v) const { if (auto* d = direct()) { d->group0_open = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 2, v); else guest::StoreU16(address + 36, v); }
  uint16_t group0_open() const { if (auto* d = direct()) return d->group0_open; return uint16_t((memory::IsHandle(address + 36) ? uint16_t(memory::OwnerLoad(address + 36, 2)) : guest::LoadU16(address + 36))); }
  void set_group0_open(uint16_t v) const { if (auto* d = direct()) { d->group0_open = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 2, v); else guest::StoreU16(address + 36, v); }
  uint32_t group0_locked_address() const { return address + 38; }
  uint16_t group0_locked_bits() const { if (auto* d = direct()) return d->group0_locked; return uint16_t((memory::IsHandle(address + 38) ? uint16_t(memory::OwnerLoad(address + 38, 2)) : guest::LoadU16(address + 38))); }
  void set_group0_locked_bits(uint16_t v) const { if (auto* d = direct()) { d->group0_locked = v; return; } if (memory::IsHandle(address + 38)) memory::OwnerStore(address + 38, 2, v); else guest::StoreU16(address + 38, v); }
  uint16_t group0_locked() const { if (auto* d = direct()) return d->group0_locked; return uint16_t((memory::IsHandle(address + 38) ? uint16_t(memory::OwnerLoad(address + 38, 2)) : guest::LoadU16(address + 38))); }
  void set_group0_locked(uint16_t v) const { if (auto* d = direct()) { d->group0_locked = v; return; } if (memory::IsHandle(address + 38)) memory::OwnerStore(address + 38, 2, v); else guest::StoreU16(address + 38, v); }
  uint32_t group0_entries_address() const { return address + 40; }
  uint32_t group0_entries_bits() const { if (auto* d = direct()) return d->group0_entries; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_group0_entries_bits(uint32_t v) const { if (auto* d = direct()) { d->group0_entries = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = EventGroupEntry>
  T group0_entries() const { return T(group0_entries_bits()); }
  void set_group0_entries(const View& w) const { set_group0_entries_bits(w.address); }
  void set_group0_entries(uint32_t v) const { set_group0_entries_bits(v); }
  uint32_t group0_locked_entries_address() const { return address + 44; }
  uint32_t group0_locked_entries_bits() const { if (auto* d = direct()) return d->group0_locked_entries; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_group0_locked_entries_bits(uint32_t v) const { if (auto* d = direct()) { d->group0_locked_entries = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  template <typename T = EventGroupEntry>
  T group0_locked_entries() const { return T(group0_locked_entries_bits()); }
  void set_group0_locked_entries(const View& w) const { set_group0_locked_entries_bits(w.address); }
  void set_group0_locked_entries(uint32_t v) const { set_group0_locked_entries_bits(v); }
  // group 4's (EventGroup.open)
  uint32_t group4_open_address() const { return address + 84; }
  uint16_t group4_open_bits() const { if (auto* d = direct()) return d->group4_open; return uint16_t((memory::IsHandle(address + 84) ? uint16_t(memory::OwnerLoad(address + 84, 2)) : guest::LoadU16(address + 84))); }
  void set_group4_open_bits(uint16_t v) const { if (auto* d = direct()) { d->group4_open = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 2, v); else guest::StoreU16(address + 84, v); }
  uint16_t group4_open() const { if (auto* d = direct()) return d->group4_open; return uint16_t((memory::IsHandle(address + 84) ? uint16_t(memory::OwnerLoad(address + 84, 2)) : guest::LoadU16(address + 84))); }
  void set_group4_open(uint16_t v) const { if (auto* d = direct()) { d->group4_open = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 2, v); else guest::StoreU16(address + 84, v); }
  uint32_t group4_locked_address() const { return address + 86; }
  uint16_t group4_locked_bits() const { if (auto* d = direct()) return d->group4_locked; return uint16_t((memory::IsHandle(address + 86) ? uint16_t(memory::OwnerLoad(address + 86, 2)) : guest::LoadU16(address + 86))); }
  void set_group4_locked_bits(uint16_t v) const { if (auto* d = direct()) { d->group4_locked = v; return; } if (memory::IsHandle(address + 86)) memory::OwnerStore(address + 86, 2, v); else guest::StoreU16(address + 86, v); }
  uint16_t group4_locked() const { if (auto* d = direct()) return d->group4_locked; return uint16_t((memory::IsHandle(address + 86) ? uint16_t(memory::OwnerLoad(address + 86, 2)) : guest::LoadU16(address + 86))); }
  void set_group4_locked(uint16_t v) const { if (auto* d = direct()) { d->group4_locked = v; return; } if (memory::IsHandle(address + 86)) memory::OwnerStore(address + 86, 2, v); else guest::StoreU16(address + 86, v); }
  uint32_t group4_entries_address() const { return address + 88; }
  uint32_t group4_entries_bits() const { if (auto* d = direct()) return d->group4_entries; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_group4_entries_bits(uint32_t v) const { if (auto* d = direct()) { d->group4_entries = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  template <typename T = EventGroupEntry>
  T group4_entries() const { return T(group4_entries_bits()); }
  void set_group4_entries(const View& w) const { set_group4_entries_bits(w.address); }
  void set_group4_entries(uint32_t v) const { set_group4_entries_bits(v); }
  uint32_t group4_locked_entries_address() const { return address + 92; }
  uint32_t group4_locked_entries_bits() const { if (auto* d = direct()) return d->group4_locked_entries; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_group4_locked_entries_bits(uint32_t v) const { if (auto* d = direct()) { d->group4_locked_entries = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  template <typename T = EventGroupEntry>
  T group4_locked_entries() const { return T(group4_locked_entries_bits()); }
  void set_group4_locked_entries(const View& w) const { set_group4_locked_entries_bits(w.address); }
  void set_group4_locked_entries(uint32_t v) const { set_group4_locked_entries_bits(v); }
  // VirtualAlloc'ed, count +20
  uint32_t list_address() const { return address + 156; }
  uint32_t list_bits() const { if (auto* d = direct()) return d->list; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_list_bits(uint32_t v) const { if (auto* d = direct()) { d->list = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  template <typename T = RaceEventEntry>
  T list() const { return T(list_bits()); }
  void set_list(const View& w) const { set_list_bits(w.address); }
  void set_list(uint32_t v) const { set_list_bits(v); }
};

// allocated; measure: new<?>@RaceEvent::Alloc, RaceEvent::Entry.ret
#pragma pack(push, 1)
struct RaceEventEntryData {
  uint32_t item_id = 0;  // +0
  uint32_t start = 0;  // +4
  uint32_t flag = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(RaceEventEntryData) == 12);
inline constexpr memory::LayoutField kRaceEventEntryFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kRaceEventEntryLayout = {kRaceEventEntryFields, 3};

struct RaceEventEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceEventEntryData* data() const { return static_cast<RaceEventEntryData*>(memory::HandleObject(address)); }
  static RaceEventEntry New() {
    return RaceEventEntry(memory::MakeHandle(new RaceEventEntryData(), sizeof(RaceEventEntryData), "RaceEventEntry", &kRaceEventEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceEventEntry NewArray(uint32_t count) {
    static_assert(sizeof(RaceEventEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceEventEntryData*>(::operator new(sizeof(RaceEventEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceEventEntryData();
    return RaceEventEntry(memory::MakeHandle(p, sizeof(RaceEventEntryData) * count, "RaceEventEntry", &kRaceEventEntryLayout, sizeof(RaceEventEntryData)));
  }
  RaceEventEntry at(uint32_t i) const { return RaceEventEntry(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceEventEntryData* direct() const { return static_cast<RaceEventEntryData*>(memory::HandleOwn(address, &kRaceEventEntryLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceEventEntryLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventEntryLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the opponent's item (its car build at the item's +160)
  uint32_t item_id_address() const { return address + 0; }
  uint32_t item_id_bits() const { if (auto* d = direct()) return d->item_id; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_item_id_bits(uint32_t v) const { if (auto* d = direct()) { d->item_id = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t item_id() const { if (auto* d = direct()) return d->item_id; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_item_id(uint32_t v) const { if (auto* d = direct()) { d->item_id = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // a start position (two halfwords, -1 -1 until placed: RaceEvent::PlaceEntry)
  uint32_t start_address() const { return address + 4; }
  uint32_t start_bits() const { if (auto* d = direct()) return d->start; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_start_bits(uint32_t v) const { if (auto* d = direct()) { d->start = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t start() const { if (auto* d = direct()) return d->start; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_start(uint32_t v) const { if (auto* d = direct()) { d->start = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // 1 open or found among the racers; 0 gives the racer flags 0x300
  uint32_t flag_address() const { return address + 8; }
  uint32_t flag_bits() const { if (auto* d = direct()) return d->flag; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_flag_bits(uint32_t v) const { if (auto* d = direct()) { d->flag = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t flag() const { if (auto* d = direct()) return d->flag; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_flag(uint32_t v) const { if (auto* d = direct()) { d->flag = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // start's high halfword: the start's route section (RaceManager::AddRacers)
  uint32_t start_section_address() const { return address + 4; }
  uint16_t start_section_bits() const { if (auto* d = direct()) return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventEntryLayout, 4, 2)); return uint16_t((memory::IsHandle(address + 4) ? uint16_t(memory::OwnerLoad(address + 4, 2)) : guest::LoadU16(address + 4))); }
  void set_start_section_bits(uint16_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventEntryLayout, 4, 2, v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 2, v); else guest::StoreU16(address + 4, v); }
  uint16_t start_section() const { return uint16_t(start_section_bits()); }
  void set_start_section(uint16_t v) const { set_start_section_bits(uint16_t(v)); }
  // start's low halfword: its slice
  uint32_t start_slice_address() const { return address + 6; }
  uint16_t start_slice_bits() const { if (auto* d = direct()) return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventEntryLayout, 6, 2)); return uint16_t((memory::IsHandle(address + 6) ? uint16_t(memory::OwnerLoad(address + 6, 2)) : guest::LoadU16(address + 6))); }
  void set_start_slice_bits(uint16_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventEntryLayout, 6, 2, v); return; } if (memory::IsHandle(address + 6)) memory::OwnerStore(address + 6, 2, v); else guest::StoreU16(address + 6, v); }
  uint16_t start_slice() const { return uint16_t(start_slice_bits()); }
  void set_start_slice(uint16_t v) const { set_start_slice_bits(uint16_t(v)); }
};

// embedded; measure: EventGroup, EventGroup::Alloc.r3, EventGroup::Free.r3
#pragma pack(push, 1)
struct EventGroupData {
  uint16_t open = 0;  // +0
  uint16_t locked = 0;  // +2
  uint32_t entries = 0;  // +4
  uint32_t locked_entries = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(EventGroupData) == 12);
inline constexpr memory::LayoutField kEventGroupFields[] = {{0, 2}, {2, 2}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kEventGroupLayout = {kEventGroupFields, 4};

struct EventGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  EventGroupData* data() const { return static_cast<EventGroupData*>(memory::HandleObject(address)); }
  static EventGroup New() {
    return EventGroup(memory::MakeHandle(new EventGroupData(), sizeof(EventGroupData), "EventGroup", &kEventGroupLayout));
  }
  template <uint32_t Stride = kSize>
  static EventGroup NewArray(uint32_t count) {
    static_assert(sizeof(EventGroupData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<EventGroupData*>(::operator new(sizeof(EventGroupData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) EventGroupData();
    return EventGroup(memory::MakeHandle(p, sizeof(EventGroupData) * count, "EventGroup", &kEventGroupLayout, sizeof(EventGroupData)));
  }
  EventGroup at(uint32_t i) const { return EventGroup(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  EventGroupData* direct() const { return static_cast<EventGroupData*>(memory::HandleOwn(address, &kEventGroupLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kEventGroupLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the open count
  uint32_t open_address() const { return address + 0; }
  uint16_t open_bits() const { if (auto* d = direct()) return d->open; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_open_bits(uint16_t v) const { if (auto* d = direct()) { d->open = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t open() const { if (auto* d = direct()) return d->open; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_open(uint16_t v) const { if (auto* d = direct()) { d->open = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  // the locked count
  uint32_t locked_address() const { return address + 2; }
  uint16_t locked_bits() const { if (auto* d = direct()) return d->locked; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_locked_bits(uint16_t v) const { if (auto* d = direct()) { d->locked = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  uint16_t locked() const { if (auto* d = direct()) return d->locked; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_locked(uint16_t v) const { if (auto* d = direct()) { d->locked = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  // VirtualAlloc'ed, open + locked entries
  uint32_t entries_address() const { return address + 4; }
  uint32_t entries_bits() const { if (auto* d = direct()) return d->entries; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_entries_bits(uint32_t v) const { if (auto* d = direct()) { d->entries = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = EventGroupEntry>
  T entries() const { return T(entries_bits()); }
  void set_entries(const View& w) const { set_entries_bits(w.address); }
  void set_entries(uint32_t v) const { set_entries_bits(v); }
  // the locked part: +4 + 4 x the open count
  uint32_t locked_entries_address() const { return address + 8; }
  uint32_t locked_entries_bits() const { if (auto* d = direct()) return d->locked_entries; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_locked_entries_bits(uint32_t v) const { if (auto* d = direct()) { d->locked_entries = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = EventGroupEntry>
  T locked_entries() const { return T(locked_entries_bits()); }
  void set_locked_entries(const View& w) const { set_locked_entries_bits(w.address); }
  void set_locked_entries(uint32_t v) const { set_locked_entries_bits(v); }
};

// embedded; measure: EventGroup.+4, EventGroup.+8, EventPools::CopyIds.r4, RaceEvent@+84.+4, RaceEvent@+84.+8
#pragma pack(push, 1)
struct EventGroupEntryData {
  uint16_t item_id = 0;  // +0
  uint16_t list_index = 0;  // +2
};
#pragma pack(pop)
static_assert(sizeof(EventGroupEntryData) == 4);
inline constexpr memory::LayoutField kEventGroupEntryFields[] = {{0, 2}, {2, 2}};
inline constexpr memory::Layout kEventGroupEntryLayout = {kEventGroupEntryFields, 2};

struct EventGroupEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  EventGroupEntryData* data() const { return static_cast<EventGroupEntryData*>(memory::HandleObject(address)); }
  static EventGroupEntry New() {
    return EventGroupEntry(memory::MakeHandle(new EventGroupEntryData(), sizeof(EventGroupEntryData), "EventGroupEntry", &kEventGroupEntryLayout));
  }
  template <uint32_t Stride = kSize>
  static EventGroupEntry NewArray(uint32_t count) {
    static_assert(sizeof(EventGroupEntryData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<EventGroupEntryData*>(::operator new(sizeof(EventGroupEntryData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) EventGroupEntryData();
    return EventGroupEntry(memory::MakeHandle(p, sizeof(EventGroupEntryData) * count, "EventGroupEntry", &kEventGroupEntryLayout, sizeof(EventGroupEntryData)));
  }
  EventGroupEntry at(uint32_t i) const { return EventGroupEntry(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  EventGroupEntryData* direct() const { return static_cast<EventGroupEntryData*>(memory::HandleOwn(address, &kEventGroupEntryLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kEventGroupEntryLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupEntryLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t item_id_address() const { return address + 0; }
  uint16_t item_id_bits() const { if (auto* d = direct()) return d->item_id; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_item_id_bits(uint16_t v) const { if (auto* d = direct()) { d->item_id = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t item_id() const { if (auto* d = direct()) return d->item_id; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_item_id(uint16_t v) const { if (auto* d = direct()) { d->item_id = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  // its index in the event's list (locations 0 to 3; -1 once promoted past the open ones)
  uint32_t list_index_address() const { return address + 2; }
  uint16_t list_index_bits() const { if (auto* d = direct()) return d->list_index; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_list_index_bits(uint16_t v) const { if (auto* d = direct()) { d->list_index = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  uint16_t list_index() const { if (auto* d = direct()) return d->list_index; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_list_index(uint16_t v) const { if (auto* d = direct()) { d->list_index = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  // the entry as one word (the swaps)
  uint32_t word_address() const { return address + 0; }
  uint32_t word_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupEntryLayout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_word_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupEntryLayout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t word() const { return uint32_t(word_bits()); }
  void set_word(uint32_t v) const { set_word_bits(uint32_t(v)); }
};

// embedded; measure: EventPools
#pragma pack(push, 1)
struct EventPoolsData {
  uint16_t open = 0;  // +0
  uint8_t open_ids[200] = {};  // +2, guest order
  uint16_t won = 0;  // +202
  uint8_t won_ids[200] = {};  // +204, guest order
  uint16_t locked = 0;  // +404
  uint8_t locked_ids[200] = {};  // +406, guest order
  uint16_t kept = 0;  // +606
};
#pragma pack(pop)
static_assert(sizeof(EventPoolsData) == 608);
inline constexpr memory::LayoutField kEventPoolsFields[] = {{0, 2}, {202, 2}, {404, 2}, {606, 2}};
inline constexpr memory::Layout kEventPoolsLayout = {kEventPoolsFields, 4};

struct EventPools : View {
  using View::View;
  static constexpr uint32_t kSize = 608;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  EventPoolsData* data() const { return static_cast<EventPoolsData*>(memory::HandleObject(address)); }
  static EventPools New() {
    return EventPools(memory::MakeHandle(new EventPoolsData(), sizeof(EventPoolsData), "EventPools", &kEventPoolsLayout));
  }
  template <uint32_t Stride = kSize>
  static EventPools NewArray(uint32_t count) {
    static_assert(sizeof(EventPoolsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<EventPoolsData*>(::operator new(sizeof(EventPoolsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) EventPoolsData();
    return EventPools(memory::MakeHandle(p, sizeof(EventPoolsData) * count, "EventPools", &kEventPoolsLayout, sizeof(EventPoolsData)));
  }
  EventPools at(uint32_t i) const { return EventPools(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  EventPoolsData* direct() const { return static_cast<EventPoolsData*>(memory::HandleOwn(address, &kEventPoolsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kEventPoolsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventPoolsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the open items not beaten
  template <typename T = EventIdList>
  T open_list() const { return T(address + 0); }
  // the open items beaten
  template <typename T = EventIdList>
  T won_list() const { return T(address + 202); }
  // the locked items and the rivals of the others
  template <typename T = EventIdList>
  T locked_list() const { return T(address + 404); }
  // the open items not beaten: the count
  uint32_t open_address() const { return address + 0; }
  uint16_t open_bits() const { if (auto* d = direct()) return d->open; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_open_bits(uint16_t v) const { if (auto* d = direct()) { d->open = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t open() const { if (auto* d = direct()) return d->open; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_open(uint16_t v) const { if (auto* d = direct()) { d->open = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  // their ids
  uint32_t open_ids_address() const { return address + 2; }
  uint8_t* open_ids() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2 : guest::Pointer(address + 2); }
  // the open items beaten: the count
  uint32_t won_address() const { return address + 202; }
  uint16_t won_bits() const { if (auto* d = direct()) return d->won; return uint16_t((memory::IsHandle(address + 202) ? uint16_t(memory::OwnerLoad(address + 202, 2)) : guest::LoadU16(address + 202))); }
  void set_won_bits(uint16_t v) const { if (auto* d = direct()) { d->won = v; return; } if (memory::IsHandle(address + 202)) memory::OwnerStore(address + 202, 2, v); else guest::StoreU16(address + 202, v); }
  uint16_t won() const { if (auto* d = direct()) return d->won; return uint16_t((memory::IsHandle(address + 202) ? uint16_t(memory::OwnerLoad(address + 202, 2)) : guest::LoadU16(address + 202))); }
  void set_won(uint16_t v) const { if (auto* d = direct()) { d->won = v; return; } if (memory::IsHandle(address + 202)) memory::OwnerStore(address + 202, 2, v); else guest::StoreU16(address + 202, v); }
  uint32_t won_ids_address() const { return address + 204; }
  uint8_t* won_ids() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(address + 204); }
  // the locked items and the rivals of the others: the count
  uint32_t locked_address() const { return address + 404; }
  uint16_t locked_bits() const { if (auto* d = direct()) return d->locked; return uint16_t((memory::IsHandle(address + 404) ? uint16_t(memory::OwnerLoad(address + 404, 2)) : guest::LoadU16(address + 404))); }
  void set_locked_bits(uint16_t v) const { if (auto* d = direct()) { d->locked = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 2, v); else guest::StoreU16(address + 404, v); }
  uint16_t locked() const { if (auto* d = direct()) return d->locked; return uint16_t((memory::IsHandle(address + 404) ? uint16_t(memory::OwnerLoad(address + 404, 2)) : guest::LoadU16(address + 404))); }
  void set_locked(uint16_t v) const { if (auto* d = direct()) { d->locked = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 2, v); else guest::StoreU16(address + 404, v); }
  uint32_t locked_ids_address() const { return address + 406; }
  uint8_t* locked_ids() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 406 : guest::Pointer(address + 406); }
  // the kept count (at most the location's cap)
  uint32_t kept_address() const { return address + 606; }
  uint16_t kept_bits() const { if (auto* d = direct()) return d->kept; return uint16_t((memory::IsHandle(address + 606) ? uint16_t(memory::OwnerLoad(address + 606, 2)) : guest::LoadU16(address + 606))); }
  void set_kept_bits(uint16_t v) const { if (auto* d = direct()) { d->kept = v; return; } if (memory::IsHandle(address + 606)) memory::OwnerStore(address + 606, 2, v); else guest::StoreU16(address + 606, v); }
  uint16_t kept() const { if (auto* d = direct()) return d->kept; return uint16_t((memory::IsHandle(address + 606) ? uint16_t(memory::OwnerLoad(address + 606, 2)) : guest::LoadU16(address + 606))); }
  void set_kept(uint16_t v) const { if (auto* d = direct()) { d->kept = v; return; } if (memory::IsHandle(address + 606)) memory::OwnerStore(address + 606, 2, v); else guest::StoreU16(address + 606, v); }
};

// embedded; measure: EventIdList
#pragma pack(push, 1)
struct EventIdListData {
  uint16_t count = 0;  // +0
  uint8_t ids[200] = {};  // +2, guest order
};
#pragma pack(pop)
static_assert(sizeof(EventIdListData) == 202);
inline constexpr memory::LayoutField kEventIdListFields[] = {{0, 2}};
inline constexpr memory::Layout kEventIdListLayout = {kEventIdListFields, 1};

struct EventIdList : View {
  using View::View;
  static constexpr uint32_t kSize = 202;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  EventIdListData* data() const { return static_cast<EventIdListData*>(memory::HandleObject(address)); }
  static EventIdList New() {
    return EventIdList(memory::MakeHandle(new EventIdListData(), sizeof(EventIdListData), "EventIdList", &kEventIdListLayout));
  }
  template <uint32_t Stride = kSize>
  static EventIdList NewArray(uint32_t count) {
    static_assert(sizeof(EventIdListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<EventIdListData*>(::operator new(sizeof(EventIdListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) EventIdListData();
    return EventIdList(memory::MakeHandle(p, sizeof(EventIdListData) * count, "EventIdList", &kEventIdListLayout, sizeof(EventIdListData)));
  }
  EventIdList at(uint32_t i) const { return EventIdList(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  EventIdListData* direct() const { return static_cast<EventIdListData*>(memory::HandleOwn(address, &kEventIdListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kEventIdListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventIdListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t count_address() const { return address + 0; }
  uint16_t count_bits() const { if (auto* d = direct()) return d->count; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_count_bits(uint16_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t count() const { if (auto* d = direct()) return d->count; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_count(uint16_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  // the ids, halfwords (read with load_at)
  uint32_t ids_address() const { return address + 2; }
  uint8_t* ids() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 2 : guest::Pointer(address + 2); }
};

// allocated; measure: new<6080>@RaceEvent::Init
#pragma pack(push, 1)
struct RaceEventPoolBlockData {
  uint16_t pool0_open = 0;  // +0
  uint8_t pad_2[200] = {};
  uint16_t pool0_won = 0;  // +202
  uint8_t pad_204[200] = {};
  uint16_t pool0_locked = 0;  // +404
  uint8_t pad_406[200] = {};
  uint16_t pool0_kept = 0;  // +606
  uint8_t pad_608[1824] = {};
  uint16_t pool4_open = 0;  // +2432
  uint8_t pad_2434[200] = {};
  uint16_t pool4_won = 0;  // +2634
  uint8_t pad_2636[200] = {};
  uint16_t pool4_locked = 0;  // +2836
  uint8_t pad_2838[200] = {};
  uint16_t pool4_kept = 0;  // +3038
  uint8_t pad_3040[3040] = {};
};
#pragma pack(pop)
static_assert(sizeof(RaceEventPoolBlockData) == 6080);
inline constexpr memory::LayoutField kRaceEventPoolBlockFields[] = {{0, 2}, {202, 2}, {404, 2}, {606, 2}, {608, 2}, {810, 2}, {1012, 2}, {1214, 2}, {1216, 2}, {1418, 2}, {1620, 2}, {1822, 2}, {1824, 2}, {2026, 2}, {2228, 2}, {2430, 2}, {2432, 2}, {2634, 2}, {2836, 2}, {3038, 2}, {3040, 2}, {3242, 2}, {3444, 2}, {3646, 2}, {3648, 2}, {3850, 2}, {4052, 2}, {4254, 2}, {4256, 2}, {4458, 2}, {4660, 2}, {4862, 2}, {4864, 2}, {5066, 2}, {5268, 2}, {5470, 2}, {5472, 2}, {5674, 2}, {5876, 2}, {6078, 2}};
inline constexpr memory::Layout kRaceEventPoolBlockLayout = {kRaceEventPoolBlockFields, 40};

struct RaceEventPoolBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 6080;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceEventPoolBlockData* data() const { return static_cast<RaceEventPoolBlockData*>(memory::HandleObject(address)); }
  static RaceEventPoolBlock New() {
    return RaceEventPoolBlock(memory::MakeHandle(new RaceEventPoolBlockData(), sizeof(RaceEventPoolBlockData), "RaceEventPoolBlock", &kRaceEventPoolBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceEventPoolBlock NewArray(uint32_t count) {
    static_assert(sizeof(RaceEventPoolBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceEventPoolBlockData*>(::operator new(sizeof(RaceEventPoolBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceEventPoolBlockData();
    return RaceEventPoolBlock(memory::MakeHandle(p, sizeof(RaceEventPoolBlockData) * count, "RaceEventPoolBlock", &kRaceEventPoolBlockLayout, sizeof(RaceEventPoolBlockData)));
  }
  RaceEventPoolBlock at(uint32_t i) const { return RaceEventPoolBlock(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceEventPoolBlockData* direct() const { return static_cast<RaceEventPoolBlockData*>(memory::HandleOwn(address, &kRaceEventPoolBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceEventPoolBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventPoolBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // ten EventPools, one a location
  template <typename T = EventPools>
  T pool(uint32_t i) const { return T(address + 0 + i * T::kSize); }
  // ten EventPools, one a location
  uint32_t pools_address() const { return address + 0; }
  uint8_t* pools() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  uint32_t pool0_open_address() const { return address + 0; }
  uint16_t pool0_open_bits() const { if (auto* d = direct()) return d->pool0_open; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_pool0_open_bits(uint16_t v) const { if (auto* d = direct()) { d->pool0_open = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t pool0_open() const { if (auto* d = direct()) return d->pool0_open; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_pool0_open(uint16_t v) const { if (auto* d = direct()) { d->pool0_open = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint32_t pool0_won_address() const { return address + 202; }
  uint16_t pool0_won_bits() const { if (auto* d = direct()) return d->pool0_won; return uint16_t((memory::IsHandle(address + 202) ? uint16_t(memory::OwnerLoad(address + 202, 2)) : guest::LoadU16(address + 202))); }
  void set_pool0_won_bits(uint16_t v) const { if (auto* d = direct()) { d->pool0_won = v; return; } if (memory::IsHandle(address + 202)) memory::OwnerStore(address + 202, 2, v); else guest::StoreU16(address + 202, v); }
  uint16_t pool0_won() const { if (auto* d = direct()) return d->pool0_won; return uint16_t((memory::IsHandle(address + 202) ? uint16_t(memory::OwnerLoad(address + 202, 2)) : guest::LoadU16(address + 202))); }
  void set_pool0_won(uint16_t v) const { if (auto* d = direct()) { d->pool0_won = v; return; } if (memory::IsHandle(address + 202)) memory::OwnerStore(address + 202, 2, v); else guest::StoreU16(address + 202, v); }
  uint32_t pool0_locked_address() const { return address + 404; }
  uint16_t pool0_locked_bits() const { if (auto* d = direct()) return d->pool0_locked; return uint16_t((memory::IsHandle(address + 404) ? uint16_t(memory::OwnerLoad(address + 404, 2)) : guest::LoadU16(address + 404))); }
  void set_pool0_locked_bits(uint16_t v) const { if (auto* d = direct()) { d->pool0_locked = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 2, v); else guest::StoreU16(address + 404, v); }
  uint16_t pool0_locked() const { if (auto* d = direct()) return d->pool0_locked; return uint16_t((memory::IsHandle(address + 404) ? uint16_t(memory::OwnerLoad(address + 404, 2)) : guest::LoadU16(address + 404))); }
  void set_pool0_locked(uint16_t v) const { if (auto* d = direct()) { d->pool0_locked = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 2, v); else guest::StoreU16(address + 404, v); }
  uint32_t pool0_kept_address() const { return address + 606; }
  uint16_t pool0_kept_bits() const { if (auto* d = direct()) return d->pool0_kept; return uint16_t((memory::IsHandle(address + 606) ? uint16_t(memory::OwnerLoad(address + 606, 2)) : guest::LoadU16(address + 606))); }
  void set_pool0_kept_bits(uint16_t v) const { if (auto* d = direct()) { d->pool0_kept = v; return; } if (memory::IsHandle(address + 606)) memory::OwnerStore(address + 606, 2, v); else guest::StoreU16(address + 606, v); }
  uint16_t pool0_kept() const { if (auto* d = direct()) return d->pool0_kept; return uint16_t((memory::IsHandle(address + 606) ? uint16_t(memory::OwnerLoad(address + 606, 2)) : guest::LoadU16(address + 606))); }
  void set_pool0_kept(uint16_t v) const { if (auto* d = direct()) { d->pool0_kept = v; return; } if (memory::IsHandle(address + 606)) memory::OwnerStore(address + 606, 2, v); else guest::StoreU16(address + 606, v); }
  uint32_t pool4_open_address() const { return address + 2432; }
  uint16_t pool4_open_bits() const { if (auto* d = direct()) return d->pool4_open; return uint16_t((memory::IsHandle(address + 2432) ? uint16_t(memory::OwnerLoad(address + 2432, 2)) : guest::LoadU16(address + 2432))); }
  void set_pool4_open_bits(uint16_t v) const { if (auto* d = direct()) { d->pool4_open = v; return; } if (memory::IsHandle(address + 2432)) memory::OwnerStore(address + 2432, 2, v); else guest::StoreU16(address + 2432, v); }
  uint16_t pool4_open() const { if (auto* d = direct()) return d->pool4_open; return uint16_t((memory::IsHandle(address + 2432) ? uint16_t(memory::OwnerLoad(address + 2432, 2)) : guest::LoadU16(address + 2432))); }
  void set_pool4_open(uint16_t v) const { if (auto* d = direct()) { d->pool4_open = v; return; } if (memory::IsHandle(address + 2432)) memory::OwnerStore(address + 2432, 2, v); else guest::StoreU16(address + 2432, v); }
  uint32_t pool4_won_address() const { return address + 2634; }
  uint16_t pool4_won_bits() const { if (auto* d = direct()) return d->pool4_won; return uint16_t((memory::IsHandle(address + 2634) ? uint16_t(memory::OwnerLoad(address + 2634, 2)) : guest::LoadU16(address + 2634))); }
  void set_pool4_won_bits(uint16_t v) const { if (auto* d = direct()) { d->pool4_won = v; return; } if (memory::IsHandle(address + 2634)) memory::OwnerStore(address + 2634, 2, v); else guest::StoreU16(address + 2634, v); }
  uint16_t pool4_won() const { if (auto* d = direct()) return d->pool4_won; return uint16_t((memory::IsHandle(address + 2634) ? uint16_t(memory::OwnerLoad(address + 2634, 2)) : guest::LoadU16(address + 2634))); }
  void set_pool4_won(uint16_t v) const { if (auto* d = direct()) { d->pool4_won = v; return; } if (memory::IsHandle(address + 2634)) memory::OwnerStore(address + 2634, 2, v); else guest::StoreU16(address + 2634, v); }
  uint32_t pool4_locked_address() const { return address + 2836; }
  uint16_t pool4_locked_bits() const { if (auto* d = direct()) return d->pool4_locked; return uint16_t((memory::IsHandle(address + 2836) ? uint16_t(memory::OwnerLoad(address + 2836, 2)) : guest::LoadU16(address + 2836))); }
  void set_pool4_locked_bits(uint16_t v) const { if (auto* d = direct()) { d->pool4_locked = v; return; } if (memory::IsHandle(address + 2836)) memory::OwnerStore(address + 2836, 2, v); else guest::StoreU16(address + 2836, v); }
  uint16_t pool4_locked() const { if (auto* d = direct()) return d->pool4_locked; return uint16_t((memory::IsHandle(address + 2836) ? uint16_t(memory::OwnerLoad(address + 2836, 2)) : guest::LoadU16(address + 2836))); }
  void set_pool4_locked(uint16_t v) const { if (auto* d = direct()) { d->pool4_locked = v; return; } if (memory::IsHandle(address + 2836)) memory::OwnerStore(address + 2836, 2, v); else guest::StoreU16(address + 2836, v); }
  uint32_t pool4_kept_address() const { return address + 3038; }
  uint16_t pool4_kept_bits() const { if (auto* d = direct()) return d->pool4_kept; return uint16_t((memory::IsHandle(address + 3038) ? uint16_t(memory::OwnerLoad(address + 3038, 2)) : guest::LoadU16(address + 3038))); }
  void set_pool4_kept_bits(uint16_t v) const { if (auto* d = direct()) { d->pool4_kept = v; return; } if (memory::IsHandle(address + 3038)) memory::OwnerStore(address + 3038, 2, v); else guest::StoreU16(address + 3038, v); }
  uint16_t pool4_kept() const { if (auto* d = direct()) return d->pool4_kept; return uint16_t((memory::IsHandle(address + 3038) ? uint16_t(memory::OwnerLoad(address + 3038, 2)) : guest::LoadU16(address + 3038))); }
  void set_pool4_kept(uint16_t v) const { if (auto* d = direct()) { d->pool4_kept = v; return; } if (memory::IsHandle(address + 3038)) memory::OwnerStore(address + 3038, 2, v); else guest::StoreU16(address + 3038, v); }
};

// embedded; measure: EventCondition, EventCondition::State.r3
struct EventCondition : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the answer wanted
  int16_t answer() const { return int16_t(guest::LoadU16(address + 0)); }
  void set_answer(int16_t v) const { guest::StoreU16(address + 0, uint16_t(v)); }
  uint32_t answer_address() const { return address + 0; }
  // 0 to 67 (the tests in race-event.md)
  int16_t kind() const { return int16_t(guest::LoadU16(address + 2)); }
  void set_kind(int16_t v) const { guest::StoreU16(address + 2, uint16_t(v)); }
  uint32_t kind_address() const { return address + 2; }
  // the arguments: -1 unused
  int32_t arg0() const { return int32_t(guest::LoadU32(address + 4)); }
  void set_arg0(int32_t v) const { guest::StoreU32(address + 4, uint32_t(v)); }
  uint32_t arg0_address() const { return address + 4; }
  int32_t arg1() const { return int32_t(guest::LoadU32(address + 8)); }
  void set_arg1(int32_t v) const { guest::StoreU32(address + 8, uint32_t(v)); }
  uint32_t arg1_address() const { return address + 8; }
  int32_t arg2() const { return int32_t(guest::LoadU32(address + 12)); }
  void set_arg2(int32_t v) const { guest::StoreU32(address + 12, uint32_t(v)); }
  uint32_t arg2_address() const { return address + 12; }
  int32_t arg3() const { return int32_t(guest::LoadU32(address + 16)); }
  void set_arg3(int32_t v) const { guest::StoreU32(address + 16, uint32_t(v)); }
  uint32_t arg3_address() const { return address + 16; }
};

// file; measure: DataTables::Item.ret, Unlock, CarDecal::FromBuild.r4
struct OpponentItem : View {
  using View::View;
  static constexpr uint32_t kSize = 3008;
  uint16_t id() const { return guest::LoadU16(address + 0); }
  void set_id(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // 0 to 47, bits counted by the unlock tests
  uint8_t category() const { return guest::LoadU8(address + 34); }
  void set_category(uint8_t v) const { guest::StoreU8(address + 34, v); }
  uint32_t category_address() const { return address + 34; }
  // 0 to 5 (signed): kind 2 waits on its category's kind 0 items, kind 5 on every other item
  uint8_t kind() const { return guest::LoadU8(address + 35); }
  void set_kind(uint8_t v) const { guest::StoreU8(address + 35, v); }
  uint32_t kind_address() const { return address + 35; }
  // 0 to 9 (signed)
  uint8_t location() const { return guest::LoadU8(address + 36); }
  void set_location(uint8_t v) const { guest::StoreU8(address + 36, v); }
  uint32_t location_address() const { return address + 36; }
  // after +35 (signed); the racer's +3924
  uint8_t sort_key() const { return guest::LoadU8(address + 37); }
  void set_sort_key(uint8_t v) const { guest::StoreU8(address + 37, v); }
  uint32_t sort_key_address() const { return address + 37; }
  // the unlock block, +40 to +87
  uint32_t unlock_address() const { return address + 40; }
  // a mask of modes (read signed)
  uint16_t mode_mask() const { return guest::LoadU16(address + 40); }
  void set_mode_mask(uint16_t v) const { guest::StoreU16(address + 40, v); }
  uint32_t mode_mask_address() const { return address + 40; }
  // 1 to 4
  uint16_t level() const { return guest::LoadU16(address + 42); }
  void set_level(uint16_t v) const { guest::StoreU16(address + 42, v); }
  uint32_t level_address() const { return address + 42; }
  // 1 listed while locked, 2 never
  uint8_t listed() const { return guest::LoadU8(address + 46); }
  void set_listed(uint8_t v) const { guest::StoreU8(address + 46, v); }
  uint32_t listed_address() const { return address + 46; }
  // 0 both conditions, else either
  uint8_t any() const { return guest::LoadU8(address + 47); }
  void set_any(uint8_t v) const { guest::StoreU8(address + 47, v); }
  uint32_t any_address() const { return address + 47; }
  // an EventCondition
  uint32_t condition_0_address() const { return address + 48; }
  // an EventCondition
  uint32_t condition_1_address() const { return address + 68; }
  // 0 to 5 (Race::SetUpRival); 3 has the rivals' rivals followed
  uint16_t race_kind() const { return guest::LoadU16(address + 88); }
  void set_race_kind(uint16_t v) const { guest::StoreU16(address + 88, v); }
  uint32_t race_kind_address() const { return address + 88; }
  // a kind 4 or 5 race's location offset
  uint8_t location_offset() const { return guest::LoadU8(address + 90); }
  void set_location_offset(uint8_t v) const { guest::StoreU8(address + 90, v); }
  uint32_t location_offset_address() const { return address + 90; }
  // 1 counts for RaceEvent::FirstOwned
  uint8_t counts_owned() const { return guest::LoadU8(address + 91); }
  void set_counts_owned(uint8_t v) const { guest::StoreU8(address + 91, v); }
  uint32_t counts_owned_address() const { return address + 91; }
  // three rival item ids (-1 ends), +92 to +96
  uint16_t rival_0() const { return guest::LoadU16(address + 92); }
  void set_rival_0(uint16_t v) const { guest::StoreU16(address + 92, v); }
  uint32_t rival_0_address() const { return address + 92; }
  uint16_t rival_1() const { return guest::LoadU16(address + 94); }
  void set_rival_1(uint16_t v) const { guest::StoreU16(address + 94, v); }
  uint32_t rival_1_address() const { return address + 94; }
  uint16_t rival_2() const { return guest::LoadU16(address + 96); }
  void set_rival_2(uint16_t v) const { guest::StoreU16(address + 96, v); }
  uint32_t rival_2_address() const { return address + 96; }
  // the availability condition (an EventCondition, Items::Available)
  uint32_t available_address() const { return address + 100; }
  // a car build (Car::FromBuild)
  uint32_t build_address() const { return address + 160; }
  // Race::SetUpRival
  uint16_t unknown_180() const { return guest::LoadU16(address + 180); }
  void set_unknown_180(uint16_t v) const { guest::StoreU16(address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  // Race::SetUpRival
  uint8_t unknown_182() const { return guest::LoadU8(address + 182); }
  void set_unknown_182(uint8_t v) const { guest::StoreU8(address + 182, v); }
  uint32_t unknown_182_address() const { return address + 182; }
  // measure fault: the build +0 (CarDecal::FromBuild: a decal entry's word +0)
  float build_gear_factor_0() const { return guest::LoadFloat(address + 0); }
  void set_build_gear_factor_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t build_gear_factor_0_address() const { return address + 0; }
  // measure fault: the build +4 (CarDecal::FromBuild: a decal entry's word +4)
  float build_gear_factor_1() const { return guest::LoadFloat(address + 4); }
  void set_build_gear_factor_1(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t build_gear_factor_1_address() const { return address + 4; }
  // measure fault: the build +8 (CarDecal::FromBuild: a decal entry's word +8)
  float build_gear_factor_2() const { return guest::LoadFloat(address + 8); }
  void set_build_gear_factor_2(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t build_gear_factor_2_address() const { return address + 8; }
  // measure fault: the build +12 (CarDecal::FromBuild: a decal entry's word +12)
  float build_gear_factor_3() const { return guest::LoadFloat(address + 12); }
  void set_build_gear_factor_3(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t build_gear_factor_3_address() const { return address + 12; }
  // measure fault: the build +16 (CarDecal::FromBuild: a decal entry's word +16)
  float build_gear_factor_4() const { return guest::LoadFloat(address + 16); }
  void set_build_gear_factor_4(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t build_gear_factor_4_address() const { return address + 16; }
  // measure fault: the build +20 (CarDecal::FromBuild: a decal entry's word +20)
  float build_gear_factor_5() const { return guest::LoadFloat(address + 20); }
  void set_build_gear_factor_5(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t build_gear_factor_5_address() const { return address + 20; }
  // measure fault: the build +24 (CarDecal::FromBuild: a decal entry's word +24)
  float build_gear_factor_6() const { return guest::LoadFloat(address + 24); }
  void set_build_gear_factor_6(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t build_gear_factor_6_address() const { return address + 24; }
  // measure fault: the build +28, a setting byte as a word (CarDecal::FromBuild: a decal entry's word +28)
  uint32_t build_setting_0() const { return guest::LoadU32(address + 28); }
  void set_build_setting_0(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t build_setting_0_address() const { return address + 28; }
  // measure fault: the build +32, a setting byte as a word (CarDecal::FromBuild: a decal entry's word +32)
  uint32_t build_setting_1() const { return guest::LoadU32(address + 32); }
  void set_build_setting_1(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t build_setting_1_address() const { return address + 32; }
  // measure fault: the build +36, a setting byte as a word
  uint32_t build_setting_2() const { return guest::LoadU32(address + 36); }
  void set_build_setting_2(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t build_setting_2_address() const { return address + 36; }
  // measure fault: the build +40, a setting byte as a word
  uint32_t build_setting_3() const { return guest::LoadU32(address + 40); }
  void set_build_setting_3(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t build_setting_3_address() const { return address + 40; }
  // measure fault: the build +44, a setting byte as a word
  uint32_t build_setting_4() const { return guest::LoadU32(address + 44); }
  void set_build_setting_4(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t build_setting_4_address() const { return address + 44; }
  // measure fault: the build +48, a setting byte as a word
  uint32_t build_setting_5() const { return guest::LoadU32(address + 48); }
  void set_build_setting_5(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t build_setting_5_address() const { return address + 48; }
  // measure fault: the build +52, a setting byte as a word
  uint32_t build_setting_6() const { return guest::LoadU32(address + 52); }
  void set_build_setting_6(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t build_setting_6_address() const { return address + 52; }
  // measure fault: the build +56, a setting byte as a word
  uint32_t build_setting_7() const { return guest::LoadU32(address + 56); }
  void set_build_setting_7(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t build_setting_7_address() const { return address + 56; }
  // measure fault: the build +60, a setting byte as a word
  uint32_t build_setting_8() const { return guest::LoadU32(address + 60); }
  void set_build_setting_8(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t build_setting_8_address() const { return address + 60; }
  // measure fault: the build +64, a setting byte as a word
  uint32_t build_setting_9() const { return guest::LoadU32(address + 64); }
  void set_build_setting_9(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t build_setting_9_address() const { return address + 64; }
  // measure fault: the build +68, a setting byte as a word
  uint32_t build_setting_10() const { return guest::LoadU32(address + 68); }
  void set_build_setting_10(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t build_setting_10_address() const { return address + 68; }
  // measure fault: the build +72, a setting byte as a word
  uint32_t build_setting_11() const { return guest::LoadU32(address + 72); }
  void set_build_setting_11(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t build_setting_11_address() const { return address + 72; }
  // measure fault: the build +76, a setting byte as a word
  uint32_t build_setting_12() const { return guest::LoadU32(address + 76); }
  void set_build_setting_12(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t build_setting_12_address() const { return address + 76; }
  // measure fault: the build +80, a setting byte as a word
  uint32_t build_setting_13() const { return guest::LoadU32(address + 80); }
  void set_build_setting_13(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t build_setting_13_address() const { return address + 80; }
  // measure fault: the build +84, a setting byte as a word
  uint32_t build_setting_14() const { return guest::LoadU32(address + 84); }
  void set_build_setting_14(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t build_setting_14_address() const { return address + 84; }
  // measure fault: the build +88, a setting byte as a word
  uint32_t build_setting_15() const { return guest::LoadU32(address + 88); }
  void set_build_setting_15(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t build_setting_15_address() const { return address + 88; }
  // measure fault: the build +92, a setting byte as a word
  uint32_t build_setting_16() const { return guest::LoadU32(address + 92); }
  void set_build_setting_16(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t build_setting_16_address() const { return address + 92; }
  // measure fault: the build +96, a setting byte as a word
  uint32_t build_setting_17() const { return guest::LoadU32(address + 96); }
  void set_build_setting_17(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t build_setting_17_address() const { return address + 96; }
  // measure fault: the build +100, a setting byte as a word
  uint32_t build_setting_18() const { return guest::LoadU32(address + 100); }
  void set_build_setting_18(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t build_setting_18_address() const { return address + 100; }
  // measure fault: the build +104, a setting byte as a word
  uint32_t build_setting_19() const { return guest::LoadU32(address + 104); }
  void set_build_setting_19(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t build_setting_19_address() const { return address + 104; }
  // measure fault: the build +108, a setting byte as a word
  uint32_t build_setting_20() const { return guest::LoadU32(address + 108); }
  void set_build_setting_20(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t build_setting_20_address() const { return address + 108; }
  // measure fault: the build +112, a setting byte as a word
  uint32_t build_setting_21() const { return guest::LoadU32(address + 112); }
  void set_build_setting_21(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t build_setting_21_address() const { return address + 112; }
  // measure fault: the build +116, a setting byte as a word
  uint32_t build_setting_22() const { return guest::LoadU32(address + 116); }
  void set_build_setting_22(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t build_setting_22_address() const { return address + 116; }
  // measure fault: the build +120, a tuning word
  uint32_t build_tuning_0() const { return guest::LoadU32(address + 120); }
  void set_build_tuning_0(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t build_tuning_0_address() const { return address + 120; }
  // measure fault: the build +124, a tuning word
  uint32_t build_tuning_1() const { return guest::LoadU32(address + 124); }
  void set_build_tuning_1(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t build_tuning_1_address() const { return address + 124; }
  // measure fault: the build +128, a tuning word
  uint32_t build_tuning_2() const { return guest::LoadU32(address + 128); }
  void set_build_tuning_2(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t build_tuning_2_address() const { return address + 128; }
  // measure fault: the build +132, a tuning word
  uint32_t build_tuning_3() const { return guest::LoadU32(address + 132); }
  void set_build_tuning_3(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t build_tuning_3_address() const { return address + 132; }
  // measure fault: the build +136, a tuning word
  uint32_t build_tuning_4() const { return guest::LoadU32(address + 136); }
  void set_build_tuning_4(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t build_tuning_4_address() const { return address + 136; }
  // measure fault: the build +140, a tuning word
  uint32_t build_tuning_5() const { return guest::LoadU32(address + 140); }
  void set_build_tuning_5(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t build_tuning_5_address() const { return address + 140; }
  // measure fault: the build +144, a tuning word
  uint32_t build_tuning_6() const { return guest::LoadU32(address + 144); }
  void set_build_tuning_6(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t build_tuning_6_address() const { return address + 144; }
  // measure fault: the build +148, a tuning word
  uint32_t build_tuning_7() const { return guest::LoadU32(address + 148); }
  void set_build_tuning_7(uint32_t v) const { guest::StoreU32(address + 148, v); }
  uint32_t build_tuning_7_address() const { return address + 148; }
  // measure fault: the build +152, a tuning word
  uint32_t build_tuning_8() const { return guest::LoadU32(address + 152); }
  void set_build_tuning_8(uint32_t v) const { guest::StoreU32(address + 152, v); }
  uint32_t build_tuning_8_address() const { return address + 152; }
  // measure fault: the build +156, a tuning word
  uint32_t build_tuning_9() const { return guest::LoadU32(address + 156); }
  void set_build_tuning_9(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t build_tuning_9_address() const { return address + 156; }
  // measure fault: the build +160, a tuning word
  uint32_t build_tuning_10() const { return guest::LoadU32(address + 160); }
  void set_build_tuning_10(uint32_t v) const { guest::StoreU32(address + 160, v); }
  uint32_t build_tuning_10_address() const { return address + 160; }
  // measure fault: the build +164, a tuning word
  uint32_t build_tuning_11() const { return guest::LoadU32(address + 164); }
  void set_build_tuning_11(uint32_t v) const { guest::StoreU32(address + 164, v); }
  uint32_t build_tuning_11_address() const { return address + 164; }
  // measure fault: the build +168, a tuning word
  uint32_t build_tuning_12() const { return guest::LoadU32(address + 168); }
  void set_build_tuning_12(uint32_t v) const { guest::StoreU32(address + 168, v); }
  uint32_t build_tuning_12_address() const { return address + 168; }
  // measure fault: the build +172, a tuning word
  uint32_t build_tuning_13() const { return guest::LoadU32(address + 172); }
  void set_build_tuning_13(uint32_t v) const { guest::StoreU32(address + 172, v); }
  uint32_t build_tuning_13_address() const { return address + 172; }
  // measure fault: the build +176, a tuning word
  uint32_t build_tuning_14() const { return guest::LoadU32(address + 176); }
  void set_build_tuning_14(uint32_t v) const { guest::StoreU32(address + 176, v); }
  uint32_t build_tuning_14_address() const { return address + 176; }
  // measure fault: the build +180, a tuning word
  uint32_t build_tuning_15() const { return guest::LoadU32(address + 180); }
  void set_build_tuning_15(uint32_t v) const { guest::StoreU32(address + 180, v); }
  uint32_t build_tuning_15_address() const { return address + 180; }
  // measure fault: the build +184, a tuning word
  uint32_t build_tuning_16() const { return guest::LoadU32(address + 184); }
  void set_build_tuning_16(uint32_t v) const { guest::StoreU32(address + 184, v); }
  uint32_t build_tuning_16_address() const { return address + 184; }
  // measure fault: the build +188, a tuning word
  uint32_t build_tuning_17() const { return guest::LoadU32(address + 188); }
  void set_build_tuning_17(uint32_t v) const { guest::StoreU32(address + 188, v); }
  uint32_t build_tuning_17_address() const { return address + 188; }
  // measure fault: the build +192, a tuning word
  uint32_t build_tuning_18() const { return guest::LoadU32(address + 192); }
  void set_build_tuning_18(uint32_t v) const { guest::StoreU32(address + 192, v); }
  uint32_t build_tuning_18_address() const { return address + 192; }
  // measure fault: the build +196, a tuning word
  uint32_t build_tuning_19() const { return guest::LoadU32(address + 196); }
  void set_build_tuning_19(uint32_t v) const { guest::StoreU32(address + 196, v); }
  uint32_t build_tuning_19_address() const { return address + 196; }
  // measure fault: the build +200, a tuning word
  uint32_t build_tuning_20() const { return guest::LoadU32(address + 200); }
  void set_build_tuning_20(uint32_t v) const { guest::StoreU32(address + 200, v); }
  uint32_t build_tuning_20_address() const { return address + 200; }
  // measure fault: the build +204, a tuning word
  uint32_t build_tuning_21() const { return guest::LoadU32(address + 204); }
  void set_build_tuning_21(uint32_t v) const { guest::StoreU32(address + 204, v); }
  uint32_t build_tuning_21_address() const { return address + 204; }
  // measure fault: the build +208, a tuning word
  uint32_t build_tuning_22() const { return guest::LoadU32(address + 208); }
  void set_build_tuning_22(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t build_tuning_22_address() const { return address + 208; }
  // measure fault: the build +212, a tuning word
  uint32_t build_tuning_23() const { return guest::LoadU32(address + 212); }
  void set_build_tuning_23(uint32_t v) const { guest::StoreU32(address + 212, v); }
  uint32_t build_tuning_23_address() const { return address + 212; }
  // measure fault: the build +216, a tuning word
  uint32_t build_tuning_24() const { return guest::LoadU32(address + 216); }
  void set_build_tuning_24(uint32_t v) const { guest::StoreU32(address + 216, v); }
  uint32_t build_tuning_24_address() const { return address + 216; }
  // measure fault: the build +220, a tuning word
  uint32_t build_tuning_25() const { return guest::LoadU32(address + 220); }
  void set_build_tuning_25(uint32_t v) const { guest::StoreU32(address + 220, v); }
  uint32_t build_tuning_25_address() const { return address + 220; }
  // measure fault: the build +224, a tuning word
  uint32_t build_tuning_26() const { return guest::LoadU32(address + 224); }
  void set_build_tuning_26(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t build_tuning_26_address() const { return address + 224; }
  // measure fault: the build +228, a tuning word
  uint32_t build_tuning_27() const { return guest::LoadU32(address + 228); }
  void set_build_tuning_27(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t build_tuning_27_address() const { return address + 228; }
  // measure fault: the build +232, a tuning word
  uint32_t build_tuning_28() const { return guest::LoadU32(address + 232); }
  void set_build_tuning_28(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t build_tuning_28_address() const { return address + 232; }
  // measure fault: the build +236, a tuning word
  uint32_t build_tuning_29() const { return guest::LoadU32(address + 236); }
  void set_build_tuning_29(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t build_tuning_29_address() const { return address + 236; }
  // measure fault: the build +240, a tuning word
  uint32_t build_tuning_30() const { return guest::LoadU32(address + 240); }
  void set_build_tuning_30(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t build_tuning_30_address() const { return address + 240; }
  // measure fault: the build +244, a tuning word
  uint32_t build_tuning_31() const { return guest::LoadU32(address + 244); }
  void set_build_tuning_31(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t build_tuning_31_address() const { return address + 244; }
  // measure fault: the build +248, a tuning word
  uint32_t build_tuning_32() const { return guest::LoadU32(address + 248); }
  void set_build_tuning_32(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t build_tuning_32_address() const { return address + 248; }
  // measure fault: the build +252, a tuning word
  uint32_t build_tuning_33() const { return guest::LoadU32(address + 252); }
  void set_build_tuning_33(uint32_t v) const { guest::StoreU32(address + 252, v); }
  uint32_t build_tuning_33_address() const { return address + 252; }
  // measure fault: the build +256, a tuning word
  uint32_t build_tuning_34() const { return guest::LoadU32(address + 256); }
  void set_build_tuning_34(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t build_tuning_34_address() const { return address + 256; }
  // measure fault: the build +260, a tuning word
  uint32_t build_tuning_35() const { return guest::LoadU32(address + 260); }
  void set_build_tuning_35(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t build_tuning_35_address() const { return address + 260; }
  // measure fault: the build +264, a tuning word
  uint32_t build_tuning_36() const { return guest::LoadU32(address + 264); }
  void set_build_tuning_36(uint32_t v) const { guest::StoreU32(address + 264, v); }
  uint32_t build_tuning_36_address() const { return address + 264; }
  // measure fault: the build +268, a tuning word
  uint32_t build_tuning_37() const { return guest::LoadU32(address + 268); }
  void set_build_tuning_37(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t build_tuning_37_address() const { return address + 268; }
  // measure fault: the build +332 (the car's +1588 entries)
  uint32_t build_entry_word_0() const { return guest::LoadU32(address + 332); }
  void set_build_entry_word_0(uint32_t v) const { guest::StoreU32(address + 332, v); }
  uint32_t build_entry_word_0_address() const { return address + 332; }
  // measure fault: the build +336 (the car's +1588 entries)
  uint32_t build_entry_word_1() const { return guest::LoadU32(address + 336); }
  void set_build_entry_word_1(uint32_t v) const { guest::StoreU32(address + 336, v); }
  uint32_t build_entry_word_1_address() const { return address + 336; }
  // measure fault: the build +340 (the car's +1588 entries)
  uint32_t build_entry_word_2() const { return guest::LoadU32(address + 340); }
  void set_build_entry_word_2(uint32_t v) const { guest::StoreU32(address + 340, v); }
  uint32_t build_entry_word_2_address() const { return address + 340; }
  // measure fault: the build +344 (the car's +1588 entries)
  uint32_t build_entry_word_3() const { return guest::LoadU32(address + 344); }
  void set_build_entry_word_3(uint32_t v) const { guest::StoreU32(address + 344, v); }
  uint32_t build_entry_word_3_address() const { return address + 344; }
  // measure fault: the build +348 (the car's +1588 entries)
  uint32_t build_entry_word_4() const { return guest::LoadU32(address + 348); }
  void set_build_entry_word_4(uint32_t v) const { guest::StoreU32(address + 348, v); }
  uint32_t build_entry_word_4_address() const { return address + 348; }
  // measure fault: the build +2768, the car's +1568 bytes' first word
  uint32_t build_car_1568() const { return guest::LoadU32(address + 2768); }
  void set_build_car_1568(uint32_t v) const { guest::StoreU32(address + 2768, v); }
  uint32_t build_car_1568_address() const { return address + 2768; }
  // measure fault: the build +2788, the car id
  uint32_t build_car_id() const { return guest::LoadU32(address + 2788); }
  void set_build_car_id(uint32_t v) const { guest::StoreU32(address + 2788, v); }
  uint32_t build_car_id_address() const { return address + 2788; }
  // measure fault: the build +2792, the paint
  uint32_t build_paint_0() const { return guest::LoadU32(address + 2792); }
  void set_build_paint_0(uint32_t v) const { guest::StoreU32(address + 2792, v); }
  uint32_t build_paint_0_address() const { return address + 2792; }
  // measure fault: the build +2796, the paint
  uint32_t build_paint_1() const { return guest::LoadU32(address + 2796); }
  void set_build_paint_1(uint32_t v) const { guest::StoreU32(address + 2796, v); }
  uint32_t build_paint_1_address() const { return address + 2796; }
  // measure fault: the build +2800, the paint
  uint32_t build_paint_2() const { return guest::LoadU32(address + 2800); }
  void set_build_paint_2(uint32_t v) const { guest::StoreU32(address + 2800, v); }
  uint32_t build_paint_2_address() const { return address + 2800; }
  // measure fault: the build +2804, the paint
  uint32_t build_paint_3() const { return guest::LoadU32(address + 2804); }
  void set_build_paint_3(uint32_t v) const { guest::StoreU32(address + 2804, v); }
  uint32_t build_paint_3_address() const { return address + 2804; }
  // measure fault: the build +2808, the paint
  uint32_t build_paint_4() const { return guest::LoadU32(address + 2808); }
  void set_build_paint_4(uint32_t v) const { guest::StoreU32(address + 2808, v); }
  uint32_t build_paint_4_address() const { return address + 2808; }
};

// allocated; measure: SaveData::Of.ret, SaveData
struct SaveData : View {
  using View::View;
  static constexpr uint32_t kSize = 129400;
  // 1 for a new game; also TodaysEvent::NextUnlockEntry's and Type2Hub::NextNotice's +0 (probably a list entry joined by the measure)
  uint32_t new_game() const { return guest::LoadU32(address + 0); }
  void set_new_game(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t new_game_address() const { return address + 0; }
  // Settings::Read: OR'd with bit 5 of +6092 into the options' +8 (the sound page's items shown)
  uint32_t shown_items() const { return guest::LoadU32(address + 4); }
  void set_shown_items(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t shown_items_address() const { return address + 4; }
  // the pick's first word (CareerPick.head)
  uint32_t pick_head() const { return guest::LoadU32(address + 8); }
  void set_pick_head(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t pick_head_address() const { return address + 8; }
  // the stats record's +12
  uint16_t wins() const { return guest::LoadU16(address + 60); }
  void set_wins(uint16_t v) const { guest::StoreU16(address + 60, v); }
  uint32_t wins_address() const { return address + 60; }
  // the stats record's +20, a race count
  uint16_t races_20() const { return guest::LoadU16(address + 68); }
  void set_races_20(uint16_t v) const { guest::StoreU16(address + 68, v); }
  uint32_t races_20_address() const { return address + 68; }
  // the stats record's +22, a race count
  uint16_t races_22() const { return guest::LoadU16(address + 70); }
  void set_races_22(uint16_t v) const { guest::StoreU16(address + 70, v); }
  uint32_t races_22_address() const { return address + 70; }
  // the day block's float +4836
  float day_float_4836() const { return guest::LoadFloat(address + 4908); }
  void set_day_float_4836(float v) const { guest::StoreFloat(address + 4908, v); }
  uint32_t day_float_4836_address() const { return address + 4908; }
  // +5844 + 14 x mode: a count, up to 9999 (mode 0's)
  uint16_t mode_counts() const { return guest::LoadU16(address + 5844); }
  void set_mode_counts(uint16_t v) const { guest::StoreU16(address + 5844, v); }
  uint32_t mode_counts_address() const { return address + 5844; }
  // +5886 + 2 x location: counts by location (location 12's)
  uint16_t location_count_12() const { return guest::LoadU16(address + 5910); }
  void set_location_count_12(uint16_t v) const { guest::StoreU16(address + 5910, v); }
  uint32_t location_count_12_address() const { return address + 5910; }
  // the stats record's +5904, a running mean
  float mean_796() const { return guest::LoadFloat(address + 5952); }
  void set_mean_796(float v) const { guest::StoreFloat(address + 5952, v); }
  uint32_t mean_796_address() const { return address + 5952; }
  // the stats record's +5976
  float best_804() const { return guest::LoadFloat(address + 6024); }
  void set_best_804(float v) const { guest::StoreFloat(address + 6024, v); }
  uint32_t best_804_address() const { return address + 6024; }
  // the stats record's +5984
  float best_828_2() const { return guest::LoadFloat(address + 6032); }
  void set_best_828_2(float v) const { guest::StoreFloat(address + 6032, v); }
  uint32_t best_828_2_address() const { return address + 6032; }
  // the stats record's +5996: a best time, in ticks
  uint32_t best_16() const { return guest::LoadU32(address + 6044); }
  void set_best_16(uint32_t v) const { guest::StoreU32(address + 6044, v); }
  uint32_t best_16_address() const { return address + 6044; }
  // probably (CarDealer::Buy adds to it); the stats record's +6004
  uint16_t cars_bought() const { return guest::LoadU16(address + 6052); }
  void set_cars_bought(uint16_t v) const { guest::StoreU16(address + 6052, v); }
  uint32_t cars_bought_address() const { return address + 6052; }
  // probably; the stats record's +6006
  uint16_t cars_sold() const { return guest::LoadU16(address + 6054); }
  void set_cars_sold(uint16_t v) const { guest::StoreU16(address + 6054, v); }
  uint32_t cars_sold_address() const { return address + 6054; }
  uint32_t last_mode() const { return guest::LoadU32(address + 6056); }
  void set_last_mode(uint32_t v) const { guest::StoreU32(address + 6056, v); }
  uint32_t last_mode_address() const { return address + 6056; }
  uint16_t mode_streak() const { return guest::LoadU16(address + 6060); }
  void set_mode_streak(uint16_t v) const { guest::StoreU16(address + 6060, v); }
  uint32_t mode_streak_address() const { return address + 6060; }
  // the stages reached, a bit each
  uint32_t stages() const { return guest::LoadU32(address + 6092); }
  void set_stages(uint32_t v) const { guest::StoreU32(address + 6092, v); }
  uint32_t stages_address() const { return address + 6092; }
  // the day (1 a fixed rival race); from 999 the day tests are met
  uint32_t day() const { return guest::LoadU32(address + 6100); }
  void set_day(uint32_t v) const { guest::StoreU32(address + 6100, v); }
  uint32_t day_address() const { return address + 6100; }
  uint32_t mode() const { return guest::LoadU32(address + 6104); }
  void set_mode(uint32_t v) const { guest::StoreU32(address + 6104, v); }
  uint32_t mode_address() const { return address + 6104; }
  // 14 to 19, 48
  uint32_t location() const { return guest::LoadU32(address + 6108); }
  void set_location(uint32_t v) const { guest::StoreU32(address + 6108, v); }
  uint32_t location_address() const { return address + 6108; }
  uint32_t next_location() const { return guest::LoadU32(address + 6112); }
  void set_next_location(uint32_t v) const { guest::StoreU32(address + 6112, v); }
  uint32_t next_location_address() const { return address + 6112; }
  // 3000 for a new game, at most 9,999,999
  uint32_t money() const { return guest::LoadU32(address + 6116); }
  void set_money(uint32_t v) const { guest::StoreU32(address + 6116, v); }
  uint32_t money_address() const { return address + 6116; }
  // the garage's list (career.toml's CareerGarageList): 4-byte nodes, cars of 1,968 bytes from +6332
  uint32_t garage_address() const { return address + 6120; }
  // the garage list's head (+204)
  uint16_t garage_head() const { return guest::LoadU16(address + 6324); }
  void set_garage_head(uint16_t v) const { guest::StoreU16(address + 6324, v); }
  uint32_t garage_head_address() const { return address + 6324; }
  // the garage list's count (+208)
  uint16_t garage_count() const { return guest::LoadU16(address + 6328); }
  void set_garage_count(uint16_t v) const { guest::StoreU16(address + 6328, v); }
  uint32_t garage_count_address() const { return address + 6328; }
  // 0x19920: the selected garage car, a position in the list
  uint32_t selected_car() const { return guest::LoadU32(address + 104736); }
  void set_selected_car(uint32_t v) const { guest::StoreU32(address + 104736, v); }
  uint32_t selected_car_address() const { return address + 104736; }
  // 0x199F8: 400 records of 12 bytes by item id
  uint32_t item_records_address() const { return address + 104952; }
  // record 0's (+ 12 x id)
  uint16_t item_record_losses() const { return guest::LoadU16(address + 104952); }
  void set_item_record_losses(uint16_t v) const { guest::StoreU16(address + 104952, v); }
  uint32_t item_record_losses_address() const { return address + 104952; }
  // record 0's (+ 12 x id)
  uint16_t item_record_wins() const { return guest::LoadU16(address + 104954); }
  void set_item_record_wins(uint16_t v) const { guest::StoreU16(address + 104954, v); }
  uint32_t item_record_wins_address() const { return address + 104954; }
  // record 0's (+ 12 x id)
  uint16_t item_record_outcome3() const { return guest::LoadU16(address + 104956); }
  void set_item_record_outcome3(uint16_t v) const { guest::StoreU16(address + 104956, v); }
  uint32_t item_record_outcome3_address() const { return address + 104956; }
  // record 0's +6, probably the rival's wins (+ 12 x id)
  uint16_t item_record_rival_wins() const { return guest::LoadU16(address + 104958); }
  void set_item_record_rival_wins(uint16_t v) const { guest::StoreU16(address + 104958, v); }
  uint32_t item_record_rival_wins_address() const { return address + 104958; }
  // record 0's +8 (+ 12 x id)
  uint32_t item_record_word() const { return guest::LoadU32(address + 104960); }
  void set_item_record_word(uint32_t v) const { guest::StoreU32(address + 104960, v); }
  uint32_t item_record_word_address() const { return address + 104960; }
  // 0x1ACE4: a 400-bit set
  uint32_t item_flags_address() const { return address + 109796; }
  // 0x1AD10: the set's word 11 (kind 64 tests 0x20)
  uint32_t item_flags_word_11() const { return guest::LoadU32(address + 109840); }
  void set_item_flags_word_11(uint32_t v) const { guest::StoreU32(address + 109840, v); }
  uint32_t item_flags_word_11_address() const { return address + 109840; }
  // 0x1C778: 64 12-byte entries {0, table, entry}, the unlocks to show
  uint32_t unlocks_shown_address() const { return address + 116600; }
  // entry 0's (+ 12 x i)
  uint32_t unlock_shown_word() const { return guest::LoadU32(address + 116600); }
  void set_unlock_shown_word(uint32_t v) const { guest::StoreU32(address + 116600, v); }
  uint32_t unlock_shown_word_address() const { return address + 116600; }
  // entry 0's (+ 12 x i)
  uint32_t unlock_shown_table() const { return guest::LoadU32(address + 116604); }
  void set_unlock_shown_table(uint32_t v) const { guest::StoreU32(address + 116604, v); }
  uint32_t unlock_shown_table_address() const { return address + 116604; }
  // entry 0's (+ 12 x i)
  uint32_t unlock_shown_entry() const { return guest::LoadU32(address + 116608); }
  void set_unlock_shown_entry(uint32_t v) const { guest::StoreU32(address + 116608, v); }
  uint32_t unlock_shown_entry_address() const { return address + 116608; }
  // 0x1CA78
  uint32_t unlocks_shown_count() const { return guest::LoadU32(address + 117368); }
  void set_unlocks_shown_count(uint32_t v) const { guest::StoreU32(address + 117368, v); }
  uint32_t unlocks_shown_count_address() const { return address + 117368; }
  // 0x1CA84: 12-byte entries {0, 8, index}
  uint32_t rewards_shown_address() const { return address + 117380; }
  // entry 0's (+ 12 x i)
  uint32_t reward_shown_word() const { return guest::LoadU32(address + 117380); }
  void set_reward_shown_word(uint32_t v) const { guest::StoreU32(address + 117380, v); }
  uint32_t reward_shown_word_address() const { return address + 117380; }
  // 0x1D504
  uint32_t rewards_shown_count() const { return guest::LoadU32(address + 120068); }
  void set_rewards_shown_count(uint32_t v) const { guest::StoreU32(address + 120068, v); }
  uint32_t rewards_shown_count_address() const { return address + 120068; }
  // 0x1D524: a movie to play after the day's results, -1 none
  uint32_t movie_after_results() const { return guest::LoadU32(address + 120100); }
  void set_movie_after_results(uint32_t v) const { guest::StoreU32(address + 120100, v); }
  uint32_t movie_after_results_address() const { return address + 120100; }
  // 0x1F85C: the options
  uint32_t options_address() const { return address + 129116; }
  // the options' +0 to +12: the four HUD options
  uint32_t hud_option_0() const { return guest::LoadU32(address + 129116); }
  void set_hud_option_0(uint32_t v) const { guest::StoreU32(address + 129116, v); }
  uint32_t hud_option_0_address() const { return address + 129116; }
  uint32_t hud_option_1() const { return guest::LoadU32(address + 129120); }
  void set_hud_option_1(uint32_t v) const { guest::StoreU32(address + 129120, v); }
  uint32_t hud_option_1_address() const { return address + 129120; }
  uint32_t hud_option_2() const { return guest::LoadU32(address + 129124); }
  void set_hud_option_2(uint32_t v) const { guest::StoreU32(address + 129124, v); }
  uint32_t hud_option_2_address() const { return address + 129124; }
  uint32_t hud_option_3() const { return guest::LoadU32(address + 129128); }
  void set_hud_option_3(uint32_t v) const { guest::StoreU32(address + 129128, v); }
  uint32_t hud_option_3_address() const { return address + 129128; }
  // the options' +20: 1 when the profile's +632 is 0 (SaveScreen::StoreSettings)
  uint32_t profile_option() const { return guest::LoadU32(address + 129136); }
  void set_profile_option(uint32_t v) const { guest::StoreU32(address + 129136, v); }
  uint32_t profile_option_address() const { return address + 129136; }
  // the options' +24: the camera setting (0 view 2, 1 view 1, 2 view 0)
  uint32_t camera_view() const { return guest::LoadU32(address + 129140); }
  void set_camera_view(uint32_t v) const { guest::StoreU32(address + 129140, v); }
  uint32_t camera_view_address() const { return address + 129140; }
  // the options' +28, 0 to 15 (race-screens.md's order; options-screen.md names them the other way round)
  uint8_t effects_volume() const { return guest::LoadU8(address + 129144); }
  void set_effects_volume(uint8_t v) const { guest::StoreU8(address + 129144, v); }
  uint32_t effects_volume_address() const { return address + 129144; }
  // the options' +29, 0 to 15; kind 66 tests it above 0 (signed)
  uint8_t music_volume() const { return guest::LoadU8(address + 129145); }
  void set_music_volume(uint8_t v) const { guest::StoreU8(address + 129145, v); }
  uint32_t music_volume_address() const { return address + 129145; }
  // the options' +36 to +41: settings bytes (+36 to +38 values -10 to 10)
  uint8_t setting_0() const { return guest::LoadU8(address + 129152); }
  void set_setting_0(uint8_t v) const { guest::StoreU8(address + 129152, v); }
  uint32_t setting_0_address() const { return address + 129152; }
  uint8_t setting_1() const { return guest::LoadU8(address + 129153); }
  void set_setting_1(uint8_t v) const { guest::StoreU8(address + 129153, v); }
  uint32_t setting_1_address() const { return address + 129153; }
  uint8_t setting_2() const { return guest::LoadU8(address + 129154); }
  void set_setting_2(uint8_t v) const { guest::StoreU8(address + 129154, v); }
  uint32_t setting_2_address() const { return address + 129154; }
  uint8_t setting_3() const { return guest::LoadU8(address + 129155); }
  void set_setting_3(uint8_t v) const { guest::StoreU8(address + 129155, v); }
  uint32_t setting_3_address() const { return address + 129155; }
  uint8_t setting_4() const { return guest::LoadU8(address + 129156); }
  void set_setting_4(uint8_t v) const { guest::StoreU8(address + 129156, v); }
  uint32_t setting_4_address() const { return address + 129156; }
  uint8_t setting_5() const { return guest::LoadU8(address + 129157); }
  void set_setting_5(uint8_t v) const { guest::StoreU8(address + 129157, v); }
  uint32_t setting_5_address() const { return address + 129157; }
  // the second distance total, beside +4908 (SaveData::AddDistance; race-world's report)
  float distance_total_2() const { return guest::LoadFloat(address + 6040); }
  void set_distance_total_2(float v) const { guest::StoreFloat(address + 6040, v); }
  uint32_t distance_total_2_address() const { return address + 6040; }
};

// embedded; measure: sub_820C4550.ret, SceneModelRecord::SetWheels.ret
struct CarSpecCopies : View {
  using View::View;
  static constexpr uint32_t kSize = 1380;
  // block 2's car id
  uint32_t block2_id_address() const { return address + 0; }
  // cc
  uint32_t displacement() const { return guest::LoadU32(address + 8); }
  void set_displacement(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t displacement_address() const { return address + 8; }
  // probably (+12 to +24: the power and torque and their rpm, SBX-Recomp's reading)
  float power() const { return guest::LoadFloat(address + 12); }
  void set_power(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t power_address() const { return address + 12; }
  // probably
  float power_rpm() const { return guest::LoadFloat(address + 16); }
  void set_power_rpm(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t power_rpm_address() const { return address + 16; }
  // probably; also the engine stage's +20 through SetWheels' join (sub_820CE7F0)
  float torque() const { return guest::LoadFloat(address + 20); }
  void set_torque(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t torque_address() const { return address + 20; }
  // probably; also the engine stage's +24 (sub_820CE600)
  float torque_rpm() const { return guest::LoadFloat(address + 24); }
  void set_torque_rpm(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t torque_rpm_address() const { return address + 24; }
  // +28 to +60 0.0 at load; also the engine stage's +28 (sub_820CE600)
  float zeroed_28() const { return guest::LoadFloat(address + 28); }
  void set_zeroed_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t zeroed_28_address() const { return address + 28; }
  // cylinders for the tuning (7: two rotors)
  uint8_t engine_type() const { return guest::LoadU8(address + 68); }
  void set_engine_type(uint8_t v) const { guest::StoreU8(address + 68, v); }
  uint32_t engine_type_address() const { return address + 68; }
  // 1 for turbo
  uint8_t turbo() const { return guest::LoadU8(address + 69); }
  void set_turbo(uint8_t v) const { guest::StoreU8(address + 69, v); }
  uint32_t turbo_address() const { return address + 69; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  // probably (kind 36 tests 1990 to 1999)
  uint32_t model_year() const { return guest::LoadU32(address + 88); }
  void set_model_year(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t model_year_address() const { return address + 88; }
  // kind 37 tests it is 4
  uint32_t spec_92() const { return guest::LoadU32(address + 92); }
  void set_spec_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t spec_92_address() const { return address + 92; }
  // kind 37 tests it is 0
  uint32_t spec_96() const { return guest::LoadU32(address + 96); }
  void set_spec_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t spec_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // kind 49: 0 wants the tuning's +108 set, else not negative
  uint32_t spec_120() const { return guest::LoadU32(address + 120); }
  void set_spec_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t spec_120_address() const { return address + 120; }
  // kind 32 tests it
  uint8_t spec_byte_129() const { return guest::LoadU8(address + 129); }
  void set_spec_byte_129(uint8_t v) const { guest::StoreU8(address + 129, v); }
  uint32_t spec_byte_129_address() const { return address + 129; }
  uint8_t unknown_130() const { return guest::LoadU8(address + 130); }
  void set_unknown_130(uint8_t v) const { guest::StoreU8(address + 130, v); }
  uint32_t unknown_130_address() const { return address + 130; }
  // the car list (0 road cars; nonzero takes car 0's aero parts)
  uint8_t list_kind() const { return guest::LoadU8(address + 131); }
  void set_list_kind(uint8_t v) const { guest::StoreU8(address + 131, v); }
  uint32_t list_kind_address() const { return address + 131; }
  uint32_t unknown_232() const { return guest::LoadU32(address + 232); }
  void set_unknown_232(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  // SceneModelRecord::SetWheels: the record's +4124 when not 0
  uint32_t wheel_236() const { return guest::LoadU32(address + 236); }
  void set_wheel_236(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t wheel_236_address() const { return address + 236; }
  // +300 to +312 read by CarData::ApplySettings (sub_820C05C8)
  float setting_300() const { return guest::LoadFloat(address + 300); }
  void set_setting_300(float v) const { guest::StoreFloat(address + 300, v); }
  uint32_t setting_300_address() const { return address + 300; }
  float setting_304() const { return guest::LoadFloat(address + 304); }
  void set_setting_304(float v) const { guest::StoreFloat(address + 304, v); }
  uint32_t setting_304_address() const { return address + 304; }
  float setting_308() const { return guest::LoadFloat(address + 308); }
  void set_setting_308(float v) const { guest::StoreFloat(address + 308, v); }
  uint32_t setting_308_address() const { return address + 308; }
  float setting_312() const { return guest::LoadFloat(address + 312); }
  void set_setting_312(float v) const { guest::StoreFloat(address + 312, v); }
  uint32_t setting_312_address() const { return address + 312; }
  // block 1 +16
  float b1_inertia_factor_x() const { return guest::LoadFloat(address + 368); }
  void set_b1_inertia_factor_x(float v) const { guest::StoreFloat(address + 368, v); }
  uint32_t b1_inertia_factor_x_address() const { return address + 368; }
  // block 1 +20
  float b1_inertia_factor_y() const { return guest::LoadFloat(address + 372); }
  void set_b1_inertia_factor_y(float v) const { guest::StoreFloat(address + 372, v); }
  uint32_t b1_inertia_factor_y_address() const { return address + 372; }
  // block 1 +24
  float b1_inertia_factor_z() const { return guest::LoadFloat(address + 376); }
  void set_b1_inertia_factor_z(float v) const { guest::StoreFloat(address + 376, v); }
  uint32_t b1_inertia_factor_z_address() const { return address + 376; }
  // block 1 +32
  float b1_length() const { return guest::LoadFloat(address + 384); }
  void set_b1_length(float v) const { guest::StoreFloat(address + 384, v); }
  uint32_t b1_length_address() const { return address + 384; }
  // block 1 +36
  float b1_height() const { return guest::LoadFloat(address + 388); }
  void set_b1_height(float v) const { guest::StoreFloat(address + 388, v); }
  uint32_t b1_height_address() const { return address + 388; }
  // block 1 +40
  float b1_width() const { return guest::LoadFloat(address + 392); }
  void set_b1_width(float v) const { guest::StoreFloat(address + 392, v); }
  uint32_t b1_width_address() const { return address + 392; }
  // block 1 +48
  float b1_center_of_mass_x() const { return guest::LoadFloat(address + 400); }
  void set_b1_center_of_mass_x(float v) const { guest::StoreFloat(address + 400, v); }
  uint32_t b1_center_of_mass_x_address() const { return address + 400; }
  // block 1 +52
  float b1_center_of_mass_y() const { return guest::LoadFloat(address + 404); }
  void set_b1_center_of_mass_y(float v) const { guest::StoreFloat(address + 404, v); }
  uint32_t b1_center_of_mass_y_address() const { return address + 404; }
  // block 1 +56
  float b1_center_of_mass_z() const { return guest::LoadFloat(address + 408); }
  void set_b1_center_of_mass_z(float v) const { guest::StoreFloat(address + 408, v); }
  uint32_t b1_center_of_mass_z_address() const { return address + 408; }
  // block 1 +64
  float b1_box_min_x() const { return guest::LoadFloat(address + 416); }
  void set_b1_box_min_x(float v) const { guest::StoreFloat(address + 416, v); }
  uint32_t b1_box_min_x_address() const { return address + 416; }
  // block 1 +68
  float b1_box_min_y() const { return guest::LoadFloat(address + 420); }
  void set_b1_box_min_y(float v) const { guest::StoreFloat(address + 420, v); }
  uint32_t b1_box_min_y_address() const { return address + 420; }
  // block 1 +72
  float b1_box_min_z() const { return guest::LoadFloat(address + 424); }
  void set_b1_box_min_z(float v) const { guest::StoreFloat(address + 424, v); }
  uint32_t b1_box_min_z_address() const { return address + 424; }
  // block 1 +80
  float b1_box_max_x() const { return guest::LoadFloat(address + 432); }
  void set_b1_box_max_x(float v) const { guest::StoreFloat(address + 432, v); }
  uint32_t b1_box_max_x_address() const { return address + 432; }
  // block 1 +84
  float b1_box_max_y() const { return guest::LoadFloat(address + 436); }
  void set_b1_box_max_y(float v) const { guest::StoreFloat(address + 436, v); }
  uint32_t b1_box_max_y_address() const { return address + 436; }
  // block 1 +88
  float b1_box_max_z() const { return guest::LoadFloat(address + 440); }
  void set_b1_box_max_z(float v) const { guest::StoreFloat(address + 440, v); }
  uint32_t b1_box_max_z_address() const { return address + 440; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_0() const { return guest::LoadFloat(address + 448); }
  void set_b1_matrix_96_0(float v) const { guest::StoreFloat(address + 448, v); }
  uint32_t b1_matrix_96_0_address() const { return address + 448; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_1() const { return guest::LoadFloat(address + 452); }
  void set_b1_matrix_96_1(float v) const { guest::StoreFloat(address + 452, v); }
  uint32_t b1_matrix_96_1_address() const { return address + 452; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_2() const { return guest::LoadFloat(address + 456); }
  void set_b1_matrix_96_2(float v) const { guest::StoreFloat(address + 456, v); }
  uint32_t b1_matrix_96_2_address() const { return address + 456; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_3() const { return guest::LoadFloat(address + 460); }
  void set_b1_matrix_96_3(float v) const { guest::StoreFloat(address + 460, v); }
  uint32_t b1_matrix_96_3_address() const { return address + 460; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_4() const { return guest::LoadFloat(address + 464); }
  void set_b1_matrix_96_4(float v) const { guest::StoreFloat(address + 464, v); }
  uint32_t b1_matrix_96_4_address() const { return address + 464; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_5() const { return guest::LoadFloat(address + 468); }
  void set_b1_matrix_96_5(float v) const { guest::StoreFloat(address + 468, v); }
  uint32_t b1_matrix_96_5_address() const { return address + 468; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_10() const { return guest::LoadFloat(address + 488); }
  void set_b1_matrix_96_10(float v) const { guest::StoreFloat(address + 488, v); }
  uint32_t b1_matrix_96_10_address() const { return address + 488; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_11() const { return guest::LoadFloat(address + 492); }
  void set_b1_matrix_96_11(float v) const { guest::StoreFloat(address + 492, v); }
  uint32_t b1_matrix_96_11_address() const { return address + 492; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_12() const { return guest::LoadFloat(address + 496); }
  void set_b1_matrix_96_12(float v) const { guest::StoreFloat(address + 496, v); }
  uint32_t b1_matrix_96_12_address() const { return address + 496; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_13() const { return guest::LoadFloat(address + 500); }
  void set_b1_matrix_96_13(float v) const { guest::StoreFloat(address + 500, v); }
  uint32_t b1_matrix_96_13_address() const { return address + 500; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_14() const { return guest::LoadFloat(address + 504); }
  void set_b1_matrix_96_14(float v) const { guest::StoreFloat(address + 504, v); }
  uint32_t b1_matrix_96_14_address() const { return address + 504; }
  // block 1 +96's 16 floats (probably a matrix)
  float b1_matrix_96_15() const { return guest::LoadFloat(address + 508); }
  void set_b1_matrix_96_15(float v) const { guest::StoreFloat(address + 508, v); }
  uint32_t b1_matrix_96_15_address() const { return address + 508; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_0() const { return guest::LoadFloat(address + 512); }
  void set_b1_matrix_160_0(float v) const { guest::StoreFloat(address + 512, v); }
  uint32_t b1_matrix_160_0_address() const { return address + 512; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_1() const { return guest::LoadFloat(address + 516); }
  void set_b1_matrix_160_1(float v) const { guest::StoreFloat(address + 516, v); }
  uint32_t b1_matrix_160_1_address() const { return address + 516; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_2() const { return guest::LoadFloat(address + 520); }
  void set_b1_matrix_160_2(float v) const { guest::StoreFloat(address + 520, v); }
  uint32_t b1_matrix_160_2_address() const { return address + 520; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_3() const { return guest::LoadFloat(address + 524); }
  void set_b1_matrix_160_3(float v) const { guest::StoreFloat(address + 524, v); }
  uint32_t b1_matrix_160_3_address() const { return address + 524; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_4() const { return guest::LoadFloat(address + 528); }
  void set_b1_matrix_160_4(float v) const { guest::StoreFloat(address + 528, v); }
  uint32_t b1_matrix_160_4_address() const { return address + 528; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_5() const { return guest::LoadFloat(address + 532); }
  void set_b1_matrix_160_5(float v) const { guest::StoreFloat(address + 532, v); }
  uint32_t b1_matrix_160_5_address() const { return address + 532; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_10() const { return guest::LoadFloat(address + 552); }
  void set_b1_matrix_160_10(float v) const { guest::StoreFloat(address + 552, v); }
  uint32_t b1_matrix_160_10_address() const { return address + 552; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_11() const { return guest::LoadFloat(address + 556); }
  void set_b1_matrix_160_11(float v) const { guest::StoreFloat(address + 556, v); }
  uint32_t b1_matrix_160_11_address() const { return address + 556; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_12() const { return guest::LoadFloat(address + 560); }
  void set_b1_matrix_160_12(float v) const { guest::StoreFloat(address + 560, v); }
  uint32_t b1_matrix_160_12_address() const { return address + 560; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_13() const { return guest::LoadFloat(address + 564); }
  void set_b1_matrix_160_13(float v) const { guest::StoreFloat(address + 564, v); }
  uint32_t b1_matrix_160_13_address() const { return address + 564; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_14() const { return guest::LoadFloat(address + 568); }
  void set_b1_matrix_160_14(float v) const { guest::StoreFloat(address + 568, v); }
  uint32_t b1_matrix_160_14_address() const { return address + 568; }
  // block 1 +160's 16 floats (probably a matrix)
  float b1_matrix_160_15() const { return guest::LoadFloat(address + 572); }
  void set_b1_matrix_160_15(float v) const { guest::StoreFloat(address + 572, v); }
  uint32_t b1_matrix_160_15_address() const { return address + 572; }
  // block 1 +224's 5 floats
  float b1_floats_224_0() const { return guest::LoadFloat(address + 576); }
  void set_b1_floats_224_0(float v) const { guest::StoreFloat(address + 576, v); }
  uint32_t b1_floats_224_0_address() const { return address + 576; }
  // block 1 +224's 5 floats
  float b1_floats_224_1() const { return guest::LoadFloat(address + 580); }
  void set_b1_floats_224_1(float v) const { guest::StoreFloat(address + 580, v); }
  uint32_t b1_floats_224_1_address() const { return address + 580; }
  // block 1 +224's 5 floats
  float b1_floats_224_2() const { return guest::LoadFloat(address + 584); }
  void set_b1_floats_224_2(float v) const { guest::StoreFloat(address + 584, v); }
  uint32_t b1_floats_224_2_address() const { return address + 584; }
  // block 1 +224's 5 floats
  float b1_floats_224_3() const { return guest::LoadFloat(address + 588); }
  void set_b1_floats_224_3(float v) const { guest::StoreFloat(address + 588, v); }
  uint32_t b1_floats_224_3_address() const { return address + 588; }
  // block 1 +224's 5 floats
  float b1_floats_224_4() const { return guest::LoadFloat(address + 592); }
  void set_b1_floats_224_4(float v) const { guest::StoreFloat(address + 592, v); }
  uint32_t b1_floats_224_4_address() const { return address + 592; }
  // block 1 +244's 5 floats
  float b1_floats_244_0() const { return guest::LoadFloat(address + 596); }
  void set_b1_floats_244_0(float v) const { guest::StoreFloat(address + 596, v); }
  uint32_t b1_floats_244_0_address() const { return address + 596; }
  // block 1 +244's 5 floats
  float b1_floats_244_1() const { return guest::LoadFloat(address + 600); }
  void set_b1_floats_244_1(float v) const { guest::StoreFloat(address + 600, v); }
  uint32_t b1_floats_244_1_address() const { return address + 600; }
  // block 1 +244's 5 floats
  float b1_floats_244_2() const { return guest::LoadFloat(address + 604); }
  void set_b1_floats_244_2(float v) const { guest::StoreFloat(address + 604, v); }
  uint32_t b1_floats_244_2_address() const { return address + 604; }
  // block 1 +244's 5 floats
  float b1_floats_244_3() const { return guest::LoadFloat(address + 608); }
  void set_b1_floats_244_3(float v) const { guest::StoreFloat(address + 608, v); }
  uint32_t b1_floats_244_3_address() const { return address + 608; }
  // block 1 +244's 5 floats
  float b1_floats_244_4() const { return guest::LoadFloat(address + 612); }
  void set_b1_floats_244_4(float v) const { guest::StoreFloat(address + 612, v); }
  uint32_t b1_floats_244_4_address() const { return address + 612; }
  // block 1 +264's 92 bytes
  float b1_record_264_0() const { return guest::LoadFloat(address + 616); }
  void set_b1_record_264_0(float v) const { guest::StoreFloat(address + 616, v); }
  uint32_t b1_record_264_0_address() const { return address + 616; }
  // block 1 +264's 92 bytes
  float b1_record_264_1() const { return guest::LoadFloat(address + 620); }
  void set_b1_record_264_1(float v) const { guest::StoreFloat(address + 620, v); }
  uint32_t b1_record_264_1_address() const { return address + 620; }
  // block 1 +264's 92 bytes
  float b1_record_264_2() const { return guest::LoadFloat(address + 624); }
  void set_b1_record_264_2(float v) const { guest::StoreFloat(address + 624, v); }
  uint32_t b1_record_264_2_address() const { return address + 624; }
  // block 1 +264's 92 bytes
  float b1_record_264_4() const { return guest::LoadFloat(address + 632); }
  void set_b1_record_264_4(float v) const { guest::StoreFloat(address + 632, v); }
  uint32_t b1_record_264_4_address() const { return address + 632; }
  // block 1 +264's 92 bytes
  float b1_record_264_5() const { return guest::LoadFloat(address + 636); }
  void set_b1_record_264_5(float v) const { guest::StoreFloat(address + 636, v); }
  uint32_t b1_record_264_5_address() const { return address + 636; }
  // block 1 +264's 92 bytes
  float b1_record_264_6() const { return guest::LoadFloat(address + 640); }
  void set_b1_record_264_6(float v) const { guest::StoreFloat(address + 640, v); }
  uint32_t b1_record_264_6_address() const { return address + 640; }
  // block 1 +264's 92 bytes
  float b1_record_264_8() const { return guest::LoadFloat(address + 648); }
  void set_b1_record_264_8(float v) const { guest::StoreFloat(address + 648, v); }
  uint32_t b1_record_264_8_address() const { return address + 648; }
  // block 1 +264's 92 bytes
  float b1_record_264_9() const { return guest::LoadFloat(address + 652); }
  void set_b1_record_264_9(float v) const { guest::StoreFloat(address + 652, v); }
  uint32_t b1_record_264_9_address() const { return address + 652; }
  // block 1 +264's 92 bytes
  float b1_record_264_10() const { return guest::LoadFloat(address + 656); }
  void set_b1_record_264_10(float v) const { guest::StoreFloat(address + 656, v); }
  uint32_t b1_record_264_10_address() const { return address + 656; }
  // block 1 +264's 92 bytes
  float b1_record_264_11() const { return guest::LoadFloat(address + 660); }
  void set_b1_record_264_11(float v) const { guest::StoreFloat(address + 660, v); }
  uint32_t b1_record_264_11_address() const { return address + 660; }
  // block 1 +264's 92 bytes
  float b1_record_264_12() const { return guest::LoadFloat(address + 664); }
  void set_b1_record_264_12(float v) const { guest::StoreFloat(address + 664, v); }
  uint32_t b1_record_264_12_address() const { return address + 664; }
  // block 1 +264's 92 bytes
  float b1_record_264_13() const { return guest::LoadFloat(address + 668); }
  void set_b1_record_264_13(float v) const { guest::StoreFloat(address + 668, v); }
  uint32_t b1_record_264_13_address() const { return address + 668; }
  // block 1 +264's 92 bytes
  float b1_record_264_14() const { return guest::LoadFloat(address + 672); }
  void set_b1_record_264_14(float v) const { guest::StoreFloat(address + 672, v); }
  uint32_t b1_record_264_14_address() const { return address + 672; }
  // block 1 +264's 92 bytes
  float b1_record_264_15() const { return guest::LoadFloat(address + 676); }
  void set_b1_record_264_15(float v) const { guest::StoreFloat(address + 676, v); }
  uint32_t b1_record_264_15_address() const { return address + 676; }
  // block 1 +356: 0 front, 1 rear, 2 and up all-wheel
  uint32_t b1_drive_type() const { return guest::LoadU32(address + 708); }
  void set_b1_drive_type(uint32_t v) const { guest::StoreU32(address + 708, v); }
  uint32_t b1_drive_type_address() const { return address + 708; }
  // block 1 +360: an index into 0x8260181C
  uint32_t b1_drive_record() const { return guest::LoadU32(address + 712); }
  void set_b1_drive_record(uint32_t v) const { guest::StoreU32(address + 712, v); }
  uint32_t b1_drive_record_address() const { return address + 712; }
  // block 1 +364
  float b1_front_torque_share() const { return guest::LoadFloat(address + 716); }
  void set_b1_front_torque_share(float v) const { guest::StoreFloat(address + 716, v); }
  uint32_t b1_front_torque_share_address() const { return address + 716; }
  // block 1 +368
  uint32_t b1_gear_count() const { return guest::LoadU32(address + 720); }
  void set_b1_gear_count(uint32_t v) const { guest::StoreU32(address + 720, v); }
  uint32_t b1_gear_count_address() const { return address + 720; }
  // block 1 +372
  float b1_final_drive() const { return guest::LoadFloat(address + 724); }
  void set_b1_final_drive(float v) const { guest::StoreFloat(address + 724, v); }
  uint32_t b1_final_drive_address() const { return address + 724; }
  // block 1 +376 (its floats +528 to +636)
  float b1_float_376() const { return guest::LoadFloat(address + 728); }
  void set_b1_float_376(float v) const { guest::StoreFloat(address + 728, v); }
  uint32_t b1_float_376_address() const { return address + 728; }
  // block 1 +380 (its floats +528 to +636)
  float b1_float_380() const { return guest::LoadFloat(address + 732); }
  void set_b1_float_380(float v) const { guest::StoreFloat(address + 732, v); }
  uint32_t b1_float_380_address() const { return address + 732; }
  // block 1 +384: the reverse ratio, then up to 8 forward (+384, 9 floats)
  float b1_gear_ratio_reverse() const { return guest::LoadFloat(address + 736); }
  void set_b1_gear_ratio_reverse(float v) const { guest::StoreFloat(address + 736, v); }
  uint32_t b1_gear_ratio_reverse_address() const { return address + 736; }
  // block 1 +420: a per-gear table's first (9 floats)
  float b1_gear_table_420() const { return guest::LoadFloat(address + 772); }
  void set_b1_gear_table_420(float v) const { guest::StoreFloat(address + 772, v); }
  uint32_t b1_gear_table_420_address() const { return address + 772; }
  // block 1 +456: a per-gear table's first (9 floats)
  float b1_gear_table_456() const { return guest::LoadFloat(address + 808); }
  void set_b1_gear_table_456(float v) const { guest::StoreFloat(address + 808, v); }
  uint32_t b1_gear_table_456_address() const { return address + 808; }
  // block 1 +492: the shift times' first (9 floats)
  float b1_shift_time_0() const { return guest::LoadFloat(address + 844); }
  void set_b1_shift_time_0(float v) const { guest::StoreFloat(address + 844, v); }
  uint32_t b1_shift_time_0_address() const { return address + 844; }
  // block 1 +532
  float b1_mass() const { return guest::LoadFloat(address + 884); }
  void set_b1_mass(float v) const { guest::StoreFloat(address + 884, v); }
  uint32_t b1_mass_address() const { return address + 884; }
  // block 1 +536
  float b1_wheelbase() const { return guest::LoadFloat(address + 888); }
  void set_b1_wheelbase(float v) const { guest::StoreFloat(address + 888, v); }
  uint32_t b1_wheelbase_address() const { return address + 888; }
  // block 1 +540
  float b1_front_track() const { return guest::LoadFloat(address + 892); }
  void set_b1_front_track(float v) const { guest::StoreFloat(address + 892, v); }
  uint32_t b1_front_track_address() const { return address + 892; }
  // block 1 +544
  float b1_rear_track() const { return guest::LoadFloat(address + 896); }
  void set_b1_rear_track(float v) const { guest::StoreFloat(address + 896, v); }
  uint32_t b1_rear_track_address() const { return address + 896; }
  // block 1 +548 (its floats +528 to +636)
  float b1_float_548() const { return guest::LoadFloat(address + 900); }
  void set_b1_float_548(float v) const { guest::StoreFloat(address + 900, v); }
  uint32_t b1_float_548_address() const { return address + 900; }
  // block 1 +552 (its floats +528 to +636)
  float b1_float_552() const { return guest::LoadFloat(address + 904); }
  void set_b1_float_552(float v) const { guest::StoreFloat(address + 904, v); }
  uint32_t b1_float_552_address() const { return address + 904; }
  // block 1 +556: a ramp rate
  float b1_reverse_ramp() const { return guest::LoadFloat(address + 908); }
  void set_b1_reverse_ramp(float v) const { guest::StoreFloat(address + 908, v); }
  uint32_t b1_reverse_ramp_address() const { return address + 908; }
  // block 1 +560 (its floats +528 to +636)
  float b1_float_560() const { return guest::LoadFloat(address + 912); }
  void set_b1_float_560(float v) const { guest::StoreFloat(address + 912, v); }
  uint32_t b1_float_560_address() const { return address + 912; }
  // block 1 +564 (its floats +528 to +636)
  float b1_float_564() const { return guest::LoadFloat(address + 916); }
  void set_b1_float_564(float v) const { guest::StoreFloat(address + 916, v); }
  uint32_t b1_float_564_address() const { return address + 916; }
  // block 1 +568 (its floats +528 to +636)
  float b1_float_568() const { return guest::LoadFloat(address + 920); }
  void set_b1_float_568(float v) const { guest::StoreFloat(address + 920, v); }
  uint32_t b1_float_568_address() const { return address + 920; }
  // block 1 +572 (its floats +528 to +636)
  float b1_float_572() const { return guest::LoadFloat(address + 924); }
  void set_b1_float_572(float v) const { guest::StoreFloat(address + 924, v); }
  uint32_t b1_float_572_address() const { return address + 924; }
  // block 1 +576 (its floats +528 to +636)
  float b1_float_576() const { return guest::LoadFloat(address + 928); }
  void set_b1_float_576(float v) const { guest::StoreFloat(address + 928, v); }
  uint32_t b1_float_576_address() const { return address + 928; }
  // block 1 +580 (its floats +528 to +636)
  float b1_float_580() const { return guest::LoadFloat(address + 932); }
  void set_b1_float_580(float v) const { guest::StoreFloat(address + 932, v); }
  uint32_t b1_float_580_address() const { return address + 932; }
  // block 1 +584 (its floats +528 to +636)
  float b1_float_584() const { return guest::LoadFloat(address + 936); }
  void set_b1_float_584(float v) const { guest::StoreFloat(address + 936, v); }
  uint32_t b1_float_584_address() const { return address + 936; }
  // block 1 +588 (its floats +528 to +636)
  float b1_float_588() const { return guest::LoadFloat(address + 940); }
  void set_b1_float_588(float v) const { guest::StoreFloat(address + 940, v); }
  uint32_t b1_float_588_address() const { return address + 940; }
  // block 1 +592 (its floats +528 to +636)
  float b1_float_592() const { return guest::LoadFloat(address + 944); }
  void set_b1_float_592(float v) const { guest::StoreFloat(address + 944, v); }
  uint32_t b1_float_592_address() const { return address + 944; }
  // block 1 +596 (its floats +528 to +636)
  float b1_float_596() const { return guest::LoadFloat(address + 948); }
  void set_b1_float_596(float v) const { guest::StoreFloat(address + 948, v); }
  uint32_t b1_float_596_address() const { return address + 948; }
  // block 1 +600
  float b1_idle_rpm() const { return guest::LoadFloat(address + 952); }
  void set_b1_idle_rpm(float v) const { guest::StoreFloat(address + 952, v); }
  uint32_t b1_idle_rpm_address() const { return address + 952; }
  // block 1 +604 (its floats +528 to +636)
  float b1_float_604() const { return guest::LoadFloat(address + 956); }
  void set_b1_float_604(float v) const { guest::StoreFloat(address + 956, v); }
  uint32_t b1_float_604_address() const { return address + 956; }
  // block 1 +608 (its floats +528 to +636)
  float b1_float_608() const { return guest::LoadFloat(address + 960); }
  void set_b1_float_608(float v) const { guest::StoreFloat(address + 960, v); }
  uint32_t b1_float_608_address() const { return address + 960; }
  // block 1 +612 (its floats +528 to +636)
  float b1_float_612() const { return guest::LoadFloat(address + 964); }
  void set_b1_float_612(float v) const { guest::StoreFloat(address + 964, v); }
  uint32_t b1_float_612_address() const { return address + 964; }
  // block 1 +616
  float b1_steering() const { return guest::LoadFloat(address + 968); }
  void set_b1_steering(float v) const { guest::StoreFloat(address + 968, v); }
  uint32_t b1_steering_address() const { return address + 968; }
  // block 1 +620 (its floats +528 to +636)
  float b1_float_620() const { return guest::LoadFloat(address + 972); }
  void set_b1_float_620(float v) const { guest::StoreFloat(address + 972, v); }
  uint32_t b1_float_620_address() const { return address + 972; }
  // block 1 +624 (its floats +528 to +636)
  float b1_float_624() const { return guest::LoadFloat(address + 976); }
  void set_b1_float_624(float v) const { guest::StoreFloat(address + 976, v); }
  uint32_t b1_float_624_address() const { return address + 976; }
  // block 1 +628 (its floats +528 to +636)
  float b1_float_628() const { return guest::LoadFloat(address + 980); }
  void set_b1_float_628(float v) const { guest::StoreFloat(address + 980, v); }
  uint32_t b1_float_628_address() const { return address + 980; }
  // the engine stage +12: probably
  float stage_max_rpm() const { return guest::LoadFloat(address + 1004); }
  void set_stage_max_rpm(float v) const { guest::StoreFloat(address + 1004, v); }
  uint32_t stage_max_rpm_address() const { return address + 1004; }
  // the engine stage +16
  float stage_16() const { return guest::LoadFloat(address + 1008); }
  void set_stage_16(float v) const { guest::StoreFloat(address + 1008, v); }
  uint32_t stage_16_address() const { return address + 1008; }
  // the engine stage +20
  float stage_rev_limit() const { return guest::LoadFloat(address + 1012); }
  void set_stage_rev_limit(float v) const { guest::StoreFloat(address + 1012, v); }
  uint32_t stage_rev_limit_address() const { return address + 1012; }
  // the engine stage +24: the torque x 10
  float stage_torque_x10() const { return guest::LoadFloat(address + 1016); }
  void set_stage_torque_x10(float v) const { guest::StoreFloat(address + 1016, v); }
  uint32_t stage_torque_x10_address() const { return address + 1016; }
  // the engine stage +28: the torque curve's first (32 floats)
  float stage_torque_curve_0() const { return guest::LoadFloat(address + 1020); }
  void set_stage_torque_curve_0(float v) const { guest::StoreFloat(address + 1020, v); }
  uint32_t stage_torque_curve_0_address() const { return address + 1020; }
  // the engine stage +156
  float stage_156() const { return guest::LoadFloat(address + 1148); }
  void set_stage_156(float v) const { guest::StoreFloat(address + 1148, v); }
  uint32_t stage_156_address() const { return address + 1148; }
  // the engine stage +160
  float stage_160() const { return guest::LoadFloat(address + 1152); }
  void set_stage_160(float v) const { guest::StoreFloat(address + 1152, v); }
  uint32_t stage_160_address() const { return address + 1152; }
  // the engine stage +164
  float stage_164() const { return guest::LoadFloat(address + 1156); }
  void set_stage_164(float v) const { guest::StoreFloat(address + 1156, v); }
  uint32_t stage_164_address() const { return address + 1156; }
  // the engine stage +168
  float stage_168() const { return guest::LoadFloat(address + 1160); }
  void set_stage_168(float v) const { guest::StoreFloat(address + 1160, v); }
  uint32_t stage_168_address() const { return address + 1160; }
  // the engine stage +172: the turbo flag
  uint32_t stage_turbo() const { return guest::LoadU32(address + 1164); }
  void set_stage_turbo(uint32_t v) const { guest::StoreU32(address + 1164, v); }
  uint32_t stage_turbo_address() const { return address + 1164; }
  // the engine stage +176
  float stage_boost() const { return guest::LoadFloat(address + 1168); }
  void set_stage_boost(float v) const { guest::StoreFloat(address + 1168, v); }
  uint32_t stage_boost_address() const { return address + 1168; }
  // the engine stage +180
  float stage_boost_2() const { return guest::LoadFloat(address + 1172); }
  void set_stage_boost_2(float v) const { guest::StoreFloat(address + 1172, v); }
  uint32_t stage_boost_2_address() const { return address + 1172; }
  // the engine stage +184
  float stage_boost_3() const { return guest::LoadFloat(address + 1176); }
  void set_stage_boost_3(float v) const { guest::StoreFloat(address + 1176, v); }
  uint32_t stage_boost_3_address() const { return address + 1176; }
  // the engine stage +188
  float stage_188() const { return guest::LoadFloat(address + 1180); }
  void set_stage_188(float v) const { guest::StoreFloat(address + 1180, v); }
  uint32_t stage_188_address() const { return address + 1180; }
  // the engine stage +192: probably
  uint32_t stage_blow_off() const { return guest::LoadU32(address + 1184); }
  void set_stage_blow_off(uint32_t v) const { guest::StoreU32(address + 1184, v); }
  uint32_t stage_blow_off_address() const { return address + 1184; }
  // the engine stage +196
  float stage_196() const { return guest::LoadFloat(address + 1188); }
  void set_stage_196(float v) const { guest::StoreFloat(address + 1188, v); }
  uint32_t stage_196_address() const { return address + 1188; }
  // the front tire +8
  float front_tire_8() const { return guest::LoadFloat(address + 1204); }
  void set_front_tire_8(float v) const { guest::StoreFloat(address + 1204, v); }
  uint32_t front_tire_8_address() const { return address + 1204; }
  // the front tire +24
  float front_tire_24() const { return guest::LoadFloat(address + 1220); }
  void set_front_tire_24(float v) const { guest::StoreFloat(address + 1220, v); }
  uint32_t front_tire_24_address() const { return address + 1220; }
  // the front tire +44
  float front_tire_radius() const { return guest::LoadFloat(address + 1240); }
  void set_front_tire_radius(float v) const { guest::StoreFloat(address + 1240, v); }
  uint32_t front_tire_radius_address() const { return address + 1240; }
  // the front tire +48
  float front_tire_width() const { return guest::LoadFloat(address + 1244); }
  void set_front_tire_width(float v) const { guest::StoreFloat(address + 1244, v); }
  uint32_t front_tire_width_address() const { return address + 1244; }
  // the front tire +52
  float front_tire_mass() const { return guest::LoadFloat(address + 1248); }
  void set_front_tire_mass(float v) const { guest::StoreFloat(address + 1248, v); }
  uint32_t front_tire_mass_address() const { return address + 1248; }
  // the front tire +56
  float front_tire_56() const { return guest::LoadFloat(address + 1252); }
  void set_front_tire_56(float v) const { guest::StoreFloat(address + 1252, v); }
  uint32_t front_tire_56_address() const { return address + 1252; }
  // the front tire +60
  float front_tire_60() const { return guest::LoadFloat(address + 1256); }
  void set_front_tire_60(float v) const { guest::StoreFloat(address + 1256, v); }
  uint32_t front_tire_60_address() const { return address + 1256; }
  // the front tire +64
  float front_tire_grip() const { return guest::LoadFloat(address + 1260); }
  void set_front_tire_grip(float v) const { guest::StoreFloat(address + 1260, v); }
  uint32_t front_tire_grip_address() const { return address + 1260; }
  // the front tire +68
  float front_tire_grip_2() const { return guest::LoadFloat(address + 1264); }
  void set_front_tire_grip_2(float v) const { guest::StoreFloat(address + 1264, v); }
  uint32_t front_tire_grip_2_address() const { return address + 1264; }
  // the front tire +72
  float front_tire_72() const { return guest::LoadFloat(address + 1268); }
  void set_front_tire_72(float v) const { guest::StoreFloat(address + 1268, v); }
  uint32_t front_tire_72_address() const { return address + 1268; }
  // the front tire +76
  float front_tire_76() const { return guest::LoadFloat(address + 1272); }
  void set_front_tire_76(float v) const { guest::StoreFloat(address + 1272, v); }
  uint32_t front_tire_76_address() const { return address + 1272; }
  // the rear tire +8
  float rear_tire_8() const { return guest::LoadFloat(address + 1296); }
  void set_rear_tire_8(float v) const { guest::StoreFloat(address + 1296, v); }
  uint32_t rear_tire_8_address() const { return address + 1296; }
  // the rear tire +24
  float rear_tire_24() const { return guest::LoadFloat(address + 1312); }
  void set_rear_tire_24(float v) const { guest::StoreFloat(address + 1312, v); }
  uint32_t rear_tire_24_address() const { return address + 1312; }
  // the rear tire +44
  float rear_tire_radius() const { return guest::LoadFloat(address + 1332); }
  void set_rear_tire_radius(float v) const { guest::StoreFloat(address + 1332, v); }
  uint32_t rear_tire_radius_address() const { return address + 1332; }
  // the rear tire +48
  float rear_tire_width() const { return guest::LoadFloat(address + 1336); }
  void set_rear_tire_width(float v) const { guest::StoreFloat(address + 1336, v); }
  uint32_t rear_tire_width_address() const { return address + 1336; }
  // the rear tire +52
  float rear_tire_mass() const { return guest::LoadFloat(address + 1340); }
  void set_rear_tire_mass(float v) const { guest::StoreFloat(address + 1340, v); }
  uint32_t rear_tire_mass_address() const { return address + 1340; }
  // the rear tire +56
  float rear_tire_56() const { return guest::LoadFloat(address + 1344); }
  void set_rear_tire_56(float v) const { guest::StoreFloat(address + 1344, v); }
  uint32_t rear_tire_56_address() const { return address + 1344; }
  // the rear tire +60
  float rear_tire_60() const { return guest::LoadFloat(address + 1348); }
  void set_rear_tire_60(float v) const { guest::StoreFloat(address + 1348, v); }
  uint32_t rear_tire_60_address() const { return address + 1348; }
  // the rear tire +64
  float rear_tire_grip() const { return guest::LoadFloat(address + 1352); }
  void set_rear_tire_grip(float v) const { guest::StoreFloat(address + 1352, v); }
  uint32_t rear_tire_grip_address() const { return address + 1352; }
  // the rear tire +68
  float rear_tire_grip_2() const { return guest::LoadFloat(address + 1356); }
  void set_rear_tire_grip_2(float v) const { guest::StoreFloat(address + 1356, v); }
  uint32_t rear_tire_grip_2_address() const { return address + 1356; }
  // the rear tire +72
  float rear_tire_72() const { return guest::LoadFloat(address + 1360); }
  void set_rear_tire_72(float v) const { guest::StoreFloat(address + 1360, v); }
  uint32_t rear_tire_72_address() const { return address + 1360; }
  // the rear tire +76
  float rear_tire_76() const { return guest::LoadFloat(address + 1364); }
  void set_rear_tire_76(float v) const { guest::StoreFloat(address + 1364, v); }
  uint32_t rear_tire_76_address() const { return address + 1364; }
};

// embedded; measure: sub_820CE600.r3, sub_820CE7F0.r3
struct StagePowerCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 1064;
  // read by sub_820CE600 below the curve (not read in detail)
  float unknown_minus_28() const { return guest::LoadFloat(address + -28); }
  void set_unknown_minus_28(float v) const { guest::StoreFloat(address + -28, v); }
  uint32_t unknown_minus_28_address() const { return address + -28; }
  // read by sub_820CE600 below the curve (not read in detail)
  float unknown_minus_12() const { return guest::LoadFloat(address + -12); }
  void set_unknown_minus_12(float v) const { guest::StoreFloat(address + -12, v); }
  uint32_t unknown_minus_12_address() const { return address + -12; }
  // 32 entries of 16 bytes: entry 0's rpm
  float rpm() const { return guest::LoadFloat(address + 0); }
  void set_rpm(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t rpm_address() const { return address + 0; }
  float torque() const { return guest::LoadFloat(address + 4); }
  void set_torque(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t torque_address() const { return address + 4; }
  float power() const { return guest::LoadFloat(address + 8); }
  void set_power(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t power_address() const { return address + 8; }
  float entry_12() const { return guest::LoadFloat(address + 12); }
  void set_entry_12(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t entry_12_address() const { return address + 12; }
  // the 32 entries after the peak's shift: entry 0's rpm
  float shifted_rpm() const { return guest::LoadFloat(address + 512); }
  void set_shifted_rpm(float v) const { guest::StoreFloat(address + 512, v); }
  uint32_t shifted_rpm_address() const { return address + 512; }
  float shifted_torque() const { return guest::LoadFloat(address + 516); }
  void set_shifted_torque(float v) const { guest::StoreFloat(address + 516, v); }
  uint32_t shifted_torque_address() const { return address + 516; }
  float shifted_power() const { return guest::LoadFloat(address + 520); }
  void set_shifted_power(float v) const { guest::StoreFloat(address + 520, v); }
  uint32_t shifted_power_address() const { return address + 520; }
  float shifted_entry_12() const { return guest::LoadFloat(address + 524); }
  void set_shifted_entry_12(float v) const { guest::StoreFloat(address + 524, v); }
  uint32_t shifted_entry_12_address() const { return address + 524; }
  // the peak power's index
  uint32_t peak_index() const { return guest::LoadU32(address + 1024); }
  void set_peak_index(uint32_t v) const { guest::StoreU32(address + 1024, v); }
  uint32_t peak_index_address() const { return address + 1024; }
  // the points counted
  uint32_t points() const { return guest::LoadU32(address + 1028); }
  void set_points(uint32_t v) const { guest::StoreU32(address + 1028, v); }
  uint32_t points_address() const { return address + 1028; }
  // how far the peak may move down (100 rpm steps, -20 to 0)
  int32_t peak_down() const { return int32_t(guest::LoadU32(address + 1032)); }
  void set_peak_down(int32_t v) const { guest::StoreU32(address + 1032, uint32_t(v)); }
  uint32_t peak_down_address() const { return address + 1032; }
  // how far up (0 to 20)
  int32_t peak_up() const { return int32_t(guest::LoadU32(address + 1036)); }
  void set_peak_up(int32_t v) const { guest::StoreU32(address + 1036, uint32_t(v)); }
  uint32_t peak_up_address() const { return address + 1036; }
  // 800.0
  float lowest_rpm() const { return guest::LoadFloat(address + 1040); }
  void set_lowest_rpm(float v) const { guest::StoreFloat(address + 1040, v); }
  uint32_t lowest_rpm_address() const { return address + 1040; }
  // the engine stage's +20 (sub_820CE7F0)
  float highest_rpm() const { return guest::LoadFloat(address + 1044); }
  void set_highest_rpm(float v) const { guest::StoreFloat(address + 1044, v); }
  uint32_t highest_rpm_address() const { return address + 1044; }
  float torque_scale() const { return guest::LoadFloat(address + 1048); }
  void set_torque_scale(float v) const { guest::StoreFloat(address + 1048, v); }
  uint32_t torque_scale_address() const { return address + 1048; }
  float shifted_peak_rpm() const { return guest::LoadFloat(address + 1052); }
  void set_shifted_peak_rpm(float v) const { guest::StoreFloat(address + 1052, v); }
  uint32_t shifted_peak_rpm_address() const { return address + 1052; }
  float shifted_peak_power() const { return guest::LoadFloat(address + 1056); }
  void set_shifted_peak_power(float v) const { guest::StoreFloat(address + 1056, v); }
  uint32_t shifted_peak_power_address() const { return address + 1056; }
  float shifted_peak_power_rpm() const { return guest::LoadFloat(address + 1060); }
  void set_shifted_peak_power_rpm(float v) const { guest::StoreFloat(address + 1060, v); }
  uint32_t shifted_peak_power_rpm_address() const { return address + 1060; }
};

// embedded; measure: RaceEvent::Init.r4
struct RaceEventModeArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t mode() const { return guest::LoadU32(address + 0); }
  void set_mode(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t mode_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::SetMode.r5
struct RaceEventLocationArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t location() const { return guest::LoadU32(address + 0); }
  void set_location(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t location_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::Init.r6
struct RaceEventFirstOwnedOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // RaceEvent::FirstOwned's answer
  uint32_t first_owned() const { return guest::LoadU32(address + 0); }
  void set_first_owned(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t first_owned_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::GroupItem.r5, EventPools::CopyIds.r6, EventPools::ToGroupNoList.r5
struct RaceEventIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // an index into a group, or the running list index (in and out)
  uint32_t index() const { return guest::LoadU32(address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: EventPools::CopyIds.r7, RaceEvent::PlaceUnlocked.r5
struct RaceEventFlagArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // CopyIds: the list entries' flag; PlaceUnlocked: place the opponents found
  uint32_t flag() const { return guest::LoadU32(address + 0); }
  void set_flag(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t flag_address() const { return address + 0; }
};

// embedded; measure: EventCondition::LocationCleared.r4
struct RaceEventLevelArg : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  // the item's level
  uint16_t level() const { return guest::LoadU16(address + 0); }
  void set_level(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t level_address() const { return address + 0; }
};

// embedded; measure: Items::Beaten.r4, EventPools::AddRivalsOf.r4
struct RaceEventItemIdRef : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  // an item id (a group entry's or a rival's)
  uint16_t id() const { return guest::LoadU16(address + 0); }
  void set_id(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t id_address() const { return address + 0; }
};

// embedded; measure: EventPools::Trim.r4
struct RaceEventCapArg : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  // the location's cap
  uint16_t cap() const { return guest::LoadU16(address + 0); }
  void set_cap(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t cap_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::PlaceUnlocked.r4, RaceEvent::SectionRival.r4
struct RaceEventRoutePosition : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // probably
  uint16_t section() const { return guest::LoadU16(address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // probably; stepped back and forward by the placements
  uint16_t slice() const { return guest::LoadU16(address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
  // both, copied as one word
  uint32_t word() const { return guest::LoadU32(address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t word_address() const { return address + 0; }
};

// embedded; measure: EventCondition@+4
struct EventConditionAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: EventCondition@+8
struct EventConditionAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: GarageScreen::SetUpPlateEditor.ret@+992
struct CarSpecCopiesAt992 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  float unknown_20() const { return guest::LoadFloat(address + 20); }
  void set_unknown_20(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: RaceEvent@+84
#pragma pack(push, 1)
struct RaceEventAt84Data {
  uint8_t pad_0[4] = {};
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(RaceEventAt84Data) == 12);
inline constexpr memory::LayoutField kRaceEventAt84Fields[] = {{4, 4}, {8, 4}};
inline constexpr memory::Layout kRaceEventAt84Layout = {kRaceEventAt84Fields, 2};

struct RaceEventAt84 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  RaceEventAt84Data* data() const { return static_cast<RaceEventAt84Data*>(memory::HandleObject(address)); }
  static RaceEventAt84 New() {
    return RaceEventAt84(memory::MakeHandle(new RaceEventAt84Data(), sizeof(RaceEventAt84Data), "RaceEventAt84", &kRaceEventAt84Layout));
  }
  template <uint32_t Stride = kSize>
  static RaceEventAt84 NewArray(uint32_t count) {
    static_assert(sizeof(RaceEventAt84Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceEventAt84Data*>(::operator new(sizeof(RaceEventAt84Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceEventAt84Data();
    return RaceEventAt84(memory::MakeHandle(p, sizeof(RaceEventAt84Data) * count, "RaceEventAt84", &kRaceEventAt84Layout, sizeof(RaceEventAt84Data)));
  }
  RaceEventAt84 at(uint32_t i) const { return RaceEventAt84(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceEventAt84Data* direct() const { return static_cast<RaceEventAt84Data*>(memory::HandleOwn(address, &kRaceEventAt84Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceEventAt84Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceEventAt84Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_4() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

// embedded; measure: RaceSounds::PickRivalMusic.ret@+160
struct OpponentItemAt160 : View {
  using View::View;
  static constexpr uint32_t kSize = 2812;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_80() const { return guest::LoadU32(address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint32_t unknown_84() const { return guest::LoadU32(address + 84); }
  void set_unknown_84(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t unknown_84_address() const { return address + 84; }
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_108() const { return guest::LoadU32(address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_128() const { return guest::LoadU32(address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t unknown_132() const { return guest::LoadU32(address + 132); }
  void set_unknown_132(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  uint32_t unknown_136() const { return guest::LoadU32(address + 136); }
  void set_unknown_136(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  uint32_t unknown_140() const { return guest::LoadU32(address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  uint32_t unknown_144() const { return guest::LoadU32(address + 144); }
  void set_unknown_144(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  uint32_t unknown_148() const { return guest::LoadU32(address + 148); }
  void set_unknown_148(uint32_t v) const { guest::StoreU32(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  uint32_t unknown_152() const { return guest::LoadU32(address + 152); }
  void set_unknown_152(uint32_t v) const { guest::StoreU32(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  uint32_t unknown_156() const { return guest::LoadU32(address + 156); }
  void set_unknown_156(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  uint32_t unknown_160() const { return guest::LoadU32(address + 160); }
  void set_unknown_160(uint32_t v) const { guest::StoreU32(address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  uint32_t unknown_164() const { return guest::LoadU32(address + 164); }
  void set_unknown_164(uint32_t v) const { guest::StoreU32(address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  uint32_t unknown_168() const { return guest::LoadU32(address + 168); }
  void set_unknown_168(uint32_t v) const { guest::StoreU32(address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  uint32_t unknown_172() const { return guest::LoadU32(address + 172); }
  void set_unknown_172(uint32_t v) const { guest::StoreU32(address + 172, v); }
  uint32_t unknown_172_address() const { return address + 172; }
  uint32_t unknown_176() const { return guest::LoadU32(address + 176); }
  void set_unknown_176(uint32_t v) const { guest::StoreU32(address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  uint32_t unknown_180() const { return guest::LoadU32(address + 180); }
  void set_unknown_180(uint32_t v) const { guest::StoreU32(address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  uint32_t unknown_184() const { return guest::LoadU32(address + 184); }
  void set_unknown_184(uint32_t v) const { guest::StoreU32(address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  uint32_t unknown_188() const { return guest::LoadU32(address + 188); }
  void set_unknown_188(uint32_t v) const { guest::StoreU32(address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  uint32_t unknown_192() const { return guest::LoadU32(address + 192); }
  void set_unknown_192(uint32_t v) const { guest::StoreU32(address + 192, v); }
  uint32_t unknown_192_address() const { return address + 192; }
  uint32_t unknown_196() const { return guest::LoadU32(address + 196); }
  void set_unknown_196(uint32_t v) const { guest::StoreU32(address + 196, v); }
  uint32_t unknown_196_address() const { return address + 196; }
  uint32_t unknown_200() const { return guest::LoadU32(address + 200); }
  void set_unknown_200(uint32_t v) const { guest::StoreU32(address + 200, v); }
  uint32_t unknown_200_address() const { return address + 200; }
  uint32_t unknown_204() const { return guest::LoadU32(address + 204); }
  void set_unknown_204(uint32_t v) const { guest::StoreU32(address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  uint32_t unknown_208() const { return guest::LoadU32(address + 208); }
  void set_unknown_208(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  uint32_t unknown_212() const { return guest::LoadU32(address + 212); }
  void set_unknown_212(uint32_t v) const { guest::StoreU32(address + 212, v); }
  uint32_t unknown_212_address() const { return address + 212; }
  uint32_t unknown_216() const { return guest::LoadU32(address + 216); }
  void set_unknown_216(uint32_t v) const { guest::StoreU32(address + 216, v); }
  uint32_t unknown_216_address() const { return address + 216; }
  uint32_t unknown_220() const { return guest::LoadU32(address + 220); }
  void set_unknown_220(uint32_t v) const { guest::StoreU32(address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  uint32_t unknown_224() const { return guest::LoadU32(address + 224); }
  void set_unknown_224(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  uint32_t unknown_228() const { return guest::LoadU32(address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  uint32_t unknown_232() const { return guest::LoadU32(address + 232); }
  void set_unknown_232(uint32_t v) const { guest::StoreU32(address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  uint32_t unknown_236() const { return guest::LoadU32(address + 236); }
  void set_unknown_236(uint32_t v) const { guest::StoreU32(address + 236, v); }
  uint32_t unknown_236_address() const { return address + 236; }
  uint32_t unknown_240() const { return guest::LoadU32(address + 240); }
  void set_unknown_240(uint32_t v) const { guest::StoreU32(address + 240, v); }
  uint32_t unknown_240_address() const { return address + 240; }
  uint32_t unknown_244() const { return guest::LoadU32(address + 244); }
  void set_unknown_244(uint32_t v) const { guest::StoreU32(address + 244, v); }
  uint32_t unknown_244_address() const { return address + 244; }
  uint32_t unknown_248() const { return guest::LoadU32(address + 248); }
  void set_unknown_248(uint32_t v) const { guest::StoreU32(address + 248, v); }
  uint32_t unknown_248_address() const { return address + 248; }
  uint32_t unknown_252() const { return guest::LoadU32(address + 252); }
  void set_unknown_252(uint32_t v) const { guest::StoreU32(address + 252, v); }
  uint32_t unknown_252_address() const { return address + 252; }
  uint32_t unknown_256() const { return guest::LoadU32(address + 256); }
  void set_unknown_256(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t unknown_256_address() const { return address + 256; }
  uint32_t unknown_260() const { return guest::LoadU32(address + 260); }
  void set_unknown_260(uint32_t v) const { guest::StoreU32(address + 260, v); }
  uint32_t unknown_260_address() const { return address + 260; }
  uint32_t unknown_264() const { return guest::LoadU32(address + 264); }
  void set_unknown_264(uint32_t v) const { guest::StoreU32(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  uint32_t unknown_268() const { return guest::LoadU32(address + 268); }
  void set_unknown_268(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  uint32_t unknown_332() const { return guest::LoadU32(address + 332); }
  void set_unknown_332(uint32_t v) const { guest::StoreU32(address + 332, v); }
  uint32_t unknown_332_address() const { return address + 332; }
  uint32_t unknown_336() const { return guest::LoadU32(address + 336); }
  void set_unknown_336(uint32_t v) const { guest::StoreU32(address + 336, v); }
  uint32_t unknown_336_address() const { return address + 336; }
  uint32_t unknown_340() const { return guest::LoadU32(address + 340); }
  void set_unknown_340(uint32_t v) const { guest::StoreU32(address + 340, v); }
  uint32_t unknown_340_address() const { return address + 340; }
  uint32_t unknown_344() const { return guest::LoadU32(address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
  uint32_t unknown_348() const { return guest::LoadU32(address + 348); }
  void set_unknown_348(uint32_t v) const { guest::StoreU32(address + 348, v); }
  uint32_t unknown_348_address() const { return address + 348; }
  uint32_t unknown_2768() const { return guest::LoadU32(address + 2768); }
  void set_unknown_2768(uint32_t v) const { guest::StoreU32(address + 2768, v); }
  uint32_t unknown_2768_address() const { return address + 2768; }
  uint32_t unknown_2788() const { return guest::LoadU32(address + 2788); }
  void set_unknown_2788(uint32_t v) const { guest::StoreU32(address + 2788, v); }
  uint32_t unknown_2788_address() const { return address + 2788; }
  uint32_t unknown_2792() const { return guest::LoadU32(address + 2792); }
  void set_unknown_2792(uint32_t v) const { guest::StoreU32(address + 2792, v); }
  uint32_t unknown_2792_address() const { return address + 2792; }
  uint32_t unknown_2796() const { return guest::LoadU32(address + 2796); }
  void set_unknown_2796(uint32_t v) const { guest::StoreU32(address + 2796, v); }
  uint32_t unknown_2796_address() const { return address + 2796; }
  uint32_t unknown_2800() const { return guest::LoadU32(address + 2800); }
  void set_unknown_2800(uint32_t v) const { guest::StoreU32(address + 2800, v); }
  uint32_t unknown_2800_address() const { return address + 2800; }
  uint32_t unknown_2804() const { return guest::LoadU32(address + 2804); }
  void set_unknown_2804(uint32_t v) const { guest::StoreU32(address + 2804, v); }
  uint32_t unknown_2804_address() const { return address + 2804; }
  uint32_t unknown_2808() const { return guest::LoadU32(address + 2808); }
  void set_unknown_2808(uint32_t v) const { guest::StoreU32(address + 2808, v); }
  uint32_t unknown_2808_address() const { return address + 2808; }
};

// embedded; measure: sub_820C4550.ret@+992
struct CarSpecCopiesAt992_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  float unknown_24() const { return guest::LoadFloat(address + 24); }
  void set_unknown_24(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  float unknown_28() const { return guest::LoadFloat(address + 28); }
  void set_unknown_28(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// embedded; measure: EventPools@+2432
struct EventPoolsAt2432 : View {
  using View::View;
  static constexpr uint32_t kSize = 408;
  uint16_t unknown_0() const { return guest::LoadU16(address + 0); }
  void set_unknown_0(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_2() const { return guest::LoadU16(address + 2); }
  void set_unknown_2(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint16_t unknown_202() const { return guest::LoadU16(address + 202); }
  void set_unknown_202(uint16_t v) const { guest::StoreU16(address + 202, v); }
  uint32_t unknown_202_address() const { return address + 202; }
  uint16_t unknown_204() const { return guest::LoadU16(address + 204); }
  void set_unknown_204(uint16_t v) const { guest::StoreU16(address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  uint16_t unknown_404() const { return guest::LoadU16(address + 404); }
  void set_unknown_404(uint16_t v) const { guest::StoreU16(address + 404, v); }
  uint32_t unknown_404_address() const { return address + 404; }
  uint16_t unknown_406() const { return guest::LoadU16(address + 406); }
  void set_unknown_406(uint16_t v) const { guest::StoreU16(address + 406, v); }
  uint32_t unknown_406_address() const { return address + 406; }
};

// embedded; measure: RaceEvent@+36
#pragma pack(push, 1)
struct EventGroupAt36Data {
  uint16_t unknown_0 = 0;  // +0
  uint16_t unknown_2 = 0;  // +2
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(EventGroupAt36Data) == 12);
inline constexpr memory::LayoutField kEventGroupAt36Fields[] = {{0, 2}, {2, 2}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kEventGroupAt36Layout = {kEventGroupAt36Fields, 4};

struct EventGroupAt36 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  EventGroupAt36Data* data() const { return static_cast<EventGroupAt36Data*>(memory::HandleObject(address)); }
  static EventGroupAt36 New() {
    return EventGroupAt36(memory::MakeHandle(new EventGroupAt36Data(), sizeof(EventGroupAt36Data), "EventGroupAt36", &kEventGroupAt36Layout));
  }
  template <uint32_t Stride = kSize>
  static EventGroupAt36 NewArray(uint32_t count) {
    static_assert(sizeof(EventGroupAt36Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<EventGroupAt36Data*>(::operator new(sizeof(EventGroupAt36Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) EventGroupAt36Data();
    return EventGroupAt36(memory::MakeHandle(p, sizeof(EventGroupAt36Data) * count, "EventGroupAt36", &kEventGroupAt36Layout, sizeof(EventGroupAt36Data)));
  }
  EventGroupAt36 at(uint32_t i) const { return EventGroupAt36(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  EventGroupAt36Data* direct() const { return static_cast<EventGroupAt36Data*>(memory::HandleOwn(address, &kEventGroupAt36Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kEventGroupAt36Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupAt36Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_unknown_0_bits(uint16_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_unknown_0(uint16_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint16_t unknown_2_bits() const { if (auto* d = direct()) return d->unknown_2; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_unknown_2_bits(uint16_t v) const { if (auto* d = direct()) { d->unknown_2 = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  uint16_t unknown_2() const { if (auto* d = direct()) return d->unknown_2; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_unknown_2(uint16_t v) const { if (auto* d = direct()) { d->unknown_2 = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_4() const { if (auto* d = direct()) return d->unknown_4; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_unknown_4(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8_bits() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t unknown_8() const { if (auto* d = direct()) return d->unknown_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_unknown_8(uint32_t v) const { if (auto* d = direct()) { d->unknown_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

// embedded; measure: RaceEvent@+36.+8
#pragma pack(push, 1)
struct EventGroupEntryPartData {
  uint16_t unknown_0 = 0;  // +0
  uint16_t unknown_2 = 0;  // +2
};
#pragma pack(pop)
static_assert(sizeof(EventGroupEntryPartData) == 4);
inline constexpr memory::LayoutField kEventGroupEntryPartFields[] = {{0, 2}, {2, 2}};
inline constexpr memory::Layout kEventGroupEntryPartLayout = {kEventGroupEntryPartFields, 2};

struct EventGroupEntryPart : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  EventGroupEntryPartData* data() const { return static_cast<EventGroupEntryPartData*>(memory::HandleObject(address)); }
  static EventGroupEntryPart New() {
    return EventGroupEntryPart(memory::MakeHandle(new EventGroupEntryPartData(), sizeof(EventGroupEntryPartData), "EventGroupEntryPart", &kEventGroupEntryPartLayout));
  }
  template <uint32_t Stride = kSize>
  static EventGroupEntryPart NewArray(uint32_t count) {
    static_assert(sizeof(EventGroupEntryPartData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<EventGroupEntryPartData*>(::operator new(sizeof(EventGroupEntryPartData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) EventGroupEntryPartData();
    return EventGroupEntryPart(memory::MakeHandle(p, sizeof(EventGroupEntryPartData) * count, "EventGroupEntryPart", &kEventGroupEntryPartLayout, sizeof(EventGroupEntryPartData)));
  }
  EventGroupEntryPart at(uint32_t i) const { return EventGroupEntryPart(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  EventGroupEntryPartData* direct() const { return static_cast<EventGroupEntryPartData*>(memory::HandleOwn(address, &kEventGroupEntryPartLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kEventGroupEntryPartLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupEntryPartLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint16_t unknown_0_bits() const { if (auto* d = direct()) return d->unknown_0; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_unknown_0_bits(uint16_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint16_t unknown_0() const { if (auto* d = direct()) return d->unknown_0; return uint16_t((memory::IsHandle(address + 0) ? uint16_t(memory::OwnerLoad(address + 0, 2)) : guest::LoadU16(address + 0))); }
  void set_unknown_0(uint16_t v) const { if (auto* d = direct()) { d->unknown_0 = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 2, v); else guest::StoreU16(address + 0, v); }
  uint32_t unknown_0_4i_address() const { return address + 0; }
  uint32_t unknown_0_4i_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupEntryPartLayout, 0, 4)); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_unknown_0_4i_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kEventGroupEntryPartLayout, 0, 4, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_4i() const { return uint32_t(unknown_0_4i_bits()); }
  void set_unknown_0_4i(uint32_t v) const { set_unknown_0_4i_bits(uint32_t(v)); }
  uint32_t unknown_2_address() const { return address + 2; }
  uint16_t unknown_2_bits() const { if (auto* d = direct()) return d->unknown_2; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_unknown_2_bits(uint16_t v) const { if (auto* d = direct()) { d->unknown_2 = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
  uint16_t unknown_2() const { if (auto* d = direct()) return d->unknown_2; return uint16_t((memory::IsHandle(address + 2) ? uint16_t(memory::OwnerLoad(address + 2, 2)) : guest::LoadU16(address + 2))); }
  void set_unknown_2(uint16_t v) const { if (auto* d = direct()) { d->unknown_2 = v; return; } if (memory::IsHandle(address + 2)) memory::OwnerStore(address + 2, 2, v); else guest::StoreU16(address + 2, v); }
};

// embedded; measure: RaceSounds::PickRivalMusic.ret@+40
struct OpponentItemAt40 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the owner's fault_ field before the measure's fixes (2026-11-22): Unlock::HasSectionCondition's unlock block (+40): its first condition's kind (the item's +50)
  uint16_t block_condition_kind() const { return guest::LoadU16(address + 10); }
  void set_block_condition_kind(uint16_t v) const { guest::StoreU16(address + 10, v); }
  uint32_t block_condition_kind_address() const { return address + 10; }
};

// embedded; measure: [826C5B2C]@+48@+3916
struct RaceEventAt3916 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/race-event.md's code uses.
struct RaceEventGlobals {
  // the race event (RaceEvent::Current)
  template <typename T = RaceEvent>
  static T event() { return T(guest::LoadU32(0x826C6078u)); }
  static void set_event(const View& v) { guest::StoreU32(0x826C6078u, v.address); }
  static void set_event(uint32_t v) { guest::StoreU32(0x826C6078u, v); }
  static constexpr uint32_t event_address() { return 0x826C6078u; }
  // halfwords by location: its group
  static int16_t location_groups() { return int16_t(guest::LoadU16(0x82008D74u)); }
  static void set_location_groups(int16_t v) { guest::StoreU16(0x82008D74u, uint16_t(v)); }
  static constexpr uint32_t location_groups_address() { return 0x82008D74u; }
  // words by mode (kinds 25 at +12 and 26)
  static uint32_t mode_words() { return guest::LoadU32(0x82008D88u); }
  static void set_mode_words(uint32_t v) { guest::StoreU32(0x82008D88u, v); }
  static constexpr uint32_t mode_words_address() { return 0x82008D88u; }
  // words by mode: the items' mode mask
  static uint32_t mode_masks() { return guest::LoadU32(0x82008DF4u); }
  static void set_mode_masks(uint32_t v) { guest::StoreU32(0x82008DF4u, v); }
  static constexpr uint32_t mode_masks_address() { return 0x82008DF4u; }
  // halfwords: locations 0 to 3's caps
  static uint16_t location_caps() { return guest::LoadU16(0x82008E00u); }
  static void set_location_caps(uint16_t v) { guest::StoreU16(0x82008E00u, v); }
  static constexpr uint32_t location_caps_address() { return 0x82008E00u; }
  // bytes: the stage each location needs
  static int8_t location_stages() { return int8_t(guest::LoadU8(0x82008E08u)); }
  static void set_location_stages(int8_t v) { guest::StoreU8(0x82008E08u, uint8_t(v)); }
  static constexpr uint32_t location_stages_address() { return 0x82008E08u; }
  // the course's route (its +8 the length)
  static View route() { return View(guest::LoadU32(0x826C5B10u)); }
  static void set_route(uint32_t v) { guest::StoreU32(0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the race data (+48 the racer records, 3,936 bytes each)
  static View race_data() { return View(guest::LoadU32(0x826C5B2Cu)); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the racers' manager (+2368 the count, 28-byte entries from +4)
  static View racers_manager() { return View(guest::LoadU32(0x826D82F4u)); }
  static void set_racers_manager(uint32_t v) { guest::StoreU32(0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // the floats PlaceUnlocked and SectionRival step positions by
  static float back_step() { return guest::LoadFloat(0x82001FB0u); }
  static void set_back_step(float v) { guest::StoreFloat(0x82001FB0u, v); }
  static constexpr uint32_t back_step_address() { return 0x82001FB0u; }
  static float slice_unit() { return guest::LoadFloat(0x82000A30u); }
  static void set_slice_unit(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t slice_unit_address() { return 0x82000A30u; }
  static float forward_scale() { return guest::LoadFloat(0x82008E34u); }
  static void set_forward_scale(float v) { guest::StoreFloat(0x82008E34u, v); }
  static constexpr uint32_t forward_scale_address() { return 0x82008E34u; }
  static float back_scale() { return guest::LoadFloat(0x82008E38u); }
  static void set_back_scale(float v) { guest::StoreFloat(0x82008E38u, v); }
  static constexpr uint32_t back_scale_address() { return 0x82008E38u; }
};

}  // namespace schema
