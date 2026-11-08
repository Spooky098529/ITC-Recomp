// The race HUD machine's handlers and helpers as plain C++
// (docs/functions/race-hud-machine.md; see race_hud_machine.h). Each
// follows its original's loads (the holder read again from the machine
// before each call where the original reads it again), calls (their return
// addresses the originals') and stores in order.

#include "race_hud_machine/race_hud_machine.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_states/steps.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_hud_machine {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

// The original's frame: its back chain at sp - bytes. The frame.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Holder(uint8_t* base, uint32_t machine) { return Load(base, machine + kHolder); }

bool ReplayBit(uint8_t* base, uint32_t holder) {
  return (Load(base, holder + kHolderFlags) & 1) != 0;
}

// The race data ([0x826C5B2C]).
constexpr uint32_t kRaceData = 0x826C5B2C;
// The racers' manager ([0x826D82F4]: +2308 the player's entry, its +24 the
// racer record's index).
constexpr uint32_t kRacers = 0x826D82F4;

}  // namespace

std::optional<uint64_t> PreRaceUpdate(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216C210 RaceHudMachine::PreRaceUpdate");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind <= 0 || (kind > 3 && kind != 6)) {
    return r3;
  }
  callees.Call(kHudTick, frame, 0x8216C258, Holder(base, machine) + kHud, 0);
  callees.Call(kHudUpdateView, frame, 0x8216C264, Holder(base, machine) + kHud);
  callees.Call(kHudUpdateWarning, frame, 0x8216C270, Holder(base, machine) + kHud);
  return callees.Call(kHudUpdateItem, frame, 0x8216C27C, Holder(base, machine) + kHud);
}

std::optional<uint64_t> PreRaceDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                    const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t kind = Load(base, machine + kKind);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216C2A0 RaceHudMachine::PreRaceDraw");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind > 3 && kind != 6) {
    return r3;
  }
  callees.Call(kHudDraw, frame, 0x8216C2E0, Holder(base, machine) + kHud);
  callees.Call(kHudRunWarning, frame, 0x8216C2EC, Holder(base, machine) + kHud);
  return callees.Call(kHudRunItem, frame, 0x8216C2F8, Holder(base, machine) + kHud);
}

std::optional<uint64_t> CountdownDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t kind = Load(base, machine + kKind);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216C330 RaceHudMachine::CountdownDraw");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind == 5) {
    return callees.Call(kAltRunMovie, frame, 0x8216C3A0, Holder(base, machine) + kAltHud);
  }
  if (kind > 6) {
    return r3;
  }
  const uint32_t hud = Holder(base, machine) + kHud;
  callees.Call(kHudDraw, frame, 0x8216C374, hud);
  callees.Call(kHudRunMovie, frame, 0x8216C380, hud, 0);
  callees.Call(kHudRunCountdown, frame, 0x8216C388, hud);
  return callees.Call(kHudRunWarning, frame, 0x8216C390, hud);
}

uint64_t StartIntro(uint8_t* base, uint32_t machine, uint32_t sp, uint32_t lr,
                    const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216C510 RaceHudMachine::StartIntro");
  return callees.Call(kHudStartMovie1, sp, lr, Holder(base, machine) + kHud);
}

uint64_t EndIntro(uint8_t* base, uint32_t machine, uint32_t sp, uint32_t lr,
                  const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216C520 RaceHudMachine::EndIntro");
  return callees.Call(kHudFinishMovie1, sp, lr, Holder(base, machine) + kHud);
}

std::optional<uint64_t> IntroUpdate(uint8_t* base, uint64_t r3, uint32_t sp,
                                    const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216C530 RaceHudMachine::IntroUpdate");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind <= 0 || (kind > 3 && kind != 6)) {
    return r3;
  }
  callees.Call(kHudTick, frame, 0x8216C578, Holder(base, machine) + kHud, 0);
  callees.Call(kHudUpdateView, frame, 0x8216C584, Holder(base, machine) + kHud);
  return callees.Call(kHudUpdateWarning, frame, 0x8216C590, Holder(base, machine) + kHud);
}

std::optional<uint64_t> IntroDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                  const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t kind = Load(base, machine + kKind);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216C5B8 RaceHudMachine::IntroDraw");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind > 3 && kind != 6) {
    return r3;
  }
  callees.Call(kHudRunMovie1, frame, 0x8216C5F8, Holder(base, machine) + kHud);
  callees.Call(kHudDraw, frame, 0x8216C604, Holder(base, machine) + kHud);
  return callees.Call(kHudRunWarning, frame, 0x8216C610, Holder(base, machine) + kHud);
}

