// The build screen natively: adapters in front of src/build_screen/
// (docs/functions/build-screen.md), with the register steps the harness
// shares (hooks/build_screen.h).

#include "hooks/build_screen.h"

#include "base/flags.h"
#include "build_screen/build_screen.h"

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
              "overrides)) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=build_screen). RestoreGroupList, InsertNode, SetMode, PowI, DrawGroupRow, InitBuildList, "
              "CopyBuilds, ListBuilds, DrawBuildRow and DrawBuildRows have no adapters: only native callers reach them, and the flag switches those");

namespace hooks {

namespace bs = build_screen;

namespace {

// The guest calls the plain C++ makes (build_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  bs::Calls calls() { return {this, &GuestCalls::Call}; }

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

void BuildScreenKeepGroupList(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::KeepGroupList(base, ctx.r1.u32);
}

void BuildScreenKeepBuildList(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::KeepBuildList(base, ctx.r1.u32);
}

void BuildScreenKeepStandList(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::KeepStandList(base, ctx.r1.u32);
}

void BuildScreenRestoreBuildList(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::RestoreBuildList(base, ctx.r1.u32);
}

void BuildScreenRestoreStandList(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::RestoreStandList(base, ctx.r1.u32);
}

void BuildScreenInitGroupLists(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::InitGroupLists(base, ctx.r1.u32, guest.calls());
}

void BuildScreenInitStandList(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::InitStandList(base, ctx.r1.u32, guest.calls());
}

void BuildScreenMarkGroups(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::MarkGroups(base);
}

void BuildScreenFilterGroups(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::FilterGroups(base, ctx.r1.u32);
}

void BuildScreenValueText(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::ValueText(base, ctx.r1.u32, ctx.r3.u32);
}

void BuildListInsert(PPCContext& ctx, uint8_t* base) {
  bs::Insert(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void GarageStageSetDark(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::SetDark(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

void BuildScreenGroupArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::GroupArrows(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

void BuildScreenBuildArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::BuildArrows(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

void BuildScreenStandArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::StandArrows(base, ctx.r1.u32);
}

void BuildScreenShowBuildMark(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::ShowBuildMark(base, ctx.r1.u32, ctx.r3.u32);
}

void BuildScreenSetPrompts(PPCContext& ctx, uint8_t* base) {
  const uint32_t pairs[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                             ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = bs::SetPrompts(base, ctx.r1.u32, pairs);
}

void BuildScreenSetHelp(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::SetHelp(base, ctx.r1.u32, ctx.r3.u32, guest.calls());
}

void BuildScreenShowNumber(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = bs::ShowNumber(base, ctx.r1.u32, ctx.r3.u32);
}

void BuildScreenDrawGroupRows(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::DrawGroupRows(base, ctx.r1.u32, guest.calls());
}

void BuildScreenHighlightGroupsA(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightGroupsA(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenHighlightGroupsB(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightGroupsB(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenFadeOut(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::FadeOut(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenHighlightBuilds(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightBuilds(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenHighlightStand(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightStand(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenHighlightStand2(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightStand2(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenHighlightDetails(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::HighlightDetails(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void BuildScreenInitBuildList(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::InitBuildList(base, ctx.r1.u32, guest.calls());
}

void BuildScreenCopyBuilds(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = bs::CopyBuilds(base, ctx.r1.u32, ctx.r3.u64);
}

void BuildScreenListBuilds(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::ListBuilds(base, ctx.r1.u32, ctx.r3.u64, guest.calls());
}

void BuildScreenDrawBuildRows(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::DrawBuildRows(base, ctx.r1.u32, guest.calls());
}

void BuildScreenDrawStandRow(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::DrawStandRow(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, guest.calls());
}

void BuildScreenShowGroup(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::ShowGroup(base, ctx.r1.u32, ctx.r3.u32, guest.calls());
}

void BuildScreenSetUpBuilds(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = bs::SetUpBuilds(base, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_BUILD_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_build_screen)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_BUILD_SCREEN(8219BA50, BuildScreenKeepGroupList)
ITC_BUILD_SCREEN(8219BB20, BuildScreenKeepBuildList)
ITC_BUILD_SCREEN(8219BBF0, BuildScreenKeepStandList)
ITC_BUILD_SCREEN(8219BB80, BuildScreenRestoreBuildList)
ITC_BUILD_SCREEN(8219BC50, BuildScreenRestoreStandList)
ITC_BUILD_SCREEN(8219BF08, BuildScreenInitGroupLists)
ITC_BUILD_SCREEN(8219C078, BuildScreenInitStandList)
ITC_BUILD_SCREEN(8219EC50, BuildScreenMarkGroups)
ITC_BUILD_SCREEN(8219CC70, BuildScreenFilterGroups)
ITC_BUILD_SCREEN(8219BCC0, BuildScreenValueText)
ITC_BUILD_SCREEN(8219C230, BuildListInsert)
ITC_BUILD_SCREEN(8217D0C8, GarageStageSetDark)
ITC_BUILD_SCREEN(8219D1B0, BuildScreenGroupArrows)
ITC_BUILD_SCREEN(8219D4B8, BuildScreenBuildArrows)
ITC_BUILD_SCREEN(8219D7B0, BuildScreenStandArrows)
ITC_BUILD_SCREEN(8219D440, BuildScreenShowBuildMark)
ITC_BUILD_SCREEN(8219DA58, BuildScreenSetPrompts)
ITC_BUILD_SCREEN(8219DB50, BuildScreenSetHelp)
ITC_BUILD_SCREEN(8219D090, BuildScreenShowNumber)
ITC_BUILD_SCREEN(8219FA28, BuildScreenDrawGroupRows)
ITC_BUILD_SCREEN(8219C528, BuildScreenHighlightGroupsA)
ITC_BUILD_SCREEN(8219C6D0, BuildScreenHighlightGroupsB)
ITC_BUILD_SCREEN(8219C850, BuildScreenFadeOut)
ITC_BUILD_SCREEN(8219C8F8, BuildScreenHighlightBuilds)
ITC_BUILD_SCREEN(8219CA08, BuildScreenHighlightStand)
ITC_BUILD_SCREEN(8219CB18, BuildScreenHighlightStand2)
ITC_BUILD_SCREEN(8219CBD8, BuildScreenHighlightDetails)
ITC_BUILD_SCREEN(8219D550, BuildScreenDrawStandRow)
ITC_BUILD_SCREEN(8219CF08, BuildScreenShowGroup)
ITC_BUILD_SCREEN(821A1008, BuildScreenSetUpBuilds)
