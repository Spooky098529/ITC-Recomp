// The location screen's pages that set up a race
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "guest.h"
#include "race_event/race_event.h"
#include "schema/location_screen.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

namespace {

using G = schema::LocationScreenGlobals;

void Leave(uint32_t machine) {
  const schema::LocationScreenMachine m(machine);
  m.set_next_state(11);
  m.set_state(10);
}

}  // namespace

uint64_t Page9(uint32_t sp, uint32_t machine, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822280D8 LocationScreen::Page9");
  // The original's 112-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  calls.Call(kEventQuery, sp, 0x82228104, save, 0);
  const uint64_t r3 = calls.Call(kRaceSetUp, sp, 0x82228110, save, ~0ull);
  Leave(machine);
  return r3;
}

uint64_t Page11(uint32_t sp, uint32_t machine, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228138 LocationScreen::Page11");
  const uint32_t frame = sp - 3136;
  guest::StoreU32(frame, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const schema::LocationScreen screen = G::screen();
  // The picked opponent (the original's +80, read only here: a value since
  // E5 step 6).
  const uint32_t picked = screen.picked_opponent();
  const uint32_t event = guest::LoadU32(race_event::kEvent);
  const uint32_t item = race_event::GroupItem(event, G::screen().group_kind(), picked);
  Copy(calls, frame, 0x82228188, frame + 96, item, 3008);
  calls.Call(kEventQuery, frame, 0x82228194, save, 1);
  const uint64_t rival = uint64_t(int64_t(int16_t(guest::LoadU16(frame + 96))));
  const uint64_t r3 = calls.Call(kRaceSetUp, frame, 0x822281A0, save, rival);
  // The race data's flags (+0x59AE4, Race::SetUp's): 0x10 a rival race.
  const schema::RaceData race = schema::RaceStatesGlobals::race_data();
  race.set_flags(race.flags() | 16);
  Leave(machine);
  return r3;
}

}  // namespace location_screen
