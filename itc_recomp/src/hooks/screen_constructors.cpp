// The type 6 and type 7 modes' screen constructors natively: adapters in
// front of src/screen_constructors/ (docs/functions/screen-constructors.md).
// Each original returns its r3 (the object), which the adapters leave as it
// is. Functions reached only from native callers have no adapters (the flag
// switches their callers): NodeBase::NodeBase (sub_82258FB0), from
// PanelGroupBase::PanelGroupBase; PanelSetBase::PanelSetBase
// (sub_8224AD28), from PanelSet::PanelSet; PanelEBase::PanelEBase
// (sub_8224EF78), from PanelE::PanelE; ListPartBase::ListPartBase,
// ListPartInnerBase::ListPartInnerBase and ListPartInner::ListPartInner
// (sub_822523F8, sub_822525E0, sub_822526A0), from ListPart::ListPart.

#include "screen_constructors/screen_constructors.h"

#include "base/flags.h"
#include "guest.h"

ITC_FLAG_BOOL(native_screen_constructors, true, "ITC",
              "Build the type 6 and type 7 modes' screens "
              "(docs/functions/screen-constructors.md: PanelSet::PanelSet sub_8224B660 with "
              "PanelSetBase::PanelSetBase sub_8224AD28 (no adapter: only PanelSet calls it; its "
              "stack words copied), PanelE::PanelE sub_82230800 with PanelEBase::PanelEBase "
              "sub_8224EF78, ListScreen::ListScreen sub_82252290, ListPart::ListPart "
              "sub_82252908 with ListPartBase sub_822523F8, ListPartInnerBase sub_822525E0 and "
              "ListPartInner sub_822526A0, PanelCBase::PanelCBase sub_8224D6D0, "
              "PanelGroupBase::PanelGroupBase sub_8224EEA8 with NodeBase::NodeBase sub_82258FB0, "
              "PanelFBase::PanelFBase sub_8224FA88, NodeTable::NodeTable sub_82247BE8, "
              "PanelDBase::PanelDBase sub_8224CFA0, MachineScreen::MachineScreen sub_8224CF58, "
              "Type6Screen0::Type6Screen0 sub_82235BD8, GroupPair::GroupPair sub_82249C08 and "
              "Type7Screen0::Type7Screen0 sub_82245FF8) natively instead of through the "
              "translated code (the same stores; checked by "
              "--run_harnesses=screen_constructors)");

namespace sc = screen_constructors;

// Defines an adapter: the flag, else the original; `body` with ctx and base.
#define ITC_SCREEN_CONSTRUCTOR(address, body)       \
  extern "C" REX_FUNC(sub_##address) {              \
    REX_FUNC_PROLOGUE();                            \
    if (!ITC_FLAG(native_screen_constructors)) {    \
      __imp__sub_##address(ctx, base);              \
      return;                                       \
    }                                               \
    body;                                           \
  }

ITC_SCREEN_CONSTRUCTOR(8224B660, sc::PanelSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                              ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r1.u32))
ITC_SCREEN_CONSTRUCTOR(82230800, sc::PanelE(base, ctx.r3.u32, ctx.r4.u32))
ITC_SCREEN_CONSTRUCTOR(82252290, sc::ListScreen(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(82252908, sc::ListPart(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(8224D6D0, sc::PanelCBase(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(8224EEA8, sc::PanelGroupBase(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(8224FA88, sc::PanelFBase(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(82247BE8, sc::NodeTable(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(8224CFA0, sc::PanelDBase(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(8224CF58, sc::MachineScreen(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(82235BD8, sc::Type6Screen0(base, ctx.r3.u32))
ITC_SCREEN_CONSTRUCTOR(82249C08, sc::GroupPair(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_SCREEN_CONSTRUCTOR(82245FF8, sc::Type7Screen0(base, ctx.r3.u32))