std::optional<uint64_t> CountdownUpdate(uint8_t* base, uint64_t r3, uint32_t sp,
                                        const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216CB98 RaceHudMachine::CountdownUpdate");
  const uint32_t frame = Frame(base, sp, 112);
  if (kind == 5) {
    guest::StoreU32(base, machine + kState, 7);
    return r3;
  }
  if (kind <= 0 || (kind > 3 && kind != 6)) {
    return r3;
  }
  callees.Call(kHudTick, frame, 0x8216CBEC, Holder(base, machine) + kHud, 1);
  callees.Call(kHudUpdateWarning, frame, 0x8216CBF8, Holder(base, machine) + kHud);
  callees.Call(kHudTickPanel, frame, 0x8216CC04, Holder(base, machine) + kHud);
  const uint64_t last =
      callees.Call(kHudUpdateView, frame, 0x8216CC10, Holder(base, machine) + kHud);
  if (Load(base, Holder(base, machine) + kCountdownRunning) == 0) {
    guest::StoreU32(base, machine + kState, 7);
  }
  return last;
}

std::optional<uint64_t> RaceDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                 const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t kind = Load(base, machine + kKind);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216CE00 RaceHudMachine::RaceDraw");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind > 6) {
    return r3;
  }
  const uint32_t holder = Holder(base, machine);
  if (kind == 5 || ReplayBit(base, holder)) {
    return callees.Call(kAltRunMovie, frame, 0x8216CE74, holder + kAltHud);
  }
  const uint32_t hud = holder + kHud;
  callees.Call(kHudDraw, frame, 0x8216CE50, hud);
  callees.Call(kHudRunWarning, frame, 0x8216CE58, hud);
  return callees.Call(kHudRunMovie, frame, 0x8216CE64, hud, 0);
}

std::optional<uint64_t> RaceUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                   const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216D918 RaceHudMachine::RaceUpdate");
  const uint32_t frame = Frame(base, sp, 112);
  if (kind <= 0 || (kind > 3 && kind != 5 && kind != 6)) {
    return r3;
  }
  const uint32_t holder = Holder(base, machine);
  if (kind != 5 && !ReplayBit(base, holder)) {
    const uint32_t hud = holder + kHud;
    callees.Call(kHudTick, frame, 0x8216D980, hud, 1);
    callees.Call(kHudUpdateView, frame, 0x8216D988, hud);
    callees.Call(kHudUpdateWarning, frame, 0x8216D990, hud);
    return callees.Call(kHudTickPanel, frame, 0x8216D998, hud);
  }
  const uint64_t answer = callees.Call(kAltTick, frame, 0x8216D9A8, holder + kAltHud);
  if (uint32_t(answer) != 0) {
    race_states::steps::Post(base, dispatcher, 20);
  }
  return answer;
}

std::optional<uint64_t> ResultsUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                      const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t race = Load(base, kRaceData);
  const int32_t race_kind = int32_t(Load(base, race + 0xA3AF0));
  const uint32_t kind = Load(base, machine + kKind);
  // Kind 4's sub_82216850 and the results of race kinds 2, 4 and 6 and of
  // kind 1 outside the career (sub_8221F930, sub_8221FFF0) are never seen
  // in play: the original runs.
  if (kind == 4 || race_kind == 2 || race_kind == 4 || race_kind == 6 ||
      (race_kind == 1 && int32_t(Load(base, race + 0xA3AF4)) == 0)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216DFD0 RaceHudMachine::ResultsUpdate");
  const uint32_t frame = Frame(base, sp, 144);
  vehicle_accessors::RacerRecordsAt(base, race + 0x59AE8, 0xFFFFFFFFu);
  if (kind < 4 || kind == 6) {
    const uint32_t hud = Holder(base, machine) + kHud;
    callees.Call(kHudTick, frame, 0x8216E038, hud, 0);
    callees.Call(kHudUpdateWarning, frame, 0x8216E040, hud);
  }
  uint64_t answer = 0;
  if (race_kind == 1 && IsFirstDay(base, frame) == 0) {
    answer = callees.Call(kCareerResultsUpdate, frame, 0x8216E0C4,
                          Load(base, Holder(base, machine) + 1024));
  }
  const uint32_t first_day = IsFirstDay(base, frame);
  if (first_day != 0) {
    race_states::steps::Post(base, dispatcher, 48);
  } else if (uint32_t(answer) != 0 || ReplayBit(base, Holder(base, machine))) {
    guest::StoreU32(base, machine + kState, 22);
  }
  return first_day;
}

