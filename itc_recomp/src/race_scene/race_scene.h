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

}  // namespace race_scene
