// The views of docs/functions/menu-sounds.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/menu-sounds.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// image; measure: [826C5C80].+0, sub_82119188.r3.+0
struct AudioManagerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  // slot 0: AudioManager::~AudioManager (deleting) sub_82119450
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: sub_82117EB8
  uint32_t is_any_bank_loading() const { return guest::LoadU32(base, address + 4); }
  void set_is_any_bank_loading(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t is_any_bank_loading_address() const { return address + 4; }
  // slot 2: sub_82117FF0
  uint32_t find_bank() const { return guest::LoadU32(base, address + 8); }
  void set_find_bank(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t find_bank_address() const { return address + 8; }
  // slot 3: sub_82117F78
  uint32_t find_free_bank_slot() const { return guest::LoadU32(base, address + 12); }
  void set_find_free_bank_slot(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t find_free_bank_slot_address() const { return address + 12; }
  // slot 4: AudioManager::LoadBank
  uint32_t load_bank() const { return guest::LoadU32(base, address + 16); }
  void set_load_bank(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t load_bank_address() const { return address + 16; }
  // slot 5: sub_82118190
  uint32_t release_bank() const { return guest::LoadU32(base, address + 20); }
  void set_release_bank(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t release_bank_address() const { return address + 20; }
  // slot 6: sub_82118200
  uint32_t is_bank_loading() const { return guest::LoadU32(base, address + 24); }
  void set_is_bank_loading(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t is_bank_loading_address() const { return address + 24; }
  // slot 7: sub_82118270
  uint32_t is_bank_loaded() const { return guest::LoadU32(base, address + 28); }
  void set_is_bank_loaded(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t is_bank_loaded_address() const { return address + 28; }
  // slot 8: sub_821182E0
  uint32_t is_bank_slot_empty() const { return guest::LoadU32(base, address + 32); }
  void set_is_bank_slot_empty(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t is_bank_slot_empty_address() const { return address + 32; }
  // slot 9: sub_82118320
  uint32_t get_cue_index() const { return guest::LoadU32(base, address + 36); }
  void set_get_cue_index(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t get_cue_index_address() const { return address + 36; }
  // slot 10: AudioManager::PlayCue2D sub_82119638
  uint32_t play_cue_2d() const { return guest::LoadU32(base, address + 40); }
  void set_play_cue_2d(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t play_cue_2d_address() const { return address + 40; }
  // slot 11: sub_82119608
  uint32_t play_cue_3d() const { return guest::LoadU32(base, address + 44); }
  void set_play_cue_3d(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t play_cue_3d_address() const { return address + 44; }
  // slot 12: sub_821189F0
  uint32_t stop_bank() const { return guest::LoadU32(base, address + 48); }
  void set_stop_bank(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t stop_bank_address() const { return address + 48; }
  // slot 13: AudioManager::StopCue
  uint32_t stop_cue() const { return guest::LoadU32(base, address + 52); }
  void set_stop_cue(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t stop_cue_address() const { return address + 52; }
  // slot 14: sub_82118B58
  uint32_t is_cue_playing() const { return guest::LoadU32(base, address + 56); }
  void set_is_cue_playing(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t is_cue_playing_address() const { return address + 56; }
  // slot 15: sub_82119690
  uint32_t play_handle() const { return guest::LoadU32(base, address + 60); }
  void set_play_handle(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t play_handle_address() const { return address + 60; }
  // slot 16: sub_82118D90
  uint32_t set_handle_position() const { return guest::LoadU32(base, address + 64); }
  void set_set_handle_position(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t set_handle_position_address() const { return address + 64; }
  // slot 17: sub_82118DD0
  uint32_t set_handle_velocity() const { return guest::LoadU32(base, address + 68); }
  void set_set_handle_velocity(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t set_handle_velocity_address() const { return address + 68; }
  // slot 18: sub_82118E10
  uint32_t set_handle_3d() const { return guest::LoadU32(base, address + 72); }
  void set_set_handle_3d(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t set_handle_3d_address() const { return address + 72; }
  // slot 19: sub_82118E50
  uint32_t set_handle_3d_params() const { return guest::LoadU32(base, address + 76); }
  void set_set_handle_3d_params(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t set_handle_3d_params_address() const { return address + 76; }
  // slot 20: sub_82118EA0
  uint32_t start_handle() const { return guest::LoadU32(base, address + 80); }
  void set_start_handle(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t start_handle_address() const { return address + 80; }
  // slot 21: sub_82118ED0
  uint32_t stop_handle() const { return guest::LoadU32(base, address + 84); }
  void set_stop_handle(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t stop_handle_address() const { return address + 84; }
  // slot 22: sub_82118F00
  uint32_t remove_handle() const { return guest::LoadU32(base, address + 88); }
  void set_remove_handle(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t remove_handle_address() const { return address + 88; }
  // slot 23: sub_82118C70
  uint32_t clear_handles() const { return guest::LoadU32(base, address + 92); }
  void set_clear_handles(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t clear_handles_address() const { return address + 92; }
  // slot 24: sub_821190B0
  uint32_t update_handles() const { return guest::LoadU32(base, address + 96); }
  void set_update_handles(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t update_handles_address() const { return address + 96; }
  // slot 25: sub_82117E08
  uint32_t set_listener_position() const { return guest::LoadU32(base, address + 100); }
  void set_set_listener_position(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t set_listener_position_address() const { return address + 100; }
  // slot 26: sub_82117E10
  uint32_t set_listener_orientation() const { return guest::LoadU32(base, address + 104); }
  void set_set_listener_orientation(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t set_listener_orientation_address() const { return address + 104; }
  // slot 27: sub_82117E18
  uint32_t set_listener_velocity() const { return guest::LoadU32(base, address + 108); }
  void set_set_listener_velocity(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t set_listener_velocity_address() const { return address + 108; }
  // slot 28: sub_82117E20
  uint32_t set_listener_params() const { return guest::LoadU32(base, address + 112); }
  void set_set_listener_params(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t set_listener_params_address() const { return address + 112; }
  // slot 29: AudioManager::GetCategory
  uint32_t get_category() const { return guest::LoadU32(base, address + 116); }
  void set_get_category(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t get_category_address() const { return address + 116; }
  // slot 30: AudioManager::SetCategoryVolume
  uint32_t set_category_volume() const { return guest::LoadU32(base, address + 120); }
  void set_set_category_volume(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t set_category_volume_address() const { return address + 120; }
  // slot 31: AudioManager::PauseCategory
  uint32_t pause_category() const { return guest::LoadU32(base, address + 124); }
  void set_pause_category(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t pause_category_address() const { return address + 124; }
  // slot 32: AudioManager::ResumeCategory
  uint32_t resume_category() const { return guest::LoadU32(base, address + 128); }
  void set_resume_category(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t resume_category_address() const { return address + 128; }
};

// allocated; measure: AudioManager.+1544, AudioManager.+1544.+0, HandleList.+4, HandleList.+4.+0
struct SoundHandleNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = SoundHandleNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = SoundHandleNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // a Sound (sound-cues.md's, 24 bytes)
  uint32_t sound() const { return guest::LoadU32(base, address + 8); }
  void set_sound(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sound_address() const { return address + 8; }
};

// embedded; measure: HandleList
struct SoundHandleList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // probably the allocator word (std::list's first)
  uint32_t allocator() const { return guest::LoadU32(base, address + 0); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t allocator_address() const { return address + 0; }
  // the head node
  template <typename T = SoundHandleNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
};

// allocated; measure: AudioManager, [826C5C80], sub_82119188.r3, sub_82118930.r3, sub_82119018.r3
struct AudioManager : View {
  using View::View;
  static constexpr uint32_t kSize = 1624;
  static constexpr uint32_t kVtable = 0x82008388;
  // 0x82008388 (0x820082B8, the base's, in the destructor)
  template <typename T = AudioManagerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82008374 (0x82000D48 first): the interface the bank pairs' owner is (+36 of a BankPair); AudioManager::GetEngine's r3
  uint32_t bank_owner_vtable() const { return guest::LoadU32(base, address + 4); }
  void set_bank_owner_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t bank_owner_vtable_address() const { return address + 4; }
  // 0x82008364 (0x82000D48 first): the interface the Sounds' owner is (Sound +0); AudioManager::GetSoundBank's r3
  uint32_t sound_owner_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_sound_owner_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sound_owner_vtable_address() const { return address + 8; }
  // the constructor's r4; its vtable's slot +8 gives the dispatcher the handler at +1584 is registered with
  uint32_t owner() const { return guest::LoadU32(base, address + 12); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t owner_address() const { return address + 12; }
  // probably a measure fault: the +8 interface's +1508 (the bank pairs' first, +1516)
  uint32_t fault_1508() const { return guest::LoadU32(base, address + 1508); }
  void set_fault_1508(uint32_t v) const { guest::StoreU32(base, address + 1508, v); }
  uint32_t fault_1508_address() const { return address + 1508; }
  // the bank pairs' vector (WordVector::Fill; one slot a bank index): +1512 its allocator word
  uint32_t bank_pairs() const { return guest::LoadU32(base, address + 1512); }
  void set_bank_pairs(uint32_t v) const { guest::StoreU32(base, address + 1512, v); }
  uint32_t bank_pairs_address() const { return address + 1512; }
  // the vector's first slot (a BankPair pointer, 0 for an empty slot)
  uint32_t bank_pairs_first() const { return guest::LoadU32(base, address + 1516); }
  void set_bank_pairs_first(uint32_t v) const { guest::StoreU32(base, address + 1516, v); }
  uint32_t bank_pairs_first_address() const { return address + 1516; }
  // the vector's end
  uint32_t bank_pairs_end() const { return guest::LoadU32(base, address + 1520); }
  void set_bank_pairs_end(uint32_t v) const { guest::StoreU32(base, address + 1520, v); }
  uint32_t bank_pairs_end_address() const { return address + 1520; }
  // the vector's capacity end (probably)
  uint32_t bank_pairs_capacity_end() const { return guest::LoadU32(base, address + 1524); }
  void set_bank_pairs_capacity_end(uint32_t v) const { guest::StoreU32(base, address + 1524, v); }
  uint32_t bank_pairs_capacity_end_address() const { return address + 1524; }
  // the list of playing Sounds (+1528, a std::list): its head node (sound-cues.md)
  uint32_t sounds_head() const { return guest::LoadU32(base, address + 1532); }
  void set_sounds_head(uint32_t v) const { guest::StoreU32(base, address + 1532, v); }
  uint32_t sounds_head_address() const { return address + 1532; }
  // the list of Sounds kept by handle (SoundHandleList; AudioManager::PlayHandle and slots 16 to 24)
  uint32_t handle_sounds_address() const { return address + 1540; }
  // its head node
  template <typename T = SoundHandleNode>
  T handle_sounds_head() const { return T(base, guest::LoadU32(base, address + 1544)); }
  void set_handle_sounds_head(const View& v) const { guest::StoreU32(base, address + 1544, v.address); }
  void set_handle_sounds_head(uint32_t v) const { guest::StoreU32(base, address + 1544, v); }
  uint32_t handle_sounds_head_address() const { return address + 1544; }
  // 1 at first: the handle AudioManager::PlayHandle gives next
  uint32_t next_handle() const { return guest::LoadU32(base, address + 1552); }
  void set_next_handle(uint32_t v) const { guest::StoreU32(base, address + 1552, v); }
  uint32_t next_handle_address() const { return address + 1552; }
  // a std::string (the constructor's r5)
  uint32_t name_address() const { return address + 1556; }
  // the handler object (40 bytes, registered with the owner's dispatcher; its +12 = 3 runs sub_82094B10 each update): 0x820066DC (0x82000EB8 first)
  uint32_t handler_vtable() const { return guest::LoadU32(base, address + 1584); }
  void set_handler_vtable(uint32_t v) const { guest::StoreU32(base, address + 1584, v); }
  uint32_t handler_vtable_address() const { return address + 1584; }
  // the handler's second vtable word: 0x82002908 (0x82000AB4, then 0x82000BA8, first)
  uint32_t handler_vtable2() const { return guest::LoadU32(base, address + 1588); }
  void set_handler_vtable2(uint32_t v) const { guest::StoreU32(base, address + 1588, v); }
  uint32_t handler_vtable2_address() const { return address + 1588; }
  // the handler's +8, 0
  uint32_t handler_8() const { return guest::LoadU32(base, address + 1592); }
  void set_handler_8(uint32_t v) const { guest::StoreU32(base, address + 1592, v); }
  uint32_t handler_8_address() const { return address + 1592; }
  // the handler's +12, 0 (3 runs sub_82094B10 each update, known-data.md)
  uint32_t handler_12() const { return guest::LoadU32(base, address + 1596); }
  void set_handler_12(uint32_t v) const { guest::StoreU32(base, address + 1596, v); }
  uint32_t handler_12_address() const { return address + 1596; }
  // the handler's +16, 0
  uint8_t handler_flag_16() const { return guest::LoadU8(base, address + 1600); }
  void set_handler_flag_16(uint8_t v) const { guest::StoreU8(base, address + 1600, v); }
  uint32_t handler_flag_16_address() const { return address + 1600; }
  // the handler's +17, 1
  uint8_t handler_flag_17() const { return guest::LoadU8(base, address + 1601); }
  void set_handler_flag_17(uint8_t v) const { guest::StoreU8(base, address + 1601, v); }
  uint32_t handler_flag_17_address() const { return address + 1601; }
  // the handler's +18, 0
  uint8_t handler_flag_18() const { return guest::LoadU8(base, address + 1602); }
  void set_handler_flag_18(uint8_t v) const { guest::StoreU8(base, address + 1602, v); }
  uint32_t handler_flag_18_address() const { return address + 1602; }
  // the handler's +20: the constructor's caller's frame word (probably a message id)
  uint32_t handler_message() const { return guest::LoadU32(base, address + 1604); }
  void set_handler_message(uint32_t v) const { guest::StoreU32(base, address + 1604, v); }
  uint32_t handler_message_address() const { return address + 1604; }
  // the handler's delegate (+24, 16 bytes): 0x8200833C (0x82000ABC first)
  uint32_t handler_delegate_vtable() const { return guest::LoadU32(base, address + 1608); }
  void set_handler_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 1608, v); }
  uint32_t handler_delegate_vtable_address() const { return address + 1608; }
  // the delegate's object: the manager
  template <typename T = AudioManager>
  T handler_delegate_object() const { return T(base, guest::LoadU32(base, address + 1612)); }
  void set_handler_delegate_object(const View& v) const { guest::StoreU32(base, address + 1612, v.address); }
  void set_handler_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 1612, v); }
  uint32_t handler_delegate_object_address() const { return address + 1612; }
  // the delegate's member pointer (two words)
  uint64_t handler_delegate_method() const { return guest::LoadU64(base, address + 1616); }
  void set_handler_delegate_method(uint64_t v) const { guest::StoreU64(base, address + 1616, v); }
  uint32_t handler_delegate_method_address() const { return address + 1616; }
};

// image; measure: AudioManager.+8
struct AudioManagerSoundOwnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0 (not read)
  uint32_t slot_0() const { return guest::LoadU32(base, address + 0); }
  void set_slot_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slot_0_address() const { return address + 0; }
};

// image; measure: AudioManager.+8.+0
struct AudioManagerSoundOwnerSlot0 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: Sound::HasHandle.r4
struct SoundHandleWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t handle() const { return guest::LoadU32(base, address + 0); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t handle_address() const { return address + 0; }
};

// embedded; measure: HandleList::Remove.r4
struct SoundHandleListValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t sound() const { return guest::LoadU32(base, address + 0); }
  void set_sound(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t sound_address() const { return address + 0; }
};

// embedded; measure: Listener::SetFloat52.r4
struct ListenerFloat52Arg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// The image words docs/functions/menu-sounds.md's code uses.
struct MenuSoundsGlobals {
  // the sound manager (AudioManager, app + 124; set by SoundGlobals::Init)
  template <typename T = AudioManager>
  static T sound_manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C80u)); }
  static void set_sound_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C80u, v.address); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // 1.0 (copied as a word)
  static uint32_t one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (copied as a word)
  static uint32_t zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1/15: a volume setting's step
  static float fifteenth(uint8_t* base) { return guest::LoadFloat(base, 0x82008060u); }
  static void set_fifteenth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008060u, v); }
  static constexpr uint32_t fifteenth_address() { return 0x82008060u; }
  // "Default": the effects' XACT category
  static constexpr uint32_t default_category_name_address() { return 0x8200A6A4u; }
  // "Music": the music's XACT category
  static constexpr uint32_t music_category_name_address() { return 0x82008348u; }
  // the effects' volume option's gain (a float; the car sounds' output scales by it)
  static uint32_t effects_volume(uint8_t* base) { return guest::LoadU32(base, 0x82616C38u); }
  static void set_effects_volume(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616C38u, v); }
  static constexpr uint32_t effects_volume_address() { return 0x82616C38u; }
  // the effects' gain again (a float; the race's sound handlers read it)
  static uint32_t effects_volume_2(uint8_t* base) { return guest::LoadU32(base, 0x82616C3Cu); }
  static void set_effects_volume_2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616C3Cu, v); }
  static constexpr uint32_t effects_volume_2_address() { return 0x82616C3Cu; }
  // the music's volume (a float; the race's sound handlers fade it)
  static uint32_t music_volume(uint8_t* base) { return guest::LoadU32(base, 0x82616BECu); }
  static void set_music_volume(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82616BECu, v); }
  static constexpr uint32_t music_volume_address() { return 0x82616BECu; }
};

}  // namespace schema
