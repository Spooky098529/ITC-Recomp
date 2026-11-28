// The camera modes natively: adapters in front of src/camera_modes/
// (docs/functions/camera-modes.md). Each leaves r3 as the original does
// where a caller could read it; flush-to-zero is left off, as the
// translated code leaves it. The director's handlers are reached through
// its state machine's functors, so their adapters stay.

#include "guest.h"
#include "lifted/names.h"
#include "lifted/functions.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "camera_modes/camera_modes.h"
#include "hooks/camera_modes.h"
#include "hooks/direct_slots.h"
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "hooks/cameras.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/ui_movies_game.h"
#include "game_modes/game_modes.h"
#include "cameras/cameras.h"
#include "lifted/sdk.h"
#include "schema/app_start_up.h"
#include "schema/cameras.h"

ITC_FLAG_BOOL(native_camera_modes, true, "ITC",
              "Run the camera modes (docs/functions/camera-modes.md: the camera director's "
              "handlers CameraDirector::ToView4 sub_82152400, ToView5 sub_821524B8, ToView3 "
              "sub_821524D8 and ToView3IfCourse sub_82153160; Viewports::ResetRoll sub_821524F0 (no adapter: only SetMode's plain C++ calls it), "
              "Camera::ZoomFromFov sub_82152548 (no adapter: only the views' cameras' plain C++ calls it), Quat::Rotate sub_821525F8 (no adapter: only ChaseCamera::Follow's plain C++ calls it), Matrix::FromQuat "
              "sub_82152698 (no adapter: only CameraTarget::Track's plain C++ calls it), Matrix::FromFloats sub_82090000 (no adapter: only TrackRoute::SplinePoint's and the static matrices' plain C++ call it in play); the lists' List40::Link "
              "sub_82153228 (no adapter: only List40::Insert's plain C++ calls it), "
              "List40::Insert sub_821544E0 (no adapter: only TvCamera::Track's and Step's plain C++ calls it), List16::Insert20 sub_82154458 (no adapter: only IntroCamera::IntroCamera's plain C++ calls it), "
              "Insert32 sub_82154378, Insert608 sub_821543E8 (no adapters: only CutsceneShot::Init's and Cutscene::Cutscene's plain C++ calls them) and Copy32 sub_82154300 (no adapter: only CutsceneShot::Init's plain C++ calls it); "
              "CameraAnims::Clear sub_82150078 and Find sub_82150300 (no adapter); ColorFilter::SetStyle "
              "sub_82152108 (no adapter: only TvCamera::Update's plain C++ calls it) and FadeCue::Step sub_82152290 (no adapter: only CutsceneShot::Step's plain C++ calls it); with --native_heap the camera "
              "animations' CameraAnims::Load sub_82150B10 (with the archive's queue native), "
              "OnLoaded sub_82150878 with CameraAnim::ParseObject sub_82150100 (no adapter), "
              "FreeAll sub_821507D8 with CameraAnim::Free sub_82150098 and CameraAnimBlock::Free "
              "sub_8214FFF0 (no adapters); CameraAnims::Evaluate sub_82150960 (no adapter) with "
              "CameraAnim::EvalCamera sub_82150350 and EvalTarget sub_821504E0 (no adapters), "
              "Duration sub_82150A98 (no adapter: only CutsceneShot::Step's plain C++ calls it), ObjectMatrix sub_82150A10 (no adapter) with CameraAnim::Matrix "
              "sub_82150590 (no adapter); with --native_heap and the archive's queue native "
              "CameraPath::Request sub_82152E08 (no adapter: only the TV camera's plain C++ calls it) with Load sub_82152D80 (no adapter); "
              "CameraPath::OnLoaded sub_82152078 with FixKeys sub_820AC000, FixKey sub_820ABF80, "
              "FixGroup sub_820ABED8 and FixTriple sub_820ABE68 (no adapters); "
              "RivalLogo::RivalLogo sub_82211848 (no adapter: only Cutscene::Cutscene's plain C++ calls it) with InitXpr sub_82211708 (no adapter) and "
              "RivalLogo::Play sub_82211AB0 (no adapter: only Cutscene::Step's plain C++ calls it; "
              "without --log_pause_movie); CameraTarget::Track "
              "sub_82153290 (no adapter: only the cameras' plain C++ calls it); Camera::Shake sub_82152CD8 (no adapter: only ChaseCamera::Update's "
              "plain C++ calls it) with ShakeRandom sub_82152BD8 and "
              "Viewport::Offset sub_821529B0 (no adapters), Viewport::PullBack sub_82152788 (no adapter: only ChaseCamera::Update's plain C++ calls it); "
              "IntroCamera::StepShot sub_82152E78 (no adapter: only IntroCamera::Update's plain C++ "
              "calls it), IntroCamera::IntroCamera sub_82158F70 with "
              "MakeShot sub_82154108 (no adapter; with native atexit); CameraDirector::Start "
              "sub_82152338 and Release sub_82153098; CutsceneShot::Init sub_82156B68 (no adapter: only Cutscene::Cutscene's plain C++ calls it) and Step "
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
              "sub_82153E58; IntroCamera::Update sub_82155E58; RivalLogo::Open sub_822118E0 (no adapter: only Cutscene::Cutscene's plain C++ calls it) and Close "
              "sub_82211980 (no adapter: only Cutscene::Step's plain C++ calls it; the UI movies' "
              "Open and Delete natively where their adapters would); Cutscene::Step "
              "sub_82155A28 (no adapter: only Update's plain C++ calls it) and Update sub_821561F8 (the logo's draw "
              "through its override, sub_82211A28); the TV camera's TvCamera::TvCamera sub_82155288 "
              "(with native atexit), TvCamera::Update sub_821593D8 with Step sub_821562F8 (no "
              "adapter: only Update's plain C++ calls it; with native atexit; the attract race's color "
              "filter by rand); the front end's GarageCamera::Update sub_8217BE98; the director's "
              "CameraDirector::InitStates sub_82159950, SetMode sub_821591F0, OnRaceStart sub_821596B0, Resume sub_821597F0 and Update sub_821594B8 (through --redraw_rival_logo's wrapper); the cutscene's Cutscene::Cutscene sub_82156CC0 "
              "with its scripts' cues and shots and the rival logo's open) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=camera_modes)");

ITC_FLAG_DECLARE(bool, log_pause_movie);

namespace cm = camera_modes;

// An adapter: the step (hooks::CameraModesStep<address>, shared with the
// harness) behind the flag.
#define ITC_CAMERA_MODES(address)                       \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_camera_modes)) { \
    hooks::CameraModesStep##address(ctx);         \
  }

// The views' camera constructors' stand-in while the SetMode harness asks
// (hooks::SetCameraConstructStandIn): it answers in place of the
// constructor, on the recompiled and the native path alike.
namespace {
hooks::CameraConstructStandIn g_construct_stand_in = nullptr;
}  // namespace

void hooks::SetCameraConstructStandIn(CameraConstructStandIn stand_in) {
  g_construct_stand_in = stand_in;
}

// A view's camera constructor's adapter: the stand-in while the harness
// asks, else as ITC_CAMERA_MODES.
#define ITC_CAMERA_MODES_CONSTRUCT(address)                                              \
  ITC_NATIVE_ENTRY_ALWAYS(address) {                                                   \
    \
    if (g_construct_stand_in) {                                                          \
      ctx.r3.u64 = g_construct_stand_in(0x##address, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, \
                                        ctx.r1.u32, uint32_t(ctx.lr));                   \
      return;                                                                            \
    }                                                                                    \
    if (!ITC_FLAG(native_camera_modes)) {                                                \
      lifted::original::c##address(ctx);                                      \
      return;                                                                            \
    }                                                                                    \
    hooks::CameraModesStep##address(ctx);                                          \
  }

namespace {

// memcpy (sub_825A7410) through the translated code, for a copy
// c_runtime::Memcpy refuses: from a frame like the original's (`frame`
// bytes) with its return address. r3 = the destination, as memcpy leaves it.
void GuestMemcpy(lifted::Regs& ctx, uint32_t frame, uint32_t return_address,
                 uint32_t destination, uint32_t source, uint32_t size) {
  guest_call::InFrame(ctx, frame, return_address, [&](uint32_t) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = source;
    ctx.r5.u64 = size;
    lifted::Enter(ctx, game::memcpy_825A7410);
  });
}

// XMemCpy (sub_82295890) through the translated code, for a copy
// c_runtime::XMemCpy refuses, as GuestMemcpy.
void GuestXMemCpy(lifted::Regs& ctx, uint32_t frame, uint32_t return_address,
                  uint32_t destination, uint32_t source, uint32_t size) {
  guest_call::InFrame(ctx, frame, return_address, [&](uint32_t) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = source;
    ctx.r5.u64 = size;
    lifted::Enter(ctx, game::XMemCpy);
  });
}

// A guest call from the frame at `frame` (the calling original's r1,
// `frame_bytes` below its caller's) with the return address `back`.
template <typename Call>
void CallFrom(lifted::Regs& ctx, uint32_t frame, uint32_t frame_bytes,
              uint32_t back, Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + frame_bytes;
  guest_call::InFrame(ctx, frame_bytes, back, [&](uint32_t) { call(); });
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
}

