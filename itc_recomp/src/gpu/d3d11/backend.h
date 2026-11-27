// The Direct3D 11 backend (docs/d3d11-backend.md, step 5): gpu::Backend over
// D3D11.1, Windows only (--renderer=d3d11). Each shader is its HLSL
// (generated_*_hlsl.h, step 4) compiled once with its fit's defines by
// D3DCompile, kept on disk. What D3D11 lacks is done here: views' swizzles
// (a swizzled copy drawn again when its image changes), triangle fans
// (indices made for them), scaled blits (a draw).

#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef NOMINMAX
#define NOMINMAX  // windows.h's min and max macros off
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <d3d11_1.h>
#include <dxgi1_5.h>
#include <wrl/client.h>

#include "gpu/backend.h"

namespace gpu::d3d11 {

using Microsoft::WRL::ComPtr;

struct ImageObject {
  ImageDesc desc;
  ComPtr<ID3D11Resource> resource;
  DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;  // the texels' (typeless for depth)
  ComPtr<ID3D11ShaderResourceView> srv;      // all of it, its own type (depth alone)
  ComPtr<ID3D11ShaderResourceView> srv_2d;   // level 0, layer 0 as a plain 2D (or 2D MS)
  ComPtr<ID3D11ShaderResourceView> srv_array;  // all of it as a 2D array (swizzles)
  ComPtr<ID3D11RenderTargetView> rtv;        // level 0, layer 0
  ComPtr<ID3D11DepthStencilView> dsv;
  ComPtr<ID3D11UnorderedAccessView> uav;
  uint64_t memory_size = 0;
  // A compressed image whose size D3D11 refuses (not a multiple of 4 at its
  // top level): kept decoded, as RGBA8 (RG8 for BC5), its uploads decoded.
  bool decoded = false;
  // Counts writes (uploads, passes, copies): swizzled copies are drawn
  // again when it moves.
  uint64_t generation = 1;
};

struct ViewObject {
  ImageObject* image = nullptr;
  ViewDesc desc;
  ComPtr<ID3D11ShaderResourceView> srv;
  // A swizzle D3D11's views can't give: the image copied through it.
  bool swizzled = false;
  std::unique_ptr<ImageObject> copy;
  std::vector<ComPtr<ID3D11RenderTargetView>> copy_targets;  // level x layer
  uint64_t copied = 0;  // the image's generation copied
};

struct BufferObject {
  BufferDesc desc;
  ComPtr<ID3D11Buffer> buffer;  // none for staging and read-back memory
  std::unique_ptr<uint8_t[]> memory;  // host-visible: what Mapped gives
  uint32_t ranges[4] = {};            // DescribeConstants
  bool mapped_once = false;
};

struct ShaderObject {
  ShaderStage stage = ShaderStage::kVertex;
  ComPtr<ID3D11VertexShader> vertex;
  ComPtr<ID3D11PixelShader> pixel;
  std::vector<uint8_t> bytecode;
  std::vector<VertexBinding> bindings;
  std::vector<VertexAttribute> attributes;
  uint32_t textures = 0, samplers = 0;
};

struct PipelineObject {
  PipelineDesc desc;
  ShaderObject* vertex = nullptr;
  ShaderObject* pixel = nullptr;  // null: depth only (or the ID pass's own)
  ComPtr<ID3D11PixelShader> id_pixel;  // the draw ID pass's
  ComPtr<ID3D11InputLayout> layout;
  ComPtr<ID3D11DepthStencilState> depth;
  ComPtr<ID3D11BlendState> blend;
  D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
  bool fan = false;
  bool culls_all = false;  // both faces culled: nothing drawn
  bool constant_alpha = false;  // its color blend reads the constant's alpha
  uint32_t strides[32] = {};
  const void* resource_layout = nullptr;
};

struct TextureSetObject {
  std::vector<ViewObject*> views;
  std::vector<ID3D11SamplerState*> samplers;
};

class Backend final : public gpu::Backend {
 public:
  struct Options {
    uint32_t frames_in_flight = 2;
    bool debug = false;  // the D3D11 debug layer
  };
  explicit Backend(const Options& options);
  ~Backend() override;

