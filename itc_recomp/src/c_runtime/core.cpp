// The C runtime's core (core.h, docs/functions/c-runtime-core.md): step for
// step as the recompiled functions, the kernel imports guest calls.

#include "c_runtime/core.h"

#include "base/log.h"
#include "guest.h"
#include "c_runtime/format.h"
#include "c_runtime/parse.h"
#include "c_runtime/start_up.h"
#include "c_runtime/strings.h"
#include "helpers/debug_output.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <climits>
#include <cmath>
#include <emmintrin.h>
#include <cstring>

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

uint32_t MtInitLocks(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B31D8 _mtinitlocks");
  uint32_t section = kLockSections;
  for (uint32_t i = 0; i < kLockCount; i++) {
    const uint32_t entry = kLockTable + 8 * i;
    if (Load(base, entry + 4) != 1) {
      continue;
    }
    Store(base, entry + 0, section);
    const uint32_t made = InitCritSecAndSpinCount(base, section, 4000, calls);
    section += 28;
    if (made == 0) {
      Store(base, entry + 0, 0);
      return 0;
    }
  }
  return 1;
}

void ThreadNotify(uint8_t* base, uint32_t starting, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3650 the C runtime's thread notify routine");
  if (starting != 0) {
    if (const uint32_t hook = Load(base, kThreadStartHook); hook != 0) {
      calls.guest(hook, {starting});
    }
    return;
  }
  if (const uint32_t hook = Load(base, kThreadEndHook); hook != 0) {
    calls.guest(hook, {0});
  }
  FreePtd(base, 0, calls);
}

uint32_t MtInit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA708 _mtinit");
  Store(base, kFlsAlloc, 0x825AA3F8);      // __crtTlsAlloc
  Store(base, kFlsGetValue, 0x825DBBF4);   // KeTlsGetValue's thunk
  Store(base, kFlsSetValue, 0x825DBC04);   // KeTlsSetValue's
  Store(base, kFlsFree, 0x825DBC24);       // KeTlsFree's
  const uint32_t getter_index = uint32_t(calls.import(Import::kKeTlsAlloc, {}));
  Store(base, kGetValueIndex, getter_index);
  if (getter_index == 0xFFFFFFFFu ||
      calls.import(Import::kKeTlsSetValue, {getter_index, Load(base, kFlsGetValue)}) == 0) {
    return 0;
  }
  c_runtime::InitPointers(base);
  auto fail = [&]() {
    MtTerm(base, calls);
    return 0u;
  };
  if (MtInitLocks(base, calls) == 0) {
    return fail();
  }
  const uint32_t fls_index = uint32_t(calls.guest(Load(base, kFlsAlloc), {0x825AA5C0}));
  Store(base, kFlsIndex, fls_index);
  if (fls_index == 0xFFFFFFFFu) {
    return fail();
  }
  const uint32_t data = calls.calloc(1, kThreadDataBytes);
  if (data == 0) {
    return fail();
  }
  if (calls.guest(Load(base, kFlsSetValue), {Load(base, kFlsIndex), data}) == 0) {
    return fail();
  }
  Store(base, data + 20, 1);
  Store(base, data + 92, kXcptActTab);
  const uint32_t id = calls.thread_id();
  Store(base, data + 0, id);
  Store(base, data + 4, 0xFFFFFFFFu);
  Store(base, kCrtNotifyBlock + 8, kCrtNotifyRoutine);
  RegisterThreadNotifyRoutine(base, kCrtNotifyBlock, 1, calls);
  return 1;
}

bool Strtol(uint8_t* base, uint32_t string, uint32_t end_out, int32_t radix, uint64_t* result) {
  ITC_NATIVE_RUNNING("sub_825A85C0 strtol");
  uint32_t end = 0;
  if (!c_runtime::StrToXl(base, string, radix, 0, end_out != 0, &end, result)) {
    return false;
  }
  if (end_out != 0) {
    Store(base, end_out, end);
  }
  return true;
}

uint32_t Strrchr(uint8_t* base, uint32_t string, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_825A8630 strrchr");
  uint32_t found = 0;
  for (uint32_t p = string;; p++) {
    const uint32_t b = guest::LoadU8(base, p);
    if (b == c) {
      if (c == 0) {
        return p;
      }
      found = p;
    }
    if (b == 0) {
      return found;
    }
  }
}

void Initterm(uint8_t* base, uint32_t begin, uint32_t end, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA1B8 _initterm");
  for (uint32_t p = begin; p < end; p += 4) {
    if (const uint32_t function = Load(base, p); function != 0) {
      calls.guest(function, {});
    }
  }
}

uint32_t SetAbortBehavior(uint8_t* base, uint32_t flags, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_825AD120 _set_abort_behavior");
  const uint32_t old = Load(base, 0x8266B7C4);
  Store(base, 0x8266B7C4, (old & ~mask) | (flags & mask));
  return old;
}

uint32_t GetSigabrt(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B49B0 __get_sigabrt");
  return Load(base, 0x82971340);
}

void CrtDebuggerHook(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B68B8 _crt_debugger_hook");
  Store(base, 0x8297820C, 0);
}

uint32_t ValidateExecute(uint32_t pointer) {
  ITC_NATIVE_RUNNING("sub_825B8910 _ValidateExecute");
  return pointer != 0 ? 1 : 0;
}

uint32_t SetUnhandledExceptionFilter(uint8_t* base, uint32_t filter) {
  ITC_NATIVE_RUNNING("sub_82297870 SetUnhandledExceptionFilter");
  const uint32_t old = Load(base, 0x829714A0);
  Store(base, 0x829714A0, filter);
  return old;
}

uint32_t GetCommandLineA(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822979D0 GetCommandLineA");
  return Load(base, 0x820007EC);
}

uint32_t Positive(uint8_t* base, uint32_t number) {
  ITC_NATIVE_RUNNING("sub_825ADE90 _positive");
  auto load = [&](uint32_t address) {
    return std::bit_cast<double>(uint64_t(Load(base, address)) << 32 | Load(base, address + 4));
  };
  const double x = load(number);
  const double zero = load(0x82002258);
  return x < zero ? 0 : 1;
}

void NmsgWrite(uint8_t* base, uint32_t number) {
  ITC_NATIVE_RUNNING("sub_825AEDA0 _NMSG_WRITE");
  for (uint32_t i = 0; i < 23; i++) {
    if (Load(base, 0x8266BA88 + 8 * i) == number) {
      helpers::DebugOutput(nullptr);
      return;
    }
  }
}

