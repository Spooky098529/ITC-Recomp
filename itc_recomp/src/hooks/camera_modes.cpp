// The camera modes natively: adapters in front of src/camera_modes/
// (docs/functions/camera-modes.md). Each leaves r3 as the original does
// where a caller could read it; flush-to-zero is left off, as the
// translated code leaves it. The director's handlers are reached through
// its state machine's functors, so their adapters stay.

#include "guest.h"

#include "base/flags.h"
#include "camera_modes/camera_modes.h"
#include "hooks/camera_modes.h"
#include "hooks/guest_call.h"

ITC_FLAG_BOOL(native_camera_modes, true, "ITC",
              "Run the camera modes (docs/functions/camera-modes.md: the camera director's "
              "handlers CameraDirector::ToView4 sub_82152400, ToView5 sub_821524B8, ToView3 "
              "sub_821524D8 and ToView3IfCourse sub_82153160; Viewports::ResetRoll sub_821524F0, "
              "Camera::ZoomFromFov sub_82152548, Quat::Rotate sub_821525F8, Matrix::FromQuat "
              "sub_82152698, Matrix::FromFloats sub_82090000; the lists' List40::Link "
              "sub_82153228, List40::Insert sub_821544E0, List16::Insert20 sub_82154458, "
              "Insert32 sub_82154378, Insert608 sub_821543E8 and Copy32 sub_82154300; "
              "CameraAnims::Clear sub_82150078 and Find sub_82150300; ColorFilter::SetStyle "
              "sub_82152108 and FadeCue::Step sub_82152290) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=camera_modes)");

namespace cm = camera_modes;

// An adapter: the step (hooks::CameraModesStep<address>, shared with the
// harness) behind the flag.
#define ITC_CAMERA_MODES(address)                       \
  extern "C" REX_FUNC(sub_##address) {                  \
    REX_FUNC_PROLOGUE();                                \
    if (!ITC_FLAG(native_camera_modes)) {               \
      __imp__sub_##address(ctx, base);                  \
      return;                                           \
    }                                                   \
    hooks::CameraModesStep##address(ctx, base);         \
  }

namespace {

// memcpy (sub_825A7410) through the translated code, for a copy
// c_runtime::Memcpy refuses: from a frame like the original's (`frame`
// bytes) with its return address. r3 = the destination, as memcpy leaves it.
void GuestMemcpy(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t return_address,
                 uint32_t destination, uint32_t source, uint32_t size) {
  guest_call::InFrame(ctx, base, frame, return_address, [&](uint32_t) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = source;
    ctx.r5.u64 = size;
    sub_825A7410(ctx, base);
  });
}

}  // namespace

