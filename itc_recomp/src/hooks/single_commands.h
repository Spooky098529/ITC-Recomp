// The single commands' adapters' register steps (src/hooks/single_commands.cpp),
// shared with the render queue's native dispatch (src/hooks/render_queue.cpp)
// and the harness (src/harnesses/single_commands.cpp).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include "graphics/render_queue.h"
#include "graphics/single_commands.h"

namespace hooks {

// Guest calls for the single commands' plain C++, made through `ctx` (r1,
// lr and the registers given set, r1 put back; the flush mode turned off
// again after each).
graphics::single_commands::GuestCall SingleCommandsCall(lifted::Regs& ctx);

// The single commands RunSingles runs natively: their plain C++ for the
// handlers that are ours while --native_single_commands is on, else nullopt
// (the guest call runs).
graphics::render_queue::SingleCommand SingleCommands(lifted::Regs& ctx);

// The adapters' native paths, without the flag: r3 as the original's.
void NativeSingleInlineVertices(lifted::Regs& ctx);
void NativeSingleSetUpDrawer(lifted::Regs& ctx);
void NativeSingleSetShaderConstants(lifted::Regs& ctx);
// Command 15 marked as command 15 (native::g_current_command) around it, as
// its wrap in src/native/replay_hooks.cpp does.
void NativeSingleDrawerDraw(lifted::Regs& ctx);
void NativeSingleCallback(lifted::Regs& ctx);

}  // namespace hooks
