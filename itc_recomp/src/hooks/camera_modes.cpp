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
#include "hooks/ui_movies_game.h"
#include "game_modes/game_modes.h"
#include "cameras/cameras.h"

ITC_FLAG_BOOL(native_camera_modes, true, "ITC",
              "Run the camera modes (docs/functions/camera-modes.md: the camera director's "
              "handlers CameraDirector::ToView4 sub_82152400, ToView5 sub_821524B8, ToView3 "
              "sub_821524D8 and ToView3IfCourse sub_82153160; Viewports::ResetRoll sub_821524F0, "
              "Camera::ZoomFromFov sub_82152548, Quat::Rotate sub_821525F8 (no adapter: only ChaseCamera::Follow's plain C++ calls it), Matrix::FromQuat "
              "sub_82152698 (no adapter: only CameraTarget::Track's plain C++ calls it), Matrix::FromFloats sub_82090000; the lists' List40::Link "
              "sub_82153228 (no adapter: only List40::Insert's plain C++ calls it), "
              "List40::Insert sub_821544E0, List16::Insert20 sub_82154458 (no adapter: only IntroCamera::IntroCamera's plain C++ calls it), "
              "Insert32 sub_82154378, Insert608 sub_821543E8 and Copy32 sub_82154300 (no adapter: only CutsceneShot::Init's plain C++ calls it); "
              "CameraAnims::Clear sub_82150078 and Find sub_82150300; ColorFilter::SetStyle "
              "sub_82152108 (no adapter: only TvCamera::Update's plain C++ calls it) and FadeCue::Step sub_82152290 (no adapter: only CutsceneShot::Step's plain C++ calls it); with --native_heap the camera "
              "animations' CameraAnims::Load sub_82150B10 (with the archive's queue native), "
              "OnLoaded sub_82150878 with CameraAnim::ParseObject sub_82150100 (no adapter), "
              "FreeAll sub_821507D8 with CameraAnim::Free sub_82150098 and CameraAnimBlock::Free "
              "sub_8214FFF0 (no adapters); CameraAnims::Evaluate sub_82150960 with "
              "CameraAnim::EvalCamera sub_82150350 and EvalTarget sub_821504E0 (no adapters), "
              "Duration sub_82150A98 (no adapter: only CutsceneShot::Step's plain C++ calls it), ObjectMatrix sub_82150A10 with CameraAnim::Matrix "
              "sub_82150590 (no adapter); with --native_heap and the archive's queue native "
              "CameraPath::Request sub_82152E08 with Load sub_82152D80 (no adapter); "
              "CameraPath::OnLoaded sub_82152078 with FixKeys sub_820AC000, FixKey sub_820ABF80, "
              "FixGroup sub_820ABED8 and FixTriple sub_820ABE68 (no adapters); "
              "RivalLogo::RivalLogo sub_82211848 with InitXpr sub_82211708 (no adapter) and "
              "RivalLogo::Play sub_82211AB0 (no adapter: only Cutscene::Step's plain C++ calls it; "
              "without --log_pause_movie); CameraTarget::Track "
              "sub_82153290; Camera::Shake sub_82152CD8 (no adapter: only ChaseCamera::Update's "
              "plain C++ calls it) with ShakeRandom sub_82152BD8 and "
              "Viewport::Offset sub_821529B0 (no adapters), Viewport::PullBack sub_82152788 (no adapter: only ChaseCamera::Update's plain C++ calls it); "
              "IntroCamera::StepShot sub_82152E78 (no adapter: only IntroCamera::Update's plain C++ "
              "calls it), IntroCamera::IntroCamera sub_82158F70 with "
              "MakeShot sub_82154108 (no adapter; with native atexit); CameraDirector::Start "
              "sub_82152338 and Release sub_82153098; CutsceneShot::Init sub_82156B68 and Step "
              "sub_82155800 (no adapter: only Cutscene::Step's plain C++ calls it)); the TV camera's RouteRange::CameraAt sub_820FE830 "
              "and TrackRoute::SliceFraction sub_820F9B48 (no adapters: only TvCamera::Step's plain C++ "
              "calls them), TvCamera::Track sub_82156980 and, with "
              "native atexit, CameraPath::Evaluate sub_820AC248 (no adapter: only TvCamera::Step's "
              "plain C++ calls it) with RouteRange::Advance "
              "sub_820FE598, Curve::Evaluate sub_820AC060, Hermite::Basis sub_82293A48, "
              "Key::Value sub_820AC178, Key::Vec3 sub_820AC1D0, CameraPath::RoutePoint "
              "sub_820ABDA8, TrackRoute::SplinePoint sub_820AB6A0 and Matrix::FromRows3 "
              "sub_820AB5F0 (no adapters)); the views' cameras' ChaseCamera::ChaseCamera "
              "sub_82154548 (with native atexit) with Place sub_82153478 and Follow "
              "sub_82153640 (no adapters), ChaseCamera::Update sub_82154808, "
              "ViewCamera1::ViewCamera1 sub_82154A48 and ViewCamera2::ViewCamera2 sub_82154D38 "
              "(with native atexit), ViewCamera1::Update sub_82153A50 and ViewCamera2::Update "
              "sub_82153E58; IntroCamera::Update sub_82155E58; RivalLogo::Open sub_822118E0 and Close "
              "sub_82211980 (no adapter: only Cutscene::Step's plain C++ calls it; the UI movies' "
              "Open and Delete natively where their adapters would); Cutscene::Step "
              "sub_82155A28 (no adapter: only Update's plain C++ calls it) and Update sub_821561F8 (the logo's draw "
              "through its override, sub_82211A28); the TV camera's TvCamera::TvCamera sub_82155288 "
              "(with native atexit), TvCamera::Update sub_821593D8 with Step sub_821562F8 (no "
              "adapter: only Update's plain C++ calls it; with native atexit; the attract race's color "
              "filter by rand); the front end's GarageCamera::Update sub_8217BE98; the director's "
              "CameraDirector::InitStates sub_82159950) natively instead of "
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

// A guest call from the frame at `frame` (the calling original's r1,
// `frame_bytes` below its caller's) with the return address `back`.
template <typename Call>
void CallFrom(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t frame_bytes,
              uint32_t back, Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + frame_bytes;
  guest_call::InFrame(ctx, base, frame_bytes, back, [&](uint32_t) { call(); });
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
}

// CameraPath::Request (r3 the TV camera, r4 the buffer, r5 the index)
// through its step from a TV camera function's frame at `frame`
// (`frame_bytes`) with the return address `back`.
void PathRequestFrom(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t frame_bytes,
                     uint32_t back, uint32_t camera, uint32_t buffer, uint32_t index) {
  CallFrom(ctx, base, frame, frame_bytes, back, [&] {
    ctx.r3.u64 = camera;
    ctx.r4.u64 = buffer;
    ctx.r5.u64 = index;
    hooks::CameraModesStep82152E08(ctx, base);
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

// Quat::Rotate (no adapter: only ChaseCamera::Follow's plain C++ calls it; the
// step kept for the harness): r3 = out, r4 = the quaternion, r5 = the vector.
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
// List16::Copy32 (no adapter: only CutsceneShot::Init's plain C++ calls it; the step kept
// for the harness).
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

// ColorFilter::SetStyle (no adapter: only TvCamera::Update's plain C++ calls
// it; the step kept for the harness): r3 = &the style. The original runs where the
// filter's slots aren't its own functions.
void CameraModesStep82152108(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cm::ColorFilterSetStyle(base, ctx.r3.u32)) {
    __imp__sub_82152108(ctx, base);
  }
}

// FadeCue::Step (no adapter: only CutsceneShot::Step's plain C++ calls it; the step kept
// for the harness).
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

// CameraAnims::Duration (no adapter: only CutsceneShot::Step's plain C++ calls it; the
// step kept for the harness).
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

// Camera::Shake (no adapter: only ChaseCamera::Update's plain C++ calls it; the
// step kept for the harness): r3 = the camera; r3 = its viewport, as Offset
// leaves it.
void CameraModesStep82152CD8(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t camera = ctx.r3.u32;
  cm::Shake(base, camera, ctx.r1.u32 - 96);
  ctx.r3.u64 = guest::LoadU32(base, camera + 20);
}

// Viewport::PullBack (no adapter: only ChaseCamera::Update's plain C++ calls
// it; the step kept for the harness): r3 = the viewport, f1 = the amount, r5 =
// the offset.
void CameraModesStep82152788(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::ViewportPullBack(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u32, ctx.r1.u32);
}

// IntroCamera::StepShot (no adapter: only IntroCamera::Update's plain C++
// calls it; the step kept for the harness): r3 = the shot; r3 = 0 or 1.
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

}  // namespace hooks

namespace {

// CameraAnim::EvalCamera's original for CutsceneShot::Step (`shot`, its
// frame at `frame`: the original's r1, 304 bytes) where its native form
// can't: from frames like Step's and Evaluate's (144 bytes). f1 as it
// leaves it.
void ShotCameraOriginal(PPCContext& ctx, uint8_t* base, uint32_t shot, uint32_t frame,
                        uint32_t object) {
  const uint32_t time = guest::LoadU32(base, shot + 600);
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + 304;
  guest_call::InFrame(ctx, base, 304, 0x82155834, [&](uint32_t) {
    guest_call::InFrame(ctx, base, 144, 0x821509C4, [&](uint32_t) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = object;
      ctx.r5.u64 = frame + 144;
      ctx.r6.u64 = time;
      __imp__sub_82150350(ctx, base);
    });
  });
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
}

}  // namespace

namespace hooks {

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
        ShotCameraOriginal(ctx, base, shot, frame, object);
        f1 = ctx.f1.f64;
      },
      &f1);
  ctx.f1.f64 = f1;
  ctx.r3.u64 = r3;
}

}  // namespace hooks

