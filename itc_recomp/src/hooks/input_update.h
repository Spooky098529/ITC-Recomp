#pragma once

#include "guest.h"

namespace input_hooks {

// The body of Input::Update (sub_820990F0; its override, with the
// render-only skip, is in src/frame_pacing/game_messages.cpp). r3 = the input
// manager. See input_update.cpp.
void InputUpdate_Run(PPCContext& ctx, uint8_t* base);

}  // namespace input_hooks
