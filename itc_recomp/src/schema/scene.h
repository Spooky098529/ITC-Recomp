// The views of docs/functions/scene.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/scene.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: SceneAssets
struct SceneAssets : View {
  using View::View;
  static constexpr uint32_t kSize = 4880;
  // asset 141's load slot (not loaded in set b unless the scene's +5044 is set)
  uint32_t slot_141_address() const { return address + 0; }
  uint32_t xpr_141_address() const { return address + 24; }
  // asset 586's slot
  uint32_t slot_586_address() const { return address + 48; }
  // a slot CarScene::LoadAssets doesn't load
  uint32_t slot_72_address() const { return address + 72; }
  uint32_t xpr_586_address() const { return address + 96; }
  uint32_t xpr_72_address() const { return address + 120; }
  // asset 586's texture provider (the tires', the scene's +208), its vtable word
  template <typename T = TextureProviderVtable>
  T tires_provider_vtable() const { return T(base, guest::LoadU32(base, address + 144)); }
  void set_tires_provider_vtable(const View& v) const { guest::StoreU32(base, address + 144, v.address); }
  void set_tires_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t tires_provider_vtable_address() const { return address + 144; }
  // the +72 slot's texture provider
  template <typename T = TextureProviderVtable>
  T slot_72_provider_vtable() const { return T(base, guest::LoadU32(base, address + 164)); }
  void set_slot_72_provider_vtable(const View& v) const { guest::StoreU32(base, address + 164, v.address); }
  void set_slot_72_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t slot_72_provider_vtable_address() const { return address + 164; }
  // asset 888's slot
  uint32_t slot_888_address() const { return address + 184; }
  uint32_t xpr_888_address() const { return address + 208; }
  // asset 888's texture provider (the calipers', the scene's +296)
  template <typename T = TextureProviderVtable>
  T calipers_provider_vtable() const { return T(base, guest::LoadU32(base, address + 232)); }
  void set_calipers_provider_vtable(const View& v) const { guest::StoreU32(base, address + 232, v.address); }
  void set_calipers_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 232, v); }
  uint32_t calipers_provider_vtable_address() const { return address + 232; }
  // asset 144's slot
  uint32_t slot_144_address() const { return address + 252; }
  uint32_t xpr_144_address() const { return address + 276; }
  // asset 144's texture provider (the scene's +364: the light kits' model +48, SceneLights::Finish)
  template <typename T = TextureProviderVtable>
  T light_kit_provider_vtable() const { return T(base, guest::LoadU32(base, address + 300)); }
  void set_light_kit_provider_vtable(const View& v) const { guest::StoreU32(base, address + 300, v.address); }
  void set_light_kit_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 300, v); }
  uint32_t light_kit_provider_vtable_address() const { return address + 300; }
  // asset 145's slot
  uint32_t slot_145_address() const { return address + 320; }
  uint32_t xpr_145_address() const { return address + 344; }
  // asset 145's texture provider (the scene's +432)
  template <typename T = TextureProviderVtable>
  T provider_145_vtable() const { return T(base, guest::LoadU32(base, address + 368)); }
  void set_provider_145_vtable(const View& v) const { guest::StoreU32(base, address + 368, v.address); }
  void set_provider_145_vtable(uint32_t v) const { guest::StoreU32(base, address + 368, v); }
  uint32_t provider_145_vtable_address() const { return address + 368; }
  // asset 142, the glow model's part file (LoadSlot::Load, the scene's own buffer)
  uint32_t glow_part_file_slot_address() const { return address + 388; }
  // the shared glow model (a part model; +412 its shape count, the assets' +828)
  uint32_t glow_model_address() const { return address + 416; }
  // asset 143, the glow model's textures
  uint32_t glow_textures_slot_address() const { return address + 880; }
  // asset 146, a TGA image
  uint32_t tga_slot_address() const { return address + 904; }
  // its header (CarScene::GetTga copies it)
  uint32_t tga_header_address() const { return address + 928; }
  // its pixels' pages (TGA::CreateImage; freed by FreePagesIn)
  uint32_t tga_pixels() const { return guest::LoadU32(base, address + 948); }
  void set_tga_pixels(uint32_t v) const { guest::StoreU32(base, address + 948, v); }
  uint32_t tga_pixels_address() const { return address + 948; }
  // asset 106's slot
  uint32_t slot_106_address() const { return address + 952; }
  uint32_t xpr_106_address() const { return address + 976; }
  // asset 106's texture provider (the digits', the scene's +1064)
  template <typename T = TextureProviderVtable>
  T digits_a_provider_vtable() const { return T(base, guest::LoadU32(base, address + 1000)); }
  void set_digits_a_provider_vtable(const View& v) const { guest::StoreU32(base, address + 1000, v.address); }
  void set_digits_a_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 1000, v); }
  uint32_t digits_a_provider_vtable_address() const { return address + 1000; }
  // asset 107's slot
  uint32_t slot_107_address() const { return address + 1020; }
  uint32_t xpr_107_address() const { return address + 1044; }
  // asset 107's texture provider (the digits', the scene's +1132)
  template <typename T = TextureProviderVtable>
  T digits_b_provider_vtable() const { return T(base, guest::LoadU32(base, address + 1068)); }
  void set_digits_b_provider_vtable(const View& v) const { guest::StoreU32(base, address + 1068, v.address); }
  void set_digits_b_provider_vtable(uint32_t v) const { guest::StoreU32(base, address + 1068, v); }
  uint32_t digits_b_provider_vtable_address() const { return address + 1068; }
  // four part files' slots, 24 bytes each (assets 129 to 132 in set a, 133 to 136 in set b; LoadSlot::Load)
  uint32_t part_file_slots_address() const { return address + 1088; }
  // their four part models, 464 bytes each
  uint32_t part_models_address() const { return address + 1184; }
  // their textures' slots, 24 bytes each (assets 137 to 140)
  uint32_t part_texture_slots_address() const { return address + 3040; }
  // 19 TGA images' slots, 24 bytes each (assets 110 to 128; probably the license plate's, CarModel::MakePlate)
  uint32_t plate_tga_slots_address() const { return address + 3136; }
  // their pixels' pages, a word each
  uint32_t plate_tga_pixels_address() const { return address + 3592; }
  // their headers, 20 bytes each
  uint32_t plate_tga_headers_address() const { return address + 3668; }
  // asset 108, a part file (set a only)
  uint32_t slot_108_address() const { return address + 4048; }
  // asset 109, its textures (set a only)
  uint32_t slot_109_address() const { return address + 4072; }
  // their part model
  uint32_t model_108_address() const { return address + 4096; }
  // asset 105's slot
  uint32_t slot_105_address() const { return address + 4560; }
  // asset 105's XPR: the number plates' blank texture
  uint32_t plate_xpr_address() const { return address + 4584; }
  // four bundles' slots, 24 bytes each (assets 97 to 100 in set a, 101 to 104 in set b)
  uint32_t bundle_slots_address() const { return address + 4608; }
  // their XPRs, 24 bytes each
  uint32_t bundle_xprs_address() const { return address + 4704; }
  // their texture providers, 20 bytes each (+0 each one's vtable word)
  uint32_t bundle_providers_address() const { return address + 4800; }
};

