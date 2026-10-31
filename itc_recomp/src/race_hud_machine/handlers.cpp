// The race HUD machine's handlers and helpers as plain C++
// (docs/functions/race-hud-machine.md; see race_hud_machine.h). Each
// follows its original's loads (the holder read again from the machine
// before each call where the original reads it again), calls (their return
// addresses the originals') and stores in order.

#include "race_hud_machine/race_hud_machine.h"

#include "base/log.h"
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

}  // namespace race_hud_machine
