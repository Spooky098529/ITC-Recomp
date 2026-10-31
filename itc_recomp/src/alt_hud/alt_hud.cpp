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
#include "race_setup/race_setup.h"
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

uint32_t Object(uint8_t* base, uint32_t holder) { return guest::LoadU32(base, holder); }

// UiMovies::Find(the movie table, the key 22 at `key`): the value's address.
uint32_t FindMovie(uint8_t* base, uint32_t key) {
  guest::StoreU32(base, key, kMovieId);
  return ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
}

double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(uint64_t(guest::LoadU32(base, address)) << 32 |
                               guest::LoadU32(base, address + 4));
}

void StoreS64(uint8_t* base, uint32_t address, int64_t value) {
  guest::StoreU32(base, address, uint32_t(uint64_t(value) >> 32));
  guest::StoreU32(base, address + 4, uint32_t(uint64_t(value)));
}

// memcpy natively, or through `calls` from the frame `r1` with the return
// address `lr` where its plain C++ refuses.
void Copy(uint8_t* base, const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
          uint32_t source, uint32_t size) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    calls.Call(kMemcpy, r1, lr, destination, source, size);
  }
}

constexpr uint32_t kTen = 0x8200F728;  // the digits' 10.0 (a double) for PowInt
constexpr uint32_t kKmhFactor = 0x8200A510;  // the km/h's factor for the dials (a double)

// The tachometer curves (three, 112 bytes each, from 0x8200FD28), the boost
// curve after them and the table by the record's index (16 bytes an entry:
// word 0 the curve, words 1 to 3 the dial's faces by the power class).
constexpr uint32_t kBoostCurve = 0x8200FE78;
constexpr uint32_t kCurveTable = 0x8200FEE8;

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
  if (peak < Fmuls(stock, Lfs(base, 0x820029F0))) {
    return 0;
  }
  return peak < Fmuls(stock, Lfs(base, 0x82002C40)) ? 1 : 2;
}

uint32_t SetUpGauges(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls,
                     double* f1) {
  ITC_NATIVE_RUNNING("sub_82216C40 AltHud::SetUpGauges");
  const uint32_t frame = sp - 496;
  Frame(base, sp, 496);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t player = guest::LoadU32(base, Object(base, holder) + kPlayer);
  const uint32_t record = player * rh::kRecordBytes + guest::LoadU32(base, rh::kRaceData);
  const uint32_t car = record + 48;
  const uint32_t index = guest::LoadU32(base, car + 12);
  const uint32_t power = PowerClass(base, car, frame, calls);
  Copy(base, calls, frame, 0x82216CB8, frame + 96, kBoostCurve - 336, 112);
  Copy(base, calls, frame, 0x82216CC8, frame + 208, kBoostCurve - 224, 112);
  Copy(base, calls, frame, 0x82216CD8, frame + 320, kBoostCurve - 112, 112);
  const uint32_t curve = guest::LoadU32(base, (index << 4) + kCurveTable);
  const uint32_t face = guest::LoadU32(base, (((index << 2) + power) << 2) + kCurveTable + 4);
  Copy(base, calls, frame, 0x82216D18, Object(base, holder) + kTach, curve * 112 + frame + 96,
       112);
  const double zero = Lfs(base, rh::kZero);
  rh::CurveEval(base, Object(base, holder) + kTach, zero);
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 8);
  guest::StoreU32(base, node + 108, face);
  Copy(base, calls, frame, 0x82216D58, Object(base, holder) + kBoost, kBoostCurve, 112);
  const uint32_t boost = Object(base, holder) + kBoost;
  rh::CurveEval(base, boost, zero);
  *f1 = zero;
  return boost;
}

uint32_t ShowGauge(uint8_t* base, uint32_t slot, uint32_t shown, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217380 AltHud::ShowGauge");
  const uint32_t frame = sp - 128;
  Frame(base, sp, 128);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t digits = ui_movies::FindNode(base, movie, 7, slot + 12);
  const uint32_t label = ui_movies::FindNode(base, movie, 7, slot + 9);
  guest::StoreU32(base, digits + 80, shown);
  guest::StoreU32(base, label + 80, shown);
  return label;
}

