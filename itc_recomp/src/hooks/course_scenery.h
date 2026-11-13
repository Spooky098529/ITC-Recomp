// The course scenery's register steps shared by its adapters
// (src/hooks/course_scenery.cpp, behind --native_course_scenery) and the
// harness (without the flag); docs/functions/course-scenery.md.

#pragma once

#include "guest.h"

#include "course_scenery/course_scenery.h"

namespace hooks {

// Scenery::DrawSetLists (sub_820F52A8): r3 = the manager, r4 = the pass
// description; r3 as the original leaves it. Its original runs with the
// command list's diagnostics on (as the recorders'). r24 is the
// view-projection's address while it runs (the draws' adapters read it
// where their plain C++ declines), then put back.
void CourseSceneryDrawSetLists(PPCContext& ctx, uint8_t* base);

// Scenery::RecordHeadlightProjectors (sub_820F34C8): r4 = the pass
// description; r3 as the original leaves it, lr put back. The original
// with --native_cameras, --native_matrix_math or --native_vehicle_accessors
// off or the command list's diagnostics on.
void CourseSceneryRecordHeadlightProjectors(PPCContext& ctx, uint8_t* base);

// Scenery::DrawArea (sub_820F3B70): r3 = the manager, r4 = the pass
// description, r5 = the area; lr put back (r3 not given: its caller doesn't
// read it). CourseSceneryDrawAreaNative: whether its plain C++ may run (its
// callees' flags, the command list's diagnostics off).
bool CourseSceneryDrawAreaNative();
void CourseSceneryDrawArea(PPCContext& ctx, uint8_t* base);

// Scenery::Draw (sub_820F6068): r3 = the manager; lr, r24 and flush-to-zero
// (off) put back, r3 not given (its callers don't read it). Run by its
// frame-state wrap (src/debug/frame_state_diff.cpp) where
// CourseSceneryDrawNative (DrawArea's condition) holds, else the original.
// TextureAnimations::Advance and Minimap::Draw are called through their
// per-frame wraps (src/frame_pacing/per_frame_logic.cpp), RecordClear
// through its adapter's work.
bool CourseSceneryDrawNative();
void CourseSceneryDraw(PPCContext& ctx, uint8_t* base);

// Scenery::Update (sub_820F4558): r3 = the manager; the original's register
// saves made, then the plain C++ (its guest calls putting flush-to-zero back
// off); r3 as the original leaves it, lr put back.
void CourseSceneryUpdate(PPCContext& ctx, uint8_t* base);

// The calls the course scenery's plain C++ makes into code that takes the
// context, as its adapters make them (for native callers elsewhere: the
// race's scenery load, src/hooks/race_update.cpp). `guest` outlives them.
struct CourseSceneryGuest {
  PPCContext* ctx;
  uint8_t* base;
};
course_scenery::Calls CourseSceneryCalls(CourseSceneryGuest* guest);
// Whether Scenery::Create's and Setup's plain C++ may run (the models'
// native lifetime with the native heap, and memcpy native).
bool CourseScenerySetUpNative();

}  // namespace hooks
