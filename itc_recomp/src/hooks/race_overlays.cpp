// The race overlays' update side natively: adapters in front of
// src/race_overlays/race_overlays.cpp (docs/functions/race-overlays.md).

#include "guest.h"

#include "base/flags.h"
#include "race_overlays/race_overlays.h"

ITC_FLAG_BOOL(native_race_overlays, true, "ITC",
              "Run the race overlays' update helpers (the car overlays' view and vehicle "
              "accessors, sub_82126550 to sub_82127178; the child overlays' setters, "
              "sub_8211E9C8 to sub_8212DF90; the game's rand, sub_82293568, and RandomRange, "
              "sub_8211F118; Marker::Set and Update, sub_8212CCE0 and sub_8212CBE0; Trail::Update and "
              "AddPoint, sub_821292E8 and sub_82129598; ParticlesB and ParticlesC::Emit, sub_8212B4A8 and sub_8212C1C0; the "
              "car-model sprites' kind and table entry, sub_82123188 and sub_8211F300; the model "
              "points' checks and positions, sub_82126A78 and sub_82127140; Sparks::UpdateStreak, "
              "sub_8212E660) natively "
              "instead of through the translated code (same results; checked by "
              "--run_harnesses=race_overlays)");

// Defines an adapter for a function of the view or overlays in r3 that
// returns a word in r3: `address` the function's (sub_<address>), `expr` the
// result (using `ctx` and `base`).
#define ITC_OVERLAY_WORD(address, expr)      \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_race_overlays)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    ctx.r3.u64 = uint32_t(expr);            \
  }

// The same for a float returned in f1 (the original's lfs, a single widened
// to double).
#define ITC_OVERLAY_FLOAT(address, expr)     \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_race_overlays)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    ctx.fpscr.disableFlushMode();           \
    ctx.f1.f64 = double(expr);              \
  }

using namespace race_overlays;

