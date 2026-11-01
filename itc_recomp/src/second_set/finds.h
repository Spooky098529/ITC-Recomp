// The second set's point set-ups as plain C++ (docs/functions/second-set.md):
// the parts' and the cockpit's `Find` functions, which find their point
// records in the part models once they've arrived (ModelPoint::Find for the
// nodes, ModelPoint::FindShape for the shapes) and scale the blended
// materials of some (PartModel::ScaleBlended). Guest data shared with the
// recompiled code; `stack` is the guest r1 at entry; each returns r3 as the
// original leaves it (its last call's). Only with native memcpy,
// --native_models and --native_material_passes (the adapters' check).
// Adapters: src/hooks/second_set.cpp.

#pragma once

#include <cstdint>
#include <functional>

namespace second_set {

// What the set-ups can't do as plain C++, and what they leave behind.
struct FindCalls {
  // Skeleton::UpdateMatrices' original (r3 = the skeleton) where its plain
  // C++ declines, from ModelPoint::Find's frame at `frame` with its return
  // address.
  std::function<void(uint32_t skeleton, uint32_t frame)> update_matrices;
  // Out: whether the last float step left flush-to-zero on (ModelPoint::Find
  // finding its node, as Matrix::Inverse's adapter leaves it) or off.
  bool flush = false;
};

// PartsPoints::FindFrontBumper (sub_820DE140): r3 = the group, r4 = the
// bumper's part model: FindShape on its 11 records; ScaleBlended by 5.0 on
// records 3 to 5; when record 8 found a model, its +456 = the group's +2644.
uint32_t PartsFindFrontBumper(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                              FindCalls& calls);
// PartsPoints::FindRearBumper (sub_820DE328): FindShape on 7 records;
// ScaleBlended by 1.0 on record 1, by 5.0 on 2 to 4; when record 6 found a
// model, its +456 = the group's +1684.
uint32_t PartsFindRearBumper(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls);
// PartsPoints::FindRearSpoiler (sub_820DE4E8): FindShape on 3 records;
// ScaleBlended by 5.0 on record 2, then 1.
uint32_t PartsFindRearSpoiler(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                              FindCalls& calls);
// PartsPoints::FindBonnet (sub_820DE5B0): ModelPoint::Find on record 0,
// FindShape on 1.
uint32_t PartsFindBonnet(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         FindCalls& calls);
// PartsPoints::FindLeftDoor (sub_820DE668): ModelPoint::Find on records 0
// and 1, FindShape on 2 to 24; ScaleBlended by 5.0 on 7 to 10.
uint32_t PartsFindLeftDoor(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls);
// PartsPoints::FindRightDoor (sub_820DE878): as the left without scaling.
uint32_t PartsFindRightDoor(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                            FindCalls& calls);
// PartsPoints::FindDoors (sub_820DEA20): FindShape on 23 records;
// ScaleBlended by 5.0 on 3 to 10.
uint32_t PartsFindDoors(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                        FindCalls& calls);
// PartsPoints::FindHeadlights (sub_820DF7F0): ModelPoint::Find on 0 to 6,
// FindShape on 7 to 12; when record 0 found its model, +3184 = the float at
// +16 of its node's entry ([[record] + 136] + 144 x +224), then ScaleBlended
// by 5.0 on 7 to 10.
uint32_t PartsFindHeadlights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls);
// PartsPoints::FindTailLights (sub_820DEC58): ModelPoint::Find on 0 to 7,
// FindShape on 8 to 14; ScaleBlended by 5.0 on 8 to 12.
uint32_t PartsFindTailLights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls);
// PartsPoints::FindFenders (sub_820DED20): FindShape on 7; ScaleBlended by
// 5.0 on 3 and 4.
uint32_t PartsFindFenders(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                          FindCalls& calls);
// PartsPoints::FindCabin (sub_820DEED8): FindShape on 5; ScaleBlended by 5.0
// on 0 and 1.
uint32_t PartsFindCabin(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                        FindCalls& calls);
// PartsPoints::FindMufflers (sub_820E0080): for records 0 to 3 (at +256),
// ModelPoint::Find, the record's +16 copied to the frame (memcpy) and from
// there to the group's matrix i (+64i, Matrix::Copy, noted for frame
// interpolation); record 4 FindShape.
uint32_t PartsFindMufflers(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls);
// PartsPoints::Find (sub_820E0258): r3 = the block, r4 = the 17 part models
// (words; 0 for one not there): the words from +37076 to +37092 and the car
// model's loading object copied by the scene's model set ([[+37088] + 4]),
// then each kind's set-up by the set (see the row).
uint32_t PartsFind(uint8_t* base, uint32_t block, uint32_t models, uint32_t stack,
                   FindCalls& calls);

