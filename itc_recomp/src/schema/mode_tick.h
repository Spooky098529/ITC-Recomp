// The views of docs/functions/mode-tick.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/mode-tick.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: ColorRect
struct ColorRect : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  // an identity matrix (ScreenFades::Update)
  uint32_t matrix_address() const { return address + 0; }
  // 0 flat (RecordFlat), 1 sub_8217A758 (never ran), above 1 nothing
  uint32_t kind() const { return guest::LoadU32(address + 64); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t kind_address() const { return address + 64; }
  // 0 from ScreenFades::Update
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  // 2 from ScreenFades::Update
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  int32_t x() const { return int32_t(guest::LoadU32(address + 80)); }
  void set_x(int32_t v) const { guest::StoreU32(address + 80, uint32_t(v)); }
  uint32_t x_address() const { return address + 80; }
  int32_t y() const { return int32_t(guest::LoadU32(address + 84)); }
  void set_y(int32_t v) const { guest::StoreU32(address + 84, uint32_t(v)); }
  uint32_t y_address() const { return address + 84; }
  uint32_t width() const { return guest::LoadU32(address + 88); }
  void set_width(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t width_address() const { return address + 88; }
  uint32_t height() const { return guest::LoadU32(address + 92); }
  void set_height(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t height_address() const { return address + 92; }
  // 0 to 255
  uint32_t red() const { return guest::LoadU32(address + 96); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t red_address() const { return address + 96; }
  // red's low byte (RecordFlat's vertex color)
  uint8_t red_byte() const { return guest::LoadU8(address + 99); }
  void set_red_byte(uint8_t v) const { guest::StoreU8(address + 99, v); }
  uint32_t red_byte_address() const { return address + 99; }
  uint32_t green() const { return guest::LoadU32(address + 100); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t green_address() const { return address + 100; }
  // green's low byte
  uint8_t green_byte() const { return guest::LoadU8(address + 103); }
  void set_green_byte(uint8_t v) const { guest::StoreU8(address + 103, v); }
  uint32_t green_byte_address() const { return address + 103; }
  uint32_t blue() const { return guest::LoadU32(address + 104); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t blue_address() const { return address + 104; }
  // blue's low byte
  uint8_t blue_byte() const { return guest::LoadU8(address + 107); }
  void set_blue_byte(uint8_t v) const { guest::StoreU8(address + 107, v); }
  uint32_t blue_byte_address() const { return address + 107; }
  // 0 to 255: above 0 blends
  int32_t alpha() const { return int32_t(guest::LoadU32(address + 108)); }
  void set_alpha(int32_t v) const { guest::StoreU32(address + 108, uint32_t(v)); }
  uint32_t alpha_address() const { return address + 108; }
};

// embedded; measure: sub_8209E540.r4, sub_8209E610.r4, sub_8209E790.r4
#pragma pack(push, 1)
struct SignInModeInterfaceData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(SignInModeInterfaceData) == 4);
inline constexpr memory::LayoutField kSignInModeInterfaceFields[] = {{0, 4}};
inline constexpr memory::Layout kSignInModeInterfaceLayout = {kSignInModeInterfaceFields, 1};

struct SignInModeInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SignInModeInterfaceData* data() const { return static_cast<SignInModeInterfaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SignInModeInterface New() {
    return SignInModeInterface(memory::MakeHandle(new SignInModeInterfaceData(), sizeof(SignInModeInterfaceData), "SignInModeInterface", &kSignInModeInterfaceLayout));
  }
  template <uint32_t Stride = kSize>
  static SignInModeInterface NewArray(uint32_t count) {
    static_assert(sizeof(SignInModeInterfaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SignInModeInterfaceData*>(::operator new(sizeof(SignInModeInterfaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SignInModeInterfaceData();
    return SignInModeInterface(memory::MakeHandle(p, sizeof(SignInModeInterfaceData) * count, "SignInModeInterface", &kSignInModeInterfaceLayout, sizeof(SignInModeInterfaceData)));
  }
  SignInModeInterface at(uint32_t i) const { return SignInModeInterface(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SignInModeInterfaceData* direct() const { return static_cast<SignInModeInterfaceData*>(memory::HandleOwn(address, &kSignInModeInterfaceLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSignInModeInterfaceLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSignInModeInterfaceLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = SignInModeInterfaceVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
};

// image; measure: sub_8209E540.r4.+0, sub_8209E610.r4.+0, sub_8209E790.r4.+0
struct SignInModeInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot 0 (the type 12 mode's Interrupt; sub_8209E540 calls it)
  uint32_t interrupt() const { return guest::LoadU32(address + 0); }
  void set_interrupt(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t interrupt_address() const { return address + 0; }
  // slot 1 (ForceExit; sub_8209E790)
  uint32_t force_exit() const { return guest::LoadU32(address + 4); }
  void set_force_exit(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t force_exit_address() const { return address + 4; }
  // slot 2 (Resume; sub_8209E610)
  uint32_t resume() const { return guest::LoadU32(address + 8); }
  void set_resume(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t resume_address() const { return address + 8; }
};

// embedded; measure: sub_8214E818.r3
struct NetIdRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // compared byte by byte
  uint32_t id_address() const { return address + 8; }
};

// embedded; measure: sub_8214E818.r4
struct NetId : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t id_address() const { return address + 0; }
};

// embedded; measure: sub_82260758.r3
struct NetTimeoutFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 after the timeout (probably)
  uint32_t flag() const { return guest::LoadU32(address + 0); }
  void set_flag(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t flag_address() const { return address + 0; }
};

// embedded; measure: sub_82262040.r3, sub_82262E10.r3, sub_82260A30.r3
struct ModeTickNet8Machine : View {
  using View::View;
  static constexpr uint32_t kSize = 9668;
  // a StateMachine (game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  // 1: Destroy goes to state 14
  uint32_t back() const { return guest::LoadU32(address + 9624); }
  void set_back(uint32_t v) const { guest::StoreU32(address + 9624, v); }
  uint32_t back_address() const { return address + 9624; }
  // 0 at Create
  uint32_t race() const { return guest::LoadU32(address + 9628); }
  void set_race(uint32_t v) const { guest::StoreU32(address + 9628, v); }
  uint32_t race_address() const { return address + 9628; }
  // 1: Destroy goes to state 15
  uint32_t leave() const { return guest::LoadU32(address + 9632); }
  void set_leave(uint32_t v) const { guest::StoreU32(address + 9632, v); }
  uint32_t leave_address() const { return address + 9632; }
  // set by ModeType12::Interrupt on the shown machine
  uint32_t shown() const { return guest::LoadU32(address + 9636); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 9636, v); }
  uint32_t shown_address() const { return address + 9636; }
  // 0 at Create
  uint32_t unknown_9640() const { return guest::LoadU32(address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
  // 0 at Create
  uint32_t unknown_9644() const { return guest::LoadU32(address + 9644); }
  void set_unknown_9644(uint32_t v) const { guest::StoreU32(address + 9644, v); }
  uint32_t unknown_9644_address() const { return address + 9644; }
  // 0 at Create
  uint32_t unknown_9648() const { return guest::LoadU32(address + 9648); }
  void set_unknown_9648(uint32_t v) const { guest::StoreU32(address + 9648, v); }
  uint32_t unknown_9648_address() const { return address + 9648; }
  // 0 at Create; the measure joins it with [0x82816120]'s pointee
  uint32_t unknown_9652() const { return guest::LoadU32(address + 9652); }
  void set_unknown_9652(uint32_t v) const { guest::StoreU32(address + 9652, v); }
  uint32_t unknown_9652_address() const { return address + 9652; }
  // 0 at Create
  uint32_t unknown_9656() const { return guest::LoadU32(address + 9656); }
  void set_unknown_9656(uint32_t v) const { guest::StoreU32(address + 9656, v); }
  uint32_t unknown_9656_address() const { return address + 9656; }
  // 1 at Create
  uint32_t unknown_9660() const { return guest::LoadU32(address + 9660); }
  void set_unknown_9660(uint32_t v) const { guest::StoreU32(address + 9660, v); }
  uint32_t unknown_9660_address() const { return address + 9660; }
  // 1 at Create
  uint32_t unknown_9664() const { return guest::LoadU32(address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
};

// embedded; measure: sub_82264B90.r3
struct ModeTickNet9Machine : View {
  using View::View;
  static constexpr uint32_t kSize = 9696;
  // a StateMachine (game-modes.toml)
  uint32_t machine_address() const { return address + 0; }
  // 1: Destroy goes to state 14
  uint32_t back() const { return guest::LoadU32(address + 9624); }
  void set_back(uint32_t v) const { guest::StoreU32(address + 9624, v); }
  uint32_t back_address() const { return address + 9624; }
  // 0 at Create
  uint32_t race() const { return guest::LoadU32(address + 9628); }
  void set_race(uint32_t v) const { guest::StoreU32(address + 9628, v); }
  uint32_t race_address() const { return address + 9628; }
  // 1: Destroy goes to state 15
  uint32_t leave() const { return guest::LoadU32(address + 9632); }
  void set_leave(uint32_t v) const { guest::StoreU32(address + 9632, v); }
  uint32_t leave_address() const { return address + 9632; }
  // 1 at Create
  uint32_t unknown_9640() const { return guest::LoadU32(address + 9640); }
  void set_unknown_9640(uint32_t v) const { guest::StoreU32(address + 9640, v); }
  uint32_t unknown_9640_address() const { return address + 9640; }
  // 1 at Create
  uint32_t unknown_9644() const { return guest::LoadU32(address + 9644); }
  void set_unknown_9644(uint32_t v) const { guest::StoreU32(address + 9644, v); }
  uint32_t unknown_9644_address() const { return address + 9644; }
  // 0 at Create
  uint32_t unknown_9648() const { return guest::LoadU32(address + 9648); }
  void set_unknown_9648(uint32_t v) const { guest::StoreU32(address + 9648, v); }
  uint32_t unknown_9648_address() const { return address + 9648; }
  // 0 at Create; a save data address (sub_821059D0's answer, probably)
  uint32_t save_data_a() const { return guest::LoadU32(address + 9652); }
  void set_save_data_a(uint32_t v) const { guest::StoreU32(address + 9652, v); }
  uint32_t save_data_a_address() const { return address + 9652; }
  // 0 at Create; a save data address (sub_821059D0's answer, probably)
  uint32_t save_data_b() const { return guest::LoadU32(address + 9656); }
  void set_save_data_b(uint32_t v) const { guest::StoreU32(address + 9656, v); }
  uint32_t save_data_b_address() const { return address + 9656; }
  // 0 at Create
  uint32_t unknown_9660() const { return guest::LoadU32(address + 9660); }
  void set_unknown_9660(uint32_t v) const { guest::StoreU32(address + 9660, v); }
  uint32_t unknown_9660_address() const { return address + 9660; }
  // 0 at Create
  uint32_t unknown_9664() const { return guest::LoadU32(address + 9664); }
  void set_unknown_9664(uint32_t v) const { guest::StoreU32(address + 9664, v); }
  uint32_t unknown_9664_address() const { return address + 9664; }
  // 0 at Create
  uint32_t unknown_9668() const { return guest::LoadU32(address + 9668); }
  void set_unknown_9668(uint32_t v) const { guest::StoreU32(address + 9668, v); }
  uint32_t unknown_9668_address() const { return address + 9668; }
  // 0 at Create
  uint32_t unknown_9672() const { return guest::LoadU32(address + 9672); }
  void set_unknown_9672(uint32_t v) const { guest::StoreU32(address + 9672, v); }
  uint32_t unknown_9672_address() const { return address + 9672; }
  // 0 at Create
  uint32_t unknown_9676() const { return guest::LoadU32(address + 9676); }
  void set_unknown_9676(uint32_t v) const { guest::StoreU32(address + 9676, v); }
  uint32_t unknown_9676_address() const { return address + 9676; }
  // 1 at Create
  uint32_t unknown_9680() const { return guest::LoadU32(address + 9680); }
  void set_unknown_9680(uint32_t v) const { guest::StoreU32(address + 9680, v); }
  uint32_t unknown_9680_address() const { return address + 9680; }
  // 0 at Create
  uint32_t unknown_9684() const { return guest::LoadU32(address + 9684); }
  void set_unknown_9684(uint32_t v) const { guest::StoreU32(address + 9684, v); }
  uint32_t unknown_9684_address() const { return address + 9684; }
  // 1 at Create
  uint32_t unknown_9688() const { return guest::LoadU32(address + 9688); }
  void set_unknown_9688(uint32_t v) const { guest::StoreU32(address + 9688, v); }
  uint32_t unknown_9688_address() const { return address + 9688; }
  // 0 at Create
  uint32_t unknown_9692() const { return guest::LoadU32(address + 9692); }
  void set_unknown_9692(uint32_t v) const { guest::StoreU32(address + 9692, v); }
  uint32_t unknown_9692_address() const { return address + 9692; }
};

// allocated; measure: new<28>@Type12Screen8::Create
struct ModeTickNet8Page : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the page Draw runs; -1 while a step runs
  uint32_t page() const { return guest::LoadU32(address + 0); }
  void set_page(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t page_address() const { return address + 0; }
  uint32_t next_page() const { return guest::LoadU32(address + 4); }
  void set_next_page(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t next_page_address() const { return address + 4; }
  uint32_t previous_page() const { return guest::LoadU32(address + 8); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t previous_page_address() const { return address + 8; }
  // the step Update runs; -1 none
  uint32_t step() const { return guest::LoadU32(address + 12); }
  void set_step(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t step_address() const { return address + 12; }
  uint32_t queued_step() const { return guest::LoadU32(address + 16); }
  void set_queued_step(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t queued_step_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t step_started() const { return guest::LoadU32(address + 24); }
  void set_step_started(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t step_started_address() const { return address + 24; }
  // measure fault: the machine's +9652, not the page object's
  uint32_t fault_9652() const { return guest::LoadU32(address + 9652); }
  void set_fault_9652(uint32_t v) const { guest::StoreU32(address + 9652, v); }
  uint32_t fault_9652_address() const { return address + 9652; }
};

// allocated; measure: new<68>@Type12Screen8::Create
struct ModeTickNet8Menu : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t cursor() const { return guest::LoadU32(address + 0); }
  void set_cursor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t cursor_address() const { return address + 0; }
  uint32_t previous() const { return guest::LoadU32(address + 4); }
  void set_previous(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  uint32_t moved() const { return guest::LoadU32(address + 8); }
  void set_moved(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t moved_address() const { return address + 8; }
  uint32_t first_item() const { return guest::LoadU32(address + 12); }
  void set_first_item(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t first_item_address() const { return address + 12; }
  // 0 at Create (the measure's pointer here is a join)
  uint32_t last_item() const { return guest::LoadU32(address + 16); }
  void set_last_item(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t last_item_address() const { return address + 16; }
  // 1 at Create
  uint32_t wraps() const { return guest::LoadU32(address + 20); }
  void set_wraps(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t wraps_address() const { return address + 20; }
  uint32_t up_down_locked() const { return guest::LoadU32(address + 24); }
  void set_up_down_locked(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t up_down_locked_address() const { return address + 24; }
  uint32_t left_right_steps() const { return guest::LoadU32(address + 28); }
  void set_left_right_steps(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t left_right_steps_address() const { return address + 28; }
  uint32_t shoulders_step() const { return guest::LoadU32(address + 32); }
  void set_shoulders_step(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t shoulders_step_address() const { return address + 32; }
  int32_t side() const { return int32_t(guest::LoadU32(address + 36)); }
  void set_side(int32_t v) const { guest::StoreU32(address + 36, uint32_t(v)); }
  uint32_t side_address() const { return address + 36; }
  uint32_t wrapped() const { return guest::LoadU32(address + 40); }
  void set_wrapped(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t wrapped_address() const { return address + 40; }
  uint32_t list_wrapped() const { return guest::LoadU32(address + 44); }
  void set_list_wrapped(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t list_wrapped_address() const { return address + 44; }
  uint32_t visible_row() const { return guest::LoadU32(address + 48); }
  void set_visible_row(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t visible_row_address() const { return address + 48; }
  uint32_t previous_row() const { return guest::LoadU32(address + 52); }
  void set_previous_row(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t previous_row_address() const { return address + 52; }
  uint32_t first_row() const { return guest::LoadU32(address + 56); }
  void set_first_row(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t first_row_address() const { return address + 56; }
  uint32_t rows_shown() const { return guest::LoadU32(address + 60); }
  void set_rows_shown(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t rows_shown_address() const { return address + 60; }
  uint32_t player() const { return guest::LoadU32(address + 64); }
  void set_player(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t player_address() const { return address + 64; }
  // measure fault: a machine's +9652
  uint32_t fault_9652() const { return guest::LoadU32(address + 9652); }
  void set_fault_9652(uint32_t v) const { guest::StoreU32(address + 9652, v); }
  uint32_t fault_9652_address() const { return address + 9652; }
  // measure fault: a join with another object
  uint32_t fault_24904() const { return guest::LoadU32(address + 24904); }
  void set_fault_24904(uint32_t v) const { guest::StoreU32(address + 24904, v); }
  uint32_t fault_24904_address() const { return address + 24904; }
};

// file; measure: new<68>@Type12Screen8::Create.+24904
struct ModeTickNet8MenuJoin : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  uint32_t cursor() const { return guest::LoadU32(address + 0); }
  void set_cursor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t cursor_address() const { return address + 0; }
  uint32_t previous() const { return guest::LoadU32(address + 4); }
  void set_previous(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  uint32_t moved() const { return guest::LoadU32(address + 8); }
  void set_moved(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t moved_address() const { return address + 8; }
  uint32_t first_item() const { return guest::LoadU32(address + 12); }
  void set_first_item(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t first_item_address() const { return address + 12; }
  uint32_t last_item() const { return guest::LoadU32(address + 16); }
  void set_last_item(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t last_item_address() const { return address + 16; }
  uint32_t wraps() const { return guest::LoadU32(address + 20); }
  void set_wraps(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t wraps_address() const { return address + 20; }
  uint32_t up_down_locked() const { return guest::LoadU32(address + 24); }
  void set_up_down_locked(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t up_down_locked_address() const { return address + 24; }
  uint32_t left_right_steps() const { return guest::LoadU32(address + 28); }
  void set_left_right_steps(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t left_right_steps_address() const { return address + 28; }
  int32_t side() const { return int32_t(guest::LoadU32(address + 36)); }
  void set_side(int32_t v) const { guest::StoreU32(address + 36, uint32_t(v)); }
  uint32_t side_address() const { return address + 36; }
  uint32_t list_wrapped() const { return guest::LoadU32(address + 44); }
  void set_list_wrapped(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t list_wrapped_address() const { return address + 44; }
  uint32_t visible_row() const { return guest::LoadU32(address + 48); }
  void set_visible_row(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t visible_row_address() const { return address + 48; }
  uint32_t previous_row() const { return guest::LoadU32(address + 52); }
  void set_previous_row(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t previous_row_address() const { return address + 52; }
  uint32_t first_row() const { return guest::LoadU32(address + 56); }
  void set_first_row(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t first_row_address() const { return address + 56; }
  uint32_t rows_shown() const { return guest::LoadU32(address + 60); }
  void set_rows_shown(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t rows_shown_address() const { return address + 60; }
  uint32_t player() const { return guest::LoadU32(address + 64); }
  void set_player(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t player_address() const { return address + 64; }
};

// The image words docs/functions/mode-tick.md's code uses.
struct ModeTickGlobals {
  // the play time's frame counter, up by one a pass
  static uint32_t play_time_frames() { return guest::LoadU32(0x826C5B90u); }
  static void set_play_time_frames(uint32_t v) { guest::StoreU32(0x826C5B90u, v); }
  static constexpr uint32_t play_time_frames_address() { return 0x826C5B90u; }
  // the menu music asked for once
  static uint32_t music_started() { return guest::LoadU32(0x826C5B94u); }
  static void set_music_started(uint32_t v) { guest::StoreU32(0x826C5B94u, v); }
  static constexpr uint32_t music_started_address() { return 0x826C5B94u; }
  // 1 for the front-end types (2, 4 to 10, 12)
  static uint32_t menu_mode() { return guest::LoadU32(0x826C5B98u); }
  static void set_menu_mode(uint32_t v) { guest::StoreU32(0x826C5B98u, v); }
  static constexpr uint32_t menu_mode_address() { return 0x826C5B98u; }
  // the state layer's slot 1, asked after the first mode is made
  static uint32_t layer_answer_0() { return guest::LoadU32(0x826C5B60u); }
  static void set_layer_answer_0(uint32_t v) { guest::StoreU32(0x826C5B60u, v); }
  static constexpr uint32_t layer_answer_0_address() { return 0x826C5B60u; }
  // the same, asked again
  static uint32_t layer_answer_1() { return guest::LoadU32(0x826C5B64u); }
  static void set_layer_answer_1(uint32_t v) { guest::StoreU32(0x826C5B64u, v); }
  static constexpr uint32_t layer_answer_1_address() { return 0x826C5B64u; }
  // the object passed to the sign-in watch (not read)
  static constexpr uint32_t sign_in_watch_address() { return 0x826C5B9Cu; }
  // the current mode's type
  static uint32_t mode_type() { return guest::LoadU32(0x826032DCu); }
  static void set_mode_type(uint32_t v) { guest::StoreU32(0x826032DCu, v); }
  static constexpr uint32_t mode_type_address() { return 0x826032DCu; }
  // the type before it
  static uint32_t old_mode_type() { return guest::LoadU32(0x826032E0u); }
  static void set_old_mode_type(uint32_t v) { guest::StoreU32(0x826032E0u, v); }
  static constexpr uint32_t old_mode_type_address() { return 0x826032E0u; }
  // the action mappers' client list (action-mappers.md)
  static uint32_t mapper_clients() { return guest::LoadU32(0x826BAB7Cu); }
  static void set_mapper_clients(uint32_t v) { guest::StoreU32(0x826BAB7Cu, v); }
  static constexpr uint32_t mapper_clients_address() { return 0x826BAB7Cu; }
  // down by one a pass
  static uint32_t network_counter() { return guest::LoadU32(0x8260185Cu); }
  static void set_network_counter(uint32_t v) { guest::StoreU32(0x8260185Cu, v); }
  static constexpr uint32_t network_counter_address() { return 0x8260185Cu; }
  // the sound manager (AudioManager; menu-sounds.md): slot 4 plays, slot 5 stops
  static uint32_t music_player() { return guest::LoadU32(0x826C5C80u); }
  static void set_music_player(uint32_t v) { guest::StoreU32(0x826C5C80u, v); }
  static constexpr uint32_t music_player_address() { return 0x826C5C80u; }
  // the 18 music names, bgm_tmp_01 to bgm_tmp_17 and BGM_SYSTEM (0x82006A18 BGM_SYSTEM)
  static constexpr uint32_t music_names_address() { return 0x82006A18u; }
  // the half written this pass, 0 or 1
  static uint32_t ring_half() { return guest::LoadU32(0x826A5098u); }
  static void set_ring_half(uint32_t v) { guest::StoreU32(0x826A5098u, v); }
  static constexpr uint32_t ring_half_address() { return 0x826A5098u; }
  // the vertices written into it
  static uint32_t ring_count() { return guest::LoadU32(0x826A509Cu); }
  static void set_ring_count(uint32_t v) { guest::StoreU32(0x826A509Cu, v); }
  static constexpr uint32_t ring_count_address() { return 0x826A509Cu; }
  static uint32_t fade_out_duration_0() { return guest::LoadU32(0x826A50ACu); }
  static void set_fade_out_duration_0(uint32_t v) { guest::StoreU32(0x826A50ACu, v); }
  static constexpr uint32_t fade_out_duration_0_address() { return 0x826A50ACu; }
  static uint32_t fade_out_duration_1() { return guest::LoadU32(0x826A50B0u); }
  static void set_fade_out_duration_1(uint32_t v) { guest::StoreU32(0x826A50B0u, v); }
  static constexpr uint32_t fade_out_duration_1_address() { return 0x826A50B0u; }
  static uint32_t fade_in_duration_0() { return guest::LoadU32(0x826A50B4u); }
  static void set_fade_in_duration_0(uint32_t v) { guest::StoreU32(0x826A50B4u, v); }
  static constexpr uint32_t fade_in_duration_0_address() { return 0x826A50B4u; }
  static uint32_t fade_in_duration_1() { return guest::LoadU32(0x826A50B8u); }
  static void set_fade_in_duration_1(uint32_t v) { guest::StoreU32(0x826A50B8u, v); }
  static constexpr uint32_t fade_in_duration_1_address() { return 0x826A50B8u; }
  // red, green, blue words, layer 0 then 1
  static constexpr uint32_t fade_colors_address() { return 0x826A50BCu; }
  // 0 to 255
  static uint32_t fade_alpha_0() { return guest::LoadU32(0x826A50D4u); }
  static void set_fade_alpha_0(uint32_t v) { guest::StoreU32(0x826A50D4u, v); }
  static constexpr uint32_t fade_alpha_0_address() { return 0x826A50D4u; }
  static uint32_t fade_alpha_1() { return guest::LoadU32(0x826A50D8u); }
  static void set_fade_alpha_1(uint32_t v) { guest::StoreU32(0x826A50D8u, v); }
  static constexpr uint32_t fade_alpha_1_address() { return 0x826A50D8u; }
  // passes left
  static uint32_t fade_in_count_0() { return guest::LoadU32(0x826A50DCu); }
  static void set_fade_in_count_0(uint32_t v) { guest::StoreU32(0x826A50DCu, v); }
  static constexpr uint32_t fade_in_count_0_address() { return 0x826A50DCu; }
  static uint32_t fade_in_count_1() { return guest::LoadU32(0x826A50E0u); }
  static void set_fade_in_count_1(uint32_t v) { guest::StoreU32(0x826A50E0u, v); }
  static constexpr uint32_t fade_in_count_1_address() { return 0x826A50E0u; }
  // passes left
  static uint32_t fade_out_count_0() { return guest::LoadU32(0x826A50E4u); }
  static void set_fade_out_count_0(uint32_t v) { guest::StoreU32(0x826A50E4u, v); }
  static constexpr uint32_t fade_out_count_0_address() { return 0x826A50E4u; }
  static uint32_t fade_out_count_1() { return guest::LoadU32(0x826A50E8u); }
  static void set_fade_out_count_1(uint32_t v) { guest::StoreU32(0x826A50E8u, v); }
  static constexpr uint32_t fade_out_count_1_address() { return 0x826A50E8u; }
  // the layers' sort keys (668, 676)
  static constexpr uint32_t fade_keys_address() { return 0x82001BF4u; }
  // 255.0
  static float fade_scale() { return guest::LoadFloat(0x82000C44u); }
  static void set_fade_scale(float v) { guest::StoreFloat(0x82000C44u, v); }
  static constexpr uint32_t fade_scale_address() { return 0x82000C44u; }
  // 1.0f's bits
  static uint32_t float_one() { return guest::LoadU32(0x82000A30u); }
  static void set_float_one(uint32_t v) { guest::StoreU32(0x82000A30u, v); }
  static constexpr uint32_t float_one_address() { return 0x82000A30u; }
  // 0.0, the color rectangle's queue depth
  static float float_zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_float_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
  // camera pass 0, passed to the color rectangles (not read)
  static constexpr uint32_t fade_camera_pass_address() { return 0x826BAC00u; }
  // -10,000.0
  static float scissor_depth_0() { return guest::LoadFloat(0x82001C00u); }
  static void set_scissor_depth_0(float v) { guest::StoreFloat(0x82001C00u, v); }
  static constexpr uint32_t scissor_depth_0_address() { return 0x82001C00u; }
  // -9,000.0
  static float scissor_depth_1() { return guest::LoadFloat(0x82001BFCu); }
  static void set_scissor_depth_1(float v) { guest::StoreFloat(0x82001BFCu, v); }
  static constexpr uint32_t scissor_depth_1_address() { return 0x82001BFCu; }
  // the color rectangles' render queue
  static uint32_t color_rect_queue() { return guest::LoadU32(0x826D83D0u); }
  static void set_color_rect_queue(uint32_t v) { guest::StoreU32(0x826D83D0u, v); }
  static constexpr uint32_t color_rect_queue_address() { return 0x826D83D0u; }
  // the color rectangles' command list
  static uint32_t color_rect_list() { return guest::LoadU32(0x826D83D4u); }
  static void set_color_rect_list(uint32_t v) { guest::StoreU32(0x826D83D4u, v); }
  static constexpr uint32_t color_rect_list_address() { return 0x826D83D4u; }
  // the app's interface (app + 16): slot 8, then 1 the scene's command list, 2 its render queue
  static uint32_t app_interface() { return guest::LoadU32(0x826BABA0u); }
  static void set_app_interface(uint32_t v) { guest::StoreU32(0x826BABA0u, v); }
  static constexpr uint32_t app_interface_address() { return 0x826BABA0u; }
  // the player's user's name at the last pass (UTF-16)
  static constexpr uint32_t last_name_address() { return 0x826BAB1Cu; }
  // 0 watching; 1 to 9 the prompts' handlers (never ran)
  static uint32_t watch_state() { return guest::LoadU32(0x826BAB3Cu); }
  static void set_watch_state(uint32_t v) { guest::StoreU32(0x826BAB3Cu, v); }
  static constexpr uint32_t watch_state_address() { return 0x826BAB3Cu; }
  // the seven checks a mode turns off (1 = off)
  static constexpr uint32_t checks_address() { return 0x826BAB40u; }
  // another user signed in
  static uint32_t check_other_user() { return guest::LoadU32(0x826BAB40u); }
  static void set_check_other_user(uint32_t v) { guest::StoreU32(0x826BAB40u, v); }
  static constexpr uint32_t check_other_user_address() { return 0x826BAB40u; }
  // the player's user signed out or changed
  static uint32_t check_signed_out() { return guest::LoadU32(0x826BAB44u); }
  static void set_check_signed_out(uint32_t v) { guest::StoreU32(0x826BAB44u, v); }
  static constexpr uint32_t check_signed_out_address() { return 0x826BAB44u; }
  // the connection ([0x82816118])
  static uint32_t check_connection() { return guest::LoadU32(0x826BAB48u); }
  static void set_check_connection(uint32_t v) { guest::StoreU32(0x826BAB48u, v); }
  static constexpr uint32_t check_connection_address() { return 0x826BAB48u; }
  // not read by the watch
  static uint32_t check_12() { return guest::LoadU32(0x826BAB4Cu); }
  static void set_check_12(uint32_t v) { guest::StoreU32(0x826BAB4Cu, v); }
  static constexpr uint32_t check_12_address() { return 0x826BAB4Cu; }
  // the user not on Live
  static uint32_t check_not_on_live() { return guest::LoadU32(0x826BAB50u); }
  static void set_check_not_on_live(uint32_t v) { guest::StoreU32(0x826BAB50u, v); }
  static constexpr uint32_t check_not_on_live_address() { return 0x826BAB50u; }
  // the network session
  static uint32_t check_session() { return guest::LoadU32(0x826BAB54u); }
  static void set_check_session(uint32_t v) { guest::StoreU32(0x826BAB54u, v); }
  static constexpr uint32_t check_session_address() { return 0x826BAB54u; }
  // not read by the watch
  static uint32_t check_24() { return guest::LoadU32(0x826BAB58u); }
  static void set_check_24(uint32_t v) { guest::StoreU32(0x826BAB58u, v); }
  static constexpr uint32_t check_24_address() { return 0x826BAB58u; }
  // 0 another user, 1 signed out, 2 the connection, 4 not on Live, 5 the session
  static uint32_t watch_reason() { return guest::LoadU32(0x826BAB5Cu); }
  static void set_watch_reason(uint32_t v) { guest::StoreU32(0x826BAB5Cu, v); }
  static constexpr uint32_t watch_reason_address() { return 0x826BAB5Cu; }
  // the signed-in count at the last pass
  static uint32_t last_count() { return guest::LoadU32(0x826BAB60u); }
  static void set_last_count(uint32_t v) { guest::StoreU32(0x826BAB60u, v); }
  static constexpr uint32_t last_count_address() { return 0x826BAB60u; }
  // the signed-in mask at the last pass
  static uint32_t last_mask() { return guest::LoadU32(0x826BAB64u); }
  static void set_last_mask(uint32_t v) { guest::StoreU32(0x826BAB64u, v); }
  static constexpr uint32_t last_mask_address() { return 0x826BAB64u; }
  static uint32_t other_user_signed_in() { return guest::LoadU32(0x826BAB68u); }
  static void set_other_user_signed_in(uint32_t v) { guest::StoreU32(0x826BAB68u, v); }
  static constexpr uint32_t other_user_signed_in_address() { return 0x826BAB68u; }
  static uint32_t user_signed_out() { return guest::LoadU32(0x826BAB6Cu); }
  static void set_user_signed_out(uint32_t v) { guest::StoreU32(0x826BAB6Cu, v); }
  static constexpr uint32_t user_signed_out_address() { return 0x826BAB6Cu; }
  // 1 makes the next mode change remake player 0's save data
  static uint32_t remake_save_data() { return guest::LoadU32(0x826BAB70u); }
  static void set_remake_save_data(uint32_t v) { guest::StoreU32(0x826BAB70u, v); }
  static constexpr uint32_t remake_save_data_address() { return 0x826BAB70u; }
  // written by the original Network::Update (probably the Live connection lost)
  static uint32_t connection_lost() { return guest::LoadU32(0x82816118u); }
  static void set_connection_lost(uint32_t v) { guest::StoreU32(0x82816118u, v); }
  static constexpr uint32_t connection_lost_address() { return 0x82816118u; }
  // a network session object present (probably)
  static uint32_t network_active() { return guest::LoadU32(0x826C7824u); }
  static void set_network_active(uint32_t v) { guest::StoreU32(0x826C7824u, v); }
  static constexpr uint32_t network_active_address() { return 0x826C7824u; }
  // the network's vector of 384-byte entries at 0x826C7308: +4 begin
  static uint32_t network_entries_begin() { return guest::LoadU32(0x826C730Cu); }
  static void set_network_entries_begin(uint32_t v) { guest::StoreU32(0x826C730Cu, v); }
  static constexpr uint32_t network_entries_begin_address() { return 0x826C730Cu; }
  // +8 end
  static uint32_t network_entries_end() { return guest::LoadU32(0x826C7310u); }
  static void set_network_entries_end(uint32_t v) { guest::StoreU32(0x826C7310u, v); }
  static constexpr uint32_t network_entries_end_address() { return 0x826C7310u; }
  // read with no session object: 0 counts as a session
  static uint32_t network_idle() { return guest::LoadU32(0x826C7878u); }
  static void set_network_idle(uint32_t v) { guest::StoreU32(0x826C7878u, v); }
  static constexpr uint32_t network_idle_address() { return 0x826C7878u; }
  // the user on each player's controller
  static constexpr uint32_t player_users_address() { return 0x826BABA4u; }
  static uint32_t player0_user() { return guest::LoadU32(0x826BABA4u); }
  static void set_player0_user(uint32_t v) { guest::StoreU32(0x826BABA4u, v); }
  static constexpr uint32_t player0_user_address() { return 0x826BABA4u; }
  // the signed-in users: a 32-byte UTF-16 name at +14 + 32 x slot
  static constexpr uint32_t user_table_address() { return 0x826C7670u; }
  static uint32_t signed_in_mask() { return guest::LoadU32(0x826C7954u); }
  static void set_signed_in_mask(uint32_t v) { guest::StoreU32(0x826C7954u, v); }
  static constexpr uint32_t signed_in_mask_address() { return 0x826C7954u; }
  static uint32_t signed_in_count() { return guest::LoadU32(0x826C7958u); }
  static void set_signed_in_count(uint32_t v) { guest::StoreU32(0x826C7958u, v); }
  static constexpr uint32_t signed_in_count_address() { return 0x826C7958u; }
  // the users on Live (probably)
  static uint32_t live_mask() { return guest::LoadU32(0x826C795Cu); }
  static void set_live_mask(uint32_t v) { guest::StoreU32(0x826C795Cu, v); }
  static constexpr uint32_t live_mask_address() { return 0x826C795Cu; }
  // four 8-byte entries: +4 blocked (action-mappers.md)
  static constexpr uint32_t action_mapper_entries_address() { return 0x826BAB80u; }
  // the race data: +0x59620 the stage list, +0xA3AF0 the human cars' arrangement (6 is two)
  template <typename T = RaceData>
  static T race_data() { return T(guest::LoadU32(0x826C5B2Cu)); }
  static void set_race_data(const View& v) { guest::StoreU32(0x826C5B2Cu, v.address); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the boot movies' first id: 10 the UBI logo
  static uint32_t boot_movie() { return guest::LoadU32(0x826C5BACu); }
  static void set_boot_movie(uint32_t v) { guest::StoreU32(0x826C5BACu, v); }
  static constexpr uint32_t boot_movie_address() { return 0x826C5BACu; }
  // the 1024 x 576 target object: +28 its mode
  template <typename T = SceneTarget>
  static T scene_target() { return T(guest::LoadU32(0x8266D08Cu)); }
  static void set_scene_target(const View& v) { guest::StoreU32(0x8266D08Cu, v.address); }
  static void set_scene_target(uint32_t v) { guest::StoreU32(0x8266D08Cu, v); }
  static constexpr uint32_t scene_target_address() { return 0x8266D08Cu; }
};

}  // namespace schema
