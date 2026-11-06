// The race's update natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/race-update.md): the race scene's (`[0x826D82D4]`)
// update in its machine's state 38, its scenery load in state 1, and what
// they call. Plain C++ over guest memory (the scene, the cars and the scene
// lights' arrays are shared with the recompiled code), with the originals'
// loads and stores in their order. Adapters: src/hooks/race_update.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>

namespace course_scenery {
struct Calls;
}  // namespace course_scenery

namespace race_update {

// The scene lights' arrays RaceLights::UploadDirectional fills (16 bytes a
// slot): the directions (normalized), and the entries' +32 and +48.
constexpr uint32_t kDirections = 0x8296B950;
constexpr uint32_t kSecond = 0x8296B980;
constexpr uint32_t kThird = 0x8296B9B0;
// RaceLights::Update's (0, 0, 0, 1).
constexpr uint32_t kOrigin = 0x8265ED00;
// RaceLights::Update's frame (its r1 less this).
constexpr uint32_t kLightsFrame = 112;

// sub_8215B330 RaceScene::VaryCar (probably): a seed from the racer index,
// the vehicle index and the racer's +16 (`pick`), then the car's +1568
// bytes it varies by it (+1580 only when the car's word +12 is 64).
// Returns nothing: r3 is the car, as the original leaves it.
void VaryCar(uint8_t* base, uint32_t car, int32_t racer, int32_t vehicle, int32_t pick);

// sub_8215C698 RaceLights::UploadDirectional (probably): `entry`'s +16
// normalized into kDirections + 16 n, +32 into kSecond + 16 n and +48 into
// kThird + 16 n (in that order: the arrays overlap past slot 2). `stack` is
// its r1: the normalized vector also left at stack - 16 (the 1.0 it stores
// at stack - 4 first is overwritten there).
void UploadDirectional(uint8_t* base, uint32_t entry, uint32_t n, uint32_t stack);

// sub_8215D008 RaceLights::Update (probably): kOrigin = (0, 0, 0, 1) (also
// left at the frame's +80), then each of the 11 entries (+16 + 96 i) of kind
// 1 through UploadDirectional with n counting them. `frame` is its r1 less
// kLightsFrame. Returns n.
uint32_t LightsUpdate(uint8_t* base, uint32_t set, uint32_t frame);

// The race data ([0x826C5B2C]: +4 the course, +32 the mode, +0xA3AF0 4 in
// split screen) and the scenery configurations' two words by index.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kSceneryConfigs = 0x82602EA0;

// sub_8215B790 RaceScene::LoadScenery: Scenery::Create, the scene's +0 =
// the manager ([0x826C551C]); the 44-byte configuration at its frame's +80
// (`stack` its r1, the frame 160 bytes below): +0 = 1, +4 = 1 in split
// screen else 0, +8 to +20 = 0, +24 and +28 kSceneryConfigs' words at 8 x
// TimesThreePlus(1, 0) for courses 8 to 10, else TimesThreePlus(0, the
// mode), +36 and +40 = 0; +32 not written (the stack's word as the
// original leaves it: the B7 call of no behavior change). Then
// Scenery::Setup and StartLoad (their plain C++ with `calls`). Returns
// StartLoad's r3. Only where hooks::CourseScenerySetUpNative.
uint32_t LoadScenery(uint8_t* base, const course_scenery::Calls& calls, uint32_t stack,
                     uint32_t scene);

// A guest call the plain C++ makes: `function` with r3 up set to `args` (64
// bits each), r1 = `frame` and the link register `lr` (the original's
// return address); its r3. Flush-to-zero off after it.
using GuestCall = std::function<uint64_t(uint32_t function, std::initializer_list<uint64_t> args,
                                         uint32_t frame, uint32_t lr)>;

// sub_8215C0D8 RaceScene::RemakeCarModel: `slot` a car slot (+0 its scene
// model, +16 its 4,244-byte record, +4272 its pool index), `scene` the
// second set ([0x826C2E44]), `car` a racer record, `pick` the paint pick
// (64 bits, for SetPaint). 0 at once while the slot's model is loading
// (SceneModel::PollLoad 0); else the old one released (ScenePools::ReleaseB
// (scene, 7, +4272), +4272 = -1) when there, +4272 = ClaimB(scene, 7), the
// record from the car (SceneModelRecord::FromCar), +0 = ScenePools::SetUpB
// (scene, 7, +4272, +16); on its block (model + 4288) +8 = 1, +16 and +336
// the identity (memcpy from the frame's +80, where it's written as the
// original builds it); a traffic record (+16 3) whose +16 (slot +32) is 0
// or 7 gets +592 = 1. Returns 1. PollLoad, ReleaseB, FromCar and SetUpB
// through `call` (their adapters, inside their diagnostics' wraps) from its
// 208-byte frame below `stack` (its back chain written).
uint32_t RemakeCarModel(uint8_t* base, const GuestCall& call, uint32_t stack, uint32_t scene,
                        uint32_t slot, uint32_t car, uint64_t pick);

// sub_8215C780 Car::TickUpdate: `slots` the race scene's car slots (scene +
// 16: +0 the count, slot v at +16 + 4,288 v: +0 its scene model, +16 its
// record, +4272 its pool index). Pass 0's light direction into its frame's
// +96 (never read); each slot's seen flag (frame +592 + 4 v) and its model's
// block +8 and +12 cleared; for each racer (Race::RacerCount, the racers at
// [0x826D82F4] + 2308): its slot seen, its scene model remade
// (RemakeCarModel, after VaryCar for flag 0x2) when its flags have 0x200
// (cleared when it answers 1), then with a model its draw block (model +
// 4288) filled from the vehicle: hidden (the view's mode 2: the body's
// matrix and the light flags only) or drawn (+12 1 in the cockpit view; the
// light flags, the gauges, the body's matrix and the four wheels'
// matrices, +732 = 1). Last, each slot not seen whose model is 0 or done
// loading (SceneModel::PollLoad) has its pool index released
// (ScenePools::ReleaseB(scene, 7, index), index -1). Its 976-byte frame
// below `stack` (its back chain written; its register saves not), the
// identities and the matrices built there as the original. The callees'
// plain C++ directly; SceneModel::PollLoad, ScenePools::ReleaseB (inside
// their diagnostics' wraps), Stages::Copy and memcpy where they refuse
// through `call`; RemakeCarModel's callees as it makes them. Returns r3 as
// the original leaves it.
uint64_t TickUpdate(uint8_t* base, const GuestCall& call, uint32_t stack, uint32_t slots);

// The guest calls RaceScene::Update makes besides `call`'s: a call with r3
// and f1 (the post effect's vtable slot 24, Glow::SetFromSpeed, and
// ScreenObject::SetSpeed's original where its plain C++ refuses), returning
// r3; and Scenery::Update (sub_820F4558) on `manager` from `frame` with the
// original's return address and r28 = `scene`, as the original has it then
// (Scenery::Update's register saves store it).
struct UpdateCalls {
  GuestCall call;
  std::function<uint64_t(uint32_t function, uint32_t r3, double f1, uint32_t frame,
                         uint32_t lr)>
      call_f1;
  std::function<uint64_t(uint32_t manager, uint32_t scene, uint32_t frame)> scenery_update;
};

// sub_8215D2B0 RaceScene::Update: `scene` the race scene ([0x826D82D4]).
// RaceLights::Update(+30064), Scenery::Update(+0) through `calls`,
// TickUpdate(+16), PostEffects::MarkUpdated; then the followed cars'
// (Racers::Vehicle) glows (post effect 1, or 5 and 9 in split screen: its
// vtable slot 24 with the speed) and screen objects (+31152, or +31156 and
// +31160: ScreenObject::Start or Stop by the vehicle's byte +2424 and the
// object's +24, through `calls.call`, then ScreenObject::SetSpeed with the
// speed / 300). Its 144-byte frame below `stack` (its back chain written;
// its register and f30/f31 saves not). Returns r3 as the original leaves
// it.
uint64_t Update(uint8_t* base, const UpdateCalls& calls, uint32_t stack, uint32_t scene);

}  // namespace race_update
