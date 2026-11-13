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
// SetWheelListEnd sub_821E7DB0, InchListEnd sub_821EF288 (PageCursor,
// PageItemCount and IsPickOwned with no adapters: their callers in play are
// native; these steps are the harness's).
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
// DecalImages::RestoreMovieTexture sub_822081A0 (r4 w, r5 a texture),
// RestoreVinylMovie sub_822081D8 and RestoreStickerMovie sub_821F6DD8 (r3
// the images).
void TuningPagesRestoreMovieTexture(PPCContext& ctx, uint8_t* base);
void TuningPagesRestoreVinylMovie(PPCContext& ctx, uint8_t* base);
void TuningPagesRestoreStickerMovie(PPCContext& ctx, uint8_t* base);
// With the native heap active (else the originals): DecalImages::Update
// sub_821FE628 (Texture::FromTga sub_820ED528 under it) and UnloadAll
// sub_82208738 (DecalImages::Unload sub_821FEA68 under it; Texture::Free by
// car_models' plain C++ with --native_car_models), r3 the images.
void TuningPagesUpdateImages(PPCContext& ctx, uint8_t* base);
void TuningPagesUnloadAll(PPCContext& ctx, uint8_t* base);

// The vinyls' records (r3 = the screen; flush-to-zero off): StoreVinyls
// sub_82203F20, LoadVinyls sub_822040A8 (memcpy through the guest where
// Buffers_Memcpy declines).
void TuningPagesStoreVinyls(PPCContext& ctx, uint8_t* base);
void TuningPagesLoadVinyls(PPCContext& ctx, uint8_t* base);

// Movie 2's nodes (r3 = the screen): ShowPricePanel sub_821D9E68,
// SetBrandLogo sub_821EED98; Color::HsvToRgb sub_8224AB50 (r4 out, r5 in).
void TuningPagesShowPricePanel(PPCContext& ctx, uint8_t* base);
void TuningPagesSetBrandLogo(PPCContext& ctx, uint8_t* base);
void TuningPagesHsvToRgb(PPCContext& ctx, uint8_t* base);

// The current choices (r3 = the screen): SetAeroAvailability sub_821DE160,
// SetCurrentChoice sub_821E1DB0 (Params::GetCarParts and AeroParts::Get82 by
// their plain C++ with --native_car_specs, else through the guest).
void TuningPagesSetAeroAvailability(PPCContext& ctx, uint8_t* base);
void TuningPagesSetCurrentChoice(PPCContext& ctx, uint8_t* base);

// The decal images' loads: TuningScreen::LoadDecalImages sub_821FEE10 (r3
// the screen), DecalImages::Request sub_821FE830 and Load sub_821F6D40 (r3
// the images, r4 an id, r5 an entry).
void TuningPagesLoadDecalImages(PPCContext& ctx, uint8_t* base);
void TuningPagesRequestImage(PPCContext& ctx, uint8_t* base);
void TuningPagesLoadImage(PPCContext& ctx, uint8_t* base);

