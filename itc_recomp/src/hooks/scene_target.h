// The scene target's register steps that make guest calls, shared by the
// adapters (src/hooks/scene_target.cpp) and the harness (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// TexturedRect::Record (sub_8217C368, a branch to RecordFlat sub_8217AA98):
// r3 = the rectangle, r5 = the bucket. The chunk natively, handed to the
// render queue at [0x826D83D0] (its slot 4, f1 = 0.0) from a 272-byte frame
// like the original's; r3 as that call leaves it. The original where the
// plain C++ declines.
void SceneTargetTexturedRect(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