  SDL_WindowFlags WindowFlags() const override { return 0; }
  std::string AttachWindow(SDL_Window* window) override;
  bool Init() override;
  void OpenDiskCache(const std::filesystem::path& directory) override;
  Extent2D OutputSize() const override { return output_size_; }
  void WindowResized() override { window_resized_ = true; }
  uint32_t BeginFrame() override;
  bool EndFrame(Image* shown, Extent2D size) override;
  void WaitIdle() override;
  uint32_t ConstantsAlignment() const override { return 256; }
  bool CanFilter(Format format) const override;
  Sampler* CreateSampler(const SamplerDesc& desc) override;
  Image* CreateImage(const ImageDesc& desc, const void* initial = nullptr) override;
  void DestroyImage(Image* image) override;
  const ImageDesc& Describe(const Image* image) const override { return Object(image).desc; }
  uint64_t MemorySize(const Image* image) const override { return Object(image).memory_size; }
  void Use(Image*, Usage) override {}  // D3D11 tracks it itself
  Buffer* CreateBuffer(const BufferDesc& desc) override;
  void DestroyBuffer(Buffer* buffer) override;
  uint8_t* Mapped(Buffer* buffer) override { return Object(buffer).memory.get(); }
  uint64_t MemorySize(const Buffer* buffer) const override {
    return reinterpret_cast<const BufferObject*>(buffer)->desc.size;
  }
  void UploadImage(Buffer* staging, Image* image, const ImageUpload* regions,
                   uint32_t count) override;
  void UploadBuffer(Buffer* staging, Buffer* buffer, const BufferUpload& region) override;
  Shader* CreateShader(const ShaderDesc& desc) override;
  Pipeline* GetPipeline(const PipelineDesc& desc, bool* made = nullptr) override;
  TextureSet* MakeTextureSet(Shader* shader, ShaderStage stage, View* const* views,
                             uint32_t view_count, Sampler* const* samplers, uint32_t sampler_count,
                             bool* found = nullptr) override;
  bool DescribeConstants(Buffer* buffer, const uint32_t ranges[4]) override;
  const void* ResourceLayout(Pipeline* pipeline) const override {
    return Object(pipeline).resource_layout;
  }
  void BindResources(Pipeline* pipeline, TextureSet* vertex, TextureSet* pixel, Buffer* constants,
                     const uint32_t offsets[4]) override;
  void BeginPass(const PassTarget& color, const PassTarget& depth, const Rect& area) override;
  void EndPass() override;
  void SetViewport(const Viewport& viewport) override;
  void SetScissor(const Rect& scissor) override;
  void SetDepthBias(float constant, float slope) override;
  void SetBlendConstants(const float constants[4]) override;
  void BindPipeline(Pipeline* pipeline) override;
  void BindVertexBuffers(Buffer* buffer, uint32_t count, const uint64_t* offsets) override;
  void BindIndexBuffer(Buffer* buffer, uint64_t offset, bool index_32bit) override;
  void Draw(uint32_t vertices, uint32_t instances, uint32_t first_vertex,
            uint32_t first_instance) override;
  void DrawIndexed(uint32_t indices, uint32_t instances, uint32_t first_index, int32_t base_vertex,
                   uint32_t first_instance) override;
  void ClearColor(Image* image, const uint32_t bits[4]) override;
  void ClearDepthStencil(Image* image, float depth, uint32_t stencil, bool clear_depth,
                         bool clear_stencil) override;
  void CopyImage(Image* source, Image* dest, uint32_t width, uint32_t height) override;
  void BlitImage(Image* source, const Rect& from, Image* dest, const Rect& to,
                 Filter filter) override;
  void ResolveImage(Image* source, Image* dest, uint32_t width, uint32_t height) override;
  void CopyBufferToImage(Buffer* buffer, Image* image, const ImageUpload* regions,
                         uint32_t count) override;
  void CopyImageToBuffer(Image* image, Buffer* buffer, uint64_t offset, uint32_t width,
                         uint32_t height) override;
  View* CreateView(Image* image, const ViewDesc& desc) override;
  void DestroyView(View* view) override;
  bool DrawResolve(const ResolveDraw& draw) override;
  bool DrawMovie(Image* const planes[3], Image* target, const Rect& area) override;
  bool ApplyGamma(Image* image, Extent2D size, const uint16_t table[256]) override;
  Pipeline* GetIdPipeline(const PipelineDesc& desc) override;
  void SetDrawId(uint32_t id) override;

