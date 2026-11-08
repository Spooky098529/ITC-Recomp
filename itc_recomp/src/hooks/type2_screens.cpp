// The type 2 screens natively: adapters in front of src/type2_screens/
// (docs/functions/type2-screens.md), with the register steps the harness
// shares (hooks/type2_screens.h).

#include "hooks/type2_screens.h"

#include "base/flags.h"
#include "type2_screens/type2_screens.h"

ITC_FLAG_BOOL(native_type2_screens, true, "ITC",
              "Run the type 2 mode's screens (docs/functions/type2-screens.md: the hub, the "
              "spec sheet, the build screen's machine and the license plate editor) natively "
              "instead of through the translated code: Screen::LeaveToHub sub_821D6A20; the "
              "license plate editor's Create sub_82198210 (VirtualAlloc through its override), "
              "OpenMovie sub_82198180 (UiMovies::Open through its override), WaitForLoad "
              "sub_821982F0, Close sub_82198498 and Destroy sub_82198118 (UiMovie::FinishAll, "
              "UiMovies::Delete and VirtualFree through their overrides), OnMessage32 "
              "sub_82198190, LeavePage sub_82198988, Start sub_82199BB8 with LoadPlate "
              "sub_82198420, RandomPlate sub_82198360 (GetLocalTime through its override, the "
              "game's rand its plain C++) and ResetMovie sub_821989E8 (UiMovie::ResetPose "
              "through its override), CharLineKind sub_821981C8, ShowRegion sub_82199898, "
              "ShowDigits sub_821998F0, ShowChars sub_82199990, MarkField sub_82199AA8, "
              "FinishRegionAnim sub_821992D8 and FinishDigitAnim sub_82199410 "
              "(UiMovie::FinishNamed through its override); ShowCharLine sub_82199688 "
              "(sub_8211A630's plain C++), ShowPlate sub_82199A20, the menus' set-ups "
              "SetUpRegionMenu sub_82199C10, SetUpNumberMenu sub_82199CC8 and SetUpCharsMenu "
              "sub_82199D88 with ShowRegionCells sub_821990D0, ShowDigitCells sub_82199350 and "
              "ShowCharCells sub_82199488 (MenuList::Init, SetSelection, Fit, UiMovie::SetText, "
              "SetNodeText and ClearText through their overrides, their unset varargs 0; "
              "HelpLine::Set's plain C++); the transitions Transition0 sub_8219A3A8, Transition1 "
              "sub_82198A40, Transition4 sub_82198C20, Transition5 sub_8219A5E8, Transition6 "
              "sub_82198D00, Transition8 sub_8219A820, Transition10 sub_8219AA00, Transition11 "
              "sub_82198DC8, Transition13 sub_8219AC88 and Transition19 sub_82198FB8 (the named "
              "animations, UiMovie::FinishAll and UiMovie::Run through their overrides; the "
              "plate editor's helpers and HelpLine::Set their plain C++); the pages RegionPage "
              "sub_82199ED0, NumberPage sub_8219A138 and CharsPage sub_82198540 (MenuList:: and "
              "MenuSound::Play through their overrides), UpdatePage sub_8219B050 and Update "
              "sub_8219B108 (the page's handler and the running transition through the guest "
              "context, UiMovie::Run through its override); the four machines' set-ups "
              "Type2Hub::InitStates sub_82190B18, PowerGraphScreen2::InitStates sub_821B90D8, "
              "BuildScreen::InitStates sub_821A16C0 and PlateEditor::InitStates sub_8219B2E8 over "
              "StateMachine::Add's plain C++; the spec sheet's PowerGraphScreen::Create "
              "sub_821B8120, OpenMovie sub_82250328, WaitForLoad sub_821B8900 (memset through its "
              "override), Close sub_821B8320 and Destroy sub_821B80B8; the build screen's "
              "machine's Create sub_8219BD80, OpenMovie sub_8219B950, WaitForLoad sub_8219BE98, "
              "Unload sub_8219C298, Close sub_8219C300, Destroy sub_8219B8C8 (MenuList::Release "
              "through its override), UpdatePage sub_8219FA80 (the page's handler through the "
              "guest context), DetailsPage sub_8219B960 and LeavePage sub_8219C3A8 with "
              "SaveBuildStates sub_8219C100 (GarageStage::SetDark's plain C++); the hub's "
              "OnYes sub_8218D6A8, OnNo sub_8218D6C0, WaitForLoad sub_8218E420 (the patched "
              "count of 1) with ImagesLoadedA sub_8218E2D8 and ImagesLoadedB sub_8218E320, "
              "FreeImage sub_8218D650, OpenSelected sub_8218E5B0, Reenter sub_8218D9B8, Choose "
              "sub_8218E768 with PostMessage5 sub_8218E7A4, Unload sub_8218E530, Close "
              "sub_8218E688, SetPrompts sub_8218F530, Back sub_8218F698, Start sub_8218F6E0 with "
              "SetUpStatus sub_8218E810 (the text setters through their overrides), NextNotice "
              "sub_8218DBD0 and NextNotice2 sub_8218DD78; Create sub_8218E078 with "
              "MarkNoticeImages sub_8218DCC0, Destroy sub_8218E1A8 with FreeImages sub_8218DE88, "
              "OpenMovie sub_8218E368 with LoadNoticeImages sub_8218E240 and LoadNoticeImage "
              "sub_8218DF38, and the archive's callback ImageLoaded sub_8218D8E8 (VirtualAlloc, "
              "VirtualFree, XMemSet, memcpy, Archive::Load, GarageStage::MakeModel, "
              "UiMovies::Open and the XPR's calls through their overrides) "
              "(the same writes; checked by --run_harnesses=type2_screens)");

