// The race HUD machine natively: adapters in front of src/race_hud_machine/
// (docs/functions/race-hud-machine.md), with the register steps the harness
// shares (hooks/race_hud_machine.h). The handlers' callees, the race HUD's
// and the alternate HUD's functions, run as their plain C++ (their guest
// calls through this context).

#include "hooks/race_hud_machine.h"

#include "alt_hud/alt_hud.h"
#include "base/flags.h"
#include "hooks/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_hud/race_hud.h"
#include "race_hud_machine/race_hud_machine.h"
#include "race_hud_object/race_hud_object.h"
#include "race_screens/race_screens.h"
#include "track_zones/track_zones.h"

#include <optional>

ITC_FLAG_BOOL(native_race_hud_machine, true, "ITC",
              "Run the race HUD machine's functions (docs/functions/race-hud-machine.md: "
              "RaceHudMachine::Init sub_8216E6E8 (the 65 transitions' words replayed in its "
              "frame, StateMachine::Add's plain C++), PreRaceUpdate sub_8216C210, PreRaceDraw sub_8216C2A0, "
              "CountdownDraw sub_8216C330, StartIntro sub_8216C510, EndIntro sub_8216C520, "
              "IntroUpdate sub_8216C530, IntroDraw sub_8216C5B8, CountdownUpdate sub_8216CB98, "
              "RaceDraw sub_8216CE00, RaceUpdate sub_8216D918, SetUp sub_8216D450 and "
              "CloseResults sub_8216D198, ResultsUpdate sub_8216DFD0, PauseUpdate sub_8216DB70 and "
              "AfterRaceUpdate sub_8216DD88, their callees the race HUD's, the alternate HUD's and "
              "the in-race screens' plain C++ (kind 4, and kind 0 "
              "where the original draws the holder's +12, never seen in play, run the "
              "originals; CloseResults' race kind 6 and ResultsUpdate's unseen results too); IsFirstDay sub_8216D3F0, OpponentPrize sub_8216C820 and ItemBonus "
              "sub_8216C938 with sub_8216C9B8 merged; the pause menu's openers OpenPause sub_8216D600, "
              "OpenRacePause sub_8216DA40 and OpenCareerPause sub_8216D8C8, PauseMenu::Open's "
              "plain C++, kind 4's and the stage's unseen openers the originals); CreateScreens "
              "sub_8216CA28 (Stages::Type's and RaceScreens::Create's plain C++, Create's with "
              "--native_heap), ClosePause sub_8216C658 (PauseMenu::Close's and ApplyOptions' "
              "plain C++), ResultsDraw sub_8216D6D0 (CareerResults::Draw's and RaceHud::Draw's "
              "plain C++; the unseen results' draws and kind 4 the original) and AfterRaceDraw "
              "sub_8216C6D8 (RaceHud::Draw's and RaceScreens::DrawAfterRaceMenu's plain C++; "
              "kinds 0 and 4 and kind 6's unseen screen the original) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=race_hud_machine)");
ITC_FLAG_DECLARE(bool, native_action_mappers);

namespace hooks {

namespace {

namespace rhm = race_hud_machine;
namespace rh = race_hud_object;

// The calls the plain C++ makes, through this context. `callees()`: the
// handlers' callees by address, run as their plain C++;
// `calls()`, `zone_calls()` and `sound_calls()`: the callees' own guest
// calls (r1 and lr set, the function run through the function table, its
// override when it has one, r1 put back), as the race HUD object's adapters
// make them. The adapter's lr is put back when it ends.
class Calls {
 public:
  Calls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~Calls() { ctx_.lr = lr_; }
  Calls(const Calls&) = delete;
  Calls& operator=(const Calls&) = delete;
  rh::Calls callees() { return {this, &Calls::Callee}; }
  rh::Calls calls() { return {this, &Calls::Guest}; }
  track_zones::OwnerCalls zone_calls() { return {this, &Calls::Zone}; }
  menu_sounds::Calls sound_calls() { return {this, &Calls::Sound}; }

