// The type 12 mode (the game's Xbox Live mode) natively: adapters in front
// of src/network_mode/ (docs/functions/network-mode.md), with the register
// steps the harness shares (hooks/network_mode.h). Its Xbox Live calls stay
// recompiled, called through the guest (the user's call, 2026-11-12).

#include "hooks/network_mode.h"

#include "base/flags.h"
#include "network_mode/network_mode.h"

ITC_FLAG_BOOL(native_network_mode, true, "ITC",
              "Run the type 12 front-end mode (docs/functions/network-mode.md: the Xbox Live "
              "mode, its Live calls still through the translated code) natively instead of "
              "through the translated code: the machines' set-ups NetModeSelect::InitStates "
              "sub_8227D680, Type12Screen1::InitStates sub_82276510, Type12Screen3::InitStates "
              "sub_8225F330, NetRanking::InitStates sub_822873A0, NetTaCourse::InitStates "
              "sub_82281858, Type12Screen6 to 9::InitStates sub_82268410, sub_8226EB40, "
              "sub_82262A88 and sub_822646B0, Type12Dialog10 to 13::InitStates sub_82288600, "
              "sub_82277B18, sub_8227F6C8 and sub_8227EA50 and PowerGraphScreen12::InitStates "
              "sub_821B96B8, NetScreen::InitBase sub_8227B3E0 (with its jump sub_8227B41C) and "
              "the eight base Inits sub_82278C20, sub_82279118, sub_8227A000, sub_8227A4F8, "
              "sub_82279610, sub_82279B08, sub_8227A9F0 and sub_8227AEE8 (StateMachine::Add's "
              "plain C++) (checked by --run_harnesses=network_mode)");

