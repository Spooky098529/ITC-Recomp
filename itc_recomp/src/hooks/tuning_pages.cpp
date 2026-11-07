// The tuning screen's pages natively: adapters in front of src/tuning_pages/
// (docs/functions/tuning-pages.md), with the register steps the harness
// shares (hooks/tuning_pages.h). The pages and the screen's update are still
// recompiled and call these, so the adapters stay.

#include "hooks/tuning_pages.h"

#include "base/flags.h"
#include "car_models/vinyl.h"
#include "hooks/heap.h"
#include "hooks/stand_models.h"
#include "tuning_pages/tuning_pages.h"

ITC_FLAG_DECLARE(bool, native_car_models);
ITC_FLAG_DECLARE(bool, native_car_specs);
ITC_FLAG_DECLARE(bool, native_action_mappers);

ITC_FLAG_BOOL(native_tuning_pages, true, "ITC",
              "Run the tuning screen's pages' functions (docs/functions/tuning-pages.md: "
              "TuningScreen::LevelCount sub_821CC728, TypeCount sub_821CC7B8, PageCursor "
              "sub_821CCD38 (its piece sub_821CCD70 merged), PageItemCount sub_821CCDB0 (its piece "
              "sub_821CCDE8 merged), IsPickOwned sub_821CCE30, MarkOwned sub_821CD498 (its piece "
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
              "Career::PickNext and GarageStage::FitSelectedCar through their overrides); state 4's "
              "page InputItems sub_821DFD48 with its movie side MoveItem sub_821D0CC0 (no adapter: "
              "only InputItems calls it; Menu::StepList, LevelCount, TypeCount, PageItemCount, "
              "PageCursor, SetEditorLimits, LoadVinyls, LoadPageLayouts, LoadDecalImages, "
              "StoreToGarageCar, ScenePools::GetB, FadeOut and the UI movies' FindNode by their "
              "plain C++ (LevelCount, TypeCount, SetEditorLimits, LoadVinyls, LoadPageLayouts and "
              "LoadDecalImages with no adapters: only InputItems calls them in play); SceneModelRecord::Copy, UiMovie::ResetPose and, not seen in play, "
              "ResetStickerEditor sub_821F8440 and LoadStickers sub_821F86C0 through the guest); "
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
              "sub_821D1A18 (Menu::StepList, IsItemAvailable, IsPickOwned, GetPrice, "
              "PageItemCount, PageCursor, GetCurrentChoice and the UI movies' FindNode and Child "
              "by their plain C++; Menu::Step, MenuSound::Play and the UI movie calls through "
              "their overrides)) "
              "natively instead of "
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

void TuningPagesFreeTexture(PPCContext& ctx, uint8_t* base) {
  const tp::TextureFreeFn free = [&ctx, base](uint32_t header_in, uint32_t data_in) -> uint64_t {
    if (ITC_FLAG(native_car_models) && NativeHeapActive() &&
        car_models::TextureFreeable(base, header_in)) {
      return car_models::TextureFree(base, header_in, data_in);
    }
    ctx.r3.u64 = header_in;
    ctx.r4.u64 = data_in;
    sub_820ECFC8(ctx, base);
    return ctx.r3.u64;
  };
  ctx.r3.u64 = tp::FreeTexture(base, ctx.r3.u64, ctx.r4.u64, free);
}

void TuningPagesRestoreVinylTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::RestoreVinylTexture(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void TuningPagesRestoreStickerTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::RestoreStickerTexture(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void TuningPagesBindVinylTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::BindVinylTexture(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void TuningPagesBindVinylTextures(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::BindVinylTextures(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesEnterEditor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::EnterEditor(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesRestoreMovieTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::RestoreMovieTexture(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void TuningPagesRestoreVinylMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::RestoreVinylMovie(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesRestoreStickerMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::RestoreStickerMovie(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesStoreVinyls(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  tp::StoreVinyls(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesLoadVinyls(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  ctx.fpscr.disableFlushMode();
  const tuning_screen::GuestCall call = StandModelsGuestCall(ctx, base);
  const uint64_t r3 = tp::LoadVinyls(base, call, screen, sp);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void TuningPagesShowPricePanel(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::ShowPricePanel(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningPagesSetBrandLogo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::SetBrandLogo(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningPagesHsvToRgb(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  tp::HsvToRgb(base, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

namespace {

// A step with the guest calls (hooks::StandModelsGuestCall), lr put back.
template <typename Body>
void WithCalls(PPCContext& ctx, uint8_t* base, Body body) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const tuning_screen::GuestCall call = StandModelsGuestCall(ctx, base);
  const uint64_t r3 = body(call);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

}  // namespace

void TuningPagesSetAeroAvailability(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::SetAeroAvailability(base, call, screen, sp, native_specs);
  });
}

void TuningPagesSetCurrentChoice(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::SetCurrentChoice(base, call, r3, sp, native_specs);
  });
}

void TuningPagesUpdateImages(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_821FE628(ctx, base);
    return;
  }
  const uint64_t images = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::UpdateImages(base, call, images, sp);
  });
}

void TuningPagesUnloadAll(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_82208738(ctx, base);
    return;
  }
  const uint64_t images = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_free = ITC_FLAG(native_car_models);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::UnloadAll(base, call, images, sp, native_free);
  });
}

void TuningPagesLoadDecalImages(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::LoadDecalImages(base, call, screen, sp);
  });
}

void TuningPagesRequestImage(PPCContext& ctx, uint8_t* base) {
  const uint64_t images = ctx.r3.u64, id = ctx.r4.u64, i = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::RequestImage(base, call, images, id, i, sp);
  });
}

void TuningPagesLoadImage(PPCContext& ctx, uint8_t* base) {
  const uint64_t images = ctx.r3.u64, id = ctx.r4.u64, i = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::LoadImage(base, call, images, id, i, sp);
  });
}

void TuningPagesStepList(PPCContext& ctx, uint8_t* base) {
  const uint64_t menu = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::StepList(base, call, menu, sp, native_slots);
  });
}

