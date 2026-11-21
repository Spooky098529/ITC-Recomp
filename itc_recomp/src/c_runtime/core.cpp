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
#include "schema/c_runtime.h"
#include "schema/c_runtime_core.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <climits>
#include <cmath>
#include <emmintrin.h>
#include <cstring>
#include <vector>

namespace c_runtime::core {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

using G = schema::CRuntimeCoreGlobals;
using schema::CrtIoInfo;
using schema::CrtLockEntry;
using schema::Stream;
using schema::ThreadData;
using schema::ThreadNotifyBlock;

// Lock table entry `i`.
CrtLockEntry LockEntry(uint8_t* base, uint32_t i) {
  return CrtLockEntry(base, G::lock_table_address() + CrtLockEntry::kSize * i);
}

// `handle`'s entry: its block's pointer read from __pioinfo, then the entry.
CrtIoInfo IoInfo(uint8_t* base, uint32_t handle) {
  return CrtIoInfo(base, Load(base, G::io_info_blocks_address() + 4 * uint32_t(int32_t(handle) >> 5)) +
                             CrtIoInfo::kSize * (handle & 31));
}

}  // namespace

uint32_t CrtTlsAlloc(uint32_t callback, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA3F8 __crtTlsAlloc");
  return uint32_t(calls.import(Import::kKeTlsAlloc, {callback}));
}

uint32_t SetFlsGetValue(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA400 __set_flsgetvalue");
  const uint32_t value =
      uint32_t(calls.import(Import::kKeTlsGetValue, {G::getvalue_index(base)}));
  if (value != 0) {
    return value;
  }
  return uint32_t(calls.import(Import::kKeTlsSetValue,
                               {G::getvalue_index(base), G::fls_get_value(base)}));
}

