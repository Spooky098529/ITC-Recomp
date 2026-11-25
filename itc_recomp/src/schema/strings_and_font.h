// The views of docs/functions/strings-and-font.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/strings-and-font.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Font
#pragma pack(push, 1)
struct FontData {
  uint32_t video_output = 0;  // +0
  uint32_t command_list = 0;  // +4
  uint32_t render_queue = 0;  // +8
  uint32_t pages_asked = 0;  // +12
  uint32_t params = 0;  // +16
  uint8_t pages[16] = {};  // +20, guest order
  uint32_t page = 0;  // +36
  uint8_t page_files[16] = {};  // +40, guest order
  float start_x = 0;  // +56
  float start_y = 0;  // +60
  uint8_t second_pass_positions[16384] = {};  // +64, guest order
  float letter_spacing = 0;  // +16448
  float line_spacing = 0;  // +16452
  uint8_t pad_16456[72] = {};
  uint8_t colors[30] = {};  // +16528, guest order
  uint8_t pad_16558[2] = {};
  uint32_t vertex_declaration = 0;  // +16560
  uint32_t vertex_shader = 0;  // +16564
  uint8_t pixel_shaders[16] = {};  // +16568, guest order
  uint8_t pad_16584[8] = {};
};
#pragma pack(pop)
static_assert(sizeof(FontData) == 16592);
inline constexpr memory::LayoutField kFontFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {36, 4}, {56, 4}, {60, 4}, {16448, 4}, {16452, 4}, {16560, 4}, {16564, 4}};
inline constexpr memory::Layout kFontLayout = {kFontFields, 12};

