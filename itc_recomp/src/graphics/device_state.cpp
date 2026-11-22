// The graphics device's state setters (docs/functions/device-state.md): see
// device_state.h. Every write matches the original's, word for word (the
// register shadow's fields are put in with the original's rotate-and-mask,
// so a value wider than its field spills the same way); the originals' stack
// temporaries (the float conversions' words below r1) aren't written.

#include "graphics/device_state.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "display/gamma.h"
#include "schema/app_start_up.h"
#include "schema/device_state.h"
#include "schema/graphics_resources.h"

#include <emmintrin.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace graphics::device_state {

namespace {

// The device (itc_recomp/schema/device-state.toml: the register shadow's
// words, the software copies, the bound objects) and the image's words the
// setters read.
using schema::Device;
using Globals = schema::DeviceStateGlobals;

uint32_t Rotl(uint32_t x, int n) { return std::rotl(x, n); }

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
float LoadF(uint8_t* base, uint32_t address) { return guest::LoadFloat(base, address); }

// A dirty mask ORed with `bits`.
void Mark(uint8_t* base, uint32_t address, uint64_t bits) {
  guest::StoreU64(base, address, guest::LoadU64(base, address) | bits);
}
void MarkShader(const Device& d, uint64_t bits) { d.set_shader_dirty(d.shader_dirty() | bits); }
void MarkRender(const Device& d, uint64_t bits) { d.set_render_dirty(d.render_dirty() | bits); }
void MarkOther(const Device& d, uint64_t bits) { d.set_other_dirty(d.other_dirty() | bits); }

// rlwimi: the word at `address` with `mask`'s bits from `value` rotated
// left by `shift`.
void Insert(uint8_t* base, uint32_t address, uint32_t value, int shift, uint32_t mask) {
  Store(base, address, (Rotl(value, shift) & mask) | (Load(base, address) & ~mask));
}

// fctiwz and fctidz as the recompiled code does them.
int32_t ToInt32(double d) {
  if (std::isnan(d)) {
    return INT32_MIN;
  }
  if (d >= double(INT_MAX)) {
    return INT_MAX;
  }
  return _mm_cvttsd_si32(_mm_set_sd(d));
}
int64_t ToInt64(double d) {
  if (std::isnan(d)) {
    return INT64_MIN;
  }
  if (d > double(LLONG_MAX)) {
    return LLONG_MAX;
  }
  return _mm_cvttsd_si64(_mm_set_sd(d));
}

float Bits(uint32_t value) { return std::bit_cast<float>(value); }

// The blend control with separate alpha off: the color half copied into
// the alpha half (the original's bit steps, kept as they are).
uint32_t AlphaFromColor(uint32_t blend) {
  const uint32_t color = blend & 0xFFFF;
  uint32_t alpha = (color & 0x1010) | (Rotl(color, 4) & 0xFFFFFFF0);
  alpha = Rotl(alpha, 12) & 0xFFFF0000;
  alpha &= ~0x00100000u;
  alpha &= ~0x10000000u;
  return alpha | color;
}

// RB_BLENDCONTROL0-3 = `blend`; +16 0x407.
void StoreBlendControls(uint8_t* base, uint32_t device, uint32_t blend) {
  const Device d(base, device);
  d.set_rb_blendcontrol0(blend);
  d.set_rb_blendcontrol1(blend);
  d.set_rb_blendcontrol2(blend);
  d.set_rb_blendcontrol3(blend);
  MarkShader(d, 0x407);
}

// SrcBlend, DestBlend and BlendOp: their bits in the software copy, then the
// four registers while blending is on.
void SetColorBlendField(uint8_t* base, uint32_t device, uint32_t value, int shift,
                        uint32_t mask) {
  const Device d(base, device);
  const uint32_t flags = d.blend_flags();
  Insert(base, d.blend_control_address(), value, shift, mask);
  if (!(flags & 0x80000000u)) {
    return;
  }
  uint32_t blend = d.blend_control();
  if (!(flags & 0x40000000u)) {
    blend = AlphaFromColor(blend);
  }
  StoreBlendControls(base, device, blend);
}

// SrcBlendAlpha, DestBlendAlpha and BlendOpAlpha: the registers only while
// blending and separate alpha are both on.
void SetAlphaBlendField(uint8_t* base, uint32_t device, uint32_t value, int shift,
                        uint32_t mask) {
  const Device d(base, device);
  const uint32_t flags = d.blend_flags();
  Insert(base, d.blend_control_address(), value, shift, mask);
  if (!(flags & 0x80000000u) || !(flags & 0x40000000u)) {
    return;
  }
  StoreBlendControls(base, device, d.blend_control());
}

// A field of RB_DEPTHCONTROL; +16 0x800, and bit 49 with `bit49`.
void SetDepthControl(uint8_t* base, uint32_t device, uint32_t value, int shift, uint32_t mask,
                     bool bit49) {
  const Device d(base, device);
  Insert(base, d.rb_depthcontrol_address(), value, shift, mask);
  MarkShader(d, bit49 ? (0x800 | (1ull << 49)) : 0x800);
}

// The wraps but 7 and 15: the field cleared and the value ORed in shifted
// (not masked: a wider value spills into the next wraps, as in the original).
// `word` is SQ_WRAPPING_0's or _1's address.
void SetWrap(uint8_t* base, uint32_t device, uint32_t word, uint32_t value, int shift) {
  const uint32_t field = 0xFu << shift;
  Store(base, word, (Load(base, word) & ~field) | (value << shift));
  MarkShader(Device(base, device), 1ull << 45);
}

// A render target's color write mask: its software copy, and its bits in
// RB_COLOR_MASK (0 without the target bound).
void SetColorWriteMask(uint8_t* base, uint32_t device, uint32_t value, uint32_t target) {
  const Device d(base, device);
  const uint32_t surface = Load(base, d.render_target0_address() + 4 * target);
  Store(base, d.color_write_enable0_address() + 4 * target, value);
  const uint32_t bits = surface != 0 ? value : 0;
  Insert(base, d.rb_color_mask_address(), bits, 4 * target, 0xFu << (4 * target));
  MarkRender(d, 0x40000);
}

// A render target's high-precision blend: its software copy, and the bound
// surface's color format switched between its two forms (2 and 10, 3 and
// 12) when it doesn't match, in the surface and RB_COLOR_INFO's shadow (at
// `color_info`).
void SetHighPrecisionBlend(uint8_t* base, uint32_t device, uint32_t value, uint32_t target,
                           uint32_t color_info, uint64_t dirty) {
  const Device d(base, device);
  const uint32_t surface = Load(base, d.render_target0_address() + 4 * target);
  Store(base, d.high_precision_blend0_address() + 4 * target, value);
  if (surface == 0) {
    return;
  }
  const schema::DeviceSurface s(base, surface);
  const uint32_t info = s.format_info();
  const uint32_t format = (info >> 16) & 0xF;
  if (format != 2 && format != 3 && format != 10 && format != 12) {
    return;
  }
  if ((((info >> 19) & 1) ^ value) == 0) {
    return;
  }
  const uint32_t now = s.format_info();
  const uint32_t f = (now >> 16) & 0xF;
  const uint32_t m = value - 1;
  const uint32_t switched = (((f >> 1) - 3) & m) | (((f + 3) << 1) & ~m);
  const uint32_t field = (switched & 0xF) << 16;
  s.set_format_info((now & ~0xF0000u) | field);
  Store(base, color_info, (Load(base, color_info) & ~0xF0000u) | field);
  MarkRender(d, dirty);
}

// The depth bias enables: PA_SU_SC_MODE_CNTL bits 11 and 12 (front and
// back) set when the face's scale or offset isn't 0.
void SetPolygonOffsetEnables(uint8_t* base, uint32_t device, bool front, bool back) {
  const Device d(base, device);
  Insert(base, d.pa_su_sc_mode_cntl_address(), front ? 1 : 0, 11, 0x800);
  Insert(base, d.pa_su_sc_mode_cntl_address(), back ? 1 : 0, 12, 0x1000);
}

// A point size times `scale` as an integer (fctiwz), its low 16 bits.
uint16_t PointSizeBits(uint8_t* base, float size, uint32_t scale) {
  return uint16_t(ToInt32(double(size * LoadF(base, scale))) & 0xFFFF);
}

// A sampler's fetch constant and its dirty bit (+16 bit 43 - sampler; 0
// past the mask, as the original's shift).
uint32_t FetchConstant(const Device& d, uint32_t sampler) {
  return d.fetch_constants_address() + 24 * sampler;
}
uint64_t SamplerBit(uint32_t sampler) {
  const uint32_t amount = (sampler + 20) & 0x7F;
  return (amount & 0x40) ? 0 : (0x8000000000000000ull >> amount);
}

// A field of one of the sampler's fetch constant words, then its bit.
void SetSamplerField(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t word,
                     uint32_t value, int shift, uint32_t mask) {
  const Device d(base, device);
  Insert(base, FetchConstant(d, sampler) + word, value, shift, mask);
  MarkShader(d, SamplerBit(sampler));
}

// Word 4's low two bits from word 3's filters and the z filter bits `z`
// (the original's steps, kept as they are).
uint32_t ZFilterBits(uint32_t word3, uint32_t z) {
  uint32_t bits = (Rotl(word3, 31) & 0x7FFFF) | (word3 & 0xFFF80000u);
  bits = (Rotl(word3, 31) & 0x7FF00000u) | (bits & 0x800FFFFFu);
  const uint32_t filters = Rotl(bits, 13) & 0xFFF;
  const uint32_t m = (z >> 2) - 1;
  return (filters & m) + (z & ~m);
}

// MagFilter and MinFilter: the filter in word 3, its anisotropic bit in word
// 4, the anisotropic filter from the sampler's max anisotropy, and word 4's
// low bits from the z filters.
void SetFilter(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value, bool mag) {
  const Device d(base, device);
  const uint32_t fc = FetchConstant(d, sampler);
  const uint32_t aniso = guest::LoadU8(base, d.max_anisotropy_address() + sampler);
  uint32_t word3 = Load(base, fc + 12);
  const uint32_t entry = Load(base, Globals::anisotropy_table_address() + 4 * aniso);
  uint32_t word4 = Load(base, fc + 16);
  const uint32_t anisotropic = value >> 2;
  const uint32_t other = (word4 >> (mag ? 11 : 10)) & 1;  // the other filter's bit
  const uint32_t own = mag ? 0x400 : 0x800;
  word4 = (Rotl(anisotropic, mag ? 10 : 11) & own) | (word4 & ~own);
  const uint32_t m = (other | anisotropic) - 1;
  Store(base, fc + 16, word4);
  const uint32_t bits = (mag ? (Rotl(entry & ~m, 6) & 0xFFFFFFC0u)
                             : (Rotl(entry & ~m, 4) & 0xFFFFFFF0u)) |
                        anisotropic | value;
  const int shift = mag ? 19 : 21;
  const uint32_t field = mag ? 0x180000 : 0x600000;
  word3 = (Rotl(bits, shift) & field) | (word3 & ~field);
  word3 = (Rotl(bits, shift) & 0xE000000) | (word3 & ~0xE000000u);
  Store(base, fc + 12, word3);
  const uint32_t z = guest::LoadU8(base, d.z_filters_address() + sampler);
  Store(base, fc + 16, (word4 & 0xFFFFFFFCu) | (ZFilterBits(word3, z) & 3));
  MarkShader(d, SamplerBit(sampler));
}

// MagFilterZ, MinFilterZ and SeparateZFilterEnable: their bit of the z
// filters (`bit` cleared, the value ORed in shifted, not masked), then word
// 4's low bits worked out again.
void SetZFilter(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value, int shift) {
  const Device d(base, device);
  const uint32_t fc = FetchConstant(d, sampler);
  const uint32_t word4 = Load(base, fc + 16);
  const uint32_t old = guest::LoadU8(base, d.z_filters_address() + sampler);
  const uint32_t z = (old & ~(1u << shift)) | (value << shift);
  const uint32_t word3 = Load(base, fc + 12);
  Store(base, fc + 16, (word4 & 0xFFFFFFFCu) | (ZFilterBits(word3, z) & 3));
  guest::StoreU8(base, d.z_filters_address() + sampler, uint8_t(z));
  MarkShader(d, SamplerBit(sampler));
}

// MaxMipLevel and MinMipLevel: the level kept, and with a texture bound the
// larger (smaller) of it and the texture's own in word 4.
void SetMipLevel(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value, bool max) {
  const Device d(base, device);
  const uint32_t texture = Load(base, d.textures_address() + 4 * sampler);
  if (texture != 0) {
    const uint32_t own = schema::DeviceTexture(base, texture).mip_levels();
    uint32_t level = max ? (own >> 2) & 0xF : (own >> 6) & 0xF;
    if (max ? !(level > value) : !(level < value)) {
      level = value;
    }
    Insert(base, FetchConstant(d, sampler) + 16, level, max ? 2 : 6,
           max ? 0x3Cu : 0x3C0u);
    MarkShader(d, SamplerBit(sampler));
  }
  guest::StoreU8(base, (max ? d.max_mip_level_address() : d.min_mip_level_address()) + sampler,
                 uint8_t(value));
}

// The fences of a replaced resource: with a command position (+10780), the
// resource's +8 = it; else, when the resource's flags (+0) meet the mask
// at +10784, an entry on the fence list (+13168; ends at +13172): the
// resource's address / 4 in bits 0-29, bit 30 clear, then -1. The
// original's bit 31 comes from an uninitialized stack word; 0 here (the
// list is in the command buffer, read by nothing since the emulated GPU
// went). The resource (a buffer, a texture or a surface) is
// graphics-resources' ResourceHeader: its common word (+0) and fence (+8).

// Whether fencing `resource` would need the full list flushed.
bool FenceNeedsFlush(uint8_t* base, uint32_t device, uint32_t resource) {
  const Device d(base, device);
  return resource != 0 && d.fence() == 0 &&
         (d.fence_mask() & schema::ResourceHeader(base, resource).common()) != 0 &&
         !(d.fence_list() < d.fence_list_end());
}

void Fence(uint8_t* base, uint32_t device, uint32_t resource) {
  if (resource == 0) {
    return;
  }
  const Device d(base, device);
  const schema::ResourceHeader r(base, resource);
  const uint32_t position = d.fence();
  if (position != 0) {
    r.set_fence(position);
    return;
  }
  if ((d.fence_mask() & r.common()) == 0) {
    return;
  }
  const uint32_t list = d.fence_list();
  const schema::DeviceFenceEntry entry(base, list);
  entry.set_resource((resource >> 2) & 0x3FFFFFFF);
  entry.set_marker(0xFFFFFFFFu);
  d.set_fence_list(list + 8);
}

// A guest address as the GPU's physical one: its low 29 bits, plus 0x1000
// for the views from 0xE0000000 (their 4 KB offset).
uint32_t Physical(uint32_t address) {
  return ((((address >> 20) & 0xFFF) + 512) & 0x1000) + (address & 0x1FFFFFFF);
}

// The float constants' copy: `count` 16-byte vectors from `data` (any
// alignment) to `to` + 16 x i, each stored at its 16-byte aligned address
// as stvx does; four loaded before four are stored, as the original's
// unrolled loop, then one at a time.
void CopyConstants(uint8_t* base, uint32_t to, uint32_t data, uint32_t count) {
  auto load = [&](uint32_t from, uint8_t* out) {
    // lvlx and lvrx: the bytes up to the next 16-byte line, then the rest.
    const uint32_t first = 16 - (from & 0xF);
    std::memcpy(out, guest::Pointer(base, from), first);
    if (first < 16) {
      std::memcpy(out + first, guest::Pointer(base, from + first), 16 - first);
    }
  };
  auto store = [&](uint32_t at, const uint8_t* in) {
    std::memcpy(guest::Pointer(base, at & ~0xFu), in, 16);
  };
  uint32_t left = count;
  if (count > 3) {
    for (uint32_t round = ((count - 4) >> 2) + 1; round != 0; round--) {
      uint8_t vectors[4][16];
      for (int i = 0; i < 4; i++) {
        load(data + 16 * i, vectors[i]);
      }
      for (int i = 0; i < 4; i++) {
        store(to + 16 * i, vectors[i]);
      }
      data += 64;
      to += 64;
      left -= 4;
    }
  }
  for (; left != 0; left--) {
    uint8_t vector[16];
    load(data, vector);
    store(to, vector);
    data += 16;
    to += 16;
  }
}

}  // namespace

void SetZEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A18D8 Device::SetRenderState_ZEnable");
  const Device d(base, device);
  const uint32_t depth = d.depth_surface().address;
  d.set_z_enable(value);
  SetDepthControl(base, device, depth != 0 ? value : 0, 1, 0x2, true);
}

void SetZFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1950 Device::SetRenderState_ZFunc");
  SetDepthControl(base, device, value, 4, 0x70, true);
}

void SetZWriteEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1920 Device::SetRenderState_ZWriteEnable");
  SetDepthControl(base, device, value, 2, 0x4, false);
}

void SetFillMode(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1290 Device::SetRenderState_FillMode");
  const Device d(base, device);
  Insert(base, d.pa_su_sc_mode_cntl_address(), value, 3, 0x7F8);
  MarkShader(d, 0x40);
}

void SetCullMode(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1260 Device::SetRenderState_CullMode");
  const Device d(base, device);
  Insert(base, d.pa_su_sc_mode_cntl_address(), value, 0, 0x7);
  MarkShader(d, 0x40);
}

void SetAlphaBlendEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1300 Device::SetRenderState_AlphaBlendEnable");
  const Device d(base, device);
  Insert(base, d.blend_flags_address(), value, 31, 0x80000000u);
  const uint32_t flags = d.blend_flags();
  uint32_t blend = d.blend_control();
  if (!(flags & 0x40000000u)) {
    blend = AlphaFromColor(blend);
  }
  if (value == 0) {
    blend = 0x00010001;  // ONE, ZERO
  }
  StoreBlendControls(base, device, blend);
}

void SetSeparateAlphaBlendEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1690 Device::SetRenderState_SeparateAlphaBlendEnable");
  const Device d(base, device);
  Insert(base, d.blend_flags_address(), value, 30, 0x40000000u);
  const bool blending = (d.blend_flags() & 0x80000000u) != 0;
  uint32_t blend = d.blend_control();
  if (value == 0) {
    blend = AlphaFromColor(blend);
  }
  if (!blending) {
    blend = 0x00010001;
  }
  StoreBlendControls(base, device, blend);
}

