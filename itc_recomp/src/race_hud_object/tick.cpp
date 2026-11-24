// The race HUD object's ticks (docs/functions/race-hud-object.md): see
// race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "racer_controllers/racer_controllers.h"
#include "schema/car_physics.h"
#include "schema/race_hud_object.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/vehicle.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace race_hud_object {

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

using Hud = schema::RaceHud;
using Racers = schema::RaceHudRacers;
using G = schema::RaceHudObjectGlobals;

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
  // The original's frame: its callees' stack pointer (they make their own
  // frames below it; no guest code is called from it, so no back chain);
  // its + 80 a local.
  const uint32_t frame = sp - 128;
  const Hud h(base, hud);
  const schema::Vehicle vehicle = h.vehicle();
  const uint32_t car = vehicle.address;
  const schema::VehicleStatus status(base, vehicle.status_address());
  double f1 = double(Vehicle_GetSpeedKmh(base, car));
  const double speed = Single(f1 * LoadDouble(base, kFactor));
  SetSpeed(base, hud, uint32_t(Fctiwz(speed)), frame, &f1);
  const int32_t int149 = Fctiwz(double(status.water_temperature()));
  const int32_t int119 = Fctiwz(double(status.oil_temperature()));
  const double value = double(status.engine_output());
  const uint32_t r6 = uint32_t(int149);
  const uint32_t r5 = uint32_t(int119);
  UpdateExtraGauges(base, hud, value, r5, r6, frame, calls);
  SetGear(base, hud, vehicle.gear(), frame);
  SetTach(base, hud, double(vehicle_accessors::VehicleFloat2224(base, car)), frame);
  SetBar(base, hud, double(vehicle.nitrous_value()), frame);
  SetBoost(base, hud, double(vehicle.boost()), frame);
  const uint32_t extra = schema::Car(base, vehicle.car()).unknown_3892();
  const uint32_t node =
      ui_movies::FindNode(base, h.dashboard().address, kNone, 13);
  guest::StoreU32(base, node + 80, extra);
  return UpdateSigns(base, hud, flag, frame, calls, zone_calls);
}

uint64_t TickTimer(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82214D78 RaceHud::TickTimer");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const Hud h(base, hud);
  const Racers racers = h.racers();
  // The original's frame + 80, handed to SetTime: a local.
  SetTimeValue(base, hud, ai_drivers::TimeFromTicks(racers.ticks()));
  const Racers racers2 = h.racers();
  const schema::RaceProgress progress = G::race_progress(base);
  const double start = double(racers2.start_distance());
  double distance = Fsubs(double(progress.race_length()), start);
  distance = Fmuls(distance, Lfs(base, 0x820029D8));
  double f1 = Single(distance * LoadDouble(base, kFactor));
  SetDistance(base, hud, frame, &f1);
  const int32_t animation = int32_t(h.racers().timer_animation());
  const uint32_t timer = h.timer().address;
  if (animation < 0) {
    const uint32_t node = ui_movies::FindNode(base, timer, kNone, 0);
    guest::StoreU32(base, node + 80, 0);
    return node;
  }
  const uint32_t node = ui_movies::FindNode(base, timer, kNone, 0);
  guest::StoreU32(base, node + 80, 1);
  if (int32_t(h.timer_animation()) == animation) {
    return node;
  }
  for (uint32_t i = 0; i < 5; i++) {
    calls.Call(kFinishNamed, frame, 0x82214E28, h.timer().address, i);
  }
  ui_movies::PlayNamed(base, h.timer().address, uint32_t(animation), 0, 0, 0, 0);
  // UiMovie::LoopNamed's r3: the slot moved to, else (from its 176-byte
  // frame below this one) the copy's place when the list had slots, or the
  // movie.
  const uint32_t movie = h.timer().address;
  const ui_movies::Moved moved =
      ui_movies::MoveNamedAnimation(base, movie, uint32_t(animation), true);
  const uint64_t r3 = moved.slot ? *moved.slot : moved.looked ? frame - 176 + 80 : movie;
  h.set_timer_animation(uint32_t(animation));
  return r3;
}

uint64_t TickRacers(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                    const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_822151F8 RaceHud::TickRacers");
  // UpdateRacerBar's stack pointer (its frame below it); no guest code is
  // called from this one, so no back chain.
  const uint32_t frame = sp - 112;
  uint64_t r3 = hud;
  const Hud h(base, hud);
  const int32_t count = int32_t(h.racer_count());
  h.set_any_bar_low(0);
  if (count > 0) {
    int64_t i = 0;
    do {
      r3 = UpdateRacerBar(base, hud, uint64_t(i), uint64_t(i), frame, calls, sounds);
      i++;
    } while (int32_t(i) < int32_t(h.racer_count()));
  }
  return r3;
}

uint64_t TickPanel(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                   const menu_sounds::Calls& sounds) {
  ITC_NATIVE_RUNNING("sub_82215488 RaceHud::TickPanel");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const uint32_t mode = Hud(base, hud).mode();
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