// CameraPath::Request (r3 the TV camera, r4 the buffer, r5 the index)
// through its step from a TV camera function's frame at `frame`
// (`frame_bytes`) with the return address `back`.
void PathRequestFrom(lifted::Regs& ctx, uint32_t frame, uint32_t frame_bytes,
                     uint32_t back, uint32_t camera, uint32_t buffer, uint32_t index) {
  CallFrom(ctx, frame, frame_bytes, back, [&] {
    ctx.r3.u64 = camera;
    ctx.r4.u64 = buffer;
    ctx.r5.u64 = index;
    hooks::CameraModesStep82152E08(ctx);
  });
}

}  // namespace

namespace hooks {

// --- The director's handlers: r3 = the director ---

// ToView4: r3 = the last racer record when the split screen ended (its
// RacerRecords::At), else the director.
ITC_REGS_STEP(CameraModesStep82152400) {
  ctx.r3.u64 = cm::ToView4(ctx.r3.u32);
}
ITC_REGS_STEP(CameraModesStep821524B8) {
  cm::ToView5(ctx.r3.u32);
}
ITC_REGS_STEP(CameraModesStep821524D8) {
  cm::ToView3();
}
ITC_REGS_STEP(CameraModesStep82153160) {
  cm::ToView3IfCourse(ctx.r3.u32);
}

// --- Small helpers ---

// Viewports::ResetRoll: r3 = viewport 1, as SetRoll leaves it.
ITC_REGS_STEP(CameraModesStep821524F0) {
  ctx.fpscr.disableFlushMode();
  cm::ResetRoll();
  ctx.r3.u64 = schema::CameraModesGlobals::viewports().second().address;
}

// Camera::ZoomFromFov: f1 = degrees, f2 = the width; f1 = the zoom. Where
// tan's native form can't stand in, the original runs.
ITC_REGS_STEP(CameraModesStep82152548) {
  ctx.fpscr.disableFlushMode();
  const auto zoom = cm::ZoomFromFov(ctx.f1.f64, ctx.f2.f64);
  if (!zoom) {
    lifted::original::c82152548(ctx);
    return;
  }
  ctx.f1.f64 = *zoom;
}

// Quat::Rotate (no adapter: only ChaseCamera::Follow's plain C++ calls it; the
// step kept for the harness): r3 = out, r4 = the quaternion, r5 = the vector.
ITC_REGS_STEP(CameraModesStep821525F8) {
  ctx.fpscr.disableFlushMode();
  cm::QuatRotate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Matrix::FromQuat (no adapter: only CameraTarget::Track's plain C++ calls it; the step kept
// for the harness).
// Matrix::FromQuat: r3 = the matrix, the quaternion in r4 and r5 (stored to
// the caller's r1 + 32 to + 47 as the original does; its vector load reads
// them back from there, so a stack pointer not 16-byte aligned runs the
// original).
ITC_REGS_STEP(CameraModesStep82152698) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    lifted::original::c82152698(ctx);
    return;
  }
  guest::StoreU32(ctx.r1.u32 + 32, uint32_t(ctx.r4.u64 >> 32));
  guest::StoreU32(ctx.r1.u32 + 36, ctx.r4.u32);
  guest::StoreU32(ctx.r1.u32 + 40, uint32_t(ctx.r5.u64 >> 32));
  guest::StoreU32(ctx.r1.u32 + 44, ctx.r5.u32);
  const uint32_t q[4] = {uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32, uint32_t(ctx.r5.u64 >> 32),
                         ctx.r5.u32};
  ctx.fpscr.disableFlushMode();
  cm::MatrixFromQuat(ctx.r3.u32, q, (ctx.r1.u32 - 16) & ~0xFu);
}

// Matrix::FromFloats (no adapter: only TrackRoute::SplinePoint's and the
// static matrices' plain C++ call it in play; the step kept for the
// harness): r3 = the matrix, r4 to r10 the first seven pointers, the other
// nine on the caller's stack (r1 + 84, + 92, ... + 148).
ITC_REGS_STEP(CameraModesStep82090000) {
  uint32_t from[16] = {ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                       ctx.r8.u32, ctx.r9.u32, ctx.r10.u32};
  for (uint32_t i = 7; i < 16; i++) {
    from[i] = guest::LoadU32(ctx.r1.u32 + 84 + 8 * (i - 7));
  }
  cm::MatrixFromFloats(ctx.r3.u32, from);
  // (r11 = the last pointer was a leftover for recompiled callers: none now)
}

// --- The lists: r3 = the list (left, or as memcpy leaves it) ---

// List40::Link (no adapter: only List40::Insert's plain C++ calls it): r3 =
// the list, r4 = the position; the step kept for the harness.
ITC_REGS_STEP(CameraModesStep82153228) {
  cm::List40Link(ctx.r3.u32, int16_t(ctx.r4.u16));
}
// List40::Insert: r4 = &the word, r5 = the position.
ITC_REGS_STEP(CameraModesStep821544E0) {
  cm::List40Insert(ctx.r3.u32, guest::LoadU32(ctx.r4.u32), ctx.r5.u32);
}
// List16::Insert20 (no adapter: only IntroCamera::IntroCamera's plain C++ calls it; the
// step kept for the harness).
// List16::Insert20: r4 = the record, r5 = the position.
ITC_REGS_STEP(CameraModesStep82154458) {
  cm::List16Insert20(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// List16::Insert32 and Insert608: r4 = the record, r5 = the position; r3 =
// the record's slot (memcpy's).
ITC_REGS_STEP(CameraModesStep82154378) {
  uint32_t to = 0;
  if (!cm::List16Insert(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, 32, &to)) {
    GuestMemcpy(ctx, 112, 0x821543E0, to, ctx.r4.u32, 32);
  }
  ctx.r3.u64 = to;
}
ITC_REGS_STEP(CameraModesStep821543E8) {
  uint32_t to = 0;
  if (!cm::List16Insert(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, 608, &to)) {
    GuestMemcpy(ctx, 112, 0x82154450, to, ctx.r4.u32, 608);
  }
  ctx.r3.u64 = to;
}
// List16::Copy32 (no adapter: only CutsceneShot::Init's plain C++ calls it; the step kept
// for the harness).
// List16::Copy32: r3 = out (returned), r4 = the list, r5 = the position.
ITC_REGS_STEP(CameraModesStep82154300) {
  const uint32_t out = ctx.r3.u32;
  uint32_t from = 0;
  if (!cm::List16Copy32(out, ctx.r4.u32, ctx.r5.u32, &from)) {
    GuestMemcpy(ctx, 96, 0x8215435C, out, from, 32);
  }
  ctx.r3.u64 = out;
}

// --- The camera animations' table ---

ITC_REGS_STEP(CameraModesStep82150078) {
  cm::AnimsClear();
}
// CameraAnims::Find: r3 = the asset id; r3 = its objects or 0.
ITC_REGS_STEP(CameraModesStep82150300) {
  ctx.r3.u64 = cm::AnimsFind(ctx.r3.u32);
}

// --- The color filter and the fade cues ---

// ColorFilter::SetStyle (no adapter: only TvCamera::Update's plain C++ calls
// it; the step kept for the harness): r3 = &the style. The original runs where the
// filter's slots aren't its own functions.
ITC_REGS_STEP(CameraModesStep82152108) {
  ctx.fpscr.disableFlushMode();
  if (!cm::ColorFilterSetStyle(ctx.r3.u32)) {
    lifted::original::c82152108(ctx);
  }
}

// FadeCue::Step (no adapter: only CutsceneShot::Step's plain C++ calls it; the step kept
// for the harness).
// FadeCue::Step: r3 = the cue (its caller, CutsceneShot::Step, doesn't read
// r3 after it).
ITC_REGS_STEP(CameraModesStep82152290) {
  cm::FadeCueStep(ctx.r3.u32);
}

// --- The camera animations' files ---

// CameraAnims::Load: r3 = the asset id; r3 = the serial (as Archive::Load
// leaves it). The original without --native_heap or the archive's queue.
ITC_REGS_STEP(CameraModesStep82150B10) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    lifted::original::c82150B10(ctx);
    return;
  }
  hooks::SetArchiveSerial(ctx, cm::AnimsLoad(ctx.r3.u32, hooks::NativeArchiveLoad()));
}

// CameraAnims::OnLoaded (the archive's callback): r4 = the data, r6 = the
// record; r3 = VirtualFree's 1. Its frame (672 bytes) is the original's; the
// copies the native layer refuses go through the guest from it (memcpy's
// return addresses 0x821508F4 and 0x8215092C; XMemCpy from ParseObject's
// frame below it, 0x821502B8).
ITC_REGS_STEP(CameraModesStep82150878) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c82150878(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 672;
  const bool done = cm::AnimsOnLoaded(ctx.r4.u32, ctx.r6.u32, frame,
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestMemcpy(ctx, 672, to == frame + 84 ? 0x821508F4 : 0x8215092C, to, from, size);
      },
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestXMemCpy(ctx, 672 + 144, 0x821502B8, to, from, size);
      });
  if (!done) {
    lifted::original::c82150878(ctx);
    return;
  }
  ctx.r3.u64 = 1;
}

