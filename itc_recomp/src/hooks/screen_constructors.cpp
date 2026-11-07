// The type 6 and type 7 modes' screen constructors natively: adapters in
// front of src/screen_constructors/ (docs/functions/screen-constructors.md)
// for the two blocks' constructors, which the modes' slot 0 (recompiled)
// calls; each returns its r3 (the block). The members have no adapters (the
// flag switches the blocks' constructors): every caller they have in play
// is native and calls their plain C++. The panel screens' four panels come
// from operator new's plain C++ (the original's call, through the guest,
// when it fails: the new handler and std::bad_alloc).

#include "screen_constructors/screen_constructors.h"

#include "base/flags.h"
#include "guest.h"
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


namespace {

// The panels' operator new: its plain C++, else the original's call (from
// the panel screen's frame) for its failure path.
struct AdapterNew {
  PPCContext* ctx;
  uint8_t* base;
};

uint32_t NewPanel(void* user, uint32_t size, uint32_t frame) {
  auto* a = static_cast<AdapterNew*>(user);
  if (auto block = memory::crt::OperatorNew(a->base, size)) {
    return *block;
  }
  PPCContext& ctx = *a->ctx;
  const uint64_t stack = ctx.r1.u64, link = ctx.lr;
  ctx.r1.u64 = frame;
  ctx.r3.u64 = size;
  __imp__sub_825A6880(ctx, a->base);
  const uint32_t block = ctx.r3.u32;
  ctx.r1.u64 = stack;
  ctx.lr = link;
  return block;
}

// The blocks' constructors (r3 the block, returned) from the registers.
void BlockStep(PPCContext& ctx, uint8_t* base,
               void (*construct)(uint8_t*, uint32_t, uint32_t, const sc::New&)) {
  const uint64_t r3 = ctx.r3.u64;
  AdapterNew user{&ctx, base};
  construct(base, ctx.r3.u32, ctx.r1.u32, sc::New{&user, NewPanel});
  ctx.r3.u64 = r3;
}

}  // namespace

ITC_SCREEN_CONSTRUCTOR(822366F0, BlockStep(ctx, base, sc::Type6Screens))
ITC_SCREEN_CONSTRUCTOR(82246B28, BlockStep(ctx, base, sc::Type7Screens))
