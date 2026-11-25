// The tuning pages' register steps (src/tuning_pages/,
// docs/functions/tuning-pages.md), shared by the adapters
// (src/hooks/tuning_pages.cpp, behind --native_tuning_pages) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The pages' counts and owned flags (r3 = the screen): LevelCount
// sub_821CC728, TypeCount sub_821CC7B8, PageCursor sub_821CCD38,
// PageItemCount sub_821CCDB0, IsPickOwned sub_821CCE30, MarkOwned
// sub_821CD498, TestAndSetWheelFlag sub_821CDF98 (r4 a wheel number),
// SetWheelListEnd sub_821E7DB0, InchListEnd sub_821EF288 (PageCursor,
// PageItemCount and IsPickOwned with no adapters: their callers in play are
// native; these steps are the harness's).
void TuningPagesLevelCount(lifted::Regs& ctx);
void TuningPagesTypeCount(lifted::Regs& ctx);
void TuningPagesPageCursor(lifted::Regs& ctx);
void TuningPagesPageItemCount(lifted::Regs& ctx);
void TuningPagesIsPickOwned(lifted::Regs& ctx);
void TuningPagesMarkOwned(lifted::Regs& ctx);
void TuningPagesTestAndSetWheelFlag(lifted::Regs& ctx);
void TuningPagesSetWheelListEnd(lifted::Regs& ctx);
void TuningPagesInchListEnd(lifted::Regs& ctx);

// The vinyl editor's set-up: SetUpVinylBusyIcon sub_821FE560,
// KeepListTextures sub_821FE7D8, SetEditorLimits sub_821FE420,
// LoadPageLayouts sub_822045F8 (r3 = the screen); DecalImages::AllLoaded
// sub_821FE5D8 (r3 = the decal images).
void TuningPagesSetUpVinylBusyIcon(lifted::Regs& ctx);
void TuningPagesAllLoaded(lifted::Regs& ctx);
void TuningPagesKeepListTextures(lifted::Regs& ctx);
void TuningPagesSetEditorLimits(lifted::Regs& ctx);
void TuningPagesLoadPageLayouts(lifted::Regs& ctx);

// The decal images' textures: DecalImages::FreeTexture sub_821FE6A0 (r3 the
// images, r4 an entry; Texture::Free by car_models' plain C++ with
// --native_car_models and the native heap, else through the guest),
// RestoreVinylTexture sub_82204698 and RestoreStickerTexture sub_821F89B8
// (r4 w, r5 a texture), BindVinylTexture sub_82204730 (r4 an entry, r5 a
// group); TuningScreen::BindVinylTextures sub_82203D40 and EnterEditor
// sub_821D84F8 (r3 the screen).
void TuningPagesFreeTexture(lifted::Regs& ctx);
void TuningPagesRestoreVinylTexture(lifted::Regs& ctx);
void TuningPagesRestoreStickerTexture(lifted::Regs& ctx);
void TuningPagesBindVinylTexture(lifted::Regs& ctx);
void TuningPagesBindVinylTextures(lifted::Regs& ctx);
void TuningPagesEnterEditor(lifted::Regs& ctx);
// DecalImages::RestoreMovieTexture sub_822081A0 (r4 w, r5 a texture),
// RestoreVinylMovie sub_822081D8 and RestoreStickerMovie sub_821F6DD8 (r3
// the images).
void TuningPagesRestoreMovieTexture(lifted::Regs& ctx);
void TuningPagesRestoreVinylMovie(lifted::Regs& ctx);
void TuningPagesRestoreStickerMovie(lifted::Regs& ctx);
// With the native heap active (else the originals): DecalImages::Update
// sub_821FE628 (Texture::FromTga sub_820ED528 under it) and UnloadAll
// sub_82208738 (DecalImages::Unload sub_821FEA68 under it; Texture::Free by
// car_models' plain C++ with --native_car_models), r3 the images.
void TuningPagesUpdateImages(lifted::Regs& ctx);
void TuningPagesUnloadAll(lifted::Regs& ctx);

