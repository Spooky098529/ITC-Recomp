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
              "HelpLine::Set's plain C++) "
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
// The license plate editor's handlers: member function functors the machine
// calls (no direct callers).
ITC_TYPE2_SCREENS(82198210, PlateCreate)
ITC_TYPE2_SCREENS(82198180, PlateOpenMovie)
ITC_TYPE2_SCREENS(821982F0, PlateWaitForLoad)
ITC_TYPE2_SCREENS(82198498, PlateClose)
ITC_TYPE2_SCREENS(82198118, PlateDestroy)
ITC_TYPE2_SCREENS(82198190, PlateOnMessage32)
ITC_TYPE2_SCREENS(82199BB8, PlateStart)
// The pages (from UpdatePage's table on its stack, recompiled).
ITC_TYPE2_SCREENS(82198988, PlateLeavePage)
// The helpers (from the recompiled transitions, pages and set-ups). No
// adapters for LoadPlate sub_82198420, RandomPlate sub_82198360 and
// ResetMovie sub_821989E8: only Start (native) reaches them, the flag
// switches it; their steps above are the harness's.
ITC_TYPE2_SCREENS(821981C8, PlateCharLineKind)
ITC_TYPE2_SCREENS(821998F0, PlateShowDigits)
ITC_TYPE2_SCREENS(82199990, PlateShowChars)
ITC_TYPE2_SCREENS(821992D8, PlateFinishRegionAnim)
ITC_TYPE2_SCREENS(82199410, PlateFinishDigitAnim)
ITC_TYPE2_SCREENS(82199688, PlateShowCharLine)
ITC_TYPE2_SCREENS(82199A20, PlateShowPlate)
ITC_TYPE2_SCREENS(82199C10, PlateSetUpRegionMenu)
ITC_TYPE2_SCREENS(82199CC8, PlateSetUpNumberMenu)
ITC_TYPE2_SCREENS(82199D88, PlateSetUpCharsMenu)
// No adapters for ShowRegionCells sub_821990D0 (SetUpRegionMenu;
// Transition3, never seen in play, reaches the original), ShowDigitCells
// sub_82199350 (only SetUpNumberMenu) and ShowCharCells sub_82199488
// (SetUpCharsMenu; Transition12, never seen in play, reaches the original),
// nor (removed, B7 loop, 2026-11-08) for ShowRegion sub_82199898 (ShowPlate;
// Transition3, never seen in play, reaches the original) and MarkField
// sub_82199AA8 (the three menus' set-ups; SetUpConfirmMenu, never seen in
// play, reaches the original): their callers in play are native, the flag
// switches those; their steps above are the harness's.
