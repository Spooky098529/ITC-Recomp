// The views of docs/functions/save-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/save-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: SaveScreen
struct SaveScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 load the movie, 2 wait for it, 9 idle, 4 Show, 5 shown, 7 leaving, 6 left
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 44 is loaded (WaitMovie)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: OnEnter goes to 1 (load the movie now), else 9 (loaded when opened)
  uint32_t load_at_once() const { return guest::LoadU32(base, address + 9612); }
  void set_load_at_once(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t load_at_once_address() const { return address + 9612; }
};

// embedded; measure: SaveScreen::Leave.r4
struct SaveScreenMode : View {
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

// allocated; measure: new<68>@SaveScreen::OnEnter, new<28>@SaveScreen::OnEnter.+8.+-31660
struct SaveScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  // 1 once a dialog is set up
  uint32_t set_up() const { return guest::LoadU32(base, address + 0); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_up_address() const { return address + 0; }
  // the result the end pages post by (kind 0: 1 one answer, else the other; kinds 2 to 5: 1 to 3, else 0)
  uint32_t answer() const { return guest::LoadU32(base, address + 4); }
  void set_answer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t answer_address() const { return address + 4; }
  // kinds 2 and 3's flag (1 for kind 2), read by page 14
  uint32_t kind2_flag() const { return guest::LoadU32(base, address + 8); }
  void set_kind2_flag(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t kind2_flag_address() const { return address + 8; }
  // the dialog's message (an index into the ShowMessage steps' tables)
  uint32_t message() const { return guest::LoadU32(base, address + 12); }
  void set_message(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t message_address() const { return address + 12; }
  // 0 the question with two answers, 1 and 2 the note, -1 none
  uint32_t dialog_state() const { return guest::LoadU32(base, address + 16); }
  void set_dialog_state(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t dialog_state_address() const { return address + 16; }
  // step 2 (CloseDialog) copies it to dialog_state
  uint32_t state_after_close() const { return guest::LoadU32(base, address + 20); }
  void set_state_after_close(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t state_after_close_address() const { return address + 20; }
  // the pages' wait counter (Draw's calls; 45 or 90)
  uint32_t wait() const { return guest::LoadU32(base, address + 24); }
  void set_wait(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t wait_address() const { return address + 24; }
  // set by SaveScreen::Cancel; Draw turns it into the kind's end page
  uint32_t cancel() const { return guest::LoadU32(base, address + 28); }
  void set_cancel(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t cancel_address() const { return address + 28; }
  // the machine's state after Draw (21 none; the end pages set 9)
  uint32_t next_state() const { return guest::LoadU32(base, address + 32); }
  void set_next_state(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t next_state_address() const { return address + 32; }
  // 0 to 7, set by the open messages' handlers
  uint32_t kind() const { return guest::LoadU32(base, address + 36); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t kind_address() const { return address + 36; }
  // a replay record's index (Replays::Load; the theater's pick)
  uint32_t replay_index() const { return guest::LoadU32(base, address + 40); }
  void set_replay_index(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t replay_index_address() const { return address + 40; }
  // kind 6's player (0, then 1)
  uint32_t player() const { return guest::LoadU32(base, address + 44); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t player_address() const { return address + 44; }
  // the dialog's default answer
  uint32_t default_answer() const { return guest::LoadU32(base, address + 48); }
  void set_default_answer(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t default_answer_address() const { return address + 48; }
  // kind 7's controller pick (0 to 3, the player who pressed)
  uint32_t controller_pick() const { return guest::LoadU32(base, address + 52); }
  void set_controller_pick(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t controller_pick_address() const { return address + 52; }
  // 1 after a set-up (which prompts the theater's WaitPage shows: 0 none, 1 one pair)
  uint32_t prompts() const { return guest::LoadU32(base, address + 56); }
  void set_prompts(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t prompts_address() const { return address + 56; }
  // 1 once SaveScreen::StoreSettings ran
  uint32_t settings_stored() const { return guest::LoadU32(base, address + 60); }
  void set_settings_stored(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t settings_stored_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
};

// The image words docs/functions/save-screen.md's code uses.
struct SaveScreenGlobals {
  // the page block (28 bytes)
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D844Cu)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D844Cu, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D844Cu, v); }
  static constexpr uint32_t pages_address() { return 0x826D844Cu; }
  // the menu list (72 bytes)
  template <typename T = MenuList>
  static T list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8450u)); }
  static void set_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8450u, v.address); }
  static void set_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8450u, v); }
  static constexpr uint32_t list_address() { return 0x826D8450u; }
  template <typename T = SaveScreenData>
  static T data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8454u)); }
  static void set_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8454u, v.address); }
  static void set_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8454u, v); }
  static constexpr uint32_t data_address() { return 0x826D8454u; }
  // the mode type (Modes::), which the end pages post by
  static uint32_t mode_type(uint8_t* base) { return guest::LoadU32(base, 0x826032DCu); }
  static void set_mode_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032DCu, v); }
  static constexpr uint32_t mode_type_address() { return 0x826032DCu; }
  // the save buffers' header (save-interface.toml's SaveBuffers: +0 the game data's buffer, +4 a replay's, +20 the replays' count)
  template <typename T = SaveBuffers>
  static T save_buffers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161F0u)); }
  static void set_save_buffers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161F0u, v.address); }
  static void set_save_buffers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161F0u, v); }
  static constexpr uint32_t save_buffers_address() { return 0x828161F0u; }
  // the replay kept for the replay screen
  static uint32_t kept_replay(uint8_t* base) { return guest::LoadU32(base, 0x826C5B50u); }
  static void set_kept_replay(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B50u, v); }
  static constexpr uint32_t kept_replay_address() { return 0x826C5B50u; }
  // the profile's read (997 while pending)
  static uint32_t profile_read(uint8_t* base) { return guest::LoadU32(base, 0x826C6080u); }
  static void set_profile_read(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6080u, v); }
  static constexpr uint32_t profile_read_address() { return 0x826C6080u; }
  // whether to read the profile
  static uint32_t profile_wanted(uint8_t* base) { return guest::LoadU32(base, 0x826C7958u); }
  static void set_profile_wanted(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7958u, v); }
  static constexpr uint32_t profile_wanted_address() { return 0x826C7958u; }
  // the profile read's results (system-services.toml's SysProfileResults: +4 the settings)
  template <typename T = SysProfileResults>
  static T profile_reader(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C6AD0u)); }
  static void set_profile_reader(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C6AD0u, v.address); }
  static void set_profile_reader(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6AD0u, v); }
  static constexpr uint32_t profile_reader_address() { return 0x826C6AD0u; }
};

}  // namespace schema
