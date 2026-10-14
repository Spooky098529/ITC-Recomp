// The SDK app's window and ours (see "One window" in
// docs/native-renderer.md). In exclusive mode our renderer's window is the
// only one shown: the SDK's window is hidden, and the SDK's event loop
// routes our window's events (keyboard, focus, close) to it, so its input
// drivers and the app's close path work unchanged. Gamepads don't depend on
// a window.

#pragma once

#include <SDL3/SDL.h>

namespace rex::ui {
class Window;
}

namespace native {

// Main thread, once the SDK's window is open (ItcApp::SetupPresentation).
void AppWindow_SetSdkWindow(rex::ui::Window* window);

// Main thread, when our renderer has created its window.
void AppWindow_AdoptOurWindow(SDL_Window* window);

}  // namespace native
