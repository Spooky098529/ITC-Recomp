// Our threads, sync and TLS for the game (E1: docs/rexglue-removal.md,
// "E1: the plan"): what other code needs of src/threads/.

#pragma once

#include <cstdint>
#include <functional>
#include <thread>

#include "lifted/lifted.h"

namespace threads {

// A host thread of ours for our own code that runs game code (native
// audio's mixer thread): `body` gets the call arguments with r1 at a stack
// of `stack_size` and r13 our per-thread block. `name` is for the log.
std::thread StartThread(uint32_t stack_size, const char* name,
                        std::function<void(lifted::Args)> body);

// The game's main thread as one of ours (in place of the SDK's
// PrepareModuleLaunch): `entry` (our start-up, xstart's) run on a host
// thread with a stack of `stack_size` and our per-thread block; joinable.
std::thread StartMainThread(uint32_t stack_size, lifted::Entry entry);

// --native_threads: our answers to the thread, sync and TLS imports.
bool Enabled();

// The per-thread block (r13) of the calling host thread when it's one of
// our threads (threads/threads.cpp), for native code that has no r13 to
// hand; 0 on other threads (the main thread, still the SDK's).
uint32_t CurrentPcr();

// The thread ID game code reads ([[r13 + 256] + 0x14C]) of the calling
// thread when it's ours; 0 otherwise.
uint32_t CurrentThreadIdIfOurs();

}  // namespace threads
