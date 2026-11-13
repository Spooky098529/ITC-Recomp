// The front end's power and torque graph as plain C++
// (docs/functions/power-graph.md); see power_graph.h. Built with strict
// floating point (CMakeLists.txt): each step is the translation's, in its
// order, so results match it bit for bit.

#include "power_graph/power_graph.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "car_makers/car_makers.h"
#include "car_physics/car_physics.h"
#include "career/career.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "minimap/minimap.h"
#include "race_overlays/race_overlays.h"
#include "c_runtime/strings.h"
#include "car_specs/car_specs.h"
#include "race_setup/race_setup.h"
#include "strings_and_font/steps.h"
#include "ui_movies/game.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "xaudio/ppc_float.h"

#include <bit>
#include <cmath>

namespace power_graph {

namespace {

// The image's constants, loaded as the originals' lfs do.
constexpr uint32_t kZero = 0x82000A34;         // 0.0
constexpr uint32_t kOne = 0x82000A30;          // 1.0
constexpr uint32_t kStep = 0x82009ECC;         // 6.4, a point's width
constexpr uint32_t kToKgfM = 0x82009EC0;       // 1 / 9.80665
constexpr uint32_t kRpmStep = 0x82009EC4;      // 400.0
constexpr uint32_t kToPs = 0x82009EC8;         // 1 / 716.2
constexpr uint32_t kTorqueScale = 0x8200822C;  // 3.2
constexpr uint32_t kPowerScale = 0x82009EBC;   // 0.32
constexpr uint32_t kCountScale = 0x82002CE8;   // 0.0025
// The empty destructor the sprites' arrays are destroyed with.
constexpr uint32_t kEmptyDestructor = 0x82106540;
// Primitive::RecordTransform's list.
constexpr uint32_t kTransformList = 0x826D83D4;
// A line's slots: the header, the transform's five, [13], the vertices'
// header and three, [15].
constexpr uint32_t kLineSlots = 12;

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// std of a sign-extended word.
void StoreS64(uint8_t* base, uint32_t address, int32_t value) {
  guest::StoreU32(base, address, value < 0 ? 0xFFFFFFFFu : 0);
  guest::StoreU32(base, address + 4, uint32_t(value));
}
// fcfid then frsp of a word (the originals' int to float through the frame).
double ToSingle(int32_t value) {
  return double(float(double(int64_t(value))));
}

using strings_and_font::steps::Fctiwz;

}  // namespace

bool LineRecordFlat(uint8_t* base, uint32_t sprite, uint32_t pass, uint32_t key, uint32_t frame,
                    const LineCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8217B508 Line::RecordFlat");
  const auto [list, queue] = calls.list_and_queue(frame);
  guest::StoreU32(base, kLineLists + 0, queue);
  guest::StoreU32(base, kLineLists + 4, list);
  const uint32_t used = guest::LoadU32(base, list + 12);
  if (guest::LoadU32(base, kTransformList) != list || guest::LoadU8(base, list + 52) != 0 ||
      guest::LoadU32(base, command_list::kOpenChunks) != 0 || used + kLineSlots < used ||
      used + kLineSlots >= guest::LoadU32(base, list + 8)) {
    return false;
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  minimap::RecordTransform(base, pass, sprite, frame - minimap::kTransformProduct);
  const uint32_t slot = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, slot + 0, 13);
  guest::StoreU32(base, slot + 8, 1);
  guest::StoreU32(base, slot + 12, 0);
  guest::StoreU32(base, slot + 4, 2);
  uint32_t vertex = command_list::AllocateVertices(base, list, 2, true);
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t from = sprite + 80 + 16 * i;
    Stfs(base, vertex + 0, Lfs(base, from + 0));
    Stfs(base, vertex + 4, Lfs(base, from + 4));
    Stfs(base, vertex + 8, Lfs(base, from + 8));
    const uint32_t color = (guest::LoadU32(base, from + 44) << 24) |
                           (uint32_t(guest::LoadU8(base, from + 35)) << 16) |
                           (uint32_t(guest::LoadU8(base, from + 39)) << 8) |
                           guest::LoadU8(base, from + 43);
    guest::StoreU32(base, vertex + 12, color);
    vertex += 24;
  }
  guest::StoreU32(base, command_list::AllocateSlots(base, list, 1), 15);
  command_list::CloseChunk(base, list);
  const int32_t slots = command_list::ChunkSlots(base, list);
  guest::StoreU16(base, header + 0, 0);
  guest::StoreU16(base, header + 2, 0);
  guest::StoreU32(base, header + 4, 0);
  guest::StoreU32(base, header + 12, 0);
  guest::StoreU32(base, header + 8, uint32_t(slots - 1));
  calls.add(frame, guest::LoadU32(base, kLineLists), header, key);
  return true;
}

