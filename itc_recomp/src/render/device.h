// Our renderer's interface (docs/conventional-renderer.md, section 5): what
// the game draws, as plain state and operations, with no graphics API's
// types. The command handlers (src/render/commands.cpp) set it from the
// game's command list; a backend (the Vulkan renderer in src/native/) reads
// the state at each operation.
//
// Resources are named by the game's objects: the guest addresses of its
// surfaces, textures, buffers, shaders and vertex declarations, whose
// headers the backend reads. Textures are made, written and freed through
// the interface (render/resources.h, step 2); buffers still are not (2b).
//
// Enum values are chosen to equal the game's (the Xbox 360's) encodings, so
// the handlers pass them through; a backend maps them by name.

#pragma once

#include <array>
#include <cstdint>

namespace render {

struct PassPlan;  // render/pass_plan.h

// A game object (a guest address), 0 for none.
struct Object {
  uint32_t address = 0;
  explicit operator bool() const { return address != 0; }
  bool operator==(const Object&) const = default;
};
struct Surface : Object {};
struct Texture : Object {};
struct VertexBuffer : Object {};
struct IndexBuffer : Object {};
// Either, as resources.h's events name them.
struct Buffer : Object {};
enum class BufferKind : uint8_t;
struct BufferData;
struct VertexShader : Object {};
struct PixelShader : Object {};
struct VertexDeclaration : Object {};

enum class Compare : uint8_t {
  kNever,
  kLess,
  kEqual,
  kLessEqual,
  kGreater,
  kNotEqual,
  kGreaterEqual,
  kAlways,
};

enum class BlendFactor : uint8_t {
  kZero = 0,
  kOne = 1,
  kSrcColor = 4,
  kInvSrcColor = 5,
  kSrcAlpha = 6,
  kInvSrcAlpha = 7,
  kDestColor = 8,
  kInvDestColor = 9,
  kDestAlpha = 10,
  kInvDestAlpha = 11,
  kConstantColor = 12,
  kInvConstantColor = 13,
  kConstantAlpha = 14,
  kInvConstantAlpha = 15,
  kSrcAlphaSaturate = 16,
};

enum class BlendOp : uint8_t { kAdd, kSubtract, kMin, kMax, kReverseSubtract };

enum class StencilOp : uint8_t {
  kKeep,
  kZero,
  kReplace,
  kIncrementClamp,
  kDecrementClamp,
  kInvert,
  kIncrementWrap,
  kDecrementWrap,
};

enum class Primitive : uint8_t {
  kPointList = 1,
  kLineList = 2,
  kLineStrip = 3,
  kTriangleList = 4,
  kTriangleFan = 5,
  kTriangleStrip = 6,
  kRectangleList = 8,
  kQuadList = 13,
};

// One render target's blending: ONE, ZERO, ADD for both when off.
struct Blend {
  BlendFactor src_color = BlendFactor::kOne, dst_color = BlendFactor::kZero;
  BlendOp color_op = BlendOp::kAdd;
  BlendFactor src_alpha = BlendFactor::kOne, dst_alpha = BlendFactor::kZero;
  BlendOp alpha_op = BlendOp::kAdd;
  bool operator==(const Blend&) const = default;
};

struct StencilFace {
  Compare compare = Compare::kAlways;
  StencilOp fail = StencilOp::kKeep, depth_fail = StencilOp::kKeep, pass = StencilOp::kKeep;
  uint8_t reference = 0, read_mask = 0xFF, write_mask = 0xFF;
};

struct DepthStencil {
  // Used only with a depth surface bound.
  bool depth_test = true, depth_write = true;
  Compare depth_compare = Compare::kLessEqual;
  bool stencil = false, two_sided = false;
  StencilFace front, back;
};

struct Raster {
  bool cull_front = false, cull_back = false;
  bool front_clockwise = false;
  // The game's fill mode bits (solid: 0).
  uint8_t fill_mode = 0;
  // Depth bias in the depth buffer's units, slope per pixel ×16 as the
  // game gives it (D3D's SlopeScaleDepthBias × 16).
  float depth_bias = 0.0f, slope_scaled_depth_bias = 0.0f;
  // Vertices already in pixels (the viewport transform off), as the game's
  // screen-space draws set it.
  bool screen_space = false;
  bool half_pixel_offset = false;
  bool primitive_restart = false;
  uint32_t restart_index = 0xFFFF;
  bool multisample = true;
  uint16_t sample_mask = 0xFFFF;
  uint8_t clip_planes = 0;
};

struct AlphaTest {
  bool enable = false;
  Compare compare = Compare::kAlways;
  float reference = 0.0f;  // 0..1
  bool alpha_to_coverage = false;
  uint8_t alpha_to_coverage_offsets = 0;
};

struct Viewport {
  uint32_t x = 0, y = 0, width = 0, height = 0;
  float min_z = 0.0f, max_z = 1.0f;
  bool operator==(const Viewport&) const = default;
};

struct Rect {
  int32_t left = 0, top = 0, right = 0, bottom = 0;
};

// A sampler, as the game's sampler states set it (their values).
enum class Address : uint8_t {
  kWrap,
  kMirror,
  kClamp,
  kMirrorOnce,
  kClampHalfway,
  kMirrorOnceHalfway,
  kBorder,
  kMirrorOnceBorder,
};
struct Sampler {
  Address address_u = Address::kWrap, address_v = Address::kWrap, address_w = Address::kWrap;
  uint32_t border_color = 0;
  // Mag and min: 0 point, 1 linear, 4 and up anisotropic (the game's
  // values); mip: 0 point, 1 linear, 2 none.
  uint8_t mag_filter = 0, min_filter = 0, mip_filter = 0;
  float lod_bias = 0.0f;
  uint8_t max_mip_level = 0, min_mip_level = 13;
  uint8_t max_anisotropy = 1;
  uint8_t mag_filter_z = 0, min_filter_z = 0;
  bool separate_z_filter = false;
  uint8_t trilinear_threshold = 0;
  float anisotropy_bias = 0.0f;
  uint8_t h_gradient_exp_bias = 0, v_gradient_exp_bias = 0;
  bool white_border_w = false, point_border = true;
};

struct Stream {
  VertexBuffer buffer;
  uint32_t offset = 0, stride = 0;
};

constexpr uint32_t kTargets = 4;
constexpr uint32_t kStages = 26;
constexpr uint32_t kStreams = 16;
constexpr uint32_t kFloatConstants = 256;

// Shader constants, in host byte order.
struct Constants {
  std::array<std::array<float, 4>, kFloatConstants> vertex, pixel;
  // Bool constants: a bit each, 128 vertex then 128 pixel (as words).
  std::array<uint32_t, 8> bools{};
  // Loop constants: 16 vertex then 16 pixel.
  std::array<uint32_t, 32> loops{};
};

// What changed since the backend last looked (it clears the bits it reads).
enum Dirty : uint32_t {
  kDirtyTargets = 1u << 0,
  kDirtyViewport = 1u << 1,
  kDirtyScissor = 1u << 2,
  kDirtyBlend = 1u << 3,
  kDirtyDepthStencil = 1u << 4,
  kDirtyRaster = 1u << 5,
  kDirtyAlphaTest = 1u << 6,
  kDirtyTextures = 1u << 7,  // textures or samplers
  kDirtyStreams = 1u << 8,
  kDirtyIndices = 1u << 9,
  kDirtyShaders = 1u << 10,  // shaders or the declaration
  kDirtyVertexConstants = 1u << 11,
  kDirtyPixelConstants = 1u << 12,
  kDirtyBoolLoopConstants = 1u << 13,
  kDirtyBlendColor = 1u << 14,
  kDirtyAll = ~0u,
};

struct State {
  std::array<Surface, kTargets> targets;
  Surface depth;
  Viewport viewport;
  bool scissor = false;
  Rect scissor_rect;
  std::array<Blend, kTargets> blend;
  std::array<uint8_t, kTargets> write_mask{0xF, 0xF, 0xF, 0xF};
  // The game's high-precision blending per target: the 360 switched the
  // surface's format between its two forms; drawn alike here.
  std::array<bool, kTargets> high_precision_blend{};
  std::array<float, 4> blend_color{};  // RGBA, 0..1
  DepthStencil depth_stencil;
  Raster raster;
  AlphaTest alpha_test;
  std::array<Texture, kStages> textures;
  std::array<Sampler, kStages> samplers;
  // The game's texture coordinate wrapping (cylindrical) per interpolator.
  std::array<uint8_t, 16> wrap{};
  std::array<Stream, kStreams> streams;
  IndexBuffer indices;
  VertexShader vertex_shader;
  PixelShader pixel_shader;
  VertexDeclaration declaration;
  Constants constants;
  uint32_t dirty = kDirtyAll;
};

// The game's primitive count: the index count is count × a + b (the
// table at 0x82000950).
struct DrawIndexed {
  Primitive primitive;
  int32_t base_vertex;
  uint32_t start_index, primitive_count;
};

// Inline vertices (in the game's byte order), drawn with stream 0's
// declaration and this stride.
struct DrawInline {
  Primitive primitive;
  const uint8_t* vertices;
  uint32_t vertex_count, stride;
};

// Which surfaces a clear clears (render targets 0-3, depth, stencil).
enum ClearFlags : uint32_t {
  kClearTarget0 = 1u << 0,
  kClearTargets = 0xF,
  kClearDepth = 1u << 4,
  kClearStencil = 1u << 5,
};
struct Clear {
  uint32_t flags = 0;
  uint32_t color = 0;  // ARGB bytes
  float depth = 1.0f;
  uint32_t stencil = 0;
};

// A copy of a surface into a texture (the 360's resolve; step 5 replaces
// most of them with passes drawn into their textures).
struct Resolve {
  // The game's flags: bits 0-1 the render target, 0x4 depth instead, 0x100
  // clear the target after, 0x200 clear depth and stencil after, bits 26-31
  // an exponent bias.
  uint32_t flags = 0;
  bool has_rect = false, has_point = false;
  Rect rect;
  int32_t point[2] = {};
  Texture destination;
  bool level_or_slice = false;
  float clear_color[4] = {};
  float clear_depth = 0.0f;
  uint32_t clear_stencil = 0;
  // A tile of predicated tiling: only the last tile's clears are made.
  bool apply_clears = true;
};

// What draws with the state. The backend reads the state at each call and
// clears the dirty bits it has taken.
class Backend {
 public:
  virtual ~Backend() = default;
  virtual void DrawIndexed(State& state, const render::DrawIndexed& draw) = 0;
  virtual void DrawInline(State& state, const render::DrawInline& draw) = 0;
  virtual void Clear(State& state, const render::Clear& clear) = 0;
  virtual void Resolve(State& state, const render::Resolve& resolve) = 0;
  // Predicated tiling: the targets are drawn whole, `width` × `height`
  // (the tiles' extent), each cleared to `clear` first.
  virtual void BeginTiling(State& state, uint32_t width, uint32_t height, float clear) = 0;
  virtual void EndTiling(State& state) = 0;
  // The frame's pass plan (render/pass_plan.h), before the list it plans is
  // replayed; it holds until the next.
  virtual void PassPlanned(const PassPlan& plan) = 0;

  // Textures made, written and freed (render/resources.h), between lists.
  virtual void TextureCreated(Texture texture) = 0;
  virtual void TextureWritten(Texture texture) = 0;
  virtual void TextureDestroyed(Texture texture) = 0;
  // Buffers made, written and freed, the same way (step 2b).
  virtual void BufferCreated(Buffer buffer, const BufferData& data) = 0;
  virtual void BufferWritten(Buffer buffer) = 0;
  virtual void BufferDestroyed(Buffer buffer) = 0;
};

}  // namespace render
