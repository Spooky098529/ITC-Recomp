// The render command handlers (the table at 0x8265EDE0; see
// docs/gpu-functions.md), wrapped to pass each command on before it runs.

#include "native/replay_hooks.h"

#include "debug/replay_stats.h"
#include "draw_packets.h"
#include "guest.h"
#include "renderer.h"
#include "state.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/inline_draws.h"
#include "graphics/shader_patch.h"
#include "graphics/video_output.h"
#include "hooks/guest_call.h"
#include "lifted/sdk.h"
#include "hooks/single_commands.h"

#include <set>
#include <utility>

ITC_FLAG_DECLARE(bool, native_single_commands);
ITC_FLAG_BOOL(native_replay_commands, true, "ITC",
              "Run the render command replay's command handlers (the table 0x8265EDE0, "
              "docs/functions/functions.md's Command::* rows: sub_8258FBD0 to sub_82591798 and "
              "command 7's sub_8258FD48) and the frame's end resolve "
              "(VideoOutput::ResolveFrontBuffer sub_8258D998), and the vertex shader patcher "
              "(sub_822AE340) natively inside their wraps "
              "instead of through the "
              "translated code (the same writes; the device's native setters called as plain "
              "C++, everything else a guest call; checked by frame dumps, as the render path's "
              "code is)");

ITC_FLAG_BOOL(native_inline_draws, true, "ITC",
              "Run the graphics library's BeginVertices (sub_825BD8F0), EndVertices "
              "(sub_825BDDA0), BeginIndexedVertices (sub_825BDDC0) and DrawVertices "
              "(sub_825BE300) natively inside their wraps (src/graphics/inline_draws.h): the "
              "dirty state's flush, the segment's end and the space reserved kept, their GPU "
              "packets and draw chain records dropped; checked by frame dumps");

namespace rc = graphics::replay_commands;
namespace inline_draws = graphics::inline_draws;

rc::GuestCall native::ReplayCall(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t function, uint32_t sp, uint32_t return_address,
                      std::initializer_list<uint64_t> args, std::optional<double> f1) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    int reg = 3;
    for (uint64_t value : args) {
      guest_call::Gpr(ctx, reg++).u64 = value;
    }
    if (f1) {
      ctx.f1.f64 = *f1;
    }
    ctx.lr = return_address;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
}

// Command handlers, wrapped so the stats logger and the native renderer
// see every command. Command 7 (sub_8258FD48) is wrapped in blend_state.cpp.
// Ids 0, 36 and 37 (sub_82304258) aren't wrapped: that function is also
// used elsewhere, and they do nothing. Inside the wrap, the handler's work
// is its plain C++ (src/graphics/replay_commands.h) with
// --native_replay_commands, else the original. `body` is the plain C++
// call, with payload (r3), arg1 (r4), arg2 (r5), sp (r1) and call (the
// guest calls) in scope; it leaves r3.
#define REPLAY_COUNTED_HANDLER(address, id, body)                                 \
  extern "C" REX_FUNC(sub_##address) {                                              \
    REX_FUNC_PROLOGUE();                                                            \
    uint32_t payload = ctx.r3.u32, arg1 = ctx.r4.u32, arg2 = ctx.r5.u32;           \
    debug::ReplayStats_OnCommand(base, id, payload, arg1, arg2);                    \
    native::Renderer_OnCommand(base, id, payload, arg1, arg2);                      \
    uint32_t outer_command = native::g_current_command;                             \
    native::g_current_command = id;                                                 \
    native::g_command_serial++;                                                     \
    if (ITC_FLAG(native_replay_commands)) {                                         \
      [[maybe_unused]] const uint32_t sp = ctx.r1.u32;                              \
      [[maybe_unused]] const rc::GuestCall call = native::ReplayCall(ctx, base);    \
      ctx.fpscr.disableFlushMode();                                                 \
      ctx.r3.u64 = (body);                                                          \
    } else {                                                                        \
      __imp__sub_##address(ctx, base);                                              \
    }                                                                               \
    native::g_current_command = outer_command;                                      \
    native::Renderer_AfterCommand(base, id, payload, arg1, arg2);                   \
  }