// CarOverlays::View: r3 = the car overlays, r4 = a slot.
ITC_OVERLAY_WORD(82126550, View(base, ctx.r3.u32, ctx.r4.u32))
// CarOverlays::ViewportEntry: r4 = an index (r3 isn't read).
ITC_OVERLAY_WORD(821264A8, ViewportEntry(ctx.r4.u32))
// The view's (r3) wheel (r4) values.
ITC_OVERLAY_FLOAT(821268F0, WheelSpin(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_FLOAT(82126908, WheelValue512(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_FLOAT(82126920, AxleValue3764(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_FLOAT(82126958, AxleValue3384(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_WORD(82126998, WheelOnGround(base, ctx.r3.u32, ctx.r4.u32))
// The view's (r3) vehicle's fields.
ITC_OVERLAY_WORD(82126A80, VehicleByte2504(base, ctx.r3.u32))
ITC_OVERLAY_WORD(82126AA0, VehicleWord2252(base, ctx.r3.u32))
ITC_OVERLAY_WORD(82126AB0, VehicleWord340(base, ctx.r3.u32))
ITC_OVERLAY_FLOAT(82126AC0, VehicleFloat6256(base, ctx.r3.u32))
ITC_OVERLAY_FLOAT(82126AD0, VehicleFloat6252(base, ctx.r3.u32))
ITC_OVERLAY_WORD(82126D18, RaceMode(base))
ITC_OVERLAY_WORD(82127178, RacerFlags(base, ctx.r3.u32))

// OverlayView::RacerId: r3 = a view; the racer's id or -1.
extern "C" REX_FUNC(sub_82126AE0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_82126AE0(ctx, base);
    return;
  }
  ctx.r3.s64 = RacerId(base, ctx.r3.u32);
}

// Defines an adapter for a setter returning nothing (`call` uses `ctx` and
// `base`).
#define ITC_OVERLAY_SET(address, call)       \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_race_overlays)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    ctx.fpscr.disableFlushMode();           \
    call;                                   \
  }

// The child overlays' setters: r3 = the overlay, r4 = a vector (or a word).
ITC_OVERLAY_SET(8212A840, SmokeSetVelocity(base, ctx.r3.u32, ctx.r4.u32))
// r5 = the second point, r6 = the view mode.
ITC_OVERLAY_SET(8212A998, SmokeSetRegion(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
ITC_OVERLAY_SET(8212BA70, ParticlesBSetPosition(base, ctx.r3.u32, ctx.r4.u32))
// f1 = speed.
ITC_OVERLAY_SET(8212C6A8, ParticlesCSetPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_OVERLAY_SET(8212DF90, SparksSetPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(821299D8, TrailSetLastPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8212CE18, MarkerSetCameraPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8211EBE0, LightSpriteSetPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(82121560, ModelSpritePlaceHidden(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8212C0E0, SetTextures104(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8212D6D8, SetTextures108(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8212CC10, MarkerSetTextures(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8211E9C8, LightSpriteSetTextures(base, ctx.r3.u32, ctx.r4.u32))
ITC_OVERLAY_SET(8211F408, ModelSpriteSetTextures(base, ctx.r3.u32, ctx.r4.u32))

// rand: returns r3.
ITC_OVERLAY_WORD(82293568, Rand(base))

// RandomRange: f1 = low, f2 = high; returns f1 (r3 is left rand()'s result).
extern "C" REX_FUNC(sub_8211F118) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_8211F118(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t drawn = 0;
  ctx.f1.f64 = double(RandomRange(base, ctx.f1.f64, ctx.f2.f64, &drawn));
  ctx.r3.u64 = drawn;
}

// Marker::Set: r3 = the marker, r4 = position, f1 = distance, r6 = view
// mode, r7 = kind; returns 1.
extern "C" REX_FUNC(sub_8212CCE0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_8212CCE0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      MarkerSet(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32, ctx.r7.u32);
}

// Marker::Update (its vtable's slot 4, so the adapter stays while the
// update loop calling it is recompiled): r3 = the marker.
ITC_OVERLAY_SET(8212CBE0, MarkerUpdate(base, ctx.r3.u32))

// Trail::Update (its vtable's slot 4, so the adapter stays while the update
// loop calling it is recompiled): r3 = the trail. Where the native version
// declines (a capacity of 0 or -1), the original runs.
extern "C" REX_FUNC(sub_821292E8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_821292E8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!TrailUpdate(base, ctx.r3.u32)) {
    __imp__sub_821292E8(ctx, base);
  }
}

// CarOverlays::ModelSpriteKind: r3 = the car overlays (not needed), r4 = a
// racer id; returns the kind.
ITC_OVERLAY_WORD(82123188, ModelSpriteKind(ctx.r4.s32))
// ModelSprite::Entry: r4 = a kind, r5 = an index (r3 isn't read); returns
// the table's word.
ITC_OVERLAY_WORD(8211F300, ModelSpriteEntry(base, ctx.r4.u32, ctx.r5.u32))

// ParticlesC::Emit: r3 = the effect, r4 = a position (f1 isn't read);
// returns 1, or 0 when full.
extern "C" REX_FUNC(sub_8212C1C0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_8212C1C0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ParticlesCEmit(base, ctx.r3.u32, ctx.r4.u32);
}

// Trail::AddPoint: r3 = the trail, r4 = position, r5 and r6 = a vector,
// f1 = speed, f2 = width, r7 = a state, r10 = a value; returns 1 for a
// point added. Where the native version declines (a capacity of 0 or -1),
// the original runs.
extern "C" REX_FUNC(sub_82129598) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_82129598(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t direction[4] = {uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32,
                                 uint32_t(ctx.r6.u64 >> 32), ctx.r6.u32};
  uint32_t result = 0;
  if (!TrailAddPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64, direction,
                     ctx.r7.u32, ctx.r10.u32, &result)) {
    __imp__sub_82129598(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

// ParticlesB::Emit: r3 = the effect, r4 = a position, f1 = a size factor
// (f2 and f3 aren't read); returns 1, or 0 when full. Where the native
// version declines (a capacity of 0 or -1), the original runs.
extern "C" REX_FUNC(sub_8212B4A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_8212B4A8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  double sine = ctx.f1.f64;
  if (!ParticlesBEmit(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, &result, &sine)) {
    __imp__sub_8212B4A8(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
  ctx.f1.f64 = sine;
}

// OverlayView::HasPoint: r3 = a view, r4 = a point index; returns 1 or 0
// (Model::HasPoint and ModelParts::HasPoint, which only it calls in play,
// are done by its plain C++).
ITC_OVERLAY_WORD(82126A78, OverlayViewHasPoint(base, ctx.r3.u32, ctx.r4.s32))

// OverlayView::PointPosition: r3 = out, r4 = a view, r5 = a point index;
// returns out (Model::PointPosition and ModelParts::PointPosition, which
// only it calls in play, are done by its plain C++).
ITC_OVERLAY_WORD(82127140, OverlayViewPointPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32))

// Sparks::UpdateStreak: r3 = the sparks, f1 = speed, r4 and r5 = a position
// (a vector passed by value), r6 = streaking, r8 = a mode (r7 isn't read);
// r3 and f1 are left the last rand() and RandomRange results, as the
// original leaves them.
extern "C" REX_FUNC(sub_8212E660) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_overlays)) {
    __imp__sub_8212E660(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t position[4] = {uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32,
                                uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32};
  uint32_t drawn = ctx.r3.u32;
  double value = ctx.f1.f64;
  SparksUpdateStreak(base, ctx.r3.u32, ctx.f1.f64, position, ctx.r6.u32, ctx.r8.u32, &drawn,
                     &value);
  ctx.r3.u64 = drawn;
  ctx.f1.f64 = value;
}
