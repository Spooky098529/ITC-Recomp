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
#include "c_runtime/arrays.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/ai_drivers.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "schema/minimap.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/track_route.h"

#include <cmath>
#include <cstring>

namespace ai_drivers {

using namespace car_physics::ops;
using schema::AiCorner;
using schema::AiDriver;
using schema::AiOther;
using schema::AiOtherSpan;
using schema::AiPath;
using schema::AiPathEntry;
using schema::AiProfile;
using schema::AiSkill;
using schema::AiSkillTrigger;
using schema::CarInput;
using schema::MinimapRacerEntry;
using schema::RaceManager;
using schema::RouteCorner;
using schema::RouteCornerEntry;
using schema::RouteSection;
using schema::RouteSlice;
using schema::TrackPosition;
using schema::TrackRouteFile;
using schema::Vehicle;
using G = schema::AiDriversGlobals;
using RG = schema::TrackRouteGlobals;

namespace {

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
  AiDriver(base, driver).set_route_section(0);
  const double one = Lfs(base, kOne);
  AiDriver(base, driver).set_route_slice(0);
  Stfs(base, AiDriver(base, driver).plan_a_w_address(), one);
  Stfs(base, AiDriver(base, driver).plan_b_w_address(), one);
  Stfs(base, AiDriver(base, driver).point_w_address(), one);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t entry = driver + 528 + 64 * i;
    Stfs(base, AiPathEntry(base, entry).w_address(), one);
    AiPathEntry(base, entry).set_section(0);
    AiPathEntry(base, entry).set_slice(0);
  }
  Stfs(base, AiDriver(base, driver).target_w_address(), one);
}

uint64_t OfIndex(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82139728 AiDriver::Of");
  const uint32_t table = Load(base, G::drivers_address());
  if (table == 0) {
    return 0;
  }
  return uint64_t(index) * AiDriver::kSize + table;
}

uint64_t Of(uint8_t* base, uint32_t index) {
  return OfIndex(base, Load(base, index));
}

void ClearCarInput(uint8_t* base, uint32_t input) {
  ITC_NATIVE_RUNNING("sub_820C96E8 CarInput::Clear");
  CarInput(base, input).set_throttle(0);
  CarInput(base, input).set_brake(0);
  CarInput(base, input).set_handbrake(0);
  CarInput(base, input).set_steering(0);
  CarInput(base, input).set_flags(0);
  CarInput(base, input).set_kind(0);
  CarInput(base, input).set_steering_rate(0);
  CarInput(base, input).set_brake_rate(0);
  CarInput(base, input).set_throttle_rate(0);
  CarInput(base, input).set_camera(0);
}

void ClearFollow(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146508 AiDriver::ClearFollow");
  StoreConstant(base, AiDriver(base, driver).cruise_speed_address(), G::five_hundred_address());
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  AiDriver(base, driver).set_lane(0xFFFF);
  AiDriver(base, driver).set_flags(0);
  AiDriver(base, driver).set_route_position(0xFFFFFFFFu);
  AiDriver(base, driver).set_state(0);
  AiDriver(base, driver).set_previous_state(0);
  AiDriver(base, driver).set_racer_mask(0);
  AiDriver(base, driver).set_plan_word(0xFFFFFFFFu);
  Stfs(base, AiDriver(base, driver).plan_a_x_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_a_y_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_a_z_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_a_w_address(), one);
  Stfs(base, AiDriver(base, driver).plan_b_x_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_b_y_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_b_z_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_b_w_address(), one);
}

void ClearOtherIndexes(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82144ED8 AiDriver::ClearOtherIndexes");
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU16(base, driver + 202 + 40 * i, 0);
  }
}

void ClearCorners(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213C720 AiPath::ClearCorners");
  AiPath(base, path).set_corner_count(0);
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t record = path + 5164 + 44 * i;
    Stfs(base, AiCorner(base, record).direction_address(), one);
    AiCorner(base, record).set_data(0);
    Stfs(base, AiCorner(base, record).speed_address(), zero);
    AiCorner(base, record).set_table(0);
    Stfs(base, AiCorner(base, record).slope_before_address(), zero);
    AiCorner(base, record).set_number(0);
    Stfs(base, AiCorner(base, record).slope_after_address(), zero);
    AiCorner(base, record).set_entry_offset(0);
    AiCorner(base, record).set_end(0);
    AiCorner(base, record).set_position(0);
    AiCorner(base, record).set_unknown_24(0);
    AiCorner(base, record).set_joined_before(0);
    AiCorner(base, record).set_joined_after(0);
    AiCorner(base, record).set_kind(0);
  }
}

void ClearCornerKind(uint8_t* base, uint32_t corner) {
  ITC_NATIVE_RUNNING("sub_8213C710 AiPath::ClearCornerKind");
  AiCorner(base, corner).set_kind(0);
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
  AiSkill(base, skill).set_speed_scale(0);
  StoreConstant(base, AiSkill(base, skill).percentage_address(), G::hundred_address());
  AiSkill(base, skill).set_speed(500);
  AiSkill(base, skill).set_throttle_cap(80);
  AiSkill(base, skill).set_corner(uint16_t(-100));
}

// --- Small reads and steps ---

void WrapAngle(uint8_t* base, uint32_t angle) {
  ITC_NATIVE_RUNNING("sub_82145408 Angle::Wrap");
  const double a = Lfs(base, angle);
  if (a > Lfs(base, G::pi_address())) {
    Stfs(base, angle, Fsubs(a, Lfs(base, G::two_pi_address())));
  } else if (a < Lfs(base, G::minus_pi_address())) {
    Stfs(base, angle, Fadds(a, Lfs(base, G::two_pi_address())));
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
  const uint32_t flags = AiDriver(base, driver).flags();
  AiDriver(base, driver).set_flags(set ? flags | 2 : flags & ~2u);
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  if (vehicle != 0) {
    vehicle_accessors::VehicleSetObjectHalf114(base, vehicle, Load(base, word));
  }
  return 1;
}

uint32_t IsFree(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821464B8 AiDriver::IsFree");
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  if (vehicle == 0) {
    return 0;
  }
  if ((Vehicle(base, vehicle).car_flags() & 129) != 0 &&
      (AiDriver(base, driver).flags() & 2) == 0) {
    return 0;
  }
  const uint32_t state = AiDriver(base, driver).state();
  return state == 0 || state == 6 ? 1 : 0;
}

double SpeedOverDistance(uint8_t* base, uint32_t driver, uint32_t distance) {
  ITC_NATIVE_RUNNING("sub_821450E0 AiDriver::SpeedOverDistance");
  double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  const double factor = Lfs(base, AiDriver(base, driver).distance_factor_address());
  const double hundred = Lfs(base, G::hundred_address());
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
  const double least = Lfs(base, G::thirty_address());
  double f1 = least;
  const double ten = Lfs(base, G::ten_address());
  if (c > ten) {
    c = Fsubs(c, ten);
    const double f11 = Lfs(base, AiDriver(base, driver).steer_factor_address());
    const double f10 = Lfs(base, AiDriver(base, driver).corner_grip_address());
    c = Fsqrts(c);
    c = Fmuls(c, f11);
    c = Fmuls(c, f10);
    f1 = Fmadds(c, ten, least);
  }
  double w = Lfs(base, width);
  const double room = Lfs(base, AiDriver(base, driver).car_radius_address());
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
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  if (vehicle == 0) {
    return 0;
  }
  double width = Fneg(Lfs(base, vehicle + 5144));
  double f0 = Lfs(base, vehicle + 5160);
  if (width < f0) {
    width = f0;
  }
  Stfs(base, AiDriver(base, driver).car_half_width_address(), width);
  const double back = Fneg(Lfs(base, vehicle + 5136));
  double length = Lfs(base, vehicle + 5152);
  if (!(back < length)) {
    length = back;
  }
  const double f9 = Fmuls(width, width);
  Stfs(base, AiDriver(base, driver).half_width_address(), width);
  Stfs(base, AiDriver(base, driver).car_half_length_address(), length);
  const double f13 = Lfs(base, 0x82002A10);
  Stfs(base, AiDriver(base, driver).corner_grip_address(), f13);
  const double f11 = Lfs(base, 0x82002CB8);
  double diagonal = Fmadds(length, length, f9);
  Stfs(base, AiDriver(base, driver).distance_factor_address(), f11);
  const double f10 = Lfs(base, 0x82002A14);
  diagonal = Fsqrts(diagonal);
  Stfs(base, AiDriver(base, driver).car_radius_address(), diagonal);
  const double half = Lfs(base, G::half_address());
  f0 = Lfs(base, vehicle + 3900);
  f0 = Fmuls(f0, half);
  f0 = Fmadds(f0, f10, f13);
  Stfs(base, AiDriver(base, driver).corner_grip_address(), f0);
  f0 = Lfs(base, 0x82001FD4);
  double f13b = Lfs(base, vehicle + 3444);
  f13b = Fdivs(f0, f13b);
  f0 = Lfs(base, 0x82002A2C);
  f0 = Fmadds(f13b, half, f0);
  const double one = Lfs(base, kOne);
  Stfs(base, AiDriver(base, driver).steer_factor_address(), f0);
  if (f0 > one) {
    Stfs(base, AiDriver(base, driver).steer_factor_address(), one);
  }
  double sum = Lfs(base, vehicle + 3512);
  const double f12 = Lfs(base, vehicle + 3508);
  sum = Fadds(sum, f12);
  f0 = Lfs(base, 0x82002204);
  Stfs(base, AiDriver(base, driver).distance_factor_address(), Fmadds(sum, f0, f11));
  return 1;
}

void UpdateHalfWidth(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145230 AiDriver::UpdateHalfWidth");
  const double cos = c_runtime::Cos(Lfs(base, AiDriver(base, driver).road_angle_address()));
  const double sin = c_runtime::Sin(Lfs(base, AiDriver(base, driver).road_angle_address()));
  const double c = Fabs(double(float(cos)));
  const double across = Lfs(base, AiDriver(base, driver).car_half_width_address());
  const double s = Fabs(double(float(sin)));
  const double along = Lfs(base, AiDriver(base, driver).car_half_length_address());
  Stfs(base, AiDriver(base, driver).half_width_address(), Fmadds(along, s, Fmuls(across, c)));
}

uint32_t TargetPoint(uint8_t* base, uint32_t path, double ahead, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213B4D8 AiPath::TargetPoint");
  const int32_t count = LoadS16(base, AiPath(base, path).entry_count_address());
  const uint32_t file = RG::route_file(base).address;
  const int32_t k = LoadS16(base, AiPath(base, path).look_ahead_address());
  if (!(k + 1 < count)) {
    return 0;
  }
  const double length = Lfs(base, TrackRouteFile(base, file).slice_length_address());
  const int32_t k2 = LoadS16(base, AiPath(base, path).look_ahead_address());
  const int32_t at = LoadS16(base, AiPath(base, path).first_address()) + k2;
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
  AiSkill(base, skill).set_source(0);
  AiSkill(base, skill).set_runs_triggers(0);
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
  AiSkill(base, skill).set_runs_triggers(0);
  AiSkill(base, skill).set_source(source);
  if (source != 0) {
    ResetSkill(base, skill);
  }
}

void LoadSkill(uint8_t* base, uint32_t skill) {
  ITC_NATIVE_RUNNING("sub_8213F730 AiSkill::Load");
  if (AiSkill(base, skill).source().address != 0) {
    for (uint32_t i = 0; i < 14; i += 2) {
      const uint32_t source = AiSkill(base, skill).source().address;
      guest::StoreU16(base, skill + 4 + i, guest::LoadU16(base, source + i));
    }
    ClearTriggers(base, skill);
  } else {
    ResetSkill(base, skill);
    AiSkill(base, skill).set_throttle_cap(100);
    if (G::race_data(base).kind() == 0) {
      AiSkill(base, skill).set_speed_scale(uint16_t(-5));
    }
  }
  StoreConstant(base, AiSkill(base, skill).percentage_address(), G::hundred_address());
}

// --- Speeds and steering ---

double LimitSpeed(uint8_t* base, uint32_t driver, uint32_t speed, uint32_t curvature,
                  uint32_t width, uint32_t slices, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139248 AiDriver::LimitSpeed");
  double f1 = CornerFactor(base, driver, curvature, width);
  const int32_t n = int32_t(Load(base, slices));
  double limit = Fmuls(Lfs(base, speed), f1);
  if (n > 0) {
    const double length = Lfs(base, RG::route_file(base).slice_length_address());
    const double count = double(float(double(int64_t(n))));
    Stfs(base, frame + 80, Fmuls(length, count));
    f1 = SpeedOverDistance(base, driver, frame + 80);
    limit = Fadds(f1, limit);
  }
  if (Lfs(base, AiDriver(base, driver).target_speed_address()) > limit) {
    Stfs(base, AiDriver(base, driver).target_speed_address(), limit);
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
  const double at = Lfs(base, AiDriver(base, driver).road_offset_address());
  const double entry = Lfs(base, entries + uint32_t(i) * 8 + 4);
  const double past = Fsubs(at, entry);
  const double zero = Lfs(base, kZero);
  const double range = past < zero ? Lfs(base, AiPathEntry(base, record).range_low_address())
                                   : Lfs(base, AiPathEntry(base, record).range_high_address());
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
  double steer = Lfs(base, AiDriver(base, driver).steer_shift_address());
  const double zero = Lfs(base, kZero);
  if (steer < zero) {
    steer = Fneg(steer);
  }
  steer = Fmuls(steer, Lfs(base, G::tenth_address()));
  if (!(steer > zero)) {
    return;
  }
  const double throttle = Lfs(base, AiDriver(base, driver).throttle_request_address());
  if (throttle < steer) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
  } else {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), Fsubs(throttle, steer));
  }
}

// --- The path's corners and points ---

