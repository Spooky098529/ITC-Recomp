// The AI drivers (docs/functions/ai-drivers.md): see ai_drivers.h.

#include "ai_drivers/ai_drivers.h"

#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "car_physics/float_ops.h"
#include "hooks/vehicle.h"
#include "strings_and_font/steps.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

#include <cmath>
#include <cstring>

namespace ai_drivers {

using namespace car_physics::ops;

namespace {

constexpr uint32_t kHundred = 0x82002A0C;      // 100.0
constexpr uint32_t kFiveHundred = 0x82001FC0;  // 500.0
constexpr uint32_t kPi = 0x82002240;
constexpr uint32_t kMinusPi = 0x820029C4;
constexpr uint32_t kTwoPi = 0x82001FA8;
constexpr uint32_t kThirty = 0x82000D44;  // 30.0
constexpr uint32_t kTen = 0x82001FD8;     // 10.0
constexpr uint32_t kHalf = 0x82001BE4;    // 0.5
constexpr uint32_t kTenth = 0x82001FBC;   // 0.1
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRaceMode = 0xA3AF4;     // the race data's word: the mode
constexpr uint32_t kRaceWord = 0xA3AF0;     // the race data's word AiSkill::Load reads
constexpr uint32_t kRaceState = 0x826D82E8;  // +6100 read by RaceAllowsSides
constexpr uint32_t kInputs = 0x826C5B30;     // a word a racer: its input (probably)
constexpr uint32_t kSectionLists = 0x82008A08;
constexpr uint32_t kMinusOne = 0x82001BE0;  // -1.0
constexpr uint32_t kOtherGap = 0x820027A4;  // a reset record's gap
constexpr uint32_t kTwenty = 0x82002568;    // 20.0
constexpr uint32_t kFifth = 0x82002208;     // 0.2
constexpr uint32_t kRace = 0x826D82F4;      // the race: +2308 its racers
// The path's normalizers (+5532): by lane (mode 1), by branch (mode 0).
constexpr uint32_t kNormalizeByLane = 0x8213B428;
constexpr uint32_t kNormalizeByBranch = 0x8213C908;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
int32_t LoadS16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
}
// An lfs then stfs of a constant (its bits: CopyWord, as an lfs then stfs
// of one word keeps them in the translated code).
void StoreConstant(uint8_t* base, uint32_t address, uint32_t constant) {
  CopyWord(base, constant, address);
}
double Fsqrts(double a) {
  return double(float(std::sqrt(a)));
}

}  // namespace

// --- Set-up and clears ---

void InitPath(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213AD48 AiDriver::InitPath");
  guest::StoreU16(base, driver + 4, 0);
  const double one = Lfs(base, kOne);
  guest::StoreU16(base, driver + 6, 0);
  Stfs(base, driver + 60, one);
  Stfs(base, driver + 76, one);
  Stfs(base, driver + 92, one);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t entry = driver + 528 + 64 * i;
    Stfs(base, entry + 12, one);
    guest::StoreU16(base, entry + 16, 0);
    guest::StoreU16(base, entry + 18, 0);
  }
  Stfs(base, driver + 6108, one);
}

uint64_t OfIndex(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82139728 AiDriver::Of");
  const uint32_t table = Load(base, kDrivers);
  if (table == 0) {
    return 0;
  }
  return uint64_t(index) * kDriverBytes + table;
}

uint64_t Of(uint8_t* base, uint32_t index) {
  return OfIndex(base, Load(base, index));
}

void ClearCarInput(uint8_t* base, uint32_t input) {
  ITC_NATIVE_RUNNING("sub_820C96E8 CarInput::Clear");
  guest::StoreU16(base, input + 4, 0);
  guest::StoreU16(base, input + 6, 0);
  guest::StoreU16(base, input + 10, 0);
  guest::StoreU16(base, input + 8, 0);
  guest::StoreU32(base, input + 0, 0);
  guest::StoreU8(base, input + 17, 0);
  guest::StoreU8(base, input + 16, 0);
  guest::StoreU8(base, input + 15, 0);
  guest::StoreU8(base, input + 14, 0);
  guest::StoreU16(base, input + 12, 0);
}

void ClearFollow(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146508 AiDriver::ClearFollow");
  StoreConstant(base, driver + 12, kFiveHundred);
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  guest::StoreU16(base, driver + 8, 0xFFFF);
  guest::StoreU32(base, driver + 16, 0);
  guest::StoreU32(base, driver + 4, 0xFFFFFFFFu);
  const uint32_t plan = driver + 32;
  guest::StoreU32(base, plan + 0, 0);
  guest::StoreU32(base, plan + 4, 0);
  guest::StoreU16(base, plan + 8, 0);
  guest::StoreU32(base, plan + 10, 0xFFFFFFFFu);
  Stfs(base, plan + 16, zero);
  Stfs(base, plan + 20, zero);
  Stfs(base, plan + 24, zero);
  Stfs(base, plan + 28, one);
  Stfs(base, plan + 32, zero);
  Stfs(base, plan + 36, zero);
  Stfs(base, plan + 40, zero);
  Stfs(base, plan + 44, one);
}

void ClearOtherIndexes(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82144ED8 AiDriver::ClearOtherIndexes");
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU16(base, driver + 202 + 40 * i, 0);
  }
}

void ClearCorners(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C720 AiPath::ClearCorners");
  guest::StoreU16(base, path + 5162, 0);
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t record = path + 5164 + 44 * i;
    Stfs(base, record + 16, one);
    guest::StoreU32(base, record + 0, 0);
    Stfs(base, record + 20, zero);
    guest::StoreU32(base, record + 4, 0);
    Stfs(base, record + 28, zero);
    guest::StoreU16(base, record + 8, 0);
    Stfs(base, record + 32, zero);
    guest::StoreU16(base, record + 10, 0);
    guest::StoreU16(base, record + 12, 0);
    guest::StoreU16(base, record + 14, 0);
    guest::StoreU32(base, record + 24, 0);
    guest::StoreU8(base, record + 36, 0);
    guest::StoreU8(base, record + 37, 0);
    guest::StoreU32(base, record + 40, 0);
  }
}

void ClearCornerKind(uint8_t* base, uint32_t corner) {
  ITC_NATIVE_RUNNING("sub_8213C710 AiPath::ClearCornerKind");
  guest::StoreU32(base, corner + 40, 0);
}

void ResetSkill(uint8_t* base, uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F698 AiSkill::Reset");
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU16(base, skill + 24 + 4 * i, 0);
    guest::StoreU8(base, skill + 26 + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU16(base, skill + 4 + 2 * i, 0);
  }
  guest::StoreU16(base, skill + 10, 0);
  StoreConstant(base, skill + 20, kHundred);
  guest::StoreU16(base, skill + 4, 500);
  guest::StoreU16(base, skill + 6, 80);
  guest::StoreU16(base, skill + 8, uint16_t(-100));
}

// --- Small reads and steps ---

void WrapAngle(uint8_t* base, uint32_t angle) {
  ITC_NATIVE_RUNNING("sub_82145408 Angle::Wrap");
  const double a = Lfs(base, angle);
  if (a > Lfs(base, kPi)) {
    Stfs(base, angle, Fsubs(a, Lfs(base, kTwoPi)));
  } else if (a < Lfs(base, kMinusPi)) {
    Stfs(base, angle, Fadds(a, Lfs(base, kTwoPi)));
  }
}

uint32_t TimeFromTicks(uint32_t ticks) {
  ITC_NATIVE_RUNNING("sub_82101760 RaceTime::FromTicks");
  const uint32_t sign = uint32_t(int32_t(ticks) >> 31);
  uint32_t rest = (ticks ^ sign) - sign;
  const uint32_t minutes = (rest / 1800) & 0x7FFF;
  rest -= minutes * 1800;
  const uint32_t seconds = (rest / 30) & 0x3F;
  rest -= seconds * 30;
  const uint32_t ms = (rest * 1000) / 30;
  return (sign & 0x80000000u) | (minutes << 16) | (seconds << 10) | (ms & 0x3FF);
}

uint32_t SetFlag2(uint8_t* base, uint32_t driver, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_82138D20 AiDriver::SetFlag2");
  const bool set = Load(base, word) != 0;
  const uint32_t flags = Load(base, driver + 16);
  guest::StoreU32(base, driver + 16, set ? flags | 2 : flags & ~2u);
  const uint32_t vehicle = Load(base, driver + 160);
  if (vehicle != 0) {
    vehicle_accessors::VehicleSetObjectHalf114(base, vehicle, Load(base, word));
  }
  return 1;
}

uint32_t IsFree(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821464B8 AiDriver::IsFree");
  const uint32_t vehicle = Load(base, driver + 160);
  if (vehicle == 0) {
    return 0;
  }
  if ((Load(base, vehicle + 6148) & 129) != 0 && (Load(base, driver + 16) & 2) == 0) {
    return 0;
  }
  const uint32_t state = Load(base, driver + 32);
  return state == 0 || state == 6 ? 1 : 0;
}

double SpeedOverDistance(uint8_t* base, uint32_t driver, uint32_t distance) {
  ITC_NATIVE_RUNNING("sub_821450E0 AiDriver::SpeedOverDistance");
  double speed = Lfs(base, driver + 100);
  const double factor = Lfs(base, driver + 136);
  const double hundred = Lfs(base, kHundred);
  if (speed > hundred) {
    speed = Fdivs(hundred, speed);
  } else {
    speed = Lfs(base, kOne);
  }
  return Fmuls(Fmuls(Lfs(base, distance), speed), factor);
}

double CornerFactor(uint8_t* base, uint32_t driver, uint32_t curvature, uint32_t width) {
  ITC_NATIVE_RUNNING("sub_821453A0 AiDriver::CornerFactor");
  double c = Lfs(base, curvature);
  const double least = Lfs(base, kThirty);
  double f1 = least;
  const double ten = Lfs(base, kTen);
  if (c > ten) {
    c = Fsubs(c, ten);
    const double f11 = Lfs(base, driver + 140);
    const double f10 = Lfs(base, driver + 132);
    c = Fsqrts(c);
    c = Fmuls(c, f11);
    c = Fmuls(c, f10);
    f1 = Fmadds(c, ten, least);
  }
  double w = Lfs(base, width);
  const double room = Lfs(base, driver + 128);
  if (w < room) {
    w = Fsubs(room, w);
    f1 = Fnmsubs(w, ten, f1);
  }
  if (f1 < least) {
    f1 = least;
  }
  return f1;
}

uint32_t ReadCar(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82144F50 AiDriver::ReadCar");
  const uint32_t vehicle = Load(base, driver + 160);
  if (vehicle == 0) {
    return 0;
  }
  double width = Fneg(Lfs(base, vehicle + 5144));
  double f0 = Lfs(base, vehicle + 5160);
  if (width < f0) {
    width = f0;
  }
  Stfs(base, driver + 120, width);
  const double back = Fneg(Lfs(base, vehicle + 5136));
  double length = Lfs(base, vehicle + 5152);
  if (!(back < length)) {
    length = back;
  }
  const double f9 = Fmuls(width, width);
  Stfs(base, driver + 116, width);
  Stfs(base, driver + 124, length);
  const double f13 = Lfs(base, 0x82002A10);
  Stfs(base, driver + 132, f13);
  const double f11 = Lfs(base, 0x82002CB8);
  double diagonal = Fmadds(length, length, f9);
  Stfs(base, driver + 136, f11);
  const double f10 = Lfs(base, 0x82002A14);
  diagonal = Fsqrts(diagonal);
  Stfs(base, driver + 128, diagonal);
  const double half = Lfs(base, kHalf);
  f0 = Lfs(base, vehicle + 3900);
  f0 = Fmuls(f0, half);
  f0 = Fmadds(f0, f10, f13);
  Stfs(base, driver + 132, f0);
  f0 = Lfs(base, 0x82001FD4);
  double f13b = Lfs(base, vehicle + 3444);
  f13b = Fdivs(f0, f13b);
  f0 = Lfs(base, 0x82002A2C);
  f0 = Fmadds(f13b, half, f0);
  const double one = Lfs(base, kOne);
  Stfs(base, driver + 140, f0);
  if (f0 > one) {
    Stfs(base, driver + 140, one);
  }
  double sum = Lfs(base, vehicle + 3512);
  const double f12 = Lfs(base, vehicle + 3508);
  sum = Fadds(sum, f12);
  f0 = Lfs(base, 0x82002204);
  Stfs(base, driver + 136, Fmadds(sum, f0, f11));
  return 1;
}

void UpdateHalfWidth(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145230 AiDriver::UpdateHalfWidth");
  const double cos = c_runtime::Cos(Lfs(base, driver + 148));
  const double sin = c_runtime::Sin(Lfs(base, driver + 148));
  const double c = Fabs(double(float(cos)));
  const double across = Lfs(base, driver + 120);
  const double s = Fabs(double(float(sin)));
  const double along = Lfs(base, driver + 124);
  Stfs(base, driver + 116, Fmadds(along, s, Fmuls(across, c)));
}

uint32_t TargetPoint(uint8_t* base, uint32_t path, double ahead, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213B4D8 AiPath::TargetPoint");
  const int32_t count = LoadS16(base, path + 5136);
  const uint32_t file = Load(base, track_route::kRoute);
  const int32_t k = LoadS16(base, path + 5142);
  if (!(k + 1 < count)) {
    return 0;
  }
  const double length = Lfs(base, file + 8);
  const int32_t k2 = LoadS16(base, path + 5142);
  const int32_t at = LoadS16(base, path + 5138) + k2;
  const int32_t next = at + 1;
  const uint32_t a = path + uint32_t(at - (at / 80) * 80) * 64;
  const uint32_t b = path + uint32_t(next - (next / 80) * 80) * 64;
  double t = double(float(double(int64_t(k))));
  t = Fnmsubs(t, length, ahead);
  double f0 = Fdivs(t, length);
  double f13 = Lfs(base, kZero);
  if (f0 < f13) {
    f0 = f13;
    f13 = Lfs(base, kOne);
  } else {
    const double one = Lfs(base, kOne);
    if (f0 < one) {
      f13 = Fsubs(one, f0);
    } else {
      f0 = one;
    }
  }
  for (uint32_t i = 0; i < 3; i++) {
    const double from = Fmuls(Lfs(base, a + 16 + 4 * i), f13);
    Stfs(base, out + 4 * i, Fmadds(Lfs(base, b + 16 + 4 * i), f0, from));
  }
  return 1;
}

// --- The skill ---

void ClearSkill(uint8_t* base, uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F700 AiSkill::Clear");
  ResetSkill(base, skill);
  guest::StoreU32(base, skill + 0, 0);
  guest::StoreU32(base, skill + 36, 0);
}

namespace {
void ClearTriggers(uint8_t* base, uint32_t skill) {
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU16(base, skill + 24 + 4 * i, 0);
    guest::StoreU8(base, skill + 26 + 4 * i, 0);
  }
}
}  // namespace

void SetSkillSource(uint8_t* base, uint32_t skill, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_8213F900 AiSkill::SetSource");
  ClearTriggers(base, skill);
  ResetSkill(base, skill);
  guest::StoreU32(base, skill + 36, 0);
  guest::StoreU32(base, skill + 0, source);
  if (source != 0) {
    ResetSkill(base, skill);
  }
}

void LoadSkill(uint8_t* base, uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F730 AiSkill::Load");
  if (Load(base, skill + 0) != 0) {
    for (uint32_t i = 0; i < 14; i += 2) {
      const uint32_t source = Load(base, skill + 0);
      guest::StoreU16(base, skill + 4 + i, guest::LoadU16(base, source + i));
    }
    ClearTriggers(base, skill);
  } else {
    ResetSkill(base, skill);
    guest::StoreU16(base, skill + 6, 100);
    if (Load(base, Load(base, kRaceData) + kRaceWord) == 0) {
      guest::StoreU16(base, skill + 10, uint16_t(-5));
    }
  }
  StoreConstant(base, skill + 20, kHundred);
}

// --- Speeds and steering ---

double LimitSpeed(uint8_t* base, uint32_t driver, uint32_t speed, uint32_t curvature,
                  uint32_t width, uint32_t slices, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139248 AiDriver::LimitSpeed");
  double f1 = CornerFactor(base, driver, curvature, width);
  const int32_t n = int32_t(Load(base, slices));
  double limit = Fmuls(Lfs(base, speed), f1);
  if (n > 0) {
    const double length = Lfs(base, Load(base, track_route::kRoute) + 8);
    const double count = double(float(double(int64_t(n))));
    Stfs(base, frame + 80, Fmuls(length, count));
    f1 = SpeedOverDistance(base, driver, frame + 80);
    limit = Fadds(f1, limit);
  }
  if (Lfs(base, driver + 6088) > limit) {
    Stfs(base, driver + 6088, limit);
  }
  return f1;
}

void LimitByCurve(uint8_t* base, uint32_t driver, uint32_t table, uint32_t fraction,
                  uint32_t index, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821390D0 AiDriver::LimitByCurve");
  const int32_t i = int32_t(Load(base, index));
  if (i < 0) {
    return;
  }
  if (!(i < LoadS16(base, Load(base, table + 0)))) {
    return;
  }
  if (record == 0) {
    return;
  }
  const uint32_t entries = Load(base, table + 4);
  const double at = Lfs(base, driver + 144);
  const double entry = Lfs(base, entries + uint32_t(i) * 8 + 4);
  const double past = Fsubs(at, entry);
  const double zero = Lfs(base, kZero);
  const double range = past < zero ? Lfs(base, record + 32) : Lfs(base, record + 36);
  const double span = Fsubs(range, entry);
  if (!(Fmuls(span, past) > zero)) {
    return;
  }
  const double part = Fdivs(past, span);
  const double one = Lfs(base, kOne);
  if (part < one) {
    if (part > Lfs(base, fraction)) {
      Stfs(base, fraction, part);
    }
  } else {
    Stfs(base, fraction, one);
  }
}

void EaseThrottleForSteer(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139B98 AiDriver::EaseThrottleForSteer");
  double steer = Lfs(base, driver + 152);
  const double zero = Lfs(base, kZero);
  if (steer < zero) {
    steer = Fneg(steer);
  }
  steer = Fmuls(steer, Lfs(base, kTenth));
  if (!(steer > zero)) {
    return;
  }
  const double throttle = Lfs(base, driver + 6140);
  if (throttle < steer) {
    Stfs(base, driver + 6140, zero);
  } else {
    Stfs(base, driver + 6140, Fsubs(throttle, steer));
  }
}

// --- The path's corners and points ---

namespace {
// The first corner record before the first with a positive offset whose
// kind is in [low, high]: 1, else 0.
uint32_t KindAhead(uint8_t* base, uint32_t path, int32_t low, int32_t high) {
  const int32_t count = LoadS16(base, path + 5162);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t record = path + 5164 + 44 * uint32_t(i);
    if (LoadS16(base, record + 10) > 0) {
      return 0;
    }
    const int32_t kind = int32_t(Load(base, record + 40));
    if (kind >= low && kind <= high) {
      return 1;
    }
  }
  return 0;
}
}  // namespace

uint32_t HasKind2Ahead(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C148 AiPath::HasKind2Ahead");
  return KindAhead(base, path, 2, 2);
}

uint32_t IsSlow(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C0E8 AiPath::IsSlow");
  return KindAhead(base, path, 2, 3);
}

uint32_t PreferredSections(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_8213C358 AiPath::PreferredSections");
  const int32_t first = int8_t(guest::LoadU8(base, a));
  if (uint32_t(first - 4) > 5) {
    return 0;
  }
  const int32_t second = int8_t(guest::LoadU8(base, b));
  const uint32_t which = uint32_t(first - 4) * 2 + uint32_t(second);
  if (which > 11) {
    return 0;
  }
  return kSectionLists + which * 64;
}

uint32_t RaceAllowsSides(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8213D318 AiPath::RaceAllowsSides");
  const int32_t mode = int32_t(Load(base, Load(base, kRaceData) + kRaceMode));
  if (mode == 4 || mode == 7 || mode == 8) {
    return 1;
  }
  if (mode == 5) {
    const int32_t state = int32_t(Load(base, Load(base, kRaceState) + 6100));
    return state == 1 || state == 3 ? 1 : 0;
  }
  return 0;
}

namespace {
// The ring entry (64 bytes from the path's +16) `at` (start + i), modulo 80
// as the original's (a negative remainder below the path).
uint32_t RingEntry(uint32_t path, int32_t at) {
  return path + uint32_t(at - (at / 80) * 80) * 64;
}
}  // namespace

void RefreshPoints(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213B5D8 AiPath::RefreshPoints");
  for (int32_t i = 1; i < LoadS16(base, path + 5140); i++) {
    const uint32_t entry = RingEntry(path, LoadS16(base, path + 5138) + i);
    const uint32_t record = Load(base, entry + 68);
    const double offset = Lfs(base, entry + 40);
    const double cx = Lfs(base, record + 0);
    Stfs(base, entry + 16, Fmadds(Lfs(base, record + 36), offset, cx));
    CopyWord(base, record + 4, entry + 20);
    const double dz = Lfs(base, record + 40);
    Stfs(base, entry + 24, Fmadds(dz, offset, Lfs(base, record + 8)));
  }
}

uint32_t ResetPath(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213D438 AiPath::Reset");
  guest::StoreU16(base, path + 5142, 1);
  guest::StoreU16(base, path + 5136, 0);
  const double one = Lfs(base, kOne);
  guest::StoreU16(base, path + 5138, 0);
  const double zero = Lfs(base, kZero);
  guest::StoreU16(base, path + 5140, 0);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t entry = path + 16 + 64 * i;
    Stfs(base, entry + 0, zero);
    Stfs(base, entry + 4, zero);
    guest::StoreU32(base, entry + 20, 0xFFFFFFFFu);
    Stfs(base, entry + 8, zero);
    guest::StoreU32(base, entry + 52, 0);
    Stfs(base, entry + 12, one);
    Stfs(base, entry + 24, zero);
    Stfs(base, entry + 48, zero);
    guest::StoreU32(base, entry + 16, 0xFFFFFFFFu);  // the route position (-1, -1)
    Stfs(base, entry + 28, zero);
    Stfs(base, entry + 32, zero);
    Stfs(base, entry + 36, zero);
    Stfs(base, entry + 40, zero);
    Stfs(base, entry + 44, zero);
  }
  ClearCorners(base, path);
  guest::StoreU16(base, path + 5144, 0);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU16(base, path + 5146 + 2 * i, 0xFFFF);
  }
  const uint32_t driver = Load(base, path + 0);
  if (driver != 0) {
    ClearOtherIndexes(base, driver);
  }
  return driver;
}

void SetLaneOffset(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8213B390 AiPathEntry::SetLaneOffset");
  const uint32_t record = Load(base, entry + 52);
  const int32_t lane = int32_t(Load(base, entry + 20));
  const int32_t lanes = guest::LoadU8(base, record + 56) & 0xF;
  if (lane < 0) {
    guest::StoreU32(base, entry + 20, 0);
  } else if (!(lane < lanes)) {
    guest::StoreU32(base, entry + 20, lanes > 1 ? uint32_t(lanes - 1) : 0);
  }
  const int32_t kept = int32_t(Load(base, entry + 20));
  const double width = Lfs(base, record + 12);
  const double half = Lfs(base, kHalf);
  const double n = double(float(double(int64_t(guest::LoadU8(base, record + 56) & 0xF))));
  const double at = double(float(double(int64_t(kept))));
  const double from = Fnmsubs(Fsubs(n, Lfs(base, kOne)), half, at);
  const double offset = Fmuls(Fdivs(from, n), width);
  Stfs(base, entry + 24, offset);
  Stfs(base, entry + 48, offset);
}

// --- The other racers ---

void UpdateOtherFlags(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82145450 AiOther::UpdateFlags");
  uint32_t flags = guest::LoadU16(base, record + 34) & ~6u;
  const int32_t state = LoadS16(base, record + 24);
  guest::StoreU16(base, record + 34, uint16_t(flags));
  if (state == 3) {
    flags |= 8;
    guest::StoreU16(base, record + 34, uint16_t(flags));
    if ((flags & 0x10) == 0) {
      return;
    }
    flags = (flags | 4) & ~0x10u;
  } else if (state == 1) {
    flags |= 16;
    guest::StoreU16(base, record + 34, uint16_t(flags));
    if ((flags & 8) == 0) {
      return;
    }
    flags = (flags | 2) & ~8u;
  } else {
    return;
  }
  guest::StoreU16(base, record + 34, uint16_t(flags));
}

void UpdateOtherTimer(uint8_t* base, uint32_t record, uint32_t index, uint32_t limit) {
  ITC_NATIVE_RUNNING("sub_821456A0 AiOther::UpdateTimer");
  const uint32_t timer = guest::LoadU16(base, record + 30);
  const double gap = Lfs(base, record + 0);
  const double thirty = Lfs(base, kThirty);
  guest::StoreU16(base, record + 30, uint16_t(timer - 1));
  if (!(gap < thirty)) {
    return;
  }
  if (!(gap > Lfs(base, limit))) {
    return;
  }
  const uint32_t input = Load(base, kInputs + (Load(base, index) & 0xFF) * 4);
  if ((Load(base, input) & 0x20000) == 0) {
    return;
  }
  guest::StoreU16(base, record + 30, 120);
}

namespace {
// An other-racer record reset (ClearOthers' and ResetOthers' stores, in
// their order).
void ResetOtherRecord(uint8_t* base, uint32_t record, double gap, double zero, double one,
                      double minus_one) {
  Stfs(base, record + 0, gap);
  guest::StoreU8(base, record + 32, 0);
  Stfs(base, record + 4, zero);
  guest::StoreU8(base, record + 33, 0);
  Stfs(base, record + 8, zero);
  guest::StoreU16(base, record + 30, 0);
  Stfs(base, record + 16, zero);
  guest::StoreU16(base, record + 24, 0);
  Stfs(base, record + 12, one);
  guest::StoreU16(base, record + 28, 0);
  Stfs(base, record + 20, minus_one);
  guest::StoreU16(base, record + 26, 0xFFFF);
  guest::StoreU16(base, record + 34, 0);
  guest::StoreU16(base, record + 36, 0);
}
}  // namespace

