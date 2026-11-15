// The render command handlers' adapters (src/native/replay_hooks.cpp): the
// guest calls their plain C++ (src/graphics/replay_commands.h) makes, shared
// with command 7's in src/interpolation/blend_state.cpp.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include "graphics/replay_commands.h"

namespace native {

// Guest calls through `ctx`: r1, lr and the registers given set (f1 when
// given), r1 put back and the flush mode turned off again after each.
graphics::replay_commands::GuestCall ReplayCall(lifted::Regs& ctx, uint8_t* base);

}  // namespace native