int32_t SegmentCount(uint8_t* base, uint32_t car) {
  const double rev = Lfs(base, car + 1280);
  return Fctiwz(double(float(std::fma(rev, Lfs(base, kCountScale), Lfs(base, kOne)))));
}

bool DrawReady(uint8_t* base, uint32_t car) {
  return SegmentCount(base, car) <= 32;
}

void Draw(uint8_t* base, uint32_t frame, double x, double y, uint32_t car,
          const LineCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8217C708 PowerGraph::Draw");
  const uint32_t torque_set = frame + kTorqueSprites;
  const uint32_t power_set = frame + kPowerSprites;
  for (uint32_t i = 0; i < 32; i++) {
    race_overlays::SpriteResetStyle2(base, torque_set + kSpriteBytes * i);
  }
  for (uint32_t i = 0; i < 32; i++) {
    race_overlays::SpriteResetStyle2(base, power_set + kSpriteBytes * i);
  }

  // The segments' x, y and z (sprites 0 to 30).
  const double zero = Lfs(base, kZero);
  const double step = Lfs(base, kStep);
  for (int32_t i = 0; i < 31; i++) {
    const uint32_t a = torque_set + kSpriteBytes * uint32_t(i);
    const uint32_t b = power_set + kSpriteBytes * uint32_t(i);
    StoreS64(base, frame + 80, i);
    StoreS64(base, frame + 88, i + 1);
    double left = ToSingle(i);
    left = double(float(left * step));
    Stfs(base, a + 80, double(float(left + x)));
    Stfs(base, b + 80, double(float(left + x)));
    double right = ToSingle(i + 1);
    right = double(float(right * step));
    Stfs(base, a + 96, double(float(right + x)));
    Stfs(base, b + 96, double(float(right + x)));
    for (uint32_t corner = 0; corner < 32; corner += 16) {
      Stfs(base, a + 84 + corner, y);
      Stfs(base, b + 84 + corner, y);
      Stfs(base, a + 88 + corner, zero);
      Stfs(base, b + 88 + corner, zero);
    }
  }

  // The 32 points: torque (kgf·m) at +112, power (PS) at +240.
  const double scale = Lfs(base, car + 1420);
  const double to_kgfm = Lfs(base, kToKgfM);
  const double rpm_step = Lfs(base, kRpmStep);
  const double to_ps = Lfs(base, kToPs);
  for (int32_t i = 0; i < 32; i++) {
    double torque = Lfs(base, car + 1292 + 4 * uint32_t(i));
    torque = double(float(torque * scale));
    torque = double(float(torque * to_kgfm));
    Stfs(base, frame + 112 + 4 * uint32_t(i), torque);
    StoreS64(base, frame + 88, i + 1);
    double rpm = ToSingle(i + 1);
    rpm = double(float(rpm * rpm_step));
    guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(rpm)));
    const int32_t whole = int32_t(guest::LoadU32(base, frame + 80));
    StoreS64(base, frame + 96, whole);
    double power = ToSingle(whole);
    power = double(float(power * torque));
    power = double(float(power * to_ps));
    Stfs(base, frame + 240 + 4 * uint32_t(i), power);
    if (power < zero) {
      Stfs(base, frame + 240 + 4 * uint32_t(i), zero);
    }
  }

  // Points 0 to 25: the heights and colors.
  const double torque_scale = Lfs(base, kTorqueScale);
  const double power_scale = Lfs(base, kPowerScale);
  for (uint32_t i = 0; i < 26; i++) {
    const uint32_t a = torque_set + kSpriteBytes * i;
    const uint32_t b = power_set + kSpriteBytes * i;
    const double torque = double(float(Lfs(base, frame + 112 + 4 * i) * torque_scale));
    const double power = double(float(Lfs(base, frame + 240 + 4 * i) * power_scale));
    Stfs(base, a + 100, double(float(y - torque)));
    Stfs(base, a + kSpriteBytes + 84, double(float(y - torque)));
    Stfs(base, b + 100, double(float(y - power)));
    Stfs(base, b + kSpriteBytes + 84, double(float(y - power)));
    for (uint32_t corner = 0; corner < 32; corner += 16) {
      guest::StoreU32(base, b + 112 + corner, 44);
      guest::StoreU32(base, b + 116 + corner, 242);
      guest::StoreU32(base, b + 120 + corner, 242);
      guest::StoreU32(base, b + 124 + corner, 255);
      guest::StoreU32(base, a + 124 + corner, 255);
      guest::StoreU32(base, a + 112 + corner, 198);
      guest::StoreU32(base, a + 116 + corner, 66);
      guest::StoreU32(base, a + 120 + corner, 14);
    }
  }

  // The count, and segments 1 to n - 1.
  const int32_t count = SegmentCount(base, car);
  guest::StoreU32(base, frame + 80, uint32_t(count));
  for (int32_t i = 1; i < count; i++) {
    const uint32_t a = torque_set + kSpriteBytes * uint32_t(i);
    const uint32_t b = power_set + kSpriteBytes * uint32_t(i);
    if (!LineRecordFlat(base, a, kPass, kKey, frame - kLineFrame, calls)) {
      calls.original(a, kTorqueLineReturn);
    }
    if (!LineRecordFlat(base, b, kPass, kKey, frame - kLineFrame, calls)) {
      calls.original(b, kPowerLineReturn);
    }
  }

  // The sprites' destructors: the power set, then the torque set, 31 to 0.
  for (const uint32_t set : {power_set, torque_set}) {
    for (int32_t i = 31; i >= 0; i--) {
      const uint32_t sprite = set + kSpriteBytes * uint32_t(i);
      c_runtime::VectorDestruct(base, sprite + 112, 16, 2, kEmptyDestructor, nullptr, nullptr);
      c_runtime::VectorDestruct(base, sprite + 80, 16, 2, kEmptyDestructor, nullptr, nullptr);
    }
  }
}

