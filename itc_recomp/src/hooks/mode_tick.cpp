// The mode manager's tick services natively: adapters in front of
// src/mode_tick/ (docs/functions/mode-tick.md). Each leaves r3 as the
// original does.

#include "guest.h"

#include "base/flags.h"
#include "mode_tick/mode_tick.h"

ITC_FLAG_BOOL(native_mode_tick, true, "ITC",
              "Run the mode manager's tick services (docs/functions/mode-tick.md: "
              "Billboards::FlipRing sub_8209BD18, ScreenFades::Init sub_8209CDE8, FadeIn "
              "sub_8209D038 and FadeOut sub_8209D0C8, Players::SignedInSlot sub_8209F658 and "
              "ActionMappers::SetBlocked sub_8209F318) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=mode_tick)");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_MODE_TICK(address, ...)     \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_mode_tick)) {  \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    __VA_ARGS__;                        \
  }

using namespace mode_tick;

// Billboards::FlipRing: no arguments.
ITC_MODE_TICK(8209BD18, BillboardsFlipRing(base))

// ScreenFades::Init: no arguments.
ITC_MODE_TICK(8209CDE8, ScreenFadesInit(base))

// ScreenFades::FadeIn and FadeOut: r3 = the passes, r4, r5, r6 = the color,
// r7 = the layer (r3 left).
ITC_MODE_TICK(8209D038, ScreenFadesFadeIn(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                          ctx.r6.u32, ctx.r7.u32))
ITC_MODE_TICK(8209D0C8, ScreenFadesFadeOut(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                           ctx.r6.u32, ctx.r7.u32))

// Players::SignedInSlot: r3 = a player; r3 = the slot or -1 (sign-extended,
// as the original's li).
ITC_MODE_TICK(8209F658, ctx.r3.s64 = PlayersSignedInSlot(base, ctx.r3.u32))

// ActionMappers::SetBlocked: r3 = the value (left).
ITC_MODE_TICK(8209F318, ActionMappersSetBlocked(base, ctx.r3.u32))
