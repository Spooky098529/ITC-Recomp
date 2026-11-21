// The views of docs/functions/track-zones.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/track-zones.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RouteRange, RouteRange::Advance.r4
struct RouteRange : View {
  using View::View;
  static constexpr uint32_t kSize = 1064;
  // 0 off, 1 set up or outside, 2 on the range (RouteRange::Update)
  uint32_t state() const { return guest::LoadU32(base, address + 0); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t state_address() const { return address + 0; }
  // the start: a section of the course's route (0x826C5B10)
  uint16_t start_section() const { return guest::LoadU16(base, address + 4); }
  void set_start_section(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t start_section_address() const { return address + 4; }
  // the start's slice in it
  uint16_t start_slice() const { return guest::LoadU16(base, address + 6); }
  void set_start_slice(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t start_slice_address() const { return address + 6; }
  uint16_t end_section() const { return guest::LoadU16(base, address + 8); }
  void set_end_section(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t end_section_address() const { return address + 8; }
  uint16_t end_slice() const { return guest::LoadU16(base, address + 10); }
  void set_end_slice(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t end_slice_address() const { return address + 10; }
  // the sections to take at forks: a List64 from +12, its 64 nodes
  uint32_t take_nodes_address() const { return address + 12; }
  // the take list's free head (its +256)
  uint16_t take_free() const { return guest::LoadU16(base, address + 268); }
  void set_take_free(uint16_t v) const { guest::StoreU16(base, address + 268, v); }
  uint32_t take_free_address() const { return address + 268; }
  // the take list's +258: -1 at reset, not read
  uint16_t take_258() const { return guest::LoadU16(base, address + 270); }
  void set_take_258(uint16_t v) const { guest::StoreU16(base, address + 270, v); }
  uint32_t take_258_address() const { return address + 270; }
  // the take list's head (its +260)
  uint16_t take_head() const { return guest::LoadU16(base, address + 272); }
  void set_take_head(uint16_t v) const { guest::StoreU16(base, address + 272, v); }
  uint32_t take_head_address() const { return address + 272; }
  // the take list's +262: -1 at reset, not read (probably the tail)
  uint16_t take_262() const { return guest::LoadU16(base, address + 274); }
  void set_take_262(uint16_t v) const { guest::StoreU16(base, address + 274, v); }
  uint32_t take_262_address() const { return address + 274; }
  // the take list's count (its +264)
  uint16_t take_count() const { return guest::LoadU16(base, address + 276); }
  void set_take_count(uint16_t v) const { guest::StoreU16(base, address + 276, v); }
  uint32_t take_count_address() const { return address + 276; }
  // the take list's 64 words: sections
  uint32_t take_sections_address() const { return address + 280; }
  // the sections to leave out: a List64 from +536 (empty in play), its nodes
  uint32_t leave_nodes_address() const { return address + 536; }
  uint16_t leave_free() const { return guest::LoadU16(base, address + 792); }
  void set_leave_free(uint16_t v) const { guest::StoreU16(base, address + 792, v); }
  uint32_t leave_free_address() const { return address + 792; }
  // -1 at reset, not read
  uint16_t leave_258() const { return guest::LoadU16(base, address + 794); }
  void set_leave_258(uint16_t v) const { guest::StoreU16(base, address + 794, v); }
  uint32_t leave_258_address() const { return address + 794; }
  uint16_t leave_head() const { return guest::LoadU16(base, address + 796); }
  void set_leave_head(uint16_t v) const { guest::StoreU16(base, address + 796, v); }
  uint32_t leave_head_address() const { return address + 796; }
  // -1 at reset, not read
  uint16_t leave_262() const { return guest::LoadU16(base, address + 798); }
  void set_leave_262(uint16_t v) const { guest::StoreU16(base, address + 798, v); }
  uint32_t leave_262_address() const { return address + 798; }
  uint16_t leave_count() const { return guest::LoadU16(base, address + 800); }
  void set_leave_count(uint16_t v) const { guest::StoreU16(base, address + 800, v); }
  uint32_t leave_count_address() const { return address + 800; }
  // the leave list's 64 words: sections
  uint32_t leave_sections_address() const { return address + 804; }
  // 1 to check the sections beside the ends (links 6 to 9)
  uint32_t check_beside() const { return guest::LoadU32(base, address + 1060); }
  void set_check_beside(uint32_t v) const { guest::StoreU32(base, address + 1060, v); }
  uint32_t check_beside_address() const { return address + 1060; }
  // measure fault: RouteRange::Advance's and ToNeighbor's r3 is an out position on the stack (section at +0), joined with the range; tools/type_map.py should split it
  uint16_t fault_out_section() const { return guest::LoadU16(base, address + 0); }
  void set_fault_out_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t fault_out_section_address() const { return address + 0; }
  // measure fault: the out position's slice at +2 (Advance, ToNeighbor)
  uint16_t fault_out_slice() const { return guest::LoadU16(base, address + 2); }
  void set_fault_out_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_out_slice_address() const { return address + 2; }
};

// allocated; measure: TrackZones
struct TrackZones : View {
  using View::View;
  static constexpr uint32_t kSize = 1199156;
  // the zone list (a List128 from +0): its 128 nodes
  uint32_t nodes_address() const { return address + 0; }
  // the zone list's free head
  uint16_t free() const { return guest::LoadU16(base, address + 512); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 512, v); }
  uint32_t free_address() const { return address + 512; }
  // -1 at reset, not read
  uint16_t list_514() const { return guest::LoadU16(base, address + 514); }
  void set_list_514(uint16_t v) const { guest::StoreU16(base, address + 514, v); }
  uint32_t list_514_address() const { return address + 514; }
  // the zone list's head
  uint16_t head() const { return guest::LoadU16(base, address + 516); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 516, v); }
  uint32_t head_address() const { return address + 516; }
  // -1 at reset, not read
  uint16_t list_518() const { return guest::LoadU16(base, address + 518); }
  void set_list_518(uint16_t v) const { guest::StoreU16(base, address + 518, v); }
  uint32_t list_518_address() const { return address + 518; }
  // the zone list's count (70 once made)
  uint16_t count() const { return guest::LoadU16(base, address + 520); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 520, v); }
  uint32_t count_address() const { return address + 520; }
  // the zone list's 128 words: the zones' addresses
  uint32_t listed_zones_address() const { return address + 524; }
  // the 70 zones (TrackZone), 17,116 bytes apart; zone i's vtable 0x82011AF0 + 12 x i
  uint32_t zones_address() const { return address + 1036; }
};

// embedded; measure: Zone, Zone0, Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10, Zone11, Zone12, Zone13, Zone14, Zone15, Zone16, Zone17, Zone18, Zone19, Zone20, Zone21, Zone22, Zone23, Zone24, Zone25, Zone26, Zone27, Zone28, Zone29, Zone30, Zone31, Zone32, Zone33, Zone34, Zone35, Zone36, Zone37, Zone38, Zone39, Zone40, Zone41, Zone42, Zone43, Zone44, Zone45, Zone46, Zone47, Zone48, Zone49, Zone50, Zone51, Zone52, Zone53, Zone54, Zone55, Zone56, Zone57, Zone58, Zone59, Zone60, Zone61, Zone62, Zone63, Zone64, Zone65, Zone66, Zone67, Zone68, Zone69
struct TrackZone : View {
  using View::View;
  static constexpr uint32_t kSize = 17116;
  // 0x82011AF0 + 12 x the zone: its set-up, Zone::Update, Zone::IsInside
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the range list (a 16-slot list from +4, TrackZoneRangeList): its nodes
  uint32_t range_nodes_address() const { return address + 4; }
  // the range list's free head
  uint16_t range_free() const { return guest::LoadU16(base, address + 68); }
  void set_range_free(uint16_t v) const { guest::StoreU16(base, address + 68, v); }
  uint32_t range_free_address() const { return address + 68; }
  // the range list's +66: -1 at reset, not read
  uint16_t range_list_66() const { return guest::LoadU16(base, address + 70); }
  void set_range_list_66(uint16_t v) const { guest::StoreU16(base, address + 70, v); }
  uint32_t range_list_66_address() const { return address + 70; }
  // the range list's head
  uint16_t range_head() const { return guest::LoadU16(base, address + 72); }
  void set_range_head(uint16_t v) const { guest::StoreU16(base, address + 72, v); }
  uint32_t range_head_address() const { return address + 72; }
  // the range list's +70: -1 at reset, not read
  uint16_t range_list_70() const { return guest::LoadU16(base, address + 74); }
  void set_range_list_70(uint16_t v) const { guest::StoreU16(base, address + 74, v); }
  uint32_t range_list_70_address() const { return address + 74; }
  // the range list's count
  uint16_t range_count() const { return guest::LoadU16(base, address + 76); }
  void set_range_count(uint16_t v) const { guest::StoreU16(base, address + 76, v); }
  uint32_t range_count_address() const { return address + 76; }
  // the range list's 16 RouteRanges, 1,064 bytes apart
  uint32_t ranges_address() const { return address + 80; }
  // its position in the owner's list
  uint32_t index() const { return guest::LoadU32(base, address + 17104); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 17104, v); }
  uint32_t index_address() const { return address + 17104; }
  // the vehicle checked (Racers::Vehicle)
  uint32_t vehicle() const { return guest::LoadU32(base, address + 17108); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 17108, v); }
  uint32_t vehicle_address() const { return address + 17108; }
  // 1 while the vehicle's track position is in one of its ranges
  uint32_t inside() const { return guest::LoadU32(base, address + 17112); }
  void set_inside(uint32_t v) const { guest::StoreU32(base, address + 17112, v); }
  uint32_t inside_address() const { return address + 17112; }
};

