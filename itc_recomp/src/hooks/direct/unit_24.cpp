// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 24's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "guest.h"
#include "hooks/guest_call.h"
#include "lifted/originals.h"

#include "action_mappers/action_mappers.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "graphics/command_buffer.h"
#include "graphics/device_state.h"
#include "hooks/c_runtime_core.h"
#include "hooks/command_buffer.h"
#include "hooks/device_state_calls.h"
#include "race_states/race_states.h"

// --- The C runtime's core (their steps in src/hooks/c_runtime_core.cpp) ---

// CloseHandle: r3 = the handle; r3 1 or 0.
ITC_DIRECT_ENTRY(82294E20) { hooks::CoreCloseHandle(ctx); }
// WriteFile: r3 = the handle, r4 = the buffer, r5 = its size, r6 = the bytes
// written out, r7 = the overlapped block; r3 1 or 0.
ITC_DIRECT_ENTRY(82295D50) { hooks::CoreWriteFile(ctx); }
// _errno: r3 = the calling thread's errno's address.
ITC_DIRECT_ENTRY(825A81E0) { hooks::CoreErrno(ctx); }
// SetFilePointer: r3 = the handle, r4 = the low word, r5 = the high word's
// address, r6 = the method; r3 the new low word.
ITC_DIRECT_ENTRY(825B9DD8) { hooks::CoreSetFilePointer(ctx); }

// --- The C runtime's helpers (src/c_runtime/strings.cpp, math.cpp) ---

// strcpy_s: r3 = the destination, r4 = its size, r5 = the source; r3 0. The
// original where the port declines (its _errno paths, an overlapping copy
// upward), as the removed adapter did.
ITC_DIRECT_ENTRY(825A9DD8) {
  if (!c_runtime::StrcpyS(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    lifted::original::c825A9DD8(ctx);
    return;
  }
  ctx.r3.u64 = 0;
}
// _decomp: f1 = x, r4 = where the exponent goes (a 32-bit int); f1 the
// mantissa.
ITC_DIRECT_ENTRY(825AFF80) {
  ctx.fpscr.disableFlushMode();
  int32_t exponent;
  ctx.f1.f64 = c_runtime::Decomp(ctx.f1.f64, &exponent);
  guest::StoreU32(ctx.r4.u32, uint32_t(exponent));
}
// mbtowc (the C locale's single-byte path): r3 = the wide character out (or
// 0), r4 = the bytes, r5 = their count; r3 0 or 1 (sign-extended).
ITC_DIRECT_ENTRY(825B7EC8) {
  ctx.r3.s64 = c_runtime::Mbtowc(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The action mappers (src/action_mappers/action_mappers.cpp) ---

// ActionMapper::IndexOf: r3 = the mapper (not read), r4 = the action id; r3
// the index (30 out of range).
ITC_DIRECT_ENTRY(8210B140) { ctx.r3.u64 = action_mappers::IndexOf(ctx.r4.u32); }
// ActionMapper::ResetAnalog: r3 = the mapper + 1072; r3 the last curve object.
ITC_DIRECT_ENTRY(8210B438) { ctx.r3.u64 = action_mappers::ResetAnalog(ctx.r3.u32); }
// ActionMapper::ResetWords: r3 = the mapper + 448; r3 the last repeat object.
ITC_DIRECT_ENTRY(8210B8A8) { ctx.r3.u64 = action_mappers::ResetWords(ctx.r3.u32); }
// ClientList::Find: r3 = the type, r4 = n; r3 the node or 0.
ITC_DIRECT_ENTRY(8259C818) {
  ctx.r3.u64 = action_mappers::ClientListFind(ctx.r3.u32, ctx.r4.u32);
}
// ClientList::Count: r3 = the list, r4 = the type, r5 = bump (its low byte);
// r3 the count.
ITC_DIRECT_ENTRY(8259C868) {
  ctx.r3.u64 = action_mappers::ClientListCount(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The graphics device (src/graphics/command_buffer.cpp, device_state.cpp) ---

// Device::WaitForFence: r3 = the device, r4 = the fence, r5 = the wait's
// code; r3 as the removed adapter left it. A recording command buffer (never
// in play) runs the original, as the segment flow's adapters do.
ITC_DIRECT_ENTRY(8229E908) {
  if (graphics::command_buffer::Recording(ctx.r3.u32)) {
    lifted::original::c8229E908(ctx);
    return;
  }
  const uint32_t device = ctx.r3.u32, fence = ctx.r4.u32, code = ctx.r5.u32;
  ctx.r3.u64 = graphics::command_buffer::WaitForFence(device, fence, code,
                                                      hooks::CommandBufferKick(ctx));
}
// Device::SetRenderTarget: r3 = the device, r4 = the index, r5 = the surface.
// The original leaves a temporary in r3 that its callers don't read; this
// leaves the device (as SetRenderTarget_External's adapter).
ITC_DIRECT_ENTRY(822A3D30) {
  ctx.fpscr.disableFlushMode();
  graphics::device_state::SetRenderTarget(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// Device::SetDefaultState: r3 = the device; in a frame like the original's
// (160 bytes), the guest calls it keeps from hooks::DeviceStateCalls, as
// Device::Init's override calls it (src/hooks/graphics_device.cpp).
ITC_DIRECT_ENTRY(822ACB90) {
  const uint32_t device = ctx.r3.u32;
  ctx.fpscr.disableFlushMode();  // the float setters'
  guest_call::InFrame(ctx, 160, 0x822ACBF0, [&](uint32_t) {
    graphics::device_state::SetDefaultState(device, hooks::DeviceStateCalls(ctx, device));
  });
}

// --- The race's states (src/race_states/race_checks.cpp) ---

// Time::Sub: r3 and r4 = the two packed times' addresses; r3 a's less b's,
// packed (Time::Pack's r3).
ITC_DIRECT_ENTRY(82162230) { ctx.r3.u64 = race_states::TimeSub(ctx.r3.u32, ctx.r4.u32); }
