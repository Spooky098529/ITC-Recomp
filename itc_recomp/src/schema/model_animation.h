// The views of docs/functions/model-animation.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/model-animation.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: TrackSet, sub_825A1148.r3
struct TrackSet : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the part file's section (0: none)
  template <typename T = AnimationSection>
  T section() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_section(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_section(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the section's +16 (its track count)'s address
  uint32_t track_count_pointer() const { return guest::LoadU32(base, address + 4); }
  void set_track_count_pointer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t track_count_pointer_address() const { return address + 4; }
  // the section's +20: its 80-byte tracks
  template <typename T = AnimationTrack>
  T tracks() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_tracks(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_tracks(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tracks_address() const { return address + 8; }
  // the section's default start time (its +4)
  float start_time() const { return guest::LoadFloat(base, address + 12); }
  void set_start_time(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t start_time_address() const { return address + 12; }
  // one value a track (16, 4, 4, 32 or 208 bytes by the set); the 16-, 32- and 208-byte ones after a 16-byte count header
  template <typename T = TrackValues>
  T values() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_values(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_values(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t values_address() const { return address + 16; }
};

// file; measure: TrackSet::View.r4
struct AnimationSection : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  // the default start time
  float start_time() const { return guest::LoadFloat(base, address + 4); }
  void set_start_time(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t start_time_address() const { return address + 4; }
  // the default end time
  float end_time() const { return guest::LoadFloat(base, address + 8); }
  void set_end_time(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t end_time_address() const { return address + 8; }
  uint32_t track_count() const { return guest::LoadU32(base, address + 16); }
  void set_track_count(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t track_count_address() const { return address + 16; }
  // the tracks from here (80 bytes each; the +0x28 section's are 20-byte SkeletonNodeEntry, the +0x3C section's 352 bytes)
  uint32_t tracks_address() const { return address + 20; }
};

// file; measure: TrackSet::Track.ret
struct AnimationTrack : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  uint32_t channel_count() const { return guest::LoadU32(base, address + 64); }
  void set_channel_count(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t channel_count_address() const { return address + 64; }
  // the first channel's offset from the section
  uint32_t channels_offset() const { return guest::LoadU32(base, address + 68); }
  void set_channels_offset(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t channels_offset_address() const { return address + 68; }
  float start_time() const { return guest::LoadFloat(base, address + 72); }
  void set_start_time(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t start_time_address() const { return address + 72; }
  float end_time() const { return guest::LoadFloat(base, address + 76); }
  void set_end_time(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t end_time_address() const { return address + 76; }
};

// file; measure: sub_825A6648.ret
struct AnimationTrack208 : View {
  using View::View;
  static constexpr uint32_t kSize = 352;
  uint64_t unknown_64() const { return guest::LoadU64(base, address + 64); }
  void set_unknown_64(uint64_t v) const { guest::StoreU64(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint64_t unknown_72() const { return guest::LoadU64(base, address + 72); }
  void set_unknown_72(uint64_t v) const { guest::StoreU64(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint64_t unknown_80() const { return guest::LoadU64(base, address + 80); }
  void set_unknown_80(uint64_t v) const { guest::StoreU64(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  uint64_t unknown_88() const { return guest::LoadU64(base, address + 88); }
  void set_unknown_88(uint64_t v) const { guest::StoreU64(base, address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint64_t unknown_96() const { return guest::LoadU64(base, address + 96); }
  void set_unknown_96(uint64_t v) const { guest::StoreU64(base, address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint64_t unknown_104() const { return guest::LoadU64(base, address + 104); }
  void set_unknown_104(uint64_t v) const { guest::StoreU64(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint64_t unknown_112() const { return guest::LoadU64(base, address + 112); }
  void set_unknown_112(uint64_t v) const { guest::StoreU64(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  uint64_t unknown_120() const { return guest::LoadU64(base, address + 120); }
  void set_unknown_120(uint64_t v) const { guest::StoreU64(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  uint64_t unknown_128() const { return guest::LoadU64(base, address + 128); }
  void set_unknown_128(uint64_t v) const { guest::StoreU64(base, address + 128, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint64_t unknown_136() const { return guest::LoadU64(base, address + 136); }
  void set_unknown_136(uint64_t v) const { guest::StoreU64(base, address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  uint64_t unknown_144() const { return guest::LoadU64(base, address + 144); }
  void set_unknown_144(uint64_t v) const { guest::StoreU64(base, address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  uint64_t unknown_152() const { return guest::LoadU64(base, address + 152); }
  void set_unknown_152(uint64_t v) const { guest::StoreU64(base, address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  uint64_t unknown_160() const { return guest::LoadU64(base, address + 160); }
  void set_unknown_160(uint64_t v) const { guest::StoreU64(base, address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  uint64_t unknown_168() const { return guest::LoadU64(base, address + 168); }
  void set_unknown_168(uint64_t v) const { guest::StoreU64(base, address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  float unknown_176() const { return guest::LoadFloat(base, address + 176); }
  void set_unknown_176(float v) const { guest::StoreFloat(base, address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  float unknown_180() const { return guest::LoadFloat(base, address + 180); }
  void set_unknown_180(float v) const { guest::StoreFloat(base, address + 180, v); }
  uint32_t unknown_180_address() const { return address + 180; }
  float unknown_184() const { return guest::LoadFloat(base, address + 184); }
  void set_unknown_184(float v) const { guest::StoreFloat(base, address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  float unknown_188() const { return guest::LoadFloat(base, address + 188); }
  void set_unknown_188(float v) const { guest::StoreFloat(base, address + 188, v); }
  uint32_t unknown_188_address() const { return address + 188; }
  float unknown_212() const { return guest::LoadFloat(base, address + 212); }
  void set_unknown_212(float v) const { guest::StoreFloat(base, address + 212, v); }
  uint32_t unknown_212_address() const { return address + 212; }
  uint32_t unknown_216() const { return guest::LoadU32(base, address + 216); }
  void set_unknown_216(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t unknown_216_address() const { return address + 216; }
};

// file; measure: TrackSet::Channels.ret
struct AnimationChannel : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t key_count() const { return guest::LoadU32(base, address + 0); }
  void set_key_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_count_address() const { return address + 0; }
  // the float index of the value it writes
  uint32_t component() const { return guest::LoadU32(base, address + 4); }
  void set_component(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t component_address() const { return address + 4; }
  // the 20-byte AnimationKeys from here
  uint32_t keys_address() const { return address + 8; }
};

// file; measure: Key, Key::Interpolate.r4
struct AnimationKey : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  float time() const { return guest::LoadFloat(base, address + 0); }
  void set_time(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t time_address() const { return address + 0; }
  // 2 linear, 8 stepped, else cubic Hermite
  uint32_t flags() const { return guest::LoadU32(base, address + 4); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t flags_address() const { return address + 4; }
  float value() const { return guest::LoadFloat(base, address + 8); }
  void set_value(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t value_address() const { return address + 8; }
  float in_tangent() const { return guest::LoadFloat(base, address + 12); }
  void set_in_tangent(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t in_tangent_address() const { return address + 12; }
  float out_tangent() const { return guest::LoadFloat(base, address + 16); }
  void set_out_tangent(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t out_tangent_address() const { return address + 16; }
};

// file; measure: CursorCurve::Sample.r4, TrackSet::Channels.ret@+8
struct AnimationKeyRun : View {
  using View::View;
  static constexpr uint32_t kSize = 60;
  // 20-byte AnimationKeys
  uint32_t keys_address() const { return address + 0; }
};

// allocated; measure: new<4294967295>@TrackSet::SetupFloat
struct TrackValues : View {
  using View::View;
  static constexpr uint32_t kSize = 208;
  // one a track: 16 bytes (x, y, z, +12 the time) at +304, a float (the time) at +324 and +344, 32 bytes (+16 the time, +20 the mask of the components written) at +364, 208 bytes at +384 (+196 sub_825A6608's)
  uint32_t values_address() const { return address + 0; }
};

// allocated; measure: new<4294967295>@TrackSet::SetupFloat4, new<4294967295>@TrackSet::Setup32, new<4294967295>@TrackSet::Setup208
struct TrackValueBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 224;
  // operator new[]'s count header: the track count
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the values (TrackSet +16 points here)
  uint32_t values_address() const { return address + 16; }
};

// file; measure: Skeleton, SkeletonBase, sub_82599BE0.r3, sub_8259B600.r3, PartsPoints.+0
struct Skeleton : View {
  using View::View;
  static constexpr uint32_t kSize = 180;
  static constexpr uint32_t kVtable = 0x8206CE50;
  // 0x8206CE50 (the base's 0x8206CE60)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a matrix, the identity when made (the root's is multiplied by it)
  uint32_t matrix_address() const { return address + 16; }
  // the node file's view (sub_825A21A8, +80 to +124) starts here: the file (+4 the node count, +8 the bones, +12 the first bone's node)
  uint32_t node_file() const { return guest::LoadU32(base, address + 80); }
  void set_node_file(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t node_file_address() const { return address + 80; }
  // the file's node records (160 bytes: +128 the parent, +132 the type, 0x40 and 0x80 flags)
  uint32_t node_records() const { return guest::LoadU32(base, address + 84); }
  void set_node_records(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t node_records_address() const { return address + 84; }
  // 16 bytes a node (Skeleton::ResetPose copies them to the node states' +0; probably)
  uint32_t rest_translations() const { return guest::LoadU32(base, address + 88); }
  void set_rest_translations(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t rest_translations_address() const { return address + 88; }
  // 16 bytes a node (to the node states' +16; probably)
  uint32_t rest_angles() const { return guest::LoadU32(base, address + 92); }
  void set_rest_angles(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t rest_angles_address() const { return address + 92; }
  // 16 bytes a node (to the node states' +32; probably)
  uint32_t rest_scales() const { return guest::LoadU32(base, address + 96); }
  void set_rest_scales(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t rest_scales_address() const { return address + 96; }
  // the view's +20
  uint32_t unknown_100() const { return guest::LoadU32(base, address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  // the file's 64-byte matrices, one a node
  uint32_t file_matrices() const { return guest::LoadU32(base, address + 104); }
  void set_file_matrices(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t file_matrices_address() const { return address + 104; }
  // the view's +28
  uint32_t unknown_108() const { return guest::LoadU32(base, address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  // the view's +32
  uint32_t unknown_112() const { return guest::LoadU32(base, address + 112); }
  void set_unknown_112(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t unknown_112_address() const { return address + 112; }
  // the view's +36 (+32's + 16)
  uint32_t unknown_116() const { return guest::LoadU32(base, address + 116); }
  void set_unknown_116(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t unknown_116_address() const { return address + 116; }
  // the view's +40
  uint32_t unknown_120() const { return guest::LoadU32(base, address + 120); }
  void set_unknown_120(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t unknown_120_address() const { return address + 120; }
  // the view's +44 (the file's +4 - +8)
  uint32_t unknown_124() const { return guest::LoadU32(base, address + 124); }
  void set_unknown_124(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t unknown_124_address() const { return address + 124; }
  // one an instance (operator new[], no header)
  template <typename T = SkeletonCursor>
  T cursors() const { return T(base, guest::LoadU32(base, address + 128)); }
  void set_cursors(const View& v) const { guest::StoreU32(base, address + 128, v.address); }
  void set_cursors(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t cursors_address() const { return address + 128; }
  // 64 bytes a node (count header) when no matrices were given
  uint32_t own_matrices() const { return guest::LoadU32(base, address + 132); }
  void set_own_matrices(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t own_matrices_address() const { return address + 132; }
  // the nodes' 144-byte records (count header): +0 translation, +16 angles, +32 scale, +64 the channels' cursors
  uint32_t node_states() const { return guest::LoadU32(base, address + 136); }
  void set_node_states(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t node_states_address() const { return address + 136; }
  // node [file +12]'s record in node_states
  uint32_t bone_states() const { return guest::LoadU32(base, address + 140); }
  void set_bone_states(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t bone_states_address() const { return address + 140; }
  // 64 bytes for each of the file's +8 (count header; +52 = 1)
  uint32_t bone_matrices() const { return guest::LoadU32(base, address + 144); }
  void set_bone_matrices(uint32_t v) const { guest::StoreU32(base, address + 144, v); }
  uint32_t bone_matrices_address() const { return address + 144; }
  // the nodes' 64-byte matrices (given, or own_matrices)
  uint32_t matrices() const { return guest::LoadU32(base, address + 148); }
  void set_matrices(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t matrices_address() const { return address + 148; }
  // cleared when made
  uint32_t unknown_152() const { return guest::LoadU32(base, address + 152); }
  void set_unknown_152(uint32_t v) const { guest::StoreU32(base, address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  uint32_t root_index() const { return guest::LoadU32(base, address + 156); }
  void set_root_index(uint32_t v) const { guest::StoreU32(base, address + 156, v); }
  uint32_t root_index_address() const { return address + 156; }
  int32_t instance_count() const { return int32_t(guest::LoadU32(base, address + 160)); }
  void set_instance_count(int32_t v) const { guest::StoreU32(base, address + 160, uint32_t(v)); }
  uint32_t instance_count_address() const { return address + 160; }
  // a float4 a node (x, y, z 0.0 at bind; +8 the node's animation time; operator new[], no header)
  uint32_t node_values() const { return guest::LoadU32(base, address + 176); }
  void set_node_values(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t node_values_address() const { return address + 176; }
};

// file; measure: SkeletonCursor
struct SkeletonCursor : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the +0x28 section (the measure joins it with its entries)
  template <typename T = AnimationSection>
  T section() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_section(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_section(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // the section's +16 (its entry count)'s address
  uint32_t entry_count_pointer() const { return guest::LoadU32(base, address + 4); }
  void set_entry_count_pointer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t entry_count_pointer_address() const { return address + 4; }
  // the section's +20: its 20-byte entries
  template <typename T = SkeletonNodeEntry>
  T entries() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_entries(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t entries_address() const { return address + 8; }
  // the section's +4
  float start_time() const { return guest::LoadFloat(base, address + 12); }
  void set_start_time(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t start_time_address() const { return address + 12; }
  // 0 (Clear, View)
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// file; measure: SkeletonCursor.+8, SkeletonCursor.+4, SkeletonCursor.+0
struct SkeletonNodeEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t node() const { return guest::LoadU32(base, address + 0); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t node_address() const { return address + 0; }
  uint32_t channel_count() const { return guest::LoadU32(base, address + 4); }
  void set_channel_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t channel_count_address() const { return address + 4; }
  // the first channel's offset from the section
  uint32_t channels_offset() const { return guest::LoadU32(base, address + 8); }
  void set_channels_offset(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t channels_offset_address() const { return address + 8; }
  // Skeleton::LoadNodeValues copies it to the node's x and z (its animation time)
  float time() const { return guest::LoadFloat(base, address + 12); }
  void set_time(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t time_address() const { return address + 12; }
  // Skeleton::LoadNodeValues copies it to the node's y
  float unknown_16() const { return guest::LoadFloat(base, address + 16); }
  void set_unknown_16(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// file; measure: PrimitiveDrawer, sub_825A13D8.r3, sub_825A1810.r3
struct PrimitiveDrawer : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr uint32_t kVtable = 0x820721D4;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // operator new(4), freed by the destructor
  template <typename T = PrimitiveDrawerDeclaration>
  T declaration() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_declaration(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_declaration(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t declaration_address() const { return address + 4; }
};

// allocated; measure: new<4>@PrimitiveDrawer::PrimitiveDrawer, sub_825A1810.r3.+4
struct PrimitiveDrawerDeclaration : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // sub_822A8980's
  uint32_t vertex_declaration() const { return guest::LoadU32(base, address + 0); }
  void set_vertex_declaration(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vertex_declaration_address() const { return address + 0; }
};

// file; measure: PhysicalPad
struct PhysicalPad : View {
  using View::View;
  static constexpr uint32_t kSize = 212;
  static constexpr uint32_t kVtable = 0x8206CBF4;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0 when made
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the 40 values (slot 1, PhysicalPad::Values)
  uint32_t values_address() const { return address + 8; }
  // a PhysicalPadRecord
  uint32_t record_a_address() const { return address + 168; }
  // a PhysicalPadRecord
  uint32_t record_b_address() const { return address + 180; }
};

// file; measure: -
struct PhysicalPadRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0.0 when made
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 0 when made
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // 0 when made
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: PhysicalPad@+168
struct PhysicalPadAt168 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(base, address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// The image words docs/functions/model-animation.md's code uses.
struct ModelAnimationGlobals {
  // the game's float 2.0 (Key::Interpolate)
  static float float_two(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_float_two(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t float_two_address() { return 0x820021E8u; }
  // 2 pi (Skeleton::LocalMatrices' angle wrap)
  static float two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA8u); }
  static void set_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA8u, v); }
  static constexpr uint32_t two_pi_address() { return 0x82001FA8u; }
  // 1 / (2 pi) (Skeleton::LocalMatrices' angle wrap)
  static float inverse_two_pi(uint8_t* base) { return guest::LoadFloat(base, 0x82001FA4u); }
  static void set_inverse_two_pi(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FA4u, v); }
  static constexpr uint32_t inverse_two_pi_address() { return 0x82001FA4u; }
};

}  // namespace schema
