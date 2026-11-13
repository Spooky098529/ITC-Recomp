// The minimap's parts natively: adapters in front of src/minimap/
// (docs/functions/minimap.md), with the register steps the harness shares
// (hooks/minimap.h). Minimap::Draw's (sub_820F5740) register steps are here;
// its override is frame interpolation's wrap
// (src/frame_pacing/per_frame_logic.cpp), which runs them.

#include "hooks/minimap.h"

#include "base/flags.h"
#include "car_scene_draw/car_scene_draw.h"
#include "hooks/c_runtime.h"
#include "hooks/car_scene_draw.h"
#include "hooks/guest_call.h"
#include "hooks/scenery_lists.h"
#include "minimap/minimap.h"

ITC_FLAG_BOOL(native_minimap, true, "ITC",
              "Run the minimap's parts (docs/functions/minimap.md: Race::FindPlayerEntry "
              "sub_82170608, Race::RacerVehicle sub_82170578, SetList::Point sub_82147D18, "
              "Minimap::RacerMarkerType sub_820F2D28 (with its switch's case 1, sub_820F2E68; "
              "sub_820F2CA8's chained record the original's), Minimap::Scaling sub_820F2EF8, "
              "Triangle::Init sub_820F4040 (no adapter: only the marker draws' plain C++ calls it), List80::Insert sub_820F5238 (with List80::Link "
              "sub_820F40C0, no adapter: only it calls it) and List80::Copy sub_820F51A8, Primitive::RecordTransform sub_8217AEA8, Triangle::Record "
              "sub_8217DAA0 (with Triangle::RecordFlat sub_8217C370, no adapter: only it calls "
              "it; the render queue's submit a guest call; sub_8217C4C0, never seen in play, the "
              "original's), Minimap::DrawCarMarker sub_820F4888 and Minimap::DrawPointMarker "
              "sub_820F4BA8 (with native atexit; Triangle::Record from their frames, through "
              "its adapter), Minimap::Draw sub_820F5740 (inside frame interpolation's wrap; with "
              "--native_scenery_lists, --native_matrix_math, --native_vehicle_accessors, "
              "--native_car_physics, --native_math and --native_c_runtime and the command list's "
              "diagnostics off, else its original); since it is native, RacerVehicle, SetList::Point, "
              "RacerMarkerType, Scaling, List80::Insert and Copy and the marker draws have no "
              "adapters: the flag switches their callers) natively instead "
              "of through the translated code (the same writes; checked by "
              "--run_harnesses=minimap)");

// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the original runs.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);
ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_c_runtime);
ITC_FLAG_DECLARE(bool, native_math);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_scenery_lists);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);

