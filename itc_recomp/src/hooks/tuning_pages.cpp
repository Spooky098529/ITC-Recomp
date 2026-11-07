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

ITC_FLAG_BOOL(native_tuning_pages, true, "ITC",
              "Run the tuning screen's pages' functions (docs/functions/tuning-pages.md: "
              "TuningScreen::LevelCount sub_821CC728, TypeCount sub_821CC7B8, PageCursor "
              "sub_821CCD38 (its piece sub_821CCD70 merged), PageItemCount sub_821CCDB0 (its piece "
              "sub_821CCDE8 merged), IsPickOwned sub_821CCE30, MarkOwned sub_821CD498 (its piece "
              "sub_821CD6C8 merged), TestAndSetWheelFlag sub_821CDF98, SetWheelListEnd "
              "sub_821E7DB0 (its piece sub_821E7DE4 merged), InchListEnd sub_821EF288 (its piece "
              "sub_821EF2FC merged); the vinyl editor's set-up SetUpVinylBusyIcon sub_821FE560, "
              "DecalImages::AllLoaded sub_821FE5D8, KeepListTextures sub_821FE7D8 (these three "
              "and BindVinylTextures with no adapters: only EnterEditor calls them), "
              "SetEditorLimits sub_821FE420 and LoadPageLayouts sub_822045F8; the decal images' "
              "textures DecalImages::FreeTexture sub_821FE6A0 (Texture::Free by its plain C++ with "
              "--native_car_models and the native heap), RestoreVinylTexture sub_82204698, "
              "RestoreStickerTexture sub_821F89B8, BindVinylTexture sub_82204730, "
              "BindVinylTextures sub_82203D40 and EnterEditor sub_821D84F8 (FadeIn's plain C++); the "
              "vinyls' records StoreVinyls sub_82203F20 and LoadVinyls sub_822040A8 (memcpy's plain "
              "C++ where Buffers_Memcpy takes it); ShowPricePanel sub_821D9E68, SetBrandLogo "
              "sub_821EED98 (its piece sub_821EEE14 merged), movie 2's nodes through the UI "
              "movies' plain C++; Color::HsvToRgb sub_8224AB50) "
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
ITC_TUNING_PAGES(821FE420, TuningPagesSetEditorLimits)
ITC_TUNING_PAGES(822045F8, TuningPagesLoadPageLayouts)
ITC_TUNING_PAGES(821FE6A0, TuningPagesFreeTexture)
ITC_TUNING_PAGES(82204698, TuningPagesRestoreVinylTexture)
ITC_TUNING_PAGES(821F89B8, TuningPagesRestoreStickerTexture)
ITC_TUNING_PAGES(82204730, TuningPagesBindVinylTexture)
ITC_TUNING_PAGES(821D84F8, TuningPagesEnterEditor)

ITC_TUNING_PAGES(82203F20, TuningPagesStoreVinyls)
ITC_TUNING_PAGES(822040A8, TuningPagesLoadVinyls)

ITC_TUNING_PAGES(821D9E68, TuningPagesShowPricePanel)
ITC_TUNING_PAGES(821EED98, TuningPagesSetBrandLogo)
ITC_TUNING_PAGES(8224AB50, TuningPagesHsvToRgb)

// No adapters (removed, B7 loop, 2026-11-07: their only caller in play,
// EnterEditor, is native and calls their plain C++; the register steps above
// are the harness's): SetUpVinylBusyIcon sub_821FE560, DecalImages::AllLoaded
// sub_821FE5D8, KeepListTextures sub_821FE7D8, BindVinylTextures sub_82203D40.