uint32_t SetUpExtraGauges(uint8_t* base, uint32_t holder, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822178A0 AltHud::SetUpExtraGauges");
  const uint32_t frame = sp - 160;
  Frame(base, sp, 160);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 92));
  const uint32_t car = guest::LoadU32(base, Object(base, holder) + kCar);
  uint32_t entry = guest::LoadU32(base, car + 5712) + 1824 + 8;
  uint32_t kept = 0;
  guest::StoreU32(base, frame + 88, 0);
  guest::StoreU32(base, frame + 84, 0);
  guest::StoreU32(base, frame + 80, 0);
  for (int k = 0; k < 5; k++, entry += 12) {
    switch (int32_t(guest::LoadU32(base, entry))) {
      case 1:
        kept = 1;
        break;
      case 2:
        guest::StoreU32(base, frame + 80, 1);
        break;
      case 3:
        guest::StoreU32(base, frame + 84, 1);
        break;
      case 4:
        guest::StoreU32(base, frame + 88, 1);
        break;
      default:
        break;
    }
  }
  if (int32_t(guest::LoadU32(base, guest::LoadU32(base, car + 5712) + 12)) == 10) {
    kept = 1;
  }
  guest::StoreU32(base, frame + 96, frame + 80);
  guest::StoreU32(base, frame + 100, frame + 84);
  guest::StoreU32(base, frame + 104, frame + 88);
  // The slots from the last: the first flag set of the three (cleared), or -1.
  for (uint32_t slot = kSlots + 8; int32_t(slot) >= int32_t(kSlots); slot -= 4) {
    guest::StoreU32(base, Object(base, holder) + slot, 0xFFFFFFFFu);
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t flag = guest::LoadU32(base, frame + 96 + 4 * i);
      if (int32_t(guest::LoadU32(base, flag)) == 1) {
        guest::StoreU32(base, guest::LoadU32(base, frame + 96 + 4 * i), 0);
        guest::StoreU32(base, Object(base, holder) + slot, i);
        break;
      }
    }
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t value = guest::LoadU32(base, Object(base, holder) + kSlots + 4 * i);
    ShowGauge(base, i, value < 3 ? 1 : 0, frame);
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 1);
  guest::StoreU32(base, node + 80, kept);
  return node;
}

uint32_t ShowPanels(uint8_t* base, uint32_t holder, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82216D78 AltHud::ShowPanels");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t panel = ui_movies::FindNode(base, movie, kNone, 3);
  guest::StoreU32(base, panel + 80, guest::LoadU32(base, Object(base, holder) + kShown));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 2);
  guest::StoreU32(base, node + 80, 0);
  return node;
}

uint32_t SetUp(uint8_t* base, uint32_t holder, uint32_t player, uint32_t sp, const Calls& calls,
               double* f1) {
  ITC_NATIVE_RUNNING("sub_82217C70 AltHud::SetUp");
  const uint32_t frame = sp - 96;
  Frame(base, sp, 96);
  guest::StoreU32(base, Object(base, holder) + kPlayer, player);
  const uint32_t car = vehicle_accessors::VehicleGet(base, player);
  guest::StoreU32(base, Object(base, holder) + kCar, car);
  alt_hud::SetUpGauges(base, holder, frame, calls, f1);
  SetUpExtraGauges(base, holder, frame);
  return ShowPanels(base, holder, frame);
}

