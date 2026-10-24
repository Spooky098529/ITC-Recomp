// The camera passes and viewports natively: adapters in front of
// src/cameras/cameras.cpp (docs/functions/cameras.md). Camera::SetViewportSize
// and Camera::SetFieldOfView are reached through the camera's vtable (slots 0
// and 1), so their adapters stay, as Camera::Update's (slot 2). Flush-to-zero is left off, as the
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

#include <functional>
#include <optional>

ITC_FLAG_BOOL(native_cameras, true, "ITC",
              "Run the camera passes and viewports (docs/functions/cameras.md: the library "
              "camera's Camera::Camera sub_82593908, Camera::Update sub_82593568, Camera::SetFieldOfView sub_82593278, Camera::BuildProjection "
              "sub_825932E8 (no adapter: only Camera::SetViewportSize's, Update's and Camera's plain C++ call it, "
              "and the flag switches them) and Camera::SetViewportSize sub_82593500; the camera passes' "
              "CameraPasses::Mode sub_8209F718, SetMode sub_8209F738, RenderTarget "
              "sub_8209F7A8, EnableForView sub_820AA1A8 and SetNear sub_820AA368, "
              "CameraPass::Enable sub_820A0968, Disable sub_820A0958 and ReleaseSurfaces "
              "sub_820A0C40, and SetZoom sub_820AA378 (no adapter: only CameraPass::FromViewport's "
              "plain C++ calls it, and the flag switches that); the viewports' Viewports::Release "
              "sub_820AA430 (with --native_heap), Viewport::SetZoom sub_820AAE90, SetRoll "
              "sub_820AA2D0, UpdateEntries sub_820AA550 (no adapter: only Update's, Set's and LookAt's plain C++ "
              "call it, and the flag switches them), Update sub_820AAE40, Set sub_820AAE48 and "
              "LookAt sub_820AAF68; CameraPass::Update sub_820A3978, its frame interpolation's notes kept, and "
              "CameraPass::RecordViewport sub_820A0978, without the command list's "
              "diagnostics, and CameraPass::FromViewport sub_820AA4A8; RecordClear sub_8209FA38, "
              "without the command list's diagnostics; CameraPasses::RecordBegins sub_8209F8A0 and "
              "RecordEnds sub_8209F900 (in their diagnostics' wrappers) with CameraPass::RecordBegin "
              "sub_820A0CC0 and RecordEnd sub_820A1C30, without the command list's diagnostics; "
              "CameraPasses::MakeRenderTarget sub_8209FBF0 with its case 0 sub_8209FC7C and "
              "CameraPass::SetUpSurfaces sub_820A2F40 with its types 0-2 chunk sub_820A2F94, with "
              "--native_video_output, --native_graphics_resources and --native_heap) natively instead of "
              "through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=cameras)");

ITC_FLAG_DECLARE(bool, native_video_output);
ITC_FLAG_DECLARE(bool, native_graphics_resources);

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

// Camera::Update (vtable slot 2): r3 = the camera. From a frame like the
// original's (320 bytes), 16-byte aligned (the original otherwise); r3 left
// as the original's last call leaves it (Matrix::Multiply's out, +144) and
// flush-to-zero off.
extern "C" REX_FUNC(sub_82593568) {
  ITC_CAMERAS_FALLBACK(82593568)
  if (!hooks::CameraUpdate(ctx, base)) {
    __imp__sub_82593568(ctx, base);
  }
}

