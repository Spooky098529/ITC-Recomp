// The tuning screen's pages natively: adapters in front of src/tuning_pages/
// (docs/functions/tuning-pages.md), with the register steps the harness
// shares (hooks/tuning_pages.h). The pages and the screen's update are still
// recompiled and call these, so the adapters stay.

#include "hooks/tuning_pages.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "car_models/vinyl.h"
#include "hooks/heap.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/load_poll.h"
#include "hooks/stand_models.h"
#include "hooks/tuning_screen.h"
#include "tuning_pages/tuning_pages.h"

#include <optional>
#include "lifted/sdk.h"

ITC_FLAG_DECLARE(bool, native_car_models);
ITC_FLAG_DECLARE(bool, native_car_specs);
ITC_FLAG_DECLARE(bool, native_action_mappers);
ITC_FLAG_DECLARE(bool, native_tuning_screen);

ITC_FLAG_BOOL(native_tuning_pages, true, "ITC",
              "Run the tuning screen's pages' functions (docs/functions/tuning-pages.md: "
              "TuningScreen::LevelCount sub_821CC728, TypeCount sub_821CC7B8, PageCursor "
              "sub_821CCD38 (its piece sub_821CCD70 merged), PageItemCount sub_821CCDB0 (its piece "
              "sub_821CCDE8 merged), IsPickOwned sub_821CCE30 (these three with no adapters: "
              "their callers in play are native), MarkOwned sub_821CD498 (its piece "
              "sub_821CD6C8 merged; no adapter: only OnDialogYes calls it), TestAndSetWheelFlag sub_821CDF98, SetWheelListEnd "
              "sub_821E7DB0 (its piece sub_821E7DE4 merged), InchListEnd sub_821EF288 (its piece "
              "sub_821EF2FC merged); the vinyl editor's set-up SetUpVinylBusyIcon sub_821FE560, "
              "DecalImages::AllLoaded sub_821FE5D8, KeepListTextures sub_821FE7D8 (these three "
              "and BindVinylTextures with no adapters: only EnterEditor calls them), "
              "SetEditorLimits sub_821FE420 and LoadPageLayouts sub_822045F8; the decal images' "
              "textures DecalImages::FreeTexture sub_821FE6A0 (Texture::Free by its plain C++ with "
              "--native_car_models and the native heap), RestoreVinylTexture sub_82204698, "
              "RestoreStickerTexture sub_821F89B8, BindVinylTexture sub_82204730, "
              "BindVinylTextures sub_82203D40, EnterEditor sub_821D84F8 (FadeIn's plain C++), "
              "RestoreMovieTexture sub_822081A0, RestoreVinylMovie sub_822081D8 and "
              "RestoreStickerMovie sub_821F6DD8 (these two with no adapters: only UnloadAll "
              "calls them); with the native heap DecalImages::Update "
              "sub_821FE628 (Texture::FromTga sub_820ED528 under it, no adapter: TGA::CreateImage, "
              "Texture::Create and the surface calls by their plain C++, Resource::Release a guest "
              "call) and DecalImages::UnloadAll sub_82208738 (DecalImages::Unload sub_821FEA68 "
              "under it, no adapter; Texture::Free by its plain C++ with --native_car_models); the "
              "vinyls' records StoreVinyls sub_82203F20 and LoadVinyls sub_822040A8 (memcpy's plain "
              "C++ where Buffers_Memcpy takes it); ShowPricePanel sub_821D9E68, SetBrandLogo "
              "sub_821EED98 (its piece sub_821EEE14 merged), movie 2's nodes through the UI "
              "movies' plain C++; Color::HsvToRgb sub_8224AB50; the current choices "
              "SetAeroAvailability sub_821DE160 and SetCurrentChoice sub_821E1DB0 (its piece "
              "sub_821E203C merged; Params::GetCarParts and AeroParts::Get82 by their plain C++ "
              "with --native_car_specs; no adapter: only OnDialogYes calls it in play); the decal "
              "images' loads LoadDecalImages sub_821FEE10 with "
              "DecalImages::Request sub_821FE830 and Load sub_821F6D40 under it (these two with "
              "no adapters: only LoadDecalImages calls them; VirtualAlloc and Archive::Load through "
              "their overrides); the input pages Menu::StepList sub_821CE1D0, LeaveEditor "
              "sub_821D8670 and InputCategories sub_821D6AD0 (the mappers' slots by their plain C++ "
              "with --native_action_mappers; Menu::Step, MenuSound::Play and the UI movie calls "
              "through their overrides; FadeIn, FadeOut, CategoryItemCount, MoveCategory and, with "
              "the native heap, DecalImages::UnloadAll by their plain C++); BackToItems "
              "sub_821D87C0, InputWheelParts sub_821EE390 with AnimateWheelParts sub_821E8CE0 and "
              "InputInches sub_821EEEA8 with AnimateInches sub_821E9110 (these two with no "
              "adapters: only their pages call them; SetWheelListEnd, ShowPricePanel, "
              "PageItemCount, PageCursor, IsPickOwned, GetPrice, PreviewWheels and the UI movies' "
              "FindNode and Child by their plain C++); the confirm dialogs' yes answers OnDialogYes "
              "sub_821E2660 with OnStickerDialogYes sub_821FA230 and, under them, "
              "DecalImages::SwapKept sub_821FE798, SaveAreaDecals sub_821F6CE8, StoreStickers "
              "sub_821F8598 and GarageDecal sub_821F84C8 (no adapters: only OnDialogYes and "
              "OnStickerDialogYes call them in play; MarkOwned, SetCurrentChoice, PageItemCount, "
              "PageCursor, StoreVinyls, BindVinylTexture and SaveData::Of by their plain C++, "
              "Career::PickNext through its override, GarageStage::FitSelectedCar by its plain C++ "
              "with --native_tuning_screen and the stand's callees' flags, else the guest's); state 4's "
              "page InputItems sub_821DFD48 with its movie side MoveItem sub_821D0CC0 (no adapter: "
              "only InputItems calls it; Menu::StepList, LevelCount, TypeCount, PageItemCount, "
              "PageCursor, SetEditorLimits, LoadVinyls, LoadPageLayouts, LoadDecalImages, "
              "StoreToGarageCar, ScenePools::GetB, FadeOut and the UI movies' FindNode by their "
              "plain C++ (LevelCount, TypeCount, SetEditorLimits, LoadVinyls, LoadPageLayouts and "
              "LoadDecalImages with no adapters: only InputItems calls them in play); SceneModelRecord::Copy and UiMovie::ResetPose "
              "through the guest; ResetStickerEditor and LoadStickers by their plain C++, below); "
              "state 8's page InputOptions sub_821D8160 with its movie side MoveOption "
              "sub_821D5610 (no adapter: only InputOptions calls it; IsPickOwned, GetPrice, "
              "PageItemCount, PageCursor, GetCurrentChoice and the UI movies' FindNode and Child "
              "by their plain C++; Menu::Step, MenuSound::Play and the UI movie calls through "
              "their overrides); state 59's page Leave sub_821E25B0 with StoreOwnedParts "
              "sub_821DD408 (no adapter: only Leave calls it; StoreToGarageCar and "
              "TestAndSetWheelFlag by their plain C++); "
              "state 16's page InputWheels sub_821F6688 with its movie side AnimateWheels "
              "sub_821F60E0 and the list's rows FillPartRowsRight sub_821F40B8 and "
              "FillPartRowsLeft sub_821F4768 (no adapters: only InputWheels and AnimateWheels "
              "call them; SetWheelListEnd, InchListEnd, PreviewWheels, ShowPricePanel, "
              "SetBrandLogo, GetPrice, SetPartName, PlayGroupAnimation, FillPartRows, "
              "HidePartMarks, ShowPartMarks and the UI movies' FindNode and Child by their plain "
              "C++; MenuSound::Play, SetText and the named animations through their overrides, "
              "the varargs SetText calls leave unset passed as 0); "
              "state 5's page InputLevels sub_821D6E20 with its movie side MoveLevel "
              "sub_821D1A18 (no adapter: only InputLevels calls it; Menu::StepList (no adapter: "
              "only InputItems and InputLevels call it in play), IsItemAvailable, IsPickOwned, GetPrice, "
              "PageItemCount, PageCursor, GetCurrentChoice and the UI movies' FindNode and Child "
              "by their plain C++; Menu::Step, MenuSound::Play and the UI movie calls through "
              "their overrides); "
              "state 6's page InputTypes sub_821D74A0 with its movie side MoveType "
              "sub_821D32C8 (no adapter: only InputTypes calls it; IsItemAvailable, IsPickOwned, GetPrice, PageItemCount, PageCursor, "
              "GetCurrentChoice and the UI movies' FindNode and Child by their plain C++; "
              "Menu::Step, MenuSound::Play and the UI movie calls through their overrides); "
              "the vinyl editor's loads DecalImages::OnLoaded sub_821FE608, Reload sub_821FE960, "
              "LoadWindow sub_821FEAF0, Scroll sub_821FED30 and FreeKept sub_821FE740 "
              "(Archive::Load through its override, Texture::Free by its plain C++ with "
              "--native_car_models and the native heap); state 34's page InputVinylView "
              "sub_82202928 with LeaveVinylView sub_82201B40 (no adapter: only InputVinylView "
              "calls it; the mappers' slots 8, 10 and 12 by their plain C++ with "
              "--native_action_mappers; MenuSound::Play, the UI movie calls and GarageStage's "
              "Command and CameraCommand through their overrides, FitSelectedCar as OnDialogYes'); state 23's "
              "page InputVinylPages sub_82204B70 with SetLastImage sub_821FE498 and its movie "
              "side MoveVinylMenu sub_821FF3A0 (no adapters: only InputVinylPages calls them; "
              "FreeKept, StoreVinyls, FadeOut and memcpy by their plain C++); states 29 to "
              "31's pages InputDecalPosition sub_82206A50, InputDecalAngle sub_82206F48 and "
              "InputDecalScale sub_82207320 (these two not seen in play) with their movie sides "
              "MoveDecalPosition sub_82200DC0, MoveDecalAngle sub_822012E8 and MoveDecalScale "
              "sub_82201620 (no adapters: only their pages call them; HighlightSelected "
              "through the guest, the UI movies' FindNode by its plain C++); state 26's page "
              "InputLayerEdit sub_82205110 with its movie side MoveLayerEdit sub_82200760 (no "
              "adapter: only InputLayerEdit calls it); state 24's page InputVinylLayers "
              "sub_82208CE0 with its movie side MoveVinylLayers sub_821FFB20 (no adapter: only "
              "InputVinylLayers calls it; FreeTexture, LoadWindow and RestoreStickerTexture by "
              "their plain C++); state 28's page InputVinylImages sub_822058F8 with its movie "
              "side MoveVinylImages sub_82202530 (no adapter: only InputVinylImages calls it; "
              "Scroll, LoadWindow, Reload, FreeTexture, FreeKept and BindVinylTexture by their "
              "plain C++, the sticker editor's sub_821FD5C0 and sub_821FD658 by their plain "
              "C++); state 25's page InputLayerActions sub_82209738 (not seen in play) with "
              "its movie side MoveLayerActions sub_82200230 and BeginLayerOrder sub_821FE370 "
              "(no adapters: only InputLayerActions calls them; HighlightSelected, FreeTexture, "
              "LoadWindow, RestoreStickerTexture and memcpy by their plain C++); state 27's page "
              "InputLayerOrder sub_8220A1C8 (not seen in play) with SwapOrderTextures "
              "sub_82207F38 and SwapLayerRecords sub_821FD518 (adapters: the recompiled "
              "TuningScreen::Command21 calls them too), EndLayerOrder sub_821FE3C8, "
              "ShowOrderNodes sub_82203A30 and MoveLayerOrder sub_82201DB8 (no adapters: only "
              "InputLayerOrder calls them); the sticker editor's SetGroupImage sub_821FD5C0 "
              "(its piece sub_821FD5F0 merged) and SetImageGroup sub_821FD658 (no adapters: "
              "only InputVinylImages and InputLayerActions call them); the screen's input "
              "handler HandleInput sub_821E6950 (its piece sub_821E69B8 merged; the pages above "
              "by their plain C++, the pages never seen in play through the guest), and under "
              "InputItems the sticker editor's set-up ResetStickerEditor sub_821F8440, "
              "LoadStickers sub_821F86C0 and RestoreAreaDecals sub_821F6C90 (no adapters: only "
              "InputItems calls them in play; GarageDecal and memcpy by their plain C++)); "
              "the sticker editor's helpers (docs/functions/sticker-editor.md, not seen in "
              "play) UpdateSoloHold sub_821F7298, UpdatePreviewHold sub_821F7348, "
              "StepUnlockedSticker sub_821F73F8, ShowSlotNode sub_821F8070, PlaceStickerNode "
              "sub_821F8160, DecalImages::SetSlot sub_821F8868 and SetStickerEditTexts "
              "sub_821F7F20, ShowSlotImage sub_821FA408, ShowStickerListImage sub_821FA460 and "
              "the movie sides AnimateStickerAreas sub_821FA508, AnimateStickerOperate "
              "sub_821F7518, AnimateStickerSlots sub_821F8A50, AnimateStickerEditMenu "
              "sub_821F8D68, ShowStickerOrder sub_821FBA40 and ShowStickerPicker sub_821FBB60 "
              "(the UI movies' FindNode by its plain C++; SetText, the named animations, "
              "UiMovie::Unloop and GarageStage::Command through their overrides, the varargs "
              "SetText calls leave unset passed as 0; no adapters: their callers, the pages, "
              "the helpers and the update's sticker commands, are native), and its input pages "
              "InputStickerAreas "
              "sub_821FBD40, InputStickerSlots sub_821F9050, InputStickerOperate sub_821F9328, "
              "InputStickerEdit sub_821F9608, InputStickerOrder sub_821FBF18, InputStickerPicker "
              "sub_821FC270, InputStickerMove sub_821F99A8, InputStickerRotate sub_821F9D30, "
              "InputStickerScale sub_821F9FB8 and InputStickerView sub_821F7000 (no adapters: "
              "only HandleInput calls them; the helpers, RestoreAreaDecals, FadeOut and memcpy "
              "by their plain C++, Menu::Step, Menu::Pressed, MenuSound::Play and the stand's "
              "commands through their overrides) "
              "natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=tuning_pages)");