std::optional<uint64_t> PauseUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                    const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4) {
    return std::nullopt;  // kind 4's pause (sub_82211458), never seen in play
  }
  ITC_NATIVE_RUNNING("sub_8216DB70 RaceHudMachine::PauseUpdate");
  const uint32_t frame = Frame(base, sp, 128);
  const int32_t mode = int32_t(Load(base, Load(base, kRaceData) + 0xA3AF4));
  uint64_t answer = 0;
  if (kind > 0 && (kind <= 3 || kind == 5 || kind == 6)) {
    if (kind == 6 && mode == 8) {
      const uint32_t hud = Holder(base, machine) + kHud;
      callees.Call(kHudTick, frame, 0x8216DBE8, hud, 1);
      callees.Call(kHudUpdateWarning, frame, 0x8216DBF0, hud);
      callees.Call(kHudTickPanel, frame, 0x8216DBF8, hud);
    }
    // PauseMenu::Update's handlers don't read r4: the dispatcher passed.
    answer = callees.Call(kPauseMenuUpdate, frame, 0x8216DC04,
                          Load(base, Load(base, kHolderGlobal) + 1012), dispatcher);
  }
  switch (uint32_t(answer)) {
    case 1:
      race_states::steps::Post(base, dispatcher, 29);
      break;
    case 2:
    case 6:
    case 7:
      race_states::steps::Post(base, dispatcher, 48);
      break;
    case 3:
      race_states::steps::Post(base, dispatcher, 45);
      // sub_82211D00 (the countdown stopped), never seen in play: the
      // original.
      return callees.Call(0x82211D00, frame, 0x8216DCF4, Holder(base, machine));
    case 4:
      race_states::steps::Post(base, dispatcher, 20);
      break;
    case 5:
      race_states::steps::Post(base, dispatcher, 21);
      break;
    case 8:
      return callees.Call(kHudApplyOptions, frame, 0x8216DD14,
                          uint64_t(Load(base, kHolderGlobal)) + kHud);
    case 9:
      race_states::steps::Post(base, dispatcher, 48);
      if (mode == 7) {
        guest::StoreU32(base, Load(base, 0x82816E3C) + 240, 1);
      }
      break;
    default:
      break;
  }
  return answer;
}

std::optional<uint64_t> AfterRaceUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher,
                                        uint32_t sp, const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t race = Load(base, kRaceData);
  const uint32_t kind = Load(base, machine + kKind);
  const uint32_t record = vehicle_accessors::RacerRecordsAt(base, race + 0x59AE8, 0xFFFFFFFFu);
  const uint32_t word = Load(base, record + 3916);
  // Kind 0's +12 screen, kind 4's and kind 6's screen after the race
  // (sub_8220B9E8, sub_82216850, sub_8221F1E8), never seen in play: the
  // original.
  if (kind == 0 || kind == 4 || (kind == 6 && (word == 1 || word == 3))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216DD88 RaceHudMachine::AfterRaceUpdate");
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t last = record;
  if ((kind >= 1 && kind <= 3) || kind == 6) {
    const uint32_t hud = Holder(base, machine) + kHud;
    callees.Call(kHudTick, frame, 0x8216DDEC, hud, 0);
    last = callees.Call(kHudUpdateWarning, frame, 0x8216DDF4, hud);
  }
  // A post, then the replay bit (+8 bit 0) set or cleared.
  auto post = [&](uint32_t message, bool set) {
    race_states::steps::Post(base, dispatcher, message);
    const uint32_t holder = Holder(base, machine);
    const uint32_t flags = Load(base, holder + kHolderFlags);
    guest::StoreU32(base, holder + kHolderFlags, set ? (flags | 1) : (flags & ~1u));
  };
  const int32_t now = int32_t(Load(base, machine + kKind));
  if (now <= 0) {
    return last;
  }
  if (now == 6) {
    if (word == 0 || word == 2) {
      race_states::steps::Post(base, dispatcher, 40);
    }
    return last;
  }
  if (now > 4) {
    return last;
  }
  const uint64_t answer = callees.Call(kAfterRaceUpdate, frame, 0x8216DF90,
                                       Load(base, Holder(base, machine) + 1020));
  switch (uint32_t(answer)) {
    case 1:
    case 8:
      post(40, false);
      break;
    case 2:
      post(42, true);
      break;
    case 4:
    case 5:
    case 7:
    case 9:
      post(48, false);
      break;
    case 6:
      post(45, false);
      break;
    default:
      break;
  }
  return answer;
}

std::optional<uint64_t> CloseResults(uint8_t* base, uint64_t r3, uint32_t sp,
                                     const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t race = Load(base, kRaceData);
  const int32_t kind = int32_t(Load(base, race + 0xA3AF0));
  if (kind == 6) {
    return std::nullopt;  // kind 6's screen after the race, never seen in play
  }
  ITC_NATIVE_RUNNING("sub_8216D198 RaceHudMachine::CloseResults");
  const uint32_t frame = Frame(base, sp, 96);
  // The results' screen by the kind (the empty sub_82106540 on it: only its
  // loads are left).
  if (kind == 1) {
    const int32_t mode = int32_t(Load(base, race + 0xA3AF4));
    Load(base, Load(base, machine + kHolder) + (mode != 0 ? 1024 : 1028));
  } else if (kind == 2) {
    Load(base, Load(base, machine + kHolder) + 1028);
  } else if (kind == 4) {
    Load(base, Load(base, machine + kHolder) + 1032);
  }
  const uint64_t closed =
      callees.Call(kAfterRaceClose, frame, 0x8216D24C, Load(base, Holder(base, machine) + 1020));
  guest::StoreU32(base, Holder(base, machine) + 440, 1);
  return closed;
}

