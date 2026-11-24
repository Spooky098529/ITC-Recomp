// The views of docs/functions/course-scenery.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/course-scenery.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Scenery, SceneryList, Lights, SceneryList::LoadAll.r4, SceneryList::Add.r4, PoolObject.+3792, SceneryList::Clear.r4, SceneryList::ReleaseUnused.r4
struct Scenery : View {
  using View::View;
  static constexpr uint32_t kSize = 248;
  // 5 (the manager is the pool set: ObjectPools)
  uint32_t pool_count() const { return guest::LoadU32(base, address + 0); }
  void set_pool_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pool_count_address() const { return address + 0; }
  // measure fault: a list table entry's area (SceneryListTableEntry)
  uint16_t table_area() const { return guest::LoadU16(base, address + 0); }
  void set_table_area(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t table_area_address() const { return address + 0; }
  // measure fault: a list table entry's model (SceneryListTableEntry)
  uint16_t table_model() const { return guest::LoadU16(base, address + 2); }
  void set_table_model(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t table_model_address() const { return address + 2; }
  // the pools' block: five 24-byte records, then the objects
  template <typename T = ObjectPoolRecord>
  T pool_records() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_pool_records(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_pool_records(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t pool_records_address() const { return address + 4; }
  // the configuration's +0: 0 everything loaded at once, else streamed (2: [[+40]])
  uint32_t mode() const { return guest::LoadU32(base, address + 8); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t mode_address() const { return address + 8; }
  // the configuration's +4: the views (players) less 1
  uint32_t view_count_minus_1() const { return guest::LoadU32(base, address + 12); }
  void set_view_count_minus_1(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t view_count_minus_1_address() const { return address + 12; }
  // the configuration's +8
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // the configuration's +12
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // the configuration's +16
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the configuration's +20
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // the configuration's +24: -> pool 0's asset id
  template <typename T = SceneryWord>
  T pool0_asset() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_pool0_asset(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_pool0_asset(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t pool0_asset_address() const { return address + 32; }
  // the configuration's +28: the course's table of areas
  template <typename T = SceneryCourseTable>
  T course_table() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_course_table(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_course_table(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t course_table_address() const { return address + 36; }
  // the configuration's +32: -> the mode taken in mode 2 when +12 is 0
  template <typename T = SceneryWord>
  T mode2_source() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_mode2_source(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_mode2_source(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t mode2_source_address() const { return address + 40; }
  // the configuration's +36: pool 0 has an object
  uint32_t pool0_on() const { return guest::LoadU32(base, address + 44); }
  void set_pool0_on(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t pool0_on_address() const { return address + 44; }
  // the configuration's +40: pool 1 is set up
  uint32_t pool1_on() const { return guest::LoadU32(base, address + 48); }
  void set_pool1_on(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t pool1_on_address() const { return address + 48; }
  // 0 by Init
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // 0 by Init (+60 to +107 cleared too)
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  // the areas with a light asset (PoolSizes)
  uint32_t area_count() const { return guest::LoadU32(base, address + 92); }
  void set_area_count(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t area_count_address() const { return address + 92; }
  // the same count again: pool 3's objects and list A's cut when streamed
  uint32_t area_count_streamed() const { return guest::LoadU32(base, address + 96); }
  void set_area_count_streamed(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t area_count_streamed_address() const { return address + 96; }
  // the areas' models (PoolSizes)
  uint32_t model_count() const { return guest::LoadU32(base, address + 100); }
  void set_model_count(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t model_count_address() const { return address + 100; }
  // the model count capped at 16: pool 4's objects and list B's cut when streamed
  uint32_t model_cap() const { return guest::LoadU32(base, address + 104); }
  void set_model_cap(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t model_cap_address() const { return address + 104; }
  // pool 0's object (StartLoad, with +44)
  template <typename T = SceneryBlockObject>
  T pool0_object() const { return T(base, guest::LoadU32(base, address + 108)); }
  void set_pool0_object(const View& v) const { guest::StoreU32(base, address + 108, v.address); }
  void set_pool0_object(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t pool0_object_address() const { return address + 108; }
  // pool 2's object (StartLoad)
  template <typename T = CourseSet>
  T course_set() const { return T(base, guest::LoadU32(base, address + 112)); }
  void set_course_set(const View& v) const { guest::StoreU32(base, address + 112, v.address); }
  void set_course_set(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t course_set_address() const { return address + 112; }
  // list A (the areas; a SceneryItemList at +116): its pool, 3
  uint32_t list_a_pool() const { return guest::LoadU32(base, address + 116); }
  void set_list_a_pool(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t list_a_pool_address() const { return address + 116; }
  uint32_t list_a_done() const { return guest::LoadU32(base, address + 120); }
  void set_list_a_done(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t list_a_done_address() const { return address + 120; }
  uint32_t list_a_count() const { return guest::LoadU32(base, address + 124); }
  void set_list_a_count(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t list_a_count_address() const { return address + 124; }
  uint32_t list_a_capacity() const { return guest::LoadU32(base, address + 128); }
  void set_list_a_capacity(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t list_a_capacity_address() const { return address + 128; }
  // the items' pointers, sorted nearest first
  uint32_t list_a_pointers() const { return guest::LoadU32(base, address + 132); }
  void set_list_a_pointers(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t list_a_pointers_address() const { return address + 132; }
  template <typename T = SceneryListItem>
  T list_a_items() const { return T(base, guest::LoadU32(base, address + 136)); }
  void set_list_a_items(const View& v) const { guest::StoreU32(base, address + 136, v.address); }
  void set_list_a_items(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t list_a_items_address() const { return address + 136; }
  template <typename T = SceneryListTableEntry>
  T list_a_table() const { return T(base, guest::LoadU32(base, address + 140)); }
  void set_list_a_table(const View& v) const { guest::StoreU32(base, address + 140, v.address); }
  void set_list_a_table(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t list_a_table_address() const { return address + 140; }
  // list B (the models; a SceneryItemList at +144): its pool, 4
  uint32_t list_b_pool() const { return guest::LoadU32(base, address + 144); }
  void set_list_b_pool(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t list_b_pool_address() const { return address + 144; }
  uint32_t list_b_done() const { return guest::LoadU32(base, address + 148); }
  void set_list_b_done(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t list_b_done_address() const { return address + 148; }
  uint32_t list_b_count() const { return guest::LoadU32(base, address + 152); }
  void set_list_b_count(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t list_b_count_address() const { return address + 152; }
  uint32_t list_b_capacity() const { return guest::LoadU32(base, address + 156); }
  void set_list_b_capacity(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t list_b_capacity_address() const { return address + 156; }
  uint32_t list_b_pointers() const { return guest::LoadU32(base, address + 160); }
  void set_list_b_pointers(uint32_t v) const { guest::StoreU32(base, address + 160, v); }
  uint32_t list_b_pointers_address() const { return address + 160; }
  template <typename T = SceneryListItem>
  T list_b_items() const { return T(base, guest::LoadU32(base, address + 164)); }
  void set_list_b_items(const View& v) const { guest::StoreU32(base, address + 164, v.address); }
  void set_list_b_items(uint32_t v) const { guest::StoreU32(base, address + 164, v); }
  uint32_t list_b_items_address() const { return address + 164; }
  template <typename T = SceneryListTableEntry>
  T list_b_table() const { return T(base, guest::LoadU32(base, address + 168)); }
  void set_list_b_table(const View& v) const { guest::StoreU32(base, address + 168, v.address); }
  void set_list_b_table(uint32_t v) const { guest::StoreU32(base, address + 168, v); }
  uint32_t list_b_table_address() const { return address + 168; }
  // the pool last asked to load (-1 none)
  uint32_t last_pool() const { return guest::LoadU32(base, address + 172); }
  void set_last_pool(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t last_pool_address() const { return address + 172; }
  // its object's index (-1 none)
  uint32_t last_index() const { return guest::LoadU32(base, address + 176); }
  void set_last_index(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t last_index_address() const { return address + 176; }
  uint32_t pool0_data_size() const { return guest::LoadU32(base, address + 180); }
  void set_pool0_data_size(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t pool0_data_size_address() const { return address + 180; }
  // pool 0's data block (AllocPhysical)
  uint32_t pool0_block() const { return guest::LoadU32(base, address + 184); }
  void set_pool0_block(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t pool0_block_address() const { return address + 184; }
  // the buffer the area models load into ([0x826A5134] or 0)
  uint32_t load_buffer() const { return guest::LoadU32(base, address + 188); }
  void set_load_buffer(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t load_buffer_address() const { return address + 188; }
  // set while Scenery::IsLoaded loads everything
  uint32_t loading_all() const { return guest::LoadU32(base, address + 192); }
  void set_loading_all(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t loading_all_address() const { return address + 192; }
  // 0x38435: camera pass bits, 0x10 the set's model, 0x20 its list, 0x400 the areas' lights drawn, 0x4000 the texture animations stopped
  uint32_t mask() const { return guest::LoadU32(base, address + 196); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t mask_address() const { return address + 196; }
  // 192.0 (0x820067B8)
  float unknown_200() const { return guest::LoadFloat(base, address + 200); }
  void set_unknown_200(float v) const { guest::StoreFloat(base, address + 200, v); }
  uint32_t unknown_200_address() const { return address + 200; }
  // 72.0 (0x820067B4)
  float unknown_204() const { return guest::LoadFloat(base, address + 204); }
  void set_unknown_204(float v) const { guest::StoreFloat(base, address + 204, v); }
  uint32_t unknown_204_address() const { return address + 204; }
  // 256.0 (0x82001FAC)
  float unknown_208() const { return guest::LoadFloat(base, address + 208); }
  void set_unknown_208(float v) const { guest::StoreFloat(base, address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  // 64.0 (0x820067B0)
  float unknown_212() const { return guest::LoadFloat(base, address + 212); }
  void set_unknown_212(float v) const { guest::StoreFloat(base, address + 212, v); }
  uint32_t unknown_212_address() const { return address + 212; }
  // the visible areas (Scenery::Update)
  uint32_t visible_count() const { return guest::LoadU32(base, address + 216); }
  void set_visible_count(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t visible_count_address() const { return address + 216; }
  uint32_t visible_capacity() const { return guest::LoadU32(base, address + 220); }
  void set_visible_capacity(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t visible_capacity_address() const { return address + 220; }
  // 272-byte entries
  template <typename T = SceneryVisibleArea>
  T visible_areas() const { return T(base, guest::LoadU32(base, address + 224)); }
  void set_visible_areas(const View& v) const { guest::StoreU32(base, address + 224, v.address); }
  void set_visible_areas(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t visible_areas_address() const { return address + 224; }
  // the TextureProvider made here (20 bytes, 64 slots); this word is its vtable 0x8206CCB4
  template <typename T = SceneryProviderVtable>
  T texture_provider() const { return T(base, guest::LoadU32(base, address + 228)); }
  void set_texture_provider(const View& v) const { guest::StoreU32(base, address + 228, v.address); }
  void set_texture_provider(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t texture_provider_address() const { return address + 228; }
};

// embedded; measure: SceneryList::Sort.r4
struct SceneryItemList : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 3 (areas) or 4 (models)
  uint32_t pool() const { return guest::LoadU32(base, address + 0); }
  void set_pool(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pool_address() const { return address + 0; }
  // the items done (LoadAll)
  uint32_t done() const { return guest::LoadU32(base, address + 4); }
  void set_done(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t done_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(base, address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_address() const { return address + 8; }
  // also the table's count
  uint32_t capacity() const { return guest::LoadU32(base, address + 12); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t capacity_address() const { return address + 12; }
  // an array of pointers to the items, sorted nearest first
  uint32_t pointers() const { return guest::LoadU32(base, address + 16); }
  void set_pointers(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t pointers_address() const { return address + 16; }
  template <typename T = SceneryListItem>
  T items() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_items(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_items(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t items_address() const { return address + 20; }
  template <typename T = SceneryListTableEntry>
  T table() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_table(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_table(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t table_address() const { return address + 24; }
};

// allocated; measure: Scenery.+4, SceneryList::Add.r4.+20
struct SceneryListItem : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float distance() const { return guest::LoadFloat(base, address + 0); }
  void set_distance(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t distance_address() const { return address + 0; }
  // the camera passes that see it
  uint32_t mask() const { return guest::LoadU32(base, address + 4); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t mask_address() const { return address + 4; }
  // the area and model halfwords as one word
  uint32_t key() const { return guest::LoadU32(base, address + 8); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t key_address() const { return address + 8; }
  uint16_t area() const { return guest::LoadU16(base, address + 8); }
  void set_area(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t area_address() const { return address + 8; }
  uint16_t model() const { return guest::LoadU16(base, address + 10); }
  void set_model(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t model_address() const { return address + 10; }
  // its pool's object, 0 while not loaded
  template <typename T = PoolObject>
  T object() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_object(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t object_address() const { return address + 12; }
};

// allocated; measure: SceneryList::LoadAll.r4.+24, Scenery@+8.+24
struct SceneryListTableEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the area and model halfwords as one word (also PoolSizes' pool 0 asset word: a measure fault)
  uint32_t key() const { return guest::LoadU32(base, address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
  uint16_t area() const { return guest::LoadU16(base, address + 0); }
  void set_area(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t area_address() const { return address + 0; }
  uint16_t model() const { return guest::LoadU16(base, address + 2); }
  void set_model(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t model_address() const { return address + 2; }
  // the asset id
  uint32_t asset() const { return guest::LoadU32(base, address + 4); }
  void set_asset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t asset_address() const { return address + 4; }
  // mode 0: its object once loaded
  template <typename T = PoolObject>
  T object() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_object(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t object_address() const { return address + 8; }
  // the next entry's object (LoadAll's original reads it 12 bytes on)
  uint32_t next_object() const { return guest::LoadU32(base, address + 20); }
  void set_next_object(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t next_object_address() const { return address + 20; }
};

// file; measure: Scenery.+32, Scenery.+40
struct SceneryWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // pool 0's asset id, or the mode in mode 2
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// file; measure: Scenery@+8
struct SceneryConfig : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // 0 everything loaded at once, else streamed
  uint32_t mode() const { return guest::LoadU32(base, address + 0); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mode_address() const { return address + 0; }
  uint32_t view_count_minus_1() const { return guest::LoadU32(base, address + 4); }
  void set_view_count_minus_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t view_count_minus_1_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  template <typename T = SceneryWord>
  T pool0_asset() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_pool0_asset(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_pool0_asset(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t pool0_asset_address() const { return address + 24; }
  template <typename T = SceneryCourseTable>
  T course_table() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_course_table(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_course_table(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t course_table_address() const { return address + 28; }
  template <typename T = SceneryWord>
  T mode2_source() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_mode2_source(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_mode2_source(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t mode2_source_address() const { return address + 32; }
  uint32_t pool0_on() const { return guest::LoadU32(base, address + 36); }
  void set_pool0_on(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t pool0_on_address() const { return address + 36; }
  uint32_t pool1_on() const { return guest::LoadU32(base, address + 40); }
  void set_pool1_on(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t pool1_on_address() const { return address + 40; }
};

// file; measure: Scenery.+36, Scenery@+8.+28
struct SceneryCourseTable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the course set's second asset (into its data; its size is pool 2's)
  uint32_t set_second_asset() const { return guest::LoadU32(base, address + 8); }
  void set_set_second_asset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t set_second_asset_address() const { return address + 8; }
  // the course set's first asset (into a physical block)
  uint32_t set_first_asset() const { return guest::LoadU32(base, address + 12); }
  void set_set_first_asset(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t set_first_asset_address() const { return address + 12; }
  uint32_t area_count() const { return guest::LoadU32(base, address + 16); }
  void set_area_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t area_count_address() const { return address + 16; }
  // area 0's light asset (area n's 12 n on; -1 ends them)
  uint32_t light_asset() const { return guest::LoadU32(base, address + 20); }
  void set_light_asset(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t light_asset_address() const { return address + 20; }
  // area 0's first model asset
  uint32_t first_model_asset() const { return guest::LoadU32(base, address + 24); }
  void set_first_model_asset(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t first_model_asset_address() const { return address + 24; }
  // area 0's last model asset
  uint32_t last_model_asset() const { return guest::LoadU32(base, address + 28); }
  void set_last_model_asset(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t last_model_asset_address() const { return address + 28; }
};

// embedded; measure: ObjectPools
struct ObjectPools : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the records (the original: one XPhysicalAlloc block, the records, then the objects; since E5 step 5 a host array, the objects' block apart)
  template <typename T = ObjectPoolRecord>
  T records() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_records(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_records(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t records_address() const { return address + 4; }
};

// allocated; measure: new<?>@ObjectPools::Init, ObjectPools::Pool.ret
#pragma pack(push, 1)
struct ObjectPoolRecordData {
  uint32_t object_count = 0;  // +0
  uint32_t object_size = 0;  // +4
  uint32_t data_size = 0;  // +8
  uint32_t first_object = 0;  // +12
  uint32_t free_head = 0;  // +16
  uint32_t used_head = 0;  // +20
};
#pragma pack(pop)
static_assert(sizeof(ObjectPoolRecordData) == 24);
inline constexpr memory::LayoutField kObjectPoolRecordFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}};
inline constexpr memory::Layout kObjectPoolRecordLayout = {kObjectPoolRecordFields, 6};

struct ObjectPoolRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ObjectPoolRecordData* data() const { return static_cast<ObjectPoolRecordData*>(memory::HandleObject(address)); }
  static ObjectPoolRecord New(uint8_t* base) {
    return ObjectPoolRecord(base, memory::MakeHandle(new ObjectPoolRecordData(), sizeof(ObjectPoolRecordData), "ObjectPoolRecord", &kObjectPoolRecordLayout));
  }
  template <uint32_t Stride = kSize>
  static ObjectPoolRecord NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ObjectPoolRecordData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ObjectPoolRecordData*>(::operator new(sizeof(ObjectPoolRecordData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ObjectPoolRecordData();
    return ObjectPoolRecord(base, memory::MakeHandle(p, sizeof(ObjectPoolRecordData) * count, "ObjectPoolRecord", &kObjectPoolRecordLayout, sizeof(ObjectPoolRecordData)));
  }
  ObjectPoolRecord at(uint32_t i) const { return ObjectPoolRecord(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ObjectPoolRecordData* direct() const { return static_cast<ObjectPoolRecordData*>(memory::HandleOwn(address, &kObjectPoolRecordLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kObjectPoolRecordLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kObjectPoolRecordLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t object_count_address() const { return address + 0; }
  uint32_t object_count_bits() const { if (auto* d = direct()) return d->object_count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_object_count_bits(uint32_t v) const { if (auto* d = direct()) { d->object_count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t object_count() const { if (auto* d = direct()) return d->object_count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_object_count(uint32_t v) const { if (auto* d = direct()) { d->object_count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t object_size_address() const { return address + 4; }
  uint32_t object_size_bits() const { if (auto* d = direct()) return d->object_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_object_size_bits(uint32_t v) const { if (auto* d = direct()) { d->object_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t object_size() const { if (auto* d = direct()) return d->object_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_object_size(uint32_t v) const { if (auto* d = direct()) { d->object_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t data_size_address() const { return address + 8; }
  uint32_t data_size_bits() const { if (auto* d = direct()) return d->data_size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_data_size_bits(uint32_t v) const { if (auto* d = direct()) { d->data_size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t data_size() const { if (auto* d = direct()) return d->data_size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_data_size(uint32_t v) const { if (auto* d = direct()) { d->data_size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t first_object_address() const { return address + 12; }
  uint32_t first_object_bits() const { if (auto* d = direct()) return d->first_object; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_first_object_bits(uint32_t v) const { if (auto* d = direct()) { d->first_object = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  template <typename T = PoolObject>
  T first_object() const { return T(base, first_object_bits()); }
  void set_first_object(const View& w) const { set_first_object_bits(w.address); }
  void set_first_object(uint32_t v) const { set_first_object_bits(v); }
  uint32_t free_head_address() const { return address + 16; }
  uint32_t free_head_bits() const { if (auto* d = direct()) return d->free_head; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_free_head_bits(uint32_t v) const { if (auto* d = direct()) { d->free_head = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  template <typename T = PoolObject>
  T free_head() const { return T(base, free_head_bits()); }
  void set_free_head(const View& w) const { set_free_head_bits(w.address); }
  void set_free_head(uint32_t v) const { set_free_head_bits(v); }
  uint32_t used_head_address() const { return address + 20; }
  uint32_t used_head_bits() const { if (auto* d = direct()) return d->used_head; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_used_head_bits(uint32_t v) const { if (auto* d = direct()) { d->used_head = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  template <typename T = PoolObject>
  T used_head() const { return T(base, used_head_bits()); }
  void set_used_head(const View& w) const { set_used_head_bits(w.address); }
  void set_used_head(uint32_t v) const { set_used_head_bits(v); }
};

// stack; measure: ObjectPools::Init.r5
struct ObjectPoolDesc : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t pool() const { return guest::LoadU32(base, address + 0); }
  void set_pool(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pool_address() const { return address + 0; }
  uint32_t object_size() const { return guest::LoadU32(base, address + 4); }
  void set_object_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_size_address() const { return address + 4; }
  uint32_t data_size() const { return guest::LoadU32(base, address + 8); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_size_address() const { return address + 8; }
  uint32_t object_count() const { return guest::LoadU32(base, address + 12); }
  void set_object_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t object_count_address() const { return address + 12; }
  uint32_t data_count() const { return guest::LoadU32(base, address + 16); }
  void set_data_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t data_count_address() const { return address + 16; }
  // makes each object (Scenery::MakePoolObject), 0 plain pool objects
  uint32_t factory() const { return guest::LoadU32(base, address + 20); }
  void set_factory(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t factory_address() const { return address + 20; }
};

// allocated; measure: PoolObject, ObjectPools::Pool.ret.+20, ObjectPools::Release.ret, PoolObject::AllocData.r3
struct PoolObject : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  static constexpr uint32_t kVtable = 0x82006848;
  template <typename T = PoolObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its data buffer (+4 to +12 AllocData's record: the data, its size, the used)
  uint32_t data() const { return guest::LoadU32(base, address + 4); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t data_address() const { return address + 4; }
  uint32_t data_size() const { return guest::LoadU32(base, address + 8); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_size_address() const { return address + 8; }
  uint32_t data_used() const { return guest::LoadU32(base, address + 12); }
  void set_data_used(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t data_used_address() const { return address + 12; }
  // its index in the pool
  uint32_t index() const { return guest::LoadU32(base, address + 16); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t index_address() const { return address + 16; }
  // on the pool's free or used list
  template <typename T = PoolObject>
  T prev() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_prev(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_prev(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t prev_address() const { return address + 20; }
  template <typename T = PoolObject>
  T next() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t next_address() const { return address + 24; }
  // 0 by the constructor
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  // 0x1 in use, 0x2 cleared when loaded, 0x8 loaded, 0x10 seen this update, 0x20 not seen, to be released
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // the updates left before release (2; -1 none)
  uint32_t countdown() const { return guest::LoadU32(base, address + 36); }
  void set_countdown(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t countdown_address() const { return address + 36; }
  // CourseSet's area_file
  template <typename T = SceneryDataFile>
  T set_area_file() const { return T(base, guest::LoadU32(base, address + 1056)); }
  void set_set_area_file(const View& v) const { guest::StoreU32(base, address + 1056, v.address); }
  void set_set_area_file(uint32_t v) const { guest::StoreU32(base, address + 1056, v); }
  uint32_t set_area_file_address() const { return address + 1056; }
  // CourseSet's texture_animations
  template <typename T = AnimationSet>
  T set_texture_animations() const { return T(base, guest::LoadU32(base, address + 1060)); }
  void set_set_texture_animations(const View& v) const { guest::StoreU32(base, address + 1060, v.address); }
  void set_set_texture_animations(uint32_t v) const { guest::StoreU32(base, address + 1060, v); }
  uint32_t set_texture_animations_address() const { return address + 1060; }
  // CourseSet's light_groups
  template <typename T = SceneryDataFile>
  T set_light_groups() const { return T(base, guest::LoadU32(base, address + 1592)); }
  void set_set_light_groups(const View& v) const { guest::StoreU32(base, address + 1592, v.address); }
  void set_set_light_groups(uint32_t v) const { guest::StoreU32(base, address + 1592, v); }
  uint32_t set_light_groups_address() const { return address + 1592; }
  // CourseSet's list_c_count
  uint32_t set_list_c_count() const { return guest::LoadU32(base, address + 1596); }
  void set_set_list_c_count(uint32_t v) const { guest::StoreU32(base, address + 1596, v); }
  uint32_t set_list_c_count_address() const { return address + 1596; }
  // CourseSet's list_c_head
  uint16_t set_list_c_head() const { return guest::LoadU16(base, address + 1860); }
  void set_set_list_c_head(uint16_t v) const { guest::StoreU16(base, address + 1860, v); }
  uint32_t set_list_c_head_address() const { return address + 1860; }
  uint32_t set_list_d_count() const { return guest::LoadU32(base, address + 2124); }
  void set_set_list_d_count(uint32_t v) const { guest::StoreU32(base, address + 2124, v); }
  uint32_t set_list_d_count_address() const { return address + 2124; }
  uint16_t set_list_d_head() const { return guest::LoadU16(base, address + 2388); }
  void set_set_list_d_head(uint16_t v) const { guest::StoreU16(base, address + 2388, v); }
  uint32_t set_list_d_head_address() const { return address + 2388; }
  uint32_t set_list_e_count() const { return guest::LoadU32(base, address + 2652); }
  void set_set_list_e_count(uint32_t v) const { guest::StoreU32(base, address + 2652, v); }
  uint32_t set_list_e_count_address() const { return address + 2652; }
  uint16_t set_list_e_head() const { return guest::LoadU16(base, address + 2916); }
  void set_set_list_e_head(uint16_t v) const { guest::StoreU16(base, address + 2916, v); }
  uint32_t set_list_e_head_address() const { return address + 2916; }
  // CourseSet's points_animations
  uint32_t set_points_animations() const { return guest::LoadU32(base, address + 3788); }
  void set_set_points_animations(uint32_t v) const { guest::StoreU32(base, address + 3788, v); }
  uint32_t set_points_animations_address() const { return address + 3788; }
  uint32_t set_points_provider() const { return guest::LoadU32(base, address + 3792); }
  void set_set_points_provider(uint32_t v) const { guest::StoreU32(base, address + 3792, v); }
  uint32_t set_points_provider_address() const { return address + 3792; }
  uint32_t set_points_race_word() const { return guest::LoadU32(base, address + 3796); }
  void set_set_points_race_word(uint32_t v) const { guest::StoreU32(base, address + 3796, v); }
  uint32_t set_points_race_word_address() const { return address + 3796; }
  uint32_t set_points_count() const { return guest::LoadU32(base, address + 3800); }
  void set_set_points_count(uint32_t v) const { guest::StoreU32(base, address + 3800, v); }
  uint32_t set_points_count_address() const { return address + 3800; }
};

// image; measure: PoolObject.+0
struct PoolObjectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the key's address and size
  uint32_t key() const { return guest::LoadU32(base, address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
  uint32_t load() const { return guest::LoadU32(base, address + 4); }
  void set_load(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t load_address() const { return address + 4; }
  uint32_t reset() const { return guest::LoadU32(base, address + 8); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t reset_address() const { return address + 8; }
};

// allocated; measure: -
struct SceneryBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static SceneryBlock New(uint8_t* base, uint32_t size) { return SceneryBlock(base, memory::NewBlob(size, "SceneryBlock")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// allocated; measure: -
struct ObjectPoolsBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static ObjectPoolsBlock New(uint8_t* base, uint32_t size) { return ObjectPoolsBlock(base, memory::NewBlob(size, "ObjectPoolsBlock")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// allocated; measure: sub_820F1098.r3
struct SceneryBlockObject : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  static constexpr uint32_t kVtable = 0x8200674C;
  // 0x8200674C pool 0's, 0x82006758 pool 1's
  template <typename T = PoolObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 by MakePoolObject; pool 0's load sub_820F1098 sets it
  uint32_t block() const { return guest::LoadU32(base, address + 56); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t block_address() const { return address + 56; }
};

// allocated; measure: CourseSet, CourseSet::OnLoaded.r6, TextureAnimations
struct CourseSet : View {
  using View::View;
  static constexpr uint32_t kSize = 3824;
  static constexpr uint32_t kVtable = 0x82006784;
  template <typename T = PoolObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its textures (an XPR; CourseSet::InitParts' +0)
  uint32_t xpr_address() const { return address + 64; }
  // its model (a Model)
  uint32_t model_address() const { return address + 96; }
  // a ModelList (entry 2)
  uint32_t list_a_address() const { return address + 528; }
  // the second file's entry 0: the areas
  template <typename T = SceneryDataFile>
  T area_file() const { return T(base, guest::LoadU32(base, address + 1056)); }
  void set_area_file(const View& v) const { guest::StoreU32(base, address + 1056, v.address); }
  void set_area_file(uint32_t v) const { guest::StoreU32(base, address + 1056, v); }
  uint32_t area_file_address() const { return address + 1056; }
  // the second file's entry 1
  template <typename T = AnimationSet>
  T texture_animations() const { return T(base, guest::LoadU32(base, address + 1060)); }
  void set_texture_animations(const View& v) const { guest::StoreU32(base, address + 1060, v.address); }
  void set_texture_animations(uint32_t v) const { guest::StoreU32(base, address + 1060, v); }
  uint32_t texture_animations_address() const { return address + 1060; }
  // a ModelList (entry 3)
  uint32_t list_b_address() const { return address + 1064; }
  // the second file's entry 2
  template <typename T = SceneryDataFile>
  T light_groups() const { return T(base, guest::LoadU32(base, address + 1592)); }
  void set_light_groups(const View& v) const { guest::StoreU32(base, address + 1592, v.address); }
  void set_light_groups(uint32_t v) const { guest::StoreU32(base, address + 1592, v); }
  uint32_t light_groups_address() const { return address + 1592; }
  // a ModelList (entry 4), drawn whole by DrawSetLists
  uint32_t list_c_count() const { return guest::LoadU32(base, address + 1596); }
  void set_list_c_count(uint32_t v) const { guest::StoreU32(base, address + 1596, v); }
  uint32_t list_c_count_address() const { return address + 1596; }
  // its List64's head (+264)
  uint16_t list_c_head() const { return guest::LoadU16(base, address + 1860); }
  void set_list_c_head(uint16_t v) const { guest::StoreU16(base, address + 1860, v); }
  uint32_t list_c_head_address() const { return address + 1860; }
  // a ModelList (entry 5), its B entries
  uint32_t list_d_count() const { return guest::LoadU32(base, address + 2124); }
  void set_list_d_count(uint32_t v) const { guest::StoreU32(base, address + 2124, v); }
  uint32_t list_d_count_address() const { return address + 2124; }
  uint16_t list_d_head() const { return guest::LoadU16(base, address + 2388); }
  void set_list_d_head(uint16_t v) const { guest::StoreU16(base, address + 2388, v); }
  uint32_t list_d_head_address() const { return address + 2388; }
  // a ModelList (entry 6), its C entries
  uint32_t list_e_count() const { return guest::LoadU32(base, address + 2652); }
  void set_list_e_count(uint32_t v) const { guest::StoreU32(base, address + 2652, v); }
  uint32_t list_e_count_address() const { return address + 2652; }
  uint16_t list_e_head() const { return guest::LoadU16(base, address + 2916); }
  void set_list_e_head(uint16_t v) const { guest::StoreU16(base, address + 2916, v); }
  uint32_t list_e_head_address() const { return address + 2916; }
  // a point list (entry 7, ModelList::LoadPoints)
  uint32_t points_address() const { return address + 3180; }
  // the point list's +608: the set's texture animations
  uint32_t points_animations() const { return guest::LoadU32(base, address + 3788); }
  void set_points_animations(uint32_t v) const { guest::StoreU32(base, address + 3788, v); }
  uint32_t points_animations_address() const { return address + 3788; }
  // its +612: the texture provider
  uint32_t points_provider() const { return guest::LoadU32(base, address + 3792); }
  void set_points_provider(uint32_t v) const { guest::StoreU32(base, address + 3792, v); }
  uint32_t points_provider_address() const { return address + 3792; }
  // its +616: from the race's table at [0x826D82E8]
  uint32_t points_race_word() const { return guest::LoadU32(base, address + 3796); }
  void set_points_race_word(uint32_t v) const { guest::StoreU32(base, address + 3796, v); }
  uint32_t points_race_word_address() const { return address + 3796; }
  // its +620
  uint32_t points_count() const { return guest::LoadU32(base, address + 3800); }
  void set_points_count(uint32_t v) const { guest::StoreU32(base, address + 3800, v); }
  uint32_t points_count_address() const { return address + 3800; }
  // the first asset's block (AllocPhysical), freed once parsed
  uint32_t first_file() const { return guest::LoadU32(base, address + 3808); }
  void set_first_file(uint32_t v) const { guest::StoreU32(base, address + 3808, v); }
  uint32_t first_file_address() const { return address + 3808; }
  // the second asset, in the object's data
  uint32_t second_file() const { return guest::LoadU32(base, address + 3812); }
  void set_second_file(uint32_t v) const { guest::StoreU32(base, address + 3812, v); }
  uint32_t second_file_address() const { return address + 3812; }
};

// allocated; measure: AreaLights, Scenery.+12, Lights.+224.+12
struct AreaLights : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  static constexpr uint32_t kVtable = 0x8200676C;
  template <typename T = PoolObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the header's data: the loaded file
  uint32_t data() const { return guest::LoadU32(base, address + 4); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t data_address() const { return address + 4; }
  // an XPR (MakePoolObject)
  uint32_t xpr_address() const { return address + 56; }
  // the file's entry 0 (0 none): its light groups
  template <typename T = SceneryDataFile>
  T light_file() const { return T(base, guest::LoadU32(base, address + 80)); }
  void set_light_file(const View& v) const { guest::StoreU32(base, address + 80, v.address); }
  void set_light_file(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t light_file_address() const { return address + 80; }
  // the key: the area and model halfwords
  uint32_t key() const { return guest::LoadU32(base, address + 84); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t key_address() const { return address + 84; }
  uint16_t area() const { return guest::LoadU16(base, address + 84); }
  void set_area(uint16_t v) const { guest::StoreU16(base, address + 84, v); }
  uint32_t area_address() const { return address + 84; }
  uint16_t model() const { return guest::LoadU16(base, address + 86); }
  void set_model(uint16_t v) const { guest::StoreU16(base, address + 86, v); }
  uint32_t model_address() const { return address + 86; }
};

// allocated; measure: AreaModel, Scenery.+16
struct AreaModel : View {
  using View::View;
  static constexpr uint32_t kSize = 560;
  static constexpr uint32_t kVtable = 0x82006778;
  template <typename T = PoolObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // its Model (+80) once parsed, else 0
  View model() const { return View(base, guest::LoadU32(base, address + 56)); }
  void set_model(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t model_address() const { return address + 56; }
  // its XPR (+528) once parsed, else 0
  View xpr() const { return View(base, guest::LoadU32(base, address + 60)); }
  void set_xpr(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t xpr_address() const { return address + 60; }
  // the key: the area and model halfwords
  uint32_t key() const { return guest::LoadU32(base, address + 64); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t key_address() const { return address + 64; }
  uint16_t area() const { return guest::LoadU16(base, address + 64); }
  void set_area(uint16_t v) const { guest::StoreU16(base, address + 64, v); }
  uint32_t area_address() const { return address + 64; }
  uint16_t model_number() const { return guest::LoadU16(base, address + 66); }
  void set_model_number(uint16_t v) const { guest::StoreU16(base, address + 66, v); }
  uint32_t model_number_address() const { return address + 66; }
  // the Model made here (448 bytes); this word its vtable 0x82006764
  uint32_t own_model() const { return guest::LoadU32(base, address + 80); }
  void set_own_model(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t own_model_address() const { return address + 80; }
  // the XPR made here
  uint32_t own_xpr_address() const { return address + 528; }
};

// allocated; measure: Lights.+224
struct SceneryVisibleArea : View {
  using View::View;
  static constexpr uint32_t kSize = 272;
  // the camera passes that see it
  uint32_t mask() const { return guest::LoadU32(base, address + 0); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mask_address() const { return address + 0; }
  uint32_t model_count() const { return guest::LoadU32(base, address + 4); }
  void set_model_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t model_count_address() const { return address + 4; }
  // the manager's model count
  uint32_t model_capacity() const { return guest::LoadU32(base, address + 8); }
  void set_model_capacity(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t model_capacity_address() const { return address + 8; }
  template <typename T = AreaLights>
  T lights() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_lights(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_lights(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t lights_address() const { return address + 12; }
  // the first of its models (4 bytes each)
  template <typename T = AreaModel>
  T models() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_models(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_models(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t models_address() const { return address + 16; }
};

// file; measure: PoolObject.+1056, PoolObject.+1592, Scenery.+12.+80, Lights.+224.+12.+80
struct SceneryDataFile : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // little-endian
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // its first byte
  uint8_t count_byte() const { return guest::LoadU8(base, address + 0); }
  void set_count_byte(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t count_byte_address() const { return address + 0; }
};

// file; measure: DataFile::GetEntry.ret
struct SceneryFileEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 148;
  // an area's model count (big-endian), a light group's scans
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // an area's four 16-byte corners
  uint32_t corners_address() const { return address + 32; }
  // a light group's lights
  uint32_t light_count() const { return guest::LoadU32(base, address + 60); }
  void set_light_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t light_count_address() const { return address + 60; }
  // a light group's first light (48 bytes each)
  float light_x() const { return guest::LoadFloat(base, address + 76); }
  void set_light_x(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t light_x_address() const { return address + 76; }
  uint64_t light_xy() const { return guest::LoadU64(base, address + 76); }
  void set_light_xy(uint64_t v) const { guest::StoreU64(base, address + 76, v); }
  uint32_t light_xy_address() const { return address + 76; }
  float light_y() const { return guest::LoadFloat(base, address + 80); }
  void set_light_y(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t light_y_address() const { return address + 80; }
  float light_z() const { return guest::LoadFloat(base, address + 84); }
  void set_light_z(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t light_z_address() const { return address + 84; }
  uint64_t light_zw() const { return guest::LoadU64(base, address + 84); }
  void set_light_zw(uint64_t v) const { guest::StoreU64(base, address + 84, v); }
  uint32_t light_zw_address() const { return address + 84; }
  // its bytes 16 to 23
  uint64_t light_16() const { return guest::LoadU64(base, address + 92); }
  void set_light_16(uint64_t v) const { guest::StoreU64(base, address + 92, v); }
  uint32_t light_16_address() const { return address + 92; }
  // an area's first model record's number (96-byte records)
  uint32_t model_number() const { return guest::LoadU32(base, address + 96); }
  void set_model_number(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t model_number_address() const { return address + 96; }
  // its bytes 24 to 31
  uint64_t light_24() const { return guest::LoadU64(base, address + 100); }
  void set_light_24(uint64_t v) const { guest::StoreU64(base, address + 100, v); }
  uint32_t light_24_address() const { return address + 100; }
};

// image; measure: Scenery::CollectAreas.r5, Scenery::CollectModels.r5
struct SceneryViewport : View {
  using View::View;
  static constexpr uint32_t kSize = 1612;
  float far_distance() const { return guest::LoadFloat(base, address + 72); }
  void set_far_distance(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t far_distance_address() const { return address + 72; }
  float eye_x() const { return guest::LoadFloat(base, address + 1600); }
  void set_eye_x(float v) const { guest::StoreFloat(base, address + 1600, v); }
  uint32_t eye_x_address() const { return address + 1600; }
  float eye_z() const { return guest::LoadFloat(base, address + 1608); }
  void set_eye_z(float v) const { guest::StoreFloat(base, address + 1608, v); }
  uint32_t eye_z_address() const { return address + 1608; }
};

// stack; measure: Scenery::DrawArea.r4, Scenery::DrawSetLists.r4, Scenery::RecordHeadlightProjectors.r4
struct SceneryPassDesc : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // the camera pass (RecordHeadlightProjectors sets it)
  uint32_t camera_pass_object() const { return guest::LoadU32(base, address + 8); }
  void set_camera_pass_object(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t camera_pass_object_address() const { return address + 8; }
  uint32_t player() const { return guest::LoadU32(base, address + 12); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t player_address() const { return address + 12; }
  uint32_t viewport_entry() const { return guest::LoadU32(base, address + 16); }
  void set_viewport_entry(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t viewport_entry_address() const { return address + 16; }
  // the camera pass's number
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 20); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t camera_pass_address() const { return address + 20; }
  // to [0x8296B90C]
  uint32_t key_a() const { return guest::LoadU32(base, address + 24); }
  void set_key_a(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t key_a_address() const { return address + 24; }
  // to [0x8296B900]
  uint32_t key_b() const { return guest::LoadU32(base, address + 28); }
  void set_key_b(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t key_b_address() const { return address + 28; }
  uint32_t mask() const { return guest::LoadU32(base, address + 32); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t mask_address() const { return address + 32; }
};

// image; measure: -
struct SceneryPassEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // the viewport ([[0x826C2CE8] + 4 x it])
  uint32_t player() const { return guest::LoadU32(base, address + 0); }
  void set_player(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t player_address() const { return address + 0; }
  // the viewport's 80-byte entry
  uint32_t viewport_entry() const { return guest::LoadU32(base, address + 4); }
  void set_viewport_entry(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t viewport_entry_address() const { return address + 4; }
  // the camera pass's number (0x826BAC00 + 592 x it)
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 8); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t camera_pass_address() const { return address + 8; }
  uint32_t key_a() const { return guest::LoadU32(base, address + 12); }
  void set_key_a(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t key_a_address() const { return address + 12; }
  uint32_t key_b() const { return guest::LoadU32(base, address + 16); }
  void set_key_b(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t key_b_address() const { return address + 16; }
  uint32_t mask() const { return guest::LoadU32(base, address + 20); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t mask_address() const { return address + 20; }
};

// image; measure: -
struct SceneryViewMask : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the view (also the viewport's player for CameraPass::FromViewport)
  uint32_t view() const { return guest::LoadU32(base, address + 0); }
  void set_view(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t view_address() const { return address + 0; }
  uint32_t viewport_entry() const { return guest::LoadU32(base, address + 4); }
  void set_viewport_entry(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t viewport_entry_address() const { return address + 4; }
  // the camera pass's number
  uint32_t camera_pass() const { return guest::LoadU32(base, address + 8); }
  void set_camera_pass(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t camera_pass_address() const { return address + 8; }
  uint32_t bits() const { return guest::LoadU32(base, address + 12); }
  void set_bits(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t bits_address() const { return address + 12; }
};

// stack; measure: Lights::FindNearest.r4
struct NearestLight : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  // the float at 0x82000A30
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
  // the light's bytes 16 to 31
  float f16() const { return guest::LoadFloat(base, address + 16); }
  void set_f16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t f16_address() const { return address + 16; }
  float f20() const { return guest::LoadFloat(base, address + 20); }
  void set_f20(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t f20_address() const { return address + 20; }
  float f24() const { return guest::LoadFloat(base, address + 24); }
  void set_f24(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t f24_address() const { return address + 24; }
  float f28() const { return guest::LoadFloat(base, address + 28); }
  void set_f28(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t f28_address() const { return address + 28; }
};

// stack; measure: Lights::FindNearest.r5
struct NearestLightPoint : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// image; measure: Scenery.+228
struct SceneryProviderVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // slot 2 (0x82594328)
  uint32_t clear_all() const { return guest::LoadU32(base, address + 8); }
  void set_clear_all(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t clear_all_address() const { return address + 8; }
  // slot 4, TextureProvider::Set (0x82594718)
  uint32_t set() const { return guest::LoadU32(base, address + 16); }
  void set_set(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t set_address() const { return address + 16; }
};

// file; measure: AnimationSet
struct AnimationSet : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the offsets, from the table's end (set + 4 (count + 1))
  uint32_t first_offset() const { return guest::LoadU32(base, address + 4); }
  void set_first_offset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_offset_address() const { return address + 4; }
};

// file; measure: Animation
struct Animation : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t frame_count() const { return guest::LoadU32(base, address + 0); }
  void set_frame_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t frame_count_address() const { return address + 0; }
  uint32_t frame() const { return guest::LoadU32(base, address + 4); }
  void set_frame(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t frame_address() const { return address + 4; }
  uint32_t tick() const { return guest::LoadU32(base, address + 8); }
  void set_tick(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tick_address() const { return address + 8; }
  uint32_t length() const { return guest::LoadU32(base, address + 12); }
  void set_length(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // each frame's start tick from here; the texture words after them
  uint32_t first_start() const { return guest::LoadU32(base, address + 16); }
  void set_first_start(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t first_start_address() const { return address + 16; }
};

// embedded; measure: Scenery@+100
struct SceneryAt100 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: Scenery@+144
struct SceneryAt144 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: Scenery@+92
struct SceneryAt92 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// The image words docs/functions/course-scenery.md's code uses.
struct CourseSceneryGlobals {
  // set by Scenery::Init, cleared by ClearGlobal
  template <typename T = Scenery>
  static T manager(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5514u)); }
  static void set_manager(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5514u, v.address); }
  static void set_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5514u, v); }
  static constexpr uint32_t manager_address() { return 0x826C5514u; }
  // the physical bytes the scenery holds
  static uint32_t physical_held(uint8_t* base) { return guest::LoadU32(base, 0x826C5518u); }
  static void set_physical_held(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5518u, v); }
  static constexpr uint32_t physical_held_address() { return 0x826C5518u; }
  // set by Scenery::Create, cleared by Destroy (the same object)
  template <typename T = Scenery>
  static T created(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C551Cu)); }
  static void set_created(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C551Cu, v.address); }
  static void set_created(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C551Cu, v); }
  static constexpr uint32_t created_address() { return 0x826C551Cu; }
  // the archive's shared load buffer's size
  static uint32_t load_buffer_size(uint8_t* base) { return guest::LoadU32(base, 0x826A5130u); }
  static void set_load_buffer_size(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5130u, v); }
  static constexpr uint32_t load_buffer_size_address() { return 0x826A5130u; }
  // the archive's shared load buffer
  static uint32_t load_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826A5134u); }
  static void set_load_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5134u, v); }
  static constexpr uint32_t load_buffer_address() { return 0x826A5134u; }
  // 192.0, the manager's +200 (copied as bits)
  static uint32_t create_float_200(uint8_t* base) { return guest::LoadU32(base, 0x820067B8u); }
  static void set_create_float_200(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820067B8u, v); }
  static constexpr uint32_t create_float_200_address() { return 0x820067B8u; }
  // 72.0, the manager's +204
  static uint32_t create_float_204(uint8_t* base) { return guest::LoadU32(base, 0x820067B4u); }
  static void set_create_float_204(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820067B4u, v); }
  static constexpr uint32_t create_float_204_address() { return 0x820067B4u; }
  // 256.0, the manager's +208
  static uint32_t create_float_208(uint8_t* base) { return guest::LoadU32(base, 0x82001FACu); }
  static void set_create_float_208(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001FACu, v); }
  static constexpr uint32_t create_float_208_address() { return 0x82001FACu; }
  // 64.0, the manager's +212
  static uint32_t create_float_212(uint8_t* base) { return guest::LoadU32(base, 0x820067B0u); }
  static void set_create_float_212(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820067B0u, v); }
  static constexpr uint32_t create_float_212_address() { return 0x820067B0u; }
  // 0.0: StartLoad's distance for every item
  static float item_distance(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_item_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t item_distance_address() { return 0x82000A34u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (the draws' ranges, inside an area's rectangle)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the render queue's key (the pass's +24)
  static uint32_t key_a(uint8_t* base) { return guest::LoadU32(base, 0x8296B90Cu); }
  static void set_key_a(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B90Cu, v); }
  static constexpr uint32_t key_a_address() { return 0x8296B90Cu; }
  // the render queue's key (the pass's +28)
  static uint32_t key_b(uint8_t* base) { return guest::LoadU32(base, 0x8296B900u); }
  static void set_key_b(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B900u, v); }
  static constexpr uint32_t key_b_address() { return 0x8296B900u; }
  // the depth buckets' key
  static uint32_t key_depth(uint8_t* base) { return guest::LoadU32(base, 0x8296B908u); }
  static void set_key_depth(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B908u, v); }
  static constexpr uint32_t key_depth_address() { return 0x8296B908u; }
  // the depth draws' key
  static uint32_t depth_key(uint8_t* base) { return guest::LoadU32(base, 0x8296B8FCu); }
  static void set_depth_key(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B8FCu, v); }
  static constexpr uint32_t depth_key_address() { return 0x8296B8FCu; }
  // the depth draws' second key
  static uint32_t other_key(uint8_t* base) { return guest::LoadU32(base, 0x8296B8F8u); }
  static void set_other_key(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B8F8u, v); }
  static constexpr uint32_t other_key_address() { return 0x8296B8F8u; }
  // Scenery::Draw's 20 SceneryPassEntry records
  static constexpr uint32_t pass_table_address() { return 0x82603060u; }
  // the 14 SceneryViewMask records
  static constexpr uint32_t view_masks_address() { return 0x82602F80u; }
  // the course set's model's draw range, x (also material-passes' projector_depth_bias)
  static float range_x(uint8_t* base) { return guest::LoadFloat(base, 0x82001FCCu); }
  static void set_range_x(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FCCu, v); }
  static constexpr uint32_t range_x_address() { return 0x82001FCCu; }
  // the course set's model's draw range, y (also scenery-lists' point_range_far)
  static float range_y(uint8_t* base) { return guest::LoadFloat(base, 0x82006804u); }
  static void set_range_y(uint8_t* base, float v) { guest::StoreFloat(base, 0x82006804u, v); }
  static constexpr uint32_t range_y_address() { return 0x82006804u; }
  // 0.1, the headlight projectors' near plane
  static float headlight_near_plane(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_headlight_near_plane(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t headlight_near_plane_address() { return 0x82001FBCu; }
};

}  // namespace schema
