// The title screen natively: adapters in front of src/title_screen/
// (docs/functions/title-screen.md), with the register steps the harness
// shares (hooks/title_screen_native.h). TitleScreen::Update sub_822221A0
// keeps its wrap in hooks/title_screen.cpp (--attract_after_s).

#include "hooks/title_screen_native.h"

#include "base/flags.h"
#include "title_screen/title_screen.h"

ITC_FLAG_BOOL(native_title_screen, true, "ITC",
              "Run the type 4 front-end mode (docs/functions/title-screen.md: the title "
              "screen, its menus and the title load before it) natively instead of through "
              "the translated code: the title load's machine, Create sub_82220BA8 and Destroy "
              "sub_82220468 (VirtualAlloc and VirtualFree through their overrides), OpenMovie "
              "sub_822204B0, WaitForMovie sub_82220C18 (UiMovies::IsLoaded's plain C++), Start "
              "sub_82220C88 (BootData::Load through its override), Draw sub_82221198 and "
              "Update sub_82221228 (the pages and transitions through the guest from their "
              "tables, Archive::QueueLength through its override), PlayLogo sub_822210C8 "
              "(UiMovie::PlayNamed, Run and the fade through their overrides), WaitForArchive "
              "sub_822204C0, PostClose sub_82221078, Unload sub_82220EF8, OpenTitle "
              "sub_82220F60 and Leave sub_82220FC8 (UiMovie::FinishAll and UiMovies::Delete "
              "through their overrides); TitleLoad::Init sub_822213B0 and TitleScreen::Init "
              "sub_82224270 (their step tables, StateMachine::Add's plain C++) (the same "
              "writes; checked by --run_harnesses=title_screen)");

namespace hooks {

namespace {

namespace ts = title_screen;

// The guest calls the plain C++ makes (title_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  ts::Calls calls() { return {this, &GuestCalls::Call}; }

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

// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 screens' Inits.
void InitSteps(PPCContext& ctx, uint8_t* base,
               uint32_t (*init)(uint8_t*, uint32_t, uint32_t, bool*), uint32_t frame_bytes) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = init(base, machine, ctx.r1.u32 - frame_bytes, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace

bool TitleScreenNative() { return ITC_FLAG(native_title_screen); }

void TitleLoadCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadOpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void TitleLoadWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::LoadWaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void TitleLoadStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadStart(base, ctx.r1.u32, guest.calls());
}
void TitleLoadDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadDraw(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadUpdate(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadPlayLogo(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadPlayLogo(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadWaitForArchive(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadWaitForArchive(base, ctx.r1.u32, guest.calls());
}
void TitleLoadPostClose(PPCContext& ctx, uint8_t* base) { ts::LoadPostClose(base, ctx.r3.u32); }
void TitleLoadUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadUnload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadOpenTitle(PPCContext& ctx, uint8_t* base) {
  ts::LoadOpenTitle(base, ctx.r3.u32, ctx.r4.u32);
}
void TitleLoadLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadLeave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadDestroy(base, ctx.r1.u32, guest.calls());
}
void TitleLoadInit(PPCContext& ctx, uint8_t* base) {
  InitSteps(ctx, base, ts::LoadInit, ts::kLoadInitFrame);
}
void TitleScreenInit(PPCContext& ctx, uint8_t* base) {
  InitSteps(ctx, base, ts::ScreenInit, ts::kScreenInitFrame);
}

}  // namespace hooks

#define ITC_TITLE_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_title_screen)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

// The title load's handlers (member function functors the machine calls; no
// direct callers).
ITC_TITLE_SCREEN(82220BA8, TitleLoadCreate)
ITC_TITLE_SCREEN(822204B0, TitleLoadOpenMovie)
ITC_TITLE_SCREEN(82220C18, TitleLoadWaitForMovie)
ITC_TITLE_SCREEN(82220C88, TitleLoadStart)
ITC_TITLE_SCREEN(82221198, TitleLoadDraw)
ITC_TITLE_SCREEN(82221228, TitleLoadUpdate)
ITC_TITLE_SCREEN(82220EF8, TitleLoadUnload)
ITC_TITLE_SCREEN(82220F60, TitleLoadOpenTitle)
ITC_TITLE_SCREEN(82220FC8, TitleLoadLeave)
ITC_TITLE_SCREEN(82220468, TitleLoadDestroy)
// Its pages (from Draw's table on its stack) and transition (from Update's).
ITC_TITLE_SCREEN(82221078, TitleLoadPostClose)
ITC_TITLE_SCREEN(822204C0, TitleLoadWaitForArchive)
ITC_TITLE_SCREEN(822210C8, TitleLoadPlayLogo)
// The Inits (from ModeType4::Init sub_82224C70).
ITC_TITLE_SCREEN(822213B0, TitleLoadInit)
ITC_TITLE_SCREEN(82224270, TitleScreenInit)
