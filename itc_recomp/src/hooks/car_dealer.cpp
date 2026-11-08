// The car dealer natively: adapters in front of src/car_dealer/
// (docs/functions/car-dealer.md), with the register steps the harness shares
// (hooks/car_dealer.h).

#include "hooks/car_dealer.h"

#include "base/flags.h"
#include "car_dealer/car_dealer.h"

ITC_FLAG_BOOL(native_car_dealer, true, "ITC",
              "Run the car dealer (docs/functions/car-dealer.md: the type 2 mode's screen "
              "machine where the player buys a car) natively instead of through the translated "
              "code: CarDealer::InitStates sub_82185920 over StateMachine::Add's plain C++; the "
              "handlers Create sub_82180D68 (VirtualAlloc through its override), OpenMovie "
              "sub_82180C08 (UiMovies::Open through its override), WaitForLoad sub_82180F20, "
              "AfterDetail sub_82180C48 and Page4 sub_82181B98; the helpers CheckLimits "
              "sub_821812B8, ReloadStandModel sub_82181330 (CarModels::BlockOf and "
              "GarageStage::ReloadModel through their overrides), SetKindBar sub_82182158, "
              "SetColorBar sub_82182770, SetScrollArrows sub_82182DD0, SetHeader sub_82182C28, "
              "SetPrompts sub_82182B30 and HelpLine::Set sub_82199B38 (UiMovie::SetText, its "
              "unset varargs 0, ScrollText and the scroll reset through their overrides); "
              "Unload sub_821816B8, Close sub_82181838 and Destroy sub_82180E70 (VirtualFree, "
              "MenuList::Release, UiMovie::FinishAll and UiMovies::Delete through their "
              "overrides), OnYes sub_82181968, UpdatePage sub_82184AF0 (the page's handler "
              "through the guest context), StartFirstRace sub_82180CE8 (Career::SetUpEvent and "
              "Race::SetUp through their overrides), BuildLists sub_821813B0 (VirtualAlloc and "
              "memcpy through their overrides) and Buy sub_82182E68 (the garage car's and the "
              "career's calls through their overrides); Leave sub_821817C0 (StartFirstRace's plain "
              "C++), Transition3 sub_82181D68 (ScreenFades::FadeOut and UiMovie::Run through "
              "their overrides); ShowCarInfo sub_82182620 with SetSpecTexts sub_821824A8, "
              "SetNameTexts sub_82182388 and SetPriceTexts sub_82182418 (the setters' unset "
              "varargs 0), ShowSwatches sub_821826C0 with SetSwatchColor sub_82181270, and "
              "FillStatus sub_82183AC8 (SetPrompts' plain C++); Update sub_82185600 with "
              "ShowBusy sub_82181558, HideBusy sub_821815D8 and HideBusyNow sub_82181640 (the "
              "running transition through the guest context, the stand's calls through their "
              "overrides), Start sub_82184B80 (BuildLists' and FillStatus' plain C++) and "
              "OnDialogDone sub_82183160 (Buy's and SetPrompts' plain C++; ShowDriveDialog "
              "sub_82182990, never seen in play, the original); the pages Page0 sub_82180F90, "
              "Page1 sub_82183428 (ShowFirstCarDialog sub_82182A80, never seen in play, the "
              "original), Page2 sub_82183600 (SetUpSpecSheet sub_821810E0, never seen in play, "
              "the original; SetPrompts', CheckLimits', ReloadStandModel's and HelpLine::Set's "
              "plain C++; SetText's unset varargs 0) and Page3 sub_82181A10 (MenuList:: and "
              "MenuSound::Play through their overrides); the menus' openers OpenListMenu "
              "sub_82184338, OpenCarList sub_82184488 and OpenCar sub_82184650 (MenuList::Init, "
              "SetSelection, Fit, CarCards::FillAll and GarageStage::SetPlates through their "
              "overrides; SetKindBar's, SetScrollArrows', ShowCarInfo's and HelpLine::Set's plain "
              "C++); the transitions Transition0 sub_82184C70, Transition4 sub_82184DA0, "
              "Transition5 sub_82184E60, Transition8 sub_82184FB8, Transition9 sub_821850B8, "
              "Transition1 sub_82181C38, Transition6 sub_82181E10, Transition11 sub_82181ED0, "
              "Transition16 sub_821820C0, Transition19 sub_821841B0 and Transition7 sub_82183DE0 "
              "(CarCards::FillAll, CarCard::Fill and MenuList::GetPreviousSelection through "
              "their overrides; SetScrollArrows' and HelpLine::Set's plain C++) (the openers', SetHeader's and ReloadStandModel's plain C++; the named animations, "
              "UiMovie::SetNodeColor and UiMovie::Run through their overrides) "
              "(the same writes; checked by --run_harnesses=car_dealer)");