namespace hooks {

namespace tp = tuning_pages;

ITC_REGS_STEP(TuningPagesLevelCount) {
  ctx.r3.u64 = tp::LevelCount(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesTypeCount) {
  ctx.r3.u64 = tp::TypeCount(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesPageCursor) {
  ctx.r3.u64 = tp::PageCursor(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesPageItemCount) {
  ctx.r3.u64 = tp::PageItemCount(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesIsPickOwned) {
  ctx.r3.u64 = tp::IsPickOwned(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesMarkOwned) { tp::MarkOwned(ctx.r3.u32); }

ITC_REGS_STEP(TuningPagesTestAndSetWheelFlag) {
  ctx.r3.u64 = tp::TestAndSetWheelFlag(ctx.r3.u32, ctx.r4.u64);
}

ITC_REGS_STEP(TuningPagesSetWheelListEnd) {
  tp::SetWheelListEnd(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesInchListEnd) {
  ctx.r3.u64 = tp::InchListEnd(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesSetUpVinylBusyIcon) {
  tp::SetUpVinylBusyIcon(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesAllLoaded) {
  ctx.r3.u64 = tp::AllLoaded(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesKeepListTextures) {
  tp::KeepListTextures(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesSetEditorLimits) {
  tp::SetEditorLimits(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesLoadPageLayouts) {
  tp::LoadPageLayouts(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesFreeTexture) {
  const tp::TextureFreeFn free = [&ctx](uint32_t header_in, uint32_t data_in) -> uint64_t {
    if (ITC_FLAG(native_car_models) && NativeHeapActive() &&
        car_models::TextureFreeable(header_in)) {
      return car_models::TextureFree(header_in, data_in);
    }
    ctx.r3.u64 = header_in;
    ctx.r4.u64 = data_in;
    lifted::Enter(ctx, game::Texture::Free);
    return ctx.r3.u64;
  };
  ctx.r3.u64 = tp::FreeTexture(ctx.r3.u64, ctx.r4.u64, free);
}

ITC_REGS_STEP(TuningPagesRestoreVinylTexture) {
  ctx.r3.u64 = tp::RestoreVinylTexture(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesRestoreStickerTexture) {
  ctx.r3.u64 = tp::RestoreStickerTexture(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesBindVinylTexture) {
  ctx.r3.u64 = tp::BindVinylTexture(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesBindVinylTextures) {
  ctx.r3.u64 = tp::BindVinylTextures(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesEnterEditor) {
  ctx.r3.u64 = tp::EnterEditor(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesRestoreMovieTexture) {
  ctx.r3.u64 = tp::RestoreMovieTexture(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesRestoreVinylMovie) {
  ctx.r3.u64 = tp::RestoreVinylMovie(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesRestoreStickerMovie) {
  ctx.r3.u64 = tp::RestoreStickerMovie(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesStoreVinyls) {
  ctx.fpscr.disableFlushMode();
  tp::StoreVinyls(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesLoadVinyls) {
  const uint64_t lr = ctx.lr;
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  ctx.fpscr.disableFlushMode();
  const tuning_screen::GuestCall call = StandModelsGuestCall(ctx);
  const uint64_t r3 = tp::LoadVinyls(call, screen, sp);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(TuningPagesShowPricePanel) {
  ctx.r3.u64 = tp::ShowPricePanel(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesSetBrandLogo) {
  ctx.r3.u64 = tp::SetBrandLogo(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesHsvToRgb) {
  ctx.fpscr.disableFlushMode();
  tp::HsvToRgb(ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesIsSlotAvailable) {
  ctx.r3.u64 = tp::IsSlotAvailable(ctx.r3.u32, ctx.r4.u64);
}

ITC_REGS_STEP(TuningPagesSetPaintGradients) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = tp::SetPaintGradients(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesSetLightKind0Bars) {
  ctx.r3.u64 = tp::SetLightKind0Bars(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesSetLightKind1Bar) {
  ctx.r3.u64 = tp::SetLightKind1Bar(ctx.r3.u32, ctx.r1.u32);
}

namespace {

// A step with the guest calls (hooks::StandModelsGuestCall), lr put back.
template <typename Body>
void WithCalls(lifted::Regs& ctx, Body body) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const tuning_screen::GuestCall call = StandModelsGuestCall(ctx);
  const uint64_t r3 = body(call);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// WithCalls for the pages that reach GarageStage::FitSelectedCar
// (OnDialogYes, InputVinylView, LeaveVinylView, MoveVinylMenu,
// InputVinylPages, HandleInput): `body(call, fit)`, `fit` its plain C++'s
// calls (the car makers' with the load poll's, as the tuning screen's
// WithStand) under its adapter's flags until it went (B7 loop, 2026-11-13:
// --native_tuning_screen and TuningStandCalleesNative), its f1 put in f1
// before each later guest call and at the end; else none (its original
// through the guest).
template <typename Body>
void WithFit(lifted::Regs& ctx, Body body) {
  if (!ITC_FLAG(native_tuning_screen) || !TuningStandCalleesNative()) {
    WithCalls(ctx,
              [&](const tuning_screen::GuestCall& call) { return body(call, tp::StandFit{}); });
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  calls.poll = &poll;
  std::optional<double> f1;
  const tuning_screen::GuestCall inner = StandModelsGuestCall(ctx);
  const tuning_screen::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                            uint32_t frame, uint32_t lr) {
    if (f1) {
      ctx.f1.f64 = *f1;
      f1.reset();
    }
    return inner(function, args, frame, lr);
  };
  const uint64_t r3 = body(call, tp::StandFit{&calls, &f1});
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  if (f1) {
    ctx.f1.f64 = *f1;
  }
  ctx.fpscr.disableFlushMode();
}

}  // namespace

ITC_REGS_STEP(TuningPagesSetAeroAvailability) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::SetAeroAvailability(call, screen, sp, native_specs);
  });
}

ITC_REGS_STEP(TuningPagesSetCurrentChoice) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::SetCurrentChoice(call, r3, sp, native_specs);
  });
}

ITC_REGS_STEP(TuningPagesUpdateImages) {
  if (!NativeHeapActive()) {
    lifted::original::c821FE628(ctx);
    return;
  }
  const uint64_t images = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::UpdateImages(call, images, sp);
  });
}

ITC_REGS_STEP(TuningPagesUnloadAll) {
  if (!NativeHeapActive()) {
    lifted::original::c82208738(ctx);
    return;
  }
  const uint64_t images = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_free = ITC_FLAG(native_car_models);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::UnloadAll(call, images, sp, native_free);
  });
}

ITC_REGS_STEP(TuningPagesLoadDecalImages) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::LoadDecalImages(call, screen, sp);
  });
}

ITC_REGS_STEP(TuningPagesRequestImage) {
  const uint64_t images = ctx.r3.u64, id = ctx.r4.u64, i = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::RequestImage(call, images, id, i, sp);
  });
}

ITC_REGS_STEP(TuningPagesLoadImage) {
  const uint64_t images = ctx.r3.u64, id = ctx.r4.u64, i = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::LoadImage(call, images, id, i, sp);
  });
}

ITC_REGS_STEP(TuningPagesStepList) {
  const uint64_t menu = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::StepList(call, menu, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesLeaveEditor) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::LeaveEditor(call, screen, sp);
  });
}

namespace {

tp::PageFlags Flags() {
  tp::PageFlags flags;
  flags.native_slots = ITC_FLAG(native_action_mappers);
  flags.native_heap = NativeHeapActive();
  flags.native_free = ITC_FLAG(native_car_models);
  return flags;
}

}  // namespace

ITC_REGS_STEP(TuningPagesBackToItems) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::BackToItems(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesInputWheelParts) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputWheelParts(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesAnimateWheelParts) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateWheelParts(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputInches) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputInches(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesAnimateInches) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateInches(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputItems) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputItems(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesMoveItem) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveItem(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputOptions) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputOptions(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesMoveOption) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveOption(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesMoveLevel) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveLevel(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesMoveType) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveType(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputLevels) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputLevels(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesInputTypes) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputTypes(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesInputWheels) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputWheels(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesAnimateWheels) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateWheels(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesFillPartRowsRight) {
  ctx.r3.u64 = tp::FillPartRowsRight(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesFillPartRowsLeft) {
  ctx.r3.u64 = tp::FillPartRowsLeft(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesStoreOwnedParts) {
  ctx.r3.u64 = tp::StoreOwnedParts(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesLeave) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = tp::Leave(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesOnDialogYes) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithFit(ctx, [&](const tuning_screen::GuestCall& call, const tp::StandFit& fit) {
    return tp::OnDialogYes(call, screen, sp, native_specs, fit);
  });
}

ITC_REGS_STEP(TuningPagesOnStickerDialogYes) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::OnStickerDialogYes(call, screen, sp);
  });
}

ITC_REGS_STEP(TuningPagesInputCategories) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  tp::PageFlags flags;
  flags.native_slots = ITC_FLAG(native_action_mappers);
  flags.native_heap = NativeHeapActive();
  flags.native_free = ITC_FLAG(native_car_models);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputCategories(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesOnImageLoaded) {
  tp::OnImageLoaded(ctx.r3.u32, ctx.r6.u32);
}

ITC_REGS_STEP(TuningPagesReloadImage) {
  const uint64_t images = ctx.r3.u64, id = ctx.r4.u64, i = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::ReloadImage(call, images, id, i, sp);
  });
}

ITC_REGS_STEP(TuningPagesLoadWindow) {
  const uint64_t images = ctx.r3.u64, first = ctx.r4.u64, i = ctx.r5.u64, n = ctx.r6.u64,
                 list = ctx.r7.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::LoadWindow(call, images, first, i, n, list, sp);
  });
}

ITC_REGS_STEP(TuningPagesScrollImages) {
  const uint64_t images = ctx.r3.u64, direction = ctx.r4.u64, i = ctx.r5.u64, n = ctx.r6.u64,
                 id = ctx.r7.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_free = ITC_FLAG(native_car_models) && NativeHeapActive();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::ScrollImages(call, images, direction, i, n, id, sp, native_free);
  });
}

ITC_REGS_STEP(TuningPagesFreeKept) {
  const uint64_t images = ctx.r3.u64, k = ctx.r4.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_free = ITC_FLAG(native_car_models) && NativeHeapActive();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    // The tail call runs from the caller's frame, returning to its caller.
    return tp::FreeKept(images, k,
                        tp::GuestTextureFree(call, sp, uint32_t(ctx.lr), native_free));
  });
}