void ClearOthers(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821459B0 AiDriver::ClearOthers");
  const double zero = Lfs(base, kZero);
  Stfs(base, driver + 488, zero);
  guest::StoreU16(base, driver + 484, 0);
  const double f11 = Lfs(base, kMinusOne);
  const double one = Lfs(base, kOne);
  guest::StoreU16(base, driver + 486, 0);
  const double gap = Lfs(base, kOtherGap);
  guest::StoreU16(base, driver + 492, 0);
  guest::StoreU32(base, driver + 496, 0);
  for (uint32_t i = 0; i < 7; i++) {
    ResetOtherRecord(base, driver + 172 + 40 * i, gap, zero, one, f11);
    guest::StoreU32(base, driver + 452 + 4 * i, 0xFFFFFFFFu);
  }
}

namespace {
// Whether racer i is in the driver's mask (+40, sign-extended) and isn't
// the driver itself (+0).
bool InMask(uint8_t* base, uint32_t driver, int32_t i) {
  if (int32_t(Load(base, driver + 0)) == i) {
    return false;
  }
  return ((1u << i) & uint32_t(LoadS16(base, driver + 40))) != 0;
}
}  // namespace

void PickTarget(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145118 AiDriver::PickTarget");
  const double zero = Lfs(base, kZero);
  double best = zero;
  if (!(int32_t(Load(base, driver + 480)) < 0)) {
    return;
  }
  guest::StoreU32(base, driver + 480, 0xFFFFFFFFu);
  for (int32_t i = 0; i < 7; i++) {
    if (!InMask(base, driver, i)) {
      continue;
    }
    const uint32_t record = driver + 172 + 40 * uint32_t(i);
    const double gap = Lfs(base, record);
    const int32_t current = int32_t(Load(base, driver + 480));
    bool take;
    if (gap < zero) {
      take = current < 0 || gap > best;
    } else {
      take = current < 0 || best < zero || gap < best;
    }
    if (take) {
      guest::StoreU32(base, driver + 480, uint32_t(i));
      best = Lfs(base, record);
    }
  }
}

void PickTargetBehind(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821451B8 AiDriver::PickTargetBehind");
  const double zero = Lfs(base, kZero);
  double best = zero;
  guest::StoreU32(base, driver + 480, 0xFFFFFFFFu);
  for (int32_t i = 0; i < 7; i++) {
    if (!InMask(base, driver, i)) {
      continue;
    }
    const uint32_t record = driver + 172 + 40 * uint32_t(i);
    const double gap = Lfs(base, record);
    if (!(gap < zero)) {
      continue;
    }
    if (int32_t(Load(base, driver + 480)) < 0 || gap > best) {
      guest::StoreU32(base, driver + 480, uint32_t(i));
      best = Lfs(base, record);
    }
  }
}

uint32_t KeepsGapFrom(uint8_t* base, uint32_t driver, uint32_t other) {
  ITC_NATIVE_RUNNING("sub_82145048 AiDriver::KeepsGapFrom");
  const uint32_t state = Load(base, driver + 32);
  if (state == 0) {
    return 1;
  }
  const uint32_t index = Load(base, other);
  const uint32_t bit = (index & 0x20) != 0 ? 0 : 1u << (index & 0x1F);
  if ((uint32_t(LoadS16(base, driver + 40)) & bit) != 0) {
    return 0;
  }
  if (state - 2 > 1) {
    return 1;
  }
  if (!(Lfs(base, driver + index * 40 + 172) > Lfs(base, kZero))) {
    return 1;
  }
  const uint32_t theirs = uint32_t(OfIndex(base, index));
  return Load(base, theirs + 32) - 2 > 1 ? 1 : 0;
}

// --- Set-up, modes and the plan ---

void SetParams(uint8_t* base, uint32_t driver, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82138D00 AiDriver::SetParams");
  guest::StoreU32(base, driver + 500, object);
  SetSkillSource(base, driver + 6048, object != 0 ? object + 120 : 0);
}

void SetSkill(uint8_t* base, uint32_t driver, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821393D8 AiDriver::SetSkill");
  LoadSkill(base, driver + 6048);
  const uint32_t value = Load(base, word);
  const uint32_t vehicle = Load(base, driver + 160);
  guest::StoreU32(base, driver + 6084, value);
  const int32_t skill = LoadS16(base, driver + 6052);
  Stfs(base, driver + 12, double(float(double(int64_t(skill)))));
  if (vehicle != 0) {
    StoreConstant(base, vehicle + 2432, kOne);
  }
}

uint32_t ClearPlan(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139820 AiDriver::ClearPlan");
  const uint32_t plan = driver + 32;
  const uint32_t path = driver + 512;
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  guest::StoreU32(base, plan + 10, 0xFFFFFFFFu);
  guest::StoreU32(base, plan + 0, 0);
  guest::StoreU32(base, plan + 4, 0);
  guest::StoreU16(base, plan + 8, 0);
  Stfs(base, plan + 16, zero);
  Stfs(base, plan + 20, zero);
  Stfs(base, plan + 24, zero);
  Stfs(base, plan + 28, one);
  Stfs(base, plan + 32, zero);
  Stfs(base, plan + 36, zero);
  Stfs(base, plan + 40, zero);
  Stfs(base, plan + 44, one);
  if (int32_t(Load(base, path + 5524)) != -1 || int32_t(Load(base, path + 5528)) != -1) {
    guest::StoreU32(base, path + 5524, 0xFFFFFFFFu);
    guest::StoreU32(base, path + 5528, 0xFFFFFFFFu);
    return ResetPath(base, path);
  }
  return path;
}

double LimitSpeedBlend(uint8_t* base, uint32_t driver, uint32_t speed, uint32_t pair,
                       uint32_t t, uint32_t w, uint32_t slices, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139B40 AiDriver::LimitSpeedBlend");
  const double target = Lfs(base, t);
  const double a = Lfs(base, pair);
  uint32_t curvature = pair;
  if (!(a < target)) {
    const double weight = Lfs(base, w);
    curvature = frame + 80;
    const double rest = Fsubs(Lfs(base, kOne), weight);
    Stfs(base, curvature, Fmadds(rest, a, Fmuls(weight, target)));
  }
  return LimitSpeed(base, driver, speed, curvature, pair + 4, slices, frame - 128);
}

uint32_t SetMode(uint8_t* base, uint32_t path, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8213D508 AiPath::SetMode");
  const uint32_t value = Load(base, mode);
  if (int32_t(value) == int32_t(Load(base, path + 5520))) {
    return path;
  }
  guest::StoreU32(base, path + 5520, value);
  const uint32_t again = Load(base, mode);
  if (again == 0) {
    guest::StoreU32(base, path + 5532, kNormalizeByBranch);
  } else if (again == 1) {
    guest::StoreU32(base, path + 5532, kNormalizeByLane);
  }
  return ResetPath(base, path);
}

uint32_t ResetFor(uint8_t* base, uint32_t path, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213E658 AiPath::ResetFor");
  guest::StoreU32(base, path + 0, driver);
  StoreConstant(base, path + 5516, kZero);
  guest::StoreU32(base, path + 5532, kNormalizeByLane);
  guest::StoreU32(base, path + 5524, 0xFFFFFFFFu);
  guest::StoreU32(base, path + 5520, 1);
  guest::StoreU32(base, path + 5528, 0xFFFFFFFFu);
  return ResetPath(base, path);
}

uint32_t NormalizeByLane(uint8_t* base, uint32_t entry, uint32_t position, uint32_t lane,
                         uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_8213B428 AiPath::NormalizeByLane");
  const uint32_t route = track_route::kRoute;
  const uint32_t on = masks == 0
                          ? track_route::NormalizeLane(base, route, position, lane)
                          : track_route::NormalizeLaneMasked(base, route, position, lane, masks);
  if (on == 0) {
    return 0;
  }
  const uint32_t file = Load(base, route);
  const uint32_t section = guest::LoadU16(base, position + 0);
  const uint32_t table = Load(base, file + 12);
  const int32_t slice = LoadS16(base, position + 2);
  const uint32_t record = Load(base, table + section * 4) + uint32_t(slice) * 84 + 124;
  guest::StoreU32(base, entry + 52, record);
  guest::StoreU16(base, entry + 16, guest::LoadU16(base, position + 0));
  guest::StoreU16(base, entry + 18, guest::LoadU16(base, position + 2));
  guest::StoreU32(base, entry + 20, uint32_t(LoadS16(base, lane)));
  SetLaneOffset(base, entry);
  return 1;
}

void UpdateLane(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82144EF8 AiDriver::UpdateLane");
  const uint32_t identity = vehicle_accessors::VehicleIdentity(Load(base, driver + 160));
  const uint32_t lane =
      track_route::LaneAt(base, track_route::kRoute, identity + 16, identity + 4);
  track_route::ClampPosition(base, frame + 80, Load(base, identity + 4));
  const uint32_t clamped = Load(base, frame + 80);
  guest::StoreU16(base, driver + 8, uint16_t(lane));
  guest::StoreU32(base, driver + 4, clamped);
}

// --- The other racers' records and the states ---

uint64_t ResetOthers(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145A48 AiDriver::ResetOthers");
  ClearOthers(base, driver);
  const double minus_one = Lfs(base, kMinusOne);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  const double gap = Lfs(base, kOtherGap);
  uint64_t r3 = driver;
  for (int32_t i = 0; i < 7; i++) {
    if (i == int32_t(Load(base, driver + 0))) {
      continue;
    }
    r3 = OfIndex(base, uint32_t(i));
    const uint32_t record = uint32_t(r3) + Load(base, driver + 0) * 40 + 172;
    ResetOtherRecord(base, record, gap, zero, one, minus_one);
  }
  return r3;
}

void ClearCarState(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146588 AiDriver::ClearCarState");
  const double zero = Lfs(base, kZero);
  Stfs(base, driver + 80, zero);
  Stfs(base, driver + 84, zero);
  Stfs(base, driver + 88, zero);
  CopyWord(base, kOne, driver + 92);
  guest::StoreU32(base, driver + 480, 0);
  for (uint32_t at = 96; at <= 104; at += 4) {
    Stfs(base, driver + at, zero);
  }
  const double far = Lfs(base, kFiveHundred);
  for (uint32_t at = 108; at <= 156; at += 4) {
    Stfs(base, driver + at, zero);
  }
  Stfs(base, driver + 164, far);
  Stfs(base, driver + 168, far);
  ClearOthers(base, driver);
}

void UpdateState4(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82146220 AiDriver::UpdateState4");
  PickTarget(base, driver);
  const uint32_t target = Load(base, driver + 480);
  if (int32_t(target) < 0) {
    return;
  }
  const double gap = Lfs(base, driver + target * 40 + 172);
  if (gap < Lfs(base, kZero)) {
    if (gap < Lfs(base, 0x82008D40)) {
      return;
    }
    StoreConstant(base, speed, kTwenty);
  } else {
    if (gap > Lfs(base, 0x8200257C)) {
      return;
    }
    StoreConstant(base, speed, kFiveHundred);
  }
}

uint64_t UpdateState5(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82146388 AiDriver::UpdateState5");
  PickTargetBehind(base, driver);
  const uint32_t target = Load(base, driver + 480);
  if (int32_t(target) < 0) {
    return driver;
  }
  if (!(Lfs(base, driver + target * 40 + 172) > Lfs(base, 0x82008E48))) {
    return driver;
  }
  const uint64_t theirs = OfIndex(base, target);
  const double their_speed = Lfs(base, uint32_t(theirs) + 100);
  const double current = Lfs(base, speed);
  const double limit = Fmuls(their_speed, Lfs(base, 0x82002A3C));
  if (current > limit) {
    Stfs(base, speed, limit);
  }
  return theirs;
}

uint32_t IsSettled(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146408 AiDriver::IsSettled");
  if (Load(base, driver + 32) == 0) {
    return 1;
  }
  guest::StoreU32(base, driver + 480, 0xFFFFFFFFu);
  PickTarget(base, driver);
  const uint32_t target = Load(base, driver + 480);
  if (int32_t(target) < 0) {
    return 1;
  }
  const int32_t state = int32_t(Load(base, driver + 32));
  const uint32_t record = driver + target * 40 + 172;
  uint32_t above, below;  // the gap's bounds (floats)
  if (state == 2) {
    above = kTwenty;
    below = 0x82008D44;  // -20.0
  } else if (state == 3) {
    above = 0x820027A8;
    below = 0x82008E5C;
  } else {
    return 1;
  }
  const double gap = Lfs(base, record);
  if (gap > Lfs(base, above)) {
    return 0;
  }
  return gap < Lfs(base, below) ? 0 : 1;
}

void MirrorOther(uint8_t* base, uint32_t out, uint32_t record, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82145610 AiOther::Mirror");
  Stfs(base, out + 0, Fneg(Lfs(base, record + 0)));
  Stfs(base, out + 4, Fneg(Lfs(base, record + 4)));
  Stfs(base, out + 8, Fneg(Lfs(base, record + 8)));
  CopyWord(base, record + 20, out + 20);
  guest::StoreU8(base, out + 32, guest::LoadU8(base, record + 32));
  guest::StoreU8(base, out + 33, guest::LoadU8(base, record + 33));
  uint32_t state = guest::LoadU16(base, record + 24);
  if (int16_t(state) == 1) {
    state = 3;
  } else if (int16_t(state) == 3) {
    state = 1;
  }
  guest::StoreU16(base, out + 24, uint16_t(state));
  UpdateOtherFlags(base, out);
  CopyWord(base, value, out + 16);
}

// --- The path's entries and corners ---

void BlockLeft(uint8_t* base, uint32_t entry, uint32_t at, uint32_t width, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213C5C8 AiPathEntry::BlockLeft");
  Stfs(base, at, Fsubs(Lfs(base, at), Lfs(base, entry + 28)));
  double edge = Fadds(Lfs(base, out), Lfs(base, entry + 28));
  Stfs(base, out, edge);
  edge = Fadds(Lfs(base, width), edge);
  Stfs(base, out, edge);
  const double from = Lfs(base, at);
  if (from < edge) {
    Stfs(base, out, from);
  }
  const double block = Lfs(base, out);
  if (!(Lfs(base, entry + 40) < block)) {
    return;
  }
  const double most = Lfs(base, entry + 44);
  Stfs(base, entry + 40, block);
  if (block > most) {
    Stfs(base, entry + 40, most);
  }
}

void BlockRight(uint8_t* base, uint32_t entry, uint32_t at, uint32_t width, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213C630 AiPathEntry::BlockRight");
  Stfs(base, at, Fsubs(Lfs(base, at), Lfs(base, entry + 28)));
  double edge = Fadds(Lfs(base, out), Lfs(base, entry + 28));
  Stfs(base, out, edge);
  edge = Fadds(Lfs(base, width), edge);
  Stfs(base, out, edge);
  const double from = Lfs(base, at);
  if (from > edge) {
    Stfs(base, out, from);
  }
  const double block = Lfs(base, out);
  if (!(Lfs(base, entry + 44) > block)) {
    return;
  }
  const double least = Lfs(base, entry + 40);
  Stfs(base, entry + 44, block);
  if (block < least) {
    Stfs(base, entry + 44, least);
  }
}

void InitCorner(uint8_t* base, uint32_t record, uint32_t data, uint32_t corner_entry,
                uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213C698 AiCorner::Init");
  guest::StoreU32(base, record + 0, data);
  guest::StoreU32(base, record + 4, data + 56);
  guest::StoreU16(base, record + 8, guest::LoadU16(base, corner_entry + 0));
  const uint32_t offset =
      guest::LoadU16(base, count) - guest::LoadU16(base, corner_entry + 2);
  guest::StoreU16(base, record + 10, uint16_t(offset));
  const uint32_t first = guest::LoadU16(base, data + 0);
  guest::StoreU16(base, record + 14, 0);
  guest::StoreU16(base, record + 12, uint16_t(first + uint32_t(int32_t(int16_t(offset)))));
  const bool negative = Lfs(base, data + 4) < Lfs(base, kZero);
  StoreConstant(base, record + 16, negative ? kMinusOne : kOne);
  guest::StoreU32(base, record + 40, 0);
}

uint32_t WithinRoadAt(uint8_t* base, uint32_t path, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_8213C1A0 AiPath::WithinRoadAt");
  const uint32_t driver = Load(base, path + 0);
  const int32_t count = LoadS16(base, path + 5136);
  const uint32_t file = Load(base, track_route::kRoute);
  const double reach = Lfs(base, driver + 128);
  const double length = Lfs(base, file + 8);
  const int32_t k =
      int32_t(uint32_t(strings_and_font::steps::Fctiwz(Fdivs(reach, length))) + 1);
  if (!(k < count)) {
    return 0;
  }
  const int32_t start = LoadS16(base, path + 5138);
  const double at = Lfs(base, offset);
  const uint32_t entry = RingEntry(path, int32_t(uint32_t(start) + uint32_t(k))) + 16;
  const uint32_t record = Load(base, entry + 52);
  const double half = Fmuls(Lfs(base, record + 12), Lfs(base, kHalf));
  if (at > Fadds(Lfs(base, record + 20), half)) {
    return 0;
  }
  const double left = Fsubs(Fneg(half), Lfs(base, record + 16));
  return at < left ? 0 : 1;
}

// --- Starts and resets ---

namespace {
// +16 bits 0 and 2 cleared.
uint32_t ClearBits0And2(uint32_t flags) {
  return flags & ~1u & ~4u;
}
}  // namespace

void ResetToMode1(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139450 AiDriver::ResetToMode1");
  guest::StoreU32(base, frame + 80, 1);
  SetFlag2(base, driver, frame + 80);
  const uint32_t flags = Load(base, driver + 16);
  const uint32_t vehicle = Load(base, driver + 160);
  guest::StoreU32(base, frame + 84, 1);
  guest::StoreU32(base, driver + 16, ClearBits0And2(flags));
  if (vehicle != 0) {
    SetMode(base, driver + 512, frame + 84);
  }
  guest::StoreU32(base, driver + 6084, 0);
  ResetSkill(base, driver + 6048);
  const uint32_t again = Load(base, driver + 160);
  StoreConstant(base, driver + 12, 0x82008948);
  if (again != 0) {
    StoreConstant(base, again + 2432, kOne);
  }
}

void ResetToMode0(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821394F0 AiDriver::ResetToMode0");
  guest::StoreU32(base, frame + 80, 0);
  SetFlag2(base, driver, frame + 80);
  const uint32_t vehicle = Load(base, driver + 160);
  guest::StoreU32(base, frame + 88, 0);
  guest::StoreU32(base, driver + 16, ClearBits0And2(Load(base, driver + 16)));
  if (vehicle != 0) {
    SetMode(base, driver + 512, frame + 88);
  }
  ResetSkill(base, driver + 6048);
  guest::StoreU32(base, driver + 6084, 0);
  const int32_t skill = LoadS16(base, driver + 6052);
  Stfs(base, driver + 12, double(float(double(int64_t(skill)))));
}

uint32_t StartRace(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139DA0 AiDriver::StartRace");
  const uint32_t flags = Load(base, driver + 16);
  const uint32_t vehicle = Load(base, driver + 160);
  guest::StoreU32(base, driver + 16, flags & ~1u);
  // The vehicle's flags read whatever the vehicle (+6148 of 0 without one),
  // as the original does.
  const uint32_t free = (Load(base, vehicle + 6148) & 129) == 0 ? 1 : 0;
  guest::StoreU32(base, frame + 80, free);
  SetFlag2(base, driver, frame + 80);
  const uint32_t set = Load(base, driver + 16) | 4;
  guest::StoreU32(base, frame + 84, 0);
  const uint32_t again = Load(base, driver + 160);
  guest::StoreU32(base, driver + 16, set);
  if (again != 0) {
    SetMode(base, driver + 512, frame + 84);
  }
  guest::StoreU32(base, frame + 80, 1);
  SetSkill(base, driver, frame + 80);
  return ClearPlan(base, driver);
}

void ResetFollow(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821467B8 AiDriver::ResetFollow");
  ClearFollow(base, driver);
  ClearCarState(base, driver);
}

bool AllSettled(uint8_t* base, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_8213ADF8 AiDrivers::AllSettled");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint32_t table = Load(base, kDrivers);
  const double seventy = Lfs(base, 0x82008930);
  double lowest = seventy;
  int32_t settled = 0;
  if (table != 0) {
    const uint32_t copy = frame + 80;
    for (int32_t i = 0; i < count; i++) {
      const uint32_t racer = Load(base, Load(base, kRace) + 2308 + 4 * uint32_t(i));
      if ((Load(base, racer + 0) & 0x10) == 0) {
        continue;
      }
      const int32_t index = int32_t(Load(base, racer + 20));
      if (index < 0) {
        continue;
      }
      // memcpy (sub_825A7410) copies natively with nothing for its override
      // to record (a destination outside the physical views, which it
      // records for the renderer); else the original runs.
      if (copy >= 0xA0000000u ||
          !c_runtime::Memcpy(base, copy, uint32_t(index) * kDriverBytes + table, kDriverBytes)) {
        return false;
      }
      settled++;
      if (IsSettled(base, copy) == 0) {
        *result = 0;
        return true;
      }
      const double speed = Lfs(base, copy + 100);
      if (lowest > speed) {
        lowest = speed;
      }
      table = Load(base, kDrivers);
    }
    if (settled == 1) {
      *result = 1;
      return true;
    }
  }
  *result = Fadds(lowest, Lfs(base, 0x820021F4)) > Lfs(base, 0x82008930) ? 1 : 0;
  return true;
}

// --- Pedals and steering ---

void AccumulateSteer(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82138F00 AiDriver::AccumulateSteer");
  const double angle = Lfs(base, driver + 6116);
  const double sum = Fadds(Lfs(base, driver + 6112), angle);
  double steer = Lfs(base, driver + 152);
  const double crossed = Fmuls(angle, steer);
  const double total = Fadds(sum, Lfs(base, driver + 6136));
  const double zero = Lfs(base, kZero);
  Stfs(base, driver + 6136, total);
  if (!(crossed < zero)) {
    return;
  }
  steer = Fmuls(steer, Lfs(base, kFifth));
  if (steer < zero) {
    steer = Fneg(steer);
  }
  const double speed = Lfs(base, driver + 100);
  if (speed < Lfs(base, kOtherGap)) {
    steer = Fmuls(Fmuls(speed, Lfs(base, 0x82002A08)), steer);
  }
  steer = Fsubs(steer, Lfs(base, kOne));
  if (!(steer > zero)) {
    return;
  }
  const double most = Lfs(base, 0x820021E4);
  Stfs(base, driver + 6136,
       steer < most ? Fmadds(angle, steer, total) : Fmadds(angle, most, total));
}

void ThrottleForSpeed(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139168 AiDriver::ThrottleForSpeed");
  const double speed = Lfs(base, driver + 100);
  if (!(speed > Lfs(base, kTwenty))) {
    return;
  }
  const double over = Fsubs(speed, Lfs(base, driver + 6088));
  const double ten = Lfs(base, kTen);
  const double zero = Lfs(base, kZero);
  if (over > ten) {
    Stfs(base, driver + 6140, zero);
    StoreConstant(base, driver + 6144, 0x820029F0);
  } else if (over > zero) {
    Stfs(base, driver + 6140, zero);
    Stfs(base, driver + 6144, zero);
  } else if (over > Lfs(base, 0x82002824)) {
    const double eased = Fmuls(Fneg(over), Lfs(base, kFifth));
    if (eased < Lfs(base, driver + 6140)) {
      Stfs(base, driver + 6140, eased);
    }
    Stfs(base, driver + 6144, zero);
  }
  if (Lfs(base, driver + 100) > Lfs(base, driver + 168)) {
    Stfs(base, driver + 6140, zero);
  }
  if ((Load(base, driver + 16) & 4) != 0) {
    return;
  }
  const double angle = Lfs(base, driver + 6116);
  if (angle > Lfs(base, 0x82001FB4) || angle < Lfs(base, 0x82002A50)) {
    Stfs(base, driver + 6140, zero);
  }
}

void EaseThrottleForSlide(uint8_t* base, uint32_t driver, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_82139BE8 AiDriver::EaseThrottleForSlide");
  const double speed = Lfs(base, driver + 100);
  if (speed < Lfs(base, kTwenty)) {
    return;
  }
  const uint32_t vehicle = Load(base, driver + 160);
  uint32_t kind = 3;
  uint32_t limit = 0x82008954;
  if (vehicle != 0) {
    kind = Load(base, vehicle + 3548);
  }
  if (kind == 0) {
    limit = 0x8200894C;
  } else if (kind == 1) {
    limit = 0x82008950;
  }
  const double f13 = Lfs(base, limit);
  const double factor = Fmuls(Lfs(base, driver + 140), Lfs(base, driver + 132));
  const double slide = Fabs(Lfs(base, driver + 6112));
  double ease = Fmsubs(slide, speed, Fmuls(factor, f13));
  const double zero = Lfs(base, kZero);
  if (!(ease > zero)) {
    return;
  }
  ease = Fmuls(ease, Lfs(base, Load(base, flag) != 0 ? kFifth : 0x82002204));
  if (!(ease > zero)) {
    return;
  }
  const double throttle = Lfs(base, driver + 6140);
  Stfs(base, driver + 6140, throttle < ease ? zero : Fsubs(throttle, ease));
}

void LimitThrottle(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139CB0 AiDriver::LimitThrottle");
  int32_t skill = LoadS16(base, driver + 6054);
  if (skill < 0) {
    skill = 0;
  } else if (skill > 100) {
    skill = 100;
  }
  const double most = Fmuls(double(float(double(int64_t(skill)))), Lfs(base, 0x8200222C));
  const double zero = Lfs(base, kZero);
  const double throttle = Lfs(base, driver + 6124);
  if (throttle < zero) {
    Stfs(base, driver + 6124, zero);
  } else if (throttle > most) {
    Stfs(base, driver + 6124, most);
  }
  const uint32_t vehicle = Load(base, driver + 160);
  const double limit = Lfs(base, vehicle + 4660);
  const double rpm = Lfs(base, vehicle + 2228);
  if (rpm < Fmuls(limit, Lfs(base, 0x82002A34))) {
    return;
  }
  if (rpm > Fmuls(limit, Lfs(base, 0x82002B90))) {
    Stfs(base, driver + 6124, zero);
    return;
  }
  const bool high = rpm > Fmuls(limit, Lfs(base, 0x82008958));
  const double now = Lfs(base, driver + 6124);
  if (now < zero) {
    Stfs(base, driver + 6124, zero);
    return;
  }
  const double cap = Lfs(base, high ? 0x8200296C : kHalf);
  if (now > cap) {
    Stfs(base, driver + 6124, cap);
  }
}