void Getbuf(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4790 _getbuf");
  Store(base, 0x82971174, Load(base, 0x82971174) + 1);
  const uint32_t buffer = calls.malloc(4096);
  Store(base, stream + 8, buffer);
  if (buffer != 0) {
    Store(base, stream + 24, 4096);
    Store(base, stream + 12, Load(base, stream + 12) | 8);
  } else {
    Store(base, stream + 8, stream + 20);
    Store(base, stream + 24, 2);
    Store(base, stream + 12, Load(base, stream + 12) | 4);
  }
  Store(base, stream + 4, 0);
  Store(base, stream + 0, Load(base, stream + 8));
}

void Freebuf(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0EE0 _freebuf");
  const uint32_t flags = Load(base, stream + 12);
  if ((flags & 0x83) == 0 || (flags & 8) == 0) {
    return;
  }
  calls.free(Load(base, stream + 8));
  const uint32_t after = Load(base, stream + 12) & ~0x8u & ~0x400u;
  Store(base, stream + 0, 0);
  Store(base, stream + 8, 0);
  Store(base, stream + 4, 0);
  Store(base, stream + 12, after);
}

uint32_t CloseHandle(uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82294E20 CloseHandle");
  const uint32_t status = uint32_t(calls.import(Import::kNtClose, {handle}));
  if (int32_t(status) < 0) {
    calls.set_last_error_from_status(status);
    return 0;
  }
  return 1;
}

uint32_t ResetEvent(uint32_t event, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825BA168 ResetEvent");
  const uint32_t status = uint32_t(calls.import(Import::kNtClearEvent, {event}));
  if (int32_t(status) < 0) {
    calls.set_last_error_from_status(status);
    return 0;
  }
  return 1;
}

uint64_t InittermE(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82297AC0 _initterm_e");
  uint64_t result = 0;
  for (uint32_t p = 0x82600000; p < 0x8260000C && uint32_t(result) == 0; p += 4) {
    if (const uint32_t function = Load(base, p); function != 0) {
      result = calls.guest(function, {0});
    }
  }
  return result;
}

uint32_t IoInit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4668 _ioinit");
  const uint32_t block = calls.calloc(32, 44);
  if (block == 0) {
    return 0xFFFFFFFFu;
  }
  Store(base, kNhandle, 32);
  Store(base, kPioinfo, block);
  for (uint32_t entry = block; entry < block + 32 * 44; entry += 44) {
    guest::StoreU8(base, entry + 4, 0);
    guest::StoreU8(base, entry + 5, 10);
    Store(base, entry + 8, 0);
    guest::StoreU8(base, entry + 41, 10);
    guest::StoreU8(base, entry + 42, 10);
    guest::StoreU8(base, entry + 40, 0);
    Store(base, entry + 0, 0xFFFFFFFFu);
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = Load(base, kPioinfo) + 44 * i;
    guest::StoreU8(base, entry + 4, 0xC1);
    Store(base, entry + 0, 0xFFFFFFFEu);
  }
  return 0;
}

