// The views of docs/functions/boot-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/boot-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Mode, BootMode, Mode::NextModeThunk.r4
#pragma pack(push, 1)
struct GameModeData {
  uint32_t vtable = 0;  // +0
  uint32_t delegate_ptr = 0;  // +4
  uint32_t delegate_vtable = 0;  // +8
  uint32_t delegate_object = 0;  // +12
  uint32_t delegate_function = 0;  // +16
  uint32_t delegate_adjustment = 0;  // +20
  uint8_t pad_24[8] = {};
  uint32_t vtable32 = 0;  // +32
  uint8_t handlers[200] = {};  // +36, guest order
  uint32_t handler_count = 0;  // +236
  uint8_t ring[600] = {};  // +240, guest order
  uint32_t ring_count = 0;  // +840
  uint32_t ring_start = 0;  // +844
};
#pragma pack(pop)
static_assert(sizeof(GameModeData) == 848);
inline constexpr memory::LayoutField kGameModeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {32, 4}, {236, 4}, {840, 4}, {844, 4}};
inline constexpr memory::Layout kGameModeLayout = {kGameModeFields, 10};

struct GameMode : View {
  using View::View;
  static constexpr uint32_t kSize = 848;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x820069EC;
  GameModeData* data() const { return static_cast<GameModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static GameMode New(uint8_t* base) {
    return GameMode(base, memory::MakeHandle(new GameModeData(), sizeof(GameModeData), "GameMode", &kGameModeLayout));
  }
  template <uint32_t Stride = kSize>
  static GameMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(GameModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<GameModeData*>(::operator new(sizeof(GameModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) GameModeData();
    return GameMode(base, memory::MakeHandle(p, sizeof(GameModeData) * count, "GameMode", &kGameModeLayout, sizeof(GameModeData)));
  }
  GameMode at(uint32_t i) const { return GameMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  GameModeData* direct() const { return static_cast<GameModeData*>(memory::HandleOwn(address, &kGameModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kGameModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kGameModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x820069EC the base's, 0x82006B70 the boot mode's, 0x82006B44 ModeType3's
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), not the mode's
  uint32_t answer_changed_address() const { return address + 0; }
  uint8_t answer_changed_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kGameModeLayout, 0, 1)); return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(base, address + 0))); }
  void set_answer_changed_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kGameModeLayout, 0, 1, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(base, address + 0, v); }
  uint8_t answer_changed() const { return uint8_t(answer_changed_bits()); }
  void set_answer_changed(uint8_t v) const { set_answer_changed_bits(uint8_t(v)); }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer), not the mode's
  uint32_t answer_address() const { return address + 0; }
  uint64_t answer_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kGameModeLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(base, address + 0))); }
  void set_answer_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kGameModeLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t answer() const { return uint64_t(answer_bits()); }
  void set_answer(uint64_t v) const { set_answer_bits(uint64_t(v)); }
  // the delegate at +8 (also the answer's next type +4 by the measure's join)
  uint32_t delegate_ptr_address() const { return address + 4; }
  uint32_t delegate_ptr_bits() const { if (auto* d = direct()) return d->delegate_ptr; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_delegate_ptr_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_ptr = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, delegate_ptr_bits()); }
  void set_delegate_ptr(const View& w) const { set_delegate_ptr_bits(w.address); }
  void set_delegate_ptr(uint32_t v) const { set_delegate_ptr_bits(v); }
  // the delegate (ModeDelegate): vtable 0x82010FA0
  uint32_t delegate_vtable_address() const { return address + 8; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the delegate's object: the mode
  uint32_t delegate_object_address() const { return address + 12; }
  uint32_t delegate_object_bits() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_delegate_object_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object() const { if (auto* d = direct()) return d->delegate_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_delegate_object(uint32_t v) const { if (auto* d = direct()) { d->delegate_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // 0x82235C08 Mode::NextModeThunk for the boot and race modes
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_function_bits() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_delegate_function_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function() const { if (auto* d = direct()) return d->delegate_function; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_delegate_function(uint32_t v) const { if (auto* d = direct()) { d->delegate_function = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  uint32_t delegate_adjustment_bits() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_delegate_adjustment_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment() const { if (auto* d = direct()) return d->delegate_adjustment; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_delegate_adjustment(uint32_t v) const { if (auto* d = direct()) { d->delegate_adjustment = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  // the second vtable: 0x82006A0C the base's, 0x82006B64 the boot mode's
  uint32_t vtable32_address() const { return address + 32; }
  uint32_t vtable32_bits() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32() const { if (auto* d = direct()) return d->vtable32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_vtable32(uint32_t v) const { if (auto* d = direct()) { d->vtable32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 36; }
  uint8_t* handlers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 36 : guest::Pointer(base, address + 36); }
  uint32_t handler_count_address() const { return address + 236; }
  uint32_t handler_count_bits() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(base, address + 236))); }
  void set_handler_count_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(base, address + 236))); }
  void set_handler_count(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(base, address + 236, v); }
  // the dispatcher's ring: 50 messages of 12 bytes (id, two words)
  uint32_t ring_address() const { return address + 240; }
  uint8_t* ring() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 240 : guest::Pointer(base, address + 240); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_count_bits() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 840) ? uint32_t(memory::OwnerLoad(address + 840, 4)) : guest::LoadU32(base, address + 840))); }
  void set_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 840)) memory::OwnerStore(address + 840, 4, v); else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 840) ? uint32_t(memory::OwnerLoad(address + 840, 4)) : guest::LoadU32(base, address + 840))); }
  void set_ring_count(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 840)) memory::OwnerStore(address + 840, 4, v); else guest::StoreU32(base, address + 840, v); }
  uint32_t ring_start_address() const { return address + 844; }
  uint32_t ring_start_bits() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 844) ? uint32_t(memory::OwnerLoad(address + 844, 4)) : guest::LoadU32(base, address + 844))); }
  void set_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 844)) memory::OwnerStore(address + 844, 4, v); else guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 844) ? uint32_t(memory::OwnerLoad(address + 844, 4)) : guest::LoadU32(base, address + 844))); }
  void set_ring_start(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 844)) memory::OwnerStore(address + 844, 4, v); else guest::StoreU32(base, address + 844, v); }
};

