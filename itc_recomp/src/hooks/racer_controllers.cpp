// The racer controllers natively: adapters in front of
// src/racer_controllers/ (docs/functions/racer-controllers.md). Each leaves
// r3 as the original does. The race flow's handlers are called through the
// functors the race flow's set-up registers (sub_82160AE0, recompiled), so
// their adapters stay until it's native. Functions reached only from native
// callers have no adapters (the flag switches their callers):
// CarInputs::Create (sub_82102258), from RaceFlow::OnSetUp; and
// CarInputs::Destroy (sub_82102300), from RaceFlow::OnEnd; sub_8215F5F0
// (never seen in play), from RaceFlow::OnState16; and RaceFlow::HoldAll
// (sub_8215FA10), from RaceFlow::OnHold and OnState16; PlayerInput::Read
// (sub_82102350), from PlayerInput::Control; InputRecording::Next
// (sub_82102A28, never seen in play), from PlayerControl::Update; and since
// the pad reader is native, PlayerInput::Control (sub_82102488), from
// ReadPad and DriveByAiWithPad; Pad::IsConnected and IsWheel (sub_8209F340,
// sub_8209F3B8), from ReadPad (sub_822231B0, not seen in play, reaches
// IsConnected's original); and InputRecording::Append (sub_821029A8), from
// PlayerControl::Update.

#include "guest.h"

#include "base/flags.h"
#include "hooks/ai_drivers.h"
#include "racer_controllers/racer_controllers.h"

ITC_FLAG_DECLARE(bool, native_action_mappers);

ITC_FLAG_BOOL(native_racer_controllers, true, "ITC",
              "Run the racer controllers (docs/functions/racer-controllers.md: "
              "RaceFlow::OnSetUp sub_8215F728, RaceFlow::OnEnd sub_8215F5E8, "
              "InputRecording::Append sub_821029A8, InputRecording::HasRoom sub_82102968, "
              "RacerControl::DriveByAi sub_8215F968, RaceFlow::HoldAll sub_8215FA10, "
              "RaceFlow::OnHold sub_8215FC28, RaceFlow::OnState16 sub_8215FB90, "
              "AiDriver::Setup sub_82170FF8, PlayerInput::Read sub_82102350, "
              "PlayerInput::Control sub_82102488, Pad::IsConnected sub_8209F340, "
              "Pad::IsWheel sub_8209F3B8, PlayerControl::ReadPad sub_8215FCA8, "
              "PlayerControl::Update sub_821604C8, RacerControl::DriveByAiWithPad sub_8215F7E8, "
              "RaceFlow::AllByAi sub_8215FA68) "
              "natively instead of through the translated code (the same words; checked by "
              "--run_harnesses=racer_controllers). Those reached only from native callers "
              "have no adapters, and the flag switches their callers: sub_82102258, "
              "sub_82102300, sub_8215F5F0, sub_8215FA10, sub_82102350, sub_82102A28, "
              "sub_82102488, sub_8209F340, sub_8209F3B8, sub_821029A8; merged into their callers: "
              "sub_821024D0, sub_82591C08");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_RACER_CONTROLLERS(address, ...)    \
  extern "C" REX_FUNC(sub_##address) {         \
    REX_FUNC_PROLOGUE();                       \
    if (!ITC_FLAG(native_racer_controllers)) { \
      __imp__sub_##address(ctx, base);         \
      return;                                  \
    }                                          \
    __VA_ARGS__;                               \
  }

using namespace racer_controllers;

namespace {

struct GuestCalls {
  PPCContext* ctx;
  uint8_t* base;
};

void CallDriveCar(void* user, uint32_t input, uint32_t index, uint32_t flag, uint32_t frame,
                  uint32_t lr) {
  auto& call = *static_cast<GuestCalls*>(user);
  PPCContext& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = input;
  ctx.r4.u64 = index;
  ctx.r5.u64 = flag;
  hooks::AiDriveCar(ctx, call.base);
  ctx.r1.u64 = saved;
}

void CallUpdateIdle(void* user, uint64_t driver, uint32_t frame, uint32_t lr, const double* f1) {
  auto& call = *static_cast<GuestCalls*>(user);
  PPCContext& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = driver;
  if (f1 != nullptr) {
    ctx.f1.f64 = *f1;
  }
  hooks::AiUpdateIdle(ctx, call.base);
  ctx.r1.u64 = saved;
}

uint64_t CallGuest(void* user, uint32_t function, uint64_t r3, uint64_t r4, bool set_r4,
                   uint32_t frame, uint32_t lr, double* f1) {
  auto& call = *static_cast<GuestCalls*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  if (set_r4) {
    ctx.r4.u64 = r4;
  }
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = saved;
  if (f1 != nullptr) {
    *f1 = ctx.f1.f64;
  }
  return ctx.r3.u64;
}

}  // namespace

// The calls the plain C++ makes into code that takes the context.
#define ITC_RACER_CALLS          \
  GuestCalls call{&ctx, base};   \
  const Calls calls { &call, CallDriveCar, CallGuest, CallUpdateIdle, \
                      ITC_FLAG(native_action_mappers) }

// --- The input recording ---

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

// --- The AI drivers' hand-off (r3 as the originals leave it; f1 DriveCar's) ---

// r3 = the racer's index, r4 = the hold flag.
ITC_RACER_CONTROLLERS(8215F968, {
  ITC_RACER_CALLS;
  DriveByAi(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls, &ctx.r3.u64);
})
// Through the race flow's functors: a jump to HoldAll, and state 16's.
ITC_RACER_CONTROLLERS(8215FC28, {
  ITC_RACER_CALLS;
  ctx.r3.u64 = HoldAll(base, ctx.r1.u32, calls);
})
ITC_RACER_CONTROLLERS(8215FB90, {
  ITC_RACER_CALLS;
  ctx.r3.u64 = OnState16(base, ctx.r1.u32, calls);
})

// --- The AI drivers' set-up ---

// r3 = &the racer's index, r4 = &the item id, r5 = &the hold flag.
ITC_RACER_CONTROLLERS(82170FF8, {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = Setup(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
})


// --- The pad reader and the human cars ---

// r3 = the record, r4 = the racer's index, r5 = the player, r6 = the message
// ring's owner, r7 = may pause. f1 left by UpdateIdle (the speed before it,
// when read).
ITC_RACER_CONTROLLERS(8215FCA8, {
  ITC_RACER_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ReadPad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                       ctx.r1.u32, calls);
})
// r3 = the racer's record slot, r4 = the racer's index, then as ReadPad.
ITC_RACER_CONTROLLERS(821604C8, {
  ITC_RACER_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = PlayerUpdate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                            ctx.r1.u32, calls);
})
// r3 not read, r4 = the racer's index, r5 = the hold flag, r6 = the player.
ITC_RACER_CONTROLLERS(8215F7E8, {
  ITC_RACER_CALLS;
  Regs regs{ctx.r3.u64, 0, false};
  DriveByAiWithPad(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32, calls, &regs);
  ctx.r3.u64 = regs.r3;
})
// r3 = the race flow.
ITC_RACER_CONTROLLERS(8215FA68, {
  ITC_RACER_CALLS;
  ctx.r3.u64 = AllByAi(base, ctx.r3.u32, ctx.r1.u32, calls);
})
