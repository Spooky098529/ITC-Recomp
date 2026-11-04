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
              "C++; _Xlen and bad_alloc's paths left to the originals), natively instead of "
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

}  // namespace hooks

// The adapters: TextureProjection::Fit (recompiled) calls them. None for
// Plane::Intersect3, Box::Extend, Vector16::Size, Vector16::Insert, InsertN
// and Allocate (removed 2026-11-04: in play only Frustum::FromMatrix's,
// Box::FromPoints', PushBack's and their callees' plain C++ call them; their
// register steps above stay for the harness).
#define ITC_CAR_SCENE_DRAW_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {            \
    REX_FUNC_PROLOGUE();                          \
    if (!ITC_FLAG(native_car_scene_draw)) {       \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    hooks::step(ctx, base);                       \
  }

ITC_CAR_SCENE_DRAW_ADAPTER(82597878, CarSceneDrawFrustumFromMatrix)
ITC_CAR_SCENE_DRAW_ADAPTER(82597E58, CarSceneDrawPlaneSegmentCrossing)
ITC_CAR_SCENE_DRAW_ADAPTER(82597210, CarSceneDrawBoxFromPoints)
ITC_CAR_SCENE_DRAW_ADAPTER(825986B8, CarSceneDrawVector16PushBack)
