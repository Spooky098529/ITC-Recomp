// The views of docs/functions/menu-helpers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/menu-helpers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Menu, new<68>@Type2Dialog::OnEnter, new<68>@Type12Screen7::Create, new<68>@NetRanking::Create, new<68>@TitleScreen::Create, new<68>@TodaysResult::Create, new<68>@TodaysEvent::Create, new<68>@TuningMachine::Create, new<68>@Type2Hub::Create, new<68>@PowerGraphScreen::Create, new<68>@Type5Dialog::OnEnter, new<68>@Type6Screen0::OnEnter, new<68>@Type7Screen0::OnEnter, new<68>@ControllerOptions::OnEnter, new<68>@UserInfo::Create, new<68>@Type12Screen1::Create#2, new<68>@Type12Screen1::Create#3, new<68>@Type12Screen1::Create#4, TuningScreen.+10044, TuningScreen.+10040
#pragma pack(push, 1)
struct FrontEndMenuData {
  uint32_t cursor = 0;  // +0
  uint32_t previous = 0;  // +4
  uint32_t moved = 0;  // +8
  uint32_t first_item = 0;  // +12
  uint32_t last_item = 0;  // +16
  uint32_t wraps = 0;  // +20
  uint32_t up_down_locked = 0;  // +24
  uint32_t left_right_steps = 0;  // +28
  uint32_t shoulders_step = 0;  // +32
  int32_t side = 0;  // +36
  uint32_t wrapped = 0;  // +40
  uint32_t list_wrapped = 0;  // +44
  uint32_t visible_row = 0;  // +48
  uint32_t previous_row = 0;  // +52
  uint32_t first_row = 0;  // +56
  uint32_t rows_shown = 0;  // +60
  uint32_t player = 0;  // +64
};
#pragma pack(pop)
static_assert(sizeof(FrontEndMenuData) == 68);
inline constexpr memory::LayoutField kFrontEndMenuFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}};
inline constexpr memory::Layout kFrontEndMenuLayout = {kFrontEndMenuFields, 17};

