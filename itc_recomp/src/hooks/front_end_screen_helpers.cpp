// The front end's screen helpers natively: adapters in front of
// src/front_end_screen_helpers/ (docs/functions/front-end-screen-helpers.md),
// with the register steps the harness shares
// (hooks/front_end_screen_helpers.h).

#include "hooks/front_end_screen_helpers.h"

#include "base/flags.h"
#include "front_end_screen_helpers/front_end_screen_helpers.h"
#include "hooks/heap.h"

#include <optional>

ITC_FLAG_BOOL(native_front_end_screen_helpers, true, "ITC",
              "Run the front end's screen helpers (docs/functions/front-end-screen-helpers.md: "
              "the modes' releases ModeType2::Release sub_82196EC0, ModeType4::Release "
              "sub_82224AB8, ModeType5::Release sub_8222FB90, ModeType9::Release sub_8225D590, "
              "ModeType10::Release sub_82288898 and ModeType12::Release sub_82277DB0 (the block "
              "over the allocation layer, the original where it isn't its pages; the sound "
              "manager's IsCuePlaying and StopCue through its vtable, StandScene::Release's plain "
              "C++, Strings::Release and UiMovies::DeleteAll's jump through their overrides); "
              "the dialogs' text setters SaveScreen::SetText sub_821A51C8, SetNote sub_821A52C0 "
              "and SetMessage sub_821A5360, Options::SetSaveDialogText sub_821C0D70, "
              "SetSaveDialogNote sub_821C0E50, SetCategory6DialogText sub_821C0BF0 and "
              "SetCategory6DialogNote sub_821C0CD0 (the UI movie setters through their "
              "overrides, their unset varargs 0) and SaveScreen::ShowNotice sub_821A63B0; the "
              "car cards CarCard::Fill sub_821821C8, CarCard::FillDetail sub_821827E0 and "
              "CarDetail::Open sub_821830C0; SaveScreen::StartCheck sub_821A5450 and Start "
              "sub_821A5540, Options::SetUpSavePage sub_821C6D48 and SetUpCategory6Page "
              "sub_821C6E50 (MenuList::Init, SaveSlots::State, ShowPrompts and "
              "SaveBuffers::Create through their overrides, the setters' varargs 0) and "
              "CarCards::FillAll sub_821822F8; the dialogs' steps SaveScreen::ShowMessage "
              "sub_821A5668, ShowMessage3 sub_821A5980 and ShowMessage4 sub_821A5D00 and "
              "Options::ShowSaveDialog sub_821C6638 and ShowCategory6Dialog sub_821C69D0 (with "
              "their split-off case 0s; the setters' plain C++, their unset varargs 0; the UI "
              "movie calls, MenuList::, MenuSound::Play and ShowPrompts through their "
              "overrides)) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=front_end_screen_helpers)");

