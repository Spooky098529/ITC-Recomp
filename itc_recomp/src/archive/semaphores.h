// The archive loader's four semaphores as host counting semaphores (the
// user's call, 2026-10-28: "Precedent first" in docs/loops/b7-loop.md). The
// game's Semaphore objects (40 bytes: +0 an event, +4 a critical section,
// +32 the count, +36 probably the maximum; video-output.md) at 0x826BAAE0
// (the queue lock), 0x826BAAB8 (the loader's work), 0x826BAA68 (a request
// done) and 0x826BAA90 (idle) are no longer used for waiting: every user,
// native and recompiled, takes these instead (the recompiled ones through
// the Semaphore::Wait and Signal adapters, src/hooks/archive.cpp, which
// route the four addresses here and leave every other semaphore to the
// game's class). Two of the game's functions read a count directly (the
// queueing and the picker read the work semaphore's +32), so each one's
// count is kept readable here and copied into its guest +32 whenever it
// changes, for the recompiled code reading it there.

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>

namespace archive {

constexpr uint32_t kQueueLock = 0x826BAAE0;
constexpr uint32_t kWork = 0x826BAAB8;
constexpr uint32_t kRequestDone = 0x826BAA68;
constexpr uint32_t kIdle = 0x826BAA90;
// The count's offset in the game's Semaphore.
constexpr uint32_t kSemaphoreCount = 32;

// A counting semaphore whose count can be read, its count mirrored to a
// guest word (`mirror`, 0 for none) on every change.
class HostSemaphore {
 public:
  // Starts at 1, as Semaphore::Init (sub_825925F0) makes them.
  explicit HostSemaphore(uint32_t mirror, int32_t count = 1);

  // Semaphore::Wait (sub_82592670): waits while the count is 0 or less, then
  // takes one.
  void Wait(uint8_t* base);
  // Semaphore::Signal (sub_825926E8): the count up by one, a waiter woken.
  void Signal(uint8_t* base);
  // The count (what the game reads at +32).
  int32_t Count() const { return count_.load(std::memory_order_acquire); }
  // How many threads are blocked in Wait (for the harness: the loader thread
  // parked on the work semaphore).
  int Waiters() const { return waiters_.load(std::memory_order_acquire); }
  bool Used() const { return used_.load(std::memory_order_acquire); }
  // Sets the count (the harness's test semaphores), mirrored.
  void SetCount(uint8_t* base, int32_t count);
  // Writes the count to the mirror.
  void Mirror(uint8_t* base);

 private:
  mutable std::mutex mutex_;
  std::condition_variable wake_;
  std::atomic<int32_t> count_;
  std::atomic<int> waiters_{0};
  std::atomic<bool> used_{false};
  uint32_t mirror_;
};

// The four.
struct Semaphores {
  HostSemaphore queue_lock{kQueueLock + kSemaphoreCount};
  HostSemaphore work{kWork + kSemaphoreCount};
  HostSemaphore request_done{kRequestDone + kSemaphoreCount};
  HostSemaphore idle{kIdle + kSemaphoreCount};

  // The one at a guest Semaphore's address, or null.
  HostSemaphore* At(uint32_t address);
  void MirrorAll(uint8_t* base);
  // Whether any of them was waited on or signaled (the live ones: whether
  // the game's code takes them, --native_archive at start-up).
  bool Used() const;
};

// The game's four (shared by every thread).
Semaphores& LiveSemaphores();
// The ones this thread's code takes: the harness's test set while a
// ScopedTestSemaphores is alive on this thread, else the live ones.
Semaphores& CurrentSemaphores();
// The harness's test set on this thread, or null.
Semaphores* TestSemaphores();

// While alive, the archive's code on this thread (native, and recompiled
// through the adapters) takes a set of its own instead of the live four, so
// the harness's cases can't wake or block the live loader thread. Their
// counts are mirrored into the same guest words (the recompiled readers read
// them there); when it ends, the live counts are written back (or, when the
// live ones aren't used, the guest words as they were: the game's own
// Semaphores then).
class ScopedTestSemaphores {
 public:
  explicit ScopedTestSemaphores(uint8_t* base);
  ~ScopedTestSemaphores();
  Semaphores& Set() { return set_; }

 private:
  uint8_t* base_;
  Semaphores set_;
  Semaphores* previous_;
  uint32_t saved_[4];
};

}  // namespace archive