// allocated; measure: BootMode::NextMode.r4
struct BootMode : View {
  using View::View;
  static constexpr uint32_t kSize = 10460;
  static constexpr uint32_t kVtable = 0x82006B70;
  // 0x82006B70
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // 0x82006B64
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // 50 messages of 12 bytes
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // the boot machine (BootMachine)
  uint32_t machine_address() const { return address + 848; }
  // +848's state: 0 before the movie, 1 playing, 2 ended
  uint32_t machine_state() const { return guest::LoadU32(base, address + 10452); }
  void set_machine_state(uint32_t v) const { guest::StoreU32(base, address + 10452, v); }
  uint32_t machine_state_address() const { return address + 10452; }
  // +848's +9608: 1 once Movies::Start was called
  uint32_t movie_started() const { return guest::LoadU32(base, address + 10456); }
  void set_movie_started(uint32_t v) const { guest::StoreU32(base, address + 10456, v); }
  uint32_t movie_started_address() const { return address + 10456; }
};

// embedded; measure: Boot, BootMode@+848
struct BootMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9612;
  // 200 transitions of 48 bytes
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 0 before the movie, 1 playing, 2 ended
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once Movies::Start was called (Boot::WaitForMovie); 0 from BootMachine::Init
  uint32_t movie_started() const { return guest::LoadU32(base, address + 9608); }
  void set_movie_started(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t movie_started_address() const { return address + 9608; }
};