void AdjustForOther(uint8_t* base, uint32_t driver, uint32_t record, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821452A0 AiDriver::AdjustForOther");
  const double gap = Lfs(base, record + 0);
  const double zero = Lfs(base, kZero);
  if (!(gap < zero)) {
    return;
  }
  const int32_t index = LoadS16(base, record + 26);
  if (index < 0) {
    return;
  }
  if (guest::LoadU16(base, record + 24) != 1) {
    return;
  }
  if (!(Lfs(base, record + 4) > zero)) {
    return;
  }
  if ((guest::LoadU16(base, record + 28) & 4) != 0) {
    return;
  }
  double distance = Fneg(gap);
  if (Load(base, mode) != 0 && index > 1) {
    const double length = Lfs(base, Load(base, track_route::kRoute) + 8);
    const double slices = double(float(double(int64_t(index - 1))));
    distance = Fmadds(slices, length, distance);
  }
  if (distance < zero) {
    distance = zero;
  }
  double scale = Lfs(base, driver + 100);
  const double factor = Lfs(base, driver + 136);
  const double hundred = Lfs(base, kHundred);
  scale = scale > hundred ? Fdivs(hundred, scale) : Lfs(base, kOne);
  const double limit = Fmadds(Fmuls(scale, factor), distance, Lfs(base, record + 16));
  if (limit < Lfs(base, driver + 168)) {
    Stfs(base, driver + 168, limit);
  }
  if (Lfs(base, record + 0) < Lfs(base, 0x82008E48)) {
    return;
  }
  if (limit < Lfs(base, driver + 164)) {
    Stfs(base, driver + 164, limit);
  }
}

uint64_t UpdateAgainstPlayer(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146608 AiDriver::UpdateAgainstPlayer");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint64_t r3 = uint32_t(count);
  StoreConstant(base, driver + 488, kZero);
  guest::StoreU16(base, driver + 484, 0);
  guest::StoreU16(base, driver + 486, 0);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(base, Load(base, kRace) + 2308 + 4 * uint32_t(i));
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0 || Load(base, driver + 0) == index) {
      continue;
    }
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
    r3 = vehicle;
    if ((Load(base, vehicle + 6148) & 129) == 0) {
      continue;
    }
    const uint32_t record = driver + index * 40 + 172;
    if (guest::LoadU8(base, record + 32) == 1) {
      if ((guest::LoadU16(base, record + 34) & 2) != 0) {
        guest::StoreU16(base, driver + 484, uint16_t(guest::LoadU16(base, driver + 484) + 1));
      }
      if ((guest::LoadU16(base, record + 34) & 4) != 0) {
        guest::StoreU16(base, driver + 486, uint16_t(guest::LoadU16(base, driver + 486) + 1));
      }
    }
    CopyWord(base, record + 0, driver + 488);
    break;
  }
  const double speed = Lfs(base, driver + 100);
  bool stuck_check = speed < Lfs(base, 0x82008034);
  if (stuck_check) {
    r3 = IsFree(base, driver);
    stuck_check = r3 != 0;
  }
  if (!stuck_check) {
    guest::StoreU16(base, driver + 492, 0);
    guest::StoreU32(base, driver + 16, Load(base, driver + 16) & ~0x20u);
    return r3;
  }
  const uint32_t flags = Load(base, driver + 16);
  const uint32_t count492 = guest::LoadU16(base, driver + 492);
  if ((flags & 0x20) == 0) {
    const double held = double(float(double(int64_t(int16_t(count492)))));
    if (!(held < Lfs(base, 0x82008238))) {
      guest::StoreU32(base, driver + 16, flags | 32);
      return r3;
    }
    const double fifteen = Lfs(base, 0x820029D0);
    if (!(speed < fifteen)) {
      return r3;
    }
    const uint32_t step = uint32_t(strings_and_font::steps::Fctiwz(Fsubs(speed, fifteen)));
    guest::StoreU16(base, driver + 492, uint16_t(count492 - (step & 0xFFFF)));
    return r3;
  }
  const uint32_t step = uint32_t(strings_and_font::steps::Fctiwz(speed));
  const int32_t left = int16_t(uint16_t(count492 - step));
  guest::StoreU16(base, driver + 492, uint16_t(left));
  if (left > 0) {
    return r3;
  }
  guest::StoreU16(base, driver + 492, 0);
  guest::StoreU32(base, driver + 16, flags & ~0x20u);
  return r3;
}

void UpdateOther(uint8_t* base, uint32_t record, uint32_t gap, uint32_t length) {
  ITC_NATIVE_RUNNING("sub_821454C8 AiOther::Update");
  const double old = Lfs(base, record + 0);
  if (!(old < Lfs(base, 0x82008E38)) && !(old > Lfs(base, kOtherGap))) {
    const double change = Fsubs(Lfs(base, gap), old);
    const double last = Lfs(base, record + 8);
    Stfs(base, record + 8, change);
    Stfs(base, record + 4, Fmuls(Fadds(last, change), Lfs(base, kHalf)));
    const double now = Lfs(base, gap);
    const double size = Lfs(base, length);
    const double zero = Lfs(base, kZero);
    double apart = Fadds(size, now);
    bool beside = false;
    if (!(apart < zero)) {
      apart = Fsubs(now, size);
      beside = !(apart > zero);
    }
    if (beside) {
      guest::StoreU16(base, record + 24, 2);
      Stfs(base, record + 20, zero);
    } else {
      const double rate = Lfs(base, record + 4);
      double meet;
      if (Fmuls(rate, apart) < zero) {
        const int32_t ticks = strings_and_font::steps::Fctiwz(Fneg(Fdivs(apart, rate)));
        meet = ticks < 10000
                   ? Fmuls(double(float(double(int64_t(ticks)))), Lfs(base, 0x82002A3C))
                   : Lfs(base, kMinusOne);
      } else {
        meet = Lfs(base, kMinusOne);
      }
      Stfs(base, record + 20, meet);
      guest::StoreU16(base, record + 24, Lfs(base, gap) > zero ? 3 : 1);
    }
  } else {
    const double zero = Lfs(base, kZero);
    Stfs(base, record + 4, zero);
    guest::StoreU16(base, record + 24, 0);
    Stfs(base, record + 8, zero);
    StoreConstant(base, record + 20, kMinusOne);
  }
  CopyWord(base, gap, record + 0);
  UpdateOtherFlags(base, record);
}

// --- Resets, steering input and the path's lists ---

void ResetControl(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139580 AiDriver::ResetControl");
  guest::StoreU32(base, frame + 80, 1);
  guest::StoreU32(base, driver + 16, Load(base, driver + 16) & ~1u);
  SetFlag2(base, driver, frame + 80);
  ResetToMode1(base, driver, frame - 112);
}

uint64_t ResetDriver(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213AAA8 AiDriver::Reset");
  ResetFollow(base, driver);
  ResetFor(base, driver + 512, driver);
  const double zero = Lfs(base, kZero);
  Stfs(base, driver + 6088, zero);
  Stfs(base, driver + 6096, zero);
  Stfs(base, driver + 6100, zero);
  const double one = Lfs(base, kOne);
  Stfs(base, driver + 6104, zero);
  Stfs(base, driver + 6108, one);
  guest::StoreU8(base, driver + 6132, 0);
  Stfs(base, driver + 6112, zero);
  guest::StoreU32(base, driver + 6148, 0x8213A1B8);  // AiDriver::UpdateStarting
  for (uint32_t at : {6116u, 6120u, 6124u, 6128u, 6136u, 6140u, 6144u}) {
    Stfs(base, driver + at, zero);
  }
  return ResetOthers(base, driver);
}

uint64_t UpdateSteerInput(uint8_t* base, uint32_t driver, uint32_t stack, double* f1,
                          bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AC70 AiDriver::UpdateSteerInput");
  const int32_t count = LoadS16(base, driver + 5674);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t at = driver + 5676 + 44 * uint32_t(i);
    if (Load(base, at + 0) == 0) {
      continue;
    }
    const int32_t kind = int32_t(Load(base, at + 40));
    if (kind == 1) {
      break;
    }
    if (kind == 2) {
      return SteerIntoCorner(base, driver, at, stack - 96, f1, f1_set);
    }
    if (kind == 3) {
      return SteerOutOfCorner(base, driver, at, stack - 96);
    }
  }
  AccumulateSteer(base, driver);
  return driver;
}

void DropPassedSections(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213BA48 AiPath::DropPassedSections");
  const int32_t count = LoadS16(base, path + 5144);
  const uint32_t driver = Load(base, path + 0);
  if (count <= 0) {
    return;
  }
  const uint32_t list = path + 5146;
  const int32_t current = LoadS16(base, driver + 4);
  if (LoadS16(base, list) == current) {
    return;
  }
  int32_t found = -1;
  for (int32_t i = 1; count > 1 && i < LoadS16(base, path + 5144); i++) {
    if (LoadS16(base, list + 2 * uint32_t(i)) == current) {
      found = i;
      break;
    }
  }
  if (found < 0) {
    guest::StoreU16(base, path + 5144, 0);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU16(base, list + 2 * i, 0xFFFF);
    }
    return;
  }
  int32_t moved = 0;
  if (count - found > 0) {
    int32_t j = 0;
    do {
      guest::StoreU16(base, list + 2 * uint32_t(j),
                      guest::LoadU16(base, list + 2 * uint32_t(j + found)));
      j++;
      moved = j;
    } while (j < LoadS16(base, path + 5144) - found);
  }
  for (int32_t i = moved; i < 8; i++) {
    guest::StoreU16(base, list + 2 * uint32_t(i), 0xFFFF);
  }
  guest::StoreU16(base, path + 5144, uint16_t(guest::LoadU16(base, path + 5144) - found));
}

uint32_t SharesSections(uint8_t* base, uint32_t path, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_8213C010 AiPath::SharesSections");
  const int32_t n = LoadS16(base, path + 5144);
  if (n <= 0) {
    return 0;
  }
  const int32_t m = LoadS16(base, record + 2);
  const uint32_t list = path + 5146;
  for (int32_t i = 0; i < n; i++) {
    if (m <= 0) {
      continue;
    }
    const int32_t section = LoadS16(base, list + 2 * uint32_t(i));
    for (int32_t j = 0; j < m; j++) {
      if (section != LoadS16(base, record + 36 + 2 * uint32_t(j))) {
        continue;
      }
      const int32_t least = i > j ? j : i;
      int32_t a = i - least, b = j - least;
      if (!(a < n)) {
        return 1;
      }
      while (b < m) {
        if (guest::LoadU16(base, list + 2 * uint32_t(a)) !=
            guest::LoadU16(base, record + 36 + 2 * uint32_t(b))) {
          return 0;
        }
        a++;
        b++;
        if (!(a < n)) {
          break;
        }
      }
      return 1;
    }
  }
  return 0;
}

void Widen(uint8_t* base, uint32_t entry, uint32_t margin, uint32_t shift) {
  ITC_NATIVE_RUNNING("sub_8213C3A8 AiPathEntry::Widen");
  Stfs(base, entry + 40, Fadds(Lfs(base, entry + 32), Lfs(base, margin)));
  const double m = Lfs(base, margin);
  const double high = Fsubs(Lfs(base, entry + 36), m);
  Stfs(base, entry + 44, high);
  const double by = Lfs(base, shift);
  if (by > Lfs(base, kZero)) {
    Stfs(base, entry + 40, Fadds(Lfs(base, entry + 40), by));
  } else {
    Stfs(base, entry + 44, Fadds(by, high));
  }
  const double lo = Lfs(base, entry + 40);
  const double hi = Lfs(base, entry + 44);
  if (!(lo > hi)) {
    return;
  }
  const double middle = Fmuls(Fadds(hi, lo), Lfs(base, kHalf));
  Stfs(base, entry + 44, middle);
  Stfs(base, entry + 40, middle);
}

void NarrowToOther(uint8_t* base, uint32_t entry, uint32_t other, uint32_t margin,
                   uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_8213C420 AiPathEntry::NarrowToOther");
  if (other == 0) {
    return;
  }
  const uint32_t record = Load(base, other + 52);
  if (record == 0) {
    return;
  }
  const double m = Lfs(base, margin);
  double lo = Fsubs(Lfs(base, other + 32), m);
  double hi = Fadds(Lfs(base, other + 36), m);
  const double least = Lfs(base, record + 24);
  if (lo < least) {
    lo = least;
  }
  const double most = Lfs(base, record + 28);
  if (hi > most) {
    hi = most;
  }
  const double by = Lfs(base, offset);
  double a = Fadds(by, lo);
  double b = Fadds(by, hi);
  if (a > b) {
    b = Fmuls(Fadds(b, a), Lfs(base, kHalf));
    a = b;
  }
  if (Lfs(base, entry + 32) < a) {
    Stfs(base, entry + 32, a);
    Stfs(base, entry + 40, a);
  }
  if (Lfs(base, entry + 36) > b) {
    Stfs(base, entry + 36, b);
    Stfs(base, entry + 44, b);
  }
}

bool AdvanceCorners(uint8_t* base, uint32_t path, uint32_t slices, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213D020 AiPath::AdvanceCorners");
  // The records' moves are memcpy's (sub_825A7410): natively only with
  // nothing for its override to record (outside the physical views) and the
  // records in one view.
  const uint32_t records = path + 5164;
  if (records >= 0xA0000000u || !c_runtime::InOneView(records, 9 * 44)) {
    return false;
  }
  *r3 = path;
  const int32_t last = LoadS16(base, path + 5162) - 1;
  if (last < 0) {
    return true;
  }
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  for (int32_t k = last; k >= 0; k--) {
    const uint32_t record = path + 5164 + 44 * uint32_t(k);
    guest::StoreU16(base, record + 12,
                    uint16_t(guest::LoadU16(base, record + 12) - guest::LoadU16(base, slices)));
    const uint32_t step = guest::LoadU16(base, slices);
    const int32_t left = LoadS16(base, record + 12);
    guest::StoreU16(base, record + 10, uint16_t(guest::LoadU16(base, record + 10) - step));
    if (left > 0) {
      continue;
    }
    int32_t j = k;
    if (k < LoadS16(base, path + 5162) - 1) {
      uint32_t to = record;
      do {
        // Adjacent records don't share memory: c_runtime::Memcpy copies them
        // (the host's memmove, which copies them the same, otherwise).
        if (!c_runtime::Memcpy(base, to, to + 44, 44)) {
          std::memmove(base + to, base + to + 44, 44);
        }
        *r3 = to;  // memcpy's r3
        j++;
        to += 44;
      } while (j < LoadS16(base, path + 5162) - 1);
    }
    const uint32_t cleared = path + 5164 + 44 * uint32_t(j);
    Stfs(base, cleared + 16, one);
    guest::StoreU32(base, cleared + 0, 0);
    Stfs(base, cleared + 20, zero);
    guest::StoreU32(base, cleared + 4, 0);
    Stfs(base, cleared + 28, zero);
    guest::StoreU16(base, cleared + 8, 0);
    Stfs(base, cleared + 32, zero);
    guest::StoreU16(base, cleared + 10, 0);
    guest::StoreU16(base, cleared + 12, 0);
    guest::StoreU16(base, cleared + 14, 0);
    guest::StoreU32(base, cleared + 24, 0);
    guest::StoreU8(base, cleared + 36, 0);
    guest::StoreU8(base, cleared + 37, 0);
    guest::StoreU32(base, cleared + 40, 0);
    guest::StoreU16(base, path + 5162, uint16_t(guest::LoadU16(base, path + 5162) - 1));
  }
  return true;
}

// --- The states and the drivers' resets ---

namespace {
// slw: 0 for shifts of 32 to 63 (the amount's low six bits).
uint32_t Slw(uint32_t value, uint32_t shift) {
  return (shift & 0x20) != 0 ? 0 : value << (shift & 0x1F);
}
// The driver's state set to `state`, the old one kept in +36.
void SetStateWord(uint8_t* base, uint32_t driver, uint32_t state) {
  const uint32_t old = Load(base, driver + 32);
  guest::StoreU32(base, driver + 32, state);
  guest::StoreU32(base, driver + 36, old);
}
}  // namespace

uint32_t Restart(uint8_t* base, uint32_t driver, uint32_t mask, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821395D0 AiDriver::Restart");
  ResetControl(base, driver, frame - 112);
  const uint32_t r3 = ResetPath(base, driver + 512);
  const uint32_t old = Load(base, driver + 32);
  guest::StoreU32(base, driver + 36, old);
  guest::StoreU32(base, driver + 32, 5);
  guest::StoreU16(base, driver + 40, uint16_t(Load(base, mask)));
  return r3;
}

uint32_t ResetAll(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139750 AiDrivers::ResetAll");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint32_t r3 = uint32_t(count);
  uint32_t mask = 0;
  guest::StoreU32(base, frame + 80, mask);
  uint32_t table = Load(base, kDrivers);
  if (table == 0) {
    return r3;
  }
  uint32_t race = Load(base, kRace);
  if (count <= 0) {
    return r3;
  }
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(base, race + 2308 + 4 * uint32_t(i));
    if ((Load(base, racer + 0) & 0x10) != 0) {
      continue;
    }
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0) {
      continue;
    }
    mask |= Slw(1, index);
  }
  guest::StoreU32(base, frame + 80, mask);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(base, race + 2308 + 4 * uint32_t(i));
    if ((Load(base, racer + 0) & 0x10) != 0) {
      continue;
    }
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0) {
      continue;
    }
    r3 = Restart(base, index * kDriverBytes + table, frame + 80, frame - 112);
    table = Load(base, kDrivers);
    race = Load(base, kRace);
  }
  return r3;
}

uint32_t SetState2or3(uint8_t* base, uint32_t driver, uint32_t word, uint32_t flag,
                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139E40 AiDriver::SetState2or3");
  ResetControl(base, driver, frame - 112);
  ClearPlan(base, driver);
  const bool set = Load(base, flag) != 0;
  const uint32_t r3 = ResetPath(base, driver + 512);
  SetStateWord(base, driver, set ? 3 : 2);
  guest::StoreU16(base, driver + 40, uint16_t(Load(base, word)));
  return r3;
}

uint32_t SetStateFromList(uint8_t* base, uint32_t driver, uint32_t word, uint32_t count,
                          uint32_t list, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139EA8 AiDriver::SetStateFromList");
  ResetControl(base, driver, frame - 112);
  const uint32_t r3 = ClearPlan(base, driver);
  const int32_t n = int32_t(Load(base, count));
  if (n <= 1) {
    return r3;
  }
  if (n <= 2) {
    const uint32_t reset = ResetPath(base, driver + 512);
    const uint32_t old = Load(base, driver + 32);
    guest::StoreU32(base, driver + 36, old);
    guest::StoreU32(base, driver + 32, 3);
    guest::StoreU16(base, driver + 40, uint16_t(Load(base, word)));
    return reset;
  }
  int32_t k = 0;
  const int32_t me = int32_t(Load(base, driver + 0));
  for (; k < n; k++) {
    if (me == int32_t(Load(base, list + 4 * uint32_t(k)))) {
      break;
    }
  }
  if (!(k < n)) {
    return r3;
  }
  uint32_t state = 3;
  uint32_t other;
  if (k == 0) {
    other = Load(base, list + 4);
  } else if (k == 1) {
    other = Load(base, list + 0);
  } else {
    other = Load(base, list + 4 * uint32_t(k - 2));
    state = 2;
  }
  const uint32_t mask = Slw(1, other);
  const uint32_t reset = ResetPath(base, driver + 512);
  const uint32_t old = Load(base, driver + 32);
  guest::StoreU16(base, driver + 40, uint16_t(mask));
  guest::StoreU32(base, driver + 32, state);
  guest::StoreU32(base, driver + 36, old);
  return reset;
}

uint32_t SetState4(uint8_t* base, uint32_t driver, uint32_t word, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139FA0 AiDriver::SetState4");
  ResetControl(base, driver, frame - 112);
  ClearPlan(base, driver);
  const uint32_t r3 = ResetPath(base, driver + 512);
  const uint32_t old = Load(base, driver + 32);
  guest::StoreU32(base, driver + 36, old);
  guest::StoreU32(base, driver + 32, 4);
  guest::StoreU16(base, driver + 40, uint16_t(Load(base, word)));
  return r3;
}

uint32_t ClearAll(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A140 AiDrivers::ClearAll");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint32_t r3 = uint32_t(count);
  if (Load(base, kDrivers) == 0 || count <= 0) {
    return r3;
  }
  for (int32_t i = 0; i < count; i++) {
    const uint32_t table = Load(base, kDrivers);
    const uint32_t racer = Load(base, Load(base, kRace) + 2308 + 4 * uint32_t(i));
    const uint32_t driver = Load(base, racer + 20) * kDriverBytes + table;
    ResetControl(base, driver, frame - 112);
    r3 = ClearPlan(base, driver);
  }
  return r3;
}

// --- The corners' joins ---

namespace {
// An int (sign-extended to 64 bits, fcfid) rounded to a float (frsp).
double IntToFloat(int32_t value) {
  return double(float(double(int64_t(value))));
}
}  // namespace

uint32_t JoinCorners(uint8_t* base, uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BCD0 AiPath::JoinCorners");
  const uint32_t data_b = Load(base, b + 0);
  uint32_t shift = 0;
  const uint32_t data_a = Load(base, a + 0);
  const uint32_t ends = data_b + 48;
  const int32_t have = int32_t(Load(base, count));
  const double f0 = Lfs(base, data_b + 28);
  double f13 = Fadds(Lfs(base, data_a + 12), f0);
  const int32_t start = LoadS16(base, data_b + 0);
  const int32_t gap = LoadS16(base, ends + 6) - start;
  if (have < gap) {
    shift = uint32_t(gap - have);
    guest::StoreU32(base, count, uint32_t(gap));
  }
  const int32_t exit = LoadS16(base, data_a + 50);
  const double f12a = Lfs(base, data_b + 28);
  const uint32_t now = Load(base, count);
  const uint32_t table_a = Load(base, a + 4);
  const uint32_t r3 = Load(base, b + 4);
  double one = Lfs(base, kOne);
  f13 = Fdivs(one, f13);
  const double f11a = Lfs(base, ((shift << 3) & 0xFFFFFFF8u) + table_a + 4);
  const uint32_t at_b = ((now + uint32_t(start)) << 3) & 0xFFFFFFF8u;
  const int32_t enter = LoadS16(base, ends + 4);
  const double f10 = Fmuls(f12a, f13);
  const int32_t span = int32_t(now - uint32_t(enter) + uint32_t(start));
  double f12 = Fsubs(f11a, Lfs(base, at_b + r3 - 4));
  const double from = IntToFloat(exit);
  const double f9 = Fneg(f12);
  double f8 = Fadds(IntToFloat(span), from);
  f8 = Fmuls(f8, Lfs(base, kHalf));
  one = Fdivs(one, f8);
  f12 = Fmuls(Fmuls(f10, one), f12);
  Stfs(base, b + 32, f12);
  double f13b = Fmuls(Lfs(base, data_a + 12), f13);
  f13b = Fmuls(f13b, f9);
  Stfs(base, a + 28, Fmuls(f13b, one));
  guest::StoreU8(base, b + 37, 1);
  guest::StoreU8(base, a + 36, 1);
  const int32_t most = LoadS16(base, ends + 4);
  if (LoadS16(base, b + 14) > most) {
    guest::StoreU16(base, b + 14, uint16_t(most));
  }
  guest::StoreU16(base, a + 14, uint16_t(shift));
  return r3;
}

uint32_t JoinCornersShort(uint8_t* base, uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BF28 AiPath::JoinCornersShort");
  const uint32_t data_a = Load(base, a + 0);
  const uint32_t have = Load(base, count);
  const uint32_t data_b = Load(base, b + 0);
  const uint32_t table_a = Load(base, a + 4);
  const uint32_t r3 = Load(base, b + 4);
  double f12 = Lfs(base, data_a + 12);
  double f0 = Lfs(base, data_b + 28);
  double f13 = Lfs(base, table_a + 4);
  const uint32_t ends = data_b + 48;
  f12 = Fadds(f12, f0);
  const int32_t exit = int32_t(uint32_t(LoadS16(base, data_a + 50)) + have);
  const int32_t start = LoadS16(base, data_b + 0);
  f13 = Fsubs(f13, Lfs(base, ((uint32_t(start) << 3) & 0xFFFFFFF8u) + r3 - 4));
  const int32_t enter = LoadS16(base, ends + 4);
  guest::StoreU8(base, b + 37, 1);
  const double f10 = Fneg(f13);
  const int32_t span = int32_t(uint32_t(start) - uint32_t(enter) + have);
  const double f9 = IntToFloat(exit);
  f12 = Fdivs(Lfs(base, kOne), f12);
  f0 = Fmuls(f12, f0);
  f0 = Fmuls(f0, f13);
  f0 = Fdivs(f0, IntToFloat(span));
  Stfs(base, b + 32, f0);
  f0 = Lfs(base, data_a + 12);
  guest::StoreU8(base, a + 36, 1);
  f0 = Fmuls(f0, f12);
  f0 = Fmuls(f0, f10);
  Stfs(base, a + 28, Fdivs(f0, f9));
  const int32_t most = LoadS16(base, ends + 4);
  if (LoadS16(base, b + 14) > most) {
    guest::StoreU16(base, b + 14, uint16_t(most));
  }
  guest::StoreU16(base, a + 14, 0);
  return r3;
}

uint32_t JoinCornerPair(uint8_t* base, uint32_t a, uint32_t b, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213DD40 AiPath::JoinCornerPair");
  const uint32_t data_b = Load(base, b + 0);
  if (data_b == 0 || Load(base, b + 4) == 0 || Load(base, a + 0) == 0 ||
      Load(base, a + 4) == 0) {
    return 0;
  }
  const int32_t gap = int32_t(uint32_t(LoadS16(base, a + 10)) - uint32_t(LoadS16(base, b + 10)) -
                              uint32_t(LoadS16(base, data_b + 0)));
  guest::StoreU32(base, frame + 80, uint32_t(gap));
  if (gap < 0) {
    if (!(Lfs(base, a + 16) == Lfs(base, b + 16))) {
      *r3 = JoinCorners(base, a, b, frame + 80);
      return 1;
    }
    guest::StoreU32(base, frame + 80, uint32_t(-gap));
    return Lfs(base, data_b + 28) < Lfs(base, kHundred) ? 2 : 3;
  }
  const double length = Lfs(base, Load(base, track_route::kRoute) + 8);
  if (Fmuls(length, IntToFloat(gap)) > Lfs(base, kHundred)) {
    return 0;
  }
  *r3 = JoinCornersShort(base, a, b, frame + 80);
  return 1;
}

