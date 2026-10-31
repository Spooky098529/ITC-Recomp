// The other in-race screens natively: adapters in front of src/race_screens/
// (docs/functions/race-screens.md), with the register steps the harness
// shares (hooks/race_screens.h).

#include "hooks/race_screens.h"

#include "base/flags.h"
#include "race_screens/race_screens.h"

ITC_FLAG_BOOL(native_race_screens, true, "ITC",
              "Run the other in-race screens' functions (docs/functions/race-screens.md: the "
              "page object's ScreenPages::Init sub_8220BA98, Go sub_8220BA68 and Finish "
              "sub_8220BA28; the menu list's Selection sub_8220BC20, SetSelection sub_8220BC30, "
              "SetEnabled sub_8220BC00, IsEnabled sub_8220BC10, Fit sub_8220BC40, Fired "
              "sub_8220C020, Update sub_8220C1E0, Accepted sub_8220BD50, Cancelled sub_8220BDE0, "
              "Pressed152 sub_8220BE70 and Pressed166 sub_8220BF90, the mapper's read slots by "
              "their plain C++ with --native_action_mappers, else through the vtable; Go and "
              "Update keep their --log_pause_movie and --log_pause_menu lines) natively instead "
              "of through the translated code (the same writes; checked by "
              "--run_harnesses=race_screens)");
ITC_FLAG_DECLARE(bool, native_action_mappers);

namespace hooks {

namespace {

// The guest calls the plain C++ makes (race_screens::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  race_screens::Calls calls() { return {this, &GuestCalls::Call}; }

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

bool NativeSlots() { return ITC_FLAG(native_action_mappers); }

}  // namespace

void RaceScreensPagesInit(PPCContext& ctx, uint8_t* base) {
  race_screens::PagesInit(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensPagesGo(PPCContext& ctx, uint8_t* base) {
  race_screens::PagesGo(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

void RaceScreensPagesFinish(PPCContext& ctx, uint8_t* base) {
  race_screens::PagesFinish(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListSelection(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::ListSelection(base, ctx.r3.u32);
}

void RaceScreensListSetSelection(PPCContext& ctx, uint8_t* base) {
  race_screens::ListSetSelection(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListSetEnabled(PPCContext& ctx, uint8_t* base) {
  race_screens::ListSetEnabled(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void RaceScreensListIsEnabled(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::ListIsEnabled(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListFit(PPCContext& ctx, uint8_t* base) {
  race_screens::ListFit(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListFired(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::ListFired(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, NativeSlots(),
                                       guest.calls());
}

void RaceScreensListUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::ListUpdate(base, ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls());
}

#define ITC_RACE_SCREENS_PRESSED(name)                                                         \
  void RaceScreensList##name(PPCContext& ctx, uint8_t* base) {                                \
    GuestCalls guest(ctx, base);                                                               \
    ctx.r3.u64 =                                                                               \
        race_screens::List##name(base, ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls()); \
  }
ITC_RACE_SCREENS_PRESSED(Accepted)
ITC_RACE_SCREENS_PRESSED(Cancelled)
ITC_RACE_SCREENS_PRESSED(Pressed152)
ITC_RACE_SCREENS_PRESSED(Pressed166)
#undef ITC_RACE_SCREENS_PRESSED

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. ScreenPages::Go's and MenuList::Update's are their logging overrides
// (src/debug/pause_movie_log.cpp, pause_menu_log.cpp), which call the steps.

#define ITC_RACE_SCREENS(address, step)       \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_race_screens)) {     \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

ITC_RACE_SCREENS(8220BA98, RaceScreensPagesInit)
ITC_RACE_SCREENS(8220BA28, RaceScreensPagesFinish)
ITC_RACE_SCREENS(8220BC20, RaceScreensListSelection)
ITC_RACE_SCREENS(8220BC30, RaceScreensListSetSelection)
ITC_RACE_SCREENS(8220BC00, RaceScreensListSetEnabled)
ITC_RACE_SCREENS(8220BC10, RaceScreensListIsEnabled)
ITC_RACE_SCREENS(8220BC40, RaceScreensListFit)
ITC_RACE_SCREENS(8220C020, RaceScreensListFired)
ITC_RACE_SCREENS(8220BD50, RaceScreensListAccepted)
ITC_RACE_SCREENS(8220BDE0, RaceScreensListCancelled)
ITC_RACE_SCREENS(8220BE70, RaceScreensListPressed152)
ITC_RACE_SCREENS(8220BF90, RaceScreensListPressed166)