// embedded; measure: FreePagesIn.r3, CarScene::GetTga.r5
struct ScenePagesWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t pages() const { return guest::LoadU32(base, address + 0); }
  void set_pages(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pages_address() const { return address + 0; }
};

// The image words docs/functions/scene.md's code uses.
struct SceneGlobals {
  // the first set of scene pools (ScenePools::MakeA, scene-loaders.md)
  template <typename T = ScenePools>
  static T first_set(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E40u)); }
  static void set_first_set(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E40u, v.address); }
  static void set_first_set(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E40u, v); }
  static constexpr uint32_t first_set_address() { return 0x826C2E40u; }
  // the car scene (CarScene::Create, 5,536 bytes)
  template <typename T = CarScene>
  static T scene(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C2E44u)); }
  static void set_scene(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C2E44u, v.address); }
  static void set_scene(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C2E44u, v); }
  static constexpr uint32_t scene_address() { return 0x826C2E44u; }
  // 167 (CarScene::Create); the scene's draw copies it to 0x8296B90C
  static uint32_t queue_key_a(uint8_t* base) { return guest::LoadU32(base, 0x82601854u); }
  static void set_queue_key_a(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82601854u, v); }
  static constexpr uint32_t queue_key_a_address() { return 0x82601854u; }
  // 270 (CarScene::Create); copied to 0x8296B900
  static uint32_t queue_key_b(uint8_t* base) { return guest::LoadU32(base, 0x82601858u); }
  static void set_queue_key_b(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82601858u, v); }
  static constexpr uint32_t queue_key_b_address() { return 0x82601858u; }
  // 3 on each CarScene::Update while the scene isn't torn down, down by one a tick (Modes::Tick)
  static uint32_t scene_ticks(uint8_t* base) { return guest::LoadU32(base, 0x8260185Cu); }
  static void set_scene_ticks(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8260185Cu, v); }
  static constexpr uint32_t scene_ticks_address() { return 0x8260185Cu; }
  // 1.0f, as its bits (the image's constant)
  static uint32_t one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // the accessories' scale CarScene::Create gives, a float's bits
  static uint32_t accessories_scale(uint8_t* base) { return guest::LoadU32(base, 0x82002FE8u); }
  static void set_accessories_scale(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82002FE8u, v); }
  static constexpr uint32_t accessories_scale_address() { return 0x82002FE8u; }
};

}  // namespace schema