uint32_t InitStdio(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD4D0 _initstdio");
  int32_t count = int32_t(Load(base, kNstream));
  if (count == 0) {
    count = 512;
    Store(base, kNstream, uint32_t(count));
  } else if (count < 20) {
    count = 20;
    Store(base, kNstream, uint32_t(count));
  }
  uint32_t piob = calls.calloc(uint32_t(count), 4);
  Store(base, kPiob, piob);
  if (piob == 0) {
    Store(base, kNstream, 20);
    piob = calls.calloc(20, 4);
    Store(base, kPiob, piob);
    if (piob == 0) {
      return 26;
    }
  }
  for (uint32_t i = 0; i < 20; i++) {
    Store(base, Load(base, kPiob) + 4 * i, kIob + 32 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t handle = Load(base, Load(base, kPioinfo + 4 * (i >> 5)) + 44 * (i & 31));
    if (handle == 0xFFFFFFFFu || handle == 0xFFFFFFFEu || handle == 0) {
      Store(base, kIob + 32 * i + 16, 0xFFFFFFFEu);
    }
  }
  return 0;
}

void FfMsgBanner(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825AEDE8 _FF_MSGBANNER");
  NmsgWrite(base, 252);
  NmsgWrite(base, 255);
}

uint32_t LockFile(uint8_t* base, uint32_t stream, bool lock, const Calls& calls) {
  if (lock) {
    ITC_NATIVE_RUNNING("sub_825AD638 _lock_file");
  } else {
    ITC_NATIVE_RUNNING("sub_825AD688 _unlock_file");
  }
  (void)base;
  if (stream >= kIob && stream <= kIob + 608) {
    const uint32_t index = uint32_t(int32_t(stream - kIob) >> 5);
    return uint32_t(calls.guest(lock ? kLock : kUnlock, {16 + index}));
  }
  return uint32_t(calls.import(
      lock ? Import::kRtlEnterCriticalSection : Import::kRtlLeaveCriticalSection,
      {stream + 32}));
}

uint32_t LockFile2(uint32_t index, uint32_t stream, bool lock, const Calls& calls) {
  if (lock) {
    ITC_NATIVE_RUNNING("sub_825AD670 _lock_file2");
  } else {
    ITC_NATIVE_RUNNING("sub_825AD6C0 _unlock_file2");
  }
  if (int32_t(index) < 20) {
    return uint32_t(calls.guest(lock ? kLock : kUnlock, {uint64_t(int64_t(int32_t(index) + 16))}));
  }
  return uint32_t(calls.import(
      lock ? Import::kRtlEnterCriticalSection : Import::kRtlLeaveCriticalSection,
      {stream + 32}));
}

uint32_t UnlockFhandle(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B7A58 _unlock_fhandle");
  const uint32_t entry =
      Load(base, kPioinfo + 4 * uint32_t(int32_t(handle) >> 5)) + 44 * (handle & 31);
  return uint32_t(calls.import(Import::kRtlLeaveCriticalSection, {entry + 12}));
}

void BugCheckStub(const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA1A0 the initial _aexit_rtn");
  calls.import(Import::kKeBugCheck, {0});
}

uint64_t RaiseException(uint8_t* base, uint32_t code, uint32_t flags, uint32_t count,
                        uint32_t parameters, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825BA420 RaiseException");
  uint64_t result = 0;
  calls.in_frame(176, [&](uint32_t frame) {
    const uint32_t record = frame + 80;
    Store(base, record + 0, code);
    Store(base, record + 4, flags & 1);
    Store(base, record + 8, 0);
    Store(base, record + 12, 0x825BA420);
    if (parameters == 0) {
      Store(base, record + 16, 0);
    } else {
      const uint32_t n = count > 15 ? 15 : count;
      Store(base, record + 16, n);
      if (n != 0) {
        c_runtime::CopyForward(base, record + 20, parameters, 4 * n);
      }
    }
    result = calls.import(Import::kRtlRaiseException, {record});
  });
  return result;
}

uint64_t CxxThrowException(uint8_t* base, uint32_t object, uint32_t throw_info,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD198 _CxxThrowException");
  uint64_t result = 0;
  calls.in_frame(144, [&](uint32_t frame) {
    const uint32_t record = frame + 80;
    if (!c_runtime::Memcpy(base, record, 0x8207275C, 32)) {
      calls.guest(0x825A7410, {record, 0x8207275C, 32});
    }
    Store(base, record + 24, object);
    Store(base, record + 28, throw_info);
    if (throw_info != 0 && (Load(base, throw_info) & 8) != 0) {
      Store(base, record + 20, 0x01994000);
    }
    result = RaiseException(base, Load(base, record + 0), Load(base, record + 4),
                            Load(base, record + 16), record + 20, calls);
  });
  return result;
}

uint64_t InvalidParameter(uint8_t* base, uint32_t expression, uint32_t function, uint32_t file,
                          uint32_t line, uint32_t reserved, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AFCC8 _invalid_parameter");
  uint64_t result = 2;
  calls.in_frame(96, [&](uint32_t) {
    if (const uint32_t handler = Load(base, kInvalidParameterHandler); handler != 0) {
      result = calls.guest(handler, {expression, function, file, line, reserved});
      return;
    }
    CrtDebuggerHook(base);
    calls.trap(22);
  });
  return result;
}

void ThreadStartup(uint8_t* base, uint32_t routine, uint32_t parameter, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825BA558 XapiThreadStartup");
  calls.in_frame(128, [&](uint32_t frame) {
    Store(base, frame + 80, 0);
    CallThreadNotifyRoutines(base, 1, calls);
    Store(base, frame + 80, uint32_t(calls.guest(routine, {parameter})));
    CallThreadNotifyRoutines(base, 0, calls);
    calls.import(Import::kExTerminateThread, {Load(base, frame + 80)});
  });
}

uint64_t AmsgExit(uint8_t* base, uint32_t number, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA158 _amsg_exit");
  uint64_t result = 0;
  calls.in_frame(96, [&](uint32_t) {
    FfMsgBanner(base);
    NmsgWrite(base, number);
    result = calls.guest(Load(base, kExitRoutine), {255});
  });
  return result;
}

uint32_t Getptd(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA580 _getptd");
  uint32_t data = 0;
  calls.in_frame(96, [&](uint32_t) {
    data = GetptdNoexit(base, calls);
    if (data == 0) {
      AmsgExit(base, 16, calls);
    }
  });
  return data;
}

uint64_t Fptrap(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B64F8 _fptrap");
  return AmsgExit(base, 2, calls);
}

uint32_t UnhandledExceptionFilter(uint8_t* base, uint32_t pointers, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82297940 UnhandledExceptionFilter");
  uint32_t result = 0;
  calls.in_frame(96, [&](uint32_t) {
    uint32_t monitor_result = 0;
    if (const uint32_t data = Load(base, Load(base, kDebugMonitorData)); data != 0) {
      monitor_result = uint32_t(calls.guest(Load(base, data + 24), {10, 0}));
    }
    if (monitor_result != 0) {
      return;
    }
    if (const uint32_t filter = Load(base, kUnhandledFilter); filter != 0) {
      if (int32_t(calls.guest(filter, {pointers})) == -1) {
        result = 0xFFFFFFFFu;
      }
    }
  });
  return result;
}

void DebugMonitorNotify(uint8_t* base, uint32_t code, double value, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B9BA8 a debug monitor notification");
  calls.in_frame(96, [&](uint32_t frame) {
    const uint32_t bits = std::bit_cast<uint32_t>(float(value));
    Store(base, frame + 124, bits);
    if (const uint32_t data = Load(base, Load(base, kDebugMonitorData)); data != 0) {
      calls.guest(Load(base, data + 24), {39, uint64_t(code | 0x02200000u)});
    }
    if (const uint32_t data = Load(base, Load(base, kDebugMonitorData)); data != 0) {
      calls.guest(Load(base, data + 24), {39, Load(base, frame + 124)});
    }
  });
}

uint32_t CxxRestoreUnhandledExceptionFilter(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B4D90 __CxxRestoreUnhandledExceptionFilter");
  return SetUnhandledExceptionFilter(base, Load(base, kCxxPreviousFilter));
}

void InvokeWatson(uint8_t* base, uint32_t return_address, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AFD10 _invoke_watson");
  calls.in_frame(2816, [&](uint32_t frame) {
    Store(base, frame + 2808, return_address);
    guest::ForEachHostPart(base, frame + 176, 2624,
                           [](uint8_t* host, uint32_t, uint32_t bytes) {
                             std::memset(host, 0, bytes);
                           });
    for (uint32_t offset = 96; offset < 176; offset += 4) {
      Store(base, frame + offset, 0);
    }
    Store(base, frame + 96, 0xC000000Du);
    Store(base, frame + 108, Load(base, frame + 2808));
    Store(base, frame + 80, frame + 96);
    Store(base, frame + 84, frame + 176);
    SetUnhandledExceptionFilter(base, 0);
    if (UnhandledExceptionFilter(base, frame + 80, calls) == 0) {
      CrtDebuggerHook(base);
    }
    calls.import(Import::kKeBugCheck, {30});
  });
}

namespace {

uint32_t HandleEntry(uint8_t* base, uint32_t handle) {
  return Load(base, kPioinfo + 4 * uint32_t(int32_t(handle) >> 5)) + 44 * (handle & 31);
}

}  // namespace

uint64_t LockFhandleFinally(const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B7A20 __lock_fhandle's __finally");
  uint64_t result = 0;
  calls.in_frame(96, [&](uint32_t) { result = calls.guest(kUnlock, {10}); });
  return result;
}

uint32_t LockFhandle(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B7958 __lock_fhandle");
  uint32_t result = 1;
  calls.in_frame(128, [&](uint32_t frame) {
    Store(base, frame + 148, handle);
    Store(base, frame + 80, 1);
    const uint32_t entry = HandleEntry(base, handle);
    if (Load(base, entry + 8) == 0) {
      calls.guest(kLock, {10});
      if (Load(base, entry + 8) == 0) {
        if (InitCritSecAndSpinCount(base, entry + 12, 4000, calls) == 0) {
          Store(base, frame + 80, 0);
        }
        Store(base, entry + 8, Load(base, entry + 8) + 1);
      }
      LockFhandleFinally(calls);
      handle = Load(base, frame + 148);
      result = Load(base, frame + 80);
    }
    if (result != 0) {
      calls.import(Import::kRtlEnterCriticalSection, {HandleEntry(base, handle) + 12});
    }
  });
  return result;
}

uint32_t FreeOsfhnd(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B77E8 _free_osfhnd");
  uint32_t result = 0;
  calls.in_frame(96, [&](uint32_t) {
    if (int32_t(handle) >= 0 && handle < Load(base, kNhandle)) {
      const uint32_t entry = HandleEntry(base, handle);
      if ((guest::LoadU8(base, entry + 4) & 1) != 0 && Load(base, entry + 0) != 0xFFFFFFFFu) {
        Store(base, entry + 0, 0xFFFFFFFFu);
        result = 0;
        return;
      }
    }
    Store(base, Errno(base, calls), 9);
    Store(base, DosErrno(base, calls), 0);
    result = 0xFFFFFFFFu;
  });
  return result;
}

uint32_t GetOsfhandle(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B7880 _get_osfhandle");
  uint32_t result = 0xFFFFFFFFu;
  calls.in_frame(96, [&](uint32_t) {
    if (handle == 0xFFFFFFFEu) {
      Store(base, DosErrno(base, calls), 0);
      Store(base, Errno(base, calls), 9);
      return;
    }
    if (int32_t(handle) >= 0 && handle < Load(base, kNhandle)) {
      const uint32_t entry = HandleEntry(base, handle);
      if ((guest::LoadU8(base, entry + 4) & 1) != 0) {
        result = Load(base, entry + 0);
        return;
      }
    }
    Store(base, DosErrno(base, calls), 0);
    Store(base, Errno(base, calls), 9);
    InvalidParameter(base, 0, 0, 0, 0, 0, calls);
  });
  return result;
}

uint64_t RaiseFinally(uint32_t locked, bool* called, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4C8C raise's __finally");
  uint64_t result = 0;
  *called = false;
  calls.in_frame(112, [&](uint32_t) {
    if (locked != 0) {
      result = calls.guest(kUnlock, {0});
      *called = true;
    }
  });
  return result;
}

uint64_t UnlockStreamsFinally(const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4F38 _fcloseall's and sub_825ACEB4 flsall's __finally");
  uint64_t result = 0;
  calls.in_frame(96, [&](uint32_t) { result = calls.guest(kUnlock, {1}); });
  return result;
}

uint64_t FlsallInnerFinally(uint8_t* base, uint32_t index, uint32_t piob_word,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825ACF04 flsall's inner __finally");
  uint64_t result = 0;
  calls.in_frame(112, [&](uint32_t) {
    const uint32_t stream = Load(base, Load(base, piob_word) + 4 * index);
    result = LockFile2(index, stream, false, calls);
  });
  return result;
}

namespace {

double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(uint64_t(Load(base, address)) << 32 | Load(base, address + 4));
}
void StoreDouble(uint8_t* base, uint32_t address, double value) {
  const uint64_t bits = std::bit_cast<uint64_t>(value);
  Store(base, address, uint32_t(bits >> 32));
  Store(base, address + 4, uint32_t(bits));
}

}  // namespace

