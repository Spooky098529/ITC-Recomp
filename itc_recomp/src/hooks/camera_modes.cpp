// The camera modes natively: adapters in front of src/camera_modes/
// (docs/functions/camera-modes.md). Each leaves r3 as the original does
// where a caller could read it; flush-to-zero is left off, as the
// translated code leaves it. The director's handlers are reached through
// its state machine's functors, so their adapters stay.

#include "guest.h"

#include "base/flags.h"
#include "camera_modes/camera_modes.h"
#include "hooks/camera_modes.h"
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "hooks/cameras.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "game_modes/game_modes.h"
#include "cameras/cameras.h"

ITC_FLAG_BOOL(native_camera_modes, true, "ITC",
              "Run the camera modes (docs/functions/camera-modes.md: the camera director's "
              "handlers CameraDirector::ToView4 sub_82152400, ToView5 sub_821524B8, ToView3 "
              "sub_821524D8 and ToView3IfCourse sub_82153160; Viewports::ResetRoll sub_821524F0, "
              "Camera::ZoomFromFov sub_82152548, Quat::Rotate sub_821525F8, Matrix::FromQuat "
              "sub_82152698 (no adapter: only CameraTarget::Track's plain C++ calls it), Matrix::FromFloats sub_82090000; the lists' List40::Link "
              "sub_82153228 (no adapter: only List40::Insert's plain C++ calls it), "
              "List40::Insert sub_821544E0, List16::Insert20 sub_82154458 (no adapter: only IntroCamera::IntroCamera's plain C++ calls it), "
              "Insert32 sub_82154378, Insert608 sub_821543E8 and Copy32 sub_82154300; "
              "CameraAnims::Clear sub_82150078 and Find sub_82150300; ColorFilter::SetStyle "
              "sub_82152108 and FadeCue::Step sub_82152290; with --native_heap the camera "
              "animations' CameraAnims::Load sub_82150B10 (with the archive's queue native), "
              "OnLoaded sub_82150878 with CameraAnim::ParseObject sub_82150100 (no adapter), "
              "FreeAll sub_821507D8 with CameraAnim::Free sub_82150098 and CameraAnimBlock::Free "
              "sub_8214FFF0 (no adapters); CameraAnims::Evaluate sub_82150960 with "
              "CameraAnim::EvalCamera sub_82150350 and EvalTarget sub_821504E0 (no adapters), "
              "Duration sub_82150A98, ObjectMatrix sub_82150A10 with CameraAnim::Matrix "
              "sub_82150590 (no adapter); with --native_heap and the archive's queue native "
              "CameraPath::Request sub_82152E08 with Load sub_82152D80 (no adapter); "
              "CameraPath::OnLoaded sub_82152078 with FixKeys sub_820AC000, FixKey sub_820ABF80, "
              "FixGroup sub_820ABED8 and FixTriple sub_820ABE68 (no adapters); "
              "RivalLogo::RivalLogo sub_82211848 with InitXpr sub_82211708 (no adapter) and "
              "RivalLogo::Play sub_82211AB0 (without --log_pause_movie); CameraTarget::Track "
              "sub_82153290; Camera::Shake sub_82152CD8 with ShakeRandom sub_82152BD8 and "
              "Viewport::Offset sub_821529B0 (no adapters), Viewport::PullBack sub_82152788; "
              "IntroCamera::StepShot sub_82152E78, IntroCamera::IntroCamera sub_82158F70 with "
              "MakeShot sub_82154108 (no adapter; with native atexit); CameraDirector::Start "
              "sub_82152338 and Release sub_82153098; CutsceneShot::Init sub_82156B68 and Step "
              "sub_82155800) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=camera_modes)");

ITC_FLAG_DECLARE(bool, log_pause_movie);

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