void TuningPagesLeaveEditor(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::LeaveEditor(base, call, screen, sp);
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

void TuningPagesBackToItems(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::BackToItems(base, call, screen, sp, flags);
  });
}

void TuningPagesInputWheelParts(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputWheelParts(base, call, screen, sp, flags);
  });
}

void TuningPagesAnimateWheelParts(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateWheelParts(base, call, screen, sp, native_slots);
  });
}

void TuningPagesInputInches(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputInches(base, call, screen, sp, flags);
  });
}

void TuningPagesAnimateInches(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateInches(base, call, screen, sp, native_slots);
  });
}

void TuningPagesInputItems(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputItems(base, call, screen, sp, flags);
  });
}

void TuningPagesMoveItem(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveItem(base, call, screen, sp, native_slots);
  });
}

void TuningPagesInputOptions(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputOptions(base, call, screen, sp, flags);
  });
}

void TuningPagesMoveOption(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveOption(base, call, screen, sp, native_slots);
  });
}

void TuningPagesMoveLevel(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::MoveLevel(base, call, screen, sp, native_slots);
  });
}

void TuningPagesInputLevels(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputLevels(base, call, screen, sp, flags);
  });
}

void TuningPagesInputWheels(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const tp::PageFlags flags = Flags();
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputWheels(base, call, screen, sp, flags);
  });
}

void TuningPagesAnimateWheels(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::AnimateWheels(base, call, screen, sp, native_slots);
  });
}

void TuningPagesFillPartRowsRight(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::FillPartRowsRight(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesFillPartRowsLeft(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::FillPartRowsLeft(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningPagesStoreOwnedParts(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tp::StoreOwnedParts(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningPagesLeave(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = tp::Leave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32);
}

void TuningPagesOnDialogYes(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::OnDialogYes(base, call, screen, sp, native_specs);
  });
}

void TuningPagesOnStickerDialogYes(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::OnStickerDialogYes(base, call, screen, sp);
  });
}

void TuningPagesInputCategories(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  tp::PageFlags flags;
  flags.native_slots = ITC_FLAG(native_action_mappers);
  flags.native_heap = NativeHeapActive();
  flags.native_free = ITC_FLAG(native_car_models);
  WithCalls(ctx, base, [&](const tuning_screen::GuestCall& call) {
    return tp::InputCategories(base, call, screen, sp, flags);
  });
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

ITC_TUNING_PAGES(821CCD38, TuningPagesPageCursor)
ITC_TUNING_PAGES(821CCDB0, TuningPagesPageItemCount)
ITC_TUNING_PAGES(821CCE30, TuningPagesIsPickOwned)
ITC_TUNING_PAGES(821CDF98, TuningPagesTestAndSetWheelFlag)
ITC_TUNING_PAGES(821FE6A0, TuningPagesFreeTexture)
ITC_TUNING_PAGES(821F89B8, TuningPagesRestoreStickerTexture)
ITC_TUNING_PAGES(82204730, TuningPagesBindVinylTexture)
ITC_TUNING_PAGES(821D84F8, TuningPagesEnterEditor)

ITC_TUNING_PAGES(82203F20, TuningPagesStoreVinyls)

ITC_TUNING_PAGES(8224AB50, TuningPagesHsvToRgb)

ITC_TUNING_PAGES(821FE628, TuningPagesUpdateImages)
ITC_TUNING_PAGES(82208738, TuningPagesUnloadAll)

ITC_TUNING_PAGES(821DE160, TuningPagesSetAeroAvailability)

ITC_TUNING_PAGES(821CE1D0, TuningPagesStepList)
ITC_TUNING_PAGES(821D8670, TuningPagesLeaveEditor)
ITC_TUNING_PAGES(821D6AD0, TuningPagesInputCategories)
ITC_TUNING_PAGES(821D87C0, TuningPagesBackToItems)
ITC_TUNING_PAGES(821EE390, TuningPagesInputWheelParts)
ITC_TUNING_PAGES(821EEEA8, TuningPagesInputInches)
ITC_TUNING_PAGES(821E2660, TuningPagesOnDialogYes)
ITC_TUNING_PAGES(821DFD48, TuningPagesInputItems)
ITC_TUNING_PAGES(821D8160, TuningPagesInputOptions)
ITC_TUNING_PAGES(821E25B0, TuningPagesLeave)
ITC_TUNING_PAGES(821F6688, TuningPagesInputWheels)
ITC_TUNING_PAGES(821D1A18, TuningPagesMoveLevel)
ITC_TUNING_PAGES(821D6E20, TuningPagesInputLevels)

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
