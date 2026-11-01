// The game's Direct3D calls the renderer still sees outside the render
// commands (which our own handlers run: src/render/commands.cpp, the
// conventional renderer, docs/conventional-renderer.md): the frame's end
// and the library's Resolve.

#include "guest.h"
#include "renderer.h"

#include "base/log.h"

#include <set>
#include <utility>

// The frame's end (see "The end of a frame" in docs/gpu-functions.md):
// sub_8258D998(video output) resolves render target 0 into one of two front
// buffer textures (video output + 4 * (68 + *(+268))) with the library's
// Resolve, outside the command list; the native renderer shows that target.
extern "C" REX_FUNC(sub_8258D998) {
  REX_FUNC_PROLOGUE();
  uint32_t video_output = ctx.r3.u32;
  uint32_t front_buffer =
      guest::LoadU32(base, video_output + 4 * (68 + guest::LoadU32(base, video_output + 268)));
  native::Renderer_FrontBufferResolve(base, guest::LoadU32(base, video_output + 140), front_buffer);
  __imp__sub_8258D998(ctx, base);
}

// The library's Resolve (r3 device, r4 flags, ...): only the frame's end
// calls it now; each caller logged once. The front buffer's ("called from
// 8258D9E8") marks the boot's end for scripted_run.sh.
extern "C" REX_FUNC(sub_822AFA58) {
  REX_FUNC_PROLOGUE();
  static std::set<uint32_t> logged;
  if (logged.insert(uint32_t(ctx.lr)).second) {
    ITC_LOG_INFO("native renderer: Resolve ({:08X}) called from {:08X} in command 0 (outside the "
                 "command list)",
                 0x822AFA58, uint32_t(ctx.lr));
  }
  __imp__sub_822AFA58(ctx, base);
}
