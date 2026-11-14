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
#include <optional>

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
  kExTerminateThread,
  kNtFlushBuffersFile,
  kNtQueryInformationFile,
  kNtSetInformationFile,
  kNtWriteFile,
  kNtWaitForSingleObjectEx,
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
  // `body(frame)` in a guest frame of `bytes` below the stack pointer (its
  // back chain stored), the calls inside made below it: the originals'
  // locals where theirs are.
  std::function<void(uint32_t bytes, const std::function<void(uint32_t frame)>& body)> in_frame;
  // The translated code's unconditional trap (`twi 31,r0,type`).
  std::function<void(uint32_t type)> trap;
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

constexpr uint32_t kExitRoutine = 0x8266B7B0;      // _aexit_rtn
constexpr uint32_t kInvalidParameterHandler = 0x82978320;
constexpr uint32_t kBugCheckStub = 0x825AA1A0;

// sub_825AA1A0 the initial _aexit_rtn (probably): KeBugCheck(0). No harness:
// ends the process (the user's call, 2026-11-14).
void BugCheckStub(const Calls& calls);

// sub_825BA420 RaiseException: in its 176-byte frame, the exception record
// at +80 (code, flags & 1, 0, the address 0x825BA420, the parameter count
// capped at 15 with the parameters copied by sub_825A7830's copy; a count
// of 0 stored for no parameters) given to RtlRaiseException; r3 as it
// leaves it. No harness: ends the process (the user's call, 2026-11-14).
uint64_t RaiseException(uint8_t* base, uint32_t code, uint32_t flags, uint32_t count,
                        uint32_t parameters, const Calls& calls);

// sub_825AD198 _CxxThrowException: in its 144-byte frame, the 32-byte record
// template at 0x8207275C (memcpy) with the object at +104 and the throw info
// at +108 (+100 = 0x01994000 when its attributes' bit 3 is set), raised
// through RaiseException. No harness: ends the process.
uint64_t CxxThrowException(uint8_t* base, uint32_t object, uint32_t throw_info,
                           const Calls& calls);

// sub_825AFCC8 _invalid_parameter: the handler at kInvalidParameterHandler,
// when set, called with r3-r7 as given (from a 96-byte frame), its r3
// returned; else _crt_debugger_hook and the trap (type 22), r3 = 2 (the trap
// path: no harness, the user's call, 2026-11-14).
uint64_t InvalidParameter(uint8_t* base, uint32_t expression, uint32_t function, uint32_t file,
                          uint32_t line, uint32_t reserved, const Calls& calls);

// sub_825BA558 XapiThreadStartup (probably): in its 128-byte frame, +80 = 0,
// the notify routines with 1, `routine(parameter)` (its result to +80), the
// notify routines with 0, ExTerminateThread(the result). No harness: ends
// the thread (the user's call, 2026-11-14).
void ThreadStartup(uint8_t* base, uint32_t routine, uint32_t parameter, const Calls& calls);

// sub_825AA158 _amsg_exit: _FF_MSGBANNER, _NMSG_WRITE(number), then the
// exit routine at kExitRoutine with 255 (from a 96-byte frame); its r3.
uint64_t AmsgExit(uint8_t* base, uint32_t number, const Calls& calls);

// sub_825AA580 _getptd: _getptd_noexit, _amsg_exit(16) when that's 0; the
// data.
uint32_t Getptd(uint8_t* base, const Calls& calls);

// sub_825B64F8 _fptrap: _amsg_exit(2) (its tail call); its r3.
uint64_t Fptrap(uint8_t* base, const Calls& calls);

constexpr uint32_t kDebugMonitorData = 0x820007F0;  // [it]: KeDebugMonitorData's address
constexpr uint32_t kUnhandledFilter = 0x829714A0;   // SetUnhandledExceptionFilter's word
constexpr uint32_t kCxxPreviousFilter = 0x82971348;  // the filter before the C++ one

// sub_82297940 UnhandledExceptionFilter: from a 96-byte frame, the debug
// monitor's routine (its data's +24) with (10, 0) when one is attached
// ([[kDebugMonitorData]] not 0); when that gives 0 (or there's none), the
// filter at kUnhandledFilter with the exception pointers when set: -1 when
// it returns -1, else 0.
uint32_t UnhandledExceptionFilter(uint8_t* base, uint32_t pointers, const Calls& calls);

