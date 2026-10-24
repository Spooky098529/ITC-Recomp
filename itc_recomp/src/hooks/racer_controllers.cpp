// The racer controllers natively: adapters in front of
// src/racer_controllers/ (docs/functions/racer-controllers.md). Each leaves
// r3 as the original does. The race flow's handlers are called through the
// functors the race flow's set-up registers (sub_82160AE0, recompiled), so
// their adapters stay until it's native. Functions reached only from native
// callers have no adapters (the flag switches their callers):
// CarInputs::Create (sub_82102258), from RaceFlow::OnSetUp; and
// CarInputs::Destroy (sub_82102300), from RaceFlow::OnEnd.

#include "guest.h"

#include "base/flags.h"
#include "racer_controllers/racer_controllers.h"

ITC_FLAG_BOOL(native_racer_controllers, true, "ITC",
              "Run the racer controllers (docs/functions/racer-controllers.md: "
              "RaceFlow::OnSetUp sub_8215F728, RaceFlow::OnEnd sub_8215F5E8, "
              "InputRecording::Append sub_821029A8, InputRecording::HasRoom sub_82102968) "
              "natively instead of through the translated code (the same words; checked by "
              "--run_harnesses=racer_controllers). Those reached only from native callers "
              "have no adapters, and the flag switches their callers: sub_82102258, "
              "sub_82102300");

// Defines an adapter: `body` uses `ctx` and `base`.
#define ITC_RACER_CONTROLLERS(address, body)   \
  extern "C" REX_FUNC(sub_##address) {         \
    REX_FUNC_PROLOGUE();                       \
    if (!ITC_FLAG(native_racer_controllers)) { \
      __imp__sub_##address(ctx, base);         \
      return;                                  \
    }                                          \
    body;                                      \
  }

using namespace racer_controllers;

// --- The input recording ---

ITC_RACER_CONTROLLERS(821029A8, ctx.r3.u64 = RecordingAppend(base, ctx.r3.u32, ctx.r4.u32))
ITC_RACER_CONTROLLERS(82102968, ctx.r3.u64 = RecordingHasRoom(base, ctx.r3.u32))

// --- The race flow's handlers (through the race flow's functors) ---

// r3 = the race flow; r3 left at the stage check's answer. The original
// where operator new can't be native.
ITC_RACER_CONTROLLERS(8215F728, {
  uint32_t r3 = 0;
  if (OnSetUp(base, ctx.r3.u32, &r3)) {
    ctx.r3.u64 = r3;
    return;
  }
  __imp__sub_8215F728(ctx, base);
})
// r3 left at the last record, as operator delete leaves it. The original
// where a record isn't the layer's.
ITC_RACER_CONTROLLERS(8215F5E8, {
  uint32_t last = 0;
  if (DestroyInputs(base, &last)) {
    ctx.r3.u64 = last;
    return;
  }
  __imp__sub_8215F5E8(ctx, base);
})