namespace hooks {

void MinimapFindPlayerEntry(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = minimap::RaceFindPlayerEntry(base, ctx.r3.u32, ctx.r4.u32);
}

void MinimapRacerVehicle(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = minimap::RaceRacerVehicle(base, ctx.r3.u32);
}

void MinimapSetListPoint(PPCContext& ctx, uint8_t* base) {
  minimap::SetListPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

void MinimapRacerMarkerType(PPCContext& ctx, uint8_t* base) {
  const auto type = minimap::RacerMarkerType(base, ctx.r3.u32, ctx.r4.s32);
  if (!type) {
    __imp__sub_820F2D28(ctx, base);
    return;
  }
  ctx.r3.s64 = *type;
}

void MinimapScaling(PPCContext& ctx, uint8_t* base) {
  if (!minimap::Scaling(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820F2EF8(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32;
}

void MinimapTriangleInit(PPCContext& ctx, uint8_t* base) {
  minimap::TriangleInit(base, ctx.r3.u32);
}

void MinimapList80Insert(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = minimap::List80Insert(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void MinimapList80Copy(PPCContext& ctx, uint8_t* base) {
  if (!minimap::List80Copy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_820F51A8(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32;
}

void MinimapRecordTransform(PPCContext& ctx, uint8_t* base) {
  if (ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
      ITC_FLAG(log_recording_by_pass)) {
    __imp__sub_8217AEA8(ctx, base);
    return;
  }
  const auto slot = minimap::RecordTransform(base, ctx.r3.u32, ctx.r4.u32,
                                             ctx.r1.u32 - minimap::kTransformProduct);
  if (!slot) {
    __imp__sub_8217AEA8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *slot;
}

void MinimapTriangleRecordFlat(PPCContext& ctx, uint8_t* base) {
  if (ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
      ITC_FLAG(log_recording_by_pass)) {
    __imp__sub_8217C370(ctx, base);
    return;
  }
  const uint32_t key = ctx.r5.u32;
  const auto chunk = minimap::TriangleRecordFlat(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - minimap::kRecordFlatFrame);
  if (!chunk) {
    __imp__sub_8217C370(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, minimap::kRecordFlatFrame, 0x8217C4B8, [&](uint32_t) {
    // The render queue's slot 4 (sub_8258EDC8 in play): r4 = the chunk, r5 =
    // the key, f1 = 0.0 (0x82000A34).
    const uint32_t queue = guest::LoadU32(base, minimap::kTriangleLists);
    ctx.r3.u64 = queue;
    ctx.r4.u64 = *chunk;
    ctx.r5.u64 = key;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, queue) + 16));
  });
  ctx.lr = lr;
}

void MinimapTriangleRecord(PPCContext& ctx, uint8_t* base) {
  switch (minimap::TriangleRecord(base, ctx.r3.u32)) {
    case minimap::TriangleRecording::kFlat:
      MinimapTriangleRecordFlat(ctx, base);
      return;
    case minimap::TriangleRecording::kOther:
      sub_8217C4C0(ctx, base);
      return;
    case minimap::TriangleRecording::kNone:
      return;
  }
}

namespace {

// A marker draw's guest calls (minimap::MarkerCalls), made from its frame
// with the original's return addresses.
struct MarkerContext {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t frame;
  uint32_t record_return;
  uint32_t memcpy_return;
  GuestAtexit atexit;

  static void Record(void* self, uint32_t triangle, uint32_t pass, uint32_t key) {
    auto& m = *static_cast<MarkerContext*>(self);
    const uint64_t stack = m.ctx.r1.u64;
    m.ctx.r1.u64 = m.frame;
    m.ctx.lr = m.record_return;
    m.ctx.r3.u64 = triangle;
    m.ctx.r4.u64 = pass;
    m.ctx.r5.u64 = key;
    sub_8217DAA0(m.ctx, m.base);
    m.ctx.r1.u64 = stack;
  }
  static void Memcpy(void* self, uint32_t to, uint32_t from, uint32_t size) {
    auto& m = *static_cast<MarkerContext*>(self);
    const uint64_t stack = m.ctx.r1.u64;
    m.ctx.r1.u64 = m.frame;
    m.ctx.lr = m.memcpy_return;
    m.ctx.r3.u64 = to;
    m.ctx.r4.u64 = from;
    m.ctx.r5.u64 = size;
    sub_825A7410(m.ctx, m.base);
    m.ctx.r1.u64 = stack;
  }
};

using MarkerDraw = void (*)(uint8_t*, uint32_t, uint32_t, uint32_t, uint32_t,
                            const minimap::MarkerCalls&);

void DrawMarker(PPCContext& ctx, uint8_t* base, MarkerDraw draw, uint32_t frame_bytes,
                uint32_t record_return, uint32_t memcpy_return) {
  const uint64_t lr = ctx.lr;
  const uint32_t pass = ctx.r3.u32, color = ctx.r4.u32, matrix = ctx.r5.u32;
  const uint32_t frame = ctx.r1.u32 - frame_bytes;
  guest::StoreU32(base, frame, ctx.r1.u32);  // the back chain, as stwu
  MarkerContext m{ctx, base, frame, record_return, memcpy_return, {ctx, base}};
  minimap::MarkerCalls calls;
  calls.atexit = m.atexit.Call();
  calls.record = &MarkerContext::Record;
  calls.memcpy = &MarkerContext::Memcpy;
  calls.user = &m;
  draw(base, pass, color, matrix, frame, calls);
  ctx.lr = lr;
}

}  // namespace

bool MinimapDrawNative() {
  return ITC_FLAG(native_minimap) && hooks::NativeAtexit() && ITC_FLAG(native_scenery_lists) &&
         ITC_FLAG(native_matrix_math) && ITC_FLAG(native_vehicle_accessors) &&
         ITC_FLAG(native_car_physics) && ITC_FLAG(native_math) && ITC_FLAG(native_c_runtime) &&
         !hooks::SceneryListsSlotDiagnostics();
}

void MinimapDraw(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  constexpr uint32_t kFrame = 28336;
  const uint32_t f = ctx.r1.u32 - kFrame;
  hooks::SceneryDrawGuest guest{ctx, base};
  const scenery_lists::DrawCalls draws = hooks::SceneryListsDrawCalls(&guest);
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  // The marker draws' guest calls from their frames below Minimap::Draw's.
  MarkerContext car{ctx, base, f - minimap::kCarMarkerFrame, 0x820F4B94, 0x820F4B84, {ctx, base}};
  MarkerContext point{ctx,        base,       f - minimap::kPointMarkerFrame,
                      0x820F5188, 0x820F5178, {ctx, base}};
  minimap::MarkerCalls car_calls, point_calls;
  for (auto [m, c] : {std::pair{&car, &car_calls}, std::pair{&point, &point_calls}}) {
    c->atexit = m->atexit.Call();
    c->record = &MarkerContext::Record;
    c->memcpy = &MarkerContext::Memcpy;
    c->user = m;
  }
  GuestAtexit atexit{ctx, base};
  minimap::DrawCalls all;
  all.draws = &draws;
  all.calls = &calls;
  all.car_marker = &car_calls;
  all.point_marker = &point_calls;
  all.atexit = atexit.Call();
  minimap::Draw(base, all, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r24.u32);
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

// r3 = the camera pass, r4 = the color, r5 = the matrix; r3 as the last
// Triangle::Record leaves it.
void MinimapDrawCarMarker(PPCContext& ctx, uint8_t* base) {
  DrawMarker(ctx, base, minimap::DrawCarMarker, minimap::kCarMarkerFrame, 0x820F4B94,
             0x820F4B84);
}
void MinimapDrawPointMarker(PPCContext& ctx, uint8_t* base) {
  DrawMarker(ctx, base, minimap::DrawPointMarker, minimap::kPointMarkerFrame, 0x820F5188,
             0x820F5178);
}

}  // namespace hooks

// Triangle::RecordFlat (sub_8217C370) has no adapter: only Triangle::Record
// calls it.

// List80::Link (sub_820F40C0) has no adapter: only List80::Insert calls it.

// Triangle::Init (sub_820F4040) has no adapter: only the marker draws call
// it, and their plain C++ calls its plain C++ (MinimapTriangleInit is the
// harness's register steps).

// Defines an adapter: the flag, else the original.
#define ITC_MINIMAP(address, step)      \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_minimap)) {    \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

ITC_MINIMAP(82170608, MinimapFindPlayerEntry)
ITC_MINIMAP(8217AEA8, MinimapRecordTransform)
ITC_MINIMAP(8217DAA0, MinimapTriangleRecord)

// No adapters (removed 2026-11-13, B7 loop, once Minimap::Draw, their only
// caller in play, was native and calls their plain C++; a temporary counter
// showed none called in an attract race): Race::RacerVehicle (sub_82170578,
// also from Racers::NearestHumanVehicle's plain C++), SetList::Point
// (sub_82147D18), Minimap::RacerMarkerType (sub_820F2D28), Minimap::Scaling
// (sub_820F2EF8), List80::Insert (sub_820F5238), List80::Copy (sub_820F51A8),
// Minimap::DrawCarMarker (sub_820F4888) and DrawPointMarker (sub_820F4BA8).
// Their register steps above stay for the harness.