// sub_825B9BA8 a debug monitor notification (probably): `value` rounded to a
// float stored at the caller's frame + 28 (the original's r1 + 124 in its
// 96-byte frame); with a debug monitor attached, its routine with (39, code |
// 0x02200000), then (39, the float's bits) when one is still attached.
void DebugMonitorNotify(uint8_t* base, uint32_t code, double value, const Calls& calls);

// sub_825B4D90 __CxxRestoreUnhandledExceptionFilter:
// SetUnhandledExceptionFilter([kCxxPreviousFilter]) (its tail call); the old
// filter.
uint32_t CxxRestoreUnhandledExceptionFilter(uint8_t* base);

// sub_825AFD10 _invoke_watson (probably): in its 2,816-byte frame, a zeroed
// 2,624-byte context at +176 (memset) and an 80-byte exception record at +96
// (code 0xC000000D, its address +12 the caller's return address, the
// original's saved link register at +2808), the pointers at +80 (+96, +176);
// SetUnhandledExceptionFilter(0), UnhandledExceptionFilter(+80),
// _crt_debugger_hook when that gives 0, then KeBugCheck(30). No harness: ends
// the process (the user's call, 2026-11-14).
void InvokeWatson(uint8_t* base, uint32_t return_address, const Calls& calls);

// sub_825B7958 __lock_fhandle: in its 128-byte frame (the handle stored at
// +148, the caller's frame + 20; +80 the result, 1), the handle's entry's
// critical section (+12) made under _lock(10) when +8 reads 0 (read again
// under the lock; __crtInitCritSecAndSpinCount with 4000, +80 = 0 when it
// fails, +8 counted up either way), then the __finally (_unlock(10)); entered
// when the result is 1. The result.
uint32_t LockFhandle(uint8_t* base, uint32_t handle, const Calls& calls);

// sub_825B7A20 __lock_fhandle's __finally: from a 96-byte frame, _unlock(10);
// its r3 (the adapter reloads the handle and result from the frame).
uint64_t LockFhandleFinally(const Calls& calls);

// sub_825B77E8 _free_osfhnd: a handle below _nhandle that's open (+4 bit 0)
// with an OS handle (+0 not -1): +0 = -1, 0; else errno 9, _doserrno 0, -1.
// From a 96-byte frame.
uint32_t FreeOsfhnd(uint8_t* base, uint32_t handle, const Calls& calls);

// sub_825B7880 _get_osfhandle: -2: _doserrno 0, errno 9, -1; out of range
// or not open: the same, then _invalid_parameter(0, 0, 0, 0, 0), -1; else
// its OS handle (+0). From a 96-byte frame.
uint32_t GetOsfhandle(uint8_t* base, uint32_t handle, const Calls& calls);

// sub_825B4C8C raise's __finally: from a 112-byte frame, _unlock(0) when
// raise took the lock (`locked`, its r28); _unlock's r3, or nullopt-like
// `called` false.
uint64_t RaiseFinally(uint32_t locked, bool* called, const Calls& calls);

// sub_825B4F38 _fcloseall's and sub_825ACEB4 flsall's outer __finally: from a
// 96-byte frame, _unlock(1); its r3.
uint64_t UnlockStreamsFinally(const Calls& calls);

// sub_825ACF04 flsall's inner __finally: from a 112-byte frame,
// _unlock_file2(index, [[piob_word] + 4 index]); its r3.
uint64_t FlsallInnerFinally(uint8_t* base, uint32_t index, uint32_t piob_word, const Calls& calls);

constexpr uint32_t kDecimalPoint = 0x8266C024;  // [[it]]: the locale's decimal point

// sub_825ABE30 exp (probably, without error reporting): step for step as the
// translated code, its scratch words below the stack pointer `sp` written
// as the original leaves them (-32 the rounded exponent, then the result's
// doubles at -24, -16 and -32 when it's scaled). The result.
double Exp(uint8_t* base, uint32_t sp, double x);

// sub_825ADD38 _forcdecpt_l: the locale's decimal point inserted after the
// digits (or the 'e', or an 'x' and the character after it), the rest moved
// up a byte through its terminator. r3 as the original leaves it (the last
// tolower's).
uint32_t Forcdecpt(uint8_t* base, uint32_t text);

