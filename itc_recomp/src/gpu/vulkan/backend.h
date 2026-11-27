// The Vulkan backend (docs/d3d11-backend.md, step 3). While the renderer's
// Vulkan calls move here piece by piece, the code not yet moved reaches
// Vulkan through the accessors at the end ("In between" in the plan).

#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include "gpu/backend.h"
#include "native/device_context.h"
#include "native/vulkan_functions.h"

namespace gpu::vulkan {

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
  Sampler* CreateSampler(const SamplerDesc& desc) override;
  static VkSampler Handle(Sampler* sampler) { return reinterpret_cast<VkSampler>(sampler); }
  Image* CreateImage(const ImageDesc& desc, const void* initial = nullptr) override;
  void DestroyImage(Image* image) override;
  const ImageDesc& Describe(const Image* image) const override { return Object(image).desc; }
  uint64_t MemorySize(const Image* image) const override { return Object(image).memory_size; }
  void Use(Image* image, Usage usage) override;
  static ImageObject& Object(Image* image) { return *reinterpret_cast<ImageObject*>(image); }
  static const ImageObject& Object(const Image* image) {
    return *reinterpret_cast<const ImageObject*>(image);
  }

  // Not yet moved (the plan's "In between").
  const native::DeviceContext& context() const { return context_; }
  // The frame's command buffer, open between BeginFrame and EndFrame.
  VkCommandBuffer commands() const { return frames_[frame_index_].commands; }
  // Command buffers to submit before the frame's (the texture and arena
  // uploads; null ones left out).
  void SubmitBefore(VkCommandBuffer commands) {
    if (commands) {
      before_.push_back(commands);
    }
  }

 private:
  bool CreateDevice();
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
  };
  std::vector<Frame> frames_;
  uint32_t frame_index_ = 0;

  native::DeviceContext context_{};
  VkPhysicalDeviceMemoryProperties memory_properties_{};
  std::vector<VkCommandBuffer> before_;
};

}  // namespace gpu::vulkan
