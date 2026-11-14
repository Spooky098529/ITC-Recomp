// The C runtime's core (core.h, docs/functions/c-runtime-core.md): step for
// step as the recompiled functions, the kernel imports guest calls.

#include "c_runtime/core.h"

#include "base/log.h"
#include "guest.h"

namespace c_runtime::core {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

}  // namespace

uint32_t CrtTlsAlloc(uint32_t callback, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA3F8 __crtTlsAlloc");
  return uint32_t(calls.import(Import::kKeTlsAlloc, {callback}));
}

uint32_t SetFlsGetValue(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA400 __set_flsgetvalue");
  const uint32_t value =
      uint32_t(calls.import(Import::kKeTlsGetValue, {Load(base, kGetValueIndex)}));
  if (value != 0) {
    return value;
  }
  return uint32_t(calls.import(Import::kKeTlsSetValue,
                               {Load(base, kGetValueIndex), Load(base, kFlsGetValue)}));
}

uint32_t GetptdNoexit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA4C0 _getptd_noexit");
  const uint32_t saved = calls.get_last_error();
  SetFlsGetValue(base, calls);
  const uint32_t getter =
      uint32_t(calls.import(Import::kKeTlsGetValue, {Load(base, kGetValueIndex)}));
  uint32_t data = uint32_t(calls.guest(getter, {Load(base, kFlsIndex)}));
  if (data == 0) {
    data = calls.calloc(1, kThreadDataBytes);
    if (data != 0) {
      const uint32_t stored = uint32_t(
          calls.guest(Load(base, kFlsSetValue), {Load(base, kFlsIndex), data}));
      if (stored != 0) {
        Store(base, data + 20, 1);
        Store(base, data + 92, kXcptActTab);
        Store(base, data + 0, calls.thread_id());
        Store(base, data + 4, 0xFFFFFFFFu);
      } else {
        calls.free(data);
        data = 0;
      }
    }
  }
  calls.set_last_error(saved);
  return data;
}

void FreeFls(uint8_t* base, uint32_t data, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA5C0 _freefls");
  if (data == 0) {
    return;
  }
  for (uint32_t offset : {36u, 44u, 52u, 60u, 68u, 72u}) {
    if (const uint32_t buffer = Load(base, data + offset); buffer != 0) {
      calls.free(buffer);
    }
  }
  if (const uint32_t table = Load(base, data + 92); table != kXcptActTab) {
    calls.free(table);
  }
  calls.free(data);
}

void MtDeleteLocks(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3260 _mtdeletelocks");
  for (uint32_t entry = kLockTable; entry < kLockTable + 8 * kLockCount; entry += 8) {
    const uint32_t section = Load(base, entry + 0);
    if (section != 0 && Load(base, entry + 4) != 1) {
      calls.free(section);
      Store(base, entry + 0, 0);
    }
  }
}

void MtTerm(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA448 _mtterm");
  if (const uint32_t index = Load(base, kFlsIndex); index != 0xFFFFFFFFu) {
    calls.guest(Load(base, kFlsFree), {index});
    Store(base, kFlsIndex, 0xFFFFFFFFu);
  }
  if (const uint32_t index = Load(base, kGetValueIndex); index != 0xFFFFFFFFu) {
    calls.import(Import::kKeTlsFree, {index});
    Store(base, kGetValueIndex, 0xFFFFFFFFu);
  }
  MtDeleteLocks(base, calls);
}

uint32_t Errno(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A81E0 _errno");
  const uint32_t data = GetptdNoexit(base, calls);
  return data != 0 ? data + 8 : kErrnoFallback;
}

uint32_t DosErrno(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A8218 __doserrno");
  const uint32_t data = GetptdNoexit(base, calls);
  return data != 0 ? data + 12 : kDosErrnoFallback;
}

