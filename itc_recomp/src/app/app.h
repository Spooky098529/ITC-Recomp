// The program's start and its main-thread loop ("A4 brought forward" in
// docs/rexglue-removal.md): our own, with rexglue as a library for guest
// memory, the kernel and the image load.

#pragma once

#include <functional>

#include "lifted/lifted.h"

namespace app {

// Runs `function` on the main thread (the one pumping SDL's events), soon.
// Any thread.
void RunOnMainThread(std::function<void()> function);

// Ends the program as closing the window does: the title's threads
// terminated, the log flushed, the process exited. Any thread.
void RequestQuit();

// Our start-up, run where the kernel would start the game's main thread
// (xstart's address; src/app/start_up.cpp).
lifted::Result StartUp(const lifted::Args& in);

}  // namespace app