 private:
  static uint64_t Callee(void* user, uint32_t function, const uint64_t args[8], const double*,
                         uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<Calls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint32_t r3 = uint32_t(args[0]);
    switch (function) {
      case rhm::kHudTick:
        return rh::Tick(base, r3, uint32_t(args[1]), r1, self.calls(), self.zone_calls());
      case rhm::kHudUpdateView:
        return rh::UpdateView(base, r3, r1);
      case rhm::kHudUpdateWarning:
        return rh::UpdateWarning(base, r3, r1, self.calls());
      case rhm::kHudUpdateItem:
        return rh::UpdateItem(base, r3, r1, self.calls());
      case rhm::kHudTickPanel:
        return rh::TickPanel(base, r3, r1, self.calls(), self.sound_calls());
      case rhm::kHudDraw:
        return rh::Draw(base, r3, r1, self.calls());
      case rhm::kHudRunWarning:
        return rh::RunWarning(base, r3, r1, self.calls(), self.sound_calls());
      case rhm::kHudRunItem:
        return rh::RunItem(base, r3, r1, self.calls());
      case rhm::kHudRunMovie:
        return self.RunHudMovie(args[0], args[1], r1, lr);
      case rhm::kHudRunCountdown:
        return rh::RunCountdown(base, r3, r1, self.calls(), self.sound_calls(), &ctx.f1.f64);
      case rhm::kHudStartMovie1:
        return rh::StartMovie1(base, r3, r1);
      case rhm::kHudFinishMovie1:
        return rh::FinishMovie1(base, r3, r1, lr, self.calls());
      case rhm::kHudRunMovie1:
        return rh::RunMovie1(base, r3, r1, lr, self.calls());
      case rhm::kHudSetPlayer:
        return rh::SetPlayer(base, r3, uint32_t(args[1]), r1);
      case rhm::kHudReset:
        return rh::Reset(base, r3, r1);
      case rhm::kHudClearWarning:
        rh::ClearWarning(base, r3);
        return args[0];  // r3 left
      case rhm::kHudSetUpGauges:
        return rh::SetUpGauges(base, r3, r1, self.calls());
      case rhm::kHudResetBar:
        rh::ResetBar(base, r3);
        return args[0];  // r3 left
      case rhm::kHudSetUpExtraGauges:
        return rh::SetUpExtraGauges(base, r3, r1);
      case rhm::kHudSetUpMirror:
        return rh::SetUpMirror(base, r3, r1, self.calls());
      case rhm::kHudApplyOptions:
        return rh::ApplyOptions(base, r3, r1);
      case rhm::kHudSetUpSigns:
        return rh::SetUpSigns(base, r3, r1, self.calls());
      case rhm::kAltSetUp:
        return alt_hud::SetUp(base, r3, uint32_t(args[1]), r1, self.calls(), &ctx.f1.f64);
      case rhm::kAltTick:
        return alt_hud::Tick(base, r3, r1, ITC_FLAG(native_action_mappers), self.calls(),
                             &ctx.f1.f64);
      case rhm::kAltRunMovie:
        return alt_hud::RunMovie(base, r1, self.calls());
      case rhm::kCareerResultsUpdate:
        // Its page's handler through the function table (r4 and r5 not read
        // by the handlers: 0).
        return race_screens::CareerResultsUpdate(base, args[0], 0, 0, r1, self.calls());
      case rhm::kPauseMenuUpdate:
        return race_screens::PauseMenuUpdate(base, args[0], args[1], r1, self.calls());
      case rhm::kAfterRaceUpdate:
        return race_screens::AfterRaceMenuUpdate(base, r1, self.calls());
      case rhm::kPauseMenuClose:
        return race_screens::PauseMenuClose(base, r1);
      case rhm::kCareerResultsDraw:
        return race_screens::CareerResultsDraw(base, r3, r1, self.calls());
      case rhm::kDrawAfterRaceMenu:
        return rh::DrawAfterRaceMenu(base, r3, r1, self.calls());
      case rhm::kScreensCreate:
        // As its adapter (src/hooks/race_hud_object.cpp): over the
        // allocation layer, else the guest call.
        if (NativeHeapActive()) {
          return rh::ScreensCreate(base, r3, r1, self.calls());
        }
        break;
      case rhm::kPauseMenuOpen:
        return race_screens::PauseMenuOpen(base, r3, args[1], args[2], r1, self.calls());
      case rhm::kAfterRaceClose:
        // As its adapter (src/hooks/race_screens.cpp): over the allocation
        // layer where the release is native, else the guest call.
        if (NativeHeapActive()) {
          if (const auto r3 = race_screens::AfterRaceMenuClose(base, r1)) {
            return *r3;
          }
        }
        break;
      default:
        break;
    }
    // Not one of the handlers' callees: the guest call.
    return Guest(user, function, args, nullptr, r1, lr);
  }

  // RaceHud::RunMovie (sub_822141E8) as its adapter (src/hooks/race_hud.cpp)
  // runs it: race_hud::RunMovie's choice, the movie's call a tail call from
  // the caller's frame with r4 passed on, state 1's from sub_82212908's
  // 112-byte frame. r3 left (the HUD) when it calls nothing.
  uint64_t RunHudMovie(uint64_t hud, uint64_t r4, uint32_t r1, uint32_t lr) {
    const race_hud::HudMovie movie = race_hud::RunMovie(base_, uint32_t(hud));
    const rh::Calls guest = calls();
    switch (movie.call) {
      case race_hud::HudCall::kNone:
        break;
      case race_hud::HudCall::kRun:
        return guest.Call(0x8211E1B8, r1, lr, movie.movie, r4);
      case race_hud::HudCall::kDraw:
        return guest.Call(0x8211D280, r1, lr, movie.movie, r4);
      case race_hud::HudCall::kState3:
        return guest.Call(0x82212958, r1, lr, hud, r4);
      case race_hud::HudCall::kRunTimer: {
        const uint32_t frame = r1 - 112;
        guest::StoreU32(base_, frame, r1);
        return guest.Call(0x8211E1B8, frame, 0x8221293C, movie.movie, r4);
      }
    }
    return hud;
  }

