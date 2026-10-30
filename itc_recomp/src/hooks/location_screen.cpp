// The career's location screen natively: adapters in front of
// src/location_screen/ (docs/functions/location-screen.md), with the
// register steps the harness shares (hooks/location_screen.h).

#include "hooks/location_screen.h"

#include "base/flags.h"
#include "hooks/career.h"
#include "hooks/heap.h"
#include "hooks/guest_call.h"
#include "hooks/race_event.h"
#include "hooks/race_setup.h"
#include "hooks/strings_and_font.h"
#include "location_screen/location_screen.h"
#include "memory/c_runtime_heap.h"

ITC_FLAG_DECLARE(bool, native_ui_movies_game);
ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

ITC_FLAG_BOOL(native_location_screen, true, "ITC",
              "Run the career's location screen (docs/functions/location-screen.md: "
              "LocationScreen::SceneryConfig sub_8228F0A0, KindText sub_82227690, "
              "IsPictureQueued sub_822291E8, PictureA sub_82229108, PictureB sub_82229158, "
              "OnMessage14 sub_82227158, HasMoreUnlocks sub_822281D8, NextUnlockEntry "
              "sub_82227618, OpponentHasCard sub_82227708, NextResultText sub_822271F8, "
              "ShowPictures sub_8222A060, HidePictures sub_8222A150, SetCarColor sub_8222AC18, "
              "SetCursor sub_8222BFE8, ListNode sub_8222C058, ShowArrows sub_8222C0C8, "
              "RunCardMovie sub_822291A8, Transition1 sub_8222AD50, Transition18 sub_8222BE60; "
              "UiMovie::Run through its override; ScreenMachine::Reenter sub_822895E0, Reset "
              "sub_822279A8 with ResetView sub_82227818 (no adapter: only Reset's plain C++ calls it), "
              "ApplyQueuedPage sub_8222C9B0, "
              "UpdatePage sub_8222EC08, its page's handler through the guest context; InitStates "
              "sub_8222F258 over StateMachine::Add's plain C++; Page9 sub_822280D8 and Page11 "
              "sub_82228138 over the career's and the race set-up's register steps, "
              "NextOpponentText sub_82227430, LoadPicture sub_8222C270, Picture::Free "
              "sub_82228438 and FreePictures sub_82228498, their heap and archive calls "
              "through those adapters; with the UI movies' heap native (their text setters' "
              "plain C++), ResetMovie sub_8222CD60, Transition29 sub_8222CFF0, ShowUnlockEntry "
              "sub_8222A978, Transition25 sub_8222D1A8, ShowListEntry sub_8222B048, "
              "Transition2 sub_8222D610, Transition14 sub_8222E6E0 and Transition16 "
              "sub_8222E8D8, Transition23 sub_8222A470, Transition33 sub_8222A718 and "
              "Transition7 sub_8222AE00, UiMovie::Finish, FinishNamed, FinishAll and Run through "
              "their overrides; ShowOpponentPictures sub_8222B200 and UpdateView sub_82227E20; "
              "with the texts' check ShowOpponentInfo sub_8222B438, Transition12 sub_8222B8A8, "
              "ShowCard sub_8222BBD0 and Transition17 sub_8222EAD0; Transition4 sub_8222D828 and "
              "Transition5 sub_8222DA10 over CameraAnims::Evaluate's plain C++, Transition8 "
              "sub_8222DC08, Transition24 sub_8222D2E8, Transition34 sub_8222D108 and Start "
              "sub_8222F0B0) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=location_screen). PictureA, PictureB, Picture::Free, KindText, "
              "SetCursor, ShowUnlockEntry, ShowCard, ListNode and ShowListEntry have no "
              "adapters: only native callers reach them, and the flag switches those");