ITC_REGS_STEP(TuningPagesInputVinylView) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithFit(ctx, [&](const tuning_screen::GuestCall& call, const tp::StandFit& fit) {
    return tp::InputVinylView(call, screen, sp, native_slots, fit);
  });
}

ITC_REGS_STEP(TuningPagesLeaveVinylView) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithFit(ctx, [&](const tuning_screen::GuestCall& call, const tp::StandFit& fit) {
    return tp::LeaveVinylView(call, screen, sp, native_slots, fit);
  });
}

ITC_REGS_STEP(TuningPagesSetLastImage) {
  tp::SetLastImage(ctx.r3.u32);
}

ITC_REGS_STEP(TuningPagesMoveVinylMenu) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithFit(ctx, [&](const tuning_screen::GuestCall& call, const tp::StandFit& fit) {
    return tp::MoveVinylMenu(call, screen, sp, native_slots, fit);
  });
}

ITC_REGS_STEP(TuningPagesInputVinylPages) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithFit(ctx, [&](const tuning_screen::GuestCall& call, const tp::StandFit& fit) {
    tp::PageFlags flags = Flags();
    flags.fit = fit;
    return tp::InputVinylPages(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesInputDecalPosition) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputDecalPosition(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesInputDecalAngle) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputDecalAngle(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesInputDecalScale) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputDecalScale(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesMoveDecalPosition) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveDecalPosition(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesMoveDecalAngle) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveDecalAngle(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesMoveDecalScale) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveDecalScale(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputLayerEdit) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputLayerEdit(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesMoveLayerEdit) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveLayerEdit(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputVinylLayers) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputVinylLayers(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesMoveVinylLayers) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveVinylLayers(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputVinylImages) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputVinylImages(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesHighlightItem) {
  ctx.r3.u64 =
      tuning_screen::HighlightItem(ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesHighlightSelected) {
  ctx.r3.u64 = tuning_screen::HighlightSelected(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesMoveVinylImages) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveVinylImages(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesInputLayerActions) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputLayerActions(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesMoveLayerActions) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveLayerActions(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesBeginLayerOrder) {
  tp::BeginLayerOrder(ctx.r3.u64);
}

ITC_REGS_STEP(TuningPagesInputLayerOrder) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::InputLayerOrder(call, screen, sp, flags);
  });
}

ITC_REGS_STEP(TuningPagesSwapOrderTextures) {
  ctx.r3.u64 = tp::SwapOrderTextures(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesSwapLayerRecords) {
  const uint64_t screen = ctx.r3.u64, first = ctx.r4.u64, second = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::SwapLayerRecords(call, screen, first, second, sp);
  });
}

