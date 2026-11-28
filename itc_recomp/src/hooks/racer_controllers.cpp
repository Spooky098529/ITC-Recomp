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
// IsConnected's original); InputRecording::Append (sub_821029A8), from
// PlayerControl::Update; and sub_82160880 (never seen in play), from
// RaceFlow::OnState22 and OnState23. Since the race flow's handlers are
// native: RacerControl::DriveByAi (sub_8215F968; sub_8215FB08, not seen in
// play, reaches the original), from the handlers, AllByAi and the
// per-racer states; DriveByAiWithPad (sub_8215F7E8), from AllByAi; AllByAi
// (sub_8215FA68), from OnState22; PlayersAndHeldAi (sub_821606D0), from
// OnState7; PlayersAndAi (sub_821607A0), from OnState23; and
// PlayerControl::Update and ReadPad (sub_821604C8, sub_8215FCA8), from the
// per-racer states and Update.

#include "guest.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/ai_drivers.h"
#include "racer_controllers/racer_controllers.h"
#include "lifted/sdk.h"

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
              "RaceFlow::AllByAi sub_8215FA68, RaceFlow::PlayersAndHeldAi sub_821606D0, "
              "RaceFlow::PlayersAndAi sub_821607A0, RaceFlow::OnState7 sub_82160940, "
              "RaceFlow::OnState22 sub_82160948, RaceFlow::OnState23 sub_821609E0, "
              "RaceFlow::OnState34 sub_8215FC38) "
              "natively instead of through the translated code (the same words; checked by "
              "--run_harnesses=racer_controllers). Those reached only from native callers "
              "have no adapters, and the flag switches their callers: sub_82102258, "
              "sub_82102300, sub_8215F5F0, sub_8215FA10, sub_82102350, sub_82102A28, "
              "sub_82102488, sub_8209F340, sub_8209F3B8, sub_821029A8, sub_82160880, "
              "sub_8215F968, sub_8215F7E8, sub_8215FA68, sub_821606D0, sub_821607A0, "
              "sub_821604C8, sub_8215FCA8; merged into their callers: "
              "sub_821024D0, sub_82591C08");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_RACER_CONTROLLERS(address, ...)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_racer_controllers)) { \
    __VA_ARGS__;                               \
  }

using namespace racer_controllers;

namespace {

struct GuestCalls {
  lifted::Regs* ctx;
};

void CallDriveCar(void* user, uint32_t input, uint32_t index, uint32_t flag, uint32_t frame,
                  uint32_t lr) {
  auto& call = *static_cast<GuestCalls*>(user);
  lifted::Regs& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = input;
  ctx.r4.u64 = index;
  ctx.r5.u64 = flag;
  hooks::AiDriveCar(ctx);
  ctx.r1.u64 = saved;
}

void CallUpdateIdle(void* user, uint64_t driver, uint32_t frame, uint32_t lr, const double* f1) {
  auto& call = *static_cast<GuestCalls*>(user);
  lifted::Regs& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = driver;
  if (f1 != nullptr) {
    ctx.f1.f64 = *f1;
  }
  hooks::AiUpdateIdle(ctx);
  ctx.r1.u64 = saved;
}

uint64_t CallGuest(void* user, uint32_t function, uint64_t r3, uint64_t r4, bool set_r4,
                   uint32_t frame, uint32_t lr, double* f1) {
  auto& call = *static_cast<GuestCalls*>(user);
  lifted::Regs& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  if (set_r4) {
    ctx.r4.u64 = r4;
  }
  lifted::CallFromContext(ctx, function);
  ctx.r1.u64 = saved;
  if (f1 != nullptr) {
    *f1 = ctx.f1.f64;
  }
  return ctx.r3.u64;
}

}  // namespace

// The calls the plain C++ makes into code that takes the context.
#define ITC_RACER_CALLS          \
  GuestCalls call{&ctx};   \
  const Calls calls { &call, CallDriveCar, CallGuest, CallUpdateIdle, \
                      ITC_FLAG(native_action_mappers) }

// --- The input recording ---

ITC_RACER_CONTROLLERS(82102968, ctx.r3.u64 = RecordingHasRoom(ctx.r3.u32))

// --- The race flow's handlers (through the race flow's functors) ---

// r3 = the race flow; r3 left at the stage check's answer. The original
// where operator new can't be native.
ITC_RACER_CONTROLLERS(8215F728, {
  uint32_t r3 = 0;
  if (OnSetUp(ctx.r3.u32, &r3)) {
    ctx.r3.u64 = r3;
    return;
  }
  lifted::original::c8215F728(ctx);
})
// r3 left at the last record, as operator delete leaves it. The original
// where a record isn't the layer's.
ITC_RACER_CONTROLLERS(8215F5E8, {
  uint32_t last = 0;
  if (OnEnd(&last)) {
    ctx.r3.u64 = last;
    return;
  }
  lifted::original::c8215F5E8(ctx);
})

// --- The AI drivers' hand-off (r3 as the originals leave it; f1 DriveCar's) ---

// Through the race flow's functors: a jump to HoldAll, and state 16's.
ITC_RACER_CONTROLLERS(8215FC28, {
  ITC_RACER_CALLS;
  ctx.r3.u64 = OnHold(ctx.r1.u32, calls);
})
ITC_RACER_CONTROLLERS(8215FB90, {
  ITC_RACER_CALLS;
  ctx.r3.u64 = OnState16(ctx.r1.u32, calls);
})

// --- The AI drivers' set-up ---

// r3 = &the racer's index, r4 = &the item id, r5 = &the hold flag.
ITC_RACER_CONTROLLERS(82170FF8, {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = Setup(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
})

// --- The race flow's per-racer states (r3 = the race flow, r4 = the message
// ring's owner) ---

// Through the race flow's functors: state 7's (a jump to PlayersAndHeldAi),
// 22's, 23's and 34's (the attract races').
ITC_RACER_CONTROLLERS(82160940, {
  ITC_RACER_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = OnState7(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
})
ITC_RACER_CONTROLLERS(82160948, {
  ITC_RACER_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = OnState22(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
})
ITC_RACER_CONTROLLERS(821609E0, {
  ITC_RACER_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = OnState23(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
})
ITC_RACER_CONTROLLERS(8215FC38, {
  ITC_RACER_CALLS;
  ctx.r3.u64 = OnState34(ctx.r1.u32, calls);
})

// For the fallback originals (docs/missing-functions.md).
// InputRecording::Append: r3 = the record, r4 = the player.
ITC_DIRECT_ENTRY(821029A8) { ctx.r3.u64 = RecordingAppend(ctx.r3.u32, ctx.r4.u32); }
// PlayerControl::ReadPad: r3 = a record, r4 = a racer index, r5 = a player,
// r6 = the message ring's owner, r7 = may pause.
ITC_DIRECT_ENTRY(8215FCA8) {
  ITC_RACER_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ReadPad(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                       ctx.r1.u32, calls);
}
