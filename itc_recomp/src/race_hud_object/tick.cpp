// The race HUD object's ticks (docs/functions/race-hud-object.md): see
// race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "racer_controllers/racer_controllers.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace race_hud_object {

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(uint64_t(guest::LoadU32(base, address)) << 32 |
                               guest::LoadU32(base, address + 4));
}

// The km/h's factor for the dials (the double at 0x8200A510).
constexpr uint32_t kFactor = 0x8200A510;

}  // namespace

uint64_t Tick(uint8_t* base, uint32_t hud, uint32_t flag, uint32_t sp, const Calls& calls,
              const track_zones::OwnerCalls& zone_calls) {
  ITC_NATIVE_RUNNING("sub_822153A0 RaceHud::Tick");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const uint32_t car = guest::LoadU32(base, hud + kHudVehicle);
  double f1 = double(Vehicle_GetSpeedKmh(base, car));
  const double speed = Single(f1 * LoadDouble(base, kFactor));
  guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(speed)));
  SetSpeed(base, hud, guest::LoadU32(base, frame + 80), frame, &f1);
  const int32_t int149 = Fctiwz(Lfs(base, car + 6252));
  const int32_t int119 = Fctiwz(Lfs(base, car + 6256));
  const double value = Lfs(base, car + 6260);
  guest::StoreU32(base, frame + 80, uint32_t(int149));
  const uint32_t r6 = guest::LoadU32(base, frame + 80);
  guest::StoreU32(base, frame + 80, uint32_t(int119));
  const uint32_t r5 = guest::LoadU32(base, frame + 80);
  UpdateExtraGauges(base, hud, value, r5, r6, frame, calls);
  SetGear(base, hud, int32_t(guest::LoadU32(base, car + 2252)), frame);
  SetTach(base, hud, double(vehicle_accessors::VehicleFloat2224(base, car)), frame);
  SetBar(base, hud, Lfs(base, car + 2420), frame);
  SetBoost(base, hud, Lfs(base, car + 2944), frame);
  const uint32_t extra = guest::LoadU32(base, guest::LoadU32(base, car + 5712) + 3892);
  const uint32_t node =
      ui_movies::FindNode(base, guest::LoadU32(base, hud + kHudDashboard), kNone, 13);
  guest::StoreU32(base, node + 80, extra);
  return UpdateSigns(base, hud, flag, frame, calls, zone_calls);
}

uint64_t TickTimer(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82214D78 RaceHud::TickTimer");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const uint32_t racers = guest::LoadU32(base, hud + 444);
  guest::StoreU32(base, frame + 80,
                  ai_drivers::TimeFromTicks(guest::LoadU32(base, racers + 3932)));
  SetTime(base, hud, frame + 80, frame);
  const uint32_t racers2 = guest::LoadU32(base, hud + 444);
  const uint32_t progress = guest::LoadU32(base, kRaceProgress);
  const double start = Lfs(base, racers2 + 828);
  double distance = Fsubs(Lfs(base, progress + 7156), start);
  distance = Fmuls(distance, Lfs(base, 0x820029D8));
  double f1 = Single(distance * LoadDouble(base, kFactor));
  SetDistance(base, hud, frame, &f1);
  const int32_t animation = int32_t(guest::LoadU32(base, guest::LoadU32(base, hud + 444) + 12));
  const uint32_t timer = guest::LoadU32(base, hud + kHudTimer);
  if (animation < 0) {
    const uint32_t node = ui_movies::FindNode(base, timer, kNone, 0);
    guest::StoreU32(base, node + 80, 0);
    return node;
  }
  const uint32_t node = ui_movies::FindNode(base, timer, kNone, 0);
  guest::StoreU32(base, node + 80, 1);
  if (int32_t(guest::LoadU32(base, hud + 464)) == animation) {
    return node;
  }
  for (uint32_t i = 0; i < 5; i++) {
    calls.Call(kFinishNamed, frame, 0x82214E28, guest::LoadU32(base, hud + kHudTimer), i);
  }
  ui_movies::PlayNamed(base, guest::LoadU32(base, hud + kHudTimer), uint32_t(animation), 0, 0, 0,
                       0);
  // UiMovie::LoopNamed's r3: the slot moved to, else (from its 176-byte
  // frame below this one) the copy's place when the list had slots, or the
  // movie.
  const uint32_t movie = guest::LoadU32(base, hud + kHudTimer);
  const ui_movies::Moved moved =
      ui_movies::MoveNamedAnimation(base, movie, uint32_t(animation), true);
  const uint64_t r3 = moved.slot ? *moved.slot : moved.looked ? frame - 176 + 80 : movie;
  guest::StoreU32(base, hud + 464, uint32_t(animation));
  return r3;
}

uint64_t TickRacers(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                    const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_822151F8 RaceHud::TickRacers");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = hud;
  const int32_t count = int32_t(guest::LoadU32(base, hud + 452));
  guest::StoreU32(base, hud + 456, 0);
  if (count > 0) {
    int64_t i = 0;
    do {
      r3 = UpdateRacerBar(base, hud, uint64_t(i), uint64_t(i), frame, calls, sounds);
      i++;
    } while (int32_t(i) < int32_t(guest::LoadU32(base, hud + 452)));
  }
  return r3;
}

uint64_t TickPanel(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                   const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82215488 RaceHud::TickPanel");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const uint32_t mode = guest::LoadU32(base, hud + 448);
  // Modes 2 to 4 never ran in play: the originals, through `calls` (mode
  // 4's r3 the HUD as well, which the original doesn't set again).
  switch (mode) {
    case 0:
      TickRacers(base, hud, frame, calls, sounds);
      break;
    case 1:
      TickTimer(base, hud, frame, calls);
      break;
    case 2:
      calls.Call(kTickMode2, frame, 0x822154DC, hud);
      break;
    case 3:
      calls.Call(kTickMode3, frame, 0x822154D0, hud);
      break;
    case 4:
      calls.Call(kTickMode4, frame, 0x822154C4, hud);
      break;
    default:
      break;
  }
  const uint32_t room = racer_controllers::RecordingHasRoom(base, 0);
  return UpdateRecordingFull(base, hud, int32_t(room), frame, calls);
}

}  // namespace race_hud_object