uint32_t AddCornersAt(uint8_t* base, uint32_t path, uint32_t entry, uint32_t frame,
                      uint32_t* r3, uint32_t* a, uint32_t* b) {
  ITC_NATIVE_RUNNING("sub_8213E970 AiPath::AddCornersAt");
  *r3 = path;
  const uint32_t slice = Load(base, entry + 52);
  uint32_t listed = slice + 60;
  const int32_t n = LoadS16(base, slice + 58);
  if (n <= 0) {
    return 0;
  }
  const int32_t have = LoadS16(base, path + 5162);
  if (have > 0) {
    const int32_t entries = LoadS16(base, path + 5136);
    if (LoadS16(base, uint32_t(have) * 44 + path + 5132) > entries) {  // the last's +12
      return 0;
    }
  }
  const uint32_t route = Load(base, track_route::kRoute);
  for (int32_t k = 0; k < n; k++, listed += 4) {
    if (have > 0) {
      const int32_t number = guest::LoadU16(base, listed + 0);
      uint32_t at = path + 5172;  // the records' +8
      int32_t i = 0;
      bool listed_already = false;
      do {
        if (number == LoadS16(base, at)) {
          listed_already = true;
          break;
        }
        i++;
        at += 44;
      } while (i < LoadS16(base, path + 5162));
      if (listed_already) {
        continue;
      }
    }
    const uint32_t data =
        Load(base, (uint32_t(guest::LoadU16(base, listed + 0)) << 2) + Load(base, route + 16));
    if (data == 0) {
      continue;
    }
    if (!(LoadS16(base, data + 54) > int32_t(guest::LoadU16(base, listed + 2)))) {
      continue;
    }
    const uint32_t shares = SharesSections(base, path, data);
    *r3 = shares;
    if (int32_t(shares) != 1) {
      continue;
    }
    if (!(have < 8)) {
      return 0;
    }
    const uint32_t record = uint32_t(have) * 44 + path + 5164;
    InitCorner(base, record, data, listed, path + 5136);
    *r3 = record;
    const int32_t now = LoadS16(base, path + 5162);
    if (now > 0) {
      const uint32_t before = uint32_t(now) * 44 + path + 5120;
      uint32_t joined = path;
      const uint32_t step = JoinCornerPair(base, record, before, frame - 112, &joined);
      if (step >= 2) {
        *a = record;
        *b = before;
        return step;
      }
      *r3 = joined;
    }
    AddCornersAtFinish(base, path);
    return 0;
  }
  return 0;
}

void AddCornersAtFinish(uint8_t* base, uint32_t path) {
  guest::StoreU16(base, path + 5162, uint16_t(guest::LoadU16(base, path + 5162) + 1));
}

// --- The states' speeds ---

uint64_t UpdateState2(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82145F48 AiDriver::UpdateState2");
  PickTarget(base, driver);
  const uint32_t target = Load(base, driver + 480);
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint32_t record = target * 40 + driver;
  const double gap = Lfs(base, record + 172);
  uint64_t r3 = driver;
  double f0;
  if (gap > Lfs(base, 0x82002834)) {
    if (!(gap < Lfs(base, kHundred))) {
      f0 = Lfs(base, kTwenty);
    } else {
      r3 = OfIndex(base, target);
      f0 = Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x82002A2C));
      const double most = Lfs(base, 0x82000F14);
      if (f0 > most) {
        f0 = most;
      }
    }
  } else {
    const double zero = Lfs(base, kZero);
    if (!(gap < zero)) {
      return r3;
    }
    r3 = OfIndex(base, target);
    const double again = Lfs(base, record + 172);
    if (!(again > Lfs(base, 0x82008E54))) {
      f0 = again > Lfs(base, 0x82008E4C)
               ? Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x820029D4))
               : Lfs(base, kFiveHundred);
      if (f0 > Lfs(base, speed)) {
        Stfs(base, speed, f0);
      }
      return r3;
    }
    const bool near = again < Lfs(base, 0x82008E50);
    f0 = Lfs(base, uint32_t(r3) + 100);
    if (!near) {
      const bool slow = f0 < Lfs(base, 0x82000F14);
      const double factor = Lfs(base, 0x82002A2C);
      if (!slow) {
        f0 = Fmuls(f0, factor);
      } else {
        f0 = Fmuls(f0, factor) < Lfs(base, kTwenty) ? zero : Lfs(base, kTwenty);
      }
    }
  }
  if (f0 < Lfs(base, speed)) {
    Stfs(base, speed, f0);
  }
  return r3;
}

uint64_t UpdateState3(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_821460C8 AiDriver::UpdateState3");
  double f11 = Lfs(base, speed);
  PickTarget(base, driver);
  const uint32_t target = Load(base, driver + 480);
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint32_t record = target * 40 + driver;
  const double gap = Lfs(base, record + 172);
  uint64_t r3 = driver;
  if (gap > Lfs(base, kZero)) {
    r3 = OfIndex(base, target);
    const double again = Lfs(base, record + 172);
    double f0;
    if (again < Lfs(base, 0x82002674)) {
      f0 = Lfs(base, uint32_t(r3) + 100);
    } else if (again < Lfs(base, kTen)) {
      f0 = Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x82002A2C));
    } else if (again < Lfs(base, kHundred)) {
      f0 = Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x82002A2C));
      const double most = Lfs(base, 0x82000F14);
      if (!(f0 < most)) {
        f0 = most;
      }
    } else {
      f0 = Lfs(base, kTwenty);
    }
    if (f0 < Lfs(base, speed)) {
      Stfs(base, speed, f0);
    }
    return r3;
  }
  if (gap < Lfs(base, 0x8200895C)) {
    f11 = Lfs(base, kFiveHundred);
  } else if (gap < Lfs(base, 0x82008E58)) {
    r3 = OfIndex(base, target);
    f11 = Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x820029D4));
  }
  if (f11 > Lfs(base, speed)) {
    Stfs(base, speed, f11);
  }
  return r3;
}

uint64_t UpdateState6(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_821462A0 AiDriver::UpdateState6");
  PickTargetBehind(base, driver);
  const uint32_t target = Load(base, driver + 480);
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint64_t other = OfIndex(base, target);
  const uint32_t theirs = uint32_t(other);
  if (int32_t(Load(base, theirs + 32)) != 6) {
    return other;
  }
  const uint32_t again = Load(base, driver + 480);
  const double mine = Lfs(base, driver + 124);
  const double their_length = Lfs(base, theirs + 124);
  double f12 = Lfs(base, theirs + 100);
  const double f0 = Lfs(base, 0x820021E8);
  const double gap = Fneg(Lfs(base, again * 40 + driver + 172));
  const double closing = Fsubs(Fnmsubs(mine, f0, gap), their_length);
  if (!(closing < Lfs(base, kZero))) {
    double scale = Lfs(base, driver + 100);
    const double factor = Lfs(base, driver + 136);
    const double hundred = Lfs(base, kHundred);
    scale = scale > hundred ? Fdivs(hundred, scale) : Lfs(base, kOne);
    f12 = Fmadds(Fmuls(scale, factor), closing, f12);
  }
  const double limit = Fmuls(f12, Lfs(base, 0x82002A3C));
  if (Lfs(base, speed) > limit) {
    Stfs(base, speed, limit);
  }
  return other;
}

uint64_t UpdateState(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82139AC8 AiDriver::UpdateState");
  switch (int32_t(Load(base, driver + 32))) {
    case 2:
      return UpdateState2(base, driver, speed);
    case 3:
      return UpdateState3(base, driver, speed);
    case 4:
      UpdateState4(base, driver, speed);
      return driver;
    case 5:
      return UpdateState5(base, driver, speed);
    case 6: {
      double f0 = Fsubs(Lfs(base, driver + 100), Lfs(base, kTen));
      const double least = Lfs(base, 0x82008948);
      if (f0 < least) {
        f0 = least;
      }
      if (f0 < Lfs(base, driver + 6088)) {
        Stfs(base, driver + 6088, f0);
      }
      return UpdateState6(base, driver, speed);
    }
    default:
      return driver;
  }
}

// --- The target speed ---

namespace {
// The ring entry k (from the path's start +5138, modulo 80) when k is below
// the entries' count +5136, else 0.
uint32_t RingEntry(uint8_t* base, uint32_t path, int32_t k) {
  if (!(LoadS16(base, path + 5136) > k)) {
    return 0;
  }
  const int32_t at = int32_t(uint32_t(LoadS16(base, path + 5138)) + uint32_t(k));
  return path + uint32_t(at - (at / 80) * 80) * 64 + 16;
}
}  // namespace

uint64_t LimitForCorner(uint8_t* base, uint32_t driver, uint32_t scale, uint32_t record,
                        uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213A620 AiDriver::LimitForCorner");
  uint64_t r3 = driver;
  CopyWord(base, scale, frame + 88);
  const uint32_t ahead = driver + 5654;  // the look-ahead (the path's +5142)
  const uint32_t data = Load(base, record + 0);
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 84, zero);
  const uint32_t speeds = data + 12;
  const uint32_t target = data + 8;
  const int32_t offset = LoadS16(base, record + 10);
  const uint32_t before = uint32_t(1 - offset);
  const int32_t entries = LoadS16(base, driver + 5648);
  const int32_t start = int32_t(uint32_t(LoadS16(base, data + 50)) + uint32_t(offset));
  const int32_t middle_at = LoadS16(base, data + 52);
  Stfs(base, record + 20, zero);
  guest::StoreU32(base, frame + 92, before);
  const int32_t end = int32_t(uint32_t(middle_at) + uint32_t(offset));
  const uint32_t path = driver + 512;
  if (start >= 0 && start < entries) {
    const uint32_t entry = RingEntry(base, path, start);
    if (entry != 0) {
      double a, b;
      if (Lfs(base, record + 16) < zero) {
        a = Lfs(base, driver + 144);
        b = Lfs(base, entry + 40);
      } else {
        a = Lfs(base, entry + 44);
        b = Lfs(base, driver + 144);
      }
      Stfs(base, record + 20, Fsubs(a, b));
    }
  }
  if (int32_t(Load(base, driver + 6032)) == 1) {
    guest::StoreU32(base, record + 40, 0);
  } else {
    int32_t skill = LoadS16(base, driver + 6056);
    if (skill < -100) {
      skill = -100;
    } else if (skill > 100) {
      skill = 100;
    }
    guest::StoreU16(base, frame + 80, uint16_t(skill));
    ClearCornerKind(base, record);
    r3 = record;
  }
  if (entries > 1) {
    LimitByCurve(base, driver, record, frame + 84, frame + 92, RingEntry(base, path, 1));
    r3 = driver;
    const int32_t look = LoadS16(base, ahead);
    const int32_t next = look + 1;
    if (entries > next) {
      const uint32_t entry = RingEntry(base, path, next);
      guest::StoreU32(base, frame + 92, uint32_t(look) + before);
      LimitByCurve(base, driver, record, frame + 84, frame + 92, entry);
    }
  }
  const auto blend = [&](uint32_t pair, uint32_t slices) {
    guest::StoreU32(base, frame + 92, slices);
    *f1 = LimitSpeedBlend(base, driver, frame + 88, pair, target, frame + 84, frame + 92,
                          frame - 96);
    *f1_set = true;
    r3 = driver;
  };
  uint32_t last;
  if (end >= 0) {
    if (start > 0) {
      blend(speeds, uint32_t(LoadS16(base, record + 10) - LoadS16(base, ahead)));
    }
    if (start < end) {
      blend(speeds + 8, uint32_t(start - LoadS16(base, ahead)));
    }
    last = uint32_t(end - LoadS16(base, ahead));
  } else {
    if (int32_t(uint32_t(LoadS16(base, data + 54)) + uint32_t(LoadS16(base, record + 10))) < 0) {
      return r3;
    }
    last = uint32_t(-end);
  }
  blend(speeds + 16, last);
  return r3;
}

uint64_t UpdateTargetSpeed(uint8_t* base, uint32_t driver, uint32_t scale, uint32_t frame,
                           double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AB98 AiDriver::UpdateTargetSpeed");
  uint64_t r3 = driver;
  const uint32_t flags = Load(base, driver + 16);
  const uint32_t vehicle = Load(base, driver + 160);
  uint32_t from;
  if ((flags & 4) != 0) {
    from = driver + 12;
  } else {
    // The vehicle's flags read whatever the vehicle, as the original does.
    const uint32_t state = Load(base, vehicle + 6148);
    if ((state & 0x60000000u) != 0) {
      from = 0x82008930;  // 70.0
    } else if ((state & 0x1C000000u) != 0) {
      from = kZero;
    } else {
      from = 0x82008948;  // 73.0
    }
  }
  CopyWord(base, from, driver + 6088);
  if (Load(base, driver + 32) != 0) {
    r3 = UpdateState(base, driver, driver + 6088);
  }
  if (LoadS16(base, driver + 5674) > 0) {
    int32_t i = 0;
    uint32_t record = driver + 5676;
    do {
      r3 = LimitForCorner(base, driver, scale, record, frame - 192, f1, f1_set);
      i++;
      record += 44;
    } while (i < LoadS16(base, driver + 5674));
  }
  return r3;
}

uint64_t UpdateSpeed(uint8_t* base, uint32_t driver, uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213ACD0 AiDriver::UpdateSpeed");
  int32_t skill = LoadS16(base, driver + 6058);
  if (skill < -100) {
    skill = -100;
  } else if (skill > 100) {
    skill = 100;
  }
  const double f0 = Lfs(base, 0x82002CE8);
  const double scale = Fmadds(IntToFloat(skill), f0, Lfs(base, kOne));
  Stfs(base, frame + 80, scale);
  return UpdateTargetSpeed(base, driver, frame + 80, frame - 128, f1, f1_set);
}

// --- Steering at the target and the pedals ---

uint32_t SteerToTarget(uint8_t* base, uint32_t driver, double* f1) {
  ITC_NATIVE_RUNNING("sub_82138D78 AiDriver::SteerToTarget");
  const uint32_t path = driver + 512;
  const uint32_t file = Load(base, track_route::kRoute);
  const uint32_t slow = IsSlow(base, path);
  const double speed = Lfs(base, driver + 100);
  double reach = Fmuls(speed, Lfs(base, slow == 0 ? 0x82008944 : 0x82008940));
  const double least = Lfs(base, 0x82002834);
  if (reach < least) {
    reach = least;
  }
  double more = Lfs(base, driver + 156);
  const double zero = Lfs(base, kZero);
  if (more < zero) {
    more = zero;
  }
  const double ahead = Fadds(reach, more);
  *f1 = ahead;
  int32_t slices = strings_and_font::steps::Fctiwz(Fdivs(ahead, Lfs(base, file + 8)));
  if (slices > 78) {
    slices = 78;
  }
  const uint32_t target = driver + 6096;
  guest::StoreU16(base, driver + 5654, uint16_t(slices));
  if (TargetPoint(base, path, ahead, target) == 0) {
    return 0;
  }
  const double dx = Fsubs(Lfs(base, target + 0), Lfs(base, driver + 80));
  const double dz = Fsubs(Lfs(base, driver + 6104), Lfs(base, driver + 88));
  const uint32_t angle = driver + 6116;
  if (dx == zero && dz == zero) {
    Stfs(base, angle, zero);
  } else {
    const double toward = double(float(c_runtime::Atan2(dx, dz)));
    Stfs(base, angle, Fsubs(toward, Lfs(base, driver + 96)));
    WrapAngle(base, angle);
    Stfs(base, angle, Fneg(Lfs(base, angle)));
  }
  const uint32_t vehicle = Load(base, driver + 160);
  const double cos = c_runtime::Cos(Lfs(base, angle));
  const double sin = c_runtime::Sin(Lfs(base, angle));
  *f1 = sin;
  const double s = double(float(sin));
  const double across = Lfs(base, vehicle + 384);
  const double c = double(float(cos));
  const uint32_t now = Load(base, driver + 160);
  const double along = Lfs(base, vehicle + 392);
  const double turned = Lfs(base, angle);
  Stfs(base, driver + 152, Fmadds(along, c, Fmuls(s, across)));
  Stfs(base, driver + 6112, Fadds(Lfs(base, now + 92), turned));
  WrapAngle(base, driver + 6112);
  return 1;
}

void UpdatePedals(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A428 AiDriver::UpdatePedals");
  const uint32_t kind2 = HasKind2Ahead(base, driver + 512);
  ThrottleForSpeed(base, driver);
  const double zero = Lfs(base, kZero);
  uint32_t flag = 0;
  if (kind2 == 0) {
    const double yaw = Fabs(Lfs(base, driver + 104));
    if (yaw > zero) {
      const double throttle = Lfs(base, driver + 6140);
      Stfs(base, driver + 6140, throttle < yaw ? zero : Fsubs(throttle, yaw));
    }
    EaseThrottleForSteer(base, driver);
    flag = 1;
  }
  guest::StoreU32(base, frame + 80, flag);
  EaseThrottleForSlide(base, driver, frame + 80);
  const double brake = Lfs(base, driver + 6144);
  if (brake > zero) {
    Stfs(base, driver + 6140, zero);
    const double step = Lfs(base, 0x8200296C);
    Stfs(base, driver + 6144, brake > step ? Fsubs(brake, step) : zero);
  }
  double steer = Fmuls(Lfs(base, driver + 152), Lfs(base, 0x82002968));
  if (steer < zero) {
    steer = Fneg(steer);
  }
  double request = Lfs(base, driver + 6144);
  const double one = Lfs(base, kOne);
  bool kept = false;
  if (steer < request) {
    if (request < zero) {
      request = zero;
    } else {
      const double most = Fsubs(one, steer);
      if (request > most) {
        request = most;
      }
    }
    const double slide = Lfs(base, driver + 6120);
    Stfs(base, driver + 6144, request);
    double turn = Fmuls(slide, Lfs(base, 0x820021E8));
    if (turn < zero) {
      turn = Fneg(turn);
    }
    if (turn < request) {
      if (request < zero) {
        request = zero;
      } else {
        const double most = Fsubs(one, turn);
        if (request > most) {
          request = most;
        }
      }
      Stfs(base, driver + 6144, request);
      kept = true;
    }
  }
  if (!kept) {
    Stfs(base, driver + 6144, zero);
  }
  const double limit = Lfs(base, kFifth);
  const auto approach = [&](uint32_t now_at, uint32_t toward_at) {
    double move = Fsubs(Lfs(base, toward_at), Lfs(base, now_at));
    if (move > limit) {
      move = limit;
    }
    const double now = Fadds(Lfs(base, now_at), move);
    Stfs(base, now_at, now);
    if (now < zero) {
      Stfs(base, now_at, zero);
    } else if (now > one) {
      Stfs(base, now_at, one);
    }
  };
  approach(driver + 6124, driver + 6140);
  approach(driver + 6128, driver + 6144);
}

// --- The drivers' table and the path's lines and ranges ---

namespace {
// The ring entry k (from the path's start +5138, modulo 80, as divw
// truncates), without a check against the entries' count.
uint32_t RingAt(uint8_t* base, uint32_t path, int32_t k) {
  const int32_t at = int32_t(uint32_t(LoadS16(base, path + 5138)) + uint32_t(k));
  return path + (uint32_t(at - (at / 80) * 80) << 6) + 16;
}
}  // namespace

bool Create(uint8_t* base, uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213B1E0 AiDrivers::Create");
  const auto block = memory::crt::OperatorNew(base, 43120);
  if (!block) {
    return false;
  }
  const uint32_t made = *block;
  if (made != 0) {
    for (uint32_t i = 0; i < 7; i++) {
      InitPath(base, made + kDriverBytes * i);
    }
  }
  guest::StoreU32(base, kDrivers, made);
  *r3 = made;
  if (made == 0) {
    return true;
  }
  guest::ForEachHostPart(base, made, 43120, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, i);
    const uint32_t driver = kDriverBytes * i + Load(base, kDrivers);
    ClearSkill(base, driver + 6048);
    guest::StoreU32(base, driver + 500, 0);
    *r3 = ResetDriver(base, driver);
    guest::StoreU32(base, kDriverBytes * i + Load(base, kDrivers), i);
    if (vehicle != 0) {
      guest::StoreU32(base, kDriverBytes * i + Load(base, kDrivers) + 160, vehicle);
    }
  }
  return true;
}

void ApplyCornerLine(uint8_t* base, uint32_t path, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8213BB58 AiPath::ApplyCornerLine");
  const uint32_t record = Load(base, index) * 44u + path + 5164;
  const uint32_t data = Load(base, record + 0);
  if (data == 0) {
    return;
  }
  const uint32_t table = Load(base, record + 4);
  if (table == 0) {
    return;
  }
  const int32_t count = LoadS16(base, data + 0);
  const int32_t first = LoadS16(base, record + 14);
  if (first >= count) {
    return;
  }
  int32_t start = int16_t(uint16_t(guest::LoadU16(base, record + 10) + uint32_t(first)));
  if (start >= LoadS16(base, path + 5136)) {
    return;
  }
  if (start < 0) {
    guest::StoreU16(base, record + 14, uint16_t(first - start));
    start = 0;
  }
  int32_t k = start;
  int32_t n = LoadS16(base, path + 5136);
  while (k < n) {
    const int32_t at = LoadS16(base, record + 14);
    if (at >= count) {
      return;
    }
    const uint32_t entry = RingEntry(base, path, k);
    CopyWord(base, uint32_t(at) * 8u + table + 4, entry + 48);
    if (guest::LoadU8(base, record + 36) == 1) {
      const int32_t until = LoadS16(base, data + 50);
      const int32_t now = LoadS16(base, record + 14);
      if (now < until) {
        const double slope = Lfs(base, record + 28);
        const double offset = Lfs(base, entry + 48);
        Stfs(base, entry + 48, Fmadds(IntToFloat(until - now), slope, offset));
      }
    }
    if (guest::LoadU8(base, record + 37) == 1) {
      const int32_t from = LoadS16(base, data + 52);
      const int32_t now = LoadS16(base, record + 14);
      if (now > from) {
        const double slope = Lfs(base, record + 32);
        const double offset = Lfs(base, entry + 48);
        Stfs(base, entry + 48, Fmadds(IntToFloat(now - from), slope, offset));
      }
    }
    guest::StoreU16(base, record + 14, uint16_t(guest::LoadU16(base, record + 14) + 1));
    k = int16_t(k + 1);
    n = LoadS16(base, path + 5136);
  }
}

void ApplyCornerLines(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CFC8 AiPath::ApplyCornerLines");
  int32_t i = 0;
  guest::StoreU32(base, frame + 80, 0);
  if (LoadS16(base, path + 5162) <= 0) {
    return;
  }
  do {
    ApplyCornerLine(base, path, frame + 80);
    i++;
    guest::StoreU32(base, frame + 80, uint32_t(i));
  } while (i < LoadS16(base, path + 5162));
}

void KeepToCornerSide(uint8_t* base, uint32_t path, uint32_t record, uint32_t count,
                      uint32_t margin, uint32_t shift) {
  ITC_NATIVE_RUNNING("sub_8213CA18 AiPath::KeepToCornerSide");
  int32_t k = LoadS16(base, record + 10);
  if (k < 1) {
    k = 1;
  }
  const double direction = Lfs(base, record + 16);
  const int32_t n = int32_t(Load(base, count));
  double m = Lfs(base, margin);
  const double zero = Lfs(base, kZero);
  const double by = Lfs(base, shift);
  if (direction < zero) {
    if (by < zero) {
      m = Fadds(by, m);
    }
    for (; k < n; k++) {
      const uint32_t entry = RingAt(base, path, k);
      const double high = Fsubs(Lfs(base, entry + 36), m);
      if (high < Lfs(base, entry + 44)) {
        Stfs(base, entry + 44, high);
      }
      const double low = Lfs(base, entry + 40);
      if (Lfs(base, entry + 44) < low) {
        Stfs(base, entry + 44, low);
      }
    }
  } else {
    if (by > zero) {
      m = Fadds(by, m);
    }
    for (; k < n; k++) {
      const uint32_t entry = RingAt(base, path, k);
      const double low = Fadds(Lfs(base, entry + 32), m);
      if (low > Lfs(base, entry + 40)) {
        Stfs(base, entry + 40, low);
      }
      const double high = Lfs(base, entry + 44);
      if (Lfs(base, entry + 40) > high) {
        Stfs(base, entry + 40, high);
      }
    }
  }
}

uint32_t SetRanges(uint8_t* base, uint32_t path, uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D670 AiPath::SetRanges");
  uint32_t r3 = path;
  const uint32_t driver = Load(base, path + 0);
  const uint32_t mode = Load(base, path + 5520);
  const double steer = Lfs(base, driver + 152);
  const double half = Lfs(base, driver + 116);
  const double shift = Fmuls(steer, Lfs(base, kFifth));
  Stfs(base, frame + 80, shift);
  Stfs(base, frame + 84, Fadds(half, Lfs(base, 0x82002A14)));
  if (int32_t(mode) == 1) {
    Stfs(base, frame + 80, Lfs(base, kZero));
  } else {
    const double lowest = Lfs(base, kMinusOne);
    if (shift < lowest) {
      Stfs(base, frame + 80, lowest);
    } else {
      const double highest = Lfs(base, kOne);
      if (shift > highest) {
        Stfs(base, frame + 80, highest);
      }
    }
  }
  for (int32_t i = 1; i < int32_t(Load(base, count)); i++) {
    r3 = RingAt(base, path, i);
    Widen(base, r3, frame + 84, frame + 80);
  }
  if (LoadS16(base, path + 5162) > 0) {
    int32_t i = 0;
    uint32_t record = path + 5164;
    do {
      r3 = path;
      KeepToCornerSide(base, path, record, count, driver + 128, frame + 80);
      i++;
      record += 44;
    } while (i < LoadS16(base, path + 5162));
  }
  return r3;
}

