#include "app_window.h"
#include "renderer.h"

#include "base/log.h"
#include <rex/ui/window_listener.h>
#include <rex/ui/window_sdl.h>
#include <rex/ui/windowed_app_context_sdl.h>

#include <string>

namespace native {
namespace {

rex::ui::WindowSDL* g_sdk_window = nullptr;
SDL_WindowID g_our_window_id = 0;

// Our window's events are routed to the SDK's window object only while it
// exists.
class SdkWindowListener : public rex::ui::WindowListener {
 public:
  void OnClosing(rex::ui::UIEvent&) override {
    if (g_our_window_id) {
      static_cast<rex::ui::SDLWindowedAppContext&>(g_sdk_window->app_context())
          .UnregisterWindow(g_our_window_id);
      g_our_window_id = 0;
    }
    g_sdk_window = nullptr;
  }
} g_listener;

}  // namespace

void AppWindow_SetSdkWindow(rex::ui::Window* window) {
  if (!window) {
    return;
  }
  g_sdk_window = static_cast<rex::ui::WindowSDL*>(window);
  g_sdk_window->AddListener(&g_listener);
  // The SDK's window has no accessor for its SDL window; ours doesn't exist
  // yet, so it's the only one.
  int count = 0;
  SDL_Window** windows = SDL_GetWindows(&count);
  if (count == 1) {
    SDL_HideWindow(windows[0]);
    ITC_LOG_INFO("native renderer: the SDK's window is hidden (exclusive mode)");
  } else {
    ITC_LOG_WARN("native renderer: {} SDL windows at startup, expected 1; the SDK's window stays",
                 count);
  }
  SDL_free(windows);
}

void AppWindow_AdoptOurWindow(SDL_Window* window) {
  if (!g_sdk_window) {
    return;
  }
  SDL_SetWindowTitle(window, g_sdk_window->GetTitle().c_str());
  g_our_window_id = SDL_GetWindowID(window);
  static_cast<rex::ui::SDLWindowedAppContext&>(g_sdk_window->app_context())
      .RegisterWindow(g_our_window_id, g_sdk_window);
  ITC_LOG_INFO("native renderer: our window's events go to the SDK's window (focus, close)");
}

}  // namespace native
