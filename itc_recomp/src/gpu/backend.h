// The graphics API under the renderer (docs/d3d11-backend.md, "The design"):
// what the renderer's logic asks of Vulkan or Direct3D 11. Built up in
// pieces (step 3); so far the window's device, the swapchain, the frames,
// samplers and images.
// Render thread only, but where a call says otherwise.

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include <SDL3/SDL.h>

#include "gpu/types.h"

namespace gpu {

struct Extent2D {
  uint32_t width = 0, height = 0;
};

class Backend {
 public:
  virtual ~Backend() = default;

  // The window's flags this API needs (SDL_WINDOW_VULKAN).
  virtual SDL_WindowFlags WindowFlags() const = 0;
  // On the main thread, once the window is made: what must be made there
  // (Vulkan's instance and surface). Returns an error, or empty.
  virtual std::string AttachWindow(SDL_Window* window) = 0;
  // The device and swapchain. False if the backend can't run.
  virtual bool Init() = 0;
  // Where it keeps what it makes once for this device and driver (Vulkan's
  // pipeline cache; empty: nowhere): read now, written as it grows.
  virtual void OpenDiskCache(const std::filesystem::path& directory) = 0;

  // The swapchain's size (the window's, in pixels).
  virtual Extent2D OutputSize() const = 0;
  // The window's size changed (any thread): the swapchain is made again
  // before the next present.
  virtual void WindowResized() = 0;

  // A frame's recording begins once its slot's earlier frame is done on the
  // GPU. Returns the slot (0 to frames in flight - 1).
  virtual uint32_t BeginFrame() = 0;
  // Ends the frame: shows `size` of `shown` (in its copy source usage),
  // fitted to the window with black bars, and submits. False if nothing
  // could be shown (no swapchain image now).
  virtual bool EndFrame(Image* shown, Extent2D size) = 0;
  // Waits for all submitted work.
  virtual void WaitIdle() = 0;

  // A sampler, kept for the backend's life (null if it can't be made).
  virtual Sampler* CreateSampler(const SamplerDesc& desc) = 0;

  // An image (null if it can't be made). `initial`: its texels to start
  // with, every layer's first level tightly packed one after another (then
  // ready to sample); without, its contents are undefined.
  virtual Image* CreateImage(const ImageDesc& desc, const void* initial = nullptr) = 0;
  // At once: the caller keeps it until the GPU is done with it.
  virtual void DestroyImage(Image* image) = 0;
  virtual const ImageDesc& Describe(const Image* image) const = 0;
  // The device memory it holds, in bytes.
  virtual uint64_t MemorySize(const Image* image) const = 0;
  // Recorded in the frame: the image is next used for `usage`.
  virtual void Use(Image* image, Usage usage) = 0;
  // A buffer (null if it can't be made), destroyed at once.
  virtual Buffer* CreateBuffer(const BufferDesc& desc) = 0;
  virtual void DestroyBuffer(Buffer* buffer) = 0;
  // A host-visible buffer's memory (null for the GPU's own).
  virtual uint8_t* Mapped(Buffer* buffer) = 0;
  virtual uint64_t MemorySize(const Buffer* buffer) const = 0;

  // Uploads from a host-visible staging buffer the caller keeps until the
  // frame is done, run before the frame's commands (whenever in the frame
  // they're asked for, in the order asked). An image is ready to sample
  // after (all of it, its contents before kept unless it was never
  // uploaded); a buffer's ranges are ready to draw from.
  virtual void UploadImage(Buffer* staging, Image* image, const ImageUpload* regions,
                           uint32_t count) = 0;
  virtual void UploadBuffer(Buffer* staging, Buffer* buffer, const BufferUpload& region) = 0;

  // A shader (null if it can't be made), kept for the backend's life.
  virtual Shader* CreateShader(const ShaderDesc& desc) = 0;
  // The pipeline for `desc`, made the first time (`made` set then) and
  // kept; null if it can't be made.
  virtual Pipeline* GetPipeline(const PipelineDesc& desc, bool* made = nullptr) = 0;

