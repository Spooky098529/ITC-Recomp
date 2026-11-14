// The type 8 mode natively: adapters in front of src/type8_mode/
// (docs/functions/type8-mode.md), with the register steps the harness
// shares (hooks/type8_mode.h). Never seen in play (ported by the user's
// 2026-11-12 call); the handlers are reached through the machines'
// transitions and the dialog's page and step tables, so their adapters
// stay.

#include "hooks/type8_mode.h"

#include "base/flags.h"
#include "hooks/type67_modes.h"
#include "hooks/type67_screens.h"
#include "type8_mode/type8_mode.h"

ITC_FLAG_BOOL(native_type8_mode, true, "ITC",
              "Run the type 8 mode (docs/functions/type8-mode.md: Type8Dialog::OnEnter "
              "sub_8223A408, OpenMovies sub_8223A398, WaitForMovies sub_8223ADC0, Draw "
              "sub_8223AC00, Update sub_8223AC58, OpenConfirm sub_8223A510, OpenNotice "
              "sub_8223A4A0, DrawWaiting sub_8223A5F0, ConfirmPage sub_8223A6B8, NoticePage "
              "sub_8223A858 and OpenConfirmStep sub_8223A940 (UiMovies::Find, IsLoaded, "
              "UiMovie::ResetPose, PlayNamed, LoopLast, Menu::Step, Menu::Pressed and "
              "MenuSound::Play's plain C++; UiMovies::Open, UiMovie::Run and the pages and "
              "steps the tables name through their overrides); Type8Screen0::Type8Screen0 "
              "sub_8223D2F0, ~Type8Screen0 sub_8223BF00, DeletingDestructor sub_8223C528, "
              "OnEnter sub_8223B630, OpenMovie sub_8223B530, WaitForMovie sub_8223B6D8, Suspend "
              "sub_8223B540, PickYes sub_8223B590, PickNo sub_8223B5A0, PickBack sub_8223B5B0, "
              "OnDialogClosed sub_8223C098, Leave sub_8223BA48, OnLeft sub_8223B588, Confirm "
              "sub_8223C430, ConfirmPage sub_8223B5C0, ChosenPage sub_8223C0C0, ShowPage "
              "sub_8223BD20, PlaceCursor sub_8223BE88 and Draw sub_8223C888 (ScreenMachine's "
              "constructor, MenuList::Init, SetSelection, Selection and Release, UiMovie::SetText, "
              "FindNode and the node setters by their plain C++ where they run natively; "
              "UiMovies::Open and Delete, FinishAll, operator delete and MenuPage as guest "
              "calls)) natively instead of through the "
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

#define ITC_T8M_SCREEN(name, expression)                     \
  void name(PPCContext& ctx, uint8_t* base) {                \
    const t8m::ScreenCalls calls = Type67ScreensCalls(ctx, base); \
    (void)calls;                                             \
    const uint32_t sp = ctx.r1.u32;                          \
    (void)sp;                                                \
    const uint32_t lr = uint32_t(ctx.lr);                    \
    (void)lr;                                                \
    ctx.r3.u64 = (expression);                               \
  }

ITC_T8M_SCREEN(Type8Screen0Construct, t8m::Screen0Construct(base, ctx.r3.u64, sp))
ITC_T8M_SCREEN(Type8Screen0Destruct, t8m::Screen0Destruct(base, ctx.r3.u64, sp, lr, calls))
ITC_T8M_SCREEN(Type8Screen0DeletingDestruct,
               t8m::Screen0DeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8M_SCREEN(Type8Screen0OnEnter, t8m::Screen0OnEnter(base, ctx.r3.u64, sp, calls))
ITC_T8M_SCREEN(Type8Screen0OpenMovie, t8m::Screen0OpenMovie(sp, lr, calls))
ITC_T8M_SCREEN(Type8Screen0WaitForMovie, t8m::Screen0WaitForMovie(base, ctx.r3.u64, sp))
ITC_T8M_SCREEN(Type8Screen0Suspend, t8m::Screen0Suspend(base, ctx.r3.u64, sp, calls))
ITC_T8M_SCREEN(Type8Screen0PickYes, t8m::Screen0Pick(base, ctx.r3.u64, 1))
ITC_T8M_SCREEN(Type8Screen0PickNo, t8m::Screen0Pick(base, ctx.r3.u64, 0))
ITC_T8M_SCREEN(Type8Screen0PickBack, t8m::Screen0Pick(base, ctx.r3.u64, 2))
ITC_T8M_SCREEN(Type8Screen0OnDialogClosed, t8m::Screen0OnDialogClosed(base, ctx.r3.u64, sp))
ITC_T8M_SCREEN(Type8Screen0Leave, t8m::Screen0Leave(base, ctx.r4.u32, sp, calls))
ITC_T8M_SCREEN(Type8Screen0OnLeft, t8m::Screen0OnLeft(base, ctx.r3.u64, sp, lr, calls))
ITC_T8M_SCREEN(Type8Screen0Confirm,
               t8m::Screen0Confirm(base, ctx.r3.u64, ctx.r5.u32, sp, calls))
ITC_T8M_SCREEN(Type8Screen0ConfirmPage, t8m::Screen0ConfirmPage(base, ctx.r3.u64))
ITC_T8M_SCREEN(Type8Screen0ChosenPage,
               t8m::Screen0ChosenPage(base, ctx.r3.u64, ctx.r4.u32, sp))
ITC_T8M_SCREEN(Type8Screen0ShowPage,
               t8m::Screen0ShowPage(base, ctx.r4.u64, ctx.r5.u64, sp))
ITC_T8M_SCREEN(Type8Screen0PlaceCursor, t8m::Screen0PlaceCursor(base, ctx.r4.u64, sp))
ITC_T8M_SCREEN(Type8Screen0Draw, t8m::Screen0Draw(base, ctx.r3.u64, ctx.r4.u64, sp, calls))

#undef ITC_T8M_SCREEN
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
// The first screen's constructor and destructor (from the member's,
// recompiled), its vtable's slot 0, its machine's handlers, its Draw's pages
// and its helpers with recompiled callers (Confirm from MenuPage, ShowPage
// and PlaceCursor from the steps).
ITC_TYPE8_MODE(8223D2F0, Type8Screen0Construct)
ITC_TYPE8_MODE(8223BF00, Type8Screen0Destruct)
ITC_TYPE8_MODE(8223C528, Type8Screen0DeletingDestruct)
ITC_TYPE8_MODE(8223B630, Type8Screen0OnEnter)
ITC_TYPE8_MODE(8223B530, Type8Screen0OpenMovie)
ITC_TYPE8_MODE(8223B6D8, Type8Screen0WaitForMovie)
ITC_TYPE8_MODE(8223B540, Type8Screen0Suspend)
ITC_TYPE8_MODE(8223B590, Type8Screen0PickYes)
ITC_TYPE8_MODE(8223B5A0, Type8Screen0PickNo)
ITC_TYPE8_MODE(8223B5B0, Type8Screen0PickBack)
ITC_TYPE8_MODE(8223C098, Type8Screen0OnDialogClosed)
ITC_TYPE8_MODE(8223BA48, Type8Screen0Leave)
ITC_TYPE8_MODE(8223B588, Type8Screen0OnLeft)
ITC_TYPE8_MODE(8223C430, Type8Screen0Confirm)
ITC_TYPE8_MODE(8223B5C0, Type8Screen0ConfirmPage)
ITC_TYPE8_MODE(8223C0C0, Type8Screen0ChosenPage)
ITC_TYPE8_MODE(8223BD20, Type8Screen0ShowPage)
ITC_TYPE8_MODE(8223BE88, Type8Screen0PlaceCursor)
ITC_TYPE8_MODE(8223C888, Type8Screen0Draw)
