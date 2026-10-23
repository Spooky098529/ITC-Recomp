// The camera passes and viewports natively: adapters in front of
// src/cameras/cameras.cpp (docs/functions/cameras.md). Camera::SetViewportSize
// and Camera::SetFieldOfView are reached through the camera's vtable (slots 0
// and 1), so their adapters stay. Flush-to-zero is left off, as the
// translated code leaves it. CameraPasses::RenderTarget's, CameraPass::Disable's
// and CameraPasses::EnableForView's adapters are the diagnostics' wrappers in
// src/debug/frame_state_diff.cpp (one override per function); they read
// this flag too; CameraPass::Update's is its diagnostics' wrapper in
// src/frame_pacing/per_frame_logic.cpp, which calls hooks::CameraPassUpdate
// here. The registers the originals leave that a caller could read (r3) are
// left the same.

#include "guest.h"

#include "base/flags.h"
#include "cameras/cameras.h"
#include "hooks/cameras.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_cameras, true, "ITC",
              "Run the camera passes and viewports (docs/functions/cameras.md: the library "
              "camera's Camera::SetFieldOfView sub_82593278, Camera::BuildProjection "
              "sub_825932E8 and Camera::SetViewportSize sub_82593500; the camera passes' "
              "CameraPasses::Mode sub_8209F718, SetMode sub_8209F738, RenderTarget "
              "sub_8209F7A8, EnableForView sub_820AA1A8 and SetNear sub_820AA368, "
              "CameraPass::Enable sub_820A0968, Disable sub_820A0958 and ReleaseSurfaces "
              "sub_820A0C40, and SetZoom sub_820AA378 (no adapter: only CameraPass::FromViewport's "
              "plain C++ calls it, and the flag switches that); the viewports' Viewports::Release "
              "sub_820AA430 (with --native_heap), Viewport::SetZoom sub_820AAE90 and SetRoll "
              "sub_820AA2D0; CameraPass::Update sub_820A3978, its frame interpolation's notes kept, and "
              "CameraPass::RecordViewport sub_820A0978, without the command list's "
              "diagnostics, and CameraPass::FromViewport sub_820AA4A8) natively instead of "
              "through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=cameras)");

#define ITC_CAMERAS_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                \
  if (!ITC_FLAG(native_cameras)) {    \
    __imp__sub_##address(ctx, base);  \
    return;                           \
  }

// Camera::SetFieldOfView: r3 = the camera, f1 = degrees.
extern "C" REX_FUNC(sub_82593278) {
  ITC_CAMERAS_FALLBACK(82593278)
  ctx.fpscr.disableFlushMode();
  cameras::SetFieldOfView(base, ctx.r3.u32, ctx.f1.f64);
}

