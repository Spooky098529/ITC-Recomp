// The graphics device's state setters (docs/functions/device-state.md): see
// device_state.h. Every write matches the original's, word for word (the
// register shadow's fields are put in with the original's rotate-and-mask,
// so a value wider than its field spills the same way); the originals' stack
// temporaries (the float conversions' words below r1) aren't written.

#include "graphics/device_state.h"

#include "guest.h"

#include "base/log.h"

#include <emmintrin.h>

#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>

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

uint32_t GetViewportEnable(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_822A2470 Device::GetRenderState_ViewportEnable");
  return Load(base, device + kPaClVteCntl) & 1;
}

}  // namespace graphics::device_state