ITC_REGS_STEP(TuningPagesEndLayerOrder) {
  tp::EndLayerOrder(ctx.r3.u64);
}

ITC_REGS_STEP(TuningPagesShowOrderNodes) {
  ctx.r3.u64 = tp::ShowOrderNodes(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesMoveLayerOrder) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveLayerOrder(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesSetGroupImage) {
  tp::SetGroupImage(ctx.r3.u64, ctx.r4.u64);
}

ITC_REGS_STEP(TuningPagesSetImageGroup) {
  tp::SetImageGroup(ctx.r3.u64, ctx.r4.u64);
}

ITC_REGS_STEP(TuningPagesResetStickerEditor) {
  ctx.r3.u64 = tp::ResetStickerEditor(ctx.r3.u64);
}

ITC_REGS_STEP(TuningPagesRestoreAreaDecals) {
  const uint64_t screen = ctx.r3.u64, area = ctx.r4.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::RestoreAreaDecals(call, screen, area, sp);
  });
}

ITC_REGS_STEP(TuningPagesLoadStickers) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::LoadStickers(call, screen, sp);
  });
}

ITC_REGS_STEP(TuningPagesHandleInput) {
  ctx.fpscr.disableFlushMode();  // the sticker pages' floats (PlaceStickerNode, D's steps)
  const uint64_t screen = ctx.r3.u64, r4 = ctx.r4.u64;
  const uint32_t sp = ctx.r1.u32;
  WithFit(ctx, [&](const tuning_screen::GuestCall& call, const tp::StandFit& fit) {
    tp::PageFlags flags = Flags();
    flags.fit = fit;
    return tp::HandleInput(call, screen, r4, sp, flags);
  });
}