namespace {
constexpr uint32_t kMovieTable = 0x826C5CCC;
constexpr uint32_t kFrontEndCars = 0x826D83E0;
constexpr uint32_t kShownAfter = 0x8200829C;  // 22.0
constexpr uint32_t kNoTime = 0x82001BE0;      // AnimationTime's answer for none
constexpr uint32_t kTuningX = 0x8200AD98;     // 1042.0
constexpr uint32_t kTuningY = 0x8200AD9C;     // 520.0
}  // namespace

bool TuningScreenShowsGraph(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* r3,
                            uint32_t* movie_out) {
  ITC_NATIVE_RUNNING("sub_821DF350 TuningScreen::DrawPowerGraph");
  guest::StoreU32(base, frame + 80, 2);
  const uint32_t entry = ui_movies::PairsFind(base, kMovieTable, frame + 80);
  *r3 = entry;
  const uint32_t movie = guest::LoadU32(base, entry);
  *movie_out = movie;
  const int32_t state = int32_t(guest::LoadU32(base, screen + 9652));
  const bool idle = guest::LoadU32(base, screen + 9656) == 0;
  const uint8_t step = guest::LoadU8(base, screen + 9660);
  if (state == 5) {
    *r3 = movie;
    auto time = [&] {
      return double(std::bit_cast<float>(ui_movies::AnimationTime(base, movie, 0xFFFFFFFFu, 5)));
    };
    if (!(time() > Lfs(base, kShownAfter)) && !(time() == Lfs(base, kNoTime))) {
      return false;
    }
    return idle && step <= 2;
  }
  if (state == 6) {
    return idle && step == 2;
  }
  return false;
}

TuningDraw TuningScreenDraw(uint8_t* base, uint32_t screen) {
  TuningDraw draw;
  draw.r3 = guest::LoadU32(base, guest::LoadU32(base, kFrontEndCars));
  draw.x = Lfs(base, kTuningX);
  draw.y = Lfs(base, kTuningY);
  draw.car = screen + 18912;
  return draw;
}

namespace {
constexpr uint32_t kEntryX = 0x8200AD98;  // 1042.0
constexpr uint32_t kEntryY = 0x82001FC0;  // 500.0
// CarData::CarData's frame (below the caller's r1).
constexpr uint32_t kCarDataFrame = 96;
}  // namespace

bool CarListEntrySetUp(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t entry,
                       uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82250670 CarList::DrawEntryPowerGraph");
  if (guest::LoadU32(base, kEntryShown) == 0) {
    return false;
  }
  const uint32_t car = frame + kEntryCar;
  const uint32_t data = frame + kEntryCarData;
  car_makers::GarageCarReset(base, calls, car, frame);
  career::CarSetModelAndClass(base, car, guest::LoadU32(base, entry + 8432), frame);
  car_physics::CarDataConstruct(base, data, frame - kCarDataFrame);
  double f1 = 0.0;
  car_makers::FromGarageCar(base, calls, car, data, frame, &f1);
  return true;
}

TuningDraw CarListEntryDraw(uint8_t* base, uint32_t frame) {
  TuningDraw draw;
  draw.r3 = guest::LoadU32(base, guest::LoadU32(base, kFrontEndCars));
  draw.x = Lfs(base, kEntryX);
  draw.y = Lfs(base, kEntryY);
  draw.car = frame + kEntryCarData;
  return draw;
}