namespace {
// The first corner record before the first with a positive offset whose
// kind is in [low, high]: 1, else 0.
uint32_t KindAhead(uint8_t* base, uint32_t path, int32_t low, int32_t high) {
  const int32_t count = LoadS16(base, AiPath(base, path).corner_count_address());
  for (int32_t i = 0; i < count; i++) {
    const uint32_t record = path + 5164 + 44 * uint32_t(i);
    if (LoadS16(base, AiCorner(base, record).entry_offset_address()) > 0) {
      return 0;
    }
    const int32_t kind = int32_t(AiCorner(base, record).kind());
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
  return G::section_lists_address() + which * 64;
}

uint32_t RaceAllowsSides(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8213D318 AiPath::RaceAllowsSides");
  const int32_t mode = int32_t(G::race_data(base).race_mode());
  if (mode == 4 || mode == 7 || mode == 8) {
    return 1;
  }
  if (mode == 5) {
    const int32_t state = int32_t(G::race_state(base).stage_type());
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
  for (int32_t i = 1; i < LoadS16(base, AiPath(base, path).plan_count_address()); i++) {
    const uint32_t entry = RingEntry(path, LoadS16(base, AiPath(base, path).first_address()) + i);
    const uint32_t record = Load(base, entry + 68);
    const double offset = Lfs(base, entry + 40);
    const double cx = Lfs(base, RouteSlice(base, record).center_x_address());
    Stfs(base, entry + 16,
         Fmadds(Lfs(base, RouteSlice(base, record).side_x_address()), offset, cx));
    CopyWord(base, RouteSlice(base, record).center_y_address(), entry + 20);
    const double dz = Lfs(base, RouteSlice(base, record).side_z_address());
    Stfs(base, entry + 24,
         Fmadds(dz, offset, Lfs(base, RouteSlice(base, record).center_z_address())));
  }
}

uint32_t ResetPath(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213D438 AiPath::Reset");
  AiPath(base, path).set_look_ahead(1);
  AiPath(base, path).set_entry_count(0);
  const double one = Lfs(base, kOne);
  AiPath(base, path).set_first(0);
  const double zero = Lfs(base, kZero);
  AiPath(base, path).set_plan_count(0);
  for (uint32_t i = 0; i < 80; i++) {
    const uint32_t entry = path + 16 + 64 * i;
    Stfs(base, AiPathEntry(base, entry).x_address(), zero);
    Stfs(base, AiPathEntry(base, entry).y_address(), zero);
    AiPathEntry(base, entry).set_lane(0xFFFFFFFFu);
    Stfs(base, AiPathEntry(base, entry).z_address(), zero);
    AiPathEntry(base, entry).set_slice_record(0);
    Stfs(base, AiPathEntry(base, entry).w_address(), one);
    Stfs(base, AiPathEntry(base, entry).offset_address(), zero);
    Stfs(base, AiPathEntry(base, entry).target_offset_address(), zero);
    AiPathEntry(base, entry).set_route_position(0xFFFFFFFFu);  // the route position (-1, -1)
    Stfs(base, AiPathEntry(base, entry).step_address(), zero);
    Stfs(base, AiPathEntry(base, entry).range_low_address(), zero);
    Stfs(base, AiPathEntry(base, entry).range_high_address(), zero);
    Stfs(base, AiPathEntry(base, entry).room_low_address(), zero);
    Stfs(base, AiPathEntry(base, entry).room_high_address(), zero);
  }
  ClearCorners(base, path);
  AiPath(base, path).set_section_count(0);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU16(base, path + 5146 + 2 * i, 0xFFFF);
  }
  const uint32_t driver = AiPath(base, path).driver().address;
  if (driver != 0) {
    ClearOtherIndexes(base, driver);
  }
  return driver;
}

void SetLaneOffset(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8213B390 AiPathEntry::SetLaneOffset");
  const uint32_t record = AiPathEntry(base, entry).slice_record().address;
  const int32_t lane = int32_t(AiPathEntry(base, entry).lane());
  const int32_t lanes = RouteSlice(base, record).lanes() & 0xF;
  if (lane < 0) {
    AiPathEntry(base, entry).set_lane(0);
  } else if (!(lane < lanes)) {
    AiPathEntry(base, entry).set_lane(lanes > 1 ? uint32_t(lanes - 1) : 0);
  }
  const int32_t kept = int32_t(AiPathEntry(base, entry).lane());
  const double width = Lfs(base, RouteSlice(base, record).width_address());
  const double half = Lfs(base, G::half_address());
  const double n = double(float(double(int64_t(RouteSlice(base, record).lanes() & 0xF))));
  const double at = double(float(double(int64_t(kept))));
  const double from = Fnmsubs(Fsubs(n, Lfs(base, kOne)), half, at);
  const double offset = Fmuls(Fdivs(from, n), width);
  Stfs(base, AiPathEntry(base, entry).offset_address(), offset);
  Stfs(base, AiPathEntry(base, entry).target_offset_address(), offset);
}

// --- The other racers ---

void UpdateOtherFlags(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82145450 AiOther::UpdateFlags");
  uint32_t flags = AiOther(base, record).history_flags() & ~6u;
  const int32_t state = LoadS16(base, AiOther(base, record).state_address());
  AiOther(base, record).set_history_flags(uint16_t(flags));
  if (state == 3) {
    flags |= 8;
    AiOther(base, record).set_history_flags(uint16_t(flags));
    if ((flags & 0x10) == 0) {
      return;
    }
    flags = (flags | 4) & ~0x10u;
  } else if (state == 1) {
    flags |= 16;
    AiOther(base, record).set_history_flags(uint16_t(flags));
    if ((flags & 8) == 0) {
      return;
    }
    flags = (flags | 2) & ~8u;
  } else {
    return;
  }
  AiOther(base, record).set_history_flags(uint16_t(flags));
}

void UpdateOtherTimer(uint8_t* base, uint32_t record, uint32_t index, uint32_t limit) {
  ITC_NATIVE_RUNNING("sub_821456A0 AiOther::UpdateTimer");
  const uint32_t timer = AiOther(base, record).timer();
  const double gap = Lfs(base, AiOther(base, record).gap_address());
  const double thirty = Lfs(base, G::thirty_address());
  AiOther(base, record).set_timer(uint16_t(timer - 1));
  if (!(gap < thirty)) {
    return;
  }
  if (!(gap > Lfs(base, limit))) {
    return;
  }
  const uint32_t input = Load(base, G::inputs_address() + (Load(base, index) & 0xFF) * 4);
  if ((CarInput(base, input).flags() & 0x20000) == 0) {
    return;
  }
  AiOther(base, record).set_timer(120);
}

namespace {
// An other-racer record reset (ClearOthers' and ResetOthers' stores, in
// their order).
void ResetOtherRecord(uint8_t* base, uint32_t record, double gap, double zero, double one,
                      double minus_one) {
  Stfs(base, AiOther(base, record).gap_address(), gap);
  AiOther(base, record).set_flag_a(0);
  Stfs(base, AiOther(base, record).closing_rate_address(), zero);
  AiOther(base, record).set_flag_b(0);
  Stfs(base, AiOther(base, record).last_change_address(), zero);
  AiOther(base, record).set_timer(0);
  Stfs(base, AiOther(base, record).speed_address(), zero);
  AiOther(base, record).set_state(0);
  Stfs(base, AiOther(base, record).factor_address(), one);
  AiOther(base, record).set_action_flags(0);
  Stfs(base, AiOther(base, record).ticks_to_meet_address(), minus_one);
  AiOther(base, record).set_ring_index(0xFFFF);
  AiOther(base, record).set_history_flags(0);
  AiOther(base, record).set_unknown_36(0);
}
}  // namespace

void ClearOthers(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821459B0 AiDriver::ClearOthers");
  const double zero = Lfs(base, kZero);
  Stfs(base, AiDriver(base, driver).player_gap_address(), zero);
  AiDriver(base, driver).set_passes_a(0);
  const double f11 = Lfs(base, G::minus_one_address());
  const double one = Lfs(base, kOne);
  AiDriver(base, driver).set_passes_b(0);
  const double gap = Lfs(base, G::other_gap_address());
  AiDriver(base, driver).set_stuck_count(0);
  AiDriver(base, driver).set_in_corner(0);
  for (uint32_t i = 0; i < 7; i++) {
    ResetOtherRecord(base, driver + 172 + 40 * i, gap, zero, one, f11);
    guest::StoreU32(base, driver + 452 + 4 * i, 0xFFFFFFFFu);
  }
}

namespace {
// Whether racer i is in the driver's mask (+40, sign-extended) and isn't
// the driver itself (+0).
bool InMask(uint8_t* base, uint32_t driver, int32_t i) {
  if (int32_t(AiDriver(base, driver).index()) == i) {
    return false;
  }
  return ((1u << i) & uint32_t(LoadS16(base, AiDriver(base, driver).racer_mask_address()))) != 0;
}
}  // namespace

void PickTarget(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145118 AiDriver::PickTarget");
  const double zero = Lfs(base, kZero);
  double best = zero;
  if (!(int32_t(AiDriver(base, driver).watched()) < 0)) {
    return;
  }
  AiDriver(base, driver).set_watched(0xFFFFFFFFu);
  for (int32_t i = 0; i < 7; i++) {
    if (!InMask(base, driver, i)) {
      continue;
    }
    const uint32_t record = driver + 172 + 40 * uint32_t(i);
    const double gap = Lfs(base, AiOther(base, record).gap_address());
    const int32_t current = int32_t(AiDriver(base, driver).watched());
    bool take;
    if (gap < zero) {
      take = current < 0 || gap > best;
    } else {
      take = current < 0 || best < zero || gap < best;
    }
    if (take) {
      AiDriver(base, driver).set_watched(uint32_t(i));
      best = Lfs(base, AiOther(base, record).gap_address());
    }
  }
}

void PickTargetBehind(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821451B8 AiDriver::PickTargetBehind");
  const double zero = Lfs(base, kZero);
  double best = zero;
  AiDriver(base, driver).set_watched(0xFFFFFFFFu);
  for (int32_t i = 0; i < 7; i++) {
    if (!InMask(base, driver, i)) {
      continue;
    }
    const uint32_t record = driver + 172 + 40 * uint32_t(i);
    const double gap = Lfs(base, AiOther(base, record).gap_address());
    if (!(gap < zero)) {
      continue;
    }
    if (int32_t(AiDriver(base, driver).watched()) < 0 || gap > best) {
      AiDriver(base, driver).set_watched(uint32_t(i));
      best = Lfs(base, AiOther(base, record).gap_address());
    }
  }
}

uint32_t KeepsGapFrom(uint8_t* base, uint32_t driver, uint32_t other) {
  ITC_NATIVE_RUNNING("sub_82145048 AiDriver::KeepsGapFrom");
  const uint32_t state = AiDriver(base, driver).state();
  if (state == 0) {
    return 1;
  }
  const uint32_t index = Load(base, other);
  const uint32_t bit = (index & 0x20) != 0 ? 0 : 1u << (index & 0x1F);
  if ((uint32_t(LoadS16(base, AiDriver(base, driver).racer_mask_address())) & bit) != 0) {
    return 0;
  }
  if (state - 2 > 1) {
    return 1;
  }
  if (!(Lfs(base, driver + index * 40 + 172) > Lfs(base, kZero))) {
    return 1;
  }
  const uint32_t theirs = uint32_t(OfIndex(base, index));
  return AiDriver(base, theirs).state() - 2 > 1 ? 1 : 0;
}

// --- Set-up, modes and the plan ---

void SetParams(uint8_t* base, uint32_t driver, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82138D00 AiDriver::SetParams");
  AiDriver(base, driver).set_profile(object);
  SetSkillSource(base, driver + 6048, object != 0 ? object + 120 : 0);
}

void SetSkill(uint8_t* base, uint32_t driver, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821393D8 AiDriver::SetSkill");
  LoadSkill(base, driver + 6048);
  const uint32_t value = Load(base, word);
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  AiDriver(base, driver).set_skill_flag(value);
  const int32_t skill = LoadS16(base, AiDriver(base, driver).skill_speed_address());
  Stfs(base, AiDriver(base, driver).cruise_speed_address(), double(float(double(int64_t(skill)))));
  if (vehicle != 0) {
    StoreConstant(base, Vehicle(base, vehicle).brake_efficiency_address(), kOne);
  }
}

uint32_t ClearPlan(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139820 AiDriver::ClearPlan");
  const uint32_t path = driver + 512;
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  AiDriver(base, driver).set_plan_word(0xFFFFFFFFu);
  AiDriver(base, driver).set_state(0);
  AiDriver(base, driver).set_previous_state(0);
  AiDriver(base, driver).set_racer_mask(0);
  Stfs(base, AiDriver(base, driver).plan_a_x_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_a_y_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_a_z_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_a_w_address(), one);
  Stfs(base, AiDriver(base, driver).plan_b_x_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_b_y_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_b_z_address(), zero);
  Stfs(base, AiDriver(base, driver).plan_b_w_address(), one);
  if (int32_t(AiPath(base, path).branch_kind()) != -1 ||
      int32_t(AiPath(base, path).leader_branch_kind()) != -1) {
    AiPath(base, path).set_branch_kind(0xFFFFFFFFu);
    AiPath(base, path).set_leader_branch_kind(0xFFFFFFFFu);
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
  if (int32_t(value) == int32_t(AiPath(base, path).mode())) {
    return path;
  }
  AiPath(base, path).set_mode(value);
  const uint32_t again = Load(base, mode);
  if (again == 0) {
    AiPath(base, path).set_normalizer(kNormalizeByBranch);
  } else if (again == 1) {
    AiPath(base, path).set_normalizer(kNormalizeByLane);
  }
  return ResetPath(base, path);
}

uint32_t ResetFor(uint8_t* base, uint32_t path, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213E658 AiPath::ResetFor");
  AiPath(base, path).set_driver(driver);
  StoreConstant(base, AiPath(base, path).largest_step_address(), kZero);
  AiPath(base, path).set_normalizer(kNormalizeByLane);
  AiPath(base, path).set_branch_kind(0xFFFFFFFFu);
  AiPath(base, path).set_mode(1);
  AiPath(base, path).set_leader_branch_kind(0xFFFFFFFFu);
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
  const uint32_t table = TrackRouteFile(base, file).sections();
  const int32_t slice = LoadS16(base, position + 2);
  const uint32_t record = Load(base, table + section * 4) + uint32_t(slice) * 84 + 124;
  AiPathEntry(base, entry).set_slice_record(record);
  AiPathEntry(base, entry).set_section(guest::LoadU16(base, position + 0));
  AiPathEntry(base, entry).set_slice(guest::LoadU16(base, position + 2));
  AiPathEntry(base, entry).set_lane(uint32_t(LoadS16(base, lane)));
  SetLaneOffset(base, entry);
  return 1;
}

void UpdateLane(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82144EF8 AiDriver::UpdateLane");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(AiDriver(base, driver).vehicle().address);
  const uint32_t lane = track_route::LaneAt(base, track_route::kRoute,
                                            TrackPosition(base, identity).point_x_address(),
                                            TrackPosition(base, identity).route_position_address());
  track_route::ClampPosition(base, frame + 80, TrackPosition(base, identity).route_position());
  const uint32_t clamped = Load(base, frame + 80);
  AiDriver(base, driver).set_lane(uint16_t(lane));
  AiDriver(base, driver).set_route_position(clamped);
}

// --- The other racers' records and the states ---

uint64_t ResetOthers(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82145A48 AiDriver::ResetOthers");
  ClearOthers(base, driver);
  const double minus_one = Lfs(base, G::minus_one_address());
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  const double gap = Lfs(base, G::other_gap_address());
  uint64_t r3 = driver;
  for (int32_t i = 0; i < 7; i++) {
    if (i == int32_t(AiDriver(base, driver).index())) {
      continue;
    }
    r3 = OfIndex(base, uint32_t(i));
    const uint32_t record = uint32_t(r3) + AiDriver(base, driver).index() * 40 + 172;
    ResetOtherRecord(base, record, gap, zero, one, minus_one);
  }
  return r3;
}

void ClearCarState(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146588 AiDriver::ClearCarState");
  const double zero = Lfs(base, kZero);
  Stfs(base, AiDriver(base, driver).point_x_address(), zero);
  Stfs(base, AiDriver(base, driver).point_y_address(), zero);
  Stfs(base, AiDriver(base, driver).point_z_address(), zero);
  CopyWord(base, kOne, AiDriver(base, driver).point_w_address());
  AiDriver(base, driver).set_watched(0);
  for (uint32_t at = 96; at <= 104; at += 4) {
    Stfs(base, driver + at, zero);
  }
  const double far = Lfs(base, G::five_hundred_address());
  for (uint32_t at = 108; at <= 156; at += 4) {
    Stfs(base, driver + at, zero);
  }
  Stfs(base, AiDriver(base, driver).speed_limit_address(), far);
  Stfs(base, AiDriver(base, driver).speed_limit_other_address(), far);
  ClearOthers(base, driver);
}

void UpdateState4(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82146220 AiDriver::UpdateState4");
  PickTarget(base, driver);
  const uint32_t target = AiDriver(base, driver).watched();
  if (int32_t(target) < 0) {
    return;
  }
  const double gap = Lfs(base, driver + target * 40 + 172);
  if (gap < Lfs(base, kZero)) {
    if (gap < Lfs(base, 0x82008D40)) {
      return;
    }
    StoreConstant(base, speed, G::twenty_address());
  } else {
    if (gap > Lfs(base, 0x8200257C)) {
      return;
    }
    StoreConstant(base, speed, G::five_hundred_address());
  }
}

uint64_t UpdateState5(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82146388 AiDriver::UpdateState5");
  PickTargetBehind(base, driver);
  const uint32_t target = AiDriver(base, driver).watched();
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
  if (AiDriver(base, driver).state() == 0) {
    return 1;
  }
  AiDriver(base, driver).set_watched(0xFFFFFFFFu);
  PickTarget(base, driver);
  const uint32_t target = AiDriver(base, driver).watched();
  if (int32_t(target) < 0) {
    return 1;
  }
  const int32_t state = int32_t(AiDriver(base, driver).state());
  const uint32_t record = driver + target * 40 + 172;
  uint32_t above, below;  // the gap's bounds (floats)
  if (state == 2) {
    above = G::twenty_address();
    below = 0x82008D44;  // -20.0
  } else if (state == 3) {
    above = 0x820027A8;
    below = 0x82008E5C;
  } else {
    return 1;
  }
  const double gap = Lfs(base, AiOther(base, record).gap_address());
  if (gap > Lfs(base, above)) {
    return 0;
  }
  return gap < Lfs(base, below) ? 0 : 1;
}

void MirrorOther(uint8_t* base, uint32_t out, uint32_t record, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82145610 AiOther::Mirror");
  Stfs(base, AiOther(base, out).gap_address(),
       Fneg(Lfs(base, AiOther(base, record).gap_address())));
  Stfs(base, AiOther(base, out).closing_rate_address(),
       Fneg(Lfs(base, AiOther(base, record).closing_rate_address())));
  Stfs(base, AiOther(base, out).last_change_address(),
       Fneg(Lfs(base, AiOther(base, record).last_change_address())));
  CopyWord(base, AiOther(base, record).ticks_to_meet_address(),
           AiOther(base, out).ticks_to_meet_address());
  AiOther(base, out).set_flag_a(AiOther(base, record).flag_a());
  AiOther(base, out).set_flag_b(AiOther(base, record).flag_b());
  uint32_t state = AiOther(base, record).state();
  if (int16_t(state) == 1) {
    state = 3;
  } else if (int16_t(state) == 3) {
    state = 1;
  }
  AiOther(base, out).set_state(uint16_t(state));
  UpdateOtherFlags(base, out);
  CopyWord(base, value, AiOther(base, out).speed_address());
}

// --- The path's entries and corners ---

void BlockLeft(uint8_t* base, uint32_t entry, uint32_t at, uint32_t width, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213C5C8 AiPathEntry::BlockLeft");
  Stfs(base, at, Fsubs(Lfs(base, at), Lfs(base, AiPathEntry(base, entry).step_address())));
  double edge = Fadds(Lfs(base, out), Lfs(base, AiPathEntry(base, entry).step_address()));
  Stfs(base, out, edge);
  edge = Fadds(Lfs(base, width), edge);
  Stfs(base, out, edge);
  const double from = Lfs(base, at);
  if (from < edge) {
    Stfs(base, out, from);
  }
  const double block = Lfs(base, out);
  if (!(Lfs(base, AiPathEntry(base, entry).room_low_address()) < block)) {
    return;
  }
  const double most = Lfs(base, AiPathEntry(base, entry).room_high_address());
  Stfs(base, AiPathEntry(base, entry).room_low_address(), block);
  if (block > most) {
    Stfs(base, AiPathEntry(base, entry).room_low_address(), most);
  }
}

void BlockRight(uint8_t* base, uint32_t entry, uint32_t at, uint32_t width, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8213C630 AiPathEntry::BlockRight");
  Stfs(base, at, Fsubs(Lfs(base, at), Lfs(base, AiPathEntry(base, entry).step_address())));
  double edge = Fadds(Lfs(base, out), Lfs(base, AiPathEntry(base, entry).step_address()));
  Stfs(base, out, edge);
  edge = Fadds(Lfs(base, width), edge);
  Stfs(base, out, edge);
  const double from = Lfs(base, at);
  if (from > edge) {
    Stfs(base, out, from);
  }
  const double block = Lfs(base, out);
  if (!(Lfs(base, AiPathEntry(base, entry).room_high_address()) > block)) {
    return;
  }
  const double least = Lfs(base, AiPathEntry(base, entry).room_low_address());
  Stfs(base, AiPathEntry(base, entry).room_high_address(), block);
  if (block < least) {
    Stfs(base, AiPathEntry(base, entry).room_high_address(), least);
  }
}

void InitCorner(uint8_t* base, uint32_t record, uint32_t data, uint32_t corner_entry,
                uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213C698 AiCorner::Init");
  AiCorner(base, record).set_data(data);
  AiCorner(base, record).set_table(data + 56);
  AiCorner(base, record).set_number(guest::LoadU16(base, corner_entry + 0));
  const uint32_t offset =
      guest::LoadU16(base, count) - guest::LoadU16(base, corner_entry + 2);
  AiCorner(base, record).set_entry_offset(uint16_t(offset));
  const uint32_t first = RouteCorner(base, data).entry_count();
  AiCorner(base, record).set_position(0);
  AiCorner(base, record).set_end(uint16_t(first + uint32_t(int32_t(int16_t(offset)))));
  const bool negative = Lfs(base, RouteCorner(base, data).direction_address()) < Lfs(base, kZero);
  StoreConstant(base, AiCorner(base, record).direction_address(),
                negative ? G::minus_one_address() : kOne);
  AiCorner(base, record).set_kind(0);
}

uint32_t WithinRoadAt(uint8_t* base, uint32_t path, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_8213C1A0 AiPath::WithinRoadAt");
  const uint32_t driver = AiPath(base, path).driver().address;
  const int32_t count = LoadS16(base, AiPath(base, path).entry_count_address());
  const uint32_t file = RG::route_file(base).address;
  const double reach = Lfs(base, AiDriver(base, driver).car_radius_address());
  const double length = Lfs(base, TrackRouteFile(base, file).slice_length_address());
  const int32_t k =
      int32_t(uint32_t(strings_and_font::steps::Fctiwz(Fdivs(reach, length))) + 1);
  if (!(k < count)) {
    return 0;
  }
  const int32_t start = LoadS16(base, AiPath(base, path).first_address());
  const double at = Lfs(base, offset);
  const uint32_t entry = RingEntry(path, int32_t(uint32_t(start) + uint32_t(k))) + 16;
  const uint32_t record = AiPathEntry(base, entry).slice_record().address;
  const double half =
      Fmuls(Lfs(base, RouteSlice(base, record).width_address()), Lfs(base, G::half_address()));
  if (at > Fadds(Lfs(base, RouteSlice(base, record).margin_right_address()), half)) {
    return 0;
  }
  const double left = Fsubs(Fneg(half), Lfs(base, RouteSlice(base, record).margin_left_address()));
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
  const uint32_t flags = AiDriver(base, driver).flags();
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  guest::StoreU32(base, frame + 84, 1);
  AiDriver(base, driver).set_flags(ClearBits0And2(flags));
  if (vehicle != 0) {
    SetMode(base, driver + 512, frame + 84);
  }
  AiDriver(base, driver).set_skill_flag(0);
  ResetSkill(base, driver + 6048);
  const uint32_t again = AiDriver(base, driver).vehicle().address;
  StoreConstant(base, AiDriver(base, driver).cruise_speed_address(), 0x82008948);
  if (again != 0) {
    StoreConstant(base, Vehicle(base, again).brake_efficiency_address(), kOne);
  }
}

void ResetToMode0(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821394F0 AiDriver::ResetToMode0");
  guest::StoreU32(base, frame + 80, 0);
  SetFlag2(base, driver, frame + 80);
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  guest::StoreU32(base, frame + 88, 0);
  AiDriver(base, driver).set_flags(ClearBits0And2(AiDriver(base, driver).flags()));
  if (vehicle != 0) {
    SetMode(base, driver + 512, frame + 88);
  }
  ResetSkill(base, driver + 6048);
  AiDriver(base, driver).set_skill_flag(0);
  const int32_t skill = LoadS16(base, AiDriver(base, driver).skill_speed_address());
  Stfs(base, AiDriver(base, driver).cruise_speed_address(), double(float(double(int64_t(skill)))));
}

uint32_t StartRace(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139DA0 AiDriver::StartRace");
  const uint32_t flags = AiDriver(base, driver).flags();
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  AiDriver(base, driver).set_flags(flags & ~1u);
  // The vehicle's flags read whatever the vehicle (+6148 of 0 without one),
  // as the original does.
  const uint32_t free = (Vehicle(base, vehicle).car_flags() & 129) == 0 ? 1 : 0;
  guest::StoreU32(base, frame + 80, free);
  SetFlag2(base, driver, frame + 80);
  const uint32_t set = AiDriver(base, driver).flags() | 4;
  guest::StoreU32(base, frame + 84, 0);
  const uint32_t again = AiDriver(base, driver).vehicle().address;
  AiDriver(base, driver).set_flags(set);
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
  uint32_t table = Load(base, G::drivers_address());
  const double seventy = Lfs(base, 0x82008930);
  double lowest = seventy;
  int32_t settled = 0;
  if (table != 0) {
    const uint32_t copy = frame + 80;
    for (int32_t i = 0; i < count; i++) {
      const uint32_t racer = Load(base, G::race(base).racers_address() + 4 * uint32_t(i));
      if ((MinimapRacerEntry(base, racer).flags() & 0x10) == 0) {
        continue;
      }
      const int32_t index = int32_t(MinimapRacerEntry(base, racer).vehicle_index());
      if (index < 0) {
        continue;
      }
      // memcpy (sub_825A7410) copies natively with nothing for its override
      // to record (a destination outside the physical views, which it
      // records for the renderer); else the original runs.
      if (copy >= 0xA0000000u ||
          !c_runtime::Memcpy(base, copy, uint32_t(index) * AiDriver::kSize + table,
                             AiDriver::kSize)) {
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
      table = Load(base, G::drivers_address());
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
  const double angle = Lfs(base, AiDriver(base, driver).target_angle_address());
  const double sum = Fadds(Lfs(base, AiDriver(base, driver).slide_angle_address()), angle);
  double steer = Lfs(base, AiDriver(base, driver).steer_shift_address());
  const double crossed = Fmuls(angle, steer);
  const double total = Fadds(sum, Lfs(base, AiDriver(base, driver).steer_request_address()));
  const double zero = Lfs(base, kZero);
  Stfs(base, AiDriver(base, driver).steer_request_address(), total);
  if (!(crossed < zero)) {
    return;
  }
  steer = Fmuls(steer, Lfs(base, G::fifth_address()));
  if (steer < zero) {
    steer = Fneg(steer);
  }
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  if (speed < Lfs(base, G::other_gap_address())) {
    steer = Fmuls(Fmuls(speed, Lfs(base, 0x82002A08)), steer);
  }
  steer = Fsubs(steer, Lfs(base, kOne));
  if (!(steer > zero)) {
    return;
  }
  const double most = Lfs(base, 0x820021E4);
  Stfs(base, AiDriver(base, driver).steer_request_address(),
       steer < most ? Fmadds(angle, steer, total) : Fmadds(angle, most, total));
}

void ThrottleForSpeed(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139168 AiDriver::ThrottleForSpeed");
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  if (!(speed > Lfs(base, G::twenty_address()))) {
    return;
  }
  const double over = Fsubs(speed, Lfs(base, AiDriver(base, driver).target_speed_address()));
  const double ten = Lfs(base, G::ten_address());
  const double zero = Lfs(base, kZero);
  if (over > ten) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
    StoreConstant(base, AiDriver(base, driver).brake_request_address(), 0x820029F0);
  } else if (over > zero) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
    Stfs(base, AiDriver(base, driver).brake_request_address(), zero);
  } else if (over > Lfs(base, 0x82002824)) {
    const double eased = Fmuls(Fneg(over), Lfs(base, G::fifth_address()));
    if (eased < Lfs(base, AiDriver(base, driver).throttle_request_address())) {
      Stfs(base, AiDriver(base, driver).throttle_request_address(), eased);
    }
    Stfs(base, AiDriver(base, driver).brake_request_address(), zero);
  }
  if (Lfs(base, AiDriver(base, driver).speed_kmh_address()) >
      Lfs(base, AiDriver(base, driver).speed_limit_other_address())) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
  }
  if ((AiDriver(base, driver).flags() & 4) != 0) {
    return;
  }
  const double angle = Lfs(base, AiDriver(base, driver).target_angle_address());
  if (angle > Lfs(base, 0x82001FB4) || angle < Lfs(base, 0x82002A50)) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
  }
}

void EaseThrottleForSlide(uint8_t* base, uint32_t driver, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_82139BE8 AiDriver::EaseThrottleForSlide");
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  if (speed < Lfs(base, G::twenty_address())) {
    return;
  }
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
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
  const double factor = Fmuls(Lfs(base, AiDriver(base, driver).steer_factor_address()),
                              Lfs(base, AiDriver(base, driver).corner_grip_address()));
  const double slide = Fabs(Lfs(base, AiDriver(base, driver).slide_angle_address()));
  double ease = Fmsubs(slide, speed, Fmuls(factor, f13));
  const double zero = Lfs(base, kZero);
  if (!(ease > zero)) {
    return;
  }
  ease = Fmuls(ease, Lfs(base, Load(base, flag) != 0 ? G::fifth_address() : 0x82002204));
  if (!(ease > zero)) {
    return;
  }
  const double throttle = Lfs(base, AiDriver(base, driver).throttle_request_address());
  Stfs(base, AiDriver(base, driver).throttle_request_address(),
       throttle < ease ? zero : Fsubs(throttle, ease));
}

void LimitThrottle(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82139CB0 AiDriver::LimitThrottle");
  int32_t skill = LoadS16(base, AiDriver(base, driver).skill_throttle_cap_address());
  if (skill < 0) {
    skill = 0;
  } else if (skill > 100) {
    skill = 100;
  }
  const double most = Fmuls(double(float(double(int64_t(skill)))), Lfs(base, 0x8200222C));
  const double zero = Lfs(base, kZero);
  const double throttle = Lfs(base, AiDriver(base, driver).throttle_address());
  if (throttle < zero) {
    Stfs(base, AiDriver(base, driver).throttle_address(), zero);
  } else if (throttle > most) {
    Stfs(base, AiDriver(base, driver).throttle_address(), most);
  }
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  const double limit = Lfs(base, vehicle + 4660);
  const double rpm = Lfs(base, Vehicle(base, vehicle).rpm_address());
  if (rpm < Fmuls(limit, Lfs(base, 0x82002A34))) {
    return;
  }
  if (rpm > Fmuls(limit, Lfs(base, 0x82002B90))) {
    Stfs(base, AiDriver(base, driver).throttle_address(), zero);
    return;
  }
  const bool high = rpm > Fmuls(limit, Lfs(base, 0x82008958));
  const double now = Lfs(base, AiDriver(base, driver).throttle_address());
  if (now < zero) {
    Stfs(base, AiDriver(base, driver).throttle_address(), zero);
    return;
  }
  const double cap = Lfs(base, high ? 0x8200296C : G::half_address());
  if (now > cap) {
    Stfs(base, AiDriver(base, driver).throttle_address(), cap);
  }
}

void AdjustForOther(uint8_t* base, uint32_t driver, uint32_t record, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821452A0 AiDriver::AdjustForOther");
  const double gap = Lfs(base, AiOther(base, record).gap_address());
  const double zero = Lfs(base, kZero);
  if (!(gap < zero)) {
    return;
  }
  const int32_t index = LoadS16(base, AiOther(base, record).ring_index_address());
  if (index < 0) {
    return;
  }
  if (AiOther(base, record).state() != 1) {
    return;
  }
  if (!(Lfs(base, AiOther(base, record).closing_rate_address()) > zero)) {
    return;
  }
  if ((AiOther(base, record).action_flags() & 4) != 0) {
    return;
  }
  double distance = Fneg(gap);
  if (Load(base, mode) != 0 && index > 1) {
    const double length = Lfs(base, RG::route_file(base).slice_length_address());
    const double slices = double(float(double(int64_t(index - 1))));
    distance = Fmadds(slices, length, distance);
  }
  if (distance < zero) {
    distance = zero;
  }
  double scale = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  const double factor = Lfs(base, AiDriver(base, driver).distance_factor_address());
  const double hundred = Lfs(base, G::hundred_address());
  scale = scale > hundred ? Fdivs(hundred, scale) : Lfs(base, kOne);
  const double limit =
      Fmadds(Fmuls(scale, factor), distance, Lfs(base, AiOther(base, record).speed_address()));
  if (limit < Lfs(base, AiDriver(base, driver).speed_limit_other_address())) {
    Stfs(base, AiDriver(base, driver).speed_limit_other_address(), limit);
  }
  if (Lfs(base, AiOther(base, record).gap_address()) < Lfs(base, 0x82008E48)) {
    return;
  }
  if (limit < Lfs(base, AiDriver(base, driver).speed_limit_address())) {
    Stfs(base, AiDriver(base, driver).speed_limit_address(), limit);
  }
}

uint64_t UpdateAgainstPlayer(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_82146608 AiDriver::UpdateAgainstPlayer");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint64_t r3 = uint32_t(count);
  StoreConstant(base, AiDriver(base, driver).player_gap_address(), kZero);
  AiDriver(base, driver).set_passes_a(0);
  AiDriver(base, driver).set_passes_b(0);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(base, G::race(base).racers_address() + 4 * uint32_t(i));
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0 || AiDriver(base, driver).index() == index) {
      continue;
    }
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
    r3 = vehicle;
    if ((Vehicle(base, vehicle).car_flags() & 129) == 0) {
      continue;
    }
    const uint32_t record = driver + index * 40 + 172;
    if (AiOther(base, record).flag_a() == 1) {
      if ((AiOther(base, record).history_flags() & 2) != 0) {
        AiDriver(base, driver).set_passes_a(uint16_t(AiDriver(base, driver).passes_a() + 1));
      }
      if ((AiOther(base, record).history_flags() & 4) != 0) {
        AiDriver(base, driver).set_passes_b(uint16_t(AiDriver(base, driver).passes_b() + 1));
      }
    }
    CopyWord(base, AiOther(base, record).gap_address(),
             AiDriver(base, driver).player_gap_address());
    break;
  }
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  bool stuck_check = speed < Lfs(base, 0x82008034);
  if (stuck_check) {
    r3 = IsFree(base, driver);
    stuck_check = r3 != 0;
  }
  if (!stuck_check) {
    AiDriver(base, driver).set_stuck_count(0);
    AiDriver(base, driver).set_flags(AiDriver(base, driver).flags() & ~0x20u);
    return r3;
  }
  const uint32_t flags = AiDriver(base, driver).flags();
  const uint32_t count492 = AiDriver(base, driver).stuck_count();
  if ((flags & 0x20) == 0) {
    const double held = double(float(double(int64_t(int16_t(count492)))));
    if (!(held < Lfs(base, 0x82008238))) {
      AiDriver(base, driver).set_flags(flags | 32);
      return r3;
    }
    const double fifteen = Lfs(base, 0x820029D0);
    if (!(speed < fifteen)) {
      return r3;
    }
    const uint32_t step = uint32_t(strings_and_font::steps::Fctiwz(Fsubs(speed, fifteen)));
    AiDriver(base, driver).set_stuck_count(uint16_t(count492 - (step & 0xFFFF)));
    return r3;
  }
  const uint32_t step = uint32_t(strings_and_font::steps::Fctiwz(speed));
  const int32_t left = int16_t(uint16_t(count492 - step));
  AiDriver(base, driver).set_stuck_count(uint16_t(left));
  if (left > 0) {
    return r3;
  }
  AiDriver(base, driver).set_stuck_count(0);
  AiDriver(base, driver).set_flags(flags & ~0x20u);
  return r3;
}