// sub_825ADDD8 _cropzeros_l: from the decimal point, the zeros before the
// exponent (and the point itself when nothing else is left) dropped, the
// exponent moved down. r3 as the original leaves it (where it found the
// point, or the terminator).
uint32_t Cropzeros(uint8_t* base, uint32_t text);

// sub_825B6098 _atodbl_l and sub_825B6140 _atoflt_l: `string` through
// __strgtold12_l (no scaling) and _ld12tod or _ld12tof into the double or
// float at `out`; 3 (overflow) or 4 (underflow) from the parse's flags or
// the conversion's status, else 0. nullopt, with nothing written, where the
// parse's plain C++ leaves it to the original (never: c_runtime::Strgtold12).
std::optional<uint32_t> AtoNumber(uint8_t* base, uint32_t out, uint32_t string, bool is_double);

// sub_825ADEB0 _fassign_l: `string` into the double (`is_double`) or float at
// `out` through AtoNumber; its result (r3 as the original leaves it).
std::optional<uint32_t> Fassign(uint8_t* base, bool is_double, uint32_t out, uint32_t string);


// sub_825B0E88 _fileno: from a 96-byte frame, a stream's handle (+16, as
// loaded); a null stream: errno 22, _invalid_parameter(0, 0, 0, 0, 0), -1
// (sign-extended). r3 as the original leaves it.
uint64_t Fileno(uint8_t* base, uint32_t stream, const Calls& calls);

// The __finally funclets giving back a file handle's lock: _close's
// (sub_825B0E50, its r30), _commit's (sub_825B4978, its r27), _write's
// (sub_825B1304, its r30) and _lseeki64's (sub_825B6730, its r30): from a 112-byte frame, _unlock_fhandle(handle);
// its r3.
uint64_t UnlockFhandleFinally(uint8_t* base, uint32_t handle, const Calls& calls);

