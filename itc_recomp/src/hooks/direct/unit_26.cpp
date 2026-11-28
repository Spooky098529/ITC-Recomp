// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 26's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "collision/collision.h"
#include "file_layer/file_layer.h"
#include "game_modes/game_modes.h"
#include "hooks/achievements.h"
#include "hooks/career.h"
#include "hooks/location_screen.h"
#include "hooks/stand_models.h"
#include "hooks/tuning_pages.h"
#include "input/input_manager.h"
#include "strings_and_font/strings_and_font.h"

// --- Shims over the areas' register steps (each step maps the registers
// and builds the area's calls; some run the original where the port
// declines). ---

// The tuning screen's pages.
ITC_DIRECT_ENTRY(821CCD38) { hooks::TuningPagesPageCursor(ctx); }
ITC_DIRECT_ENTRY(821CCDB0) { hooks::TuningPagesPageItemCount(ctx); }
ITC_DIRECT_ENTRY(821CCE30) { hooks::TuningPagesIsPickOwned(ctx); }
ITC_DIRECT_ENTRY(821CE188) { hooks::TuningPagesIsSlotAvailable(ctx); }
ITC_DIRECT_ENTRY(821CE1D0) { hooks::TuningPagesStepList(ctx); }
ITC_DIRECT_ENTRY(821E1DB0) { hooks::TuningPagesSetCurrentChoice(ctx); }
ITC_DIRECT_ENTRY(821E9488) { hooks::TuningPagesSetPaintGradients(ctx); }
ITC_DIRECT_ENTRY(821EA130) { hooks::TuningPagesSetLightKind0Bars(ctx); }
ITC_DIRECT_ENTRY(821EAE68) { hooks::TuningPagesSetLightKind1Bar(ctx); }

// The parking area (the location screen).
ITC_DIRECT_ENTRY(8222A150) { hooks::LocationScreenHidePictures(ctx); }
ITC_DIRECT_ENTRY(8222AC18) { hooks::LocationScreenSetCarColor(ctx); }
ITC_DIRECT_ENTRY(8222B200) { hooks::LocationScreenShowOpponentPictures(ctx); }
ITC_DIRECT_ENTRY(8222B438) { hooks::LocationScreenShowOpponentInfo(ctx); }
ITC_DIRECT_ENTRY(8222BFE8) { hooks::LocationScreenSetCursor(ctx); }
ITC_DIRECT_ENTRY(8222C058) { hooks::LocationScreenListNode(ctx); }
ITC_DIRECT_ENTRY(8222C0C8) { hooks::LocationScreenShowArrows(ctx); }
ITC_DIRECT_ENTRY(8222C188) { hooks::LocationScreenShowOpponentSpecs(ctx); }
ITC_DIRECT_ENTRY(8222CD60) { hooks::LocationScreenResetMovie(ctx); }

// The career.
ITC_DIRECT_ENTRY(82110B18) { hooks::CareerStatsAddPoints(ctx); }
ITC_DIRECT_ENTRY(82116588) { hooks::CareerAfterRace(ctx); }

// The achievements.
ITC_DIRECT_ENTRY(82290788) { hooks::AchievementsCheckSteps(ctx); }
ITC_DIRECT_ENTRY(82290A30) { hooks::AchievementsWriteSteps(ctx); }

// GarageStage::MakeModel.
ITC_DIRECT_ENTRY(8217FCB0) { hooks::StandModelsMakeModel(ctx); }

// --- Shims mapping the registers themselves (the deleted adapters'
// mappings). ---