// embedded; measure: Boot::WaitForMovie.r4, TitleLoad::OpenTitle.r4, TitleLoad::Leave.r4, TitleLoad::Update.r4, TitleScreen::OpenNext.r4, TitleScreen::Leave.r4, TodaysResult::Leave.r4, TodaysEvent::Leave.r4, TodaysEvent::Unloaded.r4, ReplayTheater::Wait.r4, ReplayTheater::Leave.r4, Type5Dialog::Update.r4, Type6ListScreen::Back.r4, Type7ListScreen::Back.r4, Type6NodeScreen::Back.r4, Type6PanelScreen::Back.r4, Type7PanelScreen::Back.r4, Type6PanelScreen::OpenPowerGraph.r4, Type7PanelScreen::OpenPowerGraph.r4, Type6PanelScreen::PickCar.r4, Type7PanelScreen::PickCar.r4, sub_82230BA8.r4, sub_82240518.r4, ControllerOptions::Leave.r4, Dialog::Leave.r4, Type6Dialog::Cancel.r4, Type6Dialog::ConfirmPage.r4, Type6Dialog::NoticePage.r4, Type6Dialog::Update.r4, Type7Dialog::Cancel.r4, Type7Dialog::ConfirmPage.r4, Type7Dialog::Leave.r4, Type7Dialog::NoticePage.r4, Type7Dialog::Update.r4, Type6Screen0::ChosenPage.r4, Type6Screen0::Confirm.r4, Type6Screen0::ConfirmAgain.r4, Type6Screen0::Leave.r4, Type6Screen0::MenuPage.r4, Type6Screen0::SetUpCar.r4, Type7Screen0::ChosenPage.r4, Type7Screen0::Confirm.r4, Type7Screen0::Leave.r4, Type7Screen0::MenuPage.r4, Type7Screen0::SetUpCar.r4, Type8PanelScreen::UpdateOpening.r4, Type8PanelScreen::Leave.r4, Type8PanelScreen::AskQuit.r4, Type8ListScreen::SetUpRace.r4, Type8ColorListScreen::SetUpRace.r4, sub_8226A660.r4, sub_82263D80.r4, sub_822657D8.r4, sub_82266028.r4, sub_822662F0.r4, sub_822663E0.r4, sub_82266548.r4, sub_82266658.r4, sub_82266750.r4, sub_822681D0.r4, sub_82269988.r4, sub_822702C0.r4, sub_82277178.r4, sub_82277340.r4, sub_822858D8.r4, sub_82282BC8.r6
#pragma pack(push, 1)
struct ModeDispatcherData {
  uint8_t handlers[200] = {};  // +0, guest order
  uint32_t handler_count = 0;  // +200
  uint8_t ring[600] = {};  // +204, guest order
  uint32_t ring_count = 0;  // +804
  uint32_t ring_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(ModeDispatcherData) == 812);
inline constexpr memory::LayoutField kModeDispatcherFields[] = {{200, 4}, {804, 4}, {808, 4}};
inline constexpr memory::Layout kModeDispatcherLayout = {kModeDispatcherFields, 3};