// The vinyls' records (r3 = the screen; flush-to-zero off): StoreVinyls
// sub_82203F20, LoadVinyls sub_822040A8 (memcpy through the guest where
// Buffers_Memcpy declines).
void TuningPagesStoreVinyls(lifted::Regs& ctx);
void TuningPagesLoadVinyls(lifted::Regs& ctx);

// Movie 2's nodes (r3 = the screen): ShowPricePanel sub_821D9E68,
// SetBrandLogo sub_821EED98; Color::HsvToRgb sub_8224AB50 (r4 out, r5 in).
void TuningPagesShowPricePanel(lifted::Regs& ctx);
void TuningPagesSetBrandLogo(lifted::Regs& ctx);
void TuningPagesHsvToRgb(lifted::Regs& ctx);
// The tuning pages' leaves never seen in play, for the harness only (no
// adapters: only native callers reach their plain C++, RefreshInterior and
// Command10, 13 and 14; r3 = the screen): IsSlotAvailable sub_821CE188 (r4
// the slot), SetPaintGradients sub_821E9488 (flush-to-zero off),
// SetLightKind0Bars sub_821EA130 and SetLightKind1Bar sub_821EAE68.
void TuningPagesIsSlotAvailable(lifted::Regs& ctx);
void TuningPagesSetPaintGradients(lifted::Regs& ctx);
void TuningPagesSetLightKind0Bars(lifted::Regs& ctx);
void TuningPagesSetLightKind1Bar(lifted::Regs& ctx);

// The current choices (r3 = the screen): SetAeroAvailability sub_821DE160,
// SetCurrentChoice sub_821E1DB0 (Params::GetCarParts and AeroParts::Get82 by
// their plain C++ with --native_car_specs, else through the guest).
void TuningPagesSetAeroAvailability(lifted::Regs& ctx);
void TuningPagesSetCurrentChoice(lifted::Regs& ctx);

// The decal images' loads: TuningScreen::LoadDecalImages sub_821FEE10 (r3
// the screen), DecalImages::Request sub_821FE830 and Load sub_821F6D40 (r3
// the images, r4 an id, r5 an entry).
void TuningPagesLoadDecalImages(lifted::Regs& ctx);
void TuningPagesRequestImage(lifted::Regs& ctx);
void TuningPagesLoadImage(lifted::Regs& ctx);