bool CarListCarSetUp(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t car,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82250700 CarList::DrawCarPowerGraph");
  if (guest::LoadU32(base, kEntryShown) == 0) {
    return false;
  }
  const uint32_t data = frame + kEntryCarData;
  car_physics::CarDataConstruct(base, data, frame - kCarDataFrame);
  double f1 = 0.0;
  car_makers::FromGarageCar(base, calls, car, data, frame, &f1);
  return true;
}

namespace {

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kRefresh = 0x820C4998;       // Car::Refresh
constexpr uint32_t kSetupFromCar = 0x820C27E8;  // Vehicle::SetupFromCar
constexpr uint32_t kSumScale = 0x820021E8;
// Flush-to-zero off, as the car's plain C++ expects at entry.
void FlushOff() {
  using Fpscr = rex::platform::FPSCRPlatform;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}
uint32_t Fctiwz32(double value) {
  return uint32_t(Fctiwz(value));
}

}  // namespace

uint32_t SetFromCar(uint8_t* base, uint32_t car, uint32_t tab, uint32_t sp,
                    const SetFromCarCall& call) {
  ITC_NATIVE_RUNNING("sub_82250488 PowerGraph::SetFromCar");
  const uint32_t frame = sp - kSetFromCarFrame;
  guest::StoreU32(base, frame, sp);  // the back chain (stwu)
  const uint32_t record = guest::LoadU32(base, kPowerGraphScreen) + 8;
  const uint32_t object = frame + 3008, data = frame + 1152, curve = frame + 80;
  FlushOff();
  race_setup::CarInit(base, object);
  if (!c_runtime::Memcpy(base, object, car, 1884)) {
    call(kMemcpy, object, car, 1884, frame, 0x822504C4);
  }
  FlushOff();
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, object, &refreshed)) {
    call(kRefresh, object, 0, 0, frame, 0x822504CC);
  }
  FlushOff();
  const uint32_t spec = vehicle_accessors::ObjectRecord1888(object);
  car_physics::CarDataConstruct(base, data, frame - kCarDataFrame);
  double f1 = 0.0;
  if (!car_physics::VehicleSetupFromCar(base, data, object, frame - 368, &f1)) {
    call(kSetupFromCar, data, object, 0, frame, 0x822504EC);
  }
  FlushOff();
  vehicle::PowerCurveFromEngine(base, curve, data + 1268, frame - 112);
  vehicle::PowerCurveShiftPeak(base, curve, 0, frame);
  FlushOff();
  guest::StoreU32(base, record + 80, tab);
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, record + 60 + 4 * i, guest::LoadU32(base, car + 1568 + 4 * i));
  }
  guest::StoreU32(base, record + 0, Fctiwz32(Lfs(base, car + 1892)));
  const double sum = double(float(Lfs(base, data + 400) + Lfs(base, data + 672)));
  guest::StoreU32(base, record + 4,
                  Fctiwz32(double(float(std::fma(sum, Lfs(base, kSumScale), Lfs(base, data))))));
  guest::StoreU32(base, record + 8, guest::LoadU32(base, spec + 76));
  guest::StoreU32(base, record + 12, guest::LoadU32(base, spec + 84));
  guest::StoreU32(base, record + 16, guest::LoadU32(base, spec + 80));
  guest::StoreU32(base, record + 20, guest::LoadU32(base, car + 12));
  guest::StoreU32(base, record + 24, guest::LoadU8(base, spec + 129));
  guest::StoreU32(base, record + 28, guest::LoadU8(base, spec + 130));
  // The switch on s byte +68 (cases 0 and 4 the split-off sub_822505CC).
  static constexpr uint32_t kKinds[8] = {6, 2, 3, 4, 6, 0, 1, 5};
  const uint8_t kind = guest::LoadU8(base, spec + 68);
  if (kind <= 7) {
    guest::StoreU32(base, record + 32, kKinds[kind]);
  }
  guest::StoreU32(base, record + 36, guest::LoadU8(base, spec + 69) != 0 ? 1 : 0);
  guest::StoreU32(base, record + 40, guest::LoadU32(base, spec + 8));
  guest::StoreU32(base, record + 44, Fctiwz32(Lfs(base, curve + 1056)));
  guest::StoreU32(base, record + 48, Fctiwz32(Lfs(base, curve + 1060)));
  Stfs(base, record + 52, double(float(Lfs(base, curve + 1048) * Lfs(base, kToKgfM))));
  guest::StoreU32(base, record + 56, Fctiwz32(Lfs(base, curve + 1052)));
  return curve;
}

}  // namespace power_graph