  static uint64_t Guest(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                        uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<Calls*>(user);
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

  static uint64_t Zone(void* user, uint32_t function, uint32_t r3, uint32_t r4, bool set_r4,
                       uint32_t frame, uint32_t lr) {
    auto& self = *static_cast<Calls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    ctx.r3.u64 = r3;
    if (set_r4) {
      ctx.r4.u64 = r4;
    }
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    return ctx.r3.u64;
  }

  static uint64_t Sound(void* user, uint32_t function, const uint64_t args[4], int set,
                        uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<Calls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    PPCRegister* registers[4] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6};
    for (int i = 0; i < set; i++) {
      registers[i]->u64 = args[i];
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

// A handler: its plain C++ with the machine's register, or the original
// where it answers nullopt.
template <typename Handler>
void Handle(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
            Handler handler) {
  ctx.fpscr.disableFlushMode();
  std::optional<uint64_t> r3;
  {
    Calls calls(ctx, base);
    r3 = handler(calls.callees());
  }
  if (!r3) {
    original(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

}  // namespace

void RaceHudMachineInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = rhm::MachineInit(base, machine, ctx.r1.u32 - rhm::kInitFrame, &replaced);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

void RaceHudMachinePreRaceUpdate(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C210, [&](const rh::Calls& callees) {
    return rhm::PreRaceUpdate(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachinePreRaceDraw(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C2A0, [&](const rh::Calls& callees) {
    return rhm::PreRaceDraw(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineCountdownDraw(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C330, [&](const rh::Calls& callees) {
    return rhm::CountdownDraw(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineStartIntro(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  Handle(ctx, base, __imp__sub_8216C510, [&](const rh::Calls& callees) {
    return std::optional<uint64_t>(rhm::StartIntro(base, ctx.r3.u32, ctx.r1.u32, lr, callees));
  });
}

void RaceHudMachineEndIntro(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  Handle(ctx, base, __imp__sub_8216C520, [&](const rh::Calls& callees) {
    return std::optional<uint64_t>(rhm::EndIntro(base, ctx.r3.u32, ctx.r1.u32, lr, callees));
  });
}

void RaceHudMachineIntroUpdate(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C530, [&](const rh::Calls& callees) {
    return rhm::IntroUpdate(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineIntroDraw(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C5B8, [&](const rh::Calls& callees) {
    return rhm::IntroDraw(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineCountdownUpdate(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216CB98, [&](const rh::Calls& callees) {
    return rhm::CountdownUpdate(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineRaceDraw(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216CE00, [&](const rh::Calls& callees) {
    return rhm::RaceDraw(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineRaceUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t dispatcher = ctx.r4.u32;
  Handle(ctx, base, __imp__sub_8216D918, [&](const rh::Calls& callees) {
    return rhm::RaceUpdate(base, ctx.r3.u64, dispatcher, ctx.r1.u32, callees);
  });
}

void RaceHudMachineSetUp(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216D450, [&](const rh::Calls& callees) {
    return rhm::SetUp(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineResultsUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t dispatcher = ctx.r4.u32;
  Handle(ctx, base, __imp__sub_8216DFD0, [&](const rh::Calls& callees) {
    return rhm::ResultsUpdate(base, ctx.r3.u64, dispatcher, ctx.r1.u32, callees);
  });
}

void RaceHudMachinePauseUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t dispatcher = ctx.r4.u32;
  Handle(ctx, base, __imp__sub_8216DB70, [&](const rh::Calls& callees) {
    return rhm::PauseUpdate(base, ctx.r3.u64, dispatcher, ctx.r1.u32, callees);
  });
}

void RaceHudMachineAfterRaceUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t dispatcher = ctx.r4.u32;
  Handle(ctx, base, __imp__sub_8216DD88, [&](const rh::Calls& callees) {
    return rhm::AfterRaceUpdate(base, ctx.r3.u64, dispatcher, ctx.r1.u32, callees);
  });
}

void RaceHudMachineCloseResults(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216D198, [&](const rh::Calls& callees) {
    return rhm::CloseResults(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineCreateScreens(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216CA28, [&](const rh::Calls& callees) {
    return std::optional<uint64_t>(rhm::CreateScreens(base, ctx.r3.u32, ctx.r1.u32, callees));
  });
}

void RaceHudMachineClosePause(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C658, [&](const rh::Calls& callees) {
    return std::optional<uint64_t>(rhm::ClosePause(base, ctx.r3.u64, ctx.r1.u32, callees));
  });
}

void RaceHudMachineResultsDraw(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216D6D0, [&](const rh::Calls& callees) {
    return rhm::ResultsDraw(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineAfterRaceDraw(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216C6D8, [&](const rh::Calls& callees) {
    return rhm::AfterRaceDraw(base, ctx.r3.u64, ctx.r1.u32, callees);
  });
}

void RaceHudMachineIsFirstDay(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rhm::IsFirstDay(base, ctx.r1.u32);
}

void RaceHudMachineOpponentPrize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rhm::OpponentPrize(base, ctx.r4.u32, ctx.r1.u32);
}

void RaceHudMachineItemBonus(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rhm::ItemBonus(base, ctx.r4.u32, ctx.r1.u32);
}

void RaceHudMachineOpenPause(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  Handle(ctx, base, __imp__sub_8216D600, [&](const rh::Calls& callees) {
    return rhm::OpenPause(base, ctx.r3.u64, ctx.r1.u32, lr, callees);
  });
}

void RaceHudMachineOpenRacePause(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  Handle(ctx, base, __imp__sub_8216DA40, [&](const rh::Calls& callees) {
    return rhm::OpenRacePause(base, ctx.r3.u64, ctx.r1.u32, lr, callees);
  });
}

void RaceHudMachineOpenCareerPause(PPCContext& ctx, uint8_t* base) {
  Handle(ctx, base, __imp__sub_8216D8C8, [&](const rh::Calls& callees) {
    return std::optional<uint64_t>(rhm::OpenCareerPause(base, ctx.r3.u32, ctx.r1.u32, callees));
  });
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. The handlers are reached through their functors (no direct callers),
// so their adapters stay.
#define ITC_RACE_HUD_MACHINE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {         \
    REX_FUNC_PROLOGUE();                       \
    if (!ITC_FLAG(native_race_hud_machine)) {  \
      __imp__sub_##address(ctx, base);         \
      return;                                  \
    }                                          \
    hooks::step(ctx, base);                    \
  }

ITC_RACE_HUD_MACHINE(8216E6E8, RaceHudMachineInit)
ITC_RACE_HUD_MACHINE(8216C210, RaceHudMachinePreRaceUpdate)
ITC_RACE_HUD_MACHINE(8216C2A0, RaceHudMachinePreRaceDraw)
ITC_RACE_HUD_MACHINE(8216C330, RaceHudMachineCountdownDraw)
ITC_RACE_HUD_MACHINE(8216C510, RaceHudMachineStartIntro)
ITC_RACE_HUD_MACHINE(8216C520, RaceHudMachineEndIntro)
ITC_RACE_HUD_MACHINE(8216C530, RaceHudMachineIntroUpdate)
ITC_RACE_HUD_MACHINE(8216C5B8, RaceHudMachineIntroDraw)
ITC_RACE_HUD_MACHINE(8216CB98, RaceHudMachineCountdownUpdate)
ITC_RACE_HUD_MACHINE(8216CE00, RaceHudMachineRaceDraw)
ITC_RACE_HUD_MACHINE(8216D918, RaceHudMachineRaceUpdate)
ITC_RACE_HUD_MACHINE(8216D450, RaceHudMachineSetUp)
ITC_RACE_HUD_MACHINE(8216D198, RaceHudMachineCloseResults)
ITC_RACE_HUD_MACHINE(8216DFD0, RaceHudMachineResultsUpdate)
ITC_RACE_HUD_MACHINE(8216DB70, RaceHudMachinePauseUpdate)
ITC_RACE_HUD_MACHINE(8216DD88, RaceHudMachineAfterRaceUpdate)
ITC_RACE_HUD_MACHINE(8216D3F0, RaceHudMachineIsFirstDay)
ITC_RACE_HUD_MACHINE(8216C820, RaceHudMachineOpponentPrize)
ITC_RACE_HUD_MACHINE(8216C938, RaceHudMachineItemBonus)
ITC_RACE_HUD_MACHINE(8216D600, RaceHudMachineOpenPause)
ITC_RACE_HUD_MACHINE(8216DA40, RaceHudMachineOpenRacePause)
ITC_RACE_HUD_MACHINE(8216CA28, RaceHudMachineCreateScreens)
ITC_RACE_HUD_MACHINE(8216C658, RaceHudMachineClosePause)
ITC_RACE_HUD_MACHINE(8216D6D0, RaceHudMachineResultsDraw)
ITC_RACE_HUD_MACHINE(8216C6D8, RaceHudMachineAfterRaceDraw)
