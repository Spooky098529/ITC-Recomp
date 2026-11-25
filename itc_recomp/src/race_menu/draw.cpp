// The race menu's Draw and the pages it runs last (docs/functions/race-menu.md:
// RaceMenu::Draw, LeavePage, StartPage, StartEvent): see race_menu.h.

#include "race_menu/race_menu.h"

#include "base/log.h"
#include "guest.h"
#include "race_menu/internal.h"

namespace race_menu {

namespace {

constexpr uint32_t kSetUpEvent = 0x82114268;  // Career::SetUpEvent, through its override
constexpr uint32_t kRaceSetUp = 0x82115D98;   // Race::SetUp, through its override

// Both fades' counts 0.
bool FadesDone() { return (G::fade_out_counts() | G::fade_in_counts()) == 0; }

}  // namespace

uint64_t StartEvent(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82188CE8 RaceMenu::StartEvent");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t type = EventAt(machine, Machine(machine).event_cursor()).type();
  // The location by the event's type (case 0 the piece sub_82188D44's).
  uint64_t location = 0;
  auto variant = [&](uint32_t plus) { return uint64_t(Machine(machine).variant()) + plus; };
  switch (type) {
    case 0: location = variant(0); break;
    case 1: location = variant(2); break;
    case 2: location = variant(4); break;
    case 3: location = 6; break;
    case 4: location = 7; break;
    case 5: location = variant(8); break;
    case 6: location = variant(10); break;
    case 7: location = 12; break;
    default:
      location = type <= 14 ? type + 5 : 0;  // 8 to 14 -> 13 to 19
      break;
  }
  uint64_t r3 = calls.Call(kSetUpEvent, frame, 0x82188DD8, Machine(machine).save(),
                           Machine(machine).race_mode(), location);
  if (uint32_t(location - 14) <= 5) {
    Machine(machine).set_race_set_up(0);
    return r3;
  }
  const uint32_t save = Machine(machine).save();
  Machine(machine).set_race_set_up(1);
  return calls.Call(kRaceSetUp, frame, 0x82188E04, save, ~0ull);
}

uint64_t StartPage(uint64_t machine_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218BB18 RaceMenu::StartPage");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(sp, 96);
  if (!FadesDone()) {
    return machine_r3;
  }
  const uint64_t r3 = StartEvent(machine, frame, calls);
  Machine(machine).set_state(10);
  return r3;
}

uint64_t LeavePage(uint64_t machine_r3, uint32_t mode, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218B9F0 RaceMenu::LeavePage");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(sp, 176);
  if (!FadesDone()) {
    return machine_r3;
  }
  // Cue 0 of bank 1 stopped when it plays; cue 2 of bank 1 played unless it
  // plays.
  Store(frame + 80, 0);
  Store(frame + 84, 1);
  uint64_t r3 = CallSlot(calls, kIsCuePlayingSlot, frame, 0x8218BA4C, frame + 84, frame + 80);
  if (uint8_t(r3) != 0) {
    Store(frame + 88, 0);
    Store(frame + 92, 1);
    r3 = CallSlot(calls, kStopCueSlot, frame, 0x8218BA78, frame + 92, frame + 88);
  }
  Store(frame + 100, 1);
  Store(frame + 96, 2);
  r3 = CallSlot(calls, kIsCuePlayingSlot, frame, 0x8218BAA0, frame + 100, frame + 96);
  if (uint8_t(r3) == 0) {
    Store(frame + 104, 2);
    Store(frame + 108, 1);
    r3 = calls.Call(kSoundPlay, frame, 0x8218BABC, frame + 108, frame + 104);
  }
  Post(mode, 7);
  Machine(machine).set_leave_message(6);
  return r3;
}

uint64_t Draw(uint64_t machine_r3, uint64_t mode_r4, uint32_t sp,
              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218C578 RaceMenu::Draw");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(sp, 112);
  uint64_t r3 = machine_r3;
  if (Machine(machine).takes_input() == 0) {
    return r3;
  }
  switch (G::pages().page()) {
    case 3:  // the piece sub_8218C5DC's
      r3 = KindPage(machine, frame, calls);
      break;
    case 4:
      r3 = EventPage(machine, uint32_t(mode_r4), frame, calls);
      break;
    case 5:
      r3 = VariantPage(machine, frame, calls);
      break;
    case 7:
      Back(machine);
      break;
    case 9:
      r3 = LeavePage(machine_r3, uint32_t(mode_r4), frame, calls);
      break;
    case 10:
      r3 = StartPage(machine_r3, frame, calls);
      break;
    default:
      break;
  }
  if (G::pages2().page() == 7) {
    Back(machine);
    r3 = machine_r3;
  }
  return r3;
}

}  // namespace race_menu