// The input pages: Menu::StepList sub_821CE1D0 (r3 the menu; no adapter:
// only InputItems and InputLevels call it in play, this step is the harness's),
// TuningScreen::LeaveEditor sub_821D8670 and InputCategories sub_821D6AD0
// (r3 the screen; the mappers' slots by their plain C++ with
// --native_action_mappers, DecalImages::UnloadAll by its plain C++ with the
// native heap).
void TuningPagesStepList(lifted::Regs& ctx);
void TuningPagesLeaveEditor(lifted::Regs& ctx);
void TuningPagesInputCategories(lifted::Regs& ctx);
// BackToItems sub_821D87C0, InputWheelParts sub_821EE390 and InputInches
// sub_821EEEA8 (r3 the screen), their movie sides AnimateWheelParts
// sub_821E8CE0 and AnimateInches sub_821E9110 (no adapters: only their pages
// call them; these steps are the harness's).
void TuningPagesBackToItems(lifted::Regs& ctx);
void TuningPagesInputWheelParts(lifted::Regs& ctx);
void TuningPagesAnimateWheelParts(lifted::Regs& ctx);
void TuningPagesInputInches(lifted::Regs& ctx);
void TuningPagesAnimateInches(lifted::Regs& ctx);
// InputItems sub_821DFD48 (r3 the screen) and its movie side MoveItem
// sub_821D0CC0 (no adapter: only InputItems calls it; this step is the
// harness's).
void TuningPagesInputItems(lifted::Regs& ctx);
void TuningPagesMoveItem(lifted::Regs& ctx);
// InputOptions sub_821D8160 (r3 the screen) and its movie side MoveOption
// sub_821D5610 (no adapter: only InputOptions calls it; this step is the
// harness's).
void TuningPagesInputOptions(lifted::Regs& ctx);
void TuningPagesMoveOption(lifted::Regs& ctx);
// MoveLevel sub_821D1A18 (r3 the screen), state 5's movie side (no adapter:
// only InputLevels calls it; this step is the harness's).
void TuningPagesMoveLevel(lifted::Regs& ctx);
// InputLevels sub_821D6E20 (r3 the screen), state 5's page.
void TuningPagesInputLevels(lifted::Regs& ctx);
// MoveType sub_821D32C8 (r3 the screen), state 6's movie side (no adapter:
// only InputTypes calls it; this step is the harness's).
void TuningPagesMoveType(lifted::Regs& ctx);
// InputTypes sub_821D74A0 (r3 the screen), state 6's page.
void TuningPagesInputTypes(lifted::Regs& ctx);
// Leave sub_821E25B0 (r3 the screen, r4 the object with the message ring)
// and StoreOwnedParts sub_821DD408 (no adapter: only Leave calls it; this
// step is the harness's).
void TuningPagesLeave(lifted::Regs& ctx);
void TuningPagesStoreOwnedParts(lifted::Regs& ctx);
// InputWheels sub_821F6688 (r3 the screen), its movie side AnimateWheels
// sub_821F60E0 and the list's rows FillPartRowsRight sub_821F40B8 and
// FillPartRowsLeft sub_821F4768 under it (no adapters: only InputWheels and
// AnimateWheels call them; these steps are the harness's).
void TuningPagesInputWheels(lifted::Regs& ctx);
void TuningPagesAnimateWheels(lifted::Regs& ctx);
void TuningPagesFillPartRowsRight(lifted::Regs& ctx);
void TuningPagesFillPartRowsLeft(lifted::Regs& ctx);
// The confirm dialogs' yes answers (r3 the screen): OnDialogYes sub_821E2660
// (SetCurrentChoice's Params::GetCarParts and AeroParts::Get82 by their plain
// C++ with --native_car_specs), OnStickerDialogYes sub_821FA230 (no adapter:
// only OnDialogYes calls it; this step is the harness's).
void TuningPagesOnDialogYes(lifted::Regs& ctx);
void TuningPagesOnStickerDialogYes(lifted::Regs& ctx);

// The vinyl editor's loads: DecalImages::OnLoaded sub_821FE608 (r3 a
// handle, r6 an entry), Reload sub_821FE960 (r4 an id, r5 an entry),
// LoadWindow sub_821FEAF0 (r4 the first id, r5 the first entry, r6 the
// count, r7 the list), Scroll sub_821FED30 (r4 the direction, r5 the first
// entry, r6 the count, r7 the id) and FreeKept sub_821FE740 (r4 a kept
// texture; Texture::Free by car_models' plain C++ with --native_car_models
// and the native heap); r3 the images.
void TuningPagesOnImageLoaded(lifted::Regs& ctx);
void TuningPagesReloadImage(lifted::Regs& ctx);
void TuningPagesLoadWindow(lifted::Regs& ctx);
void TuningPagesScrollImages(lifted::Regs& ctx);
void TuningPagesFreeKept(lifted::Regs& ctx);