// --- The TV camera's route and paths ---

namespace {

// TrackRoute::SplinePoint's atexit through the translated code when the
// native one can't: from its frame (`sp`, the original's r1 there) with its
// return address.
struct SplineAtexit {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&SplineAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<SplineAtexit*>(self);
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = 0x820ABB20;
    call.ctx.r3.u64 = function;
    __imp__sub_825A7068(call.ctx, call.base);
    call.ctx.r1.u64 = stack;
    call.ctx.fpscr.disableFlushMode();
    return call.ctx.r3.s32;
  }
};

}  // namespace

namespace hooks {

// RouteRange::CameraAt (no adapter: only TvCamera::Step's plain C++ calls it;
// the step kept for the harness): r3 = the range, r4 = &the position; r3 = the
// slices counted (0x82603274's word) or -1.
void CameraModesStep820FE830(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = cm::RangeCameraAt(base, ctx.r3.u32, ctx.r4.u32);
}

// TrackRoute::SliceFraction (no adapter: only TvCamera::Step's plain C++
// calls it; the step kept for the harness): r3 = &the position, r4 = the point; f1 the
// fraction. Its caller doesn't read r3.
void CameraModesStep820F9B48(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = cm::SliceFraction(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

// TvCamera::Track: r3 = the TV camera, r4 = &the position (its caller
// doesn't read r3 after). CameraPath::Request through its step (which runs
// its original where it can't) from a frame like the original's (496
// bytes).
void CameraModesStep82156980(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t camera = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 496;
  cm::TvTrack(base, camera, ctx.r4.u32, frame, [&](uint32_t buffer, uint32_t index) {
    PathRequestFrom(ctx, base, frame, 496, 0x82156B54, camera, buffer, index);
  });
}

// CameraPath::Evaluate (no adapter: only TvCamera::Step's plain C++ calls it;
// the step kept for the harness): r3 = the key, r4 = the camera's out, r5 = the
// route range, r6 = the transform, f1 = t (its caller reads neither r3 nor
// f1 after). The original without native atexit (TrackRoute::SplinePoint's
// static); the atexit's fallback from SplinePoint's frame (272 + 208 + 720
// bytes below).
void CameraModesStep820AC248(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeAtexit()) {
    __imp__sub_820AC248(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  SplineAtexit atexit{ctx, base, ctx.r1.u32 - 272 - 208 - 720};
  cm::PathEvaluate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                   ctx.r1.u32, atexit.Call());
}

}  // namespace hooks


// --- The views' cameras ---

namespace {

// The video output from a native original's frame (`frame`, its r1): its
// two slot calls (`[0x8266D088]`'s slot 7, then that object's slot 3, as
// Viewport::Init's adapter makes them) with the original's return
// addresses. Returns the video output.
uint32_t VideoOutputFrom(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t back) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.r3.u64 = guest::LoadU32(base, 0x8266D088);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
  ctx.lr = back;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
  ctx.lr = back + 16;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u32;
}

cm::VideoOutputCall VideoOutput(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t frame, uint32_t back) {
    return VideoOutputFrom(ctx, base, frame, back);
  };
}

// ChaseCamera::ChaseCamera's atexit through the translated code when the
// native one can't: from its frame (`sp`) with its return addresses.
struct ChaseAtexit {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&ChaseAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<ChaseAtexit*>(self);
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = function == 0x825DB100 ? 0x821546D4 : 0x8215471C;
    call.ctx.r3.u64 = function;
    __imp__sub_825A7068(call.ctx, call.base);
    call.ctx.r1.u64 = stack;
    call.ctx.fpscr.disableFlushMode();
    return call.ctx.r3.s32;
  }
};

