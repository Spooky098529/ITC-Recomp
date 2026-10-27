// The vehicle's register steps (src/vehicle/, docs/functions/vehicle.md),
// shared by the adapters (src/hooks/vehicle_native.cpp, behind --native_vehicle)
// and the harness (without the flag). Each turns flush-to-zero off first,
// leaves r3 (and f1 where the original leaves a value there) as the
// original does, and runs the original where the plain C++ refuses.

#pragma once

#include "guest.h"

namespace hooks {

// Matrix::FromRows sub_820BFBB8: r3 = out, r4 to r7 = the rows; r3 left.
void VehicleMatrixFromRows(PPCContext& ctx, uint8_t* base);
// TrackPosition::Copy sub_820C2150: r3 = out, r4 = from; r3 left.
void VehicleTrackPositionCopy(PPCContext& ctx, uint8_t* base);
// CameraCurve::Load sub_820BFCE8: r3 = the object, r4 = the index, r5 = the
// word; r3 left.
void VehicleCameraCurveLoad(PPCContext& ctx, uint8_t* base);
// RoutePosition::Step sub_82170F90: r3 = the position, r4 = the step, r5 =
// the kind; r3 = Normalize's.
void VehicleRoutePositionStep(PPCContext& ctx, uint8_t* base);
// Vehicle::UpdateTrackPositions sub_820C1D78: r3 = the vehicle (r3 left as
// TrackPosition::Update's plain C++ leaves it: not read by the caller).
void VehicleUpdateTrackPositions(PPCContext& ctx, uint8_t* base);
// Vehicle::LinkTrackPositions sub_820C1D08: r3 = the vehicle; r3 = the
// current shape copy.
void VehicleLinkTrackPositions(PPCContext& ctx, uint8_t* base);
// Vehicle::Refresh sub_820C0118: r3 = the vehicle; f1 = 1/60, r3 =
// RefreshFromBody's.
void VehicleRefresh(PPCContext& ctx, uint8_t* base);
// Vehicle::RemoveFromWorld sub_820C20E0: r3 = the vehicle; r3 = the old
// +344. Native only while --native_heap routes the heap to the layer.
void VehicleRemoveFromWorld(PPCContext& ctx, uint8_t* base);
// Vehicles::Create sub_820C3BD0 and Vehicles::Destroy sub_820C3B78: no
// arguments. Native only while --native_heap routes the heap to the layer.
void VehiclesCreate(PPCContext& ctx, uint8_t* base);
void VehiclesDestroy(PPCContext& ctx, uint8_t* base);

// Vehicle::ApplyInput sub_820BFD30: r3 = the vehicle, r4 = the input; r3
// the vehicle, f1 dt. SteerTowards' original in its frame where its plain
// C++ refuses; the whole original where a callee never seen in play would
// run.
void VehicleApplyInput(PPCContext& ctx, uint8_t* base);
// Vehicle::SlipWheelValue sub_820C0310: r3 = the vehicle, f1 = the speed;
// f1 = the value.
void VehicleSlipWheelValue(PPCContext& ctx, uint8_t* base);
// Racers::NearestHumanVehicle sub_820C2248: r3 = the point, r4 = n; r3 =
// the vehicle.
void VehicleNearestHuman(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
