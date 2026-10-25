// The course scenery's register steps shared by its adapters
// (src/hooks/course_scenery.cpp, behind --native_course_scenery) and the
// harness (without the flag); docs/functions/course-scenery.md.

#pragma once

#include "guest.h"

namespace hooks {

// Scenery::DrawSetLists (sub_820F52A8): r3 = the manager, r4 = the pass
// description; r3 as the original leaves it. Its original runs with the
// command list's diagnostics on (as the recorders'). r24 is the
// view-projection's address while it runs (the draws' adapters read it
// where their plain C++ declines), then put back.
void CourseSceneryDrawSetLists(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
