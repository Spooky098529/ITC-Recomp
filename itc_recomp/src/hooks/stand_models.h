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
#include "hooks/native_entry.h"

#include "stand_models/stand_models.h"

namespace hooks {

// The guest calls the plain C++ makes through `ctx` (the callee by its
// address, so overridden ones run their adapters).
stand_models::GuestCall StandModelsGuestCall(lifted::Regs& ctx);

// GarageStage::MakeModel sub_8217FCB0 (r3 the holder, r4 a model block, r5
// a draw word; with --native_vehicle_accessors and --native_scene_models;
// no adapter since 2026-11-13, the harness's step).
void StandModelsMakeModel(lifted::Regs& ctx);
// GarageStage::SetInteriorClass sub_82178FE8, SetWord5016 sub_82179598,
// SetWheelWord sub_82179A18 (r3 the holder, r4 a value), SetLightsOn
// sub_82179420 (r3 the holder, r4 a flag), SetDecalA sub_82179818 and
// SetDecalB sub_821798D8 (r3 the holder, r4 an entry, r5, r6 two words, f1
// to f4 four floats); with --native_vehicle_accessors.
void StandModelsSetInteriorClass(lifted::Regs& ctx);
void StandModelsSetWord5016(lifted::Regs& ctx);
void StandModelsSetWheelWord(lifted::Regs& ctx);
void StandModelsSetLightsOn(lifted::Regs& ctx);
void StandModelsSetDecalA(lifted::Regs& ctx);
void StandModelsSetDecalB(lifted::Regs& ctx);
// The setters that re-apply the record (with --native_vehicle_accessors and
// --native_scene_models): GarageStage::SetPart sub_82178F00 (r3 the holder,
// r4 a slot, r5 a word), SetInteriorWord sub_82179050 (r4 a kind, r5 a
// word), SetInteriorLevel sub_821791C0 (r4 a value), SetInteriorPair
// sub_82179250 (r4, r5 two words), ClearChoice sub_82179378 (r4 a choice),
// SetAero sub_82179548 (r4 a word) and ReloadDecals sub_82178EC0; lr put
// back.
void StandModelsSetPart(lifted::Regs& ctx);
void StandModelsSetInteriorWord(lifted::Regs& ctx);
void StandModelsSetInteriorLevel(lifted::Regs& ctx);
void StandModelsSetInteriorPair(lifted::Regs& ctx);
void StandModelsClearChoice(lifted::Regs& ctx);
void StandModelsSetAero(lifted::Regs& ctx);
void StandModelsReloadDecals(lifted::Regs& ctx);
// The tire grades (with --native_vehicle_accessors; lr put back):
// GarageStage::SetTireGrade sub_82179130 (r3 the holder, r4 which, r5 a
// grade), TuningScreen::SetTireGrade sub_821CCA38 (r4 which, r5 an index),
// and, without adapters (the harness's), SceneWheels::SetTireGrades
// sub_820E7470 (r3 a wheels object, r4, r5 the grades) and
// Wheels::SetTireTextures sub_820D8F18 (r3 the first set's wheels).
void StandModelsSetTireGrade(lifted::Regs& ctx);
void StandModelsTuningSetTireGrade(lifted::Regs& ctx);
void StandModelsSetTireGrades(lifted::Regs& ctx);
void StandModelsSetTireTextures(lifted::Regs& ctx);
// The second wheels (with --native_vehicle_accessors and
// --native_scene_models; lr put back): TuningScreen::StepSecondWheels
// sub_821CC890 (r3 the tuning screen), and, without adapters (the
// harness's), GarageStage::SecondWheelsPending sub_82179C48,
// AddSecondWheels sub_82179998, ApplyWheelPaint sub_821799D8 (r3 the
// holder) and SceneModel::AddSecondWheels sub_820D9E00 (r3 a scene model).
void StandModelsStepSecondWheels(lifted::Regs& ctx);
void StandModelsSecondWheelsPending(lifted::Regs& ctx);
void StandModelsAddSecondWheels(lifted::Regs& ctx);
void StandModelsApplyWheelPaint(lifted::Regs& ctx);
void StandModelsSceneAddSecondWheels(lifted::Regs& ctx);
// The wheel fitting, without adapters (only GarageStage::SetWheel's plain
// C++ calls them; the harness's): GarageStage::FitAxles sub_82178B98 (r3
// the holder, r4 a car object; f1 as it leaves it; with
// --native_car_physics and --native_math, else the original) and
// SceneWheels::SetSizes sub_820E7380 (r3 a wheels object, r4, r5 two words,
// r6 to r9 the two values).
void StandModelsFitAxles(lifted::Regs& ctx);
void StandModelsSetSizes(lifted::Regs& ctx);
// TuningScreen::PreviewWheel sub_821DFA78 (r3 the tuning screen, r4 a wheel
// number, r5 which; with --native_race_setup too) and, without an adapter
// (only PreviewWheel's plain C++ calls it; the harness's),
// GarageStage::SetWheel sub_8217DD38 (r3 the holder, r4, r5, r6 a car
// object); with --native_vehicle_accessors, --native_car_specs,
// --native_car_physics and --native_math, else the originals; f1 as
// FitAxles leaves it.
void StandModelsPreviewWheel(lifted::Regs& ctx);
void StandModelsSetWheel(lifted::Regs& ctx);
// The paint, without adapters (only native callers reach them: the tuning
// screen's UpdateStand and RestoreStand; the harness's):
// GarageStage::SetColor sub_8217C6C8 (r3 the holder, r4 which, f1 to f3)
// and its pieces SetColor0 to SetColor5 sub_8217B078, sub_8217B108,
// sub_8217B198, sub_8217B228, sub_8217B2B8, sub_8217B348 (r3 the holder,
// f1 to f3); SceneModel::ApplyPaint's and ApplyWheelPaint's plain C++ with
// the load poll's flags, else their originals; lr put back.
void StandModelsSetColor(lifted::Regs& ctx);
void StandModelsSetColor0(lifted::Regs& ctx);
void StandModelsSetColor1(lifted::Regs& ctx);
void StandModelsSetColor2(lifted::Regs& ctx);
void StandModelsSetColor3(lifted::Regs& ctx);
void StandModelsSetColor4(lifted::Regs& ctx);
void StandModelsSetColor5(lifted::Regs& ctx);
// The decals the tuning screen's never-seen paths set, without adapters
// (only its native ApplyAreaDecals and ApplyDecalWord reach them; the
// harness's): GarageStage::SetSticker sub_8217B3D8 (r3 the holder, r4 an
// area, r5 a slot, r6 a word, r7 a flag, r8 and r9 a position, f1 an
// angle, the caller's r1 + 96 a size) and SetDecalWord sub_821793D0 (r3 the
// holder, r4 a word; SceneModel::ReloadDecals' plain C++); lr put back.
void StandModelsSetSticker(lifted::Regs& ctx);
void StandModelsSetDecalWord(lifted::Regs& ctx);
// The parts the tuning screen's PreviewParts sets, without adapters (only
// its plain C++ reaches them; the harness's): GarageStage::SetParts
// sub_821797A8 (r3 the holder, r4 to r9 six words; with
// --native_vehicle_accessors and --native_scene_models, else the original)
// and SetAeroSet sub_8217DBC0 (r3 the holder, r4 an aero set, r5 and r6
// the fallbacks; also with --native_car_specs); lr put back.
void StandModelsSetParts(lifted::Regs& ctx);
void StandModelsSetAeroSet(lifted::Regs& ctx);
// The setters the tuning screen's never-seen paths reach, without adapters
// (only its plain C++ reaches them; the harness's; lr put back):
// GarageStage::SetChoice sub_82179310 (r3 the holder, r4 a choice, r5, r6),
// SetAero2 sub_821794A8 and SetDoorKind sub_821794F8 (r4 a word; with
// --native_vehicle_accessors and --native_scene_models, else the originals),
// SetAccessory sub_821792B0 (r4, r5), SceneModel::ApplyInterior
// sub_820D9F40 (r3 a scene model), SceneInterior::SetRecord sub_820E38E0
// (r3 an interior, r4 a record), GarageStage::SetWheelExtras sub_821790D0
// (r3 the holder, r4, r5), SceneWheels::SetWord68 sub_820E7410 and
// SetCaliperTexture sub_820E7428 (r3 a wheels object, r4 a word; the load
// poll's plain C++ with its flags, else Wheels::SetRecord's adapter and
// SetCaliperTexture's original).
void StandModelsSetChoice(lifted::Regs& ctx);
void StandModelsSetAero2(lifted::Regs& ctx);
void StandModelsSetDoorKind(lifted::Regs& ctx);
void StandModelsSetAccessory(lifted::Regs& ctx);
void StandModelsApplyInterior(lifted::Regs& ctx);
void StandModelsSceneInteriorSetRecord(lifted::Regs& ctx);
void StandModelsSetWheelExtras(lifted::Regs& ctx);
void StandModelsSetWord68(lifted::Regs& ctx);
void StandModelsSceneSetCaliperTexture(lifted::Regs& ctx);
// The load poll's calls when its flags are on (as ApplyWheelPaint's
// callers pass them), else null.
load_poll::Calls* StandModelsPoll(load_poll::Calls& poll);
// SceneParts::LightValue sub_820DF1A0 (r3 the parts' point block; f1).
void StandModelsLightValue(lifted::Regs& ctx);

}  // namespace hooks
