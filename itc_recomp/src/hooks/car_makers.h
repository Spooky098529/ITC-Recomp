// The car objects' makers' register steps (src/car_makers/,
// docs/functions/car-makers.md), shared by the adapters
// (src/hooks/car_makers.cpp, behind --native_car_makers) and the harness
// (without the flag); those of the functions without adapters (SetPaint,
// SetWheels, SetChoice, SetBytes: only FromCar's plain C++ reaches them) are
// the harness's alone. Each leaves r3 as the original does and flush-to-zero
// off.

#pragma once

#include "guest.h"

namespace hooks {

// CarModels::BlockOf sub_8215AAD0 (r3 a car id).
void CarMakersBlockOf(PPCContext& ctx, uint8_t* base);
// SceneModelRecord::SetPaint sub_8215AC70 (r3 a record, r4 a car object, r5
// the paint pick) and SetWheels sub_8215B238 (r3 a record, r4 a car object).
void CarMakersSetPaint(PPCContext& ctx, uint8_t* base);
void CarMakersSetWheels(PPCContext& ctx, uint8_t* base);
// SceneModelRecord::SetChoice sub_8215B1A0 (r3 the out, r4:r5 the value).
void CarMakersSetChoice(PPCContext& ctx, uint8_t* base);
// CarRecord::SetBytes sub_8215B0E0 (r3 a car record, r4 the car's +1568).
void CarMakersSetBytes(PPCContext& ctx, uint8_t* base);
// SceneModelRecord::FromCar sub_8215B878 (r3 a record, r4 a car object, r5
// the paint pick); the original unless --native_scene_models and
// --native_car_specs.
void CarMakersFromCar(PPCContext& ctx, uint8_t* base);
// The garage cars' makers, the originals unless --native_race_setup,
// --native_save_data, --native_car_specs and --native_car_physics:
// GarageCar::Reset sub_82110448 (r3 a garage car), GarageCars::StaticInit
// sub_825D9EF0, CarData::FromGarageCar sub_82179A60 (r4 a garage car, r5 a
// car data; f1 as it leaves it) and TuningScreen::SetUpCarData sub_821DF2F8
// (r3 the tuning screen).
bool CarMakersGarageCalleesNative();
void CarMakersGarageCarReset(PPCContext& ctx, uint8_t* base);
void CarMakersStaticInit(PPCContext& ctx, uint8_t* base);
void CarMakersFromGarageCar(PPCContext& ctx, uint8_t* base);
void CarMakersSetUpCarData(PPCContext& ctx, uint8_t* base);
// GarageStage::FitCar sub_82178C40 (r3 the holder, r4 a car object; f1 as
// it leaves it; with --native_car_physics and --native_math),
// GarageStage::CopyRecord sub_8217FC60 (r3 the holder, r4 a record; with
// --native_vehicle_accessors and --native_scene_models) and the stand's busy
// marker shown and hidden, sub_8219ED10 and sub_8219EDB0 (with
// --native_ui_movies_game).
void CarMakersFitCar(PPCContext& ctx, uint8_t* base);
void CarMakersCopyRecord(PPCContext& ctx, uint8_t* base);
void CarMakersShowBusyMarker(PPCContext& ctx, uint8_t* base);
// GarageStage::ReapplyRecord sub_82178DF8 and ReapplyRecordNoDecals
// sub_82178E60 (r3 the holder), the originals unless
// CarMakersReapplyCalleesNative (--native_scene_models,
// --native_front_end_car_draw and FrontEndCarDrawCalleesNative, with the
// load poll's flags).
bool CarMakersReapplyCalleesNative();
void CarMakersReapplyRecord(PPCContext& ctx, uint8_t* base);
void CarMakersReapplyRecordNoDecals(PPCContext& ctx, uint8_t* base);
void CarMakersHideBusyMarker(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