// XMemCpy (sub_82295890) through the translated code, for a copy
// c_runtime::XMemCpy refuses, as GuestMemcpy.
void GuestXMemCpy(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t return_address,
                  uint32_t destination, uint32_t source, uint32_t size) {
  guest_call::InFrame(ctx, base, frame, return_address, [&](uint32_t) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = source;
    ctx.r5.u64 = size;
    sub_82295890(ctx, base);
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

// Matrix::FromQuat (no adapter: only CameraTarget::Track's plain C++ calls it; the step kept
// for the harness).
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

// List40::Link (no adapter: only List40::Insert's plain C++ calls it): r3 =
// the list, r4 = the position; the step kept for the harness.
void CameraModesStep82153228(PPCContext& ctx, uint8_t* base) {
  cm::List40Link(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}
// List40::Insert: r4 = &the word, r5 = the position.
void CameraModesStep821544E0(PPCContext& ctx, uint8_t* base) {
  cm::List40Insert(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32), ctx.r5.u32);
}
// List16::Insert20 (no adapter: only IntroCamera::IntroCamera's plain C++ calls it; the
// step kept for the harness).
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

// --- The camera animations' files ---

// CameraAnims::Load: r3 = the asset id; r3 = the serial (as Archive::Load
// leaves it). The original without --native_heap or the archive's queue.
void CameraModesStep82150B10(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    __imp__sub_82150B10(ctx, base);
    return;
  }
  hooks::SetArchiveSerial(ctx, cm::AnimsLoad(base, ctx.r3.u32, hooks::NativeArchiveLoad(base)));
}

// CameraAnims::OnLoaded (the archive's callback): r4 = the data, r6 = the
// record; r3 = VirtualFree's 1. Its frame (672 bytes) is the original's; the
// copies the native layer refuses go through the guest from it (memcpy's
// return addresses 0x821508F4 and 0x8215092C; XMemCpy from ParseObject's
// frame below it, 0x821502B8).
void CameraModesStep82150878(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_82150878(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 672;
  const bool done = cm::AnimsOnLoaded(
      base, ctx.r4.u32, ctx.r6.u32, frame,
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestMemcpy(ctx, base, 672, to == frame + 84 ? 0x821508F4 : 0x8215092C, to, from, size);
      },
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestXMemCpy(ctx, base, 672 + 144, 0x821502B8, to, from, size);
      });
  if (!done) {
    __imp__sub_82150878(ctx, base);
    return;
  }
  ctx.r3.u64 = 1;
}

// CameraAnims::FreeAll: no arguments; r3 as the original leaves it (1, its
// memcpy's frame + 80, or unchanged).
void CameraModesStep821507D8(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !cm::AnimsFreeable(base)) {
    __imp__sub_821507D8(ctx, base);
    return;
  }
  if (const auto r3 = cm::AnimsFreeAll(base, ctx.r1.u32 - 384 + 80)) {
    ctx.r3.u64 = *r3;
  }
}

// CameraAnims::Evaluate: r3 = out, r4 = the id, r5 = the frame; r3 = 1 or
// 0, f1 the last EvalCamera's. EvalCamera's original runs from a frame like
// Evaluate's (144 bytes, returning to 0x821509C4) where its native form
// can't.
void CameraModesStep82150960(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t out = ctx.r3.u32, time = ctx.r5.u32;
  std::optional<double> f1;
  const uint32_t r3 = cm::AnimsEvaluate(
      base, out, ctx.r4.u32, time,
      [&](uint32_t object) {
        guest_call::InFrame(ctx, base, 144, 0x821509C4, [&](uint32_t) {
          ctx.r3.u64 = object;
          ctx.r4.u64 = object;
          ctx.r5.u64 = out;
          ctx.r6.u64 = time;
          __imp__sub_82150350(ctx, base);
        });
        ctx.fpscr.disableFlushMode();
      },
      &f1);
  if (f1) {
    ctx.f1.f64 = *f1;
  }
  ctx.r3.u64 = r3;
}

// CameraAnims::Duration: r3 = the id; f1 = the duration, r3 the objects'
// slot the original stopped at.
void CameraModesStep82150A98(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  ctx.f1.f64 = cm::AnimsDuration(base, ctx.r3.u32, &r3);
  ctx.r3.u64 = r3;
}

// CameraAnims::ObjectMatrix: r3 = out, r4 = the id, r5 = the frame; r3 = 1
// or 0. CameraAnim::Matrix's frame is the original's (r1 - 96 - 320), so a
// stack pointer not 16-byte aligned runs the original.
void CameraModesStep82150A10(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_82150A10(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::AnimsObjectMatrix(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 96 - 320,
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestMemcpy(ctx, base, 96 + 320, 0x82150600, to, from, size);
      });
}

// --- The camera paths ---

// CameraPath::Request: r4 = the path buffer, r5 = the index; r3 = the
// serial when a load was queued, else unchanged. The original without
// --native_heap, the archive's queue, or a block the layer didn't give.
void CameraModesStep82152E08(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue() ||
      !game_modes::Buffer28Freeable(base, ctx.r4.u32)) {
    __imp__sub_82152E08(ctx, base);
    return;
  }
  if (const auto serial =
          cm::PathRequest(base, ctx.r4.u32, ctx.r5.u32, hooks::NativeArchiveLoad(base))) {
    hooks::SetArchiveSerial(ctx, *serial);
  }
}

// CameraPath::OnLoaded (the archive's callback): r4 = the data, r6 = the
// buffer.
void CameraModesStep82152078(PPCContext& ctx, uint8_t* base) {
  cm::PathOnLoaded(base, ctx.r4.u32, ctx.r6.u32);
}

// --- The rival logo ---

