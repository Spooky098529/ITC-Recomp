// The C runtime core's adapters (hooks/c_runtime_core.cpp): the register
// steps, without the flag, shared with the harness.

#pragma once

#include "guest.h"

#include "c_runtime/core.h"

namespace hooks {

// The kernel imports, guest calls, the last error and the heap for the C
// runtime core's plain C++ (c_runtime::core::Calls), through `ctx` (r1 left
// where it is: the calls are made below the adapter's caller's frame).
c_runtime::core::Calls CRuntimeCoreCalls(PPCContext& ctx, uint8_t* base);

// The adapters' steps (r3 in and out as the originals').
void CoreCrtTlsAlloc(PPCContext& ctx, uint8_t* base);   // sub_825AA3F8
void CoreGetptdNoexit(PPCContext& ctx, uint8_t* base);  // sub_825AA4C0
void CoreFreeFls(PPCContext& ctx, uint8_t* base);       // sub_825AA5C0
void CoreMtTerm(PPCContext& ctx, uint8_t* base);        // sub_825AA448
void CoreSetFlsGetValue(PPCContext& ctx, uint8_t* base);  // sub_825AA400 (no adapter)
void CoreMtDeleteLocks(PPCContext& ctx, uint8_t* base);   // sub_825B3260 (no adapter)

void CoreErrno(PPCContext& ctx, uint8_t* base);              // sub_825A81E0
void CoreDosErrno(PPCContext& ctx, uint8_t* base);           // sub_825A8218
void CoreGetErrnoFromOsErr(PPCContext& ctx, uint8_t* base);  // sub_825A8178
void CoreDosMapErr(PPCContext& ctx, uint8_t* base);          // sub_825A8250
void CoreFreePtd(PPCContext& ctx, uint8_t* base);            // sub_825AA670
void CoreCritSecNoSpinCount(PPCContext& ctx, uint8_t* base);       // sub_825B3570
void CoreInitCritSecAndSpinCount(PPCContext& ctx, uint8_t* base);  // sub_825B35A0
void CoreRegisterThreadNotifyRoutine(PPCContext& ctx, uint8_t* base);  // sub_82297B98
void CoreCallThreadNotifyRoutines(PPCContext& ctx, uint8_t* base);     // sub_82297B38
void CoreMtInitLocks(PPCContext& ctx, uint8_t* base);   // sub_825B31D8
void CoreThreadNotify(PPCContext& ctx, uint8_t* base);  // sub_825B3650
void CoreMtInit(PPCContext& ctx, uint8_t* base);        // sub_825AA708
void CoreAtol(PPCContext& ctx, uint8_t* base);                // sub_825AAF08
void CoreStrtol(PPCContext& ctx, uint8_t* base);              // sub_825A85C0 (no adapter)
void CoreStrrchr(PPCContext& ctx, uint8_t* base);             // sub_825A8630
void CoreInitterm(PPCContext& ctx, uint8_t* base);            // sub_825AA1B8
void CoreSetAbortBehavior(PPCContext& ctx, uint8_t* base);    // sub_825AD120
void CoreGetSigabrt(PPCContext& ctx, uint8_t* base);          // sub_825B49B0
void CoreCrtDebuggerHook(PPCContext& ctx, uint8_t* base);     // sub_825B68B8
void CoreValidateExecute(PPCContext& ctx, uint8_t* base);     // sub_825B8910
void CoreSetUnhandledExceptionFilter(PPCContext& ctx, uint8_t* base);  // sub_82297870
void CoreGetCommandLineA(PPCContext& ctx, uint8_t* base);     // sub_822979D0
void CorePositive(PPCContext& ctx, uint8_t* base);            // sub_825ADE90
void CoreNmsgWrite(PPCContext& ctx, uint8_t* base);           // sub_825AEDA0
void CoreGetbuf(PPCContext& ctx, uint8_t* base);              // sub_825B4790
void CoreFreebuf(PPCContext& ctx, uint8_t* base);             // sub_825B0EE0
void CoreBugCheckStub(PPCContext& ctx, uint8_t* base);        // sub_825AA1A0
void CoreRaiseException(PPCContext& ctx, uint8_t* base);      // sub_825BA420
void CoreCxxThrowException(PPCContext& ctx, uint8_t* base);   // sub_825AD198
void CoreInvalidParameter(PPCContext& ctx, uint8_t* base);    // sub_825AFCC8
void CoreThreadStartup(PPCContext& ctx, uint8_t* base);       // sub_825BA558
void CoreAmsgExit(PPCContext& ctx, uint8_t* base);            // sub_825AA158
void CoreGetptd(PPCContext& ctx, uint8_t* base);              // sub_825AA580
void CoreFptrap(PPCContext& ctx, uint8_t* base);              // sub_825B64F8
void CoreCloseHandle(PPCContext& ctx, uint8_t* base);         // sub_82294E20
void CoreResetEvent(PPCContext& ctx, uint8_t* base);          // sub_825BA168
void CoreInittermE(PPCContext& ctx, uint8_t* base);    // sub_82297AC0
void CoreIoInit(PPCContext& ctx, uint8_t* base);       // sub_825B4668
void CoreInitStdio(PPCContext& ctx, uint8_t* base);    // sub_825AD4D0
void CoreFfMsgBanner(PPCContext& ctx, uint8_t* base);  // sub_825AEDE8
void CoreLockFile(PPCContext& ctx, uint8_t* base);     // sub_825AD638
void CoreUnlockFile(PPCContext& ctx, uint8_t* base);   // sub_825AD688
void CoreLockFile2(PPCContext& ctx, uint8_t* base);    // sub_825AD670
void CoreUnlockFile2(PPCContext& ctx, uint8_t* base);  // sub_825AD6C0
void CoreUnlockFhandle(PPCContext& ctx, uint8_t* base);  // sub_825B7A58

}  // namespace hooks
