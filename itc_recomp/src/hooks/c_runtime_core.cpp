// The C runtime's core natively (docs/functions/c-runtime-core.md): adapters
// in front of src/c_runtime/core.cpp, the kernel imports called as the
// originals call them (B7's goal, the user's call, 2026-11-14).

#include "hooks/c_runtime_core.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/video_output.h"
#include "hooks/heap.h"
#include "hooks/last_error.h"
#include "memory/c_runtime_heap.h"

ITC_FLAG_BOOL(native_c_runtime_core, true, "ITC",
              "Run the C runtime's core natively instead of through the translated code "
              "(docs/functions/c-runtime-core.md), calling the kernel imports as the originals "
              "do: __crtTlsAlloc (sub_825AA3F8), _getptd_noexit (sub_825AA4C0, with "
              "__set_flsgetvalue sub_825AA400), _freefls (sub_825AA5C0), _mtterm (sub_825AA448, "
              "with _mtdeletelocks sub_825B3260), _errno and __doserrno (sub_825A81E0, sub_825A8218), "
              "_dosmaperr and _get_errno_from_oserr (sub_825A8250, sub_825A8178), _freeptd "
              "(sub_825AA670), __crtInitCritSecAndSpinCount and __crtInitCritSecNoSpinCount "
              "(sub_825B35A0, sub_825B3570), XRegisterThreadNotifyRoutine and "
              "XapiCallThreadNotifyRoutines (sub_82297B98, sub_82297B38), _mtinitlocks "
              "(sub_825B31D8), the C runtime's thread notify routine (sub_825B3650) and _mtinit "
              "(sub_825AA708, with _init_pointers' plain C++), atol and strtol (sub_825AAF08, "
              "sub_825A85C0; strtoxl's errno paths the originals'), strrchr (sub_825A8630), "
              "_initterm (sub_825AA1B8), _set_abort_behavior (sub_825AD120), __get_sigabrt "
              "(sub_825B49B0), _crt_debugger_hook (sub_825B68B8), _ValidateExecute (sub_825B8910), "
              "SetUnhandledExceptionFilter (sub_82297870), GetCommandLineA (sub_822979D0), "
              "_positive (sub_825ADE90), _NMSG_WRITE (sub_825AEDA0), _getbuf and _freebuf "
              "(sub_825B4790, sub_825B0EE0), CloseHandle (sub_82294E20), ResetEvent "
              "(sub_825BA168), _initterm_e and _ioinit (sub_82297AC0, sub_825B4668), _initstdio "
              "(sub_825AD4D0), _FF_MSGBANNER (sub_825AEDE8), _lock_file, _unlock_file, _lock_file2 "
              "and _unlock_file2 (sub_825AD638, sub_825AD688, sub_825AD670, sub_825AD6C0; _lock and "
              "_unlock guest calls) and _unlock_fhandle (sub_825B7A58) (checked by --run_harnesses=c_runtime_core). "
              "Those reached only from native callers have no adapters, and the flag switches "
              "their callers: __set_flsgetvalue, _mtdeletelocks, _freefls, _errno, "
              "__crtInitCritSecAndSpinCount, XRegisterThreadNotifyRoutine, _freeptd, _mtterm, "
              "_mtinitlocks");

ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

namespace cc = c_runtime::core;

