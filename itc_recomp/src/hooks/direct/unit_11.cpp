// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 11's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "guest.h"
#include "action_mappers/action_mappers.h"
#include "camera_modes/camera_modes.h"
#include "graphics/resources.h"

// ActionMapper::CurveUnsigned: r3 = the analog stage, r4 = the action, r5 =
// the raw value; f1 the curve's result. r3 the curve (the virtual call's
// this, as the original passes it on).
ITC_DIRECT_ENTRY(8210B4A8) {
  const uint32_t curve = guest::LoadU32(ctx.r3.u32 + 4 * (ctx.r4.u32 + 45));
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = action_mappers::CurveUnsigned(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = curve;
}

// ActionMapper::CurveCentered: as CurveUnsigned.
ITC_DIRECT_ENTRY(8210B500) {
  const uint32_t curve = guest::LoadU32(ctx.r3.u32 + 4 * (ctx.r4.u32 + 45));
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = action_mappers::CurveCentered(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = curve;
}

// Matrix::FromRows3: r3 = out, r4..r6 = rows 0 to 2 (r3 left so).
ITC_DIRECT_ENTRY(820AB5F0) {
  ctx.fpscr.disableFlushMode();
  camera_modes::MatrixFromRows3(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

// Resource::Lock: r3 = the resource, r5 = mips, r6 = the base data, r7 = the
// mip data, r8 = the address, r9 = the size, r10 = the flags (r4 only went to
// the dropped fence wait); r3 the address.
ITC_DIRECT_ENTRY(822A63F8) {
  ctx.r3.u64 = graphics::LockResource(ctx.r3.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                      ctx.r8.u32, ctx.r9.u32, ctx.r10.u32);
}
