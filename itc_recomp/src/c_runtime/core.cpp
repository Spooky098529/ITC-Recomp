// The C runtime's core (core.h, docs/functions/c-runtime-core.md): step for
// step as the recompiled functions, the kernel imports guest calls.

#include "c_runtime/core.h"

#include "base/log.h"
#include "guest.h"
#include "c_runtime/start_up.h"
#include "c_runtime/strings.h"
#include "helpers/debug_output.h"

#include <bit>

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

}  // namespace c_runtime::core
