// The views of docs/functions/texture-compression.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/texture-compression.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ImageCodec, A16B16G16R16Codec, A16B16G16R16FCodec, A1R5G5B5Codec, A2B10G10R10Codec, A2R10G10B10Codec, A2W10V10U10Codec, A32B32G32R32FCodec, A4R4G4B4Codec, A8B8G8R8Codec, A8Codec, A8L8Codec, A8R8G8B8Codec, G16R16Codec, G16R16FCodec, G32R32FCodec, L16Codec, L6V5U5Codec, L8Codec, Q16W16V16U16Codec, Q8W8V8U8Codec, R16FCodec, R32FCodec, R5G6B5Codec, V16U16Codec, V8U8Codec, X1R5G5B5Codec, X4R4G4B4Codec, X8B8G8R8Codec, X8L8V8U8Codec, X8R8G8B8Codec
#pragma pack(push, 1)
struct ImageCodecHostData {
  uint32_t vtable = 0;  // +0
  uint32_t format = 0;  // +4
  uint32_t kind = 0;  // +8
  uint32_t has_bits = 0;  // +12
  uint32_t srgb = 0;  // +16
  uint32_t srgb_both = 0;  // +20
  uint32_t color_key = 0;  // +24
  uint32_t palette_set = 0;  // +28
  uint32_t surface = 0;  // +32
  float color_key_red = 0;  // +36
  float color_key_green = 0;  // +40
  float color_key_blue = 0;  // +44
  float color_key_alpha = 0;  // +48
  uint32_t rounding = 0;  // +52
  uint32_t palette = 0;  // +56
  uint32_t box_left = 0;  // +60
  uint32_t box_top = 0;  // +64
  uint32_t box_right = 0;  // +68
  uint32_t box_bottom = 0;  // +72
  uint32_t box_front = 0;  // +76
  uint32_t box_back = 0;  // +80
  uint32_t convert_from = 0;  // +84
  uint32_t row = 0;  // +88
  uint32_t error_row = 0;  // +92
  uint32_t row_pitch = 0;  // +96
  uint32_t slice_pitch = 0;  // +100
  uint32_t width = 0;  // +104
  uint32_t height = 0;  // +108
  uint32_t depth = 0;  // +112
  uint32_t row_bytes = 0;  // +116
  uint32_t bytes_per_pixel = 0;  // +120
  uint8_t pad_124[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(ImageCodecHostData) == 128);
inline constexpr memory::LayoutField kImageCodecFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}};
inline constexpr memory::Layout kImageCodecLayout = {kImageCodecFields, 31};

