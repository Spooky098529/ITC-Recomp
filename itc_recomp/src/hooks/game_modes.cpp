// The game modes and their state machines natively: adapters in front of
// src/game_modes/ (docs/functions/game-modes.md). Each leaves r3 as the
// original does. StateKey::Equals (sub_82106040) has none: in play only
// StateMachine::IndexOf calls it, and the flag switches that.

#include "guest.h"

#include "base/flags.h"
#include "game_modes/game_modes.h"

ITC_FLAG_BOOL(native_game_modes, true, "ITC",
              "Run the game modes and their state machines (docs/functions/game-modes.md: "
              "StateMachine::IndexOf sub_821060A0 with StateKey::Equals sub_82106040 (no adapter: "
              "only IndexOf calls it), Transition::Make sub_82106100, Transition::Assign "
              "sub_82105FE0, StateMachine::StateMachine sub_82107108, Mode::Mode sub_82106C20) "
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

// StateMachine::IndexOf: r3 = the state machine, r4 = the key; r3 = the
// index or -1.
ITC_GAME_MODES(821060A0,
               ctx.r3.s64 = StateMachineIndexOf(base, ctx.r3.u32, ctx.r4.u32))

// Transition::Make: r3 = the transition, r4 = the key, r5 = the functor part.
ITC_GAME_MODES(82106100, TransitionMake(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// Transition::Assign: r3 = the destination, r4 = the source.
ITC_GAME_MODES(82105FE0, TransitionAssign(base, ctx.r3.u32, ctx.r4.u32))

// StateMachine::StateMachine: r3 = the state machine (left).
ITC_GAME_MODES(82107108, StateMachineConstruct(base, ctx.r3.u32))

// Mode::Mode: r3 = the mode (left).
ITC_GAME_MODES(82106C20, ModeConstruct(base, ctx.r3.u32))