void SetBlendFactor(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A17C0 Device::SetRenderState_BlendFactor");
  const Device d(base, device);
  const float k = Globals::one_over_255(base);
  d.set_rb_blend_green(float((value >> 8) & 0xFF) * k);
  d.set_rb_blend_red(float((value >> 16) & 0xFF) * k);
  d.set_rb_blend_blue(float(value & 0xFF) * k);
  d.set_rb_blend_alpha(float((value >> 24) & 0xFF) * k);
  MarkRender(d, 0x3C000);
}

void SetSrcBlend(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1420 Device::SetRenderState_SrcBlend");
  SetColorBlendField(base, device, value, 0, 0x1F);
}

void SetDestBlend(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A14B0 Device::SetRenderState_DestBlend");
  SetColorBlendField(base, device, value, 8, 0x1F00);
}

void SetBlendOp(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1390 Device::SetRenderState_BlendOp");
  SetColorBlendField(base, device, value, 5, 0xE0);
}

void SetSrcBlendAlpha(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A15B0 Device::SetRenderState_SrcBlendAlpha");
  SetAlphaBlendField(base, device, value, 16, 0x1F0000);
}

void SetDestBlendAlpha(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1620 Device::SetRenderState_DestBlendAlpha");
  SetAlphaBlendField(base, device, value, 24, 0x1F000000);
}

void SetBlendOpAlpha(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1540 Device::SetRenderState_BlendOpAlpha");
  SetAlphaBlendField(base, device, value, 21, 0xE00000);
}

void SetAlphaTestEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A12C0 Device::SetRenderState_AlphaTestEnable");
  const Device d(base, device);
  Insert(base, d.rb_colorcontrol_address(), value, 3, 0x8);
  MarkShader(d, 0x200 | (1ull << 50));
}

void SetAlphaRef(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1728 Device::SetRenderState_AlphaRef");
  const Device d(base, device);
  d.set_rb_alpha_ref(float(double(value)) * Globals::one_over_255(base));
  MarkRender(d, 0x100);
}

void SetAlphaFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1790 Device::SetRenderState_AlphaFunc");
  const Device d(base, device);
  Insert(base, d.rb_colorcontrol_address(), value, 0, 0x7);
  MarkShader(d, 0x200);
}

void SetStencilEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1990 Device::SetRenderState_StencilEnable");
  const Device d(base, device);
  const uint32_t depth = d.depth_surface().address;
  d.set_stencil_enable(value);
  SetDepthControl(base, device, depth != 0 ? value : 0, 0, 0x1, true);
}

void SetTwoSidedStencilMode(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A19D8 Device::SetRenderState_TwoSidedStencilMode");
  SetDepthControl(base, device, value, 7, 0x80, true);
}

void SetStencilFail(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1A48 Device::SetRenderState_StencilFail");
  SetDepthControl(base, device, value, 11, 0x3800, true);
}

void SetStencilZFail(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1A88 Device::SetRenderState_StencilZFail");
  SetDepthControl(base, device, value, 17, 0xE0000, true);
}

void SetStencilPass(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1AC8 Device::SetRenderState_StencilPass");
  SetDepthControl(base, device, value, 14, 0x1C000, false);
}

void SetStencilFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1A18 Device::SetRenderState_StencilFunc");
  SetDepthControl(base, device, value, 8, 0x700, false);
}

void SetStencilRef(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1BD8 Device::SetRenderState_StencilRef");
  const Device d(base, device);
  d.set_stencil_ref(uint8_t(value));
  MarkRender(d, 0x200);
}

void SetStencilMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1BF8 Device::SetRenderState_StencilMask");
  const Device d(base, device);
  d.set_stencil_mask(uint8_t(value));
  MarkRender(d, 0x200);
}

void SetStencilWriteMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C18 Device::SetRenderState_StencilWriteMask");
  const Device d(base, device);
  d.set_stencil_write_mask(uint8_t(value));
  MarkRender(d, 0x200);
}

void SetCcwStencilFail(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1B28 Device::SetRenderState_CcwStencilFail");
  SetDepthControl(base, device, value, 23, 0x3800000, true);
}

void SetCcwStencilZFail(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1B68 Device::SetRenderState_CcwStencilZFail");
  SetDepthControl(base, device, value, 29, 0xE0000000u, true);
}

void SetCcwStencilPass(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1BA8 Device::SetRenderState_CcwStencilPass");
  SetDepthControl(base, device, value, 26, 0x1C000000, false);
}

void SetCcwStencilFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1AF8 Device::SetRenderState_CcwStencilFunc");
  SetDepthControl(base, device, value, 20, 0x700000, false);
}

void SetCcwStencilRef(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C38 Device::SetRenderState_CcwStencilRef");
  const Device d(base, device);
  d.set_ccw_stencil_ref(uint8_t(value));
  MarkRender(d, 0x400);
}

void SetCcwStencilMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C58 Device::SetRenderState_CcwStencilMask");
  const Device d(base, device);
  d.set_ccw_stencil_mask(uint8_t(value));
  MarkRender(d, 0x400);
}

void SetCcwStencilWriteMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C78 Device::SetRenderState_CcwStencilWriteMask");
  const Device d(base, device);
  d.set_ccw_stencil_write_mask(uint8_t(value));
  MarkRender(d, 0x400);
}

void SetClipPlaneEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C98 Device::SetRenderState_ClipPlaneEnable");
  const Device d(base, device);
  d.set_reg_200d(value != 0 ? 0x1000 : 0);
  d.set_pa_cl_clip_cntl((d.pa_cl_clip_cntl() & 0xFFFFFFC0) | value);
  MarkShader(d, 0x80);
  MarkRender(d, 0x2000000);
}

void SetPointSize(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1FA0 Device::SetRenderState_PointSize");
  const Device d(base, device);
  d.set_point_size(value);
  const uint16_t bits = PointSizeBits(base, Bits(value), Globals::times_8_address());
  d.set_pa_su_point_width(bits);  // PA_SU_POINT_SIZE, both halves
  d.set_pa_su_point_height(bits);
  MarkRender(d, 1ull << 59);
}

void SetPointSizeMin(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2000 Device::SetRenderState_PointSizeMin");
  const Device d(base, device);
  d.set_point_size_min(value);
  d.set_pa_su_point_min(PointSizeBits(base, Bits(value), Globals::times_16_address()));
  MarkRender(d, 1ull << 58);
}

void SetPointSpriteEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1F90 Device::SetRenderState_PointSpriteEnable");
  Device(base, device).set_point_sprite_enable(value);
}

void SetPointSizeMax(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2058 Device::SetRenderState_PointSizeMax");
  const Device d(base, device);
  d.set_point_size_max(value);
  d.set_pa_su_point_max(PointSizeBits(base, Bits(value), Globals::times_16_address()));
  MarkRender(d, 1ull << 58);
}

void SetMultiSampleAntiAlias(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1E60 Device::SetRenderState_MultiSampleAntiAlias");
  const Device d(base, device);
  Insert(base, d.pa_su_sc_mode_cntl_address(), value, 15, 0x8000);
  MarkShader(d, 0x40);
}

void SetMultiSampleMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1E90 Device::SetRenderState_MultiSampleMask");
  const Device d(base, device);
  d.set_pa_sc_aa_mask(value & 0xFFFF);
  MarkOther(d, 0x80000);
}

void SetSlopeScaleDepthBias(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1CF0 Device::SetRenderState_SlopeScaleDepthBias");
  const Device d(base, device);
  const float scale = Bits(value) * Globals::times_16(base);
  const float zero = Globals::zero(base);
  d.set_pa_su_poly_offset_front_scale(scale);  // front and back scales
  d.set_pa_su_poly_offset_back_scale(scale);
  const bool front = !(scale == zero) || !(d.pa_su_poly_offset_front_offset() == zero);
  const bool back = !(scale == zero) || !(d.pa_su_poly_offset_back_offset() == zero);
  SetPolygonOffsetEnables(base, device, front, back);
  MarkOther(d, (1ull << 45) | (1ull << 43));
  MarkShader(d, 0x40);
}

void SetDepthBias(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1DB8 Device::SetRenderState_DepthBias");
  const Device d(base, device);
  const float bias = Bits(value);
  const float zero = Globals::zero(base);
  const float front_scale = d.pa_su_poly_offset_front_scale();
  d.set_pa_su_poly_offset_front_offset_bits(value);  // front and back offsets
  d.set_pa_su_poly_offset_back_offset_bits(value);
  const bool front = !(front_scale == zero) || !(bias == zero);
  const bool back = !(d.pa_su_poly_offset_back_scale() == zero) || !(bias == zero);
  SetPolygonOffsetEnables(base, device, front, back);
  MarkOther(d, (1ull << 44) | (1ull << 42));
  MarkShader(d, 0x40);
}

void SetColorWriteEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1EB0 Device::SetRenderState_ColorWriteEnable");
  SetColorWriteMask(base, device, value, 0);
}

void SetColorWriteEnable1(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1EE8 Device::SetRenderState_ColorWriteEnable1");
  SetColorWriteMask(base, device, value, 1);
}

void SetColorWriteEnable2(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1F20 Device::SetRenderState_ColorWriteEnable2");
  SetColorWriteMask(base, device, value, 2);
}

