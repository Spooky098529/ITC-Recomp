// The car list natively: adapters in front of src/car_list/
// (docs/functions/car-list.md), with the register steps the harness shares
// (hooks/car_list.h).

#include "hooks/car_list.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_list/car_list.h"

ITC_FLAG_BOOL(native_car_list, true, "ITC",
              "Run the type 6 and 7 panel screens' car list (docs/functions/car-list.md: "
              "PanelGroup::SetCaption sub_8224EF18, PanelPart::FinishAll sub_82249CF0; the tabs' "
              "TabPanel::SetCursorY sub_8224FD20 and SetButtons sub_8224FD90; the rows' "
              "RowPanel::SetRow sub_8224F018, SetArrows sub_8224F680, Unhighlight sub_8224F558 "
              "and Hide sub_8224F4C8; the colors' ColorPanel::Show sub_8224D010, Select "
              "sub_8224D118, SetArrows sub_8224D2D0, ShowSwatches sub_8224D358, PlaceCursor "
              "sub_8224D3E0, SetCount sub_8224D458, SetSwatch sub_8224D4A0 and SetSwatches "
              "sub_8224D518; the car's CarPanel::SetTexts sub_8224D7F0, SetButtons sub_8224D860, "
              "PlaceCursor sub_8224D9A0, Select sub_8224DCF0, StopArrows sub_8224DF30, SetName "
              "sub_8224DED8, SetStats sub_8224E288, SetCarName sub_822417E8 and ShowStats "
              "sub_8224E920 (SetPrice the original), Panel7C::ShowArrows sub_82241868; TabPanel::SetTab "
              "sub_822501C8 and Move sub_8224FF88, RowPanel::Fill sub_8224F758, Refresh "
              "sub_8224F888 and Move sub_8224F8F8, ColorPanel::Move sub_8224D580; "
              "PowerGraph::SetEntry sub_82250368; PanelSet::Reset sub_822496C8, Picked "
              "sub_82249848, WentBack sub_82249870, WantsPowerGraph sub_82249898, SetOpenKind "
              "sub_8224B6D8, SetPreviousTab sub_8224B6E8, SetPreviousEntry sub_8224B6F0, "
              "SetPreviousColor sub_8224B6F8, PanelSetBase::PickedPage sub_82248EF0 and SaveData "
              "sub_82249668 (SaveData::Of's plain C++); PanelSetBase::ToTabs "
              "sub_82249558, PanelSet::Open sub_82249730, PanelSetBase::Free sub_8224B538 and "
              "PanelSet::Free sub_8224B880 (VirtualFree over the allocation layer, else a guest "
              "call), PanelSetBase::HandleInput sub_8224BF30 and PanelSet::HandleInput "
              "sub_8224CA28 (the pages' plain C++ where ours, else guest calls), BackPage8 "
              "sub_8224A3F8, the steps Step2 sub_82248F00, Step3 sub_8224A718, Step7 "
              "sub_82249098, Step11 sub_8224A7E0, Step18 sub_82249258 and Step26 sub_8224A8E0 "
              "(the menu lists, the screen fades and sub_825C8390 guest calls), SetUpRows "
              "sub_8224A978 and SetUpCarPage sub_8224C560 (CarPanel::ShowCarStats, never seen "
              "in play, the original), PanelSetBase::Build sub_8224C698 and PanelSet::Build "
              "sub_8224CAD8 (the entries from the allocation layer's pages); the UI "
              "movie calls through their overrides) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=car_list)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (car_list::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  car_list::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = args[0];
    ctx.r4.u64 = args[1];
    ctx.r5.u64 = args[2];
    ctx.r6.u64 = args[3];
    ctx.r7.u64 = args[4];
    ctx.r8.u64 = args[5];
    ctx.r9.u64 = args[6];
    ctx.r10.u64 = args[7];
    if (f1 != nullptr) {
      ctx.f1.f64 = *f1;
    }
    ctx.lr = lr;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

#define ITC_CL_STEP(name, expression)          \
  void name(PPCContext& ctx, uint8_t* base) {  \
    GuestCalls guest(ctx, base);               \
    const car_list::Calls calls = guest.calls(); \
    (void)calls;                               \
    const uint32_t sp = ctx.r1.u32;            \
    ctx.r3.u64 = (expression);                 \
  }

namespace cl = car_list;

ITC_CL_STEP(CarListSetCaption, cl::SetCaption(base, ctx.r3.u32, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListPartFinishAll, cl::PartFinishAll(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListTabSetCursorY, cl::TabSetCursorY(base, ctx.r3.u32, ctx.r4.u64, sp))
ITC_CL_STEP(CarListTabSetButtons, cl::TabSetButtons(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListRowSetRow,
            cl::RowSetRow(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u32, sp, calls))
ITC_CL_STEP(CarListRowSetArrows, cl::RowSetArrows(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp))
ITC_CL_STEP(CarListRowUnhighlight, cl::RowUnhighlight(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListRowHide, cl::RowHide(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListColorShow, cl::ColorShow(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListColorSelect, cl::ColorSelect(base, ctx.r3.u32, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListColorSetArrows,
            cl::ColorSetArrows(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, sp))
ITC_CL_STEP(CarListColorShowSwatches, cl::ColorShowSwatches(base, ctx.r3.u32, ctx.r4.u64, sp))
ITC_CL_STEP(CarListColorPlaceCursor, cl::ColorPlaceCursor(base, ctx.r3.u32, ctx.r4.u64, sp))
ITC_CL_STEP(CarListColorSetCount, cl::ColorSetCount(base, ctx.r3.u32, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListColorSetSwatch,
            cl::ColorSetSwatch(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u32, sp))
ITC_CL_STEP(CarListColorSetSwatches,
            cl::ColorSetSwatches(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, sp))
ITC_CL_STEP(CarListCarSetTexts, cl::CarSetTexts(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListCarSetButtons, cl::CarSetButtons(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64,
                                                    ctx.r6.u64, ctx.r7.u64, sp, calls))
ITC_CL_STEP(CarListCarPlaceCursor, cl::CarPlaceCursor(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_CL_STEP(CarListCarSelect, cl::CarSelect(base, ctx.r3.u32, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListCarStopArrows, cl::CarStopArrows(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListCarSetName, cl::CarSetName(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListCarSetStats, cl::CarSetStats(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListCarSetCarName,
            cl::CarSetCarName(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, sp, calls))
ITC_CL_STEP(CarListPanel7CShowArrows, cl::Panel7CShowArrows(base, ctx.r3.u32, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListCarShowStats, cl::CarShowStats(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListTabSetTab, cl::TabSetTab(base, ctx.r3.u32, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListTabMove, cl::TabMove(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, sp,
                                        calls))
ITC_CL_STEP(CarListRowFill,
            cl::RowFill(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, ctx.r6.u64, sp, calls))
ITC_CL_STEP(CarListRowRefresh, cl::RowRefresh(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, sp, calls))
ITC_CL_STEP(CarListRowMove, cl::RowMove(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                                        ctx.r7.u64, ctx.r9.u32, sp, calls))
ITC_CL_STEP(CarListColorMove,
            cl::ColorMove(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                          ctx.r8.u64, ctx.r9.u64, ctx.r10.u32, sp, calls))
ITC_CL_STEP(CarListPowerGraphSetEntry, cl::PowerGraphSetEntry(base, ctx.r3.u64, ctx.r4.u64))
ITC_CL_STEP(CarListSetReset, cl::SetReset(base, ctx.r3.u64))
ITC_CL_STEP(CarListSetPicked, cl::SetPicked(base, ctx.r3.u32))
ITC_CL_STEP(CarListSetWentBack, cl::SetWentBack(base, ctx.r3.u32))
ITC_CL_STEP(CarListSetWantsPowerGraph, cl::SetWantsPowerGraph(base, ctx.r3.u32))
ITC_CL_STEP(CarListSetOpenKind, cl::SetWord(base, ctx.r3.u64, 1932, ctx.r4.u64))
ITC_CL_STEP(CarListSetPreviousTab, cl::SetWord(base, ctx.r3.u64, 1884, ctx.r4.u64))
ITC_CL_STEP(CarListSetPreviousEntry, cl::SetWord(base, ctx.r3.u64, 1888, ctx.r4.u64))
ITC_CL_STEP(CarListSetPreviousColor, cl::SetWord(base, ctx.r3.u64, 1892, ctx.r4.u64))
ITC_CL_STEP(CarListPickedPage, cl::PickedPage(base, ctx.r3.u64))
ITC_CL_STEP(CarListSaveData, cl::SaveData(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListToTabs, cl::ToTabs(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListOpen, cl::Open(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListBaseFree, cl::Free(base, ctx.r3.u64, sp, calls))
// PanelSet::Free and HandleInput are tail calls of the base's on the set
// + 4, with no plain C++ of their own: their prints are here.
void CarListSetFree(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8224B880 PanelSet::Free");
  GuestCalls guest(ctx, base);
  const car_list::Calls calls = guest.calls();
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cl::Free(base, ctx.r3.u64 + 4, sp, calls);
}
ITC_CL_STEP(CarListBaseHandleInput, cl::HandleInput(base, ctx.r3.u64, sp, calls))
void CarListSetHandleInput(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8224CA28 PanelSet::HandleInput");
  GuestCalls guest(ctx, base);
  const car_list::Calls calls = guest.calls();
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cl::HandleInput(base, ctx.r3.u64 + 4, sp, calls);
}
ITC_CL_STEP(CarListBackPage8, cl::BackPage8(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListStep2, cl::Step2(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep3, cl::Step3(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep7, cl::Step7(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep11, cl::Step11(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep18, cl::Step18(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep26, cl::Step26(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListSetUpRows, cl::SetUpRows(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListSetUpCarPage, cl::SetUpCarPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListBaseBuild, cl::Build(base, ctx.r3.u64, sp, calls))
// PanelSet::Build: a tail call of the base's on the set + 4.
void CarListSetBuild(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8224CAD8 PanelSet::Build");
  GuestCalls guest(ctx, base);
  const car_list::Calls calls = guest.calls();
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cl::Build(base, ctx.r3.u64 + 4, sp, calls);
}

#undef ITC_CL_STEP

}  // namespace hooks

#define ITC_CAR_LIST(address, step)       \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_car_list)) {     \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

// The panel groups and panels (from the car list's functions, recompiled,
// and the C panels' vtables). PanelGroup::SetCaption sub_8224EF18,
// TabPanel::SetCursorY sub_8224FD20, RowPanel::SetRow sub_8224F018 and
// SetArrows sub_8224F680, ColorPanel::ShowSwatches sub_8224D358 and
// SetSwatch sub_8224D4A0, and (the second removal) PanelSetBase::ToTabs
// sub_82249558, Free sub_8224B538, HandleInput sub_8224BF30,
// PanelPart::FinishAll sub_82249CF0, TabPanel::Move sub_8224FF88,
// RowPanel::Move sub_8224F8F8, CarPanel::Select sub_8224DCF0 and
// ColorPanel::Select sub_8224D118, and (the third) RowPanel::Fill
// sub_8224F758 and Refresh sub_8224F888, CarPanel::SetTexts sub_8224D7F0
// and SetName sub_8224DED8 have no adapter (removed, B7 loop, 2026-11-08):
// in play only native callers reach them; the register steps stay for the
// harness.
ITC_CAR_LIST(8224FD90, CarListTabSetButtons)
ITC_CAR_LIST(8224F558, CarListRowUnhighlight)
ITC_CAR_LIST(8224F4C8, CarListRowHide)
ITC_CAR_LIST(8224D010, CarListColorShow)
ITC_CAR_LIST(8224D2D0, CarListColorSetArrows)
ITC_CAR_LIST(8224D3E0, CarListColorPlaceCursor)
ITC_CAR_LIST(8224D458, CarListColorSetCount)
ITC_CAR_LIST(8224D518, CarListColorSetSwatches)
ITC_CAR_LIST(8224D860, CarListCarSetButtons)
ITC_CAR_LIST(8224D9A0, CarListCarPlaceCursor)
ITC_CAR_LIST(8224DF30, CarListCarStopArrows)
ITC_CAR_LIST(8224E288, CarListCarSetStats)
ITC_CAR_LIST(822417E8, CarListCarSetCarName)
ITC_CAR_LIST(82241868, CarListPanel7CShowArrows)
ITC_CAR_LIST(8224E920, CarListCarShowStats)
ITC_CAR_LIST(822501C8, CarListTabSetTab)
ITC_CAR_LIST(8224D580, CarListColorMove)
// PowerGraph::SetEntry (from PanelSetBase::PowerGraphPage, recompiled).
ITC_CAR_LIST(82250368, CarListPowerGraphSetEntry)
// PanelSet's vtable slots 2, 8 to 10, 13, 15 to 17, the page 5 handler (from
// the page table) and PanelSetBase::SaveData (from the set's functions).
ITC_CAR_LIST(822496C8, CarListSetReset)
ITC_CAR_LIST(82249848, CarListSetPicked)
ITC_CAR_LIST(82249870, CarListSetWentBack)
ITC_CAR_LIST(82249898, CarListSetWantsPowerGraph)
ITC_CAR_LIST(8224B6D8, CarListSetOpenKind)
ITC_CAR_LIST(8224B6E8, CarListSetPreviousTab)
ITC_CAR_LIST(8224B6F0, CarListSetPreviousEntry)
ITC_CAR_LIST(8224B6F8, CarListSetPreviousColor)
ITC_CAR_LIST(82248EF0, CarListPickedPage)
ITC_CAR_LIST(82249668, CarListSaveData)
// PanelSet's slots 3, 5 and 7, the base's (from recompiled callers), the
// page 8 handler (the page table) and the steps (the step table).
ITC_CAR_LIST(82249730, CarListOpen)
ITC_CAR_LIST(8224B880, CarListSetFree)
ITC_CAR_LIST(8224CA28, CarListSetHandleInput)
ITC_CAR_LIST(8224A3F8, CarListBackPage8)
ITC_CAR_LIST(82248F00, CarListStep2)
ITC_CAR_LIST(8224A718, CarListStep3)
ITC_CAR_LIST(82249098, CarListStep7)
ITC_CAR_LIST(8224A7E0, CarListStep11)
ITC_CAR_LIST(82249258, CarListStep18)
ITC_CAR_LIST(8224A8E0, CarListStep26)
// The rows' and the car page's set-ups (from recompiled steps and pages).
ITC_CAR_LIST(8224A978, CarListSetUpRows)
ITC_CAR_LIST(8224C560, CarListSetUpCarPage)
// PanelSet's slot 1 (the panel screens' show) and the base's Build.
ITC_CAR_LIST(8224CAD8, CarListSetBuild)
ITC_CAR_LIST(8224C698, CarListBaseBuild)