namespace {

bool NativeHeap() { return ITC_FLAG(native_c_runtime_heap) && hooks::NativeHeapActive(); }

void SetArgs(PPCContext& ctx, std::initializer_list<uint64_t> args) {
  PPCRegister* const r[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                             &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
  int i = 0;
  for (uint64_t arg : args) {
    r[i++]->u64 = arg;
  }
}

}  // namespace

cc::Calls hooks::CRuntimeCoreCalls(PPCContext& ctx, uint8_t* base) {
  cc::Calls calls;
  calls.import = [&ctx, base](cc::Import import, std::initializer_list<uint64_t> args) {
    const uint64_t lr = ctx.lr;
    SetArgs(ctx, args);
    switch (import) {
      case cc::Import::kKeTlsAlloc: __imp__KeTlsAlloc(ctx, base); break;
      case cc::Import::kKeTlsGetValue: __imp__KeTlsGetValue(ctx, base); break;
      case cc::Import::kKeTlsSetValue: __imp__KeTlsSetValue(ctx, base); break;
      case cc::Import::kKeTlsFree: __imp__KeTlsFree(ctx, base); break;
      case cc::Import::kRtlInitializeCriticalSection:
        __imp__RtlInitializeCriticalSection(ctx, base);
        break;
      case cc::Import::kRtlEnterCriticalSection: __imp__RtlEnterCriticalSection(ctx, base); break;
      case cc::Import::kRtlLeaveCriticalSection: __imp__RtlLeaveCriticalSection(ctx, base); break;
      case cc::Import::kNtClose: __imp__NtClose(ctx, base); break;
      case cc::Import::kNtClearEvent: __imp__NtClearEvent(ctx, base); break;
      case cc::Import::kRtlRaiseException: __imp__RtlRaiseException(ctx, base); break;
      case cc::Import::kKeBugCheck: __imp__KeBugCheck(ctx, base); break;
    }
    ctx.lr = lr;
    return ctx.r3.u64;
  };
  calls.guest = [&ctx, base](uint32_t function, std::initializer_list<uint64_t> args) {
    const uint64_t lr = ctx.lr, sp = ctx.r1.u64;
    SetArgs(ctx, args);
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    return ctx.r3.u64;
  };
  calls.get_last_error = [&ctx, base]() { return last_error::Get(base, ctx.r13.u32); };
  calls.set_last_error = [&ctx, base](uint32_t error) {
    last_error::Set(base, ctx.r13.u32, error);
  };
  calls.calloc = [&ctx, base](uint32_t count, uint32_t size) -> uint32_t {
    if (NativeHeap()) {
      if (const auto block = memory::crt::Calloc(count, size)) {
        return *block;
      }
    }
    const uint64_t lr = ctx.lr;
    ctx.r3.u64 = count;
    ctx.r4.u64 = size;
    sub_825AC320(ctx, base);
    ctx.lr = lr;
    return ctx.r3.u32;
  };
  calls.malloc = [&ctx, base](uint32_t size) -> uint32_t {
    if (NativeHeap()) {
      if (const auto block = memory::crt::Malloc(base, size)) {
        return *block;
      }
    }
    const uint64_t lr = ctx.lr;
    ctx.r3.u64 = size;
    sub_825A9A90(ctx, base);
    ctx.lr = lr;
    return ctx.r3.u32;
  };
  calls.free = [&ctx, base](uint32_t block) {
    if (NativeHeap() && memory::crt::Free(block)) {
      return;
    }
    const uint64_t lr = ctx.lr;
    ctx.r3.u64 = block;
    sub_825A9B58(ctx, base);
    ctx.lr = lr;
  };
  calls.thread_id = []() { return graphics::video_output::CurrentThreadId(); };
  calls.set_last_error_from_status = [&ctx, base](uint32_t status) {
    last_error::SetFromStatus(base, ctx.r13.u32, status);
  };
  return calls;
}

void hooks::CoreCrtTlsAlloc(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CrtTlsAlloc(ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreSetFlsGetValue(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::SetFlsGetValue(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreGetptdNoexit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::GetptdNoexit(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFreeFls(PPCContext& ctx, uint8_t* base) {
  cc::FreeFls(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreMtDeleteLocks(PPCContext& ctx, uint8_t* base) {
  cc::MtDeleteLocks(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreMtTerm(PPCContext& ctx, uint8_t* base) {
  cc::MtTerm(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreErrno(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Errno(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreDosErrno(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::DosErrno(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreGetErrnoFromOsErr(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::GetErrnoFromOsErr(base, ctx.r3.u32);
}

void hooks::CoreDosMapErr(PPCContext& ctx, uint8_t* base) {
  cc::DosMapErr(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFreePtd(PPCContext& ctx, uint8_t* base) {
  cc::FreePtd(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCritSecNoSpinCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CritSecNoSpinCount(ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreInitCritSecAndSpinCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 =
      cc::InitCritSecAndSpinCount(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreRegisterThreadNotifyRoutine(PPCContext& ctx, uint8_t* base) {
  cc::RegisterThreadNotifyRoutine(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCallThreadNotifyRoutines(PPCContext& ctx, uint8_t* base) {
  cc::CallThreadNotifyRoutines(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreMtInitLocks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::MtInitLocks(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreThreadNotify(PPCContext& ctx, uint8_t* base) {
  cc::ThreadNotify(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreMtInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::MtInit(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreStrtol(PPCContext& ctx, uint8_t* base) {
  uint64_t result = 0;
  if (!cc::Strtol(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, &result)) {
    __imp__sub_825A85C0(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

void hooks::CoreAtol(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825AAF08 atol");
  ctx.r4.u64 = 0;
  ctx.r5.u64 = 10;
  CoreStrtol(ctx, base);
}

void hooks::CoreStrrchr(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Strrchr(base, ctx.r3.u32, ctx.r4.u32);
}

void hooks::CoreInitterm(PPCContext& ctx, uint8_t* base) {
  cc::Initterm(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreSetAbortBehavior(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::SetAbortBehavior(base, ctx.r3.u32, ctx.r4.u32);
}

void hooks::CoreGetSigabrt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::GetSigabrt(base);
}

void hooks::CoreCrtDebuggerHook(PPCContext&, uint8_t* base) { cc::CrtDebuggerHook(base); }

void hooks::CoreValidateExecute(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = cc::ValidateExecute(ctx.r3.u32);
}

void hooks::CoreSetUnhandledExceptionFilter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::SetUnhandledExceptionFilter(base, ctx.r3.u32);
}

void hooks::CoreGetCommandLineA(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::GetCommandLineA(base);
}

void hooks::CorePositive(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cc::Positive(base, ctx.r3.u32);
}

void hooks::CoreNmsgWrite(PPCContext& ctx, uint8_t* base) { cc::NmsgWrite(base, ctx.r3.u32); }

void hooks::CoreGetbuf(PPCContext& ctx, uint8_t* base) {
  cc::Getbuf(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFreebuf(PPCContext& ctx, uint8_t* base) {
  cc::Freebuf(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCloseHandle(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CloseHandle(ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreResetEvent(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::ResetEvent(ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreInittermE(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::InittermE(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreIoInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = int32_t(cc::IoInit(base, CRuntimeCoreCalls(ctx, base)));
}

void hooks::CoreInitStdio(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::InitStdio(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFfMsgBanner(PPCContext&, uint8_t* base) { cc::FfMsgBanner(base); }

void hooks::CoreLockFile(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::LockFile(base, ctx.r3.u32, true, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreUnlockFile(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::LockFile(base, ctx.r3.u32, false, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreLockFile2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::LockFile2(ctx.r3.u32, ctx.r4.u32, true, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreUnlockFile2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::LockFile2(ctx.r3.u32, ctx.r4.u32, false, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreUnlockFhandle(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::UnlockFhandle(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

#define ITC_C_RUNTIME_CORE(address, step)       \
  extern "C" REX_FUNC(sub_##address) {          \
    REX_FUNC_PROLOGUE();                        \
    if (!ITC_FLAG(native_c_runtime_core)) {     \
      __imp__sub_##address(ctx, base);          \
      return;                                   \
    }                                           \
    hooks::step(ctx, base);                     \
  }

// __crtTlsAlloc: r3 = the FLS callback (ignored); r3 = the TLS index.
ITC_C_RUNTIME_CORE(825AA3F8, CoreCrtTlsAlloc)
// _getptd_noexit: r3 = the calling thread's data, or 0.
ITC_C_RUNTIME_CORE(825AA4C0, CoreGetptdNoexit)
// __doserrno: r3 = the address. (_errno sub_825A81E0, _freefls
// sub_825AA5C0, __crtInitCritSecAndSpinCount sub_825B35A0,
// XRegisterThreadNotifyRoutine sub_82297B98, _freeptd sub_825AA670, _mtterm
// sub_825AA448 and _mtinitlocks sub_825B31D8 have no adapters: their
// callers in play are native.)
ITC_C_RUNTIME_CORE(825A8218, CoreDosErrno)
// _get_errno_from_oserr: r3 = a Windows error; r3 = its errno.
ITC_C_RUNTIME_CORE(825A8178, CoreGetErrnoFromOsErr)
// _dosmaperr: r3 = a Windows error.
ITC_C_RUNTIME_CORE(825A8250, CoreDosMapErr)
// __crtInitCritSecNoSpinCount: r3 = a critical section; r3 = 1.
ITC_C_RUNTIME_CORE(825B3570, CoreCritSecNoSpinCount)
// XapiCallThreadNotifyRoutines: r3 = 1 at a thread's start, 0 at its end.
ITC_C_RUNTIME_CORE(82297B38, CoreCallThreadNotifyRoutines)
// The C runtime's thread notify routine: r3 = 1 at a thread's start, 0 at
// its end.
ITC_C_RUNTIME_CORE(825B3650, CoreThreadNotify)
// _mtinit: r3 = 1, or 0.
ITC_C_RUNTIME_CORE(825AA708, CoreMtInit)
// atol: r3 = a string; strtol(r3, 0, 10) (strtol, sub_825A85C0, has no
// adapter: atol is its only caller).
ITC_C_RUNTIME_CORE(825AAF08, CoreAtol)
// strrchr: r3 = a string, r4 = a character; r3 = the last one, or 0.
ITC_C_RUNTIME_CORE(825A8630, CoreStrrchr)
// _initterm: r3 = begin, r4 = end.
ITC_C_RUNTIME_CORE(825AA1B8, CoreInitterm)
// _set_abort_behavior: r3 = flags, r4 = mask; r3 = the old value.
ITC_C_RUNTIME_CORE(825AD120, CoreSetAbortBehavior)
// __get_sigabrt: r3 = SIGABRT's action.
ITC_C_RUNTIME_CORE(825B49B0, CoreGetSigabrt)
// _crt_debugger_hook: r3 ignored.
ITC_C_RUNTIME_CORE(825B68B8, CoreCrtDebuggerHook)
// _ValidateExecute: r3 = a pointer; r3 = 1 when it isn't 0.
ITC_C_RUNTIME_CORE(825B8910, CoreValidateExecute)
// SetUnhandledExceptionFilter: r3 = the new filter; r3 = the old one.
ITC_C_RUNTIME_CORE(82297870, CoreSetUnhandledExceptionFilter)
// GetCommandLineA: r3 = the command line.
ITC_C_RUNTIME_CORE(822979D0, CoreGetCommandLineA)
// _positive: r3 = a double's address; r3 = 1 or 0.
ITC_C_RUNTIME_CORE(825ADE90, CorePositive)
// _NMSG_WRITE: r3 = a run-time error number.
ITC_C_RUNTIME_CORE(825AEDA0, CoreNmsgWrite)
// _getbuf, _freebuf: r3 = a stream.
ITC_C_RUNTIME_CORE(825B4790, CoreGetbuf)
ITC_C_RUNTIME_CORE(825B0EE0, CoreFreebuf)
// CloseHandle: r3 = a handle; ResetEvent: r3 = an event; r3 = 1 or 0.
ITC_C_RUNTIME_CORE(82294E20, CoreCloseHandle)
ITC_C_RUNTIME_CORE(825BA168, CoreResetEvent)
// _initterm_e: r3 = the first nonzero result, or 0.
ITC_C_RUNTIME_CORE(82297AC0, CoreInittermE)
// _ioinit: r3 = 0, or -1.
ITC_C_RUNTIME_CORE(825B4668, CoreIoInit)
// _initstdio: r3 = 0, or 26.
ITC_C_RUNTIME_CORE(825AD4D0, CoreInitStdio)
// _FF_MSGBANNER: no registers.
ITC_C_RUNTIME_CORE(825AEDE8, CoreFfMsgBanner)
// _lock_file, _unlock_file: r3 = a stream.
ITC_C_RUNTIME_CORE(825AD638, CoreLockFile)
ITC_C_RUNTIME_CORE(825AD688, CoreUnlockFile)
// _lock_file2, _unlock_file2: r3 = a stream index, r4 = the stream.
ITC_C_RUNTIME_CORE(825AD670, CoreLockFile2)
ITC_C_RUNTIME_CORE(825AD6C0, CoreUnlockFile2)
// _unlock_fhandle: r3 = a file handle.
ITC_C_RUNTIME_CORE(825B7A58, CoreUnlockFhandle)
