// The movie class's construction natively (start_up.h,
// docs/functions/movies.md: Movie::Movie, MovieWrapper::MovieWrapper) and
// the host mutex in place of the wrapper's critical section.

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_start_up.h"
#include "schema/movies.h"

#include <atomic>
#include <mutex>

namespace app_start_up {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
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

void MovieWrapperLockEnter(uint32_t lock, uint32_t critical_section) {
  ITC_NATIVE_RUNNING("sub_8258E580 ScopedLock::ScopedLock (the movie wrapper's lock)");
  Store(lock, critical_section);
  MovieWrapperMutex().lock();
}

uint32_t MovieWrapperLockLeave(uint32_t lock) {
  ITC_NATIVE_RUNNING("sub_8258E5B8 ScopedLock::~ScopedLock (the movie wrapper's lock)");
  const uint32_t critical_section = Load(lock);
  MovieWrapperMutex().unlock();
  return critical_section;
}

std::optional<uint32_t> MovieWrapperConstruct(uint32_t wrapper) {
  const schema::MovieWrapper w(wrapper);
  w.set_vtable(kMovieWrapperVtable);
  // In host memory (E5 step 4; operator new's block before, 0x82593DEC).
  static_assert(schema::MovieWrapperBlock::kSize == kMovieWrapperBlockBytes);
  const uint32_t block = schema::MovieWrapperBlock::New().address;
  ITC_NATIVE_RUNNING("sub_82593DC0 MovieWrapper::MovieWrapper");
  // InitializeCriticalSection (sub_8258E548) left out: the host mutex takes
  // the section's place (its 28 bytes left as New leaves them, as
  // Edram::Alloc's), for every user (the ScopedLock adapters).
  if (block != 0) {
    SetMovieWrapperLock(block);
  }
  w.set_block(block);
  // The player (+28) = 0.
  schema::MovieWrapperBlock(block).set_player(uint32_t{0});
  return wrapper;
}

std::optional<uint32_t> MovieConstruct(uint32_t movie, uint32_t block,
                                       uint32_t frame, const MovieCalls& calls) {
  const schema::Movie m(movie);
  const schema::AppSetUpBlock set_up(block);
  m.set_vtable(kMovieVtable);
  m.set_framework_interface(set_up.framework_interface());
  // The command at +12 (as the sound manager's at +1584).
  m.set_command_vtable_4(0x82000AB4);
  m.set_unknown_20(0);
  m.set_command_status(0);
  m.set_unknown_28(0);
  m.set_unknown_29(1);
  m.set_unknown_30(0);
  m.set_command_vtable(0x820066DC);
  m.set_command_vtable_4(0x82002908);
  m.set_delegate_vtable(kDelegateVtable);
  m.set_delegate_object(uint32_t{0});
  m.set_delegate_function(0);
  // In host memory (E5 step 4; operator new's block before, 0x820AEB10).
  static_assert(schema::MovieWrapper::kSize == 8);
  const uint32_t wrapper_block = schema::MovieWrapper::New().address;
  ITC_NATIVE_RUNNING("sub_820AEA88 Movie::Movie");
  uint32_t wrapper = 0;
  if (wrapper_block != 0) {
    wrapper = calls.wrapper(wrapper_block);
  }
  m.set_wrapper(wrapper);
  // The tick handler (key 1, Movie::OnTick 0x820AE808) and the listener
  // (key 1, 0x820AE818), each a 12-byte delegate on the frame.
  Store(frame + 92, movie);
  Store(frame + 80, 1);
  Store(frame + 88, kDelegateVtable);
  Store(frame + 96, 0x820AE808);
  calls.virtual_call(set_up.run_loop().address, 8, frame + 80, frame + 88, 0x820AEB64);
  Store(frame + 108, movie);
  Store(frame + 80, 1);
  Store(frame + 88, 0x82000ABC);
  Store(frame + 104, kDelegateVtable);
  Store(frame + 112, 0x820AE818);
  calls.virtual_call(set_up.run_loop().address, 4, frame + 80, frame + 104, 0x820AEBAC);
  m.set_state(0);
  return movie;
}

}  // namespace app_start_up