// CameraAnims::FreeAll: no arguments; r3 as the original leaves it (1, its
// memcpy's frame + 80, or unchanged).
ITC_REGS_STEP(CameraModesStep821507D8) {
  if (!hooks::NativeHeapActive() || !cm::AnimsFreeable()) {
    lifted::original::c821507D8(ctx);
    return;
  }
  if (const auto r3 = cm::AnimsFreeAll(ctx.r1.u32 - 384 + 80)) {
    ctx.r3.u64 = *r3;
  }
}

// CameraAnims::Evaluate: r3 = out, r4 = the id, r5 = the frame; r3 = 1 or
// 0, f1 the last EvalCamera's. EvalCamera's original runs from a frame like
// Evaluate's (144 bytes, returning to 0x821509C4) where its native form
// can't.
ITC_REGS_STEP(CameraModesStep82150960) {
  ctx.fpscr.disableFlushMode();
  const uint32_t out = ctx.r3.u32, time = ctx.r5.u32;
  std::optional<double> f1;
  const uint32_t r3 = cm::AnimsEvaluate(out, ctx.r4.u32, time,
      [&](uint32_t object) {
        guest_call::InFrame(ctx, 144, 0x821509C4, [&](uint32_t) {
          ctx.r3.u64 = object;
          ctx.r4.u64 = object;
          ctx.r5.u64 = out;
          ctx.r6.u64 = time;
          lifted::original::c82150350(ctx);
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
ITC_REGS_STEP(CameraModesStep82150A98) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  ctx.f1.f64 = cm::AnimsDuration(ctx.r3.u32, &r3);
  ctx.r3.u64 = r3;
}

// CameraAnims::ObjectMatrix: r3 = out, r4 = the id, r5 = the frame; r3 = 1
// or 0. CameraAnim::Matrix's frame is the original's (r1 - 96 - 320), so a
// stack pointer not 16-byte aligned runs the original.
ITC_REGS_STEP(CameraModesStep82150A10) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    lifted::original::c82150A10(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::AnimsObjectMatrix(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 96 - 320,
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestMemcpy(ctx, 96 + 320, 0x82150600, to, from, size);
      });
}

// --- The camera paths ---

// CameraPath::Request: r4 = the path buffer, r5 = the index; r3 = the
// serial when a load was queued, else unchanged. The original without
// --native_heap, the archive's queue, or a block the layer didn't give.
ITC_REGS_STEP(CameraModesStep82152E08) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue() ||
      !game_modes::Buffer28Freeable(ctx.r4.u32)) {
    lifted::original::c82152E08(ctx);
    return;
  }
  if (const auto serial =
          cm::PathRequest(ctx.r4.u32, ctx.r5.u32, hooks::NativeArchiveLoad())) {
    hooks::SetArchiveSerial(ctx, *serial);
  }
}

// CameraPath::OnLoaded (the archive's callback): r4 = the data, r6 = the
// buffer.
ITC_REGS_STEP(CameraModesStep82152078) {
  cm::PathOnLoaded(ctx.r4.u32, ctx.r6.u32);
}

// --- The rival logo ---

// RivalLogo::RivalLogo: r3 = the entry (its caller sets r3 again after).
ITC_REGS_STEP(CameraModesStep82211848) {
  cm::RivalLogoConstruct(ctx.r3.u32);
}

// RivalLogo::Play: r3 = the entry; r3 as the original leaves it. With
// --log_pause_movie the original runs (PlayNamed's override logs it).
ITC_REGS_STEP(CameraModesStep82211AB0) {
  if (ITC_FLAG(log_pause_movie)) {
    lifted::original::c82211AB0(ctx);
    return;
  }
  ctx.r3.u64 = cm::RivalLogoPlay(ctx.r3.u32, ctx.r1.u32 - 144 + 80);
}

// --- The views ---

// CameraTarget::Track: r3 = the tracker, r4 = the vehicle; r3 = tracker +
// 224 (TrackPosition::Copy's) when it tracked. Its frame (256 bytes) and
// Matrix::FromQuat's scratch below it are the original's, so a stack
// pointer not 16-byte aligned runs the original.
ITC_REGS_STEP(CameraModesStep82153290) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    lifted::original::c82153290(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t tracker = ctx.r3.u32, vehicle = ctx.r4.u32;
  cm::TargetTrack(tracker, vehicle, ctx.r1.u32 - 256);
  if (vehicle != 0) {
    ctx.r3.u64 = schema::CameraTarget(tracker).track_position_address();
  }
}

// Camera::Shake (no adapter: only ChaseCamera::Update's plain C++ calls it; the
// step kept for the harness): r3 = the camera; r3 = its viewport, as Offset
// leaves it.
ITC_REGS_STEP(CameraModesStep82152CD8) {
  ctx.fpscr.disableFlushMode();
  const uint32_t camera = ctx.r3.u32;
  cm::Shake(camera, ctx.r1.u32 - 96);
  ctx.r3.u64 = schema::ChaseCamera(camera).viewport().address;
}

// Viewport::PullBack (no adapter: only ChaseCamera::Update's plain C++ calls
// it; the step kept for the harness): r3 = the viewport, f1 = the amount, r5 =
// the offset.
ITC_REGS_STEP(CameraModesStep82152788) {
  ctx.fpscr.disableFlushMode();
  cm::ViewportPullBack(ctx.r3.u32, ctx.f1.f64, ctx.r5.u32, ctx.r1.u32);
}

// IntroCamera::StepShot (no adapter: only IntroCamera::Update's plain C++
// calls it; the step kept for the harness): r3 = the shot; r3 = 0 or 1.
ITC_REGS_STEP(CameraModesStep82152E78) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::IntroStepShot(ctx.r3.u32, ctx.r1.u32 - 272);
}

}  // namespace hooks