namespace hooks {

namespace ls = location_screen;

void LocationScreenSceneryConfig(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = ls::SceneryConfig(ctx.r3.u64, ctx.r4.u64);
}

void LocationScreenKindText(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = ls::KindText(ctx.r3.u32, ctx.r4.u64);
}

void LocationScreenIsPictureQueued(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::IsPictureQueued(base, ctx.r3.u32);
}

void LocationScreenPictureA(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::PictureA(base, ctx.r3.u32);
}

void LocationScreenPictureB(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::PictureB(base, ctx.r3.u32);
}

void LocationScreenOnMessage14(PPCContext&, uint8_t* base) { ls::OnMessage14(base); }

void LocationScreenHasMoreUnlocks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::HasMoreUnlocks(base);
}

void LocationScreenNextUnlockEntry(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::NextUnlockEntry(base);
}

void LocationScreenOpponentHasCard(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = ls::OpponentHasCard(base, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_82227708(ctx, base);
}

void LocationScreenNextResultText(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = ls::NextResultText(base, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_822271F8(ctx, base);
}

void LocationScreenShowPictures(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::ShowPictures(base, ctx.r1.u32);
}

void LocationScreenHidePictures(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::HidePictures(base, ctx.r1.u32);
}

void LocationScreenSetCarColor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::SetCarColor(base, ctx.r1.u32);
}

void LocationScreenSetCursor(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ls::SetCursor(base, ctx.r1.u32, ctx.r3.u32);
}

void LocationScreenListNode(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ls::ListNode(base, ctx.r1.u32, ctx.r3.u32);
}

void LocationScreenShowArrows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ls::ShowArrows(base, ctx.r1.u32);
}

namespace {

struct GuestRun {
  PPCContext& ctx;
  uint8_t* base;
};

uint64_t RunThrough(void* user, uint32_t movie, uint32_t draw_only, uint32_t frame_bytes,
                    uint32_t lr) {
  auto& guest = *static_cast<GuestRun*>(user);
  PPCContext& ctx = guest.ctx;
  guest_call::InFrame(ctx, guest.base, frame_bytes, lr, [&](uint32_t) {
    ctx.r3.u64 = movie;
    ctx.r4.u64 = draw_only;
    sub_8211E1B8(ctx, guest.base);
  });
  return ctx.r3.u64;
}

// Runs `step(run)` with UiMovie::Run through this context; lr put back.
template <typename Step>
uint64_t WithRun(PPCContext& ctx, uint8_t* base, Step step) {
  const uint64_t lr = ctx.lr;
  GuestRun guest{ctx, base};
  const ls::RunCall run{&guest, RunThrough};
  const uint64_t r3 = step(run);
  ctx.lr = lr;
  return r3;
}

}  // namespace

void LocationScreenRunCardMovie(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = WithRun(ctx, base, [&](const ls::RunCall& run) {
    return ls::RunCardMovie(base, sp, run);
  });
}

void LocationScreenTransition1(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, step = ctx.r3.u32;
  ctx.r3.u64 = WithRun(ctx, base, [&](const ls::RunCall& run) -> uint64_t {
    return ls::Transition1(base, sp, step, run);
  });
}

void LocationScreenTransition18(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, step = ctx.r3.u32;
  ctx.r3.u64 = WithRun(ctx, base, [&](const ls::RunCall& run) -> uint64_t {
    return ls::Transition18(base, sp, step, run);
  });
}

void LocationScreenReenter(PPCContext& ctx, uint8_t* base) { ls::Reenter(base, ctx.r3.u32); }

