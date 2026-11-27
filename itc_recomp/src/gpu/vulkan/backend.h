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
  bool EndFrame(Extent2D size) override;
  void WaitIdle() override;

  // Not yet moved (the plan's "In between").
  const native::DeviceContext& context() const { return context_; }
  // The frame's command buffer, open between BeginFrame and EndFrame.
  VkCommandBuffer commands() const { return frames_[frame_index_].commands; }
  // What EndFrame shows (TRANSFER_SRC_OPTIMAL), and command buffers to
  // submit before the frame's (the texture and arena uploads; null ones
  // left out).
  void set_shown(VkImage image) { shown_ = image; }
  void SubmitBefore(VkCommandBuffer commands) {
    if (commands) {
      before_.push_back(commands);
    }
  }

 private:
  bool CreateDevice();
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
  VkImage shown_ = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> before_;
};

}  // namespace gpu::vulkan