void SetColorWriteEnable3(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1F58 Device::SetRenderState_ColorWriteEnable3");
  SetColorWriteMask(base, device, value, 3);
}

void SetTessellationMode(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A27B0 Device::SetRenderState_TessellationMode");
  const Device d(base, device);
  Insert(base, d.vgt_hos_cntl_address(), value, 0, 0x3);
  MarkRender(d, 1ull << 54);
}

void SetMinTessellationLevel(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2740 Device::SetRenderState_MinTessellationLevel");
  const Device d(base, device);
  d.set_vgt_hos_min_tess_level(value);
  MarkRender(d, 1ull << 52);
}

void SetMaxTessellationLevel(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2778 Device::SetRenderState_MaxTessellationLevel");
  const Device d(base, device);
  d.set_vgt_hos_max_tess_level(value);
  MarkRender(d, 1ull << 53);
}

void SetWrap0(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A20B0 Device::SetRenderState_Wrap0");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 0);
}

void SetWrap1(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A20D8 Device::SetRenderState_Wrap1");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 4);
}

void SetWrap2(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2108 Device::SetRenderState_Wrap2");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 8);
}

void SetWrap3(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2138 Device::SetRenderState_Wrap3");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 12);
}

void SetWrap4(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2168 Device::SetRenderState_Wrap4");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 16);
}

void SetWrap5(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2198 Device::SetRenderState_Wrap5");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 20);
}

void SetWrap6(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A21C8 Device::SetRenderState_Wrap6");
  SetWrap(base, device, Device(base, device).sq_wrapping_0_address(), value, 24);
}

void SetWrap7(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A21F8 Device::SetRenderState_Wrap7");
  // The top field: put in masked (rlwimi), unlike the others.
  const Device d(base, device);
  Insert(base, d.sq_wrapping_0_address(), value, 28, 0xF0000000u);
  MarkShader(d, 1ull << 45);
}

// Wraps 8-15 mark +16 bit 45, 0x2183's bit, as the original does.
void SetWrap8(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2220 Device::SetRenderState_Wrap8");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 0);
}

void SetWrap9(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2248 Device::SetRenderState_Wrap9");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 4);
}

void SetWrap10(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2278 Device::SetRenderState_Wrap10");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 8);
}

void SetWrap11(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A22A8 Device::SetRenderState_Wrap11");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 12);
}

void SetWrap12(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A22D8 Device::SetRenderState_Wrap12");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 16);
}

void SetWrap13(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2308 Device::SetRenderState_Wrap13");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 20);
}

void SetWrap14(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2338 Device::SetRenderState_Wrap14");
  SetWrap(base, device, Device(base, device).sq_wrapping_1_address(), value, 24);
}

void SetWrap15(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2368 Device::SetRenderState_Wrap15");
  const Device d(base, device);
  Insert(base, d.sq_wrapping_1_address(), value, 28, 0xF0000000u);
  MarkShader(d, 1ull << 45);
}

void SetViewportEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2430 Device::SetRenderState_ViewportEnable");
  const Device d(base, device);
  d.set_pa_cl_vte_cntl(value != 0 ? 0x43F : 0x400);
  Insert(base, d.pa_cl_clip_cntl_address(), value == 0 ? 1 : 0, 16, 0x10000);
  MarkShader(d, 32 | 128);
}

void SetHighPrecisionBlendEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2480 Device::SetRenderState_HighPrecisionBlendEnable");
  SetHighPrecisionBlend(base, device, value, 0, Device(base, device).rb_color_info_address(),
                        1ull << 37);
}

void SetHighPrecisionBlendEnable1(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2530 Device::SetRenderState_HighPrecisionBlendEnable1");
  SetHighPrecisionBlend(base, device, value, 1, Device(base, device).rb_color1_info_address(),
                        1ull << 35);
}

void SetHighPrecisionBlendEnable2(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A25E0 Device::SetRenderState_HighPrecisionBlendEnable2");
  SetHighPrecisionBlend(base, device, value, 2, Device(base, device).rb_color2_info_address(),
                        1ull << 34);
}

void SetHighPrecisionBlendEnable3(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2690 Device::SetRenderState_HighPrecisionBlendEnable3");
  SetHighPrecisionBlend(base, device, value, 3, Device(base, device).rb_color3_info_address(),
                        1ull << 33);
}

void SetHalfPixelOffset(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A27E8 Device::SetRenderState_HalfPixelOffset");
  const Device d(base, device);
  Insert(base, d.pa_su_vtx_cntl_address(), value, 0, 0x1);
  MarkOther(d, 1ull << 35);
}

void SetPrimitiveResetEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2820 Device::SetRenderState_PrimitiveResetEnable");
  const Device d(base, device);
  Insert(base, d.pa_su_sc_mode_cntl_address(), value, 21, 0x200000);
  MarkShader(d, 0x40);
}

void SetPrimitiveResetIndex(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2850 Device::SetRenderState_PrimitiveResetIndex");
  const Device d(base, device);
  d.set_vgt_multi_prim_ib_reset_indx(value);
  MarkRender(d, 0x80000);
}

void SetAlphaToMaskEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2870 Device::SetRenderState_AlphaToMaskEnable");
  const Device d(base, device);
  Insert(base, d.rb_colorcontrol_address(), value, 4, 0x10);
  MarkShader(d, 0x200);
}

void SetAlphaToMaskOffsets(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A28A0 Device::SetRenderState_AlphaToMaskOffsets");
  const Device d(base, device);
  Insert(base, d.rb_colorcontrol_address(), value, 24, 0xFF000000u);
  MarkShader(d, 0x200);
}

void SetGuardBandX(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A28D8 Device::SetRenderState_GuardBandX");
  const Device d(base, device);
  d.set_pa_cl_gb_horz_clip_adj(value);
  MarkOther(d, 1ull << 32);
}

void SetGuardBandY(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2908 Device::SetRenderState_GuardBandY");
  const Device d(base, device);
  d.set_pa_cl_gb_vert_clip_adj(value);
  MarkOther(d, 1ull << 34);
}

void SetDiscardBandX(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2938 Device::SetRenderState_DiscardBandX");
  const Device d(base, device);
  d.set_pa_cl_gb_horz_disc_adj(value);
  MarkOther(d, 1ull << 31);
}

void SetDiscardBandY(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2960 Device::SetRenderState_DiscardBandY");
  const Device d(base, device);
  d.set_pa_cl_gb_vert_disc_adj(value);
  MarkOther(d, 1ull << 33);
}

void SetHiStencilEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2990 Device::SetRenderState_HiStencilEnable");
  const Device d(base, device);
  Insert(base, d.rb_hizcontrol_address(), value, 3, 0x8);
  MarkShader(d, 0x100);
}

void SetHiStencilWriteEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A29C0 Device::SetRenderState_HiStencilWriteEnable");
  const Device d(base, device);
  Insert(base, d.rb_hizcontrol_address(), value, 2, 0x4);
  MarkShader(d, 0x100);
}

void SetHiStencilFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A29F0 Device::SetRenderState_HiStencilFunc");
  const Device d(base, device);
  Insert(base, d.rb_hizcontrol_address(), value, 5, 0x20);
  MarkShader(d, 0x100);
}

void SetHiStencilRef(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A20 Device::SetRenderState_HiStencilRef");
  const Device d(base, device);
  d.set_hi_stencil_ref(uint8_t(value));
  MarkShader(d, 0x100);
}

void SetPresentInterval(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A40 Device::SetRenderState_PresentInterval");
  Device(base, device).set_present_interval(value);
}

void SetPresentImmediateThreshold(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A50 Device::SetRenderState_PresentImmediateThreshold");
  Insert(base, Device(base, device).blend_flags_address(), value, 23, 0x3F800000);
}

void SetAddressU(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A31A0 Device::SetSamplerState_AddressU");
  SetSamplerField(base, device, sampler, 0, value, 10, 0x1C00);
}

void SetAddressV(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A31F0 Device::SetSamplerState_AddressV");
  SetSamplerField(base, device, sampler, 0, value, 13, 0xE000);
}

void SetAddressW(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3240 Device::SetSamplerState_AddressW");
  SetSamplerField(base, device, sampler, 0, value, 16, 0x70000);
}

void SetBorderColor(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3130 Device::SetSamplerState_BorderColor");
  const Device d(base, device);
  const uint32_t word5 = FetchConstant(d, sampler) + 20;
  Store(base, word5, (Load(base, word5) & 0xFFFFFFFCu) | (value != 0 ? 1 : 0));
  MarkShader(d, SamplerBit(sampler));
}

void SetMagFilter(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2BF8 Device::SetSamplerState_MagFilter");
  SetFilter(base, device, sampler, value, true);
}

void SetMinFilter(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A70 Device::SetSamplerState_MinFilter");
  SetFilter(base, device, sampler, value, false);
}

void SetMipFilter(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2D80 Device::SetSamplerState_MipFilter");
  SetSamplerField(base, device, sampler, 12, value, 23, 0x1800000);
}

void SetMipMapLodBias(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2F90 Device::SetSamplerState_MipMapLodBias");
  const uint32_t bias =
      uint32_t(ToInt64(double(Bits(value) * Globals::mip_map_lod_bias_scale(base))));
  SetSamplerField(base, device, sampler, 16, bias, 12, 0x3FF000);
}

