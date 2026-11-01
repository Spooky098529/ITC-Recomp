// The translator's registers from our interface's state: see
// interface_registers.h. The register numbers are the 360's (the device's
// shadow offsets in src/native/state.cpp's blocks).

#include "interface_registers.h"

#include "guest.h"

#include "base/log.h"

#include <array>
#include <bit>
#include <climits>
#include <cmath>
#include <cstring>
#include <emmintrin.h>

namespace native {

namespace {

constexpr uint32_t kAnisotropyTable = 0x82013628;     // max anisotropy -> the filter's bits
constexpr uint32_t kMipMapLodBiasScale = 0x820029CC;  // the LOD bias to fixed point
constexpr uint32_t kAnisotropyBiasScale = 0x82000F18;
constexpr uint32_t kTimes16 = 0x820021EC;  // the depth slope's scale
constexpr uint32_t kHalf = 0x82001BE4;     // the viewport's half

enum : uint32_t {
  kRbSurfaceInfo = 0x2000,
  kRbColorInfo = 0x2001,
  kRbDepthInfo = 0x2002,
  kRbColor1Info = 0x2003,
  kVgtIndexOffset = 0x2102,
  kVgtResetIndex = 0x2103,
  kRbColorMask = 0x2104,
  kRbBlendRed = 0x2105,
  kRbStencilRefMaskBack = 0x210C,
  kRbStencilRefMask = 0x210D,
  kRbAlphaRef = 0x210E,
  kPaClViewportXScale = 0x210F,
  kSqProgramControl = 0x2180,
  kSqContextMisc = 0x2181,
  kSqInterpolatorControl = 0x2182,
  kSqWrapping0 = 0x2183,
  kRbDepthControl = 0x2200,
  kRbBlendControl0 = 0x2201,
  kRbColorControl = 0x2202,
  kPaClClipControl = 0x2204,
  kPaSuScModeControl = 0x2205,
  kPaClVteControl = 0x2206,
  kRbModeControl = 0x2208,
  kRbBlendControl1 = 0x2209,
  kPaSuVertexControl = 0x2302,
  kPaSuPolyOffsetFrontScale = 0x2380,
  kFetchConstants = 0x4800,
  kBoolConstants = 0x4900,
  kLoopConstants = 0x4908,
};

constexpr uint32_t kChecked[] = {
    kRbSurfaceInfo, kRbColorInfo, kRbDepthInfo, kVgtIndexOffset, kVgtResetIndex, kRbColorMask,
    kRbBlendRed, kRbBlendRed + 1, kRbBlendRed + 2, kRbBlendRed + 3, kRbStencilRefMaskBack,
    kRbStencilRefMask, kRbAlphaRef, kPaClViewportXScale, kPaClViewportXScale + 1,
    kPaClViewportXScale + 2, kPaClViewportXScale + 3, kPaClViewportXScale + 4,
    kPaClViewportXScale + 5, kSqProgramControl, kSqContextMisc, kSqInterpolatorControl,
    kSqWrapping0, kSqWrapping0 + 1, kRbDepthControl, kRbBlendControl0, kRbColorControl,
    kPaClClipControl, kPaSuScModeControl, kPaClVteControl, kRbModeControl, kPaSuVertexControl,
    kPaSuPolyOffsetFrontScale, kPaSuPolyOffsetFrontScale + 1, kPaSuPolyOffsetFrontScale + 2,
    kPaSuPolyOffsetFrontScale + 3,
};

// A guest address as the GPU's physical one.
uint32_t Physical(uint32_t address) {
  return ((((address >> 20) & 0xFFF) + 512) & 0x1000) + (address & 0x1FFFFFFF);
}

uint32_t Bits(float f) {
  return std::bit_cast<uint32_t>(f);
}

// fctidz as the recompiled code does it.
int64_t ToInt64(double d) {
  if (std::isnan(d)) {
    return INT64_MIN;
  }
  if (d > double(LLONG_MAX)) {
    return LLONG_MAX;
  }
  return _mm_cvttsd_si64(_mm_set_sd(d));
}

// A color surface's RB_COLOR_INFO with the target's high-precision blend
// applied to its format (2 and 10, 3 and 12), as SetRenderTarget does.
uint32_t ColorInfo(uint8_t* base, uint32_t surface, bool high_precision) {
  const uint32_t info = guest::LoadU32(base, surface + 28);
  const uint32_t format = (info >> 16) & 0xF;
  if (format != 2 && format != 3 && format != 10 && format != 12) {
    return info;
  }
  if (bool((info >> 19) & 1) == high_precision) {
    return info;
  }
  const uint32_t switched = high_precision ? (format + 3) << 1 : (format >> 1) - 3;
  return (info & ~0xF0000u) | ((switched & 0xF) << 16);
}

uint32_t EncodeBlend(const render::Blend& b) {
  return uint32_t(b.src_color) | uint32_t(b.color_op) << 5 | uint32_t(b.dst_color) << 8 |
         uint32_t(b.src_alpha) << 16 | uint32_t(b.alpha_op) << 21 | uint32_t(b.dst_alpha) << 24;
}

uint32_t StencilRefMask(const render::StencilFace& f) {
  return uint32_t(f.reference) | uint32_t(f.read_mask) << 8 | uint32_t(f.write_mask) << 16;
}

// Word 4's low two bits from word 3's filters and the z filters (the
// game's steps, src/graphics/device_state.cpp).
uint32_t ZFilterBits(uint32_t word3, uint32_t z) {
  uint32_t bits = (std::rotl(word3, 31) & 0x7FFFF) | (word3 & 0xFFF80000u);
  bits = (std::rotl(word3, 31) & 0x7FF00000u) | (bits & 0x800FFFFFu);
  const uint32_t filters = std::rotl(bits, 13) & 0xFFF;
  const uint32_t m = (z >> 2) - 1;
  return (filters & m) + (z & ~m);
}

}  // namespace

std::span<const uint32_t> InterfaceRegisters::CheckedRegisters() {
  return kChecked;
}

void InterfaceRegisters::BuildTextureFetch(uint8_t* base, const render::State& state,
                                           uint32_t stage, uint32_t* words) {
  const render::Sampler& s = state.samplers[stage];
  // The sampler's part, as its states set it.
  const uint32_t mag_aniso = (s.mag_filter >> 2) & 1, min_aniso = (s.min_filter >> 2) & 1;
  uint32_t word3 = (((s.mag_filter >> 2) | s.mag_filter) & 3) << 19 |
                   (((s.min_filter >> 2) | s.min_filter) & 3) << 21 | uint32_t(s.mip_filter & 3)
                                                                            << 23;
  if (mag_aniso || min_aniso) {
    word3 |= (guest::LoadU32(base, kAnisotropyTable + 4 * s.max_anisotropy) & 7) << 25;
  }
  const uint32_t z = (s.mag_filter_z & 1) | (s.min_filter_z & 1) << 1 |
                     uint32_t(s.separate_z_filter) << 2;
  const uint32_t lod_bias =
      uint32_t(ToInt64(double(s.lod_bias * guest::LoadFloat(base, kMipMapLodBiasScale))));
  uint32_t word4 = (ZFilterBits(word3, z) & 3) | mag_aniso << 10 | min_aniso << 11 |
                   (lod_bias & 0x3FF) << 12 | uint32_t(s.h_gradient_exp_bias & 0x1F) << 22 |
                   uint32_t(s.v_gradient_exp_bias & 0x1F) << 27;
  const uint32_t anisotropy_bias = uint32_t(
      ToInt64(double(s.anisotropy_bias * guest::LoadFloat(base, kAnisotropyBiasScale))));
  const uint32_t word5 = (s.border_color != 0 ? 1 : 0) | uint32_t(s.white_border_w) << 2 |
                         uint32_t(s.trilinear_threshold & 3) << 3 | (anisotropy_bias & 0xF) << 5;
  const uint32_t word0 = uint32_t(s.address_u) << 10 | uint32_t(s.address_v) << 13 |
                         uint32_t(s.address_w) << 16;
  const uint32_t word1 = s.point_border ? 0 : 0x800;

  // The texture's part (SetTexture), kept while none is bound.
  uint32_t* t = texture_words_[stage];
  uint32_t own_max = 0, own_min = 15;
  bool bound = false;
  if (const uint32_t texture = state.textures[stage].address) {
    bound = true;
    t[0] = guest::LoadU32(base, texture + 28);
    t[1] = Physical(guest::LoadU32(base, texture + 32));
    t[2] = guest::LoadU32(base, texture + 36);
    t[3] = guest::LoadU32(base, texture + 40);
    t[4] = guest::LoadU32(base, texture + 44);
    const uint32_t mip = guest::LoadU32(base, texture + 48);
    t[5] = ((((mip >> 20) & 0xFFF) + 512) & 0x1000) + (mip & 0x1FFFFE00);
  }
  own_max = (t[4] >> 2) & 0xF;
  own_min = (t[4] >> 6) & 0xF;
  words[0] = (word0 & 0x3FFC00) | (t[0] & ~0x3FFC00u);
  words[1] = (word1 & 0x800) | (t[1] & ~0x800u);
  words[2] = t[2];
  words[3] = (word3 & 0x7FF80000) | (t[3] & ~0x7FF80000u);
  // The mip levels: the larger of the sampler's and the texture's highest,
  // the smaller of their lowest.
  const uint32_t max_level = std::max<uint32_t>(s.max_mip_level, own_max);
  const uint32_t min_level = std::min<uint32_t>(s.min_mip_level, own_min);
  word4 = (word4 & 0xFFFFFC03) | (max_level & 0xF) << 2 | (min_level & 0xF) << 6;
  words[4] = word4;
  words[5] = (word5 & 0x1FF) | (t[5] & ~0x1FFu);
  (void)bound;
}

void InterfaceRegisters::Build(uint8_t* base, render::State& state, int32_t base_vertex,
                               rex::graphics::RegisterFile& regs) {
  uint32_t* r = regs.values;
  const uint32_t dirty = built_ ? state.dirty : render::kDirtyAll;
  built_ = true;
  state.dirty = 0;
  auto set_float = [&](uint32_t index, float f) { r[index] = Bits(f); };

  if (dirty & (render::kDirtyTargets | render::kDirtyBlend)) {
    if (const uint32_t rt0 = state.targets[0].address) {
      r[kRbSurfaceInfo] = guest::LoadU32(base, rt0 + 24);
    } else if (state.depth) {
      r[kRbSurfaceInfo] = guest::LoadU32(base, state.depth.address + 24);
    }
    for (uint32_t i = 0; i < render::kTargets; i++) {
      if (const uint32_t surface = state.targets[i].address) {
        r[i == 0 ? kRbColorInfo : kRbColor1Info + i - 1] =
            ColorInfo(base, surface, state.high_precision_blend[i]);
      }
    }
    if (state.depth) {
      r[kRbDepthInfo] = guest::LoadU32(base, state.depth.address + 28);
    }
    uint32_t mask = 0;
    for (uint32_t i = 0; i < render::kTargets; i++) {
      if (state.targets[i]) {
        mask |= uint32_t(state.write_mask[i] & 0xF) << (4 * i);
      }
    }
    r[kRbColorMask] = mask;
    r[kRbBlendControl0] = EncodeBlend(state.blend[0]);
    for (uint32_t i = 1; i < render::kTargets; i++) {
      r[kRbBlendControl1 + i - 1] = EncodeBlend(state.blend[i]);
    }
  }
  r[kVgtIndexOffset] = uint32_t(base_vertex);
  if (dirty & render::kDirtyRaster) {
    const render::Raster& raster = state.raster;
    r[kVgtResetIndex] = raster.restart_index;
    const float slope = raster.slope_scaled_depth_bias * guest::LoadFloat(base, kTimes16);
    const bool offset = slope != 0.0f || raster.depth_bias != 0.0f;
    r[kPaSuScModeControl] = uint32_t(raster.cull_front) | uint32_t(raster.cull_back) << 1 |
                            uint32_t(raster.front_clockwise) << 2 |
                            uint32_t(raster.fill_mode) << 3 | uint32_t(offset) << 11 |
                            uint32_t(offset) << 12 | uint32_t(raster.multisample) << 15 |
                            uint32_t(raster.primitive_restart) << 21 |
                            0x10000;  // vtx_window_offset_enable, always set by the game
    set_float(kPaSuPolyOffsetFrontScale, slope);
    set_float(kPaSuPolyOffsetFrontScale + 1, raster.depth_bias);
    set_float(kPaSuPolyOffsetFrontScale + 2, slope);
    set_float(kPaSuPolyOffsetFrontScale + 3, raster.depth_bias);
    r[kPaClVteControl] = raster.screen_space ? 0x400 : 0x43F;
    // dx_clip_space_def (bit 19) and the round mode (bit 2) as the game's
    // device always has them.
    r[kPaClClipControl] =
        uint32_t(raster.clip_planes) | (raster.screen_space ? 0x10000 : 0) | 0x80000;
    r[kPaSuVertexControl] = uint32_t(raster.half_pixel_offset) | 0x4;
  }
  if (dirty & render::kDirtyBlendColor) {
    for (uint32_t c = 0; c < 4; c++) {
      set_float(kRbBlendRed + c, state.blend_color[c]);
    }
  }
  if (dirty & (render::kDirtyDepthStencil | render::kDirtyTargets)) {
    const render::DepthStencil& ds = state.depth_stencil;
    const bool depth = bool(state.depth);
    r[kRbDepthControl] =
        uint32_t(depth && ds.stencil) | uint32_t(depth && ds.depth_test) << 1 |
        uint32_t(ds.depth_write) << 2 | uint32_t(ds.depth_compare) << 4 |
        uint32_t(ds.two_sided) << 7 | uint32_t(ds.front.compare) << 8 |
        uint32_t(ds.front.fail) << 11 | uint32_t(ds.front.pass) << 14 |
        uint32_t(ds.front.depth_fail) << 17 | uint32_t(ds.back.compare) << 20 |
        uint32_t(ds.back.fail) << 23 | uint32_t(ds.back.pass) << 26 |
        uint32_t(ds.back.depth_fail) << 29;
    r[kRbStencilRefMask] = StencilRefMask(ds.front);
    r[kRbStencilRefMaskBack] = StencilRefMask(ds.back);
  }
  if (dirty & render::kDirtyAlphaTest) {
    const render::AlphaTest& a = state.alpha_test;
    r[kRbColorControl] = uint32_t(a.compare) | uint32_t(a.enable) << 3 |
                         uint32_t(a.alpha_to_coverage) << 4 |
                         uint32_t(a.alpha_to_coverage_offsets) << 24;
    set_float(kRbAlphaRef, a.reference);
  }
  if (dirty & (render::kDirtyViewport | render::kDirtyTargets)) {
    // As SetViewport makes PA_CL_VPORT_*.
    const render::Viewport& v = state.viewport;
    const float half = guest::LoadFloat(base, kHalf);
    const float x_scale = float(double(v.width)) * half;
    const float y_scale = float(double(v.height)) * half;
    set_float(kPaClViewportXScale, x_scale);
    set_float(kPaClViewportXScale + 1, float(double(v.x)) + x_scale);
    set_float(kPaClViewportXScale + 2, -y_scale);
    set_float(kPaClViewportXScale + 3, float(double(v.y)) + y_scale);
    set_float(kPaClViewportXScale + 4, v.max_z - v.min_z);
    set_float(kPaClViewportXScale + 5, v.min_z);
  }
  if (dirty & render::kDirtyShaders) {
    // The shader load (sub_822AED18): the vertex and pixel shaders' program
    // control words ORed; the pixel shader's interpolators; depth-only
    // EDRAM mode without a pixel shader.
    uint32_t program = 0, misc = 0;
    if (const uint32_t vs = state.vertex_shader.address) {
      const uint32_t header = vs + 592 + guest::LoadU32(base, vs + 612);
      program = guest::LoadU32(base, header);
      misc = guest::LoadU32(base, header + 4);
    }
    if (const uint32_t ps = state.pixel_shader.address) {
      const uint32_t header = ps + 56 + guest::LoadU32(base, ps + 76);
      program |= guest::LoadU32(base, header);
      misc |= guest::LoadU32(base, header + 4);
      r[kSqInterpolatorControl] = guest::LoadU32(base, header + 24);
    }
    r[kSqProgramControl] = program;
    r[kSqContextMisc] = misc;
    r[kRbModeControl] = state.pixel_shader ? 4 : 5;
    uint32_t wrap[2] = {};
    for (uint32_t i = 0; i < 16; i++) {
      wrap[i / 8] |= uint32_t(state.wrap[i] & 0xF) << (4 * (i % 8));
    }
    r[kSqWrapping0] = wrap[0];
    r[kSqWrapping0 + 1] = wrap[1];
  }
  if (dirty & render::kDirtyTextures) {
    for (uint32_t stage = 0; stage < render::kStages; stage++) {
      BuildTextureFetch(base, state, stage, &r[kFetchConstants + 6 * stage]);
    }
  }
  if (dirty & render::kDirtyStreams) {
    // SetStreamSource: vertex fetch constant 95 - stream, made with a buffer
    // and kept without one.
    for (uint32_t stream = 0; stream < render::kStreams; stream++) {
      const render::Stream& s = state.streams[stream];
      if (s.buffer) {
        stream_words_[stream][0] = Physical(guest::LoadU32(base, s.buffer.address + 24) + s.offset);
        stream_words_[stream][1] = guest::LoadU32(base, s.buffer.address + 28) - s.offset;
      }
      r[kFetchConstants + 2 * (95 - stream)] = stream_words_[stream][0];
      r[kFetchConstants + 2 * (95 - stream) + 1] = stream_words_[stream][1];
    }
  }
  if (dirty & render::kDirtyBoolLoopConstants) {
    std::memcpy(&r[kBoolConstants], state.constants.bools.data(), 4 * 8);
    std::memcpy(&r[kLoopConstants], state.constants.loops.data(), 4 * 32);
  }
}

const std::vector<uint8_t>* InterfaceRegisters::PatchedVertexShader(uint8_t* base,
                                                                    const render::State& state,
                                                                    uint32_t inline_stride) {
  const uint32_t vs = state.vertex_shader.address;
  if (!vs) {
    return nullptr;
  }
  if (!tables_loaded_) {
    tables_ = LoadVertexFetchTables(base);
    tables_loaded_ = true;
  }
  std::vector<uint8_t> strides(16);
  for (uint32_t i = 0; i < 16; i++) {
    strides[i] = uint8_t(state.streams[i].stride >> 2);
  }
  if (inline_stride) {
    strides[0] = uint8_t(inline_stride >> 2);
  }
  const uint32_t microcode = guest::LoadU32(base, vs + 48);
  auto key = std::make_tuple(vs, microcode, state.declaration.address, strides);
  auto it = patched_.find(key);
  if (it != patched_.end()) {
    return &it->second;
  }
  const uint32_t size = guest::LoadU32(base, vs + 600);
  std::vector<uint8_t> code(base + microcode, base + microcode + size);
  GuestVertexFetchInputs inputs;
  if (state.declaration && ReadGuestVertexFetchInputs(base, vs, state.declaration.address, inputs)) {
    PatchVertexFetches(tables_, inputs.fetches(), inputs.declaration(), strides.data(),
                       code.data(), code.size());
  }
  return &patched_.emplace(std::move(key), std::move(code)).first->second;
}

}  // namespace native
