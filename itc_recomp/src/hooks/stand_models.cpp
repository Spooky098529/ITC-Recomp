// The stand's scene model holders natively: adapters in front of
// src/stand_models/ (docs/functions/stand-models.md), with the register
// steps the harness shares (hooks/stand_models.h).

#include "hooks/stand_models.h"

#include "base/flags.h"
#include "hooks/load_poll.h"
#include "load_poll/load_poll.h"
#include "scene_models/apply.h"

ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_load_poll);
ITC_FLAG_DECLARE(bool, native_scene_models);
ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_math);
ITC_FLAG_DECLARE(bool, native_car_specs);
ITC_FLAG_DECLARE(bool, native_race_setup);

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
              "and ReloadDecals' plain C++); the tire grades, GarageStage::SetTireGrade "
              "sub_82179130 with SceneWheels::SetTireGrades sub_820E7470 and "
              "Wheels::SetTireTextures sub_820D8F18 (no adapters: only it reaches them) and "
              "TuningScreen::SetTireGrade sub_821CCA38 (with --native_vehicle_accessors; "
              "Wheels::SetRecord and PartModel::SetTexture through their adapters); the second "
              "wheels, TuningScreen::StepSecondWheels sub_821CC890 with "
              "GarageStage::SecondWheelsPending sub_82179C48, AddSecondWheels sub_82179998, "
              "ApplyWheelPaint sub_821799D8 and SceneModel::AddSecondWheels sub_820D9E00 (no "
              "adapters: only it reaches them; with --native_vehicle_accessors and "
              "--native_scene_models; ScenePools::SetUpB through its adapter, "
              "SceneModel::ApplyWheelPaint's plain C++ with the load poll's flags); the wheels picked, TuningScreen::PreviewWheel sub_821DFA78 with "
              "GarageStage::SetWheel sub_8217DD38, GarageStage::FitAxles sub_82178B98 and "
              "SceneWheels::SetSizes sub_820E7380 (no adapters: only it reaches them; with "
              "--native_vehicle_accessors, --native_car_specs, --native_car_physics, --native_math "
              "and --native_race_setup; Car::Refresh, memcpy, Car::SetupVehicle and "
              "Wheels::SetRecord through their adapters where their plain C++ declines); the paint, "
              "GarageStage::SetColor sub_8217C6C8 and its pieces SetColor0 to SetColor5 "
              "sub_8217B078 to sub_8217B348 (no adapters: the tuning screen's native UpdateStand "
              "and RestoreStand call them; SceneModel::ApplyPaint's and ApplyWheelPaint's plain "
              "C++ with the load poll's flags); the decals the tuning screen's never-seen paths set, "
              "GarageStage::SetSticker sub_8217B3D8 and SetDecalWord sub_821793D0 (no adapters: "
              "its native ApplyAreaDecals and ApplyDecalWord call them) natively instead of through the translated code (the same writes; checked by --run_harnesses=stand_models)");

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

// A step with the guest calls and lr put back: `body`'s r3.
// SceneModel::ApplyWheelPaint's plain C++ (the load poll's flags, as its
// old wrap in src/debug/frame_state_diff.cpp ran it); else its original.
bool ApplyWheelPaintNative() {
  return ITC_FLAG(native_load_poll) && LoadPollCalleesNative();
}

template <typename Body>
void WithCalls(PPCContext& ctx, uint8_t* base, Body body) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::GuestCall call = StandModelsGuestCall(ctx, base);
  const uint64_t r3 = body(call);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void StandModelsSetTireGrade(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors)) {
    __imp__sub_82179130(ctx, base);
    return;
  }
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SetTireGrade(base, call, r3, r4, r5, stack);
  });
}

void StandModelsTuningSetTireGrade(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors)) {
    __imp__sub_821CCA38(ctx, base);
    return;
  }
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t r4 = ctx.r4.u32, r5 = ctx.r5.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::TuningScreenSetTireGrade(base, call, r3, r4, r5, stack);
  });
}

void StandModelsSetTireGrades(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SceneWheelsSetTireGrades(base, call, r3, r4, r5, stack);
  });
}

void StandModelsSetTireTextures(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::WheelsSetTireTextures(base, call, r3, stack);
  });
}

void StandModelsStepSecondWheels(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors) || !ITC_FLAG(native_scene_models)) {
    __imp__sub_821CC890(ctx, base);
    return;
  }
  const uint32_t r3 = ctx.r3.u32, stack = ctx.r1.u32;
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  load_poll::Calls* native_poll = ApplyWheelPaintNative() ? &poll : nullptr;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::StepSecondWheels(base, call, native_poll, r3, stack);
  });
}

void StandModelsSecondWheelsPending(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sm::SecondWheelsPending(base, ctx.r3.u32, ctx.r1.u32);
}

void StandModelsAddSecondWheels(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::AddSecondWheels(base, call, r3, stack);
  });
}

void StandModelsApplyWheelPaint(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, stack = ctx.r1.u32;
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  load_poll::Calls* native_poll = ApplyWheelPaintNative() ? &poll : nullptr;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::ApplyWheelPaint(base, call, native_poll, r3, stack);
  });
}

void StandModelsSceneAddSecondWheels(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return scene_models::SceneModelAddSecondWheels(base, call, r3, stack);
  });
}

void StandModelsFitAxles(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_car_physics) || !ITC_FLAG(native_math)) {
    __imp__sub_82178B98(ctx, base);
    return;
  }
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, stack = ctx.r1.u32;
  std::optional<double> f1;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::FitAxles(base, call, r3, r4, stack, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
}

