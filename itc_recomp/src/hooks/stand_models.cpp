// The stand's scene model holders natively: adapters in front of
// src/stand_models/ (docs/functions/stand-models.md), with the register
// steps the harness shares (hooks/stand_models.h).

#include "hooks/stand_models.h"

#include "base/flags.h"

ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_scene_models);

ITC_FLAG_BOOL(native_stand_models, true, "ITC",
              "Run the stand's scene model holders (docs/functions/stand-models.md): "
              "GarageStage::MakeModel sub_8217FCB0 (with --native_vehicle_accessors and "
              "--native_scene_models; ScenePools::SetUpB through its adapter), "
              "GarageStage::SetInteriorClass sub_82178FE8, SetWord5016 sub_82179598, "
              "SetWheelWord sub_82179A18, SetLightsOn sub_82179420 (with SceneParts::LightValue "
              "sub_820DF1A0, no adapter: only it calls it), SetDecalA sub_82179818 and SetDecalB "
              "sub_821798D8 (with --native_vehicle_accessors); the setters that re-apply the record, "
              "SetPart sub_82178F00 (its case 0 sub_82178F68 merged), SetInteriorWord sub_82179050, "
              "SetInteriorLevel sub_821791C0, SetInteriorPair sub_82179250, ClearChoice "
              "sub_82179378, SetAero sub_82179548 and ReloadDecals sub_82178EC0 (with "
              "--native_vehicle_accessors and --native_scene_models; SceneModel::ApplyRecord's "
              "and ReloadDecals' plain C++) natively instead of through the translated code (the same writes; checked by --run_harnesses=stand_models)");

namespace hooks {

namespace sm = stand_models;

stand_models::GuestCall StandModelsGuestCall(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t function, std::initializer_list<uint64_t> args, uint32_t frame,
                      uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    PPCRegister* registers[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                                 &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    int i = 0;
    for (const uint64_t arg : args) {
      registers[i++]->u64 = arg;
    }
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
}

void StandModelsMakeModel(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors) || !ITC_FLAG(native_scene_models)) {
    __imp__sub_8217FCB0(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::GuestCall call = StandModelsGuestCall(ctx, base);
  const uint64_t r3 = sm::MakeModel(base, call, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// A setter over the stage's model: r3 the holder, r4 a value.
#define ITC_STAND_SETTER(Name, address)                                          \
  void StandModels##Name(PPCContext& ctx, uint8_t* base) {                       \
    if (!ITC_FLAG(native_vehicle_accessors)) {                                   \
      __imp__sub_##address(ctx, base);                                           \
      return;                                                                    \
    }                                                                            \
    ctx.fpscr.disableFlushMode();                                                \
    ctx.r3.u64 = sm::Name(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);             \
  }

ITC_STAND_SETTER(SetInteriorClass, 82178FE8)
ITC_STAND_SETTER(SetWord5016, 82179598)
ITC_STAND_SETTER(SetWheelWord, 82179A18)

#undef ITC_STAND_SETTER

void StandModelsSetLightsOn(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors)) {
    __imp__sub_82179420(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = sm::SetLightsOn(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
}

void StandModelsSetDecalA(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors)) {
    __imp__sub_82179818(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sm::SetDecal(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                            ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.r1.u32, sm::kDecalA);
}

void StandModelsSetDecalB(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors)) {
    __imp__sub_821798D8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sm::SetDecal(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                            ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.r1.u32, sm::kDecalB);
}

// A setter that re-applies the record: `body` with the guest calls, lr put
// back; the original without its callees' flags.
template <typename Body>
void Reapplying(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
                Body body) {
  if (!ITC_FLAG(native_vehicle_accessors) || !ITC_FLAG(native_scene_models)) {
    original(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::GuestCall call = StandModelsGuestCall(ctx, base);
  const uint32_t holder = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, stack = ctx.r1.u32;
  const uint64_t r3 = body(call, holder, r4, r5, stack);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void StandModelsSetPart(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_82178F00, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t r4, uint32_t r5, uint32_t stack) {
    return sm::SetPart(base, call, holder, r4, r5, stack);
  });
}

void StandModelsSetInteriorWord(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_82179050, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t r4, uint32_t r5, uint32_t stack) {
    return sm::SetInteriorWord(base, call, holder, r4, r5, stack);
  });
}

void StandModelsSetInteriorLevel(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_821791C0, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t r4, uint32_t, uint32_t stack) {
    return sm::SetInteriorLevel(base, call, holder, r4, stack);
  });
}

void StandModelsSetInteriorPair(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_82179250, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t r4, uint32_t r5, uint32_t stack) {
    return sm::SetInteriorPair(base, call, holder, r4, r5, stack);
  });
}

void StandModelsClearChoice(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_82179378, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t r4, uint32_t, uint32_t stack) {
    return sm::ClearChoice(base, call, holder, r4, stack);
  });
}

void StandModelsSetAero(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_82179548, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t r4, uint32_t, uint32_t stack) {
    return sm::SetAero(base, call, holder, r4, stack);
  });
}

void StandModelsReloadDecals(PPCContext& ctx, uint8_t* base) {
  Reapplying(ctx, base, __imp__sub_82178EC0, [&](const sm::GuestCall& call, uint32_t holder,
                                                uint32_t, uint32_t, uint32_t stack) {
    return sm::ReloadDecals(base, call, holder, stack);
  });
}

void StandModelsLightValue(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = sm::LightValue(base, ctx.r3.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_STAND_MODELS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_stand_models)) {   \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_STAND_MODELS(8217FCB0, StandModelsMakeModel)
ITC_STAND_MODELS(82178FE8, StandModelsSetInteriorClass)
ITC_STAND_MODELS(82179598, StandModelsSetWord5016)
ITC_STAND_MODELS(82179A18, StandModelsSetWheelWord)
ITC_STAND_MODELS(82179420, StandModelsSetLightsOn)
ITC_STAND_MODELS(82179818, StandModelsSetDecalA)
ITC_STAND_MODELS(821798D8, StandModelsSetDecalB)
ITC_STAND_MODELS(82178F00, StandModelsSetPart)
ITC_STAND_MODELS(82179050, StandModelsSetInteriorWord)
ITC_STAND_MODELS(821791C0, StandModelsSetInteriorLevel)
ITC_STAND_MODELS(82179250, StandModelsSetInteriorPair)
ITC_STAND_MODELS(82179378, StandModelsClearChoice)
ITC_STAND_MODELS(82179548, StandModelsSetAero)
ITC_STAND_MODELS(82178EC0, StandModelsReloadDecals)
// sub_82178F68 (SetPart's case 0, split off by the recompiler) needs no
// adapter: only the original SetPart reaches it, as a tail call.
// SceneParts::LightValue sub_820DF1A0 has none: only SetLightsOn calls it,
// and its plain C++ calls LightValue's (its register step above is the
// harness's).