// sub_825A9664 fclose's, sub_825AD02C fflush's and sub_825AD488 fputs's
// __finally: from a 112-byte frame, _unlock_file(stream) (their r30); its r3.
uint64_t UnlockFileFinally(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_825AA338 doexit's unwind funclet: from a 112-byte frame, _unlock(8)
// (a guest call: the exit lock's host mutex is routed there) when `locked`
// (its r28) isn't 0; `called` says whether r3 was set.
uint64_t ExitUnlockFinally(uint32_t locked, bool* called, const Calls& calls);

// sub_825B3498 terminate: from a 96-byte frame, the thread's terminate
// handler (_getptd's +108) called (r3 the data) when set, then
// KeBugCheck(0xC0000144). No harness: ends the process (the user's call,
// 2026-11-14).
void Terminate(uint8_t* base, const Calls& calls);


// sub_825B0C28 _close_nolock: from a 112-byte frame, unless _get_osfhandle
// gives -1, or the handle is 1 with 2 open (or 2 with 1 open) and both have
// the same OS handle, CloseHandle on its OS handle (GetLastError when that
// fails); _free_osfhnd; its flags (+4) = 0; _dosmaperr and -1 on an error,
// else 0.
uint32_t CloseNolock(uint8_t* base, uint32_t handle, const Calls& calls);

// sub_825B0D10 _close: in its 144-byte frame (the handle at +164, the
// caller's frame + 20; the result at +80): -2: _doserrno 0, errno 9, -1; out
// of range or not open: the same and _invalid_parameter(0, 0, 0, 0, 0), -1;
// else __lock_fhandle, _close_nolock when still open (else errno 9, -1), its
// __finally. r3 as the original leaves it (-1 sign-extended, the result
// zero-extended).
uint64_t Close(uint8_t* base, uint32_t handle, const Calls& calls);

// sub_825BA510 FlushFileBuffers: from a 96-byte frame (the I/O status block
// at +80), NtFlushBuffersFile; 1, or 0 with the last error set from the
// status.
uint32_t FlushFileBuffers(uint32_t handle, const Calls& calls);

// sub_825B4828 _commit: in its 144-byte frame (the handle at +164, the
// result at +80): -2: errno 9, -1; out of range or not open: errno 9,
// _invalid_parameter(0, 0, 0, 0, 0), -1; else __lock_fhandle and, when still
// open, FlushFileBuffers on its OS handle (on failure _doserrno = the last
// error, errno 9, -1), not open: errno 9, -1; its __finally. r3 as _close's.
uint64_t Commit(uint8_t* base, uint32_t handle, const Calls& calls);


// sub_825B9DD8 SetFilePointer: in its 192-byte frame (+80 the position, +88
// the I/O status block, +96 the 56-byte network open information), the
// position from `low` (and the high word at `high`, when given, else `low`
// sign-extended) moved by `method` (0 from the start; 2 from the end of
// file, class 34; 1 and anything else from the current position, class
// 14); a negative one: last error 131; past 31 bits without `high`: 87; else
// set (class 14), the high word stored back; on a status failure the last
// error from it (and -1 at `high`). The new low word (the last error set to
// 0 when it's -1), else -1. r3 as the original leaves it (-1
// sign-extended).
uint64_t SetFilePointer(uint8_t* base, uint32_t handle, uint32_t low, uint32_t high,
                        uint32_t method, const Calls& calls);

// sub_825B6500 _lseeki64_nolock: from a 112-byte frame (the offset at +80),
// SetFilePointer on the handle's OS handle (-1: errno 9, -1), its error (when
// it gives -1 with a last error) through _dosmaperr, -1; else the handle's
// end-of-file flag (+4 bit 1) cleared, the 64-bit position. r3 as the
// original leaves it.
uint64_t LseekNolock(uint8_t* base, uint32_t handle, uint64_t offset, uint32_t origin,
                     const Calls& calls);

// sub_825B65D0 _lseeki64: in its 160-byte frame (the handle at +180, the
// result at +80, -1 first): _close's checks (-1), then under __lock_fhandle
// _lseeki64_nolock when still open, else errno 9, _doserrno 0, -1; its
// __finally (sub_825B6730, as _close's). The 64-bit result.
uint64_t Lseek(uint8_t* base, uint32_t handle, uint64_t offset, uint32_t origin,
               const Calls& calls);


// sub_82295D50 WriteFile: from a 128-byte frame; `written` (when given) set
// to 0. With an overlapped: its +0 = 259 (pending), the offset from +12 and
// +8 at +88 (high, low), NtWriteFile with its event (+16) and the overlapped
// as the I/O status block (and the APC context unless the event's low bit is
// set); an error or pending status: the last error from it, 0; else
// `written` = its +4, 1. Without: NtWriteFile with the status block at +80,
// waiting on the handle while pending (a failed wait's status taken as the
// result); success: `written` = +84, 1 (stored without a null check, as the
// original); a warning (0x8xxxxxxx): the same store, then as an error; an
// error: the last error from it, 0.
uint32_t WriteFile(uint8_t* base, uint32_t handle, uint32_t buffer, uint32_t size,
                   uint32_t written, uint32_t overlapped, const Calls& calls);

// sub_825B0F48 _write_nolock: in its 1,232-byte frame (the written count at
// +80, the text mode's 1,024-byte chunk at +96): 0 for no bytes; a null
// buffer, or an odd size in a UTF-16 or UTF-8 handle (+40's mode, as the
// signed byte >> 1, 2 or 1): _doserrno 0, errno 22, _invalid_parameter, -1;
// an append handle (+4 bit 5) seeked to its end first (_lseeki64_nolock);
// text (+4 bit 7): the bytes in chunks with each LF given a CR before it,
// written until one is short or fails; binary: one WriteFile. Bytes written:
// their count less the CRs added; none: the error (5: errno 9 and
// _doserrno 5; others _dosmaperr), or with no error 0 for a device (+4 bit
// 6) whose first byte is ^Z, else errno 28, _doserrno 0; -1. r3 as the
// original leaves it.
uint64_t WriteNolock(uint8_t* base, uint32_t handle, uint32_t buffer, uint32_t size,
                     const Calls& calls);

// sub_825B11A8 _write: in its 160-byte frame (the handle at +180, the result
// at +80), _lseeki64's checks and lock around _write_nolock (not open after
// the lock: errno 9, _doserrno 0, -1); its __finally (sub_825B1304). r3 as
// _close's.
uint64_t Write(uint8_t* base, uint32_t handle, uint32_t buffer, uint32_t size,
               const Calls& calls);


// sub_825ACC78 _flush: from a 112-byte frame, a stream being written (+12's
// low bits 2) with a buffer (_IOMYBUF or _IOSETVBUF) holding bytes (+0 less
// +8, above 0): _write of them to its _fileno; all written: _IOWRT (2)
// cleared for a read-write stream (_IORW, 0x80); else _IOERR (0x20) set, -1.
// Then +0 = +8 and +4 = 0. 0, or -1 (sign-extended).
uint64_t Flush(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_825B4CD0 __CxxUnhandledExceptionFilter: from a 112-byte frame, a C++
// exception (code 0xE06D7363, 3 parameters, magic 0x19930520 to 0x19930522)
// goes to terminate (which ends the process); then the filter before it
// (kCxxPreviousFilter), when set and _ValidateExecute passes it, called with
// the pointers: its r3; else 0.
uint64_t CxxUnhandledExceptionFilter(uint8_t* base, uint32_t pointers, const Calls& calls);


// sub_825A94C0 _fclose_nolock: from a 112-byte frame, a null stream: errno
// 22, _invalid_parameter(0, 0, 0, 0, 0), -1. A stream in use (+12 & 0x83):
// _flush (its result kept), _freebuf, _close(_fileno) (-1 when it fails,
// else the temporary file's name at +28 freed and cleared); -1 for one not
// in use; then +12 = 0. r3 as the original leaves it (-1 sign-extended,
// _flush's r3 as it gave it).
uint64_t FcloseNolock(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_825A9580 fclose: in its 112-byte frame (the stream at +132, the
// caller's frame + 20; the result at +80, -1 first): a null stream as
// _fclose_nolock's; a string stream (+12 & 0x40): +12 = 0, -1; else under
// _lock_file, _fclose_nolock, then its __finally (sub_825A9664). r3 as the
// original leaves it (-1 sign-extended for a null stream, else the result
// as loaded).
uint64_t Fclose(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_825B4E78 _fcloseall: in its 144-byte frame (+80 the count, +84 the
// index), under _lock(1), each __piob slot from 3 below _nstream: a stream
// in use closed by fclose (counted unless it gives -1), and the slots from
// 20 freed and cleared; then its __finally (sub_825B4F38). The count.
uint32_t Fcloseall(uint8_t* base, const Calls& calls);

// sub_825AEE28 _flsbuf: in its 128-byte frame (the character at +148, the
// caller's frame + 20): _fileno; a stream not writable (+12 & 0x82 0): errno
// 9, a string stream (0x40): errno 34, each with _IOERR (0x20) set and -1. A
// read stream (bit 0) not at its end (0x10) gets _IOERR and -1, else its
// pointer reset to the buffer and bit 0 cleared; then +4 = 0, _IOEOF
// cleared, _IOWRT set; no buffer yet (+12 & 0x10C 0): _getbuf, unless it's
// stdout or stderr and _isatty (the "returns 0" stub) says a terminal. With a
// buffer: +4 = its size - 1, +0 = the buffer + 1, the bytes before written
// (_write) when there are any, else an append handle (+4 bit 5) seeked to
// its end (_lseeki64; -1 is an error), then the character's low byte stored
// at the buffer; without: _write of the character's word's first byte (its
// high byte, as the original's address). Written short of the count: _IOERR,
// -1 (sign-extended); else the character's low byte.
uint64_t Flsbuf(uint8_t* base, uint32_t character, uint32_t stream, const Calls& calls);

// sub_825B40B8 _stbuf: from a 112-byte frame, _fileno, then _isatty (the
// "returns 0" stub): 0. Past it (never taken): stdout or stderr only,
// _cflush counted, none for a stream with a buffer (+12 & 0x10C); the shared
// buffer at 0x82971330 + 4 x n (malloc(4096) when 0; the 2-byte one at +20
// when that fails) as +8 and +0, its size at +24 and +4, +12 |= 0x1102; 1.
uint32_t Stbuf(uint8_t* base, uint32_t stream, const Calls& calls);

// sub_825B41A0 _ftbuf: from a 96-byte frame, `flag` not 0 and the stream's
// temporary buffer bit (+12 & 0x1000): _flush, then +24, +0, +8 = 0 and
// 0x100 and 0x1000 cleared. r3 as the original leaves it (`flag`, or
// _flush's).
uint64_t Ftbuf(uint8_t* base, uint32_t flag, uint32_t stream, const Calls& calls);

}  // namespace c_runtime::core
