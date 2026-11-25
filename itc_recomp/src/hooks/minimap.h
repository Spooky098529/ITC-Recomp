// The minimap's register steps (src/minimap/, docs/functions/minimap.md),
// shared by the adapters (src/hooks/minimap.cpp, behind --native_minimap)
// and the harness (without the flag). Each leaves r3 as the original does,
// and runs the original where the plain C++ declines.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Race::FindPlayerEntry (sub_82170608): r3 = the manager, r4 = which.
void MinimapFindPlayerEntry(lifted::Regs& ctx);
// Race::RacerVehicle (sub_82170578): r3 = the racer.
void MinimapRacerVehicle(lifted::Regs& ctx);
// SetList::Point (sub_82147D18): r3 = the list, r4 = out, r5 to r7 the
// entry, point and part (r3 left).
void MinimapSetListPoint(lifted::Regs& ctx);
// Minimap::RacerMarkerType (sub_820F2D28): r3 = the entry, r4 = the
// player's branch number; r3 = the type (sign-extended).
void MinimapRacerMarkerType(lifted::Regs& ctx);
// Minimap::Scaling (sub_820F2EF8): r3 = out, r4 = the vector; r3 = out (as
// memcpy leaves it).
void MinimapScaling(lifted::Regs& ctx);
// Triangle::Init (sub_820F4040): r3 = the triangle (left). No adapter: the
// harness's steps.
void MinimapTriangleInit(lifted::Regs& ctx);
// List80::Insert (sub_820F5238): r3 = the list, r4 = the matrix, r5 = the
// position; r3 = the data slot (as Matrix::Copy leaves it).
void MinimapList80Insert(lifted::Regs& ctx);
// List80::Copy (sub_820F51A8): r3 = out, r4 = the list, r5 = the position;
// r3 = out.
void MinimapList80Copy(lifted::Regs& ctx);
// Primitive::RecordTransform (sub_8217AEA8): r3 = the camera pass, r4 = the
// primitive; r3 = the last slot.
void MinimapRecordTransform(lifted::Regs& ctx);

// Triangle::Record (sub_8217DAA0): r3 = the triangle, r4 = the camera
// pass, r5 = the sort key: by the kind, Triangle::RecordFlat's step, the
// original sub_8217C4C0 (never seen in play), or nothing.
void MinimapTriangleRecord(lifted::Regs& ctx);
// Triangle::RecordFlat (sub_8217C370), the same registers: the chunk
// natively, handed to the render queue at [0x826D83D0] (its slot 4, a guest
// call) from a 128-byte frame like the original's; r3 as that call leaves
// it. The original with the command list's diagnostics on, or where the
// plain C++ declines.
void MinimapTriangleRecordFlat(lifted::Regs& ctx);

// Minimap::Draw (sub_820F5740): r3 = the manager, r4 = the pass
// description (r24 the map's draws' context); lr put back, r3 not given
// (its caller doesn't read it). Run by frame interpolation's wrap when
// MinimapDrawNative: --native_minimap with native atexit, its callees'
// flags and the command list's diagnostics off. `caller_r24`, the caller's
// r24 its draws hand their recorder's context (a diagnostic word), isn't
// carried (the rewiring, stage 3): its callers pass 0.
bool MinimapDrawNative();
void MinimapDraw(lifted::Regs& ctx);
void MinimapDraw(lifted::Regs& ctx, uint32_t caller_r24);

void MinimapDrawCarMarker(lifted::Regs& ctx);
void MinimapDrawPointMarker(lifted::Regs& ctx);

}  // namespace hooks
