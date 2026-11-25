// The views of docs/functions/race-overlays.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-overlays.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceOverlays
struct RaceOverlays : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // overlay 0: loads the texture set the others share
  template <typename T = CarOverlays>
  T overlay_0() const { return T(guest::LoadU32(address + 0)); }
  void set_overlay_0(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_overlay_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t overlay_0_address() const { return address + 0; }
  // overlays 1 to 24 (pointers; in races 1 to 6 are CarOverlays, the rest 0)
  uint32_t others_address() const { return address + 4; }
};

// image; measure: CarOverlays.+0, sub_8215E6C0.r3@+9608.+8.+0
struct OverlayVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // slot 0 (r4 bit 0: free)
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // slot 3
  uint32_t reset() const { return guest::LoadU32(address + 12); }
  void set_reset(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t reset_address() const { return address + 12; }
  // slot 4, once a tick
  uint32_t update() const { return guest::LoadU32(address + 16); }
  void set_update(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t update_address() const { return address + 16; }
  // slot 5 (the car overlays': each child's slot 7)
  uint32_t record() const { return guest::LoadU32(address + 20); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t record_address() const { return address + 20; }
  // slot 6: r4 a set-up (CarOverlays: 28 bytes; the children: 12), r5 a capacity
  uint32_t init() const { return guest::LoadU32(address + 24); }
  void set_init(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t init_address() const { return address + 24; }
  // slot 7: a child's recording, r4 the camera pass index (sprites.md)
  uint32_t record_child() const { return guest::LoadU32(address + 28); }
  void set_record_child(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t record_child_address() const { return address + 28; }
};

// image; measure: CarOverlays.+4.+0
struct OverlayViewVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // slot 0
  uint32_t deleting_destructor() const { return guest::LoadU32(address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
};

// allocated; measure: CarOverlays, sub_8215E6C0.r3@+9608.+8
#pragma pack(push, 1)
struct CarOverlaysData {
  uint32_t vtable = 0;  // +0
  uint8_t views[100] = {};  // +4, guest order
  uint32_t textures = 0;  // +104
  uint32_t texture_set = 0;  // +108
  uint32_t loaded = 0;  // +112
  uint32_t viewport_entry = 0;  // +116
  uint32_t second_index = 0;  // +120
  uint32_t unknown_124 = 0;  // +124
  uint32_t view_slot = 0;  // +128
  uint32_t race_word_4 = 0;  // +132
  uint32_t race_word_28 = 0;  // +136
  uint32_t race_word_32 = 0;  // +140
  uint32_t params = 0;  // +144
  uint8_t trails[16] = {};  // +148, guest order
  uint8_t smokes[16] = {};  // +164, guest order
  uint8_t unknown_180[20] = {};  // +180, guest order
  uint8_t light_sprites[16] = {};  // +200, guest order
  uint8_t unknown_216[56] = {};  // +216, guest order
  uint8_t unknown_272[56] = {};  // +272, guest order
  uint8_t sparks[32] = {};  // +328, guest order
  uint8_t particles_b[16] = {};  // +360, guest order
  uint32_t marker = 0;  // +376
  uint8_t particles_c[16] = {};  // +380, guest order
  uint8_t model_sprites[36] = {};  // +396, guest order
  uint8_t wheels[48] = {};  // +432, 4 x CarOverlaysWheel
  uint8_t unknown_480[48] = {};  // +480, guest order
  uint8_t smoke_rates[192] = {};  // +528, guest order
  uint32_t vehicle_word_2252 = 0;  // +720
  uint8_t pad_724[24] = {};
  float unknown_748 = 0;  // +748
  uint8_t unknown_752[12] = {};  // +752, guest order
  float unknown_764 = 0;  // +764
  float color_r = 0;  // +768
  float color_g = 0;  // +772
  float color_b = 0;  // +776
  float color_a = 0;  // +780
  uint32_t view_mode = 0;  // +784
  uint32_t unknown_788 = 0;  // +788
  uint8_t pad_792[4] = {};
  uint32_t viewport_record = 0;  // +796
  uint32_t camera_pass = 0;  // +800
  uint8_t viewport_pair[8] = {};  // +804, guest order
  uint32_t sparks_updating = 0;  // +812
  uint32_t marker_shown = 0;  // +816
  uint32_t unknown_820 = 0;  // +820
  uint32_t unknown_824 = 0;  // +824
  uint32_t unknown_828 = 0;  // +828
  uint32_t last_vehicle = 0;  // +832
  int32_t racer_id = 0;  // +836
  uint8_t pad_840[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarOverlaysData) == 848);
inline constexpr memory::LayoutField kCarOverlaysFields[] = {{0, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {376, 4}, {432, 4}, {436, 4}, {440, 4}, {444, 4}, {448, 4}, {452, 4}, {456, 4}, {460, 4}, {464, 4}, {468, 4}, {472, 4}, {476, 4}, {720, 4}, {748, 4}, {764, 4}, {768, 4}, {772, 4}, {776, 4}, {780, 4}, {784, 4}, {788, 4}, {796, 4}, {800, 4}, {812, 4}, {816, 4}, {820, 4}, {824, 4}, {828, 4}, {832, 4}, {836, 4}};
inline constexpr memory::Layout kCarOverlaysLayout = {kCarOverlaysFields, 43};

struct CarOverlays : View {
  using View::View;
  static constexpr uint32_t kSize = 848;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82008524;
  CarOverlaysData* data() const { return static_cast<CarOverlaysData*>(memory::HandleObject(address)); }
  static CarOverlays New() {
    return CarOverlays(memory::MakeHandle(new CarOverlaysData(), sizeof(CarOverlaysData), "CarOverlays", &kCarOverlaysLayout));
  }
  template <uint32_t Stride = kSize>
  static CarOverlays NewArray(uint32_t count) {
    static_assert(sizeof(CarOverlaysData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarOverlaysData*>(::operator new(sizeof(CarOverlaysData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarOverlaysData();
    return CarOverlays(memory::MakeHandle(p, sizeof(CarOverlaysData) * count, "CarOverlays", &kCarOverlaysLayout, sizeof(CarOverlaysData)));
  }
  CarOverlays at(uint32_t i) const { return CarOverlays(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarOverlaysData* direct() const { return static_cast<CarOverlaysData*>(memory::HandleOwn(address, &kCarOverlaysLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarOverlaysLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarOverlaysLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82008524 (OverlayVtable)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // 25 OverlayView pointers, one a view slot (0 none)
  uint32_t views_address() const { return address + 4; }
  uint8_t* views() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4 : guest::Pointer(address + 4); }
  // the texture table (42 textures, loaded by overlay 0 and shared)
  uint32_t textures_address() const { return address + 104; }
  uint32_t textures_bits() const { if (auto* d = direct()) return d->textures; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_textures_bits(uint32_t v) const { if (auto* d = direct()) { d->textures = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t textures() const { if (auto* d = direct()) return d->textures; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_textures(uint32_t v) const { if (auto* d = direct()) { d->textures = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t texture_set_address() const { return address + 108; }
  uint32_t texture_set_bits() const { if (auto* d = direct()) return d->texture_set; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_texture_set_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_set = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t texture_set() const { if (auto* d = direct()) return d->texture_set; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_texture_set(uint32_t v) const { if (auto* d = direct()) { d->texture_set = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t loaded_address() const { return address + 112; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  // an index into the 12-byte entries at 0x82008558 (the set-up's +0)
  uint32_t viewport_entry_address() const { return address + 116; }
  uint32_t viewport_entry_bits() const { if (auto* d = direct()) return d->viewport_entry; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_viewport_entry_bits(uint32_t v) const { if (auto* d = direct()) { d->viewport_entry = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t viewport_entry() const { if (auto* d = direct()) return d->viewport_entry; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_viewport_entry(uint32_t v) const { if (auto* d = direct()) { d->viewport_entry = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  // an index into the pairs at 0x820085A0 (the set-up's +4)
  uint32_t second_index_address() const { return address + 120; }
  uint32_t second_index_bits() const { if (auto* d = direct()) return d->second_index; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_second_index_bits(uint32_t v) const { if (auto* d = direct()) { d->second_index = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t second_index() const { if (auto* d = direct()) return d->second_index; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_second_index(uint32_t v) const { if (auto* d = direct()) { d->second_index = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  uint32_t unknown_124_bits() const { if (auto* d = direct()) return d->unknown_124; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_unknown_124_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_124 = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  uint32_t unknown_124() const { if (auto* d = direct()) return d->unknown_124; return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_unknown_124(uint32_t v) const { if (auto* d = direct()) { d->unknown_124 = v; return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  // the slot of the car's own view
  uint32_t view_slot_address() const { return address + 128; }
  uint32_t view_slot_bits() const { if (auto* d = direct()) return d->view_slot; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_view_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->view_slot = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  uint32_t view_slot() const { if (auto* d = direct()) return d->view_slot; return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_view_slot(uint32_t v) const { if (auto* d = direct()) { d->view_slot = v; return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  // [0x826C5B2C] + 4 at the set-up
  uint32_t race_word_4_address() const { return address + 132; }
  uint32_t race_word_4_bits() const { if (auto* d = direct()) return d->race_word_4; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_race_word_4_bits(uint32_t v) const { if (auto* d = direct()) { d->race_word_4 = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  uint32_t race_word_4() const { if (auto* d = direct()) return d->race_word_4; return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_race_word_4(uint32_t v) const { if (auto* d = direct()) { d->race_word_4 = v; return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  // [0x826C5B2C] + 28 at the set-up; the effects update with 0 or 3
  uint32_t race_word_28_address() const { return address + 136; }
  uint32_t race_word_28_bits() const { if (auto* d = direct()) return d->race_word_28; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_race_word_28_bits(uint32_t v) const { if (auto* d = direct()) { d->race_word_28 = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  uint32_t race_word_28() const { if (auto* d = direct()) return d->race_word_28; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_race_word_28(uint32_t v) const { if (auto* d = direct()) { d->race_word_28 = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  // [0x826C5B2C] + 32 at the set-up
  uint32_t race_word_32_address() const { return address + 140; }
  uint32_t race_word_32_bits() const { if (auto* d = direct()) return d->race_word_32; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_race_word_32_bits(uint32_t v) const { if (auto* d = direct()) { d->race_word_32 = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t race_word_32() const { if (auto* d = direct()) return d->race_word_32; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_race_word_32(uint32_t v) const { if (auto* d = direct()) { d->race_word_32 = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  // the effects' parameter table 0x82616F50
  uint32_t params_address() const { return address + 144; }
  uint32_t params_bits() const { if (auto* d = direct()) return d->params; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_params_bits(uint32_t v) const { if (auto* d = direct()) { d->params = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  template <typename T = CarOverlaysParams>
  T params() const { return T(params_bits()); }
  void set_params(const View& w) const { set_params_bits(w.address); }
  void set_params(uint32_t v) const { set_params_bits(v); }
  // 4 Trail pointers, one a wheel
  uint32_t trails_address() const { return address + 148; }
  uint8_t* trails() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 148 : guest::Pointer(address + 148); }
  // 4 Smoke pointers, one a wheel
  uint32_t smokes_address() const { return address + 164; }
  uint8_t* smokes() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 164 : guest::Pointer(address + 164); }
  // five words cleared by CreateChildren
  uint32_t unknown_180_address() const { return address + 180; }
  uint8_t* unknown_180() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 180 : guest::Pointer(address + 180); }
  // 4 LightSprite pointers (model points 4-7)
  uint32_t light_sprites_address() const { return address + 200; }
  uint8_t* light_sprites() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 200 : guest::Pointer(address + 200); }
  // 14 words cleared by CreateChildren
  uint32_t unknown_216_address() const { return address + 216; }
  uint8_t* unknown_216() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 216 : guest::Pointer(address + 216); }
  // 14 words cleared by CreateChildren
  uint32_t unknown_272_address() const { return address + 272; }
  uint8_t* unknown_272() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 272 : guest::Pointer(address + 272); }
  // 8 Sparks pointers
  uint32_t sparks_address() const { return address + 328; }
  uint8_t* sparks() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 328 : guest::Pointer(address + 328); }
  // 4 ParticlesB pointers (model points 4-7)
  uint32_t particles_b_address() const { return address + 360; }
  uint8_t* particles_b() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 360 : guest::Pointer(address + 360); }
  // the marker above the car
  uint32_t marker_address() const { return address + 376; }
  uint32_t marker_bits() const { if (auto* d = direct()) return d->marker; return uint32_t((memory::IsHandle(address + 376) ? uint32_t(memory::OwnerLoad(address + 376, 4)) : guest::LoadU32(address + 376))); }
  void set_marker_bits(uint32_t v) const { if (auto* d = direct()) { d->marker = v; return; } if (memory::IsHandle(address + 376)) memory::OwnerStore(address + 376, 4, v); else guest::StoreU32(address + 376, v); }
  template <typename T = Marker>
  T marker() const { return T(marker_bits()); }
  void set_marker(const View& w) const { set_marker_bits(w.address); }
  void set_marker(uint32_t v) const { set_marker_bits(v); }
  // 4 ParticlesC pointers (model points 14-17; probably the exhausts)
  uint32_t particles_c_address() const { return address + 380; }
  uint8_t* particles_c() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 380 : guest::Pointer(address + 380); }
  // 9 ModelSprite pointers
  uint32_t model_sprites_address() const { return address + 396; }
  uint8_t* model_sprites() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 396 : guest::Pointer(address + 396); }
  // 12 bytes a wheel
  template <typename T = CarOverlaysWheel>
  T wheels(uint32_t i) const { return T(address + 432 + i * T::kSize); }
  // cleared by the reset (with the rates after it)
  uint32_t unknown_480_address() const { return address + 480; }
  uint8_t* unknown_480() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 480 : guest::Pointer(address + 480); }
  // +48 x wheel: the wheel's smoke rate accumulator (f32), the rest of each 48 bytes cleared by the reset
  uint32_t smoke_rates_address() const { return address + 528; }
  uint8_t* smoke_rates() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 528 : guest::Pointer(address + 528); }
  // the vehicle's word +2252
  uint32_t vehicle_word_2252_address() const { return address + 720; }
  uint32_t vehicle_word_2252_bits() const { if (auto* d = direct()) return d->vehicle_word_2252; return uint32_t((memory::IsHandle(address + 720) ? uint32_t(memory::OwnerLoad(address + 720, 4)) : guest::LoadU32(address + 720))); }
  void set_vehicle_word_2252_bits(uint32_t v) const { if (auto* d = direct()) { d->vehicle_word_2252 = v; return; } if (memory::IsHandle(address + 720)) memory::OwnerStore(address + 720, 4, v); else guest::StoreU32(address + 720, v); }
  uint32_t vehicle_word_2252() const { if (auto* d = direct()) return d->vehicle_word_2252; return uint32_t((memory::IsHandle(address + 720) ? uint32_t(memory::OwnerLoad(address + 720, 4)) : guest::LoadU32(address + 720))); }
  void set_vehicle_word_2252(uint32_t v) const { if (auto* d = direct()) { d->vehicle_word_2252 = v; return; } if (memory::IsHandle(address + 720)) memory::OwnerStore(address + 720, 4, v); else guest::StoreU32(address + 720, v); }
  // 1.0 at construction
  uint32_t unknown_748_address() const { return address + 748; }
  uint32_t unknown_748_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_748); return uint32_t((memory::IsHandle(address + 748) ? uint32_t(memory::OwnerLoad(address + 748, 4)) : guest::LoadU32(address + 748))); }
  void set_unknown_748_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_748 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 748)) memory::OwnerStore(address + 748, 4, v); else guest::StoreU32(address + 748, v); }
  float unknown_748() const { return std::bit_cast<float>(unknown_748_bits()); }
  void set_unknown_748(float v) const { set_unknown_748_bits(std::bit_cast<uint32_t>(v)); }
  // probably a vector, 1.0 at construction
  uint32_t unknown_752_address() const { return address + 752; }
  uint8_t* unknown_752() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 752 : guest::Pointer(address + 752); }
  // 1.0 at construction
  uint32_t unknown_764_address() const { return address + 764; }
  uint32_t unknown_764_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_764); return uint32_t((memory::IsHandle(address + 764) ? uint32_t(memory::OwnerLoad(address + 764, 4)) : guest::LoadU32(address + 764))); }
  void set_unknown_764_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_764 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 764)) memory::OwnerStore(address + 764, 4, v); else guest::StoreU32(address + 764, v); }
  float unknown_764() const { return std::bit_cast<float>(unknown_764_bits()); }
  void set_unknown_764(float v) const { set_unknown_764_bits(std::bit_cast<uint32_t>(v)); }
  // 0.0 at construction and reset
  uint32_t color_r_address() const { return address + 768; }
  uint32_t color_r_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_r); return uint32_t((memory::IsHandle(address + 768) ? uint32_t(memory::OwnerLoad(address + 768, 4)) : guest::LoadU32(address + 768))); }
  void set_color_r_bits(uint32_t v) const { if (auto* d = direct()) { d->color_r = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 768)) memory::OwnerStore(address + 768, 4, v); else guest::StoreU32(address + 768, v); }
  float color_r() const { return std::bit_cast<float>(color_r_bits()); }
  void set_color_r(float v) const { set_color_r_bits(std::bit_cast<uint32_t>(v)); }
  // 0.0 at construction and reset
  uint32_t color_g_address() const { return address + 772; }
  uint32_t color_g_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_g); return uint32_t((memory::IsHandle(address + 772) ? uint32_t(memory::OwnerLoad(address + 772, 4)) : guest::LoadU32(address + 772))); }
  void set_color_g_bits(uint32_t v) const { if (auto* d = direct()) { d->color_g = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 772)) memory::OwnerStore(address + 772, 4, v); else guest::StoreU32(address + 772, v); }
  float color_g() const { return std::bit_cast<float>(color_g_bits()); }
  void set_color_g(float v) const { set_color_g_bits(std::bit_cast<uint32_t>(v)); }
  // 0.0 at construction and reset
  uint32_t color_b_address() const { return address + 776; }
  uint32_t color_b_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_b); return uint32_t((memory::IsHandle(address + 776) ? uint32_t(memory::OwnerLoad(address + 776, 4)) : guest::LoadU32(address + 776))); }
  void set_color_b_bits(uint32_t v) const { if (auto* d = direct()) { d->color_b = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 776)) memory::OwnerStore(address + 776, 4, v); else guest::StoreU32(address + 776, v); }
  float color_b() const { return std::bit_cast<float>(color_b_bits()); }
  void set_color_b(float v) const { set_color_b_bits(std::bit_cast<uint32_t>(v)); }
  // 1.0 at construction and reset
  uint32_t color_a_address() const { return address + 780; }
  uint32_t color_a_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_a); return uint32_t((memory::IsHandle(address + 780) ? uint32_t(memory::OwnerLoad(address + 780, 4)) : guest::LoadU32(address + 780))); }
  void set_color_a_bits(uint32_t v) const { if (auto* d = direct()) { d->color_a = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 780)) memory::OwnerStore(address + 780, 4, v); else guest::StoreU32(address + 780, v); }
  float color_a() const { return std::bit_cast<float>(color_a_bits()); }
  void set_color_a(float v) const { set_color_a_bits(std::bit_cast<uint32_t>(v)); }
  // the viewport's +1860: 1 and 2 turn some effects off
  uint32_t view_mode_address() const { return address + 784; }
  uint32_t view_mode_bits() const { if (auto* d = direct()) return d->view_mode; return uint32_t((memory::IsHandle(address + 784) ? uint32_t(memory::OwnerLoad(address + 784, 4)) : guest::LoadU32(address + 784))); }
  void set_view_mode_bits(uint32_t v) const { if (auto* d = direct()) { d->view_mode = v; return; } if (memory::IsHandle(address + 784)) memory::OwnerStore(address + 784, 4, v); else guest::StoreU32(address + 784, v); }
  uint32_t view_mode() const { if (auto* d = direct()) return d->view_mode; return uint32_t((memory::IsHandle(address + 784) ? uint32_t(memory::OwnerLoad(address + 784, 4)) : guest::LoadU32(address + 784))); }
  void set_view_mode(uint32_t v) const { if (auto* d = direct()) { d->view_mode = v; return; } if (memory::IsHandle(address + 784)) memory::OwnerStore(address + 784, 4, v); else guest::StoreU32(address + 784, v); }
  // 6 at construction and reset
  uint32_t unknown_788_address() const { return address + 788; }
  uint32_t unknown_788_bits() const { if (auto* d = direct()) return d->unknown_788; return uint32_t((memory::IsHandle(address + 788) ? uint32_t(memory::OwnerLoad(address + 788, 4)) : guest::LoadU32(address + 788))); }
  void set_unknown_788_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_788 = v; return; } if (memory::IsHandle(address + 788)) memory::OwnerStore(address + 788, 4, v); else guest::StoreU32(address + 788, v); }
  uint32_t unknown_788() const { if (auto* d = direct()) return d->unknown_788; return uint32_t((memory::IsHandle(address + 788) ? uint32_t(memory::OwnerLoad(address + 788, 4)) : guest::LoadU32(address + 788))); }
  void set_unknown_788(uint32_t v) const { if (auto* d = direct()) { d->unknown_788 = v; return; } if (memory::IsHandle(address + 788)) memory::OwnerStore(address + 788, 4, v); else guest::StoreU32(address + 788, v); }
  // the bound viewport's camera entry (CarOverlays::BindViewport's out +0)
  uint32_t viewport_record_address() const { return address + 796; }
  uint32_t viewport_record_bits() const { if (auto* d = direct()) return d->viewport_record; return uint32_t((memory::IsHandle(address + 796) ? uint32_t(memory::OwnerLoad(address + 796, 4)) : guest::LoadU32(address + 796))); }
  void set_viewport_record_bits(uint32_t v) const { if (auto* d = direct()) { d->viewport_record = v; return; } if (memory::IsHandle(address + 796)) memory::OwnerStore(address + 796, 4, v); else guest::StoreU32(address + 796, v); }
  uint32_t viewport_record() const { if (auto* d = direct()) return d->viewport_record; return uint32_t((memory::IsHandle(address + 796) ? uint32_t(memory::OwnerLoad(address + 796, 4)) : guest::LoadU32(address + 796))); }
  void set_viewport_record(uint32_t v) const { if (auto* d = direct()) { d->viewport_record = v; return; } if (memory::IsHandle(address + 796)) memory::OwnerStore(address + 796, 4, v); else guest::StoreU32(address + 796, v); }
  // the bound camera pass (its eye at +400)
  uint32_t camera_pass_address() const { return address + 800; }
  uint32_t camera_pass_bits() const { if (auto* d = direct()) return d->camera_pass; return uint32_t((memory::IsHandle(address + 800) ? uint32_t(memory::OwnerLoad(address + 800, 4)) : guest::LoadU32(address + 800))); }
  void set_camera_pass_bits(uint32_t v) const { if (auto* d = direct()) { d->camera_pass = v; return; } if (memory::IsHandle(address + 800)) memory::OwnerStore(address + 800, 4, v); else guest::StoreU32(address + 800, v); }
  uint32_t camera_pass() const { if (auto* d = direct()) return d->camera_pass; return uint32_t((memory::IsHandle(address + 800) ? uint32_t(memory::OwnerLoad(address + 800, 4)) : guest::LoadU32(address + 800))); }
  void set_camera_pass(uint32_t v) const { if (auto* d = direct()) { d->camera_pass = v; return; } if (memory::IsHandle(address + 800)) memory::OwnerStore(address + 800, 4, v); else guest::StoreU32(address + 800, v); }
  // the two words at 0x820085A0 + 8 x the second index
  uint32_t viewport_pair_address() const { return address + 804; }
  uint8_t* viewport_pair() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 804 : guest::Pointer(address + 804); }
  // 1 during UpdateSparks (0 again in view modes 1 and 2)
  uint32_t sparks_updating_address() const { return address + 812; }
  uint32_t sparks_updating_bits() const { if (auto* d = direct()) return d->sparks_updating; return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(address + 812))); }
  void set_sparks_updating_bits(uint32_t v) const { if (auto* d = direct()) { d->sparks_updating = v; return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(address + 812, v); }
  uint32_t sparks_updating() const { if (auto* d = direct()) return d->sparks_updating; return uint32_t((memory::IsHandle(address + 812) ? uint32_t(memory::OwnerLoad(address + 812, 4)) : guest::LoadU32(address + 812))); }
  void set_sparks_updating(uint32_t v) const { if (auto* d = direct()) { d->sparks_updating = v; return; } if (memory::IsHandle(address + 812)) memory::OwnerStore(address + 812, 4, v); else guest::StoreU32(address + 812, v); }
  uint32_t marker_shown_address() const { return address + 816; }
  uint32_t marker_shown_bits() const { if (auto* d = direct()) return d->marker_shown; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(address + 816))); }
  void set_marker_shown_bits(uint32_t v) const { if (auto* d = direct()) { d->marker_shown = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(address + 816, v); }
  uint32_t marker_shown() const { if (auto* d = direct()) return d->marker_shown; return uint32_t((memory::IsHandle(address + 816) ? uint32_t(memory::OwnerLoad(address + 816, 4)) : guest::LoadU32(address + 816))); }
  void set_marker_shown(uint32_t v) const { if (auto* d = direct()) { d->marker_shown = v; return; } if (memory::IsHandle(address + 816)) memory::OwnerStore(address + 816, 4, v); else guest::StoreU32(address + 816, v); }
  // 0 at the set-up; read by the marker's and model sprites' kinds
  uint32_t unknown_820_address() const { return address + 820; }
  uint32_t unknown_820_bits() const { if (auto* d = direct()) return d->unknown_820; return uint32_t((memory::IsHandle(address + 820) ? uint32_t(memory::OwnerLoad(address + 820, 4)) : guest::LoadU32(address + 820))); }
  void set_unknown_820_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_820 = v; return; } if (memory::IsHandle(address + 820)) memory::OwnerStore(address + 820, 4, v); else guest::StoreU32(address + 820, v); }
  uint32_t unknown_820() const { if (auto* d = direct()) return d->unknown_820; return uint32_t((memory::IsHandle(address + 820) ? uint32_t(memory::OwnerLoad(address + 820, 4)) : guest::LoadU32(address + 820))); }
  void set_unknown_820(uint32_t v) const { if (auto* d = direct()) { d->unknown_820 = v; return; } if (memory::IsHandle(address + 820)) memory::OwnerStore(address + 820, 4, v); else guest::StoreU32(address + 820, v); }
  // 0 at the set-up; read by the marker's kind
  uint32_t unknown_824_address() const { return address + 824; }
  uint32_t unknown_824_bits() const { if (auto* d = direct()) return d->unknown_824; return uint32_t((memory::IsHandle(address + 824) ? uint32_t(memory::OwnerLoad(address + 824, 4)) : guest::LoadU32(address + 824))); }
  void set_unknown_824_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_824 = v; return; } if (memory::IsHandle(address + 824)) memory::OwnerStore(address + 824, 4, v); else guest::StoreU32(address + 824, v); }
  uint32_t unknown_824() const { if (auto* d = direct()) return d->unknown_824; return uint32_t((memory::IsHandle(address + 824) ? uint32_t(memory::OwnerLoad(address + 824, 4)) : guest::LoadU32(address + 824))); }
  void set_unknown_824(uint32_t v) const { if (auto* d = direct()) { d->unknown_824 = v; return; } if (memory::IsHandle(address + 824)) memory::OwnerStore(address + 824, 4, v); else guest::StoreU32(address + 824, v); }
  // 0 at the set-up; read by the marker's and model sprites' kinds
  uint32_t unknown_828_address() const { return address + 828; }
  uint32_t unknown_828_bits() const { if (auto* d = direct()) return d->unknown_828; return uint32_t((memory::IsHandle(address + 828) ? uint32_t(memory::OwnerLoad(address + 828, 4)) : guest::LoadU32(address + 828))); }
  void set_unknown_828_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_828 = v; return; } if (memory::IsHandle(address + 828)) memory::OwnerStore(address + 828, 4, v); else guest::StoreU32(address + 828, v); }
  uint32_t unknown_828() const { if (auto* d = direct()) return d->unknown_828; return uint32_t((memory::IsHandle(address + 828) ? uint32_t(memory::OwnerLoad(address + 828, 4)) : guest::LoadU32(address + 828))); }
  void set_unknown_828(uint32_t v) const { if (auto* d = direct()) { d->unknown_828 = v; return; } if (memory::IsHandle(address + 828)) memory::OwnerStore(address + 828, 4, v); else guest::StoreU32(address + 828, v); }
  uint32_t last_vehicle_address() const { return address + 832; }
  uint32_t last_vehicle_bits() const { if (auto* d = direct()) return d->last_vehicle; return uint32_t((memory::IsHandle(address + 832) ? uint32_t(memory::OwnerLoad(address + 832, 4)) : guest::LoadU32(address + 832))); }
  void set_last_vehicle_bits(uint32_t v) const { if (auto* d = direct()) { d->last_vehicle = v; return; } if (memory::IsHandle(address + 832)) memory::OwnerStore(address + 832, 4, v); else guest::StoreU32(address + 832, v); }
  uint32_t last_vehicle() const { if (auto* d = direct()) return d->last_vehicle; return uint32_t((memory::IsHandle(address + 832) ? uint32_t(memory::OwnerLoad(address + 832, 4)) : guest::LoadU32(address + 832))); }
  void set_last_vehicle(uint32_t v) const { if (auto* d = direct()) { d->last_vehicle = v; return; } if (memory::IsHandle(address + 832)) memory::OwnerStore(address + 832, 4, v); else guest::StoreU32(address + 832, v); }
  // -1 none
  uint32_t racer_id_address() const { return address + 836; }
  uint32_t racer_id_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->racer_id); return uint32_t((memory::IsHandle(address + 836) ? uint32_t(memory::OwnerLoad(address + 836, 4)) : guest::LoadU32(address + 836))); }
  void set_racer_id_bits(uint32_t v) const { if (auto* d = direct()) { d->racer_id = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 836)) memory::OwnerStore(address + 836, 4, v); else guest::StoreU32(address + 836, v); }
  int32_t racer_id() const { return int32_t(racer_id_bits()); }
  void set_racer_id(int32_t v) const { set_racer_id_bits(uint32_t(v)); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarOverlaysWheelData {
  uint32_t trail_counter = 0;  // +0
  uint32_t smoke_counter = 0;  // +4
  uint32_t trail_added = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(CarOverlaysWheelData) == 12);
inline constexpr memory::LayoutField kCarOverlaysWheelFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kCarOverlaysWheelLayout = {kCarOverlaysWheelFields, 3};

struct CarOverlaysWheel : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarOverlaysWheelData* data() const { return static_cast<CarOverlaysWheelData*>(memory::HandleObject(address)); }
  static CarOverlaysWheel New() {
    return CarOverlaysWheel(memory::MakeHandle(new CarOverlaysWheelData(), sizeof(CarOverlaysWheelData), "CarOverlaysWheel", &kCarOverlaysWheelLayout));
  }
  template <uint32_t Stride = kSize>
  static CarOverlaysWheel NewArray(uint32_t count) {
    static_assert(sizeof(CarOverlaysWheelData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarOverlaysWheelData*>(::operator new(sizeof(CarOverlaysWheelData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarOverlaysWheelData();
    return CarOverlaysWheel(memory::MakeHandle(p, sizeof(CarOverlaysWheelData) * count, "CarOverlaysWheel", &kCarOverlaysWheelLayout, sizeof(CarOverlaysWheelData)));
  }
  CarOverlaysWheel at(uint32_t i) const { return CarOverlaysWheel(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarOverlaysWheelData* direct() const { return static_cast<CarOverlaysWheelData*>(memory::HandleOwn(address, &kCarOverlaysWheelLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarOverlaysWheelLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarOverlaysWheelLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // counts the slipping ticks against the table's +156 and +160
  uint32_t trail_counter_address() const { return address + 0; }
  uint32_t trail_counter_bits() const { if (auto* d = direct()) return d->trail_counter; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_trail_counter_bits(uint32_t v) const { if (auto* d = direct()) { d->trail_counter = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t trail_counter() const { if (auto* d = direct()) return d->trail_counter; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_trail_counter(uint32_t v) const { if (auto* d = direct()) { d->trail_counter = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // up to 25 on surface 8, then counts down while smoke is emitted
  uint32_t smoke_counter_address() const { return address + 4; }
  uint32_t smoke_counter_bits() const { if (auto* d = direct()) return d->smoke_counter; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_smoke_counter_bits(uint32_t v) const { if (auto* d = direct()) { d->smoke_counter = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t smoke_counter() const { if (auto* d = direct()) return d->smoke_counter; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_smoke_counter(uint32_t v) const { if (auto* d = direct()) { d->smoke_counter = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // whether the last tick added a trail point
  uint32_t trail_added_address() const { return address + 8; }
  uint32_t trail_added_bits() const { if (auto* d = direct()) return d->trail_added; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_trail_added_bits(uint32_t v) const { if (auto* d = direct()) { d->trail_added = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t trail_added() const { if (auto* d = direct()) return d->trail_added; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_trail_added(uint32_t v) const { if (auto* d = direct()) { d->trail_added = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

// image; measure: -
struct CarOverlaysParams : View {
  using View::View;
  static constexpr uint32_t kSize = 164;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // scales the smoke rates when DebugSwitchOff is set
  float debug_rate_scale() const { return guest::LoadFloat(address + 4); }
  void set_debug_rate_scale(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t debug_rate_scale_address() const { return address + 4; }
  // smoke on marking surfaces: this plus +12 x slip
  float marks_rate() const { return guest::LoadFloat(address + 8); }
  void set_marks_rate(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t marks_rate_address() const { return address + 8; }
  float marks_rate_slip() const { return guest::LoadFloat(address + 12); }
  void set_marks_rate_slip(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t marks_rate_slip_address() const { return address + 12; }
  // the size draw on marking surfaces
  float marks_size() const { return guest::LoadFloat(address + 16); }
  void set_marks_size(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t marks_size_address() const { return address + 16; }
  // WheelSlip's threshold for the smoke
  float smoke_slip_threshold() const { return guest::LoadFloat(address + 20); }
  void set_smoke_slip_threshold(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t smoke_slip_threshold_address() const { return address + 20; }
  // times the rate on surface 8
  float surface8_rate() const { return guest::LoadFloat(address + 24); }
  void set_surface8_rate(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t surface8_rate_address() const { return address + 24; }
  // times the rate while the wheel's smoke counter runs
  float counter_rate() const { return guest::LoadFloat(address + 28); }
  void set_counter_rate(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t counter_rate_address() const { return address + 28; }
  uint32_t unknown_32_address() const { return address + 32; }
  // the size draw on surface 8 and while the counter runs
  float surface8_size() const { return guest::LoadFloat(address + 44); }
  void set_surface8_size(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t surface8_size_address() const { return address + 44; }
  uint32_t unknown_48_address() const { return address + 48; }
  // times the rate on surfaces 1-2
  float surface12_rate() const { return guest::LoadFloat(address + 68); }
  void set_surface12_rate(float v) const { guest::StoreFloat(address + 68, v); }
  uint32_t surface12_rate_address() const { return address + 68; }
  uint32_t unknown_72_address() const { return address + 72; }
  // the size draw on surfaces 1-2
  float surface12_size() const { return guest::LoadFloat(address + 80); }
  void set_surface12_size(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t surface12_size_address() const { return address + 80; }
  uint32_t unknown_84_address() const { return address + 84; }
  // WheelSlip's threshold for the trails
  float trail_slip_threshold() const { return guest::LoadFloat(address + 152); }
  void set_trail_slip_threshold(float v) const { guest::StoreFloat(address + 152, v); }
  uint32_t trail_slip_threshold_address() const { return address + 152; }
  // a trail point is due from this many slipping ticks
  uint32_t trail_least_count() const { return guest::LoadU32(address + 156); }
  void set_trail_least_count(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t trail_least_count_address() const { return address + 156; }
  // up to this many (the counter capped at twice it)
  uint32_t trail_most_count() const { return guest::LoadU32(address + 160); }
  void set_trail_most_count(uint32_t v) const { guest::StoreU32(address + 160, v); }
  uint32_t trail_most_count_address() const { return address + 160; }
};

// image; measure: -
struct OverlayParams : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // in ticks
  uint32_t life() const { return guest::LoadU32(address + 0); }
  void set_life(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t life_address() const { return address + 0; }
  float size() const { return guest::LoadFloat(address + 4); }
  void set_size(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t size_address() const { return address + 4; }
  // the alpha or brightness (the trails: the base width)
  float alpha() const { return guest::LoadFloat(address + 8); }
  void set_alpha(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t alpha_address() const { return address + 8; }
  // fades over this many ticks of life left
  uint32_t fade_ticks() const { return guest::LoadU32(address + 12); }
  void set_fade_ticks(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t fade_ticks_address() const { return address + 12; }
};

// allocated; measure: OverlayView, CarOverlays.+4
#pragma pack(push, 1)
struct OverlayViewData {
  uint32_t vtable = 0;  // +0
  uint32_t vehicle = 0;  // +4
  uint32_t model = 0;  // +8
  uint32_t viewport = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(OverlayViewData) == 16);
inline constexpr memory::LayoutField kOverlayViewFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kOverlayViewLayout = {kOverlayViewFields, 4};

struct OverlayView : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82010B5C;
  OverlayViewData* data() const { return static_cast<OverlayViewData*>(memory::HandleObject(address)); }
  static OverlayView New() {
    return OverlayView(memory::MakeHandle(new OverlayViewData(), sizeof(OverlayViewData), "OverlayView", &kOverlayViewLayout));
  }
  template <uint32_t Stride = kSize>
  static OverlayView NewArray(uint32_t count) {
    static_assert(sizeof(OverlayViewData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<OverlayViewData*>(::operator new(sizeof(OverlayViewData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) OverlayViewData();
    return OverlayView(memory::MakeHandle(p, sizeof(OverlayViewData) * count, "OverlayView", &kOverlayViewLayout, sizeof(OverlayViewData)));
  }
  OverlayView at(uint32_t i) const { return OverlayView(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  OverlayViewData* direct() const { return static_cast<OverlayViewData*>(memory::HandleOwn(address, &kOverlayViewLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kOverlayViewLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOverlayViewLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82010B5C (OverlayViewVtable)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vehicle_address() const { return address + 4; }
  uint32_t vehicle_bits() const { if (auto* d = direct()) return d->vehicle; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_vehicle_bits(uint32_t v) const { if (auto* d = direct()) { d->vehicle = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t vehicle() const { if (auto* d = direct()) return d->vehicle; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_vehicle(uint32_t v) const { if (auto* d = direct()) { d->vehicle = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // its scene model (0 none)
  uint32_t model_address() const { return address + 8; }
  uint32_t model_bits() const { if (auto* d = direct()) return d->model; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_model_bits(uint32_t v) const { if (auto* d = direct()) { d->model = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t model() const { if (auto* d = direct()) return d->model; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_model(uint32_t v) const { if (auto* d = direct()) { d->model = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t viewport_address() const { return address + 12; }
  uint32_t viewport_bits() const { if (auto* d = direct()) return d->viewport; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_viewport_bits(uint32_t v) const { if (auto* d = direct()) { d->viewport = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t viewport() const { if (auto* d = direct()) return d->viewport; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_viewport(uint32_t v) const { if (auto* d = direct()) { d->viewport = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
};

// embedded; measure: ViewDesc, OverlayView::Set.r4
struct ViewDesc : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t vehicle() const { return guest::LoadU32(address + 0); }
  void set_vehicle(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vehicle_address() const { return address + 0; }
  uint32_t model() const { return guest::LoadU32(address + 4); }
  void set_model(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t model_address() const { return address + 4; }
  uint32_t viewport() const { return guest::LoadU32(address + 8); }
  void set_viewport(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t viewport_address() const { return address + 8; }
};

// embedded; measure: -
struct OverlayViewportBinding : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // [[0x826C2CE8] + 4 x entry.0] + 80 x entry.1
  uint32_t record() const { return guest::LoadU32(address + 0); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t record_address() const { return address + 0; }
  // 0x826BAC00 + 592 x entry.2
  uint32_t camera_pass() const { return guest::LoadU32(address + 4); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t camera_pass_address() const { return address + 4; }
  uint32_t pair_0() const { return guest::LoadU32(address + 8); }
  void set_pair_0(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t pair_0_address() const { return address + 8; }
  uint32_t pair_1() const { return guest::LoadU32(address + 12); }
  void set_pair_1(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t pair_1_address() const { return address + 12; }
};

// image; measure: -
struct OverlayViewportEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // an index into [0x826C2CE8]
  uint32_t viewport() const { return guest::LoadU32(address + 0); }
  void set_viewport(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t viewport_address() const { return address + 0; }
  // an 80-byte record of the viewport
  uint32_t record() const { return guest::LoadU32(address + 4); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t record_address() const { return address + 4; }
  // an index of the 592-byte camera passes at 0x826BAC00
  uint32_t camera_pass() const { return guest::LoadU32(address + 8); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t camera_pass_address() const { return address + 8; }
};

// embedded; measure: Overlay
#pragma pack(push, 1)
struct OverlayData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[100] = {};
  uint32_t textures = 0;  // +104
  uint32_t textures_108 = 0;  // +108
};
#pragma pack(pop)
static_assert(sizeof(OverlayData) == 112);
inline constexpr memory::LayoutField kOverlayFields[] = {{0, 4}, {104, 4}, {108, 4}};
inline constexpr memory::Layout kOverlayLayout = {kOverlayFields, 3};

struct Overlay : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  static constexpr uint32_t kVtable = 0x820085D8;
  OverlayData* data() const { return static_cast<OverlayData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static Overlay New() {
    return Overlay(memory::MakeHandle(new OverlayData(), sizeof(OverlayData), "Overlay", &kOverlayLayout));
  }
  template <uint32_t Stride = kSize>
  static Overlay NewArray(uint32_t count) {
    static_assert(sizeof(OverlayData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<OverlayData*>(::operator new(sizeof(OverlayData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) OverlayData();
    return Overlay(memory::MakeHandle(p, sizeof(OverlayData) * count, "Overlay", &kOverlayLayout, sizeof(OverlayData)));
  }
  Overlay at(uint32_t i) const { return Overlay(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  OverlayData* direct() const { return static_cast<OverlayData*>(memory::HandleOwn(address, &kOverlayLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kOverlayLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kOverlayLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x820085D8 for the base class
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // SetTextures104 (the car overlays' texture table; particle effect C's texture)
  uint32_t textures_address() const { return address + 104; }
  uint32_t textures_bits() const { if (auto* d = direct()) return d->textures; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_textures_bits(uint32_t v) const { if (auto* d = direct()) { d->textures = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t textures() const { if (auto* d = direct()) return d->textures; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_textures(uint32_t v) const { if (auto* d = direct()) { d->textures = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  // SetTextures108 (the texture holder of the trails, smoke, sparks and particle effect B)
  uint32_t textures_108_address() const { return address + 108; }
  uint32_t textures_108_bits() const { if (auto* d = direct()) return d->textures_108; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_textures_108_bits(uint32_t v) const { if (auto* d = direct()) { d->textures_108 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t textures_108() const { if (auto* d = direct()) return d->textures_108; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_textures_108(uint32_t v) const { if (auto* d = direct()) { d->textures_108 = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
};

// allocated; measure: -
struct Trail : View {
  using View::View;
  static constexpr uint32_t kSize = 64160;
  static constexpr uint32_t kVtable = 0x820086C0;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // 500 TrailPoint
  uint32_t points_address() const { return address + 128; }
  // the oldest point's index
  uint32_t oldest() const { return guest::LoadU32(address + 64128); }
  void set_oldest(uint32_t v) const { guest::StoreU32(address + 64128, v); }
  uint32_t oldest_address() const { return address + 64128; }
  // where the next point goes
  uint32_t tail() const { return guest::LoadU32(address + 64132); }
  void set_tail(uint32_t v) const { guest::StoreU32(address + 64132, v); }
  uint32_t tail_address() const { return address + 64132; }
  uint32_t count() const { return guest::LoadU32(address + 64136); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 64136, v); }
  uint32_t count_address() const { return address + 64136; }
  // a vector
  uint32_t last_position_address() const { return address + 64144; }
};

// embedded; measure: -
struct TrailPoint : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // a vector
  uint32_t position_address() const { return address + 0; }
  // a vector: the trail's last position when added
  uint32_t last_position_address() const { return address + 16; }
  uint32_t red() const { return guest::LoadU32(address + 32); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t red_address() const { return address + 32; }
  uint32_t green() const { return guest::LoadU32(address + 36); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t green_address() const { return address + 36; }
  uint32_t blue() const { return guest::LoadU32(address + 40); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t blue_address() const { return address + 40; }
  // the width (+56) as an integer
  uint32_t width_int() const { return guest::LoadU32(address + 44); }
  void set_width_int(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t width_int_address() const { return address + 44; }
  // the value AddPoint is given; 1 once more than a tick old
  uint32_t shown() const { return guest::LoadU32(address + 48); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t shown_address() const { return address + 48; }
  // the width AddPoint is given (the edges' distance)
  float side_width() const { return guest::LoadFloat(address + 52); }
  void set_side_width(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t side_width_address() const { return address + 52; }
  // fades from +124
  float width() const { return guest::LoadFloat(address + 56); }
  void set_width(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t width_address() const { return address + 56; }
  // ticks left
  uint32_t age() const { return guest::LoadU32(address + 60); }
  void set_age(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t age_address() const { return address + 60; }
  // a vector: position less last position
  uint32_t difference_address() const { return address + 64; }
  // a vector: the ribbon's edge (position less the side)
  uint32_t edge_minus_address() const { return address + 80; }
  // a vector: the ribbon's other edge
  uint32_t edge_plus_address() const { return address + 96; }
  // 1 or 2 (2: joined to the one before)
  uint32_t state() const { return guest::LoadU32(address + 112); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t state_address() const { return address + 112; }
  // set to 1 where the trail breaks
  uint32_t broken() const { return guest::LoadU32(address + 116); }
  void set_broken(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t broken_address() const { return address + 116; }
  uint32_t texture_coordinate() const { return guest::LoadU32(address + 120); }
  void set_texture_coordinate(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t texture_coordinate_address() const { return address + 120; }
  float full_width() const { return guest::LoadFloat(address + 124); }
  void set_full_width(float v) const { guest::StoreFloat(address + 124, v); }
  uint32_t full_width_address() const { return address + 124; }
};

// allocated; measure: -
struct Smoke : View {
  using View::View;
  static constexpr uint32_t kSize = 32640;
  static constexpr uint32_t kVtable = 0x820086F4;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // 0 at construction; the set-up's +0 at Init
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // 45 SmokePuff
  uint32_t puffs_address() const { return address + 128; }
  uint32_t oldest() const { return guest::LoadU32(address + 32528); }
  void set_oldest(uint32_t v) const { guest::StoreU32(address + 32528, v); }
  uint32_t oldest_address() const { return address + 32528; }
  // where the next puff goes
  uint32_t head() const { return guest::LoadU32(address + 32532); }
  void set_head(uint32_t v) const { guest::StoreU32(address + 32532, v); }
  uint32_t head_address() const { return address + 32532; }
  uint32_t count() const { return guest::LoadU32(address + 32536); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 32536, v); }
  uint32_t count_address() const { return address + 32536; }
  // a vector (Smoke::SetVelocity)
  uint32_t velocity_address() const { return address + 32544; }
  // a vector: with +32,576, the span puffs drift along when the region is on
  uint32_t region_start_address() const { return address + 32560; }
  // a vector
  uint32_t region_end_address() const { return address + 32576; }
  // a vector: SetRegion's second point (the rear axle's), InRegion's a
  uint32_t region_rear_address() const { return address + 32592; }
  // a vector: SetRegion's first point (the front axle's), InRegion's b
  uint32_t region_front_address() const { return address + 32608; }
  // the view mode SetRegion is given; 1 turns the region on
  uint32_t region_on() const { return guest::LoadU32(address + 32624); }
  void set_region_on(uint32_t v) const { guest::StoreU32(address + 32624, v); }
  uint32_t region_on_address() const { return address + 32624; }
};

// embedded; measure: -
struct SmokePuff : View {
  using View::View;
  static constexpr uint32_t kSize = 720;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t unknown_32_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: the smoke's velocity (+32,544) at the emit
  uint32_t velocity_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(address + 100); }
  void set_size(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  // the size Smoke::Emit is given
  float emit_size() const { return guest::LoadFloat(address + 112); }
  void set_emit_size(float v) const { guest::StoreFloat(address + 112, v); }
  uint32_t emit_size_address() const { return address + 112; }
  float growth() const { return guest::LoadFloat(address + 116); }
  void set_growth(float v) const { guest::StoreFloat(address + 116, v); }
  uint32_t growth_address() const { return address + 116; }
  float most_alpha() const { return guest::LoadFloat(address + 120); }
  void set_most_alpha(float v) const { guest::StoreFloat(address + 120, v); }
  uint32_t most_alpha_address() const { return address + 120; }
  float start_size() const { return guest::LoadFloat(address + 124); }
  void set_start_size(float v) const { guest::StoreFloat(address + 124, v); }
  uint32_t start_size_address() const { return address + 124; }
  // 2 to 10
  uint32_t particle_count() const { return guest::LoadU32(address + 128); }
  void set_particle_count(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t particle_count_address() const { return address + 128; }
  // a 4x4 matrix (Smoke::SetFrame)
  uint32_t frame_address() const { return address + 144; }
  // a vector: the frame's +64
  uint32_t frame_position_address() const { return address + 208; }
  // the frame's +80 (a float's bits)
  uint32_t frame_value() const { return guest::LoadU32(address + 224); }
  void set_frame_value(uint32_t v) const { guest::StoreU32(address + 224, v); }
  uint32_t frame_value_address() const { return address + 224; }
  float unknown_228() const { return guest::LoadFloat(address + 228); }
  void set_unknown_228(float v) const { guest::StoreFloat(address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  // 10 OverlayParticle
  uint32_t particles_address() const { return address + 240; }
};

// embedded; measure: Particle
struct OverlayParticle : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // a vector (1.0 in its w at construction)
  uint32_t position_address() const { return address + 0; }
  // a vector (1.0 in its w at construction)
  uint32_t velocity_address() const { return address + 16; }
  // rand() % 3 (probably a texture coordinate choice)
  uint32_t pick_a() const { return guest::LoadU32(address + 32); }
  void set_pick_a(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t pick_a_address() const { return address + 32; }
  // rand() % 4 (probably a texture coordinate choice)
  uint32_t pick_b() const { return guest::LoadU32(address + 36); }
  void set_pick_b(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t pick_b_address() const { return address + 36; }
  uint32_t shown() const { return guest::LoadU32(address + 40); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t shown_address() const { return address + 40; }
};

// allocated; measure: -
struct Sparks : View {
  using View::View;
  static constexpr uint32_t kSize = 29824;
  static constexpr uint32_t kVtable = 0x820087BC;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // 200 Spark
  uint32_t sparks_address() const { return address + 128; }
  uint32_t streak_counter() const { return guest::LoadU32(address + 28928); }
  void set_streak_counter(uint32_t v) const { guest::StoreU32(address + 28928, v); }
  uint32_t streak_counter_address() const { return address + 28928; }
  uint32_t oldest() const { return guest::LoadU32(address + 28932); }
  void set_oldest(uint32_t v) const { guest::StoreU32(address + 28932, v); }
  uint32_t oldest_address() const { return address + 28932; }
  // where the next spark goes
  uint32_t head() const { return guest::LoadU32(address + 28936); }
  void set_head(uint32_t v) const { guest::StoreU32(address + 28936, v); }
  uint32_t head_address() const { return address + 28936; }
  uint32_t count() const { return guest::LoadU32(address + 28940); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 28940, v); }
  uint32_t count_address() const { return address + 28940; }
  float spread() const { return guest::LoadFloat(address + 28944); }
  void set_spread(float v) const { guest::StoreFloat(address + 28944, v); }
  uint32_t spread_address() const { return address + 28944; }
  // a vector
  uint32_t position_address() const { return address + 28960; }
  uint32_t streaking() const { return guest::LoadU32(address + 28976); }
  void set_streaking(uint32_t v) const { guest::StoreU32(address + 28976, v); }
  uint32_t streaking_address() const { return address + 28976; }
  // a vector
  uint32_t previous_position_address() const { return address + 28992; }
  // a vector
  uint32_t latest_position_address() const { return address + 29008; }
  // 50 vectors
  uint32_t random_vectors_address() const { return address + 29024; }
};

// embedded; measure: -
struct Spark : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: where it started (the emitter's position at the emit)
  uint32_t origin_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(address + 100); }
  void set_size(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  // a vector: position less origin
  uint32_t offset_address() const { return address + 112; }
  // 0 to 2
  uint32_t state() const { return guest::LoadU32(address + 128); }
  void set_state(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t state_address() const { return address + 128; }
  uint32_t unknown_132() const { return guest::LoadU32(address + 132); }
  void set_unknown_132(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t unknown_132_address() const { return address + 132; }
  // a streak's length limit
  float limit() const { return guest::LoadFloat(address + 136); }
  void set_limit(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t limit_address() const { return address + 136; }
};

// allocated; measure: -
struct ParticlesB : View {
  using View::View;
  static constexpr uint32_t kSize = 2816;
  static constexpr uint32_t kVtable = 0x82008720;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the entries' capacity
  uint32_t capacity() const { return guest::LoadU32(address + 104); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t capacity_address() const { return address + 104; }
  // SetTextures108
  uint32_t texture_holder() const { return guest::LoadU32(address + 108); }
  void set_texture_holder(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t texture_holder_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 112)); }
  void set_params(const View& v) const { guest::StoreU32(address + 112, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t params_address() const { return address + 112; }
  // the set-up's +0 at Init
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // 15 ParticlesBEntry
  uint32_t entries_address() const { return address + 128; }
  uint32_t oldest() const { return guest::LoadU32(address + 2768); }
  void set_oldest(uint32_t v) const { guest::StoreU32(address + 2768, v); }
  uint32_t oldest_address() const { return address + 2768; }
  // where the next entry goes
  uint32_t head() const { return guest::LoadU32(address + 2772); }
  void set_head(uint32_t v) const { guest::StoreU32(address + 2772, v); }
  uint32_t head_address() const { return address + 2772; }
  uint32_t count() const { return guest::LoadU32(address + 2776); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 2776, v); }
  uint32_t count_address() const { return address + 2776; }
  // a vector
  uint32_t position_address() const { return address + 2784; }
  // a vector: the last emit's position less the position before
  uint32_t moved_address() const { return address + 2800; }
};

// embedded; measure: -
struct ParticlesBEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: where it started (the emitter's position at the emit)
  uint32_t origin_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(address + 100); }
  void set_size(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  float most_alpha() const { return guest::LoadFloat(address + 112); }
  void set_most_alpha(float v) const { guest::StoreFloat(address + 112, v); }
  uint32_t most_alpha_address() const { return address + 112; }
  uint32_t particle_count() const { return guest::LoadU32(address + 116); }
  void set_particle_count(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t particle_count_address() const { return address + 116; }
  // an OverlayParticle
  uint32_t particle_address() const { return address + 128; }
};

// allocated; measure: -
struct ParticlesC : View {
  using View::View;
  static constexpr uint32_t kSize = 2720;
  static constexpr uint32_t kVtable = 0x82008750;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // SetTextures104
  uint32_t texture() const { return guest::LoadU32(address + 104); }
  void set_texture(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t texture_address() const { return address + 104; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 108)); }
  void set_params(const View& v) const { guest::StoreU32(address + 108, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t params_address() const { return address + 108; }
  // 20 ParticlesCEntry
  uint32_t entries_address() const { return address + 112; }
  // a vector
  uint32_t position_address() const { return address + 2672; }
  uint32_t oldest() const { return guest::LoadU32(address + 2688); }
  void set_oldest(uint32_t v) const { guest::StoreU32(address + 2688, v); }
  uint32_t oldest_address() const { return address + 2688; }
  // where the next entry goes
  uint32_t newest() const { return guest::LoadU32(address + 2692); }
  void set_newest(uint32_t v) const { guest::StoreU32(address + 2692, v); }
  uint32_t newest_address() const { return address + 2692; }
  uint32_t count() const { return guest::LoadU32(address + 2696); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 2696, v); }
  uint32_t count_address() const { return address + 2696; }
  uint32_t capacity() const { return guest::LoadU32(address + 2700); }
  void set_capacity(uint32_t v) const { guest::StoreU32(address + 2700, v); }
  uint32_t capacity_address() const { return address + 2700; }
  float speed() const { return guest::LoadFloat(address + 2704); }
  void set_speed(float v) const { guest::StoreFloat(address + 2704, v); }
  uint32_t speed_address() const { return address + 2704; }
};

// embedded; measure: -
struct ParticlesCEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector: where it started (the emitter's position at the emit)
  uint32_t origin_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(address + 100); }
  void set_size(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  uint32_t unknown_112() const { return guest::LoadU32(address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
};

// embedded; measure: -
struct OverlaySpriteEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_0_address() const { return address + 0; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_16_address() const { return address + 16; }
  // a vector
  uint32_t velocity_address() const { return address + 32; }
  // a vector
  uint32_t position_address() const { return address + 48; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t red() const { return guest::LoadU32(address + 80); }
  void set_red(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  uint32_t green() const { return guest::LoadU32(address + 84); }
  void set_green(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  uint32_t blue() const { return guest::LoadU32(address + 88); }
  void set_blue(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  // the alpha (+104) as an integer (fctiwz)
  uint32_t alpha_int() const { return guest::LoadU32(address + 92); }
  void set_alpha_int(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t alpha_int_address() const { return address + 92; }
  uint32_t shown() const { return guest::LoadU32(address + 96); }
  void set_shown(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t shown_address() const { return address + 96; }
  float size() const { return guest::LoadFloat(address + 100); }
  void set_size(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t size_address() const { return address + 100; }
  float alpha() const { return guest::LoadFloat(address + 104); }
  void set_alpha(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t alpha_address() const { return address + 104; }
  // ticks left (an age counting up for the marker)
  uint32_t life() const { return guest::LoadU32(address + 108); }
  void set_life(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t life_address() const { return address + 108; }
  // the texture: the marker's kind 0-3; the car-model sprite's state (1-8, +336); 0 for the light sprite
  uint32_t kind() const { return guest::LoadU32(address + 112); }
  void set_kind(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t kind_address() const { return address + 112; }
  // 4 at a reset; the light sprite's rand() % 4 when shown
  uint32_t unknown_116() const { return guest::LoadU32(address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // the car-model sprite's (9 at a reset)
  uint32_t unknown_120() const { return guest::LoadU32(address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  // the car-model sprite's (12 at a reset)
  uint32_t unknown_124() const { return guest::LoadU32(address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  // the car-model sprite's (0 at a reset)
  uint32_t unknown_128() const { return guest::LoadU32(address + 128); }
  void set_unknown_128(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
};

// allocated; measure: -
struct LightSprite : View {
  using View::View;
  static constexpr uint32_t kSize = 288;
  static constexpr uint32_t kVtable = 0x82008444;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // nine texture table entries' addresses
  uint32_t textures_address() const { return address + 104; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 140)); }
  void set_params(const View& v) const { guest::StoreU32(address + 140, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t params_address() const { return address + 140; }
  // an OverlaySpriteEntry
  uint32_t sprite_address() const { return address + 144; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_272_address() const { return address + 272; }
};

// allocated; measure: -
struct Marker : View {
  using View::View;
  static constexpr uint32_t kSize = 304;
  static constexpr uint32_t kVtable = 0x82008784;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // four texture table entries' addresses
  uint32_t textures_address() const { return address + 104; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 120)); }
  void set_params(const View& v) const { guest::StoreU32(address + 120, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t params_address() const { return address + 120; }
  // an OverlaySpriteEntry
  uint32_t sprite_address() const { return address + 128; }
  uint32_t view_mode() const { return guest::LoadU32(address + 256); }
  void set_view_mode(uint32_t v) const { guest::StoreU32(address + 256, v); }
  uint32_t view_mode_address() const { return address + 256; }
  // a vector
  uint32_t camera_position_address() const { return address + 272; }
  // 1 for the camera-facing quad
  uint32_t facing() const { return guest::LoadU32(address + 288); }
  void set_facing(uint32_t v) const { guest::StoreU32(address + 288, v); }
  uint32_t facing_address() const { return address + 288; }
};

// allocated; measure: -
struct ModelSprite : View {
  using View::View;
  static constexpr uint32_t kSize = 576;
  static constexpr uint32_t kVtable = 0x820084E8;
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // Init's r5 as a float
  float unknown_104() const { return guest::LoadFloat(address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // 25 texture table entries' addresses
  uint32_t textures_address() const { return address + 108; }
  template <typename T = OverlayParams>
  T params() const { return T(guest::LoadU32(address + 208)); }
  void set_params(const View& v) const { guest::StoreU32(address + 208, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t params_address() const { return address + 208; }
  // an OverlaySpriteEntry
  uint32_t sprite_address() const { return address + 224; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_368_address() const { return address + 368; }
  // a vector, (0, 0, 0, 1) at a reset
  uint32_t unknown_384_address() const { return address + 384; }
  // -1 at a reset
  int32_t unknown_404() const { return int32_t(guest::LoadU32(address + 404)); }
  void set_unknown_404(int32_t v) const { guest::StoreU32(address + 404, uint32_t(v)); }
  uint32_t unknown_404_address() const { return address + 404; }
  // -1 at a reset
  int32_t unknown_408() const { return int32_t(guest::LoadU32(address + 408)); }
  void set_unknown_408(int32_t v) const { guest::StoreU32(address + 408, uint32_t(v)); }
  uint32_t unknown_408_address() const { return address + 408; }
  // ten vectors, (0, 0, 0, 1) at a reset
  uint32_t unknown_416_address() const { return address + 416; }
};

// embedded; measure: Sprite, Line::RecordFlat.r4
struct OverlaySprite : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // a 4x4 matrix (identity at ResetStyle2)
  uint32_t matrix_address() const { return address + 0; }
  // drawn only when 0
  uint32_t skip() const { return guest::LoadU32(address + 64); }
  void set_skip(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t skip_address() const { return address + 64; }
  // a render state mode, 0-5
  uint32_t blend_mode() const { return guest::LoadU32(address + 68); }
  void set_blend_mode(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t blend_mode_address() const { return address + 68; }
  // a render state mode, 0-3 (2 at ResetStyle2)
  uint32_t address_mode() const { return guest::LoadU32(address + 72); }
  void set_address_mode(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t address_mode_address() const { return address + 72; }
  // four corners (x, y, z, w); 1.0 in the first two's w at ResetStyle2
  uint32_t corners_address() const { return address + 80; }
  // from LineSprite (power-graph)
  float end0_x() const { return guest::LoadFloat(address + 80); }
  void set_end0_x(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t end0_x_address() const { return address + 80; }
  // from LineSprite (power-graph)
  float end0_y() const { return guest::LoadFloat(address + 84); }
  void set_end0_y(float v) const { guest::StoreFloat(address + 84, v); }
  uint32_t end0_y_address() const { return address + 84; }
  // from LineSprite (power-graph)
  float end0_z() const { return guest::LoadFloat(address + 88); }
  void set_end0_z(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t end0_z_address() const { return address + 88; }
  // from LineSprite (power-graph)
  float end1_x() const { return guest::LoadFloat(address + 96); }
  void set_end1_x(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t end1_x_address() const { return address + 96; }
  // from LineSprite (power-graph)
  float end1_y() const { return guest::LoadFloat(address + 100); }
  void set_end1_y(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t end1_y_address() const { return address + 100; }
  // from LineSprite (power-graph)
  float end1_z() const { return guest::LoadFloat(address + 104); }
  void set_end1_z(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t end1_z_address() const { return address + 104; }
  // 0 to 255 (from LineSprite, power-graph)
  uint32_t end0_red() const { return guest::LoadU32(address + 112); }
  void set_end0_red(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t end0_red_address() const { return address + 112; }
  // end0_red's low byte, as Line::RecordFlat reads it (from LineSprite, power-graph)
  uint8_t end0_red_byte() const { return guest::LoadU8(address + 115); }
  void set_end0_red_byte(uint8_t v) const { guest::StoreU8(address + 115, v); }
  uint32_t end0_red_byte_address() const { return address + 115; }
  // from LineSprite (power-graph)
  uint32_t end0_green() const { return guest::LoadU32(address + 116); }
  void set_end0_green(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t end0_green_address() const { return address + 116; }
  // from LineSprite (power-graph)
  uint8_t end0_green_byte() const { return guest::LoadU8(address + 119); }
  void set_end0_green_byte(uint8_t v) const { guest::StoreU8(address + 119, v); }
  uint32_t end0_green_byte_address() const { return address + 119; }
  // from LineSprite (power-graph)
  uint32_t end0_blue() const { return guest::LoadU32(address + 120); }
  void set_end0_blue(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t end0_blue_address() const { return address + 120; }
  // from LineSprite (power-graph)
  uint8_t end0_blue_byte() const { return guest::LoadU8(address + 123); }
  void set_end0_blue_byte(uint8_t v) const { guest::StoreU8(address + 123, v); }
  uint32_t end0_blue_byte_address() const { return address + 123; }
  // shifted into the vertex color's top byte (from LineSprite, power-graph)
  uint32_t end0_alpha() const { return guest::LoadU32(address + 124); }
  void set_end0_alpha(uint32_t v) const { guest::StoreU32(address + 124, v); }
  uint32_t end0_alpha_address() const { return address + 124; }
  // from LineSprite (power-graph)
  uint32_t end1_red() const { return guest::LoadU32(address + 128); }
  void set_end1_red(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t end1_red_address() const { return address + 128; }
  // from LineSprite (power-graph)
  uint8_t end1_red_byte() const { return guest::LoadU8(address + 131); }
  void set_end1_red_byte(uint8_t v) const { guest::StoreU8(address + 131, v); }
  uint32_t end1_red_byte_address() const { return address + 131; }
  // from LineSprite (power-graph)
  uint32_t end1_green() const { return guest::LoadU32(address + 132); }
  void set_end1_green(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t end1_green_address() const { return address + 132; }
  // from LineSprite (power-graph)
  uint8_t end1_green_byte() const { return guest::LoadU8(address + 135); }
  void set_end1_green_byte(uint8_t v) const { guest::StoreU8(address + 135, v); }
  uint32_t end1_green_byte_address() const { return address + 135; }
  // from LineSprite (power-graph)
  uint32_t end1_blue() const { return guest::LoadU32(address + 136); }
  void set_end1_blue(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t end1_blue_address() const { return address + 136; }
  // from LineSprite (power-graph)
  uint8_t end1_blue_byte() const { return guest::LoadU8(address + 139); }
  void set_end1_blue_byte(uint8_t v) const { guest::StoreU8(address + 139, v); }
  uint32_t end1_blue_byte_address() const { return address + 139; }
  // from LineSprite (power-graph)
  uint32_t end1_alpha() const { return guest::LoadU32(address + 140); }
  void set_end1_alpha(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t end1_alpha_address() const { return address + 140; }
};

// embedded; measure: RaceMode@+157552
struct RaceOverlaysAt157552 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: RaceOverlays::OnUpdate.r3@+9608
struct RaceOverlaysAt9608 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_8215E158.r3@+9608
struct RaceOverlaysAt9608_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_8215E680.r3@+9608
struct RaceOverlaysAt9608_3 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: sub_8215E6C0.r3@+9608
struct RaceOverlaysAt9608_4 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: sub_8215E6C0.r3@+9608.+0
struct CarOverlaysPart : View {
  using View::View;
  static constexpr uint32_t kSize = 108;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
};

// embedded; measure: sub_8215E6C0.r3@+9608.+4
struct CarOverlaysPart_2 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_8215E6C0.r3@+9608.+4.+0
struct OverlayVtablePart : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

// allocated; measure: -
struct CarOverlaysTextureFile : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static CarOverlaysTextureFile New(uint32_t size) { return CarOverlaysTextureFile(memory::NewBlob(size, "CarOverlaysTextureFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// The image words docs/functions/race-overlays.md's code uses.
struct RaceOverlaysGlobals {
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // the race data (racer records from +48, the race mode at +0xA3AF4)
  static uint32_t race_data() { return guest::LoadU32(0x826C5B2Cu); }
  static void set_race_data(uint32_t v) { guest::StoreU32(0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // OverlayViewportEntry, 12 bytes each
  static uint32_t viewport_entries() { return guest::LoadU32(0x82008558u); }
  static void set_viewport_entries(uint32_t v) { guest::StoreU32(0x82008558u, v); }
  static constexpr uint32_t viewport_entries_address() { return 0x82008558u; }
  // pairs of words, 8 bytes each
  static uint32_t viewport_pairs() { return guest::LoadU32(0x820085A0u); }
  static void set_viewport_pairs(uint32_t v) { guest::StoreU32(0x820085A0u, v); }
  static constexpr uint32_t viewport_pairs_address() { return 0x820085A0u; }
  // the viewports (camera-modes.toml)
  static uint32_t viewport_owners() { return guest::LoadU32(0x826C2CE8u); }
  static void set_viewport_owners(uint32_t v) { guest::StoreU32(0x826C2CE8u, v); }
  static constexpr uint32_t viewport_owners_address() { return 0x826C2CE8u; }
  // the camera passes (592 bytes each, cameras.toml)
  static uint32_t camera_passes() { return guest::LoadU32(0x826BAC00u); }
  static void set_camera_passes(uint32_t v) { guest::StoreU32(0x826BAC00u, v); }
  static constexpr uint32_t camera_passes_address() { return 0x826BAC00u; }
  // the sprite recorders' render queue (Overlays::SetRenderTargets)
  static uint32_t render_queue() { return guest::LoadU32(0x826C5D5Cu); }
  static void set_render_queue(uint32_t v) { guest::StoreU32(0x826C5D5Cu, v); }
  static constexpr uint32_t render_queue_address() { return 0x826C5D5Cu; }
  // the sprite recorders' command list
  static uint32_t command_list() { return guest::LoadU32(0x826C5D60u); }
  static void set_command_list(uint32_t v) { guest::StoreU32(0x826C5D60u, v); }
  static constexpr uint32_t command_list_address() { return 0x826C5D60u; }
  // the game's own rand's seed
  static uint32_t rand_seed() { return guest::LoadU32(0x82638188u); }
  static void set_rand_seed(uint32_t v) { guest::StoreU32(0x82638188u, v); }
  static constexpr uint32_t rand_seed_address() { return 0x82638188u; }
  // probably 1/32,767
  static float rand_scale() { return guest::LoadFloat(0x82002BD4u); }
  static void set_rand_scale(float v) { guest::StoreFloat(0x82002BD4u, v); }
  static constexpr uint32_t rand_scale_address() { return 0x82002BD4u; }
  static float axle_scale() { return guest::LoadFloat(0x82001FBCu); }
  static void set_axle_scale(float v) { guest::StoreFloat(0x82001FBCu, v); }
  static constexpr uint32_t axle_scale_address() { return 0x82001FBCu; }
  static float marker_slope() { return guest::LoadFloat(0x82002FB0u); }
  static void set_marker_slope(float v) { guest::StoreFloat(0x82002FB0u, v); }
  static constexpr uint32_t marker_slope_address() { return 0x82002FB0u; }
  static float marker_most_size() { return guest::LoadFloat(0x82002C3Cu); }
  static void set_marker_most_size(float v) { guest::StoreFloat(0x82002C3Cu, v); }
  static constexpr uint32_t marker_most_size_address() { return 0x82002C3Cu; }
  static float marker_least_size() { return guest::LoadFloat(0x82002568u); }
  static void set_marker_least_size(float v) { guest::StoreFloat(0x82002568u, v); }
  static constexpr uint32_t marker_least_size_address() { return 0x82002568u; }
  static float trail_most_width() { return guest::LoadFloat(0x82000C44u); }
  static void set_trail_most_width(float v) { guest::StoreFloat(0x82000C44u, v); }
  static constexpr uint32_t trail_most_width_address() { return 0x82000C44u; }
  // the trails' side scale, particle effect B's spread, the sparks' size scale
  static float half() { return guest::LoadFloat(0x82001BE4u); }
  static void set_half(float v) { guest::StoreFloat(0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  static float particles_b_size_scale() { return guest::LoadFloat(0x82008540u); }
  static void set_particles_b_size_scale(float v) { guest::StoreFloat(0x82008540u, v); }
  static constexpr uint32_t particles_b_size_scale_address() { return 0x82008540u; }
  // a random angle's low end
  static float angle_low() { return guest::LoadFloat(0x820029C4u); }
  static void set_angle_low(float v) { guest::StoreFloat(0x820029C4u, v); }
  static constexpr uint32_t angle_low_address() { return 0x820029C4u; }
  // a random angle's high end
  static float angle_high() { return guest::LoadFloat(0x82002240u); }
  static void set_angle_high(float v) { guest::StoreFloat(0x82002240u, v); }
  static constexpr uint32_t angle_high_address() { return 0x82002240u; }
  static float spread_low() { return guest::LoadFloat(0x82006908u); }
  static void set_spread_low(float v) { guest::StoreFloat(0x82006908u, v); }
  static constexpr uint32_t spread_low_address() { return 0x82006908u; }
  static float spread_high() { return guest::LoadFloat(0x82002208u); }
  static void set_spread_high(float v) { guest::StoreFloat(0x82002208u, v); }
  static constexpr uint32_t spread_high_address() { return 0x82002208u; }
  // the sparks' spread and the smoke's growth from the speed
  static float speed_scale() { return guest::LoadFloat(0x820029E4u); }
  static void set_speed_scale(float v) { guest::StoreFloat(0x820029E4u, v); }
  static constexpr uint32_t speed_scale_address() { return 0x820029E4u; }
  static float sparks_most_spread() { return guest::LoadFloat(0x82002C50u); }
  static void set_sparks_most_spread(float v) { guest::StoreFloat(0x82002C50u, v); }
  static constexpr uint32_t sparks_most_spread_address() { return 0x82002C50u; }
  static float sparks_mode_factor() { return guest::LoadFloat(0x820035B0u); }
  static void set_sparks_mode_factor(float v) { guest::StoreFloat(0x820035B0u, v); }
  static constexpr uint32_t sparks_mode_factor_address() { return 0x820035B0u; }
  static float sparks_limit_high() { return guest::LoadFloat(0x820021E4u); }
  static void set_sparks_limit_high(float v) { guest::StoreFloat(0x820021E4u, v); }
  static constexpr uint32_t sparks_limit_high_address() { return 0x820021E4u; }
  // the sparks' limit low end, the smoke's frame scale, the trails' width scale
  static float frame_scale() { return guest::LoadFloat(0x820021E8u); }
  static void set_frame_scale(float v) { guest::StoreFloat(0x820021E8u, v); }
  static constexpr uint32_t frame_scale_address() { return 0x820021E8u; }
  static float sparks_step() { return guest::LoadFloat(0x82002674u); }
  static void set_sparks_step(float v) { guest::StoreFloat(0x82002674u, v); }
  static constexpr uint32_t sparks_step_address() { return 0x82002674u; }
  static float sparks_speed_out() { return guest::LoadFloat(0x82002204u); }
  static void set_sparks_speed_out(float v) { guest::StoreFloat(0x82002204u, v); }
  static constexpr uint32_t sparks_speed_out_address() { return 0x82002204u; }
  static float sparks_speed_in() { return guest::LoadFloat(0x820087B8u); }
  static void set_sparks_speed_in(float v) { guest::StoreFloat(0x820087B8u, v); }
  static constexpr uint32_t sparks_speed_in_address() { return 0x820087B8u; }
  static float sparks_rise_state0() { return guest::LoadFloat(0x820087B0u); }
  static void set_sparks_rise_state0(float v) { guest::StoreFloat(0x820087B0u, v); }
  static constexpr uint32_t sparks_rise_state0_address() { return 0x820087B0u; }
  static float sparks_rise_state2() { return guest::LoadFloat(0x820087B4u); }
  static void set_sparks_rise_state2(float v) { guest::StoreFloat(0x820087B4u, v); }
  static constexpr uint32_t sparks_rise_state2_address() { return 0x820087B4u; }
  static float smoke_growth_high() { return guest::LoadFloat(0x82002C4Cu); }
  static void set_smoke_growth_high(float v) { guest::StoreFloat(0x82002C4Cu, v); }
  static constexpr uint32_t smoke_growth_high_address() { return 0x82002C4Cu; }
  static float smoke_rise_high() { return guest::LoadFloat(0x820029D4u); }
  static void set_smoke_rise_high(float v) { guest::StoreFloat(0x820029D4u, v); }
  static constexpr uint32_t smoke_rise_high_address() { return 0x820029D4u; }
  static float smoke_drift_scale() { return guest::LoadFloat(0x820086F0u); }
  static void set_smoke_drift_scale(float v) { guest::StoreFloat(0x820086F0u, v); }
  static constexpr uint32_t smoke_drift_scale_address() { return 0x820086F0u; }
  static float smoke_drift_high() { return guest::LoadFloat(0x82002C48u); }
  static void set_smoke_drift_high(float v) { guest::StoreFloat(0x82002C48u, v); }
  static constexpr uint32_t smoke_drift_high_address() { return 0x82002C48u; }
  static float smoke_radius_base() { return guest::LoadFloat(0x8200222Cu); }
  static void set_smoke_radius_base(float v) { guest::StoreFloat(0x8200222Cu, v); }
  static constexpr uint32_t smoke_radius_base_address() { return 0x8200222Cu; }
  static float smoke_count_scale() { return guest::LoadFloat(0x820029ECu); }
  static void set_smoke_count_scale(float v) { guest::StoreFloat(0x820029ECu, v); }
  static constexpr uint32_t smoke_count_scale_address() { return 0x820029ECu; }
  static float smoke_radius_scale() { return guest::LoadFloat(0x820086ECu); }
  static void set_smoke_radius_scale(float v) { guest::StoreFloat(0x820086ECu, v); }
  static constexpr uint32_t smoke_radius_scale_address() { return 0x820086ECu; }
  // a puff's size at a reset
  static float smoke_puff_size() { return guest::LoadFloat(0x82002A90u); }
  static void set_smoke_puff_size(float v) { guest::StoreFloat(0x82002A90u, v); }
  static constexpr uint32_t smoke_puff_size_address() { return 0x82002A90u; }
  static float particles_b_most_speed() { return guest::LoadFloat(0x82002A34u); }
  static void set_particles_b_most_speed(float v) { guest::StoreFloat(0x82002A34u, v); }
  static constexpr uint32_t particles_b_most_speed_address() { return 0x82002A34u; }
  static float particles_b_least_speed() { return guest::LoadFloat(0x82002A3Cu); }
  static void set_particles_b_least_speed(float v) { guest::StoreFloat(0x82002A3Cu, v); }
  static constexpr uint32_t particles_b_least_speed_address() { return 0x82002A3Cu; }
  static float particles_c_speed_scale() { return guest::LoadFloat(0x82002BF0u); }
  static void set_particles_c_speed_scale(float v) { guest::StoreFloat(0x82002BF0u, v); }
  static constexpr uint32_t particles_c_speed_scale_address() { return 0x82002BF0u; }
  static float particles_c_pull_scale() { return guest::LoadFloat(0x8200874Cu); }
  static void set_particles_c_pull_scale(float v) { guest::StoreFloat(0x8200874Cu, v); }
  static constexpr uint32_t particles_c_pull_scale_address() { return 0x8200874Cu; }
  // a vector added to the wheels' contacts
  static float contact_offset() { return guest::LoadFloat(0x826C5D40u); }
  static void set_contact_offset(float v) { guest::StoreFloat(0x826C5D40u, v); }
  static constexpr uint32_t contact_offset_address() { return 0x826C5D40u; }
  // a vector added to the trails' points
  static float trail_lift() { return guest::LoadFloat(0x82616F40u); }
  static void set_trail_lift(float v) { guest::StoreFloat(0x82616F40u, v); }
  static constexpr uint32_t trail_lift_address() { return 0x82616F40u; }
  static float smoke_spin_scale() { return guest::LoadFloat(0x8200805Cu); }
  static void set_smoke_spin_scale(float v) { guest::StoreFloat(0x8200805Cu, v); }
  static constexpr uint32_t smoke_spin_scale_address() { return 0x8200805Cu; }
  static float smoke_slip_scale() { return guest::LoadFloat(0x82008060u); }
  static void set_smoke_slip_scale(float v) { guest::StoreFloat(0x82008060u, v); }
  static constexpr uint32_t smoke_slip_scale_address() { return 0x82008060u; }
  static float smoke_spin_most() { return guest::LoadFloat(0x82002C38u); }
  static void set_smoke_spin_most(float v) { guest::StoreFloat(0x82002C38u, v); }
  static constexpr uint32_t smoke_spin_most_address() { return 0x82002C38u; }
  static float smoke_slip_most() { return guest::LoadFloat(0x820029D0u); }
  static void set_smoke_slip_most(float v) { guest::StoreFloat(0x820029D0u, v); }
  static constexpr uint32_t smoke_slip_most_address() { return 0x820029D0u; }
  // ModelSprite::Entry's tables (9 words each)
  static uint32_t model_sprite_kind1() { return guest::LoadU32(0x82616CC4u); }
  static void set_model_sprite_kind1(uint32_t v) { guest::StoreU32(0x82616CC4u, v); }
  static constexpr uint32_t model_sprite_kind1_address() { return 0x82616CC4u; }
  static uint32_t model_sprite_kind2() { return guest::LoadU32(0x82616CE8u); }
  static void set_model_sprite_kind2(uint32_t v) { guest::StoreU32(0x82616CE8u, v); }
  static constexpr uint32_t model_sprite_kind2_address() { return 0x82616CE8u; }
  static uint32_t model_sprite_kind345() { return guest::LoadU32(0x82616D0Cu); }
  static void set_model_sprite_kind345(uint32_t v) { guest::StoreU32(0x82616D0Cu, v); }
  static constexpr uint32_t model_sprite_kind345_address() { return 0x82616D0Cu; }
  static uint32_t model_sprite_kind6() { return guest::LoadU32(0x82616D30u); }
  static void set_model_sprite_kind6(uint32_t v) { guest::StoreU32(0x82616D30u, v); }
  static constexpr uint32_t model_sprite_kind6_address() { return 0x82616D30u; }
  static uint32_t model_sprite_kind7() { return guest::LoadU32(0x82616D54u); }
  static void set_model_sprite_kind7(uint32_t v) { guest::StoreU32(0x82616D54u, v); }
  static constexpr uint32_t model_sprite_kind7_address() { return 0x82616D54u; }
  static uint32_t model_sprite_kind8() { return guest::LoadU32(0x82616D78u); }
  static void set_model_sprite_kind8(uint32_t v) { guest::StoreU32(0x82616D78u, v); }
  static constexpr uint32_t model_sprite_kind8_address() { return 0x82616D78u; }
  static uint32_t model_sprite_other() { return guest::LoadU32(0x826C5CF0u); }
  static void set_model_sprite_other(uint32_t v) { guest::StoreU32(0x826C5CF0u, v); }
  static constexpr uint32_t model_sprite_other_address() { return 0x826C5CF0u; }
};

}  // namespace schema