namespace hooks {

namespace {

namespace cd = car_dealer;

// The guest calls the plain C++ makes (car_dealer::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  cd::Calls calls() { return {this, &GuestCalls::Call}; }

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

void CarDealerInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = cd::InitStates(base, machine, ctx.r1.u32 - cd::kInitStatesFrame, &replaced);
  // StateMachine::Add's adapter's r3: a new transition's address from the
  // machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
void CarDealerCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerOpenMovie(PPCContext& ctx, uint8_t* base) {
  // A tail call: UiMovies::Open runs on the caller's frame with its lr.
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void CarDealerWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void CarDealerAfterDetail(PPCContext&, uint8_t* base) { cd::AfterDetail(base); }
void CarDealerPage4(PPCContext& ctx, uint8_t* base) { cd::Page4(base, ctx.r3.u32); }
void CarDealerPage0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Page0(base, ctx.r1.u32, guest.calls());
}
void CarDealerPage1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Page1(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerPage2(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Page2(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerPage3(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Page3(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Unload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void CarDealerDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Destroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerOnYes(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OnYes(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void CarDealerUpdatePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::UpdatePage(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void CarDealerLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Leave(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition3(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition3(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition0(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition4(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition5(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition5(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition8(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition8(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition9(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition9(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition1(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition6(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition6(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition11(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition11(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition16(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition16(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition19(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition19(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerTransition7(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Transition7(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerShowCarInfo(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::ShowCarInfo(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void CarDealerShowSwatches(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::ShowSwatches(base, ctx.r1.u32);
}
void CarDealerFillStatus(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::FillStatus(base, ctx.r1.u32, guest.calls());
}
void CarDealerSetNameTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::SetNameTexts(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void CarDealerSetPriceTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::SetPriceTexts(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void CarDealerSetSpecTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::SetSpecTexts(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void CarDealerSetSwatchColor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetSwatchColor(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void CarDealerUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Update(base, ctx.r1.u32, guest.calls());
}
void CarDealerStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Start(base, ctx.r1.u32, guest.calls());
}
void CarDealerOnDialogDone(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OnDialogDone(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void CarDealerShowBusy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::ShowBusy(base, ctx.r1.u32, guest.calls());
}
void CarDealerHideBusy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::HideBusy(base, ctx.r1.u32, guest.calls());
}
void CarDealerHideBusyNow(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::HideBusyNow(base, ctx.r1.u32, guest.calls());
}
void CarDealerStartFirstRace(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::StartFirstRace(base, ctx.r1.u32, guest.calls());
}
void CarDealerBuildLists(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::BuildLists(base, ctx.r1.u32, guest.calls());
}
void CarDealerBuy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::Buy(base, ctx.r1.u32, guest.calls());
}
void CarDealerCheckLimits(PPCContext&, uint8_t* base) { cd::CheckLimits(base); }
void CarDealerReloadStandModel(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::ReloadStandModel(base, ctx.r1.u32, guest.calls());
}
void CarDealerSetKindBar(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetKindBar(base, ctx.r3.u64, ctx.r1.u32);
}
void CarDealerSetColorBar(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetColorBar(base, ctx.r3.u64, ctx.r1.u32);
}
void CarDealerSetScrollArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetScrollArrows(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32);
}
void CarDealerSetHeader(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cd::SetHeader(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void CarDealerSetPrompts(PPCContext& ctx, uint8_t* base) {
  const uint32_t args[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                            ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = cd::SetPrompts(base, args, ctx.r1.u32);
}
void CarDealerOpenListMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OpenListMenu(base, ctx.r1.u32, guest.calls());
}
void CarDealerOpenCarList(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OpenCarList(base, ctx.r1.u32, guest.calls());
}
void CarDealerOpenCar(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::OpenCar(base, ctx.r1.u32, guest.calls());
}
void HelpLineSet(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = cd::HelpLineSet(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_CAR_DEALER(address, step)       \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_car_dealer)) {     \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The machine's set-up (from the type 2 mode's Init sub_82197278,
// recompiled).
ITC_CAR_DEALER(82185920, CarDealerInitStates)
// The handlers: member function functors the machine calls (no direct
// callers).
ITC_CAR_DEALER(82180D68, CarDealerCreate)
ITC_CAR_DEALER(82180C08, CarDealerOpenMovie)
ITC_CAR_DEALER(82180F20, CarDealerWaitForLoad)
ITC_CAR_DEALER(82180C48, CarDealerAfterDetail)
ITC_CAR_DEALER(821816B8, CarDealerUnload)
ITC_CAR_DEALER(82181838, CarDealerClose)
ITC_CAR_DEALER(82180E70, CarDealerDestroy)
ITC_CAR_DEALER(82181968, CarDealerOnYes)
ITC_CAR_DEALER(82184AF0, CarDealerUpdatePage)
ITC_CAR_DEALER(821817C0, CarDealerLeave)
ITC_CAR_DEALER(82185600, CarDealerUpdate)
ITC_CAR_DEALER(82184B80, CarDealerStart)
ITC_CAR_DEALER(82183160, CarDealerOnDialogDone)
// No adapters for ShowBusy sub_82181558, HideBusy sub_821815D8 and
// HideBusyNow sub_82181640: only Update (native) calls them, the flag
// switches it; their steps above are the harness's.
// The transitions (from CarDealer::Update's table on its stack).
ITC_CAR_DEALER(82181D68, CarDealerTransition3)
ITC_CAR_DEALER(82184C70, CarDealerTransition0)
ITC_CAR_DEALER(82184DA0, CarDealerTransition4)
ITC_CAR_DEALER(82184E60, CarDealerTransition5)
ITC_CAR_DEALER(82184FB8, CarDealerTransition8)
ITC_CAR_DEALER(821850B8, CarDealerTransition9)
ITC_CAR_DEALER(82181C38, CarDealerTransition1)
ITC_CAR_DEALER(82181E10, CarDealerTransition6)
ITC_CAR_DEALER(82181ED0, CarDealerTransition11)
ITC_CAR_DEALER(821820C0, CarDealerTransition16)
ITC_CAR_DEALER(821841B0, CarDealerTransition19)
ITC_CAR_DEALER(82183DE0, CarDealerTransition7)
// The page handlers (from UpdatePage's table on its stack).
ITC_CAR_DEALER(82180F90, CarDealerPage0)
ITC_CAR_DEALER(82183428, CarDealerPage1)
ITC_CAR_DEALER(82183600, CarDealerPage2)
ITC_CAR_DEALER(82181A10, CarDealerPage3)
ITC_CAR_DEALER(82181B98, CarDealerPage4)
// The helpers (from the recompiled transitions and set-ups). No adapter for
// ReloadStandModel sub_82181330 (Page2, Transition9; Transition10, never
// seen in play, reaches the original) (removed, B7 loop, 2026-11-08).
// StartFirstRace (from the title menu's sub_82221AC0, recompiled; Leave is
// native). No adapters for BuildLists sub_821813B0 (only Start calls it),
// Buy sub_82182E68 (only OnDialogDone), FillStatus sub_82183AC8 (only
// Start), CheckLimits sub_821812B8 (Page2, Page3) and SetPrompts
// sub_82182B30 (OnDialogDone, Page2, FillStatus; OnMessage26, never seen in
// play, reaches the original) (removed, B7 loop, 2026-11-08): their callers
// in play are native, the flag switches those; their steps above are the
// harness's.
ITC_CAR_DEALER(82180CE8, CarDealerStartFirstRace)
ITC_CAR_DEALER(82182770, CarDealerSetColorBar)
ITC_CAR_DEALER(82182C28, CarDealerSetHeader)
// No adapters for SetKindBar sub_82182158 (OpenListMenu, OpenCar),
// SetScrollArrows sub_82182DD0 (OpenCarList, Transition7) and ShowCarInfo
// sub_82182620 (OpenCar; Transition13, never seen in play, reaches the
// original) (removed, B7 loop, 2026-11-08): their callers in play are native,
// the flag switches those. ShowSwatches (from OpenColors and Transition17,
// recompiled). No adapters
// for SetNameTexts sub_82182388, SetPriceTexts sub_82182418, SetSpecTexts
// sub_821824A8 (only ShowCarInfo calls them) and SetSwatchColor
// sub_82181270 (only ShowSwatches): their callers are native, the flag
// switches those; their steps above are the harness's.
ITC_CAR_DEALER(821826C0, CarDealerShowSwatches)
ITC_CAR_DEALER(82199B38, HelpLineSet)
// No adapters for the menus' openers OpenListMenu sub_82184338
// (Transition0, Transition8), OpenCarList sub_82184488 (Transition4,
// Transition5; Transition14, never seen in play, reaches the original) and
// OpenCar sub_82184650 (Transition9; Transition10 and Transition18, never
// seen in play, reach the original) (removed, B7 loop, 2026-11-08): their
// callers in play are native, the flag switches those; their steps above are
// the harness's.
