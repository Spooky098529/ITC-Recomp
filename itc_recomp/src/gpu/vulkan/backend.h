// The Vulkan backend (docs/d3d11-backend.md, step 3). While the renderer's
// Vulkan calls move here piece by piece, the code not yet moved reaches
// Vulkan through the accessors at the end ("In between" in the plan).

#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>

#include "gpu/backend.h"
#include "native/device_context.h"
#include "native/vulkan_functions.h"

namespace gpu::vulkan {

// A shader as this backend holds it (behind gpu::Shader): its module, its
// stage's texture set layout (shared by shaders of the same stage, texture
// types and sampler count), and a vertex shader's vertex input.
struct ShaderObject {
  VkShaderModule module = VK_NULL_HANDLE;
  VkDescriptorSetLayout textures = VK_NULL_HANDLE;
  std::vector<VkVertexInputBindingDescription> bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;
};

// A pipeline as this backend holds it (behind gpu::Pipeline), with its
// layout and the layout's sets (0 empty, 1 the constant blocks, 2 and 3 the
// vertex and pixel shaders' textures).
struct PipelineObject {
  VkPipeline pipeline = VK_NULL_HANDLE;
  VkPipelineLayout layout = VK_NULL_HANDLE;
  VkDescriptorSetLayout sets[4] = {};
};

// A buffer as this backend holds it (behind gpu::Buffer).
struct BufferObject {
  BufferDesc desc;
  VkBuffer buffer = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  VkDeviceSize memory_size = 0;
  uint8_t* mapped = nullptr;
};

// An image as this backend holds it (behind gpu::Image).
struct ImageObject {
  ImageDesc desc;
  VkImage image = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  VkDeviceSize memory_size = 0;
  VkFormat format = VK_FORMAT_UNDEFINED;
  VkImageAspectFlags aspects = 0;  // color, or depth and stencil
  // All of it (both aspects of a depth image), and a depth image's depth
  // alone (for sampling it).
  VkImageView view = VK_NULL_HANDLE;
  VkImageView depth_view = VK_NULL_HANDLE;
  // Its layout as the frame's commands leave it.
  VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
};

class Backend final : public gpu::Backend {
 public:
  struct Options {
    uint32_t frames_in_flight = 2;
    bool validation = false;
    // The window's size asked for, where the window system leaves the
    // swapchain's size to us (Wayland) and the window can't say.
    Extent2D requested_size;
  };
  explicit Backend(const Options& options);
  ~Backend() override;

  SDL_WindowFlags WindowFlags() const override { return SDL_WINDOW_VULKAN; }
  std::string AttachWindow(SDL_Window* window) override;
  bool Init() override;
  Extent2D OutputSize() const override { return {swapchain_extent_.width, swapchain_extent_.height}; }
  void WindowResized() override { window_resized_ = true; }
  uint32_t BeginFrame() override;
  bool EndFrame(Image* shown, Extent2D size) override;
  void WaitIdle() override;
  void OpenDiskCache(const std::filesystem::path& directory) override;
  // Saved at most every 10 s, when `made` (pipelines made so far) grew.
  void MaybeSaveDiskCache(size_t made);
  VkPipelineCache pipeline_cache() const { return pipeline_cache_; }
  Sampler* CreateSampler(const SamplerDesc& desc) override;
  static VkSampler Handle(Sampler* sampler) { return reinterpret_cast<VkSampler>(sampler); }
  Image* CreateImage(const ImageDesc& desc, const void* initial = nullptr) override;
  void DestroyImage(Image* image) override;
  const ImageDesc& Describe(const Image* image) const override { return Object(image).desc; }
  uint64_t MemorySize(const Image* image) const override { return Object(image).memory_size; }
  void Use(Image* image, Usage usage) override;
  Buffer* CreateBuffer(const BufferDesc& desc) override;
  void DestroyBuffer(Buffer* buffer) override;
  uint8_t* Mapped(Buffer* buffer) override { return Object(buffer).mapped; }
  uint64_t MemorySize(const Buffer* buffer) const override {
    return reinterpret_cast<const BufferObject*>(buffer)->memory_size;
  }
  static BufferObject& Object(Buffer* buffer) { return *reinterpret_cast<BufferObject*>(buffer); }
  static VkBuffer Handle(Buffer* buffer) { return buffer ? Object(buffer).buffer : VK_NULL_HANDLE; }
  void UploadImage(Buffer* staging, Image* image, const ImageUpload* regions,
                   uint32_t count) override;
  void UploadBuffer(Buffer* staging, Buffer* buffer, const BufferUpload& region) override;
  Shader* CreateShader(const ShaderDesc& desc) override;
  static ShaderObject& Object(Shader* shader) { return *reinterpret_cast<ShaderObject*>(shader); }
  Pipeline* GetPipeline(const PipelineDesc& desc, bool* made = nullptr) override;
  static PipelineObject& Object(Pipeline* pipeline) {
    return *reinterpret_cast<PipelineObject*>(pipeline);
  }
  // A stage's texture set layout: the shader's, or an empty one without.
  VkDescriptorSetLayout TexturesLayout(Shader* shader, ShaderStage stage);
  // Sets 0 (empty) and 1 (the constant blocks: system, vertex floats, pixel
  // floats, bool/loop; dynamic uniform buffers).
  VkDescriptorSetLayout empty_layout() const { return empty_layout_; }
  VkDescriptorSetLayout constants_layout() const { return constants_layout_; }
  TextureSet* MakeTextureSet(Shader* shader, ShaderStage stage, View* const* views,
                             uint32_t view_count, Sampler* const* samplers, uint32_t sampler_count,
                             bool* found = nullptr) override;
  static VkDescriptorSet Handle(TextureSet* set) { return reinterpret_cast<VkDescriptorSet>(set); }
  bool DescribeConstants(Buffer* buffer, const uint32_t ranges[4]) override;
  void BindResources(Pipeline* pipeline, TextureSet* vertex, TextureSet* pixel, Buffer* constants,
                     const uint32_t offsets[4]) override;
  // Set 0's (empty) and a constant buffer's set 1 (for the ID pass's binds).
  VkDescriptorSet empty_set() const { return empty_set_; }
  VkDescriptorSet constants_set(Buffer* buffer) const { return constants_sets_.at(buffer); }
  void BeginPass(const PassTarget& color, const PassTarget& depth, const Rect& area) override;
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
  void EndPass() override;
  View* CreateView(Image* image, const ViewDesc& desc) override;
  void DestroyView(View* view) override;
  static VkImageView Handle(View* view) { return reinterpret_cast<VkImageView>(view); }
  static ImageObject& Object(Image* image) { return *reinterpret_cast<ImageObject*>(image); }
  static const ImageObject& Object(const Image* image) {
    return *reinterpret_cast<const ImageObject*>(image);
  }

