// The alternate HUD's set-ups, setters and tick (docs/functions/alt-hud.md):
// see alt_hud.h.

#include "alt_hud/alt_hud.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "car_physics/car_physics.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "memory/c_runtime_heap.h"
#include "memory/handles.h"
#include "race_setup/race_setup.h"
#include "schema/ai_drivers.h"
#include "schema/alt_hud.h"
#include "schema/car_physics.h"
#include "schema/career.h"
#include "schema/race_hud_object.h"
#include "schema/race_setup.h"
#include "schema/vehicle.h"
#include "ui_movies/game.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace alt_hud {

namespace {

using namespace camera_modes::steps;
namespace rh = race_hud_object;

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

void Frame(uint8_t* base, uint32_t sp, uint32_t bytes) { guest::StoreU32(base, sp - bytes, sp); }

using G = schema::AltHudGlobals;

// The holder's +1004 and its object (+0, loaded at each call).
schema::AltHudHolder Holder(uint8_t* base, uint32_t holder) { return {base, holder}; }
schema::AltHud Object(uint8_t* base, uint32_t holder) { return Holder(base, holder).object(); }

// The extra gauges' slot i (slot0 to slot2, +244 + 4 i), through the view (a
// host object, E5 step 4).
uint32_t Slot(const schema::AltHud& object, uint32_t i) {
  return uint32_t(i == 0 ? object.slot0() : i == 1 ? object.slot1() : object.slot2());
}
void SetSlot(const schema::AltHud& object, uint32_t i, uint32_t value) {
  if (i == 0) {
    object.set_slot0(int32_t(value));
  } else if (i == 1) {
    object.set_slot1(int32_t(value));
  } else {
    object.set_slot2(int32_t(value));
  }
}

// UiMovies::Find(the movie table, the key 22): the value's address. The
// original stored the key in its frame word and passed its address; only
// the lookup read it (E5 step 6: the key by value).
uint32_t FindMovie(uint8_t* base) {
  return ui_movies::PairsFindKey(base, ui_movies::kMovieTable, kMovieId);
}

double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(uint64_t(guest::LoadU32(base, address)) << 32 |
                               guest::LoadU32(base, address + 4));
}

// memcpy natively, or through `calls` from the frame `r1` with the return
// address `lr` where its plain C++ refuses.
void Copy(uint8_t* base, const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
          uint32_t source, uint32_t size) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    calls.Call(kMemcpy, r1, lr, destination, source, size);
  }
}

}  // namespace

uint32_t PowerClass(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82216B08 AltHud::PowerClass");
  const uint32_t frame = sp - 3040;
  Frame(base, sp, 3040);
  const uint32_t block2 = vehicle_accessors::ObjectRecord1888(car);
  const uint32_t data = frame + 1152;
  car_physics::CarDataConstruct(base, data, frame - 96);
  double f1 = 0.0;
  if (!race_setup::CarSetupVehicle(base, car, data, frame - 368, &f1)) {
    calls.Call(kSetupVehicle, frame, 0x82216B40, car, data);
  }
  vehicle::PowerCurveFromEngine(base, frame + 80, frame + 2420, frame - 112);
  const double stock = Lfs(base, block2 + 12);
  const double peak = Lfs(base, frame + 1136);
  if (peak < Fmuls(stock, double(G::power_low(base)))) {
    return 0;
  }
  return peak < Fmuls(stock, double(G::power_high(base))) ? 1 : 2;
}

uint32_t SetUpGauges(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls,
                     double* f1) {
  ITC_NATIVE_RUNNING("sub_82216C40 AltHud::SetUpGauges");
  const uint32_t frame = sp - 496;
  Frame(base, sp, 496);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t player = Object(base, holder).player();
  const uint32_t record =
      player * rh::kRecordBytes + schema::RaceHudObjectGlobals::race_data(base).address;
  const uint32_t car = record + 48;
  const uint32_t index = schema::Car(base, car).car_id();
  const uint32_t power = PowerClass(base, car, frame, calls);
  Copy(base, calls, frame, 0x82216CB8, frame + 96, G::tach_curves_address(), 112);
  Copy(base, calls, frame, 0x82216CC8, frame + 208, G::tach_curves_address() + 112, 112);
  Copy(base, calls, frame, 0x82216CD8, frame + 320, G::tach_curves_address() + 224, 112);
  const uint32_t curve = guest::LoadU32(base, (index << 4) + G::curve_table_address());
  const uint32_t face =
      guest::LoadU32(base, (((index << 2) + power) << 2) + G::curve_table_address() + 4);
  Copy(base, calls, frame, 0x82216D18, Object(base, holder).tach_curve_address(),
       curve * 112 + frame + 96, 112);
  const double zero = double(schema::CareerGlobals::zero(base));
  rh::CurveEval(base, Object(base, holder).tach_curve_address(), zero);
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 8);
  guest::StoreU32(base, node + 108, face);
  Copy(base, calls, frame, 0x82216D58, Object(base, holder).boost_curve_address(),
       G::boost_curve_address(), 112);
  const uint32_t boost = Object(base, holder).boost_curve_address();
  rh::CurveEval(base, boost, zero);
  *f1 = zero;
  return boost;
}

