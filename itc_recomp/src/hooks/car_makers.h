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

}  // namespace hooks
