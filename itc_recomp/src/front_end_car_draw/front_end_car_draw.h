// The front end's car draw as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/front-end-car-draw.md, the fields in
// docs/known-data.md, "garage stage"): the garage stage the front-end
// screens show a car on, its command function and its animation list. The
// data is guest data shared with the recompiled code, so these work on
// guest memory with the originals' loads and stores in their order: the
// scalar float work as the translated code's doubles rounded to single
// (built with -ffp-model=strict, CMakeLists.txt). Each takes `stack`, the
// guest r1 at entry, and keeps the original's frame: its back chain written
// as stwu does (not its saved registers), its temporaries at the original's
// offsets, its callees run from it. Callees already ours are called as
// plain C++ (ScenePools::GetB, SceneModel::PollLoad, MenuSound::Play,
// memcpy, tan); the rest are guest calls (`Calls`). Expect flush-to-zero off
// at entry (the adapters turn it off) and leave it off. Adapters:
// src/hooks/front_end_car_draw.cpp.

#pragma once

#include "scenery_lists/scenery_lists.h"

#include <cstdint>
#include <functional>

namespace load_poll {
struct Calls;
}

namespace front_end_car_draw {

constexpr uint32_t kScene = 0x826C2E44;  // [it] the car scene

// What the plain C++ can't do itself.
struct Calls {
  // Any guest function `function` (through the function table: its
  // override when it has one) from the frame at `sp` with `lr`: r3 on from
  // `regs` (`count`, all 64 bits), f1 on from `fregs` (`fcount`); its r3
  // (64 bits), and its f1 in `*f1` when given. Leaves flush-to-zero off.
  // memcpy where native::Buffers_Memcpy declines, tan where c_runtime::Tan
  // can't stand in, MenuSound::Play's slot call and the callees still
  // recompiled.
  std::function<uint64_t(uint32_t function, const uint64_t* regs, int count, const double* fregs,
                         int fcount, uint32_t sp, uint32_t lr, double* f1)>
      call;
  // SceneModel::PollLoad's calls (hooks::LoadPollCalls), for the functions
  // that poll the scene model.
  load_poll::Calls* poll = nullptr;
  // Model::DrawShape's guest calls (hooks::SceneryListsDrawCalls).
  scenery_lists::DrawCalls draw;
};

// memcpy (native::Buffers_Memcpy, else a guest call from the frame at `sp`
// with `lr`, `lr` standing for the call's return address); its r3 (the
// destination). Shared by the files (garage_stage.cpp).
uint64_t Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
                uint32_t sp, uint32_t lr);

// --- The number plates and the stage's small helpers (garage_stage.cpp) ---

// sub_820DF190 Plates::Hide and sub_820DF180 Plates::Show: the parts'
// point block's (parts + 176) +2640 and +4576 = 0 (Hide) or 1 (Show).
void PlatesHide(uint8_t* base, uint32_t points);
void PlatesShow(uint8_t* base, uint32_t points);
// sub_82179BE0 GarageStage::IsBusy: 0 when the stage's scene model is
// loaded (SceneModel::PollLoad) and the stage's +4 is -1, else 1. Its
// 96-byte frame below `stack`.
uint32_t GarageStageIsBusy(uint8_t* base, const Calls& calls, uint32_t holder, uint32_t stack);
// sub_8217AFE8 GarageStage::SyncPlates: unless busy, the plates shown or
// hidden when the stage's +6092 isn't the parts' +2816. Returns r3 as the
// original leaves it (IsBusy's answer, or the point block).
uint32_t GarageStageSyncPlates(uint8_t* base, const Calls& calls, uint32_t holder,
                               uint32_t stack);
// sub_82178B40 GarageStage::SetDrawWord: the stage's +6056 and the scene
// model's +4296 = `word`. Returns the scene model (r3).
uint32_t GarageStageSetDrawWord(uint8_t* base, uint32_t holder, uint32_t word, uint32_t stack);

// --- The commands (garage_stage.cpp) ---

// sub_82179C88 GarageStage::StepAngles: command 9 to 13 on the angles at
// `angles` + 48 (the stage's +80 to +88): the yaw or pitch 0.5° up or down
// (wrapped to -π to π), or all three 0.0 (13). r3 is left.
void GarageStageStepAngles(uint8_t* base, uint32_t command, uint32_t angles);
// sub_8217F9E0 GarageAnims::Add: an animation of `type` on the list (its
// 656-byte frame below `stack`, the entries copied through it with memcpy
// as the original does). Returns r3 as the original leaves it (the last
// memcpy's destination, or the list).
uint64_t GarageAnimsAdd(uint8_t* base, const Calls& calls, uint32_t list, uint32_t type,
                        uint32_t value, uint32_t index, uint32_t word, double steps,
                        uint32_t stack);
// sub_8217D2B0 GarageStage::ApplyPreset: camera and car preset `preset`
// applied now, or kept at the stage's +6088 when the garage camera isn't
// set up. `r3` is r3 at entry. Returns r3 as the original leaves it (tan's
// _ctrlfp word, or `r3`).
uint64_t GarageStageApplyPreset(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t preset,
                                uint32_t stage, uint32_t index, uint32_t stack);
