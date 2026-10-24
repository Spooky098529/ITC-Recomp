// The graphics device's state setters (docs/functions/device-state.md): see
// device_state.h. Every write matches the original's, word for word (the
// register shadow's fields are put in with the original's rotate-and-mask,
// so a value wider than its field spills the same way); the originals' stack
// temporaries (the float conversions' words below r1) aren't written.

#include "graphics/device_state.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"

#include <emmintrin.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace graphics::device_state {

namespace {

// The image's float constants the setters read.
constexpr uint32_t kOneOver255 = 0x82002200;  // the colors' bytes to floats
constexpr uint32_t kTimes8 = 0x82000C34;      // the point size to 12.4... (x 8)
constexpr uint32_t kTimes16 = 0x820021EC;     // point sizes and the depth slope (x 16)
constexpr uint32_t kZero = 0x82000A34;        // 0.0

// The register shadow's words and the software copies (byte offsets).
constexpr uint32_t kRbDepthControl = 10420;  // 0x2200
constexpr uint32_t kRbBlendControl0 = 10424;  // 0x2201
constexpr uint32_t kRbBlendControl1 = 10456;  // 0x2209-0x220B
constexpr uint32_t kRbColorControl = 10428;  // 0x2202
constexpr uint32_t kRbDepthHiZ = 10432;      // 0x2203
constexpr uint32_t kPaClClipCntl = 10436;    // 0x2204
constexpr uint32_t kPaSuScModeCntl = 10440;  // 0x2205
constexpr uint32_t kPaClVteCntl = 10444;     // 0x2206
constexpr uint32_t kRbColorMask = 10332;     // 0x2104
constexpr uint32_t kWrapping0 = 10412;       // 0x2183
constexpr uint32_t kWrapping1 = 10416;       // 0x2184
constexpr uint32_t kBlendControl = 11592;    // the blend control's software copy
constexpr uint32_t kBlendFlags = 11596;      // bit 31 blending on, bit 30 separate alpha

uint32_t Rotl(uint32_t x, int n) { return std::rotl(x, n); }

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
float LoadF(uint8_t* base, uint32_t address) { return guest::LoadFloat(base, address); }

// A dirty mask ORed with `bits`.
void Mark(uint8_t* base, uint32_t address, uint64_t bits) {
  REX_STORE_U64(address, REX_LOAD_U64(address) | bits);
}

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
  Store(base, device + kRbBlendControl0, blend);
  Store(base, device + kRbBlendControl1, blend);
  Store(base, device + kRbBlendControl1 + 4, blend);
  Store(base, device + kRbBlendControl1 + 8, blend);
  Mark(base, device + kShaderDirty, 0x407);
}

// SrcBlend, DestBlend and BlendOp: their bits in the software copy, then the
// four registers while blending is on.
void SetColorBlendField(uint8_t* base, uint32_t device, uint32_t value, int shift,
                        uint32_t mask) {
  const uint32_t flags = Load(base, device + kBlendFlags);
  Insert(base, device + kBlendControl, value, shift, mask);
  if (!(flags & 0x80000000u)) {
    return;
  }
  uint32_t blend = Load(base, device + kBlendControl);
  if (!(flags & 0x40000000u)) {
    blend = AlphaFromColor(blend);
  }
  StoreBlendControls(base, device, blend);
}

// SrcBlendAlpha, DestBlendAlpha and BlendOpAlpha: the registers only while
// blending and separate alpha are both on.
void SetAlphaBlendField(uint8_t* base, uint32_t device, uint32_t value, int shift,
                        uint32_t mask) {
  const uint32_t flags = Load(base, device + kBlendFlags);
  Insert(base, device + kBlendControl, value, shift, mask);
  if (!(flags & 0x80000000u) || !(flags & 0x40000000u)) {
    return;
  }
  StoreBlendControls(base, device, Load(base, device + kBlendControl));
}

// A field of RB_DEPTHCONTROL; +16 0x800, and bit 49 with `bit49`.
void SetDepthControl(uint8_t* base, uint32_t device, uint32_t value, int shift, uint32_t mask,
                     bool bit49) {
  Insert(base, device + kRbDepthControl, value, shift, mask);
  Mark(base, device + kShaderDirty, bit49 ? (0x800 | (1ull << 49)) : 0x800);
}

// The wraps but 7 and 15: the field cleared and the value ORed in shifted
// (not masked: a wider value spills into the next wraps, as in the original).
void SetWrap(uint8_t* base, uint32_t device, uint32_t word, uint32_t value, int shift) {
  const uint32_t field = 0xFu << shift;
  Store(base, device + word, (Load(base, device + word) & ~field) | (value << shift));
  Mark(base, device + kShaderDirty, 1ull << 45);
}

// A render target's color write mask: its software copy, and its bits in
// RB_COLOR_MASK (0 without the target bound).
void SetColorWriteMask(uint8_t* base, uint32_t device, uint32_t value, uint32_t target) {
  const uint32_t surface = Load(base, device + kRenderTargets + 4 * target);
  Store(base, device + 11604 + 4 * target, value);
  const uint32_t bits = surface != 0 ? value : 0;
  Insert(base, device + kRbColorMask, bits, 4 * target, 0xFu << (4 * target));
  Mark(base, device + kRenderDirty, 0x40000);
}

