// The race's draw step (Race::DrawStep, sub_8215B4F8,
// docs/functions/functions.md) as plain C++: the camera passes, the car
// scene, the scenery, the post effects' and screen objects' recording, each
// through its override or the recompiled code (the dispatch table,
// hooks/lifted_call.h). Render-path code: checked by frame dumps. Its wrap
// (src/frame_pacing/game_messages.cpp) keeps the pass's race mark and the
// --log_input signs.

#pragma once

#include <cstdint>

namespace race_update {

// Race::DrawStep (sub_8215B4F8): r3 = the race (its +0 the scenery, +31152
// to +31160 the three screen objects). From its 112-byte frame below `sp`;
// returns the original's r3 (the last call's).
uint64_t RaceDrawStep(uint32_t sp, uint64_t r13, uint32_t race);

}  // namespace race_update