// Camera::Camera: r3 = the camera; r3 = it. Its frame 128 bytes, Update's
// below it.
extern "C" REX_FUNC(sub_82593908) {
  ITC_CAMERAS_FALLBACK(82593908)
  if (!hooks::CameraConstruct(ctx, base)) {
    __imp__sub_82593908(ctx, base);
  }
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

// Viewport::Update, Set and LookAt: r3 = the viewport (Set's r4 to r7 the
// four vectors, LookAt's r4 the eye and r5 the target); r3 = entry 16, as
// Viewport::UpdateEntries leaves it (no adapter of its own: only their plain
// C++ calls it). Flush-to-zero left off.
extern "C" REX_FUNC(sub_820AAE40) {
  ITC_CAMERAS_FALLBACK(820AAE40)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cameras::ViewportUpdate(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_820AAE48) {
  ITC_CAMERAS_FALLBACK(820AAE48)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cameras::ViewportSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                    ctx.r7.u32);
}

extern "C" REX_FUNC(sub_820AAF68) {
  ITC_CAMERAS_FALLBACK(820AAF68)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cameras::ViewportLookAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
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

namespace {

// The video output, as the device's users reach it: `[0x8266D088]`'s vtable
// slot 7, then that object's slot 3, as guest calls with the original's
// return addresses. r3 = the video output after.
uint32_t VideoOutputCall(PPCContext& ctx, uint8_t* base, uint32_t first, uint32_t second) {
  ctx.r3.u64 = guest::LoadU32(base, 0x8266D088);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
  ctx.lr = first;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
  ctx.lr = second;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return ctx.r3.u32;
}

// Whether the native device users can run: the device lock, the texture and
// surface makers native.
bool DeviceUsersNative() {
  return ITC_FLAG(native_video_output) && ITC_FLAG(native_graphics_resources) &&
         hooks::NativeHeapActive();
}

}  // namespace

namespace hooks {

bool CameraPassesMakeRenderTarget(PPCContext& ctx, uint8_t* base) {
  if (!DeviceUsersNative()) {
    return false;
  }
  const uint32_t id = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t video_output = 0;
  guest_call::InFrame(ctx, base, 112, 0x8209FC20, [&](uint32_t frame) {
    video_output = cameras::MakeRenderTarget(
        base, frame, id,
        {[&](int at) {
           return at == 0 ? VideoOutputCall(ctx, base, 0x8209FC20, 0x8209FC30)
                          : VideoOutputCall(ctx, base, 0x8209FF84, 0x8209FF94);
         },
         [&](uint32_t target) {
           ctx.r3.u64 = target;
           ctx.lr = 0x8209FC40;
           sub_8209F7A8(ctx, base);
           return ctx.r3.u32;
         },
         [&](uint32_t resource) {
           ctx.r3.u64 = resource;
           ctx.lr = 0x8209FC4C;
           sub_822A67B0(ctx, base);
         }});
  });
  ctx.lr = lr;
  ctx.r3.u64 = video_output;
  return true;
}

bool CameraPassSetUpSurfaces(PPCContext& ctx, uint8_t* base) {
  if (!DeviceUsersNative()) {
    return false;
  }
  const uint32_t pass = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // The lock's video output calls' return addresses, by type (types 0-2
  // the chunk's; 5-9 don't lock).
  struct ReturnAddresses {
    uint32_t first, second;
  };
  static constexpr ReturnAddresses kLock[21] = {
      {0x820A2FAC, 0x820A2FBC}, {0x820A2FAC, 0x820A2FBC}, {0x820A2FAC, 0x820A2FBC},
      {0x820A3010, 0x820A3020}, {0x820A30E8, 0x820A30F8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
      {0x820A324C, 0x820A325C}, {0x820A3414, 0x820A3424}, {0x820A360C, 0x820A361C},
      {0x820A32E4, 0x820A32F4}, {0x820A34BC, 0x820A34CC}, {0x820A36B4, 0x820A36C4},
      {0x820A337C, 0x820A338C}, {0x820A3564, 0x820A3574}, {0x820A375C, 0x820A376C},
      {0x820A3804, 0x820A3814}, {0x820A38B0, 0x820A38C0},
  };
  // The other calls' return addresses are type 3's (or type 4's for the
  // cube face; 5-9 make none of these but RenderTarget and the cube face):
  // nothing reads them.
  guest_call::InFrame(ctx, base, 544, 0x820A2F48, [&](uint32_t frame) {
    cameras::SetUpSurfaces(
        base, frame, pass,
        {[&](int at) {
           const uint32_t type = guest::LoadU32(base, pass + 584);
           const ReturnAddresses r = at == 0 && type <= 20 ? kLock[type]
                                                          : ReturnAddresses{0x820A3950, 0x820A3960};
           return VideoOutputCall(ctx, base, r.first, r.second);
         },
         [&](uint32_t id) {
           ctx.r3.u64 = id;
           ctx.lr = 0x820A2FF0;
           sub_8209F7A8(ctx, base);
           return ctx.r3.u32;
         },
         [&](uint32_t id) {
           ctx.r3.u64 = id;
           ctx.lr = 0x820A2FE8;
           sub_8209FBF0(ctx, base);
         },
         [&](uint32_t texture, uint32_t face) {
           ctx.r3.u64 = texture;
           ctx.r4.u64 = face;
           ctx.r5.u64 = 0;
           ctx.lr = 0x820A30BC;
           sub_822A5C50(ctx, base);
           return ctx.r3.u32;
         },
         [&] {
           ctx.r3.u64 = pass;
           ctx.lr = 0x820A2F60;
           sub_820A0C40(ctx, base);
         }});
  });
  ctx.lr = lr;
  return true;
}

}  // namespace hooks

// CameraPasses::MakeRenderTarget: r3 = the id (hooks::CameraPassesMakeRenderTarget).
extern "C" REX_FUNC(sub_8209FBF0) {
  ITC_CAMERAS_FALLBACK(8209FBF0)
  if (!hooks::CameraPassesMakeRenderTarget(ctx, base)) {
    __imp__sub_8209FBF0(ctx, base);
  }
}

// CameraPass::SetUpSurfaces: r3 = the pass (hooks::CameraPassSetUpSurfaces).
extern "C" REX_FUNC(sub_820A2F40) {
  ITC_CAMERAS_FALLBACK(820A2F40)
  if (!hooks::CameraPassSetUpSurfaces(ctx, base)) {
    __imp__sub_820A2F40(ctx, base);
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

// A chunk recorded and handed to the render queue, as RecordViewport and
// RecordClear do: the command list from the scene ([kScene]'s slot 8, then
// slot 1), `record(list)` (the plain C++: its first slot, or nullopt where
// the original must run), the render queue (slot 8, then slot 2) and its
// slot 4 (r4 = the chunk, r5 = the key, f1 = 0.0), the calls made from a
// frame like the original's (`frame_bytes`) with its return addresses
// (`returns`, in that order). False where the original must run (the
// command list's diagnostics on, or a chunk its command list calls would
// print about: then the scene's getters have been called once more than the
// original would, never seen in play).
bool RecordAndSubmit(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes,
                     const uint32_t (&returns)[5], uint32_t key,
                     const std::function<std::optional<uint32_t>(uint32_t list)>& record) {
  if (ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
      ITC_FLAG(log_recording_by_pass)) {
    return false;
  }
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> chunk;
  guest_call::InFrame(ctx, base, frame_bytes, returns[0], [&](uint32_t) {
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, returns[0]);
    VtableCall(ctx, base, 1, returns[1]);
    chunk = record(ctx.r3.u32);
    if (!chunk) {
      return;
    }
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, returns[2]);
    VtableCall(ctx, base, 2, returns[3]);
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = key;
    VtableCall(ctx, base, 4, returns[4]);
  });
  ctx.lr = lr;
  return chunk.has_value();
}

// CameraPass::RecordViewport's: its frame is 144 bytes.
bool RecordViewportCalls(PPCContext& ctx, uint8_t* base, uint32_t pass, uint32_t key) {
  static constexpr uint32_t kReturns[5] = {0x820A09A8, 0x820A09B8, 0x820A0A88, 0x820A0A98,
                                           0x820A0AB4};
  return RecordAndSubmit(ctx, base, 144, kReturns, key, [&](uint32_t list) {
    return cameras::RecordViewport(base, list, pass);
  });
}

// The record passes' guest calls by type: the getters' (scene slot 8, then
// slot 2) and the render queue's slot 4's return addresses for each chunk
// handed on, and (RecordEnd) CameraPasses::RenderTarget's.
struct Route {
  uint32_t get[2][2];
  uint32_t submit[2];
  uint32_t render_target;
};

// Hands `subs` to the render queue as the original does: for each, the
// scene's getters (slot 8, then slot 2) and the queue's slot 4 (r4 = the
// chunk, r5 = the key, f1 = 0.0).
void Submit(PPCContext& ctx, uint8_t* base, const cameras::Submissions& subs, const Route& route) {
  for (uint32_t i = 0; i < subs.count; i++) {
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, route.get[i][0]);
    VtableCall(ctx, base, 2, route.get[i][1]);
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
    ctx.r4.u64 = subs.chunk[i];
    ctx.r5.u64 = subs.key[i];
    VtableCall(ctx, base, 4, route.submit[i]);
  }
}

// A record pass (RecordBegin or RecordEnd): from a frame like the original's
// (176 bytes), the scene's getters for the list (slot 8, then slot 1, with
// `list_returns`), `record(list)` (the plain C++), then the chunks handed on.
// False where the original must run (the command list's diagnostics on, or
// a chunk its command list calls would print about: then the scene's getters
// have been called once more than the original would, never seen in play).
bool RecordPass(PPCContext& ctx, uint8_t* base, const uint32_t (&list_returns)[2],
                const Route& route,
                const std::function<std::optional<cameras::Submissions>(uint32_t list)>& record) {
  if (ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
      ITC_FLAG(log_recording_by_pass)) {
    return false;
  }
  const uint64_t lr = ctx.lr;
  bool done = false;
  guest_call::InFrame(ctx, base, 176, list_returns[0], [&](uint32_t) {
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    VtableCall(ctx, base, 8, list_returns[0]);
    VtableCall(ctx, base, 1, list_returns[1]);
    const std::optional<cameras::Submissions> subs = record(ctx.r3.u32);
    if (!subs) {
      return;
    }
    done = true;
    Submit(ctx, base, *subs, route);
  });
  ctx.lr = lr;
  return done;
}

Route BeginRoute(uint32_t type) {
  // The second chunk's return addresses by the first's key's path.
  static constexpr uint32_t kLast[2][2] = {{0x820A12F8, 0x820A1308}, {0x820A1358, 0x820A1368}};
  switch (type) {
    case 3: return {{{0x820A0ED8, 0x820A0EE8}}, {0x820A1C20}, 0};
    case 4: return {{{0x820A109C, 0x820A10AC}}, {0x820A1C20}, 0};
    case 19: return {{{0x820A1998, 0x820A19A8}}, {0x820A1C20}, 0};
    case 16: return {{{0x820A12B8, 0x820A12C8}, {kLast[0][0], kLast[0][1]}}, {0x820A12E4, 0x820A1C20}, 0};
    case 17: return {{{0x820A1580, 0x820A1590}, {kLast[0][0], kLast[0][1]}}, {0x820A12E4, 0x820A1C20}, 0};
    case 18: return {{{0x820A17C4, 0x820A17D4}, {kLast[0][0], kLast[0][1]}}, {0x820A12E4, 0x820A1C20}, 0};
    case 10:
    case 13: return {{{0x820A1318, 0x820A1328}, {kLast[1][0], kLast[1][1]}}, {0x820A1344, 0x820A1C20}, 0};
    case 11:
    case 14: return {{{0x820A159C, 0x820A15AC}, {kLast[1][0], kLast[1][1]}}, {0x820A1344, 0x820A1C20}, 0};
    case 12:
    case 15: return {{{0x820A17E0, 0x820A17F0}, {kLast[1][0], kLast[1][1]}}, {0x820A1344, 0x820A1C20}, 0};
    case 20: return {{{0x820A1BB4, 0x820A1BC4}, {0x820A1BF4, 0x820A1C04}}, {0x820A1BE0, 0x820A1C20}, 0};
    default: return {};
  }
}

Route EndRoute(uint32_t type) {
  static constexpr uint32_t kLast[2] = {0x820A2648, 0x820A2658};
  switch (type) {
    case 3: return {{{0x820A1DA8, 0x820A1DB8}}, {0x820A2D24}, 0x820A1D1C};
    case 4: return {{{0x820A1EC4, 0x820A1ED4}}, {0x820A2D24}, 0x820A1E38};
    case 5: return {{{0x820A1FDC, 0x820A1FEC}}, {0x820A2D24}, 0x820A1F50};
    case 6: return {{{0x820A20F8, 0x820A2108}}, {0x820A2D24}, 0x820A206C};
    case 7: return {{{0x820A2214, 0x820A2224}}, {0x820A2D24}, 0x820A2184};
    case 8: return {{{0x820A2330, 0x820A2340}}, {0x820A2D24}, 0x820A22A0};
    case 9: return {{{0x820A2448, 0x820A2458}}, {0x820A2D24}, 0x820A23BC};
    case 10:
    case 13: return {{{0x820A2664, 0x820A2674}, {kLast[0], kLast[1]}}, {0x820A2634, 0x820A2D24}, 0x820A2500};
    case 16: return {{{0x820A2608, 0x820A2618}, {kLast[0], kLast[1]}}, {0x820A2634, 0x820A2D24}, 0x820A2500};
    case 11:
    case 14: return {{{0x820A283C, 0x820A284C}, {kLast[0], kLast[1]}}, {0x820A2634, 0x820A2D24}, 0x820A2718};
    case 17: return {{{0x820A2820, 0x820A2830}, {kLast[0], kLast[1]}}, {0x820A2634, 0x820A2D24}, 0x820A2718};
    case 12:
    case 15: return {{{0x820A2A14, 0x820A2A24}, {kLast[0], kLast[1]}}, {0x820A2634, 0x820A2D24}, 0x820A28F0};
    case 18: return {{{0x820A29F8, 0x820A2A08}, {kLast[0], kLast[1]}}, {0x820A2634, 0x820A2D24}, 0x820A28F0};
    case 19: return {{{0x820A2B2C, 0x820A2B3C}}, {0x820A2D24}, 0x820A2AA0};
    case 20: return {{{0x820A2CB8, 0x820A2CC8}, {0x820A2CF8, 0x820A2D08}}, {0x820A2CE4, 0x820A2D24}, 0x820A2BB8};
    default: return {};
  }
}

}  // namespace

// CameraPass::RecordBegin and RecordEnd: r3 = the pass. Their callers
// (CameraPasses::RecordBegins and RecordEnds) are native and call the work
// directly; these stay for them with the flag off (their work makes guest
// calls).
extern "C" REX_FUNC(sub_820A0CC0) {
  ITC_CAMERAS_FALLBACK(820A0CC0)
  if (!hooks::CameraPassRecordBegin(ctx, base)) {
    __imp__sub_820A0CC0(ctx, base);
  }
}

extern "C" REX_FUNC(sub_820A1C30) {
  ITC_CAMERAS_FALLBACK(820A1C30)
  if (!hooks::CameraPassRecordEnd(ctx, base)) {
    __imp__sub_820A1C30(ctx, base);
  }
}

// CameraPass::RecordViewport: r3 = the pass, r4 = the sort key.
extern "C" REX_FUNC(sub_820A0978) {
  ITC_CAMERAS_FALLBACK(820A0978)
  hooks::CameraPassRecordViewport(ctx, base);
}

// RecordClear: r4 = the key, r5 = the flags, r6 = the color, r7 = the depth
// (its low word). The guest calls as RecordViewport's, from a 160-byte frame.
extern "C" REX_FUNC(sub_8209FA38) {
  ITC_CAMERAS_FALLBACK(8209FA38)
  hooks::RecordClear(ctx, base);
}

namespace hooks {

bool CameraUpdate(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    return false;
  }
  const uint32_t camera = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 320, 0x825935EC, [&](uint32_t frame) {
    ctx.fpscr.disableFlushMode();
    cameras::Update(base, camera, frame);
  });
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = camera + 144;
  return true;
}

