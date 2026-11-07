// The tuning pages' register steps (src/tuning_pages/,
// docs/functions/tuning-pages.md), shared by the adapters
// (src/hooks/tuning_pages.cpp, behind --native_tuning_pages) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// The pages' counts and owned flags (r3 = the screen): LevelCount
// sub_821CC728, TypeCount sub_821CC7B8, PageCursor sub_821CCD38,
// PageItemCount sub_821CCDB0, IsPickOwned sub_821CCE30, MarkOwned
// sub_821CD498, TestAndSetWheelFlag sub_821CDF98 (r4 a wheel number),
// SetWheelListEnd sub_821E7DB0, InchListEnd sub_821EF288.
void TuningPagesLevelCount(PPCContext& ctx, uint8_t* base);
void TuningPagesTypeCount(PPCContext& ctx, uint8_t* base);
void TuningPagesPageCursor(PPCContext& ctx, uint8_t* base);
void TuningPagesPageItemCount(PPCContext& ctx, uint8_t* base);
void TuningPagesIsPickOwned(PPCContext& ctx, uint8_t* base);
void TuningPagesMarkOwned(PPCContext& ctx, uint8_t* base);
void TuningPagesTestAndSetWheelFlag(PPCContext& ctx, uint8_t* base);
void TuningPagesSetWheelListEnd(PPCContext& ctx, uint8_t* base);
void TuningPagesInchListEnd(PPCContext& ctx, uint8_t* base);

// The vinyl editor's set-up: SetUpVinylBusyIcon sub_821FE560,
// KeepListTextures sub_821FE7D8, SetEditorLimits sub_821FE420,
// LoadPageLayouts sub_822045F8 (r3 = the screen); DecalImages::AllLoaded
// sub_821FE5D8 (r3 = the decal images).
void TuningPagesSetUpVinylBusyIcon(PPCContext& ctx, uint8_t* base);
void TuningPagesAllLoaded(PPCContext& ctx, uint8_t* base);
void TuningPagesKeepListTextures(PPCContext& ctx, uint8_t* base);
void TuningPagesSetEditorLimits(PPCContext& ctx, uint8_t* base);
void TuningPagesLoadPageLayouts(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
