// The views of docs/functions/save-screen.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/save-screen.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: SaveScreen
#pragma pack(push, 1)
struct SaveScreenHostData {
  uint8_t transitions[9600] = {};  // +0, guest order
  uint32_t transition_count = 0;  // +9600
  uint32_t state = 0;  // +9604
  uint32_t loaded = 0;  // +9608
  uint32_t load_at_once = 0;  // +9612
};
#pragma pack(pop)
static_assert(sizeof(SaveScreenHostData) == 9616);
inline constexpr memory::LayoutField kSaveScreenFields[] = {{9600, 4}, {9604, 4}, {9608, 4}, {9612, 4}};
inline constexpr memory::Layout kSaveScreenLayout = {kSaveScreenFields, 4};

struct SaveScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SaveScreenHostData* data() const { return static_cast<SaveScreenHostData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SaveScreen New(uint8_t* base) {
    return SaveScreen(base, memory::MakeHandle(new SaveScreenHostData(), sizeof(SaveScreenHostData), "SaveScreen", &kSaveScreenLayout));
  }
  template <uint32_t Stride = kSize>
  static SaveScreen NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SaveScreenHostData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SaveScreenHostData*>(::operator new(sizeof(SaveScreenHostData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SaveScreenHostData();
    return SaveScreen(base, memory::MakeHandle(p, sizeof(SaveScreenHostData) * count, "SaveScreen", &kSaveScreenLayout, sizeof(SaveScreenHostData)));
  }
  SaveScreen at(uint32_t i) const { return SaveScreen(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SaveScreenHostData* direct() const { return static_cast<SaveScreenHostData*>(memory::HandleOwn(address, &kSaveScreenLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSaveScreenLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSaveScreenLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint8_t* transitions() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t transition_count_bits() const { if (auto* d = direct()) return d->transition_count; return uint32_t((memory::IsHandle(address + 9600) ? uint32_t(memory::OwnerLoad(address + 9600, 4)) : guest::LoadU32(base, address + 9600))); }
  void set_transition_count_bits(uint32_t v) const { if (auto* d = direct()) { d->transition_count = v; return; } if (memory::IsHandle(address + 9600)) memory::OwnerStore(address + 9600, 4, v); else guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count() const { if (auto* d = direct()) return d->transition_count; return uint32_t((memory::IsHandle(address + 9600) ? uint32_t(memory::OwnerLoad(address + 9600, 4)) : guest::LoadU32(base, address + 9600))); }
  void set_transition_count(uint32_t v) const { if (auto* d = direct()) { d->transition_count = v; return; } if (memory::IsHandle(address + 9600)) memory::OwnerStore(address + 9600, 4, v); else guest::StoreU32(base, address + 9600, v); }
  // 1 load the movie, 2 wait for it, 9 idle, 4 Show, 5 shown, 7 leaving, 6 left
  uint32_t state_address() const { return address + 9604; }
  uint32_t state_bits() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_state_bits(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  uint32_t state() const { if (auto* d = direct()) return d->state; return uint32_t((memory::IsHandle(address + 9604) ? uint32_t(memory::OwnerLoad(address + 9604, 4)) : guest::LoadU32(base, address + 9604))); }
  void set_state(uint32_t v) const { if (auto* d = direct()) { d->state = v; return; } if (memory::IsHandle(address + 9604)) memory::OwnerStore(address + 9604, 4, v); else guest::StoreU32(base, address + 9604, v); }
  // 1 once movie 44 is loaded (WaitMovie)
  uint32_t loaded_address() const { return address + 9608; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 9608) ? uint32_t(memory::OwnerLoad(address + 9608, 4)) : guest::LoadU32(base, address + 9608))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 9608)) memory::OwnerStore(address + 9608, 4, v); else guest::StoreU32(base, address + 9608, v); }
  // nonzero: OnEnter goes to 1 (load the movie now), else 9 (loaded when opened)
  uint32_t load_at_once_address() const { return address + 9612; }
  uint32_t load_at_once_bits() const { if (auto* d = direct()) return d->load_at_once; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_load_at_once_bits(uint32_t v) const { if (auto* d = direct()) { d->load_at_once = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
  uint32_t load_at_once() const { if (auto* d = direct()) return d->load_at_once; return uint32_t((memory::IsHandle(address + 9612) ? uint32_t(memory::OwnerLoad(address + 9612, 4)) : guest::LoadU32(base, address + 9612))); }
  void set_load_at_once(uint32_t v) const { if (auto* d = direct()) { d->load_at_once = v; return; } if (memory::IsHandle(address + 9612)) memory::OwnerStore(address + 9612, 4, v); else guest::StoreU32(base, address + 9612, v); }
};

// embedded; measure: SaveScreen::Leave.r4
#pragma pack(push, 1)
struct SaveScreenModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(SaveScreenModeData) == 812);
inline constexpr memory::LayoutField kSaveScreenModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kSaveScreenModeLayout = {kSaveScreenModeFields, 2};

struct SaveScreenMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SaveScreenModeData* data() const { return static_cast<SaveScreenModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SaveScreenMode New(uint8_t* base) {
    return SaveScreenMode(base, memory::MakeHandle(new SaveScreenModeData(), sizeof(SaveScreenModeData), "SaveScreenMode", &kSaveScreenModeLayout));
  }
  template <uint32_t Stride = kSize>
  static SaveScreenMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SaveScreenModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SaveScreenModeData*>(::operator new(sizeof(SaveScreenModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SaveScreenModeData();
    return SaveScreenMode(base, memory::MakeHandle(p, sizeof(SaveScreenModeData) * count, "SaveScreenMode", &kSaveScreenModeLayout, sizeof(SaveScreenModeData)));
  }
  SaveScreenMode at(uint32_t i) const { return SaveScreenMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SaveScreenModeData* direct() const { return static_cast<SaveScreenModeData*>(memory::HandleOwn(address, &kSaveScreenModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSaveScreenModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSaveScreenModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(base, address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count_bits() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count_bits(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t message_count() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start_bits() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start_bits(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t message_start() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
};

// allocated; measure: new<68>@SaveScreen::OnEnter, new<28>@SaveScreen::OnEnter.+8.+-31660
#pragma pack(push, 1)
struct SaveScreenDataData {
  uint32_t set_up = 0;  // +0
  uint32_t answer = 0;  // +4
  uint32_t kind2_flag = 0;  // +8
  uint32_t message = 0;  // +12
  uint32_t dialog_state = 0;  // +16
  uint32_t state_after_close = 0;  // +20
  uint32_t wait = 0;  // +24
  uint32_t cancel = 0;  // +28
  uint32_t next_state = 0;  // +32
  uint32_t kind = 0;  // +36
  uint32_t replay_index = 0;  // +40
  uint32_t player = 0;  // +44
  uint32_t default_answer = 0;  // +48
  uint32_t controller_pick = 0;  // +52
  uint32_t prompts = 0;  // +56
  uint32_t settings_stored = 0;  // +60
  uint32_t unknown_64 = 0;  // +64
};
#pragma pack(pop)
static_assert(sizeof(SaveScreenDataData) == 68);
inline constexpr memory::LayoutField kSaveScreenDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}};
inline constexpr memory::Layout kSaveScreenDataLayout = {kSaveScreenDataFields, 17};

struct SaveScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  SaveScreenDataData* data() const { return static_cast<SaveScreenDataData*>(memory::HandleObject(address)); }
  static SaveScreenData New(uint8_t* base) {
    return SaveScreenData(base, memory::MakeHandle(new SaveScreenDataData(), sizeof(SaveScreenDataData), "SaveScreenData", &kSaveScreenDataLayout));
  }
  template <uint32_t Stride = kSize>
  static SaveScreenData NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SaveScreenDataData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SaveScreenDataData*>(::operator new(sizeof(SaveScreenDataData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SaveScreenDataData();
    return SaveScreenData(base, memory::MakeHandle(p, sizeof(SaveScreenDataData) * count, "SaveScreenData", &kSaveScreenDataLayout, sizeof(SaveScreenDataData)));
  }
  SaveScreenData at(uint32_t i) const { return SaveScreenData(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SaveScreenDataData* direct() const { return static_cast<SaveScreenDataData*>(memory::HandleOwn(address, &kSaveScreenDataLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSaveScreenDataLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSaveScreenDataLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 1 once a dialog is set up
  uint32_t set_up_address() const { return address + 0; }
  uint32_t set_up_bits() const { if (auto* d = direct()) return d->set_up; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_set_up_bits(uint32_t v) const { if (auto* d = direct()) { d->set_up = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t set_up() const { if (auto* d = direct()) return d->set_up; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_set_up(uint32_t v) const { if (auto* d = direct()) { d->set_up = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the result the end pages post by (kind 0: 1 one answer, else the other; kinds 2 to 5: 1 to 3, else 0)
  uint32_t answer_address() const { return address + 4; }
  uint32_t answer_bits() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t answer() const { if (auto* d = direct()) return d->answer; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_answer(uint32_t v) const { if (auto* d = direct()) { d->answer = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // kinds 2 and 3's flag (1 for kind 2), read by page 14
  uint32_t kind2_flag_address() const { return address + 8; }
  uint32_t kind2_flag_bits() const { if (auto* d = direct()) return d->kind2_flag; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_kind2_flag_bits(uint32_t v) const { if (auto* d = direct()) { d->kind2_flag = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t kind2_flag() const { if (auto* d = direct()) return d->kind2_flag; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_kind2_flag(uint32_t v) const { if (auto* d = direct()) { d->kind2_flag = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the dialog's message (an index into the ShowMessage steps' tables)
  uint32_t message_address() const { return address + 12; }
  uint32_t message_bits() const { if (auto* d = direct()) return d->message; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_message_bits(uint32_t v) const { if (auto* d = direct()) { d->message = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t message() const { if (auto* d = direct()) return d->message; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_message(uint32_t v) const { if (auto* d = direct()) { d->message = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // 0 the question with two answers, 1 and 2 the note, -1 none
  uint32_t dialog_state_address() const { return address + 16; }
  uint32_t dialog_state_bits() const { if (auto* d = direct()) return d->dialog_state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_dialog_state_bits(uint32_t v) const { if (auto* d = direct()) { d->dialog_state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t dialog_state() const { if (auto* d = direct()) return d->dialog_state; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_dialog_state(uint32_t v) const { if (auto* d = direct()) { d->dialog_state = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  // step 2 (CloseDialog) copies it to dialog_state
  uint32_t state_after_close_address() const { return address + 20; }
  uint32_t state_after_close_bits() const { if (auto* d = direct()) return d->state_after_close; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_state_after_close_bits(uint32_t v) const { if (auto* d = direct()) { d->state_after_close = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t state_after_close() const { if (auto* d = direct()) return d->state_after_close; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_state_after_close(uint32_t v) const { if (auto* d = direct()) { d->state_after_close = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // the pages' wait counter (Draw's calls; 45 or 90)
  uint32_t wait_address() const { return address + 24; }
  uint32_t wait_bits() const { if (auto* d = direct()) return d->wait; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_wait_bits(uint32_t v) const { if (auto* d = direct()) { d->wait = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t wait() const { if (auto* d = direct()) return d->wait; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_wait(uint32_t v) const { if (auto* d = direct()) { d->wait = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // set by SaveScreen::Cancel; Draw turns it into the kind's end page
  uint32_t cancel_address() const { return address + 28; }
  uint32_t cancel_bits() const { if (auto* d = direct()) return d->cancel; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_cancel_bits(uint32_t v) const { if (auto* d = direct()) { d->cancel = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t cancel() const { if (auto* d = direct()) return d->cancel; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_cancel(uint32_t v) const { if (auto* d = direct()) { d->cancel = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // the machine's state after Draw (21 none; the end pages set 9)
  uint32_t next_state_address() const { return address + 32; }
  uint32_t next_state_bits() const { if (auto* d = direct()) return d->next_state; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_next_state_bits(uint32_t v) const { if (auto* d = direct()) { d->next_state = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t next_state() const { if (auto* d = direct()) return d->next_state; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_next_state(uint32_t v) const { if (auto* d = direct()) { d->next_state = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // 0 to 7, set by the open messages' handlers
  uint32_t kind_address() const { return address + 36; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  // a replay record's index (Replays::Load; the theater's pick)
  uint32_t replay_index_address() const { return address + 40; }
  uint32_t replay_index_bits() const { if (auto* d = direct()) return d->replay_index; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_replay_index_bits(uint32_t v) const { if (auto* d = direct()) { d->replay_index = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t replay_index() const { if (auto* d = direct()) return d->replay_index; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_replay_index(uint32_t v) const { if (auto* d = direct()) { d->replay_index = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  // kind 6's player (0, then 1)
  uint32_t player_address() const { return address + 44; }
  uint32_t player_bits() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_player_bits(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  uint32_t player() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_player(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  // the dialog's default answer
  uint32_t default_answer_address() const { return address + 48; }
  uint32_t default_answer_bits() const { if (auto* d = direct()) return d->default_answer; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_default_answer_bits(uint32_t v) const { if (auto* d = direct()) { d->default_answer = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  uint32_t default_answer() const { if (auto* d = direct()) return d->default_answer; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_default_answer(uint32_t v) const { if (auto* d = direct()) { d->default_answer = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  // kind 7's controller pick (0 to 3, the player who pressed)
  uint32_t controller_pick_address() const { return address + 52; }
  uint32_t controller_pick_bits() const { if (auto* d = direct()) return d->controller_pick; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_controller_pick_bits(uint32_t v) const { if (auto* d = direct()) { d->controller_pick = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  uint32_t controller_pick() const { if (auto* d = direct()) return d->controller_pick; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_controller_pick(uint32_t v) const { if (auto* d = direct()) { d->controller_pick = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  // 1 after a set-up (which prompts the theater's WaitPage shows: 0 none, 1 one pair)
  uint32_t prompts_address() const { return address + 56; }
  uint32_t prompts_bits() const { if (auto* d = direct()) return d->prompts; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_prompts_bits(uint32_t v) const { if (auto* d = direct()) { d->prompts = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  uint32_t prompts() const { if (auto* d = direct()) return d->prompts; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_prompts(uint32_t v) const { if (auto* d = direct()) { d->prompts = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  // 1 once SaveScreen::StoreSettings ran
  uint32_t settings_stored_address() const { return address + 60; }
  uint32_t settings_stored_bits() const { if (auto* d = direct()) return d->settings_stored; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_settings_stored_bits(uint32_t v) const { if (auto* d = direct()) { d->settings_stored = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t settings_stored() const { if (auto* d = direct()) return d->settings_stored; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_settings_stored(uint32_t v) const { if (auto* d = direct()) { d->settings_stored = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_64_bits() const { if (auto* d = direct()) return d->unknown_64; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_unknown_64_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_64 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64() const { if (auto* d = direct()) return d->unknown_64; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_unknown_64(uint32_t v) const { if (auto* d = direct()) { d->unknown_64 = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
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
