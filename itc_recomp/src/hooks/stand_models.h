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
// SceneParts::LightValue sub_820DF1A0 (r3 the parts' point block; f1).
void StandModelsLightValue(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
