// The render command handlers (the table at 0x8265EDE0; see
// docs/gpu-functions.md), wrapped to pass each command on before it runs.

#include "debug/replay_stats.h"
#include "guest.h"
#include "renderer.h"

// Command handlers, wrapped so the stats logger and the native renderer
// see every command. Command 7 (sub_8258FD48) is wrapped in blend_state.cpp.
// Ids 0, 36 and 37 (sub_82304258) aren't wrapped: that function is also
// used elsewhere, and they do nothing.
#define REPLAY_COUNTED_HANDLER(address, id)                                         \
  extern "C" REX_FUNC(sub_##address) {                                              \
    REX_FUNC_PROLOGUE();                                                            \
    uint32_t payload = ctx.r3.u32, arg1 = ctx.r4.u32, arg2 = ctx.r5.u32;           \
    debug::ReplayStats_OnCommand(base, id, payload, arg1, arg2);                    \
    native::Renderer_OnCommand(base, id, payload, arg1, arg2);                      \
    __imp__sub_##address(ctx, base);                                                \
    native::Renderer_AfterCommand(base, id, payload, arg1, arg2);                   \
  }

REPLAY_COUNTED_HANDLER(8258FBD0, 1)
REPLAY_COUNTED_HANDLER(8258FC30, 2)
REPLAY_COUNTED_HANDLER(8258FC90, 3)
REPLAY_COUNTED_HANDLER(8258FCE0, 4)
REPLAY_COUNTED_HANDLER(8258E9F8, 5)
REPLAY_COUNTED_HANDLER(8258EA20, 6)
REPLAY_COUNTED_HANDLER(8258FF50, 8)
REPLAY_COUNTED_HANDLER(8258FFA0, 9)
REPLAY_COUNTED_HANDLER(8258FFF0, 10)
REPLAY_COUNTED_HANDLER(82590040, 11)
REPLAY_COUNTED_HANDLER(825900B8, 12)
REPLAY_COUNTED_HANDLER(8258EA58, 13)  // also 14: one handler
REPLAY_COUNTED_HANDLER(82590108, 15)
REPLAY_COUNTED_HANDLER(825901B0, 16)
REPLAY_COUNTED_HANDLER(82590268, 17)
REPLAY_COUNTED_HANDLER(825902E0, 18)
REPLAY_COUNTED_HANDLER(82590360, 19)
REPLAY_COUNTED_HANDLER(825903D8, 20)
REPLAY_COUNTED_HANDLER(8258EAD0, 21)
REPLAY_COUNTED_HANDLER(82591798, 22)
REPLAY_COUNTED_HANDLER(82590428, 23)
REPLAY_COUNTED_HANDLER(82590478, 24)
REPLAY_COUNTED_HANDLER(825904C8, 25)
REPLAY_COUNTED_HANDLER(82590620, 26)
REPLAY_COUNTED_HANDLER(82590698, 27)
REPLAY_COUNTED_HANDLER(8258EAE0, 28)
REPLAY_COUNTED_HANDLER(82590768, 29)
REPLAY_COUNTED_HANDLER(82590860, 30)
REPLAY_COUNTED_HANDLER(825908E0, 31)
REPLAY_COUNTED_HANDLER(82590968, 32)
REPLAY_COUNTED_HANDLER(825909E8, 33)
REPLAY_COUNTED_HANDLER(82590A68, 34)
REPLAY_COUNTED_HANDLER(82590AD0, 35)
