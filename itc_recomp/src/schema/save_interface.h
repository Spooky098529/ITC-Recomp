// The views of docs/functions/save-interface.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/save-interface.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: SaveDevices, sub_820A57D0.r3, sub_820A5848.r3, sub_820A5860.r3
struct SaveDevices : View {
  using View::View;
  static constexpr uint32_t kSize = 116;
  static constexpr uint32_t kVtable = 0x82002198;
  // 0x82002198 (0x82002148 the base's, left by the destructor)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the tick handler's key (the set-up block's +8, 1)
  uint32_t tick_key() const { return guest::LoadU32(base, address + 4); }
  void set_tick_key(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t tick_key_address() const { return address + 4; }
  // the listener's key (the set-up block's +12, 1)
  uint32_t listener_key() const { return guest::LoadU32(base, address + 8); }
  void set_listener_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t listener_key_address() const { return address + 8; }
  // the framework's +56 interface (the set-up block's +4): the device list through its slot 1
  uint32_t device_list() const { return guest::LoadU32(base, address + 12); }
  void set_device_list(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t device_list_address() const { return address + 12; }
  // the content's root name: 0x820021E0's string at the constructor, then "TXR360" (SaveDevices::SetRootName)
  uint32_t root_name_address() const { return address + 16; }
  // the four 1,000-byte slots (the operator new[] block + 8; 0 when it failed)
  template <typename T = SaveSlot>
  T slots() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_slots(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_slots(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slots_address() const { return address + 44; }
  // 4
  uint32_t slot_count() const { return guest::LoadU32(base, address + 48); }
  void set_slot_count(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot_count_address() const { return address + 48; }
  // 512
  uint32_t device_selector_flags() const { return guest::LoadU32(base, address + 52); }
  void set_device_selector_flags(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t device_selector_flags_address() const { return address + 52; }
  // the storage devices' notification listener (user::kNoListener since B5); Update runs only with one
  uint32_t listener() const { return guest::LoadU32(base, address + 56); }
  void set_listener(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t listener_address() const { return address + 56; }
  // 1,024: the save's size for XContentCalculateSize (sub_82295C18)
  uint32_t content_bytes() const { return guest::LoadU32(base, address + 60); }
  void set_content_bytes(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t content_bytes_address() const { return address + 60; }
  // 1: the count for XContentCalculateSize
  uint32_t content_count() const { return guest::LoadU32(base, address + 64); }
  void set_content_count(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t content_count_address() const { return address + 64; }
  // 100: each slot's buffers' size (SaveSlot::AllocBuffers)
  uint32_t saves_to_enumerate() const { return guest::LoadU32(base, address + 68); }
  void set_saves_to_enumerate(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t saves_to_enumerate_address() const { return address + 68; }
  // the slot whose check, save, load or delete is running (0 none; states 6 to 9 claim it)
  uint32_t busy_slot() const { return guest::LoadU32(base, address + 72); }
  void set_busy_slot(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t busy_slot_address() const { return address + 72; }
  // state 0's handler sub_820A4870; the handlers are +76 + 4 x the slot's state
  uint32_t wait_for_user() const { return guest::LoadU32(base, address + 76); }
  void set_wait_for_user(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t wait_for_user_address() const { return address + 76; }
  // state 1: sub_820A5878
  uint32_t wait_for_device() const { return guest::LoadU32(base, address + 80); }
  void set_wait_for_device(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t wait_for_device_address() const { return address + 80; }
  // state 2: sub_820A4890
  uint32_t failed() const { return guest::LoadU32(base, address + 84); }
  void set_failed(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t failed_address() const { return address + 84; }
  // state 3: sub_820A58D8
  uint32_t ready() const { return guest::LoadU32(base, address + 88); }
  void set_ready(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t ready_address() const { return address + 88; }
  // state 4: sub_820A48B8 (files-and-saves.md)
  uint32_t wait_device_selector() const { return guest::LoadU32(base, address + 92); }
  void set_wait_device_selector(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t wait_device_selector_address() const { return address + 92; }
  // state 5: sub_820A59C0
  uint32_t enumerate_step() const { return guest::LoadU32(base, address + 96); }
  void set_enumerate_step(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t enumerate_step_address() const { return address + 96; }
  // state 6: sub_820A5A88
  uint32_t check_step() const { return guest::LoadU32(base, address + 100); }
  void set_check_step(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t check_step_address() const { return address + 100; }
  // state 7: sub_820A5BE8 (files-and-saves.md)
  uint32_t save_step() const { return guest::LoadU32(base, address + 104); }
  void set_save_step(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t save_step_address() const { return address + 104; }
  // state 8: sub_820A5E68 (files-and-saves.md)
  uint32_t load_step() const { return guest::LoadU32(base, address + 108); }
  void set_load_step(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t load_step_address() const { return address + 108; }
  // state 9: sub_820A6010
  uint32_t delete_step() const { return guest::LoadU32(base, address + 112); }
  void set_delete_step(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t delete_step_address() const { return address + 112; }
};

// embedded; measure: SaveDevices::SaveDevices.r4
struct SaveDevicesSetUp : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the run loop (its slots 2 and 1 add the tick handler and the listener)
  template <typename T = FrameDriverRunLoopRef>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // the framework's +56 interface: into the save devices' +12
  uint32_t device_list() const { return guest::LoadU32(base, address + 4); }
  void set_device_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t device_list_address() const { return address + 4; }
  // 1: into the save devices' +4
  uint32_t tick_key() const { return guest::LoadU32(base, address + 8); }
  void set_tick_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tick_key_address() const { return address + 8; }
  // 1: into the save devices' +8
  uint32_t listener_key() const { return guest::LoadU32(base, address + 12); }
  void set_listener_key(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t listener_key_address() const { return address + 12; }
};

// allocated; measure: new<4008>@SaveDevices::SaveDevices
struct SaveSlotArrayBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 4008;
  // 4; the first slot's array_count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // four 1,000-byte SaveSlots (vtable 0x82002190); the save devices' +44 points here
  uint32_t slots_address() const { return address + 8; }
};

// embedded; measure: SaveDevices@+44
struct SaveSlotArrayHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the first slot (0 none): the array's count at -8, deleted and cleared
  template <typename T = SaveSlot>
  T slots() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_slots(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_slots(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slots_address() const { return address + 0; }
};

// allocated; measure: new<24>@SaveBuffers::Create
struct SaveBuffers : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // the game data's buffer (129,400 bytes, the save data's layout: career.toml's CareerSave; options-load-save.toml's OptionsLoadedGameData)
  uint32_t game_data() const { return guest::LoadU32(base, address + 0); }
  void set_game_data(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t game_data_address() const { return address + 0; }
  // a replay's buffer (1,231,088 bytes)
  uint32_t replay() const { return guest::LoadU32(base, address + 4); }
  void set_replay(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t replay_address() const { return address + 4; }
  // the 36-byte replay record being saved
  template <typename T = SaveReplayRecord>
  T replay_record() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_replay_record(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_replay_record(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t replay_record_address() const { return address + 8; }
  // the replays' records, 36 bytes each (Replays::BuildList; VirtualFree'd)
  template <typename T = SaveReplayRecord>
  T records() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_records(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_records(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t records_address() const { return address + 12; }
  // their list entries, 304 bytes each
  template <typename T = SaveReplayEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t entries_address() const { return address + 16; }
  // the records' count
  uint32_t count() const { return guest::LoadU32(base, address + 20); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t count_address() const { return address + 20; }
};

// allocated; measure: new<24>@SaveBuffers::Create.+16
struct SaveReplayEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 304;
  // the rep_... file name
  uint32_t file_name_address() const { return address + 0; }
  // 128 UTF-16 characters
  uint32_t display_name_address() const { return address + 42; }
  // the replay's number (0 the empty entry Replays::BuildList adds first)
  uint32_t number() const { return guest::LoadU32(base, address + 300); }
  void set_number(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t number_address() const { return address + 300; }
};

// allocated; measure: Replays
struct SaveReplayRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // read as a word (a pointer by the measure, 4 loads in 2 functions; not known); the record's +0 is a halfword below
  uint32_t word_0() const { return guest::LoadU32(base, address + 0); }
  void set_word_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_0_address() const { return address + 0; }
  // probably, as in a SYSTEMTIME
  uint16_t year() const { return guest::LoadU16(base, address + 0); }
  void set_year(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t year_address() const { return address + 0; }
  // probably
  uint16_t month() const { return guest::LoadU16(base, address + 2); }
  void set_month(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t month_address() const { return address + 2; }
  // probably
  uint16_t day() const { return guest::LoadU16(base, address + 6); }
  void set_day(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t day_address() const { return address + 6; }
  // probably
  uint16_t hour() const { return guest::LoadU16(base, address + 8); }
  void set_hour(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t hour_address() const { return address + 8; }
  // probably
  uint16_t minute() const { return guest::LoadU16(base, address + 10); }
  void set_minute(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t minute_address() const { return address + 10; }
  // a time packed as minutes (bits 16-30), seconds (10-15) and thousandths (0-9)
  uint32_t time() const { return guest::LoadU32(base, address + 16); }
  void set_time(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t time_address() const { return address + 16; }
  // QUEST, TIME ATTACK, VS BATTLE, NETWORK TIME ATTACK, ... (0x826380F8)
  uint32_t mode() const { return guest::LoadU32(base, address + 20); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t mode_address() const { return address + 20; }
  // probably
  uint32_t course() const { return guest::LoadU32(base, address + 24); }
  void set_course(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t course_address() const { return address + 24; }
  // the replay's number (the list's sort key; 0 the empty record)
  uint32_t number() const { return guest::LoadU32(base, address + 28); }
  void set_number(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t number_address() const { return address + 28; }
  // probably
  uint32_t result() const { return guest::LoadU32(base, address + 32); }
  void set_result(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t result_address() const { return address + 32; }
  // the next record's number (+36 +28), as Replays::Sort's first compare reads it
  uint32_t next_number() const { return guest::LoadU32(base, address + 64); }
  void set_next_number(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t next_number_address() const { return address + 64; }
};

// allocated; measure: Users, sub_82490B08.r3
#pragma pack(push, 1)
struct SignedInUsersData {
  uint8_t users[160] = {};  // +0, guest order
  uint8_t pad_160[40] = {};
  uint8_t order[16] = {};  // +200, guest order
  uint32_t count = 0;  // +216
};
#pragma pack(pop)
static_assert(sizeof(SignedInUsersData) == 220);
inline constexpr memory::LayoutField kSignedInUsersFields[] = {{216, 4}};
inline constexpr memory::Layout kSignedInUsersLayout = {kSignedInUsersFields, 1};

struct SignedInUsers : View {
  using View::View;
  static constexpr uint32_t kSize = 220;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SignedInUsersData* data() const { return static_cast<SignedInUsersData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SignedInUsers New(uint8_t* base) {
    return SignedInUsers(base, memory::MakeHandle(new SignedInUsersData(), sizeof(SignedInUsersData), "SignedInUsers", &kSignedInUsersLayout));
  }
  template <uint32_t Stride = kSize>
  static SignedInUsers NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SignedInUsersData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SignedInUsersData*>(::operator new(sizeof(SignedInUsersData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SignedInUsersData();
    return SignedInUsers(base, memory::MakeHandle(p, sizeof(SignedInUsersData) * count, "SignedInUsers", &kSignedInUsersLayout, sizeof(SignedInUsersData)));
  }
  SignedInUsers at(uint32_t i) const { return SignedInUsers(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SignedInUsersData* direct() const { return static_cast<SignedInUsersData*>(memory::HandleOwn(address, &kSignedInUsersLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSignedInUsersLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSignedInUsersLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 4 users of 40 bytes; a user's +8 is nonzero when signed in
  uint32_t users_address() const { return address + 0; }
  uint8_t* users() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  // the users by index (4 words; Users::SignedIn's (index + 50) x 4)
  uint32_t order_address() const { return address + 200; }
  uint8_t* order() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 200 : guest::Pointer(base, address + 200); }
  // the signed-in count
  uint32_t count_address() const { return address + 216; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(base, address + 216))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(base, address + 216, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(base, address + 216))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(base, address + 216, v); }
};

// image; measure: SaveDevices::SetRootName.r4, SaveSlot::RequestSave.r6, SaveSlot::RequestSave.r7, SaveSlot::RequestLoad.r6, SaveSlot::RequestLoad.r7, SaveSlot::RequestDelete.r6
struct SaveCString : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  // a byte of the string (the copy loops step the pointer)
  uint8_t first() const { return guest::LoadU8(base, address + 0); }
  void set_first(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// The image words docs/functions/save-interface.md's code uses.
struct SaveInterfaceGlobals {
  // the save interface: the app + 16 (vtable 0x82000E38), set by SaveInterface::Init
  template <typename T = AppInterface16>
  static T interface(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BDC90u)); }
  static void set_interface(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BDC90u, v.address); }
  static void set_interface(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BDC90u, v); }
  static constexpr uint32_t interface_address() { return 0x826BDC90u; }
  template <typename T = SaveBuffers>
  static T save_buffers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828161F0u)); }
  static void set_save_buffers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828161F0u, v.address); }
  static void set_save_buffers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828161F0u, v); }
  static constexpr uint32_t save_buffers_address() { return 0x828161F0u; }
  // each player's controller (4 words)
  static constexpr uint32_t controllers_address() { return 0x826BABA4u; }
  // the play time's frame counter (PlayTime::Add)
  static uint32_t play_time_frames(uint8_t* base) { return guest::LoadU32(base, 0x826C5B90u); }
  static void set_play_time_frames(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B90u, v); }
  static constexpr uint32_t play_time_frames_address() { return 0x826C5B90u; }
  // "TXR360"
  static constexpr uint32_t root_name_address() { return 0x82001ED4u; }
};

}  // namespace schema