void UpdateOther(uint8_t* base, uint32_t record, uint32_t gap, uint32_t length) {
  ITC_NATIVE_RUNNING("sub_821454C8 AiOther::Update");
  const double old = Lfs(base, AiOther(base, record).gap_address());
  if (!(old < Lfs(base, 0x82008E38)) && !(old > Lfs(base, G::other_gap_address()))) {
    const double change = Fsubs(Lfs(base, gap), old);
    const double last = Lfs(base, AiOther(base, record).last_change_address());
    Stfs(base, AiOther(base, record).last_change_address(), change);
    Stfs(base, AiOther(base, record).closing_rate_address(),
         Fmuls(Fadds(last, change), Lfs(base, G::half_address())));
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
      AiOther(base, record).set_state(2);
      Stfs(base, AiOther(base, record).ticks_to_meet_address(), zero);
    } else {
      const double rate = Lfs(base, AiOther(base, record).closing_rate_address());
      double meet;
      if (Fmuls(rate, apart) < zero) {
        const int32_t ticks = strings_and_font::steps::Fctiwz(Fneg(Fdivs(apart, rate)));
        meet = ticks < 10000 ? Fmuls(double(float(double(int64_t(ticks)))), Lfs(base, 0x82002A3C))
                             : Lfs(base, G::minus_one_address());
      } else {
        meet = Lfs(base, G::minus_one_address());
      }
      Stfs(base, AiOther(base, record).ticks_to_meet_address(), meet);
      AiOther(base, record).set_state(Lfs(base, gap) > zero ? 3 : 1);
    }
  } else {
    const double zero = Lfs(base, kZero);
    Stfs(base, AiOther(base, record).closing_rate_address(), zero);
    AiOther(base, record).set_state(0);
    Stfs(base, AiOther(base, record).last_change_address(), zero);
    StoreConstant(base, AiOther(base, record).ticks_to_meet_address(), G::minus_one_address());
  }
  CopyWord(base, gap, AiOther(base, record).gap_address());
  UpdateOtherFlags(base, record);
}

// --- Resets, steering input and the path's lists ---

void ResetControl(uint8_t* base, uint32_t driver, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139580 AiDriver::ResetControl");
  guest::StoreU32(base, frame + 80, 1);
  AiDriver(base, driver).set_flags(AiDriver(base, driver).flags() & ~1u);
  SetFlag2(base, driver, frame + 80);
  ResetToMode1(base, driver, frame - 112);
}

uint64_t ResetDriver(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_8213AAA8 AiDriver::Reset");
  ResetFollow(base, driver);
  ResetFor(base, driver + 512, driver);
  const double zero = Lfs(base, kZero);
  Stfs(base, AiDriver(base, driver).target_speed_address(), zero);
  Stfs(base, AiDriver(base, driver).target_x_address(), zero);
  Stfs(base, AiDriver(base, driver).target_y_address(), zero);
  const double one = Lfs(base, kOne);
  Stfs(base, AiDriver(base, driver).target_z_address(), zero);
  Stfs(base, AiDriver(base, driver).target_w_address(), one);
  AiDriver(base, driver).set_handbrake(0);
  Stfs(base, AiDriver(base, driver).slide_angle_address(), zero);
  AiDriver(base, driver).set_update(0x8213A1B8);  // AiDriver::UpdateStarting
  for (uint32_t at : {6116u, 6120u, 6124u, 6128u, 6136u, 6140u, 6144u}) {
    Stfs(base, driver + at, zero);
  }
  return ResetOthers(base, driver);
}

uint64_t UpdateSteerInput(uint8_t* base, uint32_t driver, uint32_t stack, double* f1,
                          bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AC70 AiDriver::UpdateSteerInput");
  const int32_t count = LoadS16(base, AiDriver(base, driver).path_corner_count_address());
  for (int32_t i = 0; i < count; i++) {
    const uint32_t at = driver + 5676 + 44 * uint32_t(i);
    if (AiCorner(base, at).data().address == 0) {
      continue;
    }
    const int32_t kind = int32_t(AiCorner(base, at).kind());
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
  const int32_t count = LoadS16(base, AiPath(base, path).section_count_address());
  const uint32_t driver = AiPath(base, path).driver().address;
  if (count <= 0) {
    return;
  }
  const uint32_t list = path + 5146;
  const int32_t current = LoadS16(base, AiDriver(base, driver).route_section_address());
  if (LoadS16(base, list) == current) {
    return;
  }
  int32_t found = -1;
  for (int32_t i = 1; count > 1 && i < LoadS16(base, AiPath(base, path).section_count_address());
       i++) {
    if (LoadS16(base, list + 2 * uint32_t(i)) == current) {
      found = i;
      break;
    }
  }
  if (found < 0) {
    AiPath(base, path).set_section_count(0);
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
    } while (j < LoadS16(base, AiPath(base, path).section_count_address()) - found);
  }
  for (int32_t i = moved; i < 8; i++) {
    guest::StoreU16(base, list + 2 * uint32_t(i), 0xFFFF);
  }
  AiPath(base, path).set_section_count(uint16_t(AiPath(base, path).section_count() - found));
}

uint32_t SharesSections(uint8_t* base, uint32_t path, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_8213C010 AiPath::SharesSections");
  const int32_t n = LoadS16(base, AiPath(base, path).section_count_address());
  if (n <= 0) {
    return 0;
  }
  const int32_t m = LoadS16(base, RouteCorner(base, record).section_count_address());
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
  Stfs(base, AiPathEntry(base, entry).room_low_address(),
       Fadds(Lfs(base, AiPathEntry(base, entry).range_low_address()), Lfs(base, margin)));
  const double m = Lfs(base, margin);
  const double high = Fsubs(Lfs(base, AiPathEntry(base, entry).range_high_address()), m);
  Stfs(base, AiPathEntry(base, entry).room_high_address(), high);
  const double by = Lfs(base, shift);
  if (by > Lfs(base, kZero)) {
    Stfs(base, AiPathEntry(base, entry).room_low_address(),
         Fadds(Lfs(base, AiPathEntry(base, entry).room_low_address()), by));
  } else {
    Stfs(base, AiPathEntry(base, entry).room_high_address(), Fadds(by, high));
  }
  const double lo = Lfs(base, AiPathEntry(base, entry).room_low_address());
  const double hi = Lfs(base, AiPathEntry(base, entry).room_high_address());
  if (!(lo > hi)) {
    return;
  }
  const double middle = Fmuls(Fadds(hi, lo), Lfs(base, G::half_address()));
  Stfs(base, AiPathEntry(base, entry).room_high_address(), middle);
  Stfs(base, AiPathEntry(base, entry).room_low_address(), middle);
}

void NarrowToOther(uint8_t* base, uint32_t entry, uint32_t other, uint32_t margin,
                   uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_8213C420 AiPathEntry::NarrowToOther");
  if (other == 0) {
    return;
  }
  const uint32_t record = AiPathEntry(base, other).slice_record().address;
  if (record == 0) {
    return;
  }
  const double m = Lfs(base, margin);
  double lo = Fsubs(Lfs(base, AiPathEntry(base, other).range_low_address()), m);
  double hi = Fadds(Lfs(base, AiPathEntry(base, other).range_high_address()), m);
  const double least = Lfs(base, RouteSlice(base, record).route_bit_a_address());
  if (lo < least) {
    lo = least;
  }
  const double most = Lfs(base, RouteSlice(base, record).route_bit_b_address());
  if (hi > most) {
    hi = most;
  }
  const double by = Lfs(base, offset);
  double a = Fadds(by, lo);
  double b = Fadds(by, hi);
  if (a > b) {
    b = Fmuls(Fadds(b, a), Lfs(base, G::half_address()));
    a = b;
  }
  if (Lfs(base, AiPathEntry(base, entry).range_low_address()) < a) {
    Stfs(base, AiPathEntry(base, entry).range_low_address(), a);
    Stfs(base, AiPathEntry(base, entry).room_low_address(), a);
  }
  if (Lfs(base, AiPathEntry(base, entry).range_high_address()) > b) {
    Stfs(base, AiPathEntry(base, entry).range_high_address(), b);
    Stfs(base, AiPathEntry(base, entry).room_high_address(), b);
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
  const int32_t last = LoadS16(base, AiPath(base, path).corner_count_address()) - 1;
  if (last < 0) {
    return true;
  }
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  for (int32_t k = last; k >= 0; k--) {
    const uint32_t record = path + 5164 + 44 * uint32_t(k);
    AiCorner(base, record)
        .set_end(uint16_t(AiCorner(base, record).end() - guest::LoadU16(base, slices)));
    const uint32_t step = guest::LoadU16(base, slices);
    const int32_t left = LoadS16(base, AiCorner(base, record).end_address());
    AiCorner(base, record).set_entry_offset(uint16_t(AiCorner(base, record).entry_offset() - step));
    if (left > 0) {
      continue;
    }
    int32_t j = k;
    if (k < LoadS16(base, AiPath(base, path).corner_count_address()) - 1) {
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
      } while (j < LoadS16(base, AiPath(base, path).corner_count_address()) - 1);
    }
    const uint32_t cleared = path + 5164 + 44 * uint32_t(j);
    Stfs(base, AiCorner(base, cleared).direction_address(), one);
    AiCorner(base, cleared).set_data(0);
    Stfs(base, AiCorner(base, cleared).speed_address(), zero);
    AiCorner(base, cleared).set_table(0);
    Stfs(base, AiCorner(base, cleared).slope_before_address(), zero);
    AiCorner(base, cleared).set_number(0);
    Stfs(base, AiCorner(base, cleared).slope_after_address(), zero);
    AiCorner(base, cleared).set_entry_offset(0);
    AiCorner(base, cleared).set_end(0);
    AiCorner(base, cleared).set_position(0);
    AiCorner(base, cleared).set_unknown_24(0);
    AiCorner(base, cleared).set_joined_before(0);
    AiCorner(base, cleared).set_joined_after(0);
    AiCorner(base, cleared).set_kind(0);
    AiPath(base, path).set_corner_count(uint16_t(AiPath(base, path).corner_count() - 1));
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
  const uint32_t old = AiDriver(base, driver).state();
  AiDriver(base, driver).set_state(state);
  AiDriver(base, driver).set_previous_state(old);
}
}  // namespace

uint32_t Restart(uint8_t* base, uint32_t driver, uint32_t mask, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821395D0 AiDriver::Restart");
  ResetControl(base, driver, frame - 112);
  const uint32_t r3 = ResetPath(base, driver + 512);
  const uint32_t old = AiDriver(base, driver).state();
  AiDriver(base, driver).set_previous_state(old);
  AiDriver(base, driver).set_state(5);
  AiDriver(base, driver).set_racer_mask(uint16_t(Load(base, mask)));
  return r3;
}

uint32_t ResetAll(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139750 AiDrivers::ResetAll");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint32_t r3 = uint32_t(count);
  uint32_t mask = 0;
  guest::StoreU32(base, frame + 80, mask);
  uint32_t table = Load(base, G::drivers_address());
  if (table == 0) {
    return r3;
  }
  uint32_t race = Load(base, G::race_address());
  if (count <= 0) {
    return r3;
  }
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(base, RaceManager(base, race).racers_address() + 4 * uint32_t(i));
    if ((MinimapRacerEntry(base, racer).flags() & 0x10) != 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    mask |= Slw(1, index);
  }
  guest::StoreU32(base, frame + 80, mask);
  for (int32_t i = 0; i < count; i++) {
    const uint32_t racer = Load(base, RaceManager(base, race).racers_address() + 4 * uint32_t(i));
    if ((MinimapRacerEntry(base, racer).flags() & 0x10) != 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    r3 = Restart(base, index * AiDriver::kSize + table, frame + 80, frame - 112);
    table = Load(base, G::drivers_address());
    race = Load(base, G::race_address());
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
  AiDriver(base, driver).set_racer_mask(uint16_t(Load(base, word)));
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
    const uint32_t old = AiDriver(base, driver).state();
    AiDriver(base, driver).set_previous_state(old);
    AiDriver(base, driver).set_state(3);
    AiDriver(base, driver).set_racer_mask(uint16_t(Load(base, word)));
    return reset;
  }
  int32_t k = 0;
  const int32_t me = int32_t(AiDriver(base, driver).index());
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
  const uint32_t old = AiDriver(base, driver).state();
  AiDriver(base, driver).set_racer_mask(uint16_t(mask));
  AiDriver(base, driver).set_state(state);
  AiDriver(base, driver).set_previous_state(old);
  return reset;
}

uint32_t SetState4(uint8_t* base, uint32_t driver, uint32_t word, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82139FA0 AiDriver::SetState4");
  ResetControl(base, driver, frame - 112);
  ClearPlan(base, driver);
  const uint32_t r3 = ResetPath(base, driver + 512);
  const uint32_t old = AiDriver(base, driver).state();
  AiDriver(base, driver).set_previous_state(old);
  AiDriver(base, driver).set_state(4);
  AiDriver(base, driver).set_racer_mask(uint16_t(Load(base, word)));
  return r3;
}

uint32_t ClearAll(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A140 AiDrivers::ClearAll");
  const int32_t count = int32_t(track_collision::RaceRacerCount(base));
  uint32_t r3 = uint32_t(count);
  if (Load(base, G::drivers_address()) == 0 || count <= 0) {
    return r3;
  }
  for (int32_t i = 0; i < count; i++) {
    const uint32_t table = Load(base, G::drivers_address());
    const uint32_t racer = Load(base, G::race(base).racers_address() + 4 * uint32_t(i));
    const uint32_t driver =
        MinimapRacerEntry(base, racer).vehicle_index() * AiDriver::kSize + table;
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
  const uint32_t data_b = AiCorner(base, b).data().address;
  uint32_t shift = 0;
  const uint32_t data_a = AiCorner(base, a).data().address;
  const uint32_t ends = data_b + 48;
  const int32_t have = int32_t(Load(base, count));
  const double f0 = Lfs(base, RouteCorner(base, data_b).length_address());
  double f13 = Fadds(Lfs(base, RouteCorner(base, data_a).speed_b_address()), f0);
  const int32_t start = LoadS16(base, RouteCorner(base, data_b).entry_count_address());
  const int32_t gap = LoadS16(base, ends + 6) - start;
  if (have < gap) {
    shift = uint32_t(gap - have);
    guest::StoreU32(base, count, uint32_t(gap));
  }
  const int32_t exit = LoadS16(base, RouteCorner(base, data_a).turn_in_address());
  const double f12a = Lfs(base, RouteCorner(base, data_b).length_address());
  const uint32_t now = Load(base, count);
  const uint32_t table_a = AiCorner(base, a).table().address;
  const uint32_t r3 = AiCorner(base, b).table().address;
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
  f8 = Fmuls(f8, Lfs(base, G::half_address()));
  one = Fdivs(one, f8);
  f12 = Fmuls(Fmuls(f10, one), f12);
  Stfs(base, AiCorner(base, b).slope_after_address(), f12);
  double f13b = Fmuls(Lfs(base, RouteCorner(base, data_a).speed_b_address()), f13);
  f13b = Fmuls(f13b, f9);
  Stfs(base, AiCorner(base, a).slope_before_address(), Fmuls(f13b, one));
  AiCorner(base, b).set_joined_after(1);
  AiCorner(base, a).set_joined_before(1);
  const int32_t most = LoadS16(base, ends + 4);
  if (LoadS16(base, AiCorner(base, b).position_address()) > most) {
    AiCorner(base, b).set_position(uint16_t(most));
  }
  AiCorner(base, a).set_position(uint16_t(shift));
  return r3;
}

uint32_t JoinCornersShort(uint8_t* base, uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BF28 AiPath::JoinCornersShort");
  const uint32_t data_a = AiCorner(base, a).data().address;
  const uint32_t have = Load(base, count);
  const uint32_t data_b = AiCorner(base, b).data().address;
  const uint32_t table_a = AiCorner(base, a).table().address;
  const uint32_t r3 = AiCorner(base, b).table().address;
  double f12 = Lfs(base, RouteCorner(base, data_a).speed_b_address());
  double f0 = Lfs(base, RouteCorner(base, data_b).length_address());
  double f13 = Lfs(base, RouteCornerEntry(base, table_a).offset_address());
  const uint32_t ends = data_b + 48;
  f12 = Fadds(f12, f0);
  const int32_t exit =
      int32_t(uint32_t(LoadS16(base, RouteCorner(base, data_a).turn_in_address())) + have);
  const int32_t start = LoadS16(base, RouteCorner(base, data_b).entry_count_address());
  f13 = Fsubs(f13, Lfs(base, ((uint32_t(start) << 3) & 0xFFFFFFF8u) + r3 - 4));
  const int32_t enter = LoadS16(base, ends + 4);
  AiCorner(base, b).set_joined_after(1);
  const double f10 = Fneg(f13);
  const int32_t span = int32_t(uint32_t(start) - uint32_t(enter) + have);
  const double f9 = IntToFloat(exit);
  f12 = Fdivs(Lfs(base, kOne), f12);
  f0 = Fmuls(f12, f0);
  f0 = Fmuls(f0, f13);
  f0 = Fdivs(f0, IntToFloat(span));
  Stfs(base, AiCorner(base, b).slope_after_address(), f0);
  f0 = Lfs(base, RouteCorner(base, data_a).speed_b_address());
  AiCorner(base, a).set_joined_before(1);
  f0 = Fmuls(f0, f12);
  f0 = Fmuls(f0, f10);
  Stfs(base, AiCorner(base, a).slope_before_address(), Fdivs(f0, f9));
  const int32_t most = LoadS16(base, ends + 4);
  if (LoadS16(base, AiCorner(base, b).position_address()) > most) {
    AiCorner(base, b).set_position(uint16_t(most));
  }
  AiCorner(base, a).set_position(0);
  return r3;
}

void JoinCornersByExit(uint8_t* base, uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BDF0 AiPath::JoinCornersByExit");
  const uint32_t data_b = AiCorner(base, b).data().address;
  const uint32_t data_a = AiCorner(base, a).data().address;
  const uint32_t have = Load(base, count);
  const int32_t exit = LoadS16(base, RouteCorner(base, data_a).turn_in_address());
  uint32_t at_b = uint32_t(LoadS16(base, RouteCorner(base, data_b).entry_count_address())) - 1;
  uint32_t at_a = have;
  if (int32_t(have) > exit) {
    at_a = uint32_t(exit);
    at_b = (uint32_t(exit) - have) + at_b;
  }
  const uint32_t span = uint32_t(LoadS16(base, RouteCorner(base, data_a).turn_in_address())) - have;
  const double to = Lfs(base, ((at_b << 3) & 0xFFFFFFF8u) + AiCorner(base, b).table().address + 4);
  const double from =
      Lfs(base, ((at_a << 3) & 0xFFFFFFF8u) + AiCorner(base, a).table().address + 4);
  AiCorner(base, a).set_position(uint16_t(at_a));
  const double rise = Fsubs(to, from);
  AiCorner(base, a).set_joined_before(1);
  Stfs(base, AiCorner(base, a).slope_before_address(), Fdivs(rise, IntToFloat(int32_t(span))));
}

uint32_t JoinCornersByEntry(uint8_t* base, uint32_t a, uint32_t b, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8213BE80 AiPath::JoinCornersByEntry");
  const uint32_t data_b = AiCorner(base, b).data().address;
  const uint32_t data_a = AiCorner(base, a).data().address;
  const uint32_t have = Load(base, count);
  const uint32_t ends = data_b + 48;
  uint32_t at_b = uint32_t(LoadS16(base, RouteCorner(base, data_b).entry_count_address())) - 1;
  const int32_t exit = LoadS16(base, RouteCorner(base, data_a).turn_in_address());
  uint32_t at_a = have;
  if (int32_t(have) > exit) {
    at_a = uint32_t(exit);
    at_b = (uint32_t(exit) - have) + at_b;
  }
  const uint32_t table_b = AiCorner(base, b).table().address;
  const uint32_t r3 = AiCorner(base, a).table().address;
  const double from_b = Lfs(base, ((at_b << 3) & 0xFFFFFFF8u) + table_b + 4);
  const double from_a = Lfs(base, ((at_a << 3) & 0xFFFFFFF8u) + r3 + 4);
  const double rise = Fsubs(from_a, from_b);
  const int32_t enter = LoadS16(base, ends + 4);
  AiCorner(base, b).set_joined_after(1);
  const int32_t held = LoadS16(base, AiCorner(base, b).position_address());
  Stfs(base, AiCorner(base, b).slope_after_address(),
       Fdivs(rise, IntToFloat(int32_t(at_b - uint32_t(enter)))));
  const int32_t most = LoadS16(base, ends + 4);
  if (held > most) {
    AiCorner(base, b).set_position(uint16_t(most));
  }
  AiCorner(base, a).set_position(uint16_t(at_a));
  return r3;
}

bool JoinCornerPair(uint8_t* base, uint32_t a, uint32_t b, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213DD40 AiPath::JoinCornerPair");
  const uint32_t data_b = AiCorner(base, b).data().address;
  if (data_b == 0 || AiCorner(base, b).table().address == 0 ||
      AiCorner(base, a).data().address == 0 || AiCorner(base, a).table().address == 0) {
    return false;
  }
  const int32_t gap =
      int32_t(uint32_t(LoadS16(base, AiCorner(base, a).entry_offset_address())) -
              uint32_t(LoadS16(base, AiCorner(base, b).entry_offset_address())) -
              uint32_t(LoadS16(base, RouteCorner(base, data_b).entry_count_address())));
  guest::StoreU32(base, frame + 80, uint32_t(gap));
  if (gap < 0) {
    if (!(Lfs(base, AiCorner(base, a).direction_address()) ==
          Lfs(base, AiCorner(base, b).direction_address()))) {
      *r3 = JoinCorners(base, a, b, frame + 80);
      return true;
    }
    guest::StoreU32(base, frame + 80, uint32_t(-gap));
    if (Lfs(base, RouteCorner(base, data_b).length_address()) < Lfs(base, G::hundred_address())) {
      JoinCornersByExit(base, a, b, frame + 80);  // r3 kept
      return false;
    }
    *r3 = JoinCornersByEntry(base, a, b, frame + 80);
    return true;
  }
  const double length = Lfs(base, RG::route_file(base).slice_length_address());
  if (Fmuls(length, IntToFloat(gap)) > Lfs(base, G::hundred_address())) {
    return false;
  }
  *r3 = JoinCornersShort(base, a, b, frame + 80);
  return true;
}

void AddCornersAt(uint8_t* base, uint32_t path, uint32_t entry, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213E970 AiPath::AddCornersAt");
  *r3 = path;
  const uint32_t slice = AiPathEntry(base, entry).slice_record().address;
  uint32_t listed = slice + 60;
  const int32_t n = LoadS16(base, RouteSlice(base, slice).corner_count_address());
  if (n <= 0) {
    return;
  }
  const int32_t have = LoadS16(base, AiPath(base, path).corner_count_address());
  if (have > 0) {
    const int32_t entries = LoadS16(base, AiPath(base, path).entry_count_address());
    if (LoadS16(base, uint32_t(have) * 44 + path + 5132) > entries) {  // the last's +12
      return;
    }
  }
  const uint32_t route = RG::route_file(base).address;
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
      } while (i < LoadS16(base, AiPath(base, path).corner_count_address()));
      if (listed_already) {
        continue;
      }
    }
    const uint32_t data = Load(base, (uint32_t(guest::LoadU16(base, listed + 0)) << 2) +
                                         TrackRouteFile(base, route).corners());
    if (data == 0) {
      continue;
    }
    if (!(LoadS16(base, RouteCorner(base, data).exit_address()) >
          int32_t(guest::LoadU16(base, listed + 2)))) {
      continue;
    }
    const uint32_t shares = SharesSections(base, path, data);
    *r3 = shares;
    if (int32_t(shares) != 1) {
      continue;
    }
    if (!(have < 8)) {
      return;
    }
    const uint32_t record = uint32_t(have) * 44 + path + 5164;
    InitCorner(base, record, data, listed, path + 5136);
    *r3 = record;
    const int32_t now = LoadS16(base, AiPath(base, path).corner_count_address());
    if (now > 0) {
      const uint32_t before = uint32_t(now) * 44 + path + 5120;
      uint32_t joined = path;
      JoinCornerPair(base, record, before, frame - 112, &joined);
      *r3 = joined;
    }
    AddCornersAtFinish(base, path);
    return;
  }
}