namespace {

// atexit for IntroCamera::MakeShot where the native one can't: the game's,
// from MakeShot's frame (`sp`) with its return address.
struct MakeShotAtexit {
  lifted::Regs& ctx;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&MakeShotAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<MakeShotAtexit*>(self);
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = function == 0x825DB150 ? 0x82154214 : 0x8215425C;
    call.ctx.r3.u64 = function;
    lifted::original::c825A7068(call.ctx);
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
ITC_REGS_STEP(CameraModesStep82158F70) {
  if (!hooks::NativeAtexit()) {
    lifted::original::c82158F70(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 368;
  MakeShotAtexit atexit{ctx, frame - 192};
  ctx.r3.u64 = cm::IntroConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
      [&](uint32_t out, uint32_t stages, uint32_t stage) {
        guest_call::InFrame(ctx, 368, 0x82159078, [&](uint32_t) {
          ctx.r3.u64 = out;
          ctx.r4.u64 = stages;
          ctx.r5.u64 = stage;
          lifted::original::c820FFD88(ctx);
        });
        ctx.fpscr.disableFlushMode();
      },
      atexit.Call());
}

// CameraDirector::Start (a handler): r3 = the director. Viewports::Init
// natively (with --native_heap and native atexit) from a frame like the
// original's (144 bytes), else its original.
ITC_REGS_STEP(CameraModesStep82152338) {
  ctx.fpscr.disableFlushMode();
  cm::DirectorStart(ctx.r3.u32, [&] {
    guest_call::InFrame(ctx, 144, 0x82152358, [&](uint32_t) {
      std::optional<uint32_t> viewport;
      if (hooks::NativeHeapActive() && hooks::NativeAtexit()) {
        viewport = hooks::ViewportsInit(ctx);
      }
      if (viewport) {
        ctx.r3.u64 = *viewport;
      } else {
        lifted::original::c820AB358(ctx);
      }
    });
    ctx.fpscr.disableFlushMode();
  });
}

// CameraDirector::Release (a handler): r3 = the director; r3 as
// Viewports::Release leaves it. CameraAnims::FreeAll and Viewports::Release
// natively where their adapters would (--native_heap, the layer's pages),
// else their originals from a frame like the original's (96 bytes).
ITC_REGS_STEP(CameraModesStep82153098) {
  ctx.fpscr.disableFlushMode();
  cm::DirectorRelease(ctx.r3.u32,
      [&] {
        if (hooks::NativeHeapActive() && cm::AnimsFreeable()) {
          if (const auto r3 = cm::AnimsFreeAll(ctx.r1.u32 - 96 - 384 + 80)) {
            ctx.r3.u64 = *r3;
          }
          return;
        }
        guest_call::InFrame(ctx, 96, 0x821530B0,
                            [&](uint32_t) { lifted::original::c821507D8(ctx); });
        ctx.fpscr.disableFlushMode();
      },
      [&] {
        if (hooks::NativeHeapActive() && cameras::ReleaseViewports()) {
          ctx.r3.u64 = 1;
          return;
        }
        guest_call::InFrame(ctx, 96, 0x82153138,
                            [&](uint32_t) { lifted::original::c820AA430(ctx); });
        ctx.fpscr.disableFlushMode();
      });
}

}  // namespace hooks

namespace {

// CutsceneShot::Init's guest calls, from frames below the stack pointer the
// calls are made with (the caller's r1 at Init's call): the copies the
// native layer refuses through the guest's memcpy from Copy32's and
// Insert32's frames below Init's (192 bytes); CameraAnims::Load natively
// where its adapter would (--native_heap, the archive's queue), else its
// original.
cm::CutsceneCalls ShotInitCalls(lifted::Regs& ctx) {
  cm::CutsceneCalls calls;
  calls.copy32 = [&ctx](uint32_t to, uint32_t from, uint32_t size) {
    guest_call::InFrame(ctx, 192, 0x82156C90, [&](uint32_t) {
      GuestMemcpy(ctx, 96, 0x8215435C, to, from, size);
    });
  };
  calls.insert32 = [&ctx](uint32_t to, uint32_t from, uint32_t size) {
    guest_call::InFrame(ctx, 192, 0x82156CA0, [&](uint32_t) {
      GuestMemcpy(ctx, 112, 0x821543E0, to, from, size);
    });
  };
  calls.load = [&ctx](uint32_t id) {
    if (hooks::NativeHeapActive() && hooks::NativeArchiveQueue()) {
      hooks::SetArchiveSerial(ctx, cm::AnimsLoad(id, hooks::NativeArchiveLoad()));
      return;
    }
    guest_call::InFrame(ctx, 192, 0x82156CB8, [&](uint32_t) {
      ctx.r3.u64 = id;
      lifted::original::c82150B10(ctx);
    });
  };
  return calls;
}

}  // namespace

namespace hooks {

// CutsceneShot::Init: r3 = the shot, r4 = the script entry; r3 as
// CameraAnims::Load leaves it (the serial). Its calls: ShotInitCalls.
ITC_REGS_STEP(CameraModesStep82156B68) {
  ctx.fpscr.disableFlushMode();
  cm::CutsceneInit(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192, ShotInitCalls(ctx));
}

}  // namespace hooks

namespace {

// CameraAnim::EvalCamera's original for CutsceneShot::Step (`shot`, its
// frame at `frame`: the original's r1, 304 bytes) where its native form
// can't: from frames like Step's and Evaluate's (144 bytes). f1 as it
// leaves it.
void ShotCameraOriginal(lifted::Regs& ctx, uint32_t shot, uint32_t frame,
                        uint32_t object) {
  const uint32_t time = schema::CutsceneShot(shot).time();
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame + 304;
  guest_call::InFrame(ctx, 304, 0x82155834, [&](uint32_t) {
    guest_call::InFrame(ctx, 144, 0x821509C4, [&](uint32_t) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = object;
      ctx.r5.u64 = frame + 144;
      ctx.r6.u64 = time;
      lifted::original::c82150350(ctx);
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
ITC_REGS_STEP(CameraModesStep82155800) {
  ctx.fpscr.disableFlushMode();
  const uint32_t shot = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 304;
  double f1 = ctx.f1.f64;
  const uint32_t r3 = cm::CutsceneStep(shot, ctx.r4.u32, frame,
      [&](uint32_t object) {
        ShotCameraOriginal(ctx, shot, frame, object);
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
  lifted::Regs& ctx;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&SplineAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<SplineAtexit*>(self);
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = 0x820ABB20;
    call.ctx.r3.u64 = function;
    lifted::original::c825A7068(call.ctx);
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
ITC_REGS_STEP(CameraModesStep820FE830) {
  ctx.r3.s64 = cm::RangeCameraAt(ctx.r3.u32, ctx.r4.u32);
}

// TrackRoute::SliceFraction (no adapter: only TvCamera::Step's plain C++
// calls it; the step kept for the harness): r3 = &the position, r4 = the point; f1 the
// fraction. Its caller doesn't read r3.
ITC_REGS_STEP(CameraModesStep820F9B48) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = cm::SliceFraction(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

// TvCamera::Track: r3 = the TV camera, r4 = &the position (its caller
// doesn't read r3 after). CameraPath::Request through its step (which runs
// its original where it can't) from a frame like the original's (496
// bytes).
ITC_REGS_STEP(CameraModesStep82156980) {
  ctx.fpscr.disableFlushMode();
  const uint32_t camera = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 496;
  cm::TvTrack(camera, ctx.r4.u32, frame, [&](uint32_t buffer, uint32_t index) {
    PathRequestFrom(ctx, frame, 496, 0x82156B54, camera, buffer, index);
  });
}

// CameraPath::Evaluate (no adapter: only TvCamera::Step's plain C++ calls it;
// the step kept for the harness): r3 = the key, r4 = the camera's out, r5 = the
// route range, r6 = the transform, f1 = t (its caller reads neither r3 nor
// f1 after). The original without native atexit (TrackRoute::SplinePoint's
// static); the atexit's fallback from SplinePoint's frame (272 + 208 + 720
// bytes below).
ITC_REGS_STEP(CameraModesStep820AC248) {
  if (!hooks::NativeAtexit()) {
    lifted::original::c820AC248(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  SplineAtexit atexit{ctx, ctx.r1.u32 - 272 - 208 - 720};
  cm::PathEvaluate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64,
                   ctx.r1.u32, atexit.Call());
}

}  // namespace hooks


// --- The views' cameras ---

namespace {

// The video output from a native original's frame (`frame`, its r1): its
// two slot calls (`[0x8266D088]`'s slot 7, then that object's slot 3, as
// Viewport::Init's adapter makes them) with the original's return
// addresses. Returns the video output.
uint32_t VideoOutputFrom(lifted::Regs& ctx, uint32_t frame, uint32_t back) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.r3.u64 = schema::AppStartUpGlobals::scene_target_app().address;
  ctx.lr = back;
  // Slot 7's plain C++ where it's the game's (hooks::DirectSlotCall).
  const uint32_t slot7 = guest::LoadU32(guest::LoadU32(ctx.r3.u32) + 28);
  if (!hooks::DirectSlotCall(ctx, slot7)) {
    lifted::CallFromContext(ctx, slot7);
  }
  ctx.ctr.u64 = guest::LoadU32(guest::LoadU32(ctx.r3.u32) + 12);
  ctx.lr = back + 16;
  lifted::CallFromContext(ctx, ctx.ctr.u32);
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u32;
}

cm::VideoOutputCall VideoOutput(lifted::Regs& ctx) {
  return [&ctx](uint32_t frame, uint32_t back) {
    return VideoOutputFrom(ctx, frame, back);
  };
}

// ChaseCamera::ChaseCamera's atexit through the translated code when the
// native one can't: from its frame (`sp`) with its return addresses.
struct ChaseAtexit {
  lifted::Regs& ctx;
  uint32_t sp;
  c_runtime::AtexitCall Call() { return {&ChaseAtexit::Run, this}; }
  static int32_t Run(void* self, uint32_t function) {
    auto& call = *static_cast<ChaseAtexit*>(self);
    const uint64_t stack = call.ctx.r1.u64;
    call.ctx.r1.u64 = call.sp;
    call.ctx.lr = function == 0x825DB100 ? 0x821546D4 : 0x8215471C;
    call.ctx.r3.u64 = function;
    lifted::original::c825A7068(call.ctx);
    call.ctx.r1.u64 = stack;
    call.ctx.fpscr.disableFlushMode();
    return call.ctx.r3.s32;
  }
};

// The views' constructors' atexit through the translated code when the
// native one can't: from the constructor's frame (`sp`), the return address
// by the function registered.
struct ViewAtexit {
  lifted::Regs& ctx;
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
    lifted::original::c825A7068(call.ctx);
    call.ctx.r1.u64 = stack;
    call.ctx.fpscr.disableFlushMode();
    return call.ctx.r3.s32;
  }
};

// Whether the chase camera's plain C++ can stand in: a 16-byte aligned
// stack pointer (CameraTarget::Track's and the vector loads' frames) and
// ZoomFromFov's tan.
bool ChaseNative(lifted::Regs& ctx) {
  return (ctx.r1.u32 & 0xF) == 0 && cm::ChaseReady();
}

}  // namespace

namespace hooks {

// ChaseCamera::Place (no adapter: only ChaseCamera::ChaseCamera's plain C++
// calls it; the step kept for the harness): r3 = the camera, r4 to r7 the
// four vectors. A stack pointer not 16-byte aligned runs the original.
ITC_REGS_STEP(CameraModesStep82153478) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    lifted::original::c82153478(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::ChasePlace(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                 ctx.r1.u32 - 112);
}

// ChaseCamera::Follow (no adapter: only ChaseCamera::ChaseCamera's and
// Update's plain C++ call it; the step kept for the harness): r3 = the
// camera, f1 = the rate, f2 = the least. The original where ChaseNative
// says no.
ITC_REGS_STEP(CameraModesStep82153640) {
  if (!ChaseNative(ctx)) {
    lifted::original::c82153640(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::ChaseFollow(ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r1.u32 - 320,
                  VideoOutput(ctx));
}

// ChaseCamera::ChaseCamera: r3 = the camera, r4 = the viewport, r5 = the
// vehicle; r3 = the camera. The original where ChaseNative says no or
// without native atexit.
ITC_REGS_STEP(CameraModesStep82154548) {
  if (!ChaseNative(ctx) || !hooks::NativeAtexit()) {
    lifted::original::c82154548(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 224;
  ChaseAtexit atexit{ctx, frame};
  ctx.r3.u64 = cm::ChaseConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                                  VideoOutput(ctx), atexit.Call());
}

// ChaseCamera::Update (vtable slot 0; its caller reads neither r3 nor f1
// after): r3 = the camera. The original where ChaseNative says no.
ITC_REGS_STEP(CameraModesStep82154808) {
  if (!ChaseNative(ctx)) {
    lifted::original::c82154808(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::ChaseUpdate(ctx.r3.u32, ctx.r1.u32 - 144, VideoOutput(ctx));
}

// TvCamera::TvCamera: r3 = the camera, r4 = the viewport, r5 = the vehicle;
// r3 = the camera. The original where ChaseNative says no or without native
// atexit (the fallback chase camera's statics).
ITC_REGS_STEP(CameraModesStep82155288) {
  if (!ChaseNative(ctx) || !hooks::NativeAtexit()) {
    lifted::original::c82155288(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 160;
  ChaseAtexit atexit{ctx, frame - 224};
  ctx.r3.u64 = cm::TvConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                               VideoOutput(ctx), atexit.Call());
}

}  // namespace hooks


namespace hooks {

// ViewCamera1::ViewCamera1 and ViewCamera2::ViewCamera2: r3 = the camera,
// r4 = the viewport, r5 = the vehicle; r3 = the camera. The original where
// ChaseNative says no or without native atexit.
ITC_REGS_STEP(CameraModesStep82154A48) {
  if (!ChaseNative(ctx) || !hooks::NativeAtexit()) {
    lifted::original::c82154A48(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 192;
  ViewAtexit atexit{ctx, frame};
  ctx.r3.u64 = cm::View1Construct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                                  VideoOutput(ctx), atexit.Call());
}
ITC_REGS_STEP(CameraModesStep82154D38) {
  if (!ChaseNative(ctx) || !hooks::NativeAtexit()) {
    lifted::original::c82154D38(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 192;
  ViewAtexit atexit{ctx, frame};
  ctx.r3.u64 = cm::View2Construct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame,
                                  VideoOutput(ctx), atexit.Call());
}

// ViewCamera1::Update and ViewCamera2::Update (vtable slot 0; their caller
// reads neither r3 nor f1 after): r3 = the camera. The original where
// ChaseNative says no.
ITC_REGS_STEP(CameraModesStep82153A50) {
  if (!ChaseNative(ctx)) {
    lifted::original::c82153A50(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::View1Update(ctx.r3.u32, ctx.r1.u32 - 352, VideoOutput(ctx));
}
ITC_REGS_STEP(CameraModesStep82153E58) {
  if (!ChaseNative(ctx)) {
    lifted::original::c82153E58(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::View2Update(ctx.r3.u32, ctx.r1.u32 - 240, VideoOutput(ctx));
}

}  // namespace hooks

namespace hooks {

// IntroCamera::Update (vtable slot 0; its caller reads neither r3 nor f1
// after): r3 = the camera. A stack pointer not 16-byte aligned runs the
// original (CameraTarget::Track's frame).
ITC_REGS_STEP(CameraModesStep82155E58) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    lifted::original::c82155E58(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::IntroUpdate(ctx.r3.u32, ctx.r1.u32 - 320);
}

}  // namespace hooks

// --- The adapters ---

// Viewports::ResetRoll sub_821524F0, Camera::ZoomFromFov sub_82152548,
// CameraTarget::Track sub_82153290, List40::Insert sub_821544E0,
// CameraPath::Request sub_82152E08, CutsceneShot::Init sub_82156B68,
// List16::Insert32 sub_82154378, Insert608 sub_821543E8,
// RivalLogo::RivalLogo sub_82211848 and RivalLogo::Open sub_822118E0 have
// no adapters (2026-10-30): their callers in play are native and call
// their plain C++; --native_camera_modes switches those callers. Their
// steps stay for the harness. Neither have CameraAnims::Find sub_82150300,
// Evaluate sub_82150960 and ObjectMatrix sub_82150A10 (B7 loop,
// 2026-11-12): every caller in play is native once LocationScreen::PollLoad
// is (their plain C++, the originals on its fallbacks).
ITC_CAMERA_MODES(82152400)
ITC_CAMERA_MODES(821524B8)
ITC_CAMERA_MODES(821524D8)
ITC_CAMERA_MODES(82153160)
ITC_CAMERA_MODES(82150078)
ITC_CAMERA_MODES(82150B10)
ITC_CAMERA_MODES(82150878)
ITC_CAMERA_MODES(821507D8)
ITC_CAMERA_MODES(82152078)
ITC_CAMERA_MODES_CONSTRUCT(82158F70)
ITC_CAMERA_MODES(82152338)
ITC_CAMERA_MODES(82153098)
ITC_CAMERA_MODES(82156980)
ITC_CAMERA_MODES_CONSTRUCT(82154548)
ITC_CAMERA_MODES(82154808)
ITC_CAMERA_MODES_CONSTRUCT(82154A48)
ITC_CAMERA_MODES(82153A50)
ITC_CAMERA_MODES_CONSTRUCT(82154D38)
ITC_CAMERA_MODES(82153E58)
ITC_CAMERA_MODES(82155E58)
ITC_CAMERA_MODES_CONSTRUCT(82155288)

// --- The rival logo's open and close ---

namespace {

// RivalLogo::Open's calls from its frame at `sp` (the original's r1, 112
// bytes): UiMovies::Open natively where its adapter would, else its
// adapter, and RivalLogo::LoadXpr (never run in play) through the
// translated code, from frames like the original's.
struct RivalLogoOpenCalls {
  std::function<uint32_t(uint32_t, uint32_t)> open;
  std::function<uint32_t(uint32_t, uint32_t)> load_xpr;
};
RivalLogoOpenCalls RivalLogoOpenCallsAt(lifted::Regs& ctx, uint32_t sp) {
  RivalLogoOpenCalls calls;
  calls.open = [&ctx, sp](uint32_t id, uint32_t asset) -> uint32_t {
    if (ui_movies_game_hooks::OpenNative()) {
      return ui_movies::OpenMovie(id, asset, sp - 128, hooks::NativeArchiveLoad(),
                                  ui_movies_game_hooks::OpenCalls(ctx, sp));
    }
    CallFrom(ctx, sp, 112, 0x8221195C, [&] {
      ctx.r3.u64 = id;
      ctx.r4.u64 = asset;
      lifted::Enter(ctx, game::UiMovies::Open);
    });
    return ctx.r3.u32;
  };
  calls.load_xpr = [&ctx, sp](uint32_t holder, uint32_t id) -> uint32_t {
    CallFrom(ctx, sp, 112, 0x82211978, [&] {
      ctx.r3.u64 = holder;
      ctx.r4.u64 = id;
      lifted::Enter(ctx, lifted::e82211878);
    });
    return ctx.r3.u32;
  };
  return calls;
}

}  // namespace

namespace hooks {

// RivalLogo::Open: r3 = the entry, r4 = the logo, r5 = the texture; r3 as
// the original leaves it. Its calls: RivalLogoOpenCallsAt.
ITC_REGS_STEP(CameraModesStep822118E0) {
  const RivalLogoOpenCalls calls = RivalLogoOpenCallsAt(ctx, ctx.r1.u32 - 112);
  ctx.r3.u64 = cm::RivalLogoOpen(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, calls.open,
                                 calls.load_xpr);
}

}  // namespace hooks

namespace {

// RivalLogo::Close's calls from its frame at `sp` (the original's r1, 128
// bytes): UiMovie::FinishAll through its adapter and RivalLogo::ReleaseXpr
// (never run in play) through the translated code, UiMovies::Delete
// natively where its adapter would, else its adapter, from frames like the
// original's.
cm::RivalLogoCloseCalls RivalLogoCloseCallsAt(lifted::Regs& ctx, uint32_t sp) {
  auto call = [&ctx, sp](uint32_t back, uint32_t function, uint32_t r3) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp + 128;
    guest_call::InFrame(ctx, 128, back, [&](uint32_t) {
      ctx.r3.u64 = r3;
      lifted::CallFromContext(ctx, function);
    });
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
  cm::RivalLogoCloseCalls calls;
  calls.finish_all = [call](uint32_t movie) { call(0x822119C0, 0x8211DFF8, movie); };
  calls.release_xpr = [call](uint32_t holder) { call(0x822119F8, 0x82211768, holder); };
  calls.remove = [&ctx, sp, call](uint32_t id) -> uint32_t {
    if (ui_movies_game_hooks::DeleteNative()) {
      return ui_movies::DeleteMovie(id, sp - 128,
                                    ui_movies_game_hooks::DeleteCalls(ctx, sp));
    }
    return call(0x82211A00, 0x8211E258, id);
  };
  return calls;
}

// Cutscene::Step's calls from its frame at `frame` (the original's r1, 192
// bytes): CutsceneShot::Step's frame 304 bytes below it, RivalLogo::Close's
// 128; RivalLogo::Play's original with --log_pause_movie (as its adapter);
// RivalLogo::Draw through its override (sub_82211A28, which frame
// interpolation's --redraw_rival_logo notes) with Step's return address.
cm::CutsceneTickCalls CutsceneTickCallsAt(lifted::Regs& ctx, uint32_t frame) {
  cm::CutsceneTickCalls calls;
  calls.camera_original = [&ctx, frame](uint32_t shot, uint32_t object) {
    ShotCameraOriginal(ctx, shot, frame - 304, object);
  };
  calls.close = RivalLogoCloseCallsAt(ctx, frame - 128);
  if (ITC_FLAG(log_pause_movie)) {
    calls.play_original = [&ctx, frame](uint32_t entry) {
      CallFrom(ctx, frame, 192, 0x82155A64, [&] {
        ctx.r3.u64 = entry;
        lifted::original::c82211AB0(ctx);
      });
    };
  }
  calls.draw = [&ctx, frame](uint32_t entry) {
    CallFrom(ctx, frame, 192, 0x82155BE0, [&] {
      ctx.r3.u64 = entry;
      lifted::Enter(ctx, game::RivalLogo::Draw);
    });
  };
  return calls;
}

}  // namespace

namespace hooks {

// RivalLogo::Close: r3 = the entry; r3 as the original leaves it.
ITC_REGS_STEP(CameraModesStep82211980) {
  const uint32_t sp = ctx.r1.u32 - 128;
  ctx.r3.u64 = cm::RivalLogoClose(ctx.r3.u32, sp, RivalLogoCloseCallsAt(ctx, sp));
}

// Cutscene::Step: r3 = the cutscene; r3 = 1 while running, 0 at its last
// shot; lr put back as the original's epilogue does (its only caller,
// Cutscene::Update, reads neither f1 nor the stack after).
ITC_REGS_STEP(CameraModesStep82155A28) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 192;
  const uint32_t cutscene = ctx.r3.u32;
  ctx.r3.u64 = cm::CutsceneTick(cutscene, frame, CutsceneTickCallsAt(ctx, frame));
  ctx.lr = lr;
}

// Cutscene::Update (vtable slot 0; CameraDirector::Update reads neither r3
// nor f1 after): r3 = the cutscene; r3 as the original leaves it, lr put
// back. Step's frame (192 bytes) below Update's (96).
ITC_REGS_STEP(CameraModesStep821561F8) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 96;
  const uint32_t cutscene = ctx.r3.u32;
  ctx.r3.u64 =
      cm::CutsceneUpdate(cutscene, frame, CutsceneTickCallsAt(ctx, frame - 192));
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES(821561F8)

// --- The TV camera's update ---

namespace {

// TvCamera::Step's guest calls from its frame at `frame` (720 bytes).
cm::TvStepCalls TvStepCallsAt(lifted::Regs& ctx, uint32_t frame,
                              uint32_t camera) {
  cm::TvStepCalls calls;
  calls.copy = [&ctx, frame](uint32_t to, uint32_t from, uint32_t size) {
    CallFrom(ctx, frame, 720, 0x8215634C, [&] {
      ctx.r3.u64 = to;
      ctx.r4.u64 = from;
      ctx.r5.u64 = size;
      lifted::Enter(ctx, game::memcpy_825A7410);
    });
  };
  calls.request = [&ctx, frame, camera](uint32_t buffer, uint32_t index, uint32_t back) {
    PathRequestFrom(ctx, frame, 720, back, camera, buffer, index);
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
ITC_REGS_STEP(CameraModesStep821562F8) {
  if ((ctx.r1.u32 & 0xF) != 0 || !hooks::NativeAtexit()) {
    lifted::original::c821562F8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 720;
  const uint32_t camera = ctx.r3.u32;
  SplineAtexit atexit{ctx, frame - 272 - 208 - 720};
  cm::TvStep(camera, frame, TvStepCallsAt(ctx, frame, camera), atexit.Call());
  ctx.lr = lr;
}

// TvCamera::Update (vtable slot 0; CameraDirector::Update reads neither r3
// nor f1 after): r3 = the TV camera; lr put back. The fallback chase
// camera's update natively (its frame 144 bytes below Update's 128), else
// its slot 0 through the translated code; Step's frame 720 bytes below.
// The original where ChaseNative says no or without native atexit.
ITC_REGS_STEP(CameraModesStep821593D8) {
  if (!ChaseNative(ctx) || !hooks::NativeAtexit()) {
    lifted::original::c821593D8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 128;
  const uint32_t camera = ctx.r3.u32;
  SplineAtexit atexit{ctx, frame - 720 - 272 - 208 - 720};
  cm::TvUpdateCalls calls;
  calls.step = TvStepCallsAt(ctx, frame - 720, camera);
  calls.video_output = VideoOutput(ctx);
  calls.slot0 = [&ctx, frame](uint32_t function, uint32_t chase) {
    CallFrom(ctx, frame, 128, 0x82159408, [&] {
      ctx.r3.u64 = chase;
      lifted::CallFromContext(ctx, function);
    });
  };
  calls.set_style_original = [&ctx, frame](uint32_t style) {
    CallFrom(ctx, frame, 128, 0x821594A0, [&] {
      ctx.r3.u64 = style;
      lifted::original::c82152108(ctx);
    });
  };
  cm::TvUpdate(camera, frame, calls, atexit.Call());
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES(821593D8)

// --- The director's state machine ---

namespace hooks {

// CameraDirector::InitStates: r3 = the director; r3 as the last
// StateMachine::Add leaves it (its adapter's: a new transition's address
// from the machine's 64-bit r3, src/hooks/game_modes.cpp).
ITC_REGS_STEP(CameraModesStep82159950) {
  ctx.fpscr.disableFlushMode();
  const uint32_t machine_offset = schema::CameraDirector(0).machine_address();
  const uint64_t machine_r3 = ctx.r3.u64 + machine_offset;
  const uint32_t machine = ctx.r3.u32 + machine_offset;
  bool replaced = false;
  const uint32_t r3 =
      cm::DirectorInitStates(ctx.r3.u32, ctx.r1.u32 - cm::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}


// SetMode's constructor calls from its frame at `frame` (the original's r1,
// 176 bytes): the view's camera object made through its adapter's step
// (which falls back to its original itself) with the call site's return
// address, or the harness's stand-in.
cm::SetModeCalls SetModeCallsAt(lifted::Regs& ctx, uint32_t frame) {
  cm::SetModeCalls calls;
  calls.construct = [&ctx, frame](uint32_t view, uint32_t object, uint32_t viewport,
                                        uint32_t vehicle, uint32_t back) -> uint32_t {
    static constexpr void (*kSteps[6])(lifted::Regs&) = {
        ITC_REGS_FN(CameraModesStep82154548), ITC_REGS_FN(CameraModesStep82154A48), ITC_REGS_FN(CameraModesStep82154D38),
        ITC_REGS_FN(CameraModesStep82158F70), ITC_REGS_FN(CameraModesStep82155288), ITC_REGS_FN(CameraModesStep82156CC0)};
    static constexpr uint32_t kFunctions[6] = {0x82154548, 0x82154A48, 0x82154D38,
                                               0x82158F70, 0x82155288, 0x82156CC0};
    if (g_construct_stand_in) {
      return g_construct_stand_in(kFunctions[view], object, viewport, vehicle, frame, back);
    }
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = object;
    ctx.r4.u64 = viewport;
    ctx.r5.u64 = vehicle;
    ctx.lr = back;
    kSteps[view](ctx);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  };
  return calls;
}

// SetMode natively from a handler's frame at `frame` (its 176 bytes below).
std::function<void(uint32_t)> SetModeFrom(lifted::Regs& ctx, uint32_t director,
                                          uint32_t frame) {
  return [&ctx, director, frame](uint32_t view) {
    cm::DirectorSetMode(director, view, frame - 176,
                        SetModeCallsAt(ctx, frame - 176));
  };
}

// Stages::Copy's original from a handler's frame (`frame_bytes`) with the
// return address `back`, where its plain C++ refuses.
cm::StagesCopyCall StagesCopyFrom(lifted::Regs& ctx, uint32_t frame,
                                  uint32_t frame_bytes, uint32_t back) {
  return [&ctx, frame, frame_bytes, back](uint32_t out, uint32_t stages, uint32_t stage) {
    CallFrom(ctx, frame, frame_bytes, back, [&] {
      ctx.r3.u64 = out;
      ctx.r4.u64 = stages;
      ctx.r5.u64 = int64_t(int32_t(stage));
      lifted::original::c820FFD88(ctx);
    });
  };
}

// CameraDirector::SetMode: r3 = the director, r4 = the view; r3 as the
// original leaves it; lr put back.
ITC_REGS_STEP(CameraModesStep821591F0) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 176;
  ctx.r3.u64 =
      cm::DirectorSetMode(ctx.r3.u32, ctx.r4.u32, frame, SetModeCallsAt(ctx, frame));
  ctx.lr = lr;
}

// CameraDirector::OnRaceStart (a handler; r3 not kept: the state machine's
// handler call doesn't read it): r3 = the director; lr put back. SetMode
// natively from its 224-byte frame.
ITC_REGS_STEP(CameraModesStep821596B0) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 224;
  const uint32_t director = ctx.r3.u32;
  cm::DirectorOnRaceStart(director, frame,
                          StagesCopyFrom(ctx, frame, 224, 0x82159730),
                          SetModeFrom(ctx, director, frame));
  ctx.lr = lr;
}

// CameraDirector::Resume (a handler; r3 and f1 not kept): r3 = the
// director; lr put back. The original where the color filter's slots aren't
// the ones done natively.
ITC_REGS_STEP(CameraModesStep821597F0) {
  if (!cm::DirectorResumeReady()) {
    lifted::original::c821597F0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 240;
  const uint32_t director = ctx.r3.u32;
  cm::DirectorResume(director, frame, StagesCopyFrom(ctx, frame, 240, 0x82159848),
                     SetModeFrom(ctx, director, frame));
  ctx.lr = lr;
}

// CameraDirector::Update (a handler, through frame interpolation's wrapper,
// src/frame_pacing/per_frame_logic.cpp, on logic passes; r3 not kept): r3 =
// the director; lr put back. TvCamera::Track through its step and the
// camera objects' slot 0 through the translated code (their adapters), from
// its 176-byte frame with the original's return addresses; SetMode
// natively.
ITC_REGS_STEP(CameraModesStep821594B8) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 176;
  const uint32_t director = ctx.r3.u32;
  cm::DirectorUpdateCalls calls;
  calls.track = [&ctx, frame](uint32_t camera, uint32_t position) {
    CallFrom(ctx, frame, 176, 0x82159594, [&] {
      ctx.r3.u64 = camera;
      ctx.r4.u64 = position;
      CameraModesStep82156980(ctx);
    });
  };
  calls.set_mode = SetModeFrom(ctx, director, frame);
  calls.slot0 = [&ctx, frame](uint32_t camera, uint32_t which) {
    const uint32_t back = which == 0 ? 0x82159614 : 0x82159648;
    CallFrom(ctx, frame, 176, back, [&] {
      ctx.r3.u64 = camera;
      ctx.ctr.u64 = guest::LoadU32(guest::LoadU32(camera));
      lifted::CallFromContext(ctx, ctx.ctr.u32);
    });
  };
  cm::DirectorUpdate(director, frame, calls);
  ctx.lr = lr;
}

// The director's update as frame interpolation's wrapper runs it on logic
// passes: natively with --native_camera_modes, else the original.
ITC_REGS_STEP(CameraModesUpdate) {
  if (!ITC_FLAG(native_camera_modes)) {
    lifted::original::c821594B8(ctx);
    return;
  }
  CameraModesStep821594B8(ctx);
}

}  // namespace hooks

ITC_CAMERA_MODES(82159950)
ITC_CAMERA_MODES(821591F0)
ITC_CAMERA_MODES(821596B0)
ITC_CAMERA_MODES(821597F0)

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
ITC_REGS_STEP(CameraModesStep8217BE98) {
  if ((ctx.r1.u32 & 0xF) != 0 || !cm::GarageReady(ctx.r3.u32)) {
    lifted::original::c8217BE98(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 272;
  cm::GarageCalls calls;
  calls.queue_and_list = [&ctx](uint32_t f) {
    std::pair<uint32_t, uint32_t> answer;
    CallFrom(ctx, f, 272, 0x8217BEC8, [&] {
      const auto slot = [&](uint32_t object, uint32_t offset, uint32_t back) {
        ctx.r3.u64 = object;
        ctx.ctr.u64 = guest::LoadU32(guest::LoadU32(object) + offset);
        ctx.lr = back;
        // (The render manager's slot 8 by its plain C++ where it's the
        // game's, hooks::DirectSlotCall.)
        if (!hooks::DirectSlotCall(ctx, ctx.ctr.u32)) {
          lifted::CallFromContext(ctx, ctx.ctr.u32);
        }
        return ctx.r3.u32;
      };
      const uint32_t scene = slot(schema::AppStartUpGlobals::render_app().address, 32, 0x8217BEC8);
      answer.first = slot(scene, 8, 0x8217BED8);
      const uint32_t again = slot(schema::AppStartUpGlobals::render_app().address, 32, 0x8217BEF4);
      answer.second = slot(again, 4, 0x8217BF04);
    });
    return answer;
  };
  calls.pass_update = [&ctx](uint32_t pass, uint32_t f) {
    CallFrom(ctx, f - 112, 112, 0x820AA534, [&] {
      ctx.r3.u64 = pass;
      lifted::Enter(ctx, game::CameraPass::Update);
    });
  };
  cm::GarageUpdate(ctx.r3.u32, frame, calls);
  ctx.r3.u64 = schema::CamerasGlobals::passes_address();
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES(8217BE98)

// --- The cutscene's constructor ---

namespace hooks {

// Cutscene::Cutscene: r3 = the cutscene, r4 = the viewport, r5 = the
// vehicle; r3 = the cutscene as the original's r23 leaves it, lr put back
// (as its epilogue). Its guest calls from a frame like the original's
// (1,632 bytes) with each call site's return address: the video output,
// CameraAnims::FreeAll natively where its adapter would (--native_heap, the
// layer's pages), else its original; Stages::Copy's original where its
// plain C++ refuses; List16::Insert32's and Insert608's refused copies
// through the guest's memcpy from their frames (112 bytes); CutsceneShot::
// Init's calls (ShotInitCalls, from Init's frame below this one; the return
// address of the scripts' last shot call, 0x82158B38, for every shot: only
// Init's own lr save below the frame would hold it); RivalLogo::Open's from
// its frame (RivalLogoOpenCallsAt). The original where ChaseNative says no
// (CameraTarget::Track's frame, ZoomFromFov's tan of the same angle).
ITC_REGS_STEP(CameraModesStep82156CC0) {
  if (!ChaseNative(ctx)) {
    lifted::original::c82156CC0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - cm::kCutsceneFrame;
  auto from_frame = [&ctx, frame](uint32_t back, const std::function<void()>& call) {
    CallFrom(ctx, frame, cm::kCutsceneFrame, back, call);
  };
  const cm::CutsceneCalls shot = ShotInitCalls(ctx);
  const RivalLogoOpenCalls logo = RivalLogoOpenCallsAt(ctx, frame - 112);
  cm::CutsceneConstructCalls calls;
  calls.video_output = VideoOutput(ctx);
  calls.free_all = [&] {
    if (hooks::NativeHeapActive() && cm::AnimsFreeable()) {
      cm::AnimsFreeAll(frame - 384 + 80);
      return;
    }
    from_frame(0x82156E6C, [&] { lifted::original::c821507D8(ctx); });
  };
  calls.stages_copy = [&](uint32_t out, uint32_t stages, uint32_t stage) {
    from_frame(0x82156ED0, [&] {
      ctx.r3.u64 = out;
      ctx.r4.u64 = stages;
      ctx.r5.u64 = stage;
      lifted::original::c820FFD88(ctx);
    });
  };
  calls.scripts.insert32 = [&](uint32_t to, uint32_t from, uint32_t size, uint32_t back) {
    from_frame(back, [&] { GuestMemcpy(ctx, 112, 0x821543E0, to, from, size); });
  };
  calls.scripts.insert608 = [&](uint32_t to, uint32_t from, uint32_t size, uint32_t back) {
    from_frame(back, [&] { GuestMemcpy(ctx, 112, 0x82154450, to, from, size); });
  };
  calls.scripts.shot.copy32 = [&](uint32_t to, uint32_t from, uint32_t size) {
    from_frame(0x82158B38, [&] { shot.copy32(to, from, size); });
  };
  calls.scripts.shot.insert32 = [&](uint32_t to, uint32_t from, uint32_t size) {
    from_frame(0x82158B38, [&] { shot.insert32(to, from, size); });
  };
  calls.scripts.shot.load = [&](uint32_t id) {
    from_frame(0x82158B38, [&] { shot.load(id); });
  };
  calls.open = [&](uint32_t id, uint32_t asset) {
    uint32_t r3 = 0;
    from_frame(0x82158F5C, [&] { r3 = logo.open(id, asset); });
    return r3;
  };
  calls.load_xpr = [&](uint32_t holder, uint32_t id) {
    uint32_t r3 = 0;
    from_frame(0x82158F5C, [&] { r3 = logo.load_xpr(holder, id); });
    return r3;
  };
  ctx.r3.u64 = cm::CutsceneConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame, calls);
  ctx.lr = lr;
}

}  // namespace hooks

ITC_CAMERA_MODES_CONSTRUCT(82156CC0)

// For the fallback originals (docs/missing-functions.md).
// TrackRoute::SplinePoint: r3 = out, r4 = the matrix, r5 = the position,
// f1 = t; its atexit through the translated code from its frame (720 bytes)
// when the native one can't, the original without native atexit (as
// CameraPath::Evaluate's step).
ITC_DIRECT_ENTRY(820AB6A0) {
  if (!hooks::NativeAtexit()) {
    lifted::original::c820AB6A0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  SplineAtexit atexit{ctx, ctx.r1.u32 - 720};
  cm::SplinePoint(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.r1.u32,
                  atexit.Call());
}
// Key::Value: r3 = the pair, f1 = t; f1 the value.
ITC_DIRECT_ENTRY(820AC178) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = cm::KeyValue(ctx.r3.u32, ctx.f1.f64, ctx.r1.u32);
}
// CameraAnimBlock::Free: r3 = the block.
ITC_DIRECT_ENTRY(8214FFF0) { cm::BlockFree(ctx.r3.u32); }
// CameraAnim::EvalTarget: r4 = the object, r5 = out, r6 = the time.
ITC_DIRECT_ENTRY(821504E0) {
  ctx.fpscr.disableFlushMode();
  cm::AnimEvalTarget(ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
// Camera::ShakeRandom: r3 = the viewport, f1 = the amount; its frame 160
// bytes below.
ITC_DIRECT_ENTRY(82152BD8) {
  ctx.fpscr.disableFlushMode();
  cm::ShakeRandom(ctx.r3.u32, ctx.f1.f64, ctx.r1.u32 - 160);
}
ITC_DIRECT_ENTRY(82153228) { hooks::CameraModesStep82153228(ctx); }
ITC_DIRECT_ENTRY(82154300) { hooks::CameraModesStep82154300(ctx); }
// RivalLogo::InitXpr: r3 = the holder; r3 = its XPR (+4), as XPR::XPR
// leaves it.
ITC_DIRECT_ENTRY(82211708) {
  cm::RivalLogoInitXpr(ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u32 + 4;
}
// Hermite::Basis: r3 = out, r4, r5 the keys, f1 = t.
ITC_DIRECT_ENTRY(82293A48) {
  ctx.fpscr.disableFlushMode();
  cm::HermiteBasis(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64);
}
// CameraAnims::Evaluate: its register step (EvalCamera's original from
// Evaluate's frame where the port declines).
ITC_DIRECT_ENTRY(82150960) { hooks::CameraModesStep82150960(ctx); }

// For the fallback originals (docs/missing-functions.md): swarm unit 34's
// shims (the stale originals round), so lifted code calls the ports.
// Matrix::FromFloats: its register step.
ITC_DIRECT_ENTRY(82090000) { hooks::CameraModesStep82090000(ctx); }
// CameraPath::RoutePoint: r3 = out, r4 = the position, r6 = the offset (r5
// unread), f1 = t; SplinePoint's atexit through the translated code from
// its frame (208 + 720 bytes below) when the native one can't, the
// original without native atexit (as CameraPath::Evaluate's step).
ITC_DIRECT_ENTRY(820ABDA8) {
  if (!hooks::NativeAtexit()) {
    lifted::original::c820ABDA8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  SplineAtexit atexit{ctx, ctx.r1.u32 - 208 - 720};
  cm::RoutePoint(ctx.r3.u32, ctx.r4.u32, ctx.r6.u32, ctx.f1.f64, ctx.r1.u32,
                 atexit.Call());
}
// Curve::Evaluate: r4 = the curve (r3 unread), f1 = t; f1 the value.
ITC_DIRECT_ENTRY(820AC060) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = cm::CurveEvaluate(ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}
// Key::Vec3: r3 = the pairs, r4 = out, f1 = t; r3 = the last pair (its
// third Key::Value's argument, as a constant pair leaves it).
ITC_DIRECT_ENTRY(820AC1D0) {
  ctx.fpscr.disableFlushMode();
  const uint32_t pairs = ctx.r3.u32;
  cm::KeyVec3(pairs, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
  ctx.r3.u64 = pairs + 16;
}
// CameraPath::Evaluate: its register step.
ITC_DIRECT_ENTRY(820AC248) { hooks::CameraModesStep820AC248(ctx); }
// TrackRoute::SliceFraction: its register step.
ITC_DIRECT_ENTRY(820F9B48) { hooks::CameraModesStep820F9B48(ctx); }
// RouteRange::Advance: r3 = out (left), r4 = the range, r5 = the distance.
ITC_DIRECT_ENTRY(820FE598) {
  cm::RangeAdvance(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32);
}
// RouteRange::CameraAt: its register step.
ITC_DIRECT_ENTRY(820FE830) { hooks::CameraModesStep820FE830(ctx); }
// CameraAnim::Free: r3 = the object; r3 = VirtualFree's 1 after a block,
// else left. The original without --native_heap.
ITC_DIRECT_ENTRY(82150098) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c82150098(ctx);
    return;
  }
  const uint32_t object = ctx.r3.u32;
  const bool blocks = schema::CameraAnim(object).blocks_used() != 0;
  cm::AnimFree(object);
  if (blocks) {
    ctx.r3.u64 = 1;
  }
}
// CameraAnim::ParseObject: r3 = the file's object; r3 = the copy. The
// copies XMemCpy refuses through the guest from ParseObject's frame (144
// bytes, returning to 0x821502B8). The original without --native_heap.
ITC_DIRECT_ENTRY(82150100) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c82150100(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::AnimParseObject(ctx.r3.u32,
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestXMemCpy(ctx, 144, 0x821502B8, to, from, size);
      });
}
// CameraAnims::Find: its register step.
ITC_DIRECT_ENTRY(82150300) { hooks::CameraModesStep82150300(ctx); }
// CameraAnim::EvalCamera: r4 = the object, r5 = out, r6 = the time; f1 =
// tan's. The original where tan's native form can't stand in.
ITC_DIRECT_ENTRY(82150350) {
  ctx.fpscr.disableFlushMode();
  const auto f1 = cm::AnimEvalCamera(ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  if (!f1) {
    lifted::original::c82150350(ctx);
    return;
  }
  ctx.f1.f64 = *f1;
}
// CameraAnim::Matrix: r4 = the object, r5 = out, r6 = the time; its frame
// 320 bytes below (16-byte aligned, so a stack pointer that isn't runs the
// original); memcpy's refused copy from it (returning to 0x82150600).
ITC_DIRECT_ENTRY(82150590) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    lifted::original::c82150590(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  cm::AnimMatrix(ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32 - 320,
      [&](uint32_t to, uint32_t from, uint32_t size) {
        GuestMemcpy(ctx, 320, 0x82150600, to, from, size);
      });
}
// The steps (each maps its registers and runs the original where its port
// declines).
ITC_DIRECT_ENTRY(82150A10) { hooks::CameraModesStep82150A10(ctx); }
ITC_DIRECT_ENTRY(82152108) { hooks::CameraModesStep82152108(ctx); }
ITC_DIRECT_ENTRY(82152548) { hooks::CameraModesStep82152548(ctx); }
ITC_DIRECT_ENTRY(821525F8) { hooks::CameraModesStep821525F8(ctx); }
ITC_DIRECT_ENTRY(82152698) { hooks::CameraModesStep82152698(ctx); }
ITC_DIRECT_ENTRY(82152788) { hooks::CameraModesStep82152788(ctx); }
ITC_DIRECT_ENTRY(82152CD8) { hooks::CameraModesStep82152CD8(ctx); }
// CameraPath::Load: r3 = the path buffer, r4 = the asset id; r3 = the
// serial (as Archive::Load leaves it). The original without --native_heap
// or the archive's queue (as Request's step).
ITC_DIRECT_ENTRY(82152D80) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    lifted::original::c82152D80(ctx);
    return;
  }
  hooks::SetArchiveSerial(ctx, cm::PathLoad(ctx.r3.u32, ctx.r4.u32,
                                            hooks::NativeArchiveLoad()));
}
ITC_DIRECT_ENTRY(82152E08) { hooks::CameraModesStep82152E08(ctx); }
ITC_DIRECT_ENTRY(82152E78) { hooks::CameraModesStep82152E78(ctx); }
ITC_DIRECT_ENTRY(82153290) { hooks::CameraModesStep82153290(ctx); }
ITC_DIRECT_ENTRY(82153478) { hooks::CameraModesStep82153478(ctx); }
ITC_DIRECT_ENTRY(82153640) { hooks::CameraModesStep82153640(ctx); }
// IntroCamera::MakeShot: r3 = the shot, r4 = the viewport, r5 = the
// vehicle; its frame 192 bytes below, the atexit's fallback from it (as
// IntroCamera::IntroCamera's step). The original without native atexit.
ITC_DIRECT_ENTRY(82154108) {
  if (!hooks::NativeAtexit()) {
    lifted::original::c82154108(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 192;
  MakeShotAtexit atexit{ctx, frame};
  cm::IntroMakeShot(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame, atexit.Call());
}
ITC_DIRECT_ENTRY(82154378) { hooks::CameraModesStep82154378(ctx); }
ITC_DIRECT_ENTRY(821543E8) { hooks::CameraModesStep821543E8(ctx); }
ITC_DIRECT_ENTRY(82154458) { hooks::CameraModesStep82154458(ctx); }
ITC_DIRECT_ENTRY(821544E0) { hooks::CameraModesStep821544E0(ctx); }
ITC_DIRECT_ENTRY(821562F8) { hooks::CameraModesStep821562F8(ctx); }
ITC_DIRECT_ENTRY(82156B68) { hooks::CameraModesStep82156B68(ctx); }
ITC_DIRECT_ENTRY(82211848) { hooks::CameraModesStep82211848(ctx); }
ITC_DIRECT_ENTRY(822118E0) { hooks::CameraModesStep822118E0(ctx); }