double Exp(uint8_t* base, uint32_t sp, double x) {
  ITC_NATIVE_RUNNING("sub_825ABE30 exp");
  const uint32_t c = 0x820726F0;  // the constants
  double f13 = LoadDouble(base, 0x82072750);
  double f0 = x * LoadDouble(base, c + 24);
  double f12 = LoadDouble(base, c + 64);
  double f11 = LoadDouble(base, c + 88);
  // fctid (the current rounding) and fcfid.
  const int64_t rounded = std::isnan(f0) ? int64_t(0x8000000000000000ULL)
                          : f0 > double(LLONG_MAX) ? LLONG_MAX
                                                   : _mm_cvtsd_si64(_mm_load_sd(&f0));
  f0 = double(rounded);
  f13 = -std::fma(f0, f13, -x);
  // fctiwz, stored as a word.
  const int32_t exponent = std::isnan(f0) ? int32_t(0x80000000U)
                           : f0 >= double(INT_MAX) ? INT_MAX
                                                   : _mm_cvttsd_si32(_mm_load_sd(&f0));
  Store(base, sp - 32, uint32_t(exponent));
  double f10 = LoadDouble(base, c + 56);
  f0 = f13 * f13;
  f10 = std::fma(f0, f12, f10);
  f12 = LoadDouble(base, c + 80);
  f11 = std::fma(f0, f11, f12);
  f12 = LoadDouble(base, c + 48);
  f10 = std::fma(f10, f0, f12);
  f12 = LoadDouble(base, c + 72);
  f11 = std::fma(f11, f0, f12);
  f0 = f10 * f13;
  f13 = f11 - f0;
  uint32_t r10 = Load(base, sp - 32) + 1;
  f0 = f0 / f13;
  f0 = f0 + f12;
  if (r10 != 0) {
    StoreDouble(base, sp - 24, f0);
    StoreDouble(base, sp - 16, f0);
    StoreDouble(base, sp - 32, f0);
    uint32_t r9 = guest::LoadU16(base, sp - 24);
    uint32_t r8 = guest::LoadU16(base, sp - 16);
    r9 = (r9 >> 4) & 0x7FF;
    r9 = uint32_t(int32_t(int16_t(uint16_t(r9 - 1022))));
    r10 = r9 + r10 + 1022;
    r10 = (r10 << 4) & 0xFFFFFFF0u;
    r8 &= 0x800F;
    r10 |= r8;
    guest::StoreU16(base, sp - 32, uint16_t(r10));
    f0 = LoadDouble(base, sp - 32);
  }
  const double f10b = x - LoadDouble(base, c + 0);
  const double f11b = LoadDouble(base, c + 8) - x;
  f0 = f10b >= 0.0 ? LoadDouble(base, 0x8266BC30) : f0;
  return f11b >= 0.0 ? LoadDouble(base, 0x82002258) : f0;
}

namespace {

int32_t ToLower(int32_t c) { return c + c_runtime::ToLowerStep(c); }

}  // namespace

uint32_t Forcdecpt(uint8_t* base, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_825ADD38 _forcdecpt_l");
  uint32_t p = text;
  if (ToLower(int8_t(guest::LoadU8(base, p))) != 101) {
    do {
      p++;
    } while (c_runtime::CTypeC(base, guest::LoadU8(base, p), 4) != 0);
  }
  const int32_t last = ToLower(int8_t(guest::LoadU8(base, p)));
  if (last == 120) {
    p += 2;
  }
  uint8_t carried = guest::LoadU8(base, p);
  guest::StoreU8(base, p, guest::LoadU8(base, Load(base, Load(base, kDecimalPoint))));
  for (uint32_t q = p + 1;; q++) {
    const uint8_t next = guest::LoadU8(base, q);
    guest::StoreU8(base, q, carried);
    if (carried == 0) {
      break;
    }
    carried = next;
  }
  return uint32_t(last);
}

