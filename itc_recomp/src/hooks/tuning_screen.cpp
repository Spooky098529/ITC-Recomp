// The tuning screen natively: adapters in front of src/tuning_screen/
// (docs/functions/tuning-screen.md), with the register steps the harness
// shares (hooks/tuning_screen.h). The screen's update and refresh, and other
// front-end screens, are still recompiled and call these, so the adapters
// stay.

#include "hooks/tuning_screen.h"

#include "base/flags.h"
#include "tuning_screen/tuning_screen.h"

ITC_FLAG_BOOL(native_tuning_screen, true, "ITC",
              "Run the tuning screen's functions (docs/functions/tuning-screen.md: the part "
              "tables' TuningScreen::CategoryItemCount sub_821CC6C0, IsItemAvailable "
              "sub_821CDB18, WheelNumber sub_821CDED8, GetPrice sub_821CD160, GetCurrentChoice "
              "sub_821CD868 and GetChoiceFlag sub_821CDC38; the debug state handlers, whose prints "
              "through the empty sub_82106540 are left out: DebugMainMenu sub_821CBE08, "
              "DebugLevelSelect sub_821CBEE8, DebugTypeSelect sub_821CBFC0, DebugOptionSelect "
              "sub_821CC248, DebugConfirmBuy sub_821CC3A8, DebugConfirmEquip sub_821CC488, "
              "DebugCategoryMenu sub_821CFCE8, DebugWheelPartSelect sub_821E8068, "
              "DebugWheelKindSelect sub_821E8128, DebugWheelInchSelect sub_821E8298, "
              "DebugDecalTopMenu sub_821FD710, DebugLayerSelect sub_821FD880, DebugLayerEdit "
              "sub_821FDCB8, DebugLayerSet sub_821FE080, DebugEditPosition sub_821FE100, "
              "DebugEditView sub_821FE178, DebugPaintConfirm sub_821FE190; the selected garage car's "
              "StoreToGarageCar sub_821DCC28 and EquipChoice sub_821DF448) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=tuning_screen)");

namespace hooks {

namespace ts = tuning_screen;

void TuningCategoryItemCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::CategoryItemCount(base, ctx.r3.u32);
}

void TuningIsItemAvailable(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::IsItemAvailable(base, ctx.r3.u32, ctx.r4.u64);
}

void TuningWheelNumber(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::WheelNumber(ctx.r4.u64, ctx.r5.u64);
}

void TuningGetPrice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetPrice(base, ctx.r3.u32);
}

void TuningGetCurrentChoice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetCurrentChoice(base, ctx.r3.u32);
}

void TuningGetChoiceFlag(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetChoiceFlag(base, ctx.r3.u32, ctx.r4.u64);
}

void TuningDebugMainMenu(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugMainMenu(base, ctx.r1.u32);
}

void TuningDebugLevelSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLevelSelect(base, ctx.r1.u32);
}

void TuningDebugTypeSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugTypeSelect(base, ctx.r1.u32);
}

void TuningDebugOptionSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugOptionSelect(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugConfirmBuy(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugConfirmBuy(base, ctx.r1.u32);
}

void TuningDebugConfirmEquip(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugConfirmEquip(base, ctx.r1.u32);
}

void TuningDebugCategoryMenu(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugCategoryMenu(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugWheelPartSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWheelPartSelect(base, ctx.r1.u32);
}

void TuningDebugWheelKindSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWheelKindSelect(base, ctx.r1.u32);
}

void TuningDebugWheelInchSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWheelInchSelect(base, ctx.r1.u32);
}

void TuningDebugDecalTopMenu(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugDecalTopMenu(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugLayerSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerSelect(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugLayerEdit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerEdit(base, ctx.r1.u32);
}

void TuningDebugLayerSet(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerSet(base, ctx.r1.u32);
}

void TuningDebugEditPosition(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditPosition();
}

void TuningDebugEditView(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditView();
}

void TuningDebugPaintConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugPaintConfirm(base, ctx.r1.u32);
}

void TuningStoreToGarageCar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ts::StoreToGarageCar(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningEquipChoice(PPCContext& ctx, uint8_t* base) { ts::EquipChoice(base, ctx.r3.u32); }

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_TUNING_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_tuning_screen)) { \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

ITC_TUNING_SCREEN(821CC6C0, TuningCategoryItemCount)
ITC_TUNING_SCREEN(821CDB18, TuningIsItemAvailable)
ITC_TUNING_SCREEN(821CDED8, TuningWheelNumber)
ITC_TUNING_SCREEN(821CD160, TuningGetPrice)
ITC_TUNING_SCREEN(821CD868, TuningGetCurrentChoice)
ITC_TUNING_SCREEN(821CDC38, TuningGetChoiceFlag)
ITC_TUNING_SCREEN(821CBE08, TuningDebugMainMenu)
ITC_TUNING_SCREEN(821CBEE8, TuningDebugLevelSelect)
ITC_TUNING_SCREEN(821CBFC0, TuningDebugTypeSelect)
ITC_TUNING_SCREEN(821CC248, TuningDebugOptionSelect)
ITC_TUNING_SCREEN(821CC3A8, TuningDebugConfirmBuy)
ITC_TUNING_SCREEN(821CC488, TuningDebugConfirmEquip)
ITC_TUNING_SCREEN(821CFCE8, TuningDebugCategoryMenu)
ITC_TUNING_SCREEN(821E8068, TuningDebugWheelPartSelect)
ITC_TUNING_SCREEN(821E8128, TuningDebugWheelKindSelect)
ITC_TUNING_SCREEN(821E8298, TuningDebugWheelInchSelect)
ITC_TUNING_SCREEN(821FD710, TuningDebugDecalTopMenu)
ITC_TUNING_SCREEN(821FD880, TuningDebugLayerSelect)
ITC_TUNING_SCREEN(821FDCB8, TuningDebugLayerEdit)
ITC_TUNING_SCREEN(821FE080, TuningDebugLayerSet)
ITC_TUNING_SCREEN(821FE100, TuningDebugEditPosition)
ITC_TUNING_SCREEN(821FE178, TuningDebugEditView)
ITC_TUNING_SCREEN(821FE190, TuningDebugPaintConfirm)
ITC_TUNING_SCREEN(821DCC28, TuningStoreToGarageCar)
ITC_TUNING_SCREEN(821DF448, TuningEquipChoice)