uint32_t SetSpeed(uint8_t* base, uint32_t speed, uint32_t sp, double* f1) {
  ITC_NATIVE_RUNNING("sub_82216E00 AltHud::SetSpeed");
  const uint32_t frame = sp - 160;
  Frame(base, sp, 160);
  const uint32_t found = FindMovie(base, frame + 80);
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
        *f1 = rh::PowInt(base, LoadDouble(base, kTen), place);
        guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(*f1)));
        if (value < int32_t(guest::LoadU32(base, frame + 80))) {
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
  const uint32_t frame = sp - 144;
  Frame(base, sp, 144);
  uint64_t r3 = holder;
  for (uint32_t slot = 0; slot < 3; slot++) {
    switch (guest::LoadU32(base, Object(base, holder) + kSlots + 4 * slot)) {
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

uint32_t SetGear(uint8_t* base, int32_t gear, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217460 AltHud::SetGear");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
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

uint32_t SetTach(uint8_t* base, uint32_t holder, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822173E8 AltHud::SetTach");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  rh::CurveEval(base, Object(base, holder) + kTach, value);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 7);
  Stfs(base, node + 36, Fadds(Lfs(base, Object(base, holder) + 104), Lfs(base, 0x8200F720)));
  return node;
}

uint32_t SetNode25(uint8_t* base, uint32_t word, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822174E0 AltHud::SetNode25");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 25);
  guest::StoreU32(base, node + 80, word);
  return node;
}

uint32_t SetBar(uint8_t* base, uint32_t holder, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822175A0 AltHud::SetBar");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 30);
  Stfs(base, Object(base, holder) + kBar, value);
  const uint32_t object = Object(base, holder);
  const double low = Lfs(base, object + 236);
  if (!(value > low)) {
    guest::StoreU32(base, node + 80, 0);
    return node;
  }
  const double range = Fsubs(Lfs(base, object + 232), low);
  Stfs(base, node + 28, Fdivs(Lfs(base, object + kBar), range));
  return node;
}

uint32_t SetBoost(uint8_t* base, uint32_t holder, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217538 AltHud::SetBoost");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  rh::CurveEval(base, Object(base, holder) + kBoost, value);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 5);
  Stfs(base, node + 36, Lfs(base, Object(base, holder) + 216));
  return node;
}

namespace {

// SetThrottle and SetBrake: node `child`'s x scale (+28) = value.
uint32_t SetScale(uint8_t* base, uint32_t child, double value, uint32_t sp) {
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
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

uint32_t SetSteering(uint8_t* base, double value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822176E8 AltHud::SetSteering");
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  const double scaled = Fmuls(Fneg(value), Lfs(base, 0x82010368));
  guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(scaled)));
  const int64_t x = int64_t(int32_t(188u - guest::LoadU32(base, frame + 80)));
  StoreS64(base, frame + 80, x);
  const double position = Single(double(x));
  const uint32_t node = ui_movies::FindNode(base, movie, kNone, 15);
  Stfs(base, node + 12, position);
  return node;
}

uint32_t Tick(uint8_t* base, uint32_t holder, uint32_t sp, bool native_slots, const Calls& calls,
              double* f1) {
  ITC_NATIVE_RUNNING("sub_82217AC8 AltHud::Tick");
  const uint32_t frame = sp - 128;
  Frame(base, sp, 128);
  if (MenuListPressedX(base, guest::LoadU32(base, holder + 4), frame, native_slots, calls) != 0) {
    const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
    const uint32_t object = Object(base, holder);
    guest::StoreU32(base, object + kShown, guest::LoadU32(base, object + kShown) ^ 1);
    const uint32_t node = ui_movies::FindNode(base, movie, kNone, 3);
    guest::StoreU32(base, node + 80, guest::LoadU32(base, Object(base, holder) + kShown));
  }
  const uint32_t object = Object(base, holder);
  const uint32_t car = guest::LoadU32(base, object + kCar);
  const uint32_t player = guest::LoadU32(base, object + kPlayer);
  const uint32_t input = guest::LoadU32(base, ((player << 2) & 0x3FC) + kInputRecords);
  *f1 = double(Vehicle_GetSpeedKmh(base, car));
  const double speed = Single(*f1 * LoadDouble(base, kKmhFactor));
  guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(speed)));
  SetSpeed(base, guest::LoadU32(base, frame + 80), frame, f1);
  const int32_t int149 = Fctiwz(Lfs(base, car + 6252));
  const int32_t int119 = Fctiwz(Lfs(base, car + 6256));
  const double value = Lfs(base, car + 6260);
  guest::StoreU32(base, frame + 80, uint32_t(int149));
  const uint32_t r6 = guest::LoadU32(base, frame + 80);
  guest::StoreU32(base, frame + 80, uint32_t(int119));
  const uint32_t r5 = guest::LoadU32(base, frame + 80);
  alt_hud::UpdateExtraGauges(base, holder, value, r5, r6, frame, calls);
  SetGear(base, int32_t(guest::LoadU32(base, car + 2252)), frame);
  *f1 = double(vehicle_accessors::VehicleFloat2224(base, car));
  SetTach(base, holder, *f1, frame);
  *f1 = Lfs(base, car + 2420);
  SetBar(base, holder, *f1, frame);
  *f1 = Lfs(base, car + 2944);
  SetBoost(base, holder, *f1, frame);
  SetNode25(base, guest::LoadU32(base, guest::LoadU32(base, car + 5712) + 3892), frame);
  const double scale = Lfs(base, 0x82002BD4);
  static constexpr struct {
    uint32_t offset;
    uint32_t (*set)(uint8_t*, double, uint32_t);
  } kInputs[3] = {{4, SetThrottle}, {6, SetBrake}, {8, SetSteering}};
  for (const auto& in : kInputs) {
    const int64_t word = int16_t(guest::LoadU16(base, input + in.offset));
    StoreS64(base, frame + 80, word);
    *f1 = Fmuls(Single(double(word)), scale);
    in.set(base, *f1, frame);
  }
  return 0;
}

uint64_t RunMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82216C00 AltHud::RunMovie");
  const uint32_t frame = sp - 96;
  Frame(base, sp, 96);
  const uint32_t movie = guest::LoadU32(base, FindMovie(base, frame + 80));
  return calls.Call(kRun, frame, 0x82216C30, movie, 0);
}

std::optional<uint64_t> Destroy(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls) {
  const uint32_t list = guest::LoadU32(base, holder + 4);
  if (!MenuListReleasable(base, list)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82216BA8 AltHud::Destroy");
  const uint32_t frame = sp - 96;
  Frame(base, sp, 96);
  auto remove = [&](uint32_t block, uint32_t lr) -> uint64_t {
    if (memory::crt::OperatorDelete(block)) {
      return block;
    }
    return calls.Call(kOperatorDelete, frame, lr, block);
  };
  MenuListRelease(base, list, frame);
  uint64_t r3 = remove(Object(base, holder), 0x82216BCC);
  const uint32_t again = guest::LoadU32(base, holder + 4);
  if (again != 0) {
    if (!MenuListRelease(base, again, frame)) {
      calls.Call(0x8220BBB0, frame, 0x82216BE0, again);
    }
    r3 = remove(again, 0x82216BE8);
  }
  return r3;
}

}  // namespace alt_hud