// The library's vertex shader patcher (see "Vertex declaration" in
// docs/gpu-functions.md): counted, so the native renderer can tell whether a
// draw's library call patched the shader's microcode.
namespace native {
uint32_t g_vertex_shader_patches = 0;
uint32_t g_last_patch[3] = {};
LoadedVertexShader g_loaded_vertex_shader;
uint32_t g_current_command = 0;
uint32_t g_command_serial = 0;
}

// The vertex shader patcher's plain C++ (src/graphics/shader_patch.h): r3
// the shader, r4 where the patched instructions go, r5 the declaration, r6
// the stream bytes; its memcpy a guest call (memcpy's override records
// copies for the renderer's mirrors), from the original's return address.
namespace {
struct PatchCopyContext {
  PPCContext* ctx;
  uint8_t* base;
};
void PatchCopy(void* context, uint32_t destination, uint32_t source, uint32_t size) {
  PatchCopyContext& c = *static_cast<PatchCopyContext*>(context);
  PPCContext& ctx = *c.ctx;
  uint8_t* base = c.base;
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = stack - 592;
  ctx.r3.u64 = destination;
  ctx.r4.u64 = source;
  ctx.r5.u64 = size;
  ctx.lr = 0x822AE900;
  sub_825A7410(ctx, base);
  ctx.r1.u64 = stack;
}
void NativePatchVertexShader(PPCContext& ctx, uint8_t* base) {
  PatchCopyContext context{&ctx, base};
  const uint32_t shader = ctx.r3.u32, microcode = ctx.r4.u32, declaration = ctx.r5.u32,
                 streams = ctx.r6.u32, sp = ctx.r1.u32;
  ctx.r3.u64 = graphics::shader_patch::PatchVertexShader(base, shader, microcode, declaration,
                                                         streams, sp, PatchCopy, &context);
}
}  // namespace

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
  if (ITC_FLAG(native_replay_commands)) {
    ctx.fpscr.disableFlushMode();
    NativePatchVertexShader(ctx, base);
  } else {
    __imp__sub_822AE340(ctx, base);
  }
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

// Inline vertex draws (DrawPrimitiveUP, command 19, and the helper object's
// draws, command 15; see "Inline vertex draws" in docs/gpu-functions.md):
// BeginVertices (sub_825BD8F0: device, primitive type, vertex count, stride
// in bytes) returns where in the command buffer the caller copies the
// vertices; End (sub_825BDDA0) follows the copy, so the native renderer
// draws them then.
namespace {
struct PendingInline {
  bool pending = false;
  uint32_t type, count, stride, data, caller;
} g_pending_inline;
// Library calls that draw or resolve made outside a replayed command (e.g.
// the front buffer resolve), logged once per function and caller.
std::set<std::pair<uint32_t, uint32_t>> g_logged_outside;
void LogOutside(const char* what, uint32_t function, uint32_t caller, uint32_t command) {
  if (!g_logged_outside.insert({function, caller}).second) {
    return;
  }
  ITC_LOG_INFO("native renderer: {} ({:08X}) called from {:08X} in command {}{}", what, function,
               caller, command, command ? "" : " (outside the command list)");
}
}  // namespace

extern "C" REX_FUNC(sub_825BD8F0) {
  REX_FUNC_PROLOGUE();
  uint32_t type = ctx.r4.u32, count = ctx.r5.u32, stride = ctx.r6.u32;
  uint32_t caller = uint32_t(ctx.lr);
  if (ITC_FLAG(native_inline_draws)) {
    lifted::Enter(ctx, base, [](uint8_t* base, const lifted::Args& a) -> lifted::Result {
      return {inline_draws::BeginVertices(base, uint32_t(a.r1), a.r13, uint32_t(a.r[0]),
                                          uint32_t(a.r[2]), uint32_t(a.r[3])),
              a.f[0]};
    });
  } else {
    __imp__sub_825BD8F0(ctx, base);
  }
  g_pending_inline = {ctx.r3.u32 != 0, type, count, stride, ctx.r3.u32, caller};
}