bool CameraConstruct(PPCContext& ctx, uint8_t* base) {
  if ((ctx.r1.u32 & 0xF) != 0) {
    return false;
  }
  const uint32_t camera = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 128, 0x82593AA4, [&](uint32_t frame) {
    ctx.fpscr.disableFlushMode();
    cameras::Construct(base, camera, frame);
  });
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = camera;
  return true;
}

void RecordClear(PPCContext& ctx, uint8_t* base) {
  static constexpr uint32_t kReturns[5] = {0x8209FA6C, 0x8209FA7C, 0x8209FB44, 0x8209FB54,
                                           0x8209FB74};
  const PPCContext in = ctx;
  const uint32_t flags = ctx.r5.u32, color = ctx.r6.u32, depth = ctx.r7.u32;
  if (!RecordAndSubmit(ctx, base, 160, kReturns, ctx.r4.u32, [&](uint32_t list) {
        return cameras::RecordClear(base, list, flags, color, depth);
      })) {
    ctx.r4 = in.r4;
    ctx.r5 = in.r5;
    ctx.r6 = in.r6;
    ctx.r7 = in.r7;
    __imp__sub_8209FA38(ctx, base);
  }
}

bool CameraPassRecordBegin(PPCContext& ctx, uint8_t* base) {
  static constexpr uint32_t kList[2] = {0x820A0CEC, 0x820A0CFC};
  const uint32_t pass = ctx.r3.u32;
  const Route route = BeginRoute(guest::LoadU32(base, pass + 584));
  if (!RecordPass(ctx, base, kList, route,
                  [&](uint32_t list) { return cameras::RecordBegin(base, list, pass); })) {
    ctx.r3.u64 = pass;
    return false;
  }
  return true;
}

