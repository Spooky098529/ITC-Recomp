// The camera passes and viewports natively: adapters in front of
// src/cameras/cameras.cpp (docs/functions/cameras.md). Camera::SetViewportSize
// and Camera::SetFieldOfView are reached through the camera's vtable (slots 0
// and 1), so their adapters stay. Flush-to-zero is left off, as the
// translated code leaves it. CameraPasses::RenderTarget's, CameraPass::Disable's
// and CameraPasses::EnableForView's adapters are the diagnostics' wrappers in
// src/debug/frame_state_diff.cpp (one override per function); they read
// this flag too. The registers the originals leave that a caller could read
// (r3) are left the same.

#include "guest.h"

#include "base/flags.h"
#include "cameras/cameras.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_cameras, true, "ITC",
              "Run the camera passes and viewports (docs/functions/cameras.md: the library "
              "camera's Camera::SetFieldOfView sub_82593278, Camera::BuildProjection "
              "sub_825932E8 and Camera::SetViewportSize sub_82593500; the camera passes' "
              "CameraPasses::Mode sub_8209F718, SetMode sub_8209F738, RenderTarget "
              "sub_8209F7A8, EnableForView sub_820AA1A8 and SetNear sub_820AA368, "
              "CameraPass::Enable sub_820A0968, Disable sub_820A0958 and ReleaseSurfaces "
              "sub_820A0C40, and SetZoom sub_820AA378; the viewports' Viewports::Release "
              "sub_820AA430 (with --native_heap), Viewport::SetZoom sub_820AAE90 and SetRoll "
              "sub_820AA2D0) natively instead of "
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

// CameraPass::SetZoom: r3 = the pass, f1 = the zoom.
extern "C" REX_FUNC(sub_820AA378) {
  ITC_CAMERAS_FALLBACK(820AA378)
  ctx.fpscr.disableFlushMode();
  cameras::PassSetZoom(base, ctx.r3.u32, ctx.f1.f64);
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
