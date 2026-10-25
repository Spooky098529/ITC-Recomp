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
              "ActionMappers::SetBlocked sub_8209F318, Modes::SetChecks sub_82106868 (Stages::Copy's "
              "plain C++ called directly), Modes::SetSceneTargetMode sub_82106740 (with "
              "sub_82106778 merged into it), SignInWatch::CheckIdle sub_8209ED58, and "
              "SignInWatch::Update sub_8209EEC8 (with sub_8209F040 merged; wcscpy's and wcscmp's plain "
              "C++ called directly; the prompts' states 1 to 9 the original's), "
              "Presence::SetForMode sub_822934B8 Dropped (presence isn't ported: only the "
              "last error its dropped XUserSetContextEx wrapper clears; sub_82293504 merged)) natively instead of through the "
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

// Modes::SetChecks: r3 = the holder (not read), r4 = the type; r3 left, but
// for type 1, Stages::Copy's out (the frame's +80, from the original's r1,
// whose high word its stwu leaves). Where Stages::Copy's plain C++ declines,
// the original from the start.
extern "C" REX_FUNC(sub_82106868) {
  REX_FUNC_PROLOGUE();
  const uint32_t frame = ctx.r1.u32 - kSetChecksFrame;
  if (!ITC_FLAG(native_mode_tick) || !ModesSetChecks(base, ctx.r4.u32, frame)) {
    __imp__sub_82106868(ctx, base);
    return;
  }
  if (ctx.r4.s32 == 1) {
    ctx.r3.u64 = ((ctx.r1.u64 & 0xFFFFFFFF00000000ull) | frame) + 80;
  }
}

// Modes::SetSceneTargetMode: r3 = the holder (not read), r4 = the type (r3
// left).
ITC_MODE_TICK(82106740, ModesSetSceneTargetMode(base, ctx.r4.u32))

// SignInWatch::CheckIdle: no arguments (r3 left); where the original would
// trap, the original.
extern "C" REX_FUNC(sub_8209ED58) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_mode_tick) || !SignInWatchCheckIdle(base)) {
    __imp__sub_8209ED58(ctx, base);
  }
}

// Presence::SetForMode: r3 = the type; r3 = 0 where the original's last
// XUserSetContextEx call answers it, else left.
ITC_MODE_TICK(822934B8, if (PresenceSetForMode(base, ctx.r13.u32, ctx.r3.u32)) ctx.r3.u64 = 0)

// SignInWatch::Update: r3 = 0x826C5B9C, r4 = the mode (both only for the
// prompts' handlers); r3 = the last name's address as wcscpy and memset
// leave it (sign-extended, as the original's addi). Where the plain C++
// says so (the prompts' states, CheckIdle's traps), the original.
extern "C" REX_FUNC(sub_8209EEC8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_mode_tick) ||
      !SignInWatchUpdate(base, ctx.r1.u32 - kSignInWatchUpdateFrame)) {
    __imp__sub_8209EEC8(ctx, base);
    return;
  }
  ctx.r3.s64 = int32_t(kLastName);
}