// The views' constructors' atexit through the translated code when the
// native one can't: from the constructor's frame (`sp`), the return address
// by the function registered.
struct ViewAtexit {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&ViewAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<ViewAtexit*>(self);
    uint32_t back = 0;
    switch (function) {
      case 0x825DB120: back = 0x82154B24; break;
      case 0x825DB118: back = 0x82154B5C; break;
      case 0x825DB110: back = 0x82154B9C; break;
      case 0x825DB108: back = 0x82154BD4; break;
      case 0x825DB140: back = 0x82154E18; break;
      case 0x825DB138: back = 0x82154E58; break;
      case 0x825DB130: back = 0x82154E90; break;
      default: back = 0x82154ED0; break;  // 0x825DB128
    }
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = back;
    call.ctx.r3.u64 = function;
    __imp__sub_825A7068(call.ctx, call.base);
    call.ctx.r1.u64 = stack;
    call.ctx.fpscr.disableFlushMode();
    return call.ctx.r3.s32;
  }
};

// Whether the chase camera's plain C++ can stand in: a 16-byte aligned
// stack pointer (CameraTarget::Track's and the vector loads' frames) and
// ZoomFromFov's tan.
bool ChaseNative(PPCContext& ctx, uint8_t* base) {
  return (ctx.r1.u32 & 0xF) == 0 && cm::ChaseReady(base);
}

}  // namespace

