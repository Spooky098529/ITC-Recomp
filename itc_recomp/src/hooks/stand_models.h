// The stand's scene model holders' register steps (src/stand_models/,
// docs/functions/stand-models.md), shared by the adapters
// (src/hooks/stand_models.cpp, behind --native_stand_models) and the
// harness (without the flag); those of the functions without adapters
// (SceneParts::LightValue: only SetLightsOn's plain C++ reaches it) are the
// harness's alone. Each turns flush-to-zero off, leaves r3 (and f1 where
// the original sets it) as the original does, and runs the original where
// the callees' plain C++ isn't on.

#pragma once

#include "guest.h"

#include "stand_models/stand_models.h"

namespace hooks {

// The guest calls the plain C++ makes through `ctx` (the callee by its
// address, so overridden ones run their adapters).
stand_models::GuestCall StandModelsGuestCall(PPCContext& ctx, uint8_t* base);

// GarageStage::MakeModel sub_8217FCB0 (r3 the holder, r4 a model block, r5
// a draw word; with --native_vehicle_accessors and --native_scene_models).
void StandModelsMakeModel(PPCContext& ctx, uint8_t* base);
// GarageStage::SetInteriorClass sub_82178FE8, SetWord5016 sub_82179598,
// SetWheelWord sub_82179A18 (r3 the holder, r4 a value), SetLightsOn
// sub_82179420 (r3 the holder, r4 a flag), SetDecalA sub_82179818 and
// SetDecalB sub_821798D8 (r3 the holder, r4 an entry, r5, r6 two words, f1
// to f4 four floats); with --native_vehicle_accessors.
void StandModelsSetInteriorClass(PPCContext& ctx, uint8_t* base);
void StandModelsSetWord5016(PPCContext& ctx, uint8_t* base);
void StandModelsSetWheelWord(PPCContext& ctx, uint8_t* base);
void StandModelsSetLightsOn(PPCContext& ctx, uint8_t* base);
void StandModelsSetDecalA(PPCContext& ctx, uint8_t* base);
void StandModelsSetDecalB(PPCContext& ctx, uint8_t* base);
// The setters that re-apply the record (with --native_vehicle_accessors and
// --native_scene_models): GarageStage::SetPart sub_82178F00 (r3 the holder,
// r4 a slot, r5 a word), SetInteriorWord sub_82179050 (r4 a kind, r5 a
// word), SetInteriorLevel sub_821791C0 (r4 a value), SetInteriorPair
// sub_82179250 (r4, r5 two words), ClearChoice sub_82179378 (r4 a choice),
// SetAero sub_82179548 (r4 a word) and ReloadDecals sub_82178EC0; lr put
// back.
void StandModelsSetPart(PPCContext& ctx, uint8_t* base);
void StandModelsSetInteriorWord(PPCContext& ctx, uint8_t* base);
void StandModelsSetInteriorLevel(PPCContext& ctx, uint8_t* base);
void StandModelsSetInteriorPair(PPCContext& ctx, uint8_t* base);
void StandModelsClearChoice(PPCContext& ctx, uint8_t* base);
void StandModelsSetAero(PPCContext& ctx, uint8_t* base);
void StandModelsReloadDecals(PPCContext& ctx, uint8_t* base);
// The tire grades (with --native_vehicle_accessors; lr put back):
// GarageStage::SetTireGrade sub_82179130 (r3 the holder, r4 which, r5 a
// grade), TuningScreen::SetTireGrade sub_821CCA38 (r4 which, r5 an index),
// and, without adapters (the harness's), SceneWheels::SetTireGrades
// sub_820E7470 (r3 a wheels object, r4, r5 the grades) and
// Wheels::SetTireTextures sub_820D8F18 (r3 the first set's wheels).
void StandModelsSetTireGrade(PPCContext& ctx, uint8_t* base);
void StandModelsTuningSetTireGrade(PPCContext& ctx, uint8_t* base);
void StandModelsSetTireGrades(PPCContext& ctx, uint8_t* base);
void StandModelsSetTireTextures(PPCContext& ctx, uint8_t* base);
// The second wheels (with --native_vehicle_accessors and
// --native_scene_models; lr put back): TuningScreen::StepSecondWheels
// sub_821CC890 (r3 the tuning screen), and, without adapters (the
// harness's), GarageStage::SecondWheelsPending sub_82179C48,
// AddSecondWheels sub_82179998, ApplyWheelPaint sub_821799D8 (r3 the
// holder) and SceneModel::AddSecondWheels sub_820D9E00 (r3 a scene model).
void StandModelsStepSecondWheels(PPCContext& ctx, uint8_t* base);
void StandModelsSecondWheelsPending(PPCContext& ctx, uint8_t* base);
void StandModelsAddSecondWheels(PPCContext& ctx, uint8_t* base);
void StandModelsApplyWheelPaint(PPCContext& ctx, uint8_t* base);
void StandModelsSceneAddSecondWheels(PPCContext& ctx, uint8_t* base);
// The wheel fitting, without adapters (only GarageStage::SetWheel's plain
// C++ calls them; the harness's): GarageStage::FitAxles sub_82178B98 (r3
// the holder, r4 a car object; f1 as it leaves it; with
// --native_car_physics and --native_math, else the original) and
// SceneWheels::SetSizes sub_820E7380 (r3 a wheels object, r4, r5 two words,
// r6 to r9 the two values).
void StandModelsFitAxles(PPCContext& ctx, uint8_t* base);
void StandModelsSetSizes(PPCContext& ctx, uint8_t* base);
// TuningScreen::PreviewWheel sub_821DFA78 (r3 the tuning screen, r4 a wheel
// number, r5 which; with --native_race_setup too) and, without an adapter
// (only PreviewWheel's plain C++ calls it; the harness's),
// GarageStage::SetWheel sub_8217DD38 (r3 the holder, r4, r5, r6 a car
// object); with --native_vehicle_accessors, --native_car_specs,
// --native_car_physics and --native_math, else the originals; f1 as
// FitAxles leaves it.
void StandModelsPreviewWheel(PPCContext& ctx, uint8_t* base);
void StandModelsSetWheel(PPCContext& ctx, uint8_t* base);
// SceneParts::LightValue sub_820DF1A0 (r3 the parts' point block; f1).
void StandModelsLightValue(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
