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
              "allocation layer); Vehicle::ApplyInput sub_820BFD30 (its callees never seen in "
              "play, sub_820B6170, sub_820B03E0, sub_820B0428, sub_820B0098, sub_820B00A8, "
              "sub_820B00B8, by the original), Vehicle::SlipWheelValue sub_820C0310, "
              "Racers::NearestHumanVehicle sub_820C2248; the power curve, PowerCurve::ShiftPeak "
              "sub_820CE1D8, Build sub_820CE430, FromEngine sub_820CE7D8, and "
              "VehicleStatus::Reset sub_820BFC48; Vehicle::SetUpSurfaceGrip sub_820C1E00, "
              "Vehicle::SetUp sub_820C3150 and ResetForCar sub_820C33B8 (their physics' set-up "
              "natively only while --native_heap routes the game's heap to the layer); "
              "VehicleStatus::Update sub_820C23E8 and Vehicle::Tick sub_820C3020) natively instead of through the translated code (the same "
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

void VehicleApplyInput(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t vehicle = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 176;
  auto steer = [&](double target, double dt, double f3, uint32_t left, uint32_t right) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = vehicle;
    ctx.f1.f64 = target;
    ctx.f2.f64 = dt;
    ctx.f3.f64 = f3;
    ctx.r4.u64 = left;
    ctx.r5.u64 = right;
    __imp__sub_820B5FD8(ctx, base);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
  if (!vh::VehicleApplyInput(base, vehicle, ctx.r4.u32, frame, steer)) {
    __imp__sub_820BFD30(ctx, base);
    return;
  }
  ctx.r3.u64 = vehicle;
  ctx.f1.f64 = double(guest::LoadFloat(base, vh::kStepSeconds));
}

void VehicleSlipWheelValue(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = vh::VehicleSlipWheelValue(base, ctx.r3.u32, ctx.f1.f64, ctx.r1.u32 - 240);
}

void VehicleNearestHuman(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = vh::RacersNearestHumanVehicle(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 304);
}

void VehicleShiftPeak(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::PowerCurveShiftPeak(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void VehicleBuildCurve(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::PowerCurveBuild(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112);
}

void VehicleCurveFromEngine(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  vh::PowerCurveFromEngine(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112);
}

void VehicleStatusReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = vh::VehicleStatusReset(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 1168);
}

void VehicleSurfaceGrip(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = vh::VehicleSetUpSurfaceGrip(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 480);
}

namespace {

void SetUpStep(PPCContext& ctx, uint8_t* base, bool reset) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 224;
  auto original = [&](uint32_t address, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    ctx.r6.u64 = r6;
    switch (address) {
      case 0x820C27E8:
        __imp__sub_820C27E8(ctx, base);
        break;
      case 0x820B5B08:
        __imp__sub_820B5B08(ctx, base);
        break;
      case 0x820B5670:
        __imp__sub_820B5670(ctx, base);
        break;
      default:
        __imp__sub_820C1D08(ctx, base);
        break;
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
  double f1 = 0.0;
  ctx.r3.u64 = vh::VehicleSetUp(base, ctx.r3.u32, ctx.r4.u32, frame, reset, NativeHeapActive(),
                                original, &f1);
  ctx.f1.f64 = f1;
}

}  // namespace

void VehicleSetUp(PPCContext& ctx, uint8_t* base) {
  SetUpStep(ctx, base, false);
}

void VehicleResetForCar(PPCContext& ctx, uint8_t* base) {
  SetUpStep(ctx, base, true);
}

void VehicleStatusUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!vh::VehicleStatusUpdate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                               ctx.r1.u32 - 384)) {
    __imp__sub_820C23E8(ctx, base);
  }
}

void VehicleTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  auto call = [&](uint32_t address, uint32_t r1, const vh::GuestRegisters& r) {
    const uint64_t saved = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = r.r3;
    ctx.r4.u64 = r.r4;
    ctx.r5.u64 = r.r5;
    ctx.r6.u64 = r.r6;
    ctx.f1.f64 = r.f1;
    ctx.f2.f64 = r.f2;
    ctx.f3.f64 = r.f3;
    switch (address) {
      case 0x820B5FD8:
        __imp__sub_820B5FD8(ctx, base);
        break;
      case 0x820BFD30:
        __imp__sub_820BFD30(ctx, base);
        break;
      case 0x820B6498:
        __imp__sub_820B6498(ctx, base);
        break;
      case 0x820B18B8:
        ctx.lr = 0x820B6514;
        __imp__sub_820B18B8(ctx, base);
        break;
      case 0x820B2100:
        ctx.lr = 0x820B6538;
        __imp__sub_820B2100(ctx, base);
        break;
      default:
        __imp__sub_820C23E8(ctx, base);
        break;
    }
    ctx.r1.u64 = saved;
    ctx.fpscr.disableFlushMode();
  };
  vh::VehicleTick(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32 - 144, call);
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
ITC_VEHICLE(820C1D08, VehicleLinkTrackPositions)
ITC_VEHICLE(820C0118, VehicleRefresh)
ITC_VEHICLE(820C20E0, VehicleRemoveFromWorld)
ITC_VEHICLE(820C3BD0, VehiclesCreate)
ITC_VEHICLE(820C3B78, VehiclesDestroy)
ITC_VEHICLE(820C0310, VehicleSlipWheelValue)
ITC_VEHICLE(820C2248, VehicleNearestHuman)
ITC_VEHICLE(820CE1D8, VehicleShiftPeak)
ITC_VEHICLE(820CE7D8, VehicleCurveFromEngine)
ITC_VEHICLE(820C3150, VehicleSetUp)
ITC_VEHICLE(820C33B8, VehicleResetForCar)
ITC_VEHICLE(820C23E8, VehicleStatusUpdate)
ITC_VEHICLE(820C3020, VehicleTick)
// Vehicle::Construct (sub_820C30D8) and Vehicles::DestroyAll (sub_820C21E0)
// have no adapters: their only callers are Vehicles::Create and Destroy.
// Vehicle::SetUpSurfaceGrip (sub_820C1E00) and VehicleStatus::Reset
// (sub_820BFC48), PowerCurve::Build (sub_820CE430), Vehicle::ApplyInput
// (sub_820BFD30) and Vehicle::UpdateTrackPositions (sub_820C1D78) have no
// adapters: their callers in play (Vehicle::SetUp and ResetForCar,
// PowerCurve::FromEngine, Vehicle::Tick) are native and call their plain
// C++; --native_vehicle switches those callers. Their register steps stay
// for the harness.