// RivalLogo::RivalLogo: r3 = the entry (its caller sets r3 again after).
void CameraModesStep82211848(PPCContext& ctx, uint8_t* base) {
  cm::RivalLogoConstruct(base, ctx.r3.u32);
}

// RivalLogo::Play: r3 = the entry; r3 as the original leaves it. With
// --log_pause_movie the original runs (PlayNamed's override logs it).
void CameraModesStep82211AB0(PPCContext& ctx, uint8_t* base) {
  if (ITC_FLAG(log_pause_movie)) {
    __imp__sub_82211AB0(ctx, base);
    return;
  }
  ctx.r3.u64 = cm::RivalLogoPlay(base, ctx.r3.u32, ctx.r1.u32 - 144 + 80);
}

// --- The views ---

// CameraTarget::Track: r3 = the tracker, r4 = the vehicle; r3 = tracker +
// 224 (TrackPosition::Copy's) when it tracked. Its frame (256 bytes) and
// Matrix::FromQuat's scratch below it are the original's, so a stack
// pointer not 16-byte aligned runs the original.
void CameraModesStep82153290(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_82153290(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t tracker = ctx.r3.u32, vehicle = ctx.r4.u32;
  cm::TargetTrack(base, tracker, vehicle, ctx.r1.u32 - 256);
  if (vehicle != 0) {
    ctx.r3.u64 = tracker + 224;
  }
}

// Camera::Shake: r3 = the camera; r3 = its viewport, as Offset leaves it.
void CameraModesStep82152CD8(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t camera = ctx.r3.u32;
  cm::Shake(base, camera, ctx.r1.u32 - 96);
  ctx.r3.u64 = guest::LoadU32(base, camera + 20);
}

// Viewport::PullBack: r3 = the viewport, f1 = the amount, r5 = the offset.
void CameraModesStep82152788(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::ViewportPullBack(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u32, ctx.r1.u32);
}

// IntroCamera::StepShot: r3 = the shot; r3 = 0 or 1.
void CameraModesStep82152E78(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::IntroStepShot(base, ctx.r3.u32, ctx.r1.u32 - 272);
}

}  // namespace hooks

namespace {

// atexit for IntroCamera::MakeShot where the native one can't: the game's,
// from MakeShot's frame (`sp`) with its return address.
struct MakeShotAtexit {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&MakeShotAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<MakeShotAtexit*>(self);
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = function == 0x825DB150 ? 0x82154214 : 0x8215425C;
    call.ctx.r3.u64 = function;
    __imp__sub_825A7068(call.ctx, call.base);
    call.ctx.r1.u64 = stack;
    call.ctx.fpscr.disableFlushMode();
    return call.ctx.r3.s32;
  }
};

}  // namespace

