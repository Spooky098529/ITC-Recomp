// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 32's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "base/float_mode.h"
#include "c_runtime/strings.h"
#include "hooks/second_set.h"
#include "lifted/originals.h"
#include "native/buffers.h"
#include "schema/second_set.h"
#include "second_set/finds.h"
#include "second_set/morph.h"
#include "second_set/wheels.h"

namespace {

namespace ss = second_set;

// The morph's flush mode, as hooks/second_set.cpp's FlushOffForMorph and
// SetFlush: off while the plain C++ runs, then as the original leaves it.
bool FlushOff(lifted::Regs& ctx) {
  const bool was_on =
      (::base::FloatMode::getcsr() & uint32_t(::base::FloatMode::FlushMask)) != 0;
  ctx.fpscr.disableFlushMode();
  return was_on;
}

void SetFlush(lifted::Regs& ctx, ss::Flush flush, bool was_on) {
  if (flush == ss::Flush::kOn || (flush == ss::Flush::kUnchanged && was_on)) {
    ctx.fpscr.enableFlushMode();
  } else {
    ctx.fpscr.disableFlushMode();
  }
}

// Whether Buffers_Memcpy will take a blend's copy (morph.cpp's WillCopy).
bool WillCopy(uint32_t to, uint32_t from, uint32_t bytes) {
  return native::Buffers_NativeMemcpy() &&
         (bytes == 0 || !c_runtime::MayShare(to, bytes, from, bytes));
}

// Morph::Blend6 or Blend7 on r3 = the record: the original where a memcpy
// would go to the original (as Morph::Blend's step declines).
void BlendStep(lifted::Regs& ctx, bool seven, lifted::RegsFunction* original) {
  const schema::Morph m(ctx.r3.u32);
  const uint32_t bytes = m.vertices() << 4;
  if (!WillCopy(m.out1().address, m.base_stream(), bytes) ||
      (seven && !WillCopy(m.out2().address, m.base_stream2(), bytes))) {
    original(ctx);
    return;
  }
  const bool was_on = FlushOff(ctx);
  const ss::BlendResult result =
      seven ? ss::Blend7(ctx.r3.u32, ctx.r1.u32) : ss::Blend6(ctx.r3.u32, ctx.r1.u32);
  ctx.r3.u64 = result.r3;
  SetFlush(ctx, result.flush, was_on);
}

// A traffic point set-up (r3 the group, r4 the part model), gated as
// TrafficPoints::Find's step.
void TrafficFindStep(lifted::Regs& ctx, hooks::FindFunction find,
                     lifted::RegsFunction* original) {
  if (!hooks::SecondSetFindsNative()) {
    original(ctx);
    return;
  }
  hooks::SecondSetFindStep(ctx, find);
}

// A wheels' group set-up (r3 the group, r4 the model), gated as
// WheelPoints::SetUp's step.
void WheelStep(lifted::Regs& ctx, hooks::WheelFunction step,
               lifted::RegsFunction* original) {
  if (!hooks::SecondSetWheelsNative()) {
    original(ctx);
    return;
  }
  hooks::SecondSetWheelStep(ctx, step);
}

}  // namespace

