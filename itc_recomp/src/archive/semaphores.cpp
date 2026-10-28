// The archive loader's four semaphores: see semaphores.h.

#include "archive/semaphores.h"

#include "base/log.h"
#include "guest.h"

namespace archive {

HostSemaphore::HostSemaphore(uint32_t mirror, int32_t count) : count_(count), mirror_(mirror) {}

void HostSemaphore::Mirror(uint8_t* base) {
  if (mirror_ != 0) {
    guest::StoreU32(base, mirror_, uint32_t(count_.load(std::memory_order_relaxed)));
  }
}

void HostSemaphore::Wait(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82592670 Semaphore::Wait (the archive's four, host)");
  std::unique_lock<std::mutex> lock(mutex_);
  used_.store(true, std::memory_order_release);
  if (count_.load(std::memory_order_relaxed) <= 0) {
    waiters_.fetch_add(1, std::memory_order_acq_rel);
    wake_.wait(lock, [&] { return count_.load(std::memory_order_relaxed) > 0; });
    waiters_.fetch_sub(1, std::memory_order_acq_rel);
  }
  count_.fetch_sub(1, std::memory_order_acq_rel);
  Mirror(base);
}

void HostSemaphore::Signal(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825926E8 Semaphore::Signal (the archive's four, host)");
  {
    std::lock_guard<std::mutex> lock(mutex_);
    used_.store(true, std::memory_order_release);
    count_.fetch_add(1, std::memory_order_acq_rel);
    Mirror(base);
  }
  wake_.notify_one();
}

void HostSemaphore::SetCount(uint8_t* base, int32_t count) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    count_.store(count, std::memory_order_release);
    Mirror(base);
  }
  wake_.notify_all();
}

HostSemaphore* Semaphores::At(uint32_t address) {
  switch (address) {
    case kQueueLock:
      return &queue_lock;
    case kWork:
      return &work;
    case kRequestDone:
      return &request_done;
    case kIdle:
      return &idle;
    default:
      return nullptr;
  }
}

void Semaphores::MirrorAll(uint8_t* base) {
  for (HostSemaphore* s : {&queue_lock, &work, &request_done, &idle}) {
    s->Mirror(base);
  }
}

bool Semaphores::Used() const {
  return queue_lock.Used() || work.Used() || request_done.Used() || idle.Used();
}

namespace {
constexpr uint32_t kAll[4] = {kQueueLock, kWork, kRequestDone, kIdle};
thread_local Semaphores* g_test = nullptr;
}  // namespace

Semaphores& LiveSemaphores() {
  static Semaphores live;
  return live;
}

Semaphores* TestSemaphores() {
  return g_test;
}

Semaphores& CurrentSemaphores() {
  return g_test ? *g_test : LiveSemaphores();
}

ScopedTestSemaphores::ScopedTestSemaphores(uint8_t* base) : base_(base), previous_(g_test) {
  for (int i = 0; i < 4; i++) {
    saved_[i] = guest::LoadU32(base, kAll[i] + kSemaphoreCount);
  }
  set_.MirrorAll(base);
  g_test = &set_;
}

ScopedTestSemaphores::~ScopedTestSemaphores() {
  g_test = previous_;
  Semaphores& outer = previous_ ? *previous_ : LiveSemaphores();
  if (outer.Used()) {
    outer.MirrorAll(base_);
  } else {
    for (int i = 0; i < 4; i++) {
      guest::StoreU32(base_, kAll[i] + kSemaphoreCount, saved_[i]);
    }
  }
}

}  // namespace archive
