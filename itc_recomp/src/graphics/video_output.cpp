// The video output's device lock (docs/functions/video-output.md): see
// video_output.h.

#include "graphics/video_output.h"

#include "threads/threads.h"

#include "guest.h"
#include "schema/device_state.h"
#include "schema/video_output.h"

#include "base/log.h"


#include <atomic>
#include <cstdlib>
#include <mutex>
#include <semaphore>

namespace graphics::video_output {

namespace {

using Globals = schema::VideoOutputGlobals;

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
  // The thread object's ID ([[r13 + 256] + 0x14C]): every thread running
  // game code is ours since E1; 0 (logged once) on any other.
  if (const uint32_t id = threads::CurrentThreadIdIfOurs()) {
    return id;
  }
  static std::once_flag logged;
  std::call_once(logged, [] {
    ITC_LOG_ERROR("video output: CurrentThreadId on a thread that isn't one of ours");
  });
  return 0;
}

uint32_t AcquireThreadOwnership(uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229D9D0 Device::AcquireThreadOwnership");
  // The original reads the thread id once more first when the word was
  // already set, and drops it (a leaf load: nothing to do).
  const uint32_t thread = CurrentThreadId();
  schema::Device(device).set_owning_thread(thread);
  return thread;
}

void ReleaseThreadOwnership(uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229DA10 Device::ReleaseThreadOwnership");
  // The original's eieio: the device's stores before this one.
  std::atomic_thread_fence(std::memory_order_release);
  schema::Device(device).set_owning_thread(0);
}

uint32_t Construct(uint32_t video_output) {
  ITC_NATIVE_RUNNING("sub_8258DB18 VideoOutput::VideoOutput");
  const schema::VideoOutput vo(video_output);
  vo.set_vtable(kVtable);
  // In place of Semaphore::Init on +224 (whose 40 bytes are left as they are).
  Register(video_output, true);
  vo.set_device(0u);
  vo.set_unknown_144(0);
  vo.set_unknown_148(0);
  vo.set_front_buffer_index(0);
  vo.set_front_buffer_0(0);
  vo.set_front_buffer_1(0);
  vo.set_unknown_280(0);
  vo.set_unknown_284(0);
  vo.set_unknown_288(0);
  vo.set_unknown_264(0);
  vo.set_unknown_265(0);
  vo.set_unknown_266(0);
  vo.set_unknown_267(255);
  vo.set_display_width(0);
  vo.set_display_height(0);
  Globals::set_vblank_count(0);
  vo.set_unknown_292(0);
  return video_output;
}

uint32_t LockDevice(uint32_t video_output) {
  ITC_NATIVE_RUNNING("sub_8258D8C8 VideoOutput::LockDevice");
  Lock& lock = LockOf(video_output);
  lock.semaphore.acquire();
  lock.held.fetch_add(1, std::memory_order_relaxed);
  const schema::VideoOutput vo(video_output);
  AcquireThreadOwnership(vo.device<schema::View>().address);
  return vo.device<schema::View>().address;
}

void UnlockDevice(uint32_t video_output, uint32_t local) {
  ITC_NATIVE_RUNNING("sub_8258D908 VideoOutput::UnlockDevice");
  const schema::VideoOutputDeviceHolder holder(local);
  ReleaseThreadOwnership(holder.device<schema::View>().address);
  holder.set_device(0u);
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

uint32_t ResolveFrontBuffer(uint32_t video_output, uint32_t sp,
                            const replay_commands::GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258D998 VideoOutput::ResolveFrontBuffer");
  const uint32_t frame = sp - 112;
  guest::StoreU32(frame, sp);
  const schema::VideoOutput vo(video_output);
  const uint32_t front = vo.front_buffer_index();
  const uint32_t device = vo.device<schema::View>().address;
  guest::StoreU32(frame + 100, 0);
  guest::StoreU32(frame + 92, 0);
  const double z = double(Globals::resolve_z());
  const uint32_t texture = guest::LoadU32(vo.front_buffer_0_address() + 4 * front);
  return uint32_t(call(0x822AFA58, frame, 0x8258D9E8, {device, 0, 0, texture, 0, 0, 0, 0}, z));
}

}  // namespace graphics::video_output