struct ImageCodec : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8203B778;
  ImageCodecHostData* data() const { return static_cast<ImageCodecHostData*>(memory::HandleObject(address)); }
  static ImageCodec New() {
    return ImageCodec(memory::MakeHandle(new ImageCodecHostData(), sizeof(ImageCodecHostData), "ImageCodec", &kImageCodecLayout));
  }
  template <uint32_t Stride = kSize>
  static ImageCodec NewArray(uint32_t count) {
    static_assert(sizeof(ImageCodecHostData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ImageCodecHostData*>(::operator new(sizeof(ImageCodecHostData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ImageCodecHostData();
    return ImageCodec(memory::MakeHandle(p, sizeof(ImageCodecHostData) * count, "ImageCodec", &kImageCodecLayout, sizeof(ImageCodecHostData)));
  }
  ImageCodec at(uint32_t i) const { return ImageCodec(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  ImageCodecHostData* direct() const { return static_cast<ImageCodecHostData*>(memory::HandleOwn(address, &kImageCodecLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kImageCodecLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kImageCodecLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the base 0x8203B778 (constructor, destructor), then the format's (0x8203B808-0x8203B9D8)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = ImageCodecVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // the description's, without bit 8 (tiled)
  uint32_t format_address() const { return address + 4; }
  uint32_t format_bits() const { if (auto* d = direct()) return d->format; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_format_bits(uint32_t v) const { if (auto* d = direct()) { d->format = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t format() const { if (auto* d = direct()) return d->format; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_format(uint32_t v) const { if (auto* d = direct()) { d->format = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the channels: 1 unsigned, 2 mixed signed, 3 signed, 4 float
  uint32_t kind_address() const { return address + 8; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // bits per pixel isn't 0 (not a block format)
  uint32_t has_bits_address() const { return address + 12; }
  uint32_t has_bits_bits() const { if (auto* d = direct()) return d->has_bits; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_has_bits_bits(uint32_t v) const { if (auto* d = direct()) { d->has_bits = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t has_bits() const { if (auto* d = direct()) return d->has_bits; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_has_bits(uint32_t v) const { if (auto* d = direct()) { d->has_bits = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // sRGB (in for a decode, out for an encode)
  uint32_t srgb_address() const { return address + 16; }
  uint32_t srgb_bits() const { if (auto* d = direct()) return d->srgb; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_srgb_bits(uint32_t v) const { if (auto* d = direct()) { d->srgb = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t srgb() const { if (auto* d = direct()) return d->srgb; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_srgb(uint32_t v) const { if (auto* d = direct()) { d->srgb = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // both sides sRGB (ImageCodec::Prepare)
  uint32_t srgb_both_address() const { return address + 20; }
  uint32_t srgb_both_bits() const { if (auto* d = direct()) return d->srgb_both; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_srgb_both_bits(uint32_t v) const { if (auto* d = direct()) { d->srgb_both = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t srgb_both() const { if (auto* d = direct()) return d->srgb_both; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_srgb_both(uint32_t v) const { if (auto* d = direct()) { d->srgb_both = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // the color key (0: none)
  uint32_t color_key_address() const { return address + 24; }
  uint32_t color_key_bits() const { if (auto* d = direct()) return d->color_key; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_color_key_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t color_key() const { if (auto* d = direct()) return d->color_key; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_color_key(uint32_t v) const { if (auto* d = direct()) { d->color_key = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  // 0 (ImageCodec::Create's LoadPalette test)
  uint32_t palette_set_address() const { return address + 28; }
  uint32_t palette_set_bits() const { if (auto* d = direct()) return d->palette_set; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_palette_set_bits(uint32_t v) const { if (auto* d = direct()) { d->palette_set = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t palette_set() const { if (auto* d = direct()) return d->palette_set; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_palette_set(uint32_t v) const { if (auto* d = direct()) { d->palette_set = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  // the data, at the box's corner for a plain format (named `data` before E5 step 5: a host view's member)
  uint32_t surface_address() const { return address + 32; }
  uint32_t surface_bits() const { if (auto* d = direct()) return d->surface; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_surface_bits(uint32_t v) const { if (auto* d = direct()) { d->surface = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  template <typename T = ImageCodecData>
  T surface() const { return T(surface_bits()); }
  void set_surface(const View& w) const { set_surface_bits(w.address); }
  void set_surface(uint32_t v) const { set_surface_bits(v); }
  // the color key's channels times 1/255
  uint32_t color_key_red_address() const { return address + 36; }
  uint32_t color_key_red_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_red); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_color_key_red_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_red = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float color_key_red() const { return std::bit_cast<float>(color_key_red_bits()); }
  void set_color_key_red(float v) const { set_color_key_red_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t color_key_green_address() const { return address + 40; }
  uint32_t color_key_green_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_green); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_color_key_green_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_green = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float color_key_green() const { return std::bit_cast<float>(color_key_green_bits()); }
  void set_color_key_green(float v) const { set_color_key_green_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t color_key_blue_address() const { return address + 44; }
  uint32_t color_key_blue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_blue); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_color_key_blue_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_blue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float color_key_blue() const { return std::bit_cast<float>(color_key_blue_bits()); }
  void set_color_key_blue(float v) const { set_color_key_blue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t color_key_alpha_address() const { return address + 48; }
  uint32_t color_key_alpha_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_alpha); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_color_key_alpha_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_alpha = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float color_key_alpha() const { return std::bit_cast<float>(color_key_alpha_bits()); }
  void set_color_key_alpha(float v) const { set_color_key_alpha_bits(std::bit_cast<uint32_t>(v)); }
  // the rounding table: 0x8263C768 (0.5s), or the dither's 0x8263C7E8
  uint32_t rounding_address() const { return address + 52; }
  uint32_t rounding_bits() const { if (auto* d = direct()) return d->rounding; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_rounding_bits(uint32_t v) const { if (auto* d = direct()) { d->rounding = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t rounding() const { if (auto* d = direct()) return d->rounding; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_rounding(uint32_t v) const { if (auto* d = direct()) { d->rounding = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // operator new (ImageCodec::LoadPalette): 1,024 bytes
  uint32_t palette_address() const { return address + 56; }
  uint32_t palette_bits() const { if (auto* d = direct()) return d->palette; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_palette_bits(uint32_t v) const { if (auto* d = direct()) { d->palette = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t palette() const { if (auto* d = direct()) return d->palette; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_palette(uint32_t v) const { if (auto* d = direct()) { d->palette = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // the box: left, top, right, bottom, front, back (0, 0, 0 at its corner for a plain format)
  uint32_t box_left_address() const { return address + 60; }
  uint32_t box_left_bits() const { if (auto* d = direct()) return d->box_left; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_box_left_bits(uint32_t v) const { if (auto* d = direct()) { d->box_left = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t box_left() const { if (auto* d = direct()) return d->box_left; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_box_left(uint32_t v) const { if (auto* d = direct()) { d->box_left = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t box_top_address() const { return address + 64; }
  uint32_t box_top_bits() const { if (auto* d = direct()) return d->box_top; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_box_top_bits(uint32_t v) const { if (auto* d = direct()) { d->box_top = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t box_top() const { if (auto* d = direct()) return d->box_top; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_box_top(uint32_t v) const { if (auto* d = direct()) { d->box_top = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t box_right_address() const { return address + 68; }
  uint32_t box_right_bits() const { if (auto* d = direct()) return d->box_right; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_box_right_bits(uint32_t v) const { if (auto* d = direct()) { d->box_right = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t box_right() const { if (auto* d = direct()) return d->box_right; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_box_right(uint32_t v) const { if (auto* d = direct()) { d->box_right = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t box_bottom_address() const { return address + 72; }
  uint32_t box_bottom_bits() const { if (auto* d = direct()) return d->box_bottom; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_box_bottom_bits(uint32_t v) const { if (auto* d = direct()) { d->box_bottom = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t box_bottom() const { if (auto* d = direct()) return d->box_bottom; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_box_bottom(uint32_t v) const { if (auto* d = direct()) { d->box_bottom = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t box_front_address() const { return address + 76; }
  uint32_t box_front_bits() const { if (auto* d = direct()) return d->box_front; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_box_front_bits(uint32_t v) const { if (auto* d = direct()) { d->box_front = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t box_front() const { if (auto* d = direct()) return d->box_front; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_box_front(uint32_t v) const { if (auto* d = direct()) { d->box_front = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t box_back_address() const { return address + 80; }
  uint32_t box_back_bits() const { if (auto* d = direct()) return d->box_back; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_box_back_bits(uint32_t v) const { if (auto* d = direct()) { d->box_back = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t box_back() const { if (auto* d = direct()) return d->box_back; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_box_back(uint32_t v) const { if (auto* d = direct()) { d->box_back = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  // the other side's channel kind (ConvertRange; 0 none)
  uint32_t convert_from_address() const { return address + 84; }
  uint32_t convert_from_bits() const { if (auto* d = direct()) return d->convert_from; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_convert_from_bits(uint32_t v) const { if (auto* d = direct()) { d->convert_from = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t convert_from() const { if (auto* d = direct()) return d->convert_from; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_convert_from(uint32_t v) const { if (auto* d = direct()) { d->convert_from = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  // a float4 row (operator new, ImageCodec::Prepare)
  uint32_t row_address() const { return address + 88; }
  uint32_t row_bits() const { if (auto* d = direct()) return d->row; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_row_bits(uint32_t v) const { if (auto* d = direct()) { d->row = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  template <typename T = TcFloat4>
  T row() const { return T(row_bits()); }
  void set_row(const View& w) const { set_row_bits(w.address); }
  void set_row(uint32_t v) const { set_row_bits(v); }
  // the error diffusion's (width + 2) float4s (operator new), or 0
  uint32_t error_row_address() const { return address + 92; }
  uint32_t error_row_bits() const { if (auto* d = direct()) return d->error_row; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_error_row_bits(uint32_t v) const { if (auto* d = direct()) { d->error_row = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  template <typename T = TcFloat4>
  T error_row() const { return T(error_row_bits()); }
  void set_error_row(const View& w) const { set_error_row_bits(w.address); }
  void set_error_row(uint32_t v) const { set_error_row_bits(v); }
  uint32_t row_pitch_address() const { return address + 96; }
  uint32_t row_pitch_bits() const { if (auto* d = direct()) return d->row_pitch; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_row_pitch_bits(uint32_t v) const { if (auto* d = direct()) { d->row_pitch = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t row_pitch() const { if (auto* d = direct()) return d->row_pitch; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_row_pitch(uint32_t v) const { if (auto* d = direct()) { d->row_pitch = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t slice_pitch_address() const { return address + 100; }
  uint32_t slice_pitch_bits() const { if (auto* d = direct()) return d->slice_pitch; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_slice_pitch_bits(uint32_t v) const { if (auto* d = direct()) { d->slice_pitch = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t slice_pitch() const { if (auto* d = direct()) return d->slice_pitch; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_slice_pitch(uint32_t v) const { if (auto* d = direct()) { d->slice_pitch = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  // the box's size
  uint32_t width_address() const { return address + 104; }
  uint32_t width_bits() const { if (auto* d = direct()) return d->width; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_width_bits(uint32_t v) const { if (auto* d = direct()) { d->width = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t width() const { if (auto* d = direct()) return d->width; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_width(uint32_t v) const { if (auto* d = direct()) { d->width = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t height_address() const { return address + 108; }
  uint32_t height_bits() const { if (auto* d = direct()) return d->height; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_height_bits(uint32_t v) const { if (auto* d = direct()) { d->height = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t height() const { if (auto* d = direct()) return d->height; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_height(uint32_t v) const { if (auto* d = direct()) { d->height = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t depth_address() const { return address + 112; }
  uint32_t depth_bits() const { if (auto* d = direct()) return d->depth; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_depth_bits(uint32_t v) const { if (auto* d = direct()) { d->depth = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t depth() const { if (auto* d = direct()) return d->depth; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_depth(uint32_t v) const { if (auto* d = direct()) { d->depth = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  // width x bytes per pixel
  uint32_t row_bytes_address() const { return address + 116; }
  uint32_t row_bytes_bits() const { if (auto* d = direct()) return d->row_bytes; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_row_bytes_bits(uint32_t v) const { if (auto* d = direct()) { d->row_bytes = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t row_bytes() const { if (auto* d = direct()) return d->row_bytes; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_row_bytes(uint32_t v) const { if (auto* d = direct()) { d->row_bytes = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t bytes_per_pixel_address() const { return address + 120; }
  uint32_t bytes_per_pixel_bits() const { if (auto* d = direct()) return d->bytes_per_pixel; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_bytes_per_pixel_bits(uint32_t v) const { if (auto* d = direct()) { d->bytes_per_pixel = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t bytes_per_pixel() const { if (auto* d = direct()) return d->bytes_per_pixel; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_bytes_per_pixel(uint32_t v) const { if (auto* d = direct()) { d->bytes_per_pixel = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
};

// image; measure: ImageCodec.+0
struct ImageCodecVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the scalar deleting destructor (r4 the flags)
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // a row to float4s: r4 the row, r5 the slice, r6 the float4s
  uint32_t decode_row() const { return guest::LoadU32(address + 4); }
  void set_decode_row(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t decode_row_address() const { return address + 4; }
  // a row from float4s, as decode_row
  uint32_t encode_row() const { return guest::LoadU32(address + 8); }
  void set_encode_row(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t encode_row_address() const { return address + 8; }
  // the color key quantized to the format
  uint32_t quantize_color_key() const { return guest::LoadU32(address + 12); }
  void set_quantize_color_key(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t quantize_color_key_address() const { return address + 12; }
};

// file; measure: ImageCodec.+32, A16B16G16R16Codec.+32, A1R5G5B5Codec.+32, A2B10G10R10Codec.+32, A2R10G10B10Codec.+32, A2W10V10U10Codec.+32, A4R4G4B4Codec.+32, A8B8G8R8Codec.+32, A8Codec.+32, A8L8Codec.+32, A8R8G8B8Codec.+32, G16R16Codec.+32, G32R32FCodec.+32, L16Codec.+32, L6V5U5Codec.+32, L8Codec.+32, Q16W16V16U16Codec.+32, Q8W8V8U8Codec.+32, R32FCodec.+32, R5G6B5Codec.+32, V16U16Codec.+32, V8U8Codec.+32, X1R5G5B5Codec.+32, X4R4G4B4Codec.+32, X8B8G8R8Codec.+32, X8L8V8U8Codec.+32, X8R8G8B8Codec.+32, YuvCodec.+32, new<?>@XGCompressSurface
struct ImageCodecData : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint8_t byte0() const { return guest::LoadU8(address + 0); }
  void set_byte0(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t byte0_address() const { return address + 0; }
  uint16_t half0() const { return guest::LoadU16(address + 0); }
  void set_half0(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t half0_address() const { return address + 0; }
  uint8_t byte1() const { return guest::LoadU8(address + 1); }
  void set_byte1(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t byte1_address() const { return address + 1; }
  uint16_t half1() const { return guest::LoadU16(address + 2); }
  void set_half1(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t half1_address() const { return address + 2; }
};

// file; measure: ImageCodec.+68, ImageCodec.+88, ImageCodec::LinearToSrgb.ret, ImageCodec::ConvertRange.r4, ImageCodec::ApplyColorKey.r4, ImageCodec::LinearToSrgb.r4, ImageCodec::SrgbToLinear.r4, ImageCodec::AddDiffusionError.r4, Dxt, Dxt1::EncodeBlock.r4, Dxt23::EncodeBlockPremultiplied.r4, DxtAlpha::EncodeBlock.r4, DxtColor::EncodeBlock.r4, DxtColor::FitEndpoints.r4, DxtColor::FitEndpoints.r5, DxtAlpha::FitEndpoints.r4, DxtAlpha::FitEndpoints.r5, A16B16G16R16Codec.+92, A1R5G5B5Codec.+92, A2B10G10R10Codec.+92, A2R10G10B10Codec.+92, A2W10V10U10Codec.+92, A4R4G4B4Codec.+92, A8B8G8R8Codec.+92, A8Codec.+92, A8L8Codec.+92, A8R8G8B8Codec.+92, G16R16Codec.+92, L16Codec.+92, L6V5U5Codec.+92, L8Codec.+92, Q16W16V16U16Codec.+92, Q8W8V8U8Codec.+92, R5G6B5Codec.+92, V16U16Codec.+92, V8U8Codec.+92, X1R5G5B5Codec.+92, X4R4G4B4Codec.+92, X8B8G8R8Codec.+92, X8L8V8U8Codec.+92, X8R8G8B8Codec.+92, A8R8G8B8Codec::DecodeRow.r6, X8R8G8B8Codec::DecodeRow.r6, R5G6B5Codec::DecodeRow.r6, X1R5G5B5Codec::DecodeRow.r6, A1R5G5B5Codec::DecodeRow.r6, A4R4G4B4Codec::DecodeRow.r6, A2B10G10R10Codec::DecodeRow.r6, A8B8G8R8Codec::DecodeRow.r6, X8B8G8R8Codec::DecodeRow.r6, G16R16Codec::DecodeRow.r6, A2R10G10B10Codec::DecodeRow.r6, A16B16G16R16Codec::DecodeRow.r6, A8Codec::DecodeRow.r6, X4R4G4B4Codec::DecodeRow.r6, L8Codec::DecodeRow.r6, A8L8Codec::DecodeRow.r6, V8U8Codec::DecodeRow.r6, L6V5U5Codec::DecodeRow.r6, X8L8V8U8Codec::DecodeRow.r6, Q8W8V8U8Codec::DecodeRow.r6, V16U16Codec::DecodeRow.r6, A2W10V10U10Codec::DecodeRow.r6, Q16W16V16U16Codec::DecodeRow.r6, L16Codec::DecodeRow.r6, R32FCodec::DecodeRow.r6, G32R32FCodec::DecodeRow.r6, DxtCodec::DecodeRow.r6, new<4294967295>@Blitter::FilterPoint, new<4294967295>@Blitter::FilterPoint#2, new<4294967295>@Blitter::FilterBox2D, new<4294967295>@Blitter::FilterBox3D, new<4294967295>@Blitter::FilterLinear2D, new<4294967295>@Blitter::FilterLinear2D#2, new<4294967295>@Blitter::FilterLinear3D, new<4294967295>@Blitter::FilterLinear3D#2, new<4294967295>@Blitter::FilterTriangle3D, new<4294967295>@Blitter::FilterTriangle3D#2, new<4294967295>@Blitter::FilterTriangle2D, new<4294967295>@Blitter::FilterTriangle2D#2, new<?>@Blitter::FilterBox2D, new<?>@Blitter::FilterBox3D
struct TcFloat4 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static TcFloat4 New(uint32_t size) { return TcFloat4(memory::NewBlob(size, "TcFloat4")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  float red() const { return guest::LoadFloat(address + 0); }
  void set_red(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t red_address() const { return address + 0; }
  float green() const { return guest::LoadFloat(address + 4); }
  void set_green(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t green_address() const { return address + 4; }
  float blue() const { return guest::LoadFloat(address + 8); }
  void set_blue(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t blue_address() const { return address + 8; }
  float alpha() const { return guest::LoadFloat(address + 12); }
  void set_alpha(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t alpha_address() const { return address + 12; }
};

// allocated; measure: DxtCodec
#pragma pack(push, 1)
struct DxtCodecData {
  uint32_t vtable = 0;  // +0
  uint32_t format = 0;  // +4
  uint32_t kind = 0;  // +8
  uint32_t has_bits = 0;  // +12
  uint32_t srgb = 0;  // +16
  uint32_t srgb_both = 0;  // +20
  uint32_t color_key = 0;  // +24
  uint32_t palette_set = 0;  // +28
  uint32_t surface = 0;  // +32
  float color_key_red = 0;  // +36
  float color_key_green = 0;  // +40
  float color_key_blue = 0;  // +44
  float color_key_alpha = 0;  // +48
  uint32_t rounding = 0;  // +52
  uint32_t palette = 0;  // +56
  uint32_t box_left = 0;  // +60
  uint32_t box_top = 0;  // +64
  uint32_t box_right = 0;  // +68
  uint32_t box_bottom = 0;  // +72
  uint32_t box_front = 0;  // +76
  uint32_t box_back = 0;  // +80
  uint32_t convert_from = 0;  // +84
  uint32_t row = 0;  // +88
  uint32_t error_row = 0;  // +92
  uint32_t row_pitch = 0;  // +96
  uint32_t slice_pitch = 0;  // +100
  uint32_t width = 0;  // +104
  uint32_t height = 0;  // +108
  uint32_t depth = 0;  // +112
  uint32_t row_bytes = 0;  // +116
  uint32_t bytes_per_pixel = 0;  // +120
  uint8_t pad_124[4] = {};
  float unknown_128 = 0;  // +128
  float unknown_132 = 0;  // +132
  uint32_t block_bytes = 0;  // +136
  uint32_t decode_block = 0;  // +140
  uint32_t encode_block = 0;  // +144
  uint32_t extent_left = 0;  // +148
  uint32_t extent_top = 0;  // +152
  uint32_t extent_right = 0;  // +156
  uint32_t extent_bottom = 0;  // +160
  uint32_t extent_front = 0;  // +164
  uint32_t extent_back = 0;  // +168
  uint32_t block_extent_left = 0;  // +172
  uint32_t block_extent_top = 0;  // +176
  uint32_t block_extent_right = 0;  // +180
  uint32_t block_extent_bottom = 0;  // +184
  uint32_t block_extent_front = 0;  // +188
  uint32_t block_extent_back = 0;  // +192
  uint32_t block_left = 0;  // +196
  uint32_t block_top = 0;  // +200
  uint32_t block_right = 0;  // +204
  uint32_t block_bottom = 0;  // +208
  uint32_t front = 0;  // +212
  uint32_t back = 0;  // +216
  uint32_t block_columns = 0;  // +220
  uint32_t block_rows = 0;  // +224
  uint32_t slices = 0;  // +228
  uint32_t unknown_232 = 0;  // +232
  uint32_t unknown_236 = 0;  // +236
  uint32_t decode_cache = 0;  // +240
  uint32_t cache_count = 0;  // +244
  uint32_t cache = 0;  // +248
  uint32_t dither = 0;  // +252
};
#pragma pack(pop)
static_assert(sizeof(DxtCodecData) == 256);
inline constexpr memory::LayoutField kDxtCodecFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {176, 4}, {180, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {220, 4}, {224, 4}, {228, 4}, {232, 4}, {236, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}};
inline constexpr memory::Layout kDxtCodecLayout = {kDxtCodecFields, 63};

struct DxtCodec : View {
  using View::View;
  static constexpr uint32_t kSize = 256;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8203B9E8;
  DxtCodecData* data() const { return static_cast<DxtCodecData*>(memory::HandleObject(address)); }
  static DxtCodec New() {
    return DxtCodec(memory::MakeHandle(new DxtCodecData(), sizeof(DxtCodecData), "DxtCodec", &kDxtCodecLayout));
  }
  template <uint32_t Stride = kSize>
  static DxtCodec NewArray(uint32_t count) {
    static_assert(sizeof(DxtCodecData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<DxtCodecData*>(::operator new(sizeof(DxtCodecData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) DxtCodecData();
    return DxtCodec(memory::MakeHandle(p, sizeof(DxtCodecData) * count, "DxtCodec", &kDxtCodecLayout, sizeof(DxtCodecData)));
  }
  DxtCodec at(uint32_t i) const { return DxtCodec(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  DxtCodecData* direct() const { return static_cast<DxtCodecData*>(memory::HandleOwn(address, &kDxtCodecLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kDxtCodecLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kDxtCodecLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x8203B9E8 (the base 0x8203B778 inside ImageCodec's constructor and destructor)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = ImageCodecVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // as ImageCodec's
  uint32_t format_address() const { return address + 4; }
  uint32_t format_bits() const { if (auto* d = direct()) return d->format; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_format_bits(uint32_t v) const { if (auto* d = direct()) { d->format = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t format() const { if (auto* d = direct()) return d->format; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_format(uint32_t v) const { if (auto* d = direct()) { d->format = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t kind_address() const { return address + 8; }
  uint32_t kind_bits() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_kind_bits(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t kind() const { if (auto* d = direct()) return d->kind; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_kind(uint32_t v) const { if (auto* d = direct()) { d->kind = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t has_bits_address() const { return address + 12; }
  uint32_t has_bits_bits() const { if (auto* d = direct()) return d->has_bits; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_has_bits_bits(uint32_t v) const { if (auto* d = direct()) { d->has_bits = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t has_bits() const { if (auto* d = direct()) return d->has_bits; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_has_bits(uint32_t v) const { if (auto* d = direct()) { d->has_bits = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t srgb_address() const { return address + 16; }
  uint32_t srgb_bits() const { if (auto* d = direct()) return d->srgb; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_srgb_bits(uint32_t v) const { if (auto* d = direct()) { d->srgb = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t srgb() const { if (auto* d = direct()) return d->srgb; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_srgb(uint32_t v) const { if (auto* d = direct()) { d->srgb = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t srgb_both_address() const { return address + 20; }
  uint32_t srgb_both_bits() const { if (auto* d = direct()) return d->srgb_both; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_srgb_both_bits(uint32_t v) const { if (auto* d = direct()) { d->srgb_both = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t srgb_both() const { if (auto* d = direct()) return d->srgb_both; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_srgb_both(uint32_t v) const { if (auto* d = direct()) { d->srgb_both = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t color_key_address() const { return address + 24; }
  uint32_t color_key_bits() const { if (auto* d = direct()) return d->color_key; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_color_key_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t color_key() const { if (auto* d = direct()) return d->color_key; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_color_key(uint32_t v) const { if (auto* d = direct()) { d->color_key = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  uint32_t palette_set_address() const { return address + 28; }
  uint32_t palette_set_bits() const { if (auto* d = direct()) return d->palette_set; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_palette_set_bits(uint32_t v) const { if (auto* d = direct()) { d->palette_set = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t palette_set() const { if (auto* d = direct()) return d->palette_set; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_palette_set(uint32_t v) const { if (auto* d = direct()) { d->palette_set = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  uint32_t surface_address() const { return address + 32; }
  uint32_t surface_bits() const { if (auto* d = direct()) return d->surface; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_surface_bits(uint32_t v) const { if (auto* d = direct()) { d->surface = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  template <typename T = ImageCodecData>
  T surface() const { return T(surface_bits()); }
  void set_surface(const View& w) const { set_surface_bits(w.address); }
  void set_surface(uint32_t v) const { set_surface_bits(v); }
  uint32_t color_key_red_address() const { return address + 36; }
  uint32_t color_key_red_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_red); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_color_key_red_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_red = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float color_key_red() const { return std::bit_cast<float>(color_key_red_bits()); }
  void set_color_key_red(float v) const { set_color_key_red_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t color_key_green_address() const { return address + 40; }
  uint32_t color_key_green_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_green); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_color_key_green_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_green = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float color_key_green() const { return std::bit_cast<float>(color_key_green_bits()); }
  void set_color_key_green(float v) const { set_color_key_green_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t color_key_blue_address() const { return address + 44; }
  uint32_t color_key_blue_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_blue); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_color_key_blue_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_blue = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float color_key_blue() const { return std::bit_cast<float>(color_key_blue_bits()); }
  void set_color_key_blue(float v) const { set_color_key_blue_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t color_key_alpha_address() const { return address + 48; }
  uint32_t color_key_alpha_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->color_key_alpha); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_color_key_alpha_bits(uint32_t v) const { if (auto* d = direct()) { d->color_key_alpha = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float color_key_alpha() const { return std::bit_cast<float>(color_key_alpha_bits()); }
  void set_color_key_alpha(float v) const { set_color_key_alpha_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t rounding_address() const { return address + 52; }
  uint32_t rounding_bits() const { if (auto* d = direct()) return d->rounding; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_rounding_bits(uint32_t v) const { if (auto* d = direct()) { d->rounding = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t rounding() const { if (auto* d = direct()) return d->rounding; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_rounding(uint32_t v) const { if (auto* d = direct()) { d->rounding = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t palette_address() const { return address + 56; }
  uint32_t palette_bits() const { if (auto* d = direct()) return d->palette; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_palette_bits(uint32_t v) const { if (auto* d = direct()) { d->palette = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t palette() const { if (auto* d = direct()) return d->palette; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_palette(uint32_t v) const { if (auto* d = direct()) { d->palette = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t box_left_address() const { return address + 60; }
  uint32_t box_left_bits() const { if (auto* d = direct()) return d->box_left; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_box_left_bits(uint32_t v) const { if (auto* d = direct()) { d->box_left = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t box_left() const { if (auto* d = direct()) return d->box_left; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_box_left(uint32_t v) const { if (auto* d = direct()) { d->box_left = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t box_top_address() const { return address + 64; }
  uint32_t box_top_bits() const { if (auto* d = direct()) return d->box_top; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_box_top_bits(uint32_t v) const { if (auto* d = direct()) { d->box_top = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t box_top() const { if (auto* d = direct()) return d->box_top; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_box_top(uint32_t v) const { if (auto* d = direct()) { d->box_top = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t box_right_address() const { return address + 68; }
  uint32_t box_right_bits() const { if (auto* d = direct()) return d->box_right; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_box_right_bits(uint32_t v) const { if (auto* d = direct()) { d->box_right = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t box_right() const { if (auto* d = direct()) return d->box_right; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_box_right(uint32_t v) const { if (auto* d = direct()) { d->box_right = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  uint32_t box_bottom_address() const { return address + 72; }
  uint32_t box_bottom_bits() const { if (auto* d = direct()) return d->box_bottom; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_box_bottom_bits(uint32_t v) const { if (auto* d = direct()) { d->box_bottom = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t box_bottom() const { if (auto* d = direct()) return d->box_bottom; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_box_bottom(uint32_t v) const { if (auto* d = direct()) { d->box_bottom = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  uint32_t box_front_address() const { return address + 76; }
  uint32_t box_front_bits() const { if (auto* d = direct()) return d->box_front; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_box_front_bits(uint32_t v) const { if (auto* d = direct()) { d->box_front = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t box_front() const { if (auto* d = direct()) return d->box_front; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_box_front(uint32_t v) const { if (auto* d = direct()) { d->box_front = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  uint32_t box_back_address() const { return address + 80; }
  uint32_t box_back_bits() const { if (auto* d = direct()) return d->box_back; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_box_back_bits(uint32_t v) const { if (auto* d = direct()) { d->box_back = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t box_back() const { if (auto* d = direct()) return d->box_back; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_box_back(uint32_t v) const { if (auto* d = direct()) { d->box_back = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  uint32_t convert_from_address() const { return address + 84; }
  uint32_t convert_from_bits() const { if (auto* d = direct()) return d->convert_from; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_convert_from_bits(uint32_t v) const { if (auto* d = direct()) { d->convert_from = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t convert_from() const { if (auto* d = direct()) return d->convert_from; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_convert_from(uint32_t v) const { if (auto* d = direct()) { d->convert_from = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  uint32_t row_address() const { return address + 88; }
  uint32_t row_bits() const { if (auto* d = direct()) return d->row; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_row_bits(uint32_t v) const { if (auto* d = direct()) { d->row = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  template <typename T = TcFloat4>
  T row() const { return T(row_bits()); }
  void set_row(const View& w) const { set_row_bits(w.address); }
  void set_row(uint32_t v) const { set_row_bits(v); }
  uint32_t error_row_address() const { return address + 92; }
  uint32_t error_row_bits() const { if (auto* d = direct()) return d->error_row; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_error_row_bits(uint32_t v) const { if (auto* d = direct()) { d->error_row = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  template <typename T = TcFloat4>
  T error_row() const { return T(error_row_bits()); }
  void set_error_row(const View& w) const { set_error_row_bits(w.address); }
  void set_error_row(uint32_t v) const { set_error_row_bits(v); }
  uint32_t row_pitch_address() const { return address + 96; }
  uint32_t row_pitch_bits() const { if (auto* d = direct()) return d->row_pitch; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_row_pitch_bits(uint32_t v) const { if (auto* d = direct()) { d->row_pitch = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t row_pitch() const { if (auto* d = direct()) return d->row_pitch; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_row_pitch(uint32_t v) const { if (auto* d = direct()) { d->row_pitch = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  uint32_t slice_pitch_address() const { return address + 100; }
  uint32_t slice_pitch_bits() const { if (auto* d = direct()) return d->slice_pitch; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_slice_pitch_bits(uint32_t v) const { if (auto* d = direct()) { d->slice_pitch = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t slice_pitch() const { if (auto* d = direct()) return d->slice_pitch; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_slice_pitch(uint32_t v) const { if (auto* d = direct()) { d->slice_pitch = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  uint32_t width_address() const { return address + 104; }
  uint32_t width_bits() const { if (auto* d = direct()) return d->width; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_width_bits(uint32_t v) const { if (auto* d = direct()) { d->width = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t width() const { if (auto* d = direct()) return d->width; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_width(uint32_t v) const { if (auto* d = direct()) { d->width = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  uint32_t height_address() const { return address + 108; }
  uint32_t height_bits() const { if (auto* d = direct()) return d->height; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_height_bits(uint32_t v) const { if (auto* d = direct()) { d->height = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t height() const { if (auto* d = direct()) return d->height; return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_height(uint32_t v) const { if (auto* d = direct()) { d->height = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  uint32_t depth_address() const { return address + 112; }
  uint32_t depth_bits() const { if (auto* d = direct()) return d->depth; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_depth_bits(uint32_t v) const { if (auto* d = direct()) { d->depth = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t depth() const { if (auto* d = direct()) return d->depth; return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_depth(uint32_t v) const { if (auto* d = direct()) { d->depth = v; return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  uint32_t row_bytes_address() const { return address + 116; }
  uint32_t row_bytes_bits() const { if (auto* d = direct()) return d->row_bytes; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_row_bytes_bits(uint32_t v) const { if (auto* d = direct()) { d->row_bytes = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t row_bytes() const { if (auto* d = direct()) return d->row_bytes; return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_row_bytes(uint32_t v) const { if (auto* d = direct()) { d->row_bytes = v; return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  uint32_t bytes_per_pixel_address() const { return address + 120; }
  uint32_t bytes_per_pixel_bits() const { if (auto* d = direct()) return d->bytes_per_pixel; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_bytes_per_pixel_bits(uint32_t v) const { if (auto* d = direct()) { d->bytes_per_pixel = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t bytes_per_pixel() const { if (auto* d = direct()) return d->bytes_per_pixel; return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_bytes_per_pixel(uint32_t v) const { if (auto* d = direct()) { d->bytes_per_pixel = v; return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  uint32_t unknown_128_address() const { return address + 128; }
  uint32_t unknown_128_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_128); return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_unknown_128_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_128 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  float unknown_128() const { return std::bit_cast<float>(unknown_128_bits()); }
  void set_unknown_128(float v) const { set_unknown_128_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t unknown_132_address() const { return address + 132; }
  uint32_t unknown_132_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_132); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_unknown_132_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_132 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  float unknown_132() const { return std::bit_cast<float>(unknown_132_bits()); }
  void set_unknown_132(float v) const { set_unknown_132_bits(std::bit_cast<uint32_t>(v)); }
  // 8 (DXT1, 3A, 5A) or 16 (DXT2/3, 4/5)
  uint32_t block_bytes_address() const { return address + 136; }
  uint32_t block_bytes_bits() const { if (auto* d = direct()) return d->block_bytes; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_block_bytes_bits(uint32_t v) const { if (auto* d = direct()) { d->block_bytes = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  uint32_t block_bytes() const { if (auto* d = direct()) return d->block_bytes; return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_block_bytes(uint32_t v) const { if (auto* d = direct()) { d->block_bytes = v; return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  // r3 = 16 float4s, r4 = the block (DXT1 0x82421938, ...)
  uint32_t decode_block_address() const { return address + 140; }
  uint32_t decode_block_bits() const { if (auto* d = direct()) return d->decode_block; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_decode_block_bits(uint32_t v) const { if (auto* d = direct()) { d->decode_block = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  uint32_t decode_block() const { if (auto* d = direct()) return d->decode_block; return uint32_t((memory::IsHandle(address + 140) ? uint32_t(memory::OwnerLoad(address + 140, 4)) : guest::LoadU32(address + 140))); }
  void set_decode_block(uint32_t v) const { if (auto* d = direct()) { d->decode_block = v; return; } if (memory::IsHandle(address + 140)) memory::OwnerStore(address + 140, 4, v); else guest::StoreU32(address + 140, v); }
  // r3 = the block, r4 = 16 float4s, r5 = dither (DXT4/5 0x82422A68, ...)
  uint32_t encode_block_address() const { return address + 144; }
  uint32_t encode_block_bits() const { if (auto* d = direct()) return d->encode_block; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_encode_block_bits(uint32_t v) const { if (auto* d = direct()) { d->encode_block = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  uint32_t encode_block() const { if (auto* d = direct()) return d->encode_block; return uint32_t((memory::IsHandle(address + 144) ? uint32_t(memory::OwnerLoad(address + 144, 4)) : guest::LoadU32(address + 144))); }
  void set_encode_block(uint32_t v) const { if (auto* d = direct()) { d->encode_block = v; return; } if (memory::IsHandle(address + 144)) memory::OwnerStore(address + 144, 4, v); else guest::StoreU32(address + 144, v); }
  // the surface: 0, 0, width, height, 0, depth (the description's extent)
  uint32_t extent_left_address() const { return address + 148; }
  uint32_t extent_left_bits() const { if (auto* d = direct()) return d->extent_left; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_extent_left_bits(uint32_t v) const { if (auto* d = direct()) { d->extent_left = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t extent_left() const { if (auto* d = direct()) return d->extent_left; return uint32_t((memory::IsHandle(address + 148) ? uint32_t(memory::OwnerLoad(address + 148, 4)) : guest::LoadU32(address + 148))); }
  void set_extent_left(uint32_t v) const { if (auto* d = direct()) { d->extent_left = v; return; } if (memory::IsHandle(address + 148)) memory::OwnerStore(address + 148, 4, v); else guest::StoreU32(address + 148, v); }
  uint32_t extent_top_address() const { return address + 152; }
  uint32_t extent_top_bits() const { if (auto* d = direct()) return d->extent_top; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_extent_top_bits(uint32_t v) const { if (auto* d = direct()) { d->extent_top = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t extent_top() const { if (auto* d = direct()) return d->extent_top; return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(address + 152))); }
  void set_extent_top(uint32_t v) const { if (auto* d = direct()) { d->extent_top = v; return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(address + 152, v); }
  uint32_t extent_right_address() const { return address + 156; }
  uint32_t extent_right_bits() const { if (auto* d = direct()) return d->extent_right; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_extent_right_bits(uint32_t v) const { if (auto* d = direct()) { d->extent_right = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  uint32_t extent_right() const { if (auto* d = direct()) return d->extent_right; return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(address + 156))); }
  void set_extent_right(uint32_t v) const { if (auto* d = direct()) { d->extent_right = v; return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(address + 156, v); }
  uint32_t extent_bottom_address() const { return address + 160; }
  uint32_t extent_bottom_bits() const { if (auto* d = direct()) return d->extent_bottom; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_extent_bottom_bits(uint32_t v) const { if (auto* d = direct()) { d->extent_bottom = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  uint32_t extent_bottom() const { if (auto* d = direct()) return d->extent_bottom; return uint32_t((memory::IsHandle(address + 160) ? uint32_t(memory::OwnerLoad(address + 160, 4)) : guest::LoadU32(address + 160))); }
  void set_extent_bottom(uint32_t v) const { if (auto* d = direct()) { d->extent_bottom = v; return; } if (memory::IsHandle(address + 160)) memory::OwnerStore(address + 160, 4, v); else guest::StoreU32(address + 160, v); }
  uint32_t extent_front_address() const { return address + 164; }
  uint32_t extent_front_bits() const { if (auto* d = direct()) return d->extent_front; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_extent_front_bits(uint32_t v) const { if (auto* d = direct()) { d->extent_front = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  uint32_t extent_front() const { if (auto* d = direct()) return d->extent_front; return uint32_t((memory::IsHandle(address + 164) ? uint32_t(memory::OwnerLoad(address + 164, 4)) : guest::LoadU32(address + 164))); }
  void set_extent_front(uint32_t v) const { if (auto* d = direct()) { d->extent_front = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 4, v); else guest::StoreU32(address + 164, v); }
  uint32_t extent_back_address() const { return address + 168; }
  uint32_t extent_back_bits() const { if (auto* d = direct()) return d->extent_back; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_extent_back_bits(uint32_t v) const { if (auto* d = direct()) { d->extent_back = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  uint32_t extent_back() const { if (auto* d = direct()) return d->extent_back; return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_extent_back(uint32_t v) const { if (auto* d = direct()) { d->extent_back = v; return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  // the extent rounded out to blocks
  uint32_t block_extent_left_address() const { return address + 172; }
  uint32_t block_extent_left_bits() const { if (auto* d = direct()) return d->block_extent_left; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_block_extent_left_bits(uint32_t v) const { if (auto* d = direct()) { d->block_extent_left = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  uint32_t block_extent_left() const { if (auto* d = direct()) return d->block_extent_left; return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_block_extent_left(uint32_t v) const { if (auto* d = direct()) { d->block_extent_left = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  uint32_t block_extent_top_address() const { return address + 176; }
  uint32_t block_extent_top_bits() const { if (auto* d = direct()) return d->block_extent_top; return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(address + 176))); }
  void set_block_extent_top_bits(uint32_t v) const { if (auto* d = direct()) { d->block_extent_top = v; return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(address + 176, v); }
  uint32_t block_extent_top() const { if (auto* d = direct()) return d->block_extent_top; return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(address + 176))); }
  void set_block_extent_top(uint32_t v) const { if (auto* d = direct()) { d->block_extent_top = v; return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(address + 176, v); }
  uint32_t block_extent_right_address() const { return address + 180; }
  uint32_t block_extent_right_bits() const { if (auto* d = direct()) return d->block_extent_right; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(address + 180))); }
  void set_block_extent_right_bits(uint32_t v) const { if (auto* d = direct()) { d->block_extent_right = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(address + 180, v); }
  uint32_t block_extent_right() const { if (auto* d = direct()) return d->block_extent_right; return uint32_t((memory::IsHandle(address + 180) ? uint32_t(memory::OwnerLoad(address + 180, 4)) : guest::LoadU32(address + 180))); }
  void set_block_extent_right(uint32_t v) const { if (auto* d = direct()) { d->block_extent_right = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 4, v); else guest::StoreU32(address + 180, v); }
  uint32_t block_extent_bottom_address() const { return address + 184; }
  uint32_t block_extent_bottom_bits() const { if (auto* d = direct()) return d->block_extent_bottom; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_block_extent_bottom_bits(uint32_t v) const { if (auto* d = direct()) { d->block_extent_bottom = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  uint32_t block_extent_bottom() const { if (auto* d = direct()) return d->block_extent_bottom; return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(address + 184))); }
  void set_block_extent_bottom(uint32_t v) const { if (auto* d = direct()) { d->block_extent_bottom = v; return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(address + 184, v); }
  uint32_t block_extent_front_address() const { return address + 188; }
  uint32_t block_extent_front_bits() const { if (auto* d = direct()) return d->block_extent_front; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_block_extent_front_bits(uint32_t v) const { if (auto* d = direct()) { d->block_extent_front = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  uint32_t block_extent_front() const { if (auto* d = direct()) return d->block_extent_front; return uint32_t((memory::IsHandle(address + 188) ? uint32_t(memory::OwnerLoad(address + 188, 4)) : guest::LoadU32(address + 188))); }
  void set_block_extent_front(uint32_t v) const { if (auto* d = direct()) { d->block_extent_front = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 4, v); else guest::StoreU32(address + 188, v); }
  uint32_t block_extent_back_address() const { return address + 192; }
  uint32_t block_extent_back_bits() const { if (auto* d = direct()) return d->block_extent_back; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_block_extent_back_bits(uint32_t v) const { if (auto* d = direct()) { d->block_extent_back = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  uint32_t block_extent_back() const { if (auto* d = direct()) return d->block_extent_back; return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(address + 192))); }
  void set_block_extent_back(uint32_t v) const { if (auto* d = direct()) { d->block_extent_back = v; return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(address + 192, v); }
  // the box rounded out to blocks (pixels)
  uint32_t block_left_address() const { return address + 196; }
  uint32_t block_left_bits() const { if (auto* d = direct()) return d->block_left; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_block_left_bits(uint32_t v) const { if (auto* d = direct()) { d->block_left = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  uint32_t block_left() const { if (auto* d = direct()) return d->block_left; return uint32_t((memory::IsHandle(address + 196) ? uint32_t(memory::OwnerLoad(address + 196, 4)) : guest::LoadU32(address + 196))); }
  void set_block_left(uint32_t v) const { if (auto* d = direct()) { d->block_left = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 4, v); else guest::StoreU32(address + 196, v); }
  uint32_t block_top_address() const { return address + 200; }
  uint32_t block_top_bits() const { if (auto* d = direct()) return d->block_top; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(address + 200))); }
  void set_block_top_bits(uint32_t v) const { if (auto* d = direct()) { d->block_top = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(address + 200, v); }
  uint32_t block_top() const { if (auto* d = direct()) return d->block_top; return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(address + 200))); }
  void set_block_top(uint32_t v) const { if (auto* d = direct()) { d->block_top = v; return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(address + 200, v); }
  uint32_t block_right_address() const { return address + 204; }
  uint32_t block_right_bits() const { if (auto* d = direct()) return d->block_right; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_block_right_bits(uint32_t v) const { if (auto* d = direct()) { d->block_right = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  uint32_t block_right() const { if (auto* d = direct()) return d->block_right; return uint32_t((memory::IsHandle(address + 204) ? uint32_t(memory::OwnerLoad(address + 204, 4)) : guest::LoadU32(address + 204))); }
  void set_block_right(uint32_t v) const { if (auto* d = direct()) { d->block_right = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 4, v); else guest::StoreU32(address + 204, v); }
  uint32_t block_bottom_address() const { return address + 208; }
  uint32_t block_bottom_bits() const { if (auto* d = direct()) return d->block_bottom; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_block_bottom_bits(uint32_t v) const { if (auto* d = direct()) { d->block_bottom = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  uint32_t block_bottom() const { if (auto* d = direct()) return d->block_bottom; return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(address + 208))); }
  void set_block_bottom(uint32_t v) const { if (auto* d = direct()) { d->block_bottom = v; return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(address + 208, v); }
  uint32_t front_address() const { return address + 212; }
  uint32_t front_bits() const { if (auto* d = direct()) return d->front; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_front_bits(uint32_t v) const { if (auto* d = direct()) { d->front = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  uint32_t front() const { if (auto* d = direct()) return d->front; return uint32_t((memory::IsHandle(address + 212) ? uint32_t(memory::OwnerLoad(address + 212, 4)) : guest::LoadU32(address + 212))); }
  void set_front(uint32_t v) const { if (auto* d = direct()) { d->front = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 4, v); else guest::StoreU32(address + 212, v); }
  uint32_t back_address() const { return address + 216; }
  uint32_t back_bits() const { if (auto* d = direct()) return d->back; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_back_bits(uint32_t v) const { if (auto* d = direct()) { d->back = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  uint32_t back() const { if (auto* d = direct()) return d->back; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(address + 216))); }
  void set_back(uint32_t v) const { if (auto* d = direct()) { d->back = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(address + 216, v); }
  uint32_t block_columns_address() const { return address + 220; }
  uint32_t block_columns_bits() const { if (auto* d = direct()) return d->block_columns; return uint32_t((memory::IsHandle(address + 220) ? uint32_t(memory::OwnerLoad(address + 220, 4)) : guest::LoadU32(address + 220))); }
  void set_block_columns_bits(uint32_t v) const { if (auto* d = direct()) { d->block_columns = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 4, v); else guest::StoreU32(address + 220, v); }
  uint32_t block_columns() const { if (auto* d = direct()) return d->block_columns; return uint32_t((memory::IsHandle(address + 220) ? uint32_t(memory::OwnerLoad(address + 220, 4)) : guest::LoadU32(address + 220))); }
  void set_block_columns(uint32_t v) const { if (auto* d = direct()) { d->block_columns = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 4, v); else guest::StoreU32(address + 220, v); }
  uint32_t block_rows_address() const { return address + 224; }
  uint32_t block_rows_bits() const { if (auto* d = direct()) return d->block_rows; return uint32_t((memory::IsHandle(address + 224) ? uint32_t(memory::OwnerLoad(address + 224, 4)) : guest::LoadU32(address + 224))); }
  void set_block_rows_bits(uint32_t v) const { if (auto* d = direct()) { d->block_rows = v; return; } if (memory::IsHandle(address + 224)) memory::OwnerStore(address + 224, 4, v); else guest::StoreU32(address + 224, v); }
  uint32_t block_rows() const { if (auto* d = direct()) return d->block_rows; return uint32_t((memory::IsHandle(address + 224) ? uint32_t(memory::OwnerLoad(address + 224, 4)) : guest::LoadU32(address + 224))); }
  void set_block_rows(uint32_t v) const { if (auto* d = direct()) { d->block_rows = v; return; } if (memory::IsHandle(address + 224)) memory::OwnerStore(address + 224, 4, v); else guest::StoreU32(address + 224, v); }
  uint32_t slices_address() const { return address + 228; }
  uint32_t slices_bits() const { if (auto* d = direct()) return d->slices; return uint32_t((memory::IsHandle(address + 228) ? uint32_t(memory::OwnerLoad(address + 228, 4)) : guest::LoadU32(address + 228))); }
  void set_slices_bits(uint32_t v) const { if (auto* d = direct()) { d->slices = v; return; } if (memory::IsHandle(address + 228)) memory::OwnerStore(address + 228, 4, v); else guest::StoreU32(address + 228, v); }
  uint32_t slices() const { if (auto* d = direct()) return d->slices; return uint32_t((memory::IsHandle(address + 228) ? uint32_t(memory::OwnerLoad(address + 228, 4)) : guest::LoadU32(address + 228))); }
  void set_slices(uint32_t v) const { if (auto* d = direct()) { d->slices = v; return; } if (memory::IsHandle(address + 228)) memory::OwnerStore(address + 228, 4, v); else guest::StoreU32(address + 228, v); }
  // -1 from DxtCodec::DxtCodec
  uint32_t unknown_232_address() const { return address + 232; }
  uint32_t unknown_232_bits() const { if (auto* d = direct()) return d->unknown_232; return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(address + 232))); }
  void set_unknown_232_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_232 = v; return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(address + 232, v); }
  uint32_t unknown_232() const { if (auto* d = direct()) return d->unknown_232; return uint32_t((memory::IsHandle(address + 232) ? uint32_t(memory::OwnerLoad(address + 232, 4)) : guest::LoadU32(address + 232))); }
  void set_unknown_232(uint32_t v) const { if (auto* d = direct()) { d->unknown_232 = v; return; } if (memory::IsHandle(address + 232)) memory::OwnerStore(address + 232, 4, v); else guest::StoreU32(address + 232, v); }
  // -1 from DxtCodec::DxtCodec
  uint32_t unknown_236_address() const { return address + 236; }
  uint32_t unknown_236_bits() const { if (auto* d = direct()) return d->unknown_236; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_unknown_236_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_236 = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  uint32_t unknown_236() const { if (auto* d = direct()) return d->unknown_236; return uint32_t((memory::IsHandle(address + 236) ? uint32_t(memory::OwnerLoad(address + 236, 4)) : guest::LoadU32(address + 236))); }
  void set_unknown_236(uint32_t v) const { if (auto* d = direct()) { d->unknown_236 = v; return; } if (memory::IsHandle(address + 236)) memory::OwnerStore(address + 236, 4, v); else guest::StoreU32(address + 236, v); }
  // DxtCodec::DecodeRow's block row (operator new of block columns x 256 bytes: float4s, a TcFloat4 blob; never seen set), freed by ~DxtCodec
  uint32_t decode_cache_address() const { return address + 240; }
  uint32_t decode_cache_bits() const { if (auto* d = direct()) return d->decode_cache; return uint32_t((memory::IsHandle(address + 240) ? uint32_t(memory::OwnerLoad(address + 240, 4)) : guest::LoadU32(address + 240))); }
  void set_decode_cache_bits(uint32_t v) const { if (auto* d = direct()) { d->decode_cache = v; return; } if (memory::IsHandle(address + 240)) memory::OwnerStore(address + 240, 4, v); else guest::StoreU32(address + 240, v); }
  template <typename T = DxtBlockCacheEntry>
  T decode_cache() const { return T(decode_cache_bits()); }
  void set_decode_cache(const View& w) const { set_decode_cache_bits(w.address); }
  void set_decode_cache(uint32_t v) const { set_decode_cache_bits(v); }
  // block rows held
  uint32_t cache_count_address() const { return address + 244; }
  uint32_t cache_count_bits() const { if (auto* d = direct()) return d->cache_count; return uint32_t((memory::IsHandle(address + 244) ? uint32_t(memory::OwnerLoad(address + 244, 4)) : guest::LoadU32(address + 244))); }
  void set_cache_count_bits(uint32_t v) const { if (auto* d = direct()) { d->cache_count = v; return; } if (memory::IsHandle(address + 244)) memory::OwnerStore(address + 244, 4, v); else guest::StoreU32(address + 244, v); }
  uint32_t cache_count() const { if (auto* d = direct()) return d->cache_count; return uint32_t((memory::IsHandle(address + 244) ? uint32_t(memory::OwnerLoad(address + 244, 4)) : guest::LoadU32(address + 244))); }
  void set_cache_count(uint32_t v) const { if (auto* d = direct()) { d->cache_count = v; return; } if (memory::IsHandle(address + 244)) memory::OwnerStore(address + 244, 4, v); else guest::StoreU32(address + 244, v); }
  // EncodeRow's: an entry a block row (by slice and block row)
  uint32_t cache_address() const { return address + 248; }
  uint32_t cache_bits() const { if (auto* d = direct()) return d->cache; return uint32_t((memory::IsHandle(address + 248) ? uint32_t(memory::OwnerLoad(address + 248, 4)) : guest::LoadU32(address + 248))); }
  void set_cache_bits(uint32_t v) const { if (auto* d = direct()) { d->cache = v; return; } if (memory::IsHandle(address + 248)) memory::OwnerStore(address + 248, 4, v); else guest::StoreU32(address + 248, v); }
  template <typename T = DxtBlockCacheEntry>
  T cache() const { return T(cache_bits()); }
  void set_cache(const View& w) const { set_cache_bits(w.address); }
  void set_cache(uint32_t v) const { set_cache_bits(v); }
  // the description's dither
  uint32_t dither_address() const { return address + 252; }
  uint32_t dither_bits() const { if (auto* d = direct()) return d->dither; return uint32_t((memory::IsHandle(address + 252) ? uint32_t(memory::OwnerLoad(address + 252, 4)) : guest::LoadU32(address + 252))); }
  void set_dither_bits(uint32_t v) const { if (auto* d = direct()) { d->dither = v; return; } if (memory::IsHandle(address + 252)) memory::OwnerStore(address + 252, 4, v); else guest::StoreU32(address + 252, v); }
  uint32_t dither() const { if (auto* d = direct()) return d->dither; return uint32_t((memory::IsHandle(address + 252) ? uint32_t(memory::OwnerLoad(address + 252, 4)) : guest::LoadU32(address + 252))); }
  void set_dither(uint32_t v) const { if (auto* d = direct()) { d->dither = v; return; } if (memory::IsHandle(address + 252)) memory::OwnerStore(address + 252, 4, v); else guest::StoreU32(address + 252, v); }
};

// allocated; measure: new<4294967295>@DxtCodec::EncodeRow, new<4294967295>@DxtCodec::DecodeRow
struct DxtBlockCacheEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static DxtBlockCacheEntry New(uint32_t size) { return DxtBlockCacheEntry(memory::NewBlob(size, "DxtBlockCacheEntry")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  // a bit for each of the block row's four rows in
  uint32_t rows_in() const { return guest::LoadU32(address + 0); }
  void set_rows_in(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t rows_in_address() const { return address + 0; }
  // the block row's 16 float4s a block column (operator new)
  template <typename T = TcFloat4>
  T pixels() const { return T(guest::LoadU32(address + 4)); }
  void set_pixels(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_pixels(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t pixels_address() const { return address + 4; }
};

// allocated; measure: YuvCodec
struct YuvCodec : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  static constexpr uint32_t kVtable = 0x8203B9F8;
  template <typename T = ImageCodecVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // as ImageCodec's
  uint32_t format() const { return guest::LoadU32(address + 4); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t format_address() const { return address + 4; }
  uint32_t srgb() const { return guest::LoadU32(address + 16); }
  void set_srgb(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t srgb_address() const { return address + 16; }
  uint32_t color_key() const { return guest::LoadU32(address + 24); }
  void set_color_key(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t color_key_address() const { return address + 24; }
  template <typename T = ImageCodecData>
  T data() const { return T(guest::LoadU32(address + 32)); }
  void set_data(const View& v) const { guest::StoreU32(address + 32, v.address); }
  void set_data(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t data_address() const { return address + 32; }
  uint32_t box_left() const { return guest::LoadU32(address + 60); }
  void set_box_left(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t box_left_address() const { return address + 60; }
  uint32_t box_top() const { return guest::LoadU32(address + 64); }
  void set_box_top(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t box_top_address() const { return address + 64; }
  uint32_t box_front() const { return guest::LoadU32(address + 76); }
  void set_box_front(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t box_front_address() const { return address + 76; }
  uint32_t convert_from() const { return guest::LoadU32(address + 84); }
  void set_convert_from(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t convert_from_address() const { return address + 84; }
  uint32_t row_pitch() const { return guest::LoadU32(address + 96); }
  void set_row_pitch(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t row_pitch_address() const { return address + 96; }
  uint32_t slice_pitch() const { return guest::LoadU32(address + 100); }
  void set_slice_pitch(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t slice_pitch_address() const { return address + 100; }
  uint32_t width() const { return guest::LoadU32(address + 104); }
  void set_width(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t width_address() const { return address + 104; }
  // the pixel pairs' float4s (operator new)
  template <typename T = YuvCodecPixels>
  T buffer() const { return T(guest::LoadU32(address + 128)); }
  void set_buffer(const View& v) const { guest::StoreU32(address + 128, v.address); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 128, v); }
  uint32_t buffer_address() const { return address + 128; }
  // the box widened to even columns
  uint32_t even_left() const { return guest::LoadU32(address + 132); }
  void set_even_left(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t even_left_address() const { return address + 132; }
  uint32_t unknown_136() const { return guest::LoadU32(address + 136); }
  void set_unknown_136(uint32_t v) const { guest::StoreU32(address + 136, v); }
  uint32_t unknown_136_address() const { return address + 136; }
  uint32_t even_right() const { return guest::LoadU32(address + 140); }
  void set_even_right(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t even_right_address() const { return address + 140; }
  uint32_t unknown_144() const { return guest::LoadU32(address + 144); }
  void set_unknown_144(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  uint32_t unknown_148() const { return guest::LoadU32(address + 148); }
  void set_unknown_148(uint32_t v) const { guest::StoreU32(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  uint32_t unknown_152() const { return guest::LoadU32(address + 152); }
  void set_unknown_152(uint32_t v) const { guest::StoreU32(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  // the buffer's pixels
  uint32_t buffer_pixels() const { return guest::LoadU32(address + 156); }
  void set_buffer_pixels(uint32_t v) const { guest::StoreU32(address + 156, v); }
  uint32_t buffer_pixels_address() const { return address + 156; }
  uint32_t unknown_160() const { return guest::LoadU32(address + 160); }
  void set_unknown_160(uint32_t v) const { guest::StoreU32(address + 160, v); }
  uint32_t unknown_160_address() const { return address + 160; }
  // 0 without a buffer
  uint32_t unknown_164() const { return guest::LoadU32(address + 164); }
  void set_unknown_164(uint32_t v) const { guest::StoreU32(address + 164, v); }
  uint32_t unknown_164_address() const { return address + 164; }
  // 0 or 8 by the format (with +172: 0 and 8, or 8 and 0)
  uint32_t unknown_168() const { return guest::LoadU32(address + 168); }
  void set_unknown_168(uint32_t v) const { guest::StoreU32(address + 168, v); }
  uint32_t unknown_168_address() const { return address + 168; }
  uint32_t unknown_172() const { return guest::LoadU32(address + 172); }
  void set_unknown_172(uint32_t v) const { guest::StoreU32(address + 172, v); }
  uint32_t unknown_172_address() const { return address + 172; }
};

// allocated; measure: YuvCodec.+128
struct YuvCodecPixels : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  float pixel0_red() const { return guest::LoadFloat(address + 0); }
  void set_pixel0_red(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t pixel0_red_address() const { return address + 0; }
  float pixel0_green() const { return guest::LoadFloat(address + 4); }
  void set_pixel0_green(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t pixel0_green_address() const { return address + 4; }
  float pixel0_blue() const { return guest::LoadFloat(address + 8); }
  void set_pixel0_blue(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t pixel0_blue_address() const { return address + 8; }
  float pixel0_alpha() const { return guest::LoadFloat(address + 12); }
  void set_pixel0_alpha(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t pixel0_alpha_address() const { return address + 12; }
  float pixel1_red() const { return guest::LoadFloat(address + 16); }
  void set_pixel1_red(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t pixel1_red_address() const { return address + 16; }
  float pixel1_green() const { return guest::LoadFloat(address + 20); }
  void set_pixel1_green(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t pixel1_green_address() const { return address + 20; }
  float pixel1_blue() const { return guest::LoadFloat(address + 24); }
  void set_pixel1_blue(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t pixel1_blue_address() const { return address + 24; }
  float pixel1_alpha() const { return guest::LoadFloat(address + 28); }
  void set_pixel1_alpha(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t pixel1_alpha_address() const { return address + 28; }
  float pixel2_red() const { return guest::LoadFloat(address + 32); }
  void set_pixel2_red(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t pixel2_red_address() const { return address + 32; }
  float pixel2_green() const { return guest::LoadFloat(address + 36); }
  void set_pixel2_green(float v) const { guest::StoreFloat(address + 36, v); }
  uint32_t pixel2_green_address() const { return address + 36; }
  float pixel2_blue() const { return guest::LoadFloat(address + 40); }
  void set_pixel2_blue(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t pixel2_blue_address() const { return address + 40; }
  float pixel2_alpha() const { return guest::LoadFloat(address + 44); }
  void set_pixel2_alpha(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t pixel2_alpha_address() const { return address + 44; }
  float pixel3_red() const { return guest::LoadFloat(address + 48); }
  void set_pixel3_red(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t pixel3_red_address() const { return address + 48; }
  float pixel3_green() const { return guest::LoadFloat(address + 52); }
  void set_pixel3_green(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t pixel3_green_address() const { return address + 52; }
  float pixel3_blue() const { return guest::LoadFloat(address + 56); }
  void set_pixel3_blue(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t pixel3_blue_address() const { return address + 56; }
  float pixel3_alpha() const { return guest::LoadFloat(address + 60); }
  void set_pixel3_alpha(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t pixel3_alpha_address() const { return address + 60; }
};

// stack; measure: Blitter, Blitter::Init.ret
struct Blitter : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // not the blit object's: XGCompressSurface's word below it (the destination point's y when none is given), joined by the measure
  uint32_t frame_point_y() const { return guest::LoadU32(address + -4); }
  void set_frame_point_y(uint32_t v) const { guest::StoreU32(address + -4, v); }
  uint32_t frame_point_y_address() const { return address + -4; }
  // the source's codec
  template <typename T = ImageCodec>
  T source() const { return T(guest::LoadU32(address + 0)); }
  void set_source(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_source(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t source_address() const { return address + 0; }
  // the destination's codec
  template <typename T = ImageCodec>
  T destination() const { return T(guest::LoadU32(address + 4)); }
  void set_destination(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_destination(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t destination_address() const { return address + 4; }
  // the D3DX filter flags: low 16 bits 1 none, 2 point, 3 linear, 4 triangle, 5 box; 0x80000 dither, 0x100000 diffusion, 0x200000 sRGB in, 0x400000 sRGB out; bits 16-18 the mirror flags
  uint32_t filter() const { return guest::LoadU32(address + 8); }
  void set_filter(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t filter_address() const { return address + 8; }
  // the filter's low byte (its type)
  uint8_t filter_type() const { return guest::LoadU8(address + 11); }
  void set_filter_type(uint8_t v) const { guest::StoreU8(address + 11, v); }
  uint32_t filter_type_address() const { return address + 11; }
};

// stack; measure: Blitter::Init.r4, Blitter::Init.r5
struct ImageDescription : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  uint32_t data() const { return guest::LoadU32(address + 0); }
  void set_data(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t data_address() const { return address + 0; }
  // the codec's format (0x18280086 A8R8G8B8, 0x1A200054 DXT4/5, ...)
  uint32_t format() const { return guest::LoadU32(address + 4); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t format_address() const { return address + 4; }
  uint32_t row_pitch() const { return guest::LoadU32(address + 8); }
  void set_row_pitch(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t row_pitch_address() const { return address + 8; }
  uint32_t slice_pitch() const { return guest::LoadU32(address + 12); }
  void set_slice_pitch(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t slice_pitch_address() const { return address + 12; }
  // the surface: 0, 0, width, height, 0, depth
  uint32_t extent_left() const { return guest::LoadU32(address + 16); }
  void set_extent_left(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t extent_left_address() const { return address + 16; }
  uint32_t extent_top() const { return guest::LoadU32(address + 20); }
  void set_extent_top(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t extent_top_address() const { return address + 20; }
  uint32_t extent_right() const { return guest::LoadU32(address + 24); }
  void set_extent_right(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t extent_right_address() const { return address + 24; }
  uint32_t extent_bottom() const { return guest::LoadU32(address + 28); }
  void set_extent_bottom(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t extent_bottom_address() const { return address + 28; }
  uint32_t extent_front() const { return guest::LoadU32(address + 32); }
  void set_extent_front(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t extent_front_address() const { return address + 32; }
  uint32_t extent_back() const { return guest::LoadU32(address + 36); }
  void set_extent_back(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t extent_back_address() const { return address + 36; }
  // the box: left, top, right, bottom, front, back
  uint32_t box_left() const { return guest::LoadU32(address + 40); }
  void set_box_left(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t box_left_address() const { return address + 40; }
  uint32_t box_top() const { return guest::LoadU32(address + 44); }
  void set_box_top(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t box_top_address() const { return address + 44; }
  uint32_t box_right() const { return guest::LoadU32(address + 48); }
  void set_box_right(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t box_right_address() const { return address + 48; }
  uint32_t box_bottom() const { return guest::LoadU32(address + 52); }
  void set_box_bottom(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t box_bottom_address() const { return address + 52; }
  uint32_t box_front() const { return guest::LoadU32(address + 56); }
  void set_box_front(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t box_front_address() const { return address + 56; }
  uint32_t box_back() const { return guest::LoadU32(address + 60); }
  void set_box_back(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t box_back_address() const { return address + 60; }
  // Blitter::Init: the filter's 0x80000
  uint32_t dither() const { return guest::LoadU32(address + 64); }
  void set_dither(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t dither_address() const { return address + 64; }
  // Blitter::Init: the filter's 0x100000
  uint32_t diffusion() const { return guest::LoadU32(address + 68); }
  void set_diffusion(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t diffusion_address() const { return address + 68; }
  // Blitter::Init: the filter's 0x200000 (source) or 0x400000 (destination)
  uint32_t srgb() const { return guest::LoadU32(address + 72); }
  void set_srgb(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t srgb_address() const { return address + 72; }
  uint32_t color_key() const { return guest::LoadU32(address + 76); }
  void set_color_key(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t color_key_address() const { return address + 76; }
  // never read by ImageCodec::Create's paths
  uint32_t palette() const { return guest::LoadU32(address + 80); }
  void set_palette(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t palette_address() const { return address + 80; }
};

// stack; measure: XGGetBlockDimensions.r4, XGGetBlockDimensions.r5
struct TcBlockDimension : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// allocated; measure: Blitter::LinearWeights.ret, new<4294967295>@Blitter::LinearWeights
struct BlitLinearWeights : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static BlitLinearWeights New(uint32_t size) { return BlitLinearWeights(memory::NewBlob(size, "BlitLinearWeights")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  // probably
  uint32_t first_index() const { return guest::LoadU32(address + 0); }
  void set_first_index(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t first_index_address() const { return address + 0; }
  // probably
  float first_weight() const { return guest::LoadFloat(address + 4); }
  void set_first_weight(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t first_weight_address() const { return address + 4; }
  // probably
  uint32_t second_index() const { return guest::LoadU32(address + 8); }
  void set_second_index(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t second_index_address() const { return address + 8; }
  // probably
  float second_weight() const { return guest::LoadFloat(address + 12); }
  void set_second_weight(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t second_weight_address() const { return address + 12; }
};

// allocated; measure: Blitter::TriangleWeights.ret, new<?>@Blitter::TriangleWeights
struct BlitTriangleWeights : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static BlitTriangleWeights New(uint32_t size) { return BlitTriangleWeights(memory::NewBlob(size, "BlitTriangleWeights")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // probably a tap's source index
  uint32_t first_index() const { return guest::LoadU32(address + 8); }
  void set_first_index(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t first_index_address() const { return address + 8; }
  // probably its weight
  float first_weight() const { return guest::LoadFloat(address + 12); }
  void set_first_weight(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t first_weight_address() const { return address + 12; }
  // probably
  uint32_t second_index() const { return guest::LoadU32(address + 16); }
  void set_second_index(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t second_index_address() const { return address + 16; }
  // probably
  float second_weight() const { return guest::LoadFloat(address + 20); }
  void set_second_weight(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t second_weight_address() const { return address + 20; }
};

// file; measure: DxtColor, DxtColor::DecodeBlock.r4
struct DxtColorBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint16_t color0() const { return guest::LoadU16(address + 0); }
  void set_color0(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t color0_address() const { return address + 0; }
  uint16_t color1() const { return guest::LoadU16(address + 2); }
  void set_color1(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t color1_address() const { return address + 2; }
  uint32_t indices() const { return guest::LoadU32(address + 4); }
  void set_indices(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t indices_address() const { return address + 4; }
  // a float4 pixel's (the measure's join)
  float red() const { return guest::LoadFloat(address + 0); }
  void set_red(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t red_address() const { return address + 0; }
  float green() const { return guest::LoadFloat(address + 4); }
  void set_green(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t green_address() const { return address + 4; }
  float blue() const { return guest::LoadFloat(address + 8); }
  void set_blue(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t blue_address() const { return address + 8; }
  float alpha() const { return guest::LoadFloat(address + 12); }
  void set_alpha(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t alpha_address() const { return address + 12; }
};

// file; measure: DxtAlpha, DxtAlpha::DecodeBlock.r4
struct DxtAlphaBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  uint8_t alpha0() const { return guest::LoadU8(address + 0); }
  void set_alpha0(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t alpha0_address() const { return address + 0; }
  // both ends
  uint16_t ends() const { return guest::LoadU16(address + 0); }
  void set_ends(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t ends_address() const { return address + 0; }
  uint8_t alpha1() const { return guest::LoadU8(address + 1); }
  void set_alpha1(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t alpha1_address() const { return address + 1; }
  // the 16 three-bit indices
  uint32_t index_bytes_address() const { return address + 2; }
  uint16_t indices_2() const { return guest::LoadU16(address + 2); }
  void set_indices_2(uint16_t v) const { guest::StoreU16(address + 2, v); }
  uint32_t indices_2_address() const { return address + 2; }
  uint16_t indices_4() const { return guest::LoadU16(address + 4); }
  void set_indices_4(uint16_t v) const { guest::StoreU16(address + 4, v); }
  uint32_t indices_4_address() const { return address + 4; }
  uint16_t indices_6() const { return guest::LoadU16(address + 6); }
  void set_indices_6(uint16_t v) const { guest::StoreU16(address + 6, v); }
  uint32_t indices_6_address() const { return address + 6; }
  // the pixels' (the measure's join)
  float pixel0_red() const { return guest::LoadFloat(address + 0); }
  void set_pixel0_red(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t pixel0_red_address() const { return address + 0; }
  float pixel0_alpha() const { return guest::LoadFloat(address + 12); }
  void set_pixel0_alpha(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t pixel0_alpha_address() const { return address + 12; }
  float pixel8_alpha() const { return guest::LoadFloat(address + 140); }
  void set_pixel8_alpha(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t pixel8_alpha_address() const { return address + 140; }
};

// file; measure: Dxt3A, Dxt3A::DecodeBlock.r4
struct Dxt3ABlock : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  // pixels 0-7's four bits each (probably)
  uint32_t alphas_low() const { return guest::LoadU32(address + 0); }
  void set_alphas_low(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t alphas_low_address() const { return address + 0; }
  // pixels 8-15's (probably)
  uint32_t alphas_high() const { return guest::LoadU32(address + 4); }
  void set_alphas_high(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t alphas_high_address() const { return address + 4; }
  // the pixels' (the measure's join)
  float pixel0_alpha() const { return guest::LoadFloat(address + 12); }
  void set_pixel0_alpha(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t pixel0_alpha_address() const { return address + 12; }
  float pixel8_alpha() const { return guest::LoadFloat(address + 140); }
  void set_pixel8_alpha(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t pixel8_alpha_address() const { return address + 140; }
};

// allocated; measure: -
struct BlitTriangleArray : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static BlitTriangleArray New(uint32_t size) { return BlitTriangleArray(memory::NewBlob(size, "BlitTriangleArray")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// The image words docs/functions/texture-compression.md's code uses.
struct TextureCompressionGlobals {
  // 1.0
  static float one() { return guest::LoadFloat(0x82000A30u); }
  static void set_one(float v) { guest::StoreFloat(0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero() { return guest::LoadFloat(0x82000A34u); }
  static void set_zero(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // probably 255.0
  static float scale_255() { return guest::LoadFloat(0x82000C44u); }
  static void set_scale_255(float v) { guest::StoreFloat(0x82000C44u, v); }
  static constexpr uint32_t scale_255_address() { return 0x82000C44u; }
  // -1.0: DxtColor::FitEndpoints' corrections' factor
  static float minus_one() { return guest::LoadFloat(0x82001BE0u); }
  static void set_minus_one(float v) { guest::StoreFloat(0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // probably 0.5
  static float half() { return guest::LoadFloat(0x82001BE4u); }
  static void set_half(float v) { guest::StoreFloat(0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // probably 1/255
  static float inverse_255() { return guest::LoadFloat(0x82002200u); }
  static void set_inverse_255(float v) { guest::StoreFloat(0x82002200u, v); }
  static constexpr uint32_t inverse_255_address() { return 0x82002200u; }
  // probably 1/5 (DxtAlpha::EncodeBlock's 6 steps)
  static float inverse_5() { return guest::LoadFloat(0x82002208u); }
  static void set_inverse_5(float v) { guest::StoreFloat(0x82002208u, v); }
  static constexpr uint32_t inverse_5_address() { return 0x82002208u; }
  // probably 1/3 (DxtColor::EncodeBlock's palette)
  static float third() { return guest::LoadFloat(0x82002A38u); }
  static void set_third(float v) { guest::StoreFloat(0x82002A38u, v); }
  static constexpr uint32_t third_address() { return 0x82002A38u; }
  // 0.125: DxtColor::FitEndpoints' weights' factor
  static float eighth() { return guest::LoadFloat(0x82002A88u); }
  static void set_eighth(float v) { guest::StoreFloat(0x82002A88u, v); }
  static constexpr uint32_t eighth_address() { return 0x82002A88u; }
  // DxtAlpha::FitEndpoints' smallest range
  static float alpha_range_min() { return guest::LoadFloat(0x82008058u); }
  static void set_alpha_range_min(float v) { guest::StoreFloat(0x82008058u, v); }
  static constexpr uint32_t alpha_range_min_address() { return 0x82008058u; }
  // the error diffusion's (probably 1/16)
  static float weight_below_right() { return guest::LoadFloat(0x820093A0u); }
  static void set_weight_below_right(float v) { guest::StoreFloat(0x820093A0u, v); }
  static constexpr uint32_t weight_below_right_address() { return 0x820093A0u; }
  // DxtAlpha::FitEndpoints' squared step to stop
  static float alpha_gradient_min() { return guest::LoadFloat(0x8201607Cu); }
  static void set_alpha_gradient_min(float v) { guest::StoreFloat(0x8201607Cu, v); }
  static constexpr uint32_t alpha_gradient_min_address() { return 0x8201607Cu; }
  // FLT_MIN: DxtColor::FitEndpoints' smallest squared extent
  static float extent_min() { return guest::LoadFloat(0x82018FE0u); }
  static void set_extent_min(float v) { guest::StoreFloat(0x82018FE0u, v); }
  static constexpr uint32_t extent_min_address() { return 0x82018FE0u; }
  // probably 2/3 (DxtColor::EncodeBlock's palette)
  static float two_thirds() { return guest::LoadFloat(0x82038450u); }
  static void set_two_thirds(float v) { guest::StoreFloat(0x82038450u, v); }
  static constexpr uint32_t two_thirds_address() { return 0x82038450u; }
  // the error diffusion's (probably 3/16)
  static float weight_below_left() { return guest::LoadFloat(0x8203B78Cu); }
  static void set_weight_below_left(float v) { guest::StoreFloat(0x8203B78Cu, v); }
  static constexpr uint32_t weight_below_left_address() { return 0x8203B78Cu; }
  // the error diffusion's (probably 5/16)
  static float weight_below() { return guest::LoadFloat(0x8203B790u); }
  static void set_weight_below(float v) { guest::StoreFloat(0x8203B790u, v); }
  static constexpr uint32_t weight_below_address() { return 0x8203B790u; }
  // probably 31 (PackR5G6B5's red and blue)
  static float scale_5() { return guest::LoadFloat(0x8203B7A0u); }
  static void set_scale_5(float v) { guest::StoreFloat(0x8203B7A0u, v); }
  static constexpr uint32_t scale_5_address() { return 0x8203B7A0u; }
  // probably 63 (green)
  static float scale_6() { return guest::LoadFloat(0x8203B7A4u); }
  static void set_scale_6(float v) { guest::StoreFloat(0x8203B7A4u, v); }
  static constexpr uint32_t scale_6_address() { return 0x8203B7A4u; }
  // probably 1/31
  static float step_5() { return guest::LoadFloat(0x8203B7A8u); }
  static void set_step_5(float v) { guest::StoreFloat(0x8203B7A8u, v); }
  static constexpr uint32_t step_5_address() { return 0x8203B7A8u; }
  // probably 1/63
  static float step_6() { return guest::LoadFloat(0x8203B7ACu); }
  static void set_step_6(float v) { guest::StoreFloat(0x8203B7ACu, v); }
  static constexpr uint32_t step_6_address() { return 0x8203B7ACu; }
  // probably 1/7 (DxtAlpha::EncodeBlock's 8 steps)
  static float inverse_7() { return guest::LoadFloat(0x8203B7B0u); }
  static void set_inverse_7(float v) { guest::StoreFloat(0x8203B7B0u, v); }
  static constexpr uint32_t inverse_7_address() { return 0x8203B7B0u; }
  // DxtAlpha::FitEndpoints' high end's weights for 8 steps (floats)
  static constexpr uint32_t alpha_high_weights_8_address() { return 0x8203EA38u; }
  // the low end's for 8 steps
  static constexpr uint32_t alpha_low_weights_8_address() { return 0x8203EA58u; }
  // the high end's for 6 steps
  static constexpr uint32_t alpha_high_weights_6_address() { return 0x8203EA78u; }
  // the low end's for 6 steps
  static constexpr uint32_t alpha_low_weights_6_address() { return 0x8203EA90u; }
  // 2^-16: DxtColor::FitEndpoints' squared corrections to stop at
  static float color_stop_min() { return guest::LoadFloat(0x8203EAA8u); }
  static void set_color_stop_min(float v) { guest::StoreFloat(0x8203EAA8u, v); }
  static constexpr uint32_t color_stop_min_address() { return 0x8203EAA8u; }
  // 2^-12: DxtColor::FitEndpoints' smallest squared length
  static float color_length_min() { return guest::LoadFloat(0x8203EAACu); }
  static void set_color_length_min(float v) { guest::StoreFloat(0x8203EAACu, v); }
  static constexpr uint32_t color_length_min_address() { return 0x8203EAACu; }
  // DxtColor::FitEndpoints' high end's weights for 4 colors (floats)
  static constexpr uint32_t color_high_weights_4_address() { return 0x8203EAB0u; }
  // the low end's for 4 colors
  static constexpr uint32_t color_low_weights_4_address() { return 0x8203EAC0u; }
  // the high end's for 3 colors
  static constexpr uint32_t color_high_weights_3_address() { return 0x8203EAD0u; }
  // the low end's for 3 colors
  static constexpr uint32_t color_low_weights_3_address() { return 0x8203EADCu; }
  // DxtColor::EncodeBlock's step to palette index (words), 4 colors
  static constexpr uint32_t color_indices_4_address() { return 0x8203EAE8u; }
  // 3 colors
  static constexpr uint32_t color_indices_3_address() { return 0x8203EAF8u; }
  // the error diffusion's (probably 7/16)
  static float weight_right() { return guest::LoadFloat(0x8203EB04u); }
  static void set_weight_right(float v) { guest::StoreFloat(0x8203EB04u, v); }
  static constexpr uint32_t weight_right_address() { return 0x8203EB04u; }
  // DxtAlpha::EncodeBlock's step to index (words), 8 steps
  static constexpr uint32_t alpha_indices_8_address() { return 0x8203EB08u; }
  // 6 steps (with 0 and 1)
  static constexpr uint32_t alpha_indices_6_address() { return 0x8203EB28u; }
  // XGCompressSurface's f1, stored each call; DXT1's 1-bit alpha below it
  static float alpha_reference() { return guest::LoadFloat(0x8263B7E8u); }
  static void set_alpha_reference(float v) { guest::StoreFloat(0x8263B7E8u, v); }
  static constexpr uint32_t alpha_reference_address() { return 0x8263B7E8u; }
  // 32 floats of 0.5 (a codec's +52 without dither)
  static constexpr uint32_t rounding_table_address() { return 0x8263C768u; }
  // 32 floats of a 4 x 4 ordered dither's offsets (with dither)
  static constexpr uint32_t dither_table_address() { return 0x8263C7E8u; }
  // in .data: the color weights the color encoder scales pixels by (0.2970, 1.0, 0.1008 in play)
  static constexpr uint32_t color_weights_address() { return 0x82649B10u; }
  // their inverses
  static constexpr uint32_t inverse_color_weights_address() { return 0x82649B20u; }
  // XGCompressSurface's flags & 1, stored each call: the DXT2/3 and 4/5 block functions premultiply alpha
  static uint32_t premultiplied() { return guest::LoadU32(0x82954660u); }
  static void set_premultiplied(uint32_t v) { guest::StoreU32(0x82954660u, v); }
  static constexpr uint32_t premultiplied_address() { return 0x82954660u; }
};

}  // namespace schema
