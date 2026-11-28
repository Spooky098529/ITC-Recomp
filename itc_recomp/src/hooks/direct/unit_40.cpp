// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 40's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "hooks/archive.h"
#include "race_overlays/race_overlays.h"
#include "schema/race_overlays.h"
#include "shader_objects/shader_objects.h"
#include "strings_and_font/strings_and_font.h"

// --- The race overlays (src/race_overlays/; the removed adapters' register
// maps) ---

namespace ro = race_overlays;

// CarOverlays::EmitSmoke: r3 = the car overlays, f1 = a rate, f2 = speed,
// r5 = the frame, r6 = the wheel, r8 = a value (r4 and r7 aren't read); r3 0,
// f1 as the original leaves it. The original where the port declines.
ITC_DIRECT_ENTRY(82123240) {
  ctx.fpscr.disableFlushMode();
  double last = ctx.f1.f64;
  if (!ro::EmitSmoke(ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r5.u32, ctx.r6.u32, ctx.r8.u32,
                     &last)) {
    lifted::original::c82123240(ctx);
    return;
  }
  ctx.r3.u64 = 0;
  ctx.f1.f64 = last;
}

// The view's (r3) wheel (r4) values: words in r3, floats in f1.
ITC_DIRECT_ENTRY(821268C8) { ctx.r3.u64 = uint32_t(ro::WheelContact(ctx.r3.u32, ctx.r4.u32)); }
ITC_DIRECT_ENTRY(821268F0) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(ro::WheelSpin(ctx.r3.u32, ctx.r4.u32));
}
ITC_DIRECT_ENTRY(82126908) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(ro::WheelValue512(ctx.r3.u32, ctx.r4.u32));
}
ITC_DIRECT_ENTRY(82126920) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(ro::AxleValue3764(ctx.r3.u32, ctx.r4.u32));
}
ITC_DIRECT_ENTRY(82126958) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(ro::AxleValue3384(ctx.r3.u32, ctx.r4.u32));
}
ITC_DIRECT_ENTRY(82126998) { ctx.r3.u64 = uint32_t(ro::WheelOnGround(ctx.r3.u32, ctx.r4.u32)); }
ITC_DIRECT_ENTRY(821269B0) {
  ctx.r3.u64 = uint32_t(ro::WheelSurfaceMarks(ctx.r3.u32, ctx.r4.u32));
}
// OverlayView::WheelSlip: f1 = a threshold. The original loads the vehicle
// into r3 and tail-calls Vehicle::WheelSlip, so r3 comes back as the vehicle.
ITC_DIRECT_ENTRY(82126A70) {
  ctx.fpscr.disableFlushMode();
  const uint32_t view = ctx.r3.u32;
  ctx.r3.u64 = schema::OverlayView(view).vehicle();
  ctx.f1.f64 = double(ro::WheelSlip(view, ctx.r4.u32, ctx.f1.f64));
}
// OverlayView::HasPoint: r4 = a point index; 1 or 0.
ITC_DIRECT_ENTRY(82126A78) {
  ctx.r3.u64 = uint32_t(ro::OverlayViewHasPoint(ctx.r3.u32, ctx.r4.s32));
}
// The view's (r3) vehicle's floats.
ITC_DIRECT_ENTRY(82126AC0) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(ro::VehicleFloat6256(ctx.r3.u32));
}
ITC_DIRECT_ENTRY(82126AD0) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(ro::VehicleFloat6252(ctx.r3.u32));
}
// OverlayView::PointPosition: r3 = out, r4 = a view, r5 = a point index;
// returns out.
ITC_DIRECT_ENTRY(82127140) {
  ctx.r3.u64 = uint32_t(ro::OverlayViewPointPosition(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32));
}
// Trail::AddPoint: r3 = the trail, r4 = position, r5 and r6 = a vector,
// f1 = speed, f2 = width, r7 = a state, r10 = a value; r3 1 for a point
// added. The original where the port declines.
ITC_DIRECT_ENTRY(82129598) {
  ctx.fpscr.disableFlushMode();
  const uint32_t direction[4] = {uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32,
                                 uint32_t(ctx.r6.u64 >> 32), ctx.r6.u32};
  uint32_t result = 0;
  if (!ro::TrailAddPoint(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64, direction,
                         ctx.r7.u32, ctx.r10.u32, &result)) {
    lifted::original::c82129598(ctx);
    return;
  }
  ctx.r3.u64 = result;
}
// The setters: r3 = the object, r4 = a vector (no return value).
ITC_DIRECT_ENTRY(821299D8) {
  ctx.fpscr.disableFlushMode();
  ro::TrailSetLastPosition(ctx.r3.u32, ctx.r4.u32);
}
// SmokeEmitter::Reset and ParticlesB::ResetEntry: r4 = the emitter or entry
// (r3, the owner, isn't read).
ITC_DIRECT_ENTRY(8212A218) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeEmitterReset(ctx.r4.u32);
}
ITC_DIRECT_ENTRY(8212A840) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeSetVelocity(ctx.r3.u32, ctx.r4.u32);
}
// Smoke::InRegion: r4 = a point; 1 or 0.
ITC_DIRECT_ENTRY(8212A868) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = uint32_t(ro::SmokeInRegion(ctx.r3.u32, ctx.r4.u32));
}
// Smoke::SetRegion: r5 = the second point, r6 = the view mode.
ITC_DIRECT_ENTRY(8212A998) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeSetRegion(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
// Smoke::SetFrame: r3 = the smoke, r4 = the frame (no return value read).
// The original where the port declines.
ITC_DIRECT_ENTRY(8212B1B0) {
  ctx.fpscr.disableFlushMode();
  if (!ro::SmokeSetFrame(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c8212B1B0(ctx);
  }
}
// Smoke::~Smoke and ParticlesB::~ParticlesB: r3 = the object, left as it
// came (the base destructor's r3, as the originals leave it).
ITC_DIRECT_ENTRY(8212B288) { ro::SmokeDestruct(ctx.r3.u32); }
ITC_DIRECT_ENTRY(8212B3E8) {
  ctx.fpscr.disableFlushMode();
  ro::ParticlesBResetEntry(ctx.r4.u32);
}
// ParticlesB::Emit: r3 = the effect, r4 = a position, f1 = a (f2 and f3
// aren't read); r3 1, or 0 when full, f1 the last sine. The original where
// the port declines.
ITC_DIRECT_ENTRY(8212B4A8) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  double sine = ctx.f1.f64;
  if (!ro::ParticlesBEmit(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, &result, &sine)) {
    lifted::original::c8212B4A8(ctx);
    return;
  }
  ctx.r3.u64 = result;
  ctx.f1.f64 = sine;
}
ITC_DIRECT_ENTRY(8212BA70) {
  ctx.fpscr.disableFlushMode();
  ro::ParticlesBSetPosition(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(8212BF88) { ro::ParticlesBDestruct(ctx.r3.u32); }

// --- The string table and the font (src/strings_and_font/) ---

namespace sf = strings_and_font;

// Font::IsLoaded: r3 = the font; r3 0 or 1.
ITC_DIRECT_ENTRY(821081A0) { ctx.r3.u64 = sf::FontIsLoaded(ctx.r3.u32); }
// Font::CountGlyphs: r3 = the font, r4 = the four counts, r5 = the text; r3
// the text's length.
ITC_DIRECT_ENTRY(82108210) {
  ctx.r3.u64 = sf::FontCountGlyphs(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
// Font::SetParams: r3 = the font, r4 = the draw parameters. Flush-to-zero
// off first, as its lfs sets it.
ITC_DIRECT_ENTRY(82108368) {
  ctx.fpscr.disableFlushMode();
  sf::FontSetParams(ctx.r3.u32, ctx.r4.u32);
}
// Font::Load: r3 = the font, r4 = the mask; r3 the last load's serial (left
// as it came when nothing was loaded), as Strings::Load's adapter. The
// original where the port hands back (never: the pages are host objects).
ITC_DIRECT_ENTRY(82109498) {
  const sf::FontLoadResult result =
      sf::FontLoad(ctx.r3.u32, ctx.r4.u32, hooks::NativeArchiveLoad());
  if (result.handed_back) {
    lifted::original::c82109498(ctx);
    return;
  }
  if (result.last) {
    hooks::SetArchiveSerial(ctx, *result.last);
  }
}
// Font::BuildVertices: r3 = the font, r4 = the text; r3 0. The original for
// rotated text (where the port declines).
ITC_DIRECT_ENTRY(82109AA8) {
  ctx.fpscr.disableFlushMode();
  if (!sf::FontBuildVertices(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c82109AA8(ctx);
    return;
  }
  ctx.r3.u64 = 0;
}
// Strings::FormatGrouped: r4 = the value, r5 = the output; r3 the output,
// its frame where the original's would be (640 bytes down). Flush-to-zero
// off, as its lfd leaves it. The original where the port declines.
ITC_DIRECT_ENTRY(8210A680) {
  ctx.fpscr.disableFlushMode();
  if (!sf::StringsFormatGrouped(ctx.r4.s32, ctx.r5.u32, ctx.r1.u32 - 640)) {
    lifted::original::c8210A680(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r5.u32;
}
// Strings::IsLoaded: r3 = the table; r3 0 or 1.
ITC_DIRECT_ENTRY(8210A768) { ctx.r3.u64 = sf::StringsIsLoaded(ctx.r3.u32); }
// Strings::Get: r3 = the table, r4 = the id (its low 16 bits); r3 the
// string, or left for an id past the last bound.
ITC_DIRECT_ENTRY(8210A800) {
  if (const auto text = sf::StringsGet(ctx.r3.u32, ctx.r4.u32 & 0xFFFF)) {
    ctx.r3.u64 = *text;
  }
}
// Strings::Init: no arguments; r3 the table when it allocates it, else left.
ITC_DIRECT_ENTRY(8210A860) {
  const uint32_t table = sf::StringsInit();
  if (table != 0) {
    ctx.r3.u64 = table;
  }
}

// --- The shader objects (src/shader_objects/) ---

// ShaderFile::Parse: r3 = the file, r4 = the record. No return value.
// (ParseFile is the public form of the port; ReadFile is its private
// helper returning the record.)
ITC_DIRECT_ENTRY(822A8568) { shader_objects::ParseFile(ctx.r3.u32, ctx.r4.u32); }
// PixelShader::Init: r3 = the object, r4 = the microcode. No return value.
ITC_DIRECT_ENTRY(822A85B8) { shader_objects::PixelShaderInit(ctx.r3.u32, ctx.r4.u32); }
// VertexShader::Init: r3 = the object, r4 = the microcode. No return value.
// The original where the port declines.
ITC_DIRECT_ENTRY(822A8740) {
  if (!shader_objects::VertexShaderInit(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c822A8740(ctx);
  }
}
// PixelShader::CopyHeader and VertexShader::CopyHeader: r3 = the object, r5
// = the file's record (r4 unread). No return value. The original where the
// port declines.
ITC_DIRECT_ENTRY(822A9EA8) {
  if (!shader_objects::CopyPixelHeader(ctx.r3.u32, ctx.r5.u32)) {
    lifted::original::c822A9EA8(ctx);
  }
}
ITC_DIRECT_ENTRY(822A9FD8) {
  if (!shader_objects::CopyVertexHeader(ctx.r3.u32, ctx.r5.u32)) {
    lifted::original::c822A9FD8(ctx);
  }
}