uint32_t ShowGauge(uint8_t* base, uint32_t slot, uint32_t shown, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217380 AltHud::ShowGauge");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t digits = ui_movies::FindNode(base, movie, 7, slot + 12);
  const uint32_t label = ui_movies::FindNode(base, movie, 7, slot + 9);
  guest::StoreU32(base, digits + 80, shown);
  guest::StoreU32(base, label + 80, shown);
  return label;
}

uint32_t SetUpExtraGauges(uint8_t* base, uint32_t holder, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822178A0 AltHud::SetUpExtraGauges");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const schema::Vehicle car(base, Object(base, holder).car().address);
  // The car's five 12-byte entries from choice0_c (+1832), 12 bytes apart.
  uint32_t entry = schema::Car(base, car.car()).choice0_c_address();
  uint32_t kept = 0;
  // The three flags (the original's frame words +80, +84 and +88, walked
  // through a table of their addresses at +96): locals (E5 step 6).
  int32_t flags[3] = {0, 0, 0};
  for (int k = 0; k < 5; k++, entry += 12) {
    switch (int32_t(guest::LoadU32(base, entry))) {
      case 1:
        kept = 1;
        break;
      case 2:
        flags[0] = 1;
        break;
      case 3:
        flags[1] = 1;
        break;
      case 4:
        flags[2] = 1;
        break;
      default:
        break;
    }
  }
  if (int32_t(schema::Car(base, car.car()).car_id()) == 10) {
    kept = 1;
  }
  // The slots from the last: the first flag set of the three (cleared), or -1.
  for (int32_t slot = 2; slot >= 0; slot--) {
    SetSlot(Object(base, holder), uint32_t(slot), 0xFFFFFFFFu);
    for (uint32_t i = 0; i < 3; i++) {
      if (flags[i] == 1) {
        flags[i] = 0;
        SetSlot(Object(base, holder), uint32_t(slot), i);
        break;
      }
    }
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t value = Slot(Object(base, holder), i);
    ShowGauge(base, i, value < 3 ? 1 : 0, sp);
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 1);
  guest::StoreU32(base, node + 80, kept);
  return node;
}

uint32_t ShowPanels(uint8_t* base, uint32_t holder, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82216D78 AltHud::ShowPanels");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t panel = ui_movies::FindNode(base, movie, kNone, 3);
  guest::StoreU32(base, panel + 80, Object(base, holder).shown());
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 2);
  guest::StoreU32(base, node + 80, 0);
  return node;
}

uint32_t SetUp(uint8_t* base, uint32_t holder, uint32_t player, uint32_t sp, const Calls& calls,
               double* f1) {
  ITC_NATIVE_RUNNING("sub_82217C70 AltHud::SetUp");
  // No frame of its own (E5 step 6): its callees' frames below `sp`.
  Object(base, holder).set_player(player);
  const uint32_t car = vehicle_accessors::VehicleGet(base, player);
  Object(base, holder).set_car(car);
  alt_hud::SetUpGauges(base, holder, sp, calls, f1);
  SetUpExtraGauges(base, holder, sp);
  return ShowPanels(base, holder, sp);
}

uint32_t SetSpeed(uint8_t* base, uint32_t speed, [[maybe_unused]] uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_82216E00 AltHud::SetSpeed");
  const uint32_t found = FindMovie(base);
  const uint32_t movie = guest::LoadU32(base, found);
  if (speed > 999) {
    return found;
  }
  const uint32_t digits = ui_movies::FindNode(base, movie, kNone, 21);
  int32_t i = int32_t(guest::LoadU32(base, digits + 104)) - 1;
  if (i < 0) {
    return digits;
  }
  int32_t value = int32_t(speed);
  uint32_t node = 0;
  for (; i >= 0; i--) {
    node = ui_movies::FindNode(base, movie, kNone, guest::LoadU32(base, digits + 100) + uint32_t(i));
    const int32_t digit = value - (value / 10) * 10;
    if (digit == 0) {
      const int32_t place = int32_t(guest::LoadU32(base, digits + 104) - uint32_t(i)) - 1;
      if (place > 0) {
        *f1 = rh::PowInt(base, LoadDouble(base, G::ten_address()), place);
        // fctiwz through the original's frame word +80: a local.
        const int32_t power = Fctiwz(*f1);
        if (value < power) {
          guest::StoreU32(base, node + 108, 0);
          continue;
        }
      }
    }
    guest::StoreU32(base, node + 108, uint32_t(digit + 1));
    value /= 10;
  }
  return node;
}