// A render target's high-precision blend: its software copy, and the bound
// surface's color format switched between its two forms (2 and 10, 3 and
// 12) when it doesn't match, in the surface and RB_COLOR_INFO's shadow.
void SetHighPrecisionBlend(uint8_t* base, uint32_t device, uint32_t value, uint32_t target,
                           uint32_t color_info, int dirty_bit) {
  const uint32_t surface = Load(base, device + kRenderTargets + 4 * target);
  Store(base, device + 11772 + 4 * target, value);
  if (surface == 0) {
    return;
  }
  const uint32_t info = Load(base, surface + 28);
  const uint32_t format = (info >> 16) & 0xF;
  if (format != 2 && format != 3 && format != 10 && format != 12) {
    return;
  }
  if ((((info >> 19) & 1) ^ value) == 0) {
    return;
  }
  const uint32_t now = Load(base, surface + 28);
  const uint32_t f = (now >> 16) & 0xF;
  const uint32_t m = value - 1;
  const uint32_t switched = (((f >> 1) - 3) & m) | (((f + 3) << 1) & ~m);
  const uint32_t field = (switched & 0xF) << 16;
  Store(base, surface + 28, (now & ~0xF0000u) | field);
  Store(base, device + color_info, (Load(base, device + color_info) & ~0xF0000u) | field);
  Mark(base, device + kRenderDirty, 1ull << dirty_bit);
}

// The depth bias enables: PA_SU_SC_MODE_CNTL bits 11 and 12 (front and
// back) set when the face's scale or offset isn't 0.
void SetPolygonOffsetEnables(uint8_t* base, uint32_t device, bool front, bool back) {
  Insert(base, device + kPaSuScModeCntl, front ? 1 : 0, 11, 0x800);
  Insert(base, device + kPaSuScModeCntl, back ? 1 : 0, 12, 0x1000);
}

// A point size times `scale` as an integer (fctiwz), its low 16 bits.
uint16_t PointSizeBits(uint8_t* base, float size, uint32_t scale) {
  return uint16_t(ToInt32(double(size * LoadF(base, scale))) & 0xFFFF);
}

// The sampler states'.
constexpr uint32_t kMipMapLodBiasScale = 0x820029CC;  // the LOD bias to fixed point
constexpr uint32_t kAnisotropyBiasScale = 0x82000F18;
constexpr uint32_t kAnisotropyTable = 0x82013628;  // max anisotropy -> the filter's bits
constexpr uint32_t kMaxAnisotropy = 11668;  // a byte a sampler
constexpr uint32_t kMaxMipLevel = 11694;
constexpr uint32_t kMinMipLevel = 11720;
constexpr uint32_t kZFilters = 11746;  // bits 0-2: mag, min, separate

// A sampler's fetch constant and its dirty bit (+16 bit 43 - sampler; 0
// past the mask, as the original's shift).
uint32_t FetchConstant(uint32_t device, uint32_t sampler) {
  return device + 1024 + 24 * sampler;
}
uint64_t SamplerBit(uint32_t sampler) {
  const uint32_t amount = (sampler + 20) & 0x7F;
  return (amount & 0x40) ? 0 : (0x8000000000000000ull >> amount);
}

// A field of one of the sampler's fetch constant words, then its bit.
void SetSamplerField(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t word,
                     uint32_t value, int shift, uint32_t mask) {
  Insert(base, FetchConstant(device, sampler) + word, value, shift, mask);
  Mark(base, device + kShaderDirty, SamplerBit(sampler));
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
  const uint32_t fc = FetchConstant(device, sampler);
  const uint32_t aniso = guest::LoadU8(base, device + sampler + kMaxAnisotropy);
  uint32_t word3 = Load(base, fc + 12);
  const uint32_t entry = Load(base, kAnisotropyTable + 4 * aniso);
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
  const uint32_t z = guest::LoadU8(base, device + sampler + kZFilters);
  Store(base, fc + 16, (word4 & 0xFFFFFFFCu) | (ZFilterBits(word3, z) & 3));
  Mark(base, device + kShaderDirty, SamplerBit(sampler));
}

// MagFilterZ, MinFilterZ and SeparateZFilterEnable: their bit of the z
// filters (`bit` cleared, the value ORed in shifted, not masked), then word
// 4's low bits worked out again.
void SetZFilter(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value, int shift) {
  const uint32_t fc = FetchConstant(device, sampler);
  const uint32_t word4 = Load(base, fc + 16);
  const uint32_t old = guest::LoadU8(base, device + sampler + kZFilters);
  const uint32_t z = (old & ~(1u << shift)) | (value << shift);
  const uint32_t word3 = Load(base, fc + 12);
  Store(base, fc + 16, (word4 & 0xFFFFFFFCu) | (ZFilterBits(word3, z) & 3));
  guest::StoreU8(base, device + sampler + kZFilters, uint8_t(z));
  Mark(base, device + kShaderDirty, SamplerBit(sampler));
}

// MaxMipLevel and MinMipLevel: the level kept, and with a texture bound the
// larger (smaller) of it and the texture's own in word 4.
void SetMipLevel(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value, bool max) {
  const uint32_t texture = Load(base, device + kTextures + 4 * sampler);
  if (texture != 0) {
    const uint32_t own = Load(base, texture + 44);
    uint32_t level = max ? (own >> 2) & 0xF : (own >> 6) & 0xF;
    if (max ? !(level > value) : !(level < value)) {
      level = value;
    }
    Insert(base, FetchConstant(device, sampler) + 16, level, max ? 2 : 6,
           max ? 0x3Cu : 0x3C0u);
    Mark(base, device + kShaderDirty, SamplerBit(sampler));
  }
  guest::StoreU8(base, device + sampler + (max ? kMaxMipLevel : kMinMipLevel), uint8_t(value));
}

