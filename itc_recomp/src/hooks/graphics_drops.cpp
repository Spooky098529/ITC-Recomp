// The graphics library's functions whose work isn't needed on PC, dropped
// (status Dropped in docs/functions/functions.md): they only made kernel video calls
// (Vd*) the SDK stubs, or wrote packets for the 360's display hardware,
// which our renderer doesn't read. See B3 in docs/rexglue-removal.md.

#include "guest.h"
#include "hooks/native_entry.h"
#include "lifted/sdk.h"

// RetrainEdram, once a frame from Swap (native Device::Init no longer calls
// it): with the SDK's VdRetrainEDRAM returning 0 it only took and released
// a lock. Swap ignores the return.
ITC_NATIVE_ENTRY_ALWAYS(822B2388) {
  
  ctx.r3.u64 = 1;
}

// Device::SetUpScaler, from Swap when the scaler changes: the 360's output
// scaler, its packets display registers our renderer doesn't read (and its
// command buffer NOPs from the SDK). Swap ignores the return.
ITC_NATIVE_ENTRY_ALWAYS(822B3FE0) {
  
}

// The scaler's notification (VdCallGraphicsNotificationRoutines, which the
// SDK answers with 0); its caller sub_822B4250 returns what it returns.
ITC_NATIVE_ENTRY_ALWAYS(822B2610) {
  
  ctx.r3.u64 = 0;
}

// The shader build's wrapper (14 calls at boot, from sub_822B8D28): on its
// first call it asked GetFileAttributes for "ShaderDumpxe:\CompareBackEnds"
// and, had the file been there, set 82953E28 to build each shader a second
// way and compare. The file never is, and nothing else reads either byte, so
// this is the build (sub_822B8230) with the arguments as they came. B4 in
// docs/rexglue-removal.md.
ITC_NATIVE_ENTRY_ALWAYS(822B8868) {
  
  lifted::CallFromContext(ctx, 0x822B8230);
}
