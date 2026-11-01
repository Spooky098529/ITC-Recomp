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

namespace load_poll {

// What the plain C++ can't do itself: the originals it calls from its
// frame where a callee's plain C++ declines, and the point set-ups' calls.
struct Calls {
  // The point set-ups' (Skeleton::UpdateMatrices' original; the flush mode
  // their last float step left, which the adapters leave).
  second_set::FindCalls find;
  // memcpy's original (sub_825A7410) from the frame at `sp` with `lr`, where
  // native::Buffers_Memcpy declines.
  std::function<void(uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr)>
      memcpy;
  // Cockpit::BindTextures' (sub_820D6108) and TrafficCar::BindTextures'
  // (sub_820D7D38) originals (r3 = the object) from the frame at `sp` with
  // `lr`, where their plain C++ declines; their r3.
  std::function<uint64_t(uint32_t object, uint32_t sp, uint32_t lr)> cockpit_bind;
  std::function<uint64_t(uint32_t object, uint32_t sp, uint32_t lr)> traffic_bind;
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

}  // namespace load_poll
