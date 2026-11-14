// The C runtime's core natively (docs/functions/c-runtime-core.md): adapters
// in front of src/c_runtime/core.cpp, the kernel imports called as the
// originals call them (B7's goal, the user's call, 2026-11-14).

#include "hooks/c_runtime_core.h"

#include "c_runtime/exit.h"
#include "c_runtime/format.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/video_output.h"
#include "hooks/c_runtime.h"
#include "hooks/guest_call.h"
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
              "_unlock guest calls), _unlock_fhandle (sub_825B7A58), _invalid_parameter "
              "(sub_825AFCC8), _amsg_exit (sub_825AA158), _getptd (sub_825AA580), _fptrap "
              "(sub_825B64F8), UnhandledExceptionFilter (sub_82297940), the debug monitor "
              "notification (sub_825B9BA8) and __CxxRestoreUnhandledExceptionFilter "
              "(sub_825B4D90), __lock_fhandle with its __finally (sub_825B7958, sub_825B7A20), "
              "_free_osfhnd and _get_osfhandle (sub_825B77E8, sub_825B7880), and the __finally "
              "funclets of raise, _fcloseall and flsall (sub_825B4C8C, sub_825B4F38, "
              "sub_825ACEB4, sub_825ACF04), exp (sub_825ABE30), _forcdecpt_l and _cropzeros_l "
              "(sub_825ADD38, sub_825ADDD8), _fassign_l, _atodbl_l, _atoflt_l and _ld12tof "
              "(sub_825ADEB0, sub_825B6098, sub_825B6140, sub_825B70D0; __strgtold12_l's and "
              "_ld12tod's plain C++), _fileno (sub_825B0E88), the __finally funclets of _close, _write, "
              "_commit and fclose and doexit's unwind funclet (sub_825B0E50, sub_825B1304, "
              "sub_825B4978, sub_825A9664, sub_825AA338), _close_nolock and _close (sub_825B0C28, "
              "sub_825B0D10), FlushFileBuffers and _commit (sub_825BA510, sub_825B4828), "
              "_lseeki64_nolock and _lseeki64 with its __finally (sub_825B6500, sub_825B65D0, "
              "sub_825B6730), _write with _write_nolock and WriteFile (sub_825B11A8, sub_825B0F48, "
              "sub_82295D50), the __finally funclets of fflush and fputs (sub_825AD02C, sub_825AD488), "
              "_flush (sub_825ACC78), __CxxUnhandledExceptionFilter (sub_825B4CD0; its "
              "terminate path checked by reading), _fcloseall with fclose and _fclose_nolock "
              "(sub_825B4E78, sub_825A9580, sub_825A94C0), _flsbuf, _stbuf and _ftbuf "
              "(sub_825AEE28, sub_825B40B8, sub_825B41A0; _stbuf's path past the _isatty stub "
              "checked by reading), bsearch (sub_825AC970), _cftoe2_l (sub_825ADF28; its "
              "_invoke_watson path checked by reading), _fassign, _forcdecpt and _cropzeros "
              "(sub_825ADF10, sub_825ADF18, sub_825ADF20) and __CxxSetUnhandledExceptionFilter "
              "(sub_825DAC58), fflush with _fflush_nolock and flsall (sub_825ACF70, sub_825ACD10, "
              "sub_825ACD90), _flushall and _endstdio (sub_825AD068, sub_825AD5F8), doexit and _cexit "
              "(sub_825AA218, sub_825AA388; only the path where _C_Exit_Done is set: the others "
              "run the game's exit functions or end the process, checked by reading) (checked by "
              "--run_harnesses=c_runtime_core); checked by reading, as "
              "they end the process or thread: the initial _aexit_rtn (sub_825AA1A0), "
              "RaiseException (sub_825BA420), _CxxThrowException (sub_825AD198), "
              "XapiThreadStartup (sub_825BA558), _invoke_watson (sub_825AFD10) and "
              "_invalid_parameter's trap, terminate (sub_825B3498), _exit (sub_825AA378). "
              "Those reached only from native callers have no adapters, and the flag switches "
              "their callers: __set_flsgetvalue, _mtdeletelocks, _freefls, _errno, "
              "__crtInitCritSecAndSpinCount, XRegisterThreadNotifyRoutine, _freeptd, _mtterm, "
              "_mtinitlocks, CloseHandle (its caller, SaveSlot::WaitClose), SetFilePointer (sub_825B9DD8, "
              "its caller _lseeki64_nolock), _write_nolock and WriteFile (sub_825B0F48, sub_82295D50, "
              "their caller _write), _close_nolock (sub_825B0C28, its caller _close), fclose and "
              "_fclose_nolock (their callers _fcloseall and fclose), _close, _freebuf and "
              "fclose's __finally (sub_825B0D10, sub_825B0EE0, sub_825A9664, their caller "
              "_fclose_nolock or fclose), _fcloseall's __finally (sub_825B4F38), _fflush_nolock, "
              "flsall and _flushall (their callers fflush, flsall and _endstdio), the __finally "
              "funclets of flsall and fflush (sub_825ACEB4, sub_825ACF04, sub_825AD02C), doexit "
              "(sub_825AA218, its callers _exit and _cexit)");

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
      case cc::Import::kExTerminateThread: __imp__ExTerminateThread(ctx, base); break;
      case cc::Import::kNtFlushBuffersFile: __imp__NtFlushBuffersFile(ctx, base); break;
      case cc::Import::kNtQueryInformationFile: __imp__NtQueryInformationFile(ctx, base); break;
      case cc::Import::kNtSetInformationFile: __imp__NtSetInformationFile(ctx, base); break;
      case cc::Import::kNtWriteFile: __imp__NtWriteFile(ctx, base); break;
      case cc::Import::kNtWaitForSingleObjectEx: __imp__NtWaitForSingleObjectEx(ctx, base); break;
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
  calls.in_frame = [&ctx, base](uint32_t bytes, const std::function<void(uint32_t)>& body) {
    const uint64_t lr = ctx.lr;
    guest_call::InFrame(ctx, base, bytes, uint32_t(lr), body);
    ctx.lr = lr;
  };
  calls.trap = [&ctx, base](uint32_t type) { ppc_trap(ctx, base, uint16_t(type)); };
  calls.atexit = [&ctx, base](uint32_t function) {
    const uint64_t lr = ctx.lr;
    hooks::GuestAtexit guest{ctx, base};
    const int32_t result = c_runtime::AtexitFrom(base, function, guest.Call());
    ctx.lr = lr;
    return result;
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

void hooks::CoreBugCheckStub(PPCContext& ctx, uint8_t* base) {
  cc::BugCheckStub(CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreRaiseException(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::RaiseException(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                   CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCxxThrowException(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CxxThrowException(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreInvalidParameter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::InvalidParameter(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                    ctx.r7.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreThreadStartup(PPCContext& ctx, uint8_t* base) {
  cc::ThreadStartup(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreAmsgExit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::AmsgExit(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreGetptd(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Getptd(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFptrap(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Fptrap(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreUnhandledExceptionFilter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = int32_t(cc::UnhandledExceptionFilter(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base)));
}

void hooks::CoreDebugMonitorNotify(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cc::DebugMonitorNotify(base, ctx.r3.u32, ctx.f1.f64, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCxxRestoreUnhandledExceptionFilter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CxxRestoreUnhandledExceptionFilter(base);
}

void hooks::CoreInvokeWatson(PPCContext& ctx, uint8_t* base) {
  cc::InvokeWatson(base, uint32_t(ctx.lr), CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreLockFhandle(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::LockFhandle(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

// The __finally funclets: r12 the parent's frame end (r31 = its frame), and
// the parent's registers they reload or keep, as the originals leave them.
void hooks::CoreLockFhandleFinally(PPCContext& ctx, uint8_t* base) {
  cc::LockFhandleFinally(CRuntimeCoreCalls(ctx, base));
  ctx.r11.u64 = cc::kPioinfo;
  ctx.r3.u64 = guest::LoadU32(base, ctx.r31.u32 + 148);
  ctx.r29.u64 = guest::LoadU32(base, ctx.r31.u32 + 80);
}

void hooks::CoreFreeOsfhnd(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = int32_t(cc::FreeOsfhnd(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base)));
}

void hooks::CoreGetOsfhandle(PPCContext& ctx, uint8_t* base) {
  const uint32_t handle = ctx.r3.u32;
  const uint32_t result = cc::GetOsfhandle(base, handle, CRuntimeCoreCalls(ctx, base));
  // The OS handle as loaded (lwz, zero-extended); -1 as `li` (sign-extended).
  const bool loaded = int32_t(handle) >= 0 && handle != 0xFFFFFFFEu &&
                      handle < guest::LoadU32(base, cc::kNhandle) &&
                      (guest::LoadU8(base, guest::LoadU32(base, cc::kPioinfo +
                                                                   4 * uint32_t(int32_t(handle) >> 5)) +
                                               44 * (handle & 31) + 4) &
                       1) != 0;
  ctx.r3.u64 = loaded ? uint64_t(result) : uint64_t(int64_t(int32_t(result)));
}

void hooks::CoreRaiseFinally(PPCContext& ctx, uint8_t* base) {
  bool called = false;
  const uint64_t r3 = cc::RaiseFinally(ctx.r28.u32, &called, CRuntimeCoreCalls(ctx, base));
  if (called) {
    ctx.r3.u64 = r3;
  }
}

void hooks::CoreUnlockStreamsFinally(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::UnlockStreamsFinally(CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFlsallInnerFinally(PPCContext& ctx, uint8_t* base) {
  const uint32_t index = ctx.r28.u32, piob_word = ctx.r29.u32;
  ctx.r4.u64 = guest::LoadU32(base, guest::LoadU32(base, piob_word) + 4 * index);
  ctx.r3.u64 = cc::FlsallInnerFinally(base, index, piob_word, CRuntimeCoreCalls(ctx, base));
  ctx.r11.u64 = 0x82980000u;
  ctx.r10.u64 = cc::kNstream;
  ctx.r27.u64 = guest::LoadU32(base, ctx.r12.u32 + 20);
}

void hooks::CoreExp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = cc::Exp(base, ctx.r1.u32, ctx.f1.f64);
}

void hooks::CoreForcdecpt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = int32_t(cc::Forcdecpt(base, ctx.r3.u32));
}

void hooks::CoreCropzeros(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Cropzeros(base, ctx.r3.u32);
}

void hooks::CoreAtodbl(PPCContext& ctx, uint8_t* base) {
  const auto r3 = cc::AtoNumber(base, ctx.r3.u32, ctx.r4.u32, true);
  if (!r3) {
    __imp__sub_825B6098(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void hooks::CoreAtoflt(PPCContext& ctx, uint8_t* base) {
  const auto r3 = cc::AtoNumber(base, ctx.r3.u32, ctx.r4.u32, false);
  if (!r3) {
    __imp__sub_825B6140(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void hooks::CoreFassign(PPCContext& ctx, uint8_t* base) {
  const auto r3 = cc::Fassign(base, ctx.r3.s32 != 0, ctx.r4.u32, ctx.r5.u32);
  if (!r3) {
    __imp__sub_825ADEB0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void hooks::CoreLd12ToFloat(PPCContext& ctx, uint8_t* base) {
  const uint32_t from = ctx.r3.u32;
  const c_runtime::LongDouble12 value{guest::LoadU16(base, from), guest::LoadU32(base, from + 2),
                                      guest::LoadU32(base, from + 6)};
  int32_t status = 0;
  guest::StoreU32(base, ctx.r4.u32,
                  c_runtime::Ld12ToFloat(value, guest::LoadU16(base, from + 10), &status));
  ctx.r3.s64 = status;
}

void hooks::CoreFileno(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Fileno(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

// The __finally funclets over a file handle's lock: the handle in r30
// (_close's, _write's) or r27 (_commit's).
void hooks::CoreCloseFinally(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::UnlockFhandleFinally(base, ctx.r30.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCommitFinally(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::UnlockFhandleFinally(base, ctx.r27.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFcloseFinally(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::UnlockFileFinally(base, ctx.r30.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreExitUnlockFinally(PPCContext& ctx, uint8_t* base) {
  bool called = false;
  const uint64_t r3 = cc::ExitUnlockFinally(ctx.r28.u32, &called, CRuntimeCoreCalls(ctx, base));
  if (called) {
    ctx.r3.u64 = r3;
  }
}

void hooks::CoreTerminate(PPCContext& ctx, uint8_t* base) {
  cc::Terminate(base, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCloseNolock(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = int32_t(cc::CloseNolock(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base)));
}

void hooks::CoreClose(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Close(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFlushFileBuffers(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::FlushFileBuffers(ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCommit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Commit(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreSetFilePointer(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::SetFilePointer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                  CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreLseekNolock(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::LseekNolock(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreLseek(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Lseek(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreWriteFile(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::WriteFile(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                             CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreWriteNolock(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::WriteNolock(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreWrite(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Write(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFlush(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Flush(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreCxxUnhandledExceptionFilter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CxxUnhandledExceptionFilter(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}

void hooks::CoreFcloseNolock(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::FcloseNolock(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFclose(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Fclose(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFcloseall(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Fcloseall(base, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFlsbuf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Flsbuf(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreStbuf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Stbuf(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFtbuf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Ftbuf(base, ctx.r3.u32, ctx.r4.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreBsearch(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Bsearch(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u32, ctx.r6.u64, ctx.r7.u32,
                           CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreCftoe2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Cftoe2(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.u32, ctx.r7.u32,
                          int8_t(ctx.r8.u8), CRuntimeCoreCalls(ctx, base));
}
// _fassign, _forcdecpt and _cropzeros: the _l functions with no locale (r6
// or r4 = 0), as the originals' tail calls.
void hooks::CoreFassignNoLocale(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825ADF10 _fassign");
  ctx.r6.u64 = 0;
  CoreFassign(ctx, base);
}
void hooks::CoreForcdecptNoLocale(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825ADF18 _forcdecpt");
  ctx.r4.u64 = 0;
  CoreForcdecpt(ctx, base);
}
void hooks::CoreCropzerosNoLocale(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825ADF20 _cropzeros");
  ctx.r4.u64 = 0;
  CoreCropzeros(ctx, base);
}
void hooks::CoreCxxSetUnhandledExceptionFilter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::CxxSetUnhandledExceptionFilter(base, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFflushNolock(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::FflushNolock(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFlsall(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Flsall(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFflush(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Fflush(base, ctx.r3.u32, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreFlushall(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Flushall(base, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreEndstdio(PPCContext& ctx, uint8_t* base) {
  cc::Endstdio(base, CRuntimeCoreCalls(ctx, base));
}
void hooks::CoreDoexit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cc::Doexit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, CRuntimeCoreCalls(ctx, base));
}
// _exit and _cexit: doexit with (code, 1, 0) and (0, 0, 1), as the
// originals' tail calls.
void hooks::CoreExit(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825AA378 _exit");
  ctx.r5.u64 = 0;
  ctx.r4.u64 = 1;
  CoreDoexit(ctx, base);
}
void hooks::CoreCexit(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825AA388 _cexit");
  ctx.r5.u64 = 1;
  ctx.r4.u64 = 0;
  ctx.r3.u64 = 0;
  CoreDoexit(ctx, base);
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
// _getbuf: r3 = a stream. (_freebuf, sub_825B0EE0, has no adapter: its only
// caller, _fclose_nolock, is native.)
ITC_C_RUNTIME_CORE(825B4790, CoreGetbuf)
// ResetEvent: r3 = an event; r3 = 1 or 0. (CloseHandle, sub_82294E20, has no
// adapter: its caller in play, SaveSlot::WaitClose, calls the plain C++.)
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
// The initial _aexit_rtn: r3 ignored; KeBugCheck(0).
ITC_C_RUNTIME_CORE(825AA1A0, CoreBugCheckStub)
// RaiseException: r3 = code, r4 = flags, r5 = parameter count, r6 = the
// parameters.
ITC_C_RUNTIME_CORE(825BA420, CoreRaiseException)
// _CxxThrowException: r3 = the object, r4 = its throw info.
ITC_C_RUNTIME_CORE(825AD198, CoreCxxThrowException)
// _invalid_parameter: r3-r7 handed to the handler.
ITC_C_RUNTIME_CORE(825AFCC8, CoreInvalidParameter)
// XapiThreadStartup: r3 = the start routine, r4 = its parameter; never
// returns.
ITC_C_RUNTIME_CORE(825BA558, CoreThreadStartup)
// _amsg_exit: r3 = a run-time error number.
ITC_C_RUNTIME_CORE(825AA158, CoreAmsgExit)
// _getptd: r3 = the calling thread's data.
ITC_C_RUNTIME_CORE(825AA580, CoreGetptd)
// _fptrap: no registers.
ITC_C_RUNTIME_CORE(825B64F8, CoreFptrap)
// UnhandledExceptionFilter: r3 = the exception pointers; r3 = -1 or 0.
ITC_C_RUNTIME_CORE(82297940, CoreUnhandledExceptionFilter)
// The debug monitor notification: r3 = a code, f1 = a value.
ITC_C_RUNTIME_CORE(825B9BA8, CoreDebugMonitorNotify)
// __CxxRestoreUnhandledExceptionFilter: r3 = the filter it replaced.
ITC_C_RUNTIME_CORE(825B4D90, CoreCxxRestoreUnhandledExceptionFilter)
// _invoke_watson: no registers; ends in KeBugCheck(30).
ITC_C_RUNTIME_CORE(825AFD10, CoreInvokeWatson)
// __lock_fhandle: r3 = a file handle; r3 = 1, or 0.
ITC_C_RUNTIME_CORE(825B7958, CoreLockFhandle)
// __lock_fhandle's __finally: r31 = its frame; r3, r29 and r11 reloaded.
ITC_C_RUNTIME_CORE(825B7A20, CoreLockFhandleFinally)
// _free_osfhnd, _get_osfhandle: r3 = a file handle.
ITC_C_RUNTIME_CORE(825B77E8, CoreFreeOsfhnd)
ITC_C_RUNTIME_CORE(825B7880, CoreGetOsfhandle)
// raise's __finally: r28 = whether raise took lock 0.
ITC_C_RUNTIME_CORE(825B4C8C, CoreRaiseFinally)
// (flsall's outer and inner __finally, sub_825ACEB4 and sub_825ACF04, and
// _fcloseall's, sub_825B4F38, have no adapters: flsall and _fcloseall are
// native.)
// exp: f1 = x; f1 = e^x.
ITC_C_RUNTIME_CORE(825ABE30, CoreExp)
// _forcdecpt_l, _cropzeros_l: r3 = a number's text.
ITC_C_RUNTIME_CORE(825ADD38, CoreForcdecpt)
ITC_C_RUNTIME_CORE(825ADDD8, CoreCropzeros)
// _fassign_l: r3 = 1 for a double, r4 = out, r5 = a string; r3 = 0, 3 or 4.
ITC_C_RUNTIME_CORE(825ADEB0, CoreFassign)
// _atodbl_l, _atoflt_l: r3 = out, r4 = a string; r3 = 0, 3 or 4.
ITC_C_RUNTIME_CORE(825B6098, CoreAtodbl)
ITC_C_RUNTIME_CORE(825B6140, CoreAtoflt)
// _ld12tof: r3 = a 12-byte long double, r4 = the float out; r3 = 0, 1 or 2.
ITC_C_RUNTIME_CORE(825B70D0, CoreLd12ToFloat)
// _fileno: r3 = a stream; r3 = its handle, or -1.
ITC_C_RUNTIME_CORE(825B0E88, CoreFileno)
// _close's and _write's __finally: r30 = the handle; _commit's: r27.
ITC_C_RUNTIME_CORE(825B0E50, CoreCloseFinally)
ITC_C_RUNTIME_CORE(825B1304, CoreCloseFinally)
ITC_C_RUNTIME_CORE(825B4978, CoreCommitFinally)
// (fclose's __finally, sub_825A9664, has no adapter: fclose is native.)
// doexit's unwind funclet: r28 = whether doexit took the exit lock.
ITC_C_RUNTIME_CORE(825AA338, CoreExitUnlockFinally)
// terminate: no registers; ends in KeBugCheck(0xC0000144).
ITC_C_RUNTIME_CORE(825B3498, CoreTerminate)
// (_close, sub_825B0D10, and _close_nolock, sub_825B0C28, have no adapters:
// _fclose_nolock, _close's only caller, is native, and _close is
// _close_nolock's.)
// FlushFileBuffers: r3 = an OS handle; r3 = 1, or 0.
ITC_C_RUNTIME_CORE(825BA510, CoreFlushFileBuffers)
// _commit: r3 = a file handle; r3 = 0, or -1.
ITC_C_RUNTIME_CORE(825B4828, CoreCommit)
// _lseeki64_nolock, _lseeki64: r3 = a file handle, r4 = a 64-bit offset, r5 =
// the origin; r3 = the 64-bit position, or -1. (SetFilePointer, sub_825B9DD8,
// has no adapter: its caller in play, File::Seek, leaves it out, and
// _lseeki64_nolock calls the plain C++.)
ITC_C_RUNTIME_CORE(825B6500, CoreLseekNolock)
ITC_C_RUNTIME_CORE(825B65D0, CoreLseek)
// _lseeki64's __finally: r30 = the handle.
ITC_C_RUNTIME_CORE(825B6730, CoreCloseFinally)
// _write: r3 = a file handle, r4 = the bytes, r5 = their count; r3 = the
// count written, or -1. (_write_nolock, sub_825B0F48, and WriteFile,
// sub_82295D50, have no adapters: _write is _write_nolock's only caller, and
// WriteFile's caller in play, SaveSlot::WriteFile, leaves it out.)
ITC_C_RUNTIME_CORE(825B11A8, CoreWrite)
// fputs's __finally: r30 = the stream. (fflush's, sub_825AD02C, has no
// adapter: fflush is native.)
ITC_C_RUNTIME_CORE(825AD488, CoreFcloseFinally)
// _flush: r3 = a stream; r3 = 0, or -1.
ITC_C_RUNTIME_CORE(825ACC78, CoreFlush)
// __CxxUnhandledExceptionFilter: r3 = the exception pointers; r3 = the
// previous filter's answer, or 0.
ITC_C_RUNTIME_CORE(825B4CD0, CoreCxxUnhandledExceptionFilter)
// _fcloseall: r3 = the streams closed. (fclose, sub_825A9580, and
// _fclose_nolock, sub_825A94C0, have no adapters: _fcloseall is fclose's
// only caller, fclose _fclose_nolock's.)
ITC_C_RUNTIME_CORE(825B4E78, CoreFcloseall)
// _flsbuf: r3 = a character, r4 = a stream; r3 = the character's low byte,
// or -1.
ITC_C_RUNTIME_CORE(825AEE28, CoreFlsbuf)
// _stbuf: r3 = a stream; r3 = 0 (1 past the _isatty stub, never).
ITC_C_RUNTIME_CORE(825B40B8, CoreStbuf)
// _ftbuf: r3 = a flag, r4 = a stream.
ITC_C_RUNTIME_CORE(825B41A0, CoreFtbuf)
// bsearch: r3 = the key, r4 = the first element, r5 = the count, r6 = the
// width, r7 = the compare function; r3 = the element found, or 0.
ITC_C_RUNTIME_CORE(825AC970, CoreBsearch)
// _cftoe2_l: r3 = the buffer, r4 = its size, r5 = the precision, r6 = caps,
// r7 = _fltout2's result, r8 = the %g flag (a byte); r3 = 0, 22 or 34.
ITC_C_RUNTIME_CORE(825ADF28, CoreCftoe2)
// _fassign: r3 = whether a double, r4 = the output, r5 = the text.
ITC_C_RUNTIME_CORE(825ADF10, CoreFassignNoLocale)
// _forcdecpt, _cropzeros: r3 = the text.
ITC_C_RUNTIME_CORE(825ADF18, CoreForcdecptNoLocale)
ITC_C_RUNTIME_CORE(825ADF20, CoreCropzerosNoLocale)
// __CxxSetUnhandledExceptionFilter: no registers; r3 = the old filter.
ITC_C_RUNTIME_CORE(825DAC58, CoreCxxSetUnhandledExceptionFilter)
// fflush: r3 = a stream (0 for all); r3 = 0, or -1 (as a word). (_fflush_nolock
// sub_825ACD10, flsall sub_825ACD90 and _flushall sub_825AD068 have no
// adapters: their callers, fflush, _fflush_nolock, flsall and _endstdio, are
// native.)
ITC_C_RUNTIME_CORE(825ACF70, CoreFflush)
// _endstdio: no registers (an exit table entry).
ITC_C_RUNTIME_CORE(825AD5F8, CoreEndstdio)
// _exit: r3 = the exit code; ends the process. _cexit: no registers. (doexit,
// sub_825AA218, has no adapter: they are its only callers.)
ITC_C_RUNTIME_CORE(825AA378, CoreExit)
ITC_C_RUNTIME_CORE(825AA388, CoreCexit)
