// The Xbox 360 GPU's data formats the renderer reads from the game: a
// texture fetch constant's fields and the enumerations in them, and the
// render target formats (docs/pc-renderer.md, step 5: the renderer's own,
// in place of the SDK's xenos.h). Only what the renderer uses.

#pragma once

#include <cstdint>

namespace native::xenos {

enum class Endian : uint32_t { kNone = 0, k8in16 = 1, k8in32 = 2, k16in32 = 3 };

enum class TextureFormat : uint32_t {
  k_1_REVERSE = 0, k_1 = 1, k_8 = 2, k_1_5_5_5 = 3, k_5_6_5 = 4, k_6_5_5 = 5, k_8_8_8_8 = 6,
  k_2_10_10_10 = 7, k_8_A = 8, k_8_B = 9, k_8_8 = 10, k_Cr_Y1_Cb_Y0_REP = 11,
  k_Y1_Cr_Y0_Cb_REP = 12, k_16_16_EDRAM = 13, k_8_8_8_8_A = 14, k_4_4_4_4 = 15,
  k_10_11_11 = 16, k_11_11_10 = 17, k_DXT1 = 18, k_DXT2_3 = 19, k_DXT4_5 = 20,
  k_16_16_16_16_EDRAM = 21, k_24_8 = 22, k_24_8_FLOAT = 23, k_16 = 24, k_16_16 = 25,
  k_16_16_16_16 = 26, k_16_EXPAND = 27, k_16_16_EXPAND = 28, k_16_16_16_16_EXPAND = 29,
  k_16_FLOAT = 30, k_16_16_FLOAT = 31, k_16_16_16_16_FLOAT = 32, k_32 = 33, k_32_32 = 34,
  k_32_32_32_32 = 35, k_32_FLOAT = 36, k_32_32_FLOAT = 37, k_32_32_32_32_FLOAT = 38,
  k_32_AS_8 = 39, k_32_AS_8_8 = 40, k_16_MPEG = 41, k_16_16_MPEG = 42, k_8_INTERLACED = 43,
  k_32_AS_8_INTERLACED = 44, k_32_AS_8_8_INTERLACED = 45, k_16_INTERLACED = 46,
  k_16_MPEG_INTERLACED = 47, k_16_16_MPEG_INTERLACED = 48, k_DXN = 49,
  k_8_8_8_8_AS_16_16_16_16 = 50, k_DXT1_AS_16_16_16_16 = 51, k_DXT2_3_AS_16_16_16_16 = 52,
  k_DXT4_5_AS_16_16_16_16 = 53, k_2_10_10_10_AS_16_16_16_16 = 54,
  k_10_11_11_AS_16_16_16_16 = 55, k_11_11_10_AS_16_16_16_16 = 56, k_32_32_32_FLOAT = 57,
  k_DXT3A = 58, k_DXT5A = 59, k_CTX1 = 60, k_DXT3A_AS_1_1_1_1 = 61,
  k_8_8_8_8_GAMMA_EDRAM = 62, k_2_10_10_10_FLOAT_EDRAM = 63,
};

enum class ColorRenderTargetFormat : uint32_t {
  k_8_8_8_8 = 0, k_8_8_8_8_GAMMA = 1, k_2_10_10_10 = 2, k_2_10_10_10_FLOAT = 3, k_16_16 = 4,
  k_16_16_16_16 = 5, k_16_16_FLOAT = 6, k_16_16_16_16_FLOAT = 7,
  k_2_10_10_10_AS_10_10_10_10 = 10, k_2_10_10_10_FLOAT_AS_16_16_16_16 = 12, k_32_FLOAT = 14,
  k_32_32_FLOAT = 15,
};

enum class DataDimension : uint32_t { k1D = 0, k2DOrStacked = 1, k3D = 2, kCube = 3 };

enum class ClampMode : uint32_t {
  kRepeat = 0, kMirroredRepeat = 1, kClampToEdge = 2, kMirrorClampToEdge = 3,
  kClampToHalfway = 4, kMirrorClampToHalfway = 5, kClampToBorder = 6, kMirrorClampToBorder = 7,
};

enum class TextureFilter : uint32_t { kPoint = 0, kLinear = 1, kBaseMap = 2, kUseFetchConst = 3 };

enum class AnisoFilter : uint32_t {
  kDisabled = 0, kMax_1_1 = 1, kMax_2_1 = 2, kMax_4_1 = 3, kMax_8_1 = 4, kMax_16_1 = 5,
  kUseFetchConst = 7,
};

enum class TextureSign : uint32_t { kUnsigned = 0, kSigned = 1, kUnsignedBiased = 2, kGamma = 3 };

enum class FetchConstantType : uint32_t {
  kInvalidTexture = 0, kInvalidVertex = 1, kTexture = 2, kVertex = 3,
};

// Vertex indices are 24 bits.
constexpr uint32_t kVertexIndexMask = (1u << 24) - 1;
// The polygon offset's slope is in 1/16 of a pixel.
constexpr float kPolygonOffsetScaleSubpixelUnit = 1.0f / 16.0f;

// A texture fetch constant: six words, in host order.
union alignas(uint32_t) xe_gpu_texture_fetch_t {
  struct {
    uint32_t dword_0, dword_1, dword_2, dword_3, dword_4, dword_5;
  };
  struct {
    FetchConstantType type : 2;  // dword_0
    TextureSign sign_x : 2;      // signs of the data's components (before the swizzle)
    TextureSign sign_y : 2;
    TextureSign sign_z : 2;
    TextureSign sign_w : 2;
    ClampMode clamp_x : 3;
    ClampMode clamp_y : 3;
    ClampMode clamp_z : 3;
    uint32_t _pad_0_19 : 3;
    uint32_t pitch : 9;  // row pitch in pixels >> 5
    uint32_t tiled : 1;

