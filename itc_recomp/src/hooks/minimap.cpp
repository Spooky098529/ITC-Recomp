// The minimap's parts natively: adapters in front of src/minimap/
// (docs/functions/minimap.md), with the register steps the harness shares
// (hooks/minimap.h). Minimap::Draw (sub_820F5740) itself stays recompiled,
// Wrapped by frame interpolation (src/frame_pacing/per_frame_logic.cpp).

#include "hooks/minimap.h"

#include "base/flags.h"
#include "minimap/minimap.h"

ITC_FLAG_BOOL(native_minimap, true, "ITC",
              "Run the minimap's parts (docs/functions/minimap.md: Race::FindPlayerEntry "
              "sub_82170608, Race::RacerVehicle sub_82170578, SetList::Point sub_82147D18, "
              "Minimap::RacerMarkerType sub_820F2D28 (with its switch's case 1, sub_820F2E68; "
              "sub_820F2CA8's chained record the original's), Minimap::Scaling sub_820F2EF8, "
              "Triangle::Init sub_820F4040, List80::Insert sub_820F5238 (with List80::Link sub_820F40C0, no adapter: only it calls it) and "
              "Copy sub_820F51A8, Primitive::RecordTransform sub_8217AEA8) natively instead "
              "of through the translated code (the same writes; checked by "
              "--run_harnesses=minimap)");

// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the original runs.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

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

}  // namespace hooks

// List80::Link (sub_820F40C0) has no adapter: only List80::Insert calls it.

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
ITC_MINIMAP(82170578, MinimapRacerVehicle)
ITC_MINIMAP(82147D18, MinimapSetListPoint)
ITC_MINIMAP(820F2D28, MinimapRacerMarkerType)
ITC_MINIMAP(820F2EF8, MinimapScaling)
ITC_MINIMAP(820F4040, MinimapTriangleInit)
ITC_MINIMAP(820F5238, MinimapList80Insert)
ITC_MINIMAP(820F51A8, MinimapList80Copy)
ITC_MINIMAP(8217AEA8, MinimapRecordTransform)
