// The car list natively: adapters in front of src/car_list/
// (docs/functions/car-list.md), with the register steps the harness shares
// (hooks/car_list.h).

#include "hooks/car_list.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_list/car_list.h"
#include "hooks/car_makers.h"

ITC_FLAG_BOOL(native_car_list, true, "ITC",
              "Run the type 6 and 7 panel screens' car list (docs/functions/car-list.md: "
              "PanelGroup::SetCaption sub_8224EF18, PanelPart::FinishAll sub_82249CF0; the "
              "destructors PanelGroupBase::~PanelGroupBase sub_8224EF08, PanelSetBase::~PanelSetBase "
              "sub_8224AE60 (MenuList::Release guest calls), PanelSet::~PanelSet sub_8224B708 and "
              "PanelSetInterface::DeletingDestructor sub_8224AC98 (operator delete a guest call); "
              "the tabs' "
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
              "PowerGraph::SetEntry sub_82250368; the panels' answers TabPanel::IsShown sub_8224FCE0, "
              "RowPanel::IsShown sub_8224F5E8 and IsHidden sub_8224F640, CarPanel::IsShown "
              "sub_8224E108, ColorPanel::IsShown sub_8224D1F0 and IsIdle sub_8224D288 "
              "(UiMovie::IsNamedActive's plain C++), and their TabPanel::Show sub_8224FE50 and Select "
              "sub_8224FB20, RowPanel::Show sub_8224F1D8 and Select sub_8224F340, CarPanel::Show "
              "sub_8224DA20, Move sub_8224DDF8 and Pick sub_8224E050 (UiMovie::UnloopNamed "
              "through its override); the steps Step0 sub_8224A570, Step1 sub_8224A6A0, Step4 "
              "sub_8224B078, Step6 sub_82248F70, Step8 sub_82248FE0, Step9 sub_8224CB28, Step12 "
              "sub_82249128 and Step19 sub_822492C8 (GarageStage::SetDrawWord's plain C++); "
              "PanelSet::Reset sub_822496C8, Picked "
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
              "sub_8224CAD8 (the entries from the allocation layer's pages), "
              "PanelSetBase::ShowStandCar sub_8224A490 and SetUpColors sub_8224B248 (with the car makers' callees native: "
              "GarageStage::SetCarClass through its override), the pages TabsPage "
              "sub_82249EB8, RowsPage sub_8224A178, ColorsPage sub_8224AEA8, CarPage sub_8224B8D8 "
              "(ShowStandCar's and PowerGraph::SetFromCar sub_82250488's plain C++; "
              "CarPanel::ShowCarStats, never seen in play, the original) and PowerGraphPage sub_8224BDC0, step 17 sub_8224C450 (SetUpColors' "
              "plain C++, with the stand's gate) (the menu list, MenuSound::Play and the power graphs' set-ups guest "
              "calls), GarageStages::DrawAll sub_821807D8, PanelSetBase::UpdateStand sub_8224C008 and "
              "Update sub_8224CD00 and PanelSet::Update sub_8224CF18 (the steps' plain C++ where "
              "ours; GarageStage::ReloadModel, SetCar, Command, IsBusy and Draw through their "
              "overrides; with the car makers' callees native); PanelSetPair's constructor "
              "sub_8224B750 (PanelSetBase::PanelSetBase's plain C++ on both bases), "
              "destructor sub_8224B830, DeletingDestructor sub_8224CA30, Build sub_8224CAE0, "
              "Reset sub_822498C0, Open sub_822499F0, HandleInput sub_8224CA80, Update "
              "sub_8224CF20 and Picked sub_82249B60 (the bases' plain C++ on both), "
              "CarPanelBase::~CarPanelBase sub_8224D7E0 and SetCarName sub_8224E878, "
              "Panel6C::SetStats sub_8224E490 and ConfirmPanel::ConfirmPanel sub_8224EBB0; the UI "
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
ITC_CL_STEP(CarListGroupBaseDestruct, cl::GroupBaseDestruct(base, ctx.r3.u64))
ITC_CL_STEP(CarListSetBaseDestruct, cl::SetBaseDestruct(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListSetDestruct, cl::SetDestruct(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListSetInterfaceDeletingDestruct,
            cl::SetInterfaceDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
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
ITC_CL_STEP(CarListTabIsShown, cl::TabIsShown(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListRowIsShown, cl::RowIsShown(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListRowIsHidden, cl::RowIsHidden(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListCarIsShown, cl::CarIsShown(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListColorIsShown, cl::ColorIsShown(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListColorIsIdle, cl::ColorIsIdle(base, ctx.r3.u32, sp))
ITC_CL_STEP(CarListTabShow, cl::TabShow(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListTabSelect, cl::TabSelect(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListRowShow, cl::RowShow(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListRowSelect, cl::RowSelect(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListCarShow, cl::CarShow(base, ctx.r3.u32, sp, calls))
ITC_CL_STEP(CarListCarMove,
            cl::CarMove(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, sp, calls))
ITC_CL_STEP(CarListCarPick, cl::CarPick(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp, calls))
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
ITC_CL_STEP(CarListStep0, cl::Step0(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep1, cl::Step1(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep4, cl::Step4(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep6, cl::Step6(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep8, cl::Step8(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep9, cl::Step9(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep12, cl::Step12(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListStep19, cl::Step19(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListSetUpRows, cl::SetUpRows(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListSetUpCarPage, cl::SetUpCarPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListBaseBuild, cl::Build(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListTabsPage, cl::TabsPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListRowsPage, cl::RowsPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListColorsPage, cl::ColorsPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListPowerGraphPage, cl::PowerGraphPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListSetUpColors, cl::SetUpColors(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListStep17, cl::Step17(base, ctx.r3.u64, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListCarPage, cl::CarPage(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListShowStandCar, cl::ShowStandCar(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListDrawAll, cl::DrawAll(base, sp, calls))
ITC_CL_STEP(CarListUpdateStand, cl::UpdateStand(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListBaseUpdate, cl::Update(base, ctx.r3.u64, sp, calls))
// PanelSet::Update: a tail call of the base's on the set + 4.
void CarListSetUpdate(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8224CF18 PanelSet::Update");
  GuestCalls guest(ctx, base);
  const car_list::Calls calls = guest.calls();
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cl::Update(base, ctx.r3.u64 + 4, sp, calls);
}
// PanelSet::Build: a tail call of the base's on the set + 4.
void CarListSetBuild(PPCContext& ctx, uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8224CAD8 PanelSet::Build");
  GuestCalls guest(ctx, base);
  const car_list::Calls calls = guest.calls();
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cl::Build(base, ctx.r3.u64 + 4, sp, calls);
}

// PanelSetPair (never seen in play), the C panels' base and the confirm
// panel.
ITC_CL_STEP(CarListBaseSetCarName,
            cl::BaseSetCarName(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, sp, calls))
ITC_CL_STEP(CarListPanel6CSetStats, cl::Panel6CSetStats(base, ctx.r3.u32, ctx.r4.u32, sp, calls))
ITC_CL_STEP(CarListCarPanelBaseDestruct, cl::CarPanelBaseDestruct(base, ctx.r3.u64))
ITC_CL_STEP(CarListConfirmPanelConstruct, cl::ConfirmPanelConstruct(base, ctx.r3.u64))
ITC_CL_STEP(CarListPairConstruct,
            cl::PairConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                              ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, sp))
ITC_CL_STEP(CarListPairDestruct, cl::PairDestruct(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListPairDeletingDestruct,
            cl::PairDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_CL_STEP(CarListPairReset, cl::PairReset(base, ctx.r3.u64, sp))
ITC_CL_STEP(CarListPairOpen, cl::PairOpen(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListPairHandleInput, cl::PairHandleInput(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListPairUpdate, cl::PairUpdate(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListPairBuild, cl::PairBuild(base, ctx.r3.u64, sp, calls))
ITC_CL_STEP(CarListPairPicked, cl::PairPicked(base, ctx.r3.u32))

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
// SetSwatch sub_8224D4A0, ColorPanel::PlaceCursor sub_8224D3E0 and SetCount
// sub_8224D458 (the fourth: their callers in play, SetUpColors and
// ColorPanel::Move, are native), and (the second removal) PanelSetBase::ToTabs
// sub_82249558, Free sub_8224B538, HandleInput sub_8224BF30,
// PanelPart::FinishAll sub_82249CF0, TabPanel::Move sub_8224FF88,
// RowPanel::Move sub_8224F8F8, CarPanel::Select sub_8224DCF0 and
// ColorPanel::Select sub_8224D118, and (the third) RowPanel::Fill
// sub_8224F758 and Refresh sub_8224F888, CarPanel::SetTexts sub_8224D7F0
// and SetName sub_8224DED8, and (the fourth, after the steps over the
// panels) TabPanel::SetTab sub_822501C8, Show sub_8224FE50, IsShown
// sub_8224FCE0, Select sub_8224FB20 and SetButtons sub_8224FD90,
// RowPanel::Show sub_8224F1D8, IsShown sub_8224F5E8, Hide sub_8224F4C8,
// IsHidden sub_8224F640 and Select sub_8224F340, CarPanel::Show
// sub_8224DA20, IsShown sub_8224E108, Move sub_8224DDF8 and PlaceCursor
// sub_8224D9A0, ColorPanel::Move sub_8224D580, IsIdle sub_8224D288,
// SetArrows sub_8224D2D0 and SetSwatches sub_8224D518, PanelSetBase::
// SetUpRows sub_8224A978 and SetUpCarPage sub_8224C560 have no adapter
// (removed, B7 loop, 2026-11-08): in play only native callers reach them;
// the register steps stay for the harness.
// The C panels' vtable slots 1 to 3. RowPanel::Unhighlight sub_8224F558,
// ColorPanel::Show sub_8224D010 and IsShown sub_8224D1F0, CarPanel::
// SetButtons sub_8224D860, StopArrows sub_8224DF30, ShowStats sub_8224E920
// and Pick sub_8224E050, PowerGraph::SetEntry sub_82250368 and
// PanelSetBase::SaveData sub_82249668 have no adapter (removed, B7 loop,
// 2026-11-08, after step 17 and the car's page: in play only native
// callers reach them; the register steps stay for the harness).
ITC_CAR_LIST(8224E288, CarListCarSetStats)
ITC_CAR_LIST(822417E8, CarListCarSetCarName)
ITC_CAR_LIST(82241868, CarListPanel7CShowArrows)
// PanelSet's vtable slots 2, 8 to 10, 13, 15 to 17 and the page 5 handler
// (from the page table).
ITC_CAR_LIST(822496C8, CarListSetReset)
ITC_CAR_LIST(82249848, CarListSetPicked)
ITC_CAR_LIST(82249870, CarListSetWentBack)
ITC_CAR_LIST(82249898, CarListSetWantsPowerGraph)
ITC_CAR_LIST(8224B6D8, CarListSetOpenKind)
ITC_CAR_LIST(8224B6E8, CarListSetPreviousTab)
ITC_CAR_LIST(8224B6F0, CarListSetPreviousEntry)
ITC_CAR_LIST(8224B6F8, CarListSetPreviousColor)
ITC_CAR_LIST(82248EF0, CarListPickedPage)
// PanelSet's slots 3, 5 and 7, the base's (from recompiled callers), the
// page 8 handler (the page table) and the steps (the step table).
ITC_CAR_LIST(82249730, CarListOpen)
ITC_CAR_LIST(8224B880, CarListSetFree)
ITC_CAR_LIST(8224CA28, CarListSetHandleInput)
ITC_CAR_LIST(8224A3F8, CarListBackPage8)
// The destructors (from the panel screens' destructors, PanelSetPair's and
// the vtables' slot 0, recompiled; never seen in play).
ITC_CAR_LIST(8224EF08, CarListGroupBaseDestruct)
ITC_CAR_LIST(8224AE60, CarListSetBaseDestruct)
ITC_CAR_LIST(8224B708, CarListSetDestruct)
ITC_CAR_LIST(8224AC98, CarListSetInterfaceDeletingDestruct)
ITC_CAR_LIST(82248F00, CarListStep2)
ITC_CAR_LIST(8224A718, CarListStep3)
ITC_CAR_LIST(82249098, CarListStep7)
ITC_CAR_LIST(8224A7E0, CarListStep11)
ITC_CAR_LIST(82249258, CarListStep18)
ITC_CAR_LIST(8224A8E0, CarListStep26)
// The steps over the panels (the step table: PanelSetBase::Update's plain
// C++ calls theirs; PanelSetPair's, never seen in play, the original's).
ITC_CAR_LIST(8224A570, CarListStep0)
ITC_CAR_LIST(8224A6A0, CarListStep1)
ITC_CAR_LIST(8224B078, CarListStep4)
ITC_CAR_LIST(82248F70, CarListStep6)
ITC_CAR_LIST(82248FE0, CarListStep8)
ITC_CAR_LIST(8224CB28, CarListStep9)
ITC_CAR_LIST(82249128, CarListStep12)
ITC_CAR_LIST(822492C8, CarListStep19)
// The rows' and the car page's set-ups (from recompiled steps and pages).
// PanelSet's slot 1 (the panel screens' show). PanelSetBase::Build
// sub_8224C698 has no adapter (removed, B7 loop, 2026-11-08: in play only
// PanelSet::Build, native, reaches it; the register step stays for the
// harness).
ITC_CAR_LIST(8224CAD8, CarListSetBuild)
// The pages 0, 1, 3 and 6 (from the page table: PanelSetBase::HandleInput's
// plain C++ calls theirs; PanelSetPair's, never seen in play, the original's
// table).
ITC_CAR_LIST(82249EB8, CarListTabsPage)
ITC_CAR_LIST(8224A178, CarListRowsPage)
ITC_CAR_LIST(8224AEA8, CarListColorsPage)
ITC_CAR_LIST(8224BDC0, CarListPowerGraphPage)
// The car's page (page 2, the same): its stand's car through ShowStandCar's
// plain C++, as ColorsPage's.
ITC_CAR_LIST(8224B8D8, CarListCarPage)
// PanelSetBase::ShowStandCar sub_8224A490 and SetUpColors sub_8224B248
// have no adapter (removed, B7 loop, 2026-11-08: in play only the car and
// colors pages', SetUpColors' and step 17's plain C++ reach them; step 25,
// never seen in play, runs SetUpColors' original).
// Step 17 (from the step table: PanelSetBase::Update's plain C++ calls its;
// PanelSetPair's, never seen in play, the original's): the original when the
// car makers' callees aren't native (SetUpColors' and ShowStandCar's plain
// C++).
extern "C" REX_FUNC(sub_8224C450) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_list) || !hooks::CarMakersShowCarByIdCalleesNative(true)) {
    __imp__sub_8224C450(ctx, base);
    return;
  }
  hooks::CarListStep17(ctx, base);
}
// PanelSet's slot 6 (the panel screens' update): the same gate as the
// stand's car (Car::Init, SetModel and Refresh's plain C++). PanelSetBase::
// Update sub_8224CD00, UpdateStand sub_8224C008 and GarageStages::DrawAll
// sub_821807D8 have no adapter (removed, B7 loop, 2026-11-08: in play only
// PanelSet::Update's and their native callers' plain C++ reach them; DrawAll's
// two type 6 callers, not seen in play, run its original; the register steps
// stay for the harness).
#define ITC_CAR_LIST_STAND(address, step)                                                     \
  extern "C" REX_FUNC(sub_##address) {                                                       \
    REX_FUNC_PROLOGUE();                                                                     \
    if (!ITC_FLAG(native_car_list) || !hooks::CarMakersShowCarByIdCalleesNative(true)) {      \
      __imp__sub_##address(ctx, base);                                                       \
      return;                                                                                \
    }                                                                                        \
    hooks::step(ctx, base);                                                                  \
  }
ITC_CAR_LIST_STAND(8224CF18, CarListSetUpdate)
// PanelSetPair's slot 6 (never seen in play): the same gate.
ITC_CAR_LIST_STAND(8224CF20, CarListPairUpdate)
#undef ITC_CAR_LIST_STAND
// PanelSetPair (the type 8 mode's panel screen's car list, never seen in
// play): its constructor and destructor (from Type8PanelScreen's, recompiled)
// and its vtable slots 0 to 3, 5 and 8; the C panels' base's
// ~CarPanelBase (from the C panels' destructors) and SetCarName (slot 2 of
// its vtable and Panel8C's), Panel6C::SetStats (from Panel8C::SetStats) and
// ConfirmPanel::ConfirmPanel (from ConfirmPanel8's).
ITC_CAR_LIST(8224B750, CarListPairConstruct)
ITC_CAR_LIST(8224B830, CarListPairDestruct)
ITC_CAR_LIST(8224CA30, CarListPairDeletingDestruct)
ITC_CAR_LIST(8224CAE0, CarListPairBuild)
ITC_CAR_LIST(822498C0, CarListPairReset)
ITC_CAR_LIST(822499F0, CarListPairOpen)
ITC_CAR_LIST(8224CA80, CarListPairHandleInput)
ITC_CAR_LIST(82249B60, CarListPairPicked)
ITC_CAR_LIST(8224D7E0, CarListCarPanelBaseDestruct)
ITC_CAR_LIST(8224EBB0, CarListConfirmPanelConstruct)
ITC_CAR_LIST(8224E490, CarListPanel6CSetStats)
ITC_CAR_LIST(8224E878, CarListBaseSetCarName)
