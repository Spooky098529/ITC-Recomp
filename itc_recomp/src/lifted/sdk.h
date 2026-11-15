// The lift sweep's boundary with the SDK (docs/loops/lift-sweep.md, "The
// pieces", 3): the only lifted code that touches the SDK's context.
//
// Import runs an SDK import (a kernel or XAM function) with a call's
// arguments. Enter runs a lifted function's entry for the SDK, or for
// code that isn't native yet, from the SDK's context; while it runs, the
// thread's context is the one calls through the boundary use.

#pragma once

#include "lifted/lifted.h"

namespace lifted {

// An SDK import, as the generated code declares it.
using ImportFunction = void(PPCContext&, uint8_t*);

Result Import(uint8_t* base, ImportFunction* function, const Args& args);

// Runs `entry` with the context's argument registers and puts its r3 and
// f1 back.
void Enter(PPCContext& ctx, uint8_t* base, Entry entry);

}  // namespace lifted