  static ImageObject& Object(Image* image) { return *reinterpret_cast<ImageObject*>(image); }
  static const ImageObject& Object(const Image* image) {
    return *reinterpret_cast<const ImageObject*>(image);
  }
  static BufferObject& Object(Buffer* buffer) { return *reinterpret_cast<BufferObject*>(buffer); }
  static ShaderObject& Object(Shader* shader) { return *reinterpret_cast<ShaderObject*>(shader); }
  static PipelineObject& Object(Pipeline* pipeline) {
    return *reinterpret_cast<PipelineObject*>(pipeline);
  }
  static const PipelineObject& Object(const Pipeline* pipeline) {
    return *reinterpret_cast<const PipelineObject*>(pipeline);
  }

 private:
  struct DescHash {
    size_t operator()(const PipelineDesc& desc) const;
  };
  // A pass of the backend's own (a full-screen triangle into one target).
  struct OwnPass {
    ComPtr<ID3D11PixelShader> pixel;
    ComPtr<ID3D11Buffer> constants;
    uint32_t constants_size = 0;
  };
  struct Readback {
    ComPtr<ID3D11Texture2D> staging;
    BufferObject* buffer = nullptr;
    uint64_t offset = 0;
    uint32_t width = 0, height = 0, texel_bytes = 0;
    bool depth = false;
  };
  struct Frame {
    ComPtr<ID3D11Query> done;
    bool submitted = false;
    std::vector<std::unique_ptr<TextureSetObject>> sets;
    std::unordered_map<std::string, TextureSetObject*> set_cache;
    std::vector<Readback> readbacks;
  };

  bool CreateDeviceAndSwapchain(HWND window);
  bool CreateOwnResources();
  bool UpdateBackBuffer();
  std::vector<uint8_t> Compile(const char* hlsl, const char* profile,
                               const std::vector<std::pair<std::string, std::string>>& defines);
  bool MakeOwnPass(OwnPass& pass, const char* hlsl, uint32_t constants_size);
  void BeginOwnPass(ID3D11RenderTargetView* target, const Rect& area, OwnPass& pass,
                    const void* constants);
  void EndOwnPass();
  bool MakeViews(ImageObject& o);
  ID3D11SamplerState* SamplerFor(Filter filter);
  void Written(ImageObject& o) { o.generation++; }
  void RefreshSwizzle(ViewObject& view);
  void ApplyStageResources(bool vertex_stage);
  void ApplyTargets();
  bool FlushDraw();
  void FinishReadbacks(Frame& frame);
  void WaitFrame(Frame& frame);

  Options options_;
  SDL_Window* window_ = nullptr;
  std::string init_error_;
  ComPtr<ID3D11Device> device0_;
  ComPtr<ID3D11Device1> device_;
  ComPtr<ID3D11DeviceContext> context0_;
  ComPtr<ID3D11DeviceContext1> context_;
  ComPtr<IDXGISwapChain1> swapchain_;
  ComPtr<ID3D11RenderTargetView> back_buffer_;
  Extent2D output_size_;
  std::atomic<bool> window_resized_{false};
  bool tearing_ = false;
  std::filesystem::path cache_directory_;
  using CompileFn = HRESULT(WINAPI*)(LPCVOID, SIZE_T, LPCSTR, const D3D_SHADER_MACRO*,
                                     ID3DInclude*, LPCSTR, LPCSTR, UINT, UINT, ID3DBlob**,
                                     ID3DBlob**);
  CompileFn compile_ = nullptr;
  std::unordered_map<uint64_t, std::vector<uint8_t>> compiled_;

