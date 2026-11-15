#include "app_window.h"

#include "base/log.h"

#include <rex/version.h>

#include <string>

namespace native {

void AppWindow_AdoptOurWindow(SDL_Window* window) {
  // The title the SDK's window had: the app's name and the SDK's build stamp.
  const std::string title = std::string("itc ") + REXGLUE_BUILD_TITLE;
  SDL_SetWindowTitle(window, title.c_str());
  ITC_LOG_INFO("native renderer: our window is the app's window ({})", title);
}

}  // namespace native