// The vinyl editor's pages (r3 = the screen): InputVinylView sub_82202928
// and LeaveVinylView sub_82201B40 (no adapter: the harness's step).
void TuningPagesInputVinylView(lifted::Regs& ctx);
void TuningPagesLeaveVinylView(lifted::Regs& ctx);
// InputVinylPages sub_82204B70 with SetLastImage sub_821FE498 and
// MoveVinylMenu sub_821FF3A0 (no adapters: the harness's steps).
void TuningPagesInputVinylPages(lifted::Regs& ctx);
void TuningPagesSetLastImage(lifted::Regs& ctx);
void TuningPagesMoveVinylMenu(lifted::Regs& ctx);
// The decal edits: InputDecalPosition sub_82206A50, InputDecalAngle
// sub_82206F48, InputDecalScale sub_82207320, and their movie sides
// MoveDecalPosition sub_82200DC0, MoveDecalAngle sub_822012E8,
// MoveDecalScale sub_82201620 (no adapters: the harness's steps).
void TuningPagesInputDecalPosition(lifted::Regs& ctx);
void TuningPagesInputDecalAngle(lifted::Regs& ctx);
void TuningPagesInputDecalScale(lifted::Regs& ctx);
void TuningPagesMoveDecalPosition(lifted::Regs& ctx);
void TuningPagesMoveDecalAngle(lifted::Regs& ctx);
void TuningPagesMoveDecalScale(lifted::Regs& ctx);
// InputLayerEdit sub_82205110 and MoveLayerEdit sub_82200760 (no adapter:
// the harness's step).
void TuningPagesInputLayerEdit(lifted::Regs& ctx);
void TuningPagesMoveLayerEdit(lifted::Regs& ctx);
// InputVinylLayers sub_82208CE0 and MoveVinylLayers sub_821FFB20 (no
// adapter: the harness's step).
void TuningPagesInputVinylLayers(lifted::Regs& ctx);
void TuningPagesMoveVinylLayers(lifted::Regs& ctx);
// InputVinylImages sub_822058F8 and MoveVinylImages sub_82202530 (no
// adapter: the harness's step).
void TuningPagesInputVinylImages(lifted::Regs& ctx);
void TuningPagesMoveVinylImages(lifted::Regs& ctx);
// TuningScreen::HighlightItem sub_82203400 and HighlightSelected
// sub_82203570 (tuning_screen's plain C++; no adapters: the harness's
// steps).
void TuningPagesHighlightItem(lifted::Regs& ctx);
void TuningPagesHighlightSelected(lifted::Regs& ctx);
// InputLayerActions sub_82209738 with MoveLayerActions sub_82200230 and
// BeginLayerOrder sub_821FE370 (no adapters: the harness's steps).
void TuningPagesInputLayerActions(lifted::Regs& ctx);
void TuningPagesMoveLayerActions(lifted::Regs& ctx);
void TuningPagesBeginLayerOrder(lifted::Regs& ctx);

// InputLayerOrder sub_8220A1C8, SwapOrderTextures sub_82207F38 and
// SwapLayerRecords sub_821FD518 (adapters: TuningScreen::Command21, recompiled,
// calls the last two); EndLayerOrder sub_821FE3C8, ShowOrderNodes
// sub_82203A30, MoveLayerOrder sub_82201DB8, SetGroupImage sub_821FD5C0 and
// SetImageGroup sub_821FD658 (no adapters: the harness's steps).
void TuningPagesInputLayerOrder(lifted::Regs& ctx);
void TuningPagesSwapOrderTextures(lifted::Regs& ctx);
void TuningPagesSwapLayerRecords(lifted::Regs& ctx);
void TuningPagesEndLayerOrder(lifted::Regs& ctx);
void TuningPagesShowOrderNodes(lifted::Regs& ctx);
void TuningPagesMoveLayerOrder(lifted::Regs& ctx);
void TuningPagesSetGroupImage(lifted::Regs& ctx);
void TuningPagesSetImageGroup(lifted::Regs& ctx);

// HandleInput sub_821E6950 (r3 the screen, r4 Leave's object; its piece
// sub_821E69B8 merged), the screen's input handler.
void TuningPagesHandleInput(lifted::Regs& ctx);
// The sticker editor's set-up under InputItems (no adapters: only InputItems
// calls them in play; the harness's steps): ResetStickerEditor sub_821F8440,
// RestoreAreaDecals sub_821F6C90 (r4 the area) and LoadStickers
// sub_821F86C0, r3 the screen.
void TuningPagesResetStickerEditor(lifted::Regs& ctx);
void TuningPagesRestoreAreaDecals(lifted::Regs& ctx);
void TuningPagesLoadStickers(lifted::Regs& ctx);