uint32_t GetErrnoFromOsErr(uint8_t* base, uint32_t error) {
  ITC_NATIVE_RUNNING("sub_825A8178 _get_errno_from_oserr");
  for (uint32_t i = 0; i < 45; i++) {
    if (Load(base, kOsErrorTable + 8 * i) == error) {
      return Load(base, kOsErrorTable + 8 * i + 4);
    }
  }
  if (error - 19 <= 17) {
    return 13;
  }
  return error - 188 <= 14 ? 8 : 22;
}

void DosMapErr(uint8_t* base, uint32_t error, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A8250 _dosmaperr");
  const uint32_t data = GetptdNoexit(base, calls);
  Store(base, data != 0 ? data + 12 : kDosErrnoFallback, error);
  const uint32_t number = GetErrnoFromOsErr(base, error);
  const uint32_t again = GetptdNoexit(base, calls);
  Store(base, again != 0 ? again + 8 : kErrnoFallback, number);
}

void FreePtd(uint8_t* base, uint32_t data, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA670 _freeptd");
  if (Load(base, kFlsIndex) != 0xFFFFFFFFu) {
    if (data == 0 &&
        calls.import(Import::kKeTlsGetValue, {Load(base, kGetValueIndex)}) != 0) {
      const uint32_t getter =
          uint32_t(calls.import(Import::kKeTlsGetValue, {Load(base, kGetValueIndex)}));
      data = uint32_t(calls.guest(getter, {Load(base, kFlsIndex)}));
    }
    calls.guest(Load(base, kFlsSetValue), {Load(base, kFlsIndex), 0});
    FreeFls(base, data, calls);
  }
  if (const uint32_t index = Load(base, kGetValueIndex); index != 0xFFFFFFFFu) {
    calls.import(Import::kKeTlsSetValue, {index, 0});
  }
}

uint32_t CritSecNoSpinCount(uint32_t section, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3570 __crtInitCritSecNoSpinCount");
  calls.import(Import::kRtlInitializeCriticalSection, {section});
  return 1;
}

uint32_t InitCritSecAndSpinCount(uint8_t* base, uint32_t section, uint32_t spin,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B35A0 __crtInitCritSecAndSpinCount");
  uint32_t init = Load(base, kCritSecInit);
  if (init == 0) {
    init = kCritSecNoSpinCount;
    Store(base, kCritSecInit, init);
  }
  // The original's __try (its handler never ran in play) has nothing to
  // catch here: the initializer is ours or the game's.
  if (init == kCritSecNoSpinCount) {
    return CritSecNoSpinCount(section, calls);
  }
  return uint32_t(calls.guest(init, {section, spin}));
}

void RegisterThreadNotifyRoutine(uint8_t* base, uint32_t block, uint32_t add,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82297B98 XRegisterThreadNotifyRoutine");
  calls.import(Import::kRtlEnterCriticalSection, {kNotifyLock});
  if (add != 0) {
    const uint32_t last = Load(base, kNotifyList + 4);
    Store(base, block + 0, kNotifyList);
    Store(base, block + 4, last);
    Store(base, last + 0, block);
    Store(base, kNotifyList + 4, block);
  } else {
    const uint32_t next = Load(base, block + 0);
    const uint32_t previous = Load(base, block + 4);
    Store(base, previous + 0, next);
    Store(base, next + 4, previous);
  }
  calls.import(Import::kRtlLeaveCriticalSection, {kNotifyLock});
}

void CallThreadNotifyRoutines(uint8_t* base, uint32_t starting, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82297B38 XapiCallThreadNotifyRoutines");
  calls.import(Import::kRtlEnterCriticalSection, {kNotifyLock});
  for (uint32_t block = Load(base, kNotifyList); block != kNotifyList;) {
    const uint32_t routine = Load(base, block + 8);
    block = Load(base, block + 0);
    calls.guest(routine, {starting});
  }
  calls.import(Import::kRtlLeaveCriticalSection, {kNotifyLock});
}

}  // namespace c_runtime::core
