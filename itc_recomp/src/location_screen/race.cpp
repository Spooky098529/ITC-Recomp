// The location screen's pages that set up a race
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "guest.h"
#include "race_event/race_event.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

namespace {

// The race data's word Race::SetUp keeps its flags in (+0x59AE4).
constexpr uint32_t kRaceFlags = 0x59AE4;

void Leave(uint8_t* base, uint32_t machine) {
  guest::StoreU32(base, machine + 9620, 11);
  guest::StoreU32(base, machine + 9604, 10);
}

}  // namespace

uint64_t Page9(uint8_t* base, uint32_t sp, uint32_t machine, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822280D8 LocationScreen::Page9");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  calls.Call(kEventQuery, frame, 0x82228104, save, 0);
  const uint64_t r3 = calls.Call(kRaceSetUp, frame, 0x82228110, save, ~0ull);
  Leave(base, machine);
  return r3;
}

uint64_t Page11(uint8_t* base, uint32_t sp, uint32_t machine, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228138 LocationScreen::Page11");
  const uint32_t frame = sp - 3136;
  guest::StoreU32(base, frame, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t screen = guest::LoadU32(base, kScreen);
  guest::StoreU32(base, frame + 80, guest::LoadU32(base, screen + 800));
  const uint32_t event = guest::LoadU32(base, race_event::kEvent);
  const uint32_t item =
      race_event::GroupItem(base, event, guest::LoadU32(base, guest::LoadU32(base, kScreen) + 804),
                            guest::LoadU32(base, frame + 80));
  Copy(base, calls, frame, 0x82228188, frame + 96, item, 3008);
  calls.Call(kEventQuery, frame, 0x82228194, save, 1);
  const uint64_t rival = uint64_t(int64_t(int16_t(guest::LoadU16(base, frame + 96))));
  const uint64_t r3 = calls.Call(kRaceSetUp, frame, 0x822281A0, save, rival);
  const uint32_t flags = guest::LoadU32(base, kRaceData) + kRaceFlags;
  guest::StoreU32(base, flags, guest::LoadU32(base, flags) | 16);
  Leave(base, machine);
  return r3;
}

}  // namespace location_screen