uint32_t GetptdNoexit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA4C0 _getptd_noexit");
  const uint32_t saved = calls.get_last_error();
  SetFlsGetValue(base, calls);
  const uint32_t getter =
      uint32_t(calls.import(Import::kKeTlsGetValue, {G::getvalue_index(base)}));
  uint32_t data = uint32_t(calls.guest(getter, {G::fls_index(base)}));
  if (data == 0) {
    data = calls.calloc(1, ThreadData::kSize);
    if (data != 0) {
      const uint32_t stored = uint32_t(
          calls.guest(G::fls_set_value(base), {G::fls_index(base), data}));
      if (stored != 0) {
        const ThreadData td(base, data);
        td.set_rand_seed(1);
        td.set_exception_actions(G::exception_actions_address());
        td.set_thread_id(calls.thread_id());
        td.set_thread_handle(0xFFFFFFFFu);
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
  // The buffers freed with it (not yet ThreadData's fields: c-runtime.toml).
  for (uint32_t offset : {36u, 44u, 52u, 60u, 68u, 72u}) {
    if (const uint32_t buffer = Load(base, data + offset); buffer != 0) {
      calls.free(buffer);
    }
  }
  if (const uint32_t table = ThreadData(base, data).exception_actions();
      table != G::exception_actions_address()) {
    calls.free(table);
  }
  calls.free(data);
}

void MtDeleteLocks(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3260 _mtdeletelocks");
  for (uint32_t i = 0; i < kLockCount; i++) {
    const CrtLockEntry entry = LockEntry(base, i);
    const uint32_t section = entry.section();
    if (section != 0 && entry.made_at_boot() != 1) {
      calls.free(section);
      entry.set_section(0);
    }
  }
}

void MtTerm(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA448 _mtterm");
  if (const uint32_t index = G::fls_index(base); index != 0xFFFFFFFFu) {
    calls.guest(G::fls_free(base), {index});
    G::set_fls_index(base, 0xFFFFFFFFu);
  }
  if (const uint32_t index = G::getvalue_index(base); index != 0xFFFFFFFFu) {
    calls.import(Import::kKeTlsFree, {index});
    G::set_getvalue_index(base, 0xFFFFFFFFu);
  }
  MtDeleteLocks(base, calls);
}

uint32_t Errno(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A81E0 _errno");
  const uint32_t data = GetptdNoexit(base, calls);
  return data != 0 ? ThreadData(base, data).errno_value_address() : G::errno_fallback_address();
}

uint32_t DosErrno(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A8218 __doserrno");
  const uint32_t data = GetptdNoexit(base, calls);
  return data != 0 ? ThreadData(base, data).doserrno_address() : G::doserrno_fallback_address();
}

uint32_t GetErrnoFromOsErr(uint8_t* base, uint32_t error) {
  ITC_NATIVE_RUNNING("sub_825A8178 _get_errno_from_oserr");
  for (uint32_t i = 0; i < 45; i++) {
    if (Load(base, G::os_error_table_address() + 8 * i) == error) {
      return Load(base, G::os_error_table_address() + 8 * i + 4);
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
  schema::DosErrnoSlot(base, data != 0 ? ThreadData(base, data).doserrno_address()
                                       : G::doserrno_fallback_address())
      .set_value(error);
  const uint32_t number = GetErrnoFromOsErr(base, error);
  const uint32_t again = GetptdNoexit(base, calls);
  schema::ErrnoSlot(base, again != 0 ? ThreadData(base, again).errno_value_address()
                                     : G::errno_fallback_address())
      .set_value(int32_t(number));
}

void FreePtd(uint8_t* base, uint32_t data, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA670 _freeptd");
  if (G::fls_index(base) != 0xFFFFFFFFu) {
    if (data == 0 &&
        calls.import(Import::kKeTlsGetValue, {G::getvalue_index(base)}) != 0) {
      const uint32_t getter =
          uint32_t(calls.import(Import::kKeTlsGetValue, {G::getvalue_index(base)}));
      data = uint32_t(calls.guest(getter, {G::fls_index(base)}));
    }
    calls.guest(G::fls_set_value(base), {G::fls_index(base), 0});
    FreeFls(base, data, calls);
  }
  if (const uint32_t index = G::getvalue_index(base); index != 0xFFFFFFFFu) {
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
  uint32_t init = schema::CRuntimeGlobals::critical_section_initializer(base);
  if (init == 0) {
    init = kCritSecNoSpinCount;
    schema::CRuntimeGlobals::set_critical_section_initializer(base, init);
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
  calls.import(Import::kRtlEnterCriticalSection, {G::notify_lock_address()});
  // The list's head is a block of links only.
  const ThreadNotifyBlock head(base, G::notify_list_address());
  const ThreadNotifyBlock node(base, block);
  if (add != 0) {
    const uint32_t last = head.previous();
    node.set_next(head.address);
    node.set_previous(last);
    ThreadNotifyBlock(base, last).set_next(block);
    head.set_previous(block);
  } else {
    const uint32_t next = node.next();
    const uint32_t previous = node.previous();
    ThreadNotifyBlock(base, previous).set_next(next);
    ThreadNotifyBlock(base, next).set_previous(previous);
  }
  calls.import(Import::kRtlLeaveCriticalSection, {G::notify_lock_address()});
}

void CallThreadNotifyRoutines(uint8_t* base, uint32_t starting, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82297B38 XapiCallThreadNotifyRoutines");
  calls.import(Import::kRtlEnterCriticalSection, {G::notify_lock_address()});
  const uint32_t head = G::notify_list_address();
  for (uint32_t block = ThreadNotifyBlock(base, head).next(); block != head;) {
    const ThreadNotifyBlock node(base, block);
    const uint32_t routine = node.routine();
    block = node.next();
    calls.guest(routine, {starting});
  }
  calls.import(Import::kRtlLeaveCriticalSection, {G::notify_lock_address()});
}

uint32_t MtInitLocks(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B31D8 _mtinitlocks");
  uint32_t section = G::lock_sections_address();
  for (uint32_t i = 0; i < kLockCount; i++) {
    const CrtLockEntry entry = LockEntry(base, i);
    if (entry.made_at_boot() != 1) {
      continue;
    }
    entry.set_section(section);
    const uint32_t made = InitCritSecAndSpinCount(base, section, 4000, calls);
    section += 28;
    if (made == 0) {
      entry.set_section(0);
      return 0;
    }
  }
  return 1;
}

void ThreadNotify(uint8_t* base, uint32_t starting, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3650 the C runtime's thread notify routine");
  if (starting != 0) {
    if (const uint32_t hook = G::thread_start_hook(base); hook != 0) {
      calls.guest(hook, {starting});
    }
    return;
  }
  if (const uint32_t hook = G::thread_end_hook(base); hook != 0) {
    calls.guest(hook, {0});
  }
  FreePtd(base, 0, calls);
}

uint32_t MtInit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA708 _mtinit");
  G::set_fls_alloc(base, 0x825AA3F8);      // __crtTlsAlloc
  G::set_fls_get_value(base, 0x825DBBF4);  // KeTlsGetValue's thunk
  G::set_fls_set_value(base, 0x825DBC04);  // KeTlsSetValue's
  G::set_fls_free(base, 0x825DBC24);       // KeTlsFree's
  const uint32_t getter_index = uint32_t(calls.import(Import::kKeTlsAlloc, {}));
  G::set_getvalue_index(base, getter_index);
  if (getter_index == 0xFFFFFFFFu ||
      calls.import(Import::kKeTlsSetValue, {getter_index, G::fls_get_value(base)}) == 0) {
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
  const uint32_t fls_index = uint32_t(calls.guest(G::fls_alloc(base), {0x825AA5C0}));
  G::set_fls_index(base, fls_index);
  if (fls_index == 0xFFFFFFFFu) {
    return fail();
  }
  const uint32_t data = calls.calloc(1, ThreadData::kSize);
  if (data == 0) {
    return fail();
  }
  if (calls.guest(G::fls_set_value(base), {G::fls_index(base), data}) == 0) {
    return fail();
  }
  const ThreadData td(base, data);
  td.set_rand_seed(1);
  td.set_exception_actions(G::exception_actions_address());
  const uint32_t id = calls.thread_id();
  td.set_thread_id(id);
  td.set_thread_handle(0xFFFFFFFFu);
  ThreadNotifyBlock(base, G::crt_notify_block_address()).set_routine(kCrtNotifyRoutine);
  RegisterThreadNotifyRoutine(base, G::crt_notify_block_address(), 1, calls);
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
    if (const uint32_t function = schema::CrtInitTable(base, p).function(); function != 0) {
      calls.guest(function, {});
    }
  }
}

uint32_t SetAbortBehavior(uint8_t* base, uint32_t flags, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_825AD120 _set_abort_behavior");
  const uint32_t old = G::abort_behavior(base);
  G::set_abort_behavior(base, (old & ~mask) | (flags & mask));
  return old;
}

uint32_t GetSigabrt(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B49B0 __get_sigabrt");
  return G::sigabrt_action(base);
}

void CrtDebuggerHook(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B68B8 _crt_debugger_hook");
  G::set_debugger_hook_dummy(base, 0);
}

uint32_t ValidateExecute(uint32_t pointer) {
  ITC_NATIVE_RUNNING("sub_825B8910 _ValidateExecute");
  return pointer != 0 ? 1 : 0;
}

uint32_t SetUnhandledExceptionFilter(uint8_t* base, uint32_t filter) {
  ITC_NATIVE_RUNNING("sub_82297870 SetUnhandledExceptionFilter");
  const uint32_t old = G::unhandled_exception_filter(base);
  G::set_unhandled_exception_filter(base, filter);
  return old;
}

uint32_t GetCommandLineA(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822979D0 GetCommandLineA");
  return G::loaded_command_line(base);
}

uint32_t Positive(uint8_t* base, uint32_t number) {
  ITC_NATIVE_RUNNING("sub_825ADE90 _positive");
  auto load = [&](uint32_t address) {
    return std::bit_cast<double>(uint64_t(Load(base, address)) << 32 | Load(base, address + 4));
  };
  const double x = load(number);
  const double zero = load(G::zero_double_address());
  return x < zero ? 0 : 1;
}

void NmsgWrite(uint8_t* base, uint32_t number) {
  ITC_NATIVE_RUNNING("sub_825AEDA0 _NMSG_WRITE");
  for (uint32_t i = 0; i < 23; i++) {
    if (Load(base, G::run_time_errors_address() + 8 * i) == number) {
      helpers::DebugOutput(nullptr);
      return;
    }
  }
}

void Getbuf(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4790 _getbuf");
  G::set_cflush(base, G::cflush(base) + 1);
  const uint32_t buffer = calls.malloc(4096);
  const Stream s(base, stream);
  s.set_buffer(buffer);
  if (buffer != 0) {
    Store(base, stream + 24, 4096);  // _bufsiz (not yet Stream's: c-runtime.toml)
    s.set_flags(s.flags() | 8);
  } else {
    s.set_buffer(stream + 20);       // _charbuf
    Store(base, stream + 24, 2);
    s.set_flags(s.flags() | 4);
  }
  s.set_room(0);
  s.set_pointer(s.buffer());
}

void Freebuf(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B0EE0 _freebuf");
  const Stream s(base, stream);
  const uint32_t flags = s.flags();
  if ((flags & 0x83) == 0 || (flags & 8) == 0) {
    return;
  }
  calls.free(s.buffer());
  const uint32_t after = s.flags() & ~0x8u & ~0x400u;
  s.set_pointer(0);
  s.set_buffer(0);
  s.set_room(0);
  s.set_flags(after);
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
  const uint32_t begin = G::xapi_initializers_address();
  for (uint32_t p = begin; p < begin + 12 && uint32_t(result) == 0; p += 4) {
    if (const uint32_t function = schema::CrtInitTable(base, p).function(); function != 0) {
      result = calls.guest(function, {0});
    }
  }
  return result;
}

uint32_t IoInit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4668 _ioinit");
  const uint32_t block = calls.calloc(32, CrtIoInfo::kSize);
  if (block == 0) {
    return 0xFFFFFFFFu;
  }
  G::set_handle_count(base, 32);
  Store(base, G::io_info_blocks_address(), block);
  for (uint32_t at = block; at < block + 32 * CrtIoInfo::kSize; at += CrtIoInfo::kSize) {
    const CrtIoInfo entry(base, at);
    entry.set_osfile(0);
    entry.set_pipe_char(10);
    entry.set_lock_made(0);
    guest::StoreU8(base, entry.pipe_char2_address() + 0, 10);
    guest::StoreU8(base, entry.pipe_char2_address() + 1, 10);
    entry.set_text_mode(0);
    entry.set_os_handle(0xFFFFFFFFu);
  }
  for (uint32_t i = 0; i < 3; i++) {
    const CrtIoInfo entry(base, Load(base, G::io_info_blocks_address()) + CrtIoInfo::kSize * i);
    entry.set_osfile(0xC1);
    entry.set_os_handle(0xFFFFFFFEu);
  }
  return 0;
}

uint32_t InitStdio(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD4D0 _initstdio");
  int32_t count = G::stream_count(base);
  if (count == 0) {
    count = 512;
    G::set_stream_count(base, count);
  } else if (count < 20) {
    count = 20;
    G::set_stream_count(base, count);
  }
  uint32_t piob = calls.calloc(uint32_t(count), 4);
  G::set_stream_table(base, piob);
  if (piob == 0) {
    G::set_stream_count(base, 20);
    piob = calls.calloc(20, 4);
    G::set_stream_table(base, piob);
    if (piob == 0) {
      return 26;
    }
  }
  for (uint32_t i = 0; i < 20; i++) {
    Store(base, G::stream_table(base) + 4 * i, kIob + Stream::kSize * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t handle = IoInfo(base, i).os_handle();
    if (handle == 0xFFFFFFFFu || handle == 0xFFFFFFFEu || handle == 0) {
      Store(base, kIob + Stream::kSize * i + 16, 0xFFFFFFFEu);  // _file
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
  return uint32_t(
      calls.import(Import::kRtlLeaveCriticalSection, {IoInfo(base, handle).lock_address()}));
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
    const schema::ExceptionRecord record(base, frame + 80);
    record.set_code(code);
    record.set_flags(flags & 1);
    record.set_record(0);
    record.set_exception_address(0x825BA420);
    if (parameters == 0) {
      record.set_parameter_count(0);
    } else {
      const uint32_t n = count > 15 ? 15 : count;
      record.set_parameter_count(n);
      if (n != 0) {
        c_runtime::CopyForward(base, record.magic_address(), parameters, 4 * n);
      }
    }
    result = calls.import(Import::kRtlRaiseException, {record.address});
  });
  return result;
}

uint64_t CxxThrowException(uint8_t* base, uint32_t object, uint32_t throw_info,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD198 _CxxThrowException");
  uint64_t result = 0;
  calls.in_frame(144, [&](uint32_t frame) {
    const schema::ExceptionRecord record(base, frame + 80);
    if (!c_runtime::Memcpy(base, record.address, 0x8207275C, 32)) {
      calls.guest(0x825A7410, {record.address, 0x8207275C, 32});
    }
    record.set_object(object);
    record.set_throw_info(throw_info);
    if (throw_info != 0 && (schema::CxxThrowInfo(base, throw_info).attributes() & 8) != 0) {
      record.set_magic(0x01994000);
    }
    result = RaiseException(base, record.code(), record.flags(), record.parameter_count(),
                            record.magic_address(), calls);
  });
  return result;
}

uint64_t InvalidParameter(uint8_t* base, uint32_t expression, uint32_t function, uint32_t file,
                          uint32_t line, uint32_t reserved, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AFCC8 _invalid_parameter");
  uint64_t result = 2;
  calls.in_frame(96, [&](uint32_t) {
    if (const uint32_t handler = schema::CRuntimeGlobals::invalid_parameter_handler(base);
        handler != 0) {
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
    result = calls.guest(schema::CRuntimeGlobals::aexit_rtn(base), {255});
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
    if (const schema::DebugMonitorData data(base, Load(base, G::debug_monitor_data(base))); data) {
      monitor_result = uint32_t(calls.guest(data.notify(), {10, 0}));
    }
    if (monitor_result != 0) {
      return;
    }
    if (const uint32_t filter = G::unhandled_exception_filter(base); filter != 0) {
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
    if (const schema::DebugMonitorData data(base, Load(base, G::debug_monitor_data(base))); data) {
      calls.guest(data.notify(), {39, uint64_t(code | 0x02200000u)});
    }
    if (const schema::DebugMonitorData data(base, Load(base, G::debug_monitor_data(base))); data) {
      calls.guest(data.notify(), {39, Load(base, frame + 124)});
    }
  });
}

uint32_t CxxRestoreUnhandledExceptionFilter(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825B4D90 __CxxRestoreUnhandledExceptionFilter");
  return SetUnhandledExceptionFilter(base, G::cxx_previous_filter(base));
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
    const CrtIoInfo entry = IoInfo(base, handle);
    if (entry.lock_made() == 0) {
      calls.guest(kLock, {10});
      if (entry.lock_made() == 0) {
        if (InitCritSecAndSpinCount(base, entry.lock_address(), 4000, calls) == 0) {
          Store(base, frame + 80, 0);
        }
        entry.set_lock_made(entry.lock_made() + 1);
      }
      LockFhandleFinally(calls);
      handle = Load(base, frame + 148);
      result = Load(base, frame + 80);
    }
    if (result != 0) {
      calls.import(Import::kRtlEnterCriticalSection, {IoInfo(base, handle).lock_address()});
    }
  });
  return result;
}

uint32_t FreeOsfhnd(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B77E8 _free_osfhnd");
  uint32_t result = 0;
  calls.in_frame(96, [&](uint32_t) {
    if (int32_t(handle) >= 0 && handle < G::handle_count(base)) {
      const CrtIoInfo entry = IoInfo(base, handle);
      if ((entry.osfile() & 1) != 0 && entry.os_handle() != 0xFFFFFFFFu) {
        entry.set_os_handle(0xFFFFFFFFu);
        result = 0;
        return;
      }
    }
    schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
    schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
    result = 0xFFFFFFFFu;
  });
  return result;
}

uint32_t GetOsfhandle(uint8_t* base, uint32_t handle, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B7880 _get_osfhandle");
  uint32_t result = 0xFFFFFFFFu;
  calls.in_frame(96, [&](uint32_t) {
    if (handle == 0xFFFFFFFEu) {
      schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
      return;
    }
    if (int32_t(handle) >= 0 && handle < G::handle_count(base)) {
      const CrtIoInfo entry = IoInfo(base, handle);
      if ((entry.osfile() & 1) != 0) {
        result = entry.os_handle();
        return;
      }
    }
    schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
    schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
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
  f0 = f10b >= 0.0 ? LoadDouble(base, G::positive_infinity_address()) : f0;
  return f11b >= 0.0 ? LoadDouble(base, G::zero_double_address()) : f0;
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
      result = Load(base, stream + 16);  // _file (not yet Stream's: c-runtime.toml)
      return;
    }
    schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
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
    if (const uint32_t handler = ThreadData(base, data).terminate_handler(); handler != 0) {
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
      const uint32_t block = Load(base, G::io_info_blocks_address());
      // The other console handle's entry (2's for 1, 1's for 2).
      const bool paired =
          (handle == 1 && (CrtIoInfo(base, block + 2 * CrtIoInfo::kSize).osfile() & 1) != 0) ||
          (handle == 2 && (CrtIoInfo(base, block + 1 * CrtIoInfo::kSize).osfile() & 1) != 0);
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
    IoInfo(base, handle).set_osfile(0);
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
      schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
    }
    schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
    return true;
  }
  if (int32_t(handle) >= 0 && handle < G::handle_count(base) &&
      (IoInfo(base, handle).osfile() & 1) != 0) {
    return false;
  }
  if (dos_errno) {
    schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
  }
  schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
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
    if ((IoInfo(base, handle).osfile() & 1) != 0) {
      Store(base, frame + 80, CloseNolock(base, handle, calls));
    } else {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
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
    if ((IoInfo(base, handle).osfile() & 1) != 0) {
      const uint32_t error = FlushFileBuffers(GetOsfhandle(base, handle, calls), calls) == 0
                                 ? calls.get_last_error()
                                 : 0;
      Store(base, frame + 80, error);
      if (error == 0) {
        failed = false;
      } else {
        schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(error);
      }
    }
    if (failed) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
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
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
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
    const CrtIoInfo entry = IoInfo(base, handle);
    entry.set_osfile(entry.osfile() & ~2u);
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
    if ((IoInfo(base, handle).osfile() & 1) != 0) {
      StoreU64(base, frame + 80, LseekNolock(base, handle, offset, origin, calls));
    } else {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
      schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
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
      const schema::CrtOverlapped o(base, overlapped);
      const uint32_t event = o.event();
      o.set_internal(259);
      Store(base, frame + 92, o.offset());
      Store(base, frame + 88, o.offset_high());
      const uint32_t context = (event & 1) == 0 ? overlapped : 0;
      status = uint32_t(calls.import(Import::kNtWriteFile, {handle, event, 0, context, overlapped,
                                                             buffer, size, frame + 88}));
      if ((status & 0xC0000000u) != 0xC0000000u && status != 259) {
        if (written != 0) {
          Store(base, written, schema::CrtOverlapped(base, overlapped).internal_high());
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
      schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
    };
    if (buffer == 0) {
      invalid();
      return;
    }
    const CrtIoInfo entry = IoInfo(base, handle);
    const int32_t mode = int8_t(entry.text_mode()) >> 1;
    if ((mode == 2 || mode == 1) && (size & 1) != 0) {
      invalid();
      return;
    }
    if ((entry.osfile() & 0x20) != 0) {
      LseekNolock(base, handle, 0, 2, calls);
    }
    uint64_t total = 0;
    uint32_t crs = 0, error = 0;
    const uint32_t chunk = frame + 96, count = frame + 80;
    if ((IoInfo(base, handle).osfile() & 0x80) != 0) {
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
        if (WriteFile(base, IoInfo(base, handle).os_handle(), chunk, bytes, count, 0,
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
      if (WriteFile(base, IoInfo(base, handle).os_handle(), buffer, size, count, 0, calls) ==
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
        schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
        schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(5);
      } else {
        DosMapErr(base, error, calls);
      }
      return;
    }
    if ((IoInfo(base, handle).osfile() & 0x40) != 0 &&
        guest::LoadU8(base, buffer) == 26) {
      result = 0;
      return;
    }
    schema::ErrnoSlot(base, Errno(base, calls)).set_value(28);
    schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
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
    if ((IoInfo(base, handle).osfile() & 1) != 0) {
      Store(base, frame + 80, uint32_t(WriteNolock(base, handle, buffer, size, calls)));
    } else {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(9);
      schema::DosErrnoSlot(base, DosErrno(base, calls)).set_value(0);
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
    const uint32_t flags = Stream(base, stream).flags();
    if ((flags & 3) == 2 && (flags & 264) != 0) {
      const uint32_t count = Stream(base, stream).pointer() - Stream(base, stream).buffer();
      if (int32_t(count) > 0) {
        const uint32_t handle = uint32_t(Fileno(base, stream, calls));
        const uint32_t wrote =
            uint32_t(Write(base, handle, Stream(base, stream).buffer(), count, calls));
        const uint32_t now = Stream(base, stream).flags();
        if (wrote == count) {
          if ((now & 0x80) != 0) {
            Stream(base, stream).set_flags(now & ~2u);
          }
        } else {
          result = uint64_t(int64_t(-1));
          Stream(base, stream).set_flags(now | 32);
        }
      }
    }
    Stream(base, stream).set_pointer(Stream(base, stream).buffer());
    Stream(base, stream).set_room(0);
  });
  return result;
}

uint64_t CxxUnhandledExceptionFilter(uint8_t* base, uint32_t pointers, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B4CD0 __CxxUnhandledExceptionFilter");
  uint64_t result = 0;
  calls.in_frame(112, [&](uint32_t) {
    const schema::ExceptionRecord record =
        schema::ExceptionPointers(base, pointers).exception_record();
    if (record.code() == 0xE06D7363u && record.parameter_count() == 3) {
      const uint32_t magic = record.magic();
      if (magic == 0x19930520u || magic == 0x19930521u || magic == 0x19930522u) {
        Terminate(base, calls);
      }
    }
    if (G::cxx_previous_filter(base) != 0 &&
        ValidateExecute(G::cxx_previous_filter(base)) != 0) {
      result = calls.guest(G::cxx_previous_filter(base), {pointers});
    }
  });
  return result;
}


uint64_t FcloseNolock(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A94C0 _fclose_nolock");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(112, [&](uint32_t) {
    if (stream == 0) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      return;
    }
    if ((Stream(base, stream).flags() & 131) != 0) {
      result = Flush(base, stream, calls);
      Freebuf(base, stream, calls);
      const uint64_t closed = Close(base, uint32_t(Fileno(base, stream, calls)), calls);
      if (int32_t(uint32_t(closed)) < 0) {
        result = uint64_t(int64_t(-1));
      } else if (const uint32_t name = Load(base, stream + 28); name != 0) {  // _tmpfname
        calls.free(name);
        Store(base, stream + 28, 0);
      }
    }
    Stream(base, stream).set_flags(0);
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
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      return;
    }
    if ((Stream(base, stream).flags() & 0x40) != 0) {
      Stream(base, stream).set_flags(0);
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
      if (i >= G::stream_count(base)) {
        break;
      }
      const uint32_t stream = Load(base, G::stream_table(base) + 4 * uint32_t(i));
      if (stream == 0) {
        continue;
      }
      if ((Stream(base, stream).flags() & 131) != 0 &&
          int32_t(uint32_t(Fclose(base, stream, calls))) != -1) {
        count++;
        Store(base, frame + 80, count);
      }
      if (i >= 20) {
        calls.free(Load(base, G::stream_table(base) + 4 * uint32_t(i)));
        Store(base, G::stream_table(base) + 4 * uint32_t(i), 0);
      }
    }
    UnlockStreamsFinally(calls);
    result = Load(base, frame + 80);
  });
  return result;
}


namespace {

// _isatty, compiled to the shared "returns 0" (sub_825C8388).
uint32_t Isatty(uint32_t) { return vehicle_accessors::DebugSwitchOff(); }

}  // namespace

uint64_t Flsbuf(uint8_t* base, uint32_t character, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AEE28 _flsbuf");
  uint64_t result = uint64_t(int64_t(-1));
  calls.in_frame(128, [&](uint32_t frame) {
    Store(base, frame + 148, character);
    const uint32_t handle = uint32_t(Fileno(base, stream, calls));
    const uint32_t flags = Stream(base, stream).flags();
    const auto fail = [&](uint32_t error) {
      if (error != 0) {
        schema::ErrnoSlot(base, Errno(base, calls)).set_value(error);
      }
      Stream(base, stream).set_flags(Stream(base, stream).flags() | 32);
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
      Stream(base, stream).set_room(0);
      if ((flags & 0x10) == 0) {
        Stream(base, stream).set_flags(flags | 32);
        return;
      }
      Stream(base, stream).set_pointer(Stream(base, stream).buffer());
      Stream(base, stream).set_flags(flags & ~1u);
    }
    uint32_t written = 0, count = 0;
    Stream(base, stream).set_room(0);
    const uint32_t now = (Stream(base, stream).flags() & ~0x10u) | 2;
    Stream(base, stream).set_flags(now);
    if ((now & 268) == 0) {
      const bool standard = stream == c_runtime::kStreams + 32 || stream == c_runtime::kStreams + 64;
      if (!standard || Isatty(handle) == 0) {
        Getbuf(base, stream, calls);
      }
    }
    if ((Stream(base, stream).flags() & 264) != 0) {
      const uint32_t buffer = Stream(base, stream).buffer();
      count = Stream(base, stream).pointer() - buffer;
      Stream(base, stream).set_room(Load(base, stream + 24) - 1);
      Stream(base, stream).set_pointer(buffer + 1);
      if (int32_t(count) > 0) {
        written = uint32_t(Write(base, handle, buffer, count, calls));
      } else {
        const uint32_t entry =
            handle == 0xFFFFFFFFu || handle == 0xFFFFFFFEu
                ? G::bad_io_info_address()
                : Load(base, G::io_info_blocks_address() + 4 * uint32_t(int32_t(handle) >> 5)) +
                      44 * (handle & 31);
        if ((CrtIoInfo(base, entry).osfile() & 0x20) != 0 &&
            int64_t(Lseek(base, handle, 0, 2, calls)) == -1) {
          fail(0);
          return;
        }
      }
      guest::StoreU8(base, Stream(base, stream).buffer(), uint8_t(Load(base, frame + 148)));
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
    G::set_cflush(base, G::cflush(base) + 1);
    if ((Stream(base, stream).flags() & 268) != 0) {
      return;
    }
    const uint32_t slot = G::std_buffers_address() + 4 * index;
    uint32_t buffer = Load(base, slot);
    if (buffer == 0) {
      buffer = calls.malloc(4096);
      Store(base, slot, buffer);
    }
    if (buffer == 0) {
      Stream(base, stream).set_buffer(stream + 20);
      Store(base, stream + 24, 2);
      Stream(base, stream).set_room(2);
      Stream(base, stream).set_pointer(stream + 20);
    } else {
      Stream(base, stream).set_buffer(buffer);
      Stream(base, stream).set_pointer(buffer);
      Store(base, stream + 24, 4096);
      Stream(base, stream).set_room(4096);
    }
    Stream(base, stream).set_flags(Stream(base, stream).flags() | 4354);
    result = 1;
  });
  return result;
}

uint64_t Ftbuf(uint8_t* base, uint32_t flag, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B41A0 _ftbuf");
  uint64_t result = flag;
  calls.in_frame(96, [&](uint32_t) {
    if (flag == 0 || (Stream(base, stream).flags() & 0x1000) == 0) {
      return;
    }
    result = Flush(base, stream, calls);
    const uint32_t flags = Stream(base, stream).flags() & ~0x100u & ~0x1000u;
    Store(base, stream + 24, 0);
    Stream(base, stream).set_pointer(0);
    Stream(base, stream).set_buffer(0);
    Stream(base, stream).set_flags(flags);
  });
  return result;
}


uint64_t Bsearch(uint8_t* base, uint64_t key, uint64_t first, uint32_t count, uint64_t width,
                 uint32_t compare, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AC970 bsearch");
  uint64_t result = 0;
  calls.in_frame(160, [&](uint32_t) {
    const auto step = [&](uint32_t n) {
      return uint64_t(int64_t(int32_t(n) * int64_t(int32_t(uint32_t(width)))));
    };
    uint64_t lo = first;
    uint64_t hi = step(count - 1) + first;
    if ((uint32_t(first) == 0 && count != 0) || uint32_t(width) == 0 || compare == 0) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      return;
    }
    if (uint32_t(lo) > uint32_t(hi)) {
      return;
    }
    uint64_t num = count;
    while (true) {
      const uint32_t half = (uint32_t(num) >> 1) & 0x7FFFFFFFu;
      if (half == 0) {
        if (uint32_t(num) != 0 && int32_t(uint32_t(calls.guest(compare, {key, lo}))) == 0) {
          result = lo;
        }
        return;
      }
      const bool odd = (num & 1) != 0;
      const uint64_t mid = step(odd ? half : half - 1) + lo;
      const int32_t order = int32_t(uint32_t(calls.guest(compare, {key, mid})));
      if (order == 0) {
        result = mid;
        return;
      }
      if (order < 0) {
        hi = mid - width;
        num = odd ? half : uint64_t(int64_t(int32_t(half)) - 1);
      } else {
        lo = mid + width;
        num = half;
      }
      if (uint32_t(lo) > uint32_t(hi)) {
        return;
      }
    }
  });
  return result;
}

namespace {


// memmove's bytes (sub_825AA840) within the caller's text.
void MoveText(uint8_t* base, uint32_t to, uint32_t from, uint32_t count) {
  std::vector<uint8_t> bytes(count);
  for (uint32_t i = 0; i < count; i++) {
    bytes[i] = guest::LoadU8(base, from + i);
  }
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU8(base, to + i, bytes[i]);
  }
}

}  // namespace

uint32_t Cftoe2(uint8_t* base, uint32_t buffer, uint32_t size, int32_t precision, uint32_t caps,
                uint32_t strflt, int32_t g_format, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825ADF28 _cftoe2_l");
  uint32_t result = 0;
  calls.in_frame(144, [&](uint32_t) {
    const auto invalid = [&](uint32_t error) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(error);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      result = error;
    };
    if (buffer == 0 || size == 0) {
      invalid(22);
      return;
    }
    if (!(size > uint32_t(precision > 0 ? precision : 0) + 9)) {
      invalid(34);
      return;
    }
    if (g_format != 0) {
      const uint32_t at = buffer + (schema::FloatStrings(base, strflt).sign() == 45 ? 1 : 0);
      if (precision > 0) {
        uint32_t end = at;
        while (guest::LoadU8(base, end) != 0) {
          end++;
        }
        MoveText(base, at + 1, at, end - at + 1);
      }
    }
    uint32_t p = buffer;
    if (int32_t(schema::FloatStrings(base, strflt).sign()) == 45) {
      guest::StoreU8(base, buffer, 45);
      p = buffer + 1;
    }
    if (precision > 0) {
      guest::StoreU8(base, p, guest::LoadU8(base, p + 1));
      p++;
      guest::StoreU8(base, p, guest::LoadU8(base, Load(base, Load(base, kDecimalPoint))));
    }
    const uint32_t e = p + (g_format == 0 ? 1 : 0) + uint32_t(precision);
    const uint32_t room = size == 0xFFFFFFFFu ? 0xFFFFFFFFu : buffer - e + size;
    if (!c_runtime::StrcpyS(base, e, room, G::exponent_text_address())) {
      if (uint32_t(calls.guest(0x825A9DD8, {e, room, G::exponent_text_address()})) != 0) {
        InvokeWatson(base, 0x825AE0D0, calls);
      }
    }
    const uint32_t digits = e + 2;
    if (caps != 0) {
      guest::StoreU8(base, e, 69);
    }
    uint32_t q = e + 1;
    if (guest::LoadU8(base, schema::FloatStrings(base, strflt).mantissa().address) != 48) {
      int32_t exponent =
          int32_t(uint32_t(schema::FloatStrings(base, strflt).decimal_exponent()) - 1);
      if (exponent < 0) {
        exponent = int32_t(0u - uint32_t(exponent));
        guest::StoreU8(base, q, 45);
      }
      q++;
      if (exponent >= 100) {
        guest::StoreU8(base, q, uint8_t(guest::LoadU8(base, q) + exponent / 100));
        exponent -= exponent / 100 * 100;
      }
      q++;
      if (exponent >= 10) {
        guest::StoreU8(base, q, uint8_t(guest::LoadU8(base, q) + exponent / 10));
        exponent -= exponent / 10 * 10;
      }
      guest::StoreU8(base, q + 1, uint8_t(guest::LoadU8(base, q + 1) + exponent));
    }
    if ((G::output_format(base) & 1) != 0 && guest::LoadU8(base, digits) == 48) {
      MoveText(base, digits, digits + 1, 3);
    }
  });
  return result;
}

uint32_t CxxSetUnhandledExceptionFilter(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825DAC58 __CxxSetUnhandledExceptionFilter");
  uint32_t previous = 0;
  calls.in_frame(96, [&](uint32_t) {
    calls.atexit(0x825B4D90);
    previous = SetUnhandledExceptionFilter(base, 0x825B4CD0);
    G::set_cxx_previous_filter(base, previous);
  });
  return previous;
}


uint64_t FflushNolock(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825ACD10 _fflush_nolock");
  uint64_t result = 0;
  calls.in_frame(96, [&](uint32_t) {
    if (stream == 0) {
      result = Flsall(base, 0, calls);
      return;
    }
    if (int32_t(uint32_t(Flush(base, stream, calls))) != 0) {
      result = uint64_t(int64_t(-1));
      return;
    }
    if ((Stream(base, stream).flags() & 0x4000) != 0) {
      const uint64_t committed = Commit(base, uint32_t(Fileno(base, stream, calls)), calls);
      result = uint32_t(committed) == 0 ? 0 : uint64_t(int64_t(-1));
    }
  });
  return result;
}

uint32_t Flsall(uint8_t* base, uint32_t flag, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825ACD90 flsall");
  uint32_t result = 0;
  calls.in_frame(144, [&](uint32_t frame) {
    Store(base, frame + 164, flag);
    Store(base, frame + 84, 0);
    Store(base, frame + 88, 0);
    calls.guest(kLock, {1});
    for (int32_t i = 0;; i++) {
      Store(base, frame + 80, uint32_t(i));
      if (i >= G::stream_count(base)) {
        break;
      }
      const uint32_t index = uint32_t(i);
      const uint32_t stream = Load(base, G::stream_table(base) + 4 * index);
      if (stream == 0 || (Stream(base, stream).flags() & 131) == 0) {
        continue;
      }
      LockFile2(index, stream, true, calls);
      const uint32_t now = Load(base, G::stream_table(base) + 4 * index);
      const uint32_t flags = Load(base, now + 12);
      if ((flags & 131) != 0) {
        if (int32_t(flag) == 1) {
          if (int32_t(uint32_t(FflushNolock(base, now, calls))) != -1) {
            Store(base, frame + 84, Load(base, frame + 84) + 1);
          }
        } else if (flag == 0 && (flags & 2) != 0 &&
                   int32_t(uint32_t(FflushNolock(base, now, calls))) == -1) {
          Store(base, frame + 88, 0xFFFFFFFFu);
        }
      }
      FlsallInnerFinally(base, index, G::stream_table_address(), calls);
    }
    UnlockStreamsFinally(calls);
    result = int32_t(Load(base, frame + 164)) == 1 ? Load(base, frame + 84)
                                                   : Load(base, frame + 88);
  });
  return result;
}

uint32_t Fflush(uint8_t* base, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825ACF70 fflush");
  uint32_t result = 0;
  calls.in_frame(112, [&](uint32_t frame) {
    Store(base, frame + 132, stream);
    if (stream == c_runtime::kStreams + 32 || stream == c_runtime::kStreams + 64) {
      return;
    }
    if (stream == 0) {
      result = Flsall(base, 0, calls);
      return;
    }
    LockFile(base, stream, true, calls);
    Store(base, frame + 80, uint32_t(FflushNolock(base, stream, calls)));
    UnlockFileFinally(base, stream, calls);
    result = Load(base, frame + 80);
  });
  return result;
}

uint32_t Flushall(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD068 _flushall");
  return Flsall(base, 1, calls);
}

void Endstdio(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD5F8 _endstdio");
  calls.in_frame(96, [&](uint32_t) {
    Flushall(base, calls);
    if (G::exit_flag(base) != 0) {
      Fcloseall(base, calls);
    }
    calls.free(G::stream_table(base));
  });
}


uint64_t Doexit(uint8_t* base, uint32_t code, uint32_t quick, uint32_t ret, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA218 doexit");
  uint64_t result = code;
  calls.in_frame(128, [&](uint32_t frame) {
    Store(base, frame + 164, ret);
    result = calls.guest(kLock, {8});
    if (G::c_exit_done(base) != 1) {
      if (G::c_termination_done(base) == 1) {
        result = calls.import(Import::kKeBugCheck, {0});
      }
      G::set_c_termination_done(base, 1);
      G::set_exit_flag(base, uint8_t(ret));
      if (int32_t(quick) == 0) {
        const uint32_t begin = schema::CRuntimeGlobals::exit_table_begin(base).address;
        uint32_t end = schema::CRuntimeGlobals::exit_table_end(base);
        Store(base, frame + 80, end);
        if (begin != 0) {
          while (true) {
            end -= 4;
            Store(base, frame + 80, end);
            if (end < begin) {
              break;
            }
            if (const uint32_t function = Load(base, end); function != 0) {
              result = calls.guest(function, {});
            }
          }
        }
        Initterm(base, G::pre_terminators_address(), G::pre_terminators_address() + 8, calls);
      }
      Initterm(base, G::terminators_address(), G::terminators_address() + 4, calls);
    }
    bool called = false;
    const uint64_t unlocked = ExitUnlockFinally(ret, &called, calls);
    if (called) {
      result = unlocked;
    }
    if (int32_t(Load(base, frame + 164)) == 0) {
      result = calls.import(Import::kKeBugCheck, {0});
    }
  });
  return result;
}

uint64_t Raise(uint8_t* base, uint32_t signal, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B49C8 raise");
  uint64_t result = 0;
  calls.in_frame(160, [&](uint32_t frame) {
    Store(base, frame + 180, signal);
    Store(base, frame + 84, 0);
    Store(base, frame + 88, 0);
    const int32_t sig = int32_t(signal);
    uint32_t locked = 0, data = 0, slot = 0;
    switch (sig) {
      case 2: slot = G::sigint_action_address(); locked = 1; break;
      case 21: slot = G::sigbreak_action_address(); locked = 1; break;
      case 6:
      case 22: slot = G::sigabrt_action_address(); locked = 1; break;
      case 15: slot = G::sigterm_action_address(); locked = 1; break;
      case 4:
      case 8:
      case 11: {
        data = GetptdNoexit(base, calls);
        Store(base, frame + 88, data);
        if (data == 0) {
          result = ~uint64_t(0);
          return;
        }
        const uint32_t table = ThreadData(base, data).exception_actions();
        const uint32_t count = uint32_t(G::exception_action_count(base));
        uint32_t entry = table;
        do {
          if (schema::ExceptionAction(base, entry).signal() == sig) {
            break;
          }
          entry += 12;
        } while (entry < count * 12 + table);
        if (!(entry < count * 12 + table) || schema::ExceptionAction(base, entry).signal() != sig) {
          entry = 0;
        }
        slot = schema::ExceptionAction(base, entry).action_address();
        break;
      }
      default:
        schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
        InvalidParameter(base, 0, 0, 0, 0, 0, calls);
        result = ~uint64_t(0);
        return;
    }
    if (locked != 0) {
      Store(base, frame + 84, 1);
    }
    const uint32_t action = Load(base, slot);
    Store(base, frame + 80, action);
    if (action == 1) {
      return;
    }
    if (action == 0) {
      Doexit(base, 3, 1, 0, calls);  // _exit(3): ends the process
    }
    if (locked != 0) {
      calls.guest(kLock, {0});
    }
    if (sig == 8 || sig == 11 || sig == 4) {
      Store(base, frame + 96, Load(base, data + 96));
      Store(base, data + 96, 0);
      if (sig == 8) {
        Store(base, frame + 100, Load(base, data + 100));
        Store(base, data + 100, 140);
      }
    }
    if (sig == 8) {
      uint32_t i = uint32_t(G::first_fpe_index(base));
      Store(base, frame + 92, i);
      while (int32_t(i) < int32_t(uint32_t(G::fpe_count(base)) + uint32_t(G::first_fpe_index(base)))) {
        schema::ExceptionAction(base, ThreadData(base, data).exception_actions() + 12 * i)
            .set_action(0);
        i++;
        Store(base, frame + 92, i);
      }
    } else {
      Store(base, slot, 0);
    }
    bool called = false;
    RaiseFinally(locked, &called, calls);
    const uint32_t again = Load(base, frame + 180);
    const uint32_t handler = Load(base, frame + 80);
    data = Load(base, frame + 88);
    if (int32_t(again) == 8) {
      calls.guest(handler, {8, Load(base, data + 100)});
    } else {
      calls.guest(handler, {again});
    }
    if (int32_t(again) == 8 || int32_t(again) == 11 || int32_t(again) == 4) {
      Store(base, data + 96, Load(base, frame + 96));
      if (int32_t(again) == 8) {
        Store(base, data + 100, Load(base, frame + 100));
      }
    }
    result = 0;
  });
  return result;
}

void Abort(uint8_t* base, uint32_t return_address, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD070 abort");
  calls.in_frame(2816, [&](uint32_t frame) {
    Store(base, frame + 2808, return_address);
    if ((G::abort_behavior(base) & 1) != 0) {
      NmsgWrite(base, 10);
    }
    if (GetSigabrt(base) != 0) {
      Raise(base, 22, calls);
    }
    if ((G::abort_behavior(base) & 2) != 0) {
      guest::ForEachHostPart(base, frame + 176, 2624,
                             [](uint8_t* host, uint32_t, uint32_t bytes) {
                               std::memset(host, 0, bytes);
                             });
      for (uint32_t offset = 96; offset < 176; offset += 4) {
        Store(base, frame + offset, 0);
      }
      Store(base, frame + 96, 0x40000015u);
      Store(base, frame + 108, Load(base, frame + 2808));
      Store(base, frame + 80, frame + 96);
      Store(base, frame + 84, frame + 176);
      SetUnhandledExceptionFilter(base, 0);
      UnhandledExceptionFilter(base, frame + 80, calls);
    }
    Doexit(base, 3, 1, 0, calls);  // _exit(3): ends the process
  });
}

void Purecall(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A7098 _purecall");
  calls.in_frame(96, [&](uint32_t) {
    if (const uint32_t handler = Load(base, kPurecallHandler); handler != 0) {
      calls.guest(handler, {});
    }
    NmsgWrite(base, 25);
    SetAbortBehavior(base, 0, 1);
    Abort(base, 0x825A70D4, calls);
  });
}

namespace {

void CopyBytes(uint8_t* base, uint32_t to, uint32_t from, uint32_t size, const Calls& calls) {
  if (!c_runtime::Memcpy(base, to, from, size)) {
    calls.guest(0x825A7410, {to, from, size});
  }
}

}  // namespace

uint64_t FwriteNolock(uint8_t* base, uint32_t buffer, uint32_t size, uint64_t count,
                      uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A96A0 _fwrite_nolock");
  uint64_t result = 0;
  calls.in_frame(416, [&](uint32_t frame) {
    const uint32_t total = size * uint32_t(count);
    if (total == 0) {
      result = 0;
      return;
    }
    uint32_t left = total;
    if (stream == c_runtime::kStreams + 32 || stream == c_runtime::kStreams + 64) {
      do {
        const uint32_t piece = left < 255 ? left : 255;
        CopyBytes(base, frame + 80, buffer, piece, calls);
        guest::StoreU8(base, frame + 80 + piece, 0);
        helpers::DebugOutput(nullptr);
        left -= piece;
      } while (left != 0);
      result = count;
      return;
    }
    const auto done = [&] { return uint64_t((total - left) / size); };
    const auto fail = [&] {
      Stream(base, stream).set_flags(Stream(base, stream).flags() | 0x20);
      result = done();
    };
    uint32_t buffer_size = (Stream(base, stream).flags() & 0x10C) != 0 ? Load(base, stream + 24) : 4096;
    do {
      const uint32_t buffered = Stream(base, stream).flags() & 0x108;
      const uint32_t room = buffered != 0 ? Stream(base, stream).room() : 0;
      if (buffered != 0 && room != 0) {
        if (int32_t(room) < 0) {
          fail();
          return;
        }
        const uint32_t n = left < room ? left : room;
        CopyBytes(base, Stream(base, stream).pointer(), buffer, n, calls);
        Stream(base, stream).set_room(Stream(base, stream).room() - n);
        Stream(base, stream).set_pointer(Stream(base, stream).pointer() + n);
        left -= n;
        buffer += n;
      } else if (left >= buffer_size) {
        if (buffered != 0 && uint32_t(Flush(base, stream, calls)) != 0) {
          result = done();
          return;
        }
        const uint32_t n = buffer_size != 0 ? left / buffer_size * buffer_size : left;
        const uint32_t handle = uint32_t(Fileno(base, stream, calls));
        const uint32_t written = uint32_t(Write(base, handle, buffer, n, calls));
        if (int32_t(written) == -1) {
          fail();
          return;
        }
        const uint32_t took = written > n ? n : written;
        left -= took;
        buffer += took;
        if (written < n) {
          fail();
          return;
        }
      } else {
        const int8_t byte = int8_t(guest::LoadU8(base, buffer));
        if (int32_t(Flsbuf(base, uint32_t(int32_t(byte)), stream, calls)) == -1) {
          result = done();
          return;
        }
        buffer++;
        left--;
        buffer_size = Load(base, stream + 24);
        if (int32_t(buffer_size) <= 0) {
          buffer_size = 1;
        }
      }
    } while (left != 0);
    result = count;
  });
  return result;
}

uint64_t Fputs(uint8_t* base, uint32_t string, uint32_t stream, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AD280 fputs");
  uint64_t result = ~uint64_t(0);
  calls.in_frame(144, [&](uint32_t frame) {
    Store(base, frame + 172, stream);
    const auto invalid = [&] {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      result = ~uint64_t(0);
    };
    if (string == 0 || stream == 0) {
      invalid();
      return;
    }
    if ((Stream(base, stream).flags() & 0x40) == 0) {
      const auto entry = [&] {
        const int32_t handle = int32_t(uint32_t(Fileno(base, stream, calls)));
        if (handle == -1 || handle == -2) {
          return G::bad_io_info_address();
        }
        return Load(base, G::io_info_blocks_address() + 4 * uint32_t(handle >> 5)) +
               44 * (uint32_t(handle) & 31);
      };
      if ((CrtIoInfo(base, entry()).text_mode() & 0xFE) != 0 ||
          (CrtIoInfo(base, entry()).text_mode() & 1) != 0) {
        invalid();
        return;
      }
    }
    uint32_t length = 0;
    while (guest::LoadU8(base, string + length) != 0) {
      length++;
    }
    Store(base, frame + 80, length);
    LockFile(base, stream, true, calls);
    const uint32_t buffering = Stbuf(base, stream, calls);
    Store(base, frame + 84, uint32_t(FwriteNolock(base, string, 1, length, stream, calls)));
    Ftbuf(base, buffering, stream, calls);
    UnlockFileFinally(base, stream, calls);
    result = Load(base, frame + 80) == Load(base, frame + 84) ? 0 : ~uint64_t(0);
  });
  return result;
}

void DebugMessage(uint8_t* base, uint32_t message, uint32_t detail, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A6948 _Debug_message");
  calls.in_frame(112, [&](uint32_t) {
    constexpr uint32_t kStderr = c_runtime::kStreams + 64;
    Fflush(base, 0, calls);
    Fputs(base, message, kStderr, calls);
    Fputs(base, detail != 0 ? detail : 0x82000CF4u, kStderr, calls);
    Fputs(base, 0x82013A84u, kStderr, calls);
    Abort(base, 0x825A69BC, calls);
  });
}

void Throw(uint8_t* base, uint32_t exception, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825A69D8 std::_Throw");
  calls.in_frame(96, [&](uint32_t) {
    const uint32_t what = uint32_t(calls.guest(Load(base, Load(base, exception) + 4), {exception}));
    DebugMessage(base, 0x820722BCu, what, calls);
  });
}

std::optional<uint64_t> Vsprintf(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args,
                                 uint32_t sp, const Calls& calls) {
  if (buffer == 0 || format == 0) {
    ITC_NATIVE_RUNNING("sub_825A71B0 vsprintf");
    ITC_NATIVE_RUNNING("sub_825A70F0 _vsprintf_l");
    calls.in_frame(128, [&](uint32_t) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
    });
    return ~uint64_t(0);
  }
  int32_t count = 0;
  if (!c_runtime::Sprintf(base, buffer, format, args, sp - 128 - 1328, &count)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825A71B0 vsprintf");
  ITC_NATIVE_RUNNING("sub_825A70F0 _vsprintf_l");
  return uint32_t(count);
}

uint32_t Fltout2(uint8_t* base, uint64_t bits, uint32_t result, uint32_t buffer, uint32_t size,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B6458 _fltout2");
  const c_runtime::LongDouble12 value = c_runtime::DoubleToLd12(bits);
  c_runtime::FloatOut out;
  if (!c_runtime::I10Output(base, value, 17, 0, &out)) {
    return uint32_t(calls.guest(0x825B6458, {bits, result, buffer, size}));  // never: 17 digits
  }
  calls.in_frame(160, [&](uint32_t frame) {
    Store(base, frame + 176, uint32_t(bits >> 32));
    Store(base, frame + 180, uint32_t(bits));
    guest::StoreU16(base, frame + 80, value.sign_exponent);
    Store(base, frame + 82, value.mantissa_high);
    Store(base, frame + 86, value.mantissa_low);
    for (uint32_t i = 0; i < sizeof(out.bytes); i++) {
      if (out.written[i]) {
        guest::StoreU8(base, frame + 96 + i, out.bytes[i]);
      }
    }
    const schema::FloatStrings strings(base, result);
    strings.set_flag(uint32_t(out.result));
    strings.set_sign(uint32_t(int32_t(int8_t(guest::LoadU8(base, frame + 98)))));
    strings.set_decimal_exponent(int32_t(int16_t(guest::LoadU16(base, frame + 96))));
    if (!c_runtime::StrcpyS(base, buffer, size, frame + 100)) {
      if (uint32_t(calls.guest(0x825A9DD8, {buffer, size, frame + 100})) != 0) {
        InvokeWatson(base, 0x825B64E8, calls);
      }
    }
    strings.set_mantissa(buffer);
  });
  return result;
}

uint64_t FpToStrGuest(uint8_t* base, uint32_t buffer, uint32_t size, int32_t digits,
                      uint32_t result, const Calls& calls) {
  if (buffer == 0 || size == 0 || !(size > uint32_t(digits > 0 ? digits : 0) + 1)) {
    ITC_NATIVE_RUNNING("sub_825B61E8 _fptostr");
    uint64_t error = 0;
    calls.in_frame(96, [&](uint32_t) {
      if (buffer != 0 && size != 0) {
        guest::StoreU8(base, buffer, 0);
      }
      error = buffer != 0 && size != 0 ? 34 : 22;
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(uint32_t(error));
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
    });
    return error;
  }
  c_runtime::GuestText text(base, buffer);
  std::vector<uint8_t> mantissa;
  const schema::FloatStrings strings(base, result);
  const uint32_t source = strings.mantissa().address;
  uint32_t read = 0;
  const uint32_t whole = digits > 0 ? uint32_t(digits) : 0;
  while (digits >= 0 && read <= whole && read < (1u << 20)) {
    const uint8_t c = guest::LoadU8(base, source + read);
    read++;
    if (c == 0) {
      break;
    }
    mantissa.push_back(c);
  }
  const int32_t before = strings.decimal_exponent();
  int32_t exponent = before;
  bool native = c_runtime::FpToStr(text, 0, size, digits, &exponent, mantissa) == 0 && text.ok();
  if (native) {
    const uint32_t end = uint32_t(text.WrittenEnd());
    native = !c_runtime::MayShare(result, 16, buffer, end) &&
             !c_runtime::MayShare(source, read, buffer, end) && text.Commit();
  }
  if (!native) {
    return calls.guest(0x825B61E8, {buffer, size, uint64_t(int64_t(digits)), result});
  }
  if (exponent != before) {
    strings.set_decimal_exponent(exponent);
  }
  return 0;
}

namespace {

// _cftof2_l (sub_825AE708) over guest memory, for %g (its callers here have
// checked the buffer and size): c_runtime::Cftof2, the original (a guest
// call) where its writes would share memory with what it reads.
uint64_t Cftof2Guest(uint8_t* base, uint32_t buffer, uint32_t size, int32_t precision,
                     uint32_t result, uint32_t g_format, uint32_t locale, const Calls& calls) {
  c_runtime::GuestText text(base, buffer);
  bool native = buffer != 0 && size != 0;
  if (native) {
    uint32_t words[3];
    const uint32_t point = c_runtime::DecimalPointAddress(base, words);
    const schema::FloatStrings strings(base, result);
    native = c_runtime::Cftof2(text, precision, int32_t(strings.sign()),
                               strings.decimal_exponent(), int8_t(g_format) != 0,
                               guest::LoadU8(base, point));
    const uint32_t end = uint32_t(text.WrittenEnd());
    native = native && !c_runtime::MayShare(result, 8, buffer, end) &&
             !c_runtime::MayShare(words[0], 4, buffer, end) &&
             !c_runtime::MayShare(words[1], 4, buffer, end) &&
             !c_runtime::MayShare(point, 1, buffer, end) && text.Commit();
  }
  if (!native) {
    return calls.guest(0x825AE708,
                       {buffer, size, uint64_t(int64_t(precision)), result, g_format, locale});
  }
  return 0;
}

}  // namespace

uint64_t Cftoe(uint8_t* base, uint32_t number, uint32_t buffer, uint32_t size, int32_t precision,
               uint32_t caps, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AE1A8 _cftoe_l");
  uint64_t r3 = 0;
  calls.in_frame(176, [&](uint32_t frame) {
    const uint64_t bits = (uint64_t(Load(base, number)) << 32) | Load(base, number + 4);
    Fltout2(base, bits, frame + 80, frame + 96, 22, calls);
    if (buffer == 0 || size == 0) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      r3 = 22;
      return;
    }
    const uint32_t minus = schema::FloatStrings(base, frame + 80).sign() == 45 ? 1 : 0;
    const uint32_t point = precision > 0 ? 1 : 0;
    const uint32_t room = size == 0xFFFFFFFFu ? 0xFFFFFFFFu : size - minus - point;
    r3 = FpToStrGuest(base, buffer + minus + point, room, precision + 1, frame + 80, calls);
    if (uint32_t(r3) != 0) {
      guest::StoreU8(base, buffer, 0);
      return;
    }
    r3 = Cftoe2(base, buffer, size, precision, caps, frame + 80, 0, calls);
  });
  return r3;
}

uint64_t Cftog(uint8_t* base, uint32_t number, uint32_t buffer, uint32_t size, int32_t precision,
               uint32_t caps, uint32_t locale, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AE9D0 _cftog_l");
  uint64_t r3 = 0;
  calls.in_frame(192, [&](uint32_t frame) {
    const uint64_t bits = (uint64_t(Load(base, number)) << 32) | Load(base, number + 4);
    Fltout2(base, bits, frame + 80, frame + 96, 22, calls);
    if (buffer == 0 || size == 0) {
      schema::ErrnoSlot(base, Errno(base, calls)).set_value(22);
      InvalidParameter(base, 0, 0, 0, 0, 0, calls);
      r3 = 22;
      return;
    }
    const uint32_t minus = schema::FloatStrings(base, frame + 80).sign() == 45 ? 1 : 0;
    const int32_t before =
        int32_t(uint32_t(schema::FloatStrings(base, frame + 80).decimal_exponent()) - 1);
    const uint32_t digits = buffer + minus;
    const uint32_t room = size == 0xFFFFFFFFu ? 0xFFFFFFFFu : size - minus;
    r3 = FpToStrGuest(base, digits, room, precision, frame + 80, calls);
    if (uint32_t(r3) != 0) {
      guest::StoreU8(base, buffer, 0);
      return;
    }
    const int32_t exponent =
        int32_t(uint32_t(schema::FloatStrings(base, frame + 80).decimal_exponent()) - 1);
    const bool carried = before < exponent;
    if (exponent < -4 || exponent >= precision) {
      r3 = Cftoe2(base, buffer, size, precision, caps, frame + 80, 1, calls);
      return;
    }
    if (carried) {
      uint32_t p = digits;
      uint8_t c;
      do {
        c = guest::LoadU8(base, p);
        p++;
      } while (c != 0);
      guest::StoreU8(base, p - 2, c);
    }
    r3 = Cftof2Guest(base, buffer, size, precision, frame + 80, 1, locale, calls);
  });
  return r3;
}

}  // namespace c_runtime::core