void SetMaxMipLevel(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3030 Device::SetSamplerState_MaxMipLevel");
  SetMipLevel(base, device, sampler, value, true);
}

void SetMaxAnisotropy(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2E70 Device::SetSamplerState_MaxAnisotropy");
  const Device d(base, device);
  const uint32_t fc = FetchConstant(d, sampler);
  const uint32_t word4 = Load(base, fc + 16);
  if ((word4 & 0x800) || (word4 & 0x400)) {  // an anisotropic filter
    const uint32_t entry = Load(base, Globals::anisotropy_table_address() + (value << 2));
    Insert(base, fc + 12, entry, 25, 0xE000000);
    MarkShader(d, SamplerBit(sampler));
  }
  guest::StoreU8(base, d.max_anisotropy_address() + sampler, uint8_t(value));
}

void SetMagFilterZ(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2CE8 Device::SetSamplerState_MagFilterZ");
  SetZFilter(base, device, sampler, value, 0);
}

void SetMinFilterZ(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2B60 Device::SetSamplerState_MinFilterZ");
  SetZFilter(base, device, sampler, value, 1);
}

void SetSeparateZFilterEnable(uint8_t* base, uint32_t device, uint32_t sampler,
                              uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2DD8 Device::SetSamplerState_SeparateZFilterEnable");
  SetZFilter(base, device, sampler, value, 2);
}

void SetMinMipLevel(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A30B0 Device::SetSamplerState_MinMipLevel");
  SetMipLevel(base, device, sampler, value, false);
}

void SetTrilinearThreshold(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3290 Device::SetSamplerState_TrilinearThreshold");
  SetSamplerField(base, device, sampler, 20, value, 3, 0x18);
}

void SetAnisotropyBias(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2EE8 Device::SetSamplerState_AnisotropyBias");
  const uint32_t bias =
      uint32_t(ToInt64(double(Bits(value) * Globals::anisotropy_bias_scale(base))));
  SetSamplerField(base, device, sampler, 20, bias, 5, 0x1E0);
}

void SetHGradientExpBias(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A32E8 Device::SetSamplerState_HGradientExpBias");
  SetSamplerField(base, device, sampler, 16, value, 22, 0x7C00000);
}

void SetVGradientExpBias(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3340 Device::SetSamplerState_VGradientExpBias");
  SetSamplerField(base, device, sampler, 16, value, 27, 0xF8000000u);
}

void SetWhiteBorderColorW(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3398 Device::SetSamplerState_WhiteBorderColorW");
  SetSamplerField(base, device, sampler, 20, value, 2, 0x4);
}

void SetPointBorderEnable(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A33F0 Device::SetSamplerState_PointBorderEnable");
  SetSamplerField(base, device, sampler, 4, value == 0 ? 1 : 0, 11, 0x800);
}

bool GetViewport(uint8_t* base, uint32_t device, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_822A3740 Device::GetViewport");
  return out < 0xA0000000u &&
         c_runtime::Memcpy(base, out, Device(base, device).viewport_x_address(), 24);
}

bool SetStreamSource(uint8_t* base, uint32_t device, uint32_t stream, uint32_t buffer,
                     uint32_t offset, uint32_t stride, uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A3758 Device::SetStreamSource");
  const Device d(base, device);
  const uint32_t slot = d.streams_address() + 4 * stream;
  if (FenceNeedsFlush(base, device, Load(base, slot))) {
    return false;
  }
  if (buffer != 0) {
    // Vertex fetch constant 95 - stream: the address, then the size.
    const schema::DeviceVertexBuffer b(base, buffer);
    const uint32_t address = b.data() + offset;
    const uint32_t size = b.data_size() - offset;
    const uint32_t vertex_fetch = d.fetch_constants_address() + 8 * (95 - stream);
    Store(base, vertex_fetch + 4, size);
    Store(base, vertex_fetch, Physical(address));
    MarkShader(d, dirty);
  }
  Fence(base, device, Load(base, slot));
  const uint32_t words = stride >> 2;
  Store(base, slot, buffer);
  guest::StoreU8(base, d.stream_strides_address() + stream, uint8_t(words));
  if (words != 0 && words != guest::LoadU8(base, d.shader_strides_address() + stream)) {
    MarkShader(d, 1ull << 51);  // the vertex shader, patched again
  }
  return true;
}

bool SetIndices(uint8_t* base, uint32_t device, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_822A3908 Device::SetIndices");
  const Device d(base, device);
  const uint32_t previous = d.index_buffer();
  if (FenceNeedsFlush(base, device, previous)) {
    return false;
  }
  Fence(base, device, previous);
  d.set_index_buffer(buffer);
  return true;
}

bool SetTexture(uint8_t* base, uint32_t device, uint32_t stage, uint32_t texture,
                uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A6008 Device::SetTexture");
  const Device d(base, device);
  const uint32_t slot = d.textures_address() + 4 * stage;
  const uint32_t previous = Load(base, slot);
  if (FenceNeedsFlush(base, device, previous)) {
    return false;
  }
  if (texture != 0) {
    const schema::DeviceTexture t(base, texture);
    const uint32_t fc = FetchConstant(d, stage);
    const uint32_t mip = t.mip_address();
    const uint32_t word0 = t.fetch0();
    const uint32_t word2 = t.fetch2();
    const uint32_t word3 = t.fetch3();
    const uint32_t levels = t.mip_levels();
    const uint32_t address = Physical(t.base_address());
    const uint32_t mip_address =
        ((((mip >> 20) & 0xFFF) + 512) & 0x1000) + (mip & 0x1FFFFE00);
    const uint32_t old0 = Load(base, fc + 0);
    Store(base, fc + 8, word2);
    const uint32_t new0 = (old0 & 0x3FFC00) | (word0 & ~0x3FFC00u);  // the clamps kept
    const uint32_t new5 = (Load(base, fc + 20) & 0x1FF) | (mip_address & ~0x1FFu);
    uint32_t max_level = guest::LoadU8(base, d.max_mip_level_address() + stage);
    const uint32_t own_max = (levels >> 2) & 0xF;
    uint32_t new4 = (Load(base, fc + 16) & 0xFFFFFC03u) | (levels & 0x3FC);
    const uint32_t new3 = (Load(base, fc + 12) & 0x7FF80000) | (word3 & ~0x7FF80000u);
    Store(base, fc + 0, new0);
    const uint32_t new1 = (Load(base, fc + 4) & 0x800) | (address & ~0x800u);
    Store(base, fc + 16, new4);
    Store(base, fc + 20, new5);
    Store(base, fc + 12, new3);
    Store(base, fc + 4, new1);
    if (own_max > max_level) {
      max_level = (t.mip_levels() >> 2) & 0xF;
    }
    const uint32_t now = t.mip_levels();
    new4 = (Rotl(max_level, 2) & 0x3C) | (new4 & ~0x3Cu);
    uint32_t min_level = guest::LoadU8(base, d.min_mip_level_address() + stage);
    const uint32_t own_min = (now >> 6) & 0xF;
    Store(base, fc + 16, new4);
    if (own_min < min_level) {
      min_level = own_min;
    }
    new4 = (Rotl(min_level, 6) & 0x3C0) | (new4 & ~0x3C0u);
    Store(base, fc + 16, new4);
    MarkShader(d, dirty);
  }
  Store(base, slot, texture);
  Fence(base, device, previous);
  return true;
}

void SetBlendControl(uint8_t* base, uint32_t device, uint32_t target, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3B30 Device::SetBlendControl");
  const Device d(base, device);
  const uint32_t index = target != 0 ? target + 7 : 0;  // 0x2201, or 0x2208 + target
  Store(base, d.rb_blendcontrol0_address() + 4 * index, value);
  const uint32_t bit = index + 0x20135;
  const uint32_t mask_word = uint32_t(int32_t(bit) >> 16) << 3;  // the dirty mask's (+16)
  const uint32_t shift = bit & 0xFF;
  const uint64_t mask = (shift & 0x40) ? 0 : (0x8000000000000000ull >> (shift & 0x3F));
  Mark(base, d.vertex_float_dirty_address() + mask_word, mask);
  MarkShader(d, 1ull << 49);
}

void SetVertexShaderConstantF(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                              uint32_t count, uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A81F8 Device::SetVertexShaderConstantF");
  const Device d(base, device);
  CopyConstants(base, d.vertex_float_constants_address() + (first << 4), data, count);
  d.set_vertex_float_dirty(d.vertex_float_dirty() | dirty);
}

void SetPixelShaderConstantF(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                             uint32_t count, uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A8308 Device::SetPixelShaderConstantF");
  const Device d(base, device);
  CopyConstants(base, d.pixel_float_constants_address() + (first << 4), data, count);
  d.set_pixel_float_dirty(d.pixel_float_dirty() | dirty);
}

void SetPixelShaderConstantB(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                             uint32_t count) {
  ITC_NATIVE_RUNNING("sub_822A8470 Device::SetPixelShaderConstantB");
  const Device d(base, device);
  do {
    const uint32_t bit = first & 0x1F;
    // The pixel bools: +10000, the bool and loop constants' +16.
    const uint32_t word = d.bool_loop_constants_address() + 16 + 4 * (first >> 5);
    const uint32_t on = (schema::DeviceBoolConstant(base, data).value() & 1) << bit;
    Store(base, word, (Load(base, word) & ~(1u << bit)) | on);
    data += 4;
    first++;
  } while (--count != 0);
  MarkRender(d, 2);
}

