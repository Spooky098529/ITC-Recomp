// The AI drivers (docs/functions/ai-drivers.md): see ai_drivers.h.

#include "ai_drivers/ai_drivers.h"

#include "c_runtime/math.h"
#include "car_physics/float_ops.h"
#include "strings_and_font/steps.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/log.h"

#include <cmath>

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
// The path's normalizers (+5532): by lane (mode 1), by branch (mode 0).
constexpr uint32_t kNormalizeByLane = 0x8213B428;
constexpr uint32_t kNormalizeByBranch = 0x8213C908;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
int32_t LoadS16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
}
// An lfs then stfs of a constant.
void StoreConstant(uint8_t* base, uint32_t address, uint32_t constant) {
  Stfs(base, address, Lfs(base, constant));
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
    Stfs(base, entry + 20, Lfs(base, record + 4));
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
  Stfs(base, driver + 92, Lfs(base, kOne));
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
  Stfs(base, out + 20, Lfs(base, record + 20));
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
  Stfs(base, out + 16, Lfs(base, value));
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

}  // namespace ai_drivers
