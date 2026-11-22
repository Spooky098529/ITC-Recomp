// The views of docs/functions/race-world.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-world.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Race, RaceManager, RaceManager::MoveUnseen.r4
struct RaceManager : View {
  using View::View;
  static constexpr uint32_t kSize = 363592;
  // 0 when made; also an entry's flags where the measure joins an entry (see above)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 80 entries of 28 bytes (MinimapRacerEntry), one for each opponent and traffic car the race data lists; entry i goes with racer record i
  uint32_t entries_address() const { return address + 4; }
  // the last three entries (77 to 79), the traffic cars
  uint32_t traffic_entries_address() const { return address + 2160; }
  // 16 words: the traffic records' numbers by model (55 to 70; -1 none)
  uint32_t traffic_models_address() const { return address + 2244; }
  // 7 pointers to the entries that have a vehicle (the racers)
  uint32_t racers_address() const { return address + 2308; }
  // the first racer's entry (the player's)
  template <typename T = MinimapRacerEntry>
  T racer0() const { return T(base, guest::LoadU32(base, address + 2308)); }
  void set_racer0(const View& v) const { guest::StoreU32(base, address + 2308, v.address); }
  void set_racer0(uint32_t v) const { guest::StoreU32(base, address + 2308, v); }
  uint32_t racer0_address() const { return address + 2308; }
  // 7 words, one a vehicle: 0 free, -1 freed before, else its ticks in use (from 1, back to 60 after 216,000)
  uint32_t vehicle_slots_address() const { return address + 2336; }
  // the racers listed at +2308 (Race::RacerCount)
  uint32_t racer_count() const { return guest::LoadU32(base, address + 2364); }
  void set_racer_count(uint32_t v) const { guest::StoreU32(base, address + 2364, v); }
  uint32_t racer_count_address() const { return address + 2364; }
  // the entries in use from +4
  uint32_t entry_count() const { return guest::LoadU32(base, address + 2368); }
  void set_entry_count(uint32_t v) const { guest::StoreU32(base, address + 2368, v); }
  uint32_t entry_count_address() const { return address + 2368; }
  // up to 4
  uint32_t opponents_with_vehicle() const { return guest::LoadU32(base, address + 2372); }
  void set_opponents_with_vehicle(uint32_t v) const { guest::StoreU32(base, address + 2372, v); }
  uint32_t opponents_with_vehicle_address() const { return address + 2372; }
  uint32_t traffic_with_vehicle() const { return guest::LoadU32(base, address + 2376); }
  void set_traffic_with_vehicle(uint32_t v) const { guest::StoreU32(base, address + 2376, v); }
  uint32_t traffic_with_vehicle_address() const { return address + 2376; }
  // the race data's +0x59618, at most 3
  uint32_t traffic_allowed() const { return guest::LoadU32(base, address + 2380); }
  void set_traffic_allowed(uint32_t v) const { guest::StoreU32(base, address + 2380, v); }
  uint32_t traffic_allowed_address() const { return address + 2380; }
  // the words in use at +2244
  uint32_t traffic_model_count() const { return guest::LoadU32(base, address + 2384); }
  void set_traffic_model_count(uint32_t v) const { guest::StoreU32(base, address + 2384, v); }
  uint32_t traffic_model_count_address() const { return address + 2384; }
  // ticks since the last traffic car (to 90)
  uint32_t traffic_ticks() const { return guest::LoadU32(base, address + 2388); }
  void set_traffic_ticks(uint32_t v) const { guest::StoreU32(base, address + 2388, v); }
  uint32_t traffic_ticks_address() const { return address + 2388; }
  // 300 records of 1,200 bytes, one a route section: +0 the section, then the sections beside it (RaceManager::BuildNeighbors); not touched by Create
  uint32_t neighbors_address() const { return address + 2392; }
  // 300 words: each record's count (1 plus the sections added)
  uint32_t neighbor_counts_address() const { return address + 362392; }
};

