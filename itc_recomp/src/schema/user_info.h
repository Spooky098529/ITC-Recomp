// The views of docs/functions/user-info.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/user-info.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: UserInfo
struct UserInfoMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the transitions' r3 is the page object's +24 (PageObject.step_started), not the machine's
  uint32_t fault_step_started() const { return guest::LoadU32(address + 0); }
  void set_fault_step_started(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t fault_step_started_address() const { return address + 0; }
  // measure fault: MenuPage's r3 is the dispatcher (UserInfoMode.message_count), not the machine
  uint32_t fault_message_count() const { return guest::LoadU32(address + 804); }
  void set_fault_message_count(uint32_t v) const { guest::StoreU32(address + 804, v); }
  uint32_t fault_message_count_address() const { return address + 804; }
  // measure fault: the dispatcher's message_start, as above
  uint32_t fault_message_start() const { return guest::LoadU32(address + 808); }
  void set_fault_message_start(uint32_t v) const { guest::StoreU32(address + 808, v); }
  uint32_t fault_message_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 at set-up, 1 open the movie, 2 its load, 9 waiting for message 27, 4 and 5 shown, 7 and 6 closed
  uint32_t state() const { return guest::LoadU32(address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 10 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // set by the mode's Init: nonzero, Create goes to state 1, else 9
  uint32_t reenter() const { return guest::LoadU32(address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
};

// embedded; measure: UserInfo::Close.r4
#pragma pack(push, 1)
struct UserInfoModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(UserInfoModeData) == 812);
inline constexpr memory::LayoutField kUserInfoModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kUserInfoModeLayout = {kUserInfoModeFields, 2};

struct UserInfoMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  UserInfoModeData* data() const { return static_cast<UserInfoModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static UserInfoMode New() {
    return UserInfoMode(memory::MakeHandle(new UserInfoModeData(), sizeof(UserInfoModeData), "UserInfoMode", &kUserInfoModeLayout));
  }
  template <uint32_t Stride = kSize>
  static UserInfoMode NewArray(uint32_t count) {
    static_assert(sizeof(UserInfoModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<UserInfoModeData*>(::operator new(sizeof(UserInfoModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) UserInfoModeData();
    return UserInfoMode(memory::MakeHandle(p, sizeof(UserInfoModeData) * count, "UserInfoMode", &kUserInfoModeLayout, sizeof(UserInfoModeData)));
  }
  UserInfoMode at(uint32_t i) const { return UserInfoMode(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  UserInfoModeData* direct() const { return static_cast<UserInfoModeData*>(memory::HandleOwn(address, &kUserInfoModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kUserInfoModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUserInfoModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count_bits() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(address + 804))); }
  void set_message_count_bits(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(address + 804, v); }
  uint32_t message_count() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(address + 804))); }
  void set_message_count(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start_bits() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(address + 808))); }
  void set_message_start_bits(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(address + 808, v); }
  uint32_t message_start() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(address + 808))); }
  void set_message_start(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(address + 808, v); }
};

// allocated; measure: new<60>@UserInfo::Create
#pragma pack(push, 1)
struct UserInfoDataData {
  uint32_t loaded = 0;  // +0
  uint32_t queued_state = 0;  // +4
  uint32_t count_12 = 0;  // +8
  uint32_t count_14 = 0;  // +12
  uint32_t count_16 = 0;  // +16
  float ratio = 0;  // +20
  uint32_t stats_5968 = 0;  // +24
  float distance_5976 = 0;  // +28
  float distance_5904 = 0;  // +32
  int32_t distance_5992 = 0;  // +36
  uint32_t stats_6024 = 0;  // +40
  uint32_t play_hours = 0;  // +44
  uint8_t play_minutes = 0;  // +48
  uint8_t play_seconds = 0;  // +49
  uint8_t pad_50[2] = {};
  uint32_t garage_count = 0;  // +52
  uint32_t save_1991C = 0;  // +56
};
#pragma pack(pop)
static_assert(sizeof(UserInfoDataData) == 60);
inline constexpr memory::LayoutField kUserInfoDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {52, 4}, {56, 4}};
inline constexpr memory::Layout kUserInfoDataLayout = {kUserInfoDataFields, 14};