uint32_t DropOffRouteCorners(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213DC08 AiPath::DropOffRouteCorners");
  if (LoadS16(base, path + 5162) <= 0) {
    return path;
  }
  uint32_t r3 = 0;
  int32_t kept = 0;
  for (;;) {
    const uint32_t data = Load(base, path + 44 * uint32_t(kept) + 5164);
    r3 = data == 0 ? 0 : SharesSections(base, path, data);
    if (int32_t(r3) != 1) {
      break;
    }
    kept = int16_t(kept + 1);
    if (!(kept < LoadS16(base, path + 5162))) {
      return r3;
    }
  }
  const double zero = Lfs(base, kZero);
  if (kept > 1) {
    const uint32_t last = path + 44 * uint32_t(kept) + 5120;  // record kept - 1
    if (guest::LoadU8(base, last + 37) == 1) {
      const uint32_t data = Load(base, last + 0);
      guest::StoreU8(base, last + 37, 0);
      const int32_t at = LoadS16(base, last + 14);
      const uint16_t most = guest::LoadU16(base, data + 52);
      if (at > int32_t(int16_t(most))) {
        guest::StoreU16(base, last + 14, most);
      }
      Stfs(base, last + 32, zero);
    }
  }
  int32_t k = kept;
  if (k < LoadS16(base, path + 5162)) {
    const double one = Lfs(base, kOne);
    do {
      const uint32_t record = path + 44 * uint32_t(k) + 5164;
      k = int16_t(k + 1);
      Stfs(base, record + 16, one);
      guest::StoreU32(base, record + 0, 0);
      Stfs(base, record + 20, zero);
      guest::StoreU32(base, record + 4, 0);
      Stfs(base, record + 28, zero);
      guest::StoreU16(base, record + 8, 0);
      Stfs(base, record + 32, zero);
      guest::StoreU16(base, record + 10, 0);
      guest::StoreU16(base, record + 12, 0);
      guest::StoreU16(base, record + 14, 0);
      guest::StoreU32(base, record + 24, 0);
      guest::StoreU8(base, record + 36, 0);
      guest::StoreU8(base, record + 37, 0);
      guest::StoreU32(base, record + 40, 0);
    } while (k < LoadS16(base, path + 5162));
  }
  guest::StoreU16(base, path + 5162, uint16_t(kept));
  return r3;
}

// --- The path's offsets, narrowing and branches ---

void SpreadOffset(uint8_t* base, uint32_t path, uint32_t first, uint32_t last, uint32_t amount,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213B890 AiPath::SpreadOffset");
  int32_t low = int32_t(Load(base, first));
  int32_t i = int32_t(Load(base, last));
  if (!(i > low)) {
    return;
  }
  const double share =
      Fdivs(Lfs(base, amount), IntToFloat(int32_t(uint32_t(i) - uint32_t(low) + 1)));
  for (;;) {
    i = int32_t(uint32_t(i) - 1);
    guest::StoreU32(base, frame + 80, uint32_t(i));
    if (i < low) {
      return;
    }
    const uint32_t entry = RingEntry(base, path, i);
    const double offset = Lfs(base, entry + 24);
    const double least = Lfs(base, entry + 32);
    const double moved =
        Fmadds(IntToFloat(int32_t(uint32_t(i) - uint32_t(low) + 1)), share, offset);
    uint32_t end;
    if (moved < least) {
      end = entry + 32;
    } else if (moved > Lfs(base, entry + 36)) {
      end = entry + 36;
    } else {
      Stfs(base, entry + 24, moved);
      low = int32_t(Load(base, first));
      i = int32_t(Load(base, frame + 80));
      continue;
    }
    Stfs(base, frame + 88, Fsubs(Lfs(base, end), Lfs(base, entry + 24)));
    SpreadOffset(base, path, first, frame + 80, frame + 88, frame - 128);
    const double bound = Lfs(base, end);
    CopyWord(base, end, entry + 24);
    int32_t high = int32_t(Load(base, last));
    const int32_t at = int32_t(Load(base, frame + 80));
    if (!(at < int32_t(uint32_t(high) - 1))) {
      return;
    }
    const double step =
        Fdivs(Fsubs(bound, moved), IntToFloat(int32_t(uint32_t(high) - uint32_t(at))));
    for (int32_t j = int32_t(uint32_t(at) + 1); j < high; j = int32_t(uint32_t(j) + 1)) {
      // The same entry each time, as the original does.
      const double now = Lfs(base, entry + 24);
      Stfs(base, entry + 24,
           Fmadds(IntToFloat(int32_t(uint32_t(high) - uint32_t(j))), step, now));
      high = int32_t(Load(base, last));
    }
    return;
  }
}

void ClampOffset(uint8_t* base, uint32_t path, uint32_t entry, uint32_t last, uint32_t first,
                 uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CF58 AiPath::ClampOffset");
  const double offset = Lfs(base, entry + 24);
  uint32_t end;
  if (offset < Lfs(base, entry + 32)) {
    end = entry + 32;
  } else if (offset > Lfs(base, entry + 36)) {
    end = entry + 36;
  } else {
    return;
  }
  Stfs(base, frame + 80, Fsubs(Lfs(base, end), offset));
  SpreadOffset(base, path, first, last, frame + 80, frame - 128);
  CopyWord(base, end, entry + 24);
}

uint32_t NarrowToOtherEx(uint8_t* base, uint32_t entry, uint32_t other, uint32_t step,
                         uint32_t offset, uint32_t low, uint32_t high, uint32_t limit) {
  ITC_NATIVE_RUNNING("sub_8213C4C0 AiPathEntry::NarrowToOtherEx");
  if (other == 0) {
    return 0;
  }
  const uint32_t record = Load(base, other + 52);
  if (record == 0) {
    return 0;
  }
  double lo = Fsubs(Lfs(base, other + 32), Lfs(base, low));
  double hi = Fadds(Lfs(base, other + 36), Lfs(base, high));
  const double least = Lfs(base, record + 24);
  if (lo < least) {
    lo = least;
  }
  const double most = Lfs(base, record + 28);
  if (hi > most) {
    hi = most;
  }
  const double by = Lfs(base, offset);
  lo = Fadds(by, lo);
  hi = Fadds(by, hi);
  if (lo > hi) {
    hi = Fmuls(Fadds(hi, lo), Lfs(base, kHalf));
    lo = hi;
  }
  uint32_t narrowed = 0;
  if (Lfs(base, entry + 32) < lo) {
    Stfs(base, entry + 32, lo);
    Stfs(base, entry + 40, lo);
    const double margin = Lfs(base, low);
    if (margin < Lfs(base, limit)) {
      Stfs(base, low, Fadds(Lfs(base, step), margin));
    }
    narrowed = 1;
  } else {
    CopyWord(base, step, low);
  }
  if (Lfs(base, entry + 36) > hi) {
    Stfs(base, entry + 36, hi);
    Stfs(base, entry + 44, hi);
    const double margin = Lfs(base, high);
    if (margin < Lfs(base, limit)) {
      Stfs(base, high, Fadds(Lfs(base, step), margin));
    }
    return 1;
  }
  CopyWord(base, step, high);
  return narrowed;
}

uint32_t NarrowBehind(uint8_t* base, uint32_t path, uint32_t other, uint32_t entry,
                      uint32_t step, uint32_t limit, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D140 AiPath::NarrowBehind");
  CopyWord(base, step, frame + 84);
  CopyWord(base, step, frame + 80);
  int32_t k = int32_t(uint32_t(LoadS16(base, path + 5136)) - 1);
  uint32_t r3;
  for (;;) {
    Stfs(base, frame + 88, Fneg(Lfs(base, other + 28)));
    r3 = NarrowToOtherEx(base, entry, other, step, frame + 88, frame + 84, frame + 80, limit);
    if (int32_t(r3) != 1) {
      return r3;
    }
    k = int32_t(uint32_t(k) - 1);
    other = entry;
    if (k < 0) {
      return r3;
    }
    entry = RingEntry(base, path, k);
    if (entry == 0) {
      return r3;
    }
  }
}

int32_t CompareBranches(uint8_t* base, uint32_t path, uint32_t a, uint32_t b, uint32_t a_number,
                        uint32_t b_number, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_8213C788 AiPath::CompareBranches");
  if (uint32_t((guest::LoadU8(base, b + 2) & 0xF) - 1) <= 3) {
    return -1;
  }
  if (uint32_t((guest::LoadU8(base, a + 2) & 0xF) - 1) <= 3) {
    return 1;
  }
  const uint32_t driver = Load(base, path + 0);
  const uint32_t vehicle = Load(base, driver + 160);
  const uint32_t flag = vehicle == 0 ? 0 : (Load(base, vehicle + 6148) >> 2) & 1;
  if (flag == 0) {
    if (uint32_t(guest::LoadU8(base, b + 3)) - 7 <= 1) {
      return -1;
    }
    if (uint32_t(guest::LoadU8(base, a + 3)) - 7 <= 1) {
      return 1;
    }
  }
  uint32_t passed = 0;
  const uint32_t object = Load(base, driver + 500);
  if (object != 0 && uint32_t(LoadS16(base, object + 88) - 4) <= 1) {
    uint32_t list = PreferredSections(base, object + 36, object + 90);
    if (list != 0) {
      int32_t section = LoadS16(base, list + 0);
      if (section >= 0) {
        const int32_t first = LoadS16(base, a_number);
        do {
          if (section == first) {
            return -1;
          }
          if (section == LoadS16(base, b_number)) {
            return 1;
          }
          list += 2;
          passed++;
          section = LoadS16(base, list + 0);
        } while (section >= 0);
      }
    }
  }
  if (masks == 0) {
    return 0;
  }
  uint32_t at = (passed << 2) + masks;
  uint32_t mask = Load(base, at);
  if (int32_t(mask) == 0) {
    return 0;
  }
  const uint32_t b_bit = Slw(1, guest::LoadU8(base, b + 3));
  const uint32_t a_bit = Slw(1, guest::LoadU8(base, a + 3));
  do {
    if ((a_bit & mask) != 0) {
      return (b_bit & mask) == 0 ? -1 : 0;
    }
    if ((b_bit & mask) != 0) {
      return 1;
    }
    at += 4;
    mask = Load(base, at);
  } while (int32_t(mask) != 0);
  return 0;
}

uint32_t NormalizeByBranch(uint8_t* base, uint32_t path, uint32_t entry, uint32_t position,
                           uint32_t lane, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_8213C908 AiPath::NormalizeByBranch");
  const uint32_t route = track_route::kRoute;
  const uint32_t object = Load(base, Load(base, path + 0) + 500);
  uint32_t on;
  if (object != 0 && uint32_t(LoadS16(base, object + 88) - 4) <= 1) {
    const uint32_t list = PreferredSections(base, object + 36, object + 90);
    on = track_route::NormalizePath(base, route, position, list, masks);
  } else {
    on = track_route::Normalize(base, route, position, masks);
  }
  if (int32_t(on) == 0) {
    return 0;
  }
  const uint32_t file = Load(base, route);
  const uint32_t section = guest::LoadU16(base, position + 0);
  const int32_t slice = LoadS16(base, position + 2);
  const uint32_t table = Load(base, file + 12);
  const double zero = Lfs(base, kZero);
  const uint32_t record = Load(base, (section << 2) + table) + uint32_t(slice) * 84 + 124;
  guest::StoreU32(base, entry + 52, record);
  guest::StoreU16(base, entry + 16, guest::LoadU16(base, position + 0));
  guest::StoreU16(base, entry + 18, guest::LoadU16(base, position + 2));
  const int32_t at = LoadS16(base, lane);
  Stfs(base, entry + 24, zero);
  Stfs(base, entry + 48, zero);
  guest::StoreU32(base, entry + 20, uint32_t(at));
  CopyWord(base, record + 0, entry + 0);
  CopyWord(base, record + 4, entry + 4);
  CopyWord(base, record + 8, entry + 8);
  return 1;
}

// --- Making room for the other racers ---

uint64_t MeasureOthers(uint8_t* base, uint32_t path, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213B650 AiPath::MeasureOthers");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint64_t r3 = uint32_t(count);
  const double zero = Lfs(base, kZero);
  int32_t at = 2;     // kept from one racer to the next, as the original does
  double sum = zero;  // the same
  for (int32_t k = 0; k < count; k++) {
    const int32_t index = int32_t(Load(base, Load(base, path + 0) + 452 + 4 * uint32_t(k)));
    if (index < 0) {
      continue;
    }
    r3 = OfIndex(base, uint32_t(index));
    const uint32_t other = uint32_t(r3);
    const uint32_t driver = Load(base, path + 0);
    const uint32_t entry = out + 20 * uint32_t(k);
    Stfs(base, entry + 4, zero);
    Stfs(base, entry + 8, zero);
    guest::StoreU16(base, entry + 2, 0);
    Stfs(base, entry + 12, zero);
    Stfs(base, entry + 16, zero);
    const uint32_t record = uint32_t(index) * 40 + driver + 172;
    guest::StoreU16(base, entry + 0, 0);
    const uint16_t ring = guest::LoadU16(base, record + 26);
    const int32_t last = int16_t(ring);
    if (!(at > last)) {
      const int32_t n = LoadS16(base, path + 5136);
      do {
        if (!(at < n)) {
          break;
        }
        const int32_t m = int32_t(uint32_t(LoadS16(base, path + 5138)) + uint32_t(at));
        at++;
        sum = Fadds(Lfs(base, path + (uint32_t(m - (m / 80) * 80) << 6) + 44), sum);
      } while (!(at > last));
    }
    guest::StoreU16(base, entry + 0, ring);
    uint16_t next;
    if ((guest::LoadU16(base, record + 28) & 4) != 0) {
      next = uint16_t(guest::LoadU16(base, record + 26) + 1);
    } else {
      const int32_t after = LoadS16(base, record + 26) + 1;
      const int32_t look = LoadS16(base, path + 5140);
      next = uint16_t(look < after ? after : look);
    }
    Stfs(base, entry + 8, sum);
    guest::StoreU16(base, entry + 2, next);
    const uint32_t me = Load(base, path + 0);
    Stfs(base, entry + 12, Fadds(Lfs(base, me + 116), Lfs(base, other + 116)));
    const int32_t n = LoadS16(base, path + 5136);
    if (!(LoadS16(base, record + 26) < n)) {
      guest::StoreU32(base, Load(base, path + 0) + 452 + 4 * uint32_t(k), 0xFFFFFFFFu);
      return r3;
    }
    const uint32_t file = Load(base, track_route::kRoute);
    const double gap = Lfs(base, record + 0);
    const double length = Lfs(base, file + 8);
    int32_t slot;
    if (gap > length) {
      CopyWord(base, other + 144, entry + 4);
      if (LoadS16(base, path + 5136) <= 0) {
        continue;
      }
      if ((guest::LoadU16(base, record + 28) & 0x10) == 0) {
        continue;
      }
      slot = LoadS16(base, path + 5138);
    } else {
      int32_t ahead = int32_t(
          uint32_t(strings_and_font::steps::Fctiwz(Fdivs(Fneg(gap), length))) + 1);
      if (ahead < 0) {
        ahead = 0;
      }
      if (!(ahead < n)) {
        CopyWord(base, other + 144, entry + 4);
        continue;
      }
      slot = int32_t(uint32_t(LoadS16(base, path + 5138)) + uint32_t(ahead));
    }
    const double ox = Lfs(base, other + 80);
    const double oz = Lfs(base, other + 88);
    const uint32_t slice = Load(base, path + (uint32_t(slot - (slot / 80) * 80) << 6) + 68);
    double dx = Fsubs(ox, Lfs(base, slice + 0));
    const double dz = Fsubs(oz, Lfs(base, slice + 8));
    dx = Fmuls(dx, Lfs(base, slice + 36));
    Stfs(base, entry + 4, Fmadds(dz, Lfs(base, slice + 40), dx));
  }
  return r3;
}

namespace {
// AvoidOnLeft's and AvoidOnRight's spread: the width over a's +0 - 1
// entries (at frame + 80), kept within the path's step +5516.
void SpreadWidth(uint8_t* base, uint32_t path, uint32_t a, double line, double mine,
                 uint32_t frame) {
  const double zero = Lfs(base, kZero);
  const int32_t first = LoadS16(base, a + 0);
  double width = Fsubs(line, mine);
  Stfs(base, frame + 80, width);
  if (first > 1) {
    width = Fdivs(width, IntToFloat(first - 1));
    Stfs(base, frame + 80, width);
  }
  const double step = Lfs(base, path + 5516);
  if (Fadds(step, width) < zero) {
    Stfs(base, frame + 80, Fneg(step));
  } else if (Fsubs(width, step) > zero) {
    Stfs(base, frame + 80, step);
  }
}
}  // namespace

uint64_t AvoidOnLeft(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CB30 AiPath::AvoidOnLeft");
  const uint32_t driver = Load(base, path + 0);
  uint64_t r3 = driver;
  const uint32_t mode = Load(base, path + 5520);
  const double across = Fmuls(Lfs(base, a + 16), Lfs(base, record + 12));
  const double zero = Lfs(base, kZero);
  double steer = zero;
  const double mine = Lfs(base, driver + 144);
  CopyWord(base, driver + 144, frame + 96);
  if (int32_t(mode) != 1) {
    steer = Lfs(base, driver + 152);
  }
  if (mine < across) {
    guest::StoreU32(base, frame + 88, mode == 1 ? 0 : 1);
    AdjustForOther(base, driver, record, frame + 88);
  }
  double line = Fadds(across, Lfs(base, 0x82002A3C));
  if (mine < line && Lfs(base, record + 0) < zero) {
    line = Fmadds(IntToFloat(LoadS16(base, a + 2) + 1), Fsubs(line, mine), line);
  }
  line = Fadds(line, Lfs(base, 0x8200296C));
  Stfs(base, frame + 88, line);
  if (steer > zero) {
    line = Fmadds(steer, Lfs(base, kFifth), line);
    Stfs(base, frame + 88, line);
  }
  if (!(mine < line)) {
    for (int32_t j = 1; j < LoadS16(base, a + 2); j++) {
      const uint32_t entry = RingAt(base, path, j);
      line = Fsubs(line, Lfs(base, entry + 28));
      if (Lfs(base, entry + 40) < line) {
        const double most = Lfs(base, entry + 44);
        Stfs(base, entry + 40, line);
        if (line > most) {
          Stfs(base, entry + 40, most);
        }
      }
    }
    return r3;
  }
  SpreadWidth(base, path, a, line, mine, frame);
  for (int32_t j = 1; j < LoadS16(base, a + 2); j++) {
    r3 = RingAt(base, path, j);
    BlockLeft(base, uint32_t(r3), frame + 88, frame + 80, frame + 96);
  }
  return r3;
}

uint64_t AvoidOnRight(uint8_t* base, uint32_t path, uint32_t a, uint32_t record,
                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CD50 AiPath::AvoidOnRight");
  uint64_t r3 = path;
  const double across = Fmuls(Lfs(base, a + 16), Lfs(base, record + 12));
  const uint32_t driver = Load(base, path + 0);
  const double mine = Lfs(base, driver + 144);
  CopyWord(base, driver + 144, frame + 96);
  if (mine > across) {
    guest::StoreU32(base, frame + 88, Load(base, path + 5520));
    AdjustForOther(base, driver, record, frame + 88);
    r3 = driver;
  }
  double line = Fsubs(across, Lfs(base, 0x82002A3C));
  const double zero = Lfs(base, kZero);
  if (mine > line && Lfs(base, record + 0) < zero) {
    line = Fmadds(IntToFloat(LoadS16(base, a + 2) + 1), Fsubs(line, mine), line);
  }
  const double steer = Lfs(base, driver + 152);
  line = Fsubs(line, Lfs(base, 0x8200296C));
  Stfs(base, frame + 88, line);
  if (steer < zero) {
    line = Fmadds(steer, Lfs(base, kFifth), line);
    Stfs(base, frame + 88, line);
  }
  if (!(mine > line)) {
    for (int32_t j = 1; j < LoadS16(base, a + 2); j++) {
      const uint32_t entry = RingAt(base, path, j);
      line = Fsubs(line, Lfs(base, entry + 28));
      if (Lfs(base, entry + 44) > line) {
        const double least = Lfs(base, entry + 40);
        Stfs(base, entry + 44, line);
        if (line < least) {
          Stfs(base, entry + 44, least);
        }
      }
    }
    return r3;
  }
  SpreadWidth(base, path, a, line, mine, frame);
  for (int32_t j = 1; j < LoadS16(base, a + 2); j++) {
    r3 = RingAt(base, path, j);
    BlockRight(base, uint32_t(r3), frame + 88, frame + 80, frame + 96);
  }
  return r3;
}

uint64_t ChooseSideForOther(uint8_t* base, uint32_t path, uint32_t a, uint32_t record,
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8213D788 AiPath::ChooseSideForOther");
  const uint32_t driver = Load(base, path + 0);
  const double a12 = Lfs(base, a + 12);
  const int32_t index = LoadS16(base, record + 26);
  const int32_t n = LoadS16(base, path + 5136);
  const double a8 = Lfs(base, a + 8);
  const double width = Fadds(a12, Lfs(base, kFifth));
  const double reach = Fadds(Lfs(base, driver + 144), a8);
  const double zero = Lfs(base, kZero);
  double left = zero, right = zero;
  if (index < n) {
    const int32_t m = int32_t(uint32_t(LoadS16(base, path + 5138)) + uint32_t(index));
    const uint32_t entry = path + (uint32_t(m - (m / 80) * 80) << 6);
    left = Lfs(base, entry + 56);
    right = Lfs(base, entry + 60);
  }
  const int32_t kind = LoadS16(base, record + 24);
  const double other = Lfs(base, a + 4);
  const double gap = Fsubs(reach, other);
  bool to_right;
  if (kind == 1) {
    const double room = Fadds(width, Lfs(base, kOne));
    if (!(Fadds(other, room) > right)) {
      to_right = gap < zero && !(Fsubs(other, room) < left);
    } else if (!(Fsubs(other, room) < left)) {
      to_right = true;
    } else if (!(Fadds(other, width) > right)) {
      to_right = gap < zero && !(Fsubs(other, width) < left);
    } else if (!(Fsubs(other, width) < left)) {
      to_right = true;
    } else {
      to_right = gap < zero;
    }
  } else if (uint32_t(kind - 2) > 1) {
    return path;
  } else {
    to_right = gap < zero;
  }
  if (to_right) {
    Stfs(base, a + 16, Fsubs(other, width));
    return AvoidOnRight(base, path, a, record, stack - 160);
  }
  Stfs(base, a + 16, Fadds(other, width));
  return AvoidOnLeft(base, path, a, record, stack - 176);
}

uint64_t AvoidByLane(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D898 AiPath::AvoidByLane");
  const uint32_t driver = Load(base, path + 0);
  const double other = Lfs(base, a + 4);
  const double one = Lfs(base, kOne);
  const double room = Fadds(Lfs(base, a + 12), one);
  const double off = Fsubs(Lfs(base, driver + 144), other);
  if (!(off < room) || !(off > Fneg(room))) {
    return path;
  }
  if (LoadS16(base, path + 5136) <= 1) {
    return path;
  }
  const uint32_t entry = RingAt(base, path, 1);
  if (int32_t(entry) == 0) {
    return path;
  }
  const uint32_t slice = Load(base, entry + 52);
  if (slice == 0) {
    return path;
  }
  const uint32_t lanes = guest::LoadU8(base, slice + 56) & 0xF;
  int32_t lane = 0;
  if (lanes > 1) {
    const int32_t now = int32_t(Load(base, entry + 20));
    if (now <= 0) {
      lane = 1;
    } else if (off < Lfs(base, kZero)) {
      lane = now - 1;
    } else {
      lane = now + 1;
      if (!(uint32_t(lane) < lanes)) {
        lane = now - 1;
      }
    }
  }
  const double width = Lfs(base, slice + 12);
  const double half = Lfs(base, kHalf);
  const double count = IntToFloat(int32_t(lanes));
  const double centered = Fnmsubs(Fsubs(count, one), half, IntToFloat(lane));
  Stfs(base, a + 16, Fmuls(Fdivs(centered, count), width));
  if (lane > int32_t(Load(base, entry + 20))) {
    return AvoidOnLeft(base, path, a, record, frame - 176);
  }
  return AvoidOnRight(base, path, a, record, frame - 160);
}

uint64_t AvoidOthers(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E690 AiPath::AvoidOthers");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t a = frame + 80 + 20 * i;
    Stfs(base, a + 4, zero);
    guest::StoreU16(base, a + 0, 0);
    Stfs(base, a + 8, zero);
    guest::StoreU16(base, a + 2, 0);
    Stfs(base, a + 12, zero);
    Stfs(base, a + 16, zero);
  }
  uint64_t r3 = MeasureOthers(base, path, frame + 80);
  for (int32_t k = 0; k < count; k++) {
    const uint32_t driver = Load(base, path + 0);
    const int32_t index = int32_t(Load(base, driver + 452 + 4 * uint32_t(k)));
    if (index < 0) {
      return r3;
    }
    const uint32_t record = uint32_t(index) * 40 + driver + 172;
    const uint32_t a = frame + 80 + 20 * uint32_t(k);
    const int32_t flags = LoadS16(base, record + 28);
    if ((flags & 1) != 0) {
      r3 = ChooseSideForOther(base, path, a, record, frame);
    } else if ((flags & 6) != 0) {
      if (k == 0) {
        CopyWord(base, a + 4, a + 16);
        AvoidOnLeft(base, path, a, record, frame - 176);
        r3 = AvoidOnRight(base, path, a, record, frame - 160);
      }
    } else if ((flags & 0x10) != 0 && k == 0) {
      r3 = AvoidByLane(base, path, a, record, frame - 96);
    }
  }
  return r3;
}

// --- The skill's timed triggers ---

namespace {
// The race's time in seconds: RaceTime::FromTicks on the race state's +6116,
// minutes x 60 + seconds.
int32_t RaceSeconds(uint8_t* base) {
  const uint32_t time = TimeFromTicks(Load(base, Load(base, kRaceState) + 6116));
  return int32_t(((time >> 16) & 0x7FFF) * 60 + ((time >> 10) & 0x3F));
}
// The race state's record (+2184 + 972 x i + 4) whose word matches the
// driver's +500 object's halfword +0; 0 with none.
uint32_t SkillEntry(uint8_t* base, uint32_t driver) {
  const uint32_t object = Load(base, driver + 500);
  const uint32_t list = Load(base, kRaceState) + 2184;
  if (object == 0) {
    return 0;
  }
  const int32_t n = int32_t(Load(base, list));
  if (n <= 0) {
    return 0;
  }
  const int32_t key = LoadS16(base, object + 0);
  for (int32_t i = 0; i < n; i++) {
    if (int32_t(Load(base, list + 8 + 972 * uint32_t(i))) == key) {
      return list + 972 * uint32_t(i) + 4;
    }
  }
  return 0;
}
}  // namespace