extern "C" REX_FUNC(sub_825BDDA0) {
  REX_FUNC_PROLOGUE();
  PendingInline draw = g_pending_inline;
  g_pending_inline.pending = false;
  if (draw.pending) {
    uint32_t command = native::g_current_command;
    if (command == 15 || command == 19) {
      native::Renderer_InlineDraw(base, draw.type, draw.count, draw.stride, draw.data);
    } else {
      LogOutside("an inline vertex draw, not drawn, BeginVertices", 0x825BD8F0, draw.caller,
                 command);
    }
  }
  if (ITC_FLAG(native_inline_draws)) {
    inline_draws::EndVertices(base, ctx.r3.u32);  // r3 left as the device, as the original
    return;
  }
  __imp__sub_825BDDA0(ctx, base);
}

REPLAY_COUNTED_HANDLER(8258FBD0, 1, rc::SetRenderStates(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258FC30, 2, rc::SetSamplerStates(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258FC90, 3, rc::SetBlendControls(base, payload, arg1))
REPLAY_COUNTED_HANDLER(8258FCE0, 4, rc::SetTextures(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258E9F8, 5, rc::InlineVertices(base, payload, arg1))
REPLAY_COUNTED_HANDLER(8258EA20, 6, rc::InlineIndices(base, payload, arg1, arg2))
REPLAY_COUNTED_HANDLER(8258FF50, 8, rc::SetVertexShaders(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258FFA0, 9, rc::SetPixelShaders(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258FFF0, 10, rc::SetVertexDeclarations(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590040, 11, rc::SetStreamSources(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(825900B8, 12, rc::SetIndices(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258EA58, 13, rc::SetUpDrawer(base, payload, arg1, sp, call))  // also 14: one handler
REPLAY_COUNTED_HANDLER(82590108, 15, rc::DrawerDraw(base, arg1, sp, call))
REPLAY_COUNTED_HANDLER(825901B0, 16, rc::DrawerDrawIndexed(base, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590268, 17, rc::DrawVertices(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(825902E0, 18, rc::DrawIndexedVertices(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590360, 19, rc::DrawVerticesUP(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(825903D8, 20, rc::ReleaseResources(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258EAD0, 21, rc::Skip(arg1))
REPLAY_COUNTED_HANDLER(82591798, 22, rc::DeferRelease(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590428, 23, rc::SetRenderTargets(base, payload, arg1))
REPLAY_COUNTED_HANDLER(82590478, 24, rc::SetDepthStencilSurfaces(base, payload, arg1))
REPLAY_COUNTED_HANDLER(825904C8, 25, rc::Resolves(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590620, 26, rc::Clears(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590698, 27, rc::DrawIndexedVerticesUP(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(8258EAE0, 28, rc::Callbacks(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590768, 29, rc::RestoreSurfaces(base, payload, arg1))
REPLAY_COUNTED_HANDLER(82590860, 30, rc::SetViewports(base, payload, arg1, sp))
REPLAY_COUNTED_HANDLER(825908E0, 31, rc::SetScissorRects(base, payload, arg1, sp))
REPLAY_COUNTED_HANDLER(82590968, 32, rc::BeginTiling(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(825909E8, 33, rc::EndTiling(base, payload, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590A68, 34, rc::SaveSurfaces(base, arg1, sp, call))
REPLAY_COUNTED_HANDLER(82590AD0, 35, rc::RestoreSavedSurfaces(base, arg1))


// The chain walker (sub_8258EE98) also has items of single commands, a
// slot each ([id, arguments]), dispatched by sub_8258E630 through a second
// table at 0x8265ED48 with the same ids. In a race only 5, 7, 13 and 15
// come this way (a few a frame: the minimap's player triangle). Its 15,
// the helper object's draw, is marked as command 15 so the native renderer
// draws its inline vertices; the others reach the device's shadow, which
// the renderer reads anyway. Its body is native with
// --native_single_commands (src/graphics/single_commands.h; RunSingles'
// plain C++ calls it directly, marked the same way: hooks::SingleCommands).
extern "C" REX_FUNC(sub_8258F5C0) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_single_commands)) {
    hooks::NativeSingleDrawerDraw(ctx, base);
    return;
  }
  uint32_t outer_command = native::g_current_command;
  native::g_current_command = 15;
  __imp__sub_8258F5C0(ctx, base);
  native::g_current_command = outer_command;
}

// The frame's end (see "The end of a frame" in docs/gpu-functions.md):
// sub_8258D998(video output) resolves render target 0 into one of two front
// buffer textures (video output + 4 * (68 + *(+268))) with the library's
// Resolve, outside command 25; the native renderer shows that target.
extern "C" REX_FUNC(sub_8258D998) {
  REX_FUNC_PROLOGUE();
  uint32_t video_output = ctx.r3.u32;
  uint32_t front_buffer =
      guest::LoadU32(base, video_output + 4 * (68 + guest::LoadU32(base, video_output + 268)));
  native::Renderer_FrontBufferResolve(base, guest::LoadU32(base, video_output + 140), front_buffer);
  if (!ITC_FLAG(native_replay_commands)) {
    __imp__sub_8258D998(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = graphics::video_output::ResolveFrontBuffer(base, video_output, ctx.r1.u32,
                                                          native::ReplayCall(ctx, base));
}

// The library's other draws and Resolve: logged when called outside the
// commands that are known to make them (18, 17, 27, 25). DrawVertices and
// BeginIndexedVertices (never seen in play; our renderer draws neither) run
// natively with --native_inline_draws.
extern "C" REX_FUNC(sub_825BE300) {
  REX_FUNC_PROLOGUE();
  if (native::g_current_command != 17) {
    LogOutside("DrawVertices", 0x825BE300, uint32_t(ctx.lr), native::g_current_command);
  }
  if (!ITC_FLAG(native_inline_draws)) {
    __imp__sub_825BE300(ctx, base);
    return;
  }
  // r3 left as the device (its callers don't read it).
  lifted::Enter(ctx, base, [](uint8_t* base, const lifted::Args& a) -> lifted::Result {
    inline_draws::DrawVertices(base, uint32_t(a.r1), a.r13, uint32_t(a.r[0]), uint32_t(a.r[1]),
                               uint32_t(a.r[2]), uint32_t(a.r[3]));
    return {a.r[0], a.f[0]};
  });
}
extern "C" REX_FUNC(sub_825BDDC0) {
  REX_FUNC_PROLOGUE();
  if (native::g_current_command != 27) {
    LogOutside("BeginIndexedVertices", 0x825BDDC0, uint32_t(ctx.lr), native::g_current_command);
  }
  if (!ITC_FLAG(native_inline_draws)) {
    __imp__sub_825BDDC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, [](uint8_t* base, const lifted::Args& a) -> lifted::Result {
    return {inline_draws::BeginIndexedVertices(base, uint32_t(a.r1), a.r13, uint32_t(a.r[0]),
                                               uint32_t(a.r[3]), uint32_t(a.r[4]),
                                               uint32_t(a.r[5]), uint32_t(a.r[6]),
                                               uint32_t(a.r[7])),
            a.f[0]};
  });
}

// DrawIndexedVertices: in exclusive mode without its packets (see
// draw_packets.cpp).
extern "C" REX_FUNC(sub_825BE708) {
  REX_FUNC_PROLOGUE();
  if (native::g_current_command != 18) {
    LogOutside("DrawIndexedVertices", 0x825BE708, uint32_t(ctx.lr), native::g_current_command);
  }
  native::DrawPackets_IndexedDraw(ctx, base);
}

// The library's Resolve (r3 device, r4 flags, r5 source rect, r6
// destination texture, r7 destination point, r8 level, r9 slice, r10 clear
// color as 4 floats, f1 clear Z). Command 25's are handled from its items;
// command 33 (EndTiling, see "Predicated tiling" in docs/gpu-functions.md)
// makes one per tile (the tile count at device +12488), passed on here.
extern "C" REX_FUNC(sub_822AFA58) {
  REX_FUNC_PROLOGUE();
  static uint32_t serial = 0, tile = 0;
  uint32_t command = native::g_current_command;
  if (command == 33) {
    if (serial != native::g_command_serial) {
      serial = native::g_command_serial;
      tile = 0;
    }
    uint32_t device = ctx.r3.u32;
    native::Renderer_TiledResolve(base, device, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                  ctx.r10.u32, float(ctx.f1.f64), tile++,
                                  guest::LoadU32(base, device + 12488));
  } else if (command != 25) {
    LogOutside("Resolve", 0x822AFA58, uint32_t(ctx.lr), command);
  }
  __imp__sub_822AFA58(ctx, base);
}