struct Font : View {
  using View::View;
  static constexpr uint32_t kSize = 16592;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  FontData* data() const { return static_cast<FontData*>(memory::HandleObject(address)); }
  static Font New() {
    return Font(memory::MakeHandle(new FontData(), sizeof(FontData), "Font", &kFontLayout));
  }
  template <uint32_t Stride = kSize>
  static Font NewArray(uint32_t count) {
    static_assert(sizeof(FontData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FontData*>(::operator new(sizeof(FontData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FontData();
    return Font(memory::MakeHandle(p, sizeof(FontData) * count, "Font", &kFontLayout, sizeof(FontData)));
  }
  Font at(uint32_t i) const { return Font(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  FontData* direct() const { return static_cast<FontData*>(memory::HandleOwn(address, &kFontLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kFontLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kFontLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the device's state object (Font::Font's video output)
  uint32_t video_output_address() const { return address + 0; }
  uint32_t video_output_bits() const { if (auto* d = direct()) return d->video_output; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_video_output_bits(uint32_t v) const { if (auto* d = direct()) { d->video_output = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  View video_output() const { return View(video_output_bits()); }
  void set_video_output(uint32_t v) const { set_video_output_bits(v); }
  uint32_t command_list_address() const { return address + 4; }
  uint32_t command_list_bits() const { if (auto* d = direct()) return d->command_list; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_command_list_bits(uint32_t v) const { if (auto* d = direct()) { d->command_list = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = CommandList>
  T command_list() const { return T(command_list_bits()); }
  void set_command_list(const View& w) const { set_command_list_bits(w.address); }
  void set_command_list(uint32_t v) const { set_command_list_bits(v); }
  uint32_t render_queue_address() const { return address + 8; }
  uint32_t render_queue_bits() const { if (auto* d = direct()) return d->render_queue; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_render_queue_bits(uint32_t v) const { if (auto* d = direct()) { d->render_queue = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = RenderQueue>
  T render_queue() const { return T(render_queue_bits()); }
  void set_render_queue(const View& w) const { set_render_queue_bits(w.address); }
  void set_render_queue(uint32_t v) const { set_render_queue_bits(v); }
  // the mask of pages asked for (Font::Load)
  uint32_t pages_asked_address() const { return address + 12; }
  uint32_t pages_asked_bits() const { if (auto* d = direct()) return d->pages_asked; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_pages_asked_bits(uint32_t v) const { if (auto* d = direct()) { d->pages_asked = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t pages_asked() const { if (auto* d = direct()) return d->pages_asked; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_pages_asked(uint32_t v) const { if (auto* d = direct()) { d->pages_asked = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the draw parameters while drawing (Font::SetParams)
  uint32_t params_address() const { return address + 16; }
  uint32_t params_bits() const { if (auto* d = direct()) return d->params; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_params_bits(uint32_t v) const { if (auto* d = direct()) { d->params = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  template <typename T = FontDrawParams>
  T params() const { return T(params_bits()); }
  void set_params(const View& w) const { set_params_bits(w.address); }
  void set_params(uint32_t v) const { set_params_bits(v); }
  // the four pages' pointers (FontPage, 28 bytes each; +4i)
  uint32_t pages_address() const { return address + 20; }
  uint8_t* pages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 20 : guest::Pointer(address + 20); }
  // the page drawn with (the parameters' +60)
  uint32_t page_address() const { return address + 36; }
  uint32_t page_bits() const { if (auto* d = direct()) return d->page; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_page_bits(uint32_t v) const { if (auto* d = direct()) { d->page = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  template <typename T = FontPage>
  T page() const { return T(page_bits()); }
  void set_page(const View& w) const { set_page_bits(w.address); }
  void set_page(uint32_t v) const { set_page_bits(v); }
  // the four pages' files (+4i; assets 22655-22658, the ids at 0x82007C5C)
  uint32_t page_files_address() const { return address + 40; }
  uint8_t* page_files() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 40 : guest::Pointer(address + 40); }
  // the text's start point
  uint32_t start_x_address() const { return address + 56; }
  uint32_t start_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->start_x); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_start_x_bits(uint32_t v) const { if (auto* d = direct()) { d->start_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  float start_x() const { return std::bit_cast<float>(start_x_bits()); }
  void set_start_x(float v) const { set_start_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t start_y_address() const { return address + 60; }
  uint32_t start_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->start_y); return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_start_y_bits(uint32_t v) const { if (auto* d = direct()) { d->start_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  float start_y() const { return std::bit_cast<float>(start_y_bits()); }
  void set_start_y(float v) const { set_start_y_bits(std::bit_cast<uint32_t>(v)); }
  // each character's x, y (floats, 8 bytes a character) for the second pass (never in play)
  uint32_t second_pass_positions_address() const { return address + 64; }
  uint8_t* second_pass_positions() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 64 : guest::Pointer(address + 64); }
  uint32_t letter_spacing_address() const { return address + 16448; }
  uint32_t letter_spacing_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->letter_spacing); return uint32_t((memory::IsHandle(address + 16448) ? uint32_t(memory::OwnerLoad(address + 16448, 4)) : guest::LoadU32(address + 16448))); }
  void set_letter_spacing_bits(uint32_t v) const { if (auto* d = direct()) { d->letter_spacing = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16448)) memory::OwnerStore(address + 16448, 4, v); else guest::StoreU32(address + 16448, v); }
  float letter_spacing() const { return std::bit_cast<float>(letter_spacing_bits()); }
  void set_letter_spacing(float v) const { set_letter_spacing_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t line_spacing_address() const { return address + 16452; }
  uint32_t line_spacing_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->line_spacing); return uint32_t((memory::IsHandle(address + 16452) ? uint32_t(memory::OwnerLoad(address + 16452, 4)) : guest::LoadU32(address + 16452))); }
  void set_line_spacing_bits(uint32_t v) const { if (auto* d = direct()) { d->line_spacing = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16452)) memory::OwnerStore(address + 16452, 4, v); else guest::StoreU32(address + 16452, v); }
  float line_spacing() const { return std::bit_cast<float>(line_spacing_bits()); }
  void set_line_spacing(float v) const { set_line_spacing_bits(std::bit_cast<uint32_t>(v)); }
  // ten r, g, b colors for the characters 0xFFFF down to 0xFFF6
  uint32_t colors_address() const { return address + 16528; }
  uint8_t* colors() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16528 : guest::Pointer(address + 16528); }
  uint32_t vertex_declaration_address() const { return address + 16560; }
  uint32_t vertex_declaration_bits() const { if (auto* d = direct()) return d->vertex_declaration; return uint32_t((memory::IsHandle(address + 16560) ? uint32_t(memory::OwnerLoad(address + 16560, 4)) : guest::LoadU32(address + 16560))); }
  void set_vertex_declaration_bits(uint32_t v) const { if (auto* d = direct()) { d->vertex_declaration = v; return; } if (memory::IsHandle(address + 16560)) memory::OwnerStore(address + 16560, 4, v); else guest::StoreU32(address + 16560, v); }
  template <typename T = VertexDeclaration>
  T vertex_declaration() const { return T(vertex_declaration_bits()); }
  void set_vertex_declaration(const View& w) const { set_vertex_declaration_bits(w.address); }
  void set_vertex_declaration(uint32_t v) const { set_vertex_declaration_bits(v); }
  uint32_t vertex_shader_address() const { return address + 16564; }
  uint32_t vertex_shader_bits() const { if (auto* d = direct()) return d->vertex_shader; return uint32_t((memory::IsHandle(address + 16564) ? uint32_t(memory::OwnerLoad(address + 16564, 4)) : guest::LoadU32(address + 16564))); }
  void set_vertex_shader_bits(uint32_t v) const { if (auto* d = direct()) { d->vertex_shader = v; return; } if (memory::IsHandle(address + 16564)) memory::OwnerStore(address + 16564, 4, v); else guest::StoreU32(address + 16564, v); }
  template <typename T = VertexShader>
  T vertex_shader() const { return T(vertex_shader_bits()); }
  void set_vertex_shader(const View& w) const { set_vertex_shader_bits(w.address); }
  void set_vertex_shader(uint32_t v) const { set_vertex_shader_bits(v); }
  // one per texture channel (PixelShader, +4i)
  uint32_t pixel_shaders_address() const { return address + 16568; }
  uint8_t* pixel_shaders() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16568 : guest::Pointer(address + 16568); }
};

// file; measure: Font.+16, Font::SetParams.r4
struct FontDrawParams : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  int32_t x() const { return int32_t(guest::LoadU32(address + 0)); }
  void set_x(int32_t v) const { guest::StoreU32(address + 0, uint32_t(v)); }
  uint32_t x_address() const { return address + 0; }
  int32_t y() const { return int32_t(guest::LoadU32(address + 4)); }
  void set_y(int32_t v) const { guest::StoreU32(address + 4, uint32_t(v)); }
  uint32_t y_address() const { return address + 4; }
  // probably the point the scale is about
  int32_t origin_x() const { return int32_t(guest::LoadU32(address + 8)); }
  void set_origin_x(int32_t v) const { guest::StoreU32(address + 8, uint32_t(v)); }
  uint32_t origin_x_address() const { return address + 8; }
  int32_t origin_y() const { return int32_t(guest::LoadU32(address + 12)); }
  void set_origin_y(int32_t v) const { guest::StoreU32(address + 12, uint32_t(v)); }
  uint32_t origin_y_address() const { return address + 12; }
  // degrees (rotated text never in play)
  float angle() const { return guest::LoadFloat(address + 16); }
  void set_angle(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t angle_address() const { return address + 16; }
  uint8_t red() const { return guest::LoadU8(address + 20); }
  void set_red(uint8_t v) const { guest::StoreU8(address + 20, v); }
  uint32_t red_address() const { return address + 20; }
  uint8_t green() const { return guest::LoadU8(address + 21); }
  void set_green(uint8_t v) const { guest::StoreU8(address + 21, v); }
  uint32_t green_address() const { return address + 21; }
  uint8_t blue() const { return guest::LoadU8(address + 22); }
  void set_blue(uint8_t v) const { guest::StoreU8(address + 22, v); }
  uint32_t blue_address() const { return address + 22; }
  // 0 to 1
  float alpha() const { return guest::LoadFloat(address + 24); }
  void set_alpha(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t alpha_address() const { return address + 24; }
  float scale_x() const { return guest::LoadFloat(address + 28); }
  void set_scale_x(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t scale_x_address() const { return address + 28; }
  float scale_y() const { return guest::LoadFloat(address + 32); }
  void set_scale_y(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t scale_y_address() const { return address + 32; }
  float glyph_scale_x() const { return guest::LoadFloat(address + 36); }
  void set_glyph_scale_x(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t glyph_scale_x_address() const { return address + 36; }
  float glyph_scale_y() const { return guest::LoadFloat(address + 40); }
  void set_glyph_scale_y(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t glyph_scale_y_address() const { return address + 40; }
  // the clip rectangle (all 0 for none)
  uint32_t clip_x() const { return guest::LoadU32(address + 44); }
  void set_clip_x(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t clip_x_address() const { return address + 44; }
  uint32_t clip_y() const { return guest::LoadU32(address + 48); }
  void set_clip_y(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t clip_y_address() const { return address + 48; }
  uint32_t clip_width() const { return guest::LoadU32(address + 52); }
  void set_clip_width(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t clip_width_address() const { return address + 52; }
  uint32_t clip_height() const { return guest::LoadU32(address + 56); }
  void set_clip_height(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t clip_height_address() const { return address + 56; }
  // the font page
  uint32_t page() const { return guest::LoadU32(address + 60); }
  void set_page(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t page_address() const { return address + 60; }
  uint32_t box_width() const { return guest::LoadU32(address + 68); }
  void set_box_width(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t box_width_address() const { return address + 68; }
  uint32_t box_height() const { return guest::LoadU32(address + 72); }
  void set_box_height(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t box_height_address() const { return address + 72; }
  // 0 left, 1 center, 2 right, 3 justified
  uint32_t align_x() const { return guest::LoadU32(address + 76); }
  void set_align_x(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t align_x_address() const { return address + 76; }
  // 0 top, 1 middle, 2 bottom, 3 justified
  uint32_t align_y() const { return guest::LoadU32(address + 80); }
  void set_align_y(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t align_y_address() const { return address + 80; }
  int32_t letter_spacing() const { return int32_t(guest::LoadU32(address + 84)); }
  void set_letter_spacing(int32_t v) const { guest::StoreU32(address + 84, uint32_t(v)); }
  uint32_t letter_spacing_address() const { return address + 84; }
  int32_t line_spacing() const { return int32_t(guest::LoadU32(address + 88)); }
  void set_line_spacing(int32_t v) const { guest::StoreU32(address + 88, uint32_t(v)); }
  uint32_t line_spacing_address() const { return address + 88; }
  // probably the second pass's color
  uint8_t unknown_92() const { return guest::LoadU8(address + 92); }
  void set_unknown_92(uint8_t v) const { guest::StoreU8(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint8_t unknown_93() const { return guest::LoadU8(address + 93); }
  void set_unknown_93(uint8_t v) const { guest::StoreU8(address + 93, v); }
  uint32_t unknown_93_address() const { return address + 93; }
  uint8_t unknown_94() const { return guest::LoadU8(address + 94); }
  void set_unknown_94(uint8_t v) const { guest::StoreU8(address + 94, v); }
  uint32_t unknown_94_address() const { return address + 94; }
  // the second pass's offset
  float second_offset_x() const { return guest::LoadFloat(address + 96); }
  void set_second_offset_x(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t second_offset_x_address() const { return address + 96; }
  float second_offset_y() const { return guest::LoadFloat(address + 100); }
  void set_second_offset_y(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t second_offset_y_address() const { return address + 100; }
  float unknown_104() const { return guest::LoadFloat(address + 104); }
  void set_unknown_104(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t second_page() const { return guest::LoadU32(address + 108); }
  void set_second_page(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t second_page_address() const { return address + 108; }
  // the second pass on (never in play)
  uint32_t second_pass() const { return guest::LoadU32(address + 112); }
  void set_second_pass(uint32_t v) const { guest::StoreU32(address + 112, v); }
  uint32_t second_pass_address() const { return address + 112; }
  uint32_t layer() const { return guest::LoadU32(address + 116); }
  void set_layer(uint32_t v) const { guest::StoreU32(address + 116, v); }
  uint32_t layer_address() const { return address + 116; }
  uint32_t text_id() const { return guest::LoadU32(address + 120); }
  void set_text_id(uint32_t v) const { guest::StoreU32(address + 120, v); }
  uint32_t text_id_address() const { return address + 120; }
};

// allocated; measure: Font.+36, new<28>@Font::Load, Font::OnPageLoaded.r6
#pragma pack(push, 1)
struct FontPageData {
  uint32_t glyph_table = 0;  // +0
  uint32_t records = 0;  // +4
  uint32_t xpr = 0;  // +8
  uint32_t texture = 0;  // +12
  uint32_t texture_width = 0;  // +16
  uint32_t texture_height = 0;  // +20
  uint32_t loaded = 0;  // +24
};
#pragma pack(pop)
static_assert(sizeof(FontPageData) == 28);
inline constexpr memory::LayoutField kFontPageFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}};
inline constexpr memory::Layout kFontPageLayout = {kFontPageFields, 7};

struct FontPage : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  FontPageData* data() const { return static_cast<FontPageData*>(memory::HandleObject(address)); }
  static FontPage New() {
    return FontPage(memory::MakeHandle(new FontPageData(), sizeof(FontPageData), "FontPage", &kFontPageLayout));
  }
  template <uint32_t Stride = kSize>
  static FontPage NewArray(uint32_t count) {
    static_assert(sizeof(FontPageData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FontPageData*>(::operator new(sizeof(FontPageData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FontPageData();
    return FontPage(memory::MakeHandle(p, sizeof(FontPageData) * count, "FontPage", &kFontPageLayout, sizeof(FontPageData)));
  }
  FontPage at(uint32_t i) const { return FontPage(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  FontPageData* direct() const { return static_cast<FontPageData*>(memory::HandleOwn(address, &kFontPageLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kFontPageLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kFontPageLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the page file's entry 0
  uint32_t glyph_table_address() const { return address + 0; }
  uint32_t glyph_table_bits() const { if (auto* d = direct()) return d->glyph_table; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_glyph_table_bits(uint32_t v) const { if (auto* d = direct()) { d->glyph_table = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = FontGlyphTable>
  T glyph_table() const { return T(glyph_table_bits()); }
  void set_glyph_table(const View& w) const { set_glyph_table_bits(w.address); }
  void set_glyph_table(uint32_t v) const { set_glyph_table_bits(v); }
  // the glyph table's +4: 4 bytes per UTF-16 code
  uint32_t records_address() const { return address + 4; }
  uint32_t records_bits() const { if (auto* d = direct()) return d->records; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_records_bits(uint32_t v) const { if (auto* d = direct()) { d->records = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = FontGlyphRecord>
  T records() const { return T(records_bits()); }
  void set_records(const View& w) const { set_records_bits(w.address); }
  void set_records(uint32_t v) const { set_records_bits(v); }
  // the page file's entry 1
  uint32_t xpr_address() const { return address + 8; }
  uint32_t xpr_bits() const { if (auto* d = direct()) return d->xpr; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_xpr_bits(uint32_t v) const { if (auto* d = direct()) { d->xpr = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  template <typename T = XPR>
  T xpr() const { return T(xpr_bits()); }
  void set_xpr(const View& w) const { set_xpr_bits(w.address); }
  void set_xpr(uint32_t v) const { set_xpr_bits(v); }
  // the XPR's texture (four glyph sheets, one per channel)
  uint32_t texture_address() const { return address + 12; }
  uint32_t texture_bits() const { if (auto* d = direct()) return d->texture; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_texture_bits(uint32_t v) const { if (auto* d = direct()) { d->texture = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  View texture() const { return View(texture_bits()); }
  void set_texture(uint32_t v) const { set_texture_bits(v); }
  uint32_t texture_width_address() const { return address + 16; }
  uint32_t texture_width_bits() const { if (auto* d = direct()) return d->texture_width; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_texture_width_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_width = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t texture_width() const { if (auto* d = direct()) return d->texture_width; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_texture_width(uint32_t v) const { if (auto* d = direct()) { d->texture_width = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t texture_height_address() const { return address + 20; }
  uint32_t texture_height_bits() const { if (auto* d = direct()) return d->texture_height; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_texture_height_bits(uint32_t v) const { if (auto* d = direct()) { d->texture_height = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t texture_height() const { if (auto* d = direct()) return d->texture_height; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_texture_height(uint32_t v) const { if (auto* d = direct()) { d->texture_height = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t loaded_address() const { return address + 24; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
};

// file; measure: Font.+36.+0
struct FontGlyphTable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t cell_width() const { return guest::LoadU16(address + 0); }
  void set_cell_width(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t cell_width_address() const { return address + 0; }
  uint16_t cell_height() const { return guest::LoadU16(address + 2); }
  void set_cell_height(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t cell_height_address() const { return address + 2; }
};

// file; measure: Font.+36.+4
struct FontGlyphRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the cell's index in the sheet
  uint16_t cell() const { return guest::LoadU16(address + 0); }
  void set_cell(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t cell_address() const { return address + 0; }
  // the advance x 4 plus the texture channel 0-3
  uint16_t advance_and_channel() const { return guest::LoadU16(address + 2); }
  void set_advance_and_channel(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t advance_and_channel_address() const { return address + 2; }
};

// file; measure: Font::BuildVertices.r4, Font::CountGlyphs.r5, Font::LineGlyphs.r4, Font::LineWidth.r4, Strings::Get.ret
struct FontText : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint16_t unit() const { return guest::LoadU16(address + 0); }
  void set_unit(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t unit_address() const { return address + 0; }
  uint16_t next_unit() const { return guest::LoadU16(address + 2); }
  void set_next_unit(uint16_t v) const { guest::StoreU16(address + 2, v); }
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
  float x() const { return guest::LoadFloat(address + 0); }
  void set_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  float y() const { return guest::LoadFloat(address + 4); }
  void set_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  float z() const { return guest::LoadFloat(address + 8); }
  void set_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t z_address() const { return address + 8; }
  uint32_t color() const { return guest::LoadU32(address + 12); }
  void set_color(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t color_address() const { return address + 12; }
  float u() const { return guest::LoadFloat(address + 16); }
  void set_u(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t u_address() const { return address + 16; }
  float v() const { return guest::LoadFloat(address + 20); }
  void set_v(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t v_address() const { return address + 20; }
};

// allocated; measure: Strings
#pragma pack(push, 1)
struct StringsData {
  uint8_t files[68] = {};  // +0, guest order
  uint8_t loaded_flags[68] = {};  // +68, guest order
  uint32_t files_asked = 0;  // +136
  uint8_t pad_140[4096] = {};
  uint8_t load_counts[68] = {};  // +4236, guest order
};
#pragma pack(pop)
static_assert(sizeof(StringsData) == 4304);
inline constexpr memory::LayoutField kStringsFields[] = {{136, 4}};
inline constexpr memory::Layout kStringsLayout = {kStringsFields, 1};

struct Strings : View {
  using View::View;
  static constexpr uint32_t kSize = 4304;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  StringsData* data() const { return static_cast<StringsData*>(memory::HandleObject(address)); }
  static Strings New() {
    return Strings(memory::MakeHandle(new StringsData(), sizeof(StringsData), "Strings", &kStringsLayout));
  }
  template <uint32_t Stride = kSize>
  static Strings NewArray(uint32_t count) {
    static_assert(sizeof(StringsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<StringsData*>(::operator new(sizeof(StringsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) StringsData();
    return Strings(memory::MakeHandle(p, sizeof(StringsData) * count, "Strings", &kStringsLayout, sizeof(StringsData)));
  }
  Strings at(uint32_t i) const { return Strings(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  StringsData* direct() const { return static_cast<StringsData*>(memory::HandleOwn(address, &kStringsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kStringsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kStringsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the 17 string files (+4i; assets 22638-22654, the ids at 0x82007DD0)
  uint32_t files_address() const { return address + 0; }
  uint8_t* files() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  // each file's loaded flag (+4i; StringsLoadedFlag)
  uint32_t loaded_flags_address() const { return address + 68; }
  uint8_t* loaded_flags() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 68 : guest::Pointer(address + 68); }
  // the mask of files asked for
  uint32_t files_asked_address() const { return address + 136; }
  uint32_t files_asked_bits() const { if (auto* d = direct()) return d->files_asked; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_files_asked_bits(uint32_t v) const { if (auto* d = direct()) { d->files_asked = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  uint32_t files_asked() const { if (auto* d = direct()) return d->files_asked; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_files_asked(uint32_t v) const { if (auto* d = direct()) { d->files_asked = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  // each file's count of loads (+4i)
  uint32_t load_counts_address() const { return address + 4236; }
  uint8_t* load_counts() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4236 : guest::Pointer(address + 4236); }
};

// file; measure: Strings::OnLoaded.r6
#pragma pack(push, 1)
struct StringsLoadedFlagData {
  uint32_t loaded = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(StringsLoadedFlagData) == 4);
inline constexpr memory::LayoutField kStringsLoadedFlagFields[] = {{0, 4}};
inline constexpr memory::Layout kStringsLoadedFlagLayout = {kStringsLoadedFlagFields, 1};

struct StringsLoadedFlag : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  StringsLoadedFlagData* data() const { return static_cast<StringsLoadedFlagData*>(memory::HandleObject(address)); }
  static StringsLoadedFlag New() {
    return StringsLoadedFlag(memory::MakeHandle(new StringsLoadedFlagData(), sizeof(StringsLoadedFlagData), "StringsLoadedFlag", &kStringsLoadedFlagLayout));
  }
  template <uint32_t Stride = kSize>
  static StringsLoadedFlag NewArray(uint32_t count) {
    static_assert(sizeof(StringsLoadedFlagData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<StringsLoadedFlagData*>(::operator new(sizeof(StringsLoadedFlagData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) StringsLoadedFlagData();
    return StringsLoadedFlag(memory::MakeHandle(p, sizeof(StringsLoadedFlagData) * count, "StringsLoadedFlag", &kStringsLoadedFlagLayout, sizeof(StringsLoadedFlagData)));
  }
  StringsLoadedFlag at(uint32_t i) const { return StringsLoadedFlag(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  StringsLoadedFlagData* direct() const { return static_cast<StringsLoadedFlagData*>(memory::HandleOwn(address, &kStringsLoadedFlagLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kStringsLoadedFlagLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kStringsLoadedFlagLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t loaded_address() const { return address + 0; }
  uint32_t loaded_bits() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_loaded_bits(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t loaded() const { if (auto* d = direct()) return d->loaded; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_loaded(uint32_t v) const { if (auto* d = direct()) { d->loaded = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
};

// allocated; measure: -
struct StringFile : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static StringFile New(uint32_t size) { return StringFile(memory::NewBlob(size, "StringFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// The image words docs/functions/strings-and-font.md's code uses.
struct StringsAndFontGlobals {
  template <typename T = Font>
  static T font() { return T(guest::LoadU32(0x826C5C00u)); }
  static void set_font(const View& v) { guest::StoreU32(0x826C5C00u, v.address); }
  static void set_font(uint32_t v) { guest::StoreU32(0x826C5C00u, v); }
  static constexpr uint32_t font_address() { return 0x826C5C00u; }
  template <typename T = Strings>
  static T strings() { return T(guest::LoadU32(0x826C5C34u)); }
  static void set_strings(const View& v) { guest::StoreU32(0x826C5C34u, v.address); }
  static void set_strings(uint32_t v) { guest::StoreU32(0x826C5C34u, v); }
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
  static uint32_t chunk() { return guest::LoadU32(0x826C5BFCu); }
  static void set_chunk(uint32_t v) { guest::StoreU32(0x826C5BFCu, v); }
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
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.5
  static float half() { return guest::LoadFloat(0x82001BE4u); }
  static void set_half(float v) { guest::StoreFloat(0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // 0.001
  static float thousandth() { return guest::LoadFloat(0x820029D8u); }
  static void set_thousandth(float v) { guest::StoreFloat(0x820029D8u, v); }
  static constexpr uint32_t thousandth_address() { return 0x820029D8u; }
  // 255.0
  static float full_alpha() { return guest::LoadFloat(0x82000C44u); }
  static void set_full_alpha(float v) { guest::StoreFloat(0x82000C44u, v); }
  static constexpr uint32_t full_alpha_address() { return 0x82000C44u; }
  // 1.0 (read as two words, lfd)
  static double one_double() { return std::bit_cast<double>(guest::LoadU64(0x82002250u)); }
  static void set_one_double(double v) { guest::StoreU64(0x82002250u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t one_double_address() { return 0x82002250u; }
  // 1/360 (read as two words, lfd)
  static double turn() { return std::bit_cast<double>(guest::LoadU64(0x82007C78u)); }
  static void set_turn(double v) { guest::StoreU64(0x82007C78u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t turn_address() { return 0x82007C78u; }
};

}  // namespace schema