struct UserInfoData : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  UserInfoDataData* data() const { return static_cast<UserInfoDataData*>(memory::HandleObject(address)); }
  static UserInfoData New() {
    return UserInfoData(memory::MakeHandle(new UserInfoDataData(), sizeof(UserInfoDataData), "UserInfoData", &kUserInfoDataLayout));
  }
  template <uint32_t Stride = kSize>
  static UserInfoData NewArray(uint32_t count) {
    static_assert(sizeof(UserInfoDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<UserInfoDataData*>(::operator new(sizeof(UserInfoDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) UserInfoDataData();
    return UserInfoData(memory::MakeHandle(p, sizeof(UserInfoDataData) * count, "UserInfoData", &kUserInfoDataLayout, sizeof(UserInfoDataData)));
  }
  UserInfoData at(uint32_t i) const { return UserInfoData(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  UserInfoDataData* direct() const { return static_cast<UserInfoDataData*>(memory::HandleOwn(address, &kUserInfoDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kUserInfoDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kUserInfoDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 1 once loaded (WaitForLoad)
  uint32_t loaded_address() const { return address + 0; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the state Update moves the machine to: 21 none, 9 from Closing
  uint32_t queued_state_address() const { return address + 4; }
  uint32_t queued_state_bits() const { if (auto* d = direct()) return d->queued_state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_queued_state_bits(uint32_t v) const { if (auto* d = direct()) { d->queued_state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t queued_state() const { if (auto* d = direct()) return d->queued_state; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_queued_state(uint32_t v) const { if (auto* d = direct()) { d->queued_state = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the stats' halfword +12 (node 8)
  uint32_t count_12_address() const { return address + 8; }
  uint32_t count_12_bits() const { if (auto* d = direct()) return d->count_12; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_count_12_bits(uint32_t v) const { if (auto* d = direct()) { d->count_12 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t count_12() const { if (auto* d = direct()) return d->count_12; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_count_12(uint32_t v) const { if (auto* d = direct()) { d->count_12 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // the stats' halfword +14 (node 9)
  uint32_t count_14_address() const { return address + 12; }
  uint32_t count_14_bits() const { if (auto* d = direct()) return d->count_14; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_count_14_bits(uint32_t v) const { if (auto* d = direct()) { d->count_14 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t count_14() const { if (auto* d = direct()) return d->count_14; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_count_14(uint32_t v) const { if (auto* d = direct()) { d->count_14 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the stats' halfword +16 (node 10)
  uint32_t count_16_address() const { return address + 16; }
  uint32_t count_16_bits() const { if (auto* d = direct()) return d->count_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_count_16_bits(uint32_t v) const { if (auto* d = direct()) { d->count_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t count_16() const { if (auto* d = direct()) return d->count_16; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_count_16(uint32_t v) const { if (auto* d = direct()) { d->count_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // +12 / +22 times 100 (node 11, string 453; probably the win rate)
  uint32_t ratio_address() const { return address + 20; }
  uint32_t ratio_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->ratio); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_ratio_bits(uint32_t v) const { if (auto* d = direct()) { d->ratio = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float ratio() const { return std::bit_cast<float>(ratio_bits()); }
  void set_ratio(float v) const { set_ratio_bits(std::bit_cast<uint32_t>(v)); }
  // the stats' +5968 (node 12)
  uint32_t stats_5968_address() const { return address + 24; }
  uint32_t stats_5968_bits() const { if (auto* d = direct()) return d->stats_5968; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_stats_5968_bits(uint32_t v) const { if (auto* d = direct()) { d->stats_5968 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t stats_5968() const { if (auto* d = direct()) return d->stats_5968; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_stats_5968(uint32_t v) const { if (auto* d = direct()) { d->stats_5968 = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // the stats' +5976, shown times 0.62138 (node 19, kilometers in miles)
  uint32_t distance_5976_address() const { return address + 28; }
  uint32_t distance_5976_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->distance_5976); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_distance_5976_bits(uint32_t v) const { if (auto* d = direct()) { d->distance_5976 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float distance_5976() const { return std::bit_cast<float>(distance_5976_bits()); }
  void set_distance_5976(float v) const { set_distance_5976_bits(std::bit_cast<uint32_t>(v)); }
  // the stats' +5904, shown times 0.62138 (node 20)
  uint32_t distance_5904_address() const { return address + 32; }
  uint32_t distance_5904_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->distance_5904); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_distance_5904_bits(uint32_t v) const { if (auto* d = direct()) { d->distance_5904 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float distance_5904() const { return std::bit_cast<float>(distance_5904_bits()); }
  void set_distance_5904(float v) const { set_distance_5904_bits(std::bit_cast<uint32_t>(v)); }
  // the stats' +5992 truncated, shown times 0.62138 (node 21)
  uint32_t distance_5992_address() const { return address + 36; }
  uint32_t distance_5992_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->distance_5992); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_distance_5992_bits(uint32_t v) const { if (auto* d = direct()) { d->distance_5992 = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  int32_t distance_5992() const { return int32_t(distance_5992_bits()); }
  void set_distance_5992(int32_t v) const { set_distance_5992_bits(uint32_t(v)); }
  // the stats' +6024 (node 22, SetNumberText)
  uint32_t stats_6024_address() const { return address + 40; }
  uint32_t stats_6024_bits() const { if (auto* d = direct()) return d->stats_6024; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_stats_6024_bits(uint32_t v) const { if (auto* d = direct()) { d->stats_6024 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t stats_6024() const { if (auto* d = direct()) return d->stats_6024; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_stats_6024(uint32_t v) const { if (auto* d = direct()) { d->stats_6024 = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // the play time's hours (PlayTime::Add; shown at most 9999)
  uint32_t play_hours_address() const { return address + 44; }
  uint32_t play_hours_bits() const { if (auto* d = direct()) return d->play_hours; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_play_hours_bits(uint32_t v) const { if (auto* d = direct()) { d->play_hours = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t play_hours() const { if (auto* d = direct()) return d->play_hours; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_play_hours(uint32_t v) const { if (auto* d = direct()) { d->play_hours = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // PlayTime::Add's word +4, stored as a word (bytes in guest order in the host object since E5 step 4): the minutes and seconds bytes
  uint32_t play_time_address() const { return address + 48; }
  uint8_t* play_time() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 48 : guest::Pointer(address + 48); }
  uint32_t play_minutes_address() const { return address + 48; }
  uint8_t play_minutes_bits() const { if (auto* d = direct()) return d->play_minutes; return uint8_t((memory::IsHandle(address + 48) ? uint8_t(memory::OwnerLoad(address + 48, 1)) : guest::LoadU8(address + 48))); }
  void set_play_minutes_bits(uint8_t v) const { if (auto* d = direct()) { d->play_minutes = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 1, v); else guest::StoreU8(address + 48, v); }
  uint8_t play_minutes() const { if (auto* d = direct()) return d->play_minutes; return uint8_t((memory::IsHandle(address + 48) ? uint8_t(memory::OwnerLoad(address + 48, 1)) : guest::LoadU8(address + 48))); }
  void set_play_minutes(uint8_t v) const { if (auto* d = direct()) { d->play_minutes = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 1, v); else guest::StoreU8(address + 48, v); }
  uint32_t play_seconds_address() const { return address + 49; }
  uint8_t play_seconds_bits() const { if (auto* d = direct()) return d->play_seconds; return uint8_t((memory::IsHandle(address + 49) ? uint8_t(memory::OwnerLoad(address + 49, 1)) : guest::LoadU8(address + 49))); }
  void set_play_seconds_bits(uint8_t v) const { if (auto* d = direct()) { d->play_seconds = v; return; } if (memory::IsHandle(address + 49)) memory::OwnerStore(address + 49, 1, v); else guest::StoreU8(address + 49, v); }
  uint8_t play_seconds() const { if (auto* d = direct()) return d->play_seconds; return uint8_t((memory::IsHandle(address + 49) ? uint8_t(memory::OwnerLoad(address + 49, 1)) : guest::LoadU8(address + 49))); }
  void set_play_seconds(uint8_t v) const { if (auto* d = direct()) { d->play_seconds = v; return; } if (memory::IsHandle(address + 49)) memory::OwnerStore(address + 49, 1, v); else guest::StoreU8(address + 49, v); }
  // the garage's count (node 24, string 444)
  uint32_t garage_count_address() const { return address + 52; }
  uint32_t garage_count_bits() const { if (auto* d = direct()) return d->garage_count; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_garage_count_bits(uint32_t v) const { if (auto* d = direct()) { d->garage_count = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t garage_count() const { if (auto* d = direct()) return d->garage_count; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_garage_count(uint32_t v) const { if (auto* d = direct()) { d->garage_count = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // the save's +0x1991C (node 24)
  uint32_t save_1991C_address() const { return address + 56; }
  uint32_t save_1991C_bits() const { if (auto* d = direct()) return d->save_1991C; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_save_1991C_bits(uint32_t v) const { if (auto* d = direct()) { d->save_1991C = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t save_1991C() const { if (auto* d = direct()) return d->save_1991C; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_save_1991C(uint32_t v) const { if (auto* d = direct()) { d->save_1991C = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
};

// embedded; measure: PlayTime::Add.ret
struct PlayTimeAnswer : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the stats' +6032
  uint32_t hours() const { return guest::LoadU32(address + 0); }
  void set_hours(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t hours_address() const { return address + 0; }
  // the stats' word +6036: the minutes, seconds and frames bytes
  uint32_t time_word() const { return guest::LoadU32(address + 4); }
  void set_time_word(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t time_word_address() const { return address + 4; }
};

// The image words docs/functions/user-info.md's code uses.
struct UserInfoGlobals {
  // pages 0 the menu page, 1 none (closing); steps 0 the opening, 1 the closing
  template <typename T = PageObject>
  static T pages() { return T(guest::LoadU32(0x826D84C4u)); }
  static void set_pages(const View& v) { guest::StoreU32(0x826D84C4u, v.address); }
  static void set_pages(uint32_t v) { guest::StoreU32(0x826D84C4u, v); }
  static constexpr uint32_t pages_address() { return 0x826D84C4u; }
  // a front-end menu, items 0 to 7, no wrap; stepped, its cursor not read
  template <typename T = FrontEndMenu>
  static T menu() { return T(guest::LoadU32(0x826D84C8u)); }
  static void set_menu(const View& v) { guest::StoreU32(0x826D84C8u, v.address); }
  static void set_menu(uint32_t v) { guest::StoreU32(0x826D84C8u, v); }
  static constexpr uint32_t menu_address() { return 0x826D84C8u; }
  template <typename T = UserInfoData>
  static T data() { return T(guest::LoadU32(0x826D84CCu)); }
  static void set_data(const View& v) { guest::StoreU32(0x826D84CCu, v.address); }
  static void set_data(uint32_t v) { guest::StoreU32(0x826D84CCu, v); }
  static constexpr uint32_t data_address() { return 0x826D84CCu; }
  // 0.62138 (kilometers to miles)
  static double miles_per_km() { return std::bit_cast<double>(guest::LoadU64(0x8200A510u)); }
  static void set_miles_per_km(double v) { guest::StoreU64(0x8200A510u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t miles_per_km_address() { return 0x8200A510u; }
};

}  // namespace schema