void AddCornersAtFinish(uint8_t* base, uint32_t path) {
  AiPath(base, path).set_corner_count(uint16_t(AiPath(base, path).corner_count() + 1));
}

// --- The states' speeds ---

uint64_t UpdateState2(uint8_t* base, uint32_t driver, uint32_t speed) {
  ITC_NATIVE_RUNNING("sub_82145F48 AiDriver::UpdateState2");
  PickTarget(base, driver);
  const uint32_t target = AiDriver(base, driver).watched();
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint32_t record = target * 40 + driver;
  const double gap = Lfs(base, record + 172);
  uint64_t r3 = driver;
  double f0;
  if (gap > Lfs(base, 0x82002834)) {
    if (!(gap < Lfs(base, G::hundred_address()))) {
      f0 = Lfs(base, G::twenty_address());
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
               : Lfs(base, G::five_hundred_address());
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
        f0 = Fmuls(f0, factor) < Lfs(base, G::twenty_address()) ? zero
                                                                : Lfs(base, G::twenty_address());
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
  const uint32_t target = AiDriver(base, driver).watched();
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
    } else if (again < Lfs(base, G::ten_address())) {
      f0 = Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x82002A2C));
    } else if (again < Lfs(base, G::hundred_address())) {
      f0 = Fmuls(Lfs(base, uint32_t(r3) + 100), Lfs(base, 0x82002A2C));
      const double most = Lfs(base, 0x82000F14);
      if (!(f0 < most)) {
        f0 = most;
      }
    } else {
      f0 = Lfs(base, G::twenty_address());
    }
    if (f0 < Lfs(base, speed)) {
      Stfs(base, speed, f0);
    }
    return r3;
  }
  if (gap < Lfs(base, 0x8200895C)) {
    f11 = Lfs(base, G::five_hundred_address());
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
  const uint32_t target = AiDriver(base, driver).watched();
  if (int32_t(target) < 0) {
    return driver;
  }
  const uint64_t other = OfIndex(base, target);
  const uint32_t theirs = uint32_t(other);
  if (int32_t(AiDriver(base, theirs).state()) != 6) {
    return other;
  }
  const uint32_t again = AiDriver(base, driver).watched();
  const double mine = Lfs(base, AiDriver(base, driver).car_half_length_address());
  const double their_length = Lfs(base, AiDriver(base, theirs).car_half_length_address());
  double f12 = Lfs(base, AiDriver(base, theirs).speed_kmh_address());
  const double f0 = Lfs(base, 0x820021E8);
  const double gap = Fneg(Lfs(base, again * 40 + driver + 172));
  const double closing = Fsubs(Fnmsubs(mine, f0, gap), their_length);
  if (!(closing < Lfs(base, kZero))) {
    double scale = Lfs(base, AiDriver(base, driver).speed_kmh_address());
    const double factor = Lfs(base, AiDriver(base, driver).distance_factor_address());
    const double hundred = Lfs(base, G::hundred_address());
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
  switch (int32_t(AiDriver(base, driver).state())) {
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
      double f0 =
          Fsubs(Lfs(base, AiDriver(base, driver).speed_kmh_address()), Lfs(base, G::ten_address()));
      const double least = Lfs(base, 0x82008948);
      if (f0 < least) {
        f0 = least;
      }
      if (f0 < Lfs(base, AiDriver(base, driver).target_speed_address())) {
        Stfs(base, AiDriver(base, driver).target_speed_address(), f0);
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
  if (!(LoadS16(base, AiPath(base, path).entry_count_address()) > k)) {
    return 0;
  }
  const int32_t at =
      int32_t(uint32_t(LoadS16(base, AiPath(base, path).first_address())) + uint32_t(k));
  return path + uint32_t(at - (at / 80) * 80) * 64 + 16;
}
}  // namespace

uint64_t LimitForCorner(uint8_t* base, uint32_t driver, uint32_t scale, uint32_t record,
                        uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213A620 AiDriver::LimitForCorner");
  uint64_t r3 = driver;
  CopyWord(base, scale, frame + 88);
  const uint32_t ahead = driver + 5654;  // the look-ahead (the path's +5142)
  const uint32_t data = AiCorner(base, record).data().address;
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 84, zero);
  const uint32_t speeds = data + 12;
  const uint32_t target = data + 8;
  const int32_t offset = LoadS16(base, AiCorner(base, record).entry_offset_address());
  const uint32_t before = uint32_t(1 - offset);
  const int32_t entries = LoadS16(base, AiDriver(base, driver).path_entry_count_address());
  const int32_t start = int32_t(uint32_t(LoadS16(base, RouteCorner(base, data).turn_in_address())) +
                                uint32_t(offset));
  const int32_t middle_at = LoadS16(base, RouteCorner(base, data).apex_address());
  Stfs(base, AiCorner(base, record).speed_address(), zero);
  guest::StoreU32(base, frame + 92, before);
  const int32_t end = int32_t(uint32_t(middle_at) + uint32_t(offset));
  const uint32_t path = driver + 512;
  if (start >= 0 && start < entries) {
    const uint32_t entry = RingEntry(base, path, start);
    if (entry != 0) {
      double a, b;
      if (Lfs(base, AiCorner(base, record).direction_address()) < zero) {
        a = Lfs(base, AiDriver(base, driver).road_offset_address());
        b = Lfs(base, AiPathEntry(base, entry).room_low_address());
      } else {
        a = Lfs(base, AiPathEntry(base, entry).room_high_address());
        b = Lfs(base, AiDriver(base, driver).road_offset_address());
      }
      Stfs(base, AiCorner(base, record).speed_address(), Fsubs(a, b));
    }
  }
  if (int32_t(AiDriver(base, driver).path_mode()) == 1) {
    AiCorner(base, record).set_kind(0);
  } else {
    int32_t skill = LoadS16(base, AiDriver(base, driver).skill_corner_address());
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
      blend(speeds, uint32_t(LoadS16(base, AiCorner(base, record).entry_offset_address()) -
                             LoadS16(base, ahead)));
    }
    if (start < end) {
      blend(speeds + 8, uint32_t(start - LoadS16(base, ahead)));
    }
    last = uint32_t(end - LoadS16(base, ahead));
  } else {
    if (int32_t(uint32_t(LoadS16(base, RouteCorner(base, data).exit_address())) +
                uint32_t(LoadS16(base, AiCorner(base, record).entry_offset_address()))) < 0) {
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
  const uint32_t flags = AiDriver(base, driver).flags();
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  uint32_t from;
  if ((flags & 4) != 0) {
    from = driver + 12;
  } else {
    // The vehicle's flags read whatever the vehicle, as the original does.
    const uint32_t state = Vehicle(base, vehicle).car_flags();
    if ((state & 0x60000000u) != 0) {
      from = 0x82008930;  // 70.0
    } else if ((state & 0x1C000000u) != 0) {
      from = kZero;
    } else {
      from = 0x82008948;  // 73.0
    }
  }
  CopyWord(base, from, AiDriver(base, driver).target_speed_address());
  if (AiDriver(base, driver).state() != 0) {
    r3 = UpdateState(base, driver, driver + 6088);
  }
  if (LoadS16(base, AiDriver(base, driver).path_corner_count_address()) > 0) {
    int32_t i = 0;
    uint32_t record = driver + 5676;
    do {
      r3 = LimitForCorner(base, driver, scale, record, frame - 192, f1, f1_set);
      i++;
      record += 44;
    } while (i < LoadS16(base, AiDriver(base, driver).path_corner_count_address()));
  }
  return r3;
}

uint64_t UpdateSpeed(uint8_t* base, uint32_t driver, uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213ACD0 AiDriver::UpdateSpeed");
  int32_t skill = LoadS16(base, AiDriver(base, driver).skill_speed_scale_address());
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
  const uint32_t file = RG::route_file(base).address;
  const uint32_t slow = IsSlow(base, path);
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  double reach = Fmuls(speed, Lfs(base, slow == 0 ? 0x82008944 : 0x82008940));
  const double least = Lfs(base, 0x82002834);
  if (reach < least) {
    reach = least;
  }
  double more = Lfs(base, AiDriver(base, driver).slice_offset_address());
  const double zero = Lfs(base, kZero);
  if (more < zero) {
    more = zero;
  }
  const double ahead = Fadds(reach, more);
  *f1 = ahead;
  int32_t slices = strings_and_font::steps::Fctiwz(
      Fdivs(ahead, Lfs(base, TrackRouteFile(base, file).slice_length_address())));
  if (slices > 78) {
    slices = 78;
  }
  const uint32_t target = driver + 6096;
  AiDriver(base, driver).set_path_look_ahead(uint16_t(slices));
  if (TargetPoint(base, path, ahead, target) == 0) {
    return 0;
  }
  const double dx =
      Fsubs(Lfs(base, target + 0), Lfs(base, AiDriver(base, driver).point_x_address()));
  const double dz = Fsubs(Lfs(base, AiDriver(base, driver).target_z_address()),
                          Lfs(base, AiDriver(base, driver).point_z_address()));
  const uint32_t angle = driver + 6116;
  if (dx == zero && dz == zero) {
    Stfs(base, angle, zero);
  } else {
    const double toward = double(float(c_runtime::Atan2(dx, dz)));
    Stfs(base, angle, Fsubs(toward, Lfs(base, AiDriver(base, driver).heading_address())));
    WrapAngle(base, angle);
    Stfs(base, angle, Fneg(Lfs(base, angle)));
  }
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  const double cos = c_runtime::Cos(Lfs(base, angle));
  const double sin = c_runtime::Sin(Lfs(base, angle));
  *f1 = sin;
  const double s = double(float(sin));
  const double across = Lfs(base, Vehicle(base, vehicle).ground_velocity_address());
  const double c = double(float(cos));
  const uint32_t now = AiDriver(base, driver).vehicle().address;
  const double along = Lfs(base, Vehicle(base, vehicle).ground_velocity_address() + 8);
  const double turned = Lfs(base, angle);
  Stfs(base, AiDriver(base, driver).steer_shift_address(), Fmadds(along, c, Fmuls(s, across)));
  Stfs(base, AiDriver(base, driver).slide_angle_address(), Fadds(Lfs(base, now + 92), turned));
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
    const double yaw = Fabs(Lfs(base, AiDriver(base, driver).yaw_rate_address()));
    if (yaw > zero) {
      const double throttle = Lfs(base, AiDriver(base, driver).throttle_request_address());
      Stfs(base, AiDriver(base, driver).throttle_request_address(),
           throttle < yaw ? zero : Fsubs(throttle, yaw));
    }
    EaseThrottleForSteer(base, driver);
    flag = 1;
  }
  guest::StoreU32(base, frame + 80, flag);
  EaseThrottleForSlide(base, driver, frame + 80);
  const double brake = Lfs(base, AiDriver(base, driver).brake_request_address());
  if (brake > zero) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
    const double step = Lfs(base, 0x8200296C);
    Stfs(base, AiDriver(base, driver).brake_request_address(),
         brake > step ? Fsubs(brake, step) : zero);
  }
  double steer =
      Fmuls(Lfs(base, AiDriver(base, driver).steer_shift_address()), Lfs(base, 0x82002968));
  if (steer < zero) {
    steer = Fneg(steer);
  }
  double request = Lfs(base, AiDriver(base, driver).brake_request_address());
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
    const double slide = Lfs(base, AiDriver(base, driver).steering_address());
    Stfs(base, AiDriver(base, driver).brake_request_address(), request);
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
      Stfs(base, AiDriver(base, driver).brake_request_address(), request);
      kept = true;
    }
  }
  if (!kept) {
    Stfs(base, AiDriver(base, driver).brake_request_address(), zero);
  }
  const double limit = Lfs(base, G::fifth_address());
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
  const int32_t at =
      int32_t(uint32_t(LoadS16(base, AiPath(base, path).first_address())) + uint32_t(k));
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
      InitPath(base, made + AiDriver::kSize * i);
    }
  }
  guest::StoreU32(base, G::drivers_address(), made);
  *r3 = made;
  if (made == 0) {
    return true;
  }
  guest::ForEachHostPart(base, made, 43120, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, i);
    const uint32_t driver = AiDriver::kSize * i + Load(base, G::drivers_address());
    ClearSkill(base, driver + 6048);
    AiDriver(base, driver).set_profile(0);
    *r3 = ResetDriver(base, driver);
    guest::StoreU32(base, AiDriver::kSize * i + Load(base, G::drivers_address()), i);
    if (vehicle != 0) {
      guest::StoreU32(base, AiDriver::kSize * i + Load(base, G::drivers_address()) + 160, vehicle);
    }
  }
  return true;
}

uint64_t DestroyRings(uint8_t* base, uint64_t drivers) {
  ITC_NATIVE_RUNNING("sub_8213AD98 AiDrivers::DestroyRings");
  constexpr uint32_t kEmptyDestructor = 0x82106540;
  uint64_t ring = drivers + 7 * AiDriver::kSize + 528;
  for (int32_t i = 6; i >= 0; i--) {
    ring -= AiDriver::kSize;
    // The empty destructor is never called, so no guest call is needed.
    c_runtime::VectorDestruct(base, ring, 64, 80, kEmptyDestructor, nullptr, nullptr);
  }
  return ring;
}

bool Destroy(uint8_t* base, uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8213B2B8 AiDrivers::Destroy");
  const uint32_t drivers = Load(base, G::drivers_address());
  if (!hooks::NativeHeapActive() || (drivers != 0 && !memory::IsHeapBlock(drivers))) {
    return false;
  }
  if (drivers != 0) {
    DestroyRings(base, drivers);
    memory::crt::OperatorDelete(drivers);
    *r3 = drivers;  // operator delete's adapter leaves r3 as the block
  }
  guest::StoreU32(base, G::drivers_address(), 0);
  return true;
}

void ApplyCornerLine(uint8_t* base, uint32_t path, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8213BB58 AiPath::ApplyCornerLine");
  const uint32_t record = Load(base, index) * 44u + path + 5164;
  const uint32_t data = AiCorner(base, record).data().address;
  if (data == 0) {
    return;
  }
  const uint32_t table = AiCorner(base, record).table().address;
  if (table == 0) {
    return;
  }
  const int32_t count = LoadS16(base, RouteCorner(base, data).entry_count_address());
  const int32_t first = LoadS16(base, AiCorner(base, record).position_address());
  if (first >= count) {
    return;
  }
  int32_t start = int16_t(uint16_t(AiCorner(base, record).entry_offset() + uint32_t(first)));
  if (start >= LoadS16(base, AiPath(base, path).entry_count_address())) {
    return;
  }
  if (start < 0) {
    AiCorner(base, record).set_position(uint16_t(first - start));
    start = 0;
  }
  int32_t k = start;
  int32_t n = LoadS16(base, AiPath(base, path).entry_count_address());
  while (k < n) {
    const int32_t at = LoadS16(base, AiCorner(base, record).position_address());
    if (at >= count) {
      return;
    }
    const uint32_t entry = RingEntry(base, path, k);
    CopyWord(base, uint32_t(at) * 8u + table + 4, AiPathEntry(base, entry).target_offset_address());
    if (AiCorner(base, record).joined_before() == 1) {
      const int32_t until = LoadS16(base, RouteCorner(base, data).turn_in_address());
      const int32_t now = LoadS16(base, AiCorner(base, record).position_address());
      if (now < until) {
        const double slope = Lfs(base, AiCorner(base, record).slope_before_address());
        const double offset = Lfs(base, AiPathEntry(base, entry).target_offset_address());
        Stfs(base, AiPathEntry(base, entry).target_offset_address(),
             Fmadds(IntToFloat(until - now), slope, offset));
      }
    }
    if (AiCorner(base, record).joined_after() == 1) {
      const int32_t from = LoadS16(base, RouteCorner(base, data).apex_address());
      const int32_t now = LoadS16(base, AiCorner(base, record).position_address());
      if (now > from) {
        const double slope = Lfs(base, AiCorner(base, record).slope_after_address());
        const double offset = Lfs(base, AiPathEntry(base, entry).target_offset_address());
        Stfs(base, AiPathEntry(base, entry).target_offset_address(),
             Fmadds(IntToFloat(now - from), slope, offset));
      }
    }
    AiCorner(base, record).set_position(uint16_t(AiCorner(base, record).position() + 1));
    k = int16_t(k + 1);
    n = LoadS16(base, AiPath(base, path).entry_count_address());
  }
}

void ApplyCornerLines(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CFC8 AiPath::ApplyCornerLines");
  int32_t i = 0;
  guest::StoreU32(base, frame + 80, 0);
  if (LoadS16(base, AiPath(base, path).corner_count_address()) <= 0) {
    return;
  }
  do {
    ApplyCornerLine(base, path, frame + 80);
    i++;
    guest::StoreU32(base, frame + 80, uint32_t(i));
  } while (i < LoadS16(base, AiPath(base, path).corner_count_address()));
}

void KeepToCornerSide(uint8_t* base, uint32_t path, uint32_t record, uint32_t count,
                      uint32_t margin, uint32_t shift) {
  ITC_NATIVE_RUNNING("sub_8213CA18 AiPath::KeepToCornerSide");
  int32_t k = LoadS16(base, AiCorner(base, record).entry_offset_address());
  if (k < 1) {
    k = 1;
  }
  const double direction = Lfs(base, AiCorner(base, record).direction_address());
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
      const double high = Fsubs(Lfs(base, AiPathEntry(base, entry).range_high_address()), m);
      if (high < Lfs(base, AiPathEntry(base, entry).room_high_address())) {
        Stfs(base, AiPathEntry(base, entry).room_high_address(), high);
      }
      const double low = Lfs(base, AiPathEntry(base, entry).room_low_address());
      if (Lfs(base, AiPathEntry(base, entry).room_high_address()) < low) {
        Stfs(base, AiPathEntry(base, entry).room_high_address(), low);
      }
    }
  } else {
    if (by > zero) {
      m = Fadds(by, m);
    }
    for (; k < n; k++) {
      const uint32_t entry = RingAt(base, path, k);
      const double low = Fadds(Lfs(base, AiPathEntry(base, entry).range_low_address()), m);
      if (low > Lfs(base, AiPathEntry(base, entry).room_low_address())) {
        Stfs(base, AiPathEntry(base, entry).room_low_address(), low);
      }
      const double high = Lfs(base, AiPathEntry(base, entry).room_high_address());
      if (Lfs(base, AiPathEntry(base, entry).room_low_address()) > high) {
        Stfs(base, AiPathEntry(base, entry).room_low_address(), high);
      }
    }
  }
}

