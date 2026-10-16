// Diagnostics (--log_thread_cpu): every 2 s, each busy thread's CPU use by
// name (from /proc/self/task), to find which thread pegs a core on slower
// machines. Linux only.

#pragma once

namespace debug {

// Starts the sampling thread if --log_thread_cpu is set.
void ThreadCpu_Start();

// Stops it, at shutdown.
void ThreadCpu_Stop();

}  // namespace debug