namespace {

// A sticker page's step: r3 the screen, flush-to-zero off.
using StickerPage = uint64_t (*)(const tuning_screen::GuestCall&, uint64_t, uint32_t,
                                 const tp::PageFlags&);
void RunStickerPage(lifted::Regs& ctx, StickerPage page) {
  ctx.fpscr.disableFlushMode();
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return page(call, screen, sp, flags);
  });
}

}  // namespace

ITC_REGS_STEP(TuningPagesUpdateSoloHold) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) -> uint64_t {
    return tp::UpdateSoloHold(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesUpdatePreviewHold) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) -> uint64_t {
    return tp::UpdatePreviewHold(call, screen, sp, native_slots);
  });
}

ITC_REGS_STEP(TuningPagesStepUnlockedSticker) {
  ctx.r3.u64 = tp::StepUnlockedSticker(ctx.r3.u32, ctx.r4.u64, ctx.r5.u64);
}

ITC_REGS_STEP(TuningPagesShowSlotNode) {
  ctx.r3.u64 = tp::ShowSlotNode(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesPlaceStickerNode) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      tp::PlaceStickerNode(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesSetSlot) {
  ctx.r3.u64 = tp::SetSlot(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesSetStickerEditTexts) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::SetStickerEditTexts(call, screen, sp);
  });
}

