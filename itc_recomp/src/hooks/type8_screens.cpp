// The type 8 mode's panel screen natively: adapters in front of
// src/type8_screens/ (docs/functions/type8-screens.md), with the register
// steps the harness shares (hooks/type8_screens.h). Never seen in play
// (ported by the user's 2026-11-12 call); the slots are reached through the
// vtables (the screen's, the group's and Panel8C's), the handlers through
// the screen's machine, so their adapters stay.

#include "hooks/type8_screens.h"

#include "base/flags.h"
#include "hooks/type67_screens.h"
#include "type8_screens/type8_screens.h"

ITC_FLAG_BOOL(native_type8_screens, true, "ITC",
              "Run the type 8 mode's panel screen (docs/functions/type8-screens.md: "
              "PanelGroup8::PanelGroup8 sub_82239C10 and DeletingDestructor sub_8223A2A8, "
              "Panel8F::Panel8F sub_82239C70, Panel8E::Panel8E sub_82239D58, Panel8C::Panel8C "
              "sub_82239E78, DeletingDestructor sub_8223A348, SetStats sub_8223A090 and "
              "ShowArrows sub_8223A098, Panel8D::Panel8D sub_8223A100, "
              "ConfirmPanel8::ConfirmPanel8 sub_8223A208 (the panels' bases, "
              "Panel6C::SetStats and UiMovie::ShowNode and HideNode through the car list's and "
              "the screen constructors' plain C++ or guest calls, operator delete a guest "
              "call); Type8PanelScreen::InitWords sub_82238420, IsLoaded sub_82238418, "
              "OpenMovies sub_82238440, DeleteMovies sub_82238450, OnMessage2 sub_82238458, "
              "OnSaveLoaded sub_82238530, OnAnswer0 to 2 sub_82238540, sub_82238550 and "
              "sub_82238560, Leave sub_82238B10, FadeInGroup sub_82238468 (PanelFade::In's plain "
              "C++, UiMovie::FinishNamed, PlayNamed and LoopNamed guest calls), UpdateOpening "
              "sub_82238A00, Show sub_82238928, Draw sub_82238E78 with DrawList sub_82238DC8 "
              "and AskQuit sub_82238BA0 (UiMovie::SetText's plain C++ where the text setters "
              "are native), OnDialogClosed sub_82238990 and ShowPlayerNodes sub_82238570 "
              "(UiMovie::FindNode, ShowNode, HideNode and SetNodeTexture's and ScreenFades' "
              "plain C++; the shared panel handlers' and the holder's slots through "
              "type67_screens::Slot)) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=type8_screens)");

