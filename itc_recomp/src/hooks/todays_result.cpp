// The type 9 mode (the career's end of the day) natively: adapters in front
// of src/todays_result/ (docs/functions/todays-result.md), with the register
// steps the harness shares (hooks/todays_result.h).

#include "hooks/todays_result.h"

#include "base/flags.h"
#include "todays_result/todays_result.h"

ITC_FLAG_BOOL(native_todays_result, true, "ITC",
              "Run the type 9 front-end mode (docs/functions/todays-result.md: the career's "
              "end of the day, today's result and today's event) natively instead of through "
              "the translated code: ModeType9::Init sub_8225D6D0 (TodaysResult::InitStates "
              "sub_8225B358 and TodaysEvent::InitStates sub_8225CF30 as plain C++ over "
              "StateMachine::Add's; the screen start, Strings::Load, VirtualAlloc, the loading "
              "screen and ScreenFades::FadeIn through their overrides), WaitForLoads "
              "sub_8225D918, NextMode sub_8225DC38, LeaveScreen sub_8225DB70, WaitForShown "
              "sub_8225DA68 and Leaving sub_8225DE78 (GameMessages::Dispatch through its "
              "override; FindShown sub_8225D658 their plain C++), IsLoaded sub_8225D6A8 and "
              "TodaysEvent::Kind sub_8225BA48; today's result's machine, Create sub_8225A7A0 and "
              "Destroy sub_8225A8A8 (VirtualAlloc and VirtualFree through their overrides), "
              "OpenMovie sub_8225A510, WaitForMovie sub_8225A930, Start sub_8225B300 with "
              "ReadDay sub_8225A520 (memcpy, DayBlock::WinStreak and Word4832 and "
              "Career::NextDay through their overrides), SetUpMovie sub_8225B258 and SetTexts "
              "sub_8225ADE0 (the text setters through their overrides, their unset varargs 0), "
              "Draw sub_8225B0C8 and Update sub_8225B128 (the pages and transitions through the "
              "guest from their tables), Unload sub_8225AA20, Unloaded sub_8225A9A0 and Leave "
              "sub_8225AA88; its pages WaitPage sub_8225A9B0 (Menu::Pressed and MenuSound::Play "
              "through their overrides) and LeavePage sub_8225AB30 with QueueMovie sub_8225A720 "
              "(Progress::CheckUnlocks, recompiled, and RaceEvent::Destroy through the guest); "
              "its transitions Intro sub_8225ABD8, ShowLoop sub_8225AC88 and FadeOut "
              "sub_8225AD20; today's event's machine, Create sub_8225BE50 with MarkImages "
              "sub_8225BAB8 (DataTables::Entry56's plain C++), OpenMovie sub_8225C130 with "
              "LoadImages sub_8225C008 and LoadImagePair sub_8225BD10 (Archive::AssetSize, "
              "XMemSet and Archive::Load through their overrides), WaitForMovie sub_8225C160 "
              "with ImagesLoadedA sub_8225C0A0 and B sub_8225C0E8, Start sub_8225C990 with "
              "SetUpTexts sub_8225C648 and NextUnlockEntry sub_8225BB70 (Sound::Play through "
              "the guest), Unload sub_8225C1F8, Unloaded sub_8225C278 (the sound manager's "
              "slots through the guest), Leave sub_8225C348 and Destroy sub_8225BF78 with "
              "FreeImages sub_8225BC60 (Type2Hub::FreeImage through its override), Draw "
              "sub_8225C928 and Update sub_8225CDE8 (the pages and transitions, with "
              "Type2Hub::Transition9 and Transition11, through the guest from their tables); "
              "its pages EventPage sub_8225C428 (Menu::Pressed, MenuSound::Play, RevealText and "
              "HasMoreText through their overrides) and EndPage sub_8225C590 with "
              "QueueEventMovies sub_8225B9B8 (Progress::CheckUnlocks, recompiled, through the "
              "guest); its transitions Intro sub_8225CB18, FadeOut sub_8225C770, NextUnlock "
              "sub_8225CC80 and NextPage sub_8225CD50 with ShowUnlock sub_8225CA50 and "
              "ShowImages sub_8225C848 (checked by --run_harnesses=todays_result)");

