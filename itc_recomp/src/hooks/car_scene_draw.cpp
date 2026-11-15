// The car scene's draw natively: adapters in front of src/car_scene_draw/
// (docs/functions/car-scene-draw.md), with the register steps the harness
// shares (hooks/car_scene_draw.h).

#include "hooks/car_scene_draw.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "car_scene_draw/car_scene_draw.h"
#include "hooks/car_draw.h"
#include "lifted/sdk.h"

ITC_FLAG_DECLARE(bool, native_car_draw);
ITC_FLAG_DECLARE(bool, native_models);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the root runs the original (as the cameras'
// recording does).
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

ITC_FLAG_BOOL(native_car_scene_draw, true, "ITC",
              "Run the car scene's draw (docs/functions/car-scene-draw.md): so far "
              "TextureProjection::Fit's geometry library, Frustum::FromMatrix sub_82597878 with "
              "Plane::Intersect3 sub_825976C8, Plane::SegmentCrossing sub_82597E58, "
              "Box::FromPoints sub_82597210 with Box::Extend sub_82597150 and Vector16::Size "
              "sub_825971E8, and Vector16::PushBack sub_825986B8 with Insert sub_82598648, "
              "InsertN sub_82598288 and Allocate sub_82598158 (operator new and delete as plain "
              "C++; _Xlen and bad_alloc's paths left to the originals), Matrix::LookAt "
              "sub_82597378, Matrix::Perspective sub_825972A8, Projector::Build sub_82597610 and "
              "BuildOrtho sub_82597518, ScenePass::SetLight sub_820C4EB8, SceneModel::BeamColor "
              "sub_820DAA38, Beam sub_820DAAC8 and BeamShape sub_820DAB10, "
              "CarScene::PlayerModel sub_820C4FB8, TextureProjection::Fit sub_82598768 and the "
              "root CarScene::Draw sub_820C5DE8 (the wrapped ones inside their "
              "--find_per_frame_state wraps; the root with --native_models, "
              "--native_matrix_math and --native_vehicle_accessors and the command list's "
              "diagnostics off, else its original), natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=car_scene_draw)");

namespace hooks {

namespace csd = car_scene_draw;

ITC_REGS_STEP(CarSceneDrawFrustumFromMatrix) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::FrustumFromMatrix(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarSceneDrawPlaneIntersect3) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      csd::PlaneIntersect3(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarSceneDrawPlaneSegmentCrossing) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::PlaneSegmentCrossing(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                         ctx.r1.u32);
}

ITC_REGS_STEP(CarSceneDrawBoxFromPoints) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::BoxFromPoints(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarSceneDrawBoxExtend) {
  ctx.fpscr.disableFlushMode();
  csd::BoxExtend(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(CarSceneDrawVector16Size) {
  ctx.r3.u64 = csd::Vector16Size(base, ctx.r3.u32);
}

ITC_REGS_STEP(CarSceneDrawVector16PushBack) {
  const auto r3 = csd::Vector16PushBack(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  if (!r3) {
    lifted::original::c825986B8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(CarSceneDrawVector16Insert) {
  const auto r3 =
      csd::Vector16Insert(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
  if (!r3) {
    lifted::Sdk(ctx, base, __imp__sub_82598648);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(CarSceneDrawVector16InsertN) {
  const auto r3 =
      csd::Vector16InsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
  if (!r3) {
    lifted::Sdk(ctx, base, __imp__sub_82598288);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(CarSceneDrawVector16Allocate) {
  const auto r3 = csd::Vector16Allocate(base, ctx.r4.u32, ctx.r1.u32);
  if (!r3) {
    lifted::Sdk(ctx, base, __imp__sub_82598158);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(CarSceneDrawLookAt) {
  ctx.fpscr.disableFlushMode();
  csd::LookAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarSceneDrawPerspective) {
  ctx.fpscr.disableFlushMode();
  if (!csd::Perspective(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64,
                        ctx.r1.u32)) {
    lifted::Sdk(ctx, base, __imp__sub_825972A8);
  }
}

ITC_REGS_STEP(CarSceneDrawProjectorBuild) {
  ctx.fpscr.disableFlushMode();
  if (!csd::ProjectorBuild(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                           ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.r1.u32)) {
    lifted::Sdk(ctx, base, __imp__sub_82597610);
    return;
  }
  ctx.fpscr.enableFlushMode();
}

ITC_REGS_STEP(CarSceneDrawProjectorBuildOrtho) {
  ctx.fpscr.disableFlushMode();
  csd::ProjectorBuildOrtho(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                           ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.r1.u32);
  ctx.fpscr.enableFlushMode();
}

ITC_REGS_STEP(CarSceneDrawSetLight) {
  ctx.fpscr.disableFlushMode();
  const auto memcpy = [&](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp,
                          uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = bytes;
    lifted::original::c825A7410(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  ctx.r3.u64 = csd::ScenePassSetLight(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32,
                                      memcpy);
}

ITC_REGS_STEP(CarSceneDrawBeamColor) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::SceneModelBeamColor(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarSceneDrawBeam) {
  csd::SceneModelBeam(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(CarSceneDrawBeamShape) {
  csd::SceneModelBeamShape(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(CarSceneDrawPlayerModel) {
  ctx.r3.u64 = csd::CarScenePlayerModel(base, ctx.r3.u32);
}

ITC_REGS_STEP(CarSceneDrawTextureProjectionFit) {
  ctx.fpscr.disableFlushMode();
  const auto push_back = [&](uint32_t vector, uint32_t value, uint32_t sp, uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = vector;
    ctx.r4.u64 = value;
    lifted::original::c825986B8(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  uint32_t r3 = 0;
  if (!csd::TextureProjectionFit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64,
                                 ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.f5.f64, ctx.r1.u32,
                                 push_back, &r3)) {
    lifted::Sdk(ctx, base, __imp__sub_82598768);
    return;
  }
  ctx.r3.u64 = r3;
  ctx.fpscr.enableFlushMode();
}

car_scene_draw::SceneDrawCalls CarSceneDrawRootCalls(lifted::Regs& ctx, uint8_t* base,
                                                      PPCContext* sdk) {
  car_scene_draw::SceneDrawCalls calls;
  calls.call = [&ctx, base](uint32_t function, const uint64_t* regs, int count,
                            const double* fregs, int fcount, uint32_t sp,
                            uint32_t lr) -> uint64_t {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    lifted::Temp* gprs[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                            &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < count; i++) {
      gprs[i]->u64 = regs[i];
    }
    if (fcount > 0) {
      ctx.fpscr.disableFlushMode();
    }
    lifted::Temp* fprs[5] = {&ctx.f1, &ctx.f2, &ctx.f3, &ctx.f4, &ctx.f5};
    for (int i = 0; i < fcount; i++) {
      fprs[i]->f64 = fregs[i];
    }
    lifted::CallFromContext(ctx, base, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  calls.memcpy = [&ctx, base](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp,
                              uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = bytes;
    lifted::original::c825A7410(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  calls.push_back = [&ctx, base](uint32_t vector, uint32_t value, uint32_t sp, uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = vector;
    ctx.r4.u64 = value;
    lifted::original::c825986B8(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  // Car::DrawUpdate's body as its wrap in src/debug/frame_state_diff.cpp
  // runs it (the root makes the wrap's checks).
  // r24 goes in the SDK's context the view was made from (`sdk`, the one
  // lifted::Sdk runs on), as Car::DrawUpdate reads it from its caller;
  // without one (a caller on a view only), it isn't carried.
  calls.car_draw_update = [&ctx, base, sdk](uint64_t model, uint64_t r24, uint32_t sp,
                                            uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    const uint64_t saved_r24 = sdk != nullptr ? sdk->r24.u64 : 0;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = model;
    if (sdk != nullptr) {
      sdk->r24.u64 = r24;
    }
    ctx.fpscr.disableFlushMode();
    if (ITC_FLAG(native_car_draw)) {
      lifted::Sdk(ctx, base, CarDrawUpdate);
    } else {
      lifted::Sdk(ctx, base, __imp__sub_820DAC68);
    }
    ctx.r1.u64 = stack;
    if (sdk != nullptr) {
      sdk->r24.u64 = saved_r24;
    }
    ctx.fpscr.disableFlushMode();
  };
  return calls;
}

void CarSceneDrawRoot(PPCContext& sdk, uint8_t* base) {
  // On the SDK's context (the rewiring, stage 3): it hands Car::DrawUpdate
  // r24 in its own, which the view doesn't carry; its body on a view of it.
  lifted::RegsScope scope(sdk);
  lifted::Regs& ctx = scope.regs;
  // The callees' fallbacks the car draw also declines on (its
  // CarDraw*CalleesNative), and the command list's diagnostics.
  if (!ITC_FLAG(native_models) || !ITC_FLAG(native_matrix_math) ||
      !ITC_FLAG(native_vehicle_accessors) || ITC_FLAG(log_overlay_uploads) ||
      ITC_FLAG(log_slot_callers) || ITC_FLAG(log_recording_by_pass)) {
    lifted::Sdk(ctx, base, __imp__sub_820C5DE8);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = CarSceneDrawRootCalls(ctx, base, &sdk);
  const uint32_t r3 =
      car_scene_draw::CarSceneDraw(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

}  // namespace hooks

// No REX_FUNC adapters are left here (all removed 2026-11-04 once their
// callers in play were native: Frustum::FromMatrix, Plane::SegmentCrossing,
// Box::FromPoints, Vector16::PushBack and Matrix::Perspective are called by
// TextureProjection::Fit's and Projector::Build's plain C++, Plane::Intersect3,
// Box::Extend, Vector16::Size, Insert, InsertN and Allocate by their callees'
// and Matrix::LookAt by the projectors'). The register steps above stay for
// the harness and the wraps: the wrapped functions' are called by their wraps
// in src/debug/frame_state_diff.cpp (ITC_FRAME_STATE_WRAP_CAR_SCENE_DRAW).
