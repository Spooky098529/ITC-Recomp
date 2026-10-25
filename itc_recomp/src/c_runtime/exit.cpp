// The C runtime's exit table and exit lock natively (exit.h), storing what
// the translated code stores, in the same order.

#include "c_runtime/exit.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

#include "guest.h"

#include <algorithm>

namespace c_runtime {

std::recursive_mutex& ExitMutex() {
  static std::recursive_mutex mutex;
  return mutex;
}

uint32_t LockExit(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B3430 _lock");
  ExitMutex().lock();
  return guest::LoadU32(base, kLockTable + 8 * kExitLock);
}

uint32_t UnlockExit(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B32D0 _unlock");
  ExitMutex().unlock();
  return guest::LoadU32(base, kLockTable + 8 * kExitLock);
}

std::optional<uint32_t> OnExit(uint8_t* base, uint32_t function) {
  ITC_NATIVE_RUNNING("sub_825A6F60 _onexit");
  // _lockexit and _unlockexit (sub_825AA1A8, sub_825A7044 over sub_825AA1B0):
  // lock 8, the host mutex.
  std::lock_guard<std::recursive_mutex> lock(ExitMutex());
  const uint32_t begin = guest::LoadU32(base, kExitTableBegin);
  const uint32_t end = guest::LoadU32(base, kExitTableEnd);
  if (end < begin) {
    return 0u;
  }
  const uint32_t used = end - begin;
  const uint32_t needed = used + 4;
  if (needed < 4) {
    return 0u;
  }
  uint32_t slot = end;
  const std::optional<uint32_t> size = memory::crt::Msize(begin);
  if (!size) {
    return std::nullopt;
  }
  if (*size < needed) {
    uint32_t table = 0;
    const uint32_t first = *size + std::min(*size, 2048u);
    if (first >= *size) {
      const std::optional<uint32_t> grown = memory::crt::Realloc(base, begin, first);
      if (!grown) {
        return std::nullopt;
      }
      table = *grown;
    }
    if (table == 0) {
      const uint32_t second = *size + 16;
      if (second < *size) {
        return 0u;
      }
      const std::optional<uint32_t> grown = memory::crt::Realloc(base, begin, second);
      if (!grown) {
        return std::nullopt;
      }
      table = *grown;
      if (table == 0) {
        return 0u;
      }
    }
    guest::StoreU32(base, kExitTableBegin, table);
    // srawi 2, then the word's offset: the used bytes with their low two
    // bits cleared.
    slot = table + (used & ~3u);
  }
  guest::StoreU32(base, slot, function);
  guest::StoreU32(base, kExitTableEnd, slot + 4);
  return function;
}

std::optional<int32_t> Atexit(uint8_t* base, uint32_t function) {
  ITC_NATIVE_RUNNING("sub_825A7068 atexit");
  const std::optional<uint32_t> result = OnExit(base, function);
  if (!result) {
    return std::nullopt;
  }
  return *result == 0 ? -1 : 0;
}

int32_t AtexitFrom(uint8_t* base, uint32_t function, const AtexitCall& call) {
  if (const std::optional<int32_t> result = Atexit(base, function)) {
    return *result;
  }
  return call.guest(call.user, function);
}

}  // namespace c_runtime