  // A stage's texture set for this frame: `shader`'s views (in its
  // textures' order) and samplers (null shader: none). The same contents
  // give the same set within a frame (`found` set then).
  virtual TextureSet* MakeTextureSet(Shader* shader, ShaderStage stage, View* const* views,
                                     uint32_t view_count, Sampler* const* samplers,
                                     uint32_t sampler_count, bool* found = nullptr) = 0;

  // `buffer` (a host-visible uniform buffer) as the draws' four constant
  // blocks (system, vertex floats, pixel floats, bool/loop), each
  // `ranges[i]` bytes at the offsets BindResources gives. Once a buffer.
  virtual bool DescribeConstants(Buffer* buffer, const uint32_t ranges[4]) = 0;
  // In a pass: the draw's texture sets and constant blocks, for `pipeline`.
  virtual void BindResources(Pipeline* pipeline, TextureSet* vertex, TextureSet* pixel,
                             Buffer* constants, const uint32_t offsets[4]) = 0;

  // A pass drawing into `color` and `depth` (either may have no image)
  // within `area`, until EndPass. Copies, clears, uploads and Use aren't
  // made inside one.
  virtual void BeginPass(const PassTarget& color, const PassTarget& depth, const Rect& area) = 0;
  virtual void EndPass() = 0;

  // In a pass: the draw's dynamic state, pipeline, vertex and index
  // buffers, and the draws.
  virtual void SetViewport(const Viewport& viewport) = 0;
  virtual void SetScissor(const Rect& scissor) = 0;
  virtual void SetDepthBias(float constant, float slope) = 0;
  virtual void SetBlendConstants(const float constants[4]) = 0;
  virtual void BindPipeline(Pipeline* pipeline) = 0;
  // Bindings 0 to count - 1, all from `buffer`, at `offsets`.
  virtual void BindVertexBuffers(Buffer* buffer, uint32_t count, const uint64_t* offsets) = 0;
  virtual void BindIndexBuffer(Buffer* buffer, uint64_t offset, bool index_32bit) = 0;
  virtual void Draw(uint32_t vertices, uint32_t instances, uint32_t first_vertex,
                    uint32_t first_instance) = 0;
  virtual void DrawIndexed(uint32_t indices, uint32_t instances, uint32_t first_index,
                           int32_t base_vertex, uint32_t first_instance) = 0;

  // In the frame, outside passes, the images in their copy usages (the
  // cleared one copy destination). Level 0, layer 0.
  // A color image cleared to `bits` (four floats or integers).
  virtual void ClearColor(Image* image, const uint32_t bits[4]) = 0;
  // A depth image's depth and/or stencil cleared.
  virtual void ClearDepthStencil(Image* image, float depth, uint32_t stencil, bool clear_depth,
                                 bool clear_stencil) = 0;
  // The top-left `width` x `height` copied as is (all aspects).
  virtual void CopyImage(Image* source, Image* dest, uint32_t width, uint32_t height) = 0;
  // `from` scaled into `to` with `filter` (color).
  virtual void BlitImage(Image* source, const Rect& from, Image* dest, const Rect& to,
                         Filter filter) = 0;
  // A multisampled image's top-left `width` x `height` averaged into `dest`.
  virtual void ResolveImage(Image* source, Image* dest, uint32_t width, uint32_t height) = 0;
  // As UploadImage, but here in the frame (the image a copy destination).
  virtual void CopyBufferToImage(Buffer* buffer, Image* image, const ImageUpload* regions,
                                 uint32_t count) = 0;

  // A view for sampling (null if it can't be made), destroyed at once.
  virtual View* CreateView(Image* image, const ViewDesc& desc) = 0;
  virtual void DestroyView(View* view) = 0;
};

}  // namespace gpu
