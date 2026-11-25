// Handles of ours (E1: docs/rexglue-removal.md, "E1: the plan"): events
// (threads/events.cpp) from kEventHandleBase, threads (threads/threads.cpp)
// from kThreadHandleBase, both below the SDK's (0xF8000000 up).

#pragma once

#include <cstdint>

namespace threads {

constexpr uint32_t kEventHandleBase = 0xE1000000;
constexpr uint32_t kThreadHandleBase = 0xE2000000;
constexpr uint32_t kOurHandlesEnd = 0xF0000000;

inline bool OurHandle(uint32_t handle) {
  return handle >= kEventHandleBase && handle < kOurHandlesEnd;
}

inline bool ThreadHandle(uint32_t handle) {
  return handle >= kThreadHandleBase && handle < kOurHandlesEnd;
}

// NtClose on a thread handle of ours (threads/threads.cpp): true when it was
// one.
bool CloseThreadHandle(uint32_t handle);

}  // namespace threads