std::optional<uint64_t> SetUp(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216D450 RaceHudMachine::SetUp");
  const uint32_t frame = Frame(base, sp, 160);
  uint64_t last = r3;
  uint32_t hud = 0;
  bool hud_set_up = false;
  if (kind == 1 || kind == 2) {
    // Then as kind 3.
    callees.Call(kAltSetUp, frame, 0x8216D598, Holder(base, machine) + kAltHud, 0);
  }
  if (kind >= 1 && kind <= 3) {
    hud = Holder(base, machine) + kHud;
    callees.Call(kHudSetPlayer, frame, 0x8216D5AC, hud, 0);
    hud_set_up = true;
  } else if (kind == 5) {
    last = callees.Call(kAltSetUp, frame, 0x8216D584, Holder(base, machine) + kAltHud, 0);
  } else if (kind == 6) {
    const uint32_t holder = Holder(base, machine);
    hud = holder + kHud;
    // The player's racer record's word +3888 (race data + 3,936 x (its
    // index + 1)), bit 0x400.
    const uint32_t index = Load(base, Load(base, Load(base, kRacers) + 2308) + 24);
    const uint32_t word = Load(base, (index + 1) * 3936 + Load(base, kRaceData));
    const uint32_t player = (word >> 10) & 1;
    callees.Call(kHudSetPlayer, frame, player != 0 ? 0x8216D4D8 : 0x8216D4E8, hud, player);
    callees.Call(kAltSetUp, frame, 0x8216D4F4, holder + kAltHud, player);
    hud_set_up = true;
  }
  if (hud_set_up) {
    callees.Call(kHudReset, frame, 0x8216D5B4, hud);
    callees.Call(kHudClearWarning, frame, 0x8216D5BC, hud);
    callees.Call(kHudSetUpGauges, frame, 0x8216D5C4, hud);
    callees.Call(kHudResetBar, frame, 0x8216D5CC, hud);
    callees.Call(kHudSetUpExtraGauges, frame, 0x8216D5D4, hud);
    callees.Call(kHudSetUpMirror, frame, 0x8216D5DC, hud);
    callees.Call(kHudApplyOptions, frame, 0x8216D5E4, hud);
    last = callees.Call(kHudSetUpSigns, frame, 0x8216D5EC, hud);
  }
  guest::StoreU32(base, machine + kFlag, 0);
  return last;
}

uint64_t CreateScreens(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216CA28 RaceHudMachine::CreateScreens");
  const uint32_t frame = Frame(base, sp, 112);
  guest::StoreU32(base, machine + kKind, Load(base, Load(base, kRaceData) + 0xA3AF0));
  const uint32_t race = Load(base, kRaceData);
  const int32_t type = int32_t(vehicle_accessors::StagesType(base, race + 0x59620, 0xFFFFFFFFu));
  const uint32_t kind = Load(base, machine + kKind);
  // The holder's kind, or -1 for none.
  int32_t holder_kind = -1;
  auto by_type = [type](int32_t even, int32_t odd) {
    if (type == 0 || type == 2) {
      return even;
    }
    return type == 1 || type == 3 ? odd : -1;
  };
  switch (kind) {
    case 0:
      holder_kind = 8;
      break;
    case 1:
      if (type == 0 || type == 2 || type == 4) {
        holder_kind = 0;
      } else if (type == 1 || type == 3) {
        holder_kind = Load(base, Load(base, kRaceData) + 0xA3AF4) == 0 ? 2 : 1;
      }
      break;
    case 2:
      holder_kind = 2;
      break;
    case 3:
      holder_kind = 3;
      break;
    case 4:
      holder_kind = by_type(4, 5);
      break;
    case 5:
      holder_kind = 9;
      break;
    case 6:
      holder_kind = by_type(7, 6);
      break;
    default:
      holder_kind = 10;
      break;
  }
  uint64_t r3 = uint32_t(type);
  if (holder_kind >= 0) {
    r3 = callees.Call(kScreensCreate, frame, 0x8216CB74, uint32_t(holder_kind));
  }
  guest::StoreU32(base, machine + kHolder, Load(base, kHolderGlobal));
  return r3;
}

