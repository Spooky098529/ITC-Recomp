// The camera passes and viewports natively: adapters in front of
// src/cameras/cameras.cpp (docs/functions/cameras.md). Camera::SetViewportSize
// and Camera::SetFieldOfView are reached through the camera's vtable (slots 0
// and 1), so their adapters stay. Flush-to-zero is left off, as the
// translated code leaves it.

#include "guest.h"

#include "base/flags.h"
#include "cameras/cameras.h"

ITC_FLAG_BOOL(native_cameras, true, "ITC",
              "Run the camera passes and viewports (docs/functions/cameras.md: the library "
              "camera's Camera::SetFieldOfView sub_82593278, Camera::BuildProjection "
              "sub_825932E8 and Camera::SetViewportSize sub_82593500) natively instead of "
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