ITC_REGS_STEP(TuningPagesShowSlotImage) {
  ctx.r3.u64 = tp::ShowSlotImage(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesShowStickerListImage) {
  ctx.r3.u64 = tp::ShowStickerListImage(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesShowStickerOrder) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = tp::ShowStickerOrder(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(TuningPagesAnimateStickerAreas) {
  const uint32_t screen = ctx.r3.u32, show = ctx.r4.u32, sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateStickerAreas(call, screen, show, sp);
  });
}

ITC_REGS_STEP(TuningPagesAnimateStickerOperate) {
  const uint32_t screen = ctx.r3.u32, show = ctx.r4.u32, sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateStickerOperate(call, screen, show, sp);
  });
}

ITC_REGS_STEP(TuningPagesAnimateStickerSlots) {
  const uint32_t screen = ctx.r3.u32, show = ctx.r4.u32, sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateStickerSlots(call, screen, show, sp);
  });
}

ITC_REGS_STEP(TuningPagesAnimateStickerEditMenu) {
  const uint32_t screen = ctx.r3.u32, show = ctx.r4.u32, sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateStickerEditMenu(call, screen, show, sp);
  });
}

ITC_REGS_STEP(TuningPagesShowStickerPicker) {
  const uint32_t screen = ctx.r3.u32, show = ctx.r4.u32, sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::ShowStickerPicker(call, screen, show, sp);
  });
}

ITC_REGS_STEP(TuningPagesInputStickerAreas) {
  RunStickerPage(ctx, tp::InputStickerAreas);
}

ITC_REGS_STEP(TuningPagesInputStickerSlots) {
  RunStickerPage(ctx, tp::InputStickerSlots);
}

ITC_REGS_STEP(TuningPagesInputStickerOperate) {
  RunStickerPage(ctx, tp::InputStickerOperate);
}

ITC_REGS_STEP(TuningPagesInputStickerEdit) {
  RunStickerPage(ctx, tp::InputStickerEdit);
}

ITC_REGS_STEP(TuningPagesInputStickerView) {
  RunStickerPage(ctx, tp::InputStickerView);
}

ITC_REGS_STEP(TuningPagesInputStickerOrder) {
  RunStickerPage(ctx, tp::InputStickerOrder);
}

ITC_REGS_STEP(TuningPagesInputStickerPicker) {
  RunStickerPage(ctx, tp::InputStickerPicker);
}

ITC_REGS_STEP(TuningPagesInputStickerMove) {
  RunStickerPage(ctx, tp::InputStickerMove);
}

ITC_REGS_STEP(TuningPagesInputStickerRotate) {
  RunStickerPage(ctx, tp::InputStickerRotate);
}

ITC_REGS_STEP(TuningPagesInputStickerScale) {
  RunStickerPage(ctx, tp::InputStickerScale);
}

void TuningPagesStickerCommand(lifted::Regs& ctx, uint32_t command) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tp::StickerCommand(call, command, r3, sp).value_or(r3);
  });
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_TUNING_PAGES(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_tuning_pages)) { \
    hooks::step(ctx);               \
  }

// DecalImages::UnloadAll sub_82208738 has no adapter (removed, B7 loop,
// 2026-11-13): in play only native callers reach it (TuningScreen::
// InputCategories and BackToItems, TuningMachine::Destroy), calling its
// plain C++ with the native heap; its step above is the harness's.

ITC_TUNING_PAGES(821E2660, TuningPagesOnDialogYes)

ITC_TUNING_PAGES(821FE608, TuningPagesOnImageLoaded)

// The screen's input handler: a vtable slot (no direct callers), so its
// adapter stays.
ITC_TUNING_PAGES(821E6950, TuningPagesHandleInput)

