// The renderer's own values for draw and sampler state
// (docs/d3d11-backend.md, step 2): what `render::` holds and the renderer's
// logic works with, independent of the graphics API. Each backend maps them
// to its own (the Vulkan one in src/native/vulkan_types.h).

#pragma once

#include <cstdint>

namespace gpu {

// A depth or alpha test's compare, in the order the game's GPU numbers them
// (D3D's minus one): the draw constants' alpha test flag passes this number
// to the shaders as is.
enum class CompareOp : uint8_t {
  kNever = 0,
  kLess = 1,
  kEqual = 2,
  kLessOrEqual = 3,
  kGreater = 4,
  kNotEqual = 5,
  kGreaterOrEqual = 6,
  kAlways = 7,
};

enum class BlendFactor : uint8_t {
  kZero,
  kOne,
  kSrcColor,
  kOneMinusSrcColor,
  kSrcAlpha,
  kOneMinusSrcAlpha,
  kDstColor,
  kOneMinusDstColor,
  kDstAlpha,
  kOneMinusDstAlpha,
  kConstantColor,
  kOneMinusConstantColor,
  kConstantAlpha,
  kOneMinusConstantAlpha,
  kSrcAlphaSaturate,
};

enum class BlendOp : uint8_t {
  kAdd,
  kSubtract,
  kReverseSubtract,
  kMin,
  kMax,
};

// Which faces are culled (bits: 1 front, 2 back; both culls everything).
enum class CullMode : uint8_t {
  kNone = 0,
  kFront = 1,
  kBack = 2,
  kFrontAndBack = 3,
};

enum class Filter : uint8_t {
  kNearest,
  kLinear,
};

enum class MipMode : uint8_t {
  kNearest,
  kLinear,
};

enum class AddressMode : uint8_t {
  kRepeat,
  kMirroredRepeat,
  kClampToEdge,
  kClampToBorder,
  // Mirrored once, then clamped to the edge (D3D's MIRROR_ONCE).
  kMirrorClampToEdge,
};

enum class BorderColor : uint8_t {
  kTransparentBlack,
  kOpaqueWhite,
};

// A sampler. The backend applies its device's limits (anisotropy it lacks
// is off, a mirror clamp to edge it lacks repeats mirrored).
struct SamplerDesc {
  Filter mag_filter = Filter::kNearest, min_filter = Filter::kNearest;
  MipMode mip_mode = MipMode::kNearest;
  AddressMode address[3] = {AddressMode::kRepeat, AddressMode::kRepeat, AddressMode::kRepeat};
  // 0: off.
  float max_anisotropy = 0.0f;
  float mip_lod_bias = 0.0f;
  // The levels sampled (a max of 1000: all).
  float min_lod = 0.0f, max_lod = 0.0f;
  BorderColor border = BorderColor::kTransparentBlack;
};

// Image and vertex formats, by their components' order in memory.
enum class Format : uint8_t {
  kUndefined,
  kR8Unorm,
  kR8G8Unorm,
  kR8G8B8A8Unorm,
  kA2B10G10R10Unorm,  // packed in 32 bits, R in the low bits
  kR16Unorm,
  kR16G16Unorm,
  kR16G16B16A16Unorm,
  kR16Float,
  kR16G16Float,
  kR16G16B16A16Float,
  kR32Float,
  kR32G32Float,
  kR32G32B32A32Float,
  kR32Uint,
  kR32G32Uint,
  kR32G32B32Uint,
  kR32G32B32A32Uint,
  kD32FloatS8Uint,
  kBC1,  // RGBA
  kBC2,
  kBC3,
  kBC5,
};

enum class ImageType : uint8_t {
  k2D,
  k2DArray,  // a 2D image viewed as an array (of `layers`)
  k3D,
  kCube,  // 6 layers
};

// What an image may be used for (bits).
enum ImageUsageBits : uint32_t {
  kImageSampled = 1,
  kImageColorTarget = 2,
  kImageDepthTarget = 4,
  kImageCopySource = 8,
  kImageCopyDest = 16,
  kImageStorage = 32,
};

struct ImageDesc {
  ImageType type = ImageType::k2D;
  Format format = Format::kUndefined;
  uint32_t width = 1, height = 1, depth = 1;
  uint32_t layers = 1, levels = 1, samples = 1;
  uint32_t usage = 0;  // ImageUsageBits
};

// What an image is about to be used for (docs/d3d11-backend.md, "Usage, not
// layouts"): Vulkan changes its layout when it differs from the last use.
enum class Usage : uint8_t {
  // Its contents may be dropped: no barrier now, the next use starts from
  // nothing.
  kUndefined,
  kSampled,
  kDepthSampled,  // a depth image sampled as depth
  kColorTarget,
  kDepthTarget,
  kCopySource,
  kCopyDest,
  kStorage,
};

// The backends' objects: opaque, each backend's own behind them.
struct Sampler;
struct Image;

}  // namespace gpu
