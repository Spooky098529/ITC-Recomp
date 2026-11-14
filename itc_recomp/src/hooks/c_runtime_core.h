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

}  // namespace hooks
