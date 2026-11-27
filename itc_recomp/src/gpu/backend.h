// The graphics API under the renderer (docs/d3d11-backend.md, "The design"):
// what the renderer's logic asks of Vulkan or Direct3D 11. Built up in
// pieces (step 3); so far the window's device, the swapchain and the frames.
// Render thread only, but where a call says otherwise.

#pragma once

#include <cstdint>
#include <string>

#include <SDL3/SDL.h>

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

  // The swapchain's size (the window's, in pixels).
  virtual Extent2D OutputSize() const = 0;
  // The window's size changed (any thread): the swapchain is made again
  // before the next present.
  virtual void WindowResized() = 0;

  // A frame's recording begins once its slot's earlier frame is done on the
  // GPU. Returns the slot (0 to frames in flight - 1).
  virtual uint32_t BeginFrame() = 0;
  // Ends the frame: shows the image the renderer made (in its copy source
  // usage, `size` of it), fitted to the window with black bars, and
  // submits. False if nothing could be shown (no swapchain image now).
  virtual bool EndFrame(Extent2D size) = 0;
  // Waits for all submitted work.
  virtual void WaitIdle() = 0;
};

}  // namespace gpu
