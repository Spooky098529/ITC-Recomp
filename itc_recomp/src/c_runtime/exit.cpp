// The C runtime's exit table and exit lock natively (exit.h), storing what
// the translated code stores, in the same order.

#include "c_runtime/exit.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

#include "guest.h"
#include "schema/c_runtime.h"
#include "schema/c_runtime_core.h"

#include <algorithm>

namespace c_runtime {

std::recursive_mutex& ExitMutex() {
  static std::recursive_mutex mutex;
  return mutex;
}

namespace {

// Lock 8's entry in the C runtime's lock table (c-runtime-core's).
schema::CrtLockEntry ExitLockEntry() {
  return schema::CrtLockEntry(schema::CRuntimeCoreGlobals::lock_table_address() + 8 * kExitLock);
}

}  // namespace

uint32_t LockExit() {
  ITC_NATIVE_RUNNING("sub_825B3430 _lock");
  ExitMutex().lock();
  return ExitLockEntry().section();
}

uint32_t UnlockExit() {
  ITC_NATIVE_RUNNING("sub_825B32D0 _unlock");
  ExitMutex().unlock();
  return ExitLockEntry().section();
}

std::optional<uint32_t> OnExit(uint32_t function) {
  ITC_NATIVE_RUNNING("sub_825A6F60 _onexit");
  // _lockexit and _unlockexit (sub_825AA1A8, sub_825A7044 over sub_825AA1B0):
  // lock 8, the host mutex.
  std::lock_guard<std::recursive_mutex> lock(ExitMutex());
  const uint32_t begin = schema::CRuntimeGlobals::exit_table_begin().address;
  const uint32_t end = schema::CRuntimeGlobals::exit_table_end();
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
      const std::optional<uint32_t> grown = memory::crt::Realloc(begin, first);
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
      const std::optional<uint32_t> grown = memory::crt::Realloc(begin, second);
      if (!grown) {
        return std::nullopt;
      }
      table = *grown;
      if (table == 0) {
        return 0u;
      }
    }
    schema::CRuntimeGlobals::set_exit_table_begin(table);
    // srawi 2, then the word's offset: the used bytes with their low two
    // bits cleared.
    slot = table + (used & ~3u);
  }
  guest::StoreU32(slot, function);
  schema::CRuntimeGlobals::set_exit_table_end(slot + 4);
  return function;
}

std::optional<uint32_t> InitExitTable() {
  ITC_NATIVE_RUNNING("sub_825A6F00 __onexitinit");
  const std::optional<uint32_t> table = memory::crt::Calloc(32, 4);
  if (!table) {
    return std::nullopt;
  }
  schema::CRuntimeGlobals::set_exit_table_begin(*table);
  schema::CRuntimeGlobals::set_exit_table_end(*table);
  if (*table == 0) {
    return 24u;
  }
  schema::ExitTable(*table).set_first(0);
  return 0u;
}

std::optional<int32_t> Atexit(uint32_t function) {
  ITC_NATIVE_RUNNING("sub_825A7068 atexit");
  const std::optional<uint32_t> result = OnExit(function);
  if (!result) {
    return std::nullopt;
  }
  return *result == 0 ? -1 : 0;
}

int32_t AtexitFrom(uint32_t function, const AtexitCall& call) {
  if (const std::optional<int32_t> result = Atexit(function)) {
    return *result;
  }
  return call.guest(call.user, function);
}

}  // namespace c_runtime
