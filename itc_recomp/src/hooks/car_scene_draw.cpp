// The car scene's draw natively: adapters in front of src/car_scene_draw/
// (docs/functions/car-scene-draw.md), with the register steps the harness
// shares (hooks/car_scene_draw.h).

#include "hooks/car_scene_draw.h"

#include "base/flags.h"
#include "car_scene_draw/car_scene_draw.h"

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
              "CarScene::PlayerModel sub_820C4FB8 and TextureProjection::Fit sub_82598768 (the "
              "wrapped ones inside their "
              "--find_per_frame_state wraps), natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=car_scene_draw)");

namespace hooks {

namespace csd = car_scene_draw;

void CarSceneDrawFrustumFromMatrix(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::FrustumFromMatrix(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarSceneDrawPlaneIntersect3(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      csd::PlaneIntersect3(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

void CarSceneDrawPlaneSegmentCrossing(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::PlaneSegmentCrossing(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                         ctx.r1.u32);
}

void CarSceneDrawBoxFromPoints(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::BoxFromPoints(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarSceneDrawBoxExtend(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  csd::BoxExtend(base, ctx.r3.u32, ctx.r4.u32);
}

void CarSceneDrawVector16Size(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = csd::Vector16Size(base, ctx.r3.u32);
}

void CarSceneDrawVector16PushBack(PPCContext& ctx, uint8_t* base) {
  const auto r3 = csd::Vector16PushBack(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  if (!r3) {
    __imp__sub_825986B8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void CarSceneDrawVector16Insert(PPCContext& ctx, uint8_t* base) {
  const auto r3 =
      csd::Vector16Insert(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
  if (!r3) {
    __imp__sub_82598648(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void CarSceneDrawVector16InsertN(PPCContext& ctx, uint8_t* base) {
  const auto r3 =
      csd::Vector16InsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
  if (!r3) {
    __imp__sub_82598288(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void CarSceneDrawVector16Allocate(PPCContext& ctx, uint8_t* base) {
  const auto r3 = csd::Vector16Allocate(base, ctx.r4.u32, ctx.r1.u32);
  if (!r3) {
    __imp__sub_82598158(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void CarSceneDrawLookAt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  csd::LookAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

void CarSceneDrawPerspective(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!csd::Perspective(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64,
                        ctx.r1.u32)) {
    __imp__sub_825972A8(ctx, base);
  }
}

void CarSceneDrawProjectorBuild(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!csd::ProjectorBuild(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                           ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.r1.u32)) {
    __imp__sub_82597610(ctx, base);
    return;
  }
  ctx.fpscr.enableFlushMode();
}

void CarSceneDrawProjectorBuildOrtho(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  csd::ProjectorBuildOrtho(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                           ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.r1.u32);
  ctx.fpscr.enableFlushMode();
}

void CarSceneDrawSetLight(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto memcpy = [&](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp,
                          uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = bytes;
    __imp__sub_825A7410(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  ctx.r3.u64 = csd::ScenePassSetLight(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32,
                                      memcpy);
}

void CarSceneDrawBeamColor(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = csd::SceneModelBeamColor(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarSceneDrawBeam(PPCContext& ctx, uint8_t* base) {
  csd::SceneModelBeam(base, ctx.r3.u32, ctx.r4.u32);
}

void CarSceneDrawBeamShape(PPCContext& ctx, uint8_t* base) {
  csd::SceneModelBeamShape(base, ctx.r3.u32, ctx.r4.u32);
}

void CarSceneDrawPlayerModel(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = csd::CarScenePlayerModel(base, ctx.r3.u32);
}

void CarSceneDrawTextureProjectionFit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto push_back = [&](uint32_t vector, uint32_t value, uint32_t sp, uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = vector;
    ctx.r4.u64 = value;
    __imp__sub_825986B8(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  uint32_t r3 = 0;
  if (!csd::TextureProjectionFit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64,
                                 ctx.f2.f64, ctx.f3.f64, ctx.f4.f64, ctx.f5.f64, ctx.r1.u32,
                                 push_back, &r3)) {
    __imp__sub_82598768(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
  ctx.fpscr.enableFlushMode();
}

car_scene_draw::SceneDrawCalls CarSceneDrawRootCalls(PPCContext& ctx, uint8_t* base) {
  car_scene_draw::SceneDrawCalls calls;
  calls.call = [&ctx, base](uint32_t function, const uint64_t* regs, int count,
                            const double* fregs, int fcount, uint32_t sp,
                            uint32_t lr) -> uint64_t {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    PPCRegister* gprs[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                            &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < count; i++) {
      gprs[i]->u64 = regs[i];
    }
    if (fcount > 0) {
      ctx.fpscr.disableFlushMode();
    }
    PPCRegister* fprs[5] = {&ctx.f1, &ctx.f2, &ctx.f3, &ctx.f4, &ctx.f5};
    for (int i = 0; i < fcount; i++) {
      fprs[i]->f64 = fregs[i];
    }
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
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
    __imp__sub_825A7410(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  calls.push_back = [&ctx, base](uint32_t vector, uint32_t value, uint32_t sp, uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = vector;
    ctx.r4.u64 = value;
    __imp__sub_825986B8(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  return calls;
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