// The fences of a replaced resource: with a command position (+10780), the
// resource's +8 = it; else, when the resource's flags (+0) meet the mask
// at +10784, an entry on the fence list (+13168; ends at +13172): the
// resource's address / 4 in bits 0-29, bit 30 clear, then -1. The
// original's bit 31 comes from an uninitialized stack word; 0 here (the
// list is in the command buffer, read by nothing since the emulated GPU
// went).
constexpr uint32_t kCommandPosition = 10780;
constexpr uint32_t kFenceMask = 10784;
constexpr uint32_t kFenceList = 13168;
constexpr uint32_t kFenceListEnd = 13172;

// Whether fencing `resource` would need the full list flushed.
bool FenceNeedsFlush(uint8_t* base, uint32_t device, uint32_t resource) {
  return resource != 0 && Load(base, device + kCommandPosition) == 0 &&
         (Load(base, device + kFenceMask) & Load(base, resource)) != 0 &&
         !(Load(base, device + kFenceList) < Load(base, device + kFenceListEnd));
}

void Fence(uint8_t* base, uint32_t device, uint32_t resource) {
  if (resource == 0) {
    return;
  }
  const uint32_t position = Load(base, device + kCommandPosition);
  if (position != 0) {
    Store(base, resource + 8, position);
    return;
  }
  if ((Load(base, device + kFenceMask) & Load(base, resource)) == 0) {
    return;
  }
  const uint32_t list = Load(base, device + kFenceList);
  Store(base, list, (resource >> 2) & 0x3FFFFFFF);
  Store(base, list + 4, 0xFFFFFFFFu);
  Store(base, device + kFenceList, list + 8);
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
    std::memcpy(out, REX_RAW_ADDR(from), first);
    if (first < 16) {
      std::memcpy(out + first, REX_RAW_ADDR(from + first), 16 - first);
    }
  };
  auto store = [&](uint32_t at, const uint8_t* in) {
    std::memcpy(REX_RAW_ADDR(at & ~0xFu), in, 16);
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
  const uint32_t depth = Load(base, device + kDepthSurface);
  Store(base, device + 11620, value);
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
  Insert(base, device + kPaSuScModeCntl, value, 3, 0x7F8);
  Mark(base, device + kShaderDirty, 0x40);
}

void SetCullMode(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1260 Device::SetRenderState_CullMode");
  Insert(base, device + kPaSuScModeCntl, value, 0, 0x7);
  Mark(base, device + kShaderDirty, 0x40);
}

void SetAlphaBlendEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1300 Device::SetRenderState_AlphaBlendEnable");
  Insert(base, device + kBlendFlags, value, 31, 0x80000000u);
  const uint32_t flags = Load(base, device + kBlendFlags);
  uint32_t blend = Load(base, device + kBlendControl);
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
  Insert(base, device + kBlendFlags, value, 30, 0x40000000u);
  const bool blending = (Load(base, device + kBlendFlags) & 0x80000000u) != 0;
  uint32_t blend = Load(base, device + kBlendControl);
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
  const float k = LoadF(base, kOneOver255);
  guest::StoreFloat(base, device + 10340, float((value >> 8) & 0xFF) * k);
  guest::StoreFloat(base, device + 10336, float((value >> 16) & 0xFF) * k);
  guest::StoreFloat(base, device + 10344, float(value & 0xFF) * k);
  guest::StoreFloat(base, device + 10348, float((value >> 24) & 0xFF) * k);
  Mark(base, device + kRenderDirty, 0x3C000);
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
  Insert(base, device + kRbColorControl, value, 3, 0x8);
  Mark(base, device + kShaderDirty, 0x200 | (1ull << 50));
}

void SetAlphaRef(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1728 Device::SetRenderState_AlphaRef");
  guest::StoreFloat(base, device + 10372, float(double(value)) * LoadF(base, kOneOver255));
  Mark(base, device + kRenderDirty, 0x100);
}

void SetAlphaFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1790 Device::SetRenderState_AlphaFunc");
  Insert(base, device + kRbColorControl, value, 0, 0x7);
  Mark(base, device + kShaderDirty, 0x200);
}

void SetStencilEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1990 Device::SetRenderState_StencilEnable");
  const uint32_t depth = Load(base, device + kDepthSurface);
  Store(base, device + 11624, value);
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
  guest::StoreU8(base, device + 10371, uint8_t(value));
  Mark(base, device + kRenderDirty, 0x200);
}

void SetStencilMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1BF8 Device::SetRenderState_StencilMask");
  guest::StoreU8(base, device + 10370, uint8_t(value));
  Mark(base, device + kRenderDirty, 0x200);
}

void SetStencilWriteMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C18 Device::SetRenderState_StencilWriteMask");
  guest::StoreU8(base, device + 10369, uint8_t(value));
  Mark(base, device + kRenderDirty, 0x200);
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
  guest::StoreU8(base, device + 10367, uint8_t(value));
  Mark(base, device + kRenderDirty, 0x400);
}

void SetCcwStencilMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C58 Device::SetRenderState_CcwStencilMask");
  guest::StoreU8(base, device + 10366, uint8_t(value));
  Mark(base, device + kRenderDirty, 0x400);
}

void SetCcwStencilWriteMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C78 Device::SetRenderState_CcwStencilWriteMask");
  guest::StoreU8(base, device + 10365, uint8_t(value));
  Mark(base, device + kRenderDirty, 0x400);
}

void SetClipPlaneEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1C98 Device::SetRenderState_ClipPlaneEnable");
  Store(base, device + 10292, value != 0 ? 0x1000 : 0);  // 0x200D
  Store(base, device + kPaClClipCntl, (Load(base, device + kPaClClipCntl) & 0xFFFFFFC0) | value);
  Mark(base, device + kShaderDirty, 0x80);
  Mark(base, device + kRenderDirty, 0x2000000);
}

void SetPointSize(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1FA0 Device::SetRenderState_PointSize");
  Store(base, device + 11636, value);
  const uint16_t bits = PointSizeBits(base, Bits(value), kTimes8);
  guest::StoreU16(base, device + 10470, bits);  // PA_SU_POINT_SIZE, both halves
  guest::StoreU16(base, device + 10468, bits);
  Mark(base, device + kRenderDirty, 1ull << 59);
}

void SetPointSizeMin(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2000 Device::SetRenderState_PointSizeMin");
  Store(base, device + 11640, value);
  guest::StoreU16(base, device + 10474, PointSizeBits(base, Bits(value), kTimes16));
  Mark(base, device + kRenderDirty, 1ull << 58);
}

void SetPointSpriteEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1F90 Device::SetRenderState_PointSpriteEnable");
  Store(base, device + 11628, value);
}

void SetPointSizeMax(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2058 Device::SetRenderState_PointSizeMax");
  Store(base, device + 11644, value);
  guest::StoreU16(base, device + 10472, PointSizeBits(base, Bits(value), kTimes16));
  Mark(base, device + kRenderDirty, 1ull << 58);
}

void SetMultiSampleAntiAlias(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1E60 Device::SetRenderState_MultiSampleAntiAlias");
  Insert(base, device + kPaSuScModeCntl, value, 15, 0x8000);
  Mark(base, device + kShaderDirty, 0x40);
}

void SetMultiSampleMask(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1E90 Device::SetRenderState_MultiSampleMask");
  Store(base, device + 10624, value & 0xFFFF);  // PA_SC_AA_MASK
  Mark(base, device + kOtherDirty, 0x80000);
}

void SetSlopeScaleDepthBias(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1CF0 Device::SetRenderState_SlopeScaleDepthBias");
  const float scale = Bits(value) * LoadF(base, kTimes16);
  const float zero = LoadF(base, kZero);
  guest::StoreFloat(base, device + 10704, scale);  // front and back scales
  guest::StoreFloat(base, device + 10712, scale);
  const bool front = !(scale == zero) || !(LoadF(base, device + 10708) == zero);
  const bool back = !(scale == zero) || !(LoadF(base, device + 10716) == zero);
  SetPolygonOffsetEnables(base, device, front, back);
  Mark(base, device + kOtherDirty, (1ull << 45) | (1ull << 43));
  Mark(base, device + kShaderDirty, 0x40);
}

void SetDepthBias(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A1DB8 Device::SetRenderState_DepthBias");
  const float bias = Bits(value);
  const float zero = LoadF(base, kZero);
  const float front_scale = LoadF(base, device + 10704);
  Store(base, device + 10708, value);  // front and back offsets
  Store(base, device + 10716, value);
  const bool front = !(front_scale == zero) || !(bias == zero);
  const bool back = !(LoadF(base, device + 10712) == zero) || !(bias == zero);
  SetPolygonOffsetEnables(base, device, front, back);
  Mark(base, device + kOtherDirty, (1ull << 44) | (1ull << 42));
  Mark(base, device + kShaderDirty, 0x40);
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
  Insert(base, device + 10488, value, 0, 0x3);  // 0x2285
  Mark(base, device + kRenderDirty, 1ull << 54);
}

void SetMinTessellationLevel(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2740 Device::SetRenderState_MinTessellationLevel");
  Store(base, device + 10496, value);  // 0x2287
  Mark(base, device + kRenderDirty, 1ull << 52);
}

void SetMaxTessellationLevel(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2778 Device::SetRenderState_MaxTessellationLevel");
  Store(base, device + 10492, value);  // 0x2286
  Mark(base, device + kRenderDirty, 1ull << 53);
}

void SetWrap0(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A20B0 Device::SetRenderState_Wrap0");
  SetWrap(base, device, kWrapping0, value, 0);
}

void SetWrap1(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A20D8 Device::SetRenderState_Wrap1");
  SetWrap(base, device, kWrapping0, value, 4);
}

void SetWrap2(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2108 Device::SetRenderState_Wrap2");
  SetWrap(base, device, kWrapping0, value, 8);
}

void SetWrap3(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2138 Device::SetRenderState_Wrap3");
  SetWrap(base, device, kWrapping0, value, 12);
}

void SetWrap4(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2168 Device::SetRenderState_Wrap4");
  SetWrap(base, device, kWrapping0, value, 16);
}

void SetWrap5(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2198 Device::SetRenderState_Wrap5");
  SetWrap(base, device, kWrapping0, value, 20);
}

void SetWrap6(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A21C8 Device::SetRenderState_Wrap6");
  SetWrap(base, device, kWrapping0, value, 24);
}

