// The race overlays' update side natively: adapters in front of
// src/race_overlays/race_overlays.cpp (docs/functions/race-overlays.md).

#include "guest.h"

#include "base/flags.h"
#include "race_overlays/race_overlays.h"

ITC_FLAG_BOOL(native_race_overlays, true, "ITC",
              "Run the race overlays' update helpers (the car overlays' view and vehicle "
              "accessors, sub_82126550 to sub_82127178; the child overlays' setters, "
              "sub_8211E9C8 to sub_8212DF90) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=race_overlays)");

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
