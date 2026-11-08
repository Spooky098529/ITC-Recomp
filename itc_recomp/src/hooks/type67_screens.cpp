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
