// The garage screen natively: adapters in front of src/garage_screen/
// (docs/functions/garage-screen.md), with the register steps the harness
// shares (hooks/garage_screen.h).

#include "hooks/garage_screen.h"

#include "base/flags.h"
#include "garage_screen/garage_screen.h"

ITC_FLAG_BOOL(native_garage_screen, true, "ITC",
              "Run the type 2 mode's garage screen (docs/functions/garage-screen.md: the "
              "player's garage cars over the stand, UI movie 5) natively instead of through the "
              "translated code: the helpers SetCarRow sub_82193180, FillRows sub_82194CD8, "
              "SetPrompts sub_821939A0, SetHelpLine sub_82193A90 and SetUpLists sub_821956C0 "
              "(the UI movie and menu list calls through their overrides, SetScrollArrows the "
              "original); the transitions Transition0 sub_82195BA0, Transition1 sub_82192D60 "
              "and Transition3 sub_82192E20; the pages CarListPage sub_82194078 (ReloadStandCar "
              "the original) and LeavePage sub_82192C18; UpdatePage sub_82194EA8 and Update "
              "sub_82196230 (the pages and transitions through the guest from their tables, the "
              "stand's calls through their overrides, PutCarOnStand and the busy marker's three "
              "the originals); the machine's handlers Create sub_82191C58 and Destroy "
              "sub_821915B0 (VirtualAlloc and VirtualFree through their overrides), OpenMovie "
              "sub_82191618, WaitForLoad sub_82191D70, Start sub_82193DC0 with ReadSave "
              "sub_82191BB0 and SetHeader sub_82192C80, Unload sub_821927E8 and Close "
              "sub_82192850; InitStates sub_82196520 (its step table, StateMachine::Add's plain C++); "
              "the paths never seen in play: SetScrollArrows sub_82193288, SetActionMarker "
              "sub_821936B0, SetOptionMarker sub_82193720, ShowSpecTexts sub_82193798, "
              "ShowCarNames sub_82193910, SetPlateQuestion sub_82193320, CopyGarageCar "
              "sub_82192770, ShowBusy sub_82193C58, HideBusy sub_82193CE0, HideBusyNow "
              "sub_82193D48, ResetStand sub_82191B20, SetUpView sub_82194AC8, ToggleTurn "
              "sub_821918D0, ToggleDoors sub_82191920, ToggleDoors2 sub_82191970, TurnLeft "
              "sub_821919C0, TurnRight sub_82191A18, CarValue sub_82192378 and SalePrice "
              "sub_82192690; ReloadStandCar sub_82193B10, PutCarOnStand sub_82193BA8, ShowCarSpecs "
              "sub_82194D90, SetUseQuestion sub_821933D0, SetSaleQuestion sub_821934D0, "
              "SetUpPlateEditor sub_82191810, SetUpSpecSheet sub_82191628 (its piece "
              "sub_82191768 merged), CameraA sub_82191A70 and CameraB sub_82191AC8 (the car "
              "objects and the stand's calls through their overrides); Transition2 sub_82194F30, "
              "Transition4 sub_82195D10 to Transition10 sub_82195530, Transition12 sub_821955E0 to "
              "Transition14 sub_821948B0, Transition16 sub_821930D8, Transition17 sub_82196170, "
              "SetUpActions sub_821958D0 and SetUpOptions sub_82194B20 (Transition11 and "
              "Transition15 the originals: Run's r8 leftover stored into live state) "
              "(the same writes; checked by "
              "--run_harnesses=garage_screen)");

