// The game modes and their state machines natively: adapters in front of
// src/game_modes/ (docs/functions/game-modes.md). Each leaves r3 as the
// original does. Functions reached only from native callers have no
// adapters (the flag switches their callers): StateKey::Equals
// (sub_82106040), from StateMachine::IndexOf; StateMachine::IndexOf
// (sub_821060A0), from StateMachine::Add and Find; StateMachine::Find
// (sub_821061F0), from StateMachine::Handle (its register step,
// hooks::GameModesFind, kept for the harness).

#include "guest.h"

#include "base/flags.h"
#include "game_modes/game_modes.h"
#include "hooks/game_modes.h"
#include "hooks/guest_call.h"

ITC_FLAG_BOOL(native_game_modes, true, "ITC",
              "Run the game modes and their state machines (docs/functions/game-modes.md: "
              "StateMachine::IndexOf sub_821060A0 with StateKey::Equals sub_82106040 (no adapters: "
              "only Add and Find call IndexOf, and only IndexOf Equals), Transition::Make sub_82106100, Transition::Assign "
              "sub_82105FE0, StateMachine::StateMachine sub_82107108, StateMachine::Add "
              "sub_82106158, StateMachine::Find sub_821061F0 (no adapter: only Handle calls it), "
              "StateMachine::Handle sub_82106280, "
              "Mode::Mode sub_82106C20, the race overlays' state machine's handlers "
              "sub_8215DD58, sub_8215DD90, sub_8215DDD0 and sub_8215DE10, the last's virtual "
              "calls guest calls) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=game_modes)");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_GAME_MODES(address, ...)     \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_game_modes)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    __VA_ARGS__;                         \
  }

using namespace game_modes;

// Transition::Make: r3 = the transition, r4 = the key, r5 = the functor part.
ITC_GAME_MODES(82106100, TransitionMake(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// Transition::Assign: r3 = the destination, r4 = the source.
ITC_GAME_MODES(82105FE0, TransitionAssign(base, ctx.r3.u32, ctx.r4.u32))

// StateMachine::StateMachine: r3 = the state machine (left).
ITC_GAME_MODES(82107108, StateMachineConstruct(base, ctx.r3.u32))

// Mode::Mode: r3 = the mode (left).
ITC_GAME_MODES(82106C20, ModeConstruct(base, ctx.r3.u32))

// StateMachine::Add: r3 = the state machine, r4 = the key, r5 = the functor
// part; r3 = the index, or the new transition (as the original computes it,
// from the machine's whole register).
ITC_GAME_MODES(82106158, hooks::GameModesAdd(ctx, base))

// StateMachine::Handle: r3 = the state machine, r4 = the message; r3 = the
// functor part.
ITC_GAME_MODES(82106280, hooks::GameModesHandle(ctx, base))

// The race overlays' state machine's handlers: r3 = the state machine.
ITC_GAME_MODES(8215DD58, OverlaysSetFlags(base, ctx.r3.u32))
ITC_GAME_MODES(8215DD90, OverlaysClearFlags(base, ctx.r3.u32))
ITC_GAME_MODES(8215DDD0, OverlaysSetFlag820(base, ctx.r3.u32))
ITC_GAME_MODES(8215DE10, hooks::GameModesResetOverlays(ctx, base))

namespace {

// The functor part's r3 as the original leaves it: a transition's is the
// machine's register plus the offset, the default's address sign-extended.
void SetPartR3(PPCContext& ctx, uint64_t machine_r3, uint32_t machine, uint32_t part,
               bool found) {
  if (found) {
    ctx.r3.u64 = machine_r3 + (part - machine);
  } else {
    ctx.r3.s64 = int32_t(part);
  }
}

}  // namespace

void hooks::GameModesAdd(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = StateMachineAdd(base, machine, ctx.r4.u32, ctx.r5.u32, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

void hooks::GameModesFind(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool found = false;
  const uint32_t part = StateMachineFind(base, machine, LoadStateKey(base, ctx.r4.u32), &found);
  SetPartR3(ctx, machine_r3, machine, part, found);
}

void hooks::GameModesHandle(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool found = false;
  const uint32_t part = StateMachineHandle(base, machine, ctx.r4.u32, &found);
  SetPartR3(ctx, machine_r3, machine, part, found);
}

// sub_8215DE10's work with its virtual calls (each overlay's slot 3) made
// from a frame like the original's (112 bytes, returning to 0x8215DE4C).
void hooks::GameModesResetOverlays(PPCContext& ctx, uint8_t* base) {
  OverlaysReset(base, ctx.r3.u32, [&](uint32_t overlay, uint32_t target) {
    guest_call::InFrame(ctx, base, 112, 0x8215DE4C, [&](uint32_t) {
      ctx.r3.u64 = overlay;
      ctx.ctr.u64 = target;
      REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    });
  });
}
