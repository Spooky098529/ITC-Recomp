// The scene models' load poll as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/load-poll.md, the fields in
// docs/known-data.md): the parts' finishers (each part of a scene model
// handed to its second-set object once its first-set loading object has
// arrived) and the record's apply steps. The data is guest data shared with
// the recompiled code, so these work on guest memory, with the originals'
// loads and stores in their order. Each takes `stack`, the guest r1 at
// entry, writes its frame's back chain (not its saved registers) and runs
// its callees' plain C++ from that frame; each returns r3 as the original
// leaves it. Only with the callees' plain C++ native (the adapters' check,
// hooks::LoadPollCalleesNative). Adapters: src/hooks/load_poll.cpp.

#pragma once

#include <cstdint>
#include <functional>

#include "second_set/finds.h"
#include "second_set/wheels.h"

namespace load_poll {

// What the plain C++ can't do itself: the originals it calls from its
// frame where a callee's plain C++ declines, and the point set-ups' calls.
struct Calls {
  // The point set-ups' (Skeleton::UpdateMatrices' original; the flush mode
  // their last float step left, which the adapters leave).
  second_set::FindCalls find;
  // The wheels' point set-up's (WheelPoints::SetUp).
  second_set::WheelCalls wheel;
  // Out: the flush mode as the last float step left it (a point set-up's;
  // unchanged when none ran).
  second_set::Flush flush = second_set::Flush::kUnchanged;
  // memcpy's original (sub_825A7410) from the frame at `sp` with `lr`, where
  // native::Buffers_Memcpy declines.
  std::function<void(uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr)>
      memcpy;
  // Cockpit::BindTextures' (sub_820D6108) and TrafficCar::BindTextures'
  // (sub_820D7D38) originals (r3 = the object) from the frame at `sp` with
  // `lr`, where their plain C++ declines; their r3.
  std::function<uint64_t(uint32_t object, uint32_t sp, uint32_t lr)> cockpit_bind;
  std::function<uint64_t(uint32_t object, uint32_t sp, uint32_t lr)> traffic_bind;
  // PartModel::SetMaterials' original (sub_820D3318; r3 the model, r4 to
  // r10 `regs`) from the frame at `sp` with `lr`, where its plain C++
  // declines; its r3.
  std::function<uint64_t(uint32_t model, const uint64_t regs[7], uint32_t sp, uint32_t lr)>
      set_materials;
};

// --- The first set's wheels ---

// Wheels::SetRecord (sub_820D8EC0): memcpy(+28, record, 156) (a tail call).
// Returns memcpy's r3 (the destination).
uint64_t WheelsSetRecord(uint8_t* base, uint32_t wheels, uint32_t record, uint32_t stack,
                         Calls& calls);
// Wheels::SetCaliperTexture (sub_820D8ED0): PartModel::SetTexture on the
// calipers (+2336, +2800) with the record's +88 (+116), every shape.
uint64_t WheelsSetCaliperTexture(uint8_t* base, uint32_t wheels, uint32_t stack);

// --- The car model ---

// CarModel::IsReady (sub_820D4048): CarModel::IsLoaded for model sets 0
// and 1 ([[+7256] + 8]), else 0.
uint64_t CarModelIsReady(uint8_t* base, uint32_t car_model);
// CarParts::SetVinylTextures (sub_820D72D8): for each part model (+5280 +
// 464i; 216 in set a, 187 in set b, none otherwise, the set read again each
// time) but 7, 9 and 14 in set b: +48 = [+24] + 4048 and
// Model::SetVinylTextures(model, [[+24] + 6684], [[+24] + 6692]). `r3` is
// r3 at entry (left when nothing is called).
uint64_t CarPartsSetVinylTextures(uint8_t* base, uint32_t parts, uint64_t r3, uint32_t stack);

// --- The interior ---

// InteriorDials::SetBlock (sub_820E3040): +4 = `block`, +2896 =
// sub_820D3728(block); +8 = the scene + 1132 for set B (sub_820D3638), else
// + 1064 for set A (sub_820D3600), else left. r3 the last test's answer.
uint64_t InteriorDialsSetBlock(uint8_t* base, uint32_t dials, uint64_t block);
// InteriorMeters::SetGauge (sub_820E32E8; never seen in play): +4 x (1324 +
// gauge) = `entry`; PartModel::SetTexture on the gauge's point record's
// model and shape with the texture index [+0] + 12 x gauge + 16 (3 above 2
// when the scene's +4 is set); +4 x (1332 + gauge) = sub_820D3658([+0] + 12
// x gauge + 12's word, the index's word read again).
uint64_t InteriorMetersSetGauge(uint8_t* base, uint32_t meters, uint64_t gauge, uint64_t entry,
                                uint32_t stack);
// SceneInterior::SetGauges (sub_820E3938): for gauges 0 to 6 (not 1 when
// the car's block is 5) whose record entry (+48 + 12i) is set,
// InteriorMeters::SetGauge(+3312, i, [+11668] + 20 x the entry's second
// word + 4864). `r3` is r3 at entry (left when nothing is called).
uint64_t SceneInteriorSetGauges(uint8_t* base, uint32_t interior, uint64_t r3, uint32_t stack);

// --- The light kit and the traffic body ---

// SceneLights::Finish (sub_820DDA60): 1 when +13156 is set; else 0 unless
// Pool4Object::IsLoaded([+28]); then +13156 = 1, +13152 = [+28] + 112,
// PartModel::SetTexture(it, [+32], -1), its +48 = [+13160] + 364 and
// LightPoints::Find(+96, it); 1.
uint64_t SceneLightsFinish(uint8_t* base, uint32_t kit, uint32_t stack, Calls& calls);
// SceneLights::SetRecord (sub_820DCB70): memcpy(+32, record, 52), then
// Pool4Object::SetRecord([+28], record). Returns its r3.
uint64_t SceneLightsSetRecord(uint8_t* base, uint32_t kit, uint32_t record, uint32_t stack,
                              Calls& calls);
// SceneLights::ApplyRecord (sub_820DCBC0): +104 to +135 = 0, SetRecord, then
// PartModel::SetTexture([+13152], [+32], -1).
uint64_t SceneLightsApplyRecord(uint8_t* base, uint32_t kit, uint32_t record, uint32_t stack,
                                Calls& calls);
// SceneTraffic::Finish (sub_820E5A68): 1 when +9792 is set; else
// TrafficCar::BindTextures([+9760]), returned when 0; then +9764 = [+9760]
// + 3776, +9768 + 4i = [+9760] + 4256 + 464i, the point block's +1460 and
// +1456 (+3728) = the body's +7048 and +7040, TrafficPoints::Find(+3728,
// +9768); 1.
uint64_t SceneTrafficFinish(uint8_t* base, uint32_t body, uint32_t stack, Calls& calls);

// --- The paint ---
//
// The paint's registers: r4 to r9 (three 16-byte colors, two doublewords
// each), passed on as they came.

// SceneParts::ApplyPaint (sub_820E0908): sub_820D7388([+104], regs) (a tail
// call).
uint64_t ScenePartsApplyPaint(uint8_t* base, uint32_t parts, const uint64_t regs[6],
                              uint32_t stack);
// SceneInterior::ApplyPaint (sub_820E4250): sub_820D7840([+36], regs) (a
// tail call).
uint64_t SceneInteriorApplyPaint(uint8_t* base, uint32_t interior, const uint64_t regs[6],
                                 uint32_t stack);
// SceneCockpit::ApplyPaint (sub_820DC138): sub_820D6328([+32], regs), then
// SceneInterior::ApplyPaint([+28], regs).
uint64_t SceneCockpitApplyPaint(uint8_t* base, uint32_t cockpit, const uint64_t regs[6],
                                uint32_t stack);
// TrafficCar::SetPaint (sub_820D7E18; never seen in play): PartModel::SetPaint
// on each of the six part models (+4256 + 464i) whose slot state (+4124 +
// 24i) is 3. `r3` is r3 at entry (left when nothing is called).
uint64_t TrafficCarSetPaint(uint8_t* base, uint32_t traffic, uint64_t r3, const uint64_t regs[6],
                            uint32_t stack);
// SceneTraffic::ApplyPaint (sub_820E61D0; never seen in play):
// TrafficCar::SetPaint([+9760], regs) (a tail call).
uint64_t SceneTrafficApplyPaint(uint8_t* base, uint32_t body, const uint64_t regs[6],
                                uint32_t stack);
// SceneModel::ApplyPaint (sub_820D9B68): when ready (+5036), the record's
// three colors (+64, +80, +96) to SceneTraffic::ApplyPaint([+5092]) for
// traffic (+32 = 3), else to SceneParts::ApplyPaint([+5060]),
// SceneInterior::ApplyPaint([+5068]) and, with a cockpit (+5076),
// CarModel::SetVinylTextures([[+5076] + 32]) and
// SceneCockpit::ApplyPaint([+5076]). `r3` is r3 at entry (left when
// nothing is called).
uint64_t SceneModelApplyPaint(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack);

// The wheels' paint: six 16-byte values v0 to v5, the first three and
// v3's first half in r4 to r10 (`regs`), v3's second half, v4 and v5 in the
// caller's frame (`stack` + 88, + 96, + 112: the parameter area as the
// original passes them).
//
// Wheels::ApplyPaint (sub_820D91E0): PartModel::SetPaintBody on the tires
// +240 with (v1, v0, v2) and +704 with (v4, v3, v5), the rims or brakes
// +1216 with (v0, v1, v2) and +1680 with (v3, v4, v5).
uint64_t WheelsApplyPaint(uint8_t* base, uint32_t wheels, const uint64_t regs[7], uint32_t stack);
// SceneWheels::ApplyPaint (sub_820E8A00): v5 and v4 copied (memcpy) into its
// frame's +112 and +96 and v3's second half to +88, then
// Wheels::ApplyPaint([+28], the same).
uint64_t SceneWheelsApplyPaint(uint8_t* base, uint32_t object, const uint64_t regs[7],
                               uint32_t stack, Calls& calls);
// SceneModel::ApplyWheelPaint (sub_820D9C50): when ready (+5036), not
// traffic (+32 = 3), with +4072 set and wheels (+5100): the record's +112
// to +207 as v0 to v5 (v5 and v4 copied into its frame first) to
// SceneWheels::ApplyPaint([+5100]). `r3` is r3 at entry (left when nothing
// is called).
uint64_t SceneModelApplyWheelPaint(uint8_t* base, uint32_t model, uint64_t r3, uint32_t stack,
                                   Calls& calls);

// --- The finishers' builds ---

// SceneInterior::Finish (sub_820E4140): 1 when +11660 is set. Else
// Interior::IsLoaded([+36]), returned as is; when it's nonzero: the six
// part models +11632 + 4i = [+36] + 304 + 464i, each (when not 0) with +48 =
// +32 and +452 = +11664 under a cockpit (+11656), else
// CarModel::SelectedTexture([[+24] + 24]); InteriorDials::SetBlock(+400,
// the car's block [[[+24] + 24] + 32]); +3316 = the block, +8636 = +32,
// +11604 = +40, +11600 = [[+24] + 24] + 7168, +11608 = [[+24] + 24] + 3760;
// InteriorPoints::Find(+160, +11632), SceneInterior::SetGauges, +11660 = 1.
uint64_t SceneInteriorFinish(uint8_t* base, uint32_t interior, uint32_t stack, Calls& calls);
// SceneWheels::SetUpPoints (sub_820E8908): nothing when +16772 is set. Else
// the point block (+192): +0 = +24 when set, else +4 = +16776 and +8 =
// +16780; +12 = the object + 32, +16, +20, +24 = [+28] + 240, + 1216, +
// 2336; WheelPoints::SetUp(+192), Wheels::SetRecord([+28], +32),
// Wheels::SetCaliperTexture([+28]), +16772 = 1. `r3` is r3 at entry (left
// when nothing is done).
uint64_t SceneWheelsSetUpPoints(uint8_t* base, uint32_t object, uint64_t r3, uint32_t stack,
                                Calls& calls);
// SceneParts::SetSelectedTexture (sub_820DF250): each of the 17 part models
// present (+32 + 4k) gets +452 = CarModel::SelectedTexture([[+24] + 24]).
// `r3` is r3 at entry (left when none is present).
uint64_t ScenePartsSetSelectedTexture(uint8_t* base, uint32_t parts, uint64_t r3, uint32_t stack);
// SceneParts::Build (sub_820E05E0): +32 + 4k = CarParts::Part([+104], k, a,
// b) for the 17 kinds (the record's words at +108 to +156, see the row;
// kind 4's +124 at 13 gives 0 without the call); PartsPoints::Reset(+176);
// +37260 = +24, +37256 = +108, +37252 = [[+24] + 24] + 32, +37268 = [[+24] +
// 24] + 3760; PartsPoints::Find(+176, +32), CarParts::WalkMaterials([+104])
// and SetSelectedTexture.
uint64_t ScenePartsBuild(uint8_t* base, uint32_t parts, uint32_t stack, Calls& calls);

// --- The wheels' materials and the last finishers ---

// Wheels::BindTextures (sub_820D9638): 0 while the calipers' second slot
// (+2324) is loading (2). Else the models' texture providers (+48: the
// tires the scene + 208, the rims or brakes +2240 and +2260, the calipers
// the scene + 296), PartModel::SetTexture on the tires with +40 and +56,
// then PartModel::SetMaterials on each tire from its frame (flag 5 at
// +176, -1 at +132, +140, +148, 1.0 at +204, +220, +236 and 0.0 at +244,
// +224 copied to +96 by memcpy; the words it never writes passed on as they
// are). Returns 1.
uint64_t WheelsBindTextures(uint8_t* base, uint32_t wheels, uint32_t stack, Calls& calls);
// SceneWheels::Finish (sub_820E7328): 1 when +16768 is set; else
// Wheels::BindTextures([+28]), returned when 0; then +16768 = 1 and 1.
uint64_t SceneWheelsFinish(uint8_t* base, uint32_t object, uint32_t stack, Calls& calls);
// Wheels::ApplyPalette (sub_820D92A0): PartModel::ApplyPalette on its six
// models, then PartModel::SetMaterials twice on each tire and rim or brake
// (the category at +140 1, then 2), the floats at the frame's +240 and +244
// the record's +20 and +24 (+48, +52) for +240 and +1216, +36 and +40 (+64,
// +68) for +704 and +1680; the frame as Wheels::BindTextures' (its
// unwritten words passed on).
uint64_t WheelsApplyPalette(uint8_t* base, uint32_t wheels, uint32_t stack, Calls& calls);
// SceneParts::Finish (sub_820E2E68): 1 when +100 is set; else +100 =
// CarParts::IsLoaded([+104]); when it's set and the scene's ([+37284]) +4
// is 0 or 1, SceneParts::Build. Returns the loaded answer.
uint64_t ScenePartsFinish(uint8_t* base, uint32_t parts, uint32_t stack, Calls& calls);
// SceneCockpit::Finish (sub_820DBFF8): 1 when +4312 is set. Else 0 unless
// Cockpit::BindTextures([+32]); its interior's (+28) +11664 = [[+32] +
// 3504] and SceneInterior::Finish(+28), 0 unless done; then +4312 = 1,
// +4288 + 4i = [+32] + 688 + 464i and CockpitPoints::Find(+192, +4288); 1.
uint64_t SceneCockpitFinish(uint8_t* base, uint32_t cockpit, uint32_t stack, Calls& calls);

}  // namespace load_poll
