// The tuning screen's pages natively: adapters in front of src/tuning_pages/
// (docs/functions/tuning-pages.md), with the register steps the harness
// shares (hooks/tuning_pages.h). The pages and the screen's update are still
// recompiled and call these, so the adapters stay.

#include "hooks/tuning_pages.h"

#include "base/flags.h"
#include "tuning_pages/tuning_pages.h"

ITC_FLAG_BOOL(native_tuning_pages, true, "ITC",
              "Run the tuning screen's pages' functions (docs/functions/tuning-pages.md: "
              "TuningScreen::LevelCount sub_821CC728, TypeCount sub_821CC7B8, PageCursor "
              "sub_821CCD38 (its piece sub_821CCD70 merged), PageItemCount sub_821CCDB0 (its piece "
              "sub_821CCDE8 merged), IsPickOwned sub_821CCE30, MarkOwned sub_821CD498 (its piece "
              "sub_821CD6C8 merged), TestAndSetWheelFlag sub_821CDF98, SetWheelListEnd "
              "sub_821E7DB0 (its piece sub_821E7DE4 merged), InchListEnd sub_821EF288 (its piece "
              "sub_821EF2FC merged); the vinyl editor's set-up SetUpVinylBusyIcon sub_821FE560, "
              "DecalImages::AllLoaded sub_821FE5D8, KeepListTextures sub_821FE7D8, "
              "SetEditorLimits sub_821FE420 and LoadPageLayouts sub_822045F8) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=tuning_pages)");

namespace hooks {

namespace tp = tuning_pages;

void TuningPagesLevelCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::LevelCount(base, ctx.r3.u32);
}

void TuningPagesTypeCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::TypeCount(base, ctx.r3.u32);
}

void TuningPagesPageCursor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::PageCursor(base, ctx.r3.u32);
}

void TuningPagesPageItemCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::PageItemCount(base, ctx.r3.u32);
}

void TuningPagesIsPickOwned(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::IsPickOwned(base, ctx.r3.u32);
}

void TuningPagesMarkOwned(PPCContext& ctx, uint8_t* base) { tp::MarkOwned(base, ctx.r3.u32); }

void TuningPagesTestAndSetWheelFlag(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::TestAndSetWheelFlag(base, ctx.r3.u32, ctx.r4.u64);
}

void TuningPagesSetWheelListEnd(PPCContext& ctx, uint8_t* base) {
  tp::SetWheelListEnd(base, ctx.r3.u32);
}

void TuningPagesInchListEnd(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::InchListEnd(base, ctx.r3.u32);
}

void TuningPagesSetUpVinylBusyIcon(PPCContext& ctx, uint8_t* base) {
  tp::SetUpVinylBusyIcon(base, ctx.r3.u32);
}

void TuningPagesAllLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::AllLoaded(base, ctx.r3.u32);
}

void TuningPagesKeepListTextures(PPCContext& ctx, uint8_t* base) {
  tp::KeepListTextures(base, ctx.r3.u32);
}

void TuningPagesSetEditorLimits(PPCContext& ctx, uint8_t* base) {
  tp::SetEditorLimits(base, ctx.r3.u32);
}

void TuningPagesLoadPageLayouts(PPCContext& ctx, uint8_t* base) {
  tp::LoadPageLayouts(base, ctx.r3.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_TUNING_PAGES(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_tuning_pages)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_TUNING_PAGES(821CC728, TuningPagesLevelCount)
ITC_TUNING_PAGES(821CC7B8, TuningPagesTypeCount)
ITC_TUNING_PAGES(821CCD38, TuningPagesPageCursor)
ITC_TUNING_PAGES(821CCDB0, TuningPagesPageItemCount)
ITC_TUNING_PAGES(821CCE30, TuningPagesIsPickOwned)
ITC_TUNING_PAGES(821CD498, TuningPagesMarkOwned)
ITC_TUNING_PAGES(821CDF98, TuningPagesTestAndSetWheelFlag)
ITC_TUNING_PAGES(821E7DB0, TuningPagesSetWheelListEnd)
ITC_TUNING_PAGES(821EF288, TuningPagesInchListEnd)
ITC_TUNING_PAGES(821FE560, TuningPagesSetUpVinylBusyIcon)
ITC_TUNING_PAGES(821FE5D8, TuningPagesAllLoaded)
ITC_TUNING_PAGES(821FE7D8, TuningPagesKeepListTextures)
ITC_TUNING_PAGES(821FE420, TuningPagesSetEditorLimits)
ITC_TUNING_PAGES(822045F8, TuningPagesLoadPageLayouts)
