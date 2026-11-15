// The build screen natively: adapters in front of src/build_screen/
// (docs/functions/build-screen.md), with the register steps the harness
// shares (hooks/build_screen.h).

#include "hooks/build_screen.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "build_screen/build_screen.h"
#include "car_makers/car_makers.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "hooks/front_end_car_draw.h"
#include "lifted/sdk.h"

ITC_FLAG_DECLARE(bool, native_car_makers);
ITC_FLAG_DECLARE(bool, native_scene_models);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);

ITC_FLAG_BOOL(native_build_screen, true, "ITC",
              "Run the build screen's functions (docs/functions/build-screen.md: the menu "
              "lists' BuildScreen::KeepGroupList sub_8219BA50, KeepBuildList sub_8219BB20, "
              "KeepStandList sub_8219BBF0, RestoreGroupList sub_8219BAB0, RestoreBuildList "
              "sub_8219BB80, RestoreStandList sub_8219BC50, InitGroupLists sub_8219BF08, "
              "InitStandList sub_8219C078 (MenuList::Init through its override), ValueText "
              "sub_8219BCC0; the build lists' BuildList::Insert sub_8219C230 and InsertNode "
              "sub_8219C1C8, MarkGroups sub_8219EC50, FilterGroups sub_8219CC70; the garage "
              "stage's GarageStage::SetDark sub_8217D0C8 with GarageLights::SetMode sub_8217B7C8; "
              "the rows' and nodes' GroupArrows sub_8219D1B0, BuildArrows sub_8219D4B8, "
              "StandArrows sub_8219D7B0, ShowBuildMark sub_8219D440, SetPrompts sub_8219DA58, "
              "SetHelp sub_8219DB50 (the text setters through their overrides), ShowNumber "
              "sub_8219D090 with PowI sub_8219BD30, DrawGroupRows sub_8219FA28 with DrawGroupRow "
              "sub_8219CE08 (SetNodeText through its override); the transitions HighlightGroups "
              "sub_8219C528, HighlightGroups2 sub_8219C6D0, FadeOut sub_8219C850 "
              "(ScreenFades::FadeOut's plain C++), HighlightBuilds sub_8219C8F8, HighlightStand "
              "sub_8219CA08, HighlightStand2 sub_8219CB18, HighlightDetails sub_8219CBD8 (the UI "
              "movie animation calls through their overrides); the builds level's InitBuildList "
              "sub_8219BFE8, CopyBuilds sub_821A0128, ListBuilds sub_821A0D38, DrawBuildRow "
              "sub_8219D248, DrawBuildRows sub_8219D3E8, DrawStandRow sub_8219D550, ShowGroup "
              "sub_8219CF08 and SetUpBuilds sub_821A1008 (SetText and SetNodeText through their "
              "overrides); the groups level's SetUpGroups sub_821A0060 and the transitions Open "
              "sub_821A08D8, BackToGroups sub_821A0A38 and ChangeCategory sub_8219FC60; the "
              "transition OpenBuilds sub_821A1138; the stand's RestoreStand sub_8219F868 and the "
              "transition BackToStand sub_8219FF78; the details level's transition OpenDetails "
              "sub_8219FEC8 with SetUpDetails sub_8219F8E0 and ShowDetails sub_8219D820; the "
              "transition OpenStand sub_8219FE10 with SetUpStand sub_8219F780 and PutOnStand "
              "sub_8219D6F0 (GarageStage::ReloadModel's plain C++ with --native_car_makers, "
              "--native_vehicle_accessors and --native_scene_models, else its adapter)) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=build_screen). RestoreGroupList, InsertNode, SetMode, PowI, DrawGroupRow, InitBuildList, "
              "CopyBuilds, ListBuilds, DrawBuildRow, DrawBuildRows, SetUpGroups, FilterGroups, "
              "MarkGroups, InitGroupLists, DrawGroupRows, ShowGroup, ShowNumber, GroupArrows, "
              "SetUpDetails, ShowDetails, KeepGroupList, KeepStandList, RestoreStandList, ValueText, "
              "BuildArrows, ShowBuildMark, RestoreStand, KeepBuildList, RestoreBuildList, "
              "InitStandList, DrawStandRow, StandArrows, SetPrompts, SetHelp, SetUpStand and "
              "PutOnStand "
              "have no adapters: only native callers reach them, and the flag switches those");