void SetWrap7(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A21F8 Device::SetRenderState_Wrap7");
  // The top field: put in masked (rlwimi), unlike the others.
  Insert(base, device + kWrapping0, value, 28, 0xF0000000u);
  Mark(base, device + kShaderDirty, 1ull << 45);
}

// Wraps 8-15 mark +16 bit 45, 0x2183's bit, as the original does.
void SetWrap8(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2220 Device::SetRenderState_Wrap8");
  SetWrap(base, device, kWrapping1, value, 0);
}

void SetWrap9(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2248 Device::SetRenderState_Wrap9");
  SetWrap(base, device, kWrapping1, value, 4);
}

void SetWrap10(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2278 Device::SetRenderState_Wrap10");
  SetWrap(base, device, kWrapping1, value, 8);
}

void SetWrap11(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A22A8 Device::SetRenderState_Wrap11");
  SetWrap(base, device, kWrapping1, value, 12);
}

void SetWrap12(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A22D8 Device::SetRenderState_Wrap12");
  SetWrap(base, device, kWrapping1, value, 16);
}

void SetWrap13(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2308 Device::SetRenderState_Wrap13");
  SetWrap(base, device, kWrapping1, value, 20);
}

void SetWrap14(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2338 Device::SetRenderState_Wrap14");
  SetWrap(base, device, kWrapping1, value, 24);
}

void SetWrap15(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2368 Device::SetRenderState_Wrap15");
  Insert(base, device + kWrapping1, value, 28, 0xF0000000u);
  Mark(base, device + kShaderDirty, 1ull << 45);
}

void SetViewportEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2430 Device::SetRenderState_ViewportEnable");
  Store(base, device + kPaClVteCntl, value != 0 ? 0x43F : 0x400);
  Insert(base, device + kPaClClipCntl, value == 0 ? 1 : 0, 16, 0x10000);
  Mark(base, device + kShaderDirty, 32 | 128);
}

void SetHighPrecisionBlendEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2480 Device::SetRenderState_HighPrecisionBlendEnable");
  SetHighPrecisionBlend(base, device, value, 0, 10244, 37);
}

void SetHighPrecisionBlendEnable1(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2530 Device::SetRenderState_HighPrecisionBlendEnable1");
  SetHighPrecisionBlend(base, device, value, 1, 10252, 35);
}

void SetHighPrecisionBlendEnable2(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A25E0 Device::SetRenderState_HighPrecisionBlendEnable2");
  SetHighPrecisionBlend(base, device, value, 2, 10256, 34);
}

void SetHighPrecisionBlendEnable3(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2690 Device::SetRenderState_HighPrecisionBlendEnable3");
  SetHighPrecisionBlend(base, device, value, 3, 10260, 33);
}

void SetHalfPixelOffset(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A27E8 Device::SetRenderState_HalfPixelOffset");
  Insert(base, device + 10560, value, 0, 0x1);  // PA_SU_VTX_CNTL
  Mark(base, device + kOtherDirty, 1ull << 35);
}

void SetPrimitiveResetEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2820 Device::SetRenderState_PrimitiveResetEnable");
  Insert(base, device + kPaSuScModeCntl, value, 21, 0x200000);
  Mark(base, device + kShaderDirty, 0x40);
}

void SetPrimitiveResetIndex(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2850 Device::SetRenderState_PrimitiveResetIndex");
  Store(base, device + 10328, value);  // 0x2103
  Mark(base, device + kRenderDirty, 0x80000);
}

void SetAlphaToMaskEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2870 Device::SetRenderState_AlphaToMaskEnable");
  Insert(base, device + kRbColorControl, value, 4, 0x10);
  Mark(base, device + kShaderDirty, 0x200);
}

void SetAlphaToMaskOffsets(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A28A0 Device::SetRenderState_AlphaToMaskOffsets");
  Insert(base, device + kRbColorControl, value, 24, 0xFF000000u);
  Mark(base, device + kShaderDirty, 0x200);
}

void SetGuardBandX(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A28D8 Device::SetRenderState_GuardBandX");
  Store(base, device + 10572, value);  // 0x2305
  Mark(base, device + kOtherDirty, 1ull << 32);
}

void SetGuardBandY(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2908 Device::SetRenderState_GuardBandY");
  Store(base, device + 10564, value);  // 0x2303
  Mark(base, device + kOtherDirty, 1ull << 34);
}

void SetDiscardBandX(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2938 Device::SetRenderState_DiscardBandX");
  Store(base, device + 10576, value);  // 0x2306
  Mark(base, device + kOtherDirty, 1ull << 31);
}

void SetDiscardBandY(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2960 Device::SetRenderState_DiscardBandY");
  Store(base, device + 10568, value);  // 0x2304
  Mark(base, device + kOtherDirty, 1ull << 33);
}

void SetHiStencilEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2990 Device::SetRenderState_HiStencilEnable");
  Insert(base, device + kRbDepthHiZ, value, 3, 0x8);
  Mark(base, device + kShaderDirty, 0x100);
}

void SetHiStencilWriteEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A29C0 Device::SetRenderState_HiStencilWriteEnable");
  Insert(base, device + kRbDepthHiZ, value, 2, 0x4);
  Mark(base, device + kShaderDirty, 0x100);
}