namespace hooks {

// IntroCamera::IntroCamera: r3 = the camera, r4 = the viewport, r5 = the
// vehicle; r3 = the camera. Stages::Copy's original from the original's
// frame (368 bytes) where its plain C++ refuses; MakeShot's frame below it.
// The original without native atexit.
void CameraModesStep82158F70(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeAtexit()) {
    __imp__sub_82158F70(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 368;
  MakeShotAtexit atexit{ctx, base, frame - 192};
  ctx.r3.u64 = cm::IntroConstruct(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
      [&](uint32_t out, uint32_t stages, uint32_t stage) {
        guest_call::InFrame(ctx, base, 368, 0x82159078, [&](uint32_t) {
          ctx.r3.u64 = out;
          ctx.r4.u64 = stages;
          ctx.r5.u64 = stage;
          __imp__sub_820FFD88(ctx, base);
        });
        ctx.fpscr.disableFlushMode();
      },
      atexit.Call());
}

// CameraDirector::Start (a handler): r3 = the director. Viewports::Init
// natively (with --native_heap and native atexit) from a frame like the
// original's (144 bytes), else its original.
void CameraModesStep82152338(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::DirectorStart(base, ctx.r3.u32, [&] {
    guest_call::InFrame(ctx, base, 144, 0x82152358, [&](uint32_t) {
      std::optional<uint32_t> viewport;
      if (hooks::NativeHeapActive() && hooks::NativeAtexit()) {
        viewport = hooks::ViewportsInit(ctx, base);
      }
      if (viewport) {
        ctx.r3.u64 = *viewport;
      } else {
        __imp__sub_820AB358(ctx, base);
      }
    });
    ctx.fpscr.disableFlushMode();
  });
}

// CameraDirector::Release (a handler): r3 = the director; r3 as
// Viewports::Release leaves it. CameraAnims::FreeAll and Viewports::Release
// natively where their adapters would (--native_heap, the layer's pages),
// else their originals from a frame like the original's (96 bytes).
void CameraModesStep82153098(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::DirectorRelease(
      base, ctx.r3.u32,
      [&] {
        if (hooks::NativeHeapActive() && cm::AnimsFreeable(base)) {
          if (const auto r3 = cm::AnimsFreeAll(base, ctx.r1.u32 - 96 - 384 + 80)) {
            ctx.r3.u64 = *r3;
          }
          return;
        }
        guest_call::InFrame(ctx, base, 96, 0x821530B0,
                            [&](uint32_t) { __imp__sub_821507D8(ctx, base); });
        ctx.fpscr.disableFlushMode();
      },
      [&] {
        if (hooks::NativeHeapActive() && cameras::ReleaseViewports(base)) {
          ctx.r3.u64 = 1;
          return;
        }
        guest_call::InFrame(ctx, base, 96, 0x82153138,
                            [&](uint32_t) { __imp__sub_820AA430(ctx, base); });
        ctx.fpscr.disableFlushMode();
      });
}

// CutsceneShot::Init: r3 = the shot, r4 = the script entry; r3 as
// CameraAnims::Load leaves it (the serial). The copies the native layer
// refuses go through the guest's memcpy from Copy32's and Insert32's frames
// below the original's (192 bytes); CameraAnims::Load natively where its
// adapter would (--native_heap, the archive's queue), else its original.
void CameraModesStep82156B68(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::CutsceneCalls calls;
  calls.copy32 = [&](uint32_t to, uint32_t from, uint32_t size) {
    guest_call::InFrame(ctx, base, 192, 0x82156C90, [&](uint32_t) {
      GuestMemcpy(ctx, base, 96, 0x8215435C, to, from, size);
    });
  };
  calls.insert32 = [&](uint32_t to, uint32_t from, uint32_t size) {
    guest_call::InFrame(ctx, base, 192, 0x82156CA0, [&](uint32_t) {
      GuestMemcpy(ctx, base, 112, 0x821543E0, to, from, size);
    });
  };
  calls.load = [&](uint32_t id) {
    if (hooks::NativeHeapActive() && hooks::NativeArchiveQueue()) {
      hooks::SetArchiveSerial(ctx, cm::AnimsLoad(base, id, hooks::NativeArchiveLoad(base)));
      return;
    }
    guest_call::InFrame(ctx, base, 192, 0x82156CB8, [&](uint32_t) {
      ctx.r3.u64 = id;
      __imp__sub_82150B10(ctx, base);
    });
  };
  cm::CutsceneInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192, calls);
}

// CutsceneShot::Step: r3 = the shot, r4 = where the view goes; r3 = its
// +604, f1 the last f1 (Duration's). EvalCamera's original runs from frames
// like Step's (304 bytes) and Evaluate's (144) where its native form can't.
void CameraModesStep82155800(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t shot = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 304;
  double f1 = ctx.f1.f64;
  const uint32_t r3 = cm::CutsceneStep(
      base, shot, ctx.r4.u32, frame,
      [&](uint32_t object) {
        const uint32_t time = guest::LoadU32(base, shot + 600);
        guest_call::InFrame(ctx, base, 304, 0x82155834, [&](uint32_t) {
          guest_call::InFrame(ctx, base, 144, 0x821509C4, [&](uint32_t) {
            ctx.r3.u64 = object;
            ctx.r4.u64 = object;
            ctx.r5.u64 = frame + 144;
            ctx.r6.u64 = time;
            __imp__sub_82150350(ctx, base);
          });
        });
        ctx.fpscr.disableFlushMode();
        f1 = ctx.f1.f64;
      },
      &f1);
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
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
ITC_CAMERA_MODES(82090000)
ITC_CAMERA_MODES(821544E0)
ITC_CAMERA_MODES(82154378)
ITC_CAMERA_MODES(821543E8)
ITC_CAMERA_MODES(82154300)
ITC_CAMERA_MODES(82150078)
ITC_CAMERA_MODES(82150300)
ITC_CAMERA_MODES(82152108)
ITC_CAMERA_MODES(82152290)
ITC_CAMERA_MODES(82150B10)
ITC_CAMERA_MODES(82150878)
ITC_CAMERA_MODES(821507D8)
ITC_CAMERA_MODES(82150960)
ITC_CAMERA_MODES(82150A98)
ITC_CAMERA_MODES(82150A10)
ITC_CAMERA_MODES(82152E08)
ITC_CAMERA_MODES(82152078)
ITC_CAMERA_MODES(82211848)
ITC_CAMERA_MODES(82211AB0)
ITC_CAMERA_MODES(82153290)
ITC_CAMERA_MODES(82152CD8)
ITC_CAMERA_MODES(82152788)
ITC_CAMERA_MODES(82152E78)
ITC_CAMERA_MODES(82158F70)
ITC_CAMERA_MODES(82152338)
ITC_CAMERA_MODES(82153098)
ITC_CAMERA_MODES(82156B68)
ITC_CAMERA_MODES(82155800)
