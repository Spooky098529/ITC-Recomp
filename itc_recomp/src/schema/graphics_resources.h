// The views of docs/functions/graphics-resources.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/graphics-resources.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: Resource, sub_822AD740.r3
struct ResourceHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 180;
  // the low 4 bits the type; bits 8-11 the lock count (256 a lock); 0x200000 write-combined or the other view's lock, 0x40000000 a texture's level, 0x00100000 a render target or depth surface, 0x80000000 holding EDRAM
  uint32_t common() const { return guest::LoadU32(address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  // the fence the destroy waits for (Dropped)
  uint32_t fence() const { return guest::LoadU32(address + 8); }
  void set_fence(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t fence_address() const { return address + 8; }
  // probably
  uint32_t read_fence() const { return guest::LoadU32(address + 12); }
  void set_read_fence(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t read_fence_address() const { return address + 12; }
  // the base data's locked range, 128-byte units, start << 16 | end, 0xFFFF0000 none
  uint32_t locked_base() const { return guest::LoadU32(address + 20); }
  void set_locked_base(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t locked_base_address() const { return address + 20; }
  // a texture's: the mip data's locked range
  uint32_t locked_mips() const { return guest::LoadU32(address + 24); }
  void set_locked_mips(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t locked_mips_address() const { return address + 24; }
  // a buffer's: its data (a vertex buffer's low 2 bits 3)
  uint32_t data() const { return guest::LoadU32(address + 24); }
  void set_data(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t data_address() const { return address + 24; }
  // a texture's level (a surface): its texture
  template <typename T = TextureHeader>
  T parent() const { return T(guest::LoadU32(address + 24)); }
  void set_parent(const View& v) const { guest::StoreU32(address + 24, v.address); }
  void set_parent(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t parent_address() const { return address + 24; }
  // a texture's: the low 6 bits the GPU format, bit 10 an array, the top 20 bits the base data
  uint32_t fetch_format() const { return guest::LoadU32(address + 32); }
  void set_fetch_format(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t fetch_format_address() const { return address + 32; }
  // a texture's: bits 9-10 the dimension, the top 20 bits the mip data; type 6's +48 its data
  uint32_t fetch_mips() const { return guest::LoadU32(address + 48); }
  void set_fetch_mips(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t fetch_mips_address() const { return address + 48; }
  // type 9's (not seen in play): physical memory XMemFree'd by sub_822AD9D8 when not 0
  uint32_t type9_data() const { return guest::LoadU32(address + 152); }
  void set_type9_data(uint32_t v) const { guest::StoreU32(address + 152, v); }
  uint32_t type9_data_address() const { return address + 152; }
  // type 9's, read by sub_822AD740 (not read)
  uint32_t unknown_156() const { return guest::LoadU32(address + 156); }
  void set_unknown_156(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t unknown_156_address() const { return address + 156; }
  // type 9's, read by sub_822AD978 (not read)
  uint32_t unknown_164() const { return guest::LoadU32(address + 164); }
  void set_unknown_164(uint32_t v) const { guest::StoreU32(address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  // type 9's, read by sub_822AD978 (not read)
  uint32_t unknown_176() const { return guest::LoadU32(address + 176); }
  void set_unknown_176(uint32_t v) const { guest::StoreU32(address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
};

// file; measure: Texture, Resource.+24, new<52>@Device::CreateTexture
struct TextureHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // the low 4 bits the type (3); bits 8-11 the lock count; 0x200000 write-combined
  uint32_t common() const { return guest::LoadU32(address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  uint32_t fence() const { return guest::LoadU32(address + 8); }
  void set_fence(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t fence_address() const { return address + 8; }
  // probably
  uint32_t read_fence() const { return guest::LoadU32(address + 12); }
  void set_read_fence(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t read_fence_address() const { return address + 12; }
  // the base data's locked range (128-byte units, 0xFFFF0000 none)
  uint32_t locked_base() const { return guest::LoadU32(address + 20); }
  void set_locked_base(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t locked_base_address() const { return address + 20; }
  // the mip data's locked range
  uint32_t locked_mips() const { return guest::LoadU32(address + 24); }
  void set_locked_mips(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t locked_mips_address() const { return address + 24; }
  // the fetch constant's first word: the top bit tiled, bits 22-30 the pitch (32-texel units)
  uint32_t fetch_tiling() const { return guest::LoadU32(address + 28); }
  void set_fetch_tiling(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t fetch_tiling_address() const { return address + 28; }
  // the low 6 bits the GPU format, bit 10 an array, the top 20 bits the base data's address
  uint32_t fetch_format() const { return guest::LoadU32(address + 32); }
  void set_fetch_format(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t fetch_format_address() const { return address + 32; }
  // 2D: width - 1 in bits 0-12, height - 1 in 13-25; 3D 11, 11, 10 bits; 1D 24 bits; an array's depth - 1 from bit 26
  uint32_t fetch_size() const { return guest::LoadU32(address + 36); }
  void set_fetch_size(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t fetch_size_address() const { return address + 36; }
  // the swizzle; the top bit probably a border, bit 30 adds 2 to the base size
  uint32_t fetch_swizzle() const { return guest::LoadU32(address + 40); }
  void set_fetch_swizzle(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t fetch_swizzle_address() const { return address + 40; }
  // bits 6-9 the last mip level (levels = that + 1)
  uint32_t fetch_mip_range() const { return guest::LoadU32(address + 44); }
  void set_fetch_mip_range(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t fetch_mip_range_address() const { return address + 44; }
  // bits 9-10 the dimension (0 1D, 1 2D, 2 3D, 3 cube), bit 11 packed mips, the top 20 bits the mip data's address
  uint32_t fetch_mips() const { return guest::LoadU32(address + 48); }
  void set_fetch_mips(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t fetch_mips_address() const { return address + 48; }
};

// allocated; measure: Surface, Font::OnPageLoaded.ret, new<48>@Device::CreateSurface, new<48>@Texture::GetSurfaceLevel
struct SurfaceHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // 4; 0x40000000 a texture's level, 0x00100000 a render target or depth surface, 0x80000000 holding EDRAM
  uint32_t common() const { return guest::LoadU32(address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  // 0xFFFF0000
  uint32_t locked_base() const { return guest::LoadU32(address + 20); }
  void set_locked_base(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t locked_base_address() const { return address + 20; }
  // a texture's level: its texture
  template <typename T = TextureHeader>
  T parent() const { return T(guest::LoadU32(address + 24)); }
  void set_parent(const View& v) const { guest::StoreU32(address + 24, v.address); }
  void set_parent(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t parent_address() const { return address + 24; }
  // a render target: the pitch in 80-pixel tiles (low 14 bits) and the samples (above)
  uint32_t pitch() const { return guest::LoadU32(address + 24); }
  void set_pitch(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t pitch_address() const { return address + 24; }
  // a render target: the multisample type in its low 2 bits
  uint16_t multisample() const { return guest::LoadU16(address + 24); }
  void set_multisample(uint16_t v) const { guest::StoreU16(address + 24, v); }
  uint32_t multisample_address() const { return address + 24; }
  // a texture's level: the level in bits 28-31, the face in 22-27
  uint32_t level_face() const { return guest::LoadU32(address + 28); }
  void set_level_face(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t level_face_address() const { return address + 28; }
  // a render target: the EDRAM base (low 12 bits) and format bits
  uint32_t edram() const { return guest::LoadU32(address + 28); }
  void set_edram(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t edram_address() const { return address + 28; }
  // bit 0 holding the hi-z
  uint32_t hi_z() const { return guest::LoadU32(address + 32); }
  void set_hi_z(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t hi_z_address() const { return address + 32; }
  // width - 1 at bit 18, height - 1 at bit 3
  uint32_t size() const { return guest::LoadU32(address + 36); }
  void set_size(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t size_address() const { return address + 36; }
  uint32_t format() const { return guest::LoadU32(address + 40); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t format_address() const { return address + 40; }
  // the EDRAM tiles x 5,120
  uint32_t edram_bytes() const { return guest::LoadU32(address + 44); }
  void set_edram_bytes(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t edram_bytes_address() const { return address + 44; }
};

// allocated; measure: VertexBuffer, IndexBuffer, new<32>@Device::CreateVertexBuffer, new<32>@Device::CreateIndexBuffer, Device.+12180
struct BufferHeader : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // 1 vertex, 2 index; an index buffer's format in the top 3 bits
  uint32_t common() const { return guest::LoadU32(address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  uint32_t ref_count() const { return guest::LoadU32(address + 4); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t ref_count_address() const { return address + 4; }
  // the data's locked range (0xFFFF0000 none)
  uint32_t locked_base() const { return guest::LoadU32(address + 20); }
  void set_locked_base(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t locked_base_address() const { return address + 20; }
  // the data's address (a vertex buffer's low 2 bits 3)
  uint32_t data() const { return guest::LoadU32(address + 24); }
  void set_data(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t data_address() const { return address + 24; }
  // the bytes (a vertex buffer's in bits 2-25, with bits 28 and 1 set)
  uint32_t size() const { return guest::LoadU32(address + 28); }
  void set_size(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t size_address() const { return address + 28; }
  // stored by Device::SetIndices on the bound index buffer (Device.+12180; single-commands' report); not read in detail
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: Texture::LevelDesc.r5, Surface::GetDesc.r4
struct LevelDesc : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  uint32_t format() const { return guest::LoadU32(address + 0); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t format_address() const { return address + 0; }
  // Resource::GetType's; 4 a surface
  uint32_t type() const { return guest::LoadU32(address + 4); }
  void set_type(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t type_address() const { return address + 4; }
  // probably; 0
  uint32_t usage() const { return guest::LoadU32(address + 8); }
  void set_usage(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t usage_address() const { return address + 8; }
  // probably; 0
  uint32_t pool() const { return guest::LoadU32(address + 12); }
  void set_pool(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t pool_address() const { return address + 12; }
  // the multisample type
  uint32_t multisample() const { return guest::LoadU32(address + 16); }
  void set_multisample(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t multisample_address() const { return address + 16; }
  // probably; 0
  uint32_t multisample_quality() const { return guest::LoadU32(address + 20); }
  void set_multisample_quality(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t multisample_quality_address() const { return address + 20; }
  uint32_t width() const { return guest::LoadU32(address + 24); }
  void set_width(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t width_address() const { return address + 24; }
  uint32_t height() const { return guest::LoadU32(address + 28); }
  void set_height(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t height_address() const { return address + 28; }
};

// allocated; measure: [82953E20], new<804>@Edram::Alloc
struct EdramAllocator : View {
  using View::View;
  static constexpr uint32_t kSize = 804;
  // the kernel's critical section in the game's (a host std::mutex in ours)
  uint32_t lock_address() const { return address + 0; }
  // 6,144 at the start
  uint32_t free_tiles() const { return guest::LoadU32(address + 28); }
  void set_free_tiles(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t free_tiles_address() const { return address + 28; }
  // 6,144 tiles, a set bit used, the most significant bit first
  uint32_t bitmap_address() const { return address + 32; }
  // 0x55555555: ends the bitmap's scans
  uint32_t sentinel() const { return guest::LoadU32(address + 800); }
  void set_sentinel(uint32_t v) const { guest::StoreU32(address + 800, v); }
  uint32_t sentinel_address() const { return address + 800; }
};

// embedded; measure: SurfaceHeader::Set.r7
struct SurfaceParameters : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t edram_base() const { return guest::LoadU32(address + 0); }
  void set_edram_base(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t edram_base_address() const { return address + 0; }
  uint32_t hi_z_base() const { return guest::LoadU32(address + 4); }
  void set_hi_z_base(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t hi_z_base_address() const { return address + 4; }
  // read, its use not worked out
  uint32_t unknown_8() const { return guest::LoadU32(address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: Texture::LockRect.r6
struct LockedRect : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t pitch() const { return guest::LoadU32(address + 0); }
  void set_pitch(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t pitch_address() const { return address + 0; }
  // the address
  uint32_t bits() const { return guest::LoadU32(address + 4); }
  void set_bits(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t bits_address() const { return address + 4; }
};

// embedded; measure: Texture::LockRect.r7
struct GpuLockRect : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t left() const { return guest::LoadU32(address + 0); }
  void set_left(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t left_address() const { return address + 0; }
  uint32_t top() const { return guest::LoadU32(address + 4); }
  void set_top(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t top_address() const { return address + 4; }
};

// embedded; measure: Device::GetStreamSource.r5, Device::GetStreamSource.r6, Edram::Alloc.r4, Edram::NextRun.r4, GpuFormat::BlockSize.r4, GpuFormat::BlockSize.r5, SurfaceHeader::Set.r9, SurfaceHeader::Set.r10, Texture::BaseSize.r4, Texture::BaseSize.r5, Texture::BaseSize.r6, Texture::LevelLayout.r5, Texture::LevelLayout.r6, Texture::LevelLayout.r7, Texture::LevelLayout.r8, Texture::LevelLayout.r9, Texture::LockLevel.r7, Texture::LockLevel.r8, Texture::LockLevel.r10, TextureLayout, TextureLayout::LevelSize.r4, TextureLayout::LevelSize.r5, TextureLayout::MipTailOffset.r10
struct GpuOutWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// The image words docs/functions/graphics-resources.md's code uses.
struct GraphicsResourcesGlobals {
  // the graphics device
  template <typename T = Device>
  static T device() { return T(guest::LoadU32(0x820007B0u)); }
  static void set_device(const View& v) { guest::StoreU32(0x820007B0u, v.address); }
  static void set_device(uint32_t v) { guest::StoreU32(0x820007B0u, v); }
  static constexpr uint32_t device_address() { return 0x820007B0u; }
  // 1 while a depth surface holds the hi-z
  static uint32_t hi_z_taken() { return guest::LoadU32(0x829537C4u); }
  static void set_hi_z_taken(uint32_t v) { guest::StoreU32(0x829537C4u, v); }
  static constexpr uint32_t hi_z_taken_address() { return 0x829537C4u; }
  // made on first use by Edram::Alloc
  template <typename T = EdramAllocator>
  static T edram_allocator() { return T(guest::LoadU32(0x82953E20u)); }
  static void set_edram_allocator(const View& v) { guest::StoreU32(0x82953E20u, v.address); }
  static void set_edram_allocator(uint32_t v) { guest::StoreU32(0x82953E20u, v); }
  static constexpr uint32_t edram_allocator_address() { return 0x82953E20u; }
  // the first of the GPU formats' halfwords (by format): bits 8-11 the format bits, the low byte the bits a pixel
  static uint16_t format_table() { return guest::LoadU16(0x82013468u); }
  static void set_format_table(uint16_t v) { guest::StoreU16(0x82013468u, v); }
  static constexpr uint32_t format_table_address() { return 0x82013468u; }
};

}  // namespace schema