uint32_t Cropzeros(uint8_t* base, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_825ADDD8 _cropzeros_l");
  auto byte = [&](uint32_t a) { return guest::LoadU8(base, a); };
  auto point = [&]() { return byte(Load(base, Load(base, kDecimalPoint))); };
  uint32_t p = text;
  int32_t c = int8_t(byte(p));
  if (c != 0) {
    const int32_t dp = int8_t(point());
    while (c != dp) {
      p++;
      c = int8_t(byte(p));
      if (c == 0) {
        break;
      }
    }
  }
  if (byte(p) == 0) {
    return p;
  }
  uint32_t q = p + 1;
  for (int32_t d = int8_t(byte(q)); d != 0 && d != 101 && d != 69; d = int8_t(byte(q))) {
    q++;
  }
  uint32_t from = q;
  do {
    q--;
  } while (byte(q) == 48);
  if (byte(q) == point()) {
    q--;
  }
  uint8_t moved;
  do {
    moved = byte(from);
    q++;
    from++;
    guest::StoreU8(base, q, moved);
  } while (moved != 0);
  return p;
}

std::optional<uint32_t> AtoNumber(uint8_t* base, uint32_t out, uint32_t string, bool is_double) {
  if (is_double) {
    ITC_NATIVE_RUNNING("sub_825B6098 _atodbl_l");
  } else {
    ITC_NATIVE_RUNNING("sub_825B6140 _atoflt_l");
  }
  c_runtime::Strgtold12Result parsed;
  if (!c_runtime::Strgtold12(base, string, 0, 0, 0, 0, &parsed)) {
    return std::nullopt;
  }
  int32_t status = 0;
  if (is_double) {
    const uint64_t bits = c_runtime::Ld12ToDouble(parsed.value.value, parsed.value.extra, &status);
    Store(base, out + 4, uint32_t(bits));
    Store(base, out + 0, uint32_t(bits >> 32));
  } else {
    Store(base, out, c_runtime::Ld12ToFloat(parsed.value.value, parsed.value.extra, &status));
  }
  if ((parsed.flags & 3) == 0) {
    return status == 1 ? 3u : status == 2 ? 4u : 0u;
  }
  return (parsed.flags & 1) != 0 ? 4u : 3u;
}

std::optional<uint32_t> Fassign(uint8_t* base, bool is_double, uint32_t out, uint32_t string) {
  ITC_NATIVE_RUNNING("sub_825ADEB0 _fassign_l");
  return AtoNumber(base, out, string, is_double);
}

uint64_t Fileno(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0E88 _fileno");
  uint64_t result = 0;
  calls.in_frame(96, [&](uint32_t) {
    if (stream != 0) {
      result = Load(base, stream + 16);
      return;
    }
    Store(base, Errno(base, calls), 22);
    InvalidParameter(base, 0, 0, 0, 0, 0, calls);
    result = uint64_t(int64_t(-1));
  });
  return result;
}

uint64_t UnlockFhandleFinally(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0E50, sub_825B4978, sub_825B1304, sub_825B6730 _close's, _commit's, _write's, _lseeki64's __finally");
  uint64_t result = 0;
  calls.in_frame(112, [&](uint32_t) { result = UnlockFhandle(base, handle, calls); });
  return result;
}

uint64_t UnlockFileFinally(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A9664, sub_825AD02C, sub_825AD488 fclose's, fflush's, fputs's __finally");
  uint64_t result = 0;
  calls.in_frame(112, [&](uint32_t) { result = LockFile(base, stream, false, calls); });
  return result;
}

uint64_t ExitUnlockFinally(uint32_t locked, bool* called, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA338 doexit's unwind funclet");
  uint64_t result = 0;
  *called = false;
  calls.in_frame(112, [&](uint32_t) {
    if (locked != 0) {
      result = calls.guest(kUnlock, {8});
      *called = true;
    }
  });
  return result;
}

void Terminate(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3498 terminate");
  calls.in_frame(96, [&](uint32_t) {
    const uint32_t data = Getptd(base, calls);
    if (const uint32_t handler = Load(base, data + 108); handler != 0) {
      calls.guest(handler, {data});
    }
    calls.import(Import::kKeBugCheck, {0xC0000144u});
  });
}

uint32_t CloseNolock(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0C28 _close_nolock");
  uint32_t result = 0;
  calls.in_frame(112, [&](uint32_t) {
    uint32_t error = 0;
    if (GetOsfhandle(base, handle, calls) != 0xFFFFFFFFu) {
      const uint32_t block = Load(base, kPioinfo);
      const bool paired = (handle == 1 && (guest::LoadU8(base, block + 92) & 1) != 0) ||
                          (handle == 2 && (guest::LoadU8(base, block + 48) & 1) != 0);
      bool skip = false;
      if (paired) {
        const uint32_t one = GetOsfhandle(base, 1, calls);
        skip = one == GetOsfhandle(base, 2, calls);
      }
      if (!skip && CloseHandle(GetOsfhandle(base, handle, calls), calls) == 0) {
        error = calls.get_last_error();
      }
    }
    FreeOsfhnd(base, handle, calls);
    guest::StoreU8(base, HandleEntry(base, handle) + 4, 0);
    if (error != 0) {
      DosMapErr(base, error, calls);
      result = 0xFFFFFFFFu;
    }
  });
  return result;
}

namespace {

// _close's and _commit's checks before the lock: -2 (`dos_errno`: _doserrno
// 0 first, as _close), out of range or not open (with _invalid_parameter);
// true with r3 = -1 when one fails.
bool HandleChecksFail(uint8_t* base, uint32_t handle, bool dos_errno, const Calls& calls) {
  if (handle == 0xFFFFFFFEu) {
    if (dos_errno) {
      Store(base, DosErrno(base, calls), 0);
    }
    Store(base, Errno(base, calls), 9);
    return true;
  }
  if (int32_t(handle) >= 0 && handle < Load(base, kNhandle) &&
      (guest::LoadU8(base, HandleEntry(base, handle) + 4) & 1) != 0) {
    return false;
  }
  if (dos_errno) {
    Store(base, DosErrno(base, calls), 0);
  }
  Store(base, Errno(base, calls), 9);
  InvalidParameter(base, 0, 0, 0, 0, 0, calls);
  return true;
}

}  // namespace

uint64_t Close(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0D10 _close");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(144, [&](uint32_t frame) {
    Store(base, frame + 164, handle);
    if (HandleChecksFail(base, handle, true, calls)) {
      return;
    }
    LockFhandle(base, handle, calls);
    if ((guest::LoadU8(base, HandleEntry(base, handle) + 4) & 1) != 0) {
      Store(base, frame + 80, CloseNolock(base, handle, calls));
    } else {
      Store(base, Errno(base, calls), 9);
      Store(base, frame + 80, 0xFFFFFFFFu);
    }
    UnlockFhandleFinally(base, handle, calls);
    result = Load(base, frame + 80);
  });
  return result;
}