uint64_t StartCountdown(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216CC60 RaceHudMachine::StartCountdown");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  const uint32_t race = Load(base, kRaceData);
  uint64_t result = r3;
  auto type = [&]() {
    const uint32_t t = vehicle_accessors::StagesType(base, race + 0x59620, 0xFFFFFFFFu);
    result = t;
    return t;
  };
  uint64_t mode = 0;
  switch (kind) {
    case 1: {
      const uint32_t t = type();
      if (t == 1 || t == 3) {
        mode = Load(base, Load(base, kRaceData) + 0xA3AF4) == 0 ? 2 : 1;
      }
      break;
    }
    case 2:
      mode = 2;
      break;
    case 4: {
      const uint32_t t = type();
      mode = t == 1 || t == 3 ? 1 : 0;
      break;
    }
    case 6: {
      const uint32_t t = type();
      mode = t == 0 || t == 2 ? 3 : t == 1 || t == 3 ? 4 : 0;
      break;
    }
    default:
      break;
  }
  if (ReplayBit(base, Holder(base, machine))) {
    return result;
  }
  const int32_t now = int32_t(Load(base, machine + kKind));
  if (now <= 0) {
    return result;
  }
  const uint32_t progress = race_hud_object::kRaceProgress;
  if (now <= 3 || now == 6) {
    guest::StoreU32(base, Holder(base, machine) + 460, Load(base, progress) + 2184);
    callees.Call(kHudSetMode, frame, 0x8216CDB4, Holder(base, machine) + kHud, mode);
    return callees.Call(kHudStartCountdown, frame, 0x8216CDC0, Holder(base, machine) + kHud);
  }
  if (now == 4) {
    guest::StoreU32(base, Holder(base, machine) + 1000, Load(base, progress) + 2184);
    callees.Call(k656SetMode, frame, 0x8216CDEC, Holder(base, machine) + 656, mode);
    return callees.Call(k656Start, frame, 0x8216CDF8, Holder(base, machine) + 656);
  }
  return result;
}

uint64_t ClosePause(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216C658 RaceHudMachine::ClosePause");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  uint64_t last = r3;
  if (kind == 4) {
    // The empty sub_82106540 on the holder's +1016: its r3 left.
    last = Load(base, Holder(base, machine) + 1016);
  } else if (kind > 0 && (kind <= 3 || kind == 6)) {
    callees.Call(kPauseMenuClose, frame, 0x8216C69C, Load(base, Holder(base, machine) + 1012));
    last = callees.Call(kHudApplyOptions, frame, 0x8216C6A8,
                        uint64_t(Holder(base, machine)) + kHud);
  }
  guest::StoreU32(base, machine + kFlag, 0);
  return last;
}

std::optional<uint64_t> ResultsDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                    const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t race = Load(base, kRaceData);
  const uint32_t race_kind = Load(base, race + 0xA3AF0);
  const bool career = race_kind == 1 && Load(base, race + 0xA3AF4) != 0;
  if (Load(base, machine + kKind) == 4 || race_kind == 0 || race_kind == 2 || race_kind == 4 ||
      race_kind == 6 || (race_kind == 1 && !career)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8216D6D0 RaceHudMachine::ResultsDraw");
  const uint32_t frame = Frame(base, sp, 96);
  if (career && IsFirstDay(base, frame) == 0) {
    callees.Call(kCareerResultsDraw, frame, 0x8216D76C, Load(base, Holder(base, machine) + 1024));
  }
  return callees.Call(kHudDraw, frame, 0x8216D794, uint64_t(Holder(base, machine)) + kHud);
}

std::optional<uint64_t> AfterRaceDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const uint32_t race = Load(base, kRaceData);
  const uint32_t kind = Load(base, machine + kKind);
  const uint32_t record = vehicle_accessors::RacerRecordsAt(base, race + 0x59AE8, 0xFFFFFFFFu);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  if (kind == 6) {
    const int32_t word = int32_t(Load(base, record + 3916));
    if (word == 1 || word == 3) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_8216C6D8 RaceHudMachine::AfterRaceDraw");
  const uint32_t frame = Frame(base, sp, 96);
  if (kind > 3) {
    return record;
  }
  callees.Call(kHudDraw, frame, 0x8216C768, uint64_t(Holder(base, machine)) + kHud);
  return callees.Call(kDrawAfterRaceMenu, frame, 0x8216C770, Holder(base, machine));
}

std::optional<uint64_t> PauseDraw(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216CF08 RaceHudMachine::PauseDraw");
  const uint32_t machine = uint32_t(r3);
  const uint32_t kind = Load(base, machine + kKind);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(base, sp, 96);
  if (kind > 6) {
    return r3;
  }
  if (kind != 5 && !ReplayBit(base, Holder(base, machine))) {
    callees.Call(kHudDraw, frame, 0x8216CF58, uint64_t(Holder(base, machine)) + kHud);
    callees.Call(kHudDrawWarning, frame, 0x8216CF64, uint64_t(Holder(base, machine)) + kHud);
    return callees.Call(kDrawPauseMenu, frame, 0x8216CF6C, Holder(base, machine));
  }
  callees.Call(kDrawPauseMenu, frame, 0x8216CF78, Holder(base, machine));
  return callees.Call(kAltRunMovie, frame, 0x8216CF84, uint64_t(Holder(base, machine)) + kAltHud);
}

