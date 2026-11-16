// Our threads, sync and TLS for the game (E1: docs/rexglue-removal.md,
// "E1: the plan"): what other code needs of src/threads/.

#pragma once

#include <cstdint>

namespace threads {

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