uint32_t FlushFileBuffers(uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825BA510 FlushFileBuffers");
  uint32_t result = 1;
  calls.in_frame(96, [&](uint32_t frame) {
    const uint32_t status =
        uint32_t(calls.import(Import::kNtFlushBuffersFile, {handle, frame + 80}));
    if (int32_t(status) < 0) {
      calls.set_last_error_from_status(status);
      result = 0;
    }
  });
  return result;
}

uint64_t Commit(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4828 _commit");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(144, [&](uint32_t frame) {
    Store(base, frame + 164, handle);
    if (HandleChecksFail(base, handle, false, calls)) {
      return;
    }
    LockFhandle(base, handle, calls);
    bool failed = true;
    if ((guest::LoadU8(base, HandleEntry(base, handle) + 4) & 1) != 0) {
      const uint32_t error = FlushFileBuffers(GetOsfhandle(base, handle, calls), calls) == 0
                                 ? calls.get_last_error()
                                 : 0;
      Store(base, frame + 80, error);
      if (error == 0) {
        failed = false;
      } else {
        Store(base, DosErrno(base, calls), error);
      }
    }
    if (failed) {
      Store(base, Errno(base, calls), 9);
      Store(base, frame + 80, 0xFFFFFFFFu);
    }
    UnlockFhandleFinally(base, handle, calls);
    result = Load(base, frame + 80);
  });
  return result;
}

namespace {

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

}  // namespace

uint64_t SetFilePointer(uint8_t* base, uint32_t handle, uint32_t low, uint32_t high,
                        uint32_t method, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B9DD8 SetFilePointer");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(192, [&](uint32_t frame) {
    uint64_t position;
    if (high != 0) {
      Store(base, frame + 84, low);
      Store(base, frame + 80, Load(base, high));
      position = LoadU64(base, frame + 80);
    } else {
      position = uint64_t(int64_t(int32_t(low)));
    }
    uint64_t moved;
    if (method == 0) {
      moved = position;
    } else {
      const bool end = method == 2;
      const uint32_t status = uint32_t(calls.import(
          Import::kNtQueryInformationFile,
          {handle, frame + 88, end ? frame + 96 : frame + 80, end ? 56u : 8u, end ? 34u : 14u}));
      if (int32_t(status) < 0) {
        calls.set_last_error_from_status(status);
        return;
      }
      moved = LoadU64(base, end ? frame + 136 : frame + 80) + position;
    }
    StoreU64(base, frame + 80, moved);
    if (int64_t(moved) < 0) {
      calls.set_last_error(131);
      return;
    }
    if (high == 0 && (Load(base, frame + 80) & 0x7FFFFFFFu) != 0) {
      calls.set_last_error(87);
      return;
    }
    const uint32_t status = uint32_t(
        calls.import(Import::kNtSetInformationFile, {handle, frame + 88, frame + 80, 8, 14}));
    if (int32_t(status) < 0) {
      calls.set_last_error_from_status(status);
      if (high != 0) {
        Store(base, high, 0xFFFFFFFFu);
      }
      return;
    }
    if (high != 0) {
      Store(base, high, Load(base, frame + 80));
    }
    if (Load(base, frame + 84) == 0xFFFFFFFFu) {
      calls.set_last_error(0);
    }
    result = Load(base, frame + 84);
  });
  return result;
}

uint64_t LseekNolock(uint8_t* base, uint32_t handle, uint64_t offset, uint32_t origin,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B6500 _lseeki64_nolock");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(112, [&](uint32_t frame) {
    StoreU64(base, frame + 80, offset);
    const uint32_t os = GetOsfhandle(base, handle, calls);
    if (os == 0xFFFFFFFFu) {
      Store(base, Errno(base, calls), 9);
      return;
    }
    const uint32_t low =
        uint32_t(SetFilePointer(base, os, Load(base, frame + 84), frame + 80, origin, calls));
    Store(base, frame + 84, low);
    if (low == 0xFFFFFFFFu) {
      if (const uint32_t error = calls.get_last_error(); error != 0) {
        DosMapErr(base, error, calls);
        return;
      }
    }
    const uint32_t entry = HandleEntry(base, handle);
    guest::StoreU8(base, entry + 4, guest::LoadU8(base, entry + 4) & ~2u);
    result = LoadU64(base, frame + 80);
  });
  return result;
}

uint64_t Lseek(uint8_t* base, uint32_t handle, uint64_t offset, uint32_t origin,
               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B65D0 _lseeki64");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(160, [&](uint32_t frame) {
    Store(base, frame + 180, handle);
    StoreU64(base, frame + 80, ~uint64_t(0));
    if (HandleChecksFail(base, handle, true, calls)) {
      return;
    }
    LockFhandle(base, handle, calls);
    if ((guest::LoadU8(base, HandleEntry(base, handle) + 4) & 1) != 0) {
      StoreU64(base, frame + 80, LseekNolock(base, handle, offset, origin, calls));
    } else {
      Store(base, Errno(base, calls), 9);
      Store(base, DosErrno(base, calls), 0);
      StoreU64(base, frame + 80, ~uint64_t(0));
    }
    UnlockFhandleFinally(base, handle, calls);
    result = LoadU64(base, frame + 80);
  });
  return result;
}

uint32_t WriteFile(uint8_t* base, uint32_t handle, uint32_t buffer, uint32_t size,
                   uint32_t written, uint32_t overlapped, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82295D50 WriteFile");
  uint32_t result = 0;
  calls.in_frame(128, [&](uint32_t frame) {
    if (written != 0) {
      Store(base, written, 0);
    }
    uint32_t status;
    if (overlapped != 0) {
      const uint32_t event = Load(base, overlapped + 16);
      Store(base, overlapped + 0, 259);
      Store(base, frame + 92, Load(base, overlapped + 8));
      Store(base, frame + 88, Load(base, overlapped + 12));
      const uint32_t context = (event & 1) == 0 ? overlapped : 0;
      status = uint32_t(calls.import(Import::kNtWriteFile, {handle, event, 0, context, overlapped,
                                                             buffer, size, frame + 88}));
      if ((status & 0xC0000000u) != 0xC0000000u && status != 259) {
        if (written != 0) {
          Store(base, written, Load(base, overlapped + 4));
        }
        result = 1;
        return;
      }
    } else {
      status = uint32_t(calls.import(Import::kNtWriteFile,
                                     {handle, 0, 0, 0, frame + 80, buffer, size, 0}));
      bool waited_ok = true;
      if (status == 259) {
        status = uint32_t(calls.import(Import::kNtWaitForSingleObjectEx, {handle, 1, 0, 0}));
        if (int32_t(status) >= 0) {
          status = Load(base, frame + 80);
        } else {
          waited_ok = false;
        }
      }
      if (waited_ok && int32_t(status) >= 0) {
        Store(base, written, Load(base, frame + 84));
        result = 1;
        return;
      }
      if ((status & 0xC0000000u) == 0x80000000u) {
        Store(base, written, Load(base, frame + 84));
      }
    }
    calls.set_last_error_from_status(status);
  });
  return result;
}

