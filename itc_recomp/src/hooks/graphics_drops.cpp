// The graphics library's functions whose work isn't needed on PC, dropped
// (status Dropped in docs/functions.md): they only made kernel video calls
// (Vd*) the SDK stubs, or wrote packets for the 360's display hardware,
// which our renderer doesn't read. See B3 in docs/rexglue-removal.md.

#include "guest.h"

// RetrainEdram, once a frame from Swap (native Device::Init no longer calls
// it): with
// the SDK's VdRetrainEDRAM returning 0 it only took and released a lock.
// Its callers ignore the return.
extern "C" REX_FUNC(sub_822B2388) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 1;
}

// Device::SetUpScaler, from Swap when the scaler changes: the 360's output
// scaler, its packets display registers our renderer doesn't read (and its
// command buffer NOPs from the SDK). Swap ignores the return.
extern "C" REX_FUNC(sub_822B3FE0) {
  REX_FUNC_PROLOGUE();
}

// The scaler's notification (VdCallGraphicsNotificationRoutines, which the
// SDK answers with 0); its caller sub_822B4250 returns what it returns.
extern "C" REX_FUNC(sub_822B2610) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 0;
}