bool CameraPassRecordEnd(PPCContext& ctx, uint8_t* base) {
  static constexpr uint32_t kList[2] = {0x820A1C5C, 0x820A1C6C};
  const uint32_t pass = ctx.r3.u32;
  const Route route = EndRoute(guest::LoadU32(base, pass + 584));
  if (!RecordPass(ctx, base, kList, route, [&](uint32_t list) {
        return cameras::RecordEnd(base, list, pass, [&](uint32_t id) {
          // Through its override (the diagnostics' wrapper), as the original.
          ctx.r3.u64 = id;
          ctx.lr = route.render_target;
          sub_8209F7A8(ctx, base);
          return ctx.r3.u32;
        });
      })) {
    ctx.r3.u64 = pass;
    return false;
  }
  return true;
}

void CameraPassesRecordBegins(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x8209F8D4, [&](uint32_t) {
    cameras::RecordBegins(base, [&](uint32_t pass) {
      ctx.r3.u64 = pass;
      ctx.lr = 0x8209F8D4;
      if (!ITC_FLAG(native_cameras) || !CameraPassRecordBegin(ctx, base)) {
        ctx.r3.u64 = pass;
        ctx.lr = 0x8209F8D4;
        __imp__sub_820A0CC0(ctx, base);
      }
    });
  });
  ctx.lr = lr;
}

void CameraPassesRecordEnds(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x8209F934, [&](uint32_t) {
    cameras::RecordEnds(base, [&](uint32_t pass) {
      ctx.r3.u64 = pass;
      ctx.lr = 0x8209F934;
      if (!ITC_FLAG(native_cameras) || !CameraPassRecordEnd(ctx, base)) {
        ctx.r3.u64 = pass;
        ctx.lr = 0x8209F934;
        __imp__sub_820A1C30(ctx, base);
      }
    });
  });
  ctx.lr = lr;
}

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