void SetHiStencilFunc(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A29F0 Device::SetRenderState_HiStencilFunc");
  Insert(base, device + kRbDepthHiZ, value, 5, 0x20);
  Mark(base, device + kShaderDirty, 0x100);
}

void SetHiStencilRef(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A20 Device::SetRenderState_HiStencilRef");
  guest::StoreU8(base, device + kRbDepthHiZ + 2, uint8_t(value));
  Mark(base, device + kShaderDirty, 0x100);
}

void SetPresentInterval(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A40 Device::SetRenderState_PresentInterval");
  Store(base, device + 13240, value);
}

void SetPresentImmediateThreshold(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2A50 Device::SetRenderState_PresentImmediateThreshold");
  Insert(base, device + kBlendFlags, value, 23, 0x3F800000);
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
  const uint32_t word5 = FetchConstant(device, sampler) + 20;
  Store(base, word5, (Load(base, word5) & 0xFFFFFFFCu) | (value != 0 ? 1 : 0));
  Mark(base, device + kShaderDirty, SamplerBit(sampler));
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
      uint32_t(ToInt64(double(Bits(value) * LoadF(base, kMipMapLodBiasScale))));
  SetSamplerField(base, device, sampler, 16, bias, 12, 0x3FF000);
}

void SetMaxMipLevel(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3030 Device::SetSamplerState_MaxMipLevel");
  SetMipLevel(base, device, sampler, value, true);
}

void SetMaxAnisotropy(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A2E70 Device::SetSamplerState_MaxAnisotropy");
  const uint32_t fc = FetchConstant(device, sampler);
  const uint32_t word4 = Load(base, fc + 16);
  if ((word4 & 0x800) || (word4 & 0x400)) {  // an anisotropic filter
    const uint32_t entry = Load(base, kAnisotropyTable + (value << 2));
    Insert(base, fc + 12, entry, 25, 0xE000000);
    Mark(base, device + kShaderDirty, SamplerBit(sampler));
  }
  guest::StoreU8(base, device + sampler + kMaxAnisotropy, uint8_t(value));
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
      uint32_t(ToInt64(double(Bits(value) * LoadF(base, kAnisotropyBiasScale))));
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
  return out < 0xA0000000u && c_runtime::Memcpy(base, out, device + 12392, 24);
}

bool SetStreamSource(uint8_t* base, uint32_t device, uint32_t stream, uint32_t buffer,
                     uint32_t offset, uint32_t stride, uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A3758 Device::SetStreamSource");
  const uint32_t slot = device + 4 * (stream + 3051);  // +12204, the streams
  if (FenceNeedsFlush(base, device, Load(base, slot))) {
    return false;
  }
  if (buffer != 0) {
    // Vertex fetch constant 95 - stream: the address, then the size.
    const uint32_t address = Load(base, buffer + 24) + offset;
    const uint32_t size = Load(base, buffer + 28) - offset;
    const uint32_t index = 17 - stream;
    Store(base, device + 8 * index + 1652, size);
    Store(base, device + 8 * (index + 206), Physical(address));
    Mark(base, device + kShaderDirty, dirty);
  }
  Fence(base, device, Load(base, slot));
  const uint32_t words = stride >> 2;
  Store(base, slot, buffer);
  guest::StoreU8(base, device + stream + 12272, uint8_t(words));
  if (words != 0 && words != guest::LoadU8(base, device + stream + 11568)) {
    Mark(base, device + kShaderDirty, 1ull << 51);  // the vertex shader, patched again
  }
  return true;
}

bool SetIndices(uint8_t* base, uint32_t device, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_822A3908 Device::SetIndices");
  const uint32_t previous = Load(base, device + 12180);
  if (FenceNeedsFlush(base, device, previous)) {
    return false;
  }
  Fence(base, device, previous);
  Store(base, device + 12180, buffer);
  return true;
}

bool SetTexture(uint8_t* base, uint32_t device, uint32_t stage, uint32_t texture,
                uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A6008 Device::SetTexture");
  const uint32_t slot = device + 4 * (stage + 3072);  // +12288, the textures
  const uint32_t previous = Load(base, slot);
  if (FenceNeedsFlush(base, device, previous)) {
    return false;
  }
  if (texture != 0) {
    const uint32_t fc = FetchConstant(device, stage);
    const uint32_t mip = Load(base, texture + 48);
    const uint32_t word0 = Load(base, texture + 28);
    const uint32_t word2 = Load(base, texture + 36);
    const uint32_t word3 = Load(base, texture + 40);
    const uint32_t levels = Load(base, texture + 44);
    const uint32_t address = Physical(Load(base, texture + 32));
    const uint32_t mip_address =
        ((((mip >> 20) & 0xFFF) + 512) & 0x1000) + (mip & 0x1FFFFE00);
    const uint32_t old0 = Load(base, fc + 0);
    Store(base, fc + 8, word2);
    const uint32_t new0 = (old0 & 0x3FFC00) | (word0 & ~0x3FFC00u);  // the clamps kept
    const uint32_t new5 = (Load(base, fc + 20) & 0x1FF) | (mip_address & ~0x1FFu);
    uint32_t max_level = guest::LoadU8(base, device + stage + kMaxMipLevel);
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
      max_level = (Load(base, texture + 44) >> 2) & 0xF;
    }
    const uint32_t now = Load(base, texture + 44);
    new4 = (Rotl(max_level, 2) & 0x3C) | (new4 & ~0x3Cu);
    uint32_t min_level = guest::LoadU8(base, device + stage + kMinMipLevel);
    const uint32_t own_min = (now >> 6) & 0xF;
    Store(base, fc + 16, new4);
    if (own_min < min_level) {
      min_level = own_min;
    }
    new4 = (Rotl(min_level, 6) & 0x3C0) | (new4 & ~0x3C0u);
    Store(base, fc + 16, new4);
    Mark(base, device + kShaderDirty, dirty);
  }
  Store(base, slot, texture);
  Fence(base, device, previous);
  return true;
}

