// The C runtime core's adapters (hooks/c_runtime_core.cpp): the register
// steps, without the flag, shared with the harness.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include "c_runtime/core.h"

namespace hooks {

// The kernel imports, guest calls, the last error and the heap for the C
// runtime core's plain C++ (c_runtime::core::Calls), through `ctx` (r1 left
// where it is: the calls are made below the adapter's caller's frame).
c_runtime::core::Calls CRuntimeCoreCalls(lifted::Regs& ctx, uint8_t* base);

// The adapters' steps (r3 in and out as the originals').
void CoreCrtTlsAlloc(lifted::Regs& ctx, uint8_t* base);   // sub_825AA3F8
void CoreGetptdNoexit(lifted::Regs& ctx, uint8_t* base);  // sub_825AA4C0
void CoreFreeFls(lifted::Regs& ctx, uint8_t* base);       // sub_825AA5C0
void CoreMtTerm(lifted::Regs& ctx, uint8_t* base);        // sub_825AA448
void CoreSetFlsGetValue(lifted::Regs& ctx, uint8_t* base);  // sub_825AA400 (no adapter)
void CoreMtDeleteLocks(lifted::Regs& ctx, uint8_t* base);   // sub_825B3260 (no adapter)

void CoreErrno(lifted::Regs& ctx, uint8_t* base);              // sub_825A81E0
void CoreDosErrno(lifted::Regs& ctx, uint8_t* base);           // sub_825A8218
void CoreGetErrnoFromOsErr(lifted::Regs& ctx, uint8_t* base);  // sub_825A8178
void CoreDosMapErr(lifted::Regs& ctx, uint8_t* base);          // sub_825A8250
void CoreFreePtd(lifted::Regs& ctx, uint8_t* base);            // sub_825AA670
void CoreCritSecNoSpinCount(lifted::Regs& ctx, uint8_t* base);       // sub_825B3570
void CoreInitCritSecAndSpinCount(lifted::Regs& ctx, uint8_t* base);  // sub_825B35A0
void CoreRegisterThreadNotifyRoutine(lifted::Regs& ctx, uint8_t* base);  // sub_82297B98
void CoreCallThreadNotifyRoutines(lifted::Regs& ctx, uint8_t* base);     // sub_82297B38
void CoreMtInitLocks(lifted::Regs& ctx, uint8_t* base);   // sub_825B31D8
void CoreThreadNotify(lifted::Regs& ctx, uint8_t* base);  // sub_825B3650
void CoreMtInit(lifted::Regs& ctx, uint8_t* base);        // sub_825AA708
void CoreAtol(lifted::Regs& ctx, uint8_t* base);                // sub_825AAF08
void CoreStrtol(lifted::Regs& ctx, uint8_t* base);              // sub_825A85C0 (no adapter)
void CoreStrrchr(lifted::Regs& ctx, uint8_t* base);             // sub_825A8630
void CoreInitterm(lifted::Regs& ctx, uint8_t* base);            // sub_825AA1B8
void CoreSetAbortBehavior(lifted::Regs& ctx, uint8_t* base);    // sub_825AD120
void CoreGetSigabrt(lifted::Regs& ctx, uint8_t* base);          // sub_825B49B0
void CoreCrtDebuggerHook(lifted::Regs& ctx, uint8_t* base);     // sub_825B68B8
void CoreValidateExecute(lifted::Regs& ctx, uint8_t* base);     // sub_825B8910
void CoreSetUnhandledExceptionFilter(lifted::Regs& ctx, uint8_t* base);  // sub_82297870
void CoreGetCommandLineA(lifted::Regs& ctx, uint8_t* base);     // sub_822979D0
void CorePositive(lifted::Regs& ctx, uint8_t* base);            // sub_825ADE90
void CoreNmsgWrite(lifted::Regs& ctx, uint8_t* base);           // sub_825AEDA0
void CoreGetbuf(lifted::Regs& ctx, uint8_t* base);              // sub_825B4790
void CoreFreebuf(lifted::Regs& ctx, uint8_t* base);             // sub_825B0EE0
void CoreBugCheckStub(lifted::Regs& ctx, uint8_t* base);        // sub_825AA1A0
void CoreRaiseException(lifted::Regs& ctx, uint8_t* base);      // sub_825BA420
void CoreCxxThrowException(lifted::Regs& ctx, uint8_t* base);   // sub_825AD198
void CoreInvalidParameter(lifted::Regs& ctx, uint8_t* base);    // sub_825AFCC8
void CoreThreadStartup(lifted::Regs& ctx, uint8_t* base);       // sub_825BA558
void CoreAmsgExit(lifted::Regs& ctx, uint8_t* base);            // sub_825AA158
void CoreGetptd(lifted::Regs& ctx, uint8_t* base);              // sub_825AA580
void CoreFptrap(lifted::Regs& ctx, uint8_t* base);              // sub_825B64F8
void CoreUnhandledExceptionFilter(lifted::Regs& ctx, uint8_t* base);            // sub_82297940
void CoreDebugMonitorNotify(lifted::Regs& ctx, uint8_t* base);                  // sub_825B9BA8
void CoreCxxRestoreUnhandledExceptionFilter(lifted::Regs& ctx, uint8_t* base);  // sub_825B4D90
void CoreInvokeWatson(lifted::Regs& ctx, uint8_t* base);                        // sub_825AFD10
void CoreLockFhandle(lifted::Regs& ctx, uint8_t* base);           // sub_825B7958
void CoreFreeOsfhnd(lifted::Regs& ctx, uint8_t* base);            // sub_825B77E8
void CoreGetOsfhandle(lifted::Regs& ctx, uint8_t* base);          // sub_825B7880
void CoreUnlockStreamsFinally(lifted::Regs& ctx, uint8_t* base);  // sub_825B4F38, sub_825ACEB4
void CoreExp(lifted::Regs& ctx, uint8_t* base);                   // sub_825ABE30
void CoreForcdecpt(lifted::Regs& ctx, uint8_t* base);             // sub_825ADD38
void CoreCropzeros(lifted::Regs& ctx, uint8_t* base);             // sub_825ADDD8
void CoreFassign(lifted::Regs& ctx, uint8_t* base);               // sub_825ADEB0
void CoreAtodbl(lifted::Regs& ctx, uint8_t* base);                // sub_825B6098
void CoreAtoflt(lifted::Regs& ctx, uint8_t* base);                // sub_825B6140
void CoreLd12ToFloat(lifted::Regs& ctx, uint8_t* base);           // sub_825B70D0
void CoreCloseHandle(lifted::Regs& ctx, uint8_t* base);         // sub_82294E20
void CoreResetEvent(lifted::Regs& ctx, uint8_t* base);          // sub_825BA168
void CoreInittermE(lifted::Regs& ctx, uint8_t* base);    // sub_82297AC0
void CoreIoInit(lifted::Regs& ctx, uint8_t* base);       // sub_825B4668
void CoreInitStdio(lifted::Regs& ctx, uint8_t* base);    // sub_825AD4D0
void CoreFfMsgBanner(lifted::Regs& ctx, uint8_t* base);  // sub_825AEDE8
void CoreLockFile(lifted::Regs& ctx, uint8_t* base);     // sub_825AD638
void CoreUnlockFile(lifted::Regs& ctx, uint8_t* base);   // sub_825AD688
void CoreLockFile2(lifted::Regs& ctx, uint8_t* base);    // sub_825AD670
void CoreUnlockFile2(lifted::Regs& ctx, uint8_t* base);  // sub_825AD6C0
void CoreUnlockFhandle(lifted::Regs& ctx, uint8_t* base);  // sub_825B7A58

void CoreFileno(lifted::Regs& ctx, uint8_t* base);             // sub_825B0E88
void CoreTerminate(lifted::Regs& ctx, uint8_t* base);          // sub_825B3498

void CoreCloseNolock(lifted::Regs& ctx, uint8_t* base);       // sub_825B0C28 (no adapter)
void CoreClose(lifted::Regs& ctx, uint8_t* base);             // sub_825B0D10
void CoreFlushFileBuffers(lifted::Regs& ctx, uint8_t* base);  // sub_825BA510
void CoreCommit(lifted::Regs& ctx, uint8_t* base);            // sub_825B4828

void CoreSetFilePointer(lifted::Regs& ctx, uint8_t* base);  // sub_825B9DD8 (no adapter)
void CoreLseekNolock(lifted::Regs& ctx, uint8_t* base);     // sub_825B6500
void CoreLseek(lifted::Regs& ctx, uint8_t* base);           // sub_825B65D0

void CoreWriteFile(lifted::Regs& ctx, uint8_t* base);    // sub_82295D50 (no adapter)
void CoreWriteNolock(lifted::Regs& ctx, uint8_t* base);  // sub_825B0F48 (no adapter)
void CoreWrite(lifted::Regs& ctx, uint8_t* base);        // sub_825B11A8

void CoreFlush(lifted::Regs& ctx, uint8_t* base);                        // sub_825ACC78
void CoreCxxUnhandledExceptionFilter(lifted::Regs& ctx, uint8_t* base);  // sub_825B4CD0

void CoreFcloseNolock(lifted::Regs& ctx, uint8_t* base);  // sub_825A94C0 (no adapter)
void CoreFclose(lifted::Regs& ctx, uint8_t* base);        // sub_825A9580 (no adapter)
void CoreFcloseall(lifted::Regs& ctx, uint8_t* base);     // sub_825B4E78

void CoreFlsbuf(lifted::Regs& ctx, uint8_t* base);  // sub_825AEE28
void CoreStbuf(lifted::Regs& ctx, uint8_t* base);   // sub_825B40B8
void CoreFtbuf(lifted::Regs& ctx, uint8_t* base);   // sub_825B41A0

void CoreBsearch(lifted::Regs& ctx, uint8_t* base);    // sub_825AC970
void CoreCftoe2(lifted::Regs& ctx, uint8_t* base);     // sub_825ADF28
void CoreFassignNoLocale(lifted::Regs& ctx, uint8_t* base);    // sub_825ADF10
void CoreForcdecptNoLocale(lifted::Regs& ctx, uint8_t* base);  // sub_825ADF18
void CoreCropzerosNoLocale(lifted::Regs& ctx, uint8_t* base);  // sub_825ADF20
void CoreCxxSetUnhandledExceptionFilter(lifted::Regs& ctx, uint8_t* base);  // sub_825DAC58

void CoreFflushNolock(lifted::Regs& ctx, uint8_t* base);  // sub_825ACD10 (no adapter)
void CoreFlsall(lifted::Regs& ctx, uint8_t* base);        // sub_825ACD90 (no adapter)
void CoreFflush(lifted::Regs& ctx, uint8_t* base);        // sub_825ACF70
void CoreFlushall(lifted::Regs& ctx, uint8_t* base);      // sub_825AD068 (no adapter)
void CoreEndstdio(lifted::Regs& ctx, uint8_t* base);      // sub_825AD5F8

void CoreDoexit(lifted::Regs& ctx, uint8_t* base);  // sub_825AA218 (no adapter)
void CoreExit(lifted::Regs& ctx, uint8_t* base);    // sub_825AA378
void CoreCexit(lifted::Regs& ctx, uint8_t* base);   // sub_825AA388

void CoreRaise(lifted::Regs& ctx, uint8_t* base);     // sub_825B49C8 (no adapter)
void CoreAbort(lifted::Regs& ctx, uint8_t* base);     // sub_825AD070
void CorePurecall(lifted::Regs& ctx, uint8_t* base);  // sub_825A7098

void CoreFwriteNolock(lifted::Regs& ctx, uint8_t* base);  // sub_825A96A0 (no adapter)
void CoreFputs(lifted::Regs& ctx, uint8_t* base);         // sub_825AD280
void CoreThrow(lifted::Regs& ctx, uint8_t* base);         // sub_825A69D8
void CoreVsprintf(lifted::Regs& ctx, uint8_t* base);      // sub_825A71B0
void CoreCftoe(lifted::Regs& ctx, uint8_t* base);         // sub_825AE1A8
void CoreCftog(lifted::Regs& ctx, uint8_t* base);         // sub_825AE9D0

}  // namespace hooks
