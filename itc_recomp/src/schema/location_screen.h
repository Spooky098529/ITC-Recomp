// The views of docs/functions/location-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/location-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: LocationScreen, LocationScreen::OnPictureLoadedA.r6, LocationScreen::OnPictureLoadedB.r6
struct LocationScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 912;
  // 0 at reset (also the view's +0 and RaceEvent::GroupItem's event +0 by the measure's joins)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 2 once Page7 left for another screen (message 6); OnMessage3 then posts 9
  uint32_t away_state() const { return guest::LoadU32(base, address + 4); }
  void set_away_state(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t away_state_address() const { return address + 4; }
  // list A: 32 pointers to LocationScreenPicture (asset 22696 + id)
  uint32_t pictures_a_address() const { return address + 8; }
  uint32_t picture_count_a() const { return guest::LoadU32(base, address + 136); }
  void set_picture_count_a(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t picture_count_a_address() const { return address + 136; }
  // ring A: 32 picture ids queued for loading
  uint32_t queued_a_address() const { return address + 140; }
  uint32_t queued_count_a() const { return guest::LoadU32(base, address + 268); }
  void set_queued_count_a(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t queued_count_a_address() const { return address + 268; }
  uint32_t queued_start_a() const { return guest::LoadU32(base, address + 272); }
  void set_queued_start_a(uint32_t v) const { guest::StoreU32(base, address + 272, v); }
  uint32_t queued_start_a_address() const { return address + 272; }
  // list B: 32 pointers to LocationScreenPicture (asset 22741 + id)
  uint32_t pictures_b_address() const { return address + 276; }
  uint32_t picture_count_b() const { return guest::LoadU32(base, address + 404); }
  void set_picture_count_b(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t picture_count_b_address() const { return address + 404; }
  // ring B: 32 picture ids queued for loading
  uint32_t queued_b_address() const { return address + 408; }
  uint32_t queued_count_b() const { return guest::LoadU32(base, address + 536); }
  void set_queued_count_b(uint32_t v) const { guest::StoreU32(base, address + 536, v); }
  uint32_t queued_count_b_address() const { return address + 536; }
  uint32_t queued_start_b() const { return guest::LoadU32(base, address + 540); }
  void set_queued_start_b(uint32_t v) const { guest::StoreU32(base, address + 540, v); }
  uint32_t queued_start_b_address() const { return address + 540; }
  // movie 21's node (6, 1) picture texture, put back by Unload
  uint32_t default_texture_a() const { return guest::LoadU32(base, address + 544); }
  void set_default_texture_a(uint32_t v) const { guest::StoreU32(base, address + 544, v); }
  uint32_t default_texture_a_address() const { return address + 544; }
  // movie 21's node (6, 0) picture texture, put back by Unload
  uint32_t default_texture_b() const { return guest::LoadU32(base, address + 548); }
  void set_default_texture_b(uint32_t v) const { guest::StoreU32(base, address + 548, v); }
  uint32_t default_texture_b_address() const { return address + 548; }
  // the course scenery ([0x826C551C] after Scenery::Create)
  template <typename T = Scenery>
  T scenery() const { return T(base, guest::LoadU32(base, address + 552)); }
  void set_scenery(const View& v) const { guest::StoreU32(base, address + 552, v.address); }
  void set_scenery(uint32_t v) const { guest::StoreU32(base, address + 552, v); }
  uint32_t scenery_address() const { return address + 552; }
  // the course view (LocationScreenView)
  uint32_t view_address() const { return address + 560; }
  // the picked opponent: its index in the race event's group
  uint32_t picked_opponent() const { return guest::LoadU32(base, address + 800); }
  void set_picked_opponent(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t picked_opponent_address() const { return address + 800; }
  // the race event's group kind (4 to 9) by the location
  uint32_t group_kind() const { return guest::LoadU32(base, address + 804); }
  void set_group_kind(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t group_kind_address() const { return address + 804; }
  // the group's opponent count
  uint32_t opponent_count() const { return guest::LoadU32(base, address + 808); }
  void set_opponent_count(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t opponent_count_address() const { return address + 808; }
  // the location record's entry count (save +0x1AD20 + 100 x index, its +96)
  uint32_t list_count() const { return guest::LoadU32(base, address + 812); }
  void set_list_count(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t list_count_address() const { return address + 812; }
  // set when page 0's third item was taken (page 7, another screen): Create puts the menu's cursor back on 2, Start skips the after-race page
  uint32_t item2_taken() const { return guest::LoadU32(base, address + 816); }
  void set_item2_taken(uint32_t v) const { guest::StoreU32(base, address + 816, v); }
  uint32_t item2_taken_address() const { return address + 816; }
  // 1 after a race won with an item of category 3 to 5 (the after-race transitions); read by Page12 and Page13
  uint32_t won_card_item() const { return guest::LoadU32(base, address + 820); }
  void set_won_card_item(uint32_t v) const { guest::StoreU32(base, address + 820, v); }
  uint32_t won_card_item_address() const { return address + 820; }
  // the opponents' scene models: 6 indices into pool 7 (ScenePools::GetB)
  uint32_t opponent_models_address() const { return address + 824; }
  // the location's index (0 to 5): the locations' table row
  uint32_t location_index() const { return guest::LoadU32(base, address + 852); }
  void set_location_index(uint32_t v) const { guest::StoreU32(base, address + 852, v); }
  uint32_t location_index_address() const { return address + 852; }
  // PollSceneTeardown's flag
  uint32_t teardown_started() const { return guest::LoadU32(base, address + 856); }
  void set_teardown_started(uint32_t v) const { guest::StoreU32(base, address + 856, v); }
  uint32_t teardown_started_address() const { return address + 856; }
  // PollSceneTeardown's countdown (5)
  uint32_t teardown_countdown() const { return guest::LoadU32(base, address + 860); }
  void set_teardown_countdown(uint32_t v) const { guest::StoreU32(base, address + 860, v); }
  uint32_t teardown_countdown_address() const { return address + 860; }
  // the save data's location (+6108; 14 to 19)
  uint32_t location() const { return guest::LoadU32(base, address + 864); }
  void set_location(uint32_t v) const { guest::StoreU32(base, address + 864, v); }
  uint32_t location_address() const { return address + 864; }
  // the unlock list's position (ShowUnlockEntry)
  uint32_t unlock_position() const { return guest::LoadU32(base, address + 868); }
  void set_unlock_position(uint32_t v) const { guest::StoreU32(base, address + 868, v); }
  uint32_t unlock_position_address() const { return address + 868; }
  // an entry walk's offset (the 56- or 20-byte entry's index)
  uint32_t entry_offset() const { return guest::LoadU32(base, address + 872); }
  void set_entry_offset(uint32_t v) const { guest::StoreU32(base, address + 872, v); }
  uint32_t entry_offset_address() const { return address + 872; }
  // an entry walk's table (DataTables::Entry56)
  uint32_t entry_table() const { return guest::LoadU32(base, address + 876); }
  void set_entry_table(uint32_t v) const { guest::StoreU32(base, address + 876, v); }
  uint32_t entry_table_address() const { return address + 876; }
  // the page other screens' messages queue (-1 none; ApplyQueuedPage)
  uint32_t queued_page() const { return guest::LoadU32(base, address + 880); }
  void set_queued_page(uint32_t v) const { guest::StoreU32(base, address + 880, v); }
  uint32_t queued_page_address() const { return address + 880; }
  // the transition to it (-1: the page set directly)
  uint32_t queued_transition() const { return guest::LoadU32(base, address + 884); }
  void set_queued_transition(uint32_t v) const { guest::StoreU32(base, address + 884, v); }
  uint32_t queued_transition_address() const { return address + 884; }
  // set by Page4 while NextOpponentText finds more; Transition9 then hides the pictures instead of resetting the opponents' menu
  uint32_t more_opponent_text() const { return guest::LoadU32(base, address + 888); }
  void set_more_opponent_text(uint32_t v) const { guest::StoreU32(base, address + 888, v); }
  uint32_t more_opponent_text_address() const { return address + 888; }
  // 1 when the garage's selected car's spec byte +771 is 1 (page 0's third item refused, SetCarColor 2)
  uint32_t special_car() const { return guest::LoadU32(base, address + 892); }
  void set_special_car(uint32_t v) const { guest::StoreU32(base, address + 892, v); }
  uint32_t special_car_address() const { return address + 892; }
  // 1 once PollLoad started the fade out
  uint32_t load_faded() const { return guest::LoadU32(base, address + 896); }
  void set_load_faded(uint32_t v) const { guest::StoreU32(base, address + 896, v); }
  uint32_t load_faded_address() const { return address + 896; }
  // UpdatePage restarts the page object when set (ModeType5::Resume)
  uint32_t restart() const { return guest::LoadU32(base, address + 900); }
  void set_restart(uint32_t v) const { guest::StoreU32(base, address + 900, v); }
  uint32_t restart_address() const { return address + 900; }
  // measure fault: the machine's (LocationScreenMachine), r3 of the state handlers
  uint32_t machine_state() const { return guest::LoadU32(base, address + 9604); }
  void set_machine_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t machine_state_address() const { return address + 9604; }
  // measure fault: the machine's
  uint32_t machine_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_machine_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t machine_loaded_address() const { return address + 9608; }
  // measure fault: the machine's (also the car settings' machine's at the block's +0, by ModeType5::Init)
  uint32_t machine_reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_machine_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t machine_reenter_address() const { return address + 9612; }
  // measure fault: the machine's (also the car settings')
  uint32_t machine_delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_machine_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t machine_delete_movie_address() const { return address + 9616; }
  // measure fault: the machine's
  uint32_t machine_next_state() const { return guest::LoadU32(base, address + 9620); }
  void set_machine_next_state(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t machine_next_state_address() const { return address + 9620; }
  // measure fault: the type 5 mode's block +19252 (this screen's machine's reenter), by ModeType5::Init
  uint32_t block_location_reenter() const { return guest::LoadU32(base, address + 19252); }
  void set_block_location_reenter(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t block_location_reenter_address() const { return address + 19252; }
  // measure fault: the block's +19256
  uint32_t block_location_delete_movie() const { return guest::LoadU32(base, address + 19256); }
  void set_block_location_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t block_location_delete_movie_address() const { return address + 19256; }
  // measure fault: the block's +28892 (the dialog's machine's reenter)
  uint32_t block_dialog_reenter() const { return guest::LoadU32(base, address + 28892); }
  void set_block_dialog_reenter(uint32_t v) const { guest::StoreU32(base, address + 28892, v); }
  uint32_t block_dialog_reenter_address() const { return address + 28892; }
  // measure fault: the block's +28896
  uint32_t block_dialog_delete_movie() const { return guest::LoadU32(base, address + 28896); }
  void set_block_dialog_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 28896, v); }
  uint32_t block_dialog_delete_movie_address() const { return address + 28896; }
  // measure fault: the block's shown machine's id (3, 16 or 21)
  uint32_t block_shown() const { return guest::LoadU32(base, address + 28920); }
  void set_block_shown(uint32_t v) const { guest::StoreU32(base, address + 28920, v); }
  uint32_t block_shown_address() const { return address + 28920; }
  // measure fault: the block's +28924, 21 by ModeType5::Init (readers not looked for)
  uint32_t block_shown_initial() const { return guest::LoadU32(base, address + 28924); }
  void set_block_shown_initial(uint32_t v) const { guest::StoreU32(base, address + 28924, v); }
  uint32_t block_shown_initial_address() const { return address + 28924; }
};

// embedded; measure: -
struct LocationScreenView : View {
  using View::View;
  static constexpr uint32_t kSize = 240;
  // 0 at reset
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 0 at reset
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // 1 at reset
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // 0 at reset
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // 1 at reset
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // the camera animation's second vector (probably the position: UpdatePage copies it to 0x8265ECE0); (0, 0, 0, 1) at reset
  uint32_t camera_vector_1_address() const { return address + 32; }
  // the camera animation's first vector; (0, 0, 0, 1) at reset
  uint32_t camera_vector_0_address() const { return address + 48; }
  // (0, 0, 0, 1) at reset; readers not looked for
  uint32_t vector_64_address() const { return address + 64; }
  // (0, 0, 0, 1) at reset; readers not looked for
  uint32_t vector_80_address() const { return address + 80; }
  // 0.0 at reset
  float unknown_96() const { return guest::LoadFloat(base, address + 96); }
  void set_unknown_96(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  // 0.0 at reset
  float unknown_100() const { return guest::LoadFloat(base, address + 100); }
  void set_unknown_100(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  // the camera animation's first float (0.0 at reset)
  float camera_float_0() const { return guest::LoadFloat(base, address + 104); }
  void set_camera_float_0(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t camera_float_0_address() const { return address + 104; }
  // the camera animation's second float (0.0 at reset)
  float camera_float_1() const { return guest::LoadFloat(base, address + 108); }
  void set_camera_float_1(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t camera_float_1_address() const { return address + 108; }
  // an identity at reset
  uint32_t matrix0_address() const { return address + 112; }
  // an identity at reset
  uint32_t matrix1_address() const { return address + 176; }
};

// embedded; measure: LocationScreen::Page8.r4, LocationScreen::Page9.r4, LocationScreen::Page11.r4
struct LocationScreenMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9624;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movies 21 and 30, the pictures and the scenery are loaded (PollLoad); Reenter goes to 4 with it, else 1
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // 1 (ModeType5::Init): Create goes to state 1, else 9
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: OnMessage6 deletes movies 21 and 30 and clears `loaded`
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
  // the state Destroy leaves: 11 a race set up, 13 otherwise
  uint32_t next_state() const { return guest::LoadU32(base, address + 9620); }
  void set_next_state(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t next_state_address() const { return address + 9620; }
};

// embedded; measure: LocationScreen::OnMessage3.r4, LocationScreen::Unload.r4
#pragma pack(push, 1)
struct LocationScreenDispatcherData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(LocationScreenDispatcherData) == 812);
inline constexpr memory::LayoutField kLocationScreenDispatcherFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kLocationScreenDispatcherLayout = {kLocationScreenDispatcherFields, 2};

struct LocationScreenDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  LocationScreenDispatcherData* data() const { return static_cast<LocationScreenDispatcherData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static LocationScreenDispatcher New(uint8_t* base) {
    return LocationScreenDispatcher(base, memory::MakeHandle(new LocationScreenDispatcherData(), sizeof(LocationScreenDispatcherData), "LocationScreenDispatcher", &kLocationScreenDispatcherLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kLocationScreenDispatcherLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kLocationScreenDispatcherLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint8_t* messages() const { return on_host() ? reinterpret_cast<uint8_t*>(data()) + 204 : guest::Pointer(base, address + 204); }
  uint32_t message_count() const { return on_host() ? uint32_t(data()->message_count) : uint32_t(guest::LoadU32(base, address + 804)); }
  void set_message_count(uint32_t v) const { if (on_host()) data()->message_count = v; else guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_bits() const { return on_host() ? data()->message_count : guest::LoadU32(base, address + 804); }
  void set_message_count_bits(uint32_t v) const { if (on_host()) data()->message_count = v; else guest::StoreU32(base, address + 804, v); }
  // the ring's first slot
  uint32_t message_start() const { return on_host() ? uint32_t(data()->message_start) : uint32_t(guest::LoadU32(base, address + 808)); }
  void set_message_start(uint32_t v) const { if (on_host()) data()->message_start = v; else guest::StoreU32(base, address + 808, v); }
  uint32_t message_start_bits() const { return on_host() ? data()->message_start : guest::LoadU32(base, address + 808); }
  void set_message_start_bits(uint32_t v) const { if (on_host()) data()->message_start = v; else guest::StoreU32(base, address + 808, v); }
};

// allocated; measure: Picture, LocationScreen.+8, new<16>@LocationScreen::OnPictureLoadedA, new<16>@LocationScreen::OnPictureLoadedB
#pragma pack(push, 1)
struct LocationScreenPictureData {
  uint32_t loaded = 0;  // +0
  uint32_t id = 0;  // +4
  uint32_t texture = 0;  // +8
  uint32_t xpr = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(LocationScreenPictureData) == 16);
inline constexpr memory::LayoutField kLocationScreenPictureFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kLocationScreenPictureLayout = {kLocationScreenPictureFields, 4};

struct LocationScreenPicture : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  LocationScreenPictureData* data() const { return static_cast<LocationScreenPictureData*>(memory::HandleObject(address)); }
  static LocationScreenPicture New(uint8_t* base) {
    return LocationScreenPicture(base, memory::MakeHandle(new LocationScreenPictureData(), sizeof(LocationScreenPictureData), "LocationScreenPicture", &kLocationScreenPictureLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kLocationScreenPictureLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kLocationScreenPictureLayout, offset, width, v); }
  uint32_t loaded() const { return data()->loaded; }
  void set_loaded(uint32_t v) const { data()->loaded = v; }
  uint32_t loaded_bits() const { return data()->loaded; }
  void set_loaded_bits(uint32_t v) const { data()->loaded = v; }
  // the picture id (a 56- or 20-byte data table entry's +16), -1 until the ring's front is taken
  uint32_t id() const { return data()->id; }
  void set_id(uint32_t v) const { data()->id = v; }
  uint32_t id_bits() const { return data()->id; }
  void set_id_bits(uint32_t v) const { data()->id = v; }
  template <typename T = LocationScreenPictureTexture>
  T texture() const { return T(base, data()->texture); }
  void set_texture(const View& w) const { const uint32_t v = w.address; data()->texture = v; }
  void set_texture(uint32_t v) const { data()->texture = v; }
  uint32_t texture_bits() const { return data()->texture; }
  void set_texture_bits(uint32_t v) const { data()->texture = v; }
  // the XPR (24 bytes, XPR::XPR and XPR::Create over the loaded buffer)
  View xpr() const { return View(base, data()->xpr); }
  void set_xpr(uint32_t v) const { data()->xpr = v; }
  uint32_t xpr_bits() const { return data()->xpr; }
  void set_xpr_bits(uint32_t v) const { data()->xpr = v; }
};

// allocated; measure: Picture.+8, new<12>@LocationScreen::OnPictureLoadedA, new<12>@LocationScreen::OnPictureLoadedB, new<16>@LocationScreen::OnPictureLoadedA.+8, new<16>@LocationScreen::OnPictureLoadedB.+8
struct LocationScreenPictureTexture : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the XPR's texture 0 (XPR::GetTexture)
  uint32_t texture() const { return guest::LoadU32(base, address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
  // Texture::GetLevelDesc's +24
  uint32_t width() const { return guest::LoadU32(base, address + 4); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t width_address() const { return address + 4; }
  // Texture::GetLevelDesc's +28
  uint32_t height() const { return guest::LoadU32(base, address + 8); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t height_address() const { return address + 8; }
};

// file; measure: DataTables::Entry20.ret
struct LocationScreenEntry20 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // added to the walk's offset (+872); 0 ends it
  uint32_t next() const { return guest::LoadU32(base, address + 4); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_address() const { return address + 4; }
  // a text id less 4650
  uint32_t text() const { return guest::LoadU32(base, address + 12); }
  void set_text(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t text_address() const { return address + 12; }
  // the picture id (PictureA, PictureB)
  uint32_t picture() const { return guest::LoadU32(base, address + 16); }
  void set_picture(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t picture_address() const { return address + 16; }
};

// image; measure: -
struct LocationScreenLocation : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the view's camera animation id
  uint32_t view_anim() const { return guest::LoadU32(base, address + 0); }
  void set_view_anim(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t view_anim_address() const { return address + 0; }
  // the cars' places' camera animation id
  uint32_t cars_anim() const { return guest::LoadU32(base, address + 4); }
  void set_cars_anim(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t cars_anim_address() const { return address + 4; }
  // the list's camera animation id (UpdateView's)
  uint32_t list_anim() const { return guest::LoadU32(base, address + 8); }
  void set_list_anim(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t list_anim_address() const { return address + 8; }
  // a scenery area (LocationScreen::SceneryConfig)
  uint32_t scenery_area() const { return guest::LoadU32(base, address + 12); }
  void set_scenery_area(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t scenery_area_address() const { return address + 12; }
};

// image; measure: -
struct LocationScreenCard : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // movie 30's first named animation
  uint32_t named_0() const { return guest::LoadU32(base, address + 0); }
  void set_named_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t named_0_address() const { return address + 0; }
  // the second named animation (with a card)
  uint32_t named_1() const { return guest::LoadU32(base, address + 4); }
  void set_named_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t named_1_address() const { return address + 4; }
  // the child of the first animation's node shown
  uint32_t child() const { return guest::LoadU32(base, address + 8); }
  void set_child(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t child_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // text (-1, 12) with a card
  uint32_t text() const { return guest::LoadU32(base, address + 16); }
  void set_text(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t text_address() const { return address + 16; }
  // movie 30's named animation Transition17 plays
  uint32_t card_named() const { return guest::LoadU32(base, address + 20); }
  void set_card_named(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t card_named_address() const { return address + 20; }
};

// embedded; measure: LocationScreen@+804
struct LocationScreenAt804 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: new<28940>@ModeType5::Init
struct LocationScreenPart : View {
  using View::View;
  static constexpr uint32_t kSize = 28928;
  uint32_t unknown_9612() const { return guest::LoadU32(base, address + 9612); }
  void set_unknown_9612(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t unknown_9612_address() const { return address + 9612; }
  uint32_t unknown_9616() const { return guest::LoadU32(base, address + 9616); }
  void set_unknown_9616(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t unknown_9616_address() const { return address + 9616; }
  uint32_t unknown_19252() const { return guest::LoadU32(base, address + 19252); }
  void set_unknown_19252(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t unknown_19252_address() const { return address + 19252; }
  uint32_t unknown_19256() const { return guest::LoadU32(base, address + 19256); }
  void set_unknown_19256(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t unknown_19256_address() const { return address + 19256; }
  uint32_t unknown_28892() const { return guest::LoadU32(base, address + 28892); }
  void set_unknown_28892(uint32_t v) const { guest::StoreU32(base, address + 28892, v); }
  uint32_t unknown_28892_address() const { return address + 28892; }
  uint32_t unknown_28896() const { return guest::LoadU32(base, address + 28896); }
  void set_unknown_28896(uint32_t v) const { guest::StoreU32(base, address + 28896, v); }
  uint32_t unknown_28896_address() const { return address + 28896; }
  uint32_t unknown_28920() const { return guest::LoadU32(base, address + 28920); }
  void set_unknown_28920(uint32_t v) const { guest::StoreU32(base, address + 28920, v); }
  uint32_t unknown_28920_address() const { return address + 28920; }
  uint32_t unknown_28924() const { return guest::LoadU32(base, address + 28924); }
  void set_unknown_28924(uint32_t v) const { guest::StoreU32(base, address + 28924, v); }
  uint32_t unknown_28924_address() const { return address + 28924; }
};

// embedded; measure: LocationScreen@+560
struct LocationScreenAt560 : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint64_t unknown_32() const { return guest::LoadU64(base, address + 32); }
  void set_unknown_32(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint64_t unknown_40() const { return guest::LoadU64(base, address + 40); }
  void set_unknown_40(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint64_t unknown_48() const { return guest::LoadU64(base, address + 48); }
  void set_unknown_48(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint64_t unknown_56() const { return guest::LoadU64(base, address + 56); }
  void set_unknown_56(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint64_t unknown_64() const { return guest::LoadU64(base, address + 64); }
  void set_unknown_64(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint64_t unknown_72() const { return guest::LoadU64(base, address + 72); }
  void set_unknown_72(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint64_t unknown_80() const { return guest::LoadU64(base, address + 80); }
  void set_unknown_80(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint64_t unknown_88() const { return guest::LoadU64(base, address + 88); }
  void set_unknown_88(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  float unknown_96() const { return guest::LoadFloat(base, address + 96); }
  void set_unknown_96(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  float unknown_100() const { return guest::LoadFloat(base, address + 100); }
  void set_unknown_100(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  float unknown_104() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  float unknown_108() const { return guest::LoadFloat(base, address + 108); }
  void set_unknown_108(float v) const { guest::StoreFloat(base, address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
};

// embedded; measure: TrafficCar@+4288
struct LocationScreenAt4288 : View {
  using View::View;
  static constexpr uint32_t kSize = 736;
  uint32_t unknown_732() const { return guest::LoadU32(base, address + 732); }
  void set_unknown_732(uint32_t v) const { guest::StoreU32(base, address + 732, v); }
  uint32_t unknown_732_address() const { return address + 732; }
};

// embedded; measure: new<28940>@ModeType5::Init@+9640
struct LocationScreenAt9640 : View {
  using View::View;
  static constexpr uint32_t kSize = 9608;
  uint32_t unknown_9604() const { return guest::LoadU32(base, address + 9604); }
  void set_unknown_9604(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t unknown_9604_address() const { return address + 9604; }
};

// The image words docs/functions/location-screen.md's code uses.
struct LocationScreenGlobals {
  // the page object: 16 pages, 36 transitions
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E48u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E48u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E48u, v); }
  static constexpr uint32_t pages_address() { return 0x82814E48u; }
  // the location's menu (5 items)
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E4Cu)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E4Cu, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E4Cu, v); }
  static constexpr uint32_t menu_address() { return 0x82814E4Cu; }
  template <typename T = LocationScreen>
  static T screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E50u)); }
  static void set_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E50u, v.address); }
  static void set_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E50u, v); }
  static constexpr uint32_t screen_address() { return 0x82814E50u; }
  // the location's list (list_count items)
  template <typename T = FrontEndMenu>
  static T list_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E54u)); }
  static void set_list_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E54u, v.address); }
  static void set_list_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E54u, v); }
  static constexpr uint32_t list_menu_address() { return 0x82814E54u; }
  // the opponents (opponent_count items)
  template <typename T = FrontEndMenu>
  static T opponent_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E58u)); }
  static void set_opponent_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E58u, v.address); }
  static void set_opponent_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E58u, v); }
  static constexpr uint32_t opponent_menu_address() { return 0x82814E58u; }
  // race, the card, back
  template <typename T = FrontEndMenu>
  static T confirm_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82814E5Cu)); }
  static void set_confirm_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82814E5Cu, v.address); }
  static void set_confirm_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E5Cu, v); }
  static constexpr uint32_t confirm_menu_address() { return 0x82814E5Cu; }
  // 1 on one path of NextResultTextB (never ran); readers not looked for
  static uint32_t result_word(uint8_t* base) { return guest::LoadU32(base, 0x82814E68u); }
  static void set_result_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E68u, v); }
  static constexpr uint32_t result_word_address() { return 0x82814E68u; }
  // 6 LocationScreenLocation
  static constexpr uint32_t locations_address() { return 0x82636518u; }
  // LocationScreenCard rows by location and card category
  static constexpr uint32_t cards_address() { return 0x82636578u; }
  // 8 bytes an entry, by (area + 2) x 3 + the mode (SceneryConfig)
  static constexpr uint32_t scenery_configs_address() { return 0x82602EA0u; }
  // the camera animations' time for each opponent (6 words)
  static constexpr uint32_t opponent_times_address() { return 0x82010780u; }
  // the view's camera_vector_1, copied by UpdatePage
  static constexpr uint32_t view_copy_address() { return 0x8265ECE0u; }
  // 0x826366A0's 16 bytes, by UpdateView
  static constexpr uint32_t light_view_address() { return 0x8265ED00u; }
  // UpdateView's first array (probably the course view's lights)
  static constexpr uint32_t light_array_a_address() { return 0x8296B950u; }
  static constexpr uint32_t light_array_b_address() { return 0x8296B980u; }
  // UpdateView's r3
  static constexpr uint32_t light_array_c_address() { return 0x8296B9B0u; }
  // zeroed by UpdateView
  static constexpr uint32_t light_zeros_a_address() { return 0x8296BAE0u; }
  // zeroed by UpdateView
  static constexpr uint32_t light_zeros_b_address() { return 0x8296BB60u; }
  static constexpr uint32_t light_source_a_address() { return 0x826366A0u; }
  static constexpr uint32_t light_source_b_address() { return 0x826366B0u; }
  static constexpr uint32_t light_source_c_address() { return 0x826366C0u; }
};

}  // namespace schema