// Camera::BuildProjection: r3 = the camera, r4 to r7 = &field of view,
// &aspect, &far, &near. Its stack words are held in the plain C++, the same
// with the stack pointer 16-byte aligned (as the game's always is); the
// original otherwise.
extern "C" REX_FUNC(sub_825932E8) {
  ITC_CAMERAS_FALLBACK(825932E8)
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_825932E8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cameras::BuildProjection(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

// Camera::SetViewportSize: r3 = the camera, r4 = width, r5 = height (their
// low words).
extern "C" REX_FUNC(sub_82593500) {
  ITC_CAMERAS_FALLBACK(82593500)
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_82593500(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cameras::SetViewportSize(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// CameraPasses::Mode: r3 = which (1); r3 = the mode (lwz) or -1.
extern "C" REX_FUNC(sub_8209F718) {
  ITC_CAMERAS_FALLBACK(8209F718)
  const int32_t which = ctx.r3.s32;
  const uint32_t mode = cameras::Mode(base, which);
  if (which == 1) {
    ctx.r3.u64 = mode;
  } else {
    ctx.r3.s64 = -1;
  }
}

// CameraPasses::SetMode: r3 = which (1), r4 = the mode. Where pass 4's field
// of view is set, r3 = pass 4 and f1 = the degrees, as the original's tail
// call leaves them.
extern "C" REX_FUNC(sub_8209F738) {
  ITC_CAMERAS_FALLBACK(8209F738)
  const int32_t which = ctx.r3.s32;
  const uint32_t mode = ctx.r4.u32;
  ctx.fpscr.disableFlushMode();
  if (!cameras::SetMode(base, which, mode)) {
    __imp__sub_8209F738(ctx, base);
    return;
  }
  if (which == 1 && (mode == 0 || mode == 1)) {
    ctx.r3.u64 = cameras::Pass(4);
    ctx.f1.f64 = double(guest::LoadFloat(base, mode == 0 ? 0x82001E8C : 0x82001E88));
  }
}

// CameraPass::Enable: r3 = the pass.
extern "C" REX_FUNC(sub_820A0968) {
  ITC_CAMERAS_FALLBACK(820A0968)
  cameras::Enable(base, ctx.r3.u32);
}

// CameraPasses::SetNear: f1 = the distance.
extern "C" REX_FUNC(sub_820AA368) {
  ITC_CAMERAS_FALLBACK(820AA368)
  ctx.fpscr.disableFlushMode();
  cameras::SetNear(base, ctx.f1.f64);
}

// CameraPass::ReleaseSurfaces: r3 = the pass. Resource::Release is called as
// a guest call from a frame like the original's (112 bytes), with its return
// addresses; r3 as the original leaves it (the pass for types without
// surfaces, else Release's answer for +580, or 0 when +580 was empty).
extern "C" REX_FUNC(sub_820A0C40) {
  ITC_CAMERAS_FALLBACK(820A0C40)
  const uint32_t pass = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  bool released_580 = false;
  bool has_surfaces = false;
  guest_call::InFrame(ctx, base, 112, 0x820A0C8C, [&](uint32_t) {
    has_surfaces = cameras::ReleaseSurfaces(base, pass, [&](uint32_t surface, uint32_t field) {
      ctx.r3.u64 = surface;
      ctx.lr = field == 576 ? 0x820A0C8C : 0x820A0CA0;
      sub_822A67B0(ctx, base);
      released_580 = field == 580;
    });
  });
  ctx.lr = lr;
  if (!has_surfaces) {
    ctx.r3.u64 = pass;
  } else if (!released_580) {
    ctx.r3.u64 = 0;
  }
}

// Viewports::Release: no arguments; r3 = VirtualFree's answer for the block
// (1). The original unless --native_heap routes VirtualFree to the
// allocation layer, or where a block isn't the layer's.
extern "C" REX_FUNC(sub_820AA430) {
  ITC_CAMERAS_FALLBACK(820AA430)
  if (!hooks::NativeHeapActive() || !cameras::ReleaseViewports(base)) {
    __imp__sub_820AA430(ctx, base);
    return;
  }
  ctx.r3.u64 = 1;
}

// Viewport::SetZoom: r3 = the viewport, f1 = the zoom.
extern "C" REX_FUNC(sub_820AAE90) {
  ITC_CAMERAS_FALLBACK(820AAE90)
  ctx.fpscr.disableFlushMode();
  cameras::ViewportSetZoom(base, ctx.r3.u32, ctx.f1.f64);
}

// Viewport::SetRoll: r3 = the viewport, f1 = the angle.
extern "C" REX_FUNC(sub_820AA2D0) {
  ITC_CAMERAS_FALLBACK(820AA2D0)
  ctx.fpscr.disableFlushMode();
  cameras::ViewportSetRoll(base, ctx.r3.u32, ctx.f1.f64);
}

// --- CameraPass::Update and its viewport recording ---

// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the recording runs the original.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

namespace {

constexpr uint32_t kScene = 0x826BABA0;

// A call through slot `slot` of r3's vtable, returning to `return_address`.
void VtableCall(PPCContext& ctx, uint8_t* base, uint32_t slot, uint32_t return_address) {
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 4 * slot));
}

// CameraPass::RecordViewport's work: the command list from the scene
// ([kScene]'s slot 8, then slot 1), the chunk recorded natively, handed to
// the render queue (slot 8, then slot 2; the queue's slot 4: r4 = the
// chunk, r5 = the key, f1 = 0.0), the calls made from a frame like the
// original's (144 bytes) with its return addresses. False where the
// original must run (the diagnostics on, or a chunk its command list calls
// would print about: then the scene's getters have been called once more
// than the original would, never seen in play).
bool RecordViewportCalls(PPCContext& ctx, uint8_t* base, uint32_t pass, uint32_t key) {
  if (ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
      ITC_FLAG(log_recording_by_pass)) {
    return false;
  }
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> chunk;
  guest_call::InFrame(ctx, base, 144, 0x820A09A8, [&](uint32_t) {
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, 0x820A09A8);
    VtableCall(ctx, base, 1, 0x820A09B8);
    chunk = cameras::RecordViewport(base, ctx.r3.u32, pass);
    if (!chunk) {
      return;
    }
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, 0x820A0A88);
    VtableCall(ctx, base, 2, 0x820A0A98);
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = key;
    VtableCall(ctx, base, 4, 0x820A0AB4);
  });
  ctx.lr = lr;
  return chunk.has_value();
}

}  // namespace

// CameraPass::RecordViewport: r3 = the pass, r4 = the sort key.
extern "C" REX_FUNC(sub_820A0978) {
  ITC_CAMERAS_FALLBACK(820A0978)
  hooks::CameraPassRecordViewport(ctx, base);
}

namespace hooks {

void CameraPassRecordViewport(PPCContext& ctx, uint8_t* base) {
  const uint32_t pass = ctx.r3.u32;
  const uint64_t key = ctx.r4.u64;
  if (!RecordViewportCalls(ctx, base, pass, uint32_t(key))) {
    ctx.r3.u64 = pass;
    ctx.r4.u64 = key;
    __imp__sub_820A0978(ctx, base);
  }
}

bool CameraPassUpdate(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_cameras) || (ctx.r1.u32 & 0xF) != 0) {
    return false;
  }
  const uint32_t pass = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(ctx, base, 272, 0x820A39C0, [&](uint32_t frame) {
    ctx.fpscr.disableFlushMode();
    done = cameras::PassUpdate(base, pass, frame, [&](uint32_t key) {
      if (!RecordViewportCalls(ctx, base, pass, key)) {
        ctx.r3.u64 = pass;
        ctx.r4.u64 = key;
        ctx.lr = 0x820A39C0;
        __imp__sub_820A0978(ctx, base);
      }
    });
  });
  ctx.lr = lr;
  if (!done) {
    return false;
  }
  // As the original leaves them: flush-to-zero off, r3 the pass (types that
  // stop after the viewport size) or its view-projection (Matrix::Multiply's
  // out).
  ctx.fpscr.disableFlushMode();
  const uint32_t type = guest::LoadU32(base, pass + 584);
  ctx.r3.u64 = ((type >= 10 && type <= 18) || type == 20) ? pass : pass + 144;
  return true;
}

}  // namespace hooks

// CameraPass::FromViewport: r3 = the pass, r4 = the player, r5 = the entry.
// The fields natively, then CameraPass::Update called as the original's tail
// (its wrapper, src/frame_pacing/per_frame_logic.cpp, keeps its diagnostics
// and runs it natively) from a frame like the original's (112 bytes).
extern "C" REX_FUNC(sub_820AA4A8) {
  ITC_CAMERAS_FALLBACK(820AA4A8)
  const uint32_t pass = ctx.r3.u32;
  ctx.fpscr.disableFlushMode();
  cameras::FromViewport(base, pass, ctx.r4.u32, ctx.r5.u32);
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x820AA534, [&](uint32_t) {
    ctx.r3.u64 = pass;
    sub_820A3978(ctx, base);
  });
  ctx.lr = lr;
}
