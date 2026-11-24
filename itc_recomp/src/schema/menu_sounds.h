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

// allocated; measure: AudioManager.+1544, AudioManager.+1544.+0, AudioManager::RemoveHandle.r3@+1540.+4, AudioManager::RemoveHandle.r3@+1540.+4.+0
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

// embedded; measure: AudioManager::RemoveHandle.r3@+1540
#pragma pack(push, 1)
struct SoundHandleListData {
  uint32_t allocator = 0;  // +0
  uint32_t head = 0;  // +4
  uint32_t size = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(SoundHandleListData) == 12);
inline constexpr memory::LayoutField kSoundHandleListFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kSoundHandleListLayout = {kSoundHandleListFields, 3};

struct SoundHandleList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SoundHandleListData* data() const { return static_cast<SoundHandleListData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SoundHandleList New(uint8_t* base) {
    return SoundHandleList(base, memory::MakeHandle(new SoundHandleListData(), sizeof(SoundHandleListData), "SoundHandleList", &kSoundHandleListLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundHandleList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundHandleListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundHandleListData*>(::operator new(sizeof(SoundHandleListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundHandleListData();
    return SoundHandleList(base, memory::MakeHandle(p, sizeof(SoundHandleListData) * count, "SoundHandleList", &kSoundHandleListLayout, sizeof(SoundHandleListData)));
  }
  SoundHandleList at(uint32_t i) const { return SoundHandleList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundHandleListData* direct() const { return static_cast<SoundHandleListData*>(memory::HandleOwn(address, &kSoundHandleListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundHandleListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundHandleListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // probably the allocator word (std::list's first)
  uint32_t allocator_address() const { return address + 0; }
  uint32_t allocator_bits() const { if (auto* d = direct()) return d->allocator; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_allocator_bits(uint32_t v) const { if (auto* d = direct()) { d->allocator = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t allocator() const { if (auto* d = direct()) return d->allocator; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_allocator(uint32_t v) const { if (auto* d = direct()) { d->allocator = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the head node
  uint32_t head_address() const { return address + 4; }
  uint32_t head_bits() const { if (auto* d = direct()) return d->head; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_head_bits(uint32_t v) const { if (auto* d = direct()) { d->head = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = SoundHandleNode>
  T head() const { return T(base, head_bits()); }
  void set_head(const View& w) const { set_head_bits(w.address); }
  void set_head(uint32_t v) const { set_head_bits(v); }
  uint32_t size_address() const { return address + 8; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
};

// allocated; measure: AudioManager, [826C5C80], sub_82119188.r3, sub_82118930.r3, sub_82119018.r3
#pragma pack(push, 1)
struct AudioManagerData {
  uint32_t vtable = 0;  // +0
  uint32_t bank_owner_vtable = 0;  // +4
  uint32_t sound_owner_vtable = 0;  // +8
  uint32_t owner = 0;  // +12
  uint8_t engine[100] = {};  // +16, 1 x AudioEngine
  uint8_t pad_116[1392] = {};
  uint32_t fault_1508 = 0;  // +1508
  uint32_t bank_pairs = 0;  // +1512
  uint32_t bank_pairs_first = 0;  // +1516
  uint32_t bank_pairs_end = 0;  // +1520
  uint32_t bank_pairs_capacity_end = 0;  // +1524
  uint8_t pad_1528[4] = {};
  uint32_t sounds_head = 0;  // +1532
  uint8_t pad_1536[8] = {};
  uint32_t handle_sounds_head = 0;  // +1544
  uint8_t pad_1548[4] = {};
  uint32_t next_handle = 0;  // +1552
  uint8_t name[28] = {};  // +1556, guest order
  uint32_t handler_vtable = 0;  // +1584
  uint32_t handler_vtable2 = 0;  // +1588
  uint32_t handler_8 = 0;  // +1592
  uint32_t handler_12 = 0;  // +1596
  uint8_t handler_flag_16 = 0;  // +1600
  uint8_t handler_flag_17 = 0;  // +1601
  uint8_t handler_flag_18 = 0;  // +1602
  uint8_t pad_1603[1] = {};
  uint32_t handler_message = 0;  // +1604
  uint32_t handler_delegate_vtable = 0;  // +1608
  uint32_t handler_delegate_object = 0;  // +1612
  uint64_t handler_delegate_method = 0;  // +1616
};
#pragma pack(pop)
static_assert(sizeof(AudioManagerData) == 1624);
inline constexpr memory::LayoutField kAudioManagerFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {52, 4}, {56, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {1456, 4}, {1460, 4}, {1464, 4}, {1468, 4}, {1472, 4}, {1476, 4}, {1480, 4}, {1484, 4}, {1488, 4}, {1492, 4}, {1496, 4}, {1500, 4}, {1504, 4}, {1508, 4}, {1512, 4}, {1516, 4}, {1520, 4}, {1524, 4}, {1532, 4}, {1544, 4}, {1552, 4}, {1584, 4}, {1588, 4}, {1592, 4}, {1596, 4}, {1604, 4}, {1608, 4}, {1612, 4}, {1616, 8}};
inline constexpr memory::Layout kAudioManagerLayout = {kAudioManagerFields, 49};

struct AudioManager : View {
  using View::View;
  static constexpr uint32_t kSize = 1624;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82008388;
  AudioManagerData* data() const { return static_cast<AudioManagerData*>(memory::HandleObject(address)); }
  static AudioManager New(uint8_t* base) {
    return AudioManager(base, memory::MakeHandle(new AudioManagerData(), sizeof(AudioManagerData), "AudioManager", &kAudioManagerLayout));
  }
  template <uint32_t Stride = kSize>
  static AudioManager NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AudioManagerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AudioManagerData*>(::operator new(sizeof(AudioManagerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AudioManagerData();
    return AudioManager(base, memory::MakeHandle(p, sizeof(AudioManagerData) * count, "AudioManager", &kAudioManagerLayout, sizeof(AudioManagerData)));
  }
  AudioManager at(uint32_t i) const { return AudioManager(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AudioManagerData* direct() const { return static_cast<AudioManagerData*>(memory::HandleOwn(address, &kAudioManagerLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAudioManagerLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAudioManagerLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82008388 (0x820082B8, the base's, in the destructor)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  template <typename T = AudioManagerVtable>
  T vtable() const { return T(base, vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // 0x82008374 (0x82000D48 first): the interface the bank pairs' owner is (+36 of a BankPair); AudioManager::GetEngine's r3
  uint32_t bank_owner_vtable_address() const { return address + 4; }
  uint32_t bank_owner_vtable_bits() const { if (auto* d = direct()) return d->bank_owner_vtable; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_bank_owner_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_owner_vtable = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t bank_owner_vtable() const { if (auto* d = direct()) return d->bank_owner_vtable; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_bank_owner_vtable(uint32_t v) const { if (auto* d = direct()) { d->bank_owner_vtable = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // 0x82008364 (0x82000D48 first): the interface the Sounds' owner is (Sound +0); AudioManager::GetSoundBank's r3
  uint32_t sound_owner_vtable_address() const { return address + 8; }
  uint32_t sound_owner_vtable_bits() const { if (auto* d = direct()) return d->sound_owner_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_sound_owner_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_owner_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t sound_owner_vtable() const { if (auto* d = direct()) return d->sound_owner_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_sound_owner_vtable(uint32_t v) const { if (auto* d = direct()) { d->sound_owner_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the constructor's r4; its vtable's slot +8 gives the dispatcher the handler at +1584 is registered with
  uint32_t owner_address() const { return address + 12; }
  uint32_t owner_bits() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_owner_bits(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t owner() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_owner(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // the audio engine (sound-cues.toml): the game's XACT and X3DAudio state
  template <typename T = AudioEngine>
  T engine() const { return T(base, address + 16); }
  // the listener (sound-cues.toml, Listener::Listener)
  template <typename T = SoundListener>
  T listener() const { return T(base, address + 1456); }
  // probably a measure fault: the +8 interface's +1508 (the bank pairs' first, +1516)
  uint32_t fault_1508_address() const { return address + 1508; }
  uint32_t fault_1508_bits() const { if (auto* d = direct()) return d->fault_1508; return uint32_t((memory::IsHandle(address + 1508) ? uint32_t(memory::OwnerLoad(address + 1508, 4)) : guest::LoadU32(base, address + 1508))); }
  void set_fault_1508_bits(uint32_t v) const { if (auto* d = direct()) { d->fault_1508 = v; return; } if (memory::IsHandle(address + 1508)) memory::OwnerStore(address + 1508, 4, v); else guest::StoreU32(base, address + 1508, v); }
  uint32_t fault_1508() const { if (auto* d = direct()) return d->fault_1508; return uint32_t((memory::IsHandle(address + 1508) ? uint32_t(memory::OwnerLoad(address + 1508, 4)) : guest::LoadU32(base, address + 1508))); }
  void set_fault_1508(uint32_t v) const { if (auto* d = direct()) { d->fault_1508 = v; return; } if (memory::IsHandle(address + 1508)) memory::OwnerStore(address + 1508, 4, v); else guest::StoreU32(base, address + 1508, v); }
  // the bank pairs' vector (WordVector::Fill; one slot a bank index): +1512 its allocator word
  uint32_t bank_pairs_address() const { return address + 1512; }
  uint32_t bank_pairs_bits() const { if (auto* d = direct()) return d->bank_pairs; return uint32_t((memory::IsHandle(address + 1512) ? uint32_t(memory::OwnerLoad(address + 1512, 4)) : guest::LoadU32(base, address + 1512))); }
  void set_bank_pairs_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs = v; return; } if (memory::IsHandle(address + 1512)) memory::OwnerStore(address + 1512, 4, v); else guest::StoreU32(base, address + 1512, v); }
  uint32_t bank_pairs() const { if (auto* d = direct()) return d->bank_pairs; return uint32_t((memory::IsHandle(address + 1512) ? uint32_t(memory::OwnerLoad(address + 1512, 4)) : guest::LoadU32(base, address + 1512))); }
  void set_bank_pairs(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs = v; return; } if (memory::IsHandle(address + 1512)) memory::OwnerStore(address + 1512, 4, v); else guest::StoreU32(base, address + 1512, v); }
  // the vector's first slot (a BankPair pointer, 0 for an empty slot)
  uint32_t bank_pairs_first_address() const { return address + 1516; }
  uint32_t bank_pairs_first_bits() const { if (auto* d = direct()) return d->bank_pairs_first; return uint32_t((memory::IsHandle(address + 1516) ? uint32_t(memory::OwnerLoad(address + 1516, 4)) : guest::LoadU32(base, address + 1516))); }
  void set_bank_pairs_first_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs_first = v; return; } if (memory::IsHandle(address + 1516)) memory::OwnerStore(address + 1516, 4, v); else guest::StoreU32(base, address + 1516, v); }
  uint32_t bank_pairs_first() const { if (auto* d = direct()) return d->bank_pairs_first; return uint32_t((memory::IsHandle(address + 1516) ? uint32_t(memory::OwnerLoad(address + 1516, 4)) : guest::LoadU32(base, address + 1516))); }
  void set_bank_pairs_first(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs_first = v; return; } if (memory::IsHandle(address + 1516)) memory::OwnerStore(address + 1516, 4, v); else guest::StoreU32(base, address + 1516, v); }
  // the vector's end
  uint32_t bank_pairs_end_address() const { return address + 1520; }
  uint32_t bank_pairs_end_bits() const { if (auto* d = direct()) return d->bank_pairs_end; return uint32_t((memory::IsHandle(address + 1520) ? uint32_t(memory::OwnerLoad(address + 1520, 4)) : guest::LoadU32(base, address + 1520))); }
  void set_bank_pairs_end_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs_end = v; return; } if (memory::IsHandle(address + 1520)) memory::OwnerStore(address + 1520, 4, v); else guest::StoreU32(base, address + 1520, v); }
  uint32_t bank_pairs_end() const { if (auto* d = direct()) return d->bank_pairs_end; return uint32_t((memory::IsHandle(address + 1520) ? uint32_t(memory::OwnerLoad(address + 1520, 4)) : guest::LoadU32(base, address + 1520))); }
  void set_bank_pairs_end(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs_end = v; return; } if (memory::IsHandle(address + 1520)) memory::OwnerStore(address + 1520, 4, v); else guest::StoreU32(base, address + 1520, v); }
  // the vector's capacity end (probably)
  uint32_t bank_pairs_capacity_end_address() const { return address + 1524; }
  uint32_t bank_pairs_capacity_end_bits() const { if (auto* d = direct()) return d->bank_pairs_capacity_end; return uint32_t((memory::IsHandle(address + 1524) ? uint32_t(memory::OwnerLoad(address + 1524, 4)) : guest::LoadU32(base, address + 1524))); }
  void set_bank_pairs_capacity_end_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs_capacity_end = v; return; } if (memory::IsHandle(address + 1524)) memory::OwnerStore(address + 1524, 4, v); else guest::StoreU32(base, address + 1524, v); }
  uint32_t bank_pairs_capacity_end() const { if (auto* d = direct()) return d->bank_pairs_capacity_end; return uint32_t((memory::IsHandle(address + 1524) ? uint32_t(memory::OwnerLoad(address + 1524, 4)) : guest::LoadU32(base, address + 1524))); }
  void set_bank_pairs_capacity_end(uint32_t v) const { if (auto* d = direct()) { d->bank_pairs_capacity_end = v; return; } if (memory::IsHandle(address + 1524)) memory::OwnerStore(address + 1524, 4, v); else guest::StoreU32(base, address + 1524, v); }
  // the list of playing Sounds (+1528, a std::list): its head node (sound-cues.md)
  uint32_t sounds_head_address() const { return address + 1532; }
  uint32_t sounds_head_bits() const { if (auto* d = direct()) return d->sounds_head; return uint32_t((memory::IsHandle(address + 1532) ? uint32_t(memory::OwnerLoad(address + 1532, 4)) : guest::LoadU32(base, address + 1532))); }
  void set_sounds_head_bits(uint32_t v) const { if (auto* d = direct()) { d->sounds_head = v; return; } if (memory::IsHandle(address + 1532)) memory::OwnerStore(address + 1532, 4, v); else guest::StoreU32(base, address + 1532, v); }
  uint32_t sounds_head() const { if (auto* d = direct()) return d->sounds_head; return uint32_t((memory::IsHandle(address + 1532) ? uint32_t(memory::OwnerLoad(address + 1532, 4)) : guest::LoadU32(base, address + 1532))); }
  void set_sounds_head(uint32_t v) const { if (auto* d = direct()) { d->sounds_head = v; return; } if (memory::IsHandle(address + 1532)) memory::OwnerStore(address + 1532, 4, v); else guest::StoreU32(base, address + 1532, v); }
  // the list of Sounds kept by handle (SoundHandleList; AudioManager::PlayHandle and slots 16 to 24)
  uint32_t handle_sounds_address() const { return address + 1540; }
  uint8_t* handle_sounds() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1540 : guest::Pointer(base, address + 1540); }
  // its head node
  uint32_t handle_sounds_head_address() const { return address + 1544; }
  uint32_t handle_sounds_head_bits() const { if (auto* d = direct()) return d->handle_sounds_head; return uint32_t((memory::IsHandle(address + 1544) ? uint32_t(memory::OwnerLoad(address + 1544, 4)) : guest::LoadU32(base, address + 1544))); }
  void set_handle_sounds_head_bits(uint32_t v) const { if (auto* d = direct()) { d->handle_sounds_head = v; return; } if (memory::IsHandle(address + 1544)) memory::OwnerStore(address + 1544, 4, v); else guest::StoreU32(base, address + 1544, v); }
  template <typename T = SoundHandleNode>
  T handle_sounds_head() const { return T(base, handle_sounds_head_bits()); }
  void set_handle_sounds_head(const View& w) const { set_handle_sounds_head_bits(w.address); }
  void set_handle_sounds_head(uint32_t v) const { set_handle_sounds_head_bits(v); }
  // 1 at first: the handle AudioManager::PlayHandle gives next
  uint32_t next_handle_address() const { return address + 1552; }
  uint32_t next_handle_bits() const { if (auto* d = direct()) return d->next_handle; return uint32_t((memory::IsHandle(address + 1552) ? uint32_t(memory::OwnerLoad(address + 1552, 4)) : guest::LoadU32(base, address + 1552))); }
  void set_next_handle_bits(uint32_t v) const { if (auto* d = direct()) { d->next_handle = v; return; } if (memory::IsHandle(address + 1552)) memory::OwnerStore(address + 1552, 4, v); else guest::StoreU32(base, address + 1552, v); }
  uint32_t next_handle() const { if (auto* d = direct()) return d->next_handle; return uint32_t((memory::IsHandle(address + 1552) ? uint32_t(memory::OwnerLoad(address + 1552, 4)) : guest::LoadU32(base, address + 1552))); }
  void set_next_handle(uint32_t v) const { if (auto* d = direct()) { d->next_handle = v; return; } if (memory::IsHandle(address + 1552)) memory::OwnerStore(address + 1552, 4, v); else guest::StoreU32(base, address + 1552, v); }
  // a std::string (the constructor's r5)
  uint32_t name_address() const { return address + 1556; }
  uint8_t* name() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1556 : guest::Pointer(base, address + 1556); }
  // the handler object (40 bytes, registered with the owner's dispatcher; its +12 = 3 runs sub_82094B10 each update): 0x820066DC (0x82000EB8 first)
  uint32_t handler_vtable_address() const { return address + 1584; }
  uint32_t handler_vtable_bits() const { if (auto* d = direct()) return d->handler_vtable; return uint32_t((memory::IsHandle(address + 1584) ? uint32_t(memory::OwnerLoad(address + 1584, 4)) : guest::LoadU32(base, address + 1584))); }
  void set_handler_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_vtable = v; return; } if (memory::IsHandle(address + 1584)) memory::OwnerStore(address + 1584, 4, v); else guest::StoreU32(base, address + 1584, v); }
  uint32_t handler_vtable() const { if (auto* d = direct()) return d->handler_vtable; return uint32_t((memory::IsHandle(address + 1584) ? uint32_t(memory::OwnerLoad(address + 1584, 4)) : guest::LoadU32(base, address + 1584))); }
  void set_handler_vtable(uint32_t v) const { if (auto* d = direct()) { d->handler_vtable = v; return; } if (memory::IsHandle(address + 1584)) memory::OwnerStore(address + 1584, 4, v); else guest::StoreU32(base, address + 1584, v); }
  // the handler's second vtable word: 0x82002908 (0x82000AB4, then 0x82000BA8, first)
  uint32_t handler_vtable2_address() const { return address + 1588; }
  uint32_t handler_vtable2_bits() const { if (auto* d = direct()) return d->handler_vtable2; return uint32_t((memory::IsHandle(address + 1588) ? uint32_t(memory::OwnerLoad(address + 1588, 4)) : guest::LoadU32(base, address + 1588))); }
  void set_handler_vtable2_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_vtable2 = v; return; } if (memory::IsHandle(address + 1588)) memory::OwnerStore(address + 1588, 4, v); else guest::StoreU32(base, address + 1588, v); }
  uint32_t handler_vtable2() const { if (auto* d = direct()) return d->handler_vtable2; return uint32_t((memory::IsHandle(address + 1588) ? uint32_t(memory::OwnerLoad(address + 1588, 4)) : guest::LoadU32(base, address + 1588))); }
  void set_handler_vtable2(uint32_t v) const { if (auto* d = direct()) { d->handler_vtable2 = v; return; } if (memory::IsHandle(address + 1588)) memory::OwnerStore(address + 1588, 4, v); else guest::StoreU32(base, address + 1588, v); }
  // the handler's +8, 0
  uint32_t handler_8_address() const { return address + 1592; }
  uint32_t handler_8_bits() const { if (auto* d = direct()) return d->handler_8; return uint32_t((memory::IsHandle(address + 1592) ? uint32_t(memory::OwnerLoad(address + 1592, 4)) : guest::LoadU32(base, address + 1592))); }
  void set_handler_8_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_8 = v; return; } if (memory::IsHandle(address + 1592)) memory::OwnerStore(address + 1592, 4, v); else guest::StoreU32(base, address + 1592, v); }
  uint32_t handler_8() const { if (auto* d = direct()) return d->handler_8; return uint32_t((memory::IsHandle(address + 1592) ? uint32_t(memory::OwnerLoad(address + 1592, 4)) : guest::LoadU32(base, address + 1592))); }
  void set_handler_8(uint32_t v) const { if (auto* d = direct()) { d->handler_8 = v; return; } if (memory::IsHandle(address + 1592)) memory::OwnerStore(address + 1592, 4, v); else guest::StoreU32(base, address + 1592, v); }
  // the handler's +12, 0 (3 runs sub_82094B10 each update, known-data.md)
  uint32_t handler_12_address() const { return address + 1596; }
  uint32_t handler_12_bits() const { if (auto* d = direct()) return d->handler_12; return uint32_t((memory::IsHandle(address + 1596) ? uint32_t(memory::OwnerLoad(address + 1596, 4)) : guest::LoadU32(base, address + 1596))); }
  void set_handler_12_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_12 = v; return; } if (memory::IsHandle(address + 1596)) memory::OwnerStore(address + 1596, 4, v); else guest::StoreU32(base, address + 1596, v); }
  uint32_t handler_12() const { if (auto* d = direct()) return d->handler_12; return uint32_t((memory::IsHandle(address + 1596) ? uint32_t(memory::OwnerLoad(address + 1596, 4)) : guest::LoadU32(base, address + 1596))); }
  void set_handler_12(uint32_t v) const { if (auto* d = direct()) { d->handler_12 = v; return; } if (memory::IsHandle(address + 1596)) memory::OwnerStore(address + 1596, 4, v); else guest::StoreU32(base, address + 1596, v); }
  // the handler's +16, 0
  uint32_t handler_flag_16_address() const { return address + 1600; }
  uint8_t handler_flag_16_bits() const { if (auto* d = direct()) return d->handler_flag_16; return uint8_t((memory::IsHandle(address + 1600) ? uint8_t(memory::OwnerLoad(address + 1600, 1)) : guest::LoadU8(base, address + 1600))); }
  void set_handler_flag_16_bits(uint8_t v) const { if (auto* d = direct()) { d->handler_flag_16 = v; return; } if (memory::IsHandle(address + 1600)) memory::OwnerStore(address + 1600, 1, v); else guest::StoreU8(base, address + 1600, v); }
  uint8_t handler_flag_16() const { if (auto* d = direct()) return d->handler_flag_16; return uint8_t((memory::IsHandle(address + 1600) ? uint8_t(memory::OwnerLoad(address + 1600, 1)) : guest::LoadU8(base, address + 1600))); }
  void set_handler_flag_16(uint8_t v) const { if (auto* d = direct()) { d->handler_flag_16 = v; return; } if (memory::IsHandle(address + 1600)) memory::OwnerStore(address + 1600, 1, v); else guest::StoreU8(base, address + 1600, v); }
  // the handler's +17, 1
  uint32_t handler_flag_17_address() const { return address + 1601; }
  uint8_t handler_flag_17_bits() const { if (auto* d = direct()) return d->handler_flag_17; return uint8_t((memory::IsHandle(address + 1601) ? uint8_t(memory::OwnerLoad(address + 1601, 1)) : guest::LoadU8(base, address + 1601))); }
  void set_handler_flag_17_bits(uint8_t v) const { if (auto* d = direct()) { d->handler_flag_17 = v; return; } if (memory::IsHandle(address + 1601)) memory::OwnerStore(address + 1601, 1, v); else guest::StoreU8(base, address + 1601, v); }
  uint8_t handler_flag_17() const { if (auto* d = direct()) return d->handler_flag_17; return uint8_t((memory::IsHandle(address + 1601) ? uint8_t(memory::OwnerLoad(address + 1601, 1)) : guest::LoadU8(base, address + 1601))); }
  void set_handler_flag_17(uint8_t v) const { if (auto* d = direct()) { d->handler_flag_17 = v; return; } if (memory::IsHandle(address + 1601)) memory::OwnerStore(address + 1601, 1, v); else guest::StoreU8(base, address + 1601, v); }
  // the handler's +18, 0
  uint32_t handler_flag_18_address() const { return address + 1602; }
  uint8_t handler_flag_18_bits() const { if (auto* d = direct()) return d->handler_flag_18; return uint8_t((memory::IsHandle(address + 1602) ? uint8_t(memory::OwnerLoad(address + 1602, 1)) : guest::LoadU8(base, address + 1602))); }
  void set_handler_flag_18_bits(uint8_t v) const { if (auto* d = direct()) { d->handler_flag_18 = v; return; } if (memory::IsHandle(address + 1602)) memory::OwnerStore(address + 1602, 1, v); else guest::StoreU8(base, address + 1602, v); }
  uint8_t handler_flag_18() const { if (auto* d = direct()) return d->handler_flag_18; return uint8_t((memory::IsHandle(address + 1602) ? uint8_t(memory::OwnerLoad(address + 1602, 1)) : guest::LoadU8(base, address + 1602))); }
  void set_handler_flag_18(uint8_t v) const { if (auto* d = direct()) { d->handler_flag_18 = v; return; } if (memory::IsHandle(address + 1602)) memory::OwnerStore(address + 1602, 1, v); else guest::StoreU8(base, address + 1602, v); }
  // the handler's +20: the constructor's caller's frame word (probably a message id)
  uint32_t handler_message_address() const { return address + 1604; }
  uint32_t handler_message_bits() const { if (auto* d = direct()) return d->handler_message; return uint32_t((memory::IsHandle(address + 1604) ? uint32_t(memory::OwnerLoad(address + 1604, 4)) : guest::LoadU32(base, address + 1604))); }
  void set_handler_message_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_message = v; return; } if (memory::IsHandle(address + 1604)) memory::OwnerStore(address + 1604, 4, v); else guest::StoreU32(base, address + 1604, v); }
  uint32_t handler_message() const { if (auto* d = direct()) return d->handler_message; return uint32_t((memory::IsHandle(address + 1604) ? uint32_t(memory::OwnerLoad(address + 1604, 4)) : guest::LoadU32(base, address + 1604))); }
  void set_handler_message(uint32_t v) const { if (auto* d = direct()) { d->handler_message = v; return; } if (memory::IsHandle(address + 1604)) memory::OwnerStore(address + 1604, 4, v); else guest::StoreU32(base, address + 1604, v); }
  // the handler's delegate (+24, 16 bytes): 0x8200833C (0x82000ABC first)
  uint32_t handler_delegate_vtable_address() const { return address + 1608; }
  uint32_t handler_delegate_vtable_bits() const { if (auto* d = direct()) return d->handler_delegate_vtable; return uint32_t((memory::IsHandle(address + 1608) ? uint32_t(memory::OwnerLoad(address + 1608, 4)) : guest::LoadU32(base, address + 1608))); }
  void set_handler_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_delegate_vtable = v; return; } if (memory::IsHandle(address + 1608)) memory::OwnerStore(address + 1608, 4, v); else guest::StoreU32(base, address + 1608, v); }
  uint32_t handler_delegate_vtable() const { if (auto* d = direct()) return d->handler_delegate_vtable; return uint32_t((memory::IsHandle(address + 1608) ? uint32_t(memory::OwnerLoad(address + 1608, 4)) : guest::LoadU32(base, address + 1608))); }
  void set_handler_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->handler_delegate_vtable = v; return; } if (memory::IsHandle(address + 1608)) memory::OwnerStore(address + 1608, 4, v); else guest::StoreU32(base, address + 1608, v); }
  // the delegate's object: the manager
  uint32_t handler_delegate_object_address() const { return address + 1612; }
  uint32_t handler_delegate_object_bits() const { if (auto* d = direct()) return d->handler_delegate_object; return uint32_t((memory::IsHandle(address + 1612) ? uint32_t(memory::OwnerLoad(address + 1612, 4)) : guest::LoadU32(base, address + 1612))); }
  void set_handler_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_delegate_object = v; return; } if (memory::IsHandle(address + 1612)) memory::OwnerStore(address + 1612, 4, v); else guest::StoreU32(base, address + 1612, v); }
  template <typename T = AudioManager>
  T handler_delegate_object() const { return T(base, handler_delegate_object_bits()); }
  void set_handler_delegate_object(const View& w) const { set_handler_delegate_object_bits(w.address); }
  void set_handler_delegate_object(uint32_t v) const { set_handler_delegate_object_bits(v); }
  // the delegate's member pointer (two words)
  uint32_t handler_delegate_method_address() const { return address + 1616; }
  uint64_t handler_delegate_method_bits() const { if (auto* d = direct()) return d->handler_delegate_method; return uint64_t((memory::IsHandle(address + 1616) ? uint64_t(memory::OwnerLoad(address + 1616, 8)) : guest::LoadU64(base, address + 1616))); }
  void set_handler_delegate_method_bits(uint64_t v) const { if (auto* d = direct()) { d->handler_delegate_method = v; return; } if (memory::IsHandle(address + 1616)) memory::OwnerStore(address + 1616, 8, v); else guest::StoreU64(base, address + 1616, v); }
  uint64_t handler_delegate_method() const { if (auto* d = direct()) return d->handler_delegate_method; return uint64_t((memory::IsHandle(address + 1616) ? uint64_t(memory::OwnerLoad(address + 1616, 8)) : guest::LoadU64(base, address + 1616))); }
  void set_handler_delegate_method(uint64_t v) const { if (auto* d = direct()) { d->handler_delegate_method = v; return; } if (memory::IsHandle(address + 1616)) memory::OwnerStore(address + 1616, 8, v); else guest::StoreU64(base, address + 1616, v); }
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

// embedded; measure: AudioManager@+1552
struct AudioManagerAt1552 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
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

#include "schema/sound_cues.h"
