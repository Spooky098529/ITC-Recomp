// The scene model's record re-applies as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/scene-models.md,
// their callers, the stand's setters, in stand-models.md): what the stand's
// setters call after changing a word of the stage's scene model's record.
// Guest memory with the originals' loads and stores in their order, each
// with the original's frame below `stack` (its back chain written, not its
// saved registers); the callees through `call` (their adapters, with the
// original's return address), memcpy natively where native::Buffers_Memcpy
// takes it. Adapters: src/hooks/scene_models.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <optional>

namespace scene_models {

// A guest function by its address (its override when it has one) with r3 on
// from `args`, r1 = `frame`, the link register `lr`; its r3.
using ApplyCall = std::function<uint64_t(uint32_t function, std::initializer_list<uint64_t> args,
                                         uint32_t frame, uint32_t lr)>;

// sub_820E08B0 SceneParts::Apply: nothing when the parts' scene
// ([+37284])'s +4 is 1 (r3 the parts); else the 60-byte record copied to
// +108 and SceneParts::Build (its r3). Its 96-byte frame below `stack`.
uint64_t ScenePartsApply(uint8_t* base, const ApplyCall& call, uint32_t parts, uint32_t record,
                         uint32_t stack);
// sub_820E4258 SceneInterior::Apply: the 120-byte record copied to +40 and
// to [+36] + 28; +9656 = [+11668] + 784 + 464 x the record's +100 when it's
// set; the six parts' ([+11632 + 4i]) +452 = CarModel::SelectedTexture of
// [[+24] + 24]; then SceneInterior::SetGauges (its r3). Its 112-byte frame
// below `stack`.
uint64_t SceneInteriorApply(uint8_t* base, const ApplyCall& call, uint32_t interior,
                            uint32_t record, uint32_t stack);
// sub_820DA758 SceneModel::ApplyRecord: nothing until SceneModel::PollLoad
// answers 1 (its r3 otherwise); block 5 (+48) +3828 = 0, +3824 = 1; the car
// record into the car model (CarModel::SetRecord), SceneParts::Apply(+5060,
// +3744), SceneInterior::Apply(+5068, +3804). Returns r3 as the original
// leaves it (the scene for kind 2, else SceneInterior::Apply's). Its
// 144-byte frame below `stack`. nullopt, with nothing done, when the scene's
// (+5116) +4 isn't 0: the race's path (the parts claimed and set up again),
// never taken in play (the callers are the garage stage's), left to the
// original.
std::optional<uint64_t> SceneModelApplyRecord(uint8_t* base, const ApplyCall& call,
                                              uint32_t model, uint32_t stack);
// sub_820D9D48 SceneModel::ReloadDecals: when ready (+5036) and not traffic
// (+32 3): +5036 = +5056 = 0, CarModel::SetRecord([[+5048] + 24], +48) and
// CarModel::ReloadDecals on it (its r3); else r3 the model. Its 96-byte
// frame below `stack`.
uint64_t SceneModelReloadDecals(uint8_t* base, const ApplyCall& call, uint32_t model,
                                uint32_t stack);

}  // namespace scene_models
