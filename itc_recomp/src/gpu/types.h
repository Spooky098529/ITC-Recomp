// The renderer's own values for draw and sampler state
// (docs/d3d11-backend.md, step 2): what `render::` holds and the renderer's
// logic works with, independent of the graphics API. Each backend maps them
// to its own (the Vulkan one in src/native/vulkan_types.h).

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

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
  kCube,  // layers: 6 (or a multiple)
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

// A view of all of an image's levels and layers, its components from
// `swizzle` (the image's R, G, B or A, or a constant).
enum class Swizzle : uint8_t { kR, kG, kB, kA, kZero, kOne };
struct ViewDesc {
  ImageType type = ImageType::k2D;
  Swizzle swizzle[4] = {Swizzle::kR, Swizzle::kG, Swizzle::kB, Swizzle::kA};
  // A depth image's depth alone.
  bool depth_only = false;
};

// What a buffer may be used for (bits).
enum BufferUsageBits : uint32_t {
  kBufferVertex = 1,
  kBufferIndex = 2,
  kBufferUniform = 4,
  kBufferStorage = 8,
  kBufferCopySource = 16,
  kBufferCopyDest = 32,
};

struct BufferDesc {
  uint64_t size = 0;
  uint32_t usage = 0;  // BufferUsageBits
  // In memory the CPU writes (and reads) directly, mapped for its life;
  // else the GPU's own.
  bool host_visible = false;
};

// An upload's region: from a staging buffer into an image (rows of
// `row_texels`, `rows` of them a layer; texels, or 4x4 blocks' texels for a
// compressed format) or into a buffer.
struct ImageUpload {
  uint64_t offset = 0;
  uint32_t row_texels = 0, rows = 0;
  uint32_t level = 0, layer = 0;
  uint32_t width = 0, height = 0;
};
struct BufferUpload {
  uint64_t source_offset = 0, dest_offset = 0, size = 0;
};

// A shader: its code (SPIR-V) and what it's bound with. Its textures and
// samplers are its stage's set (the vertex shader's 2, the pixel shader's
// 3): the textures' types in binding order, then the samplers.
enum class ShaderStage : uint8_t { kVertex, kPixel };
struct VertexBinding {
  uint32_t binding = 0, stride = 0;
  bool per_instance = false;
};
struct VertexAttribute {
  uint32_t location = 0, binding = 0;
  Format format = Format::kUndefined;
  uint32_t offset = 0;
};
struct ShaderDesc {
  ShaderStage stage = ShaderStage::kVertex;
  const uint32_t* spirv = nullptr;
  size_t spirv_words = 0;
  std::vector<ImageType> textures;
  uint32_t samplers = 0;
  // A vertex shader's vertex input.
  std::vector<VertexBinding> bindings;
  std::vector<VertexAttribute> attributes;
};

// The order Vulkan numbers them.
enum class Topology : uint8_t {
  kPointList,
  kLineList,
  kLineStrip,
  kTriangleList,
  kTriangleStrip,
  kTriangleFan,
};

struct Shader;

// A draw's pipeline: its shaders and fixed-function state. Plain 32-bit
// fields (the enums' values) and no padding, so it's hashed and compared
// as bytes: zero it before filling. A draw is bound with its shaders'
// texture sets (2 and 3) and the four constant blocks (set 1); viewport,
// scissor, depth bias and blend constants are set per draw.
struct PipelineDesc {
  Shader* vertex;
  Shader* pixel;  // null: depth only
  uint32_t topology, primitive_restart;  // Topology
  uint32_t cull_mode, front_clockwise;   // CullMode
  uint32_t z_enable, z_write, z_func;    // CompareOp
  uint32_t blend_enable, src_color, dst_color, color_op, src_alpha, dst_alpha,
      alpha_op;  // BlendFactor, BlendOp
  uint32_t write_mask;  // RGBA bits
  // The target: its color format (kUndefined: none), a depth target or
  // not, the samples.
  uint32_t color_format, depth, samples;
  bool operator==(const PipelineDesc&) const = default;
};
static_assert(sizeof(PipelineDesc) == 2 * sizeof(void*) + 18 * sizeof(uint32_t));

// A pass's target: its image (in its target usage), what the pass starts
// from and keeps. The clear value as bits: four floats or integers for
// color; depth and stencil.
enum class LoadOp : uint8_t { kLoad, kClear, kDontCare };
enum class StoreOp : uint8_t { kStore, kDontCare };
struct PassTarget {
  struct Image* image = nullptr;
  LoadOp load = LoadOp::kLoad;
  StoreOp store = StoreOp::kStore;
  uint32_t clear_color[4] = {};
  float clear_depth = 0.0f;
  uint32_t clear_stencil = 0;
};
struct Rect {
  int32_t x = 0, y = 0;
  uint32_t width = 0, height = 0;
};
struct Viewport {
  float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
  float min_depth = 0.0f, max_depth = 1.0f;
};

// A resolve drawn (Backend::DrawResolve): each pixel of `area` in `dest`
// from `source`'s pixel at the same place plus `offset`, its first
// `samples` averaged (a multisampled source; a depth image's depth),
// times `scale`, red and blue exchanged if asked.
struct ResolveDraw {
  struct Image* source = nullptr;
  struct Image* dest = nullptr;
  Rect area;
  int32_t offset[2] = {};
  int32_t samples = 1;
  float scale = 1.0f;
  bool swap_red_blue = false;
};

// The backends' objects: opaque, each backend's own behind them.
struct TextureSet;
struct Pipeline;
struct Buffer;
struct Sampler;
struct Image;
struct View;

}  // namespace gpu