struct FrontEndMenu : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  FrontEndMenuData* data() const { return static_cast<FrontEndMenuData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static FrontEndMenu New() {
    return FrontEndMenu(memory::MakeHandle(new FrontEndMenuData(), sizeof(FrontEndMenuData), "FrontEndMenu", &kFrontEndMenuLayout));
  }
  template <uint32_t Stride = kSize>
  static FrontEndMenu NewArray(uint32_t count) {
    static_assert(sizeof(FrontEndMenuData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrontEndMenuData*>(::operator new(sizeof(FrontEndMenuData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrontEndMenuData();
    return FrontEndMenu(memory::MakeHandle(p, sizeof(FrontEndMenuData) * count, "FrontEndMenu", &kFrontEndMenuLayout, sizeof(FrontEndMenuData)));
  }
  FrontEndMenu at(uint32_t i) const { return FrontEndMenu(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  FrontEndMenuData* direct() const { return static_cast<FrontEndMenuData*>(memory::HandleOwn(address, &kFrontEndMenuLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kFrontEndMenuLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kFrontEndMenuLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t cursor_address() const { return address + 0; }
  uint32_t cursor_bits() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_cursor_bits(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t cursor() const { if (auto* d = direct()) return d->cursor; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_cursor(uint32_t v) const { if (auto* d = direct()) { d->cursor = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t previous_address() const { return address + 4; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t previous() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_previous(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // set when the last Menu::Step moved the cursor
  uint32_t moved_address() const { return address + 8; }
  uint32_t moved_bits() const { if (auto* d = direct()) return d->moved; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_moved_bits(uint32_t v) const { if (auto* d = direct()) { d->moved = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t moved() const { if (auto* d = direct()) return d->moved; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_moved(uint32_t v) const { if (auto* d = direct()) { d->moved = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // 0 in every menu seen
  uint32_t first_item_address() const { return address + 12; }
  uint32_t first_item_bits() const { if (auto* d = direct()) return d->first_item; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_first_item_bits(uint32_t v) const { if (auto* d = direct()) { d->first_item = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t first_item() const { if (auto* d = direct()) return d->first_item; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_first_item(uint32_t v) const { if (auto* d = direct()) { d->first_item = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t last_item_address() const { return address + 16; }
  uint32_t last_item_bits() const { if (auto* d = direct()) return d->last_item; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_last_item_bits(uint32_t v) const { if (auto* d = direct()) { d->last_item = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t last_item() const { if (auto* d = direct()) return d->last_item; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_last_item(uint32_t v) const { if (auto* d = direct()) { d->last_item = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // 1 when the cursor wraps (probably)
  uint32_t wraps_address() const { return address + 20; }
  uint32_t wraps_bits() const { if (auto* d = direct()) return d->wraps; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_wraps_bits(uint32_t v) const { if (auto* d = direct()) { d->wraps = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t wraps() const { if (auto* d = direct()) return d->wraps; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_wraps(uint32_t v) const { if (auto* d = direct()) { d->wraps = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t up_down_locked_address() const { return address + 24; }
  uint32_t up_down_locked_bits() const { if (auto* d = direct()) return d->up_down_locked; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_up_down_locked_bits(uint32_t v) const { if (auto* d = direct()) { d->up_down_locked = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t up_down_locked() const { if (auto* d = direct()) return d->up_down_locked; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_up_down_locked(uint32_t v) const { if (auto* d = direct()) { d->up_down_locked = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t left_right_steps_address() const { return address + 28; }
  uint32_t left_right_steps_bits() const { if (auto* d = direct()) return d->left_right_steps; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_left_right_steps_bits(uint32_t v) const { if (auto* d = direct()) { d->left_right_steps = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t left_right_steps() const { if (auto* d = direct()) return d->left_right_steps; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_left_right_steps(uint32_t v) const { if (auto* d = direct()) { d->left_right_steps = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t shoulders_step_address() const { return address + 32; }
  uint32_t shoulders_step_bits() const { if (auto* d = direct()) return d->shoulders_step; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_shoulders_step_bits(uint32_t v) const { if (auto* d = direct()) { d->shoulders_step = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  uint32_t shoulders_step() const { if (auto* d = direct()) return d->shoulders_step; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_shoulders_step(uint32_t v) const { if (auto* d = direct()) { d->shoulders_step = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  // -1 or 1 when left or right was asked
  uint32_t side_address() const { return address + 36; }
  uint32_t side_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->side); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_side_bits(uint32_t v) const { if (auto* d = direct()) { d->side = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  int32_t side() const { return int32_t(side_bits()); }
  void set_side(int32_t v) const { set_side_bits(uint32_t(v)); }
  // 1 when the last Menu::Step wrapped
  uint32_t wrapped_address() const { return address + 40; }
  uint32_t wrapped_bits() const { if (auto* d = direct()) return d->wrapped; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_wrapped_bits(uint32_t v) const { if (auto* d = direct()) { d->wrapped = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  uint32_t wrapped() const { if (auto* d = direct()) return d->wrapped; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_wrapped(uint32_t v) const { if (auto* d = direct()) { d->wrapped = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  // 1 when the last Menu::StepList wrapped (probably; tuning-pages.md)
  uint32_t list_wrapped_address() const { return address + 44; }
  uint32_t list_wrapped_bits() const { if (auto* d = direct()) return d->list_wrapped; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_list_wrapped_bits(uint32_t v) const { if (auto* d = direct()) { d->list_wrapped = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  uint32_t list_wrapped() const { if (auto* d = direct()) return d->list_wrapped; return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_list_wrapped(uint32_t v) const { if (auto* d = direct()) { d->list_wrapped = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  // the visible row, kept within first_row to rows_shown - 1 (probably)
  uint32_t visible_row_address() const { return address + 48; }
  uint32_t visible_row_bits() const { if (auto* d = direct()) return d->visible_row; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_visible_row_bits(uint32_t v) const { if (auto* d = direct()) { d->visible_row = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t visible_row() const { if (auto* d = direct()) return d->visible_row; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_visible_row(uint32_t v) const { if (auto* d = direct()) { d->visible_row = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // the row before the last up or down step (probably)
  uint32_t previous_row_address() const { return address + 52; }
  uint32_t previous_row_bits() const { if (auto* d = direct()) return d->previous_row; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_previous_row_bits(uint32_t v) const { if (auto* d = direct()) { d->previous_row = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t previous_row() const { if (auto* d = direct()) return d->previous_row; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_previous_row(uint32_t v) const { if (auto* d = direct()) { d->previous_row = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // probably
  uint32_t first_row_address() const { return address + 56; }
  uint32_t first_row_bits() const { if (auto* d = direct()) return d->first_row; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_first_row_bits(uint32_t v) const { if (auto* d = direct()) { d->first_row = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t first_row() const { if (auto* d = direct()) return d->first_row; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_first_row(uint32_t v) const { if (auto* d = direct()) { d->first_row = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // 8 where seen (probably)
  uint32_t rows_shown_address() const { return address + 60; }
  uint32_t rows_shown_bits() const { if (auto* d = direct()) return d->rows_shown; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_rows_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->rows_shown = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t rows_shown() const { if (auto* d = direct()) return d->rows_shown; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_rows_shown(uint32_t v) const { if (auto* d = direct()) { d->rows_shown = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // the player: an index into [0x826BABA4], then the mapper entry at 0x826BAB80
  uint32_t player_address() const { return address + 64; }
  uint32_t player_bits() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_player_bits(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t player() const { if (auto* d = direct()) return d->player; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_player(uint32_t v) const { if (auto* d = direct()) { d->player = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
};

// allocated; measure: new<28>@Type2Dialog::OnEnter, new<28>@Type12Screen7::Create, ScreenPages, new<28>@PauseMenu::Create, new<28>@RaceDialog::Create, new<28>@AfterRaceMenu::Create, new<28>@PauseMenu4::Create, new<28>@Type12Screen6::Create, new<28>@Type12Screen9::Create, new<28>@Type12Dialog11::Create, new<28>@NetRanking::Create, new<28>@TitleScreen::Create, new<28>@TodaysResult::Create, new<28>@TodaysEvent::Create, new<28>@TuningMachine::Create, new<28>@Type2Hub::Create, new<28>@BuildScreen::Create, new<28>@PowerGraphScreen::Create, new<28>@ReplayTheater::Create, new<28>@Type5Dialog::OnEnter, new<28>@Type6Screen0::OnEnter, new<28>@Type7Screen0::OnEnter, new<28>@ControllerOptions::OnEnter, new<28>@UserInfo::Create, ScreenPages::Finish.r3
#pragma pack(push, 1)
struct PageObjectData {
  uint32_t page = 0;  // +0
  uint32_t next_page = 0;  // +4
  uint32_t previous_page = 0;  // +8
  uint32_t step = 0;  // +12
  uint32_t queued_step = 0;  // +16
  uint32_t unknown_20 = 0;  // +20
  uint32_t step_started = 0;  // +24
};
#pragma pack(pop)
static_assert(sizeof(PageObjectData) == 28);
inline constexpr memory::LayoutField kPageObjectFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}};
inline constexpr memory::Layout kPageObjectLayout = {kPageObjectFields, 7};

struct PageObject : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  PageObjectData* data() const { return static_cast<PageObjectData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static PageObject New() {
    return PageObject(memory::MakeHandle(new PageObjectData(), sizeof(PageObjectData), "PageObject", &kPageObjectLayout));
  }
  template <uint32_t Stride = kSize>
  static PageObject NewArray(uint32_t count) {
    static_assert(sizeof(PageObjectData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PageObjectData*>(::operator new(sizeof(PageObjectData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PageObjectData();
    return PageObject(memory::MakeHandle(p, sizeof(PageObjectData) * count, "PageObject", &kPageObjectLayout, sizeof(PageObjectData)));
  }
  PageObject at(uint32_t i) const { return PageObject(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PageObjectData* direct() const { return static_cast<PageObjectData*>(memory::HandleOwn(address, &kPageObjectLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPageObjectLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPageObjectLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the page Draw runs; -1 while a step runs
  uint32_t page_address() const { return address + 0; }
  uint32_t page_bits() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_page_bits(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t page() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_page(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the page after the step
  uint32_t next_page_address() const { return address + 4; }
  uint32_t next_page_bits() const { if (auto* d = direct()) return d->next_page; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_next_page_bits(uint32_t v) const { if (auto* d = direct()) { d->next_page = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t next_page() const { if (auto* d = direct()) return d->next_page; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_next_page(uint32_t v) const { if (auto* d = direct()) { d->next_page = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t previous_page_address() const { return address + 8; }
  uint32_t previous_page_bits() const { if (auto* d = direct()) return d->previous_page; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_previous_page_bits(uint32_t v) const { if (auto* d = direct()) { d->previous_page = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_page() const { if (auto* d = direct()) return d->previous_page; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_previous_page(uint32_t v) const { if (auto* d = direct()) { d->previous_page = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // the step Update runs; -1 none
  uint32_t step_address() const { return address + 12; }
  uint32_t step_bits() const { if (auto* d = direct()) return d->step; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_step_bits(uint32_t v) const { if (auto* d = direct()) { d->step = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t step() const { if (auto* d = direct()) return d->step; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_step(uint32_t v) const { if (auto* d = direct()) { d->step = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the step after it
  uint32_t queued_step_address() const { return address + 16; }
  uint32_t queued_step_bits() const { if (auto* d = direct()) return d->queued_step; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_queued_step_bits(uint32_t v) const { if (auto* d = direct()) { d->queued_step = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t queued_step() const { if (auto* d = direct()) return d->queued_step; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_queued_step(uint32_t v) const { if (auto* d = direct()) { d->queued_step = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_20_bits() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_unknown_20_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t unknown_20() const { if (auto* d = direct()) return d->unknown_20; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_unknown_20(uint32_t v) const { if (auto* d = direct()) { d->unknown_20 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the step's started word
  uint32_t step_started_address() const { return address + 24; }
  uint32_t step_started_bits() const { if (auto* d = direct()) return d->step_started; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_step_started_bits(uint32_t v) const { if (auto* d = direct()) { d->step_started = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t step_started() const { if (auto* d = direct()) return d->step_started; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_step_started(uint32_t v) const { if (auto* d = direct()) { d->step_started = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
};

}  // namespace schema