uint32_t VertexDeclarationFromFvf(uint8_t* base, uint32_t fvf, uint32_t declaration) {
  ITC_NATIVE_RUNNING("sub_822A6A88 VertexDeclaration::FromFvf");
  // The element types (D3DDECLTYPE_*'s 360 codes) and usages.
  constexpr uint32_t kFloat3 = 0x2A23B9, kFloat4Ish = 0x1A23A6, kFloat1 = 0x2C83A4;
  constexpr uint32_t kFloat2 = 0x2C23A5, kColor = 0x182886, kUbyte4 = 0x1A2286;
  constexpr uint8_t kPosition = 0, kBlendWeight = 1, kBlendIndices = 2, kNormal = 3,
                    kPointSize = 4, kTexCoord = 5, kColorUsage = 10;
  // The texture coordinates' table: a byte a format, then the types.
  constexpr uint32_t kTexCoordTable = Globals::tex_coord_table_address();
  uint32_t at = declaration + 52;  // the elements (app-start-up's VertexElement)
  uint32_t count = 0;   // the original's r8
  uint32_t offset = 0;  // the next element's offset (r9)
  auto element = [&](uint32_t type, uint32_t element_offset, uint8_t usage, uint8_t index) {
    const schema::VertexElement e(base, at);
    e.set_stream(0);
    e.set_offset(uint16_t(element_offset));
    e.set_element_type(type);
    e.set_method(0);
    e.set_usage(usage);
    e.set_usage_index(index);
    at += 12;
  };
  const bool ubyte4 = (fvf & 0x1000) != 0;  // the last beta is UBYTE4 indices
  const bool color = (fvf & 0x8000) != 0;   // the last beta is a D3DCOLOR
  switch (fvf & 0x400E) {
    case 2:  // XYZ
      element(kFloat3, 0, kPosition, 0);
      offset = 12, count = 1;
      break;
    case 0x4002:  // XYZW
      element(kFloat4Ish, 0, kPosition, 0);
      offset = 16, count = 1;
      break;
    case 6:  // XYZB1
      element(kFloat3, 0, kPosition, 0);
      offset = 16, count = 2;
      if (ubyte4) {
        element(kUbyte4, 12, kBlendIndices, 0);
      } else if (color) {
        element(kColor, 12, kBlendIndices, 0);
      } else {
        element(kFloat1, 12, kBlendWeight, 0);
      }
      break;
    case 8:  // XYZB2
      element(kFloat3, 0, kPosition, 0);
      if (ubyte4) {
        element(kFloat1, 12, kBlendWeight, 0);
        element(kUbyte4, 16, kBlendIndices, 0);
        offset = 20, count = 3;
      } else if (color) {
        element(kColor, 12, kBlendWeight, 0);
        element(kUbyte4, 16, kBlendIndices, 0);
        offset = 20, count = 3;
      } else {
        element(kFloat2, 12, kBlendWeight, 0);
        offset = 20, count = 2;
      }
      break;
    case 10:  // XYZB3
      element(kFloat3, 0, kPosition, 0);
      offset = 24;
      if (ubyte4) {
        element(kFloat2, 12, kBlendWeight, 0);
        element(kUbyte4, 20, kBlendIndices, 0);
        count = 3;
      } else if (color) {
        element(kFloat2, 12, kBlendWeight, 0);
        element(kColor, 20, kBlendIndices, 0);
        count = 3;
      } else {
        element(kFloat3, 12, kBlendWeight, 0);
        count = 2;
      }
      break;
    case 12:  // XYZB4
      element(kFloat3, 0, kPosition, 0);
      offset = 28;
      if (ubyte4) {
        element(kFloat3, 12, kBlendWeight, 0);
        element(kUbyte4, 24, kBlendIndices, 0);
        count = 3;
      } else if (color) {
        element(kFloat3, 12, kBlendWeight, 0);
        element(kColor, 24, kBlendIndices, 0);
        count = 3;
      } else {
        element(kFloat4Ish, 12, kBlendWeight, 0);
        count = 2;
      }
      break;
    case 14:  // XYZB5
      element(kFloat3, 0, kPosition, 0);
      element(kFloat4Ish, 12, kBlendWeight, 0);
      offset = 32, count = 3;
      element(ubyte4 ? kUbyte4 : color ? kColor : kFloat1, 28, kBlendIndices, 0);
      break;
    default:  // 4 (XYZRHW) and the rest: no position element
      break;
  }
  if (fvf & 0x10) {  // NORMAL
    element(kFloat3, offset, kNormal, 0);
    count++, offset = (offset + 12) & 0xFFFF;
  }
  if (fvf & 0x20) {  // PSIZE
    element(kFloat1, offset, kPointSize, 0);
    count++, offset = (offset + 4) & 0xFFFF;
  }
  if (fvf & 0x40) {  // DIFFUSE
    element(kColor, offset, kColorUsage, 0);
    count++, offset = (offset + 4) & 0xFFFF;
  }
  if (fvf & 0x80) {  // SPECULAR
    element(kColor, offset, kColorUsage, 1);
    count++, offset = (offset + 4) & 0xFFFF;
  }
  const uint32_t texcoords = (fvf >> 8) & 0xF;
  uint32_t formats = (fvf >> 16) & 0xFFFF;
  count += texcoords;
  for (uint32_t i = 0; i < texcoords; i++) {
    const uint32_t size = guest::LoadU8(base, kTexCoordTable + (formats & 3)) & ~3u;
    element(Load(base, kTexCoordTable + size), offset, kTexCoord, uint8_t(i));
    formats >>= 2;
    offset = (offset + size) & 0xFFFF;
  }
  // The end marker (D3DDECL_END): stream 0xFF, type unused; three words
  // across VertexElement's fields, stored as the original's words.
  Store(base, at + 0, 0x00FF0000);
  Store(base, at + 4, 0xFFFFFFFFu);
  Store(base, at + 8, 0);
  const schema::VertexDeclaration decl(base, declaration);
  decl.set_element_count(count);
  decl.set_highest_stream(0);
  decl.set_unknown_48(0);
  return (at - declaration - 52) / 12;
}

void SetFVF(uint8_t* base, uint32_t device, uint32_t fvf) {
  ITC_NATIVE_RUNNING("sub_822AA0E8 Device::SetFVF");
  const Device d(base, device);
  d.set_fvf(fvf);
  VertexDeclarationFromFvf(base, fvf, d.fvf_declaration_address());
  d.set_vertex_declaration(d.fvf_declaration_address());
  MarkShader(d, 1ull << 51);
}

void SetScissorRect(uint8_t* base, uint32_t device, uint32_t rect) {
  ITC_NATIVE_RUNNING("sub_822A3450 Device::SetScissorRect");
  const int32_t left = int32_t(Load(base, rect + 0)), top = int32_t(Load(base, rect + 4));
  const int32_t right = int32_t(Load(base, rect + 8)), bottom = int32_t(Load(base, rect + 12));
  const Device d(base, device);
  const bool scissor = d.scissor_test_enable() != 0;
  int32_t x0 = int32_t(d.viewport_x()), y0 = int32_t(d.viewport_y());
  int32_t x1 = int32_t(d.viewport_width() + uint32_t(x0));
  int32_t y1 = int32_t(d.viewport_height() + uint32_t(y0));
  d.set_scissor_left(uint32_t(left));
  d.set_scissor_top(uint32_t(top));
  d.set_scissor_right(uint32_t(right));
  d.set_scissor_bottom(uint32_t(bottom));
  if (scissor) {
    x0 = x0 > left ? x0 : left;
    y0 = y0 > top ? y0 : top;
    x1 = x1 < right ? x1 : right;
    y1 = y1 < bottom ? y1 : bottom;
  }
  // PA_SC_WINDOW_SCISSOR_TL and _BR's shadows: 15 bits each.
  uint32_t tl = d.pa_sc_window_scissor_tl(), br = d.pa_sc_window_scissor_br();
  tl = (Rotl(uint32_t(y0), 16) & 0x7FFF0000) | (tl & 0x8000FFFFu);
  br = (Rotl(uint32_t(y1), 16) & 0x7FFF0000) | (br & 0x8000FFFFu);
  tl = (uint32_t(x0) & 0x7FFF) | (tl & 0xFFFF8000u);
  br = (uint32_t(x1) & 0x7FFF) | (br & 0xFFFF8000u);
  d.set_pa_sc_window_scissor_tl(tl);
  d.set_pa_sc_window_scissor_br(br);
  // Device::SetScreenScissor (sub_822A0F80) with the window: Dropped (its
  // packets, which nothing reads).
}

void SetScissorTestEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3D20 Device::SetRenderState_ScissorTestEnable");
  const Device d(base, device);
  d.set_scissor_test_enable(value);
  SetScissorRect(base, device, d.scissor_left_address());
}

