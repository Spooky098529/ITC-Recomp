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

// The decal images' textures: DecalImages::FreeTexture sub_821FE6A0 (r3 the
// images, r4 an entry; Texture::Free by car_models' plain C++ with
// --native_car_models and the native heap, else through the guest),
// RestoreVinylTexture sub_82204698 and RestoreStickerTexture sub_821F89B8
// (r4 w, r5 a texture), BindVinylTexture sub_82204730 (r4 an entry, r5 a
// group); TuningScreen::BindVinylTextures sub_82203D40 and EnterEditor
// sub_821D84F8 (r3 the screen).
void TuningPagesFreeTexture(PPCContext& ctx, uint8_t* base);
void TuningPagesRestoreVinylTexture(PPCContext& ctx, uint8_t* base);
void TuningPagesRestoreStickerTexture(PPCContext& ctx, uint8_t* base);
void TuningPagesBindVinylTexture(PPCContext& ctx, uint8_t* base);
void TuningPagesBindVinylTextures(PPCContext& ctx, uint8_t* base);
void TuningPagesEnterEditor(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