namespace hooks {

// ChaseCamera::Place (no adapter: only ChaseCamera::ChaseCamera's plain C++
// calls it; the step kept for the harness): r3 = the camera, r4 to r7 the
// four vectors. A stack pointer not 16-byte aligned runs the original.
void CameraModesStep82153478(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_82153478(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::ChasePlace(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                 ctx.r1.u32 - 112);
}

// ChaseCamera::Follow (no adapter: only ChaseCamera::ChaseCamera's and
// Update's plain C++ call it; the step kept for the harness): r3 = the
// camera, f1 = the rate, f2 = the least. The original where ChaseNative
// says no.
void CameraModesStep82153640(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base)) {
    __imp__sub_82153640(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::ChaseFollow(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r1.u32 - 320,
                  VideoOutput(ctx, base));
}

// ChaseCamera::ChaseCamera: r3 = the camera, r4 = the viewport, r5 = the
// vehicle; r3 = the camera. The original where ChaseNative says no or
// without native atexit.
void CameraModesStep82154548(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base) || !hooks::NativeAtexit()) {
    __imp__sub_82154548(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 224;
  ChaseAtexit atexit{ctx, base, frame};
  ctx.r3.u64 = cm::ChaseConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                                  VideoOutput(ctx, base), atexit.Call());
}

// ChaseCamera::Update (vtable slot 0; its caller reads neither r3 nor f1
// after): r3 = the camera. The original where ChaseNative says no.
void CameraModesStep82154808(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base)) {
    __imp__sub_82154808(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::ChaseUpdate(base, ctx.r3.u32, ctx.r1.u32 - 144, VideoOutput(ctx, base));
}

// TvCamera::TvCamera: r3 = the camera, r4 = the viewport, r5 = the vehicle;
// r3 = the camera. The original where ChaseNative says no or without native
// atexit (the fallback chase camera's statics).
void CameraModesStep82155288(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base) || !hooks::NativeAtexit()) {
    __imp__sub_82155288(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 160;
  ChaseAtexit atexit{ctx, base, frame - 224};
  ctx.r3.u64 = cm::TvConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                               VideoOutput(ctx, base), atexit.Call());
}

}  // namespace hooks