void SetViewport(uint8_t* base, uint32_t device, uint32_t viewport) {
  ITC_NATIVE_RUNNING("sub_822A3518 Device::SetViewport");
  const Device d(base, device);
  const uint32_t target = d.render_target0().address;
  uint32_t surface = target;
  if (int32_t(surface) == 0) {
    surface = d.depth_surface().address;
    if (surface == 0) {
      return;
    }
  }
  // Whether predicated tiling holds the bound set (its size then the
  // tiling's): flag 0x10, or 0x20 with each bound surface the one saved
  // (+12468 to +12484) or none.
  const uint8_t flags = d.tiling_flags();
  bool tiled = false;
  if (flags & 0x10) {
    tiled = true;
  } else if (flags & 0x20) {
    tiled = true;
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t bound = Load(base, d.render_target0_address() + 4 * k);
      const uint32_t saved = Load(base, d.saved_render_target0_address() + 4 * k);
      if (saved != bound && bound != 0) {
        tiled = false;
        break;
      }
    }
  }
  uint32_t max_width, max_height;
  if (tiled) {
    max_width = d.tiling_width();
    max_height = d.tiling_height();
  } else {
    const uint32_t size = schema::DeviceSurface(base, surface).size();
    max_height = (Rotl(size, 29) & 0x7FFF) + 1;
    max_width = (Rotl(size, 14) & 0x3FFF) + 1;
  }
  const schema::DeviceViewport v(base, viewport);
  const uint32_t x = v.x(), y = v.y();
  uint32_t right = v.width() + x;
  if (!(right < max_width)) {
    right = max_width;
  }
  uint32_t bottom = v.height() + y;
  if (!(bottom < max_height)) {
    bottom = max_height;
  }
  const uint32_t width = right - x, height = bottom - y;
  d.set_viewport_x(x);
  d.set_viewport_y(y);
  d.set_viewport_width(width);
  d.set_viewport_height(height);
  d.set_viewport_min_z(v.min_z_bits());  // min and max Z
  d.set_viewport_max_z(v.max_z_bits());
  SetScissorRect(base, device, d.scissor_left_address());
  // PA_CL_VPORT_XSCALE ... _ZOFFSET.
  const float half = Globals::one_half(base);
  const float x_scale = float(double(width)) * half;
  const float y_scale = float(double(height)) * half;
  d.set_pa_cl_vport_xscale(x_scale);
  d.set_pa_cl_vport_xoffset(float(double(x)) + x_scale);
  d.set_pa_cl_vport_yscale(-y_scale);
  d.set_pa_cl_vport_yoffset(float(double(y)) + y_scale);
  d.set_pa_cl_vport_zscale(v.max_z() - v.min_z());
  d.set_pa_cl_vport_zoffset(v.min_z_bits());
  MarkRender(d, 0xFC);
}

void SetSurfaceInfo(uint8_t* base, uint32_t device, uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_822A3C88 Device::SetSurfaceInfo");
  const Device d(base, device);
  const uint32_t info = schema::DeviceSurface(base, surface).surface_info();
  const uint32_t msaa = (info >> 16) & 3;
  d.set_pa_sc_aa_config(msaa == 0 ? 0 : msaa == 1 ? 0x8001 : 0xC003);
  d.set_rb_surface_info(info);
  MarkRender(d, 1ull << 38);
  MarkOther(d, 1ull << 36);
  for (uint32_t i = 0; i < 16; i += 4) {  // the whole surface
    Store(base, d.scissor_left_address() + i, Load(base, Globals::full_scissor_address() + i));
  }
  SetViewport(base, device, Globals::full_viewport_address());
}

namespace {

// Whether predicated tiling holds the bound set: flag 0x10 of +10808, or
// 0x20 with each bound surface (four render targets and the depth surface)
// the one saved at +12468..+12484 or none.
bool TilingHoldsBoundSet(uint8_t* base, uint32_t device, uint8_t flags) {
  if (flags & 0x10) {
    return true;
  }
  if (!(flags & 0x20)) {
    return false;
  }
  const Device d(base, device);
  for (uint32_t k = 0; k < 5; k++) {
    const uint32_t bound = Load(base, d.render_target0_address() + 4 * k);
    if (Load(base, d.saved_render_target0_address() + 4 * k) != bound && bound != 0) {
      return false;
    }
  }
  return true;
}

}  // namespace

void UpdatePredicatedTiling(uint8_t* base, uint32_t device) {
  const Device d(base, device);
  bool on = false;
  if (d.predicated() == 0) {
    const uint8_t flags = d.tiling_flags();
    on = TilingHoldsBoundSet(base, device, flags) && !(flags & 0x8) &&
         ((d.flags3() & 0x40) || (d.unknown_11865() == 0 && d.unknown_11931() == 0));
  }
  const uint8_t flags = d.tiling_flags();
  d.set_tiling_flags(uint8_t((flags & ~0x2) | (on ? 0x2 : 0)));
}

void SetRenderTarget(uint8_t* base, uint32_t device, uint32_t index, uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_822A3D30 Device::SetRenderTarget");
  const Device d(base, device);
  Store(base, d.render_target0_address() + 4 * index, surface);
  if (surface != 0) {
    // RB_COLOR_INFO's shadow: 0x2001 for target 0, 0x2003-0x2005 for 1-3
    // (+24 bit 37, 35, 34, 33), then the high-precision blend applied
    // again to the surface's format.
    const uint32_t slot = index == 0 ? 0 : index + 1;
    const uint32_t color_info = d.rb_color_info_address() + 4 * slot;
    Store(base, color_info, schema::DeviceSurface(base, surface).format_info());
    const uint32_t shift = (26 + slot) & 0xFF;
    const uint64_t bit = (shift & 0x40) ? 0 : (0x8000000000000000ull >> (shift & 0x7F));
    MarkRender(d, bit);
    SetHighPrecisionBlend(base, device, Load(base, d.high_precision_blend0_address() + 4 * index),
                          index, color_info, bit);
  }
  if (index > 3) {
    return;
  }
  if (index == 0) {
    const uint32_t info = surface != 0 ? surface : d.depth_surface().address;
    if (info != 0) {
      SetSurfaceInfo(base, device, info);
    }
    UpdatePredicatedTiling(base, device);
  }
  // The target's color write mask applied again to RB_COLOR_MASK; +24
  // bit 18.
  SetColorWriteMask(base, device, Load(base, d.color_write_enable0_address() + 4 * index), index);
}

void SetRenderTargetExternal(uint8_t* base, uint32_t device, uint32_t index,
                             uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_822A4340 Device::SetRenderTarget_External");
  SetRenderTarget(base, device, index, surface);
}

void SetDepthStencilSurface(uint8_t* base, uint32_t device, uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_822A4060 Device::SetDepthStencilSurface");
  const Device d(base, device);
  d.set_depth_surface(surface);
  if (surface != 0) {
    if (d.render_target0().address == 0) {
      SetSurfaceInfo(base, device, surface);
    }
    const schema::DeviceSurface s(base, surface);
    const bool own_tiling = (d.flags3() & 0x40) != 0;
    d.set_rb_depth_info(s.format_info());
    d.set_rb_hizcontrol(s.hiz());
    const uint32_t hiz = s.hiz();
    const uint8_t b10810 = d.flags2();
    d.set_flags2(uint8_t((Rotl(hiz, 6) & 0x40) | (b10810 & ~0x40)));
    if (own_tiling && TilingHoldsBoundSet(base, device, d.tiling_flags())) {
      // RB_SURFACE_INFO's top 14 bits from the tiling's pitch.
      Insert(base, d.rb_surface_info_address(), d.tiling_pitch(), 18, 0xFFFC0000u);
    }
    MarkRender(d, 1ull << 36);
    MarkShader(d, 0x100 | (1ull << 49));
  } else {
    d.set_rb_hizcontrol(d.rb_hizcontrol() & 0xFFFFFFF0u);
    const uint8_t b10810 = d.flags2();
    d.set_flags2(uint8_t(b10810 & 191));
    MarkShader(d, 0x100);
  }
  UpdatePredicatedTiling(base, device);
  // The Z and stencil enables applied again to RB_DEPTHCONTROL (0 without
  // a depth surface).
  const uint32_t z = d.z_enable();
  d.set_z_enable(z);
  SetDepthControl(base, device, d.depth_surface().address != 0 ? z : 0, 1, 0x2, true);
  const uint32_t stencil = d.stencil_enable();
  d.set_stencil_enable(stencil);
  SetDepthControl(base, device, d.depth_surface().address != 0 ? stencil : 0, 0, 0x1, true);
}

// --- The packet writers ---

namespace {

uint64_t Rotl64(uint64_t x, int n) { return std::rotl(x, n); }

// A render state setter from the device's table, natively when it's one of
// ours (the empty sub_82106540 of states 0-36 does nothing).
bool CallRenderStateSetter(uint8_t* base, uint32_t device, uint32_t setter, uint32_t value) {
  switch (setter) {
#define ITC_SETTER_CASE(address, name, uses_float) \
  case 0x##address:                                \
    name(base, device, value);                     \
    return true;
    ITC_RENDER_STATE_SETTERS(ITC_SETTER_CASE)
#undef ITC_SETTER_CASE
    case 0x822A3D20:
      SetScissorTestEnable(base, device, value);
      return true;
    case 0x82106540:
      return true;
    default:
      return false;
  }
}

bool CallSamplerStateSetter(uint8_t* base, uint32_t device, uint32_t setter, uint32_t sampler,
                            uint32_t value) {
  switch (setter) {
#define ITC_SETTER_CASE(address, name, uses_float) \
  case 0x##address:                                \
    name(base, device, sampler, value);            \
    return true;
    ITC_SAMPLER_STATE_SETTERS(ITC_SETTER_CASE)
#undef ITC_SETTER_CASE
    case 0x82106540:
      return true;
    default:
      return false;
  }
}

}  // namespace