uint32_t SkillEntryWord(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213F7E0 AiSkill::EntryWord");
  const uint32_t entry = SkillEntry(base, driver);
  if (entry == 0) {
    return 0;
  }
  return uint32_t(strings_and_font::steps::Fctiwz(Lfs(base, entry + 824)));
}

double SkillEntryValue(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213F868 AiSkill::EntryValue");
  const uint32_t entry = SkillEntry(base, driver);
  if (entry == 0) {
    return Lfs(base, kZero);
  }
  return Fmuls(IntToFloat(int32_t(Load(base, entry + 784))), Lfs(base, 0x8200805C));
}

uint32_t TriggerStarts(uint8_t* base, uint32_t driver, uint32_t trigger, double* f1,
                       bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213F958 AiSkill::TriggerStarts");
  const uint32_t kind = Load(base, trigger + 0);
  const auto value = [&] { return LoadS16(base, trigger + 4); };
  switch (kind) {
    case 0:
      return Lfs(base, driver + 488) < IntToFloat(value()) ? 0 : 1;
    case 1:
      return Lfs(base, driver + 488) > IntToFloat(int32_t(0u - uint32_t(value()))) ? 0 : 1;
    case 2:
      return RaceSeconds(base) < value() ? 0 : 1;
    case 3:
      return RaceSeconds(base) > value() ? 0 : 1;
    case 4:
      return int32_t(SkillEntryWord(base, driver)) < value() ? 0 : 1;
    case 5:
      return int32_t(SkillEntryWord(base, driver)) > value() ? 0 : 1;
    case 6:
    case 7: {
      const double got = SkillEntryValue(base, driver);
      *f1 = got;
      *f1_set = true;
      const double against = IntToFloat(value());
      return (kind == 6 ? got < against : got > against) ? 0 : 1;
    }
    case 8:
      return Load(base, driver + 496) != 0 ? 0 : 1;
    case 9:
      return Load(base, driver + 496) == 0 ? 0 : 1;
    case 10: {
      const uint32_t vehicle = Load(base, driver + 160);
      if (vehicle == 0) {
        return 0;
      }
      return (Load(base, vehicle + 6152) & 1) == 0 ? 0 : 1;
    }
    case 11:
      return LoadS16(base, driver + 484) > 0 ? 1 : 0;
    case 12:
      return LoadS16(base, driver + 486) > 0 ? 1 : 0;
    case 13:
      return (Load(base, Load(base, kInputs)) & 0x100) == 0 ? 0 : 1;
    case 14:
      return (Load(base, Load(base, kInputs)) & 0x20000) == 0 ? 0 : 1;
    default:
      return 0;
  }
}

uint32_t TriggerEnds(uint8_t* base, uint32_t state, uint32_t driver, uint32_t trigger,
                     double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213FB48 AiSkill::TriggerEnds");
  const uint32_t kind = Load(base, trigger + 0);
  const auto value = [&] { return LoadS16(base, trigger + 4); };
  switch (kind) {
    case 0:
      return Lfs(base, driver + 488) < IntToFloat(value()) ? 1 : 0;
    case 1:
      return Lfs(base, driver + 488) > IntToFloat(int32_t(0u - uint32_t(value()))) ? 1 : 0;
    case 2:
      return RaceSeconds(base) < value() ? 1 : 0;
    case 3:
      return RaceSeconds(base) > value() ? 1 : 0;
    case 4:
      return int32_t(SkillEntryWord(base, driver)) < value() ? 1 : 0;
    case 5:
      return int32_t(SkillEntryWord(base, driver)) > value() ? 1 : 0;
    case 6:
    case 7: {
      const double got = SkillEntryValue(base, driver);
      *f1 = got;
      *f1_set = true;
      const double against = IntToFloat(value());
      return (kind == 6 ? got < against : got > against) ? 1 : 0;
    }
    case 8:
      return Load(base, driver + 496) != 0 ? 1 : 0;
    case 9:
      return Load(base, driver + 496) == 0 ? 1 : 0;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14: {
      const uint16_t left = uint16_t(guest::LoadU16(base, state + 0) - 1);
      guest::StoreU16(base, state + 0, left);
      return left != 0 ? 0 : 1;
    }
    default:
      return 1;
  }
}

uint32_t UpdateSkill(uint8_t* base, uint32_t skill, uint32_t driver, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213FCF8 AiSkill::Update");
  if (Load(base, skill + 0) == 0 || int32_t(Load(base, skill + 36)) != 1) {
    return 0;
  }
  uint32_t changed = 0;
  uint32_t state = skill + 24;
  for (uint32_t at = 16; at < 40; at += 8, state += 4) {
    const uint32_t trigger = at + Load(base, skill + 0);
    if (int32_t(Load(base, trigger + 0)) == -1) {
      continue;
    }
    int32_t step;
    if (guest::LoadU8(base, state + 2) == 0) {
      step = TriggerStarts(base, driver, trigger, f1, f1_set) != 0 ? 1 : 0;
    } else {
      step = TriggerEnds(base, state, driver, trigger, f1, f1_set) != 0 ? -1 : 0;
    }
    if (step > 0) {
      guest::StoreU16(base, state + 0, 150);
      guest::StoreU8(base, state + 2, 1);
      const double now = Lfs(base, skill + 20);
      Stfs(base, skill + 20, Fadds(IntToFloat(LoadS16(base, trigger + 4)), now));
    } else if (step < 0) {
      guest::StoreU8(base, state + 2, 0);
      const double now = Lfs(base, skill + 20);
      Stfs(base, skill + 20, Fsubs(now, IntToFloat(LoadS16(base, trigger + 4))));
    } else {
      continue;
    }
    changed = 1;
  }
  return changed;
}

// --- Steering through the corners and driving ---

void LimitSlide(uint8_t* base, uint32_t driver, uint32_t record, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_821392D8 AiDriver::LimitSlide");
  const double speed = Lfs(base, driver + 100);
  if (speed < Lfs(base, kHundred)) {
    if (speed < Lfs(base, kTwenty)) {
      return;
    }
    if (speed < Fmuls(Lfs(base, driver + 6088), Lfs(base, 0x82001070))) {
      return;
    }
  }
  const double back = Fneg(Lfs(base, record + 16));
  const double steer = Lfs(base, driver + 6136);
  const double zero = Lfs(base, kZero);
  const double side = Lfs(base, steer < zero ? driver + 112 : driver + 108);
  const double ratio = Fmuls(Fdivs(steer, side), back);
  const double fifth = Lfs(base, kFifth);
  if (!(ratio > fifth)) {
    return;
  }
  const double yaw = Fmuls(Lfs(base, driver + 104), back);
  if (yaw < Lfs(base, 0x82002A10)) {
    Stfs(base, driver + 6140, zero);
    Stfs(base, driver + 6136, Fmuls(steer, Lfs(base, 0x820021E8)));
  }
  if (int32_t(Load(base, flag)) == 0) {
    return;
  }
  if (!(ratio > Lfs(base, 0x820029D4)) || yaw < fifth) {
    return;
  }
  double most = Lfs(base, kOne);
  const double grip = Lfs(base, driver + 132);
  if (grip < most) {
    most = grip;
  }
  if (!(yaw < most)) {
    return;
  }
  Stfs(base, driver + 6140, zero);
  guest::StoreU8(base, driver + 6132, 1);
}

namespace {
// The room on the corner's side at the first ring entry (its +40 or +44
// against the driver's +144); `valid` false without that entry or its slice.
double CornerRoom(uint8_t* base, uint32_t driver, uint32_t record, bool* valid) {
  const uint32_t path = driver + 512;
  const uint32_t entry = LoadS16(base, path + 5136) > 1 ? RingAt(base, path, 1) : 0;
  *valid = entry != 0 && Load(base, entry + 52) != 0;
  if (!*valid) {
    return 0;
  }
  if (Lfs(base, record + 16) < Lfs(base, kZero)) {
    return Fsubs(Lfs(base, driver + 144), Lfs(base, entry + 40));
  }
  return Fsubs(Lfs(base, entry + 44), Lfs(base, driver + 144));
}
}  // namespace

uint32_t SteerByRoom(uint8_t* base, uint32_t driver, uint32_t record, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82138FA8 AiDriver::SteerByRoom");
  uint32_t result = 0;
  if (LoadS16(base, driver + 5648) > 1) {
    bool valid = false;
    const double room = CornerRoom(base, driver, record, &valid);
    if (valid) {
      Stfs(base, out, room);
      if (room > Lfs(base, driver + 116)) {
        const double angle = Lfs(base, driver + 6116);
        if (!(Fmuls(Lfs(base, record + 16), angle) > Lfs(base, kZero))) {
          result = 1;
        } else {
          const double now = Lfs(base, driver + 6136);
          const double half = Fmuls(angle, Lfs(base, kHalf));
          const double sum = Fadds(Fadds(Lfs(base, driver + 6112), now), half);
          Stfs(base, driver + 6136, sum);
          const double size = Lfs(base, driver + 128);
          const double left = Lfs(base, out);
          if (size > left) {
            Stfs(base, driver + 6136,
                 Fmadds(Fsubs(Lfs(base, kOne), Fdivs(left, size)), half, sum));
          }
          return 1;
        }
      }
    }
  }
  AccumulateSteer(base, driver);
  return result;
}

uint64_t SteerOutOfCorner(uint8_t* base, uint32_t driver, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139A08 AiDriver::SteerOutOfCorner");
  const double back = Fneg(Lfs(base, record + 16));
  const double zero = Lfs(base, kZero);
  const double speed = Lfs(base, driver + 100);
  Stfs(base, frame + 80, zero);
  if (!(speed > Lfs(base, kTwenty))) {
    AccumulateSteer(base, driver);
    return driver;
  }
  const uint32_t result = SteerByRoom(base, driver, record, frame + 80);
  if (result == 0) {
    return 0;
  }
  const double turn = Fmuls(Lfs(base, driver + 6116), back);
  const double target = Lfs(base, driver + 6088);
  const double now = Lfs(base, driver + 100);
  bool cut;
  if (turn < zero) {
    cut = now > target || (Fmuls(Lfs(base, driver + 144), back) < zero &&
                           Fmuls(Lfs(base, driver + 104), back) < zero);
  } else {
    cut = now > target && !(Fmuls(Lfs(base, driver + 6112), back) < zero);
  }
  if (cut) {
    Stfs(base, driver + 6140, zero);
  }
  return result;
}

uint64_t SteerAtCornerPoint(uint8_t* base, uint32_t driver, uint32_t record, uint32_t at,
                            uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213A200 AiDriver::SteerAtCornerPoint");
  int32_t k = int32_t(Load(base, at));
  const double zero = Lfs(base, kZero);
  const int32_t look = LoadS16(base, driver + 5654);
  if (k < look) {
    k = look;
  }
  if (!(k < LoadS16(base, driver + 5648))) {
    AccumulateSteer(base, driver);
    return driver;
  }
  bool valid = false;
  double room = CornerRoom(base, driver, record, &valid);
  if (!valid) {
    room = zero;
  }
  if (room < Lfs(base, driver + 116)) {
    guest::StoreU32(base, record + 40, 3);
    return SteerOutOfCorner(base, driver, record, frame - 96);
  }
  const uint32_t path = driver + 512;
  const uint32_t target = RingEntry(base, path, k);
  if (target == 0) {
    AccumulateSteer(base, driver);
    return driver;
  }
  const double dx = Fsubs(Lfs(base, target + 0), Lfs(base, driver + 80));
  const double dz = Fsubs(Lfs(base, target + 8), Lfs(base, driver + 88));
  *f1 = dx;  // the original works it out in f1
  *f1_set = true;
  if (dx == zero && dz == zero) {
    AccumulateSteer(base, driver);
    return driver;
  }
  const double toward = c_runtime::Atan2(dx, dz);
  *f1 = toward;
  *f1_set = true;
  Stfs(base, frame + 80, Fneg(Fsubs(double(float(toward)), Lfs(base, driver + 96))));
  WrapAngle(base, frame + 80);
  const double turn = Lfs(base, driver + 6116);
  const double angle = Lfs(base, frame + 80);
  if (Fmuls(turn, angle) != zero) {
    Stfs(base, driver + 6136, Fadds(Lfs(base, driver + 6136), turn));
  }
  const double sum = Fadds(Lfs(base, driver + 6112), Lfs(base, driver + 6136));
  Stfs(base, driver + 6136, sum);
  if (!(room < zero)) {
    double add = Fmuls(Fdivs(room, Lfs(base, driver + 128)), angle);
    Stfs(base, frame + 80, add);
    const double least = Lfs(base, 0x82008960);
    if (add < least) {
      add = least;
      Stfs(base, frame + 80, add);
    } else {
      const double most = Lfs(base, 0x82001FC4);
      if (add > most) {
        add = most;
        Stfs(base, frame + 80, add);
      }
    }
    Stfs(base, driver + 6136, Fadds(sum, add));
  }
  guest::StoreU32(base, frame + 84, 0);
  LimitSlide(base, driver, record, frame + 84);
  return driver;
}

uint64_t SteerIntoCorner(uint8_t* base, uint32_t driver, uint32_t record, uint32_t frame,
                         double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AB40 AiDriver::SteerIntoCorner");
  const uint32_t data = Load(base, record + 0);
  const int32_t offset = LoadS16(base, record + 10);
  if (int32_t(uint32_t(LoadS16(base, data + 52)) + uint32_t(offset)) < 0) {
    guest::StoreU32(base, record + 40, 3);
    return SteerOutOfCorner(base, driver, record, frame - 96);
  }
  guest::StoreU32(base, frame + 80, uint32_t(LoadS16(base, data + 50)) + uint32_t(offset));
  return SteerAtCornerPoint(base, driver, record, frame + 80, frame - 128, f1, f1_set);
}

uint32_t UpdateSteering(uint8_t* base, uint32_t driver, uint32_t frame, double* f1,
                        bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AEF0 AiDriver::UpdateSteering");
  UpdateSteerInput(base, driver, frame, f1, f1_set);
  double scale = Lfs(base, 0x8200296C);
  double limit = Lfs(base, 0x82002A10);
  const uint32_t slow = IsSlow(base, driver + 512);
  const double fifth = Lfs(base, kFifth);
  if (slow == 0) {
    const double yaw = Lfs(base, driver + 104);
    if (yaw > Lfs(base, 0x82002FB0) || yaw < Lfs(base, 0x82002C00)) {
      Stfs(base, driver + 6136, Fnmsubs(yaw, fifth, Lfs(base, driver + 6136)));
    }
  } else {
    scale = fifth;
    limit = Lfs(base, 0x82002A14);
  }
  const double yaw = Lfs(base, driver + 104);
  if (yaw > limit) {
    Stfs(base, driver + 6136, Fnmsubs(Fsubs(yaw, limit), scale, Lfs(base, driver + 6136)));
  } else if (yaw < Fneg(limit)) {
    Stfs(base, driver + 6136, Fnmsubs(Fadds(yaw, limit), scale, Lfs(base, driver + 6136)));
  }
  double steer = Lfs(base, driver + 6136);
  const double zero = Lfs(base, kZero);
  steer = Fdivs(steer, Lfs(base, steer < zero ? driver + 112 : driver + 108));
  const double now = Lfs(base, driver + 6120);
  Stfs(base, driver + 6136, steer);
  double move = Fsubs(steer, now);
  if (Fadds(move, fifth) < zero) {
    move = Lfs(base, 0x82006908);
  } else if (Fsubs(move, fifth) > zero) {
    move = fifth;
  }
  const double next = Fadds(now, move);
  Stfs(base, driver + 6120, next);
  const double one = Lfs(base, kOne);
  if (Fadds(next, one) < zero) {
    Stfs(base, driver + 6120, Lfs(base, kMinusOne));
  } else if (Fsubs(next, one) > zero) {
    Stfs(base, driver + 6120, one);
  }
  return slow;
}

uint64_t Drive(uint8_t* base, uint32_t driver, uint32_t input, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_8213B058 AiDriver::Drive");
  const uint32_t vehicle = Load(base, driver + 160);
  StoreConstant(base, driver + 6140, 0x820027A8);
  guest::StoreU8(base, driver + 6132, 0);
  const double zero = Lfs(base, kZero);
  Stfs(base, driver + 6144, zero);
  Stfs(base, driver + 6136, zero);
  if (SteerToTarget(base, driver, f1) == 0) {
    Stfs(base, driver + 6120, zero);
    guest::StoreU8(base, driver + 6132, 0);
    Stfs(base, driver + 6128, zero);
    Stfs(base, driver + 6124, zero);
  } else if (((Load(base, vehicle + 6148) >> 26) & 1) != 0) {
    Stfs(base, driver + 6120, zero);
    Stfs(base, driver + 6124, zero);
    Stfs(base, driver + 6128, Lfs(base, kOne));
    guest::StoreU16(base, input + 10, 32767);
  } else {
    double f = 0;
    bool set = false;
    UpdateSpeed(base, driver, frame - 96, &f, &set);
    if (LoadS16(base, driver + 6060) <= 0) {
      const double most = Lfs(base, driver + 164);
      if (Lfs(base, driver + 6088) > most) {
        Stfs(base, driver + 6088, most);
      }
    }
    const double least = Lfs(base, kTwenty);
    if (Lfs(base, driver + 6088) < least) {
      Stfs(base, driver + 6088, least);
    }
    UpdateSteering(base, driver, frame - 112, &f, &set);
    if (set) {
      *f1 = f;
    }
    UpdatePedals(base, driver, frame - 112);
    if ((Load(base, driver + 16) & 0x20) != 0) {
      guest::StoreU32(base, input + 0, Load(base, input + 0) | 0x200);
      Stfs(base, driver + 6120, Fmuls(Lfs(base, driver + 6120), Lfs(base, 0x82001BE8)));
      Stfs(base, driver + 6124, zero);
    }
  }
  if (guest::LoadU8(base, driver + 6132) != 0) {
    guest::StoreU16(base, input + 10, 32767);
  }
  LimitThrottle(base, driver);
  const double brake = Lfs(base, driver + 6128);
  if (brake > zero) {
    Stfs(base, driver + 6124, zero);
  }
  const double scale = Lfs(base, 0x82007F58);
  const double steer = Fmuls(Lfs(base, driver + 6120), scale);
  const double throttle = Fmuls(Lfs(base, driver + 6124), scale);
  const double pressed = Fmuls(brake, scale);
  guest::StoreU16(base, input + 4, uint16_t(strings_and_font::steps::Fctiwz(throttle)));
  guest::StoreU16(base, input + 8, uint16_t(strings_and_font::steps::Fctiwz(steer)));
  guest::StoreU16(base, input + 6, uint16_t(strings_and_font::steps::Fctiwz(pressed)));
  return driver;
}

// --- The path's line through the corners ---

namespace {
constexpr uint32_t kFourFifths = 0x82001070;  // 0.8
// The entry's offset +24 (just set to `offset`) kept within +40 to +44.
void KeepInRange(uint8_t* base, uint32_t entry, double offset) {
  const double low = Lfs(base, entry + 40);
  if (offset < low) {
    Stfs(base, entry + 24, low);
    return;
  }
  const double high = Lfs(base, entry + 44);
  if (offset > high) {
    Stfs(base, entry + 24, high);
  }
}
// The entry's offset +24 moved from `from` by at most the path's +5516.
void LimitStep(uint8_t* base, uint32_t path, uint32_t entry, double from, double zero) {
  double step = Fsubs(Lfs(base, entry + 24), from);
  const double most = Lfs(base, path + 5516);
  if (Fadds(most, step) < zero) {
    step = Fneg(most);
  } else if (Fsubs(step, most) > zero) {
    step = most;
  }
  Stfs(base, entry + 24, Fadds(step, from));
}
// The corner steps' shared start: the end index (`end`, the corner's data's
// halfword at `point` plus the record's offset +10) and the start (the word
// at `index`); 0 to stop (`*stop` = the return value) when the end isn't
// past it. Else the start index at frame + 80, `*span` = end - start, and
// the end clamped to the look-ahead +5140 (`*reached` = 1 then).
bool CornerSpan(uint8_t* base, uint32_t path, uint32_t record, uint32_t point, uint32_t index,
                uint32_t frame, int32_t* start, int32_t* end, int32_t* span,
                uint32_t* reached) {
  *start = int32_t(Load(base, index));
  *end = int32_t(uint32_t(LoadS16(base, Load(base, record + 0) + point)) +
                 uint32_t(LoadS16(base, record + 10)));
  *reached = 0;
  if (!(*end > *start)) {
    return false;
  }
  guest::StoreU32(base, frame + 80, uint32_t(*start));
  *span = int32_t(uint32_t(*end) - uint32_t(*start));
  const int32_t look = LoadS16(base, path + 5140);
  if (!(*end < look)) {
    *end = look;
    *reached = 1;
  }
  return true;
}
// A corner step's last part for the entry: kept in range, the step from
// `from` limited, ClampOffset, the previous entry and the index moved on.
// True while the index is below `end`.
bool FinishCornerEntry(uint8_t* base, uint32_t path, uint32_t entry, double from, double zero,
                       uint32_t index, uint32_t previous, int32_t end, uint32_t frame) {
  KeepInRange(base, entry, Lfs(base, entry + 24));
  LimitStep(base, path, entry, from, zero);
  ClampOffset(base, path, entry, index, frame + 80, frame - 96);
  guest::StoreU32(base, previous, entry);
  guest::StoreU32(base, index, Load(base, index) + 1);
  return int32_t(Load(base, index)) < end;
}
}  // namespace

void ContinueLine(uint8_t* base, uint32_t path, uint32_t entry, uint32_t index, uint32_t first,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213DB08 AiPath::ContinueLine");
  if (!(int32_t(Load(base, index)) < LoadS16(base, path + 5140))) {
    return;
  }
  const double zero = Lfs(base, kZero);
  uint32_t previous = entry;
  do {
    const uint32_t at = RingEntry(base, path, int32_t(Load(base, index)));
    const double offset = Fadds(Lfs(base, at + 28), Lfs(base, previous + 24));
    Stfs(base, at + 24, offset);
    const double low = Lfs(base, at + 40);
    bool clamp = offset < low;
    double bound = low;
    if (!clamp) {
      bound = Lfs(base, at + 44);
      clamp = offset > bound;
    }
    if (clamp) {
      Stfs(base, at + 24, bound);
      LimitStep(base, path, at, offset, zero);
    }
    ClampOffset(base, path, at, index, first, frame - 96);
    guest::StoreU32(base, index, Load(base, index) + 1);
    previous = at;
  } while (int32_t(Load(base, index)) < LoadS16(base, path + 5140));
}