void LocationScreenReset(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32;
  if (!ls::ResetNative(screen)) {
    __imp__sub_822279A8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ls::Reset(base, screen);
  ctx.r3.u64 = screen + 560 + 176;
}

void LocationScreenResetView(PPCContext& ctx, uint8_t* base) {
  const uint32_t view = ctx.r3.u32;
  if (!ls::ResetNative(view - 560)) {
    __imp__sub_82227818(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ls::ResetView(base, view);
  ctx.r3.u64 = view + 176;
}

void LocationScreenApplyQueuedPage(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = ls::ApplyQueuedPage(base, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
  }
}

namespace {

void PageThrough(void* user, uint32_t handler, uint32_t dispatcher, uint32_t machine,
                 uint32_t frame_bytes, uint32_t lr) {
  auto& guest = *static_cast<GuestRun*>(user);
  PPCContext& ctx = guest.ctx;
  uint8_t* base = guest.base;
  guest_call::InFrame(ctx, base, frame_bytes, lr, [&](uint32_t) {
    ctx.r3.u64 = dispatcher;
    ctx.r4.u64 = machine;
    ctx.ctr.u64 = handler;
    REX_CALL_INDIRECT_FUNC(handler);
  });
}

}  // namespace

void LocationScreenUpdatePage(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  GuestRun guest{ctx, base};
  const ls::PageCall call{&guest, PageThrough};
  ls::UpdatePage(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, call);
  ctx.lr = lr;
}

void LocationScreenInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 =
      ls::InitStates(base, machine, ctx.r1.u32 - ls::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}


// --- The second migration's guest calls ---

LocationScreenCalls::LocationScreenCalls(PPCContext& ctx, uint8_t* base)
    : ctx_(ctx),
      base_(base),
      lr_(ctx.lr),
      event_(std::make_unique<RaceEventGuest>(ctx, base)),
      format_(std::make_unique<FormatGuestCalls>(ctx, base)),
      calls_{this, &LocationScreenCalls::Call, ctx.r1.u32, &event_->get(), &format_->get()} {}

LocationScreenCalls::~LocationScreenCalls() { ctx_.lr = lr_; }

uint64_t LocationScreenCalls::Call(void* user, uint32_t function, const uint64_t args[4],
                                   uint32_t r1, uint32_t lr) {
  auto& self = *static_cast<LocationScreenCalls*>(user);
  PPCContext& ctx = self.ctx_;
  uint8_t* base = self.base_;
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = r1;
  ctx.r3.u64 = args[0];
  ctx.r4.u64 = args[1];
  ctx.r5.u64 = args[2];
  ctx.r6.u64 = args[3];
  ctx.lr = lr;
  switch (function) {
    case ls::kEventQuery:
      CareerEventQuery(ctx, base);
      break;
    case ls::kRaceSetUp:
      RaceSetupSetUp(ctx, base);
      break;
    case ls::kArchiveLoad:
      ctx.r7.u64 = 0;
      ctx.r8.u64 = 0;
      [[fallthrough]];
    default:
      ctx.ctr.u64 = function;
      REX_CALL_INDIRECT_FUNC(function);
      break;
  }
  ctx.r1.u64 = stack;
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}

void LocationScreenPage9(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, machine = ctx.r4.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::Page9(base, sp, machine, calls.get());
}

void LocationScreenPage11(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, machine = ctx.r4.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::Page11(base, sp, machine, calls.get());
}

void LocationScreenNextOpponentText(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::NextOpponentText(base, sp, calls.get());
}

void LocationScreenLoadPicture(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, screen = ctx.r3.u32, id = ctx.r4.u32, list = ctx.r5.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::LoadPicture(base, sp, screen, id, list, calls.get());
}

void LocationScreenPictureFree(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, picture = ctx.r3.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::PictureFree(base, sp, picture, calls.get());
}

void LocationScreenFreePictures(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32, screen = ctx.r3.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::FreePictures(base, sp, screen, calls.get());
}

bool LocationScreenTextsNative(uint8_t* base) {
  return ITC_FLAG(native_ui_movies_game) && ITC_FLAG(native_c_runtime_heap) &&
         NativeHeapActive() && memory::crt::GetProcessHeap(base) != 0;
}

namespace {

// The texts' functions: natively while the UI movies' text setters are, else
// the original; r3 the plain C++'s.
template <typename Step>
void Texts(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*), Step step) {
  if (!LocationScreenTextsNative(base)) {
    original(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t sp = ctx.r1.u32, r3 = ctx.r3.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = step(sp, r3, calls.get());
}

}  // namespace

void LocationScreenResetMovie(PPCContext& ctx, uint8_t* base) {
  Texts(ctx, base, __imp__sub_8222CD60, [&](uint32_t sp, uint32_t, const ls::Calls& calls) {
    return ls::ResetMovie(base, sp, calls);
  });
}

void LocationScreenShowUnlockEntry(PPCContext& ctx, uint8_t* base) {
  Texts(ctx, base, __imp__sub_8222A978, [&](uint32_t sp, uint32_t, const ls::Calls& calls) {
    return ls::ShowUnlockEntry(base, sp, calls);
  });
}

void LocationScreenShowListEntry(PPCContext& ctx, uint8_t* base) {
  Texts(ctx, base, __imp__sub_8222B048, [&](uint32_t sp, uint32_t, const ls::Calls& calls) {
    return ls::ShowListEntry(base, sp, calls);
  });
}

#define LOCATION_SCREEN_TRANSITION(address, name)                                     \
  void LocationScreen##name(PPCContext& ctx, uint8_t* base) {                         \
    Texts(ctx, base, __imp__sub_##address,                                            \
          [&](uint32_t sp, uint32_t step, const ls::Calls& calls) {                   \
            return ls::name(base, sp, step, calls);                                   \
          });                                                                         \
  }
LOCATION_SCREEN_TRANSITION(8222CFF0, Transition29)
LOCATION_SCREEN_TRANSITION(8222D1A8, Transition25)
LOCATION_SCREEN_TRANSITION(8222D610, Transition2)
LOCATION_SCREEN_TRANSITION(8222E6E0, Transition14)
LOCATION_SCREEN_TRANSITION(8222E8D8, Transition16)
LOCATION_SCREEN_TRANSITION(8222A470, Transition23)
LOCATION_SCREEN_TRANSITION(8222A718, Transition33)
LOCATION_SCREEN_TRANSITION(8222AE00, Transition7)
LOCATION_SCREEN_TRANSITION(8222B8A8, Transition12)
LOCATION_SCREEN_TRANSITION(8222EAD0, Transition17)
LOCATION_SCREEN_TRANSITION(8222D828, Transition4)
LOCATION_SCREEN_TRANSITION(8222DA10, Transition5)
LOCATION_SCREEN_TRANSITION(8222DC08, Transition8)
LOCATION_SCREEN_TRANSITION(8222D2E8, Transition24)
LOCATION_SCREEN_TRANSITION(8222D108, Transition34)
#undef LOCATION_SCREEN_TRANSITION

void LocationScreenShowOpponentInfo(PPCContext& ctx, uint8_t* base) {
  Texts(ctx, base, __imp__sub_8222B438, [&](uint32_t sp, uint32_t, const ls::Calls& calls) {
    return ls::ShowOpponentInfo(base, sp, calls);
  });
}

void LocationScreenStart(PPCContext& ctx, uint8_t* base) {
  Texts(ctx, base, __imp__sub_8222F0B0, [&](uint32_t sp, uint32_t, const ls::Calls& calls) {
    return ls::Start(base, sp, calls);
  });
}

void LocationScreenShowCard(PPCContext& ctx, uint8_t* base) {
  if (!LocationScreenTextsNative(base)) {
    __imp__sub_8222BBD0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t sp = ctx.r1.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::ShowCard(base, sp, calls.get());
}

void LocationScreenShowOpponentPictures(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::ShowOpponentPictures(base, sp, calls.get());
}

void LocationScreenUpdateView(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t sp = ctx.r1.u32;
  LocationScreenCalls calls(ctx, base);
  ctx.r3.u64 = ls::UpdateView(base, sp, calls.get());
}

}  // namespace hooks

#define LOCATION_SCREEN_ADAPTER(address, step)  \
  extern "C" REX_FUNC(sub_##address) {          \
    REX_FUNC_PROLOGUE();                        \
    if (!ITC_FLAG(native_location_screen)) {    \
      __imp__sub_##address(ctx, base);          \
      return;                                   \
    }                                           \
    hooks::step(ctx, base);                     \
  }

LOCATION_SCREEN_ADAPTER(8228F0A0, LocationScreenSceneryConfig)
LOCATION_SCREEN_ADAPTER(822291E8, LocationScreenIsPictureQueued)
LOCATION_SCREEN_ADAPTER(82227158, LocationScreenOnMessage14)
LOCATION_SCREEN_ADAPTER(822281D8, LocationScreenHasMoreUnlocks)
LOCATION_SCREEN_ADAPTER(82227618, LocationScreenNextUnlockEntry)
LOCATION_SCREEN_ADAPTER(82227708, LocationScreenOpponentHasCard)
LOCATION_SCREEN_ADAPTER(822271F8, LocationScreenNextResultText)
LOCATION_SCREEN_ADAPTER(8222A060, LocationScreenShowPictures)
LOCATION_SCREEN_ADAPTER(8222A150, LocationScreenHidePictures)
LOCATION_SCREEN_ADAPTER(8222AC18, LocationScreenSetCarColor)
LOCATION_SCREEN_ADAPTER(8222C0C8, LocationScreenShowArrows)
LOCATION_SCREEN_ADAPTER(822291A8, LocationScreenRunCardMovie)
LOCATION_SCREEN_ADAPTER(8222AD50, LocationScreenTransition1)
LOCATION_SCREEN_ADAPTER(8222BE60, LocationScreenTransition18)
LOCATION_SCREEN_ADAPTER(822895E0, LocationScreenReenter)
LOCATION_SCREEN_ADAPTER(822279A8, LocationScreenReset)
LOCATION_SCREEN_ADAPTER(8222C9B0, LocationScreenApplyQueuedPage)
LOCATION_SCREEN_ADAPTER(8222EC08, LocationScreenUpdatePage)
LOCATION_SCREEN_ADAPTER(8222F258, LocationScreenInitStates)
// No adapters (only native callers reach them in play, and the flag switches
// those; their steps above are the harness's): PictureA sub_82229108 and
// PictureB sub_82229158, Picture::Free sub_82228438, KindText sub_82227690,
// SetCursor sub_8222BFE8, ShowUnlockEntry sub_8222A978, ShowCard
// sub_8222BBD0, ListNode sub_8222C058, ShowListEntry sub_8222B048.
LOCATION_SCREEN_ADAPTER(822280D8, LocationScreenPage9)
LOCATION_SCREEN_ADAPTER(82228138, LocationScreenPage11)
LOCATION_SCREEN_ADAPTER(82227430, LocationScreenNextOpponentText)
LOCATION_SCREEN_ADAPTER(8222C270, LocationScreenLoadPicture)
LOCATION_SCREEN_ADAPTER(82228498, LocationScreenFreePictures)
LOCATION_SCREEN_ADAPTER(8222CD60, LocationScreenResetMovie)
LOCATION_SCREEN_ADAPTER(8222CFF0, LocationScreenTransition29)
LOCATION_SCREEN_ADAPTER(8222D1A8, LocationScreenTransition25)
LOCATION_SCREEN_ADAPTER(8222D610, LocationScreenTransition2)
LOCATION_SCREEN_ADAPTER(8222E6E0, LocationScreenTransition14)
LOCATION_SCREEN_ADAPTER(8222E8D8, LocationScreenTransition16)
LOCATION_SCREEN_ADAPTER(8222A470, LocationScreenTransition23)
LOCATION_SCREEN_ADAPTER(8222A718, LocationScreenTransition33)
LOCATION_SCREEN_ADAPTER(8222AE00, LocationScreenTransition7)
LOCATION_SCREEN_ADAPTER(8222B200, LocationScreenShowOpponentPictures)
LOCATION_SCREEN_ADAPTER(82227E20, LocationScreenUpdateView)
LOCATION_SCREEN_ADAPTER(8222B438, LocationScreenShowOpponentInfo)
LOCATION_SCREEN_ADAPTER(8222B8A8, LocationScreenTransition12)
LOCATION_SCREEN_ADAPTER(8222EAD0, LocationScreenTransition17)
LOCATION_SCREEN_ADAPTER(8222D828, LocationScreenTransition4)
LOCATION_SCREEN_ADAPTER(8222DA10, LocationScreenTransition5)
LOCATION_SCREEN_ADAPTER(8222DC08, LocationScreenTransition8)
LOCATION_SCREEN_ADAPTER(8222D2E8, LocationScreenTransition24)
LOCATION_SCREEN_ADAPTER(8222D108, LocationScreenTransition34)
LOCATION_SCREEN_ADAPTER(8222F0B0, LocationScreenStart)