void SetRegisterF01Bits(uint8_t* base, uint32_t device, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_8229B7D8");
  Insert(base, Device(base, device).reg_f01_address(), bits, 12, 0x3000);
  // WAIT_UNTIL and register 0xF01 = +10788: packets nothing reads.
}

void DirtyAllState(uint8_t* base, uint32_t device, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_822A3B90 Device::DirtyAllState");
  const Device d(base, device);
  if (int32_t(mode) == 0) {
    // The binning packets (from +12448 and +12452): packets nothing reads.
    SetRegisterF01Bits(base, device, Rotl(d.reg_f01(), 20) & 3);
  }
  // Device::SetScreenScissor with the window scissor: Dropped.
  d.set_vertex_float_dirty(~0ull);
  d.set_pixel_float_dirty(~0ull);
  d.set_shader_dirty(~0ull);
  d.set_render_dirty(~0ull);
  d.set_other_dirty(~0ull);
}

void FlushForGprAllocation(uint8_t* base, uint32_t device, const DeviceCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8229F680");
  const Device d(base, device);
  const uint64_t shader = d.shader_dirty();
  if (shader != 0) {
    if (shader & 0x100) {
      const uint64_t bits = calls.flush_hiz(shader, d.rb_hizcontrol());
      d.set_shader_dirty(d.shader_dirty() | bits);
    }
    uint64_t m = d.shader_dirty();
    if (m & 0xFFF) {
      calls.flush_registers(Rotl64(m, 52) & 0xFFF0000000000000ull, 0x2200,
                            d.rb_depthcontrol_address());
      d.set_shader_dirty(d.shader_dirty() & ~0xFFFull);
    }
    m = d.shader_dirty();
    if (m & (0x1Full << 44)) {
      calls.flush_registers(Rotl64(m, 15) & 0xF800000000000000ull, 0x2180,
                            d.sq_program_cntl_address());
      d.set_shader_dirty(d.shader_dirty() & Rotl64(~31ull, 44));
    }
  }
  const uint64_t render = d.render_dirty();
  if (render != 0) {
    if (uint32_t(render) & 0x7FFFFC) {
      calls.flush_registers(Rotl64(render, 41) & 0xFFFFF80000000000ull, 0x2100,
                            d.reg_2100_address());
      d.set_render_dirty(d.render_dirty() & 0xFFFFFFFFFF800003ull);
    }
    uint64_t m = d.render_dirty();
    if (m & 0x7FFF800000ull) {
      calls.flush_registers(Rotl64(m, 25) & 0xFFFF000000000000ull, 0x2000,
                            d.rb_surface_info_address());
      d.set_render_dirty(d.render_dirty() & Rotl64(0xFFFFFFFFFFFF0000ull, 23));
    }
    m = d.render_dirty();
    if (m & (Rotl64(0x1FFFFF, 39) & 0xFFFFFF8000000000ull)) {
      calls.flush_registers(Rotl64(m, 4) & 0xFFFFF80000000000ull, 0x2280,
                            d.pa_su_point_height_address());
      d.set_render_dirty(d.render_dirty() & Rotl64(0xFFFFFFFFFFE00000ull, 39));
    }
  }
  const uint64_t other = d.other_dirty();
  if (other != 0 && (other & 0x3FFFFFFFFFull)) {
    calls.flush_registers(Rotl64(other, 26) & 0xFFFFFFFFFC000000ull, 0x2300,
                          d.reg_2300_address());
    d.set_other_dirty(d.other_dirty() & 0xFFFFFFC000000000ull);
  }
  // Left out: the segment end, the two built-in shaders (IM_LOAD_IMMEDIATE),
  // registers 0x2180 and 0x2181, sub_822AF978's register block, the registers
  // set for the dummy draws, the screen scissors (Dropped) and the two dummy
  // draws with their reservations (sub_8229E308): packets nothing reads. What
  // they overwrite is marked dirty again, as the original does, so the next
  // draw writes it from the shadow: sub_822AF978's marks first, then its own.
  MarkRender(d, 0x400000 | 0x200000);
  MarkShader(d, 0x80 | 0x40 | 0x20);
  MarkOther(d, 0x80000);
  MarkRender(d, 0x2000000);
  MarkShader(d, (1ull << 51) | (1ull << 52) | 0x800 | 0x100 | 0x8);
  MarkRender(d, 0x40000 | (1ull << 59));
  MarkOther(d, 1ull << 35);
  MarkShader(d, 1ull << 43);
}

void SetShaderGprAllocation(uint8_t* base, uint32_t device, uint32_t vertex, uint32_t pixel,
                            const DeviceCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822A8A60 Device::SetShaderGprAllocation");
  const uint32_t both = (Rotl(vertex, 8) & 0x7F00) | (pixel & ~0x7F00u);
  uint32_t split = (Rotl(both, 4) & 0x7FFF0) & ~0x800u;
  if (split == 0) {
    split = 0x40400;
  }
  Device(base, device).set_sq_gpr_management(split);
  // WAIT_UNTIL and register 0xD00 = the split (with bit 0, eight times, then
  // without): packets nothing reads, nor +10792.
  FlushForGprAllocation(base, device, calls);
}

bool SetGammaRamp(uint8_t* base, uint32_t device, uint32_t ramp) {
  ITC_NATIVE_RUNNING("sub_822A3A20 Device::SetGammaRamp");
  const uint32_t kept = Device(base, device).gamma_ramp_address();
  if (kept >= 0xA0000000u || !c_runtime::InOneView(kept, 1536)) {
    return false;
  }
  display::GammaRamp in;
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 256; i++) {
      in[c][i] = guest::LoadU16(base, ramp + 512 * c + 2 * i);
    }
  }
  const display::GammaRamp out =
      display::ApplyGammaCurve(display::MakeGammaCurve(display::kGamma, false), in);
  bool same = true;
  for (int c = 0; c < 3 && same; c++) {
    for (int i = 0; i < 256; i++) {
      if (guest::LoadU16(base, kept + 512 * c + 2 * i) != out[c][i]) {
        same = false;
        break;
      }
    }
  }
  if (same) {
    return true;
  }
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 256; i++) {
      guest::StoreU16(base, kept + 512 * c + 2 * i, out[c][i]);
    }
  }
  // sub_822B0C38, the display's gamma registers: packets nothing reads (our
  // renderer applies the 360's gamma from its own tables, --xbox_gamma).
  return true;
}

void SetDefaultState(uint8_t* base, uint32_t device, const DeviceCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822ACB90 Device::SetDefaultState");
  const Device d(base, device);
  const uint32_t interval = d.present_interval();
  for (uint32_t state = 0; state < 384; state += 4) {
    const uint32_t entry = Globals::render_state_table_address() + 3 * state;
    Store(base, d.render_state_setters_address() + state, Load(base, entry + 4));
    Store(base, d.render_state_getters_address() + state, Load(base, entry + 0));
    const uint32_t setter = Load(base, d.render_state_setters_address() + state);
    const uint32_t value = Load(base, entry + 8);
    if (!CallRenderStateSetter(base, device, setter, value)) {
      calls.setter(setter, value, 0);
    }
  }
  for (uint32_t sampler = 0; sampler < 26; sampler++) {
    for (uint32_t state = 0; state < 80; state += 4) {
      const uint32_t entry = Globals::sampler_state_table_address() + 3 * state;
      Store(base, d.sampler_state_setters_address() + state, Load(base, entry + 4));
      Store(base, d.sampler_state_getters_address() + state, Load(base, entry + 0));
      const uint32_t setter = Load(base, d.sampler_state_setters_address() + state);
      const uint32_t value = Load(base, entry + 8);
      if (!CallSamplerStateSetter(base, device, setter, sampler, value)) {
        calls.setter(setter, sampler, value);
      }
    }
    const uint32_t shift = (sampler + 20) & 0xFF;
    const uint64_t dirty = (shift & 0x40) ? 0 : (0x8000000000000000ull >> (shift & 0x7F));
    if (!SetTexture(base, device, sampler, 0, dirty)) {
      calls.set_texture(sampler, 0, dirty);
    }
  }
  // The default vertex declaration's header (the type, its count).
  d.set_fvf_declaration_type(5);
  d.set_fvf_declaration_count(1);
  SetRegisterF01Bits(base, device, 2);
  SetFVF(base, device, 2);
  SetPresentInterval(base, device, interval);
  d.set_predicated(0);
  UpdatePredicatedTiling(base, device);
  d.set_bin_mask_high(0);
  d.set_predication_select(0);
  d.set_predication_select_high(0);
  d.set_bin_mask(0xFFFFFFFFu);
  if (!(d.tiling_flags() & 0x80)) {
    SetShaderGprAllocation(base, device, 0, 0, calls);
  }
}

uint32_t GetViewportEnable(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_822A2470 Device::GetRenderState_ViewportEnable");
  return Device(base, device).pa_cl_vte_cntl() & 1;
}

}  // namespace graphics::device_state