  std::vector<Frame> frames_;
  uint32_t frame_index_ = 0;

  std::unordered_map<PipelineDesc, std::unique_ptr<PipelineObject>, DescHash> pipelines_;
  std::unordered_map<PipelineDesc, std::unique_ptr<PipelineObject>, DescHash> id_pipelines_;
  std::unordered_map<std::string, std::unique_ptr<char>> resource_layouts_;
  std::unordered_map<uint64_t, ComPtr<ID3D11RasterizerState>> rasterizers_;
  std::vector<ComPtr<ID3D11SamplerState>> samplers_;
  std::vector<std::unique_ptr<ShaderObject>> shaders_;

  // What's bound (applied at the draw when it changed).
  PipelineObject* pipeline_ = nullptr;
  bool pipeline_dirty_ = true, raster_dirty_ = true, vertex_buffers_dirty_ = true;
  bool index_dirty_ = true, blend_dirty_ = true;
  ID3D11RenderTargetView* color_target_ = nullptr;
  ID3D11DepthStencilView* depth_target_ = nullptr;
  ImageObject* color_image_ = nullptr;
  ImageObject* depth_image_ = nullptr;
  bool in_pass_ = false;
  D3D11_VIEWPORT viewport_{};
  D3D11_RECT scissor_{};
  float bias_constant_ = 0.0f, bias_slope_ = 0.0f;
  float blend_constants_[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  BufferObject* vertex_buffer_ = nullptr;
  uint32_t vertex_buffer_count_ = 0;
  uint64_t vertex_offsets_[32] = {};
  BufferObject* index_buffer_ = nullptr;
  uint64_t index_offset_ = 0;
  bool index_32bit_ = false;
  // Each stage's bound textures (as BindResources gave them).
  TextureSetObject* stage_sets_[2] = {};
  uint32_t stage_view_counts_[2] = {};
  // The draws' pixel constant blocks (put back after the backend's own passes).
  ID3D11Buffer* pixel_constants_[4] = {};
  UINT pixel_first_[4] = {}, pixel_number_[4] = {};
  int32_t vertex_info_[2] = {INT32_MIN, INT32_MIN};  // base vertex, first instance (b4)
  ComPtr<ID3D11Buffer> vertex_info_buffer_;

  // The backend's own.
  ComPtr<ID3D11VertexShader> fullscreen_vertex_;
  OwnPass resolve_, resolve_ms_, movie_, blit_, swizzle_;
  ComPtr<ID3D11PixelShader> id_pixel_;
  ComPtr<ID3D11Buffer> id_constants_;
  ComPtr<ID3D11ComputeShader> gamma_;
  ComPtr<ID3D11Buffer> gamma_constants_, gamma_table_;
  ComPtr<ID3D11ShaderResourceView> gamma_table_view_;
  bool gamma_failed_ = false, gamma_uploaded_ = false;
  ComPtr<ID3D11SamplerState> point_sampler_, linear_sampler_;
  ComPtr<ID3D11RasterizerState> own_rasterizer_;
  ComPtr<ID3D11DepthStencilState> no_depth_;
  ComPtr<ID3D11BlendState> no_blend_;
  ComPtr<ID3D11Buffer> fan_indices_;
  uint32_t fan_index_at_ = 0;
  std::unique_ptr<ImageObject> resolve_temp_;
  bool warned_indexed_fan_ = false, warned_swizzle_ = false, warned_copy_ = false;
};

}  // namespace gpu::d3d11
