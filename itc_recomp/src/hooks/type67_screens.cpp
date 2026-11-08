// The type 6 and 7 modes' list, panel and node screens natively: adapters
// in front of src/type67_screens/ (docs/functions/type67-screens.md), with
// the register steps the harness shares (hooks/type67_screens.h). The
// handlers are reached through their machines' transitions (functors) and
// the slots through the screens' vtables, so their adapters stay.

#include "hooks/type67_screens.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "hooks/type67_modes.h"

ITC_FLAG_BOOL(native_type67_screens, true, "ITC",
              "Run the type 6 and 7 modes' list, panel and node screens "
              "(docs/functions/type67-screens.md: the list screens' ListScreen::OnEnter "
              "sub_82251618, OpenMovie sub_82251508, WaitForMovie sub_82251678, Leave "
              "sub_82251760, OnLeft sub_82251710 and IsLoaded sub_82242F70, "
              "Type6ListScreen::OpenMovie sub_82233320 and Release sub_82233330, "
              "Type7ListScreen::OpenMovie sub_82242F10 and Release sub_82242F20; the node "
              "screen's NodeScreen::OnEnter sub_822591B0, OpenMovie sub_822590D0, WaitForMovie "
              "sub_82259208, Leave sub_822592D8 and OnLeft sub_82259298, "
              "Type6NodeScreen::IsLoaded sub_82236FD8, OpenMovie sub_82236EB0 and Release "
              "sub_82236EC0; their vtable calls to the plain C++ where a slot holds one, "
              "else guest calls; UiMovies::Open and Delete natively where their adapters "
              "would, MenuList::Release's plain C++ over the allocation layer) natively instead "
              "of through the translated code (the same writes; checked by "
              "--run_harnesses=type67_screens)");

