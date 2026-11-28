// The type 6 and type 7 modes' screen constructors' flag
// (src/screen_constructors/, docs/functions/screen-constructors.md): it
// switches the blocks' constructors in their only callers, the modes' slot
// 0 (native, src/hooks/type67_modes.cpp). The members have no adapters
// either: every caller they have in play is native and calls their plain
// C++.

#include "screen_constructors/screen_constructors.h"

#include "base/flags.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"
#include "lifted/sdk.h"
#include "memory/c_runtime_heap.h"

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
              "Type6Screen0::Type6Screen0 sub_82235BD8, GroupPair::GroupPair sub_82249C08, "
              "Type7Screen0::Type7Screen0 sub_82245FF8, Panel6C sub_82231CD0, Panel7C "
              "sub_822416D8, Panel6D sub_82231E40, Panel7D sub_822418B8, Panel6F sub_822308E8, "
              "Panel7F sub_82240268, PanelGroup sub_82240110, PanelGroup6B sub_82231C68, "
              "PanelGroup7B sub_82241680, PairedNodeTable::PairedNodeTable sub_82247C60 and "
              "Type7ListScreen::Type7ListScreen sub_82242BE8 with ColorListScreen sub_822550F8 "
              "and ColorNodeTable sub_82247D08, Type6Screens::Type6Screens sub_822366F0 with "
              "Type6ListScreen sub_822330F0, TitledListScreen sub_822570F0, NodeListScreen "
              "sub_82256F68, Type6NodeScreen sub_82236C30, NodeScreen sub_8225A3D8 (its stack "
              "words copied) and Type6PanelScreen sub_822309C8, Type7Screens::Type7Screens "
              "sub_82246B28 with Type7PanelScreen sub_822403A0) natively instead of through the "
              "translated code (the same stores; checked by "
              "--run_harnesses=screen_constructors)");

// No adapters (removed, B7 loop, 2026-11-07): the blocks' constructors'
// only callers, the modes' slot 0 (src/hooks/type67_modes.cpp), are native
// and call Type6Screens' and Type7Screens' plain C++ (the panels' operator
// new its plain C++, the original's call through the guest when it fails),
// or their originals with this flag off.

// For the fallback originals (docs/missing-functions.md): the stale
// originals round (2026-11-28), so lifted code calls the ports. Each
// original leaves `this` (r3) in r3.

namespace {

// The panels' operator new as the modes' slot 0 gives it
// (src/hooks/type67_modes.cpp): our C runtime heap, else the original's
// call through the guest from the caller's frame.
struct PanelNew {
  lifted::Regs* ctx;
  static uint32_t New(void* user, uint32_t size, uint32_t frame) {
    auto* p = static_cast<PanelNew*>(user);
    if (auto block = memory::crt::OperatorNew(size)) {
      return *block;
    }
    lifted::Regs& ctx = *p->ctx;
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = size;
    lifted::Sdk(ctx, lifted::original::c825A6880);
    const uint32_t block = ctx.r3.u32;
    ctx.r1.u64 = stack;
    ctx.lr = link;
    return block;
  }
};

template <void (*Construct)(uint32_t, uint32_t, const screen_constructors::New&)>
void WithNew(lifted::Regs& ctx) {
  const uint64_t self = ctx.r3.u64;
  PanelNew panel_new{&ctx};
  const screen_constructors::New new_panel{&panel_new, PanelNew::New};
  Construct(ctx.r3.u32, ctx.r1.u32, new_panel);
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = self;
}

template <void (*Construct)(uint32_t)>
void Plain(lifted::Regs& ctx) {
  Construct(ctx.r3.u32);
}

}  // namespace

ITC_DIRECT_ENTRY(822309C8) { WithNew<screen_constructors::Type6PanelScreen>(ctx); }
ITC_DIRECT_ENTRY(822403A0) { WithNew<screen_constructors::Type7PanelScreen>(ctx); }
ITC_DIRECT_ENTRY(822366F0) { WithNew<screen_constructors::Type6Screens>(ctx); }
ITC_DIRECT_ENTRY(82246B28) { WithNew<screen_constructors::Type7Screens>(ctx); }
ITC_DIRECT_ENTRY(822330F0) { Plain<screen_constructors::Type6ListScreen>(ctx); }
ITC_DIRECT_ENTRY(82242BE8) { Plain<screen_constructors::Type7ListScreen>(ctx); }
ITC_DIRECT_ENTRY(82235BD8) { Plain<screen_constructors::Type6Screen0>(ctx); }
ITC_DIRECT_ENTRY(82245FF8) { Plain<screen_constructors::Type7Screen0>(ctx); }
ITC_DIRECT_ENTRY(82236C30) {
  screen_constructors::Type6NodeScreen(ctx.r3.u32, ctx.r1.u32);
}