uint32_t SetRanges(uint8_t* base, uint32_t path, uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D670 AiPath::SetRanges");
  uint32_t r3 = path;
  const uint32_t driver = AiPath(base, path).driver().address;
  const uint32_t mode = AiPath(base, path).mode();
  const double steer = Lfs(base, AiDriver(base, driver).steer_shift_address());
  const double half = Lfs(base, AiDriver(base, driver).half_width_address());
  const double shift = Fmuls(steer, Lfs(base, G::fifth_address()));
  Stfs(base, frame + 80, shift);
  Stfs(base, frame + 84, Fadds(half, Lfs(base, 0x82002A14)));
  if (int32_t(mode) == 1) {
    Stfs(base, frame + 80, Lfs(base, kZero));
  } else {
    const double lowest = Lfs(base, G::minus_one_address());
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
  if (LoadS16(base, AiPath(base, path).corner_count_address()) > 0) {
    int32_t i = 0;
    uint32_t record = path + 5164;
    do {
      r3 = path;
      KeepToCornerSide(base, path, record, count, driver + 128, frame + 80);
      i++;
      record += 44;
    } while (i < LoadS16(base, AiPath(base, path).corner_count_address()));
  }
  return r3;
}

uint32_t DropOffRouteCorners(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213DC08 AiPath::DropOffRouteCorners");
  if (LoadS16(base, AiPath(base, path).corner_count_address()) <= 0) {
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
    if (!(kept < LoadS16(base, AiPath(base, path).corner_count_address()))) {
      return r3;
    }
  }
  const double zero = Lfs(base, kZero);
  if (kept > 1) {
    const uint32_t last = path + 44 * uint32_t(kept) + 5120;  // record kept - 1
    if (AiCorner(base, last).joined_after() == 1) {
      const uint32_t data = AiCorner(base, last).data().address;
      AiCorner(base, last).set_joined_after(0);
      const int32_t at = LoadS16(base, AiCorner(base, last).position_address());
      const uint16_t most = RouteCorner(base, data).apex();
      if (at > int32_t(int16_t(most))) {
        AiCorner(base, last).set_position(most);
      }
      Stfs(base, AiCorner(base, last).slope_after_address(), zero);
    }
  }
  int32_t k = kept;
  if (k < LoadS16(base, AiPath(base, path).corner_count_address())) {
    const double one = Lfs(base, kOne);
    do {
      const uint32_t record = path + 44 * uint32_t(k) + 5164;
      k = int16_t(k + 1);
      Stfs(base, AiCorner(base, record).direction_address(), one);
      AiCorner(base, record).set_data(0);
      Stfs(base, AiCorner(base, record).speed_address(), zero);
      AiCorner(base, record).set_table(0);
      Stfs(base, AiCorner(base, record).slope_before_address(), zero);
      AiCorner(base, record).set_number(0);
      Stfs(base, AiCorner(base, record).slope_after_address(), zero);
      AiCorner(base, record).set_entry_offset(0);
      AiCorner(base, record).set_end(0);
      AiCorner(base, record).set_position(0);
      AiCorner(base, record).set_unknown_24(0);
      AiCorner(base, record).set_joined_before(0);
      AiCorner(base, record).set_joined_after(0);
      AiCorner(base, record).set_kind(0);
    } while (k < LoadS16(base, AiPath(base, path).corner_count_address()));
  }
  AiPath(base, path).set_corner_count(uint16_t(kept));
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
    const double offset = Lfs(base, AiPathEntry(base, entry).offset_address());
    const double least = Lfs(base, AiPathEntry(base, entry).range_low_address());
    const double moved =
        Fmadds(IntToFloat(int32_t(uint32_t(i) - uint32_t(low) + 1)), share, offset);
    uint32_t end;
    if (moved < least) {
      end = entry + 32;
    } else if (moved > Lfs(base, AiPathEntry(base, entry).range_high_address())) {
      end = entry + 36;
    } else {
      Stfs(base, AiPathEntry(base, entry).offset_address(), moved);
      low = int32_t(Load(base, first));
      i = int32_t(Load(base, frame + 80));
      continue;
    }
    Stfs(base, frame + 88,
         Fsubs(Lfs(base, end), Lfs(base, AiPathEntry(base, entry).offset_address())));
    SpreadOffset(base, path, first, frame + 80, frame + 88, frame - 128);
    const double bound = Lfs(base, end);
    CopyWord(base, end, AiPathEntry(base, entry).offset_address());
    int32_t high = int32_t(Load(base, last));
    const int32_t at = int32_t(Load(base, frame + 80));
    if (!(at < int32_t(uint32_t(high) - 1))) {
      return;
    }
    const double step =
        Fdivs(Fsubs(bound, moved), IntToFloat(int32_t(uint32_t(high) - uint32_t(at))));
    for (int32_t j = int32_t(uint32_t(at) + 1); j < high; j = int32_t(uint32_t(j) + 1)) {
      // The same entry each time, as the original does.
      const double now = Lfs(base, AiPathEntry(base, entry).offset_address());
      Stfs(base, AiPathEntry(base, entry).offset_address(),
           Fmadds(IntToFloat(int32_t(uint32_t(high) - uint32_t(j))), step, now));
      high = int32_t(Load(base, last));
    }
    return;
  }
}

void ClampOffset(uint8_t* base, uint32_t path, uint32_t entry, uint32_t last, uint32_t first,
                 uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CF58 AiPath::ClampOffset");
  const double offset = Lfs(base, AiPathEntry(base, entry).offset_address());
  uint32_t end;
  if (offset < Lfs(base, AiPathEntry(base, entry).range_low_address())) {
    end = entry + 32;
  } else if (offset > Lfs(base, AiPathEntry(base, entry).range_high_address())) {
    end = entry + 36;
  } else {
    return;
  }
  Stfs(base, frame + 80, Fsubs(Lfs(base, end), offset));
  SpreadOffset(base, path, first, last, frame + 80, frame - 128);
  CopyWord(base, end, AiPathEntry(base, entry).offset_address());
}

