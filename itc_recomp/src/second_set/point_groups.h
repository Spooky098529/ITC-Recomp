// The second set's point groups' resets and single records' inits as plain
// C++ (docs/functions/second-set.md): each group's records cleared
// (PointRecord::Clear) and named (SetNode for the first `nodes`, SetShape for
// the rest) from a table of string pointers in the image, with the few words
// each sets. `stack` is the guest r1 at entry; each returns r3 as the
// original leaves it (the last record named, unless said). Only where memcpy
// is native (the adapters' check). Adapters: src/hooks/second_set.cpp.

#pragma once

#include <cstdint>

namespace second_set {

// --- The parts' (PartsPoints, the block at the parts + 176) ---

// PartsPoints::ResetFrontBumper (sub_820DE0E0): +2640 = 1, +2644 and +2648
// = 0; 11 shapes from 0x826025F4.
uint32_t PartsResetFrontBumper(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetRearBumper (sub_820DE2C8): +1680 = 1, +1684 = 0; 7
// shapes from 0x82602624.
uint32_t PartsResetRearBumper(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetRearSpoiler (sub_820DE498): 3 shapes from 0x82602644.
uint32_t PartsResetRearSpoiler(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetBonnet (sub_820DE548): a node and a shape from
// 0x82602650.
uint32_t PartsResetBonnet(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetLeftDoor (sub_820DE600): +6080 = 0; 2 nodes and 23
// shapes from 0x82602658.
uint32_t PartsResetLeftDoor(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetRightDoor (sub_820DE7E8): 2 nodes and 23 shapes from
// 0x826026C0; then PartModel::ScaleBlended(record +4, record +228, 5.0) on
// records 7 to 10. Returns the last ScaleBlended's r3. Only with
// --native_material_passes (the adapter's check).
uint32_t PartsResetRightDoor(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetDoors (sub_820DE9D0): 23 shapes from 0x82602728.
uint32_t PartsResetDoors(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetHeadlights (sub_820DF740): 7 nodes and 6 shapes from
// 0x82602788; the identity (the frame's +80) into +3120 (memcpy). Returns
// memcpy's r3 (+3120).
uint32_t PartsResetHeadlights(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetTailLights (sub_820DEBF0): 8 nodes and 7 shapes from
// 0x826027BC.
uint32_t PartsResetTailLights(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetFenders (sub_820DECD0): 7 shapes from 0x826027F8.
uint32_t PartsResetFenders(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetCabin (sub_820DEE88): 5 shapes from 0x82602814.
uint32_t PartsResetCabin(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetEngine (sub_820DF020): 2 shapes from 0x82602828.
uint32_t PartsResetEngine(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetMufflers (sub_820DF0C0): 4 nodes and a shape from
// 0x82602830, the records at +256.
uint32_t PartsResetMufflers(uint8_t* base, uint32_t group, uint32_t stack);
// PartsPoints::ResetShadows (sub_820DF128): +960 = 0; 4 shapes from
// 0x82602844.
uint32_t PartsResetShadows(uint8_t* base, uint32_t group, uint32_t stack);
// The single records' inits: PointRecord::PointRecord, Clear, SetShape
// with the name at the word given; return the record. PartsPoints'
// InitCooling (sub_820DF410, [0x82602620]), InitSideSkirts (sub_820DF570,
// [0x82602640]), InitLeftMirror (sub_820DF668, [0x826026BC]),
// InitRightMirror (sub_820DF6B0, [0x82602724]), InitMirror (sub_820DF6F8,
// [0x82602784]); CockpitPoints' InitBonnet (sub_820DC0A8, [0x8200524C]) and
// InitMirror (sub_820DC0F0, [0x82005258]).
uint32_t PartsInitCooling(uint8_t* base, uint32_t record, uint32_t stack);
uint32_t PartsInitSideSkirts(uint8_t* base, uint32_t record, uint32_t stack);
uint32_t PartsInitLeftMirror(uint8_t* base, uint32_t record, uint32_t stack);
uint32_t PartsInitRightMirror(uint8_t* base, uint32_t record, uint32_t stack);
uint32_t PartsInitMirror(uint8_t* base, uint32_t record, uint32_t stack);
uint32_t CockpitInitBonnet(uint8_t* base, uint32_t record, uint32_t stack);
uint32_t CockpitInitMirror(uint8_t* base, uint32_t record, uint32_t stack);

// --- The cockpit's (CockpitPoints, the block at the cockpit + 192) ---

// CockpitPoints::ResetFenders (sub_820DB878): 3 shapes from 0x82005238.
uint32_t CockpitResetFenders(uint8_t* base, uint32_t group, uint32_t stack);
// CockpitPoints::ResetCabin (sub_820DB930): 2 shapes from 0x82005244.
uint32_t CockpitResetCabin(uint8_t* base, uint32_t group, uint32_t stack);
// CockpitPoints::ResetDoors (sub_820DB9E8): 2 shapes from 0x82005250.
uint32_t CockpitResetDoors(uint8_t* base, uint32_t group, uint32_t stack);
// CockpitPoints::ResetHeadlights (sub_820DBAA0): 4 nodes and 4 shapes from
// 0x8200525C.
uint32_t CockpitResetHeadlights(uint8_t* base, uint32_t group, uint32_t stack);

}  // namespace second_set