uint64_t UpdateExtraGauges(uint8_t* base, uint32_t holder, double value, uint64_t int119,
                           uint64_t int149, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82217A30 AltHud::UpdateExtraGauges");
  // No slots (E5 step 6): the gauges' calls made from `sp` (they read none
  // of the caller's frame).
  const uint32_t frame = sp;
  uint64_t r3 = holder;
  for (uint32_t slot = 0; slot < 3; slot++) {
    switch (Slot(Object(base, holder), slot)) {
      case 0:
        r3 = calls.Call(kGaugeFloat, frame, 0x82217AAC, holder, 0, slot, 0, &value);
        break;
      case 1:
        r3 = calls.Call(kGaugeInt119, frame, 0x82217A98, holder, int119, slot);
        break;
      case 2:
        r3 = calls.Call(kGaugeInt149, frame, 0x82217A84, holder, int149, slot);
        break;
      default:
        break;
    }
  }
  return r3;
}

uint32_t SetGear(uint8_t* base, int32_t gear, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217460 AltHud::SetGear");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 23);
  uint32_t picture;
  if (gear > 0) {
    picture = gear == 6 ? 7 : uint32_t(gear - 1);
  } else {
    picture = gear == 0 ? 5 : 6;
  }
  guest::StoreU32(base, node + 108, picture);
  return node;
}

uint32_t SetTach(uint8_t* base, uint32_t holder, double value, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822173E8 AltHud::SetTach");
  rh::CurveEval(base, Object(base, holder).tach_curve_address(), value);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 7);
  Stfs(base, node + 36,
       Fadds(double(Object(base, holder).tach_output()), double(G::tach_offset(base))));
  return node;
}

uint32_t SetNode25(uint8_t* base, uint32_t word, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822174E0 AltHud::SetNode25");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 25);
  guest::StoreU32(base, node + 80, word);
  return node;
}

uint32_t SetBar(uint8_t* base, uint32_t holder, double value, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822175A0 AltHud::SetBar");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 30);
  Object(base, holder).set_bar(float(value));
  const schema::AltHud object = Object(base, holder);
  const double low = double(object.bar_low());
  if (!(value > low)) {
    guest::StoreU32(base, node + 80, 0);
    return node;
  }
  const double range = Fsubs(double(object.bar_high()), low);
  Stfs(base, node + 28, Fdivs(double(object.bar()), range));
  return node;
}

uint32_t SetBoost(uint8_t* base, uint32_t holder, double value, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217538 AltHud::SetBoost");
  rh::CurveEval(base, Object(base, holder).boost_curve_address(), value);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 5);
  Stfs(base, node + 36, double(Object(base, holder).boost_output()));
  return node;
}

namespace {

// SetThrottle and SetBrake: node `child`'s x scale (+28) = value.
uint32_t SetScale(uint8_t* base, uint32_t child, double value, [[maybe_unused]] uint32_t sp) {
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, child);
  Stfs(base, node + 28, value);
  return node;
}

}  // namespace

uint32_t SetThrottle(uint8_t* base, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217638 AltHud::SetThrottle");
  return SetScale(base, 16, value, sp);
}

uint32_t SetBrake(uint8_t* base, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217690 AltHud::SetBrake");
  return SetScale(base, 17, value, sp);
}

uint32_t SetSteering(uint8_t* base, double value, [[maybe_unused]] uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822176E8 AltHud::SetSteering");
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  const double scaled = Fmuls(Fneg(value), double(G::steering_scale(base)));
  // fctiwz and the doubleword for fcfid through the original's frame word
  // +80: locals (E5 step 6).
  const int64_t x = int64_t(int32_t(188u - uint32_t(Fctiwz(scaled))));
  const double position = Single(double(x));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 15);
  Stfs(base, node + 12, position);
  return node;
}