struct ModeDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ModeDispatcherData* data() const { return static_cast<ModeDispatcherData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeDispatcher New(uint8_t* base) {
    return ModeDispatcher(base, memory::MakeHandle(new ModeDispatcherData(), sizeof(ModeDispatcherData), "ModeDispatcher", &kModeDispatcherLayout));
  }
  template <uint32_t Stride = kSize>
  static ModeDispatcher NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeDispatcherData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeDispatcherData*>(::operator new(sizeof(ModeDispatcherData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeDispatcherData();
    return ModeDispatcher(base, memory::MakeHandle(p, sizeof(ModeDispatcherData) * count, "ModeDispatcher", &kModeDispatcherLayout, sizeof(ModeDispatcherData)));
  }
  ModeDispatcher at(uint32_t i) const { return ModeDispatcher(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeDispatcherData* direct() const { return static_cast<ModeDispatcherData*>(memory::HandleOwn(address, &kModeDispatcherLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeDispatcherLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeDispatcherLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 0; }
  uint8_t* handlers() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(base, address + 0); }
  uint32_t handler_count_address() const { return address + 200; }
  uint32_t handler_count_bits() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(base, address + 200))); }
  void set_handler_count_bits(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(base, address + 200, v); }
  uint32_t handler_count() const { if (auto* d = direct()) return d->handler_count; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(base, address + 200))); }
  void set_handler_count(uint32_t v) const { if (auto* d = direct()) { d->handler_count = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(base, address + 200, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t ring_address() const { return address + 204; }
  uint8_t* ring() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(base, address + 204); }
  uint32_t ring_count_address() const { return address + 804; }
  uint32_t ring_count_bits() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_ring_count_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count() const { if (auto* d = direct()) return d->ring_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_ring_count(uint32_t v) const { if (auto* d = direct()) { d->ring_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t ring_start_address() const { return address + 808; }
  uint32_t ring_start_bits() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_ring_start_bits(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start() const { if (auto* d = direct()) return d->ring_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_ring_start(uint32_t v) const { if (auto* d = direct()) { d->ring_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
};

// embedded; measure: sub_82107D80.r3
struct BootModeInterface32 : View {
  using View::View;
  static constexpr uint32_t kSize = 816;
  // 0x82006B64
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the mode's ring (+240)
  uint32_t ring_address() const { return address + 208; }
  // the mode's +840
  uint32_t ring_count() const { return guest::LoadU32(base, address + 808); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_count_address() const { return address + 808; }
  // the mode's +844
  uint32_t ring_start() const { return guest::LoadU32(base, address + 812); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 812, v); }
  uint32_t ring_start_address() const { return address + 812; }
};

// image; measure: Mode::NextModeThunk.r4.+0
struct GameModeVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 0: BootMode::Init for the boot mode
  uint32_t init() const { return guest::LoadU32(base, address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  uint32_t slot1() const { return guest::LoadU32(base, address + 4); }
  void set_slot1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot1_address() const { return address + 4; }
  // slots 2 to 5: Mode::NoNextMode in every mode
  uint32_t slot2() const { return guest::LoadU32(base, address + 8); }
  void set_slot2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot2_address() const { return address + 8; }
  uint32_t slot3() const { return guest::LoadU32(base, address + 12); }
  void set_slot3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot3_address() const { return address + 12; }
  uint32_t slot4() const { return guest::LoadU32(base, address + 16); }
  void set_slot4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot4_address() const { return address + 16; }
  uint32_t slot5() const { return guest::LoadU32(base, address + 20); }
  void set_slot5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot5_address() const { return address + 20; }
  // slot 6: the next-mode query (BootMode::NextMode)
  uint32_t next_mode() const { return guest::LoadU32(base, address + 24); }
  void set_next_mode(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t next_mode_address() const { return address + 24; }
  // slot 7
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 28); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t deleting_destructor_address() const { return address + 28; }
};

// embedded; measure: ModeDelegate::Invoke.r4
#pragma pack(push, 1)
struct ModeDelegateData {
  uint32_t vtable = 0;  // +0
  uint32_t object = 0;  // +4
  uint32_t function = 0;  // +8
  uint32_t adjustment = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(ModeDelegateData) == 16);
inline constexpr memory::LayoutField kModeDelegateFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kModeDelegateLayout = {kModeDelegateFields, 4};

struct ModeDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ModeDelegateData* data() const { return static_cast<ModeDelegateData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ModeDelegate New(uint8_t* base) {
    return ModeDelegate(base, memory::MakeHandle(new ModeDelegateData(), sizeof(ModeDelegateData), "ModeDelegate", &kModeDelegateLayout));
  }
  template <uint32_t Stride = kSize>
  static ModeDelegate NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ModeDelegateData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ModeDelegateData*>(::operator new(sizeof(ModeDelegateData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ModeDelegateData();
    return ModeDelegate(base, memory::MakeHandle(p, sizeof(ModeDelegateData) * count, "ModeDelegate", &kModeDelegateLayout, sizeof(ModeDelegateData)));
  }
  ModeDelegate at(uint32_t i) const { return ModeDelegate(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ModeDelegateData* direct() const { return static_cast<ModeDelegateData*>(memory::HandleOwn(address, &kModeDelegateLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kModeDelegateLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kModeDelegateLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x82010FA0
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the mode
  uint32_t object_address() const { return address + 4; }
  uint32_t object_bits() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_object_bits(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t object() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_object(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // Mode::NextModeThunk (0x82235C08) for the boot and race modes
  uint32_t function_address() const { return address + 8; }
  uint32_t function_bits() const { if (auto* d = direct()) return d->function; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_function_bits(uint32_t v) const { if (auto* d = direct()) { d->function = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t function() const { if (auto* d = direct()) return d->function; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_function(uint32_t v) const { if (auto* d = direct()) { d->function = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // added to the object
  uint32_t adjustment_address() const { return address + 12; }
  uint32_t adjustment_bits() const { if (auto* d = direct()) return d->adjustment; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_adjustment_bits(uint32_t v) const { if (auto* d = direct()) { d->adjustment = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t adjustment() const { if (auto* d = direct()) return d->adjustment; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_adjustment(uint32_t v) const { if (auto* d = direct()) { d->adjustment = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
};

// embedded; measure: ModeDelegate
struct ModeAnswer : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 1 to change modes
  uint8_t changed() const { return guest::LoadU8(base, address + 0); }
  void set_changed(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t changed_address() const { return address + 0; }
  // the first word as copied: changed and three bytes the stack's
  uint32_t word0() const { return guest::LoadU32(base, address + 0); }
  void set_word0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word0_address() const { return address + 0; }
  // the next mode's type
  uint32_t next_type() const { return guest::LoadU32(base, address + 4); }
  void set_next_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_type_address() const { return address + 4; }
};

// allocated; measure: ModeType8
struct ModeType8 : View {
  using View::View;
  static constexpr uint32_t kSize = 87768;
  static constexpr uint32_t kVtable = 0x82006D60;
  // 0x82006D60
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // 0x82006D54
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher: 50 handler pointers
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // 50 messages of 12 bytes
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // ModeType8Member
  uint32_t member_address() const { return address + 848; }
};

// embedded; measure: ModeType8@+848
struct ModeType8Member : View {
  using View::View;
  static constexpr uint32_t kSize = 86920;
  // Type8Screen0 (its machine +4)
  uint32_t screen0_address() const { return address + 0; }
  // Type8Dialog
  uint32_t dialog_address() const { return address + 9792; }
  // the list screen
  uint32_t list_screen_address() const { return address + 19544; }
  // the color list screen
  uint32_t color_list_screen_address() const { return address + 30912; }
  // the panel screen
  uint32_t panel_screen_address() const { return address + 42740; }
  // the options (a ScreenMachine)
  uint32_t options_address() const { return address + 57992; }
  // the controller pages (a ScreenMachine)
  uint32_t controller_pages_address() const { return address + 67632; }
  // the save screen (a ScreenMachine)
  uint32_t save_screen_address() const { return address + 77272; }
  // the mode's +87760: the 8-byte answer TearDown returns, its byte 1 to change modes; cleared by the constructor
  uint8_t answer_changed() const { return guest::LoadU8(base, address + 86912); }
  void set_answer_changed(uint8_t v) const { guest::StoreU8(base, address + 86912, v); }
  uint32_t answer_changed_address() const { return address + 86912; }
  // the answer's next mode (1 the race, 4 the type 4 front end, 12); cleared by the constructor
  uint32_t answer_next_type() const { return guest::LoadU32(base, address + 86916); }
  void set_answer_next_type(uint32_t v) const { guest::StoreU32(base, address + 86916, v); }
  uint32_t answer_next_type_address() const { return address + 86916; }
};

// The image words docs/functions/boot-mode.md's code uses.
struct BootModeGlobals {
  // the boot movie paused (1); BootMode::NextMode
  static uint32_t movie_paused(uint8_t* base) { return guest::LoadU32(base, 0x826C5BA4u); }
  static void set_movie_paused(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BA4u, v); }
  static constexpr uint32_t movie_paused_address() { return 0x826C5BA4u; }
  // the mode the boot mode is asked to go to (13 none)
  static uint32_t mode_asked(uint8_t* base) { return guest::LoadU32(base, 0x826C5BA8u); }
  static void set_mode_asked(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BA8u, v); }
  static constexpr uint32_t mode_asked_address() { return 0x826C5BA8u; }
  // up to five movie ids, 11 ending the list
  static constexpr uint32_t movie_ids_address() { return 0x826C5BACu; }
  // the mode type after the list (4 at boot)
  static uint32_t next_type(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC0u); }
  static void set_next_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC0u, v); }
  static constexpr uint32_t next_type_address() { return 0x826C5BC0u; }
  // a button skips the movies
  static uint32_t skippable(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC4u); }
  static void set_skippable(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC4u, v); }
  static constexpr uint32_t skippable_address() { return 0x826C5BC4u; }
  // the list's next entry
  static uint32_t movie_index(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC8u); }
  static void set_movie_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC8u, v); }
  static constexpr uint32_t movie_index_address() { return 0x826C5BC8u; }
  // the pad blocked (known-data.md): either word set
  static uint32_t pad_blocked0(uint8_t* base) { return guest::LoadU32(base, 0x826C7960u); }
  static void set_pad_blocked0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7960u, v); }
  static constexpr uint32_t pad_blocked0_address() { return 0x826C7960u; }
  static uint32_t pad_blocked1(uint8_t* base) { return guest::LoadU32(base, 0x826C7964u); }
  static void set_pad_blocked1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C7964u, v); }
  static constexpr uint32_t pad_blocked1_address() { return 0x826C7964u; }
  // the passes word ScreenFades::FadeIn is given at a movie's start (5)
  static uint32_t start_fade_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_start_fade_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t start_fade_passes_address() { return 0x82001BECu; }
};

}  // namespace schema
