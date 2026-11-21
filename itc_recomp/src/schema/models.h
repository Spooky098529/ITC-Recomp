// The views of docs/functions/models.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/models.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Model, Mesh
struct Model : View {
  using View::View;
  static constexpr uint32_t kSize = 432;
  static constexpr uint32_t kVtable = 0x8206CD18;
  // 0x8206CD18 (a part model's 0x8200338C)
  template <typename T = ModelVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the part file's bytes before its vertex data, copied into a memory block (Memory::AllocVirtual)
  template <typename T = PartFile>
  T header_block() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_header_block(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_header_block(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t header_block_address() const { return address + 16; }
  // the vertex data in physical memory (Memory::AllocPhysicalEx)
  uint32_t vertex_data() const { return guest::LoadU32(base, address + 20); }
  void set_vertex_data(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t vertex_data_address() const { return address + 20; }
  // 16 bytes a pair (Memory::AllocVirtual)
  template <typename T = ModelShaderPair>
  T shader_pairs() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_shader_pairs(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_shader_pairs(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t shader_pairs_address() const { return address + 24; }
  // the header block again (+16): its +76 says whether the sections are read
  template <typename T = PartFile>
  T header() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_header(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_header(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t header_address() const { return address + 28; }
  // the part file's +0x14 made a pointer (not read here)
  uint32_t table_14() const { return guest::LoadU32(base, address + 32); }
  void set_table_14(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t table_14_address() const { return address + 32; }
  // the part file's +0x18 made a pointer (not read here)
  uint32_t table_18() const { return guest::LoadU32(base, address + 36); }
  void set_table_18(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t table_18_address() const { return address + 36; }
  // the part file's +0x1C made a pointer: 544 bytes a shape
  template <typename T = ModelShape>
  T shapes() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_shapes(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_shapes(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t shapes_address() const { return address + 40; }
  // the part file's +0x20 made a pointer (not read here)
  uint32_t table_20() const { return guest::LoadU32(base, address + 44); }
  void set_table_20(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t table_20_address() const { return address + 44; }
  // +80 once Model::Load has a texture set, else 0
  template <typename T = TextureProvider>
  T texture_provider() const { return T(base, guest::LoadU32(base, address + 48)); }
  void set_texture_provider(const View& v) const { guest::StoreU32(base, address + 48, v.address); }
  void set_texture_provider(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t texture_provider_address() const { return address + 48; }
  // its own texture set (an XPR)
  uint32_t xpr_address() const { return address + 56; }
  // its TextureProvider (64 slots); Model::Load reads its vtable here
  uint32_t textures_address() const { return address + 80; }
  // the skeleton (model-animation.toml's Skeleton), for the node file
  uint32_t skeleton_address() const { return address + 112; }
  // the TrackSet for the part file's +0x2C section (a float4 a track)
  uint32_t track_set_float4_address() const { return address + 304; }
  // the +304 set's values (its +16): the float4 table a material's +0xD0 indexes (vertex c9 + i)
  uint32_t float4_values() const { return guest::LoadU32(base, address + 320); }
  void set_float4_values(uint32_t v) const { guest::StoreU32(base, address + 320, v); }
  uint32_t float4_values_address() const { return address + 320; }
  // the TrackSet for the +0x30 section (a float a track)
  uint32_t track_set_float_a_address() const { return address + 324; }
  // the TrackSet for the +0x34 section (a float a track)
  uint32_t track_set_float_b_address() const { return address + 344; }
  // the TrackSet for the +0x38 section, the material animation (32 bytes a track)
  uint32_t track_set_material_address() const { return address + 364; }
  // the TrackSet for the +0x3C section (208 bytes a track)
  uint32_t track_set_208_address() const { return address + 384; }
  // the part file's +4
  uint32_t file_04() const { return guest::LoadU32(base, address + 404); }
  void set_file_04(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t file_04_address() const { return address + 404; }
  // the part file's +8
  uint32_t file_08() const { return guest::LoadU32(base, address + 408); }
  void set_file_08(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t file_08_address() const { return address + 408; }
  // the part file's +0xC
  uint32_t shape_count() const { return guest::LoadU32(base, address + 412); }
  void set_shape_count(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t shape_count_address() const { return address + 412; }
  // the part file's +0x10
  uint32_t file_10() const { return guest::LoadU32(base, address + 416); }
  void set_file_10(uint32_t v) const { guest::StoreU32(base, address + 416, v); }
  uint32_t file_10_address() const { return address + 416; }
  // the shader section's +0
  uint32_t shader_pair_count() const { return guest::LoadU32(base, address + 420); }
  void set_shader_pair_count(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t shader_pair_count_address() const { return address + 420; }
  // a part model's (PartModel): with +456, Model::RecordMaterials' textures from +456
  uint32_t use_override_texture() const { return guest::LoadU32(base, address + 432); }
  void set_use_override_texture(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t use_override_texture_address() const { return address + 432; }
  // a part model's: a texture index replaced by +460 (-1 at Clear)
  uint32_t replaced_texture_index() const { return guest::LoadU32(base, address + 436); }
  void set_replaced_texture_index(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t replaced_texture_index_address() const { return address + 436; }
  // a part model's: forces shared pair 1005
  uint32_t force_pair_1005() const { return guest::LoadU32(base, address + 440); }
  void set_force_pair_1005(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t force_pair_1005_address() const { return address + 440; }
  // a part model's: the vinyl sheet (SetVinylTextures' a)
  uint32_t vinyl_sheet() const { return guest::LoadU32(base, address + 444); }
  void set_vinyl_sheet(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t vinyl_sheet_address() const { return address + 444; }
  // a part model's: SetVinylTextures' b
  uint32_t vinyl_texture_b() const { return guest::LoadU32(base, address + 448); }
  void set_vinyl_texture_b(uint32_t v) const { guest::StoreU32(base, address + 448, v); }
  uint32_t vinyl_texture_b_address() const { return address + 448; }
  // a part model's: category 3's texture
  uint32_t category3_texture() const { return guest::LoadU32(base, address + 452); }
  void set_category3_texture(uint32_t v) const { guest::StoreU32(base, address + 452, v); }
  uint32_t category3_texture_address() const { return address + 452; }
  // a part model's: an override texture (with +432)
  uint32_t override_texture() const { return guest::LoadU32(base, address + 456); }
  void set_override_texture(uint32_t v) const { guest::StoreU32(base, address + 456, v); }
  uint32_t override_texture_address() const { return address + 456; }
  // a part model's: the texture that replaces +436's index
  uint32_t replacement_texture() const { return guest::LoadU32(base, address + 460); }
  void set_replacement_texture(uint32_t v) const { guest::StoreU32(base, address + 460, v); }
  uint32_t replacement_texture_address() const { return address + 460; }
};

// image; measure: Model.+0
struct ModelVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // slot 1: a per-draw hook (r6 the c13 color, r7 the blend index's address)
  uint32_t draw_hook() const { return guest::LoadU32(base, address + 4); }
  void set_draw_hook(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t draw_hook_address() const { return address + 4; }
  // the texture provider's slot 5 (through +48): a texture by XPR2 and index
  uint32_t provider_xpr_texture() const { return guest::LoadU32(base, address + 20); }
  void set_provider_xpr_texture(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t provider_xpr_texture_address() const { return address + 20; }
  // the texture provider's slot 6 (through +48): the environment texture
  uint32_t provider_environment_texture() const { return guest::LoadU32(base, address + 24); }
  void set_provider_environment_texture(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t provider_environment_texture_address() const { return address + 24; }
};

// allocated; measure: Model.+24, new<?>@Mesh::LoadShaders
struct ModelShaderPair : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the vertex shader object (Memory::AllocVirtual)
  uint32_t vertex_shader() const { return guest::LoadU32(base, address + 0); }
  void set_vertex_shader(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vertex_shader_address() const { return address + 0; }
  // the pixel shader object
  uint32_t pixel_shader() const { return guest::LoadU32(base, address + 4); }
  void set_pixel_shader(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t pixel_shader_address() const { return address + 4; }
  // physical memory (Memory::AllocPhysicalEx)
  uint32_t vertex_microcode() const { return guest::LoadU32(base, address + 8); }
  void set_vertex_microcode(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t vertex_microcode_address() const { return address + 8; }
  uint32_t pixel_microcode() const { return guest::LoadU32(base, address + 12); }
  void set_pixel_microcode(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t pixel_microcode_address() const { return address + 12; }
};

// file; measure: Mesh::Parse.r4, new<?>@Mesh::Parse
struct PartFile : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  // 0x00150000, else a debug print
  uint32_t version() const { return guest::LoadU32(base, address + 0); }
  void set_version(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t version_address() const { return address + 0; }
  // kept at model +404
  uint32_t file_04() const { return guest::LoadU32(base, address + 4); }
  void set_file_04(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t file_04_address() const { return address + 4; }
  // kept at model +408
  uint32_t file_08() const { return guest::LoadU32(base, address + 8); }
  void set_file_08(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t file_08_address() const { return address + 8; }
  // kept at model +412
  uint32_t shape_count() const { return guest::LoadU32(base, address + 12); }
  void set_shape_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t shape_count_address() const { return address + 12; }
  // kept at model +416
  uint32_t file_10() const { return guest::LoadU32(base, address + 16); }
  void set_file_10(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t file_10_address() const { return address + 16; }
  // an offset, made a pointer at model +32
  uint32_t table_14() const { return guest::LoadU32(base, address + 20); }
  void set_table_14(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t table_14_address() const { return address + 20; }
  // an offset, made a pointer at model +36
  uint32_t table_18() const { return guest::LoadU32(base, address + 24); }
  void set_table_18(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t table_18_address() const { return address + 24; }
  // the shapes' offset (544 bytes each), made a pointer at model +40
  uint32_t shapes() const { return guest::LoadU32(base, address + 28); }
  void set_shapes(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t shapes_address() const { return address + 28; }
  // an offset, made a pointer at model +44
  uint32_t table_20() const { return guest::LoadU32(base, address + 32); }
  void set_table_20(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t table_20_address() const { return address + 32; }
  // the node file's offset (Skeleton::Bind), 0 none
  uint32_t node_file() const { return guest::LoadU32(base, address + 36); }
  void set_node_file(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t node_file_address() const { return address + 36; }
  // the +0x28 section (Skeleton::LoadNodeValues), 0 none
  uint32_t node_values_section() const { return guest::LoadU32(base, address + 40); }
  void set_node_values_section(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t node_values_section_address() const { return address + 40; }
  // the +0x2C section, always read (TrackSet::SetupFloat4, model +304)
  uint32_t float4_section() const { return guest::LoadU32(base, address + 44); }
  void set_float4_section(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t float4_section_address() const { return address + 44; }
  // the +0x30 section (TrackSet::SetupFloat, model +324), 0 none
  uint32_t float_section_a() const { return guest::LoadU32(base, address + 48); }
  void set_float_section_a(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t float_section_a_address() const { return address + 48; }
  // the +0x34 section (TrackSet::SetupFloat, model +344), 0 none
  uint32_t float_section_b() const { return guest::LoadU32(base, address + 52); }
  void set_float_section_b(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t float_section_b_address() const { return address + 52; }
  // the +0x38 section, the material animation (TrackSet::Setup32, model +364), 0 none
  uint32_t material_section() const { return guest::LoadU32(base, address + 56); }
  void set_material_section(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t material_section_address() const { return address + 56; }
  // the +0x3C section (TrackSet::Setup208, model +384), 0 none
  uint32_t section_208() const { return guest::LoadU32(base, address + 60); }
  void set_section_208(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t section_208_address() const { return address + 60; }
  // the bytes before it are the header block
  uint32_t vertex_data_offset() const { return guest::LoadU32(base, address + 64); }
  void set_vertex_data_offset(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t vertex_data_offset_address() const { return address + 64; }
  uint32_t vertex_data_size() const { return guest::LoadU32(base, address + 68); }
  void set_vertex_data_size(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t vertex_data_size_address() const { return address + 68; }
  // the shader section's offset (a ShaderSection)
  uint32_t shader_section() const { return guest::LoadU32(base, address + 72); }
  void set_shader_section(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t shader_section_address() const { return address + 72; }
  // the node file and the +0x28, +0x30 to +0x3C sections are read only when it isn't 0 (read in the header block)
  uint32_t has_sections() const { return guest::LoadU32(base, address + 76); }
  void set_has_sections(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t has_sections_address() const { return address + 76; }
};

// file; measure: Mesh::LoadShaders.r4
struct ShaderSection : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t pair_count() const { return guest::LoadU32(base, address + 0); }
  void set_pair_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pair_count_address() const { return address + 0; }
  // the first pair's entry
  uint32_t first_entry_address() const { return address + 32; }
  // the first entry's +4
  uint32_t first_vertex_size() const { return guest::LoadU32(base, address + 36); }
  void set_first_vertex_size(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t first_vertex_size_address() const { return address + 36; }
  // the first entry's +8
  uint32_t first_pixel_size() const { return guest::LoadU32(base, address + 40); }
  void set_first_pixel_size(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t first_pixel_size_address() const { return address + 40; }
};

// file; measure: -
struct ShaderPairEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // the vertex shader file's size
  uint32_t vertex_size() const { return guest::LoadU32(base, address + 4); }
  void set_vertex_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vertex_size_address() const { return address + 4; }
  // the pixel shader file's size
  uint32_t pixel_size() const { return guest::LoadU32(base, address + 8); }
  void set_pixel_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pixel_size_address() const { return address + 8; }
};

// file; measure: Mesh.+40, WorkMem
struct ModelShape : View {
  using View::View;
  static constexpr uint32_t kSize = 544;
  // WorkMem::AllocVertexBuffer's store (a work memory block's, which the measure files here)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // a stream descriptor: +0 the vertex count, +4 the type, +8 the stride
  uint32_t stream0_address() const { return address + 240; }
  // a stream descriptor (9 or 10 here, or 6 in stream 2, binds two streams)
  uint32_t stream1_address() const { return address + 256; }
  // a stream descriptor
  uint32_t stream2_address() const { return address + 272; }
  // a vertex buffer header (Resource::Relocate'd to the vertex data)
  uint32_t vertex_buffer0_address() const { return address + 288; }
  // its fetch constant word: bits 28-29 set to 1 at load
  uint32_t vertex_buffer0_fetch() const { return guest::LoadU32(base, address + 316); }
  void set_vertex_buffer0_fetch(uint32_t v) const { guest::StoreU32(base, address + 316, v); }
  uint32_t vertex_buffer0_fetch_address() const { return address + 316; }
  // a vertex buffer header (relocated when not skinned)
  uint32_t vertex_buffer1_address() const { return address + 320; }
  // as vertex_buffer0_fetch
  uint32_t vertex_buffer1_fetch() const { return guest::LoadU32(base, address + 348); }
  void set_vertex_buffer1_fetch(uint32_t v) const { guest::StoreU32(base, address + 348, v); }
  uint32_t vertex_buffer1_fetch_address() const { return address + 348; }
  // a vertex buffer header (relocated when not skinned)
  uint32_t vertex_buffer2_address() const { return address + 352; }
  // as vertex_buffer0_fetch
  uint32_t vertex_buffer2_fetch() const { return guest::LoadU32(base, address + 380); }
  void set_vertex_buffer2_fetch(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t vertex_buffer2_fetch_address() const { return address + 380; }
  // the stream's file offset, moved to the vertex data at load
  uint32_t stream0_offset() const { return guest::LoadU32(base, address + 384); }
  void set_stream0_offset(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t stream0_offset_address() const { return address + 384; }
  // moved only when not skinned
  uint32_t stream1_offset() const { return guest::LoadU32(base, address + 388); }
  void set_stream1_offset(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t stream1_offset_address() const { return address + 388; }
  // moved only when not skinned
  uint32_t stream2_offset() const { return guest::LoadU32(base, address + 392); }
  void set_stream2_offset(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t stream2_offset_address() const { return address + 392; }
  // made a pointer into the header block at load
  template <typename T = MaterialBlock>
  T materials() const { return T(base, guest::LoadU32(base, address + 396)); }
  void set_materials(const View& v) const { guest::StoreU32(base, address + 396, v.address); }
  void set_materials(uint32_t v) const { guest::StoreU32(base, address + 396, v); }
  uint32_t materials_address() const { return address + 396; }
  // a skin array, made a pointer at load
  uint32_t skin_array_a() const { return guest::LoadU32(base, address + 400); }
  void set_skin_array_a(uint32_t v) const { guest::StoreU32(base, address + 400, v); }
  uint32_t skin_array_a_address() const { return address + 400; }
  // a skin array, made a pointer at load
  uint32_t skin_array_b() const { return guest::LoadU32(base, address + 404); }
  void set_skin_array_b(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t skin_array_b_address() const { return address + 404; }
  // made a pointer at load
  template <typename T = SkinBlock>
  T skin() const { return T(base, guest::LoadU32(base, address + 408)); }
  void set_skin(const View& v) const { guest::StoreU32(base, address + 408, v.address); }
  void set_skin(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t skin_address() const { return address + 408; }
  // a skinned shape's stream 1 (probably set at run time)
  uint32_t skinned_stream1() const { return guest::LoadU32(base, address + 412); }
  void set_skinned_stream1(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t skinned_stream1_address() const { return address + 412; }
  // a skinned shape's stream 2 (probably set at run time)
  uint32_t skinned_stream2() const { return guest::LoadU32(base, address + 420); }
  void set_skinned_stream2(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t skinned_stream2_address() const { return address + 420; }
  // 1 when skinned (only stream 0 in the vertex data; weights normalized at load)
  uint32_t skinned() const { return guest::LoadU32(base, address + 428); }
  void set_skinned(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t skinned_address() const { return address + 428; }
  // sampler state 28 of samplers 0 to 3
  uint32_t sampler_state_28_address() const { return address + 432; }
};

// file; measure: Mesh.+40.+396
struct MaterialBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 528;
  uint32_t record_count() const { return guest::LoadU32(base, address + 0); }
  void set_record_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t record_count_address() const { return address + 0; }
  uint32_t group0_count() const { return guest::LoadU32(base, address + 8); }
  void set_group0_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t group0_count_address() const { return address + 8; }
  // probably the blended draws
  uint32_t group1_count() const { return guest::LoadU32(base, address + 12); }
  void set_group1_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t group1_count_address() const { return address + 12; }
  // the first record (each one's +384, +0x180, made a pointer into the header block at load)
  uint32_t records_address() const { return address + 16; }
};

// file; measure: Mesh.+40.+408, WorkMem.+408
struct SkinBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t vertex_count() const { return guest::LoadU32(base, address + 0); }
  void set_vertex_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vertex_count_address() const { return address + 0; }
  // a u32 a vertex; made a pointer at load
  template <typename T = SkinInfluenceCount>
  T influence_counts() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_influence_counts(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_influence_counts(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t influence_counts_address() const { return address + 8; }
  // 16 bytes an influence; made a pointer at load
  template <typename T = SkinWeight>
  T weights() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_weights(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_weights(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t weights_address() const { return address + 12; }
  // made a pointer at load
  uint32_t bone_indices() const { return guest::LoadU32(base, address + 16); }
  void set_bone_indices(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t bone_indices_address() const { return address + 16; }
  // an object recorded as command 22
  uint32_t object() const { return guest::LoadU32(base, address + 20); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t object_address() const { return address + 20; }
};

// file; measure: WorkMem.+408.+8
struct SkinInfluenceCount : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
};

// file; measure: WorkMem.+408.+12
struct SkinWeight : View {
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
  // the next weight (Model::Load walks them)
  uint32_t next_address() const { return address + 16; }
};

// embedded; measure: TextureProvider, sub_82594440.r3
struct TextureProvider : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr uint32_t kVtable = 0x8206CCB4;
  // 0x8206CCB4
  template <typename T = TextureProviderVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the std::vector's allocator (empty, not read); the vector's functions' first
  uint32_t vector() const { return guest::LoadU32(base, address + 4); }
  void set_vector(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vector_address() const { return address + 4; }
  // the first slot (0: no storage); the vector's functions' last
  template <typename T = TextureSlot>
  T first() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t first_address() const { return address + 8; }
  // the end of the slots; the vector's functions' end of storage
  template <typename T = TextureSlot>
  T last() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_last(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t last_address() const { return address + 12; }
  // the end of the storage
  uint32_t end() const { return guest::LoadU32(base, address + 16); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t end_address() const { return address + 16; }
};

// embedded; measure: -
struct TexturePointerVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // empty, not read
  uint32_t allocator() const { return guest::LoadU32(base, address + 0); }
  void set_allocator(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t allocator_address() const { return address + 0; }
  // the first slot (0: no storage)
  template <typename T = TextureSlot>
  T first() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  // the end of the slots
  template <typename T = TextureSlot>
  T last() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_last(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  // the end of the storage
  uint32_t end() const { return guest::LoadU32(base, address + 12); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// allocated; measure: new<?>@TextureProvider::Allocate, TextureProvider::Assign.r5, TextureProvider::At.ret, TextureProvider::InsertN.r4, TextureProvider::InsertN.r6
struct TextureSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t texture() const { return guest::LoadU32(base, address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
};

// image; measure: TextureProvider.+0, Model.+80, SceneAssets.+144, SceneAssets.+164, SceneAssets.+232, SceneAssets.+300, SceneAssets.+368, SceneAssets.+1000, SceneAssets.+1068, SceneAssets.+4800, CarScene::OnFileLoaded.r6.+296, CarScene::OnFileLoaded.r6.+364, CarScene::OnFileLoaded.r6.+432, CarScene::OnFileLoaded.r6.+1064, CarScene::OnFileLoaded.r6.+1132, CarModelLoads.+320, Cockpit.+496, WheelsLoads.+2048, TrafficCar.+4064, TrafficCar::OnFileLoaded.r6.+4064
struct TextureProviderVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // slot 2 (0x82594328, scenery-lists.md)
  uint32_t clear_all() const { return guest::LoadU32(base, address + 8); }
  void set_clear_all(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t clear_all_address() const { return address + 8; }
  // slot 3, TextureProvider::Get (0x825946B8)
  uint32_t get() const { return guest::LoadU32(base, address + 12); }
  void set_get(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t get_address() const { return address + 12; }
  // slot 4, TextureProvider::Set (0x82594718)
  uint32_t set() const { return guest::LoadU32(base, address + 16); }
  void set_set(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t set_address() const { return address + 16; }
  // slots 5 to 7, TextureProvider::GetXprTexture (0x825941D0)
  uint32_t xpr_texture_5() const { return guest::LoadU32(base, address + 20); }
  void set_xpr_texture_5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t xpr_texture_5_address() const { return address + 20; }
  // the environment texture
  uint32_t xpr_texture_6() const { return guest::LoadU32(base, address + 24); }
  void set_xpr_texture_6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t xpr_texture_6_address() const { return address + 24; }
  uint32_t xpr_texture_7() const { return guest::LoadU32(base, address + 28); }
  void set_xpr_texture_7(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t xpr_texture_7_address() const { return address + 28; }
  // slots 8 to 10, TextureProvider::FindXprTexture (0x82594230)
  uint32_t find_xpr_texture_8() const { return guest::LoadU32(base, address + 32); }
  void set_find_xpr_texture_8(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t find_xpr_texture_8_address() const { return address + 32; }
  uint32_t find_xpr_texture_9() const { return guest::LoadU32(base, address + 36); }
  void set_find_xpr_texture_9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t find_xpr_texture_9_address() const { return address + 36; }
  uint32_t find_xpr_texture_10() const { return guest::LoadU32(base, address + 40); }
  void set_find_xpr_texture_10(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t find_xpr_texture_10_address() const { return address + 40; }
};

// embedded; measure: XPR
struct XPR : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // the header block (Memory::AllocVirtual, freed with Memory::FreeLater): +0 the record count, the records from +4
  uint32_t header_block() const { return guest::LoadU32(base, address + 0); }
  void set_header_block(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t header_block_address() const { return address + 0; }
  uint32_t header_size() const { return guest::LoadU32(base, address + 4); }
  void set_header_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t header_size_address() const { return address + 4; }
  // the data block (Memory::AllocPhysical 0xBC7F0000, freed with FreePhysicalLater 0x807F0000)
  uint32_t data_block() const { return guest::LoadU32(base, address + 8); }
  void set_data_block(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_block_address() const { return address + 8; }
  uint32_t data_size() const { return guest::LoadU32(base, address + 12); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t data_size_address() const { return address + 12; }
  // the header block + 4
  template <typename T = XprRecord>
  T records() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_records(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_records(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t records_address() const { return address + 16; }
  // the header block's +0
  uint32_t record_count() const { return guest::LoadU32(base, address + 20); }
  void set_record_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t record_count_address() const { return address + 20; }
  // measure fault: CourseSet::InitParts' store (course-scenery.md)
  uint32_t fault_3724() const { return guest::LoadU32(base, address + 3724); }
  void set_fault_3724(uint32_t v) const { guest::StoreU32(base, address + 3724, v); }
  uint32_t fault_3724_address() const { return address + 3724; }
  // measure fault: as fault_3724
  uint32_t fault_3728() const { return guest::LoadU32(base, address + 3728); }
  void set_fault_3728(uint32_t v) const { guest::StoreU32(base, address + 3728, v); }
  uint32_t fault_3728_address() const { return address + 3728; }
  // measure fault: as fault_3724
  uint32_t fault_3732() const { return guest::LoadU32(base, address + 3732); }
  void set_fault_3732(uint32_t v) const { guest::StoreU32(base, address + 3732, v); }
  uint32_t fault_3732_address() const { return address + 3732; }
  // measure fault: as fault_3724
  uint32_t fault_3736() const { return guest::LoadU32(base, address + 3736); }
  void set_fault_3736(uint32_t v) const { guest::StoreU32(base, address + 3736, v); }
  uint32_t fault_3736_address() const { return address + 3736; }
};

// allocated; measure: new<?>@XPR::Create
struct XprRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // `TX..` for a texture; the header block's +0: the record count
  uint32_t type() const { return guest::LoadU32(base, address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  // the resource header's offset in the header block
  uint32_t header_offset() const { return guest::LoadU32(base, address + 4); }
  void set_header_offset(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t header_offset_address() const { return address + 4; }
  // the name's offset in the header block, made a pointer at load
  uint32_t name() const { return guest::LoadU32(base, address + 12); }
  void set_name(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t name_address() const { return address + 12; }
};

// file; measure: XPR::Create.r4
struct Xpr2File : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // `XPR2` (0x58505232)
  uint32_t magic() const { return guest::LoadU32(base, address + 0); }
  void set_magic(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t magic_address() const { return address + 0; }
  uint32_t header_size() const { return guest::LoadU32(base, address + 4); }
  void set_header_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t header_size_address() const { return address + 4; }
  uint32_t data_size() const { return guest::LoadU32(base, address + 8); }
  void set_data_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t data_size_address() const { return address + 8; }
};

// allocated; measure: Memory
struct MemoryBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  // the header's +0: `XLMM` (0x584C4D4D)
  uint32_t tag() const { return guest::LoadU32(base, address + -4096); }
  void set_tag(uint32_t v) const { guest::StoreU32(base, address + -4096, v); }
  uint32_t tag_address() const { return address + -4096; }
  // the header's +4
  uint32_t kind() const { return guest::LoadU32(base, address + -4092); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + -4092, v); }
  uint32_t kind_address() const { return address + -4092; }
  // the header's +8
  uint32_t passes() const { return guest::LoadU32(base, address + -4088); }
  void set_passes(uint32_t v) const { guest::StoreU32(base, address + -4088, v); }
  uint32_t passes_address() const { return address + -4088; }
  // the header's +12 (Memory::FreePhysicalLater sets it)
  uint32_t attributes() const { return guest::LoadU32(base, address + -4084); }
  void set_attributes(uint32_t v) const { guest::StoreU32(base, address + -4084, v); }
  uint32_t attributes_address() const { return address + -4084; }
};

// allocated; measure: new<?>@sub_8258E138, new<?>@Memory::AllocPhysicalEx, [8296BD58].+0.+8
struct MemoryBlockHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 4096;
  // `XLMM` (0x584C4D4D), checked by Memory::FreeLater, cleared when freed
  uint32_t tag() const { return guest::LoadU32(base, address + 0); }
  void set_tag(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t tag_address() const { return address + 0; }
  // 0 VirtualAlloc, 1 XPhysicalAllocEx, 2 XMemAlloc (PendingFrees::FreeDue frees by it)
  uint32_t kind() const { return guest::LoadU32(base, address + 4); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t kind_address() const { return address + 4; }
  // the pass starts PendingFrees::FreeDue waits once it's put off (1 when made)
  uint32_t passes() const { return guest::LoadU32(base, address + 8); }
  void set_passes(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t passes_address() const { return address + 8; }
  // XMemFree's attributes (0 when made)
  uint32_t attributes() const { return guest::LoadU32(base, address + 12); }
  void set_attributes(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t attributes_address() const { return address + 12; }
};

// image; measure: PendingFrees
struct PendingFreesList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the head node
  template <typename T = PendingFreesNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
};

// allocated; measure: new<12>@PendingFrees::Insert, PendingFrees::Insert.r4, [8296BD58], [8296BD58].+0, [8296BD58].+0.+4
struct PendingFreesNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = PendingFreesNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = PendingFreesNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  template <typename T = MemoryBlockHeader>
  T header() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_header(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_header(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t header_address() const { return address + 8; }
};

// file; measure: PendingFrees::Insert.r5
struct PendingFreesValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = MemoryBlockHeader>
  T header() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_header(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_header(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t header_address() const { return address + 0; }
};

// allocated; measure: new<12>@Shaders::LoadPair, Shaders::OnPairLoaded.r6
struct ShaderPairLoad : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // in the shared load buffer
  uint32_t vertex_file() const { return guest::LoadU32(base, address + 0); }
  void set_vertex_file(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vertex_file_address() const { return address + 0; }
  // right after the vertex file
  uint32_t pixel_file() const { return guest::LoadU32(base, address + 4); }
  void set_pixel_file(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t pixel_file_address() const { return address + 4; }
  // the pair index (0-33, 1000-1005, 2000-2055)
  uint32_t pair() const { return guest::LoadU32(base, address + 8); }
  void set_pair(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t pair_address() const { return address + 8; }
};

// file; measure: sub_822A8568.r3, sub_822A9EA8.r5.+0, sub_822A9FD8.r5.+0
struct ShaderFile : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // low 6 bits set: a vertex shader
  uint32_t magic() const { return guest::LoadU32(base, address + 0); }
  void set_magic(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t magic_address() const { return address + 0; }
  // the bytes before the microcode (copied after the object's own)
  uint32_t header_size() const { return guest::LoadU32(base, address + 4); }
  void set_header_size(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t header_size_address() const { return address + 4; }
  uint32_t microcode_size() const { return guest::LoadU32(base, address + 8); }
  void set_microcode_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t microcode_size_address() const { return address + 8; }
};

// file; measure: sub_822A8568.r4, sub_822A9EA8.r5, sub_822A9FD8.r5
struct ShaderFileRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  template <typename T = ShaderFile>
  T file() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_file(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t file_address() const { return address + 0; }
  // 592 (vertex) or 56 (pixel) + the header size
  uint32_t object_bytes() const { return guest::LoadU32(base, address + 4); }
  void set_object_bytes(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_bytes_address() const { return address + 4; }
  // the file + its header size
  uint32_t microcode() const { return guest::LoadU32(base, address + 8); }
  void set_microcode(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t microcode_address() const { return address + 8; }
  uint32_t microcode_bytes() const { return guest::LoadU32(base, address + 12); }
  void set_microcode_bytes(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t microcode_bytes_address() const { return address + 12; }
};

// file; measure: Skeleton.+80, sub_8259A9E0.r3.+80, sub_82599A20.r3.+80, sub_825A21A8.ret.+0
struct NodeFile : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  uint32_t node_count() const { return guest::LoadU32(base, address + 4); }
  void set_node_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_count_address() const { return address + 4; }
  uint32_t bone_count() const { return guest::LoadU32(base, address + 8); }
  void set_bone_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t bone_count_address() const { return address + 8; }
  // the first bone's node
  uint32_t first_bone() const { return guest::LoadU32(base, address + 12); }
  void set_first_bone(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t first_bone_address() const { return address + 12; }
  // the view's +4 (skeleton +84)
  uint32_t node_records_offset() const { return guest::LoadU32(base, address + 16); }
  void set_node_records_offset(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t node_records_offset_address() const { return address + 16; }
  // the view's +8 (skeleton +88)
  uint32_t rest_translations_offset() const { return guest::LoadU32(base, address + 20); }
  void set_rest_translations_offset(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t rest_translations_offset_address() const { return address + 20; }
  // the view's +12 (skeleton +92)
  uint32_t rest_angles_offset() const { return guest::LoadU32(base, address + 24); }
  void set_rest_angles_offset(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t rest_angles_offset_address() const { return address + 24; }
  // the view's +16 (skeleton +96)
  uint32_t rest_scales_offset() const { return guest::LoadU32(base, address + 28); }
  void set_rest_scales_offset(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t rest_scales_offset_address() const { return address + 28; }
  // an offset: the view's +20 (skeleton +100)
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  // the view's +24 (skeleton +104): 64 bytes a node
  uint32_t matrices_offset() const { return guest::LoadU32(base, address + 36); }
  void set_matrices_offset(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t matrices_offset_address() const { return address + 36; }
  // an offset: the view's +28 (skeleton +108)
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // the view's +32, and +36 = it + 16
  uint32_t tail_offset() const { return guest::LoadU32(base, address + 44); }
  void set_tail_offset(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t tail_offset_address() const { return address + 44; }
  // an offset: the view's +40
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
};

// embedded; measure: sub_825A21A8.r3
struct NodeFileView : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  template <typename T = NodeFile>
  T file() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_file(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t file_address() const { return address + 0; }
  template <typename T = SkeletonNodeRecord>
  T node_records() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_node_records(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_node_records(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_records_address() const { return address + 4; }
  // 16 bytes a node
  uint32_t rest_translations() const { return guest::LoadU32(base, address + 8); }
  void set_rest_translations(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t rest_translations_address() const { return address + 8; }
  // 16 bytes a node
  uint32_t rest_angles() const { return guest::LoadU32(base, address + 12); }
  void set_rest_angles(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t rest_angles_address() const { return address + 12; }
  // 16 bytes a node
  uint32_t rest_scales() const { return guest::LoadU32(base, address + 16); }
  void set_rest_scales(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t rest_scales_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // the file's 64-byte matrices
  uint32_t matrices() const { return guest::LoadU32(base, address + 24); }
  void set_matrices(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t matrices_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t tail() const { return guest::LoadU32(base, address + 32); }
  void set_tail(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t tail_address() const { return address + 32; }
  // the tail + 16
  uint32_t tail_16() const { return guest::LoadU32(base, address + 36); }
  void set_tail_16(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t tail_16_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // the node count minus the bone count
  uint32_t non_bone_count() const { return guest::LoadU32(base, address + 44); }
  void set_non_bone_count(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t non_bone_count_address() const { return address + 44; }
};

// file; measure: Skeleton.+84
struct SkeletonNodeRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  // the node whose matrix's translation row is copied in (UpdateNodeMatrices)
  uint32_t translation_node() const { return guest::LoadU32(base, address + 92); }
  void set_translation_node(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t translation_node_address() const { return address + 92; }
  // -1 none
  int32_t parent() const { return int32_t(guest::LoadU32(base, address + 128)); }
  void set_parent(int32_t v) const { guest::StoreU32(base, address + 128, uint32_t(v)); }
  uint32_t parent_address() const { return address + 128; }
  // 4 or below (signed) a node; 8 and 16 others; flags 0x40 (ResetPose takes +64) and 0x80 (a bone)
  uint32_t type() const { return guest::LoadU32(base, address + 132); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t type_address() const { return address + 132; }
};

// allocated; measure: Skeleton.+136, Skeleton.+140
struct SkeletonNodeState : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // also read as a word (CursorCurve::Sample's cursor)
  float translation_x() const { return guest::LoadFloat(base, address + 0); }
  void set_translation_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t translation_x_address() const { return address + 0; }
  float translation_y() const { return guest::LoadFloat(base, address + 4); }
  void set_translation_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t translation_y_address() const { return address + 4; }
  float translation_z() const { return guest::LoadFloat(base, address + 8); }
  void set_translation_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t translation_z_address() const { return address + 8; }
  // wrapped into [-pi, pi] by LocalMatrices
  float angle_x() const { return guest::LoadFloat(base, address + 16); }
  void set_angle_x(float v) const { guest::StoreFloat(base, address + 16, v); }
  uint32_t angle_x_address() const { return address + 16; }
  float angle_y() const { return guest::LoadFloat(base, address + 20); }
  void set_angle_y(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t angle_y_address() const { return address + 20; }
  float angle_z() const { return guest::LoadFloat(base, address + 24); }
  void set_angle_z(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t angle_z_address() const { return address + 24; }
  float scale_x() const { return guest::LoadFloat(base, address + 32); }
  void set_scale_x(float v) const { guest::StoreFloat(base, address + 32, v); }
  uint32_t scale_x_address() const { return address + 32; }
  float scale_y() const { return guest::LoadFloat(base, address + 36); }
  void set_scale_y(float v) const { guest::StoreFloat(base, address + 36, v); }
  uint32_t scale_y_address() const { return address + 36; }
  float scale_z() const { return guest::LoadFloat(base, address + 40); }
  void set_scale_z(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t scale_z_address() const { return address + 40; }
};

// allocated; measure: Skeleton.+148, Cockpit::OnFileLoaded.r6.+3472
struct SkeletonMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float m00() const { return guest::LoadFloat(base, address + 0); }
  void set_m00(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t m00_address() const { return address + 0; }
  uint64_t row0_xy() const { return guest::LoadU64(base, address + 0); }
  void set_row0_xy(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t row0_xy_address() const { return address + 0; }
  float m11() const { return guest::LoadFloat(base, address + 20); }
  void set_m11(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t m11_address() const { return address + 20; }
  float m22() const { return guest::LoadFloat(base, address + 40); }
  void set_m22(float v) const { guest::StoreFloat(base, address + 40, v); }
  uint32_t m22_address() const { return address + 40; }
  // the translation row
  float m30() const { return guest::LoadFloat(base, address + 48); }
  void set_m30(float v) const { guest::StoreFloat(base, address + 48, v); }
  uint32_t m30_address() const { return address + 48; }
  uint64_t row3_xy() const { return guest::LoadU64(base, address + 48); }
  void set_row3_xy(uint64_t v) const { guest::StoreU64(base, address + 48, v); }
  uint32_t row3_xy_address() const { return address + 48; }
  float m31() const { return guest::LoadFloat(base, address + 52); }
  void set_m31(float v) const { guest::StoreFloat(base, address + 52, v); }
  uint32_t m31_address() const { return address + 52; }
  float m32() const { return guest::LoadFloat(base, address + 56); }
  void set_m32(float v) const { guest::StoreFloat(base, address + 56, v); }
  uint32_t m32_address() const { return address + 56; }
  uint64_t row3_zw() const { return guest::LoadU64(base, address + 56); }
  void set_row3_zw(uint64_t v) const { guest::StoreU64(base, address + 56, v); }
  uint32_t row3_zw_address() const { return address + 56; }
};

// allocated; measure: new<4294967295>@sub_8259A9E0#2, new<4294967295>@sub_8259A9E0#3, new<4294967295>@sub_8259A9E0#4, sub_8259A9E0.r3.+144
struct SkeletonCountedArray : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // the first entry's +12: 1.0
  float first_one_12() const { return guest::LoadFloat(base, address + 28); }
  void set_first_one_12(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t first_one_12_address() const { return address + 28; }
  // the first entry's +28: 1.0
  float first_one_28() const { return guest::LoadFloat(base, address + 44); }
  void set_first_one_28(float v) const { guest::StoreFloat(base, address + 44, v); }
  uint32_t first_one_28_address() const { return address + 44; }
  // the bone matrices' (skeleton +144): 1
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // the first entry's +44: 1.0
  float first_one_44() const { return guest::LoadFloat(base, address + 60); }
  void set_first_one_44(float v) const { guest::StoreFloat(base, address + 60, v); }
  uint32_t first_one_44_address() const { return address + 60; }
  // the first entry's +60: 1.0
  float first_one_60() const { return guest::LoadFloat(base, address + 76); }
  void set_first_one_60(float v) const { guest::StoreFloat(base, address + 76, v); }
  uint32_t first_one_60_address() const { return address + 76; }
  // a node state's word (lifted, not read)
  uint32_t unknown_80() const { return guest::LoadU32(base, address + 80); }
  void set_unknown_80(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t unknown_80_address() const { return address + 80; }
  // a node state's word (lifted, not read)
  uint32_t unknown_148() const { return guest::LoadU32(base, address + 148); }
  void set_unknown_148(uint32_t v) const { guest::StoreU32(base, address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
};

// allocated; measure: new<4294967295>@sub_82599A20
struct SkeletonNodeValue : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 0.0 at bind
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  // 0.0 at bind
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  // 0.0 at bind; also the node's animation time
  float z() const { return guest::LoadFloat(base, address + 8); }
  void set_z(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t z_address() const { return address + 8; }
};

// file; measure: sub_820D8D20.r6
struct PartModelPicker : View {
  using View::View;
  static constexpr uint32_t kSize = 2288;
  // compared with r3 (not read)
  uint32_t unknown_196() const { return guest::LoadU32(base, address + 196); }
  void set_unknown_196(uint32_t v) const { guest::StoreU32(base, address + 196, v); }
  uint32_t unknown_196_address() const { return address + 196; }
  // compared with r3 (not read)
  uint32_t unknown_1172() const { return guest::LoadU32(base, address + 1172); }
  void set_unknown_1172(uint32_t v) const { guest::StoreU32(base, address + 1172, v); }
  uint32_t unknown_1172_address() const { return address + 1172; }
  // compared with r3 (not read)
  uint32_t unknown_2148() const { return guest::LoadU32(base, address + 2148); }
  void set_unknown_2148(uint32_t v) const { guest::StoreU32(base, address + 2148, v); }
  uint32_t unknown_2148_address() const { return address + 2148; }
  // compared with r3 (not read)
  uint32_t unknown_2284() const { return guest::LoadU32(base, address + 2284); }
  void set_unknown_2284(uint32_t v) const { guest::StoreU32(base, address + 2284, v); }
  uint32_t unknown_2284_address() const { return address + 2284; }
};

// The image words docs/functions/models.md's code uses.
struct ModelsGlobals {
  // the archive's shared load buffer (Shaders::LoadPair places the shader files in it)
  static uint32_t load_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826A5134u); }
  static void set_load_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A5134u, v); }
  static constexpr uint32_t load_buffer_address() { return 0x826A5134u; }
  // the bytes Shaders::LoadAll's pairs have placed in the load buffer
  static uint32_t shader_cursor(uint8_t* base) { return guest::LoadU32(base, 0x826C1CD8u); }
  static void set_shader_cursor(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C1CD8u, v); }
  static constexpr uint32_t shader_cursor_address() { return 0x826C1CD8u; }
  // the vertex shaders by pair index (2,056 pointers)
  static constexpr uint32_t shared_vertex_table_address() { return 0x826BFCB8u; }
  // the pixel shaders by pair index (2,056 pointers)
  static constexpr uint32_t shared_pixel_table_address() { return 0x826BDC98u; }
  // the graphics library's pointer to the vertex table (SetSharedShaders)
  static uint32_t shared_vertex_shaders(uint8_t* base) { return guest::LoadU32(base, 0x8296B7B4u); }
  static void set_shared_vertex_shaders(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B7B4u, v); }
  static constexpr uint32_t shared_vertex_shaders_address() { return 0x8296B7B4u; }
  // the graphics library's pointer to the pixel table
  static uint32_t shared_pixel_shaders(uint8_t* base) { return guest::LoadU32(base, 0x8296B7B8u); }
  static void set_shared_pixel_shaders(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B7B8u, v); }
  static constexpr uint32_t shared_pixel_shaders_address() { return 0x8296B7B8u; }
  // VertexDeclaration's word table (by stream type; a second from +72)
  static uint32_t stream_kinds(uint8_t* base) { return guest::LoadU32(base, 0x8265EC00u); }
  static void set_stream_kinds(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8265EC00u, v); }
  static constexpr uint32_t stream_kinds_address() { return 0x8265EC00u; }
  // VertexDeclaration's table of declarations
  static uint32_t vertex_declarations(uint8_t* base) { return guest::LoadU32(base, 0x8296B7F4u); }
  static void set_vertex_declarations(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B7F4u, v); }
  static constexpr uint32_t vertex_declarations_address() { return 0x8296B7F4u; }
  // the critical section over the memory blocks and the pending frees (a host mutex in its place)
  static constexpr uint32_t memory_blocks_lock_address() { return 0x8296BD38u; }
  // the pending frees (a PendingFreesList)
  static constexpr uint32_t pending_frees_address() { return 0x8296BD54u; }
  // Model::Load's weight quantization: the factor
  static float weight_multiply(uint8_t* base) { return guest::LoadFloat(base, 0x82001BDCu); }
  static void set_weight_multiply(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BDCu, v); }
  static constexpr uint32_t weight_multiply_address() { return 0x82001BDCu; }
  // Model::Load's weight quantization: the addend (0.5)
  static float weight_add(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_weight_add(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t weight_add_address() { return 0x82001BE4u; }
  // Model::Load's weight quantization: the scale back
  static float weight_scale(uint8_t* base) { return guest::LoadFloat(base, 0x820029D8u); }
  static void set_weight_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D8u, v); }
  static constexpr uint32_t weight_scale_address() { return 0x820029D8u; }
};

}  // namespace schema