uint64_t WriteNolock(uint8_t* base, uint32_t handle, uint32_t buffer, uint32_t size,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0F48 _write_nolock");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(1232, [&](uint32_t frame) {
    if (size == 0) {
      result = 0;
      return;
    }
    const auto invalid = [&] {
      Store(base, DosErrno(base, calls), 0);
      Store(base, Errno(base, calls), 22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
    };
    if (buffer == 0) {
      invalid();
      return;
    }
    const uint32_t entry = HandleEntry(base, handle);
    const int32_t mode = int8_t(guest::LoadU8(base, entry + 40)) >> 1;
    if ((mode == 2 || mode == 1) && (size & 1) != 0) {
      invalid();
      return;
    }
    if ((guest::LoadU8(base, entry + 4) & 0x20) != 0) {
      LseekNolock(base, handle, 0, 2, calls);
    }
    uint64_t total = 0;
    uint32_t crs = 0, error = 0;
    const uint32_t chunk = frame + 96, count = frame + 80;
    if ((guest::LoadU8(base, HandleEntry(base, handle) + 4) & 0x80) != 0) {
      uint32_t at = buffer;
      while (true) {
        uint32_t out = chunk, n = 0;
        while (at - buffer < size) {
          const uint8_t c = guest::LoadU8(base, at);
          at++;
          if (c == 10) {
            crs++;
            n++;
            guest::StoreU8(base, out++, 13);
          }
          n++;
          guest::StoreU8(base, out++, c);
          if (n >= 1024) {
            break;
          }
        }
        const uint32_t bytes = out - chunk;
        if (WriteFile(base, Load(base, HandleEntry(base, handle)), chunk, bytes, count, 0,
                      calls) == 0) {
          error = calls.get_last_error();
          break;
        }
        const uint32_t wrote = Load(base, count);
        total += wrote;
        if (int32_t(wrote) < int32_t(bytes) || at - buffer >= size) {
          break;
        }
      }
    } else {
      if (WriteFile(base, Load(base, HandleEntry(base, handle)), buffer, size, count, 0, calls) ==
          0) {
        error = calls.get_last_error();
      } else {
        total = Load(base, count);
      }
    }
    if (uint32_t(total) != 0) {
      result = total - crs;
      return;
    }
    if (error != 0) {
      if (error == 5) {
        Store(base, Errno(base, calls), 9);
        Store(base, DosErrno(base, calls), 5);
      } else {
        DosMapErr(base, error, calls);
      }
      return;
    }
    if ((guest::LoadU8(base, HandleEntry(base, handle) + 4) & 0x40) != 0 &&
        guest::LoadU8(base, buffer) == 26) {
      result = 0;
      return;
    }
    Store(base, Errno(base, calls), 28);
    Store(base, DosErrno(base, calls), 0);
  });
  return result;
}

uint64_t Write(uint8_t* base, uint32_t handle, uint32_t buffer, uint32_t size,
               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B11A8 _write");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(160, [&](uint32_t frame) {
    Store(base, frame + 180, handle);
    if (HandleChecksFail(base, handle, true, calls)) {
      return;
    }
    LockFhandle(base, handle, calls);
    if ((guest::LoadU8(base, HandleEntry(base, handle) + 4) & 1) != 0) {
      Store(base, frame + 80, uint32_t(WriteNolock(base, handle, buffer, size, calls)));
    } else {
      Store(base, Errno(base, calls), 9);
      Store(base, DosErrno(base, calls), 0);
      Store(base, frame + 80, 0xFFFFFFFFu);
    }
    UnlockFhandleFinally(base, handle, calls);
    result = Load(base, frame + 80);
  });
  return result;
}

uint64_t Flush(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825ACC78 _flush");
  uint64_t result = 0;
  calls.in_frame(112, [&](uint32_t) {
    const uint32_t flags = Load(base, stream + 12);
    if ((flags & 3) == 2 && (flags & 264) != 0) {
      const uint32_t count = Load(base, stream + 0) - Load(base, stream + 8);
      if (int32_t(count) > 0) {
        const uint32_t handle = uint32_t(Fileno(base, stream, calls));
        const uint32_t wrote =
            uint32_t(Write(base, handle, Load(base, stream + 8), count, calls));
        const uint32_t now = Load(base, stream + 12);
        if (wrote == count) {
          if ((now & 0x80) != 0) {
            Store(base, stream + 12, now & ~2u);
          }
        } else {
          result = uint64_t(int64_t(-1));
          Store(base, stream + 12, now | 32);
        }
      }
    }
    Store(base, stream + 0, Load(base, stream + 8));
    Store(base, stream + 4, 0);
  });
  return result;
}

uint64_t CxxUnhandledExceptionFilter(uint8_t* base, uint32_t pointers, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4CD0 __CxxUnhandledExceptionFilter");
  uint64_t result = 0;
  calls.in_frame(112, [&](uint32_t) {
    const uint32_t record = Load(base, pointers + 0);
    if (Load(base, record + 0) == 0xE06D7363u && Load(base, record + 16) == 3) {
      const uint32_t magic = Load(base, record + 20);
      if (magic == 0x19930520u || magic == 0x19930521u || magic == 0x19930522u) {
        Terminate(base, calls);
      }
    }
    if (Load(base, kCxxPreviousFilter) != 0 &&
        ValidateExecute(Load(base, kCxxPreviousFilter)) != 0) {
      result = calls.guest(Load(base, kCxxPreviousFilter), {pointers});
    }
  });
  return result;
}


uint64_t FcloseNolock(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A94C0 _fclose_nolock");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(112, [&](uint32_t) {
    if (stream == 0) {
      Store(base, Errno(base, calls), 22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      return;
    }
    if ((Load(base, stream + 12) & 131) != 0) {
      result = Flush(base, stream, calls);
      Freebuf(base, stream, calls);
      const uint64_t closed = Close(base, uint32_t(Fileno(base, stream, calls)), calls);
      if (int32_t(uint32_t(closed)) < 0) {
        result = uint64_t(int64_t(-1));
      } else if (const uint32_t name = Load(base, stream + 28); name != 0) {
        calls.free(name);
        Store(base, stream + 28, 0);
      }
    }
    Store(base, stream + 12, 0);
  });
  return result;
}

