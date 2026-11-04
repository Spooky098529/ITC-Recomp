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
};

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

}  // namespace front_end_car_draw