namespace hooks {

namespace {

namespace tr = todays_result;

// The guest calls the plain C++ makes (todays_result::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  tr::Calls calls() { return {this, &GuestCalls::Call}; }

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

// StateMachine::Add's r3: a new transition's address from the machine's
// 64-bit r3, as the title screen's Inits.
template <uint32_t (*InitStates)(uint8_t*, uint32_t, uint32_t, bool*)>
void InitStatesStep(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = InitStates(base, machine, ctx.r1.u32 - tr::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace

bool TodaysResultNative() { return ITC_FLAG(native_todays_result); }

void ModeType9Init(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ModeInit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// The delegate functions return the answer (r3 as it came).
#define ITC_MODE_TYPE9_DELEGATE(name)                                       \
  void ModeType9##name(PPCContext& ctx, uint8_t* base) {                    \
    const uint64_t answer = ctx.r3.u64;                                     \
    {                                                                       \
      GuestCalls guest(ctx, base);                                          \
      tr::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());    \
    }                                                                       \
    ctx.r3.u64 = answer;                                                    \
  }
ITC_MODE_TYPE9_DELEGATE(WaitForLoads)
ITC_MODE_TYPE9_DELEGATE(NextMode)
ITC_MODE_TYPE9_DELEGATE(LeaveScreen)
ITC_MODE_TYPE9_DELEGATE(WaitForShown)
ITC_MODE_TYPE9_DELEGATE(Leaving)
#undef ITC_MODE_TYPE9_DELEGATE
void ModeType9IsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::IsLoaded(base, ctx.r3.u32);
}
void ModeType9FindShown(PPCContext& ctx, uint8_t* base) { tr::FindShown(base, ctx.r3.u32); }
void TodaysEventKind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::Kind(base, ctx.r1.u32);
}
void TodaysResultInitStates(PPCContext& ctx, uint8_t* base) {
  InitStatesStep<tr::ResultInitStates>(ctx, base);
}
void TodaysEventInitStates(PPCContext& ctx, uint8_t* base) {
  InitStatesStep<tr::EventInitStates>(ctx, base);
}

void TodaysResultCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ResultCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TodaysResultOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ResultOpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void TodaysResultWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::ResultWaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
// The handlers and helpers with no argument but the frame.
#define ITC_TODAYS_RESULT_FRAME(name, function)                     \
  void name(PPCContext& ctx, uint8_t* base) {                       \
    GuestCalls guest(ctx, base);                                    \
    ctx.r3.u64 = tr::function(base, ctx.r1.u32, guest.calls());     \
  }
ITC_TODAYS_RESULT_FRAME(TodaysResultStart, ResultStart)
ITC_TODAYS_RESULT_FRAME(TodaysResultReadDay, ReadDay)
ITC_TODAYS_RESULT_FRAME(TodaysResultSetUpMovie, SetUpMovie)
ITC_TODAYS_RESULT_FRAME(TodaysResultSetTexts, SetTexts)
ITC_TODAYS_RESULT_FRAME(TodaysResultUpdate, ResultUpdate)
ITC_TODAYS_RESULT_FRAME(TodaysResultWaitPage, ResultWaitPage)
ITC_TODAYS_RESULT_FRAME(TodaysResultQueueMovie, ResultQueueMovie)
#undef ITC_TODAYS_RESULT_FRAME
void TodaysResultDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ResultDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void TodaysResultLeavePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ResultLeavePage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// The transitions (r3 the started word).
#define ITC_TODAYS_RESULT_TRANSITION(name, function)                           \
  void name(PPCContext& ctx, uint8_t* base) {                                  \
    GuestCalls guest(ctx, base);                                               \
    ctx.r3.u64 = tr::function(base, ctx.r3.u32, ctx.r1.u32, guest.calls());    \
  }
ITC_TODAYS_RESULT_TRANSITION(TodaysResultIntro, ResultIntro)
ITC_TODAYS_RESULT_TRANSITION(TodaysResultShowLoop, ResultShowLoop)
ITC_TODAYS_RESULT_TRANSITION(TodaysResultFadeOut, ResultFadeOut)
ITC_TODAYS_RESULT_TRANSITION(TodaysResultUnload, ResultUnload)
ITC_TODAYS_RESULT_TRANSITION(TodaysResultDestroy, ResultDestroy)
#undef ITC_TODAYS_RESULT_TRANSITION
void TodaysResultUnloaded(PPCContext& ctx, uint8_t* base) {
  tr::ResultUnloaded(base, ctx.r3.u32);
}
void TodaysResultLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ResultLeave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void TodaysEventCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TodaysEventMarkImages(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::MarkImages(base, ctx.r1.u32);
}
void TodaysEventOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventOpenMovie(base, ctx.r1.u32, guest.calls());
}
void TodaysEventLoadImages(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::LoadImages(base, ctx.r1.u32, guest.calls());
}
void TodaysEventLoadImagePair(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::LoadImagePair(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TodaysEventWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::EventWaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void TodaysEventImagesLoadedA(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::ImagesLoadedA(base, ctx.r3.u32);
}
void TodaysEventImagesLoadedB(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::ImagesLoadedB(base, ctx.r3.u32);
}
void TodaysEventStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventStart(base, ctx.r1.u32, guest.calls());
}
void TodaysEventSetUpTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::SetUpTexts(base, ctx.r1.u32, guest.calls());
}
void TodaysEventNextUnlockEntry(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::NextUnlockEntry(base, ctx.r1.u32);
}
void TodaysEventUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventUnload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TodaysEventUnloaded(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventUnloaded(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TodaysEventLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventLeave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TodaysEventDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventDestroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TodaysEventFreeImages(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::FreeImages(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void TodaysEventDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EventDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
#define ITC_TODAYS_EVENT_FRAME(name, function)                      \
  void name(PPCContext& ctx, uint8_t* base) {                       \
    GuestCalls guest(ctx, base);                                    \
    ctx.r3.u64 = tr::function(base, ctx.r1.u32, guest.calls());     \
  }
ITC_TODAYS_EVENT_FRAME(TodaysEventUpdate, EventUpdate)
ITC_TODAYS_EVENT_FRAME(TodaysEventEventPage, EventPage)
ITC_TODAYS_EVENT_FRAME(TodaysEventQueueEventMovies, QueueEventMovies)
ITC_TODAYS_EVENT_FRAME(TodaysEventShowUnlock, ShowUnlock)
#undef ITC_TODAYS_EVENT_FRAME
void TodaysEventEndPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::EndPage(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
#define ITC_TODAYS_EVENT_TRANSITION(name, function)                            \
  void name(PPCContext& ctx, uint8_t* base) {                                  \
    GuestCalls guest(ctx, base);                                               \
    ctx.r3.u64 = tr::function(base, ctx.r3.u32, ctx.r1.u32, guest.calls());    \
  }
ITC_TODAYS_EVENT_TRANSITION(TodaysEventIntro, EventIntro)
ITC_TODAYS_EVENT_TRANSITION(TodaysEventFadeOut, EventFadeOut)
ITC_TODAYS_EVENT_TRANSITION(TodaysEventNextUnlock, NextUnlock)
ITC_TODAYS_EVENT_TRANSITION(TodaysEventNextPage, NextPage)
#undef ITC_TODAYS_EVENT_TRANSITION
void TodaysEventShowImages(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::ShowImages(base, ctx.r3.u32, ctx.r1.u32);
}

}  // namespace hooks

#define ITC_TODAYS_RESULT(address, step)   \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_todays_result)) { \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

// The mode: Init (vtable 0x82006CAC's slot 0, from Modes::Tick), the
// delegate functions (its slots 6, 8, 10, 11 and 13, through the slot
// thunks) and IsLoaded (the +32 vtable 0x82006CA0's slot 0). FindShown
// sub_8225D658 and the two InitStates: no adapters (only the mode's
// functions, native, call them; the register steps above are the
// harness's). Kind: today's event's Create and EndPage call it too.
ITC_TODAYS_RESULT(8225D6D0, ModeType9Init)
ITC_TODAYS_RESULT(8225D918, ModeType9WaitForLoads)
ITC_TODAYS_RESULT(8225DC38, ModeType9NextMode)
ITC_TODAYS_RESULT(8225DB70, ModeType9LeaveScreen)
ITC_TODAYS_RESULT(8225DA68, ModeType9WaitForShown)
ITC_TODAYS_RESULT(8225DE78, ModeType9Leaving)
ITC_TODAYS_RESULT(8225D6A8, ModeType9IsLoaded)
ITC_TODAYS_RESULT(8225BA48, TodaysEventKind)
// Today's result's handlers (member function functors the machine calls; no
// direct callers), pages (from Draw's table on its stack) and transitions
// (from Update's table). ReadDay sub_8225A520, SetUpMovie sub_8225B258,
// SetTexts sub_8225ADE0 and QueueMovie sub_8225A720: no adapters (only Start
// and LeavePage, native, call them).
ITC_TODAYS_RESULT(8225A7A0, TodaysResultCreate)
ITC_TODAYS_RESULT(8225A510, TodaysResultOpenMovie)
ITC_TODAYS_RESULT(8225A930, TodaysResultWaitForMovie)
ITC_TODAYS_RESULT(8225B300, TodaysResultStart)
ITC_TODAYS_RESULT(8225B0C8, TodaysResultDraw)
ITC_TODAYS_RESULT(8225B128, TodaysResultUpdate)
ITC_TODAYS_RESULT(8225A9B0, TodaysResultWaitPage)
ITC_TODAYS_RESULT(8225AB30, TodaysResultLeavePage)
ITC_TODAYS_RESULT(8225ABD8, TodaysResultIntro)
ITC_TODAYS_RESULT(8225AC88, TodaysResultShowLoop)
ITC_TODAYS_RESULT(8225AD20, TodaysResultFadeOut)
ITC_TODAYS_RESULT(8225AA20, TodaysResultUnload)
ITC_TODAYS_RESULT(8225A9A0, TodaysResultUnloaded)
ITC_TODAYS_RESULT(8225AA88, TodaysResultLeave)
ITC_TODAYS_RESULT(8225A8A8, TodaysResultDestroy)
// Today's event's handlers (member function functors the machine calls; no
// direct callers). MarkImages sub_8225BAB8, LoadImages sub_8225C008,
// LoadImagePair sub_8225BD10, ImagesLoadedA sub_8225C0A0 and B sub_8225C0E8,
// SetUpTexts sub_8225C648, NextUnlockEntry sub_8225BB70 and FreeImages
// sub_8225BC60: no adapters (only the handlers above, native, call them, and
// the recompiled EventPage calls the original NextUnlockEntry).
ITC_TODAYS_RESULT(8225BE50, TodaysEventCreate)
ITC_TODAYS_RESULT(8225C130, TodaysEventOpenMovie)
ITC_TODAYS_RESULT(8225C160, TodaysEventWaitForMovie)
ITC_TODAYS_RESULT(8225C990, TodaysEventStart)
ITC_TODAYS_RESULT(8225C1F8, TodaysEventUnload)
ITC_TODAYS_RESULT(8225C278, TodaysEventUnloaded)
ITC_TODAYS_RESULT(8225C348, TodaysEventLeave)
ITC_TODAYS_RESULT(8225BF78, TodaysEventDestroy)
// Today's event's Draw and Update (functors), pages (from Draw's table) and
// transitions (from Update's table). QueueEventMovies sub_8225B9B8,
// ShowUnlock sub_8225CA50 and ShowImages sub_8225C848: no adapters (only
// EndPage, the transitions and ShowUnlock, native, call them).
ITC_TODAYS_RESULT(8225C928, TodaysEventDraw)
ITC_TODAYS_RESULT(8225CDE8, TodaysEventUpdate)
ITC_TODAYS_RESULT(8225C428, TodaysEventEventPage)
ITC_TODAYS_RESULT(8225C590, TodaysEventEndPage)
ITC_TODAYS_RESULT(8225CB18, TodaysEventIntro)
ITC_TODAYS_RESULT(8225C770, TodaysEventFadeOut)
ITC_TODAYS_RESULT(8225CC80, TodaysEventNextUnlock)
ITC_TODAYS_RESULT(8225CD50, TodaysEventNextPage)