// The second set (src/second_set/, hooks/second_set.h's steps).
ITC_DIRECT_ENTRY(820E35D8) { hooks::SecondSetInteriorPointsReset(ctx); }
ITC_DIRECT_ENTRY(820E3748) { hooks::SecondSetSceneInteriorReset(ctx); }
ITC_DIRECT_ENTRY(820E3890) { hooks::SecondSetSceneInteriorRelease(ctx); }
ITC_DIRECT_ENTRY(820E39B0) { hooks::SecondSetInteriorInitBody(ctx); }
ITC_DIRECT_ENTRY(820E4FD8) { hooks::SecondSetInteriorPointsConstruct(ctx); }
ITC_DIRECT_ENTRY(820E50C0) { hooks::SecondSetInteriorPointsDestruct(ctx); }
ITC_DIRECT_ENTRY(820E5178) { hooks::SecondSetSceneInteriorDestruct(ctx); }
ITC_DIRECT_ENTRY(820E54E8) { hooks::SecondSetTrafficResetBody(ctx); }
// TrafficPoints::FindBody: r3 the group, r4 to r7 models 0 to 3; gated as
// ModelPoint::FindShape's step (no float work, the flush mode untouched).
ITC_DIRECT_ENTRY(820E5538) {
  if (!native::Buffers_NativeMemcpy()) {
    lifted::original::c820E5538(ctx);
    return;
  }
  ctx.r3.u64 = ss::TrafficFindBody(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                   ctx.r7.u32, ctx.r1.u32);
}
ITC_DIRECT_ENTRY(820E56D0) { hooks::SecondSetTrafficResetHeadlights(ctx); }
ITC_DIRECT_ENTRY(820E5738) {
  TrafficFindStep(ctx, ss::TrafficFindHeadlights, lifted::original::c820E5738);
}
ITC_DIRECT_ENTRY(820E57A0) { hooks::SecondSetTrafficResetTailLights(ctx); }
ITC_DIRECT_ENTRY(820E5808) {
  TrafficFindStep(ctx, ss::TrafficFindTailLights, lifted::original::c820E5808);
}
ITC_DIRECT_ENTRY(820E5880) { hooks::SecondSetTrafficPointsReset(ctx); }
ITC_DIRECT_ENTRY(820E5930) { hooks::SecondSetSceneTrafficReset(ctx); }
ITC_DIRECT_ENTRY(820E5A18) { hooks::SecondSetSceneTrafficRelease(ctx); }
ITC_DIRECT_ENTRY(820E68E8) { hooks::SecondSetTrafficPointsConstruct(ctx); }
ITC_DIRECT_ENTRY(820E69F0) { hooks::SecondSetSceneTrafficDestruct(ctx); }
ITC_DIRECT_ENTRY(820E74B8) { hooks::SecondSetWheelsResetTire(ctx); }
ITC_DIRECT_ENTRY(820E7598) {
  WheelStep(ctx, ss::WheelsSetUpTire, lifted::original::c820E7598);
}
ITC_DIRECT_ENTRY(820E7918) { hooks::SecondSetWheelsResetRimA(ctx); }
ITC_DIRECT_ENTRY(820E79C8) {
  WheelStep(ctx, ss::WheelsSetUpRimA, lifted::original::c820E79C8);
}
ITC_DIRECT_ENTRY(820E7C08) { hooks::SecondSetWheelsResetRimB(ctx); }
ITC_DIRECT_ENTRY(820E7CA0) {
  WheelStep(ctx, ss::WheelsSetUpRimB, lifted::original::c820E7CA0);
}
ITC_DIRECT_ENTRY(820E7F90) { hooks::SecondSetWheelsResetBrakes(ctx); }
ITC_DIRECT_ENTRY(820E8028) {
  WheelStep(ctx, ss::WheelsSetUpBrakes, lifted::original::c820E8028);
}
ITC_DIRECT_ENTRY(820E85C8) { hooks::SecondSetWheelPointsReset(ctx); }
ITC_DIRECT_ENTRY(820E88A8) { hooks::SecondSetSceneWheelsReset(ctx); }
ITC_DIRECT_ENTRY(820E89B0) { hooks::SecondSetSceneWheelsRelease(ctx); }
ITC_DIRECT_ENTRY(820E8A80) { hooks::SecondSetWheelsInitTire(ctx); }
ITC_DIRECT_ENTRY(820E99C0) { hooks::SecondSetWheelsInitRimA(ctx); }
ITC_DIRECT_ENTRY(820EA8E8) { hooks::SecondSetWheelsInitRimB(ctx); }
ITC_DIRECT_ENTRY(820EB320) { hooks::SecondSetWheelsInitBrakes(ctx); }
ITC_DIRECT_ENTRY(820ECC48) { hooks::SecondSetWheelPointsConstruct(ctx); }
ITC_DIRECT_ENTRY(820ECCB8) { hooks::SecondSetWheelPointsDestruct(ctx); }
ITC_DIRECT_ENTRY(820ECDD0) { hooks::SecondSetSceneWheelsDestruct(ctx); }
ITC_DIRECT_ENTRY(8258B578) { BlendStep(ctx, false, lifted::original::c8258B578); }
ITC_DIRECT_ENTRY(8258B678) { BlendStep(ctx, true, lifted::original::c8258B678); }
ITC_DIRECT_ENTRY(8258BD28) { hooks::SecondSetMorphBlend(ctx); }
ITC_DIRECT_ENTRY(82594D10) { hooks::SecondSetMorphRun(ctx); }
