// The views of docs/functions/options-load-save.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/options-load-save.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<28>@Options::OnEnter#2, new<28>@Options::OnEnter#4
#pragma pack(push, 1)
struct OptionsSubPageBlockData {
  uint32_t page = 0;  // +0
  uint32_t next_page = 0;  // +4
  uint32_t previous_page = 0;  // +8
  uint32_t step = 0;  // +12
  uint32_t queued_step = 0;  // +16
  uint32_t unknown_20 = 0;  // +20
  uint32_t step_started = 0;  // +24
};
#pragma pack(pop)
static_assert(sizeof(OptionsSubPageBlockData) == 28);
inline constexpr memory::LayoutField kOptionsSubPageBlockFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}};
inline constexpr memory::Layout kOptionsSubPageBlockLayout = {kOptionsSubPageBlockFields, 7};

struct OptionsSubPageBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  OptionsSubPageBlockData* data() const { return static_cast<OptionsSubPageBlockData*>(memory::HandleObject(address)); }
  static OptionsSubPageBlock New() {
    return OptionsSubPageBlock(memory::MakeHandle(new OptionsSubPageBlockData(), sizeof(OptionsSubPageBlockData), "OptionsSubPageBlock", &kOptionsSubPageBlockLayout));
  }
  template <uint32_t Stride = kSize>
  static OptionsSubPageBlock NewArray(uint32_t count) {
    static_assert(sizeof(OptionsSubPageBlockData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<OptionsSubPageBlockData*>(::operator new(sizeof(OptionsSubPageBlockData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) OptionsSubPageBlockData();
    return OptionsSubPageBlock(memory::MakeHandle(p, sizeof(OptionsSubPageBlockData) * count, "OptionsSubPageBlock", &kOptionsSubPageBlockLayout, sizeof(OptionsSubPageBlockData)));
  }
  OptionsSubPageBlock at(uint32_t i) const { return OptionsSubPageBlock(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  OptionsSubPageBlockData* direct() const { return static_cast<OptionsSubPageBlockData*>(memory::HandleOwn(address, &kOptionsSubPageBlockLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kOptionsSubPageBlockLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOptionsSubPageBlockLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the sub-page: 0 the device check, 1 the question, 2 a note, 3 the wait, 4 the close, 5 the cancel's, 6 the device's answer (load only)
  uint32_t page_address() const { return address + 0; }
  uint32_t page_bits() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_page_bits(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t page() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_page(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the page after the step (the options' page block layout)
  uint32_t next_page_address() const { return address + 4; }
  uint32_t next_page_bits() const { if (auto* d = direct()) return d->next_page; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_next_page_bits(uint32_t v) const { if (auto* d = direct()) { d->next_page = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t next_page() const { if (auto* d = direct()) return d->next_page; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_next_page(uint32_t v) const { if (auto* d = direct()) { d->next_page = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the sub-page before, set unless the same
  uint32_t previous_page_address() const { return address + 8; }
  uint32_t previous_page_bits() const { if (auto* d = direct()) return d->previous_page; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_previous_page_bits(uint32_t v) const { if (auto* d = direct()) { d->previous_page = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t previous_page() const { if (auto* d = direct()) return d->previous_page; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_previous_page(uint32_t v) const { if (auto* d = direct()) { d->previous_page = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // -1 here
  uint32_t step_address() const { return address + 12; }
  uint32_t step_bits() const { if (auto* d = direct()) return d->step; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_step_bits(uint32_t v) const { if (auto* d = direct()) { d->step = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t step() const { if (auto* d = direct()) return d->step; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_step(uint32_t v) const { if (auto* d = direct()) { d->step = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // -1 here
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

// embedded; measure: LoadPage, SavePage
struct OptionsLeaveStarted : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 on the leave step's first pass, then 1
  uint32_t started() const { return guest::LoadU32(address + 0); }
  void set_started(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t started_address() const { return address + 0; }
};

// allocated; measure: new<129400>@SaveBuffers::Create
struct OptionsLoadedGameData : View {
  using View::View;
  static constexpr uint32_t kSize = 129400;
  static OptionsLoadedGameData New(uint32_t size) { return OptionsLoadedGameData(memory::NewBlob(size, "OptionsLoadedGameData")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  // the garage list's count; 0 is kind 12's note (probably not the game's data)
  uint16_t garage_count() const { return guest::LoadU16(address + 6328); }
  void set_garage_count(uint16_t v) const { guest::StoreU16(address + 6328, v); }
  uint32_t garage_count_address() const { return address + 6328; }
};

// The image words docs/functions/options-load-save.md's code uses.
struct OptionsLoadSaveGlobals {
  // page 9's (the load's) sub-page block
  template <typename T = OptionsSubPageBlock>
  static T load_pages() { return T(guest::LoadU32(0x826D847Cu)); }
  static void set_load_pages(const View& v) { guest::StoreU32(0x826D847Cu, v.address); }
  static void set_load_pages(uint32_t v) { guest::StoreU32(0x826D847Cu, v); }
  static constexpr uint32_t load_pages_address() { return 0x826D847Cu; }
  // page 10's (the save's) sub-page block
  template <typename T = OptionsSubPageBlock>
  static T save_pages() { return T(guest::LoadU32(0x826D8488u)); }
  static void set_save_pages(const View& v) { guest::StoreU32(0x826D8488u, v.address); }
  static void set_save_pages(uint32_t v) { guest::StoreU32(0x826D8488u, v); }
  static constexpr uint32_t save_pages_address() { return 0x826D8488u; }
};

}  // namespace schema