namespace hooks {

namespace bs = build_screen;

namespace {

// The guest calls the plain C++ makes (build_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  bs::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
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
    lifted::CallFromContext(ctx, base, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

// GarageStage::ReloadModel for the stand: its plain C++ where its adapter
// would run it (--native_car_makers, --native_vehicle_accessors,
// --native_scene_models), else the function through `calls`.
bs::ReloadCall Reloader(lifted::Regs& ctx, uint8_t* base, const bs::Calls& calls) {
  return [&ctx, base, calls](uint32_t holder, uint32_t block, uint32_t frame,
                             uint32_t lr) -> uint64_t {
    if (ITC_FLAG(native_car_makers) && ITC_FLAG(native_vehicle_accessors) &&
        ITC_FLAG(native_scene_models)) {
      const uint64_t link = ctx.lr;
      ctx.fpscr.disableFlushMode();
      const front_end_car_draw::Calls fe = FrontEndCarDrawCalls(ctx, base);
      const uint64_t r3 = car_makers::GarageStageReloadModel(base, fe, holder, block, frame);
      ctx.fpscr.disableFlushMode();
      ctx.lr = link;
      return r3;
    }
    return calls.Call(0x8217F880, frame, lr, holder, block);
  };
}

}  // namespace

ITC_REGS_STEP(BuildScreenPutOnStand) {
  GuestCalls guest(ctx, base);
  const bs::Calls calls = guest.calls();
  ctx.r3.u64 = bs::PutOnStand(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, Reloader(ctx, base, calls));
}

ITC_REGS_STEP(BuildScreenSetUpStand) {
  GuestCalls guest(ctx, base);
  const bs::Calls calls = guest.calls();
  ctx.r3.u64 = bs::SetUpStand(base, ctx.r1.u32, calls, Reloader(ctx, base, calls));
}

ITC_REGS_STEP(BuildScreenOpenStand) {
  GuestCalls guest(ctx, base);
  const bs::Calls calls = guest.calls();
  ctx.r3.u64 = bs::OpenStand(base, ctx.r3.u32, ctx.r1.u32, calls, Reloader(ctx, base, calls));
}

ITC_REGS_STEP(BuildScreenKeepGroupList) {
  ctx.r3.u64 = bs::KeepGroupList(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenKeepBuildList) {
  ctx.r3.u64 = bs::KeepBuildList(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenKeepStandList) {
  ctx.r3.u64 = bs::KeepStandList(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenRestoreBuildList) {
  ctx.r3.u64 = bs::RestoreBuildList(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenRestoreStandList) {
  ctx.r3.u64 = bs::RestoreStandList(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenInitGroupLists) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::InitGroupLists(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenInitStandList) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::InitStandList(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenMarkGroups) {
  ctx.r3.u64 = bs::MarkGroups(base);
}

ITC_REGS_STEP(BuildScreenFilterGroups) {
  ctx.r3.u64 = bs::FilterGroups(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenValueText) {
  ctx.r3.u64 = bs::ValueText(base, ctx.r1.u32, ctx.r3.u32);
}

ITC_REGS_STEP(BuildListInsert) {
  bs::Insert(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(GarageStageSetDark) {
  ctx.r3.u64 = bs::SetDark(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(BuildScreenGroupArrows) {
  ctx.r3.u64 = bs::GroupArrows(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(BuildScreenBuildArrows) {
  ctx.r3.u64 = bs::BuildArrows(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(BuildScreenStandArrows) {
  ctx.r3.u64 = bs::StandArrows(base, ctx.r1.u32);
}

ITC_REGS_STEP(BuildScreenShowBuildMark) {
  ctx.r3.u64 = bs::ShowBuildMark(base, ctx.r1.u32, ctx.r3.u32);
}

ITC_REGS_STEP(BuildScreenSetPrompts) {
  const uint32_t pairs[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                             ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = bs::SetPrompts(base, ctx.r1.u32, pairs);
}

ITC_REGS_STEP(BuildScreenSetHelp) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::SetHelp(base, ctx.r1.u32, ctx.r3.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenShowNumber) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = bs::ShowNumber(base, ctx.r1.u32, ctx.r3.u32);
}

ITC_REGS_STEP(BuildScreenDrawGroupRows) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::DrawGroupRows(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenHighlightGroupsA) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightGroupsA(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenHighlightGroupsB) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightGroupsB(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenFadeOut) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::FadeOut(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenHighlightBuilds) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightBuilds(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenHighlightStand) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightStand(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenHighlightStand2) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightStand2(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenHighlightDetails) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightDetails(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenInitBuildList) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::InitBuildList(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenCopyBuilds) {
  ctx.r3.u64 = bs::CopyBuilds(base, ctx.r1.u32, ctx.r3.u64);
}

ITC_REGS_STEP(BuildScreenListBuilds) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::ListBuilds(base, ctx.r1.u32, ctx.r3.u64, guest.calls());
}

ITC_REGS_STEP(BuildScreenDrawBuildRows) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::DrawBuildRows(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenDrawStandRow) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::DrawStandRow(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenShowGroup) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::ShowGroup(base, ctx.r1.u32, ctx.r3.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenSetUpBuilds) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::SetUpBuilds(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenSetUpGroups) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::SetUpGroups(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenOpen) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::Open(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenBackToGroups) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::BackToGroups(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenOpenBuilds) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::OpenBuilds(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenRestoreStand) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::RestoreStand(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenBackToStand) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::BackToStand(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenOpenDetails) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::OpenDetails(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenSetUpDetails) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::SetUpDetails(base, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenShowDetails) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::ShowDetails(base, ctx.r1.u32, ctx.r3.u32, guest.calls());
}

ITC_REGS_STEP(BuildScreenChangeCategory) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::ChangeCategory(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_BUILD_SCREEN(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_build_screen)) { \
    hooks::step(ctx, base);               \
  }

ITC_BUILD_SCREEN(8219C230, BuildListInsert)
ITC_BUILD_SCREEN(8217D0C8, GarageStageSetDark)
ITC_BUILD_SCREEN(8219C528, BuildScreenHighlightGroupsA)
ITC_BUILD_SCREEN(8219C6D0, BuildScreenHighlightGroupsB)
ITC_BUILD_SCREEN(8219C850, BuildScreenFadeOut)
ITC_BUILD_SCREEN(8219C8F8, BuildScreenHighlightBuilds)
ITC_BUILD_SCREEN(8219CA08, BuildScreenHighlightStand)
ITC_BUILD_SCREEN(8219CB18, BuildScreenHighlightStand2)
ITC_BUILD_SCREEN(8219CBD8, BuildScreenHighlightDetails)
ITC_BUILD_SCREEN(821A1008, BuildScreenSetUpBuilds)
ITC_BUILD_SCREEN(821A08D8, BuildScreenOpen)
ITC_BUILD_SCREEN(821A0A38, BuildScreenBackToGroups)
ITC_BUILD_SCREEN(8219FC60, BuildScreenChangeCategory)
ITC_BUILD_SCREEN(821A1138, BuildScreenOpenBuilds)
ITC_BUILD_SCREEN(8219FF78, BuildScreenBackToStand)
ITC_BUILD_SCREEN(8219FEC8, BuildScreenOpenDetails)
ITC_BUILD_SCREEN(8219FE10, BuildScreenOpenStand)
// Removed (B7 loop, 2026-11-06, with SetUpStand native): KeepBuildList
// sub_8219BB20, RestoreBuildList sub_8219BB80, InitStandList sub_8219C078,
// DrawStandRow sub_8219D550, StandArrows sub_8219D7B0, SetPrompts
// sub_8219DA58 and SetHelp sub_8219DB50: their callers in play are native.
// BuildScreen::SetUpStand sub_8219F780 and PutOnStand sub_8219D6F0 have no
// adapters: only OpenStand calls SetUpStand, and PutOnStand's callers in
// play are SetUpStand (MoveStand and ChangeGroupOnStand never ran); their
// register steps above are the harness's.