namespace hooks {

namespace t8 = type8_screens;

#define ITC_T8_STEP(name, expression)                          \
  void name(PPCContext& ctx, uint8_t* base) {                  \
    const t8::Calls calls = Type67ScreensCalls(ctx, base);     \
    (void)calls;                                               \
    const uint32_t sp = ctx.r1.u32;                            \
    (void)sp;                                                  \
    ctx.r3.u64 = (expression);                                 \
  }

ITC_T8_STEP(Type8PanelGroupConstruct, t8::PanelGroup8Construct(base, ctx.r3.u64, ctx.r4.u32, sp))
ITC_T8_STEP(Type8Panel8FConstruct,
            t8::Panel8FConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8Panel8EConstruct,
            t8::Panel8EConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8Panel8CConstruct,
            t8::Panel8CConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8Panel8DConstruct,
            t8::Panel8DConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8ConfirmPanelConstruct,
            t8::ConfirmPanel8Construct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8PanelGroupDeletingDestruct,
            t8::PanelGroup8DeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Panel8CDeletingDestruct,
            t8::Panel8CDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Panel8CSetStats,
            t8::Panel8CSetStats(base, ctx.r3.u64, ctx.r4.u32, sp, t8::CarListCalls(calls)))
ITC_T8_STEP(Type8Panel8CShowArrows,
            t8::Panel8CShowArrows(base, ctx.r3.u64, ctx.r4.u64, sp, t8::CarListCalls(calls)))

ITC_T8_STEP(Type8InitWords, t8::InitWords(base, ctx.r3.u64))
ITC_T8_STEP(Type8IsLoaded, t8::IsLoaded(base))
ITC_T8_STEP(Type8OpenMovies, t8::OpenMovies(sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8DeleteMovies, t8::DeleteMovies(sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8OnMessage2, t8::SetWord(base, ctx.r3.u64, 0x82238458))
ITC_T8_STEP(Type8OnSaveLoaded, t8::SetWord(base, ctx.r3.u64, 0x82238530))
ITC_T8_STEP(Type8OnAnswer0, t8::SetWord(base, ctx.r3.u64, 0x82238540))
ITC_T8_STEP(Type8OnAnswer1, t8::SetWord(base, ctx.r3.u64, 0x82238550))
ITC_T8_STEP(Type8OnAnswer2, t8::SetWord(base, ctx.r3.u64, 0x82238560))
ITC_T8_STEP(Type8Leave, t8::Leave(base, ctx.r3.u64, ctx.r4.u64))
ITC_T8_STEP(Type8FadeInGroup, t8::FadeInGroup(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8UpdateOpening, t8::UpdateOpening(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Show, t8::Show(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Draw, t8::Draw(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8OnDialogClosed, t8::OnDialogClosed(base, ctx.r3.u64, sp, calls))

#undef ITC_T8_STEP

}  // namespace hooks

#define ITC_TYPE8_SCREENS(address, step)    \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_type8_screens)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The panels' and the group's constructors (from the screen's constructor,
// recompiled) and their vtables' slots.
ITC_TYPE8_SCREENS(82239C10, Type8PanelGroupConstruct)
ITC_TYPE8_SCREENS(82239C70, Type8Panel8FConstruct)
ITC_TYPE8_SCREENS(82239D58, Type8Panel8EConstruct)
ITC_TYPE8_SCREENS(82239E78, Type8Panel8CConstruct)
ITC_TYPE8_SCREENS(8223A100, Type8Panel8DConstruct)
ITC_TYPE8_SCREENS(8223A208, Type8ConfirmPanelConstruct)
ITC_TYPE8_SCREENS(8223A2A8, Type8PanelGroupDeletingDestruct)
ITC_TYPE8_SCREENS(8223A348, Type8Panel8CDeletingDestruct)
ITC_TYPE8_SCREENS(8223A090, Type8Panel8CSetStats)
ITC_TYPE8_SCREENS(8223A098, Type8Panel8CShowArrows)
// The screen's vtable slots and its machine's leaf handlers.
ITC_TYPE8_SCREENS(82238420, Type8InitWords)
ITC_TYPE8_SCREENS(82238418, Type8IsLoaded)
ITC_TYPE8_SCREENS(82238440, Type8OpenMovies)
ITC_TYPE8_SCREENS(82238450, Type8DeleteMovies)
ITC_TYPE8_SCREENS(82238458, Type8OnMessage2)
ITC_TYPE8_SCREENS(82238530, Type8OnSaveLoaded)
ITC_TYPE8_SCREENS(82238540, Type8OnAnswer0)
ITC_TYPE8_SCREENS(82238550, Type8OnAnswer1)
ITC_TYPE8_SCREENS(82238560, Type8OnAnswer2)
ITC_TYPE8_SCREENS(82238B10, Type8Leave)
// The machine's other handlers. ShowPlayerNodes sub_82238570, AskQuit
// sub_82238BA0 and DrawList sub_82238DC8 have no adapters: their only
// callers are these handlers, native (with the flag off, the originals call
// the originals).
ITC_TYPE8_SCREENS(82238468, Type8FadeInGroup)
ITC_TYPE8_SCREENS(82238A00, Type8UpdateOpening)
ITC_TYPE8_SCREENS(82238928, Type8Show)
ITC_TYPE8_SCREENS(82238E78, Type8Draw)
ITC_TYPE8_SCREENS(82238990, Type8OnDialogClosed)