// embedded; measure: ZoneRanges
struct TrackZoneRangeList : View {
  using View::View;
  static constexpr uint32_t kSize = 17100;
  // 16 nodes: (next, slot) halfwords
  uint32_t nodes_address() const { return address + 0; }
  // the free list's head
  uint16_t free() const { return guest::LoadU16(base, address + 64); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 64, v); }
  uint32_t free_address() const { return address + 64; }
  // -1 at reset, not read
  uint16_t list_66() const { return guest::LoadU16(base, address + 66); }
  void set_list_66(uint16_t v) const { guest::StoreU16(base, address + 66, v); }
  uint32_t list_66_address() const { return address + 66; }
  uint16_t head() const { return guest::LoadU16(base, address + 68); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 68, v); }
  uint32_t head_address() const { return address + 68; }
  // -1 at reset, not read
  uint16_t list_70() const { return guest::LoadU16(base, address + 70); }
  void set_list_70(uint16_t v) const { guest::StoreU16(base, address + 70, v); }
  uint32_t list_70_address() const { return address + 70; }
  uint16_t count() const { return guest::LoadU16(base, address + 72); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 72, v); }
  uint32_t count_address() const { return address + 72; }
  // 16 RouteRanges, 1,064 bytes apart
  uint32_t ranges_address() const { return address + 76; }
};