std::optional<uint64_t> RacePauseDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216CFE0 RaceHudMachine::RacePauseDraw");
  const uint32_t machine = uint32_t(r3);
  const uint32_t kind = Load(base, machine + kKind);
  if (kind == 0 || kind == 4) {
    return std::nullopt;
  }
  const uint32_t frame = Frame(base, sp, 96);
  if (kind > 6) {
    return r3;
  }
  if (kind != 5 && !ReplayBit(base, Holder(base, machine))) {
    callees.Call(kHudDraw, frame, 0x8216D030, uint64_t(Holder(base, machine)) + kHud);
    callees.Call(kHudRunMovie, frame, 0x8216D040, uint64_t(Holder(base, machine)) + kHud, 1);
    callees.Call(kHudDrawWarning, frame, 0x8216D04C, uint64_t(Holder(base, machine)) + kHud);
    return callees.Call(kDrawPauseMenu, frame, 0x8216D054, Holder(base, machine));
  }
  callees.Call(kDrawPauseMenu, frame, 0x8216D060, Holder(base, machine));
  return callees.Call(kAltRunMovie, frame, 0x8216D06C, uint64_t(Holder(base, machine)) + kAltHud);
}

namespace {

// PauseMenu::Open(holder +1012, page, flag) as a tail call.
uint64_t OpenAt(uint8_t* base, uint32_t holder, uint64_t page, uint64_t flag, uint32_t sp,
                uint32_t lr, const Calls& callees) {
  return callees.Call(kPauseMenuOpen, sp, lr, Load(base, holder + 1012), page, flag);
}

}  // namespace

std::optional<uint64_t> OpenPause(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                                  const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4 || kind == 6) {
    return std::nullopt;
  }
  if (kind < 1 || kind > 6) {
    return r3;
  }
  ITC_NATIVE_RUNNING("sub_8216D600 RaceHudMachine::OpenPause");
  const uint32_t holder = Holder(base, machine);
  switch (kind) {
    case 1:
      return OpenAt(base, holder, ReplayBit(base, holder) ? 8 : 0, 0, sp, lr, callees);
    case 2:
      return OpenAt(base, holder, 3, 0, sp, lr, callees);
    case 3:
      return OpenAt(base, holder, 4, 0, sp, lr, callees);
    default:
      return OpenAt(base, holder, 8, 0, sp, lr, callees);
  }
}

std::optional<uint64_t> OpenRacePause(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                                      const Calls& callees) {
  const uint32_t machine = uint32_t(r3);
  const int32_t kind = int32_t(Load(base, machine + kKind));
  if (kind == 4 || (kind == 6 && !ReplayBit(base, Holder(base, machine)))) {
    return std::nullopt;
  }
  if (kind < 1 || kind > 6) {
    return r3;
  }
  ITC_NATIVE_RUNNING("sub_8216DA40 RaceHudMachine::OpenRacePause");
  const uint32_t holder = Holder(base, machine);
  switch (kind) {
    case 1:
      if (ReplayBit(base, holder)) {
        return OpenAt(base, holder, 8, 0, sp, lr, callees);
      }
      if (Load(base, Load(base, kRaceData) + 0xA3AF4) != 0) {
        return OpenCareerPause(base, machine, sp, callees);
      }
      return OpenAt(base, holder, 2, 0, sp, lr, callees);
    case 2:
      return OpenAt(base, holder, ReplayBit(base, holder) ? 8 : 3, 0, sp, lr, callees);
    case 3:
      return OpenAt(base, holder, 4, 0, sp, lr, callees);
    default:  // 5, and 6 with the replay bit
      return OpenAt(base, holder, 8, 0, sp, lr, callees);
  }
}

uint64_t OpenCareerPause(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216D8C8 RaceHudMachine::OpenCareerPause");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t first_day = IsFirstDay(base, frame);
  return OpenAt(base, Holder(base, machine), 1, first_day != 0 ? 1 : 0, frame, 0x8216D900,
                callees);
}

