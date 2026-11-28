// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 5's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "achievements/achievements.h"
#include "guest.h"
#include "hooks/frame_driver.h"
#include "hooks/save_data.h"
#include "race_overlays/race_overlays.h"
#include "strings_and_font/strings_and_font.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

// The vehicle accessors and vectors.
// CollisionObject::Link and Unlink, and the collision record's copy (r3 left
// as it came: the copy's out).
ITC_DIRECT_ENTRY(820B7BF0) {
  vehicle_accessors::CollisionObjectLink(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(820B7C18) {
  vehicle_accessors::CollisionObjectUnlink(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(820B7D78) {
  vehicle_accessors::CollisionRecordCopy(ctx.r3.u32, ctx.r4.u32);
}
// Vehicle::WheelSlip: r3 the vehicle, r4 the wheel, f1 the threshold; f1.
ITC_DIRECT_ENTRY(820C0180) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = vehicle_accessors::VehicleWheelSlip(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}
// Matrix::SetTranslation: r3 back as the matrix + 48 (its copy's
// destination).
ITC_DIRECT_ENTRY(82293818) {
  const uint32_t matrix = ctx.r3.u32;
  vector_library::MatrixSetTranslation(matrix, ctx.r4.u32);
  ctx.r3.u64 = uint32_t(matrix + 48);
}
// Vec4::DivideBy: r3 *= 1.0 / f1; f1 the inverse and r4 the vector, as the
// original's tail call leaves them.
ITC_DIRECT_ENTRY(82294348) {
  ctx.fpscr.disableFlushMode();
  const uint32_t v = ctx.r3.u32;
  ctx.f1.f64 = vector_library::Vec4DivideBy(v, ctx.f1.f64);
  ctx.r4.u64 = v;
}

// The font and strings.
// Font::LineWidth: r3 the font, r4 the text; f1.
ITC_DIRECT_ENTRY(82108458) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = strings_and_font::FontLineWidth(ctx.r3.u32, ctx.r4.u32);
}
// Font::LineGlyphs: r4 the line; r3 the count.
ITC_DIRECT_ENTRY(82108810) {
  ctx.r3.u64 = strings_and_font::FontLineGlyphs(ctx.r4.u32);
}
// Font::TextWidth: r3 the font, r4 the draw parameters, r5 the text; f1.
ITC_DIRECT_ENTRY(82108B30) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = strings_and_font::FontTextWidth(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// Font::ClipGlyph: r3 the font, r4 the six vertices; r3.
ITC_DIRECT_ENTRY(82108C90) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = strings_and_font::FontClipGlyph(ctx.r3.u32, ctx.r4.u32);
}
// Strings::PageCount: r4 the string (r3 unused); r3.
ITC_DIRECT_ENTRY(8210A5C0) {
  ctx.r3.u64 = strings_and_font::StringsPageCount(ctx.r4.u32);
}

// The race overlays.
// Model::HasPoint: r3 the scene model, r4 the point index; r3.
ITC_DIRECT_ENTRY(820D9DB0) {
  ctx.r3.u64 = race_overlays::ModelHasPoint(ctx.r3.u32, ctx.r4.s32);
}
// Model::PointPosition: r3 out, r4 the scene model, r5 the index; r3 out.
ITC_DIRECT_ENTRY(820DA988) {
  ctx.r3.u64 = race_overlays::ModelPointPosition(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32);
}
// Smoke::Emit: r3 the smoke, f1 the size, f2 the speed, r5 the frame, r7 the
// region flag, r8 the value; r3 the result, f1 the last sine (as it came
// when full). Declined (a capacity of 0 or -1): the original.
ITC_DIRECT_ENTRY(8212A350) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  double last = ctx.f1.f64;
  if (!race_overlays::SmokeEmit(ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r5.u32, ctx.r7.u32,
                                ctx.r8.u32, &result, &last)) {
    lifted::original::c8212A350(ctx);
    return;
  }
  ctx.r3.u64 = result;
  ctx.f1.f64 = last;
}
// RaceOverlays::Clear: r3 the list (left as it came).
ITC_DIRECT_ENTRY(8215DE80) { race_overlays::RaceOverlaysClear(ctx.r3.u32); }

// The achievements.
// Achievements::AllBeaten: r3 the list of item ids, r4 the count; r3.
ITC_DIRECT_ENTRY(82290698) {
  ctx.r3.u64 = achievements::AllBeaten(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
// Achievements::BeatsCourseTime: r3 &the route, r4 &the packed time; r3.
ITC_DIRECT_ENTRY(82290710) {
  ctx.r3.u64 = achievements::BeatsCourseTime(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

// The save data: ControlMap::SetDefault (r3 left as it came).
ITC_DIRECT_ENTRY(821058A8) { hooks::SaveDataControlMapSetDefault(ctx); }

// The frame driver: CreateThread (with a processor).
ITC_DIRECT_ENTRY(825BA380) { hooks::CreateThreadStep(ctx); }