// CockpitPoints::FindHeadlights (sub_820DBB08): ModelPoint::Find on 0 to 3,
// FindShape on 4 to 7; ScaleBlended by 1.0 on 5 and 6.
uint32_t CockpitFindHeadlights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls);
// CockpitPoints::Find (sub_820DBC88): r3 = the block, r4 = six part models
// (words): FindShape on the fenders (3, model 4), the cabin (2 at +720, 5),
// the bonnet (+1200, 0), the doors (2 at +1440, 1), the mirror (+1920, 2),
// then FindHeadlights (+2160, 3).
uint32_t CockpitFind(uint8_t* base, uint32_t block, uint32_t models, uint32_t stack,
                     FindCalls& calls);

// InteriorPoints::FindDials (sub_820E30C0): ModelPoint::Find on records 0
// and 1 (at +16), FindShape on 2 to 11; when the group's +4 is one
// IsBlockSetB or IsBlockSetA accepts, ScaleBlended by 1.0 on 9 to 11.
uint32_t InteriorFindDials(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls);
// InteriorPoints::FindMeters (sub_820E3298): ModelPoint::Find on records 0
// to 6 (at +16), FindShape on 7 to 21.
uint32_t InteriorFindMeters(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                            FindCalls& calls);
// InteriorPoints::FindSeats (sub_820E34A8): ModelPoint::Find on 0 and 1 (at
// +16), FindShape on 2 and 3.
uint32_t InteriorFindSeats(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls);
// InteriorPoints::Find (sub_820E3660): r4 = six part models (words): the
// groups' +0 = [+11444], +9492 = [+11440], +9496 = the scene + 784 + 464 x
// [[+11444] + 100] when that isn't 0; then FindShape(+0, model 2),
// FindDials(+240, 3), FindMeters(+3152, 0), the steering's 4 (model 5),
// FindSeats(+9488, 4), the roll cage's 4 (model 1).
uint32_t InteriorFind(uint8_t* base, uint32_t block, uint32_t models, uint32_t stack,
                      FindCalls& calls);
// LightPoints::FindGroup (sub_820DC850): for its 18 records (at +16),
// FindShape then ScaleBlended by 1.0.
uint32_t LightsFindGroup(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         FindCalls& calls);
// LightPoints::Find (sub_820DC900): FindGroup on +48, +4384, +8720.
uint32_t LightsFind(uint8_t* base, uint32_t block, uint32_t model, uint32_t stack,
                    FindCalls& calls);
// TrafficPoints::FindBody (sub_820E5538): r4 to r7 models 0 to 3:
// FindShape on its 6 records with models 3, 0, 2, 2, 0, 1.
uint32_t TrafficFindBody(uint8_t* base, uint32_t group, uint32_t m0, uint32_t m1, uint32_t m2,
                         uint32_t m3, uint32_t stack);
// TrafficPoints::FindHeadlights (sub_820E5738): ModelPoint::Find on 0 to 3,
// FindShape on 4 to 6; ScaleBlended by 3.0 on 5.
uint32_t TrafficFindHeadlights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls);
// TrafficPoints::FindTailLights (sub_820E5808): ModelPoint::Find on 0 to 7,
// FindShape on 8 to 11; ScaleBlended by 3.0 on 9 and 10.
uint32_t TrafficFindTailLights(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls);
// TrafficPoints::Find (sub_820E58C8): r4 = six part models (words):
// FindBody(+16, models 0 to 3), FindHeadlights(+1472, 4),
// FindTailLights(+3152, 5).
uint32_t TrafficFind(uint8_t* base, uint32_t block, uint32_t models, uint32_t stack,
                     FindCalls& calls);

}  // namespace second_set
