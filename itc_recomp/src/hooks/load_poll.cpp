// The scene models' load poll natively: adapters in front of src/load_poll/
// (docs/functions/load-poll.md), with the register steps the harness shares
// (hooks/load_poll.h). The finishers and apply steps the diagnostics wrap
// (--find_per_frame_state, --debug_hide) have their adapters there, in
// src/debug/frame_state_diff.cpp, calling these steps.

#include "hooks/load_poll.h"

#include "base/flags.h"
#include "hooks/second_set.h"

ITC_FLAG_DECLARE(bool, native_second_set);
ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_car_models);

ITC_FLAG_BOOL(native_load_poll, true, "ITC",
              "Run the scene models' load poll (docs/functions/load-poll.md: Wheels::SetRecord "
              "sub_820D8EC0 and SetCaliperTexture sub_820D8ED0, CarModel::IsReady sub_820D4048 "
              "and CarParts::SetVinylTextures sub_820D72D8 (inside the diagnostics' wraps), "
              "InteriorDials::SetBlock sub_820E3040, SceneInterior::SetGauges sub_820E3938 "
              "(with InteriorMeters::SetGauge sub_820E32E8 and sub_820D3658, never seen in play, "
              "no adapters), SceneLights::Finish sub_820DDA60 and ApplyRecord sub_820DCBC0 "
              "(inside the wraps; with SetRecord sub_820DCB70, no adapter) and "
              "SceneTraffic::Finish sub_820E5A68 (inside the wrap)) natively instead of through "
              "the translated code, with --native_second_set, --native_scene_loaders, "
              "--native_car_models, native memcpy, --native_models and --native_material_passes "
              "(else the originals; the same writes; checked by --run_harnesses=load_poll)");

namespace hooks {

namespace lp = load_poll;

bool LoadPollCalleesNative() {
  return ITC_FLAG(native_second_set) && ITC_FLAG(native_scene_loaders) &&
         ITC_FLAG(native_car_models) && SecondSetFindsNative();
}

namespace {

// A call to an original from the plain C++'s frame at `sp`, r1 and lr put
// back after, the flush mode left off.
template <typename Body>
void FromFrame(PPCContext& ctx, uint32_t sp, uint32_t lr, Body body) {
  const uint64_t stack = ctx.r1.u64, link = ctx.lr;
  ctx.r1.u64 = sp;
  ctx.lr = lr;
  body();
  ctx.r1.u64 = stack;
  ctx.lr = link;
  ctx.fpscr.disableFlushMode();
}

// The flush mode as the plain C++'s last float step left it.
void LeaveFlush(PPCContext& ctx, const lp::Calls& calls) {
  if (calls.find.flush) {
    ctx.fpscr.enableFlushMode();
  } else {
    ctx.fpscr.disableFlushMode();
  }
}

}  // namespace

lp::Calls LoadPollCalls(PPCContext& ctx, uint8_t* base) {
  lp::Calls calls;
  calls.find = SecondSetFindCalls(ctx, base);
  calls.memcpy = [&ctx, base](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp,
                              uint32_t lr) {
    FromFrame(ctx, sp, lr, [&] {
      ctx.r3.u64 = to;
      ctx.r4.u64 = from;
      ctx.r5.u64 = bytes;
      __imp__sub_825A7410(ctx, base);
    });
  };
  calls.cockpit_bind = [&ctx, base](uint32_t object, uint32_t sp, uint32_t lr) {
    uint64_t r3 = 0;
    FromFrame(ctx, sp, lr, [&] {
      ctx.r3.u64 = object;
      __imp__sub_820D6108(ctx, base);
      r3 = ctx.r3.u64;
    });
    return r3;
  };
  calls.traffic_bind = [&ctx, base](uint32_t object, uint32_t sp, uint32_t lr) {
    uint64_t r3 = 0;
    FromFrame(ctx, sp, lr, [&] {
      ctx.r3.u64 = object;
      __imp__sub_820D7D38(ctx, base);
      r3 = ctx.r3.u64;
    });
    return r3;
  };
  return calls;
}

#define ITC_LOAD_POLL_ORIGINAL(address)   \
  if (!LoadPollCalleesNative()) {         \
    __imp__sub_##address(ctx, base);      \
    return;                               \
  }                                       \
  ctx.fpscr.disableFlushMode();

void LoadPollWheelsSetRecord(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820D8EC0)
  lp::Calls calls = LoadPollCalls(ctx, base);
  ctx.r3.u64 = lp::WheelsSetRecord(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
}

void LoadPollWheelsSetCaliperTexture(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820D8ED0)
  ctx.r3.u64 = lp::WheelsSetCaliperTexture(base, ctx.r3.u32, ctx.r1.u32);
}