void SetBlendControl(uint8_t* base, uint32_t device, uint32_t target, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3B30 Device::SetBlendControl");
  const uint32_t index = target != 0 ? target + 7 : 0;  // 0x2201, or 0x2208 + target
  Store(base, device + 4 * (index + 2606), value);
  const uint32_t bit = index + 0x20135;
  const uint32_t mask_word = uint32_t(int32_t(bit) >> 16) << 3;
  const uint32_t shift = bit & 0xFF;
  const uint64_t mask = (shift & 0x40) ? 0 : (0x8000000000000000ull >> (shift & 0x3F));
  Mark(base, device + mask_word, mask);
  Mark(base, device + kShaderDirty, 1ull << 49);
}

void SetVertexShaderConstantF(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                              uint32_t count, uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A81F8 Device::SetVertexShaderConstantF");
  CopyConstants(base, device + ((first + 112) << 4), data, count);
  Mark(base, device + 0, dirty);
}

void SetPixelShaderConstantF(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                             uint32_t count, uint64_t dirty) {
  ITC_NATIVE_RUNNING("sub_822A8308 Device::SetPixelShaderConstantF");
  CopyConstants(base, device + ((first + 368) << 4), data, count);
  Mark(base, device + 8, dirty);
}

void SetPixelShaderConstantB(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                             uint32_t count) {
  ITC_NATIVE_RUNNING("sub_822A8470 Device::SetPixelShaderConstantB");
  do {
    const uint32_t bit = first & 0x1F;
    const uint32_t word = device + 4 * ((first >> 5) + 2500);  // +10000
    const uint32_t on = (Load(base, data) & 1) << bit;
    Store(base, word, (Load(base, word) & ~(1u << bit)) | on);
    data += 4;
    first++;
  } while (--count != 0);
  Mark(base, device + kRenderDirty, 2);
}

