#include "app_window.h"

#include "base/log.h"

#include <itc_version.h>

#include <string>

namespace native {

void AppWindow_AdoptOurWindow(SDL_Window* window) {
  // The app's name and our version (cmake/version.cmake; the SDK's build
  // stamp until E4).
  const std::string title = std::string("itc ") + ITC_VERSION;
  SDL_SetWindowTitle(window, title.c_str());
  ITC_LOG_INFO("native renderer: our window is the app's window ({})", title);
}

}  // namespace native