namespace hooks {

namespace {

namespace gs = garage_screen;

// The guest calls the plain C++ makes (garage_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  gs::Calls calls() { return {this, &GuestCalls::Call}; }

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

void GarageScreenSetCarRow(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetCarRow(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenFillRows(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::FillRows(base, ctx.r1.u32, guest.calls());
}
void GarageScreenSetPrompts(PPCContext& ctx, uint8_t* base) {
  const uint32_t args[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                            ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = gs::SetPrompts(base, args, ctx.r1.u32);
}
void GarageScreenSetHelpLine(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetHelpLine(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void GarageScreenSetUpLists(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetUpLists(base, ctx.r1.u32, guest.calls());
}
void GarageScreenTransition0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Transition0(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenTransition1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Transition1(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenTransition3(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Transition3(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void GarageScreenCarListPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::CarListPage(base, ctx.r1.u32, guest.calls());
}
void GarageScreenLeavePage(PPCContext& ctx, uint8_t* base) {
  gs::LeavePage(base, ctx.r3.u32);
}
void GarageScreenUpdatePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      gs::UpdatePage(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32, guest.calls());
}
void GarageScreenUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Update(base, ctx.r1.u32, guest.calls());
}
void GarageScreenCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// A tail call: Open runs from the adapter's frame with its lr.
void GarageScreenOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::OpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void GarageScreenWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gs::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void GarageScreenReadSave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gs::ReadSave(base, ctx.r1.u32);
}
void GarageScreenSetHeader(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetHeader(base, ctx.r1.u32, guest.calls());
}
void GarageScreenStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Start(base, ctx.r1.u32, guest.calls());
}
void GarageScreenUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Unload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Destroy(base, ctx.r1.u32, guest.calls());
}
void GarageScreenSetScrollArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gs::SetScrollArrows(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void GarageScreenSetActionMarker(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = gs::SetActionMarker(base, ctx.r3.u32, ctx.r1.u32);
}
void GarageScreenSetOptionMarker(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = gs::SetOptionMarker(base, ctx.r3.u32, ctx.r1.u32);
}
void GarageScreenShowSpecTexts(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ShowSpecTexts(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenShowCarNames(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ShowCarNames(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void GarageScreenSetPlateQuestion(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetPlateQuestion(base, ctx.r1.u32, guest.calls());
}
void GarageScreenCopyGarageCar(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::CopyGarageCar(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32,
                                 guest.calls());
}
void GarageScreenShowBusy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ShowBusy(base, ctx.r1.u32, guest.calls());
}
void GarageScreenHideBusy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::HideBusy(base, ctx.r1.u32, guest.calls());
}
void GarageScreenHideBusyNow(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::HideBusyNow(base, ctx.r1.u32, guest.calls());
}
void GarageScreenResetStand(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ResetStand(base, ctx.r1.u32, guest.calls());
}
void GarageScreenSetUpView(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetUpView(base, ctx.r1.u32, guest.calls());
}
// The toggles' commands are tail calls: from the adapter's frame and lr.
void GarageScreenToggleTurn(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ToggleTurn(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void GarageScreenToggleDoors(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ToggleDoors(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void GarageScreenToggleDoors2(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::ToggleDoors2(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void GarageScreenTurnLeft(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::TurnLeft(base, ctx.r1.u32, guest.calls());
}
void GarageScreenTurnRight(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::TurnRight(base, ctx.r1.u32, guest.calls());
}
// CarValue: f1 (r3 left the car).
void GarageScreenCarValue(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = gs::CarValue(base, ctx.r3.u32, ctx.r1.u32);
}
void GarageScreenSalePrice(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = gs::SalePrice(base, ctx.r3.u32, ctx.r1.u32);
}
#define ITC_GARAGE_STEP(name, call)                    \
  void GarageScreen##name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();                     \
    GuestCalls guest(ctx, base);                      \
    ctx.r3.u64 = gs::call;                            \
    ctx.fpscr.disableFlushMode();                     \
  }
ITC_GARAGE_STEP(ReloadStandCar, ReloadStandCar(base, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(PutCarOnStand, PutCarOnStand(base, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(ShowCarSpecs, ShowCarSpecs(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(SetUseQuestion, SetUseQuestion(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(SetSaleQuestion, SetSaleQuestion(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(SetUpPlateEditor, SetUpPlateEditor(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(SetUpSpecSheet, SetUpSpecSheet(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(CameraA, CameraA(base, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(CameraB, CameraB(base, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition2, Transition2(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition4, Transition4(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition5, Transition5(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition6, Transition6(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition7, Transition7(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition8, Transition8(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition9, Transition9(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition10, Transition10(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition12, Transition12(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition13, Transition13(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition14, Transition14(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition16, Transition16(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(Transition17, Transition17(base, ctx.r3.u32, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(SetUpActions, SetUpActions(base, ctx.r1.u32, guest.calls()))
ITC_GARAGE_STEP(SetUpOptions, SetUpOptions(base, ctx.r1.u32, guest.calls()))
#undef ITC_GARAGE_STEP
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 dialog's Init.
void GarageScreenInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = gs::InitStates(base, machine, ctx.r1.u32 - gs::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define ITC_GARAGE_SCREEN(address, step)    \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_garage_screen)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The movie 5 helpers' adapters removed (B7 loop, 2026-11-09): their callers
// in play are native and call their plain C++; the recompiled callers never
// seen in play run the originals. Their register steps stay for the harness.

// The transitions seen in play (from Update's table on its stack).
ITC_GARAGE_SCREEN(82195BA0, GarageScreenTransition0)
ITC_GARAGE_SCREEN(82192D60, GarageScreenTransition1)
ITC_GARAGE_SCREEN(82192E20, GarageScreenTransition3)
// The pages seen in play (from UpdatePage's table on its stack).
ITC_GARAGE_SCREEN(82194078, GarageScreenCarListPage)
ITC_GARAGE_SCREEN(82192C18, GarageScreenLeavePage)
// UpdatePage and Update (functors the machine calls).
ITC_GARAGE_SCREEN(82194EA8, GarageScreenUpdatePage)
ITC_GARAGE_SCREEN(82196230, GarageScreenUpdate)
// The machine's handlers (functors); Start's two helpers' adapters removed
// (Start's plain C++ calls theirs).
ITC_GARAGE_SCREEN(82191C58, GarageScreenCreate)
ITC_GARAGE_SCREEN(82191618, GarageScreenOpenMovie)
ITC_GARAGE_SCREEN(82191D70, GarageScreenWaitForLoad)
ITC_GARAGE_SCREEN(82193DC0, GarageScreenStart)
ITC_GARAGE_SCREEN(821927E8, GarageScreenUnload)
ITC_GARAGE_SCREEN(82192850, GarageScreenClose)
ITC_GARAGE_SCREEN(821915B0, GarageScreenDestroy)
// The texts, markers and stand helpers never seen in play (their callers,
// never seen in play, still recompiled).
ITC_GARAGE_SCREEN(82193288, GarageScreenSetScrollArrows)
ITC_GARAGE_SCREEN(821936B0, GarageScreenSetActionMarker)
ITC_GARAGE_SCREEN(82193720, GarageScreenSetOptionMarker)
ITC_GARAGE_SCREEN(82193798, GarageScreenShowSpecTexts)
ITC_GARAGE_SCREEN(82193910, GarageScreenShowCarNames)
ITC_GARAGE_SCREEN(82193320, GarageScreenSetPlateQuestion)
ITC_GARAGE_SCREEN(82192770, GarageScreenCopyGarageCar)
ITC_GARAGE_SCREEN(82193C58, GarageScreenShowBusy)
ITC_GARAGE_SCREEN(82193CE0, GarageScreenHideBusy)
ITC_GARAGE_SCREEN(82193D48, GarageScreenHideBusyNow)
ITC_GARAGE_SCREEN(82191B20, GarageScreenResetStand)
ITC_GARAGE_SCREEN(82194AC8, GarageScreenSetUpView)
ITC_GARAGE_SCREEN(821918D0, GarageScreenToggleTurn)
ITC_GARAGE_SCREEN(82191920, GarageScreenToggleDoors)
ITC_GARAGE_SCREEN(82191970, GarageScreenToggleDoors2)
ITC_GARAGE_SCREEN(821919C0, GarageScreenTurnLeft)
ITC_GARAGE_SCREEN(82191A18, GarageScreenTurnRight)
ITC_GARAGE_SCREEN(82192378, GarageScreenCarValue)
ITC_GARAGE_SCREEN(82192690, GarageScreenSalePrice)
// The car objects never seen in play (their callers still recompiled).
ITC_GARAGE_SCREEN(82193B10, GarageScreenReloadStandCar)
ITC_GARAGE_SCREEN(82193BA8, GarageScreenPutCarOnStand)
ITC_GARAGE_SCREEN(82194D90, GarageScreenShowCarSpecs)
ITC_GARAGE_SCREEN(821933D0, GarageScreenSetUseQuestion)
ITC_GARAGE_SCREEN(821934D0, GarageScreenSetSaleQuestion)
ITC_GARAGE_SCREEN(82191810, GarageScreenSetUpPlateEditor)
ITC_GARAGE_SCREEN(82191628, GarageScreenSetUpSpecSheet)
ITC_GARAGE_SCREEN(82191A70, GarageScreenCameraA)
ITC_GARAGE_SCREEN(82191AC8, GarageScreenCameraB)
// The transitions and set-ups never seen in play (Update calls the
// transitions' plain C++; the recompiled Transition11 and Transition15 and
// the pages still reach the set-ups).
ITC_GARAGE_SCREEN(82194F30, GarageScreenTransition2)
ITC_GARAGE_SCREEN(82195D10, GarageScreenTransition4)
ITC_GARAGE_SCREEN(82192EF8, GarageScreenTransition5)
ITC_GARAGE_SCREEN(82194680, GarageScreenTransition6)
ITC_GARAGE_SCREEN(821951A8, GarageScreenTransition7)
ITC_GARAGE_SCREEN(82195E30, GarageScreenTransition8)
ITC_GARAGE_SCREEN(82195410, GarageScreenTransition9)
ITC_GARAGE_SCREEN(82195530, GarageScreenTransition10)
ITC_GARAGE_SCREEN(821955E0, GarageScreenTransition12)
ITC_GARAGE_SCREEN(82192FD0, GarageScreenTransition13)
ITC_GARAGE_SCREEN(821948B0, GarageScreenTransition14)
ITC_GARAGE_SCREEN(821930D8, GarageScreenTransition16)
ITC_GARAGE_SCREEN(82196170, GarageScreenTransition17)
ITC_GARAGE_SCREEN(821958D0, GarageScreenSetUpActions)
ITC_GARAGE_SCREEN(82194B20, GarageScreenSetUpOptions)
// InitStates (from the type 2 mode's Init sub_82197278, recompiled).
ITC_GARAGE_SCREEN(82196520, GarageScreenInitStates)