uint32_t ApproachCorner(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                        uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213DE30 AiPath::ApproachCorner");
  int32_t end = int32_t(uint32_t(LoadS16(base, record + 10)) + 1);
  const int32_t start = int32_t(Load(base, index));
  if (!(end > start)) {
    return 0;
  }
  const uint32_t at = RingEntry(base, path, end);
  guest::StoreU32(base, frame + 80, uint32_t(start));
  const double target =
      at != 0 ? Lfs(base, at + 48) : Lfs(base, Load(base, record + 4) + 4);
  const double gap = Fmsubs(target, Lfs(base, kFourFifths),
                            Lfs(base, Load(base, previous) + 24));
  const double most = Fmuls(Lfs(base, path + 5516), Lfs(base, kHalf));
  const double zero = Lfs(base, kZero);
  double step = Fdivs(gap, IntToFloat(int32_t(uint32_t(end) - uint32_t(start))));
  if (Fadds(most, step) < zero) {
    step = Fneg(most);
  } else if (Fsubs(step, most) > zero) {
    step = most;
  }
  uint32_t reached = 0;
  const int32_t look = LoadS16(base, path + 5140);
  if (!(end < look)) {
    end = look;
    reached = 1;
  }
  if (!(start < end)) {
    return reached;
  }
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const double from =
        Fadds(Lfs(base, Load(base, previous) + 24), Lfs(base, entry + 28));
    Stfs(base, entry + 24, Fadds(from, step));
    more = FinishCornerEntry(base, path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t CornerToEntry(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                       uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E018 AiPath::CornerToEntry");
  int32_t start, end, span;
  uint32_t reached;
  if (!CornerSpan(base, path, record, 50, index, frame, &start, &end, &span, &reached)) {
    return 0;
  }
  const uint32_t first = Load(base, previous);
  const double step = Fdivs(Fsubs(Lfs(base, first + 48), Lfs(base, first + 24)),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(base, kZero);
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const uint32_t before = Load(base, previous);
    const double target = Lfs(base, entry + 48);
    const double side = Lfs(base, record + 16);
    const double limit = Fmuls(side, target);
    const double offset = Lfs(base, before + 24);
    double from = Fsubs(Fadds(target, offset), Lfs(base, before + 48));
    if (Fmuls(side, from) < limit) {
      Stfs(base, entry + 24, Fadds(step, from));
    } else {
      from = Fadds(Lfs(base, entry + 28), offset);
      if (Fmuls(from, Lfs(base, record + 16)) < limit) {
        Stfs(base, entry + 24, target);
      } else {
        Stfs(base, entry + 24, from);
      }
    }
    more = FinishCornerEntry(base, path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t CornerToApex(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                      uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E1D0 AiPath::CornerToApex");
  int32_t start, end, span;
  uint32_t reached;
  if (!CornerSpan(base, path, record, 52, index, frame, &start, &end, &span, &reached)) {
    return 0;
  }
  const uint32_t first = Load(base, previous);
  const double step = Fdivs(Fsubs(Lfs(base, first + 48), Lfs(base, first + 24)),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(base, kZero);
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const uint32_t before = Load(base, previous);
    const double target = Lfs(base, entry + 48);
    const double from =
        Fsubs(Fadds(Lfs(base, before + 24), target), Lfs(base, before + 48));
    const double offset = Fadds(step, from);
    Stfs(base, entry + 24, offset);
    const double side = Lfs(base, record + 16);
    if (Fmuls(side, offset) < Fmuls(side, target)) {
      Stfs(base, entry + 24, target);
    }
    more = FinishCornerEntry(base, path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t CornerToExit(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                      uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E360 AiPath::CornerToExit");
  int32_t start, end, span;
  uint32_t reached;
  if (!CornerSpan(base, path, record, 54, index, frame, &start, &end, &span, &reached)) {
    return 0;
  }
  const uint32_t first = Load(base, previous);
  const double scale = Lfs(base, kFourFifths);
  const double step =
      Fdivs(Fmuls(Fsubs(Lfs(base, first + 48), Lfs(base, first + 24)), scale),
            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(base, kZero);
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const uint32_t before = Load(base, previous);
    const double target = Lfs(base, entry + 48);
    const double from =
        Fmadds(Fsubs(target, Lfs(base, before + 48)), scale, Lfs(base, before + 24));
    Stfs(base, entry + 24, from);
    const double side = Lfs(base, record + 16);
    if (Fmuls(side, from) < Fmuls(side, target)) {
      Stfs(base, entry + 24, Fadds(step, from));
    }
    more = FinishCornerEntry(base, path, entry, from, zero, index, previous, end, frame);
  } while (more);
  return reached;
}

uint32_t PlanCorners(uint8_t* base, uint32_t path, uint32_t entry, uint32_t index, uint32_t end,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E7B8 AiPath::PlanCorners");
  const uint32_t previous = frame + 172;
  guest::StoreU32(base, previous, entry);
  for (int32_t k = 0; k < LoadS16(base, path + 5162); k++) {
    const uint32_t record = path + 5164 + 44 * uint32_t(k);
    if (Load(base, record + 0) == 0 || Load(base, record + 4) == 0) {
      continue;
    }
    const int32_t offset = LoadS16(base, record + 10);
    if (!(offset < 1) && int32_t(Load(base, index)) > offset) {
      guest::StoreU32(base, index, uint32_t(offset));
      guest::StoreU32(base, previous,
                      RingEntry(base, path, int32_t(uint32_t(offset) - 1)));
    }
    const int32_t until = int32_t(Load(base, end));
    if (int32_t(Load(base, index)) < until) {
      guest::StoreU32(base, index, uint32_t(until));
      guest::StoreU32(base, previous, RingEntry(base, path, int32_t(uint32_t(until) - 1)));
    }
    if (ApproachCorner(base, path, record, index, previous, frame - 160) == 1 ||
        CornerToEntry(base, path, record, index, previous, frame - 160) == 1 ||
        CornerToApex(base, path, record, index, previous, frame - 160) == 1 ||
        CornerToExit(base, path, record, index, previous, frame - 160) == 1) {
      return 1;
    }
    guest::StoreU32(base, end,
                    uint32_t(LoadS16(base, Load(base, record + 0) + 54)) +
                        uint32_t(LoadS16(base, record + 10)));
  }
  guest::StoreU32(base, end, Load(base, index));
  ContinueLine(base, path, Load(base, previous), index, end, frame - 112);
  return path;
}

uint32_t PlanLine(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213F4B0 AiPath::PlanLine");
  const uint32_t entry = RingEntry(base, path, 0);
  guest::StoreU32(base, frame + 80, 1);
  guest::StoreU32(base, frame + 84, 1);
  if (LoadS16(base, path + 5162) > 0) {
    return PlanCorners(base, path, entry, frame + 84, frame + 80, frame - 144);
  }
  ContinueLine(base, path, entry, frame + 84, frame + 80, frame - 112);
  return path;
}

// --- The path's branch kinds and lines ---

namespace {
constexpr uint32_t kBranchMasks = 0x82008970;
constexpr uint32_t kLeaderGap = 0x82008D40;  // -50.0
// The race data's flags ([0x826C5B2C] + 20).
uint32_t RaceFlags(uint8_t* base) {
  return Load(base, Load(base, kRaceData) + 20);
}
// The flags the branch kinds need: 0x10, 0x4, 0x8 or 0x20.
bool BranchFlags(uint32_t flags) {
  return (flags & 0x3C) != 0;
}
// The kind for a route section's branch number (1 to 14, by the originals'
// jump tables) under the race flags; -1 for none.
int32_t KindForBranch(uint32_t flags, uint32_t number) {
  switch (number - 1) {
    case 0:
    case 4:
      return 5;
    case 1:
    case 5:
      return 4;
    case 2:
    case 7:
    case 12:
      return (flags & 0x8) != 0 ? 7 : -1;
    case 3:
    case 6:
    case 13:
      return (flags & 0x4) != 0 ? 6 : -1;
    case 10:
      return (flags & 0x10) != 0 ? 2 : -1;
    case 11:
      return (flags & 0x20) != 0 ? 3 : -1;
    default:
      return -1;
  }
}
// The branch number (byte +3) of the section of a vehicle identity's route
// position (+4); false when the section is negative.
bool BranchNumberAt(uint8_t* base, uint32_t identity, uint32_t* number) {
  const uint32_t section = guest::LoadU16(base, identity + 4);
  if (int16_t(section) < 0) {
    return false;
  }
  const uint32_t table = Load(base, Load(base, track_route::kRoute) + 12);
  *number = guest::LoadU8(base, Load(base, table + (section << 2)) + 3);
  return true;
}
}  // namespace

uint32_t BranchMasksFor(uint8_t* base, uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_8213D378 AiPath::BranchMasksFor");
  const uint32_t flags = RaceFlags(base);
  if (!BranchFlags(flags)) {
    return kBranchMasks;
  }
  switch (Load(base, kind)) {
    case 1:
    case 6:
    case 7:
      if ((flags & 0xC) != 0) {
        return kBranchMasks + 16;
      }
      [[fallthrough]];
    case 2:
    case 8:
      if ((flags & 0x10) != 0) {
        return kBranchMasks + 32;
      }
      [[fallthrough]];
    case 3:
    case 9:
      if ((flags & 0x20) != 0) {
        return kBranchMasks + 48;
      }
      return kBranchMasks;
    default:
      return kBranchMasks;
  }
}

uint32_t LeaderBranchKind(uint8_t* base, uint32_t path, uint32_t kind, uint32_t best) {
  ITC_NATIVE_RUNNING("sub_8213D1F0 AiPath::LeaderBranchKind");
  const uint32_t flags = RaceFlags(base);
  if (!BranchFlags(flags)) {
    return path;
  }
  const uint32_t driver = Load(base, path + 0);
  const uint32_t vehicle = Load(base, driver + 160);
  if (vehicle == 0) {
    return 0;
  }
  if (int32_t(Load(base, kind)) != -1 && !(Lfs(base, driver + 488) > Lfs(base, best))) {
    return vehicle;
  }
  const uint32_t identity = vehicle_accessors::VehicleIdentity(vehicle);
  uint32_t number = 0;
  if (!BranchNumberAt(base, identity, &number)) {
    return identity;
  }
  const int32_t found = KindForBranch(flags, number);
  if (found < 0) {
    return identity;
  }
  guest::StoreU32(base, kind, uint32_t(found));
  Stfs(base, best, Lfs(base, driver + 488));
  return identity;
}

uint64_t FindSideSection(uint8_t* base, uint32_t path, uint32_t out, uint32_t point,
                         uint32_t masks, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D550 AiPath::FindSideSection");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(Load(base, Load(base, path + 0) + 160));
  const uint32_t position = frame + 80;
  track_route::ClampPosition(base, position, Load(base, identity + 4));
  uint64_t r3 = position;
  const uint32_t section = guest::LoadU16(base, position + 0);
  if (int16_t(section) < 0) {
    return r3;
  }
  const uint16_t slice = guest::LoadU16(base, position + 2);
  if (int16_t(slice) < 0) {
    return r3;
  }
  const uint32_t route = track_route::kRoute;
  uint32_t taken = Load(base, position);
  guest::StoreU32(base, frame + 84, taken);
  uint32_t current =
      Load(base, Load(base, Load(base, route) + 12) + ((section & 0xFFFF) << 2));
  uint32_t link = current + 52;
  for (int i = 0; i < 4; i++, link += 4) {
    const uint16_t number = guest::LoadU16(base, link);
    if (int16_t(number) < 0) {
      continue;
    }
    const uint32_t candidate =
        Load(base, Load(base, Load(base, route) + 12) + (uint32_t(number) << 2));
    const int32_t prefer =
        CompareBranches(base, path, current, candidate, frame + 84, link, masks);
    r3 = uint64_t(int64_t(prefer));
    if (!(prefer > 0)) {
      continue;
    }
    guest::StoreU16(base, position + 0, number);
    guest::StoreU16(base, position + 2, slice);
    r3 = track_route::Normalize(base, route, position, masks);
    r3 = track_route::OnSlice(base, route, point, position, 0, frame - 128);
    if (r3 == 0) {
      continue;
    }
    track_route::ClampPosition(base, frame + 88, Load(base, position));
    r3 = frame + 88;
    taken = Load(base, frame + 88);
    current = candidate;
    guest::StoreU32(base, frame + 84, taken);
  }
  guest::StoreU32(base, out, taken);
  return r3;
}

void FollowTargets(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D9D8 AiPath::FollowTargets");
  uint32_t previous = RingEntry(base, path, 0);
  int32_t i = 1;
  guest::StoreU32(base, frame + 80, uint32_t(i));
  if (!(LoadS16(base, path + 5140) > 1)) {
    return;
  }
  const double zero = Lfs(base, kZero);
  do {
    const uint32_t entry = RingEntry(base, path, i);
    const double target = Lfs(base, previous + 48);
    const double before = Lfs(base, previous + 24);
    const double step = Lfs(base, entry + 28);
    Stfs(base, entry + 24, target);
    const double from = Fadds(step, before);
    KeepInRange(base, entry, target);
    LimitStep(base, path, entry, from, zero);
    guest::StoreU32(base, frame + 84, 1);
    ClampOffset(base, path, entry, frame + 80, frame + 84, frame - 96);
    i = int32_t(uint32_t(i) + 1);
    previous = entry;
    guest::StoreU32(base, frame + 80, uint32_t(i));
  } while (i < LoadS16(base, path + 5140));
}

uint32_t FollowLeaderBranch(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213EB48 AiPath::FollowLeaderBranch");
  const uint32_t driver = Load(base, path + 0);
  uint32_t r3 = path;
  int32_t kind = -1;
  if ((Load(base, driver + 16) & 0x4) != 0) {
    const uint32_t object = Load(base, driver + 500);
    const uint32_t flags = RaceFlags(base);
    if (BranchFlags(flags) && object != 0) {
      const int32_t mode = LoadS16(base, object + 88);
      if ((mode < 4 || mode > 5) && !(Lfs(base, driver + 488) > Lfs(base, kLeaderGap))) {
        const int32_t count = int32_t(track_collision::RaceRacerCount(base));
        uint32_t found = 0;
        for (int32_t i = 0; i < count; i++) {
          const uint32_t racer = Load(base, Load(base, kRace) + 2308 + 4 * uint32_t(i));
          const uint32_t index = Load(base, racer + 20);
          if (int32_t(index) < 0) {
            continue;
          }
          const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
          if (vehicle != 0 && (Load(base, vehicle + 6148) & 0x81) != 0) {
            found = vehicle;
            break;
          }
        }
        r3 = 0;
        if (found != 0) {
          const uint32_t identity = vehicle_accessors::VehicleIdentity(found);
          r3 = identity;
          uint32_t number = 0;
          if (BranchNumberAt(base, identity, &number)) {
            kind = KindForBranch(flags, number);
          }
        }
      }
    }
  }
  if (int32_t(Load(base, path + 5528)) != kind) {
    guest::StoreU32(base, path + 5528, uint32_t(kind));
    r3 = ResetPath(base, path);
  }
  return r3;
}

// --- The drivers' updates and the pairs ---

uint64_t CompareWith(uint8_t* base, uint64_t a, uint32_t other, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82145B08 AiDriver::CompareWith");
  const uint32_t driver = uint32_t(a);
  guest::StoreU32(base, frame + 84, Load(base, other));
  const uint64_t b64 = Of(base, frame + 84);
  const uint32_t b = uint32_t(b64);
  const uint32_t record = uint32_t(uint64_t(Load(base, other)) * 40 + a + 172);
  if (Load(base, b + 160) == 0) {
    ResetOtherRecord(base, record, Lfs(base, kOtherGap), Lfs(base, kZero), Lfs(base, kOne),
                     Lfs(base, kMinusOne));
    return b64;
  }
  const uint32_t mine = vehicle_accessors::VehicleIdentity(Load(base, driver + 160));
  const uint32_t theirs = vehicle_accessors::VehicleIdentity(Load(base, b + 160));
  const double distance =
      track_route::Distance(base, track_route::kRoute, theirs + 4, mine + 4);
  const double length = Fadds(Lfs(base, b + 124), Lfs(base, driver + 124));
  const double ahead = Fadds(Lfs(base, driver + 156), distance);
  Stfs(base, record + 16, Lfs(base, b + 100));
  Stfs(base, frame + 88, Fsubs(ahead, Lfs(base, b + 156)));
  Stfs(base, frame + 80, Fmuls(length, Lfs(base, kFourFifths)));
  const bool both = (Load(base, driver + 16) & 0x4) != 0 && (Load(base, b + 16) & 0x4) != 0;
  guest::StoreU8(base, record + 32, both ? 1 : 0);
  guest::StoreU8(base, record + 33, 0);
  UpdateOther(base, record, frame + 88, frame + 80);
  const uint64_t back64 = uint64_t(Load(base, driver + 0)) * 40 + b64 + 172;
  const uint32_t back = uint32_t(back64);
  MirrorOther(base, back, record, driver + 100);
  if (guest::LoadU16(base, driver + 8) == guest::LoadU16(base, b + 8)) {
    UpdateOtherTimer(base, record, other, frame + 80);
    UpdateOtherTimer(base, back, driver, frame + 80);
  }
  return back64;
}

uint64_t UpdateAll(uint8_t* base, uint32_t frame, DriverUpdate update, void* user) {
  ITC_NATIVE_RUNNING("sub_82139628 AiDrivers::UpdateAll");
  const uint32_t count = track_collision::RaceRacerCount(base);
  uint64_t r3 = count;
  if (Load(base, kDrivers) == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  const auto index_of = [&](uint32_t i) {
    return Load(base, Load(base, Load(base, kRace) + 2308 + 4 * i) + 20);
  };
  const auto driver_of = [&](uint32_t index) {
    return uint64_t(index) * kDriverBytes + Load(base, kDrivers);
  };
  for (uint32_t i = 0; i < count; i++) {
    const uint64_t driver = driver_of(index_of(i));
    r3 = update(user, driver, Load(base, uint32_t(driver) + 6148));
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t mine = index_of(i);
    for (uint32_t j = i + 1; int32_t(j) < int32_t(count); j++) {
      const uint32_t theirs = index_of(j);
      uint64_t driver;
      if (int32_t(mine) < int32_t(theirs)) {
        guest::StoreU32(base, frame + 80, theirs);
        driver = driver_of(mine);
      } else {
        guest::StoreU32(base, frame + 80, mine);
        driver = driver_of(theirs);
      }
      r3 = CompareWith(base, driver, frame + 80, frame - 144);
    }
  }
  return r3;
}

// --- Reacting to the other racers ---

namespace {
constexpr uint32_t kHundredth = 0x8200222C;   // the percentages' scale (0.01)
constexpr uint32_t kReactGap = 0x8200277C;    // states 1 and 3's gap limit
constexpr uint32_t kReactSpeed = 0x82000F14;  // state 1's speed limit
constexpr uint32_t kReactRate = 0x820029F8;   // state 1's rate limit
constexpr uint32_t kTick = 0x82617730;        // 1/30 s
constexpr uint32_t kPerKmh = 0x82002BD8;      // 1/3.6
// A skill percentage (a halfword) kept in 0 to 100.
int32_t Percentage(uint8_t* base, uint32_t at) {
  const int32_t value = LoadS16(base, at);
  if (value < 0) {
    return 0;
  }
  return value > 100 ? 100 : value;
}
void OrFlags(uint8_t* base, uint32_t record, uint16_t bits) {
  guest::StoreU16(base, record + 28, uint16_t(guest::LoadU16(base, record + 28) | bits));
}
uint16_t Truncated(double value) {
  return uint16_t(strings_and_font::steps::Fctiwz(value));
}
// PlanReaction's +12 from a percentage, with the flag 0x2.
void ReactionScale(uint8_t* base, uint32_t record, int32_t percentage) {
  const double scale = IntToFloat(percentage);
  OrFlags(base, record, 0x2);
  Stfs(base, record + 12, Fmuls(scale, Lfs(base, kHundredth)));
}
}  // namespace

void PlanReaction(uint8_t* base, uint32_t record, uint32_t skill, uint32_t rate, uint32_t flag,
                  uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821456F8 AiOther::PlanReaction");
  const uint32_t state = uint32_t(LoadS16(base, record + 24));
  Stfs(base, record + 12, Lfs(base, kOne));
  if (state < 1) {
    guest::StoreU16(base, record + 26, 0xFFFF);
    guest::StoreU16(base, record + 28, 0);
    return;
  }
  if (state == 1) {
    const double gap = Lfs(base, kReactGap);
    if (int32_t(flag) == 0 && Lfs(base, record + 16) > Lfs(base, kReactSpeed) &&
        !(Lfs(base, record + 4) > Lfs(base, kTenth)) && Lfs(base, record + 0) < gap) {
      guest::StoreU16(base, record + 26, 0xFFFF);
      return;
    }
    if (Lfs(base, record + 0) > gap && !(Lfs(base, record + 4) < Lfs(base, kReactRate))) {
      guest::StoreU16(base, record + 26, 1);
      OrFlags(base, record, 0x1);
      return;
    }
    const double meet = Lfs(base, record + 20);
    if (meet < Lfs(base, kZero)) {
      guest::StoreU16(base, record + 26, 0xFFFF);
      return;
    }
    OrFlags(base, record, 0x1);
    const int32_t percentage = Percentage(base, skill + 16);
    const double by = Lfs(base, rate);
    double ahead = Fadds(IntToFloat(percentage), Lfs(base, kHundred));
    ahead = Fmuls(ahead, by);
    ahead = Fmuls(ahead, meet);
    guest::StoreU16(base, record + 26, Truncated(Fmuls(ahead, Lfs(base, kHundredth))));
    return;
  }
  if (state < 3) {
    guest::StoreU16(base, record + 26, 1);
    if (Percentage(base, skill + 14) > 0) {
      return;
    }
    OrFlags(base, record, 0x1);
    return;
  }
  if (state != 3) {
    return;
  }
  if (int32_t(mode) == 1 && int32_t(flag) == 0 && LoadS16(base, record + 30) > 0) {
    guest::StoreU16(base, record + 26, 2);
    OrFlags(base, record, 0x10);
    return;
  }
  if (Lfs(base, record + 0) > Lfs(base, kTen)) {
    const double meet = Lfs(base, record + 20);
    if (meet < Lfs(base, kZero)) {
      guest::StoreU16(base, record + 26, 0xFFFF);
      return;
    }
    guest::StoreU16(base, record + 26, Truncated(Fmuls(Lfs(base, rate), meet)));
    if (guest::LoadU8(base, record + 33) != 1) {
      return;
    }
    const int32_t percentage = Percentage(base, skill + 14);
    if (!(percentage > 0)) {
      return;
    }
    ReactionScale(base, record, percentage);
    return;
  }
  if (int32_t(flag) == 0) {
    guest::StoreU16(base, record + 26, 1);
    OrFlags(base, record, 0x1);
    return;
  }
  guest::StoreU16(base, record + 26, 0xFFFF);
  if (guest::LoadU8(base, record + 33) != 1) {
    return;
  }
  const int32_t percentage = Percentage(base, skill + 14);
  if (!(percentage > 0)) {
    return;
  }
  guest::StoreU16(base, record + 26, 1);
  ReactionScale(base, record, percentage);
}

uint64_t ReactToOthers(uint8_t* base, uint32_t driver, uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82145CA8 AiDriver::ReactToOthers");
  guest::StoreU32(base, frame + 84, Load(base, driver + 0));
  const uint32_t file = Load(base, track_route::kRoute);
  const uint64_t skill = Of(base, frame + 84) + 6048;
  const uint32_t racers = track_collision::RaceRacerCount(base);
  uint64_t r3 = racers;
  const double speed = Lfs(base, driver + 100);
  double rate = Fmuls(Lfs(base, kTick), Lfs(base, kPerKmh));
  const double length = Lfs(base, file + 8);
  const double zero = Lfs(base, kZero);
  rate = Fdivs(rate, length);
  Stfs(base, frame + 80, Fmuls(rate, speed));
  uint32_t listed = 0;
  if (int32_t(racers) > 0) {
    const double ten = Lfs(base, kTen);
    const double far = Lfs(base, kReactGap);
    const double one = Lfs(base, kOne);
    uint32_t slot = driver + 452;
    for (uint32_t i = 0; i < racers; i++) {
      const uint32_t other = Load(base, Load(base, Load(base, kRace) + 2308 + 4 * i) + 20);
      guest::StoreU32(base, frame + 88, other);
      if (int32_t(Load(base, driver + 0)) == int32_t(other)) {
        continue;
      }
      const uint64_t record64 = uint64_t(other) * 40 + driver + 172;
      const uint32_t record = uint32_t(record64);
      guest::StoreU16(base, record + 28, 0);
      const uint32_t keeps = KeepsGapFrom(base, driver, frame + 88);
      r3 = keeps;
      bool react = int32_t(keeps) != 0;
      if (!react) {
        const uint32_t bit = (other & 0x20) != 0 ? 0 : 1u << (other & 0x3F);
        if ((uint32_t(LoadS16(base, driver + 40)) & bit) != 0) {
          Stfs(base, record + 12, one);
          const int32_t state = int32_t(Load(base, driver + 32));
          if (state == 2) {
            const double gap = Lfs(base, record + 0);
            if (gap < zero) {
              const double slices = Fdivs(gap, Lfs(base, file + 8));
              guest::StoreU16(base, record + 28, 4);
              guest::StoreU16(base, record + 26, Truncated(Fneg(slices)));
            }
          } else if (state == 3) {
            const double gap = Lfs(base, record + 0);
            guest::StoreU16(base, record + 28, 1);
            if (gap < far && gap > ten) {
              guest::StoreU16(base, record + 26,
                              Truncated(Fdivs(Fabs(gap), Lfs(base, file + 8))));
            } else {
              guest::StoreU16(base, record + 26, 1);
            }
          } else {
            react = true;
          }
        }
      }
      if (react) {
        const uint32_t vehicle = Load(base, driver + 160);
        const uint32_t flag7 = vehicle != 0 ? (Load(base, vehicle + 6148) >> 2) & 1 : 0;
        const uint32_t flag6 = (Load(base, driver + 16) >> 2) & 1;
        PlanReaction(base, record, uint32_t(skill), frame + 80, flag6, flag7);
        r3 = record64;
      }
      if (guest::LoadU16(base, record + 28) != 0) {
        const int32_t at = LoadS16(base, record + 26);
        if (!(at < 0) && at < int32_t(Load(base, count))) {
          guest::StoreU32(base, slot, other);
          listed++;
          slot += 4;
        }
      }
    }
  }
  guest::StoreU32(base, ((listed + 113) << 2) + driver, 0xFFFFFFFFu);
  if (!(int32_t(listed - 1) > 0)) {
    return r3;
  }
  uint32_t at = driver + 452;
  uint32_t k = 1;
  for (uint32_t outer = 0; outer < listed - 1; outer++, at += 4, k++) {
    if (!(int32_t(k) < int32_t(listed))) {
      continue;
    }
    uint32_t next = at + 4;
    for (uint32_t n = listed - k; n != 0; n--, next += 4) {
      const uint32_t a = Load(base, at);
      const uint32_t b = Load(base, next);
      const uint32_t ra = uint32_t(uint64_t(a) * 40 + driver);
      const uint32_t rb = uint32_t(uint64_t(b) * 40 + driver);
      const int32_t ia = LoadS16(base, ra + 198);
      const int32_t ib = LoadS16(base, rb + 198);
      bool swap = ia > ib;
      if (ia == ib) {
        const double gb = Lfs(base, rb + 172);
        swap = !(gb > zero) && Lfs(base, ra + 172) < gb;
      }
      if (swap) {
        guest::StoreU32(base, at, b);
        guest::StoreU32(base, next, a);
      }
    }
  }
  return k;
}

// --- The drivers' starts ---

namespace {
constexpr uint32_t kMinusHundred = 0x8200895C;  // -100.0: LeaderBranchKind's first best
// The race's racer i's record ([0x826D82F4] + 2308).
uint32_t RacerAt(uint8_t* base, uint32_t i) {
  return Load(base, Load(base, kRace) + 2308 + 4 * i);
}
}  // namespace

uint64_t StartGroup(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A000 AiDrivers::StartGroup");
  const uint32_t count = track_collision::RaceRacerCount(base);
  uint64_t r3 = count;
  StoreConstant(base, frame + 88, kMinusHundred);
  uint32_t mask = 0;
  guest::StoreU32(base, frame + 80, mask);
  guest::StoreU32(base, frame + 84, 0xFFFFFFFFu);
  if (Load(base, kDrivers) == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(base, i);
    const uint32_t flags = Load(base, racer + 0);
    if ((flags & 0x10) == 0) {
      continue;
    }
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0) {
      continue;
    }
    if ((flags & 0xC) == 0) {
      mask |= Slw(1, index);
    }
    const uint64_t driver = uint64_t(index) * kDriverBytes + Load(base, kDrivers);
    r3 = LeaderBranchKind(base, uint32_t(driver + 512), frame + 84, frame + 88);
  }
  guest::StoreU32(base, frame + 80, mask);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(base, i);
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0) {
      continue;
    }
    const uint32_t flags = Load(base, racer + 0);
    const uint64_t driver = uint64_t(index) * kDriverBytes + Load(base, kDrivers);
    if ((flags & 0x10) == 0) {
      continue;
    }
    if ((flags & 0xC) == 0) {
      guest::StoreU32(base, frame + 92, 0);
      r3 = SetState2or3(base, uint32_t(driver), frame + 80, frame + 92, frame - 112);
    }
    const uint64_t path = driver + 512;
    r3 = path;
    const uint32_t best = Load(base, frame + 84);
    if (int32_t(Load(base, uint32_t(path) + 5524)) != int32_t(best)) {
      guest::StoreU32(base, uint32_t(path) + 5524, best);
      r3 = ResetPath(base, uint32_t(path));
    }
  }
  return r3;
}

uint64_t StartRanked(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A8B0 AiDrivers::StartRanked");
  const uint32_t count = track_collision::RaceRacerCount(base);
  uint64_t r3 = count;
  uint32_t table = Load(base, kDrivers);
  StoreConstant(base, frame + 92, kMinusHundred);
  uint32_t mask = 0;
  uint32_t listed = 0;
  guest::StoreU32(base, frame + 80, mask);
  guest::StoreU32(base, frame + 84, listed);
  guest::StoreU32(base, frame + 88, 0xFFFFFFFFu);
  if (table == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  const uint32_t list = frame + 96;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(base, i);
    if ((Load(base, racer + 0) & 0x10) == 0) {
      continue;
    }
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0) {
      continue;
    }
    const uint64_t driver = uint64_t(index) * kDriverBytes + table;
    mask |= Slw(1, index);
    r3 = LeaderBranchKind(base, uint32_t(driver + 512), frame + 88, frame + 92);
    guest::StoreU32(base, list + 4 * listed, index);
    listed++;
    table = Load(base, kDrivers);
  }
  guest::StoreU32(base, frame + 84, listed);
  guest::StoreU32(base, frame + 80, mask);
  // Sorted only with three or more listed, as the original (its count
  // checked against 2).
  if (int32_t(listed) > 2) {
    for (uint32_t i = 0; i < listed; i++) {
      for (uint32_t j = i + 1; j < listed; j++) {
        const uint32_t at = list + 4 * i;
        const uint32_t next = list + 4 * j;
        const uint32_t a = Load(base, at) * kDriverBytes + table;
        const uint32_t b = Load(base, next) * kDriverBytes + table;
        uint32_t flags = Load(base, Load(base, a + 160) + 6148) & 0x81;
        r3 = flags;
        if (flags != 0) {
          continue;
        }
        flags = Load(base, Load(base, b + 160) + 6148) & 0x81;
        r3 = flags;
        if (flags == 0 && Lfs(base, a + 488) > Lfs(base, b + 488)) {
          continue;
        }
        const uint32_t second = Load(base, next);
        const uint32_t first = Load(base, at);
        guest::StoreU32(base, at, second);
        guest::StoreU32(base, next, first);
      }
    }
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(base, i);
    const uint32_t index = Load(base, racer + 20);
    if (int32_t(index) < 0) {
      continue;
    }
    const uint32_t flags = Load(base, racer + 0);
    const uint64_t driver = uint64_t(index) * kDriverBytes + table;
    r3 = driver;
    if ((flags & 0x10) != 0) {
      SetStateFromList(base, uint32_t(driver), frame + 80, frame + 84, list, frame - 128);
      const uint64_t path = driver + 512;
      r3 = path;
      const uint32_t best = Load(base, frame + 88);
      if (int32_t(Load(base, uint32_t(path) + 5524)) != int32_t(best)) {
        guest::StoreU32(base, uint32_t(path) + 5524, best);
        r3 = ResetPath(base, uint32_t(path));
      }
    } else {
      r3 = SetState4(base, uint32_t(driver), frame + 80, frame - 112);
    }
    table = Load(base, kDrivers);
  }
  return r3;
}

// --- Reading the vehicle ---

namespace {
constexpr uint32_t kYawScale = 0x82008E60;  // -1/60 (probably)
// The route slice record of a route position (the halfwords at `position`:
// the section, then the slice): the section's record (the route file's +12
// table) + 124 + 84 x the slice.
uint32_t SliceRecordAt(uint8_t* base, uint32_t position) {
  const uint32_t table = Load(base, Load(base, track_route::kRoute) + 12);
  const uint32_t section = guest::LoadU16(base, position + 0);
  const int32_t slice = LoadS16(base, position + 2);
  return uint32_t(slice) * 84 + Load(base, table + (section << 2)) + 124;
}
}  // namespace

uint64_t ReadVehicle(uint8_t* base, uint32_t driver, uint32_t slice) {
  ITC_NATIVE_RUNNING("sub_821467F0 AiDriver::ReadVehicle");
  const uint32_t identity = vehicle_accessors::VehicleIdentity(Load(base, driver + 160));
  const uint32_t body = Load(base, Load(base, driver + 160) + 336);
  const uint32_t spin = vehicle_accessors::RigidBodyAngularVelocity(body);
  for (uint32_t i = 0; i < 16; i += 4) {  // two ld/std pairs: the bits
    CopyWord(base, identity + 16 + i, driver + 80 + i);
  }
  Stfs(base, driver + 92, Lfs(base, kOne));
  Stfs(base, driver + 100, double(Vehicle_GetSpeedKmh(base, Load(base, driver + 160))));
  const uint32_t vehicle = Load(base, driver + 160);
  const double yaw = Fmuls(Lfs(base, vehicle + 2356), Lfs(base, spin + 4));
  StoreConstant(base, driver + 164, kFiveHundred);
  StoreConstant(base, driver + 168, kFiveHundred);
  Stfs(base, driver + 104, Fmuls(yaw, Lfs(base, kYawScale)));
  const uint32_t orientation = vehicle_accessors::RigidBodyOrientation(body);
  const double x = Lfs(base, orientation + 0);
  const double zero = Lfs(base, kZero);
  if (x == zero && Lfs(base, orientation + 8) == zero) {
    Stfs(base, driver + 96, zero);
  } else {
    Stfs(base, driver + 96, double(float(c_runtime::Atan2(x, Lfs(base, orientation + 8)))));
  }
  const uint32_t own = Load(base, driver + 160);
  CopyWord(base, own + 320, driver + 108);
  Stfs(base, driver + 112, Fneg(Lfs(base, own + 316)));
  if (slice != 0) {
    const double dx = Fsubs(Lfs(base, driver + 80), Lfs(base, slice + 0));
    const double dz = Fsubs(Lfs(base, driver + 88), Lfs(base, slice + 8));
    const double across = Fmuls(dx, Lfs(base, slice + 36));
    Stfs(base, driver + 144, Fmadds(dz, Lfs(base, slice + 40), across));
    const double side = Lfs(base, slice + 36);
    const double direction =
        double(float(c_runtime::Atan2(Lfs(base, slice + 40), Fneg(side))));
    Stfs(base, driver + 148, Fsubs(direction, Lfs(base, driver + 96)));
    WrapAngle(base, driver + 148);
    UpdateHalfWidth(base, driver);
  } else {
    Stfs(base, driver + 144, zero);
  }
  const uint32_t record = SliceRecordAt(base, identity + 4);
  if (record != 0) {
    const double dx = Fsubs(Lfs(base, driver + 80), Lfs(base, record + 0));
    const double dz = Fsubs(Lfs(base, driver + 88), Lfs(base, record + 8));
    const double along = Fmuls(dx, Lfs(base, record + 44));
    Stfs(base, driver + 156, Fmadds(dz, Lfs(base, record + 48), along));
  } else {
    Stfs(base, driver + 156, zero);
  }
  const uint64_t r3 = UpdateAgainstPlayer(base, driver);
  guest::StoreU32(base, driver + 480, 0xFFFFFFFFu);
  return r3;
}

uint64_t ReadVehicleOwnSlice(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821469D0 AiDriver::ReadVehicleOwnSlice");
  const uint32_t identity = vehicle_accessors::VehicleIdentity(Load(base, driver + 160));
  return ReadVehicle(base, driver, SliceRecordAt(base, identity + 4));
}

// --- The path's branch masks ---

uint64_t BranchMasks(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E500 AiPath::BranchMasks");
  // The lists' address as the original's lis and addi leave it (sign-extended).
  const uint64_t lists = uint64_t(int64_t(int32_t(kBranchMasks)));
  const uint32_t driver = Load(base, path + 0);
  const uint32_t object = Load(base, driver + 500);
  const uint32_t vehicle = Load(base, driver + 160);
  uint32_t kind = Load(base, path + 5524);
  if (int32_t(kind) == -1) {
    kind = Load(base, path + 5528);
  }
  if (int32_t(kind) != -1) {
    return uint64_t((kind << 4) & 0xFFFFFFF0u) + lists;
  }
  if (object != 0) {
    guest::StoreU32(base, frame + 80, uint32_t(int32_t(int8_t(guest::LoadU8(base, object + 36)))));
    return uint64_t(int64_t(int32_t(BranchMasksFor(base, frame + 80))));
  }
  if (vehicle != 0) {
    const uint32_t flags = Load(base, vehicle + 6148);
    if ((flags & 0x81) == 0) {
      if ((flags & 0x4) == 0) {
        return lists;
      }
      const uint32_t other = Load(base, vehicle + 5712);
      if (other == 0) {
        return 0;
      }
      const int32_t word = int32_t(Load(base, other + 12));
      if (word == 66 || word == 67 || word == 70) {
        return lists + 128;
      }
      return 0;
    }
  }
  if (RaceAllowsSides(base) == 0) {
    return lists;
  }
  switch (Load(base, Load(base, kRaceData) + 4)) {
    case 2:
    case 3:
      return lists + 16;
    case 4:
      return lists + 32;
    case 5:
      return lists + 48;
    default:  // 0, 1, 6, 7 and above 7
      return lists;
  }
}

// --- Keeping to the path's entries ---

uint32_t KeepBranch(uint8_t* base, uint32_t path, uint32_t a, uint32_t b, uint32_t point,
                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213EAB0 AiPath::KeepBranch");
  const uint64_t masks = BranchMasks(base, path, frame - 96);
  const uint32_t table = Load(base, Load(base, track_route::kRoute) + 12);
  const uint32_t section_b = Load(base, table + (uint32_t(guest::LoadU16(base, b)) << 2));
  const uint32_t section_a = Load(base, table + (uint32_t(guest::LoadU16(base, a)) << 2));
  const int32_t prefer =
      CompareBranches(base, path, section_a, section_b, a, b, uint32_t(masks));
  if (prefer != 0) {
    return prefer < 0 ? 1 : 0;
  }
  return track_route::LocateAt(base, track_route::kRoute, point, b, frame - 112) != 0 ? 1 : 0;
}

uint32_t LeftEntry(uint8_t* base, uint32_t path, uint32_t point, uint32_t entry,
                   uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213F140 AiPath::LeftEntry");
  const uint32_t at = entry + 16;
  const uint32_t driver = Load(base, path + 0);
  const uint32_t position = driver + 4;
  if (guest::LoadU16(base, at + 0) == guest::LoadU16(base, position + 0) &&
      guest::LoadU16(base, at + 2) == guest::LoadU16(base, position + 2)) {
    const int32_t lane = int32_t(Load(base, entry + 20));
    if (lane < 0 || lane == LoadS16(base, driver + 8)) {
      return 0;
    }
  }
  const uint32_t record = Load(base, entry + 52);
  const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
  const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  const double length = Lfs(base, Load(base, track_route::kRoute) + 8);
  const double along = Fmadds(Lfs(base, record + 44), dx, Fmuls(Lfs(base, record + 48), dz));
  if (along > length) {
    return 1;
  }
  if (along < Fneg(length)) {
    guest::StoreU16(base, path + 5136, 0);
    return 1;
  }
  const double half = Fmuls(Lfs(base, record + 12), Lfs(base, kHalf));
  const double across = Fmadds(Lfs(base, record + 36), dx, Fmuls(Lfs(base, record + 40), dz));
  Stfs(base, frame + 80, across);
  const double minus_half = Fneg(half);
  if (across > half || across < minus_half) {
    if (across > Fadds(Lfs(base, record + 20), half)) {
      return 1;
    }
    if (across < Fsubs(minus_half, Lfs(base, record + 16))) {
      return 1;
    }
    if (KeepBranch(base, path, at, position, point, frame - 128) == 0) {
      return 1;
    }
    if (WithinRoadAt(base, path, frame + 80) == 0) {
      return 1;
    }
  }
  int32_t lane = 0;
  if (!(int32_t(Load(base, entry + 20)) < 0)) {
    lane = int16_t(track_route::LaneAt(base, track_route::kRoute, point, at));
    if (int32_t(Load(base, entry + 20)) != lane) {
      return 1;
    }
  }
  guest::StoreU16(base, position + 0, guest::LoadU16(base, at + 0));
  guest::StoreU16(base, position + 2, guest::LoadU16(base, at + 2));
  guest::StoreU16(base, driver + 8, uint16_t(lane));
  return 0;
}

bool CornersMovable(uint32_t path) {
  const uint32_t records = path + 5164;
  return records < 0xA0000000u && c_runtime::InOneView(records, 9 * 44);
}

bool DropPassed(uint8_t* base, uint32_t path, uint32_t point, uint32_t frame, uint64_t* r3) {
  // AdvanceCorners' condition for its native copies (it only depends on the
  // path): checked first, so the original runs whole where it fails.
  if (!CornersMovable(path)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8213F2E0 AiPath::DropPassed");
  *r3 = path;
  if (!(guest::LoadU16(base, path + 5138) < 32768) || !(LoadS16(base, path + 5136) > 0)) {
    return true;
  }
  const uint32_t before = guest::LoadU16(base, path + 5136);
  const auto first = [&]() {
    return path + 16 + (uint32_t(LoadS16(base, path + 5138)) << 6);
  };
  uint32_t left = LeftEntry(base, path, point, first(), frame - 160);
  *r3 = left;
  if (left == 0) {
    return true;
  }
  guest::StoreU16(base, frame + 80, 0xFFFF);
  guest::StoreU16(base, frame + 82, 0xFFFF);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  int32_t tries = 5;
  for (;;) {
    const uint32_t entry = first();
    const uint32_t none = Load(base, frame + 80);
    Stfs(base, entry + 0, zero);
    guest::StoreU32(base, entry + 16, none);
    Stfs(base, entry + 4, zero);
    guest::StoreU32(base, entry + 20, 0xFFFFFFFFu);
    Stfs(base, entry + 8, zero);
    guest::StoreU32(base, entry + 52, 0);
    Stfs(base, entry + 12, one);
    Stfs(base, entry + 24, zero);
    Stfs(base, entry + 48, zero);
    for (uint32_t at = 28; at <= 44; at += 4) {
      Stfs(base, entry + at, zero);
    }
    const int16_t count = int16_t(guest::LoadU16(base, path + 5136) - 1);
    guest::StoreU16(base, path + 5136, uint16_t(count));
    if (!(count > 0) || --tries < 0) {
      *r3 = ResetPath(base, path);
      return true;
    }
    const int32_t next = LoadS16(base, path + 5138) + 1;
    const int16_t wrapped = int16_t(next - (next / 80) * 80);
    guest::StoreU16(base, path + 5138, uint16_t(wrapped));
    left = LeftEntry(base, path, point, first(), frame - 160);
    *r3 = left;
    if (left == 0) {
      break;
    }
  }
  if (!(LoadS16(base, path + 5136) > 0)) {
    return true;
  }
  const uint32_t entry = first();
  guest::StoreU16(base, frame + 80, uint16_t(before - guest::LoadU16(base, path + 5136)));
  uint32_t ignored = 0;
  AdvanceCorners(base, path, frame + 80, &ignored);  // can't fail: checked above
  DropPassedSections(base, path);
  *r3 = path;
  const uint32_t record = Load(base, entry + 52);
  const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
  const double offset = Fmadds(dz, Lfs(base, record + 40), Fmuls(dx, Lfs(base, record + 36)));
  Stfs(base, entry + 24, offset);
  Stfs(base, entry + 0, Fmadds(Lfs(base, record + 36), offset, Lfs(base, record + 0)));
  CopyWord(base, record + 4, entry + 4);
  Stfs(base, entry + 8, Fmadds(Lfs(base, record + 40), offset, Lfs(base, record + 8)));
  return true;
}

// --- The path's update ---

namespace {
constexpr uint32_t kExtendReach = 0x820027A4;  // 200.0: how far ahead the ring reaches
constexpr uint32_t kLookAhead = 0x82008D44;    // -20.0
constexpr uint32_t kTwelve = 0x82002C78;       // 12.0
}  // namespace

uint64_t Extend(uint8_t* base, uint32_t path, uint32_t point, uint32_t frame,
                const PathCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8213ECF8 AiPath::Extend");
  const uint32_t file = Load(base, track_route::kRoute);
  const double length = Lfs(base, file + 8);
  const double slices = Fdivs(Lfs(base, kExtendReach), length);
  Stfs(base, frame + 92, Fmuls(length, Lfs(base, kHundredth)));
  Stfs(base, frame + 96, Fmuls(length, Lfs(base, kTenth)));
  guest::StoreU32(base, frame + 84, uint32_t(strings_and_font::steps::Fctiwz(slices)));
  int32_t ahead = int32_t(Load(base, frame + 84));
  if (!(ahead < 80)) {
    ahead = 80;
  }
  const int32_t count = LoadS16(base, path + 5136);
  int32_t limit = ahead;
  if (!(count + 10 > ahead)) {
    limit = count + 10;
  }
  guest::StoreU16(base, frame + 80, 0);
  guest::StoreU32(base, frame + 84, 0);
  const uint64_t masks = BranchMasks(base, path, frame - 96);
  uint64_t r3 = masks;
  const auto ring_at = [&](int32_t at) {
    return path + 16 + (uint32_t(at - (at / 80) * 80) << 6);
  };
  uint32_t previous = 0;
  if (count > 0) {
    previous = ring_at(LoadS16(base, path + 5138) + (count - 1));
    guest::StoreU16(base, frame + 84, guest::LoadU16(base, previous + 16));
    guest::StoreU16(base, frame + 86, uint16_t(guest::LoadU16(base, previous + 18) + 1));
    guest::StoreU16(base, frame + 80, uint16_t(Load(base, previous + 20)));
  } else {
    r3 = FindSideSection(base, path, frame + 84, point, uint32_t(masks), frame - 208);
    if (int32_t(Load(base, path + 5520)) == 1) {
      guest::StoreU16(base, frame + 80, guest::LoadU16(base, Load(base, path + 0) + 8));
    } else {
      guest::StoreU16(base, frame + 80, 0xFFFF);
    }
  }
  if (LoadS16(base, path + 5136) < limit) {
    guest::StoreU16(base, frame + 88, 0xFFFF);
    guest::StoreU16(base, frame + 90, 0xFFFF);
    const double one = Lfs(base, kOne);
    const double zero = Lfs(base, kZero);
    for (;;) {
      const uint32_t entry =
          ring_at(LoadS16(base, path + 5138) + LoadS16(base, path + 5136));
      const uint32_t shift = entry + 28;
      const uint32_t none = Load(base, frame + 88);
      Stfs(base, entry + 0, zero);
      guest::StoreU32(base, entry + 20, 0xFFFFFFFFu);
      Stfs(base, entry + 12, one);
      guest::StoreU32(base, entry + 16, none);
      Stfs(base, entry + 48, zero);
      guest::StoreU32(base, entry + 52, 0);
      Stfs(base, entry + 4, zero);
      Stfs(base, entry + 8, zero);
      Stfs(base, entry + 24, zero);
      Stfs(base, shift, zero);
      for (uint32_t at = 32; at <= 44; at += 4) {
        Stfs(base, entry + at, zero);
      }
      const uint32_t normalizer = Load(base, path + 5532);
      if (normalizer == kNormalizeByLane) {
        r3 = NormalizeByLane(base, entry, frame + 84, frame + 80, uint32_t(masks));
      } else if (normalizer == kNormalizeByBranch) {
        r3 = NormalizeByBranch(base, path, entry, frame + 84, frame + 80, uint32_t(masks));
      } else {
        r3 = calls.normalize(calls.user, normalizer, frame, path, entry, frame + 84, frame + 80,
                             masks);
      }
      if (int32_t(uint32_t(r3)) != 1) {
        break;
      }
      guest::StoreU16(base, frame + 84, guest::LoadU16(base, entry + 16));
      guest::StoreU16(base, frame + 86, uint16_t(guest::LoadU16(base, entry + 18) + 1));
      const auto add_section = [&](uint16_t section) {
        const uint32_t at = uint32_t(LoadS16(base, path + 5144) + 2573) << 1;
        guest::StoreU16(base, path + at, section);
        guest::StoreU16(base, path + 5144, uint16_t(guest::LoadU16(base, path + 5144) + 1));
        r3 = DropOffRouteCorners(base, path);
      };
      if (previous != 0) {
        uint32_t record = Load(base, entry + 52);
        if (guest::LoadU16(base, entry + 16) == guest::LoadU16(base, previous + 16)) {
          Stfs(base, shift, zero);
        } else {
          const uint32_t before = Load(base, previous + 52);
          const double dx = Fsubs(Lfs(base, before + 0), Lfs(base, record + 0));
          const double dz = Fsubs(Lfs(base, before + 8), Lfs(base, record + 8));
          Stfs(base, shift, Fmadds(dz, Lfs(base, record + 40), Fmuls(dx, Lfs(base, record + 36))));
          add_section(guest::LoadU16(base, entry + 16));
          record = Load(base, entry + 52);
        }
        CopyWord(base, record + 24, entry + 32);
        CopyWord(base, record + 28, entry + 36);
        CopyWord(base, record + 28, entry + 44);
        CopyWord(base, entry + 32, entry + 40);
        NarrowToOther(base, entry, previous, frame + 92, shift);
        r3 = NarrowBehind(base, path, entry, previous, frame + 92, frame + 96, frame - 128);
      } else if (guest::LoadU16(base, path + 5136) == 0) {
        const uint32_t record = Load(base, entry + 52);
        const uint16_t section = guest::LoadU16(base, entry + 16);
        CopyWord(base, record + 24, entry + 32);
        CopyWord(base, record + 28, entry + 36);
        CopyWord(base, entry + 32, entry + 40);
        CopyWord(base, entry + 36, entry + 44);
        const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
        const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
        const double offset = Fmadds(dz, Lfs(base, record + 40), Fmuls(dx, Lfs(base, record + 36)));
        Stfs(base, entry + 24, offset);
        Stfs(base, entry + 0, Fmadds(Lfs(base, record + 36), offset, Lfs(base, record + 0)));
        CopyWord(base, record + 4, entry + 4);
        Stfs(base, entry + 8, Fmadds(Lfs(base, record + 40), offset, Lfs(base, record + 8)));
        add_section(section);
      }
      uint32_t corners_r3 = 0, a = 0, b = 0;
      const uint32_t corners = frame - 144;
      const uint32_t step = AddCornersAt(base, path, entry, corners, &corners_r3, &a, &b);
      if (step >= 2) {
        r3 = calls.join(calls.user, step, corners, path, a, b);
        AddCornersAtFinish(base, path);
      } else {
        r3 = corners_r3;
      }
      previous = entry;
      const int16_t added = int16_t(guest::LoadU16(base, path + 5136) + 1);
      guest::StoreU16(base, path + 5136, uint16_t(added));
      if (!(added < limit)) {
        break;
      }
    }
  }
  if (int32_t(Load(base, path + 5520)) != 1) {
    ApplyCornerLines(base, path, frame - 112);
    r3 = path;
  }
  const uint32_t driver = Load(base, path + 0);
  if (driver == 0) {
    return r3;
  }
  uint32_t inside = 0;
  if (LoadS16(base, path + 5162) > 0) {
    uint32_t record = path + 5164;
    int32_t i = 0;
    do {
      const uint32_t data = Load(base, record + 0);
      if (data != 0) {
        const int32_t offset = LoadS16(base, record + 10);
        const int32_t look = LoadS16(base, path + 5142);
        if (!(look < LoadS16(base, data + 48) + offset) &&
            !(look > LoadS16(base, data + 54) + offset)) {
          inside = 1;
          break;
        }
      }
      i++;
      record += 44;
    } while (i < LoadS16(base, path + 5162));
  }
  guest::StoreU32(base, driver + 496, inside);
  return r3;
}

bool UpdatePath(uint8_t* base, uint32_t path, uint32_t frame, const PathCalls& calls,
                uint64_t* r3) {
  if (!CornersMovable(path)) {  // DropPassed's condition, checked first
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8213F530 AiPath::Update");
  *r3 = path;
  guest::StoreU32(base, frame + 80, 0);
  const uint32_t file = Load(base, track_route::kRoute);
  const uint32_t driver = Load(base, path + 0);
  const uint32_t point = driver + 80;
  if (!(guest::LoadU16(base, driver + 8) < 32768)) {
    return true;
  }
  FollowLeaderBranch(base, path);
  uint64_t dropped = 0;
  DropPassed(base, path, point, frame - 160, &dropped);  // can't fail: checked above
  *r3 = Extend(base, path, point, frame - 208, calls);
  const int16_t entries = int16_t(guest::LoadU16(base, path + 5136));
  const int32_t count = entries;
  const int32_t reach =
      strings_and_font::steps::Fctiwz(Fdivs(Lfs(base, kLookAhead), Lfs(base, file + 8)));
  const int16_t look = int16_t(guest::LoadU16(base, path + 5142) - uint16_t(reach) + 1);
  guest::StoreU16(base, path + 5140, uint16_t(look));
  if (look > count) {
    guest::StoreU16(base, path + 5140, uint16_t(entries));
  }
  if (!(count > 1)) {
    return true;
  }
  const uint32_t own = Load(base, path + 0);
  if ((Load(base, own + 16) & 0x1) != 0) {
    return true;
  }
  const double speed = Fadds(Lfs(base, own + 100), Lfs(base, kTwenty));
  const double per = Fdivs(Lfs(base, Load(base, track_route::kRoute) + 8), speed);
  int32_t span = int32_t(uint32_t(LoadS16(base, path + 5140)) << 2);
  guest::StoreU32(base, frame + 80, uint32_t(span));
  Stfs(base, path + 5516, Fmuls(per, Lfs(base, kTwelve)));
  if (span > count) {
    guest::StoreU32(base, frame + 80, uint32_t(count));
  }
  SetRanges(base, path, frame + 80, frame - 128);
  ReactToOthers(base, Load(base, path + 0), frame + 80, frame - 240);
  const uint32_t mine = Load(base, path + 0);
  const int32_t first = int32_t(Load(base, mine + 452));
  int32_t ring = -1;
  if (!(first < 0)) {
    ring = LoadS16(base, uint32_t(first) * 40 + mine + 198);
  }
  if (!(ring < 0)) {
    AvoidOthers(base, path, frame - 272);
  }
  if (int32_t(Load(base, path + 5520)) == 1) {
    FollowTargets(base, path, frame - 128);
  } else {
    PlanLine(base, path, frame - 96);
  }
  RefreshPoints(base, path);
  *r3 = path;
  return true;
}

}  // namespace ai_drivers