uint32_t VertexDeclarationFromFvf(uint8_t* base, uint32_t fvf, uint32_t declaration) {
  ITC_NATIVE_RUNNING("sub_822A6A88 VertexDeclaration::FromFvf");
  // The element types (D3DDECLTYPE_*'s 360 codes) and usages.
  constexpr uint32_t kFloat3 = 0x2A23B9, kFloat4Ish = 0x1A23A6, kFloat1 = 0x2C83A4;
  constexpr uint32_t kFloat2 = 0x2C23A5, kColor = 0x182886, kUbyte4 = 0x1A2286;
  constexpr uint8_t kPosition = 0, kBlendWeight = 1, kBlendIndices = 2, kNormal = 3,
                    kPointSize = 4, kTexCoord = 5, kColorUsage = 10;
  constexpr uint32_t kTexCoordTable = 0x82013690;  // a byte a format, then the types
  uint32_t at = declaration + 52;
  uint32_t count = 0;   // the original's r8
  uint32_t offset = 0;  // the next element's offset (r9)
  auto element = [&](uint32_t type, uint32_t element_offset, uint8_t usage, uint8_t index) {
    guest::StoreU16(base, at + 0, 0);
    guest::StoreU16(base, at + 2, uint16_t(element_offset));
    Store(base, at + 4, type);
    guest::StoreU8(base, at + 8, 0);
    guest::StoreU8(base, at + 9, usage);
    guest::StoreU8(base, at + 10, index);
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
  // The end marker (D3DDECL_END): stream 0xFF, type unused.
  Store(base, at + 0, 0x00FF0000);
  Store(base, at + 4, 0xFFFFFFFFu);
  Store(base, at + 8, 0);
  Store(base, declaration + 24, count);
  Store(base, declaration + 28, 0);
  Store(base, declaration + 48, 0);
  return (at - declaration - 52) / 12;
}

void SetFVF(uint8_t* base, uint32_t device, uint32_t fvf) {
  ITC_NATIVE_RUNNING("sub_822AA0E8 Device::SetFVF");
  Store(base, device + 12176, fvf);
  VertexDeclarationFromFvf(base, fvf, device + 11936);
  Store(base, device + 11552, device + 11936);  // the vertex declaration
  Mark(base, device + kShaderDirty, 1ull << 51);
}

void SetScissorRect(uint8_t* base, uint32_t device, uint32_t rect) {
  ITC_NATIVE_RUNNING("sub_822A3450 Device::SetScissorRect");
  const int32_t left = int32_t(Load(base, rect + 0)), top = int32_t(Load(base, rect + 4));
  const int32_t right = int32_t(Load(base, rect + 8)), bottom = int32_t(Load(base, rect + 12));
  const bool scissor = Load(base, device + 11600) != 0;
  int32_t x0 = int32_t(Load(base, device + 12392)), y0 = int32_t(Load(base, device + 12396));
  int32_t x1 = int32_t(Load(base, device + 12400) + uint32_t(x0));
  int32_t y1 = int32_t(Load(base, device + 12404) + uint32_t(y0));
  Store(base, device + 12416, uint32_t(left));
  Store(base, device + 12420, uint32_t(top));
  Store(base, device + 12424, uint32_t(right));
  Store(base, device + 12428, uint32_t(bottom));
  if (scissor) {
    x0 = x0 > left ? x0 : left;
    y0 = y0 > top ? y0 : top;
    x1 = x1 < right ? x1 : right;
    y1 = y1 < bottom ? y1 : bottom;
  }
  // PA_SC_WINDOW_SCISSOR_TL and _BR's shadows: 15 bits each.
  uint32_t tl = Load(base, device + 10308), br = Load(base, device + 10312);
  tl = (Rotl(uint32_t(y0), 16) & 0x7FFF0000) | (tl & 0x8000FFFFu);
  br = (Rotl(uint32_t(y1), 16) & 0x7FFF0000) | (br & 0x8000FFFFu);
  tl = (uint32_t(x0) & 0x7FFF) | (tl & 0xFFFF8000u);
  br = (uint32_t(x1) & 0x7FFF) | (br & 0xFFFF8000u);
  Store(base, device + 10308, tl);
  Store(base, device + 10312, br);
  // Device::SetScreenScissor (sub_822A0F80) with the window: Dropped (its
  // packets, which nothing reads).
}

void SetScissorTestEnable(uint8_t* base, uint32_t device, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_822A3D20 Device::SetRenderState_ScissorTestEnable");
  Store(base, device + 11600, value);
  SetScissorRect(base, device, device + 12416);
}

void SetViewport(uint8_t* base, uint32_t device, uint32_t viewport) {
  ITC_NATIVE_RUNNING("sub_822A3518 Device::SetViewport");
  const uint32_t target = Load(base, device + kRenderTargets);
  uint32_t surface = target;
  if (int32_t(surface) == 0) {
    surface = Load(base, device + kDepthSurface);
    if (surface == 0) {
      return;
    }
  }
  // Whether predicated tiling holds the bound set (its size then the
  // tiling's): flag 0x10, or 0x20 with each bound surface the one saved
  // (+12468 to +12484) or none.
  const uint8_t flags = guest::LoadU8(base, device + 10808);
  bool tiled = false;
  if (flags & 0x10) {
    tiled = true;
  } else if (flags & 0x20) {
    tiled = true;
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t bound = Load(base, device + kRenderTargets + 4 * k);
      const uint32_t saved = Load(base, device + 12468 + 4 * k);
      if (saved != bound && bound != 0) {
        tiled = false;
        break;
      }
    }
  }
  uint32_t max_width, max_height;
  if (tiled) {
    max_width = Load(base, device + 12916);
    max_height = Load(base, device + 12920);
  } else {
    const uint32_t size = Load(base, surface + 36);
    max_height = (Rotl(size, 29) & 0x7FFF) + 1;
    max_width = (Rotl(size, 14) & 0x3FFF) + 1;
  }
  const uint32_t x = Load(base, viewport + 0), y = Load(base, viewport + 4);
  uint32_t right = Load(base, viewport + 8) + x;
  if (!(right < max_width)) {
    right = max_width;
  }
  uint32_t bottom = Load(base, viewport + 12) + y;
  if (!(bottom < max_height)) {
    bottom = max_height;
  }
  const uint32_t width = right - x, height = bottom - y;
  Store(base, device + 12392, x);
  Store(base, device + 12396, y);
  Store(base, device + 12400, width);
  Store(base, device + 12404, height);
  Store(base, device + 12408, Load(base, viewport + 16));  // min and max Z
  Store(base, device + 12412, Load(base, viewport + 20));
  SetScissorRect(base, device, device + 12416);
  // PA_CL_VPORT_XSCALE ... _ZOFFSET.
  const float half = LoadF(base, 0x82001BE4);
  const float x_scale = float(double(width)) * half;
  const float y_scale = float(double(height)) * half;
  guest::StoreFloat(base, device + 10376, x_scale);
  guest::StoreFloat(base, device + 10380, float(double(x)) + x_scale);
  guest::StoreFloat(base, device + 10384, -y_scale);
  guest::StoreFloat(base, device + 10388, float(double(y)) + y_scale);
  guest::StoreFloat(base, device + 10392,
                    LoadF(base, viewport + 20) - LoadF(base, viewport + 16));
  Store(base, device + 10396, Load(base, viewport + 16));
  Mark(base, device + kRenderDirty, 0xFC);
}

void SetSurfaceInfo(uint8_t* base, uint32_t device, uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_822A3C88 Device::SetSurfaceInfo");
  const uint32_t info = Load(base, surface + 24);
  const uint32_t msaa = (info >> 16) & 3;
  Store(base, device + 10556, msaa == 0 ? 0 : msaa == 1 ? 0x8001 : 0xC003);  // the AA config
  Store(base, device + 10240, info);  // RB_SURFACE_INFO
  Mark(base, device + kRenderDirty, 1ull << 38);
  Mark(base, device + kOtherDirty, 1ull << 36);
  for (uint32_t i = 0; i < 16; i += 4) {
    Store(base, device + 12416 + i, Load(base, 0x82013618 + i));  // the whole surface
  }
  SetViewport(base, device, 0x82013600);
}

uint32_t GetViewportEnable(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_822A2470 Device::GetRenderState_ViewportEnable");
  return Load(base, device + kPaClVteCntl) & 1;
}

}  // namespace graphics::device_state