// The sticker editor's helpers (not seen in play). No adapters (B7 loop,
// 2026-11-13: their only callers, the sticker pages, are native and call
// their plain C++; the register steps above are the harness's):
// UpdateSoloHold sub_821F7298, UpdatePreviewHold sub_821F7348,
// ShowStickerOrder sub_821FBA40; and, once the update's sticker commands
// went native (the sticker editor's second migration, 2026-11-13: every
// caller in generated/ is native and calls their plain C++),
// StepUnlockedSticker sub_821F73F8, ShowSlotNode sub_821F8070,
// PlaceStickerNode sub_821F8160, DecalImages::SetSlot sub_821F8868,
// SetStickerEditTexts sub_821F7F20, ShowSlotImage sub_821FA408,
// ShowStickerListImage sub_821FA460, AnimateStickerAreas sub_821FA508,
// AnimateStickerOperate sub_821F7518, AnimateStickerSlots sub_821F8A50,
// AnimateStickerEditMenu sub_821F8D68 and ShowStickerPicker sub_821FBB60.

// No adapters (removed, B7 loop, 2026-11-13: their callers in play are all
// native and call their plain C++; the register steps above are the
// harness's): TestAndSetWheelFlag sub_821CDF98 (LoadOwnedParts,
// StoreOwnedParts), StoreVinyls sub_82203F20 (OnDialogYes, InputVinylPages),
// Color::HsvToRgb sub_8224AB50 (PanelSetBase::SetUpColors) and
// SetAeroAvailability sub_821DE160 (TuningScreen::SetUp, SetCurrentChoice).

// No adapters (removed, B7 loop, 2026-11-13, after HandleInput went native:
// their only caller, HandleInput, is native and calls their plain C++; the
// register steps above are the harness's): the input pages InputCategories
// sub_821D6AD0, InputItems sub_821DFD48, InputLevels sub_821D6E20,
// InputTypes sub_821D74A0, InputOptions sub_821D8160, InputWheelParts
// sub_821EE390, InputWheels sub_821F6688, InputInches sub_821EEEA8, Leave
// sub_821E25B0, EnterEditor sub_821D84F8, LeaveEditor sub_821D8670,
// BackToItems sub_821D87C0, InputVinylView sub_82202928, InputVinylPages
// sub_82204B70, InputVinylLayers sub_82208CE0, InputLayerActions
// sub_82209738, InputLayerEdit sub_82205110, InputLayerOrder sub_8220A1C8,
// InputVinylImages sub_822058F8, InputDecalPosition sub_82206A50,
// InputDecalAngle sub_82206F48, InputDecalScale sub_82207320, and
// DecalImages::Update sub_821FE628. SwapOrderTextures sub_82207F38 and
// SwapLayerRecords sub_821FD518: their only callers, InputLayerOrder and
// TuningScreen::Command21, are native (Command21 calls them through the guest
// only with --native_tuning_pages off, the original's case).

// No adapters (removed, B7 loop, 2026-11-13: their callers in play are all
// native and call their plain C++; the register steps above are the
// harness's): DecalImages::FreeTexture sub_821FE6A0, RestoreStickerTexture
// sub_821F89B8, BindVinylTexture sub_82204730, Reload sub_821FE960,
// LoadWindow sub_821FEAF0, Scroll sub_821FED30, FreeKept sub_821FE740.

// No adapters (B7 loop, 2026-11-13: their only caller, HandleInput, is native
// and calls their plain C++; the register steps above are the harness's): the
// sticker editor's pages InputStickerAreas sub_821FBD40, InputStickerSlots
// sub_821F9050, InputStickerOperate sub_821F9328, InputStickerEdit
// sub_821F9608, InputStickerOrder sub_821FBF18, InputStickerPicker
// sub_821FC270, InputStickerMove sub_821F99A8, InputStickerRotate
// sub_821F9D30, InputStickerScale sub_821F9FB8 and InputStickerView
// sub_821F7000.

// No adapter (B7 loop, 2026-11-13: its only caller, InputVinylView, is native
// and calls its plain C++; the register step above is the harness's):
// LeaveVinylView sub_82201B40.

// No adapters (B7 loop, 2026-11-13: their only caller, InputVinylPages, is
// native and calls their plain C++; the register steps above are the
// harness's): SetLastImage sub_821FE498, MoveVinylMenu sub_821FF3A0.

// No adapters (B7 loop, 2026-11-13: their only caller, InputLayerActions, is
// native and calls their plain C++; the register steps above are the
// harness's): MoveLayerActions sub_82200230, BeginLayerOrder sub_821FE370.

// No adapters (B7 loop, 2026-11-13: their only caller, InputLayerOrder, is
// native and calls their plain C++; the register steps above are the
// harness's): EndLayerOrder sub_821FE3C8, ShowOrderNodes sub_82203A30,
// MoveLayerOrder sub_82201DB8. The sticker editor's SetGroupImage
// sub_821FD5C0 (its piece sub_821FD5F0 merged) and SetImageGroup sub_821FD658:
// their only callers, InputVinylImages and InputLayerActions, are native.

// No adapters (B7 loop, 2026-11-13: their only callers, the decal edit pages
// InputDecalPosition, InputDecalAngle and InputDecalScale, are native and
// call their plain C++; the register steps above are the harness's):
// MoveDecalPosition sub_82200DC0, MoveDecalAngle sub_822012E8, MoveDecalScale
// sub_82201620; MoveLayerEdit sub_82200760 (InputLayerEdit); MoveVinylLayers
// sub_821FFB20 (InputVinylLayers); MoveVinylImages sub_82202530
// (InputVinylImages).

// No adapters (removed, B7 loop, 2026-11-07: their only caller in play,
// EnterEditor, is native and calls their plain C++; the register steps above
// are the harness's): SetUpVinylBusyIcon sub_821FE560, DecalImages::AllLoaded
// sub_821FE5D8, KeepListTextures sub_821FE7D8, BindVinylTextures sub_82203D40.

