// The per-thread block (r13) of the calling host thread when it's one of
// our threads (threads/threads.cpp), for native code that has no r13 to
// hand; 0 on other threads (the main thread, still the SDK's, until E2).

#pragma once

#include <cstdint>

namespace threads {

uint32_t CurrentPcr();

// The thread ID game code reads ([[r13 + 256] + 0x14C]) of the calling
// thread when it's ours; 0 otherwise.
uint32_t CurrentThreadIdIfOurs();

}  // namespace threads
