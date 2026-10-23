// The AI drivers (docs/functions/ai-drivers.md): see ai_drivers.h.

#include "ai_drivers/ai_drivers.h"

#include "c_runtime/math.h"
#include "car_physics/float_ops.h"
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

uint64_t Of(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82139728 AiDriver::Of");
  const uint32_t table = Load(base, kDrivers);
  if (table == 0) {
    return 0;
  }
  return uint64_t(Load(base, index)) * kDriverBytes + table;
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

}  // namespace ai_drivers