// sub_82180108 GarageStage::Command: command `command` on the holder's
// stage (the animations, the angles, the presets). `r3` is the holder (r3 at
// entry, all 64 bits). Returns r3 as the original leaves it.
uint64_t GarageStageCommand(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t command,
                            uint32_t stack);

// --- The animations (garage_stage.cpp) ---
// An entry (28 bytes): +0 the type, +4 the steps, +8 a word, +12 the
// value's address, +16 the scene model's index, +20 done, +24 a float.

// sub_82179D78 GarageAnims::Turn: the yaw at [entry + 12] + 52 less 0.5°
// (wrapped); done (+20 = 1, returns 1) when +8 is 0 and the yaw is 0.0.
uint32_t GarageAnimsTurn(uint8_t* base, uint32_t entry);
// sub_82179DF0 GarageAnims::Open and sub_82179EA8 Close: the value at
// [entry + 12] stepped up (Open) or down (Close) by the car's maximum
// (0x82635E90 + 48 x the scene model's +48) / +4, clamped; MenuSound::Play
// (15) on the first step (Open) or at 0.0 (Close); 1 when done. Their
// 128- and 112-byte frames below `stack`.
uint32_t GarageAnimsOpen(uint8_t* base, const Calls& calls, uint32_t entry, uint32_t stack);
uint32_t GarageAnimsClose(uint8_t* base, const Calls& calls, uint32_t entry, uint32_t stack);
// sub_8217D120 GarageAnims::Run: each entry's runner by its type (0 Turn,
// 2 Open, 3 Close, 4 and 5 the doors' OpenDoors sub_8217B6B8 and CloseDoors
// sub_8217B740, never seen in play: guest calls); when any finished, the
// list rebuilt without the done ones through its 624-byte frame (memcpy,
// as the original). Returns whether any finished.
uint32_t GarageAnimsRun(uint8_t* base, const Calls& calls, uint32_t list, uint32_t stack);

// --- The light kit and the floor (garage_stage.cpp) ---

// sub_820D9D10 SceneModel::ApplyLightKit (docs/functions/load-poll.md):
// when the model is ready (+5036), not traffic and has a light kit
// (+5084), SceneLights::ApplyRecord(kit, model + 4224) (a tail call from
// `stack`, inside its diagnostics' wrap). Returns r3 as the original leaves
// it (`model`, 0 for no kit, the kit when --debug_hide names ApplyRecord,
// else ApplyRecord's).
uint64_t SceneModelApplyLightKit(uint8_t* base, const Calls& calls, uint32_t model,
                                 uint32_t stack);
// sub_82179678 GarageStage::SetLightKit: the light kit's group `group` (0
// to 3, 3 all three) set to `word` and the colors, +4224 the table's word
// at `index` (its 176-byte frame's +80), then ApplyLightKit from its frame.
uint64_t GarageStageSetLightKit(uint8_t* base, const Calls& calls, uint32_t holder,
                                uint32_t group, uint32_t word, uint32_t index, double f1,
                                double f2, double f3, uint32_t stack);
// sub_820C8480 Floor::Draw: with `word` set and the scene's +4 0, the
// matrix's last row (0, 0, 0, 1) (built at its 144-byte frame's +80) and
// each shape of the part model at scene + 4160 drawn
// (Model::DrawShape(model, 0x826BAC90, matrix, i, 0, 0, 0), `caller_r24`
// its r24). Returns r3 as the original leaves it.
uint64_t FloorDraw(uint8_t* base, const Calls& calls, uint32_t scene, uint32_t matrix,
                   uint32_t word, uint32_t caller_r24, uint32_t stack);

// --- The scene lights (lights.cpp) ---

// sub_8217BA20 GarageStage::SetLights: the scene lights' constants
// (0x8265ED00, 0x8296B950 to 0x8296BCDF; docs/known-data.md) from the
// stage's lights at `lights` (stage + 4512), y negated when `mirrored`:
// three lights, four spots (each spot's direction normalized as the vector
// unit does, its cone from cos, c_runtime::Cos), through its 368-byte frame
// below `stack` as the original. Returns r3 as the original leaves it (its
// frame's +128).
uint32_t GarageStageSetLights(uint8_t* base, uint32_t lights, uint32_t mirrored, uint32_t stack);

// --- The car's pose (pose.cpp) ---

// sub_8217CAB8 GarageStage::PoseCar: ScenePools::GetB (its answer unused);
// the scene model's block `block` (m + 4288): +732 = 1, the world matrix
// (+16) the identity raised by the body's height (stage +6144 = the mean of
// +6136 and +6140) and turned by its pitch (+6148 = atan2 of their
// difference over the wheelbase +6152) and the yaw (+84); the four wheels'
// turns (+336 + 64i: camber and toe) and placements (+80 + 64i); `out` the
// mirror (Matrix::FromScaleRotationTranslation, scale (1, -1, 1)). Through
// its 800-byte frame below `stack`, every matrix at the original's offsets
// and every Matrix::Multiply noted for interpolation with the original's
// return address. Returns r3 as the original leaves it (`out`).
uint32_t GarageStagePoseCar(uint8_t* base, const Calls& calls, uint32_t holder, uint32_t block,
                            uint32_t out, uint32_t stack);

}  // namespace front_end_car_draw
