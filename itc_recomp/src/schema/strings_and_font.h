// The views of docs/functions/strings-and-font.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/strings-and-font.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Font
struct Font : View {
  using View::View;
  static constexpr uint32_t kSize = 16592;
  // the device's state object (Font::Font's video output)
  View video_output() const { return View(base, guest::LoadU32(base, address + 0)); }
  void set_video_output(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t video_output_address() const { return address + 0; }
  template <typename T = CommandList>
  T command_list() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_command_list(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_command_list(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t command_list_address() const { return address + 4; }
  template <typename T = RenderQueue>
  T render_queue() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_render_queue(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_render_queue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t render_queue_address() const { return address + 8; }
  // the mask of pages asked for (Font::Load)
  uint32_t pages_asked() const { return guest::LoadU32(base, address + 12); }
  void set_pages_asked(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pages_asked_address() const { return address + 12; }
  // the draw parameters while drawing (Font::SetParams)
  template <typename T = FontDrawParams>
  T params() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_params(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_params(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t params_address() const { return address + 16; }
  // the four pages' pointers (FontPage, 28 bytes each; +4i)
  uint32_t pages_address() const { return address + 20; }
  // the page drawn with (the parameters' +60)
  template <typename T = FontPage>
  T page() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_page(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t page_address() const { return address + 36; }
  // the four pages' files (+4i; assets 22655-22658, the ids at 0x82007C5C)
  uint32_t page_files_address() const { return address + 40; }
  // the text's start point
  float start_x() const { return guest::LoadFloat(base, address + 56); }
  void set_start_x(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t start_x_address() const { return address + 56; }
  float start_y() const { return guest::LoadFloat(base, address + 60); }
  void set_start_y(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t start_y_address() const { return address + 60; }
  // each character's x, y (floats, 8 bytes a character) for the second pass (never in play)
  uint32_t second_pass_positions_address() const { return address + 64; }
  float letter_spacing() const { return guest::LoadFloat(base, address + 16448); }
  void set_letter_spacing(float v) const { guest::StoreFloat(base, address + 16448, v); }
  uint32_t letter_spacing_address() const { return address + 16448; }
  float line_spacing() const { return guest::LoadFloat(base, address + 16452); }
  void set_line_spacing(float v) const { guest::StoreFloat(base, address + 16452, v); }
  uint32_t line_spacing_address() const { return address + 16452; }
  // ten r, g, b colors for the characters 0xFFFF down to 0xFFF6
  uint32_t colors_address() const { return address + 16528; }
  template <typename T = VertexDeclaration>
  T vertex_declaration() const { return T(base, guest::LoadU32(base, address + 16560)); }
  void set_vertex_declaration(const View& v) const { guest::StoreU32(base, address + 16560, v.address); }
  void set_vertex_declaration(uint32_t v) const { guest::StoreU32(base, address + 16560, v); }
  uint32_t vertex_declaration_address() const { return address + 16560; }
  template <typename T = VertexShader>
  T vertex_shader() const { return T(base, guest::LoadU32(base, address + 16564)); }
  void set_vertex_shader(const View& v) const { guest::StoreU32(base, address + 16564, v.address); }
  void set_vertex_shader(uint32_t v) const { guest::StoreU32(base, address + 16564, v); }
  uint32_t vertex_shader_address() const { return address + 16564; }
  // one per texture channel (PixelShader, +4i)
  uint32_t pixel_shaders_address() const { return address + 16568; }
};

// file; measure: Font.+16, Font::SetParams.r4
struct FontDrawParams : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  int32_t x() const { return int32_t(guest::LoadU32(base, address + 0)); }
  void set_x(int32_t v) const { guest::StoreU32(base, address + 0, uint32_t(v)); }
  uint32_t x_address() const { return address + 0; }
  int32_t y() const { return int32_t(guest::LoadU32(base, address + 4)); }
  void set_y(int32_t v) const { guest::StoreU32(base, address + 4, uint32_t(v)); }
  uint32_t y_address() const { return address + 4; }
  // probably the point the scale is about
  int32_t origin_x() const { return int32_t(guest::LoadU32(base, address + 8)); }
  void set_origin_x(int32_t v) const { guest::StoreU32(base, address + 8, uint32_t(v)); }
  uint32_t origin_x_address() const { return address + 8; }
  int32_t origin_y() const { return int32_t(guest::LoadU32(base, address + 12)); }
  void set_origin_y(int32_t v) const { guest::StoreU32(base, address + 12, uint32_t(v)); }
  uint32_t origin_y_address() const { return address + 12; }
  // degrees (rotated text never in play)
  float angle() const { return guest::LoadFloat(base, address + 16); }
  void set_angle(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t angle_address() const { return address + 16; }
  uint8_t red() const { return guest::LoadU8(base, address + 20); }
  void set_red(uint8_t v) const { guest::StoreU8(base, address + 20, v); }
  uint32_t red_address() const { return address + 20; }
  uint8_t green() const { return guest::LoadU8(base, address + 21); }
  void set_green(uint8_t v) const { guest::StoreU8(base, address + 21, v); }
  uint32_t green_address() const { return address + 21; }
  uint8_t blue() const { return guest::LoadU8(base, address + 22); }
  void set_blue(uint8_t v) const { guest::StoreU8(base, address + 22, v); }
  uint32_t blue_address() const { return address + 22; }
  // 0 to 1
  float alpha() const { return guest::LoadFloat(base, address + 24); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t alpha_address() const { return address + 24; }
  float scale_x() const { return guest::LoadFloat(base, address + 28); }
  void set_scale_x(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t scale_x_address() const { return address + 28; }
  float scale_y() const { return guest::LoadFloat(base, address + 32); }
  void set_scale_y(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t scale_y_address() const { return address + 32; }
  float glyph_scale_x() const { return guest::LoadFloat(base, address + 36); }
  void set_glyph_scale_x(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t glyph_scale_x_address() const { return address + 36; }
  float glyph_scale_y() const { return guest::LoadFloat(base, address + 40); }
  void set_glyph_scale_y(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t glyph_scale_y_address() const { return address + 40; }
  // the clip rectangle (all 0 for none)
  uint32_t clip_x() const { return guest::LoadU32(base, address + 44); }
  void set_clip_x(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t clip_x_address() const { return address + 44; }
  uint32_t clip_y() const { return guest::LoadU32(base, address + 48); }
  void set_clip_y(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t clip_y_address() const { return address + 48; }
  uint32_t clip_width() const { return guest::LoadU32(base, address + 52); }
  void set_clip_width(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t clip_width_address() const { return address + 52; }
  uint32_t clip_height() const { return guest::LoadU32(base, address + 56); }
  void set_clip_height(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t clip_height_address() const { return address + 56; }
  // the font page
  uint32_t page() const { return guest::LoadU32(base, address + 60); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t page_address() const { return address + 60; }
  uint32_t box_width() const { return guest::LoadU32(base, address + 68); }
  void set_box_width(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t box_width_address() const { return address + 68; }
  uint32_t box_height() const { return guest::LoadU32(base, address + 72); }
  void set_box_height(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t box_height_address() const { return address + 72; }
  // 0 left, 1 center, 2 right, 3 justified
  uint32_t align_x() const { return guest::LoadU32(base, address + 76); }
  void set_align_x(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t align_x_address() const { return address + 76; }
  // 0 top, 1 middle, 2 bottom, 3 justified
  uint32_t align_y() const { return guest::LoadU32(base, address + 80); }
  void set_align_y(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t align_y_address() const { return address + 80; }
  int32_t letter_spacing() const { return int32_t(guest::LoadU32(base, address + 84)); }
  void set_letter_spacing(int32_t v) const { guest::StoreU32(base, address + 84, uint32_t(v)); }
  uint32_t letter_spacing_address() const { return address + 84; }
  int32_t line_spacing() const { return int32_t(guest::LoadU32(base, address + 88)); }
  void set_line_spacing(int32_t v) const { guest::StoreU32(base, address + 88, uint32_t(v)); }
  uint32_t line_spacing_address() const { return address + 88; }
  // probably the second pass's color
  uint8_t unknown_92() const { return guest::LoadU8(base, address + 92); }
  void set_unknown_92(uint8_t v) const { guest::StoreU8(base, address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint8_t unknown_93() const { return guest::LoadU8(base, address + 93); }
  void set_unknown_93(uint8_t v) const { guest::StoreU8(base, address + 93, v); }
  uint32_t unknown_93_address() const { return address + 93; }
  uint8_t unknown_94() const { return guest::LoadU8(base, address + 94); }
  void set_unknown_94(uint8_t v) const { guest::StoreU8(base, address + 94, v); }
  uint32_t unknown_94_address() const { return address + 94; }
  // the second pass's offset
  float second_offset_x() const { return guest::LoadFloat(base, address + 96); }
  void set_second_offset_x(float v) const { guest::StoreFloat(base, address + 96, v); }
  uint32_t second_offset_x_address() const { return address + 96; }
  float second_offset_y() const { return guest::LoadFloat(base, address + 100); }
  void set_second_offset_y(float v) const { guest::StoreFloat(base, address + 100, v); }
  uint32_t second_offset_y_address() const { return address + 100; }
  float unknown_104() const { return guest::LoadFloat(base, address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(base, address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t second_page() const { return guest::LoadU32(base, address + 108); }
  void set_second_page(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t second_page_address() const { return address + 108; }
  // the second pass on (never in play)
  uint32_t second_pass() const { return guest::LoadU32(base, address + 112); }
  void set_second_pass(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t second_pass_address() const { return address + 112; }
  uint32_t layer() const { return guest::LoadU32(base, address + 116); }
  void set_layer(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t layer_address() const { return address + 116; }
  uint32_t text_id() const { return guest::LoadU32(base, address + 120); }
  void set_text_id(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t text_id_address() const { return address + 120; }
};

// allocated; measure: Font.+36, new<28>@Font::Load, Font::OnPageLoaded.r6
struct FontPage : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // the page file's entry 0
  template <typename T = FontGlyphTable>
  T glyph_table() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_glyph_table(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_glyph_table(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t glyph_table_address() const { return address + 0; }
  // the glyph table's +4: 4 bytes per UTF-16 code
  template <typename T = FontGlyphRecord>
  T records() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_records(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_records(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t records_address() const { return address + 4; }
  // the page file's entry 1
  template <typename T = XPR>
  T xpr() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_xpr(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_xpr(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t xpr_address() const { return address + 8; }
  // the XPR's texture (four glyph sheets, one per channel)
  View texture() const { return View(base, guest::LoadU32(base, address + 12)); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t texture_address() const { return address + 12; }
  uint32_t texture_width() const { return guest::LoadU32(base, address + 16); }
  void set_texture_width(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t texture_width_address() const { return address + 16; }
  uint32_t texture_height() const { return guest::LoadU32(base, address + 20); }
  void set_texture_height(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t texture_height_address() const { return address + 20; }
  uint32_t loaded() const { return guest::LoadU32(base, address + 24); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t loaded_address() const { return address + 24; }
};

// file; measure: Font.+36.+0
struct FontGlyphTable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t cell_width() const { return guest::LoadU16(base, address + 0); }
  void set_cell_width(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t cell_width_address() const { return address + 0; }
  uint16_t cell_height() const { return guest::LoadU16(base, address + 2); }
  void set_cell_height(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t cell_height_address() const { return address + 2; }
};

// file; measure: Font.+36.+4
struct FontGlyphRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the cell's index in the sheet
  uint16_t cell() const { return guest::LoadU16(base, address + 0); }
  void set_cell(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t cell_address() const { return address + 0; }
  // the advance x 4 plus the texture channel 0-3
  uint16_t advance_and_channel() const { return guest::LoadU16(base, address + 2); }
  void set_advance_and_channel(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t advance_and_channel_address() const { return address + 2; }
};

// file; measure: Font::BuildVertices.r4, Font::CountGlyphs.r5, Font::LineGlyphs.r4, Font::LineWidth.r4, Strings::Get.ret
struct FontText : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t unit() const { return guest::LoadU16(base, address + 0); }
  void set_unit(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t unit_address() const { return address + 0; }
  uint16_t next_unit() const { return guest::LoadU16(base, address + 2); }
  void set_next_unit(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t next_unit_address() const { return address + 2; }
};

// file; measure: Font::ClipGlyph.r4
struct FontGlyphQuad : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // six FontVertex
  uint32_t vertices_address() const { return address + 0; }
};

// file; measure: Font::OutCode.r4
struct FontVertex : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  uint32_t color() const { return guest::LoadU32(base, address + 12); }
  void set_color(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t color_address() const { return address + 12; }
  float u() const { return guest::LoadFloat(base, address + 16); }
  void set_u(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t u_address() const { return address + 16; }
  float v() const { return guest::LoadFloat(base, address + 20); }
  void set_v(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t v_address() const { return address + 20; }
};

// allocated; measure: Strings
struct Strings : View {
  using View::View;
  static constexpr uint32_t kSize = 4304;
  // the 17 string files (+4i; assets 22638-22654, the ids at 0x82007DD0)
  uint32_t files_address() const { return address + 0; }
  // each file's loaded flag (+4i; StringsLoadedFlag)
  uint32_t loaded_flags_address() const { return address + 68; }
  // the mask of files asked for
  uint32_t files_asked() const { return guest::LoadU32(base, address + 136); }
  void set_files_asked(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t files_asked_address() const { return address + 136; }
  // each file's count of loads (+4i)
  uint32_t load_counts_address() const { return address + 4236; }
};

// file; measure: Strings::OnLoaded.r6
struct StringsLoadedFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t loaded() const { return guest::LoadU32(base, address + 0); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t loaded_address() const { return address + 0; }
};

// The image words docs/functions/strings-and-font.md's code uses.
struct StringsAndFontGlobals {
  template <typename T = Font>
  static T font(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C00u)); }
  static void set_font(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C00u, v.address); }
  static void set_font(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C00u, v); }
  static constexpr uint32_t font_address() { return 0x826C5C00u; }
  template <typename T = Strings>
  static T strings(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5C34u)); }
  static void set_strings(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5C34u, v.address); }
  static void set_strings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t strings_address() { return 0x826C5C34u; }
  // the four pixel shaders' sources, 12 bytes each: the HLSL, its length, 0 (sub_825D9590)
  static constexpr uint32_t shader_entries_address() { return 0x826C5C04u; }
  // the four pixel shaders' HLSL pointers
  static constexpr uint32_t pixel_shader_sources_address() { return 0x8260333Cu; }
  // Font::DrawPass: the glyphs per texture channel (+4i)
  static constexpr uint32_t glyph_counts_address() { return 0x826C5BCCu; }
  // the channels' vertex write pointers (+4i)
  static constexpr uint32_t write_pointers_address() { return 0x826C5BDCu; }
  // the channels' vertices' starts (+4i)
  static constexpr uint32_t vertex_starts_address() { return 0x826C5BECu; }
  // the chunk's first slot while drawing (0 between texts)
  static uint32_t chunk(uint8_t* base) { return guest::LoadU32(base, 0x826C5BFCu); }
  static void set_chunk(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BFCu, v); }
  static constexpr uint32_t chunk_address() { return 0x826C5BFCu; }
  // the four pages' asset ids
  static constexpr uint32_t font_page_ids_address() { return 0x82007C5Cu; }
  // the 17 files' asset ids
  static constexpr uint32_t string_file_ids_address() { return 0x82007DD0u; }
  // the 18 string id bounds (signed words)
  static constexpr uint32_t string_bounds_address() { return 0x82007E18u; }
  // L"-%s"
  static constexpr uint32_t minus_format_address() { return 0x82007E8Cu; }
  // L"%d"
  static constexpr uint32_t plain_format_address() { return 0x82007E94u; }
  // L",%03d"
  static constexpr uint32_t grouped_format_address() { return 0x82007E9Cu; }
  // L"%%%dd"
  static constexpr uint32_t int_format_address() { return 0x82007EA8u; }
  // L"%%.%df"
  static constexpr uint32_t float_format_address() { return 0x82007EB4u; }
  // L"%%0%dd"
  static constexpr uint32_t zeroed_format_address() { return 0x82007EC4u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 0.001
  static float thousandth(uint8_t* base) { return guest::LoadFloat(base, 0x820029D8u); }
  static void set_thousandth(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D8u, v); }
  static constexpr uint32_t thousandth_address() { return 0x820029D8u; }
  // 255.0
  static float full_alpha(uint8_t* base) { return guest::LoadFloat(base, 0x82000C44u); }
  static void set_full_alpha(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000C44u, v); }
  static constexpr uint32_t full_alpha_address() { return 0x82000C44u; }
  // 1.0 (read as two words, lfd)
  static double one_double(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82002250u)); }
  static void set_one_double(uint8_t* base, double v) { guest::StoreU64(base, 0x82002250u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t one_double_address() { return 0x82002250u; }
  // 1/360 (read as two words, lfd)
  static double turn(uint8_t* base) { return std::bit_cast<double>(guest::LoadU64(base, 0x82007C78u)); }
  static void set_turn(uint8_t* base, double v) { guest::StoreU64(base, 0x82007C78u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t turn_address() { return 0x82007C78u; }
};

}  // namespace schema