// No adapters (removed, B7 loop, 2026-11-07: their only callers in play are
// native and call their plain C++; the register steps above are the
// harness's): DecalImages::RestoreVinylTexture sub_82204698
// (RestoreMovieTexture), RestoreMovieTexture sub_822081A0
// (RestoreStickerMovie), RestoreVinylMovie sub_822081D8 and
// RestoreStickerMovie sub_821F6DD8 (UnloadAll).

// No adapters (B7 loop, 2026-11-07: their only caller, LoadDecalImages, is
// native and calls their plain C++; the register steps above are the
// harness's): DecalImages::Request sub_821FE830 and Load sub_821F6D40.

// No adapters (B7 loop, 2026-11-07: their only caller in play, OnDialogYes, is
// native and calls their plain C++; the register step above is the
// harness's): OnStickerDialogYes sub_821FA230, and under it SaveAreaDecals
// sub_821F6CE8, StoreStickers sub_821F8598 with GarageDecal sub_821F84C8; under
// OnDialogYes DecalImages::SwapKept sub_821FE798.

// No adapter (B7 loop, 2026-11-07: its only caller, InputItems, is native and
// calls its plain C++; the register step above is the harness's): MoveItem
// sub_821D0CC0.

// No adapter (B7 loop, 2026-11-07: its only caller, InputOptions, is native
// and calls its plain C++; the register step above is the harness's):
// MoveOption sub_821D5610.

// No adapters (removed, B7 loop, 2026-11-07: their only callers in play are
// native and call their plain C++; the register steps above are the
// harness's): PageItemCount sub_821CCDB0 and PageCursor sub_821CCD38
// (InputLevels, InputTypes, InputOptions, InputItems, OnDialogYes,
// InputWheelParts), IsPickOwned sub_821CCE30 (InputLevels, InputTypes,
// InputOptions, InputInches; its callers not seen in play, recompiled, call
// the original) and MoveType sub_821D32C8 (InputTypes).

// No adapters (removed, B7 loop, 2026-11-07: their only callers in play,
// InputItems and InputLevels, are native and call their plain C++; the
// register steps above are the harness's): Menu::StepList sub_821CE1D0
// (InputItems, InputLevels) and MoveLevel sub_821D1A18 (InputLevels).

// No adapters (removed, B7 loop, 2026-11-07: their only callers in play,
// InputWheelParts and InputWheels, are native and call their plain C++; the
// register steps above are the harness's): SetWheelListEnd sub_821E7DB0,
// InchListEnd sub_821EF288, ShowPricePanel sub_821D9E68, SetBrandLogo
// sub_821EED98.

// No adapters (B7 loop, 2026-11-07: their only callers, InputWheels and
// AnimateWheels, are native and call their plain C++; the register steps
// above are the harness's): AnimateWheels sub_821F60E0, FillPartRowsRight
// sub_821F40B8 and FillPartRowsLeft sub_821F4768 (not seen in play).

// No adapter (B7 loop, 2026-11-07: its only caller, Leave, is native and
// calls its plain C++; the register step above is the harness's):
// StoreOwnedParts sub_821DD408.

// No adapters (removed, B7 loop, 2026-11-07: their only caller in play,
// InputItems, is native and calls their plain C++; the register steps above
// are the harness's): LevelCount sub_821CC728, TypeCount sub_821CC7B8,
// SetEditorLimits sub_821FE420, LoadVinyls sub_822040A8, LoadPageLayouts
// sub_822045F8, LoadDecalImages sub_821FEE10.

// No adapters (removed, B7 loop, 2026-11-07: their only caller in play,
// OnDialogYes, is native and calls their plain C++; the register steps above
// are the harness's): MarkOwned sub_821CD498, SetCurrentChoice sub_821E1DB0
// (sub_821F1780, recompiled and not seen in play, calls the original).

// For the fallback originals (docs/missing-functions.md): the stale
// originals round (2026-11-28), so lifted code runs the ports.
ITC_DIRECT_ENTRY(821F6DD8) { hooks::TuningPagesRestoreStickerMovie(ctx); }
ITC_DIRECT_ENTRY(822081D8) { hooks::TuningPagesRestoreVinylMovie(ctx); }

// DecalImages::Unload: r3 the images, r4 the entry; with the native heap (as
// UnloadAll's step), else its original.
ITC_DIRECT_ENTRY(821FEA68) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c821FEA68(ctx);
    return;
  }
  const uint64_t images = ctx.r3.u64, i = ctx.r4.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_free = ITC_FLAG(native_car_models);
  hooks::WithCalls(ctx, [&](const tuning_screen::GuestCall& call) {
    return tuning_pages::Unload(call, images, i, sp, native_free);
  });
}

// Texture::FromTga: r3 the file, r4 its size, r5 and r6 the header and data
// out; with the native heap (as DecalImages::Update's step) and when the port
// takes it, else its original.
ITC_DIRECT_ENTRY(820ED528) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820ED528(ctx);
    return;
  }
  const uint32_t file = ctx.r3.u32, size = ctx.r4.u32;
  const uint32_t header = ctx.r5.u32, data = ctx.r6.u32, sp = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const tuning_screen::GuestCall call = hooks::StandModelsGuestCall(ctx);
  const auto made = tuning_pages::TextureFromTga(call, file, size, header, data, sp);
  ctx.lr = lr;
  if (!made) {
    ctx.r3.u64 = file;
    ctx.r4.u64 = size;
    ctx.r5.u64 = header;
    ctx.r6.u64 = data;
    lifted::original::c820ED528(ctx);
    return;
  }
  ctx.r3.u64 = *made;
}