uint32_t Tick(uint8_t* base, uint32_t holder, uint32_t sp, bool native_slots, const Calls& calls,
              double* f1) {
  ITC_NATIVE_RUNNING("sub_82217AC8 AltHud::Tick");
  // No frame (E5 step 6): its words were fctiwz and fcfid temporaries, now
  // locals; its callees' frames below `sp`.
  const uint32_t frame = sp;
  if (MenuListPressedX(base, Holder(base, holder).menu_list().address, frame, native_slots, calls) !=
      0) {
    const uint32_t movie = guest::LoadU32(base, FindMovie(base));
    const schema::AltHud object = Object(base, holder);
    object.set_shown(object.shown() ^ 1);
    const uint32_t node = ui_movies::FindNode(base, movie, kNone, 3);
    guest::StoreU32(base, node + 80, Object(base, holder).shown());
  }
  const schema::AltHud object = Object(base, holder);
  const uint32_t car = object.car().address;
  const schema::Vehicle vehicle(base, car);
  const schema::VehicleStatus status(base, vehicle.status_address());
  const uint32_t player = object.player();
  const schema::CarInput input(
      base, guest::LoadU32(base, ((player << 2) & 0x3FC) + G::input_records_address()));
  *f1 = double(Vehicle_GetSpeedKmh(base, car));
  const double speed = Single(*f1 * LoadDouble(base, G::kmh_factor_address()));
  SetSpeed(base, uint32_t(Fctiwz(speed)), frame, f1);
  const int32_t int149 = Fctiwz(double(status.water_temperature()));
  const int32_t int119 = Fctiwz(double(status.oil_temperature()));
  const double value = double(status.engine_output());
  const uint32_t r6 = uint32_t(int149);
  const uint32_t r5 = uint32_t(int119);
  alt_hud::UpdateExtraGauges(base, holder, value, r5, r6, frame, calls);
  SetGear(base, vehicle.gear(), frame);
  *f1 = double(vehicle_accessors::VehicleFloat2224(base, car));
  SetTach(base, holder, *f1, frame);
  *f1 = double(vehicle.nitrous_value());
  SetBar(base, holder, *f1, frame);
  *f1 = double(vehicle.boost());
  SetBoost(base, holder, *f1, frame);
  SetNode25(base, schema::Car(base, vehicle.car()).unknown_3892(), frame);
  const double scale = double(G::input_scale(base));
  static constexpr struct {
    uint16_t (schema::CarInput::*get)() const;
    uint32_t (*set)(uint8_t*, double, uint32_t);
  } kInputs[3] = {{&schema::CarInput::throttle, SetThrottle},
                  {&schema::CarInput::brake, SetBrake},
                  {&schema::CarInput::steering, SetSteering}};
  for (const auto& in : kInputs) {
    const int64_t word = int16_t((input.*in.get)());
    *f1 = Fmuls(Single(double(word)), scale);
    in.set(base, *f1, frame);
  }
  return 0;
}

uint64_t RunMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82216C00 AltHud::RunMovie");
  // No frame (E5 step 6): the key by value, UiMovie::Run called from `sp`.
  const uint32_t movie = guest::LoadU32(base, FindMovie(base));
  return calls.Call(kRun, sp, 0x82216C30, movie, 0);
}

std::optional<uint64_t> Destroy(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls) {
  const uint32_t list = Holder(base, holder).menu_list().address;
  if (!MenuListReleasable(base, list)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82216BA8 AltHud::Destroy");
  // No slots (E5 step 6): its calls made from `sp`.
  const uint32_t frame = sp;
  auto remove = [&](uint32_t block, uint32_t lr) -> uint64_t {
    if (memory::crt::OperatorDelete(block)) {
      return block;
    }
    return calls.Call(kOperatorDelete, frame, lr, block);
  };
  MenuListRelease(base, list, frame);
  // operator delete at 0x82216BCC: the object in host memory (E5 step 4,
  // RaceScreens::Make1004's New) deleted; r3 the block, as the native
  // operator delete leaves it.
  const schema::AltHud object = Object(base, holder);
  uint64_t r3;
  if (memory::IsHandle(object.address)) {
    object.Delete();
    r3 = object.address;
  } else {
    r3 = remove(object.address, 0x82216BCC);
  }
  const uint32_t again = Holder(base, holder).menu_list().address;
  if (again != 0) {
    if (!MenuListRelease(base, again, frame)) {
      calls.Call(0x8220BBB0, frame, 0x82216BE0, again);
    }
    r3 = remove(again, 0x82216BE8);
  }
  return r3;
}

}  // namespace alt_hud