namespace hooks {

// ViewCamera1::ViewCamera1 and ViewCamera2::ViewCamera2: r3 = the camera,
// r4 = the viewport, r5 = the vehicle; r3 = the camera. The original where
// ChaseNative says no or without native atexit.
void CameraModesStep82154A48(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base) || !hooks::NativeAtexit()) {
    __imp__sub_82154A48(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 192;
  ViewAtexit atexit{ctx, base, frame};
  ctx.r3.u64 = cm::View1Construct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                                  VideoOutput(ctx, base), atexit.Call());
}
void CameraModesStep82154D38(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base) || !hooks::NativeAtexit()) {
    __imp__sub_82154D38(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 192;
  ViewAtexit atexit{ctx, base, frame};
  ctx.r3.u64 = cm::View2Construct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                                  VideoOutput(ctx, base), atexit.Call());
}

// ViewCamera1::Update and ViewCamera2::Update (vtable slot 0; their caller
// reads neither r3 nor f1 after): r3 = the camera. The original where
// ChaseNative says no.
void CameraModesStep82153A50(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base)) {
    __imp__sub_82153A50(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::View1Update(base, ctx.r3.u32, ctx.r1.u32 - 352, VideoOutput(ctx, base));
}
void CameraModesStep82153E58(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base)) {
    __imp__sub_82153E58(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::View2Update(base, ctx.r3.u32, ctx.r1.u32 - 240, VideoOutput(ctx, base));
}

}  // namespace hooks

