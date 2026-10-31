// The race scene natively: adapters in front of src/race_scene/
// (docs/functions/race-scene.md), with the register steps the harness
// shares (hooks/race_scene.h).

#include "hooks/race_scene.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "race_scene/race_scene.h"

ITC_FLAG_BOOL(native_race_scene, true, "ITC",
              "Run the race scene (docs/functions/race-scene.md: RaceSceneMachine::Init "
              "sub_8215D808 with RaceScene::Create sub_8215D738 and RaceScene::RaceScene "
              "sub_8215D6C8, which have no adapters: only Init's plain C++ calls them, with "
              "--native_heap; RaceLights::SetUp sub_8215CF78 with RaceLights::InitEntry "
              "sub_8215C368, no adapter: only SetUp's plain C++ calls it) natively instead of through the translated code (the same words, "
              "bit for bit; checked by --run_harnesses=race_scene)");

namespace hooks {

namespace rs = race_scene;

void RaceSceneMachineInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const auto r3 = NativeHeapActive()
                      ? rs::MachineInit(base, machine, ctx.r1.u32 - rs::kInitFrame, &replaced)
                      : std::nullopt;
  if (!r3) {
    __imp__sub_8215D808(ctx, base);
    return;
  }
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? *r3 : machine_r3 + (*r3 - machine);
}

void RaceSceneLightsSetUp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::LightsSetUp(base, ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u64 + 16 + 96 * 10;
  ctx.f1.f64 = rs::LightsLastF1(base);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_SCENE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_race_scene)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

ITC_RACE_SCENE(8215D808, RaceSceneMachineInit)
ITC_RACE_SCENE(8215CF78, RaceSceneLightsSetUp)
// RaceLights::InitEntry (sub_8215C368) has no adapter: its only caller,
// RaceLights::SetUp, is native; --native_race_scene switches it.
// RaceScene::Create (sub_8215D738) and RaceScene::RaceScene (sub_8215D6C8)
// have no adapters: their only caller, RaceSceneMachine::Init, is native;
// --native_race_scene switches it.
