// The views of docs/functions/accessories.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/accessories.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Accessories, AccessoriesBase
struct Accessories : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  static constexpr uint32_t kVtable = 0x82006740;
  // 0x82006740 (the base's 0x8200672C while it's built or torn down, the list node's 0x82010B5C after)
  template <typename T = AccessoriesVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the three entries, by the interior record's +112 (3 is none); 0 when not acquired
  template <typename T = AccessoryEntry>
  T entry_0() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_entry_0(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_entry_0(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t entry_0_address() const { return address + 4; }
  template <typename T = AccessoryEntry>
  T entry_1() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_entry_1(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_entry_1(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t entry_1_address() const { return address + 8; }
  template <typename T = AccessoryEntry>
  T entry_2() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_entry_2(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_entry_2(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t entry_2_address() const { return address + 12; }
  // the scale copied as two doublewords
  uint64_t scale_xy() const { return guest::LoadU64(base, address + 16); }
  void set_scale_xy(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t scale_xy_address() const { return address + 16; }
  // the scale, (1, 1, 1, 1) by default; copied into each accessory's +1024
  float scale_x() const { return guest::LoadFloat(base, address + 16); }
  void set_scale_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t scale_x_address() const { return address + 16; }
  float scale_y() const { return guest::LoadFloat(base, address + 20); }
  void set_scale_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t scale_y_address() const { return address + 20; }
  uint64_t scale_zw() const { return guest::LoadU64(base, address + 24); }
  void set_scale_zw(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t scale_zw_address() const { return address + 24; }
  float scale_z() const { return guest::LoadFloat(base, address + 24); }
  void set_scale_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t scale_z_address() const { return address + 24; }
  float scale_w() const { return guest::LoadFloat(base, address + 28); }
  void set_scale_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t scale_w_address() const { return address + 28; }
};

// image; measure: Accessories.+0
struct AccessoriesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0: the deleting destructor sub_820F0FE0
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: Accessories::Acquire sub_820F0EB8
  uint32_t acquire() const { return guest::LoadU32(base, address + 4); }
  void set_acquire(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t acquire_address() const { return address + 4; }
};

// allocated; measure: AccessoryEntry, Accessories.+4, new<1104>@AccessoryEntry::New, AccessoryEntry::OnLoaded.r6
struct AccessoryEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 1104;
  // the reference count: 1 when made, +1 an acquire, -1 a release (destroyed at 0)
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // an AccessoryFile: archive asset 23008 + the index
  uint32_t file_address() const { return address + 4; }
  // the file's +16: its data (a data file of three parts)
  uint32_t file_data() const { return guest::LoadU32(base, address + 20); }
  void set_file_data(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t file_data_address() const { return address + 20; }
  // an Accessory
  uint32_t accessory_address() const { return address + 48; }
  // the accessory's +1024: its scale
  uint64_t accessory_scale_xy() const { return guest::LoadU64(base, address + 1072); }
  void set_accessory_scale_xy(uint64_t v) const { guest::StoreU64(base, address + 1072, v); }
  uint32_t accessory_scale_xy_address() const { return address + 1072; }
  uint64_t accessory_scale_zw() const { return guest::LoadU64(base, address + 1080); }
  void set_accessory_scale_zw(uint64_t v) const { guest::StoreU64(base, address + 1080, v); }
  uint32_t accessory_scale_zw_address() const { return address + 1080; }
  // the accessory's +1040
  uint32_t accessory_texture_index() const { return guest::LoadU32(base, address + 1088); }
  void set_accessory_texture_index(uint32_t v) const { guest::StoreU32(base, address + 1088, v); }
  uint32_t accessory_texture_index_address() const { return address + 1088; }
  // the accessory's +1048
  uint32_t accessory_loaded() const { return guest::LoadU32(base, address + 1096); }
  void set_accessory_loaded(uint32_t v) const { guest::StoreU32(base, address + 1096, v); }
  uint32_t accessory_loaded_address() const { return address + 1096; }
};

// embedded; measure: AccessoryFile, AccessoryFile::OnArchiveDone.r6
struct AccessoryFile : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  static constexpr uint32_t kVtable = 0x82006738;
  // 0x82006738; the list node's 0x82010B5C after the destructor
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // -1 when none
  uint32_t asset_id() const { return guest::LoadU32(base, address + 4); }
  void set_asset_id(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t asset_id_address() const { return address + 4; }
  // the archive load's serial, -1 when none
  uint32_t serial() const { return guest::LoadU32(base, address + 8); }
  void set_serial(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t serial_address() const { return address + 8; }
  // 0 none, 1 loading, 2 loaded
  uint32_t state() const { return guest::LoadU32(base, address + 12); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t state_address() const { return address + 12; }
  uint32_t data() const { return guest::LoadU32(base, address + 16); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t data_address() const { return address + 16; }
  // Archive::AssetSize's answer (0 when set from data)
  uint32_t size() const { return guest::LoadU32(base, address + 20); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t size_address() const { return address + 20; }
  // the data is freed with VirtualFree on reset
  uint32_t owned() const { return guest::LoadU32(base, address + 24); }
  void set_owned(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t owned_address() const { return address + 24; }
  // called when the archive's load completes (r6 = the context)
  uint32_t callback() const { return guest::LoadU32(base, address + 28); }
  void set_callback(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t callback_address() const { return address + 28; }
  uint32_t callback_context() const { return guest::LoadU32(base, address + 32); }
  void set_callback_context(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t callback_context_address() const { return address + 32; }
};

// embedded; measure: Accessory, Accessories::AcquireEntry.ret
struct Accessory : View {
  using View::View;
  static constexpr uint32_t kSize = 1056;
  static constexpr uint32_t kVtable = 0x820066C8;
  // 0x820066C8 (the part model's while it's built)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // only Matrix::FromScaleRotationTranslation's out reaches it: a measure fault (its r3 is a matrix)
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // as unknown_4
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // the model's shapes
  template <typename T = AccessoryShape>
  T shapes() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_shapes(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_shapes(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shapes_address() const { return address + 40; }
  // the model's texture provider (embedded at +80): its vtable
  template <typename T = AccessoryTexturesVtable>
  T textures_vtable() const { return T(base, guest::LoadU32(base, address + 80)); }
  void set_textures_vtable(const View& v) const { guest::StoreU32(base, address + 80, v.address); }
  void set_textures_vtable(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t textures_vtable_address() const { return address + 80; }
  uint32_t shape_count() const { return guest::LoadU32(base, address + 412); }
  void set_shape_count(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t shape_count_address() const { return address + 412; }
  // an AccessoryFile: the part model
  uint32_t model_file_address() const { return address + 464; }
  // +464's state
  uint32_t model_file_state() const { return guest::LoadU32(base, address + 476); }
  void set_model_file_state(uint32_t v) const { guest::StoreU32(base, address + 476, v); }
  uint32_t model_file_state_address() const { return address + 476; }
  // +464's data
  uint32_t model_file_data() const { return guest::LoadU32(base, address + 480); }
  void set_model_file_data(uint32_t v) const { guest::StoreU32(base, address + 480, v); }
  uint32_t model_file_data_address() const { return address + 480; }
  // an AccessoryFile: the XPR
  uint32_t xpr_file_address() const { return address + 500; }
  // +500's data
  uint32_t xpr_file_data() const { return guest::LoadU32(base, address + 516); }
  void set_xpr_file_data(uint32_t v) const { guest::StoreU32(base, address + 516, v); }
  uint32_t xpr_file_data_address() const { return address + 516; }
  // an AccessorySkeleton
  uint32_t skeleton_address() const { return address + 544; }
  // the skeleton's +80
  template <typename T = AccessoryNodeFile>
  T skeleton_node_file() const { return T(base, guest::LoadU32(base, address + 624)); }
  void set_skeleton_node_file(const View& v) const { guest::StoreU32(base, address + 624, v.address); }
  void set_skeleton_node_file(uint32_t v) const { guest::StoreU32(base, address + 624, v); }
  uint32_t skeleton_node_file_address() const { return address + 624; }
  // the skeleton's +88: the anchor is its row at the swing node
  template <typename T = AccessoryNodeRow>
  T skeleton_rows_0() const { return T(base, guest::LoadU32(base, address + 632)); }
  void set_skeleton_rows_0(const View& v) const { guest::StoreU32(base, address + 632, v.address); }
  void set_skeleton_rows_0(uint32_t v) const { guest::StoreU32(base, address + 632, v); }
  uint32_t skeleton_rows_0_address() const { return address + 632; }
  // the skeleton's +108, handed to Accessory::BindSkin
  uint32_t skeleton_unknown_108() const { return guest::LoadU32(base, address + 652); }
  void set_skeleton_unknown_108(uint32_t v) const { guest::StoreU32(base, address + 652, v); }
  uint32_t skeleton_unknown_108_address() const { return address + 652; }
  // the skeleton's +148
  template <typename T = AccessoryMatrix>
  T skeleton_node_matrices() const { return T(base, guest::LoadU32(base, address + 692)); }
  void set_skeleton_node_matrices(const View& v) const { guest::StoreU32(base, address + 692, v.address); }
  void set_skeleton_node_matrices(uint32_t v) const { guest::StoreU32(base, address + 692, v); }
  uint32_t skeleton_node_matrices_address() const { return address + 692; }
  // the skeleton's +256
  uint32_t skeleton_swing_node() const { return guest::LoadU32(base, address + 800); }
  void set_skeleton_swing_node(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t skeleton_swing_node_address() const { return address + 800; }
  // the anchor copied as two doublewords
  uint64_t anchor_xy() const { return guest::LoadU64(base, address + 992); }
  void set_anchor_xy(uint64_t v) const { guest::StoreU64(base, address + 992, v); }
  uint32_t anchor_xy_address() const { return address + 992; }
  // the anchor position (w = 1)
  float anchor_x() const { return guest::LoadFloat(base, address + 992); }
  void set_anchor_x(float v) const { guest::StoreFloat(base, address + 992, v); }
  uint32_t anchor_x_address() const { return address + 992; }
  float anchor_y() const { return guest::LoadFloat(base, address + 996); }
  void set_anchor_y(float v) const { guest::StoreFloat(base, address + 996, v); }
  uint32_t anchor_y_address() const { return address + 996; }
  uint64_t anchor_zw() const { return guest::LoadU64(base, address + 1000); }
  void set_anchor_zw(uint64_t v) const { guest::StoreU64(base, address + 1000, v); }
  uint32_t anchor_zw_address() const { return address + 1000; }
  float anchor_z() const { return guest::LoadFloat(base, address + 1000); }
  void set_anchor_z(float v) const { guest::StoreFloat(base, address + 1000, v); }
  uint32_t anchor_z_address() const { return address + 1000; }
  float anchor_w() const { return guest::LoadFloat(base, address + 1004); }
  void set_anchor_w(float v) const { guest::StoreFloat(base, address + 1004, v); }
  uint32_t anchor_w_address() const { return address + 1004; }
  // the three angles
  float angle_x() const { return guest::LoadFloat(base, address + 1008); }
  void set_angle_x(float v) const { guest::StoreFloat(base, address + 1008, v); }
  uint32_t angle_x_address() const { return address + 1008; }
  float angle_y() const { return guest::LoadFloat(base, address + 1012); }
  void set_angle_y(float v) const { guest::StoreFloat(base, address + 1012, v); }
  uint32_t angle_y_address() const { return address + 1012; }
  float angle_z() const { return guest::LoadFloat(base, address + 1016); }
  void set_angle_z(float v) const { guest::StoreFloat(base, address + 1016, v); }
  uint32_t angle_z_address() const { return address + 1016; }
  // 1.0
  float angle_w() const { return guest::LoadFloat(base, address + 1020); }
  void set_angle_w(float v) const { guest::StoreFloat(base, address + 1020, v); }
  uint32_t angle_w_address() const { return address + 1020; }
  // the set's scale, copied as two doublewords
  uint64_t scale_xy() const { return guest::LoadU64(base, address + 1024); }
  void set_scale_xy(uint64_t v) const { guest::StoreU64(base, address + 1024, v); }
  uint32_t scale_xy_address() const { return address + 1024; }
  float scale_x() const { return guest::LoadFloat(base, address + 1024); }
  void set_scale_x(float v) const { guest::StoreFloat(base, address + 1024, v); }
  uint32_t scale_x_address() const { return address + 1024; }
  float scale_y() const { return guest::LoadFloat(base, address + 1028); }
  void set_scale_y(float v) const { guest::StoreFloat(base, address + 1028, v); }
  uint32_t scale_y_address() const { return address + 1028; }
  uint64_t scale_zw() const { return guest::LoadU64(base, address + 1032); }
  void set_scale_zw(uint64_t v) const { guest::StoreU64(base, address + 1032, v); }
  uint32_t scale_zw_address() const { return address + 1032; }
  float scale_z() const { return guest::LoadFloat(base, address + 1032); }
  void set_scale_z(float v) const { guest::StoreFloat(base, address + 1032, v); }
  uint32_t scale_z_address() const { return address + 1032; }
  float scale_w() const { return guest::LoadFloat(base, address + 1036); }
  void set_scale_w(float v) const { guest::StoreFloat(base, address + 1036, v); }
  uint32_t scale_w_address() const { return address + 1036; }
  uint32_t texture_index() const { return guest::LoadU32(base, address + 1040); }
  void set_texture_index(uint32_t v) const { guest::StoreU32(base, address + 1040, v); }
  uint32_t texture_index_address() const { return address + 1040; }
  // bit 0; 1 by default
  uint32_t visible() const { return guest::LoadU32(base, address + 1044); }
  void set_visible(uint32_t v) const { guest::StoreU32(base, address + 1044, v); }
  uint32_t visible_address() const { return address + 1044; }
  uint32_t loaded() const { return guest::LoadU32(base, address + 1048); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 1048, v); }
  uint32_t loaded_address() const { return address + 1048; }
};

// embedded; measure: Accessory.+40
struct AccessoryShape : View {
  using View::View;
  static constexpr uint32_t kSize = 544;
  // 1: BindSkin skins the shape (probably)
  uint32_t skinned() const { return guest::LoadU32(base, address + 428); }
  void set_skinned(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t skinned_address() const { return address + 428; }
};

// image; measure: Accessory.+80
struct AccessoryTexturesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // slot 3: answers the texture set (its +20 the texture count)
  uint32_t texture_set() const { return guest::LoadU32(base, address + 12); }
  void set_texture_set(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t texture_set_address() const { return address + 12; }
};

// embedded; measure: AccessorySkeleton, sub_8259A2B0.r3, sub_8259A400.r3, sub_8259B660.r3, sub_8259BB98.r3
struct AccessorySkeleton : View {
  using View::View;
  static constexpr uint32_t kSize = 448;
  static constexpr uint32_t kVtable = 0x82006700;
  // 0x82006700
  template <typename T = AccessorySkeletonVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the node file (SkeletonBase's view of it starts here)
  template <typename T = AccessoryNodeFile>
  T node_file() const { return T(base, guest::LoadU32(base, address + 80)); }
  void set_node_file(const View& v) const { guest::StoreU32(base, address + 80, v.address); }
  void set_node_file(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t node_file_address() const { return address + 80; }
  // the node records, 160 bytes each
  template <typename T = AccessoryNodeRecord>
  T nodes() const { return T(base, guest::LoadU32(base, address + 84)); }
  void set_nodes(const View& v) const { guest::StoreU32(base, address + 84, v.address); }
  void set_nodes(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t nodes_address() const { return address + 84; }
  // three arrays of 16-byte rows, one a node, copied into the local entries' +0, +16, +32 by Reset
  template <typename T = AccessoryNodeRow>
  T rows_0() const { return T(base, guest::LoadU32(base, address + 88)); }
  void set_rows_0(const View& v) const { guest::StoreU32(base, address + 88, v.address); }
  void set_rows_0(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t rows_0_address() const { return address + 88; }
  template <typename T = AccessoryNodeRow>
  T rows_1() const { return T(base, guest::LoadU32(base, address + 92)); }
  void set_rows_1(const View& v) const { guest::StoreU32(base, address + 92, v.address); }
  void set_rows_1(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t rows_1_address() const { return address + 92; }
  template <typename T = AccessoryNodeRow>
  T rows_2() const { return T(base, guest::LoadU32(base, address + 96)); }
  void set_rows_2(const View& v) const { guest::StoreU32(base, address + 96, v.address); }
  void set_rows_2(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t rows_2_address() const { return address + 96; }
  // read by sub_8259ACE0 and sub_8259AF20 (not read)
  uint32_t unknown_104() const { return guest::LoadU32(base, address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // read by sub_8259A400 (not read)
  template <typename T = AccessorySkeletonTable>
  T unknown_120() const { return T(base, guest::LoadU32(base, address + 120)); }
  void set_unknown_120(const View& v) const { guest::StoreU32(base, address + 120, v.address); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  // the nodes' 144-byte local entries
  template <typename T = AccessoryNodeLocal>
  T locals() const { return T(base, guest::LoadU32(base, address + 136)); }
  void set_locals(const View& v) const { guest::StoreU32(base, address + 136, v.address); }
  void set_locals(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t locals_address() const { return address + 136; }
  // +136 + 144 x the node file's +12 (the swinging nodes' entries)
  template <typename T = AccessoryNodeLocal>
  T swing_locals() const { return T(base, guest::LoadU32(base, address + 140)); }
  void set_swing_locals(const View& v) const { guest::StoreU32(base, address + 140, v.address); }
  void set_swing_locals(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t swing_locals_address() const { return address + 140; }
  // a 64-byte entry for each of the node file's +8
  template <typename T = AccessorySwingEntry>
  T swing_entries() const { return T(base, guest::LoadU32(base, address + 144)); }
  void set_swing_entries(const View& v) const { guest::StoreU32(base, address + 144, v.address); }
  void set_swing_entries(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t swing_entries_address() const { return address + 144; }
  // the nodes' 64-byte matrices
  template <typename T = AccessoryMatrix>
  T node_matrices() const { return T(base, guest::LoadU32(base, address + 148)); }
  void set_node_matrices(const View& v) const { guest::StoreU32(base, address + 148, v.address); }
  void set_node_matrices(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t node_matrices_address() const { return address + 148; }
  // 0 until the first step (Reset clears it, Swing sets it)
  uint32_t stepped() const { return guest::LoadU32(base, address + 152); }
  void set_stepped(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t stepped_address() const { return address + 152; }
  // the second base's: 0x820066FC (the list node's 0x82010B5C while built or torn down)
  uint32_t vtable_2() const { return guest::LoadU32(base, address + 176); }
  void set_vtable_2(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t vtable_2_address() const { return address + 176; }
  // an AccessoryFile: the node file
  uint32_t file_address() const { return address + 180; }
  // +180's state
  uint32_t file_state() const { return guest::LoadU32(base, address + 192); }
  void set_file_state(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t file_state_address() const { return address + 192; }
  // +180's data
  uint32_t file_data() const { return guest::LoadU32(base, address + 196); }
  void set_file_data(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t file_data_address() const { return address + 196; }
  // the job (40 bytes, as DebugText's in framework.md): 0x820066DC (0x82000EB8 after its destructor)
  uint32_t job_vtable() const { return guest::LoadU32(base, address + 216); }
  void set_job_vtable(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t job_vtable_address() const { return address + 216; }
  // 0x82000AB4 first, then 0x82002908
  uint32_t job_functor_vtable() const { return guest::LoadU32(base, address + 220); }
  void set_job_functor_vtable(uint32_t v) const { guest::StoreU32(base, address + 220, v); }
  uint32_t job_functor_vtable_address() const { return address + 220; }
  // the job's +8: 0
  uint32_t unknown_224() const { return guest::LoadU32(base, address + 224); }
  void set_unknown_224(uint32_t v) const { guest::StoreU32(base, address + 224, v); }
  uint32_t unknown_224_address() const { return address + 224; }
  // the job's +12: 0
  uint32_t unknown_228() const { return guest::LoadU32(base, address + 228); }
  void set_unknown_228(uint32_t v) const { guest::StoreU32(base, address + 228, v); }
  uint32_t unknown_228_address() const { return address + 228; }
  // the job's +16: 0
  uint8_t unknown_232() const { return guest::LoadU8(base, address + 232); }
  void set_unknown_232(uint8_t v) const { guest::StoreU8(base, address + 232, v); }
  uint32_t unknown_232_address() const { return address + 232; }
  // the job's +17: 1
  uint8_t unknown_233() const { return guest::LoadU8(base, address + 233); }
  void set_unknown_233(uint8_t v) const { guest::StoreU8(base, address + 233, v); }
  uint32_t unknown_233_address() const { return address + 233; }
  // the job's +18: 0
  uint8_t unknown_234() const { return guest::LoadU8(base, address + 234); }
  void set_unknown_234(uint8_t v) const { guest::StoreU8(base, address + 234, v); }
  uint32_t unknown_234_address() const { return address + 234; }
  // the job's +20: Step stores a frame word it never sets
  uint32_t unknown_236() const { return guest::LoadU32(base, address + 236); }
  void set_unknown_236(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t unknown_236_address() const { return address + 236; }
  // 0x8200833C (0x82000ABC, the functors' base, after)
  uint32_t job_delegate_vtable() const { return guest::LoadU32(base, address + 240); }
  void set_job_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 240, v); }
  uint32_t job_delegate_vtable_address() const { return address + 240; }
  // the skeleton, set by Step (0 when made)
  template <typename T = AccessorySkeleton>
  T job_target() const { return T(base, guest::LoadU32(base, address + 244)); }
  void set_job_target(const View& v) const { guest::StoreU32(base, address + 244, v.address); }
  void set_job_target(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t job_target_address() const { return address + 244; }
  // the delegate's method, written as a doubleword
  uint64_t job_method_pair() const { return guest::LoadU64(base, address + 248); }
  void set_job_method_pair(uint64_t v) const { guest::StoreU64(base, address + 248, v); }
  uint32_t job_method_pair_address() const { return address + 248; }
  // AccessorySkeleton::Update sub_820F0628 once stepped (0 when made)
  uint32_t job_method() const { return guest::LoadU32(base, address + 248); }
  void set_job_method(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t job_method_address() const { return address + 248; }
  // 0
  uint32_t job_method_extra() const { return guest::LoadU32(base, address + 252); }
  void set_job_method_extra(uint32_t v) const { guest::StoreU32(base, address + 252, v); }
  uint32_t job_method_extra_address() const { return address + 252; }
  // the node whose name holds the string at 0x82006710
  uint32_t swing_node() const { return guest::LoadU32(base, address + 256); }
  void set_swing_node(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t swing_node_address() const { return address + 256; }
  uint32_t set_up() const { return guest::LoadU32(base, address + 260); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t set_up_address() const { return address + 260; }
  // 1 when made
  uint32_t swinging() const { return guest::LoadU32(base, address + 264); }
  void set_swinging(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t swinging_address() const { return address + 264; }
  // probably
  float angle() const { return guest::LoadFloat(base, address + 268); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 268, v); }
  uint32_t angle_address() const { return address + 268; }
  // probably
  float angle_speed() const { return guest::LoadFloat(base, address + 272); }
  void set_angle_speed(float v) const { guest::StoreFloat(base, address + 272, v); }
  uint32_t angle_speed_address() const { return address + 272; }
  // this step's matrix
  uint32_t matrix_address() const { return address + 288; }
  // this step's matrix (+288, 64 bytes)
  float matrix_row0_w() const { return guest::LoadFloat(base, address + 300); }
  void set_matrix_row0_w(float v) const { guest::StoreFloat(base, address + 300, v); }
  uint32_t matrix_row0_w_address() const { return address + 300; }
  float matrix_row1_w() const { return guest::LoadFloat(base, address + 316); }
  void set_matrix_row1_w(float v) const { guest::StoreFloat(base, address + 316, v); }
  uint32_t matrix_row1_w_address() const { return address + 316; }
  float matrix_row2_x() const { return guest::LoadFloat(base, address + 320); }
  void set_matrix_row2_x(float v) const { guest::StoreFloat(base, address + 320, v); }
  uint32_t matrix_row2_x_address() const { return address + 320; }
  float matrix_row2_z() const { return guest::LoadFloat(base, address + 328); }
  void set_matrix_row2_z(float v) const { guest::StoreFloat(base, address + 328, v); }
  uint32_t matrix_row2_z_address() const { return address + 328; }
  float matrix_row2_w() const { return guest::LoadFloat(base, address + 332); }
  void set_matrix_row2_w(float v) const { guest::StoreFloat(base, address + 332, v); }
  uint32_t matrix_row2_w_address() const { return address + 332; }
  // the translation
  float matrix_row3_x() const { return guest::LoadFloat(base, address + 336); }
  void set_matrix_row3_x(float v) const { guest::StoreFloat(base, address + 336, v); }
  uint32_t matrix_row3_x_address() const { return address + 336; }
  float matrix_row3_y() const { return guest::LoadFloat(base, address + 340); }
  void set_matrix_row3_y(float v) const { guest::StoreFloat(base, address + 340, v); }
  uint32_t matrix_row3_y_address() const { return address + 340; }
  float matrix_row3_z() const { return guest::LoadFloat(base, address + 344); }
  void set_matrix_row3_z(float v) const { guest::StoreFloat(base, address + 344, v); }
  uint32_t matrix_row3_z_address() const { return address + 344; }
  float matrix_row3_w() const { return guest::LoadFloat(base, address + 348); }
  void set_matrix_row3_w(float v) const { guest::StoreFloat(base, address + 348, v); }
  uint32_t matrix_row3_w_address() const { return address + 348; }
  // the last step's matrix
  uint32_t last_matrix_address() const { return address + 352; }
  // the last step's matrix (+352, 64 bytes)
  float last_row0_w() const { return guest::LoadFloat(base, address + 364); }
  void set_last_row0_w(float v) const { guest::StoreFloat(base, address + 364, v); }
  uint32_t last_row0_w_address() const { return address + 364; }
  float last_row1_w() const { return guest::LoadFloat(base, address + 380); }
  void set_last_row1_w(float v) const { guest::StoreFloat(base, address + 380, v); }
  uint32_t last_row1_w_address() const { return address + 380; }
  float last_row2_w() const { return guest::LoadFloat(base, address + 396); }
  void set_last_row2_w(float v) const { guest::StoreFloat(base, address + 396, v); }
  uint32_t last_row2_w_address() const { return address + 396; }
  // the translation
  float last_row3_x() const { return guest::LoadFloat(base, address + 400); }
  void set_last_row3_x(float v) const { guest::StoreFloat(base, address + 400, v); }
  uint32_t last_row3_x_address() const { return address + 400; }
  float last_row3_y() const { return guest::LoadFloat(base, address + 404); }
  void set_last_row3_y(float v) const { guest::StoreFloat(base, address + 404, v); }
  uint32_t last_row3_y_address() const { return address + 404; }
  float last_row3_z() const { return guest::LoadFloat(base, address + 408); }
  void set_last_row3_z(float v) const { guest::StoreFloat(base, address + 408, v); }
  uint32_t last_row3_z_address() const { return address + 408; }
  float last_row3_w() const { return guest::LoadFloat(base, address + 412); }
  void set_last_row3_w(float v) const { guest::StoreFloat(base, address + 412, v); }
  uint32_t last_row3_w_address() const { return address + 412; }
  // the last step's movement (probably), copied as two doublewords
  uint64_t movement_xy() const { return guest::LoadU64(base, address + 416); }
  void set_movement_xy(uint64_t v) const { guest::StoreU64(base, address + 416, v); }
  uint32_t movement_xy_address() const { return address + 416; }
  float movement_x() const { return guest::LoadFloat(base, address + 416); }
  void set_movement_x(float v) const { guest::StoreFloat(base, address + 416, v); }
  uint32_t movement_x_address() const { return address + 416; }
  float movement_y() const { return guest::LoadFloat(base, address + 420); }
  void set_movement_y(float v) const { guest::StoreFloat(base, address + 420, v); }
  uint32_t movement_y_address() const { return address + 420; }
  uint64_t movement_zw() const { return guest::LoadU64(base, address + 424); }
  void set_movement_zw(uint64_t v) const { guest::StoreU64(base, address + 424, v); }
  uint32_t movement_zw_address() const { return address + 424; }
  float movement_z() const { return guest::LoadFloat(base, address + 424); }
  void set_movement_z(float v) const { guest::StoreFloat(base, address + 424, v); }
  uint32_t movement_z_address() const { return address + 424; }
  // 1.0
  float movement_w() const { return guest::LoadFloat(base, address + 428); }
  void set_movement_w(float v) const { guest::StoreFloat(base, address + 428, v); }
  uint32_t movement_w_address() const { return address + 428; }
  // the swing vector (probably), copied as two doublewords
  uint64_t swing_xy() const { return guest::LoadU64(base, address + 432); }
  void set_swing_xy(uint64_t v) const { guest::StoreU64(base, address + 432, v); }
  uint32_t swing_xy_address() const { return address + 432; }
  float swing_x() const { return guest::LoadFloat(base, address + 432); }
  void set_swing_x(float v) const { guest::StoreFloat(base, address + 432, v); }
  uint32_t swing_x_address() const { return address + 432; }
  float swing_y() const { return guest::LoadFloat(base, address + 436); }
  void set_swing_y(float v) const { guest::StoreFloat(base, address + 436, v); }
  uint32_t swing_y_address() const { return address + 436; }
  uint64_t swing_zw() const { return guest::LoadU64(base, address + 440); }
  void set_swing_zw(uint64_t v) const { guest::StoreU64(base, address + 440, v); }
  uint32_t swing_zw_address() const { return address + 440; }
  float swing_z() const { return guest::LoadFloat(base, address + 440); }
  void set_swing_z(float v) const { guest::StoreFloat(base, address + 440, v); }
  uint32_t swing_z_address() const { return address + 440; }
  // 1.0
  float swing_w() const { return guest::LoadFloat(base, address + 444); }
  void set_swing_w(float v) const { guest::StoreFloat(base, address + 444, v); }
  uint32_t swing_w_address() const { return address + 444; }
};

// image; measure: sub_8259BB98.r3.+0
struct AccessorySkeletonVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 0x82106540 in the skeletons' vtables
  uint32_t slot_2() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_address() const { return address + 8; }
  // 0x82106540 too
  uint32_t slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
};

// file; measure: AccessorySkeleton.+80, Accessory.+624, sub_8259A2B0.r3.+80, sub_8259A400.r3.+80, sub_8259B660.r3.+80, sub_8259BB98.r3.+80
struct AccessoryNodeFile : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t node_count() const { return guest::LoadU32(base, address + 4); }
  void set_node_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_count_address() const { return address + 4; }
  // the swinging nodes
  uint32_t swing_count() const { return guest::LoadU32(base, address + 8); }
  void set_swing_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t swing_count_address() const { return address + 8; }
  // the first swinging node's index
  uint32_t swing_first() const { return guest::LoadU32(base, address + 12); }
  void set_swing_first(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t swing_first_address() const { return address + 12; }
};

// file; measure: AccessorySkeleton.+84
struct AccessoryNodeRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  // a string (strstr'd for the swing node); its length probably
  uint32_t name_address() const { return address + 0; }
  // 16 bytes copied into the local entry's +48 when flags has 0x40
  uint64_t rest_xy() const { return guest::LoadU64(base, address + 64); }
  void set_rest_xy(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t rest_xy_address() const { return address + 64; }
  uint64_t rest_zw() const { return guest::LoadU64(base, address + 72); }
  void set_rest_zw(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t rest_zw_address() const { return address + 72; }
  // 0x40 its +64 is the local position; 0x80 a swinging node turned by Update
  uint32_t flags() const { return guest::LoadU32(base, address + 132); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t flags_address() const { return address + 132; }
  // its length from its parent (SwingNode)
  float length() const { return guest::LoadFloat(base, address + 140); }
  void set_length(float v) const { guest::StoreFloat(base, address + 140, v); }
  uint32_t length_address() const { return address + 140; }
};

// file; measure: AccessorySkeleton.+88, AccessorySkeleton.+92, AccessorySkeleton.+96, Accessory.+632
struct AccessoryNodeRow : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// allocated; measure: AccessorySkeleton.+136, AccessorySkeleton.+140
struct AccessoryNodeLocal : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  uint64_t row0_xy() const { return guest::LoadU64(base, address + 0); }
  void set_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t row0_xy_address() const { return address + 0; }
  float row0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  float row0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_row0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t row0_y_address() const { return address + 4; }
  uint64_t row0_zw() const { return guest::LoadU64(base, address + 8); }
  void set_row0_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t row0_zw_address() const { return address + 8; }
  float row0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_row0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t row0_z_address() const { return address + 8; }
  uint64_t row1_xy() const { return guest::LoadU64(base, address + 16); }
  void set_row1_xy(uint64_t v) const { guest::StoreU64(base, address + 16, v); }
  uint32_t row1_xy_address() const { return address + 16; }
  float row1_x() const { return guest::LoadFloat(base, address + 16); }
  void set_row1_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t row1_x_address() const { return address + 16; }
  // a swinging node's angle (Update: a constant + atan2)
  float row1_y() const { return guest::LoadFloat(base, address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  uint64_t row1_zw() const { return guest::LoadU64(base, address + 24); }
  void set_row1_zw(uint64_t v) const { guest::StoreU64(base, address + 24, v); }
  uint32_t row1_zw_address() const { return address + 24; }
  float row1_z() const { return guest::LoadFloat(base, address + 24); }
  void set_row1_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t row1_z_address() const { return address + 24; }
  uint64_t row2_xy() const { return guest::LoadU64(base, address + 32); }
  void set_row2_xy(uint64_t v) const { guest::StoreU64(base, address + 32, v); }
  uint32_t row2_xy_address() const { return address + 32; }
  uint64_t row2_zw() const { return guest::LoadU64(base, address + 40); }
  void set_row2_zw(uint64_t v) const { guest::StoreU64(base, address + 40, v); }
  uint32_t row2_zw_address() const { return address + 40; }
  // the position (the node record's +64 when its flags have 0x40)
  uint64_t row3_xy() const { return guest::LoadU64(base, address + 48); }
  void set_row3_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t row3_xy_address() const { return address + 48; }
  uint64_t row3_zw() const { return guest::LoadU64(base, address + 56); }
  void set_row3_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t row3_zw_address() const { return address + 56; }
};

// allocated; measure: AccessorySkeleton.+144, sub_8259A2B0.r3.+144, sub_8259A400.r3.+144, sub_8259B660.r3.+144, sub_8259BB98.r3.+144
struct AccessorySwingEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float row0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  float row0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_row0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t row0_y_address() const { return address + 4; }
  float row0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_row0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t row0_z_address() const { return address + 8; }
  // 1.0 when made
  float row0_w() const { return guest::LoadFloat(base, address + 12); }
  void set_row0_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t row0_w_address() const { return address + 12; }
  float row1_x() const { return guest::LoadFloat(base, address + 16); }
  void set_row1_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t row1_x_address() const { return address + 16; }
  float row1_y() const { return guest::LoadFloat(base, address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  float row1_z() const { return guest::LoadFloat(base, address + 24); }
  void set_row1_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t row1_z_address() const { return address + 24; }
  // 1.0 when made
  float row1_w() const { return guest::LoadFloat(base, address + 28); }
  void set_row1_w(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t row1_w_address() const { return address + 28; }
  float row2_x() const { return guest::LoadFloat(base, address + 32); }
  void set_row2_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t row2_x_address() const { return address + 32; }
  float row2_y() const { return guest::LoadFloat(base, address + 36); }
  void set_row2_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t row2_y_address() const { return address + 36; }
  float row2_z() const { return guest::LoadFloat(base, address + 40); }
  void set_row2_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t row2_z_address() const { return address + 40; }
  // 1.0 when made
  float row2_w() const { return guest::LoadFloat(base, address + 44); }
  void set_row2_w(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t row2_w_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  // 1 when made
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// allocated; measure: AccessorySkeleton.+148, sub_8259A2B0.r3.+148, Accessory::SetAnchor.r4
struct AccessoryMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint64_t row0_xy() const { return guest::LoadU64(base, address + 0); }
  void set_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t row0_xy_address() const { return address + 0; }
  // the scale's x in FromScaleRotationTranslation
  float row0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_row0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t row0_x_address() const { return address + 0; }
  // the scale's y
  float row1_y() const { return guest::LoadFloat(base, address + 20); }
  void set_row1_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t row1_y_address() const { return address + 20; }
  // the scale's z
  float row2_z() const { return guest::LoadFloat(base, address + 40); }
  void set_row2_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t row2_z_address() const { return address + 40; }
  uint64_t translation_xy() const { return guest::LoadU64(base, address + 48); }
  void set_translation_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t translation_xy_address() const { return address + 48; }
  float translation_x() const { return guest::LoadFloat(base, address + 48); }
  void set_translation_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t translation_x_address() const { return address + 48; }
  float translation_y() const { return guest::LoadFloat(base, address + 52); }
  void set_translation_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t translation_y_address() const { return address + 52; }
  uint64_t translation_zw() const { return guest::LoadU64(base, address + 56); }
  void set_translation_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t translation_zw_address() const { return address + 56; }
  float translation_z() const { return guest::LoadFloat(base, address + 56); }
  void set_translation_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t translation_z_address() const { return address + 56; }
};

// file; measure: sub_8259A400.r3.+120
struct AccessorySkeletonTable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// embedded; measure: Accessories::SetScale.r4, sub_8259BB98.r4, sub_8259B188.r4
struct AccessoryFloat4 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint64_t xy() const { return guest::LoadU64(base, address + 0); }
  void set_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t xy_address() const { return address + 0; }
  uint64_t zw() const { return guest::LoadU64(base, address + 8); }
  void set_zw(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t zw_address() const { return address + 8; }
};

// embedded; measure: AccessorySkeleton::SwingNode.r5, sub_8259B660.r3.+144@+16
struct AccessoryFloat3 : View {
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

// embedded; measure: AccessorySkeleton::Step.r4
struct AccessoryWorkerPool : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AccessoryWorkerPoolVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: AccessorySkeleton::Step.r4.+0
struct AccessoryWorkerPoolVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1: r4 = the job, r5 = 1
  uint32_t submit() const { return guest::LoadU32(base, address + 4); }
  void set_submit(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t submit_address() const { return address + 4; }
};

// embedded; measure: Accessory@+1008
struct AccessoryAt1008 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: Accessory@+1024
struct AccessoryAt1024 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: Accessory@+992
struct AccessoryAt992 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(base, address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// The image words docs/functions/accessories.md's code uses.
struct AccessoriesGlobals {
  // made by Accessories::Create, freed by Accessories::Destroy
  template <typename T = Accessories>
  static T set(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5510u)); }
  static void set_set(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5510u, v.address); }
  static void set_set(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5510u, v); }
  static constexpr uint32_t set_address() { return 0x826C5510u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the material groups Accessory::SetTexture walks: 0
  static uint32_t material_group_0(uint8_t* base) { return guest::LoadU32(base, 0x820066D0u); }
  static void set_material_group_0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820066D0u, v); }
  static constexpr uint32_t material_group_0_address() { return 0x820066D0u; }
  // 1
  static uint32_t material_group_1(uint8_t* base) { return guest::LoadU32(base, 0x820066D4u); }
  static void set_material_group_1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820066D4u, v); }
  static constexpr uint32_t material_group_1_address() { return 0x820066D4u; }
  // the string a swinging node's name holds (not read)
  static constexpr uint32_t swing_node_name_address() { return 0x82006710u; }
};

}  // namespace schema
