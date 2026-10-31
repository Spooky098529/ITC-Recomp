// The race scene natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/race-scene.md): the race scene's state machine
// (RaceSceneMachine::, race mode + 87936) and the 31,168-byte object it
// keeps at [0x826D82D4] (RaceScene::). Plain C++ over guest memory (the
// object and the machine are shared with the recompiled code), with the
// originals' loads and stores in their order. Adapters:
// src/hooks/race_scene.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace race_scene {

constexpr uint32_t kScene = 0x826D82D4;  // the race scene
constexpr uint32_t kSceneBytes = 31168;
// The machine's toggle (probably the pause): +9608.
constexpr uint32_t kPaused = 9608;
// RaceSceneMachine::Init's frame (its r1 less this).
constexpr uint32_t kInitFrame = 864;

// sub_8215D6C8 RaceScene::RaceScene: the seven car slots' records
// (SceneModelRecord::SceneModelRecord on +48 + 4,288 i), then the w of the
// 11 light entries' five vectors (+30080 + 96 i, +28 to +92) = the float at
// 0x82000A30. Returns the object.
uint32_t SceneConstruct(uint8_t* base, uint32_t scene);
// sub_8215D738 RaceScene::Create: [kScene] = 0, operator new(31,168) and,
// when made, RaceScene::RaceScene on it into [kScene] (else 0). False, with
// only [kScene] = 0 written, where operator new's plain C++ refuses (the
// original then runs).
bool SceneCreate(uint8_t* base);
// sub_8215D808 RaceSceneMachine::Init: RaceScene::Create, the state = 1 and
// the 11 transitions added (StateMachine::Add), built in the original's
// stack frame at `frame` (its r1 less kInitFrame: see race_scene_init.cpp).
// Returns the last StateMachine::Add's answer (`replaced` as its), or
// nothing, with only [kScene] = 0 written, where RaceScene::Create refuses.
std::optional<uint32_t> MachineInit(uint8_t* base, uint32_t machine, uint32_t frame,
                                    bool* replaced);

// --- The light set (the object's +30064) ---

// sub_8215C368 RaceLights::InitEntry (probably): a 96-byte entry for its
// index (11 or more, as signed: nothing): +0 = 0, then kind 1 (indices
// below 3) or 2 with their vectors from the image's constants (the cone's
// two cosines, cos, for kind 2), and the color +16 by index. Returns
// nothing: r3 is left as it was, f1 as the last cos leaves it.
void LightsInitEntry(uint8_t* base, uint32_t entry, int32_t index);
// sub_8215CF78 RaceLights::SetUp (probably): +0, +1072, +8, +4 = 0, then
// the 11 entries (+16 + 96 i) set up and counted (+4 kind 1, +8 kind 2, +0
// both).
void LightsSetUp(uint8_t* base, uint32_t set);
// f1 as the last entry's InitEntry leaves it: cos of the double at
// 0x82002FE0.
double LightsLastF1(uint8_t* base);

// --- The post effects' parameters (post-effects.md) ---

// Whether the race's post effects are split screen's: [[0x826C2CE8] + 8].
bool SplitScreen(uint8_t* base);
// Whether SetEffectParams' and Glows::SetParams' vtable calls reach the
// setters' plain C++: the glows (effects 5 and 9 in split screen, else 1)
// and the color filters (6 and 10, else 2) are there, the glows' slots 2 to
// 12 are Glow::Set8436 to Set8476 (sub_82136EF8 to sub_82136F48) and the
// filters' slot 8 ColorFilter::Set8120 (sub_82135148). Else the originals
// run (they'd call through whatever is there, or through address 0).
bool EffectParamsReady(uint8_t* base);
// sub_8228F0B0 Glows::SetParams (probably): the 11 floats at `entry` given
// to each glow's slots 2 to 12 in order. Returns the last glow (r3 as the
// original leaves it). Only where EffectParamsReady.
uint32_t GlowsSetParams(uint8_t* base, uint32_t entry);
// sub_8228F5C0 RaceScene::SetEffectParams (probably): the 44-byte entry at
// 0x82637CC0 + 44 (3 kind + index) (kind below 8, as signed; else the one at
// 0x82637C94) through Glows::SetParams, then the color filters' slot 8 with
// the float at 0x82002204. Returns the last filter (r3 as the original
// leaves it), `f1` the value (f1 as it leaves it). Only where
// EffectParamsReady.
uint32_t SetEffectParams(uint8_t* base, uint32_t kind, uint32_t index, double* f1);

}  // namespace race_scene