// The input pages: Menu::StepList sub_821CE1D0 (r3 the menu; no adapter:
// only InputItems and InputLevels call it in play, this step is the harness's),
// TuningScreen::LeaveEditor sub_821D8670 and InputCategories sub_821D6AD0
// (r3 the screen; the mappers' slots by their plain C++ with
// --native_action_mappers, DecalImages::UnloadAll by its plain C++ with the
// native heap).
void TuningPagesStepList(PPCContext& ctx, uint8_t* base);
void TuningPagesLeaveEditor(PPCContext& ctx, uint8_t* base);
void TuningPagesInputCategories(PPCContext& ctx, uint8_t* base);
// BackToItems sub_821D87C0, InputWheelParts sub_821EE390 and InputInches
// sub_821EEEA8 (r3 the screen), their movie sides AnimateWheelParts
// sub_821E8CE0 and AnimateInches sub_821E9110 (no adapters: only their pages
// call them; these steps are the harness's).
void TuningPagesBackToItems(PPCContext& ctx, uint8_t* base);
void TuningPagesInputWheelParts(PPCContext& ctx, uint8_t* base);
void TuningPagesAnimateWheelParts(PPCContext& ctx, uint8_t* base);
void TuningPagesInputInches(PPCContext& ctx, uint8_t* base);
void TuningPagesAnimateInches(PPCContext& ctx, uint8_t* base);
// InputItems sub_821DFD48 (r3 the screen) and its movie side MoveItem
// sub_821D0CC0 (no adapter: only InputItems calls it; this step is the
// harness's).
void TuningPagesInputItems(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveItem(PPCContext& ctx, uint8_t* base);
// InputOptions sub_821D8160 (r3 the screen) and its movie side MoveOption
// sub_821D5610 (no adapter: only InputOptions calls it; this step is the
// harness's).
void TuningPagesInputOptions(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveOption(PPCContext& ctx, uint8_t* base);
// MoveLevel sub_821D1A18 (r3 the screen), state 5's movie side (no adapter:
// only InputLevels calls it; this step is the harness's).
void TuningPagesMoveLevel(PPCContext& ctx, uint8_t* base);
// InputLevels sub_821D6E20 (r3 the screen), state 5's page.
void TuningPagesInputLevels(PPCContext& ctx, uint8_t* base);
// MoveType sub_821D32C8 (r3 the screen), state 6's movie side (no adapter:
// only InputTypes calls it; this step is the harness's).
void TuningPagesMoveType(PPCContext& ctx, uint8_t* base);
// InputTypes sub_821D74A0 (r3 the screen), state 6's page.
void TuningPagesInputTypes(PPCContext& ctx, uint8_t* base);
// Leave sub_821E25B0 (r3 the screen, r4 the object with the message ring)
// and StoreOwnedParts sub_821DD408 (no adapter: only Leave calls it; this
// step is the harness's).
void TuningPagesLeave(PPCContext& ctx, uint8_t* base);
void TuningPagesStoreOwnedParts(PPCContext& ctx, uint8_t* base);
// InputWheels sub_821F6688 (r3 the screen), its movie side AnimateWheels
// sub_821F60E0 and the list's rows FillPartRowsRight sub_821F40B8 and
// FillPartRowsLeft sub_821F4768 under it (no adapters: only InputWheels and
// AnimateWheels call them; these steps are the harness's).
void TuningPagesInputWheels(PPCContext& ctx, uint8_t* base);
void TuningPagesAnimateWheels(PPCContext& ctx, uint8_t* base);
void TuningPagesFillPartRowsRight(PPCContext& ctx, uint8_t* base);
void TuningPagesFillPartRowsLeft(PPCContext& ctx, uint8_t* base);
// The confirm dialogs' yes answers (r3 the screen): OnDialogYes sub_821E2660
// (SetCurrentChoice's Params::GetCarParts and AeroParts::Get82 by their plain
// C++ with --native_car_specs), OnStickerDialogYes sub_821FA230 (no adapter:
// only OnDialogYes calls it; this step is the harness's).
void TuningPagesOnDialogYes(PPCContext& ctx, uint8_t* base);
void TuningPagesOnStickerDialogYes(PPCContext& ctx, uint8_t* base);

// The vinyl editor's loads: DecalImages::OnLoaded sub_821FE608 (r3 a
// handle, r6 an entry), Reload sub_821FE960 (r4 an id, r5 an entry),
// LoadWindow sub_821FEAF0 (r4 the first id, r5 the first entry, r6 the
// count, r7 the list), Scroll sub_821FED30 (r4 the direction, r5 the first
// entry, r6 the count, r7 the id) and FreeKept sub_821FE740 (r4 a kept
// texture; Texture::Free by car_models' plain C++ with --native_car_models
// and the native heap); r3 the images.
void TuningPagesOnImageLoaded(PPCContext& ctx, uint8_t* base);
void TuningPagesReloadImage(PPCContext& ctx, uint8_t* base);
void TuningPagesLoadWindow(PPCContext& ctx, uint8_t* base);
void TuningPagesScrollImages(PPCContext& ctx, uint8_t* base);
void TuningPagesFreeKept(PPCContext& ctx, uint8_t* base);

// The vinyl editor's pages (r3 = the screen): InputVinylView sub_82202928
// and LeaveVinylView sub_82201B40 (no adapter: the harness's step).
void TuningPagesInputVinylView(PPCContext& ctx, uint8_t* base);
void TuningPagesLeaveVinylView(PPCContext& ctx, uint8_t* base);
// InputVinylPages sub_82204B70 with SetLastImage sub_821FE498 and
// MoveVinylMenu sub_821FF3A0 (no adapters: the harness's steps).
void TuningPagesInputVinylPages(PPCContext& ctx, uint8_t* base);
void TuningPagesSetLastImage(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveVinylMenu(PPCContext& ctx, uint8_t* base);
// The decal edits: InputDecalPosition sub_82206A50, InputDecalAngle
// sub_82206F48, InputDecalScale sub_82207320, and their movie sides
// MoveDecalPosition sub_82200DC0, MoveDecalAngle sub_822012E8,
// MoveDecalScale sub_82201620 (no adapters: the harness's steps).
void TuningPagesInputDecalPosition(PPCContext& ctx, uint8_t* base);
void TuningPagesInputDecalAngle(PPCContext& ctx, uint8_t* base);
void TuningPagesInputDecalScale(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveDecalPosition(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveDecalAngle(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveDecalScale(PPCContext& ctx, uint8_t* base);
// InputLayerEdit sub_82205110 and MoveLayerEdit sub_82200760 (no adapter:
// the harness's step).
void TuningPagesInputLayerEdit(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveLayerEdit(PPCContext& ctx, uint8_t* base);
// InputVinylLayers sub_82208CE0 and MoveVinylLayers sub_821FFB20 (no
// adapter: the harness's step).
void TuningPagesInputVinylLayers(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveVinylLayers(PPCContext& ctx, uint8_t* base);
// InputVinylImages sub_822058F8 and MoveVinylImages sub_82202530 (no
// adapter: the harness's step).
void TuningPagesInputVinylImages(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveVinylImages(PPCContext& ctx, uint8_t* base);
// TuningScreen::HighlightItem sub_82203400 and HighlightSelected
// sub_82203570 (tuning_screen's plain C++; no adapters: the harness's
// steps).
void TuningPagesHighlightItem(PPCContext& ctx, uint8_t* base);
void TuningPagesHighlightSelected(PPCContext& ctx, uint8_t* base);
// InputLayerActions sub_82209738 with MoveLayerActions sub_82200230 and
// BeginLayerOrder sub_821FE370 (no adapters: the harness's steps).
void TuningPagesInputLayerActions(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveLayerActions(PPCContext& ctx, uint8_t* base);
void TuningPagesBeginLayerOrder(PPCContext& ctx, uint8_t* base);

// InputLayerOrder sub_8220A1C8, SwapOrderTextures sub_82207F38 and
// SwapLayerRecords sub_821FD518 (adapters: TuningScreen::Command21, recompiled,
// calls the last two); EndLayerOrder sub_821FE3C8, ShowOrderNodes
// sub_82203A30, MoveLayerOrder sub_82201DB8, SetGroupImage sub_821FD5C0 and
// SetImageGroup sub_821FD658 (no adapters: the harness's steps).
void TuningPagesInputLayerOrder(PPCContext& ctx, uint8_t* base);
void TuningPagesSwapOrderTextures(PPCContext& ctx, uint8_t* base);
void TuningPagesSwapLayerRecords(PPCContext& ctx, uint8_t* base);
void TuningPagesEndLayerOrder(PPCContext& ctx, uint8_t* base);
void TuningPagesShowOrderNodes(PPCContext& ctx, uint8_t* base);
void TuningPagesMoveLayerOrder(PPCContext& ctx, uint8_t* base);
void TuningPagesSetGroupImage(PPCContext& ctx, uint8_t* base);
void TuningPagesSetImageGroup(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