void LoadPollCarModelIsReady(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820D4048)
  ctx.r3.u64 = lp::CarModelIsReady(base, ctx.r3.u32);
}

void LoadPollCarPartsSetVinylTextures(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820D72D8)
  ctx.r3.u64 = lp::CarPartsSetVinylTextures(base, ctx.r3.u32, ctx.r3.u64, ctx.r1.u32);
}

void LoadPollInteriorDialsSetBlock(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820E3040)
  ctx.r3.u64 = lp::InteriorDialsSetBlock(base, ctx.r3.u32, ctx.r4.u64);
}

void LoadPollInteriorMetersSetGauge(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820E32E8)
  ctx.r3.u64 = lp::InteriorMetersSetGauge(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32);
}

void LoadPollSceneInteriorSetGauges(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820E3938)
  ctx.r3.u64 = lp::SceneInteriorSetGauges(base, ctx.r3.u32, ctx.r3.u64, ctx.r1.u32);
}

void LoadPollSceneLightsFinish(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820DDA60)
  lp::Calls calls = LoadPollCalls(ctx, base);
  ctx.r3.u64 = lp::SceneLightsFinish(base, ctx.r3.u32, ctx.r1.u32, calls);
  LeaveFlush(ctx, calls);
}

void LoadPollSceneLightsSetRecord(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820DCB70)
  lp::Calls calls = LoadPollCalls(ctx, base);
  ctx.r3.u64 = lp::SceneLightsSetRecord(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
}

void LoadPollSceneLightsApplyRecord(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820DCBC0)
  lp::Calls calls = LoadPollCalls(ctx, base);
  ctx.r3.u64 = lp::SceneLightsApplyRecord(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
}

void LoadPollSceneTrafficFinish(PPCContext& ctx, uint8_t* base) {
  ITC_LOAD_POLL_ORIGINAL(820E5A68)
  lp::Calls calls = LoadPollCalls(ctx, base);
  ctx.r3.u64 = lp::SceneTrafficFinish(base, ctx.r3.u32, ctx.r1.u32, calls);
  LeaveFlush(ctx, calls);
}

#undef ITC_LOAD_POLL_ORIGINAL

}  // namespace hooks

#define ITC_LOAD_POLL(address, step)    \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_load_poll)) {  \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

// No adapters (only their callers here call them; the steps above are the
// harness's): InteriorMeters::SetGauge sub_820E32E8, SceneLights::SetRecord
// sub_820DCB70. In the diagnostics' wraps (src/debug/frame_state_diff.cpp):
// CarModel::IsReady sub_820D4048, CarParts::SetVinylTextures sub_820D72D8,
// SceneLights::Finish sub_820DDA60 and ApplyRecord sub_820DCBC0,
// SceneTraffic::Finish sub_820E5A68.
ITC_LOAD_POLL(820D8EC0, LoadPollWheelsSetRecord)
ITC_LOAD_POLL(820D8ED0, LoadPollWheelsSetCaliperTexture)
ITC_LOAD_POLL(820E3040, LoadPollInteriorDialsSetBlock)
ITC_LOAD_POLL(820E3938, LoadPollSceneInteriorSetGauges)
