// The race scene natively: adapters in front of src/race_scene/
// (docs/functions/race-scene.md), with the register steps the harness
// shares (hooks/race_scene.h).

#include "hooks/race_scene.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "race_scene/race_scene.h"

ITC_FLAG_BOOL(native_race_scene, true, "ITC",
              "Run the race scene (docs/functions/race-scene.md: RaceSceneMachine::Init "
              "sub_8215D808 with RaceScene::Create sub_8215D738 and RaceScene::RaceScene "
              "sub_8215D6C8, which have no adapters: only Init's plain C++ calls them, with "
              "--native_heap; RaceLights::SetUp sub_8215CF78 with RaceLights::InitEntry "
              "sub_8215C368, no adapter: only SetUp's plain C++ calls it; RaceScene::SetEffectParams "
              "sub_8228F5C0 with Glows::SetParams sub_8228F0B0, no adapter: only SetEffectParams' "
              "plain C++ calls it, the glows' and color filters' setters' plain C++ called where "
              "their vtables hold them, else the original; RaceSceneMachine::TogglePause sub_8215C750 "
              "and Resume sub_8215D0D8 with Race::ToggleEffects' plain C++; RaceScene::Destroy "
              "sub_8215D790 with DestroyCars sub_8215D548, no adapter: only Destroy's plain C++ "
              "calls it, with --native_heap) natively instead of through the translated code (the same words, "
              "bit for bit; checked by --run_harnesses=race_scene)");

namespace hooks {

namespace rs = race_scene;

ITC_REGS_STEP(RaceSceneMachineInit) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const auto r3 = NativeHeapActive()
                      ? rs::MachineInit(machine, ctx.r1.u32 - rs::kInitFrame, &replaced)
                      : std::nullopt;
  if (!r3) {
    lifted::original::c8215D808(ctx);
    return;
  }
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? *r3 : machine_r3 + (*r3 - machine);
}

ITC_REGS_STEP(RaceSceneLightsSetUp) {
  ctx.fpscr.disableFlushMode();
  rs::LightsSetUp(ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u64 + 16 + 96 * 10;
  ctx.f1.f64 = rs::LightsLastF1();
}

ITC_REGS_STEP(RaceSceneSetEffectParams) {
  ctx.fpscr.disableFlushMode();
  if (!rs::EffectParamsReady()) {
    lifted::original::c8228F5C0(ctx);
    return;
  }
  double f1 = 0;
  ctx.r3.u64 = rs::SetEffectParams(ctx.r3.u32, ctx.r4.u32, &f1);
  ctx.f1.f64 = f1;
}

ITC_REGS_STEP(RaceSceneTogglePause) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::TogglePause(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSceneResume) {
  ctx.fpscr.disableFlushMode();
  if (const auto r3 = rs::Resume(ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  }
}

ITC_REGS_STEP(RaceSceneDestroy) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive() || !rs::DestroyReady()) {
    lifted::original::c8215D790(ctx);
    return;
  }
  if (const uint32_t scene = rs::Destroy()) {
    ctx.r3.u64 = scene;
  }
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_SCENE(address, step)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_scene)) { \
    hooks::step(ctx);              \
  }

ITC_RACE_SCENE(8215D808, RaceSceneMachineInit)
ITC_RACE_SCENE(8215CF78, RaceSceneLightsSetUp)
ITC_RACE_SCENE(8228F5C0, RaceSceneSetEffectParams)
// The machine's handlers are reached through their functors (no direct
// callers), so their adapters stay.
ITC_RACE_SCENE(8215C750, RaceSceneTogglePause)
ITC_RACE_SCENE(8215D0D8, RaceSceneResume)
ITC_RACE_SCENE(8215D790, RaceSceneDestroy)
// RaceScene::DestroyCars (sub_8215D548) has no adapter: its only caller,
// RaceScene::Destroy, is native; --native_race_scene switches it.
// Glows::SetParams (sub_8228F0B0) has no adapter: its only caller,
// RaceScene::SetEffectParams, is native; --native_race_scene switches it.
// RaceLights::InitEntry (sub_8215C368) has no adapter: its only caller,
// RaceLights::SetUp, is native; --native_race_scene switches it.
// RaceScene::Create (sub_8215D738) and RaceScene::RaceScene (sub_8215D6C8)
// have no adapters: their only caller, RaceSceneMachine::Init, is native;
// --native_race_scene switches it.
