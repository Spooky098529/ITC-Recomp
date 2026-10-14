// The render command handlers (the table at 0x8265EDE0; see
// docs/gpu-functions.md), wrapped to pass each command on before it runs.

#include "debug/replay_stats.h"
#include "guest.h"
#include "renderer.h"
#include "state.h"

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

// The library's vertex shader patcher (see "Vertex declaration" in
// docs/gpu-functions.md): counted, so the native renderer can tell whether a
// draw's library call patched the shader's microcode.
namespace native {
uint32_t g_vertex_shader_patches = 0;
uint32_t g_last_patch[3] = {};
LoadedVertexShader g_loaded_vertex_shader;
}

// When the GPU may still be using a vertex shader's microcode, the library
// can't patch it in place: sub_822AEBF0 copies it into the command buffer
// (IM_LOAD_IMMEDIATE) and patches the copy, leaving the object's microcode
// (+48) and the id it's patched for (+24) as they were. The GPU then runs
// the copy, and keeps running it while the shader isn't loaded again. So
// the copy, once patched, is kept here for the native renderer.
extern "C" REX_FUNC(sub_822AE340) {
  REX_FUNC_PROLOGUE();
  uint32_t object = ctx.r3.u32, microcode = ctx.r4.u32, declaration = ctx.r5.u32;
  bool copy = microcode != guest::LoadU32(base, object + 48);
  native::g_vertex_shader_patches++;
  native::g_last_patch[0] = object;
  native::g_last_patch[1] = declaration;
  native::g_last_patch[2] = declaration ? guest::LoadU32(base, declaration + 48) : 0;
  __imp__sub_822AE340(ctx, base);
  if (copy) {
    native::LoadedVertexShader& loaded = native::g_loaded_vertex_shader;
    loaded.object = object;
    loaded.declaration_id = declaration ? guest::LoadU32(base, declaration + 48) : 0;
    uint32_t size = guest::LoadU32(base, object + 600);
    loaded.microcode.assign(base + microcode, base + microcode + size);
  } else {
    native::g_loaded_vertex_shader.object = 0;  // the object's own, patched in place
  }
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