namespace hooks {

namespace {

namespace nm = network_mode;

// StateMachine::Add's r3: a new transition's address from the machine's
// 64-bit r3, as the type 9 mode's Inits.
uint64_t AddR3(uint64_t machine_r3, uint32_t r3, bool replaced) {
  return replaced ? r3 : machine_r3 + (r3 - uint32_t(machine_r3));
}

template <uint32_t (*InitStates)(uint8_t*, uint32_t, uint32_t, bool*), uint32_t kFrame>
void InitStatesStep(PPCContext& ctx, uint8_t* base) {
  bool replaced = false;
  const uint32_t r3 = InitStates(base, ctx.r3.u32, ctx.r1.u32 - kFrame, &replaced);
  ctx.r3.u64 = AddR3(ctx.r3.u64, r3, replaced);
}

template <uint32_t (*InitBase)(uint8_t*, uint32_t, uint32_t, uint32_t, bool*)>
void InitBaseStep(PPCContext& ctx, uint8_t* base) {
  bool replaced = false;
  const uint32_t r3 =
      InitBase(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - nm::kInitBaseFrame, &replaced);
  ctx.r3.u64 = AddR3(ctx.r3.u64, r3, replaced);
}

}  // namespace

bool NetworkModeNative() { return ITC_FLAG(native_network_mode); }

#define ITC_NET_INIT_STATES(name, function, frame) \
  void name(PPCContext& ctx, uint8_t* base) { InitStatesStep<nm::function, nm::frame>(ctx, base); }
ITC_NET_INIT_STATES(NetModeSelectInitStates, ModeSelectInitStates, kModeSelectInitFrame)
ITC_NET_INIT_STATES(Type12Screen1InitStates, Screen1InitStates, kScreen1InitFrame)
ITC_NET_INIT_STATES(Type12Screen3InitStates, Screen3InitStates, kScreen3InitFrame)
ITC_NET_INIT_STATES(NetRankingInitStates, RankingInitStates, kRankingInitFrame)
ITC_NET_INIT_STATES(NetTaCourseInitStates, TaCourseInitStates, kTaCourseInitFrame)
ITC_NET_INIT_STATES(Type12Screen6InitStates, Screen6InitStates, kScreen6InitFrame)
ITC_NET_INIT_STATES(Type12Screen7InitStates, Screen7InitStates, kScreen7InitFrame)
ITC_NET_INIT_STATES(Type12Screen8InitStates, Screen8InitStates, kScreen8InitFrame)
ITC_NET_INIT_STATES(Type12Screen9InitStates, Screen9InitStates, kScreen9InitFrame)
ITC_NET_INIT_STATES(Type12Dialog10InitStates, Dialog10InitStates, kDialog10InitFrame)
ITC_NET_INIT_STATES(Type12Dialog11InitStates, Dialog11InitStates, kDialog11InitFrame)
ITC_NET_INIT_STATES(Type12Dialog12InitStates, Dialog12InitStates, kDialog12InitFrame)
ITC_NET_INIT_STATES(Type12Dialog13InitStates, Dialog13InitStates, kDialog13InitFrame)
ITC_NET_INIT_STATES(PowerGraphScreen12InitStates, PowerGraphInitStates, kPowerGraphInitFrame)
#undef ITC_NET_INIT_STATES

#define ITC_NET_INIT_BASE(name, function) \
  void name(PPCContext& ctx, uint8_t* base) { InitBaseStep<nm::function>(ctx, base); }
ITC_NET_INIT_BASE(Type12Screen6InitBase, Screen6InitBase)
ITC_NET_INIT_BASE(Type12Screen7InitBase, Screen7InitBase)
ITC_NET_INIT_BASE(Type12Screen8InitBase, Screen8InitBase)
ITC_NET_INIT_BASE(Type12Screen9InitBase, Screen9InitBase)
ITC_NET_INIT_BASE(Type12Dialog10InitBase, Dialog10InitBase)
ITC_NET_INIT_BASE(Type12Dialog11InitBase, Dialog11InitBase)
ITC_NET_INIT_BASE(Type12Dialog12InitBase, Dialog12InitBase)
ITC_NET_INIT_BASE(Type12Dialog13InitBase, Dialog13InitBase)
#undef ITC_NET_INIT_BASE

// r4 = r5 moved first; out of the table's range, r3 as it came.
void NetScreenInitBase(PPCContext& ctx, uint8_t* base) {
  bool replaced = false;
  uint32_t r3 = 0;
  if (nm::InitBase(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &replaced, &r3)) {
    ctx.r3.u64 = AddR3(ctx.r3.u64, r3, replaced);
  }
}

}  // namespace hooks

#define ITC_NETWORK_MODE(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_network_mode)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

// The fourteen InitStates: from ModeType12::Init (recompiled).
ITC_NETWORK_MODE(8227D680, NetModeSelectInitStates)
ITC_NETWORK_MODE(82276510, Type12Screen1InitStates)
ITC_NETWORK_MODE(8225F330, Type12Screen3InitStates)
ITC_NETWORK_MODE(822873A0, NetRankingInitStates)
ITC_NETWORK_MODE(82281858, NetTaCourseInitStates)
ITC_NETWORK_MODE(82268410, Type12Screen6InitStates)
ITC_NETWORK_MODE(8226EB40, Type12Screen7InitStates)
ITC_NETWORK_MODE(82262A88, Type12Screen8InitStates)
ITC_NETWORK_MODE(822646B0, Type12Screen9InitStates)
ITC_NETWORK_MODE(82288600, Type12Dialog10InitStates)
ITC_NETWORK_MODE(82277B18, Type12Dialog11InitStates)
ITC_NETWORK_MODE(8227F6C8, Type12Dialog12InitStates)
ITC_NETWORK_MODE(8227EA50, Type12Dialog13InitStates)
ITC_NETWORK_MODE(821B96B8, PowerGraphScreen12InitStates)
// NetScreen::InitBase and its base Inits: from the InitStates above (and
// the jump sub_8227B41C, recompiled, for case 6).
ITC_NETWORK_MODE(8227AEE8, Type12Dialog13InitBase)