namespace hooks {

// --- The director's handlers: r3 = the director ---

// ToView4: r3 = the last racer record when the split screen ended (its
// RacerRecords::At), else the director.
void CameraModesStep82152400(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::ToView4(base, ctx.r3.u32);
}
void CameraModesStep821524B8(PPCContext& ctx, uint8_t* base) {
  cm::ToView5(base, ctx.r3.u32);
}
void CameraModesStep821524D8(PPCContext& ctx, uint8_t* base) {
  cm::ToView3(base);
}
void CameraModesStep82153160(PPCContext& ctx, uint8_t* base) {
  cm::ToView3IfCourse(base, ctx.r3.u32);
}

// --- Small helpers ---

// Viewports::ResetRoll: r3 = viewport 1, as SetRoll leaves it.
void CameraModesStep821524F0(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::ResetRoll(base);
  ctx.r3.u64 = guest::LoadU32(base, guest::LoadU32(base, cm::kViewports) + 4);
}

// Camera::ZoomFromFov: f1 = degrees, f2 = the width; f1 = the zoom. Where
// tan's native form can't stand in, the original runs.
void CameraModesStep82152548(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto zoom = cm::ZoomFromFov(base, ctx.f1.f64, ctx.f2.f64);
  if (!zoom) {
    __imp__sub_82152548(ctx, base);
    return;
  }
  ctx.f1.f64 = *zoom;
}

// Quat::Rotate: r3 = out, r4 = the quaternion, r5 = the vector.
void CameraModesStep821525F8(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::QuatRotate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Matrix::FromQuat: r3 = the matrix, the quaternion in r4 and r5 (stored to
// the caller's r1 + 32 to + 47 as the original does; its vector load reads
// them back from there, so a stack pointer not 16-byte aligned runs the
// original).
void CameraModesStep82152698(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_82152698(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r1.u32 + 32, uint32_t(ctx.r4.u64 >> 32));
  guest::StoreU32(base, ctx.r1.u32 + 36, ctx.r4.u32);
  guest::StoreU32(base, ctx.r1.u32 + 40, uint32_t(ctx.r5.u64 >> 32));
  guest::StoreU32(base, ctx.r1.u32 + 44, ctx.r5.u32);
  const uint32_t q[4] = {uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32, uint32_t(ctx.r5.u64 >> 32),
                         ctx.r5.u32};
  ctx.fpscr.disableFlushMode();
  cm::MatrixFromQuat(base, ctx.r3.u32, q, (ctx.r1.u32 - 16) & ~0xFu);
}

// Matrix::FromFloats: r3 = the matrix, r4 to r10 the first seven pointers,
// the other nine on the caller's stack (r1 + 84, + 92, ... + 148).
void CameraModesStep82090000(PPCContext& ctx, uint8_t* base) {
  uint32_t from[16] = {ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                       ctx.r8.u32, ctx.r9.u32, ctx.r10.u32};
  for (uint32_t i = 7; i < 16; i++) {
    from[i] = guest::LoadU32(base, ctx.r1.u32 + 84 + 8 * (i - 7));
  }
  cm::MatrixFromFloats(base, ctx.r3.u32, from);
  ctx.r11.u64 = from[15];
}

// --- The lists: r3 = the list (left, or as memcpy leaves it) ---

void CameraModesStep82153228(PPCContext& ctx, uint8_t* base) {
  cm::List40Link(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}
// List40::Insert: r4 = &the word, r5 = the position.
void CameraModesStep821544E0(PPCContext& ctx, uint8_t* base) {
  cm::List40Insert(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32), ctx.r5.u32);
}
// List16::Insert20: r4 = the record, r5 = the position.
void CameraModesStep82154458(PPCContext& ctx, uint8_t* base) {
  cm::List16Insert20(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// List16::Insert32 and Insert608: r4 = the record, r5 = the position; r3 =
// the record's slot (memcpy's).
void CameraModesStep82154378(PPCContext& ctx, uint8_t* base) {
  uint32_t to = 0;
  if (!cm::List16Insert(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, 32, &to)) {
    GuestMemcpy(ctx, base, 112, 0x821543E0, to, ctx.r4.u32, 32);
  }
  ctx.r3.u64 = to;
}
void CameraModesStep821543E8(PPCContext& ctx, uint8_t* base) {
  uint32_t to = 0;
  if (!cm::List16Insert(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, 608, &to)) {
    GuestMemcpy(ctx, base, 112, 0x82154450, to, ctx.r4.u32, 608);
  }
  ctx.r3.u64 = to;
}
// List16::Copy32: r3 = out (returned), r4 = the list, r5 = the position.
void CameraModesStep82154300(PPCContext& ctx, uint8_t* base) {
  const uint32_t out = ctx.r3.u32;
  uint32_t from = 0;
  if (!cm::List16Copy32(base, out, ctx.r4.u32, ctx.r5.u32, &from)) {
    GuestMemcpy(ctx, base, 96, 0x8215435C, out, from, 32);
  }
  ctx.r3.u64 = out;
}

// --- The camera animations' table ---

void CameraModesStep82150078(PPCContext& ctx, uint8_t* base) {
  cm::AnimsClear(base);
}
// CameraAnims::Find: r3 = the asset id; r3 = its objects or 0.
void CameraModesStep82150300(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::AnimsFind(base, ctx.r3.u32);
}

// --- The color filter and the fade cues ---

// ColorFilter::SetStyle: r3 = &the style. The original runs where the
// filter's slots aren't its own functions.
void CameraModesStep82152108(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cm::ColorFilterSetStyle(base, ctx.r3.u32)) {
    __imp__sub_82152108(ctx, base);
  }
}

// FadeCue::Step: r3 = the cue (its caller, CutsceneShot::Step, doesn't read
// r3 after it).
void CameraModesStep82152290(PPCContext& ctx, uint8_t* base) {
  cm::FadeCueStep(base, ctx.r3.u32);
}

}  // namespace hooks

// --- The adapters ---

ITC_CAMERA_MODES(82152400)
ITC_CAMERA_MODES(821524B8)
ITC_CAMERA_MODES(821524D8)
ITC_CAMERA_MODES(82153160)
ITC_CAMERA_MODES(821524F0)
ITC_CAMERA_MODES(82152548)
ITC_CAMERA_MODES(821525F8)
ITC_CAMERA_MODES(82152698)
ITC_CAMERA_MODES(82090000)
ITC_CAMERA_MODES(82153228)
ITC_CAMERA_MODES(821544E0)
ITC_CAMERA_MODES(82154458)
ITC_CAMERA_MODES(82154378)
ITC_CAMERA_MODES(821543E8)
ITC_CAMERA_MODES(82154300)
ITC_CAMERA_MODES(82150078)
ITC_CAMERA_MODES(82150300)
ITC_CAMERA_MODES(82152108)
ITC_CAMERA_MODES(82152290)
