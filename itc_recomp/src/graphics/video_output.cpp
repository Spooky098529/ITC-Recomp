// The video output's device lock (docs/functions/video-output.md): see
// video_output.h.

#include "graphics/video_output.h"

#include "guest.h"

#include "base/log.h"

#include <rex/system/xthread.h>

#include <atomic>
#include <cstdlib>
#include <mutex>
#include <semaphore>

namespace graphics::video_output {

namespace {

// One device lock a video output (the game makes one; the harness its own
// on a scratch one), found by the video output's address. Entries are never
// removed: the game's video output lives until the process ends (its release,
// sub_8258DBA8, never runs).
struct Lock {
  std::atomic<uint32_t> video_output{0};
  std::binary_semaphore semaphore{1};
  std::atomic<int> held{0};  // a check only: given back more than taken
};

constexpr int kLocks = 8;
Lock g_locks[kLocks];
std::atomic<int> g_count{0};
std::mutex g_register;

Lock* Find(uint32_t video_output) {
  const int count = g_count.load(std::memory_order_acquire);
  for (int i = 0; i < count; i++) {
    if (g_locks[i].video_output.load(std::memory_order_relaxed) == video_output) {
      return &g_locks[i];
    }
  }
  return nullptr;
}

// The lock for `video_output`, made when there's none. With `reset`, one
// made before is put back to a count of 1 (the constructor run again over
// it: only the harness does).
Lock* Register(uint32_t video_output, bool reset) {
  std::lock_guard<std::mutex> guard(g_register);
  if (Lock* lock = Find(video_output)) {
    if (reset) {
      (void)lock->semaphore.try_acquire();
      lock->semaphore.release();
      lock->held.store(0, std::memory_order_relaxed);
    }
    return lock;
  }
  const int count = g_count.load(std::memory_order_relaxed);
  if (count == kLocks) {
    ITC_LOG_ERROR("video output: more than {} video outputs made ({:08X})", kLocks,
                  video_output);
    std::abort();
  }
  g_locks[count].video_output.store(video_output, std::memory_order_relaxed);
  g_count.store(count + 1, std::memory_order_release);
  return &g_locks[count];
}

Lock& LockOf(uint32_t video_output) {
  if (Lock* lock = Find(video_output)) {
    return *lock;
  }
  ITC_LOG_ERROR("video output: the device lock of {:08X} used before its constructor",
                video_output);
  return *Register(video_output, false);
}

}  // namespace

uint32_t CurrentThreadId() {
  ITC_NATIVE_RUNNING("sub_825B9A60 CurrentThreadId");
  return rex::system::XThread::GetCurrentThreadId();
}

uint32_t AcquireThreadOwnership(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229D9D0 Device::AcquireThreadOwnership");
  // The original reads the thread id once more first when the word was
  // already set, and drops it (a leaf load: nothing to do).
  const uint32_t thread = CurrentThreadId();
  guest::StoreU32(base, device + kDeviceThread, thread);
  return thread;
}

void ReleaseThreadOwnership(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229DA10 Device::ReleaseThreadOwnership");
  // The original's eieio: the device's stores before this one.
  std::atomic_thread_fence(std::memory_order_release);
  guest::StoreU32(base, device + kDeviceThread, 0);
}

uint32_t Construct(uint8_t* base, uint32_t video_output) {
  ITC_NATIVE_RUNNING("sub_8258DB18 VideoOutput::VideoOutput");
  guest::StoreU32(base, video_output + 0, kVtable);
  // In place of Semaphore::Init on +224 (whose 40 bytes are left as they are).
  Register(video_output, true);
  for (uint32_t offset : {140u, 144u, 148u, 268u, 272u, 276u, 280u, 284u, 288u}) {
    guest::StoreU32(base, video_output + offset, 0);
  }
  guest::StoreU8(base, video_output + 264, 0);
  guest::StoreU8(base, video_output + 265, 0);
  guest::StoreU8(base, video_output + 266, 0);
  guest::StoreU8(base, video_output + 267, 255);
  guest::StoreU32(base, video_output + 296, 0);
  guest::StoreU32(base, video_output + 300, 0);
  guest::StoreU32(base, kVblankCount, 0);
  guest::StoreU8(base, video_output + 292, 0);
  return video_output;
}

uint32_t LockDevice(uint8_t* base, uint32_t video_output) {
  ITC_NATIVE_RUNNING("sub_8258D8C8 VideoOutput::LockDevice");
  Lock& lock = LockOf(video_output);
  lock.semaphore.acquire();
  lock.held.fetch_add(1, std::memory_order_relaxed);
  AcquireThreadOwnership(base, guest::LoadU32(base, video_output + kDevice));
  return guest::LoadU32(base, video_output + kDevice);
}

void UnlockDevice(uint8_t* base, uint32_t video_output, uint32_t local) {
  ITC_NATIVE_RUNNING("sub_8258D908 VideoOutput::UnlockDevice");
  ReleaseThreadOwnership(base, guest::LoadU32(base, local));
  guest::StoreU32(base, local, 0);
  Lock& lock = LockOf(video_output);
  if (lock.held.fetch_sub(1, std::memory_order_relaxed) <= 0) {
    // The game's semaphore would count up to 2; a binary semaphore can't.
    // Never seen: logged, and the lock left free.
    lock.held.fetch_add(1, std::memory_order_relaxed);
    static std::atomic<bool> logged{false};
    if (!logged.exchange(true)) {
      ITC_LOG_ERROR("video output: the device lock of {:08X} given back when not held",
                    video_output);
    }
    return;
  }
  lock.semaphore.release();
}

}  // namespace graphics::video_output
