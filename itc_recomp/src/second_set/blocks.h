// The second set's point blocks' constructors and resets as plain C++
// (docs/functions/second-set.md): each block's records constructed
// (PointRecord::PointRecord) and its groups reset or initialized, or reset
// again, over the point groups' plain C++ (point_groups.h). Guest data
// shared with the recompiled code; `stack` is the guest r1 at entry; each
// returns r3 as the original leaves it (the block for the constructors and
// inits, the last reset's r3 for the resets). Only where memcpy is native,
// and with --native_material_passes for the parts' (ResetRightDoor's
// ScaleBlended) (the adapters' check). Adapters: src/hooks/second_set.cpp.

#pragma once

#include <cstdint>

namespace second_set {

// PartsPoints::InitLeftDoor (sub_820DF5B8) and InitRightDoor (sub_820DF610):
// 25 PointRecord::PointRecord, the w's of the matrix at +6016 (+6028 to
// +6076) = 1.0, ResetLeftDoor or ResetRightDoor.
uint32_t PartsInitLeftDoor(uint8_t* base, uint32_t group, uint32_t stack);
uint32_t PartsInitRightDoor(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::InitHeadlights (sub_820E1388): 13 PointRecord::PointRecord,
// the w's of the matrix at +3120 = 1.0, ResetHeadlights.
uint32_t PartsInitHeadlights(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::InitMufflers (sub_820E0010): the w's of the four matrices at
// +0 = 1.0, five PointRecord::PointRecord at +256, ResetMufflers.
uint32_t PartsInitMufflers(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::PartsPoints (sub_820E2A10) and Reset (sub_820E00F0): the
// parts' block (the parts + 176).
uint32_t PartsPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack);
uint32_t PartsPointsReset(uint8_t* base, uint32_t block, uint32_t stack);
// CockpitPoints::CockpitPoints (sub_820DC190) and Reset (sub_820DBC10).
uint32_t CockpitPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack);
uint32_t CockpitPointsReset(uint8_t* base, uint32_t block, uint32_t stack);
// InteriorPoints::InteriorPoints (sub_820E4FD8) and Reset (sub_820E35D8).
uint32_t InteriorPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack);
uint32_t InteriorPointsReset(uint8_t* base, uint32_t block, uint32_t stack);
// LightPoints::LightPoints (sub_820DDCF8) and Reset (sub_820DC8A8).
uint32_t LightPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack);
uint32_t LightPointsReset(uint8_t* base, uint32_t block, uint32_t stack);
// TrafficPoints::TrafficPoints (sub_820E68E8) and Reset (sub_820E5880).
uint32_t TrafficPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack);
uint32_t TrafficPointsReset(uint8_t* base, uint32_t block, uint32_t stack);

// The wheels' group inits: the records PointRecord::PointRecord, 1.0 into
// the w's of the two rows at +1808 (the tire's), +1712 (the rims') or +2912
// (the brakes'), then the group's reset. WheelPoints::InitTire
// (sub_820E8A80, 6 records at +368), InitRimA (sub_820E99C0, 7 at +32),
// InitRimB (sub_820EA8E8, 7 at +32) and InitBrakes (sub_820EB320, 12 at +32).
// Return the group.
uint32_t WheelsInitTire(uint8_t* base, uint32_t group, uint32_t stack);
uint32_t WheelsInitRimA(uint8_t* base, uint32_t group, uint32_t stack);
uint32_t WheelsInitRimB(uint8_t* base, uint32_t group, uint32_t stack);
uint32_t WheelsInitBrakes(uint8_t* base, uint32_t group, uint32_t stack);
// WheelPoints::WheelPoints (sub_820ECC48): the inits at +32 and +1872
// (tires), +3712 and +5456 (rims A), +7200 and +8944 (rims B), +10688 and
// +13632 (brakes). Returns the block.
uint32_t WheelPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack);
// WheelPoints::Reset (sub_820E85C8): +0 to +24 = 0; the resets at the same
// offsets; the rear groups' front flags (+2172, +5468, +8956, +13644) = 0.
// Returns the last reset's r3.
uint32_t WheelPointsReset(uint8_t* base, uint32_t block, uint32_t stack);

// The point blocks' destructors: the empty destructor (sub_82106540) on each
// record, last first, so nothing is written. Return the r3 the original
// leaves (the last record given to it: the block's first). PartsPoints
// (sub_820E2C10), CockpitPoints (sub_820DC258), InteriorPoints
// (sub_820E50C0), LightPoints (sub_820DDD90; +64), WheelPoints (sub_820ECCB8;
// +400) and TrafficPoints (sub_820E6980; +16).
uint32_t PartsPointsDestruct(uint32_t block);
uint32_t CockpitPointsDestruct(uint32_t block);
uint32_t InteriorPointsDestruct(uint32_t block);
uint32_t LightPointsDestruct(uint32_t block);
uint32_t WheelPointsDestruct(uint32_t block);
uint32_t TrafficPointsDestruct(uint32_t block);

}  // namespace second_set
