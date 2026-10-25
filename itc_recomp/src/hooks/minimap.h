// The minimap's register steps (src/minimap/, docs/functions/minimap.md),
// shared by the adapters (src/hooks/minimap.cpp, behind --native_minimap)
// and the harness (without the flag). Each leaves r3 as the original does,
// and runs the original where the plain C++ declines.

#pragma once

#include "guest.h"

namespace hooks {

// Race::FindPlayerEntry (sub_82170608): r3 = the manager, r4 = which.
void MinimapFindPlayerEntry(PPCContext& ctx, uint8_t* base);
// Race::RacerVehicle (sub_82170578): r3 = the racer.
void MinimapRacerVehicle(PPCContext& ctx, uint8_t* base);
// SetList::Point (sub_82147D18): r3 = the list, r4 = out, r5 to r7 the
// entry, point and part (r3 left).
void MinimapSetListPoint(PPCContext& ctx, uint8_t* base);
// Minimap::RacerMarkerType (sub_820F2D28): r3 = the entry, r4 = the
// player's branch number; r3 = the type (sign-extended).
void MinimapRacerMarkerType(PPCContext& ctx, uint8_t* base);
// Minimap::Scaling (sub_820F2EF8): r3 = out, r4 = the vector; r3 = out (as
// memcpy leaves it).
void MinimapScaling(PPCContext& ctx, uint8_t* base);
// Triangle::Init (sub_820F4040): r3 = the triangle (left).
void MinimapTriangleInit(PPCContext& ctx, uint8_t* base);
// List80::Insert (sub_820F5238): r3 = the list, r4 = the matrix, r5 = the
// position; r3 = the data slot (as Matrix::Copy leaves it).
void MinimapList80Insert(PPCContext& ctx, uint8_t* base);
// List80::Copy (sub_820F51A8): r3 = out, r4 = the list, r5 = the position;
// r3 = out.
void MinimapList80Copy(PPCContext& ctx, uint8_t* base);
// Primitive::RecordTransform (sub_8217AEA8): r3 = the camera pass, r4 = the
// primitive; r3 = the last slot.
void MinimapRecordTransform(PPCContext& ctx, uint8_t* base);

// Triangle::Record (sub_8217DAA0): r3 = the triangle, r4 = the camera
// pass, r5 = the sort key: by the kind, Triangle::RecordFlat's step, the
// original sub_8217C4C0 (never seen in play), or nothing.
void MinimapTriangleRecord(PPCContext& ctx, uint8_t* base);
// Triangle::RecordFlat (sub_8217C370), the same registers: the chunk
// natively, handed to the render queue at [0x826D83D0] (its slot 4, a guest
// call) from a 128-byte frame like the original's; r3 as that call leaves
// it. The original with the command list's diagnostics on, or where the
// plain C++ declines.
void MinimapTriangleRecordFlat(PPCContext& ctx, uint8_t* base);

void MinimapDrawCarMarker(PPCContext& ctx, uint8_t* base);
void MinimapDrawPointMarker(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