namespace hooks {

// IntroCamera::Update (vtable slot 0; its caller reads neither r3 nor f1
// after): r3 = the camera. A stack pointer not 16-byte aligned runs the
// original (CameraTarget::Track's frame).
void CameraModesStep82155E58(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    __imp__sub_82155E58(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::IntroUpdate(base, ctx.r3.u32, ctx.r1.u32 - 320);
}

}  // namespace hooks

// --- The adapters ---

ITC_CAMERA_MODES(82152400)
ITC_CAMERA_MODES(821524B8)
ITC_CAMERA_MODES(821524D8)
ITC_CAMERA_MODES(82153160)
ITC_CAMERA_MODES(821524F0)
ITC_CAMERA_MODES(82152548)
ITC_CAMERA_MODES(82090000)
ITC_CAMERA_MODES(821544E0)
ITC_CAMERA_MODES(82154378)
ITC_CAMERA_MODES(821543E8)
ITC_CAMERA_MODES(82150078)
ITC_CAMERA_MODES(82150300)
ITC_CAMERA_MODES(82150B10)
ITC_CAMERA_MODES(82150878)
ITC_CAMERA_MODES(821507D8)
ITC_CAMERA_MODES(82150960)
ITC_CAMERA_MODES(82150A10)
ITC_CAMERA_MODES(82152E08)
ITC_CAMERA_MODES(82152078)
ITC_CAMERA_MODES(82211848)
ITC_CAMERA_MODES(82153290)
ITC_CAMERA_MODES(82158F70)
ITC_CAMERA_MODES(82152338)
ITC_CAMERA_MODES(82153098)
ITC_CAMERA_MODES(82156B68)
ITC_CAMERA_MODES(82156980)
ITC_CAMERA_MODES(82154548)
ITC_CAMERA_MODES(82154808)
ITC_CAMERA_MODES(82154A48)
ITC_CAMERA_MODES(82153A50)
ITC_CAMERA_MODES(82154D38)
ITC_CAMERA_MODES(82153E58)
ITC_CAMERA_MODES(82155E58)
ITC_CAMERA_MODES(82155288)

// --- The rival logo's open and close ---

namespace hooks {

// RivalLogo::Open: r3 = the entry, r4 = the logo, r5 = the texture; r3 as
// the original leaves it. UiMovies::Open natively where its adapter would,
// else its adapter, and RivalLogo::LoadXpr (never run in play) through the
// translated code, from frames like the original's (112 bytes).
void CameraModesStep822118E0(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32 - 112;
  ctx.r3.u64 = cm::RivalLogoOpen(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
      [&](uint32_t id, uint32_t asset) -> uint32_t {
        if (ui_movies_game_hooks::OpenNative()) {
          return ui_movies::OpenMovie(base, id, asset, sp - 128, hooks::NativeArchiveLoad(base),
                                      ui_movies_game_hooks::OpenCalls(ctx, base, sp));
        }
        guest_call::InFrame(ctx, base, 112, 0x8221195C, [&](uint32_t) {
          ctx.r3.u64 = id;
          ctx.r4.u64 = asset;
          sub_8211E808(ctx, base);
        });
        return ctx.r3.u32;
      },
      [&](uint32_t holder, uint32_t id) -> uint32_t {
        guest_call::InFrame(ctx, base, 112, 0x82211978, [&](uint32_t) {
          ctx.r3.u64 = holder;
          ctx.r4.u64 = id;
          sub_82211878(ctx, base);
        });
        return ctx.r3.u32;
      });
}

}  // namespace hooks

namespace {

// RivalLogo::Close's calls from its frame at `sp` (the original's r1, 128
// bytes): UiMovie::FinishAll through its adapter and RivalLogo::ReleaseXpr
// (never run in play) through the translated code, UiMovies::Delete
// natively where its adapter would, else its adapter, from frames like the
// original's.
cm::RivalLogoCloseCalls RivalLogoCloseCallsAt(PPCContext& ctx, uint8_t* base, uint32_t sp) {
  auto call = [&ctx, base, sp](uint32_t back, void (*function)(PPCContext&, uint8_t*),
                               uint32_t r3) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp + 128;
    guest_call::InFrame(ctx, base, 128, back, [&](uint32_t) {
      ctx.r3.u64 = r3;
      function(ctx, base);
    });
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
  cm::RivalLogoCloseCalls calls;
  calls.finish_all = [call](uint32_t movie) { call(0x822119C0, sub_8211DFF8, movie); };
  calls.release_xpr = [call](uint32_t holder) { call(0x822119F8, sub_82211768, holder); };
  calls.remove = [&ctx, base, sp, call](uint32_t id) -> uint32_t {
    if (ui_movies_game_hooks::DeleteNative()) {
      return ui_movies::DeleteMovie(base, id, sp - 128,
                                    ui_movies_game_hooks::DeleteCalls(ctx, base, sp));
    }
    return call(0x82211A00, sub_8211E258, id);
  };
  return calls;
}

// Cutscene::Step's calls from its frame at `frame` (the original's r1, 192
// bytes): CutsceneShot::Step's frame 304 bytes below it, RivalLogo::Close's
// 128; RivalLogo::Play's original with --log_pause_movie (as its adapter);
// RivalLogo::Draw through its override (sub_82211A28, which frame
// interpolation's --redraw_rival_logo notes) with Step's return address.
cm::CutsceneTickCalls CutsceneTickCallsAt(PPCContext& ctx, uint8_t* base, uint32_t frame) {
  cm::CutsceneTickCalls calls;
  calls.camera_original = [&ctx, base, frame](uint32_t shot, uint32_t object) {
    ShotCameraOriginal(ctx, base, shot, frame - 304, object);
  };
  calls.close = RivalLogoCloseCallsAt(ctx, base, frame - 128);
  if (ITC_FLAG(log_pause_movie)) {
    calls.play_original = [&ctx, base, frame](uint32_t entry) {
      CallFrom(ctx, base, frame, 192, 0x82155A64, [&] {
        ctx.r3.u64 = entry;
        __imp__sub_82211AB0(ctx, base);
      });
    };
  }
  calls.draw = [&ctx, base, frame](uint32_t entry) {
    CallFrom(ctx, base, frame, 192, 0x82155BE0, [&] {
      ctx.r3.u64 = entry;
      sub_82211A28(ctx, base);
    });
  };
  return calls;
}

}  // namespace

namespace hooks {

// RivalLogo::Close: r3 = the entry; r3 as the original leaves it.
void CameraModesStep82211980(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32 - 128;
  ctx.r3.u64 = cm::RivalLogoClose(base, ctx.r3.u32, sp, RivalLogoCloseCallsAt(ctx, base, sp));
}

// Cutscene::Step: r3 = the cutscene; r3 = 1 while running, 0 at its last
// shot; lr put back as the original's epilogue does (its only caller,
// Cutscene::Update, reads neither f1 nor the stack after).
void CameraModesStep82155A28(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 192;
  const uint32_t cutscene = ctx.r3.u32;
  ctx.r3.u64 = cm::CutsceneTick(base, cutscene, frame, CutsceneTickCallsAt(ctx, base, frame));
  ctx.lr = lr;
}

// Cutscene::Update (vtable slot 0; CameraDirector::Update reads neither r3
// nor f1 after): r3 = the cutscene; r3 as the original leaves it, lr put
// back. Step's frame (192 bytes) below Update's (96).
void CameraModesStep821561F8(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 96;
  const uint32_t cutscene = ctx.r3.u32;
  ctx.r3.u64 =
      cm::CutsceneUpdate(base, cutscene, frame, CutsceneTickCallsAt(ctx, base, frame - 192));
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES(822118E0)
ITC_CAMERA_MODES(821561F8)

// --- The TV camera's update ---

namespace {

// TvCamera::Step's guest calls from its frame at `frame` (720 bytes).
cm::TvStepCalls TvStepCallsAt(PPCContext& ctx, uint8_t* base, uint32_t frame,
                              uint32_t camera) {
  cm::TvStepCalls calls;
  calls.copy = [&ctx, base, frame](uint32_t to, uint32_t from, uint32_t size) {
    CallFrom(ctx, base, frame, 720, 0x8215634C, [&] {
      ctx.r3.u64 = to;
      ctx.r4.u64 = from;
      ctx.r5.u64 = size;
      sub_825A7410(ctx, base);
    });
  };
  calls.request = [&ctx, base, frame, camera](uint32_t buffer, uint32_t index, uint32_t back) {
    PathRequestFrom(ctx, base, frame, 720, back, camera, buffer, index);
  };
  return calls;
}

}  // namespace

namespace hooks {

// TvCamera::Step (no adapter: only TvCamera::Update's plain C++ calls it; the
// step kept for the harness): r3 = the TV camera. The original
// with a stack pointer not 16-byte aligned (CameraTarget::Track's frame) or
// without native atexit (CameraPath::Evaluate's spline static; its fallback
// from SplinePoint's frame, 272 + 208 + 720 bytes below Step's).
void CameraModesStep821562F8(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0 || !hooks::NativeAtexit()) {
    __imp__sub_821562F8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 720;
  const uint32_t camera = ctx.r3.u32;
  SplineAtexit atexit{ctx, base, frame - 272 - 208 - 720};
  cm::TvStep(base, camera, frame, TvStepCallsAt(ctx, base, frame, camera), atexit.Call());
  ctx.lr = lr;
}

// TvCamera::Update (vtable slot 0; CameraDirector::Update reads neither r3
// nor f1 after): r3 = the TV camera; lr put back. The fallback chase
// camera's update natively (its frame 144 bytes below Update's 128), else
// its slot 0 through the translated code; Step's frame 720 bytes below.
// The original where ChaseNative says no or without native atexit.
void CameraModesStep821593D8(PPCContext& ctx, uint8_t* base) {
  if (!ChaseNative(ctx, base) || !hooks::NativeAtexit()) {
    __imp__sub_821593D8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 128;
  const uint32_t camera = ctx.r3.u32;
  SplineAtexit atexit{ctx, base, frame - 720 - 272 - 208 - 720};
  cm::TvUpdateCalls calls;
  calls.step = TvStepCallsAt(ctx, base, frame - 720, camera);
  calls.video_output = VideoOutput(ctx, base);
  calls.slot0 = [&ctx, base, frame](uint32_t function, uint32_t chase) {
    CallFrom(ctx, base, frame, 128, 0x82159408, [&] {
      ctx.r3.u64 = chase;
      ctx.ctr.u64 = function;
      REX_CALL_INDIRECT_FUNC(function);
    });
  };
  calls.set_style_original = [&ctx, base, frame](uint32_t style) {
    CallFrom(ctx, base, frame, 128, 0x821594A0, [&] {
      ctx.r3.u64 = style;
      __imp__sub_82152108(ctx, base);
    });
  };
  cm::TvUpdate(base, camera, frame, calls, atexit.Call());
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES(821593D8)

// --- The director's state machine ---

namespace hooks {

// CameraDirector::InitStates: r3 = the director; r3 as the last
// StateMachine::Add leaves it (its adapter's: a new transition's address
// from the machine's 64-bit r3, src/hooks/game_modes.cpp).
void CameraModesStep82159950(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64 + cm::kMachine;
  const uint32_t machine = ctx.r3.u32 + cm::kMachine;
  bool replaced = false;
  const uint32_t r3 =
      cm::DirectorInitStates(base, ctx.r3.u32, ctx.r1.u32 - cm::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

ITC_CAMERA_MODES(82159950)

// --- The garage camera ---

namespace hooks {

// GarageCamera::Update (its one caller, the front end's car draw
// sub_8217FE60, reads neither r3 nor f1 after): r3 = the camera; lr put
// back, r3 = camera pass 0 (as CameraPass::Enable leaves it). The app
// interface's four slot calls from its 272-byte frame with the original's
// return addresses; CameraPass::Update from FromViewport's 112-byte frame
// below it, as FromViewport's adapter makes it. The original with a stack
// pointer not 16-byte aligned (the vector loads and stores on its frame) or
// where tan's native form can't stand in.
void CameraModesStep8217BE98(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0 || !cm::GarageReady(base, ctx.r3.u32)) {
    __imp__sub_8217BE98(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 272;
  cm::GarageCalls calls;
  calls.queue_and_list = [&ctx, base](uint32_t f) {
    std::pair<uint32_t, uint32_t> answer;
    CallFrom(ctx, base, f, 272, 0x8217BEC8, [&] {
      const auto slot = [&](uint32_t object, uint32_t offset, uint32_t back) {
        ctx.r3.u64 = object;
        ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, object) + offset);
        ctx.lr = back;
        REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
        return ctx.r3.u32;
      };
      const uint32_t scene = slot(guest::LoadU32(base, 0x826BABA0), 32, 0x8217BEC8);
      answer.first = slot(scene, 8, 0x8217BED8);
      const uint32_t again = slot(guest::LoadU32(base, 0x826BABA0), 32, 0x8217BEF4);
      answer.second = slot(again, 4, 0x8217BF04);
    });
    return answer;
  };
  calls.pass_update = [&ctx, base](uint32_t pass, uint32_t f) {
    CallFrom(ctx, base, f - 112, 112, 0x820AA534, [&] {
      ctx.r3.u64 = pass;
      sub_820A3978(ctx, base);
    });
  };
  cm::GarageUpdate(base, ctx.r3.u32, frame, calls);
  ctx.r3.u64 = 0x826BAC00;
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES(8217BE98)