uint32_t IsFirstDay(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8216D3F0 RaceHudMachine::IsFirstDay");
  Frame(base, sp, 96);
  if (int32_t(Load(base, Load(base, kRaceData) + 0xA3AF0)) != 1) {
    return 0;
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const int32_t day = int32_t(Load(base, save + 6100));
  return day <= 999 && day == 1 ? 1 : 0;
}

uint32_t OpponentPrize(uint8_t* base, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8216C820 RaceHudMachine::OpponentPrize");
  Frame(base, sp, 112);
  if (int32_t(id) == -1) {
    return 0;
  }
  const uint32_t item = data_tables::Item(base, id);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t record = id * 12 + save;
  const int32_t level = int32_t(int8_t(guest::LoadU8(base, item + 37))) + 1;
  int32_t times = 1;
  if (guest::LoadU16(base, record + 0x199FA) == 1) {
    times = guest::LoadU16(base, record + 0x199F8) != 0 ||
                    guest::LoadU16(base, record + 0x199FC) != 0
                ? 2
                : 3;
  }
  // The item's byte +35 (signed, compared unsigned).
  const uint32_t grade = uint32_t(int32_t(int8_t(guest::LoadU8(base, item + 35))));
  int32_t bonus = 0;
  if (grade == 0) {
    bonus = 10;
  } else if (grade == 1) {
    bonus = 20;
  } else if (grade == 2) {
    bonus = 15;
  } else if (grade < 6) {
    bonus = 30;
  }
  uint32_t value = uint32_t(int64_t(bonus / 10) * int64_t(times));
  value = uint32_t(int64_t(int32_t(value)) * int64_t(level));
  value = value * 10000u;
  return uint32_t(int32_t(value) / 1000);
}

uint32_t ItemBonus(uint8_t* base, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8216C938 RaceHudMachine::ItemBonus");
  Frame(base, sp, 96);
  if (int32_t(id) == -1) {
    return 0;
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  if (guest::LoadU16(base, id * 12 + save + 0x199FA) != 1) {
    return 0;
  }
  const uint32_t n = id - 376;
  int32_t value = 0;
  if (n <= 2) {
    value = 1000000;  // sub_8216C9B8's
  } else if (n <= 5) {
    value = 1500000;
  } else if (n == 6) {
    value = 2000000;
  } else if (n <= 14) {
    value = 3000000;
  } else if (n == 15) {
    value = 5000000;
  } else if (n <= 22) {
    value = 10000000;
  } else if (n == 23) {
    value = 100000000;
  }
  return uint32_t(value / 1000);
}

uint64_t DestroyScreens(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                        const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216C208 RaceHudMachine::DestroyScreens");
  return callees.Call(kScreensDestroy, sp, lr, r3);
}

uint64_t OnRaceEnd(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216E5B0 RaceHudMachine::OnRaceEnd");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  uint64_t last = r3;
  switch (int32_t(Load(base, machine + kKind))) {
    case 1:
      if (Load(base, Load(base, kRaceData) + 0xA3AF4) == 0) {
        last = callees.Call(kShowResults, frame, 0x8216E630, r3, 1);
      } else {
        last = IsFirstDay(base, frame);
        if (uint32_t(last) == 0) {
          last = AwardPrize(base, r3, frame, callees);
        }
      }
      break;
    case 2:
      last = callees.Call(kShowResults, frame, 0x8216E630, r3, 0);
      break;
    case 4:
      last = callees.Call(kShowSplitResults, frame, 0x8216E5F8, r3);
      break;
    case 6:
      last = callees.Call(kShowKind6Results, frame, 0x8216E5EC, r3);
      break;
    default:
      break;
  }
  guest::StoreU32(base, Holder(base, machine) + 440, 0);
  return last;
}

uint64_t AwardPrize(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees) {
  ITC_NATIVE_RUNNING("sub_8216E150 RaceHudMachine::AwardPrize");
  using namespace camera_modes::steps;
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 336);
  const uint32_t block = frame + 96;
  auto store = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, block + offset, value); };
  auto word = [&](uint32_t offset) { return Load(base, block + offset); };
  const uint32_t result =
      vehicle_accessors::RacerRecordsAt(base, Load(base, kRaceData) + 0x59AE8, 0xFFFFFFFFu);
  uint32_t save = 0;
  if (int32_t(Load(base, machine + kKind)) == 1) {
    save = vehicle_accessors::SaveDataOf(base, 0);
  }
  if (!ReplayBit(base, Holder(base, machine))) {
    // memset(block, 0, 168).
    for (uint32_t i = 0; i < 168; i++) {
      base[block + i] = 0;
    }
    const int32_t outcome = int32_t(Load(base, result + 3920));
    if (outcome == 1) {
      const int32_t place = int32_t(Load(base, result + 3924));
      if (place == 1) {
        store(0, 0);
      } else if (place == 2) {
        store(0, 2);
      } else if (place == 3) {
        store(0, 1);
      }
      if (int32_t(Load(base, result + 3924)) == 1) {
        store(8, 0);
        store(4, 0);
        store(20, 0);
        const double f0 = Fmuls(Lfs(base, result + 828), Lfs(base, 0x82001FD8));
        guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(f0)));
        store(8, uint32_t(int32_t(Load(base, frame + 80)) / 1000));
        if (int32_t(Load(base, result + 0)) > 1) {
          int32_t i = 1;
          uint32_t at = result + 980;
          do {
            const uint32_t prize = OpponentPrize(base, Load(base, at), frame);
            store(4, prize + word(4));
            const uint32_t bonus = ItemBonus(base, Load(base, at), frame);
            i++;
            at += 972;
            store(20, bonus + word(20));
          } while (i < int32_t(Load(base, result + 0)));
        }
      }
    } else if (outcome == 3) {
      store(4, 0);
      store(8, 0);
      store(0, 2);
    }
    const double f30 = Lfs(base, 0x82000A30);
    const double f31 = Lfs(base, 0x82000A34);
    double factor = f31;
    if (save != 0) {
      const uint32_t day = save + 72;
      // DayBlock::WinStreak again before each test, as the original.
      auto streak = [&] { return int32_t(int16_t(career::DayBlockWinStreak(base, day))); };
      if (streak() >= 20) {
        factor = Lfs(base, 0x820021E8);
      } else if (streak() >= 15) {
        factor = Lfs(base, 0x82002CF0);
      } else if (streak() >= 10) {
        factor = Lfs(base, 0x820027A8);
      } else if (streak() >= 7) {
        factor = Lfs(base, 0x820029F0);
      } else if (streak() >= 5) {
        factor = Lfs(base, 0x820029D4);
      } else if (streak() >= 3) {
        factor = Lfs(base, 0x82002CB0);
      } else {
        factor = f30;
      }
    }
    Stfs(base, block + 12, factor);
    {
      // (+4 + +8) as a doubleword at the frame's +80 (fcfid), times it.
      const int64_t sum = int32_t(word(8) + word(4));
      guest::StoreU32(base, frame + 80, uint32_t(uint64_t(sum) >> 32));
      guest::StoreU32(base, frame + 84, uint32_t(sum));
      store(16, uint32_t(Fctiwz(Fmuls(Single(double(sum)), factor))));
    }
    double f0;
    if (guest::LoadU16(base, result + 968) == 0) {
      f0 = f31;
      Stfs(base, block + 24, f0);
    } else {
      f0 = Lfs(base, result + 844);
      Stfs(base, block + 24, f0);
      if (f0 == f31) {
        f0 = f30;
        Stfs(base, block + 24, f0);
      }
    }
    const double scale = Lfs(base, 0x82001BE0);
    store(28, uint32_t(Fctiwz(Fmuls(f0, scale))));
    if (int32_t(int16_t(guest::LoadU16(base, result + 968))) > 1) {
      int32_t k = 1;
      uint32_t at = result + 852;
      do {
        double value = Lfs(base, at);
        Stfs(base, frame + 80, value);
        if (value == f31) {
          value = f30;
          Stfs(base, frame + 80, value);
        }
        const uint32_t n = word(160);
        const double product = Fmuls(value, scale);
        store(160, n + 1);
        k++;
        at += 8;
        guest::StoreU32(base, frame + 84, uint32_t(Fctiwz(product)));
        // The doubleword at the frame's +80 (the float, the int) copied as
        // one (ld, std).
        const uint32_t high = Load(base, frame + 80), low = Load(base, frame + 84);
        const uint32_t pair = frame + 128 + (n << 3);
        guest::StoreU32(base, pair, high);
        guest::StoreU32(base, pair + 4, low);
      } while (k < int32_t(int16_t(guest::LoadU16(base, result + 968))));
    }
    store(164, Load(base, save + 6116));
    callees.Call(kCareerResultsOpen, frame, 0x8216E4A4, Load(base, Holder(base, machine) + 1024),
                 block);
    uint64_t total = uint64_t(word(28)) + word(16) + word(20);
    const uint32_t pairs = word(160);
    for (uint32_t p = 0; p < pairs; p++) {
      total += Load(base, block + 36 + 8 * p);
    }
    career::DayBlockAddWord4832(base, save + 72, total);
    uint32_t money = uint32_t(Load(base, save + 6116) + total);
    if (int32_t(money) > 9999999) {
      money = 9999999;
    }
    guest::StoreU32(base, save + 6116, money);
    career::StatsAddPoints(base, save + 48, total);
  }
  const uint32_t flag = Load(base, result + 3920) == 3 ? 1 : 0;
  uint32_t race = Load(base, kRaceData);
  uint32_t menu;
  if (Load(base, race + 0xA3AF4) == 0) {
    menu = 4;
  } else if (vehicle_accessors::StagesHasNext(base, race + 0x59620) != 0 &&
             int32_t(Load(base, result + 3924)) == 1) {
    menu = 3;
  } else {
    race = Load(base, kRaceData);
    menu = int32_t(vehicle_accessors::StagesWord808(base, race + 0x59620)) != 0 ? 1 : 0;
  }
  return callees.Call(kAfterRaceOpen, frame, 0x8216E5A0, Load(base, Holder(base, machine) + 1020),
                      menu, flag);
}

}  // namespace race_hud_machine