namespace hooks {

namespace ts = type67_screens;

ts::Calls Type67ScreensCalls(PPCContext& ctx, uint8_t* base) {
  ts::Calls calls;
  static_cast<type67_modes::Calls&>(calls) = Type67Calls(ctx, base);
  calls.native_heap = NativeHeapActive();
  calls.set_r4 = [&ctx](uint64_t r4) { ctx.r4.u64 = r4; };
  return calls;
}

#define ITC_TS_STEP(name, expression)                \
  void name(PPCContext& ctx, uint8_t* base) {        \
    const ts::Calls calls = Type67ScreensCalls(ctx, base); \
    (void)calls;                                     \
    ctx.r3.u64 = (expression);                       \
  }

ITC_TS_STEP(ListScreenOnEnter, ts::ListScreenOnEnter(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenOpenMovie,
            ts::ListScreenOpenMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListScreenWaitForMovie,
            ts::ListScreenWaitForMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenLeave, ts::ListScreenLeave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenOnLeft, ts::ListScreenOnLeft(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenIsLoaded, ts::ListScreenIsLoaded(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(Type6ListScreenOpenMovie,
            ts::ListScreenOpenAsset(base, ctx.r3.u64, false, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type7ListScreenOpenMovie,
            ts::ListScreenOpenAsset(base, ctx.r3.u64, true, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type6ListScreenRelease, ts::ListScreenRelease(base, ctx.r3.u64, false, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenRelease, ts::ListScreenRelease(base, ctx.r3.u64, true, ctx.r1.u32, calls))

ITC_TS_STEP(NodeScreenOnEnter, ts::NodeScreenOnEnter(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenOpenMovie,
            ts::NodeScreenOpenMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(NodeScreenWaitForMovie,
            ts::NodeScreenWaitForMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenLeave, ts::NodeScreenLeave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenOnLeft, ts::NodeScreenOnLeft(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenIsLoaded, ts::NodeScreenIsLoaded(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(Type6NodeScreenOpenMovie,
            ts::NodeScreenOpenAsset(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type6NodeScreenRelease, ts::NodeScreenRelease(base, ctx.r3.u64, ctx.r1.u32, calls))

ITC_TS_STEP(PanelGroupMovie, ts::PanelGroupMovie(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(GroupPairSelectA, ts::GroupPairSelectA(base, ctx.r3.u64))
ITC_TS_STEP(GroupPairReset, ts::GroupPairReset(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(GroupPairRun, ts::GroupPairRun(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(GroupPairWaitForFade, ts::GroupPairWaitForFade(base, ctx.r3.u64))
ITC_TS_STEP(GroupPairFinishAll, ts::GroupPairFinishAll(base, ctx.r3.u64, ctx.r1.u32, calls))

ITC_TS_STEP(PanelScreenIsLoaded, ts::PanelScreenIsLoaded(base, ctx.r1.u32))
ITC_TS_STEP(Type6PanelScreenOpenMovies, ts::PanelScreenOpenMovies(base, ctx.r1.u32, false, calls))
ITC_TS_STEP(Type7PanelScreenOpenMovies, ts::PanelScreenOpenMovies(base, ctx.r1.u32, true, calls))
ITC_TS_STEP(PanelScreenDeleteMovies, ts::PanelScreenDeleteMovies(base, ctx.r1.u32, calls))
ITC_TS_STEP(Type7PanelScreenBack, ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x82240860))
ITC_TS_STEP(Type6PanelScreenOpenPowerGraph,
            ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x82247058))
ITC_TS_STEP(Type7PanelScreenOpenPowerGraph,
            ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x822408B8))
ITC_TS_STEP(Type7PanelScreenCarList, ts::Type7CarWord(base, 1972))
ITC_TS_STEP(Type7PanelScreenCarEntry, ts::Type7CarWord(base, 1976))
ITC_TS_STEP(Type7PanelScreenCarSlot, ts::Type7CarWord(base, 1980))
ITC_TS_STEP(PanelScreenOnEnter, ts::PanelScreenOnEnter(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOpenMovie,
            ts::PanelScreenOpenMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenWaitForMovie,
            ts::PanelScreenWaitForMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOpen, ts::PanelScreenOpen(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenShow, ts::PanelScreenShow(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenUpdate, ts::PanelScreenUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOnMessage24,
            ts::PanelScreenOnMessage24(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenLeave, ts::PanelScreenLeave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOnLeft, ts::PanelScreenOnLeft(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7PanelScreenDraw,
            ts::Type7PanelScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

#undef ITC_TS_STEP

}  // namespace hooks

#define ITC_TYPE67_SCREENS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_type67_screens)) {   \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

// The list screens' handlers (through their machines' transitions) and
// vtable slots.
ITC_TYPE67_SCREENS(82251618, ListScreenOnEnter)
ITC_TYPE67_SCREENS(82251508, ListScreenOpenMovie)
ITC_TYPE67_SCREENS(82251678, ListScreenWaitForMovie)
ITC_TYPE67_SCREENS(82251760, ListScreenLeave)
ITC_TYPE67_SCREENS(82251710, ListScreenOnLeft)
ITC_TYPE67_SCREENS(82242F70, ListScreenIsLoaded)
ITC_TYPE67_SCREENS(82233320, Type6ListScreenOpenMovie)
ITC_TYPE67_SCREENS(82242F10, Type7ListScreenOpenMovie)
ITC_TYPE67_SCREENS(82233330, Type6ListScreenRelease)
ITC_TYPE67_SCREENS(82242F20, Type7ListScreenRelease)
// The node screen's handlers and vtable slots.
ITC_TYPE67_SCREENS(822591B0, NodeScreenOnEnter)
ITC_TYPE67_SCREENS(822590D0, NodeScreenOpenMovie)
ITC_TYPE67_SCREENS(82259208, NodeScreenWaitForMovie)
ITC_TYPE67_SCREENS(822592D8, NodeScreenLeave)
ITC_TYPE67_SCREENS(82259298, NodeScreenOnLeft)
ITC_TYPE67_SCREENS(82236FD8, Type6NodeScreenIsLoaded)
ITC_TYPE67_SCREENS(82236EB0, Type6NodeScreenOpenMovie)
ITC_TYPE67_SCREENS(82236EC0, Type6NodeScreenRelease)
// The group pair's slots (through its vtable) and PanelGroup::Movie (from
// the car list's functions, recompiled).
ITC_TYPE67_SCREENS(82258FC8, PanelGroupMovie)
ITC_TYPE67_SCREENS(82249C30, GroupPairSelectA)
ITC_TYPE67_SCREENS(82249C40, GroupPairReset)
ITC_TYPE67_SCREENS(82249BE0, GroupPairRun)
ITC_TYPE67_SCREENS(82249BA8, GroupPairWaitForFade)
ITC_TYPE67_SCREENS(82249C70, GroupPairFinishAll)
// The panel screens' vtable slots.
ITC_TYPE67_SCREENS(82230790, PanelScreenIsLoaded)
ITC_TYPE67_SCREENS(82230758, Type6PanelScreenOpenMovies)
ITC_TYPE67_SCREENS(822401D0, Type7PanelScreenOpenMovies)
ITC_TYPE67_SCREENS(82240208, PanelScreenDeleteMovies)
ITC_TYPE67_SCREENS(82240860, Type7PanelScreenBack)
ITC_TYPE67_SCREENS(82247058, Type6PanelScreenOpenPowerGraph)
ITC_TYPE67_SCREENS(822408B8, Type7PanelScreenOpenPowerGraph)
ITC_TYPE67_SCREENS(82240238, Type7PanelScreenCarList)
ITC_TYPE67_SCREENS(82240248, Type7PanelScreenCarEntry)
ITC_TYPE67_SCREENS(82240258, Type7PanelScreenCarSlot)
// The panel screens' handlers (through their machines' transitions).
// PanelScreen::DrawList sub_82248B58 has no adapter (removed, B7 loop,
// 2026-11-08): in play only Type7PanelScreen::DrawList (native) calls it;
// the other screens' draws that call it (sub_82230EF0, sub_82238DC8, never
// seen in play) run the original.
ITC_TYPE67_SCREENS(82249D18, PanelScreenOnEnter)
ITC_TYPE67_SCREENS(82248CD0, PanelScreenOpenMovie)
ITC_TYPE67_SCREENS(82249D88, PanelScreenWaitForMovie)
ITC_TYPE67_SCREENS(82249DF8, PanelScreenOpen)
ITC_TYPE67_SCREENS(82248CE0, PanelScreenShow)
ITC_TYPE67_SCREENS(82248C48, PanelScreenUpdate)
ITC_TYPE67_SCREENS(82248DF8, PanelScreenOnMessage24)
ITC_TYPE67_SCREENS(82249E38, PanelScreenLeave)
ITC_TYPE67_SCREENS(82248B08, PanelScreenOnLeft)
ITC_TYPE67_SCREENS(82240958, Type7PanelScreenDraw)