uint64_t Fclose(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A9580 fclose");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(112, [&](uint32_t frame) {
    Store(base, frame + 132, stream);
    Store(base, frame + 80, 0xFFFFFFFFu);
    if (stream == 0) {
      Store(base, Errno(base, calls), 22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      return;
    }
    if ((Load(base, stream + 12) & 0x40) != 0) {
      Store(base, stream + 12, 0);
    } else {
      LockFile(base, stream, true, calls);
      Store(base, frame + 80, uint32_t(FcloseNolock(base, stream, calls)));
      UnlockFileFinally(base, stream, calls);
    }
    result = Load(base, frame + 80);
  });
  return result;
}

uint32_t Fcloseall(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4E78 _fcloseall");
  uint32_t result = 0;
  calls.in_frame(144, [&](uint32_t frame) {
    uint32_t count = 0;
    Store(base, frame + 80, 0);
    calls.guest(kLock, {1});
    for (int32_t i = 3;; i++) {
      Store(base, frame + 84, uint32_t(i));
      if (i >= int32_t(Load(base, kNstream))) {
        break;
      }
      const uint32_t stream = Load(base, Load(base, kPiob) + 4 * uint32_t(i));
      if (stream == 0) {
        continue;
      }
      if ((Load(base, stream + 12) & 131) != 0 &&
          int32_t(uint32_t(Fclose(base, stream, calls))) != -1) {
        count++;
        Store(base, frame + 80, count);
      }
      if (i >= 20) {
        calls.free(Load(base, Load(base, kPiob) + 4 * uint32_t(i)));
        Store(base, Load(base, kPiob) + 4 * uint32_t(i), 0);
      }
    }
    UnlockStreamsFinally(calls);
    result = Load(base, frame + 80);
  });
  return result;
}


namespace {

constexpr uint32_t kBadIoInfo = 0x8266BFB8;  // __badioinfo
constexpr uint32_t kCflush = 0x82971174;     // _cflush
constexpr uint32_t kStdBuffers = 0x82971330;  // _stbuf's two shared buffers

// _isatty, compiled to the shared "returns 0" (sub_825C8388).
uint32_t Isatty(uint32_t) { return vehicle_accessors::DebugSwitchOff(); }

}  // namespace

uint64_t Flsbuf(uint8_t* base, uint32_t character, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AEE28 _flsbuf");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(128, [&](uint32_t frame) {
    Store(base, frame + 148, character);
    const uint32_t handle = uint32_t(Fileno(base, stream, calls));
    const uint32_t flags = Load(base, stream + 12);
    const auto fail = [&](uint32_t error) {
      if (error != 0) {
        Store(base, Errno(base, calls), error);
      }
      Store(base, stream + 12, Load(base, stream + 12) | 32);
    };
    if ((flags & 130) == 0) {
      fail(9);
      return;
    }
    if ((flags & 0x40) != 0) {
      fail(34);
      return;
    }
    if ((flags & 1) != 0) {
      Store(base, stream + 4, 0);
      if ((flags & 0x10) == 0) {
        Store(base, stream + 12, flags | 32);
        return;
      }
      Store(base, stream + 0, Load(base, stream + 8));
      Store(base, stream + 12, flags & ~1u);
    }
    uint32_t written = 0, count = 0;
    Store(base, stream + 4, 0);
    const uint32_t now = (Load(base, stream + 12) & ~0x10u) | 2;
    Store(base, stream + 12, now);
    if ((now & 268) == 0) {
      const bool standard = stream == c_runtime::kStreams + 32 || stream == c_runtime::kStreams + 64;
      if (!standard || Isatty(handle) == 0) {
        Getbuf(base, stream, calls);
      }
    }
    if ((Load(base, stream + 12) & 264) != 0) {
      const uint32_t buffer = Load(base, stream + 8);
      count = Load(base, stream + 0) - buffer;
      Store(base, stream + 4, Load(base, stream + 24) - 1);
      Store(base, stream + 0, buffer + 1);
      if (int32_t(count) > 0) {
        written = uint32_t(Write(base, handle, buffer, count, calls));
      } else {
        const uint32_t entry =
            handle == 0xFFFFFFFFu || handle == 0xFFFFFFFEu
                ? kBadIoInfo
                : Load(base, kPioinfo + 4 * uint32_t(int32_t(handle) >> 5)) + 44 * (handle & 31);
        if ((guest::LoadU8(base, entry + 4) & 0x20) != 0 &&
            int64_t(Lseek(base, handle, 0, 2, calls)) == -1) {
          fail(0);
          return;
        }
      }
      guest::StoreU8(base, Load(base, stream + 8), uint8_t(Load(base, frame + 148)));
    } else {
      count = 1;
      written = uint32_t(Write(base, handle, frame + 148, 1, calls));
    }
    if (int32_t(written) != int32_t(count)) {
      fail(0);
      return;
    }
    result = Load(base, frame + 148) & 0xFF;
  });
  return result;
}

uint32_t Stbuf(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B40B8 _stbuf");
  uint32_t result = 0;
  calls.in_frame(112, [&](uint32_t) {
    if (Isatty(uint32_t(Fileno(base, stream, calls))) == 0) {
      return;
    }
    uint32_t index;
    if (stream == c_runtime::kStreams + 32) {
      index = 0;
    } else if (stream == c_runtime::kStreams + 64) {
      index = 1;
    } else {
      return;
    }
    Store(base, kCflush, Load(base, kCflush) + 1);
    if ((Load(base, stream + 12) & 268) != 0) {
      return;
    }
    const uint32_t slot = kStdBuffers + 4 * index;
    uint32_t buffer = Load(base, slot);
    if (buffer == 0) {
      buffer = calls.malloc(4096);
      Store(base, slot, buffer);
    }
    if (buffer == 0) {
      Store(base, stream + 8, stream + 20);
      Store(base, stream + 24, 2);
      Store(base, stream + 4, 2);
      Store(base, stream + 0, stream + 20);
    } else {
      Store(base, stream + 8, buffer);
      Store(base, stream + 0, buffer);
      Store(base, stream + 24, 4096);
      Store(base, stream + 4, 4096);
    }
    Store(base, stream + 12, Load(base, stream + 12) | 4354);
    result = 1;
  });
  return result;
}

uint64_t Ftbuf(uint8_t* base, uint32_t flag, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B41A0 _ftbuf");
  uint64_t result = flag;
  calls.in_frame(96, [&](uint32_t) {
    if (flag == 0 || (Load(base, stream + 12) & 0x1000) == 0) {
      return;
    }
    result = Flush(base, stream, calls);
    const uint32_t flags = Load(base, stream + 12) & ~0x100u & ~0x1000u;
    Store(base, stream + 24, 0);
    Store(base, stream + 0, 0);
    Store(base, stream + 8, 0);
    Store(base, stream + 12, flags);
  });
  return result;
}

}  // namespace c_runtime::core
