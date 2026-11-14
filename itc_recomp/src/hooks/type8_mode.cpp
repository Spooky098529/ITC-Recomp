// The type 8 mode natively: adapters in front of src/type8_mode/
// (docs/functions/type8-mode.md), with the register steps the harness
// shares (hooks/type8_mode.h). Never seen in play (ported by the user's
// 2026-11-12 call); the handlers are reached through the machines'
// transitions and the dialog's page and step tables, so their adapters
// stay.

#include "hooks/type8_mode.h"

#include "base/flags.h"
#include "hooks/type67_modes.h"
#include "type8_mode/type8_mode.h"

ITC_FLAG_BOOL(native_type8_mode, true, "ITC",
              "Run the type 8 mode (docs/functions/type8-mode.md: Type8Dialog::OnEnter "
              "sub_8223A408, OpenMovies sub_8223A398, WaitForMovies sub_8223ADC0, Draw "
              "sub_8223AC00, Update sub_8223AC58, OpenConfirm sub_8223A510, OpenNotice "
              "sub_8223A4A0, DrawWaiting sub_8223A5F0, ConfirmPage sub_8223A6B8, NoticePage "
              "sub_8223A858 and OpenConfirmStep sub_8223A940 (UiMovies::Find, IsLoaded, "
              "UiMovie::ResetPose, PlayNamed, LoopLast, Menu::Step, Menu::Pressed and "
              "MenuSound::Play's plain C++; UiMovies::Open, UiMovie::Run and the pages and "
              "steps the tables name through their overrides)) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=type8_mode)");

namespace hooks {

namespace t8m = type8_mode;

#define ITC_T8M_STEP(name, expression)                       \
  void name(PPCContext& ctx, uint8_t* base) {                \
    const t8m::Calls calls = Type67Calls(ctx, base);         \
    (void)calls;                                             \
    const uint32_t sp = ctx.r1.u32;                          \
    (void)sp;                                                \
    ctx.r3.u64 = (expression);                               \
  }

ITC_T8M_STEP(Type8DialogOnEnter, t8m::DialogOnEnter(base, ctx.r3.u64))
ITC_T8M_STEP(Type8DialogOpenMovies, t8m::DialogOpenMovies(base, sp, calls))
ITC_T8M_STEP(Type8DialogWaitForMovies, t8m::DialogWaitForMovies(base, ctx.r3.u64, sp))
ITC_T8M_STEP(Type8DialogDraw, t8m::DialogDraw(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8M_STEP(Type8DialogUpdate, t8m::DialogUpdate(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_T8M_STEP(Type8DialogOpenConfirm, t8m::DialogOpenConfirm(base, ctx.r3.u32, sp))
ITC_T8M_STEP(Type8DialogOpenNotice, t8m::DialogOpenNotice(base, ctx.r3.u64))
ITC_T8M_STEP(Type8DialogDrawWaiting, t8m::DialogDrawWaiting(base, ctx.r3.u64, sp))
ITC_T8M_STEP(Type8DialogConfirmPage,
             t8m::DialogConfirmPage(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_T8M_STEP(Type8DialogNoticePage, t8m::DialogNoticePage(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_T8M_STEP(Type8DialogOpenConfirmStep, t8m::DialogOpenConfirmStep(base, ctx.r3.u32, sp, calls))

#undef ITC_T8M_STEP

}  // namespace hooks

#define ITC_TYPE8_MODE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_type8_mode)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// The dialog's handlers (its machine's transitions), pages (its Draw's
// table) and step 0 (its Update's table): no direct callers.
ITC_TYPE8_MODE(8223A408, Type8DialogOnEnter)
ITC_TYPE8_MODE(8223A398, Type8DialogOpenMovies)
ITC_TYPE8_MODE(8223ADC0, Type8DialogWaitForMovies)
ITC_TYPE8_MODE(8223AC00, Type8DialogDraw)
ITC_TYPE8_MODE(8223AC58, Type8DialogUpdate)
ITC_TYPE8_MODE(8223A510, Type8DialogOpenConfirm)
ITC_TYPE8_MODE(8223A4A0, Type8DialogOpenNotice)
ITC_TYPE8_MODE(8223A5F0, Type8DialogDrawWaiting)
ITC_TYPE8_MODE(8223A6B8, Type8DialogConfirmPage)
ITC_TYPE8_MODE(8223A858, Type8DialogNoticePage)
ITC_TYPE8_MODE(8223A940, Type8DialogOpenConfirmStep)
