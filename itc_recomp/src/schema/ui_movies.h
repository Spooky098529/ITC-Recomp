// The views of docs/functions/ui-movies.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/ui-movies.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: CommandList::AllocateVertices.ret
struct CommandListVertices : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  float v0_x() const { return guest::LoadFloat(base, address + 0); }
  void set_v0_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t v0_x_address() const { return address + 0; }
  float v0_y() const { return guest::LoadFloat(base, address + 4); }
  void set_v0_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t v0_y_address() const { return address + 4; }
  float v0_z() const { return guest::LoadFloat(base, address + 8); }
  void set_v0_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t v0_z_address() const { return address + 8; }
  // ARGB, a byte each
  uint32_t v0_color() const { return guest::LoadU32(base, address + 12); }
  void set_v0_color(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t v0_color_address() const { return address + 12; }
  float v0_u() const { return guest::LoadFloat(base, address + 16); }
  void set_v0_u(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t v0_u_address() const { return address + 16; }
  float v0_v() const { return guest::LoadFloat(base, address + 20); }
  void set_v0_v(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t v0_v_address() const { return address + 20; }
  float v1_x() const { return guest::LoadFloat(base, address + 24); }
  void set_v1_x(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t v1_x_address() const { return address + 24; }
  float v1_y() const { return guest::LoadFloat(base, address + 28); }
  void set_v1_y(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t v1_y_address() const { return address + 28; }
  float v1_z() const { return guest::LoadFloat(base, address + 32); }
  void set_v1_z(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t v1_z_address() const { return address + 32; }
  uint32_t v1_color() const { return guest::LoadU32(base, address + 36); }
  void set_v1_color(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t v1_color_address() const { return address + 36; }
  float v1_u() const { return guest::LoadFloat(base, address + 40); }
  void set_v1_u(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t v1_u_address() const { return address + 40; }
  float v1_v() const { return guest::LoadFloat(base, address + 44); }
  void set_v1_v(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t v1_v_address() const { return address + 44; }
  float v2_x() const { return guest::LoadFloat(base, address + 48); }
  void set_v2_x(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t v2_x_address() const { return address + 48; }
  float v2_y() const { return guest::LoadFloat(base, address + 52); }
  void set_v2_y(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t v2_y_address() const { return address + 52; }
  float v2_z() const { return guest::LoadFloat(base, address + 56); }
  void set_v2_z(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t v2_z_address() const { return address + 56; }
  uint32_t v2_color() const { return guest::LoadU32(base, address + 60); }
  void set_v2_color(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t v2_color_address() const { return address + 60; }
  float v2_u() const { return guest::LoadFloat(base, address + 64); }
  void set_v2_u(float v) const { guest::StoreFloat(base, address + 64, v); }
  uint32_t v2_u_address() const { return address + 64; }
  float v2_v() const { return guest::LoadFloat(base, address + 68); }
  void set_v2_v(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t v2_v_address() const { return address + 68; }
  float v3_x() const { return guest::LoadFloat(base, address + 72); }
  void set_v3_x(float v) const { guest::StoreFloat(base, address + 72, v); }
  uint32_t v3_x_address() const { return address + 72; }
  float v3_y() const { return guest::LoadFloat(base, address + 76); }
  void set_v3_y(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t v3_y_address() const { return address + 76; }
  float v3_z() const { return guest::LoadFloat(base, address + 80); }
  void set_v3_z(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t v3_z_address() const { return address + 80; }
  uint32_t v3_color() const { return guest::LoadU32(base, address + 84); }
  void set_v3_color(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t v3_color_address() const { return address + 84; }
  float v3_u() const { return guest::LoadFloat(base, address + 88); }
  void set_v3_u(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t v3_u_address() const { return address + 88; }
  float v3_v() const { return guest::LoadFloat(base, address + 92); }
  void set_v3_v(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t v3_v_address() const { return address + 92; }
};

// file; measure: sub_825A0EB0.r3, sub_825A0ED8.r3
struct UiMovieFile : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // the scene at the file + this
  uint32_t scene_offset() const { return guest::LoadU32(base, address + 4); }
  void set_scene_offset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t scene_offset_address() const { return address + 4; }
  // X2D\0 then 005\0
  uint32_t magic_address() const { return address + 8; }
  // its animations' first table index
  uint32_t first_animation() const { return guest::LoadU32(base, address + 16); }
  void set_first_animation(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t first_animation_address() const { return address + 16; }
  uint32_t animation_count() const { return guest::LoadU32(base, address + 20); }
  void set_animation_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t animation_count_address() const { return address + 20; }
  // the table's entries
  uint32_t table_size() const { return guest::LoadU32(base, address + 24); }
  void set_table_size(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t table_size_address() const { return address + 24; }
  // the table's first word: 8-byte entries from here, offsets turned into pointers
  uint32_t table() const { return guest::LoadU32(base, address + 32); }
  void set_table(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t table_address() const { return address + 32; }
};

// file; measure: sub_825A0C60.r3
struct UiMovieXmd : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // XMD\0 then 001\0
  uint32_t magic_address() const { return address + 0; }
  // bit 0 doubles the swapped words (the swap never runs)
  uint32_t flags() const { return guest::LoadU32(base, address + 8); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t flags_address() const { return address + 8; }
  uint32_t entry_count() const { return guest::LoadU32(base, address + 12); }
  void set_entry_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t entry_count_address() const { return address + 12; }
  // the first entry's offset: 4 bytes each from here
  uint32_t entries() const { return guest::LoadU32(base, address + 16); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t entries_address() const { return address + 16; }
};

// file; measure: sub_8259D5C8.r3
struct UiMovieChunk : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // TRM\0 at the end
  uint32_t tag() const { return guest::LoadU32(base, address + 0); }
  void set_tag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tag_address() const { return address + 0; }
  // the offset to the next chunk
  uint32_t next() const { return guest::LoadU32(base, address + 4); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t next_address() const { return address + 4; }
};

// file; measure: sub_825A6558.r3, sub_825A6570.r3
struct UiMovieAnimation : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  float length() const { return guest::LoadFloat(base, address + 12); }
  void set_length(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // probably its name's first byte (ui-movies-game.md)
  uint8_t name() const { return guest::LoadU8(base, address + 16); }
  void set_name(uint8_t v) const { guest::StoreU8(base, address + 16, v); }
  uint32_t name_address() const { return address + 16; }
  // entry index
  uint32_t first_node_track_list() const { return guest::LoadU32(base, address + 32); }
  void set_first_node_track_list(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t first_node_track_list_address() const { return address + 32; }
  uint32_t node_track_list_count() const { return guest::LoadU32(base, address + 36); }
  void set_node_track_list_count(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t node_track_list_count_address() const { return address + 36; }
  // entry index
  uint32_t first_key_list() const { return guest::LoadU32(base, address + 40); }
  void set_first_key_list(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t first_key_list_address() const { return address + 40; }
  uint32_t key_list_count() const { return guest::LoadU32(base, address + 44); }
  void set_key_list_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t key_list_count_address() const { return address + 44; }
  // entry index
  uint32_t first_sub_animation() const { return guest::LoadU32(base, address + 48); }
  void set_first_sub_animation(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t first_sub_animation_address() const { return address + 48; }
  uint32_t sub_animation_count() const { return guest::LoadU32(base, address + 52); }
  void set_sub_animation_count(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t sub_animation_count_address() const { return address + 52; }
  // the entries in all
  uint32_t entry_count() const { return guest::LoadU32(base, address + 56); }
  void set_entry_count(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t entry_count_address() const { return address + 56; }
  // the first entry's word: 8-byte entries from here, offsets turned into pointers
  uint32_t entries() const { return guest::LoadU32(base, address + 64); }
  void set_entries(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t entries_address() const { return address + 64; }
};

// file; measure: sub_825A6528.ret
struct UiMovieNodeTrackList : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the node's +8
  uint32_t node_id() const { return guest::LoadU32(base, address + 8); }
  void set_node_id(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_id_address() const { return address + 8; }
  uint32_t track_count() const { return guest::LoadU32(base, address + 12); }
  void set_track_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t track_count_address() const { return address + 12; }
};

// file; measure: sub_825A6540.ret
struct UiMovieKeyList : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t key_count() const { return guest::LoadU32(base, address + 12); }
  void set_key_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t key_count_address() const { return address + 12; }
};

// file; measure: sub_825A6508.ret, UiMovie::ApplyTrack.r4
struct UiMovieTrack : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // its property id (UiMovie::TrackProperty)
  uint32_t property() const { return guest::LoadU32(base, address + 0); }
  void set_property(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t property_address() const { return address + 0; }
  // its key list's index in the animation
  uint32_t key_list() const { return guest::LoadU32(base, address + 4); }
  void set_key_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t key_list_address() const { return address + 4; }
  // the time its keys start at
  float start() const { return guest::LoadFloat(base, address + 8); }
  void set_start(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t start_address() const { return address + 8; }
  // the primitive's vertex for indirect properties
  uint32_t vertex() const { return guest::LoadU32(base, address + 12); }
  void set_vertex(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t vertex_address() const { return address + 12; }
};

// file; measure: sub_825A6518.ret, sub_8259FA20.r3, sub_8259FA20.r4, sub_8259FAA8.r3, sub_8259FAA8.r4, sub_8259FB30.r3, sub_8259FB30.r4, sub_8259FC18.r3, sub_8259FC18.r4, sub_8259FD40.r3, sub_8259FD40.r4
struct UiMovieKey : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // 0 step, 1 linear, 2 cubic, 3 eased
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  float time() const { return guest::LoadFloat(base, address + 4); }
  void set_time(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t time_address() const { return address + 4; }
  // a float track's value
  float value() const { return guest::LoadFloat(base, address + 8); }
  void set_value(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t value_address() const { return address + 8; }
  // an integer track's value
  uint32_t int_value() const { return guest::LoadU32(base, address + 8); }
  void set_int_value(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t int_value_address() const { return address + 8; }
  // probably the tangent's angle arriving at this key
  float tangent_in() const { return guest::LoadFloat(base, address + 12); }
  void set_tangent_in(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t tangent_in_address() const { return address + 12; }
  // probably the tangent's angle leaving the key before
  float tangent_out() const { return guest::LoadFloat(base, address + 16); }
  void set_tangent_out(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t tangent_out_address() const { return address + 16; }
};

// file; measure: sub_825A6558.ret
struct UiMovieSubAnimation : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // 0: the node itself, else its children with this id
  uint32_t node_id() const { return guest::LoadU32(base, address + 8); }
  void set_node_id(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t node_id_address() const { return address + 8; }
  // a table index until fixed up
  template <typename T = UiMovieAnimation>
  T animation() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_animation(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_animation(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t animation_address() const { return address + 16; }
  // its range in the parent's time (+24, +28)
  float start() const { return guest::LoadFloat(base, address + 24); }
  void set_start(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t start_address() const { return address + 24; }
  float end() const { return guest::LoadFloat(base, address + 28); }
  void set_end(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t end_address() const { return address + 28; }
  // mapped onto its own time (+32, +36)
  float map_start() const { return guest::LoadFloat(base, address + 32); }
  void set_map_start(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t map_start_address() const { return address + 32; }
  float map_end() const { return guest::LoadFloat(base, address + 36); }
  void set_map_end(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t map_end_address() const { return address + 36; }
};

// image; measure: UiMovie::TrackProperty.ret
struct UiMovieTrackProperty : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t node_offset() const { return guest::LoadU32(base, address + 0); }
  void set_node_offset(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t node_offset_address() const { return address + 0; }
  // 0 float, 1 signed and 2 unsigned integer
  uint32_t type() const { return guest::LoadU32(base, address + 4); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t type_address() const { return address + 4; }
  // the fill's word: sets << 24, indirect << 16, two leftover stack bytes
  uint32_t flags() const { return guest::LoadU32(base, address + 8); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t flags_address() const { return address + 8; }
  // 1 sets the field, 0 adds to it
  uint8_t sets() const { return guest::LoadU8(base, address + 8); }
  void set_sets(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t sets_address() const { return address + 8; }
  // a primitive's vertex field (the track's +12)
  uint8_t indirect() const { return guest::LoadU8(base, address + 9); }
  void set_indirect(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t indirect_address() const { return address + 9; }
};

// file; measure: sub_8259D618.ret
struct UiMovieTextureEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the index in the movie's textures
  uint32_t texture() const { return guest::LoadU32(base, address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
};

// embedded; measure: sub_8259D9F8.r3, sub_8259D9F8.r4
struct UiMovieDrawContext : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  // a 4x4 matrix
  uint32_t matrix_address() const { return address + 0; }
  // the w column, set to 1.0 before a copy
  float matrix_3() const { return guest::LoadFloat(base, address + 12); }
  void set_matrix_3(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t matrix_3_address() const { return address + 12; }
  float matrix_7() const { return guest::LoadFloat(base, address + 28); }
  void set_matrix_7(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t matrix_7_address() const { return address + 28; }
  float matrix_11() const { return guest::LoadFloat(base, address + 44); }
  void set_matrix_11(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t matrix_11_address() const { return address + 44; }
  float matrix_15() const { return guest::LoadFloat(base, address + 60); }
  void set_matrix_15(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t matrix_15_address() const { return address + 60; }
  // the clip rectangle (integers; all 0 for none)
  uint32_t clip_left() const { return guest::LoadU32(base, address + 64); }
  void set_clip_left(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t clip_left_address() const { return address + 64; }
  uint32_t clip_top() const { return guest::LoadU32(base, address + 68); }
  void set_clip_top(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t clip_top_address() const { return address + 68; }
  uint32_t clip_right() const { return guest::LoadU32(base, address + 72); }
  void set_clip_right(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t clip_right_address() const { return address + 72; }
  uint32_t clip_bottom() const { return guest::LoadU32(base, address + 76); }
  void set_clip_bottom(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t clip_bottom_address() const { return address + 76; }
  // the color (0 to 1)
  float red() const { return guest::LoadFloat(base, address + 80); }
  void set_red(float v) const { guest::StoreFloat(base, address + 80, v); }
  uint32_t red_address() const { return address + 80; }
  float green() const { return guest::LoadFloat(base, address + 84); }
  void set_green(float v) const { guest::StoreFloat(base, address + 84, v); }
  uint32_t green_address() const { return address + 84; }
  float blue() const { return guest::LoadFloat(base, address + 88); }
  void set_blue(float v) const { guest::StoreFloat(base, address + 88, v); }
  uint32_t blue_address() const { return address + 88; }
  float alpha() const { return guest::LoadFloat(base, address + 92); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 92, v); }
  uint32_t alpha_address() const { return address + 92; }
  // the movie's +28
  template <typename T = UiMovieTextureList>
  T textures() const { return T(base, guest::LoadU32(base, address + 96)); }
  void set_textures(const View& v) const { guest::StoreU32(base, address + 96, v.address); }
  void set_textures(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t textures_address() const { return address + 96; }
  // the movie's +40
  template <typename T = UiMovieDrawTextList>
  T text_list() const { return T(base, guest::LoadU32(base, address + 100)); }
  void set_text_list(const View& v) const { guest::StoreU32(base, address + 100, v.address); }
  void set_text_list(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t text_list_address() const { return address + 100; }
  // -1 from the movie update (UiMovie::Draw's r7)
  uint32_t unknown_104() const { return guest::LoadU32(base, address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  // set when a scale isn't 1.0
  uint32_t scaled() const { return guest::LoadU32(base, address + 108); }
  void set_scaled(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t scaled_address() const { return address + 108; }
};

// embedded; measure: UiMovie.+96
struct UiMovieTextureList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the textures' table: a word each, by a texture entry's index
  uint32_t data() const { return guest::LoadU32(base, address + 0); }
  void set_data(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t data_address() const { return address + 0; }
  // 512
  uint32_t capacity() const { return guest::LoadU32(base, address + 4); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t capacity_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(base, address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_address() const { return address + 8; }
};

// embedded; measure: UiMovie.+100
struct UiMovieDrawTextList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = UiMovieTextItem>
  T items() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_items(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_items(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t items_address() const { return address + 0; }
  // 512
  uint32_t capacity() const { return guest::LoadU32(base, address + 4); }
  void set_capacity(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t capacity_address() const { return address + 4; }
  uint32_t count() const { return guest::LoadU32(base, address + 8); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_address() const { return address + 8; }
};

// embedded; measure: sub_8259DAD0.r3.+88, sub_8259E098.r3.+88
struct UiMovieQuadPosition : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  float w() const { return guest::LoadFloat(base, address + 12); }
  void set_w(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t w_address() const { return address + 12; }
};

// embedded; measure: sub_8259DAD0.r3.+92, sub_8259E098.r3.+92
struct UiMovieQuadColor : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t red() const { return guest::LoadU32(base, address + 0); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t red_address() const { return address + 0; }
  // the red word's low byte
  uint8_t red_low() const { return guest::LoadU8(base, address + 3); }
  void set_red_low(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t red_low_address() const { return address + 3; }
  uint32_t green() const { return guest::LoadU32(base, address + 4); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t green_address() const { return address + 4; }
  uint8_t green_low() const { return guest::LoadU8(base, address + 7); }
  void set_green_low(uint8_t v) const { guest::StoreU8(base, address + 7, v); }
  uint32_t green_low_address() const { return address + 7; }
  uint32_t blue() const { return guest::LoadU32(base, address + 8); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t blue_address() const { return address + 8; }
  uint8_t blue_low() const { return guest::LoadU8(base, address + 11); }
  void set_blue_low(uint8_t v) const { guest::StoreU8(base, address + 11, v); }
  uint32_t blue_low_address() const { return address + 11; }
  uint32_t alpha() const { return guest::LoadU32(base, address + 12); }
  void set_alpha(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t alpha_address() const { return address + 12; }
};

// embedded; measure: sub_8259DAD0.r3.+96
struct UiMovieQuadCoord : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  float u() const { return guest::LoadFloat(base, address + 0); }
  void set_u(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t u_address() const { return address + 0; }
  float v() const { return guest::LoadFloat(base, address + 4); }
  void set_v(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t v_address() const { return address + 4; }
};

// embedded; measure: sub_8259D730.r4
struct UiMovieClipRect : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t left() const { return guest::LoadU32(base, address + 0); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  uint32_t top() const { return guest::LoadU32(base, address + 4); }
  void set_top(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t top_address() const { return address + 4; }
  uint32_t right() const { return guest::LoadU32(base, address + 8); }
  void set_right(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t right_address() const { return address + 8; }
  uint32_t bottom() const { return guest::LoadU32(base, address + 12); }
  void set_bottom(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t bottom_address() const { return address + 12; }
};

// allocated; measure: [82970C18]
struct UiMovieRenderer : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = UiMovieRendererVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: [82970C18].+0
struct UiMovieRendererVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // slot 4: r4 a chunk, r5 its key, f1 its depth
  uint32_t submit() const { return guest::LoadU32(base, address + 16); }
  void set_submit(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t submit_address() const { return address + 16; }
};

// The image words docs/functions/ui-movies.md's code uses.
struct UiMoviesGlobals {
  // set by UiMovie::SetRenderer
  template <typename T = UiMovieRenderer>
  static T renderer(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82970C18u)); }
  static void set_renderer(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82970C18u, v.address); }
  static void set_renderer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970C18u, v); }
  static constexpr uint32_t renderer_address() { return 0x82970C18u; }
  // the list the recorders record into (UiMovie::SetRenderer)
  template <typename T = CommandList>
  static T command_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82970C1Cu)); }
  static void set_command_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82970C1Cu, v.address); }
  static void set_command_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970C1Cu, v); }
  static constexpr uint32_t command_list_address() { return 0x82970C1Cu; }
  // set once the descriptors are filled
  static uint8_t track_properties_filled(uint8_t* base) { return guest::LoadU8(base, 0x82970C20u); }
  static void set_track_properties_filled(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x82970C20u, v); }
  static constexpr uint32_t track_properties_filled_address() { return 0x82970C20u; }
  // 64 UiMovieTrackProperty descriptors
  static constexpr uint32_t track_properties_address() { return 0x82970C28u; }
  // bit 0: the table cleared and its clean-up registered
  static uint32_t track_properties_guard(uint8_t* base) { return guest::LoadU32(base, 0x82970F28u); }
  static void set_track_properties_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970F28u, v); }
  static constexpr uint32_t track_properties_guard_address() { return 0x82970F28u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0 (also the quads' depth)
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // the vertex colors' scale to integers
  static float color_to_integer(uint8_t* base) { return guest::LoadFloat(base, 0x82000C44u); }
  static void set_color_to_integer(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000C44u, v); }
  static constexpr uint32_t color_to_integer_address() { return 0x82000C44u; }
  // sub_8259D7B0's second chunk's depth
  static float state_reset_depth(uint8_t* base) { return guest::LoadFloat(base, 0x82001BFCu); }
  static void set_state_reset_depth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BFCu, v); }
  static constexpr uint32_t state_reset_depth_address() { return 0x82001BFCu; }
  // sub_8259D7B0's first chunk's depth
  static float clip_reset_depth(uint8_t* base) { return guest::LoadFloat(base, 0x82001C00u); }
  static void set_clip_reset_depth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001C00u, v); }
  static constexpr uint32_t clip_reset_depth_address() { return 0x82001C00u; }
  // the keys' ease and cubic constant
  static float ease_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820021E4u); }
  static void set_ease_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E4u, v); }
  static constexpr uint32_t ease_scale_address() { return 0x820021E4u; }
  // the nodes' integer colors' scale (probably 1/255)
  static float color_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002200u); }
  static void set_color_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002200u, v); }
  static constexpr uint32_t color_scale_address() { return 0x82002200u; }
  // degrees to radians
  static float degrees(uint8_t* base) { return guest::LoadFloat(base, 0x82002948u); }
  static void set_degrees(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002948u, v); }
  static constexpr uint32_t degrees_address() { return 0x82002948u; }
  // the cubic keys' tangent scale
  static float tangent_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002A38u); }
  static void set_tangent_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A38u, v); }
  static constexpr uint32_t tangent_scale_address() { return 0x82002A38u; }
  // the rotation's, the scale's and the step keys' threshold
  static float near_zero(uint8_t* base) { return guest::LoadFloat(base, 0x82002A44u); }
  static void set_near_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A44u, v); }
  static constexpr uint32_t near_zero_address() { return 0x82002A44u; }
  // the layer to a depth
  static float layer_depth(uint8_t* base) { return guest::LoadFloat(base, 0x82071520u); }
  static void set_layer_depth(uint8_t* base, float v) { guest::StoreFloat(base, 0x82071520u, v); }
  static constexpr uint32_t layer_depth_address() { return 0x82071520u; }
};

}  // namespace schema