  // Not yet moved (the plan's "In between").
  const native::DeviceContext& context() const { return context_; }
  // The frame's command buffer, open between BeginFrame and EndFrame.
  VkCommandBuffer commands() const { return frames_[frame_index_].commands; }

 private:
  bool CreateDevice();
  // The frame's upload commands, begun if they aren't.
  VkCommandBuffer Uploads();
  // Records `commands` with `record` and runs them now, waiting for them.
  template <typename Record>
  bool RunNow(Record record);
  bool CreateSwapchain();
  void DestroySwapchain();

  Options options_;
  SDL_Window* window_ = nullptr;
  VkInstance instance_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;
  uint32_t queue_family_ = 0;
  VkQueue queue_ = VK_NULL_HANDLE;

  VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
  VkFormat swapchain_format_ = VK_FORMAT_UNDEFINED;
  VkExtent2D swapchain_extent_{};
  std::vector<VkImage> swapchain_images_;
  std::vector<VkSemaphore> render_done_;  // one per swapchain image
  bool swapchain_stale_ = false;
  std::atomic<bool> window_resized_{false};

  struct Frame {
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer commands = VK_NULL_HANDLE;
    VkFence done = VK_NULL_HANDLE;
    VkSemaphore image_ready = VK_NULL_HANDLE;
    // Uploads, submitted before the frame's commands.
    VkCommandPool upload_pool = VK_NULL_HANDLE;
    VkCommandBuffer uploads = VK_NULL_HANDLE;
    // Texture sets: pools, kept for later frames, and the one in use.
    std::vector<VkDescriptorPool> texture_pools;
    size_t texture_pool = 0;
  };
  bool AddTexturePool(Frame& frame);
  std::vector<Frame> frames_;
  uint32_t frame_index_ = 0;

  native::DeviceContext context_{};
  VkPhysicalDeviceMemoryProperties memory_properties_{};
  std::vector<VkCommandBuffer> before_;
  // The upload commands are open (from the first upload, maybe before the
  // first frame, to the frame's end), and hold buffer uploads.
  bool uploading_ = false, uploaded_buffers_ = false;
  std::unordered_map<std::string, VkDescriptorSetLayout> texture_layouts_;
  // This frame's texture sets by their contents (backend.cpp).
  std::unique_ptr<class FrameSetCache> texture_sets_;
  std::vector<uint64_t> set_key_;
  std::vector<VkDescriptorImageInfo> set_infos_;
  std::vector<VkWriteDescriptorSet> set_writes_;
  VkDescriptorSetLayout empty_layout_ = VK_NULL_HANDLE, constants_layout_ = VK_NULL_HANDLE;
  VkDescriptorSet empty_set_ = VK_NULL_HANDLE;
  std::unordered_map<Buffer*, VkDescriptorSet> constants_sets_;
  struct DescHash {
    size_t operator()(const PipelineDesc& desc) const;
  };
  struct LayoutPairHash {
    size_t operator()(const std::pair<VkDescriptorSetLayout, VkDescriptorSetLayout>& key) const {
      return std::hash<const void*>()(key.first) ^
             (std::hash<const void*>()(key.second) * 0x9E3779B97F4A7C15ull);
    }
  };
  std::unordered_map<std::pair<VkDescriptorSetLayout, VkDescriptorSetLayout>, VkPipelineLayout,
                     LayoutPairHash>
      pipeline_layouts_;
  std::unordered_map<PipelineDesc, std::unique_ptr<PipelineObject>, DescHash> pipelines_;
  VkPipelineCache pipeline_cache_ = VK_NULL_HANDLE;
  std::filesystem::path pipeline_cache_file_;
  size_t pipeline_cache_saved_pipelines_ = 0;
  std::chrono::steady_clock::time_point pipeline_cache_saved_;
};

}  // namespace gpu::vulkan