    TextureFormat format : 6;  // dword_1
    Endian endianness : 2;
    uint32_t request_size : 2;
    uint32_t stacked : 1;
    uint32_t nearest_clamp_policy : 1;
    uint32_t base_address : 20;  // >> 12

    union {  // dword_2: each size less 1
      struct {
        uint32_t width : 24;
        uint32_t _pad : 8;
      } size_1d;
      struct {
        uint32_t width : 13;
        uint32_t height : 13;
        uint32_t stack_depth : 6;
      } size_2d;
      struct {
        uint32_t width : 11;
        uint32_t height : 11;
        uint32_t depth : 10;
      } size_3d;
    };

    uint32_t num_format : 1;  // dword_3
    uint32_t swizzle : 12;    // xyzw, 3 bits each
    int32_t exp_adjust : 6;
    TextureFilter mag_filter : 2;
    TextureFilter min_filter : 2;
    TextureFilter mip_filter : 2;
    AnisoFilter aniso_filter : 3;
    uint32_t arbitrary_filter : 3;
    uint32_t border_size : 1;

    uint32_t vol_mag_filter : 1;  // dword_4
    uint32_t vol_min_filter : 1;
    uint32_t mip_min_level : 4;
    uint32_t mip_max_level : 4;
    uint32_t mag_aniso_walk : 1;
    uint32_t min_aniso_walk : 1;
    int32_t lod_bias : 10;
    int32_t grad_exp_adjust_h : 5;
    int32_t grad_exp_adjust_v : 5;

    uint32_t border_color : 2;  // dword_5
    uint32_t force_bc_w_to_max : 1;
    uint32_t tri_clamp : 2;
    int32_t aniso_bias : 4;
    DataDimension dimension : 2;
    uint32_t packed_mips : 1;
    uint32_t mip_address : 20;  // >> 12
  };
};
static_assert(sizeof(xe_gpu_texture_fetch_t) == 24);

// The sign of each component after the swizzle, 2 bits each (TextureSign):
// a constant component (swizzle 4-7) takes the others' sign when they're all
// signed (or, all constant, the data's when all four are signed).
inline uint8_t SwizzleSigns(const xe_gpu_texture_fetch_t& fetch) {
  uint8_t signs = 0, constant_mask = 0;
  bool any_not_signed = false, any_signed = false;
  for (uint32_t i = 0; i < 4; ++i) {
    uint32_t swizzle = (fetch.swizzle >> (i * 3)) & 0b111;
    if (swizzle & 0b100) {
      constant_mask |= uint8_t(1) << (i * 2);
    } else {
      auto sign = TextureSign((fetch.dword_0 >> (2 + swizzle * 2)) & 0b11);
      signs |= uint8_t(sign) << (i * 2);
      (sign == TextureSign::kSigned ? any_signed : any_not_signed) = true;
    }
  }
  TextureSign constants_sign = TextureSign::kUnsigned;
  if (constant_mask == 0b01010101) {
    if (((fetch.dword_0 >> 2) & 0xFF) == uint32_t(TextureSign::kSigned) * 0b01010101) {
      constants_sign = TextureSign::kSigned;
    }
  } else if (any_signed && !any_not_signed) {
    constants_sign = TextureSign::kSigned;
  }
  return uint8_t(signs | uint8_t(constants_sign) * constant_mask);
}

}  // namespace native::xenos
