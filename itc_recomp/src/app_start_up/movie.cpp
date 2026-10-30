// The movie class's construction natively (start_up.h,
// docs/functions/movies.md: Movie::Movie, MovieWrapper::MovieWrapper) and
// the host mutex in place of the wrapper's critical section.

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <atomic>
#include <mutex>

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The wrapper's block (its critical section at +0) the native constructor
// made, 0 before (or when the recompiled constructor made it).
std::atomic<uint32_t> g_wrapper_lock{0};

}  // namespace

std::recursive_mutex& MovieWrapperMutex() {
  static std::recursive_mutex mutex;
  return mutex;
}

uint32_t MovieWrapperLock() { return g_wrapper_lock.load(std::memory_order_acquire); }
void SetMovieWrapperLock(uint32_t critical_section) {
  g_wrapper_lock.store(critical_section, std::memory_order_release);
}
bool IsMovieWrapperLock(uint32_t critical_section) {
  return critical_section != 0 && critical_section == MovieWrapperLock();
}

void MovieWrapperLockEnter(uint8_t* base, uint32_t lock, uint32_t critical_section) {
  ITC_NATIVE_RUNNING("sub_8258E580 ScopedLock::ScopedLock (the movie wrapper's lock)");
  Store(base, lock, critical_section);
  MovieWrapperMutex().lock();
}

uint32_t MovieWrapperLockLeave(uint8_t* base, uint32_t lock) {
  ITC_NATIVE_RUNNING("sub_8258E5B8 ScopedLock::~ScopedLock (the movie wrapper's lock)");
  const uint32_t critical_section = Load(base, lock);
  MovieWrapperMutex().unlock();
  return critical_section;
}

std::optional<uint32_t> MovieWrapperConstruct(uint8_t* base, uint32_t wrapper) {
  Store(base, wrapper + 0, kMovieWrapperVtable);
  const auto block = memory::crt::OperatorNew(base, kMovieWrapperBlockBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82593DC0 MovieWrapper::MovieWrapper");
  // InitializeCriticalSection (sub_8258E548) left out: the host mutex takes
  // the section's place (its 28 bytes left as operator new leaves them, as
  // Edram::Alloc's), for every user (the ScopedLock adapters).
  if (*block != 0) {
    SetMovieWrapperLock(*block);
  }
  Store(base, wrapper + 4, *block);
  // The player (+28) = 0; with no block the original stores at 28 too.
  Store(base, *block + 28, 0);
  return wrapper;
}

std::optional<uint32_t> MovieConstruct(uint8_t* base, uint32_t movie, uint32_t block,
                                       uint32_t frame, const MovieCalls& calls) {
  Store(base, movie + 0, kMovieVtable);
  Store(base, movie + 4, Load(base, block + 4));
  // The command at +12 (as the sound manager's at +1584).
  Store(base, movie + 16, 0x82000AB4);
  Store(base, movie + 20, 0);
  Store(base, movie + 24, 0);
  guest::StoreU8(base, movie + 28, 0);
  guest::StoreU8(base, movie + 29, 1);
  guest::StoreU8(base, movie + 30, 0);
  Store(base, movie + 12, 0x820066DC);
  Store(base, movie + 16, 0x82002908);
  Store(base, movie + 36, kDelegateVtable);
  Store(base, movie + 40, 0);
  Store(base, movie + 44, 0);
  const auto wrapper_block = memory::crt::OperatorNew(base, 8);
  if (!wrapper_block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820AEA88 Movie::Movie");
  uint32_t wrapper = 0;
  if (*wrapper_block != 0) {
    wrapper = calls.wrapper(*wrapper_block);
  }
  Store(base, movie + 8, wrapper);
  // The tick handler (key 1, Movie::OnTick 0x820AE808) and the listener
  // (key 1, 0x820AE818), each a 12-byte delegate on the frame.
  Store(base, frame + 92, movie);
  Store(base, frame + 80, 1);
  Store(base, frame + 88, kDelegateVtable);
  Store(base, frame + 96, 0x820AE808);
  calls.virtual_call(Load(base, block + 0), 8, frame + 80, frame + 88, 0x820AEB64);
  Store(base, frame + 108, movie);
  Store(base, frame + 80, 1);
  Store(base, frame + 88, 0x82000ABC);
  Store(base, frame + 104, kDelegateVtable);
  Store(base, frame + 112, 0x820AE818);
  calls.virtual_call(Load(base, block + 0), 4, frame + 80, frame + 104, 0x820AEBAC);
  Store(base, movie + 308, 0);
  return movie;
}

}  // namespace app_start_up
