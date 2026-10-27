// The vehicle code above the car physics natively: adapters in front of
// src/vehicle/ (docs/functions/vehicle.md), with the register steps the
// harness shares (hooks/vehicle_native.h).

#include "hooks/vehicle_native.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "hooks/heap.h"
#include "vehicle/vehicle.h"

ITC_FLAG_BOOL(native_vehicle, true, "ITC",
              "Run the vehicle code above the car physics (docs/functions/vehicle.md: "
              "Matrix::FromRows sub_820BFBB8, TrackPosition::Copy sub_820C2150, "
              "CameraCurve::Load sub_820BFCE8, RoutePosition::Step sub_82170F90, "
              "Vehicle::UpdateTrackPositions sub_820C1D78, Vehicle::LinkTrackPositions "
              "sub_820C1D08, Vehicle::Refresh sub_820C0118; Vehicle::RemoveFromWorld "
              "sub_820C20E0, Vehicles::Create sub_820C3BD0 with Vehicle::Construct "
              "sub_820C30D8, Vehicles::Destroy sub_820C3B78 with Vehicles::DestroyAll "
              "sub_820C21E0 (these only while --native_heap routes the game's heap to the "
              "allocation layer)) natively instead of through the translated code (the same "
              "words, bit for bit; checked by --run_harnesses=vehicle)");

namespace hooks {

namespace vh = vehicle;

void VehicleMatrixFromRows(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::MatrixFromRows(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

void VehicleTrackPositionCopy(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::TrackPositionCopy(base, ctx.r3.u32, ctx.r4.u32);
}

void VehicleCameraCurveLoad(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::CameraCurveLoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void VehicleRoutePositionStep(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      vh::RoutePositionStep(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112);
}

void VehicleUpdateTrackPositions(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::VehicleUpdateTrackPositions(base, ctx.r3.u32, ctx.r1.u32 - 112);
}

void VehicleLinkTrackPositions(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!vh::VehicleLinkTrackPositions(base, ctx.r3.u32, &r3)) {
    __imp__sub_820C1D08(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

void VehicleRefresh(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0.0;
  uint32_t r3 = 0;
  if (!vh::VehicleRefresh(base, ctx.r3.u32, ctx.r1.u32 - 96, &f1, &r3)) {
    __imp__sub_820C0118(ctx, base);
    return;
  }
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
}

void VehicleRemoveFromWorld(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!NativeHeapActive() || !vh::VehicleRemoveFromWorld(base, ctx.r3.u32, &r3)) {
    __imp__sub_820C20E0(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

void VehiclesCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = NativeHeapActive() ? vh::VehiclesCreate(base, ctx.r1.u32 - 112) : std::nullopt;
  if (!r3) {
    __imp__sub_820C3BD0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void VehiclesDestroy(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive() || !vh::VehiclesDestroy(base)) {
    __imp__sub_820C3B78(ctx, base);
  }
  // r3 not set: the original leaves operator delete's (free's) there, or
  // its own r3 without a block; its callers don't read it.
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_VEHICLE(address, step)      \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_vehicle)) {    \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

ITC_VEHICLE(820BFBB8, VehicleMatrixFromRows)
ITC_VEHICLE(820C2150, VehicleTrackPositionCopy)
ITC_VEHICLE(820BFCE8, VehicleCameraCurveLoad)
ITC_VEHICLE(82170F90, VehicleRoutePositionStep)
ITC_VEHICLE(820C1D78, VehicleUpdateTrackPositions)
ITC_VEHICLE(820C1D08, VehicleLinkTrackPositions)
ITC_VEHICLE(820C0118, VehicleRefresh)
ITC_VEHICLE(820C20E0, VehicleRemoveFromWorld)
ITC_VEHICLE(820C3BD0, VehiclesCreate)
ITC_VEHICLE(820C3B78, VehiclesDestroy)
// Vehicle::Construct (sub_820C30D8) and Vehicles::DestroyAll (sub_820C21E0)
// have no adapters: their only callers are Vehicles::Create and Destroy.
