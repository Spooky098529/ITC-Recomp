// The C runtime's core natively (docs/functions/c-runtime-core.md): adapters
// in front of src/c_runtime/core.cpp, the kernel imports called as the
// originals call them (B7's goal, the user's call, 2026-11-14).

#include "hooks/c_runtime_core.h"

#include "base/flags.h"
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
              "XapiCallThreadNotifyRoutines (sub_82297B98, sub_82297B38) (checked by --run_harnesses=c_runtime_core). "
              "Those reached only from native callers have no adapters, and the flag switches "
              "their callers: __set_flsgetvalue, _mtdeletelocks, _freefls, _errno");

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
// _mtterm: no registers.
ITC_C_RUNTIME_CORE(825AA448, CoreMtTerm)
// __doserrno: r3 = the address. (_errno, sub_825A81E0, and _freefls,
// sub_825AA5C0, have no adapters: their callers in play are native.)
ITC_C_RUNTIME_CORE(825A8218, CoreDosErrno)
// _get_errno_from_oserr: r3 = a Windows error; r3 = its errno.
ITC_C_RUNTIME_CORE(825A8178, CoreGetErrnoFromOsErr)
// _dosmaperr: r3 = a Windows error.
ITC_C_RUNTIME_CORE(825A8250, CoreDosMapErr)
// _freeptd: r3 = a thread's data, or 0 for the calling thread's.
ITC_C_RUNTIME_CORE(825AA670, CoreFreePtd)
// __crtInitCritSecNoSpinCount: r3 = a critical section; r3 = 1.
ITC_C_RUNTIME_CORE(825B3570, CoreCritSecNoSpinCount)
// __crtInitCritSecAndSpinCount: r3 = a critical section, r4 = a spin count.
ITC_C_RUNTIME_CORE(825B35A0, CoreInitCritSecAndSpinCount)
// XRegisterThreadNotifyRoutine: r3 = a notify block, r4 = 1 to add, 0 to remove.
ITC_C_RUNTIME_CORE(82297B98, CoreRegisterThreadNotifyRoutine)
// XapiCallThreadNotifyRoutines: r3 = 1 at a thread's start, 0 at its end.
ITC_C_RUNTIME_CORE(82297B38, CoreCallThreadNotifyRoutines)
