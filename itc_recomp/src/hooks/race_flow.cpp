// The race flow natively: the adapter in front of src/race_flow/
// (docs/functions/race-flow.md), with the register step the harness shares
// (hooks/race_flow.h).

#include "hooks/race_flow.h"

#include "base/flags.h"
#include "race_flow/race_flow.h"

ITC_FLAG_BOOL(native_race_flow, true, "ITC",
              "Run the race flow's set-up (docs/functions/race-flow.md: RaceFlow::Init "
              "sub_82160AE0, the 53 transitions' words replayed in its frame, "
              "StateMachine::Add's plain C++) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=race_flow)");

namespace hooks {

void RaceFlowInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 =
      race_flow::Init(base, machine, ctx.r1.u32 - race_flow::kInitFrame, &replaced);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

// The adapter: the register step above, or the original with the flag off.
// Its only caller, RaceMode::Init sub_82151D20, is recompiled.
extern "C" REX_FUNC(sub_82160AE0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_flow)) {
    __imp__sub_82160AE0(ctx, base);
    return;
  }
  hooks::RaceFlowInit(ctx, base);
}
