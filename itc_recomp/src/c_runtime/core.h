// The C runtime's core (docs/functions/c-runtime-core.md): the wrappers of
// the game's statically linked C runtime and XAPI over kernel imports, as
// plain C++ over the same guest words the recompiled code uses (the TLS
// indexes, the per-thread data, the lock table, the stream and handle
// tables), one state for recompiled and native callers. By "B7's goal"
// (docs/loops/b7-loop.md, the user's call, 2026-11-14) they call the kernel
// imports exactly as the originals do, through Calls (guest calls the
// adapters make, src/hooks/c_runtime_core.cpp); only the imports wait for
// the core.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>

namespace c_runtime::core {

// The globals worked out (docs/known-data.md).
constexpr uint32_t kFlsIndex = 0x8266B7B4;       // __flsindex: the data's TLS index
constexpr uint32_t kGetValueIndex = 0x8266B7B8;  // __getvalueindex: the getter's TLS index
constexpr uint32_t kFlsAlloc = 0x8297115C;       // the FLS stand-ins _mtinit sets
constexpr uint32_t kFlsGetValue = 0x82971160;
constexpr uint32_t kFlsSetValue = 0x82971164;
constexpr uint32_t kFlsFree = 0x82971168;
constexpr uint32_t kXcptActTab = 0x8266BE60;     // _XcptActTab
constexpr uint32_t kThreadDataBytes = 196;
constexpr uint32_t kLockTable = 0x8266BD40;      // 36 entries of 8 bytes
constexpr uint32_t kLockCount = 36;
constexpr uint32_t kErrnoFallback = 0x8266B658;     // errno with no thread data
constexpr uint32_t kDosErrnoFallback = 0x8266B65C;  // _doserrno with none
constexpr uint32_t kOsErrorTable = 0x8266B4F0;      // 45 pairs (error, errno)
constexpr uint32_t kCritSecInit = 0x8297132C;       // the critical section initializer
constexpr uint32_t kCritSecNoSpinCount = 0x825B3570;
constexpr uint32_t kNotifyLock = 0x82638780;        // XAPI's notify list's lock
constexpr uint32_t kNotifyList = 0x8263879C;        // its head (+0 first, +4 last)
constexpr uint32_t kLockSections = 0x829711A0;      // the boot locks' critical sections
constexpr uint32_t kThreadStartHook = 0x820723A0;   // called at a thread's start
constexpr uint32_t kThreadEndHook = 0x820723A4;     // and at its end
constexpr uint32_t kCrtNotifyBlock = 0x82971150;    // the C runtime's notify block
constexpr uint32_t kCrtNotifyRoutine = 0x825B3650;

enum class Import {
  kKeTlsAlloc,
  kKeTlsGetValue,
  kKeTlsSetValue,
  kKeTlsFree,
  kRtlInitializeCriticalSection,
  kRtlEnterCriticalSection,
  kRtlLeaveCriticalSection,
  kNtClose,
  kNtClearEvent,
  kRtlRaiseException,
  kKeBugCheck,
};

struct Calls {
  // A kernel import with r3 onward set to `args`; its r3.
  std::function<uint64_t(Import import, std::initializer_list<uint64_t> args)> import;
  // A guest function by its address (a function pointer the game stores:
  // an import's thunk, recompiled code or an adapter) with r3 onward set to
  // `args`; its r3.
  std::function<uint64_t(uint32_t function, std::initializer_list<uint64_t> args)> guest;
  // GetLastError and SetLastError on the calling thread (hooks/last_error.h).
  std::function<uint32_t()> get_last_error;
  std::function<void(uint32_t error)> set_last_error;
  // calloc (sub_825AC320), malloc (sub_825A9A90) and free (sub_825A9B58):
  // the native heap's plain C++ when it's on the layer, else the game's.
  std::function<uint32_t(uint32_t count, uint32_t size)> calloc;
  std::function<uint32_t(uint32_t size)> malloc;
  std::function<void(uint32_t block)> free;
  // The calling guest thread's id (sub_825B9A60).
  std::function<uint32_t()> thread_id;
  // SetLastErrorFromStatus (sub_82296E88) on the calling thread.
  std::function<void(uint32_t status)> set_last_error_from_status;
};

// sub_825AA3F8 __crtTlsAlloc (probably): KeTlsAlloc (the FLS callback in r3
// ignored, as the original branches with it left in r3).
uint32_t CrtTlsAlloc(uint32_t callback, const Calls& calls);

// sub_825AA400 __set_flsgetvalue: the TLS slot [kGetValueIndex] given the
// getter [kFlsGetValue] on this thread when it reads 0; returns the value
// read, or KeTlsSetValue's result when it stored (r3 as the original leaves
// it; _getptd_noexit doesn't read it).
uint32_t SetFlsGetValue(uint8_t* base, const Calls& calls);

// sub_825AA4C0 _getptd_noexit: the calling thread's C runtime data (196
// bytes), made and set up when missing (calloc, stored through
// [kFlsSetValue]: +20 = 1, +92 = _XcptActTab, +0 the thread's id, +4 = -1;
// freed when the store fails), or 0. The last error kept around it.
uint32_t GetptdNoexit(uint8_t* base, const Calls& calls);

// sub_825AA5C0 _freefls: frees a thread's data (0: nothing) and its buffers
// at +36, +44, +52, +60, +68, +72 when set, +92 unless it's _XcptActTab.
void FreeFls(uint8_t* base, uint32_t data, const Calls& calls);

// sub_825B3260 _mtdeletelocks: frees every lock table entry's critical
// section that is set and wasn't made at boot (+4 not 1), clearing +0.
void MtDeleteLocks(uint8_t* base, const Calls& calls);

// sub_825AA448 _mtterm: __flsindex freed through [kFlsFree] and -1,
// __getvalueindex through KeTlsFree and -1 (each unless already -1), then
// _mtdeletelocks.
void MtTerm(uint8_t* base, const Calls& calls);

// sub_825A81E0 _errno: the calling thread's errno (its data +8), or
// kErrnoFallback without data.
uint32_t Errno(uint8_t* base, const Calls& calls);

// sub_825A8218 __doserrno: its _doserrno (+12), or kDosErrnoFallback.
uint32_t DosErrno(uint8_t* base, const Calls& calls);

// sub_825A8178 _get_errno_from_oserr: `error`'s errno from the table, else
// 13 (EACCES) for 19 to 36, 8 (ENOEXEC) for 188 to 202, 22 (EINVAL).
uint32_t GetErrnoFromOsErr(uint8_t* base, uint32_t error);

// sub_825A8250 _dosmaperr: `error` stored as _doserrno, then errno set to
// its _get_errno_from_oserr (each through _getptd_noexit, as the original).
void DosMapErr(uint8_t* base, uint32_t error, const Calls& calls);

// sub_825AA670 _freeptd: `data`, or the calling thread's (through the
// getter, when the getter slot is set) for 0: when __flsindex isn't -1, its
// FLS value set to 0 through [kFlsSetValue] and _freefls; then the getter
// slot set to 0 (KeTlsSetValue) unless __getvalueindex is -1.
void FreePtd(uint8_t* base, uint32_t data, const Calls& calls);

// sub_825B3570 __crtInitCritSecNoSpinCount (probably):
// RtlInitializeCriticalSection; returns 1.
uint32_t CritSecNoSpinCount(uint32_t section, const Calls& calls);

// sub_825B35A0 __crtInitCritSecAndSpinCount: the initializer at
// [kCritSecInit] (set to kCritSecNoSpinCount when 0) called with the
// section and spin count; its result.
uint32_t InitCritSecAndSpinCount(uint8_t* base, uint32_t section, uint32_t spin,
                                 const Calls& calls);

// sub_82297B98 XRegisterThreadNotifyRoutine: under kNotifyLock, `block`
// linked at the list's tail (`add` not 0) or unlinked.
void RegisterThreadNotifyRoutine(uint8_t* base, uint32_t block, uint32_t add,
                                 const Calls& calls);

// sub_82297B38 XapiCallThreadNotifyRoutines (probably): under kNotifyLock,
// each block's routine (+8) called with `starting`, the next block read
// before the call.
void CallThreadNotifyRoutines(uint8_t* base, uint32_t starting, const Calls& calls);

// sub_825B31D8 _mtinitlocks: each lock table entry made at boot (+4 = 1)
// given the next 28 bytes of kLockSections and initialized
// (__crtInitCritSecAndSpinCount with 4000); 1, or 0 with that entry's +0
// cleared when one fails.
uint32_t MtInitLocks(uint8_t* base, const Calls& calls);

// sub_825B3650 the C runtime's thread notify routine (probably): `starting`
// not 0: [kThreadStartHook] called (with `starting`) when set; 0:
// [kThreadEndHook] (with 0) when set, then _freeptd(0).
void ThreadNotify(uint8_t* base, uint32_t starting, const Calls& calls);

// sub_825AA708 _mtinit: the FLS stand-ins set, __getvalueindex = KeTlsAlloc
// holding the getter (KeTlsSetValue), _init_pointers, _mtinitlocks,
// __flsindex from [kFlsAlloc], the main thread's data made and stored as
// _getptd_noexit does, the notify routine registered. 1, or 0 (after
// _mtterm once the getter's slot is set).
uint32_t MtInit(uint8_t* base, const Calls& calls);

// sub_825A85C0 strtol: strtoxl (c_runtime::StrToXl) signed with the global
// locale; false where that leaves the original's path to the caller (its
// errno paths, a multibyte locale): the caller runs the original.
bool Strtol(uint8_t* base, uint32_t string, uint32_t end_out, int32_t radix, uint64_t* result);

// sub_825A8630 strrchr: the last `c` in `string` (compared as a 32-bit int
// with each byte), or 0; for 0, the terminator.
uint32_t Strrchr(uint8_t* base, uint32_t string, uint32_t c);

// sub_825AA1B8 _initterm: each nonzero word from `begin` up to `end`
// called (a guest call).
void Initterm(uint8_t* base, uint32_t begin, uint32_t end, const Calls& calls);

// sub_825AD120 _set_abort_behavior: __abort_behavior ([0x8266B7C4]) =
// (old & ~mask) | (flags & mask); the old value.
uint32_t SetAbortBehavior(uint8_t* base, uint32_t flags, uint32_t mask);

// sub_825B49B0 __get_sigabrt (probably): [0x82971340].
uint32_t GetSigabrt(uint8_t* base);

// sub_825B68B8 _crt_debugger_hook: [0x8297820C] = 0.
void CrtDebuggerHook(uint8_t* base);

// sub_825B8910 _ValidateExecute (probably, a stub): `pointer` != 0.
uint32_t ValidateExecute(uint32_t pointer);

// sub_82297870 SetUnhandledExceptionFilter: [0x829714A0] swapped with
// `filter`; the old one.
uint32_t SetUnhandledExceptionFilter(uint8_t* base, uint32_t filter);

// sub_822979D0 GetCommandLineA (probably): [0x820007EC].
uint32_t GetCommandLineA(uint8_t* base);

// sub_825ADE90 _positive: 1 unless the double at `number` is below the
// constant at 0x82002258 (0.0), as the original's compare (NaN gives 1).
uint32_t Positive(uint8_t* base, uint32_t number);

// sub_825AEDA0 _NMSG_WRITE: the run-time error `number`'s message (the
// 23-entry table at 0x8266BA88) to OutputDebugStringA, which drops it
// (helpers::DebugOutput); nothing for a number not in it.
void NmsgWrite(uint8_t* base, uint32_t number);

// sub_825B4790 _getbuf: _cflush ([0x82971174]) counted; malloc(4096) into
// +8 with +24 = 4096 and _IOMYBUF, else the 2-byte buffer at +20, +24 = 2,
// _IONBF; +4 = 0, +0 = +8.
void Getbuf(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_825B0EE0 _freebuf: when in use (+12 & 0x83) with _IOMYBUF, frees +8
// and clears +0, +4, +8 and the _IOMYBUF and _IOSETVBUF bits.
void Freebuf(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_82294E20 CloseHandle: NtClose; 1, or 0 with the last error set from
// the status.
uint32_t CloseHandle(uint32_t handle, const Calls& calls);

// sub_825BA168 ResetEvent: NtClearEvent; 1, or 0 as CloseHandle's.
uint32_t ResetEvent(uint32_t event, const Calls& calls);

constexpr uint32_t kIob = 0x8266B7D0;         // _iob: 20 streams of 32 bytes
constexpr uint32_t kNstream = 0x82978328;     // _nstream
constexpr uint32_t kPiob = 0x82978324;        // __piob
constexpr uint32_t kNhandle = 0x82978210;     // _nhandle
constexpr uint32_t kPioinfo = 0x82978220;     // __pioinfo (blocks of 32 handles)
constexpr uint32_t kLock = 0x825B3430;        // _lock (c-runtime.md)
constexpr uint32_t kUnlock = 0x825B32D0;      // _unlock

// sub_82297AC0 _initterm_e (probably XAPI's own): each nonzero word of
// 0x82600000-0x8260000C called (r3 = 0) until one returns nonzero; that,
// or 0.
uint64_t InittermE(uint8_t* base, const Calls& calls);  // r3 in 64 bits

// sub_825B4668 _ioinit: the first handle block (calloc(32, 44)) into
// __pioinfo[0], _nhandle = 32, each entry set up, the first three
// (+4 = 0xC1, +0 = -2); 0, or -1 when calloc fails.
uint32_t IoInit(uint8_t* base, const Calls& calls);

// sub_825AD4D0 _initstdio: _nstream 512 when 0, at least 20; __piob =
// calloc(_nstream, 4) (20 when that fails; 26 returned when both do); the
// first 20 slots at _iob's streams; the first three streams' handle -2 when
// their OS handle is -1, -2 or 0. 0.
uint32_t InitStdio(uint8_t* base, const Calls& calls);

// sub_825AEDE8 _FF_MSGBANNER: _NMSG_WRITE(252), then _NMSG_WRITE(255).
void FfMsgBanner(uint8_t* base);

// sub_825AD638 _lock_file and sub_825AD688 _unlock_file: a stream inside
// _iob (to +608) through _lock or _unlock(16 + its index) (guest calls: lock
// 8's host mutex is routed there), else Rtl{Enter,Leave}CriticalSection on
// stream + 32. r3 as the call leaves it.
uint32_t LockFile(uint8_t* base, uint32_t stream, bool lock, const Calls& calls);

// sub_825AD670 _lock_file2 and sub_825AD6C0 _unlock_file2: an index below
// 20 through _lock or _unlock(16 + index), else the stream's critical
// section (+32).
uint32_t LockFile2(uint32_t index, uint32_t stream, bool lock, const Calls& calls);

// sub_825B7A58 _unlock_fhandle: RtlLeaveCriticalSection on the handle's
// entry + 12.
uint32_t UnlockFhandle(uint8_t* base, uint32_t handle, const Calls& calls);

}  // namespace c_runtime::core