// BoxShape::SetCorners: r3 = the box, r4 = the min corner, r5 = the max
// corner, its 224-byte frame; r3 comes back as the box + 96, f1 as the last
// face's distance.
ITC_DIRECT_ENTRY(820B99D0) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = collision::BoxSetCorners(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 224);
  ctx.r3.u64 = ctx.r3.u32 + 96;
}
// BoxShape::FaceDistance: r3 = the box, r4 = the face, r5 = the point; f1
// the distance, r3 comes back as the face's normal.
ITC_DIRECT_ENTRY(820BA3B0) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  ctx.f1.f64 = collision::BoxFaceDistance(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &r3);
  ctx.r3.u64 = r3;
}
// BoxShape::BoxShape(const&): r3 = the box, r4 = the box to copy (r3
// left). The original where the memcpys can't be native.
ITC_DIRECT_ENTRY(820BB688) {
  ctx.fpscr.disableFlushMode();
  if (!collision::BoxCopyConstruct(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820BB688(ctx);
  }
}
// Contact::Set: r3 = the contact, r4 = the point, r5 = the normal, f1 = the
// depth, f2 = the time; r3 comes back as the contact + 32.
ITC_DIRECT_ENTRY(820BC908) {
  ctx.fpscr.disableFlushMode();
  collision::ContactSet(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.f2.f64);
  ctx.r3.u64 = ctx.r3.u32 + 32;
}
// BoxBox::SwapForImpact and SwapForPushOut: r3 = the query; r3 comes back
// as the query + 64 (the ports' result).
ITC_DIRECT_ENTRY(820BE490) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = collision::BoxBoxSwapForImpact(ctx.r3.u32);
}
ITC_DIRECT_ENTRY(820BEBF0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = collision::BoxBoxSwapForPushOut(ctx.r3.u32);
}

// The file layer's constructors and destructors: r3 = the object, left as
// it is (the originals return it).
ITC_DIRECT_ENTRY(8258C5F8) { file_layer::FileDestruct(ctx.r3.u32); }
ITC_DIRECT_ENTRY(82593B00) { file_layer::AsyncFileDestruct(ctx.r3.u32); }
ITC_DIRECT_ENTRY(82593C28) { file_layer::PathConstruct(ctx.r3.u32); }
ITC_DIRECT_ENTRY(82593C48) { file_layer::PathDestruct(ctx.r3.u32); }
// Path::Build: r3 = the path, r4 = the kind, r5 = the name, r1 the stack at
// entry; r3 the text (or 0 for a null name). The original where sprintf
// declines.
ITC_DIRECT_ENTRY(82593CA0) {
  uint32_t result = 0;
  if (!file_layer::PathBuild(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &result)) {
    lifted::original::c82593CA0(ctx);
    return;
  }
  ctx.r3.u64 = result;
}

// Font::LineY: r3 = the font, r4 = the line count, r5 = the line; f1 its y
// (as it came for an alignment above 3).
ITC_DIRECT_ENTRY(82108568) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = strings_and_font::FontLineY(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64);
}
// Font::LineX: r3 = the font, r4 = the line; f1 its x. (The original's r3
// after justified text is the glyph count; no caller reads it.)
ITC_DIRECT_ENTRY(82108FE0) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = strings_and_font::FontLineX(ctx.r3.u32, ctx.r4.u32);
}
// Font::AddGlyph: r3 = the font, r6 = the character, r7 = the color, f1, f2
// = x, y. The original for rotated text. (The original's r3 after is
// ClipGlyph's or a count; its only caller, Font::BuildVertices, reads none.)
ITC_DIRECT_ENTRY(82109560) {
  ctx.fpscr.disableFlushMode();
  if (!strings_and_font::FontAddGlyph(ctx.r3.u32, ctx.r6.u32, ctx.r7.u32, ctx.f1.f64,
                                      ctx.f2.f64)) {
    lifted::original::c82109560(ctx);
  }
}

// InputProcessor::Assign: r3 = the destination, r4 = the source (r3 left).
ITC_DIRECT_ENTRY(82098D60) { input::InputProcessorAssign(ctx.r3.u32, ctx.r4.u32); }

// Mode::Mode: r3 = the mode (left).
ITC_DIRECT_ENTRY(82106C20) { game_modes::ModeConstruct(ctx.r3.u32); }