uint32_t NarrowToOtherEx(uint8_t* base, uint32_t entry, uint32_t other, uint32_t step,
                         uint32_t offset, uint32_t low, uint32_t high, uint32_t limit) {
  ITC_NATIVE_RUNNING("sub_8213C4C0 AiPathEntry::NarrowToOtherEx");
  if (other == 0) {
    return 0;
  }
  const uint32_t record = AiPathEntry(base, other).slice_record().address;
  if (record == 0) {
    return 0;
  }
  double lo = Fsubs(Lfs(base, AiPathEntry(base, other).range_low_address()), Lfs(base, low));
  double hi = Fadds(Lfs(base, AiPathEntry(base, other).range_high_address()), Lfs(base, high));
  const double least = Lfs(base, RouteSlice(base, record).route_bit_a_address());
  if (lo < least) {
    lo = least;
  }
  const double most = Lfs(base, RouteSlice(base, record).route_bit_b_address());
  if (hi > most) {
    hi = most;
  }
  const double by = Lfs(base, offset);
  lo = Fadds(by, lo);
  hi = Fadds(by, hi);
  if (lo > hi) {
    hi = Fmuls(Fadds(hi, lo), Lfs(base, G::half_address()));
    lo = hi;
  }
  uint32_t narrowed = 0;
  if (Lfs(base, AiPathEntry(base, entry).range_low_address()) < lo) {
    Stfs(base, AiPathEntry(base, entry).range_low_address(), lo);
    Stfs(base, AiPathEntry(base, entry).room_low_address(), lo);
    const double margin = Lfs(base, low);
    if (margin < Lfs(base, limit)) {
      Stfs(base, low, Fadds(Lfs(base, step), margin));
    }
    narrowed = 1;
  } else {
    CopyWord(base, step, low);
  }
  if (Lfs(base, AiPathEntry(base, entry).range_high_address()) > hi) {
    Stfs(base, AiPathEntry(base, entry).range_high_address(), hi);
    Stfs(base, AiPathEntry(base, entry).room_high_address(), hi);
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
  int32_t k = int32_t(uint32_t(LoadS16(base, AiPath(base, path).entry_count_address())) - 1);
  uint32_t r3;
  for (;;) {
    Stfs(base, frame + 88, Fneg(Lfs(base, AiPathEntry(base, other).step_address())));
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
  if (uint32_t((RouteSection(base, b).kind() & 0xF) - 1) <= 3) {
    return -1;
  }
  if (uint32_t((RouteSection(base, a).kind() & 0xF) - 1) <= 3) {
    return 1;
  }
  const uint32_t driver = AiPath(base, path).driver().address;
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  const uint32_t flag = vehicle == 0 ? 0 : (Vehicle(base, vehicle).car_flags() >> 2) & 1;
  if (flag == 0) {
    if (uint32_t(RouteSection(base, b).branch()) - 7 <= 1) {
      return -1;
    }
    if (uint32_t(RouteSection(base, a).branch()) - 7 <= 1) {
      return 1;
    }
  }
  uint32_t passed = 0;
  const uint32_t object = AiDriver(base, driver).profile().address;
  if (object != 0 &&
      uint32_t(LoadS16(base, AiProfile(base, object).branch_kind_address()) - 4) <= 1) {
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
  const uint32_t b_bit = Slw(1, RouteSection(base, b).branch());
  const uint32_t a_bit = Slw(1, RouteSection(base, a).branch());
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
  const uint32_t object = Load(base, AiPath(base, path).driver().address + 500);
  uint32_t on;
  if (object != 0 &&
      uint32_t(LoadS16(base, AiProfile(base, object).branch_kind_address()) - 4) <= 1) {
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
  const uint32_t table = TrackRouteFile(base, file).sections();
  const double zero = Lfs(base, kZero);
  const uint32_t record = Load(base, (section << 2) + table) + uint32_t(slice) * 84 + 124;
  AiPathEntry(base, entry).set_slice_record(record);
  AiPathEntry(base, entry).set_section(guest::LoadU16(base, position + 0));
  AiPathEntry(base, entry).set_slice(guest::LoadU16(base, position + 2));
  const int32_t at = LoadS16(base, lane);
  Stfs(base, AiPathEntry(base, entry).offset_address(), zero);
  Stfs(base, AiPathEntry(base, entry).target_offset_address(), zero);
  AiPathEntry(base, entry).set_lane(uint32_t(at));
  CopyWord(base, RouteSlice(base, record).center_x_address(), AiPathEntry(base, entry).x_address());
  CopyWord(base, RouteSlice(base, record).center_y_address(), AiPathEntry(base, entry).y_address());
  CopyWord(base, RouteSlice(base, record).center_z_address(), AiPathEntry(base, entry).z_address());
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
    const int32_t index =
        int32_t(Load(base, AiPath(base, path).driver().address + 452 + 4 * uint32_t(k)));
    if (index < 0) {
      continue;
    }
    r3 = OfIndex(base, uint32_t(index));
    const uint32_t other = uint32_t(r3);
    const uint32_t driver = AiPath(base, path).driver().address;
    const uint32_t entry = out + 20 * uint32_t(k);
    Stfs(base, AiOtherSpan(base, entry).other_offset_address(), zero);
    Stfs(base, AiOtherSpan(base, entry).step_sum_address(), zero);
    AiOtherSpan(base, entry).set_end_entry(0);
    Stfs(base, AiOtherSpan(base, entry).widths_address(), zero);
    Stfs(base, AiOtherSpan(base, entry).side_offset_address(), zero);
    const uint32_t record = uint32_t(index) * 40 + driver + 172;
    AiOtherSpan(base, entry).set_first_entry(0);
    const uint16_t ring = AiOther(base, record).ring_index();
    const int32_t last = int16_t(ring);
    if (!(at > last)) {
      const int32_t n = LoadS16(base, AiPath(base, path).entry_count_address());
      do {
        if (!(at < n)) {
          break;
        }
        const int32_t m =
            int32_t(uint32_t(LoadS16(base, AiPath(base, path).first_address())) + uint32_t(at));
        at++;
        sum = Fadds(Lfs(base, path + (uint32_t(m - (m / 80) * 80) << 6) + 44), sum);
      } while (!(at > last));
    }
    AiOtherSpan(base, entry).set_first_entry(ring);
    uint16_t next;
    if ((AiOther(base, record).action_flags() & 4) != 0) {
      next = uint16_t(AiOther(base, record).ring_index() + 1);
    } else {
      const int32_t after = LoadS16(base, AiOther(base, record).ring_index_address()) + 1;
      const int32_t look = LoadS16(base, AiPath(base, path).plan_count_address());
      next = uint16_t(look < after ? after : look);
    }
    Stfs(base, AiOtherSpan(base, entry).step_sum_address(), sum);
    AiOtherSpan(base, entry).set_end_entry(next);
    const uint32_t me = AiPath(base, path).driver().address;
    Stfs(base, AiOtherSpan(base, entry).widths_address(),
         Fadds(Lfs(base, AiDriver(base, me).half_width_address()),
               Lfs(base, AiDriver(base, other).half_width_address())));
    const int32_t n = LoadS16(base, AiPath(base, path).entry_count_address());
    if (!(LoadS16(base, AiOther(base, record).ring_index_address()) < n)) {
      guest::StoreU32(base, AiPath(base, path).driver().address + 452 + 4 * uint32_t(k),
                      0xFFFFFFFFu);
      return r3;
    }
    const uint32_t file = RG::route_file(base).address;
    const double gap = Lfs(base, AiOther(base, record).gap_address());
    const double length = Lfs(base, TrackRouteFile(base, file).slice_length_address());
    int32_t slot;
    if (gap > length) {
      CopyWord(base, AiDriver(base, other).road_offset_address(),
               AiOtherSpan(base, entry).other_offset_address());
      if (LoadS16(base, AiPath(base, path).entry_count_address()) <= 0) {
        continue;
      }
      if ((AiOther(base, record).action_flags() & 0x10) == 0) {
        continue;
      }
      slot = LoadS16(base, AiPath(base, path).first_address());
    } else {
      int32_t ahead = int32_t(
          uint32_t(strings_and_font::steps::Fctiwz(Fdivs(Fneg(gap), length))) + 1);
      if (ahead < 0) {
        ahead = 0;
      }
      if (!(ahead < n)) {
        CopyWord(base, AiDriver(base, other).road_offset_address(),
                 AiOtherSpan(base, entry).other_offset_address());
        continue;
      }
      slot = int32_t(uint32_t(LoadS16(base, AiPath(base, path).first_address())) + uint32_t(ahead));
    }
    const double ox = Lfs(base, AiDriver(base, other).point_x_address());
    const double oz = Lfs(base, AiDriver(base, other).point_z_address());
    const uint32_t slice = Load(base, path + (uint32_t(slot - (slot / 80) * 80) << 6) + 68);
    double dx = Fsubs(ox, Lfs(base, RouteSlice(base, slice).center_x_address()));
    const double dz = Fsubs(oz, Lfs(base, RouteSlice(base, slice).center_z_address()));
    dx = Fmuls(dx, Lfs(base, RouteSlice(base, slice).side_x_address()));
    Stfs(base, AiOtherSpan(base, entry).other_offset_address(),
         Fmadds(dz, Lfs(base, RouteSlice(base, slice).side_z_address()), dx));
  }
  return r3;
}

namespace {
// AvoidOnLeft's and AvoidOnRight's spread: the width over a's +0 - 1
// entries (at frame + 80), kept within the path's step +5516.
void SpreadWidth(uint8_t* base, uint32_t path, uint32_t a, double line, double mine,
                 uint32_t frame) {
  const double zero = Lfs(base, kZero);
  const int32_t first = LoadS16(base, AiOtherSpan(base, a).first_entry_address());
  double width = Fsubs(line, mine);
  Stfs(base, frame + 80, width);
  if (first > 1) {
    width = Fdivs(width, IntToFloat(first - 1));
    Stfs(base, frame + 80, width);
  }
  const double step = Lfs(base, AiPath(base, path).largest_step_address());
  if (Fadds(step, width) < zero) {
    Stfs(base, frame + 80, Fneg(step));
  } else if (Fsubs(width, step) > zero) {
    Stfs(base, frame + 80, step);
  }
}
}  // namespace

uint64_t AvoidOnLeft(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CB30 AiPath::AvoidOnLeft");
  const uint32_t driver = AiPath(base, path).driver().address;
  uint64_t r3 = driver;
  const uint32_t mode = AiPath(base, path).mode();
  const double across = Fmuls(Lfs(base, AiOtherSpan(base, a).side_offset_address()),
                              Lfs(base, AiOther(base, record).factor_address()));
  const double zero = Lfs(base, kZero);
  double steer = zero;
  const double mine = Lfs(base, AiDriver(base, driver).road_offset_address());
  CopyWord(base, AiDriver(base, driver).road_offset_address(), frame + 96);
  if (int32_t(mode) != 1) {
    steer = Lfs(base, AiDriver(base, driver).steer_shift_address());
  }
  if (mine < across) {
    guest::StoreU32(base, frame + 88, mode == 1 ? 0 : 1);
    AdjustForOther(base, driver, record, frame + 88);
  }
  double line = Fadds(across, Lfs(base, 0x82002A3C));
  if (mine < line && Lfs(base, AiOther(base, record).gap_address()) < zero) {
    line = Fmadds(IntToFloat(LoadS16(base, AiOtherSpan(base, a).end_entry_address()) + 1),
                  Fsubs(line, mine), line);
  }
  line = Fadds(line, Lfs(base, 0x8200296C));
  Stfs(base, frame + 88, line);
  if (steer > zero) {
    line = Fmadds(steer, Lfs(base, G::fifth_address()), line);
    Stfs(base, frame + 88, line);
  }
  if (!(mine < line)) {
    for (int32_t j = 1; j < LoadS16(base, AiOtherSpan(base, a).end_entry_address()); j++) {
      const uint32_t entry = RingAt(base, path, j);
      line = Fsubs(line, Lfs(base, AiPathEntry(base, entry).step_address()));
      if (Lfs(base, AiPathEntry(base, entry).room_low_address()) < line) {
        const double most = Lfs(base, AiPathEntry(base, entry).room_high_address());
        Stfs(base, AiPathEntry(base, entry).room_low_address(), line);
        if (line > most) {
          Stfs(base, AiPathEntry(base, entry).room_low_address(), most);
        }
      }
    }
    return r3;
  }
  SpreadWidth(base, path, a, line, mine, frame);
  for (int32_t j = 1; j < LoadS16(base, AiOtherSpan(base, a).end_entry_address()); j++) {
    r3 = RingAt(base, path, j);
    BlockLeft(base, uint32_t(r3), frame + 88, frame + 80, frame + 96);
  }
  return r3;
}

uint64_t AvoidOnRight(uint8_t* base, uint32_t path, uint32_t a, uint32_t record,
                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213CD50 AiPath::AvoidOnRight");
  uint64_t r3 = path;
  const double across = Fmuls(Lfs(base, AiOtherSpan(base, a).side_offset_address()),
                              Lfs(base, AiOther(base, record).factor_address()));
  const uint32_t driver = AiPath(base, path).driver().address;
  const double mine = Lfs(base, AiDriver(base, driver).road_offset_address());
  CopyWord(base, AiDriver(base, driver).road_offset_address(), frame + 96);
  if (mine > across) {
    guest::StoreU32(base, frame + 88, AiPath(base, path).mode());
    AdjustForOther(base, driver, record, frame + 88);
    r3 = driver;
  }
  double line = Fsubs(across, Lfs(base, 0x82002A3C));
  const double zero = Lfs(base, kZero);
  if (mine > line && Lfs(base, AiOther(base, record).gap_address()) < zero) {
    line = Fmadds(IntToFloat(LoadS16(base, AiOtherSpan(base, a).end_entry_address()) + 1),
                  Fsubs(line, mine), line);
  }
  const double steer = Lfs(base, AiDriver(base, driver).steer_shift_address());
  line = Fsubs(line, Lfs(base, 0x8200296C));
  Stfs(base, frame + 88, line);
  if (steer < zero) {
    line = Fmadds(steer, Lfs(base, G::fifth_address()), line);
    Stfs(base, frame + 88, line);
  }
  if (!(mine > line)) {
    for (int32_t j = 1; j < LoadS16(base, AiOtherSpan(base, a).end_entry_address()); j++) {
      const uint32_t entry = RingAt(base, path, j);
      line = Fsubs(line, Lfs(base, AiPathEntry(base, entry).step_address()));
      if (Lfs(base, AiPathEntry(base, entry).room_high_address()) > line) {
        const double least = Lfs(base, AiPathEntry(base, entry).room_low_address());
        Stfs(base, AiPathEntry(base, entry).room_high_address(), line);
        if (line < least) {
          Stfs(base, AiPathEntry(base, entry).room_high_address(), least);
        }
      }
    }
    return r3;
  }
  SpreadWidth(base, path, a, line, mine, frame);
  for (int32_t j = 1; j < LoadS16(base, AiOtherSpan(base, a).end_entry_address()); j++) {
    r3 = RingAt(base, path, j);
    BlockRight(base, uint32_t(r3), frame + 88, frame + 80, frame + 96);
  }
  return r3;
}

uint64_t ChooseSideForOther(uint8_t* base, uint32_t path, uint32_t a, uint32_t record,
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8213D788 AiPath::ChooseSideForOther");
  const uint32_t driver = AiPath(base, path).driver().address;
  const double a12 = Lfs(base, AiOtherSpan(base, a).widths_address());
  const int32_t index = LoadS16(base, AiOther(base, record).ring_index_address());
  const int32_t n = LoadS16(base, AiPath(base, path).entry_count_address());
  const double a8 = Lfs(base, AiOtherSpan(base, a).step_sum_address());
  const double width = Fadds(a12, Lfs(base, G::fifth_address()));
  const double reach = Fadds(Lfs(base, AiDriver(base, driver).road_offset_address()), a8);
  const double zero = Lfs(base, kZero);
  double left = zero, right = zero;
  if (index < n) {
    const int32_t m =
        int32_t(uint32_t(LoadS16(base, AiPath(base, path).first_address())) + uint32_t(index));
    const uint32_t entry = path + (uint32_t(m - (m / 80) * 80) << 6);
    left = Lfs(base, entry + 56);
    right = Lfs(base, entry + 60);
  }
  const int32_t kind = LoadS16(base, AiOther(base, record).state_address());
  const double other = Lfs(base, AiOtherSpan(base, a).other_offset_address());
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
    Stfs(base, AiOtherSpan(base, a).side_offset_address(), Fsubs(other, width));
    return AvoidOnRight(base, path, a, record, stack - 160);
  }
  Stfs(base, AiOtherSpan(base, a).side_offset_address(), Fadds(other, width));
  return AvoidOnLeft(base, path, a, record, stack - 176);
}

uint64_t AvoidByLane(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D898 AiPath::AvoidByLane");
  const uint32_t driver = AiPath(base, path).driver().address;
  const double other = Lfs(base, AiOtherSpan(base, a).other_offset_address());
  const double one = Lfs(base, kOne);
  const double room = Fadds(Lfs(base, AiOtherSpan(base, a).widths_address()), one);
  const double off = Fsubs(Lfs(base, AiDriver(base, driver).road_offset_address()), other);
  if (!(off < room) || !(off > Fneg(room))) {
    return path;
  }
  if (LoadS16(base, AiPath(base, path).entry_count_address()) <= 1) {
    return path;
  }
  const uint32_t entry = RingAt(base, path, 1);
  if (int32_t(entry) == 0) {
    return path;
  }
  const uint32_t slice = AiPathEntry(base, entry).slice_record().address;
  if (slice == 0) {
    return path;
  }
  const uint32_t lanes = RouteSlice(base, slice).lanes() & 0xF;
  int32_t lane = 0;
  if (lanes > 1) {
    const int32_t now = int32_t(AiPathEntry(base, entry).lane());
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
  const double width = Lfs(base, RouteSlice(base, slice).width_address());
  const double half = Lfs(base, G::half_address());
  const double count = IntToFloat(int32_t(lanes));
  const double centered = Fnmsubs(Fsubs(count, one), half, IntToFloat(lane));
  Stfs(base, AiOtherSpan(base, a).side_offset_address(), Fmuls(Fdivs(centered, count), width));
  if (lane > int32_t(AiPathEntry(base, entry).lane())) {
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
    Stfs(base, AiOtherSpan(base, a).other_offset_address(), zero);
    AiOtherSpan(base, a).set_first_entry(0);
    Stfs(base, AiOtherSpan(base, a).step_sum_address(), zero);
    AiOtherSpan(base, a).set_end_entry(0);
    Stfs(base, AiOtherSpan(base, a).widths_address(), zero);
    Stfs(base, AiOtherSpan(base, a).side_offset_address(), zero);
  }
  uint64_t r3 = MeasureOthers(base, path, frame + 80);
  for (int32_t k = 0; k < count; k++) {
    const uint32_t driver = AiPath(base, path).driver().address;
    const int32_t index = int32_t(Load(base, driver + 452 + 4 * uint32_t(k)));
    if (index < 0) {
      return r3;
    }
    const uint32_t record = uint32_t(index) * 40 + driver + 172;
    const uint32_t a = frame + 80 + 20 * uint32_t(k);
    const int32_t flags = LoadS16(base, AiOther(base, record).action_flags_address());
    if ((flags & 1) != 0) {
      r3 = ChooseSideForOther(base, path, a, record, frame);
    } else if ((flags & 6) != 0) {
      if (k == 0) {
        CopyWord(base, AiOtherSpan(base, a).other_offset_address(),
                 AiOtherSpan(base, a).side_offset_address());
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
  const uint32_t time = TimeFromTicks(G::race_state(base).ticks());
  return int32_t(((time >> 16) & 0x7FFF) * 60 + ((time >> 10) & 0x3F));
}
// The race state's record (+2184 + 972 x i + 4) whose word matches the
// driver's +500 object's halfword +0; 0 with none.
uint32_t SkillEntry(uint8_t* base, uint32_t driver) {
  const uint32_t object = AiDriver(base, driver).profile().address;
  const uint32_t list = G::race_state(base).results_address();
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
  const uint32_t kind = AiSkillTrigger(base, trigger).kind();
  const auto value = [&] { return LoadS16(base, AiSkillTrigger(base, trigger).value_address()); };
  switch (kind) {
    case 0:
      return Lfs(base, AiDriver(base, driver).player_gap_address()) < IntToFloat(value()) ? 0 : 1;
    case 1:
      return Lfs(base, AiDriver(base, driver).player_gap_address()) >
                     IntToFloat(int32_t(0u - uint32_t(value())))
                 ? 0
                 : 1;
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
      return AiDriver(base, driver).in_corner() != 0 ? 0 : 1;
    case 9:
      return AiDriver(base, driver).in_corner() == 0 ? 0 : 1;
    case 10: {
      const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
      if (vehicle == 0) {
        return 0;
      }
      return (Vehicle(base, vehicle).collision_flags() & 1) == 0 ? 0 : 1;
    }
    case 11:
      return LoadS16(base, AiDriver(base, driver).passes_a_address()) > 0 ? 1 : 0;
    case 12:
      return LoadS16(base, AiDriver(base, driver).passes_b_address()) > 0 ? 1 : 0;
    case 13:
      return (Load(base, Load(base, G::inputs_address())) & 0x100) == 0 ? 0 : 1;
    case 14:
      return (Load(base, Load(base, G::inputs_address())) & 0x20000) == 0 ? 0 : 1;
    default:
      return 0;
  }
}

uint32_t TriggerEnds(uint8_t* base, uint32_t state, uint32_t driver, uint32_t trigger,
                     double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213FB48 AiSkill::TriggerEnds");
  const uint32_t kind = AiSkillTrigger(base, trigger).kind();
  const auto value = [&] { return LoadS16(base, AiSkillTrigger(base, trigger).value_address()); };
  switch (kind) {
    case 0:
      return Lfs(base, AiDriver(base, driver).player_gap_address()) < IntToFloat(value()) ? 1 : 0;
    case 1:
      return Lfs(base, AiDriver(base, driver).player_gap_address()) >
                     IntToFloat(int32_t(0u - uint32_t(value())))
                 ? 1
                 : 0;
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
      return AiDriver(base, driver).in_corner() != 0 ? 1 : 0;
    case 9:
      return AiDriver(base, driver).in_corner() == 0 ? 1 : 0;
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
  if (AiSkill(base, skill).source().address == 0 ||
      int32_t(AiSkill(base, skill).runs_triggers()) != 1) {
    return 0;
  }
  uint32_t changed = 0;
  uint32_t state = skill + 24;
  for (uint32_t at = 16; at < 40; at += 8, state += 4) {
    const uint32_t trigger = at + AiSkill(base, skill).source().address;
    if (int32_t(AiSkillTrigger(base, trigger).kind()) == -1) {
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
      const double now = Lfs(base, AiSkill(base, skill).percentage_address());
      Stfs(base, AiSkill(base, skill).percentage_address(),
           Fadds(IntToFloat(LoadS16(base, AiSkillTrigger(base, trigger).value_address())), now));
    } else if (step < 0) {
      guest::StoreU8(base, state + 2, 0);
      const double now = Lfs(base, AiSkill(base, skill).percentage_address());
      Stfs(base, AiSkill(base, skill).percentage_address(),
           Fsubs(now, IntToFloat(LoadS16(base, AiSkillTrigger(base, trigger).value_address()))));
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
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  if (speed < Lfs(base, G::hundred_address())) {
    if (speed < Lfs(base, G::twenty_address())) {
      return;
    }
    if (speed <
        Fmuls(Lfs(base, AiDriver(base, driver).target_speed_address()), Lfs(base, 0x82001070))) {
      return;
    }
  }
  const double back = Fneg(Lfs(base, AiCorner(base, record).direction_address()));
  const double steer = Lfs(base, AiDriver(base, driver).steer_request_address());
  const double zero = Lfs(base, kZero);
  const double side = Lfs(base, steer < zero ? driver + 112 : driver + 108);
  const double ratio = Fmuls(Fdivs(steer, side), back);
  const double fifth = Lfs(base, G::fifth_address());
  if (!(ratio > fifth)) {
    return;
  }
  const double yaw = Fmuls(Lfs(base, AiDriver(base, driver).yaw_rate_address()), back);
  if (yaw < Lfs(base, 0x82002A10)) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
    Stfs(base, AiDriver(base, driver).steer_request_address(), Fmuls(steer, Lfs(base, 0x820021E8)));
  }
  if (int32_t(Load(base, flag)) == 0) {
    return;
  }
  if (!(ratio > Lfs(base, 0x820029D4)) || yaw < fifth) {
    return;
  }
  double most = Lfs(base, kOne);
  const double grip = Lfs(base, AiDriver(base, driver).corner_grip_address());
  if (grip < most) {
    most = grip;
  }
  if (!(yaw < most)) {
    return;
  }
  Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
  AiDriver(base, driver).set_handbrake(1);
}

namespace {
// The room on the corner's side at the first ring entry (its +40 or +44
// against the driver's +144); `valid` false without that entry or its slice.
double CornerRoom(uint8_t* base, uint32_t driver, uint32_t record, bool* valid) {
  const uint32_t path = driver + 512;
  const uint32_t entry =
      LoadS16(base, AiPath(base, path).entry_count_address()) > 1 ? RingAt(base, path, 1) : 0;
  *valid = entry != 0 && AiPathEntry(base, entry).slice_record().address != 0;
  if (!*valid) {
    return 0;
  }
  if (Lfs(base, AiCorner(base, record).direction_address()) < Lfs(base, kZero)) {
    return Fsubs(Lfs(base, AiDriver(base, driver).road_offset_address()),
                 Lfs(base, AiPathEntry(base, entry).room_low_address()));
  }
  return Fsubs(Lfs(base, AiPathEntry(base, entry).room_high_address()),
               Lfs(base, AiDriver(base, driver).road_offset_address()));
}
}  // namespace

uint32_t SteerByRoom(uint8_t* base, uint32_t driver, uint32_t record, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82138FA8 AiDriver::SteerByRoom");
  uint32_t result = 0;
  if (LoadS16(base, AiDriver(base, driver).path_entry_count_address()) > 1) {
    bool valid = false;
    const double room = CornerRoom(base, driver, record, &valid);
    if (valid) {
      Stfs(base, out, room);
      if (room > Lfs(base, AiDriver(base, driver).half_width_address())) {
        const double angle = Lfs(base, AiDriver(base, driver).target_angle_address());
        if (!(Fmuls(Lfs(base, AiCorner(base, record).direction_address()), angle) >
              Lfs(base, kZero))) {
          result = 1;
        } else {
          const double now = Lfs(base, AiDriver(base, driver).steer_request_address());
          const double half = Fmuls(angle, Lfs(base, G::half_address()));
          const double sum =
              Fadds(Fadds(Lfs(base, AiDriver(base, driver).slide_angle_address()), now), half);
          Stfs(base, AiDriver(base, driver).steer_request_address(), sum);
          const double size = Lfs(base, AiDriver(base, driver).car_radius_address());
          const double left = Lfs(base, out);
          if (size > left) {
            Stfs(base, AiDriver(base, driver).steer_request_address(),
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
  const double back = Fneg(Lfs(base, AiCorner(base, record).direction_address()));
  const double zero = Lfs(base, kZero);
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  Stfs(base, frame + 80, zero);
  if (!(speed > Lfs(base, G::twenty_address()))) {
    AccumulateSteer(base, driver);
    return driver;
  }
  const uint32_t result = SteerByRoom(base, driver, record, frame + 80);
  if (result == 0) {
    return 0;
  }
  const double turn = Fmuls(Lfs(base, AiDriver(base, driver).target_angle_address()), back);
  const double target = Lfs(base, AiDriver(base, driver).target_speed_address());
  const double now = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  bool cut;
  if (turn < zero) {
    cut = now > target ||
          (Fmuls(Lfs(base, AiDriver(base, driver).road_offset_address()), back) < zero &&
           Fmuls(Lfs(base, AiDriver(base, driver).yaw_rate_address()), back) < zero);
  } else {
    cut = now > target &&
          !(Fmuls(Lfs(base, AiDriver(base, driver).slide_angle_address()), back) < zero);
  }
  if (cut) {
    Stfs(base, AiDriver(base, driver).throttle_request_address(), zero);
  }
  return result;
}

uint64_t SteerAtCornerPoint(uint8_t* base, uint32_t driver, uint32_t record, uint32_t at,
                            uint32_t frame, double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213A200 AiDriver::SteerAtCornerPoint");
  int32_t k = int32_t(Load(base, at));
  const double zero = Lfs(base, kZero);
  const int32_t look = LoadS16(base, AiDriver(base, driver).path_look_ahead_address());
  if (k < look) {
    k = look;
  }
  if (!(k < LoadS16(base, AiDriver(base, driver).path_entry_count_address()))) {
    AccumulateSteer(base, driver);
    return driver;
  }
  bool valid = false;
  double room = CornerRoom(base, driver, record, &valid);
  if (!valid) {
    room = zero;
  }
  if (room < Lfs(base, AiDriver(base, driver).half_width_address())) {
    AiCorner(base, record).set_kind(3);
    return SteerOutOfCorner(base, driver, record, frame - 96);
  }
  const uint32_t path = driver + 512;
  const uint32_t target = RingEntry(base, path, k);
  if (target == 0) {
    AccumulateSteer(base, driver);
    return driver;
  }
  const double dx =
      Fsubs(Lfs(base, target + 0), Lfs(base, AiDriver(base, driver).point_x_address()));
  const double dz =
      Fsubs(Lfs(base, target + 8), Lfs(base, AiDriver(base, driver).point_z_address()));
  *f1 = dx;  // the original works it out in f1
  *f1_set = true;
  if (dx == zero && dz == zero) {
    AccumulateSteer(base, driver);
    return driver;
  }
  const double toward = c_runtime::Atan2(dx, dz);
  *f1 = toward;
  *f1_set = true;
  Stfs(base, frame + 80,
       Fneg(Fsubs(double(float(toward)), Lfs(base, AiDriver(base, driver).heading_address()))));
  WrapAngle(base, frame + 80);
  const double turn = Lfs(base, AiDriver(base, driver).target_angle_address());
  const double angle = Lfs(base, frame + 80);
  if (Fmuls(turn, angle) != zero) {
    Stfs(base, AiDriver(base, driver).steer_request_address(),
         Fadds(Lfs(base, AiDriver(base, driver).steer_request_address()), turn));
  }
  const double sum = Fadds(Lfs(base, AiDriver(base, driver).slide_angle_address()),
                           Lfs(base, AiDriver(base, driver).steer_request_address()));
  Stfs(base, AiDriver(base, driver).steer_request_address(), sum);
  if (!(room < zero)) {
    double add = Fmuls(Fdivs(room, Lfs(base, AiDriver(base, driver).car_radius_address())), angle);
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
    Stfs(base, AiDriver(base, driver).steer_request_address(), Fadds(sum, add));
  }
  guest::StoreU32(base, frame + 84, 0);
  LimitSlide(base, driver, record, frame + 84);
  return driver;
}

uint64_t SteerIntoCorner(uint8_t* base, uint32_t driver, uint32_t record, uint32_t frame,
                         double* f1, bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AB40 AiDriver::SteerIntoCorner");
  const uint32_t data = AiCorner(base, record).data().address;
  const int32_t offset = LoadS16(base, AiCorner(base, record).entry_offset_address());
  if (int32_t(uint32_t(LoadS16(base, RouteCorner(base, data).apex_address())) + uint32_t(offset)) <
      0) {
    AiCorner(base, record).set_kind(3);
    return SteerOutOfCorner(base, driver, record, frame - 96);
  }
  guest::StoreU32(
      base, frame + 80,
      uint32_t(LoadS16(base, RouteCorner(base, data).turn_in_address())) + uint32_t(offset));
  return SteerAtCornerPoint(base, driver, record, frame + 80, frame - 128, f1, f1_set);
}

uint32_t UpdateSteering(uint8_t* base, uint32_t driver, uint32_t frame, double* f1,
                        bool* f1_set) {
  ITC_NATIVE_RUNNING("sub_8213AEF0 AiDriver::UpdateSteering");
  UpdateSteerInput(base, driver, frame, f1, f1_set);
  double scale = Lfs(base, 0x8200296C);
  double limit = Lfs(base, 0x82002A10);
  const uint32_t slow = IsSlow(base, driver + 512);
  const double fifth = Lfs(base, G::fifth_address());
  if (slow == 0) {
    const double yaw = Lfs(base, AiDriver(base, driver).yaw_rate_address());
    if (yaw > Lfs(base, 0x82002FB0) || yaw < Lfs(base, 0x82002C00)) {
      Stfs(base, AiDriver(base, driver).steer_request_address(),
           Fnmsubs(yaw, fifth, Lfs(base, AiDriver(base, driver).steer_request_address())));
    }
  } else {
    scale = fifth;
    limit = Lfs(base, 0x82002A14);
  }
  const double yaw = Lfs(base, AiDriver(base, driver).yaw_rate_address());
  if (yaw > limit) {
    Stfs(base, AiDriver(base, driver).steer_request_address(),
         Fnmsubs(Fsubs(yaw, limit), scale,
                 Lfs(base, AiDriver(base, driver).steer_request_address())));
  } else if (yaw < Fneg(limit)) {
    Stfs(base, AiDriver(base, driver).steer_request_address(),
         Fnmsubs(Fadds(yaw, limit), scale,
                 Lfs(base, AiDriver(base, driver).steer_request_address())));
  }
  double steer = Lfs(base, AiDriver(base, driver).steer_request_address());
  const double zero = Lfs(base, kZero);
  steer = Fdivs(steer, Lfs(base, steer < zero ? driver + 112 : driver + 108));
  const double now = Lfs(base, AiDriver(base, driver).steering_address());
  Stfs(base, AiDriver(base, driver).steer_request_address(), steer);
  double move = Fsubs(steer, now);
  if (Fadds(move, fifth) < zero) {
    move = Lfs(base, 0x82006908);
  } else if (Fsubs(move, fifth) > zero) {
    move = fifth;
  }
  const double next = Fadds(now, move);
  Stfs(base, AiDriver(base, driver).steering_address(), next);
  const double one = Lfs(base, kOne);
  if (Fadds(next, one) < zero) {
    Stfs(base, AiDriver(base, driver).steering_address(), Lfs(base, G::minus_one_address()));
  } else if (Fsubs(next, one) > zero) {
    Stfs(base, AiDriver(base, driver).steering_address(), one);
  }
  return slow;
}

uint64_t Drive(uint8_t* base, uint32_t driver, uint32_t input, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_8213B058 AiDriver::Drive");
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  StoreConstant(base, AiDriver(base, driver).throttle_request_address(), 0x820027A8);
  AiDriver(base, driver).set_handbrake(0);
  const double zero = Lfs(base, kZero);
  Stfs(base, AiDriver(base, driver).brake_request_address(), zero);
  Stfs(base, AiDriver(base, driver).steer_request_address(), zero);
  if (SteerToTarget(base, driver, f1) == 0) {
    Stfs(base, AiDriver(base, driver).steering_address(), zero);
    AiDriver(base, driver).set_handbrake(0);
    Stfs(base, AiDriver(base, driver).brake_address(), zero);
    Stfs(base, AiDriver(base, driver).throttle_address(), zero);
  } else if (((Vehicle(base, vehicle).car_flags() >> 26) & 1) != 0) {
    Stfs(base, AiDriver(base, driver).steering_address(), zero);
    Stfs(base, AiDriver(base, driver).throttle_address(), zero);
    Stfs(base, AiDriver(base, driver).brake_address(), Lfs(base, kOne));
    CarInput(base, input).set_handbrake(32767);
  } else {
    double f = 0;
    bool set = false;
    UpdateSpeed(base, driver, frame - 96, &f, &set);
    if (LoadS16(base, AiDriver(base, driver).skill_caps_target_address()) <= 0) {
      const double most = Lfs(base, AiDriver(base, driver).speed_limit_address());
      if (Lfs(base, AiDriver(base, driver).target_speed_address()) > most) {
        Stfs(base, AiDriver(base, driver).target_speed_address(), most);
      }
    }
    const double least = Lfs(base, G::twenty_address());
    if (Lfs(base, AiDriver(base, driver).target_speed_address()) < least) {
      Stfs(base, AiDriver(base, driver).target_speed_address(), least);
    }
    UpdateSteering(base, driver, frame - 112, &f, &set);
    if (set) {
      *f1 = f;
    }
    UpdatePedals(base, driver, frame - 112);
    if ((AiDriver(base, driver).flags() & 0x20) != 0) {
      CarInput(base, input).set_flags(CarInput(base, input).flags() | 0x200);
      Stfs(base, AiDriver(base, driver).steering_address(),
           Fmuls(Lfs(base, AiDriver(base, driver).steering_address()), Lfs(base, 0x82001BE8)));
      Stfs(base, AiDriver(base, driver).throttle_address(), zero);
    }
  }
  if (AiDriver(base, driver).handbrake() != 0) {
    CarInput(base, input).set_handbrake(32767);
  }
  LimitThrottle(base, driver);
  const double brake = Lfs(base, AiDriver(base, driver).brake_address());
  if (brake > zero) {
    Stfs(base, AiDriver(base, driver).throttle_address(), zero);
  }
  const double scale = Lfs(base, 0x82007F58);
  const double steer = Fmuls(Lfs(base, AiDriver(base, driver).steering_address()), scale);
  const double throttle = Fmuls(Lfs(base, AiDriver(base, driver).throttle_address()), scale);
  const double pressed = Fmuls(brake, scale);
  CarInput(base, input).set_throttle(uint16_t(strings_and_font::steps::Fctiwz(throttle)));
  CarInput(base, input).set_steering(uint16_t(strings_and_font::steps::Fctiwz(steer)));
  CarInput(base, input).set_brake(uint16_t(strings_and_font::steps::Fctiwz(pressed)));
  return driver;
}

// --- The path's line through the corners ---

namespace {
// The entry's offset +24 (just set to `offset`) kept within +40 to +44.
void KeepInRange(uint8_t* base, uint32_t entry, double offset) {
  const double low = Lfs(base, AiPathEntry(base, entry).room_low_address());
  if (offset < low) {
    Stfs(base, AiPathEntry(base, entry).offset_address(), low);
    return;
  }
  const double high = Lfs(base, AiPathEntry(base, entry).room_high_address());
  if (offset > high) {
    Stfs(base, AiPathEntry(base, entry).offset_address(), high);
  }
}
// The entry's offset +24 moved from `from` by at most the path's +5516.
void LimitStep(uint8_t* base, uint32_t path, uint32_t entry, double from, double zero) {
  double step = Fsubs(Lfs(base, AiPathEntry(base, entry).offset_address()), from);
  const double most = Lfs(base, AiPath(base, path).largest_step_address());
  if (Fadds(most, step) < zero) {
    step = Fneg(most);
  } else if (Fsubs(step, most) > zero) {
    step = most;
  }
  Stfs(base, AiPathEntry(base, entry).offset_address(), Fadds(step, from));
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
  *end = int32_t(uint32_t(LoadS16(base, AiCorner(base, record).data().address + point)) +
                 uint32_t(LoadS16(base, AiCorner(base, record).entry_offset_address())));
  *reached = 0;
  if (!(*end > *start)) {
    return false;
  }
  guest::StoreU32(base, frame + 80, uint32_t(*start));
  *span = int32_t(uint32_t(*end) - uint32_t(*start));
  const int32_t look = LoadS16(base, AiPath(base, path).plan_count_address());
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
  KeepInRange(base, entry, Lfs(base, AiPathEntry(base, entry).offset_address()));
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
  if (!(int32_t(Load(base, index)) < LoadS16(base, AiPath(base, path).plan_count_address()))) {
    return;
  }
  const double zero = Lfs(base, kZero);
  uint32_t previous = entry;
  do {
    const uint32_t at = RingEntry(base, path, int32_t(Load(base, index)));
    const double offset = Fadds(Lfs(base, AiPathEntry(base, at).step_address()),
                                Lfs(base, AiPathEntry(base, previous).offset_address()));
    Stfs(base, AiPathEntry(base, at).offset_address(), offset);
    const double low = Lfs(base, AiPathEntry(base, at).room_low_address());
    bool clamp = offset < low;
    double bound = low;
    if (!clamp) {
      bound = Lfs(base, AiPathEntry(base, at).room_high_address());
      clamp = offset > bound;
    }
    if (clamp) {
      Stfs(base, AiPathEntry(base, at).offset_address(), bound);
      LimitStep(base, path, at, offset, zero);
    }
    ClampOffset(base, path, at, index, first, frame - 96);
    guest::StoreU32(base, index, Load(base, index) + 1);
    previous = at;
  } while (int32_t(Load(base, index)) < LoadS16(base, AiPath(base, path).plan_count_address()));
}

uint32_t ApproachCorner(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                        uint32_t previous, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213DE30 AiPath::ApproachCorner");
  int32_t end = int32_t(uint32_t(LoadS16(base, AiCorner(base, record).entry_offset_address())) + 1);
  const int32_t start = int32_t(Load(base, index));
  if (!(end > start)) {
    return 0;
  }
  const uint32_t at = RingEntry(base, path, end);
  guest::StoreU32(base, frame + 80, uint32_t(start));
  const double target = at != 0 ? Lfs(base, AiPathEntry(base, at).target_offset_address())
                                : Lfs(base, AiCorner(base, record).table().address + 4);
  const double gap =
      Fmsubs(target, Lfs(base, G::four_fifths_address()), Lfs(base, Load(base, previous) + 24));
  const double most =
      Fmuls(Lfs(base, AiPath(base, path).largest_step_address()), Lfs(base, G::half_address()));
  const double zero = Lfs(base, kZero);
  double step = Fdivs(gap, IntToFloat(int32_t(uint32_t(end) - uint32_t(start))));
  if (Fadds(most, step) < zero) {
    step = Fneg(most);
  } else if (Fsubs(step, most) > zero) {
    step = most;
  }
  uint32_t reached = 0;
  const int32_t look = LoadS16(base, AiPath(base, path).plan_count_address());
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
    const double from = Fadds(Lfs(base, Load(base, previous) + 24),
                              Lfs(base, AiPathEntry(base, entry).step_address()));
    Stfs(base, AiPathEntry(base, entry).offset_address(), Fadds(from, step));
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
  const double step = Fdivs(Fsubs(Lfs(base, AiPathEntry(base, first).target_offset_address()),
                                  Lfs(base, AiPathEntry(base, first).offset_address())),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(base, kZero);
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const uint32_t before = Load(base, previous);
    const double target = Lfs(base, AiPathEntry(base, entry).target_offset_address());
    const double side = Lfs(base, AiCorner(base, record).direction_address());
    const double limit = Fmuls(side, target);
    const double offset = Lfs(base, AiPathEntry(base, before).offset_address());
    double from =
        Fsubs(Fadds(target, offset), Lfs(base, AiPathEntry(base, before).target_offset_address()));
    if (Fmuls(side, from) < limit) {
      Stfs(base, AiPathEntry(base, entry).offset_address(), Fadds(step, from));
    } else {
      from = Fadds(Lfs(base, AiPathEntry(base, entry).step_address()), offset);
      if (Fmuls(from, Lfs(base, AiCorner(base, record).direction_address())) < limit) {
        Stfs(base, AiPathEntry(base, entry).offset_address(), target);
      } else {
        Stfs(base, AiPathEntry(base, entry).offset_address(), from);
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
  const double step = Fdivs(Fsubs(Lfs(base, AiPathEntry(base, first).target_offset_address()),
                                  Lfs(base, AiPathEntry(base, first).offset_address())),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(base, kZero);
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const uint32_t before = Load(base, previous);
    const double target = Lfs(base, AiPathEntry(base, entry).target_offset_address());
    const double from = Fsubs(Fadds(Lfs(base, AiPathEntry(base, before).offset_address()), target),
                              Lfs(base, AiPathEntry(base, before).target_offset_address()));
    const double offset = Fadds(step, from);
    Stfs(base, AiPathEntry(base, entry).offset_address(), offset);
    const double side = Lfs(base, AiCorner(base, record).direction_address());
    if (Fmuls(side, offset) < Fmuls(side, target)) {
      Stfs(base, AiPathEntry(base, entry).offset_address(), target);
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
  const double scale = Lfs(base, G::four_fifths_address());
  const double step = Fdivs(Fmuls(Fsubs(Lfs(base, AiPathEntry(base, first).target_offset_address()),
                                        Lfs(base, AiPathEntry(base, first).offset_address())),
                                  scale),
                            IntToFloat(span));
  if (!(start < end)) {
    return reached;
  }
  const double zero = Lfs(base, kZero);
  bool more;
  do {
    const uint32_t entry = RingEntry(base, path, int32_t(Load(base, index)));
    const uint32_t before = Load(base, previous);
    const double target = Lfs(base, AiPathEntry(base, entry).target_offset_address());
    const double from =
        Fmadds(Fsubs(target, Lfs(base, AiPathEntry(base, before).target_offset_address())), scale,
               Lfs(base, AiPathEntry(base, before).offset_address()));
    Stfs(base, AiPathEntry(base, entry).offset_address(), from);
    const double side = Lfs(base, AiCorner(base, record).direction_address());
    if (Fmuls(side, from) < Fmuls(side, target)) {
      Stfs(base, AiPathEntry(base, entry).offset_address(), Fadds(step, from));
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
  for (int32_t k = 0; k < LoadS16(base, AiPath(base, path).corner_count_address()); k++) {
    const uint32_t record = path + 5164 + 44 * uint32_t(k);
    if (AiCorner(base, record).data().address == 0 || AiCorner(base, record).table().address == 0) {
      continue;
    }
    const int32_t offset = LoadS16(base, AiCorner(base, record).entry_offset_address());
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
                    uint32_t(LoadS16(base, AiCorner(base, record).data().address + 54)) +
                        uint32_t(LoadS16(base, AiCorner(base, record).entry_offset_address())));
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
  if (LoadS16(base, AiPath(base, path).corner_count_address()) > 0) {
    return PlanCorners(base, path, entry, frame + 84, frame + 80, frame - 144);
  }
  ContinueLine(base, path, entry, frame + 84, frame + 80, frame - 112);
  return path;
}

// --- The path's branch kinds and lines ---

namespace {
// The race data's flags ([0x826C5B2C] + 20).
uint32_t RaceFlags(uint8_t* base) { return G::race_data(base).branch_flags(); }
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
  const uint32_t section = TrackPosition(base, identity).route_section();
  if (int16_t(section) < 0) {
    return false;
  }
  const uint32_t table = RG::route_file(base).sections();
  *number = guest::LoadU8(base, Load(base, table + (section << 2)) + 3);
  return true;
}
}  // namespace

uint32_t BranchMasksFor(uint8_t* base, uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_8213D378 AiPath::BranchMasksFor");
  const uint32_t flags = RaceFlags(base);
  if (!BranchFlags(flags)) {
    return G::branch_mask_lists_address();
  }
  switch (Load(base, kind)) {
    case 1:
    case 6:
    case 7:
      if ((flags & 0xC) != 0) {
        return G::branch_mask_lists_address() + 16;
      }
      [[fallthrough]];
    case 2:
    case 8:
      if ((flags & 0x10) != 0) {
        return G::branch_mask_lists_address() + 32;
      }
      [[fallthrough]];
    case 3:
    case 9:
      if ((flags & 0x20) != 0) {
        return G::branch_mask_lists_address() + 48;
      }
      return G::branch_mask_lists_address();
    default:
      return G::branch_mask_lists_address();
  }
}

uint32_t LeaderBranchKind(uint8_t* base, uint32_t path, uint32_t kind, uint32_t best) {
  ITC_NATIVE_RUNNING("sub_8213D1F0 AiPath::LeaderBranchKind");
  const uint32_t flags = RaceFlags(base);
  if (!BranchFlags(flags)) {
    return path;
  }
  const uint32_t driver = AiPath(base, path).driver().address;
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  if (vehicle == 0) {
    return 0;
  }
  if (int32_t(Load(base, kind)) != -1 &&
      !(Lfs(base, AiDriver(base, driver).player_gap_address()) > Lfs(base, best))) {
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
  Stfs(base, best, Lfs(base, AiDriver(base, driver).player_gap_address()));
  return identity;
}

uint64_t FindSideSection(uint8_t* base, uint32_t path, uint32_t out, uint32_t point,
                         uint32_t masks, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213D550 AiPath::FindSideSection");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(Load(base, AiPath(base, path).driver().address + 160));
  const uint32_t position = frame + 80;
  track_route::ClampPosition(base, position, TrackPosition(base, identity).route_position());
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
  if (!(LoadS16(base, AiPath(base, path).plan_count_address()) > 1)) {
    return;
  }
  const double zero = Lfs(base, kZero);
  do {
    const uint32_t entry = RingEntry(base, path, i);
    const double target = Lfs(base, AiPathEntry(base, previous).target_offset_address());
    const double before = Lfs(base, AiPathEntry(base, previous).offset_address());
    const double step = Lfs(base, AiPathEntry(base, entry).step_address());
    Stfs(base, AiPathEntry(base, entry).offset_address(), target);
    const double from = Fadds(step, before);
    KeepInRange(base, entry, target);
    LimitStep(base, path, entry, from, zero);
    guest::StoreU32(base, frame + 84, 1);
    ClampOffset(base, path, entry, frame + 80, frame + 84, frame - 96);
    i = int32_t(uint32_t(i) + 1);
    previous = entry;
    guest::StoreU32(base, frame + 80, uint32_t(i));
  } while (i < LoadS16(base, AiPath(base, path).plan_count_address()));
}

uint32_t FollowLeaderBranch(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_8213EB48 AiPath::FollowLeaderBranch");
  const uint32_t driver = AiPath(base, path).driver().address;
  uint32_t r3 = path;
  int32_t kind = -1;
  if ((AiDriver(base, driver).flags() & 0x4) != 0) {
    const uint32_t object = AiDriver(base, driver).profile().address;
    const uint32_t flags = RaceFlags(base);
    if (BranchFlags(flags) && object != 0) {
      const int32_t mode = LoadS16(base, AiProfile(base, object).branch_kind_address());
      if ((mode < 4 || mode > 5) && !(Lfs(base, AiDriver(base, driver).player_gap_address()) >
                                      Lfs(base, G::leader_gap_address()))) {
        const int32_t count = int32_t(track_collision::RaceRacerCount(base));
        uint32_t found = 0;
        for (int32_t i = 0; i < count; i++) {
          const uint32_t racer = Load(base, G::race(base).racers_address() + 4 * uint32_t(i));
          const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
          if (int32_t(index) < 0) {
            continue;
          }
          const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
          if (vehicle != 0 && (Vehicle(base, vehicle).car_flags() & 0x81) != 0) {
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
  if (int32_t(AiPath(base, path).leader_branch_kind()) != kind) {
    AiPath(base, path).set_leader_branch_kind(uint32_t(kind));
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
  if (AiDriver(base, b).vehicle().address == 0) {
    ResetOtherRecord(base, record, Lfs(base, G::other_gap_address()), Lfs(base, kZero),
                     Lfs(base, kOne), Lfs(base, G::minus_one_address()));
    return b64;
  }
  const uint32_t mine =
      vehicle_accessors::VehicleIdentity(AiDriver(base, driver).vehicle().address);
  const uint32_t theirs = vehicle_accessors::VehicleIdentity(AiDriver(base, b).vehicle().address);
  const double distance =
      track_route::Distance(base, track_route::kRoute, theirs + 4, mine + 4);
  const double length = Fadds(Lfs(base, AiDriver(base, b).car_half_length_address()),
                              Lfs(base, AiDriver(base, driver).car_half_length_address()));
  const double ahead = Fadds(Lfs(base, AiDriver(base, driver).slice_offset_address()), distance);
  Stfs(base, AiOther(base, record).speed_address(),
       Lfs(base, AiDriver(base, b).speed_kmh_address()));
  Stfs(base, frame + 88, Fsubs(ahead, Lfs(base, AiDriver(base, b).slice_offset_address())));
  Stfs(base, frame + 80, Fmuls(length, Lfs(base, G::four_fifths_address())));
  const bool both =
      (AiDriver(base, driver).flags() & 0x4) != 0 && (AiDriver(base, b).flags() & 0x4) != 0;
  AiOther(base, record).set_flag_a(both ? 1 : 0);
  AiOther(base, record).set_flag_b(0);
  UpdateOther(base, record, frame + 88, frame + 80);
  const uint64_t back64 = uint64_t(AiDriver(base, driver).index()) * 40 + b64 + 172;
  const uint32_t back = uint32_t(back64);
  MirrorOther(base, back, record, driver + 100);
  if (AiDriver(base, driver).lane() == AiDriver(base, b).lane()) {
    UpdateOtherTimer(base, record, other, frame + 80);
    UpdateOtherTimer(base, back, driver, frame + 80);
  }
  return back64;
}

uint64_t UpdateAll(uint8_t* base, uint32_t frame, DriverUpdate update, void* user) {
  ITC_NATIVE_RUNNING("sub_82139628 AiDrivers::UpdateAll");
  const uint32_t count = track_collision::RaceRacerCount(base);
  uint64_t r3 = count;
  if (Load(base, G::drivers_address()) == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  const auto index_of = [&](uint32_t i) {
    return MinimapRacerEntry(base, Load(base, G::race(base).racers_address() + 4 * i))
        .vehicle_index();
  };
  const auto driver_of = [&](uint32_t index) {
    return uint64_t(index) * AiDriver::kSize + Load(base, G::drivers_address());
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
// A skill percentage (a halfword) kept in 0 to 100.
int32_t Percentage(uint8_t* base, uint32_t at) {
  const int32_t value = LoadS16(base, at);
  if (value < 0) {
    return 0;
  }
  return value > 100 ? 100 : value;
}
void OrFlags(uint8_t* base, uint32_t record, uint16_t bits) {
  AiOther(base, record).set_action_flags(uint16_t(AiOther(base, record).action_flags() | bits));
}
uint16_t Truncated(double value) {
  return uint16_t(strings_and_font::steps::Fctiwz(value));
}
// PlanReaction's +12 from a percentage, with the flag 0x2.
void ReactionScale(uint8_t* base, uint32_t record, int32_t percentage) {
  const double scale = IntToFloat(percentage);
  OrFlags(base, record, 0x2);
  Stfs(base, AiOther(base, record).factor_address(),
       Fmuls(scale, Lfs(base, G::hundredth_address())));
}
}  // namespace

void PlanReaction(uint8_t* base, uint32_t record, uint32_t skill, uint32_t rate, uint32_t flag,
                  uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821456F8 AiOther::PlanReaction");
  const uint32_t state = uint32_t(LoadS16(base, AiOther(base, record).state_address()));
  Stfs(base, AiOther(base, record).factor_address(), Lfs(base, kOne));
  if (state < 1) {
    AiOther(base, record).set_ring_index(0xFFFF);
    AiOther(base, record).set_action_flags(0);
    return;
  }
  if (state == 1) {
    const double gap = Lfs(base, G::react_gap_address());
    if (int32_t(flag) == 0 &&
        Lfs(base, AiOther(base, record).speed_address()) > Lfs(base, G::react_speed_address()) &&
        !(Lfs(base, AiOther(base, record).closing_rate_address()) >
          Lfs(base, G::tenth_address())) &&
        Lfs(base, AiOther(base, record).gap_address()) < gap) {
      AiOther(base, record).set_ring_index(0xFFFF);
      return;
    }
    if (Lfs(base, AiOther(base, record).gap_address()) > gap &&
        !(Lfs(base, AiOther(base, record).closing_rate_address()) <
          Lfs(base, G::react_rate_address()))) {
      AiOther(base, record).set_ring_index(1);
      OrFlags(base, record, 0x1);
      return;
    }
    const double meet = Lfs(base, AiOther(base, record).ticks_to_meet_address());
    if (meet < Lfs(base, kZero)) {
      AiOther(base, record).set_ring_index(0xFFFF);
      return;
    }
    OrFlags(base, record, 0x1);
    const int32_t percentage = Percentage(base, skill + 16);
    const double by = Lfs(base, rate);
    double ahead = Fadds(IntToFloat(percentage), Lfs(base, G::hundred_address()));
    ahead = Fmuls(ahead, by);
    ahead = Fmuls(ahead, meet);
    AiOther(base, record)
        .set_ring_index(Truncated(Fmuls(ahead, Lfs(base, G::hundredth_address()))));
    return;
  }
  if (state < 3) {
    AiOther(base, record).set_ring_index(1);
    if (Percentage(base, skill + 14) > 0) {
      return;
    }
    OrFlags(base, record, 0x1);
    return;
  }
  if (state != 3) {
    return;
  }
  if (int32_t(mode) == 1 && int32_t(flag) == 0 &&
      LoadS16(base, AiOther(base, record).timer_address()) > 0) {
    AiOther(base, record).set_ring_index(2);
    OrFlags(base, record, 0x10);
    return;
  }
  if (Lfs(base, AiOther(base, record).gap_address()) > Lfs(base, G::ten_address())) {
    const double meet = Lfs(base, AiOther(base, record).ticks_to_meet_address());
    if (meet < Lfs(base, kZero)) {
      AiOther(base, record).set_ring_index(0xFFFF);
      return;
    }
    AiOther(base, record).set_ring_index(Truncated(Fmuls(Lfs(base, rate), meet)));
    if (AiOther(base, record).flag_b() != 1) {
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
    AiOther(base, record).set_ring_index(1);
    OrFlags(base, record, 0x1);
    return;
  }
  AiOther(base, record).set_ring_index(0xFFFF);
  if (AiOther(base, record).flag_b() != 1) {
    return;
  }
  const int32_t percentage = Percentage(base, skill + 14);
  if (!(percentage > 0)) {
    return;
  }
  AiOther(base, record).set_ring_index(1);
  ReactionScale(base, record, percentage);
}

uint64_t ReactToOthers(uint8_t* base, uint32_t driver, uint32_t count, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82145CA8 AiDriver::ReactToOthers");
  guest::StoreU32(base, frame + 84, AiDriver(base, driver).index());
  const uint32_t file = RG::route_file(base).address;
  const uint64_t skill = Of(base, frame + 84) + 6048;
  const uint32_t racers = track_collision::RaceRacerCount(base);
  uint64_t r3 = racers;
  const double speed = Lfs(base, AiDriver(base, driver).speed_kmh_address());
  double rate = Fmuls(Lfs(base, G::tick_address()), Lfs(base, G::per_kmh_address()));
  const double length = Lfs(base, TrackRouteFile(base, file).slice_length_address());
  const double zero = Lfs(base, kZero);
  rate = Fdivs(rate, length);
  Stfs(base, frame + 80, Fmuls(rate, speed));
  uint32_t listed = 0;
  if (int32_t(racers) > 0) {
    const double ten = Lfs(base, G::ten_address());
    const double far = Lfs(base, G::react_gap_address());
    const double one = Lfs(base, kOne);
    uint32_t slot = driver + 452;
    for (uint32_t i = 0; i < racers; i++) {
      const uint32_t other =
          MinimapRacerEntry(base, Load(base, G::race(base).racers_address() + 4 * i))
              .vehicle_index();
      guest::StoreU32(base, frame + 88, other);
      if (int32_t(AiDriver(base, driver).index()) == int32_t(other)) {
        continue;
      }
      const uint64_t record64 = uint64_t(other) * 40 + driver + 172;
      const uint32_t record = uint32_t(record64);
      AiOther(base, record).set_action_flags(0);
      const uint32_t keeps = KeepsGapFrom(base, driver, frame + 88);
      r3 = keeps;
      bool react = int32_t(keeps) != 0;
      if (!react) {
        const uint32_t bit = (other & 0x20) != 0 ? 0 : 1u << (other & 0x3F);
        if ((uint32_t(LoadS16(base, AiDriver(base, driver).racer_mask_address())) & bit) != 0) {
          Stfs(base, AiOther(base, record).factor_address(), one);
          const int32_t state = int32_t(AiDriver(base, driver).state());
          if (state == 2) {
            const double gap = Lfs(base, AiOther(base, record).gap_address());
            if (gap < zero) {
              const double slices =
                  Fdivs(gap, Lfs(base, TrackRouteFile(base, file).slice_length_address()));
              AiOther(base, record).set_action_flags(4);
              AiOther(base, record).set_ring_index(Truncated(Fneg(slices)));
            }
          } else if (state == 3) {
            const double gap = Lfs(base, AiOther(base, record).gap_address());
            AiOther(base, record).set_action_flags(1);
            if (gap < far && gap > ten) {
              AiOther(base, record)
                  .set_ring_index(Truncated(Fdivs(
                      Fabs(gap), Lfs(base, TrackRouteFile(base, file).slice_length_address()))));
            } else {
              AiOther(base, record).set_ring_index(1);
            }
          } else {
            react = true;
          }
        }
      }
      if (react) {
        const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
        const uint32_t flag7 = vehicle != 0 ? (Vehicle(base, vehicle).car_flags() >> 2) & 1 : 0;
        const uint32_t flag6 = (AiDriver(base, driver).flags() >> 2) & 1;
        PlanReaction(base, record, uint32_t(skill), frame + 80, flag6, flag7);
        r3 = record64;
      }
      if (AiOther(base, record).action_flags() != 0) {
        const int32_t at = LoadS16(base, AiOther(base, record).ring_index_address());
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
// The race's racer i's record ([0x826D82F4] + 2308).
uint32_t RacerAt(uint8_t* base, uint32_t i) {
  return Load(base, G::race(base).racers_address() + 4 * i);
}
}  // namespace

uint64_t StartGroup(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213A000 AiDrivers::StartGroup");
  const uint32_t count = track_collision::RaceRacerCount(base);
  uint64_t r3 = count;
  StoreConstant(base, frame + 88, G::minus_hundred_address());
  uint32_t mask = 0;
  guest::StoreU32(base, frame + 80, mask);
  guest::StoreU32(base, frame + 84, 0xFFFFFFFFu);
  if (Load(base, G::drivers_address()) == 0 || !(int32_t(count) > 0)) {
    return r3;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(base, i);
    const uint32_t flags = MinimapRacerEntry(base, racer).flags();
    if ((flags & 0x10) == 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    if ((flags & 0xC) == 0) {
      mask |= Slw(1, index);
    }
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + Load(base, G::drivers_address());
    r3 = LeaderBranchKind(base, uint32_t(driver + 512), frame + 84, frame + 88);
  }
  guest::StoreU32(base, frame + 80, mask);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t racer = RacerAt(base, i);
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    const uint32_t flags = MinimapRacerEntry(base, racer).flags();
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + Load(base, G::drivers_address());
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
  uint32_t table = Load(base, G::drivers_address());
  StoreConstant(base, frame + 92, G::minus_hundred_address());
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
    if ((MinimapRacerEntry(base, racer).flags() & 0x10) == 0) {
      continue;
    }
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + table;
    mask |= Slw(1, index);
    r3 = LeaderBranchKind(base, uint32_t(driver + 512), frame + 88, frame + 92);
    guest::StoreU32(base, list + 4 * listed, index);
    listed++;
    table = Load(base, G::drivers_address());
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
        const uint32_t a = Load(base, at) * AiDriver::kSize + table;
        const uint32_t b = Load(base, next) * AiDriver::kSize + table;
        uint32_t flags = AiDriver(base, a).vehicle().car_flags() & 0x81;
        r3 = flags;
        if (flags != 0) {
          continue;
        }
        flags = AiDriver(base, b).vehicle().car_flags() & 0x81;
        r3 = flags;
        if (flags == 0 && Lfs(base, AiDriver(base, a).player_gap_address()) >
                              Lfs(base, AiDriver(base, b).player_gap_address())) {
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
    const uint32_t index = MinimapRacerEntry(base, racer).vehicle_index();
    if (int32_t(index) < 0) {
      continue;
    }
    const uint32_t flags = MinimapRacerEntry(base, racer).flags();
    const uint64_t driver = uint64_t(index) * AiDriver::kSize + table;
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
    table = Load(base, G::drivers_address());
  }
  return r3;
}

// --- Reading the vehicle ---

namespace {
// The route slice record of a route position (the halfwords at `position`:
// the section, then the slice): the section's record (the route file's +12
// table) + 124 + 84 x the slice.
uint32_t SliceRecordAt(uint8_t* base, uint32_t position) {
  const uint32_t table = RG::route_file(base).sections();
  const uint32_t section = guest::LoadU16(base, position + 0);
  const int32_t slice = LoadS16(base, position + 2);
  return uint32_t(slice) * 84 + Load(base, table + (section << 2)) + 124;
}
}  // namespace

uint64_t ReadVehicle(uint8_t* base, uint32_t driver, uint32_t slice) {
  ITC_NATIVE_RUNNING("sub_821467F0 AiDriver::ReadVehicle");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(AiDriver(base, driver).vehicle().address);
  const uint32_t body = AiDriver(base, driver).vehicle().body().address;
  const uint32_t spin = vehicle_accessors::RigidBodyAngularVelocity(body);
  for (uint32_t i = 0; i < 16; i += 4) {  // two ld/std pairs: the bits
    CopyWord(base, TrackPosition(base, identity).point_x_address() + i, driver + 80 + i);
  }
  Stfs(base, AiDriver(base, driver).point_w_address(), Lfs(base, kOne));
  Stfs(base, AiDriver(base, driver).speed_kmh_address(),
       double(Vehicle_GetSpeedKmh(base, AiDriver(base, driver).vehicle().address)));
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  const double yaw =
      Fmuls(Lfs(base, Vehicle(base, vehicle).inverse_dt_address()), Lfs(base, spin + 4));
  StoreConstant(base, AiDriver(base, driver).speed_limit_address(), G::five_hundred_address());
  StoreConstant(base, AiDriver(base, driver).speed_limit_other_address(),
                G::five_hundred_address());
  Stfs(base, AiDriver(base, driver).yaw_rate_address(),
       Fmuls(yaw, Lfs(base, G::yaw_scale_address())));
  const uint32_t orientation = vehicle_accessors::RigidBodyOrientation(body);
  const double x = Lfs(base, orientation + 0);
  const double zero = Lfs(base, kZero);
  if (x == zero && Lfs(base, orientation + 8) == zero) {
    Stfs(base, AiDriver(base, driver).heading_address(), zero);
  } else {
    Stfs(base, AiDriver(base, driver).heading_address(),
         double(float(c_runtime::Atan2(x, Lfs(base, orientation + 8)))));
  }
  const uint32_t own = AiDriver(base, driver).vehicle().address;
  CopyWord(base, own + 320, AiDriver(base, driver).steer_limit_a_address());
  Stfs(base, AiDriver(base, driver).steer_limit_b_address(), Fneg(Lfs(base, own + 316)));
  if (slice != 0) {
    const double dx = Fsubs(Lfs(base, AiDriver(base, driver).point_x_address()),
                            Lfs(base, RouteSlice(base, slice).center_x_address()));
    const double dz = Fsubs(Lfs(base, AiDriver(base, driver).point_z_address()),
                            Lfs(base, RouteSlice(base, slice).center_z_address()));
    const double across = Fmuls(dx, Lfs(base, RouteSlice(base, slice).side_x_address()));
    Stfs(base, AiDriver(base, driver).road_offset_address(),
         Fmadds(dz, Lfs(base, RouteSlice(base, slice).side_z_address()), across));
    const double side = Lfs(base, RouteSlice(base, slice).side_x_address());
    const double direction = double(
        float(c_runtime::Atan2(Lfs(base, RouteSlice(base, slice).side_z_address()), Fneg(side))));
    Stfs(base, AiDriver(base, driver).road_angle_address(),
         Fsubs(direction, Lfs(base, AiDriver(base, driver).heading_address())));
    WrapAngle(base, driver + 148);
    UpdateHalfWidth(base, driver);
  } else {
    Stfs(base, AiDriver(base, driver).road_offset_address(), zero);
  }
  const uint32_t record =
      SliceRecordAt(base, TrackPosition(base, identity).route_position_address());
  if (record != 0) {
    const double dx = Fsubs(Lfs(base, AiDriver(base, driver).point_x_address()),
                            Lfs(base, RouteSlice(base, record).center_x_address()));
    const double dz = Fsubs(Lfs(base, AiDriver(base, driver).point_z_address()),
                            Lfs(base, RouteSlice(base, record).center_z_address()));
    const double along = Fmuls(dx, Lfs(base, RouteSlice(base, record).forward_x_address()));
    Stfs(base, AiDriver(base, driver).slice_offset_address(),
         Fmadds(dz, Lfs(base, RouteSlice(base, record).forward_z_address()), along));
  } else {
    Stfs(base, AiDriver(base, driver).slice_offset_address(), zero);
  }
  const uint64_t r3 = UpdateAgainstPlayer(base, driver);
  AiDriver(base, driver).set_watched(0xFFFFFFFFu);
  return r3;
}

uint64_t ReadVehicleOwnSlice(uint8_t* base, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_821469D0 AiDriver::ReadVehicleOwnSlice");
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(AiDriver(base, driver).vehicle().address);
  return ReadVehicle(base, driver,
                     SliceRecordAt(base, TrackPosition(base, identity).route_position_address()));
}

// --- The path's branch masks ---

uint64_t BranchMasks(uint8_t* base, uint32_t path, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8213E500 AiPath::BranchMasks");
  // The lists' address as the original's lis and addi leave it (sign-extended).
  const uint64_t lists = uint64_t(int64_t(int32_t(G::branch_mask_lists_address())));
  const uint32_t driver = AiPath(base, path).driver().address;
  const uint32_t object = AiDriver(base, driver).profile().address;
  const uint32_t vehicle = AiDriver(base, driver).vehicle().address;
  uint32_t kind = AiPath(base, path).branch_kind();
  if (int32_t(kind) == -1) {
    kind = AiPath(base, path).leader_branch_kind();
  }
  if (int32_t(kind) != -1) {
    return uint64_t((kind << 4) & 0xFFFFFFF0u) + lists;
  }
  if (object != 0) {
    guest::StoreU32(base, frame + 80,
                    uint32_t(int32_t(int8_t(AiProfile(base, object).branch_byte()))));
    return uint64_t(int64_t(int32_t(BranchMasksFor(base, frame + 80))));
  }
  if (vehicle != 0) {
    const uint32_t flags = Vehicle(base, vehicle).car_flags();
    if ((flags & 0x81) == 0) {
      if ((flags & 0x4) == 0) {
        return lists;
      }
      const uint32_t other = Vehicle(base, vehicle).car();
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
  switch (G::race_data(base).route_course()) {
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
  const uint32_t table = RG::route_file(base).sections();
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
  const uint32_t driver = AiPath(base, path).driver().address;
  const uint32_t position = driver + 4;
  if (guest::LoadU16(base, at + 0) == guest::LoadU16(base, position + 0) &&
      guest::LoadU16(base, at + 2) == guest::LoadU16(base, position + 2)) {
    const int32_t lane = int32_t(AiPathEntry(base, entry).lane());
    if (lane < 0 || lane == LoadS16(base, AiDriver(base, driver).lane_address())) {
      return 0;
    }
  }
  const uint32_t record = AiPathEntry(base, entry).slice_record().address;
  const double dz =
      Fsubs(Lfs(base, point + 8), Lfs(base, RouteSlice(base, record).center_z_address()));
  const double dx =
      Fsubs(Lfs(base, point + 0), Lfs(base, RouteSlice(base, record).center_x_address()));
  const double length = Lfs(base, RG::route_file(base).slice_length_address());
  const double along = Fmadds(Lfs(base, RouteSlice(base, record).forward_x_address()), dx,
                              Fmuls(Lfs(base, RouteSlice(base, record).forward_z_address()), dz));
  if (along > length) {
    return 1;
  }
  if (along < Fneg(length)) {
    AiPath(base, path).set_entry_count(0);
    return 1;
  }
  const double half =
      Fmuls(Lfs(base, RouteSlice(base, record).width_address()), Lfs(base, G::half_address()));
  const double across = Fmadds(Lfs(base, RouteSlice(base, record).side_x_address()), dx,
                               Fmuls(Lfs(base, RouteSlice(base, record).side_z_address()), dz));
  Stfs(base, frame + 80, across);
  const double minus_half = Fneg(half);
  if (across > half || across < minus_half) {
    if (across > Fadds(Lfs(base, RouteSlice(base, record).margin_right_address()), half)) {
      return 1;
    }
    if (across < Fsubs(minus_half, Lfs(base, RouteSlice(base, record).margin_left_address()))) {
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
  if (!(int32_t(AiPathEntry(base, entry).lane()) < 0)) {
    lane = int16_t(track_route::LaneAt(base, track_route::kRoute, point, at));
    if (int32_t(AiPathEntry(base, entry).lane()) != lane) {
      return 1;
    }
  }
  guest::StoreU16(base, position + 0, guest::LoadU16(base, at + 0));
  guest::StoreU16(base, position + 2, guest::LoadU16(base, at + 2));
  AiDriver(base, driver).set_lane(uint16_t(lane));
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
  if (!(AiPath(base, path).first() < 32768) ||
      !(LoadS16(base, AiPath(base, path).entry_count_address()) > 0)) {
    return true;
  }
  const uint32_t before = AiPath(base, path).entry_count();
  const auto first = [&]() {
    return path + 16 + (uint32_t(LoadS16(base, AiPath(base, path).first_address())) << 6);
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
    Stfs(base, AiPathEntry(base, entry).x_address(), zero);
    AiPathEntry(base, entry).set_route_position(none);
    Stfs(base, AiPathEntry(base, entry).y_address(), zero);
    AiPathEntry(base, entry).set_lane(0xFFFFFFFFu);
    Stfs(base, AiPathEntry(base, entry).z_address(), zero);
    AiPathEntry(base, entry).set_slice_record(0);
    Stfs(base, AiPathEntry(base, entry).w_address(), one);
    Stfs(base, AiPathEntry(base, entry).offset_address(), zero);
    Stfs(base, AiPathEntry(base, entry).target_offset_address(), zero);
    for (uint32_t at = 28; at <= 44; at += 4) {
      Stfs(base, entry + at, zero);
    }
    const int16_t count = int16_t(AiPath(base, path).entry_count() - 1);
    AiPath(base, path).set_entry_count(uint16_t(count));
    if (!(count > 0) || --tries < 0) {
      *r3 = ResetPath(base, path);
      return true;
    }
    const int32_t next = LoadS16(base, AiPath(base, path).first_address()) + 1;
    const int16_t wrapped = int16_t(next - (next / 80) * 80);
    AiPath(base, path).set_first(uint16_t(wrapped));
    left = LeftEntry(base, path, point, first(), frame - 160);
    *r3 = left;
    if (left == 0) {
      break;
    }
  }
  if (!(LoadS16(base, AiPath(base, path).entry_count_address()) > 0)) {
    return true;
  }
  const uint32_t entry = first();
  guest::StoreU16(base, frame + 80, uint16_t(before - AiPath(base, path).entry_count()));
  uint32_t ignored = 0;
  AdvanceCorners(base, path, frame + 80, &ignored);  // can't fail: checked above
  DropPassedSections(base, path);
  *r3 = path;
  const uint32_t record = AiPathEntry(base, entry).slice_record().address;
  const double dx =
      Fsubs(Lfs(base, point + 0), Lfs(base, RouteSlice(base, record).center_x_address()));
  const double dz =
      Fsubs(Lfs(base, point + 8), Lfs(base, RouteSlice(base, record).center_z_address()));
  const double offset = Fmadds(dz, Lfs(base, RouteSlice(base, record).side_z_address()),
                               Fmuls(dx, Lfs(base, RouteSlice(base, record).side_x_address())));
  Stfs(base, AiPathEntry(base, entry).offset_address(), offset);
  Stfs(base, AiPathEntry(base, entry).x_address(),
       Fmadds(Lfs(base, RouteSlice(base, record).side_x_address()), offset,
              Lfs(base, RouteSlice(base, record).center_x_address())));
  CopyWord(base, RouteSlice(base, record).center_y_address(), AiPathEntry(base, entry).y_address());
  Stfs(base, AiPathEntry(base, entry).z_address(),
       Fmadds(Lfs(base, RouteSlice(base, record).side_z_address()), offset,
              Lfs(base, RouteSlice(base, record).center_z_address())));
  return true;
}

// --- The path's update ---


uint64_t Extend(uint8_t* base, uint32_t path, uint32_t point, uint32_t frame,
                const PathCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8213ECF8 AiPath::Extend");
  const uint32_t file = RG::route_file(base).address;
  const double length = Lfs(base, TrackRouteFile(base, file).slice_length_address());
  const double slices = Fdivs(Lfs(base, G::other_gap_address()), length);
  Stfs(base, frame + 92, Fmuls(length, Lfs(base, G::hundredth_address())));
  Stfs(base, frame + 96, Fmuls(length, Lfs(base, G::tenth_address())));
  guest::StoreU32(base, frame + 84, uint32_t(strings_and_font::steps::Fctiwz(slices)));
  int32_t ahead = int32_t(Load(base, frame + 84));
  if (!(ahead < 80)) {
    ahead = 80;
  }
  const int32_t count = LoadS16(base, AiPath(base, path).entry_count_address());
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
    previous = ring_at(LoadS16(base, AiPath(base, path).first_address()) + (count - 1));
    guest::StoreU16(base, frame + 84, AiPathEntry(base, previous).section());
    guest::StoreU16(base, frame + 86, uint16_t(AiPathEntry(base, previous).slice() + 1));
    guest::StoreU16(base, frame + 80, uint16_t(AiPathEntry(base, previous).lane()));
  } else {
    r3 = FindSideSection(base, path, frame + 84, point, uint32_t(masks), frame - 208);
    if (int32_t(AiPath(base, path).mode()) == 1) {
      guest::StoreU16(base, frame + 80,
                      guest::LoadU16(base, AiPath(base, path).driver().address + 8));
    } else {
      guest::StoreU16(base, frame + 80, 0xFFFF);
    }
  }
  if (LoadS16(base, AiPath(base, path).entry_count_address()) < limit) {
    guest::StoreU16(base, frame + 88, 0xFFFF);
    guest::StoreU16(base, frame + 90, 0xFFFF);
    const double one = Lfs(base, kOne);
    const double zero = Lfs(base, kZero);
    for (;;) {
      const uint32_t entry = ring_at(LoadS16(base, AiPath(base, path).first_address()) +
                                     LoadS16(base, AiPath(base, path).entry_count_address()));
      const uint32_t shift = entry + 28;
      const uint32_t none = Load(base, frame + 88);
      Stfs(base, AiPathEntry(base, entry).x_address(), zero);
      AiPathEntry(base, entry).set_lane(0xFFFFFFFFu);
      Stfs(base, AiPathEntry(base, entry).w_address(), one);
      AiPathEntry(base, entry).set_route_position(none);
      Stfs(base, AiPathEntry(base, entry).target_offset_address(), zero);
      AiPathEntry(base, entry).set_slice_record(0);
      Stfs(base, AiPathEntry(base, entry).y_address(), zero);
      Stfs(base, AiPathEntry(base, entry).z_address(), zero);
      Stfs(base, AiPathEntry(base, entry).offset_address(), zero);
      Stfs(base, shift, zero);
      for (uint32_t at = 32; at <= 44; at += 4) {
        Stfs(base, entry + at, zero);
      }
      const uint32_t normalizer = AiPath(base, path).normalizer();
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
      guest::StoreU16(base, frame + 84, AiPathEntry(base, entry).section());
      guest::StoreU16(base, frame + 86, uint16_t(AiPathEntry(base, entry).slice() + 1));
      const auto add_section = [&](uint16_t section) {
        const uint32_t at =
            uint32_t(LoadS16(base, AiPath(base, path).section_count_address()) + 2573) << 1;
        guest::StoreU16(base, path + at, section);
        AiPath(base, path).set_section_count(uint16_t(AiPath(base, path).section_count() + 1));
        r3 = DropOffRouteCorners(base, path);
      };
      if (previous != 0) {
        uint32_t record = AiPathEntry(base, entry).slice_record().address;
        if (AiPathEntry(base, entry).section() == AiPathEntry(base, previous).section()) {
          Stfs(base, shift, zero);
        } else {
          const uint32_t before = AiPathEntry(base, previous).slice_record().address;
          const double dx =
              Fsubs(Lfs(base, RouteSlice(base, before).center_x_address()), Lfs(base, record + 0));
          const double dz =
              Fsubs(Lfs(base, RouteSlice(base, before).center_z_address()), Lfs(base, record + 8));
          Stfs(base, shift, Fmadds(dz, Lfs(base, record + 40), Fmuls(dx, Lfs(base, record + 36))));
          add_section(AiPathEntry(base, entry).section());
          record = AiPathEntry(base, entry).slice_record().address;
        }
        CopyWord(base, record + 24, AiPathEntry(base, entry).range_low_address());
        CopyWord(base, record + 28, AiPathEntry(base, entry).range_high_address());
        CopyWord(base, record + 28, AiPathEntry(base, entry).room_high_address());
        CopyWord(base, AiPathEntry(base, entry).range_low_address(),
                 AiPathEntry(base, entry).room_low_address());
        NarrowToOther(base, entry, previous, frame + 92, shift);
        r3 = NarrowBehind(base, path, entry, previous, frame + 92, frame + 96, frame - 128);
      } else if (AiPath(base, path).entry_count() == 0) {
        const uint32_t record = AiPathEntry(base, entry).slice_record().address;
        const uint16_t section = AiPathEntry(base, entry).section();
        CopyWord(base, record + 24, AiPathEntry(base, entry).range_low_address());
        CopyWord(base, record + 28, AiPathEntry(base, entry).range_high_address());
        CopyWord(base, AiPathEntry(base, entry).range_low_address(),
                 AiPathEntry(base, entry).room_low_address());
        CopyWord(base, AiPathEntry(base, entry).range_high_address(),
                 AiPathEntry(base, entry).room_high_address());
        const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
        const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
        const double offset = Fmadds(dz, Lfs(base, record + 40), Fmuls(dx, Lfs(base, record + 36)));
        Stfs(base, AiPathEntry(base, entry).offset_address(), offset);
        Stfs(base, AiPathEntry(base, entry).x_address(),
             Fmadds(Lfs(base, record + 36), offset, Lfs(base, record + 0)));
        CopyWord(base, record + 4, AiPathEntry(base, entry).y_address());
        Stfs(base, AiPathEntry(base, entry).z_address(),
             Fmadds(Lfs(base, record + 40), offset, Lfs(base, record + 8)));
        add_section(section);
      }
      uint32_t corners_r3 = 0;
      AddCornersAt(base, path, entry, frame - 144, &corners_r3);
      r3 = corners_r3;
      previous = entry;
      const int16_t added = int16_t(AiPath(base, path).entry_count() + 1);
      AiPath(base, path).set_entry_count(uint16_t(added));
      if (!(added < limit)) {
        break;
      }
    }
  }
  if (int32_t(AiPath(base, path).mode()) != 1) {
    ApplyCornerLines(base, path, frame - 112);
    r3 = path;
  }
  const uint32_t driver = AiPath(base, path).driver().address;
  if (driver == 0) {
    return r3;
  }
  uint32_t inside = 0;
  if (LoadS16(base, AiPath(base, path).corner_count_address()) > 0) {
    uint32_t record = path + 5164;
    int32_t i = 0;
    do {
      const uint32_t data = Load(base, record + 0);
      if (data != 0) {
        const int32_t offset = LoadS16(base, record + 10);
        const int32_t look = LoadS16(base, AiPath(base, path).look_ahead_address());
        if (!(look < LoadS16(base, RouteCorner(base, data).start_address()) + offset) &&
            !(look > LoadS16(base, RouteCorner(base, data).exit_address()) + offset)) {
          inside = 1;
          break;
        }
      }
      i++;
      record += 44;
    } while (i < LoadS16(base, AiPath(base, path).corner_count_address()));
  }
  AiDriver(base, driver).set_in_corner(inside);
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
  const uint32_t file = RG::route_file(base).address;
  const uint32_t driver = AiPath(base, path).driver().address;
  const uint32_t point = driver + 80;
  if (!(AiDriver(base, driver).lane() < 32768)) {
    return true;
  }
  FollowLeaderBranch(base, path);
  uint64_t dropped = 0;
  DropPassed(base, path, point, frame - 160, &dropped);  // can't fail: checked above
  *r3 = Extend(base, path, point, frame - 208, calls);
  const int16_t entries = int16_t(AiPath(base, path).entry_count());
  const int32_t count = entries;
  const int32_t reach = strings_and_font::steps::Fctiwz(
      Fdivs(Lfs(base, G::look_ahead_address()),
            Lfs(base, TrackRouteFile(base, file).slice_length_address())));
  const int16_t look = int16_t(AiPath(base, path).look_ahead() - uint16_t(reach) + 1);
  AiPath(base, path).set_plan_count(uint16_t(look));
  if (look > count) {
    AiPath(base, path).set_plan_count(uint16_t(entries));
  }
  if (!(count > 1)) {
    return true;
  }
  const uint32_t own = AiPath(base, path).driver().address;
  if ((Load(base, own + 16) & 0x1) != 0) {
    return true;
  }
  const double speed = Fadds(Lfs(base, own + 100), Lfs(base, G::twenty_address()));
  const double per = Fdivs(Lfs(base, RG::route_file(base).slice_length_address()), speed);
  int32_t span = int32_t(uint32_t(LoadS16(base, AiPath(base, path).plan_count_address())) << 2);
  guest::StoreU32(base, frame + 80, uint32_t(span));
  Stfs(base, AiPath(base, path).largest_step_address(), Fmuls(per, Lfs(base, G::twelve_address())));
  if (span > count) {
    guest::StoreU32(base, frame + 80, uint32_t(count));
  }
  SetRanges(base, path, frame + 80, frame - 128);
  ReactToOthers(base, AiPath(base, path).driver().address, frame + 80, frame - 240);
  const uint32_t mine = AiPath(base, path).driver().address;
  const int32_t first = int32_t(Load(base, mine + 452));
  int32_t ring = -1;
  if (!(first < 0)) {
    ring = LoadS16(base, uint32_t(first) * 40 + mine + 198);
  }
  if (!(ring < 0)) {
    AvoidOthers(base, path, frame - 272);
  }
  if (int32_t(AiPath(base, path).mode()) == 1) {
    FollowTargets(base, path, frame - 128);
  } else {
    PlanLine(base, path, frame - 96);
  }
  RefreshPoints(base, path);
  *r3 = path;
  return true;
}

// --- The drivers' updates ---

bool UpdateDriver(uint8_t* base, uint32_t driver, uint32_t input, uint32_t flag, uint32_t frame,
                  const PathCalls& calls, uint64_t* r3, double* f1) {
  if (!CornersMovable(driver + 512)) {  // AiPath::Update's condition, checked first
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8213B310 AiDriver::Update");
  const uint32_t flags = AiDriver(base, driver).flags();
  if ((flags & 0x2) == 0 || int32_t(Load(base, flag)) == int32_t((flags >> 2) & 1)) {
    if (Load(base, flag) != 0) {
      ResetControl(base, driver, frame - 112);
    } else {
      StartRace(base, driver, frame - 112);
    }
  }
  uint64_t path_r3 = 0;
  UpdatePath(base, driver + 512, frame - 144, calls, &path_r3);  // can't fail: checked above
  *r3 = Drive(base, driver, input, frame - 160, f1);
  return true;
}

bool DriveCar(uint8_t* base, uint32_t input, uint32_t index, uint32_t flag, uint32_t frame,
              const PathCalls& calls, uint64_t* r3, double* f1) {
  const uint32_t table = Load(base, G::drivers_address());
  if (table != 0 && !CornersMovable(uint32_t(uint64_t(index) * AiDriver::kSize + table) + 512)) {
    return false;  // AiPath::Update's condition, checked first
  }
  ITC_NATIVE_RUNNING("sub_8215F678 AiDriver::DriveCar");
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, index);
  const uint32_t identity = vehicle_accessors::VehicleIdentity(vehicle);
  track_route::ClampPosition(base, frame + 88, TrackPosition(base, identity).route_position());
  Vehicle_GetSpeedKmh(base, vehicle);  // not read (its f1 Drive's later)
  if (Vehicle(base, vehicle).automatic() == 0) {
    Vehicle(base, vehicle).set_automatic(1);
  }
  if ((Vehicle(base, vehicle).car_flags() & 0x81) != 0 &&
      Vehicle(base, vehicle).nitrous_on() != 0) {
    Vehicle(base, vehicle).set_nitrous_on(0);
  }
  ClearCarInput(base, input);
  guest::StoreU32(base, frame + 84, flag);
  guest::StoreU32(base, frame + 80, index);
  CarInput(base, input).set_kind(2);
  const uint32_t driver = uint32_t(Of(base, frame + 80));
  uint64_t ignored = 0;
  UpdateDriver(base, driver, input, frame + 84, frame - 112, calls, &ignored, f1);
  *r3 = input;
  return true;
}

uint64_t UpdateIdle(uint8_t* base, uint32_t driver, uint32_t frame, const PathCalls& calls,
                    bool* done) {
  *done = CornersMovable(driver + 512);  // AiPath::Update's condition, checked first
  if (!*done) {
    return 0;
  }
  ITC_NATIVE_RUNNING("sub_821399A8 AiDriver::UpdateIdle");
  if ((AiDriver(base, driver).flags() & 0x2) != 0) {
    ResetToMode0(base, driver, frame - 128);
  }
  uint64_t r3 = 0;
  UpdatePath(base, driver + 512, frame - 144, calls, &r3);
  const double zero = Lfs(base, kZero);
  Stfs(base, AiDriver(base, driver).steering_address(), zero);
  AiDriver(base, driver).set_handbrake(0);
  Stfs(base, AiDriver(base, driver).throttle_address(), zero);
  Stfs(base, AiDriver(base, driver).brake_address(), zero);
  return r3;
}

}  // namespace ai_drivers