namespace hooks {

namespace {

namespace fe = front_end_screen_helpers;

// The guest calls the plain C++ makes (front_end_screen_helpers::Calls),
// through this context: r1, r3 to r10, f1 when given and lr set, the
// function run through the function table (its override when it has one),
// r1 put back. The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  fe::Calls calls() { return {this, &GuestCalls::Call}; }

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

bool Set(PPCContext& ctx, const std::optional<uint64_t>& r3) {
  if (!r3) {
    return false;
  }
  ctx.r3.u64 = *r3;
  return true;
}

using Release = std::optional<uint64_t> (*)(uint8_t*, uint32_t, uint32_t, const fe::Calls&);

bool RunRelease(PPCContext& ctx, uint8_t* base, Release release) {
  GuestCalls guest(ctx, base);
  return Set(ctx, release(base, ctx.r3.u32, ctx.r1.u32, guest.calls()));
}

void Varargs(const PPCContext& ctx, uint64_t varargs[7]) {
  const uint64_t from[7] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                            ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  for (int i = 0; i < 7; i++) {
    varargs[i] = from[i];
  }
}

}  // namespace

bool FrontEndModeType2Release(PPCContext& ctx, uint8_t* base) {
  return RunRelease(ctx, base, fe::ModeType2Release);
}
bool FrontEndModeType4Release(PPCContext& ctx, uint8_t* base) {
  return RunRelease(ctx, base, fe::ModeType4Release);
}
bool FrontEndModeType5Release(PPCContext& ctx, uint8_t* base) {
  return RunRelease(ctx, base, fe::ModeType5Release);
}
bool FrontEndModeType9Release(PPCContext& ctx, uint8_t* base) {
  return RunRelease(ctx, base, fe::ModeType9Release);
}
bool FrontEndModeType10Release(PPCContext& ctx, uint8_t* base) {
  return RunRelease(ctx, base, fe::ModeType10Release);
}
bool FrontEndModeType12Release(PPCContext& ctx, uint8_t* base) {
  return RunRelease(ctx, base, fe::ModeType12Release);
}

void FrontEndSaveScreenSetText(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 = fe::SaveScreenSetText(base, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenSetNote(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 = fe::SaveScreenSetNote(base, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenSetMessage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 = fe::SaveScreenSetMessage(base, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsSetSaveDialogText(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 =
      fe::OptionsSetDialogText(base, 13, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsSetSaveDialogNote(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 =
      fe::OptionsSetDialogNote(base, 13, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsSetCategory6DialogText(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 =
      fe::OptionsSetDialogText(base, 14, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsSetCategory6DialogNote(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  uint64_t varargs[7];
  Varargs(ctx, varargs);
  ctx.r3.u64 =
      fe::OptionsSetDialogNote(base, 14, ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenShowNotice(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::SaveScreenShowNotice(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void FrontEndSaveScreenStartCheck(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::SaveScreenStartCheck(base, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::SaveScreenStart(base, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsSetUpSavePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::OptionsSetUpDialogPage(base, 13, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsSetUpCategory6Page(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::OptionsSetUpDialogPage(base, 14, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenShowMessage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::SaveScreenShowMessage(base, 0, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenShowMessage3(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::SaveScreenShowMessage(base, 1, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void FrontEndSaveScreenShowMessage4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::SaveScreenShowMessage(base, 2, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsShowSaveDialog(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::OptionsShowDialog(base, 13, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void FrontEndOptionsShowCategory6Dialog(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::OptionsShowDialog(base, 14, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void FrontEndCarCardsFillAll(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::CarCardsFillAll(base, ctx.r1.u32, guest.calls());
}

void FrontEndCarCardFill(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::CarCardFill(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void FrontEndCarCardFillDetail(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::CarCardFillDetail(base, ctx.r1.u32, guest.calls());
}
void FrontEndCarDetailOpen(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = fe::CarDetailOpen(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_FRONT_END_HELPERS(address, step)              \
  extern "C" REX_FUNC(sub_##address) {                    \
    REX_FUNC_PROLOGUE();                                  \
    if (!ITC_FLAG(native_front_end_screen_helpers)) {     \
      __imp__sub_##address(ctx, base);                    \
      return;                                             \
    }                                                     \
    hooks::step(ctx, base);                               \
  }

#define ITC_FRONT_END_RELEASE(address, step)                                         \
  extern "C" REX_FUNC(sub_##address) {                                               \
    REX_FUNC_PROLOGUE();                                                             \
    if (!ITC_FLAG(native_front_end_screen_helpers) || !hooks::NativeHeapActive() ||  \
        !hooks::step(ctx, base)) {                                                   \
      __imp__sub_##address(ctx, base);                                               \
    }                                                                                \
  }

// The modes' releases: slot 1 of their vtables, from Modes::Tick (no
// direct callers).
ITC_FRONT_END_RELEASE(82196EC0, FrontEndModeType2Release)
ITC_FRONT_END_RELEASE(82224AB8, FrontEndModeType4Release)
ITC_FRONT_END_RELEASE(8222FB90, FrontEndModeType5Release)
ITC_FRONT_END_RELEASE(8225D590, FrontEndModeType9Release)
ITC_FRONT_END_RELEASE(82288898, FrontEndModeType10Release)
ITC_FRONT_END_RELEASE(82277DB0, FrontEndModeType12Release)

// No adapters for the dialogs' text setters SaveScreen::SetText
// sub_821A51C8 and SetNote sub_821A52C0, Options::SetSaveDialogText
// sub_821C0D70, SetSaveDialogNote sub_821C0E50, SetCategory6DialogText
// sub_821C0BF0 and SetCategory6DialogNote sub_821C0CD0 (removed, B7 loop,
// 2026-11-08): their callers in play (the save screen's and the options'
// steps and starts) are native, the flag switches those; their steps above
// are the harness's. SaveScreen::ShowMessage5 sub_821A6030 (never seen in
// play) reaches the originals.
// The save screen's step 6 (its Update's table, no direct callers).
ITC_FRONT_END_HELPERS(821A63B0, FrontEndSaveScreenShowNotice)
// No adapter for SaveScreen::SetMessage sub_821A5360: its only caller,
// ShowNotice, is native (the flag switches it); its step above is the
// harness's.

// No adapters for the save screen's starts SaveScreen::StartCheck
// sub_821A5450 and Start sub_821A5540 (removed, B7 loop, 2026-11-09): their
// callers, SaveScreen::Show sub_821A6608 and its case 0 sub_821A6650, are
// native (--native_save_screen switches them); their steps above are the
// harness's. The options' dialog pages' set-ups (from Options::OpenCategory
// sub_821C7188, recompiled).
ITC_FRONT_END_HELPERS(821C6D48, FrontEndOptionsSetUpSavePage)
ITC_FRONT_END_HELPERS(821C6E50, FrontEndOptionsSetUpCategory6Page)
// The dialogs' steps: the save screen's Update table (steps 0, 3 and 4)
// and Options::Update's (43 and 46), no direct callers. Their jump tables'
// split-off case 0 functions (sub_821A56E8, sub_821A5A00, sub_821A5D7C,
// sub_821C66BC, sub_821C6A54) are reached only from those tables, so they
// need no adapters.
ITC_FRONT_END_HELPERS(821A5668, FrontEndSaveScreenShowMessage)
ITC_FRONT_END_HELPERS(821A5980, FrontEndSaveScreenShowMessage3)
ITC_FRONT_END_HELPERS(821A5D00, FrontEndSaveScreenShowMessage4)
ITC_FRONT_END_HELPERS(821C6638, FrontEndOptionsShowSaveDialog)
ITC_FRONT_END_HELPERS(821C69D0, FrontEndOptionsShowCategory6Dialog)
// CarCards::FillAll (from sub_82183DE0 and sub_82184488, recompiled).
ITC_FRONT_END_HELPERS(821822F8, FrontEndCarCardsFillAll)

// The car cards: Fill from CarCards::FillAll and sub_82183DE0 (recompiled),
// Open through a functor (no direct callers). No adapter for
// CarCard::FillDetail sub_821827E0: its only caller, Open, is native (the
// flag switches it); its step above is the harness's.
ITC_FRONT_END_HELPERS(821821C8, FrontEndCarCardFill)
ITC_FRONT_END_HELPERS(821830C0, FrontEndCarDetailOpen)
