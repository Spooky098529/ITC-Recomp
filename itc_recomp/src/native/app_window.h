// The app's window: our renderer's (see "One window" in
// docs/native-renderer.md). Since our own start-up (src/app/main.cpp,
// 2026-11-15) the SDK makes no window: its events (a close) go to our
// main-thread loop. Input is gamepads only (no --mnk_mode since
// docs/rexglue-removal.md's A0), which don't depend on a window.

#pragma once

#include <SDL3/SDL.h>

namespace native {

// Main thread, when our renderer has created its window.
void AppWindow_AdoptOurWindow(SDL_Window* window);

}  // namespace native