// embedded; measure: List128
struct List128 : View {
  using View::View;
  static constexpr uint32_t kSize = 1036;
  // 128 nodes: (next, slot) halfwords
  uint32_t nodes_address() const { return address + 0; }
  // the free list's head
  uint16_t free() const { return guest::LoadU16(base, address + 512); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 512, v); }
  uint32_t free_address() const { return address + 512; }
  // -1 at reset, not read
  uint16_t list_514() const { return guest::LoadU16(base, address + 514); }
  void set_list_514(uint16_t v) const { guest::StoreU16(base, address + 514, v); }
  uint32_t list_514_address() const { return address + 514; }
  uint16_t head() const { return guest::LoadU16(base, address + 516); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 516, v); }
  uint32_t head_address() const { return address + 516; }
  // -1 at reset, not read
  uint16_t list_518() const { return guest::LoadU16(base, address + 518); }
  void set_list_518(uint16_t v) const { guest::StoreU16(base, address + 518, v); }
  uint32_t list_518_address() const { return address + 518; }
  uint16_t count() const { return guest::LoadU16(base, address + 520); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 520, v); }
  uint32_t count_address() const { return address + 520; }
  // the 128 data words
  uint32_t words_address() const { return address + 524; }
};

// embedded; measure: List64
struct List64 : View {
  using View::View;
  static constexpr uint32_t kSize = 524;
  // 64 nodes: (next, slot) halfwords
  uint32_t nodes_address() const { return address + 0; }
  // the free list's head
  uint16_t free() const { return guest::LoadU16(base, address + 256); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 256, v); }
  uint32_t free_address() const { return address + 256; }
  // -1 at reset, not read
  uint16_t list_258() const { return guest::LoadU16(base, address + 258); }
  void set_list_258(uint16_t v) const { guest::StoreU16(base, address + 258, v); }
  uint32_t list_258_address() const { return address + 258; }
  uint16_t head() const { return guest::LoadU16(base, address + 260); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 260, v); }
  uint32_t head_address() const { return address + 260; }
  // -1 at reset, not read
  uint16_t list_262() const { return guest::LoadU16(base, address + 262); }
  void set_list_262(uint16_t v) const { guest::StoreU16(base, address + 262, v); }
  uint32_t list_262_address() const { return address + 262; }
  uint16_t count() const { return guest::LoadU16(base, address + 264); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 264, v); }
  uint32_t count_address() const { return address + 264; }
  // the 64 data words
  uint32_t words_address() const { return address + 268; }
};