// The sticker editor's helpers (docs/functions/sticker-editor.md; r3 the
// screen unless said): UpdateSoloHold sub_821F7298, UpdatePreviewHold
// sub_821F7348, StepUnlockedSticker sub_821F73F8 (r4 an id, r5 a step
// count), ShowSlotNode sub_821F8070 (r4 an id, r5 a slot, r6 the current
// slot), PlaceStickerNode sub_821F8160 (r4 an id, r5 a slot, r6 a record),
// DecalImages::SetSlot sub_821F8868 (r3 the images, r4 a sub-scene, r5 and
// r6 two nodes, r7 and r8 their texture slots, r9 an entry, r10 a flag),
// SetStickerEditTexts sub_821F7F20 (no adapters: their callers are all
// native; these steps and the movie sides' below are the harness's).
void TuningPagesUpdateSoloHold(lifted::Regs& ctx);
void TuningPagesUpdatePreviewHold(lifted::Regs& ctx);
void TuningPagesStepUnlockedSticker(lifted::Regs& ctx);
void TuningPagesShowSlotNode(lifted::Regs& ctx);
void TuningPagesPlaceStickerNode(lifted::Regs& ctx);
void TuningPagesSetSlot(lifted::Regs& ctx);
void TuningPagesSetStickerEditTexts(lifted::Regs& ctx);
// ShowSlotImage sub_821FA408 (r4 an id, r5 a slot), ShowStickerListImage
// sub_821FA460 (r4 an id, r5 an index); the movie sides (r4 show):
// AnimateStickerAreas sub_821FA508, AnimateStickerOperate sub_821F7518,
// AnimateStickerSlots sub_821F8A50, AnimateStickerEditMenu sub_821F8D68,
// ShowStickerOrder sub_821FBA40, ShowStickerPicker sub_821FBB60.
void TuningPagesShowSlotImage(lifted::Regs& ctx);
void TuningPagesShowStickerListImage(lifted::Regs& ctx);
void TuningPagesAnimateStickerAreas(lifted::Regs& ctx);
void TuningPagesAnimateStickerOperate(lifted::Regs& ctx);
void TuningPagesAnimateStickerSlots(lifted::Regs& ctx);
void TuningPagesAnimateStickerEditMenu(lifted::Regs& ctx);
void TuningPagesShowStickerOrder(lifted::Regs& ctx);
void TuningPagesShowStickerPicker(lifted::Regs& ctx);
// The sticker editor's input pages (r3 the screen; no adapters: the
// harness's steps): InputStickerAreas sub_821FBD40, InputStickerSlots
// sub_821F9050, InputStickerOperate sub_821F9328, InputStickerEdit
// sub_821F9608, InputStickerView sub_821F7000, InputStickerOrder
// sub_821FBF18, InputStickerPicker sub_821FC270, InputStickerMove
// sub_821F99A8, InputStickerRotate sub_821F9D30, InputStickerScale
// sub_821F9FB8.
void TuningPagesInputStickerAreas(lifted::Regs& ctx);
void TuningPagesInputStickerSlots(lifted::Regs& ctx);
void TuningPagesInputStickerOperate(lifted::Regs& ctx);
void TuningPagesInputStickerEdit(lifted::Regs& ctx);
void TuningPagesInputStickerView(lifted::Regs& ctx);
void TuningPagesInputStickerOrder(lifted::Regs& ctx);
void TuningPagesInputStickerPicker(lifted::Regs& ctx);
void TuningPagesInputStickerMove(lifted::Regs& ctx);
void TuningPagesInputStickerRotate(lifted::Regs& ctx);
void TuningPagesInputStickerScale(lifted::Regs& ctx);
// The update's sticker commands 32 to 41 and 43 to 51 (r3 the screen;
// `command` picks the runner, 46, 48 and 50 the same; no adapters: the
// native update calls their plain C++, these steps are the harness's).
void TuningPagesStickerCommand(lifted::Regs& ctx, uint32_t command);

}  // namespace hooks