namespace hooks {

namespace {

namespace t2 = type2_screens;

// The guest calls the plain C++ makes (type2_screens::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  t2::Calls calls() { return {this, &GuestCalls::Call}; }

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

void ScreenLeaveToHub(PPCContext& ctx, uint8_t* base) {
  t2::LeaveToHub(base, ctx.r3.u32, ctx.r4.u32);
}
void PlateCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateOpenMovie(PPCContext& ctx, uint8_t* base) {
  // A tail call: UiMovies::Open runs on the caller's frame with its lr.
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateOpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void PlateWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateWaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void PlateClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateClose(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void PlateDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateDestroy(base, ctx.r1.u32, guest.calls());
}
void PlateOnMessage32(PPCContext&, uint8_t* base) { t2::PlateOnMessage32(base); }
void PlateStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateStart(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void PlateLeavePage(PPCContext& ctx, uint8_t* base) { t2::PlateLeavePage(base, ctx.r3.u32); }
void PlateCharLineKind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateCharLineKind(base, ctx.r3.u32);
}
void PlateRandom(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateRandom(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void PlateLoad(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateLoad(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void PlateResetMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateResetMovie(base, ctx.r1.u32, guest.calls());
}
void PlateShowRegion(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateShowRegion(base, ctx.r3.u32, ctx.r1.u32);
}
void PlateShowDigits(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateShowDigits(base, ctx.r3.u32, ctx.r1.u32);
}
void PlateShowChars(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateShowChars(base, ctx.r3.u32, ctx.r1.u32);
}
void PlateMarkField(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateMarkField(base, ctx.r3.u32, ctx.r1.u32);
}
void PlateFinishRegionAnim(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateFinishRegionAnim(base, ctx.r1.u32, guest.calls());
}
void PlateFinishDigitAnim(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateFinishDigitAnim(base, ctx.r1.u32, guest.calls());
}

void PlateShowRegionCells(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateShowRegionCells(base, ctx.r1.u32, guest.calls());
}
void PlateShowDigitCells(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateShowDigitCells(base, ctx.r1.u32, guest.calls());
}
void PlateShowCharCells(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateShowCharCells(base, ctx.r1.u32, guest.calls());
}
void PlateShowCharLine(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateShowCharLine(base, ctx.r3.u32, ctx.r1.u32);
}
void PlateShowPlate(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::PlateShowPlate(base, ctx.r3.u32, ctx.r1.u32);
}
void PlateSetUpRegionMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateSetUpRegionMenu(base, ctx.r1.u32, guest.calls());
}
void PlateSetUpNumberMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateSetUpNumberMenu(base, ctx.r1.u32, guest.calls());
}
void PlateSetUpCharsMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateSetUpCharsMenu(base, ctx.r1.u32, guest.calls());
}
void PlateTransition0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition0(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition1(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition4(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition5(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition5(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition6(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition6(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition8(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition8(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition10(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition10(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition11(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition11(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition13(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition13(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateTransition19(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateTransition19(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateRegionPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateRegionPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateNumberPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateNumberPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateCharsPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateCharsPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PlateUpdatePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateUpdatePage(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void PlateUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::PlateUpdate(base, ctx.r1.u32, guest.calls());
}

void SheetCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::SheetCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void SheetOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::SheetOpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void SheetWaitForLoad(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::SheetWaitForLoad(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void SheetClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::SheetClose(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void SheetDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::SheetDestroy(base, ctx.r1.u32, guest.calls());
}
void BuildCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void BuildOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildOpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void BuildWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::BuildWaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void BuildUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildUnload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void BuildClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildClose(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void BuildDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildDestroy(base, ctx.r1.u32, guest.calls());
}
void BuildSaveBuildStates(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::BuildSaveBuildStates(base, ctx.r1.u32);
}
void BuildLeavePage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::BuildLeavePage(base, ctx.r3.u32, ctx.r1.u32);
}
void BuildDetailsPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildDetailsPage(base, ctx.r1.u32, guest.calls());
}
void BuildUpdatePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::BuildUpdatePage(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void HubOnYes(PPCContext&, uint8_t* base) { t2::HubOnYes(base); }
void HubOnNo(PPCContext&, uint8_t* base) { t2::HubOnNo(base); }
void HubImagesLoadedA(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::HubImagesLoadedA(base, ctx.r3.u32);
}
void HubImagesLoadedB(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::HubImagesLoadedB(base, ctx.r3.u32);
}
void HubWaitForLoad(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubWaitForLoad(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void HubFreeImage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubFreeImage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void HubOpenSelected(PPCContext& ctx, uint8_t* base) {
  t2::HubOpenSelected(base, ctx.r3.u32, ctx.r4.u32);
}
void HubReenter(PPCContext& ctx, uint8_t* base) { t2::HubReenter(base, ctx.r3.u32); }
void HubPostMessage5(PPCContext& ctx, uint8_t* base) { t2::HubPostMessage5(base, ctx.r3.u32); }
void HubChoose(PPCContext& ctx, uint8_t* base) { t2::HubChoose(base, ctx.r3.u32, ctx.r4.u32); }
void HubUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubUnload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void HubClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubClose(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void HubSetPrompts(PPCContext& ctx, uint8_t* base) {
  const uint32_t args[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                            ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = t2::HubSetPrompts(base, args, ctx.r1.u32);
}
void HubBack(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = t2::HubBack(base, ctx.r1.u32); }
void HubSetUpStatus(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubSetUpStatus(base, ctx.r1.u32, guest.calls());
}
void HubNextNotice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::HubNextNotice(base, ctx.r1.u32);
}
void HubNextNotice2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::HubNextNotice2(base, ctx.r1.u32);
}
void HubStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubStart(base, ctx.r1.u32, guest.calls());
}

void HubCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void HubDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubDestroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void HubOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubOpenMovie(base, ctx.r1.u32, guest.calls());
}
void HubImageLoaded(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubImageLoaded(base, ctx.r4.u32, ctx.r6.u32, ctx.r1.u32, guest.calls());
}
void HubMarkNoticeImages(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2::HubMarkNoticeImages(base, ctx.r1.u32);
}
void HubFreeImages(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubFreeImages(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void HubLoadNoticeImage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubLoadNoticeImage(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void HubLoadNoticeImages(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2::HubLoadNoticeImages(base, ctx.r1.u32, guest.calls());
}

namespace {
// An Init's register step: StateMachine::Add's adapter's r3, a new
// transition's address from the machine's 64-bit r3.
template <uint32_t (*Init)(uint8_t*, uint32_t, uint32_t, bool*)>
void InitStep(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = Init(base, machine, ctx.r1.u32 - frame_bytes, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
}  // namespace

void HubInitStates(PPCContext& ctx, uint8_t* base) {
  InitStep<t2::HubInitStates>(ctx, base, t2::kHubInitFrame);
}
void SheetInitStates(PPCContext& ctx, uint8_t* base) {
  InitStep<t2::SheetInitStates>(ctx, base, t2::kSheetInitFrame);
}
void BuildInitStates(PPCContext& ctx, uint8_t* base) {
  InitStep<t2::BuildInitStates>(ctx, base, t2::kBuildInitFrame);
}
void PlateInitStates(PPCContext& ctx, uint8_t* base) {
  InitStep<t2::PlateInitStates>(ctx, base, t2::kPlateInitFrame);
}

}  // namespace hooks

#define ITC_TYPE2_SCREENS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_type2_screens)) {   \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    hooks::step(ctx, base);                  \
  }

// Screen::LeaveToHub: the (8, 3) handler of four type 2 machines (no direct
// callers).
ITC_TYPE2_SCREENS(821D6A20, ScreenLeaveToHub)
// The four machines' set-ups (from the type 2 mode's Init sub_82197278,
// recompiled).
ITC_TYPE2_SCREENS(82190B18, HubInitStates)
ITC_TYPE2_SCREENS(821B90D8, SheetInitStates)
ITC_TYPE2_SCREENS(821A16C0, BuildInitStates)
ITC_TYPE2_SCREENS(8219B2E8, PlateInitStates)
// The spec sheet's handlers (functors the machine calls; OpenMovie also
// registered by three other modes' Inits).
ITC_TYPE2_SCREENS(821B8120, SheetCreate)
ITC_TYPE2_SCREENS(82250328, SheetOpenMovie)
ITC_TYPE2_SCREENS(821B8900, SheetWaitForLoad)
ITC_TYPE2_SCREENS(821B8320, SheetClose)
ITC_TYPE2_SCREENS(821B80B8, SheetDestroy)
// The build screen's machine's handlers (functors) and pages (from its
// UpdatePage's table); no adapter for SaveBuildStates sub_8219C100 (only
// LeavePage, native, calls it; its step above is the harness's).
ITC_TYPE2_SCREENS(8219BD80, BuildCreate)
ITC_TYPE2_SCREENS(8219B950, BuildOpenMovie)
ITC_TYPE2_SCREENS(8219BE98, BuildWaitForLoad)
ITC_TYPE2_SCREENS(8219C298, BuildUnload)
ITC_TYPE2_SCREENS(8219C300, BuildClose)
ITC_TYPE2_SCREENS(8219B8C8, BuildDestroy)
ITC_TYPE2_SCREENS(8219FA80, BuildUpdatePage)
ITC_TYPE2_SCREENS(8219B960, BuildDetailsPage)
ITC_TYPE2_SCREENS(8219C3A8, BuildLeavePage)
// The hub's handlers (functors the machine calls: WaitForLoad, Start,
// Unload, OpenSelected, Reenter, Close, OnYes, OnNo, Back; Choose from
// UpdatePage's table on its stack, recompiled) and helpers (from the
// recompiled hub code). No adapters for ImagesLoadedA sub_8218E2D8 and
// ImagesLoadedB sub_8218E320 (only WaitForLoad), PostMessage5 sub_8218E7A4
// (only Choose) and SetUpStatus sub_8218E810 (only Start): their steps
// above are the harness's.
ITC_TYPE2_SCREENS(8218D6A8, HubOnYes)
ITC_TYPE2_SCREENS(8218D6C0, HubOnNo)
ITC_TYPE2_SCREENS(8218E420, HubWaitForLoad)
ITC_TYPE2_SCREENS(8218D650, HubFreeImage)
ITC_TYPE2_SCREENS(8218E5B0, HubOpenSelected)
ITC_TYPE2_SCREENS(8218D9B8, HubReenter)
ITC_TYPE2_SCREENS(8218E768, HubChoose)
ITC_TYPE2_SCREENS(8218E530, HubUnload)
ITC_TYPE2_SCREENS(8218E688, HubClose)
ITC_TYPE2_SCREENS(8218F530, HubSetPrompts)
ITC_TYPE2_SCREENS(8218F698, HubBack)
ITC_TYPE2_SCREENS(8218F6E0, HubStart)
ITC_TYPE2_SCREENS(8218DBD0, HubNextNotice)
ITC_TYPE2_SCREENS(8218DD78, HubNextNotice2)
// The hub's Create, OpenMovie and Destroy (functors) and the archive's
// callback ImageLoaded (its address handed to Archive::Load; also by
// sub_8225BD10, not read). No adapters for MarkNoticeImages sub_8218DCC0
// (only Create), FreeImages sub_8218DE88 (only Destroy), LoadNoticeImages
// sub_8218E240 (only OpenMovie) and LoadNoticeImage sub_8218DF38 (only
// LoadNoticeImages): their steps above are the harness's.
ITC_TYPE2_SCREENS(8218E078, HubCreate)
ITC_TYPE2_SCREENS(8218E368, HubOpenMovie)
ITC_TYPE2_SCREENS(8218E1A8, HubDestroy)
ITC_TYPE2_SCREENS(8218D8E8, HubImageLoaded)
// The license plate editor's handlers: member function functors the machine
// calls (no direct callers).
ITC_TYPE2_SCREENS(82198210, PlateCreate)
ITC_TYPE2_SCREENS(82198180, PlateOpenMovie)
ITC_TYPE2_SCREENS(821982F0, PlateWaitForLoad)
ITC_TYPE2_SCREENS(82198498, PlateClose)
ITC_TYPE2_SCREENS(82198118, PlateDestroy)
ITC_TYPE2_SCREENS(82198190, PlateOnMessage32)
ITC_TYPE2_SCREENS(82199BB8, PlateStart)
ITC_TYPE2_SCREENS(8219B050, PlateUpdatePage)
ITC_TYPE2_SCREENS(8219B108, PlateUpdate)
// The pages (from UpdatePage's table on its stack, recompiled).
ITC_TYPE2_SCREENS(82198988, PlateLeavePage)
ITC_TYPE2_SCREENS(82199ED0, PlateRegionPage)
ITC_TYPE2_SCREENS(8219A138, PlateNumberPage)
ITC_TYPE2_SCREENS(82198540, PlateCharsPage)
// The helpers (from the recompiled transitions, pages and set-ups). No
// adapters for LoadPlate sub_82198420, RandomPlate sub_82198360 and
// ResetMovie sub_821989E8: only Start (native) reaches them, the flag
// switches it; their steps above are the harness's.
// The transitions (from PlateEditor::Update's table on its stack,
// recompiled).
ITC_TYPE2_SCREENS(8219A3A8, PlateTransition0)
ITC_TYPE2_SCREENS(82198A40, PlateTransition1)
ITC_TYPE2_SCREENS(82198C20, PlateTransition4)
ITC_TYPE2_SCREENS(8219A5E8, PlateTransition5)
ITC_TYPE2_SCREENS(82198D00, PlateTransition6)
ITC_TYPE2_SCREENS(8219A820, PlateTransition8)
ITC_TYPE2_SCREENS(8219AA00, PlateTransition10)
ITC_TYPE2_SCREENS(82198DC8, PlateTransition11)
ITC_TYPE2_SCREENS(8219AC88, PlateTransition13)
ITC_TYPE2_SCREENS(82198FB8, PlateTransition19)
// No adapters for ShowRegionCells sub_821990D0 (SetUpRegionMenu;
// Transition3, never seen in play, reaches the original), ShowDigitCells
// sub_82199350 (only SetUpNumberMenu) and ShowCharCells sub_82199488
// (SetUpCharsMenu; Transition12, never seen in play, reaches the original),
// nor (removed, B7 loop, 2026-11-08) for ShowRegion sub_82199898 (ShowPlate;
// Transition3, never seen in play, reaches the original) and MarkField
// sub_82199AA8 (the three menus' set-ups; SetUpConfirmMenu, never seen in
// play, reaches the original): their callers in play are native, the flag
// switches those; their steps above are the harness's. No adapters either
// (removed, B7 loop, 2026-11-08) for CharLineKind sub_821981C8 (Transition13),
// ShowDigits sub_821998F0 (ShowPlate, Transition5, Transition8), ShowChars
// sub_82199990 (ShowPlate, Transition13), ShowCharLine sub_82199688
// (Transition10, Transition13), ShowPlate sub_82199A20 (Transition0) and the
// menus' set-ups SetUpRegionMenu sub_82199C10 (Transition0), SetUpNumberMenu
// sub_82199CC8 (Transition5, Transition8) and SetUpCharsMenu sub_82199D88
// (Transition10, Transition13), FinishRegionAnim sub_821992D8 (RegionPage,
// Transition5) and FinishDigitAnim sub_82199410 (NumberPage, Transition10):
// their callers in play are native (the transitions never seen in play
// reach the originals), the flag switches those; their steps above are the
// harness's.