// embedded; measure: RaceWorld
struct RaceWorld : View {
  using View::View;
  static constexpr uint32_t kSize = 11852;
  // the StateMachine (game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  // 1 at set-up; 2 loading, 3 the start, 7 racing, 24 paused, 37 the end (probably)
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 when the race data's +0xA3AF0 is 6 and the current stage's +76 is 2 or more (probably two human cars: the Xbox Live mode's network battle)
  uint32_t two_humans() const { return guest::LoadU32(base, address + 9608); }
  void set_two_humans(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t two_humans_address() const { return address + 9608; }
  // sub_821743A8's (reached only with two_humans; not read)
  uint32_t unknown_9628() const { return guest::LoadU32(base, address + 9628); }
  void set_unknown_9628(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t unknown_9628_address() const { return address + 9628; }
  // sub_821743A8's (not read)
  uint32_t unknown_9632() const { return guest::LoadU32(base, address + 9632); }
  void set_unknown_9632(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t unknown_9632_address() const { return address + 9632; }
  // sub_821743A8's (not read)
  uint32_t unknown_9700() const { return guest::LoadU32(base, address + 9700); }
  void set_unknown_9700(uint32_t v) const { guest::StoreU32(base, address + 9700, v); }
  uint32_t unknown_9700_address() const { return address + 9700; }
  // a pointer sub_821743A8 reads (not read)
  uint32_t unknown_11844() const { return guest::LoadU32(base, address + 11844); }
  void set_unknown_11844(uint32_t v) const { guest::StoreU32(base, address + 11844, v); }
  uint32_t unknown_11844_address() const { return address + 11844; }
  // a pointer sub_821743A8 reads (not read)
  uint32_t unknown_11848() const { return guest::LoadU32(base, address + 11848); }
  void set_unknown_11848(uint32_t v) const { guest::StoreU32(base, address + 11848, v); }
  uint32_t unknown_11848_address() const { return address + 11848; }
};

// embedded; measure: SortedList, SortedList.+1604.+8, SortedList.+1604.+8.+12, SortedList.+1604, SortedList.+1604.+8.+12.+8
struct RaceSortedList : View {
  using View::View;
  static constexpr uint32_t kSize = 1612;
  // 100 RaceSortedNode's, filled in order
  uint32_t nodes_address() const { return address + 0; }
  // node 0's key
  float node0_key() const { return guest::LoadFloat(base, address + 4); }
  void set_node0_key(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t node0_key_address() const { return address + 4; }
  template <typename T = RaceSortedNode>
  T node0_previous() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_node0_previous(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_node0_previous(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node0_previous_address() const { return address + 8; }
  template <typename T = RaceSortedNode>
  T node0_next() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_node0_next(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_node0_next(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t node0_next_address() const { return address + 12; }
  // the node with the lowest key
  template <typename T = RaceSortedNode>
  T first() const { return T(base, guest::LoadU32(base, address + 1600)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 1600, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 1600, v); }
  uint32_t first_address() const { return address + 1600; }
  // where Insert's search starts
  template <typename T = RaceSortedNode>
  T last_inserted() const { return T(base, guest::LoadU32(base, address + 1604)); }
  void set_last_inserted(const View& v) const { guest::StoreU32(base, address + 1604, v.address); }
  void set_last_inserted(uint32_t v) const { guest::StoreU32(base, address + 1604, v); }
  uint32_t last_inserted_address() const { return address + 1604; }
  uint32_t count() const { return guest::LoadU32(base, address + 1608); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 1608, v); }
  uint32_t count_address() const { return address + 1608; }
};

// embedded; measure: -
struct RaceSortedNode : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the item (a racers' manager entry)
  uint32_t item() const { return guest::LoadU32(base, address + 0); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t item_address() const { return address + 0; }
  // ascending
  float key() const { return guest::LoadFloat(base, address + 4); }
  void set_key(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t key_address() const { return address + 4; }
  template <typename T = RaceSortedNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t previous_address() const { return address + 8; }
  template <typename T = RaceSortedNode>
  T next() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t next_address() const { return address + 12; }
};

// embedded; measure: sub_820B6880.ret
struct BodyOrientation : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // x and y (copied as a doubleword)
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  // z and w (copied as a doubleword)
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// embedded; measure: RaceEvent::PlaceEntry.r6, RaceEvent::PlaceGroup.r4, RaceEvent::PlaceStarts.r5
struct RaceStartPosition : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // section and slice as one word
  uint32_t position() const { return guest::LoadU32(base, address + 0); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t position_address() const { return address + 0; }
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
};

// embedded; measure: RaceEvent::PlaceEntry.r5
struct RaceStartEntryIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint16_t index() const { return guest::LoadU16(base, address + 0); }
  void set_index(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::PlaceEntry.r7, RaceEvent::PlaceGroupFixed.r4
struct RaceStartStep : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  // signed
  uint16_t step() const { return guest::LoadU16(base, address + 0); }
  void set_step(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t step_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::PlaceGroup.r6, RaceEvent::PlaceGroupSplit.r6, RaceEvent::PlaceGroupTwoRows.r7, RaceEvent::PlaceGroupFixed.r5
struct RaceStartGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t group() const { return guest::LoadU32(base, address + 0); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t group_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::PlaceGroup.r7
struct RaceStartBothWays : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t both_ways() const { return guest::LoadU32(base, address + 0); }
  void set_both_ways(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t both_ways_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::PlaceGroupSplit.r7
struct RaceStartRow : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t row() const { return guest::LoadU32(base, address + 0); }
  void set_row(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t row_address() const { return address + 0; }
};

// embedded; measure: RaceEvent::PlaceStarts.r4
struct RaceStartKind : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
};

// embedded; measure: RaceManager::JumpSections.r4
struct RaceJumpPlayerPoint : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// embedded; measure: RaceManager::JumpSections.r5
struct RaceJumpBranch : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t branch() const { return guest::LoadU32(base, address + 0); }
  void set_branch(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t branch_address() const { return address + 0; }
};

// embedded; measure: RaceManager::JumpSections.r6, RaceManager::JumpSections.r7
struct RaceJumpFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t flag() const { return guest::LoadU32(base, address + 0); }
  void set_flag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t flag_address() const { return address + 0; }
};

// embedded; measure: RaceManager::Neighbours.r4
struct RaceNeighboursEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t entry() const { return guest::LoadU32(base, address + 0); }
  void set_entry(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t entry_address() const { return address + 0; }
};

// embedded; measure: Race@+48
struct RaceManagerAt48 : View {
  using View::View;
  static constexpr uint32_t kSize = 6014;
  uint32_t unknown_6008() const { return guest::LoadU32(base, address + 6008); }
  void set_unknown_6008(uint32_t v) const { guest::StoreU32(base, address + 6008, v); }
  uint32_t unknown_6008_address() const { return address + 6008; }
  uint16_t unknown_6012() const { return guest::LoadU16(base, address + 6012); }
  void set_unknown_6012(uint16_t v) const { guest::StoreU16(base, address + 6012, v); }
  uint32_t unknown_6012_address() const { return address + 6012; }
};

// embedded; measure: RaceManager@+32
struct RaceManagerAt32 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: RaceManager@+4
struct RaceManagerAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
};

// embedded; measure: RaceManager@+2392
struct RaceManagerAt2392 : View {
  using View::View;
  static constexpr uint32_t kSize = 360004;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_360000() const { return guest::LoadU32(base, address + 360000); }
  void set_unknown_360000(uint32_t v) const { guest::StoreU32(base, address + 360000, v); }
  uint32_t unknown_360000_address() const { return address + 360000; }
};

// embedded; measure: RaceMode@+116812
struct RaceWorldAt116812 : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
  uint32_t unknown_9608() const { return guest::LoadU32(base, address + 9608); }
  void set_unknown_9608(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t unknown_9608_address() const { return address + 9608; }
};

// The image words docs/functions/race-world.md's code uses.
struct RaceWorldGlobals {
  // the racers' manager (0 when none)
  template <typename T = RaceManager>
  static T racers_manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82F4u)); }
  static void set_racers_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82F4u, v.address); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // the race data (a pointer; race-states.toml's RaceData)
  static uint32_t race_data(uint8_t* base) { return guest::LoadU32(base, 0x826C5B2Cu); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race event (a pointer; race-event.toml's RaceEvent)
  static uint32_t race_event(uint8_t* base) { return guest::LoadU32(base, 0x826C6078u); }
  static void set_race_event(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6078u, v); }
  static constexpr uint32_t race_event_address() { return 0x826C6078u; }
  // the course's route (a pointer)
  static uint32_t route(uint8_t* base) { return guest::LoadU32(base, 0x826C5B10u); }
  static void set_route(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the seven vehicles (a pointer)
  static uint32_t vehicles(uint8_t* base) { return guest::LoadU32(base, 0x826C2E34u); }
  static void set_vehicles(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E34u, v); }
  static constexpr uint32_t vehicles_address() { return 0x826C2E34u; }
  // the racers' car input records (a pointer a racer, 20 bytes each; indexed by the racer's low byte)
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // the race's progress (a pointer; its 972-byte results entries from +2972)
  static uint32_t race_progress(uint8_t* base) { return guest::LoadU32(base, 0x826D82E8u); }
  static void set_race_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t race_progress_address() { return 0x826D82E8u; }
  // TrackCollision::Unload's r3 (RaceWorld::Unload)
  static uint32_t unload_object(uint8_t* base) { return guest::LoadU32(base, 0x826C5B28u); }
  static void set_unload_object(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B28u, v); }
  static constexpr uint32_t unload_object_address() { return 0x826C5B28u; }
  // set to the float at 0x82000A34 by RaceManager::ResetWorld
  static float reset_float(uint8_t* base) { return guest::LoadFloat(base, 0x826C2D80u); }
  static void set_reset_float(uint8_t* base, float v) { guest::StoreFloat(base, 0x826C2D80u, v); }
  static constexpr uint32_t reset_float_address() { return 0x826C2D80u; }
  // which racers' results words RaceWorld::Tick clears (255 none); its +2 and +4 halfwords the start's section and slice (RaceStart::Place)
  static uint8_t start_byte(uint8_t* base) { return guest::LoadU8(base, 0x826C6B0Cu); }
  static void set_start_byte(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x826C6B0Cu, v); }
  static constexpr uint32_t start_byte_address() { return 0x826C6B0Cu; }
  // its +4 (0x826C7824): set, the start from an object (RaceStart::Place); picks flags 0x1 or 0x80 in RaceWorld::Tick
  static uint32_t start_object(uint8_t* base) { return guest::LoadU32(base, 0x826C7820u); }
  static void set_start_object(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7820u, v); }
  static constexpr uint32_t start_object_address() { return 0x826C7820u; }
  // picks which human car's flags (0x1 or 0x80) RaceWorld::Tick matches
  static uint32_t human_flag(uint8_t* base) { return guest::LoadU32(base, 0x826C7824u); }
  static void set_human_flag(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7824u, v); }
  static constexpr uint32_t human_flag_address() { return 0x826C7824u; }
  // three floats: a racer's point (RaceManager::NoteMarkedRacer)
  static constexpr uint32_t marked_racer_address() { return 0x826C61A4u; }
  // the physics step's time given a new traffic car
  static float traffic_step(uint8_t* base) { return guest::LoadFloat(base, 0x826193C8u); }
  static void set_traffic_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x826193C8u, v); }
  static constexpr uint32_t traffic_step_address() { return 0x826193C8u; }
  // 0.0 (image)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the sort key of entries flagged 0x10 or 0x400 (image)
  static float far_key(uint8_t* base) { return guest::LoadFloat(base, 0x8200222Cu); }
  static void set_far_key(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200222Cu, v); }
  static constexpr uint32_t far_key_address() { return 0x8200222Cu; }
  // 20.0: the neighbor sections' and the traffic's nearness (image)
  static float near_distance(uint8_t* base) { return guest::LoadFloat(base, 0x82002568u); }
  static void set_near_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002568u, v); }
  static constexpr uint32_t near_distance_address() { return 0x82002568u; }
  // the traffic's radii scale; Neighbours 1's ticks factor (image)
  static float segment_end(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_segment_end(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t segment_end_address() { return 0x820021E8u; }
  // the opponents' radii: the speed x this (image)
  static float opponent_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021F4u); }
  static void set_opponent_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021F4u, v); }
  static constexpr uint32_t opponent_scale_address() { return 0x820021F4u; }
  // the near radii's base (image)
  static float near_base(uint8_t* base) { return guest::LoadFloat(base, 0x82002A28u); }
  static void set_near_base(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A28u, v); }
  static constexpr uint32_t near_base_address() { return 0x82002A28u; }
  // the far radii's base (image)
  static float far_base(uint8_t* base) { return guest::LoadFloat(base, 0x82008914u); }
  static void set_far_base(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008914u, v); }
  static constexpr uint32_t far_base_address() { return 0x82008914u; }
  // the far radii's cap (image)
  static float far_cap(uint8_t* base) { return guest::LoadFloat(base, 0x82003384u); }
  static void set_far_cap(uint8_t* base, float v) { guest::StoreFloat(base, 0x82003384u, v); }
  static constexpr uint32_t far_cap_address() { return 0x82003384u; }
  // the opponents' near radius when capped (image)
  static float opponent_near_cap(uint8_t* base) { return guest::LoadFloat(base, 0x820099CCu); }
  static void set_opponent_near_cap(uint8_t* base, float v) { guest::StoreFloat(base, 0x820099CCu, v); }
  static constexpr uint32_t opponent_near_cap_address() { return 0x820099CCu; }
  // the traffic's near radius when capped (image)
  static float traffic_near_cap(uint8_t* base) { return guest::LoadFloat(base, 0x820099C8u); }
  static void set_traffic_near_cap(uint8_t* base, float v) { guest::StoreFloat(base, 0x820099C8u, v); }
  static constexpr uint32_t traffic_near_cap_address() { return 0x820099C8u; }
  // the traffic's spacing (image)
  static float traffic_gap(uint8_t* base) { return guest::LoadFloat(base, 0x82009968u); }
  static void set_traffic_gap(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009968u, v); }
  static constexpr uint32_t traffic_gap_address() { return 0x82009968u; }
  // the four nearest racers' limit; Refresh's spawn radius (image)
  static float nearest_limit(uint8_t* base) { return guest::LoadFloat(base, 0x8200996Cu); }
  static void set_nearest_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200996Cu, v); }
  static constexpr uint32_t nearest_limit_address() { return 0x8200996Cu; }
  // Refresh: beyond this a racer loses its vehicle (image)
  static float refresh_despawn(uint8_t* base) { return guest::LoadFloat(base, 0x82009970u); }
  static void set_refresh_despawn(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009970u, v); }
  static constexpr uint32_t refresh_despawn_address() { return 0x82009970u; }
  // the traffic's speed placed (and its limit) (image)
  static float traffic_speed(uint8_t* base) { return guest::LoadFloat(base, 0x82009974u); }
  static void set_traffic_speed(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009974u, v); }
  static constexpr uint32_t traffic_speed_address() { return 0x82009974u; }
  // the traffic's offset behind (image)
  static float traffic_back(uint8_t* base) { return guest::LoadFloat(base, 0x820029D0u); }
  static void set_traffic_back(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D0u, v); }
  static constexpr uint32_t traffic_back_address() { return 0x820029D0u; }
  // a slice's ticks, by default (image)
  static float ticks_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820099D4u); }
  static void set_ticks_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820099D4u, v); }
  static constexpr uint32_t ticks_scale_address() { return 0x820099D4u; }
  // Neighbours 2 (one behind): the ticks factor (image)
  static float behind_factor(uint8_t* base) { return guest::LoadFloat(base, 0x82001070u); }
  static void set_behind_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001070u, v); }
  static constexpr uint32_t behind_factor_address() { return 0x82001070u; }
  // Neighbours 3 (both): the ticks factor (image)
  static float both_factor(uint8_t* base) { return guest::LoadFloat(base, 0x820029F0u); }
  static void set_both_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029F0u, v); }
  static constexpr uint32_t both_factor_address() { return 0x820029F0u; }
  // Neighbours: the farthest one ahead (image)
  static float ahead_limit(uint8_t* base) { return guest::LoadFloat(base, 0x82001FC0u); }
  static void set_ahead_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FC0u, v); }
  static constexpr uint32_t ahead_limit_address() { return 0x82001FC0u; }
  // Neighbours: the farthest one behind (image)
  static float behind_limit(uint8_t* base) { return guest::LoadFloat(base, 0x82009798u); }
  static void set_behind_limit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009798u, v); }
  static constexpr uint32_t behind_limit_address() { return 0x82009798u; }
  // JumpSections: the distance from the player, squared and flat (image)
  static float jump_distance(uint8_t* base) { return guest::LoadFloat(base, 0x820099D0u); }
  static void set_jump_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x820099D0u, v); }
  static constexpr uint32_t jump_distance_address() { return 0x820099D0u; }
  // the rolling start's speed (image)
  static float rolling_speed(uint8_t* base) { return guest::LoadFloat(base, 0x820097FCu); }
  static void set_rolling_speed(uint8_t* base, float v) { guest::StoreFloat(base, 0x820097FCu, v); }
  static constexpr uint32_t rolling_speed_address() { return 0x820097FCu; }
  // 1.5: the km/h scaled against the traffic's speed (image)
  static float slow_factor(uint8_t* base) { return guest::LoadFloat(base, 0x820027A8u); }
  static void set_slow_factor(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A8u, v); }
  static constexpr uint32_t slow_factor_address() { return 0x820027A8u; }
  // the start's step back (image)
  static float start_back_step(uint8_t* base) { return guest::LoadFloat(base, 0x82008E3Cu); }
  static void set_start_back_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008E3Cu, v); }
  static constexpr uint32_t start_back_step_address() { return 0x82008E3Cu; }
  // the step between start entries (image)
  static float start_group_step(uint8_t* base) { return guest::LoadFloat(base, 0x82008E34u); }
  static void set_start_group_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008E34u, v); }
  static constexpr uint32_t start_group_step_address() { return 0x82008E34u; }
  // a halfword a row (0 to 5): an index into start_split_sections, 0x8000 or more for none (image)
  static constexpr uint32_t start_rows_address() { return 0x82008E14u; }
  // the split groups' sections, halfwords (image)
  static uint16_t start_split_sections(uint8_t* base) { return guest::LoadU16(base, 0x82008DE8u); }
  static void set_start_split_sections(uint8_t* base, uint16_t v) { guest::StoreU16(base, 0x82008DE8u, v); }
  static constexpr uint32_t start_split_sections_address() { return 0x82008DE8u; }
};

}  // namespace schema