// embedded; measure: RouteRange::OnPath.r4, RouteRange::Test.r4, RouteRange::ToNeighbor.r5, RouteRange::Update.r4
struct TrackZonePosition : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // a section of the course's route
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the slice in it
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
};

// embedded; measure: RouteRange::Init.r4
struct RouteRangeEnds : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint16_t start_section() const { return guest::LoadU16(base, address + 0); }
  void set_start_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t start_section_address() const { return address + 0; }
  uint16_t start_slice() const { return guest::LoadU16(base, address + 2); }
  void set_start_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t start_slice_address() const { return address + 2; }
  uint16_t end_section() const { return guest::LoadU16(base, address + 4); }
  void set_end_section(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t end_section_address() const { return address + 4; }
  uint16_t end_slice() const { return guest::LoadU16(base, address + 6); }
  void set_end_slice(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t end_slice_address() const { return address + 6; }
};

// embedded; measure: List64::Insert.r4, List128::Insert.r4
struct TrackZonesListWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: Zone::Reset.r4
struct TrackZoneSetupArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  uint32_t vehicle() const { return guest::LoadU32(base, address + 4); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vehicle_address() const { return address + 4; }
};

// The image words docs/functions/track-zones.md's code uses.
struct TrackZonesGlobals {
  // the track zones' owner (0 when none)
  template <typename T = TrackZones>
  static T track_zones(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161ECu)); }
  static void set_track_zones(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161ECu, v.address); }
  static void set_track_zones(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161ECu, v); }
  static constexpr uint32_t track_zones_address() { return 0x828161ECu; }
  // the slices counted along a route range's path (RouteRange::OnPath)
  static uint32_t slice_count(uint8_t* base) { return guest::LoadU32(base, 0x82603274u); }
  static void set_slice_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82603274u, v); }
  static constexpr uint32_t slice_count_address() { return 0x82603274u; }
  // the 70 zones' vtables (.rdata, 12 bytes each)
  static constexpr uint32_t zone_vtables_address() { return 0x82011AF0u; }
  // the course's route (track-route.toml's TrackRoute, embedded): its file, +12 the sections table
  template <typename T = TrackRouteFile>
  static T route(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B10u)); }
  static void set_route(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B10u, v.address); }
  static void set_route(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the race data (race-states.toml): +0x59614 the racers added, +0x59620 the stages, +0xA3AF0 the kind
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the racers' manager (race-world.toml): +2308 the racers' entries
  template <typename T = RaceManager>
  static T racers_manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_racers_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // 0 for player 2's vehicle in mode 6 (TrackZones::Create)
  static uint32_t human_flag(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_human_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t human_flag_address() { return 0x826C7824u; }
};

}  // namespace schema