void StandModelsSetSizes(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, stack = ctx.r1.u32;
  const uint64_t r6 = ctx.r6.u64, r7 = ctx.r7.u64, r8 = ctx.r8.u64, r9 = ctx.r9.u64;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SceneWheelsSetSizes(base, call, r3, r4, r5, r6, r7, r8, r9, stack);
  });
}

bool WheelCalleesNative() {
  return ITC_FLAG(native_vehicle_accessors) && ITC_FLAG(native_car_specs) &&
         ITC_FLAG(native_car_physics) && ITC_FLAG(native_math);
}

void StandModelsSetWheel(PPCContext& ctx, uint8_t* base) {
  if (!WheelCalleesNative()) {
    __imp__sub_8217DD38(ctx, base);
    return;
  }
  const uint32_t r3 = ctx.r3.u32, r5 = ctx.r5.u32, r6 = ctx.r6.u32, stack = ctx.r1.u32;
  const uint64_t r4 = ctx.r4.u64;
  std::optional<double> f1;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SetWheel(base, call, r3, r4, r5, r6, stack, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
}

void StandModelsPreviewWheel(PPCContext& ctx, uint8_t* base) {
  if (!WheelCalleesNative() || !ITC_FLAG(native_race_setup)) {
    __imp__sub_821DFA78(ctx, base);
    return;
  }
  const uint32_t r3 = ctx.r3.u32, r5 = ctx.r5.u32, stack = ctx.r1.u32;
  const uint64_t r4 = ctx.r4.u64;
  std::optional<double> f1;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::PreviewWheel(base, call, r3, r4, r5, stack, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
}

void StandModelsSetColor(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t r4 = ctx.r4.u32, stack = ctx.r1.u32;
  const double f1 = ctx.f1.f64, f2 = ctx.f2.f64, f3 = ctx.f3.f64;
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  load_poll::Calls* native_poll = ApplyWheelPaintNative() ? &poll : nullptr;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SetColor(base, call, native_poll, r3, r4, f1, f2, f3, stack);
  });
}

namespace {

void SetColorPiece(PPCContext& ctx, uint8_t* base, uint32_t piece) {
  const uint32_t r3 = ctx.r3.u32, stack = ctx.r1.u32;
  const double f1 = ctx.f1.f64, f2 = ctx.f2.f64, f3 = ctx.f3.f64;
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  load_poll::Calls* native_poll = ApplyWheelPaintNative() ? &poll : nullptr;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SetColorPiece(base, call, native_poll, r3, piece, f1, f2, f3, stack);
  });
}

}  // namespace

void StandModelsSetColor0(PPCContext& ctx, uint8_t* base) { SetColorPiece(ctx, base, 0); }
void StandModelsSetColor1(PPCContext& ctx, uint8_t* base) { SetColorPiece(ctx, base, 1); }
void StandModelsSetColor2(PPCContext& ctx, uint8_t* base) { SetColorPiece(ctx, base, 2); }
void StandModelsSetColor3(PPCContext& ctx, uint8_t* base) { SetColorPiece(ctx, base, 3); }
void StandModelsSetColor4(PPCContext& ctx, uint8_t* base) { SetColorPiece(ctx, base, 4); }
void StandModelsSetColor5(PPCContext& ctx, uint8_t* base) { SetColorPiece(ctx, base, 5); }

void StandModelsSetSticker(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sm::SetSticker(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                              ctx.r8.u64, ctx.r9.u64, ctx.f1.f64, ctx.r1.u32);
}

void StandModelsSetDecalWord(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, stack = ctx.r1.u32;
  WithCalls(ctx, base, [&](const sm::GuestCall& call) {
    return sm::SetDecalWord(base, call, r3, r4, stack);
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
ITC_STAND_MODELS(82179130, StandModelsSetTireGrade)
ITC_STAND_MODELS(821CCA38, StandModelsTuningSetTireGrade)
ITC_STAND_MODELS(821CC890, StandModelsStepSecondWheels)
ITC_STAND_MODELS(821DFA78, StandModelsPreviewWheel)
// GarageStage::SetWheel sub_8217DD38 has none: only TuningScreen::PreviewWheel
// calls it, and its plain C++ calls SetWheel's, which calls FitAxles' and
// SceneWheels::SetSizes' (none either; their register steps above are the
// harness's).
// GarageStage::SecondWheelsPending sub_82179C48, AddSecondWheels
// sub_82179998, ApplyWheelPaint sub_821799D8 and SceneModel::AddSecondWheels
// sub_820D9E00 have none: only TuningScreen::StepSecondWheels reaches them,
// and its plain C++ calls theirs (their register steps above are the
// harness's).
// SceneWheels::SetTireGrades sub_820E7470 and Wheels::SetTireTextures
// sub_820D8F18 have none: only GarageStage::SetTireGrade reaches them, and
// its plain C++ calls theirs (their register steps above are the harness's).
// sub_82178F68 (SetPart's case 0, split off by the recompiler) needs no
// adapter: only the original SetPart reaches it, as a tail call.
// GarageStage::SetColor sub_8217C6C8 and its pieces SetColor0 to SetColor5
// sub_8217B078 to sub_8217B348 have none: only the tuning screen's
// UpdateStand and RestoreStand reach them, and their plain C++ calls
// SetColor's (their register steps above are the harness's).
// GarageStage::SetSticker sub_8217B3D8 and SetDecalWord sub_821793D0 have
// none: only the tuning screen's ApplyAreaDecals and ApplyDecalWord reach
// them, and their plain C++ calls theirs (their register steps above are
// the harness's).
// SceneParts::LightValue sub_820DF1A0 has none: only SetLightsOn calls it,
// and its plain C++ calls LightValue's (its register step above is the
// harness's).
