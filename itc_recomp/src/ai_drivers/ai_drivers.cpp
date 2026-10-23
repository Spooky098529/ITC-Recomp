// The AI drivers (docs/functions/ai-drivers.md): see ai_drivers.h.

#include "ai_drivers/ai_drivers.h"

#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "car_physics/float_ops.h"
#include "strings_and_font/steps.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/log.h"

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

uint32_t UpdateSteerInput(uint8_t* base, uint32_t driver, uint32_t* record) {
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
    if (kind == 2 || kind == 3) {
      *record = at;
      return uint32_t(kind);
    }
  }
  AccumulateSteer(base, driver);
  return 0;
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

}  // namespace ai_drivers
